#include "fact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <math.h>


long timeval_diff(struct timeval *t2, struct timeval *t1)
{
  long diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  return (diff);
}
// n : number of iterations of the loop
void compute(int n) {
  // long computation
  double sum=0;
  int i;
  for(i=0;i<n;i++) {
    sum=sum/2 + i*3*2 - 4;
  }
}

int main(int argc,char* modes[]) {
    //Lecture du mode d'execution (pour savoir si le nombre de thread est spécifié ou pas)
    //Le nombre de thread optimal peut être changé au début du fichier, à noté que ce nombre est totalement aribitraire
    int nbre_thread_opti = 4;   
    int ok; 
    
    struct timeval start, fin;
    gettimeofday(&start, NULL);
    if (strcmp(modes[1],"-N")==0){
    printf("Execution avec %i threads\n",atoi(modes[2]));
    ok = principale(atoi(modes[2]),modes[3],modes[4]);
    }

    else {
    printf("Execution avec %d threads\n",nbre_thread_opti);
    ok = principale(nbre_thread_opti,modes[1],modes[2]);  
    }

    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    gettimeofday(&fin, NULL);
    printf("%d\n",timeval_diff(&start,&fin));
    double temps = (fin.tv_sec - start.tv_sec) * 1e6;  // temps en [s]
    temps = (temps + (fin.tv_usec - start.tv_usec)) * 1e-6;
    printf("Valeur de retour : %i",ok);
    if (ok == -1){printf(" -> erreur à l'ouverture.");}
    if (ok == -2){ printf(" -> erreur causée par un malloc.");}
    if (ok == -3){printf(" -> erreur lors de la création d'un thread.");}
    if (ok == -4){printf(" -> erreur lors de la fermeture.");}
    printf("\n");
    
    printf("Mémoire utilisée = %ld kBytes\n",r_usage.ru_maxrss);
    printf("Temps d'exécution = %f [s]\n", temps);
    return 0;
}
