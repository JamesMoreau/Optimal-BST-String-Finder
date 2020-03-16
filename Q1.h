#include <stdbool.h>

#ifndef __Q1__
#define __Q1__

#define TABLE_ROWS 4
#define TABLE_COLUMNS 4

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

vector* read_file(char* filename);
node* node_constructor(char* key, int frequency);
void insert_node(node** current, node* new_node);
void print_table(cell table[TABLE_ROWS][TABLE_COLUMNS]);
void fill_zeroes(cell table[TABLE_ROWS][TABLE_COLUMNS]);
int weight(vector* v, int i, int j);

#endif