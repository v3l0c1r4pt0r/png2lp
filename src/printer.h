#ifndef PRINTER_H
#define PRINTER_H
#include <stdint.h>

struct printer;
struct page;

/**
 * \brief Handler of function feeding bit to printer
 *
 * \param printer Printer descriptor
 * \param page Page descriptor
 * \param x Position on X axis
 * \param y Position on Y axis
 * \param bit Bit to set
 */
typedef void (*printer_feed_bit_t)(struct printer *printer, struct page *page, int x, int y, int bit);
typedef void (*printer_set_size_t)(struct printer *printer, struct page *page, int width, int height);

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

sink_t *printer_register_sink(printer_t *printer);
void printer_register_page(sink_t *sink, page_t *page);
sink_t *printer_get_sink(char *name);
char **printer_get_sink_pages(sink_t *sink);
void printer_set_sink_page(sink_t *sink, page_t *page);

#endif // PRINTER_H
