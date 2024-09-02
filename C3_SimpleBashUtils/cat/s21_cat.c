#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int count;
} sus;

int pars(int argc, char **argv, sus *flag);
void read_file(sus flag, char *filename);
void v_symbols(int *m);
void print_b(int *m, int *index);

int main(int argc, char **argv) {
  int flag_file = 0;
  sus flag = {0};
  int error = pars(argc, argv, &flag);
  if (error == 0) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-' || flag_file == 1) {
        read_file(flag, argv[i]);
        flag_file = 1;
      }
    }
  }
  return 0;
}

int pars(int argc, char **argv, sus *flag) {
  int m = 0, error = 0;
  const struct option long_flags[] = {
      {"number", no_argument, NULL, 'n'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  while ((m = getopt_long(argc, argv, "+nbstTeEv", long_flags, 0)) != -1)
    switch (m) {
      case 'b':
        flag->b = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'E':
        flag->e = 1;
        flag->v = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'T':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      default:
        error = 1;
        fprintf(stderr, "cat: illegal option -- %s\n", argv[optind]);
        fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
        break;
    }
  return error;
}

void read_file(sus flag, char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (file != NULL) {
    int prev_str = '\n', prev_prev_str, m = fgetc(file);

    if (flag.b == 0 && flag.n == 0 && flag.e == 0 && flag.s == 0 &&  //класс кэт
        flag.t == 0 && flag.v == 0) {
      while (m != EOF) {
        putc(m, stdout);
        m = fgetc(file);
      }
    } else {
      int index = 1;
      while (m != EOF) {
        if (flag.b == 1 && prev_str == '\n' && m != '\n')  // b
          print_b(&m, &index);
        if (flag.s == 1 && m == '\n' && prev_str == '\n' &&  // s
            prev_prev_str == '\n') {
          prev_prev_str = prev_str;
          prev_str = m;
          m = fgetc(file);
          continue;
        }
        if (flag.n == 1 && flag.b == 0 && prev_str == '\n')  // n
          printf("%6d\t", index++);
        if ((flag.t == 1 && m == '\t' && flag.v == 1) ||  // t
            (flag.t == 1 && m == '\t')) {
          fputc('^', stdout);
          fputc('I', stdout);
          prev_str = m;
          m = fgetc(file);
          continue;
        }
        if ((flag.e == 1 && m == '\n') ||
            (flag.v == 1 && flag.e == 1 && m == '\n'))  // e
          printf("$");
        if (flag.v == 1)  // v
          v_symbols(&m);
        fputc(m, stdout);
        prev_prev_str = prev_str;
        prev_str = m;
        m = fgetc(file);
      }
    }
    fclose(file);
  } else {
    fprintf(stderr, "cat: %s: No such file or directory: \n", file_name);
  }
}

void v_symbols(int *m) {
  if (*m > 127 && *m < 160) printf("M-^");
  if ((*m < 32 && *m != '\n' && *m != '\t') || *m == 127) printf("^");
  if ((*m < 32 || (*m > 126 && *m < 160)) && *m != '\n' && *m != '\t')
    *m = *m > 126 ? *m - 128 + 64 : *m + 64;
}

void print_b(int *m, int *index) {
  if (*m != '\n') {
    printf("%6d\t", (*index)++);
  }
}
