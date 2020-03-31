#include "Prime.h"
#include "CUnit/CUnit.h"
#include <sys/fcntl.h>

void test(){
    /*is_prime :
    -number < 0
    -tab == NULL
    -number == 1, 2, 3, 5, 7, 11, 1223, 51991, 802499 ==> retourne 1
               4, 10, 303, 18252 ==> retourne 0

    prime_divs :
    -tab == NULL
    -number == 2, 3, 5, 7, 11, 1223, 51991, 802499 ==> retourne lui-même
               4, 10, 100 ==> retourne [2] [2,3,5] [2,3,5,...]

    principale:
    -input_file vide
    -ligne != int/long*/
    
    /*
    //test is_div()
    CU_ASSERT_TRUE(is_div(2, 2));
    CU_ASSERT_TRUE(is_div(15, 3));
    CU_ASSERT_FALSE(is_div(19, 2));
    
    //test is_prime()
    Repertoire_t *a = (Repertoire_t *) malloc(sizeof(struct repertoire));
    a->liste = (long *) malloc(sizeof(long) *4);
    a->liste[0] = (long) 2; a->liste[1] = (long) 3; a->liste[2] = (long) 5; a->liste[3] = (long) 7;
    a->nbre_elem = 4;

    CU_ASSERT_TRUE(is_prime(-5, a));
    CU_ASSERT_FALSE(is_prime(-10, a));
    int prime[] = {1, 2, 3, 5, 7, 11, 1223, 51991, 802499};
    for(int i = 0; i < 9; i++){
        CU_ASSERT_TRUE(is_prime(prime[i], a));
    }
    int l[] = {4, 10, 303, 18252};
    for(int i = 0; i < 9; i++){
        CU_ASSERT_FALSE(is_prime(l[i], a));
    }
        
    //test prime_divs()
    for(int i = 0; i < 9; i++){
        CU_ASSERT_EQUALS(prime_divs(prime[i], a), prime[i]);
    }
    CU_ASSERT_EQUALS(prime_divs(4, a), 2);
    int ret1[] = {2, 3, 5};
    for(int i = 0; i < 3; i++){
        CU_ASSERT_EQUALS((prime_divs(4, a) + i), ret1[i]);
    }
    for(int i = 0; i < 3; i++){
        CU_ASSERT_EQUALS((prime_divs(4, a) + i), ret1[i]);
    }
    int ret2[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293};
    for(int i = 0; i < 62 ; i++){
        CU_ASSERT_EQUALS((prime_divs(4, a) + i), ret2[i]);
    }
    */
    //test principale()
    int fdIn = open("testPrincipaleInput.txt", O_CREAT | O_RDWR );
    if(fdIn == -1){return -1;}
    close(fdIn);
    int fdOut = open("testPrincipaleOutput.txt", O_CREAT | O_RDWR,S_IRWXU);
    if(fdOut == -1){return -1;}
    close(fdOut);
    principale("testPrincipaleInput","testPrincipaleOutput");
    
    //je ne sais pas trop comment créer les fichiers
    
    //test avec un fichier vide
    
    //test avec juste des nombres
    
    //test avec des chars,strings..
}