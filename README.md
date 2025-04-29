# 8809-OS
A hobby operating system written in C.

## Progress
- [x] GDT
- [x] IDT - ISRS & IQRS
- [x] VGA driver to print to screen
- [x] Keyboard driver
- [x] Physical Memory Manager (PMM)
- [x] Virtual Memory Manager (VMM) for paging

## Building and Running
### Prerequisites
- grub
- xorriso
- nasm
- i386-elf-gcc
- qemu

### Building ISO
Run the following bash commands:
```bash
git clone "https://github.com/DevNugget/8809-OS"
cd 8809-OS
./build_kernel.sh
./build_iso.sh
```
### Running the ISO
```bash
qemu-system-i386 -cdrom os.iso
```
