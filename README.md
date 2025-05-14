# 8809-OS
A hobby operating system written in C.

## Progress
- [x] GDT
- [x] IDT - ISRS & IQRS
- [x] VGA driver to print to screen
- [x] Keyboard driver
- [x] Physical Memory Manager (PMM)
- [x] Virtual Memory Manager (VMM) for paging
- [x] Fatfs Filesystem (Read-Only)
- [x] Running user program from file in usermode
- [x] Basic ELF binary loader
- [x] Basic Shell
- [x] Load programs from filesystem via shell
- [x] Graphics mode shell (Graphical Command-line Interface)
- [x] Change graphical font via .psf file
- [x] Change theme via theme files
- [x] Init system to initialize shell user configuration (shellrc)
- [x] Ability to change and utilize a 'current working directory'

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
./run.sh
```
or to build and run:
```bash
./build_run.sh
```
