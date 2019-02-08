#include <stdlib.h>

#include "list.h"
#include "oki.h"
#include "tty.h"
#include "logger.h"
#include "printer.h"

typedef struct {
  printer_t printer;
  list_t *pages;
} printer_descriptor_t;

list_t *printers;

void register_sinks()
{
  printers = list_alloc();
  oki_init();
  tty_init();
}

static printer_descriptor_t *printer_get_descriptor(printer_t *printer)
{
  return (printer_descriptor_t*) printer;
}

int printer_register_sink(printer_t *printer)
{
  // TODO: check error codes
  printer_descriptor_t *descr = (printer_descriptor_t*) malloc(sizeof(printer_descriptor_t));

  descr->printer = *printer;
  descr->pages = list_alloc();

  list_append(printers, descr);

  return 0;
}

int printer_register_page(char *printer, page_t *page)
{
  // TODO: check error codes
  printer_descriptor_t *descr;

  list_t *it = printers;
  while ((it = list_next(it)) != NULL)
  {
    descr = ((printer_descriptor_t*) it->value);
    if (strcmp(descr->printer.name, printer) == 0)
    {
      DEBUG("found sink for %s", printer);
      break;
    }
    descr = NULL;
  }

  list_append(descr->pages, page);

  return 0;
}

char **printer_get_sinks()
{
  int i = 0;
  int count = 0;
  list_t *descr = printers;
  char **result;

  /* get number of printers */
  while ((descr = list_next(descr)) != NULL)
  {
    count++;
  }

  /* allocate array for printer names */
  result = (char**) malloc(count + 1);

  descr = printers;
  while ((descr = list_next(descr)) != NULL)
  {
    result[i++] = ((printer_descriptor_t*) descr->value)->printer.name;
  }

  result[i] = NULL;

  return result;
}

sink_t printer_get_sink(char *name)
{
  sink_t result = {
    .printer = NULL,
    .page = NULL,
    .fd = -1
  };
  printer_t *printer;
  list_t *it = printers;
  while ((it = list_next(it)) != NULL)
  {
    printer = &(((printer_descriptor_t*) it->value)->printer);
    if (strcmp(printer->name, name) == 0)
    {
      DEBUG("found sink for %s", name);
      result.printer = printer;
      break;
    }
  }
  return result;
}

char **printer_get_sink_pages(sink_t *sink)
{
  int i = 0;
  int count = 0;
  list_t *pages;
  list_t *it;
  char **result;
  printer_descriptor_t *descr;

  /* find descriptor for sink */
  it = printers;
  while ((it = list_next(it)) != NULL)
  {
    pages = ((printer_descriptor_t*) it->value)->pages;
    descr = (printer_descriptor_t*) it->value;
    if (strcmp(descr->printer.name, sink->printer->name) == 0)
    {
      DEBUG("found sink for %s", sink->printer->name);
      break;
    }
    pages = NULL;
  }

  /* get number of pages */
  it = pages;
  while ((it = list_next(it)) != NULL)
  {
    count++;
  }

  /* allocate array for page names */
  result = (char**) malloc(count + 1);

  it = pages;
  while ((it = list_next(it)) != NULL)
  {
    result[i++] = ((page_t*) it->value)->name;
  }

  result[i] = NULL;

  return result;
}
