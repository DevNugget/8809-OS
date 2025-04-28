#include "vga_driver.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard_driver.h"
#include "pmm.h"
#include "util.h"
#include "multiboot.h"

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    terminal_init();
    gdt_init();
    idt_init();
    asm volatile("sti");
    terminal_writestring("Interrupts enabled\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("8809 OS\n");

    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        terminal_writestring("Invalid magic number!");
        while(1) {}
    }

    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        terminal_writestring("Invalid memory map given by GRUB bootloader");
        while(1) {}
    }

    pmm_init(mbd);

    for (int i = 0; i < 5; i++) {
        uint32_t frame = pmm_alloc_frame();
        terminal_writestring("Allocated frame at ");
        terminal_hexprint(frame);
        terminal_writestring("\n");
        pmm_print_status();
    }

    keyboard_init();
    // Testing keyboard driver
    while (1) {
        if (inportb(0x64) & 0x01) {  // Check keyboard status
            keyboard_getchar();
        }
    }
}