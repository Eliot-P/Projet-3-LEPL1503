#include "Prime.h"
#include "Prime_thread.h"
#include <time.h>
#include <stdio.h>
double *test(double *time_array){
	clock_t t;
	t = clock();
	principale("testPrincipaleInput.txt","testPrincipaleOutput.txt");
	t = clock()-t;
	double time_simple =(((double)t)/CLOCKS_PER_SEC)*1000;
	clock_t t_thread;
	t_thread = clock();
	principale_th("testPrincipaleInput.txt","testPrincipaleOutput_th.txt");
	t_thread = clock()-t_thread;
	double time_thread = (((double)t_thread)/CLOCKS_PER_SEC)*1000;
	time_array[0] = time_simple; time_array[1] = time_thread;
	return time_array;
}