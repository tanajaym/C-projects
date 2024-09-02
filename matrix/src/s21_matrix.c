#include "s21_matrix.h"
#define SUCCESS 1
#define FAILURE 0

// 0 - OK
// 1 - INCORRECT_MATRIX
// 2 - CALCULATION_ERROR
int s21_create_matrix(int rows, int columns, matrix_t *res) {
  int flag = OK;
  InitResult(res);

  res->rows = rows;
  res->columns = columns;

  if (check_format(res) != OK) flag = INCORRECT_MATRIX;

  if (flag == OK) {
    res->matrix = (double **)calloc(rows, sizeof(double *));

    for (int i = 0; i < rows; ++i) {
      res->matrix[i] = (double *)calloc(columns, sizeof(double));
    }
  }
  return flag;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  // SUCCESS = 1
  // FAILURE = 0
  int flag = SUCCESS;  // 1

  if (A->rows != B->rows || A->columns != B->columns) flag = FAILURE;  // 0

  if (check_format(A) != OK || check_format(B) != OK) flag = FAILURE;  // 0
  if (flag) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) < 1e-7)  //|A-B|<1e-7
          flag = SUCCESS;                                    // 1
        else
          flag = FAILURE;  // 0
      }
    }
  }
  return flag;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  InitResult(res);

  if (check_format(A) != OK || check_format(B) != OK) flag = INCORRECT_MATRIX;

  if (A->rows != B->rows || A->columns != B->columns) flag = CALCULATION_ERROR;

  if (flag == OK) {
    if (s21_create_matrix(A->rows, A->columns, res) == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          res->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      }
    } else
      flag = INCORRECT_MATRIX;
  }
  return flag;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  InitResult(res);

  if ((check_format(A) == INCORRECT_MATRIX) ||
      (check_format(B) == INCORRECT_MATRIX))
    flag = INCORRECT_MATRIX;

  if (A->rows != B->rows || A->columns != B->columns) flag = CALCULATION_ERROR;

  if (flag == OK) {
    if (s21_create_matrix(A->rows, A->columns, res) == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          res->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
    } else
      flag = INCORRECT_MATRIX;
  }
  return flag;
}

void s21_remove_matrix(matrix_t *mtrx) {
  if (mtrx->matrix) {
    for (int i = 0; i < mtrx->rows; ++i) {
      free(mtrx->matrix[i]);
    }
    free(mtrx->matrix);
  }
  mtrx->rows = 0;
  mtrx->columns = 0;
  mtrx->matrix = NULL;
}

int s21_mult_number(matrix_t *A, double n, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  InitResult(res);

  if (check_format(A)) flag = INCORRECT_MATRIX;

  if (s21_create_matrix(A->rows, A->columns, res) == OK) {
    if (A->rows == res->rows && A->columns == res->columns) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          res->matrix[i][j] = A->matrix[i][j] * n;
        }
      }
    } else
      flag = INCORRECT_MATRIX;
  } else
    flag = CALCULATION_ERROR;
  return flag;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  InitResult(res);

  if (A->rows != B->columns || B->rows != A->columns) {
    flag = CALCULATION_ERROR;
  }
  if (s21_create_matrix(A->rows, B->columns, res) != OK) {
    flag = INCORRECT_MATRIX;
  }

  if (flag == OK) {
    for (int i = 0; i < res->rows; i++) {
      for (int j = 0; j < res->columns; j++) {
        res->matrix[i][j] = 0;
        for (int n = 0; n < A->columns; n++) {
          res->matrix[i][j] += A->matrix[i][n] * B->matrix[n][j];
        }
      }
    }
  }
  return flag;
}

int s21_transpose(matrix_t *A, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  InitResult(res);

  if (s21_create_matrix(A->columns, A->rows, res) != OK) {
    flag = INCORRECT_MATRIX;
  }

  if (flag == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        res->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return flag;
}

int s21_determinant(matrix_t *A, double *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = 0;
  double det = 0;

  if (A->columns != A->rows) flag = CALCULATION_ERROR;

  if (check_format(A) != OK) flag = INCORRECT_MATRIX;

  if (flag == OK) {
    *res = 0.0;

    if (A->rows == 1)
      *res = A->matrix[0][0];
    else if (A->rows == 2)
      *res =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    else {
      for (int i = 0; i < A->columns; i++) {
        matrix_t tmp = {0};
        flag = get_minor(A, 0, i, &tmp);
        flag = s21_determinant(&tmp, &det);
        *res += A->matrix[0][i] * (pow(-1, i) * det);
        s21_remove_matrix(&tmp);
      }
    }
  }
  return flag;
}

int s21_calc_complements(matrix_t *A, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  double det = 0;
  InitResult(res);

  if (check_format(A) != OK)
    flag = INCORRECT_MATRIX;

  else if (A->rows != A->columns)  // only for square
    flag = CALCULATION_ERROR;

  else if (s21_create_matrix(A->rows, A->columns, res) != OK)
    flag = CALCULATION_ERROR;

  else if (A->rows == 1) {
    res->matrix[0][0] = A->matrix[0][0];
    flag = OK;
  }

  else if (flag == OK && A->rows != 1) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t tmp = {0};
        flag = get_minor(A, i, j, &tmp);
        if (!flag) flag = s21_determinant(&tmp, &det);
        if (!flag) res->matrix[i][j] = det * pow(-1, i + j);
        s21_remove_matrix(&tmp);
      }
    }
  }

  return flag;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *res) {
  // 0 - OK
  // 1 - INCORRECT_MATRIX
  // 2 - CALCULATION_ERROR
  int flag = OK;
  double det = 0.0f;
  InitResult(res);

  if (check_format(A) != OK) flag = INCORRECT_MATRIX;

  if (A->rows != A->columns)  // only for square
    flag = CALCULATION_ERROR;

  if (flag == OK) {
    s21_determinant(A, &det);
    if (det == 0) flag = CALCULATION_ERROR;
  }

  if (flag == OK) {
    matrix_t tmp = {0}, tmp_2 = {0};

    flag = s21_calc_complements(A, &tmp);
    if (!flag) flag = s21_transpose(&tmp, &tmp_2);
    if (!flag) flag = s21_mult_number(&tmp_2, 1 / det, res);

    s21_remove_matrix(&tmp);
    s21_remove_matrix(&tmp_2);
  }
  return flag;
}
