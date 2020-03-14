all: Q1

Q1: Q1.c Q1.h vector.c vector.h
	gcc -g Q1.c vector.c -o $@