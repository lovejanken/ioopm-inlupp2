#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "linked_list.h"
#include "hash_table.h"
#include "list_iterator.h"
#include "common.h"
#include "db.h"
#include "hash_table_iterator.h"


void print_menu()
{
    printf(
        "[A] Add Merchandise\n"
        "[L] List Merchandise\n"
        "[D] Remove Merchandise\n"
        "[E] Edit Merchandise\n"
        "[S] Show Stock\n"
        "[P] Replenish\n"
        "[C] Create Cart\n"
        "[R] Remove Cart\n"
        "[+] Add To Cart\n"
        "[-] Remove From Cart\n"
        "[=] Calculate Cost\n"
        "[O] Checkout\n"
        "[Q] Quit"
    );
}

answer_t versal(char *str)
{
    answer_t converted;
    converted.char_value = toupper(str[0]);
    return converted;
}

bool check_answer(char *str)
{
    if (strlen(str) != 1)
    {
        return false;
    }

    char *correct_answers = "AaLlDdEeSsPpCcRr+-=OoQq";
    for (int i = 0; i < strlen(correct_answers); i++)
    {
        if (str == correct_answers[i]) return true;
    }

    return false;
}

char ask_question_menu(char *question)
{
    answer_t answer = ask_question(question, check_answer, versal);
    return answer.char_value;
}

int event_loop(store_t *store)
{
    int answer;
    do {
        print_menu();

        answer = ask_question_menu("What do you want to do?: ");

        if (answer == 'A')
        {
            ioopm_add_merch()
        }

        else if (answer == 'T')
        {
            no_items = remove_item_from_db(items, no_items);
        }

        else if (answer == 'R')
        {
            editdb(items, no_items);
        }

        else if (answer == 'G')
        {
            printf("Not yet implemented\n");
        }

        else if (answer == 'H')
        {
            list_db(items, no_items);
        }
    } while (answer != 'A');

    return 0;
}


/*
struct item
{
    char *name;
    char *description;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_menu()
{
    printf(
        "[L]ägga till en vara\n"
        "[T]a bort en vara\n"
        "[R]edigera en vara\n"
        "Ån[g]ra senaste ändringen\n"
        "Lista [h]ela varukatalogen\n"
        "[A]vsluta\n"
    );
}

bool check_answer(char *str)
{
    if (strlen(str) != 1)
    {
        return false;
    }

    else if (
        str[0] == 'A' ||
        str[0] == 'a' ||
        str[0] == 'L' ||
        str[0] == 'l' ||
        str[0] == 'T' ||
        str[0] == 't' ||
        str[0] == 'g' ||
        str[0] == 'G' ||
        str[0] == 'h' ||
        str[0] == 'H' ||
        str[0] == 'R' ||
        str[0] == 'r')
    {
        return true;
    }
    else {
        return false;
    }
}

answer_t versal(char *str)
{
    answer_t converted;
    converted.char_value = toupper(str[0]);
    return converted;
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

char ask_question_menu(char *question)
{
    answer_t answer = ask_question(question, check_answer, versal);
    return answer.char_value;
}

void print_item(item_t *item1)
{
    int SEK = item1->price / 100;
    int decimal = item1->price % 100;
    printf("Name: %s\nDesc: %s\nPrice: %d.%d\nShelf: %s\n", item1->name, item1->description, SEK, decimal, item1->shelf);
}

void list_db(item_t *items, int no_items)
{
    for (int i = 0; i < no_items; i++)
    {
        printf("%d. %s\n", i + 1, items[i].name);
    }
}

int remove_item_from_db(item_t *items, int no_items)
{
    list_db(items, no_items);
    int vara;
    do {
        vara = ask_question_int("Vilken vara vill du ta bort?: ");
        } while (vara < 1 || vara > no_items);

    for (int i = vara - 1; i < no_items; i++)
        {
            items[i] = items[i + 1];
        }
    return no_items = no_items - 1;
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

item_t input_item()
{
    char *name = ask_question_string("Namn på artikeln: ");
    char *description = ask_question_string("Beskrivning på artikeln: ");
    int price = ask_question_int("Pris på artikeln: ");
    char *shelf = ask_question_shelf("Hyllplats: ");
    item_t new_item = make_item(name, description, price, shelf);
    return new_item;
}

bool is_positive(char *str)
{
    if (is_number(str) == false)
    {
        return false;
    }

    else
    {
        if (str[0] == '-') {
            return false;
        }

        else
        {
            return true;
        }
    }
}

void editdb(item_t *items, int no_items)
{
    int vara;
    do {
        list_db(items, no_items);
        vara = ask_question_int("Vilken vara vill du redigera?: ");
        } while (vara < 1 || vara > no_items);
    print_item(&items[vara - 1]);
    printf("Du kan nu redigera varan.\n");
    items[vara - 1] = input_item();
}

int add_item_to_db(item_t *items, int no_items)
{
    item_t new_item = input_item();
    items[no_items] = new_item;
    return no_items + 1;
}

int event_loop(item_t *items, int no_items)
{
    int answer;
    do {
        print_menu();

        answer = ask_question_menu("Vad vill du göra?: ");

        if (answer == 'L')
        {
            no_items = add_item_to_db(items, no_items);
        }

        else if (answer == 'T')
        {
            no_items = remove_item_from_db(items, no_items);
        }

        else if (answer == 'R')
        {
            editdb(items, no_items);
        }

        else if (answer == 'G')
        {
            printf("Not yet implemented\n");
        }

        else if (answer == 'H')
        {
            list_db(items, no_items);
        }
    } while (answer != 'A');

    return 0;
}

int main()
{
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu
    event_loop(db, db_siz);
}
    */