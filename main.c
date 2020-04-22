#include "Prime_thread.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *filename[]) {
    printf("Ouverture du fichier %s\n",filename[1]);
    principale(atoi(filename[2]),filename[1],"Output_thread.txt");
    printf("fin de l'execution\n");
    return 0;
}
