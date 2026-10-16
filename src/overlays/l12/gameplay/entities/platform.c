/* Level 12 moby classes: platform, miner, blarg_trooper, blarg_helicopter, edwina, helicopter, crane_arm. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    float position[3];
    char unk0C[0x12];
    u16 visibility;
    char unk20[0x1170];
} L12Slot;
extern char D_L12_001CBF40_c[] __asm__("D_L12_001CBF40");
extern char D_L12_001CAD00_c[] __asm__("D_L12_001CAD00");
extern char D_L12_001CBE00_c[] __asm__("D_L12_001CBE00");

extern char D_L12_0020BC90[];
extern char D_L12_00167240[];
extern float absolute_float(float) __asm__("func_001F99C0");
extern void FUN_L12_002bf770(int);

extern int D_L12_00151388 __attribute__((sda));
extern float D_0015ED70_c __asm__("D_0015ED70");
extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern void slot_limit_c(int) __asm__("FUN_001e93b0");
extern void initialize_slots_c(void *, int) __asm__("FUN_L01_002b7a48");
extern void initialize_slot_data_c(float) __asm__("FUN_L01_002b7ef0");
extern void copy_slot_c(void *, void *) __asm__("FUN_L01_002b7c68");
extern void finish_slots_c(void *, int) __asm__("FUN_L01_002b7fe0");
extern float D_L12_00180340[8];
extern int D_L12_001612D0;
extern void *D_L12_001612D4;
extern int D_L12_001612D8;
extern float D_L12_0016120C;
extern float D_L12_00161210;
extern float D_L12_00161214;
extern int D_L12_00161208;
extern float angle_c(float, float) __asm__("FUN_001f9e90");
extern float sine_c(float) __asm__("FUN_001f9dc8");
extern float cosine_c(float) __asm__("FUN_001f9de0");
extern int random_integer_below_c(int) __asm__("FUN_00213260");
extern float random_float_between_c(float, float) __asm__("FUN_002132a8");
extern void move_scalar_c(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void move_slot_c(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");
extern int check_moby_c(float, void *) __asm__("FUN_L01_00275690");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void register_callback_c(void *, void *) __asm__("FUN_L12_0021be50");
extern void FUN_L12_002bf6d0(struct Moby *);
extern void FUN_L12_002bf770(int);
extern void FUN_L12_002bf748(void);

void FUN_L12_002bf140(struct Moby *moby) {
    int state = moby->state;
    int *data = (int *)moby->pvars;
    int slot;
    float value;
    float other;
    int i;
    switch (state) {
    case 0:
        i = (data[3] = D_L12_00151388) + 1;
        D_L12_00151388 = i;
        if (i > 0x7f) {
            slot_limit_c((int)D_L12_0020BC90);
        }
        moby->scale = moby->pclass->scale * 2.0f;
        data[0] = 3;
        moby->update_dist = 0xff;
        moby->flags |= 1;
        if (data[3] == 0) {
            {
                int i = 16;
                do {
                    ((L12Slot *)D_L12_001CBF40_c)[i].visibility = 0;
                } while (--i >= 0);
            }
            initialize_slots_c(D_L12_001CBF40_c, 17);
            initialize_slot_data_c(1.0f);
            *(float *)(D_L12_001CAD00_c + 0x00) = 16.0f;
            *(float *)(D_L12_001CAD00_c + 0x04) = 16.0f;
            *(float *)(D_L12_001CAD00_c + 0x08) = -8.0f;
            *(float *)(D_L12_001CAD00_c + 0x0c) = -8.0f;
            *(float *)(D_L12_001CAD00_c + 0x10) = 1.0f;
            *(float *)(D_L12_001CAD00_c + 0x14) = 1.0f;
            *(float *)(D_L12_001CAD00_c + 0x18) = 0.9f;
            *(float *)(D_L12_001CAD00_c + 0x20) = 0.1f;
            value = angle_c(D_L12_00180340[4], D_L12_00180340[5]);
            *(float *)(D_L12_001CAD00_c + 0x30) = sine_c(value) * 0.57735f;
            *(float *)(D_L12_001CAD00_c + 0x34) = cosine_c(value) * 0.57735f;
            *(float *)(D_L12_001CAD00_c + 0x38) = -0.57735f;
            *(int *)(D_L12_001CAD00_c + 0x28) = data[0] + 0x28;
            *(int *)(D_L12_001CAD00_c + 0x2c) = data[1] + 0x28;
            *(u8 *)(D_L12_001CAD00_c + 0x3c) = 0x1c;
            *(u8 *)(D_L12_001CAD00_c + 0x3d) = 0x40;
            D_L12_001612D0 = (int)D_L12_001CBF40_c;
            D_L12_001612D4 = D_L12_001CBE00_c;
            D_L12_001612D8 = 17;
            D_L12_0016120C = 32768.0f;
            D_L12_00161210 = 255.0f;
            D_L12_00161214 = 48.0f;
            D_L12_00161208 = 0;
        }
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190) = moby->pos.x;
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190 + 4) = moby->pos.y;
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190 + 8) = moby->pos.z;
        *(int *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x14) = data[0] + 0x28;
        *(int *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x18) = data[1] + 0x28;
        *(u8 *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x1c) = 0x1c;
        *(u8 *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x1d) = 0x44;
        ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0xffff;
        copy_slot_c(D_L12_001CBF40_c + data[3] * 0x1190, D_L12_001CBE00_c + data[3] * 16);
        moby->state = 1;
        break;
    case 1:
        D_L12_00151388 = 0;
        FUN_L12_002bf770(data[3]);
        moby->state = 2;
        *(float *)(data + 4) = moby->pos.z;
        break;
    case 2:
        if ((moby->command & 4) == 0) {
            moby->command = 1;
        } else {
            data[5] = 1;
            *(float *)(data + 4) += 5.0f;
            moby->command = 1;
        }
        if (data[5] != 0) {
            move_scalar_c(&moby->pos.z, (float *)(data + 2), *(float *)(data + 4), D_0015ED70_c * 4.0f, D_0015ED70_c * 4.0f, D_0015ED6C_c * 4.0f);
            FUN_L12_002bf6d0(moby);
        }
        if (absolute_float(hero.motion.pos.f[0] - moby->pos.x) <= 8.0f ||
            absolute_float(hero.motion.pos.f[1] - moby->pos.y) <= 8.0f) {
            hero.water_surface_z = moby->pos.z;
        }
        if (random_integer_below_c(200) == 0) {
            char *p;
            other = random_float_between_c(-4.0f, 4.0f);
            value = *(float *)(D_L12_001CBF40_c + data[3] * 0x1190);
            value += other;
            other = random_float_between_c(-4.0f, 4.0f);
            p = D_L12_001CBF40_c + data[3] * 0x1190;
            move_slot_c(value, *(float *)(p + 4) + other, 1.0f, -0.05f, p, 1, 1);
        }
        break;
    }
update_slot:
    slot = data[3];
    if (slot >= 0) {
        float visible = 0.0f;
        if (check_moby_c(64.0f, moby) != -1) visible = 1.0f;
        if (data[6] != -1 && is_point_inside_clip_volume(D_L12_00167240, data[6]) == 0) visible = 0.0f;
        if (visible == 0.0f) {
            ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0;
        } else {
            ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0xffff;
        }
        if (data[3] == 0) {
            finish_slots_c(D_L12_001CBF40_c, 17);
            register_callback_c(FUN_L12_002bf748, moby);
        }
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

extern Ent D_L12_001CBF40[];
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L12_002bf770(int arg) {
    int i;
    D_L12_001CBF40[arg].n[0] = 0xFF;
    D_L12_001CBF40[arg].n[2] = 0xFF;
    D_L12_001CBF40[arg].n[1] = 0xFF;
    D_L12_001CBF40[arg].n[3] = 0xFF;
    D_L12_001CBF40[arg].n[4] = 0xFF;
    D_L12_001CBF40[arg].n[5] = 0xFF;
    D_L12_001CBF40[arg].n[6] = 0xFF;
    D_L12_001CBF40[arg].n[7] = 0xFF;
    for (i = 0; i < 17; i++) {
        if (arg != i) {
            Ent *t = D_L12_001CBF40;
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
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E29C8), where it is exact; names translated to the US level program. */

extern int FUN_L00_002db8f8(void *);
extern void FUN_L01_0026e090(int, int);

int FUN_L12_002e16b8(struct Moby *moby) {
    int active;
    if (moby->state == 9 || moby->state == 11 || moby->state == 13)
        return 0;
    active = FUN_L00_002db8f8(moby);
    if (active) {
        if (moby->group != 0xFF)
            FUN_L01_0026e090(moby->group, 1);
        moby->state = 8;
    } else if (moby->state == 8) {
        moby->state = 1;
    }
    return active;
}
#define NOT_SDA

#define MACRO_ADDR

/* Move the vendor moby to state eight unless it is in state nine or eleven. */
/* Ported from rac1-decomp (src/overlays/l12/vendor_002C0310.c: func_L12_002E2B08), where it is exact; names translated to the US level program. */


extern int FUN_L00_002dbe20(struct Moby *);

int FUN_L12_002e17f8(struct Moby *moby) {
    int result = FUN_L00_002dbe20(moby);
    if (moby->state == 9 || moby->state == 11) {
        return 0;
    }
    moby->state = 8;
    return result;
}
#include "sda.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2B88), where it is exact; names translated to the US level program. */

typedef union {
    long long quad;
    float f[4];
} L12Vector;

extern float D_0015ED70_c __asm__("D_0015ED70");
extern float probe_ground_height_c(void *, int, float) __asm__("func_00213508");
extern void approach_value_c(f32 *, f32, f32) __asm__("func_00213ED8");
extern float func_00214358_order(float, void *, int) __asm__("FUN_00213508");
extern void func_00214D28_order(float, float, void *) __asm__("FUN_00213ed8");

void FUN_L12_002e1878(char *moby) {
    L12Vector position;
    float value;
    qcopy(&position, moby + 0x10);
    position.f[2] += 0.5f;
    value = func_00214358_order(0.5f, &position, 0);
    func_00214D28_order(value, D_0015ED70_c * 27.0f, moby + 0x18);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2BF8), where it is exact; names translated to the US level program. */

extern int random_integer_below(int);
extern int scale_game_frames(int);
extern void blend_moby_animation(void *, int, int, int);

void FUN_L12_002e18e8(struct Moby *moby) {
    switch (random_integer_below(2)) {
    case 0:
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(8));
        break;
    case 1:
        if (moby->prev_seq != 0xF)
            blend_moby_animation(moby, 0xF, 0, scale_game_frames(8));
        break;
    default:
        if (moby->prev_seq != 0x10)
            blend_moby_animation(moby, 0x10, 0, scale_game_frames(8));
        break;
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Scatters 27 particles around a point with random offsets and velocities. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2CB8), where it is exact; names translated to the US level program. */


extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern short D_L12_00161888 __attribute__((sda));
extern short D_L12_0016188C __attribute__((sda));
extern short D_L12_00161890 __attribute__((sda));
extern short D_L12_00161894 __attribute__((sda));
extern short D_L12_00161898 __attribute__((sda));
extern short D_L12_0016189C __attribute__((sda));
extern short D_L12_001618A0 __attribute__((sda));
extern short D_L12_001618A4 __attribute__((sda));
extern short D_L12_001618AC __attribute__((sda));
extern short D_L12_001618B0 __attribute__((sda));
extern short D_L12_001618B4 __attribute__((sda));
extern short D_L12_001618B8 __attribute__((sda));
extern short D_L12_001618BC __attribute__((sda));
extern short D_L12_001618C0 __attribute__((sda));
int D_L12_001618C4 = 0xa0f0e8e0;
int D_L12_001618C8 = 0x20f0e8e0;
int D_L12_001618CC = 0x10017;
void FUN_L12_002e19a8(void *pos) {
    float v[4];
    float w[4];
    float v3[4];
    float v4[4];
    float r;
    int i;
    int a, b, c;
    *(OvlQuad *)v = *(OvlQuad *)pos;
    i = 0x1A;
    do {
        r = random_angle_radians();
        qcopy(w, v);
        w[2] = w[2] + random_float_between_alt(-0.25f, -0.75f);
        w[0] = w[0] + random_float_between_alt(-0.25f, 0.25f);
        w[1] = w[1] + random_float_between_alt(-0.25f, 0.25f);
        clear_u64_value(v3);
        v4[0] = fast_cos(r) * (random_float_between_alt(*(float *)&D_L12_00161898,
                                                        *(float *)&D_L12_0016189C) *
                               frame_time);
        v4[1] = fast_sin(r) * (random_float_between_alt(*(float *)&D_L12_00161898,
                                                        *(float *)&D_L12_0016189C) *
                               frame_time);
        *(int *)&v4[2] = 0;
        v3[2] = random_float_between_alt(*(float *)&D_L12_00161888, *(float *)&D_L12_0016188C) *
                frame_time;
        v4[2] = random_float_between_alt(*(float *)&D_L12_00161890, *(float *)&D_L12_00161894) *
                frame_time;
        v3[3] = *(float *)&D_L12_001618A0;
        v4[3] = *(float *)&D_L12_001618A4;
        a = func_001FA898_r(random_float_between_alt((float)*(int *)&D_L12_001618AC,
                                                     (float)*(int *)&D_L12_001618B0));
        b = func_001FA898_r(random_float_between_alt((float)*(int *)&D_L12_001618B4,
                                                     (float)*(int *)&D_L12_001618B8));
        c = func_001FA898_r(random_float_between_alt((float)*(int *)&D_L12_001618BC,
                                                     (float)*(int *)&D_L12_001618C0));
        spawn_spark(w, v3, v4, D_L12_001618C4, D_L12_001618C8, a, b, c,
                     D_L12_001618CC);
    } while (--i >= 0);
}
#include "rnc/math/vector.h"

/* Updates a Hoven troll's idle, pursuit, melee, leap, landing and death states. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2EF0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct TrollData54_2E2EF0 TrollData54_2E2EF0;

typedef struct TrollMoby54_2E2EF0 {
    char p0[0x10]; float pos[4]; unsigned char state, group; char p22[2]; char *model;
    char p28[9]; unsigned char enabled; unsigned short joints, flags; char p36[0xA];
    float rot[4]; char p50[2]; unsigned char anim_end, anim; float anim_time, anim_rate;
    char p5C[0x14]; unsigned char anim_flags; char p71[7]; TrollData54_2E2EF0 *data;
    char p7C[3]; unsigned char active; char p80[0x14]; int collision;
    char p98[0x24]; unsigned char proximity;
} TrollMoby54_2E2EF0;

struct TrollData54_2E2EF0 {
    char p0[0x20]; float health; char p24[5]; unsigned char mode; char p2A[0x2E];
    unsigned char blend; char p59; unsigned char count; char p5B[0x6D]; short stuck; char pCA[6]; void *tuning;
    char pD4[0x4C]; float fall[4], gravity, zero, launch_speed, launch_time; int fall_flags, fall_mode; float damping;
    char p14C[0x11]; unsigned char falling; char p15E[0x12]; float fall_a, fall_b; char p178[8];
    int effect_class; char p184[4]; float effect_size0, effect_size1; char p190[0x14]; float effect_speed;
    char p1A8[0x10]; int effect_flags; char p1BC[0x14]; float idle_pos[4], spare, idle_speed;
    char p1E8[0x18]; char collision_data[0x68]; float look_angle; char p26C[0x14];
    float home[4]; char p290[0x10]; float turn_velocity, circle_angle, circle_rate, range;
    char p2B0[8]; int init0, sleep, circle_timer, alert_timer; short group_timer, pad2CA, idle_timer, pad2CE;
    int path, blocked, return_timer, group_enabled; int *group_count; int group_check;
};

typedef struct TargetReport54_2E2EF0 { float v0[4], v10[4], v20[4], v30[4]; TrollMoby54_2E2EF0 *moby; int mode; char pad48[8]; } TargetReport54_2E2EF0;

typedef struct {
    char pad0[0x248];
    void *child;
} Level12VendorData_2e1be0;

typedef struct {
    char pad0[0x78];
    Level12VendorData_2e1be0 *data;
    char pad7C[0x38];
    short fieldB4;
} Level12VendorMoby;

extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
extern char *D_L12_001B0930[];
extern char D_L12_00167240[];
int D_L12_001618E8 = 0xa030303;
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz_c3(void *) __asm__("FUN_001f9af0");
extern float D_0015ED64;
extern float D_0015ED6C;
extern float D_0015ED70;
extern float FUN_001f9b80(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height_c2(void *, int, float) __asm__("func_00213508");
extern float D_L12_001618D8 __attribute__((sda));
extern float D_L12_001618DC __attribute__((sda));
extern int FUN_001f9770(void *);
extern int FUN_L00_00259740(void *, void *, int);
extern int FUN_L00_002dc9f0();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 rand(void);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 tick_countdown_32_alt_c2(s32 *) __asm__("FUN_001f9740");
extern void FUN_L00_00257470_c(Level12VendorMoby *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_00259bc8(void *, int, int, void *, void *, float);
extern void FUN_L00_0025c558(void *, void *, float, int, int, int);
extern void FUN_L00_0025fcb8(char *, float *, float);
extern void FUN_L00_002628d8_c(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void FUN_L12_002e3098_u(struct Moby *) __asm__("FUN_L12_002e3098");
extern void approach_value_alt(void *, float, float) __asm__("FUN_00213ed8");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a28");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float FUN_L00_0025abf0(float *a, float *b, float *out, float speed, float g);
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L00_00257b90(int lo, int hi);
int FUN_L00_002591d0(char **out, int idx, int a, int b);
int FUN_L00_002592b8(char **out, char *target, int a, int b);
int FUN_L00_0025c698(void *moby, void *motion);
int FUN_L12_002e2eb8_c(struct Moby *m, float speed) __asm__("FUN_L12_002e2eb8");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void FUN_L00_0025a120(char *o);
void FUN_L00_00261630(u8 *o, u8 *s, f32 a, f32 b);
void FUN_L00_00263b38(char *p, float x);
void FUN_L01_0026d930(char *p);
void FUN_L10_002e3de8(char *moby);
void FUN_L12_002e18e8_c(struct Moby *moby) __asm__("FUN_L12_002e18e8");
void FUN_L12_002e19a8_c(void *pos) __asm__("FUN_L12_002e19a8");
void FUN_L12_002e3528_c(struct Moby *moby) __asm__("FUN_L12_002e3528");
extern char D_0013E633_2E2EF0[] __asm__("D_001413D0");
extern char *D_L12_001B0C30_2E2EF0[] __asm__("D_L12_001B0930");
extern char D_L12_00167240[];
extern void func_L12_002E4838_2E2EF0(void *) __asm__("FUN_L12_002e3528");
extern void func_L00_00264BB0_2E2EF0(void *,float) __asm__("FUN_L00_00263b38");
extern int select54(char *,char *,float) __asm__("FUN_L01_00274b78");
extern void func_00213DE0_2E2EF0(void *,int,int,int) __asm__("FUN_00212f90");
extern void func_L01_0026E8E0_2E2EF0(char *) __asm__("FUN_L01_0026d930");
extern int func_001F9850_2E2EF0(int) __asm__("FUN_001f96f8");
extern int func_L00_00258BC8_2E2EF0(int,int) __asm__("FUN_L00_00257b90");
extern int func_001160D8_2E2EF0(void) __asm__("FUN_001160d8");
extern int func_002140B0_2E2EF0(int) __asm__("FUN_00213260");
extern float func_00214358_2E2EF0(void *,int,float) __asm__("FUN_00213508");
extern float func_001F9D10_2E2EF0(void *,void *) __asm__("FUN_001f9b48");
extern float func_L00_001FF860_2E2EF0(float,float) __asm__("FUN_001f9e90");
extern float func_001FA850_2E2EF0(float,float) __asm__("FUN_001fa688");
extern float func_001F9B88_2E2EF0(float) __asm__("FUN_001f99c0");
extern float func_002140F8_2E2EF0(float,float) __asm__("FUN_002132a8");
extern float smooth54(float *,float,float) __asm__("FUN_00213ed8");
extern void func_L00_002626A8_2E2EF0(char *,char *,float,float) __asm__("FUN_L00_00261630");
extern void fast_vec_sub(void *,void *,void *) __asm__("FUN_001f9a28");
extern float func_001F9D48_2E2EF0(void *,void *) __asm__("FUN_001f9b80");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int func_L00_0025A208_2E2EF0(int *,int,int,int) __asm__("FUN_L00_002591d0");
extern int func_L00_0025A2F0_2E2EF0(int *,int,int,int) __asm__("FUN_L00_002592b8");
extern int func_001F9938_2E2EF0(void *) __asm__("FUN_001f9770");
extern void func_L01_0026F040_2E2EF0(int,int) __asm__("FUN_L01_0026e090");
extern void func_L12_002E2CB8_2E2EF0(void *) __asm__("FUN_L12_002e19a8");
extern void func_L00_002592B0_2E2EF0(char *,float *,float,float,float,float) __asm__("FUN_L00_00258278");
extern void func_L12_002E2BF8_2E2EF0(unsigned char *) __asm__("FUN_L12_002e18e8");
extern float func_L00_0025BC48_2E2EF0(void *,void *,void *,float,float) __asm__("FUN_L00_0025abf0");
extern void setup_fall54(float,char *,float *,int,int,int) __asm__("FUN_L00_0025c558");
extern int func_L00_0025D6F0_2E2EF0(void *,void *) __asm__("FUN_L00_0025c698");
extern int func_L12_002E41C8_2E2EF0(char *,float) __asm__("FUN_L12_002e2eb8");
extern int func_001F9908_2E2EF0(int *) __asm__("FUN_001f9740");
extern int func_00215B18_2E2EF0(char *,float) __asm__("FUN_00214cc8");
extern float func_001FA790_2E2EF0(float,float) __asm__("FUN_001fa5c8");
extern float func_001FA748_2E2EF0(float,float) __asm__("FUN_001fa580");
extern float func_001F9F90_2E2EF0(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2E2EF0(float) __asm__("FUN_001f9de0");
extern void func_L00_0025AC00_2E2EF0(void *,int,int,void *,void *,float) __asm__("FUN_L00_00259bc8");
extern int func_L00_0025A778_2E2EF0(void *,void *,int) __asm__("FUN_L00_00259740");
extern int func_L00_002DDEA0_2E2EF0(void *,void *) __asm__("FUN_L00_002dc9f0");
extern float height54(void *) __asm__("FUN_0020c9e0");
extern void func_L00_002584A8_2E2EF0(void *,int,int) __asm__("FUN_L00_00257470");
extern void func_L10_002E51A8_2E2EF0(char *) __asm__("FUN_L10_002e3de8");
extern void func_L12_002E43A8_2E2EF0(void *) __asm__("FUN_L12_002e3098");
extern void func_0020D678_2E2EF0(void *) __asm__("FUN_0020c828");
extern void func_L00_00263950_2E2EF0(char *,char *,int,float,float) __asm__("FUN_L00_002628d8");
extern void func_L00_0025B178_2E2EF0(void *) __asm__("FUN_L00_0025a120");

void FUN_L12_002e1be0(TrollMoby54_2E2EF0 *moby) {
    TargetReport54_2E2EF0 work;
    TrollMoby54_2E2EF0 *iterator;
    float trajectory;
    TrollData54_2E2EF0 *data = moby->data;
    TrollMoby54_2E2EF0 *target;
    float angle;
    int count, first, blocked, hit;
    func_L12_002E4838_2E2EF0(moby);
    func_L00_00264BB0_2E2EF0(data->collision_data,2.2f);
    select54((char *)moby,(char *)&work,data->range);
    target=work.moby;
    if (!target) target=*(TrollMoby54_2E2EF0 **)D_0013E633_2E2EF0;
    if (moby->pos[2]<26.0f) { func_0020D678_2E2EF0(moby); return; }
    switch(moby->state) {
    case 0:
        if(data->sleep) {
            qcopy(data->home,moby->pos); moby->state=9;
            if(moby->anim!=12) func_00213DE0_2E2EF0(moby,12,0,3);
            moby->collision=0; moby->flags=(moby->flags|0x41)&0xEFFF;
        } else if(data->home[3]!=0.0f) {
            moby->state=11; moby->joints=0; moby->flags&=0xEFFF; moby->collision=0;
        } else {
            qcopy(data->home,moby->pos); moby->state=1;
            if(moby->anim!=0) func_00213DE0_2E2EF0(moby,0,0,3);
        }
        {
        int initial_mode=1; /* loaded ahead of the other constants: it is the last one stored (retail keeps it in $a2 into the call's delay slot) */
        data->alert_timer=0; data->init0=0; data->damping=0.5f; data->health=2.0f; data->blend=8; data->count=5; data->mode=initial_mode; data->fall_flags=0x200;
        func_L01_0026E8E0_2E2EF0((char *)&data->effect_class);
        data->effect_class=0x38D; data->effect_flags|=0x10; data->effect_speed=D_0015ED6C*5.5f;
        data->effect_size1=0.5f; data->effect_size0=0.5f;
        if(data->group_enabled) { first=func_001F9850_2E2EF0(20); data->group_timer=func_L00_00258BC8_2E2EF0(first,func_001F9850_2E2EF0(170)); }
        qcopy(data->idle_pos,moby->pos);
        first=func_001F9850_2E2EF0(120); data->idle_timer=func_L00_00258BC8_2E2EF0(first,func_001F9850_2E2EF0(700));
        qcopy(work.v0,moby->pos); work.v0[2]+=0.8f;
        moby->pos[2]=func_00214358_2E2EF0(work.v0,0,0.5f);
        if(func_001160D8_2E2EF0()&1) moby->flags|=0x8000;
        data->tuning=&D_L12_001618E8;
        }
        break;
    case 1: {
        float idle_distance, idle_angle, idle_difference;
        idle_distance=func_001F9D10_2E2EF0(target->pos,data->home);
        idle_angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        idle_difference=func_001FA850_2E2EF0(moby->rot[2],idle_angle);
        if(idle_distance<data->range && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<4.0f) {
            moby->state=3;
            if(idle_difference<0.9599311f) {
                if(moby->anim!=2) func_00213DE0_2E2EF0(moby,2,0,func_001F9850_2E2EF0(5));
                moby->anim_rate=1.55f;
            } else if(moby->anim!=17) func_00213DE0_2E2EF0(moby,17,0,func_001F9850_2E2EF0(5));
        } else if(((moby->proximity&1)&&func_002140B0_2E2EF0(19)==0)||moby->proximity==7) {
            if(func_002140B0_2E2EF0(256)&1) data->circle_rate=-data->circle_rate;
            data->circle_angle=data->circle_rate*0.017453292f;
            data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f));
            moby->state=3;
            if(moby->anim!=2) func_00213DE0_2E2EF0(moby,2,0,func_001F9850_2E2EF0(7));
            moby->proximity=0;
        } else {
            if(moby->anim!=15) func_00213DE0_2E2EF0(moby,15,0,func_001F9850_2E2EF0(14));
            count=data->idle_timer;
            if(count>func_001F9850_2E2EF0(20)) smooth54(&data->idle_speed,D_0015ED6C*2.5f,D_0015ED70*7.0f);
            else smooth54(&data->idle_speed,0.0f,D_0015ED70*7.0f);
            moby->anim_rate=0.8f;
            func_L00_002626A8_2E2EF0((char *)moby,(char *)data->idle_pos,0.35f,0.25f);
        }
        break;
    }
    case 9:
        if(data->group_enabled) {
            if(func_001F9D48_2E2EF0(moby->pos,target->pos)<2.0f) break;
            if(data->group_check<D_L12_0015F5CC) {
                data->group_check=D_L12_0015F5CC; count=0;
                func_L00_0025A208_2E2EF0((int *)&iterator,moby->group,0,0);
                while(iterator) {
                    if((signed char)iterator->state>=0 && iterator->state!=9) count++;
                    func_L00_0025A2F0_2E2EF0((int *)&iterator,(int)iterator,0,0);
                }
                *data->group_count=count;
            }
            if(*data->group_count>=6) break;
        }
        fast_vec_sub(work.v0,target->pos,moby->pos);
        if(fast_vec_length(work.v0)<data->range && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<8.0f && func_002140B0_2E2EF0(19)==0) {
            if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
        } else if(!((moby->proximity&1)&&func_001F9938_2E2EF0(&data->group_timer)) && moby->proximity!=7) break;
        moby->state=13;
        if(moby->anim!=7) func_00213DE0_2E2EF0(moby,7,0,1);
        moby->flags|=0x1000;
        *(u128 *)work.v10=*(u128 *)moby->pos;
        func_L12_002E2CB8_2E2EF0(work.v10);
        break;
    case 13: {
        int collision=*(int *)(moby->model+0x10);
        moby->flags&=0xFFBE; moby->collision=collision;
        angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.03f,0.3f,D_0015ED6C*6.2831855f);
        if(moby->anim_flags&2) {
            if(func_002140B0_2E2EF0(256)&1) data->circle_rate=-data->circle_rate;
            data->circle_angle=data->circle_rate*0.017453292f;
            data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f));
            moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); moby->proximity=0;
        }
        break;
    }
    case 11:
        angle=func_L00_001FF860_2E2EF0(data->home[0]-moby->pos[0],data->home[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.05f,0.3f,0.2f);
        fast_vec_sub(work.v0,target->pos,data->home);
        if((fast_vec_length(work.v0)<data->range&&func_001F9B88_2E2EF0(data->home[2]-target->pos[2])<8.0f)||moby->proximity==2) {
            if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,2);
            moby->joints=0x40; data->zero=0.0f;
            data->gravity=D_L12_001618DC*D_0015ED70; data->launch_speed=D_L12_001618D8*D_0015ED6C;
            data->launch_time=func_L00_0025BC48_2E2EF0(moby->pos,data->home,&trajectory,data->launch_speed,-D_L12_001618DC);
            data->fall_mode=5; data->falling=0;
            setup_fall54(moby->rot[2],(char *)moby,data->fall,8,1,0);
            data->fall_a=5.0f; data->fall_b=10.0f; moby->state=12; moby->flags|=0x1000;
            moby->collision=*(int *)(moby->model+0x10);
        }
        break;
    case 12:
        if(func_L00_0025D6F0_2E2EF0(moby,data->fall)&1) {
            moby->state=14;
            if(moby->anim!=9) func_00213DE0_2E2EF0(moby,9,8,func_001F9850_2E2EF0(7));
            return;
        }
        if(moby->pos[2]<0.0f) { func_0020D678_2E2EF0(moby); return; }
        break;
    case 14:
        if(moby->anim_flags&2) {
            moby->state=4; data->blocked=0;
            if(func_002140B0_2E2EF0(256)&1) data->circle_rate=-data->circle_rate;
            data->circle_angle=data->circle_rate*0.017453292f;
            data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f));
            func_L12_002E2BF8_2E2EF0((unsigned char *)moby);
        }
        break;
    case 3:
        angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.03f,0.3f,D_0015ED6C*6.2831855f);
        if(moby->anim_flags&2) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); }
        break;
    case 4: {
        float chase_angle, look_difference, clamp;
        chase_angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        look_difference=func_001FA790_2E2EF0(chase_angle,moby->rot[2]);
        clamp=0.6981317f;
        if(look_difference>clamp || (clamp=-0.6981317f,look_difference<clamp)) look_difference=clamp;
        data->look_angle=look_difference;
        if(func_L12_002E41C8_2E2EF0((char *)moby,func_001FA748_2E2EF0(chase_angle,data->circle_angle))==2) { moby->state=6; data->return_timer=0; }
        else {
            if(func_001F9908_2E2EF0(&data->circle_timer)) {
                data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f)); data->circle_angle=-data->circle_angle;
            }
            fast_vec_sub(work.v0,target->pos,data->home);
            if(func_001F9D48_2E2EF0(moby->pos,target->pos)<1.5f) {
                moby->state=5;
                if(moby->anim!=4) func_00213DE0_2E2EF0(moby,4,0,func_001F9850_2E2EF0(5));
            } else if(fast_vec_length(work.v0)<data->range+5.5f && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<4.4f) {
                if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
            } else if(moby->proximity!=1 && func_002140B0_2E2EF0(9)==0) { moby->state=6; data->return_timer=0; }
        }
        moby->proximity=0;
        break;
    }
    case 5:
        (*(TrollMoby54_2E2EF0 **)D_0013E633_2E2EF0)->anim_rate=1.3f;
        angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.05f,0.3f,0.2f);
        if(moby->anim_end==moby->anim && func_00215B18_2E2EF0((char *)moby,13.0f) && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<0.5f && func_001F9D48_2E2EF0(moby->pos,target->pos)<2.0f && func_001FA850_2E2EF0(func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]),moby->rot[2])<0.2617994f) {
            work.v0[0]=func_001F9F90_2E2EF0(moby->rot[2])*0.2f; work.v0[1]=func_001F9FA8_2E2EF0(moby->rot[2])*0.2f; work.v0[2]=0.0f;
            qcopy(work.v10,target->pos); work.v10[2]+=0.75f;
            func_L00_0025AC00_2E2EF0(target,(int)moby,1,work.v10,work.v0,1.0f);
        }
        if((moby->anim_flags&2)&&func_001F9D48_2E2EF0(moby->pos,target->pos)>=2.0f) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); }
        break;
    case 6: {
        float return_distance;
        func_L12_002E41C8_2E2EF0((char *)moby,func_L00_001FF860_2E2EF0(data->home[0]-moby->pos[0],data->home[1]-moby->pos[1]));
        if(func_001F9D48_2E2EF0(moby->pos,data->home)<2.0f) {
            moby->state=1;
            if(moby->anim!=0) func_00213DE0_2E2EF0(moby,0,0,func_001F9850_2E2EF0(7));
        }
        return_distance=func_001F9D10_2E2EF0(target->pos,data->home); data->return_timer++; blocked=0;
        if(data->path!=-1) {
            char *path=D_L12_001B0C30_2E2EF0[data->path];
            blocked=func_L00_0025A778_2E2EF0(D_0013F3D0_r,path+0x10,*(int *)path)==0;
        }
        if(!blocked) {
            if(return_distance<data->range) {
                if(data->return_timer>func_001F9850_2E2EF0(70)) {
                    if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
                    moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby);
                }
            } else if(moby->proximity==1) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); }
        }
        moby->proximity=0;
        break;
    }
    case 7:
        if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
        if(func_L00_0025D6F0_2E2EF0(moby,data->fall)&1) {
            moby->state=10;
            if(moby->anim!=13) func_00213DE0_2E2EF0(moby,13,8,func_001F9850_2E2EF0(8));
            return;
        }
        if(moby->pos[2]<27.0f) { func_0020D678_2E2EF0(moby); return; }
        break;
    case 8:
        if(func_L00_002DDEA0_2E2EF0(moby,data->fall)) { moby->state=6; data->return_timer=0; data->stuck=0; }
        break;
    case 10:
        if(height54(moby)>29.0f) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); return; }
        break;
    case 99:
        hit=func_L00_0025D6F0_2E2EF0(moby,data->fall);
        if((hit&0x40)||(height54(moby)>37.0f && moby->anim_end==moby->anim)) {
            moby->anim_rate=0.0f; func_L00_002584A8_2E2EF0(moby,0,-1); moby->state=15; moby->flags|=8;
        } else if(moby->pos[2]<10.0f) { func_L00_002584A8_2E2EF0(moby,0,-1); { func_0020D678_2E2EF0(moby); return; } }
        else if(hit&1) func_L10_002E51A8_2E2EF0((char *)moby);
        break;
    case 15:
        func_L10_002E51A8_2E2EF0((char *)moby); func_L12_002E43A8_2E2EF0(moby); func_L12_002E43A8_2E2EF0(moby); { func_0020D678_2E2EF0(moby); return; }
    }
    if(moby->enabled && moby->state!=9 && moby->state!=8) {
        func_L00_00263950_2E2EF0((char *)moby,data->collision_data,0,D_0015ED64*0.018f,D_0015ED64*0.3f);
        if(moby->enabled && func_001F9D10_2E2EF0(moby->pos,D_L12_00167240)<28.0f) { func_L00_0025B178_2E2EF0(moby); moby->active=22; }
    }
    return;
}
/* Moves the moby along its path at the given speed and keeps it on the path; returns the move result, or 2 when it has been stuck too long. */
#include "qcopy.h"
extern char *D_L12_001B0930_w[] __asm__("D_L12_001B0930");
extern float D_0015ED6C_w __asm__("D_0015ED6C");
extern void FUN_L12_002e1878_w(char *) __asm__("FUN_L12_002e1878");
extern void approach_w(char *, float *, float, float, float, float) __asm__("FUN_L00_00258278");
extern float cos_w(float) __asm__("FUN_001f9dc8");
extern float sin_w(float) __asm__("FUN_001f9de0");
extern int move_w(void *, void *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern void sub_w(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_w(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_w(void *, void *, void *) __asm__("FUN_001f9a10");
extern int path_w(int, void *, void *, void *) __asm__("FUN_L00_00261b48");
extern int inside_w(void *, void *, int) __asm__("FUN_L00_00259740");
extern float dist_w(void *, void *) __asm__("FUN_001f9b48");
extern int frames_w(int) __asm__("FUN_001f96f8");

int FUN_L12_002e2eb8(struct Moby *m, float speed) {
    char *d = (char *)m->pvars;
    float old[4] __attribute__((aligned(16)));
    float dir[4];
    float hit[4];
    float res[4] __attribute__((aligned(16)));
    float t[4];
    float u[4];
    int r;
    *(OvlQuad *)old = *(OvlQuad *)&m->pos;
    FUN_L12_002e1878_w(m);
    approach_w(m, (float *)(d + 0x2A0), speed, 0.05f, 0.3f, 0.2f);
    dir[0] = cos_w(m->rot.z) * 2.0f;
    dir[1] = sin_w(m->rot.z) * 2.0f;
    dir[2] = 0.0f;
    r = move_w(m, d + 0x180, dir, hit, 1.0f);
    if (*(int *)(d + 0x2D0) != -1) {
        sub_w(u, old, &m->pos);
        norm_w(u, u, 0.05f);
        add_w(t, old, u);
        if (path_w(*(int *)(d + 0x2D0), &m->pos, t, res) != 0) {
            char *p = D_L12_001B0930_w[*(int *)(d + 0x2D0)];
            if (inside_w(&m->pos, p + 0x10, *(int *)p) == 0)
                *(OvlQuad *)&m->pos = *(OvlQuad *)res;
            if (dist_w(&m->pos, old) < D_0015ED6C_w * 0.5f) {
                *(int *)(d + 0x2D4) += 1;
                if (frames_w(10) < *(int *)(d + 0x2D4))
                    r = 2;
            } else {
                *(int *)(d + 0x2D4) = 0;
            }
        }
    }
    return r;
}
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/state/item_state.h"

extern int D_L12_0016183C __attribute__((sda));
extern int D_L12_00161840 __attribute__((sda));
extern float D_L12_00161844 __attribute__((sda));
extern float D_L12_00161848 __attribute__((sda));
extern float D_L12_0016184C __attribute__((sda));
extern float D_L12_00161850 __attribute__((sda));
extern float D_L12_00161854 __attribute__((sda));
extern float D_L12_00161858 __attribute__((sda));
extern float D_L12_0016185C __attribute__((sda));
extern float D_L12_00161860 __attribute__((sda));
extern float D_L12_00161864 __attribute__((sda));
extern float D_L12_00161868 __attribute__((sda));
extern int D_L12_0016186C __attribute__((sda));
extern int D_L12_00161870 __attribute__((sda));
extern int D_L12_00161874 __attribute__((sda));
extern int D_L12_00161878 __attribute__((sda));
extern int D_L12_0016187C __attribute__((sda));
extern int D_L12_00161880 __attribute__((sda));
extern int D_L12_00161884 __attribute__((sda));
extern void FUN_L00_0025f8e0(void *, float);
extern float random_float_between_e3098(float, float) __asm__("FUN_002132a8");
extern float fast_cos_e3098(float) __asm__("FUN_001f9dc8");
extern float fast_sin_e3098(float) __asm__("FUN_001f9de0");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz_e3098(void *, void *, void *) __asm__("FUN_001f9a10");
extern int scale_game_frames_e3098(int) __asm__("FUN_001f96f8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int truncate_float(float) __asm__("FUN_001fa6d0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern char *spawn_particle(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void FUN_L12_002e3098(struct Moby *moby) {
    float pos[4];
    float vel[4];
    float accel[4];
    float push[4];
    char *vars;
    int count;
    float *pos_ptr;
    void *src;
    int i;
    int j;
    float angle;
    float speed;
    float rise;
    float size;
    float time_base;
    int start_color;
    int end_color;
    int life;
    int fade;

    count = D_L12_0016183C;
    vars = (char *)moby->pvars;
    if (moby->state == 15)
        count = truncate_float((float)count * 1.5f);
    for (i = 0; i < count; i++) {
        pos_ptr = pos;
        src = &moby->pos;
        qcopy_nc(pos, src);
        FUN_L00_0025f8e0(pos_ptr, D_L12_00161844);
        pos[2] += 0.55f;
        angle = random_angle_radians();
        speed = random_float_between_e3098(D_L12_00161850, D_L12_00161854);
        rise = random_float_between_e3098(D_L12_00161858, D_L12_0016185C);
        if (moby->state == 15) {
            speed *= 0.55f;
            rise *= 0.55f;
            pos[2] -= 0.2f;
        }
        vel[0] = fast_cos_e3098(angle) * (speed * D_0015ED6C);
        vel[1] = fast_sin_e3098(angle) * (speed * D_0015ED6C);
        vel[2] = 0.0f + rise * D_0015ED6C;
        scale_vector_xyz(push, vars + 0x120, 0.5f);
        push[2] = 0.0f;
        add_vector_xyz_e3098(vel, vel, push);
        qcopy(accel, vel);
        accel[2] -= D_L12_00161860 * D_0015ED70 * (float)scale_game_frames_e3098(D_L12_00161880);
        for (j = 0; j < D_L12_00161840; j++) {
            size = random_float_between_e3098(0.5f, 1.5f);
            vel[3] = size * D_L12_00161864;
            accel[3] = size * D_L12_00161868;
            FUN_L00_0025f8e0(pos, D_L12_00161848);
            FUN_L00_0025f8e0(accel, D_L12_0016184C * D_0015ED6C);
            start_color = fast_tween_color(D_L12_0016186C, D_L12_00161874, random_float_between_e3098(0.0f, 1.0f));
            end_color = fast_tween_color(D_L12_00161870, D_L12_00161878, random_float_between_e3098(0.0f, 1.0f));
            life = scale_game_frames_e3098(D_L12_0016187C);
            fade = scale_game_frames_e3098(D_L12_00161880);
            time_base = (float)D_L12_00161884;
            spawn_particle(pos, vel, accel, start_color, end_color, life, fade,
                truncate_float(scale_time(random_float_between_e3098(time_base * 0.5f, time_base * 2.5f))), -1);
        }
    }
}
#include "qcopy.h"
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/vector.h"

/* Variables of the l12 troll moby (moby->pvars); the layout of FUN_L00_002e1678's TrollVars moved by 0xB0. */
typedef struct {
    u8 pad0[0x20];
    f32 health;          /* 0x20 */
    u8 pad24[2];
    s16 stun_timer;      /* 0x26 */
    u8 pad28[0x10];
    s32 reset_alert;     /* 0x38 */
    u8 pad3C[0xD4];
    u8 anim[7];          /* 0x110: animation blend state */
    u8 unk117;           /* 0x117 */
    u8 pad118[8];
    u8 fall[0x10];       /* 0x120 */
    f32 unk130;          /* 0x130 */
    f32 unk134;          /* 0x134 */
    f32 unk138;          /* 0x138 */
    f32 unk13C;          /* 0x13C */
    u8 pad140[4];
    s32 unk144;          /* 0x144 */
    u8 pad148[8];
    f32 unk150;          /* 0x150 */
    f32 unk154;          /* 0x154 */
    f32 unk158;          /* 0x158 */
    u8 pad15C;
    u8 unk15D;           /* 0x15D */
    u8 pad15E[0x12];
    f32 unk170;          /* 0x170 */
    f32 unk174;          /* 0x174 */
    u8 pad178[0x134];
    f32 speed;           /* 0x2AC */
    u8 pad2B0[0x14];
    s32 alert_timer;     /* 0x2C4 */
    u8 pad2C8[0x20];
    s16 call_enabled;    /* 0x2E8 */
    s16 call_timer;      /* 0x2EA */
} TrollVars_e3528;

typedef struct {
    u8 pad0[0x10];
    Vec4 pos;
} TrollHit_e3528;

extern f32 D_0015ED6C_e3528 __asm__("D_0015ED6C");
extern s32 D_001413D0_e3528 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 scale_game_frames_e3528(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown_e3528(s32 *) __asm__("FUN_001f9740");
extern s32 random_integer_below_e3528(s32) __asm__("FUN_00213260");
extern f32 fast_add_rotations_e3528(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_cos_e3528(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_e3528(f32) __asm__("FUN_001f9de0");
extern s32 count_group_e3528(s32, s32) __asm__("FUN_L01_0026e008");
extern void alert_group_e3528(s32, s32) __asm__("FUN_L01_0026e090");
extern void call_for_help_e3528(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern TrollHit_e3528 *find_hit_e3528(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void take_damage_e3528(struct Moby *, TrollHit_e3528 *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void knockback_e3528(void *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void start_fall_e3528(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void anim_reset_e3528(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_e3528(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void troll_alert_e3528(struct Moby *) __asm__("FUN_L12_002e3098");

extern f32 D_L12_001618D0 __attribute__((sda));
extern f32 D_L12_001618D4 __attribute__((sda));
/* Damage and alert step of the l12 troll: refreshes its speed while alerted, now and then calls for help behind
   itself, and takes hits (dying, or staggering back). */
void FUN_L12_002e3528(struct Moby *moby) {
    TrollVars_e3528 *vars = (TrollVars_e3528 *)moby->pvars;
    Vec4 v;
    s32 hit;
    f32 dmg;
    f32 angle;
    f32 unused;
    TrollHit_e3528 *coll;

    if (vars->reset_alert != 0) {
        vars->reset_alert = 0;
        vars->alert_timer = scale_game_frames_e3528(0xF0);
    }
    if (moby->state != 9 && moby->state != 0xB && moby->state != 0) {
        if (tick_countdown_e3528(&vars->alert_timer)) {
            vars->speed = 12.0f;
        } else {
            vars->speed = 20.0f;
        }
    }
    if (vars->call_enabled != 0 && moby->state != 9) {
        s16 n = ++vars->call_timer;
        if (scale_game_frames_e3528(0x3C) * 15 < n && moby->visible == 0 && moby->group < 0xFF &&
            count_group_e3528(moby->group, -1) >= 4 && random_integer_below_e3528(0x45) == 0) {
            v.f[0] = fast_cos_e3528(fast_add_rotations_e3528(moby->rot.z, 3.1415927f));
            v.f[1] = fast_sin_e3528(fast_add_rotations_e3528(moby->rot.z, 3.1415927f));
            v.f[2] = 0.0f;
            call_for_help_e3528(moby, D_001413D0_e3528, 0x10000, &moby->pos, v.f, 1.0f);
        }
    }
    coll = find_hit_e3528(moby, 0x330000, 0);
    dmg = 0.0f;
    take_damage_e3528(moby, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (coll != 0 && moby->state != 0x63 && moby->state != 8) {
        if (moby->group != 0xFF) {
            alert_group_e3528(moby->group, 7);
        }
        if (dmg != 0.0f) {
            vars->health -= dmg;
            vars->unk130 = 0.008f;
            vars->unk134 = 0.0005f;
            vars->unk138 = D_L12_001618D4 * D_0015ED6C_e3528;
            vars->unk13C = D_L12_001618D0 * D_0015ED6C_e3528;
            vars->unk15D = 0;
            vars->unk144 = 9;
            vars->unk150 = 1.0f;
            vars->unk154 = 1.0f;
            vars->unk158 = 1.0f;
            if (vars->health <= 0.0f) {
                moby->flags &= ~0x1000;
                vars->unk138 = D_0015ED6C_e3528 * 8.0f;
                vars->unk13C = D_0015ED6C_e3528 * 10.0f;
                v.q = coll->pos.q;
                knockback_e3528(&v, &angle, &vars->unk138, &vars->unk13C);
                start_fall_e3528(moby, vars->fall, 6, 1, 0, angle);
                vars->unk170 = 11.0f;
                vars->unk174 = 18.0f;
                moby->state = 0x63;
                vars->unk117 = 0x78;
                anim_reset_e3528(moby, vars->anim);
            } else {
                v.q = coll->pos.q;
                knockback_e3528(&v, &unused, &vars->unk138, &vars->unk13C);
                start_fall_e3528(moby, vars->fall, 0xE, 1, 0, unused);
                vars->unk174 = vars->unk170 = -1.0f;
                moby->state = 7;
                vars->unk117 = 0xFA;
                vars->stun_timer = scale_game_frames_e3528(0x3C);
                anim_reset_e3528(moby, vars->anim);
            }
            troll_alert_e3528(moby);
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_e3528(moby, vars->anim);
}
/* Hoven engine exhaust: emits a flame and a smoke puff from each of the moby's two nozzles (the second one
 * mirrored across its local Y axis), with random jitter. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E4C58), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
int FUN_L00_00257b90(int lo, int hi);
unsigned char *FUN_L00_0026d000(void *pos, int n, void *vel, int color, float r, float lo, float hi, float size);
extern void *func_L00_0026DEA0_x(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

float D_L12_00161920[4] __attribute__((section(".sdata"))) = {-1.5f, 0.8f, -0.5f, 0.0f};
void FUN_L12_002e3948(struct Moby *moby) {
    float a[4];
    float b[4];
    float p1[4];
    float p2[4];
    float z[4];
    if (moby->visible == 0) return;
    clear_u64_value_c(z);
    a[0] = -random_float_between(0.0f, frame_time * 5.0f);
    a[1] = random_float_between(-(frame_time * 2.0f), frame_time * 2.0f);
    a[2] = random_float_between(-(frame_time * 2.0f), frame_time * 2.0f);
    transform_vector_by_basis(a, a, &moby->rot_row0);
    transform_vector_by_basis(p1, &D_L12_00161920, &moby->rot_row0);
    add_vector_xyz_c(p1, p1, &moby->pos);
    b[0] = -random_float_between(0.0f, frame_time);
    b[1] = random_float_between(-frame_time, frame_time);
    b[2] = random_float_between(-frame_time, frame_time);
    transform_vector_by_basis(b, b, &moby->rot_row0);
    qcopy(p2, &D_L12_00161920);
    p2[1] = -p2[1];
    transform_vector_by_basis(p2, p2, &moby->rot_row0);
    add_vector_xyz_c(p2, p2, &moby->pos);
    FUN_L00_0026cbb0(p1, a, 0x4F007FFF, 0x1FFFFFFF, FUN_L00_00257b90(scale_game_frames(8), scale_game_frames(0x11)), 1, 10000.0f);
    func_L00_0026DEA0_x(p1, 0.05f, 1.01f, 1.03f, 6, b, 30000.0f, 0x404040);
    FUN_L00_0026cbb0(p2, a, 0x4F007FFF, 0x1FFFFFFF, FUN_L00_00257b90(scale_game_frames(8), scale_game_frames(0x11)), 1, 10000.0f);
    func_L00_0026DEA0_x(p2, 0.05f, 1.01f, 1.03f, 6, b, 30000.0f, 0x404040);
}
/* Steers the moby toward a target point, speeding up or braking, and turns it toward the given angles. */
extern float D_0015ED6C_k __asm__("D_0015ED6C");
extern float D_0015ED70_k __asm__("D_0015ED70");
extern float D_0015ED64_k __asm__("D_0015ED64");
extern void sub_k(void *, void *, void *) __asm__("FUN_001f9a28");
extern float len_k(void *) __asm__("FUN_001f9af0");
extern void norm_k(void *, void *, float) __asm__("FUN_001f9bf8");
extern float diff_k(float, float) __asm__("FUN_001fa688");
extern void appr_k(void *, float, float) __asm__("FUN_00213ed8");
extern float turn_k(float *, float, float *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern void add_k(void *, void *, void *) __asm__("FUN_001f9a10");

extern float D_L12_00161918 __attribute__((sda));
void FUN_L12_002e3c08(char *m, float *ang, void *target, int unused, int brake) {
    char *d = *(char **)(m + 0x78);
    float v[4];
    float dist;
    float speed;
    float acc;
    float lim = 1.9198622f;
    sub_k(v, target, m + 0x10);
    dist = len_k(v);
    speed = len_k(d + 0x40);
    if (brake) {
        if (speed == 0.0f)
            speed = D_0015ED6C_k * 0.01f;
        acc = *(float *)(d + 0xD4) * D_0015ED70_k;
        if (acc < speed * speed / (dist + dist))
            speed -= acc;
        else if (speed < *(float *)(d + 0xD0) * D_0015ED6C_k)
            speed += acc;
    } else {
        if (speed < *(float *)(d + 0xD0) * D_0015ED6C_k)
            speed += *(float *)(d + 0xD4) * D_0015ED70_k;
        else if (*(float *)(d + 0xD0) * D_0015ED6C_k < speed)
            speed -= *(float *)(d + 0xD4) * D_0015ED70_k;
    }
    norm_k(d + 0x40, v, speed);
    appr_k(m + 0x40, ang[0], diff_k(ang[0], *(float *)(m + 0x40)) / D_L12_00161918);
    appr_k(m + 0x44, ang[1], diff_k(ang[1], *(float *)(m + 0x44)) / D_L12_00161918);
    turn_k((float *)(m + 0x48), ang[2], (float *)(d + 0xF8), 0, D_0015ED64_k * 0.005f, D_0015ED64_k * 0.3f,
           D_0015ED6C_k * lim);
    add_k(m + 0x10, m + 0x10, d + 0x40);
}
/* Projects a moby's offset from a reference point into a 2D pair, or zeroes it. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E5138), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void FUN_001f9a28(void *, void *, void *);
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");

extern short D_L12_00161914 __attribute__((sda));
void FUN_L12_002e3e28(struct Moby *moby, float *out, int on) {
    float m[4];
    float a[4];
    float b[4];
    float pad[12];
    float g;
    if (on) {
        FUN_001f9a28(a, D_0013E533 + 0xE9D, &moby->pos);
        clear_u64_value(m);
        m[2] = -moby->rot.z;
        euler_rotation_matrix(b, m);
        transform_vector(a, a, b);
        g = *(float *)&D_L12_00161914;
        out[1] = a[0] / g;
        out[0] = -a[1] / g;
    } else {
        out[0] = 0;
        out[1] = 0;
    }
}
/* Update of the Hoven flying platform (class 240): follows or eases along its path, bobs, falls, and reports its motion. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E51E8), where it is exact; names translated to the US level program. */


typedef struct { float x, y, z, w; } Node_2E51E8;

typedef struct {
    int n;
    int pad4[3];
    Node_2E51E8 nodes[1];
} Path_2E51E8;

typedef struct {
    char pad0[0x10];
    int f10;
} Class_2E51E8;

typedef struct {
    int idx;
    signed char dir;
    char pad5[0xB];
    Path_2E51E8 *path;
    int pathIdx;
} Walk_2E51E8;

typedef struct {
    char pad0[0x3E];
    unsigned short f3E;
    float vel[4];
    char pad50[0x10];
    char f60[0x40];
    Walk_2E51E8 walk;
    char padB8[0x18];
    float fD0;
    float fD4;
    float fD8;
    int fDC;
    int timer;
    int kind;
    float fE8;
    float fEC;
    int fF0;
    char padF4[8];
    int fFC;
} Data_2E51E8;

extern Moby *D_L12_0015FFD8;
extern char *D_L12_001B0930[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 vector_length_xyz_c2(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float FUN_001f9b80(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int FUN_L01_00277fb8(void *);
extern int FUN_L08_002025d0(void *, int, int, void *, int);
extern s32 FUN_L01_0026e008(s32, s32);
extern s32 tick_countdown_32_alt_c(s32 *) __asm__("FUN_001f9740");
extern void FUN_L00_0025b6b8(void *, void *, void *, float);
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00260738(char *, void *, void *, void *);
extern void add_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value_c2(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L12_002e3948_c(struct Moby *moby) __asm__("FUN_L12_002e3948");
void FUN_L12_002e3c08_c(char *m, float *ang, void *target, int unused, int brake) __asm__("FUN_L12_002e3c08");
void FUN_L12_002e3e28_c(struct Moby *moby, float *out, int on) __asm__("FUN_L12_002e3e28");
extern Path_2E51E8 *D_L12_001B0C30_2E51E8[] __asm__("D_L12_001B0930");
extern void func_L12_002E4F18_2E51E8(void *, void *, void *, int, int) __asm__("FUN_L12_002e3c08");

void FUN_L12_002e3ed8(Moby *moby) {
    float oldPos[4];
    float oldRot[4];
    float delta[4];
    float angles[4];
    float goal[4];
    float to_goal[4];
    Data_2E51E8 *data;

    data = (*(Data_2E51E8 **)&moby->pvars);
    qcopy(oldPos, (*(float (*)[4])&moby->pos));
    qcopy(oldRot, (*(float (*)[4])&moby->rot));
    if (moby->state != 0 && data->fFC != -1 && (signed char)D_L12_0015FFD8[data->fFC].state >= 0) {
        FUN_L12_002e3948_c((char *)moby);
        return;
    }
    if (moby->state != 0 && data->fDC != -1 && FUN_L01_0026e008(data->fDC, -1) != 0) {
        FUN_L12_002e3948_c((char *)moby);
        return;
    }
    switch (moby->state) {
    case 0:
        switch (data->kind) {
        case 0:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            data->walk.path = D_L12_001B0C30_2E51E8[data->walk.pathIdx];
            data->walk.idx = 0;
            data->walk.dir = 1;
            if (data->fD0 == 0.0f) data->fD0 = 6.0f;
            if (data->fD4 == 0.0f) data->fD4 = 6.0f;
            data->f3E |= 9;
            clear_u64_value_c2(data->vel);
            moby->state = 7;
            break;
        case 1:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            data->walk.path = D_L12_001B0C30_2E51E8[data->walk.pathIdx];
            data->walk.idx = 0;
            data->walk.dir = 1;
            if (data->fD4 == 0.0f) data->fD4 = 2.0f;
            if (data->fD0 == 0.0f) data->fD0 = 4.0f;
            data->fD8 = random_angle_radians_c();
            data->f3E |= 9;
            clear_u64_value_c2(data->vel);
            moby->state = 2;
            break;
        case 2:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            (*(float (*)[4])&moby->rot)[1] = -1.5707964f;
            (*(float (*)[4])&moby->rot)[2] = (*(float (*)[4])&moby->rot)[2] + 3.1415927f;
            data->walk.path = D_L12_001B0C30_2E51E8[data->walk.pathIdx];
            data->walk.idx = 0;
            data->walk.dir = 1;
            if (data->fD4 == 0.0f) data->fD4 = 8.0f;
            if (data->fD0 == 0.0f) data->fD0 = 8.0f;
            clear_u64_value_c2(data->vel);
            moby->state = 4;
            break;
        case 3:
            moby->draw_dist = 0;
            moby->state = 1;
            (*(int *)&moby->collision) = 0;
            break;
        case 4:
            moby->state = 9;
            data->fD4 = 4.0f;
            data->fD0 = 4.0f;
            data->f3E &= 0xFFFE;
            break;
        }
        break;
    case 7: {
        Walk_2E51E8 *w = &data->walk;
        Path_2E51E8 *path;
        int next;
        FUN_L12_002e3948_c((char *)moby);
        path = w->path;
        next = (w->idx + path->n + w->dir) % path->n;
        qcopy(goal, &path->nodes[next]);
        if (distance_xyz((*(float (*)[4])&moby->pos), &path->nodes[next]) < 0.5f) {
            data->walk.idx = next;
            if (data->walk.path->nodes[next].w != -1.0f) {
                data->fD0 = data->walk.path->nodes[next].w;
            }
        }
        subtract_vector_xyz_c2(to_goal, goal, (*(float (*)[4])&moby->pos));
        angles[2] = fast_arc_tan(to_goal[0], to_goal[1]);
        if (FUN_L01_00277fb8(moby)) {
            FUN_L12_002e3e28_c((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, goal, 1, 0);
        } else {
            FUN_L12_002e3e28_c((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, goal, 0, 0);
        }
        break;
    }
    case 2: {
        Path_2E51E8 *path = data->walk.path;
        float swing;
        data->fD4 = 0.0f;
        data->fD8 = fast_add_rotations_c2(data->fD8, D_0015ED6C * 1.2217305f);
        swing = fast_sin_c(data->fD8);
        if (data->fD8 > 1.5707964f && data->fD8 < 3.1415927f) swing = 2.0f - swing;
        if (data->fD8 > -3.1415927f && data->fD8 < -1.5707964f) swing = 2.0f - swing;
        swing = swing * 0.5f;
        swing = swing + 0.5f;
        if (swing > 1.0f) swing = swing - 2.0f;
        swing = absolute_float(swing);
        *(u128 *)angles = *(u128 *)&path->nodes[0];
        *(u128 *)goal = *(u128 *)&path->nodes[1];
        FUN_L00_0025b6b8((*(float (*)[4])&moby->pos), angles, goal, swing);
        angles[2] = (*(float (*)[4])&moby->rot)[2];
        if (FUN_L01_00277fb8(moby)) {
            FUN_L12_002e3e28_c((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 1, 1);
        } else {
            FUN_L12_002e3e28_c((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 0, 1);
        }
        break;
    }
    case 4: {
        Path_2E51E8 *path = data->walk.path;
        float swing;
        data->fD8 = fast_add_rotations_c2(data->fD8, D_0015ED6C * 1.2217305f);
        swing = fast_sin_c(data->fD8);
        if (data->fD8 > 1.5707964f && data->fD8 < 3.1415927f) swing = 2.0f - swing;
        if (data->fD8 > -3.1415927f && data->fD8 < -1.5707964f) swing = 2.0f - swing;
        swing = swing * 0.5f;
        swing = swing + 0.5f;
        if (swing > 1.0f) swing = swing - 2.0f;
        swing = absolute_float(swing);
        *(u128 *)angles = *(u128 *)&path->nodes[0];
        *(u128 *)goal = *(u128 *)&path->nodes[1];
        FUN_L00_0025b6b8((*(float (*)[4])&moby->pos), angles, goal, swing);
        (*(float (*)[4])&moby->rot)[1] = -1.5707964f;
        break;
    }
    case 3:
        angles[2] = (*(float (*)[4])&moby->rot)[2];
        if (tick_countdown_32_alt_c(&data->timer)) {
            if (data->walk.dir == 1) {
                data->walk.dir = 2;
            } else {
                data->walk.dir = 1;
            }
            moby->state = 2;
        }
        if (FUN_L01_00277fb8(moby)) {
            FUN_L12_002e3e28_c((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 1, 1);
        } else {
            FUN_L12_002e3e28_c((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 0, 1);
        }
        break;
    case 5:
        if (tick_countdown_32_alt_c(&data->timer)) {
            if (data->walk.dir == 1) {
                data->walk.dir = 2;
            } else {
                data->walk.dir = 1;
            }
            moby->state = 4;
        }
        if (data->walk.dir == 2) {
            FUN_L12_002e3948_c((char *)moby);
        }
        qcopy(angles, (*(float (*)[4])&moby->rot));
        func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 0, 1);
        break;
    case 6:
        if (vector_length_xyz_c2(data->vel) < D_0015ED6C * 6.0f) {
            data->vel[2] = data->vel[2] + D_0015ED70 * 6.0f;
        }
        add_vector_xyz_c3((*(float (*)[4])&moby->pos), (*(float (*)[4])&moby->pos), data->vel);
        if (moby->visible == 0) {
            mark_moby_for_removal_c(moby);
        }
        break;
    case 8:
        add_vector_xyz_c3((*(float (*)[4])&moby->pos), (*(float (*)[4])&moby->pos), data->vel);
        if (FUN_L08_002025d0((*(float (*)[4])&moby->pos), 0x600, 0, moby, 0) && data->walk.path == 0) {
            FUN_L00_0025e450_c(moby, data->vel, 0, 2.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c(moby);
            return;
        }
        data->vel[2] = data->vel[2] - D_0015ED70 * 7.0f;
        (*(float (*)[4])&moby->rot)[0] = fast_add_rotations_c2(data->fE8, (*(float (*)[4])&moby->rot)[0]);
        (*(float (*)[4])&moby->rot)[1] = fast_add_rotations_c2(data->fEC, (*(float (*)[4])&moby->rot)[1]);
        if (data->walk.path != 0 && FUN_001f9b80((*(float (*)[4])&moby->pos), &data->walk.path->nodes[0]) < 0.5f) {
            moby->state = 0;
        }
        if ((*(float (*)[4])&moby->pos)[2] < 0.0f) {
            mark_moby_for_removal_c(moby);
        }
        break;
    case 9: {
        Moby *o = (Moby *)((char *)D_L12_0015FFD8 + (data->fF0 << 8));
        qcopy(angles, (*(float (*)[4])&moby->pos));
        angles[2] = (*(float (*)[4])&o->pos)[2];
        clear_u64_value_c2(goal);
        goal[2] = (*(float (*)[4])&moby->rot)[2];
        if (FUN_L01_00277fb8(moby)) {
            angles[2] = angles[2] + 0.2f;
            FUN_L12_002e3e28_c((char *)moby, goal, 1);
            func_L12_002E4F18_2E51E8(moby, goal, angles, 1, 1);
        } else {
            angles[2] = angles[2] + 0.4f;
            FUN_L12_002e3e28_c((char *)moby, goal, 0);
            func_L12_002E4F18_2E51E8(moby, goal, angles, 0, 1);
        }
        break;
    }
    }
    subtract_vector_xyz_c2(delta, (*(float (*)[4])&moby->pos), oldPos);
    FUN_L00_00260738(data->f60, delta, oldRot, (*(float (*)[4])&moby->rot));
}
/* Level overlay code after gameplay/vendor; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

#include "qcopy.h"

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad00[0xC];
    float radius;
    char pad10[0x30];
} L16WatchActivation;

typedef struct {
    char pad00[0x20];
    L16WatchActivation activation;
    char body[0x60];
    float padC0;
    float pitch;
    float yaw;
    float padCC;
    float height;
    char padD4[0xC];
    char head[0x60];
    char pad140[8];
    float head_yaw;
    char pad14C[0x14];
    float target[4];
    int region;
    char pad174[4];
    int moving_timer;
    int target_timer;
} L16WatchData;

typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0xF];
    unsigned char opacity;
    char pad31[0x17];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x24];
    L16WatchData *data;
    char pad7C[0x34];
    unsigned char widget;
} L16WatchMoby;

typedef struct {
    char pad00[0x70];
    unsigned char entered;
    unsigned char mission;
} L16WatchFlags;

extern char D_0013E533[];
extern char D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9e90_cf(float, float) __asm__("FUN_001fa688");
extern float FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float);
extern float truncate_float_to_s32_cf(float, float) __asm__("FUN_002132a8");
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *);
extern void FUN_L01_002783a8_c(void *, float) __asm__("FUN_L01_002783a8");
extern unsigned char alternate_item_available[] __asm__("D_0013D388");
extern int D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern void FUN_0020c828_c(void *) __asm__("FUN_0020c828");
extern void FUN_L02_0025c758(void *);
extern char *func_0020D348_m_x(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8_x(void *) __asm__("FUN_L00_00250df8");
extern void FUN_L12_0027b9c0_x(void *, void *, int) __asm__("FUN_L12_0027b9c0");
extern void FUN_L00_00263d40_c(int, int) __asm__("FUN_L00_00263d40");
extern int FUN_L00_00266448(void *, void *);
extern int FUN_L00_002668a0(void *, void *);
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");
extern int random_float_between_cf(float) __asm__("FUN_001fa6d0");
extern int scale_game_frames(int);
extern int tick_countdown_32(int *);
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_002502a0(int);
extern void FUN_L00_00260860(int, int);
extern void FUN_L00_002628d8(float, float, void *, void *, int);
extern void FUN_L00_00284e50(void *, void *);
extern void FUN_L12_002e6b60(char *);
extern void add_vector_xyz(void *, void *, void *);
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void subtract_vector_xyz(void *, void *, void *);
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

extern char *D_L12_001600EC;
void FUN_L12_002e6c48(L16WatchMoby *m) {
    L16WatchScratch scratch;
    L16WatchData *d = m->data;
    float rate;
    float head_rate;
    int tracking;
    FUN_L12_002e6b60((char *)m);
    switch (m->state) {
    case 0:
        if (alternate_item_available[1] != 0) {
            FUN_0020c828_c(m);
            return;
        }
        m->state = 1;
        FUN_L00_002668a0(m, &d->activation);
        FUN_L02_0025c758(m);
        break;
    case 1:
        if (alternate_item_available[1] == 0 && FUN_L00_00266448(m, &d->activation)) {
            FUN_L01_002783a8_c(m, 2.5f);
            *(int *)((char *)m + 0x58) = 0;
            m->state = 2;
        }
        break;
    case 2:
        if (D_L12_0015F5C4 != 2) {
            *(float *)((char *)m + 0x58) = 1.0f;
            alternate_item_available[1] = 1;
            FUN_L00_00263d40_c(0x2EE7, scale_game_frames(300));
            alternate_item_available[0x61] = 1;
            FUN_L00_002502a0(m->widget);
            if (d->region != -1) {
                char *p = D_L12_001600EC + (d->region << 7);
                FUN_L00_00284e50(p + 0x30, p + 0x70);
            }
            memcard_save_data(0, -1);
            m->state = 1;
        }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (m->animation == 0) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (FUN_001f9b80(m->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                m->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    m->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    m->position[1])) < 1.5707964f) {
            if (vector_length_xyz(player + 0x80) > 0.01f)
                d->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32(&d->moving_timer);
        } else if (d->moving_timer) {
            d->moving_timer = 0;
            qcopy(d->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&d->target_timer)) {
            float heading;
            d->target_timer =
                random_float_between_cf(multiply_global_factor_ed64(truncate_float_to_s32_cf(180.0f, 300.0f)));
            heading =
                fast_add_rotations(m->yaw, truncate_float_to_s32_cf(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(d->target, 6.0f, heading,
                                   truncate_float_to_s32_cf(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(d->target, d->target, m->position);
        }
        if (d->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, d->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, m->position);
        scratch.eye[2] += 1.0f;
        subtract_vector_xyz(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), m->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        d->pitch = pitch;
        d->yaw = yaw * 0.3f;
        d->head_yaw = yaw * 0.7f;
    }
    if (D_0015EDB0_b)
        d->height = 2.75f;
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->body, 1);
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, (char *)d + 0xE0, 0);
    if (*(int *)((char *)d + 0x180) == 0) {
        char *o = func_0020D348_m_x(0x11F);
        *(char **)((char *)d + 0x180) = o;
        if (o) {
            *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)m + 0x32);
            (*(unsigned char **)((char *)d + 0x180))[0x31] = 1;
            {
                char *p = *(char **)((char *)d + 0x180);
                qcopy(p + 0x10, (char *)m + 0x10);
                qcopy(p + 0x40, (char *)m + 0x40);
            }
            FUN_L00_00250df8_x(m);
        }
    } else {
        FUN_L12_0027b9c0_x(m, *(void **)((char *)d + 0x180), 2);
    }
}
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8588), where it is exact; names translated to the US level program. */

extern char D_L12_00167240[];
extern void FUN_L12_002e71b0(void);

void FUN_L12_002e7208(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    switch (moby->state) {
    case 0:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case 1: {
        int value = *(int *)(data + 0xC);
        if (value == -1 || is_point_inside_clip_volume(D_L12_00167240, value)) {
            add_draw_callback((void *)FUN_L12_002e71b0, moby);
        }
    } break;
    }
}
/* Forward the vendor parameters with fixed range and scale values. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8610), where it is exact; names translated to the US level program. */

extern void FUN_L00_0025f3e8(void *, void *, int, float, float);

void FUN_L12_002e7290(void *a, void *b) {
    FUN_L00_0025f3e8(a, b, -1, 1.0f, 10.0f);
}
extern void FUN_L12_002e81e0(struct Moby *);
extern void FUN_L12_002e8608(struct Moby *, float *);
extern void FUN_L12_002e86d0(struct Moby *);
extern void FUN_L12_002e8720(struct Moby *);
extern void FUN_L12_002e87b0(char *);
extern void FUN_L12_002e8808(struct Moby *, float *, float, float);
extern s32 D_001413D0_e81e0 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 scale_game_frames_e81e0(s32) __asm__("FUN_001f96f8");
extern f32 fast_cos_e81e0(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_e81e0(f32) __asm__("FUN_001f9de0");
extern void call_for_help_e81e0(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int random_integer_below_e81e0(s32) __asm__("FUN_00213260");
extern float angle_difference_8808(float, float) __asm__("FUN_001fa688");
extern float fast_add_rotations_e81e0(float, float) __asm__("FUN_001fa580");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void subtract_8808(void *, void *, void *) __asm__("FUN_001f9a28");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern struct Moby *D_L12_00178380[];
extern int coll_line_fix_72(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int query_mobys_72(void *, int, void *, int, float) __asm__("FUN_L00_001f2868");
extern int FUN_L00_0025e3b8(void *);
extern float FUN_L00_00257c48(float, float);
extern void blend_moby_animation_ex(void *, int, int, int, int) __asm__("FUN_002130d8");
extern int FUN_L01_0028b510(void *, void *, float);
extern int FUN_L12_0027b8e0(void *, void *, float, float, float);
extern float vector_project_72(void *) __asm__("FUN_001f9b20");
extern void FUN_L04_002aee30(void *, void *, void *, int);

#define OGRE_I(offset) (*(int *)(data + (offset)))
#define OGRE_F(offset) (*(float *)(data + (offset)))
#define OGRE_B(offset) (*(u8 *)(data + (offset)))
#define OGRE_H(offset) (*(u16 *)(data + (offset)))
#define OGRE_CHILD (*(struct Moby **)(data + 0x248))

/* Ogre update: initialization, path following, aiming, attack and removal.
   The retail unsigned group sentinel is 0xff; unordered angle comparisons
   keep the original branch outcomes. Quad copies include the fourth word. */
void FUN_L12_002e72c0(struct Moby *moby) {
    TargetReport54_2E2EF0 work;
    Vec4 v;
    Vec4 delta;
    char *data = (char *)moby->pvars;
    struct Moby *target;

    func_L00_00264BB0_2E2EF0(data + 0x120, 3.0f);
    if (D_L12_0015F5C4 == 2) goto hide;
    if (moby->state >= 3) {
        moby->flags &= 0xffbe;
        if (OGRE_CHILD) OGRE_CHILD->flags &= 0xffbe;
    }
    if (OGRE_B(0x269) && hero.state.current == 0 && hero.state.prev == 0x67) {
        if (FUN_001f9b80(&moby->pos, &hero.motion.pos) < 30.0f) goto leave;
    }
    if (OGRE_I(0x270) != -1) {
        if (!is_point_inside_clip_volume(D_L12_00167240, OGRE_I(0x270))) {
hide:
            moby->flags |= 0x41;
            if (OGRE_CHILD) OGRE_CHILD->flags |= 0x41;
            return;
        }
        moby->flags &= 0xffbe;
        if (OGRE_CHILD) OGRE_CHILD->flags &= 0xffbe;
    }
    if (OGRE_B(0x2e) == 2) goto leave;
    FUN_L12_002e81e0(moby);
    select54((char *)moby, (char *)&work, OGRE_F(0x230));
    target = (struct Moby *)work.moby;
    if (!target) {
        target = hero.moby;
        qcopy(work.v20, hero.motion.unkD0.f);
    }
    switch (moby->state) {
    case 0: {
        float value;
        float other = OGRE_F(0x230);
        OGRE_F(0x20) = 2.0f;
        OGRE_H(0x24) = 2;
        OGRE_B(0x29) = 1;
        OGRE_B(0x2e) = 1;
        OGRE_B(0x58) = 8;
        OGRE_B(0x5a) = 8;
        OGRE_F(0x28c) = other;
        OGRE_I(0x90) = 0x200;
        OGRE_F(0x98) = 0.5f;
        func_L01_0026E8E0_2E2EF0(data + 0xd0);
        value = D_0015ED6C * 5.5f;
        OGRE_I(0xd0) = 0x38d;
        *(volatile float *)(data + 0xd8) = 1.0f;
        OGRE_I(0x108) = 0x18;
        *(volatile float *)(data + 0xf4) = value;
        *(volatile float *)(data + 0xdc) = 1.0f;
        qcopy(data + 0x220, &moby->pos);
        if (moby->group != 0xff) {
            if (**(int **)(data + 0x24c) == 1) moby->flags |= 0x8000;
            **(int **)(data + 0x24c) = (**(int **)(data + 0x24c) == 0);
        } else if (func_001160D8_2E2EF0() & 1) {
            moby->flags |= 0x8000;
        }
        if (OGRE_I(0x274)) OGRE_I(0x288) = scale_game_frames_e81e0(FUN_L00_00257b90(0, 300));
        moby->rot.x = 0.0f;
        moby->rot.y = 0.0f;
        if (OGRE_F(0x244) == moby->rot.x) {
            if (moby->group != 0xff) {
                int *path = *(int **)(data + 0x250);
                int count;
                OGRE_F(0x244) = (float)*path;
                count = *path;
                if (count > 0) *path = -count;
                else *path = count + 20;
            } else OGRE_F(0x244) = FUN_L00_00257c48(15.0f, 35.0f);
        }
        OGRE_F(0x244) *= 0.017453292f;
        moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
        if (OGRE_B(0x268)) {
            moby->flags |= 0x41;
            if (OGRE_CHILD) OGRE_CHILD->flags |= 0x41;
        }
        moby->state = 2;
        break;
    }
    case 2: {
        if (distance_xyz(&moby->pos, &hero.motion.pos) > 64.0f) return;
        if (moby->prev_seq != 0 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 0, 0, scale_game_frames_e81e0(8), 5);
        if (!FUN_L12_0027b8e0(moby, &target->pos, OGRE_F(0x230), 3.0f, 0.0f)) {
            if ((OGRE_I(0x25c) == -1 || !is_point_inside_clip_volume(&target->pos, OGRE_I(0x25c))) && moby->command == 0) break;
        }
        if (moby->flags & 1) {
            qcopy(v.f, &moby->pos);
            v.f[2] += 0.5f;
            if (coll_line_fix_72(&v, D_L12_00167240, 2, moby, 0)) {
                moby->flags &= 0xffbe;
                if (OGRE_CHILD) OGRE_CHILD->flags &= 0xffbe;
            } else {
                if (OGRE_CHILD) mark_moby_for_removal(OGRE_CHILD);
                mark_moby_for_removal(moby);
                return;
            }
        }
        if (moby->group != 0xff) FUN_L01_0026e090(moby->group, 1);
        if (OGRE_I(0x254) == -1) goto state3;
        else {
            OGRE_I(0x260) = FUN_L01_0028b510(&moby->pos, D_L12_001B0930_w[OGRE_I(0x254)], 0.0f);
            moby->state = 1;
        }
        break;
    }
    case 1: {
        int *path;
        if (!tick_countdown_32_alt_c2((int *)(data + 0x258))) break;
        if (moby->prev_seq != 8 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 8, 0, scale_game_frames_e81e0(11), 5);
        path = (int *)D_L12_001B0930_w[OGRE_I(0x254)];
        if (FUN_001f9b80(&moby->pos, path + (OGRE_I(0x260) * 4 + 4)) < 3.0f) ++OGRE_I(0x260);
        if (*path - 1 < OGRE_I(0x260)) OGRE_I(0x260) = *path - 1;
        FUN_L12_002e8808(moby, (float *)(path + (OGRE_I(0x260) * 4 + 4)), 0.0f, D_0015ED6C * 8.0f);
        if (OGRE_I(0x260) == *path - 1) goto state3;
        else if (FUN_001f9b80(&moby->pos, &target->pos) < OGRE_F(0x264)) moby->state = 3;
        break;
    }
    case 3: {
        float angle;
        if (OGRE_B(0x268) == 1) {
            int hit;
            qcopy(v.f, &moby->pos);
            v.f[0] += fast_cos_e81e0(moby->rot.z);
            v.f[1] += fast_sin_e81e0(moby->rot.z);
            hit = query_mobys_72(&v, 0, moby, 0, 1.5f);
            if (hit) {
                qcopy(delta.f, ((char *)&D_L12_0015F580));
                if (hit > 0) {
                    struct Moby **iterator = D_L12_00178380;
                    int count = hit;
                    do {
                        if (FUN_L00_0025e3b8(*iterator) && func_001F9B88_2E2EF0(moby->pos.z - (*iterator)->pos.z) < 0.7f)
                            call_for_help_e81e0(*iterator, (int)hero.moby, 0x10000, &moby->pos, delta.f, 1.0f);
                        --count;
                        ++iterator;
                    } while (count != 0);
                }
            }
        }
        angle = func_001FA790_2E2EF0(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), moby->rot.z);
        if (angle > 0.9599311f) angle = 0.9599311f;
        else if (angle < -0.9599311f) angle = -0.9599311f;
        OGRE_F(0x188) = angle * 0.5f;
        OGRE_F(0x208) = angle * 0.5f;
        FUN_L12_002e8808(moby, (float *)&target->pos, OGRE_F(0x244), 0.0f);
        if (FUN_001f9b80(&moby->pos, &target->pos) < 5.5f || OGRE_B(0x242)) {
            angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            if (angle_difference_8808(moby->rot.z, angle) < 0.08726646f) goto state5;
        }
        if (moby->prev_seq != 8 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 8, 0, scale_game_frames_e81e0(11), 5);
        break;
    }
    case 5: {
        if (OGRE_F(0x28c) < 8.0f) OGRE_F(0x28c) = 8.0f;
        FUN_L12_002e8608(moby, (float *)&target->pos);
        FUN_L12_002e87b0((char *)moby);
        if (moby->prev_seq != 2 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 2, 0, scale_game_frames_e81e0(8), 5);
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 10 && moby->seq == moby->prev_seq)
                blend_moby_animation_ex(moby, 10, 0, scale_game_frames_e81e0(5), 5);
            moby->state = 6;
        }
        break;
    }
    case 6: {
        int count; float angle; float other; float value;
        FUN_L12_002e8608(moby, (float *)&target->pos);
        FUN_L12_002e87b0((char *)moby);
        if (OGRE_CHILD && is_value_within_interpolated_window(moby, 0.0f)) {
            moby_point_position(OGRE_CHILD, 0, &v);
            subtract_8808(&delta, work.v20, &v);
            angle = fast_arc_tan(delta.f[0], delta.f[1]);
            other = fast_add_rotations_e81e0(moby->rot.z, OGRE_F(0x208));
            angle = 0.2268928f < angle_difference_8808(angle, other) ? other : angle;
            value = vector_project_72(&delta);
            value = fast_arc_tan(value, delta.f[2]);
            build_spherical_offset(&delta, D_0015ED6C * 15.0f, angle, value);
            allocate_voice_for_target_entry(0, 0, (int)moby);
            count = truncate_float_to_s32(18.0f / (D_0015ED6C * 15.0f));
            FUN_L04_002aee30(moby, &v, &delta, count);
        }
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 5 && moby->seq == moby->prev_seq)
                blend_moby_animation_ex(moby, 5, 0, scale_game_frames_e81e0(8), 5);
            moby->state = 7;
        }
        break;
    }
    case 7: {
        float angle;
        FUN_L12_002e8608(moby, (float *)&target->pos);
        FUN_L12_002e87b0((char *)moby);
        if ((moby->prev_seq != 5 || (moby->anim_flags & 2)) && moby->prev_seq != 0 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 0, 0, scale_game_frames_e81e0(8), 5);
        if (!random_integer_below_e81e0(9)) goto state5;
        if (FUN_001f9b80(&moby->pos, &target->pos) > 10.5f && !OGRE_B(0x242)) goto state3;
        angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        if (angle_difference_8808(moby->rot.z, angle) > 0.4886922f) moby->state = 3;
        break;
    }
    case 8: {
        float angle;
        FUN_L12_002e8808(moby, (float *)(data + 0x220), 0.0f, 0.0f);
        angle = OGRE_F(0x23c) * 30.0f;
        if (angle > 0.7853982f) angle = 0.7853982f;
        else if (angle < -0.7853982f) angle = -0.7853982f;
        OGRE_F(0x188) = angle * 0.7f;
        OGRE_F(0x208) = angle * 0.4f;
        if (FUN_001f9b80(&moby->pos, data + 0x220) < 1.0f) moby->state = 2;
        break;
    }
    case 4: {
        float angle;
        moby->flags &= 0xffbe;
        if (FUN_L00_0025c698(moby, data + 0x70) & 0x60) {
            if (FUN_001f9b80(&moby->pos, &target->pos) < 6.0f || OGRE_B(0x242)) {
                angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
                if (angle_difference_8808(moby->rot.z, angle) < 0.12217305f) {
state5:
                    moby->state = 5;
                    break;
                }
            }
state3:
            moby->state = 3;
            break;
        }
        if (!(moby->pos.z < 2.0f)) goto finish;
        goto leave;
    }
    case 9: {
        if (FUN_L00_0025c698(moby, data + 0x70) & 0x60) {
            FUN_L00_00257470(moby, 0, -1);
            FUN_L12_002e7290(moby, &moby->pos);
            goto leave;
        }
        if (moby->pos.z < 2.0f) {
            FUN_L00_00257470(moby, 0, -1);
leave:
            FUN_L12_002e86d0(moby);
            return;
        }
        break;
    }
    }
finish:
    if (moby->visible || (OGRE_CHILD && OGRE_CHILD->visible)) {
        FUN_L12_002e8720(moby);
        if (moby->flags & 0x8000) {
            OGRE_F(0x188) = -OGRE_F(0x188);
            OGRE_F(0x208) = -OGRE_F(0x208);
        }
        FUN_L00_002628d8(D_0015ED64 * 0.018f, D_0015ED64 * 0.3f, moby, data + 0x120, 5);
        FUN_L00_002628d8(D_0015ED64 * 0.024f, D_0015ED64 * 0.3f, moby, data + 0x1a0, 6);
    }
    return;
}
#undef OGRE_I
#undef OGRE_F
#undef OGRE_B
#undef OGRE_H
#undef OGRE_CHILD
/* Variables of the l12 ogre moby (moby->pvars): FUN_L00_002e1678's TrollVars
   layout, with the alert and call-for-help state at 0x230.. */
typedef struct {
    u8 pad0[0x20];
    f32 health;          /* 0x20 */
    u8 pad24[2];
    s16 stun_timer;      /* 0x26 */
    u8 pad28[0x10];
    s32 reset_alert;     /* 0x38 */
    u8 pad3C[0x24];
    u8 anim[7];          /* 0x60: animation blend state */
    u8 unk67;            /* 0x67 */
    u8 pad68[8];
    u8 fall[0x10];       /* 0x70 */
    f32 unk80;           /* 0x80 */
    u8 pad84[4];
    f32 unk88;           /* 0x88 */
    f32 unk8C;           /* 0x8C */
    u8 pad90[4];
    s32 unk94;           /* 0x94 */
    u8 pad98[8];
    f32 unkA0;           /* 0xA0 */
    f32 unkA4;           /* 0xA4 */
    f32 unkA8;           /* 0xA8 */
    u8 padAC;
    u8 unkAD;            /* 0xAD */
    u8 padAE[0x12];
    f32 unkC0;           /* 0xC0 */
    f32 unkC4;           /* 0xC4 */
    u8 padC8[0x168];
    f32 speed;           /* 0x230 */
    u8 pad234[0xC];
    s16 alert_timer;     /* 0x240 */
    u8 pad242[0x32];
    s32 call_enabled;    /* 0x274 */
    s32 groups[4];       /* 0x278: group ids counted before calling for help (-1: none) */
    s32 call_timer;      /* 0x288 */
    f32 alert_speed;     /* 0x28C */
    s32 volume;          /* 0x290: clip volume that hands the ogre to FUN_L12_002e86d0 (-1: none) */
} OgreVars_e81e0;

typedef struct {
    u8 pad0[0x10];
    Vec4 pos;
    struct Moby *moby;   /* 0x20 */
} OgreHit_e81e0;

extern s32 D_001413D0_e81e0 __asm__("D_001413D0") __attribute__((section(".data")));
extern f32 D_0015ED6C_e81e0 __asm__("D_0015ED6C");
extern f32 D_0015ED70_e81e0 __asm__("D_0015ED70");
extern s32 scale_game_frames_e81e0(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown_16_e81e0(s16 *) __asm__("FUN_001f9770");
extern s32 random_integer_below_e81e0(s32) __asm__("FUN_00213260");
extern f32 fast_add_rotations_e81e0(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_cos_e81e0(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_e81e0(f32) __asm__("FUN_001f9de0");
extern s32 count_group_e81e0(s32, s32) __asm__("FUN_L01_0026e008");
extern void alert_group_e81e0(s32, s32) __asm__("FUN_L01_0026e090");
extern void call_for_help_e81e0(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern s32 is_point_inside_clip_volume_e81e0(void *, s32) __asm__("FUN_00214720");
extern OgreHit_e81e0 *find_hit_e81e0(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void take_damage_e81e0(struct Moby *, OgreHit_e81e0 *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void knockback_e81e0(void *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void start_fall_e81e0(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void anim_reset_e81e0(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_e81e0(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void ogre_leave_e81e0(struct Moby *) __asm__("FUN_L12_002e86d0");

/* Damage and alert step of the l12 ogre (cf. FUN_L12_002e3528): refreshes its
   speed while alerted, calls for help behind itself once its groups have 9 or
   more members, hands off inside its clip volume, and takes hits from other
   classes (dying, or staggering back). */
void FUN_L12_002e81e0(struct Moby *moby) {
    OgreVars_e81e0 *vars = (OgreVars_e81e0 *)moby->pvars;
    Vec4 v;
    s32 hit;
    f32 dmg;
    f32 angle;
    f32 unused;
    OgreHit_e81e0 *coll;

    if (vars->reset_alert != 0) {
        vars->reset_alert = 0;
        vars->alert_timer = scale_game_frames_e81e0(0xF0);
    }
    if (moby->state != 0) {
        vars->speed = tick_countdown_16_e81e0(&vars->alert_timer) ? vars->alert_speed : 20.0f;
    }
    if (vars->call_enabled != 0 && moby->state != 2) {
        vars->call_timer++;
        if (scale_game_frames_e81e0(0x3C) * 17 < vars->call_timer) {
            s32 count = 0;
            if (vars->groups[0] != -1)
                count += count_group_e81e0(vars->groups[0], 2);
            if (vars->groups[1] != -1)
                count += count_group_e81e0(vars->groups[1], 2);
            if (vars->groups[2] != -1)
                count += count_group_e81e0(vars->groups[2], 2);
            if (vars->groups[3] != -1)
                count += count_group_e81e0(vars->groups[3], 2);
            if (count >= 9 && random_integer_below_e81e0(0x27) == 0) {
                v.f[0] = fast_cos_e81e0(fast_add_rotations_e81e0(moby->rot.z, 3.1415927f));
                v.f[1] = fast_sin_e81e0(fast_add_rotations_e81e0(moby->rot.z, 3.1415927f));
                v.f[2] = 0.0f;
                call_for_help_e81e0(moby, D_001413D0_e81e0, 0x10000, &moby->pos, v.f, 1.0f);
            }
        }
    }
    if (vars->volume != -1 && is_point_inside_clip_volume_e81e0(&hero.motion.pos, vars->volume)) {
        ogre_leave_e81e0(moby);
        return;
    }
    coll = find_hit_e81e0(moby, 0x330000, 0);
    dmg = 0.0f;
    take_damage_e81e0(moby, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (coll != 0 && coll->moby->oclass != moby->oclass && coll->moby->oclass != 0xB8 &&
        moby->state != 9 && dmg != 0.0f) {
        if (moby->group != 0xFF) {
            alert_group_e81e0(moby->group, 1);
        }
        vars->health -= dmg;
        vars->unk80 = D_0015ED70_e81e0 * 30.0f;
        vars->unkAD = 0;
        vars->unk94 = 9;
        vars->unkA0 = 1.0f;
        vars->unkA4 = 1.0f;
        vars->unkA8 = 1.0f;
        if (vars->health <= 0.0f) {
            moby->flags &= ~0x1000;
            vars->unk88 = D_0015ED6C_e81e0 * 12.0f;
            vars->unk8C = D_0015ED6C_e81e0 * 9.0f;
            v.q = coll->pos.q;
            knockback_e81e0(&v, &angle, &vars->unk88, &vars->unk8C);
            start_fall_e81e0(moby, vars->fall, 9, 1, 0, angle);
            vars->unkC0 = 10.0f;
            vars->unkC4 = 19.0f;
            moby->state = 9;
            vars->unk67 = 0x78;
            anim_reset_e81e0(moby, vars->anim);
        } else {
            vars->unk88 = D_0015ED6C_e81e0 * 4.7f;
            vars->unk8C = D_0015ED6C_e81e0 * 7.7f;
            v.q = coll->pos.q;
            knockback_e81e0(&v, &unused, &vars->unk88, &vars->unk8C);
            start_fall_e81e0(moby, vars->fall, 6, 1, 0, unused);
            vars->unkC0 = 4.0f;
            vars->unkC4 = 8.0f;
            moby->state = 4;
            vars->unk67 = 0xFA;
            vars->stun_timer = scale_game_frames_e81e0(0x3C);
            anim_reset_e81e0(moby, vars->anim);
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_e81e0(moby, vars->anim);
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9988), where it is exact; names translated to the US level program. */

extern float FUN_001fa5c8(float, float);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");

void FUN_L12_002e8608(struct Moby *moby, float *pos) {
    char *data = (char *)moby->pvars;
    float a = fast_arc_tan(pos[0] - moby->pos.x, pos[1] - moby->pos.y);
    float d = FUN_001fa5c8(a, moby->rot.z);
    if (d > 1.2217305f) {
        d = 1.2217305f;
    } else if (d < -1.2217305f) {
        d = -1.2217305f;
    }
    *(float *)(data + 0x188) = d * 0.35f;
    *(float *)(data + 0x208) = d * 0.8f;
}
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Release the vendor moby and any child it owns. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9A50), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x248];
    void *child;
} Level12VendorData;


extern void FUN_L00_00257470(struct Moby *, int, int);

void FUN_L12_002e86d0(struct Moby *moby) {
    moby->unkB4 = 0;
    FUN_L00_00257470(moby, 0, -1);
    if (((Level12VendorData *)moby->pvars)->child != 0) {
        mark_moby_for_removal(((Level12VendorData *)moby->pvars)->child);
    }
    mark_moby_for_removal(moby);
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9AA0), where it is exact; names translated to the US level program. */

extern void *FUN_L04_002ce080(void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L12_0027b9c0(void *, void *, int);

void FUN_L12_002e8720(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    unsigned char *target = *(unsigned char **)(data + 0x248);
    if (target != 0) {
        unsigned char *current;
        FUN_L12_0027b9c0(moby, target, 1);
        current = *(unsigned char **)(data + 0x248);
        if (moby->flags & 1) {
            *(unsigned short *)(current + 0x34) |= 0x41;
        } else {
            *(unsigned short *)(current + 0x34) &= 0xFFBE;
        }
    } else {
        char vector[16];
        moby_point_position(moby, 1, vector);
        *(void **)(data + 0x248) = FUN_L04_002ce080(moby, vector);
    }
}
/* Initialize two vendor vectors using the global scale. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9B30), where it is exact; names translated to the US level program. */

extern void approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
f32 probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

/* Eases the moby's height toward the ground below it. */
void FUN_L12_002e87b0(char *moby) {
    float value = probe_ground_height(moby + 0x10, 0, 0.5f);
    approach_value(moby + 0x18, value, 27.0f * frame_time_sq);
}
extern float angle_8808(float, float) __asm__("FUN_001f9e90");
extern float limit_angle_8808(float, float) __asm__("FUN_001fa580");
extern float angle_difference_8808(float, float) __asm__("FUN_001fa688");
extern float distance_8808(void *, void *) __asm__("FUN_001f9b80");
extern float moved_distance_8808(void *, void *) __asm__("FUN_001f9b48");
extern void turn_8808(void *, void *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void approach_8808(void *, float, float) __asm__("FUN_00213ed8");
extern void steer_8808(void *, void *) __asm__("FUN_L12_00271dd8");
extern void subtract_8808(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int path_8808(int, void *, void *, void *) __asm__("FUN_L00_00261b48");
extern int inside_8808(void *, void *, int) __asm__("FUN_L00_00259740");
extern int frames_8808(int) __asm__("FUN_001f96f8");

void FUN_L12_002e8808(struct Moby *moby, float *target, float max_angle, float approach_speed) {
    char *data = (char *)moby->pvars;
    float distance;
    float angle;
    float old[4] __attribute__((aligned(16)));
    float result[4] __attribute__((aligned(16)));
    float next[4] __attribute__((aligned(16)));
    float delta[4] __attribute__((aligned(16)));
    int count;

    if (*(unsigned char *)(data + 0x242)) {
        float *rotation = &moby->rot.z;
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn_8808(rotation, data + 0x23c, angle, frame_time_sq * 15.707963f,
                  frame_time_sq * 12.566371f, frame_time * 25.132742f);
        return;
    }
    distance = distance_8808(&moby->pos, target);
    if (distance > 6.5f && max_angle != 0.0f) {
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        angle = limit_angle_8808(angle, max_angle);
        turn_8808(&moby->rot.z, data + 0x23c, angle, frame_time_sq * 15.707963f,
                  frame_time_sq * 12.566371f, frame_time * 25.132742f);
    } else {
        float *rotation = &moby->rot.z;
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn_8808(rotation, data + 0x23c, angle, frame_time_sq * 15.707963f,
                  frame_time_sq * 12.566371f, frame_time * 25.132742f);
    }
    if (approach_speed != 0.0f) {
        approach_8808(data + 0xf4, approach_speed, frame_time_sq * 7.0f);
    } else {
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        if (angle_difference_8808(moby->rot.z, angle) < 1.5707964f && 6.0f < distance) {
            approach_8808(data + 0xf4, frame_time * 7.0f, frame_time_sq * 7.0f);
        } else {
            approach_8808(data + 0xf4, frame_time, frame_time_sq * 7.0f);
        }
    }
    *(OvlQuad *)old = *(OvlQuad *)&moby->pos;
    steer_8808(moby, data + 0xd0);
    if (*(int *)(data + 0x26c) != -1) {
        subtract_8808(delta, old, &moby->pos);
        fast_vec_normalize(delta, delta, 0.05f);
        fast_vec_add(next, old, delta);
        if (path_8808(*(int *)(data + 0x26c), &moby->pos, next, result)) {
            char *path = D_L12_001B0930_w[*(int *)(data + 0x26c)];
            if (!inside_8808(&moby->pos, path + 0x10, *(int *)path))
                *(OvlQuad *)&moby->pos = *(OvlQuad *)result;
            if (moved_distance_8808(&moby->pos, old) < frame_time * 0.5f) {
                ++*(unsigned char *)(data + 0x243);
                count = *(unsigned char *)(data + 0x243);
                if (frames_8808(15) < count)
                    *(unsigned char *)(data + 0x242) = 1;
            } else {
                *(unsigned char *)(data + 0x243) = 0;
            }
        }
    }
}
extern f32 approach_value_e9f68(void *, f32, f32) __asm__("FUN_00213ed8");
extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern float FUN_L00_00257c48(float, float);
extern void FUN_L01_00274b78(void *, void *, float);
extern void FUN_L00_00263ac8(void *, int, void *, float);
extern void FUN_L12_002eb220(struct Moby *);
extern void FUN_L12_002eae00(struct Moby *);
extern int FUN_L12_002eaf68(struct Moby *, int);
extern void FUN_L12_0027ba70(int);
extern struct Moby *FUN_L12_00308670(struct Moby *);
extern void FUN_L12_00308708(struct Moby *, struct Moby *);
extern int allocate_voice_for_target_entry_e9f68(int, int, int) __asm__("FUN_0022da68");
extern void release_voice(int) __asm__("FUN_0022d798");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern char *FUN_L12_002ecce8(void *, void *, int, int, float, float, float);
extern void FUN_L00_0025e450(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int);
extern unsigned char D_001413F4 __attribute__((section(".data")));
extern int D_L12_00161F00 __attribute__((section(".sdata")));
extern int D_L12_00161F20 __attribute__((section(".sdata")));
extern int D_L12_001619A0 __attribute__((sda));
typedef struct { char pad[0x25c]; struct Moby *moby; int count; } Tracking_e9f68;
extern Tracking_e9f68 D_L12_00167100;
extern unsigned char D_0014C050[];
#include "rnc/audio/voice_pool.h"
extern unsigned char D_0013E550[];
extern unsigned char D_0013D4CB __attribute__((section(".data")));
extern unsigned short D_00141C50 __attribute__((section(".data")));
extern int D_L12_0015F580_gp __asm__("D_L12_0015F580") __attribute__((section(".sdata")));

void FUN_L12_002e9f68(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    struct Moby *child;
    float target[20] __attribute__((aligned(16)));
    OvlVec4 position, delta, aim, velocity;
    int index, count, limit, a, b, color, life, size, triggered;
    float distance, angle, value, spread;
    if (D_L12_0015F5C4 == 2) return;
    FUN_L01_00274b78(moby, target, 70.0f);
    child = *(struct Moby **)(data + 0xac);
    if (child != 0 && (signed char)child->state >= 0)
        FUN_L00_00263ac8(child, 0, data + 0xd0, 2.7f);
    FUN_L12_002eb220(moby);
    if (D_001413F4 == 2) {
        moby->scale = moby->pclass->scale * 0.5f;
        {
            struct Moby *child = *(struct Moby **)(data + 0xac);
            if (child != 0) child->scale = child->pclass->scale * 0.5f;
        }
    } else {
        moby->scale = moby->pclass->scale;
        {
            struct Moby *child = *(struct Moby **)(data + 0xac);
            if (child != 0) child->scale = child->pclass->scale;
        }
    }
    if ((unsigned int)(moby->state - 2) < 4) {
        if (*(int *)(data + 0xb0) == -1)
            *(int *)(data + 0xb0) = allocate_voice_for_target_entry_e9f68(3, 4, (int)moby);
    } else {
        int index = *(int *)(data + 0xb0);
        if (index != -1 && (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby && ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state)
            release_voice(index);
        *(int *)(data + 0xb0) = -1;
    }
    index = *(int *)(data + 0xc0);
    if (current_level_index == 12 && D_L12_00161F00 != 0) {
        if (index == 0) {
            if (D_L12_00167100.moby == moby) D_L12_00167100.moby = 0;
            if (!moby->visible) {
                if (*(struct Moby **)(data + 0xac)) {
                    mark_moby_for_removal(*(struct Moby **)(data + 0xac));
                    *(struct Moby **)(data + 0xac) = 0;
                }
                mark_moby_for_removal(moby);
                return;
            }
            index = *(int *)(data + 0xc0);
            goto check_mode;
        }
    } else {
check_mode:
        if (index == 0) goto update_state;
    }
    if (D_0014C050[moby->mission + current_level_index * 16] == 255) {
        if (*(struct Moby **)(data + 0xac)) {
            mark_moby_for_removal(*(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (index == 0) goto update_state;
    if (D_L12_001619A0 != 0) {
        if (*(struct Moby **)(data + 0xac)) {
            FUN_L12_00308708(moby, *(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
update_state:
    switch (moby->state) {
    case 0: {
        int a, b, limit;
        *(short *)(data + 0x24) = 6;
        *(float *)(data + 0x20) = 6.0f;
        data[0x28] = 2;
        data[0x29] = 0;
        *(float *)(data + 0x94) = 0.0f;
        *(float *)(data + 0x98) = 0.0f;
        *(float *)(data + 0x9c) = 0.0f;
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        FUN_L12_0027ba70(*(int *)(data + 0xa0));
        FUN_L12_0027ba70(*(int *)(data + 0xa0));
        FUN_L12_0027ba70(*(int *)(data + 0xa4));
        if (moby->draw_dist < 128) moby->draw_dist = 128;
        if (*(struct Moby **)(data + 0xac) == 0) {
            *(struct Moby **)(data + 0xac) = FUN_L12_00308670(moby);
            (*(struct Moby **)(data + 0xac))->update_dist = 0;
            (*(struct Moby **)(data + 0xac))->flags |= 0x41;
        }
        qcopy(&moby->pos, D_L12_001B0930[*(int *)(data + 0xa0)] + 0x10);
        if (*(int *)(data + 0xc0) != 0 && *(int *)(data + 0xc4) == 0) {
            a = scale_game_frames_alt(240);
            b = scale_game_frames_alt(400);
            a = FUN_L00_00257b90(a, b);
            {
                int limit = D_L12_00161F20 - 1;
                *(int *)(data + 0xc4) = a;
                if (limit > 10) limit = 10;
                if (limit < 0) limit = 0;
                *(int *)(data + 0xc4) += limit * scale_game_frames_alt(35);
            }
        }
        *(int *)(data + 0xb4) = 0;
        moby->state = 1;
        break;
    }
    case 1: {
        int triggered, a;
        struct Moby *child;
        triggered = 0;
        if (*(int *)(data + 0xc0) && D_L12_00161F00)
            triggered = tick_countdown_32_alt_c2((int *)(data + 0xc4)) != 0;
        if ((*(int *)(data + 0x80) != -1 && is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0x80))) || triggered) {
            data[0x58] = 22; data[0x59] = 1; data[0x5a] = 7;
            moby->hit_slot = 255;
            moby->flags &= 0xffbe;
            child = *(struct Moby **)(data + 0xac);
            if (child) child->flags &= 0xffbe;
            moby->flags |= 0x1000;
            a = moby->pclass->collision;
            moby->state = 2;
            moby->collision = a;
        }
        break;
    }
    case 2:
        if (*(int *)(data + 0xc0) == 0 && D_L12_00167100.moby == 0) {
            D_L12_00167100.moby = moby;
            D_L12_00167100.count = 0;
        }
        if (*(int *)(data + 0xc0)) {
            count = *(short *)(data + 0x8c);
            limit = D_L12_00161F20 - 3;
            if (limit > 5) limit = 5;
            if (limit < 0) limit = 0;
            if (count < limit * scale_game_frames_alt(15))
                *(short *)(data + 0x8c) = limit * scale_game_frames_alt(15);
        }
        if (FUN_L12_002eaf68(moby, *(int *)(data + 0xa0))) moby->state = 3;
        break;
    case 3: {
        int count, limit, a, b, color, life, size;
        float distance, angle, value, spread;
        if (current_level_index == 12 && !D_0013D4CB && !D_00141C50) help_request(0x2ee2, 0x5d);
        distance = FUN_001f9b80(&moby->pos, &hero.motion.pos);
        if (*(int *)(data + 0xc0) == 0) {
            if (distance < 21.0f && moby->pos.z - hero.motion.pos.f[2] > 1.0f) {
                if (D_L12_00167100.moby == 0) {
                    D_L12_00167100.moby = moby; D_L12_00167100.count = 0;
                }
            } else if (D_L12_00167100.moby == moby) D_L12_00167100.moby = 0;
        }
        distance = FUN_001f9b80(&moby->pos, target);
        if ((distance < 40.0f && moby->pos.z - target[2] > -2.0f) || *(int *)(data + 0xc0)) {
            angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
            moby->rot.z = spring_turn((float *)(data + 0x88), moby->rot.z, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
            value = FUN_001f9b80(target, &moby->pos);
            angle = fast_arc_tan(value, moby->pos.z - target[2]);
            moby->rot.y = spring_turn((float *)(data + 0x9c), moby->rot.y, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
            clear_u64_value(data + 0x70);
            FUN_L12_002eae00(moby);
            if (FUN_001f9770(data + 0x8c)) {
                ++*(short *)(data + 0xbc);
                moby_point_position(moby, *(int *)(data + 0xb8), &position);
                *(int *)(data + 0xb8) = *(int *)(data + 0xb8) == 0;
                qcopy(&aim, target + 8);
                if (*(int *)(data + 0xc0)) {
                    qcopy(&aim, D_L12_00167240);
                    aim.f[2] += random_float_between(-0.5f, 0.1f);
                    aim.f[0] += FUN_L00_00257c48(0.0f, 0.4f);
                }
                subtract_vector_xyz_c3(&delta, &aim, &position);
                if (*(int *)(data + 0xc0)) fast_vec_normalize(&delta, &delta, frame_time * 22.0f);
                else fast_vec_normalize(&delta, &delta, frame_time * 15.0f);
                angle = fast_arc_tan(delta.f[0], delta.f[1]);
                spread = 1.3f;
                a = scale_game_frames_alt(130);
                FUN_L12_002ecce8(&delta, &position, (int)moby, a, angle, frame_time * 15.0f, 0.0f);
                allocate_voice_for_target_entry_e9f68(0, 0, (int)moby);
                fast_vec_normalize(&delta, &delta, 0.5f);
                fast_vec_add(&position, &position, &delta);
                for (count = 24; count >= 0; --count) {
                    velocity.f[0] = FUN_L00_00257c48(0.0f, frame_time * spread);
                    velocity.f[1] = FUN_L00_00257c48(0.0f, frame_time * spread);
                    velocity.f[2] = FUN_L00_00257c48(0.0f, frame_time * spread);
                    a = FUN_L00_00257b90(170, 240);
                    b = FUN_L00_00257b90(110, 180);
                    color = (b << 8) | ((long)a | 0x7f000000L);
                    a = FUN_L00_00257b90(30, 70);
                    life = scale_game_frames_alt(a);
                    size = FUN_L00_00257b90(120, 170);
                    FUN_L00_00269958(&position, &velocity, color, 255, life, 30, size, 1);
                }
                limit = 4;
                if (*(int *)(data + 0xc0)) {
                    if (D_L12_00161F20 > 10) limit = 2;
                    if (D_L12_00161F20 > 6) limit = 3;
                }
                if (*(short *)(data + 0xbc) >= limit && *(int *)(data + 0xa4) != -1) {
                    *(short *)(data + 0xbc) = 0; moby->state = 4;
                } else *(short *)(data + 0x8c) = scale_game_frames_alt(40);
            }
        }
        break;
    }
    case 4:
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        moby->rot.z = spring_turn((float *)(data + 0x88), moby->rot.z, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
        {
            char *path = D_L12_001B0930[*(int *)(data + 0xa4)];
            if (*(short *)(data + 0xbe) != 0) subtract_vector_xyz_c3(&position, path + 16, &moby->pos);
            else subtract_vector_xyz_c3(&position, path + *(int *)path * 16, &moby->pos);
        }
        distance = vector_length_xyz_c3(&position);
        if (!(distance > 5.5f)) {
            approach_value_e9f68(data + 0x94, 0.0f, frame_time_sq * 9.0f);
            if (*(float *)(data + 0x94) == 0.0f) {
                *(short *)(data + 0x8c) = scale_game_frames_alt(50);
                *(short *)(data + 0xbe) = *(short *)(data + 0xbe) == 0;
                moby->state = 3;
                break;
            }
        } else approach_value_e9f68(data + 0x94, frame_time * 10.0f, frame_time_sq * 7.0f);
        fast_vec_normalize(data + 0x70, &position, *(float *)(data + 0x94));
        fast_vec_add(&moby->pos, &moby->pos, data + 0x70);
        FUN_L12_002eae00(moby);
        break;
    case 5:
        if (FUN_001f9770(data + 0x8e)) {
            moby->state = moby->command;
        } else {
            value = *(float *)(data + 0x94) - frame_time_sq * 4.0f;
            *(float *)(data + 0x94) = value;
            if (value < 0.0f) *(float *)(data + 0x94) = 0.0f;
            value = fast_cos_c(moby->rot.z);
            position.f[0] = value * -*(float *)(data + 0x94);
            value = fast_sin_c2(moby->rot.z);
            position.f[1] = value * -*(float *)(data + 0x94);
            position.f[2] = 0.0f;
            fast_vec_add(&moby->pos, &moby->pos, &position);
        }
        break;
    case 6: {
        int index;
        if (D_L12_00167100.moby == moby) D_L12_00167100.moby = 0;
        if (*(struct Moby **)(data + 0xac)) {
            FUN_L12_00308708(moby, *(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        FUN_L00_0025e450(moby, ((char *)&D_L12_0015F580_gp), &moby->pos, 0.0f, 0.0f, 20, 3, 4, 4.0f, 2.0f, 100000.0f, 2, 3.0f, 15.0f, 1, 1, -1, 0);
        if (*(int *)(data + 0xc0) == 0) FUN_L00_00257470(moby, 0, -1);
        index = *(int *)(data + 0xb0);
        if (index != -1 && (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby && ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state) release_voice(index);
        *(int *)(data + 0xb0) = -1;
        if (*(int *)(data + 0xc0) == 0) {
remove:
            mark_moby_for_removal(moby);
            return;
        }
        a = scale_game_frames_alt(240);
        b = scale_game_frames_alt(400);
        *(int *)(data + 0xc4) = FUN_L00_00257b90(a, b);
        moby->collision = 0;
        moby->state = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        break;
    }
    }
    moby->pos.z -= fast_cos_c(*(float *)(data + 0x90)) * 0.17f;
    a = scale_game_frames_alt(120);
    angle = fast_add_rotations_c3(*(float *)(data + 0x90), 6.2831855f / (float)a);
    *(float *)(data + 0x90) = angle;
    moby->pos.z += fast_cos_c(angle) * 0.17f;
    {
        struct Moby *child = *(struct Moby **)(data + 0xac);
        if (child) {
            *(OvlQuad *)&child->pos = *(OvlQuad *)&moby->pos;
            child = *(struct Moby **)(data + 0xac);
            *(OvlQuad *)&child->rot = *(OvlQuad *)&moby->rot;
            advance_moby_animation(*(struct Moby **)(data + 0xac));
            FUN_L00_00250df8_x(*(struct Moby **)(data + 0xac));
            (*(struct Moby **)(data + 0xac))->flags |= 6;
        }
    }
    if (*(int *)(data + 0xc0) == 0 && moby->state > 1 && moby->visible && distance_xyz(&moby->pos, D_L12_00167240) < 30.0f) {
        FUN_L00_0025a120((char *)moby);
        moby->shadow_range = 24;
    }
}
/* Springs the moby's two rotation angles toward a heading taken from its data's direction vector. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC180), where it is exact; names translated to the US level program. */

extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

void FUN_L12_002eae00(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float ang = FUN_001fa5c8(fast_arc_tan(*(float *)(data + 0x70), *(float *)(data + 0x74)),
                             moby->rot.z);
    float a = *(float *)(data + 0x94) * 0.2268928f * FUN_001f9dc8(ang) / (frame_time * 10.0f);
    float b = *(float *)(data + 0x94) * -0.2268928f * FUN_001f9de0(ang) / (frame_time * 10.0f);
    float r0, k0, d0, m0, k1, d1, m1;
    k0 = frame_time_sq * 0.5235988f;
    d0 = frame_time_sq * 1.0471976f;
    m0 = frame_time * 0.7853982f;
    r0 = spring_turn((float *)(data + 0x9C), moby->rot.y, a, k0, d0, m0);
    k1 = frame_time_sq * 1.0471976f;
    d1 = frame_time_sq * 2.0943952f;
    m1 = frame_time * 1.2217305f;
    moby->rot.y = r0;
    moby->rot.x =
        spring_turn((float *)(data + 0x98), moby->rot.x, b, k1, d1, m1);
}
extern char *D_L12_001B0930_f[] __asm__("D_L12_001B0930");
extern float D_0015ED6C_f __asm__("D_0015ED6C");
extern float D_0015ED70_f __asm__("D_0015ED70");
extern void ahead_f(char *, float *, float) __asm__("FUN_L01_00274b78");
extern float atan_f(float, float) __asm__("FUN_001f9e90");
extern float turn_f(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern void appr_f(void *, float, float) __asm__("FUN_00213ed8");
extern void sub_f(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_f(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_f(void *, void *, void *) __asm__("FUN_001f9a10");
extern float dist_f(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_L12_002eae00(struct Moby *);
typedef struct { char pad[0x48]; float rz; } M_af68;
typedef struct { float x, y, z, w; } N_af68;

/* Follows a path: steers toward the current node, slows to a stop near the end, returns 1 once stopped. */
int FUN_L12_002eaf68(struct Moby *m, int idx) {
    char *path = D_L12_001B0930_f[idx];
    char *d = (char *)m->pvars;
    float sum = 0.0f;
    float v[20]; /* target record from the finder; also the step vector */
    int i;
    float a;
    for (i = *(int *)(d + 0xB4); i < *(int *)path - 1; i++)
        sum += *(float *)(path + i * 16 + 0x1C);
    if (sum > 5.0f) {
        a = atan_f(*(float *)((*(int *)(d + 0xB4) << 4) + path + 0x10) - m->pos.x,
                   *(float *)((*(int *)(d + 0xB4) << 4) + path + 0x14) - m->pos.y);
        ((M_af68 *)m)->rz = turn_f((float *)(d + 0x88), ((M_af68 *)m)->rz, a, 0.004f, 0.3f, D_0015ED6C_f * 0.7853982f);
        appr_f(d + 0x94, D_0015ED6C_f * 10.0f, D_0015ED70_f * 7.0f);
    } else {
        ahead_f(m, v, 48.0f);
        a = atan_f(v[0] - m->pos.x, v[1] - m->pos.y);
        ((M_af68 *)m)->rz = turn_f((float *)(d + 0x88), ((M_af68 *)m)->rz, a, 0.004f, 0.3f, D_0015ED6C_f * 0.7853982f);
        appr_f(d + 0x94, 0.0f, D_0015ED70_f * 9.0f);
        sum = *(float *)(d + 0x94);
        if (sum == 0.0f)
            return 1;
    }
    sub_f(v, &((N_af68 *)path)[*(int *)(d + 0xB4) + 1], &m->pos);
    norm_f(d + 0x70, v, *(float *)(d + 0x94));
    add_f(&m->pos, &m->pos, d + 0x70);
    if (dist_f(&m->pos, &((N_af68 *)path)[*(int *)(d + 0xB4) + 1]) < 2.0f) {
        (*(int *)(d + 0xB4))++;
        if (*(int *)path - 1 < *(int *)(d + 0xB4))
            *(int *)(d + 0xB4) = *(int *)path - 1;
    }
    FUN_L12_002eae00(m);
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC5A0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_0025d458(void *m, short *p);
extern void FUN_L00_0025d538(void *, void *);
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L12_002eb220(struct Moby *m) {
    int st;
    float fv;
    int res;
    char *d;
    char *r;
    float a, b, p, q;
    d = (char *)m->pvars;
    fv = 0.0f;
    r = find_moby_hit(m, 0x330000, 0);
    res = hit_reaction(m, r, d + 0x20, 0, &st, &fv, 0, 4);
    if (st != 1 && m->state != 6 &&
        *(short *)(*(char **)(r + 0x20) + 0xA6) != 0xB8 &&
        *(short *)(*(char **)(r + 0x20) + 0xA6) != 0x199) {
        float t;
        if (*(short *)(*(char **)(r + 0x20) + 0xA6) == 0x1CA)
            fv = 10.0f;
        t = *(float *)(d + 0x20) - fv;
        *(float *)(d + 0x20) = t;
        if (res == 1 || t <= 0.0f) {
            allocate_voice_for_target_entry(2, 0, (int)m);
            m->state = 6;
        } else if (res != 0xB) {
            *(unsigned char *)(d + 0x67) = 0xB4;
            FUN_L00_0025d458(m, (short *)(d + 0x60));
            if (*(float *)(r + 0x1C) == 5627.925f && m->state != 5) {
                a = fast_arc_tan(m->pos.x - *(float *)(r + 0),
                                 m->pos.y - *(float *)(r + 4));
                b = fast_subtract_rotations(a, m->rot.z);
                p = fast_cos(b) * 0.08726646f;
                q = fast_sin(b) * -0.34906584f;
                if (absolute_float(m->rot.x) < absolute_float(q)) {
                    m->rot.x = q;
                    *(float *)(d + 0x98) = 0.0f;
                }
                if (absolute_float(m->rot.y) < absolute_float(p)) {
                    m->rot.y = p;
                    *(float *)(d + 0x9C) = 0.0f;
                }
                *(float *)(d + 0x94) = frame_time * 4.0f;
                *(short *)(d + 0x8E) = scale_game_frames(0x3C);
                m->command = m->state;
                m->state = 5;
                allocate_voice_for_target_entry(1, 0, (int)m);
            }
        }
    }
    m->hit_slot = 0xFF;
    FUN_L00_0025d538(m, d + 0x60);
}
/* Data of the second watcher (FUN_L12_002eb570): the L16 watcher layout with
 * the turret and base mobys it spawns at 0x164/0x168. */
typedef struct {
    char pad00[0x20];
    L16WatchActivation activation;
    char body[0x60];
    float padC0;
    float pitch;
    float yaw;
    float padCC;
    float height;
    char padD4[0xC];
    char head[0x60];
    char pad140[8];
    float head_yaw;
    char pad14C[0x14];
    int region;
    char *turret;
    char *base;
    char pad16C[4];
    float target[4];
    char pad180[4];
    int moving_timer;
    int target_timer;
} L12WatchData;

typedef struct {
    char pad[0x46C];
    int hidden;
} L12WatchGlobals;

extern L12WatchGlobals D_0013D5B0_b570 __asm__("D_0013D5B0") __attribute__((section(".data")));
extern int D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern float D_0015ED64_b570 __asm__("D_0015ED64");
extern void FUN_L03_002db480_b570(void *) __asm__("FUN_L03_002db480");
extern void blend_moby_animation_b570(void *, int, int, int) __asm__("FUN_00212f90");
extern void FUN_L00_00260860_b570(int, int) __asm__("FUN_L00_00260860");

void FUN_L12_002eb570(L16WatchMoby *m) {
    L16WatchScratch scratch;
    L12WatchData *d = (L12WatchData *)m->data;
    float rate;
    float head_rate;
    int tracking;
    FUN_L03_002db480_b570(m);
    if (D_0013D5B0_b570.hidden == 0) {
        if (m->animation != 1)
            blend_moby_animation_b570(m, 1, 0, 2);
    } else if (m->animation != 0) {
        blend_moby_animation_b570(m, 0, 0, 2);
    }
    switch (m->state) {
    case 0:
        m->state = 1;
        FUN_L00_002668a0(m, &d->activation);
        d->activation.radius = 1.7f;
        FUN_L02_0025c758(m);
        if (alternate_item_available[0x13C])
            *(short *)((char *)d + 0x56) = -1;
        break;
    case 1:
        if (alternate_item_available[0x13C] == 0 && FUN_L00_00266448(m, &d->activation)) {
            FUN_L01_002783a8_c(m, 2.52f);
            *(int *)((char *)m + 0x58) = 0;
            m->state = 2;
        }
        break;
    case 2:
        if (D_L12_0015F5C4 != 2) {
            *(float *)((char *)m + 0x58) = 1.0f;
            FUN_L00_002502a0(m->widget);
            if (d->region != -1) {
                char *p = D_L12_001600EC + (d->region << 7);
                FUN_L00_00284e50(p + 0x30, p + 0x70);
            }
            m->state = 1;
            if (*(short *)((char *)d + 0x24) == 2) {
                FUN_L00_00260860_b570(4, 0);
                FUN_L00_00263d40_c(0x2EE6, -1);
                if (d->region != -1) {
                    char *p = D_L12_001600EC + (d->region << 7);
                    FUN_L00_00284e50(p + 0x30, p + 0x70);
                }
                memcard_save_data(0, -1);
            }
        }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (m->animation == 0) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (FUN_001f9b80(m->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                m->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    m->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    m->position[1])) < 1.5707964f) {
            if (vector_length_xyz(player + 0x80) > 0.01f)
                d->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32(&d->moving_timer);
        } else if (d->moving_timer) {
            d->moving_timer = 0;
            qcopy(d->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&d->target_timer)) {
            float heading;
            d->target_timer =
                random_float_between_cf(multiply_global_factor_ed64(truncate_float_to_s32_cf(180.0f, 300.0f)));
            heading =
                fast_add_rotations(m->yaw, truncate_float_to_s32_cf(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(d->target, 6.0f, heading,
                                   truncate_float_to_s32_cf(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(d->target, d->target, m->position);
        }
        if (d->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, d->target);
        }
    }
    if (D_0013D5B0_b570.hidden == 0) {
        if (d->turret == 0) {
            char *o = func_0020D348_m_x(0x4C1);
            d->turret = o;
            if (o) {
                *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)m + 0x32);
                d->turret[0x31] = 1;
                {
                    char *p = d->turret;
                    qcopy(p + 0x10, (char *)m + 0x10);
                    qcopy(p + 0x40, (char *)m + 0x40);
                    FUN_L00_00250df8_x(p);
                }
                *(unsigned short *)(d->turret + 0x34) = 0;
                blend_moby_animation_b570(d->turret, 1, 0, 2);
            }
        } else if (D_L12_0015F5C4 == 2) {
            *(unsigned short *)(d->turret + 0x34) |= 1;
            d->turret[0x31] = 0;
        } else {
            *(unsigned short *)(d->turret + 0x34) &= ~1;
            d->turret[0x31] = 1;
        }
        if (d->base == 0) {
            char *o = func_0020D348_m_x(0x196);
            d->base = o;
            if (o) {
                *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)m + 0x32);
                d->base[0x31] = 1;
                {
                    char *p = d->base;
                    qcopy(p + 0x10, (char *)m + 0x10);
                    qcopy(p + 0x40, (char *)m + 0x40);
                    FUN_L00_00250df8_x(p);
                }
            }
        } else if (D_L12_0015F5C4 == 2) {
            *(unsigned short *)(d->turret + 0x34) |= 1;
            d->turret[0x31] = 0;
            *(unsigned short *)(d->base + 0x34) |= 1;
            d->base[0x31] = 0;
        } else {
            *(unsigned short *)(d->turret + 0x34) &= ~1;
            d->turret[0x31] = 1;
            *(unsigned short *)(d->base + 0x34) &= ~1;
            d->base[0x31] = 1;
        }
    } else {
        if (d->base) {
            FUN_0020c828_c(d->base);
            d->base = 0;
        }
        if (d->turret) {
            FUN_0020c828_c(d->turret);
            d->turret = 0;
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, m->position);
        scratch.eye[2] += 1.0f;
        subtract_vector_xyz(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), m->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        d->pitch = pitch;
        d->head_yaw = d->yaw = yaw * 0.5f;
    }
    if (D_0015EDB0_b)
        d->height = 2.75f;
    FUN_L00_002628d8(rate * D_0015ED64_b570, head_rate * D_0015ED64_b570, m, d->body, 1);
    FUN_L00_002628d8(rate * D_0015ED64_b570, head_rate * D_0015ED64_b570, m, d->head, 0);
}
/* Hit handler: counts hits toward a help message, bursts debris and blinks the moby until its timer runs out. */
extern char D_0013D408[];
extern int tick_countdown_32_alt(int *arg0) __asm__("FUN_001f9740");
extern short D_0015EE04 __attribute__((sda));
extern s32 D_L12_001619B4 __attribute__((sda));
extern void FUN_L00_0025e450(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int);
extern void FUN_L00_00260a88(void *, int, int, int, int);
extern void FUN_L00_00263d40(int, int);
extern void clear_vector(void *) __asm__("func_001F99F8");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void FUN_L12_002ebca8(struct Moby *m, int flag) {
    char *d = (char *)m->pvars;
    float a[4];
    float b[4];
    unsigned char *q;
    if (find_moby_hit(m, 0xA30000, 0) != 0) {
        if (hero.state.current != 0x32) {
            *(int *)&D_0015EE04 += 1;
            if (*(int *)&D_0015EE04 >= 5) {
                q = (unsigned char *)(D_0013D408);
                if (q[0x15] == 0) {
                    q[0x15] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    FUN_L00_00263d40(0x53D6, -1);
                }
            }
        }
        qcopy(a, &m->pos);
        clear_vector(b);
        FUN_L00_00260a88(m, 0x1F, 0x4A, 2, -1);
        *(int *)(d + 0xA4) = scale_game_frames(500);
        FUN_L00_0025e450(m, b, a, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, -1, 3.0f, 15.0f, 1,
                         1, -1, 0);
    }
    m->hit_slot = 0xFF;
    if (*(int *)&D_L12_001619B4 != 0 && flag != 0) {
        m->collision = 0;
        m->flags = (m->flags | 0x41) & 0xEFFF;
    }
    if (*(int *)(d + 0xA4) == 0 && (m->flags & 1) == 0)
        return;
    if (tick_countdown_32_alt((int *)(d + 0xA4)) != 0 && flag != 0 &&
        *(int *)&D_L12_001619B4 == 0) {
        m->collision = m->pclass->collision;
        m->flags = (m->flags & 0xFFBE) | 0x1000;
    } else {
        m->collision = 0;
        m->flags = (m->flags | 0x41) & 0xEFFF;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002ED228), where it is exact; names translated to the US level program. */

extern char *D_L12_001B0930[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern void FUN_L01_0028b410(char *p);
extern void FUN_L12_002ebca8_c(char *m, int flag) __asm__("FUN_L12_002ebca8");
extern void add_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_L01_00275690(char *vec, float scale);
extern int func_L01_00276680_i(char *vec, float scale) __asm__("FUN_L01_00275690");

extern short D_L12_001619A8 __attribute__((sda));
extern short D_L12_001619AC __attribute__((sda));
extern short D_L12_001619B0 __attribute__((sda));
void FUN_L12_002ebea8(char *m) {
    char *d = *(char **)(m + 0x78);
    float A[4];
    float B[4];
    float C[4];
    float E[4];
    if (d != 0) {
        int r = func_L01_00276680_i(m, convert_integer_to_float(*(short *)(m + 0x32)));
        FUN_L12_002ebca8_c(m, r == -1);
        switch ((unsigned char)m[0x20]) {
        case 0:
            if (*(int *)(d + 0x74) != -1) {
                char *tbl;
                int idx;
                int cnt;
                FUN_L01_0028b410(d + 0x60);
                tbl = D_L12_001B0930[*(int *)(d + 0x74)];
                *(char **)(d + 0x70) = tbl;
                idx = *(int *)(d + 0xA0);
                cnt = *(int *)tbl - 1;
                if (cnt < idx) idx = cnt;
                qcopy((m + 0x10), tbl + idx * 16 + 0x10);
                d[0x2A] = 0x10;
                d[0x28] = 2;
                d[0x2C] = 0x2C;
                *(float *)(d + 0x30) = 1.4f;
                d[0x2B] = 1;
                *(int *)(d + 0x60) = idx;
                m[0x20] = 1;
                *(short *)(m + 0x32) = 0xAA;
                *(unsigned short *)(m + 0x34) |= 0x1000;
            }
            break;
        case 1: {
            char *p = d + 0x60;
            int cur = *(int *)p;
            char *tbl = *(char **)(p + 0x10);
            int step = *(char *)(p + 4);
            int n = *(int *)tbl;
            int prev = (cur + n - step) % n;
            int next = (cur + n + step) % n;
            float h;
            float ang;
            qcopy(A, tbl + prev * 16 + 0x10);
            qcopy(B, tbl + cur * 16 + 0x10);
            qcopy(C, tbl + next * 16 + 0x10);
            subtract_vector_xyz_c(A, B, A);
            subtract_vector_xyz_c(B, C, B);
            h = fast_arc_tan(A[0], A[1]);
            h = fast_subtract_rotations(h, fast_arc_tan(B[0], B[1]));
            ang = h / vector_length_xyz_c(A);
            *(float *)(m + 0x40) = fast_add_rotations_c(*(float *)(m + 0x40),
                fast_subtract_rotations(ang * *(float *)&D_L12_001619AC, *(float *)(m + 0x40)) / *(float *)&D_L12_001619B0);
            if (distance_xyz(m + 0x10, C) < *(float *)&D_L12_001619A8 * D_0015ED6C) {
                char *t3;
                *(int *)(d + 0x60) = next;
                t3 = *(char **)(p + 0x10);
                if (next == *(int *)t3 - 1) {
                    qcopy((m + 0x10), t3 + 0x10);
                }
            }
            subtract_vector_xyz_c(E, C, m + 0x10);
            normalize_vector_xyz(E, E, *(float *)&D_L12_001619A8 * D_0015ED6C);
            add_vector_xyz_c2(m + 0x10, m + 0x10, E);
            *(float *)(m + 0x48) = fast_add_rotations_c(*(float *)(m + 0x48),
                fast_subtract_rotations(fast_arc_tan(E[0], E[1]), *(float *)(m + 0x48)) / *(float *)&D_L12_001619B0);
            *(float *)(m + 0x44) = 0.52359879f;
            break;
        }
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Idle critter: waits a random time, then plays one of its fidget animations (or reacts when the hero comes close). */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002ED550), where it is exact; names translated to the US level program. */

extern float FUN_001f9b80(void *, void *);
extern int FUN_L00_001fefc8(void *);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");

void FUN_L12_002ec1d0(unsigned char *m) {
    float dist = FUN_001f9b80(m + 0x10, &hero.motion.pos);
    switch (m[0x20]) {
    case 0:
        if (m[0x53] != 0)
            blend_moby_animation(m, 0, 0, 0);
        m[0x20] = 1;
        m[0xBC] = random_integer_below(scale_game_frames(0xB4));
        break;
    case 1:
        if (m[0x52] == 0 && dist < 9.0f && m[0xBC] < 2)
            m[0xBC] = 2;
        if (dist < 12.0f && m[0x52] == m[0x53] && m[0x52] != 0)
            m[0xBC] = 0;
        if (FUN_L00_001fefc8(m + 0xBC)) {
            switch (m[0x52]) {
            case 0:
                if (random_integer_below(0x100) & 1) {
                    if (m[0x53] != 3)
                        blend_moby_animation(m, 3, 0, 0);
                } else {
                    if (m[0x53] != 5)
                        blend_moby_animation(m, 5, 0, 0);
                }
                break;
            case 1:
                if (m[0x53] != 4)
                    blend_moby_animation(m, 4, 0, 0);
                break;
            case 2:
                if (m[0x53] != 6)
                    blend_moby_animation(m, 6, 0, 0);
                break;
            }
            m[0x20] = 2;
        }
        break;
    case 2:
        if (m[0x70] & 2) {
            switch (m[0x52]) {
            case 3:
                if (m[0x53] != 1)
                    blend_moby_animation(m, 1, 0, 0);
                break;
            case 4:
            case 6:
                if (m[0x53] != 0)
                    blend_moby_animation(m, 0, 0, 0);
                break;
            case 5:
                if (m[0x53] != 2)
                    blend_moby_animation(m, 2, 0, 0);
                break;
            }
            m[0x20] = 1;
            m[0xBC] = random_integer_below(scale_game_frames(0xB4));
        }
        break;
    }
}
