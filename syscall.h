#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include "vga_driver.h"
#include "keyboard_driver.h"
#include "idt.h"
#include "util.h"

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

uint32_t syscall_handler(struct regs *regs);

theme_t current_theme;

#endif
