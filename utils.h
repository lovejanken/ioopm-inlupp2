#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
  char char_value;
  size_t size_value;
} answer_t;

typedef bool check_func(char *str);
typedef answer_t convert_func(char *str);

void println(char *str);
void print(char *str);
int read_string(char *buf, int buf_siz);
bool is_positive(char *str);
bool is_number(char *str);
answer_t versal(char *str);
int ask_question_int(char *question);
size_t ask_question_size(char *question);
char *ask_question_string(char *question);
void print(char *str);
void println(char *str);
answer_t ask_question(char *question, check_func *check, convert_func *convert);
bool not_empty(char *str);
char *ask_question_user_input(char *question, check_func *func);
bool check_yes(char *string);
bool check_no(char *string);
bool check_is_shelf(char *string);
bool always_true(char *string);
#endif