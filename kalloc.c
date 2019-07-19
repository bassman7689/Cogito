#include <stdint.h>
#include <limits.h>

#include "tty.h"
#include "kalloc.h"

struct page_head {
  struct page_head *next;
};

struct {
  struct page_head *freelist;
} kpages;

void kfree(char* v)
{
  struct page_head* ph = (struct page_head*)v;
  terminal_writestring("Freeing Page: ");
  terminal_writehex((uintptr_t)v);
  terminal_putchar('\n');
  memset(v, 1, PAGE_SIZE);
  ph->next = kpages.freelist;
  kpages.freelist = ph;
}

void freerange(void *start, void *end)
{
  char *p = (char*)PGROUNDUP((uintptr_t)start);
  if ((uintptr_t)start > (INT_MAX - ((uintptr_t)end))) {
    terminal_writestring("Integer Overflow!");
  }

  for (;p + PAGE_SIZE <= (char*)end; p+=PAGE_SIZE)
    kfree(p);
}

void kinit(void *start, void *end)
{
  terminal_writestring("freeing from: ");
  terminal_writehex((uintptr_t)start);
  terminal_writestring(" to: ");
  terminal_writehex((uintptr_t)end);
  terminal_putchar('\n');
  freerange(start, end);
}

char* kalloc()
{
  struct page_head *ph;

  ph = kpages.freelist;
  if (ph)
    kpages.freelist = ph->next;
  return (char*)ph;
}
