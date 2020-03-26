#include "vector.h"

#ifndef __Q1__
#define __Q1__

#define TABLE_ROWS 5
#define TABLE_COLUMNS 5

typedef struct {
	double probability;
	int root_index;
} cell;

/* tree stuff */
void make_tree(cell** C, vector* words, int left_bound, int right_bound, node** parent_child);

/* algorithm */
double minimum_cost(cell** C, int i, int j);
double weight(vector* v, int i, int j);

/* tools */
void print_probabilities(cell** table);
char *strdup(const char *c);

/* setup */
vector* read_file(char* filename);
int num_occurences(vector*, char*);
void fill_zeroes(cell** table);
int compare_keys(const void* a, const void* b);

#endif