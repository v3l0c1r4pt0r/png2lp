#include <stdio.h>
#include <png.h>

#include "binrd.h"
#include "simplepng.h"
#include "printer.h"

int main(int argc, char **argv)
{
  int x = 0, y = 0;
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
  if (bmp.bit_depth != 1)
  {
    printf("unsupported color scheme: %d\n", bmp.bit_depth);
  }

  while (y < bmp.height)
  {
    bitstream_t reader = init_bitstream(bmp.rows[y], bmp.width);
    while(isnext(&reader))
    {
      printf("%s", get_bit(&reader)?"  ":"\u2588\u2588");
      x++;
    }
    printf("\n");
    y++;
  }

  printf("Finished\n");
  simple_png_free(&bmp);
  return 0;
}
