CC = gcc 
CFLAGS = -Wall -Werror -Wextra -g
OBJ = s21_arithmetic.c s21_comparisons.c s21_converts.c s21_other_functions.c

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  	OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif

ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	ADD_LIB = -lcheck
endif


all: clean s21_decimal.a test 


s21_decimal.a:
	@$(CC) $(CFLAGS) -c s21_*.c
	@ar rc s21_decimal.a s21_*.o
	@ranlib s21_decimal.a

test: clean s21_decimal.a
	gcc $(LDFLAGS) -o test test.c s21_decimal.a -lcheck
	./test

gcov_report: clean $(SOURCE)
	gcc --coverage $(SOURCE)  *.c -o test -lcheck
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info test

style:
	@clang-format -style=Google -n *.c *.h tests_unity/*.c tests_unity/*.h
clean:
	@rm -rf *.a
	@rm -rf *.o
	@rm -rf function_files/*.o
	@rm -rf main
	@rm -rf tests
	@rm -rf *.dSYM
	@rm -rf *.out
	@rm -rf gcov_report
	@rm -rf *.info
	@rm -rf *.gcda
	@rm -rf *.gcno

.PHONY:	s21_decimal.a test gcov_report
