/*
 Created by Constantin van Ypersele on 13/04/2020

    - Traduction du code python -
       - Version avec threads -

Last update: 20:04:08  13/04/2020
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

// >> quand le type se termine par "Th" c'est que la structure est employée par les threads
typedef struct repertoire{
    int nbre_elem;
    unsigned long long *liste;
} Repertoire_t;

typedef struct entrepot{
    int size;
    int putindex;
    int takeindex;
    unsigned long long *buffer;
} Entrepot_Th;

typedef struct lecteur{  //Argument qu'on passe a la fct lecture + on suppose qu'on lit tjs 64 bytes
    char *ligne;
    FILE *fichier;
    Entrepot_Th *tableau;
    pthread_mutex_t flag;
    sem_t semaphore;
} Lecteur_Th;

// == FONCTIONS == //
int is_div(unsigned long long number, unsigned long long i){  // True si i divise number, sinon false
    return number%i == 0;
}

int  is_prime(unsigned long long number, Repertoire_t *tab){
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
    tab->liste = (unsigned long long *) realloc((*tab).liste,((*tab).nbre_elem + 1) * sizeof(unsigned long long));
    if ((*tab).liste == NULL) { return -1; }
    tab->liste[(*tab).nbre_elem++] = number;
    return 1;
}

Repertoire_t *prime_divs(unsigned long long number, Repertoire_t *tab){
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

    if (is_prime(number,tab) == 1){
        return arr;}

    for (unsigned long long j = 2; j < number/2; j++){

        if  ((is_div(number,j)) && (is_prime(j,tab))){
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
     */
    while (fgets(lect->ligne,sizeof(unsigned long long),lect->fichier) != NULL) {
        unsigned long long number = atol(lect->ligne);
        //if number < 2  >> à traiter
        sem_wait(&(lect->semaphore));
        pthread_mutex_lock(&(lect->flag)); // bloque le tableau1 pour ajouter l'élément
        lect->tableau->buffer[lect->tableau->putindex] = number;
        lect->tableau->putindex = (lect->tableau->putindex + 1) % lect->tableau->size;
        pthread_mutex_unlock(&(lect->flag));
        sem_post(&(lect->semaphore));
    }
    // gestion à traiter quand on arrive à la fin du fichier
}



int principale(int N, char *input_file, char *output_file) {
    /*
     * pré: input != NULL ; output_file != NULL  N > 0 et peut être NULL et sera alors par défaut = 4
     * input est un fichier qui contient un élément (int,char,float,...) par ligne
     *
     * post: écris dans chaque ligne de output_file la liste des diviseurs premiers du INT à la ligne correspondante dans
     * input. Si jamais l'élément n'est pas un int, écris une erreur.
     *
     * return : 0 -> aucune erreur survenue
     *         -1 -> erreur causée par fopen()
     *         -2 -> erreur causée par malloc()
     *         -3 -> erreur causée par fclose()
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

    unsigned long long maximum = N + N/2;   // C'est la taille de nos deux buffers
    char line[sizeof(unsigned long long)]; // la dedans qu'est stockée la ligne du fichier

    pthread_mutex_t firstmutex;  // Utilisé pour protéger l'accès au tableau1
    pthread_mutex_init(&firstmutex,NULL);
    pthread_mutex_t secondmutex; //  " " "  tableau2
    pthread_mutex_init(&secondmutex,NULL); // initialise les mutex

    sem_t firstempty;
    sem_init(&firstempty,0,maximum);
    sem_t secondemtpy;
    sem_init(&secondemtpy,0,maximum);
    sem_t firstfull;
    sem_init(&firstfull,0,0);
    sem_t secondfull;
    sem_init(&secondfull,0,0);

    // 2) INITIALISATION DES STRUCTURES //

    Repertoire_t *rep = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (rep == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;
    }
    rep->nbre_elem = 0;


    Entrepot_Th *tableau1 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau1 == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;}
    tableau1->buffer = (unsigned long long*) malloc(sizeof(unsigned long long) * (N + N / 2));
    if (tableau1->buffer == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;}
    tableau1->putindex = 0;
    tableau1->takeindex = 0;
    tableau1->size = (int) maximum;


    Entrepot_Th *tableau2 = (Entrepot_Th *) malloc(sizeof(struct entrepot));
    if (tableau2 == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;}
    tableau2->buffer = (unsigned long long*) malloc(sizeof(unsigned long long) * (N + N/2));
    if (tableau2->buffer == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;}
    tableau2->putindex = 0;
    tableau2->takeindex = 0;
    tableau2->size = (int) maximum;

    Lecteur_Th *lect = (Lecteur_Th *) malloc(sizeof(struct lecteur));
    if (lect == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;}
    lect->ligne = line;
    lect->fichier = filein;
    lect->tableau = tableau1;
    lect->flag = firstmutex;
    lect->semaphore = firstempty;



    // 3) INITIALISATION DES THREADS //

    pthread_t lecteur;
    int l = pthread_create(&lecteur, NULL, (void *(*)(void *)) &lecture, lect);


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
    int x = fclose(filein);
    if (x != 0){
        fclose(fileout);
        return -3;
    }
    int y = fclose(fileout);
    if (y != 0){
        return -3;
    }
    return 0;
}

/// Fonction auxiliaires \\\*
void imprimer(Repertoire_t *tab){
    for (int i = 0; i < tab->nbre_elem; i++){
        printf("%llu ",tab->liste[i]);
    }
}