#include "test_lib.h"
#include "Prime.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	if((test())==0)
		return EXIT_SUCCESS;
	else
		return test();
}