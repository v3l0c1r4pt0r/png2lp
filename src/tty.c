#include "logger.h"
#include "tty.h"

int tty_feed_bit(sink_t *sink, int x, int y, int bit);
int tty_set_size(sink_t *sink, int width, int height);

static char start[] = "";
static char nl[]="\n";

printer_t tty_vt100 = {
  .name = "VT100",
  .feed_bit = tty_feed_bit,
  .set_size = tty_set_size,
};

page_t tty_80x25 = {
  .name = "80x25 terminal",
  .width=80,
  .height=25
};

void tty_init()
{
  // TODO: check error codes
  printer_register_sink(&tty_vt100);
  printer_register_page(tty_vt100.name, &tty_80x25);
}

int tty_feed_bit(sink_t *sink, int x, int y, int bit)
{
  DEBUG("fed bit %d on (%d,%d)", bit,x,y);
  return -1;
}

int tty_set_size(sink_t *sink, int width, int height)
{
  DEBUG("set img size to %d,%d", width, height);
  return -1;
}
