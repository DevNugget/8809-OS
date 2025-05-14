#include <stdint.h>
#include "task.h"
#include "vga_driver.h"

void switch_to_user_mode_elf(uint32_t entry_point) {
    uint32_t eflags;
    asm volatile("pushfl; popl %0" : "=r"(eflags));
    //terminal_writestring("Current EFLAGS: ");
    //terminal_hexprint(eflags);
    //terminal_writestring("\n");

    asm volatile(
        "cli\n"
        "mov $0x23, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        "pushl $0x23\n"
        "pushl $0xBFFFF000\n"
        "pushfl\n"
        "popl %%eax\n"
        "orl $0x200, %%eax\n"
        "pushl %%eax\n"
        "pushl $0x1B\n"
        "pushl %[entry]\n"
        "iret\n"
        :
        : [entry] "r"(entry_point)
        : "eax", "memory"
    );
}


void switch_to_user_mode() {
    // Debug current state
    uint32_t eflags;
    asm volatile("pushfl; popl %0" : "=r"(eflags));
    //terminal_writestring("Current EFLAGS: ");
    //terminal_hexprint(eflags);
    //terminal_writestring("\n");

    asm volatile(
        "cli\n"
        // Set up user data segments
        "mov $0x23, %%ax\n"   // User data segment (RPL=3)
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        // Set up stack frame for iret
        "pushl $0x23\n"       // User data segment
        "pushl $0xBFFFF000\n" // User stack pointer
        "pushfl\n"            // EFLAGS
        "popl %%eax\n"        // Get current EFLAGS
        "orl $0x200, %%eax\n" // Set interrupt flag
        "pushl %%eax\n"       // Push back modified EFLAGS
        "pushl $0x1B\n"       // User code segment (RPL=3)
        "pushl $0x400000\n"   // User code entry point
        "iret\n"
        ::: "eax", "memory"
    );
}

void return_to_usermode(uint32_t eip) {
    // Debug current state
    uint32_t eflags;
    asm volatile("pushfl; popl %0" : "=r"(eflags));
    terminal_writestring("Current EFLAGS: ");
    terminal_hexprint(eflags);
    terminal_writestring("\n");

    terminal_writestring("Returning to usermode at EIP: ");
    terminal_hexprint(eip);
    terminal_writestring("\n");

    asm volatile(
        "cli\n"
        // Set up user data segments
        "mov $0x23, %%ax\n"   // User data segment (RPL=3)
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        // Set up stack frame for iret
        "pushl $0x23\n"       // User data segment
        "pushl $0xBFFFF000\n" // User stack pointer
        "pushfl\n"            // EFLAGS
        "popl %%eax\n"        // Get current EFLAGS
        "orl $0x200, %%eax\n" // Set interrupt flag
        "pushl %%eax\n"       // Push back modified EFLAGS
        "pushl $0x1B\n"       // User code segment (RPL=3)
        "pushl %0\n"          // User code entry point (corrected)
        "iret\n"
        :
        : "r" (eip)           // No more +1
        : "eax", "memory"
    );
}
