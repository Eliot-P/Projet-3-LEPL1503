/*
 Created by Constantin on 16-03-20.

    - Traduction du code python -
*/
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

typedef struct repertoire{  //Initialiser à maximum = 0 et nbre_elem = 0
    long maximum;
    int nbre_elem;
    long *liste;
} Repertoire_t;

int is_div(long number, long i){  // True si i divise number, sinon false
    return number%i == 0;
}
// PAS OUBLIER DE FREE LA LISTE DES NBRE PREMIER A LA FIN DU FICHIER

int  is_prime(long number, Repertoire_t *tab){
    /*
     * Retourne 1 si number est premier, 0 si number ne l'est pas et -1 en cas d'erreur
     * La manière de procéder est la suivante:
     *
     *  - 1) (*tab).liste ne contient aucun élément: on va donc calculer de 2 à number-1 et ajouter tout les nbres premier
     *  que l'on  rencontre dans la liste (*tab).liste. Ensuit on retourne le résultat adéquat
     *
     *  -2) (*tab).liste est non vide:
     *      - On regarde si number est un/mutliple d'un des éléments  de (*tab).liste
     *      - Sinon, on va regarder du (*tab).maximum à number -1 et à nouveau, ajouter tout les nbres premiers que l'on
     *      rencontre dans (*tab).liste et si jamais number est un nbre premier on l'ajoute également dans la liste.
     */
    if ((*tab).nbre_elem == 0){   // cas du premier appel

        (*tab).liste = (long *) malloc(sizeof(((*tab).nbre_elem + 1) * sizeof(long)));
        if ((*tab).liste == NULL){ return -1;}

        (*tab).liste[0] = (long) 2;
        (*tab).nbre_elem = (long) 1;
        (*tab).maximum = (long) 2;

        for (long i = 3; i <= number; i++) {
            int div = 0;
            for (long j = (*tab).liste[0]; j < (*tab).liste[(*tab).nbre_elem - 1]; j++) {
                // On regarde si i est un multiple d'un nbre premier
                if (i % j == 0) {
                    div++;
                    break;
                }
            }
            if ((div == 0) && (i > (*tab).maximum)) {
                for (long j = (*tab).maximum; j < i; j++)
                    if (i % j == 0) {
                        div++;
                        break;
                    }
            }
            if (div == 0) {
                (*tab).liste = realloc((*tab).liste, sizeof(((*tab).nbre_elem + 1) * sizeof(long)));
                if ( (*tab).liste == NULL) { return -1; } //> comment gérer l'erreur de realloc ?
                (*tab).liste[(*tab).nbre_elem++] = i;
                (*tab).maximum = i;
            }
        }
        return (*tab).maximum == number; // Si number est premier, il est le dernier élément dans liste
    }
    else {
        for (long i = 0; i < (*tab).nbre_elem; i++) {
            //printf("%ld",(*tab).liste[i]);
            if ((*tab).liste[i] == number) { return 1; }
            if (number % (*tab).liste[i] == 0) {
                break;
            }  // cas où number est un multiple d'un des nbre premiers
        }

        for (long i = (*tab).maximum; i <= number; i++){
            int div = 0;
            for (long j = 2; j < (*tab).nbre_elem; j++){ // On regarde élément par élement si c'est un nbre premier
                if (i % (*tab).liste[j] == 0){  // cas où l'élément i est un multiple d'un nbre premier connu
                    div++;                      // cette boucle permet d'éliminer une grosse partie des nbre
                    break;                      // > paires, multiples de 3 etc...
                }
            } // On a donc un nbre i qui est fort intéressant...
            if (div == 0){  // signifie qu'aucun nbre premier de la liste ne divise i
                for (long a = (*tab).maximum; a < i; a++){
                    if (i%a == 0){  //on regarde alors si aucun nbre ne divise i
                        div++;
                        break;
                    }
                }
            }
            if (div == 0){  //On a bien un nbre premier et donc on doit réajuster la taille de la structure pour le stocker
                (*tab).liste = realloc(& (*tab).liste, sizeof(((*tab).nbre_elem +1) * sizeof(long)));
                if ( (*tab).liste == NULL){ return  -1;}
                (*tab).liste[(*tab).nbre_elem++] = i;
                (*tab).maximum = i;
            }
        }
        return  (*tab).maximum == number;
    }
}

char *prime_divs(long number, Repertoire_t *tab){
    /*  retourne un tableau de char contenant tout les diviseurs premiers de number
     * > alloue dynamiquement de la mémoire pour créer le tableau
    */

    int ok = is_prime(number,tab);
    if (ok == 1){
        return (char *) number;
    }
    char *arr = (char *) malloc(sizeof((char) number +1));
    if (arr == NULL){ return  "Erreur avec Malloc";}
    int index = 0;
    arr[index++] = (char) number;
    arr[index++] = ' '; // Pour avoir un espace entre les nbre !!

    for (long j = 0; j < (*tab).nbre_elem; j++){    // Tout nbre non premier est un multiple de nbre premiers
        if  (is_div(number,(*tab).liste[j])){
            arr = (char *) realloc( arr, sizeof(arr) + sizeof((char) j) +1);   // OK???
            if (arr == NULL){ return  "Erreur avec Realloc";}
            arr[index++] = (char) j;
            arr[index++] = ' ';
        }
    }
    printf("%s",arr);
    return arr;
}

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
     */
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

    rep->nbre_elem = 0;
    rep->maximum = 0;

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
}