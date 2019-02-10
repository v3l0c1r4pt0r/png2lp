#include <stdio.h>
#include <string.h>
#include <png.h>
#include <getopt.h>
#include <limits.h>

#include "binrd.h"
#include "simplepng.h"
#include "logger.h"
#include "printer.h"

#define LOG_OPTION (( CHAR_MAX + 1 ))

enum command
{
  CMD_PRINT = 0,
  CMD_LIST,
  CMD_PAGELIST,
};

int main(int argc, char **argv)
{
  int c;
  int digit_optind = 0;
  struct opts {
    char *printer;
    char *page;
    enum command cmd;
    char *filename;
  } opts = {
    .cmd = CMD_PRINT,
    .printer = "VT100",
    .page = "80x25 terminal (no overflow)",
  };

  while (1)
  {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      /* {name, has_arg, flag, val} */
      {"printer", required_argument, 0, 'p' },
      {"page", required_argument, 0, 'P' },
      {"list", no_argument, 0, 'l' },
      {"list-pages", required_argument, 0, 'L' },
      {"log", required_argument, 0, LOG_OPTION },
      {"help", no_argument, 0, 'h' },
      {0, 0, 0, 0 }
    };

    c = getopt_long(argc, argv, "p:P:lL:h",
        long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 0:
        /* for default long options
         * (option->flag==0 -> 4th element of struct option) */
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;

      case '0':
      case '1':
      case '2':
        if (digit_optind != 0 && digit_optind != this_option_optind)
          printf("digits occur in two different argv-elements.\n");
        digit_optind = this_option_optind;
        printf("option %c\n", c);
        break;

      case 'p':
        /* --printer=VAL */
        opts.printer = optarg;
        break;

      case 'P':
        /* --page=VAL */
        opts.page = optarg;
        break;

      case 'l':
        /* --list */
        opts.cmd = CMD_LIST;
        break;

      case 'L':
        /* --list-pages=VAL */
        opts.cmd = CMD_PAGELIST;
        opts.printer = optarg;
        break;

      case LOG_OPTION:
        /* --log=VAL */
        set_log_level(optarg);
        break;

      case 'h':
        /* --printer=VAL */
        printf("Usage: %s [-p PRINTER] [-P PAGE] FILENAME | -l | -L PRINTER | -h\n",
            argv[0]);
        return 0;

      case '?':
        /* -* not in optstring */
        break;

      default:
        /* optstrings not handled (mistakes?) */
        printf("?? getopt returned character code 0%o ??\n", c);
    }
  }

  /* positional arguments */
  if (optind < argc)
  {
    opts.filename = argv[optind++];
  }
  if (optind < argc)
  {
    ERROR("too many positional arguments (first superfluous: '%s')", argv[optind]);
  }

  int x = 0, y = 0;
  if (argc < 2)
  {
    printf("Usage: %s [file-name]\n", argv[0]);
    return 1;
  }

  /* init printers */
  register_sinks();

  if (opts.cmd == CMD_LIST)
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
  else if (opts.cmd == CMD_PAGELIST)
  {
    /* list pages */
    int i;
    sink_t sink = printer_get_sink(opts.printer);
    char **pages = printer_get_sink_pages(&sink);

    for (i = 0; pages[i] != NULL; i++)
    {
      printf("%d: %s\n", i, pages[i]);
    }

    return 0;
  }

  /* convert png from param 1 */
  bitmap_t bmp = simple_png_read(opts.filename);
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
  sink_t sink = printer_get_sink(opts.printer);
  if (sink.printer == NULL)
  {
    ERROR("sink not found");
    return 1;
  }

  /* select page */
  page_t *page = printer_get_sink_page_by_name(&sink, opts.page);
  if (page == NULL)
  {
    ERROR("page '%s' cannot be found for '%s'", opts.page, opts.printer);
    return 1;
  }
  printer_set_sink_page(&sink, page);

  /* select size */
  if (printer_set_size(&sink, bmp.width, bmp.height))
  {
    ERROR("cannot set desired size: [%d,%d]", bmp.width, bmp.height);
  }

  while (y < bmp.height)
  {
    bitstream_t reader = init_bitstream(bmp.rows[y], bmp.width);
    x = 0;
    while(isnext(&reader))
    {
      int bit = get_bit(&reader);
      if (printer_feed_bit(&sink, x, y, bit))
      {
        ERROR("setting bit to (%d,%d) failed", x, y);
        return 1;
      }
      x++;
    }
    y++;
  }

  DEBUG("image parsed, cleaning up");
  printer_destroy(&sink);
  simple_png_free(&bmp);
  return 0;
}
