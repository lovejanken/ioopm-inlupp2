#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include <stdlib.h>
#include "db.h"

void ioopm_print_menu()
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

static bool check_answer(char *str)
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

char ioopm_ask_question_menu(char *question)
{
    answer_t answer = ask_question(question, check_answer, versal);
    return answer.char_value;
}

typedef bool print_outcome_func_s(store_t *store, char *str);
static void print_outcome_s(print_outcome_func_s *func, store_t *store, char *str, char *if_true, char *if_false)
{
    if (!func(store, str)) {
        printf(if_false, str);
    } else {
        printf(if_true, str);
    }
}

typedef bool print_outcome_func_z(store_t *store, size_t siz);
static void print_outcome_z(print_outcome_func_z *func, store_t *store, size_t siz, char *if_true, char *if_false)
{
    if (!func(store, siz)) {
        printf(if_false, siz);
    } else {
        printf(if_true, siz);
    }
}

typedef bool print_outcome_func_zz(store_t *store, size_t siz1, size_t *siz2);
static void print_outcome_zz(print_outcome_func_zz *func, store_t *store, size_t siz1, size_t siz2, char *if_true, char *if_false)
{
    if (!func(store, siz1, &siz2)) {
        printf(if_false, siz1);
    } else {
        printf(if_true, siz2);
    }
}

typedef bool print_outcome_func_zsz(store_t *store, size_t siz1, char *str, size_t siz2);
static void print_outcome_zsz(print_outcome_func_zsz *func, store_t *store, size_t siz1, char *str, size_t siz2, char *if_true, char *if_false)
{
    if (!func(store, siz1, str, siz2)) {
        printf(if_false, str, siz1);
    } else {
        printf(if_true, siz2, str, siz1);
    }
}

typedef bool print_outcome_func_ssz(store_t *store, char *str1, char *str2, size_t siz);
static void print_outcome_ssz(print_outcome_func_ssz *func, store_t *store, char *str1, char *str2, size_t siz, char *if_true, char *if_false)
{
    if (!func(store, str1, str2, siz)) {
        printf(if_false, str1);
    } else {
        printf(if_true, str1);
    }
}

typedef bool print_outcome_func_sssz(store_t *store, char *str1, char *str2, char *str3, size_t siz);
static void print_outcome_sssz(print_outcome_func_sssz *func, store_t *store, char *str1, char *str2, char *str3, size_t siz, char *if_true, char *if_false)
{
    if (!func(store, str1, str2, str3, siz)) {
        printf(if_false, str1);
    } else {
        printf(if_true, str1);
    }
}

void ioopm_add_merch_menu(store_t *store)
{
    char *name = ask_question_string("Name of the merchandise: ");
    char *description = ask_question_string("Description of the merchandise: ");
    size_t price = ask_question_size("Price of the merchandise: ");
    char *if_true = "Merch %s has been added.\n";
    char *if_false = "Merch %s already exists.\n";
    print_outcome_ssz(ioopm_add_merch, store, name, description, price, if_true, if_false);
    free(name);
    free(description);
}

void ioopm_remove_merch_menu(store_t *store)
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
        char *if_true = "Merch %s has been removed.\n";
        char *if_false = "Merch %s does not exist.\n";
        print_outcome_s(ioopm_remove_merch, store, merch, if_true, if_false);
    }
    free(merch);
    free(confirmation);
}

void ioopm_edit_merch_menu(store_t *store)
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
        char *if_true = "Merch %s has been edited.\n";
        char *if_false = "Could not edit merch %s.\n";
        print_outcome_sssz(ioopm_edit_merch, store, old_name, new_name, new_description, new_price, if_true, if_false);
    }

    else
    {
        printf("Merch %s has not been edited.\n", old_name);
    }
    free(old_name);
    free(new_name);
    free(new_description);
    free(confirmation);
}

void ioopm_show_stock_menu(store_t *store)
{
    ioopm_list_merch(store);
    char *merch = ask_question_string("What merch do you want to see the stock of?");
    char *if_true = "";
    char *if_false = "Merch %s does not exist.\n";
    print_outcome_s(ioopm_show_stock, store, merch, if_true, if_false);
    free(merch);
}

static bool check_is_shelf(char *string)
{
    if (strlen(string) != 3 || !isalpha(string[0]) || !isdigit(string[1]) || !isdigit(string[2]))
    {
        return false;
    }
    return true;
}

void ioopm_replenish_menu(store_t *store)
{
    ioopm_list_merch(store);
    char *shelf = ask_question_string("What is the name of the shelf you want to replenish?");
    char *merch_name = ask_question_string("What is the name of the merch you want to replenish?");
    size_t quantity = ask_question_size("How many items do you want to add?");

    if (check_is_shelf(shelf))
    {
        char *if_true = "Merch %s has been replenished.\n";
        char *if_false = "Merch either doesn't exist or another merch already occupies that shelf.\n";
        print_outcome_ssz(ioopm_replenish, store, merch_name, shelf, quantity, if_true, if_false);
    }

    else
    {
        printf("%s is not a valid ID.\n", shelf);
    }
    free(shelf);
    free(merch_name);
}

void ioopm_create_cart_menu(store_t *store)
{
    ioopm_create_cart(store);
    printf("You have created a new cart with ID: %zu\n", ioopm_store_get_size(store));
}

void ioopm_remove_cart_menu(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to remove?");
    char *confirmation = ask_question_string("[Y/N] Are you sure you want to remove the cart?");
    if (check_yes(confirmation))
    {
        char *if_true = "Cart %zu has been removed.\n";
        char *if_false = "Cart does not exist.\n";
        print_outcome_z(ioopm_remove_cart, store, cart_id, if_true, if_false);
    }

    else
    {
        printf("Cart %zu has not been removed.\n", cart_id);
    }

    free(confirmation);
}

void ioopm_add_to_cart_menu(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to add to?");
    char *merch_name = ask_question_string("What is the name of the merch you want to add to cart?");
    size_t quantity = ask_question_size("How many items do you want to add?");
    char *if_true = "Added %zu of item %s to cart %zu\n";
    char *if_false = "Could not add item %s to cart %zu\n";
    print_outcome_zsz(ioopm_add_to_cart, store, cart_id, merch_name, quantity, if_true, if_false);

    free(merch_name);
}

void ioopm_remove_from_cart_menu(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to remove from?");
    char *merch_name = ask_question_string("What is the name of the merch you want to remove from cart?");
    size_t quantity = ask_question_size("How many items do you want to remove?");
    char *if_true = "Removed %zu of item %s from cart %zu\n";
    char *if_false = "Could not remove item %s from cart %zu\n";
    print_outcome_zsz(ioopm_remove_from_cart, store, cart_id, merch_name, quantity, if_true, if_false);
    free(merch_name);
}

void ioopm_calculate_cost_menu(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to calculate the cost of?");
    size_t cost = 0;
    char *if_false = "Could not calculate cost of cart ID: %zu\n";
    char *if_true = "Your total cost is: %zu\n";
    print_outcome_zz(ioopm_calculate_cost, store, cart_id, cost, if_true, if_false);
}

void ioopm_checkout_menu(store_t *store)
{
    size_t cart_id = ask_question_size("What is the ID of the cart you want to checkout?");
    char *if_true = "Cart %zu has been checked out.\n";
    char *if_false = "Could not checkout cart ID: %zu\n";
    print_outcome_z(ioopm_checkout, store, cart_id, if_true, if_false);
}

bool ioopm_quit_menu(store_t *store)
{
    char *confirmation = ask_question_string("[Y/N] Are you sure you want to quit?");
    if (check_yes(confirmation))
    {
        ioopm_quit(store);
        free(confirmation);
        return false;
    }
    free(confirmation);
    return true;
}

int ioopm_event_loop(store_t *store)
{
    int answer;
    bool continue_shopping = true;
    while (continue_shopping)
    {
        ioopm_print_menu();

        answer = ioopm_ask_question_menu("What do you want to do?: ");

        if (answer == 'A')
        {
            ioopm_add_merch_menu(store);
        }

        else if (answer == 'L')
        {
            ioopm_list_merch(store);
        }

        else if (answer == 'D')
        {
            ioopm_remove_merch_menu(store);
        }

        else if (answer == 'E')
        {
            ioopm_edit_merch_menu(store);
        }

        else if (answer == 'S')
        {
            ioopm_show_stock_menu(store);
        }

        else if (answer == 'P')
        {
            ioopm_replenish_menu(store);
        }

        else if (answer == 'C')
        {
            ioopm_create_cart_menu(store);
        }

        else if (answer == 'R')
        {
            ioopm_remove_cart_menu(store);
        }

        else if (answer == '+')
        {
            ioopm_add_to_cart_menu(store);
        }

        else if (answer == '-')
        {
            ioopm_remove_from_cart_menu(store);
        }

        else if (answer == '=')
        {
            ioopm_calculate_cost_menu(store);
        }

        else if (answer == 'O')
        {
            ioopm_checkout_menu(store);
        }

        else if (answer == 'Q')
        {
            continue_shopping = ioopm_quit_menu(store);
        }
    }

    return 0;
}

int main(void)
{
    store_t *store = ioopm_create_store();
    ioopm_event_loop(store);
}