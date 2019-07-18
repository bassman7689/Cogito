#ifndef KALLOC_H
#define KALLOC_H

#include "multiboot.h"

#define PAGE_SIZE 4096

#define PGROUNDUP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

void kinit(multiboot_info_t *mbi);
void freerange(void *start, void *end);
void kfree(char *v);
char* kalloc();

#endif
