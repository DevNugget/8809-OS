#define SYSCALL_WRITE 1
#define SYSCALL_WRITEINT 2
#define SYSCALL_READCHAR 3
#define SYSCALL_TERMCOLOR 4
#define SYSCALL_EXEC 5
#define SYSCALL_EXIT 6
#define SYSCALL_CHECKDIR 7
#define SYSCALL_RECOVERCWD 8
#define SYSCALL_READDIR 9
#define SYSCALL_WRITEFILE 10
#define SYSCALL_TERMCLEAR 11
#define SYSCALL_TERMFONT 12
#define SYSCALL_TERMGETFG 13
#define SYSCALL_TERMGETBG 14
#define SYSCALL_TERMLDTHEME 15
#define SYSCALL_TERMGETTHEME 16
#define SYSCALL_READFILE 17

#include <stdint.h>
#include <stddef.h>
#include "vga.h"

struct dirent {
    char name[256];
    uint8_t is_dir;
};

static inline int syscall_write(const char* str) {
    int ret;
    asm volatile("int $0x80"
                 : "=a"(ret)
                 : "a"(SYSCALL_WRITE), "b"(str));
    return ret;
}

static inline int syscall_writeint(int32_t str) {
    int ret;
    asm volatile("int $0x80"
                 : "=a"(ret)
                 : "a"(SYSCALL_WRITEINT), "b"(str));
    return ret;
}

static inline char syscall_readchar() {
    char c;
    asm volatile("int $0x80"
                 : "=a"(c)
                 : "a"(SYSCALL_READCHAR));
    return c;
}

static inline void syscall_termcolor(uint32_t fg, uint32_t bg) {
    asm volatile("int $0x80"
                 :
                 : "a"(SYSCALL_TERMCOLOR), "b"(fg), "c"(bg));
}

static inline void syscall_exit() {
    asm volatile("int $0x80"
                 :
                 : "a"(SYSCALL_EXIT));
}

static inline int syscall_exec(const char* filename, const char* cwd) {
    int ret;
    asm volatile("int $0x80"
                 : "=a" (ret)
                 : "a"(SYSCALL_EXEC), "b"(filename), "c"(cwd));

    return ret;
}

static inline int syscall_checkdir(const char* path) {
    int result;
    asm volatile (
        "int $0x80"
        : "=a" (result)
        : "a" (SYSCALL_CHECKDIR), "b" (path)
    );
    return result;
}

static inline void syscall_recovercwd(char *dest) {
    asm volatile (
        "int $0x80"
        :
        : "a"(SYSCALL_RECOVERCWD), "b"(dest)
    );
}

static inline int syscall_readdir(const char *path, struct dirent *entry, int index) {
    int result;
    asm volatile (
        "int $0x80"
        : "=a"(result)
        : "a"(SYSCALL_READDIR), "b"(path), "c"(entry), "d"(index)
    );
    return result;
}

static inline int syscall_writefile(const char *path, const char *data, unsigned int len) {
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(SYSCALL_WRITEFILE), "b"(path), "c"(data), "d"(len)
    );
    return ret;
}

static inline void syscall_termclear() {
    asm volatile (
        "int $0x80"
        :: "a"(SYSCALL_TERMCLEAR)
    );
}

static inline int syscall_termfont(const char* filename) {
    int ret;
    asm volatile("int $0x80"
                 : "=a" (ret)
                 : "a"(SYSCALL_TERMFONT), "b"(filename));

    return ret;
}

static inline uint32_t syscall_termgetfg() {
    uint32_t color;
    asm volatile (
        "int $0x80"
        : "=a" (color)
        : "a"(SYSCALL_TERMGETFG)
    );

    return color;
}

static inline uint32_t syscall_termgetbg() {
    uint32_t color;
    asm volatile (
        "int $0x80"
        : "=a" (color)
        : "a"(SYSCALL_TERMGETBG)
    );

    return color;
}

static inline int syscall_ldtheme(const char* theme_path) {
    int ret;
    asm volatile("int $0x80"
                 : "=a"(ret)
                 : "a"(SYSCALL_TERMLDTHEME), "b"(theme_path));
    return ret;
}

static inline int syscall_gettheme(theme_t* theme) {
    int ret;
    asm volatile("int $0x80"
                 : "=a"(ret)
                 : "a"(SYSCALL_TERMGETTHEME), "b"(theme));
    return ret;
}

static inline int read_file(const char *path, char *buf, size_t size) {
    int ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(SYSCALL_READFILE), "b"(path), "c"(buf), "d"(size)
    );
    return ret;
}

#define MAX_INPUT_LEN 256

void readline(char* buffer, int max_len) {
    int i = 0;

    while (i < max_len - 1) {
        char c = syscall_readchar();

        if (c == '\n') {
            syscall_write("\n");  // echo newline
            break;
        }

        if (c == '\b' || c == 127) {
            if (i > 0) {
                i--;
                syscall_write("\b \b");  // erase last character on screen
            }
            continue;
        }

        buffer[i++] = c;

        char c_str[] = {c, '\0'};
        // Echo character
        syscall_write(c_str);
    }

    buffer[i] = '\0';
}
