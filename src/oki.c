#include "oki.h"

void oki_linefeed();
void oki_feed_bit(struct printer *printer, struct page *page, int x, int y, int bit);
void oki_init();
void oki_enter_graphic_mode(int columns);

static char start[] = "\eK\x23\x01";
static char nl[]="\r\eJ\x18";

printer_t oki_3321 = {
  .name = "Oki 3321",
  .feed_bit = oki_feed_bit,
};

page_t oki_a4p = {
  .name = "A4 Portrait",
  .width=0x1e4,
  .height=95*8
};

void oki_init()
{
  sink_t *sink = printer_register_sink(&oki_3321);
  printer_register_page(sink, &oki_a4p);
}

void oki_feed_bit(struct printer *printer, struct page *page, int x, int y, int bit)
{
  return;
}
