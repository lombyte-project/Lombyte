/* HUD: initializes a numeric element. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/shared/ui/hud.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"

/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00237B20), where it is exact; names translated to the US level program. */

/* Sets unk7C to scale_game_frames(0xA) + 0x1E, clears 0x48/0x4A, w/h 0x20, calls 00235c80. */
void initialize_numeric_hud_element(HudElem *hud_element) __asm__("FUN_L00_00237190");

void initialize_numeric_hud_element(HudElem *hud_element) {
    hud_element->visible_timer = scale_game_frames(0xA) + 0x1E;
    hud_element->offset_x = 0;
    hud_element->offset_y = 0;
    hud_element->w = 0x20;
    hud_element->h = 0x20;
    initialize_numeric_hud_values_and_dimensions(hud_element);
}
