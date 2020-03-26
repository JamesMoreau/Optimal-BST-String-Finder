#include "binary_search_tree.h"
#include <stdbool.h>

node* node_constructor(char* key, double probability) {
	node* new_node = malloc(sizeof(node));
	
	new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(new_node->key, key);

	new_node->probability = probability;
	
	new_node->children[0] = NULL;
	new_node->children[1] = NULL;

	return new_node;
}

void add_node(node* to_add, node** root) {
    if (!to_add) return;

    node* current = *root;
    node* parent = *root;

    if (!(*root)) { // tree is empty
        *root = to_add;
        return;
    }

    while (true) {
        parent = current;
        if (strcmp(current->key, to_add->key) < 0) {
            current = current->children[1];
            if (!current) {
                parent->children[1] = to_add;
                return;
            }
        } else {
            current = current->children[0];
            if (!current) {
                parent->children[0] = to_add;
                return;
            }
        }
    }
}

void delete_tree(node* root) {
    if (!root) return;

    delete_tree(root->children[0]);
    delete_tree(root->children[1]);

    if (!root->key) {
        free(root->key);
    }

    free(root);
}

void print_tree(node* root) {
    if (!root) return;

    print_tree(root->children[0]);
    printf("key: [%s], probability: [%lf]\n", root->key, root->probability);
    print_tree(root->children[1]);
}

node* find_node(node* root, char* key) {
    if (!root || !key) return (NULL);

    node* current = root;
    node* parent = root;

        while (true) {
        parent = current;

        printf("Compared with %s (%.4lf), ", parent->key, parent->probability);
        if (strcmp(parent->key, key) < 0) {
            printf("go right subtree.\n");
            current = current->children[1];
            
            if (!current) return (NULL);

        } else if (strcmp(parent->key, key) > 0) {
            printf("go left subtree.\n");
            current = current->children[0];

            if (!current) return (NULL);

        } else { // key is equal!
            printf("found.\n");
            return (current);
        }
    }
}

node* find_node_optimal(node* root, char* key) {
    if (!root || !key) return (NULL);

    node* current = root;
    node* parent = root;

        while (true) {
        parent = current;

        printf("Compared with %s (%.2lf), ", parent->key, parent->probability);
        if (strcmp(parent->key, key) < 0) {
            printf("go right subtree.\n");
            current = current->children[1];
            
            if (!current) return (NULL);

        } else if (strcmp(parent->key, key) > 0) {
            printf("go left subtree.\n");
            current = current->children[0];

            if (!current) return (NULL);

        } else { // key is equal!
            printf("found.\n");
            return (current);
        }
    }
}
