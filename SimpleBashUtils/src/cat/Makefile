FL = -Wall -Werror -Wextra
all: s21_cat
s21_cat: s21_cat.o 
	gcc $(FL) s21_cat.o -o s21_cat
s21_cat.o: s21_cat.c
	gcc $(FL) -c s21_cat.c
clean: 
	-rm *.o
	-rm s21_cat

rebuild: clean s21_cat
clang: clang-format -i *.c *.h


