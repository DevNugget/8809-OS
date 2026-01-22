#pragma once
#include <stdint.h>

#include "../string.h"

// FALLBACK THEME: Default theme colors
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

#include "../kernel/console.h"

int load_theme(const char* path, theme_t* theme, gterminal_t* gterm, framebuffer_t* fb, psf1_font_t* loaded_font);
