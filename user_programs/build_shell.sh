i386-elf-gcc -ffreestanding -m32 -c user_programs/shell.c -o user_programs/shell.o
i386-elf-ld -T user_programs/link.ld user_programs/shell.o -o user_programs/shell.elf
