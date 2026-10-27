/* Level 13 moby classes: elevator, blarg_turret, blarg_gunner, robosquawker, fighter_jet. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern int stub_printf() __asm__("FUN_001e93b0");
extern void delete_moby(void *) __asm__("FUN_0020c828");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern char *D_L13_00160058_m_b0e80 __asm__("D_L13_0015FFD8");
extern char D_L13_001F41F8[];
void update_moby_volume_trigger(char *moby) __asm__("FUN_L13_002b0e80");

void update_moby_volume_trigger(char *moby) {
    char *data = *(char **)(moby + 0x78);
    if (*(int *)data == -1) {
        stub_printf(D_L13_001F41F8, *(short *)(moby + 0xB2));
        delete_moby(moby);
        return;
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)data) != 0) {
        if (data[0x38] == 0) {
            {
                int uid = *(int *)(data + 0x4);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x24] != -1)
                                    m[0x20] = data[0x24];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x14) != -1 && data[0x2C] != -1)
                set_moby_list_state(*(int *)(data + 0x14), data[0x2C]);
            {
                int uid = *(int *)(data + 0x8);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x25] != -1)
                                    m[0x20] = data[0x25];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x18) != -1 && data[0x2D] != -1)
                set_moby_list_state(*(int *)(data + 0x18), data[0x2D]);
            {
                int uid = *(int *)(data + 0xC);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x26] != -1)
                                    m[0x20] = data[0x26];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x1C) != -1 && data[0x2E] != -1)
                set_moby_list_state(*(int *)(data + 0x1C), data[0x2E]);
            {
                int uid = *(int *)(data + 0x10);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x27] != -1)
                                    m[0x20] = data[0x27];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x20) != -1 && data[0x2F] != -1)
                set_moby_list_state(*(int *)(data + 0x20), data[0x2F]);
            if (data[0x34] != -1)
                moby[0x20] = data[0x34];
            if (data[0x36] != -1)
                moby[0xBC] = data[0x36];
            data[0x38] = 1;
        }
    } else {
        if (data[0x38] == 1) {
            {
                int uid = *(int *)(data + 0x4);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x28] != -1)
                                    m[0x20] = data[0x28];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x14) != -1 && data[0x30] != -1)
                set_moby_list_state(*(int *)(data + 0x14), data[0x30]);
            {
                int uid = *(int *)(data + 0x8);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x29] != -1)
                                    m[0x20] = data[0x29];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x18) != -1 && data[0x31] != -1)
                set_moby_list_state(*(int *)(data + 0x18), data[0x31]);
            {
                int uid = *(int *)(data + 0xC);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x2A] != -1)
                                    m[0x20] = data[0x2A];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x1C) != -1 && data[0x32] != -1)
                set_moby_list_state(*(int *)(data + 0x1C), data[0x32]);
            {
                int uid = *(int *)(data + 0x10);
                if (uid != -1) {
                    char *m = D_L13_00160058_m_b0e80 + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x2B] != -1)
                                    m[0x20] = data[0x2B];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x20) != -1 && data[0x33] != -1)
                set_moby_list_state(*(int *)(data + 0x20), data[0x33]);
            if (data[0x35] != -1)
                moby[0x20] = data[0x35];
            if (data[0x37] != -1)
                moby[0xBC] = data[0x37];
            data[0x38] = 0;
        }
    }
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

/* Variables of the class-1380 lift (moby->pvars). */
typedef struct {
    u8 pad0[0x20];
    u8 path[0x40];       /* 0x20: passed to update_entity_matrix with the moved delta */
    f32 top;             /* 0x60 */
    f32 bottom;          /* 0x64 */
    f32 speed;           /* 0x68 */
    s32 voice;           /* 0x6C */
    f32 blend;           /* 0x70: camera blend, 0 to 1 */
    f32 blend_speed;     /* 0x74 */
    f32 pulse;           /* 0x78: colour pulse angle */
    u8 armed;            /* 0x7C: the hero stepped off since the lift stopped */
} LiftVars;

extern char D_L13_001670C0_2b39e0[] __asm__("D_L13_001670C0");
extern float D_0015ED70_2b39e0 __asm__("D_0015ED70");
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern int float_to_int(float) __asm__("FUN_001fa6d0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern s32 allocate_voice_2b39e0(s32, s32, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
void compute_elevator_camera_pose(struct Moby *moby, float *camera_position, float *camera_rotation) __asm__("FUN_L13_002b4060");

extern void compute_elevator_camera_pose(struct Moby *, float *, float *) __asm__("FUN_L13_002b4060");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

/* Updates the lift: rides between its two heights, blends the camera while the hero rides it and pulses its colour while it waits. */
void elevator_update(struct Moby *moby) __asm__("FUN_L13_002b39e0");
void elevator_update(struct Moby *moby) {
    enum {
        ELEVATOR_STATE_INITIALIZE = 0,
        ELEVATOR_STATE_BLEND_CAMERA = 1,
        ELEVATOR_STATE_MOVE_TO_TOP = 2,
        ELEVATOR_STATE_MOVE_TO_BOTTOM = 3,
        ELEVATOR_STATE_WAIT_AT_TOP = 4,
        ELEVATOR_STATE_WAIT_AT_BOTTOM = 5,
        ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_TOP = 6,
        ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_BOTTOM = 7
    };
    LiftVars *vars = (LiftVars *)moby->pvars;
    float delta[4];
    float rot[4];
    float camera_position_scratch[4];
    float camera_pose_scratch[4];
    float blended_camera_rotation[4];

    scale_vector_xyz(delta, &moby->pos, -1.0f);
    qcopy(rot, &moby->rot);
    switch (moby->state) {
    case ELEVATOR_STATE_INITIALIZE:
        vars->pulse = random_angle_radians();
        vars->voice = -1;
        moby->state = 3;
        break;
    case ELEVATOR_STATE_BLEND_CAMERA: {
        float old = vars->blend;
        advance_accelerated_scalar(&vars->blend, &vars->blend_speed, 1.0f, D_0015ED70_2b39e0 * 4.0f,
                                   D_0015ED70_2b39e0 * 4.0f, frame_time * 4.0f);
        if (vars->blend < 1.0f) {
            float t = (1.0f - vars->blend) / (1.0f - old);
            char *base;
            compute_elevator_camera_pose(moby, camera_pose_scratch, blended_camera_rotation);
            subtract_vector_xyz(camera_position_scratch, D_L13_001670C0_2b39e0, camera_pose_scratch);
            base = D_L13_001670C0_2b39e0 - 0x140;
            scale_vector_xyz(camera_position_scratch, camera_position_scratch, t);
            add_vector_xyz(camera_pose_scratch, camera_pose_scratch, camera_position_scratch);
            blended_camera_rotation[1] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x154), blended_camera_rotation[1]) * t, blended_camera_rotation[1]);
            blended_camera_rotation[2] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x158), blended_camera_rotation[2]) * t, blended_camera_rotation[2]);
            FUN_L00_002ea9d8(camera_pose_scratch);
            FUN_L00_002eaa30(blended_camera_rotation);
        } else {
            u8 s = moby->command;
            moby->command = 1;
            moby->state = s;
        }
        break;
    }
    case ELEVATOR_STATE_MOVE_TO_TOP:
    case ELEVATOR_STATE_MOVE_TO_BOTTOM: {
        float target;
        if (!voice_is_playing(moby, vars->voice)) {
            vars->voice = allocate_voice_2b39e0(0, 4, moby);
        }
        if (moby->command != 0) {
            compute_elevator_camera_pose(moby, camera_position_scratch, camera_pose_scratch);
            FUN_L00_002ea9d8(camera_position_scratch);
            FUN_L00_002eaa30(camera_pose_scratch);
        }
        if (moby->state == 2) {
            target = vars->top;
        } else {
            target = vars->bottom;
        }
        advance_accelerated_scalar(&moby->pos.z, &vars->speed, target, D_0015ED70_2b39e0 * 8.0f,
                                   D_0015ED70_2b39e0 * 8.0f, frame_time * 40.0f);
        if (absolute_float(moby->pos.z - target) == 0.0f) {
            if (moby->state == 2) {
                moby->state = 6;
            } else {
                moby->state = 7;
            }
            if (moby->command != 0) {
                hero_set_state(HERO_STATE_IDLE, 0);
                FUN_L00_002eac18(1);
            }
        }
        break;
    }
    case ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_TOP:
    case ELEVATOR_STATE_WAIT_FOR_HERO_EXIT_AT_BOTTOM:
        if (voice_is_playing(moby, vars->voice)) {
            release_voice_slot(vars->voice);
            vars->voice = -1;
        }
        if (hero.ground_moby != moby || hero.air_frames.s != 0 ||
            vector_distance(&hero.motion.pos, &moby->pos) > 0.5f) {
            u8 s = moby->state;
            vars->armed = 0;
            if (s == 7) {
                moby->state = 5;
            } else {
                moby->state = 4;
            }
        }
        break;
    case ELEVATOR_STATE_WAIT_AT_TOP:
    case ELEVATOR_STATE_WAIT_AT_BOTTOM: {
        int i;
        vars->pulse = fast_add_rotations(vars->pulse, frame_time * 6.2831855f);
        i = float_to_int((fast_sin(vars->pulse) * 4.0f - 3.0f) * 128.0f);
        if (i > 0x80) {
            i = 0x80;
        } else if (i < 0x20) {
            i = 0x20;
        }
        {
            int t = (i << 8) | 0x80000000;
            moby->glow_color = (i << 16) | t | i;
        }
        if (hero.ground_moby != moby || hero.air_frames.s != 0 ||
            vector_distance(&hero.motion.pos, &moby->pos) > 1.1f) {
            vars->armed = 1;
        }
        if (vars->armed != 0 && hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance(&hero.motion.pos, &moby->pos) < 0.5f) {
            moby->glow_color = 0x80208020;
            if (moby->state == 5) {
                moby->command = 2;
            } else {
                moby->command = 3;
            }
            moby->state = 1;
            hero_set_state(0x72, 1);
            jet_mount(D_L13_001670C0_2b39e0, D_L13_001670C0_2b39e0 + 0x10, 1, 0, 0);
            vars->blend = 0.0f;
        } else if (moby->state == 4) {
            char *p = (char *)&hero.motion.pos;
            float d = vector_distance(&moby->pos, p);
            if (absolute_float(*(float *)(p + 0x258) - vars->bottom) < 2.0f && d < 32.0f && d > 5.0f) {
                moby->command = 0;
                moby->state = 3;
                vars->speed = -(frame_time * 5.0f);
            }
        } else if (moby->state == 5) {
            char *p = (char *)&hero.motion.pos;
            float d = vector_distance(&moby->pos, p);
            if (absolute_float(*(float *)(p + 0x258) - vars->top) < 2.0f && d < 32.0f && d > 5.0f) {
                moby->command = 0;
                moby->state = 2;
                vars->speed = frame_time * 5.0f;
            }
        }
        break;
    }
    }
    add_vector_xyz(delta, delta, &moby->pos);
    update_entity_matrix(vars->path, delta, rot, &moby->rot);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5210), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern unsigned char D_0013E533[];
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");

void compute_elevator_camera_pose(struct Moby *moby, float *v, float *w) __asm__("FUN_L13_002b4060");
void compute_elevator_camera_pose(struct Moby *moby, float *camera_position, float *camera_rotation) {
    float camera_offset_azimuth = moby->rot.z;
    float horizontal_offset_radius = 2.5f;
    float z = 1.5f;
    unsigned char *x;
    if ((moby->command >= 2 && (moby->command & 1)) || (moby->state >= 2 && (moby->state & 1))) {
        camera_offset_azimuth = fast_subtract_rotations(camera_offset_azimuth, 1.5707964f);
    } else {
        camera_offset_azimuth = fast_add_rotations(camera_offset_azimuth, 1.5707964f);
    }
    if (moby->oclass == 0x15) {
        horizontal_offset_radius = 2.0f;
        z = 1.2f;
    }
    camera_position[0] = fast_cos(camera_offset_azimuth) * horizontal_offset_radius;
    camera_position[1] = fast_sin(camera_offset_azimuth) * horizontal_offset_radius;
    camera_position[2] = z;
    fast_vec_add(camera_position, camera_position, &moby->pos);
    clear_u64_value(camera_rotation);
    x = D_0013E533 + 0xE1D;
    camera_rotation[2] = fast_arc_tan(*(float *)(x + 0xD0) - camera_position[0], *(float *)(x + 0xD4) - camera_position[1]);
    *(int *)(camera_rotation + 1) = 0;
}
/* Ported from rac1-decomp src/overlays/l13_gemlik/vendor_002B2020.c (func_L13_002B5368) */

/* Update for moby class 29 (blarg_turret) on level 13: turns toward its target and fires its gun moby. */

typedef struct Moby Moby;

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V4_2B5368;

typedef struct Cls_2B5368 { char pad0[0x24]; float scale; } Cls_2B5368;

typedef struct Moby_2B5368 {
    char pad0[0x10];
    V4_2B5368 pos;
    signed char state;
    char pad21[3];
    Cls_2B5368 *cls;
    char pad28[4];
    float scale;
    char pad30[0x10];
    V4_2B5368 rot;
    char pad50[3];
    unsigned char b53;
    char pad54[0x24];
    void *data;
    char pad7C[0x2A];
    short cls_id;
    char padA8[0x18];
    V4_2B5368 mat[3];
} Moby_2B5368;

typedef struct Data_2B5368 {
    char pad0[0x40];
    Moby_2B5368 *target;
    char pad44[0xC];
    int volume;
    float f54;
    char pad58[4];
    Moby_2B5368 *gun;
    float f60;
    float f64;
    float f68;
    float f6C;
    char fx[0x10];
} Data_2B5368;

extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float D_0015ED70_c __asm__("D_0015ED70");
extern int FUN_L00_00257b90(int, int);
extern unsigned char D_0013F420[];
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
float FUN_L00_0025be00(float *cur, float *vel, float target, float lim, float accel, float maxv);
struct Moby *FUN_L13_002b4958_c(struct Moby *parent) __asm__("FUN_L13_002b4958");
void FUN_L00_00257d78(float *out, float lo, float hi);
void FUN_L13_002b47a0_c(struct Moby *moby) __asm__("FUN_L13_002b47a0");
void FUN_L13_002b48d8_c(struct Moby *moby) __asm__("FUN_L13_002b48d8");
extern void FUN_L13_002b48d8_2b0e80(void *) __asm__("FUN_L13_002b48d8");
extern void *FUN_L13_002b4958_2b0e80(void *) __asm__("FUN_L13_002b4958");
extern void FUN_L00_00263ac8_2b0e80(float, void *, int, void *) __asm__("FUN_L00_00263ac8");
extern void FUN_00212f90_2b0e80(void *, int, int, int) __asm__("FUN_00212f90");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float approach_angle(void *, float, void *, float, float, float) __asm__("FUN_L00_0025be00");
extern float FUN_001fa5c8_2b0e80(float, float) __asm__("FUN_001fa5c8");
extern float FUN_001fa580_2b0e80(float, float) __asm__("FUN_001fa580");
extern void FUN_L13_002b47a0_2b0e80(void *) __asm__("FUN_L13_002b47a0");
extern void mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025e450_2b0e80(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern unsigned char D_0013F420_2b0e80[] __asm__("D_0013F420");

void blarg_turret_update(Moby_2B5368 *m) __asm__("FUN_L13_002b41b8");
void blarg_turret_update(Moby_2B5368 *turret_moby) {
    enum {
        BLARG_TURRET_STATE_INITIALIZE = 0,
        BLARG_TURRET_STATE_WAIT_FOR_TARGET = 1,
        BLARG_TURRET_STATE_TRACK_TARGET = 3,
        BLARG_TURRET_STATE_DESTROY = 4,
        BLARG_TURRET_STATE_DESTRUCTION_DELAY = 5
    };
    Data_2B5368 *data = turret_moby->data;
    Moby_2B5368 *target_moby;
    Moby_2B5368 *gun;
    float target_yaw_scratch;

    FUN_L13_002b48d8_2b0e80(turret_moby);
    target_moby = data->target;
    if (data->gun != 0 && data->gun->cls_id == 0x24 && data->gun->state >= 0) {
        FUN_L00_00263ac8_2b0e80(2.7f, data->gun, 1, data->fx);
    }
    switch ((unsigned char)turret_moby->state) {
    case BLARG_TURRET_STATE_INITIALIZE:
        turret_moby->scale = turret_moby->cls->scale * 1.5f;
        data->gun = FUN_L13_002b4958_2b0e80(turret_moby);
        data->f54 = turret_moby->rot.z;
        if (turret_moby->b53 != 0) {
            FUN_00212f90_2b0e80(turret_moby, 0, 0, 10);
        }
        turret_moby->state = 1;
        break;
    case BLARG_TURRET_STATE_WAIT_FOR_TARGET:
        target_yaw_scratch = fast_difference_between_rotations(angle_atan2(target_moby->pos.x - turret_moby->pos.x, target_moby->pos.y - turret_moby->pos.y), data->f54);
        if (target_yaw_scratch < 1.5707964f && vector_distance(&turret_moby->pos, &target_moby->pos) < 20.0f
            && (data->volume == -1 || is_point_inside_clip_volume(D_0013F420_2b0e80, data->volume) != 0)) {
            data->f68 = 8.0f;
            if (turret_moby->b53 != 1) {
                int animation_start_frame = random_int_between(0, 3);
                FUN_00212f90_2b0e80(turret_moby, 1, animation_start_frame, random_int_between(7, 13));
            }
            turret_moby->state = 3;
        }
        approach_angle(&turret_moby->rot.z, data->f54, &data->f60, D_0015ED70_c * 12.566371f, D_0015ED70_c * 12.566371f, frame_time * 12.566371f);
        if (data->gun == 0 || data->gun->cls_id != 0x24 || (unsigned char)data->gun->state == 0xFE
            || (unsigned char)data->gun->state == 0xFD || (unsigned char)data->gun->state == 2) {
            if (turret_moby->b53 != 0) {
                FUN_00212f90_2b0e80(turret_moby, 0, 0, random_int_between(7, 13));
            }
            data->f64 = 1.0f;
            turret_moby->state = 5;
        }
        break;
    case BLARG_TURRET_STATE_TRACK_TARGET:
        target_yaw_scratch = fast_difference_between_rotations(angle_atan2(target_moby->pos.x - turret_moby->pos.x, target_moby->pos.y - turret_moby->pos.y), data->f54);
        if (target_yaw_scratch >= 1.5707964f || vector_distance(&turret_moby->pos, &target_moby->pos) > 22.0f
            || (data->volume != -1 && is_point_inside_clip_volume(D_0013F420_2b0e80, data->volume) == 0)) {
            if (turret_moby->b53 != 0) {
                FUN_00212f90_2b0e80(turret_moby, 0, 0, random_int_between(7, 13));
            }
            turret_moby->state = 1;
        } else {
            float yaw_offset_limit_radians;
            approach_value(&data->f68, 20.0f, frame_time * 6.0f);
            target_yaw_scratch = FUN_001fa5c8_2b0e80(angle_atan2(target_moby->pos.x - turret_moby->pos.x, target_moby->pos.y - turret_moby->pos.y), data->f54);
            yaw_offset_limit_radians = data->f6C * 0.017453292f;
            if (yaw_offset_limit_radians < target_yaw_scratch) {
                target_yaw_scratch = yaw_offset_limit_radians;
            } else if (target_yaw_scratch < -yaw_offset_limit_radians) {
                target_yaw_scratch = -yaw_offset_limit_radians;
            }
            target_yaw_scratch = FUN_001fa580_2b0e80(target_yaw_scratch, data->f54);
            approach_angle(&turret_moby->rot.z, target_yaw_scratch, &data->f60, D_0015ED70_c * 12.566371f, D_0015ED70_c * 12.566371f, frame_time * 12.566371f);
            FUN_L13_002b47a0_2b0e80(turret_moby);
            if (data->gun == 0 || data->gun->cls_id != 0x24 || (unsigned char)data->gun->state == 0xFE
                || (unsigned char)data->gun->state == 0xFD || (unsigned char)data->gun->state == 2) {
                if (turret_moby->b53 != 0) {
                    FUN_00212f90_2b0e80(turret_moby, 0, 0, random_int_between(7, 13));
                }
                data->f64 = 1.0f;
                turret_moby->state = 5;
            }
        }
        break;
    case BLARG_TURRET_STATE_DESTROY: {
        V4_2B5368 destruction_effect_vector;
        random_vector(&destruction_effect_vector.x, 0.5f, 1.0f);
        mark_moby_for_removal_cf(turret_moby, 0, -1);
        FUN_L00_0025e450_2b0e80(turret_moby, &destruction_effect_vector, &turret_moby->pos, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
        delete_moby(turret_moby);
        return;
    }
    case BLARG_TURRET_STATE_DESTRUCTION_DELAY:
        approach_angle(&turret_moby->rot.z, data->f54, &data->f60, D_0015ED70_c * 12.566371f, D_0015ED70_c * 12.566371f, frame_time * 25.132742f);
        data->f64 -= 1.0f;
        if (data->f64 <= 0.0f) {
            turret_moby->state = 4;
        }
        break;
    }
    gun = data->gun;
    if (gun != 0 && gun->cls_id == 0x24 && (unsigned char)gun->state != 0xFE && (unsigned char)gun->state != 0xFD) {
        qcopy(&gun->rot, &turret_moby->rot);
        *(u128 *)&gun->mat[0] = *(u128 *)&turret_moby->mat[0];
        *(u128 *)&gun->mat[1] = *(u128 *)&turret_moby->mat[1];
        *(u128 *)&gun->mat[2] = *(u128 *)&turret_moby->mat[2];
    } else {
        data->gun = 0;
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5950), where it is exact; names translated to the US level program. */

extern char *spawn_gemlik_projectile(float, float *, float *, void *) __asm__("FUN_L13_003061c8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int frame_count __asm__("D_L13_0015F5CC");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

#include "rnc/overlay/l13/gameplay/entities/elevator_globals.h"
extern float blarg_turret_launch_displacement_scale_gp __asm__("D_L13_00161388") __attribute__((sda));
int D_L13_0016138C = 2;
void fire_blarg_turret_projectile(struct Moby *moby) __asm__("FUN_L13_002b47a0");

void fire_blarg_turret_projectile(struct Moby *moby) {
    float projectile_spawn_position[4];
    float horizontal_offset[4];
    char *data;
    float target_distance, x, projectile_yaw;
    data = (char *)moby->pvars;
    if (frame_count % D_L13_0016138C == 0) {
        moby_point_position(moby, 0, projectile_spawn_position);
        target_distance = vector_distance(&moby->pos, *(char **)(data + 0x40) + 0x10);
        x = (20.0f - target_distance) / 20.0f * 5.0f + 2.5f;
        projectile_yaw = fast_add_rotations(random_float_between(-x, x) * DEG_TO_RAD,
                                 moby->rot.z);
        horizontal_offset[0] = fast_cos(projectile_yaw) * (blarg_turret_launch_displacement_scale_gp * frame_time);
        horizontal_offset[1] = fast_sin(projectile_yaw) * (blarg_turret_launch_displacement_scale_gp * frame_time);
        horizontal_offset[2] = 0;
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, horizontal_offset);
        spawn_gemlik_projectile(*(float *)(data + 0x68), projectile_spawn_position, horizontal_offset, moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5A88), where it is exact; names translated to the US level program. */

extern int select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");

void update_blarg_turret_target_and_state(struct Moby *moby) __asm__("FUN_L13_002b48d8");

void update_blarg_turret_target_and_state(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float distance;
    if (moby->state == 3)
        distance = 22.0f;
    else
        distance = 20.0f;
    select_moby_combat_target(moby, data, distance);
    if (*(int *)(data + 0x40) == 0) {
        *(int *)(data + 0x40) = ((int)hero.moby);
    }
    if (moby->command >= 4)
        moby->state = moby->command;
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"


/* pvars of the child spawned by FUN_L13_002b4958 */
typedef struct {
    void *unk0;
    u8 pad4[0x8];
    void *unkC;
    u8 pad10[0x10];
    f32 unk20;
    s16 unk24;
    u8 pad26[2];
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 pad2B[5];
    f32 unk30;
    u8 pad34[0x2C];
    u8 unk60[0x10];
    struct Moby *parent;
} Vars;

extern struct Moby *create_moby(s32 oclass) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8(struct Moby *moby);
extern void copy_hero_moby_field_38(struct Moby *moby) __asm__("FUN_L00_0025d1b8");

/* spawns a class 0x24 child that copies its parent's position and scale */
struct Moby *spawn_blarg_turret_child(struct Moby *parent) __asm__("FUN_L13_002b4958");

struct Moby *spawn_blarg_turret_child(struct Moby *parent) {
    struct Moby *child_moby;
    Vars *child_vars;

    child_moby = create_moby(0x24);
    if (child_moby != 0) {
        child_vars = (Vars *)child_moby->pvars;
        child_vars->parent = parent;
        child_vars->unk0 = &child_vars->unk20;
        child_vars->unkC = child_vars->unk60;
        child_moby->update_dist = parent->update_dist;
        child_moby->draw_dist = child_vars->parent->draw_dist;
        child_moby->visible = 1;
        child_moby->flags |= 0x20;
        child_moby->state = 0;
        child_moby->command = 0;
        qcopy(&child_moby->pos, &child_vars->parent->pos);
        child_moby->scale = child_moby->pclass->scale * parent->scale / parent->pclass->scale;
        child_vars->unk20 = 1.0f;
        child_vars->unk24 = 1;
        child_vars->unk29 = 1;
        child_vars->unk2A = 3;
        child_vars->unk28 = 0;
        child_vars->unk30 = 0.9f;
        FUN_L00_00250df8(child_moby);
        copy_hero_moby_field_38(child_moby);
    }
    return child_moby;
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5C08), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void process_blarg_gunner_hits(struct Moby *moby, char *moby_vars) __asm__("FUN_L13_002b4a58");

void process_blarg_gunner_hits(struct Moby *moby, char *moby_vars) {
    enum {
        HIT_REACTION_NONE = 11
    };
    float *tm = (float *)(moby_vars + 0x20);
    char *hit_record;
    int k;
    int hit_reaction_result;
    if (moby->state != 2) {
        hit_record = find_moby_hit(moby, 0x330000, 0);
        hit_reaction_result = hit_reaction(moby, hit_record, tm, 0, &k, 0, 0, 4);
        if (hit_record != 0) {
            if (*(int *)(hit_record + 0x24) & 0x800000) {
                char *hit_source_moby = *(char **)(hit_record + 0x20);
                if (hit_source_moby == 0) {
                    *(int *)(hit_record + 0x2C) = 0;
                    k = 1;
                } else {
                    float f = fast_difference_between_rotations(
                        fast_arc_tan(*(float *)(hit_source_moby + 0x10) - moby->pos.x,
                                     *(float *)(hit_source_moby + 0x14) - moby->pos.y),
                        *(float *)(*(char **)(*(char **)(moby_vars + 0x70) + 0x78) + 0x54));
                    if (f < 1.65806282f) {
                        *(int *)(hit_record + 0x2C) = 0;
                        k = 1;
                    }
                }
            }
        }
        switch (hit_reaction_result) {
        case 1:
        case 2:
            *(int *)tm = 0;
            break;
        case 0:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case HIT_REACTION_NONE:
            break;
        }
        if (k >= 2) {
            float hit_damage = *(float *)(hit_record + 0x2C);
            float current_health = *tm;
            if (current_health <= hit_damage) {
                char *x;
                *(int *)tm = 0;
                moby->flags &= 0xEFFF;
                x = *(char **)(moby_vars + 0x70);
                if (x != 0) {
                    if (*(short *)(x + 0xA6) == 0x1D) {
                        unsigned char parent_turret_state = x[0x20];
                        if (parent_turret_state != 0xFE) {
                            if (parent_turret_state != 0xFD)
                                x[0x20] = 4;
                        }
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                moby_vars[0x67] = 0x78;
                trooper_anim_update(moby, (short *)(moby_vars + 0x60));
                if (moby->prev_seq != 3)
                    blend_moby_animation(moby, 3, 1, 0);
                moby->state = 2;
            } else {
                *tm = current_health - hit_damage;
                *(unsigned char *)(moby_vars + 0x67) = 0xFA;
                *(short *)(moby_vars + 0x26) = scale_game_frames(0x3C);
                trooper_anim_update(moby, (short *)(moby_vars + 0x60));
            }
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_vars + 0x60);
}
/* Updates the gem-lock turret: idle animations, then a burst of particles when destroyed. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5E30), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    char pos[0x10];
    unsigned char b20;
    char pad21[0xB];
    float f2C;
    unsigned char b30;
    unsigned char b31;
    short s32;
    unsigned short h34;
    char pad36[0x12];
    float f48;
    char pad4C[0x2C];
    Child *child;
    char pad7C[0x14];
    int f90;
} Obj;

extern float D_L13_001670C0[];
extern int FUN_L00_00257b90(int, int);
extern int FUN_L13_002b4a58_c() __asm__("FUN_L13_002b4a58");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void blarg_gunner_update(struct Moby *moby) __asm__("FUN_L13_002b4c80");
void blarg_gunner_update(struct Moby *moby) {
    enum {
        BLARG_GUNNER_STATE_INITIALIZE = 0,
        BLARG_GUNNER_STATE_ACTIVE = 1,
        BLARG_GUNNER_STATE_DESTROY = 2
    };
    unsigned char *moby_vars = moby->pvars;
    float explosion_offset[4];
    float explosion_spawn_position[4];
    switch (moby->state) {
    case BLARG_GUNNER_STATE_INITIALIZE:
        moby_vars[0x29] = 1;
        moby->state = 1;
        moby_vars[0x5A] = 6;
        moby_vars[0x58] = 8;
        break;
    case BLARG_GUNNER_STATE_ACTIVE:
        if ((*(unsigned char **)(moby_vars + 0x70))[0x20] == 3) {
            if (moby->prev_seq != 2) {
                int x = FUN_L00_00257b90(0, 3);
                int y = FUN_L00_00257b90(7, 0xC);
                blend_moby_animation(moby, 2, x, y);
            }
        } else {
            if (moby->prev_seq >= 2 || (moby->anim_flags & 2)) {
                if (!random_integer_below(7)) {
                    if (moby->prev_seq != 1) {
                        blend_moby_animation(moby, 1, 0, FUN_L00_00257b90(7, 0xD));
                    }
                } else {
                    if (moby->prev_seq != 0) {
                        blend_moby_animation(moby, 0, 0, FUN_L00_00257b90(7, 0xD));
                    }
                }
                moby->anim_speed = random_float_between(0.8f, 1.2f);
            }
        }
        FUN_L13_002b4a58_c(moby, moby_vars);
        break;
    case BLARG_GUNNER_STATE_DESTROY:
        if (moby->anim_flags & 2) {
            subtract_vector_xyz(explosion_offset, D_L13_001670C0, &moby->pos);
            moby_point_position(moby, 0, explosion_spawn_position);
            spawn_explosion(moby, explosion_offset, explosion_spawn_position, 0.0f, 0.0f, 0x14, 9, 0x20, 10.0f, 7.0f, 20.0f, -1, 2.0f,
                             40.0f, 0, 1, -1, 0);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B60D8), where it is exact; names translated to the US level program. */

extern void * create_moby_joint_particle(void *, int, int, float, float, int, float, int) __asm__("FUN_L13_00280400");
extern void kill_part(void *) __asm__("FUN_L00_00267a08");

/* Per-frame glow effect: spawns it while the moby is active (byte 0x31 set), frees it otherwise. */
void update_robosquawker_particle(struct Moby *moby, char *data) __asm__("FUN_L13_002b4f28");

void update_robosquawker_particle(struct Moby *moby, char *data) {
    float opacity = 0.2f;
    void **handle = (void **)(data + 0x228);
    if (moby->visible) {
        if (!*handle) {
            *handle = create_moby_joint_particle(moby, 0, 0x80808080, opacity, opacity, 0x10808080, 0.8f, 0x19);
        }
    } else if (*handle) {
        kill_part(*handle);
        *handle = 0;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Release the vendor moby's attached object and clear its reference. */
/* Ported from rac1-decomp (src/overlays/l13/vendor_002B2020.c: func_L13_002B6168), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x228];
    void *value;
} Level13VendorMoby;

extern void kill_part(void *) __asm__("FUN_L00_00267a08");

/* Passes the stored value to free_effect_particle once, then clears it. */
void release_robosquawker_particle(Level13VendorMoby *moby) __asm__("FUN_L13_002b4fb8");

void release_robosquawker_particle(Level13VendorMoby *moby) {
    if (moby->value != 0) {
        kill_part(moby->value);
        moby->value = 0;
    }
}
#include "sda.h"

#include "qzero.h"

/* adjusts moby height toward the sampled ground height */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B61A0), where it is exact; names translated to the US level program. */

extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern f32 D_0015ED70;

void update_robosquawker_ground_height(struct Moby *moby, char *moby_vars) __asm__("FUN_L13_002b4ff0");

void update_robosquawker_ground_height(struct Moby *moby, char *moby_vars) {
    float ground = probe_ground_height(&moby->pos, 0, 0.5f);
    float z = moby->pos.z;
    if (ground < z + 0.3f) {
        float vertical_step = *(float *)(moby_vars + 0x128) - *(float *)&D_0015ED70 * 9.8f;
        *(float *)(moby_vars + 0x128) = vertical_step;
        moby->pos.z = moby->pos.z + vertical_step;
        if (moby->pos.z < ground)
            moby->pos.z = ground;
    } else if (z < ground) {
        moby->pos.z = z + *(float *)&D_0015ED70 * 9.8f;
        qzero(moby_vars + 0x120);
    } else {
        qzero(moby_vars + 0x120);
    }
}

/* Robosquawker update: tracks a target, walks its path, flies to a random point, hops and pecks after the hero, and dies with a burst of parts. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B6288), where it is exact; names translated to the US level program. */

typedef struct {
    float f[4];
} RsVec __attribute__((aligned(16)));

typedef struct {
    char pad0[0x20];
    unsigned char state;
    char pad21[0x85];
    short hA6;
} RsTargetMoby;

typedef struct {
    RsVec pos;
    RsVec v10;
    RsVec v20;
    RsVec v30;
    RsTargetMoby *moby;
    int kind;
} RsTarget;

typedef struct {
    char pad0[0x20];
    float f20;
    short h24;
    short h26;
    unsigned char b28;
    unsigned char b29;
    char pad2A[0xE];
    int w38;
    char pad3C[4];
    RsVec v40;
    char pad50[8];
    unsigned char b58;
    char pad59;
    unsigned char b5A;
    char pad5B[0x6D];
    short hC8;
    char padCA[6];
    void *pD0;
    char padD4[0x3C];
    char x110[0x10];
    char x120[0x10];
    float f130;
    float f134;
    float f138;
    float f13C;
    int w140;
    int w144;
    float f148;
    char pad14C[0x11];
    unsigned char b15D;
    char pad15E[0x12];
    float f170;
    float f174;
    char pad178[8];
    int w180;
    int w184;
    float f188;
    float f18C;
    char pad190[8];
    float f198;
    char pad19C[8];
    float f1A4;
    char pad1A8[0x10];
    int w1B8;
    char pad1BC[0x14];
    RsVec v1D0;
    RsVec v1E0;
    float f1F0;
    float f1F4;
    float f1F8;
    float f1FC;
    float f200;
    float f204;
    short h208;
    short h20A;
    int w20C;
    int w210;
    short h214;
    short h216;
    char *p218;
    int w21C;
    int w220;
    int w224;
    void *p228;
    int w22C;
    unsigned char x230[1];
} RsVars;

typedef struct {
    char pad0[0x10];
    RsVec pos;
    unsigned char state;
    unsigned char b21;
    char pad22[2];
    char *w24;
    char pad28[9];
    unsigned char b31;
    char pad32[2];
    unsigned short h34;
    char pad36[0xA];
    RsVec rot;
    char pad50[2];
    unsigned char b52;
    unsigned char b53;
    char pad54[4];
    float f58;
    char pad5C[0x14];
    unsigned char b70;
    char pad71[7];
    RsVars *vars;
    char pad7C[3];
    unsigned char b7F;
    char pad80[0x14];
    int w94;
    char pad98[0xC];
    unsigned char bA4;
    char padA5;
    unsigned short hA6;
    char padA8[0x14];
    unsigned char bBC;
} RsMoby;

typedef struct RsOther {
    char pad0[0x10];
    RsVec pos;
    signed char state;
    char pad21[7];
    struct RsOther *next;
    char pad2C[0x7A];
    short hA6;
} RsOther;

typedef struct {
    char pad0[0x10];
    RsVec pos;
    RsMoby *src;
    char pad24[8];
    float f2C;
} RsHit;

extern char *D_L13_001B07B0[];
extern char *D_L13_0015FFE4;
extern char *FUN_L00_0025a420(void *, int, int);
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_L13_0015F62C;
extern float D_L13_0015F630;
extern float robosquawker_damage_knockback_vertical_speed_gp __asm__("D_L13_00161390") __attribute__((sda));
extern float robosquawker_damage_knockback_horizontal_speed_gp __asm__("D_L13_00161394") __attribute__((sda));
extern float robosquawker_home_hop_horizontal_speed_gp __asm__("D_L13_00161398") __attribute__((sda));
#include "rnc/overlay/l13/gameplay/entities/elevator_data/robosquawker_update.h"
extern float D_L13_001670C0[];
extern float FUN_L04_001f1978(float);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern float approach_value(float *, float, float);
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int D_L13_0015F5C4;
extern int D_L13_001613B0[] __attribute__((section(".sdata")));
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00258830(void *, void *, float, float, float, int);
extern int FUN_L00_00258b50(void *, void *, void *, void *, float);
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_002dc9f0(void *, void *);
extern int count_active_moby_group_members(int, int) __asm__("FUN_L01_0026e008");
extern int select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern int select_list_point_toward_target(void *, void *, void *, void *) __asm__("FUN_L01_0028b5e0");
extern int reserve_expired_frame_slot(int *, int, int) __asm__("FUN_L01_002efbf8");
extern int select_robosquawker_target(void *, void *, float) __asm__("FUN_L13_0026bd00");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 rand(void);
extern s32 random_integer_below(s32) __asm__("func_00213260");
short D_L13_001613A0 = 0x505;
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void FUN_L00_0025a120(void *);
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025f730(void *, float);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void FUN_L01_0026d930(char *p);
extern void FUN_L01_0026e090(int, int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float absolute_float(float input) __asm__("func_001F99C0");
void update_robosquawker_particle(struct Moby *moby, char *data) __asm__("FUN_L13_002b4f28");
void update_robosquawker_ground_height(struct Moby *m, char *d) __asm__("FUN_L13_002b4ff0");
extern int func_001F9908_i(void *) __asm__("FUN_001f9740");

void robosquawker_update(RsMoby *m) __asm__("FUN_L13_002b50d8");
void robosquawker_update(RsMoby *robosquawker_moby) {
    enum {
        ROBOSQUAWKER_STATE_INITIALIZE = 0,
        ROBOSQUAWKER_STATE_GROUND_IDLE = 1,
        ROBOSQUAWKER_STATE_FACE_AWAY_WAIT_FOR_CHASE = 2,
        ROBOSQUAWKER_STATE_CHASE_TARGET = 3,
        ROBOSQUAWKER_STATE_PECK_ATTACK = 4,
        ROBOSQUAWKER_STATE_RETURN_HOME = 5,
        ROBOSQUAWKER_STATE_DAMAGE_KNOCKBACK = 6,
        ROBOSQUAWKER_STATE_RECOVER_TO_FLIGHT_IDLE = 7,
        ROBOSQUAWKER_STATE_WAIT_FOR_CLOSE_TARGET = 8,
        ROBOSQUAWKER_STATE_PREPARE_HOME_HOP = 0xA,
        ROBOSQUAWKER_STATE_HOME_HOP = 0xB,
        ROBOSQUAWKER_STATE_BLOCKED_RETURN_HOME = 0xC,
        ROBOSQUAWKER_STATE_WALK_FORWARD_AVOID_MOBYS = 0xD,
        ROBOSQUAWKER_STATE_FLIGHT_IDLE = 0xE,
        ROBOSQUAWKER_STATE_FLY_TO_RANDOM_POINT = 0xF,
        ROBOSQUAWKER_STATE_TAKEOFF = 0x10,
        ROBOSQUAWKER_STATE_LAND = 0x11,
        ROBOSQUAWKER_STATE_FOLLOW_FLIGHT_PATH = 0x12,
        ROBOSQUAWKER_STATE_DEATH_KNOCKBACK = 0x63
    };
    RsVars *robosquawker_vars;
    RsVec old;
    RsTarget selected_target_record;
    RsVec va;
    RsVec vb;
    RsVec vc;
    int hits;
    float dmg;
    float knockback_yaw;
    float flight_step_length;
    float out;
    RsHit *hit;
    RsTargetMoby *tm;

    robosquawker_vars = robosquawker_moby->vars;
    FUN_L00_00263ac8(2.5f, (int)robosquawker_moby, 0, robosquawker_vars->x230);
    if (robosquawker_moby->b31 != 0) {
        if (distance_xyz(&robosquawker_moby->pos, D_L13_001670C0) < 28.0f) {
            FUN_L00_0025a120(robosquawker_moby);
            robosquawker_moby->b7F = 0x16;
        }
    }
    if (D_L13_0015F5C4 == 2) {
        robosquawker_moby->w94 = 0;
        robosquawker_moby->h34 |= 0x41;
        return;
    }
    robosquawker_moby->w94 = *(int *)(robosquawker_moby->w24 + 0x10);
    robosquawker_moby->h34 &= 0xFFBE;
    select_robosquawker_target(robosquawker_moby, &selected_target_record, robosquawker_vars->f1FC + 4.0f);
    tm = selected_target_record.moby;
    if (tm == 0 || (robosquawker_vars->h214 == 0 && robosquawker_vars->w22C != -1 && is_point_inside_clip_volume(&selected_target_record, robosquawker_vars->w22C) == 0)) {
        selected_target_record.moby = 0;
        tm = 0;
        qzero(&selected_target_record);
    }
    qcopy(&old, &robosquawker_moby->pos);
    if (robosquawker_vars->w38 != 0) {
        robosquawker_vars->h214 = scale_game_frames_alt(0xF0);
    }
    robosquawker_vars->w38 = 0;
    fast_dec_timer_s16(&robosquawker_vars->h208);
    if (robosquawker_moby->state != 0) {
        if (fast_dec_timer_s16(&robosquawker_vars->h214) != 0) {
            robosquawker_vars->f1FC = robosquawker_vars->v1D0.f[3];
        } else {
            robosquawker_vars->f1FC = robosquawker_vars->f200;
        }
    }
    dmg = 0.0f;
    hit = FUN_L00_0025a420(robosquawker_moby, 0x330000, 0);
    fun_00213928(robosquawker_moby, hit, &robosquawker_vars->f20, 0, &hits, &dmg, 0, 4);
    fast_dec_timer_s16(&robosquawker_vars->h26);
    if (robosquawker_vars->h26 < scale_game_frames_alt(0x2D) && hit != 0 && robosquawker_moby->state != 0x63) {
        if (hit->src->hA6 >= 0xB0 && hit->src->hA6 <= 0xB1 && robosquawker_vars->h26 != 0) {
            hit->f2C = 0.0f;
        }
        if (robosquawker_moby->b21 != 0xFF) {
            FUN_L01_0026e090(robosquawker_moby->b21, 1);
        }
        if (hit->f2C != 0.0f) {
            knockback_yaw = fast_arc_tan(robosquawker_moby->pos.f[0] - selected_target_record.pos.f[0], robosquawker_moby->pos.f[1] - selected_target_record.pos.f[1]);
            robosquawker_vars->f148 = 0.5f;
            robosquawker_vars->w140 = 0x202;
            robosquawker_vars->f20 -= hit->f2C;
            robosquawker_vars->f130 = 0.0055f;
            robosquawker_vars->f134 = 0.0011f;
            robosquawker_vars->f138 = robosquawker_damage_knockback_horizontal_speed_gp * frame_time;
            robosquawker_vars->f13C = robosquawker_damage_knockback_vertical_speed_gp * frame_time;
            robosquawker_vars->w144 = 9;
            robosquawker_vars->b15D = 0;
            if (robosquawker_vars->f20 <= 0.0f) {
                robosquawker_moby->h34 &= 0xEFFF;
                robosquawker_vars->f138 = frame_time * 10.0f;
                robosquawker_vars->f13C = frame_time * 5.5f;
                *(u128 *)&va = *(u128 *)&hit->pos;
                trooper_knockback_arc(&va, &knockback_yaw, &robosquawker_vars->f138, &robosquawker_vars->f13C);
                trooper_start_knockback(robosquawker_moby, robosquawker_vars->x120, knockback_yaw, (rand() & 1) + 9, 1, 0);
                robosquawker_vars->f170 = 11.0f;
                robosquawker_vars->f174 = 18.0f;
                robosquawker_moby->state = 0x63;
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
                robosquawker_vars->x110[7] = 0x78;
                trooper_anim_update(robosquawker_moby, (short *)robosquawker_vars->x110);
                FUN_L00_00257470(robosquawker_moby, 0, -1);
            } else {
                *(u128 *)&va = *(u128 *)&hit->pos;
                trooper_knockback_arc(&va, &knockback_yaw, &robosquawker_vars->f138, &robosquawker_vars->f13C);
                trooper_start_knockback(robosquawker_moby, robosquawker_vars->x120, knockback_yaw, (rand() & 1) + 9, 1, 0);
                robosquawker_vars->f170 = 5.0f;
                robosquawker_vars->f174 = 10.0f;
                robosquawker_moby->state = 6;
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
                *(unsigned char *)&robosquawker_vars->x110[7] = 0xFA;
                robosquawker_vars->h26 = scale_game_frames_alt(0x3C);
                trooper_anim_update(robosquawker_moby, (short *)robosquawker_vars->x110);
            }
        }
    }
    robosquawker_moby->bA4 = 0xFF;
    switch (robosquawker_moby->state) {
    case ROBOSQUAWKER_STATE_INITIALIZE:
        qcopy(&robosquawker_vars->v1D0, &robosquawker_moby->pos);
        robosquawker_vars->v1D0.f[3] = robosquawker_vars->f1FC;
        if (robosquawker_vars->h20A != 0) {
            robosquawker_moby->state = 1;
            if (robosquawker_moby->b53 != 3) {
                blend_moby_animation_c(robosquawker_moby, 3, 0, 0);
            }
        } else {
            robosquawker_moby->state = 0xE;
            robosquawker_moby->pos.f[2] += 6.0f;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, 0);
            }
        }
        robosquawker_vars->h214 = 0;
        robosquawker_vars->h208 = 0;
        robosquawker_vars->b29 = 0;
        robosquawker_vars->f20 = 1.0f;
        robosquawker_vars->h24 = 1;
        robosquawker_vars->b58 = 8;
        robosquawker_vars->b5A = 4;
        FUN_L01_0026d930((char *)&robosquawker_vars->w180);
        robosquawker_vars->w180 = 0x38D;
        robosquawker_vars->f18C = 2.0f;
        robosquawker_vars->f188 = 2.0f;
        robosquawker_vars->f1A4 = robosquawker_vars->f204 * frame_time;
        robosquawker_vars->w1B8 |= 0x30;
        if (rand() & 1) {
            robosquawker_moby->h34 |= 0x8000;
        }
        robosquawker_vars->pD0 = &D_L13_001613A0;
        if (robosquawker_vars->w220 != -1) {
            robosquawker_vars->w224 = select_point_index_by_distance(&robosquawker_moby->pos, D_L13_001B07B0[robosquawker_vars->w220], 0.0f);
        }
        break;
    case ROBOSQUAWKER_STATE_FLIGHT_IDLE: {
        float d;

        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        subtract_vector_xyz(&va, &selected_target_record, &robosquawker_moby->pos);
        d = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        if (d > 1.0f
            && vector_length_xy(&va) < robosquawker_vars->f1FC + 4.0f
            && absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 10.0f
            && random_integer_below(0x13) == 0) {
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->state = 0x11;
            release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
        } else if (d > 1.0f && robosquawker_moby->bBC == 1 && random_integer_below(4) == 0) {
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
            robosquawker_moby->state = 0x11;
            clear_u64_value(&robosquawker_vars->v40);
            robosquawker_moby->bBC = 0;
        } else if (robosquawker_vars->w220 != -1) {
            char *path;

            path = D_L13_001B07B0[robosquawker_vars->w220];
            if (distance_xyz(&robosquawker_moby->pos, path + (robosquawker_vars->w224 * 16 + 16)) < 0.25f) {
                robosquawker_vars->w224 += 1;
                if (robosquawker_vars->w224 >= *(int *)path) {
                    robosquawker_vars->w224 = 0;
                }
            }
            qcopy(&robosquawker_vars->v1E0, path + robosquawker_vars->w224 * 16 + 16);
            robosquawker_moby->state = 0x12;
        } else if (func_001F9908_i(&robosquawker_vars->w20C) != 0) {
            int i;

            robosquawker_vars->w20C = scale_game_frames_alt(FUN_L00_00257b90(0x28, 0x78));
            for (i = 0; i < 10; i++) {
                float r;
                float a;

                r = random_float_between(0.0f, 4.0f);
                a = random_angle_radians();
                vb.f[0] = fast_cos(a) * r;
                vb.f[1] = fast_sin(a) * r;
                vb.f[2] = 0.0f;
                vb.f[2] = random_float_between(5.0f, 7.0f);
                add_vector_xyz(&robosquawker_vars->v1E0, &vb, &robosquawker_vars->v1D0);
                if (probe_ground_height(&robosquawker_vars->v1E0, 0x120, 0.5f) > 1.0f) {
                    break;
                }
            }
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->state = 0xF;
        }
        break;
    }
    case ROBOSQUAWKER_STATE_FLY_TO_RANDOM_POINT: {
        float a;

        a = fast_arc_tan(robosquawker_vars->v1E0.f[0] - robosquawker_moby->pos.f[0], robosquawker_vars->v1E0.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.03f, 0.6f, 0.09f);
        flight_step_length = vector_length_xyz(&robosquawker_vars->v40);
        robosquawker_moby->f58 = flight_step_length * 20.0f + 1.0f;
        subtract_vector_xyz(&robosquawker_vars->v40, &robosquawker_vars->v1E0, &robosquawker_moby->pos);
        if (vector_length_xyz(&robosquawker_vars->v40) > 1.0f) {
            approach_value(&flight_step_length, frame_time * 4.0f, frame_time_sq * 6.0f);
        } else {
            approach_value(&flight_step_length, frame_time, frame_time_sq * 8.0f);
        }
        FUN_L00_0025f730(&robosquawker_vars->v40, flight_step_length);
        add_vector_xyz(&va, &robosquawker_vars->v40, &robosquawker_moby->pos);
        if (coll_sphere(1.2f, &va, 0, robosquawker_moby) != 0) {
            clear_u64_value(&robosquawker_vars->v40);
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->state = 0xE;
        } else {
            qcopy(&robosquawker_moby->pos, &va);
        }
        if (distance_xyz(&robosquawker_moby->pos, &robosquawker_vars->v1E0) < flight_step_length || flight_step_length == 0.0f) {
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->state = 0xE;
        }
        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_FOLLOW_FLIGHT_PATH: {
        char *path;
        float a;
        float d;

        path = D_L13_001B07B0[robosquawker_vars->w220];
        a = fast_arc_tan(robosquawker_vars->v1E0.f[0] - robosquawker_moby->pos.f[0], robosquawker_vars->v1E0.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.018f, 0.8f, 0.09f);
        subtract_vector_xyz(&robosquawker_vars->v40, &robosquawker_vars->v1E0, &robosquawker_moby->pos);
        FUN_L00_0025f730(&robosquawker_vars->v40, frame_time * 4.0f);
        add_vector_xyz(&va, &robosquawker_vars->v40, &robosquawker_moby->pos);
        robosquawker_moby->f58 = vector_length_xyz(&robosquawker_vars->v40) * 20.0f + 1.0f;
        if (coll_sphere(1.2f, &va, 0, robosquawker_moby) != 0) {
            int i;

            robosquawker_vars->w20C = scale_game_frames_alt(0x78);
            for (i = 0; i < 10; i++) {
                float r;
                float an;

                r = random_float_between(0.0f, 3.0f);
                an = random_angle_radians();
                vb.f[0] = fast_cos(an) * r;
                vb.f[1] = fast_sin(an) * r;
                vb.f[2] = 0.0f;
                vb.f[2] = random_float_between(5.0f, 7.0f);
                add_vector_xyz(&robosquawker_vars->v1E0, &vb, &robosquawker_vars->v1D0);
                if (probe_ground_height(&robosquawker_vars->v1E0, 0x120, 0.5f) > 1.0f) {
                    break;
                }
            }
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->state = 0xF;
        } else {
            qcopy(&robosquawker_moby->pos, &va);
        }
        d = distance_xyz(&robosquawker_moby->pos, &robosquawker_vars->v1E0);
        if (d < vector_length_xyz(&robosquawker_vars->v40)) {
            float g;

            robosquawker_vars->w224 += 1;
            if (robosquawker_vars->w224 >= *(int *)path) {
                robosquawker_vars->w224 = 0;
            }
            qcopy(&robosquawker_vars->v1E0, path + robosquawker_vars->w224 * 16 + 16);
            robosquawker_vars->v1E0.f[2] = robosquawker_vars->v1E0.f[2] + random_float_between(-0.2f, 0.2f);
            g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
            if (g > 1.0f
                && vector_length_xy(&vb) < robosquawker_vars->f1FC + 4.0f
                && absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 10.0f
                && random_integer_below(0x13) == 0) {
                if (robosquawker_moby->b53 != 0) {
                    blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
                }
                qzero(&robosquawker_vars->v40);
                robosquawker_moby->state = 0x11;
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
            } else if (g > 1.0f && robosquawker_moby->bBC == 1 && random_integer_below(4) == 0) {
                if (random_integer_below(0x100) & 1) {
                    robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
                }
                robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
                robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
                if (robosquawker_moby->b53 != 0) {
                    blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
                }
                release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
                qzero(&robosquawker_vars->v40);
                robosquawker_moby->state = 0x11;
                clear_u64_value(&robosquawker_vars->v40);
                robosquawker_moby->bBC = 0;
            }
        }
        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_LAND: {
        float g;

        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        approach_value(&robosquawker_vars->v40.f[2], -(frame_time * 8.0f), frame_time_sq * 10.8f);
        FUN_L00_00258830(robosquawker_moby, &robosquawker_vars->v40, 0.6f, 0.5f, 0.0f, 0);
        if (robosquawker_moby->b53 != 2
            && robosquawker_moby->pos.f[2] - g <= frame_time * 8.0f * FUN_L04_001f1978(45.0f)
            && robosquawker_moby->b53 != 2) {
            blend_moby_animation_c(robosquawker_moby, 2, 0, scale_game_frames_alt(5));
        }
        if (robosquawker_moby->pos.f[2] <= g) {
            robosquawker_moby->pos.f[2] = g;
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            clear_u64_value(&robosquawker_vars->v40);
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
        }
        break;
    }
    case ROBOSQUAWKER_STATE_TAKEOFF: {
        float g;

        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        if (robosquawker_moby->pos.f[2] - g < 3.0f) {
            approach_value(&robosquawker_vars->v40.f[2], frame_time * 8.0f, frame_time_sq * 10.8f);
        } else {
            approach_value(&robosquawker_vars->v40.f[2], frame_time, frame_time_sq * 10.8f);
        }
        FUN_L00_00258830(robosquawker_moby, &robosquawker_vars->v40, 0.6f, 0.5f, 0.0f, 0);
        robosquawker_moby->f58 = 6.0f - (robosquawker_moby->pos.f[2] - g) / 6.0f * 5.0f;
        if (g + 6.0f <= robosquawker_moby->pos.f[2]) {
            robosquawker_moby->pos.f[2] = g + 6.0f;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->state = 0xE;
            clear_u64_value(&robosquawker_vars->v40);
        }
        update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_GROUND_IDLE:
        subtract_vector_xyz(&va, &selected_target_record, &robosquawker_moby->pos);
        if (vector_length_xyz(&va) < robosquawker_vars->f1FC + 4.0f
            && absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f
            && random_integer_below(0x13) == 0) {
            robosquawker_moby->state = 2;
            if (robosquawker_moby->b53 != 4) {
                blend_moby_animation_c(robosquawker_moby, 4, 0, scale_game_frames_alt(3));
            }
        } else if (robosquawker_moby->bBC == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->bBC = 0;
        } else if (vector_length_xyz(&va) > robosquawker_vars->f1FC + 6.0f && robosquawker_vars->h20A == 0) {
            qzero(&robosquawker_vars->v40);
            robosquawker_moby->state = 0x10;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
            }
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    case ROBOSQUAWKER_STATE_WAIT_FOR_CLOSE_TARGET:
        subtract_vector_xyz(&va, &selected_target_record, &robosquawker_moby->pos);
        if (vector_length_xyz(&va) < robosquawker_vars->f1FC - 4.0f
            && absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f
            && random_integer_below(0x13) == 0) {
            robosquawker_moby->state = 2;
            robosquawker_moby->pos.f[2] += 1.0f;
            if (robosquawker_moby->b53 != 4) {
                blend_moby_animation_c(robosquawker_moby, 4, 0, scale_game_frames_alt(3));
            }
        }
        break;
    case ROBOSQUAWKER_STATE_PREPARE_HOME_HOP: {
        float a;

        a = fast_arc_tan(robosquawker_vars->v1D0.f[0] - robosquawker_moby->pos.f[0], robosquawker_vars->v1D0.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        subtract_vector_xyz(&va, &selected_target_record, &robosquawker_vars->v1D0);
        if ((vector_length_xyz(&va) < robosquawker_vars->f1FC && absolute_float(robosquawker_vars->v1D0.f[2] - selected_target_record.pos.f[2]) < 8.0f) || robosquawker_moby->bBC == 2) {
            if (robosquawker_moby->b21 != 0xFF) {
                FUN_L01_0026e090(robosquawker_moby->b21, 2);
            }
            if (robosquawker_moby->b53 != (rand() & 1) + 9) {
                blend_moby_animation_c(robosquawker_moby, (rand() & 1) + 9, 0, scale_game_frames_alt(3));
            }
            robosquawker_vars->f134 = 0.0f;
            robosquawker_vars->f130 = robosquawker_home_hop_gravity_gp;
            robosquawker_vars->f138 = robosquawker_home_hop_horizontal_speed_gp * frame_time;
            robosquawker_vars->f13C = FUN_L00_0025abf0(&robosquawker_moby->pos, &robosquawker_vars->v1D0, &out, robosquawker_home_hop_horizontal_speed_gp * frame_time, -robosquawker_home_hop_gravity_gp);
            robosquawker_vars->w144 = 0xD;
            robosquawker_vars->b15D = 0;
            trooper_start_knockback(robosquawker_moby, robosquawker_vars->x120, robosquawker_moby->rot.f[2], (rand() & 1) + 9, 1, 0);
            robosquawker_vars->f170 = 5.0f;
            robosquawker_vars->f174 = 10.0f;
            update_robosquawker_particle((unsigned char *)robosquawker_moby, (char *)robosquawker_vars);
            robosquawker_moby->state = 0xB;
        }
        break;
    }
    case ROBOSQUAWKER_STATE_HOME_HOP:
        if (mine_fall(robosquawker_moby, robosquawker_vars->x120) & 1) {
            release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
            qzero(robosquawker_vars->x120);
            robosquawker_moby->state = 3;
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 8, scale_game_frames_alt(3));
            }
            return;
        } else if (robosquawker_moby->pos.f[2] < 5.0f) {
            FUN_L00_00257470(robosquawker_moby, 0, -1);
            mark_moby_for_removal_c(robosquawker_moby);
            return;
        }
        break;
    case ROBOSQUAWKER_STATE_FACE_AWAY_WAIT_FOR_CHASE: {
        float a;

        a = fast_arc_tan(robosquawker_moby->pos.f[0] - selected_target_record.pos.f[0], robosquawker_moby->pos.f[1] - selected_target_record.pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.02f, 0.3f, 0.1f);
        subtract_vector_xyz(&va, &selected_target_record, &robosquawker_moby->pos);
        if (vector_length_xyz(&va) < robosquawker_vars->f1FC
            && absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f
            && random_integer_below(0x13) == 0) {
            if (robosquawker_moby->b21 != 0xFF) {
                FUN_L01_0026e090(robosquawker_moby->b21, 1);
            }
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->bBC = 0;
        } else if (robosquawker_moby->bBC == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                robosquawker_vars->f1F8 = -robosquawker_vars->f1F8;
            }
            robosquawker_vars->f1F4 = robosquawker_vars->f1F8 * 0.017453292f;
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
            robosquawker_moby->bBC = 0;
        }
        if (robosquawker_moby->b53 == 4 && (robosquawker_moby->b70 & 2)) {
            blend_moby_animation_c(robosquawker_moby, 3, 0, scale_game_frames_alt(3));
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_CHASE_TARGET: {
        float r;
        int res;
        float a;
        float d;

        r = absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]);
        r = r / vector_distance(&robosquawker_moby->pos, &selected_target_record);
        if (robosquawker_vars->w21C != -1) {
            select_list_point_toward_target(robosquawker_moby, D_L13_001B07B0[robosquawker_vars->w21C], &selected_target_record, &vb);
        } else {
            qcopy(&vb, &selected_target_record);
        }
        a = fast_arc_tan(vb.f[0] - robosquawker_moby->pos.f[0], vb.f[1] - robosquawker_moby->pos.f[1]) + robosquawker_vars->f1F4;
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(robosquawker_moby, &robosquawker_vars->w180, &va, &vb, 1.0f);
        if (func_001F9908_i(&robosquawker_vars->w210) != 0) {
            robosquawker_vars->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            robosquawker_vars->f1F4 = -robosquawker_vars->f1F4;
        }
        subtract_vector_xyz(&vc, &selected_target_record, &robosquawker_vars->v1D0);
        d = vector_distance(&robosquawker_moby->pos, &selected_target_record);
        if ((d < 1.5f
             || (selected_target_record.moby != 0 && selected_target_record.moby->hA6 == 0x10E && selected_target_record.moby->state != 0xFE && selected_target_record.moby->state != 0xFD && d < 2.3f))
            && !(res & 2)) {
            robosquawker_moby->state = 4;
            if (robosquawker_moby->b53 != 6) {
                blend_moby_animation_c(robosquawker_moby, 6, 0, scale_game_frames_alt(3));
            }
        } else {
            if (r > 1.0f || (res & 2)) {
                robosquawker_moby->pos.f[0] = old.f[0];
                robosquawker_moby->pos.f[1] = old.f[1];
            }
            if (vector_length_xyz(&vc) < robosquawker_vars->f1FC + 4.0f && absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]) < 8.0f) {
                if (robosquawker_moby->b21 != 0xFF) {
                    FUN_L01_0026e090(robosquawker_moby->b21, 1);
                }
            } else if (robosquawker_moby->bBC != 1) {
                robosquawker_moby->state = 5;
            }
        }
        robosquawker_moby->bBC = 0;
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_PECK_ATTACK: {
        float a;
        int same;
        float hh;
        float dz;
        float d;
        float da;
        float g;

        a = fast_arc_tan(selected_target_record.pos.f[0] - robosquawker_moby->pos.f[0], selected_target_record.pos.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        same = robosquawker_moby->b52 == robosquawker_moby->b53;
        hh = compute_interpolated_record_value(robosquawker_moby);
        dz = absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]);
        d = vector_distance(&robosquawker_moby->pos, &selected_target_record);
        da = fast_difference_between_rotations(fast_arc_tan(selected_target_record.pos.f[0] - robosquawker_moby->pos.f[0], selected_target_record.pos.f[1] - robosquawker_moby->pos.f[1]), robosquawker_moby->rot.f[2]);
        if (same
            && absolute_float(hh - 13.0f) < 0.2f
            && dz < 1.5f
            && (d < 2.0f
                || (selected_target_record.moby != 0 && selected_target_record.moby->hA6 == 0x10E && selected_target_record.moby->state != 0xFE && selected_target_record.moby->state != 0xFD && d < 2.6f))
            && da < 0.2617994f) {
            va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 0.2f;
            va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 0.2f;
            va.f[2] = 0.0f;
            qcopy(&vb, &selected_target_record);
            vb.f[2] += 0.75f;
            if (tm != 0) {
                spawn_nearby_effect(tm, (int)robosquawker_moby, 1, &vb, &va, 1.0f);
            }
        }
        if ((robosquawker_moby->b70 & 2) && vector_distance(&robosquawker_moby->pos, &selected_target_record) > 1.5f) {
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
        }
        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        robosquawker_vars->f198 -= frame_time_sq * 9.8f;
        robosquawker_moby->pos.f[2] += robosquawker_vars->f198;
        if (robosquawker_moby->pos.f[2] < g) {
            robosquawker_vars->f198 = 0.0f;
            robosquawker_moby->pos.f[2] = g;
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        break;
    }
    case ROBOSQUAWKER_STATE_RETURN_HOME: {
        int res;
        float a;

        if (robosquawker_vars->w21C != -1) {
            select_list_point_toward_target(robosquawker_moby, D_L13_001B07B0[robosquawker_vars->w21C], &robosquawker_vars->v1D0, &vb);
        } else {
            qcopy(&vb, &robosquawker_vars->v1D0);
        }
        a = fast_arc_tan(vb.f[0] - robosquawker_moby->pos.f[0], vb.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.02f, 0.3f, 0.1f);
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(robosquawker_moby, &robosquawker_vars->w180, &va, &vb, 1.0f);
        if (vector_distance(&robosquawker_moby->pos, &robosquawker_vars->v1D0) < 2.0f) {
            robosquawker_moby->state = 1;
            if (robosquawker_moby->b53 != 3) {
                blend_moby_animation_c(robosquawker_moby, 3, 0, scale_game_frames_alt(3));
            }
        }
        subtract_vector_xyz(&vc, &selected_target_record, &robosquawker_vars->v1D0);
        if (vector_length_xyz(&vc) < robosquawker_vars->f1FC) {
            if (robosquawker_moby->b21 != 0xFF) {
                FUN_L01_0026e090(robosquawker_moby->b21, 1);
            }
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
        } else if (robosquawker_moby->bBC == 1) {
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
        } else if (res & 2) {
            robosquawker_moby->pos.f[0] = old.f[0];
            robosquawker_moby->pos.f[1] = old.f[1];
            robosquawker_moby->state = 0xC;
            if (robosquawker_moby->b53 != 4) {
                blend_moby_animation_c(robosquawker_moby, 4, 0, scale_game_frames_alt(3));
            }
        }
        update_robosquawker_ground_height((char *)robosquawker_moby, (char *)robosquawker_vars);
        robosquawker_moby->bBC = 0;
        break;
    }
    case ROBOSQUAWKER_STATE_DAMAGE_KNOCKBACK:
        if (mine_fall(robosquawker_moby, robosquawker_vars->x120) & 1) {
            qzero(robosquawker_vars->x120);
            robosquawker_moby->state = 3;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
            return;
        } else if (robosquawker_moby->pos.f[2] < 5.0f) {
            FUN_L00_00257470(robosquawker_moby, 0, -1);
            mark_moby_for_removal_c(robosquawker_moby);
            return;
        }
        break;
    case ROBOSQUAWKER_STATE_RECOVER_TO_FLIGHT_IDLE:
        release_robosquawker_particle((Level13VendorMoby *)robosquawker_vars);
        if (FUN_L00_002dc9f0(robosquawker_moby, robosquawker_vars->x120) != 0) {
            robosquawker_moby->state = 0xE;
            if (robosquawker_moby->b53 != 0) {
                blend_moby_animation_c(robosquawker_moby, 0, 0, 0);
            }
            robosquawker_vars->hC8 = 0;
        }
        break;
    case ROBOSQUAWKER_STATE_DEATH_KNOCKBACK: {
        int slot_reuse_delay_frames;

        robosquawker_moby->h34 &= 0xEFFF;
        if (mine_fall(robosquawker_moby, robosquawker_vars->x120) & 0x140) {
            clear_u64_value(&va);
            slot_reuse_delay_frames = (int)((float)scale_game_frames_alt(1) * 60.0f);
            if (reserve_expired_frame_slot(D_L13_001613B0, 3, slot_reuse_delay_frames) >= 0) {
                cart_burst(robosquawker_moby, &robosquawker_moby->pos, 4, 0.5f, 13.0f);
            }
            mark_moby_for_removal_c(robosquawker_moby);
            return;
        } else if (robosquawker_moby->pos.f[2] < 0.0f) {
            mark_moby_for_removal_c(robosquawker_moby);
            return;
        }
        break;
    }
    case ROBOSQUAWKER_STATE_BLOCKED_RETURN_HOME: {
        float r;
        int res;
        float a;

        r = absolute_float(robosquawker_moby->pos.f[2] - selected_target_record.pos.f[2]);
        r = r / vector_distance(&robosquawker_moby->pos, &selected_target_record);
        if (robosquawker_vars->w21C != -1) {
            select_list_point_toward_target(robosquawker_moby, D_L13_001B07B0[robosquawker_vars->w21C], &robosquawker_vars->v1D0, &vb);
        } else {
            qcopy(&vb, &robosquawker_vars->v1D0);
        }
        a = fast_arc_tan(vb.f[0] - robosquawker_moby->pos.f[0], vb.f[1] - robosquawker_moby->pos.f[1]);
        FUN_L00_00258278((char *)robosquawker_moby, &robosquawker_vars->f1F0, a, 0.05f, 0.3f, 0.2f);
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(robosquawker_moby, &robosquawker_vars->w180, &va, &vb, 1.0f);
        subtract_vector_xyz(&vc, &selected_target_record, &robosquawker_vars->v1D0);
        if (vector_distance(&robosquawker_moby->pos, &selected_target_record) < 1.5f && r <= 1.0f) {
            robosquawker_moby->state = 4;
            if (robosquawker_moby->b53 != 6) {
                blend_moby_animation_c(robosquawker_moby, 6, 0, scale_game_frames_alt(3));
            }
        } else if (r <= 1.0f && !(res & 2)) {
            robosquawker_moby->state = 5;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
        } else {
            if (robosquawker_moby->b70 & 2) {
                int anim;

                anim = 4;
                random_integer_below(4);
                if (random_integer_below(4) != 0) {
                    anim = 3;
                }
                blend_moby_animation_c(robosquawker_moby, anim, 0, scale_game_frames_alt(3));
            }
            if (random_integer_below(0x63) == 0) {
                qzero(&robosquawker_vars->v40);
                robosquawker_moby->state = 0x10;
                if (robosquawker_moby->b53 != 0) {
                    blend_moby_animation_c(robosquawker_moby, 0, 0, scale_game_frames_alt(3));
                }
            }
        }
        robosquawker_moby->bBC = 0;
        break;
    }
    case ROBOSQUAWKER_STATE_WALK_FORWARD_AVOID_MOBYS: {
        RsOther *candidate_moby;
        float g;

        if (robosquawker_moby->b52 != 5 && robosquawker_moby->b53 != 5) {
            blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
        }
        va.f[0] = fast_cos(robosquawker_moby->rot.f[2]) * (robosquawker_vars->f204 * frame_time);
        va.f[1] = fast_sin(robosquawker_moby->rot.f[2]) * (robosquawker_vars->f204 * frame_time);
        va.f[2] = 0.0f;
        add_vector_xyz(&vb, &robosquawker_moby->pos, &va);
        for (candidate_moby = (RsOther *)D_L13_0015FFE4; candidate_moby != 0; candidate_moby = candidate_moby->next) {
            if (candidate_moby == (RsOther *)robosquawker_moby) {
                continue;
            }
            if (candidate_moby->state < 0) {
                continue;
            }
            if (candidate_moby->hA6 != 0 && candidate_moby->hA6 != 0x3F) {
                continue;
            }
            if (vector_distance(&vb, &candidate_moby->pos) < 2.0f) {
                float z;
                float a;
                float d;

                z = robosquawker_moby->pos.f[2];
                a = fast_arc_tan(candidate_moby->pos.f[0] - robosquawker_moby->pos.f[0], candidate_moby->pos.f[1] - robosquawker_moby->pos.f[1]);
                a = fast_subtract_rotations(robosquawker_moby->rot.f[2], a);
                d = frame_time * 1.5707964f;
                if (!(a > 0.0f)) {
                    d = -d;
                }
                robosquawker_moby->rot.f[2] = fast_add_rotations(robosquawker_moby->rot.f[2], d);
                subtract_vector_xyz(&vb, &robosquawker_moby->pos, &candidate_moby->pos);
                normalize_vector_xyz(&vb, &vb, 2.0f);
                add_vector_xyz(&vb, &vb, &candidate_moby->pos);
                vb.f[2] = z;
            }
        }
        qcopy(&robosquawker_moby->pos, &vb);
        g = probe_ground_height(&robosquawker_moby->pos, 0x120, 0.5f);
        robosquawker_vars->f198 -= frame_time_sq * 9.8f;
        robosquawker_moby->pos.f[2] += robosquawker_vars->f198;
        if (robosquawker_moby->pos.f[2] < g) {
            robosquawker_vars->f198 = 0.0f;
            robosquawker_moby->pos.f[2] = g;
        }
        if (robosquawker_vars->p218 == 0 || vector_distance(&robosquawker_moby->pos, robosquawker_vars->p218 + 0x10) > convert_integer_to_float(robosquawker_vars->h216)) {
            robosquawker_moby->state = 5;
            if (robosquawker_moby->b53 != 5) {
                blend_moby_animation_c(robosquawker_moby, 5, 0, scale_game_frames_alt(3));
            }
        }
        robosquawker_moby->bBC = 0;
        break;
    }
    }
    trooper_anim_finish(robosquawker_moby, robosquawker_vars->x110);
    if (robosquawker_moby->pos.f[0] < 2.0f || robosquawker_moby->pos.f[0] > 1021.0f
        || robosquawker_moby->pos.f[1] < 2.0f || robosquawker_moby->pos.f[1] > 1021.0f
        || robosquawker_moby->pos.f[2] < 2.0f || robosquawker_moby->pos.f[2] > 1021.0f) {
        mark_moby_for_removal_c(robosquawker_moby);
        return;
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B9800), where it is exact; names translated to the US level program. */


extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");
extern void spawn_type_74_particle(void *, float *, float *, void *, int) __asm__("FUN_L01_0028a7a8");

short D_L13_001613D8 = 0xff;
short D_L13_001613E8 = -1;
short D_L13_001613F8 = 0x7f4f;
short D_L13_00161408 = -32513;
short D_L13_00161418 = 0x7f00;
void spawn_fighter_jet_joint_particles(void *moby, char *flight_data) __asm__("FUN_L13_002b8590");

void spawn_fighter_jet_joint_particles(void *moby, char *flight_data) {
    float initial_particle_velocity[4];
    float joint_world_position[4];
    float first_particle_spawn_position[4];
    float second_particle_spawn_position[4];
    float third_particle_spawn_position[4];
    float particle_velocity_scale;
    int i;

    for (i = 1; i < 3; i++) {
        *(OvlQuad *)initial_particle_velocity = 0;
        particle_velocity_scale = *(float *)(flight_data + 0x64) / (frame_time * 24.0f);
        if (particle_velocity_scale > 1.0f)
            particle_velocity_scale = 1.0f;
        else if (particle_velocity_scale < 0.35f)
            particle_velocity_scale = 0.35f;
        moby_point_position(moby, i, joint_world_position);
        joint_world_position[3] = 0;
        qcopy(first_particle_spawn_position, joint_world_position);
        qcopy(second_particle_spawn_position, joint_world_position);
        qcopy(third_particle_spawn_position, joint_world_position);
        add_random_offset_to_vector_xyz(first_particle_spawn_position, *(float *)(flight_data + 0xC4) * 0.02f + 0.02f);
        add_random_offset_to_vector_xyz(second_particle_spawn_position, *(float *)(flight_data + 0xC4) * 0.02f + 0.02f);
        add_random_offset_to_vector_xyz(third_particle_spawn_position, *(float *)(flight_data + 0xC4) * 0.01f + 0.02f);
        initial_particle_velocity[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 2.0f) * frame_time) *
            particle_velocity_scale;
        spawn_type_74_particle(moby, first_particle_spawn_position, initial_particle_velocity, &D_L13_001613D8, 1);
        initial_particle_velocity[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 3.0f) * frame_time) *
            particle_velocity_scale;
        spawn_type_74_particle(moby, second_particle_spawn_position, initial_particle_velocity, &D_L13_001613E8, 1);
        initial_particle_velocity[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 4.0f) * frame_time) *
            particle_velocity_scale;
        spawn_type_74_particle(moby, third_particle_spawn_position, initial_particle_velocity, &D_L13_001613F8, 1);
        if (*(float *)(flight_data + 0xC4) > 0.5f) {
            add_random_offset_to_vector_xyz(first_particle_spawn_position, *(float *)(flight_data + 0xC4) * 0.02f + 0.01f);
            add_random_offset_to_vector_xyz(second_particle_spawn_position, *(float *)(flight_data + 0xC4) * 0.02f + 0.01f);
            add_random_offset_to_vector_xyz(third_particle_spawn_position, *(float *)(flight_data + 0xC4) * 0.01f + 0.01f);
            initial_particle_velocity[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 2.0f) *
                       frame_time) *
                     particle_velocity_scale;
            spawn_type_74_particle(moby, first_particle_spawn_position, initial_particle_velocity, &D_L13_001613D8, 1);
            initial_particle_velocity[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 3.0f) *
                       frame_time) *
                     particle_velocity_scale;
            spawn_type_74_particle(moby, second_particle_spawn_position, initial_particle_velocity, &D_L13_001613E8, 1);
            initial_particle_velocity[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 4.0f) *
                       frame_time) *
                     particle_velocity_scale;
            spawn_type_74_particle(moby, third_particle_spawn_position, initial_particle_velocity, &D_L13_001613F8, 1);
            initial_particle_velocity[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 2.0f) *
                       frame_time) *
                     particle_velocity_scale;
            spawn_type_74_particle(moby, first_particle_spawn_position, initial_particle_velocity, &D_L13_00161408, 1);
            initial_particle_velocity[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 3.0f) *
                       frame_time) *
                     particle_velocity_scale;
            spawn_type_74_particle(moby, joint_world_position, initial_particle_velocity, &D_L13_00161418, 1);
        }
    }
}
#include "sda.h"
#include "qzero.h"
typedef struct {
    short tex;
    short u;
    short v;
    short w;
    short h;
    short fA;
    short x;
    short y;
} IconEC5D0;
extern IconEC5D0 D_L13_001CBDB8[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float vector_length_xyz(void *a);
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern unsigned char frame_size[] __asm__("D_0013E504");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

/* Draws a map icon for a moby relative to the hero (level copy of FUN_L17_002eaae0 without the mirror flag). */
void draw_fighter_jet_radar_icon(char *radar_rotation, char *from, char *moby, int icon, long color, int tex) __asm__("FUN_L13_002b89f8");

void draw_fighter_jet_radar_icon(char *radar_rotation, char *from, char *moby, int icon, long color, int tex) {
    float turn[4];
    float radar_rotation_matrix[16];
    float delta[4];
    float radar_offset[4];
    float radar_distance;
    int x, y;
    IconEC5D0 *icon_definition;

    qzero(turn);
    turn[2] = *(float *)(radar_rotation + 8);
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(radar_rotation_matrix, turn);
    subtract_vector_xyz(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(radar_offset, delta, radar_rotation_matrix);
    scale_vector_xyz(radar_offset, radar_offset, 0.0714285746f);
    radar_distance = vector_length_xyz(radar_offset);
    x = func_001FA898_r(radar_offset[0]);
    y = func_001FA898_r(-radar_offset[1]);
    if (radar_distance < 46.0f) {
        if (radar_distance > 38.0f) {
            float distance_weighted_alpha;
            radar_distance = 46.0f - radar_distance;
            distance_weighted_alpha = convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * radar_distance;
            color &= 0xFFFFFF;
            color = (func_001FA898_r(distance_weighted_alpha * 0.125f) << 24) | color;
        }
        icon_definition = &D_L13_001CBDB8[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - icon_definition->x, y - icon_definition->y, icon_definition->w, icon_definition->h, icon_definition->u, icon_definition->v, icon_definition->w, icon_definition->h, color,
                           get_effect_texture(tex + 0x28 + icon_definition->tex));
    }
}
/* Ported from rac1-decomp src/overlays/l13_gemlik/vendor_002B2020.c (func_L13_002B9E50) */

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Walks the moby list: picks the on-screen target, draws the radar icons and target markers, and points an arrow at
 * off-screen pickups. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B9E50), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x34];
    unsigned short f34;
    char pad36[2];
    long f38;
} Moby_2B9E50;

extern char D_L13_001672D0[];
extern char D_L13_001CBD40[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L13_001672E0[];
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float seg_sqrt(float) __asm__("func_001F9988");
extern int frame_count __asm__("D_L13_0015F5CC");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
short D_L13_001613C8 = 2;
extern unsigned char D_0013E500[];
extern unsigned char D_0013F3D0[];
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
extern void emit_transformed_2d_vertex_packet(void *, int, int, long, float, float, float, float) __asm__("FUN_L13_002b7d50");
extern void draw_fighter_jet_radar_icon(char *rot, char *from, char *moby, int icon, long color, int tex) __asm__("FUN_L13_002b89f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void project_to_screen(f32 *, void *) __asm__("func_001F2070");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

int select_fighter_jet_target_and_draw_indicators(int source_moby, char *moby_vars, char *radar_origin_position, void *radar_rotation, float f0, float f1, float f2, char *current_target_moby, char *excluded_target_moby) __asm__("FUN_L13_002b8be0");

int select_fighter_jet_target_and_draw_indicators(int source_moby, char *moby_vars, char *radar_origin_position, void *radar_rotation, float f0, float f1, float f2, char *current_target_moby, char *excluded_target_moby) {
    float v[4];
    float offscreen_arrow_rotation_matrix[16];
    float u[4];
    int sx, sy, tx, ty;
    char *candidate_moby;
    char *selected_target_moby;
    int best;
    int *scr;
    int dx, dy, target_selection_radius, screen_distance_squared;
    float ang, source_to_candidate_distance, yaw_error, k, angular_error_magnitude, arrow_pulse_phase;
    char *tab;

    candidate_moby = D_L13_0015FFE4;
    selected_target_moby = 0;
    best = 0x7FFFFFFF;
    *(int *)(moby_vars + 0xE8) = 0;
    while (candidate_moby != 0) {
        tab = D_L13_001CBD40;
        scr = (int *)(D_0013E500);
        if ((((Moby_2B9E50 *)candidate_moby)->f34 & 0x1000) && !(((Moby_2B9E50 *)candidate_moby)->f34 & 3) && *(unsigned char *)(candidate_moby + 0x31) != 0 &&
            candidate_moby != 0 && *(char **)(candidate_moby + 0x24) != 0 && *(short *)(*(char **)(candidate_moby + 0x24) + 0x46) == 5) {
            project_world_point_to_screen_pixels(candidate_moby + 0x10, &sx, &sy, 0);
            dx = sx - *(int *)(moby_vars + 0xE0);
            dy = sy - *(int *)(moby_vars + 0xE4);
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;
            qcopy(v, D_L13_001672E0);
            scale_vector_xyz(v, D_L13_001672E0, *(float *)(candidate_moby + 0xC) / 1280.0f);
            add_vector_xyz(v, v, candidate_moby + 0x10);
            project_world_point_to_screen_pixels(v, &tx, &ty, 0);
            tx -= sx;
            ty -= sy;
            target_selection_radius = func_001FA898_r(seg_sqrt(convert_integer_to_float(tx * tx + ty * ty))) + 0x1C;
            if (dx < target_selection_radius && dy < target_selection_radius) {
                if (current_target_moby != 0) {
                    if (candidate_moby == current_target_moby) {
                        best = 0;
                        selected_target_moby = candidate_moby;
                    }
                } else if (candidate_moby != excluded_target_moby) {
                    screen_distance_squared = dx * dx + dy * dy;
                    if (screen_distance_squared < best) {
                        best = screen_distance_squared;
                        selected_target_moby = candidate_moby;
                    }
                }
            }
        }
        switch (*(short *)(candidate_moby + 0xA6)) {
        case 0xE0:
        case 0x4C2:
            draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 2, 0x75808080, *(int *)(moby_vars + 0x104));
            break;
        case 0xE4:
        case 0x4C4:
            draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 2, 0x75808080, *(int *)(moby_vars + 0x104));
            break;
        case 0x52:
            draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 1, 0x75004080, *(int *)(moby_vars + 0x104));
            break;
        case 0x53:
            draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 1, 0x75007080, *(int *)(moby_vars + 0x104));
            break;
        case 0x184:
        case 0x191:
        case 0x3EB:
        case 0x504:
            project_to_screen(v, candidate_moby + 0x10);
            if (*(short *)(candidate_moby + 0xA6) != 0x184) {
                if (*(unsigned char *)(candidate_moby + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f, ang);
                    }
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f, -ang);
                    }
                }
            }
            if (*(short *)(candidate_moby + 0xA6) != 0x191) {
                if (*(unsigned char *)(candidate_moby + 0x31) == 0) {
                    char *np = candidate_moby + 0x10;
                    int arrow_alpha;
                    transpose_rotation_matrix(offscreen_arrow_rotation_matrix, D_L13_001672D0);
                    subtract_vector_xyz(u, np, D_0013F3D0);
                    transform_vector(u, u, offscreen_arrow_rotation_matrix);
                    u[0] = -u[1];
                    u[1] = -u[2];
                    u[2] = 0.0f;
                    normalize_vector_xyz(u, u, 160.0f);
                    ang = fast_add_rotations(fast_arc_tan(u[0], u[1]), 1.5707964f);
                    u[0] += 256.0f;
                    u[1] += 208.0f;
                    source_to_candidate_distance = distance_xyz((char *)source_moby + 0x10, np);
                    yaw_error = absolute_float(fast_subtract_rotations(fast_arc_tan(*(float *)(candidate_moby + 0x10) - *(float *)((char *)source_moby + 0x10),
                                                                      *(float *)(candidate_moby + 0x14) - *(float *)((char *)source_moby + 0x14)),
                                                    *(float *)(moby_vars + 0x38)));
                    k = absolute_float(fast_subtract_rotations(-fast_arc_tan(vector_distance((char *)source_moby + 0x10, np),
                                                                       *(float *)(candidate_moby + 0x18) - *(float *)((char *)source_moby + 0x18)),
                                                    *(float *)(moby_vars + 0x34)));
                    angular_error_magnitude = seg_sqrt(yaw_error * yaw_error + k * k);
                    arrow_pulse_phase = fast_add_rotations(*(float *)(moby_vars + 0x114), 0.1f / (angular_error_magnitude / 3.1415927f));
                    *(float *)(moby_vars + 0x114) = arrow_pulse_phase;
                    arrow_alpha = func_001FA898_r((fast_sin(arrow_pulse_phase) + 1.4f) * 53.0f);
                    if (yaw_error < 0.7853982f && k < 0.7853982f) {
                        {
                            float x = (v[0] - scr[4]) * 0.0625f;
                            float y = (v[1] - scr[5]) * 0.0625f;
                            float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                            emit_transformed_2d_vertex_packet(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f, ang);
                        }
                        {
                            float x = (v[0] - scr[4]) * 0.0625f;
                            float y = (v[1] - scr[5]) * 0.0625f;
                            float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                            emit_transformed_2d_vertex_packet(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f, -ang);
                        }
                    } else {
                        emit_transformed_2d_vertex_packet(&D_L13_001613C8, 3, 0xFFFFF3, (arrow_alpha << 24) | 0x8000, u[0], u[1], 240.0f / source_to_candidate_distance + 1.5f, ang);
                    }
                }
                draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 0, 0x75808080, *(int *)(moby_vars + 0x104));
                if (*(short *)(candidate_moby + 0xA6) != 0x191) {
                    *(int *)(moby_vars + 0xE8) = *(int *)(moby_vars + 0xE8) + 1;
                }
            }
            break;
        case 0x6F:
        case 0x4BE:
            if (!(*(unsigned short *)(candidate_moby + 0x34) & 1)) {
                project_to_screen(v, candidate_moby + 0x10);
                if (*(unsigned char *)(candidate_moby + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(D_L13_001CBD40, 0x1E, 0xFFFFF3, 0xFF00FF00, x, y, 1.0f, -ang);
                    }
                }
                draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 1, 0x75808080, *(int *)(moby_vars + 0x104));
            }
            break;
        case 0x4BF:
            if (!(*(unsigned short *)(candidate_moby + 0x34) & 1) && *(unsigned char *)(candidate_moby + 0x20) >= 2) {
                project_to_screen(v, candidate_moby + 0x10);
                if (*(unsigned char *)(candidate_moby + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(D_L13_001CBD40, 0x1E, 0xFFFFF3, 0xFF00FF00, x, y, 1.0f, -ang);
                    }
                }
                draw_fighter_jet_radar_icon(radar_rotation, radar_origin_position, candidate_moby, 1, 0x75808080, *(int *)(moby_vars + 0x104));
            }
            break;
        case 0x185: case 0x186: case 0x187: case 0x188:
        case 0x189: case 0x18A: case 0x18B: case 0x18C:
        case 0x18F: case 0x190:
        case 0x534: case 0x535: case 0x536: case 0x537:
        case 0x538: case 0x539: case 0x53A:
            break;
        }
        candidate_moby = *(char **)(candidate_moby + 0x28);
    }
    return (int)selected_target_moby;
}
#include "qcopy.h"

/* Per-frame target tracking: validates the current target, re-picks one and drops it when lost. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BA828), where it is exact; names translated to the US level program. */

extern char D_L13_001670D0[];
extern int D_L13_0015F5C4;
extern char D_0013C940[];
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
int select_fighter_jet_target_and_draw_indicators(int a, char *p, char *a2, void *a3, float f0, float f1, float f2, char *cur, char *g) __asm__("FUN_L13_002b8be0");
extern void tick_countdown_32(int *arg0) __asm__("FUN_001f9740");

#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(char **)((char *)(p) + (o)))
#define H(p, o) (*(short *)((char *)(p) + (o)))
#define B(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define FLAG    hero.ship_ammo

void update_fighter_jet_target_tracking(int source_moby, char *moby_vars) __asm__("FUN_L13_002b95b8");

void update_fighter_jet_target_tracking(int source_moby, char *moby_vars) {
    char *excluded_target_moby = 0;
    char *pad;
    char *current_target_moby;
    char *selected_target_moby;
    float radar_rotation_copy[4];
    if (!(D_L13_0015F5C4 >= 3 && D_L13_0015F5C4 <= 4)) {
        tick_countdown_32((int *)(moby_vars + 0x8C));
    }
    if (W(moby_vars, 0x8C) != 0 && P(moby_vars, 0x88) != 0 && H(P(moby_vars, 0x88), 0xA6) == W(moby_vars, 0x124) &&
        B(P(moby_vars, 0x88), 0x20) != 0xFE && B(P(moby_vars, 0x88), 0x20) != 0xFD) {
        pad = D_0013C940;
        if (B(P(moby_vars, 0x88), 0x31) != 0) {
            if (W(pad, 0x1B4) & 0x2000) {
                goto take;
            }
            if (FLAG != 0) {
                goto skip;
            }
        }
    }
    pad = D_0013C940;
    if (W(pad, 0x1B4) & 0x2000) {
    take:
        excluded_target_moby = P(moby_vars, 0x88);
    }
    W(moby_vars, 0xEC) = 0;
    W(moby_vars, 0x124) = -1;
    W(moby_vars, 0x88) = 0;
skip:
    qcopy(radar_rotation_copy, D_L13_001670D0);
    selected_target_moby = (char *)select_fighter_jet_target_and_draw_indicators(source_moby, moby_vars, D_L13_001670D0 - 0x10, radar_rotation_copy, 0.19634955f, 0.19634955f, 255.0f,
                                 P(moby_vars, 0x88), excluded_target_moby);
    if ((selected_target_moby != P(moby_vars, 0x88) && W(moby_vars, 0x8C) >= 0x1F5) || FLAG == 0) {
        W(moby_vars, 0xEC) = 0;
        W(moby_vars, 0x124) = -1;
        W(moby_vars, 0x88) = 0;
    }
    if (selected_target_moby != 0 && (P(moby_vars, 0x88) == 0 || (H(P(moby_vars, 0x88), 0xA6) != 0x3EB && H(selected_target_moby, 0xA6) == 0x3EB)) &&
        FLAG != 0) {
        P(moby_vars, 0x88) = selected_target_moby;
        W(moby_vars, 0x124) = H(selected_target_moby, 0xA6);
        W(moby_vars, 0x8C) = W(moby_vars, 0x138);
    }
    if (selected_target_moby != P(moby_vars, 0x88)) {
        W(moby_vars, 0x118) = W(moby_vars, 0x118) + 1;
        if (scale_game_frames(0x3C) < W(moby_vars, 0x118)) {
            W(moby_vars, 0xEC) = 0;
            W(moby_vars, 0x124) = -1;
            W(moby_vars, 0x88) = 0;
        }
    } else {
        W(moby_vars, 0x118) = 0;
    }
    current_target_moby = P(moby_vars, 0x88);
    if (current_target_moby != 0 && W(moby_vars, 0x84) == 0) {
        if (H(current_target_moby, 0xA6) != W(moby_vars, 0x124) || B(current_target_moby, 0x20) == 0xFE || B(current_target_moby, 0x20) == 0xFD) {
            W(moby_vars, 0x88) = 0;
            W(moby_vars, 0x124) = -1;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BAA68). */

/* Draws the Gemlik turret HUD: target marker, ammo icons, gauge sprites and warning, refreshes the gauge colour table, then the two frame halves. */

extern char *msg_string(s32) __asm__("func_001FDD10");
extern char D_L13_001CBC60[];
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int D_L13_001CBE70[];
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 font_print_center_large(s32, s32, u64, s32, s32) __asm__("func_001F6C20");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s32 get_effect_texture_c(s32) __asm__("func_001F44B8");
int *hp_bar_clut[1] __asm__("D_L13_001613C0") = {0};
extern unsigned char D_0013E500[];
extern unsigned char D_0013F350[];
extern void append_scaled_screen_triangle_strip(float, float, float, void *, int, int, long) __asm__("FUN_L11_00310ad0");
extern void project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
extern void append_rotated_sprite_quad(float, float, float, float, float, int, int, int, int, int, int, int, float, float);
extern void draw_textured_quad_local(int, int, int, int, int, int, int, int, long, long) __asm__("FUN_001f5450");
void append_four_quarter_turn_effect_quads(unsigned char a, unsigned char b, unsigned char c, unsigned char d, float x, float y, float s, float ang) __asm__("FUN_L13_002b8320");
void update_fighter_jet_target_tracking(int a, char *p) __asm__("FUN_L13_002b95b8");
extern long func_001F4868_l(int) __asm__("FUN_001f44b8");
extern void func_L13_002B9590_f(float, float, float, float, unsigned char, unsigned char, unsigned char, unsigned char) __asm__("FUN_L13_002b8320");
extern void *func_001FE540_id(int) __asm__("FUN_001fdd10");
extern int font_print_center_small(int, int, long, void *, int) __asm__("FUN_001f6b88");

void draw_fighter_jet_hud(char *moby) __asm__("FUN_L13_002b97f8");

void draw_fighter_jet_hud(char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    char *screen_dimensions;
    unsigned char *base;
    unsigned char *player_ammo_state;
    unsigned char *player_health_state;
    unsigned char *health_warning_player_state;
    int target_screen_x;
    int target_screen_y;
    int aim_reticle_texture;
    int i;
    unsigned char ammo_icon_count;
    unsigned char target_marker_alpha;
    int j;
    int x;
    int y;
    int ex;
    int ey;
    int target_marker_green;
    int health_palette_fill_end;
    int lo;
    int hi;
    int health_palette_index;
    float marker_transition_fraction;
    float k;
    float marker_opacity_factor;
    float hud_health_step;
    float hud_health_step_magnitude;

    update_fighter_jet_target_tracking((int)moby, moby_vars);
    if (*(int *)(moby_vars + 0x88) != 0) {
        project_world_point_to_screen_pixels(*(char **)(moby_vars + 0x88) + 0x10, &target_screen_x, &target_screen_y, 0);
        if (*(int *)(moby_vars + 0x8C) > scale_game_frames_alt(0x1F4)) {
            marker_transition_fraction = convert_integer_to_float(*(int *)(moby_vars + 0x8C) - scale_game_frames_alt(0x1F4));
            marker_transition_fraction = marker_transition_fraction / convert_integer_to_float(scale_game_frames_alt(0x4B));
            marker_opacity_factor = 2.0f * (1.0f - marker_transition_fraction);
            k = marker_transition_fraction * 5.0f + 1.0f;
            if (marker_opacity_factor > 1.0f) marker_opacity_factor = 1.0f;
            target_marker_alpha = func_001FA898_r(marker_opacity_factor * 96.0f);
            func_L13_002B9590_f(convert_integer_to_float(target_screen_x), convert_integer_to_float(target_screen_y), k, normalize_angle(convert_integer_to_float(frame_count) / 30.0f), 0, 0xFF, 0, target_marker_alpha);
            *(int *)(moby_vars + 0xEC) = 0;
        } else {
            target_marker_green = 0xFF;
            if ((*(int *)(moby_vars + 0x8C) / scale_game_frames_alt(0x14)) & 1) target_marker_green = 0;
            func_L13_002B9590_f(convert_integer_to_float(target_screen_x), convert_integer_to_float(target_screen_y), 1.0f, 0.0f, 0xFF, target_marker_green, 0, 0x60);
            *(int *)(moby_vars + 0xEC) = *(int *)(moby_vars + 0x88);
        }
    }
    y = 0x40;
    x = 0x18;
    base = D_0013F350;
    ammo_icon_count = base[0x15F7];
    for (i = 0; i < ammo_icon_count; i++) {
        player_ammo_state = D_0013F350;
        if (i < player_ammo_state[0x15F6]) {
            append_scaled_screen_triangle_strip(x, y, 0.5f, D_L13_001CBC60, 0x19, 0xFFFFF3, 0x50008F00);
        } else {
            append_scaled_screen_triangle_strip(x, y, 0.5f, D_L13_001CBC60, 0x19, 0xFFFFF3, 0x20004F00);
        }
        ammo_icon_count = player_ammo_state[0x15F7];
        if (i == ammo_icon_count >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }
    ex = *(int *)(moby_vars + 0xE0);
    ey = *(int *)(moby_vars + 0xE4);
    aim_reticle_texture = get_effect_texture_c(0x11);
    append_rotated_sprite_quad(ex, ey, 40.0f, 40.0f, 0.0f, 0x3F, 0x3F, aim_reticle_texture, 0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    append_rotated_sprite_quad(ex, ey, 40.0f, 40.0f, 0.0f, 0x3F, 0x3F, get_effect_texture_c(0x12), 0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    append_rotated_sprite_quad(ex, ey, 10.0f, 10.0f, 0.0f, 0x1F, 0x1F, get_effect_texture_c(8), 0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    health_warning_player_state = D_0013F350;
    if (*(float *)(health_warning_player_state + 0x15FC) < *(float *)(health_warning_player_state + 0x1600) / 10.0f && (frame_count / 90) & 1) {
        font_print_center_small(0x100, 0x186, 0x80000080L, func_001FE540_id(0x5269), 0x64);
    }
    player_health_state = D_0013F350;
    hud_health_step = *(float *)(player_health_state + 0x15FC) - *(float *)(moby_vars + 0xF0);
    hud_health_step *= 0.2f;
    hud_health_step_magnitude = absolute_float(hud_health_step);
    if (hud_health_step_magnitude > 1.0f) hud_health_step /= hud_health_step_magnitude;
    *(float *)(moby_vars + 0xF0) += hud_health_step;
    health_palette_fill_end = func_001FA898_r(*(float *)(moby_vars + 0xF0) * 251.0f / *(float *)(player_health_state + 0x1600)) + 2;
    if (health_palette_fill_end > 0xFD) health_palette_fill_end = 0xFD;
    if (health_palette_fill_end < 2) health_palette_fill_end = 2;
    lo = *(int *)(moby_vars + 0xF4) <= health_palette_fill_end ? *(int *)(moby_vars + 0xF4) : health_palette_fill_end;
    hi = *(int *)(moby_vars + 0xF4) < health_palette_fill_end ? health_palette_fill_end : *(int *)(moby_vars + 0xF4);
    for (j = lo; j <= hi; j++) {
        health_palette_index = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < health_palette_fill_end) {
            hp_bar_clut[0][health_palette_index] = D_L13_001CBE70[health_palette_index];
        } else {
            hp_bar_clut[0][health_palette_index] = 0x80000000;
        }
    }
    screen_dimensions = (char *)D_0013E500;
    draw_textured_quad_local(0x170, *(int *)(screen_dimensions + 4) - 0x90, 0x80, 0x80, 0, 0, 0x80, 0x80, 0x70808080, func_001F4868_l(*(int *)(moby_vars + 0x104) + 0x28));
    draw_textured_quad_local(0x170, *(int *)(screen_dimensions + 4) - 0x90, 0x80, 0x80, 0, 0, 0x80, 0x80, 0x70808080, func_001F4868_l(*(int *)(moby_vars + 0x104) + 0x29));
    *(int *)(moby_vars + 0xF4) = health_palette_fill_end;
}
/* Builds a rotated offset vector from the moby, flips a facing flag on a pad press, and updates the aim. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BB068), where it is exact; names translated to the US level program. */

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int spawn_class_3f1_moby(void *, void *, void *, float, float) __asm__("FUN_L11_00308848");
extern unsigned char D_0013A4E0[];
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");

void update_fighter_jet_gun_and_aim(struct Moby *moby, char *flight_data, float azimuth_angle, float elevation_angle) __asm__("FUN_L13_002b9df8");

void update_fighter_jet_gun_and_aim(struct Moby *moby, char *flight_data, float azimuth_angle, float elevation_angle) {
    char aim_vector[16];
    char projectile_spawn_position[16];
    char muzzle_offset[16];
    char aim_world_position[16];
    build_spherical_offset(aim_vector, frame_time * 400.0f, azimuth_angle, elevation_angle);
    fast_vec_add(aim_vector, aim_vector, flight_data);
    if (fast_dec_timer(flight_data + 0x80) && (*(int *)(D_0013A4E0 + 0x2610) & 0x84)) {
        allocate_voice_for_target_entry(3, 0, (int)moby);
        fast_vec_normalize(muzzle_offset, aim_vector, 2.2f);
        moby_point_position(moby, 3 + (flight_data[0x61] & 1), projectile_spawn_position);
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, muzzle_offset);
        spawn_class_3f1_moby(moby, aim_vector, projectile_spawn_position, 200.0f, -1.0f);
        *(int *)(flight_data + 0x80) = scale_game_frames(4);
        flight_data[0x61] ^= 1;
    }
    fast_vec_normalize(aim_vector, aim_vector, 23.0f);
    fast_vec_add(aim_world_position, aim_vector, &moby->pos);
    project_world_point_to_screen_pixels(aim_world_position, flight_data + 0xE0, flight_data + 0xE4, 0);
}
/* Fires a timed burst from a moby while a pad button is held and ammo remains. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BB1C8), where it is exact; names translated to the US level program. */

extern int spawn_ship_rocket(void *, void *, float, int, void *, int) __asm__("FUN_L13_002e6a58");

void fire_fighter_jet_rocket(struct Moby *moby, char *flight_data) __asm__("FUN_L13_002b9f58");

void fire_fighter_jet_rocket(struct Moby *moby, char *flight_data) {
    if (fast_dec_timer(flight_data + 0x84) && (*(int *)(D_0013A4E0 + 0x2610) & 0x28)) {
        unsigned char *player_ammo_state = D_0013E533 + 0xE1D;
        if (player_ammo_state[0x15F6] != 0) {
            char projectile_spawn_position[16];
            char projectile_spawn_offset[16];
            float projectile_scale;
            int projectile_frame_count;
            char *projectile_initial_rotation = &moby->rot;
            short t = (*(unsigned short *)(flight_data + 0x6A) + 1) & 3;
            *(short *)(flight_data + 0x6A) = t;
            moby_point_position(moby, t + 7, projectile_spawn_position);
            fast_vec_normalize(projectile_spawn_offset, flight_data, 0.3f);
            fast_vec_add(projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
            projectile_scale = frame_time * 100.0f;
            projectile_frame_count = scale_game_frames(300);
            if (spawn_ship_rocket(moby, projectile_spawn_position, projectile_scale, *(int *)(flight_data + 0xEC), projectile_initial_rotation, projectile_frame_count) != 0) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                player_ammo_state[0x15F6]--;
                *(int *)(flight_data + 0x84) = scale_game_frames(30);
            }
        }
    }
}
#include "eetypes.h"
extern float D_L13_0016CDF0 __attribute__((section(".data")));
extern void jet_reflect_vector(void *,void *,void *) __asm__("FUN_L00_001ff660");
extern void jet_explosion(void *, void *, void *, float, float, int, int, int,
                          float, float, float, int, float, float, int, int, int, int)
    __asm__("FUN_L00_0025e450");
/* Flies the player's fighter: throttle and stick input, steering toward the path, camera, and collision response. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BB3E8), where it is exact; names translated to the US level program. */
typedef float FlightVec[4] __attribute__((aligned(16)));
typedef struct {
    char pad00[0x10];
    FlightVec pos;
    unsigned char state;
    char pad21[0x13];
    unsigned short flags;
    char pad36[0xA];
    float rot_x;
    float rot_y;
    float rot_z;
    char pad4C[0x70];
    unsigned char unkBC;
} FlightMoby;
typedef struct {
    char pad00[0x10];
    FlightVec direction;
    float camera_roll, camera_pitch, camera_yaw, pad2C;
    float roll, pitch, yaw, pad3C;
    char pad40[0x20];
    unsigned char camera_mode;
    unsigned char gun_side;
    short death_timer;
    float speed;
    short launch_frames;
    short gun_index;
    float steer_x;
    float steer_y;
    char pad74[0x1C];
    float steer_x_velocity, steer_y_velocity, roll_velocity, pad9C;
    float camera_roll_velocity, camera_pitch_velocity, camera_yaw_velocity, padAC;
    float fov, camera_offset_x, camera_offset_y, camera_offset_z;
    float fov_velocity, speed_fraction, camera_offset_x_velocity, camera_offset_y_velocity;
    float camera_offset_z_velocity, padD4, padD8;
    int hit_cooldown;
    char padE0[0x2C];
    int boost_sound;
    int target_path;
} FlightData;
extern char *D_L13_001600EC_gp __asm__("D_L13_001600EC") __attribute__((section(".sdata")));
extern f32 frame_scale_gp __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern f32 frame_scale_sq_gp __asm__("D_0015ED64") __attribute__((section(".sdata")));
extern f32 frame_time_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern u64 D_L13_00161430 __attribute__((sda));
extern char *D_L13_00167100 __attribute__((section(".data")));
extern char D_L13_00174140[];
extern char D_L13_001F4450[];
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern float spring_angle(char *, char *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");
extern float interpolate_value_with_repeated_squaring(int, int, float, float, float, float, float) __asm__("FUN_L13_002ba088");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void update_view_context(void) __asm__("FUN_001f2d98");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");

void update_fighter_jet_flight(void *moby_arg, void *data_arg) __asm__("FUN_L13_002ba178");

void update_fighter_jet_flight(void *moby_arg, void *data_arg) {
    FlightMoby *moby = moby_arg;
    FlightData *data = data_arg;
    FlightVec old_pos;
    FlightVec impulse;
    FlightVec velocity;
    FlightVec cross;
    FlightVec vertical;
    FlightVec camera_angles;
    FlightVec target;
    FlightVec offset;
    float steer_x;
    float steer_y;
    float steer_rate_x;
    float steer_rate_y;
    float pitch_limit;
    float yaw_limit;
    float distance;
    float height, height_again;
    float half_fov, tan_half_fov;
    float speed_fraction;
    char *pad_state;
    char *player;
    char *voice;
    char *hit;
    int voice_index;
    int hit_class;

    *(u128 *)velocity = 0;
    *(u128 *)vertical = 0;
    vertical[2] = 1.0f;
    velocity[2] = frame_time_gp * 8.0f;
    moby->flags &= 0xFEFF;
    *(u128 *)impulse = *(u128 *)velocity;
    if (moby->unkBC == 0) {
        if (data->launch_frames > scale_game_frames(120)) {
            pad_state = (char *)D_0013A4E0 + 0x2460;
            if (*(int *)(pad_state + 0x1B4) & 0x40) {
                voice_index = data->boost_sound;
                if (voice_index != -1) {
                    voice = (char *)D_0013E533 + 0x1D + voice_index * 0x70;
                    if (*(char **)(voice + 0x88) == (char *)moby && *(unsigned char *)(voice + 0x74) != 0) {
                        release_voice_slot(voice_index);
                    }
                }
                data->boost_sound = -1;
                data->boost_sound = allocate_voice_for_target_entry(1, 4, (int)moby);
            } else if (*(int *)(pad_state + 0x1B8) & 0x40) {
                voice_index = data->boost_sound;
                if (voice_index != -1) {
                    voice = (char *)D_0013E533 + 0x1D + voice_index * 0x70;
                    if (*(char **)(voice + 0x88) == (char *)moby && *(unsigned char *)(voice + 0x74) != 0) {
                        release_voice_slot(voice_index);
                    }
                }
                data->boost_sound = -1;
            }
            pad_state = (char *)D_0013A4E0 + 0x2460;
            if (*(int *)(pad_state + 0x1B0) & 0x40) {
                float boosted = data->speed + frame_scale_gp * 0.035f;
                float max_speed = frame_time_gp * 30.0f;
                data->speed = boosted;
                if (max_speed < boosted) {
                    data->speed = max_speed;
                }
            } else {
                float cruise_speed = frame_time_gp * 24.0f;
                float step = frame_scale_gp * 0.025f;
                float speed = data->speed;
                if (speed < cruise_speed - step) {
                    data->speed = speed + step;
                } else if (cruise_speed < speed) {
                    data->speed = speed + (cruise_speed - speed) * (frame_scale_gp * 0.1f);
                } else {
                    data->speed = cruise_speed;
                }
            }
        } else {
            data->launch_frames += 1;
            data->speed += frame_scale_gp * 0.003f;
        }
    } else {
        data->speed = 0.0001f;
    }

    if (data->launch_frames > scale_game_frames(5)) {
        char *player = (char *)D_0013E533 + 0xE1D;
        steer_rate_x = frame_scale_gp * 0.1f;
        steer_rate_y = frame_scale_gp * 0.25f;
        steer_x = *(float *)(player + 0x1D20);
        steer_y = *(float *)(player + 0x1D24);
        if (*(int *)(D_0013A4E0 + 0x2600) & 0x200) {
            steer_x = steer_x / (frame_scale_gp * 0.012271847f);
            steer_x = steer_x * (frame_scale_gp * 0.015707964f);
            steer_y = steer_y / (frame_scale_gp * 0.012271847f);
            steer_y = steer_y * (frame_scale_gp * 0.015707964f);
            steer_rate_x = frame_scale_gp * 0.08f;
            steer_rate_y = frame_scale_gp * 0.08f;
        }
        vendor_spring(&data->steer_x, &data->steer_x_velocity, steer_x, frame_scale_sq_gp * 0.03f, frame_scale_sq_gp, steer_rate_x);
        vendor_spring(&data->steer_y, &data->steer_y_velocity, steer_y, frame_scale_sq_gp * 0.03f, frame_scale_sq_gp, steer_rate_y);
        spring_angle((char *)&data->roll, (char *)&data->roll_velocity, 0, data->steer_x, frame_scale_sq_gp * 0.05f, frame_scale_sq_gp, frame_scale_gp);
        data->pitch = fast_subtract_rotations(data->pitch, data->steer_y * (frame_scale_gp * 0.012271847f));
        data->yaw = fast_subtract_rotations(data->yaw, data->steer_x * (frame_scale_gp * 0.012271847f));
        if (data->pitch > 1.3962634f) {
            data->pitch = 1.3962634f;
        } else if (data->pitch < -1.3962634f) {
            data->pitch = -1.3962634f;
        }
        if (moby->rot_y > 1.3962634f) {
            moby->rot_y = 1.3962634f;
        } else if (moby->rot_y < -1.3962634f) {
            moby->rot_y = -1.3962634f;
        }
    }

    pitch_limit = 310.0f;
    yaw_limit = 500.0f;
    if (data->target_path != -1) {
        int path_offset = data->target_path << 7;
        char *paths = D_L13_001600EC_gp;
        qcopy(target, ((char (*)[128])paths)[data->target_path] + 0x30);
        pitch_limit = fast_vec_length(paths + path_offset + 0x20);
        add_vector_xyz(offset, (data->target_path << 7) + D_L13_001600EC_gp, (data->target_path << 7) + D_L13_001600EC_gp + 0x10);
        yaw_limit = fast_vec_length(offset);
    } else {
        qcopy(target, &D_L13_00161430);
    }

    distance = vector_distance(moby->pos, &D_L13_00161430);
    data->yaw = interpolate_value_with_repeated_squaring(1, 5, distance, data->yaw, fast_arc_tan(target[0] - moby->pos[0], target[1] - moby->pos[1]), 50.0f, yaw_limit);
    moby->rot_z = interpolate_value_with_repeated_squaring(1, 5, distance, moby->rot_z, fast_arc_tan(target[0] - moby->pos[0], target[1] - moby->pos[1]), 50.0f, yaw_limit);
    height = absolute_float(target[2] - moby->pos[2]);
    data->pitch = interpolate_value_with_repeated_squaring(1, 5, height, data->pitch, -fast_arc_tan(vector_distance(moby->pos, target), target[2] - moby->pos[2]), 50.0f, pitch_limit);
    height_again = absolute_float(target[2] - moby->pos[2]);
    moby->rot_y = interpolate_value_with_repeated_squaring(1, 5, height_again, moby->rot_y, -fast_arc_tan(vector_distance(moby->pos, target), target[2] - moby->pos[2]), 50.0f, pitch_limit);
    build_spherical_offset(velocity, data->speed, data->yaw, -data->pitch);
    fast_vec_normalize(data->direction, velocity, 1.0f);
    qcopy(old_pos, moby->pos);
    if (moby->unkBC == 0) {
        add_vector_xyz(moby->pos, moby->pos, velocity);
    }
    {
        float min_speed = frame_time_gp * 24.0f;
        float excess = data->speed - min_speed;
        float range = frame_time_gp * 30.0f - min_speed;
        if (excess < 0.0f) {
            speed_fraction = 0.0f / range;
        } else {
            speed_fraction = excess / range;
        }
    }
    data->speed_fraction = speed_fraction;
    {
        FlightVec camera_offsets[2];
        /* Fakematch: retail's compiler expands the offset table initializer in place (a clobber,
         * then a quadword clear per row); our cc1 calls memset for it, and this union
         * initializer gives retail's sequence. */
        union {
            u128 q;
            struct {
                FlightVec pos;
                FlightVec rot;
            } v;
        } camera = { 0 };
        float basis[12];
        camera.v.pos[0] = (-15.0f - data->steer_y - speed_fraction * 4.0f) * 0.25f;
        camera.v.pos[1] = data->steer_x * -3.0f * 0.25f;
        camera.v.pos[2] = (4.0f + data->steer_y * 2.0f) * 0.25f;
        *(u128 *)camera.v.rot = 0;
        camera.v.rot[0] = -1.275f;
        camera.v.rot[2] = 0.65f;
        *(u128 *)camera_offsets[0] = *(u128 *)camera.v.pos;
        *(u128 *)camera_offsets[1] = *(u128 *)camera.v.rot;
        if (moby->state == 2) {
            data->camera_offset_x = camera_offsets[data->camera_mode][0];
            data->camera_offset_y = camera_offsets[data->camera_mode][1];
            data->camera_offset_z = camera_offsets[data->camera_mode][2];
        } else {
            vendor_spring(&data->camera_offset_x, &data->camera_offset_x_velocity, camera_offsets[data->camera_mode][0], frame_scale_sq_gp * 0.15f, frame_scale_sq_gp * 0.8f, frame_scale_gp);
            vendor_spring(&data->camera_offset_y, &data->camera_offset_y_velocity, camera_offsets[data->camera_mode][1], frame_scale_sq_gp * 0.15f, frame_scale_sq_gp * 0.8f, frame_scale_gp);
            vendor_spring(&data->camera_offset_z, &data->camera_offset_z_velocity, camera_offsets[data->camera_mode][2], frame_scale_sq_gp * 0.15f, frame_scale_sq_gp * 0.8f, frame_scale_gp);
        }
        offset[0] = data->camera_offset_x;
        offset[1] = data->camera_offset_y;
        offset[2] = data->camera_offset_z;
        offset[3] = 0.0f;
        euler_rotation_basis(basis, (char *)&data->roll);
        transform_vector_by_basis(target, offset, basis);
        add_vector_xyz(camera.v.pos, moby->pos, target);
        FUN_L00_002ea9d8(camera.v.pos);
        camera.v.rot[0] = fast_add_rotations(data->roll * 0.5f, data->steer_x * 0.03125f);
        camera.v.rot[1] = fast_subtract_rotations(fast_add_rotations(data->pitch, 0.09817477f), data->steer_y * (frame_scale_gp * 0.012271847f));
        camera.v.rot[2] = data->yaw;
        if (moby->state == 2) {
            qcopy(&data->camera_roll, camera.v.rot);
        } else {
            spring_angle((char *)&data->camera_roll, (char *)&data->camera_roll_velocity, 0, camera.v.rot[0], frame_scale_sq_gp * 0.15f, frame_scale_sq_gp * 0.8f, frame_scale_gp);
            spring_angle((char *)&data->camera_pitch, (char *)&data->camera_pitch_velocity, 0, camera.v.rot[1], frame_scale_sq_gp * 0.15f, frame_scale_sq_gp * 0.8f, frame_scale_gp);
            spring_angle((char *)&data->camera_yaw, (char *)&data->camera_yaw_velocity, 0, camera.v.rot[2], frame_scale_sq_gp * 0.15f, frame_scale_sq_gp * 0.8f, frame_scale_gp);
        }
        camera_angles[0] = data->camera_roll;
        camera_angles[2] = data->camera_yaw;
        camera_angles[1] = data->camera_pitch;
        FUN_L00_002eaa30(camera_angles);
        if (*(short *)(D_L13_00167100 + 0x86) != 5) {
            stub_printf(D_L13_001F4450);
            jet_mount(camera.v.pos, camera_angles, 0, 0, 0);
        }
    }
    vendor_spring(&data->fov, &data->fov_velocity, 1.134464f + (1.4835298f - 1.134464f) * speed_fraction, 0.05f, 1.0f, 1.0f);
    half_fov = data->fov * 0.5f;
    tan_half_fov = fast_sin(half_fov) / fast_cos(half_fov);
    D_L13_0016CDF0 = tan_half_fov;
    update_view_context();
    moby->rot_x = fast_add_rotations(data->roll, data->steer_x * 0.5f);
    moby->rot_y = fast_add_rotations(data->pitch, -data->steer_y * 0.5f);
    moby->rot_z = fast_add_rotations(data->yaw, -data->steer_x * 0.0f);
    if (coll_sphere(1.2f, moby->pos, 0, moby)) {
        hit = D_L13_00174140;
        if (*(char **)(hit + 0x18) == 0 || (hit_class = *(short *)(*(char **)(hit + 0x18) + 0xA6), hit_class != 0 && hit_class != 0x192 && hit_class != 0x4C2 && hit_class != 0x4C3 && hit_class != 0x52 && hit_class != 0x53)) {
            qcopy(moby->pos, hit + 0x30);
            jet_reflect_vector(velocity, velocity, hit + 0x40);
            data->yaw = fast_add_rotations(data->yaw, fast_subtract_rotations(fast_arc_tan(velocity[0], velocity[1]), data->yaw) * (frame_scale_gp * 0.1f));
            data->pitch = fast_add_rotations(data->pitch, fast_subtract_rotations(-fast_arc_tan(vector_length_xy(velocity), velocity[2]), data->pitch) * (frame_scale_gp * 0.1f));
            data->speed = data->speed * 0.7f;
            if (*(char **)(hit + 0x18) != 0) {
                if (data->hit_cooldown == 0) {
                    player = (char *)D_0013E533 + 0xE1D;
                    *(float *)(player + 0x15FC) = *(float *)(player + 0x15FC) - 5.0f;
                    if (*(float *)(player + 0x15FC) < 0.0f) {
                        subtract_vector_xyz(cross, hit + 0x50, hit + 0x60);
                        fast_vec_normalize(cross, cross, 1.0f);
                        jet_reflect_vector(impulse, velocity, hit + 0x40);
                        fast_vec_normalize(impulse, impulse, frame_time_gp + frame_time_gp);
                        fast_vec_normalize(vertical, hit + 0x40, 1.0f);
                        jet_explosion(moby, impulse, 0, 8.0f, 100.0f, 0x1E, 0xA, 0x18, 16.0f, 8.0f, 9.0f, 6, 1.0f, 50.0f, 1, 1, -1, 0);
                        data->death_timer = scale_game_frames(240);
                        moby->state = 6;
                    }
                    data->hit_cooldown = scale_game_frames(10);
                }
            } else {
                *(float *)(D_0013E533 + 0x2419) = 0.0f;
                jet_explosion(moby, impulse, 0, 8.0f, 100.0f, 0x1E, 0xA, 0x18, 16.0f, 8.0f, 9.0f, 6, 1.0f, 50.0f, 1, 1, -1, 0);
                moby->flags |= 0x41;
                data->death_timer = scale_game_frames(240);
                moby->state = 6;
            }
        }
    }
    enqueue_callback_list_1(draw_fighter_jet_hud, moby);
    fire_fighter_jet_rocket((struct Moby *)moby, (char *)data);
    update_fighter_jet_gun_and_aim((struct Moby *)moby, (char *)data, fast_add_rotations(data->camera_yaw, -data->steer_x / 7.0f),
                     -fast_add_rotations(data->camera_pitch, -(data->steer_y / 7.0f) - 0.1f));
}

/* Level 13 fighter state. Offsets come from the checked level 13 retail instructions. */
typedef float JetVec[4] __attribute__((aligned(16)));
typedef struct Jet Jet;
typedef struct JetData JetData;
typedef struct { char pad0[0x24]; float scale; } JetModel;
struct Jet {
    char pad0[0x10]; JetVec position;
    unsigned char state,pad21[3]; JetModel *model; float pad28,scale;
    unsigned char alpha,pad31; unsigned short render_flags,flags; char pad36[0xA];
    JetVec rotation; char pad50[0x28]; JetData *data;
    char pad7C[0x18]; int animation; char pad98[0xC];
    unsigned char status,padA5; unsigned short class_id; char padA8[8]; unsigned char persistent;
};
struct JetData {
    JetVec velocity,pad10,previous_rotation,angles,spawn_position,spawn_rotation;
    unsigned char animation_frame,gun_side; short death_timer; float speed;
    short missile_index,gun_index; int counter6C,counter70,pad74,pad78; float blend;
    int gun_timer,missile_timer; char pad88[0x18];
    float angle_a,angle_b,angle_c,limit_a,limit_b; char padB4[0xC]; int counterC0,padC4;
    float angle_d,angle_e,angle_f; char padD4[8]; int timer;
    int hud_x,hud_y,energy,padEC; float hud_health; int hud_previous,wait_timer;
    int destination_path,escape_path,texture_id,engine_sound,weapon_sound;
    char pad110[0xC]; int group_a,hit_class,target_class,lock_index,exit_path,entry_path,trigger,start_timer,group_b;
};
typedef struct {
    char pad0[0x15F0]; Jet *craft; unsigned short class_id; unsigned char ammo,ammo2,ammo3,ammo4,ammo5,ammo6;
    float health,max_health,health_scale; int health_meter; short weapon; unsigned char pad160E,pad160F;
    char pad1610[0xA70]; Jet *moby; int state; char pad2088[0x1C]; unsigned char busy,in_craft;
} JetPlayer;
typedef struct { char pad0[0x1C]; int active; char pad20[0x30]; int pending; } JetTransition;
typedef struct { char pad0[0x160]; float flash; int pad164,ticks; } JetCamera;
typedef struct { char pad0[0xA]; unsigned short palette; char padC[4]; } JetTexture;
typedef struct { char pad0[0x30]; JetVec position; char pad40[0x30]; JetVec rotation; } JetPath;
typedef struct { char pad0[0x10]; JetVec velocity; Jet *source; int pad24,pad28; float damage; int flags; } JetHit;
extern JetPlayer jet_player47 __asm__("D_0013F350");
extern JetCamera jet_camera47 __asm__("D_L13_00166F80");
extern JetTransition D_001516D0[];
#include "rnc/gameplay/state/game_globals.h"
extern unsigned char D_0013E550[];
extern unsigned char D_0013A4E0[];
#include "rnc/overlay/l13/gameplay/entities/elevator_data/fighter_jet_update.h"
extern JetTexture D_L13_0016D480[];
extern unsigned int D_L13_0015F420;
int hp_bar_saved __asm__("D_L13_001613C4") = 0;
extern char D_L13_001CBDE8[];
short D_L13_00161440 = 0x7f;
extern float D_L13_0015F3FC;
extern int D_L13_0015F608;
extern JetPath *D_L13_001600EC;
extern char D_L13_001F4470[], D_L13_001F44A8[];
extern int jet_fast_dec_timer(void *) __asm__("FUN_001f9740");
extern JetHit *jet_find_hit(void *,int,int) __asm__("FUN_L00_0025a420");
extern void build_spherical_offset(float *,float,float,float) __asm__("FUN_00214db0");
extern void jet_scale_vector(void *,void *,float) __asm__("FUN_001f9c90");
extern void jet_add_vector(void *,void *,void *) __asm__("FUN_001f9a10");
extern float jet_angle(float,float) __asm__("FUN_001f9e90");
extern float jet_sub_rot(float,float) __asm__("FUN_001fa5c8");
extern float jet_add_rot(float,float) __asm__("FUN_001fa580");
extern float jet_length_xy(void *) __asm__("FUN_001f9b20");
extern int jet_scale_ticks(int) __asm__("FUN_001f96f8");
extern void jet_group_state(int,int,int,int) __asm__("FUN_L13_002650a8");
extern void jet_group_switch(void *,int,int,int) __asm__("FUN_L13_002651c8");
extern void jet_group_flag(int,int) __asm__("FUN_L13_00265170");
extern float jet_distance(void *,void *) __asm__("FUN_L00_00250430");
extern float jet_blend(float *,float,float) __asm__("FUN_00213ed8");
extern void jet_animate(void *,float) __asm__("FUN_L00_00259fe8");
extern void jet_place_camera(void *,void *,int,int) __asm__("FUN_L00_00216f90");
extern void jet_mount(void *,void *,int,int,int) __asm__("FUN_L00_002eaaa0");
extern void update_fighter_jet_flight(void *,void *) __asm__("FUN_L13_002ba178");
extern void jet_camera_mode(void) __asm__("FUN_L00_00203b18");
extern void jet_game_state(int,int) __asm__("FUN_L01_0027a248");
extern void jet_set_hero_state(int,int) __asm__("FUN_L00_002223f8");
extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");
extern void jet_set_weapon(int,int) __asm__("FUN_001ff570");
extern void jet_update_moby(void *) __asm__("FUN_0020e098");
extern void jet_stop_sound(int) __asm__("FUN_0022d798");
extern void jet_death_complete(void) __asm__("FUN_L00_00211250");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *,void *) __asm__("FUN_L00_00284e50");
extern void jet_force_help(int,int) __asm__("FUN_002151d8");
extern int jet_try_help(int,int) __asm__("FUN_00215130");
extern void jet_delete_moby(void *) __asm__("FUN_0020c828");
extern int jet_allocate_voice(int,int,int) __asm__("FUN_0022da68");
extern int jet_check_trigger(void *,int) __asm__("FUN_00214720");
extern int jet_fast_dec_death_timer(void *) __asm__("FUN_001f9770");

void fighter_jet_update(Jet *m) __asm__("FUN_L13_002bb068");
void fighter_jet_update(Jet *jet_moby) {
    enum {
        FIGHTER_JET_STATE_INITIALIZE = 0,
        FIGHTER_JET_STATE_WAIT_FOR_ENTRY = 1,
        FIGHTER_JET_STATE_MOUNT_PLAYER = 2,
        FIGHTER_JET_STATE_FLIGHT = 4,
        FIGHTER_JET_STATE_EXIT_PLAYER = 5,
        FIGHTER_JET_STATE_STOP_DEATH_SOUNDS = 6,
        FIGHTER_JET_STATE_WAIT_FOR_DEATH_RESET = 7,
        FIGHTER_JET_STATE_FORCED_EXIT = 8,
        FIGHTER_JET_STATE_DELETE = 9
    };
    JetData *jet_data;
    JetVec impulse;
    int state,initial_state;
    if (((JetPlayer*)(&jet_player47))->state==0x72) return;
    jet_data=jet_moby->data; jet_data->hit_class=0;
    jet_fast_dec_timer((int*)&jet_data->timer);
    initial_state=jet_moby->state;
    if ((unsigned int)(initial_state-6)>=2 && (unsigned char)initial_state!=1 && (unsigned char)initial_state!=2) {
        JetHit *hit=(JetHit*)jet_find_hit(jet_moby,0x30001,0);
        if (hit && jet_moby->state==4) {
            if (((JetPlayer*)(&jet_player47))->health>hit->damage) {
                ((JetPlayer*)(&jet_player47))->health-=hit->damage;
                if (hit->source && (unsigned int)(hit->source->class_id-0x52)<2) jet_data->hit_class=(short)hit->source->class_id;
                if (hit->flags&1) {
                    float weight,yaw_delta,pitch_delta;
                    jet_camera47.flash=1.0f; jet_camera47.ticks=jet_scale_ticks(4);
                    weight=hit->damage*0.05f;
                    build_spherical_offset(impulse,jet_data->speed,jet_data->angles[2],-jet_data->angles[1]);
                    jet_scale_vector(hit->velocity,hit->velocity,frame_time*3.0f);
                    jet_add_vector(jet_moby->position,jet_moby->position,hit->velocity);
                    jet_reflect_vector(impulse,impulse,hit->velocity);
                    yaw_delta=jet_sub_rot(jet_angle(impulse[0],impulse[1]),jet_data->angles[2]);
                    jet_data->angles[2]=jet_add_rot(jet_data->angles[2],yaw_delta*(fighter_jet_frame_scale*0.1f)*weight);
                    pitch_delta=jet_sub_rot(jet_angle(jet_length_xy(impulse),impulse[2]),jet_data->angles[1]);
                    jet_data->angles[1]=jet_add_rot(jet_data->angles[1],pitch_delta*(fighter_jet_frame_scale*0.1f)*weight);
                }
            } else {
                build_spherical_offset(impulse,jet_data->speed,jet_data->angles[2],-jet_data->angles[1]);
                if (hit->source && (unsigned int)(hit->source->class_id-0x52)<2) jet_data->hit_class=(short)hit->source->class_id+10000;
                jet_camera47.flash=0.2f; jet_camera47.ticks=jet_scale_ticks(5);
                ((JetPlayer*)(&jet_player47))->health=0.0f; jet_moby->flags|=0x41;
                jet_explosion(jet_moby,impulse,0,0.0f,0.0f,10,3,16,4.0f,2.0f,9.0f,6,1.0f,15.0f,1,1,-1,0);
                jet_moby->animation=0; jet_data->death_timer=jet_scale_ticks(240); jet_moby->state=6;
            }
        }
    }
    jet_moby->status=0xFF;
    if (((JetPlayer*)(&jet_player47))->craft==jet_moby) ((JetPlayer*)(&jet_player47))->health_meter=(int)(((JetPlayer*)(&jet_player47))->health*0.00390625f*200.0f);
    switch(jet_moby->state) {
    case FIGHTER_JET_STATE_INITIALIZE: {
        float limit;
        int *palette;
        JetTexture *texture_table;
        unsigned int palette_data;
        int texture_index,initialized;
        jet_moby->state=1; jet_data->animation_frame=0; jet_moby->scale=jet_moby->model->scale;
        *(float*)((char*)jet_data+0x20)=jet_data->angles[0]; *(float*)((char*)jet_data+0x24)=jet_data->angles[1];
        limit=jet_angle(1.0f,D_L13_0016CDF0); limit+=limit;
        jet_data->counterC0=0; jet_data->hit_class=0; *(float*)((char*)jet_data+0xAC)=limit; *(float*)((char*)jet_data+0xB0)=limit;
        qcopy(jet_data->angles,jet_moby->rotation); qcopy(jet_data->spawn_rotation,jet_moby->rotation); qcopy(jet_data->spawn_position,jet_moby->position);
        texture_index=jet_data->texture_id;
        texture_table=D_L13_0016D480;
        jet_data->blend=0.0f;
        texture_index+=40;
        palette_data=D_L13_0015F420;
        initialized=hp_bar_saved;
        palette=(int*)(palette_data+(texture_table[texture_index].palette<<4));
        hp_bar_clut[0]=palette;
        if (!initialized) __builtin_memcpy(D_L13_001CBE70,palette,1024);
        hp_bar_saved=1; jet_data->wait_timer=1; jet_data->energy=1;
        if (jet_moby->persistent!=0xFF && (visited52)[jet_moby->persistent+(persistence_table_row_index<<4)]==0xFF) jet_moby->state=9;
        if (jet_data->group_a>0) jet_group_state(jet_data->group_a,0,0,0);
        if (jet_data->group_b>0) jet_group_state(jet_data->group_b,0,0,0);
        jet_group_switch(D_L13_001CBDE8,1,1,-1); jet_group_switch(&D_L13_00161440,1,-1,-1); jet_group_flag(0x102,0);
        jet_data->lock_index=-1; jet_data->start_timer=jet_scale_ticks(575);
        break;
    }
    case FIGHTER_JET_STATE_WAIT_FOR_ENTRY: {
        short enter=0;
        if (jet_moby->persistent!=0xFF && (visited52)[jet_moby->persistent+(persistence_table_row_index<<4)]==0xFF) jet_moby->state=9;
        else if (jet_data->trigger!=-1 && jet_check_trigger(D_0013F3D0,jet_data->trigger)) { jet_data->blend=0.99f; D_L13_0015F3FC=0.99f; enter=1; }
        else if (jet_distance(jet_moby,((JetPlayer*)(&jet_player47))->moby)<2.0f) {
            int pressed=jet_try_help(7,0x53E4)!=0;
            if (*(int*)(D_0013A4E0+0x2604)&0x10) enter=pressed;
        }
        if (enter) {
            float limit;
            jet_moby->scale=jet_moby->model->scale;
            *(float*)((char*)jet_data+0x20)=jet_data->angles[0]; *(float*)((char*)jet_data+0x24)=jet_data->angles[1];
            limit=jet_angle(1.0f,D_L13_0016CDF0); limit+=limit;
            jet_data->counterC0=0; jet_data->hit_class=0; *(float*)((char*)jet_data+0xAC)=limit; *(float*)((char*)jet_data+0xB0)=limit;
            qcopy(jet_data->angles,jet_moby->rotation); jet_data->wait_timer=1; jet_data->energy=1; jet_moby->state=2; jet_data->lock_index=-1;
        } else if (jet_data->blend!=0.0f) { jet_blend(&jet_data->blend,0.0f,frame_time*4.0f); D_L13_0015F3FC=jet_data->blend; }
        jet_animate((unsigned char*)jet_moby,1.5f);
        break;
    }
    case FIGHTER_JET_STATE_MOUNT_PLAYER:
        jet_blend(&jet_data->blend,1.0f,frame_time*4.0f); D_L13_0015F3FC=jet_data->blend;
        if (jet_data->blend==1.0f) {
            if (jet_data->entry_path!=-1) {
                JetPath *path=&D_L13_001600EC[jet_data->entry_path];
                jet_place_camera(path->position,path->rotation,50,1);
                {
                    int index=jet_data->entry_path;
                    JetPath *paths=D_L13_001600EC;
                    qcopy(jet_moby->position,paths[index].position); qcopy(jet_moby->rotation,paths[index].rotation);
                }
            }
            jet_set_hero_state(0x32,1);
            jet_mount(jet_moby->position,jet_moby->rotation,0,0,0);
            jet_data->missile_index=0; jet_data->counter6C=0; jet_data->counter70=0; jet_data->missile_timer=0; jet_data->speed=fighter_jet_frame_scale*0.1f; jet_data->gun_timer=0;
            jet_moby->alpha=jet_moby->render_flags=0xFF; *(int*)((char*)((JetPlayer*)(&jet_player47))->moby+0x98)=-1;
            qcopy(jet_data->previous_rotation,jet_moby->rotation);
            jet_data->angle_a=0; jet_data->angle_b=0; jet_data->angle_c=0; jet_data->angle_d=0; jet_data->angle_e=0; jet_data->angle_f=0; ((JetPlayer*)(&jet_player47))->craft=jet_moby; { unsigned short cls=jet_moby->class_id; ((JetPlayer*)(&jet_player47))->health=255.0f; ((JetPlayer*)(&jet_player47))->ammo=10; ((JetPlayer*)(&jet_player47))->pad160E=1; ((JetPlayer*)(&jet_player47))->ammo2=20; ((JetPlayer*)(&jet_player47))->class_id=cls; ((JetPlayer*)(&jet_player47))->ammo3=20; jet_data->hud_previous=255; jet_data->hud_health=0; ((JetPlayer*)(&jet_player47))->ammo4=3; ((JetPlayer*)(&jet_player47))->ammo5=3; ((JetPlayer*)(&jet_player47))->ammo6=3; ((JetPlayer*)(&jet_player47))->pad160F=0; ((JetPlayer*)(&jet_player47))->max_health=256.0f; ((JetPlayer*)(&jet_player47))->health_scale=100.0f; }
            build_spherical_offset(jet_data,fighter_jet_frame_scale*0.1f,jet_data->angles[2],-jet_data->angles[1]);
            if (jet_data->group_a>0) jet_group_state(jet_data->group_a,1,1,1);
            if (jet_data->group_b>0) jet_group_state(jet_data->group_b,1,1,1);
            jet_group_switch(D_L13_001CBDE8,0,0,-1); jet_group_switch(&D_L13_00161440,0,-1,-1); jet_group_flag(0x102,0);
            jet_moby->position[2]+=3.0f; jet_moby->scale=jet_moby->model->scale*0.25f; ((JetPlayer*)(&jet_player47))->in_craft=1;
            if (jet_data->engine_sound==-1) jet_data->engine_sound=jet_allocate_voice(0,4,(int)jet_moby);
            D_L13_0015F608=2; update_fighter_jet_flight(jet_moby,jet_data); jet_camera_mode(); jet_game_state(2,4); jet_moby->state=4;
        }
        break;
    case FIGHTER_JET_STATE_FLIGHT: {
        unsigned char *audio;
        if (jet_data->blend!=0.0f) { jet_blend(&jet_data->blend,0.0f,frame_time*4.0f); D_L13_0015F3FC=jet_data->blend; }
        jet_force_help(6,0); D_L13_0015F608=2;
        audio=D_0013E550+jet_data->engine_sound*0x70;
        if (*(Jet**)(audio+0x88)!=jet_moby || !audio[0x74]) { jet_data->engine_sound=-1; jet_data->engine_sound=jet_allocate_voice(0,4,(int)jet_moby); }
        update_fighter_jet_flight(jet_moby,jet_data);
        if (((JetPlayer*)(&jet_player47))->pad160F&1) jet_moby->state=5;
        if (jet_data->energy<=0) { jet_data->wait_timer=jet_scale_ticks(1200); jet_moby->state=8; }
        break;
    }
    case FIGHTER_JET_STATE_EXIT_PLAYER:
        ((JetPlayer*)(&jet_player47))->in_craft=0; *(int*)((char*)((JetPlayer*)(&jet_player47))->moby+0x98)=0;
        jet_set_hero_state(0,1); jet_end_mount(0); jet_set_weapon(((JetPlayer*)(&jet_player47))->weapon,0);
        qcopy(jet_moby->position,jet_data->spawn_position); qcopy(jet_moby->rotation,jet_data->spawn_rotation);
        jet_moby->scale=jet_moby->model->scale; jet_update_moby(jet_moby);
        if (jet_data->group_a>0) jet_group_state(jet_data->group_a,0,0,0);
        if (jet_data->group_b>0) jet_group_state(jet_data->group_b,0,0,0);
        jet_group_switch(D_L13_001CBDE8,1,1,-1); jet_group_switch(&D_L13_00161440,1,-1,-1); jet_group_flag(0x102,0);
        if (jet_data->exit_path!=-1) { JetPath *path=&D_L13_001600EC[jet_data->exit_path]; jet_place_camera(path->position,path->rotation,0,1); }
        else jet_place_camera(jet_data->spawn_position,D_L13_001600EC[jet_data->destination_path].rotation,0,1);
        {
            int id=jet_data->engine_sound;
            unsigned char *audio;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->engine_sound=-1;
            id=jet_data->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->weapon_sound=-1;
        }
        jet_game_state(0,5); jet_data->blend=0.99f; D_L13_0015F3FC=0.99f; jet_moby->state=1;
        break;
    case FIGHTER_JET_STATE_STOP_DEATH_SOUNDS:
        D_L13_0015F608=2;
        {
            int id=jet_data->engine_sound;
            unsigned char *audio;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->engine_sound=-1;
            id=jet_data->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->weapon_sound=-1;
        }
        jet_moby->state=7; break;
    case FIGHTER_JET_STATE_WAIT_FOR_DEATH_RESET: {
        JetTransition *transition;
        D_L13_0015F608=2;
        if (jet_fast_dec_death_timer(&jet_data->death_timer)) {
            transition=(JetTransition*)(D_001516D0);
            if (!transition->pending) {
            int active=transition->active;
            if (active==-1) {
                jet_death_complete(); jet_end_mount(0);
                qcopy(jet_moby->position,jet_data->spawn_position); qcopy(jet_moby->rotation,jet_data->spawn_rotation); jet_update_moby(jet_moby);
                {
            int id=jet_data->engine_sound;
            unsigned char *audio;
            if (id!=active) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->engine_sound=-1;
            id=jet_data->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->weapon_sound=-1;
        }
                jet_moby->state=0;
            }
        }
        }
        break;
    }
    case FIGHTER_JET_STATE_FORCED_EXIT:
        ((JetPlayer*)(&jet_player47))->in_craft=0; *(int*)((char*)((JetPlayer*)(&jet_player47))->moby+0x98)=0; jet_set_hero_state(0,1); jet_end_mount(0);
        if (jet_data->escape_path==-1) { stub_printf(D_L13_001F4470); qcopy(jet_moby->position,jet_data->spawn_position); qcopy(jet_moby->rotation,jet_data->spawn_rotation); }
        else { int index=jet_data->escape_path; JetPath *paths=D_L13_001600EC; qcopy(jet_moby->position,paths[index].position); qcopy(jet_moby->rotation,paths[index].rotation); jet_moby->position[2]+=0.57318f; }
        jet_moby->scale=jet_moby->model->scale; jet_update_moby(jet_moby);
        if (jet_data->destination_path==-1) stub_printf(D_L13_001F44A8);
        else {
            JetPath *path;
            if (jet_data->group_a>0) jet_group_state(jet_data->group_a,0,0,0);
            if (jet_data->group_b>0) jet_group_state(jet_data->group_b,0,0,0);
            jet_group_switch(D_L13_001CBDE8,1,1,-1); jet_group_switch(&D_L13_00161440,1,-1,-1); jet_group_flag(0x102,0);
            path=&D_L13_001600EC[jet_data->destination_path]; jet_place_camera(path->position,path->rotation,0,1);
        }
        if (jet_moby->persistent!=0xFF) jet_clear_persistent(jet_moby->persistent);
        {
            int id=jet_data->engine_sound;
            unsigned char *audio;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->engine_sound=-1;
            id=jet_data->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==jet_moby && audio[0x74]) jet_stop_sound(id); }
            jet_data->weapon_sound=-1;
        }
        jet_reset_camera((char *)&jet_player47+0x80,(char *)&jet_player47+0x90); jet_moby->state=9;
        break;
    case FIGHTER_JET_STATE_DELETE: jet_delete_moby(jet_moby); return;
    case 10: break;
    default: break;
    }
    if (jet_moby->position[0]<15.0f) jet_moby->position[0]=15.0f;
    if (jet_moby->position[0]>1008.0f) jet_moby->position[0]=1008.0f;
    if (jet_moby->position[1]<15.0f) jet_moby->position[1]=15.0f;
    if (jet_moby->position[1]>1008.0f) jet_moby->position[1]=1008.0f;
    if (jet_moby->position[2]<15.0f) jet_moby->position[2]=15.0f;
    if (jet_moby->position[2]>1008.0f) jet_moby->position[2]=1008.0f;
    state=jet_moby->state;
    if ((unsigned char)state==4) { qcopy_nc((char *)&jet_player47+0x80,jet_moby->position); qcopy_nc((char *)&jet_player47+0x90,jet_data->angles); }
    if ((unsigned int)(state-3)<2 || (unsigned char)state==8) spawn_fighter_jet_joint_particles(jet_moby,(char*)jet_data);
}
