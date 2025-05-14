#include <stdint.h>
#include "vmm.h"
#include "pmm.h"
#include "vga_driver.h"
#include "idt.h"
#include "string.h"

__attribute__((aligned(4096))) static uint32_t page_directory[1024];
__attribute__((aligned(4096))) static uint32_t first_page_table[1024];

void vmm_identity_map_first_4mb() {
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW;
    }

    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;
}

void vmm_enable_paging() {
    asm volatile("mov %0, %%cr3" :: "r"(page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  // Set paging bit
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

int vmm_map_page(uint32_t virt_addr, uint32_t phys_addr, uint32_t flags) {
    // Validate addresses
    if (virt_addr < 0x1000 && virt_addr != 0) {  // Allow NULL mapping for special cases
        terminal_writestring("VMM_ERR_INVALID_VIRT_ADDR\n");
        return VMM_ERR_INVALID_ADDR;
    }

    // Allow physical addresses where GRUB loads modules
    if (phys_addr < 0x1000 && !(phys_addr >= 0x100000 && phys_addr < 0x200000)) {
        terminal_writestring("VMM_ERR_INVALID_PHYS_ADDR at ");
        terminal_hexprint(phys_addr);
        terminal_writestring("\n");
        return VMM_ERR_INVALID_ADDR;
    }

    uint32_t page_dir_index = virt_addr >> 22;
    uint32_t page_table_index = (virt_addr >> 12) & 0x3FF;

    // Check if page table exists
    if (page_directory[page_dir_index] & PAGE_PRESENT) {
        uint32_t* page_table = (uint32_t*)(page_directory[page_dir_index] & ~0xFFF);
        // Check if page is already mapped
        if (page_table[page_table_index] & PAGE_PRESENT) {
        	//terminal_writestring("ERROR DOUBLE MAP\n");
            return VMM_ERR_DOUBLE_MAP;
        }
    } else {
        // Allocate new page table
        uint32_t new_table_phys = pmm_alloc_frame();
        if (!new_table_phys) return VMM_ERR_NO_MEMORY;

        uint32_t* new_table = (uint32_t*)new_table_phys;
        pmm_memset(new_table, 0, 4096);
        page_directory[page_dir_index] = new_table_phys | PAGE_PRESENT | PAGE_RW | PAGE_USER;
    }

    // Get the page table and map the entry
    uint32_t* page_table = (uint32_t*)(page_directory[page_dir_index] & ~0xFFF);
    page_table[page_table_index] = (phys_addr & ~0xFFF) | (flags & (PAGE_PRESENT|PAGE_RW|PAGE_USER|PAGE_EXECUTABLE));

    asm volatile("invlpg (%0)" ::"r" (virt_addr) : "memory");
    //terminal_writestring("Mapped ");
    //terminal_hexprint(virt_addr);
    //terminal_writestring(" to ");
    //terminal_hexprint(phys_addr);
    //terminal_writestring("\n");
    return VMM_SUCCESS;
}

void page_fault_handler(struct regs *r) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // Extract page fault error code bits
    uint32_t present = !(r->err_code & 0x1); // Page not present
    uint32_t write = r->err_code & 0x2;      // Write operation?
    uint32_t user = r->err_code & 0x4;       // User mode?
    uint32_t reserved = r->err_code & 0x8;   // Overwritten CPU-reserved bits
    uint32_t id = r->err_code & 0x10;        // Instruction fetch?

    terminal_setcolor(VGA_COLOR_LIGHT_RED);
    terminal_writestring("\nPage Fault at ");
    terminal_hexprint(faulting_address);
    terminal_writestring(" [");

    if (present) terminal_writestring("(not present)");
    if (write) terminal_writestring("(write)");
    if (user) terminal_writestring("(user-mode)");
    if (reserved) terminal_writestring("(reserved)");
    if (id) terminal_writestring("(instruction-fetch)");

    terminal_writestring("]\n");
    terminal_setcolor(VGA_COLOR_WHITE);

    // For now just hang, but later you can implement:
    // - Demand paging
    // - Copy-on-write
    // - Stack growth
    while(1);
}

void vmm_init() {
	idt_set_gate(14, (uint32_t)page_fault_handler, 0x08, 0x8E);

	//uint32_t cr0;
    //asm volatile("mov %%cr0, %0" : "=r"(cr0));
    //asm volatile("mov %0, %%cr0" :: "r"(cr0 | 0x00010000)); // Set WP bit

    vmm_identity_map_first_4mb();
    vmm_enable_paging();
    terminal_writestring("Paging enabled.\n");
}

uint32_t vmm_create_user_directory() {
    uint32_t new_dir = (uint32_t)pmm_alloc_frame();
    uint32_t *dir = (uint32_t*)new_dir;

    // 1. Clear directory
    memset(dir, 0, 4096);

    // 2. Copy kernel mappings (last 256 entries)
    uint32_t current_cr3;
    asm volatile("mov %%cr3, %0" : "=r"(current_cr3));
    uint32_t *current_dir = (uint32_t*)current_cr3;
    memcpy(&dir[768], &current_dir[768], 1024);

    // 3. Self-map
    dir[1023] = new_dir | 0x7;  // Present + RW + User

    return new_dir;
}
