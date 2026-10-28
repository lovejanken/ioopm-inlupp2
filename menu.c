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

bool check_answer(char *str)
{
    if (strlen(str) != 1)
    {
        return false;
    }

    char *correct_answers = "AaLlDdEeSsPpCcRr+-=OoQq";
    for (size_t i = 0; i < strlen(correct_answers); i++)
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
    size_t price = ask_question_size("Price of the merchandise: ");
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
    size_t new_price = ask_question_size("What do you want the new price to be?");

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
    size_t quantity = ask_question_size("How many items do you want to add?");

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
    printf("You have created a new cart with ID: %zu\n", store_get_size(store));
}

void remove_cart_question(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to remove?");
    char *confirmation = ask_question_string("[Y/N] Are you sure you want to remove the cart?");
    if (check_yes(confirmation))
    {
        if (!ioopm_remove_cart(store, cart_id))
        {
            printf("Cart does not exist.\n");
        }

        else
        {
            printf("Cart %zu has been removed.\n", cart_id);
        }
    }

    else
    {
        printf("Cart %zu has not been removed.\n", cart_id);
    }
}

void add_to_cart_question(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to add to?");
    char *merch_name = ask_question_string("What is the name of the merch you want to add to cart?");
    size_t quantity = ask_question_size("How many items do you want to add?");

    if (!ioopm_add_to_cart(store, cart_id, merch_name, quantity))
    {
        printf("Could not add item %s to cart\n", merch_name);
    }

    else
    {
        printf("Added %zu of item %s to cart %zu\n", quantity, merch_name, cart_id);
    }
}

void remove_from_cart_question(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to remove from?");
    char *merch_name = ask_question_string("What is the name of the merch you want to remove from cart?");
    size_t quantity = ask_question_size("How many items do you want to remove?");

    if (!ioopm_remove_from_cart(store, cart_id, merch_name, quantity))
    {
        printf("Could not remove item %s from cart %zu\n", merch_name, cart_id);
    }

    else
    {
        printf("Removed %zu of item %s from cart %zu\n", quantity, merch_name, cart_id);
    }
}

void calculate_cost_question(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to calculate the cost of?");
    size_t cost = 0;
    if (ioopm_calculate_cost(store, cart_id, &cost))
    {
        printf("Your total cost is: %zu\n", cost);
    }

    else
    {
        printf("Could not calculate cost of cart ID: %zu\n", cart_id);
    }
}

void checkout_question(store_t *store)
{
        size_t cart_id = ask_question_size("What is the ID of the cart you want to checkout?");
        if (!ioopm_checkout(store, cart_id))
        {
            printf("Could not checkout cart ID: %zu\n", cart_id);
        }

        else
        {
            printf("Cart %zu has been checked out.\n", cart_id);
        }
}

bool quit_menu(store_t *store)
{
    char *confirmation = ask_question_string("[Y/N] Are you sure you want to quit?");
    if (check_yes(confirmation))
    {
        ioopm_quit(store);
        return false;
    }
    return true;
}

int event_loop(store_t *store)
{
    int answer;
    bool continue_shopping = true;
    while (continue_shopping)
    {
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

        else if (answer == 'Q')
        {
            continue_shopping = quit_menu(store);
        }

    }

    return 0;
}

int main(void)
{
    store_t *store = ioopm_create_store();
    event_loop(store);
}