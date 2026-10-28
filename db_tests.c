#include <CUnit/Basic.h>
#include "hash_table.h"
#include "list_iterator.h"
#include "linked_list.h"
#include "db.h"
#include "common.h"
#include <stdlib.h>

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

void test_create_destroy_store()
{
  store_t *store = ioopm_create_store();
  CU_ASSERT(store != NULL);
  ioopm_destroy_store(store);
}

void test_add_merch()
{
  store_t *store = ioopm_create_store();

  CU_ASSERT_TRUE(ioopm_add_merch(store, "Banan", "Gul", 200));

  ioopm_destroy_store(store);
}

void test_add_empty_merch()
{
  store_t *store = ioopm_create_store();

  CU_ASSERT_FALSE(ioopm_add_merch(store, "", "Gul", 200));

  ioopm_destroy_store(store);
}

void test_remove_merch()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  CU_ASSERT_TRUE(ioopm_remove_merch(store, "Banan"));

  ioopm_destroy_store(store);
}

void test_remove_non_existent_merch()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  CU_ASSERT_FALSE(ioopm_remove_merch(store, "Bänk"));

  ioopm_destroy_store(store);
}

void test_edit_merch()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  ioopm_add_merch(store, "Apa", "Brun", 200);
  CU_ASSERT_TRUE(ioopm_edit_merch(store, "Banan", "Äpple", "Grönt", 200));
  CU_ASSERT_FALSE(ioopm_edit_merch(store, "Banan", "Äpple", "Grönt", 200));

  ioopm_destroy_store(store);
}

void test_replenish_merch()
{
  int result;
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  ioopm_add_merch(store, "Apa", "Brun", 200);
  ioopm_replenish(store, "A12", "Banan", 10);


  ioopm_get_stock_from_location(store, "Banan", "A12", &result);

  CU_ASSERT_EQUAL(result, 10);

  ioopm_edit_merch(store, "Banan", "Äpple", "Grön", 200);

  ioopm_get_stock_from_location(store, "Äpple", "A12", &result);

  CU_ASSERT_EQUAL(result, 10);

  CU_ASSERT_FALSE(ioopm_get_stock_from_location(store, "Äpple", "A15", &result));

  ioopm_destroy_store(store);
}

void test_create_cart()
{
  store_t *store = ioopm_create_store();
  elem_t local;

  ioopm_add_merch(store, "Banan", "Gul", 200);
  ioopm_create_cart(store);
  CU_ASSERT_TRUE(ioopm_cart_lookup(store, int_elem(1), &local));
  ioopm_destroy_store(store);
}

void test_add_to_cart()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  ioopm_replenish(store, "A10", "Banan", 100);
  ioopm_create_cart(store);
  CU_ASSERT_TRUE(ioopm_add_to_cart(store, 1, "Banan", 10));

  CU_ASSERT_FALSE(ioopm_add_to_cart(store, 1, "Banan", 91));

  CU_ASSERT_TRUE(ioopm_add_to_cart(store, 1, "Banan", 90));

  ioopm_destroy_store(store);
}

void test_remove_from_cart()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  ioopm_replenish(store, "A10", "Banan", 100);
  ioopm_create_cart(store);
  CU_ASSERT_TRUE(ioopm_add_to_cart(store, 1, "Banan", 10));

  CU_ASSERT_TRUE(ioopm_remove_from_cart(store, 1, "Banan", 5));

  elem_t result;
  elem_t cart;

  ioopm_cart_lookup(store, int_elem(1), &cart);
  ioopm_hash_table_lookup(cart.c->items, string_elem("Banan"), &result);
  CU_ASSERT_EQUAL(result.i, 5);

  ioopm_destroy_store(store);
}

void test_calculate_cost()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 1);
  ioopm_add_merch(store, "Ägg", "Vitt", 1);
  ioopm_add_merch(store, "Äpple", "Grönt", 1);
  ioopm_replenish(store, "A10", "Banan", 5);
  ioopm_replenish(store, "A10", "Banan", 5);
  ioopm_replenish(store, "B20", "Ägg", 5);
  ioopm_replenish(store, "B25", "Ägg", 5);
  ioopm_replenish(store, "C10", "Äpple", 100);
  ioopm_create_cart(store);
  ioopm_add_to_cart(store, 1, "Banan", 10);
  ioopm_add_to_cart(store, 1, "Ägg", 10);
  ioopm_add_to_cart(store, 1, "Äpple", 10);

  int cost = 0;
  CU_ASSERT_TRUE(ioopm_calculate_cost(store, 1, &cost));
  printf("%d\n", cost);
  CU_ASSERT_EQUAL(cost, 30);
  
  ioopm_destroy_store(store);
}

void test_checkout()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 1);
  ioopm_add_merch(store, "Ägg", "Vitt", 1);
  ioopm_add_merch(store, "Äpple", "Grönt", 1);
  ioopm_replenish(store, "A10", "Banan", 5);
  ioopm_replenish(store, "A10", "Banan", 5);
  ioopm_replenish(store, "B20", "Ägg", 5);
  ioopm_replenish(store, "B25", "Ägg", 5);
  ioopm_replenish(store, "C10", "Äpple", 100);
  ioopm_create_cart(store);
  ioopm_add_to_cart(store, 1, "Ägg", 8);
  ioopm_add_to_cart(store, 1, "Banan", 1);  
  ioopm_add_to_cart(store, 1, "Äpple", 10);

  int cost1 = 0;
  ioopm_calculate_cost(store, 1, &cost1);
  CU_ASSERT_EQUAL(cost1, 19);


  ioopm_checkout(store, 1);

  elem_t Ägg;
  ioopm_merch_lookup(store, string_elem("Ägg"), &Ägg);

  ioopm_show_stock(store, "Ägg");

  ioopm_create_cart(store);
  ioopm_add_to_cart(store, 2, "Banan", 1);
  CU_ASSERT_FALSE(ioopm_add_to_cart(store, 2, "Ägg", 5));
  ioopm_add_to_cart(store, 2, "Äpple", 10);
  int cost2 = 0;
  ioopm_calculate_cost(store, 2, &cost2);
  CU_ASSERT_EQUAL(cost2, 11);


  ioopm_destroy_store(store);
}


int main() {
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
  }

  if (
    (CU_add_test(my_test_suite, "Check if creating and destroying store works", test_create_destroy_store) == NULL) ||
    (CU_add_test(my_test_suite, "Check if adding merch to the store works", test_add_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if adding merch with no name to the store works", test_add_empty_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if removing merch from the store works", test_remove_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if removing non-existent merch works", test_remove_non_existent_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if editing existant merch works and checking that the old merch is removed", test_edit_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if replenishing merch works as intended", test_replenish_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if creating a cart works", test_create_cart) == NULL) ||
    (CU_add_test(my_test_suite, "Check if adding to a cart works", test_add_to_cart) == NULL) ||
    (CU_add_test(my_test_suite, "Check if removing from a cart works", test_remove_from_cart) == NULL) ||
    (CU_add_test(my_test_suite, "Check if calculating cost works", test_calculate_cost) == NULL) ||
    (CU_add_test(my_test_suite, "Check if checkout works", test_checkout) == NULL) ||
0
  )
  {
      CU_cleanup_registry();
      return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_NORMAL);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}