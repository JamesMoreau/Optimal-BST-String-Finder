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
	double value;
	node* root;
} cell;

vector* read_file(char* filename);
node* node_constructor(char* key, int frequency);
void insert_node(node** current, node* new_node);
void print_table(cell table[4][4]);
#endif