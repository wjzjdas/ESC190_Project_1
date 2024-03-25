#if !defined(AUTOCOMPLETE_H)
#define AUTOCOMPLETE_H

#include <string.h>

typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;


void read_in_terms(struct term **terms, int *pnterms, char *filename);
int lowest_match(struct term *terms, int nterms, char *substr);
int highest_match(struct term *terms, int nterms, char *substr);
void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr);
#endif
