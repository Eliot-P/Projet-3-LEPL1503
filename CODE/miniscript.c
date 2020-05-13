#include "Prime.h"
#include "fact.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

// Chrono pour mesurer le temps d'execution d'un programme //
double chrono_thread(char *in, char *out){
    struct timeval start,end;
    gettimeofday(&start, NULL) ;
    principale(4,in,out);
    gettimeofday(&end,NULL);
	double time_taken; 
    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6; 
    return time_taken;
}

double chrono_simple(char *in, char *out){
    struct timeval start,end;
    gettimeofday(&start, NULL) ;
    principale_simple(in,out);
    gettimeofday(&end,NULL);
	double time_taken; 
    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6; 
    return time_taken;
}
int main(int argc,char* modes[]) {
	double time_thread = chrono_thread(modes[1],modes[3])*1000;
	double time_simple = chrono_simple(modes[1],modes[2])*1000;
	printf("temps mis pour l'execution de l'exemple d'input normal : %f [ms]\n",time_simple);
	printf("temps mis pour l'execution de l'exemple d'input avec thread : %f [ms]\n",time_thread);
	return 0;
	
}
