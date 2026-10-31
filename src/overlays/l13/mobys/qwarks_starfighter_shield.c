/* Level 13 moby class qwarks_starfighter_shield (0x160): the translucent shell Qwark's starfighter raises in attack phase 4; it grows around the ship with a scrolling texture and fades out when the phase ends or the ship is gone. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern char *create_moby(int);

char *spawn_qwarks_starfighter_shield(char *owner) __asm__("FUN_L13_002e8438");

char *spawn_qwarks_starfighter_shield(char *owner) {
    char *moby = create_moby(0x160);
    if (moby != 0) {
        char *spawned_moby_vars;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0x3FF;
        moby[0x31] = 1;
        spawned_moby_vars = ((char *)MOBY(moby)->pvars);
        moby[0x20] = 0;
        moby[0xBC] = 0;
        qcopy(moby + 0x10, owner + 0x10);
        *(char **)spawned_moby_vars = owner;
        *(int *)(spawned_moby_vars + 4) = 0;
        MOBY(moby)->flags |= 0x200;
        update_entity(moby);
        copy_hero_moby_field_38(moby);
        MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24);
    }
    return moby;
}

extern void offset_moby_class_point_pairs(unsigned char *, int, int) __asm__("FUN_L00_0024efb0");

void qwarks_starfighter_shield_update(struct Moby *moby) __asm__("FUN_L13_002e84d8");

/* Moby class 352 */
void qwarks_starfighter_shield_update(struct Moby *moby) {
    enum {
        INITIALIZE_COLLISION = 0,
        FADE_IN = 1,
        WAIT_FOR_OWNER_REMOVAL = 2,
        FADE_OUT = 3,
        REMOVE_MOBY = 4
    };
    int *moby_data = (int *)moby->pvars;
    int  previous_point_pair_y_offset;
    int  step;

    switch (moby->state) {
    case INITIALIZE_COLLISION:
        moby->collision = moby->pclass->collision;
        moby->state = 1;
        break;
    case FADE_IN: {
        float fade = frame_scale * 0.02f;
        moby->scale += (moby->pclass->scale * 4.0f - moby->scale) * 0.1f;
        ((float *)moby_data)[1] += fade;
        if (((float *)moby_data)[1] >= 1.0f) {
            ((float *)moby_data)[1] = 1.0f;
            moby->state = 2;
        }
        moby->alpha = truncate_float_to_s32(((float *)moby_data)[1] * 60.0f);
        break;
    }
    case WAIT_FOR_OWNER_REMOVAL:
        moby->scale += (moby->pclass->scale * 4.0f - moby->scale) * 0.1f;
        if (moby_data[0] == 0 || ((unsigned char *)moby_data[0])[0x20] == 0xFE ||
            ((unsigned char *)moby_data[0])[0x20] == 0xFD) {
            moby->state = 3;
        }
        break;
    case FADE_OUT: {
        float fade = frame_scale * 0.02f;
        moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
        ((float *)moby_data)[1] -= fade;
        if (((float *)moby_data)[1] <= 0.0f) {
            ((float *)moby_data)[1] = 0.0f;
            moby->state = 4;
        }
        moby->alpha = truncate_float_to_s32(((float *)moby_data)[1] * 60.0f);
        break;
    }
    case REMOVE_MOBY:
        mark_moby_for_removal(moby);
        return;
    }
    previous_point_pair_y_offset = moby_data[2];
    step = -0x40;
    moby_data[2] = previous_point_pair_y_offset + step;
    if (moby_data[2] > 0x1000) {
        step = -0x1040;
        moby_data[2] = previous_point_pair_y_offset + step;
    } else if (moby_data[2] < 0) {
        step = 0xFC0;
        moby_data[2] = previous_point_pair_y_offset + step;
    }
    offset_moby_class_point_pairs(moby->pclass, 0, step);
}
