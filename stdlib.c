#include "stdlib.h"

void itoa(int value, char *str, int base)
{
  char *p = str;
  do {
    int digit = value % base;
    *p++ = (digit < 10) ? digit + '0' : digit + 'A' - 10;
  } while (value /= base);

  char *p1 = str;
  char *p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }
}
