#include "Prime_thread.h"
#include <stdio.h>
int main(int argc,char *filename[]) {
    printf("Ouverture du fichier %s\n",filename[1]);
    principale_th(filename[1],"Output_thread.txt");
    printf("fin de l'execution");
    return 0;
}
