#include "Prime.h"
#include <time.h>
double test(){
	clock_t t;
	t = clock();
	principale("testPrincipaleInput.txt","testPrincipaleOutput.txt");
	t = clock()-t;
	double time = (((double)t)/CLOCKS_PER_SEC)*1000;
	return time;
}