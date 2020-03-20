#include <stdbool.h>
#include "vector.h"

#ifndef __Q1__
#define __Q1__

#define TABLE_ROWS 601
#define TABLE_COLUMNS 601

typedef struct node node;

struct node {
	char* key;
	double probability;
	node* children[2];
};

typedef struct {
	double probability;
	node* root;
} cell;

typedef struct {
	int min_index;
	int minimum;
} min;

/* printing tools */
void print_table(cell** table);
void print_diagonals(cell** table);
void print_weights(cell** table, vector* test_data);
void print_minimums(cell** table);

/* tree stuff */
node* node_constructor(char* key, double probability);
void make_tree_recursive(cell** C, vector* words, int left_bound, int right_bound, node** parent_child);
int get_index(vector* words, node* to_find);

/* algorithm */
min minimum_cost(cell** C, int i, int j);
double weight(vector* v, int i, int j);

/* setup */
vector* read_file(char* filename);
int num_occurences(vector*, char*);
void fill_zeroes(cell** table);
char *strdup(const char *c);

#endif