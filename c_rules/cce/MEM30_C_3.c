#include <stdlib.h>

struct list_s {
  int value;
  struct list_s* next;
};

void free_list(struct list_s* head) {
  struct list_s* p;
  struct list_s* q;
  for (p = head; p != NULL; p = q) {
    q = p->next;
    free(p);
  }
  head = NULL;
}

