#include "oki.h"

void oki_linefeed();
void oki_feed_bit(struct printer *printer, struct page *page, int x, int y, int bit);
void oki_init();
void oki_enter_graphic_mode(int columns);

void oki_init()
{
  sink_t *sink = printer_register_sink(oki_3321);
  printer_register_page(sink, &oki_a4p);
}
