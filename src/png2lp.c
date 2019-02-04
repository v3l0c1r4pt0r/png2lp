#include <stdio.h>

#include "binrd.h"
#include "simplepng.h"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Usage: %s [file-name]\n", argv[0]);
    return 1;
  }
  char *fn = argv[1];
  bitmap_t bmp = simple_png_read(fn);
  if (bmp.width == 0 || bmp.height == 0)
  {
    printf("simple_read_png failed\n");
    return 1;
  }

  bitstream_t reader = init_bitstream(bmp.rows[0], bmp.width);
  while(isnext(&reader))
  {
    printf("%d", get_bit(&reader));
  }

  printf("Finished\n");
  return 0;
}
