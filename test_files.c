#include "Prime.h"
#include "Prime_thread.h"
#include <time.h>
#include <stdio.h>
#include <Python.h>
#include <stdlib.h>
#include <pthread.h>


double *test(char *argv[],double *time_array){

	double time_simple;
	double time_thread;
	double time_python;

	if (atoi(argv[1])){
    	struct timeval start,end;
    	gettimeofday(&start, NULL) ;
		principale_simple("Input.txt","Output_simple.txt");
		gettimeofday(&end,NULL);
		time_simple = (end.tv_sec - start.tv_sec) * 1e6; 
		time_simple = (time_simple + (end.tv_usec -  start.tv_usec)) * 1e-6; 
	}
	if (!(atoi(argv[1]))){time_simple = -1;}


	if (atoi(argv[2])){
    	struct timeval start,end;
    	gettimeofday(&start, NULL) ;
		principale_simple("Input.txt","Output_simple.txt");
		gettimeofday(&end,NULL);
		time_thread = (end.tv_sec - start.tv_sec) * 1e6; 
		time_thread = (time_thread + (end.tv_usec -  start.tv_usec)) * 1e-6; 
	}
	if (!(atoi(argv[2]))){time_thread = -1;}


	if (atoi(argv[3])){
		Py_Initialize();
		FILE *py = fopen("prime_divs_template.py","r");
    	struct timeval start,end;
    	gettimeofday(&start, NULL) ;
		PyRun_SimpleFile(py,"prime_divs_template.py");
		gettimeofday(&end,NULL);
		fclose(py);
		Py_Finalize();
		time_python = (end.tv_sec - start.tv_sec) * 1e6; 
		time_python = (time_python + (end.tv_usec -  start.tv_usec)) * 1e-6; 
	}
	if (!(atoi(argv[3]))){time_python = -1;}

	time_array[0] = time_simple; time_array[1] = time_thread; time_array[2] = time_python; 
	return time_array;
}

int main(int argc,char *argv[]) {
	double time_array[3];
    double * time_ptr = test(argv,time_array);
	if(time_ptr[0] != -1){
    	printf("temps mis pour l'execution de l'exemple d'input normal : %f [ms]\n",time_ptr[0]);
	}
	if(time_ptr[1] != -1){
    	printf("temps mis pour l'execution de l'exemple d'input avec thread : %f [ms]\n",time_ptr[1]);
	}
	if(time_ptr[2] != -1){
    	printf("temps mis pour l'execution de l'exemple d'input avec python : %f [ms]\n",time_ptr[2]);
	}
	return 0;
}
