#include <stdint.h>

#include "binrd.h"

bitstream_t init_bitstream(uint8_t *buf, int bitcount)
{
  bitstream_t bs;
  bs.stream = buf;
  bs.bitcount = bitcount;
  bs.bits = 0;
  bs.bits_loaded = 0;

  return bs;
}

void feed_reader(bitstream_t *bs)
{
  if (bs->bits_loaded != 0)
    return;

  bs->bits = *(bs->stream++);
  bs->bits_loaded = 8;
  bs->bitcount -= 8;

  if (bs->bitcount < 0)
  {
    bs->bits_loaded += bs->bitcount;
    bs->bitcount = 0;
  }
}

int get_bit(bitstream_t *bs)
{
  int result;

  if (bs->bits_loaded == 0)
    feed_reader(bs);

  result = (bs->bits & 0x80) == 0x80;
  bs->bits <<= 1;
  bs->bits_loaded--;

  return result;
}

int isnext(bitstream_t *bs)
{
  return bs->bitcount != 0;
}
