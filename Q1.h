#include "vector.h"

#ifndef __Q1__
#define __Q1__

#define TABLE_ROWS 601
#define TABLE_COLUMNS 601

typedef struct {
	double probability;
	int root_index;
} cell;

/* tree stuff */
void make_tree(cell** C, vector* words, int left_bound, int right_bound, node** parent_child);

/* algorithm */
int minimum_cost(cell** C, int i, int j);
double weight(vector* v, int i, int j);

/* setup */
vector* read_file(char* filename);
int num_occurences(vector*, char*);
void fill_zeroes(cell** table);
int compare_keys(const void* a, const void* b);
char *strdup(const char *c);

#endif