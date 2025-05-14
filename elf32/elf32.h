#ifndef ELF32_H
#define ELF32_H

#include <stdint.h>
#include "../pmm.h"
#include "../vmm.h"
#include "../string.h"
#include "../vga_driver.h"
#include "../fatfs/ff.h"

typedef struct {
    uint8_t  e_ident[16];     // ELF Magic, class, etc.
    uint16_t e_type;          // Executable = 2
    uint16_t e_machine;       // EM_386 = 3
    uint32_t e_version;
    uint32_t e_entry;         // Entry point
    uint32_t e_phoff;         // Program header table offset
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;         // Number of program headers
} Elf32_Ehdr;

typedef struct {
    uint32_t p_type;          // PT_LOAD = 1
    uint32_t p_offset;        // Offset in file
    uint32_t p_vaddr;         // Virtual address to map to
    uint32_t p_paddr;
    uint32_t p_filesz;        // Size in file
    uint32_t p_memsz;         // Size in memory
    uint32_t p_flags;         // Flags (read/write/exec)
    uint32_t p_align;
} Elf32_Phdr;

uint32_t load_elf(const char* path);

#endif