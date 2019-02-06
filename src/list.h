#ifndef LIST_H
#define LIST_H

typedef enum {
  LIST_ORDINARY = 0, /**< all removable elements are of this type */
  LIST_FIRST,
  LIST_LAST,
} list_type_t;

struct list;
typedef struct {
  void *value;
  struct list *next;
  list_type_t type;
} list_t;

list_t *list_alloc();
void list_free(list_t *first);
list_t *list_next(list_t *element);
void list_append(list_t *previous, void *value);
void list_remove(list_t *previous);
int list_has_next(list_t *element);

#endif // LIST_H
