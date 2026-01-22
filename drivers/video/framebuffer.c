#include "framebuffer.h"

void fb_init(framebuffer_t* fb, multiboot_info_t* mbd) {
    fb->addr = (void *)(uintptr_t)(mbd->framebuffer_addr);
    fb->width = mbd->framebuffer_width;
    fb->height = mbd->framebuffer_height;
    fb->bpp = mbd->framebuffer_bpp;
    fb->pitch = mbd->framebuffer_pitch;
}

void fb_putpixel(framebuffer_t* fb, int x, int y, uint32_t color) {
    if (x < 0 || (uint32_t)x >= fb->width || y < 0 || (uint32_t)y >= fb->height)
        return;

    uint8_t *pixel_ptr = (uint8_t *)fb->addr + y * fb->pitch + x * (fb->bpp / 8);

    // Assuming framebuffer_bpp is 32 (ARGB)
    if (fb->bpp == 32) {
        *(uint32_t *)pixel_ptr = color;
    } else if (fb->bpp == 24) {
        pixel_ptr[0] = color & 0xFF;         // Blue
        pixel_ptr[1] = (color >> 8) & 0xFF;  // Green
        pixel_ptr[2] = (color >> 16) & 0xFF; // Red
    } else if (fb->bpp == 16) {
        // Convert RGB to 5:6:5
        uint16_t r = (color >> 16) & 0xFF;
        uint16_t g = (color >> 8) & 0xFF;
        uint16_t b = color & 0xFF;
        uint16_t rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *(uint16_t *)pixel_ptr = rgb565;
    }
}

uint32_t fb_getpixel(int x, int y, framebuffer_t* fb) {
    if (x < 0 || x >= (int)fb->width) return 0;
    if (y < 0 || y >= (int)fb->height) return 0;

    uint16_t* addr = (uint16_t*)fb->addr;
    return addr[y * fb->width + x];
}

void fb_clear(framebuffer_t* fb, uint32_t color) {
    for (int y = 0; (uint32_t)y < fb->height; ++y) {
        for (int x = 0; (uint32_t)x < fb->width; ++x) {
            fb_putpixel(fb, x, y, color);
        }
    }
    //gterm_cursor_x = 0;
    //gterm_cursor_y = 0;
}
