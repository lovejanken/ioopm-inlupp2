#include <CUnit/Basic.h>
#include "hash_table.h"
#include "list_iterator.h"
#include "linked_list.h"
#include "db.h"
#include "common.h"

static bool string_eq(elem_t key1, elem_t key2)
{
  return (strcmp(key1.s, key2.s) == 0);
}

static size_t string_hash(elem_t key)
{
  size_t result = 0;
  while (*key.s != '\0')
  {
    result = result * 31 + ((unsigned char) *key.s);
    key.s++;
  }
  return result;
}

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

void create_destroy_store()
{
  store_t *store = ioopm_create_store();
  CU_ASSERT(store != NULL);
  ioopm_destroy_store(store);

}

void add_merch()
{
  store_t *store = ioopm_create_store();

  CU_ASSERT_TRUE(ioopm_add_merch(store, "Banan", "Gul", 200));

  ioopm_destroy_store(store);
}

void add_and_remove_merch()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  CU_ASSERT_TRUE(ioopm_remove_merch(store, "Banan"));

  ioopm_destroy_store(store);
}

void remove_wrong_merch()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  CU_ASSERT_FALSE(ioopm_remove_merch(store, "Bänk"));

  ioopm_destroy_store(store);
}

void edit_existing_merch()
{
  store_t *store = ioopm_create_store();

  ioopm_add_merch(store, "Banan", "Gul", 200);
  ioopm_add_merch(store, "Apa", "Brun", 200);
  CU_ASSERT_TRUE(ioopm_edit_merch(store, "Banan", "Äpple", "Grönt", 200));
  CU_ASSERT_FALSE(ioopm_edit_merch(store, "Banan", "Äpple", "Grönt", 200));

  ioopm_destroy_store(store);
}

void replenish_merch()
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

  CU_ASSERT_FALSE(ioopm_get_stock_from_location(store, "Äpple", "A1", &result));

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
    (CU_add_test(my_test_suite, "Check if creating and destroying store works", create_destroy_store) == NULL) ||
    (CU_add_test(my_test_suite, "Check if adding merch to the store works", add_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if adding merch and removing it works", add_and_remove_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if removing non-existent merch works", remove_wrong_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if editing existant merch works and checking that the old merch is removed", edit_existing_merch) == NULL) ||
    (CU_add_test(my_test_suite, "Check if replenishing merch works as intended", replenish_merch) == NULL) ||
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