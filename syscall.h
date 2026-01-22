#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include "keyboard_driver.h"
#include "idt.h"
#include "util.h"
#include "kernel/console.h"

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
#define SYSCALL_DRAWPIXEL 18
#define SYSCALL_DRAWIMAGE 19
#define SYSCALL_MOVECURSOR 20
#define SYSCALL_CLEARLINEFROM 21

uint32_t syscall_handler(struct regs *regs);

#endif
