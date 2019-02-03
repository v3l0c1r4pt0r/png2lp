#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "binrd.h"

#include "binrd.c"

//typedef struct {uint8_t *stream; int bitcount; int *expected;} vector_t;
//
//const vector_t vectors[] = {
//  {{0xf5, 0x80}, 9, {}},
//  {{0xaa}, 8, {1, 0, 1, 0, 1, 0, 1, 0}},
//  {{0x80}, 1, {1}},
//};

static void test_get_bit_1(void **state)
{
  int actual[1];
  int i = 0;
  bitstream_t bs;
  uint8_t stream[] = {0x80};
  int bitcount = 1;
  int expected[] = {1};

  bs = init_bitstream(stream, bitcount);

  while(isnext(&bs))
  {
    actual[i++] = get_bit(&bs);
  }
  assert_memory_equal(expected, actual, bitcount);
}

static void test_get_bit_8(void **state)
{
  int actual[8];
  int i = 0;
  bitstream_t bs;
  uint8_t stream[] = {0xaa};
  int bitcount = 8;
  int expected[] = {1, 0, 1, 0, 1, 0, 1, 0};

  bs = init_bitstream(stream, bitcount);

  while(isnext(&bs))
  {
    actual[i++] = get_bit(&bs);
  }
  assert_memory_equal(expected, actual, bitcount);
}

static void test_get_bit_9(void **state)
{
  int actual[9];
  int i = 0;
  bitstream_t bs;
  uint8_t stream[] = {0xf5, 0x80};
  int bitcount = 9;
  int expected[] = {1, 1, 1, 1, 0, 1, 0, 1, 1};

  bs = init_bitstream(stream, bitcount);

  while(isnext(&bs))
  {
    actual[i++] = get_bit(&bs);
  }
  assert_memory_equal(expected, actual, bitcount);
}

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_get_bit_1),
    cmocka_unit_test(test_get_bit_8),
    cmocka_unit_test(test_get_bit_9),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
