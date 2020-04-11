#ifndef _PRIME_H_
#define _PRIME_H_
	typedef struct repertoire{
    long maximum;
    int nbre_elem;
    long *liste;
} Repertoire_t;
	int is_div(int, int);
	int  is_prime(long, Repertoire_t *);
	char prime_divs(long,Repertoire_t *);
	int principale(char *, char *);
	void imprimer(Repertoire_t *);
#endif