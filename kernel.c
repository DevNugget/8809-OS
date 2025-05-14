#include "vga_driver.h"
#include "gdt.h"
#include "tss.h"
#include "idt.h"
#include "keyboard_driver.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "task.h"
#include "multiboot.h"
#include "fatfs/ff.h"
#include "elf32/elf32.h"
#include <stdint.h>

#define KHEAP_START 0xC1000000
#define KHEAP_INITIAL_SIZE (2 * 1024 * 1024)

FATFS fat_fs;
FIL file;

void map_framebuffer_pages(multiboot_info_t* mbd);
void* map_heap_pages();
void map_user_stack();
void mount_filesystem();

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    gterminal_init(mbd);
    gdt_init();
    idt_init();

    extern uint32_t stack_top;
    tss_init((uint32_t)&stack_top);

    asm volatile("sti");

    pmm_init(mbd, magic);

    for (int i = 0; i < 5; i++) {
        uint32_t frame = pmm_alloc_frame();
    }

    vmm_init();

    map_framebuffer_pages(mbd);

    heap_init(map_heap_pages(), KHEAP_INITIAL_SIZE);

    mount_filesystem();
    load_psf1_font("fonts/aply16.psf"); // Load default font
    uint32_t elf_entry = load_elf("bin/init.elf"); // Load the system init program
    map_user_stack();

    keyboard_init();

    switch_to_user_mode_elf(elf_entry);
}

/**
 * @brief Maps the linear framebuffer into virtual memory using identity mapping.
 *
 * This function calculates the total size of the framebuffer using pitch and height,
 * then identity maps each page (4KB) of the framebuffer into virtual memory with
 * read/write permissions.
 * 
 * @param mbd Pointer to the Multiboot information structure containing framebuffer info.
 */
void map_framebuffer_pages(multiboot_info_t* mbd) {
    uint32_t fb_addr = mbd->framebuffer_addr;
    uint32_t fb_size = mbd->framebuffer_pitch * mbd->framebuffer_height;

    for (uint32_t offset = 0; offset < fb_size; offset += 4096) {
        uint32_t phys = fb_addr + offset;
        uint32_t virt = phys; // identity map
        vmm_map_page(virt, phys, PAGE_PRESENT | PAGE_RW);
    }
}

/**
 * @brief Maps a range of virtual memory for the kernel heap.
 *
 * Allocates physical frames and maps them to a predefined virtual range
 * starting at KHEAP_START with a size of KHEAP_INITIAL_SIZE.
 *
 * @return void* Pointer to the base address of the heap (KHEAP_START).
 */
void* map_heap_pages() {
    void* heap_base = (void*)KHEAP_START;
    for (uint32_t addr = KHEAP_START; addr < KHEAP_START + KHEAP_INITIAL_SIZE; addr += 4096) {
        uint32_t frame = pmm_alloc_frame();
        vmm_map_page(addr, frame, PAGE_PRESENT | PAGE_RW);
    }

    return heap_base;
}

/**
 * @brief Maps the user-mode stack into virtual memory.
 * 
 * Maps two pages near the top of the user address space to serve as the
 * initial user-mode stack. The stack is mapped with user-mode and read/write permissions.
 */
void map_user_stack() {
    vmm_map_page(0xBFFFF000 - 4096, pmm_alloc_frame(), PAGE_PRESENT | PAGE_USER | PAGE_RW);
    vmm_map_page(0xBFFFF000, pmm_alloc_frame(), PAGE_PRESENT | PAGE_USER | PAGE_RW);
}

/**
 * @brief Mounts the root filesystem using the FatFs library.
 * 
 * Attempts to mount the filesystem on the default drive. If it fails,
 * prints an error message to the terminal.
 */
void mount_filesystem() {
    FRESULT res = f_mount(&fat_fs, "", 1); // 1 = mount now
    if (res != FR_OK) {
        terminal_writestring("Failed to mount filesystem\n");
        return;
    }
}
