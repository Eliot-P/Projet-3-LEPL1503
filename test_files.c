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
		clock_t t;
		t = clock();
		principale_simple("Input.txt","Output_simple.txt");
		t = clock()-t;
		time_simple =(((double)t)/CLOCKS_PER_SEC)*1000;
	}
	if (!(atoi(argv[1]))){time_simple = -1;}


	if (atoi(argv[2])){
	clock_t t_thread;
	t_thread = clock();
	principale(4,"Input.txt","Output_thread.txt");
	t_thread = clock()-t_thread;
	time_thread = (((double)t_thread)/CLOCKS_PER_SEC)*1000;
	}
	if (!(atoi(argv[2]))){time_thread = -1;}


	if (atoi(argv[3])){
		Py_Initialize();
		FILE *py = fopen("prime_divs_template.py","r");
		clock_t t_python;
		t_python = clock();
		PyRun_SimpleFile(py,"prime_divs_template.py");
		t_python = clock()-t_python;
		time_python = (((double)t_python)/CLOCKS_PER_SEC)*1000;
		fclose(py);
		Py_Finalize();
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
