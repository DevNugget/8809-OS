// keyboard_driver.c
// Keyboard driver — supports Shift, CapsLock, Ctrl (left+right), arrow keys,
// and emits UNIX-style control characters for Ctrl+letter combos.

#include <stdbool.h>
#include <stdint.h>
#include "keyboard_driver.h"
#include "vga_driver.h"
#include "util.h"

#define KEYBOARD_DATA_PORT 0x60

// Public special key values
#define KEY_UP    0x81
#define KEY_DOWN  0x82
#define KEY_LEFT  0x83
#define KEY_RIGHT 0x84

// === Scancode -> ASCII tables (Set 1) ===
static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/',
    0, '*', 0, ' ', 0, // up to 0x3F
    // Fill remainder with zeros to 128 entries
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const char scancode_shifted[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t', 'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?',
    0, '*', 0, ' ', 0,
    // remainder zeros
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// === State flags ===
static volatile bool shift_pressed = false;
static volatile bool caps_lock     = false;
static volatile bool ctrl_pressed  = false;
static volatile bool extended      = false;

// === Keyboard ring buffer ===
#define KEYBOARD_BUFFER_SIZE 256
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

// === Helper: map scancode to char with shift/caps applied ===
static char map_scancode_to_char(uint8_t sc) {
    char c = shift_pressed ? scancode_shifted[sc] : scancode_to_ascii[sc];
    // Caps lock affects letters only (and should be XOR with shift)
    if (c >= 'a' && c <= 'z') {
        if (shift_pressed ^ caps_lock) c -= 32;
    } else if (c >= 'A' && c <= 'Z') {
        // if shift not pressed and caps is on, force lowercase? (handled above)
    }
    return c;
}

// === IRQ handler ===
static void keyboard_callback(struct regs *regs) {
    uint8_t sc = inportb(KEYBOARD_DATA_PORT);

    // Extended prefix (0xE0) handling
    if (sc == 0xE0) {
        extended = true;
        return;
    }

    bool release = sc & 0x80;
    sc &= 0x7F; // index scancode without release bit

    // Right-Ctrl comes as E0 1D; handle Ctrl for both left and right
    if (extended && sc == 0x1D) {
        ctrl_pressed = !release;
        extended = false; // consumed extended
        return;
    }

    // Left Ctrl scancode (non-extended)
    if (!extended && sc == 0x1D) {
        ctrl_pressed = !release;
        return;
    }

    // Shift keys (left: 0x2A, right: 0x36)
    if (sc == 0x2A || sc == 0x36) {
        shift_pressed = !release;
        extended = false;
        return;
    }

    // Caps Lock (toggle on key press)
    if (sc == 0x3A && !release) {
        caps_lock = !caps_lock;
        extended = false;
        return;
    }

    // If this is a release event for other keys, just clear extended and ignore
    if (release) {
        extended = false;
        return;
    }

    // Handle extended arrow keys (E0 48/50/4B/4D)
    if (extended) {
        extended = false;
        if (sc == 0x48) { buffer_put((char)KEY_UP); return; }
        if (sc == 0x50) { buffer_put((char)KEY_DOWN); return; }
        if (sc == 0x4B) { buffer_put((char)KEY_LEFT); return; }
        if (sc == 0x4D) { buffer_put((char)KEY_RIGHT); return; }
        // Unknown extended key: ignore for now
        return;
    }

    // Map to character using scancode tables
    char c = map_scancode_to_char(sc);
    if (!c) return;

    // If Ctrl is held and the character is a letter, emit control code:
    // Ctrl+letter => ASCII 0x01..0x1A (same as Unix terminals)
    if (ctrl_pressed) {
        // Normalize to lowercase letter for mapping
        if (c >= 'A' && c <= 'Z') {
            c = (c - 'A') + 'a';
        }
        if (c >= 'a' && c <= 'z') {
            c = (char)( (c - 'a') + 1 ); // Ctrl+A=1 ... Ctrl+Z=26
            buffer_put(c);
            return;
        }
        // Optional: support Ctrl+@ (NUL), Ctrl+[, Ctrl+\ etc if desired.
    }

    // Normal printable character -> push to buffer
    buffer_put(c);
    (void)regs;
}

// === Public API ===
void keyboard_init() {
    irq_install_handler(1, keyboard_callback);
}

char keyboard_getchar() {
    while (buffer_is_empty()) asm volatile ("hlt");
    return buffer_get();
}
