#include "syscall.h"
#include "kernel/console.h"
#include "keyboard_driver.h"
#include "task.h"
#include "elf32/elf32.h"
#include "fatfs/ff.h"
#include <stdint.h>
#include "string.h"

char cwd[256] = "/";
struct dirent {
    char name[256];
    uint8_t is_dir;
};

int syscall_read_file(const char *path, char *buf, size_t bufsize) {
    FIL file;
    FRESULT res = f_open(&file, path, FA_READ);
    if (res != FR_OK)
        return -1;

    UINT bytes_read;
    res = f_read(&file, buf, bufsize - 1, &bytes_read);
    f_close(&file);

    if (res != FR_OK)
        return -1;

    buf[bytes_read] = '\0'; // Null-terminate just in case
    return bytes_read;
}

uint32_t syscall_handler(struct regs *regs) {
    asm volatile ("sti");
    switch (regs->eax) {
        case SYSCALL_WRITE: {
            // Assume ebx = char* user string, ecx = length
            char* user_str = (char*)regs->ebx;
            uint32_t len = regs->ecx;

            console_write(user_str);
            //terminal_write_screen_size();

            break;
        }

        case SYSCALL_CLEARLINEFROM: {
            int col = (int)regs->ebx; // starting column

            console_clear_line_from(col);
            break;
        }

        case SYSCALL_MOVECURSOR: {
            int row = (int)regs->ebx;
            int col = (int)regs->ecx;

            console_move_cursor(row, col);
            break;
        }

        case SYSCALL_DRAWPIXEL: {
            int x = (int)regs->ebx;
            int y = (int)regs->ecx;
            uint32_t color = (uint32_t)regs->edx;

            // bounds check
            if (x < 0 || x >= 640 || y < 0 || y >= 480) {
                // Optionally log error, ignore, or return error code
                break;
            }

            console_putpixel(x, y, color);
            break;
        }

        case SYSCALL_DRAWIMAGE: {
            int x = regs->ebx;
            int y = regs->ecx;
            int width = regs->edx;
            int height = regs->esi;
            unsigned int* pixels = (unsigned int*)regs->edi;

            for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                    unsigned int color = pixels[row * width + col];
                    console_putpixel(x + col, y + row, color);
                }
            }
            break;
        }


        case SYSCALL_READCHAR: {
            char c = keyboard_getchar();
            regs->eax = (uint32_t)c;

            break;
        }

        case SYSCALL_WRITEINT: {
            // Assume ebx = char* user string, ecx = length
            int32_t user_int = (int32_t)regs->ebx;
            uint32_t len = regs->ecx;

            console_decprint(user_int);

            break;
        }

        case SYSCALL_TERMCOLOR: {
            uint32_t fg = regs-> ebx;
            uint32_t bg = regs-> ecx;

            console_set_colors(fg, bg);
            break;
        }

        case SYSCALL_EXEC: {
            const char* filename = (const char*)regs->ebx;
            const char* last_cwd = (const char*)regs->ecx;

            strcpy(cwd, last_cwd);

            uint32_t new_entry = load_elf(filename);
            if (new_entry != 0) {
                switch_to_user_mode_elf(new_entry);
            } else if (new_entry == 0) {
                regs->eax = 0;
            }

            break;
        }

        case SYSCALL_EXIT: {
            uint32_t shell_reload_entry = load_elf("bin/shell.elf");
            switch_to_user_mode_elf(shell_reload_entry);

            break;
        }

        case SYSCALL_CHECKDIR: {
            const char* path = (const char*)regs->ebx;
            DIR dir;
            FILINFO fno;
            FRESULT res = f_opendir(&dir, path);
            if (res == FR_OK) {
                f_closedir(&dir);
                regs->eax = 1;
                break;  // exists
            }
            regs->eax = 0; // doesn't exist or error
            break;
        }

        case SYSCALL_RECOVERCWD: {
            char *dest = (char *)regs->ebx; // User-provided buffer
            strcpy(dest, cwd);
            break;
        }

        case SYSCALL_READDIR: {
            const char *path = (const char*)regs->ebx;
            struct dirent *user_entry = (struct dirent*)regs->ecx;
            int index = (int)regs->edx;

            DIR dir;
            FILINFO fno;
            int count = 0;

            if (f_opendir(&dir, path) != FR_OK) {
                regs->eax = 1;
                break;
            }

            while (1) {
                if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0) {
                    regs->eax = 1;
                    break;
                }

                if (count == index) {
                    strcpy(user_entry->name, fno.fname);
                    user_entry->is_dir = (fno.fattrib & AM_DIR) ? 1 : 0;
                    regs->eax = 0;
                    break;
                }

                if (count > index) {
                    regs->eax = 1;
                    break;
                }

                count++;
            }

            f_closedir(&dir);
            break;
        }

        case SYSCALL_WRITEFILE: {
            const char *path = (const char *)regs->ebx;
            const char *data = (const char *)regs->ecx;
            uint32_t len = regs->edx;

            FIL file;
            UINT bw;

            if (f_open(&file, path, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
                regs->eax = 1; // error
                break;
            }

            if (f_write(&file, data, len, &bw) != FR_OK || bw != len) {
                f_close(&file);
                regs->eax = 1;
                break;
            }

            f_close(&file);
            regs->eax = 0; // success
            break;
        }

        case SYSCALL_TERMCLEAR: {
            console_clear();
            break;
        }

        case SYSCALL_TERMFONT: {
            const char* filename = (const char*)regs->ebx;

            int err = console_changefont(filename);
            regs->eax = err;

            break;
        }

        case SYSCALL_TERMGETFG: {
            regs->eax = console_get_fg();
            break;
        }

        case SYSCALL_TERMGETBG: {
            regs->eax = console_get_bg();
            break;
        }

        case SYSCALL_TERMLDTHEME: {
            // Get the theme file path from user-space (regs->ebx points to the path)
            const char* theme_path = (const char*)regs->ebx;

            // Load the theme from the file and apply it
            int result = console_changetheme(theme_path);
            console_clear();

            // Return the result to user-space (0 for success, non-zero for failure)
            regs->eax = result;

            break;
        }

        case SYSCALL_TERMGETTHEME: {
            theme_t* user_theme = (theme_t*)regs->ebx;

            memcpy(user_theme, console_gettheme(), sizeof(theme_t));

            break;
        }

        case SYSCALL_READFILE: {
            const char *path = (const char *)regs->ebx;
            char *buf = (char *)regs->ecx;
            size_t size = regs->edx;
            regs->eax = syscall_read_file(path, buf, size);
            break;
        }

        default: {
            console_write("Unknown syscall: ");
            console_decprint(regs->eax);
            console_putchar('\n');
            //return_to_usermode(regs->eip);
            return 0;
        }
    }
    return 0;
}
