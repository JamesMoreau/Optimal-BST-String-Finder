#include "vector.h"
#include "set.h"

#ifndef __Q2__
#define __Q2__

vector* read_file(char* filename);
int num_occurences(vector*, char*);
int num_occurences(vector* all_words, char* word);
int compare_probabilities(const void* a, const void* b);

#endif