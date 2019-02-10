#include <stddef.h>
#include <stdlib.h>

#include "logger.h"
#include "oki.h"

void oki_linefeed();
int oki_feed_bit(sink_t *sink, int x, int y, int bit);
void oki_init();
void oki_enter_graphic_mode(int columns);
int oki_create(sink_t *sink);
int oki_destroy(sink_t *sink);

static char start[] = "\eK\x23\x01";
static char nl[]="\r\eJ\x18";

printer_t oki_3321 = {
  .name = "Oki 3321",
  .feed_bit = oki_feed_bit,
  .create = oki_create,
  .destroy = oki_destroy,
};

page_t oki_a4p = {
  .name = "A4 Portrait",
  .width=0x1e4,
  .height=95*8
};

void oki_init()
{
  // TODO: check error codes
  printer_register_sink(&oki_3321);
  printer_register_page(oki_3321.name, &oki_a4p);
}

oki_state_t *oki_get_sink_state(sink_t *sink)
{
  return (oki_state_t*) sink->private_data;
}

int oki_free_image(sink_t *sink)
{
  int i;
  oki_state_t *state = oki_get_sink_state(sink);

  if (state == NULL)
  {
    ERROR("no state in sink");
    return -1;
  }

  if (state->rows != NULL)
  {
    for (i = 0; i < state->num_of_rows; i++)
    {
      free(state->rows[i]);
    }
    free(state->rows);
  }
  else
  {
    return -1;
  }

  return 0;
}

int oki_create(sink_t *sink)
{
  oki_state_t *state = (oki_state_t*) malloc(sizeof(oki_state_t));
  if (state == NULL)
  {
    return -1;
  }
  state->width = -1;
  state->height = -1;
  state->ready_column = -1;
  state->ready_row = -1;
  state->num_of_rows = -1;
  state->rows = NULL;

  sink->private_data = (void*) state;

  return 0;
}

int oki_destroy(sink_t *sink)
{
  oki_state_t *state = oki_get_sink_state(sink);
  oki_free_image(sink);
  free(state);

  return 0;
}

int oki_feed_bit(sink_t *sink, int x, int y, int bit)
{
  return -1;
}
