// Début traduction code python

int is_div(int number, int i){  // True si i divise number, sinon false
    return number%i == 0;
}

int  is_prime(int number){  // Vérifie si un nbre est premier
    for (int i = 2; i < number; i++){      
        if (is_div(number, i)){ return 0;}
    }
    return 1;
}

int *prime_divs(int number){ // Employer malloc pr créer un tableau ?
    int *prime_dividers[1];
    *prime_dividers[0] = number;
    int index = 1;
    for (int i = 2; i < number; i++){
        if (is_prime(number) && is_div(number,i)){
            *prime_dividers[index++] = i;
        }
    }
    return *prime_dividers;
}