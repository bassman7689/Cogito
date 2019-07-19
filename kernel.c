#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty.h"
#include "multiboot.h"
#include "kalloc.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern char end[]; // first address after kernel

void print_memory_map(multiboot_info_t *mbi)
{
  uintptr_t cur = (uintptr_t)mbi->mmap_addr;
  uintptr_t end = cur + mbi->mmap_length;

  while (cur < end) {
    multiboot_memory_map_t* mmm = (multiboot_memory_map_t*)cur;
    uint64_t addr = ((uint64_t)mmm->base_addr_low) | ((uint64_t)mmm->base_addr_high) << 32;
    uint64_t len = ((uint64_t)mmm->length_low) | ((uint64_t)mmm->length_high) << 32;
    terminal_writestring("start: ");
    terminal_writehex(addr);
    terminal_writestring(" length: ");
    terminal_writehex(len);
    switch(mmm->type) {
    case MULTIBOOT_MEMORY_AVAILABLE:
      terminal_writestring(" AVAILABLE\n");
      break;
    case MULTIBOOT_MEMORY_RESERVED:
      terminal_writestring(" RESERVED\n");
      break;
    case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
      terminal_writestring(" ACPI RECLAIMABLE\n");
      break;
    case MULTIBOOT_MEMORY_NVS:
      terminal_writestring(" NVS\n");
      break;
    case MULTIBOOT_MEMORY_BADRAM:
      terminal_writestring(" BADRAM\n");
      break;
    }

    cur += mmm->size + sizeof(uintptr_t);
  }
}
 
void kernel_main(uint32_t magic, multiboot_info_t *mbi) 
{
  /* Initialize terminal interface */
  terminal_initialize();

  if (magic != 0x2BADB002) {
    terminal_writestring("Invalid magic number from grub: ");
    terminal_writehex(magic);
    terminal_putchar('\n');
    return;
  }

  if ((mbi->flags & (1<<6)) == 0) {
    terminal_writestring("No multiboot memory map was provided!\n");
    return;
  }

  terminal_writestring("Hello, Kernel World!\n");

  print_memory_map(mbi);
 
  terminal_writestring("Initializing page frame allocator\n");
  kinit(end, end+4*1024*1024);

  char *test_page = kalloc();
  terminal_writestring("Test Page Address: ");
  terminal_writehex((uintptr_t)test_page);

  /* terminal_writestring("Test Page Value: "); */
  /* terminal_writehex(*test_page); */
}
