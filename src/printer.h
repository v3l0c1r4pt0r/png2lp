#ifndef PRINTER_H
#define PRINTER_H
#include <stdint.h>

struct printer;
struct page;
struct sink;

/**
 * \brief Handler of function feeding bit to printer
 *
 * \param printer Printer descriptor
 * \param page Page descriptor
 * \param x Position on X axis
 * \param y Position on Y axis
 * \param bit Bit to set
 */
typedef void (*printer_feed_bit_t)(struct sink *sink, int x, int y, int bit);
typedef void (*printer_set_size_t)(struct sink *sink, int width, int height);

typedef struct printer {
  char *name;
  printer_feed_bit_t feed_bit;
  printer_set_size_t set_size;
} printer_t;

typedef struct page {
  char *name;
  unsigned int width;
  unsigned int height;
  unsigned int dpi;
} page_t;

typedef struct sink {
  printer_t *printer;
  page_t *page;
  int fd;
} sink_t;

void register_sinks();
int printer_register_sink(printer_t *printer);
int printer_register_page(char *printer, page_t *page);
sink_t printer_get_sink(char *name);
char **printer_get_sinks();
page_t *printer_get_sink_page_by_name(sink_t *sink, char *name);
char **printer_get_sink_pages(sink_t *sink);
void printer_set_sink_page(sink_t *sink, page_t *page);

#endif // PRINTER_H
