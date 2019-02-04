#ifndef SIMPLEPNG_H
#define SIMPLEPNG_H
#include <png.h>

typedef struct {
  int width, height;
  png_byte color_type;
  png_byte bit_depth;
  png_bytep *rows;
  png_uint_32 row_count; /**< num of bytes per row */
} bitmap_t;

bitmap_t simple_png_read(char* file_name);

#endif // SIMPLEPNG_H
