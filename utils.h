#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @file utils.h
 * @author Love Jonasson Ankén, Walter Erik Blomberg, Alexander Thoresson
 * @date 12 oct 2026
 * @brief Some general utility functions, mostly focused on ask_question.
 *
 * answer_t is a union type that makes it so that ask_question can return different types.
 * Functions not tied to a single program. Most of them are based of ask_question and
 * are more specified, or they are the check functions for the more specified functions.
 */

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
  char char_value;
  size_t size_value;
} answer_t;

typedef bool check_func(char *str);
typedef answer_t convert_func(char *str);

/// @brief Write string str to stdout
/// @param str string to be printed
void print(char *str);

/// @brief Write string str to stdout, ending with a linebreak
/// @param str string to be printed
void println(char *str);

/// @brief Change the first char to be uppercase
/// @param str string to be changed
/// @return changed string
answer_t versal(char *str);

/// @brief Reads a string from stdin
/// @param buf where the string will be stored
/// @param buf_siz the size of buf
/// @return the length of the read string including '\0'
int read_string(char *buf, int buf_siz);

/// @brief Asks a question and returns an answer
/// @param question the question to be asked
/// @param check a function to controll the answer
/// @param convert a function to convert the answer
/// @return the answer to the question
answer_t ask_question(char *question, check_func *check, convert_func *convert);

/// @brief Cheks if string str is a number
/// @param str the string to be checked
/// @return true if str is a number, otherwise false
bool is_number(char *str);

/// @brief Asks question expecting an integer for an answer
/// @param question the question to be asked
/// @return the answer to the question as an int
int ask_question_int(char *question);

/// @brief Checks if string str is a positive number
/// @param str the string to be checked
/// @return true if str is a positive number, otherwise false
bool is_positive(char *str);

/// @brief Asks question expecting a size_t for an answer
/// @param question the question to be asked
/// @return the answer to the question as a size_t
size_t ask_question_size(char *question);

/// @brief Checks if string str is empty
/// @param str the string to be checked
/// @return true if str is a non empty string, otherwise false
bool is_not_empty(char *str);

/// @brief Asks question expecting a string for an answer
/// @param question the question to be asked
/// @return the answer to the question as a string
char *ask_question_string(char *question);

/// @brief Checks if first char of string is equal to any char in valid_inputs
/// @param string the string to be checked
/// @param valid_inputs the valid char in a string
/// @return true if string is a valid input, otherwise false
bool check_valid_input(char *string, char *valid_inputs);

/// @brief Checks if first char of string is y/Y
/// @param string the string to be checked
/// @return true if the first char of string is y/Y, otherwise false
bool check_yes(char *string);

/// @brief Checks if first char of string is n/N
/// @param string the string to be checked
/// @return true if the first char of string is n/N, otherwise false
bool check_no(char *string);

/// @brief Returns true
/// @param string the string to be checked, but is not used
/// @return true
bool always_true(char *string);

/// @brief Asks question expecting a string for an answer, with specific check function
/// @param question the question to be asked
/// @param func a function to controll the answer
/// @return the answer to the question as a string
char *ask_question_user_input(char *question, check_func *func);
#endif