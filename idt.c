#include <stdint.h>

#include "idt.h"
#include "vga_driver.h"
#include "util.h"
#include "task.h"

struct idt_entry idt[256];
struct idt_ptr idt_ptr;

extern void idt_load();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].zero = 0;
    idt[num].selector = sel;
    idt[num].flags = flags;
}

void isrs_install()
{
    idt_set_gate(0,  (uint32_t)isr0, 0x08,  0x8E);
    idt_set_gate(1,  (uint32_t)isr1, 0x08,  0x8E);
    idt_set_gate(2,  (uint32_t)isr2, 0x08,  0x8E);
    idt_set_gate(3,  (uint32_t)isr3, 0x08,  0x8E);
    idt_set_gate(4,  (uint32_t)isr4, 0x08,  0x8E);
    idt_set_gate(5,  (uint32_t)isr5, 0x08,  0x8E);
    idt_set_gate(6,  (uint32_t)isr6, 0x08,  0x8E);
    idt_set_gate(7,  (uint32_t)isr7, 0x08,  0x8E);

    idt_set_gate(8,  (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);

    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);

    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
    idt_set_gate(0x80, (uint32_t)isr128, 0x08, 0xEE); 
}

void idt_init() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    isrs_install();
    irq_install();

    idt_load();

    terminal_writestring("IDT initialized\n");
}

const char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void isr_handler(struct regs *r) {
    if (r->int_no < 32)
    {
        terminal_setcolor(VGA_COLOR_RED);
        terminal_writestring(exception_messages[r->int_no]);
        terminal_writestring(" Exception. System Halted!\n");
        for (;;);
    }
}

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r)) {
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
void irq_remap(void) {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_install() {
    irq_remap();

    idt_set_gate(32, (unsigned)irq0,  0x08, 0x8E | 0x60);  // IRQ0  (Timer)
    idt_set_gate(33, (unsigned)irq1,  0x08, 0x8E);  // IRQ1  (Keyboard)
    idt_set_gate(34, (unsigned)irq2,  0x08, 0x8E);  // IRQ2  (Cascade)
    idt_set_gate(35, (unsigned)irq3,  0x08, 0x8E);  // IRQ3  (COM2)
    idt_set_gate(36, (unsigned)irq4,  0x08, 0x8E);  // IRQ4  (COM1)
    idt_set_gate(37, (unsigned)irq5,  0x08, 0x8E);  // IRQ5  (LPT2)
    idt_set_gate(38, (unsigned)irq6,  0x08, 0x8E);  // IRQ6  (Floppy)
    idt_set_gate(39, (unsigned)irq7,  0x08, 0x8E);  // IRQ7  (LPT1)
    idt_set_gate(40, (unsigned)irq8,  0x08, 0x8E);  // IRQ8  (RTC)
    idt_set_gate(41, (unsigned)irq9,  0x08, 0x8E);  // IRQ9  (ACPI)
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);  // IRQ10 (Open)
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);  // IRQ11 (Open)
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);  // IRQ12 (PS2 Mouse)
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);  // IRQ13 (FPU)
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);  // IRQ14 (Primary ATA)
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);  // IRQ15 (Secondary ATA)
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void irq_handler(struct regs *r) {
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
}
