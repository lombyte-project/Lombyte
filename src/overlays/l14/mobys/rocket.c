/* Level 14 rocket moby class (922): flight to its path point, exhaust puffs, and removal with its target. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/quad.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/l14/mobys/puff.h"
#include "rnc/overlay/l14/mobys/rocket_globals.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float interpolate_angle(float, float, float) __asm__("FUN_L00_0025b750");

extern unsigned char *D_L14_0015FFD8;

extern float vector_length_xy(void *);

typedef struct {
    s32 point;  /* 0x0: path point index into l14_cuboids (0x80 each, position at 0x30) */
    s32 target; /* 0x4: moby index removed on arrival */
    s32 sound;  /* 0x8: voice slot, -1 when none */
    s32 timer;  /* 0xC */
} FlyerVars;

extern s32 rocket_initial_timer_frames_gp __asm__("D_L14_00161FB0") __attribute__((sda));

extern f32 rocket_travel_speed_gp __asm__("D_L14_00161FA8") __attribute__((sda));

extern f32 rocket_steering_blend_fraction_gp __asm__("D_L14_00161FAC") __attribute__((sda));

extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern void spawn_rocket_exhaust_puffs(struct Moby *) __asm__("FUN_L14_002fded0");

void rocket_update(struct Moby *moby) __asm__("FUN_L14_002fdbb8");

/* Moby class 922 */
void rocket_update(struct Moby *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_IDLE = 1,
        ROCKET_STATE_FLY_TO_POINT = 2
    };
    FlyerVars   *rocket_vars = (FlyerVars *)moby->pvars;
    Vec4f        destination_delta;
    OvlVec4      step;
    f32          distance_to_destination;
    f32          speed;
    char        *slot;
    struct Moby *target;

    switch (moby->state) {
    case ROCKET_STATE_INITIALIZE:
        if (rocket_vars->point < 0 || rocket_vars->target < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->pos.z += 1.5f;
        rocket_vars->sound = -1;
        rocket_vars->timer = scale_game_frames(rocket_initial_timer_frames_gp);
        return;
    case ROCKET_STATE_IDLE:
        return;
    case ROCKET_STATE_FLY_TO_POINT:
        fast_vec_sub(
            &destination_delta, (char *)l14_cuboids + rocket_vars->point * 0x80 + 0x30, &moby->pos);
        distance_to_destination = fast_vec_length(&destination_delta);
        fast_dec_timer(&rocket_vars->timer);
        speed = rocket_travel_speed_gp * frame_time;
        if (distance_to_destination < speed || distance_to_destination <= 0.0f) {
            if (rocket_vars->sound != -1) {
                slot = ((char *)&voice_pool) + rocket_vars->sound * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && ((u8 *)slot)[0x74] != 0) {
                    release_voice_slot(rocket_vars->sound);
                }
            }
            target = (struct Moby *)(D_L14_0015FFD8 + rocket_vars->target * 0x100);
            rocket_vars->sound = -1;
            if (target != 0) {
                mark_moby_for_removal(target);
            }
            qcopy(&moby->pos, (char *)l14_cuboids + rocket_vars->point * 0x80 + 0x30);
            qcopy(&step, &moby->rot_row0);
            step.f[2] = 1.0f;
            spawn_explosion(moby,
                            &step,
                            &moby->pos,
                            0.0f,
                            0.0f,
                            0x14,
                            0xA,
                            0xC,
                            10.0f,
                            6.0f,
                            9.0f,
                            0,
                            1.0f,
                            30.0f,
                            1,
                            0xA,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, 2.0f * DEG_TO_RAD);
        moby->rot.y = interpolate_angle(
            moby->rot.y,
            -fast_arc_tan(vector_length_xy(&destination_delta), destination_delta.z),
            rocket_steering_blend_fraction_gp);
        moby->rot.z = interpolate_angle(moby->rot.z,
                                        fast_arc_tan(destination_delta.x, destination_delta.y),
                                        rocket_steering_blend_fraction_gp);
        fast_vec_scale(&step, &moby->rot_row0, speed);
        fast_vec_add((u8 *)&moby->pos, (u8 *)&moby->pos, (u8 *)&step);
        spawn_rocket_exhaust_puffs(moby);
        if (voice_is_playing(moby, rocket_vars->sound) == 0) {
            rocket_vars->sound = allocate_voice_for_target_entry(1, 4, (int)moby);
        }
        return;
    }
}

typedef float PuffVec[4] __attribute__((aligned(16)));

extern float D_L14_0015F580;

extern float rocket_puff_initial_right_offset_gp __asm__("D_L14_00161F90") __attribute__((sda));

extern float rocket_puff_forward_offset_gp __asm__("D_L14_00161F94") __attribute__((sda));

extern float rocket_puff_up_offset_gp __asm__("D_L14_00161F98") __attribute__((sda));

extern float rocket_puff_middle_right_offset_delta_gp __asm__("D_L14_00161F9C")
    __attribute__((sda));

extern float rocket_puff_final_right_offset_gp __asm__("D_L14_00161FA0") __attribute__((sda));

int D_L14_00161FA4 = 0x7f204080;

void spawn_rocket_exhaust_puffs(struct Moby *source_moby) __asm__("FUN_L14_002fded0");

void spawn_rocket_exhaust_puffs(struct Moby *source_moby) {
    PuffVec  A;
    PuffVec  B;
    PuffVec  C;
    L14Puff *puff;
    int      i, j, puff_signed_amount, puff_lifetime_frames, k, col;
    float    puff_size;

    fast_vec_scale(A, &source_moby->rot_row0, rocket_puff_initial_right_offset_gp);
    fast_vec_add(B, &source_moby->pos, A);
    fast_vec_scale(A, &source_moby->rot_row2, rocket_puff_up_offset_gp);
    fast_vec_add(B, B, A);
    qcopy(C, B);
    fast_vec_scale(A, &source_moby->rot_row1, rocket_puff_forward_offset_gp);
    fast_vec_add(B, B, A);
    for (i = 1; i >= 0; i--) {
        puff_signed_amount = random_integer_below(0x10);
        k = random_integer_below(2) == 0 ? puff_signed_amount : -puff_signed_amount;
        puff = spawn_puff(B, 0.2f, 1.0f, 0.9f, k, &D_L14_0015F580, 200000.0f, D_L14_00161FA4);
        if (puff != 0) {
            L14PuffTail *f = &puff->tail;

            puff->life = scale_game_frames(0xC);
            f->mode = 2;
            f->tint = 0x7F;
            f->fade = puff->life;
        }
    }
    fast_vec_scale(A, &source_moby->rot_row0, rocket_puff_middle_right_offset_delta_gp);
    puff_size = 100000.0f;
    fast_vec_add(B, B, A);
    puff_signed_amount = 0x10;
    puff_lifetime_frames = scale_game_frames(2);
    for (j = 0; j < 3; j++) {
        puff = spawn_puff(
            B, 0.05f, 1.0f, 1.0f, puff_signed_amount, &D_L14_0015F580, puff_size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *f = &puff->tail;

            puff->life = puff_lifetime_frames;
            puff->shade = random_integer_below(0xFF);
            f->mode = 2;
            f->tint = 0x7F;
            f->fade = puff->life;
        }
        puff_signed_amount = -puff_signed_amount;
        puff_lifetime_frames = puff_lifetime_frames << 1;
        puff_size = puff_size - 20000.0f;
    }
    puff_size = 300000.0f;
    fast_vec_scale(A, &source_moby->rot_row0, rocket_puff_final_right_offset_gp);
    fast_vec_add(B, C, A);
    col = random_int_between(0x40, 0x80);
    k = (col << 8) | 0x7F000000;
    col = col | ((col << 16) | k);
    if (random_integer_below(3)) {
        L14Puff *final_puff;

        puff_signed_amount = random_integer_below(4);
        k = random_integer_below(2) == 0 ? puff_signed_amount : -puff_signed_amount;
        final_puff = spawn_puff(B, 0.05f, 1.0f, 1.0f, k, &D_L14_0015F580, puff_size, col);
        if (final_puff != 0) {
            L14PuffTail *f = &final_puff->tail;

            final_puff->life = scale_game_frames(0x3C);
            if (random_integer_below(2)) {
                final_puff->alpha = 0x44;
            }
            f->tint = 0x40;
            f->mode = 2;
            f->fade = scale_game_frames(0x3C);
        }
    }
}

void mark_moby_and_linked_moby_for_removal(struct Moby *moby) __asm__("FUN_L14_002fe238");

void mark_moby_and_linked_moby_for_removal(struct Moby *moby) {
    char *moby_vars = ((char *)moby->pvars);
    char *linked_moby = D_L14_0015FFD8 + (*(int *)(moby_vars + 4) << 8);
    if (linked_moby != 0 && linked_moby[0x20] >= 0)
        mark_moby_for_removal(linked_moby);
    if (((char)moby->state) >= 0)
        mark_moby_for_removal(moby);
}
