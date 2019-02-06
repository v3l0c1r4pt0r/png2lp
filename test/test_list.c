#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <setjmp.h>
#include <cmocka.h>

#include "list.h"

#include "list.c"

static void test_remove_last(void **state)
{
  list_t *list;
  int actual;
  int i = 0;
  int bitcount = 1;
  int expected = -1;

  list = list_alloc();

  assert_int_equal(expected, list_remove(list));

  list_free(list);
}

static void test_remove_theonly(void **state)
{
  list_t *list;
  int actual;
  int i = 0;
  int bitcount = 1;
  int expected = 0;

  list = list_alloc();
  list_append(list, (void*)0x1337);

  assert_int_equal(expected, list_remove(list));

  list_free(list);
}

static void test_iterate(void **state)
{
  list_t *list;
  list_t *actual;
  int i = 0;
  int bitcount = 1;
  void *expected[5] = {0xdeadbeef, 0x123, 1, 0x1337, NULL};

  list = list_alloc();
  list_append(list, (void*)0x1337);
  list_append(list, (void*)1);
  list_append(list, (void*)0x123);
  list_append(list, (void*)0xdeadbeef);
  actual = list;

  while (list_has_next(actual))
  {
    actual = list_next(actual);
    assert_int_equal(expected[i++], actual->value);
  }

  list_free(list);
}

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_remove_last),
    cmocka_unit_test(test_remove_theonly),
    cmocka_unit_test(test_iterate),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
