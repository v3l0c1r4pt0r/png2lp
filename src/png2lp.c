#include <stdio.h>
#include <string.h>
#include <png.h>

#include "binrd.h"
#include "simplepng.h"
#include "logger.h"
#include "printer.h"

int main(int argc, char **argv)
{
  set_numeric_log_level(LEVEL_DEBUG);

  char *printer_name = "VT100";
  char *page_name = "80x25 terminal (no overflow)";

  int x = 0, y = 0;
  if (argc < 2)
  {
    printf("Usage: %s [file-name]\n", argv[0]);
    return 1;
  }

  /* init printers */
  register_sinks();

  if (strcmp(argv[1], "-l") == 0)
  {
    /* list printers */
    int i;
    char **printers = printer_get_sinks();

    for (i = 0; printers[i] != NULL; i++)
    {
      printf("%d: %s\n", i, printers[i]);
    }

    return 0;
  }
  else if (strcmp(argv[1], "-L") == 0 && argc > 2)
  {
    /* list pages */
    int i;
    sink_t sink = printer_get_sink(argv[2]);
    char **pages = printer_get_sink_pages(&sink);

    for (i = 0; pages[i] != NULL; i++)
    {
      printf("%d: %s\n", i, pages[i]);
    }

    return 0;
  }

  /* convert png from param 1 */
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
    return 1;
  }

  /* find requested sink */
  sink_t sink = printer_get_sink(printer_name);
  if (sink.printer == NULL)
  {
    ERROR("sink not found");
    return 1;
  }

  /* select page */
  page_t *page = printer_get_sink_page_by_name(&sink, page_name);
  if (page == NULL)
  {
    ERROR("page '%s' cannot be found for '%s'", page_name, printer_name);
    return 1;
  }
  printer_set_sink_page(&sink, page);

  /* select size */
  printer_set_size(&sink, bmp.width, bmp.height);

  while (y < bmp.height)
  {
    bitstream_t reader = init_bitstream(bmp.rows[y], bmp.width);
    x = 0;
    while(isnext(&reader))
    {
      int bit = get_bit(&reader);
      printer_feed_bit(&sink, x, y, bit);
      x++;
    }
    y++;
  }

  printf("Finished\n");
  printer_destroy(&sink);
  simple_png_free(&bmp);
  return 0;
}
