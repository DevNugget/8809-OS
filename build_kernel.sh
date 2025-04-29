nasm -felf32 boot.asm -o boot.o

i386-elf-gcc -ffreestanding -c kernel.c  -o kernel.o -std=gnu99 -ffreestanding  

i386-elf-gcc -ffreestanding -c vga_driver.c  -o vga_driver.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -ffreestanding -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -ffreestanding -c idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -ffreestanding -c util.c -o util.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -ffreestanding -c keyboard_driver.c -o keyboard_driver.o -std=gnu99 -ffreestanding -O2 

i386-elf-gcc -ffreestanding -c pmm.c -o pmm.o -std=gnu99 -ffreestanding -O2 

i386-elf-gcc -ffreestanding -c vmm.c -o vmm.o -std=gnu99 -ffreestanding -O2 

i386-elf-gcc -ffreestanding -c tss.c -o tss.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c task.c -o task.o -std=gnu99 -ffreestanding -O2 

i386-elf-gcc -ffreestanding -c fatfs/diskio.c -o diskio.o -std=gnu99 -ffreestanding -O2 

i386-elf-gcc -ffreestanding -c fatfs/ff.c -o ff.o -std=gnu99 -ffreestanding -O2 

i386-elf-gcc -ffreestanding -c string.c -o string.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c ata.c -o ata.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib boot.o util.o string.o gdt.o tss.o idt.o keyboard_driver.o pmm.o vmm.o kernel.o vga_driver.o ata.o diskio.o ff.o task.o -lgcc
