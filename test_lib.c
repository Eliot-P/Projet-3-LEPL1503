#include "Prime.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <sys/fcntl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void test_is_div(){
    CU_ASSERT_TRUE(is_div(2, 2));
    CU_ASSERT_TRUE(is_div(8, 2));
    CU_ASSERT_FALSE(is_div(19, 2));
    CU_ASSERT_FALSE(is_div(19, 3));

}
int test_is_prime(){
    Repertoire_t *test_array = (Repertoire_t *) malloc(sizeof(struct repertoire));
    if (test_array == NULL){ return -1;}
    test_array->liste = (long *) malloc(sizeof(long) *4);
    if (test_array->liste == NULL){return -1;}
    test_array->liste[0] = (long) 2;
    test_array->liste[1] = (long) 3;
    test_array->liste[2] = (long) 5;
    test_array->liste[3] = (long) 7;
    CU_ASSERT_TRUE(is_prime(13,test_array));
    
    CU_ASSERT_FALSE(is_prime(10, test_array));
    int prime_test_array[] = {1, 2, 3, 5, 7, 11, 1223, 51991, 802499};
    for(int i = 0; i < 9; i++){
        CU_ASSERT_TRUE(is_prime(prime_test_array[i], test_array));
    }
    int not_prime_test_array[] = {4, 10, 303, 18252};
    for(int i = 0; i < 9; i++){
        CU_ASSERT_FALSE(is_prime(not_prime_test_array[i], test_array));
    }   
    return 0;
}
    /*



    //test prime_divs()
    for(int i = 0; i < 9; i++){
        CU_ASSERT_EQUAL(prime_divs(prime[i], a), prime[i]);
    }
    CU_ASSERT_EQUAL(prime_divs(4, a), 2);
    int ret1[] = {2, 3, 5};
    for(int i = 0; i < 3; i++){
        CU_ASSERT_EQUAL((prime_divs(4, a) + i), ret1[i]);
    }
    for(int i = 0; i < 3; i++){
        CU_ASSERT_EQUAL((prime_divs(4, a) + i), ret1[i]);
    }
    int ret2[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293};
    for(int i = 0; i < 62 ; i++){
        CU_ASSERT_EQUAL((prime_divs(4, a) + i), ret2[i]);
    }
    */
int main (){
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Prime test", 0, 0);
    CU_add_test(suite, "is_div Test", test_is_div);
    CU_add_test(suite,"is_prime_test",test_is_prime);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
