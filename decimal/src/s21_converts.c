#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 0;
  set_zero(dst);
  if (src < 0) {
    dst->bits[3] |= (1 << 31);
    src = -src;
  }
  if (src < 0) {
    set_sign(dst, 1);
    src = src * (-1);
  }
  if (src > 2147483647) res = 1;
  dst->bits[0] = src;
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_truncate(src, &src);
  int sign = get_bit(src, 127);
  int flags = 0;
  if (src.bits[1] || src.bits[2]) flags = 1;
  if (src.bits[2] == 0 && src.bits[1] == 0 && dst) {
    *dst = src.bits[0];
  }
  if (sign) {
    *dst *= -1;
  }
  return flags;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int flags = 0;

  if (src.bits[1] || src.bits[2]) flags = 1;
  if ((dst == NULL) || (get_scale(src) > 28)) {
    *dst = 0;
    flags = 0;
  } else {
    int sign = get_sign(src);
    int exp = get_scale(src);
    double res = 0;

    for (int bit = 0; bit < 32 * 3; ++bit) {
      res += pow(2, bit) * (src.bits[0] % 2);
      move_decimal_right(&src);
    }

    res *= sign == 1 ? -1 : 1;
    res /= pow(10, exp);

    *dst = (float)res;
  }

  return flags;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  set_zero(dst);
  int result = 0;
  if (fabs(src) < powl(10.0, -1 * 28)) {
    result = 1;
  } else if (src >= powl(2.0, 96)) {
    result = 1;
  } else if (src < -1 * powl(2.0, 96)) {
    result = 1;
  } else {
    if (src < 0.0) {
      set_sign(dst, 1);
      src *= -1;
    }
  }

  char str[14] = {0};
  sprintf(str, "%.7g", src);
  int i = round_string(str);
  int int_src = 0;
  sscanf(str, "%d", &int_src);

  dst->bits[0] = int_src;
  if (i != -1) set_scale(dst, i);

  return result;
}
