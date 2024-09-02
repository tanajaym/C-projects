#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  *result = value;
  if (!get_sign(value))
    set_sign(result, 1);
  else {
    set_sign(result, 0);
  }
  return err;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  int scale = get_scale(value);
  for (; scale > 0; scale--) {
    div_on_10(&value);
  }
  set_scale(&value, 0);
  *result = value;
  return err;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int err = 0, temp_div = 0;
  int sign = get_sign(value);
  int scale = get_scale(value);

  set_sign(&value, 0);
  s21_truncate(value, result);
  for (int i = 0; i < scale; i++) {
    temp_div = div_on_10(&value);
  }

  if (temp_div >= 5) {
    s21_decimal plus_one = {{1, 0, 0, 0}};
    err = s21_add(*result, plus_one, result);
  }
  if (sign == 1) set_sign(result, 1);

  return err;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int err = 0;
  int sign = get_sign(value);
  int scale = get_scale(value);

  int original_sign = sign;

  if (sign && scale) {
    s21_truncate(value, result);
    set_sign(&value, 0);

    s21_decimal plus_one = {{1, 0, 0, 0}};
    err = s21_add(*result, plus_one, result);
    set_sign(result, 1);
  }

  err = s21_truncate(value, result);

  if (original_sign) {
    set_sign(result, 1);
  } else {
    set_sign(result, 0);
  }

  return err;
}
