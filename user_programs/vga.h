#ifndef VGA_H
#define VGA_H

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

#define COLOR_BLACK       0x000000
#define COLOR_WHITE       0xFFFFFF
#define COLOR_RED         0xFF0000
#define COLOR_GREEN       0x00FF00
#define COLOR_BLUE        0x0000FF
#define COLOR_YELLOW      0xFFFF00
#define COLOR_CYAN        0x00FFFF
#define COLOR_MAGENTA     0xFF00FF
#define COLOR_GRAY        0x808080
#define COLOR_LIGHT_GRAY  0xC0C0C0
#define COLOR_DARK_GRAY   0x404040
#define COLOR_ORANGE      0xFFA500
#define COLOR_PURPLE      0x800080
#define COLOR_BROWN       0xA52A2A
#define COLOR_PINK        0xFFC0CB

#include <stdint.h>

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


#endif
