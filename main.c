#include "test_lib.h"
#include "Premier_pas.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
    Repertoire_t *rep = (Repertoire_t *) malloc(struct repertoire);
    rep->maximum = 0;
    rep->nbre_elem = 0;
    printf(is_prime(300000,rep));
	return 0;
}
