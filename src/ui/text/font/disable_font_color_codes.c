#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

void disable_font_color_codes(void) __asm__("func_001F61F8");

void disable_font_color_codes(void) {
    font_color_codes_enabled = 0;
}
