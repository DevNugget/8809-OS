#include "syscall.h"
#include "string.h"
#include "vga.h"

theme_t current_theme;

int prompt_count = 0;
char cwd[256] = "/";

void shell_prompt() {
    syscall_termcolor(current_theme.light_green, current_theme.black);
    syscall_write("8809 [");
    syscall_termcolor(current_theme.light_blue, current_theme.black);
    syscall_write(cwd);
    syscall_termcolor(current_theme.light_green, current_theme.black);
    syscall_write("]");
    syscall_termcolor(current_theme.gray, current_theme.black);
    syscall_write("::");
    syscall_termcolor(current_theme.white, current_theme.black);
}

void shell_loop() {
    char input[256];

    while (1) {
        shell_prompt();
        prompt_count++;
        readline(input, sizeof(input));

        // Skip empty input
        if (input[0] == '\0') continue;

        // Parse and handle commands
        if (!strcmp(input, "help")) {
            syscall_write("Available commands: help, echo, exit\n");
        } else if (!strcmp(input, "clear")) {
            syscall_termclear();
        } else if (!strncmp(input, "echo ", 5)) {
            syscall_write(input + 5);
            syscall_write("\n");
        } else if (!strncmp(input, "ldfont ", 7)) {
            const char* path = input + 7;

            char fullpath[512];
            if (path[0] == '/') {
                strcpy(fullpath, path);  // absolute path
            } else {
                strcpy(fullpath, cwd);
                if (cwd[strlen(cwd) - 1] != '/') strcat(fullpath, "/");
                strcat(fullpath, path);
            }

            syscall_termfont(fullpath);
        } else if (!strncmp(input, "ldtheme ", 8)) {
            const char* path = input + 8;

            char fullpath[512];
            if (path[0] == '/') {
                strcpy(fullpath, path);  // absolute path
            } else {
                strcpy(fullpath, cwd);
                if (cwd[strlen(cwd) - 1] != '/') strcat(fullpath, "/");
                strcat(fullpath, path);
            }

            syscall_ldtheme(fullpath);
            syscall_gettheme(&current_theme);
        } else if (!strcmp(input, "exit")) {
            syscall_write("SHELL EXIT\n");
            syscall_exit();
            break;
        } else if (!strncmp(input, "cd ", 3)) {
            const char* path = input + 3;
            if (syscall_checkdir(path) == 1) {
                strcpy(cwd, path);
            } else {
                syscall_write("Directory not found: ");
                syscall_write(path);
                syscall_write("\n");
            }
        } else if (!strncmp(input, "mkfile ", 6)) {
            const char* filename = input + 7;

            if (filename[0] == '\0') {
                syscall_write("Usage: mkfile <filename>\n");
            } else {
                char fullpath[512];
                if (filename[0] == '/') {
                    strcpy(fullpath, filename);  // absolute path
                } else {
                    strcpy(fullpath, cwd);
                    if (cwd[strlen(cwd) - 1] != '/') {
                        strcat(fullpath, "/");
                    }
                    strcat(fullpath, filename);  // append filename without leading space
                }

                int res = syscall_writefile(fullpath, "", 0); // create empty file
                if (res == 1) {
                    syscall_write("Failed to create file: ");
                    syscall_write(fullpath);
                    syscall_write("\n");
                }
            }
        } else {
            char fullpath[512];
            if (input[0] == '/') {
                strcpy(fullpath, input);  // absolute path
            } else {
                strcpy(fullpath, cwd);
                if (cwd[strlen(cwd) - 1] != '/') strcat(fullpath, "/");
                strcat(fullpath, input);
            }

            int result = syscall_exec(fullpath, cwd);
            if (result == 0) {
                syscall_write("Failed to execute: ");
                syscall_write(fullpath);
                syscall_write("\n");
            }
        }
    }
}

void _start() {
    //syscall_ldtheme("themes/default.txt");
    syscall_gettheme(&current_theme);
    syscall_termcolor(current_theme.white, current_theme.black);
    syscall_recovercwd(cwd);
    shell_loop();
}
