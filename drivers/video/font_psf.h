#pragma once

#include <stdint.h>
#include "framebuffer.h"
#include "../../fatfs/ff.h"
#include "../../heap.h"

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

int load_psf1_font(const char* path, psf1_font_t* loaded_font);
void draw_char_psf(int x, int y, char c, uint32_t color, psf1_font_t* loaded_font, framebuffer_t* fb);


