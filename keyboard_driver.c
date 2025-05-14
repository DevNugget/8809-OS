#include <stdbool.h>
#include <stdint.h>

#include "keyboard_driver.h"
#include "vga_driver.h"
#include "util.h"

#define KEYBOARD_DATA_PORT 0x60

static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/',
    0, '*', 0, ' ', 0,
    // Fill in more if needed
};

static volatile char current_key = 0;
static volatile int waiting_for_key = 0;

#define KEYBOARD_BUFFER_SIZE 128
static char key_buffer[KEYBOARD_BUFFER_SIZE];
static volatile int buffer_head = 0;
static volatile int buffer_tail = 0;

static int buffer_is_empty() {
    return buffer_head == buffer_tail;
}

static int buffer_is_full() {
    return ((buffer_head + 1) % KEYBOARD_BUFFER_SIZE) == buffer_tail;
}

static void buffer_put(char c) {
    if (!buffer_is_full()) {
        key_buffer[buffer_head] = c;
        buffer_head = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    }
}

static char buffer_get() {
    if (buffer_is_empty()) return 0;
    char c = key_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

static void keyboard_callback(struct regs *regs) {
    uint8_t scancode = inportb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) {
        // Key release, ignore
        return;
    }

    char c = scancode_to_ascii[scancode];
    if (c) {
        buffer_put(c);
    }

    (void)regs;
}

void keyboard_init() {
    irq_install_handler(1, keyboard_callback);
}

char keyboard_getchar() {
    // Wait for a character to appear in the buffer
    while (buffer_is_empty()) {
        // Optionally add sleep/pause to reduce CPU usage
        //asm volatile ("hlt");
    }

    return buffer_get();
}
