#include <stdlib.h>

#include "list.h"

list_t *list_alloc()
{
  list_t *first = (list_t*) malloc(sizeof(list_t));
  list_t *last = (list_t*) malloc(sizeof(list_t));

  first->next = last;
  last->next = NULL;

  first->type = LIST_FIRST;
  last->type = LIST_LAST;

  return first;
}

int list_free(list_t *first)
{
  if (first == NULL)
  {
    return -1;
  }

  /* remove every ordinary element (last will produce error) */
  while(list_remove(first) == 0);

  /* deallocate first and last elements */
  free(first->next);
  free(first);

  return 0;
}

list_t *list_next(list_t *element)
{
  if (element == NULL)
  {
    return NULL;
  }

  return element->next;
}

int list_append(list_t *previous, void *value)
{
  if (previous == NULL)
  {
    return -1;
  }

  list_t *e = (list_t*) malloc(sizeof(list_t));
  if (e == NULL)
  {
    return -1;
  }

  e->value = value;
  e->next = previous->next;
  e->type = LIST_ORDINARY;

  previous->next = e;

  return 0;
}

int list_remove(list_t *previous)
{
  if (previous == NULL)
  {
    return -1;
  }

  list_t *target = previous->next;

  if (target->type != LIST_ORDINARY)
  {
    return -1;
  }

  previous->next = target->next;

  free(target);

  return 0;
}

int list_has_next(list_t *element)
{
  if (element == NULL)
  {
    return -1;
  }

  return element->next != NULL && element->next->type == LIST_ORDINARY;
}
