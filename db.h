#pragma once
#include <stddef.h>
#include "db.c"
#include <stdbool.h>


void ioopm_add_merch(ioopm_hash_table_t *ht, char *name, char *desc, int price);

void ioopm_list_merch(ioopm_hash_table_t *ht);

void ioopm_remove_merch(ioopm_hash_table_t *ht, merch_t *item);

void ioopm_edit_merch(ioopm_hash_table_t *ht, merch_t *item);

void ioopm_show_stock(ioopm_hash_table_t *ht, merch_t *item);

void ioopm_replenish(ioopm_hash_table_t *ht, merch_t *item, char *loc, int amount);

cart_t ioopm_create_cart();

void ioopm_remove_cart(cart_t *cart);

void ioopm_add_to_cart(cart_t *cart, merch_t *item, int amount);

void ioopm_remove_from_cart(cart_t *cart, merch_t *item, int amount);

int ioopm_calculate_cost(cart_t *cart);

void ioopm_checkout(cart_t *cart, ioopm_hash_table_t *ht);

void ioopm_quit();