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
		double probability = ((double) num_occurences(all_words, word)) / all_words->total;
		
		vector_add(probability_table, node_constructor(word, probability));
	}

	/* cleanup */
	for (int i = 0; i < num_words; i++) {
		free(arr[i]);
	}
	free (arr);
	
	set_destroy(unique_words);
	free(unique_words);

	for (int i = 0; i < all_words->total; i++) {
		free(vector_get(all_words, i));
	}
	vector_free(all_words);
	free(all_words);

	fclose(f);

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

void print_probabilities(cell** table) {
	printf("______________TABLE______________\n");
	for (int i = 0; i < TABLE_ROWS; i++) {
		for (int j = 0; j < TABLE_COLUMNS; j++) {
			printf ("%.1lf\t", table[i][j].weight);
		}
		printf("\n");
	}
}

void print_roots(cell** table) {
	printf("____________ROOT_TABLE____________\n");
	for (int i = 0; i < TABLE_ROWS; i++) {
		for (int j = 0; j < TABLE_COLUMNS; j++) {
			printf ("%d\t", table[i][j].root_index);
		}
		printf("\n");
	}
}

void fill_zeroes(cell** table) {
	for (int i = 0; i < TABLE_ROWS; i++) {
		for (int j = 0; j < TABLE_COLUMNS; j++) {
			table[i][j].weight = 0;
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

double minimum_cost(cell** C, int i, int j) {
	double minimum = INT_MAX;
	int min_root_index = -1;

	for (int k = i + 1; k <= j; k++) {
		double cost = C[i][k - 1].weight + C[k][j].weight;
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

void make_tree(cell** C, vector* words, int left_bound, int right_bound, node** parent_child) {
	if (left_bound < 0 || right_bound < 0 || right_bound > words->total) return;

	int sub_root_index = C[left_bound][right_bound].root_index;
    if (sub_root_index < 0) return;
	node* sub_root = vector_get(words, sub_root_index);
	(*parent_child) = sub_root;
    sub_root->probability = C[left_bound][right_bound].weight;

	make_tree(C, words, left_bound, sub_root_index, &(sub_root->children[0])); //left tree
	make_tree(C, words, sub_root_index + 1, right_bound, &(sub_root->children[1])); //right tree
}

int main() {
	vector* words = read_file("data_7.txt");
	qsort(words->items, words->total, sizeof(node*), compare_keys);

	cell** C = calloc(TABLE_COLUMNS, sizeof(cell*));
	for (int i = 0; i < TABLE_COLUMNS; i++) {
		C[i] = calloc(TABLE_COLUMNS, sizeof(cell));
	}
	
	fill_zeroes(C);

	for (int k = 2; k <= TABLE_ROWS; k++) { // fill the table
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			C[i][j].weight = minimum_cost(C, i, j) + weight(words, i, j);
		}
	}

	int root_index = C[0][TABLE_COLUMNS - 1].root_index;
	node* root = vector_get(words, root_index); //upper right cell
	root->probability = C[0][TABLE_COLUMNS - 1].weight;

	make_tree(C, words, 0, root_index, &(root->children[0])); // make left children
	make_tree(C, words, root_index + 1, words->total, &(root->children[1])); // make right children

	char user_input[1000]; user_input[0] = '\0';

	printf("Enter a key: ");
	scanf("%s", user_input);

	node* to_find = find_node_optimal(root, user_input);

	if (!to_find) {
		printf("Not found.\n");
	}

	for (int i = 0; i < words->total; i++) {
		node* to_delete = vector_get(words, i);
		free(to_delete->key);
		free(to_delete);
	}
	vector_free(words);
	free(words);

	for (int i = 0; i < TABLE_ROWS; i++) {
		free(C[i]);
	}
	free(C);

	return 1;
}


