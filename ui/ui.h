#pragma once

#include "../drivers/video/framebuffer.h"
#include "../drivers/video/font_psf.h"
#include "../drivers/video/gterminal.h"
#include "theme.h"

typedef struct {
    framebuffer_t fb;
    psf1_font_t font;
    gterminal_t term;
    theme_t theme;
} ui_context_t;

void ui_init(multiboot_info_t* mbd);

