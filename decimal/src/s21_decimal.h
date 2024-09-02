#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINUS 0x80000000  // 10000000 00000000 00000000 00000000
#define SCALE 0x00ff0000  // 00000000 11111111 00000000 00000001
#define nan 0.0 / 0.0
#define inf +1.0 / 0.0

typedef struct {
  unsigned int bits[4];
} s21_decimal;

// based

// void print_dec(s21_decimal num);

int get_bit(s21_decimal dec, int index);
void set_bit(s21_decimal *num, int index, int bit);
int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, int sign);
int get_scale(s21_decimal num);
int set_scale(s21_decimal *num, int val);
void normalization(s21_decimal *value_1, s21_decimal *value_2);
void increase_scale(s21_decimal *value, int diff);
int is_zero(s21_decimal num);
void left_shift_pro(s21_decimal *dec, int shift);
void left_shift(s21_decimal *dec);
void multi_10_scale_help(s21_decimal *val);
void set_zero(s21_decimal *num);
int round_string(char str[14]);
int div_on_10(s21_decimal *result);
void move_decimal_right(s21_decimal *value);

// compi
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int is_greater_simple(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// arith
int add_simple(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result);
int sub_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// round
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// conv
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal value, int *dst);
int s21_from_decimal_to_float(s21_decimal value, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);

#endif