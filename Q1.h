#include <stdbool.h>

#ifndef __Q1__
#define __Q1__

typedef struct {
    char* word;
    int frequency;
} data_node;

typedef struct node node;

struct node {
	char* key;
	double probability;
	node* children[2];
};

void read_file(char* filename);
node* node_constructor(char* key, double probability);
void insert_node(node** current, node* new_node);
int* in_vector(vector* v, char* key);

#endif