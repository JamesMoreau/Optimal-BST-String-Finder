#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include <stdbool.h>
#include "set.h"
#include "horspool.h"
#include "Q1.h"

vector* read_file(char* filename) {
	FILE* f = fopen(filename, "r");
	if (!f) exit(EXIT_FAILURE);

	/* set stuff */
	char buffer[1000]; buffer[0] = '\0';
	char *toAdd = NULL;
	simple_set* words = malloc(sizeof(simple_set));
	set_init(words);

	while (!feof (f)) {
		buffer[0] = '\0';
		fscanf (f, "%s", buffer);
		set_add(words, buffer);
	}

	uint64_t num_words;
	char** arr = set_to_array(words, &num_words);

	/* word counts with horspool */
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

	printf("num_words: %d\n", num_words);
	for (int i = 0; i < num_words; i++) {
		char* word = arr[i];
		int frequency = horspool(text, word);
		
		vector_add(frequency_table, node_constructor(word, frequency));
	}

	free(arr);

	return frequency_table;
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

void print_table(int** arr) {
	printf("_____TABLE_____\n");
	for (int i = 0; i < 601; i++) {
		for (int j = 0; j < 601; j++) {
			printf ("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

void fill_zeroes(cell** table) {
	for (int i = 0; i < 601; i++) {
		table[i][i].value = 0;
	}
}

int main() {
	vector* test_data = malloc(sizeof(vector));
	vector_init(test_data);

	node data1;
	data1.key = "A";
	data1.frequency = 1;


	cell test_table[601][601];

	return 1;
}
/* 	vector* words = read_file("data_7.txt");
	for (int i = 0; i < words->total; i++) {
		node* node = vector_get(words, i);
		printf("word: [%s], count: %d\n", node->key, node->frequency);
	}*/


	/* printf("num_words: %d\n", input->total);
	for (int i = 0; i < input->total; i++) {
		// printf("%s\n", vector_get(input, i));
	} */
