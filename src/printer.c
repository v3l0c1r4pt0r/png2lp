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
  printer_descriptor_t *descr = (printer_descriptor_t*) malloc(sizeof(printer_descriptor_t));

  descr->printer = printer;
  descr->pages = list_alloc();

  list_append(printers, descr);

  return 0;
}

int printer_register_page(char *printer, page_t *page)
{
  return -1;
}

char **printer_get_sinks()
{
  int i = 0;
  int count = 0;
  list_t *descr = printers;
  char **result;

  /* get number of printers */
  while ((descr = list_next(descr)) != NULL) // TODO: why this counts to 3, where there are only 2 items
  {
    count++;
  }

  /* allocate array for printer names */
  result = (char**) malloc(count + 1);

  descr = printers;
  while (list_has_next(descr))
  {
    descr = list_next(descr);
    result[i++] = ((printer_descriptor_t*) descr->value)->printer->name;
  }

  result[i] = NULL;

  return result;
}
