//#include "Prime.h"
#include "Prime_thread.c"
#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

int test_is_div(){
    CU_ASSERT_TRUE(is_div(2, 2));
    CU_ASSERT_TRUE(is_div(8, 2));
    CU_ASSERT_FALSE(is_div(19, 2));
    CU_ASSERT_FALSE(is_div(19, 3));
    return 0;

}
int test_is_prime(){
    Repertoire_t_th *test_array = (Repertoire_t_th *) malloc(sizeof(struct repertoire));
    if (test_array == NULL){ return -1;}
    test_array->liste = (long *) malloc(sizeof(long) *4);
    if (test_array->liste == NULL){return -1;}
    test_array->liste[0] = (long) 2;
    test_array->liste[1] = (long) 3;
    test_array->liste[2] = (long) 5;
    test_array->liste[3] = (long) 7;
    CU_ASSERT_TRUE(is_prime(13,test_array));
    CU_ASSERT_FALSE(is_prime(10, test_array));
    long prime_test_array[] = {2, 3, 5, 7, 11, 1223, 51991, 802499};
    for(int i = 0; i < 7; i++){
        CU_ASSERT_TRUE(is_prime(prime_test_array[i], test_array));
    }
    long not_prime_test_array[] = {4, 10, 303, 18252};
    for(int i = 0; i < 3; i++){
        CU_ASSERT_FALSE(is_prime(not_prime_test_array[i], test_array));
    }   
    return 0;
}
    
int test_is_prime(){
     Repertoire_t_th *a = (Repertoire_t_th *) malloc(sizeof(struct repertoire));
    if (a == NULL){ return -1;}
    pthread_mutex_t mutex;
    unsigned long long p[] = {2, 13, 97, 997, 3456863};
    for (int i = 0; i < 5; i++){
        CU_ASSERT_TRUE(is_prime(p[i],a));//,&mutex));
    }
    return 0;
}


int test_prime_divs(){
    Repertoire_t_th *test_array = (Repertoire_t_th *) malloc(sizeof(struct repertoire));
    if (test_array == NULL){ return -1;}
    test_array->liste = (long *) malloc(sizeof(long) *4);
    if (test_array->liste == NULL){return -1;}
    test_array->liste[0] = (long) 2;
    test_array->liste[1] = (long) 3;
    test_array->liste[2] = (long) 5;
    test_array->liste[3] = (long) 7;

    for(int i = 0; i < 7; i++){
        int prime_test_array[] = {2, 3, 5, 7, 11, 1223, 51991, 802499};
        CU_ASSERT_EQUAL(prime_divs(prime_test_array[i], test_array), prime_test_array[i]);
    }
    CU_ASSERT_EQUAL(prime_divs(4, test_array), 2);
    int ret1[] = {2, 3, 5};
    for(int i = 0; i < 3; i++){
        CU_ASSERT_EQUAL((prime_divs(4, test_array) + i), ret1[i]);
    }
    for(int i = 0; i < 3; i++){
        CU_ASSERT_EQUAL((prime_divs(4, test_array) + i), ret1[i]);
    }
    int ret2[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293};
    for(int i = 0; i < (sizeof(ret2)/sizeof(ret2[0])) ; i++){
        CU_ASSERT_EQUAL((prime_divs(4, test_array) + i), ret2[i]);
    }
    return 0;
}


int main (){
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Prime test", 0, 0);
    CU_add_test(suite, "is_div Test", test_is_div);
    CU_add_test(suite,"is_prime_test",test_is_prime);
    CU_add_test(suite,"prime_divs_test",test_prime_divs);
    CU_add_test(suite,"test_is_prime",test_is_prime);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
