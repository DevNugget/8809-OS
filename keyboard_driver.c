#include <stdbool.h>

#include "keyboard_driver.h"
#include "vga_driver.h"
#include "util.h"

static bool shift_down = false;
static bool caps_lock = false;
static bool ctrl_down = false;
static bool alt_down = false;

// US QWERTY scancode table
static const char scancode_table[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

static const char shift_scancode_table[] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2', '3', '0', '.'
};

#define INPUT_BUFFER_SIZE 256
static char input_buffer[INPUT_BUFFER_SIZE];

static uint32_t buffer_start = 0;
static uint32_t buffer_end = 0;

static void keyboard_buffer_push(char c) {
    if ((buffer_end + 1) % INPUT_BUFFER_SIZE == buffer_start) return;  // Buffer full
    input_buffer[buffer_end] = c;
    buffer_end = (buffer_end + 1) % INPUT_BUFFER_SIZE;
}

char keyboard_getchar() {
    while (buffer_start == buffer_end) { asm volatile("hlt"); }  // Wait for input
    char c = input_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % INPUT_BUFFER_SIZE;
    return c;
}

void keyboard_handler(struct regs *r) {
    uint8_t scancode = inportb(0x60);
    
    // Handle key release and modifier keys
    switch (scancode) {
        case 0x2A: shift_down = true; break;     // Left Shift press
        case 0xAA: shift_down = false; break;    // Left Shift release
        case 0x3A: caps_lock = !caps_lock; break; // Caps Lock toggle
        case 0x1D: ctrl_down = true; break;      // Ctrl press
        case 0x9D: ctrl_down = false; break;     // Ctrl release
        case 0x38: alt_down = true; break;       // Alt press
        case 0xB8: alt_down = false; break;      // Alt release
    }

    // Ignore key releases (except modifiers above)
    if (scancode & 0x80) return;

    // Translate scancode to ASCII
    char c = 0;
    if (shift_down || caps_lock) {
        c = shift_scancode_table[scancode];
    } else {
        c = scancode_table[scancode];
    }

    // Handle special keys
    if (ctrl_down && c == 'c') {
        terminal_writestring("^C");
        // TODO: Send SIGINT to shell
        return;
    }

    if (c == '\b') {
        if (buffer_start != buffer_end) {
            buffer_end = (buffer_end - 1) % INPUT_BUFFER_SIZE;
        }
    }

    if (c) {
        keyboard_buffer_push(c);  // Store for shell
        terminal_putchar(c);      // Echo to screen
    }
}

void keyboard_init() {
    irq_install_handler(1, keyboard_handler);  // Register IRQ1 handler
    terminal_writestring("Keyboard driver initialized\n");
}