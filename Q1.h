#include <stdbool.h>
#include "vector.h"

#ifndef __Q1__
#define __Q1__

#define TABLE_ROWS 601
#define TABLE_COLUMNS 601

typedef struct node node;

struct node {
	char* key;
	int frequency;
	node* children[2];
};

typedef struct {
	int value;
	node* root;
} cell;

typedef struct {
	int min_index;
	int minimum;
} min;

vector* read_file(char* filename);
node* node_constructor(char* key, int frequency);
void insert_node(node** current, node* new_node);
void print_table(cell** table);
void print_diagonals(cell** table);
void print_weights(cell** table, vector* test_data);
void print_minimums(cell** table);
void fill_zeroes(cell** table);
int weight(vector* v, int i, int j);
min minimum_cost(cell** C, int i, int j);
int num_occurences(vector*, char*);
char *strdup(const char *c);
#endif