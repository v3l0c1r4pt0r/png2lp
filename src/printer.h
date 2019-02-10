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
typedef int (*printer_feed_bit_t)(struct sink *sink, int x, int y, int bit);
typedef int (*printer_set_size_t)(struct sink *sink, int width, int height);
typedef int (*printer_create_t)(struct sink *sink);
typedef int (*printer_destroy_t)(struct sink *sink);

typedef struct printer {
  char *name;
  char *description;
  printer_feed_bit_t feed_bit;
  printer_set_size_t set_size;
  printer_create_t create;
  printer_destroy_t destroy;
} printer_t;

typedef struct page {
  char *name;
  char *description;
  unsigned int width;
  unsigned int height;
  unsigned int dpi;
} page_t;

typedef struct sink {
  printer_t *printer;
  page_t *page;
  int fd;
  void *private_data;
} sink_t;

void register_sinks();
int printer_register_sink(printer_t *printer);
int printer_register_page(char *printer, page_t *page);
sink_t printer_get_sink(char *name);
char *printer_get_description(char *name);
char **printer_get_sinks();
page_t *printer_get_sink_page_by_name(sink_t *sink, char *name);
char *printer_get_page_description(sink_t *sink, char *name);
char **printer_get_sink_pages(sink_t *sink);
void printer_set_sink_page(sink_t *sink, page_t *page);

int printer_feed_bit(sink_t *sink, int x, int y, int bit);
int printer_set_size(sink_t *sink, int width, int height);
int printer_create(sink_t *sink);
int printer_destroy(sink_t *sink);

#endif // PRINTER_H
