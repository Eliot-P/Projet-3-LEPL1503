#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include "fact.h"


// == VARIABLES GLOBALES == //
int fin_de_lecture = 0;
int Threads_de_calculs_finis = 0;


// == ACCESSIBILITÃ‰ == //

int fermer(FILE *entree, FILE *sortie,int i){  //raccourci pour fermer les fichier quand on a une erreur
    fclose(entree);
    fclose(sortie);
    return i;
}

// == FONCTIONS == //

int is_div(unsigned long long number, unsigned long long i){ 
    if (i==0){return -1;}
    return number%i == 0;
}

int AppendNumber(unsigned long long number, Repertoire_t_th *rep){
    rep->liste = (unsigned long long *) realloc(rep->liste,(rep->nbre_elem + 1)*sizeof(unsigned long long));
    if (rep->liste == NULL){
        free(rep->liste);
        free(rep);
        return -1;
    }
    rep->liste[rep->nbre_elem++] = number;
    return 0;
}

Repertoire_t_th *prime_divs_opti(unsigned long long number){
    Repertoire_t_th *arr = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (arr == NULL){
        free(arr);
        return NULL;}
    
    arr->liste = (unsigned long long *) malloc(sizeof(number));
    if (arr->liste == NULL){ 
        free(arr);
        return  NULL;}

    arr->nbre_elem = 0;
    arr->liste[arr->nbre_elem++] = number;
    if (number < 2){ return arr;}
    
    for (unsigned long long i = 2; i < (number/2) +1; i++){
        if (is_div(number,i)==1){
            int premier = 0;
            for (unsigned long long j = 0; j < arr->nbre_elem; j++){
                if (i % arr->liste[j] == 0){
                    premier++;
                    break;
                }
            }
            if (premier == 0){
                if (AppendNumber(i,arr) == -1){return NULL;}
            }
        }
    }
    return arr;
}

///== FONCTIONS PROPRES AUX BUFFERS == ///

void putNumber(Entrepot_Th *tab,unsigned long long number){
    (tab->buffer[tab->putindex]).liste = (unsigned long long *) malloc(sizeof(unsigned long long));
    (tab->buffer[tab->putindex]).liste[0] = number;
    (tab->buffer[tab->putindex]).nbre_elem = 1;
    tab->putindex = (tab->putindex + 1) % tab->size;
    tab->nbre++;
}

void putRepertoire(Repertoire_t_th *rep,Entrepot_Th *tab){
    tab->buffer[tab->putindex] = *rep;
    tab->putindex = (tab->putindex + 1) % tab->size;
    tab->nbre++;
}

Repertoire_t_th takeRepertoire(Entrepot_Th *tab){
    Repertoire_t_th entree = tab->buffer[tab->takeindex];
    tab->takeindex = (tab->takeindex + 1) % tab->size;
    tab->nbre--;
    return entree;
}

void *lecture(void* arg){
    /*
     * lecture prend en argument une structure (de type Lecteur_Th) qui contient les informations 
     * nÃ©cessaires Ã  la lecture des nombres du fichier d'entrÃ© et Ã  leur transmission dans le 1e buffer.
     */
    Lecteur_Th *lect =  (Lecteur_Th *) arg;
    while (fgets(lect->ligne,30,lect->fichier) != NULL) {
        unsigned long long number = atoll(lect->ligne);
        sem_wait(lect->semaphores[0]);  // On attend une place vide  > "firstempty"
        pthread_mutex_lock(lect->flag); // bloque le tableau1 pour ajouter l'Ã©lÃ©ment
    
        putNumber(lect->tableau,number);    // Ajoute number dans le 1e buffer

        pthread_mutex_unlock(lect->flag);
        sem_post(lect->semaphores[1]);  //On ajoute un Ã©lÃ©ment donc on rÃ©veille les "firstfill"
    }
    fin_de_lecture++;

    for (int i = 0; i < (lect->tableau->size)/2; i++){
        sem_wait(lect->semaphores[0]);  // On attend une place vide
        sem_post(lect->semaphores[1]);  //On 'ajoute' un Ã©lÃ©ment donc on rÃ©veille les "firstfill"
    }
    return NULL;
}

void *calcul(void* arg) {
    /*
    * calcul est une fonction qui prend en argument une structure (de type 'Usine_Th') contentant toutes les 
    * informations nÃ©cessaires Ã  la rÃ©ception des nombres, le calcul de leur diviseurs premiers et Ã  
    * la transmission de ces rÃ©sultats dans le 2e buffer.
    */
    Usine_Th *usine = (Usine_Th *) arg;

    while ((fin_de_lecture == 0) || (usine->tabin->nbre != 0)){

        sem_wait(usine->semaphores[1]);  //Attend d'avoir des Ã©lÃ©ments Ã  prendre > 'firstfill'
        
        if ((fin_de_lecture != 0) && (usine->tabin->nbre == 0)){break;}
        
        pthread_mutex_lock(usine->flags[0]);
        Repertoire_t_th entree = takeRepertoire(usine->tabin);
        pthread_mutex_unlock(usine->flags[0]);
        sem_post(usine->semaphores[0]); // signale aux "firstempty" qu'on a pris un Ã©lÃ©ment
        Repertoire_t_th *resultat = prime_divs_opti(entree.liste[0]);
        sem_wait(usine->semaphores[2]);  //Attend "secondempty"  > une place vide ds le 2e tableau
        pthread_mutex_lock(usine->flags[1]);    // ProtÃ¨ge le 2e tableau
        putRepertoire(resultat,usine->tabout);
        pthread_mutex_unlock(usine->flags[1]);
        sem_post(usine->semaphores[3]); //On rÃ©veille les "secondfill" car on a ajoutÃ© un Ã©lÃ©ment
        
        free(entree.liste);
        free(resultat);
    }

    pthread_mutex_lock(usine->flags[1]);    // On protÃ¨ge l'accÃ¨s Ã  la variable globale
    Threads_de_calculs_finis++;
    if (Threads_de_calculs_finis == (usine->tabout->size)/2){sem_post(usine->semaphores[3]);}
    pthread_mutex_unlock(usine->flags[1]);
    return NULL;
}

void *ecriture(void* arg) {
    /*
    * ecriture est une fonction qui prend comme argument une structure (de type 'Imprimerie_Th') contenant
    * toutes les informations nÃ©cessaires Ã  la rÃ©ception des rÃ©sultats dans le 2e tableau et Ã  l'Ã©criture de 
    * ceux-ci dans le fichier de sortie.
    */
    Imprimerie_Th *impr = (Imprimerie_Th *) arg;
    Repertoire_t_th resultat;
    while ((Threads_de_calculs_finis < (impr->tabout->size)/2) || (impr->tabout->nbre != 0)) {

        sem_wait(impr->semaphores[1]);  //On attend des Ã©lÃ©ment dans le tableau2 > "secondfill"
        if ((Threads_de_calculs_finis == (impr->tabout->size)/2) && (impr->tabout->nbre == 0)){break;}

        pthread_mutex_lock(impr->flag); // On protÃ¨ge le tableau

        resultat = takeRepertoire(impr->tabout); 

        pthread_mutex_unlock(impr->flag);
        sem_post(impr->semaphores[0]);  //On rÃ©veille les "secondempty" car on a pris un Ã©lÃ©ment

        if ((resultat.nbre_elem == 1) && (resultat.liste[0]) < 2) {    // Exception oÃ¹ on a number < 2
            fprintf(impr->fichierOut, "Erreur: %llu est infÃ©rieur Ã  2 !", resultat.liste[0]);}

        else {
            for (int i = 0; i < resultat.nbre_elem; i++) {
                fprintf(impr->fichierOut, "%llu ", resultat.liste[i]);}
        }
        fputc('\n',impr->fichierOut);
        free(resultat.liste);
    }
    return NULL;
}


int principale(int N, char *input_file, char *output_file) {
    /*
     * principale est une fonction qui traite de maniÃ¨re concurrente l'Ã©criture des diviseurs premiers des 
     * nombres stockÃ©s sur chaque ligne du fichier 'input_file' dans le fichier 'output_file' en utilisant N
     * thread de calcul.
     * 
     * prÃ©: input_file, output_file != NULL et N >= 0  
     * 
     * post: 'output_file' contient tous les nombres prÃ©sents dans 'input_file' ainsi que la liste leur diviseur 
     * premier. Dans le cas oÃ¹ le nombre est infÃ©rieur Ã  2, un message d'erreur est Ã©crit.
     * 
     * return : 0 -> aucune erreur survenue
     *         -1 -> erreur causÃ©e par fopen()
     *         -2 -> erreur causÃ©e par malloc()
     *         -3 -> erreur causÃ©e par pthread_create()
     *         -4 -> erreur causÃ©e par fclose()
     */

    // == OUVERTURE == //
    FILE *filein = fopen(input_file,"r");
    if (filein == NULL){return -1;}
    FILE *fileout = fopen(output_file,"w");
    if (fileout == NULL){
        fclose(filein);
        return -1;}


    // == PRÃ‰PARATION == //

    // 1) INITIALISATION DES VARIABLES //
    if (N <= 0){N = 4;}

    unsigned long long maximum = 2*N;   // taille de nos tableaux
    char line[30];// permet de stocker chaque ligne du fichier

    pthread_mutex_t firstmutex;  // UtilisÃ© pour protÃ©ger l'accÃ¨s au tableau1
    pthread_mutex_init(&firstmutex,NULL);
    pthread_mutex_t secondmutex; //  UtilisÃ© pour protÃ©ger l'accÃ¨s au tableau2
    pthread_mutex_init(&secondmutex,NULL);

    sem_t firstempty; // nbre de place VIDE dans tableau1
    sem_init(&firstempty,0,maximum);
    sem_t secondemtpy;  // nbre de place VIDE dans tableau2
    sem_init(&secondemtpy,0,maximum);
    sem_t firstfill;    // nbre de place REMPLIE dans le tableau1
    sem_init(&firstfill, 0, 0);
    sem_t secondfill;   // nbre de place REMPLIE dans le tableau2
    sem_init(&secondfill, 0, 0);

    // 2) INITIALISATION DES STRUCTURES //

    Entrepot_Th *tableau1 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau1 == NULL){
        return fermer(filein,fileout,-2);}

    tableau1->buffer = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th) * maximum);
    if (tableau1->buffer == NULL){
        free(tableau1);
        return fermer(filein,fileout,-2);}
    tableau1->putindex = 0;
    tableau1->takeindex = 0;
    tableau1->size = (int) maximum;
    tableau1->nbre = 0;


    Entrepot_Th *tableau2 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau2 == NULL){
        free(tableau1->buffer);
        free(tableau1);
        return fermer(filein,fileout,-2);}
    
    tableau2->buffer = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th) * maximum);
    if (tableau2->buffer == NULL){return fermer(filein,fileout,-2);}
    tableau2->putindex = 0;
    tableau2->takeindex = 0;
    tableau2->size = (int) maximum;
    tableau2->nbre = 0;

    Lecteur_Th *lect = (Lecteur_Th *) malloc(sizeof(struct lecteur));
    if (lect == NULL){
        free(tableau1->buffer);
        free(tableau1);
        free(tableau2->buffer);
        free(tableau2);
        return fermer(filein,fileout,-2);}
    lect->ligne = line;
    lect->fichier = filein;
    lect->tableau = tableau1;
    lect->flag = &firstmutex;
    lect->semaphores[0] = &firstempty;
    lect->semaphores[1] = &firstfill;

    Usine_Th *calc = (Usine_Th *) malloc(sizeof(struct usine));
    if (calc == NULL){
        free(tableau1->buffer);
        free(tableau1);
        free(tableau2->buffer);
        free(tableau2);
        free(lect);
        return fermer(filein,fileout,-2);}
    calc->tabin = tableau1;
    calc->tabout = tableau2;
    calc->semaphores[0] = &firstempty;
    calc->semaphores[1] = &firstfill;
    calc->semaphores[2] = &secondemtpy;
    calc->semaphores[3] = &secondfill;
    calc->flags[0] = &firstmutex;   
    calc->flags[1] = &secondmutex;

    Imprimerie_Th *imp = (Imprimerie_Th *) malloc(sizeof(struct imprimerie));
    if (imp == NULL){
        free(tableau1->buffer);
        free(tableau1);
        free(tableau2->buffer);
        free(tableau2);
        free(lect);
        free(calc);
        return fermer(filein,fileout,-2);
    }
    imp->fichierOut = fileout;  
    imp->tabout = tableau2;     
    imp->semaphores[0] = &secondemtpy;
    imp->semaphores[1] = &secondfill;
    imp->flag = &secondmutex;       


    // 3) INITIALISATION DES THREADS //

    pthread_t lecteur_th;
    int l = pthread_create(&lecteur_th, NULL, &lecture, (void *) lect);
    if (l != 0){ return fermer(filein,fileout,-3);}

    pthread_t calcul_th[N];
    for (int i = 0; i < N; i++){
        int c = pthread_create(&(calcul_th[i]), NULL, &calcul, (void *) calc);
        if (c != 0){return fermer(filein,fileout,-3);}
    }

    pthread_t imprimeur_th;
    int I = pthread_create(&imprimeur_th, NULL, &ecriture, (void *) imp);
    if (I != 0){return fermer(filein,fileout,-3);}

    int ll = pthread_join(lecteur_th,NULL);
    if (ll != 0){ fermer(filein,fileout,-3);}
    for (int k = 0; k < N; k++){
        int cc = pthread_join(calcul_th[k],NULL);
        if (cc != 0){return fermer(filein,fileout,-3);}
    }
    int ii = pthread_join(imprimeur_th,NULL);
    if (ii != 0){ return fermer(filein,fileout,-3);}


    // == FREE == //
    free(tableau1->buffer);
    free(tableau1);
    free(tableau2->buffer);
    free(tableau2);

    free(lect);

    free(calc);

    free(imp);
    // == FERMETURE == //
    
    pthread_mutex_destroy(&firstmutex);
    pthread_mutex_destroy(&secondmutex);
    
    sem_destroy(&firstempty);
    sem_destroy(&firstfill);
    sem_destroy(&secondemtpy);
    sem_destroy(&secondfill);
    
    int x = fclose(filein);
    if (x != 0){
        fclose(fileout);
        return -4;}

    int y = fclose(fileout);
    if (y != 0){return -4;}

    return 0;
}