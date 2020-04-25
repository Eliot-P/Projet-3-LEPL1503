#include "Prime.h"
#include "Prime_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

double *mini_test(double *time_array){

	double time_simple;
	double time_thread;

	clock_t start_thread,end_thread;
	start_thread = clock();
	principale(4,"Input.txt","Output_thread.txt");
	end_thread = clock();
	time_thread =1000*((double)(end_thread - start_thread) / (double)(CLOCKS_PER_SEC));
	
	clock_t start_simple,end_simple;
	start_simple = clock();
	principale_simple("Input.txt","Output_simple.txt");
	end_simple = clock();
	time_simple =1000*((double)(end_simple - start_simple) / (double)(CLOCKS_PER_SEC));
	
	time_array[0] = time_simple; time_array[1] = time_thread;
	return time_array;
}

long chrono_simple(){
    struct timeval chrono;
    gettimeofday(&chrono, NULL) ;
    long debut = chrono.tv_usec;
    principale_simple("Input.txt","Output_thread.txt");
    gettimeofday(&chrono,NULL);
    long fin = chrono.tv_usec;
    return (fin-debut)/1000;
}
long chrono_thread(){
    struct timeval chrono;
    gettimeofday(&chrono, NULL) ;
    long debut = chrono.tv_usec;
    principale(4,"Input.txt","Output_thread.txt");
    gettimeofday(&chrono,NULL);
    long fin = chrono.tv_usec;
    return (fin-debut)/1000;
}


int main() {
	
	double time_array[2];
    double * time_ptr = mini_test(time_array);
	printf("temps mis pour l'execution de l'exemple d'input normal : %f [ms]\n",time_ptr[0]);
	printf("temps mis pour l'execution de l'exemple d'input avec thread : %f [ms]\n",time_ptr[1]);
	
/*
	long time_thread = chrono_thread();
	long time_simple = chrono_simple();
	printf("temps mis pour l'execution de l'exemple d'input normal : %ld [ms]\n",time_simple);
	printf("temps mis pour l'execution de l'exemple d'input avec thread : %ld [ms]\n",time_thread);
	return 0;
	*/
	
}
