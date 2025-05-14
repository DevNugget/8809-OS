#include "syscall.h"
#include "vga.h"

void _start() {
    syscall_write(" ________  ________  ________  ________     \n");
    syscall_write("|\\   __  \\|\\   __  \\|\\   __  \\|\\  ___  \\    \n");
    syscall_write("\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\____   \\   \n");
    syscall_write(" \\ \\   __  \\ \\   __  \\ \\  \\\\\\  \\|____|\\  \\  \n");
    syscall_write("  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\\\\\  \\  __\\_\\  \\ \n");
    syscall_write("   \\ \\_______\\ \\_______\\ \\_______\\|\\_______\\\n");
    syscall_write("    \\|_______|\\|_______|\\|_______|\\|_______|\n");
    syscall_write("                                            \n\n");
    syscall_termcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    syscall_write("OS: ");
    syscall_termcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    syscall_write("8809\n");
    syscall_termcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    syscall_write("Kernel: ");
    syscall_termcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    syscall_write("8809-SU-M-0425\n");
    syscall_exit();
}
