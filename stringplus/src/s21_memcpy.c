#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  /*Копирует n символов из src в dest.*/
  char *d = dest;
  const char *s = src;

  for (s21_size_t i = 0; i < n; ++i) {
    d[i] = s[i];
  }

  return d;
}