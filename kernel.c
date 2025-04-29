#include "vga_driver.h"
#include "gdt.h"
#include "tss.h"
#include "idt.h"
#include "keyboard_driver.h"
#include "pmm.h"
#include "vmm.h"
#include "util.h"
#include "task.h"
#include "multiboot.h"
#include "fatfs/ff.h"

FATFS fat_fs;
FIL file;

typedef void (*module_entry_t)(void);

void execute_module(multiboot_module_t* mod) {
    // 1. Verify module is in accessible memory
    if (mod->mod_start < 0x100000 || mod->mod_end > 0x200000) {
        terminal_writestring("Module in invalid memory range\n");
        return;
    }

    // 2. Print module info
    terminal_writestring("Executing module at ");
    terminal_hexprint(mod->mod_start);
    terminal_writestring("-");
    terminal_hexprint(mod->mod_end);
    terminal_writestring(" (");
    terminal_decprint(mod->mod_end - mod->mod_start);
    terminal_writestring(" bytes)\n");

    // 3. Cast module start address to function pointer
    module_entry_t module_entry = (module_entry_t)mod->mod_start;

    // 4. Debug: Print first few instructions
    terminal_writestring("First 8 bytes: ");
    for (int i = 0; i < 8; i++) {
        terminal_hexprint(*((uint8_t*)mod->mod_start + i));
        terminal_writestring(" ");
    }
    terminal_writestring("\n");

    // 5. Execute the module
    terminal_writestring("Calling module entry...\n");
    //module_entry();
    terminal_writestring("Module execution completed\n");
}

void execute_user_module(multiboot_module_t* mod) {
    // 1. Map user code to 0x400000
    uint32_t user_code_pages = ((mod->mod_end - mod->mod_start) + 4095) / 4096;
    for (uint32_t i = 0; i < user_code_pages; i++) {
        vmm_map_page(0x400000 + (i * 4096),
                    mod->mod_start + (i * 4096),
                    PAGE_PRESENT | PAGE_USER | PAGE_EXECUTABLE);
    }

    // 2. Map user stack (8KB) at 0xBFFFF000
    uint32_t stack_frame = pmm_alloc_frame();
    vmm_map_page(0xBFFFF000 - 4096, stack_frame, 
                PAGE_PRESENT | PAGE_USER | PAGE_RW);
    vmm_map_page(0xBFFFF000, pmm_alloc_frame(),
                PAGE_PRESENT | PAGE_USER | PAGE_RW);

    // 3. Debug output
    terminal_writestring("User program mapped at 0x400000\n");
    terminal_writestring("User stack at 0xBFFFF000\n");

    // 4. Switch to user mode
    switch_to_user_mode();
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    terminal_init();
    gdt_init();
    idt_init();

    extern uint32_t stack_top;
    tss_init((uint32_t)&stack_top);
    
    asm volatile("sti");
    terminal_writestring("Interrupts enabled\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("8809 OS\n");

    pmm_init(mbd, magic);
    
    for (int i = 0; i < 5; i++) {
        uint32_t frame = pmm_alloc_frame();
        terminal_writestring("Allocated frame at ");
        terminal_hexprint(frame);
        terminal_writestring("\n");
        pmm_print_status();
    }

    vmm_init();
    //vmm_map_page(0xC0100000, 0x200000, PAGE_PRESENT | PAGE_RW);

    FRESULT res = f_mount(&fat_fs, "", 1); // 1 = mount now
        if (res != FR_OK) {
        terminal_writestring("Failed to mount filesystem\n");
        return;
    }
    terminal_writestring("Fatfs mounted successfully\n");

    FRESULT res_fr = f_open(&file, "program.bin", FA_READ);
    if (res_fr != FR_OK) {
        terminal_writestring("f_open failed: ");
        terminal_decprint(res_fr); // Will print FatFs error code (e.g. 1 = FR_DISK_ERR)
        terminal_writestring("\n");
        return;
    }
    terminal_writestring("program.bin opened successfully\n");

    // 2. Prepare user space area
    UINT file_size = f_size(&file);
    UINT user_addr = 0x400000;
    UINT user_pages = (file_size + 4095) / 4096;

    for (uint32_t i = 0; i < user_pages; i++) {
        uint32_t frame = pmm_alloc_frame();
        vmm_map_page(user_addr + (i * 4096), frame,
                     PAGE_PRESENT | PAGE_USER | PAGE_RW | PAGE_EXECUTABLE);
    }

    // 3. Read directly into user memory
    UINT bytes_read;
    res = f_read(&file, (void*)user_addr, file_size, &bytes_read);
    f_close(&file);

    terminal_writestring("Loaded ");
    terminal_decprint(bytes_read);
    terminal_writestring(" bytes from file to user memory\n");

    // 4. Set up stack and jump to user mode
    vmm_map_page(0xBFFFF000 - 4096, pmm_alloc_frame(), PAGE_PRESENT | PAGE_USER | PAGE_RW);
    vmm_map_page(0xBFFFF000, pmm_alloc_frame(), PAGE_PRESENT | PAGE_USER | PAGE_RW);

    switch_to_user_mode(); // or switch_to_user_mode_with_entry(0x400000);
    //extern uint32_t __module_space_start;
    //extern uint32_t __module_space_end;

    //if (mbd->flags & (1 << 3)) {
    //    multiboot_module_t* mods = (multiboot_module_t*)mbd->mods_addr;
    //    if (mbd->mods_count > 0) {
    //        execute_user_module(&mods[0]);
    //    }
    //}
/*
    terminal_writestring("First 8 bytes at 0x400000: ");
    for (int i = 0; i < 8; i++) {
        terminal_hexprint(*((uint8_t*)0x400000 + i));
        terminal_writestring(" ");
    }
    terminal_writestring("\n");
*/
    /*unsigned int address_of_module = mbd->mods_addr;
    terminal_hexprint(address_of_module);
    terminal_writestring("\n");
    terminal_hexprint(mbd->mods_count);
    //switch_to_user_mode();

    */
   
    /*
    // Map virtual 0xC0000000 → physical 1MB
    vmm_map_page(0xC0100000, 0x200000, PAGE_PRESENT | PAGE_RW);  // Map virtual 0xC0100000 → physical 2MB
    vmm_map_page(0xC0600000, 0x300000, PAGE_PRESENT);

     uint32_t *test_ptr = (uint32_t*)0xC0100000;
    *test_ptr = 0xDEADBEEF;
    terminal_writestring("Write to 0xC0200000 successful!\n");
    terminal_writestring("Read back: ");
    terminal_hexprint(*test_ptr);
    terminal_writestring("\n");

    asm volatile("cli");
    switch_to_user_mode();
    */

    //uint32_t *test_ptr_2 = (uint32_t*)0xC0300000;
    //terminal_hexprint(*test_ptr_2);x
    //terminal_writestring("\n");

    /*
    keyboard_init();
    // Testing keyboard driver
    while (1) {
        if (inportb(0x64) & 0x01) {  // Check keyboard status
            keyboard_getchar();
        }
    }
    */
    //setup_user_environment();
   
}
