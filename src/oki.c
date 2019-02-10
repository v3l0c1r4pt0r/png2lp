#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "logger.h"
#include "oki.h"

void oki_linefeed();
int oki_feed_bit(sink_t *sink, int x, int y, int bit);
int oki_set_size(sink_t *sink, int width, int height);
void oki_init();
void oki_enter_graphic_mode(int columns);
int oki_create(sink_t *sink);
int oki_destroy(sink_t *sink);

static char start[] = "\eK";
static char nl[]="\r\eJ\x18";

uint8_t masks[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

typedef struct {
  uint8_t bits;
  uint8_t fill;
} word_t; /**< describes one character pushed to printer */

typedef struct {
  int width;
  int height;
  int num_of_rows; /**< number of rows, where row contain subrow of 8 pixels */
  word_t **rows;
  int ready_row; /**< location of last ready to print row */
  int ready_column; /**< number of last column ready to print in a row */
} oki_state_t;

printer_t oki_3321 = {
  .name = "Oki 3321",
  .feed_bit = oki_feed_bit,
  .set_size = oki_set_size,
  .create = oki_create,
  .destroy = oki_destroy,
};

page_t oki_a4p = {
  .name = "A4 Portrait",
  .width=0x1e4,
  .height=102*8
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
  int row, bitn;
  int *r,*c;
  oki_state_t *state = oki_get_sink_state(sink);
  word_t *word;

  DEBUG("feeding bit %d on (%d,%d)", bit,x,y);

  if (x > state->width | y > state->height)
  {
    ERROR("(%d,%d): cannot write outside the image", x, y);
    return -1;
  }

  /* find row */
  row = y / 8;
  bitn = y % 8;
  DEBUG("setting bit #%d to row %d", bitn, row);

  /* set bit and mark as written in a word */
  word = &state->rows[row][x];
  if (!bit)
  {
    word->bits |= masks[bitn];
  }
  word->fill |= masks[bitn];

  DEBUG("word state is %02x on mask %02x", word->bits, word->fill);

  /* print all ready words from current to last set */
  r = &state->ready_row; /* this way we can use short name while still
                          * incrementing ready state */
  c = &state->ready_column;

  /* for every row from last printed to currently updated */
  for (; *r <= row; (*r)++)
  {
    /* for every column in that row */
    for (; *c < state->width; (*c)++)
    {
      word = &state->rows[*r][*c];
      if (word->fill == 0xff)
      {
        /* if first in a row, enter graphic mode */
        if (*c == 0)
        {
          oki_enter_graphic_mode(state->width);
        }
        DEBUG("found ready word @%d,%d: %02x", *r, *c, word->bits);
        printf("%c", word->bits);
      }
      else
      {
        return 0;
      }

      /* if we reached column of X, leave to not damage state->ready_column */
      if (*c > x && *c != state->width)
      {
        return 0;
      }
    }
    *c = 0;
    DEBUG("finished row %d", *r);
    oki_linefeed();
    // TODO: print EOL
  }

  /* getting here means everything is ready */

  return 0;
}

int oki_set_size(sink_t *sink, int width, int height)
{
  int i;
  int rc = 0;
  oki_state_t *state = oki_get_sink_state(sink);

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

  /* if there was some buffer before, free it now */
  oki_free_image(sink);

  /* allocate buffers for image state */
  state->num_of_rows = state->height / 8 + ((state->height % 8) == 1);
  DEBUG("number of word rows to cover whole img is %d", state->num_of_rows);
  state->rows = (word_t**) malloc(sizeof(word_t*) * state->num_of_rows);
  for (i = 0; i < state->num_of_rows; i++)
  {
    state->rows[i] = (word_t*) calloc(state->width, sizeof(word_t));
  }

  /* set current printing state */
  state->ready_row = 0;
  state->ready_column = 0;

  /* set unused bits in last row to 1, if any */
  if (state->num_of_rows * 8 > state->height)
  {
    int mask_start = state->height % 8;
    uint8_t mask = 0;
    for (i = mask_start; i < 8; i++)
    {
      mask |= masks[i];
    }
    DEBUG("setting mask %02x for unused bits on last row", mask);

    /* iterate through whole row */
    for (i = 0; i < state->width; i++)
    {
      state->rows[state->num_of_rows - 1][i].fill = mask;
    }
  }

  return rc;
}

void oki_enter_graphic_mode(int columns)
{
  if (columns > 0xffff)
  {
    ERROR("not enough room to push whole number (16bit available)");
  }
  printf("%s%c%c", start, columns % 0x100, columns / 0x100);
}

void oki_linefeed()
{
  printf(nl);
}
