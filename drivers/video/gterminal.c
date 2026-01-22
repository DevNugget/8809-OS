#include "gterminal.h"
#include "font_psf.h"
#include "framebuffer.h"

void gterminal_init(gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* font) {
    gterm->cursor_x = 0;
    gterm->cursor_y = 0;

    gterm->rows = (fb->height / font->glyph_size);
    gterm->cols = (fb->width / 8);

    gterm->fg = 0xFFFFFF;
    gterm->bg = 0x000000;

    gterm->cursor_visible = 1;
    gterm->cursor_char_under = ' ';
}

void gterminal_scroll(gterminal_t* gterm, framebuffer_t* fb, 
        psf1_font_t* loaded_font) {
    int row_size = loaded_font->glyph_size;
    uint8_t* addr = (uint8_t*)fb->addr;

    for (int y = 0; (uint32_t)y < fb->height - row_size; ++y) {
        for (int x = 0; (uint32_t)x < fb->width * (fb->bpp / 8); ++x) {
            addr[y * fb->pitch + x] = addr[(y + row_size) * fb->pitch + x];
        }
    }

    // Clear the last row
    for (int y = fb->height - row_size; (uint32_t)y < fb->height; ++y) {
        for (int x = 0; (uint32_t)x < fb->width; ++x) {
            fb_putpixel(fb, x, y, gterm->bg);
        }
    }

    gterm->cursor_y -= 1;
}

void gterminal_clear(gterminal_t* gterm, framebuffer_t* fb) {
    for (int y = 0; (uint32_t)y < fb->height; ++y) {
        for (int x = 0; (uint32_t)x < fb->width; ++x) {
            fb_putpixel(fb, x, y, gterm->bg);
        }
    }

    gterm->cursor_x = 0;
    gterm->cursor_y = 0;
}

// Draw the cursor at the current position
void gterminal_draw_cursor(gterminal_t* gterm, framebuffer_t* fb, 
        psf1_font_t* loaded_font) {
    if (gterm->cursor_visible) {
        // Save original character under cursor (if needed)
        // cursor_char_under is stored elsewhere

        // Draw cursor as a solid block or underline
        int cursor_x = gterm->cursor_x * 8;
        int cursor_y = gterm->cursor_y * loaded_font->glyph_size;

        // Block cursor (full character)
        for (int y = 0; (uint32_t)y < loaded_font->glyph_size; ++y) {
            for (int x = 0; (uint32_t)x < 8; ++x) {
                //uint32_t old = get_pixel(gterm_cursor_x + x, gterm_cursor_y + y);
                fb_putpixel(fb, cursor_x + x, cursor_y + y, gterm->fg);
            }
        }

        // Alternative: Underline cursor (only bottom row)
        /*
        for (int x = 0; x < 8; ++x) {
            draw_pixel(cursor_x + x, cursor_y + loaded_font.glyph_size - 1, gterminal_color);
        }
        */
    }
}

void gterminal_hide_cursor(gterminal_t* gterm, framebuffer_t* fb, 
        psf1_font_t* loaded_font) {
    int cursor_x = gterm->cursor_x * 8;
    int cursor_y = gterm->cursor_y * loaded_font->glyph_size;

    // Clear cursor area
    for (int y = 0; (uint32_t)y < loaded_font->glyph_size; ++y) {
        for (int x = 0; (uint32_t)x < 8; ++x) {
            fb_putpixel(fb, cursor_x + x, cursor_y + y, gterm->bg);
        }
    }

    // Redraw the character that was under the cursor
    if (gterm->cursor_char_under != ' ') {
        draw_char_psf(cursor_x, cursor_y, gterm->cursor_char_under, gterm->fg, loaded_font, fb);
    }
}

void gterminal_putchar(char c, gterminal_t* gterm, framebuffer_t* fb, 
        psf1_font_t* loaded_font) {
    // Hide cursor before modifying text
    gterminal_hide_cursor(gterm, fb, loaded_font);

    if (c == '\n') {
        gterm->cursor_x = 0;
        gterm->cursor_y++;
        if (gterm->cursor_y >= gterm->rows) {
            gterminal_scroll(gterm, fb, loaded_font);
        }

        gterm->cursor_char_under = ' ';
    } else if (c == '\b') {
        if (gterm->cursor_x > 0) {
            gterm->cursor_x--;
        } else if (gterm->cursor_y > 0) {
            gterm->cursor_y--;
            gterm->cursor_x = gterm->cols - 1;
        }
        // Clear the character visually
        for (int y = 0; (uint32_t)y < loaded_font->glyph_size; ++y) {
            for (int x = 0; x < 8; ++x) {
                fb_putpixel(
                    fb,
                    gterm->cursor_x * 8 + x,
                    gterm->cursor_y * loaded_font->glyph_size + y,
                    gterm->bg
                );
            }
        }
        gterm->cursor_char_under = ' ';
    } else {
        draw_char_psf(
            gterm->cursor_x * 8,
            gterm->cursor_y * loaded_font->glyph_size,
            c,
            gterm->fg,
            loaded_font,
            fb
        );
        gterm->cursor_char_under = c;
        gterm->cursor_x++;
        if (gterm->cursor_x >= gterm->cols) {
            gterm->cursor_x = 0;
            gterm->cursor_y++;
            if (gterm->cursor_y >= gterm->rows) {
                gterminal_scroll(gterm, fb, loaded_font);
            }
        }
    }

    // Update character under cursor
    gterm->cursor_char_under = ' ';  // Default is space, should be whatever is at current position

    // Draw cursor at new position
    //cursor_blink_state = 1;  // Reset blink state
    //cursor_blink_timer = 0;  // Reset blink timer (you'll need to implement this)
    gterminal_draw_cursor(gterm, fb, loaded_font);
}

void gterminal_move_cursor(int row, int col, gterminal_t* gterm, 
        framebuffer_t* fb, psf1_font_t* loaded_font) {
    // Clamp to screen bounds
    if (row < 0) row = 0;
    if (row >= gterm->rows) row = gterm->rows - 1;
    if (col < 0) col = 0;
    if (col >= gterm->cols) col = gterm->cols - 1;

    // Hide old cursor
    gterminal_hide_cursor(gterm, fb, loaded_font);

    // Update cursor position
    gterm->cursor_x = col;
    gterm->cursor_y = row;

    // Draw cursor at new position
    gterminal_draw_cursor(gterm, fb, loaded_font);
}

void gterminal_clear_line_from(int col, gterminal_t* gterm, framebuffer_t* fb,
        psf1_font_t* loaded_font) {
    int row = gterm->cursor_y; // current row

    if (col < 0) col = 0;
    if (col >= gterm->cols) return;

    // Clear the rest of the line
    for (int x = col; x < gterm->cols; x++) {
        int px = x * 8;
        int py = row * loaded_font->glyph_size;

        for (uint32_t y = 0; y < loaded_font->glyph_size; y++) {
            for (uint32_t xx = 0; xx < 8; xx++) {
                fb_putpixel(fb, px + xx, py + y, gterm->bg);
            }
        }
    }

    // Move cursor to the start of cleared area
    gterm->cursor_x = col;
    gterminal_draw_cursor(gterm, fb, loaded_font);
}

void gterminal_set_colors(uint32_t fg, uint32_t bg, gterminal_t* gterm) {
    gterm->fg = fg;
    gterm->bg = bg;
}

void gterminal_writestring(const char* str, gterminal_t* gterm, framebuffer_t* fb,
        psf1_font_t* loaded_font) {
    while (*str) {
        gterminal_putchar(*str++, gterm, fb, loaded_font);
    }
}

void gterminal_decprint(uint32_t num, gterminal_t* gterm, framebuffer_t* fb,
        psf1_font_t* loaded_font) {
    char buffer[12]; // Enough for 32-bit number (max 10 digits) + null terminator
    int i = 0;

    // Handle 0 explicitly
    if (num == 0) {
        gterminal_putchar('0', gterm, fb, loaded_font);
        return;
    }

    // Build the string in reverse order
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Print in correct order (reverse of how we built it)
    while (--i >= 0) {
        gterminal_putchar(buffer[i], gterm, fb, loaded_font);
    }
}

uint32_t gterminal_getfg(gterminal_t* gterm) {
    return gterm->fg;
}

uint32_t gterminal_getbg(gterminal_t* gterm) {
    return gterm->bg;
}

