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
	if (!new_node) exit(EXIT_FAILURE);
	
	new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(new_node->key, key);

	new_node->probability = probability;
	
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;

	return new_node;
}

int get_index(vector* words, node* to_find) {
	for (int i = 0; i < words->total; i++) {
		node* word_to_compare = vector_get(words, i);
		if (strcmp(to_find->key, word_to_compare->key) == 0) {
			return (i);
		}
	}

	printf("INDEX NOT FOUND: SHOULD NOT HAPPEN");
	exit(EXIT_FAILURE);
}

void make_tree(cell** C, vector* words) {
	node* root = C[0][600].root;
	int index = get_index(words, root);
	printf("node with key: %s, has index %d\n", root->key, index);

	make_tree_recursive(C, words, 0, index - 1, vector_get(words, index));
	make_tree_recursive(C, words, index, words->total, vector_get(words, index));
}

node* make_tree_recursive(cell** C, vector* words, int left_bound, int right_bound, node* parent) {
	if (left_bound == right_bound) {
		return;
	}

	int index = 0;
	parent->children[0] = C[left_bound][right_bound].root;
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

void print_diagonals(cell** table) {
	printf("____________DIAGONALS____________\n");
	for (int k = 2; k <= TABLE_ROWS; k++) {
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			printf ("%.2lf\t", table[i][j].probability);
		}
		printf("\n");
	}
}

void print_weights(cell** table, vector* data) {
	printf("_____________WEIGHTS_____________\n");
	for (int k = 2; k <= TABLE_ROWS; k++) {
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			printf("W: %d\t", weight(data, i, j));
		}
		printf("\n");
	}
}

void print_minimums(cell** table) {
	printf("___________MINIMUMS___________\n");
	for (int k = 2; k <= TABLE_ROWS; k++) {
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			printf("M: %d\t", minimum_cost(table, i, j));
		}
		printf("\n");
	}
}

void fill_zeroes(cell** table) {
	for (int i = 0; i < 600; i++) {
		for (int j = 0; j < 600; j++) {
			table[i][j].probability = 0;
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

min minimum_cost(cell** C, int i, int j) {
	double minimum = INT_MAX;
	int min_root_index = -1;
	// printf("i: %d, j: %d\n", i, j);
	for (int k = i + 1; k <= j; k++) {
		// printf("C[i][k-1] = %d, i = %d, k = %d, j = %d\n", C[i][k - 1].value, i, k, j);
		// printf("C[k][j] = %d, i = %d, k = %d, j = %d\n", C[k][j].value, i , k, j);
		double cost = C[i][k - 1].probability + C[k][j].probability;
		if (cost < minimum) {
			minimum = cost;
			min_root_index = k - 1;
		}
	}

	min minimum_struct;
	minimum_struct.min_index = min_root_index;
	minimum_struct.minimum = minimum;

	return minimum_struct;
}

int compare_keys(const void* a, const void* b) {
	node* first = *(node**)a;
	node* second = *(node**)b;
	return (strcmp((char*)(*first).key, (char*)(*second).key));
}

int main() {
	vector* words = read_file("data_7.txt");
	qsort(words->items, words->total, sizeof(node*), compare_keys);

	printf("number of words: %d\n", words->total);
	for (int i = 0; i < words->total; i++) {
		node* node = vector_get(words, i);
		// printf("word: [%s], count: %lf\n", node->key, node->probability);
	} //print tool

	/* initialize the table */
	cell** C = calloc(TABLE_COLUMNS, sizeof(cell*));
	for (int i = 0; i < TABLE_COLUMNS; i++) {
		C[i] = calloc(TABLE_COLUMNS, sizeof(cell));
	}
	fill_zeroes(C);

	//making table
	for (int k = 2; k <= TABLE_ROWS; k++) { //Iterate over diagonals of the matrix
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			
			min min_struct = minimum_cost(C, i, j); 
			C[i][j].probability = min_struct.minimum + weight(words, i, j);
			C[i][j].root = vector_get(words, min_struct.min_index);

			// printf("C[%d][%d].root: %s\n", i, j, C[i][j].root->key); //useful
			// printf("i: %d, j: %d, k: %d\n", i, j, k);
			// printf("min_root_index: %d\n", min_struct.min_index);
			// printf("here\n");
		}
	}

	printf("average number of comparisons: %lf\n", C[0][600].probability);

	make_tree(C, words);

	printf("END\n");
	return 1;
}
/* void insert_node(node** current, node* new_node) {
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
} */
	
	/* vector test_data;
	vector_init(&test_data);

	node data1;
	data1.key = "A";
	data1.frequency = 1;
	vector_add(&test_data, &data1);
	
	node data2;
	data2.key = "B";
	data2.frequency = 2;
	vector_add(&test_data, &data2);

	node data3;
	data3.key = "C";
	data3.frequency = 4;
	vector_add(&test_data, &data3);

	node data4;
	data4.key = "D";
	data4.frequency = 3;
	vector_add(&test_data, &data4);
	
	cell test_table[TABLE_ROWS][TABLE_COLUMNS];
	fill_zeroes(test_table);

	print_table(test_table);
	print_diagonals(test_table);
	print_weights(test_table, &test_data);
	// print_minimums(test_table);

	print_table(test_table);
	vector_free(&test_data); */
