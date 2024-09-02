#include "s21_decimal.h"

// int main (){
// s21_decimal num = {{2147483647,
//                     0b00000000000000000000000000000000,
//                     0b00000000000000000000000000000000,
//                     0b00000000000000000000000000000000}};

// // s21_decimal num2 = {{5,
// //                      0b00000000000000000000000000000000,
// //                      0b00000000000000000000000000000000,
// //                      0b10000000000000000000000000000000}};

// // s21_decimal num3;
// // int rez3 = s21_is_less(num, num2);
// // if (rez3) printf("less\n");
// // else printf("ne less\n");

//   // s21_mul (num, num2, &num3);

// multi_10_scale_help(&num);
// multi_10_scale_help(&num);
// multi_10_scale_help(&num);

//   // print_dec(num);
//   // print_dec(num2);
  


// return 0;
// }

// void print_dec(s21_decimal num) {
//   for (int i = 127; i >= 0; --i) {
//     printf("%d", get_bit(num, i));
//     if (i == 32 || i == 64 || i == 96) printf(" ");
//   }
//   printf("\n");
// }


int get_bit(s21_decimal num, int bit) {
  int index = bit % 32;
  int q = bit / 32;
  int mask = 1u << index;
  return (num.bits[q] & mask) >> index;
}

void set_bit(s21_decimal *num, int index, int bit) {
  int position = index % 32;
  int num_i = index / 32;
  int mask = 1u << position;
  if (bit == 1)
    num->bits[num_i] |= mask;
  else
    num->bits[num_i] &= (~mask);
}

int get_sign(s21_decimal num) { return get_bit(num, 127); }

void set_sign(s21_decimal *num, int sign) {
  if (sign == 1) {
    set_bit(num, 127, 1);
  } else if (sign == 0) {
    set_bit(num, 127, 0);
  }
}

int get_scale(s21_decimal num) { return (num.bits[3] & SCALE) >> 16; }

int set_scale(s21_decimal *num, int val) {
  num->bits[3] &= MINUS;
  return num->bits[3] |= val << 16;
}

int is_zero(s21_decimal num) {
  int res = 0;
  for (int i = 0; i < 3; i++) {
    if (num.bits[i] == 0) res = 1;
  }
  return res;
}

void set_zero(s21_decimal *num) {
  for (int i = 0; i < 4; i++) {
    num->bits[i] = 0;
  }
}

void left_shift_pro(s21_decimal *dec, int shift) {
  for (int i = 0; i < shift; i++) {
    left_shift(dec);
  }
}

void left_shift(s21_decimal *dec) {
  int end_zero_mantiss = get_bit(*dec, 32 - 1);
  int end_first_mantiss = get_bit(*dec, 64 - 1);
  for (int i = 0; i < 3; ++i) {
    dec->bits[i] <<= 1;
  }
  set_bit(dec, 32, end_zero_mantiss);
  set_bit(dec, 64, end_first_mantiss);
}

void multi_10_scale_help(s21_decimal *value) {
  s21_decimal multi = {{10, 0, 0, 0}};
  s21_decimal res_cpy = {{0, 0, 0, 0}};
  s21_mul (*value, multi, &res_cpy);
  *value = res_cpy;
  int current_scale = get_scale(res_cpy) + 1;
  set_scale(value, current_scale);
}

int div_on_10(s21_decimal *result) {
  int rem = 0;
  for (int i = 95; i >= 0; --i) {
    int bit = get_bit(*result, i);
    int res = (bit + rem * 2) / 10;
    rem = (bit + rem * 2) % 10;
    set_bit(result, i, res);
  }
  return rem;
}

void normalization(s21_decimal *value_1, s21_decimal *value_2) {
  int scale_1 = get_scale(*value_1), scale_2 = get_scale(*value_2);
  while (scale_1 != scale_2) {
    int delta = abs(scale_1 - scale_2);
    if (scale_1 < scale_2) {
      if ((delta + scale_1) < 29) {
        multi_10_scale_help(value_1);
        scale_1 = get_scale(*value_1);
      } else {
        div_on_10(value_1);
        scale_1 = get_scale(*value_1);
      }
    } else if (scale_1 > scale_2) {
      if ((scale_2 + delta) < 29) {
        multi_10_scale_help(value_2);
        scale_2 = get_scale(*value_2);
      } else {
        div_on_10(value_2);
        scale_2 = get_scale(*value_2);
      }
    }
  }
}

void increase_scale(s21_decimal *value, int diff) {
  for (int i = 0; i < diff; ++i) {
    multi_10_scale_help(value);
  }
}

void move_decimal_right(s21_decimal *value) {
  int preserve_bit, preserve_bit_prev = 0;
  for (int i = 2; i >= 0; --i) {
    preserve_bit = value->bits[i] % 2;
    value->bits[i] >>= 1;
    value->bits[i] += preserve_bit_prev << 31;
    preserve_bit_prev = preserve_bit;
  }
}

int round_string(char str[14]) {
  int position = -1;
  int original_len = strlen(str);
  for (int i = 0; i < (int)strlen(str); i++) {
    if (str[i] == '.') position = i;
    if (position != -1) str[i] = str[i + 1];
  }
  return position != -1 ? original_len - position - 1 : -1;
}

