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

// Hej

struct cart
{
    ioopm_hash_table_t *items;
};

struct cart_item
{
    merch_t *item;
    int amount;
};

struct store {
    ioopm_hash_table_t *merch_table;
    ioopm_hash_table_t *stock_by_shelf;
    ioopm_list_t *carts;
};

store_t *ioopm_create_store(void)
{
    store_t *store = malloc(sizeof(store_t));
    store->carts = ioopm_list_create();
    store->merch_table = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);
    store->stock_by_shelf = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);

    return store;
}

static void remove_stock(merch_t item)
{
    shelf_list_t *location_list = item.locs;
    if (location_list != NULL)
    {
        ioopm_list_destroy(location_list);
    }
}

static void remove_stock_from_ht(store_t *store, merch_t merch)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->stock_by_shelf);
    elem_t result = int_elem(-1);
    while(!ioopm_hash_table_iterator_at_end(it))
    {
        if (strcmp(ioopm_hash_table_iterator_current_value(it).m.name, merch.name) == 0)
        {
            ioopm_hash_table_remove(store->stock_by_shelf, ioopm_hash_table_iterator_current_key(it), &result);
        }
        ioopm_hash_table_iterator_advance(it);
    }
}

static void destroy_merch_data(merch_t merch)
{
    if (merch.locs != NULL) {
        for (int i = 0; i < ioopm_list_size(merch.locs); i++) {
            shelf_t shelf = ioopm_list_get(merch.locs, i).sh;
            free(shelf.shelf);
        }

        ioopm_list_destroy(merch.locs);
    }

    free(merch.name);
    free(merch.description);
}

static void destroy_all_merch(store_t *store)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->merch_table);

    while (!ioopm_hash_table_iterator_at_end(it)) {
        merch_t merch = ioopm_hash_table_iterator_current_value(it).m;

        destroy_merch_data(merch);
        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);
}

static void destroy_cart(cart_t *cart)
{
    if (cart == NULL)
    {
        return;
    }

    ioopm_hash_table_destroy(cart->items);
    free(cart);
}

static void destroy_all_carts(store_t *store)
{
    while (!ioopm_list_is_empty(store->carts))
    {
        elem_t current_entry = ioopm_list_remove(store->carts, 0);
        destroy_cart(current_entry.p);
    }

    ioopm_list_destroy(store->carts);
    store->carts = NULL;
}


void ioopm_destroy_store(store_t *store)
{
    destroy_all_carts(store);
    ioopm_hash_table_destroy(store->stock_by_shelf);
    destroy_all_merch(store);
    ioopm_hash_table_destroy(store->merch_table);
    free(store);
}

bool ioopm_add_merch(store_t *store, char *name, char *desc, int price)
{
    elem_t local;
    if (ioopm_hash_table_lookup(store->merch_table, string_elem(name), &local))
    {
        return false;
    }

    merch_t new_item = 
    {
        .name = strdup(name),
        .description = strdup(desc),
        .price = price,
        .locs = ioopm_list_create()
    };

    ioopm_hash_table_insert(store->merch_table, string_elem(name), merch_elem(new_item));
    return true;
}

bool ioopm_remove_merch(store_t *store, char *name)
{
    elem_t merch;
    elem_t local;
    if (!ioopm_hash_table_remove(store->merch_table, string_elem(name), &merch))
    {
        return false;
    }

    for (int i = 0; i < ioopm_list_size(merch.m.locs); i++)
    {
        shelf_t shelf = ioopm_list_get(merch.m.locs, i).sh;
        ioopm_hash_table_remove(store->stock_by_shelf, string_elem(shelf.shelf), &local);
    }

    destroy_merch_data(merch.m);
    return true;
}

void ioopm_list_merch(store_t *store)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->merch_table);

    int counter = 0;

    while (!ioopm_hash_table_iterator_at_end(it))
    {
        if (counter == 4)
        {
            char *answer = ask_question_user_input("Do you want to continue printing merchandise?", check_yes_or_no);
            if (strcmp(answer, "N") == 0)
            {
                printf("%s", answer);
                break;
            }

            else
            {
                counter = 0;
            }
        }

        merch_t current_item = ioopm_hash_table_iterator_current_value(it).m;
        printf("Name: %s\nDescription: %s\nPrice: %d\n", current_item.name, current_item.description, current_item.price);
        ioopm_hash_table_iterator_advance(it);
        counter++;
    }

}

bool ioopm_edit_merch(store_t *store, char *old_name, char *new_name, char *new_description, int new_price)
{
    elem_t local;
    if (ioopm_hash_table_lookup(store->merch_table, string_elem(new_name), &local))
    {
        return false;
    }

    elem_t  old_merch;
    
    ioopm_hash_table_lookup(store->merch_table, string_elem(old_name), &old_merch);

    merch_t edited_merch = {
        .name = strdup(new_name),
        .description = strdup(new_description),
        .price = new_price,
        .locs = old_merch.m.locs
    };

    if (ioopm_hash_table_remove(store->merch_table, string_elem(old_name), &local))
    {
        ioopm_hash_table_insert(store->merch_table, string_elem(new_name), merch_elem(edited_merch));

        for (int i = 0; i < ioopm_list_size(edited_merch.locs); i++)
        {
            shelf_t shelf = ioopm_list_get(edited_merch.locs, i).sh;
            ioopm_hash_table_insert(store->stock_by_shelf, string_elem(shelf.shelf), merch_elem(edited_merch));
        }

        free(old_merch.m.name);
        free(old_merch.m.description);

        return true;
    }

    return false;
}

bool ioopm_get_stock_from_location(store_t *store, char *merch_name, char *location, int *result)
{
    elem_t merch = int_elem(-1);
    if (!ioopm_hash_table_lookup(store->merch_table, string_elem(merch_name), &merch))
    {
        return false;
    }

    ioopm_list_t *list = merch.m.locs;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    while (!ioopm_list_iterator_at_end(it))
    {
        shelf_t shelf = ioopm_list_iterator_current(it).sh;
        if (strcmp(shelf.shelf, location) == 0)
        {
            *result = shelf.quantity;

            ioopm_list_iterator_destroy(it);
            return true;
        }
        ioopm_list_iterator_advance(it);
    }

    ioopm_list_iterator_destroy(it);

    return false;
    
}

bool ioopm_show_stock(store_t *store, merch_t item)
{
    if (ioopm_hash_table_has_key(store->merch_table, merch_elem(item)))
    {
        shelf_list_t *item_list = item.locs;
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(item_list);
        do
        {
            elem_t shelf = ioopm_list_iterator_current(it);
            printf("Shelf: %s, Quantity: %d", shelf.sh.shelf, shelf.sh.quantity);
            ioopm_list_iterator_advance(it);
        } while (!ioopm_list_iterator_at_end(it));

        return true;
    }

    else return false;
}

static elem_t find_shelf_in_list(ioopm_list_t *list, char *key)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    while (!ioopm_list_iterator_at_end(it))
    {
        if (ioopm_list_iterator_current(it).sh.shelf == key)
        {
            return ioopm_list_iterator_current(it);
        }
        ioopm_list_iterator_advance(it);
    }
}

bool ioopm_replenish(store_t *store, char *location, char *merch_name, int amount)
{
    if (amount < 1) return false;

    elem_t merch = int_elem(-1);
    if (ioopm_hash_table_lookup(store->stock_by_shelf, string_elem(location), &merch))
    {

        if (strcmp(merch.m.name, merch_name) != 0)
        {
            return false;
        }

        ioopm_list_t *list = merch.m.locs;
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

        while (!ioopm_list_iterator_at_end(it))
        {
            shelf_t shelf = ioopm_list_iterator_current(it).sh;
            if (strcmp(shelf.shelf, location) == 0)
            {
                shelf.quantity += amount;
                ioopm_list_iterator_remove(it);
                ioopm_list_iterator_insert(it, shelf_elem(shelf));
            }
            ioopm_list_iterator_advance(it);
        }

        return true;
    }

    else
    {
        ioopm_hash_table_lookup(store->merch_table, string_elem(merch_name), &merch);

        shelf_t shelf = {
            .quantity = amount,
            .shelf = strdup(location)
        };

        ioopm_list_append(merch.m.locs, shelf_elem(shelf));
        ioopm_hash_table_insert(store->stock_by_shelf, string_elem(shelf.shelf), merch);
        return true;
    }
}

cart_t *ioopm_create_cart(store_t *store)
{
    cart_t *new_cart = malloc(sizeof(cart_t));
    new_cart->items = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);
    ioopm_list_append(store->carts, ptr_elem(new_cart));
    return new_cart;
}

bool ioopm_add_to_cart(store_t *store, cart_t *cart, char *merch_name, int amount)
{
    if (ioopm_hash_table_has_key(store->merch_table, string_elem(merch_name)))
    {
        ioopm_hash_table_insert(cart->items, string_elem(merch_name), int_elem(amount));
        return true;
    }

    return false;
}

/* int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);
  elem_t result = int_elem(-1);
  ioopm_add_merch(ht, "Stol", "Man kan sitta på den.", 200);
  ioopm_add_merch(ht, "Bord", "Man kan sitta på den.", 200);
  ioopm_add_merch(ht, "S", "Man kan sitta på den.", 200);
  ioopm_add_merch(ht, "P", "Man kan sitta på den.", 200);
  ioopm_add_merch(ht, "Hs", "Man kan sitta på den.", 200);
  ioopm_add_merch(ht, "Gs", "Man kan sitta på den.", 200);
  ioopm_list_merch(ht);
  ioopm_edit_merch(ht);
  ioopm_list_merch(ht);
  

  ioopm_hash_table_destroy(ht);
}

*/

///////////////
///////////////


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

/* int main(int argc, char *argv[])
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
} */