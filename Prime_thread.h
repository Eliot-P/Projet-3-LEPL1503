#ifndef _PRIME__THREAD_H_
#define _PRIME__THREAD_H_
	typedef struct repertoire_th{
    long maximum;
    int nbre_elem;
    long *liste;
} Repertoire_t_th;
	int is_div_th(int, int);
	int  is_prime_th(long, Repertoire_t_th *);
	char prime_divs_th(long,Repertoire_t_th *);
	int principale_th(char *, char *);
#endif