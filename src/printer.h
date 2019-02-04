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

typedef struct printer {
  printer_feed_bit_t feed_bit;
} printer_t;

typedef struct page {
  unsigned int width;
  unsigned int height;
  unsigned int dpi;
} page_t;

void register_printer(printer_t *printer);
void register_format(printer_t *printer, page_t *page);

#endif // PRINTER_H
