#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
		double probability = ((double) num_occurences(all_words, word)) / num_words;
		
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

node* node_constructor(char* key, double probability) {
	node* new_node = malloc(sizeof(node));
	
	new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(new_node->key, key);

	new_node->probability = probability;
	
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;

	return new_node;
}

int compare_keys(const void* a, const void* b) {
	node* first = *(node**)a;
	node* second = *(node**)b;
	return (strcmp((char*)(*first).key, (char*)(*second).key));
}

int find_largest_probability(vector* words, int left_bound, int right_bound) {
	if (left_bound < 0 || left_bound >= right_bound || right_bound > words->total) exit(EXIT_FAILURE);

	int max_index = left_bound;
	for (int i = left_bound + 1; i < right_bound; i++) {
		if (((node*)vector_get(words, i))->probability > ((node*)vector_get(words, max_index))->probability) {
			max_index = i;
		}
	}

	return max_index;
}

void make_tree(vector* words, int left_bound, int right_bound, node** parent_child) {
	if (left_bound < 0 || right_bound < 0 || right_bound > words->total) return;

	printf("recursion\n");

	int highest_probability_index = find_largest_probability(words, left_bound, right_bound);
	node* word = vector_get(words, highest_probability_index);

	(*parent_child) = word;

	make_tree(words, left_bound, highest_probability_index - 1, &(word->children[0]));
	make_tree(words, highest_probability_index, right_bound, &(word->children[1]));
}

int main() {
	vector* words = read_file("data_7.txt");
	qsort(words->items, words->total, sizeof(node*), compare_keys);

	/* making tree */
	int highest_probability_index = find_largest_probability(words, 0, words->total);
	node* root = vector_get(words, highest_probability_index);
	
	printf("word: [%s], at index [%d]\n", root->key, highest_probability_index);
	make_tree(words, 0, highest_probability_index - 1, &(root->children[0]));
	make_tree(words, highest_probability_index, words->total, &(root->children[1]));

	printf("DONE\n");
	return 0;
}