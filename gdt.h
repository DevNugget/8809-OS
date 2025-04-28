#ifndef GDT_H
#define GDT_H

// Function to initialize the GDT (called from kernel_main)
void gdt_init();
void gdt_load();

#endif
