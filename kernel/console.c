#include "console.h"

extern ui_context_t ui;

void console_init(void) {
    // Nothing special for now, but this is where you'd later
    // support switching terminals, etc.
}

void console_putchar(char c) {
    gterminal_putchar(c, &ui.term, &ui.fb, &ui.font);
}

void console_write(const char* str) {
    gterminal_writestring(str, &ui.term, &ui.fb, &ui.font);
}

void console_clear(void) {
    gterminal_clear(&ui.term, &ui.fb);
}

void console_move_cursor(int row, int col) {
    gterminal_move_cursor(row, col, &ui.term, &ui.fb, &ui.font);
}

void console_clear_line_from(int col) {
    gterminal_clear_line_from(col, &ui.term, &ui.fb, &ui.font);
}

void console_set_colors(uint32_t fg, uint32_t bg) {
    gterminal_set_colors(fg, bg, &ui.term);
}

uint32_t console_get_fg(void) {
    return gterminal_getfg(&ui.term);
}

uint32_t console_get_bg(void) {
    return gterminal_getbg(&ui.term);
}

void console_draw_cursor(void) {
    gterminal_draw_cursor(&ui.term, &ui.fb, &ui.font);
}

void console_hide_cursor(void) {
    gterminal_hide_cursor(&ui.term, &ui.fb, &ui.font);
}

void console_decprint(uint32_t num) {
    gterminal_decprint(num, &ui.term, &ui.fb, &ui.font);
}

void console_putpixel(int x, int y, uint32_t color) {
    fb_putpixel(&ui.fb, x, y, color);
}

int console_changefont(const char* path) {
    int err = load_psf1_font(path, &ui.font); 
    return err;
}

int console_changetheme(const char* path) {
    int err = load_theme(path, &ui.theme, &ui.term, &ui.fb, &ui.font);
    console_set_colors(ui.theme.white, ui.theme.white);

    return err;
}

theme_t* console_gettheme() {
    return &ui.theme;
}
