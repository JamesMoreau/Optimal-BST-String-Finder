#include "vector.h"
#include "set.h"

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
node* node_constructor(char* key, double probability);
int num_occurences(vector* all_words, char* word);
int compare_keys(const void* a, const void* b);
int find_largest_probability(vector* words, int left_bound, int right_bound);
void make_tree(vector* words, int left_bound, int right_bound, node** parent_child);


#endif