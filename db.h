#pragma once
#include <stddef.h>
#include "common.h"

/**
 * @file db.h
 * @author Love Jonasson Ankén, Alexander Thoresson
 * @date 12 oct 2026
 * @brief A simple database for a store.
 *
 * store_t is a struct representing the whole store that the database belongs to.
 * All of the functions take a store_t as one of their inputs.
 * The functions relate to all manners of the store, like the merch, the stock, and the cart.
 * The functions concerning the database are used by the menu.
 */

typedef struct store store_t;

/// @brief Allocates memory to a store_t and initialize it
/// @param 
/// @return an empty store
store_t *ioopm_create_store(void);

/// @brief Destroys a store and everything in it, and frees all memory concerning it
/// @param store the store to destroy
void ioopm_destroy_store(store_t *store);

/// @brief Returns the number of all carts that have been registered, including removed carts
/// @param store the store to get the size from
/// @return the number of all carts that have been registered, including removed carts
size_t ioopm_store_get_size(store_t *store);

/// @brief 
/// @param store 
/// @param key 
/// @param result 
/// @return 
bool ioopm_merch_lookup(store_t *store, elem_t key, elem_t *result);

bool ioopm_add_merch(store_t *store, char *name, char *desc, size_t price);

bool ioopm_list_merch(store_t *store);

bool ioopm_remove_merch(store_t *store, char *name);

bool ioopm_edit_merch(store_t *store, char *old_name, char *new_name, char *new_description, size_t new_price);

bool ioopm_show_stock(store_t *store, char *merch_name);

bool ioopm_replenish(store_t *store, char *location, char *merch_name, size_t amount);

bool ioopm_merch_is_empty(store_t *store);

bool ioopm_get_stock_from_location(store_t *store, char *merch_name, char *location, size_t *result);

void ioopm_create_cart(store_t *store);

bool ioopm_remove_cart(store_t *store, size_t cart_id);

bool ioopm_cart_lookup(store_t *store, elem_t key, elem_t *result);

bool ioopm_add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount);

bool ioopm_remove_from_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount);

bool ioopm_calculate_cost(store_t *store, size_t cart_id, size_t *result);

bool ioopm_checkout(store_t *store, size_t cart_id);

void ioopm_quit(store_t *store);