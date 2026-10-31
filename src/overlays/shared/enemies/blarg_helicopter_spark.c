/* Shared moby class 0x350: a spark that homes on its target moby. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_line.h"
#include "qcopy.h"
#include "rnc/overlay/entities.h"

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef int u128_2D5B88 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2D5B88;

typedef struct {
    Vec_2D5B88 vel;
    Moby      *target;
    int        timer;
    int        flip;
} Data;

typedef struct {
    Vec_2D5B88     dir;
    void          *owner;
    int            flags;
    unsigned char  b18;
    unsigned char  b19;
    unsigned short h1A;
    float          f1C;
    int            i20;
} Hit_2D5B88;

typedef struct {
    Vec_2D5B88 pos;
    Vec_2D5B88 v10;
    Vec_2D5B88 v20;
    Vec_2D5B88 v30;
    void      *moby;
} Tgt;

void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  FUN_L00_0024f7c8(void *, int, void *);
extern int select_moby_target_with_hero_fallback(void *, void *, float) __asm__("FUN_L03_0024e830");

extern Moby *D_L03_00173F58 NOT_SDA;
extern char                *FUN_L00_0026dd20(void *pos, void *vel, int c, int d, float f);
extern char                 D_0013E533[];
extern CollisionHit         coll_hit __asm__("D_L03_00173F60");
extern char                 D_L03_00173F80[];
extern float                frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
float                       homing_spark_turn_rate_degrees_gp __asm__("D_L03_00161B30") = 9e+01f;
extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void create_type_31_part(void *, void *, void *, void *, int) __asm__("FUN_L03_00260130");

/* Update of moby class 848: a spark that homes on its target moby, sheds particles and an arc when close, and dies on a hit. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002D5B88), where it is exact; names translated to the US level program. */

void helicopter_spark_update(Moby *moby) __asm__("FUN_L03_002d47c0");

/* Moby class 848 */
void helicopter_spark_update(Moby *moby) {
    enum {
        SPARK_HOME_AND_EMIT = 1,
        SPARK_CLEANUP_AND_REMOVE = 2
    };
    Tgt        target_info;
    Vec_2D5B88 start_pos;
    Hit_2D5B88 hit;
    Vec_2D5B88 direction;
    Vec_2D5B88 muzzle;
    Vec_2D5B88 aim;
    Vec_2D5B88 ray_end;
    Vec_2D5B88 side_offset;
    Vec_2D5B88 gun_pos;
    Vec_2D5B88 spark_vel;
    Vec_2D5B88 spark_dir;
    Vec_2D5B88 tracer_vel;
    Vec_2D5B88 tracer_offset;
    float      heading;
    int       *voice_slot = 0;
    Data      *homing_spark_pvars = (Data *)((char *)moby->pvars);
    int        state;
    int        i;
    int        j;
    float      max_climb;
    float      target_distance;

    select_moby_target_with_hero_fallback(moby, &target_info, 20.0f);
    if (homing_spark_pvars->target->oclass == 0x277) {
        voice_slot = (int *)(((char *)homing_spark_pvars->target->pvars) + 0x250);
    } else if (homing_spark_pvars->target->oclass == 0xE9) {
        voice_slot = (int *)(((char *)homing_spark_pvars->target->pvars) + 0x1B8);
        if (homing_spark_pvars->target->state == 0x40 || homing_spark_pvars->target->state == 1) {
            moby->state = 2;
        }
    }
    state = moby->state;
    switch (state) {
    case SPARK_HOME_AND_EMIT:
        set_vector_length_xy(&direction, homing_spark_pvars, 1.0f);
        homing_spark_pvars->vel.z = 1.0f;
        make_hit_descriptor(&hit, moby, 0x10003, 2.0f, &direction);
        set_vector_length_xy(&hit, &hit, 1.0f);
        hit.dir.z = 1.0f;
        hit.dir.w = 5627.9248f;
        hit.b18 = state;
        hit.b19 = state;
        hit.h1A = moby->oclass;
        qcopy(&start_pos, &moby->pos);
        homing_spark_pvars->vel.z = -(moby->pos.z - homing_spark_pvars->vel.w) / 10.0f;
        max_climb = frame_time_macro * 8.0f;
        if (homing_spark_pvars->vel.z > max_climb) {
            homing_spark_pvars->vel.z = max_climb;
        } else if (homing_spark_pvars->vel.z < -max_climb) {
            homing_spark_pvars->vel.z = -max_climb;
        }
        heading = fast_arc_tan(homing_spark_pvars->vel.x, homing_spark_pvars->vel.y);
        if (fast_difference_between_rotations(
                heading,
                fast_arc_tan(target_info.pos.x - moby->pos.x, target_info.pos.y - moby->pos.y)) <
            1.5707964f) {
            step_angle_toward_target(
                &heading,
                0,
                fast_arc_tan(target_info.pos.x - moby->pos.x, target_info.pos.y - moby->pos.y),
                homing_spark_turn_rate_degrees_gp * 0.017453292f * frame_time_macro);
            homing_spark_pvars->vel.x = fast_cos(heading) * vector_length_xy(homing_spark_pvars);
            homing_spark_pvars->vel.y = fast_sin(heading) * vector_length_xy(homing_spark_pvars);
        }
        fast_vec_add(&moby->pos, &moby->pos, homing_spark_pvars);
        target_distance = vector_distance_xy(&moby->pos, &homing_spark_pvars->target->pos);
        if (voice_slot != 0) {
            int voice = *voice_slot;
            if (homing_spark_pvars->target->pos.z - target_info.pos.z < target_distance) {
                if (voice_is_playing(homing_spark_pvars->target, voice) == 0) {
                    *voice_slot =
                        allocate_voice_for_target_entry(0, 4, (int)homing_spark_pvars->target);
                }
            } else {
                if (voice != -1) {
                    char *entry = ((char *)&voice_pool) + voice * 0x70;
                    if (*(Moby **)(entry + 0x88) == homing_spark_pvars->target &&
                        *(unsigned char *)(entry + 0x74) != 0) {
                        release_voice_slot(voice);
                    }
                }
                *voice_slot = -1;
            }
        }
        if (homing_spark_pvars->target->pos.z - target_info.pos.z < target_distance) {
            if (homing_spark_pvars->timer % scale_game_frames(6) == 0) {
                homing_spark_pvars->flip ^= 1;
                side_offset.x = fast_cos(fast_add_rotations(
                                    heading, homing_spark_pvars->flip ? 1.5707964f : -1.5707964f)) *
                                0.25f;
                side_offset.y = fast_sin(fast_add_rotations(
                                    heading, homing_spark_pvars->flip ? 1.5707964f : -1.5707964f)) *
                                0.25f;
                side_offset.z = 0.0f;
                fast_vec_add(&gun_pos, &moby->pos, &side_offset);
                FUN_L00_0024f7c8(homing_spark_pvars->target, 3 - homing_spark_pvars->flip, &muzzle);
                fast_vec_sub(&aim, &gun_pos, &muzzle);
                fast_vec_scale(&ray_end, &aim, 2.0f);
                fast_vec_add(&ray_end, &ray_end, &muzzle);
                for (i = 0; i < 5; i++) {
                    *(u128_2D5B88 *)&spark_dir = 0;
                    spark_dir.x = random_float_between(-1.0f, 1.0f);
                    spark_dir.y = random_float_between(-1.0f, 1.0f);
                    spark_dir.z = random_float_between(-1.0f, 1.0f);
                    *(u128_2D5B88 *)&spark_vel = *(u128_2D5B88 *)&spark_dir;
                    fast_vec_normalize(&spark_vel, &spark_vel, fast_vec_length(&aim) * 0.65f);
                    fast_vec_add(&spark_vel, &aim, &spark_vel);
                    fast_vec_normalize(
                        &spark_vel,
                        &spark_vel,
                        random_float_between(frame_time_macro * 5.0f, frame_time_macro * 15.0f));
                    FUN_L00_0026dd20(
                        &muzzle,
                        &spark_vel,
                        0x5F2F4F6F,
                        random_int_between(scale_game_frames(5), scale_game_frames(10)),
                        50000.0f);
                }
                fast_vec_normalize(&aim, &aim, frame_time_macro * 0.5f);
                FUN_L00_0026dd20(&muzzle,
                                 &aim,
                                 0x2F4F7F7F,
                                 random_int_between(scale_game_frames(4), scale_game_frames(7)),
                                 400000.0f);
                fast_vec_scale(&spark_dir, homing_spark_pvars, scale_time(6.0f));
                fast_vec_add(&spark_dir, &spark_dir, &moby->pos);
                fast_vec_sub(&spark_dir, &spark_dir, &side_offset);
                fast_vec_sub(&tracer_vel, &spark_dir, &muzzle);
                fast_vec_normalize(&spark_dir, &tracer_vel, 2.0f);
                fast_vec_sub(&tracer_offset, &muzzle, &spark_dir);
                *(u128_2D5B88 *)&spark_vel = *(u128_2D5B88 *)&tracer_offset;
                fast_vec_add(&spark_dir, &spark_dir, &spark_vel);
                fast_vec_normalize(&tracer_vel, &tracer_vel, frame_time_macro * 100.0f);
                create_type_31_part(homing_spark_pvars->target,
                                    &spark_vel,
                                    &spark_dir,
                                    &tracer_vel,
                                    scale_game_frames(0x2D));
                if (collision_line(&muzzle, &ray_end, 0, homing_spark_pvars->target, 0) != 0) {
                    qcopy(&muzzle, &coll_hit);
                    fast_vec_normalize(&aim, &aim, frame_time_macro * 10.0f);
                    for (j = 0; j < 10; j++) {
                        *(u128_2D5B88 *)&spark_dir = 0;
                        spark_dir.x = random_float_between(-1.0f, 1.0f);
                        spark_dir.y = random_float_between(-1.0f, 1.0f);
                        spark_dir.z = random_float_between(-1.0f, 1.0f);
                        *(u128_2D5B88 *)&spark_vel = *(u128_2D5B88 *)&spark_dir;
                        reflect_vector(&spark_dir, &aim, D_L03_00173F80);
                        fast_vec_normalize(
                            &spark_vel, &spark_vel, fast_vec_length(&spark_dir) * 0.5f);
                        fast_vec_add(&spark_dir, &spark_dir, &spark_vel);
                        fast_vec_normalize(
                            &spark_dir,
                            &spark_dir,
                            random_float_between(frame_time_macro * 3.0f, frame_time_macro * 6.0f));
                        FUN_L00_0026dd20(
                            &muzzle,
                            &spark_dir,
                            0x7F2F4F6F,
                            random_int_between(scale_game_frames(0xF), scale_game_frames(0x14)),
                            30000.0f);
                    }
                    fast_vec_normalize(&aim, &aim, -1.0f);
                    fast_vec_add(&ray_end, &muzzle, &aim);
                    if (collision_line(&muzzle, &ray_end, 0x10, homing_spark_pvars->target, &hit) !=
                            0 &&
                        D_L03_00173F58 != 0 &&
                        (D_L03_00173F58 == ((Hero *)(D_0013E533 + 0xE1D))->moby ||
                         D_L03_00173F58->oclass == 0x1DF)) {
                        moby->state = 2;
                    }
                    qcopy(&ray_end, &muzzle);
                    ray_end.z += 2.0f;
                    if (collision_line(&muzzle, &ray_end, 0x10, homing_spark_pvars->target, &hit) !=
                            0 &&
                        D_L03_00173F58 != 0 &&
                        (D_L03_00173F58 == ((Hero *)(D_0013E533 + 0xE1D))->moby ||
                         D_L03_00173F58->oclass == 0x1DF)) {
                        moby->state = 2;
                    }
                    if (collect_mobies_in_sphere(0.5f, &moby->pos, 0x10, moby, &hit) != 0 &&
                        D_L03_00173F58 != 0 &&
                        (D_L03_00173F58 == ((Hero *)(D_0013E533 + 0xE1D))->moby ||
                         D_L03_00173F58->oclass == 0x1DF)) {
                        moby->state = 2;
                    }
                }
            }
        }
        if (fast_dec_timer(&homing_spark_pvars->timer) != 0 || homing_spark_pvars->target == 0 ||
            homing_spark_pvars->target->state == 0xFE ||
            homing_spark_pvars->target->state == 0xFD) {
            moby->state = 2;
        }
        break;
    case SPARK_CLEANUP_AND_REMOVE:
        if (voice_slot != 0) {
            int voice = *voice_slot;
            if (voice != -1) {
                char *entry = ((char *)&voice_pool) + voice * 0x70;
                if (*(Moby **)(entry + 0x88) == homing_spark_pvars->target &&
                    *(unsigned char *)(entry + 0x74) != 0) {
                    release_voice_slot(voice);
                }
            }
            *voice_slot = -1;
        }
        mark_moby_for_removal(moby);
        break;
    }
}

float D_L07_00161AB8 = 9e+01f;
float D_L15_00161E40 = 9e+01f;
