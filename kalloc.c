#include <stdint.h>

#include "tty.h"
#include "kalloc.h"

#define PGROUNDUP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

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
  char *p;
  p = (char*)PGROUNDUP((uint32_t)start);
  for(; p + PAGE_SIZE <= (char*)end; p += PAGE_SIZE) {
    terminal_writestring("freeing page at: ");
    terminal_writehex((uint32_t)p);
    kfree(p);
  }
}

void kinit(multiboot_info_t *mbi)
{
  uintptr_t cur = (uintptr_t)mbi->mmap_addr;
  uintptr_t end = cur + mbi->mmap_length;
  while (cur < end) {
    multiboot_memory_map_t *mmm = (multiboot_memory_map_t*)cur;
    uintptr_t start_addr = mmm->addr;
    uintptr_t end_addr = mmm->addr + mmm->len;

    terminal_writestring("next\n");
    if (start_addr == 0x0) {
      cur += mmm->size + sizeof(uintptr_t);
      continue;
    }

    if (mmm->type == MULTIBOOT_MEMORY_AVAILABLE) {
      terminal_writestring("freeing multboot_memory_map_t range from: ");
      terminal_writehex(start_addr);
      terminal_writestring(" to: ");
      terminal_writehex(end_addr);
      terminal_putchar('\n');
      
      freerange((void*)start_addr, (void*)end_addr);
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
