/* Level 00 moby class help_hints (0x585): requests the Veldin help messages when the hero enters their clip volumes or the guarded mobys fall. */
#include "types.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/hero/hero.h"
#include "rnc/game/usage_stats.h"
#include "rnc/rendering/clip_volume.h"

typedef struct {
    u8   pad0[0x20];
    u8   state;
    u8   pad21[0xF];
    u8   alpha;
    u8   pad31[0x47];
    s32 *vars;
} HelpMoby;

typedef struct {
    u8 pad0[0x20];
    u8 state;
} HelpTarget;

extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern u16  D_00141850[];
extern u16  D_00141968[];

void update_help_hints(HelpMoby *moby) __asm__("FUN_L00_002e0988");

void update_help_hints(HelpMoby *moby) {
    s32         *vars;
    s32         *target_index_ptr;
    s32          i;
    HelpTarget  *target_moby;
    struct Hero *hero_ptr;

    vars = moby->vars;
    if (moby->state == 0) {
        moby->state = 1;
        moby->alpha = 0xFF;
    }
    if (D_00141968[0x18 / 2] == 0) {
        help_request(3, 3);
    }
    if (D_00141968[0x3A0 / 2] == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[4]) != 0) {
            help_request(0x4E2A, 0x74);
        }
    }
    if (D_00141968[0x48 / 2] == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[0]) != 0 &&
            D_00141850[0] == 0) {
            help_request(5, 9);
        }
    }
    if (D_00141968[0x40 / 2] == 0 && usage_stats.stat[0].count == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[1]) != 0) {
            help_request(4, 8);
        }
    }
    if (D_00141968[0] == 0) {
        if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars[2]) != 0) {
            help_request(0, 0);
        }
    }
    if (D_00141968[0x8 / 2] == 0 && D_00141968[0x10 / 2] == 0) {
        hero_ptr = &hero;
        target_index_ptr = vars + 5;
        for (i = 0; i < 4; i++) {
            if (*target_index_ptr != -1) {
                target_moby = (HelpTarget *)(moby_table + (*target_index_ptr << 8));
                if (target_moby == 0 || target_moby->state == 0xFE || target_moby->state == 0xFD) {
                    if (hero_ptr->health.hp < 4) {
                        help_request(1, 1);
                    } else {
                        help_request(2, 2);
                    }
                }
            }
            target_index_ptr++;
        }
    }
}
