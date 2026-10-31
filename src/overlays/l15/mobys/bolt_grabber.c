/* Level 15 bolt grabber moby class. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/pickups/infobot.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern int D_L15_0015F5C4;

extern int D_L15_0015F640;

extern unsigned char D_0013D4FD[];

extern void grant_inventory_item(int, int) __asm__("FUN_L00_00260860");

extern void start_scene(int) __asm__("FUN_L00_00298840");

extern void emit_moby_sprite_particles(char *) __asm__("FUN_L10_00298940");

extern short bolt_grabber_scale_multiplier_gp __asm__("D_L15_00162078") __attribute__((sda));

void bolt_grabber_update(struct Moby *m) __asm__("FUN_L15_002ea748");

/* Moby class 1388 */
void bolt_grabber_update(struct Moby *moby) {
    enum {
        BOLT_GRABBER_INITIALIZE = 0,
        BOLT_GRABBER_WAIT_FOR_PROXIMITY = 1,
        BOLT_GRABBER_FINALIZE_AND_SAVE = 2,
        BOLT_GRABBER_REMOVE = 3
    };
    moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
    moby->scale = moby->pclass->scale * *(float *)&bolt_grabber_scale_multiplier_gp;
    switch (moby->state) {
    case BOLT_GRABBER_INITIALIZE:
        initialize_moby_particle_effect((char *)moby);
        if (D_0013D4FD[0xD] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->pos.z = moby->pos.z + 1.0f;
        break;
    case BOLT_GRABBER_WAIT_FOR_PROXIMITY:
        emit_moby_sprite_particles((char *)moby);
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 3.0f) {
            moby->flags |= 0x41;
            start_scene(5);
            moby->state = 2;
        }
        break;
    case BOLT_GRABBER_FINALIZE_AND_SAVE:
        if (D_L15_0015F5C4 != 2) {
            queue_dialogue_message(0x3AA3, -1);
            D_L15_0015F640 = scale_game_frames(0xB4);
            grant_inventory_item(0x22, 1);
            memcard_save_data(0, -1);
            moby->state = 3;
        }
        break;
    case BOLT_GRABBER_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}

extern float fast_add_rotations(float, float);
