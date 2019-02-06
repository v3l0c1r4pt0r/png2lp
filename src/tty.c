#include "tty.h"

void tty_init()
{
  sink_t *sink = printer_register_sink(tty_vt100);
  printer_register_page(sink, &tty_80x25);
}
