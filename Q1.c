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

	/* set stuff */
	char buffer[1000]; buffer[0] = '\0';
	char *toAdd = NULL;
	simple_set* words = malloc(sizeof(simple_set));
	set_init(words);

	int counter = 0;
	while (!feof (f)) {
		buffer[0] = '\0';
		fscanf (f, "%s", buffer);
		// printf("buffer: %s\n", buffer);
		if (feof(f)) break;
		if (counter  == 0) { // this removes garbage value of first word read in by fscanf
			int length = strlen(buffer);
			for (int i = 3; i < length; i++) {
				buffer[i-3] = buffer[i];
			}
			buffer[length - 3] = '\0';
		}
		counter++;
		set_add(words, buffer);
	}

	uint64_t num_words;
	char** arr = set_to_array(words, &num_words);

	unsigned long text_length;
	rewind(f);
	fseek(f, 0, SEEK_END);
	text_length = ftell(f);
	rewind(f);

    char* text = malloc(sizeof(char) * (text_length + 1));
	if (!text) abort();
	fread(text, sizeof(char), text_length, f);
	text[text_length] = '\0';

	vector* frequency_table = malloc(sizeof(vector));
	vector_init(frequency_table);

	for (int i = 0; i < num_words; i++) {
		char* word = arr[i];
		int frequency = num_occurences(text, text_length, word, strlen(word));
		
		vector_add(frequency_table, node_constructor(word, frequency));
	}

	free(arr);

	return frequency_table;
}

int num_occurences(char* text, int text_length, char* pattern, int pattern_length) {
	int count = 0;
	for (long int i = 0; i < text_length - pattern_length; i++) {
        bool found = brute_force_string_match(pattern, text, i);
        if (found)
            count++;
    }
}

bool brute_force_string_match(const char* p, const char* t, long int i){
    int pattern_length = strlen(p);
    int count = 0;

    for (int j = 0; j < pattern_length; j++) {
        if (p[j] == t[j + i])
            count++;
    }

    return (count == pattern_length);
}

node* node_constructor(char* key, int frequency) {
	node* new_node = malloc(sizeof(node));
	if (!new_node) exit(EXIT_FAILURE);
	
	new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(new_node->key, key);

	new_node->frequency = frequency;
	
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

void print_table(cell** table) {
	printf("______________TABLE______________\n");
	for (int i = 0; i < TABLE_ROWS; i++) {
		for (int j = 0; j < TABLE_COLUMNS; j++) {
			printf ("%d\t", table[i][j].value);
		}
		printf("\n");
	}
}

void print_diagonals(cell** table) {
	printf("____________DIAGONALS____________\n");
	for (int k = 2; k <= TABLE_ROWS; k++) {
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			printf ("%d\t", table[i][j].value);
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
	printf("before fill zeroes\n");
	for (int i = 0; i < 600; i++) {
		for (int j = 0; j < 600; j++) {
			table[i][j].value = 0;
		}
	}
	printf("after fill zeroes\n");
}

int weight(vector* v, int i, int j) {
	int sum = 0;
	for (int s = i; s < j; s++) {
		node* temp = (node*) vector_get(v, s);
		sum += temp->frequency;
	}

	return (sum);
}

int minimum_cost(cell** C, int i, int j) {
	int minimum = 10000;

	for (int k = i + 1; k <= j; k++) {
		// printf("C[i][k-1] = %d, i = %d, k = %d, j = %d\n", C[i][k - 1].value, i, k, j);
		// printf("C[k][j] = %d, i = %d, k = %d, j = %d\n", C[k][j].value, i , k, j);
		int cost = C[i][k - 1].value + C[k][j].value;
		if (cost < minimum) {
			minimum = cost;
		}
	}

	return minimum;
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

	cell** words_table = calloc(601, sizeof(cell*));
	for (int i = 0; i < 601; i++) {
		words_table[i] = calloc(601, sizeof(cell));
	}
	fill_zeroes(words_table);
	printf("before loop\n");
	for (int k = 2; k <= TABLE_ROWS; k++) { //Iterate over diagonals of the matrix
		for (int i = 0; i < TABLE_COLUMNS - k + 1; i++) {
			int j = i + k - 1;
			words_table[i][j].value = minimum_cost(words_table, i, j);  //weight(words, i, j); 
		}
	}
	printf("after loop\n");

	/* printf("num_words: %d\n", input->total);
	for (int i = 0; i < input->total; i++) {
		printf("%s\n", vector_get(input, i));
	}
 */
	for (int i = 0; i < 601; i++) {
		free(words_table[i]);
	}
	free(words_table);

	printf("END\n");
	return 1;
}

	/* for (int i = 0; i < words->total; i++) {
		node* node = vector_get(words, i);
		printf("word: [%s], count: %d\n", node->key, node->frequency);
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
