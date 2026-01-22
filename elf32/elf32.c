#include "elf32.h"

uint32_t load_elf(const char* path) {
    FIL file;
    FRESULT res = f_open(&file, path, FA_READ);
    if (res != FR_OK) {
        console_write("Failed to open ELF file\n");
        return 0;
    }

    // Read ELF header
    Elf32_Ehdr elf_hdr;
    UINT bytes_read;
    res = f_read(&file, &elf_hdr, sizeof(Elf32_Ehdr), &bytes_read);
    if (res != FR_OK || bytes_read != sizeof(Elf32_Ehdr)) {
        console_write("Failed to read ELF header\n");
        f_close(&file);
        return 0;
    }

    // Check ELF magic
    if (elf_hdr.e_ident[0] != 0x7F || elf_hdr.e_ident[1] != 'E' ||
        elf_hdr.e_ident[2] != 'L' || elf_hdr.e_ident[3] != 'F') {
        console_write("Invalid ELF magic\n");
        f_close(&file);
        return 0;
    }

    // Read program headers
    for (int i = 0; i < elf_hdr.e_phnum; i++) {
        Elf32_Phdr phdr;
        f_lseek(&file, elf_hdr.e_phoff + i * sizeof(Elf32_Phdr));
        res = f_read(&file, &phdr, sizeof(Elf32_Phdr), &bytes_read);
        if (res != FR_OK || bytes_read != sizeof(Elf32_Phdr)) {
            console_write("Failed to read program header\n");
            f_close(&file);
            return 0;
        }

        if (phdr.p_type != 1) continue; // Only load PT_LOAD segments

        // Allocate and map memory
        for (uint32_t offset = 0; offset < phdr.p_memsz; offset += 4096) {
            uint32_t va = phdr.p_vaddr + offset;
            uint32_t frame = pmm_alloc_frame();
            vmm_map_page(va, frame, PAGE_PRESENT | PAGE_USER | PAGE_RW | PAGE_EXECUTABLE);
        }

        // Load segment into memory
        f_lseek(&file, phdr.p_offset);
        f_read(&file, (void*)phdr.p_vaddr, phdr.p_filesz, &bytes_read);

        // Zero out the remaining part if p_memsz > p_filesz
        if (phdr.p_memsz > phdr.p_filesz) {
            memset((void*)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
        }
    }

    f_close(&file);
    return elf_hdr.e_entry;
}

int unload_elf(const char* path) {
    FIL file;
    FRESULT res = f_open(&file, path, FA_READ);
    if (res != FR_OK) {
        console_write("Failed to open ELF file\n");
        return -1;
    }

    Elf32_Ehdr elf_hdr;
    UINT bytes_read;
    res = f_read(&file, &elf_hdr, sizeof(Elf32_Ehdr), &bytes_read);
    if (res != FR_OK || bytes_read != sizeof(Elf32_Ehdr)) {
        console_write("Failed to read ELF header\n");
        f_close(&file);
        return -1;
    }

    for (int i = 0; i < elf_hdr.e_phnum; i++) {
        Elf32_Phdr phdr;
        f_lseek(&file, elf_hdr.e_phoff + i * sizeof(Elf32_Phdr));
        res = f_read(&file, &phdr, sizeof(Elf32_Phdr), &bytes_read);
        if (res != FR_OK || bytes_read != sizeof(Elf32_Phdr)) {
            console_write("Failed to read program header\n");
            f_close(&file);
            return -1;
        }

        if (phdr.p_type != 1) continue; // Only PT_LOAD

        for (uint32_t offset = 0; offset < phdr.p_memsz; offset += 4096) {
            uint32_t va = phdr.p_vaddr + offset;

            // Get page table
            uint32_t page_dir_index = va >> 22;
            uint32_t page_table_index = (va >> 12) & 0x3FF;

            if (!(page_directory[page_dir_index] & PAGE_PRESENT)) continue;

            uint32_t* page_table = (uint32_t*)(page_directory[page_dir_index] & ~0xFFF);
            if (!(page_table[page_table_index] & PAGE_PRESENT)) continue;

            uint32_t phys = page_table[page_table_index] & ~0xFFF;
            pmm_free_frame(phys); // Return the frame to the pool

            // Unmap the page
            page_table[page_table_index] = 0;
            asm volatile("invlpg (%0)" ::"r" (va) : "memory");

            console_write("Unmapped ");
            //terminal_hexprint(va);
            console_write("\n");
        }
    }

    f_close(&file);
    return 0;
}
