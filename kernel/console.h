#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../drivers/video/font_psf.h"
#include "../ui/ui.h"
#include "../drivers/video/gterminal.h"
#include "../ui/theme.h"

void console_init(void);

void console_write(const char* str);
void console_putchar(char c);
void console_clear(void);

void console_move_cursor(int row, int col);
void console_clear_line_from(int col);

void console_set_colors(uint32_t fg, uint32_t bg);
uint32_t console_get_fg(void);
uint32_t console_get_bg(void);

void console_draw_cursor(void);
void console_hide_cursor(void);

void console_decprint(uint32_t num);

void console_putpixel(int x, int y, uint32_t color);
int console_changefont(const char* path);
int console_changetheme(const char* path);
theme_t* console_gettheme();
