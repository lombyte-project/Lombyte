/* Level 09 moby class 0x3e8: help hints; the help message of its area, and the item unlock once every airship part is down. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "sda.h"

/* Unlocks item 14 with its voice line once no airship part updated last frame, records the level time and visit, and requests the help message in its clip volume. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00301C38), where it is exact; names translated to the US level program. */
extern int           D_0015EEA4;
extern unsigned char D_0013D4C2_q[] __asm__("D_0013D4C2");
extern unsigned char D_0013F3D0_lui[] __asm__("D_0013F3D0") NOT_SDA;
extern unsigned char D_00141968_lui[] __asm__("D_00141968") NOT_SDA;
void                 update_help_hints(struct Moby *moby) __asm__("FUN_L09_00300888");

void update_help_hints(struct Moby *moby) {
    int            *data = (int *)moby->pvars;
    unsigned char  *item_unlock_flags;
    unsigned char  *frame_and_level_record;
    unsigned short *tp;
    int             scaled_frame_quotient;
    if (D_L09_0015F5CC > 0) {
        if (D_L09_001613C8 == 0) {
            item_unlock_flags = item_unlocked;
            if (item_unlock_flags[0xE] == 0) {
                item_unlock_flags[0xE] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
                D_L09_001613C8 = 0;
            } else {
                D_L09_001613C8 = 0;
            }
        } else {
            D_L09_001613C8 = 0;
        }
    }
    frame_and_level_record = D_00141968_lui;
    tp = (unsigned short *)(frame_and_level_record + 0x2D8);
    if (*tp == 0) {
        if (D_0013D4C2_q[9] != 0) {
            (*tp)++;
        }
        scaled_frame_quotient = scale_game_frames(D_0015EEA4) / 600;
        if (*(unsigned short *)(frame_and_level_record + 0x2DA) < scaled_frame_quotient) {
            *(unsigned short *)(frame_and_level_record + 0x2DA) =
                scale_game_frames(D_0015EEA4) / 600;
        }
        *(int *)(frame_and_level_record + 0x2DC) =
            *(int *)(frame_and_level_record + 0x2DC) | (1 << current_level_index) | 0x80000000;
        if (is_point_inside_clip_volume(D_0013F3D0_lui, *data) != 0) {
            help_request(0x2329, 0x5B);
        }
    }
}
