/*
 Created by Constantin on 16-03-20.

    - Traduction du code python -
*/
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int is_div(int number, int i){  // True si i divise number, sinon false
    return number%i == 0;
}

int  is_prime(int number){  // Vérifie si un nbre est premier
    for (int i = 0; i < number; i++){
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

int principale(char *input, char *output_file){
    /**
     * pré: input != NULL ; output_file != NULL  Ce sont des fichiers
     * input est un fichier qui contient un élément (int,char,float,...) par ligne
     *
     * post: écris dans chaque ligne de output_file la liste des diviseurs premiers du INT à la ligne correspondante dans
     * input. Si jamais l'élément n'est pas un int, écris une erreur.
     *
     * return : 0 -> aucune erreur survenue
     *         -1 -> erreur causée par open()
     *         -2 -> erreur causée par un mmap
     *         -3 -> erreur causée par un munmap
     *         -4 -> erreur causée par close()
     *         -5 -> erreur causée par fstat()
     *
     */
     int in = open(input, O_RDONLY);
     if (in == -1){return -1;}
     int out = open(output_file, O_RDWR | O_CREAT, S_IRWXU);
     if (out == -1){return -1;}

     struct stat buf;
     int fs = fstat(in, &buf);
     if (fs == -1){
         close(in);
         close(out);
         return -5;
     }

     int size = buf.st_size;
     if (size == 0){        //exception où le fichier est vide
         int j = close(in);
         if (j == -1){return -1;}
         return close(out);
     }

     int ft = ftruncate(out,size);
     if (ft == -1){
         close(out);
         close(in);
         return -6;
     }

     int *tabin = (int *) mmap(NULL,size,PROT_READ,MAP_SHARED,in,0);
     if (*tabin == -1){
         close(in);
         close(out);
         return -2;
     }
     
     // eviter d utiliser mmap
     int *tabout = (int *) mmap(NULL,size,PROT_WRITE|PROT_READ,MAP_SHARED,out,0);
     if (*tabout == -1){
         munmap(tabin,size);
         close(in);
         close(out);
         return -2;
     }

     for (int i = 0; i < size/4; i++){
         int number = tabin[i];

         if (number < 2){
             char a[] = "Erreur: nombre < 2 !\n";
             char b = (char) number;
             char *message = strcpy(&b, (const char *) &a);
             size += sizeof(message);
             int ff = ftruncate(out,size);      // On modifie la taille du fichier pr pouvoir écrire dedans
             if (ff == -1){
                 munmap(tabin,buf.st_size);
                 munmap(tabout,buf.st_size);
                 close(in);
                 close(out);
                 printf("Mémoire insufisante dans le fichier de sortie");
                 return -5;
             }
             memcpy(tabout, (const void *) (message), sizeof(message));

             int s = msync(tabout,size,MS_SYNC);
             if (s == -1){
                 munmap(tabin,buf.st_size);
                 munmap(tabout,buf.st_size);
                 close(in);
                 close(out);
                 return -7;
             }

         }
         else{
             int *tab = prime_divs(number); // Pour avoir le nbre d'élément on fait sizeof()/4 vu que c'est que des int
             char mess = (char) number;
             char arr[sizeof(*tab)/4 +1]; // Est-ce que ca prend bien le nbre de byte du tableau ?
             for (int a = 0; a < sizeof(tab) / 4; a++){
                 arr[a] = (char) tab[a];
             }
             arr[sizeof(tab)] = '\n';
             char *message = strcpy(&mess,arr);
             size += sizeof(message);

             int ftt = ftruncate(*tabout,size);
             if (ftt == -1){
                 munmap(tabin,buf.st_size);
                 munmap(tabout,buf.st_size);
                 close(in);
                 close(out);
                 printf("Mémoire insufisante dans le fichier de sortie");
                 return -5;
             }

             memcpy(tabout, (const void *) (message), sizeof(message));
             int s = msync(tabout,size,MS_SYNC);
             if (s == -1){
                 munmap(tabin,buf.st_size);
                 munmap(tabout,buf.st_size);
                 close(in);
                 close(out);
                 return -7;
             }
         }
     }

     int m = munmap(tabin,buf.st_size);
     int l = munmap(tabout,size);
     if ((l ==-1)|(m==-1)){
         close(in);
         close(out);
         return -3;
     }
     int j = close(in);
     int i = close(out);
     if ((i==-1)|(j==-1)){return -4;}
     return 0;
}