#include <stdint.h>
#include "string.h"
#include "x86.h"

size_t strlen(const char* str)
{
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void *memcopy(void *dst, void *src, size_t size)
{
  char *dstp = (char*)dst;
  char *srcp  = (char*)src;
  for (size_t i = 0; i < size; i++) {
    dstp[i] = srcp[i];
  }

  return dstp;
}

void *memset(void *ptr, int x, size_t size)
{
  if ((uintptr_t)ptr%4 == 0 && size%4 == 0) {
    x &= 0xFF;
    stosl(ptr, (x<<24)|(x<<16)|(x<<8)|x, size/4);
  } else {
    stosb(ptr, x, size);
  }
  return ptr;
}
