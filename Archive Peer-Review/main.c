#include "fact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nbre_thread_opti = 8;
int main(int argc,char *modes[]) {
    //Lecture du mode d'execution (pour savoir si le nombre de thread est spécifié ou pas)
    //Le nombre de thread optimal peut être changé au début du fichier, à noté que ce nombre est totalement aribitraire
    
    if (strcmp(modes[1],"-N")==0){
    printf("Ouverture des fichiers %s et %s\nExecution avec %s threads\n",modes[3],modes[4],modes[2]);
    principale(atoi(modes[2]),modes[3],modes[4]);
    }

    else {
    printf("Ouverture des fichiers %s et %s\n Execution avec %d threads\n",modes[1],modes[2],nbre_thread_opti);
    principale(nbre_thread_opti,modes[1],modes[2]);  
    }

    printf("fin de l'execution\n");
    return 0;
}
