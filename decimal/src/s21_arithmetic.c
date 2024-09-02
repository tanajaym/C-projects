#include "s21_decimal.h"

int add_simple(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  unsigned memory = 0;
  unsigned sum = 0;
  for (int i = 0; i <= 95; ++i) {
    int bit_one = get_bit(value_1, i);
    int bit_two = get_bit(value_2, i);
    if (!bit_one && !bit_two) {
      set_bit(result, i, 0);
    }
    if (!bit_one && bit_two) {
      set_bit(result, i, 1);
    }
    if (bit_one && !bit_two) {
      set_bit(result, i, 1);
    }
    if (bit_one && bit_two) {
      set_bit(result, i, 0);
    } else {
      set_bit(result, i, 1);
    }
    sum = bit_one + bit_two + memory;
    memory = sum / 2;
    sum = sum % 2;

    set_bit(result, i, sum);
  }
  return sum;
}

int sub_simple(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  set_zero(result);
  int index = 0, scale = get_scale(value_1);
  for (; index <= 95; ++index) {
    int count_1 = get_bit(value_1, index);
    int count_2 = get_bit(value_2, index);
    if (count_1 && count_2) {
      set_bit(result, index, 0);
    }
    if (count_1 == 0 && count_2) {
      int i = index, count = 0;
      for (; i <= 95 && !count; i++) {
        if (get_bit(value_1, i)) {
          count = 1;
          set_bit(result, index, 1);
          set_bit(&value_1, i, 0);
          for (int j = i - 1; j > index; j--) {
            set_bit(&value_1, j, 1);
          }
        }
      }
      set_bit(result, index, 1);
    }
    if (count_1 && count_2 == 0) {
      set_bit(result, index, 1);
    }
    if (count_1 == 0 && count_2 == 0) {
      set_bit(result, index, 0);
    }
  }
  set_scale(result, scale);
  return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign_1 = get_sign(value_1), sign_2 = get_sign(value_2);
  if ((value_1.bits[0] > 4294967294) || (value_2.bits[0] > 4294967294)) err = 1;
  if (err == 1 && (sign_1 || sign_2)) err = 2;

  set_zero(result);
  normalization(&value_1, &value_2);
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  if (sign_1 == 1 && sign_2 == 1) {
    add_simple(value_1, value_2, result);
    set_sign(result, 1);
  } else if (sign_1 == 0 && sign_2 == 1) {
    if (s21_is_less(value_1, value_2)) {
      s21_sub(value_2, value_1, result);
      if (!is_zero(*result)) {
        set_sign(result, 1);
      }
    } else {
      s21_sub(value_1, value_2, result);
    }
  } else if (sign_1 == 1 && sign_2 == 0) {
    if (s21_is_less(value_1, value_2)) {
      s21_sub(value_2, value_1, result);
    } else {
      s21_sub(value_1, value_2, result);
      if (!is_zero(*result)) {
        set_sign(result, 1);
      }
    }
  } else if (sign_1 == 0 && sign_2 == 0) {
    add_simple(value_1, value_2, result);
  }
  set_scale(result, get_scale(value_1));
  return err;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  if ((value_1.bits[0] > 4294967294) || (value_2.bits[0] > 4294967294)) err = 1;
  if (err == 1 && (sign_1 || sign_2)) err = 2;

  set_zero(result);
  normalization(&value_1, &value_2);
  set_sign(&value_1, 0);
  set_sign(&value_2, 0);
  if (sign_1 && sign_2) {
    if (s21_is_less(value_1, value_2)) {
      sub_simple(value_2, value_1, result);
    } else {
      sub_simple(value_1, value_2, result);
      set_sign(result, 1);
    }
  } else if (sign_1 == 0 && sign_2 == 1) {
    add_simple(value_1, value_2, result);
  } else if (sign_1 == 1 && sign_2 == 0) {
    add_simple(value_1, value_2, result);
    set_sign(result, 1);
  } else if (sign_1 == 0 && sign_2 == 0) {
    if (s21_is_less(value_1, value_2)) {
      sub_simple(value_1, value_2, result);
      set_sign(result, 1);
    } else {
      sub_simple(value_1, value_2, result);
    }
  }
  set_scale(result, get_scale(value_1));

  return err;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  if ((value_1.bits[0] > 4294967294) || (value_2.bits[0] > 4294967294)) err = 1;
  if (err == 1 && (sign_1 || sign_2)) err = 2;

  set_zero(result);

  for (int i = 0; i < 96; i++) {
    if (get_bit(value_1, i) == 1) {
      add_simple(value_2, *result, result);
    }
    left_shift_pro(&value_2, 1);
  }

  if (get_bit(*result, 96)) err = 1;
  if (sign_1 == sign_2) set_sign(result, 0);
  if (sign_1 != sign_2) set_sign(result, 1);
  return err;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign_1 = get_sign(value_1), sign_2 = get_sign(value_2);
  if ((value_1.bits[0] > 4294967294) || (value_2.bits[0] > 4294967294)) err = 1;
  if (err == 1 && (sign_1 || sign_2)) err = 2;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0)
    err = 3;

  normalization(&value_1, &value_2);
  set_zero(result);
  s21_decimal memory = value_2;
  unsigned delimoe = 96, delitel = 96;
  while (get_bit(value_1, delimoe) != 1) {
    delimoe--;
  }

  while (get_bit(value_2, delitel) != 1) {
    delitel--;
  }

  while (delimoe >= delitel) {
    left_shift_pro(&value_2, delimoe - delitel);
    if (s21_is_greater_or_equal(value_1, value_2)) {
      s21_sub(value_1, value_2, &value_1);
      set_bit(result, delimoe - delitel, 1);
    } else {
      set_bit(result, delimoe - delitel, 0);
    }
    value_2 = memory;
    delimoe--;
  }
  return err;
}
