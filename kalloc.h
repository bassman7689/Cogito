#ifndef KALLOC_H
#define KALLOC_H

#include "multiboot.h"

#define EXTMEM 0x100000

#define PAGE_SIZE 4096
#define KERNBASE 0x80000000
#define KERNLINK KERNBASE + EXTMEM

#define P2V(a) ((void *)(((char *) (a)) + KERNBASE))

#define PGROUNDUP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

void kinit(void *start, void *end);
void freerange(void *start, void *end);
void kfree(char *v);
char* kalloc();

#endif
