#include "syscall.h"
#include "string.h"

char *trim(char *s) {
    while (*s == ' ' || *s == '\t') s++;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end-- = '\0';
    }
    return s;
}

void shell_load_rc() {
    char buf[1024];
    int bytes = read_file("/shellrc", buf, sizeof(buf) - 1);
    if (bytes <= 0) return;

    buf[bytes] = '\0';

    char *cursor = buf;
    while (*cursor) {
        // Get one line at a time
        char *line = cursor;
        while (*cursor && *cursor != '\n') cursor++;
        if (*cursor == '\n') *cursor++ = '\0';

        // Only process lines starting with "set "
        if (strncmp(line, "set ", 4) == 0) {
            char *equal = strchr(line + 4, '=');
            if (!equal) continue;

            *equal = '\0';
            char *key = trim(line + 4);
            char *value = trim(equal + 1);

            //syscall_write("KEY: "); syscall_write(key); syscall_write("\n");
            //syscall_write("VAL: "); syscall_write(value); syscall_write("\n");

            if (strcmp(key, "theme") == 0) {
                syscall_ldtheme(value);
            }
            else if (strcmp(key, "font") == 0) {
                syscall_termfont(value);
            }
        }
    }
}

void _start() {
    shell_load_rc();
    syscall_exec("/bin/shell.elf", "/");
}
