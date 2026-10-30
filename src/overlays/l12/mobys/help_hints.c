/* Level 12 help hints controller (class 0x1a6): Hoven help messages and the group-cleared dialogue. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/game/item_state.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/collision.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
/* Plays a dialogue once the moby group is cleared, updates the help records (count, time, level mask)
 * and requests help messages 0x6D (in the clip volume), 0x6E (too long in hero state 0x81
 * without steering) and 0x3E. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_002EE600), where it is exact; names translated to the US level program. */

extern char D_00141968_h[] __asm__("D_00141968");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

void update_help_hints(struct Moby *moby) __asm__("FUN_L12_002ed280");

void update_help_hints(struct Moby *moby) {
    int *moby_vars = (int *)moby->pvars;
    if (count_active_moby_group_members(moby_vars[3], -1) == 0) {
        unsigned char *dialogue_trigger_state = item_unlocked;
        if (dialogue_trigger_state[0x14] == 0) {
            dialogue_trigger_state[0x14] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
    }
    {
        char *h = D_00141968_h;
        if (*(unsigned short *)(h + 0x368) == 0) {
            struct Hero *q = &hero;
            if (q->state.current == 0x81) {
                *(unsigned short *)(h + 0x368) = *(unsigned short *)(h + 0x368) + 1;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > *(unsigned short *)(h + 0x36A)) {
                *(unsigned short *)(h + 0x36A) = scale_game_frames(D_0015EEA4) / 600;
            }
            *(unsigned int *)(h + 0x36C) =
                *(unsigned int *)(h + 0x36C) | (1 << current_level_index) | 0x80000000;
            if (is_point_inside_clip_volume(&q->motion.pos, moby_vars[0])) {
                help_request(0x2EE3, 0x6D);
            }
        }
    }
    {
        char *h = D_00141968_h;
        if (*(unsigned short *)(h + 0x370) == 0 ||
            (*(unsigned short *)(h + 0x370) < 2 &&
             scale_game_frames(D_0015EEA4) - *(unsigned short *)(h + 0x372) * 600 >
                 scale_game_frames(72000))) {
            struct Hero *q = &hero;
            if (q->state.current == 0x81) {
                moby_vars[1]++;
                if ((pad_held[0] & 3) && 0.3f < q->stick_magnitude) {
                    moby_vars[2]++;
                } else {
                    moby_vars[2] = 0;
                }
                if (scale_game_frames(0x28) < moby_vars[2]) {
                    char *help_records = D_00141968_h;
                    moby_vars[1] = 0;
                    if (*(unsigned short *)(help_records + 0x370) <= 0xFFFE) {
                        *(unsigned short *)(help_records + 0x370) =
                            *(unsigned short *)(help_records + 0x370) + 1;
                    }
                    if (scale_game_frames(D_0015EEA4) / 600 >
                        *(unsigned short *)(help_records + 0x372)) {
                        *(unsigned short *)(help_records + 0x372) =
                            scale_game_frames(D_0015EEA4) / 600;
                    }
                    *(unsigned int *)(help_records + 0x374) =
                        *(unsigned int *)(help_records + 0x374) | (1 << current_level_index) |
                        0x80000000;
                }
                {
                    int   help_threshold_step_frames = scale_game_frames(0x1C20);
                    int   help_threshold_base_frames = scale_game_frames(0x708);
                    char *k = D_00141968_h;
                    if (*(unsigned short *)(k + 0x370) * help_threshold_step_frames +
                            help_threshold_base_frames <
                        moby_vars[1]) {
                        help_request(0x2EE4, 0x6E);
                    }
                }
            }
        }
    }
    /* Fakematch: the pointer-plus form (not item_unlocked[0xBC]) keeps a second address load;
     * the indexed form makes the function 8 bytes longer. */
    if (*(item_unlocked + 0xBC) != 0) {
        char *k = D_00141968_h;
        if (*(unsigned short *)(k + 0x1F0) == 0) {
            help_request(0x2EE0, 0x3E);
        }
    }
}
