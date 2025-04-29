#include <stdint.h>
#include "tss.h"
#include "gdt.h"
#include "util.h"
#include "vga_driver.h"
#include "string.h"

struct tss_entry tss;

void tss_flush() {
    asm volatile("mov $0x2B, %ax\n"  // 0x2B = GDT index 5, RPL=3
                 "ltr %ax");
}

void tss_init(uint32_t kernel_stack) {
    memset(&tss, 0, sizeof(tss));
    
    tss.ss0 = 0x10;  // Kernel data segment
    tss.esp0 = kernel_stack;
    
    // Set the privilege level change bits
    tss.cs = 0x0B;      // User code segment with RPL 3
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x13; // User data segment with RPL 3
    
    tss_flush();
    
    terminal_writestring("TSS initialized with kernel stack at ");
    terminal_hexprint(kernel_stack);
    terminal_writestring("\n");
}