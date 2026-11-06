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

/// @brief Lookup merch key in store, and its value is stored in result
/// @param store the store to lookup
/// @param key corresponds to the merch to lookup
/// @param result where value of lookup is stored
/// @return true if lookup succeeds, otherwise false
bool ioopm_merch_lookup(store_t *store, elem_t key, elem_t *result);

/// @brief Adds a new merch to the store
/// @param store the store to add the merch to
/// @param name name of new merch
/// @param desc description of new merch
/// @param price price of new merch
/// @return true if it succeeds, otherwise false
bool ioopm_add_merch(store_t *store, char *name, char *desc, size_t price);

/// @brief Lists all merch in the store
/// @param store the store which list is displayed
/// @return true if it succeeds, otherwise false
bool ioopm_list_merch(store_t *store);

/// @brief Removes merch with name name from the store
/// @param store the store to remove the merch from
/// @param name the merch to be removed
/// @return true if it succeeds, otherwise false
bool ioopm_remove_merch(store_t *store, char *name);

/// @brief Edits the merch with name old_name in the store
/// @param store the store containing the merch to edit
/// @param old_name name of merch to edit
/// @param new_name new name of the merch
/// @param new_description new description of the merch
/// @param new_price new price of the merch
/// @return true if it succeeds, otherwise false
bool ioopm_edit_merch(store_t *store, char *old_name, char *new_name, char *new_description, size_t new_price);

/// @brief Shows all the stock and where it is stored, for a chosen merch
/// @param store the store to use
/// @param merch_name the name of the merch to show the stock of
/// @return true if it succeeds, otherwise false
bool ioopm_show_stock(store_t *store, char *merch_name);

/// @brief Replenish the stock of a merch named merch_name with amount in location
/// @param store the store to use
/// @param location where the stock is stored
/// @param merch_name name of merch to replenish
/// @param amount the amount to replenish
/// @return true if it succeeds, otherwise false
bool ioopm_replenish(store_t *store, char *merch_name, char *location, size_t amount);

/// @brief Checks if there are any merch in the store
/// @param store the store to use
/// @return true if the store does not contain any merch, otherwise false
bool ioopm_merch_is_empty(store_t *store);

/// @brief Allocates memory to a cart_t and initialize it
/// @param store the store to use
void ioopm_create_cart(store_t *store);

/// @brief Removes a cart and everything in it, and frees all memory concerning it
/// @param store the store to use
/// @param cart_id id of the cart to remove
/// @return true if it succeeds, otherwise false
bool ioopm_remove_cart(store_t *store, size_t cart_id);

/// @brief Lookup cart key in store, and its value is stored in result
/// @param store the store to use
/// @param key corresponds to the cart to lookup
/// @param result where value of lookup is stored
/// @return true if it succeeds, otherwise false
bool ioopm_cart_lookup(store_t *store, elem_t key, elem_t *result);

/// @brief Adds amount of merch named merch_name to cart with id cart_id
/// @param store the store to use
/// @param cart_id id of the cart to use
/// @param merch_name name of the merch to use
/// @param amount to add to the cart
/// @return true if it succeeds, otherwise false
bool ioopm_add_to_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount);

/// @brief Removes amount of merch named merch_name from the cart with id cart_id
/// @param store the store to use
/// @param cart_id id of the cart to use
/// @param merch_name name of the merch to use
/// @param amount to remove from the cart
/// @return true if it succeeds, otherwise false
bool ioopm_remove_from_cart(store_t *store, size_t cart_id, char *merch_name, size_t amount);

/// @brief Calculates the cost of everything added to the cart with id cart_id
/// @param store the store to use
/// @param cart_id id of the cart to use
/// @param result recieves the total cost of everything in the cart
/// @return true if it succeeds, otherwise false
bool ioopm_calculate_cost(store_t *store, size_t cart_id, size_t *result);

/// @brief Checks out cart with id cart_id from the store, removing the stock inn the cart from the store
/// @param store the store to use
/// @param cart_id id of the cart to use
/// @return true if it succeeds, otherwise false
bool ioopm_checkout(store_t *store, size_t cart_id);

/// @brief Destroys the store
/// @param store the store to use
void ioopm_quit(store_t *store);