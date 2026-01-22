// PHYSICAL MEMORY MANAGER (PMM)
#include <stddef.h>
#include <stdint.h>
#include "pmm.h"
#include "vga_driver.h"
#include "multiboot.h"
#include "string.h"

static uint32_t* frame_bitmap;
static uint32_t total_frames;
static uint32_t used_frames = 0;

// Private helper macros
#define INDEX_FROM_BIT(b)  ((b) / 32)
#define OFFSET_FROM_BIT(b) ((b) % 32)

// External symbols from linker script
extern uint32_t __boot_heap_start;
extern uint32_t __boot_heap_end;
static uint32_t* boot_heap_ptr = (uint32_t*)&__boot_heap_start;

// Temporary allocator (used ONLY until PMM is ready)
static void* boot_alloc(size_t size) {
    size = (size + 3) & ~3;  // Align to 4 bytes
    if ((uint32_t)boot_heap_ptr + size > (uint32_t)&__boot_heap_end) {
        pmm_panic("Bootstrap heap exhausted");
    }
    void* ptr = boot_heap_ptr;
    boot_heap_ptr += size / 4;
    return ptr;
}

static void pmm_panic(const char* msg) {
    //gterminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    terminal_writestring("PMM PANIC: ");
    terminal_writestring(msg);
    terminal_writestring("\n");
    while (1) {}  // Halt forever
}

void pmm_memset(void* ptr, uint8_t val, size_t size) {
    uint8_t* p = ptr;
    while (size--) *p++ = val;
}

void pmm_init(multiboot_info_t* mbd, unsigned int magic) {
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

	uint32_t mem_size = (mbd->mem_lower + mbd->mem_upper) * 1024;
	terminal_writestring("Initializing PMM with memory size: ");
    terminal_hexprint(mem_size);
    terminal_writestring(" bytes\n");

    // Calculate bitmap size
    total_frames = (mem_size + 0xFFF) / 0x1000;
    uint32_t bitmap_size = (total_frames + 31) / 32 * 4;

    // Allocate bitmap using bootstrap heap
    frame_bitmap = boot_alloc(bitmap_size);
    pmm_memset(frame_bitmap, 0xFF, bitmap_size);  // Mark all as "used"

	int i;
    for(i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        terminal_writestring("  Base: ");
        terminal_hexprint(mmmt->addr);
        terminal_writestring(", Length: ");
        terminal_hexprint(mmmt->len);
        terminal_writestring(", Type: ");
        terminal_hexprint(mmmt->type);
        terminal_writestring("\n");
        //printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
         //   mmmt->addr, mmmt->len, mmmt->size, mmmt->type);

        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint32_t start_frame = (mmmt->addr + 0xFFF) / 0x1000;
			uint32_t end_frame = (mmmt->addr + mmmt->len) / 0x1000;

	        // Ensure we don't go beyond bitmap bounds
	        if (end_frame > total_frames) end_frame = total_frames;

	        for (uint32_t i = start_frame; i < end_frame; i++) {
	            frame_bitmap[INDEX_FROM_BIT(i)] &= ~(1 << OFFSET_FROM_BIT(i));
	        }
        }
    }

	extern int _start;
	extern int __kernel_end;

    uint32_t kernel_start = _start;
    uint32_t kernel_end = __kernel_end;
    for (uint32_t i = kernel_start / 0x1000; i < kernel_end / 0x1000; i++) {
        frame_bitmap[INDEX_FROM_BIT(i)] |= (1 << OFFSET_FROM_BIT(i));
        used_frames++;
    }
}

uint32_t pmm_alloc_frame() {
    for (uint32_t i = 0; i < total_frames; i++) {
        if (!(frame_bitmap[INDEX_FROM_BIT(i)] & (1 << OFFSET_FROM_BIT(i)))) {
            frame_bitmap[INDEX_FROM_BIT(i)] |= (1 << OFFSET_FROM_BIT(i));
            used_frames++;
            return i * 0x1000;
        }
    }
    pmm_panic("Out of memory");
    return 0;
}

void pmm_free_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    if (frame >= total_frames) pmm_panic("Invalid frame free");

    frame_bitmap[INDEX_FROM_BIT(frame)] &= ~(1 << OFFSET_FROM_BIT(frame));
    used_frames--;
}

uint32_t pmm_get_total_frames() { return total_frames; }
uint32_t pmm_get_used_frames() { return used_frames; }

void pmm_print_status() {
    terminal_writestring("PMM Status: ");
    terminal_decprint(pmm_get_used_frames());
    terminal_writestring(" used / ");
    terminal_decprint(pmm_get_total_frames());
    terminal_writestring(" total frames\n");
}
