#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2048

typedef struct flags {
  int e;  //можно задать несколько слов для поиска
  int i;  //игнорит регистр (большая/маленькая буква) во время поиска
  int v;  //выводит все кроме шаблона
  int c;  //выводит кол-во совпадений
  int l;  //печатает только имя файла, в котором найден шаблон
  int n;  //номер строк с совпадением
  int h;  //не пишет имя файла при исп неск
  int s;  //игнорит ошипку
  int f;  //файл - это паттерн
  int mulitple_files;
  char pattern[MAX];
} sus;

void pars(int argc, char **argv, sus *flags);
void pars_dop_opt(int argc, char **argv, sus *flags);
void poutput(char **argv, sus *flags);
void flag_i(int *max_reg, sus *flags);
void flag_f(sus *flag);
void flag_c(int similar_count, char *file_name, sus *flags);
void print_file_name(int similar_count, char *file_name, sus *flags);
void flag_n(int similar_count, sus *flags);
void flag_l(int similar, int similar_count, char **argv, sus *flags);
void flag_v(int *similar, sus *flags);

int main(int argc, char **argv) {
  if (argc > 2) {
    sus flags = {0};
    pars(argc, argv, &flags);
    while (optind < argc) {
      poutput(argv, &flags);
      optind++;
    }
  }
  return 0;
}

void pars(int argc, char **argv, sus *flags) {
  int m = 0;
  while ((m = getopt_long(argc, argv, "e:ivclnhf:s", 0, 0)) != -1) {
    switch (m) {
      case 'e':
        flags->e = 1;
        strncat(flags->pattern, optarg, MAX - strlen(flags->pattern));
        strncat(flags->pattern, "|", MAX - strlen(flags->pattern));
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        flags->n = 0;
        break;
      case 'l':
        flags->l = 1;
        flags->n = 0;
        break;
      case 'n':
        if (flags->c == 0 && flags->l == 0) {
          flags->n = 1;
        }
        break;
      case 'h':
        flags->h = 1;
        break;
      case 'f':
        flags->f = 1;
        flag_f(flags);  // f
        break;
      case 's':
        flags->s = 1;
        break;
      default:
        fprintf(stderr, "grep: Invalid argument\n");
    }
  }
  pars_dop_opt(argc, argv, flags);
}
void pars_dop_opt(int argc, char **argv, sus *flags) {
  if (flags->e == 0 && flags->f == 0) {
    strcat(flags->pattern, argv[optind]);
    optind++;
  }
  if (flags->e == 1 || flags->f == 1) {
    flags->pattern[strlen(flags->pattern) - 1] = '\0';
  }
  if (optind + 1 != argc) {
    flags->mulitple_files = 1;
  }
}

void poutput(char **argv, sus *flags) {  // basic grep
  regmatch_t pattern_start = {0};        // для o
  regex_t regex = {0};
  char buff[MAX] = {0};
  int similar = 0, similar_count = 0, max_reg = 0, count_str = 1;
  FILE *file = fopen(argv[optind], "rt");
  if (file != NULL) {
    flag_i(&max_reg, flags);  // i

    regcomp(&regex, flags->pattern, max_reg);

    while (fgets(buff, MAX, file) != NULL) {
      similar = regexec(&regex, buff, 1, &pattern_start, 0);

      flag_v(&similar, flags);  // v
      if (similar == 0) {
        similar_count++;
        if (flags->mulitple_files)
          print_file_name(similar_count, argv[optind], flags);
        if (flags->n) flag_n(count_str, flags);  // n
        if (!flags->l && !flags->c) {
          printf("%s", buff);
          if (buff[strlen(buff) - 1] != '\n') printf("\n");
        }
        flag_l(similar, similar_count, argv, flags);  // l
      }
      count_str++;
    }
    if (flags->c) {
      flag_c(similar_count, argv[optind], flags);  //с
    }
    regfree(&regex);
  } else {
    if (!flags->s)  // s
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
  fclose(file);
}

void flag_f(sus *flags) {
  char buff[MAX] = {0};
  FILE *file = fopen(optarg, "rt");
  if (file != NULL) {
    while (fgets(buff, sizeof(buff), file) != NULL) {
      size_t len = strlen(buff);
      if (len > 0 && buff[len - 1] == '\n') {
        buff[len - 1] = '\0';
      }
      strncat(flags->pattern, buff, MAX - strlen(flags->pattern));
      strncat(flags->pattern, "|", MAX - strlen(flags->pattern));
    }
  } else {
    fprintf(stderr,
            "usage: grep [-eivclnhsfo]\n\
              [-e pattern] [-f file]\n\
              [pattern] [file ...]\n");
  }
  fclose(file);
}

void flag_i(int *max_reg, sus *flags) {
  *max_reg = REG_EXTENDED;
  if (flags->i) {
    *max_reg = REG_EXTENDED | REG_ICASE;  // | =  совмещение
  }
}

void flag_c(int similar_count, char *file_name, sus *flags) {
  if (!flags->c) {
    printf("%s:", file_name);
  }

  if (similar_count == 0) {
    if (!flags->n) printf("%d\n", similar_count);
  }

  else if (!flags->l) {
    printf("%d\n", similar_count);
  } else if (similar_count) {
    printf("1");
  }
}

void print_file_name(int similar_count, char *file_name, sus *flags) {
  if (!flags->h && !flags->l) {  // h
    if (!flags->c || similar_count == 1) {
      printf("%s:", file_name);
    }
  }
}

void flag_n(int count_str, sus *flags) {
  if (flags->n) {
    printf("%d:", count_str);
  }
}

void flag_l(int similar, int similar_count, char **argv, sus *flags) {
  if (flags->l && similar_count == 1 && !similar) printf("%s\n", argv[optind]);
}

void flag_v(int *similar, sus *flags) {
  if (flags->v) *similar = !*similar;
}