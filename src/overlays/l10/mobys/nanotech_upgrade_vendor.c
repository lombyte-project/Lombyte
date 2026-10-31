/* Level 10 moby class nanotech_upgrade_vendor: sells the two health upgrades. */
#include "types.h"

#include "rnc/overlay/level_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/ui/help/try_set_help_message.h"

typedef struct Moby Moby;

/* Health upgrade vendor: sells two upgrades (4000 bolts: 5 hp, 30000 bolts: 8 hp) once each,
   then saves the game and shows the purchase message. */
extern void set_moby_dialogue_record_value(struct Moby *, int) __asm__("FUN_L00_00266858");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern int D_0013ED98 MACRO_ADDR;

#include "rnc/overlay/l10/mobys/nanotech_upgrade_vendor_globals.h"

extern int purchased_nanotech_health_gp __asm__("D_0013EDA0") __attribute__((sda));
extern int D_L10_0013F640 __asm__("D_L10_0013F640") MACRO_ADDR;

void nanotech_upgrade_vendor_update(struct Moby *moby) __asm__("FUN_L10_002e8358");

/* Moby class 1326 */
void nanotech_upgrade_vendor_update(struct Moby *moby) {
    int  *timer;
    float angle;
    int   purchase;
    int   upgrade_level;

    timer = (int *)moby->pvars;
    fast_dec_timer(timer + 1);
    if (moby->state != 0) {
        if (moby->state == 1)
            goto state_one;
        return;
    }
    {
        angle =
            fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y);
        if (fast_difference_between_rotations(moby->rot.z, angle) < 1.5707964f &&
            vector_distance_xy(&moby->pos, hero.motion.pos.f) < 4.0f && timer[1] == 0 &&
            D_L10_0015F5C4_vendor != 2) {
            set_moby_dialogue_record_value(moby, 1);
            if (alternate_item_available[4] == 0) {
                if (current_bolt_count >= 4001) {
                    int accepted = try_set_help_message(1, 0x271e) != 0;
                    if (!(D_0013CAE4 & 0x10))
                        return;
                    if (accepted == 0)
                        return;
                    alternate_item_available[4] = 1;
                    upgrade_level = 5;
                    hero.health.hp = upgrade_level;
                    current_bolt_count = D_0013ED98 - 4000;
                    purchased_nanotech_health_gp = upgrade_level;
                    start_scene(0);
                    moby->state = 1;
                } else {
                    try_set_help_message(1, 0x2720);
                    return;
                }
            } else {
                if (alternate_item_available[5] != 0 || timer[1] != 0)
                    return;
                if (current_bolt_count >= 30001) {
                    int accepted = try_set_help_message(1, 0x271f) != 0;
                    if (!(D_0013CAE4 & 0x10))
                        return;
                    if (accepted == 0)
                        return;
                    alternate_item_available[5] = 1;
                    upgrade_level = 8;
                    hero.health.hp = upgrade_level;
                    current_bolt_count = D_0013ED98 - 30000;
                    purchased_nanotech_health_gp = upgrade_level;
                    start_scene(1);
                    moby->state = 1;
                } else {
                    try_set_help_message(1, 0x2721);
                    return;
                }
            }
        }
    }
    return;
state_one:
    if (D_L10_0015F5C4_vendor != 2) {
        memcard_save_data(0, -1);
        switch (alternate_item_available[5]) {
        default:
            purchase = 0x271d;
            break;
        case TRUE:
            purchase = 0x271d;
            break;
        case FALSE:
            purchase = 0x271c;
            break;
        }
        queue_dialogue_message(purchase, -1);
        D_L10_0013F640 = 0xb4;
        timer[1] = scale_game_frames(0xf0);
        moby->state = 0;
    }
}
