// PHYSICAL MEMORY MANAGER (PMM)
#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

static void pmm_panic(const char* msg);
static void pmm_memset(void* ptr, uint8_t val, size_t size);

void pmm_init(uint32_t mem_size, uint32_t* mmap_addr, uint32_t mmap_length, multiboot_info_t* mbd);

uint32_t pmm_alloc_frame();
void pmm_free_frame(uint32_t frame_addr);

uint32_t pmm_get_total_frames();
uint32_t pmm_get_used_frames();

void pmm_print_status();

static void* boot_alloc(size_t size);

#endif // PMM_H