#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __BST_API__
#define __BST_API__

typedef struct node node;

struct node {
	char* key;
	double probability;
	node* children[2];
};

node* node_constructor(char* key, double probability);
void add_node(node* to_add, node** root);
void delete_tree(node* root);
void print_tree(node* root);
node* find_node(node* root, char* key);
node* find_node_optimal(node* root, char* key);


#endif