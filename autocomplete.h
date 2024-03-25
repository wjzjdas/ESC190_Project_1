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
int weight_compare(const void *weightA, const void *weightB){
    const term *weight1 = (const term*)weightA;
    const term *weight2 = (const term*)weightB; 
    if (weight1 -> weight < weight2 -> weight){ // first smaller than second, should do qsort
        return 1;
    }
    else if (weight1->weight > weight2 -> weight){ // first smaller than second, in non-increasing order
        return -1;
    }
    else{ // two term weight are equal
        return 0;
    }
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr){
    /*The function takes terms (assume it is sorted lexicographically in increasing order), the number
of terms nterms, and the query string substr, and places the answers in answer, with *n_answer
being the number of answers. The answers should be sorted by weight in non-increasing order.*/

    // find the start of the search in the file for substr using binary search
    int start_index = lowest_match(terms, nterms, substr);
    int end_index = highest_match(terms, nterms, substr);
    // check if there is no similar word found in list in terms wrt substr
    if (start_index == -1 || end_index == -1){
        *n_answer = 0;
        *answer = NULL;
    }

    *n_answer = end_index - start_index + 1;
    // places the answer in *answer pointer to store the memory address of the value
    *answer = (term *)malloc(*n_answer * sizeof(term));
    for (int i = 0; i < *n_answer; i++)
    {
      (*answer)[i] = terms[start_index + i];
    }
    qsort(*answer, *n_answer, sizeof(term), weight_compare);
}

#endif
