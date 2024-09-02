#include <check.h>

#include "s21_matrix.h"

START_TEST(minor_success) {
  matrix_t pes;
  matrix_t kot;
  matrix_t result;
  int rows = 3, columns = 3;
  int eq = 0;

  s21_create_matrix(rows, columns, &kot);
  s21_create_matrix(2, 2, &result);

  kot.matrix[0][0] = 5;
  kot.matrix[0][1] = 7;
  kot.matrix[0][2] = 1;
  kot.matrix[1][0] = -4;
  kot.matrix[1][1] = 1;
  kot.matrix[1][2] = 0;
  kot.matrix[2][0] = 2;
  kot.matrix[2][1] = 0;
  kot.matrix[2][2] = 3;

  result.matrix[0][0] = 5;
  result.matrix[0][1] = 1;
  result.matrix[1][0] = 2;
  result.matrix[1][1] = 3;

  int res = get_minor(&kot, 1, 1, &pes);
  eq = s21_eq_matrix(&result, &pes);
  ck_assert_int_eq(0, res);
  ck_assert_int_eq(SUCCESS, eq);

  s21_remove_matrix(&kot);
  s21_remove_matrix(&pes);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(minor_fail) {
  matrix_t pes;
  matrix_t kot;
  matrix_t result;
  int eq = 0;

  s21_create_matrix(-3, -3, &kot);
  s21_create_matrix(2, 2, &result);

  int res = get_minor(&kot, 1, 1, &pes);
  eq = s21_eq_matrix(&result, &pes);
  ck_assert_int_eq(1, res);
  ck_assert_int_eq(FAILURE, eq);

  s21_remove_matrix(&kot);
  s21_remove_matrix(&pes);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(create_success) {
  matrix_t pes;
  int rows = 5, columns = 5;
  int res = s21_create_matrix(rows, columns, &pes);
  ck_assert_int_eq(0, res);

  s21_remove_matrix(&pes);
}
END_TEST

START_TEST(create_fail) {
  matrix_t pes;
  int rows = -7, columns = -5;
  int res = s21_create_matrix(rows, columns, &pes);
  ck_assert_int_eq(1, res);

  s21_remove_matrix(&pes);
}
END_TEST

START_TEST(equal_success) {
  matrix_t pes, kot;
  int rows = 4, columns = 4;

  s21_create_matrix(rows, columns, &pes);
  s21_create_matrix(rows, columns, &kot);

  fill_matrix(0, &pes);
  fill_matrix(0, &kot);

  int res = s21_eq_matrix(&pes, &kot);
  ck_assert_int_eq(SUCCESS, res);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
}
END_TEST

START_TEST(equal_fail) {
  matrix_t pes, kot;
  int rows = 4, columns = 4;

  s21_create_matrix(rows, columns, &pes);
  s21_create_matrix(rows, columns, &kot);

  fill_matrix(0, &pes);

  for (int i = 0; i < kot.rows; ++i) {
    for (int j = 0; j < kot.columns; ++j) {
      kot.matrix[i][j] = 1;
    }
  }

  int res = s21_eq_matrix(&pes, &kot);
  ck_assert_int_eq(FAILURE, res);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
}
END_TEST

START_TEST(sum_success) {
  matrix_t pes, kot, res;
  int rows = 4, columns = 4;

  s21_create_matrix(rows, columns, &pes);
  s21_create_matrix(rows, columns, &kot);

  for (int i = 0; i < kot.rows; ++i) {
    for (int j = 0; j < kot.columns; ++j) {
      kot.matrix[i][j] = 1;
      pes.matrix[i][j] = 1;
    }
  }

  s21_sum_matrix(&pes, &kot, &res);

  for (int i = 0; i < res.rows; ++i) {
    for (int j = 0; j < res.columns; ++j) {
      ck_assert_int_eq(res.matrix[i][j], 2);
    }
  }

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(sum_fail) {
  matrix_t pes, kot, res;
  int rows = 4, columns = 4;

  s21_create_matrix(rows, columns, &pes);
  s21_create_matrix(rows, columns, &kot);

  for (int i = 0; i < kot.rows; ++i) {
    for (int j = 0; j < kot.columns; ++j) {
      kot.matrix[i][j] = -1;
      pes.matrix[i][j] = -2;
    }
  }

  s21_sum_matrix(&pes, &kot, &res);

  for (int i = 0; i < res.rows; ++i) {
    for (int j = 0; j < res.columns; ++j) {
      ck_assert_int_eq(res.matrix[i][j], -3);
    }
  }

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(sub_success) {
  matrix_t pes, kot, res;
  int rows = 4, columns = 4;

  s21_create_matrix(rows, columns, &pes);
  s21_create_matrix(rows, columns, &kot);

  for (int i = 0; i < kot.rows; ++i) {
    for (int j = 0; j < kot.columns; ++j) {
      kot.matrix[i][j] = 3;
      pes.matrix[i][j] = 2;
    }
  }

  s21_sub_matrix(&kot, &pes, &res);

  for (int i = 0; i < res.rows; ++i) {
    for (int j = 0; j < res.columns; ++j) {
      ck_assert_int_eq(res.matrix[i][j], 1);
    }
  }

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(sub_fail) {
  int num = 1, result = 0;
  matrix_t pes, kot, res;

  s21_create_matrix(2, 1, &pes);
  s21_create_matrix(3, 3, &kot);

  for (int i = 0; i < pes.rows; i++) {
    for (int j = 0; j < pes.columns; j++) {
      pes.matrix[i][j] = num++;
    }
  }

  s21_sub_matrix(&kot, &pes, &res);

  result = s21_sub_matrix(&kot, &pes, &res);
  ck_assert_int_eq(2, result);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(mult_success) {
  matrix_t pes, kot, res, true_res;

  s21_create_matrix(3, 2, &pes);
  s21_create_matrix(2, 3, &kot);
  s21_create_matrix(3, 3, &true_res);

  pes.matrix[0][0] = 1;
  pes.matrix[0][1] = 4;
  pes.matrix[1][0] = 2;
  pes.matrix[1][1] = 5;
  pes.matrix[2][0] = 3;
  pes.matrix[2][1] = 6;

  kot.matrix[0][0] = 1;
  kot.matrix[0][1] = -1;
  kot.matrix[0][2] = 1;
  kot.matrix[1][0] = 2;
  kot.matrix[1][1] = 3;
  kot.matrix[1][2] = 4;

  true_res.matrix[0][0] = 9;
  true_res.matrix[0][1] = 11;
  true_res.matrix[0][2] = 17;
  true_res.matrix[1][0] = 12;
  true_res.matrix[1][1] = 13;
  true_res.matrix[1][2] = 22;
  true_res.matrix[2][0] = 15;
  true_res.matrix[2][1] = 15;
  true_res.matrix[2][2] = 27;

  s21_mult_matrix(&pes, &kot, &res);

  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.columns; j++) {
      ck_assert_int_eq(res.matrix[i][j], true_res.matrix[i][j]);
    }
  }
  ck_assert_int_eq(SUCCESS, s21_eq_matrix(&true_res, &res));

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
  s21_remove_matrix(&true_res);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(mult_fail) {
  matrix_t pes, kot, res;

  s21_create_matrix(4, 0, &pes);
  s21_create_matrix(0, 4, &kot);

  fill_matrix(1, &pes);
  s21_mult_matrix(&kot, &pes, &res);

  int result = s21_mult_matrix(&kot, &pes, &res);
  ck_assert_int_eq(1, result);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(transpose_success) {
  matrix_t pes, res;

  s21_create_matrix(3, 2, &pes);

  pes.matrix[0][0] = 1;
  pes.matrix[0][1] = 4;
  pes.matrix[1][0] = 2;
  pes.matrix[1][1] = 5;
  pes.matrix[2][0] = 3;
  pes.matrix[2][1] = 6;

  s21_transpose(&pes, &res);
  int val = 1;
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.columns; j++) {
      ck_assert_int_eq(res.matrix[i][j], val++);
    }
  }

  s21_remove_matrix(&pes);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(transpose_fail) {
  matrix_t pes, res;

  s21_create_matrix(0, 0, &pes);
  int result = s21_transpose(&pes, &res);
  ck_assert_int_eq(1, result);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(determinant_diff) {
  double res = 0;
  matrix_t pes;

  s21_create_matrix(3, 3, &pes);
  pes.matrix[0][0] = 1;
  pes.matrix[0][1] = 4;
  pes.matrix[0][2] = 5;

  pes.matrix[1][0] = 6;
  pes.matrix[1][1] = 8;
  pes.matrix[1][2] = 9;

  pes.matrix[2][0] = 2;
  pes.matrix[2][1] = 3;
  pes.matrix[2][2] = 1;

  s21_determinant(&pes, &res);
  ck_assert_int_eq(39, res);

  s21_remove_matrix(&pes);
}
END_TEST

START_TEST(determinant_1_1) {
  int num = 1;
  double res = 0;
  matrix_t pes;

  s21_create_matrix(1, 1, &pes);

  for (int i = 0; i < pes.rows; i++) {
    for (int j = 0; j < pes.columns; j++) {
      pes.matrix[i][j] = num++;
    }
  }

  s21_determinant(&pes, &res);
  ck_assert_int_eq(1, res);

  s21_remove_matrix(&pes);
}
END_TEST

START_TEST(determinant_3_3) {
  int num = 1;
  double res = 0;
  matrix_t pes;

  s21_create_matrix(3, 3, &pes);

  for (int i = 0; i < pes.rows; i++) {
    for (int j = 0; j < pes.columns; j++) {
      pes.matrix[i][j] = num++;
    }
  }

  s21_determinant(&pes, &res);
  ck_assert_int_eq(0, res);

  s21_remove_matrix(&pes);
}
END_TEST

START_TEST(determinant_value) {
  int num = 1;
  double res = 0;
  matrix_t pes;

  s21_create_matrix(5, 4, &pes);

  for (int i = 0; i < pes.rows; i++) {
    for (int j = 0; j < pes.columns; j++) {
      pes.matrix[i][j] = num++;
    }
  }

  int result = s21_determinant(&pes, &res);
  ck_assert_int_eq(result, 2);

  s21_remove_matrix(&pes);
}
END_TEST

START_TEST(calc_comp_success) {
  int result = 0;
  matrix_t pes, res, kot;

  s21_create_matrix(3, 3, &pes);
  s21_create_matrix(3, 3, &kot);

  pes.matrix[0][0] = 1;
  pes.matrix[0][1] = 2;
  pes.matrix[0][2] = 3;
  pes.matrix[1][0] = 0;
  pes.matrix[1][1] = 4;
  pes.matrix[1][2] = 2;
  pes.matrix[2][0] = 5;
  pes.matrix[2][1] = 2;
  pes.matrix[2][2] = 1;

  kot.matrix[0][0] = 0;
  kot.matrix[0][1] = 10;
  kot.matrix[0][2] = -20;
  kot.matrix[1][0] = 4;
  kot.matrix[1][1] = -14;
  kot.matrix[1][2] = 8;
  kot.matrix[2][0] = -8;
  kot.matrix[2][1] = -2;
  kot.matrix[2][2] = 4;

  result = s21_calc_complements(&pes, &res);
  ck_assert_int_eq(0, result);
  ck_assert_int_eq(SUCCESS, s21_eq_matrix(&kot, &res));

  s21_remove_matrix(&pes);
  s21_remove_matrix(&res);
  s21_remove_matrix(&kot);
}
END_TEST

START_TEST(calc_comp_fail) {
  int res = 0;
  matrix_t pes, kot;

  s21_create_matrix(2, 0, &pes);
  s21_create_matrix(0, 3, &kot);

  res = s21_calc_complements(&pes, &kot);
  ck_assert_int_eq(1, res);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&kot);
}
END_TEST

START_TEST(s21_inverse_matrix_success) {
  int result = 0;
  matrix_t pes, res, kot;

  s21_create_matrix(3, 3, &pes);
  s21_create_matrix(3, 3, &kot);

  kot.matrix[0][0] = 1;
  kot.matrix[0][1] = -1;
  kot.matrix[0][2] = 1;
  kot.matrix[1][0] = -38;
  kot.matrix[1][1] = 41;
  kot.matrix[1][2] = -34;
  kot.matrix[2][0] = 27;
  kot.matrix[2][1] = -29;
  kot.matrix[2][2] = 24;

  pes.matrix[0][0] = 2;
  pes.matrix[0][1] = 5;
  pes.matrix[0][2] = 7;
  pes.matrix[1][0] = 6;
  pes.matrix[1][1] = 3;
  pes.matrix[1][2] = 4;
  pes.matrix[2][0] = 5;
  pes.matrix[2][1] = -2;
  pes.matrix[2][2] = -3;

  result = s21_inverse_matrix(&kot, &res);
  result = s21_eq_matrix(&pes, &res);
  ck_assert_int_eq(SUCCESS, result);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&res);
  s21_remove_matrix(&kot);
}
END_TEST

START_TEST(s21_inverse_matrix_fail) {
  int result = 0;
  matrix_t pes, res;

  s21_create_matrix(5, 9, &pes);

  result = s21_inverse_matrix(&pes, &res);
  ck_assert_int_eq(2, result);

  s21_remove_matrix(&pes);
  s21_remove_matrix(&res);
}
END_TEST

int main() {
  Suite *s1 = suite_create("s21_matrix");
  TCase *tc1_1 = tcase_create("s21_matrix");
  SRunner *sr = srunner_create(s1);
  int nf = 0;
  suite_add_tcase(s1, tc1_1);

  tcase_add_test(tc1_1, minor_success);
  tcase_add_test(tc1_1, minor_fail);
  tcase_add_test(tc1_1, create_success);
  tcase_add_test(tc1_1, create_fail);
  tcase_add_test(tc1_1, equal_success);
  tcase_add_test(tc1_1, equal_fail);
  tcase_add_test(tc1_1, sum_success);
  tcase_add_test(tc1_1, sum_fail);
  tcase_add_test(tc1_1, sub_success);
  tcase_add_test(tc1_1, sub_fail);
  tcase_add_test(tc1_1, mult_success);
  tcase_add_test(tc1_1, mult_fail);
  tcase_add_test(tc1_1, transpose_success);
  tcase_add_test(tc1_1, transpose_fail);
  tcase_add_test(tc1_1, determinant_diff);
  tcase_add_test(tc1_1, determinant_1_1);
  tcase_add_test(tc1_1, determinant_3_3);
  tcase_add_test(tc1_1, determinant_value);
  tcase_add_test(tc1_1, calc_comp_success);
  tcase_add_test(tc1_1, calc_comp_fail);
  tcase_add_test(tc1_1, s21_inverse_matrix_success);
  tcase_add_test(tc1_1, s21_inverse_matrix_fail);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}