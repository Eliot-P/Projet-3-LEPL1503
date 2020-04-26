#ifndef _FACT_H_
#define _FACT_H_

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

	typedef struct repertoire_th{
    int nbre_elem;
    unsigned long long *liste;
} Repertoire_t_th;

	typedef struct entrepot{
    int size;
    int nbre;
    int putindex;
    int takeindex;
    Repertoire_t_th *buffer;
} Entrepot_Th;

	typedef struct lecteur{
    char *ligne;
    FILE *fichier;
    Entrepot_Th *tableau;
    pthread_mutex_t *flag;
    sem_t *semaphores[2];
} Lecteur_Th;

	typedef struct usine{
    Entrepot_Th *tabin;
	Entrepot_Th *tabout;
	Repertoire_t_th *rep;
	pthread_mutex_t *flags[3];
    sem_t *semaphores[4];
}Usine_Th;

	typedef struct imprimerie{
    int N; 
	FILE *fichierOut;   
    pthread_mutex_t *flag;  
	sem_t *semaphores[2];   
	Entrepot_Th *tabout;   
}Imprimerie_Th;

	int is_div(unsigned long long, unsigned long long);
	int  is_prime(unsigned long long, Repertoire_t_th *,pthread_mutex_t *);
	Repertoire_t_th *prime_divs(unsigned long long , Repertoire_t_th *,pthread_mutex_t *);
	int principale(int,char *, char *);
	void *ecriture(void*);
	void *calcul(void*);
	void *lecture(void*);
	int fermer(FILE *, FILE *,int );
	void imprimer(Repertoire_t_th *);
	
#endif