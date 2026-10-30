/* Level 16 door (class 0x287) and its group helpers. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"

#include "qcopy.h"

extern float D_0015ED6C;

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float D_0015ED6C, frame_time;
extern float frame_time_sq;

extern float D_0015ED6C, D_0015EE70;
extern float frame_time_sq;

void door_update(unsigned char *moby) __asm__("FUN_L16_002d59e0");

void door_update(unsigned char *moby) {
    enum {
        DOOR_STATE_INITIALIZE = 0,
        DOOR_STATE_MOVING = 1,
        DOOR_STATE_IDLE = 2
    };
    char *door_data = ((char *)MOBY(moby)->pvars);
    char *door_position;
    char  position_offset[16];
    switch (moby[0x20]) {
    case DOOR_STATE_INITIALIZE:
        qcopy(door_data, moby + 0x10);
        moby[0x20] = 2;
        break;
    case DOOR_STATE_MOVING:
        door_position = (char *)moby + 0x10;
        qcopy(door_position, door_data);
        advance_accelerated_scalar(*(float *)(door_data + 0x14),
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   D_0015ED6C * 8.0f,
                                   (float *)(door_data + 0x18),
                                   (float *)(door_data + 0x10));
        fast_vec_normalize(position_offset, moby + 0xC0, *(float *)(door_data + 0x18));
        fast_vec_add(door_position, door_position, position_offset);
        if (*(float *)(door_data + 0x18) == *(float *)(door_data + 0x14)) {
            moby[0x20] = 2;
        }
        break;
    case DOOR_STATE_IDLE:
        break;
    }
}

extern char *D_L16_0015FFD8;

extern char D_L16_001671C0[];

extern int *D_L16_001ABCC0[];

#include "rnc/overlay/l16/mobys/door_globals.h"

/* Activate the linked mobys and return the one nearest the challenge target. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D6E98), where it is exact; names translated to the US level program. */

extern float class_0287_group_activation_value_gp __asm__("D_L16_00161A80") __attribute__((sda));

#include "rnc/overlay/l16/mobys/door.h"

void *set_class_0287_group_activation_and_find_nearest(int index,
                                                       int secondary) __asm__("FUN_L16_002d5ad0");

void *set_class_0287_group_activation_and_find_nearest(int index, int secondary) {
    short *group_entry = (short *)D_L16_001ABCC0[index];
    char  *nearest = 0;
    float  distance = 1000.0f;
    char  *moby;
    float *data;
    float  candidate_distance;
    float  activation_value;
    if (!group_entry)
        return 0;
    while (1) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass != 0x287)
            continue;
        candidate_distance = fast_vec_dist(D_L16_001671C0, moby + 0x10);
        if (candidate_distance < distance) {
            distance = candidate_distance;
            nearest = moby;
        }
        data = ((float *)MOBY(moby)->pvars);
        moby[0x20] = 1;
        activation_value = class_0287_group_activation_value_gp;
        data[5] = activation_value;
        if (secondary)
            data[6] = activation_value;
        if (*group_entry++ < 0)
            return nearest;
    }
}

/* Activate the linked mobys, clearing their timers, and return the one nearest the challenge target. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D6F90), where it is exact; names translated to the US level program. */

void *activate_class_0287_group_and_find_nearest(int index,
                                                 int secondary) __asm__("FUN_L16_002d5bc8");

void *activate_class_0287_group_and_find_nearest(int index, int secondary) {
    short *group_entry = (short *)D_L16_001ABCC0[index];
    char  *nearest = 0;
    float  distance = 1000.0f;
    char  *moby;
    int   *data;
    float  candidate_distance;
    if (!group_entry)
        return 0;
    while (1) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass != 0x287)
            continue;
        candidate_distance = fast_vec_dist(D_L16_001671C0, moby + 0x10);
        data = ((int *)MOBY(moby)->pvars);
        if (candidate_distance < distance) {
            distance = candidate_distance;
            nearest = moby;
        }
        moby[0x20] = 1;
        data[5] = 0;
        if (secondary)
            data[6] = 0;
        if (*group_entry++ < 0)
            return nearest;
    }
}
