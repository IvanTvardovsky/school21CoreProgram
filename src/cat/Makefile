CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

utils.o: utils.c
	$(CC) -c $(FLAGS) utils.c
s21_cat.o: s21_cat.c
	$(CC) -c $(FLAGS) s21_cat.c
s21_cat: s21_cat.o utils.o
	$(CC) $(FLAGS) s21_cat.o utils.o -o s21_cat
clean:
	rm -rf *.o
rebuild: clean make