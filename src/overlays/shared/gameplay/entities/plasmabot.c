/* Shared moby classes: plasmabot, plasmabot_hatch, mine. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/entities.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B44B8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int *D_L14_001B0BB0[];
extern int FUN_L00_0025ff38(char *, char *, float, int, int, int *, int);

void FUN_L14_002b3268(char *moby) {
    char *data = *(char **)(moby + 0x78);
    int *tbl = D_L14_001B0BB0[*(int *)(data + 0x244)];
    if (FUN_L00_0025ff38(moby, data + 0xD0, *(float *)(data + 0x234), 0, 0, tbl + 4, *tbl) != 2) {
        if (*(float *)(data + 0x234) < vector_distance(moby + 0x10, data + 0xD0)) {
            *(int *)(data + 0x114) = 2;
        }
    }
    if (*(int *)(data + 0x110) == 0) {
        *(int *)(data + 0x110) = *(int *)(D_0013E533 + 0x2E9D);
    }
}
/* 0x002b3318, 24 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Manipulate moby flags and clear field
void FUN_L14_002b3318(char *moby) {
    unsigned short *field = (unsigned short *)(moby + 0x34);
    *field = (*field | 0x41) & 0xEFFF;
    *(int *)(moby + 0x94) = 0;
}

/* 0x002b3330, 36 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Clears two flag bits, sets another, and copies a word from the moby's linked record.
void FUN_L14_002b3330(char *moby) {
    unsigned short *field = (unsigned short *)(moby + 0x34);
    int v;
    *field &= 0xFFBE;
    v = *(int *)(*(char **)(moby + 0x24) + 0x10);
    *field |= 0x1000;
    *(int *)(moby + 0x94) = v;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B45A8), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");

void FUN_L14_002b3358(char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    char *path;
    int i;
    *(int *)(moby_vars + 0x250) = 0;
    *(int *)(moby_vars + 0x24C) = 0;
    *(float *)(moby_vars + 0x25C) = *(float *)(moby_vars + 0x250);
    *(float *)(moby_vars + 0x258) = *(float *)(moby_vars + 0x250);
    *(float *)(moby_vars + 0x254) = *(float *)(moby_vars + 0x250);
    if (*(int *)(moby_vars + 0x240) == -1)
        return;
    path = (char *)D_L14_001B0BB0[*(int *)(moby_vars + 0x240)];
    for (i = 0; i < *(int *)path; i++) {
        int point_offset = i * 16;
        *(float *)(path + point_offset + 0x1C) =
            fast_vec_dist(path + (point_offset + 0x10), path + (((i + 1) % *(int *)path) * 16 + 0x10));
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B47C8), where it is exact; names translated to the US level program. */


extern float D_L14_001614DC __attribute__((sda));
void FUN_L14_002b3578(char *moby) {
    float planar_velocity_scale = (D_L14_001614DC - 1.0f) * frame_scale + 1.0f;
    char *moby_vars = *(char **)(moby + 0x78);
    *(float *)(moby_vars + 0x254) = *(float *)(moby_vars + 0x254) * planar_velocity_scale;
    *(float *)(moby_vars + 0x258) = *(float *)(moby_vars + 0x258) * planar_velocity_scale;
    *(float *)(moby + 0x10) = *(float *)(moby + 0x10) + *(float *)(moby_vars + 0x254);
    *(float *)(moby + 0x14) = *(float *)(moby + 0x14) + *(float *)(moby_vars + 0x258);
}
/* steers the pitch angle of a moby toward a target and updates the joint */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4A00), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void FUN_L00_001fff28(void *, int, float);

void FUN_L14_002b37b0(char *moby, float x) {
    char *moby_vars = *(char **)(moby + 0x78);
    float target_joint_angle = -*(float *)(moby_vars + 0x268);
    float r = fast_subtract_rotations(x, *(float *)(moby + 0x48));
    target_joint_angle = fast_add_rotations(target_joint_angle, -r);
    if (target_joint_angle > 1.2217304706573486f)
        target_joint_angle = 1.2217304706573486f;
    else if (target_joint_angle < -1.2217304706573486f)
        target_joint_angle = -1.2217304706573486f;
    {
        float k = frame_time_sq * 12.566370964050293f;
        approach_angle((float *)(moby_vars + 0x270), (float *)(moby_vars + 0x274), target_joint_angle, k, k,
                         frame_time * 6.2831854820251465f);
    }
    FUN_L00_001fff28(moby_vars + 0x130, 2, *(float *)(moby_vars + 0x270));
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4AE0), where it is exact; names translated to the US level program. */

extern char *D_L14_ffd0_e_macro[] __asm__("D_L14_0015FFD0") __attribute__((section(".sdata")));
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern short *D_L14_001ABF40[];
extern int D_L14_001614E0;

void FUN_L14_002b3890(int idx) {
    short *p;
    if (idx != -1) {
        p = D_L14_001ABF40[idx];
        if (p != 0) {
            do {
                char *moby = D_L14_ffd0_e_macro[2] + ((*(unsigned short *)p & 0x7FFF) << 8);
                char *data = *(char **)(moby + 0x78);
                if (*(short *)(moby + 0xA6) == 0x1C) {
                    *(short *)(data + 0x260) = scale_game_frames(D_L14_001614E0);
                }
            } while (*p++ >= 0);
        }
    }
}
#include "qcopy.h"

/* Recomputes a path's segment lengths, aims the moby along its first segment, and recomputes a second path. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002FFD88), where it is exact; names translated to the US level program. */

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");

void FUN_L14_002fe900(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *path;
    int i;
    path = D_L14_001B0BB0[*(int *)(data + 0xD8)];
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) = fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    *(int *)(data + 0xD0) = 0;
    *(int *)(data + 0xD4) = 0;
    qcopy(moby + 0x10, path + 0x10);
    *(float *)(moby + 0x48) = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                           *(float *)(path + 0x24) - *(float *)(path + 0x14));
    *(float *)(data + 0xF0) = *(float *)(data + 0xE4) = *(float *)(data + 0xE8) =
        *(float *)(data + 0xEC) = 0.0f;
    if (*(int *)(data + 0xFC) != 1) {
        path = D_L14_001B0BB0[*(int *)(data + 0xDC)];
        for (i = 0; i < *(int *)path - 1; i++) {
            *(float *)(path + 0x1C + i * 0x10) =
                fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
        }
    }
}
/* rebuilds two vectors from the moby's transform and applies them to its linked object */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_003000B0), where it is exact; names translated to the US level program. */

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

extern float D_L14_00161FC4 __attribute__((sda));
extern float D_L14_00161FC8 __attribute__((sda));
void FUN_L14_002fec28(char *moby) {
    char *data = *(char **)(moby + 0x78);
    float position_offset[4];
    if (*(char **)(data + 0xF4) != 0) {
        char *linked_position;
        fast_vec_normalize(position_offset, moby + 0xC0, D_L14_00161FC4);
        fast_vec_add(*(char **)(data + 0xF4) + 0x10, moby + 0x10, position_offset);
        fast_vec_normalize(position_offset, moby + 0xE0, D_L14_00161FC8);
        linked_position = *(char **)(data + 0xF4) + 0x10;
        fast_vec_add(linked_position, linked_position, position_offset);
    }
}
/* Spawns moby class 0x3A5 at the given position, owned by owner, with three randomised angle values. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00300F80), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_00250df8(void *);

unsigned char *FUN_L14_002ffaf8(unsigned char *owner, char *pos) {
    unsigned char *spawned_moby = func_0020D348_m(0x3A5);
    if (spawned_moby != 0) {
        char *spawned_vars = *(char **)(spawned_moby + 0x78);
        *(unsigned char **)spawned_vars = owner;
        *(int *)(spawned_vars + 8) = 0;
        *(int *)(spawned_vars + 0xC) = 0;
        *(float *)(spawned_vars + 4) = -1.0f;
        *(float *)(spawned_vars + 0x10) = random_float_between_alt(0.0104719754f, 0.0209439509f);
        *(float *)(spawned_vars + 0x14) = random_float_between_alt(0.00698131695f, 0.0139626339f);
        *(float *)(spawned_vars + 0x18) = random_float_between_alt(0.00349065848f, 0.00698131695f);
        qcopy(spawned_moby + 0x10, pos);
        *(float *)(spawned_moby + 0x48) = *(float *)(owner + 0x48);
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}

#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* Plasmabot update: validates its path and target, walks the path, turns to aim at its owner's target and fires, dies or despawns. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B2A28), where it is exact; names translated to the US level program. */

typedef struct {
    float x;
    float y;
    float scale;
    float distance;
    char *moby;
    int flags;
    unsigned char mode;
    unsigned char enabled;
    unsigned short oclass;
    float strength;
    int count;
} LineInfo;

extern char D_001413F4[];
extern char D_L14_001FBB60[];
extern char D_L14_001FBBA0[];
extern char D_L14_001FBBF8[];
extern char D_L14_001FBC38[];
extern char D_L14_001FBC90[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations_c2(f32, f32) __asm__("func_001FA5C8");
extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height(float, void *, int);
extern int D_L14_0015F5C4;
extern int coll_line_fix(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261968(int, void *, void *);
extern int FUN_L00_002e1f28(void *, int, int);
extern int FUN_L00_002e2040(char *);
extern int FUN_L00_002e2190(void *, void *, void *);
extern int FUN_L14_002b39a0_c(unsigned char *) __asm__("FUN_L14_002b39a0");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern float D_L14_001614E4_gp __asm__("D_L14_001614E4") __attribute__((sda));
extern float D_L14_001614E8_gp __asm__("D_L14_001614E8") __attribute__((sda));
extern int D_L14_001614EC_gp __asm__("D_L14_001614EC") __attribute__((sda));
extern int D_L14_001614F0;
extern float D_L14_001614F4_gp __asm__("D_L14_001614F4") __attribute__((sda));
extern u8 D_L14_00174560[];
extern void DebugPrint(char *, ...);
extern void FUN_L00_0024f7c8(char *, int, void *);
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025e450_c(struct Moby *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263ac8(float, char *, int, char *);
extern void add_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_moby_bone_transform_c(char *, int, void *) __asm__("build_moby_bone_transform");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float FUN_L14_002b35d8_c(char *moby, int flag, float angle) __asm__("FUN_L14_002b35d8");
int FUN_L14_002b3418_c(char *moby) __asm__("FUN_L14_002b3418");
int FUN_L14_002b3920_c(int index) __asm__("FUN_L14_002b3920");
int FUN_L14_002d9870_c(int i) __asm__("FUN_L14_002d9870");
unsigned char *FUN_L14_002dea98_c(char *owner, char *pos) __asm__("FUN_L14_002dea98");
unsigned char *FUN_L14_002e0170_c(char *owner, float *scale, int mode) __asm__("FUN_L14_002e0170");
void FUN_L14_002b2600_c(struct Moby *m) __asm__("FUN_L14_002b2600");
void FUN_L14_002b2ed8_c(struct Moby *m) __asm__("FUN_L14_002b2ed8");
void FUN_L14_002b2f60_c(char *m) __asm__("FUN_L14_002b2f60");
void FUN_L14_002b3268_c(char *moby) __asm__("FUN_L14_002b3268");
void FUN_L14_002b3318_c(char *moby) __asm__("FUN_L14_002b3318");
void FUN_L14_002b3330_c(char *moby) __asm__("FUN_L14_002b3330");
void FUN_L14_002b3358_c(char *moby) __asm__("FUN_L14_002b3358");
void FUN_L14_002b3578_c(char *moby) __asm__("FUN_L14_002b3578");
void FUN_L14_002b37b0_c(char *moby, float angle) __asm__("FUN_L14_002b37b0");
void FUN_L14_002b3890_c(int idx) __asm__("FUN_L14_002b3890");
void attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");

#define call_4BF0(m) FUN_L14_002b39a0_c((unsigned char *)(m))
#define call_CE58 ((float (*)(float *, float, float *, float, float, float))approach_angle)
#define call_F4A8 ((void (*)(char *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int))FUN_L00_0025e450_c)
#define call_D678 ((void (*)(char *))mark_moby_for_removal_c)

void FUN_L14_002b17d8(char *moby);
void FUN_L14_002b17d8(char *moby) {
    enum {
        PLASMABOT_INITIALIZE = 0,
        PLASMABOT_WAIT_TO_START_PATH_TRAVERSAL = 1,
        PLASMABOT_START_PATH_TRAVERSAL = 2,
        PLASMABOT_FOLLOW_PATH_FORWARD = 3,
        PLASMABOT_AIM_AT_TARGET = 4,
        PLASMABOT_WAIT_ANIMATION_BEFORE_AIM_SWEEP = 5,
        PLASMABOT_SWEEP_AIM_AND_TRACE_LINE = 6,
        PLASMABOT_WAIT_ANIMATION_BEFORE_AIM = 7,
        PLASMABOT_FOLLOW_PATH_BACKWARD = 8,
        PLASMABOT_RETURN_TO_PATH_START = 9,
        PLASMABOT_WAIT_ANIMATION_BEFORE_RESUMING_AIM = 10,
        PLASMABOT_FALL_THEN_EXPLODE = 11,
        PLASMABOT_SPAWN_DEBRIS_AND_REMOVE = 12,
        PLASMABOT_FACE_TARGET_AND_WAIT = 13
    };
    char *data;
    char *owner;
    int state;
    float mat[4][4];
    float direction[4];
    float end[4];
    LineInfo info;
    int status;

    data = *(char **)(moby + 0x78);
    owner = 0;
    FUN_L00_00263ac8(2.5f, moby, 2, data + 0x120);
    if (*(int *)(data + 0x288) != 0) {
        if (((unsigned char *)D_001413F4)[0] != 2) {
            *(unsigned short *)(moby + 0x34) |= 0x41;
            *(int *)(moby + 0x94) = 0;
            if (((unsigned char *)moby)[0x53] == 5) {
                FUN_L00_002e2040(moby);
                FUN_L00_00257470(moby, 0, -1);
                { call_D678(moby); return; }
            }
            ((unsigned char *)moby)[0x30] = 0xFF;
            *(unsigned short *)(moby + 0x34) &= 0xEFFF;
            return;
        }
        *(unsigned short *)(moby + 0x34) &= 0xFFBE;
        if (((unsigned char *)moby)[0x20] != 0xB) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        }
        ((unsigned char *)moby)[0x30] = 0x40;
        *(unsigned short *)(moby + 0x34) |= 0x1000;
    }
    if (((unsigned char *)moby)[0x20] != 0) {
        FUN_L14_002b2f60_c(moby);
        FUN_L14_002b3268_c(moby);
        FUN_L14_002b2ed8_c((struct Moby *)moby);
        owner = *(char **)(data + 0x110);
    }
    if (D_L14_0015F5C4 == 2) {
        ((unsigned char *)moby)[0x31] = 0;
        *(unsigned short *)(moby + 0x34) |= 1;
    } else if (((unsigned char *)moby)[0x20] != 1) {
        ((unsigned char *)moby)[0x31] = 1;
        *(unsigned short *)(moby + 0x34) &= 0xFFFE;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case PLASMABOT_INITIALIZE: {
        if (((unsigned char *)data)[0x24A] != 0) {
            if (*(int *)(data + 0x240) == -1) {
                DebugPrint(D_L14_001FBB60, *(short *)(moby + 0xB2));
                { call_D678(moby); return; }
            }
            if (*D_L14_001B0BB0[*(int *)(data + 0x240)] == 0) {
                DebugPrint(D_L14_001FBBA0, *(short *)(moby + 0xB2));
                { call_D678(moby); return; }
            }
        }
        if (*(int *)(data + 0x244) == -1) {
            DebugPrint(D_L14_001FBBF8, *(short *)(moby + 0xB2));
            { call_D678(moby); return; }
        }
        if (*D_L14_001B0BB0[*(int *)(data + 0x244)] == 0) {
            DebugPrint(D_L14_001FBC38, *(short *)(moby + 0xB2));
            { call_D678(moby); return; }
        }
        if (((unsigned char *)data)[0x24A] != 0 && *(int *)(data + 0x264) < 0) {
            DebugPrint(D_L14_001FBC90, *(short *)(moby + 0xB2));
            { call_D678(moby); return; }
        }
        if (random_integer_below(2) != 0) {
            *(unsigned short *)(moby + 0x34) |= 0x8000;
        }
        FUN_L14_002b3358_c(moby);
        if (*(int *)(data + 0x240) != -1) {
            qcopy(moby + 0x10, (char *)D_L14_001B0BB0[*(int *)(data + 0x240)] + 0x10);
        }
        data[0x28] = 1;
        data[0x29] = 0;
        *(float *)(data + 0x20) = 2.0f;
        *(float *)(data + 0x30) = 1.25f;
        *(short *)(data + 0x24) = 2;
        data[0x58] = truncate_float_to_s32(15.0f);
        data[0x5A] = truncate_float_to_s32(16.0f);
        *(float *)(data + 0x27C) = 1.0f;
        *(short *)(data + 0x248) = 0;
        *(int *)(data + 0x23C) = 0;
        *(short *)(data + 0x260) = 0;
        *(int *)(data + 0x268) = 0;
        *(int *)(data + 0x26C) = 0;
        *(int *)(data + 0x280) = 0;
        *(int *)(data + 0x284) = 0;
        *(int *)(data + 0x270) = 0;
        *(int *)(data + 0x274) = 0;
        if (((unsigned char *)data)[0x121] == 0) {
            attach_manipulator(moby, 2, data + 0x120);
        }
        if (((unsigned char *)data)[0x161] == 0) {
            attach_manipulator(moby, 3, data + 0x160);
        }
        if (((unsigned char *)data)[0x24A] != 0) {
            FUN_L14_002b3318_c(moby);
            ((unsigned char *)moby)[0x20] = 1;
            return;
        }
        *(float *)(moby + 0x18) = probe_ground_height(0.5f, moby + 0x10, 0);
        if (*(int *)(data + 0x240) == -1) {
            ((unsigned char *)moby)[0x20] = 4;
        } else {
            ((unsigned char *)moby)[0x20] = 0xD;
        }
        return;
    }
    case PLASMABOT_WAIT_TO_START_PATH_TRAVERSAL: {
        float *path;
        if (*(int *)(data + 0x114) == 2) return;
        if (fast_dec_timer_s16(data + 0x248) == 0) return;
        if (FUN_L14_002d9870_c(*(int *)(data + 0x264)) == 0) return;
        if (call_4BF0(moby) == 0) return;
        *(short *)(data + 0x248) = scale_game_frames(D_L14_001614F0);
        FUN_L14_002b3330_c(moby);
        ((unsigned char *)moby)[0x20] = 2;
        set_moby_animation(moby, 1, 0);
        path = (float *)D_L14_001B0BB0[*(int *)(data + 0x240)];
        *(float *)(moby + 0x48) = fast_arc_tan_c(path[8] - path[4], path[9] - path[5]);
        *(int *)(data + 0x268) = 0;
        *(float *)(data + 0x27C) = 1.0f;
        *(int *)(data + 0x26C) = 0;
        *(int *)(data + 0x270) = 0;
        *(int *)(data + 0x274) = 0;
        *(int *)(data + 0x24C) = 0;
        *(int *)(data + 0x250) = 0;
        *(float *)(data + 0x25C) = *(float *)(data + 0x268);
        *(float *)(data + 0x258) = *(float *)(data + 0x268);
        *(float *)(data + 0x254) = *(float *)(data + 0x268);
        return;
    }
    case PLASMABOT_START_PATH_TRAVERSAL:
        if (compute_interpolated_record_value(moby) >= 3.0f) {
            FUN_L14_002b3418_c(moby);
        }
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            ((unsigned char *)moby)[0x20] = 3;
            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            *(float *)(moby + 0x58) = 1.0f;
        }
        break;
    case PLASMABOT_RETURN_TO_PATH_START: {
        float *path = (float *)D_L14_001B0BB0[*(int *)(data + 0x240)];
        vendor_spring((float *)(moby + 0x10), (float *)(data + 0x254), path[4], 0.01f, 0.2f, 0.0f);
        vendor_spring((float *)(moby + 0x14), (float *)(data + 0x258), path[5], 0.01f, 0.2f, 0.0f);
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 1;
            *(float *)(moby + 0x58) = 1.0f;
            FUN_L14_002b3318_c(moby);
            return;
        }
        break;
    }
    case PLASMABOT_FACE_TARGET_AND_WAIT: {
        float ang;
        float k;
        ang = fast_arc_tan_c(*(float *)(owner + 0x10) - *(float *)(moby + 0x10),
                                *(float *)(owner + 0x14) - *(float *)(moby + 0x14));
        k = frame_time_sq * 12.566371f;
        call_CE58((float *)(moby + 0x48), ang, (float *)(data + 0x23C), k, k, frame_time * 6.2831855f);
        if (*(int *)(data + 0x114) == 2) break;
        if (fast_dec_timer_s16(data + 0x248) == 0) break;
        ((unsigned char *)moby)[0x20] = 3;
        blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        *(float *)(moby + 0x58) = 1.0f;
        *(int *)(data + 0x250) = 0;
        *(float *)(data + 0x27C) = 1.0f;
        *(int *)(data + 0x24C) = 0;
        *(float *)(data + 0x25C) = *(float *)(data + 0x250);
        *(float *)(data + 0x258) = *(float *)(data + 0x250);
        *(float *)(data + 0x254) = *(float *)(data + 0x250);
        break;
    }
    case PLASMABOT_FOLLOW_PATH_FORWARD:
        if (FUN_L14_002b3418_c(moby) != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 4;
        }
        break;
    case PLASMABOT_FOLLOW_PATH_BACKWARD:
        if (((unsigned char *)data)[0x24A] != 0) {
            FUN_L14_002d9870_c(*(int *)(data + 0x264));
        }
        if (FUN_L14_002b3418_c(moby) != 0) {
            if (((unsigned char *)data)[0x24A] != 0) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
                ((unsigned char *)moby)[0x20] = 9;
            } else {
                *(short *)(data + 0x248) = scale_game_frames(D_L14_001614F0);
                ((unsigned char *)moby)[0x20] = 0xD;
            }
        }
        break;
    case PLASMABOT_AIM_AT_TARGET: {
        float ang;
        float k;
        float aim;
        FUN_L14_002b3578_c(moby);
        ang = fast_arc_tan_c(*(float *)(owner + 0x10) - *(float *)(moby + 0x10),
                                *(float *)(owner + 0x14) - *(float *)(moby + 0x14));
        k = frame_time_sq * 12.566371f;
        call_CE58((float *)(moby + 0x48), ang, (float *)(data + 0x23C), k, k, frame_time * 6.2831855f);
        aim = FUN_L14_002b35d8_c(moby, 0, *(float *)(moby + 0x48));
        FUN_L14_002b37b0_c(moby, ang);
        if (*(int *)(data + 0x114) == 2) {
            if (*(int *)(data + 0x240) == -1) break;
            ((unsigned char *)moby)[0x20] = 8;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
            }
            {
                int index = *(int *)(data + 0x24C) - 1;
                char *path;
                *(int *)(data + 0x24C) = index;
                path = (char *)D_L14_001B0BB0[*(int *)(data + 0x240)];
                *(float *)(data + 0x250) = *(float *)(path + (index << 4) + 0x1C);
            }
            *(float *)(data + 0x27C) = -1.0f;
            *(float *)(data + 0x254) = *(float *)(data + 0x258) = *(float *)(data + 0x25C) = 0.0f;
            FUN_L00_002e2040(moby);
            break;
        }
        if (fast_dec_timer_s16(data + 0x260) == 0) break;
        if (FUN_L14_002b3920_c(((unsigned char *)moby)[0x21]) >= 2) break;
        if (FUN_L00_002e1f28(moby, 1, 0) == 0) break;
        if (random_integer_below(4) != 0) break;
        if (!(fast_difference_between_rotations(*(float *)(moby + 0x48), ang) < 0.017453292f)) break;
        if (!(aim < 0.017453292f)) break;
        if (FUN_L00_00261968(*(int *)(data + 0x244), moby + 0x10, owner + 0x10) != 0) break;
        FUN_L14_002b3890_c(((unsigned char *)moby)[0x21]);
        *(float *)(data + 0x278) = ang;
        ((unsigned char *)moby)[0x20] = 5;
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
        *(short *)(data + 0x262) = scale_game_frames(D_L14_001614EC_gp);
        break;
    }
    case PLASMABOT_WAIT_ANIMATION_BEFORE_AIM_SWEEP: {
        float ang;
        ang = fast_arc_tan_c(*(float *)(owner + 0x10) - *(float *)(moby + 0x10),
                                *(float *)(owner + 0x14) - *(float *)(moby + 0x14));
        FUN_L14_002b35d8_c(moby, 0, fast_subtract_rotations_c2(ang, D_L14_001614E8_gp));
        FUN_L14_002b37b0_c(moby, ang);
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 6;
        }
        break;
    }
    case PLASMABOT_SWEEP_AIM_AND_TRACE_LINE: {
        int flag;
        float ang;
        float progress;
        float sweep;
        float angle;
        flag = 0;
        ang = fast_arc_tan_c(*(float *)(owner + 0x10) - *(float *)(moby + 0x10),
                                *(float *)(owner + 0x14) - *(float *)(moby + 0x14));
        progress = convert_integer_to_float(*(short *)(data + 0x262));
        progress = progress / convert_integer_to_float(scale_game_frames(D_L14_001614EC_gp));
        sweep = D_L14_001614E8_gp;
        angle = fast_subtract_rotations_c2(*(float *)(data + 0x278), -sweep + (sweep + sweep) * progress);
        if (((unsigned char *)data)[0x24B] != 0) {
            if (*(float *)(owner + 0x18) + 0.1f - *(float *)(moby + 0x18) < 0.0f) {
                flag = 1;
            }
        }
        FUN_L14_002b35d8_c(moby, flag, angle);
        FUN_L14_002b37b0_c(moby, ang);
        build_moby_bone_transform_c(moby, 0, mat);
        qcopy(direction, mat[1]);
        if (flag == 0) {
            direction[2] = 0.0f;
        }
        fast_vec_normalize(direction, direction, D_L14_001614E4_gp);
        add_vector_xyz_c3(end, mat[3], direction);
        fast_vec_normalize(direction, direction, D_L14_001614F4_gp);
        add_vector_xyz_c3(mat[3], mat[3], direction);
        info.moby = moby;
        info.flags = 0x10001;
        info.count = 1;
        info.strength = 1.0f;
        info.x = fast_cos(*(float *)(moby + 0x48));
        info.y = fast_sin(*(float *)(moby + 0x48));
        info.scale = 1.0f;
        info.distance = 5627.9248f;
        info.enabled = 1;
        info.oclass = *(unsigned short *)(moby + 0xA6);
        info.mode = 0;
        if (coll_line_fix(mat[3], end, 0, moby, &info) != 0) {
            qcopy(end, D_L14_00174560);
        }
        FUN_L00_002e2190(moby, mat, end);
        if (fast_dec_timer_s16(data + 0x262) != 0) {
            blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 7;
            FUN_L00_002e2040(moby);
        }
        break;
    }
    case PLASMABOT_WAIT_ANIMATION_BEFORE_AIM:
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 4;
        }
        break;
    case PLASMABOT_WAIT_ANIMATION_BEFORE_RESUMING_AIM: {
        long index = ((unsigned char *)moby)[0x52];
        if ((moby[0x70] & 2) && (unsigned char)index == ((unsigned char *)moby)[0x53]) {
            if ((unsigned char)index != 7) goto check_next;
            goto idle;
        }
        if ((unsigned char)index == 7) break;
    check_next:
        if (((unsigned char *)moby)[0x53] == 7) break;
    idle:
        blend_moby_animation(moby, 0, 0, 10);
        ((unsigned char *)moby)[0x20] = 4;
        break;
    }
    case PLASMABOT_FALL_THEN_EXPLODE: {
        if ((mine_fall(moby, data + 0x60) & 0x160) == 0) return;
        FUN_L00_002e2040(moby);
        FUN_L00_0024f7c8(moby, 2, mat);
        FUN_L14_002dea98_c(moby, (char *)mat);
        call_F4A8(moby, data + 0x40, moby + 0x10, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
        if (((unsigned char *)D_001413F4)[0] == 2) {
            FUN_L00_00257470(moby, 0x200, -1);
            { call_D678(moby); return; }
        }
        FUN_L00_00257470(moby, 0, -1);
        { call_D678(moby); return; }
    }
    case PLASMABOT_SPAWN_DEBRIS_AND_REMOVE: {
        FUN_L00_0024f7c8(moby, 2, mat);
        FUN_L14_002dea98_c(moby, (char *)mat);
        status = 0;
        ((void (*)(char *, int *, int))FUN_L14_002e0170_c)(moby, &status, 1);
        FUN_L00_002e2040(moby);
        call_D678(moby);
        return;
    }
    }
    state = ((unsigned char *)moby)[0x20];
    if (!(state >= 4 && state <= 7) && state != 1) {
        FUN_L14_002b35d8_c(moby, 0, *(float *)(moby + 0x48));
        FUN_L14_002b37b0_c(moby, *(float *)(moby + 0x48));
    }
    FUN_L14_002b2600_c((struct Moby *)moby);
}
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

typedef struct {
    u8 pad0[0x1E0];
    Vec4 exhaust;    /* 0x1E0 */
    Vec4 nozzle;     /* 0x1F0 */
    Vec4 joint6;     /* 0x200 */
    Vec4 beam_start; /* 0x210 */
    Vec4 beam_end;   /* 0x220 */
} ShipVars;

typedef struct {
    u8 pad0[0x10];
    f32 scale;       /* 0x10 */
    u8 pad14[2];
    s16 mode;        /* 0x16 */
    s32 color;       /* 0x18 */
} ParticleFade;

typedef struct {
    u8 pad0[0xA];
    s16 frames;      /* 0x0A */
    f32 life;        /* 0x0C */
    u8 pad10[0x10];
    ParticleFade fade; /* 0x20 */
} Particle;

extern Vec4 D_L14_001674C0 __attribute__((section(".data")));
extern u8 D_L14_0015F580[] __attribute__((section(".sdata")));
extern f32 D_L14_001614A8 __attribute__((sda));
extern f32 D_L14_001614B0 __attribute__((sda));
extern f32 D_L14_001614B4 __attribute__((sda));
extern f32 D_L14_001614B8 __attribute__((sda));

extern void FUN_L14_002b3a20_c(struct Moby *) __asm__("FUN_L14_002b3a20");
extern void FUN_L14_002b2928_u(struct Moby *) __asm__("FUN_L14_002b2928");
extern void enqueue_callback_list_1(void (*)(struct Moby *), struct Moby *) __asm__("FUN_001f4600");
extern void moby_point_position(struct Moby *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void build_moby_bone_transform(struct Moby *, s32, void *) __asm__("FUN_0020cca8");
extern void FUN_00213358(void *, f32, f32);
extern Particle *FUN_L00_00274948(void *, void *, s32, struct Moby *);
extern f32 FUN_002132a8(f32, f32);
extern s32 FUN_00213260(s32);
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");

/* Ship engine effects: puffs exhaust particles from joint 1 and places the nozzle and beam points from joints 7 and 2. */
void FUN_L14_002b2600(struct Moby *moby)
{
    Vec4 pos;
    Vec4 dir;
    Vec4 vel;
    Vec4 mat[4];
    Vec4 dx;
    Vec4 dy;
    Vec4 dz;
    ShipVars *pv;
    Particle *particle;
    ParticleFade *fade;
    s32 i;
    f32 inverse_lifetime_frames;

    pv = (ShipVars *)moby->pvars;
    enqueue_callback_list_1(FUN_L14_002b3a20_c, moby);
    moby_point_position(moby, 1, &pos);
    fast_vec_sub(&dir, &pos, &D_L14_001674C0);
    fast_vec_normalize(&dir, &dir, -0.5f);
    fast_vec_add(&pv->exhaust, &pos, &dir);
    FUN_00213358(&vel, 0.005f, 0.03f);
    particle = FUN_L00_00274948(&pos, &vel, 0x7F, moby);
    if (particle != NULL) {
        particle->life = FUN_002132a8(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        particle = FUN_L00_00274948(&pos, D_L14_0015F580, 0x7F, moby);
        if (particle == NULL) {
            continue;
        }
        fade = &particle->fade;
        if (i == 2 && FUN_00213260(8) == 0) {
            particle->life = 180000.0f;
        } else {
            particle->life = FUN_002132a8(80000.0f, 120000.0f);
        }
        particle->frames = scale_game_frames(2);
        inverse_lifetime_frames = 1.0f / convert_integer_to_float(particle->frames);
        fade->mode = 3;
        fade->color = 0x7F7F7F;
        fade->scale = inverse_lifetime_frames;
    }
    moby_point_position(moby, 6, &pv->joint6);
    build_moby_bone_transform(moby, 7, mat);
    fast_vec_scale(&dx, &mat[0], D_L14_001614A8);
    fast_vec_add(&pv->nozzle, &mat[3], &dx);
    build_moby_bone_transform(moby, 2, mat);
    fast_vec_scale(&dx, &mat[0], D_L14_001614B0);
    fast_vec_scale(&dy, &mat[1], D_L14_001614B4);
    fast_vec_scale(&dz, &mat[2], D_L14_001614B8);
    fast_vec_add(&pv->beam_start, &mat[3], &dx);
    fast_vec_add(&pv->beam_start, &pv->beam_start, &dy);
    fast_vec_add(&pv->beam_start, &pv->beam_start, &dz);
    fast_vec_scale(&dy, &dy, -2.0f);
    fast_vec_add(&pv->beam_end, &pv->beam_start, &dy);
    enqueue_callback_list_1(FUN_L14_002b2928_u, moby);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B3B78), where it is exact; names translated to the US level program. */

typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));

typedef union {
    VecQuad q;
    f32 f[4];
    s32 i[4];
} Vec4_2b2928;

extern Vec4_2b2928 D_L14_001D8850_lui[] __asm__("D_L14_001D8850") __attribute__((section(".data")));
extern char D_0013F5E0[] __attribute__((section(".data")));
extern Vec4 D_L14_001674C0 __attribute__((section(".data")));
extern float D_L14_001614AC __attribute__((sda)); /* no foreign declaration */
extern float D_L14_001614BC __attribute__((sda)); /* no foreign declaration */
extern float D_L14_001614C0 __attribute__((sda)); /* no foreign declaration */
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void add_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void fast_draw_quad_real(void *, void *, s32);
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");
extern void subtract_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern float D_L14_001614AC_lui __asm__("D_L14_001614AC");
extern float D_L14_001614BC_lui __asm__("D_L14_001614BC");
extern float D_L14_001614C0_lui __asm__("D_L14_001614C0");

void FUN_L14_002b2928(char *moby) {
    char *data = *(char **)(moby + 0x78);
    struct {
        Vec4_2b2928 o[4];
        unsigned int c[4];
        float g[8];
        u64 t[4];
    } pk;
    Vec4_2b2928 quad_transform[4];
    Vec4_2b2928 w;
    char *pp;
    char *nozzle_position;
    char *r;
    int i;
    int j;
    int alpha;

    qcopy(&quad_transform[3], data + 0x1E0);
    quad_transform[3].f[3] = 1.0f;
    subtract_vector_xyz_c3(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    cross_vectors_xyz(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    cross_vectors_xyz(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    pk.t[1] = get_effect_texture(11);
    pk.t[3] = 0x8000000048ULL;
    pk.t[2] = 0xFF9000000260ULL;
    pk.t[0] = 5;
    pk.c[3] = 0x80FFFFFF;
    pk.c[2] = 0x80FFFFFF;
    pk.c[1] = 0x80FFFFFF;
    pk.c[0] = 0x80FFFFFF;
    pk.g[0] = 0.0f;
    pk.g[1] = 0.0f;
    pk.g[2] = 0.0f;
    pk.g[3] = 1.0f;
    pk.g[4] = 1.0f;
    pk.g[5] = 0.0f;
    pk.g[6] = 1.0f;
    pk.g[7] = 1.0f;
    for (i = 0; i < 4; i++) {
        fast_vec_scale_c(&pk.o[i], &D_L14_001D8850_lui[i], 0.4f);
        transform_vector(&pk.o[i], &pk.o[i], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], data + 0x200);
    pp = data + 0x220;
    subtract_vector_xyz_c3(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    r = data + 0x210;
    nozzle_position = data + 0x1F0;
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    cross_vectors_xyz(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    cross_vectors_xyz(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    pk.c[3] = 0x202020FF;
    pk.c[2] = 0x202020FF;
    pk.c[1] = 0x202020FF;
    pk.c[0] = 0x202020FF;
    for (j = 0; j < 4; j++) {
        fast_vec_scale_c(&pk.o[j], &D_L14_001D8850_lui[j], 0.2f);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], nozzle_position);
    subtract_vector_xyz_c3(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    cross_vectors_xyz(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    cross_vectors_xyz(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    fast_vec_scale_c(&w, &quad_transform[0], D_L14_001614AC_lui);
    add_vector_xyz_c2(&quad_transform[3], &quad_transform[3], &w);
    pk.c[3] = 0x4020FF20;
    pk.c[2] = 0x4020FF20;
    pk.c[1] = 0x4020FF20;
    pk.c[0] = 0x4020FF20;
    alpha = 0x40;
    for (j = 0; j < 4; j++) {
        fast_vec_scale_c(&pk.o[j], &D_L14_001D8850_lui[j], 0.2f);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], r);
    subtract_vector_xyz_c3(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    cross_vectors_xyz(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    cross_vectors_xyz(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    fast_vec_scale_c(&w, &quad_transform[0], D_L14_001614BC_lui);
    add_vector_xyz_c2(&quad_transform[3], &quad_transform[3], &w);
    pk.c[3] = (alpha << 24) | 0xFF2020;
    pk.c[2] = (alpha << 24) | 0xFF2020;
    pk.c[1] = (alpha << 24) | 0xFF2020;
    pk.c[0] = (alpha << 24) | 0xFF2020;
    for (j = 0; j < 4; j++) {
        fast_vec_scale_c(&pk.o[j], &D_L14_001D8850_lui[j], D_L14_001614C0_lui);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], pp);
    subtract_vector_xyz_c3(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    cross_vectors_xyz(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    cross_vectors_xyz(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    fast_vec_scale_c(&w, &quad_transform[0], D_L14_001614BC_lui);
    add_vector_xyz_c2(&quad_transform[3], &quad_transform[3], &w);
    pk.c[3] = (alpha << 24) | 0xFF2020;
    pk.c[2] = (alpha << 24) | 0xFF2020;
    pk.c[1] = (alpha << 24) | 0xFF2020;
    pk.c[0] = (alpha << 24) | 0xFF2020;
    for (j = 0; j < 4; j++) {
        fast_vec_scale_c(&pk.o[j], &D_L14_001D8850_lui[j], D_L14_001614C0_lui);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);
}
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"

/* pvars of the moby FUN_L14_002b2ed8 updates */
typedef struct {
    u8 pad00[0x38];
    s32 restart;   /* 0x38: nonzero picks a new random delay */
    u8 pad3C[0x1F8];
    f32 unk234;    /* 0x234 */
    s32 delay;     /* 0x238: frames, counted down */
} DelayVars_2b2ed8;

extern f32 random_float_between_2b2ed8(f32, f32) __asm__("FUN_002132a8");
extern f32 FUN_001f96b0_2b2ed8(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32_2b2ed8(f32) __asm__("FUN_001fa6d0");
extern s32 tick_countdown_2b2ed8(s32 *) __asm__("FUN_001f9740");

extern f32 D_L14_001614C4 __attribute__((sda));
void FUN_L14_002b2ed8(struct Moby *moby) {
    DelayVars_2b2ed8 *pv = (DelayVars_2b2ed8 *)moby->pvars;

    if (pv->restart != 0) {
        pv->delay = truncate_float_to_s32_2b2ed8(
            FUN_001f96b0_2b2ed8(random_float_between_2b2ed8(180.0f, 240.0f)));
        pv->restart = 0;
    }
    tick_countdown_2b2ed8(&pv->delay);
    if (pv->delay != 0) {
        pv->unk234 = D_L14_001614C4 + 6.0f;
    } else {
        pv->unk234 = D_L14_001614C4;
    }
}



/* Enemy hit reaction: takes damage from attacks (not from its own kind), staggers or is knocked away and dies. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B41B0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char D_0013E533[];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L00_002e2040(char *);
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern float D_L14_001614C8 __attribute__((sda));
extern void FUN_L00_00257470(void *, int, int);
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern char D_0013E533[];
extern void func_L00_0025D5B0_i(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");

void FUN_L14_002b2f60(char *moby) {
    enum {
        HIT_REACTION_UNAVAILABLE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DEATH = 2,
        HIT_REACTION_STAGGER = 3,
        HIT_REACTION_NONE = 11
    };
    char *state = *(char **)(moby + 0x78);
    int hit_kind;
    float damage;
    char *hit_info;
    int result;
    if (((unsigned char *)moby)[0x20] == 0 || ((unsigned char *)moby)[0x20] == 0xB) {
        if (((unsigned char *)moby)[0x20] == 0xB) {
            ((unsigned char *)moby)[0xA4] = 0xFF;
            trooper_anim_finish(moby, state + 0xC0);
        }
        return;
    }
    damage = 0.0f;
    hit_info = find_moby_hit(moby, 0x330000, 0);
    if (hit_info != 0 && *(char **)(hit_info + 0x20) != 0
        && (*(short *)(*(char **)(hit_info + 0x20) + 0xA6) == *(short *)(moby + 0xA6) || *(short *)(*(char **)(hit_info + 0x20) + 0xA6) == 0x370)) {
        hit_info = 0;
    }
    result = hit_reaction(moby, hit_info, state + 0x20, 0, &hit_kind, &damage, 0, 4);
    if (hit_kind != 1 && ((unsigned char *)moby)[0x20] != 0xB) {
        *(float *)(state + 0x20) -= damage;
        if (*(float *)(state + 0x20) <= 0.0f) result = 1;
        switch (result) {
        case HIT_REACTION_STAGGER: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
        {
            char *p60 = state + 0x60;
            int t;
            float e, g, k;
            FUN_L00_002e2040(moby);
            moby[0x20] = 0xA;
            t = scale_game_frames(0x3C);
            e = frame_time;
            g = D_L14_001614C8 * e;
            k = e * 8.0f;
            *(short *)(state + 0x26) = t;
            *(float *)(state + 0x78) = k;
            *(float *)(state + 0x7C) = g;
            *(float *)(state + 0xB0) = 11.0f;
            *(float *)(state + 0xB4) = 29.0f;
            func_L00_0025D5B0_i(moby, p60, fast_arc_tan(*(float *)(hit_info + 0x10), *(float *)(hit_info + 0x14)), 7, 1, 0);
        }
            ((unsigned char *)state)[0xC7] = 0xFA;
            break;
        case HIT_REACTION_LETHAL: case HIT_REACTION_DEATH: {
            float y;
            if (*(int *)(hit_info + 0x24) & 0x800000) {
                moby[0x20] = 0xC;
            } else {
                moby[0x20] = 0xB;
            }
            *(unsigned short *)(moby + 0x34) &= 0xEFFF;
            {
                int r80 = truncate_float_to_s32(*(float *)(state + 0x88) * 1024.0f);
                float k = frame_time * 10.0f;
                *(float *)(state + 0x88) = 0.3f;
                *(float *)(state + 0x78) = k;
                *(float *)(state + 0x7C) = k;
                *(float *)(state + 0xB0) = 9.0f;
                *(int *)(state + 0x80) = r80;
                *(float *)(state + 0xB4) = 15.0f;
            }
            y = fast_arc_tan(*(float *)(hit_info + 0x10), *(float *)(hit_info + 0x14));
            func_L00_0025D5B0_i(moby, state + 0x60, y, 8, scale_game_frames(5), 0);
            ((unsigned char *)state)[0xC7] = 0xFA;
            if (((unsigned char *)D_0013E533)[0x2EC1] == 2) {
                FUN_L00_00257470(moby, 0x200, -1);
            } else {
                FUN_L00_00257470(moby, 0, -1);
            }
            *(int *)(moby + 0x94) = 0;
            break;
        }
        case HIT_REACTION_UNAVAILABLE: case HIT_REACTION_NONE:
            break;
        }
        trooper_anim_update(moby, (short *)(state + 0xC0));
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
    trooper_anim_finish(moby, state + 0xC0);
}
/* steers the moby toward its target: queries the path, eases x/y, then turns to face the heading */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4668), where it is exact; names translated to the US level program. */

extern float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern int sample_path(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_L00_0025CE58_2B4668(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

extern float D_L14_001614D8 __attribute__((sda));
int FUN_L14_002b3418(char *moby) {
    float path_target_position[4];
    float b[4];
    float target_offset[4];
    char *data = *(char **)(moby + 0x78);
    int reached = sample_path(D_L14_001B0BB0[*(int *)(data + 0x240)], path_target_position, data + 0x24C, data + 0x250, 0,
                              D_L14_001614D8 * frame_time * *(float *)(data + 0x27C));
    float ang;
    float k;
    float m;
    qcopy(b, moby + 0x10);
    vendor_spring((float *)(moby + 0x10), (float *)(data + 0x254), path_target_position[0], 0.01f, 0.2f, 0.0f);
    vendor_spring((float *)(moby + 0x14), (float *)(data + 0x258), path_target_position[1], 0.01f, 0.2f, 0.0f);
    subtract_vector_xyz_c(target_offset, path_target_position, b);
    ang = fast_arc_tan(target_offset[0], target_offset[1]);
    k = frame_time_sq * 12.566371f;
    m = frame_time * 6.2831855f;
    func_L00_0025CE58_2B4668((float *)(moby + 0x48), ang, (float *)(data + 0x23C), k, k, m);
    return reached != 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4828), where it is exact; names translated to the US level program. */

extern f32 fast_subtract_rotations_c(f32, f32) __asm__("func_001FA5C8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern void multiply_quaternions(void *, void *, void *) __asm__("FUN_001fa3c0");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
extern float func_L00_0025CE58_2B4828(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

/* Eases the moby's yaw offset toward target_angle (clamped to about 70 degrees); with `flag` also eases the pitch toward its owner.
   Returns the remaining difference between the target and the current yaw offset. */
float FUN_L14_002b35d8(char *moby, int flag, float target_angle) {
    char *state = *(char **)(moby + 0x78);
    char *owner = *(char **)(state + 0x110);
    float turn = fast_subtract_rotations_c(target_angle, *(float *)(moby + 0x48));
    if (turn > 1.2217304706573486f) turn = 1.2217304706573486f;
    else if (turn < -1.2217304706573486f) turn = -1.2217304706573486f;
    {
        float w = frame_time_sq * 12.566370964050293f;
        func_L00_0025CE58_2B4828((float *)(state + 0x268), turn, (float *)(state + 0x26C), w, w, frame_time * 6.2831854820251465f);
    }
    FUN_L00_001fff28(state + 0x170, 2, *(float *)(state + 0x268));
    if (flag) {
        float owner_aim_position[4];
        float b[4];
        float pitch_quaternion[4];
        float *bp;
        float *cp;
        struct Hero *g;
        float target_pitch_angle;
        qcopy(owner_aim_position, owner + 0x10);
        g = &hero;
        if (((char *)g->moby) == owner) owner_aim_position[2] = owner_aim_position[2] - g->ground_distance;
        bp = b;
        subtract_vector_xyz_c2(bp, owner_aim_position, moby + 0x10);
        b[2] = b[2] + 1.5f;
        target_pitch_angle = -fast_arc_tan(vector_length_xy(bp), b[2]);
        func_L00_0025CE58_2B4828((float *)(state + 0x280), target_pitch_angle, (float *)(state + 0x284), frame_time_sq * 12.566370964050293f, frame_time_sq * 12.566370964050293f, frame_time * 6.2831854820251465f);
        cp = pitch_quaternion;
        FUN_L00_001fff28(cp, 1, *(float *)(state + 0x280));
        multiply_quaternions(state + 0x170, state + 0x170, cp);
    }
    return fast_difference_between_rotations(turn, *(float *)(state + 0x268));
}
extern short *D_L14_001ABF40_3920[] __asm__("D_L14_001ABF40");
extern char *D_L14_0015FFD8_3920 __asm__("D_L14_0015FFD8");

int FUN_L14_002b3920(int moby_list_index) {
    int count = 0;
    short *moby_list_entry;

    if (moby_list_index == -1)
        return 0;
    moby_list_entry = D_L14_001ABF40_3920[moby_list_index];
    if (moby_list_entry == 0)
        return 0;
    do {
        unsigned short v = *(unsigned short *)moby_list_entry;
        unsigned char *other = (unsigned char *)(D_L14_0015FFD8_3920 + ((v & 0x7FFF) << 8));
        if (*(short *)(other + 0xA6) == 0x1C) {
            if ((unsigned char)(other[0x20] - 5) <= 1)
                count++;
        }
    } while (*moby_list_entry++ >= 0);
    return count;
}
/* Returns 0 if any listed moby of class 0x1C is in state 2, else 1. */

extern char *D_L14_0015FFD8;
extern short *D_L14_001ABF40[];

int FUN_L14_002b39a0(unsigned char *source_moby) {
    unsigned short *moby_list_entry;
    if (*(volatile unsigned char *)(source_moby + 0x21) == 0xFF || (moby_list_entry = (unsigned short *)D_L14_001ABF40[source_moby[0x21]]) == 0)
        return 1;
    do {
        char *moby = D_L14_0015FFD8 + ((*moby_list_entry & 0x7FFF) << 8);
        if (*(short *)(moby + 0xA6) == 0x1C && *(unsigned char *)(moby + 0x20) == 2)
            return 0;
    } while ((short)*moby_list_entry++ >= 0);
    return 1;
}
/* Level overlay code after unclassified; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */
#include "qcopy.h"
/* Draws the moby's glow quad, pulled toward the camera so it is not hidden by the moby. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF4A0), where it is exact; names translated to the US level program. */

typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_2af4a0;

extern Vec4 D_L14_001674C0 __attribute__((section(".data")));
extern float D_L14_001D8890[4][4];
extern float D_L14_001614F8 __attribute__((sda));
extern float D_L14_001614FC __attribute__((sda));
extern float D_L14_00161500 __attribute__((sda));
extern u32 D_L14_00161504 __attribute__((sda));
extern float vector_length_xyz(void *a);
extern void add_vector_xyz(void *, void *, void *);
extern void draw_geometry_quad(void *, void *, int);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
unsigned long get_effect_texture_alt(s32) __asm__("FUN_001f44b8");

void FUN_L14_002b3a20(char *m) {
    float pos[4];
    Pk_2af4a0 pk;
    float v[4];
    float w[4];
    float len;
    u32 col;
    int j;
    qcopy(pos, m + 0x10);
    pos[2] += 0.01f;
    pos[3] = 1.0f;
    pk.b = get_effect_texture_alt(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    subtract_vector_xyz(v, ((char *)&D_L14_001674C0), pos);
    len = vector_length_xyz(v);
    if (0.0f < len) {
        float position_offset_distance = D_L14_001614FC;
        float unclamped_offset_distance = len - D_L14_00161500;
        if (unclamped_offset_distance < position_offset_distance) {
            position_offset_distance = unclamped_offset_distance;
            if (unclamped_offset_distance < 0.0f)
                position_offset_distance = 0.0f;
        }
        normalize_vector_xyz(w, v, position_offset_distance);
        add_vector_xyz(pos, pos, w);
    }
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    col = D_L14_00161504;
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (j = 0; j < 4; j++) {
        scale_vector_xyz(pk.m[j], D_L14_001D8890[j], D_L14_001614F8);
        add_vector_xyz(pk.m[j], pk.m[j], pos);
    }
    draw_geometry_quad(&pk, 0, 0);
}
#define NOT_SDA __attribute__((section(".data")))

extern u8 *D_L14_00167500 NOT_SDA;
extern s32 FUN_001f96f8_c(s32) __asm__("FUN_001f96f8");
extern s32 fast_dec_timer(void *) __asm__("FUN_001f9740");

void plasmabot_hatch_update(struct Moby *moby) __asm__("FUN_L14_002d96e0");
void plasmabot_hatch_update(struct Moby *moby) {
    enum {
        HATCH_WAIT_ACTIVATION = 0,
        HATCH_WAIT_ACTIVATION_DELAY = 1,
        HATCH_WAIT_ACTIVATION_ANIMATION = 2,
        HATCH_WAIT_ACTIVATION_GATE_CLEAR = 3,
        HATCH_IDLE_AFTER_ACTIVATION_GATE_CLEAR = 4,
        HATCH_WAIT_ANIMATION_BEFORE_RESET_DELAY = 5,
        HATCH_WAIT_RESET_DELAY = 6,
        HATCH_WAIT_RESET_ANIMATION = 7
    };
    char *data = (char *)moby->pvars;
    if (hero.vehicle_mode == 2) {
        moby->collision = 0;
    } else {
        moby->collision = moby->pclass->collision;
    }
    switch (moby->state) {
    case HATCH_WAIT_ACTIVATION:
        if (*(s32 *)data != 0 && *(s16 *)(D_L14_00167500 + 0x86) == 0x14) {
            moby->state = 1;
            *(s32 *)(data + 8) = FUN_001f96f8_c(*(s32 *)(data + 4));
        }
        break;
    case HATCH_WAIT_ACTIVATION_DELAY:
        if (fast_dec_timer(data + 8) != 0) {
            moby->state = 2;
            set_moby_animation(moby, 1, 0);
        }
        break;
    case HATCH_WAIT_ACTIVATION_ANIMATION:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 3;
            set_moby_animation(moby, 2, 0);
        }
        break;
    case HATCH_WAIT_ACTIVATION_GATE_CLEAR:
        if (*(s16 *)(D_L14_00167500 + 0x86) != 0x14) {
            set_moby_animation(moby, 0, 0);
            moby->state = 4;
        }
        break;
    case HATCH_IDLE_AFTER_ACTIVATION_GATE_CLEAR:
        break;
    case HATCH_WAIT_ANIMATION_BEFORE_RESET_DELAY:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 6;
            set_moby_animation(moby, 2, 0);
        }
        break;
    case HATCH_WAIT_RESET_DELAY:
        if (fast_dec_timer(data + 8) != 0) {
            set_moby_animation(moby, 3, 0);
            moby->state = 7;
        }
        break;
    case HATCH_WAIT_RESET_ANIMATION:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 0;
            set_moby_animation(moby, 0, 0);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002DAC00), where it is exact; names translated to the US level program. */

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

extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");

int FUN_L14_002d9870(int i) {
    unsigned char *moby = (unsigned char *)(D_L14_ffd0_e_macro[2] + i * 256);
    int *data = *(int **)(moby + 0x78);
    int r = 0;
    if (moby[0x20] == 0) {
        moby[0x20] = 5;
        set_moby_animation(moby, 1, 0);
    } else {
        r = moby[0x20] == 6;
    }
    data[2] = scale_game_frames(0x3C);
    return r;
}
extern float D_0015ED60;
#define DE670_THRESHOLD D_L14_00161B20
#define DE670_DAMPING D_L14_00161B1C
#define DE670_GRAVITY D_L14_00161B18
extern void *D_001413D0_de670 __asm__("D_001413D0") __attribute__((section(".data")));
extern void FUN_L14_002dec40(struct Moby *);
extern void enqueue_callback_list_1(void (*)(struct Moby *), struct Moby *) __asm__("FUN_001f4600");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 absolute_float(f32) __asm__("FUN_001f99c0");
extern f32 fast_add_rotations(f32, f32);
extern s32 fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 random_integer_below_de670(s32) __asm__("FUN_00213260");
extern s32 FUN_L00_00257b90(s32, s32);
extern f32 random_float_between_de670(f32, f32) __asm__("FUN_002132a8");
extern s32 coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void spawn_explosion(void *, void *, void *, f32, f32, s32, s32, s32,
                              f32, f32, f32, f32, s32, f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern void FUN_L00_0026cbb0_de670(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern u8 *FUN_L00_0026d000_de670(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
extern void mark_moby_for_removal(struct Moby *) __asm__("FUN_0020c828");

extern f32 D_L14_00161B18 __attribute__((sda));
extern f32 D_L14_00161B1C __attribute__((sda));
extern f32 D_L14_00161B20 __attribute__((sda));
void FUN_L14_002de670(struct Moby *moby) {
    f32 *velocity = (f32 *)moby->pvars;
    s32 i;
    s32 signed_particle_variation;
    s32 alpha;
    s32 color;
    f32 size;
    f32 absolute_velocity;
    u8 *particle;
    u8 *fade;

    if (random_integer_below_de670(2) != 0) {
        enqueue_callback_list_1(FUN_L14_002dec40, moby);
    }
    fast_vec_add(&moby->pos, &moby->pos, velocity);
    absolute_velocity = absolute_float(velocity[0]);
    if (DE670_THRESHOLD * D_0015ED60 < absolute_velocity) {
        velocity[0] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    absolute_velocity = absolute_float(velocity[1]);
    if (DE670_THRESHOLD * D_0015ED60 < absolute_velocity) {
        velocity[1] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    if (velocity[2] > 0.0f) {
        velocity[2] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    velocity[2] -= DE670_GRAVITY * frame_scale_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, velocity[4]);
    moby->rot.y = fast_add_rotations(moby->rot.y, velocity[5]);
    moby->rot.z = fast_add_rotations(moby->rot.z, velocity[6]);

    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x ||
        moby->pos.y < 2.0f || 1021.0f < moby->pos.y ||
        moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer(velocity + 8) != 0 || coll_sphere(0.5f, &moby->pos, 0, *(void **)(velocity + 9)) != 0) {
        spawn_explosion(moby, velocity, &moby->pos, 0.0f, 0.0f, 5, 2, 4,
                            1.0f, 0.5f, 9.0f, 1.0f, 0, 15.0f, 1, 5, -1, 0);
        coll_sphere(1.0f, &moby->pos, 0, D_001413D0_de670);
        mark_moby_for_removal(moby);
        return;
    }
    FUN_L00_0026cbb0_de670(&moby->pos, D_L14_0015F580, 0x4F007FFF, 0x1FFFFFFF,
                         FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)), 1, 60000.0f);
    for (i = 0; i < 2; i++) {
        signed_particle_variation = random_integer_below_de670(6);
        if (random_integer_below_de670(2) != 0) signed_particle_variation = -signed_particle_variation;
        size = random_float_between_de670(40000.0f, 100000.0f);
        color = FUN_L00_00257b90(0x30, 0xFF);
        alpha = 0x60000000;
        particle = FUN_L00_0026d000_de670(&moby->pos, 0.025f, 1.0f, 1.0f, signed_particle_variation,
                                       D_L14_0015F580, size,
                                       color | ((color << 16) | ((color << 8) | alpha)));
        if (particle != 0) {
            particle[3] = 0x44;
            fade = particle + 0x20;
            *(s16 *)(particle + 0xA) = scale_game_frames(0x3C);
            *(s32 *)(fade + 4) = 2;
            fade[0xA] = 0x60;
            fade[0xB] = particle[0xA];
        }
    }
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002DFE98), where it is exact; names translated to the US level program. */

extern float FUN_L00_00257c48(float lo, float hi);
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void build_look_at_matrix(void *dst, void *vec, void *axis,
                                 float angle) __asm__("FUN_00214890");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");

extern float D_L14_00161B08 __attribute__((sda));
extern float D_L14_00161B0C __attribute__((sda));
extern float D_L14_00161B10 __attribute__((sda));
extern float D_L14_00161B14 __attribute__((sda));
unsigned char *FUN_L14_002dea98(char *owner, char *pos) {
    unsigned char *spawned_moby = func_0020D348_m(0x145);
    char *spawned_vars;
    char *g;
    float vec[8];
    if (spawned_moby != 0) {
        spawned_vars = *(char **)(spawned_moby + 0x78);
        *(char **)(spawned_vars + 0x24) = owner;
        qcopy(spawned_moby + 0x10, pos);
        *(float *)(spawned_moby + 0x48) = *(float *)(owner + 0x48);
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        g = (char *)&hero.motion.pos;
        subtract_vector_xyz(vec, spawned_moby + 0x10, g);
        g = g + 0x210;
        vec[2] = 0.0f;
        normalize_vector_xyz(vec, vec,
                             random_float_between_alt(D_L14_00161B08 * frame_scale,
                                                      D_L14_00161B0C * frame_scale));
        cross_vectors_xyz(vec + 4, g, vec);
        build_look_at_matrix(spawned_vars, vec, g, FUN_L00_00257c48(0.34906584f, 1.0471976f));
        *(float *)(spawned_vars + 8) = random_float_between_alt(D_L14_00161B10 * frame_scale,
                                                     D_L14_00161B14 * frame_scale);
        *(float *)(spawned_vars + 0x10) = FUN_L00_00257c48(0.0f, 0.008726646f);
        *(float *)(spawned_vars + 0x14) = FUN_L00_00257c48(0.05235988f, 0.13962634f);
        *(float *)(spawned_vars + 0x18) = FUN_L00_00257c48(0.0f, DEG_TO_RAD);
        *(int *)(spawned_vars + 0x20) = scale_game_frames(200);
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
extern void FUN_L14_002e03b8(struct Moby *);
extern s32 D_001413D0_dfd10 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 tick_countdown_dfd10(s32 *) __asm__("FUN_001f9740");
extern f32 fabsf_dfd10(f32) __asm__("FUN_001f99c0");
extern void add_vector_xyz_dfd10(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_dfd10(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz_dfd10(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 fast_add_rotations_dfd10(f32, f32) __asm__("FUN_001fa580");
extern s32 coll_sphere_dfd10(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void spawn_explosion_dfd10(struct Moby *, void *, void *, f32, f32, s32, s32, s32,
    f32, f32, f32, f32, s32, f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern void mark_moby_for_removal_dfd10(struct Moby *) __asm__("FUN_0020c828");
extern int random_integer_below_dfd10(int) __asm__("FUN_00213260");
extern int FUN_L00_0028dc90_dfd10(int, int, struct Moby *, int) __asm__("FUN_L00_0028dc90");
extern int follow_path_dfd10(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

typedef struct {
    u8 pad0[0x10];
    void *points;
    u8 pad14[0xC];
} RacePath_dfd10;
extern RacePath_dfd10 *race_paths_dfd10 __asm__("D_L14_0015F70C");

typedef struct {
    struct Moby *owner;
    s32 timer;
    f32 path_speed;
    s32 path;
    f32 path_t;
    f32 path_u;
    f32 offset_x;
    f32 offset_y;
    f32 spin_x;
    f32 spin_y;
    f32 spin_z;
    u8 pad2C[4];
    f32 velocity[3];
} Data_dfd10;

extern f32 D_L14_00161B40 __attribute__((sda));
extern f32 D_L14_00161B44 __attribute__((sda));
extern f32 D_L14_00161B48 __attribute__((sda));
/* A piece of debris: follows its race path until its timer runs out, then flies, spins and falls until it hits something or leaves the map. */
void FUN_L14_002dfd10(struct Moby *moby) {
    Data_dfd10 *data = (Data_dfd10 *)moby->pvars;
    Vec4 probe;
    f32 vec[4];
    f32 *velocity;
    void *points;
    f32 speed;

    if (moby->state == 0) {
        points = race_paths_dfd10[data->path].points;
        qcopy(&probe, &moby->pos);
        follow_path_dfd10(points, &moby->pos, &data->path_t, &data->path_u, 0, data->path_speed);
        subtract_vector_xyz_dfd10(vec, &moby->pos, &probe);
        data->offset_x = vec[0];
        data->offset_y = vec[1];
        FUN_L14_002e03b8(moby);
        if (tick_countdown_dfd10(&data->timer) == 0)
            return;
        normalize_vector_xyz_dfd10(vec, vec, 0.5f);
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.25f;
        FUN_L00_0028dc90_dfd10(1, 0, moby, data->owner->oclass);
        spawn_explosion_dfd10(moby, vec, &probe, 0.0f, 0.0f, 5, 2, 4, 1.0f, 0.5f, 9.0f, 1.0f, -1, 15.0f, 0, 3, -1, 0);
        coll_sphere_dfd10(1.0f, &moby->pos, 0, (void *)D_001413D0_dfd10);
        mark_moby_for_removal_dfd10(moby);
        return;
    }
    velocity = data->velocity;
    add_vector_xyz_dfd10(&moby->pos, &moby->pos, velocity);
    speed = fabsf_dfd10(data->velocity[0]);
    if (D_L14_00161B48 * frame_scale < speed)
        data->velocity[0] *= (D_L14_00161B44 - 1.0f) * frame_scale + 1.0f;
    speed = fabsf_dfd10(data->velocity[1]);
    if (D_L14_00161B48 * frame_scale < speed)
        data->velocity[1] *= (D_L14_00161B44 - 1.0f) * frame_scale + 1.0f;
    if (data->velocity[2] > 0.0f)
        data->velocity[2] *= (D_L14_00161B44 - 1.0f) * frame_scale + 1.0f;
    data->velocity[2] -= D_L14_00161B40 * frame_scale_sq;
    moby->rot.x = fast_add_rotations_dfd10(moby->rot.x, data->spin_x);
    moby->rot.y = fast_add_rotations_dfd10(moby->rot.y, data->spin_y);
    moby->rot.z = fast_add_rotations_dfd10(moby->rot.z, data->spin_z);
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x ||
        moby->pos.y < 2.0f || 1021.0f < moby->pos.y ||
        moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal_dfd10(moby);
        return;
    }
    normalize_vector_xyz_dfd10(vec, &moby->rot_row2, 0.52f);
    add_vector_xyz_dfd10(&probe, &moby->pos, vec);
    if (tick_countdown_dfd10(&data->timer) != 0 || coll_sphere_dfd10(0.5f, &probe, 0, data->owner) != 0) {
        FUN_L00_0028dc90_dfd10(0, 0, moby, data->owner->oclass);
        spawn_explosion_dfd10(moby, velocity, &moby->pos, 0.0f, 0.0f, 5, 2, 4, 1.0f, 0.5f, 9.0f, 1.0f, -1, 15.0f, 0, 3, -1, 0);
        coll_sphere_dfd10(1.0f, &moby->pos, 0, (void *)D_001413D0_dfd10);
        mark_moby_for_removal_dfd10(moby);
        return;
    }
    if (random_integer_below_dfd10(2) != 0)
        FUN_L14_002e03b8(moby);
}
extern unsigned char *spawn_moby_2e0170(int) __asm__("FUN_0020c4f8");
extern int scale_game_frames_2e0170(int) __asm__("FUN_001f96f8");
extern void subtract_vector_2e0170(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_2e0170(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_vectors_2e0170(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void build_look_at_2e0170(void *, void *, void *, float) __asm__("FUN_00214890");
extern float random_scaled_2e0170(float, float) __asm__("FUN_002132a8");
extern float random_angle_2e0170(float, float) __asm__("FUN_L00_00257c48");
extern void create_effect_2e0170(void *, void *, void *, void *, void *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern float D_0015ED60;
extern char *race_paths_2e0170 __asm__("D_L14_0015F70C");

extern float D_L14_00161B30 __attribute__((sda));
extern float D_L14_00161B34 __attribute__((sda));
extern float D_L14_00161B38 __attribute__((sda));
extern float D_L14_00161B3C __attribute__((sda));
unsigned char *FUN_L14_002e0170(char *owner, float *scale, int mode) {
    unsigned char *spawned_moby = spawn_moby_2e0170(0x193);
    unsigned char *pos;
    if (spawned_moby != 0) {
        char *spawned_vars = *(char **)(spawned_moby + 0x78);
        float vec[8];
        pos = spawned_moby + 0x10;
        qcopy(pos, owner + 0x10);
        *(float *)(spawned_moby + 0x48) = *(float *)(owner + 0x48);
        spawned_moby[0x20] = mode;
        spawned_moby[0x30] = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        *(char **)spawned_vars = owner;
        if (spawned_moby[0x20] == 0) {
            int index;
            char *path;
            int *fields = (int *)spawned_vars;
            fields[1] = scale_game_frames_2e0170(90);
            path = race_paths_2e0170;
            *(float *)(spawned_vars + 8) = *scale * frame_time;
            index = *(int *)(*(char **)(owner + 0x78) + 0x60);
            fields[5] = 0;
            fields[3] = index;
            fields[4] = 0;
            create_effect_2e0170(*(void **)(path + index * 0x20 + 0x10),
                pos, vec, fields + 4, fields + 5, 0, 20.0f, 5.0f, *(float *)(spawned_vars + 0x14));
        } else {
            float angle;
            *(int *)(spawned_vars + 4) = scale_game_frames_2e0170(200);
            subtract_vector_2e0170(vec, pos, &hero.motion.pos);
            vec[2] = 0.0f;
            normalize_vector_2e0170(vec, vec,
                random_scaled_2e0170(D_L14_00161B30 * D_0015ED60,
                                       D_L14_00161B34 * D_0015ED60));
            cross_vectors_2e0170(vec + 4, &hero.gravity_dir, vec);
            angle = random_angle_2e0170(0.5759586691856384f, 1.3962633609771729f);
            build_look_at_2e0170(spawned_vars + 0x30, vec, &hero.gravity_dir, angle);
            *(float *)(spawned_vars + 0x38) = random_scaled_2e0170(D_L14_00161B38 * D_0015ED60,
                D_L14_00161B3C * D_0015ED60);
            *(float *)(spawned_vars + 0x20) = random_angle_2e0170(0.0f, 0.008726646f);
            *(float *)(spawned_vars + 0x24) = random_angle_2e0170(0.05235988f, 0.13962634f);
            *(float *)(spawned_vars + 0x28) = random_angle_2e0170(0.0f, 0.017453293f);
        }
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002B2A28.c (func_L14_002E17B8) */
/* Exhaust puffs from the moby's nozzle: two coloured sparks and three white puffs of shrinking size. */

typedef struct Moby Moby;

typedef int u128_2E17B8 __attribute__((mode(TI)));

typedef union {
    u128_2E17B8 q;
    float f[4];
} Vec4_2E17B8;

typedef struct Vars_2E17B8 {
    char pad0[0x18];
    float f18;                    /* 0x18 */
    float f1C;                    /* 0x1C */
} Vars_2E17B8;

typedef struct ParticleFade_2E17B8 {
    char pad0[4];
    int i4;                       /* 0x04 */
    char pad8[2];
    unsigned char bA;             /* 0x0A */
    unsigned char bB;             /* 0x0B */
    char padC[4];
    float scale;                  /* 0x10 */
    char pad14[2];
    short mode;                   /* 0x16 */
    int color;                    /* 0x18 */
} ParticleFade_2E17B8;

typedef struct Particle_2E17B8 {
    char pad0[3];
    unsigned char b3;             /* 0x03 */
    char pad4[6];
    short frames;                 /* 0x0A */
    float life;                   /* 0x0C */
    char pad10[0x10];
    ParticleFade_2E17B8 fade;     /* 0x20 */
} Particle_2E17B8;

typedef struct {
    unsigned char pad0[4];
    int w4;
    unsigned char pad8[2];
    unsigned char bA;
    unsigned char bB;
} T_eca8;

typedef struct {
    unsigned char pad0[8];
    unsigned char b8;
    unsigned char b9;
    short hA;
    unsigned char padC[0x14];
    T_eca8 tail;
} P_eca8;

char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern P_eca8 *FUN_L00_0026d000(void *, float, float, float, int, void *, float, int);
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float D_0015ED60;
extern int D_L14_0015F580_c __asm__("D_L14_0015F580");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
int FUN_L00_00257b90(int lo, int hi);
unsigned char *FUN_L00_00274948_c(u128_2E17B8 *a, u128_2E17B8 *b, int c, int d) __asm__("FUN_L00_00274948");
void random_spherical_offset(void *out, f32 a, f32 b) __asm__("FUN_00213358");
extern void func_L00_001FF4B0_2E17B8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_2E17B8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_002141A8_2E17B8(void *, float, float) __asm__("FUN_00213358");
extern Particle_2E17B8 *func_L00_002757E8_2E17B8(void *, void *, int, Moby *) __asm__("FUN_L00_00274948");
extern float func_002140F8_2E17B8(float, float) __asm__("FUN_002132a8");
extern int func_002140B0_2E17B8(int) __asm__("FUN_00213260");
extern int func_001F9850_2E17B8(int) __asm__("FUN_001f96f8");
extern float func_001FA888_2E17B8(int) __asm__("FUN_001fa6c0");
extern float func_001F9F90_2E17B8(float x) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2E17B8(float) __asm__("FUN_001f9de0");
extern float func_L00_00258C80_2E17B8(float lo, float hi) __asm__("FUN_L00_00257c48");
extern int func_L00_00258BC8_2E17B8(int, int) __asm__("FUN_L00_00257b90");
extern void func_L00_0026DA50_2E17B8(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern Particle_2E17B8 *func_L00_0026DEA0_2E17B8(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

void FUN_L14_002e03b8(Moby *moby) {
    Vec4_2E17B8 pos;
    Vec4_2E17B8 dir;
    Vec4_2E17B8 vel;
    Vec4_2E17B8 off;
    Vars_2E17B8 *pv;
    Particle_2E17B8 *particle;
    Particle_2E17B8 *e;
    ParticleFade_2E17B8 *fade;
    int i;
    int n;
    int color_intensity;
    int alpha;
    float size;

    pv = *(Vars_2E17B8 **)&moby->pvars;
    func_L00_001FF4B0_2E17B8(&dir, &*(Vec4_2E17B8 *)&moby->rot_row2, 0.2f);
    func_001F9BD8_2E17B8(&pos, &*(Vec4_2E17B8 *)&moby->pos, &dir);
    func_002141A8_2E17B8(&vel, 0.005f, 0.03f);
    particle = func_L00_002757E8_2E17B8(&pos, &vel, 0x7F, moby);
    if (particle != 0) {
        particle->life = func_002140F8_2E17B8(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        float f;
        int color;
        particle = func_L00_002757E8_2E17B8(&pos, &D_L14_0015F580_c, 0x7F, moby);
        if (particle == 0) {
            continue;
        }
        fade = &particle->fade;
        if (i == 2 && func_002140B0_2E17B8(8) == 0) {
            particle->life = 180000.0f;
        } else {
            particle->life = func_002140F8_2E17B8(80000.0f, 120000.0f);
        }
        particle->frames = func_001F9850_2E17B8(2);
        f = 1.0f / func_001FA888_2E17B8(particle->frames);
        color = 0x7F7F7F;
        fade->mode = 4;
        fade->color = color;
        fade->scale = f;
    }
    vel.f[0] = pv->f18;
    vel.f[1] = pv->f1C;
    vel.f[2] = 0.0f;
    vel.f[3] = 1.0f;
    if (moby->state == 0) {
        func_L00_001FF4B0_2E17B8(&dir, &*(Vec4_2E17B8 *)&moby->rot_row2, 0.95f);
    } else {
        func_L00_001FF4B0_2E17B8(&dir, &*(Vec4_2E17B8 *)&moby->rot_row2, 1.15f);
        vel.f[2] = D_0015ED60 * 0.03f;
    }
    func_001F9BD8_2E17B8(&pos, &*(Vec4_2E17B8 *)&moby->pos, &dir);
    off.f[0] = func_001F9F90_2E17B8(moby->rot.z) * 0.3f;
    off.f[1] = func_001F9FA8_2E17B8(moby->rot.z) * 0.3f;
    off.f[2] = 0.0f;
    func_001F9BD8_2E17B8(&pos, &pos, &off);
    if (moby->state == 1) {
        pos.f[0] += func_L00_00258C80_2E17B8(0.0f, 0.1f);
        pos.f[1] += func_L00_00258C80_2E17B8(0.0f, 0.1f);
    }
    func_L00_0026DA50_2E17B8(&pos, &vel, 0x4F007FFF, 0x1FFFFFFF,
                             func_L00_00258BC8_2E17B8(func_001F9850_2E17B8(10), func_001F9850_2E17B8(20)), 1, 60000.0f);
    n = func_002140B0_2E17B8(6);
    if (func_002140B0_2E17B8(2) != 0) {
        n = -n;
    }
    size = func_002140F8_2E17B8(60000.0f, 140000.0f);
    color_intensity = func_L00_00258BC8_2E17B8(0x30, 0xFF);
    alpha = 0x60000000;
    e = func_L00_0026DEA0_2E17B8(&pos, 0.025f, 1.0f, 1.0f, n, &vel, size, color_intensity | ((color_intensity << 16) | ((color_intensity << 8) | alpha)));
    if (e != 0) {
        ParticleFade_2E17B8 *ef;
        ef = &e->fade;
        e->b3 = 0x44;
        e->frames = func_001F9850_2E17B8(0x3C);
        ef->i4 = 2;
        ef->bA = 0x60;
        ef->bB = e->frames;
    }
}
/* Exhaust puffs from the moby's nozzle: two coloured sparks and three white puffs of shrinking size. */
extern int below_y(int) __asm__("FUN_00213260");
extern int frames_y(int) __asm__("FUN_001f96f8");
extern void scale_y(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_y(void *, void *, void *) __asm__("FUN_001f9a10");
extern P_eca8 *puff_y(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern float D_L14_0015F580_y[] __asm__("D_L14_0015F580") __attribute__((section(".sdata")));


extern float D_L14_00161FCC __attribute__((sda));
extern float D_L14_00161FD0 __attribute__((sda));
extern float D_L14_00161FD4 __attribute__((sda));
extern float D_L14_00161FD8 __attribute__((sda));
extern int D_L14_00161FE4 __attribute__((sda));
void FUN_L14_002feca8(unsigned char *moby) {
    float off[4];
    float pos[4];
    int i;
    int j;
    int n;
    int life;
    float size;
    scale_y(off, moby + 0xC0, D_L14_00161FCC);
    add_y(pos, moby + 0x10, off);
    scale_y(off, moby + 0xE0, D_L14_00161FD0);
    add_y(pos, pos, off);
    for (i = 0; i < 2; i++) {
        P_eca8 *part;
        T_eca8 *tail;
        n = below_y(0x10);
        part = puff_y(pos, 0.2f, 1.0f, 0.9f, below_y(2) == 0 ? n : -n, D_L14_0015F580_y, 100000.0f, D_L14_00161FE4);
        if (part) {
            tail = &part->tail;
            part->hA = frames_y(0xC);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->hA;
        }
    }
    scale_y(off, moby + 0xC0, D_L14_00161FD4);
    add_y(pos, moby + 0x10, off);
    scale_y(off, moby + 0xE0, D_L14_00161FD8);
    add_y(pos, pos, off);
    n = 0x10;
    life = frames_y(2);
    size = 80000.0f;
    for (j = 0; j < 3; j++) {
        P_eca8 *part;
        T_eca8 *tail;
        part = puff_y(pos, 0.05f, 1.0f, 1.0f, n, D_L14_0015F580_y, size, 0x7FFFFFFF);
        if (part) {
            tail = &part->tail;
            part->hA = life;
            part->b8 = below_y(0xFF);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->hA;
        }
        n = -n;
        size -= 20000.0f;
        life *= 2;
    }
}
typedef struct {
    struct Moby *link;
    float rest_z;
    float z_speed;
    float phase;
    float spin[3];
} FloatMineVars_ff640;

typedef struct {
    float damage;
    s16 unk4;
    u8 pad6[0xA];
    float unk10;
    u8 pad14[0x26];
    u8 unk3A;
    u8 unk3B;
    u8 pad3C[4];
} HitInfo_ff640;

typedef struct {
    u8 pad0[0x20];
    struct Moby *moby;
} MobyHit_ff640;

extern char *D_L14_0015FFDC;
extern float D_L14_00162000 __attribute__((sda));
extern float probe_ground_height_ff640(float, void *, int) __asm__("FUN_00213508");
extern float random_float_between_ff640(float, float) __asm__("FUN_002132a8");
extern void ease_ff640(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern MobyHit_ff640 *find_moby_hit_ff640(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern void clear_ff640(void *, int) __asm__("FUN_001f9810");
extern int take_damage_ff640(struct Moby *, MobyHit_ff640 *, HitInfo_ff640 *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void clear_vector_ff640(void *) __asm__("FUN_001f99f8");
extern int play_sound_ff640(int, int, struct Moby *) __asm__("FUN_0022da68");
extern void explode_ff640(struct Moby *, void *, void *, float, float, int, int, int, float, float, float, float,
                          int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void remove_moby_ff640(struct Moby *) __asm__("FUN_0020c828");
extern int sphere_visible_ff640(float *, float) __asm__("FUN_001fa728");
extern float add_rotations_ff640(float, float) __asm__("FUN_001fa580");
extern float sin_ff640(float) __asm__("FUN_001f9de0");
extern int ftoi_ff640(float) __asm__("FUN_001fa6d0");

/* Floating mine: spins and pulses (and bobs in level 16); blows up on a hit and respawns once out of view. */
void FUN_L14_002ff640(struct Moby *moby) {
    enum {
        FLOAT_MINE_INITIALIZE = 0,
        FLOAT_MINE_ACTIVE = 1,
        FLOAT_MINE_WAIT_OFFSCREEN_RESPAWN = 2
    };
    FloatMineVars_ff640 *vars = (FloatMineVars_ff640 *)moby->pvars;
    union {
        HitInfo_ff640 info;
        float sphere[4];
    } scratch;
    float vel[4];
    int count;
    MobyHit_ff640 *hit;
    float glow_pulse_sine;
    int r;
    int g;

    switch (moby->state) {
    case FLOAT_MINE_INITIALIZE:
        vars->rest_z = -1.0f;
        if (vars->link == 0) {
            moby->pos.z = probe_ground_height_ff640(0.5f, &moby->pos, 0) + 0.7f;
            vars->spin[0] = random_float_between_ff640(0.010471975f, 0.020943951f);
            vars->spin[1] = random_float_between_ff640(0.006981317f, 0.013962634f);
            vars->spin[2] = random_float_between_ff640(0.0034906585f, 0.006981317f);
            moby->state = 1;
        } else if ((s8)vars->link->state < 0) {
            moby->state = 1;
        }
        break;
    case FLOAT_MINE_ACTIVE: {
        if (current_level_index == 16) {
            if (vars->rest_z < 0.0f)
                vars->rest_z = probe_ground_height_ff640(0.5f, &moby->pos, 0) + 0.7f;
            ease_ff640(&moby->pos.z, &vars->z_speed, vars->rest_z, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f, 0.0f);
            if ((char *)moby > D_L14_0015FFDC && hero.state.control_mode != 15)
                goto remove;
        }
        hit = find_moby_hit_ff640(moby, 0x230000, 0);
        clear_ff640(&scratch.info, 0x40);
        scratch.info.unk4 = 1;
        scratch.info.damage = 1.0f;
        scratch.info.unk10 = 0.5f;
        scratch.info.unk3A = 7;
        scratch.info.unk3B = 0xFF;
        take_damage_ff640(moby, hit, &scratch.info, 0, &count, 0, 0, 4);
        if ((count >= 2 && hit != 0 && hit->moby->oclass != 0x283) || hero.coll_hit_moby == (u8 *)moby) {
            clear_vector_ff640(vel);
            play_sound_ff640(0, 0, moby);
            if (hero.coll_hit_moby == (u8 *)moby)
                explode_ff640(moby, vel, &moby->pos, 3.0f, 1.0f, 10, 20, 60, 3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            else
                explode_ff640(moby, vel, &moby->pos, 0.0f, 0.0f, 10, 20, 60, 3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            if ((char *)moby < D_L14_0015FFDC) {
                moby->state = 2;
                moby->flags |= 0x41;
                moby->hit_slot = 0xFF;
                moby->collision = 0;
                moby->update_dist = 0xFF;
                return;
            }
        remove:
            remove_moby_ff640(moby);
            return;
        }
        break;
    }
    case FLOAT_MINE_WAIT_OFFSCREEN_RESPAWN: {
        qcopy(scratch.sphere, &moby->pos);
        scratch.sphere[3] = 3.0f;
        if (sphere_visible_ff640(scratch.sphere, 64.0f) == -1) {
            moby->state = 1;
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
        }
        break;
    }
    }
    vars->phase = add_rotations_ff640(vars->phase, D_L14_00162000);
    glow_pulse_sine = sin_ff640(vars->phase);
    r = ftoi_ff640(glow_pulse_sine * 48.0f) + 0xCF;
    g = ftoi_ff640(glow_pulse_sine * 32.0f) + 0x30;
    moby->glow_color = g << 16 | (0xFF000000 | g << 8) | r;
    moby->rot.x = add_rotations_ff640(moby->rot.x, vars->spin[0]);
    moby->rot.y = add_rotations_ff640(moby->rot.y, vars->spin[1]);
    moby->rot.z = add_rotations_ff640(moby->rot.z, vars->spin[2]);
}
