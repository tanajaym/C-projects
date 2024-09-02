FL = -Wall -Werror -Wextra
all: s21_grep
s21_grep: s21_grep.o 
	gcc $(FL) s21_grep.o -o s21_grep
s21_grep.o: s21_grep.c
	gcc $(FL) -c s21_grep.c
clean: 
	-rm *.o
	-rm s21_grep

rebuild: clean s21_grep
clang: clang-format -i *.c *.h


