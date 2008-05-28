#include <stdlib.h>

struct list_s {
  int value;
  struct list_s* next;
};

void free_list(struct list_s* head) {
  struct list_s* p;
  for (p = head; p != NULL; p = p->next)     /* WRONG */
    free(p);
}
