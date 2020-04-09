#include "Prime.h"
#include "Prime_thread.h"
#include <time.h>
#include <stdio.h>
#include <Python.h>
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
	Py_Initialize();
	FILE *py = fopen("prime_divs_template.py","r");
	clock_t t_python;
	t_python = clock();
	PyRun_SimpleFile(py,"prime_divs_template.py");
	t_python = clock()-t_python;
	double time_python = (((double)t_python)/CLOCKS_PER_SEC)*1000;
	fclose(py);
	Py_Finalize();
	time_array[0] = time_simple; time_array[1] = time_thread; time_array[2] = time_python; 
	return time_array;
}