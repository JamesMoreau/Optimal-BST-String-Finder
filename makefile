all: Q1

Q1: Q1.c Q1.h vector.c vector.h set.c set.h
	gcc -g Q1.c vector.c set.c -o $@