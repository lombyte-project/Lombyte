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
extern void func_L01_0026F090_b0e80(int list, int state) __asm__("FUN_L01_0026e0e0");
extern char *D_L13_00160058_m_b0e80 __asm__("D_L13_0015FFD8");
extern char D_L13_001F41F8[];
void FUN_L13_002b0e80(char *moby) {
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x14), data[0x2C]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x18), data[0x2D]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x1C), data[0x2E]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x20), data[0x2F]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x14), data[0x30]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x18), data[0x31]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x1C), data[0x32]);
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
                func_L01_0026F090_b0e80(*(int *)(data + 0x20), data[0x33]);
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
extern void FUN_L13_002b4060(struct Moby *, float *, float *);
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
        ELEVATOR_STATE_MOVE_TO_BOTTOM = 3
    };
    LiftVars *vars = (LiftVars *)moby->pvars;
    float delta[4];
    float rot[4];
    float a[4];
    float b[4];
    float c[4];

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
            FUN_L13_002b4060(moby, b, c);
            subtract_vector_xyz(a, D_L13_001670C0_2b39e0, b);
            base = D_L13_001670C0_2b39e0 - 0x140;
            scale_vector_xyz(a, a, t);
            add_vector_xyz(b, b, a);
            c[1] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x154), c[1]) * t, c[1]);
            c[2] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x158), c[2]) * t, c[2]);
            FUN_L00_002ea9d8(b);
            FUN_L00_002eaa30(c);
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
            FUN_L13_002b4060(moby, a, b);
            FUN_L00_002ea9d8(a);
            FUN_L00_002eaa30(b);
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
    case 6:
    case 7:
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
    case 4:
    case 5: {
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

void FUN_L13_002b4060(struct Moby *moby, float *v, float *w);
void FUN_L13_002b4060(struct Moby *moby, float *v, float *w) {
    float ang = moby->rot.z;
    float r = 2.5f;
    float z = 1.5f;
    unsigned char *x;
    if ((moby->command >= 2 && (moby->command & 1)) || (moby->state >= 2 && (moby->state & 1))) {
        ang = fast_subtract_rotations(ang, 1.5707964f);
    } else {
        ang = fast_add_rotations(ang, 1.5707964f);
    }
    if (moby->oclass == 0x15) {
        r = 2.0f;
        z = 1.2f;
    }
    v[0] = fast_cos(ang) * r;
    v[1] = fast_sin(ang) * r;
    v[2] = z;
    fast_vec_add(v, v, &moby->pos);
    clear_u64_value(w);
    x = D_0013E533 + 0xE1D;
    w[2] = fast_arc_tan(*(float *)(x + 0xD0) - v[0], *(float *)(x + 0xD4) - v[1]);
    *(int *)(w + 1) = 0;
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
void blarg_turret_update(Moby_2B5368 *m) {
    Data_2B5368 *data = m->data;
    Moby_2B5368 *t;
    Moby_2B5368 *gun;
    float r;

    FUN_L13_002b48d8_2b0e80(m);
    t = data->target;
    if (data->gun != 0 && data->gun->cls_id == 0x24 && data->gun->state >= 0) {
        FUN_L00_00263ac8_2b0e80(2.7f, data->gun, 1, data->fx);
    }
    switch ((unsigned char)m->state) {
    case 0:
        m->scale = m->cls->scale * 1.5f;
        data->gun = FUN_L13_002b4958_2b0e80(m);
        data->f54 = m->rot.z;
        if (m->b53 != 0) {
            FUN_00212f90_2b0e80(m, 0, 0, 10);
        }
        m->state = 1;
        break;
    case 1:
        r = fast_difference_between_rotations(angle_atan2(t->pos.x - m->pos.x, t->pos.y - m->pos.y), data->f54);
        if (r < 1.5707964f && vector_distance(&m->pos, &t->pos) < 20.0f
            && (data->volume == -1 || is_point_inside_clip_volume(D_0013F420_2b0e80, data->volume) != 0)) {
            data->f68 = 8.0f;
            if (m->b53 != 1) {
                int a = random_int_between(0, 3);
                FUN_00212f90_2b0e80(m, 1, a, random_int_between(7, 13));
            }
            m->state = 3;
        }
        approach_angle(&m->rot.z, data->f54, &data->f60, D_0015ED70_c * 12.566371f, D_0015ED70_c * 12.566371f, frame_time * 12.566371f);
        if (data->gun == 0 || data->gun->cls_id != 0x24 || (unsigned char)data->gun->state == 0xFE
            || (unsigned char)data->gun->state == 0xFD || (unsigned char)data->gun->state == 2) {
            if (m->b53 != 0) {
                FUN_00212f90_2b0e80(m, 0, 0, random_int_between(7, 13));
            }
            data->f64 = 1.0f;
            m->state = 5;
        }
        break;
    case 3:
        r = fast_difference_between_rotations(angle_atan2(t->pos.x - m->pos.x, t->pos.y - m->pos.y), data->f54);
        if (r >= 1.5707964f || vector_distance(&m->pos, &t->pos) > 22.0f
            || (data->volume != -1 && is_point_inside_clip_volume(D_0013F420_2b0e80, data->volume) == 0)) {
            if (m->b53 != 0) {
                FUN_00212f90_2b0e80(m, 0, 0, random_int_between(7, 13));
            }
            m->state = 1;
        } else {
            float lim;
            approach_value(&data->f68, 20.0f, frame_time * 6.0f);
            r = FUN_001fa5c8_2b0e80(angle_atan2(t->pos.x - m->pos.x, t->pos.y - m->pos.y), data->f54);
            lim = data->f6C * 0.017453292f;
            if (lim < r) {
                r = lim;
            } else if (r < -lim) {
                r = -lim;
            }
            r = FUN_001fa580_2b0e80(r, data->f54);
            approach_angle(&m->rot.z, r, &data->f60, D_0015ED70_c * 12.566371f, D_0015ED70_c * 12.566371f, frame_time * 12.566371f);
            FUN_L13_002b47a0_2b0e80(m);
            if (data->gun == 0 || data->gun->cls_id != 0x24 || (unsigned char)data->gun->state == 0xFE
                || (unsigned char)data->gun->state == 0xFD || (unsigned char)data->gun->state == 2) {
                if (m->b53 != 0) {
                    FUN_00212f90_2b0e80(m, 0, 0, random_int_between(7, 13));
                }
                data->f64 = 1.0f;
                m->state = 5;
            }
        }
        break;
    case 4: {
        V4_2B5368 v;
        random_vector(&v.x, 0.5f, 1.0f);
        mark_moby_for_removal_cf(m, 0, -1);
        FUN_L00_0025e450_2b0e80(m, &v, &m->pos, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
        delete_moby(m);
        return;
    }
    case 5:
        approach_angle(&m->rot.z, data->f54, &data->f60, D_0015ED70_c * 12.566371f, D_0015ED70_c * 12.566371f, frame_time * 25.132742f);
        data->f64 -= 1.0f;
        if (data->f64 <= 0.0f) {
            m->state = 4;
        }
        break;
    }
    gun = data->gun;
    if (gun != 0 && gun->cls_id == 0x24 && (unsigned char)gun->state != 0xFE && (unsigned char)gun->state != 0xFD) {
        qcopy(&gun->rot, &m->rot);
        *(u128 *)&gun->mat[0] = *(u128 *)&m->mat[0];
        *(u128 *)&gun->mat[1] = *(u128 *)&m->mat[1];
        *(u128 *)&gun->mat[2] = *(u128 *)&m->mat[2];
    } else {
        data->gun = 0;
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5950), where it is exact; names translated to the US level program. */

extern char *FUN_L13_003061c8(float, float *, float *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int frame_count __asm__("D_L13_0015F5CC");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern float D_L13_00161388 __attribute__((sda));
int D_L13_0016138C = 2;
void FUN_L13_002b47a0(struct Moby *moby) {
    float v[4];
    float horizontal_offset[4];
    char *data;
    float d, x, ang;
    data = (char *)moby->pvars;
    if (frame_count % D_L13_0016138C == 0) {
        moby_point_position(moby, 0, v);
        d = vector_distance(&moby->pos, *(char **)(data + 0x40) + 0x10);
        x = (20.0f - d) / 20.0f * 5.0f + 2.5f;
        ang = fast_add_rotations(random_float_between(-x, x) * DEG_TO_RAD,
                                 moby->rot.z);
        horizontal_offset[0] = fast_cos(ang) * (D_L13_00161388 * frame_time);
        horizontal_offset[1] = fast_sin(ang) * (D_L13_00161388 * frame_time);
        horizontal_offset[2] = 0;
        fast_vec_add(v, v, horizontal_offset);
        FUN_L13_003061c8(*(float *)(data + 0x68), v, horizontal_offset, moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5A88), where it is exact; names translated to the US level program. */

extern int FUN_L00_0025fcb8(void *, void *, float);

void FUN_L13_002b48d8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float distance;
    if (moby->state == 3)
        distance = 22.0f;
    else
        distance = 20.0f;
    FUN_L00_0025fcb8(moby, data, distance);
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
extern void FUN_L00_0025d1b8(struct Moby *moby);

/* spawns a class 0x24 child that copies its parent's position and scale */
struct Moby *FUN_L13_002b4958(struct Moby *parent) {
    struct Moby *m;
    Vars *v;

    m = create_moby(0x24);
    if (m != 0) {
        v = (Vars *)m->pvars;
        v->parent = parent;
        v->unk0 = &v->unk20;
        v->unkC = v->unk60;
        m->update_dist = parent->update_dist;
        m->draw_dist = v->parent->draw_dist;
        m->visible = 1;
        m->flags |= 0x20;
        m->state = 0;
        m->command = 0;
        qcopy(&m->pos, &v->parent->pos);
        m->scale = m->pclass->scale * parent->scale / parent->pclass->scale;
        v->unk20 = 1.0f;
        v->unk24 = 1;
        v->unk29 = 1;
        v->unk2A = 3;
        v->unk28 = 0;
        v->unk30 = 0.9f;
        FUN_L00_00250df8(m);
        FUN_L00_0025d1b8(m);
    }
    return m;
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B5C08), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void FUN_L13_002b4a58(struct Moby *m, char *p) {
    enum {
        HIT_REACTION_NONE = 11
    };
    float *tm = (float *)(p + 0x20);
    char *q;
    int k;
    int r;
    if (m->state != 2) {
        q = find_moby_hit(m, 0x330000, 0);
        r = hit_reaction(m, q, tm, 0, &k, 0, 0, 4);
        if (q != 0) {
            if (*(int *)(q + 0x24) & 0x800000) {
                char *t = *(char **)(q + 0x20);
                if (t == 0) {
                    *(int *)(q + 0x2C) = 0;
                    k = 1;
                } else {
                    float f = fast_difference_between_rotations(
                        fast_arc_tan(*(float *)(t + 0x10) - m->pos.x,
                                     *(float *)(t + 0x14) - m->pos.y),
                        *(float *)(*(char **)(*(char **)(p + 0x70) + 0x78) + 0x54));
                    if (f < 1.65806282f) {
                        *(int *)(q + 0x2C) = 0;
                        k = 1;
                    }
                }
            }
        }
        switch (r) {
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
            float f1 = *(float *)(q + 0x2C);
            float f0 = *tm;
            if (f0 <= f1) {
                char *x;
                *(int *)tm = 0;
                m->flags &= 0xEFFF;
                x = *(char **)(p + 0x70);
                if (x != 0) {
                    if (*(short *)(x + 0xA6) == 0x1D) {
                        unsigned char st = x[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD)
                                x[0x20] = 4;
                        }
                    }
                }
                FUN_L00_00257470(m, 0, -1);
                p[0x67] = 0x78;
                trooper_anim_update(m, (short *)(p + 0x60));
                if (m->prev_seq != 3)
                    blend_moby_animation(m, 3, 1, 0);
                m->state = 2;
            } else {
                *tm = f0 - f1;
                *(unsigned char *)(p + 0x67) = 0xFA;
                *(short *)(p + 0x26) = scale_game_frames(0x3C);
                trooper_anim_update(m, (short *)(p + 0x60));
            }
        }
    }
    m->hit_slot = 0xFF;
    trooper_anim_finish(m, p + 0x60);
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
    unsigned char *d = moby->pvars;
    float a[4];
    float b[4];
    switch (moby->state) {
    case 0:
        d[0x29] = 1;
        moby->state = 1;
        d[0x5A] = 6;
        d[0x58] = 8;
        break;
    case 1:
        if ((*(unsigned char **)(d + 0x70))[0x20] == 3) {
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
        FUN_L13_002b4a58_c(moby, d);
        break;
    case 2:
        if (moby->anim_flags & 2) {
            subtract_vector_xyz(a, D_L13_001670C0, &moby->pos);
            moby_point_position(moby, 0, b);
            spawn_explosion(moby, a, b, 0.0f, 0.0f, 0x14, 9, 0x20, 10.0f, 7.0f, 20.0f, -1, 2.0f,
                             40.0f, 0, 1, -1, 0);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B60D8), where it is exact; names translated to the US level program. */

extern void * FUN_L13_00280400(void *, int, int, float, float, int, float, int);
extern void kill_part(void *) __asm__("FUN_L00_00267a08");

/* Per-frame glow effect: spawns it while the moby is active (byte 0x31 set), frees it otherwise. */
void FUN_L13_002b4f28(struct Moby *moby, char *data) {
    float opacity = 0.2f;
    void **handle = (void **)(data + 0x228);
    if (moby->visible) {
        if (!*handle) {
            *handle = FUN_L13_00280400(moby, 0, 0x80808080, opacity, opacity, 0x10808080, 0.8f, 0x19);
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
void FUN_L13_002b4fb8(Level13VendorMoby *moby) {
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

void FUN_L13_002b4ff0(struct Moby *m, char *d) {
    float ground = probe_ground_height(&m->pos, 0, 0.5f);
    float z = m->pos.z;
    if (ground < z + 0.3f) {
        float v = *(float *)(d + 0x128) - *(float *)&D_0015ED70 * 9.8f;
        *(float *)(d + 0x128) = v;
        m->pos.z = m->pos.z + v;
        if (m->pos.z < ground)
            m->pos.z = ground;
    } else if (z < ground) {
        m->pos.z = z + *(float *)&D_0015ED70 * 9.8f;
        qzero(d + 0x120);
    } else {
        qzero(d + 0x120);
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
extern float D_L13_00161390 __attribute__((sda));
extern float D_L13_00161394 __attribute__((sda));
extern float D_L13_00161398 __attribute__((sda));
extern float D_L13_0016139C __attribute__((sda));
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
extern int FUN_L01_0026e008(int, int);
extern int FUN_L01_0028b510(void *, void *, float);
extern int FUN_L01_0028b5e0(void *, void *, void *, void *);
extern int FUN_L01_002efbf8(int *, int, int);
extern int FUN_L13_0026bd00(void *, void *, float);
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
void FUN_L13_002b4f28(struct Moby *moby, char *data);
void FUN_L13_002b4ff0(struct Moby *m, char *d);
extern int func_001F9908_i(void *) __asm__("FUN_001f9740");

void robosquawker_update(RsMoby *m) __asm__("FUN_L13_002b50d8");
void robosquawker_update(RsMoby *m) {
    RsVars *v;
    RsVec old;
    RsTarget tgt;
    RsVec va;
    RsVec vb;
    RsVec vc;
    int hits;
    float dmg;
    float ang;
    float h;
    float out;
    RsHit *hit;
    RsTargetMoby *tm;

    v = m->vars;
    FUN_L00_00263ac8(2.5f, (int)m, 0, v->x230);
    if (m->b31 != 0) {
        if (distance_xyz(&m->pos, D_L13_001670C0) < 28.0f) {
            FUN_L00_0025a120(m);
            m->b7F = 0x16;
        }
    }
    if (D_L13_0015F5C4 == 2) {
        m->w94 = 0;
        m->h34 |= 0x41;
        return;
    }
    m->w94 = *(int *)(m->w24 + 0x10);
    m->h34 &= 0xFFBE;
    FUN_L13_0026bd00(m, &tgt, v->f1FC + 4.0f);
    tm = tgt.moby;
    if (tm == 0 || (v->h214 == 0 && v->w22C != -1 && is_point_inside_clip_volume(&tgt, v->w22C) == 0)) {
        tgt.moby = 0;
        tm = 0;
        qzero(&tgt);
    }
    qcopy(&old, &m->pos);
    if (v->w38 != 0) {
        v->h214 = scale_game_frames_alt(0xF0);
    }
    v->w38 = 0;
    fast_dec_timer_s16(&v->h208);
    if (m->state != 0) {
        if (fast_dec_timer_s16(&v->h214) != 0) {
            v->f1FC = v->v1D0.f[3];
        } else {
            v->f1FC = v->f200;
        }
    }
    dmg = 0.0f;
    hit = FUN_L00_0025a420(m, 0x330000, 0);
    fun_00213928(m, hit, &v->f20, 0, &hits, &dmg, 0, 4);
    fast_dec_timer_s16(&v->h26);
    if (v->h26 < scale_game_frames_alt(0x2D) && hit != 0 && m->state != 0x63) {
        if (hit->src->hA6 >= 0xB0 && hit->src->hA6 <= 0xB1 && v->h26 != 0) {
            hit->f2C = 0.0f;
        }
        if (m->b21 != 0xFF) {
            FUN_L01_0026e090(m->b21, 1);
        }
        if (hit->f2C != 0.0f) {
            ang = fast_arc_tan(m->pos.f[0] - tgt.pos.f[0], m->pos.f[1] - tgt.pos.f[1]);
            v->f148 = 0.5f;
            v->w140 = 0x202;
            v->f20 -= hit->f2C;
            v->f130 = 0.0055f;
            v->f134 = 0.0011f;
            v->f138 = D_L13_00161394 * frame_time;
            v->f13C = D_L13_00161390 * frame_time;
            v->w144 = 9;
            v->b15D = 0;
            if (v->f20 <= 0.0f) {
                m->h34 &= 0xEFFF;
                v->f138 = frame_time * 10.0f;
                v->f13C = frame_time * 5.5f;
                *(u128 *)&va = *(u128 *)&hit->pos;
                trooper_knockback_arc(&va, &ang, &v->f138, &v->f13C);
                trooper_start_knockback(m, v->x120, ang, (rand() & 1) + 9, 1, 0);
                v->f170 = 11.0f;
                v->f174 = 18.0f;
                m->state = 0x63;
                FUN_L13_002b4fb8((Level13VendorMoby *)v);
                v->x110[7] = 0x78;
                trooper_anim_update(m, (short *)v->x110);
                FUN_L00_00257470(m, 0, -1);
            } else {
                *(u128 *)&va = *(u128 *)&hit->pos;
                trooper_knockback_arc(&va, &ang, &v->f138, &v->f13C);
                trooper_start_knockback(m, v->x120, ang, (rand() & 1) + 9, 1, 0);
                v->f170 = 5.0f;
                v->f174 = 10.0f;
                m->state = 6;
                FUN_L13_002b4fb8((Level13VendorMoby *)v);
                *(unsigned char *)&v->x110[7] = 0xFA;
                v->h26 = scale_game_frames_alt(0x3C);
                trooper_anim_update(m, (short *)v->x110);
            }
        }
    }
    m->bA4 = 0xFF;
    switch (m->state) {
    case 0:
        qcopy(&v->v1D0, &m->pos);
        v->v1D0.f[3] = v->f1FC;
        if (v->h20A != 0) {
            m->state = 1;
            if (m->b53 != 3) {
                blend_moby_animation_c(m, 3, 0, 0);
            }
        } else {
            m->state = 0xE;
            m->pos.f[2] += 6.0f;
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, 0);
            }
        }
        v->h214 = 0;
        v->h208 = 0;
        v->b29 = 0;
        v->f20 = 1.0f;
        v->h24 = 1;
        v->b58 = 8;
        v->b5A = 4;
        FUN_L01_0026d930((char *)&v->w180);
        v->w180 = 0x38D;
        v->f18C = 2.0f;
        v->f188 = 2.0f;
        v->f1A4 = v->f204 * frame_time;
        v->w1B8 |= 0x30;
        if (rand() & 1) {
            m->h34 |= 0x8000;
        }
        v->pD0 = &D_L13_001613A0;
        if (v->w220 != -1) {
            v->w224 = FUN_L01_0028b510(&m->pos, D_L13_001B07B0[v->w220], 0.0f);
        }
        break;
    case 0xE: {
        float d;

        FUN_L13_002b4f28((unsigned char *)m, (char *)v);
        subtract_vector_xyz(&va, &tgt, &m->pos);
        d = probe_ground_height(&m->pos, 0x120, 0.5f);
        if (d > 1.0f
            && vector_length_xy(&va) < v->f1FC + 4.0f
            && absolute_float(m->pos.f[2] - tgt.pos.f[2]) < 10.0f
            && random_integer_below(0x13) == 0) {
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            m->state = 0x11;
            FUN_L13_002b4fb8((Level13VendorMoby *)v);
        } else if (d > 1.0f && m->bBC == 1 && random_integer_below(4) == 0) {
            if (random_integer_below(0x100) & 1) {
                v->f1F8 = -v->f1F8;
            }
            v->f1F4 = v->f1F8 * 0.017453292f;
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            FUN_L13_002b4fb8((Level13VendorMoby *)v);
            m->state = 0x11;
            clear_u64_value(&v->v40);
            m->bBC = 0;
        } else if (v->w220 != -1) {
            char *path;

            path = D_L13_001B07B0[v->w220];
            if (distance_xyz(&m->pos, path + (v->w224 * 16 + 16)) < 0.25f) {
                v->w224 += 1;
                if (v->w224 >= *(int *)path) {
                    v->w224 = 0;
                }
            }
            qcopy(&v->v1E0, path + v->w224 * 16 + 16);
            m->state = 0x12;
        } else if (func_001F9908_i(&v->w20C) != 0) {
            int i;

            v->w20C = scale_game_frames_alt(FUN_L00_00257b90(0x28, 0x78));
            for (i = 0; i < 10; i++) {
                float r;
                float a;

                r = random_float_between(0.0f, 4.0f);
                a = random_angle_radians();
                vb.f[0] = fast_cos(a) * r;
                vb.f[1] = fast_sin(a) * r;
                vb.f[2] = 0.0f;
                vb.f[2] = random_float_between(5.0f, 7.0f);
                add_vector_xyz(&v->v1E0, &vb, &v->v1D0);
                if (probe_ground_height(&v->v1E0, 0x120, 0.5f) > 1.0f) {
                    break;
                }
            }
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            m->state = 0xF;
        }
        break;
    }
    case 0xF: {
        float a;

        a = fast_arc_tan(v->v1E0.f[0] - m->pos.f[0], v->v1E0.f[1] - m->pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.03f, 0.6f, 0.09f);
        h = vector_length_xyz(&v->v40);
        m->f58 = h * 20.0f + 1.0f;
        subtract_vector_xyz(&v->v40, &v->v1E0, &m->pos);
        if (vector_length_xyz(&v->v40) > 1.0f) {
            approach_value(&h, frame_time * 4.0f, frame_time_sq * 6.0f);
        } else {
            approach_value(&h, frame_time, frame_time_sq * 8.0f);
        }
        FUN_L00_0025f730(&v->v40, h);
        add_vector_xyz(&va, &v->v40, &m->pos);
        if (coll_sphere(1.2f, &va, 0, m) != 0) {
            clear_u64_value(&v->v40);
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            m->state = 0xE;
        } else {
            qcopy(&m->pos, &va);
        }
        if (distance_xyz(&m->pos, &v->v1E0) < h || h == 0.0f) {
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            m->state = 0xE;
        }
        FUN_L13_002b4f28((unsigned char *)m, (char *)v);
        break;
    }
    case 0x12: {
        char *path;
        float a;
        float d;

        path = D_L13_001B07B0[v->w220];
        a = fast_arc_tan(v->v1E0.f[0] - m->pos.f[0], v->v1E0.f[1] - m->pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.018f, 0.8f, 0.09f);
        subtract_vector_xyz(&v->v40, &v->v1E0, &m->pos);
        FUN_L00_0025f730(&v->v40, frame_time * 4.0f);
        add_vector_xyz(&va, &v->v40, &m->pos);
        m->f58 = vector_length_xyz(&v->v40) * 20.0f + 1.0f;
        if (coll_sphere(1.2f, &va, 0, m) != 0) {
            int i;

            v->w20C = scale_game_frames_alt(0x78);
            for (i = 0; i < 10; i++) {
                float r;
                float an;

                r = random_float_between(0.0f, 3.0f);
                an = random_angle_radians();
                vb.f[0] = fast_cos(an) * r;
                vb.f[1] = fast_sin(an) * r;
                vb.f[2] = 0.0f;
                vb.f[2] = random_float_between(5.0f, 7.0f);
                add_vector_xyz(&v->v1E0, &vb, &v->v1D0);
                if (probe_ground_height(&v->v1E0, 0x120, 0.5f) > 1.0f) {
                    break;
                }
            }
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            m->state = 0xF;
        } else {
            qcopy(&m->pos, &va);
        }
        d = distance_xyz(&m->pos, &v->v1E0);
        if (d < vector_length_xyz(&v->v40)) {
            float g;

            v->w224 += 1;
            if (v->w224 >= *(int *)path) {
                v->w224 = 0;
            }
            qcopy(&v->v1E0, path + v->w224 * 16 + 16);
            v->v1E0.f[2] = v->v1E0.f[2] + random_float_between(-0.2f, 0.2f);
            g = probe_ground_height(&m->pos, 0x120, 0.5f);
            if (g > 1.0f
                && vector_length_xy(&vb) < v->f1FC + 4.0f
                && absolute_float(m->pos.f[2] - tgt.pos.f[2]) < 10.0f
                && random_integer_below(0x13) == 0) {
                if (m->b53 != 0) {
                    blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
                }
                qzero(&v->v40);
                m->state = 0x11;
                FUN_L13_002b4fb8((Level13VendorMoby *)v);
            } else if (g > 1.0f && m->bBC == 1 && random_integer_below(4) == 0) {
                if (random_integer_below(0x100) & 1) {
                    v->f1F8 = -v->f1F8;
                }
                v->f1F4 = v->f1F8 * 0.017453292f;
                v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
                if (m->b53 != 0) {
                    blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
                }
                FUN_L13_002b4fb8((Level13VendorMoby *)v);
                qzero(&v->v40);
                m->state = 0x11;
                clear_u64_value(&v->v40);
                m->bBC = 0;
            }
        }
        FUN_L13_002b4f28((unsigned char *)m, (char *)v);
        break;
    }
    case 0x11: {
        float g;

        g = probe_ground_height(&m->pos, 0x120, 0.5f);
        approach_value(&v->v40.f[2], -(frame_time * 8.0f), frame_time_sq * 10.8f);
        FUN_L00_00258830(m, &v->v40, 0.6f, 0.5f, 0.0f, 0);
        if (m->b53 != 2
            && m->pos.f[2] - g <= frame_time * 8.0f * FUN_L04_001f1978(45.0f)
            && m->b53 != 2) {
            blend_moby_animation_c(m, 2, 0, scale_game_frames_alt(5));
        }
        if (m->pos.f[2] <= g) {
            m->pos.f[2] = g;
            if (random_integer_below(0x100) & 1) {
                v->f1F8 = -v->f1F8;
            }
            v->f1F4 = v->f1F8 * 0.017453292f;
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            m->state = 3;
            clear_u64_value(&v->v40);
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
        }
        break;
    }
    case 0x10: {
        float g;

        g = probe_ground_height(&m->pos, 0x120, 0.5f);
        if (m->pos.f[2] - g < 3.0f) {
            approach_value(&v->v40.f[2], frame_time * 8.0f, frame_time_sq * 10.8f);
        } else {
            approach_value(&v->v40.f[2], frame_time, frame_time_sq * 10.8f);
        }
        FUN_L00_00258830(m, &v->v40, 0.6f, 0.5f, 0.0f, 0);
        m->f58 = 6.0f - (m->pos.f[2] - g) / 6.0f * 5.0f;
        if (g + 6.0f <= m->pos.f[2]) {
            m->pos.f[2] = g + 6.0f;
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
            m->state = 0xE;
            clear_u64_value(&v->v40);
        }
        FUN_L13_002b4f28((unsigned char *)m, (char *)v);
        break;
    }
    case 1:
        subtract_vector_xyz(&va, &tgt, &m->pos);
        if (vector_length_xyz(&va) < v->f1FC + 4.0f
            && absolute_float(m->pos.f[2] - tgt.pos.f[2]) < 8.0f
            && random_integer_below(0x13) == 0) {
            m->state = 2;
            if (m->b53 != 4) {
                blend_moby_animation_c(m, 4, 0, scale_game_frames_alt(3));
            }
        } else if (m->bBC == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                v->f1F8 = -v->f1F8;
            }
            v->f1F4 = v->f1F8 * 0.017453292f;
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
            m->bBC = 0;
        } else if (vector_length_xyz(&va) > v->f1FC + 6.0f && v->h20A == 0) {
            qzero(&v->v40);
            m->state = 0x10;
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
            }
        }
        FUN_L13_002b4ff0((char *)m, (char *)v);
        break;
    case 8:
        subtract_vector_xyz(&va, &tgt, &m->pos);
        if (vector_length_xyz(&va) < v->f1FC - 4.0f
            && absolute_float(m->pos.f[2] - tgt.pos.f[2]) < 8.0f
            && random_integer_below(0x13) == 0) {
            m->state = 2;
            m->pos.f[2] += 1.0f;
            if (m->b53 != 4) {
                blend_moby_animation_c(m, 4, 0, scale_game_frames_alt(3));
            }
        }
        break;
    case 0xA: {
        float a;

        a = fast_arc_tan(v->v1D0.f[0] - m->pos.f[0], v->v1D0.f[1] - m->pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.05f, 0.3f, 0.2f);
        subtract_vector_xyz(&va, &tgt, &v->v1D0);
        if ((vector_length_xyz(&va) < v->f1FC && absolute_float(v->v1D0.f[2] - tgt.pos.f[2]) < 8.0f) || m->bBC == 2) {
            if (m->b21 != 0xFF) {
                FUN_L01_0026e090(m->b21, 2);
            }
            if (m->b53 != (rand() & 1) + 9) {
                blend_moby_animation_c(m, (rand() & 1) + 9, 0, scale_game_frames_alt(3));
            }
            v->f134 = 0.0f;
            v->f130 = D_L13_0016139C;
            v->f138 = D_L13_00161398 * frame_time;
            v->f13C = FUN_L00_0025abf0(&m->pos, &v->v1D0, &out, D_L13_00161398 * frame_time, -D_L13_0016139C);
            v->w144 = 0xD;
            v->b15D = 0;
            trooper_start_knockback(m, v->x120, m->rot.f[2], (rand() & 1) + 9, 1, 0);
            v->f170 = 5.0f;
            v->f174 = 10.0f;
            FUN_L13_002b4f28((unsigned char *)m, (char *)v);
            m->state = 0xB;
        }
        break;
    }
    case 0xB:
        if (mine_fall(m, v->x120) & 1) {
            FUN_L13_002b4fb8((Level13VendorMoby *)v);
            qzero(v->x120);
            m->state = 3;
            if (random_integer_below(0x100) & 1) {
                v->f1F8 = -v->f1F8;
            }
            v->f1F4 = v->f1F8 * 0.017453292f;
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 8, scale_game_frames_alt(3));
            }
            return;
        } else if (m->pos.f[2] < 5.0f) {
            FUN_L00_00257470(m, 0, -1);
            mark_moby_for_removal_c(m);
            return;
        }
        break;
    case 2: {
        float a;

        a = fast_arc_tan(m->pos.f[0] - tgt.pos.f[0], m->pos.f[1] - tgt.pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.02f, 0.3f, 0.1f);
        subtract_vector_xyz(&va, &tgt, &m->pos);
        if (vector_length_xyz(&va) < v->f1FC
            && absolute_float(m->pos.f[2] - tgt.pos.f[2]) < 8.0f
            && random_integer_below(0x13) == 0) {
            if (m->b21 != 0xFF) {
                FUN_L01_0026e090(m->b21, 1);
            }
            if (random_integer_below(0x100) & 1) {
                v->f1F8 = -v->f1F8;
            }
            v->f1F4 = v->f1F8 * 0.017453292f;
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
            m->bBC = 0;
        } else if (m->bBC == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                v->f1F8 = -v->f1F8;
            }
            v->f1F4 = v->f1F8 * 0.017453292f;
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
            m->bBC = 0;
        }
        if (m->b53 == 4 && (m->b70 & 2)) {
            blend_moby_animation_c(m, 3, 0, scale_game_frames_alt(3));
        }
        FUN_L13_002b4ff0((char *)m, (char *)v);
        break;
    }
    case 3: {
        float r;
        int res;
        float a;
        float d;

        r = absolute_float(m->pos.f[2] - tgt.pos.f[2]);
        r = r / vector_distance(&m->pos, &tgt);
        if (v->w21C != -1) {
            FUN_L01_0028b5e0(m, D_L13_001B07B0[v->w21C], &tgt, &vb);
        } else {
            qcopy(&vb, &tgt);
        }
        a = fast_arc_tan(vb.f[0] - m->pos.f[0], vb.f[1] - m->pos.f[1]) + v->f1F4;
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.05f, 0.3f, 0.2f);
        va.f[0] = fast_cos(m->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(m->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(m, &v->w180, &va, &vb, 1.0f);
        if (func_001F9908_i(&v->w210) != 0) {
            v->w210 = scale_game_frames_alt((int)random_float_between(30.0f, 90.0f));
            v->f1F4 = -v->f1F4;
        }
        subtract_vector_xyz(&vc, &tgt, &v->v1D0);
        d = vector_distance(&m->pos, &tgt);
        if ((d < 1.5f
             || (tgt.moby != 0 && tgt.moby->hA6 == 0x10E && tgt.moby->state != 0xFE && tgt.moby->state != 0xFD && d < 2.3f))
            && !(res & 2)) {
            m->state = 4;
            if (m->b53 != 6) {
                blend_moby_animation_c(m, 6, 0, scale_game_frames_alt(3));
            }
        } else {
            if (r > 1.0f || (res & 2)) {
                m->pos.f[0] = old.f[0];
                m->pos.f[1] = old.f[1];
            }
            if (vector_length_xyz(&vc) < v->f1FC + 4.0f && absolute_float(m->pos.f[2] - tgt.pos.f[2]) < 8.0f) {
                if (m->b21 != 0xFF) {
                    FUN_L01_0026e090(m->b21, 1);
                }
            } else if (m->bBC != 1) {
                m->state = 5;
            }
        }
        m->bBC = 0;
        FUN_L13_002b4ff0((char *)m, (char *)v);
        break;
    }
    case 4: {
        float a;
        int same;
        float hh;
        float dz;
        float d;
        float da;
        float g;

        a = fast_arc_tan(tgt.pos.f[0] - m->pos.f[0], tgt.pos.f[1] - m->pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.05f, 0.3f, 0.2f);
        same = m->b52 == m->b53;
        hh = compute_interpolated_record_value(m);
        dz = absolute_float(m->pos.f[2] - tgt.pos.f[2]);
        d = vector_distance(&m->pos, &tgt);
        da = fast_difference_between_rotations(fast_arc_tan(tgt.pos.f[0] - m->pos.f[0], tgt.pos.f[1] - m->pos.f[1]), m->rot.f[2]);
        if (same
            && absolute_float(hh - 13.0f) < 0.2f
            && dz < 1.5f
            && (d < 2.0f
                || (tgt.moby != 0 && tgt.moby->hA6 == 0x10E && tgt.moby->state != 0xFE && tgt.moby->state != 0xFD && d < 2.6f))
            && da < 0.2617994f) {
            va.f[0] = fast_cos(m->rot.f[2]) * 0.2f;
            va.f[1] = fast_sin(m->rot.f[2]) * 0.2f;
            va.f[2] = 0.0f;
            qcopy(&vb, &tgt);
            vb.f[2] += 0.75f;
            if (tm != 0) {
                spawn_nearby_effect(tm, (int)m, 1, &vb, &va, 1.0f);
            }
        }
        if ((m->b70 & 2) && vector_distance(&m->pos, &tgt) > 1.5f) {
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
        }
        g = probe_ground_height(&m->pos, 0x120, 0.5f);
        v->f198 -= frame_time_sq * 9.8f;
        m->pos.f[2] += v->f198;
        if (m->pos.f[2] < g) {
            v->f198 = 0.0f;
            m->pos.f[2] = g;
        }
        FUN_L13_002b4ff0((char *)m, (char *)v);
        break;
    }
    case 5: {
        int res;
        float a;

        if (v->w21C != -1) {
            FUN_L01_0028b5e0(m, D_L13_001B07B0[v->w21C], &v->v1D0, &vb);
        } else {
            qcopy(&vb, &v->v1D0);
        }
        a = fast_arc_tan(vb.f[0] - m->pos.f[0], vb.f[1] - m->pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.02f, 0.3f, 0.1f);
        va.f[0] = fast_cos(m->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(m->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(m, &v->w180, &va, &vb, 1.0f);
        if (vector_distance(&m->pos, &v->v1D0) < 2.0f) {
            m->state = 1;
            if (m->b53 != 3) {
                blend_moby_animation_c(m, 3, 0, scale_game_frames_alt(3));
            }
        }
        subtract_vector_xyz(&vc, &tgt, &v->v1D0);
        if (vector_length_xyz(&vc) < v->f1FC) {
            if (m->b21 != 0xFF) {
                FUN_L01_0026e090(m->b21, 1);
            }
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
        } else if (m->bBC == 1) {
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
        } else if (res & 2) {
            m->pos.f[0] = old.f[0];
            m->pos.f[1] = old.f[1];
            m->state = 0xC;
            if (m->b53 != 4) {
                blend_moby_animation_c(m, 4, 0, scale_game_frames_alt(3));
            }
        }
        FUN_L13_002b4ff0((char *)m, (char *)v);
        m->bBC = 0;
        break;
    }
    case 6:
        if (mine_fall(m, v->x120) & 1) {
            qzero(v->x120);
            m->state = 3;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
            return;
        } else if (m->pos.f[2] < 5.0f) {
            FUN_L00_00257470(m, 0, -1);
            mark_moby_for_removal_c(m);
            return;
        }
        break;
    case 7:
        FUN_L13_002b4fb8((Level13VendorMoby *)v);
        if (FUN_L00_002dc9f0(m, v->x120) != 0) {
            m->state = 0xE;
            if (m->b53 != 0) {
                blend_moby_animation_c(m, 0, 0, 0);
            }
            v->hC8 = 0;
        }
        break;
    case 0x63: {
        int t;

        m->h34 &= 0xEFFF;
        if (mine_fall(m, v->x120) & 0x140) {
            clear_u64_value(&va);
            t = (int)((float)scale_game_frames_alt(1) * 60.0f);
            if (FUN_L01_002efbf8(D_L13_001613B0, 3, t) >= 0) {
                cart_burst(m, &m->pos, 4, 0.5f, 13.0f);
            }
            mark_moby_for_removal_c(m);
            return;
        } else if (m->pos.f[2] < 0.0f) {
            mark_moby_for_removal_c(m);
            return;
        }
        break;
    }
    case 0xC: {
        float r;
        int res;
        float a;

        r = absolute_float(m->pos.f[2] - tgt.pos.f[2]);
        r = r / vector_distance(&m->pos, &tgt);
        if (v->w21C != -1) {
            FUN_L01_0028b5e0(m, D_L13_001B07B0[v->w21C], &v->v1D0, &vb);
        } else {
            qcopy(&vb, &v->v1D0);
        }
        a = fast_arc_tan(vb.f[0] - m->pos.f[0], vb.f[1] - m->pos.f[1]);
        FUN_L00_00258278((char *)m, &v->f1F0, a, 0.05f, 0.3f, 0.2f);
        va.f[0] = fast_cos(m->rot.f[2]) * 2.0f;
        va.f[1] = fast_sin(m->rot.f[2]) * 2.0f;
        va.f[2] = 0.0f;
        res = FUN_L00_00258b50(m, &v->w180, &va, &vb, 1.0f);
        subtract_vector_xyz(&vc, &tgt, &v->v1D0);
        if (vector_distance(&m->pos, &tgt) < 1.5f && r <= 1.0f) {
            m->state = 4;
            if (m->b53 != 6) {
                blend_moby_animation_c(m, 6, 0, scale_game_frames_alt(3));
            }
        } else if (r <= 1.0f && !(res & 2)) {
            m->state = 5;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
        } else {
            if (m->b70 & 2) {
                int anim;

                anim = 4;
                random_integer_below(4);
                if (random_integer_below(4) != 0) {
                    anim = 3;
                }
                blend_moby_animation_c(m, anim, 0, scale_game_frames_alt(3));
            }
            if (random_integer_below(0x63) == 0) {
                qzero(&v->v40);
                m->state = 0x10;
                if (m->b53 != 0) {
                    blend_moby_animation_c(m, 0, 0, scale_game_frames_alt(3));
                }
            }
        }
        m->bBC = 0;
        break;
    }
    case 0xD: {
        RsOther *o;
        float g;

        if (m->b52 != 5 && m->b53 != 5) {
            blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
        }
        va.f[0] = fast_cos(m->rot.f[2]) * (v->f204 * frame_time);
        va.f[1] = fast_sin(m->rot.f[2]) * (v->f204 * frame_time);
        va.f[2] = 0.0f;
        add_vector_xyz(&vb, &m->pos, &va);
        for (o = (RsOther *)D_L13_0015FFE4; o != 0; o = o->next) {
            if (o == (RsOther *)m) {
                continue;
            }
            if (o->state < 0) {
                continue;
            }
            if (o->hA6 != 0 && o->hA6 != 0x3F) {
                continue;
            }
            if (vector_distance(&vb, &o->pos) < 2.0f) {
                float z;
                float a;
                float d;

                z = m->pos.f[2];
                a = fast_arc_tan(o->pos.f[0] - m->pos.f[0], o->pos.f[1] - m->pos.f[1]);
                a = fast_subtract_rotations(m->rot.f[2], a);
                d = frame_time * 1.5707964f;
                if (!(a > 0.0f)) {
                    d = -d;
                }
                m->rot.f[2] = fast_add_rotations(m->rot.f[2], d);
                subtract_vector_xyz(&vb, &m->pos, &o->pos);
                normalize_vector_xyz(&vb, &vb, 2.0f);
                add_vector_xyz(&vb, &vb, &o->pos);
                vb.f[2] = z;
            }
        }
        qcopy(&m->pos, &vb);
        g = probe_ground_height(&m->pos, 0x120, 0.5f);
        v->f198 -= frame_time_sq * 9.8f;
        m->pos.f[2] += v->f198;
        if (m->pos.f[2] < g) {
            v->f198 = 0.0f;
            m->pos.f[2] = g;
        }
        if (v->p218 == 0 || vector_distance(&m->pos, v->p218 + 0x10) > convert_integer_to_float(v->h216)) {
            m->state = 5;
            if (m->b53 != 5) {
                blend_moby_animation_c(m, 5, 0, scale_game_frames_alt(3));
            }
        }
        m->bBC = 0;
        break;
    }
    }
    trooper_anim_finish(m, v->x110);
    if (m->pos.f[0] < 2.0f || m->pos.f[0] > 1021.0f
        || m->pos.f[1] < 2.0f || m->pos.f[1] > 1021.0f
        || m->pos.f[2] < 2.0f || m->pos.f[2] > 1021.0f) {
        mark_moby_for_removal_c(m);
        return;
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002B9800), where it is exact; names translated to the US level program. */


extern void FUN_L00_0025f8e0(float *v, float s);
extern void FUN_L01_0028a7a8(void *, float *, float *, void *, int);

short D_L13_001613D8 = 0xff;
short D_L13_001613E8 = -1;
short D_L13_001613F8 = 0x7f4f;
short D_L13_00161408 = -32513;
short D_L13_00161418 = 0x7f00;
void FUN_L13_002b8590(void *moby, char *obj) {
    float vel[4];
    float pos[4];
    float a[4];
    float b[4];
    float c[4];
    float s;
    int i;

    for (i = 1; i < 3; i++) {
        *(OvlQuad *)vel = 0;
        s = *(float *)(obj + 0x64) / (frame_time * 24.0f);
        if (s > 1.0f)
            s = 1.0f;
        else if (s < 0.35f)
            s = 0.35f;
        moby_point_position(moby, i, pos);
        pos[3] = 0;
        qcopy(a, pos);
        qcopy(b, pos);
        qcopy(c, pos);
        FUN_L00_0025f8e0(a, *(float *)(obj + 0xC4) * 0.02f + 0.02f);
        FUN_L00_0025f8e0(b, *(float *)(obj + 0xC4) * 0.02f + 0.02f);
        FUN_L00_0025f8e0(c, *(float *)(obj + 0xC4) * 0.01f + 0.02f);
        vel[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 2.0f) * frame_time) *
            s;
        FUN_L01_0028a7a8(moby, a, vel, &D_L13_001613D8, 1);
        vel[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 3.0f) * frame_time) *
            s;
        FUN_L01_0028a7a8(moby, b, vel, &D_L13_001613E8, 1);
        vel[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 4.0f) * frame_time) *
            s;
        FUN_L01_0028a7a8(moby, c, vel, &D_L13_001613F8, 1);
        if (*(float *)(obj + 0xC4) > 0.5f) {
            FUN_L00_0025f8e0(a, *(float *)(obj + 0xC4) * 0.02f + 0.01f);
            FUN_L00_0025f8e0(b, *(float *)(obj + 0xC4) * 0.02f + 0.01f);
            FUN_L00_0025f8e0(c, *(float *)(obj + 0xC4) * 0.01f + 0.01f);
            vel[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 2.0f) *
                       frame_time) *
                     s;
            FUN_L01_0028a7a8(moby, a, vel, &D_L13_001613D8, 1);
            vel[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 3.0f) *
                       frame_time) *
                     s;
            FUN_L01_0028a7a8(moby, b, vel, &D_L13_001613E8, 1);
            vel[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 4.0f) *
                       frame_time) *
                     s;
            FUN_L01_0028a7a8(moby, c, vel, &D_L13_001613F8, 1);
            vel[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 2.0f) *
                       frame_time) *
                     s;
            FUN_L01_0028a7a8(moby, a, vel, &D_L13_00161408, 1);
            vel[0] = -((random_float_between(6.0f, 7.0f) + *(float *)(obj + 0xC4) * 3.0f) *
                       frame_time) *
                     s;
            FUN_L01_0028a7a8(moby, pos, vel, &D_L13_00161418, 1);
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
void FUN_L13_002b89f8(char *rot, char *from, char *moby, int icon, long color, int tex) {
    float turn[4];
    float mat[16];
    float delta[4];
    float pos[4];
    float len;
    int x, y;
    IconEC5D0 *t;

    qzero(turn);
    turn[2] = *(float *)(rot + 8);
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(mat, turn);
    subtract_vector_xyz(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(pos, delta, mat);
    scale_vector_xyz(pos, pos, 0.0714285746f);
    len = vector_length_xyz(pos);
    x = func_001FA898_r(pos[0]);
    y = func_001FA898_r(-pos[1]);
    if (len < 46.0f) {
        if (len > 38.0f) {
            float f;
            len = 46.0f - len;
            f = convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * len;
            color &= 0xFFFFFF;
            color = (func_001FA898_r(f * 0.125f) << 24) | color;
        }
        t = &D_L13_001CBDB8[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - t->x, y - t->y, t->w, t->h, t->u, t->v, t->w, t->h, color,
                           get_effect_texture(tex + 0x28 + t->tex));
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
extern void FUN_L11_00311210(void *, void *, void *, int);
extern void FUN_L13_002b7d50(void *, int, int, long, float, float, float, float);
extern void FUN_L13_002b89f8(char *rot, char *from, char *moby, int icon, long color, int tex);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void project_to_screen(f32 *, void *) __asm__("func_001F2070");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

int FUN_L13_002b8be0(int a, char *p, char *radar_origin_position, void *radar_rotation, float f0, float f1, float f2, char *cur, char *g) {
    float v[4];
    float mat[16];
    float u[4];
    int sx, sy, tx, ty;
    char *m;
    char *ret;
    int best;
    int *scr;
    int dx, dy, r, d2;
    float ang, dist, h, k, s, t;
    char *tab;

    m = D_L13_0015FFE4;
    ret = 0;
    best = 0x7FFFFFFF;
    *(int *)(p + 0xE8) = 0;
    while (m != 0) {
        tab = D_L13_001CBD40;
        scr = (int *)(D_0013E500);
        if ((((Moby_2B9E50 *)m)->f34 & 0x1000) && !(((Moby_2B9E50 *)m)->f34 & 3) && *(unsigned char *)(m + 0x31) != 0 &&
            m != 0 && *(char **)(m + 0x24) != 0 && *(short *)(*(char **)(m + 0x24) + 0x46) == 5) {
            FUN_L11_00311210(m + 0x10, &sx, &sy, 0);
            dx = sx - *(int *)(p + 0xE0);
            dy = sy - *(int *)(p + 0xE4);
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;
            qcopy(v, D_L13_001672E0);
            scale_vector_xyz(v, D_L13_001672E0, *(float *)(m + 0xC) / 1280.0f);
            add_vector_xyz(v, v, m + 0x10);
            FUN_L11_00311210(v, &tx, &ty, 0);
            tx -= sx;
            ty -= sy;
            r = func_001FA898_r(seg_sqrt(convert_integer_to_float(tx * tx + ty * ty))) + 0x1C;
            if (dx < r && dy < r) {
                if (cur != 0) {
                    if (m == cur) {
                        best = 0;
                        ret = m;
                    }
                } else if (m != g) {
                    d2 = dx * dx + dy * dy;
                    if (d2 < best) {
                        best = d2;
                        ret = m;
                    }
                }
            }
        }
        switch (*(short *)(m + 0xA6)) {
        case 0xE0:
        case 0x4C2:
            FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 2, 0x75808080, *(int *)(p + 0x104));
            break;
        case 0xE4:
        case 0x4C4:
            FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 2, 0x75808080, *(int *)(p + 0x104));
            break;
        case 0x52:
            FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 1, 0x75004080, *(int *)(p + 0x104));
            break;
        case 0x53:
            FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 1, 0x75007080, *(int *)(p + 0x104));
            break;
        case 0x184:
        case 0x191:
        case 0x3EB:
        case 0x504:
            project_to_screen(v, m + 0x10);
            if (*(short *)(m + 0xA6) != 0x184) {
                if (*(unsigned char *)(m + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        FUN_L13_002b7d50(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f, ang);
                    }
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        FUN_L13_002b7d50(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f, -ang);
                    }
                }
            }
            if (*(short *)(m + 0xA6) != 0x191) {
                if (*(unsigned char *)(m + 0x31) == 0) {
                    char *np = m + 0x10;
                    int c;
                    transpose_rotation_matrix(mat, D_L13_001672D0);
                    subtract_vector_xyz(u, np, D_0013F3D0);
                    transform_vector(u, u, mat);
                    u[0] = -u[1];
                    u[1] = -u[2];
                    u[2] = 0.0f;
                    normalize_vector_xyz(u, u, 160.0f);
                    ang = fast_add_rotations(fast_arc_tan(u[0], u[1]), 1.5707964f);
                    u[0] += 256.0f;
                    u[1] += 208.0f;
                    dist = distance_xyz((char *)a + 0x10, np);
                    h = absolute_float(fast_subtract_rotations(fast_arc_tan(*(float *)(m + 0x10) - *(float *)((char *)a + 0x10),
                                                                      *(float *)(m + 0x14) - *(float *)((char *)a + 0x14)),
                                                    *(float *)(p + 0x38)));
                    k = absolute_float(fast_subtract_rotations(-fast_arc_tan(vector_distance((char *)a + 0x10, np),
                                                                       *(float *)(m + 0x18) - *(float *)((char *)a + 0x18)),
                                                    *(float *)(p + 0x34)));
                    s = seg_sqrt(h * h + k * k);
                    t = fast_add_rotations(*(float *)(p + 0x114), 0.1f / (s / 3.1415927f));
                    *(float *)(p + 0x114) = t;
                    c = func_001FA898_r((fast_sin(t) + 1.4f) * 53.0f);
                    if (h < 0.7853982f && k < 0.7853982f) {
                        {
                            float x = (v[0] - scr[4]) * 0.0625f;
                            float y = (v[1] - scr[5]) * 0.0625f;
                            float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                            FUN_L13_002b7d50(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f, ang);
                        }
                        {
                            float x = (v[0] - scr[4]) * 0.0625f;
                            float y = (v[1] - scr[5]) * 0.0625f;
                            float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                            FUN_L13_002b7d50(tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f, -ang);
                        }
                    } else {
                        FUN_L13_002b7d50(&D_L13_001613C8, 3, 0xFFFFF3, (c << 24) | 0x8000, u[0], u[1], 240.0f / dist + 1.5f, ang);
                    }
                }
                FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 0, 0x75808080, *(int *)(p + 0x104));
                if (*(short *)(m + 0xA6) != 0x191) {
                    *(int *)(p + 0xE8) = *(int *)(p + 0xE8) + 1;
                }
            }
            break;
        case 0x6F:
        case 0x4BE:
            if (!(*(unsigned short *)(m + 0x34) & 1)) {
                project_to_screen(v, m + 0x10);
                if (*(unsigned char *)(m + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        FUN_L13_002b7d50(D_L13_001CBD40, 0x1E, 0xFFFFF3, 0xFF00FF00, x, y, 1.0f, -ang);
                    }
                }
                FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 1, 0x75808080, *(int *)(p + 0x104));
            }
            break;
        case 0x4BF:
            if (!(*(unsigned short *)(m + 0x34) & 1) && *(unsigned char *)(m + 0x20) >= 2) {
                project_to_screen(v, m + 0x10);
                if (*(unsigned char *)(m + 0x31) != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        FUN_L13_002b7d50(D_L13_001CBD40, 0x1E, 0xFFFFF3, 0xFF00FF00, x, y, 1.0f, -ang);
                    }
                }
                FUN_L13_002b89f8(radar_rotation, radar_origin_position, m, 1, 0x75808080, *(int *)(p + 0x104));
            }
            break;
        case 0x185: case 0x186: case 0x187: case 0x188:
        case 0x189: case 0x18A: case 0x18B: case 0x18C:
        case 0x18F: case 0x190:
        case 0x534: case 0x535: case 0x536: case 0x537:
        case 0x538: case 0x539: case 0x53A:
            break;
        }
        m = *(char **)(m + 0x28);
    }
    return (int)ret;
}
#include "qcopy.h"

/* Per-frame target tracking: validates the current target, re-picks one and drops it when lost. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BA828), where it is exact; names translated to the US level program. */

extern char D_L13_001670D0[];
extern int D_L13_0015F5C4;
extern char D_0013C940[];
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
int FUN_L13_002b8be0(int a, char *p, char *a2, void *a3, float f0, float f1, float f2, char *cur, char *g);
extern void tick_countdown_32(int *arg0) __asm__("FUN_001f9740");

#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(char **)((char *)(p) + (o)))
#define H(p, o) (*(short *)((char *)(p) + (o)))
#define B(p, o) (*(unsigned char *)((char *)(p) + (o)))
#define FLAG    hero.ship_ammo

void FUN_L13_002b95b8(int a, char *p) {
    char *g = 0;
    char *pad;
    char *t;
    char *r;
    float v[4];
    if (!(D_L13_0015F5C4 >= 3 && D_L13_0015F5C4 <= 4)) {
        tick_countdown_32((int *)(p + 0x8C));
    }
    if (W(p, 0x8C) != 0 && P(p, 0x88) != 0 && H(P(p, 0x88), 0xA6) == W(p, 0x124) &&
        B(P(p, 0x88), 0x20) != 0xFE && B(P(p, 0x88), 0x20) != 0xFD) {
        pad = D_0013C940;
        if (B(P(p, 0x88), 0x31) != 0) {
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
        g = P(p, 0x88);
    }
    W(p, 0xEC) = 0;
    W(p, 0x124) = -1;
    W(p, 0x88) = 0;
skip:
    qcopy(v, D_L13_001670D0);
    r = (char *)FUN_L13_002b8be0(a, p, D_L13_001670D0 - 0x10, v, 0.19634955f, 0.19634955f, 255.0f,
                                 P(p, 0x88), g);
    if ((r != P(p, 0x88) && W(p, 0x8C) >= 0x1F5) || FLAG == 0) {
        W(p, 0xEC) = 0;
        W(p, 0x124) = -1;
        W(p, 0x88) = 0;
    }
    if (r != 0 && (P(p, 0x88) == 0 || (H(P(p, 0x88), 0xA6) != 0x3EB && H(r, 0xA6) == 0x3EB)) &&
        FLAG != 0) {
        P(p, 0x88) = r;
        W(p, 0x124) = H(r, 0xA6);
        W(p, 0x8C) = W(p, 0x138);
    }
    if (r != P(p, 0x88)) {
        W(p, 0x118) = W(p, 0x118) + 1;
        if (scale_game_frames(0x3C) < W(p, 0x118)) {
            W(p, 0xEC) = 0;
            W(p, 0x124) = -1;
            W(p, 0x88) = 0;
        }
    } else {
        W(p, 0x118) = 0;
    }
    t = P(p, 0x88);
    if (t != 0 && W(p, 0x84) == 0) {
        if (H(t, 0xA6) != W(p, 0x124) || B(t, 0x20) == 0xFE || B(t, 0x20) == 0xFD) {
            W(p, 0x88) = 0;
            W(p, 0x124) = -1;
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
extern void FUN_L11_00310ad0(float, float, float, void *, int, int, long);
extern void FUN_L11_00311210(void *, void *, void *, int);
extern void append_rotated_sprite_quad(float, float, float, float, float, int, int, int, int, int, int, int, float, float);
extern void draw_textured_quad_local(int, int, int, int, int, int, int, int, long, long) __asm__("FUN_001f5450");
void FUN_L13_002b8320(unsigned char a, unsigned char b, unsigned char c, unsigned char d, float x, float y, float s, float ang);
void FUN_L13_002b95b8_c(int a, char *p) __asm__("FUN_L13_002b95b8");
extern long func_001F4868_l(int) __asm__("FUN_001f44b8");
extern void func_L13_002B9590_f(float, float, float, float, unsigned char, unsigned char, unsigned char, unsigned char) __asm__("FUN_L13_002b8320");
extern void *func_001FE540_id(int) __asm__("FUN_001fdd10");
extern int font_print_center_small(int, int, long, void *, int) __asm__("FUN_001f6b88");

void FUN_L13_002b97f8(char *moby) {
    char *d = *(char **)(moby + 0x78);
    char *s;
    unsigned char *base;
    unsigned char *g;
    unsigned char *h;
    unsigned char *b;
    int sp10;
    int sp14;
    int tex;
    int i;
    unsigned char n;
    unsigned char c;
    int j;
    int x;
    int y;
    int ex;
    int ey;
    int a;
    int v;
    int lo;
    int hi;
    int idx;
    float t;
    float k;
    float m;
    float f;
    float q;

    FUN_L13_002b95b8_c((int)moby, d);
    if (*(int *)(d + 0x88) != 0) {
        FUN_L11_00311210(*(char **)(d + 0x88) + 0x10, &sp10, &sp14, 0);
        if (*(int *)(d + 0x8C) > scale_game_frames_alt(0x1F4)) {
            t = convert_integer_to_float(*(int *)(d + 0x8C) - scale_game_frames_alt(0x1F4));
            t = t / convert_integer_to_float(scale_game_frames_alt(0x4B));
            m = 2.0f * (1.0f - t);
            k = t * 5.0f + 1.0f;
            if (m > 1.0f) m = 1.0f;
            c = func_001FA898_r(m * 96.0f);
            func_L13_002B9590_f(convert_integer_to_float(sp10), convert_integer_to_float(sp14), k, normalize_angle(convert_integer_to_float(frame_count) / 30.0f), 0, 0xFF, 0, c);
            *(int *)(d + 0xEC) = 0;
        } else {
            a = 0xFF;
            if ((*(int *)(d + 0x8C) / scale_game_frames_alt(0x14)) & 1) a = 0;
            func_L13_002B9590_f(convert_integer_to_float(sp10), convert_integer_to_float(sp14), 1.0f, 0.0f, 0xFF, a, 0, 0x60);
            *(int *)(d + 0xEC) = *(int *)(d + 0x88);
        }
    }
    y = 0x40;
    x = 0x18;
    base = D_0013F350;
    n = base[0x15F7];
    for (i = 0; i < n; i++) {
        g = D_0013F350;
        if (i < g[0x15F6]) {
            FUN_L11_00310ad0(x, y, 0.5f, D_L13_001CBC60, 0x19, 0xFFFFF3, 0x50008F00);
        } else {
            FUN_L11_00310ad0(x, y, 0.5f, D_L13_001CBC60, 0x19, 0xFFFFF3, 0x20004F00);
        }
        n = g[0x15F7];
        if (i == n >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }
    ex = *(int *)(d + 0xE0);
    ey = *(int *)(d + 0xE4);
    tex = get_effect_texture_c(0x11);
    append_rotated_sprite_quad(ex, ey, 40.0f, 40.0f, 0.0f, 0x3F, 0x3F, tex, 0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    append_rotated_sprite_quad(ex, ey, 40.0f, 40.0f, 0.0f, 0x3F, 0x3F, get_effect_texture_c(0x12), 0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    append_rotated_sprite_quad(ex, ey, 10.0f, 10.0f, 0.0f, 0x1F, 0x1F, get_effect_texture_c(8), 0xFFFFF3, 0xFF20FF20, 0, 0, 0.5f, 0.5f);
    b = D_0013F350;
    if (*(float *)(b + 0x15FC) < *(float *)(b + 0x1600) / 10.0f && (frame_count / 90) & 1) {
        font_print_center_small(0x100, 0x186, 0x80000080L, func_001FE540_id(0x5269), 0x64);
    }
    h = D_0013F350;
    f = *(float *)(h + 0x15FC) - *(float *)(d + 0xF0);
    f *= 0.2f;
    q = absolute_float(f);
    if (q > 1.0f) f /= q;
    *(float *)(d + 0xF0) += f;
    v = func_001FA898_r(*(float *)(d + 0xF0) * 251.0f / *(float *)(h + 0x1600)) + 2;
    if (v > 0xFD) v = 0xFD;
    if (v < 2) v = 2;
    lo = *(int *)(d + 0xF4) <= v ? *(int *)(d + 0xF4) : v;
    hi = *(int *)(d + 0xF4) < v ? v : *(int *)(d + 0xF4);
    for (j = lo; j <= hi; j++) {
        idx = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < v) {
            hp_bar_clut[0][idx] = D_L13_001CBE70[idx];
        } else {
            hp_bar_clut[0][idx] = 0x80000000;
        }
    }
    s = (char *)D_0013E500;
    draw_textured_quad_local(0x170, *(int *)(s + 4) - 0x90, 0x80, 0x80, 0, 0, 0x80, 0x80, 0x70808080, func_001F4868_l(*(int *)(d + 0x104) + 0x28));
    draw_textured_quad_local(0x170, *(int *)(s + 4) - 0x90, 0x80, 0x80, 0, 0, 0x80, 0x80, 0x70808080, func_001F4868_l(*(int *)(d + 0x104) + 0x29));
    *(int *)(d + 0xF4) = v;
}
/* Builds a rotated offset vector from the moby, flips a facing flag on a pad press, and updates the aim. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BB068), where it is exact; names translated to the US level program. */

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int FUN_L11_00308848(void *, void *, void *, float, float);
extern unsigned char D_0013A4E0[];
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L11_00311210(void *, void *, void *, int);
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");

void FUN_L13_002b9df8(struct Moby *moby, char *obj, float p2, float p3) {
    char a[16];
    char b[16];
    char c[16];
    char d[16];
    build_spherical_offset(a, frame_time * 400.0f, p2, p3);
    fast_vec_add(a, a, obj);
    if (fast_dec_timer(obj + 0x80) && (*(int *)(D_0013A4E0 + 0x2610) & 0x84)) {
        allocate_voice_for_target_entry(3, 0, (int)moby);
        fast_vec_normalize(c, a, 2.2f);
        moby_point_position(moby, 3 + (obj[0x61] & 1), b);
        fast_vec_add(b, b, c);
        FUN_L11_00308848(moby, a, b, 200.0f, -1.0f);
        *(int *)(obj + 0x80) = scale_game_frames(4);
        obj[0x61] ^= 1;
    }
    fast_vec_normalize(a, a, 23.0f);
    fast_vec_add(d, a, &moby->pos);
    FUN_L11_00311210(d, obj + 0xE0, obj + 0xE4, 0);
}
/* Fires a timed burst from a moby while a pad button is held and ammo remains. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002B2020.c: func_L13_002BB1C8), where it is exact; names translated to the US level program. */

extern int FUN_L13_002e6a58(void *, void *, float, int, void *, int);

void FUN_L13_002b9f58(struct Moby *moby, char *obj) {
    if (fast_dec_timer(obj + 0x84) && (*(int *)(D_0013A4E0 + 0x2610) & 0x28)) {
        unsigned char *u = D_0013E533 + 0xE1D;
        if (u[0x15F6] != 0) {
            char a[16];
            char b[16];
            float s;
            int h;
            char *pos = &moby->rot;
            short t = (*(unsigned short *)(obj + 0x6A) + 1) & 3;
            *(short *)(obj + 0x6A) = t;
            moby_point_position(moby, t + 7, a);
            fast_vec_normalize(b, obj, 0.3f);
            fast_vec_add(a, a, b);
            s = frame_time * 100.0f;
            h = scale_game_frames(300);
            if (FUN_L13_002e6a58(moby, a, s, *(int *)(obj + 0xEC), pos, h) != 0) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                u[0x15F6]--;
                *(int *)(obj + 0x84) = scale_game_frames(30);
            }
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L13_002ba178.s", FUN_L13_002ba178);
#else
#endif

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
extern float D_L13_0015ED60;
extern int D_L13_0015ED84;
extern float D_L13_0016CDF0 __attribute__((section(".data")));
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
extern void jet_reflect_vector(void *,void *,void *) __asm__("FUN_L00_001ff660");
extern float jet_angle(float,float) __asm__("FUN_001f9e90");
extern float jet_sub_rot(float,float) __asm__("FUN_001fa5c8");
extern float jet_add_rot(float,float) __asm__("FUN_001fa580");
extern float jet_length_xy(void *) __asm__("FUN_001f9b20");
extern int jet_scale_ticks(int) __asm__("FUN_001f96f8");
extern void jet_explosion(void *, void *, void *, float, float, int, int, int,
                          float, float, float, int, float, float, int, int, int, int)
    __asm__("FUN_L00_0025e450");
extern void jet_group_state(int,int,int,int) __asm__("FUN_L13_002650a8");
extern void jet_group_switch(void *,int,int,int) __asm__("FUN_L13_002651c8");
extern void jet_group_flag(int,int) __asm__("FUN_L13_00265170");
extern float jet_distance(void *,void *) __asm__("FUN_L00_00250430");
extern float jet_blend(float *,float,float) __asm__("FUN_00213ed8");
extern void jet_animate(void *,float) __asm__("FUN_L00_00259fe8");
extern void jet_place_camera(void *,void *,int,int) __asm__("FUN_L00_00216f90");
extern void jet_mount(void *,void *,int,int,int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L13_002ba178(void *,void *);
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
void fighter_jet_update(Jet *m) {
    JetData *d;
    JetVec impulse;
    int state,initial_state;
    if (((JetPlayer*)(&jet_player47))->state==0x72) return;
    d=m->data; d->hit_class=0;
    jet_fast_dec_timer((int*)&d->timer);
    initial_state=m->state;
    if ((unsigned int)(initial_state-6)>=2 && (unsigned char)initial_state!=1 && (unsigned char)initial_state!=2) {
        JetHit *hit=(JetHit*)jet_find_hit(m,0x30001,0);
        if (hit && m->state==4) {
            if (((JetPlayer*)(&jet_player47))->health>hit->damage) {
                ((JetPlayer*)(&jet_player47))->health-=hit->damage;
                if (hit->source && (unsigned int)(hit->source->class_id-0x52)<2) d->hit_class=(short)hit->source->class_id;
                if (hit->flags&1) {
                    float weight,yaw_delta,pitch_delta;
                    jet_camera47.flash=1.0f; jet_camera47.ticks=jet_scale_ticks(4);
                    weight=hit->damage*0.05f;
                    build_spherical_offset(impulse,d->speed,d->angles[2],-d->angles[1]);
                    jet_scale_vector(hit->velocity,hit->velocity,frame_time*3.0f);
                    jet_add_vector(m->position,m->position,hit->velocity);
                    jet_reflect_vector(impulse,impulse,hit->velocity);
                    yaw_delta=jet_sub_rot(jet_angle(impulse[0],impulse[1]),d->angles[2]);
                    d->angles[2]=jet_add_rot(d->angles[2],yaw_delta*(D_L13_0015ED60*0.1f)*weight);
                    pitch_delta=jet_sub_rot(jet_angle(jet_length_xy(impulse),impulse[2]),d->angles[1]);
                    d->angles[1]=jet_add_rot(d->angles[1],pitch_delta*(D_L13_0015ED60*0.1f)*weight);
                }
            } else {
                build_spherical_offset(impulse,d->speed,d->angles[2],-d->angles[1]);
                if (hit->source && (unsigned int)(hit->source->class_id-0x52)<2) d->hit_class=(short)hit->source->class_id+10000;
                jet_camera47.flash=0.2f; jet_camera47.ticks=jet_scale_ticks(5);
                ((JetPlayer*)(&jet_player47))->health=0.0f; m->flags|=0x41;
                jet_explosion(m,impulse,0,0.0f,0.0f,10,3,16,4.0f,2.0f,9.0f,6,1.0f,15.0f,1,1,-1,0);
                m->animation=0; d->death_timer=jet_scale_ticks(240); m->state=6;
            }
        }
    }
    m->status=0xFF;
    if (((JetPlayer*)(&jet_player47))->craft==m) ((JetPlayer*)(&jet_player47))->health_meter=(int)(((JetPlayer*)(&jet_player47))->health*0.00390625f*200.0f);
    switch(m->state) {
    case 0: {
        float limit;
        int *palette;
        JetTexture *texture_table;
        unsigned int palette_data;
        int texture_index,initialized;
        m->state=1; d->animation_frame=0; m->scale=m->model->scale;
        *(float*)((char*)d+0x20)=d->angles[0]; *(float*)((char*)d+0x24)=d->angles[1];
        limit=jet_angle(1.0f,D_L13_0016CDF0); limit+=limit;
        d->counterC0=0; d->hit_class=0; *(float*)((char*)d+0xAC)=limit; *(float*)((char*)d+0xB0)=limit;
        qcopy(d->angles,m->rotation); qcopy(d->spawn_rotation,m->rotation); qcopy(d->spawn_position,m->position);
        texture_index=d->texture_id;
        texture_table=D_L13_0016D480;
        d->blend=0.0f;
        texture_index+=40;
        palette_data=D_L13_0015F420;
        initialized=hp_bar_saved;
        palette=(int*)(palette_data+(texture_table[texture_index].palette<<4));
        hp_bar_clut[0]=palette;
        if (!initialized) __builtin_memcpy(D_L13_001CBE70,palette,1024);
        hp_bar_saved=1; d->wait_timer=1; d->energy=1;
        if (m->persistent!=0xFF && (visited52)[m->persistent+(D_L13_0015ED84<<4)]==0xFF) m->state=9;
        if (d->group_a>0) jet_group_state(d->group_a,0,0,0);
        if (d->group_b>0) jet_group_state(d->group_b,0,0,0);
        jet_group_switch(D_L13_001CBDE8,1,1,-1); jet_group_switch(&D_L13_00161440,1,-1,-1); jet_group_flag(0x102,0);
        d->lock_index=-1; d->start_timer=jet_scale_ticks(575);
        break;
    }
    case 1: {
        short enter=0;
        if (m->persistent!=0xFF && (visited52)[m->persistent+(D_L13_0015ED84<<4)]==0xFF) m->state=9;
        else if (d->trigger!=-1 && jet_check_trigger(D_0013F3D0,d->trigger)) { d->blend=0.99f; D_L13_0015F3FC=0.99f; enter=1; }
        else if (jet_distance(m,((JetPlayer*)(&jet_player47))->moby)<2.0f) {
            int pressed=jet_try_help(7,0x53E4)!=0;
            if (*(int*)(D_0013A4E0+0x2604)&0x10) enter=pressed;
        }
        if (enter) {
            float limit;
            m->scale=m->model->scale;
            *(float*)((char*)d+0x20)=d->angles[0]; *(float*)((char*)d+0x24)=d->angles[1];
            limit=jet_angle(1.0f,D_L13_0016CDF0); limit+=limit;
            d->counterC0=0; d->hit_class=0; *(float*)((char*)d+0xAC)=limit; *(float*)((char*)d+0xB0)=limit;
            qcopy(d->angles,m->rotation); d->wait_timer=1; d->energy=1; m->state=2; d->lock_index=-1;
        } else if (d->blend!=0.0f) { jet_blend(&d->blend,0.0f,frame_time*4.0f); D_L13_0015F3FC=d->blend; }
        jet_animate((unsigned char*)m,1.5f);
        break;
    }
    case 2:
        jet_blend(&d->blend,1.0f,frame_time*4.0f); D_L13_0015F3FC=d->blend;
        if (d->blend==1.0f) {
            if (d->entry_path!=-1) {
                JetPath *path=&D_L13_001600EC[d->entry_path];
                jet_place_camera(path->position,path->rotation,50,1);
                {
                    int index=d->entry_path;
                    JetPath *paths=D_L13_001600EC;
                    qcopy(m->position,paths[index].position); qcopy(m->rotation,paths[index].rotation);
                }
            }
            jet_set_hero_state(0x32,1);
            jet_mount(m->position,m->rotation,0,0,0);
            d->missile_index=0; d->counter6C=0; d->counter70=0; d->missile_timer=0; d->speed=D_L13_0015ED60*0.1f; d->gun_timer=0;
            m->alpha=m->render_flags=0xFF; *(int*)((char*)((JetPlayer*)(&jet_player47))->moby+0x98)=-1;
            qcopy(d->previous_rotation,m->rotation);
            d->angle_a=0; d->angle_b=0; d->angle_c=0; d->angle_d=0; d->angle_e=0; d->angle_f=0; ((JetPlayer*)(&jet_player47))->craft=m; { unsigned short cls=m->class_id; ((JetPlayer*)(&jet_player47))->health=255.0f; ((JetPlayer*)(&jet_player47))->ammo=10; ((JetPlayer*)(&jet_player47))->pad160E=1; ((JetPlayer*)(&jet_player47))->ammo2=20; ((JetPlayer*)(&jet_player47))->class_id=cls; ((JetPlayer*)(&jet_player47))->ammo3=20; d->hud_previous=255; d->hud_health=0; ((JetPlayer*)(&jet_player47))->ammo4=3; ((JetPlayer*)(&jet_player47))->ammo5=3; ((JetPlayer*)(&jet_player47))->ammo6=3; ((JetPlayer*)(&jet_player47))->pad160F=0; ((JetPlayer*)(&jet_player47))->max_health=256.0f; ((JetPlayer*)(&jet_player47))->health_scale=100.0f; }
            build_spherical_offset(d,D_L13_0015ED60*0.1f,d->angles[2],-d->angles[1]);
            if (d->group_a>0) jet_group_state(d->group_a,1,1,1);
            if (d->group_b>0) jet_group_state(d->group_b,1,1,1);
            jet_group_switch(D_L13_001CBDE8,0,0,-1); jet_group_switch(&D_L13_00161440,0,-1,-1); jet_group_flag(0x102,0);
            m->position[2]+=3.0f; m->scale=m->model->scale*0.25f; ((JetPlayer*)(&jet_player47))->in_craft=1;
            if (d->engine_sound==-1) d->engine_sound=jet_allocate_voice(0,4,(int)m);
            D_L13_0015F608=2; FUN_L13_002ba178(m,d); jet_camera_mode(); jet_game_state(2,4); m->state=4;
        }
        break;
    case 4: {
        unsigned char *audio;
        if (d->blend!=0.0f) { jet_blend(&d->blend,0.0f,frame_time*4.0f); D_L13_0015F3FC=d->blend; }
        jet_force_help(6,0); D_L13_0015F608=2;
        audio=D_0013E550+d->engine_sound*0x70;
        if (*(Jet**)(audio+0x88)!=m || !audio[0x74]) { d->engine_sound=-1; d->engine_sound=jet_allocate_voice(0,4,(int)m); }
        FUN_L13_002ba178(m,d);
        if (((JetPlayer*)(&jet_player47))->pad160F&1) m->state=5;
        if (d->energy<=0) { d->wait_timer=jet_scale_ticks(1200); m->state=8; }
        break;
    }
    case 5:
        ((JetPlayer*)(&jet_player47))->in_craft=0; *(int*)((char*)((JetPlayer*)(&jet_player47))->moby+0x98)=0;
        jet_set_hero_state(0,1); jet_end_mount(0); jet_set_weapon(((JetPlayer*)(&jet_player47))->weapon,0);
        qcopy(m->position,d->spawn_position); qcopy(m->rotation,d->spawn_rotation);
        m->scale=m->model->scale; jet_update_moby(m);
        if (d->group_a>0) jet_group_state(d->group_a,0,0,0);
        if (d->group_b>0) jet_group_state(d->group_b,0,0,0);
        jet_group_switch(D_L13_001CBDE8,1,1,-1); jet_group_switch(&D_L13_00161440,1,-1,-1); jet_group_flag(0x102,0);
        if (d->exit_path!=-1) { JetPath *path=&D_L13_001600EC[d->exit_path]; jet_place_camera(path->position,path->rotation,0,1); }
        else jet_place_camera(d->spawn_position,D_L13_001600EC[d->destination_path].rotation,0,1);
        {
            int id=d->engine_sound;
            unsigned char *audio;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->engine_sound=-1;
            id=d->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->weapon_sound=-1;
        }
        jet_game_state(0,5); d->blend=0.99f; D_L13_0015F3FC=0.99f; m->state=1;
        break;
    case 6:
        D_L13_0015F608=2;
        {
            int id=d->engine_sound;
            unsigned char *audio;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->engine_sound=-1;
            id=d->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->weapon_sound=-1;
        }
        m->state=7; break;
    case 7: {
        JetTransition *transition;
        D_L13_0015F608=2;
        if (jet_fast_dec_death_timer(&d->death_timer)) {
            transition=(JetTransition*)(D_001516D0);
            if (!transition->pending) {
            int active=transition->active;
            if (active==-1) {
                jet_death_complete(); jet_end_mount(0);
                qcopy(m->position,d->spawn_position); qcopy(m->rotation,d->spawn_rotation); jet_update_moby(m);
                {
            int id=d->engine_sound;
            unsigned char *audio;
            if (id!=active) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->engine_sound=-1;
            id=d->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->weapon_sound=-1;
        }
                m->state=0;
            }
        }
        }
        break;
    }
    case 8:
        ((JetPlayer*)(&jet_player47))->in_craft=0; *(int*)((char*)((JetPlayer*)(&jet_player47))->moby+0x98)=0; jet_set_hero_state(0,1); jet_end_mount(0);
        if (d->escape_path==-1) { stub_printf(D_L13_001F4470); qcopy(m->position,d->spawn_position); qcopy(m->rotation,d->spawn_rotation); }
        else { int index=d->escape_path; JetPath *paths=D_L13_001600EC; qcopy(m->position,paths[index].position); qcopy(m->rotation,paths[index].rotation); m->position[2]+=0.57318f; }
        m->scale=m->model->scale; jet_update_moby(m);
        if (d->destination_path==-1) stub_printf(D_L13_001F44A8);
        else {
            JetPath *path;
            if (d->group_a>0) jet_group_state(d->group_a,0,0,0);
            if (d->group_b>0) jet_group_state(d->group_b,0,0,0);
            jet_group_switch(D_L13_001CBDE8,1,1,-1); jet_group_switch(&D_L13_00161440,1,-1,-1); jet_group_flag(0x102,0);
            path=&D_L13_001600EC[d->destination_path]; jet_place_camera(path->position,path->rotation,0,1);
        }
        if (m->persistent!=0xFF) jet_clear_persistent(m->persistent);
        {
            int id=d->engine_sound;
            unsigned char *audio;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->engine_sound=-1;
            id=d->weapon_sound;
            if (id!=-1) { audio=D_0013E550+id*0x70; if (*(Jet**)(audio+0x88)==m && audio[0x74]) jet_stop_sound(id); }
            d->weapon_sound=-1;
        }
        jet_reset_camera((char *)&jet_player47+0x80,(char *)&jet_player47+0x90); m->state=9;
        break;
    case 9: jet_delete_moby(m); return;
    case 10: break;
    default: break;
    }
    if (m->position[0]<15.0f) m->position[0]=15.0f;
    if (m->position[0]>1008.0f) m->position[0]=1008.0f;
    if (m->position[1]<15.0f) m->position[1]=15.0f;
    if (m->position[1]>1008.0f) m->position[1]=1008.0f;
    if (m->position[2]<15.0f) m->position[2]=15.0f;
    if (m->position[2]>1008.0f) m->position[2]=1008.0f;
    state=m->state;
    if ((unsigned char)state==4) { qcopy_nc((char *)&jet_player47+0x80,m->position); qcopy_nc((char *)&jet_player47+0x90,d->angles); }
    if ((unsigned int)(state-3)<2 || (unsigned char)state==8) FUN_L13_002b8590(m,(char*)d);
}
#ifdef NON_MATCHING
#include "rnc/input/pad_state.h"
extern float vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern float FUN_L00_0025bc98(float *, float, float *, int, float, float, float);
extern float FUN_L13_002ba088(int, int, float, float, float, float, float);
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void update_view_context(void) __asm__("FUN_001f2d98");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_L13_00204dc8");
extern float D_00141070_2b6140 __asm__("D_00141070") __attribute__((section(".data")));
extern float D_00141074_2b6140 __asm__("D_00141074") __attribute__((section(".data")));
extern u64 D_L13_00161430 __attribute__((sda));
extern char *D_L13_00167100 __attribute__((section(".data")));
extern char D_L13_00174140[];
extern char D_L13_001F4450[];
#define F(p,o) (*(float *)((char *)(p)+(o)))

/* Flight motion, steering, camera and collision response for the mounted fighter. */
static inline float turn_angle(float distance, JetData *d, int axis, float angle, float limit) {
    return FUN_L13_002ba088(1, 5, distance, d->angles[axis], angle, 50.0f, limit);
}

void FUN_L13_002ba178(void *moby, void *obj) {
    Jet *m = moby;
    JetData *d = obj;
    JetVec old_position;
    JetVec impulse;
    JetVec velocity;
    JetVec cross;
    JetVec vertical;
    JetVec rotation;
    JetVec target;
    JetVec offset;
    JetVec offsets[2];
    JetVec camera_position;
    JetVec camera_rotation;
    JetVec matrix[3];
    float pitch_limit, yaw_limit;
    float distance, angle, height, speed_fraction;
    float x, y;
    struct JetInputAxes { char pad0[0x1D20]; float x, y; };
    struct JetCollision { char pad0[0x18]; char *hit; char pad1C[0x14]; JetVec position, normal, cross, vertical; };
    int id;
    short cls;
    *(u128 *)velocity = 0;
    *(u128 *)vertical = 0;
    vertical[2] = 1.0f;
    velocity[2] = frame_time * 8.0f;
    m->flags &= 0xFEFF;
    *(u128 *)impulse = *(u128 *)velocity;
    if (B(m,0xBC) == 0) {
      if (d->missile_index > scale_game_frames(120)) {
        if (((struct PadState *)D_0013C940)->raw_pressed & 0x40) {
            id = d->weapon_sound;
            if (id != -1) {
                unsigned char *audio = D_0013E550 + id*0x70;
                if (*(Jet **)(audio+0x88)==m && audio[0x74])
                    release_voice_slot(id);
            }
            d->weapon_sound = -1;
            d->weapon_sound = allocate_voice_for_target_entry(1,4,(int)m);
        } else if (((struct PadState *)D_0013C940)->raw_released & 0x40) {
            id = d->weapon_sound;
            if (id != -1) {
                unsigned char *audio = D_0013E550 + id*0x70;
                if (*(Jet **)(audio+0x88)==m && audio[0x74])
                    release_voice_slot(id);
            }
            d->weapon_sound = -1;
        }
        if (((struct PadState *)D_0013C940)->raw_held&0x40) {
            d->speed += frame_scale*0.035f;
            if (frame_time*30.0f < d->speed) d->speed = frame_time*30.0f;
        } else {
            float speed = frame_time*24.0f;
            float step = frame_scale*0.025f;
            if (d->speed < speed-step) d->speed += step;
            else if (speed < d->speed) d->speed += (speed-d->speed)*(frame_scale*0.1f);
            else d->speed = speed;
        }
    } else {
        float step = frame_scale*0.003f;
        d->missile_index++;
        d->speed += step;
    }
    } else {
        d->speed = 0.0001f;
    }
    if (d->missile_index > scale_game_frames(5)) {
        float x, y, turn_rate, pitch_rate;
        turn_rate = frame_scale*0.1f;
        pitch_rate = frame_scale*0.25f;
        x = ((struct JetInputAxes *)D_0013F350)->x;
        y = ((struct JetInputAxes *)D_0013F350)->y;
        if (W(D_0013A4E0,0x2600)&0x200) {
            float divisor = frame_scale*0.012271847f;
            float multiplier = frame_scale*0.015707964f;
            turn_rate = pitch_rate = frame_scale*0.08f;
            x /= divisor;
            y /= divisor;
            x *= multiplier;
            y *= multiplier;
        }
        vendor_spring(&F(d,0x6C),&F(d,0x90),x,frame_scale_sq*0.03f,frame_scale_sq,turn_rate);
        vendor_spring(&F(d,0x70),&F(d,0x94),y,frame_scale_sq*0.03f,frame_scale_sq,pitch_rate);
        FUN_L00_0025bc98(&d->angles[0],F(d,0x6C),&F(d,0x98),0,frame_scale_sq*0.05f,frame_scale_sq,frame_scale);
        d->angles[1] = fast_subtract_rotations(d->angles[1],F(d,0x70)*(frame_scale*0.012271847f));
        d->angles[2] = fast_subtract_rotations(d->angles[2],F(d,0x6C)*(frame_scale*0.012271847f));
        if (d->angles[1] > 1.3962634f) d->angles[1] = 1.3962634f;
        else if (d->angles[1] < -1.3962634f) d->angles[1] = -1.3962634f;
        if (m->rotation[1] > 1.3962634f) m->rotation[1] = 1.3962634f;
        else if (m->rotation[1] < -1.3962634f) m->rotation[1] = -1.3962634f;
    }
    {
    float pitch_limit, yaw_limit, distance, angle, height;
    pitch_limit = 310.0f; yaw_limit = 500.0f;
    id = W(d,0x110);
    if (id != -1) {
        qcopy_nc(target,D_L13_001600EC[id].position);
        pitch_limit = fast_vec_length((char *)&D_L13_001600EC[id]+0x20);
        add_vector_xyz(offset,&D_L13_001600EC[W(d,0x110)],(char *)&D_L13_001600EC[W(d,0x110)]+0x10);
        yaw_limit = fast_vec_length(offset);
    } else qcopy(target,&D_L13_00161430);
    distance = vector_distance(m->position,&D_L13_00161430);
    angle = angle_atan2(target[0]-m->position[0],target[1]-m->position[1]);
    d->angles[2] = turn_angle(distance, d, 2, angle, yaw_limit);
    angle = angle_atan2(target[0]-m->position[0],target[1]-m->position[1]);
    m->rotation[2] = FUN_L13_002ba088(1,5,distance,m->rotation[2],angle,50.0f,yaw_limit);
    height = absolute_float(target[2]-m->position[2]);
    distance = vector_distance(m->position,target);
    angle = angle_atan2(distance,target[2]-m->position[2]);
    d->angles[1] = FUN_L13_002ba088(1,5,height,d->angles[1],-angle,50.0f,pitch_limit);
    height = absolute_float(target[2]-m->position[2]);
    distance = vector_distance(m->position,target);
    angle = angle_atan2(distance,target[2]-m->position[2]);
    m->rotation[1] = FUN_L13_002ba088(1,5,height,m->rotation[1],-angle,50.0f,pitch_limit);
    }
    build_spherical_offset(velocity,d->speed,d->angles[2],-d->angles[1]);
    normalize_vector_xyz(d->pad10,velocity,1.0f);
    qcopy(old_position,m->position);
    if (!B(m,0xBC)) add_vector_xyz(m->position,m->position,velocity);
    height = d->speed-frame_time*24.0f;
    distance = frame_time*30.0f-frame_time*24.0f;
    if (height < 0.0f) speed_fraction = 0.0f/distance;
    else speed_fraction = height/distance;
    /* The Z component is replaced below before the offset is copied. */
    ((u64 *)camera_position)[1] = 0;
    *(u128 *)camera_rotation = 0;
    camera_position[0] = ((-15.0f-F(d,0x70))-speed_fraction*4.0f)*0.25f;
    camera_position[1] = F(d,0x6C)*-3.0f*0.25f;
    camera_position[2] = (F(d,0x70)+F(d,0x70)+4.0f)*0.25f;
    camera_rotation[0] = -1.275f;
    camera_rotation[2] = 0.65f;
    F(d,0xC4) = speed_fraction;
    *(u128 *)offsets[0] = *(u128 *)camera_position;
    *(u128 *)offsets[1] = *(u128 *)camera_rotation;
    if (m->state == 2) {
        F(d,0xB4)=offsets[d->animation_frame][0];
        F(d,0xB8)=offsets[d->animation_frame][1];
        F(d,0xBC)=offsets[d->animation_frame][2];
    } else {
        vendor_spring(&F(d,0xB4),&F(d,0xC8),offsets[d->animation_frame][0],frame_scale_sq*0.15f,frame_scale_sq*0.8f,frame_scale);
        vendor_spring(&F(d,0xB8),&F(d,0xCC),offsets[d->animation_frame][1],frame_scale_sq*0.15f,frame_scale_sq*0.8f,frame_scale);
        vendor_spring(&F(d,0xBC),&F(d,0xD0),offsets[d->animation_frame][2],frame_scale_sq*0.15f,frame_scale_sq*0.8f,frame_scale);
    }
    offset[0]=F(d,0xB4); offset[1]=F(d,0xB8); offset[2]=F(d,0xBC); offset[3]=0;
    euler_rotation_basis(matrix,d->angles);
    transform_vector_by_basis(target,offset,matrix);
    add_vector_xyz(camera_position,m->position,target);
    FUN_L00_002ea9d8(camera_position);
    camera_rotation[0]=fast_add_rotations(d->angles[0]*0.5f,F(d,0x6C)*0.03125f);
    angle=fast_add_rotations(d->angles[1],0.09817477f);
    camera_rotation[1]=fast_subtract_rotations(angle,F(d,0x70)*(frame_scale*0.012271847f));
    camera_rotation[2]=d->angles[2];
    if (m->state==2) qcopy(d->previous_rotation,camera_rotation);
    else {
        FUN_L00_0025bc98(&d->previous_rotation[0],camera_rotation[0],&F(d,0xA0),0,frame_scale_sq*0.15f,frame_scale_sq*0.8f,frame_scale);
        FUN_L00_0025bc98(&d->previous_rotation[1],camera_rotation[1],&F(d,0xA4),0,frame_scale_sq*0.15f,frame_scale_sq*0.8f,frame_scale);
        FUN_L00_0025bc98(&d->previous_rotation[2],camera_rotation[2],&F(d,0xA8),0,frame_scale_sq*0.15f,frame_scale_sq*0.8f,frame_scale);
    }
    rotation[0]=d->previous_rotation[0]; rotation[2]=d->previous_rotation[2]; rotation[1]=d->previous_rotation[1];
    FUN_L00_002eaa30(rotation);
    if (H(D_L13_00167100,0x86)!=5) {
        stub_printf(D_L13_001F4450);
        jet_mount(camera_position,rotation,0,0,0);
    }
    vendor_spring(&F(d,0xB0),&F(d,0xC0),speed_fraction*0.34906578f+1.134464f,0.05f,1.0f,1.0f);
    angle=F(d,0xB0)*0.5f;
    y=fast_sin(angle);
    y=y/fast_cos(angle);
    D_L13_0016CDF0=y;
    update_view_context();
    m->rotation[0]=fast_add_rotations(d->angles[0],F(d,0x6C)*0.5f);
    m->rotation[1]=fast_add_rotations(d->angles[1],-F(d,0x70)*0.5f);
    m->rotation[2]=fast_add_rotations(d->angles[2],-F(d,0x6C)*0.0f);
    if (coll_sphere(1.2f,m->position,0,m)) {
        char *hit=((struct JetCollision *)D_L13_00174140)->hit;
        if (!hit || ((cls=H(hit,0xA6))!=0 && cls!=0x192 && cls!=0x4C2 && cls!=0x4C3 && cls!=0x52 && cls!=0x53)) {
            qcopy(m->position,((struct JetCollision *)D_L13_00174140)->position);
            jet_reflect_vector(velocity,velocity,((struct JetCollision *)D_L13_00174140)->normal);
            angle=fast_subtract_rotations(angle_atan2(velocity[0],velocity[1]),d->angles[2]);
            d->angles[2]=fast_add_rotations(d->angles[2],angle*(frame_scale*0.1f));
            angle=angle_atan2(jet_length_xy(velocity),velocity[2]);
            angle=fast_subtract_rotations(-angle,d->angles[1]);
            d->angles[1]=fast_add_rotations(d->angles[1],angle*(frame_scale*0.1f));
            d->speed*=0.7f;
            if (((struct JetCollision *)D_L13_00174140)->hit) {
              if (!d->timer) {
                ((JetPlayer *)D_0013F350)->health-=5.0f;
                if (((JetPlayer *)D_0013F350)->health<0.0f) {
                    subtract_vector_xyz(cross,((struct JetCollision *)D_L13_00174140)->cross,((struct JetCollision *)D_L13_00174140)->vertical);
                    normalize_vector_xyz(cross,cross,1.0f);
                    jet_reflect_vector(impulse,velocity,((struct JetCollision *)D_L13_00174140)->normal);
                    normalize_vector_xyz(impulse,impulse,frame_time+frame_time);
                    normalize_vector_xyz(vertical,((struct JetCollision *)D_L13_00174140)->normal,1.0f);
                    jet_explosion(m,impulse,0,8.0f,100.0f,30,10,24,16.0f,8.0f,9.0f,6,1.0f,50.0f,1,1,-1,0);
                    d->death_timer=scale_game_frames(240); m->state=6;
                }
                d->timer=scale_game_frames(10);
              }
            } else {
                ((JetPlayer *)D_0013F350)->health=0.0f;
                jet_explosion(m,impulse,0,8.0f,100.0f,30,10,24,16.0f,8.0f,9.0f,6,1.0f,50.0f,1,1,-1,0);
                m->flags|=0x41;
                d->death_timer=scale_game_frames(240); m->state=6;

            }
        }
    }
    enqueue_callback_list_1(FUN_L13_002b97f8,m);
    FUN_L13_002b9f58((struct Moby *)m,(char *)d);
    x=fast_add_rotations(d->previous_rotation[2],-F(d,0x6C)/7.0f);
    y=fast_add_rotations(d->previous_rotation[1],-(F(d,0x70)/7.0f)-0.1f);
    FUN_L13_002b9df8((struct Moby *)m,(char *)d,x,-y);
}
#undef F
#endif
