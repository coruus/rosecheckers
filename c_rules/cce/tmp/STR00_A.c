#include <string.h>

void STR00_A() {
  char src1[100] = "hello";
  char src2[7] =  {'g','o','o','d','b','y','e'};
  char dst1[6], dst2[5];
  int r1, r2;

  r1 = strcpy_s(dst1, 6, src1);
  r2 = strcpy_s(dst2, 5, src2);
}
