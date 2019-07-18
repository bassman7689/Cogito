#include "stdlib.h"

char *itoa(int value, char *str, int base)
{
  char *p = str;
  int negate = 0;
  if (value < 0 && base == 10) {
    negate = 1;
    value = -value;
  }

  do {
    int rem = value % base;
    *p++ = (rem < 10) ? rem + '0' : (rem-10) + 'A';
    value = value / base;
  }while (value != 0);

  if (negate)
    *p++ = '-';

  *p = '\0';

  char *p1 = str;
  char *p2 = p-1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1++ = *p2;
    *p2-- = tmp;
  }

  return str;
}
