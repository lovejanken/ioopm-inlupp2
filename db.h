#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "common.h"
#include "hash_table.h"
#include "linked_list.h"

/*
//Lista av alla carts som existerar
typedef ioopm_list_t cart_list_t;
*/

typedef struct store store_t;

store_t *ioopm_create_store(void);

void ioopm_destroy_store(store_t *store);

bool ioopm_add_merch(store_t *store, char *name, char *desc, int price);

void ioopm_list_merch(store_t *store);

bool ioopm_remove_merch(store_t *store, char *name);

bool ioopm_edit_merch(store_t *store, char *old_name, char *new_name, char *new_description, int new_price);

bool ioopm_get_stock_from_location(store_t *store, char *merch_name, char *location, int *result);

bool ioopm_show_stock(store_t *store, merch_t item);

bool ioopm_replenish(store_t *store, char *location, char *merch_name, int amount);

cart_t *ioopm_create_cart(store_t *store);

bool ioopm_remove_cart(store_t *store, cart_t *cart);

bool ioopm_add_to_cart(store_t *store, cart_t *cart, char *merch_name, int amount);

bool ioopm_remove_from_cart(cart_t *cart, char *merch_name, int amount);

int ioopm_calculate_cost(store_t *store, cart_t *cart);

void ioopm_checkout(store_t *store, cart_t *cart);

void ioopm_quit();