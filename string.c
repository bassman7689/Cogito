#include "string.h"

size_t strlen(const char* str)
{
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void *memmove(void *dst, void *src, size_t size)
{
  char *dstp = (char*)dst;
  char *srcp  = (char*)src;
  for (int i = size; i >= 0; i--) {
    dstp[i] = srcp[i];
  }

  return dstp;
}

void *memset(void *ptr, int x, size_t size)
{
  char *dst = (char*)ptr;
  for (size_t i = 0; i < size; i++) {
    dst[i] = x;
  }

  return dst;
}
