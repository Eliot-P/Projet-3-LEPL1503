#include "Prime.h"
#include "Prime_thread.h"
#include "test_files.h"
int main() {
    //printf("%s",test());
    double time_array[3];
    double * time_ptr = test(time_array);
    printf("temps mis pour l'execution de l'exemple d'input : %f [ms]\ntemps mis pour l'execution de l'exemple d'input avec thread : %f [ms]\ntemps mis pour l'execution de l'exemple d'input en python : %f [ms]",time_ptr[0],time_ptr[1],time_ptr[2]);
    return 0;
}
