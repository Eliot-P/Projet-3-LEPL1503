#include "fact.h"
#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

int test_is_div(){
    // Test de la fonction is_div
    CU_ASSERT_TRUE(is_div(2, 2));
    CU_ASSERT_TRUE(is_div(8, 2));
    CU_ASSERT_FALSE(is_div(19, 2));
    CU_ASSERT_FALSE(is_div(19, 3));
    return 0;
}

int test_is_prime(){
    //test de la fonction is_prime
    //D'abord on initialise une structure test afin que la fonction puisse effectuer les changements dessus
    Repertoire_t_th *test_struct = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (test_struct == NULL){ return -1;}
    pthread_mutex_t mutex;
    //Liste des 30 premiers nimbre premiers
    unsigned long long array_of_prime[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};
    for (int i = 0; i < 30; i++){
        //D'abord on test que la fonction les considère bien comme des nombres premiers
        CU_ASSERT_TRUE(is_prime(array_of_prime[i],test_struct,&mutex));
        //Ensuite on vérifie que chaque nombre est bien ajouté dans la liste
        CU_ASSERT_EQUAL(test_struct->liste[i], array_of_prime[i]);
        //Enfin on vérifie aussi que le nombre d'élément est aussi actualisé dans la structure
        CU_ASSERT_EQUAL(test_struct->nbre_elem, i+1);
    }
    //Liste des 30 premiers nombres non-premiers
    unsigned long long array_of_not_prime[] = {1,4,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28,30,32,33,34,35,36,38,39,40,42,44};
    //On fixe aussi le nombre d'éléments présent dans la liste
    int nbr_no_change = test_struct->nbre_elem;
    for (int i = 0; i < 30; i++){
        //D'abord on test que la fonction les considère bien comme des nombres non-premiers
        CU_ASSERT_FALSE(is_prime(array_of_not_prime,test_struct,&mutex));
        //Ensuite on vérifie que ces nombres ne sont pas ajouté dans la liste et que celle-ci n'a pas changé
        CU_ASSERT_EQUAL(test_struct->liste[i],array_of_prime[i]);
        //Enfin on vérifie que le nombre d'élément dans la liste n'a pas changé non plus
        CU_ASSERT_EQUAL(test_struct->nbre_elem,nbr_no_change);
    }
    return 0;
}


int test_prime_divs(){
    //Test de la fonction prime_divs
    //D'abord on initialise une structure test afin que la fonction puisse effectuer les changements dessus
    Repertoire_t_th *test_struct = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (test_struct == NULL){ return -1;}
    //On initialise aussi un thread unique pour la fonction
    pthread_mutex_t mutex;
    //Liste des 30 premiers nombres premiers
    unsigned long long array_of_prime[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113};
    //Liste des 30 premiers nombres non-premiers
    unsigned long long array_of_not_prime[] = {1,4,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28,30,32,33,34,35,36,38,39,40,42,44};
    for (int i = 0; i < 30; i++){
        //D'abord on vérifie que chaque nombre premier est bien ajouté dans la liste
        // !! cette liste se comporte comme une liste FIFO (first in first out) donc le dernier élément ajouté se trouve à l'index 0
        CU_ASSERT_EQUAL(prime_divs(array_of_prime[i],test_struct,&mutex)->liste[0],array_of_prime[i]);
        //Enfin on vérifie aussi que le nombre d'élément est aussi actualisé dans la structure
        CU_ASSERT_EQUAL(test_struct->nbre_elem, i+1);
        }
    //On fixe le nombre d'élément dans la liste
    int nbr_no_change = test_struct->nbre_elem;
    //On fixe le dernier élément ajouté dans la liste
    unsigned long long list_no_change = test_struct->liste[0];
    for (int i = 0; i < 30; i++){
        //D'abord on vérifie que le nombre non-premier n'a pas été ajouté dans la liste (donc ne se trouve pas à l'index 0)
        CU_ASSERT_NOT_EQUAL(prime_divs(array_of_prime[i],test_struct,&mutex)->liste[0],array_of_not_prime[i]);
        //Ensuite on vérifie que l'élément à l'index 0 est toujours le même
        CU_ASSERT_EQUAL(test_struct->liste[0],list_no_change);
        //Enfin on vérifie que le nombre d'élément dans la liste est toujours le même
        CU_ASSERT_EQUAL(test_struct->nbre_elem,nbr_no_change);
    }
    return 0;
}


int main (){
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Prime test", 0, 0);
    CU_add_test(suite, "is_div Test", test_is_div);
    CU_add_test(suite,"is_prime_test",test_is_prime);
    CU_add_test(suite,"prime_divs_test",test_prime_divs);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}