#include "fact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <math.h>


int nbre_thread_opti = 4;
int main(int argc,char* modes[]) {
    //Lecture du mode d'execution (pour savoir si le nombre de thread est spécifié ou pas)
    //Le nombre de thread optimal peut être changé au début du fichier, à noté que ce nombre est totalement aribitraire
    
    struct timeval start, fin;
    gettimeofday(&start, NULL);
    if (strcmp(modes[1],"-N")==0){
    printf("Ouverture des fichiers %s et %s\nExecution avec %i threads\n",modes[3],modes[4],atoi(modes[2]));
    principale(atoi(modes[2]),modes[3],modes[4]);
    }

    else {
    printf("Ouverture des fichiers %s et %s\nExecution avec %d threads\n",modes[1],modes[2],nbre_thread_opti);
    principale(nbre_thread_opti,modes[1],modes[2]);  
    }

    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    gettimeofday(&fin, NULL);
    double temps = (fin.tv_sec - start.tv_sec) * 1e6;  // temps en [ms]
    temps = (temps + (fin.tv_usec - start.tv_usec)) * 1e-6;
    printf("Memory usage = %ld kBytes\n",r_usage.ru_maxrss);
    printf("Temps d'exécution = %f [s]\n", temps);
    return 0;
}
