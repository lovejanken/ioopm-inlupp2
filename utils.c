#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "utils.h"


extern char *strdup(const char *);

void print(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
    }
}

void println(char *str)
{
    print(str);
    putchar('\n');
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    int buf_siz = 255;
    char buf[buf_siz];

    do
        {
            println(question);
            read_string(buf, buf_siz);
        } while (!check(buf));

    return convert(buf);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value; // svaret som ett heltal
}

bool is_number(char *str)
{   
    //Basfall
    if (strlen(str) < 1)
    {
        return false;
    }

    int i = 0;
    //Kolla om första tecken är '-' men inte enbart
    if (str[0] == '-' && strlen(str) != 1)
    {
        i++;
    }
    while (i < strlen(str))
    {
        if (isdigit(str[i]) == false)
        {
            return false;
        }

        else
        {
            i++;
        }
    }

    return true;

}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

int read_string(char *buf, int buf_size)
{
    int c;

    int counter = 0;
    while (counter < buf_size - 1)
    {
        c = getchar();

        if (c == '\n' || c == EOF)
        {
            break;
        }

        buf[counter] = c;
        counter++;
    }

    if (c != '\n' && c != EOF)
    {
        do
                {
                    c = getchar();
                }
            while (c != '\n' && c != EOF);
            putchar('\n');
            
    }

    buf[counter] = '\0';

    return counter;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}

bool check_yes_or_no(char *string)
{
    if (strlen(string) != 1)
    {
        return false;
    }

    char *valid_inputs = "YyNn";
    char answer = string[0];

    for (int i = 0; valid_inputs[i] != NULL; i++)
    {
        if (answer == valid_inputs[i])
        {
            return true;
        }
    }

    return false;

}

char *ask_question_user_input(char *question, check_func *func)
{
    char *answer = ask_question(question, func, (convert_func *) strdup).string_value;
    printf("%s", answer);
    return answer;
}