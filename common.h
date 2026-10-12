#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "db.h"

/**
 * @file common.h
 * @author Elis Filén, Alexander Thoresson
 * @date 01 Oct 2026
 * @brief contanis types common to most other files
 *
 * elem_t is a union that is used for our hash tables and linked lists.
 * The function types are used when creating a hash table.
 * 
 */


typedef struct merch merch_t;

typedef struct shelf shelf_t;

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
};

// macros for constructing elem_t values
#define int_elem(x) ((elem_t){.i = (x)})
#define bool_elem(x) ((elem_t){.b = (x)})
#define ptr_elem(x) ((elem_t){.p = (x)})
#define string_elem(x) ((elem_t){.s = (x)})
#define merch_elem(x) ((elem_t){.m = (x)})
#define shelf_elem(x) ((elem_t){.sh = (x)})

// ioopm_eq_function that compare elem_t of type bool, string, int, float have been implemented
typedef bool ioopm_eq_function(elem_t a, elem_t b);
// ioopm_hash_function that computates hash value of key, of type int, string have been implemented
typedef size_t ioopm_hash_function(elem_t key);