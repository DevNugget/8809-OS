#include "syscall.h"
#include "string.h"
#include "vga.h"

theme_t current_theme;

void ls(const char* path) {
    struct dirent entry;
    int i = 0;

    while (1) {
        if (syscall_readdir(path, &entry, i) == 1) break;
        if (entry.name[0] == '\0') break;

        if (entry.is_dir) syscall_termcolor(current_theme.light_blue, current_theme.black);
        syscall_write(entry.name);
        if (entry.is_dir) syscall_write("/");  // add slash for folders
        if (entry.is_dir) syscall_termcolor(current_theme.white, current_theme.black);
        syscall_write("\n");
        i++;
        // /break;
    }
}

void _start() {
    char cwd[256];
    syscall_recovercwd(cwd);

    syscall_gettheme(&current_theme);

    syscall_write("\n");
    ls(cwd); // list current directory
    syscall_write("\n");
    syscall_exit();
}
