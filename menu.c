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
        "[Q] Quit\n"
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
        if (str[0] == correct_answers[i]) return true;
    }

    return false;
}

char ask_question_menu(char *question)
{
    answer_t answer = ask_question(question, check_answer, versal);
    return answer.char_value;
}

void add_merch_question(store_t *store)
{
    char *name = ask_question_string("Name of the merchandise: ");
    char *description = ask_question_string("Description of the merchandise: ");
    int price = ask_question_int("Price of the merchandise: ");
    if (!ioopm_add_merch(store, name, description, price))
    {
        printf("Merch %s already exists.\n", name);
    }

    else
    {
        printf("Merch %s has been added.\n", name);
    }
}

void remove_merch_question(store_t *store)
{
    if (ioopm_merch_is_empty(store))
    {
        printf("There are no merch to remove.\n");
        return;
    }

    ioopm_list_merch(store);
    char *merch = ask_question_string("What merchandise do you want to remove?");
    char *confirmation = ask_question_string("[Y/N] Are you sure you want to remove the merch?");
    if (check_yes(confirmation))
    {
        if (!ioopm_remove_merch(store, merch))
        {
            printf("Merch %s does not exist.\n", merch);
        }

        else
        {
        printf("Merch %s has been removed.\n", merch);
        }
    }
}

void edit_merch_question(store_t *store)
{
    if (ioopm_merch_is_empty(store))
    {
        printf("There are no merch to edit.\n");
        return;
    }

    ioopm_list_merch(store);
    char *old_name = ask_question_string("What merchandise do you want to edit?");
    char *new_name = ask_question_string("What do you want the new name to be?");
    char *new_description = ask_question_string("What do you want the new description to be?");
    int new_price = ask_question_int("What do you want the new price to be?");

    char *confirmation = ask_question_string("[Y/N] Are you sure you want to edit the merch?");
    if (check_yes(confirmation))
    {
        if (!ioopm_edit_merch(store, old_name, new_name, new_description, new_price))
        {
            printf("Could not edit merch %s.\n", old_name);
        }

        else
        {
        printf("Merch %s has been edited.\n", old_name);
        }
    }

    else
    {
        printf("Merch %s has not been edited.\n", old_name);
    }
}

void show_stock_question(store_t *store)
{
    ioopm_list_merch(store);
    char *merch = ask_question_string("What merch do you want to see the stock of?");
    if (!ioopm_show_stock(store, merch))
    {
        printf("Merch %s does not exist.\n", merch);
    }
}

void replenish_question(store_t *store)
{
    ioopm_list_merch(store);
    char *shelf = ask_question_string("What is the name of the shelf you want to replenish?");
    char *merch_name = ask_question_string("What is the name of the merch you want to replenish?");
    int quantity = ask_question_int("How many items do you want to add?");

    if (check_is_shelf(shelf))
    {
        if (!ioopm_replenish(store, shelf, merch_name, quantity))
        {
            printf("Merch either doesn't exist or another merch already occupies that shelf.\n");
        }

        else
        {
        printf("Merch %s has been replenished.\n", merch_name);
        }
    }

    else
    {
        printf("%s is not a valid ID.\n", shelf);
    }
}

void create_cart_menu(store_t *store)
{
    ioopm_create_cart(store);
    printf("You have created a new cart with ID: %d\n", store_get_size(store));
}

void remove_cart_question(store_t *store)
{
    int cart_id = ask_question_int("What is the ID of the cart you want to remove?");
    char *confirmation = ask_question_string("[Y/N] Are you sure you want to remove the cart?");
    if (check_yes(confirmation))
    {
        if (!ioopm_remove_cart(store, cart_id))
        {
            printf("Cart does not exist.\n");
        }

        else
        {
            printf("Cart %d has been removed.\n", cart_id);
        }
    }

    else
    {
        printf("Cart %d has not been removed.\n", cart_id);
    }
}

void add_to_cart_question(store_t *store)
{
    int cart_id = ask_question_int("What is the ID of the cart you want to add to?");
    char *merch_name = ask_question_string("What is the name of the merch you want to add to cart?");
    int quantity = ask_question_int("How many items do you want to add?");

    if (!ioopm_add_to_cart(store, cart_id, merch_name, quantity))
    {
        printf("Could not add item %s to cart\n", merch_name);
    }

    else
    {
        printf("Added %d of item %s to cart %d\n", quantity, merch_name, cart_id);
    }
}

void remove_from_cart_question(store_t *store)
{
    int cart_id = ask_question_int("What is the ID of the cart you want to remove from?");
    char *merch_name = ask_question_string("What is the name of the merch you want to remove from cart?");
    int quantity = ask_question_int("How many items do you want to remove?");

    if (!ioopm_remove_from_cart(store, cart_id, merch_name, quantity))
    {
        printf("Could not remove item %s from cart %d\n", merch_name, cart_id);
    }

    else
    {
        printf("Removed %d of item %s from cart %d\n", quantity, merch_name, cart_id);
    }
}

void calculate_cost_question(store_t *store)
{
    int cart_id = ask_question_int("What is the ID of the cart you want to calculate the cost of?");
    int cost = 0;
    if (ioopm_calculate_cost(store, cart_id, &cost))
    {
        printf("Your total cost is: %d\n", cost);
    }

    else
    {
        printf("Could not calculate cost of cart ID: %d\n", cart_id);
    }
}

void checkout_question(store_t *store)
{
        int cart_id = ask_question_int("What is the ID of the cart you want to checkout?");
        if (!ioopm_checkout(store, cart_id))
        {
            printf("Could not checkout cart ID: %d\n", cart_id);
        }

        else
        {
            printf("Cart %d has been checked out.\n", cart_id);
        }
}

int event_loop(store_t *store)
{
    int answer;
    do {
        print_menu();

        answer = ask_question_menu("What do you want to do?: ");

        if (answer == 'A')
        {
            add_merch_question(store);
        }

        else if (answer == 'L')
        {
            ioopm_list_merch(store);
        }

        else if (answer == 'D')
        {
            remove_merch_question(store);
        }

        else if (answer == 'E')
        {
            edit_merch_question(store);
        }

        else if (answer == 'S')
        {
            show_stock_question(store);
        }

        else if (answer == 'P')
        {
            replenish_question(store);
        }

        else if (answer == 'C')
        {
            create_cart_menu(store);
        }

        else if (answer == 'R')
        {
            remove_cart_question(store);
        }

        else if (answer == '+')
        {
            add_to_cart_question(store);
        }

        else if (answer == '-')
        {
            remove_from_cart_question(store);
        }

        else if (answer == '=')
        {
            calculate_cost_question(store);
        }

        else if (answer == 'O')
        {
            checkout_question(store);
        }

    } while (answer != 'Q');

    return 0;
}

int main(void)
{
    store_t *store = ioopm_create_store();
    event_loop(store);
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