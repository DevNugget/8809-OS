// PHYSICAL MEMORY MANAGER (PMM)
#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

static void pmm_panic(const char* msg);
void pmm_memset(void* ptr, uint8_t val, size_t size);

void pmm_init(multiboot_info_t* mbd, unsigned int magic);

uint32_t pmm_alloc_frame();
void pmm_free_frame(uint32_t frame_addr);

uint32_t pmm_get_total_frames();
uint32_t pmm_get_used_frames();

void pmm_print_status();

static void* boot_alloc(size_t size);

#endif // PMM_H