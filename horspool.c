#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void horspool_shift_table(char* p, int table[]) {
	int pattern_length = strlen(p);

	for (int i = 0; i < 500; i++) {
		table[i] = pattern_length;
	}

	for (int j = 0; j < pattern_length - 1; j++) {
		table[p[j]] = pattern_length - j - 1;
	}
}

int horspool(char* text, char* pattern) {
    int table[500] = {0};
    int pattern_length = strlen(pattern);
    int text_length = strlen(text);
	horspool_shift_table(pattern, table);

	int found_count = 0;
	int shifts = 0;
	long int i = 0;
	while (i < text_length - pattern_length) {
		int new_i = text[i + pattern_length - 1];
		if (new_i < 0) {
			i++;
			new_i = text[i + pattern_length - 1];
		}

		if (
			pattern[pattern_length - 1] == new_i && 
			memcmp(pattern, text + i, pattern_length - 1) == 0
			) found_count++;

		shifts++;
		i += table[new_i];
	}

    return found_count;
}