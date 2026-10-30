#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

void enable_font_color_codes(void) __asm__("func_001F61E8");

void enable_font_color_codes(void) {
    font_color_codes_enabled = 1;
}
