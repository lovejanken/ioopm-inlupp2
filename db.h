#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "common.h"
#include "hash_table.h"
#include "linked_list.h"

typedef struct store store_t;

store_t *ioopm_create_store(void);

void ioopm_destroy_store(store_t *store);

size_t store_get_size(store_t *store);

bool ioopm_merch_lookup(store_t *store, elem_t key, elem_t *result);

bool ioopm_cart_lookup(store_t *store, elem_t key, elem_t *result);

bool ioopm_add_merch(store_t *store, char *name, char *desc, size_t price);

bool ioopm_list_merch(store_t *store);

bool ioopm_remove_merch(store_t *store, char *name);

bool ioopm_edit_merch(store_t *store, char *old_name, char *new_name, char *new_description, size_t new_price);

bool ioopm_get_stock_from_location(store_t *store, char *merch_name, char *location, size_t *result);

bool ioopm_show_stock(store_t *store, char *merch_name);

bool ioopm_replenish(store_t *store, char *location, char *merch_name, size_t amount);

bool ioopm_merch_exists(store_t *store, char *merch_name);

bool ioopm_merch_is_empty(store_t *store);

void ioopm_create_cart(store_t *store);

bool ioopm_remove_cart(store_t *store, size_t cart_id);

bool ioopm_add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount);

bool ioopm_remove_from_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount);

bool ioopm_calculate_cost(store_t *store, size_t cart_id, size_t *result);

bool ioopm_checkout(store_t *store, size_t cart_id);

void ioopm_quit(store_t *store);