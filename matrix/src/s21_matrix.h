#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

enum errors {
  OK,                 // 0
  INCORRECT_MATRIX,   // 1
  CALCULATION_ERROR,  // 2
};

// s21_matrix.c
int s21_create_matrix(int rows, int columns, matrix_t *res);
void s21_remove_matrix(matrix_t *mtrx);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *res);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *res);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// additional_functions.c
int get_minor(matrix_t *A, int rows, int cols, matrix_t *result);
int check_format(matrix_t *A);
void fill_matrix(int n, matrix_t *mtrx);
void InitResult(matrix_t *result);

#endif
