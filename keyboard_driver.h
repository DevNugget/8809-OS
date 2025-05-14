#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h>
#include "idt.h"

void keyboard_init();
static void keyboard_callback(struct regs *r);
char keyboard_getchar();  // Blocking read (for shell)

// Scancode translation states
#define KEY_RELEASED 0x80
#define SHIFT_DOWN   0x01
#define CAPS_LOCK    0x02

#endif
