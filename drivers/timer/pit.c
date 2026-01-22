#include "pit.h"

#define PIT_FREQUENCY 1193182

void pit_irq_handler(struct regs* regs) {
    schedule_next();

    outportb(0x20, 0x20);
}

void pit_init(uint32_t frequency) {
    uint32_t divisor = PIT_FREQUENCY / frequency;

    outportb(0x43, 0x36);            // Command byte
    outportb(0x40, divisor & 0xFF);  // Low byte
    outportb(0x40, divisor >> 8);    // High byte
    
    irq_install_handler(0, pit_irq_handler);
}


