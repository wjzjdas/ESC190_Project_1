#if !defined(AUTOCOMPLETE_H)
#define AUTOCOMPLETE_H

#include <string.h>

typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;

//Test
void read_in_terms(struct term **terms, int *pnterms, char *filename){
    //open the files
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d\n", pnterms);
    // create spaces of memory for each term
    *terms = (term *)malloc(*pnterms * sizeof(term));
    for (int i = 0; i < *pnterms; i++){
        fscanf(file, "%lf\t%[^\n]", &((*terms)[i].weight), (*terms)[i].term);
    }
    fclose(file);
    qsort(*terms, *pnterms, sizeof(term), string_compare);
}

int lowest_match(struct term *terms, int nterms, char *substr){
    int min = 0;
    int max = nterms - 1;
    int final_index = -1;
    int str_len = strlen(substr);

    while (min < max){ //by binary search and each compares the term for their first 3 letters similarity
        int mid = min + (max - min) / 2;
        int compare = strncmp(terms[mid].term, substr, str_len);
        if (compare >= 0){
            max = mid - 1;
            if (compare == 0){
                final_index = mid;
            }
        }
        else{
            min = mid + 1;
        }
    }
    return final_index;
}

int highest_match(struct term *terms, int nterms, char *substr){
    int index = -1;

    //For Binary Searching
    int min = 0;
    int max = nterms - 1;
    int len = strlen(substr);

    // min <= max because we are finding the highest match, so we must continue the loop until compare goes from 0 to positive
    while (min <= max){ 
        
        // [enf(min) eng enga(mid) engb enh enha(max)] 
        // for even lists, We are using the element before the median as mid
        int mid = min + (max - min) / 2; 

        char temp[200];
        strcpy(temp, terms[mid].term); 

        int compare = strcmp(substr, temp); // -1, 0, 1


        if (compare <= 0){ 
            if (strlen(temp) >= len && substr[len-1] >= temp[len-1]){
                index = mid;
                min = mid + 1; // so now we are left with [engb(min) enh enha(max)]
            } else {
                max = mid - 1;
            }
        } else {
            max = mid - 1;
        }
    }
    return index;
}
void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){

}

#endif
