#include "syscall.h"
#include "string.h"

#define BUFFER_SIZE 512

void _start() {
    char input[256];
    readline(input, sizeof(input));

    if (input[0] == '\0') {
        syscall_write("Usage: cat <filename>\n");
        syscall_exit();
    }

    // Build full path
    char fullpath[512];
    if (input[0] == '/') {
        strcpy(fullpath, input); // absolute path
    } else {
        // assume cwd from shell is "/" if not passed
        strcpy(fullpath, "/");
        if (fullpath[strlen(fullpath) - 1] != '/') strcat(fullpath, "/");
        strcat(fullpath, input);
    }

    char buf[BUFFER_SIZE];
    int read_bytes;

    read_bytes = read_file(fullpath, buf, BUFFER_SIZE);
        // write to terminal
    buf[read_bytes] = '\0';  // make sure it's null-terminated
    syscall_write(buf);

    if (read_bytes < 0) {
        syscall_write("Failed to read file: ");
        syscall_write(fullpath);
        syscall_write("\n");
    }

    syscall_write("\n");
    syscall_exit();
}
