#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT Entry (Gate Descriptor)
struct idt_entry {
    uint16_t base_low;    // Lower 16 bits of ISR address
    uint16_t selector;    // Kernel code segment selector (0x08)
    uint8_t  zero;        // Always 0
    uint8_t  flags;       // Flags (P=1, DPL=00b, Type=0xE for interrupt gate)
    uint16_t base_high;   // Upper 16 bits of ISR address
} __attribute__((packed));

// IDT Pointer (for LIDT)
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct regs
{
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

// Public functions
void idt_init();          // Sets up the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);  // Configures an IDT entry

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void isrs_install();

extern void irq0(); 
extern void irq1();  
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void irq_install();
void irq_handler(struct regs *r);

void irq_install_handler(int irq, void (*handler)(struct regs *r));

#endif
