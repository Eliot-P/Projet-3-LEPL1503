#include "fact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <math.h>

int main(int argc,char* modes[]) {
  //Lecture du mode d'execution (pour savoir si le nombre de thread est spécifié ou pas)
  //Le nombre de thread optimal peut être changé au début du fichier, à noté que ce nombre est totalement aribitraire
  if (argc < 3){return -1;}
  int quiet_condition = 0;
  int nbre_thread_opti = 4;
  int nbre_of_thread;
  int return_value;
  char *in = modes[argc - 2];
  char *out = modes[argc - 1];

  switch (strcmp(modes[1],"-N")){
  case 0 :
    nbre_of_thread = modes[2];
    if (strcmp(modes[3],"-q")==0){quiet_condition = 1;}
    break;
  
  default:
    nbre_of_thread = nbre_thread_opti;
    if (strcmp(modes[1],"-q")==0){quiet_condition = 1;}
    break;
  }
  
  struct timeval start, fin;
  gettimeofday(&start, NULL);
  if(quiet_condition==1){printf("Execution avec %i threads\n",atoi(modes[2]));}
  return_value = principale(nbre_of_thread,in,out);

  struct rusage r_usage;
  getrusage(RUSAGE_SELF,&r_usage);
  gettimeofday(&fin, NULL);
  double temps = (fin.tv_sec - start.tv_sec) * 1e6;  // temps en [s]
  temps = (temps + (fin.tv_usec - start.tv_usec)) * 1e-6;
  if(quiet_condition==1){printf("Valeur de retour : %i",return_value);}
  if (return_value == -1){printf(" -> erreur à l'ouverture.");}
  if (return_value == -2){ printf(" -> erreur causée par un malloc.");}
  if (return_value == -3){printf(" -> erreur lors de la création d'un thread.");}
  if (return_value == -4){printf(" -> erreur lors de la fermeture.");}
  printf("\n");
  
  if(quiet_condition==1){printf("Mémoire utilisée = %ld kBytes\n",r_usage.ru_maxrss);}
  if(quiet_condition==1){printf("Temps d'exécution = %f [s]\n", temps);}
  return 0;
}
