#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "vector.h"
#include "set.h"
#include "Q1.h"

vector* read_file(char* filename) {
	FILE* f = fopen(filename, "r");
	if (!f) exit(EXIT_FAILURE);

	char buffer[1000]; buffer[0] = '\0';

	simple_set* unique_words = malloc(sizeof(simple_set)); set_init(unique_words);
	vector* all_words = malloc(sizeof(vector)); vector_init(all_words);

	int counter = 0;
	while (!feof (f)) {
		buffer[0] = '\0';
		fscanf (f, "%s", buffer);
		if (feof(f)) break;
		
		if (counter  == 0) { // this removes garbage value of first word read in by fscanf
			int length = strlen(buffer);
			for (int i = 3; i < length; i++) {
				buffer[i-3] = buffer[i];
			}
			buffer[length - 3] = '\0';
		}

		counter++;
		vector_add(all_words, strdup(buffer));
		set_add(unique_words, buffer);
	}

	uint64_t num_words;
	char** arr = set_to_array(unique_words, &num_words);

	vector* probability_table = malloc(sizeof(vector)); vector_init(probability_table);

	for (int i = 0; i < num_words; i++) {
		char* word = arr[i];
		double probability = ((double) num_occurences(all_words, word)) / num_words;
		
		vector_add(probability_table, node_constructor(word, probability));
	}

	return probability_table;
}

char *strdup(const char *c) {
	char *dup = malloc(strlen(c) + 1);
	if (dup) strcpy(dup, c);
	return (dup);
}

int num_occurences(vector* all_words, char* word) {
	int count = 0;
	for (long int i = 0; i < all_words->total; i++) {
		char* word_to_compare = vector_get(all_words, i);
        if (strcmp(word, word_to_compare) == 0)
            count++;
    }

	return (count);
}

node* node_constructor(char* key, double probability) {
	node* new_node = malloc(sizeof(node));
	
	new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(new_node->key, key);

	new_node->probability = probability;
	
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;

	return new_node;
}

void make_tree(cell** C, vector* words, int left_bound, int right_bound, node** parent_child) {
	if (left_bound < 0 || right_bound < 0 || right_bound > words->total) return;

	printf("recursion.\n");

	int root_index = C[left_bound][right_bound].root_index;
	node* sub_root = vector_get(words, root_index);
	(*parent_child) = sub_root;

	if (!sub_root) {
		exit(EXIT_FAILURE);
	}

	make_tree(C, words, left_bound, root_index - 1, &(sub_root->children[0])); //left tree
	make_tree(C, words, root_index, right_bound, &(sub_root->children[1])); //right tree
}

void print_table(cell** table) {
	printf("______________TABLE______________\n");
	for (int i = 0; i < TABLE_ROWS; i++) {
		for (int j = 0; j < TABLE_COLUMNS; j++) {
			printf ("%.2lf\t", table[i][j].probability);
		}
		printf("\n");
	}
}

void fill_zeroes(cell** table) {
	for (int i = 0; i < 600; i++) {
		for (int j = 0; j < 600; j++) {
			table[i][j].probability = 0;
			table[i][j].root_index = -1;
		}
	}
}

double weight(vector* v, int i, int j) {
	double sum = 0;
	for (int s = i; s < j; s++) {
		node* temp = (node*) vector_get(v, s);
		sum += temp->probability;
	}

	return (sum);
}

int minimum_cost(cell** C, int i, int j) {
	double minimum = INT_MAX;
	int min_root_index = -1;

	for (int k = i + 1; k <= j; k++) {
		// printf("C[%d][%d] = %d, C[%d][%d] = %d\n", i, k - 1, C[i][k - 1].probability, k, j, C[k][j].probability);
		double cost = C[i][k - 1].probability + C[k][j].probability;
		if (cost < minimum) {
			minimum = cost;
			min_root_index = k - 1;
		}
	}

	C[i][j].root_index = min_root_index; // save the index of minimum cost root
	return minimum;
}

int compare_keys(const void* a, const void* b) {
	node* first = *(node**)a;
	node* second = *(node**)b;
	return (strcmp((char*)(*first).key, (char*)(*second).key));
}

int main() {
	vector* words = read_file("data_7.txt");
	// qsort(words->items, words->total, sizeof(node*), compare_keys); //?do we have to sort?

	/* initialize the table */
	cell** C = calloc(TABLE_COLUMNS, sizeof(cell*));
	for (int i = 0; i < TABLE_COLUMNS; i++) {
		C[i] = calloc(TABLE_COLUMNS, sizeof(cell));
	}
	fill_zeroes(C);

	/* making the table */
	for (int k = 2; k <= TABLE_ROWS; k++) { //Iterate over diagonals of the matrix
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			C[i][j].probability = minimum_cost(C, i, j) + weight(words, i, j);
			// printf("C[%d][%d].root: %s\n", i, j, ((node*)vector_get(words, C[i][j].root_index))->key); //useful
		}
	}
	printf("average number of comparisons: %lf\n", C[0][600].probability);

	/* Making tree */
	node* root = vector_get(words, C[0][600].root_index);
	printf("node with key: [%s], has index %d\n", root->key, C[0][600].root_index);

	make_tree(C, words, 0, C[0][600].root_index - 1, &root->children[0]);
	make_tree(C, words, C[0][600].root_index, words->total, &root->children[1]);

	printf("END\n");
	return 1;
}
