#include "syscall.h"
#include "vga.h"

theme_t current_theme;

void _start() {
    syscall_gettheme(&current_theme);
    syscall_write(" ________  ________  ________  ________     \n");
    syscall_write("|\\   __  \\|\\   __  \\|\\   __  \\|\\  ___  \\    \n");
    syscall_write("\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\____   \\   \n");
    syscall_write(" \\ \\   __  \\ \\   __  \\ \\  \\\\\\  \\|____|\\  \\  \n");
    syscall_write("  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\\\\\  \\  __\\_\\  \\ \n");
    syscall_write("   \\ \\_______\\ \\_______\\ \\_______\\|\\_______\\\n");
    syscall_write("    \\|_______|\\|_______|\\|_______|\\|_______|\n");
    syscall_write("                                            \n\n");
    syscall_termcolor(current_theme.white, current_theme.black);
    syscall_write("OS: ");
    syscall_termcolor(current_theme.gray, current_theme.black);
    syscall_write("8809\n");
    syscall_termcolor(current_theme.white, current_theme.black);
    syscall_write("Kernel: ");
    syscall_termcolor(current_theme.gray, current_theme.black);
    syscall_write("8809-SU-M-0425\n");
    syscall_exit();
}
