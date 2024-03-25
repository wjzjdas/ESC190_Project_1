#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"

// part 1
// helper function to help the qsort function sort the term in lexigraphical order
int term_compare(const void *charA, const void *charB){
    return strcmp(((const term*)charA) -> term, ((const term*)charB) -> term);
}

void read_in_terms(term **terms, int *pnterms, char *filename){
    //open the files and read the terms
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d\n", pnterms);
    // create spaces of memory for each term
    *terms = (term *)malloc(*pnterms * sizeof(term));
    if (*terms == NULL){
        printf("No memory is available for the allocation");
        fclose(file);
        exit(1);
    }
    char buffer[250];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) && i < *pnterms){ // starting going through the file and sort each character elements
        char *find_tab = strchr(buffer, '\t'); // find the tabs in the character string
        if (find_tab != NULL){
            *find_tab = '\0'; // using null character to end one term
            (*terms)[i].weight = atof(buffer); // converting the term into double type
            strncpy((*terms)[i].term, find_tab + 1, sizeof((*terms)[i].term) - 1);
            // setting each character and complete them
            int size = sizeof((*terms)[i].term);
            (*terms)[i].term[size - 1] = '\0'; //setting every last character to null to end it
            char* new_line = strchr((*terms)[i].term, '\n'); //ends the new line with '0'
            if (new_line != NULL){
                *new_line = '\0';
            }
        }
        i++;
    }
    fclose(file);
    qsort(*terms, *pnterms, sizeof(term), term_compare);
}

// part 2
int lowest_match(term *terms, int nterms, char *substr){
    int min = 0;
    int max = nterms - 1;
    int final_index = -1;
    int str_len = strlen(substr);

    while (min <= max){ //by binary search and each compares the term for their first 3 letters similarity
        int mid = min + (max - min) / 2;
        int compare = strncmp(terms[mid].term, substr, str_len);
        if (compare < 0){
            min = mid + 1;
        } 
        else{
            if (compare == 0){
                final_index = mid; // keep looking for lest 
            } 
            max = mid - 1;
        }
    }
    return final_index; // no matches were found then -1 is returned
}


// part 3
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
        // char temp[200];
        // strcpy(temp, terms[mid].term); 
        int compare = strncmp(terms[mid].term, substr, len); // -1, 0, 1
        if (compare <= 0){ // the substr is bigger
            if (compare == 0){ // if two strings matches
                index = mid;
                min = mid + 1; // so now we are left with [engb(min) enh enha(max)], keep searching the right
            } 
            min = mid + 1; // the substr is before the selected element
        } 
        else{ // substr is after
            max = mid - 1; // the substr is after the selected element in term
        }
    }
    return index;
}


// part 4
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
