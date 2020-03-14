#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include <stdbool.h>
#include "Q1.h"

vector* read_file(char* filename) {
	FILE* f = fopen(filename, "r");
	if (!f) exit(EXIT_FAILURE);

	char buffer[1000]; buffer[0] = '\0';
	char *toAdd = NULL;
	vector* input = malloc(sizeof(vector));
	vector_init(input);

	while (!feof (f)) {
		buffer[0] = '\0';
		fscanf (f, "%s", buffer);
		toAdd = malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(toAdd, buffer);
		vector_add(input, toAdd);
	}

	char* temp = vector_get(input, 2045); // bad input
	free(temp);
	vector_delete(input, 2045);

	printf("num_words: %d\n", input->total);
	for (int i = 0; i < input->total; i++) {
		// printf("%s\n", vector_get(input, i));
	}

	return input;
}

int* in_vector(vector* v, char* key) {
	for (int i = 0; i < v->total; i++) {
		if (strcmp(v->items[i], key)) {
			return &i;
		}
	}

	return NULL;
}

node* node_constructor(char* key, double probability) {
	node* new_node = malloc(sizeof(node));
	if (!new_node) exit(EXIT_FAILURE);
	
	new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(new_node->key, key);

	new_node->probability = probability;
	
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;

	return new_node;
}

void insert_node(node** current, node* new_node) {
	if (*current) {
		//node already exists
		if (strcmp((*current)->key, new_node->key) < 0) {
			insert_node(&((*current)->children[0]), new_node);
		} else {
			insert_node(&((*current)->children[1]), new_node);
		}
	} else {
		//put node at current
		(*current) = new_node;
	}
}

int main() {
	read_file("data_7.txt");


	return 1;
}