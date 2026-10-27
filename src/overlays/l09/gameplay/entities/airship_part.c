int find_nearest_moby_by_class() __asm__("FUN_L09_002c2610");

int spawn_airship_part() __asm__("FUN_L09_002c2500");

/* Airship parts update. Its own unit: it reads the spawn counter D_L09_001613C8 through gp, where FUN_L09_00300888 in asteroid.c reads it through lui. */
#include "types.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/hero.h"
#include "asm.h"
#include "qcopy.h"

extern s32 FUN_00120478_2c26c8(f32) __asm__("FUN_00120478");
extern void FUN_001f9a10_2c26c8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28_2c26c8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void get_moby_joint_matrices(void *, int, int *, void *) __asm__("FUN_00210850");
extern void FUN_L00_0024f7c8_2c26c8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
typedef struct Data_2c26c8 {
    float vel[4];
    char pad10[0x10];
    float f20;
    float f24;
    short h28;
    short h2A;
    struct Moby *parent;
    float off[4];
    float f40;
    float f44;
    float f48;
    float f4C;
    float f50;
    float f54;
    float f58;
    float f5C;
    float f60;
    float f64;
    int f68;
    struct Moby *parts[9];
} Data_2c26c8;
typedef struct { char pad0[0x38]; unsigned char b38; unsigned char b39; } Flags_2c26c8;
typedef struct { int v[2]; } Pair_2c26c8;
/* one scratch slot of the frame: a joint matrix in state 0, two vectors (rows 0 and 1) elsewhere, and the effect id pair in state 4 */
typedef union { float m[4][4]; Pair_2c26c8 pair; } Buf_2c26c8;

extern int FUN_001e93b0_2c26c8() __asm__("FUN_001e93b0");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void FUN_001fa050_2c26c8(void *, void *) __asm__("FUN_001fa050");
extern void FUN_001f9d20_2c26c8(void *, void *, void *) __asm__("FUN_001f9d20");
extern void FUN_L00_00257470_2c26c8(void *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025f090_2c26c8(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_001f99f8_2c26c8(void *) __asm__("FUN_001f99f8");
extern char *FUN_00218888_2c26c8(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void FUN_001f9a68_2c26c8(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001fa580_2c26c8(float, float) __asm__("FUN_001fa580");
extern int FUN_001f9770_2c26c8(void *) __asm__("FUN_001f9770");
extern int FUN_001f96f8_2c26c8(int) __asm__("FUN_001f96f8");
extern int FUN_L00_00257b90_2c26c8(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_0022da68_2c26c8(int, int, int) __asm__("FUN_0022da68");
extern void FUN_001f9bf8_2c26c8(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_002132a8_2c26c8(float, float) __asm__("FUN_002132a8");
extern void FUN_0020c828_2c26c8(void *) __asm__("FUN_0020c828");
extern int FUN_00213260_2c26c8(int) __asm__("FUN_00213260");
extern void FUN_L00_0026a9f0_2c26c8(void *, void *, int, int, int, int, int, int, float, float) __asm__("FUN_L00_0026a9f0");
extern float FUN_00213308_2c26c8(void) __asm__("FUN_00213308");
extern float FUN_001f9dc8_2c26c8(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2c26c8(float) __asm__("FUN_001f9de0");
extern float FUN_001f96b0_2c26c8(float) __asm__("FUN_001f96b0");
extern int truncate_float_to_s32_r(float) __asm__("FUN_001fa6d0");
extern char D_0013D388_2c26c8[] __asm__("D_0013D388");

extern char D_L09_002082A0[];
extern Pair_2c26c8 D_L09_001613D0 __attribute__((section(".data")));
#include "rnc/overlay/l09/gameplay/entities/airship_part_globals.h"
extern int airship_part_low_altitude_effect_count_gp __asm__("D_L09_00161368") __attribute__((sda));
extern int D_L09_0016136C __attribute__((sda));
extern int D_L09_00161370 __attribute__((sda));
extern float D_L09_00161374 __attribute__((sda));
extern float D_L09_00161378 __attribute__((sda));
extern float D_L09_0016137C __attribute__((sda));
extern float D_L09_00161380 __attribute__((sda));
extern int D_L09_00161384 __attribute__((sda));
extern int D_L09_00161388 __attribute__((sda));
extern int D_L09_0016138C __attribute__((sda));
extern float D_L09_00161390 __attribute__((sda));
extern float D_L09_00161394 __attribute__((sda));
#include "rnc/overlay/l09/gameplay/entities/airship_part_data/update_airship_part_3.h"
extern float airship_part_initial_tumble_x_speed_limit_degrees_gp __asm__("D_L09_001613B0") __attribute__((sda));
extern float airship_part_initial_tumble_y_speed_limit_degrees_gp __asm__("D_L09_001613B4") __attribute__((sda));
#include "rnc/overlay/l09/gameplay/entities/airship_part_data/update_airship_part_2.h"
extern float airship_hull_breakup_wait_x_rotation_speed_degrees_gp __asm__("D_L09_001613BC") __attribute__((sda));
#include "rnc/overlay/l09/gameplay/entities/airship_part_data/update_airship_part.h"
extern f32 frame_time __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern int D_L09_001613C8 __attribute__((sda));
/* Update for the airship parts (classes 0x49E-0x4A5): attach to the hull, ride it, then break off, tumble away and fall with smoke and sparks. */
void update_airship_part(struct Moby *moby) __asm__("FUN_L09_002c26c8");
void update_airship_part(struct Moby *moby) {
    enum {
        AIRSHIP_PART_STATE_INITIALIZE = 0,
        AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_COMMAND = 1,
        AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_GATE = 2,
        AIRSHIP_PART_STATE_BEGIN_BREAKUP = 3,
        AIRSHIP_PART_STATE_FALLING_WITH_EFFECTS = 4,
        AIRSHIP_PART_STATE_FALLING_AFTER_DAMPING = 5
    };
    float point_offset_and_effect_scratch[4];
    Buf_2c26c8 transform_and_effect_scratch;
    float rotm[4][4];
    float nearest_parent_distance;
    int joint;
    Data_2c26c8 *part_vars = (*(Data_2c26c8 **)&moby->pvars);

    D_L09_001613C8++;
    switch (moby->state) {
    case AIRSHIP_PART_STATE_INITIALIZE: {
        float joint_offset_scale;

        FUN_L00_0024f7c8_2c26c8(moby, 0, point_offset_and_effect_scratch);
        if (part_vars->f68 != -1 && ((Flags_2c26c8 *)(D_0013D388_2c26c8 + part_vars->f68))->b39 != 0) {
            FUN_0020c828_2c26c8(moby);
            return;
        }
        moby->draw_dist = 0x3FF;
        if (moby->oclass == 0x4A0) {
            if (part_vars->h28 != 0) {
                int i;
                moby->draw_dist = 0xFF;
                part_vars->parent = 0;
                for (i = 0x49E; i < 0x4A6; i++) {
                    if (i != 0x4A0) {
                        spawn_airship_part((char *)moby, (char *)point_offset_and_effect_scratch, i);
                    }
                }
            }
        } else {
            struct Moby *p;
            nearest_parent_distance = 100000.0f;
            p = (struct Moby *)find_nearest_moby_by_class(((f32 *)&moby->pos), 0x4A0, &nearest_parent_distance);
            if (nearest_parent_distance < 20.0f) {
                part_vars->parent = p;
                (*(Data_2c26c8 **)&p->pvars)->parts[moby->oclass - 0x49E] = moby;
            } else {
                FUN_001e93b0_2c26c8(D_L09_002082A0, (*(short *)&moby->save_id), moby->oclass, FUN_00120478_2c26c8(nearest_parent_distance));
            }
        }
        joint = 0;
        joint_offset_scale = moby->scale * 0.0009765625f;
        get_moby_joint_matrices(moby, 1, &joint, transform_and_effect_scratch.m);
        FUN_001f9a68_2c26c8(part_vars->off, transform_and_effect_scratch.m[3], joint_offset_scale);
        moby->state = 1;
        break;
    }
    case AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_COMMAND:
    case AIRSHIP_PART_STATE_WAIT_FOR_BREAKUP_GATE: {
        if (moby->oclass == 0x4A0) {
            update_moby_vertical_oscillation(part_vars->f48, part_vars->f4C * 0.017453292f * frame_time, (char *)moby, &part_vars->f40, &part_vars->f44);
            update_moby_rotation_oscillation(moby, (char *)&part_vars->f50, (char *)&part_vars->f54, part_vars->f58 * 0.017453292f, part_vars->f5C * 0.017453292f * frame_time, part_vars->f60 * 0.017453292f * frame_time);
        }
        if (moby->oclass != 0x4A0 && part_vars->parent != 0 && part_vars->parent->state != 3) {
            struct Moby *p = part_vars->parent;
            qcopy(((f32 *)&moby->rot), ((f32 *)&p->rot));
            FUN_001fa050_2c26c8(rotm, ((f32 *)&p->rot));
            FUN_001f9d20_2c26c8(point_offset_and_effect_scratch, part_vars->off, rotm);
            FUN_L00_0024f7c8_2c26c8(part_vars->parent, 0, transform_and_effect_scratch.m[0]);
            FUN_001f9a28_2c26c8(((f32 *)&moby->pos), transform_and_effect_scratch.m[0], point_offset_and_effect_scratch);
        }
        if (part_vars->parent != 0) {
            moby->flags &= ~1;
            moby->flags |= part_vars->parent->flags & 1;
            moby->command = part_vars->parent->command;
        }
        if (moby->state == 2) {
            if (moby->oclass == 0x4A0) {
                moby->rot.x = FUN_001fa580_2c26c8(moby->rot.x, airship_hull_breakup_wait_x_rotation_speed_degrees_gp * 0.017453292f * frame_time);
            }
            if (FUN_001f9770_2c26c8(&part_vars->h2A) != 0) {
                moby->state = 3;
            }
        } else if (moby->command == 1) {
            int i;
            moby->update_dist = 0xFF;
            part_vars->h2A = FUN_L00_00257b90_2c26c8(FUN_001f96f8_2c26c8(airship_part_breakup_delay_range_start_gp), FUN_001f96f8_2c26c8(airship_part_breakup_delay_range_end_gp));
            for (i = 0; i < 9; i++) {
                if (part_vars->parts[i] != 0) {
                    part_vars->parts[i]->update_dist = 0xFF;
                }
            }
            moby->state = 2;
        }
        break;
    }
    case AIRSHIP_PART_STATE_BEGIN_BREAKUP: {
        FUN_001f9770_2c26c8(&part_vars->h2A);
        ((Flags_2c26c8 *)(D_0013D388_2c26c8))->b38 = 1;
        if (part_vars->f68 != -1) {
            ((Flags_2c26c8 *)(D_0013D388_2c26c8 + part_vars->f68))->b39 = 1;
        }
        FUN_L00_00257470_2c26c8(moby, 0x200, -1);
        FUN_L00_0025f090_2c26c8(moby, ((f32 *)&moby->pos), -1, 4.0f, 13.0f);
        if (moby->oclass == 0x4A0) {
            FUN_0022da68_2c26c8(2, 0, (int)moby);
        }
        FUN_L00_0024f7c8_2c26c8(moby, 0, point_offset_and_effect_scratch);
        FUN_001f9a28_2c26c8(part_vars->vel, ((f32 *)&moby->pos), point_offset_and_effect_scratch);
        FUN_001f9a68_2c26c8(part_vars->vel, part_vars->vel, airship_part_breakup_offset_velocity_scale_gp * frame_time / 20.0f);
        FUN_001f9a28_2c26c8(transform_and_effect_scratch.m[0], point_offset_and_effect_scratch, &hero.motion.pos);
        if (moby->oclass >= 0x4A1 && moby->oclass <= 0x4A2) {
            FUN_001f9bf8_2c26c8(transform_and_effect_scratch.m[0], transform_and_effect_scratch.m[0], airship_part_breakup_away_from_hero_speed_override_gp * frame_time);
        } else {
            FUN_001f9bf8_2c26c8(transform_and_effect_scratch.m[0], transform_and_effect_scratch.m[0], airship_part_breakup_away_from_hero_speed_gp * frame_time);
        }
        FUN_001f9a10_2c26c8(part_vars->vel, part_vars->vel, transform_and_effect_scratch.m[0]);
        part_vars->vel[2] = 0.0f;
        part_vars->f20 = FUN_002132a8_2c26c8(-(airship_part_initial_tumble_x_speed_limit_degrees_gp * 0.017453292f * frame_time), airship_part_initial_tumble_x_speed_limit_degrees_gp * 0.017453292f * frame_time);
        part_vars->f24 = FUN_002132a8_2c26c8(-(airship_part_initial_tumble_y_speed_limit_degrees_gp * 0.017453292f * frame_time), airship_part_initial_tumble_y_speed_limit_degrees_gp * 0.017453292f * frame_time);
        part_vars->h2A = FUN_002132a8_2c26c8(FUN_001f96f8_2c26c8(0x5A), FUN_001f96f8_2c26c8(0x96));
        part_vars->f64 = FUN_002132a8_2c26c8(airship_part_initial_downward_acceleration_range_start_gp * frame_time_sq_macro, airship_part_initial_downward_acceleration_range_end_gp * frame_time_sq_macro);
        if (moby->oclass >= 0x4A1 && moby->oclass <= 0x4A2) {
            part_vars->f20 = part_vars->f20 / part_vars->f20 * (1.5707964f / part_vars->h2A);
            part_vars->f64 = FUN_002132a8_2c26c8(airship_part_downward_acceleration_override_range_start_gp * frame_time_sq_macro, airship_part_downward_acceleration_override_range_end_gp * frame_time_sq_macro);
        }
        moby->collision = 0;
        moby->state = 4;
        break;
    }
    case AIRSHIP_PART_STATE_FALLING_WITH_EFFECTS:
        part_vars->vel[2] -= part_vars->f64;
        FUN_001f9a10_2c26c8(((f32 *)&moby->pos), ((f32 *)&moby->pos), part_vars->vel);
        moby->rot.x = FUN_001fa580_2c26c8(moby->rot.x, part_vars->f20);
        moby->rot.y = FUN_001fa580_2c26c8(moby->rot.y, part_vars->f24);
        if (moby->pos.x < 12.0f || moby->pos.x > 1011.0f || moby->pos.y < 12.0f || moby->pos.y > 1011.0f || moby->pos.z < 12.0f || moby->pos.z > 1011.0f) {
            FUN_0020c828_2c26c8(moby);
            return;
        }
        if (FUN_00213260_2c26c8(3) == 0) {
            FUN_001f99f8_2c26c8(point_offset_and_effect_scratch);
            transform_and_effect_scratch.pair = D_L09_001613D0;
            FUN_L00_0026a9f0_2c26c8(((f32 *)&moby->pos), point_offset_and_effect_scratch, 0x2F3F3F7F, transform_and_effect_scratch.pair.v[FUN_00213260_2c26c8(2)], FUN_L00_00257b90_2c26c8(FUN_001f96f8_2c26c8(0xF), FUN_001f96f8_2c26c8(0x14)), FUN_L00_00257b90_2c26c8(FUN_001f96f8_2c26c8(0x19), FUN_001f96f8_2c26c8(0x1E)), 0, 0, 400000.0f, FUN_002132a8_2c26c8(8.0f, 16.0f) * frame_time);
        }
        if (moby->pos.z < 25.0f) {
            int i;
            for (i = 0; i < airship_part_low_altitude_effect_count_gp; i++) {
                float effect_direction_angle = FUN_00213308_2c26c8();
                qcopy(transform_and_effect_scratch.m[1], ((f32 *)&moby->pos));
                transform_and_effect_scratch.m[1][0] += FUN_002132a8_2c26c8(-3.0f, 3.0f);
                transform_and_effect_scratch.m[1][1] += FUN_002132a8_2c26c8(-3.0f, 3.0f);
                point_offset_and_effect_scratch[2] = D_L09_00161374 * frame_time;
                point_offset_and_effect_scratch[0] += FUN_001f9dc8_2c26c8(effect_direction_angle) * D_L09_0016137C;
                point_offset_and_effect_scratch[1] += FUN_001f9de0_2c26c8(effect_direction_angle) * D_L09_0016137C;
                transform_and_effect_scratch.m[0][2] = D_L09_00161378 * frame_time;
                transform_and_effect_scratch.m[0][0] += FUN_001f9dc8_2c26c8(effect_direction_angle) * D_L09_00161380;
                transform_and_effect_scratch.m[0][1] += FUN_001f9de0_2c26c8(effect_direction_angle) * D_L09_00161380;
                point_offset_and_effect_scratch[3] = D_L09_00161390;
                transform_and_effect_scratch.m[0][3] = D_L09_00161394;
                FUN_00218888_2c26c8(transform_and_effect_scratch.m[1], point_offset_and_effect_scratch, transform_and_effect_scratch.m[0], D_L09_0016136C, D_L09_00161370,
                    truncate_float_to_s32_r(FUN_001f96b0_2c26c8(FUN_002132a8_2c26c8(D_L09_00161384, D_L09_00161384 * 2))),
                    truncate_float_to_s32_r(FUN_001f96b0_2c26c8(FUN_002132a8_2c26c8(D_L09_00161388, D_L09_00161388 * 2))),
                    truncate_float_to_s32_r(FUN_001f96b0_2c26c8(FUN_002132a8_2c26c8(D_L09_0016138C, D_L09_0016138C * 2))),
                    -1);
            }
            FUN_001f9a68_2c26c8(part_vars->vel, part_vars->vel, 0.45f);
            part_vars->f20 *= 0.9f;
            part_vars->f24 *= 0.9f;
            FUN_0022da68_2c26c8(0, 0, (int)moby);
            moby->state = 5;
        }
        break;
    case AIRSHIP_PART_STATE_FALLING_AFTER_DAMPING:
        part_vars->vel[2] -= frame_time_sq_macro * 10.8f / 3.0f;
        FUN_001f9a10_2c26c8(((f32 *)&moby->pos), ((f32 *)&moby->pos), part_vars->vel);
        moby->rot.x = FUN_001fa580_2c26c8(moby->rot.x, part_vars->f20);
        moby->rot.y = FUN_001fa580_2c26c8(moby->rot.y, part_vars->f24);
        if (moby->pos.x < 12.0f || moby->pos.x > 1011.0f || moby->pos.y < 12.0f || moby->pos.y > 1011.0f || moby->pos.z < 12.0f || moby->pos.z > 1011.0f) {
            FUN_0020c828_2c26c8(moby);
        }
        break;
    }
}
