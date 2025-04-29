#include <stddef.h>
#include <stdint.h>
#include "vga_driver.h"
#include "util.h"
#include "string.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_init(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

    terminal_writestring("VGA driver initialized\n");
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        return;
    }

    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    switch (c) {
        case '\n': {
            terminal_column = 0;
            terminal_row++;
            break;
        }

        case '\b': {  // Backspace
            if (terminal_column > 0) {
                terminal_column--;
            } else if (terminal_row > 0) {
                terminal_row--;
                terminal_column = VGA_WIDTH - 1;
            }
            // Erase the character
            terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
            break;
        }

        default: {
            terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
            terminal_column++;
        }
    }

	if (terminal_column == VGA_WIDTH) {
		terminal_column = 0;
        terminal_row++;
	}

    terminal_scroll();
    terminal_movecsr(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_movecsr(size_t x, size_t y) {
    uint32_t temp;

    temp = y * VGA_WIDTH + x;

    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

// Converts a 4-bit nibble to a hex character
char nibble_to_hex(uint8_t nibble) {
    nibble &= 0xF;  // Ensure only 4 bits
    return (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
}

// Prints a 32-bit value as 0xFFFFFFFF
void terminal_hexprint(uint32_t value) {
    terminal_writestring("0x");  // Prefix

    // Print each byte (from highest to lowest)
    for (int i = 28; i >= 0; i -= 4) {  // Iterate by 4 bits (nibble)
        uint8_t nibble = (value >> i) & 0xF;
        terminal_putchar(nibble_to_hex(nibble));
    }
}

void terminal_decprint(uint32_t num) {
    char buffer[12]; // Enough for 32-bit number (max 10 digits) + null terminator
    int i = 0;
    
    // Handle 0 explicitly
    if (num == 0) {
        terminal_putchar('0');
        return;
    }
    
    // Build the string in reverse order
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Print in correct order (reverse of how we built it)
    while (--i >= 0) {
        terminal_putchar(buffer[i]);
    }
}

void terminal_scroll(void) {
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (0x0F << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(terminal_row >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = terminal_row - 25 + 1;
        memcpy ((unsigned short *)0xB8000, (unsigned short *)0xB8000 + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw ((unsigned short *)0xB8000 + (25 - temp) * 80, blank, 80);
        terminal_row = 25 - 1;
    }
}