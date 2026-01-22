#include "ui.h"

ui_context_t ui;

void ui_init(multiboot_info_t* mbd) {
    fb_init(&ui.fb, mbd);
    load_psf1_font("fonts/aply16.psf", &ui.font);
    gterminal_init(&ui.term, &ui.fb, &ui.font);
}
