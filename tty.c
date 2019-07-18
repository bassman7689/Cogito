#include "tty.h"
#include "stdlib.h"

size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static void clear_row(uint16_t *row) {
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    row[x] = vga_entry(' ', terminal_color);
  }
}
 
void terminal_initialize(void) 
{
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    const size_t index = y * VGA_WIDTH;
    clear_row(&terminal_buffer[index]);
    terminal_buffer[index] = vga_entry(' ', terminal_color);
  }
}

void terminal_setcolor(uint8_t color) 
{
  terminal_color = color;
}
 
static void terminal_putentryat(char c, uint8_t color, size_t x)
{
  const size_t index = (VGA_HEIGHT-1) * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scrollup()
{
  for (size_t y = 1; y < VGA_HEIGHT; y++) {
      const size_t prev_row = (y-1) * VGA_WIDTH;
      const size_t cur_row = y * VGA_WIDTH;
      clear_row(&terminal_buffer[prev_row]);
      memmove(&terminal_buffer[prev_row], &terminal_buffer[cur_row], VGA_WIDTH);
  }
  clear_row(&terminal_buffer[(VGA_HEIGHT-1)*VGA_WIDTH]);
}
 
void terminal_putchar(char c) 
{
  if (c == '\n') {
    terminal_scrollup();
    terminal_column = 0;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column);
    terminal_column++;
  }
  
  if (terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    terminal_scrollup();
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

  itoa(num, data, 16);
  terminal_writestring("0x");
  terminal_writestring(data);
}

