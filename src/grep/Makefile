CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

utils.o: utils.c
	$(CC) -c $(FLAGS) utils.c
s21_grep.o: s21_grep.c
	$(CC) -c $(FLAGS) s21_grep.c
s21_grep: s21_grep.o utils.o
	$(CC) $(FLAGS) s21_grep.o utils.o -o s21_grep