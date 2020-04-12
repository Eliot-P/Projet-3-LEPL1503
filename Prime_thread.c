/*
 Copied by Eliot from Constantin on 06-04-20.

    - Traduction du code python -

Last update: 17:46:12  6/04/2020
*/

#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct repertoire_th{
    int nbre_elem;
    long *liste;    // Array qui contiendra les nbres premiers (mélangés !)
} Repertoire_t_th;

int is_div_th(long number, long i){  // True si i divise number, sinon false
    return number%i == 0;
}
void imprimer_th(Repertoire_t_th *tab){
    for (int i = 0; i < tab->nbre_elem; i++){
        printf("%ld ",tab->liste[i]);
    }
}

int  is_prime_th(long number, Repertoire_t_th *tab){
    /*
     * retourne 1 si number est un nombre premier et retourne 0 sinon
     *
     * La fonction procède de la sorte:
     *      - 1) Elle vérifie si number est divisible par un des nbre premiers contenu dans tab->liste
     *      - 2) Si aucun nbre  premier divise number, on va vérifier si number en est un en appliquant la définition
     *      mathématique (aucun diviseur entre 2 et number/2) et s'il est premier on l'ajoute dans le tableau d'array
     */

    for (long i = 0; i < tab->nbre_elem; i++){
        if (tab->liste[i] == number){return 1;}
        if (is_div_th(number,tab->liste[i])){
            return 0;
        }
    }
    for (long i = 2; i < number/2; i++) {   // On peut pe optimiser avec un max et en repartant du max si number<max
        if (is_div_th(number, i)) {
            return 0;
        }
    }
    tab->liste = (long *) realloc((*tab).liste,((*tab).nbre_elem + 1) * sizeof(long));
    if ((*tab).liste == NULL) { return -1; }
    tab->liste[(*tab).nbre_elem++] = number;
    return 1;
}

Repertoire_t_th *prime_divs_th(long number, Repertoire_t_th *tab){
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
    if (arr == NULL){ return NULL;}
    arr->liste = (long *) malloc(sizeof(number));
    if (arr->liste == NULL){ return  NULL;}

    arr->nbre_elem = 0;
    arr->liste[arr->nbre_elem++] = number;

    if (is_prime_th(number,tab) == 1){
        return arr;}

    for (long j = 2; j < number/2; j++){

        if  ((is_div_th(number,j)) && (is_prime_th(j,tab))){
            arr->liste = (long *) realloc(arr->liste, (arr->nbre_elem +1)*sizeof(long));
            if (arr->liste == NULL){ return  NULL;}
            arr->liste[arr->nbre_elem++] = j;
        }
    }
    return arr;
}

int principale_th(char *input_file, char *output_file) {
    /*
     * pré: input != NULL ; output_file != NULL  Ce sont des fichiers
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

    FILE *filein = fopen(input_file,"r");
    if (filein == NULL){return -1;}
    FILE *fileout = fopen(output_file,"w");
    if (fileout == NULL){return -1;}

    // On instancie la structure
    Repertoire_t_th *rep = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    rep->liste = (long *) malloc(sizeof(long) *4);
    if (rep->liste == NULL){
        return -2;}
    rep->liste[0] = (long) 2; rep->liste[1] = (long) 3; rep->liste[2] = (long) 5; rep->liste[3] = (long) 7;
    rep->nbre_elem = 4;

    char chaine[50];
    long i = 0;
    while (fgets(chaine,50,filein) != NULL){ // Je comprend pas pk 'chaine' est considéré comme un pointeur
        //long number = strtol(chaine,NULL,32); // Comment gérer les erreurs si ça fonctionne pas..?
        long number = atol(chaine);
        if (number < 2){
            fprintf(fileout,"Erreur à la ligne %ld (%ld < 2)\n",i,number);
        }
        else{
            Repertoire_t_th *divs = prime_divs_th(number,rep);
            for (long x = 0; x < divs->nbre_elem; x++){
                fprintf(fileout,"%ld ",divs->liste[x]);
            }
            fputc(13,fileout);  // retour à la ligne sous (certains) windows !!!!
            fputc(10,fileout); // retour à la ligne standar (\n)
            free(divs);  // libère bien le contenu du pointeur ??? XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        }
        i++;
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