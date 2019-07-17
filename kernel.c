#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode color constants. */
enum vga_color {
		VGA_COLOR_BLACK = 0,
		VGA_COLOR_BLUE = 1,
		VGA_COLOR_GREEN = 2,
		VGA_COLOR_CYAN = 3,
		VGA_COLOR_RED = 4,
		VGA_COLOR_MAGENTA = 5,
		VGA_COLOR_BROWN = 6,
		VGA_COLOR_LIGHT_GREY = 7,
		VGA_COLOR_DARK_GREY = 8,
		VGA_COLOR_LIGHT_BLUE = 9,
		VGA_COLOR_LIGHT_GREEN = 10,
		VGA_COLOR_LIGHT_CYAN = 11,
		VGA_COLOR_LIGHT_RED = 12,
		VGA_COLOR_LIGHT_MAGENTA = 13,
		VGA_COLOR_LIGHT_BROWN = 14,
		VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
  return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
  return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void) 
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}
 
void terminal_setcolor(uint8_t color) 
{
  terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) 
{
  if (c == '\n') {
    terminal_column = 0;
    terminal_row += 1;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    terminal_column++;
  }
  
  if (terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_row = 0;
  }
}
 
void terminal_write(const char* data, size_t size) 
{
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
  terminal_write(data, strlen(data));
}

void terminal_writehex(uint32_t num) {
  char data[20];
  char* p = data;
  for (size_t i = 0; i < 20; i++)
    data[i] = '\0';


  do {
    int digit = num % 16;
    *p++ = (digit < 10) ? digit + '0' : digit + 'A' - 10;
  } while (num /= 16);

  *p++ = 'x';
  *p++ = '0';
  *p = '\0';

  char *p1 = data;
  char *p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  terminal_writestring(data);
}

void print_memory_map(multiboot_info_t *mbi)
{
  uintptr_t cur = (uintptr_t)mbi->mmap_addr;
  uintptr_t end = cur + mbi->mmap_length;

  while (cur < end) {
    multiboot_memory_map_t* mmm = (multiboot_memory_map_t*)cur;
    terminal_writestring("start: ");
    terminal_writehex(mmm->addr);
    terminal_writestring(" length: ");
    terminal_writehex(mmm->len);
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
 
  terminal_writestring("Hello, Kernel World!\n");

  print_memory_map(mbi);
}
