#ifndef _PRIME_H_
#define _PRIME_H_
	typedef struct repertoire{
    long maximum;
    int nbre_elem;
    long *liste;
} Repertoire_t;
	int is_div_simple(int, int);
	int  is_prime_simple(long, Repertoire_t *);
	char prime_divs_simple(long,Repertoire_t *);
	int principale_simple(char *, char *);
	void imprimer_simple(Repertoire_t *);
#endif