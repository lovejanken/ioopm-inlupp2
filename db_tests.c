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

void test1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(-1);
  ioopm_add_merch(ht, "Stol", "Man kan sitta på den.", 200);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("Stol"), &result));

  ioopm_remove_merch(ht, "Stol");

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("Stol"), &result));

  ioopm_hash_table_destroy(ht);
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
    (CU_add_test(my_test_suite, "Check if add merch and remove merch work", test1) == NULL) ||
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