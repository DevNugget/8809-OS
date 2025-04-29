#include <stdint.h>
#include "gdt.h"
#include "vga_driver.h"
#include "tss.h"

extern void gdt_load();

// GDT Entry structure (8 bytes)
struct gdt_entry {
    uint16_t limit_low;    // Lower 16 bits of segment limit
    uint16_t base_low;     // Lower 16 bits of segment base
    uint8_t  base_middle;  // Next 8 bits of segment base
    uint8_t  access;       // Access flags (present, privilege, type)
    uint8_t  granularity;  // Flags + upper 4 bits of limit
    uint8_t  base_high;    // Last 8 bits of segment base
} __attribute__((packed));

// GDT Descriptor (pointer for LGDT)
struct gdt_ptr {
    uint16_t limit;  // Size of GDT - 1
    uint32_t base;   // Start address of GDT
} __attribute__((packed));

// Define the GDT (3 entries: NULL, Code, Data)
struct gdt_entry gdt[6];
struct gdt_ptr gdt_ptr;

// Set a GDT entry
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt[index].access = access;
}

// Initialize the GDT
void gdt_init() {
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint32_t) &gdt;

    // NULL descriptor (required)
    gdt_set_entry(0, 0, 0, 0, 0);

    // Kernel Code Segment (Ring 0)
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Kernel Data Segment (Ring 0)
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // User Code Segment (Ring 3)
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // User Data Segment (Ring 3)
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_set_entry(5, (uint32_t)&tss, sizeof(tss), 0xE9, 0x00);

    // Load the GDT (requires assembly)
    gdt_load();

    terminal_writestring("GDT initialized\n");
}
