#ifndef TSS_H
#define TSS_H

#include <stdint.h>

struct tss_entry {
    uint32_t prev_tss;   // Previous TSS link
    uint32_t esp0;       // Stack pointer for ring 0
    uint32_t ss0;        // Stack segment for ring 0
    uint32_t esp1;       // Stack pointer for ring 1 (unused)
    uint32_t ss1;        // Stack segment for ring 1 (unused)
    uint32_t esp2;       // Stack pointer for ring 2 (unused)
    uint32_t ss2;        // Stack segment for ring 2 (unused)
    uint32_t cr3;        // Page directory base
    uint32_t eip;        // Instruction pointer
    uint32_t eflags;     // Flags register
    uint32_t eax;        // General purpose registers
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;        // Stack pointer
    uint32_t ebp;        // Base pointer
    uint32_t esi;        // Source index
    uint32_t edi;        // Destination index
    uint32_t es;         // Extra segment
    uint32_t cs;         // Code segment
    uint32_t ss;         // Stack segment
    uint32_t ds;         // Data segment
    uint32_t fs;         // Additional data segment
    uint32_t gs;         // Additional data segment
    uint32_t ldt;        // Local descriptor table
    uint16_t trap;       // Trap flag
    uint16_t iomap_base; // I/O permission bitmap offset
} __attribute__((packed));

extern void tss_flush();
extern struct tss_entry tss;

void tss_init(uint32_t kernel_stack);

#endif
