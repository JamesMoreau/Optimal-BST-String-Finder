#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "binary_search_tree.h"
#include "Q2.h"
#include "vector.h"
#include "set.h"

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

	return probability_table;
}

int num_occurences(vector* all_words, char* word) {
	int count = 0;
	for (long int i = 0; i < all_words->total; i++) {
		char* word_to_compare = vector_get(all_words, i);
        if (strcmp(word, word_to_compare) == 0){
            count++;
		}
    }

	return (count);
}

int compare_probabilities(const void* a, const void* b) {
	node* first = *(node**)a;
	node* second = *(node**)b;

	if (first->probability > second->probability) {
		return -1;
	} else if (first->probability < second->probability) {
		return 1;
	} else {
		return 0;
	}
}

int main() {
	vector* words = read_file("data_7.txt");
	qsort(words->items, words->total, sizeof(node*), compare_probabilities);

	node* root = NULL;

	node* to_add;
	for (int i = 0; i < words->total; i++) {
		to_add = vector_get(words, i);
		// printf("key: [%s]\n", to_add->key);
		add_node(to_add, &root);
	}

	char user_input[1000]; user_input[0] = '\0';

	printf("Enter a key: ");
	scanf("%s", user_input);

	node* to_find = find_node(root, user_input);

	if (!to_find) {
		printf("Not found.\n");
	}

	delete_tree(root);
	return 0;
}