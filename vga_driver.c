#include <stddef.h>
#include <stdint.h>
#include "vga_driver.h"

char font8x8[128][8] = {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (;)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

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
    if(terminal_row >= VGA_HEIGHT)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = terminal_row - VGA_HEIGHT + 1;
        memcpy ((unsigned short *)0xB8000, (unsigned short *)0xB8000 + temp * VGA_WIDTH, (VGA_HEIGHT - temp) * VGA_WIDTH * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw ((unsigned short *)0xB8000 + (VGA_HEIGHT - temp) * VGA_WIDTH, blank, VGA_WIDTH);
        terminal_row = VGA_HEIGHT - 1;
    }
}

void terminal_clear() {
   	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	terminal_movecsr(terminal_column, terminal_row);
}

uint8_t reverse_bits(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// Set VGA 80x50 text mode with 8x8 font
void load_font8x8() {
    // Set up VGA sequencer for font loading
    outportb(VGA_SEQUENCER_INDEX, 0x00);
    outportb(VGA_SEQUENCER_DATA, 0x01);    // Synchronous reset

    outportb(VGA_SEQUENCER_INDEX, 0x02);
    outportb(VGA_SEQUENCER_DATA, 0x04);    // Enable writing to plane 2

    outportb(VGA_SEQUENCER_INDEX, 0x04);
    outportb(VGA_SEQUENCER_DATA, 0x07);    // Memory mode: enable extended memory, odd/even, plane 2

    outportb(VGA_SEQUENCER_INDEX, 0x00);
    outportb(VGA_SEQUENCER_DATA, 0x03);    // Clear synchronous reset

    // Set up VGA graphics controller for font loading
    outportb(VGA_GRAPHICS_INDEX, 0x04);
    outportb(VGA_GRAPHICS_DATA, 0x02);     // Select plane 2

    outportb(VGA_GRAPHICS_INDEX, 0x05);
    outportb(VGA_GRAPHICS_DATA, 0x00);     // Disable odd/even addressing

    outportb(VGA_GRAPHICS_INDEX, 0x06);
    outportb(VGA_GRAPHICS_DATA, 0x00);     // Map VGA memory at 0xA0000

    // Write font data to VGA memory
    volatile unsigned char *vga_mem = (volatile unsigned char *)FONT_MEM_LOCATION;

    for (int i = 0; i < 128; i++) {
        for (int j = 8; j > -1; j--) {
            vga_mem[i * 32 + j] = reverse_bits(font8x8[i][j]);
        }
    }

    // Restore VGA registers to text mode operation
    outportb(VGA_SEQUENCER_INDEX, 0x00);
    outportb(VGA_SEQUENCER_DATA, 0x01);    // Synchronous reset

    outportb(VGA_SEQUENCER_INDEX, 0x02);
    outportb(VGA_SEQUENCER_DATA, 0x03);    // Enable writing to planes 0 and 1

    outportb(VGA_SEQUENCER_INDEX, 0x04);
    outportb(VGA_SEQUENCER_DATA, 0x03);    // Memory mode: normal operation

    outportb(VGA_SEQUENCER_INDEX, 0x00);
    outportb(VGA_SEQUENCER_DATA, 0x03);    // Clear synchronous reset

    // Restore graphics controller
    outportb(VGA_GRAPHICS_INDEX, 0x04);
    outportb(VGA_GRAPHICS_DATA, 0x00);     // Select plane 0

    outportb(VGA_GRAPHICS_INDEX, 0x05);
    outportb(VGA_GRAPHICS_DATA, 0x10);     // Enable odd/even addressing

    outportb(VGA_GRAPHICS_INDEX, 0x06);
    outportb(VGA_GRAPHICS_DATA, 0x0E);     // Map text buffer for text mode
}

void set_cursor_shape(uint8_t start, uint8_t end) {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, (inportb(0x3D5) & 0xC0) | start); // start at scanline 6
    outportb(0x3D4, 0x0B);
    outportb(0x3D5, (inportb(0x3D5) & 0xE0) | end);
}

// Function to switch to 80x50 text mode
void switch_to_80x50_mode() {
    unsigned char temp;

    // Unlock CRTC registers (clear bit 7 of register 0x11)
    outportb(VGA_CRTC_INDEX, 0x11);
    temp = inportb(VGA_CRTC_DATA);
    outportb(VGA_CRTC_DATA, temp & 0x7F);

    outportb(0x3D4, 0x09);

    // Program CRTC registers for 80x50 text mode
    static const unsigned char crtc_regs[] = {
        0x00, 0x6B,   // Horizontal Total
        0x01, 0x59,   // Horizontal Display End
        0x02, 0x50,   // Horizontal Blank Start
        0x03, 0x82,   // Horizontal Blank End
        0x04, 0x55,   // Horizontal Retrace Start
        0x05, 0x81,   // Horizontal Retrace End
        0x06, 0x7B,   // Vertical Total (123 scanlines)
        0x07, 0x3F,   // Overflow (bits 8-9 of vertical total)
        0x08, 0x00,   // Preset Row Scan
        0x09, 0x07,   // Maximum Scan Line (8 pixels high)
        0x10, 0x6B,   // Vertical Retrace Start (107)
        0x11, 0x8E,   // Vertical Retrace End and unlock CRTC
        0x12, 0xFB,   // Vertical Display End (49 rows)
        0x13, 0x2D,   // Offset (80 chars * 2 bytes = 160 bytes per row)
        0x14, 0x00,   // Underline Location
        0x15, 0x5F,   // Vertical Blank Start (95)
        0x16, 0x7E,   // Vertical Blank End (126)
        0x17, 0xA3,   // Mode Control
    };

    // Apply CRTC register values
    for (int i = 0; i < sizeof(crtc_regs); i += 2) {
        outportb(VGA_CRTC_INDEX, crtc_regs[i]);
        outportb(VGA_CRTC_DATA, crtc_regs[i+1]);
    }

    // Load the 8x8 font
    load_font8x8();
    set_cursor_shape(0x00, 0x07);
}

void terminal_write_screen_size() {
    // Horizontal displayed (chars per row)
    outportb(0x3D4, 0x01);
    uint8_t cols = inportb(0x3D5) + 1;

    // Vertical displayed (rows)
    outportb(0x3D4, 0x12);
    uint8_t rows = inportb(0x3D5) + 1;

    terminal_writestring("Screen size: ");
    terminal_decprint(rows);
    terminal_writestring("x");
    terminal_decprint(cols);
    terminal_writestring("\n");
}

void *framebuffer;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint32_t framebuffer_bpp;
uint32_t framebuffer_pitch;

#define MAX_FONT_SIZE 8192

uint8_t font_buffer[MAX_FONT_SIZE]; // static font data buffer

psf1_font_t loaded_font = {
    .glyph_buffer = font_buffer,
    .glyph_count = 0,
    .glyph_size = 0
};

uint32_t gterm_cursor_x = 0;
uint32_t gterm_cursor_y = 0;

#define GTERM_COLS (framebuffer_width / 8)
#define GTERM_ROWS (framebuffer_height / loaded_font.glyph_size)

uint32_t gterminal_color = 0xFFFFFF;
uint32_t gterminal_bgcolor = 0x000000;

void gterminal_init(multiboot_info_t* mbd) {
    framebuffer = (void *)(uintptr_t)(mbd->framebuffer_addr);
    framebuffer_width = mbd->framebuffer_width;
    framebuffer_height = mbd->framebuffer_height;
    framebuffer_bpp = mbd->framebuffer_bpp;
    framebuffer_pitch = mbd->framebuffer_pitch;
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x < 0 || (uint32_t)x >= framebuffer_width || y < 0 || (uint32_t)y >= framebuffer_height)
        return;

    uint8_t *pixel_ptr = (uint8_t *)framebuffer + y * framebuffer_pitch + x * (framebuffer_bpp / 8);

    // Assuming framebuffer_bpp is 32 (ARGB)
    if (framebuffer_bpp == 32) {
        *(uint32_t *)pixel_ptr = color;
    } else if (framebuffer_bpp == 24) {
        pixel_ptr[0] = color & 0xFF;         // Blue
        pixel_ptr[1] = (color >> 8) & 0xFF;  // Green
        pixel_ptr[2] = (color >> 16) & 0xFF; // Red
    } else if (framebuffer_bpp == 16) {
        // Convert RGB to 5:6:5
        uint16_t r = (color >> 16) & 0xFF;
        uint16_t g = (color >> 8) & 0xFF;
        uint16_t b = color & 0xFF;
        uint16_t rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *(uint16_t *)pixel_ptr = rgb565;
    }
}

void draw_example() {
    uint32_t red = 0xFF0000;
    for (uint32_t y = 0; y < 100; ++y) {
        for (uint32_t x = 0; x < 100; ++x) {
            draw_pixel(x, y, red);
        }
    }
}

int load_psf1_font(const char* path) {
    FIL file;
    UINT read;

    if (f_open(&file, path, FA_READ) != FR_OK) {

        return -1; // failed to open
    }

    psf1_header_t header;
    f_read(&file, &header, sizeof(header), &read);

    if (header.magic[0] != 0x36 || header.magic[1] != 0x04) {
        //draw_example();
        f_close(&file);
        return -2; // invalid magic
    }

    uint32_t glyphs = (header.mode & 0x01) ? 512 : 256;
    uint32_t glyph_size = header.charsize;
    uint32_t font_data_size = glyphs * glyph_size;

    uint8_t* buffer = kmalloc(font_data_size); // replace with your own allocator
    if (!buffer) {
        //draw_example();
        f_close(&file);
        return -3; // allocation failed
    }

    f_read(&file, buffer, font_data_size, &read);
    f_close(&file);

    loaded_font.glyph_buffer = buffer;
    loaded_font.glyph_count = glyphs;
    loaded_font.glyph_size = glyph_size;

    return 0; // success
}

void draw_char_psf(int x, int y, char c, uint32_t color) {
    uint8_t* glyph = &loaded_font.glyph_buffer[(uint8_t)c * loaded_font.glyph_size];

    for (int row = 0; (uint32_t)row < loaded_font.glyph_size; ++row) {
        uint8_t bits = glyph[row];
        for (int col = 0; (uint32_t)col < 8; ++col) {
            if (bits & (1 << (7 - col))) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void gterminal_scroll() {
    int row_size = loaded_font.glyph_size;
    uint8_t* fb = (uint8_t*)framebuffer;

    for (int y = 0; (uint32_t)y < framebuffer_height - row_size; ++y) {
        for (int x = 0; (uint32_t)x < framebuffer_width * (framebuffer_bpp / 8); ++x) {
            fb[y * framebuffer_pitch + x] = fb[(y + row_size) * framebuffer_pitch + x];
        }
    }

    // Clear the last row
    for (int y = framebuffer_height - row_size; (uint32_t)y < framebuffer_height; ++y) {
        for (int x = 0; (uint32_t)x < framebuffer_width; ++x) {
            draw_pixel(x, y, gterminal_bgcolor);
        }
    }

    gterm_cursor_y -= 1;
}

void gterminal_clear() {
    for (int y = 0; (uint32_t)y < framebuffer_height; ++y) {
        for (int x = 0; (uint32_t)x < framebuffer_width; ++x) {
            draw_pixel(x, y, gterminal_bgcolor);
        }
    }
    gterm_cursor_x = 0;
    gterm_cursor_y = 0;
}

int cursor_visible = 1;
//int cursor_blink_state = 1;  // 1 = visible, 0 = hidden
//uint32_t cursor_blink_timer = 0;
//#define CURSOR_BLINK_INTERVAL 500  // milliseconds

// Original character under cursor (for restoring when cursor moves)
char cursor_char_under = ' ';

// Draw the cursor at the current position
void gterminal_draw_cursor() {
    if (cursor_visible) {
        // Save original character under cursor (if needed)
        // cursor_char_under is stored elsewhere

        // Draw cursor as a solid block or underline
        int cursor_x = gterm_cursor_x * 8;
        int cursor_y = gterm_cursor_y * loaded_font.glyph_size;

        // Block cursor (full character)
        for (int y = 0; (uint32_t)y < loaded_font.glyph_size; ++y) {
            for (int x = 0; (uint32_t)x < 8; ++x) {
                draw_pixel(cursor_x + x, cursor_y + y, gterminal_color);
            }
        }

        // Alternative: Underline cursor (only bottom row)
        /*
        for (int x = 0; x < 8; ++x) {
            draw_pixel(cursor_x + x, cursor_y + loaded_font.glyph_size - 1, gterminal_color);
        }
        */
    }
}

// Hide the cursor by redrawing the original character
void gterminal_hide_cursor() {
    int cursor_x = gterm_cursor_x * 8;
    int cursor_y = gterm_cursor_y * loaded_font.glyph_size;

    // Clear cursor area
    for (int y = 0; (uint32_t)y < loaded_font.glyph_size; ++y) {
        for (int x = 0; (uint32_t)x < 8; ++x) {
            draw_pixel(cursor_x + x, cursor_y + y, gterminal_bgcolor);
        }
    }

    // Redraw the character that was under the cursor
    if (cursor_char_under != ' ') {
        draw_char_psf(cursor_x, cursor_y, cursor_char_under, gterminal_color);
    }
}

// Modified putchar to handle cursor
void gterminal_putchar(char c) {
    // Hide cursor before modifying text
    gterminal_hide_cursor();

    if (c == '\n') {
        gterm_cursor_x = 0;
        gterm_cursor_y++;
        if (gterm_cursor_y >= GTERM_ROWS) {
            gterminal_scroll();
        }
    } else if (c == '\b') {
        if (gterm_cursor_x > 0) {
            gterm_cursor_x--;
        } else if (gterm_cursor_y > 0) {
            gterm_cursor_y--;
            gterm_cursor_x = GTERM_COLS - 1;
        }
        // Clear the character visually
        for (int y = 0; (uint32_t)y < loaded_font.glyph_size; ++y) {
            for (int x = 0; x < 8; ++x) {
                draw_pixel(
                    gterm_cursor_x * 8 + x,
                    gterm_cursor_y * loaded_font.glyph_size + y,
                    gterminal_bgcolor
                );
            }
        }
        cursor_char_under = ' ';
    } else {
        draw_char_psf(
            gterm_cursor_x * 8,
            gterm_cursor_y * loaded_font.glyph_size,
            c,
            gterminal_color
        );
        gterm_cursor_x++;
        if (gterm_cursor_x >= GTERM_COLS) {
            gterm_cursor_x = 0;
            gterm_cursor_y++;
            if (gterm_cursor_y >= GTERM_ROWS) {
                gterminal_scroll();
            }
        }
    }

    // Update character under cursor
    cursor_char_under = ' ';  // Default is space, should be whatever is at current position

    // Draw cursor at new position
    //cursor_blink_state = 1;  // Reset blink state
    //cursor_blink_timer = 0;  // Reset blink timer (you'll need to implement this)
    gterminal_draw_cursor();
}

void gterminal_set_colors(uint32_t fg, uint32_t bg) {
    gterminal_color = fg;
    gterminal_bgcolor = bg;
}

void gterminal_writestring(const char* str) {
    while (*str) {
        gterminal_putchar(*str++);
    }
}

uint32_t gterminal_getfg() {
    return gterminal_color;
}

uint32_t gterminal_getbg() {
    return gterminal_bgcolor;
}

uint32_t hex_to_uint32(const char* hex) {
    uint32_t color = 0;

    // Ensure the string starts with '#' and has 7 characters
    if (hex[0] == '#' && strlen(hex) == 7) {
        for (int i = 1; i < 7; ++i) {
            color *= 16; // Shift left by 4 bits for each new digit
            if (hex[i] >= '0' && hex[i] <= '9') {
                color += hex[i] - '0';
            } else if (hex[i] >= 'a' && hex[i] <= 'f') {
                color += hex[i] - 'a' + 10;
            } else if (hex[i] >= 'A' && hex[i] <= 'F') {
                color += hex[i] - 'A' + 10;
            }
        }
    }

    return color;
}

int load_theme(const char* path, theme_t* theme) {
    FIL file;
    UINT read;
    char buffer[256];
    char line[256];
    int line_index = 0;
    gterminal_writestring(path);
    gterminal_writestring("\n");
    // Open the theme file
    if (f_open(&file, path, FA_READ) != FR_OK) {
        gterminal_writestring("couldnt open file\n");
        // Default theme if the file cannot be opened
        theme->black = COLOR_BLACK;
        theme->red = COLOR_RED;
        theme->green = COLOR_GREEN;
        theme->yellow = COLOR_YELLOW;
        theme->blue = COLOR_BLUE;
        theme->magenta = COLOR_MAGENTA;
        theme->cyan = COLOR_CYAN;
        theme->white = COLOR_WHITE;
        theme->gray = COLOR_GRAY;

        theme->light_black = COLOR_LIGHT_BLACK;
        theme->light_red = COLOR_LIGHT_RED;
        theme->light_green = COLOR_LIGHT_GREEN;
        theme->light_yellow = COLOR_LIGHT_YELLOW;
        theme->light_blue = COLOR_LIGHT_BLUE;
        theme->light_magenta = COLOR_LIGHT_MAGENTA;
        theme->light_cyan = COLOR_LIGHT_CYAN;
        theme->light_white = COLOR_LIGHT_WHITE;
        theme->light_gray = COLOR_LIGHT_GRAY;
        return -1; // File not found
    }

    // Default values for theme colors
    theme->black = COLOR_BLACK;
    theme->red = COLOR_RED;
    theme->green = COLOR_GREEN;
    theme->yellow = COLOR_YELLOW;
    theme->blue = COLOR_BLUE;
    theme->magenta = COLOR_MAGENTA;
    theme->cyan = COLOR_CYAN;
    theme->white = COLOR_WHITE;
    theme->gray = COLOR_GRAY;

    theme->light_black = COLOR_LIGHT_BLACK;
    theme->light_red = COLOR_LIGHT_RED;
    theme->light_green = COLOR_LIGHT_GREEN;
    theme->light_yellow = COLOR_LIGHT_YELLOW;
    theme->light_blue = COLOR_LIGHT_BLUE;
    theme->light_magenta = COLOR_LIGHT_MAGENTA;
    theme->light_cyan = COLOR_LIGHT_CYAN;
    theme->light_white = COLOR_LIGHT_WHITE;
    theme->light_gray = COLOR_LIGHT_GRAY;

    // Read the file content into the buffer and process line by line
    while (f_read(&file, buffer, sizeof(buffer), &read) == FR_OK && read > 0) {
        for (int i = 0; i < read; i++) {
            // If we've encountered a newline or EOF, process the line
            if (buffer[i] == '\n' || buffer[i] == '\r') {
                if (line_index > 0) {
                    line[line_index] = '\0'; // Null-terminate the line
                    line_index = 0;

                    // Skip comments

                    // Parse each line and update the theme
                    if (strncmp(line, "black=", 6) == 0) {
                        theme->black = hex_to_uint32(line + 6);
                    } else if (strncmp(line, "red=", 4) == 0) {
                        theme->red = hex_to_uint32(line + 4);
                    } else if (strncmp(line, "green=", 6) == 0) {
                        theme->green = hex_to_uint32(line + 6);
                    } else if (strncmp(line, "yellow=", 7) == 0) {
                        theme->yellow = hex_to_uint32(line + 7);
                    } else if (strncmp(line, "blue=", 5) == 0) {
                        theme->blue = hex_to_uint32(line + 5);
                    } else if (strncmp(line, "magenta=", 8) == 0) {
                        theme->magenta = hex_to_uint32(line + 8);
                    } else if (strncmp(line, "cyan=", 5) == 0) {
                        theme->cyan = hex_to_uint32(line + 5);
                    } else if (strncmp(line, "white=", 6) == 0) {
                        theme->white = hex_to_uint32(line + 6);
                    }  else if (strncmp(line, "gray=", 5) == 0) {
                        theme->gray = hex_to_uint32(line + 5);
                    }

                    // Light variants
                    else if (strncmp(line, "light_black=", 12) == 0) {
                        theme->light_black = hex_to_uint32(line + 12);
                    } else if (strncmp(line, "light_red=", 10) == 0) {
                        theme->light_red = hex_to_uint32(line + 10);
                    } else if (strncmp(line, "light_green=", 12) == 0) {
                        theme->light_green = hex_to_uint32(line + 12);
                    } else if (strncmp(line, "light_yellow=", 13) == 0) {
                        theme->light_yellow = hex_to_uint32(line + 13);
                    } else if (strncmp(line, "light_blue=", 11) == 0) {
                        theme->light_blue = hex_to_uint32(line + 11);
                    } else if (strncmp(line, "light_magenta=", 14) == 0) {
                        theme->light_magenta = hex_to_uint32(line + 14);
                    } else if (strncmp(line, "light_cyan=", 11) == 0) {
                        theme->light_cyan = hex_to_uint32(line + 11);
                    } else if (strncmp(line, "light_white=", 12) == 0) {
                        theme->light_white = hex_to_uint32(line + 12);
                    } else if (strncmp(line, "light_gray=", 11) == 0) {
                        theme->light_gray = hex_to_uint32(line + 11);
                    }
                }
            } else {
                // Accumulate characters in the line buffer
                if (line_index < sizeof(line) - 1) {
                    line[line_index++] = buffer[i];
                }
            }
        }
    }

    // Close the file
    f_close(&file);

    return 0; // Success
}
