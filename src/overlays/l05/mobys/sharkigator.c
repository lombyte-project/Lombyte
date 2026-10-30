/* Level 05 moby class sharkigator and its water effects. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "macros.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/props/grow_and_fade_out.h"
#include "qcopy.h"

extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern float D_L05_00161388 __attribute__((sda));
extern float D_L05_001612E0_macro[4] __asm__("D_L05_001612E0") MACRO_ADDR;
extern char  D_L05_001671C0[] __asm__("D_L05_001671C0");

extern int   random_integer_below(int) __asm__("FUN_00213260");
extern void  mark_moby_2d1688(char *) __asm__("FUN_0020c828");
extern void  set_action_2d1688(char *, int, int, int) __asm__("FUN_00212f90");
extern float steer_2d1688(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void  fast_vec_normalize(float, void *, void *) __asm__("FUN_001f9bf8");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float absolute_float(float) __asm__("func_001F99C0");
extern float
advance_accelerated_scalar(float *, float, float *, float, float, float) __asm__("FUN_00213f38");
extern int  point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int  allocate_voice_for_target_entry_2d1688(int, int, char *) __asm__("FUN_0022da68");
extern void hero_respawn(void) __asm__("FUN_L00_00211250");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void spawn_sharkigator_surface_effects(char *) __asm__("FUN_L05_002d1e30");
extern void spawn_sharkigator_splash_effects(int, char *) __asm__("FUN_L05_002d1fd8");

void                sharkigator_update(char *moby) __asm__("FUN_L05_002d1688");
extern float        fast_vec_dist(void *, void *);
typedef struct Hero Hero;

void sharkigator_update(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    int   state = MOBY(moby)->state;
    int   one = 1;
    char *path;
    char *point;
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    float z[4] __attribute__((aligned(16)));
    float speed;
    float height;
    int   index;
    int   duration;

    switch (state) {
    case 0:
        goto state_zero;
    case 1:
        goto state_one;
    case 2:
        goto state_two;
    case 3:
        goto state_three;
    default:
        return;
    }
state_zero: {
    float height;
    if (*(int *)(data + 0x64) != 0) {
        MOBY(moby)->state = 2;
        MOBY(moby)->flags |= 1;
        MOBY(moby)->update_dist = 0xff;
        *(float *)(data + 0x74) = MOBY(moby)->pos.z;
        *(int *)(data + 0x7c) = scale_game_frames(30);
        return;
    }
    index = *(int *)(data + 0x60);
    if (index == -1) {
        mark_moby_2d1688(moby);
        return;
    }
    *(int *)(data + 0x68) = random_integer_below(*(int *)l05_paths[index]);
    *(int *)(data + 0x6c) = scale_game_frames(600);
    if (MOBY(moby)->prev_seq != 1) {
        set_action_2d1688(moby, 1, 0, scale_game_frames(10));
    }
    height = *(float *)(((char *)l05_paths[*(int *)(data + 0x60)]) + 0x18) - (D_L05_00161388);
    MOBY(moby)->pos.z = height;
    *(float *)(data + 0x80) = height;
    MOBY(moby)->state = one;
    return;
}
state_one: {
    path = ((char *)l05_paths[*(int *)(data + 0x60)]);
    qcopy(v, path + *(int *)(data + 0x68) * 16 + 0x10);
    point = moby + 0x48;
    {
        float y = v[1];
        float x = v[0];
        height = fast_arc_tan(x - MOBY(moby)->pos.x, y - MOBY(moby)->pos.y);
    }
    {
        float pi = 3.1415927f;
        float turn_step = frame_time_sq * pi;
        steer_2d1688((float *)(point),
                     (float *)(data + 0x70),
                     height,
                     turn_step,
                     turn_step,
                     frame_time * pi);
    }
    fast_vec_normalize(frame_time * 3.0f, w, moby + 0xc0);
    fast_vec_add(moby + 0x10, moby + 0x10, w);
    height = fast_vec_dist(moby + 0x10, v);
    if (height < 3.0f || fast_dec_timer(data + 0x6c) != 0) {
        index = random_integer_below(*(int *)l05_paths[*(int *)(data + 0x60)]);
        *(int *)(data + 0x68) =
            (*(int *)(data + 0x68) + index) % *(int *)l05_paths[*(int *)(data + 0x60)];
        *(int *)(data + 0x6c) = scale_game_frames(600);
    }
    if ((hero.state.control_mode == 0x12 || hero.state.current == 0x12) &&
        vector_distance_xy(moby + 0x10, &hero.motion.pos) < 8.0f) {
        speed = (D_L05_001612E0_macro[2] - D_L05_00161388) - 2.0f;
    } else {
        speed = (D_L05_001612E0_macro[2] - D_L05_00161388) - 0.5f;
        spawn_sharkigator_surface_effects(moby);
    }
    advance_accelerated_scalar((float *)&MOBY(moby)->pos.z,
                               speed,
                               (float *)(data + 0x78),
                               frame_time_sq + frame_time_sq,
                               frame_time_sq + frame_time_sq,
                               frame_time + frame_time);
    return;
}
state_two: {
    char *path;
    fast_dec_timer(data + 0x7c);
    height = absolute_float(hero.water_surface_z - *(float *)(data + 0x74));
    if (height < 0.33f && hero.state.control_mode == 0x12) {
        char *point;
        if (*(int *)(data + 0x7c) != 0)
            return;
        point = D_L05_001671C0 - 0x140;
        MOBY(moby)->rot.y = -1.5533431f;
        MOBY(moby)->rot.z = fast_add_rotations(*(float *)(point + 0x158), 1.5707964f);
        euler_rotation_basis(moby + 0xc0, moby + 0x40);
        qcopy(moby + 0x10, &hero.motion.pos);
        fast_vec_normalize((D_L05_00161388), v, moby + 0xe0);
        fast_vec_sub(moby + 0x10, moby + 0x10, v);
        {
            float speed = frame_time * 10.0f;
            MOBY(moby)->pos.z -= 2.0f;
            *(float *)(data + 0x78) = speed;
        }
        MOBY(moby)->flags &= 0xfffe;
        if (MOBY(moby)->prev_seq != state)
            set_action_2d1688(moby, 2, 0, scale_game_frames(5));
        MOBY(moby)->command = 0;
        MOBY(moby)->state = 3;
        jet_mount(point + 0x140, point + 0x150, 0, 0, 0);
        return;
    }
    if (hero.state.current == 0x12)
        return;
    path = ((char *)l05_paths[*(int *)(data + 0x60)]);
    if (point_in_path_xy(&hero.motion.pos, path + 0x10, *(int *)path) != 0)
        *(int *)(data + 0x7c) = scale_game_frames(600);
    else
        *(int *)(data + 0x7c) = scale_game_frames(180);
    return;
}
state_three: {
    float speed;
    {
        struct Hero *h = &hero;
        if (h->water_surface_z < MOBY(moby)->pos.z || *(float *)(data + 0x78) < 0.0f) {
            if (h->state.current != 0x72) {
                qcopy_nc(v, &h->motion.pos);
                {
                    /* Fakematch: the pin and the empty barriers order the call setup like
                           retail ($5 before the v[2] store, $4 after it, in the jal slot). */
                    register char *splash_pos __asm__("$5") = (char *)v;
                    BARRIER();
                    v[2] = h->water_surface_z;
                    BARRIER();
                    spawn_sharkigator_splash_effects((int)moby, splash_pos);
                }
                set_race_camera(&h->motion.pos, &h->motion.rot, 0x72, 0);
                /* Fakematch: the two join statements are repeated in both branches so
                       that a real insn sits between set_camera (the last call of this
                       block) and the block-ending branch; jump2 cross-jumps the identical
                       tails back into one join (same bytes). With the call adjacent to the
                       branch, haifa add_branch_dependences makes it depend on every insn,
                       including the a0=moby move, which then ties with a1 and the water
                       store for the 2-wide issue and the store takes the jal delay slot;
                       retail has a0 in the slot. Sibling rocket.c FUN_L18_002a7220 gets the
                       same order from a real intermediate call. */
                h->unk20A5 = one;
                h->motion.pos.f[2] += 20.0f;
            } else {
                qcopy(&h->motion.pos, moby + 0x10);
                h->unk20A5 = one;
                h->motion.pos.f[2] += 20.0f;
            }
            /* Fakematch: a gcse-time set of moby so PRE does not hoist the
                   *(moby + 0x18) load below out of the join; folds to nothing. */
            moby++;
            moby--;
            speed = *(float *)(data + 0x78);
        } else {
            speed = *(float *)(data + 0x78);
        }
    }
    {
        float acceleration = frame_time_sq * 10.0f;
        float threshold = frame_time + frame_time;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + speed;
        {
            float speed = *(float *)(data + 0x78) - acceleration;
            *(float *)(data + 0x78) = speed;
            if (speed < threshold) {
                fast_vec_normalize((D_L05_00161388), v, moby + 0xe0);
                steer_2d1688((float *)&MOBY(moby)->rot.y,
                             (float *)(data + 0x70),
                             1.5533431f,
                             frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f,
                             frame_time * 25.132742f);
                euler_rotation_basis(moby + 0xc0, moby + 0x40);
                fast_vec_normalize((D_L05_00161388), w, moby + 0xe0);
                fast_vec_sub(z, w, v);
                fast_vec_sub(moby + 0x10, moby + 0x10, z);
            }
        }
    }
    if (*(float *)(data + 0x78) < 0.0f) {
        if (MOBY(moby)->pos.z < hero.water_surface_z && MOBY(moby)->command == 0) {
            MOBY(moby)->command = 1;
            allocate_voice_for_target_entry_2d1688(0, 0, moby);
            fast_vec_normalize((D_L05_00161388), v, moby + 0xe0);
            fast_vec_add(v, v, moby + 0x10);
            v[2] = hero.water_surface_z;
            spawn_sharkigator_splash_effects((int)moby, (char *)v);
        }
    }
    if (*(float *)(data + 0x78) < 0.0f && MOBY(moby)->pos.z < *(float *)(data + 0x74) - 8.0f)
        hero_respawn();
    if ((MOBY(moby)->anim_flags & 2) != 0 && MOBY(moby)->prev_seq != 1)
        set_action_2d1688(moby, 1, 0, scale_game_frames(10));
}
}

typedef float V2d1e30[4] __attribute__((aligned(16)));
extern float  random_float_between(float, float) __asm__("FUN_002132a8");
extern void   fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern char  *spawn_2d1e30(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");
extern float  D_L05_001612E8 __asm__("D_L05_001612E8");
extern char   D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;

/* When the timer runs out, spawns two effects beside the moby, one to each side, and restarts the timer. */
void spawn_sharkigator_surface_effects(char *moby) __asm__("FUN_L05_002d1e30");

void spawn_sharkigator_surface_effects(char *moby) {
    V2d1e30 spawn_position;
    char   *data;
    char   *spawned_effect;
    float   opposite_heading;
    int     i;
    float  *spawn_height_ptr;

    data = ((char *)MOBY(moby)->pvars);
    if (fast_dec_timer(data + 0x84) == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        opposite_heading = fast_add_rotations(MOBY(moby)->rot.z, 3.14159274f);
        fast_add_rotations(opposite_heading, random_float_between(-0.785398006f, 0.785398006f));
        fast_vec_scale(spawn_position, moby + 0xD0, random_float_between(-0.5f, 0.5f));
        fast_vec_add(spawn_position, spawn_position, moby + 0x10);
        spawn_position[2] = D_L05_001612E8 + 0.05f;
        spawn_height_ptr = &D_L05_001612E8;
        spawned_effect = spawn_2d1e30(spawn_position,
                                      D_L05_0015F580_macro,
                                      spawn_height_ptr,
                                      random_float_between(0.7f, 1.0f),
                                      i == 0 ? 2.0f : -2.0f);
        if (spawned_effect != 0) {
            *(short *)(spawned_effect + 0xA) = scale_game_frames(15);
        }
        *(int *)(data + 0x84) = truncate_float_to_s32(random_float_between(3.0f, 5.0f));
    }
}

/* Spawns a burst of particles and a ring of sparks around a position. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002D3220), where it is exact; names translated to the US level program. */

extern char  D_L05_0015F580_macro[] __asm__("D_L05_0015F580") MACRO_ADDR;
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float D_L05_001612E8;
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern unsigned char             *
spawn_splash(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");

void spawn_sharkigator_splash_effects(int unused, char *position) __asm__("FUN_L05_002d1fd8");

void spawn_sharkigator_splash_effects(int unused, char *position) {
    float effect_spawn_vector[4];
    char *spawned_object;
    int   i;
    char *base;
    int   random_spawn_value;
    spawned_object = spawn_burst_moby(position, 3.0f);
    if (spawned_object)
        spawned_object[0x23] = 0x70;
    for (base = (char *)&D_L05_001612E8, i = 15; i >= 0; i--) {
        float effect_xy_heading = random_angle_radians();
        float k = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
        effect_spawn_vector[0] = fast_cos(effect_xy_heading) * k;
        effect_spawn_vector[1] = fast_sin(effect_xy_heading) * k;
        effect_spawn_vector[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
        random_spawn_value = random_int_between(0x5A, 0x78);
        spawn_debris_chunk(
            position, effect_spawn_vector, random_integer_below(2), random_spawn_value);
    }
    for (i = 0; i < 16; i++) {
        unsigned char *splash;
        random_vector(effect_spawn_vector, 1.0f, 1.0f);
        fast_vec_add(effect_spawn_vector, effect_spawn_vector, position);
        effect_spawn_vector[2] = D_L05_001612E8 + 0.05f;
        splash = spawn_splash(effect_spawn_vector,
                              D_L05_0015F580_macro,
                              base,
                              random_float_between(0.7f, 1.0f),
                              i == 0 ? 2.0f : -2.0f);
        if (splash) {
            *(short *)(splash + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(30.0f, 60.0f)));
        }
    }
}
