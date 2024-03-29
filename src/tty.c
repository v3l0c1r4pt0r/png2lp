#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "logger.h"
#include "tty.h"

typedef struct {
  int width;
  int height;
  int cur_row; /**< row, where last printer character was */
  int cur_column; /**< columns of last printed character */
} tty_state_t;

int tty_feed_bit(sink_t *sink, int x, int y, int bit);
int tty_set_size(sink_t *sink, int width, int height);
int tty_create(sink_t *sink);
int tty_destroy(sink_t *sink);

printer_t tty_vt100 = {
  .name = "vt100",
  .description = "VT100",
  .feed_bit = tty_feed_bit,
  .set_size = tty_set_size,
  .create = tty_create,
  .destroy = tty_destroy,
};

page_t tty_80x25 = {
  .name = "80x25",
  .description = "80x25 terminal",
  .width=80,
  .height=25
};

page_t tty_80xINF = {
  .name = "80xINF",
  .description = "80x25 terminal (no overflow)",
  .width=80,
  .height=(unsigned int)-1
};

void tty_init()
{
  // TODO: check error codes
  printer_register_sink(&tty_vt100);
  printer_register_page(tty_vt100.name, &tty_80x25);
  printer_register_page(tty_vt100.name, &tty_80xINF);
}

tty_state_t *tty_get_sink_state(sink_t *sink)
{
  return (tty_state_t*) sink->private_data;
}

int tty_feed_bit(sink_t *sink, int x, int y, int bit)
{
  tty_state_t *state = tty_get_sink_state(sink);

  if (x > state->width | y > state->height)
  {
    ERROR("(%d,%d): cannot write outside the image", x, y);
    return -1;
  }

  if (x != state->cur_column + 1 || y != state->cur_row)
  {
    DEBUG("wrongly fed bit %d on (%d,%d)", bit,x,y);
    return -1;
  }

  printf("%s", bit?"  ":"\u2588\u2588");
  state->cur_column++;

  if (state->cur_column == (state->width - 1))
  {
    DEBUG("finished row %d", state->cur_row);
    printf("\n");
    state->cur_row++;
    state->cur_column = -1;
  }

  return 0;
}

int tty_set_size(sink_t *sink, int width, int height)
{
  int rc = 0;
  tty_state_t *state = tty_get_sink_state(sink);

  DEBUG("setting img size to %d,%d", width, height);
  state->width = width;
  state->height = height;

  /* check if feasible */
  if (width > sink->page->width)
  {
    WARNING("img requested is wider than maximum, shrinking");
    state->width = sink->page->width;
    rc = 1;
  }

  if (height > sink->page->height)
  {
    WARNING("img requested is higher than maximum, shrinking");
    state->height = sink->page->height;
    rc = 1;
  }

  return rc;
}

int tty_create(sink_t *sink)
{
  tty_state_t *state = (tty_state_t*) malloc(sizeof(tty_state_t));
  state->width = -1;
  state->height = -1;

  /* set to (0, -1); next pixel will be (0, 0) */
  state->cur_row = 0;
  state->cur_column = -1;

  sink->private_data = (void*) state;

  return 0;
}

int tty_destroy(sink_t *sink)
{
  free(sink->private_data);
}
