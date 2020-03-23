#include "vector.h"

#ifndef __Q2__
#define __Q2__

typedef struct node node;

struct node {
	char* key;
	double probability;
	node* children[2];
};

vector* read_file(char* filename);
int num_occurences(vector*, char*);

#endif