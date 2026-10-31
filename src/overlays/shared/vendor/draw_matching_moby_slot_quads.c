/* Vendor: draws the slot quads of matching mobys. */
#include "types.h"
#include "asm.h"

#include "sda.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/mobys/moby.h"

/* Walks the moby list matching this moby's id and pulses each match's data slots while it is bright. */

extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_matching_moby_slot_quads(char *self) __asm__("FUN_L02_002ecc68");

void draw_matching_moby_slot_quads(char *reference_moby) {
    struct Moby *current_moby;
    get_first_moby_in_group((int *)&current_moby, *(unsigned char *)(reference_moby + 0x21), 0, 0);
    while (current_moby != 0) {
        if (current_moby->visible != 0) {
            char *slot_quad_data = ((char *)current_moby->pvars);
            if (current_moby->oclass == *(short *)(reference_moby + 0xA6)) {
                if ((*(int *)(slot_quad_data + 0xe0) & 0xFF) > 0x80) {
                    char *slot_position = slot_quad_data + 0x80;
                    int   i;
                    for (i = 0x5; i >= 0; i--) {
                        draw_offset_textured_effect_quad(
                            slot_position, *(int *)(slot_quad_data + 0xe0), 1.2f, 0.57f);
                        slot_position += 0x10;
                    }
                }
            }
        }
        get_next_moby_in_group((int *)&current_moby, current_moby, 0, 0);
    }
}
