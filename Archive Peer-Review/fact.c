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


// == ACCESSIBILITÉ == //

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

int  is_prime(unsigned long long number, Repertoire_t_th *tab,pthread_mutex_t *mutex){
    /*
     * retourne 1 si number est un nombre premier, retourne 0 si le nombre n'est pas premier et -1 en cas d'erreur     *
     * La fonction procède de la sorte:
     *      - 1) Elle vérifie si number est divisible par un des nbre premiers contenu dans tab->liste
     *      - 2) Si aucun nbre  premier divise number, on va vérifier si number en est un en appliquant la définition
     *      mathématique (aucun diviseur entre 2 et number/2) et s'il est premier on l'ajoute dans le tableau d'array
     */

    for (unsigned long long i = 0; i < tab->nbre_elem; i++){
        if (tab->liste[i] == number){return 1;}
        if (is_div(number,tab->liste[i])){
            return 0;
        }
    }

    for (unsigned long long i = 2; i < sqrt(number); i++) {
        if (is_div(number, i)){return 0;}
    }
    pthread_mutex_lock(mutex);  // On protège l'accès au répertoire quand on ajoute un nbre premier
    tab->liste = (unsigned long long *) realloc((*tab).liste,((*tab).nbre_elem + 1) * sizeof(unsigned long long));
    if ((*tab).liste == NULL) { return -1; }
    tab->liste[(*tab).nbre_elem++] = number;
    pthread_mutex_unlock(mutex);
    return 1;
}

Repertoire_t_th *prime_divs(unsigned long long number, Repertoire_t_th *tab,pthread_mutex_t *mutex){
    /*  retourne un pointeur vers une structure Reperetoire_t contenant une liste de tout les diviseurs premiers de
     * number et le nbre de diviseurs ou NULL pour une erreur de malloc (le tableau contenant les diviseurs est
     * aloué dynamiquement).
     * La fonction procède de la manière suivante:
     *      - 1) Elle regarde si number est premier, si c'est le cas elle retourne directement (long *) number
     *
     *      - 2) Ensuite elle va regarder tous les chiffres i € [2, number/2]
     *          > Si i divise number et est premier, alors il est ajouté au tableau
     *          > Sinon on passe au prochain nombre
     *
     *  !!! On est "obligé" de retourner une structure car si on retourne simplement un tableau de long on ne connait
     *  pas sa taille et donc il est impossible d'itérer dessus sans avoir d'erreur. Il nous faut une variable qui nous
     *  donne le nombre d'éléments dans le tableau.
     */
    Repertoire_t_th *arr = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (arr == NULL){
        return NULL;}
    arr->liste = (unsigned long long *) malloc(sizeof(number));
    if (arr->liste == NULL){ 
        free(arr);
        return  NULL;}

    arr->nbre_elem = 0;
    arr->liste[arr->nbre_elem++] = number;

    if (number < 2){ return arr;}

    if (is_prime(number,tab,mutex) == 1){
        return arr;}

    for (unsigned long long j = 2; j < (number/2) +1; j++){

        if  ((is_div(number,j)) && (is_prime(j,tab,mutex))){
            arr->liste = (unsigned long long int *) realloc(arr->liste, (arr->nbre_elem +1) * sizeof(unsigned long long));
            if (arr->liste == NULL){ 
                free(arr->nbre_elem);
                free(arr);
                return  NULL;}
            arr->liste[arr->nbre_elem++] = j;
        }
    }
    return arr;
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

Repertoire_t_th *prime_divs_opti(unsigned long long number,Repertoire_t_th *rep, pthread_mutex_t *mutex){
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
    if (arr->nbre_elem == 1){
        pthread_mutex_lock(mutex);
        int a = AppendNumber(arr->liste[0],rep);
        pthread_mutex_unlock(mutex);
        if (a == -1){return NULL;}
    }
    else{
        for (int i = 1; i < arr->nbre_elem; i++){
            int add = 0;
            for (int j = 0; j < rep->nbre_elem; j++){
                if (arr->liste[i] == rep->liste[j]){
                    add++;
                    break;
                }
            }
            if (add == 0){
                pthread_mutex_lock(mutex);
                int a = AppendNumber(arr->liste[i],rep);
                pthread_mutex_unlock(mutex);
                if (a == -1){return NULL;}
            }
        }
    }
    return arr;
}

///== FONCTIONS PROPRES AUX BUFFERS ==\\\

void putNumber(Entrepot_Th *tab,unsigned long long number){
    //free((tab->buffer[tab->putindex].liste));
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
     * lecture est une fonction à un seul argument (comme ça on peut employer des threads dessus)
     * lit la ligne, et place dans le lect->buffer le résultat
     */
    Lecteur_Th *lect =  (Lecteur_Th *) arg;
    while (fgets(lect->ligne,30,lect->fichier) != NULL) {
        unsigned long long number = atoll(lect->ligne);
        sem_wait(lect->semaphores[0]);  // On attend une place vide  > "firstempty"
        pthread_mutex_lock(lect->flag); // bloque le tableau1 pour ajouter l'élément
    
        putNumber(lect->tableau,number);    // Ajoute number dans le 1e buffer

        pthread_mutex_unlock(lect->flag);
        sem_post(lect->semaphores[1]);  //On ajoute un élément donc on réveille les "firstfill"
    }
    fin_de_lecture++;

    for (int i = 0; i < (lect->tableau->size)/2; i++){
        sem_wait(lect->semaphores[0]);  // On attend une place vide
        sem_post(lect->semaphores[1]);  //On 'ajoute' un élément donc on réveille les "firstfill"
    }
    return NULL;
}

void *calcul(void* arg) {
    Usine_Th *usine = (Usine_Th *) arg;

    while ((fin_de_lecture == 0) || (usine->tabin->nbre != 0)){

        sem_wait(usine->semaphores[1]);  //Attend d'avoir des éléments à prendre > 'firstfill'
        
        if ((fin_de_lecture != 0) && (usine->tabin->nbre == 0)){break;}
        
        pthread_mutex_lock(usine->flags[0]);
        Repertoire_t_th entree = takeRepertoire(usine->tabin);
        pthread_mutex_unlock(usine->flags[0]);
        sem_post(usine->semaphores[0]); // signale aux "firstempty" qu'on a pris un élément
        
        Repertoire_t_th *resultat = prime_divs(entree.liste[0], usine->rep, usine->flags[2]);

        sem_wait(usine->semaphores[2]);  //Attend "secondempty"  >> une place vide ds le 2e tableau
        pthread_mutex_lock(usine->flags[1]);    // Protège le 2e tableau
        putRepertoire(resultat,usine->tabout);
        pthread_mutex_unlock(usine->flags[1]);
        sem_post(usine->semaphores[3]); //On réveille les "secondfill" car on a ajouté un élément
        
        free(entree.liste);
        free(resultat);
    }

    pthread_mutex_lock(usine->flags[2]);    // On protège l'accès à la variable globale
    Threads_de_calculs_finis++;
    if (Threads_de_calculs_finis == (usine->tabout->size)/2){sem_post(usine->semaphores[3]);}
    pthread_mutex_unlock(usine->flags[2]);
    return NULL;
}

void *ecriture(void* arg) {
    Imprimerie_Th *impr = (Imprimerie_Th *) arg;
    Repertoire_t_th resultat;
    while ((Threads_de_calculs_finis < (impr->tabout->size)/2) || (impr->tabout->nbre != 0)) {

        sem_wait(impr->semaphores[1]);  //On attend des élément dans le tableau2: "secondfill"
        if ((Threads_de_calculs_finis == (impr->tabout->size)/2) && (impr->tabout->nbre == 0)){break;}

        pthread_mutex_lock(impr->flag); // On protège le tableau

        resultat = takeRepertoire(impr->tabout); 

        pthread_mutex_unlock(impr->flag);
        sem_post(impr->semaphores[0]);  //On réveille les "secondempty" car on a pris un élément

        if ((resultat.nbre_elem == 1) && (resultat.liste[0]) < 2) {    // Exception où on a number < 2
            fprintf(impr->fichierOut, "Erreur: %lld est inférieur à 2 !", resultat.liste[0]);}

        else {
            for (int i = 0; i < resultat.nbre_elem; i++) {
                fprintf(impr->fichierOut, "%lld ", resultat.liste[i]);}
        }
        fputc('\n',impr->fichierOut);
        free(resultat.liste);
    }
    return NULL;
}


int principale(int N, char *input_file, char *output_file) {
    /*
     * pré: input != NULL ; output_file != NULL N >= NULL (si N = 0, cela signifie qu'on veut la valeur par
     * défaut..> 4)
     * input est un fichier qui contient un élément (int,char,float,...) par ligne
     *
     * post: écris dans chaque ligne de output_file la liste des diviseurs premiers du INT à la ligne correspondante dans
     * input. Si jamais l'élément n'est pas un int, écris une erreur.
     *
     * return : 0 -> aucune erreur survenue
     *         -1 -> erreur causée par fopen()
     *         -2 -> erreur causée par malloc()
     *         -3 -> erreur causée par pthread_create()
     *         -4 -> erreur causée par fclose()
     */

    // == OUVERTURE == //
    FILE *filein = fopen(input_file,"r");
    if (filein == NULL){return -1;}
    FILE *fileout = fopen(output_file,"w");
    if (fileout == NULL){
        fclose(filein);
        return -1;}


    // == PRÉPARATION == //

    // 1) INITIALISATION DES VARIABLES //
    if (N <= 0){N = 4;}

    unsigned long long maximum = 2*N;   // C'est la taille de nos deux buffers
    char line[30];// là dedans qu'est stockée la ligne du fichier

    pthread_mutex_t firstmutex;  // Utilisé pour protéger l'accès au tableau1
    pthread_mutex_init(&firstmutex,NULL);
    pthread_mutex_t secondmutex; //  Utilisé pour protéger l'accès au tableau2
    pthread_mutex_init(&secondmutex,NULL);
    pthread_mutex_t thirdmutex;
    pthread_mutex_init(&thirdmutex,NULL);   //Utilisé pour protéger le rep où on stocke les nbre premiers

    sem_t firstempty; // nbre de place VIDE dans tableau1
    sem_init(&firstempty,0,maximum);
    sem_t secondemtpy;  // nbre de place VIDE dans tableau2
    sem_init(&secondemtpy,0,maximum);
    sem_t firstfill;    // nbre de place REMPLIE dans le tableau1
    sem_init(&firstfill, 0, 0);
    sem_t secondfill;   // nbre de place REMPLIE dans le tableau2
    sem_init(&secondfill, 0, 0);

    // 2) INITIALISATION DES STRUCTURES //
    Repertoire_t_th *rep = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (rep == NULL){return fermer(filein,fileout,-2);}
    rep->nbre_elem = 0;

    Entrepot_Th *tableau1 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau1 == NULL){
        free(rep);
        return fermer(filein,fileout,-2);}

    tableau1->buffer = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th) * maximum);
    if (tableau1->buffer == NULL){
        free(rep);
        free(tableau1);
        return fermer(filein,fileout,-2);}
    tableau1->putindex = 0;
    tableau1->takeindex = 0;
    tableau1->size = (int) maximum;
    tableau1->nbre = 0;


    Entrepot_Th *tableau2 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau2 == NULL){
        free(rep);
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
        free(rep);
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
        free(rep);
        free(tableau1->buffer);
        free(tableau1);
        free(tableau2->buffer);
        free(tableau2);
        free(lect);
        return fermer(filein,fileout,-2);}
    calc->tabin = tableau1;
    calc->tabout = tableau2;
    calc->rep = rep;
    calc->semaphores[0] = &firstempty;
    calc->semaphores[1] = &firstfill;
    calc->semaphores[2] = &secondemtpy;
    calc->semaphores[3] = &secondfill;
    calc->flags[0] = &firstmutex;   // Protéger le tableau1
    calc->flags[1] = &secondmutex;  //Protéger le tableau2
    calc->flags[2] = &thirdmutex;   // Protéger le repertoire qui contient tt les nbres premiers


    Imprimerie_Th *imp = (Imprimerie_Th *) malloc(sizeof(struct imprimerie));
    if (imp == NULL){
        free(rep);
        free(tableau1->buffer);
        free(tableau1);
        free(tableau2->buffer);
        free(tableau2);
        free(lect);
        free(calc);
        return fermer(filein,fileout,-2);
    }
    imp->fichierOut = fileout;  // fichier où on écrit les résultats
    imp->tabout = tableau2;     // Tableau où on prend les éléments
    imp->semaphores[0] = &secondemtpy;
    imp->semaphores[1] = &secondfill;
    imp->flag = &secondmutex;       // Protéger tableau2 lors des accès

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
    free(rep->liste);
    free(rep);

    free(imp);

    // == FERMETURE == //
    
    pthread_mutex_destroy(&firstmutex);
    pthread_mutex_destroy(&secondmutex);
    pthread_mutex_destroy(&thirdmutex);
    
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