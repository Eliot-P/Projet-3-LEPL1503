/*
 Created by Constantin on 16-03-20.

    - Traduction du code python -

Last update: 23:20:17  30/03/2020
*/

#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct repertoire{  //Initialiser à maximum = 0 et nbre_elem = 0
    int nbre_elem;
    long *liste;
} Repertoire_t;

int is_div(long number, long i){  // True si i divise number, sinon false
    return number%i == 0;
}

int  is_prime(long number, Repertoire_t *tab){
    /*
     * Retourne 1 si number est premier, 0 si number ne l'est pas et -1 en cas d'erreur
     * La fonction va calculer tout les nombres premier jusque number, PEUT IMPORTE LA VALEUR DE NUMBER !
     * On considère que tab contient déjà quelques nombres premier et donc n'est pas vide !!
     * La manière de procéder est la suivante :
     *
     *      - On regarde si number est un (/mutliple d'un des) éléments  de (*tab).liste
     *      - Sinon, on va regarder du (*tab).maximum à number -1 et à nouveau, ajouter tout les nbres premiers que l'on
     *      rencontre dans (*tab).liste et si jamais number est un nbre premier on l'ajoute également dans la liste.
     */
    if (number < 0){    // exception où on aurait un nbre négatif (sinon les boucles for plantent )
        number = number * -1;
    }
    for (long i = 0; i < tab->nbre_elem; i++){
        if (tab->liste[i] == number){return 1;}
        if (number % tab->liste[i] == 0){
            return 0;
        }
    }
    for (long i = 2; i <= number/2; i++) {
        if (is_div(number, i)) {
            return 0;
        }
    }
    tab->liste = (long *) reallocarray((*tab).liste,(*tab).nbre_elem +1, sizeof(long));
    if ((*tab).liste == NULL) { return -1; }
    tab->liste[(*tab).nbre_elem++] = number;
    return 1;
}

long *prime_divs(long number, Repertoire_t *tab){
    /*  retourne un pointeur vers un tableau de long contenant tout les diviseurs premiers de number
     * > alloue dynamiquement de la mémoire pour créer le tableau
    */

    if (is_prime(number,tab) == 1){
        return (long *) number;
    }
    long *arr = (long *) malloc(sizeof(number));
    if (arr == NULL){ return (long *) -1;}

    int index = 0;
    long size = sizeof(number);
    arr[index++] = number;

    for (long j = 2; j < number/2 +1; j++){

        if  ((is_div(number,j)) && (is_prime(j,tab))){
            size = size + sizeof(j); // (char) long +1
            arr = (long *) reallocarray(arr, size, sizeof(long));
            if (arr == NULL){ return  (long *) -1;}
            arr[index++] = j;
        }
    }
    return arr;
}

int principale(char *input_file, char *output_file) {
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
    Repertoire_t *rep = (Repertoire_t *) malloc(sizeof(struct repertoire));
    rep->liste = (long *) malloc(sizeof(long) *4);
    if (rep->liste == NULL){
        return -2;}
    rep->liste[0] = (long) 2; rep->liste[1] = (long) 3; rep->liste[2] = (long) 5; rep->liste[3] = (long) 7;
    rep->nbre_elem = 4;

    char chaine[sizeof(long)];
    int i = 0;
    while (fgets(chaine,sizeof(long),filein) != NULL){ // Je comprend pas pk 'chaine' est considéré comme un pointeur
        long number = strtol(chaine,NULL,16); // Comment gérer les erreurs si ça fonctionne pas..?
        if (number < 2){
            fprintf(fileout,"Erreur à la %ie ligne (%ld < 2)\n",i,number);
        }
        else{
            long *divs = prime_divs(number,rep);
            int taille = sizeof(divs)/8;

            fprintf(fileout,"%ld ",number);
            //fprintf(fileout,"%ld",divs);
            //if (taille == 1){ continue;}
            for (int s = 0; s < taille; s++){
                fprintf(fileout,"%ld ",divs[s]);
            }
            fprintf(fileout,"\n");
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

/*
int principale(char *input, char *output_file) {
    /*
     * pré: input != NULL ; output_file != NULL  Ce sont des fichiers
     * input est un fichier qui contient un élément (int,char,float,...) par ligne
     *
     * post: écris dans chaque ligne de output_file la liste des diviseurs premiers du INT à la ligne correspondante dans
     * input. Si jamais l'élément n'est pas un int, écris une erreur.
     *
     * return : 0 -> aucune erreur survenue
     *         -1 -> erreur causée par open()
     *         -2 -> erreur causée par un mmap
     *         -3 -> erreur causée par un munmap
     *         -4 -> erreur causée par close()
     *         -5 -> erreur causée par fstat()
     *          -6 -> erreur causée par malloc()
     *          -7 -> erreur causée par msync()

    int in = open(input, O_RDONLY);
    if (in == -1) { return -1; }
    int out = open(output_file, O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
    if (out == -1) { return -1; }

    struct stat buf;
    int fs = fstat(in, &buf);
    if (fs == -1) {
        close(in);
        close(out);
        return -5;
    }

    if (buf.st_size == 0) {        //exception où le fichier est vide
        int j = close(in);
        if (j == -1) { return -1; }
        return close(out);
    }

    long *tabin = (long *) mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, in, 0);
    if (*tabin == -1) {
        close(in);
        close(out);
        return -2;
    }
    int size = 0;
    Repertoire_t *rep = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (rep == NULL) {
        munmap(tabin, buf.st_size);
        close(in);
        close(out);
        return -6;
    }

    rep->nbre_elem =
    rep->maximum = 0;
    is_prime(1000, rep);
    for (long i = 0; i < buf.st_size / sizeof(long); i++) {
        long number = tabin[i];

        if (number < 2) {
            char *message = (char *) malloc(sizeof("Erreur: nombre inférieur à 2 !\n")); // +1 pr le \0 ??
            if (message == NULL) { return -6; }
            message = "Erreur: nombre inférieur à 2 !\n";

            size += sizeof(message);
            int ff = ftruncate(out, size);      // On modifie la taille du fichier pr pouvoir écrire dedans
            if (ff == -1) {
                munmap(tabin, buf.st_size);
                close(in);
                close(out);
                printf("Mémoire insufisante dans le fichier de sortie");
                return -5;
            }
            char *tabout = (char *) mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, out, 0);
            if ((int) *tabout == -1) {
                munmap(tabin, size);
                close(in);
                close(out);
                return -2;
            }
            memcpy(tabout, message, sizeof(&message));

            int s = msync(tabout, size, MS_SYNC);
            free(message);     // Libère la mémoire

            if (s == -1) {
                munmap(tabin, buf.st_size);
                munmap(tabout, buf.st_size);
                close(in);
                close(out);
                return -7;
            }
            int f = munmap(tabout, size);
            if (f == -1) {
                munmap(tabin, buf.st_size);
                close(in);
                close(out);
                return -3;
            }
        } else {

            char *message = prime_divs(number, rep);
            size += sizeof(*message);  // !!!! Est-ce que ca fait bien référence au contenu de message ?!!!!!!

            char *tabout = (char *) mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, out, 0);
            if ((int) *tabout == -1) {
                munmap(tabin, size);
                close(in);
                close(out);
                return -2;
            }
            memcpy(tabout, &message, sizeof(*message));
            int s = msync(tabout, size, MS_SYNC);

            free(&message);     // Libère la mémoire

            if (s == -1) {
                munmap(tabin, buf.st_size);
                munmap(tabout, buf.st_size);
                close(in);
                close(out);
                return -7;
            }
            int f = munmap(tabout, size);
            if (f == -1) {
                munmap(tabin, buf.st_size);
                close(in);
                close(out);
                return -3;
            }
        }
    }

    int m = munmap(tabin, buf.st_size);
    if (m == -1) {
        close(in);
        close(out);
        return -3;
    }
    int j = close(in);
    int i = close(out);
    if ((i == -1) | (j == -1)) { return -4; }
    return 0;
}*/