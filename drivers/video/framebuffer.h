#pragma once
#include <stdint.h>
#include "../../multiboot.h"

typedef struct {
    void* addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
} framebuffer_t;

void fb_init(framebuffer_t* fb, multiboot_info_t* mbd);
void fb_putpixel(framebuffer_t* fb, int x, int y, uint32_t color);
uint32_t fb_getpixel(int x, int y, framebuffer_t* fb);
void fb_clear(framebuffer_t* fb, uint32_t color);
