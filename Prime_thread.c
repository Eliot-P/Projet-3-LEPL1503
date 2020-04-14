/*
 Created by Constantin on 16-03-20.

    - Traduction du code python -

Last update: 16:16:30  14/04/2020
*/


#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// == STRUCTURES == //

// >> quand le type se termine par "Th" c'est que la structure est faite exclusivement pour les threads
typedef struct repertoire{
    int nbre_elem;
    unsigned long long *liste;
} Repertoire_t;

typedef struct entrepot{
    int size;   // Taille du tableau
    int nbre;   // Nombre d'éléments dans le tableau
    int putindex;
    int takeindex;
    Repertoire_t *buffer;  // On aura un tableau de Repertoire_t
} Entrepot_Th;

typedef struct lecteur{  //Argument qu'on passe a la fct lecture
    char *ligne;
    FILE *fichier;
    Entrepot_Th *tableau;
    pthread_mutex_t flag;
    sem_t semaphore[2];
} Lecteur_Th;

typedef struct usine{
    Entrepot_Th *tabin; // tableau où on prend les number
    Entrepot_Th *tabout;  // tableau où on stocke les Repertoire_t qui contiennent les rpses
    Repertoire_t *rep;  //La structure a protéger (contient tous les nbre premier)
    pthread_mutex_t *flags[3];
    sem_t *semaphores[3];
}Usine_Th;

typedef struct imprimerie{
    FILE *fichierOut;   // Fichier où écrire
    pthread_mutex_t *flag;  // Mutex pour protéger le tableau où la fonction prend les résultats
    sem_t *semaphores[2];   // Permet d'attendre si jamais l'entrepot est vide
    Entrepot_Th *tabout;    // Contient les résultats à écrire
}Imprimerie_Th;


/// == ACCESSIBILITÉ == //
/*
void imprimer(Repertoire_t *tab){
    for (int i = 0; i < tab->nbre_elem; i++){
        printf("%llu ",tab->liste[i]);
    }
}
*/
int fermer(FILE *entree, FILE *sortie,int i){  //raccourci pour fermer les fichier quand on a une erreur    fclose(entree);
    fclose(sortie);
    return i;
}

// == FONCTIONS == //

int is_div(unsigned long long number, unsigned long long i){  // True si i divise number, sinon false
    return number%i == 0;
}

int  is_prime(unsigned long long number, Repertoire_t *tab,pthread_mutex_t *mutex){
    /*
     * retourne 1 si number est un nombre premier et retourne 0 sinon
     *
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
    for (unsigned long long i = 2; i < number/2; i++) {   // On peut pe optimiser avec un max et en repartant du max si number<max
        if (is_div(number, i)) {
            return 0;
        }
    }
    pthread_mutex_lock(mutex);  // On protège l'accès au répertoire quand on ajoute un nbre premier
    tab->liste = (unsigned long long *) realloc((*tab).liste,((*tab).nbre_elem + 1) * sizeof(unsigned long long));
    if ((*tab).liste == NULL) { return -1; }
    tab->liste[(*tab).nbre_elem++] = number;
    pthread_mutex_unlock(mutex);
    return 1;
}

Repertoire_t *prime_divs(unsigned long long number, Repertoire_t *tab,pthread_mutex_t *mutex){
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
    Repertoire_t *arr = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (arr == NULL){ return NULL;}
    arr->liste = (unsigned long long *) malloc(sizeof(number));
    if (arr->liste == NULL){ return  NULL;}

    arr->nbre_elem = 0;
    arr->liste[arr->nbre_elem++] = number;

    if (number < 2){ return arr;}

    if (is_prime(number,tab,mutex) == 1){
        return arr;}

    for (unsigned long long j = 2; j < number/2; j++){

        if  ((is_div(number,j)) && (is_prime(j,tab,mutex))){
            arr->liste = (unsigned long long int *) realloc(arr->liste, (arr->nbre_elem + 1) * sizeof(unsigned long long));
            if (arr->liste == NULL){ return  NULL;}
            arr->liste[arr->nbre_elem++] = j;
        }
    }
    return arr;
}

void lecture(Lecteur_Th *lect){
    /*
     * lecture est une fonction à un seul argument (comme ça on peut employer des threads dessus)
     * lit la ligne, et place dans le lect->buffer le résultat
     *  >> Comment gérer les fails de malloc ?*/
    while (fgets(lect->ligne,sizeof(unsigned long long),lect->fichier) != NULL) {
        unsigned long long number = atol(lect->ligne);
        if (lect->tableau->nbre == lect->tableau->size){
            sem_wait(&(lect->semaphore[0]));
        }
        pthread_mutex_lock(&(lect->flag)); // bloque le tableau1 pour ajouter l'élément
        (lect->tableau->buffer[lect->tableau->putindex]).liste = (unsigned long long *) malloc(sizeof(unsigned long long));
        (lect->tableau->buffer[lect->tableau->putindex]).liste[0] = number;
        lect->tableau->putindex = (lect->tableau->putindex + 1) % lect->tableau->size;
        lect->tableau->nbre++;
        pthread_mutex_unlock(&(lect->flag));
        sem_post(&(lect->semaphore[0]));
    }
    sem_post(&lect->semaphore[1]);
}

void calcul(Usine_Th *usine) {
    int ok1 = 0;
    while ((ok1 == 0)| (usine->tabin->nbre != 0)) {
        sem_getvalue(usine->semaphores[2], &ok1);

        if (usine->tabin->nbre == 0) {
            sem_wait(usine->semaphores[0]);}    //Attend d'avoir des éléments à prendre

        pthread_mutex_lock(usine->flags[0]);
        Repertoire_t entree = usine->tabin->buffer[usine->tabin->takeindex];
        usine->tabin->takeindex = (usine->tabin->takeindex + 1) % usine->tabin->size;
        usine->tabin->nbre--;
        pthread_mutex_unlock(usine->flags[0]);
        sem_post(usine->semaphores[0]); // signale qu'on a pris un élément

        // On calcule les diviseurs premiers de number puis on libère la mémoire allouée pour son stockage
        Repertoire_t *resultat = prime_divs(entree.liste[0], usine->rep, usine->flags[2]);
        pthread_mutex_lock(usine->flags[0]);
        free(&entree);
        pthread_mutex_unlock(usine->flags[0]);

        if (usine->tabout->nbre ==  usine->tabout->size) {
            sem_wait(usine->semaphores[1]);  // Attend qu'il y ait de la place pr déposer son élément
        }
        pthread_mutex_lock(usine->flags[1]);    // Protège le 2e tableau
        usine->tabout->buffer[usine->tabout->putindex] = *resultat;
        usine->tabout->putindex = (usine->tabout->putindex + 1) % usine->tabout->size;
        usine->tabout->nbre++;
        pthread_mutex_unlock(usine->flags[1]);
        sem_post(usine->semaphores[1]);
    }
}

void ecriture(Imprimerie_Th *impr) {
    int a = 0;
    while ((a == 0) | (impr->tabout->nbre != 0)) {
        sem_getvalue(impr->semaphores[1], &a);

        if (impr->tabout->nbre == 0) {
            sem_wait(impr->semaphores[0]);
        } // On attend que le tableau soit non vide

        pthread_mutex_lock(impr->flag); // On protège le tableau
        Repertoire_t resultat = impr->tabout->buffer[impr->tabout->takeindex];
        impr->tabout->takeindex = (impr->tabout->takeindex + 1) % impr->tabout->size;
        impr->tabout->nbre--;
        pthread_mutex_unlock(impr->flag);
        sem_post(impr->semaphores[0]);

        if ((resultat.nbre_elem == 1) && (resultat.liste[0]) < 2) {    // Exception où on a number < 2
            fprintf(impr->fichierOut, "Erreur: %lld est inférieur à 2 !\n", resultat.liste[0]);
        } else {
            for (int i = 0; i < resultat.nbre_elem; i++) {
                fprintf(impr->fichierOut, "%lld ", resultat.liste[i]);
            }
            fprintf(impr->fichierOut, "\n");
        }

        pthread_mutex_lock(impr->flag); // Nécessaire de lock le tableau ?
        free(&resultat);
        pthread_mutex_unlock(impr->flag);
    }
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
    if (N == 0){N = 4;}
    unsigned long long maximum = N + N/2;   // C'est la taille de nos deux buffers
    char line[sizeof(unsigned long long)]; // la dedans qu'est stockée la ligne du fichier

    pthread_mutex_t firstmutex;  // Utilisé pour protéger l'accès au tableau1
    pthread_mutex_init(&firstmutex,NULL);
    pthread_mutex_t secondmutex; //  Utilisé pour protéger l'accès au tableau2
    pthread_mutex_init(&secondmutex,NULL);
    pthread_mutex_t thirdmutex;
    pthread_mutex_init(&thirdmutex,NULL);   //Utilisé pour protéger le rep où on stocke les nbre premiers

    sem_t firstempty; // nbre de place vide dans tableau1
    sem_init(&firstempty,0,maximum);
    sem_t secondemtpy;  // nbre de place vide dans tableau2
    sem_init(&secondemtpy,0,maximum);
    sem_t firstfull;    // nbre de place occupée dans le tableau1
    sem_init(&firstfull,0,0);
    sem_t secondfull;   // nbre de place occupée dans le tableau2
    sem_init(&secondfull,0,0);
    sem_t fin;
    sem_init(&fin,0,0);
    // 2) INITIALISATION DES STRUCTURES //

    Repertoire_t *rep = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (rep == NULL){return fermer(filein,fileout,-2);}
    rep->nbre_elem = 0;


    Entrepot_Th *tableau1 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau1 == NULL){return fermer(filein,fileout,-2);}
    tableau1->buffer = (Repertoire_t *) malloc(sizeof(struct repertoire) * maximum);//On aura un tableau de
    if (tableau1->buffer == NULL){return fermer(filein,fileout,-2);} // (N +N/2) repertoire
    tableau1->putindex = 0;
    tableau1->takeindex = 0;
    tableau1->size = (int) maximum;
    tableau1->nbre = 0;


    Entrepot_Th *tableau2 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau2 == NULL){return fermer(filein,fileout,-2);}
    tableau2->buffer = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (tableau2->buffer == NULL){return fermer(filein,fileout,-2);}
    tableau2->putindex = 0;
    tableau2->takeindex = 0;
    tableau2->size = (int) maximum;
    tableau2->nbre = 0;

    Lecteur_Th *lect = (Lecteur_Th *) malloc(sizeof(struct lecteur));
    if (lect == NULL){return fermer(filein,fileout,-2);}
    lect->ligne = line;
    lect->fichier = filein;
    lect->tableau = tableau1;
    lect->flag = firstmutex;
    lect->semaphore[0] = firstempty;
    lect->semaphore[1] = fin;

    Usine_Th *calc = (Usine_Th *) malloc(sizeof(struct usine));
    if (calc == NULL){return fermer(filein,fileout,-2);}
    calc->semaphores[0] = &firstfull;  // Savoir si le tableau1 est rempli
    calc->semaphores[1] = &secondemtpy;  // Savoir si le tableau2 est vide
    calc->semaphores[2] = &fin; // Permet de savoir si on est arrivé à la fin du fichier
    calc->flags[0] = &firstmutex;   // Protéger le tableau1
    calc->flags[1] = &secondmutex;  //Protéger le tableau2
    calc->flags[2] = &thirdmutex;   // Protéger le repertoire qui contient tt les nbres premiers

    Imprimerie_Th *imp = (Imprimerie_Th *) malloc(sizeof(struct imprimerie));
    imp->fichierOut = fileout;  // fichier où on écrit les résultats
    imp->tabout = tableau2;     // Tableau où on prend les éléments
    imp->semaphores[0] = &secondfull;   // Savoir si le tableau2 est rempli
    imp->semaphores[1] = &fin;
    imp->flag = &secondmutex;       // Protéger tableau2 lors des accès

    // 3) INITIALISATION DES THREADS //

    pthread_t lecteur_th;
    int l = pthread_create(&lecteur_th, NULL, (void *(*)(void *)) &lecture, lect);
    if (l != 0){ return fermer(filein,fileout,-3);}
    int ll = pthread_join(lecteur_th,NULL);
    if (ll != 0){ fermer(filein,fileout,-3);}

    pthread_t calcul_th[N];
    for (int i = 0; i < N; i++){
        int c = pthread_create(&calcul_th[i], NULL, (void *(*)(void *)) &calcul, calc);
        if (c != 0){return fermer(filein,fileout,-3);}
    }

    for (int i = 0; i < N; i++){
        int cc = pthread_join(calcul_th[i],NULL);
        if (cc != 0){return fermer(filein,fileout,-3);}
    }

    pthread_t imprimeur_th;
    int i = pthread_create(&imprimeur_th, NULL, (void *(*)(void *)) ecriture, imp);
    if (i != 0){return fermer(filein,fileout,-3);}
    int ii = pthread_join(imprimeur_th,NULL);
    if (ii != 0){ return fermer(filein,fileout,-3);}

    /*
    // == DÉBUT == //
    char chaine[65];
    while (fgets(chaine,65,filein) != NULL){
        unsigned long long number = atol(chaine);
        if (number < 2){
            fprintf(fileout,"Erreur: %llu < 2\n",number);
        }
        else{
            Repertoire_t *divs = prime_divs(number,rep);
            for (unsigned long long x = 0; x < divs->nbre_elem; x++){
                fprintf(fileout,"%llu ",divs->liste[x]);
            }
            fputc(13,fileout);  // retour à la ligne sous (certains) windows !!!!
            fputc(10,fileout); // retour à la ligne standar (\n)
            free(divs);  // libère bien le contenu du pointeur ??? XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        }
    }
    free(rep);
     */
    // == FREE == //

    free(lect);
    free(calc);
    free(imp);

    // == FERMETURE == //

    int x = fclose(filein);
    if (x != 0){
        fclose(fileout);
        return -4;}

    int y = fclose(fileout);
    if (y != 0){
        return -4;}
    return 0;
}