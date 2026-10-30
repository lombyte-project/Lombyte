/* Level 16 test_dummy_door and its lookup helpers. */
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

extern float D_0015ED6C;

extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7080), where it is exact; names translated to the US level program. */

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern int D_L16_0015F5CC;
extern s32 SubtractIntegerWithClamp(s32);

#include "rnc/overlay/l16/mobys/test_dummy_door_globals.h"

extern float test_dummy_door_height_offset_gp __asm__("D_L16_00161A84") __attribute__((sda));
extern int   last_test_dummy_door_sound_request_frame_gp __asm__("D_L16_00161A88")
    __attribute__((sda));
void test_dummy_door_update(struct Moby *moby) __asm__("FUN_L16_002d5cb8");

void test_dummy_door_update(struct Moby *moby) {
    enum {
        TEST_DUMMY_DOOR_STATE_INITIALIZE = 0,
        TEST_DUMMY_DOOR_STATE_UPDATE_HEIGHT = 1
    };
    char *data = (char *)moby->pvars;
    switch (moby->state) {
    case TEST_DUMMY_DOOR_STATE_INITIALIZE:
        moby->state = 1;
        *(float *)data = moby->pos.z;
        break;
    case TEST_DUMMY_DOOR_STATE_UPDATE_HEIGHT: {
        int   timer_result = fast_dec_timer((int *)(data + 8));
        float target_height;
        if (timer_result != 0) {
            if (timer_result == 2) {
                if (SubtractIntegerWithClamp(last_test_dummy_door_sound_request_frame_gp -
                                             D_L16_0015F5CC) >= 4) {
                    last_test_dummy_door_sound_request_frame_gp = D_L16_0015F5CC;
                    allocate_voice_for_target_entry(1, 0, moby);
                }
            }
            target_height = *(float *)data;
        } else {
            target_height = *(float *)data + test_dummy_door_height_offset_gp;
        }
        advance_accelerated_scalar(target_height,
                                   frame_time_sq * 10.0f,
                                   frame_time_sq * 10.0f,
                                   D_0015ED6C * 10.0f,
                                   &moby->pos.z,
                                   (float *)(data + 4));
        break;
    }
    }
}

extern char *D_L16_0015FFD8;

#include "rnc/overlay/l16/mobys/test_dummy_door.h"

extern int *D_L16_001ABCC0[];

/* The linked moby of class 0x28A nearest a position, within 37 units. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7178), where it is exact; names translated to the US level program. */

void *find_nearest_test_dummy_door(int index, void *position) __asm__("FUN_L16_002d5db0");

void *find_nearest_test_dummy_door(int index, void *position) {
    short *group_entry = (short *)D_L16_001ABCC0[index];
    char  *nearest = 0;
    float  distance = 37.0f;
    char  *moby;
    float  candidate_distance;
    if (!group_entry)
        return 0;
    while (1) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass != 0x28A)
            continue;
        candidate_distance = fast_vec_dist(position, moby + 0x10);
        if (candidate_distance < distance) {
            distance = candidate_distance;
            nearest = moby;
        }
        if (*group_entry++ < 0)
            return nearest;
    }
}

extern int scale_game_frames(int);

extern s32 FUN_001f99a8_5e80(s32) __asm__("FUN_001f99a8");

void refresh_test_dummy_door_timer(struct Moby *moby) __asm__("FUN_L16_002d5e80");

void refresh_test_dummy_door_timer(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    if (*(int *)(moby_data + 8) == 0) {
        if (FUN_001f99a8_5e80(last_test_dummy_door_sound_request_frame_gp - D_L16_0015F5CC) >= 4) {
            last_test_dummy_door_sound_request_frame_gp = D_L16_0015F5CC;
            allocate_voice_for_target_entry(0, 0, moby);
        }
    }
    *(int *)(moby_data + 8) = scale_game_frames(5);
}
