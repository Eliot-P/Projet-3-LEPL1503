#include "Prime_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int nbre_thread_opti = 8;
int main(int argc,char *modes[]) {
    //Lecture du mode d'execution (pour savoir si le nombre de thread est spécifié ou pas)
    //Le nombre de thread optimal peut être changé au début du fichier, à noté que ce nombre est totalement aribitraire
    
    if (strcmp(modes[1],"-N")==0){
        struct timeval start,end;
        gettimeofday(&start, NULL) ;
        //printf("Ouverture des fichiers %s et %s\nExecution avec %s threads\n",modes[3],modes[4],modes[2]);
        principale(atoi(modes[2]),modes[3],modes[4]);
        gettimeofday(&end,NULL);
        double time_taken; 
        time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
        time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
        printf("%f\n",time_taken);
    }
    
    else {
    printf("Ouverture des fichiers %s et %s\n Execution avec %d threads\n",modes[1],modes[2],nbre_thread_opti);
    principale(nbre_thread_opti,modes[1],modes[2]);  
    }
    
    //printf("fin de l'execution\n");
    return 0;
}
