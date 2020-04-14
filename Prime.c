/*
 Created by Constantin on 16-03-20.

    - Traduction du code python -

Last update: 11:05:12  13/04/2020
*/

#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct repertoire{
    int nbre_elem;
    unsigned long long *liste;    // Array qui contiendra les nbres premiers (mélangés !)
} Repertoire_t;

int is_div_simple(unsigned long long number, unsigned long long i){  // True si i divise number, sinon false
    return number%i == 0;
}

int  is_prime_simple(unsigned long long number, Repertoire_t *tab){
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
        if (is_div_simple(number,tab->liste[i])){
            return 0;
        }
    }
    for (unsigned long long i = 2; i < number/2; i++) {   // On peut pe optimiser avec un max et en repartant du max si number<max
        if (is_div_simple(number, i)) {
            return 0;
        }
    }
    tab->liste = (unsigned long long *) realloc((*tab).liste,((*tab).nbre_elem + 1) * sizeof(unsigned long long));
    if ((*tab).liste == NULL) { return -1; }
    tab->liste[(*tab).nbre_elem++] = number;
    return 1;
}

Repertoire_t *prime_divs_simple(unsigned long long number, Repertoire_t *tab){
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

    if (is_prime_simple(number,tab) == 1){
        return arr;}

    for (unsigned long long j = 2; j < number/2; j++){

        if  ((is_div_simple(number,j)) && (is_prime_simple(j,tab))){
            arr->liste = (unsigned long long int *) realloc(arr->liste, (arr->nbre_elem + 1) * sizeof(unsigned long long));
            if (arr->liste == NULL){ return  NULL;}
            arr->liste[arr->nbre_elem++] = j;
        }
    }
    return arr;
}

int principale_simple(int N, char *input_file, char *output_file) {
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
    Repertoire_t *rep = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (rep == NULL){
        fclose(filein);
        fclose(fileout);
        return -2;
    }
    rep->nbre_elem = 0;





    // == DÉBUT == //
    char chaine[50];
    while (fgets(chaine,50,filein) != NULL){ // Je comprend pas pk 'chaine' est considéré comme un pointeur
        //long number = strtol(chaine,NULL,32); // Comment gérer les erreurs si ça fonctionne pas..?
        unsigned long long number = atol(chaine);
        if (number < 2){
            fprintf(fileout,"Erreur: %llu < 2\n",number);
        }
        else{
            Repertoire_t *divs = prime_divs_simple(number,rep);
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
void imprimer_simple(Repertoire_t *tab){
    for (int i = 0; i < tab->nbre_elem; i++){
        printf("%llu ",tab->liste[i]);
    }
}