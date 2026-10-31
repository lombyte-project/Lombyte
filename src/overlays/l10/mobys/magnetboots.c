/* Level 10 moby class magnetboots: the magneboots pickup. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/moby_spawn.h"

#include "rnc/overlay/l10/mobys/magnetboots_globals.h"

/* Spinning pickup: waits until the hero is close and level with it, then plays the scene, saves and goes away. */
extern char *D_L10_0015FFD8 __asm__("D_L10_0015FFD8");
extern void  initialize_moby_particle_effect(void *) __asm__("FUN_L00_002d6bf0");
extern void  emit_moby_sprite_particles(void *) __asm__("FUN_L10_00298940");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void  start_scene(int) __asm__("FUN_L00_00298840");
extern void  grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");

void magnetboots_update(struct Moby *m) __asm__("FUN_L10_00298668");

/* Moby class 18 */
void magnetboots_update(struct Moby *moby) {
    enum {
        MAGNETBOOTS_STATE_INITIALIZE = 0,
        MAGNETBOOTS_STATE_WAIT_FOR_HERO_PROXIMITY = 1,
        MAGNETBOOTS_STATE_WAIT_FOR_SCENE_EXIT = 2,
        MAGNETBOOTS_STATE_REMOVE = 3
    };
    char *pickup_vars = (char *)moby->pvars;
    moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
    switch (moby->state) {
    case MAGNETBOOTS_STATE_INITIALIZE:
        initialize_moby_particle_effect(moby);
        if (D_0013D504[0] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->pos.z = moby->pos.z + 0.5f;
        moby->scale = moby->scale * 0.667f;
        break;
    case MAGNETBOOTS_STATE_WAIT_FOR_HERO_PROXIMITY:
        emit_moby_sprite_particles(moby);
        if (vector_distance(&moby->pos, &hero.motion.pos) < 3.0f) {
            struct Hero *hero_ref = &hero;
            if (absolute_float(moby->pos.z - hero_ref->motion.pos.f[2]) < 2.0f &&
                hero_ref->health.hp != 0) {
                moby->flags |= 0x41;
                start_scene(2);
                moby->state = 2;
            }
        }
        break;
    case MAGNETBOOTS_STATE_WAIT_FOR_SCENE_EXIT:
        if (D_L10_0015F5C4_vendor != 2) {
            queue_dialogue_message(0x271B, -1);
            dialogue_message_duration = 0xB4;
            grant_inventory_item(0x1C, 1);
            if (*(int *)(pickup_vars + 4) != -1) {
                char *linked_moby = D_L10_0015FFD8 + (*(int *)(pickup_vars + 4) << 8);
                jet_reset_camera(linked_moby + 0x10, linked_moby + 0x40);
            }
            memcard_save_data(0, -1);
            moby->state = 3;
        }
        break;
    case MAGNETBOOTS_STATE_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}
