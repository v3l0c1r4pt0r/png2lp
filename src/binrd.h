#ifndef BINRD_H
#define BINRD_H
#include <stdint.h>

typedef struct {
  uint8_t *stream;
  int bitcount;
  uint8_t bits;
  int bits_loaded;
} bitstream_t;

bitstream_t init_bitstream(uint8_t *buf, int bitcount);
int get_bit(bitstream_t *bs);
int isnext(bitstream_t *bs);

#endif // BINRD_H
