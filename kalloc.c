#include <stdint.h>

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
  ph->next = kpages.freelist;
  kpages.freelist = ph;
}

void freerange(void *start, void *end)
{
  char *p = (char*)PGROUNDUP((uintptr_t)start);
  terminal_writestring("addr of p: ");
  terminal_writehex((uintptr_t)p);
  terminal_putchar('\n');

  int i = 0;
  for (;p + PAGE_SIZE <= (char*)end; p+=PAGE_SIZE, i++) {
    kfree(p);
    if (i > ((end - start) / PAGE_SIZE)) {
      terminal_writestring("past the end of the range\n");
      for (;;) {}
    }
  }

  terminal_writestring("done in freerange\n");
}

void kinit(multiboot_info_t *mbi)
{
  uintptr_t cur = (uintptr_t)mbi->mmap_addr;
  uintptr_t end = cur + mbi->mmap_length;
  while (cur < end) {
    multiboot_memory_map_t *mmm = (multiboot_memory_map_t*)cur;
    uintptr_t start_addr = mmm->addr;
    uintptr_t end_addr = mmm->addr + mmm->len;

    if (start_addr != 0 && mmm->type == MULTIBOOT_MEMORY_AVAILABLE) {
      freerange((void*)start_addr, (void*)end_addr);
    } else {
      terminal_writestring("skipping from: ");
      terminal_writehex((uint32_t)start_addr);
      terminal_writestring(" to: ");
      terminal_writehex((uint32_t)end_addr);
      terminal_putchar('\n');
    }
    cur += mmm->size + sizeof(uintptr_t);
  }
  terminal_writestring("done freeing\n");
}

char* kalloc()
{
  struct page_head *ph;

  ph = kpages.freelist;
  if (ph)
    kpages.freelist = ph->next;
  return (char*)ph;
}
