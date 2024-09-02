#include "s21_decimal.h"

int is_simple_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0, match = 0;
  normalization(&value_1, &value_2);
  res = s21_is_greater(value_1, value_2) ? 0 : 1;
  for (int i = 95; i >= 0; i--) {
    if (get_bit(value_1, i) != get_bit(value_2, i)) {
      match = 1;
      break;
    }
  }
  if (match == 0) {
    res = 0;
  }
  return res;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  normalization(&value_1, &value_2);

  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  if (sign_1 < sign_2) {
    res = 0;
  } else if (sign_1 > sign_2) {
    res = 1;
  } else {
    res = is_simple_less(value_1, value_2) && sign_1 == 0 ? 1 : 0;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2)) {
    res = 1;
  }
  return res;
}

int is_greater_simple(s21_decimal value_1, s21_decimal value_2) {
  int res = 0, flag = 0;
  for (int i = 0; i <= 95; i++) {
    if (get_bit (value_1, i) != get_bit (value_2, i)) {
    if (get_bit (value_1, i) > get_bit (value_2, i))
      res = 1;
    else if (get_bit (value_1, i) < get_bit (value_2, i)) res = 0;
    
    flag = 1;
    break;
    }
  }
  if (!flag) res = 0;
  
  return res;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = 1;
  normalization(&value_1, &value_2);

  if (get_sign(value_1) < get_sign(value_2) && !is_zero(value_1) &&
      !is_zero(value_2))
    res = 1;
  else if (get_sign(value_1) > get_sign(value_2))
    res = 0;
  else if (get_sign(value_1) == get_sign(value_2)) {
    res = is_greater_simple(value_1, value_2);
  }
  return res;
}


int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2)) {
    res = 1;
  } else
    res = 0;
  return res;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 1;
  normalization(&value_1, &value_2);
  if ((get_sign(value_1) == get_sign(value_2))) {
    for (int i = 0; i <= 95; i++) {
      if (get_bit(value_1, i) != get_bit(value_2, i)) {
        res = 0;
      }
    }
  } else {
    res = 0;
  }
  return res;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}