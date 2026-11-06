# Inlupp 2: Webstore backend

# Build and test

make test        build and run test files using CUnit
make memtest     run the tests and check for memory leaks using Valgrind
make coverage    run the tests and check coverage using gcov
make clean       clear the files created when using make

Requires gcc, make, CUnit and Valgrind.

# Programs

    make run

Runs the menu, allowing use of all implemented functions.

# Design decisions

The hash table and linked list implementation comes from Alexander's assignment 1 (that includes `common.h`). The `utils.c` comes from Love's bootstrap labs.

The database uses the struct `store_t` to store data. `store_t` contains `merch_table`, `stock_by_shelf`, `carts` and `number_of_carts`.

`merch_table` is defined as a hash table that uses the name of the merch as a key and the merch item (`merch_t`) as value. If a merch is edited, the old merch gets removed and the new edited one is inserted into the hash table. This way the updated merch gets a new key, incase the name is changed.

`stock_by_shelf` is a hash table that uses the shelf name as key and the merch associated with the shelf as value. `stock_by_shelf` is used to easily lookup if a shelf is already occupied, without having to iterate through each merch's locations.

`carts` is a hash table that uses the cart number as key and a `cart_t` as value.

`merch_t` is the struct used to define each merch. It contains the name of the merch as a string, the description of the merch as a string, the price of the item as a `size_t` (as it cannot be negative) and the locations the merch is stored at as a `ioopm_list_t`. The nodes in the list are of the type `shelf_t`.

`shelf_t` contains the name of the shelf as a string and the quantity of the specific merch stored at the location as a `size_t`.

`cart_t` consists of a `ioopm_hash_table_t` that stores each merch added to the cart. The hash table uses the merch name as key and the amount as the value. `cart_t` also has the id of the cart as a `size_t`, the id of a cart is defined when it's created using `ioopm_cart_create`. When a cart is created `number_of_carts` is incremented by one and the new cart gets the id of the updated `number_of_carts`. Removing a cart does not update `number_of_carts`.

Hash tables are used to keep the database time efficient. In most cases we are accessing specific addresses, which makes using lists inefficient, having to iterate through the list to find the correct address. The exception for this is for the `merch_t` locations, where we are interested in going through all locations instead of finding a specific one. Therefore a `ioopm_list_t` is used in this instance.

# Test coverage

File 'db.c'
Lines executed:88.89% of 252
Branches executed:97.67% of 86
Taken at least once:73.26% of 86
Calls executed:93.10% of 116
Creating 'db.c.gcov'

Lines executed:88.89% of 252

# Shortcuts and known issues

FIXME: `ioopm_list_merch` does not currently print the merch in alphabetical order. This can be fixed to improve useability of the program.