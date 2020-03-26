all: Q1 Q2

Q1: Q1.c Q1.h vector.c vector.h set.c set.h binary_search_tree.c binary_search_tree.h
	gcc -g Q1.c vector.c set.c binary_search_tree.c -o $@

Q2: Q2.c Q2.h vector.c vector.h set.c set.h binary_search_tree.c binary_search_tree.h
	gcc -g Q2.c vector.c set.c binary_search_tree.c -o $@