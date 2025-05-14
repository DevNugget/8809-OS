#ifndef VGA_DRIVER_H
#define VGA_DRIVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "multiboot.h"
#include "util.h"
#include "string.h"
#include "heap.h"
#include "fatfs/ff.h"

#define VGA_WIDTH 90
#define VGA_HEIGHT 60
#define VGA_MEMORY 0xB8000

#define VGA_SEQUENCER_INDEX   0x3C4
#define VGA_SEQUENCER_DATA    0x3C5
#define VGA_GRAPHICS_INDEX    0x3CE
#define VGA_GRAPHICS_DATA     0x3CF
#define VGA_CRTC_INDEX        0x3D4
#define VGA_CRTC_DATA         0x3D5
#define FONT_MEM_LOCATION     0xA0000

typedef enum VGAColor {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} VGAColor;

#define GVGA_COLOR_BLACK        0x000000
#define GVGA_COLOR_BLUE         0x0000AA
#define GVGA_COLOR_GREEN        0x00AA00
#define GVGA_COLOR_CYAN         0x00AAAA
#define GVGA_COLOR_RED          0xAA0000
#define GVGA_COLOR_MAGENTA      0xAA00AA
#define GVGA_COLOR_BROWN        0xAA5500
#define GVGA_COLOR_LIGHT_GRAY   0xAAAAAA
#define GVGA_COLOR_DARK_GRAY    0x555555
#define GVGA_COLOR_LIGHT_BLUE   0x5555FF
#define GVGA_COLOR_LIGHT_GREEN  0x55FF55
#define GVGA_COLOR_LIGHT_CYAN   0x55FFFF
#define GVGA_COLOR_LIGHT_RED    0xFF5555
#define GVGA_COLOR_PINK         0xFF55FF
#define GVGA_COLOR_YELLOW       0xFFFF55
#define GVGA_COLOR_WHITE        0xFFFFFF

#define COLOR_BLACK       0x181825
#define COLOR_RED         0xF38BA8
#define COLOR_GREEN       0xA6E3A1
#define COLOR_YELLOW      0xF9E2AF
#define COLOR_BLUE        0x89B4FA
#define COLOR_MAGENTA     0xD2A6FF
#define COLOR_CYAN        0x89DCEB
#define COLOR_WHITE       0xCDD6F4
#define COLOR_GRAY        0x808080
#define COLOR_LIGHT_GRAY  0xC0C0C0

// Define lighter variants
#define COLOR_LIGHT_BLACK 0x6E6A76
#define COLOR_LIGHT_RED   0xF5A8B1
#define COLOR_LIGHT_GREEN 0xA6F3A1
#define COLOR_LIGHT_YELLOW 0xF9F1A1
#define COLOR_LIGHT_BLUE  0x8BB9F7
#define COLOR_LIGHT_MAGENTA 0xD2A9FF
#define COLOR_LIGHT_CYAN  0x8AC9F2
#define COLOR_LIGHT_WHITE 0xFFFFFF

typedef struct {
    uint32_t black;
    uint32_t red;
    uint32_t green;
    uint32_t yellow;
    uint32_t blue;
    uint32_t magenta;
    uint32_t cyan;
    uint32_t white;
    uint32_t gray;

    uint32_t light_black;
    uint32_t light_red;
    uint32_t light_green;
    uint32_t light_yellow;
    uint32_t light_blue;
    uint32_t light_magenta;
    uint32_t light_cyan;
    uint32_t light_white;
    uint32_t light_gray;
} theme_t;

static inline uint16_t vga_entry_color(VGAColor fg, VGAColor bg) {
    return (bg << 4) | (fg & 0x0F);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return ((uint16_t) color << 8) | uc;
}

size_t strlen(const char* str);

void terminal_init(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_movecsr(size_t x, size_t y);
char nibble_to_hex(uint8_t nibble);
void terminal_hexprint(uint32_t value);
void terminal_decprint(uint32_t num);
void terminal_scroll(void);
void terminal_clear();
void switch_to_80x50_mode();
void terminal_write_screen_size();
void gterminal_init(multiboot_info_t* mbd);
void draw_pixel(int x, int y, uint32_t color);
void draw_example();

typedef struct {
    uint8_t magic[2];      // 0x36 0x04
    uint8_t mode;          // bit 0: 512 glyphs if set, otherwise 256
    uint8_t charsize;      // bytes per glyph
} __attribute__((packed)) psf1_header_t;

typedef struct {
    uint8_t* glyph_buffer;
    uint32_t glyph_count;
    uint32_t glyph_size;
} psf1_font_t;

int load_psf1_font(const char* path);
void draw_char_psf(int x, int y, char c, uint32_t color);
void gterminal_writestring(const char* str);
void gterminal_set_colors(uint32_t fg, uint32_t bg);
void gterminal_clear();
uint32_t gterminal_getfg();
uint32_t gterminal_getbg();
int load_theme(const char* path, theme_t* theme);

#endif
