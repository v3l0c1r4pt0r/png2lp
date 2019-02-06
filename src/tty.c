#include "tty.h"

void tty_feed_bit(struct printer *printer, struct page *page, int x, int y, int bit);

static char start[] = "";
static char nl[]="\n";

printer_t tty_vt100 = {
  .name = "VT100",
  .feed_bit = tty_feed_bit,
};

page_t tty_80x25 = {
  .width=80,
  .height=25
};

void tty_init()
{
  sink_t *sink = printer_register_sink(&tty_vt100);
  printer_register_page(sink, &tty_80x25);
}

void tty_feed_bit(struct printer *printer, struct page *page, int x, int y, int bit)
{
  return;
}