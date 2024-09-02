CC = gcc 
FL = -Wall -Werror -Wextra -g
OBJ = s21_*.c 
TEST_EXECUTABLE:=test.out
MANUAL_TEST_EXECUTABLE:=main_test.out

all: test
	
s21_matrix.a: 
	 $(CC) $(FL) -c $(OBJ)
	 ar rc s21_matrix.a s21_*.o
	 ranlib s21_matrix.a

test: clean s21_matrix.a
	$(CC) $(FL) -o ${TEST_EXECUTABLE} tests.c s21_matrix.a -lcheck
	./${TEST_EXECUTABLE}

test_valg: clean s21_matrix.a
	$(CC) -g -o ${TEST_EXECUTABLE} tests.c s21_matrix.a -lcheck
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --log-file=valgrind.log \
         ./${TEST_EXECUTABLE} 
test_valg_manual: clean s21_matrix.a
	$(CC) -g -o ${MANUAL_TEST_EXECUTABLE} main_test.c s21_matrix.a
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --log-file=valgrind.log \
         ./${MANUAL_TEST_EXECUTABLE} 

gcov_report: clean $(OBJ)  
	$(CC) --coverage $(OBJ) tests.c -o ${TEST_EXECUTABLE} -lcheck 
	./${TEST_EXECUTABLE}
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info test

clang: 
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h 
	clang-format -i *.c *.h
	rm -rf .clang-format

clean: 
	rm -rf *.a 
	rm -rf *.o 
	rm -rf *.gcno *.gcda *.info
	rm -rf *.out
	rm -rf gcov_report
	rm -rf *.h.gch
	rm -rf *.dSYM
	rm -rf report
	rm -rf .clang-format
