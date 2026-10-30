/* Level 14 tilting platform moby class and its tilt basis. */
#include "types.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/vector.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/world/collision/collision_line.h"
#include "qzero.h"

extern float D_L14_0015F580[] MACRO_ADDR;

extern void vendor_spring(void *, void *, float, float, float, float) __asm__("FUN_L00_0025b8c0");

extern void FUN_L14_002de2b8_call(struct Moby *, f32, f32) __asm__("FUN_L14_002de2b8");

#include "rnc/overlay/l14/mobys/platform_data/update_tilting_platform.h"

void update_tilting_platform(struct Moby *moby) __asm__("FUN_L14_002de1f8");

void update_tilting_platform(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    void *rotation;
    u128  old_rotation;
    qcopy(&old_rotation, &moby->rot);

    if (moby->state == 0) {
        moby->state = 1;
        moby->command = 0;
        moby->flags |= 0x100;
        moby->pos.z += 0.35f;
        *(int *)(data + 0x5C) = 1;
        *(int *)(data + 0x70) = 0;
        qzero(data + 0x60);
        qzero(&moby->rot);
        rotation = &moby->rot;
    } else {
        rotation = &moby->rot;
    }
    FUN_L14_002de2b8_call(moby, unused_platform_tilt_argument, 8.0f);
    update_entity_matrix(data + 0x20, D_L14_0015F580, &old_rotation, rotation);
}

/* Ported from rac1-decomp src/overlays/l14_oltanis/vendor_002ACCC0.c (func_L14_002DF6B8) */
#include "eetypes.h"

typedef int u128_s07 __attribute__((mode(TI)));

extern int allocate_voice_slot(void *, int, int, int, int) __asm__("FUN_0022d7f0");

#include "rnc/overlay/l14/mobys/platform_data/update_platform_tilt_basis.h"

extern char D_0013F3D0[];

/* The caller passes two floats in f12/f13; this routine never reads them. */
void update_platform_tilt_basis(char *moby,
                                f32   unused_arg1,
                                f32   unused_arg2) __asm__("FUN_L14_002de2b8");

void update_platform_tilt_basis(char *moby, f32 unused_arg1, f32 unused_arg2) {
    char *data = ((char *)MOBY(moby)->pvars);
    char *hero_state = (char *)((char *)((char *)&hero));
    float target_up_vector[4];
    float up[4];
    float direction_scratch[4];
    float ax[4];
    float direction_length;
    float dt;
    float tilt_angle;
    float max_tilt_angle;

    *(u128_s07 *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;

    if ((*(char **)(hero_state + 0x2FC) == moby && *(short *)(hero_state + 0x30E) == 0) ||
        (*(char **)(hero_state + 0x4F8) == moby && *(int *)(hero_state + 0x208C) == 3)) {
        char *hero_data = (char *)((char *)((char *)&hero));
        MOBY(moby)->command = 1;
        if (absolute_float(*(float *)(hero_data + 0x80) - MOBY(moby)->pos.x) < 0.1f &&
            absolute_float(*(float *)(hero_data + 0x84) - MOBY(moby)->pos.y) < 0.1f) {
            qcopy(target_up_vector, up);
        } else {
            fast_vec_sub(direction_scratch, (char *)((char *)&D_0013F3D0), moby + 0x10);
            direction_scratch[2] = direction_scratch[2] + platform_tilt_direction_z_offset;
            direction_length = fast_vec_length(direction_scratch);
            if (direction_length == 0.0f) {
                qcopy(target_up_vector, up);
            } else {
                dt = fast_vec_dot(up, direction_scratch);
                tilt_angle = 1.5707963705f - fast_arc_sin(dt / direction_length);
                max_tilt_angle = platform_max_tilt_degrees * 0.0174532924f;
                if (max_tilt_angle < tilt_angle) {
                    tilt_angle = max_tilt_angle;
                }
                fast_vec_cross(ax, direction_scratch, up);
                build_look_at_matrix(target_up_vector, up, ax, tilt_angle);
                fast_vec_normalize(target_up_vector, target_up_vector, 1.0f);
            }
        }
    } else {
        qcopy(target_up_vector, up);
        MOBY(moby)->command = 0;
    }

    if (fast_dec_timer(data + 0x70)) {
        if (0.1f < absolute_float(MOBY(moby)->rot_row2.x - target_up_vector[0]) ||
            0.1f < absolute_float(MOBY(moby)->rot_row2.y - target_up_vector[1]) ||
            0.1f < absolute_float(MOBY(moby)->rot_row2.z - target_up_vector[2])) {
            *(int *)(data + 0x70) = scale_game_frames(60);
            MOBY(moby)->pos.z = MOBY(moby)->pos.z + 2.0f;
            allocate_voice_slot(*(void **)(((char *)MOBY(moby)->pclass) + 0x28),
                                8,
                                (int)moby,
                                (int)(moby + 0x10),
                                0x400);
            MOBY(moby)->pos.z = MOBY(moby)->pos.z - 2.0f;
        }
    }

    vendor_spring((float *)&MOBY(moby)->rot_row2,
                  (float *)(data + 0x60),
                  target_up_vector[0],
                  0.005f,
                  0.2f,
                  0.0f);
    vendor_spring((float *)&MOBY(moby)->rot_row2.y,
                  (float *)(data + 0x64),
                  target_up_vector[1],
                  0.005f,
                  0.2f,
                  0.0f);
    vendor_spring((float *)&MOBY(moby)->rot_row2.z,
                  (float *)(data + 0x68),
                  target_up_vector[2],
                  0.005f,
                  0.2f,
                  0.0f);

    fast_vec_normalize(moby + 0xE0, moby + 0xE0, 1.0f);
    *(u128_s07 *)direction_scratch = 0;
    direction_scratch[0] = 1.0f;
    direction_scratch[3] = 1.0f;
    fast_vec_cross(moby + 0xD0, direction_scratch, moby + 0xE0);
    fast_vec_normalize(moby + 0xD0, moby + 0xD0, 1.0f);
    fast_vec_cross(moby + 0xC0, moby + 0xE0, moby + 0xD0);
    MOBY(moby)->rot_row0.w = 0.0f;
    MOBY(moby)->rot_row1.w = 0.0f;
    MOBY(moby)->rot_row2.w = 0.0f;
}
