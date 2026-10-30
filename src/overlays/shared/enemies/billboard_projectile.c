/* Billboard projectile (class 0x9C): flies along its velocity, bursts into particles on collision and times out. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "qcopy.h"

extern char D_L01_00167240[];

extern char *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern char  D_L01_001742E0[];
extern float D_L01_001742C0[];
extern void  spawn_two_point_effect_particle(char *, char *) __asm__("FUN_L00_0026c6e8");

extern int noop_callback_l(void *) __asm__("FUN_001e9448");
extern int collect_mobies_in_sphere(float, void *, int, int, void *) __asm__("FUN_L00_001f2868");
extern void
FUN_L00_00259bc8_2c7530(float, void *, void *, int, void *, void *) __asm__("FUN_L00_00259bc8");
extern void spawn_type_14_particle(int, void *) __asm__("FUN_L01_00280970");
extern void FUN_0020c828(void *);

extern s32 moby_motion_particle_random_sign_base_gp __asm__("D_L01_00161494") __attribute__((sda));
extern f32 moby_motion_collision_radius_gp __asm__("D_L01_0016149C") __attribute__((sda));

/* spawns two bursts of particles around a point */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002C84D8), where it is exact; names translated to the US level program. */

void spawn_collision_particle_bursts(void) __asm__("FUN_L01_002c72c8");

void spawn_collision_particle_bursts(void) {
    float offset_vector[4];
    float spawn_position[4];
    float base_angle, b;
    int   i;
    fast_vec_sub(offset_vector, D_L01_00167240, D_L01_001742E0);
    base_angle = -fast_arc_tan(vector_length_xy(offset_vector), offset_vector[2]);
    fast_arc_tan(offset_vector[0], offset_vector[1]);
    for (i = 19; i >= 0; i--) {
        float x, y;
        x = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        y = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        build_spherical_offset(offset_vector, 0.2f, y, x);
        fast_vec_scale(spawn_position, offset_vector, random_float_between(1.0f, 3.0f));
        fast_vec_add(spawn_position, spawn_position, D_L01_001742E0);
        spawn_two_point_effect_particle((char *)spawn_position, (char *)offset_vector);
    }
    for (i = 9; i >= 0; i--) {
        float x, y;
        spawn_position[0] = D_L01_001742C0[8] + randf_sym(0.0f, 0.1f);
        spawn_position[1] = D_L01_001742C0[9] + randf_sym(0.0f, 0.1f);
        spawn_position[2] = D_L01_001742C0[10] + randf_sym(0.0f, 0.1f);
        x = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        y = fast_add_rotations(randf_sym(0.0f, 3.1415927f), base_angle);
        build_spherical_offset(offset_vector, 0.05f, y, x);
        spawn_burst(spawn_position,
                    offset_vector,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    random_int_between(scale_game_frames(10), scale_game_frames(20)),
                    1,
                    20000.0f);
    }
}

void update_billboard_projectile(char *moby) __asm__("FUN_L01_002c7530");

void update_billboard_projectile(char *moby) {
    enum {
        UPDATE_TRANSFORM_ONLY = 0,
        COLLISION_CHECK_AND_COUNTDOWN = 1,
        TIMED_REMOVAL = 2
    };
    int  *data = ((int *)MOBY(moby)->pvars);
    char *camera = D_L01_00167240;
    char  hit[0x30];
    float normal[4];
    float distance;

    distance = vector_distance_xy(camera, moby + 0x10);
    camera -= 0x140;
    MOBY(moby)->rot.y = -fast_arc_tan(distance, *(float *)(camera + 0x148) - MOBY(moby)->pos.z);
    MOBY(moby)->rot.z = fast_arc_tan(*(float *)(camera + 0x140) - MOBY(moby)->pos.x,
                                     *(float *)(camera + 0x144) - MOBY(moby)->pos.y);
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, *(float *)(data + 3));
    fast_vec_add(moby + 0x10, moby + 0x10, data + 8);
    switch ((u8)moby[0x20]) {
    case COLLISION_CHECK_AND_COUNTDOWN:
        spawn_type_14_particle(moby_motion_particle_random_sign_base_gp, moby);
        fast_vec_normalize(normal, data + 8, 1.0f);
        make_hit_descriptor(hit, moby, 0, 0.0f, normal);
        if (collect_mobies_in_sphere(
                moby_motion_collision_radius_gp, moby + 0x10, 0x10, data[6], hit) &&
            !noop_callback_l(moby)) {
            CollisionHit *contact = (CollisionHit *)D_L01_001742C0;
            if (contact->moby != 0)
                FUN_L00_00259bc8_2c7530(
                    1.0f, contact->moby, moby, 0x10001, &contact->point, normal);
            ((void (*)(void *))spawn_collision_particle_bursts)(moby);
            goto remove;
        }
        if (fast_dec_timer(data)) {
            data[0] = scale_game_frames(0x1e);
            moby[0x20] = 2;
            if (data[4] != 0)
                data[4] = 0;
        }
        break;
    case UPDATE_TRANSFORM_ONLY:
        break;
    case TIMED_REMOVAL:
        if (fast_dec_timer(data)) {
        remove:
            FUN_0020c828(moby);
        } else {
            int duration = scale_game_frames(0x1e);
            moby[0x23] = truncate_float_to_s32((float)data[0] * (127.0f / (float)duration));
        }
        break;
    }
}

s32 D_L06_00161464 = 8;
f32 D_L06_0016146C = 0.25f;
s32 D_L08_0016148C = 8;
f32 D_L08_00161494 = 0.25f;
s32 D_L09_001614F4 = 8;
f32 D_L09_001614FC = 0.25f;
s32 D_L15_001615F4 = 8;
f32 D_L15_001615FC = 0.25f;
s32 D_L17_0016154C = 8;
f32 D_L17_00161554 = 0.25f;
