#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Prime.h"
#include "test_lib.h"



int main() {
    /*
    Repertoire_t *a = (Repertoire_t *) malloc(sizeof(struct repertoire));
    a->liste = (long *) malloc(sizeof(long) *4);
    a->liste[0] = (long) 2; a->liste[1] = (long) 3; a->liste[2] = (long) 5; a->liste[3] = (long) 7;
    a->maximum = 7;
    a->nbre_elem = 4;

    time_t depart = time(NULL);
    is_prime(2000000,a);
    time_t intermediaire = time(NULL);
    //is_prime(1000000,a);
    time_t fin = time(NULL);
    printf("Nombre d'élements: %i\nPlus grand nombre premier: %ld\n",a->nbre_elem,a->maximum);
    printf("Temps mis pour 2 000 000 : %li [s]\nTemps mis pour 1 000 000 : %li [s]",(intermediaire-depart),(fin-intermediaire));
    /*
    char *mess = prime_divs(100,a);
    printf("%c",*mess);
    */
    test();
    return 0;
}
