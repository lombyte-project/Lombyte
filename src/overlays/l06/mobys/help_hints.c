/* Level 06 moby class 0x544: help hints, the help messages of the level's areas. */
#include "types.h"
#include "asm.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/usage_stats.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/game/item_state.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"

extern struct UsageStat help_records[] __asm__("D_00141968");
extern char            *D_L06_0015FFD8_83b0 __asm__("D_L06_0015FFD8");
extern u8               level06_help_hint_variant_selector[] __asm__("D_0013D4CE");

/* Level 6 help hints: shows the help messages for the area and records how often and when each was due. */
void help_hints_update(struct Moby *moby) __asm__("FUN_L06_003083b0");

/* Moby class 1348 */
void help_hints_update(struct Moby *moby) {
    int         *help_hint_reference_indices = (int *)moby->pvars;
    struct Moby *indexed_moby;
    int          i;
    int          j;
    int          elapsed;
    if (help_records[0x2A].count == 0 &&
        is_point_inside_clip_volume(&hero.motion.pos, help_hint_reference_indices[9])) {
        help_request(0x1773, 0x2A);
    }
    if (help_records[0x29].count == 0) {
        for (i = 0; i < 6; i++) {
            if (help_hint_reference_indices[i] != -1 &&
                ((struct Moby *)(D_L06_0015FFD8_83b0 + (help_hint_reference_indices[i] << 8)))
                        ->state != 1) {
                help_request(0x1772, 0x29);
            }
        }
    }
    if (help_records[0x2E].count == 0 || help_records[0x2F].count == 0) {
        struct Moby *target =
            (struct Moby *)(D_L06_0015FFD8_83b0 + (help_hint_reference_indices[10] << 8));
        if (target == 0 || target->state == 0xFE || target->state == 0xFD) {
            if (level06_help_hint_variant_selector[0]) {
                help_request(0x1777, 0x2E);
                if (help_records[0x2F].count <= 0xFFFE) {
                    help_records[0x2F].count++;
                }
                if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x2F].unk2) {
                    help_records[0x2F].unk2 = scale_game_frames(D_0015EEA4) / 600;
                }
                help_records[0x2F].level_mask =
                    help_records[0x2F].level_mask | (1 << current_level_index) | 0x80000000;
            } else {
                help_request(0x1778, 0x2F);
                if (help_records[0x2E].count <= 0xFFFE) {
                    help_records[0x2E].count++;
                }
                if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x2E].unk2) {
                    help_records[0x2E].unk2 = scale_game_frames(D_0015EEA4) / 600;
                }
                help_records[0x2E].level_mask =
                    help_records[0x2E].level_mask | (1 << current_level_index) | 0x80000000;
            }
        }
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, help_hint_reference_indices[8]) &&
        count_active_moby_group_members(help_hint_reference_indices[6], -1) == 0) {
        for (j = 0; j < 6; j++) {
            indexed_moby =
                (struct Moby *)(D_L06_0015FFD8_83b0 + (help_hint_reference_indices[j] << 8));
            if (help_hint_reference_indices[j] != -1 && indexed_moby != 0 &&
                indexed_moby->state != 0xFE && indexed_moby->state != 0xFD &&
                indexed_moby->state != 1) {
                if (help_records[0x28].count != 0) {
                    elapsed = scale_game_frames(D_0015EEA4) - help_records[0x28].unk2 * 600;
                    if ((int)(scale_game_frames(18) * 60.0f) < elapsed ||
                        help_records[0x28].unk2 * 600 == 0) {
                        help_request(0x1771, 0x28);
                    } else if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x28].unk2) {
                        help_records[0x28].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                } else {
                    if (help_records[0x28].count <= 0xFFFE) {
                        help_records[0x28].count++;
                    }
                    if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x28].unk2) {
                        help_records[0x28].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                    help_records[0x28].level_mask =
                        help_records[0x28].level_mask | (1 << current_level_index) | 0x80000000;
                }
            }
            if (indexed_moby == 0 || indexed_moby->state == 0xFE || indexed_moby->state == 0xFD) {
                help_records[0x28].count = 0xFFFF;
            }
        }
    }
    if (alternate_item_available[0x25] == 0) {
        struct Moby *trigger =
            (struct Moby *)(D_L06_0015FFD8_83b0 + (help_hint_reference_indices[11] << 8));
        if (help_hint_reference_indices[11] != -1 && trigger->command != 0) {
            alternate_item_available[0x25] = 1;
        }
    }
}
