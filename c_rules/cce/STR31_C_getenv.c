#include <string.h>
#include <stdlib.h>

int main() {
  /* ... */
  char *editor;
  char *buff;

  editor = getenv("EDITOR");
  if (editor) {
    size_t len = strlen(editor)+1;
    buff = (char *)malloc(len);
    if (!buff) {
      /* Handle malloc() Error */
    }
    memcpy(buff, editor, len);
  }
  /* ... */
  return 0;
}
