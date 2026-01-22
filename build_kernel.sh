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

i386-elf-gcc -ffreestanding -c fatfs/ata.c -o ata.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c heap.c -o heap.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c elf32/elf32.c -o elf32.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c syscall.c -o syscall.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c drivers/video/font_psf.c -o font_psf.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c drivers/video/framebuffer.c -o framebuffer.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c drivers/video/gterminal.c -o gterminal.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c kernel/console.c -o console.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c ui/theme.c -o theme.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -ffreestanding -c ui/ui.c -o ui.o -std=gnu99 -ffreestanding -O2

i386-elf-gcc -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib boot.o util.o string.o gdt.o tss.o idt.o keyboard_driver.o pmm.o vmm.o heap.o elf32.o syscall.o kernel.o vga_driver.o ata.o diskio.o ff.o task.o font_psf.o framebuffer.o gterminal.o console.o theme.o ui.o -lgcc

rm boot.o
rm util.o
rm string.o
rm gdt.o
rm tss.o
rm idt.o
rm keyboard_driver.o
rm pmm.o
rm vmm.o
rm heap.o
rm elf32.o
rm kernel.o
rm vga_driver.o
rm ata.o
rm diskio.o
rm ff.o
rm task.o
rm syscall.o
