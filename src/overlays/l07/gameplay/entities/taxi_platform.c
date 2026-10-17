/* Level 07 moby classes: purple_ship, ocean_umbris, taxi_platform, boltcrate_possibly_for_metal_detector, bomb, water, bombomatic, sharkigator. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_002FD198), where it is exact; names translated to the US level program. */

extern void func_00233AB8(void) __asm__("FUN_L07_002ba260");

void purple_ship_update(char *arg) __asm__("FUN_L07_002fbdb8");
void purple_ship_update(char *arg) {
    arg[0x30] = 8;
    *(short *)(arg + 0xA6) = 0x215;
    add_draw_callback((void *)func_00233AB8, arg);
}
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"

/* spawns a moby of class 0x370 at a position moving along a direction */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030C918), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_L00_00250df8(void *);
extern void make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
void blend_moby_animation_u(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");

unsigned char *FUN_L07_0030b538(void *pos, float *dir, int a, int b) {
    unsigned char *m = func_0020D348_m(0x370);
    if (m) {
        char *d;
        float V[4];
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0x7F;
        m[0x31] = 1;
        *(int *)(m + 0x40) = 0;
        d = *(char **)(m + 0x78);
        *(float *)(m + 0x44) = -fast_arc_tan(vector_length_xy(dir), dir[2]);
        *(float *)(m + 0x48) = fast_arc_tan(dir[0], dir[1]);
        qcopy(m + 0x10, pos);
        qcopy(d + 0x40, pos);
        qcopy(d + 0x30, dir);
        qcopy(V, d + 0x30);
        V[2] = 0.0f;
        set_vector_length_xy(V, V, 1.0f);
        V[2] = 1.0f;
        make_hit_descriptor(d, (int)m, 0x10001, V, 1.0f);
        d[0x18] = 1;
        d[0x19] = 1;
        blend_moby_animation_u(m, 1, 0, scale_game_frames(5));
        *(short *)(m + 0x34) = 0x208;
        *(int *)(d + 0x50) = a;
        *(int *)(d + 0x54) = b;
        m[0x23] = 0x10;
        FUN_L00_00250df8(m);
    }
    return m;
}
/* spawns a moby with random velocity and copies position */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030CE70), where it is exact; names translated to the US level program. */

extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern char *func_0020D348_m(int) __asm__("FUN_0020c4f8");

char *FUN_L07_0030ba90(char *pos, char *vec, int c, short d) {
    char *m = func_0020D348_m(0x372);
    char *p;
    if (m) {
        p = *(char **)(m + 0x78);
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(unsigned short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        *(float *)(m + 0x40) = random_angle_radians();
        *(float *)(m + 0x44) = random_angle_radians();
        *(float *)(m + 0x48) = random_angle_radians();
        *(float *)(p + 0x18) = randf_sym(100.0f, 300.0f) * DEG_TO_RAD * frame_time;
        *(float *)(p + 0x1C) = randf_sym(100.0f, 300.0f) * DEG_TO_RAD * frame_time;
        qcopy(m + 0x10, pos);
        qcopy(p, vec);
        *(int *)(p + 0x10) = c;
        *(int *)(p + 0x14) = d;
        FUN_L00_00250df8(m);
    }
    return m;
}
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E820), where it is exact; names translated to the US level program. */

extern char D_L07_00166E40[];
extern void l07_moby_draw_callback(void) __asm__("FUN_L07_0030d420");
extern int is_point_inside_clip_volume() __asm__("FUN_00214720");
extern char D_L07_00208560[];
extern void FUN_L02_002a40d0(void *, float);

void ocean_umbris_update(unsigned char *moby) __asm__("FUN_L07_0030d440");
void ocean_umbris_update(unsigned char *moby) {
    switch (moby[0x20]) {
    case 0:
        FUN_L02_002a40d0(D_L07_00208560, 0.66666669f);
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        break;
    case 1:
        if (!is_point_inside_clip_volume(D_L07_00166E40, **(int **)(moby + 0x78))) {
            add_draw_callback((void *)l07_moby_draw_callback, moby);
        }
        break;
    }
}
/* Slows and steers a moby toward another when it is below and close in height. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F4C0), where it is exact; names translated to the US level program. */

extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void spawn_nearby_effect(char *m, float t, int a, int b, void *pos, void *vel) __asm__("FUN_L00_00259bc8");
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L07_0030e0e0(char *a, char *b, char *c) {
    float v[4];
    float w[4];
    if (*(float *)(a + 0x18) < *(float *)(b + 0x18) + *(float *)(c + 0x10) * 0.3f) {
        float d = vector_distance(b + 0x10, a + 0x10);
        if (absolute_float(d - *(float *)(c + 8)) < 0.3f) {
            fast_vec_sub(v, a + 0x10, b + 0x10);
            fast_vec_normalize(w, v, *(float *)(c + 8) - 0.05f);
            fast_vec_normalize(v, v, 1.0f);
            v[2] = 1.0f;
            spawn_nearby_effect(a, *(float *)(c + 0x1c), (int)b, 0x10001, w, v);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F5D8), where it is exact; names translated to the US level program. */

extern char *D_L07_0015FFE4;
extern int FUN_L07_0030e0e0_c() __asm__("FUN_L07_0030e0e0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void FUN_L07_0030dcf0_u(unsigned char *) __asm__("FUN_L07_0030dcf0");
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");
void delete_moby(struct Obj *obj) __asm__("FUN_0020c828");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L07_0030e1f8(unsigned char *m) {
    char *d = *(char **)(m + 0x78);
    float r;
    char *p;
    *(float *)(d + 8) = *(float *)(d + 8) + *(float *)(d + 0xC);
    *(float *)(m + 0x2C) = *(float *)(d + 8);
    r = (*(float *)(d + 8) - *(float *)d) / (*(float *)(d + 4) - *(float *)d);
    if (r < 0.2f) {
        m[0x23] = func_001FA898_r(r * 5.0f * 127.0f);
    } else if (r > 0.8f) {
        m[0x23] = func_001FA898_r((1.0f - r) * 5.0f * 127.0f);
    } else {
        m[0x23] = 0x80;
    }
    if (r >= 1.0f) {
        delete_moby(m);
        return;
    }
    enqueue_callback_list_1((void *)FUN_L07_0030dcf0_u, m);
    FUN_L07_0030e0e0_c(*(void **)(D_0013E533 + 0x2E9D), m, d);
    for (p = D_L07_0015FFE4; p; p = *(char **)(p + 0x28)) {
        FUN_L07_0030e0e0_c(p, m, d);
    }
}
/* Spawn the class 0x419 effect moby at a position with a data block, and start its particle. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F750), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0026daa0(char *src, int col, int w, int v, float x);

char *FUN_L07_0030e370(char *pos, char *vec, int arg) {
    char *m = func_0020D348_m(0x419);
    if (m != 0) {
        char *data;
        *(short *)(m + 0x32) = *(unsigned char *)(m + 0x30) = 0xFF;
        m[0x31] = 1;
        data = *(char **)(m + 0x78);
        *(float *)(m + 0x40) = random_angle_radians();
        *(float *)(m + 0x44) = random_angle_radians();
        *(float *)(m + 0x48) = random_angle_radians();
        qcopy(m + 0x10, pos);
        qcopy(data, vec);
        *(int *)(data + 0x10) = arg;
        m[0x23] = 0x30;
        *(unsigned short *)(m + 0x34) |= 0x200;
        FUN_L00_00250df8(m);
        FUN_L00_0026daa0(m, 0x800040FF, 0x80, -1, 500000.0f);
    }
    return m;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030FFA8), where it is exact; names translated to the US level program. */

extern float FUN_L00_00259710(void *);

int FUN_L07_0030ebc8(char *moby, char *data) {
    float distance = FUN_L00_00259710(moby + 0x10);
    float delta;
    *(float *)(data + 0x130) = distance;
    delta = **(float **)(data + 0x12C) - distance;
    *(float *)(data + 0x138) = distance + delta * 0.5f;
    if (delta < 0.41f)
        return 2;
    if (delta > 0.68f)
        return 0;
    return 1;
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00310038), where it is exact; names translated to the US level program. */

void blend_moby_animation_u(MobyAnim *moby, int new_seq, int frame_index, int blend_frames) __asm__("FUN_00212f90");

void FUN_L07_0030ec58(char *arg) {
    switch (((unsigned char *)arg)[0xBC]) {
    case 0:
        if (((unsigned char *)arg)[0x53] != 0) {
            blend_moby_animation_u(arg, 0, 0, 10);
        }
        arg[0x20] = 1;
        break;
    case 1:
        if (((unsigned char *)arg)[0x53] != 3) {
            blend_moby_animation_u(arg, 3, 0, 10);
        }
        arg[0x20] = 7;
        break;
    case 2:
        if (((unsigned char *)arg)[0x53] != 9) {
            blend_moby_animation_u(arg, 9, 0, 10);
        }
        arg[0x20] = 0xF;
        break;
    }
}
/* Step a moby's velocity toward a target: gravity when above a height, then ease and clamp. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003104D8), where it is exact; names translated to the US level program. */

extern float advance_accelerated_scalar(float *, float *, float, float, float,
                                        float) __asm__("FUN_00213f38");
extern int FUN_L00_00258ad0(int a, int b, int g, int e, float c, float d);
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");

void FUN_L07_0030f0f8(char *a, char *b, float f12, float f13, float f14) {
    float v[4];
    char *q;
    if (**(float **)(b + 0x12C) - 0.3f < *(float *)(a + 0x18)) {
        *(float *)(b + 0xD8) -= frame_time * 9.8f;
    }
    qcopy(v, a + 0x10);
    advance_accelerated_scalar(v + 2, (float *)(b + 0xD8), *(float *)(b + 0x138), f12, f13, f14);
    q = b + 0xD0;
    if (FUN_L00_00258ad0((int)a, (int)q, 0x400, 0x10, 1.0f, 0.0f)) {
        fast_vec_scale(q, q, 0.5f);
    }
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003105C0), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");

int FUN_L07_0030f1e0(char *moby, char *obj, float a, float lo, float up, float hi, float down,
                     float vmax, float vmin) {
    float speed = vector_length_xy(obj + 0xD0);
    int r = 0;
    float d = fast_difference_between_rotations(*(float *)(moby + 0x48), a);
    if (d < lo) {
        speed += up;
        r = 1;
    } else if (hi < d) {
        speed -= down;
    }
    if (vmax < speed) {
        speed = vmax;
    } else if (speed < vmin) {
        speed = vmin;
    }
    *(float *)(obj + 0xD0) = fast_cos(*(float *)(moby + 0x48)) * speed;
    *(float *)(obj + 0xD4) = fast_sin(*(float *)(moby + 0x48)) * speed;
    return r;
}
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003108B0), where it is exact; names translated to the US level program. */

extern int D_L07_00173ED8 __attribute__((section(".data")));
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

int FUN_L07_0030f4d0(void *moby, int id, float scale) {
    char temp[16];
    moby_point_position(moby, 0, temp);
    if (coll_sphere(scale, temp, 0x21, moby) && D_L07_00173ED8 == id) {
        return 1;
    }
    return 0;
}
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003134B8), where it is exact; names translated to the US level program. */

void FUN_L07_003120d8(char *moby) {
    float distance = distance_xyz(moby + 0x10, D_L07_00166E40);
    char *state;
    if (distance > 80.0f)
        distance = 80.0f;
    state = D_L07_00166E40 - 0x140;
    *(int *)(state + 0x168) = 0x14;
    *(float *)(state + 0x160) = ((80.0f - distance) / 80.0f) * 0.1f;
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313530), where it is exact; names translated to the US level program. */

extern char D_L07_00180340[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void FUN_L00_0023e838(int);

void FUN_L07_00312150(char *a) {
    int h;
    float f;
    int c;
    float *p;
    if (fast_dec_timer_s16(a + 0x284)) {
        if (*(short *)(a + 0x280) != -1) {
            FUN_L00_0023e838(*(short *)(a + 0x280));
            *(short *)(a + 0x280) = -1;
            *(short *)(a + 0x282) = -1;
        }
    } else if (*(short *)(a + 0x280) != -1) {
        h = (*(short *)(a + 0x286) * 7) / 8;
        if (h < *(short *)(a + 0x284)) {
            f = convert_integer_to_float(*(short *)(a + 0x284) - h);
            f /= convert_integer_to_float(*(short *)(a + 0x286) - h);
            c = fast_tween_color(0, *(int *)(a + 0x288), 1.0f - f);
        } else {
            f = convert_integer_to_float(*(short *)(a + 0x284));
            f /= convert_integer_to_float(h);
            c = fast_tween_color(*(int *)(a + 0x288), *(int *)(a + 0x28C), 1.0f - f);
        }
        p = (float *)(D_L07_00180340 + *(short *)(a + 0x280) * 32);
        p[0] = convert_integer_to_float(c & 0xFF) * 0.0078125f;
        p[1] = convert_integer_to_float((c >> 8) & 0xFF) * 0.0078125f;
        p[2] = convert_integer_to_float((c >> 16) & 0xFF) * 0.0078125f;
    }
}
/* steps a hit-effect emitter: orient, spawn a puff, and reseed the timer */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313690), where it is exact; names translated to the US level program. */

extern int fast_dec_timer(int *counter) __asm__("FUN_001f9740");
extern int FUN_0022da68(int, int, int);
extern int FUN_L07_0030e370_c() __asm__("FUN_L07_0030e370");
extern void FUN_L00_001ff600(float, void *, void *);
extern void FUN_L07_00311eb8_u(float, void *, void *, short, void *, short, unsigned int, int) __asm__("FUN_L07_00311eb8");

int FUN_L07_003122b0(void *mp, char *obj, float *vec) {
    unsigned char *moby = mp;
    float A[4];
    float B[4];
    if (fast_dec_timer((int *)(obj + 0x160))) {
        moby_point_position(moby, *(int *)(obj + 0x164) & 1, A);
        fast_vec_sub(B, vec, A);
        FUN_L00_001ff600(10.7f, B, B);
        B[2] += 0.5f;
        if (B[2] > 0.0f)
            B[2] = 0.0f;
        fast_vec_normalize(B, B, frame_time * 9.0f);
        FUN_L07_0030e370_c(A, B, moby);
        FUN_0022da68(0, 0, (int)moby);
        FUN_L07_00311eb8_u(10.0f, moby, obj, 0, A, scale_game_frames(0x1E), 0x800060FF, 0);
        *(int *)(obj + 0x160) = scale_game_frames(9);
        if (++*(int *)(obj + 0x164) >= 0x15) {
            *(short *)(obj + 0x198) = scale_game_frames(0x23);
            *(int *)(obj + 0x160) = scale_game_frames(0x1E);
            return 1;
        }
    }
    return moby[0x20];
}

#define F(p, o) (*(float *)((p) + (o)))

/* Moves a moby along a path, easing between nodes and tracking the hero. */
extern char *D_L07_001B0530[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L01_00277fb8(void *);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_00227638(void);
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013E550[];

#define F(p, o) (*(float *)((p) + (o)))

void FUN_L07_002cd2b0(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *path;
    float orig[4], delta[4], tmp[4];
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L07_001B0530[*(int *)(data + 0xB4)];
    qcopy(orig, moby + 0x10);
    fast_dec_timer_s16(data + 0xBA);
    if (moby[0x20] == 0) {
        float f;
        char *pts;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 5;
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        f = 0.0f;
        if ((moby[0xBC] = data[0xA0]) == 0) {
            f = 1.0f;
        }
        F(data, 0xA4) = f;
        pts = path + 0x10;
        qcopy((moby + 0x10), func_001FA898_r(f) ? pts + (*(int *)path - 1) * 16 : pts);
        *(short *)(data + 0xBA) = 0;
        *(int *)(data + 0xA8) = 0;
        *(int *)(data + 0xBC) = 0;
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        *(int *)(data + 0xC4) = -1;
    }
    switch (moby[0xBC]) {
    case 0: {
        char *cls = *(char **)(moby + 0x24);
        float t = F(moby, 0x2C) - F(cls, 0x24) / 60.0f;
        F(moby, 0x2C) = t;
        if (t < 0.0f) {
            F(moby, 0x2C) = F(cls, 0x24);
            qcopy((moby + 0x10), path + 0x10);
            moby[0xBC] = 1;
            F(data, 0xA4) = 0.0f;
        }
        break;
    }
    case 1:
        if (*(short *)(data + 0xB8) != 0) {
            if (FUN_L01_00277fb8(moby)) {
                if (*(int *)(data + 0xC8) == 0) {
                    if (FUN_L01_00277fb8(moby)) {
                        hero_set_state(0x72, 1);
                    }
                }
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case 2:
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f &&
             vector_distance((moby + 0x10), &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - F(moby, 0x18)) < 4.0f &&
             F(moby, 0x18) - hero.motion.pos.f[2] > 1.0f)) {
            int r;
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xBA) = scale_game_frames(30);
            }
            r = *(int *)(data + 0xC4);
            *(int *)(data + 0xA8) = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            *(int *)(data + 0xC4) = -1;
        } else {
            int i;
            float f20, t;
            char *q;
            if (!voice_is_playing(moby, *(int *)(data + 0xC4))) {
                *(int *)(data + 0xC4) = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
            F(data, 0xA8) += F(data, 0xAC) * frame_time;
            if (absolute_float(F(data, 0xA8)) > absolute_float(F(data, 0xAC))) {
                F(data, 0xA8) = F(data, 0xAC);
            }
            F(data, 0xA4) = F(data, 0xA4) + F(data, 0xA8);
            if (0.5f < absolute_float(F(data, 0xA4) - 0.5f)) {
                int r;
                if (*(int *)(data + 0xC8) == 0 && FUN_L01_00277fb8(moby)) {
                    FUN_L00_00227638();
                }
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                *(int *)(data + 0xA8) = 0;
                *(short *)(data + 0xBA) = scale_game_frames(15);
                r = *(int *)(data + 0xC4);
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            f20 = convert_integer_to_float(*(int *)path - 1);
            i = func_001FA898_r(f20 * F(data, 0xA4));
            f20 = convert_integer_to_float(*(int *)path - 1);
            t = convert_integer_to_float(i);
            f20 *= F(data, 0xA4);
            f20 -= t;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                q = path + 0x10 + i * 16;
                subtract_vector_xyz(tmp, path + 0x20 + i * 16, q);
                scale_vector_xyz(tmp, tmp, f20);
                add_vector_xyz((moby + 0x10), tmp, q);
            }
            F(data, 0xBC) = F(moby, 0x18) - orig[2];
        }
        break;
    }
    subtract_vector_xyz(delta, (moby + 0x10), orig);
    update_entity_matrix(data + 0x60, delta, moby + 0x40, moby + 0x40);
    if (!FUN_L01_00277fb8(moby)) {
        if (2.0f < vector_distance((moby + 0x10), &hero.motion.pos)) {
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xB8) = 1;
            }
        }
    }
}
/* Moves a moby along a path between its two ends, hidden while its gate byte is set. */
extern char *D_L07_001B0530[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L01_00277fb8(void *);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char D_L07_0015FC88[] __attribute__((section(".sdata")));
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_00227638(void);
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013E550[];

void FUN_L07_002cdb28(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *path;
    float orig[4], delta[4], tmp[4];
    if (D_L07_0015FC88[moby->mission] != 0xFF) {
        moby->collision = 0;
        moby->flags |= 0x41;
        return;
    }
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L07_001B0530[*(int *)(data + 0xB4)];
    qcopy(orig, &moby->pos);
    fast_dec_timer_s16(data + 0xBA);
    if (moby->state == 0) {
        float f;
        char *pts;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 5;
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        f = 0.0f;
        if ((moby->command = data[0xA0]) == 0) {
            f = 1.0f;
        }
        *(float *)(data + 0xA4) = f;
        pts = path + 0x10;
        qcopy(&moby->pos, func_001FA898_r(f) ? pts + (*(int *)path - 1) * 16 : pts);
        *(short *)(data + 0xBA) = 0;
        *(int *)(data + 0xA8) = 0;
        *(int *)(data + 0xBC) = 0;
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        *(int *)(data + 0xC4) = -1;
        moby->flags &= 0xFFBE;
        moby->collision = moby->pclass->collision;
    }
    switch (moby->command) {
    case 0: {
        char *cls = moby->pclass;
        float t = moby->scale - *(float *)(cls + 0x24) / 60.0f;
        moby->scale = t;
        if (t < 0.0f) {
            moby->scale = *(float *)(cls + 0x24);
            qcopy(&moby->pos, path + 0x10);
            moby->command = 1;
            *(float *)(data + 0xA4) = 0.0f;
        }
        break;
    }
    case 1:
        if (*(short *)(data + 0xB8) != 0) {
            if (FUN_L01_00277fb8(moby)) {
                if (*(int *)(data + 0xC8) == 0) {
                    if (FUN_L01_00277fb8(moby)) {
                        hero_set_state(0x72, 1);
                    }
                }
                moby->command = 2;
                *(float *)(data + 0xAC) = 1.0f / multiply_global_factor_ed64(*(float *)(data + 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case 2:
        if (*(short *)(data + 0xBA) != 0 ||
            (*(float *)(data + 0xBC) < 0.0f &&
             vector_distance(&moby->pos, &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f &&
             moby->pos.z - hero.motion.pos.f[2] > 1.0f)) {
            int r;
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xBA) = scale_game_frames(30);
            }
            r = *(int *)(data + 0xC4);
            *(int *)(data + 0xA8) = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            *(int *)(data + 0xC4) = -1;
        } else {
            int i;
            float f20, t;
            char *q;
            if (!voice_is_playing(moby, *(int *)(data + 0xC4))) {
                *(int *)(data + 0xC4) = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
            *(float *)(data + 0xA8) += *(float *)(data + 0xAC) * frame_time;
            if (absolute_float(*(float *)(data + 0xA8)) > absolute_float(*(float *)(data + 0xAC))) {
                *(float *)(data + 0xA8) = *(float *)(data + 0xAC);
            }
            *(float *)(data + 0xA4) = *(float *)(data + 0xA4) + *(float *)(data + 0xA8);
            if (0.5f < absolute_float(*(float *)(data + 0xA4) - 0.5f)) {
                int r;
                if (*(int *)(data + 0xC8) == 0 && FUN_L01_00277fb8(moby)) {
                    FUN_L00_00227638();
                }
                if (0.0f < *(float *)(data + 0xAC)) {
                    moby->command = 3;
                    *(float *)(data + 0xA4) = 1.0f;
                } else {
                    moby->command = 1;
                    *(float *)(data + 0xA4) = 0.0f;
                }
                *(int *)(data + 0xA8) = 0;
                *(short *)(data + 0xBA) = scale_game_frames(15);
                r = *(int *)(data + 0xC4);
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            f20 = convert_integer_to_float(*(int *)path - 1);
            i = func_001FA898_r(f20 * *(float *)(data + 0xA4));
            f20 = convert_integer_to_float(*(int *)path - 1);
            t = convert_integer_to_float(i);
            f20 *= *(float *)(data + 0xA4);
            f20 -= t;
            if (i == *(int *)path - 1) {
                qcopy(&moby->pos, path + 0x10 + i * 16);
            } else {
                q = path + 0x10 + i * 16;
                subtract_vector_xyz(tmp, path + 0x20 + i * 16, q);
                scale_vector_xyz(tmp, tmp, f20);
                add_vector_xyz(&moby->pos, tmp, q);
            }
            *(float *)(data + 0xBC) = moby->pos.z - orig[2];
        }
        break;
    case 3:
        if (!FUN_L01_00277fb8(moby)) {
            moby->command = 0;
        }
        break;
    }
    subtract_vector_xyz(delta, &moby->pos, orig);
    update_entity_matrix(data + 0x60, delta, &moby->rot, &moby->rot);
    if (!FUN_L01_00277fb8(moby)) {
        if (2.0f < vector_distance(&moby->pos, &hero.motion.pos)) {
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xB8) = 1;
            }
        }
    }
}
/* Update for moby class 436 on level 07: steps through its scenes as the hero enters its clip volumes. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_002F6F20), where it is exact; names translated to the US level program. */

typedef struct {
    int open_volume;
    int finish_volume;
    int removed_moby;
    int moby_0c;
    int moby_10;
    int moby_14;
    int points_18;
    int points_1c;
} Class436Vars;

typedef struct {
    char pad0[0x30];
    float unk30[4];
    char pad40[0x30];
    float unk70[4];
} ScenePoints;

extern u8 D_0013D3B1[];
extern u8 alternate_item_available[128] __asm__("D_0013D388");
extern struct Moby *level_mobys __asm__("D_L07_0015FFD8");
extern int scene_state __asm__("D_L07_0015F5C4");
extern ScenePoints *scene_points __asm__("D_L07_001600EC");
extern void delete_moby_m(void *) __asm__("FUN_0020c828");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern int is_point_inside_clip_volume_i(void *, int) __asm__("FUN_00214720");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void FUN_L00_00299460(int);
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern void FUN_L07_00270248(int x, int y, int w, int h);
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");
extern void initialize_global_state_entry(int) __asm__("FUN_0022e188");

void FUN_L07_002f5ba0(struct Moby *moby)
{
    Class436Vars *data = (Class436Vars *)moby->pvars;
    float v[4];

    switch (moby->state) {
    case 0:
        if (D_0013D3B1[0] != 0 && data->removed_moby != -1) {
            delete_moby_m(&level_mobys[data->removed_moby]);
        }
        if (scene_state == 0) moby->state = 1;
        break;
    case 1:
        if (alternate_item_available[0x28] == 0) {
            alternate_item_available[0x28] = 1;
            moby->state = 2;
            start_scene(0);
        } else {
            moby->state = 3;
        }
        break;
    case 2:
        if (scene_state != 2) moby->state = 3;
        break;
    case 3:
        moby->state = 4;
        break;
    case 4:
        if (alternate_item_available[0x29] == 0) {
            if (data->open_volume != -1) {
                if (is_point_inside_clip_volume_i(&hero.motion.pos, data->open_volume) != 0) {
                    alternate_item_available[0x29] = 1;
                    if (data->moby_0c != -1) {
                        struct Moby *e = &level_mobys[data->moby_0c];
                        e->flags |= 0x41;
                    }
                    if (data->moby_14 != -1) {
                        struct Moby *e = &level_mobys[data->moby_14];
                        e->flags |= 0x41;
                    }
                    moby->state = 5;
                    start_scene(1);
                }
            } else {
                moby->state = 8;
            }
        } else {
            moby->state = 7;
        }
        if (moby->state != 4 && data->removed_moby != -1) {
            delete_moby_m(&level_mobys[data->removed_moby]);
        }
        break;
    case 5:
        if (scene_state == 2) break;
        if (data->moby_0c != -1) {
            struct Moby *e = &level_mobys[data->moby_0c];
            e->flags |= 0x41;
        }
        moby->state = 6;
        start_scene(2);
        break;
    case 6:
        if (scene_state == 2) break;
        if (data->points_18 != -1) {
            FUN_L00_00216f90(scene_points[data->points_18].unk30, scene_points[data->points_18].unk70, 0, 1);
        }
        moby->state = 7;
        break;
    case 7:
        if (data->moby_0c != -1) {
            struct Moby *e = &level_mobys[data->moby_0c];
            e->flags &= 0xFFBE;
        }
        if (data->moby_14 != -1) {
            struct Moby *e = &level_mobys[data->moby_14];
            e->flags &= 0xFFBE;
        }
        jet_game_state(2, 4);
        moby->state = 8;
        break;
    case 8:
        if (alternate_item_available[0x2A] == 0) {
            struct Moby *e;
            if (data->moby_0c == -1 || (e = &level_mobys[data->moby_0c]) == 0 || e->state == 0xFE || e->state == 0xFD) {
                if (data->finish_volume != -1) {
                    if (is_point_inside_clip_volume_i(&hero.motion.pos, data->finish_volume) != 0) {
                        FUN_L00_00216f90(scene_points[data->points_1c].unk30, scene_points[data->points_1c].unk70, 0x72, 1);
                        alternate_item_available[0x2A] = 1;
                        moby->state = 9;
                        start_scene(3);
                    }
                } else {
                    moby->state = 13;
                }
            }
        } else {
            moby->state = 12;
        }
        if (moby->state != 8 && data->moby_10 != -1) {
            struct Moby *e = &level_mobys[data->moby_10];
            e->flags |= 0x41;
        }
        break;
    case 9:
        if (scene_state == 2) break;
        moby->state = 10;
        FUN_L00_00299460(8);
        break;
    case 10:
        if (scene_state == 2) break;
        moby->state = 11;
        start_scene(4);
    case 11:
        if (scene_state == 2) break;
        if (data->points_1c != -1) {
            FUN_L00_00216f90(scene_points[data->points_1c].unk30, scene_points[data->points_1c].unk70, 0, 1);
        }
        if (data->moby_10 != -1) {
            struct Moby *e = &level_mobys[data->moby_10];
            subtract_vector_xyz(v, &hero.motion.pos, &e->pos);
            clamp_vector_length(v, v, 0.9f);
            add_vector_xyz(&e->pos, &hero.motion.pos, v);
            delete_moby_m(e);
        }
        FUN_L07_00270248(0x87, 0xC, 0x56, 0x3C);
        unlock_planet(8);
        memcard_save_data(0, 8);
        initialize_global_state_entry(8);
        moby->state = 12;
        break;
    case 12:
        if (data->moby_10 != -1) {
            struct Moby *e = &level_mobys[data->moby_10];
            if (e != 0 && e->state != 0xFE && e->state != 0xFD) {
                delete_moby_m(e);
            }
        }
        moby->state = 13;
        break;
    case 13:
        delete_moby_m(moby);
        break;
    }
}
#include "rnc/gameplay/entities/moby.h"

struct SpawnClassView {
    u8 pad0[0x10];
    u32 unk10;     /* 0x10 */
    u8 pad14[0x10];
    f32 scale;     /* 0x24 */
    u8 pad28[0x1C];
    u16 flags;     /* 0x44 */
};

struct SpawnMobyView {
    u8 pad0[0x36];
    s16 unk36;     /* 0x36 */
    u8 pad38[0x39];
    u8 unk71;      /* 0x71 */
};

struct AnimTimer {
    f32 frames;
    s16 length;
};

extern void update_moby_grids(struct Moby *, u32) __asm__("FUN_0020dc20");
extern void FUN_L00_0025d1b8(struct Moby *);
extern struct AnimTimer *suck_get_aim_height(struct Moby *) __asm__("FUN_002141f8");

/* resets a spawned moby from its class defaults and restarts its animation */
void FUN_L07_0030af40(struct Moby *m) {
    struct SpawnClassView *cls;
    struct AnimTimer *t;

    m->state = 0;
    cls = (struct SpawnClassView *)m->pclass;
    m->command = 0;
    m->flags = cls->flags;
    m->scale = cls->scale;
    m->update_dist = 0xFF;
    m->draw_dist = 0xFF;
    m->visible = 1;
    update_moby_grids(m, 0x80807F7F);
    ((struct SpawnMobyView *)m)->unk36 = 0x7F80;
    m->unk72 = 0xFF;
    m->hit_slot = 0xFF;
    ((struct SpawnMobyView *)m)->unk71 = 0xFF;
    m->collision = ((struct SpawnClassView *)m->pclass)->unk10;
    FUN_L00_0025d1b8(m);
    FUN_L00_00250df8(m);
    t = suck_get_aim_height(m);
    if (t != 0)
        t->frames = t->length;
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L07_0030b000.s", FUN_L07_0030b000);
#else
extern char *D_L07_001B0530[] __asm__("D_L07_001B0530");
extern char *FUN_L00_0025a420(void *, int, int);
extern s32 FUN_00213928(struct Moby *, char *, f32 *, s32, s32 *, s32, s32, s32);
extern void FUN_L01_0026e0e0(s32, s32);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_0025e450_l07(f32, f32, f32, f32, f32, f32, f32, struct Moby *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern s32 FUN_L00_002591d0(s32 *, s32, s32, s32);
extern s32 FUN_L00_002592b8(s32 *, s32, s32, s32);
extern void debug_print(char *, ...) __asm__("FUN_001e93b0");
extern s32 FUN_L01_0028b510(f32, void *, void *);
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern int sample_path(f32, void *, void *, void *, void *, s32) __asm__("FUN_L00_0025d808");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void jet_animate(struct Moby *, f32) __asm__("FUN_L00_00259fe8");
extern char D_L07_00211550[] __asm__("D_L07_00211550");
extern char D_L07_00211580[] __asm__("D_L07_00211580");
extern char D_L07_002115D0[] __asm__("D_L07_002115D0");

void boltcrate_possibly_for_metal_detector_update(struct Moby *moby) __asm__("FUN_L07_0030b000");
void boltcrate_possibly_for_metal_detector_update(struct Moby *moby) {
    char *data;
    char *path;
    f32 *remaining;
    s32 count;
    s32 current;
    s32 child;
    s32 i;
    s32 first;
    s32 next;
    f32 distance;
    union { u128 q; f32 f[4]; } position;

    if (!moby) return;
    data = (char *)moby->pvars;
    if (!data) return;
    remaining = (f32 *)(data + 0x20);
    if (*(u8 *)(data + 0x94)) {
        char *limit = (char *)FUN_L00_0025a420(moby, 0x230000, 0);
        s32 result = FUN_00213928(moby, limit, remaining, 0, &count, 0, 0, 4);
        switch ((u32)result) {
        case 0:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            break;
        case 1:
        case 2:
            *remaining = 0.0f;
            break;
        }
        if (count > 1) {
            f32 step = *(f32 *)(limit + 0x2c);
            if (*remaining <= step) {
                *remaining = 0.0f;
                FUN_L01_0026e0e0(*(s32 *)(data + 0x74), 3);
                clear_u64_value(&position);
                FUN_L00_0025e450_l07(0.0f, 0.0f, 4.0f, 2.0f, 9.0f, 1.0f, 15.0f,
                                    moby, &position, 0, 10, 3, 16, -1, 1, 1, -1, 0);
                delete_moby((struct Obj *)moby);
                return;
            }
            *remaining -= step;
            *(u8 *)(data + 0x67) = 0xfa;
            *(s16 *)(data + 0x26) = scale_game_frames(0x3c);
            trooper_anim_update(moby, (short *)(data + 0x60));
        }
        moby->hit_slot = 0xff;
    }
    trooper_anim_finish(moby, data + 0x60);
    if (*(s32 *)(data + 0x70) == -1) return;
    path = D_L07_001B0530[*(s32 *)(data + 0x70)];
    switch (moby->state) {
    case 0: {
        s32 path_count;
        current = 0;
        next = 0;
        qcopy(&position, path + 0x10);
        path_count = *(s32 *)path;
        {
            s32 j = 0;
            s32 k;
            while (j < path_count) {
                k = j + 1;
                *(f32 *)(path + (j << 4) + 0x1c) =
                    fast_vec_dist(path + 0x10 + j * 16,
                                  path + 0x10 + (k % path_count) * 16);
                j = k;
                path_count = *(s32 *)path;
            }
        }
        if (FUN_L00_002591d0(&current, *(s32 *)(data + 0x74), 1, 0)) {
            debug_print(D_L07_00211550, *(s32 *)(data + 0x74));
            return;
        }
        first = FUN_L01_0028b510(0.0f, (char *)moby + 0x10, path);
        *(s32 *)(data + 0x90) = first;
        i = 0;
        if (0 < *(s32 *)(data + 0x78)) do {
            child = *(s32 *)(current + 0x78);
            *(s32 *)(child + 0xa0) = (s32)moby;
            qcopy((void *)(current + 0x10), &position);
            qcopy((void *)(child + 0x80), &position);
            qcopy((void *)(child + 0x70), &position);
            *(s32 *)(child + 0x90) = first;
            *(s32 *)(child + 0x94) = first + 1;
            if (FUN_L00_002592b8(&current, current, 1, 0)) {
                s32 child_count = *(s32 *)(data + 0x78);
                s32 updated = i + 1;
                if (i < child_count - 1)
                    debug_print(D_L07_00211580, updated, *(s32 *)(data + 0x74));
                else if (i == child_count - 1)
                    debug_print(D_L07_002115D0, *(s32 *)(data + 0x74), child_count);
                *(s32 *)(data + 0x78) = updated;
                break;
            }
            sample_path(*(f32 *)(data + 0x80), path, &position, &first, &next, 1);
            i++;
        } while (i < *(s32 *)(data + 0x78));
        if (!*(u8 *)(data + 0x94)) {
            moby->collision = 0;
            moby->flags &= 0xefff;
        }
        if (!*(u8 *)(data + 0x95)) moby->flags |= 1;
        moby->state = 1;
        break;
    }
    case 1: {
        s32 cursor;
        s32 identifier = *(s32 *)(data + 0x74);
        cursor = 0;
        FUN_L00_002591d0(&cursor, identifier, 1, 0);
        i = 0;
        if (0 < *(s32 *)(data + 0x78)) do {
            child = *(s32 *)(cursor + 0x78);
            {
                f32 segment_distance = fast_vec_dist((char *)child + 0x80,
                              path + 0x10 + *(s32 *)(child + 0x90) * 16);
                f32 step = *(f32 *)(data + 0x7c) * frame_time;
                distance = segment_distance;
                sample_path(step, path,
                         (char *)child + 0x80, (char *)child + 0x90, &distance, 1);
            }
            *(s32 *)(child + 0x94) = *(s32 *)(child + 0x90) + 1;
            if (cursor == 0 || *(u8 *)(cursor + 0x20) == 0xfe ||
                *(u8 *)(cursor + 0x20) == 0xfd) {
                if (*(s32 *)(data + 0x84) != -1) {
                    fast_vec_sub((char *)child + 0x60, (char *)child + 0x80,
                                  (char *)cursor + 0x10);
                    qcopy((void *)(cursor + 0x10), (void *)(child + 0x80));
                    if (fast_dec_timer((int *)(child + 0x98)) &&
                        *(s32 *)(child + 0x90) == *(s32 *)(data + 0x90))
                        FUN_L07_0030af40((struct Moby *)cursor);
                }
            }
            FUN_L00_002592b8(&cursor, cursor, 1, 0);
            i++;
        } while (i < *(s32 *)(data + 0x78));
        break;
    }
    }
    jet_animate(moby, 1.5f);
}
#endif
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

/* Pvars of a flying bolt. */
typedef struct {
    u8 pad0[0x30];
    Vec4 vel;        /* 0x30 */
    Vec4 origin;     /* 0x40 */
    void *ignore;    /* 0x50: collision owner to skip */
    s32 solid;       /* 0x54 */
} BoltVars;

extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void FUN_L07_0029a2a0(void *, void *);
extern void FUN_L00_001ff660(void *, void *, void *);
extern char *FUN_L00_0026dd20(char *, char *, int c, int d, float f);
extern int play_moby_sound(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern Vec4 D_L07_00173EE0;
extern char D_L07_00173F00[];

/* Bolt update: flies on, sparks out at the end of its range, bursts on impact, removes it out of bounds. */
void FUN_L07_0030b6a8(struct Moby *m) {
    BoltVars *v = (BoltVars *)m->pvars;
    Vec4 old;
    Vec4 dir;
    Vec4 spark;
    int pass;
    int i;
    int j;

    qcopy(&old, &m->pos);
    add_vector_xyz(&m->pos, &m->pos, &v->vel);
    pass = v->solid == 0;
    if (vector_distance(&m->pos, &v->origin) > 30.0f) {
        if (m->visible != 0) {
            for (i = 0; i < 3; i++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                dir.q = spark.q;
                normalize_vector_xyz(&dir, &dir, vector_length_xyz(&spark) * 0.1f);
                add_vector_xyz(&dir, &spark, &dir);
                normalize_vector_xyz(&dir, &dir,
                                     random_float_between(frame_time + frame_time, frame_time * 4.0f));
                FUN_L07_0029a2a0(&m->pos, &dir);
            }
        }
        mark_moby_for_removal(m);
        return;
    }
    if ((m->visible != 0 || vector_distance(&m->pos, &hero.motion.pos) < 30.0f) &&
        collision_line(&old, &m->pos, pass, v->ignore, v) != 0) {
        qcopy(&m->pos, &D_L07_00173EE0);
        if (m->visible != 0) {
            for (j = 0; j < 5; j++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                dir.q = spark.q;
                FUN_L00_001ff660(&spark, &v->vel, D_L07_00173F00);
                normalize_vector_xyz(&dir, &dir, vector_length_xyz(&spark) * 0.5f);
                add_vector_xyz(&dir, &spark, &dir);
                normalize_vector_xyz(&dir, &dir,
                                     random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                FUN_L00_0026dd20((char *)&m->pos, (char *)&dir, 0x7F2F4F6F,
                                 random_int_between(scale_game_frames(10), scale_game_frames(15)),
                                 30000.0f);
            }
        }
        play_moby_sound(0, 0, m);
        mark_moby_for_removal(m);
        return;
    }
    if (m->pos.x < 2.0f || m->pos.x > 1021.0f || m->pos.y < 2.0f || m->pos.y > 1021.0f ||
        m->pos.z < 2.0f || m->pos.z > 1021.0f) {
        mark_moby_for_removal(m);
    }
}
/* Pvars of a tumbling piece of debris. */
typedef struct {
    Vec4f vel;       /* 0x00 */
    void *ignore;    /* 0x10: collision owner to skip */
    s32 variant;     /* 0x14 */
    f32 spin_y;      /* 0x18 */
    f32 spin_z;      /* 0x1C */
} DebrisVars;

extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                            float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern struct Moby *debris_target __asm__("D_L07_00173ED8") __attribute__((section(".data")));
extern char debris_far_point[] __asm__("D_L07_00166E40");

/* Debris update: moves and spins it, bursts it on impact, removes it out of bounds. */
void FUN_L07_0030bbc8(struct Moby *m) {
    Vec4f *pos = &m->pos;
    DebrisVars *d = (DebrisVars *)m->pvars;
    Vec4 old;
    float size;

    qcopy(&old, pos);
    add_vector_xyz(pos, pos, d);
    d->vel.z -= frame_time_sq * 9.8f;
    m->rot.y = fast_add_rotations(m->rot.y, d->spin_y);
    m->rot.z = fast_add_rotations(m->rot.z, d->spin_z);
    if (collision_line(&old, pos, 0, d->ignore, NULL) != 0 || coll_sphere(0.5f, pos, 0, d->ignore) != 0) {
        if (debris_target == NULL || (u16)(debris_target->oclass - 0x371) >= 2) {
            if (m->visible != 0 || vector_distance(pos, &hero.motion.pos) < 5.0f) {
                size = 0.0f;
                if (d->variant % 3 == 0) {
                    size = 10.0f;
                }
                if (vector_distance(pos, debris_far_point) > 50.0f) {
                    if (d->variant & 1) {
                        spawn_explosion(m, d, 0, 2.0f, 1.0f, 1, 2, 3, 0.0f, 0.0f, 1000.0f, 1.3f, -1,
                                        size, 0, 1, -1, 0);
                    } else {
                        spawn_explosion(m, d, 0, 2.0f, 1.0f, 0, 2, 3, 0.0f, 0.0f, 1000.0f, 1.2f, 0,
                                        size, 0, 1, -1, 0);
                    }
                } else {
                    if (d->variant & 1) {
                        spawn_explosion(m, d, 0, 2.0f, 1.0f, 0, 2, 4, 0.0f, 1.0f, 1000.0f, 1.0f, 0,
                                        size, 0, 1, -1, 0);
                    } else {
                        spawn_explosion(m, d, 0, 2.0f, 1.0f, 1, 2, 4, 0.0f, 0.0f, 1000.0f, 1.0f, 0,
                                        size, 0, 1, -1, 0);
                    }
                }
            } else {
                play_moby_sound(0, 0, m);
            }
            mark_moby_for_removal(m);
            return;
        }
    }
    if (m->pos.x < 2.0f || m->pos.x > 1021.0f || m->pos.y < 2.0f || m->pos.y > 1021.0f ||
        m->pos.z < 2.0f || m->pos.z > 1021.0f) {
        mark_moby_for_removal(m);
    }
}
/* Ported from rac1-decomp src/overlays/l07_umbris/vendor_002CE470.c (func_L07_0030D8E8) */

typedef struct {
    float x;                   /* 0x00 */
    float y;                   /* 0x04 */
    float z;                   /* 0x08 */
    char padC[0x10];
    unsigned char b1C;         /* 0x1C */
    unsigned char b1D;         /* 0x1D */
    short h1E;                 /* 0x1E */
    char rest[0x1170];
} Rec_30D8E8;

typedef struct {
    float f0;                  /* 0x00 */
    float f4;
    float f8;
    float fC;
    float f10;                 /* 0x10 */
    float f14;
    float f18;
    float f1C;
    float f20;                 /* 0x20 */
    float f24;
    int i28;                   /* 0x28 */
    int i2C;
    float f30;                 /* 0x30 */
    float f34;
    float f38;
    unsigned char b3C;         /* 0x3C */
    unsigned char b3D;
} Cab_30D8E8;

typedef struct {
    char pad0[0x10];
    float f10;
    float f14;
} Dir_30D8E8;

typedef struct {
    float hi;                  /* 0x00 */
    float lo;                  /* 0x04 */
    int f8;                    /* 0x08 */
    int fC;                    /* 0x0C */
    int volume;                /* 0x10 */
    int idx;                   /* 0x14 */
    int timer;                 /* 0x18 */
    int f1C;                   /* 0x1C */
} Data_30D8E8;

typedef struct {
    char pad0[0x10];
    float pos[4];              /* 0x10 */
    unsigned char state;       /* 0x20 */
    unsigned char f21;         /* 0x21 */
    char pad22[0x12];
    unsigned short flags;      /* 0x34 */
    char pad36[0x42];
    Data_30D8E8 *data;         /* 0x78 */
    char pad7C[0x18];
    int f94;                   /* 0x94 */
    char pad98[0x24];
    unsigned char sub;         /* 0xBC */
} Moby_30D8E8;

extern Cab_30D8E8 D_L07_001CA900;
extern Dir_30D8E8 D_L07_0017FF40;
extern Rec_30D8E8 *D_L07_001612D0;
extern char D_0013D4EC[];
extern float D_0015ED6C;
extern char D_0013D4C0[];
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L07_0016120C;
extern float D_L07_00161210;
extern float D_L07_00161214;
extern int D_L07_00161208;
extern void *D_L07_001612D4;
extern int D_L07_001612D8; /* no foreign declaration */
extern int D_L07_00161B30 __attribute__((sda)); /* no foreign declaration */
extern unsigned short D_L07_00161B34 __attribute__((sda)); /* no foreign declaration */
extern int D_L07_00161B38 __attribute__((sda)); /* no foreign declaration */
extern int D_L07_00204700[][4];
extern int pool_splash() __asm__("FUN_L00_002a3ec8");
extern int pool_init_tiles() __asm__("FUN_L01_002b7a48");
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(int *arg0) __asm__("FUN_001f9740");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern unsigned short D_L07_00204680[];
extern void approach_value(f32 *, f32, f32) __asm__("func_00213ED8");
extern void enqueue_callback_list_1_c(void *, void *) __asm__("FUN_001f4600");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
int FUN_L07_00281ef8(int id);
void FUN_L01_0026e090(int i, int v);
void FUN_L01_0026e0e0(int list, int state);
void FUN_L01_00275690(char *vec, float scale);
void pool_bind_tile(float *in, int *out) __asm__("FUN_L01_002b7c68");
void pool_set_scale(float s) __asm__("FUN_L01_002b7ef0");
void pool_flush(char *p, int n) __asm__("FUN_L01_002b7fe0");
void pool_ripple(void *a, int b) __asm__("FUN_L01_002b8288");
void FUN_L07_0030cd20(void);
void FUN_L07_0030cd48_c(int arg) __asm__("FUN_L07_0030cd48");
extern Rec_30D8E8 D_L07_001DCE40_2cd2b0[] __asm__("D_L07_001DCE40");
extern void FUN_L01_002b7a48_2cd2b0(void *, int) __asm__("FUN_L01_002b7a48");
extern void FUN_L01_002b7ef0_2cd2b0(float) __asm__("FUN_L01_002b7ef0");
extern void FUN_L01_002b7c68_2cd2b0(void *, void *) __asm__("FUN_L01_002b7c68");
extern void FUN_L01_002b8288_2cd2b0(void *, int, float, float, float, float) __asm__("FUN_L01_002b8288");
extern void FUN_L07_0030cd48_2cd2b0(int) __asm__("FUN_L07_0030cd48");
extern void FUN_L01_0026e0e0_2cd2b0(int, int) __asm__("FUN_L01_0026e0e0");
extern int FUN_L07_00281ef8_2cd2b0(void *, int, int, int) __asm__("FUN_L07_00281ef8");
extern float FUN_00213ed8_2cd2b0(float *, float, float) __asm__("FUN_00213ed8");
extern void FUN_L01_0026e090_2cd2b0(int, int) __asm__("FUN_L01_0026e090");
extern void FUN_L00_002a3ec8_2cd2b0(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern int FUN_00214720_2cd2b0(void *, int) __asm__("FUN_00214720");
extern int FUN_L01_00275690_2cd2b0(void *, float) __asm__("FUN_L01_00275690");
extern void FUN_L01_002b7fe0_2cd2b0(void *, int) __asm__("FUN_L01_002b7fe0");
extern void FUN_L07_0030cd20_2cd2b0(void) __asm__("FUN_L07_0030cd20");

void water_update(Moby_30D8E8 *m) __asm__("FUN_L07_0030c508");
void water_update(Moby_30D8E8 *m) {
    Data_30D8E8 *d = m->data;
    int i;
    int j;
    int k;
    int n;
    float ang;
    float on;

    if (d == 0) {
        return;
    }
    if (D_L07_00161B38 != 0) {
        D_0013D4C0[4] = 0;
        D_0013D4EC[0] = 0;
    }
    switch (m->state) {
    case 0:
        m->flags |= 0x81;
        if (d->idx == 0) {
            Cab_30D8E8 *cab;
            Dir_30D8E8 *dir;
            int (*p)[4];
            FUN_L01_002b7a48_2cd2b0(D_L07_001DCE40_2cd2b0, 0x24);
            FUN_L01_002b7ef0_2cd2b0(1.0f);
            cab = &D_L07_001CA900;
            dir = &D_L07_0017FF40;
            cab->f0 = 16.0f;
            cab->f4 = 16.0f;
            cab->f8 = -8.0f;
            cab->fC = -8.0f;
            cab->f10 = 1.0f;
            cab->f14 = 1.0f;
            cab->f18 = 0.9f;
            cab->f20 = 0.1f;
            ang = fast_arc_tan(dir->f10, dir->f14);
            cab->f30 = fast_cos(ang) * 0.57735f;
            cab->f34 = fast_sin(ang) * 0.57735f;
            cab->f38 = -0.57735f;
            cab->i28 = 0x28;
            cab->i2C = 0x29;
            p = D_L07_00204700;
            D_L07_001612D8 = 0x24;
            D_L07_0016120C = 32768.0f;
            D_L07_00161210 = 255.0f;
            D_L07_00161214 = 48.0f;
            cab->b3C = 0x40;
            cab->b3D = 0x40;
            D_L07_001612D0 = D_L07_001DCE40_2cd2b0;
            D_L07_001612D4 = p;
            D_L07_00161208 = 0;
            for (i = 0; i < 0x24; i++) {
                FUN_L01_002b7c68_2cd2b0(&D_L07_001DCE40_2cd2b0[i], p[i]);
                D_L07_001DCE40_2cd2b0[d->idx].h1E = D_L07_00204680[d->idx];
            }
            for (k = 0; k < 0x24; k++) {
                Rec_30D8E8 *t = D_L07_001DCE40_2cd2b0;
                Rec_30D8E8 *r = &t[k];
                for (j = 0; j < 2; j++) {
                    float x = r->x + random_float_between(-6.0f, 6.0f);
                    float y = r->y + random_float_between(-6.0f, 6.0f);
                    FUN_L01_002b8288_2cd2b0(t, 0x24, x, y, 2.0f, 0.2f);
                }
            }
            for (n = 0; n < 0x24; n++) {
                FUN_L07_0030cd48_2cd2b0(n);
            }
        }
        if (d->fC != 0) {
            m->state = 3;
            d->timer = scale_game_frames_alt(0x3C);
            if (m->pos[2] < d->lo) {
                D_L07_001DCE40_2cd2b0[d->idx].h1E = 0;
            }
        } else if (d->f8 != 0) {
            m->state = 1;
        } else {
            m->state = 2;
        }
        break;
    case 2:
        if (m->sub & 4) {
            FUN_L01_0026e0e0_2cd2b0(m->f21, 1);
        } else if (FUN_L07_00281ef8_2cd2b0(m, m->f21, 1, 0) && m->sub != 1) {
            FUN_00213ed8_2cd2b0(&m->pos[2], d->lo, D_0015ED6C * 4.0f);
            FUN_L01_0026e090_2cd2b0(m->f21, 1);
        } else if (m->sub == 1) {
            FUN_00213ed8_2cd2b0(&m->pos[2], d->lo, D_0015ED6C * 4.0f);
        }
        if (random_integer_below(200) == 0) {
            float x = D_L07_001DCE40_2cd2b0[d->idx].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            Rec_30D8E8 *r = &D_L07_001DCE40_2cd2b0[d->idx];
            FUN_L00_002a3ec8_2cd2b0(r, 1, 1, x, r->y + y, 1.0f, -0.05f);
        }
        break;
    case 1:
        if (m->sub & 8) {
            FUN_L01_0026e0e0_2cd2b0(m->f21, 2);
        } else if (FUN_L07_00281ef8_2cd2b0(m, m->f21, 1, 0) && m->sub != 2) {
            FUN_00213ed8_2cd2b0(&m->pos[2], d->hi, D_0015ED6C * 4.0f);
            FUN_L01_0026e090_2cd2b0(m->f21, 2);
        } else if (m->sub == 2) {
            FUN_00213ed8_2cd2b0(&m->pos[2], d->hi, D_0015ED6C * 4.0f);
        }
        if (random_integer_below(200) == 0) {
            float x = D_L07_001DCE40_2cd2b0[d->idx].x + random_float_between(-4.0f, 4.0f);
            float y = random_float_between(-4.0f, 4.0f);
            Rec_30D8E8 *r = &D_L07_001DCE40_2cd2b0[d->idx];
            FUN_L00_002a3ec8_2cd2b0(r, 1, 1, x, r->y + y, 1.0f, -0.05f);
        }
        break;
    case 3:
        if (d->volume >= 0 && FUN_00214720_2cd2b0(&hero.motion.pos, d->volume)) {
            d->f1C = 0;
            FUN_L01_0026e0e0_2cd2b0(m->f21, 4);
        }
        break;
    case 4:
        if (fast_dec_timer(&d->timer)) {
            d->timer = scale_game_frames_alt(0x708);
            m->state = 5;
        }
        break;
    case 5:
        m->pos[2] += D_0015ED6C * 0.32f;
        if (fast_dec_timer(&d->timer)) {
            m->state = 6;
        }
        break;
    case 6:
        m->pos[2] += D_0015ED6C * 0.42f;
        if (m->pos[2] >= 54.0f) {
            m->state = 7;
        }
        break;
    case 7:
        if (d->hi < m->pos[2]) {
            D_L07_001DCE40_2cd2b0[d->idx].h1E = 0;
            mark_moby_for_removal_c2(m);
            return;
        }
        break;
    }
    if (d->idx < 0) {
        return;
    }
    {
        Cab_30D8E8 *cab = &D_L07_001CA900;
        D_L07_001DCE40_2cd2b0[d->idx].b1C = cab->b3C;
        D_L07_001DCE40_2cd2b0[d->idx].b1D = cab->b3D;
    }
    D_L07_001DCE40_2cd2b0[d->idx].z = m->pos[2];
    on = 0.0f;
    if (m->f94 != 0 && FUN_L01_00275690_2cd2b0(m, 48.0f) != -1) {
        if (!(m->state >= 3 && m->state <= 7) || (m->pos[2] > d->lo && m->pos[2] < d->hi)) {
            on = 1.0f;
        }
    }
    if (on == 0.0f) {
        D_L07_001DCE40_2cd2b0[d->idx].h1E = 0;
    } else {
        D_L07_001DCE40_2cd2b0[d->idx].h1E = D_L07_00204680[d->idx];
    }
    if (d->idx == 0) {
        FUN_L01_002b7fe0_2cd2b0(D_L07_001DCE40_2cd2b0, 0x24);
        enqueue_callback_list_1_c(FUN_L07_0030cd20_2cd2b0, m);
    }
    if (d->idx == D_L07_00161B30) {
        D_L07_001DCE40_2cd2b0[d->idx].h1E = D_L07_00161B34;
        D_L07_00204680[d->idx] = D_L07_00161B34;
    }
}
/* Level overlay code after unclassified; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */

typedef struct {
    float x;
    float y;
    int pad;
    unsigned char n[0x14];
    char rest[0x1170];
} Ent;

extern Ent D_L07_001DCE40[];
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L07_0030cd48(int arg) {
    int i;
    D_L07_001DCE40[arg].n[0] = 0xFF;
    D_L07_001DCE40[arg].n[2] = 0xFF;
    D_L07_001DCE40[arg].n[1] = 0xFF;
    D_L07_001DCE40[arg].n[3] = 0xFF;
    D_L07_001DCE40[arg].n[4] = 0xFF;
    D_L07_001DCE40[arg].n[5] = 0xFF;
    D_L07_001DCE40[arg].n[6] = 0xFF;
    D_L07_001DCE40[arg].n[7] = 0xFF;
    for (i = 0; i < 36; i++) {
        if (arg != i) {
            Ent *t = D_L07_001DCE40;
            Ent *e = &t[arg];
            float x = e->x - t[i].x;
            float y = e->y - t[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    e->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    e->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    e->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    e->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        e->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        e->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        e->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        e->n[6] = i;
                }
            }
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E370), where it is exact; names translated to the US level program. */
extern s32 D_L07_00184528[] __attribute__((section(".data")));
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");

typedef struct {
    f32 origin_x;
    f32 origin_y;
    f32 origin_z;
    s16 sound_slot;
    s8 trigger_sound;
    s8 arrival_sound;
    s32 target_index;
    s16 flag_index;
    s16 target_state;
    s32 target_state_word;
    f32 radius;
} FUN_L07_0030cf90_Vars;

/* Slides out from its origin once its linked moby reaches one state and back when it reaches
   another; a level flag is set while it is out. */
void FUN_L07_0030cf90(struct Moby *moby) {
    extern char *level_mobys_gp __asm__("D_L07_0015FFD8") __attribute__((sda));
    FUN_L07_0030cf90_Vars *vars = (FUN_L07_0030cf90_Vars *)moby->pvars;
    f32 origin[4];
    Vec4 rotated;
    Vec4 step;
    struct Moby *target;
    s16 voice;
    f32 distance;
    f32 step_size;

    origin[0] = vars->origin_x;
    origin[1] = vars->origin_y;
    origin[2] = vars->origin_z;
    origin[3] = 0.0f;
    if (moby->mission != 0xff && visited52[moby->mission + (current_level_index << 4)] == 0xff)
        vars->target_index = -1;

    switch (moby->state) {
    case 0:
    {
        Vec4f *position = &moby->pos;
        vars->origin_x = position->x;
        vars->origin_y = position->y;
        vars->origin_z = position->z;
        vars->sound_slot = -1;
        if (vars->flag_index != -1)
            D_L07_00184528[vars->flag_index] = 0;
        moby->state = 1;
        break;
    }
    case 1:
        if (vars->target_index != -1) {
            target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100);
            if (target != 0 && target->state != 0xfe && target->state != 0xfd &&
                target->state != vars->target_state)
                return;
        }
        if (vars->trigger_sound != -1)
            play_moby_sound(vars->trigger_sound, 0, moby);
        moby->state = 2;
        break;
    case 2:
        if (vars->target_index != -1 &&
            (target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100)) != 0 &&
            target->state != 0xfe && target->state != 0xfd &&
            target->state == vars->target_state_word) {
            play_moby_sound(vars->trigger_sound, 0, moby);
            moby->state = 4;
            break;
        }
        distance = vector_distance(&moby->pos, origin);
        if (vars->radius < distance) {
            if (vars->arrival_sound != -1)
                play_moby_sound(vars->arrival_sound, 0, moby);
            if (vars->flag_index != -1)
                D_L07_00184528[vars->flag_index] = 1;
            moby->state = 3;
        } else {
            step.q = 0;
            step.f[1] = -(frame_scale * 0.01f);
            rotated.q = step.q;
            if (moby->oclass == 0x3f5 || moby->oclass == 0x429)
                rotated.f[1] = -rotated.f[1];
            transform_vector_by_basis(&step, &rotated, &moby->rot_row0);
            add_vector_xyz(&moby->pos, &moby->pos, &step);
            return;
        }
        break;
    case 3:
        if (vars->target_index != -1) {
            target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100);
            if (target != 0 && target->state != 0xfe && target->state != 0xfd) {
                if (target->state != vars->target_state_word)
                    return;
                if (vars->trigger_sound != -1)
                    play_moby_sound(vars->trigger_sound, 0, moby);
                moby->state = 4;
            }
        }
        break;
    case 4:
        if (vars->target_index == -1 ||
            (target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100)) == 0 ||
            target->state == 0xfe || target->state == 0xfd ||
            target->state == vars->target_state) {
            play_moby_sound(vars->trigger_sound, 0, moby);
            moby->state = 2;
            return;
        }
        distance = vector_distance(&moby->pos, origin);
        step_size = frame_scale * 0.01f;
        if (distance <= step_size) {
            Vec4f *position = &moby->pos;
            position->x = vars->origin_x;
            position->y = vars->origin_y;
            position->z = vars->origin_z;
            position->w = 0.0f;
            voice = vars->sound_slot;
            if (voice != -1) {
                u8 *slot = (u8 *)D_0013E550 + voice * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0)
                    release_voice_slot(voice);
            }
            vars->sound_slot = -1;
            if (vars->arrival_sound != -1)
                play_moby_sound(vars->arrival_sound, 0, moby);
            if (vars->flag_index != -1)
                D_L07_00184528[vars->flag_index] = 0;
            moby->state = 1;
            break;
        }
        step.q = 0;
        step.f[1] = step_size;
        rotated.q = step.q;
        if (moby->oclass == 0x3f5 || moby->oclass == 0x429)
            rotated.f[1] = -rotated.f[1];
        transform_vector_by_basis(&step, &rotated, &moby->rot_row0);
        add_vector_xyz(&moby->pos, &moby->pos, &step);
        break;
    case 5:
        break;
    }
}
#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E8B8), where it is exact; names translated to the US level program. */

char *FUN_L07_0030ba90_c(char *pos, char *vec, int c, short d) __asm__("FUN_L07_0030ba90");
extern char *FUN_L00_0025a420(void *, int, int);
extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_0015ED70 __attribute__((section(".sdata")));
extern float D_L07_0015F580[] __attribute__((section(".sdata")));
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int D_L07_0015F5CC __attribute__((section(".sdata")));
extern int FUN_L00_00257b90(int, int);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int);
extern void *FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void FUN_L00_001fff28(void *, int, float);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025a120(void *);
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void get_target_sight(void *, void *, float) __asm__("FUN_L07_002886d8");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void mark_moby_for_removal_c3(void *) __asm__("func_0020C828");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
void attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");

void bombomatic_update(char *m) __asm__("FUN_L07_0030d4d8");
void bombomatic_update(char *m) {
    char *d;
    char *s16;
    char *t;
    int ia;
    int r;

    float lim;
    float tv;
    float v20[4];
    float v30[20];
    float v80[4];
    float v90[4];
    float vA0[4];
    float vB0[16];
    float vF0[4];
    float v100[4];

    if (m == 0) {
        return;
    }
    d = *(char **)(m + 0x78);
    if (((unsigned char *)m)[0x31] != 0) {
        if (fast_vec_dist(m + 0x10, D_L07_00166E40) < 27.0f) {
            FUN_L00_0025a120(m);
            m[0x7F] = 0x15;
        }
    }
    if (d == 0) {
        return;
    }
    s16 = FUN_L00_0025a420(m, 0x230000, 0);
    t = d + 0x20;
    r = fun_00213928(m, s16, t, 0, &ia, 0, 0, 4);
    switch (r) {
    case 0:
        break;
    case 1:
        *(int *)t = 0;
        break;
    case 2:
        *(int *)t = 0;
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    }

    if (ia >= 2) {
        char *o;
        if (s16 == 0) {
            lim = *(float *)(s16 + 0x2C);
        } else {
            o = *(char **)(s16 + 0x20);
            if (o == 0) {
                lim = *(float *)(s16 + 0x2C);
            } else {
                if (*(short *)(o + 0xA6) == 0x372) {
                    goto eb20;
                }
                lim = *(float *)(s16 + 0x2C);
            }
        }
        tv = *(float *)t;
        if (tv <= lim) {
            FUN_L00_00257470(m, 0, -1);
            *(float *)t = 0.0f;
            clear_u64_value(v20);
            FUN_L00_0025e450_c(m, v20, 0, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, 1.0f, 1, 15.0f, 1, 1, -1, 0);
            FUN_L00_00263fd8(m, 0x621, (float *)(m + 0x10), m + 0x40, 0, 0, 0.0f, D_L07_0015F580, D_L07_0015F580, D_L07_0015F580);
            FUN_L00_00263fd8(m, 0x622, (float *)(m + 0x10), m + 0x40, 0, 0, 0.0f, D_L07_0015F580, D_L07_0015F580, D_L07_0015F580);
            mark_moby_for_removal_c3(m);
            return;
        }
        tv -= lim;
        *(float *)t = tv;
        *(unsigned char *)(d + 0x67) = 0xFA;
        *(short *)(d + 0x26) = scale_game_frames(60);
        trooper_anim_update(m, (short *)(d + 0x60));
    }
eb20:
    ((unsigned char *)m)[0xA4] = 0xFF;
    trooper_anim_finish(m, d + 0x60);
    get_target_sight(m, v30, 20.0f);
    switch (((unsigned char *)m)[0x20]) {
    case 0: {
            int r1 = scale_game_frames(*(int *)(d + 0xB0));
            int r2;
            int r3;
            r2 = FUN_L00_00257b90(0, 10);
            r3 = scale_game_frames(r2);
            *(int *)(d + 0xB4) = D_L07_0015F5CC + r1 + r3;
            attach_manipulator(m, 0, d + 0x70);
            *(int *)(d + 0xC0) = 0;
            allocate_voice_for_target_entry(0, 4, (int)m);
            m[0x20] = 1;
            *(int *)(d + 0xC4) = 0;
        break;
    }
    case 1: {
        if (*(int *)(d + 0xB4) <= D_L07_0015F5CC) {
            int r9 = scale_game_frames(0xB4);
            *(int *)(d + 0xB4) = D_L07_0015F5CC + r9;
            *(short *)(d + 0xB8) = 0;
            *(float *)(d + 0xBC) = random_angle_radians_c();
            *(short *)(d + 0xBA) = 0;
            m[0x20] = 2;
        }
        break;
    }
    case 2: {
            int i;
            if (fast_dec_timer_s16(d + 0xB8) != 0) {
                *(short *)(d + 0xB8) = scale_game_frames(7);
                if (((unsigned char *)m)[0x31] == 0) {
                    if (!(vector_distance_xy_alt(m + 0x10, D_L07_00166E40) < 90.0f && (*(int *)(d + 0xC4) & 1) != 0)) {
                        if (!(vector_distance_xy_alt(m + 0x10, D_L07_00166E40) < 40.0f)) {
                            goto eeE4;
                        }
                    }
                }
                *(int *)(d + 0xC4) ^= 1;
                if (vector_distance_xy_alt(m + 0x10, v30) < 20.0f) {
                    *(float *)(d + 0xBC) = fast_arc_tan_c(v30[0] - *(float *)(m + 0x10), v30[1] - *(float *)(m + 0x14));
                }
                v20[0] = random_float_between_c(3.0f, 11.0f);
                v20[1] = random_float_between_c(-2.0f, 2.0f);
                v20[2] = 0.0f;
                vF0[3] = -1.0f;
                {
                    float best = 3.1415927f;
                    float h;
                    for (i = 1; i < 4; i++) {
                        float f;
                        float g;
                        FUN_L00_0024f7c8(m, i, v100);
                        f = fast_arc_tan_c(v100[0] - *(float *)(m + 0x10), v100[1] - *(float *)(m + 0x14));
                        g = fast_subtract_rotations(*(float *)(d + 0xBC), D_0015ED6C_gp * 17.80235863f);
                        f = fast_difference_between_rotations_c(f, g);
                        if (f < best) {
                            qcopy(vF0, v100);
                            best = f;
                        }
                    }
                    qcopy(vA0, vF0);
                }
                qzero(v80);
                v80[2] = *(float *)(d + 0xBC);
                euler_rotation_matrix(vB0, v80);
                transform_vector(v20, v20, vB0);
                fast_vec_normalize_c(v90, v20, D_0015ED6C_gp * 7.0f);
                add_vector_xyz(v20, v20, m + 0x10);
                v90[2] = FUN_L00_0025abf0(vA0, v20, 0, D_0015ED6C_gp * 7.0f, -(D_0015ED70 * 9.8f));
                add_vector_xyz(vA0, vA0, v90);
                FUN_L07_0030ba90_c(vA0, v90, (int)m, *(short *)(d + 0xBA));
                *(short *)(d + 0xBA) = *(short *)(d + 0xBA) + 1;
            }
        eeE4:
            if (*(int *)(d + 0xB4) <= D_L07_0015F5CC) {
                int r9 = scale_game_frames(0x78);
                *(int *)(d + 0xB4) = D_L07_0015F5CC + r9;
                m[0x20] = 1;
            }
        break;
    }
    }

    if (((unsigned char *)m)[0x20] != 0) {
        float f = D_0015ED6C_gp * 17.80235863f;
        float g = fast_add_rotations_c(*(float *)(d + 0xC0), f);
        *(float *)(d + 0xC0) = g;
        FUN_L00_001fff28(d + 0x80, 2, g);
    }
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030EF90), where it is exact; names translated to the US level program. */

extern s32 scale_game_frames(s32) __asm__("func_001F96F8");

unsigned char *FUN_L07_0030dbb0(int a, void *pos, int b, float f0, float f1, float f2, float f3,
                                float f4) {
    unsigned char *moby = (unsigned char *)func_0020D348_m(0x416);
    if (moby != 0) {
        char *data;
        char *g;
        char *h;
        data = *(char **)(moby + 0x78);
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        qcopy(moby + 0x10, pos);
        *(int *)(data + 0x14) = a;
        *(float *)(data + 4) = f1;
        *(float *)(data + 0x10) = f2;
        *(float *)(data + 0xC) = f3;
        *(float *)data = f0;
        *(float *)(data + 8) = f0;
        *(int *)(data + 0x18) = b;
        *(float *)(data + 0x1C) = f4;
        *(unsigned short *)(moby + 0x34) |= 0x200;
        g = D_L07_00166E40;
        h = g - 0x140;
        *(float *)(h + 0x160) = 0.3f - vector_distance(moby + 0x10, g) / 100.0f;
        *(int *)(h + 0x168) = scale_game_frames(0x14);
        FUN_L00_00250df8(moby);
        FUN_0022da68(0, 0, (int)moby);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F0D0), where it is exact; names translated to the US level program. */

typedef u32 u128_0F0D0 __attribute__((mode(TI), aligned(16)));

typedef struct { char b[0x90]; } __attribute__((aligned(16))) Pkt_0F0D0;

extern float split_float_fraction(void *, float) __asm__("FUN_L00_00200228");
extern int D_L07_0015F5CC;
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void fast_draw_quad_real(void *, int, int) __asm__("func_001F7D30");

void FUN_L07_0030dcf0(unsigned char *moby) {
    char packet[0x90] __attribute__((aligned(16)));
    char second[0x90] __attribute__((aligned(16)));
    char phase_work[0x10] __attribute__((aligned(16)));
    unsigned int alpha = (unsigned int)moby[0x23] / 5u;
    int index = 0;
    unsigned int green = 0x80;
    float band = 0.0f;
    char *data = *(char **)(moby + 0x78);
    unsigned int green_step = (green / 11) & 0xFF;
    float radius_threshold = 0.9f;
    float inner_scale = 0.96f;
    float pi = 3.1415927f;
    *(long *)(packet + 0x78) = get_effect_texture(*(int *)(data + 0x18));
    *(long *)(packet + 0x80) = 0xFF9000000260L;
    *(long *)(packet + 0x70) = 4;
    *(long *)(packet + 0x88) = 0x8000000048L;
    do {
        float zero, radius, side, center, angle, inner_radius, width, phase, ft;
        unsigned int gb = (green << 8) | 0x80;
        int color = ((alpha & 0xFF) << 24) | gb | (green << 16);
        *(int *)(packet + 0x4C) = color;
        *(int *)(packet + 0x48) = color;
        *(int *)(packet + 0x44) = color;
        *(int *)(packet + 0x40) = color;
        if (*(float *)(data + 8) < radius_threshold)
            radius = *(float *)(data + 8) * band;
        else
            radius = *(float *)(data + 8) - band * radius_threshold;
        side = (1.0f - band * 0.5f) * *(float *)(data + 0x10);
        ft = convert_integer_to_float(D_L07_0015F5CC);
        phase = split_float_fraction(phase_work, ft / (band * 40.0f + 40.0f));
        if ((index & 1) == 0)
            phase = -phase;
        *(float *)(packet + 0x50) = phase;
        *(float *)(packet + 0x60) = phase;
        width = convert_integer_to_float((index + 1) < 6 ? index + 1 : 5);
        center = *(float *)(moby + 0x10);
        *(float *)(packet + 0x58) = *(float *)(packet + 0x50) + width;
        {int iz = 0; zero = *(float *)&iz;}
        *(float *)(packet + 0x54) = zero;
        *(float *)(packet + 0x64) = 1.0f;
        *(float *)(packet + 0x14) = *(float *)(moby + 0x14);
        *(float *)(packet + 0x18) = side + *(float *)(moby + 0x18);
        *(float *)(packet + 0x38) = *(float *)(moby + 0x18);
        *(float *)(packet + 0x10) = center - radius * (inner_scale - band * 0.03f);
        *(float *)(packet + 0x6C) = 1.0f;
        *(float *)(packet + 0x34) = *(float *)(packet + 0x14);
        *(float *)(packet + 0x30) = center - radius;
        *(float *)(packet + 0x68) = *(float *)(packet + 0x58);
        *(float *)(packet + 0x5C) = zero;
        *(float *)(packet + 0x08) = side + *(float *)(moby + 0x18);
        *(float *)(packet + 0x28) = *(float *)(moby + 0x18);
        *(Pkt_0F0D0 *)second = *(Pkt_0F0D0 *)packet;
        *(float *)(second + 0x18) = *(float *)(moby + 0x18) - 5.0f;
        *(float *)(second + 8) = *(float *)(second + 0x18);
        angle = -2.8274336f;
        do {
            float x, y, rr;
            *(u128_0F0D0 *)(packet + 0) = *(u128_0F0D0 *)(packet + 0x10);
            *(u128_0F0D0 *)(packet + 0x20) = *(u128_0F0D0 *)(packet + 0x30);
            x = fast_cos(angle);
            rr = band * 0.03f;
            inner_radius = radius * (inner_scale - rr);
            *(float *)(packet + 0x10) = *(float *)(moby + 0x10) + inner_radius * x;
            x = fast_cos(angle);
            *(float *)(packet + 0x30) = *(float *)(moby + 0x10) + radius * x;
            y = fast_sin(angle);
            *(float *)(packet + 0x14) = *(float *)(moby + 0x14) + inner_radius * y;
            y = fast_sin(angle);
            angle = angle + 0.31415927f;
            *(float *)(packet + 0x34) = *(float *)(moby + 0x14) + radius * y;
            fast_draw_quad_real(packet, 0, 1);
            *(u128_0F0D0 *)(second + 0) = *(u128_0F0D0 *)(second + 0x10);
            *(u128_0F0D0 *)(second + 0x20) = *(u128_0F0D0 *)(second + 0x30);
            *(float *)(second + 0x30) = *(float *)(packet + 0x30);
            *(float *)(second + 0x34) = *(float *)(packet + 0x34);
            *(float *)(second + 0x10) = *(float *)(packet + 0x10);
            *(float *)(second + 0x14) = *(float *)(packet + 0x14);
            fast_draw_quad_real(second, 0, 1);
        } while (angle < pi);
        index++;
        band = band + 0.1f;
        green = green - green_step;
        green &= 0xFF;
    } while (band < 1.0f);
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F838), where it is exact; names translated to the US level program. */

extern int D_L07_0015F5CC;
extern int D_L07_00173ED8;
extern int FUN_001efa68(void *, void *, int, int, int);
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int FUN_L00_0025e450();
extern void FUN_L07_0029b8d8(char *, int, float, float);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int func_L00_001F10E0_f(float, void *, int, int) __asm__("FUN_L00_001f0d60");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");

extern char *D_L07_00173ED0[];
void FUN_L07_0030e458(struct Moby *m) {
    char *d = (char *)m->pvars;
    float prev[4];
    qcopy(prev, &m->pos);
    add_vector_xyz(&m->pos, &m->pos, d);
    {
        float g = frame_time_sq * 9.8f;
        float r = frame_time * 6.2831855f;
        *(float *)(d + 8) -= g;
        m->rot.z += r;
    }
    if (m->scale < m->pclass->scale * 2.8f) {
        m->scale *= 1.2f;
    }
    if (D_L07_0015F5CC % 3 == 0) {
        FUN_L07_0029b8d8(m, 0, 400000.0f, 5.0f);
    }
    if (FUN_001efa68(prev, &m->pos, 0, *(int *)(d + 0x10), 0) != 0 ||
        func_L00_001F10E0_f(0.1f, &m->pos, 0, *(int *)(d + 0x10)) != 0) {
        char *o = D_L07_00173ED0[2];
        if (o == 0 || (*(short *)(o + 0xA6) != 0x415 && *(short *)(o + 0xA6) != 0x419 &&
                       *(short *)(o + 0xA6) != 0x452)) {
            if (m->visible != 0) {
                func_L00_0025F4A8_alt(m, d, 0, 2.0f, 1.0f, 0, 3, 5, 0.0f, 0.0f, 1000.0f, 1.2f, 0,
                                      10.0f, 0, 1, -1, 0);
            } else {
                allocate_voice_for_target_entry(0, 0, (int)m);
            }
            mark_moby_for_removal(m);
            return;
        }
    }
    if (m->pos.x < 2.0f || 1021.0f < m->pos.x ||
        m->pos.y < 2.0f || 1021.0f < m->pos.y ||
        m->pos.z < 2.0f || 1021.0f < m->pos.z) {
        mark_moby_for_removal(m);
    }
}
#include "rnc/gameplay/entities/moby.h"

/* Knockback/launch parameters handed to trooper_start_knockback. */
typedef struct {
    u8 pad0[0x10];
    f32 gravity;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s32 flags;
    s32 unk24;
    f32 unk28;
    u8 pad2C[4];
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8 pad3C;
    u8 unk3D;
    u8 pad3E[0xA];
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
} LaunchParams;

extern void trooper_knockback_arc(Vec4f *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(f32, struct Moby *, LaunchParams *, s32, s32, s32) __asm__("FUN_L00_0025c558");

void FUN_L07_0030e720(struct Moby *moby, LaunchParams *l, Vec4f *dir) {
    Vec4f d;
    f32 ang;

    l->gravity = frame_time_sq * 29.400002f;
    l->unk14 = 0.0005f;
    l->unk18 = frame_time * 9.0f;
    l->unk1C = frame_time * 7.0f;
    l->flags = 0x400;
    l->unk24 = 13;
    l->unk28 = 1.0f;
    l->unk30 = 0.24f;
    l->unk34 = 0.5f;
    l->unk38 = 0.65f;
    l->unk48 = 0.2f;
    l->unk4C = 0.01f;
    l->unk50 = 3.0f;
    l->unk54 = 9.0f;
    l->unk3D = 0;
    *(u128 *)&d = *(u128 *)dir;
    trooper_knockback_arc(&d, &ang, &l->unk18, &l->unk1C);
    trooper_start_knockback(ang, moby, l, 12, 1, 0);
}

void FUN_L07_0030e858(struct Moby *moby, LaunchParams *l, Vec4f *dir) {
    Vec4f d;
    f32 ang;

    l->gravity = frame_time_sq * 29.400002f;
    l->unk14 = 0.0005f;
    l->unk18 = frame_time * 9.0f;
    l->unk1C = frame_time * 7.0f;
    l->flags = 0x599;
    l->unk24 = 13;
    l->unk28 = 1.4f;
    l->unk30 = 0.24f;
    l->unk34 = 0.5f;
    l->unk38 = 0.65f;
    l->unk48 = 0.2f;
    l->unk4C = 0.01f;
    l->unk50 = 8.0f;
    l->unk54 = 19.0f;
    l->unk3D = 0;
    *(u128 *)&d = *(u128 *)dir;
    trooper_knockback_arc(&d, &ang, &l->unk18, &l->unk1C);
    trooper_start_knockback(ang, moby, l, 6, 1, 0);
}

#define NOT_SDA

#define MACRO_ADDR

/* Hit handler: reacts to the hit type, then takes damage and dies (state 0xE) when health runs out. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030FD78), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

void FUN_L07_0030e998(char *m, char *x, float *hp) {
    char *info;
    int hit;
    int r;
    if (((unsigned char *)m)[0x20] == 0xE)
        return;
    info = find_moby_hit(m, 0x330000, 0);
    r = hit_reaction(m, info, hp, 0, &hit, 0, 0, 4);
    if (((unsigned char *)m)[0xBC] != 0) {
        switch (r) {
        case 0:
            break;
        case 1:
        case 2:
            *hp = 0.0f;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            FUN_L07_0030e858(m, x + 0x70, info + 0x10);
            m[0x20] = 0xC;
            break;
        case 9:
        case 10:
            if (((unsigned char *)m)[0x53] != 4) {
                blend_moby_animation(m, 4, 4, 4);
            }
            m[0x20] = 0xD;
            break;
        case 11:
            break;
        }
        if (hit >= 2) {
            if (*hp <= *(float *)(info + 0x2C)) {
                *hp = 0.0f;
                *(unsigned short *)(m + 0x34) &= 0xEFFF;
                FUN_L07_0030e720(m, x + 0x70, info + 0x10);
                FUN_L00_00257470(m, 0, -1);
                x[0x67] = 0x78;
                trooper_anim_update(m, (short *)(x + 0x60));
                m[0x20] = 0xE;
            } else {
                *hp -= *(float *)(info + 0x2C);
                ((unsigned char *)x)[0x67] = 0xFA;
                *(short *)(x + 0x26) = scale_game_frames(0x3C);
                trooper_anim_update(m, (short *)(x + 0x60));
            }
        }
    }
    ((unsigned char *)m)[0xA4] = 0xFF;
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030FF18), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");

void FUN_L07_0030eb38(char *data) {
    int i, count;
    if (data != 0) {
        count = *(int *)data;
        i = 0;
        if (count > 0) {
            do {
                int o = i * 16;
                int next = (i + 1) % count;
                *(float *)(data + o + 0x1C) =
                    fast_vec_dist(data + (o + 0x10), data + (next * 16 + 0x10));
                i++;
                count = *(int *)data;
            } while (i < count);
        }
    }
}

extern char *D_L07_0015FFD8_30ed30 __asm__("D_L07_0015FFD8");
extern char *D_L07_001600EC_30ed30 __asm__("D_L07_001600EC");
char D_L07_00211720[] = "Illegal Bounding path for moby 1059 instance #%d.\n";
char D_L07_00211758[] = "Illegal Hero Detect path for moby 1059 instance #%d.\n";
char D_L07_00211790[] = "Illegal water moby for moby 1059 instance #%d.\n";
char D_L07_002117C0[] = "Illegal guard point cuboid for moby 1059 instance #%d.\n";
extern void stub_printf(char *, int) __asm__("FUN_001e93b0");
extern int rand_e50(void) __asm__("FUN_001160d8");

/* Sets up the walker from its path, target and spot ids; reports a missing id and returns 1, else returns 0. */
int FUN_L07_0030ed30(char *moby, char *data) {
    if (*(int *)(data + 0x110) == -1) {
        stub_printf(D_L07_00211720, (moby - D_L07_0015FFD8_30ed30) >> 8);
        return 1;
    }
    FUN_L07_0030eb38(*(char **)(data + 0x128) = D_L07_001B0530[*(int *)(data + 0x110)]);
    if (*(int *)(data + 0x144) == -1) {
        stub_printf(D_L07_00211758, (moby - D_L07_0015FFD8_30ed30) >> 8);
        return 1;
    }
    FUN_L07_0030eb38(*(char **)(data + 0x148) = D_L07_001B0530[*(int *)(data + 0x144)]);
    if (*(int *)(data + 0x118) == -1) {
        stub_printf(D_L07_00211790, (moby - D_L07_0015FFD8_30ed30) >> 8);
        return 1;
    }
    *(char **)(data + 0x12C) = D_L07_0015FFD8_30ed30 + (*(int *)(data + 0x118) << 8) + 0x18;
    moby[0xBC] = FUN_L07_0030ebc8(moby, data);
    if (*(int *)(data + 0x114) == -1) {
        stub_printf(D_L07_002117C0, (moby - D_L07_0015FFD8_30ed30) >> 8);
        return 1;
    }
    qcopy(data + 0xF0, &D_L07_001600EC_30ed30[*(int *)(data + 0x114) * 0x80 + 0x30]);
    qcopy(data + 0xE0, moby + 0x10);
    ((void (*)(char *, char *))FUN_L07_0030ec58)(moby, data);
    *(int *)(data + 0x134) = rand_e50() % 0x78;
    if (rand_e50() & 1) {
        *(float *)(data + 0x140) = 0.34906584f;
    } else {
        *(float *)(data + 0x140) = -0.34906584f;
    }
    *(int *)(data + 0x14C) = 0;
    data[0x29] = 1;
    return 0;
}

/* Picks the walker heading: keeps going while the probe ahead stays on its path and clear,
 * else sweeps by its turn step for a clear heading, else faces the goal. */
struct WalkerPath {
    s32 count;
    u8 pad4[0xC];
    Vec4 pts[1];
};

struct WalkerData {
    u8 pad0[0xF0];
    Vec4 goal;                 /* 0xF0 */
    u8 pad100[0x28];
    struct WalkerPath *path;   /* 0x128 */
    u8 pad12C[8];
    s32 wander;                /* 0x134 */
    u8 pad138[4];
    f32 heading;               /* 0x13C */
    f32 turnStep;              /* 0x140 */
};

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f99c0(float);
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int line_blocked_30eee8(void *, void *, int, struct Moby *, int) __asm__("FUN_001efa68");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

void FUN_L07_0030eee8(struct Moby *moby, struct WalkerData *data) {
    float probe[4];
    float angle;
    float swept;
    float step;

    if (data->wander == 0) {
        data->heading = fast_arc_tan(data->goal.f[0] - moby->pos.x, data->goal.f[1] - moby->pos.y);
        return;
    }
    {
        probe[0] = fast_cos(moby->rot.z) * 5.2f;
        probe[1] = fast_sin(moby->rot.z) * 5.2f;
        *(s32 *)&probe[2] = 0;
        add_vector_xyz(probe, probe, &moby->pos);
        if (point_in_path_xy(probe, data->path->pts, data->path->count) &&
            !line_blocked_30eee8(&moby->pos, probe, 0x24, moby, 0) &&
            !coll_sphere(1.5f, probe, 0x24, moby))
            return;
        step = FUN_001f99c0(data->turnStep);
        angle = moby->rot.z;
        for (swept = 0.0f; swept < 6.2831855f; swept += step) {
            angle = fast_add_rotations(angle, data->turnStep);
            probe[0] = fast_cos(angle) * 5.2f;
            probe[1] = fast_sin(angle) * 5.2f;
            *(s32 *)&probe[2] = 0;
            add_vector_xyz(probe, probe, &moby->pos);
            if (point_in_path_xy(probe, data->path->pts, data->path->count) &&
                !line_blocked_30eee8(&moby->pos, probe, 0x24, moby, 0) &&
                !coll_sphere(1.5f, probe, 0x24, moby)) {
                data->heading = angle;
                return;
            }
        }
    }
    data->heading = fast_arc_tan(data->goal.f[0] - moby->pos.x, data->goal.f[1] - moby->pos.y);
}

extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern int probe_world_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

/* In the given state and animation, when the walker is within range, pushes it along its
 * velocity; a hit on the target bounces it back up and switches to the next state. */
void FUN_L07_0030f308(char *moby, char *obj, int state, int anim, int seq, int frame, int next, int id,
                      float lo, float hi, float len, float scale, float radius) {
    float v[4];
    float m[12];
    float p[4];
    float d;
    char *vel;

    d = compute_interpolated_record_value(moby);
    if (((unsigned char *)moby)[0x20] != state) {
        return;
    }
    if (((unsigned char *)moby)[0x52] != anim) {
        return;
    }
    if (!(lo < d)) {
        return;
    }
    if (!(d < hi)) {
        return;
    }
    vel = obj + 0xD0;
    fast_vec_normalize(v, vel, len);
    v[2] = 1.0f;
    make_hit_descriptor((char *)m, (int)moby, 1, v, scale);
    moby_point_position(moby, 0, p);
    if (probe_world_sphere(p, radius, 0x21, moby, m) == 0) {
        return;
    }
    if (D_L07_00173ED8 != id) {
        return;
    }
    fast_vec_scale(vel, vel, -1.0f);
    /* *&obj gives obj the register retail uses */
    *(float *)(*&obj + 0xD8) = -(frame_scale * 0.05f);
    if (((unsigned char *)moby)[0x53] != seq) {
        blend_moby_animation(moby, seq, frame, 5);
    }
    moby[0x20] = next;
}

/* Finds another moby whose state lies within the inclusive range. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00310920), where it is exact; names translated to the US level program. */

extern int FUN_L00_002591d0(int *, int, int, int);
extern int FUN_L00_002592b8(int *, int, int, int);

int FUN_L07_0030f540(unsigned char *moby, int lower, int upper) {
    unsigned char *other = 0;
    if (!FUN_L00_002591d0((int *)&other, moby[0x21], 0, 0)) {
        if (other) {
            do {
                if (other != moby) {
                    int state = other[0x20];
                    if (state >= lower) {
                        if (state <= upper) {
                            goto found;
                        }
                    }
                }
                if (FUN_L00_002592b8((int *)&other, (int)other, 0, 0)) break;
            } while (other);
        }
    }
    return 0;
found:
    return 1;
}
/* Update function of the sharkigator (class 1059): tracks the hero, swims its path, lunges and bites. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003109D0), where it is exact; names translated to the US level program. */


struct WalkerTarget {
    float pos[4];
    u128 q10;
    u128 q20;
    u128 q30;
    int moby;
    int kind;
};

extern Rec_30D8E8 * D_L07_001612D0;
extern char D_0013E533[];
extern char D_L07_002117F8[];
extern char D_L07_00211838[];
extern f32 fast_arc_tan_c2(f32, f32) __asm__("func_001F9E90");
extern f32 random_angle_radians_c2(void) __asm__("func_00213308");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern float FUN_L00_00258110(float *vel, float cur, float target, float k, float d, float max);
extern float resolve_camera_surface_height(void *, int) __asm__("func_002135F0");
extern int D_L07_0015F5CC;
extern int D_L07_001612D8;
extern int DebugPrint_alt() __asm__("FUN_001e93b0");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int FUN_L00_00259740(void *, void *, int);
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L07_0030ebc8_c(char *moby, char *data) __asm__("FUN_L07_0030ebc8");
extern int is_point_inside_clip_volume_c2(void *arg0, int arg1) __asm__("func_00214720");
extern int tick_countdown_32_alt(int *arg0) __asm__("FUN_001f9740");
extern s32 rand(void);
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern unsigned char D_L07_00161B50_gp[1] __asm__("D_L07_00161B50") __attribute__((sda));
extern unsigned char D_L07_00161B40_gp[1] __asm__("D_L07_00161B40") __attribute__((sda));
extern unsigned char *FUN_L00_002715e8(void *, int, int, float, float);
extern void FUN_L00_002223f8(int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025e450_c2(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263ac8(void *, int, void *, float);
extern void FUN_L07_0030ec58_c(char *arg) __asm__("FUN_L07_0030ec58");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");
extern void mark_moby_for_removal_c4(void *) __asm__("func_0020C828");
extern void no_op_main_callback_alt(void *, void *) __asm__("FUN_001e93e8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L07_0030ed30_c(char *moby, char *data) __asm__("FUN_L07_0030ed30");
int FUN_L07_0030f1e0_c(char *moby, char *obj, float a, float lo, float up, float hi, float down, float vmax, float vmin) __asm__("FUN_L07_0030f1e0");
int FUN_L07_0030f4d0_c(void *moby, int id, float scale) __asm__("FUN_L07_0030f4d0");
int FUN_L07_0030f540_c(unsigned char *moby, int lower, int upper) __asm__("FUN_L07_0030f540");
void FUN_L00_002a3ec8_c(void *grids, s32 count, s32 mode, f32 x, f32 z, f32 radius, f32 amount) __asm__("FUN_L00_002a3ec8");
void FUN_L07_0030e998_c(char *m, char *x, float *hp) __asm__("FUN_L07_0030e998");
void FUN_L07_0030eee8_c(struct Moby *moby, struct WalkerData *data) __asm__("FUN_L07_0030eee8");
void FUN_L07_0030f0f8_c(char *a, char *b, float f12, float f13, float f14) __asm__("FUN_L07_0030f0f8");
void FUN_L07_0030f308_c(char *moby, char *obj, int state, int anim, int seq, int frame, int next, int id, float lo, float hi, float len, float scale, float radius) __asm__("FUN_L07_0030f308");
extern char D_0013F65C[];
extern char D_0013F350[];
extern void func_L00_002A5158_30D8E8(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");

#define func_L00_002592B0_t(m,a,v,k,d,x) ((void (*)(char *, float, float *, float, float, float))FUN_L00_00258278)(m,a,v,k,d,x)

void sharkigator_update(unsigned char *moby) __asm__("FUN_L07_0030f5f0");
void sharkigator_update(unsigned char *moby) {
    char *data;
    struct WalkerTarget target;
    float point[4];
    float offset[4];
    float goal[4];
    float delta[4];
    float previous[4];
    int wander;
    int inside;
    int next_sequence;
    int next_state;
    unsigned char old_command;
    unsigned char command;
    char *hero;

    if (moby == 0) {
        return;
    }
    data = *(char **)(moby + 0x78);
    if (data == 0) {
        return;
    }
    FUN_L00_00263ac8(moby, 0, data + 0x150, 2.2f);
    if (moby[0x20] == 0) {
        if (FUN_L07_0030ed30_c((char *)moby, data)) {
            DebugPrint_alt(D_L07_002117F8, (int)((char *)moby - (char *)level_mobys) >> 8);
            mark_moby_for_removal_c4(moby);
            return;
        }
        goto end;
    }
    get_target_sight(moby, &target, 30.0f);
    FUN_L07_0030e998_c((char *)moby, data, (float *)(data + 0x20));
    trooper_anim_finish(moby, data + 0x60);
    wander = tick_countdown_32_alt((int *)(data + 0x134)) != 0;
    if (*(int *)(data + 0x110) != 0 && moby[0x20] >= 7) {
        char *path = *(char **)(data + 0x148);
        inside = FUN_L00_00259740(&target, path + 0x10, *(int *)path) != 0;
    } else {
        char *path = *(char **)(data + 0x148);
        hero = (char *)&D_0013F3D0;
        inside = FUN_L00_00259740(hero, path + 0x10, *(int *)path) != 0;
        if (vector_distance_xy_alt(moby + 0x10, hero) < 30.0f) {
            target.moby = *(int *)(hero + 0x2000);
            qcopy(&target.pos, hero);
            qcopy(&target.q10, hero + 0x10);
            qcopy(&target.q20, hero + 0x40);
            qcopy(&target.q30, hero + 0x50);
            target.kind = 0;
        } else {
            target.moby = 0;
            qzero(&target.pos);
            qzero(&target.q10);
            qzero(&target.q30);
            qzero(&target.q20);
            target.kind = 2;
        }
    }
    if (*(int *)(data + 0x110) != 0) {
        char *path = *(char **)(data + 0x128);
        FUN_L00_00259740(moby + 0x10, path + 0x10, *(int *)path);
    }
    old_command = moby[0xBC];
    next_state = 0xFF;
    next_sequence = 0xFF;
    moby[0xBC] = FUN_L07_0030ebc8_c((char *)moby, data);
    command = moby[0xBC];
    if (old_command != command) {
        next_state = ((struct { unsigned char t[3][3]; } *)D_L07_00161B40_gp)->t[old_command][command];
        next_sequence = ((struct { unsigned char t[3][3]; } *)D_L07_00161B50_gp)->t[old_command][command];
    }
    FUN_L00_0024f7c8(moby, 0, point);
    if (coll_sphere(0.8f, point, 1, moby) != 0 && collision_hit_material() == 0) {
        float height;
        float scale;
        height = **(float **)(data + 0x12C);
        height -= F(data, 0x130);
        height *= 0.5f;
        if (height < absolute_float(F(moby, 0x18) - point[2])) {
            height = (**(float **)(data + 0x12C) - F(data, 0x130)) * 0.5f;
        } else {
            height = absolute_float(F(moby, 0x18) - point[2]);
        }
        if (F(data, 0xD8) < 0.0f) {
            height = -height;
        }
        if (moby[0x20] == 0xE || moby[0x20] == 0xC) {
            height += height;
        }
        if (0.5f < vector_distance_xy_alt(moby + 0x10, (Vec4 *)&D_L07_00173EE0)) {
            scale = vector_distance_xy_alt(moby + 0x10, (Vec4 *)&D_L07_00173EE0);
            scale += scale;
        } else {
            scale = 1.0f;
        }
        if (D_L07_0015F5CC % 45 == *(short *)(moby + 0xB2) && random_integer_below(2) == 0) {
            func_L00_002A5158_30D8E8(D_L07_001612D0, D_L07_001612D8, 1, F(moby, 0x10), F(moby, 0x14), scale, height);
        }
        if (D_L07_0015F5CC % ((rand() & 7) + 20) == 0) {
            float size = random_float_between_c2(1.5f, 2.4f);
            point[0] += random_float_between_c2(-0.15f, 0.15f);
            point[1] += random_float_between_c2(-0.15f, 0.15f);
            point[2] = resolve_camera_surface_height((Vec4 *)&D_L07_00173EE0, 0);
            FUN_L00_002715e8(point, 0, -1, size, 5250.0f);
        }
    }
    if (1.0f <= F(data, 0x14C)) {
        *(u128 *)&D_0013F3D0 = *(u128 *)(moby + 0x10);
    }

    switch (moby[0x20]) {
    case 0:
        DebugPrint_alt(D_L07_00211838);
        goto end;
    case 1:
        if ((moby[0x70] & 2) && moby[0x53] != 0) {
            blend_moby_animation(moby, 0, 0, 10);
        }
        if (inside && target.moby != 0 && target.moby == *(int *)&D_001413D0 &&
            !FUN_L07_0030f540_c(moby, 4, 6)) {
            if (moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 10);
            }
            moby[0x20] = 4;
        }
        if (target.moby != 0 && target.moby == *(int *)&D_001413D0 &&
            FUN_L07_0030f4d0_c(moby, target.moby, 1.5f)) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, 2);
            }
            moby[0x20] = 5;
        } else if (wander) {
            if (moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 200 + 300;
            F(data, 0x13C) = random_angle_radians_c2();
            moby[0x20] = 2;
        }
        fast_vec_scale_c(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        FUN_L07_0030f0f8_c((char *)moby, data, frame_time_sq * 15.0f, frame_time_sq * 50.0f, frame_time * 8.0f);
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case 2:
        if ((moby[0x70] & 2) && moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        if (inside && target.moby != 0 && target.moby == *(int *)&D_001413D0) {
            if (!FUN_L07_0030f540_c(moby, 4, 6)) {
                if (moby[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, 10);
                }
                moby[0x20] = 4;
            }
        } else {
            float stiffness = 1.74532926f;
            float damping = 4.5378561f;
            FUN_L00_00258278((char *)moby, (float *)(data + 0x120), F(data, 0x13C),
                              frame_time_sq * stiffness, frame_time_sq * damping, frame_time * stiffness);
            F(moby, 0x44) = FUN_L00_00258110((float *)(data + 0x124), F(moby, 0x44), 0.0f,
                                              frame_time_sq * stiffness, frame_time_sq * damping, frame_time * stiffness);
            if (wander && is_point_inside_clip_volume_c2(moby + 0x10, *(int *)(data + 0x114))) {
                if (moby[0x53] != 0) {
                    blend_moby_animation(moby, 0, 0, 10);
                }
                *(int *)(data + 0x134) = rand() % 120 + 60;
                moby[0x20] = 1;
            }
        }
        FUN_L07_0030eee8_c((struct Moby *)moby, (struct WalkerData *)data);
        if (FUN_L07_0030f1e0_c((char *)moby, data, F(data, 0x13C), 0.209439516f, frame_time_sq * 6.0f,
                              0.52359879f, frame_time_sq * 4.0f, frame_time * 7.0f, frame_time * 3.0f) &&
            wander) {
            if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 120 + 60;
            moby[0x20] = 1;
        }
        if (target.moby != 0 && target.moby == *(int *)&D_001413D0 &&
            FUN_L07_0030f4d0_c(moby, target.moby, 1.5f)) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, 2);
            }
            moby[0x20] = 5;
        }
        FUN_L07_0030f0f8_c((char *)moby, data, frame_time_sq * 15.0f, frame_time_sq * 50.0f, frame_time * 8.0f);
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case 4: {
        float angle;
        float pitch;
        int close = 1;
        float pi4 = 12.566371f;
        float three = 3.0f;
        float zero = 0.0f;
        angle = fast_arc_tan_c2(target.pos[0] - F(moby, 0x10), target.pos[1] - F(moby, 0x14));
        FUN_L00_00258278((char *)moby, (float *)(data + 0x120), angle,
                          frame_time_sq * pi4, frame_time_sq * zero, frame_time * pi4);
        pitch = vector_distance_xy_alt(moby + 0x10, &target);
        pitch = fast_arc_tan_c2(pitch, target.pos[2] - F(moby, 0x18));
        F(moby, 0x44) = FUN_L00_00258110((float *)(data + 0x124), F(moby, 0x44), -pitch,
                                          frame_time_sq * pi4, frame_time_sq * zero, frame_time * pi4);
        if (!(vector_distance_xy_alt(moby + 0x10, ((char *)&D_0013F3D0)) < three)) {
            close = 0;
        }
        if (close) {
            fast_vec_scale_c(data + 0xD0, data + 0xD0, frame_scale * (0.9f - 1.0f) + 1.0f);
        }
        if (FUN_L07_0030f1e0_c((char *)moby, data, angle, 0.209439516f, frame_time_sq * 20.0f,
                              0.52359879f, frame_time_sq * three, frame_time * 20.0f, frame_time * 5.0f) &&
            close) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 4, 2);
            }
            *(int *)(moby + 0x94) = 0;
            moby[0x20] = 5;
            F(data, 0x14C) = zero;
            qcopy(data + 0x100, moby + 0x10);
        }
        ((struct { char pad[0x138]; float f; } *)data)->f = target.pos[2] - 0.25f;
        FUN_L07_0030f0f8_c((char *)moby, data, frame_time_sq * 38.0f, frame_time_sq * 0.0f, frame_time * 38.0f);
        if (!inside || *(short *)&D_0013F65C == 0) {
            if (moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 10);
            }
            moby[0x20] = 2;
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    }
    case 5: {
        float angle;
        char *mpos;
        char *start;
        float one;
        FUN_L00_0024f7c8(moby, 0, point);
        mpos = (char *)moby + 0x10;
        subtract_vector_xyz(offset, point, mpos);
        angle = fast_arc_tan_c2(target.pos[0] - point[0], target.pos[1] - point[1]);
        func_L00_002592B0_t((char *)moby, angle, (float *)(data + 0x120),
                          frame_time_sq * 12.566371f, frame_time_sq * 0.0f, frame_time * 12.566371f);
        F(moby, 0x44) *= frame_scale * 0.7f;
        F(data, 0x14C) += 0.028f;
        qcopy(previous, mpos);
        subtract_vector_xyz(goal, &target, offset);
        start = data + 0x100;
        subtract_vector_xyz(delta, goal, start);
        one = 1.0f;
        fast_vec_scale_c(delta, delta, F(data, 0x14C) < one ? F(data, 0x14C) : one);
        add_vector_xyz(mpos, delta, start);
        no_op_main_callback_alt(previous, mpos);
        no_op_main_callback_alt(start, mpos);
        subtract_vector_xyz(data + 0xD0, mpos, previous);
        if (one <= F(data, 0x14C)) {
            char *hero_bytes = (char *)&D_0013F350;
            *(int *)(hero_bytes + 0x22A8) = 0;
            FUN_L00_002223f8(0x3D, 1);
            *(int *)(*(char **)(hero_bytes + 0x2080) + 0x98) = -1;
            hero_bytes[0x20A5] = 1;
            moby[0x20] = 6;
        } else {
            F(data, 0x13C) = angle;
        }
        if (inside || *(short *)&D_0013F65C != 0) {
            goto end;
        }
        *(int *)(data + 0x14C) = 0;
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        moby[0x20] = 2;
        goto end;
    }
    case 6: {
        float seven = 7.0f;
        float pi = 3.14159274f;
        float pi2 = 1.57079637f;
        FUN_L07_0030eee8_c((struct Moby *)moby, (struct WalkerData *)data);
        FUN_L07_0030f1e0_c((char *)moby, data, F(data, 0x13C), 0.209439516f, frame_time_sq * 6.0f,
                          0.52359879f, frame_time_sq * 4.0f, frame_time * seven, frame_time * 3.0f);
        FUN_L07_0030f0f8_c((char *)moby, data, frame_time_sq * 12.0f, frame_time_sq * 50.0f, frame_time * seven);
        FUN_L00_00258278((char *)moby, (float *)(data + 0x120), F(data, 0x13C),
                          frame_time_sq * pi2, frame_time_sq * pi, frame_time * pi);
        F(moby, 0x44) = FUN_L00_00258110((float *)(data + 0x124), F(moby, 0x44), 0.0f,
                                          frame_time_sq * pi2, frame_time_sq * pi, frame_time * pi);
        if ((moby[0x70] & 2) && moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    }
    case 7:
        if (inside) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            moby[0x20] = 9;
        } else if (wander) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 200 + 300;
            F(data, 0x13C) = random_angle_radians_c2();
            moby[0x20] = 8;
        }
        fast_vec_scale_c(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        F(data, 0xD8) -= frame_time * 9.8f;
        if (FUN_L00_00258ad0((int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f)) {
            fast_vec_scale_c(data + 0xD0, data + 0xD0, 0.5f);
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case 8:
        if (inside) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            moby[0x20] = 9;
        } else {
            float stiffness = 0.785398185f;
            float damping = 1.57079637f;
            FUN_L00_00258278((char *)moby, (float *)(data + 0x120), F(data, 0x13C),
                              frame_time_sq * stiffness, frame_time_sq * damping, frame_time * stiffness);
            F(moby, 0x44) = FUN_L00_00258110((float *)(data + 0x124), F(moby, 0x44), 0.0f,
                                              frame_time_sq * stiffness, frame_time_sq * damping, frame_time * stiffness);
            if (wander && is_point_inside_clip_volume_c2(moby + 0x10, *(int *)(data + 0x114))) {
                if (moby[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, 10);
                }
                *(int *)(data + 0x134) = rand() % 120 + 60;
                moby[0x20] = 7;
            }
        }
        FUN_L07_0030eee8_c((struct Moby *)moby, (struct WalkerData *)data);
        if (FUN_L07_0030f1e0_c((char *)moby, data, F(data, 0x13C), 0.34906584f, frame_time_sq * 3.0f,
                              1.04719758f, frame_time_sq * 9.0f, frame_time, frame_time) &&
            wander) {
            if (moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 120 + 60;
            moby[0x20] = 7;
        }
        F(data, 0xD8) -= frame_time * 9.8f;
        if (FUN_L00_00258ad0((int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f)) {
            fast_vec_scale_c(data + 0xD0, data + 0xD0, 0.5f);
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case 9:
    case 10: {
        float angle;
        float pi2 = 1.57079637f;
        angle = fast_arc_tan_c2(target.pos[0] - F(moby, 0x10), target.pos[1] - F(moby, 0x14));
        func_L00_002592B0_t((char *)moby, angle, (float *)(data + 0x120),
                          frame_time_sq * 0.785398185f, frame_time_sq * pi2, frame_time * 0.785398185f);
        if (FUN_L07_0030f1e0_c((char *)moby, data, angle, 0.52359879f, frame_time_sq * 4.0f,
                              pi2, frame_time_sq * 9.0f, frame_time * 1.5f, frame_time) &&
            vector_distance_xy_alt(moby + 0x10, &target) < 1.9f && moby[0x20] != 10) {
            if (moby[0x53] != 5) {
                blend_moby_animation(moby, 5, 0, 2);
            }
            moby[0x20] = 10;
        }
        F(data, 0xD8) -= frame_time * 9.8f;
        if (FUN_L00_00258ad0((int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f)) {
            fast_vec_scale_c(data + 0xD0, data + 0xD0, 0.5f);
        }
        data[0x58] = func_001FA898_r(8.0f);
        data[0x5A] = func_001FA898_r(9.6f);
        if (!inside) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            moby[0x20] = 8;
        }
        ((void (*)(char *, float, char *, float, int, float, int, float, float, int, int, int, int))FUN_L07_0030f308_c)((char *)moby, 17.0f, data, 20.0f, 10, 1.0f, 5, 1.0f, 1.0f, 4, 4, 11, target.moby);
        if (moby[0x70] & 2) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 200 + 300;
            moby[0x20] = 8;
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    }
    case 11:
        if (moby[0x70] & 2) {
            moby[0x20] = 7;
        }
        goto end;
    case 12:
        if (mine_fall(moby, data + 0x70) & 1) {
            ((void (*)(char *, char *))FUN_L07_0030ec58_c)((char *)moby, data);
        }
        goto end;
    case 13:
        if (moby[0x70] & 2) {
            ((void (*)(char *, char *))FUN_L07_0030ec58_c)((char *)moby, data);
        }
        goto end;
    case 14:
        if (mine_fall(moby, data + 0x70) & (moby[0x70] & 2)) {
            FUN_L00_0025e450_c2(moby, data + 0xD0, 0, 0.0f, 0.0f, 3, 3, 5, 2.0f, 1.0f, 4.0f, 1.0f, -1, 7.0f,
                              0, 1, -1, 0);
            mark_moby_for_removal_c4(moby);
            return;
        }
        goto end;
    case 16:
        fast_vec_scale_c(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        F(data, 0xD8) -= frame_time * 9.8f;
        FUN_L00_00258ad0((int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f);
        if (moby[0x70] & 2) {
            ((void (*)(char *, char *))FUN_L07_0030ec58_c)((char *)moby, data);
        }
        goto end;
    case 15:
        fast_vec_scale_c(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        F(data, 0xD8) -= frame_time * 9.8f;
        FUN_L00_00258ad0((int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f);
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case 17:
        fast_vec_scale_c(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        if (moby[0x70] & 2) {
            ((void (*)(char *, char *))FUN_L07_0030ec58_c)((char *)moby, data);
        }
        goto end;
    default:
        goto end;
    }
end:
    if (F(moby, 0x10) < 2.0f || 1021.0f < F(moby, 0x10) || F(moby, 0x14) < 2.0f ||
        1021.0f < F(moby, 0x14) || F(moby, 0x18) < 2.0f || 1021.0f < F(moby, 0x18)) {
        mark_moby_for_removal_c4(moby);
    }
}
extern int rand(void) __asm__("FUN_001160d8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern float FUN_L00_0025bc00(float *, float, int, float);
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void FUN_L07_00288968(void *, void *, void *, void *, void *);
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern float dot_vector_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void mark_moby_for_removal_c(void *) __asm__("FUN_0020c828");
extern char *D_L07_0015FFD8_3112c8 __asm__("D_L07_0015FFD8");
extern char D_0013F3D0[];
extern char D_0013F440[];
extern float D_0013F448;


void FUN_L07_003112c8(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *parent = 0;
    char *parent_data = 0;
    float previous[4];
    float delta[4];
    float rotation[4];
    float scratch[4];
    float current_rotation[4];
    float current_position[4];
    float transformed[4];
    float target;
    float height;
    float sway;
    float roll;
    float speed;
    int timer;

    qcopy(previous, moby + 0x10);
    scale_vector_xyz(delta, moby + 0x10, -1.0f);
    qcopy(rotation, moby + 0x40);
    if (*(int *)(data + 0xA0) != -1) {
        parent = D_L07_0015FFD8_3112c8 + *(int *)(data + 0xA0) * 0x100;
        parent_data = *(char **)(parent + 0x78);
    }
    if (((unsigned char *)moby)[0xBC] == 2) {
        moby[0x20] = 2;
        moby[0xBC] = 3;
        *(int *)(data + 0xAC) = ((unsigned char *)data)[0xBD];
    }
    switch (((unsigned char *)moby)[0x20]) {
    case 0:
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 13;
        timer = rand();
        if (timer & 1)
            *(float *)(moby + 0x48) = fast_add_rotations(*(float *)(moby + 0x48), 3.1415927f);
        *(float *)(data + 0xA4) = *(float *)(moby + 0x40);
        *(float *)(data + 0xA8) = *(float *)(moby + 0x44);
        *(int *)(data + 0xC8) = -1;
        moby[0x20] = 1;
        moby[0xBC] = 0;
        if (((unsigned char *)moby)[0xB0] != 0xFF) {
            if (visited52[((unsigned char *)moby)[0xB0] + current_level_index * 16] != 0xFF)
                break;
            *(float *)(moby + 0x18) -= 5.0f;
            if (((unsigned char *)data)[0xBC] == 0) {
                mark_moby_for_removal_c(moby);
                return;
            }
            moby[0x20] = 5;
        }
        break;
    case 1:
        if (((unsigned char *)moby)[0xBC] != 1) break;
        if (*(int *)(data + 0xAC) == 0)
            *(int *)(data + 0xAC) = rand() % 10;
        if (((unsigned char *)data)[0xBE] != 0)
            FUN_L01_0026e090(((unsigned char *)moby)[0x21], 2);
        *(float *)(moby + 0x40) = fast_add_rotations(*(float *)(data + 0xA4),
            fast_sin(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.75f)) * 0.017453292f);
        *(float *)(moby + 0x44) = fast_add_rotations(*(float *)(data + 0xA8),
            fast_cos(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.89f)) * 0.017453292f);
        break;
    case 2:
        *(float *)(moby + 0x40) = fast_add_rotations(*(float *)(data + 0xA4),
            fast_sin(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.75f)) * 0.017453292f);
        *(float *)(moby + 0x44) = fast_add_rotations(*(float *)(data + 0xA8),
            fast_cos(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.89f)) * 0.017453292f);
        if (!fast_dec_timer((int *)(data + 0xAC))) break;
        if (((unsigned char *)data)[0xBE] != 0) {
            parent_data[0x19D] = data[0xBF];
            parent[0x20] = 12;
        }
        moby[0x20] = 3;
        break;
    case 3:
        speed = 1.3962634f;
        speed -= *(float *)(moby + 0x40);
        speed *= 0.05f;
        *(float *)(moby + 0x40) += speed;
        {
            /* Retail takes the sine of the rotation step. */
            float descent = fast_sin(speed) * 3.1f;
            float gravity = frame_time_sq;
            float drop = 15.0f;
            *(float *)(moby + 0x18) -= descent;
            *(float *)(data + 0xB0) -= *(float *)(data + 0xB4) * gravity;
            *(float *)(moby + 0x18) += *(float *)(data + 0xB0);
            if (!(*(float *)(moby + 0x18) < *(float *)(data + 0xB8) - drop)) break;
            if (((unsigned char *)data)[0xBC] != 0) {
                *(float *)(data + 0xB0) = 0.0f;
                moby[0x20] = 4;
            } else {
                mark_moby_for_removal_c(moby);
                return;
            }
        }
        break;
    case 4:
        if (((unsigned char *)parent_data)[0xEF] < 2 && ((unsigned char *)parent)[0x20] > 11) break;
        *(float *)(data + 0xB0) = random_float_between_alt(frame_time, frame_time * 4.0f);
        *(float *)(data + 0xC0) = random_angle_radians();
        *(float *)(data + 0xC4) = random_angle_radians();
        moby[0x20] = 5;
        *(float *)(moby + 0x40) = 0.0f;
        *(float *)(moby + 0x44) = 0.0f;
        *(int *)(data + 0xAC) = 0;
        break;
    case 5:
        target = *(float *)(moby + 0x18) + *(float *)(data + 0xB0);
        height = *(float *)(data + 0xB8);
        if (hero.air_frames.s == 0 && (char *)hero.ground_moby == moby) {
            if (*(int *)(data + 0xAC) == 0)
                FUN_0022da68(0, 0, (int)moby);
            timer = scale_game_frames(90);
            if (*(int *)(data + 0xAC) < timer)
                *(int *)(data + 0xAC) = scale_game_frames(90);
        }
        if (*(int *)(data + 0xAC) != 0) {
            if (*(float *)(moby + 0x18) > *(float *)(data + 0xB8) - 2.6f) {
                *(float *)(data + 0xB0) = -(frame_time * 0.25f);
            } else {
                *(float *)(data + 0xB0) = 0.0f;
            }
        } else {
            if (target < height) {
                float correction = (height - target) * 0.2f;
                if (correction > 0.9f) correction = 0.9f;
                else if (correction < -0.9f) correction = -0.9f;
                *(float *)(data + 0xB0) += correction;
            }
            {
                float velocity = *(float *)(data + 0xB0) - frame_time_sq * 9.8f;
                float cap = frame_time * 5.0f;
                *(float *)(data + 0xB0) = velocity;
                if (cap < velocity)
                    *(float *)(data + 0xB0) = cap;
            }
        }
        *(float *)(moby + 0x18) += *(float *)(data + 0xB0);
        *(float *)(data + 0xC0) = fast_add_rotations(*(float *)(data + 0xC0), 0.01f);
        *(float *)(data + 0xC4) = fast_add_rotations(*(float *)(data + 0xC0), 0.017f);
        sway = fast_sin(*(float *)(data + 0xC0)) * 0.02f;
        roll = fast_sin(*(float *)(data + 0xC4)) * 0.02f;
        if (*(int *)(data + 0xAC) != 0) {
            float forward, side;
            fast_dec_timer((int *)(data + 0xAC));
            subtract_vector_xyz(scratch, &hero.motion.pos, moby + 0x10);
            forward = dot_vector_xyz(scratch, moby + 0xC0);
            side = dot_vector_xyz(scratch, moby + 0xD0) * -0.03f;
            forward *= 0.03f;
            sway = fast_add_rotations(sway, side);
            sway = fast_add_rotations(sway, forward);
            *(u128 *)current_rotation = *(u128 *)(moby + 0x40);
            *(u128 *)current_position = *(u128 *)(moby + 0x10);
            FUN_L07_00288968(&hero.motion.pos, transformed, moby + 0x10, current_rotation, moby + 0x40);
            subtract_vector_xyz(&hero.motion.platform_delta, transformed, &hero.motion.pos);
            hero.motion.platform_delta.f[2] += *(float *)(moby + 0x18) - current_position[2];
        }
        FUN_L00_0025bc00((float *)(moby + 0x40), sway, 0, frame_time * 0.17453292f);
        FUN_L00_0025bc00((float *)(moby + 0x44), roll, 0, frame_time * 0.17453292f);
        if (*(float *)(moby + 0x40) > 0.5235988f) *(float *)(moby + 0x40) = 0.5235988f;
        if (*(float *)(moby + 0x40) < -0.5235988f) *(float *)(moby + 0x40) = -0.5235988f;
        if (*(float *)(moby + 0x44) > 0.5235988f) *(float *)(moby + 0x44) = 0.5235988f;
        if (*(float *)(moby + 0x44) < -0.5235988f) *(float *)(moby + 0x44) = -0.5235988f;
        break;
    }
    if (((unsigned char *)moby)[0x20] == 5 && fast_dec_timer((int *)(data + 0xC8))) {
        float last = *(float *)(data + 0xCC);
        float height = *(float *)(data + 0xB8);
        if ((height <= last && *(float *)(moby + 0x18) < height) || (last <= height && height < *(float *)(moby + 0x18))) {
            FUN_0022da68(0, 0, (int)moby);
            *(int *)(data + 0xC8) = rand() % scale_game_frames(20) + scale_game_frames(20);
        }
    }
    *(float *)(data + 0xCC) = *(float *)(moby + 0x18);
    add_vector_xyz(delta, delta, moby + 0x10);
    update_entity_matrix(data + 0x60, delta, rotation, moby + 0x40);
}
/* Falling leaf: drops off its branch after a random delay, swaying, then falls faster and vanishes near the ground. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00312FA8), where it is exact; names translated to the US level program. */

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
s32 allocate_voice_for_target_entry_c(s32 entry_index, s32 flags,
                                      void *target) __asm__("FUN_0022da68");
void mark_moby_for_removal_c(void *obj) __asm__("FUN_0020c828");

void FUN_L07_00311bc8(char *m) {
    char *d;
    float prev[4];
    float v[4];
    float t;
    float g;
    if (m == 0)
        return;
    d = *(char **)(m + 0x78);
    if (d == 0)
        return;
    qcopy(prev, m + 0x10);
    switch (((unsigned char *)m)[0x20]) {
    case 0:
        *(int *)(d + 0x20) = 0;
        *(short *)(d + 0x24) = 0;
        d[0x28] = 4;
        *(short *)(d + 0x3E) = 0xD;
        set_moby_color(m, 0x50, 0x40, 0x10);
        *(float *)(m + 0x40) += random_float_between_alt(-0.034906585f, 0.034906585f);
        *(float *)(m + 0x44) += random_float_between_alt(-0.034906585f, 0.034906585f);
        *(float *)(m + 0x18) += random_float_between_alt(-0.02f, 0.02f);
        m[0x20] = 1;
        break;
    case 1:
        if (FUN_L01_00277fb8(m)) {
            *(float *)(d + 0xA8) = *(float *)(m + 0x40);
            *(float *)(d + 0xAC) = *(float *)(m + 0x44);
            *(short *)(d + 0xA0) = scale_game_frames(0x1E);
            if (*(short *)(d + 0xA2) != -1)
                allocate_voice_for_target_entry_c(*(short *)(d + 0xA2), 0, (int)m);
            m[0x20] = 2;
        }
        break;
    case 2:
        *(float *)(m + 0x40) =
            fast_sin(normalize_angle(convert_integer_to_float(*(short *)(d + 0xA0)) * 0.75f)) *
            DEG_TO_RAD;
        {
            float c =
                fast_cos(normalize_angle(convert_integer_to_float(*(short *)(d + 0xA0)) * 0.89f));
            float e = frame_scale;
            *(float *)(m + 0x44) = c * DEG_TO_RAD;
            *(float *)(m + 0x18) -= e * 0.002f;
        }
        if (fast_dec_timer_s16(d + 0xA0)) {
            *(float *)(d + 0xA4) = frame_scale * 0.002f;
            m[0x20] = 3;
        }
        break;
    case 3:
        t = (1.3962634f - *(float *)(m + 0x40)) * 0.05f;
        *(float *)(m + 0x40) += t;
        t = fast_sin(t);
        g = frame_time_sq * 9.8f;
        *(float *)(m + 0x18) -= t + t;
        *(float *)(d + 0xA4) += g;
        *(float *)(m + 0x18) -= *(float *)(d + 0xA4);
        if (*(float *)(m + 0x18) < 2.0f) {
            mark_moby_for_removal_c(m);
            return;
        }
        break;
    case 4:
        break;
    }
    subtract_vector_xyz(v, m + 0x10, prev);
    update_entity_matrix(d + 0x60, v, m + 0x40, m + 0x40);
}
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313298), where it is exact; names translated to the US level program. */

extern int FUN_L00_0023e5e0(float *, float, float, int);

void FUN_L07_00311eb8(float radius, void *moby_, void *obj_, short pa, void *pos, short b, unsigned int color, int ca) {
    char *moby = moby_;
    char *obj = obj_;
    float v[4];
    char *e;
    int y;
    if (pos == 0) {
        qcopy(v, moby + 0x10);
    } else {
        qcopy(v, pos);
    }
    if (*(short *)(obj + 0x280) == -1) {
        if (b > 0) {
            if (radius == -1.0f) {
                radius = 15.0f;
            }
            if ((int)color == -1) {
                color = 0x80004080U;
            }
            if (ca == -1) {
                ca = 0;
            }
            *(short *)(obj + 0x280) = FUN_L00_0023e5e0(v, radius, 0.0f, color);
            *(short *)(obj + 0x284) = 0;
        }
    }
    if (*(short *)(obj + 0x280) != -1) {
        e = D_L07_00180340 + (*(short *)(obj + 0x280) << 5);
        if (*(short *)(obj + 0x282) == pa && (pa == -1 || *(short *)(obj + 0x284) == 0) || *(short *)(obj + 0x282) != pa) {
            if (b != -1) {
                *(short *)(obj + 0x284) = b;
                *(short *)(obj + 0x286) = b;
            }
            if (radius != -1.0f) {
                *(float *)(e + 0x1C) = radius;
            }
        } else {
            if (b != -1) {
                y = *(short *)(obj + 0x286) * 7 / 8;
                if (!(y < *(short *)(obj + 0x284))) {
                    *(short *)(obj + 0x284) = y;
                }
                if (radius != -1.0f) {
                    *(float *)(e + 0x1C) = radius;
                }
            } else if (radius != -1.0f) {
                *(float *)(e + 0x1C) = radius;
            }
        }
        if ((int)color != -1) {
            *(int *)(obj + 0x288) = color;
            *(float *)e = convert_integer_to_float(color & 0xFF) * 0.0078125f;
            *(float *)(e + 4) = convert_integer_to_float(((int)color >> 8) & 0xFF) * 0.0078125f;
            *(float *)(e + 8) = convert_integer_to_float(((int)color >> 16) & 0xFF) * 0.0078125f;
        }
        if (ca != -1) {
            *(int *)(obj + 0x28C) = ca;
        }
        if (pos != 0) {
            qcopy(e + 0x10, pos);
        }
        if (pa != -1) {
            *(short *)(obj + 0x282) = pa;
        }
    }
}
/* Ported from rac1-decomp src/overlays/l07_umbris/vendor_002CE470.c (func_L07_00313800) */
#include "sda.h"

/* Umbris: sets up the effect vectors from the moby's data block and emits the GS packet. */

typedef int u128_30CA88 __attribute__((mode(TI)));

typedef union {
    u128_30CA88 q;
    float f[4];
} Vec4_30CA88;

typedef struct EffData_313800 {
    char pad0[0x140];
    float f140[4];
    float f150[3];
    float f15C;
} EffData_313800;


typedef struct Packet_313800 {
    float a[4];
    float b[4];
    float c[4];
    float d[4];
    unsigned int col[4];
    float uv[8];
    long x120;
    long x128;
    long x130;
    long x138;
} Packet_313800;

extern int D_L07_0015F5CC_lui __asm__("D_L07_0015F5CC");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
unsigned char *FUN_L00_002729c8(void *pos, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, int g, float x, float y);
extern float FUN_001fa6c0_2cd2b0(int) __asm__("FUN_001fa6c0");
extern float FUN_L00_0025e310_2cd2b0(float) __asm__("FUN_L00_0025e310");
extern float FUN_001f9dc8_2cd2b0(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2cd2b0(float) __asm__("FUN_001f9de0");
extern void add_vector_xyz_cf(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_001f9bf8_2cd2b0(void *, void *, float) __asm__("FUN_001f9bf8");
extern int FUN_001efa68_2cd2b0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int FUN_001f96f8_2cd2b0(int) __asm__("FUN_001f96f8");
extern void *FUN_L00_002729c8_2cd2b0(float *pos, unsigned char a1, unsigned char a2, unsigned char a3,
    unsigned char a4, unsigned char a5, void *a6, float f0, float f1) __asm__("FUN_L00_002729c8");
extern long FUN_001f44b8_2cd2b0(int) __asm__("FUN_001f44b8");
extern char D_L07_00167050_2cd2b0[] __asm__("D_L07_00167050");
extern char D_L07_00173EE0_2cd2b0[] __asm__("D_L07_00173EE0");

void FUN_L07_00312420(struct Moby *arg)
{
    float v0[4], v10[4], v20[4], v30[4], v40[4], v50[4], m[4], c2[4], h[4], ii[4], jj[4];
    float f20, t;
    int flag = 1;
    EffData_313800 *p = ((EffData_313800 *)arg->pvars);
    int k;
    Packet_313800 pkt;

    f20 = FUN_001fa6c0_2cd2b0(D_L07_0015F5CC_lui);
    qcopy(v50, p->f140);
    t = FUN_001f9dc8_2cd2b0(FUN_L00_0025e310_2cd2b0(f20 / 11.0f));
    f20 = f20 / 10.0f;
    t = FUN_L00_0025e310_2cd2b0(f20 + t * 1.5707964f);
    v50[0] = v50[0] + FUN_001f9de0_2cd2b0(t);
    add_vector_xyz_cf(arg, 2, v30);
    subtract_vector_xyz(v10, v50, v30);
    FUN_001f9bf8_2cd2b0(v10, v10, 30.0f);
    add_vector_xyz(v0, v50, v10);
    if (FUN_001efa68_2cd2b0(v30, v0, 2, arg, 0)) {
        qcopy(v0, D_L07_00173EE0_2cd2b0);
    } else {
        flag = 0;
    }
    subtract_vector_xyz(v40, v30, v0);
    cross_vectors_xyz(v20, D_L07_00167050_2cd2b0, v40);
    FUN_001f9bf8_2cd2b0(v20, v20, 0.25f);
    add_vector_xyz(v10, v20, v0);
    if (flag) {
        FUN_L00_002729c8_2cd2b0(v0, 0, FUN_001f96f8_2cd2b0((rand_e50() & 1) + 0x2D),
            FUN_001f96f8_2cd2b0((rand_e50() & 7) + 0x37), 1,
            FUN_001f96f8_2cd2b0((rand_e50() & 3) + 0xD), arg, 1.0f, 1.0f);
        if (p->f15C != 0.0f) {
            subtract_vector_xyz(c2, v0, p->f150);
            FUN_001f9bf8_2cd2b0(c2, c2, 0.2f);
            qcopy(m, p->f150);
            for (k = 0; k < 4; k++) {
                add_vector_xyz(m, m, c2);
                subtract_vector_xyz(h, m, v30);
                FUN_001f9bf8_2cd2b0(h, h, 5.0f);
                subtract_vector_xyz(ii, m, h);
                add_vector_xyz(jj, m, h);
                if (FUN_001efa68_2cd2b0(ii, jj, 2, arg, 0)) {
                    qcopy(m, D_L07_00173EE0_2cd2b0);
                }
                if (k == 2) {
                    FUN_L00_002729c8_2cd2b0(m, 0, FUN_001f96f8_2cd2b0((rand_e50() & 1) + 0x2D),
                        FUN_001f96f8_2cd2b0((rand_e50() & 7) + 0x37), 1,
                        FUN_001f96f8_2cd2b0((rand_e50() & 3) + 0xD), arg, 1.0f, 1.0f);
                } else {
                    FUN_L00_002729c8_2cd2b0(m, 1, FUN_001f96f8_2cd2b0(0x2D),
                        FUN_001f96f8_2cd2b0(0x37), 1,
                        FUN_001f96f8_2cd2b0(0xD), arg, 1.0f, 1.0f);
                }
            }
        }
        qcopy(p->f150, v0);
        p->f15C = 1.0f;
    } else {
        p->f15C = 0.0f;
    }
    pkt.x128 = FUN_001f44b8_2cd2b0(0x3B);
    pkt.col[0] = pkt.col[1] = pkt.col[2] = pkt.col[3] = 0x80808080;
    pkt.x130 = 0xFF9000000260;
    pkt.x120 = 4;
    pkt.x138 = 0x8000000048;
    pkt.uv[4] = pkt.uv[0] = 0.0f;
    pkt.uv[6] = pkt.uv[2] = 1.0f;
    pkt.uv[3] = pkt.uv[1] = 0.0f;
    pkt.uv[7] = pkt.uv[5] = 1.0f;
    qcopy_nc(pkt.a, v30);
    add_vector_xyz(pkt.b, pkt.a, v20);
    qcopy(pkt.c, v0);
    qcopy(pkt.d, v10);
    draw_geometry_quad(&pkt, 0, 1);
}

s32 D_L07_001CA928[1] __attribute__((section(".data"))) = {0};
s32 D_L07_001CA92C[1] __attribute__((section(".data"))) = {0};
f32 D_L07_001CA930[1] __attribute__((section(".data"))) = {0};
f32 D_L07_001CA934[1] __attribute__((section(".data"))) = {0};
f32 D_L07_001CA938[1] __attribute__((section(".data"))) = {0};

char D_L07_00211580[] = "Moby 871 short on mines at %d in group %d compared to mine count in pVar %d\n";

char D_L07_002117F8[] = "Initialization failed for moby 1059 instance #%d. Deleting.\n";
