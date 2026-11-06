/* HUD: updates a counter. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/shared/ui/hud.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/ui/hud/draw_weapon_ammo_hud_panel.h"

/* Eases a counter's shown value toward its target and animates its two drifting digits. */
/* Ported from rac1-decomp (src/overlays/shared/hud_00235960.c: func_L00_00236830), where it is exact; names translated to the US level program. */

void hud_update_counter(HudElem *hud_element) __asm__("FUN_L00_00235ea0");

void hud_update_counter(HudElem *hud_element) {
    unsigned char *counter_state_bytes = hud_element->cnt;
    int            counter_value;
    int            cap;
    int            value_distance;
    int            easing_step;
    int            x;
    unsigned char  easing_accumulator_value;

    if (hud_element->value_source != 0) {
        counter_value = *hud_element->value_source;
        cap = hud_element->max_value;
        if (counter_value < 0) {
            counter_value = 0;
        }
        hud_element->target_value = counter_value;
        if (cap < counter_value) {
            hud_element->target_value = cap;
        }
    }
    if (hud_element->shown_value != hud_element->target_value) {
        hud_element->visible_timer = scale_game_frames(0xB4);
        if (hud_element->slide >= 0x18) {
            x = hud_element->shown_value - hud_element->target_value;
            value_distance = x;
            value_distance = value_distance < 0 ? -value_distance : value_distance;
            if (value_distance != 0) {
                easing_step = truncate_float_to_s32(
                    square_root(convert_integer_to_float(value_distance) / 25.0f) * 5.0f);
                easing_step =
                    easing_step < value_distance * scale_game_frames(2) / scale_game_frames(10)
                        ? value_distance * scale_game_frames(2) / scale_game_frames(10)
                        : easing_step;
                if (easing_step >= 0x7A) {
                    easing_step = 0x79;
                } else if (easing_step <= 0) {
                    easing_step = 1;
                }
                counter_state_bytes[3] += easing_step;
                easing_accumulator_value = counter_state_bytes[3];
                if (scale_game_frames(2) < easing_accumulator_value) {
                    easing_step = counter_state_bytes[3] / scale_game_frames(2);
                    if (hud_element->target_value < hud_element->shown_value) {
                        hud_element->shown_value = hud_element->shown_value - easing_step;
                    } else {
                        hud_element->shown_value = hud_element->shown_value + easing_step;
                    }
                    counter_state_bytes[3] -= easing_step * scale_game_frames(2);
                }
            }
        }
    }
    if (hud_element->visible_timer >= scale_game_frames(5)) {
        if (counter_state_bytes[0] < scale_game_frames(8)) {
            counter_state_bytes[0]++;
        } else if (counter_state_bytes[1] < scale_game_frames(8)) {
            counter_state_bytes[1]++;
        }
    } else {
        if (counter_state_bytes[1] != 0 || counter_state_bytes[0] != 0) {
            hud_element->slide = 1;
        }
        if (counter_state_bytes[1] != 0) {
            counter_state_bytes[1]--;
        } else if (counter_state_bytes[0] != 0) {
            counter_state_bytes[0]--;
        }
    }
    update_hud_animation_frame((char *)hud_element + 0x40);
}
