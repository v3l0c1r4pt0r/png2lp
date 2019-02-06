#include <stdlib.h>

#include "list.h"
#include "oki.h"
#include "tty.h"
#include "printer.h"

typedef struct {
  printer_t *printer;
  list_t *pages;
} printer_descriptor_t;

list_t *printers;

void register_sinks()
{
  printers = list_alloc();
  oki_init();
  tty_init();
}

int printer_register_sink(printer_t *printer)
{
  return NULL;
}

int printer_register_page(char *printer, page_t *page)
{
  return -1;
}
