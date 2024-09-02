#include "s21_matrix.h"

int check_format(matrix_t* A) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  int flag = OK;
  if (A == NULL) flag = INCORRECT_MATRIX;
  if (A->rows <= 0 || A->columns <= 0) flag = INCORRECT_MATRIX;

  return flag;
}

void InitResult(matrix_t* result) {
  result->rows = 0;
  result->columns = 0;
  result->matrix = NULL;
}

void fill_matrix(int n, matrix_t* mtrx) {
  for (int i = 0; i < mtrx->rows; i++) {
    for (int j = 0; j < mtrx->columns; j++) {
      mtrx->matrix[i][j] = n;
    }
  }
}

int get_minor(matrix_t* A, int rows, int cols, matrix_t* res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;

  if (s21_create_matrix(A->rows - 1, A->columns - 1, res) != OK)
    flag = INCORRECT_MATRIX;

  if (flag == OK) {
    int r = 0, c = 0;
    for (int i = 0; i < A->rows; i++) {
      if (i == rows) continue;
      c = 0;
      for (int j = 0; j < A->columns; j++) {
        if (j == cols) continue;
        res->matrix[r][c] = A->matrix[i][j];
        c++;
      }
      r++;
    }
  }
  return flag;
}
