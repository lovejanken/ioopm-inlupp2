#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

struct item
{
    char *name;
    char *description;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_item(item_t *item1)
{
    int SEK = item1->price / 100;
    int decimal = item1->price % 100;
    printf("Name: %s\nDesc: %s\nPrice: %d.%d\nShelf: %s\n", item1->name, item1->description, SEK, decimal, item1->shelf);
}

item_t make_item(char *name, char *description, int price, char *shelf)
{
    item_t new_item;
    new_item.name = name;
    new_item.description = description;
    new_item.price = price;
    new_item.shelf = shelf;
    return new_item;
}

bool is_shelf(char *shelf)
{ 
    //Basfall
    if (strlen(shelf) <= 1 || !isalpha(shelf[0]))
    {
        return false;
    }

    int i = 1;
    while (i < strlen(shelf))
    {
        if (!isdigit(shelf[i]))
        {
            return false;
        }

        i++;
    }

    return true;

}

char *ask_question_shelf(char *question)
{
    answer_t answer = ask_question(question, is_shelf, (convert_func *) strdup);
    return answer.string_value;
}

item_t input_item()
{

    char *name = ask_question_string("Namn på artikeln: ");
    char *description = ask_question_string("Beskrivning på artikeln: ");
    int price = ask_question_int("Pris på artikeln: ");
    char *shelf = ask_question_shelf("Hyllplats: ");

    item_t new_item = make_item(name, description, price, shelf);
    return new_item;
}

char *magick(char *array1[], char *array2[], char *array3[], int length)
{
    char buf[255];
    int first_random = rand() % length;
    int second_random = rand() % length;
    int third_random = rand() % length;

    int i = 0;
    // https://www.geeksforgeeks.org/cpp/different-ways-to-copy-a-string-in-c-c/
    // Använde denna för att hitta strcpy och strcat, om man får använda det istället för en loop?

    strcpy(buf, array1[first_random]);
    strcat(buf, "-");
    strcat(buf, array2[second_random]);
    strcat(buf, " ");
    strcat(buf, array3[third_random]);
    strcat(buf, "\0");
    return strdup(buf);

}

void list_db(item_t *items, int no_items)
{
    for (int i = 0; i < no_items; i++)
    {
        printf("%d. %s\n", i + 1, items[i].name);
    }
}

void editdb(item_t *items, int no_items)
{
    int vara;
    do {
        vara = ask_question_int("Vilken vara vill du redigera?: ");
        } while (vara < 1 || vara > no_items);
    print_item(&items[vara - 1]);
    printf("Du kan nu redigera varan.\n");
    items[vara - 1] = input_item();
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
  char *array1[] = {"apa", "katt", "hund" }; // TODO: Lägg till!
  char *array2[] = { "tandkräm", "tandborste", "munskydd" }; // TODO: Lägg till!
  char *array3[] = { "telefon", "dator", "tv" }; // TODO: Lägg till!

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
        char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
        int price = rand() % 200000;
        char shelf[] = { rand() % ('Z'-'A') + 'A',
                         rand() % 10 + '0',
                         rand() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, strdup(shelf));

        db[db_siz] = item;
        ++db_siz;
      }

     // Skriv ut innehållet
     list_db(db, db_siz);
     editdb(db, db_siz);
     list_db(db, db_siz);


  }
  return 0;
}