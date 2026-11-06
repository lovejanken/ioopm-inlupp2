#pragma once
#include <stddef.h>

/**
 * @file common.h
 * @author Elis Filén, Love Jonasson Ankén, Alexander Thoresson
 * @date 12 Oct 2026
 * @brief contanis several structs, a union with macros, and two typedefined functions.
 *
 * The structs merch_t, shelf_t, and cart_t are used in our database.
 * The structs are placed here in order to use them in the union elem_t.
 * elem_t is a union that is used for our hash tables and linked lists.
 * The function types are used when creating a hash table.
 */

typedef struct merch merch_t;

typedef struct list ioopm_list_t;

typedef struct shelf shelf_t;

typedef struct cart cart_t;

typedef struct hash_table ioopm_hash_table_t;

struct merch
{
    char *name;         // name of the merch
    char *description;  // description of the merch
    size_t price;       // price of the merch
    ioopm_list_t *locs; // list of shelf_t, all locations the merch is stored in
};

struct shelf
{
    char *shelf;     // name of the shelf
    size_t quantity; // the amount of stock stored on the shelf
};

struct cart
{
    ioopm_hash_table_t *items; // hash table containing all the merch in the cart
    size_t id;                 // id of the cart
};

typedef union elem elem_t;

union elem
{
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
    char *s;
    merch_t m;
    shelf_t sh;
    cart_t *c;
};

// macros for constructing elem_t values
#define int_elem(x) ((elem_t){.i = (x)})
#define bool_elem(x) ((elem_t){.b = (x)})
#define ptr_elem(x) ((elem_t){.p = (x)})
#define string_elem(x) ((elem_t){.s = (x)})
#define merch_elem(x) ((elem_t){.m = (x)})
#define shelf_elem(x) ((elem_t){.sh = (x)})
#define cart_elem(x) ((elem_t){.c = (x)})

// ioopm_eq_function that compare elem_t of type bool, string, int, float have been implemented
typedef bool ioopm_eq_function(elem_t a, elem_t b);
// ioopm_hash_function that computates hash value of key, of type int, string have been implemented
typedef size_t ioopm_hash_function(elem_t key);