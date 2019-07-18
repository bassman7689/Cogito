#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

char *itoa(int32_t value, char *str, int base);
char *lltoa(int64_t value, char *str, int base);

#endif
