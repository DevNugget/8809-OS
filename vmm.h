// VIRTUAL MEMORY ALLOCATOR (VMM)
#ifndef VMM_H
#define VMM_H

#define PAGE_PRESENT     (1 << 0)
#define PAGE_RW          (1 << 1)
#define PAGE_USER        (1 << 2)
#define PAGE_WRITETHROUGH (1 << 3)
#define PAGE_CACHE_DISABLE (1 << 4)
#define PAGE_EXECUTABLE (1 << 0)  // Bit 63 in PTE (NX bit)

#define VMM_SUCCESS 0
#define VMM_ERR_DOUBLE_MAP 1
#define VMM_ERR_INVALID_ADDR 2
#define VMM_ERR_NO_MEMORY 3

#include <stdint.h>
#include "pmm.h"
#include "vga_driver.h"

__attribute__((aligned(4096))) static uint32_t page_directory[1024];
__attribute__((aligned(4096))) static uint32_t first_page_table[1024];

void vmm_init();
int vmm_map_page(uint32_t virt_addr, uint32_t phys_addr, uint32_t flags);
uint32_t vmm_create_user_directory();

#endif
