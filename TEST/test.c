#include "../SRC/fact.h"
#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int test_is_div(){
    // Test de la fonction is_div
    CU_ASSERT_TRUE(is_div(2, 2));
    CU_ASSERT_TRUE(is_div(8, 2));
    CU_ASSERT_FALSE(is_div(19, 2));
    CU_ASSERT_FALSE(is_div(19, 3));
    return 0;
}

int test_prime_divs_opti(){
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
    free(test_struct);
    return 0;
}

int test_tricky_cases(){
    Repertoire_t_th *test_struct = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (test_struct == NULL){ return -1;}
    pthread_mutex_t mutex;
    unsigned long long array_of_tricky_number[] = {0};
    CU_ASSERT_FALSE(is_div(1,0));
    CU_ASSERT_TRUE(is_div(0,1));
    for (int i = 0; i < 1; i++){
        CU_ASSERT_EQUAL(prime_divs(array_of_tricky_number[i],test_struct,&mutex)->liste[0],array_of_tricky_number[i]);
        CU_ASSERT_EQUAL(test_struct->nbre_elem, i+1);
        CU_ASSERT_FALSE(is_prime(array_of_tricky_number[i],test_struct,&mutex));
    }
    free(test_struct);
    return 0;
}

int test_output(){
    //fichier d'entrée
    FILE *input_test= fopen("./fichiers_test/input_test.txt","r");
    if (input_test == NULL){return -1;}
    //fichier à comparer avec la sortie
    FILE *output_test= fopen("./fichiers_test/output_test.txt","r");
    if (output_test == NULL){return -1;}
    //création du fichier output
    principale(4, "./fichiers_test/input_test.txt", "./fichiers_test/output.txt");
    FILE *output= fopen("./fichiers_test/output.txt","r");
    if (output == NULL){return -1;}
    char line[30];
    char line_test[30];
    //comparaison entre les deux fichiers
    while(fgets(line, 30, output) != NULL){
        fgets(line_test, 30, output_test);
        CU_ASSERT_EQUAL(*line, *line_test);
    }
    return 0;
}

int AppendNumber_test(){
    //création d'un répertoire pour le test
    Repertoire_t_th *rep = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th));
    if (rep == NULL){
        return -1;
    }
    rep->liste = (unsigned long long *) malloc(sizeof(unsigned long long));
    if (rep->liste == NULL){
        return  -1;
    }
    rep->nbre_elem = 0;

    for(int i = 0; i < 100; i++){
        int number = rand() % 1000; //création d'un nombre random entre 0 et 999
        int err = AppendNumber(number, rep); // ajout de number
        if(err == -1){
            return -1;
        }
        CU_ASSERT_EQUAL(number, rep->liste[i]) //vérifie si number à été ajouté
    }
    return 0;
}

int putNumber_test(){
    for(int j = 0; j<10; j++) {
        //création d'un entrepot pour le test
        Entrepot_Th *tab = (Entrepot_Th *) malloc(sizeof(struct entrepot));
        if (tab == NULL) {
            return -1;
        }
        tab->buffer = (Repertoire_t_th *) malloc(sizeof(struct repertoire_th) * 8);
        if (tab->buffer == NULL) {
            return -1;
        }
        tab->putindex = 0;
        tab->takeindex = 0;
        tab->size = 8;
        tab->nbre = 0;


        for (int i = 0; i < 7; i++) {
            int number = rand() % 1000; //création d'un nombre random entre 0 et 999
            putNumber(tab, number); // ajout de number

            CU_ASSERT_EQUAL((tab->buffer[i]).liste[0], number); //vérifie si number à été ajouté
            CU_ASSERT_EQUAL(1, (tab->buffer[i]).nbre_elem); //verifie que le nombre d'élément à été augmenté à 1
            CU_ASSERT_EQUAL(i + 1, tab->nbre); //verifie que le nbre augmente à chaque ajout
            //free((tab->buffer[tab->putindex]).liste);
        }
    }
    return 0;

}

int test_big_number(){
    //fichier d'entrée
    FILE *input_test= fopen("./fichiers_test/big_numbers_in.txt","r");
    if (input_test == NULL){return -1;}
    //fichier à comparer avec la sortie
    FILE *output_test= fopen("./fichiers_test/big_numbers_out.txt","r");
    if (output_test == NULL){return -1;}
    //création du fichier output
    principale(4, "./fichiers_test/big_numbers_in.txt", "./fichiers_test/output.txt");
    FILE *output= fopen("./fichiers_test/output.txt","r");
    if (output == NULL){return -1;}
    char line[30];
    char line_test[30];
    //comparaison entre les deux fichiers
    while(fgets(line, 30, output) != NULL){
        fgets(line_test, 30, output_test);
        CU_ASSERT_EQUAL(*line, *line_test);
    }
    return 0;
}

int main(){
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Prime test", 0, 0);
    CU_add_test(suite,"tricky_cases",test_tricky_cases);
    CU_add_test(suite, "is_div Test", test_is_div);
    CU_add_test(suite,"is_prime_test",test_is_prime);
    CU_add_test(suite,"prime_divs_test",test_prime_divs_opti);
    CU_add_test(suite,"test_output", test_output);
    CU_add_test(suite,"test_big_number", test_big_number);
    CU_add_test(suite,"AppendNumber_test", AppendNumber_test);
    CU_add_test(suite,"putNumber_test", putNumber_test);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}

