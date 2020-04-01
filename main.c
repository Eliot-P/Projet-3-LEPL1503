#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Prime.h"
#include "test_lib.h"



int main() {
    clock_t t;
    t = clock();
    test();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("%f\n", time_taken);
    return 0;
}
