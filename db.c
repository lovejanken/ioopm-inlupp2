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

struct store {
    ioopm_hash_table_t *merch_table;
    ioopm_hash_table_t *stock_by_shelf;
    ioopm_hash_table_t *carts;
    size_t number_of_carts;
};


store_t *ioopm_create_store(void)
{
    store_t *store = malloc(sizeof(store_t));
    store->carts = ioopm_hash_table_create(ioopm_hash_int, ioopm_int_comp);
    store->merch_table = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);
    store->stock_by_shelf = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);
    store->number_of_carts = 0;

    return store;
}

static void destroy_merch_data(merch_t merch)
{
    if (merch.locs != NULL) {
        for (size_t i = 0; i < ioopm_list_size(merch.locs); i++) {
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
   ioopm_hash_table_destroy_cart(store->carts);
}

bool ioopm_merch_lookup(store_t *store, elem_t key, elem_t *result)
{
    return ioopm_hash_table_lookup(store->merch_table, key, result);
}

bool ioopm_cart_lookup(store_t *store, elem_t key, elem_t *result)
{
    return ioopm_hash_table_lookup(store->carts, key, result);
}

bool ioopm_remove_cart(store_t *store, size_t cart_id)
{
    elem_t local;
    if (ioopm_hash_table_remove(store->carts, int_elem(cart_id), &local))
    {
        destroy_cart(local.c);
        return true;
    }

    return false;
}

size_t store_get_size(store_t *store)
{
    return store->number_of_carts;
}


void ioopm_destroy_store(store_t *store)
{
    destroy_all_carts(store);
    ioopm_hash_table_destroy(store->stock_by_shelf);
    destroy_all_merch(store);
    ioopm_hash_table_destroy(store->merch_table);
    free(store);
}

bool ioopm_add_merch(store_t *store, char *name, char *desc, size_t price)
{
    if (strlen(name) < 1)
    {
        return false;
    }

    elem_t local;
    if (ioopm_merch_lookup(store, string_elem(name), &local))
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

bool ioopm_list_merch(store_t *store)
{
    if (ioopm_hash_table_is_empty(store->merch_table))
    {
        return false;
    }
    
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->merch_table);

    size_t counter = 0;

    while (!ioopm_hash_table_iterator_at_end(it))
    {
        if (counter == 20)
        {
            char *answer = ask_question_user_input("[Y/N] Do you want to continue printing merch?", always_true);
            if (check_no(answer))
            {
                break;
            }

            else
            {
                counter = 0;
            }
        }

        merch_t current_item = ioopm_hash_table_iterator_current_value(it).m;
        printf("Name: %s\nDescription: %s\nPrice: %d\n\n", current_item.name, current_item.description, current_item.price);
        ioopm_hash_table_iterator_advance(it);
        counter++;
    }
    ioopm_hash_table_iterator_destroy(it);
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

    for (size_t i = 0; i < ioopm_list_size(merch.m.locs); i++)
    {
        shelf_t shelf = ioopm_list_get(merch.m.locs, i).sh;
        ioopm_hash_table_remove(store->stock_by_shelf, string_elem(shelf.shelf), &local);
    }

    destroy_merch_data(merch.m);
    return true;
}

bool ioopm_edit_merch(store_t *store, char *old_name, char *new_name, char *new_description, size_t new_price)
{
    elem_t local;
    if (ioopm_merch_lookup(store, string_elem(new_name), &local) && strcmp(old_name, new_name) != 0)
    {
        return false;
    }

    elem_t  old_merch;
    
    ioopm_merch_lookup(store, string_elem(old_name), &old_merch);

    merch_t edited_merch = {
        .name = strdup(new_name),
        .description = strdup(new_description),
        .price = new_price,
        .locs = old_merch.m.locs
    };

    if (ioopm_hash_table_remove(store->merch_table, string_elem(old_name), &local))
    {
        ioopm_hash_table_insert(store->merch_table, string_elem(new_name), merch_elem(edited_merch));

        for (size_t i = 0; i < ioopm_list_size(edited_merch.locs); i++)
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

bool ioopm_merch_exists(store_t *store, char *merch_name)
{
    return ioopm_hash_table_has_key(store->merch_table, string_elem(merch_name));
}

bool ioopm_merch_is_empty(store_t *store)
{
    return ioopm_hash_table_is_empty(store->merch_table);
}

bool ioopm_get_stock_from_location(store_t *store, char *merch_name, char *location, size_t *result)
{
    elem_t merch = int_elem(-1);
    if (!ioopm_merch_lookup(store, string_elem(merch_name), &merch))
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

bool ioopm_show_stock(store_t *store, char *merch_name)
{
    elem_t merch = int_elem(-1);
    if (ioopm_merch_lookup(store, string_elem(merch_name), &merch))
    {
        ioopm_list_t *item_list = merch.m.locs;
        if (ioopm_list_is_empty(item_list))
        {
            printf("Merch doesn't have any stock.\n");
            return true;
        }
        ioopm_list_iterator_t *it = ioopm_list_iterator_create(item_list);
        while (!ioopm_list_iterator_at_end(it))
        {
            elem_t shelf = ioopm_list_iterator_current(it);
            printf("Shelf: %s, Quantity: %d\n", shelf.sh.shelf, shelf.sh.quantity);
            ioopm_list_iterator_advance(it);
        }

        ioopm_list_iterator_destroy(it);
        return true;
    }

    else return false;
}

bool ioopm_replenish(store_t *store, char *location, char *merch_name, size_t amount)
{
    if (amount < 1) return false;
    size_t counter = 0;

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
                ioopm_list_remove(list, counter);
                ioopm_list_insert(list, counter, shelf_elem(shelf));
                break;
            }
            counter++;
            ioopm_list_iterator_advance(it);
        }

        ioopm_list_iterator_destroy(it);

        return true;
    }

    else
    {
        if (ioopm_merch_lookup(store, string_elem(merch_name), &merch))
        {

            shelf_t shelf = {
                .quantity = amount,
                .shelf = strdup(location)
            };

            ioopm_list_append(merch.m.locs, shelf_elem(shelf));
            ioopm_hash_table_insert(store->stock_by_shelf, string_elem(shelf.shelf), merch);
            return true;
        }

        else
        {
            return false;
        }
    }
}

void ioopm_create_cart(store_t *store)
{
    cart_t *new_cart = malloc(sizeof(cart_t));
    new_cart->items = ioopm_hash_table_create(ioopm_hash_string, ioopm_string_comp);
    store->number_of_carts++;
    new_cart->id = store->number_of_carts;
    ioopm_hash_table_insert(store->carts, int_elem(store->number_of_carts), cart_elem(new_cart));

}

static size_t sum_stock_of_merch(store_t *store, char *merch_name)
{
    elem_t merch = int_elem(-1);
    ioopm_merch_lookup(store, string_elem(merch_name), &merch);

    ioopm_list_t *list = merch.m.locs;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    size_t total_stock = 0;

    while (!ioopm_list_iterator_at_end(it))
    {
        total_stock += ioopm_list_iterator_current(it).sh.quantity;
        ioopm_list_iterator_advance(it);
    }

    ioopm_list_iterator_destroy(it);

    return total_stock;
}

static size_t sum_merch_stock_of_carts(store_t *store, char *merch_name)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(store->carts);
    cart_t *current_cart;
    size_t total_sum = 0;
    elem_t current_amount = int_elem(0);

    while (!ioopm_hash_table_iterator_at_end(it))
    {
        current_cart = ioopm_hash_table_iterator_current_value(it).c;
        if (ioopm_hash_table_lookup(current_cart->items, string_elem(merch_name), &current_amount))
        {
            total_sum += current_amount.i;
        }
        ioopm_hash_table_iterator_advance(it);
        
    }
    ioopm_hash_table_iterator_destroy(it);

    return total_sum;
}

bool ioopm_add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount)
{
    if (ioopm_hash_table_has_key(store->merch_table, string_elem(merch_name)))
    {
        elem_t elem_cart;
        if (!ioopm_cart_lookup(store, int_elem(cart_id), &elem_cart))
        {
            return false;
        }
        size_t cart_stock = sum_merch_stock_of_carts(store, merch_name);
        if ((sum_stock_of_merch(store, merch_name) - cart_stock) < amount)
        {
            return false;
        }
        elem_t existing_amount = int_elem(0);
        if (ioopm_hash_table_lookup(elem_cart.c->items, string_elem(merch_name), &existing_amount))
        {
            ioopm_hash_table_insert(elem_cart.c->items, string_elem(merch_name), int_elem(existing_amount.i + amount));
        }

        else
        {
            ioopm_hash_table_insert(elem_cart.c->items, string_elem(merch_name), int_elem(amount));
        }
        return true;
    }

    return false;
}

bool ioopm_remove_from_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount)
{
    elem_t elem_amount = int_elem(0);
    elem_t cart;
    ioopm_cart_lookup(store, int_elem(cart_id), &cart);

    if (ioopm_hash_table_remove(cart.c->items, string_elem(merch_name), &elem_amount))
    {
        size_t current_amount = elem_amount.i;
        if (current_amount > amount)
        {
            ioopm_hash_table_insert(cart.c->items, string_elem(merch_name), int_elem(current_amount - amount));
        }
        return true;
    }

    return false;
}

bool ioopm_calculate_cost(store_t *store, size_t cart_id, size_t *result)
{
    elem_t cart;
    if (!ioopm_cart_lookup(store, int_elem(cart_id), &cart))
    {
        return false;
    }

    size_t total_price = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart.c->items);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        elem_t merch_name = ioopm_hash_table_iterator_current_key(it);
        elem_t merch_amount = ioopm_hash_table_iterator_current_value(it);
        elem_t current_merch;
        ioopm_merch_lookup(store, merch_name, &current_merch);
        size_t current_price = current_merch.m.price;
        total_price = total_price + (merch_amount.i * current_price);
        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_hash_table_iterator_destroy(it);
    *result = total_price;
    return true;
}

static void remove_stock_from_available(store_t *store, char *merch_name, size_t amount)
{
    elem_t current_merch = int_elem(-1);
    elem_t local = int_elem(-1);
    size_t quantity = 0;
    ioopm_merch_lookup(store, string_elem(merch_name), &current_merch);
    ioopm_list_t *locations = current_merch.m.locs;
    size_t counter = 0;
    size_t local_amount = amount;
    size_t list_size = ioopm_list_size(locations);

    while (local_amount > 0 && counter < list_size)
    {
        shelf_t shelf = ioopm_list_get(locations, counter).sh;
        //printf("SHELF NAME: %s\n", shelf.shelf);
        quantity = shelf.quantity;

        if (local_amount >= quantity)
        {
            local_amount = local_amount - quantity;
            //printf("LOCAL AMOUNT: %d\n", local_amount);
            ioopm_hash_table_remove(store->stock_by_shelf, string_elem(shelf.shelf), &local);
            ioopm_list_remove(locations, counter);
            free(shelf.shelf);
        }

        else
        {
            shelf.quantity = shelf.quantity - local_amount;
            //printf("SHELF QUANTITY: %d\n", shelf.quantity);
            local_amount = 0;
            ioopm_list_append(locations, shelf_elem(shelf));
            ioopm_list_remove(locations, counter);
            counter++;
            
        }
    }
}

bool ioopm_checkout(store_t *store, size_t cart_id)
{
    elem_t cart;
    if (!ioopm_cart_lookup(store, int_elem(cart_id), &cart))
    {
        return false;
    }

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart.c->items);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        char *merch_name = ioopm_hash_table_iterator_current_key(it).s;
        size_t amount = ioopm_hash_table_iterator_current_value(it).i;
        
        remove_stock_from_available(store, merch_name, amount);

        ioopm_hash_table_iterator_advance(it);
    }

    ioopm_remove_cart(store, cart.c->id);
    ioopm_hash_table_iterator_destroy(it);
    return true;

}

void quit(store_t *store)
{
    ioopm_destroy_store(store);
    return;
}