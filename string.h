#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
void *memmove(void *dst, void *src, size_t size);
void *memset(void *ptr, int x, size_t size);

#endif
