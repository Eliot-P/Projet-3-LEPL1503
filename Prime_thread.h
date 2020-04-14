#ifndef _PRIME__THREAD_H_
#define _PRIME__THREAD_H_
	typedef struct repertoire_th{
    long maximum;
    int nbre_elem;
    long *liste;
} Repertoire_t_th;
	int is_div(int, int);
	int  is_prime(long, Repertoire_t_th *);
	char prime_divs(long,Repertoire_t_th *);
	int principale(char *, char *);
#endif