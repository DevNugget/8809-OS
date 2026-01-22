#include "font_psf.h"
#include "framebuffer.h"

int load_psf1_font(const char* path, psf1_font_t* loaded_font) {
    FIL file;
    UINT read;

    if (f_open(&file, path, FA_READ) != FR_OK) {

        return -1; // failed to open
    }

    psf1_header_t header;
    f_read(&file, &header, sizeof(header), &read);

    if (header.magic[0] != 0x36 || header.magic[1] != 0x04) {
        //draw_example();
        f_close(&file);
        return -2; // invalid magic
    }

    uint32_t glyphs = (header.mode & 0x01) ? 512 : 256;
    uint32_t glyph_size = header.charsize;
    uint32_t font_data_size = glyphs * glyph_size;

    uint8_t* buffer = kmalloc(font_data_size); // replace with your own allocator
    if (!buffer) {
        //draw_example();
        f_close(&file);
        return -3; // allocation failed
    }

    f_read(&file, buffer, font_data_size, &read);
    f_close(&file);

    loaded_font->glyph_buffer = buffer;
    loaded_font->glyph_count = glyphs;
    loaded_font->glyph_size = glyph_size;

    return 0; // success
}

void draw_char_psf(int x, int y, char c, uint32_t color, psf1_font_t* loaded_font, framebuffer_t* fb) {
    uint8_t* glyph = &loaded_font->glyph_buffer[(uint8_t)c * loaded_font->glyph_size];

    for (int row = 0; (uint32_t)row < loaded_font->glyph_size; ++row) {
        uint8_t bits = glyph[row];
        for (int col = 0; (uint32_t)col < 8; ++col) {
            if (bits & (1 << (7 - col))) {
                fb_putpixel(fb, x + col, y + row, color);
            }
        }
    }
}



