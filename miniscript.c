#include "Prime.h"
#include "Prime_thread.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


double *test(double *time_array){

	double time_simple;
	double time_thread;
	
	clock_t t;
	t = clock();
	principale_simple("Input.txt","Output_simple.txt");
	t = clock()-t;
	time_simple =(((double)t)/CLOCKS_PER_SEC)*1000;


	clock_t t_thread;
	t_thread = clock();
	principale(4,"Input.txt","Output_thread.txt");
	t_thread = clock()-t_thread;
	time_thread = (((double)t_thread)/CLOCKS_PER_SEC)*1000;


	time_array[0] = time_simple; time_array[1] = time_thread;
	return time_array;
}

int main() {
	double time_array[2];
    double * time_ptr = test(time_array);
	printf("temps mis pour l'execution de l'exemple d'input normal : %f [ms]\n",time_ptr[0]);
	printf("temps mis pour l'execution de l'exemple d'input avec thread : %f [ms]\n",time_ptr[1]);
	return 0;
}
