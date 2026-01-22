#pragma once

#include "framebuffer.h"
#include "font_psf.h"

typedef struct {
    uint32_t cursor_x;
    uint32_t cursor_y;

    uint32_t fg;
    uint32_t bg;

    uint32_t cols;
    uint32_t rows;

    int cursor_visible;

    char cursor_char_under;
} gterminal_t;


void gterminal_init(gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* font);
void gterminal_scroll(gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_clear(gterminal_t* gterm, framebuffer_t* fb);
void gterminal_draw_cursor(gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_hide_cursor(gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_putchar(char c, gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_move_cursor(int row, int col, gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_clear_line_from(int col, gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_set_colors(uint32_t fg, uint32_t bg, gterminal_t* gterm);
void gterminal_writestring(const char* str, gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
void gterminal_decprint(uint32_t num, gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
uint32_t gterminal_getfg(gterminal_t* gterm);
uint32_t gterminal_getbg(gterminal_t* gterm);