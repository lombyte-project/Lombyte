/* Spinning fragments (classes 0x43D-0x440): pieces of a break-apart that fall, spin and expire; their spawner picks one of the four at random. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00301FE8), where it is exact; names translated to the US level program. */

extern f32 D_L06_00162080_gp __asm__("D_L06_00162080") __attribute__((sda));

extern float scale_time(float) __asm__("FUN_001f96b0");

void update_falling_spinning_moby(struct Moby *moby) __asm__("FUN_L06_00300b90");

void update_falling_spinning_moby(struct Moby *moby) {
    char *data;
    float vertical_velocity_decrement = frame_time_sq * 15.0f;
    moby->scale = moby->pclass->scale * *(float *)&D_L06_00162080_gp;
    data = (char *)moby->pvars;
    *(float *)(data + 8) -= vertical_velocity_decrement;
    fast_vec_add(&moby->pos.x, &moby->pos.x, (float *)data);
    fast_vec_add(&moby->rot.x, &moby->rot.x, (float *)(data + 0x10));
    moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x10));
    moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x14));
    moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(data + 0x18));
    if (--moby->command == 0) {
        mark_moby_for_removal(moby);
    }
}

/* Spawns one fragment of the break-apart at pos, flying along vel with a
 * random spin. */
unsigned char *
spawn_spinning_moby_fragment(char  *unused_source_moby,
                             float *fragment_spawn_position,
                             float *fragment_initial_velocity) __asm__("FUN_L06_00300c60");

unsigned char *spawn_spinning_moby_fragment(char  *unused_source_moby,
                                            float *fragment_spawn_position,

                                            float *fragment_initial_velocity) {
    u128           position_copy, velocity_copy;
    u128          *pp = &position_copy;
    u128          *saved_velocity = &velocity_copy;
    unsigned char *fragment_moby;
    char          *fragment_vars;

    *pp = *(u128 *)fragment_spawn_position;
    *saved_velocity = *(u128 *)fragment_initial_velocity;
    fragment_moby = create_moby(truncate_float_to_s32(random_float_between(1085.0f, 1089.0f)));

    if (fragment_moby != 0) {
        fragment_vars = ((char *)MOBY(fragment_moby)->pvars);
        MOBY(fragment_moby)->scale =
            *(float *)(((char *)MOBY(fragment_moby)->pclass) + 0x24) * D_L06_00162080_gp;
        fragment_moby[0x30] = 0xFF;
        MOBY(fragment_moby)->draw_dist = 0x7E;
        fragment_moby[0x31] = 1;
        qcopy(fragment_moby + 0x10, pp);
        qcopy(fragment_vars, saved_velocity);
        *(float *)(fragment_vars + 0x10) =
            random_float_between(-360.0f, 360.0f) * DEG_TO_RAD * frame_time;
        *(float *)(fragment_vars + 0x14) =
            random_float_between(-360.0f, 360.0f) * DEG_TO_RAD * frame_time;
        *(float *)(fragment_vars + 0x18) =
            random_float_between(-360.0f, 360.0f) * DEG_TO_RAD * frame_time;
        fragment_moby[0xBC] =
            truncate_float_to_s32(scale_time(random_float_between(60.0f, 120.0f)));
        update_entity(fragment_moby);
    }
    return fragment_moby;
}

f32 D_L10_00161D6C = 0.1f;
f32 D_L13_00161D34 = 0.1f;
