#include "rnc/gameplay/hero.h"
#include "rnc/audio/voice_pool.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"
#include "rnc/overlay/entities.h"

#include "rnc/math/vector.h"

#include "qcopy.h"

typedef struct Moby Moby;

extern float D_0015ED6C;
extern char *D_L12_0015FFD8_gp __asm__("D_L12_0015FFD8") __attribute__((section(".sdata")));

/* Hoven turret battery update: aims each manned gun at the hero, fires, takes hits and smokes when damaged. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00306890), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    char pad0[0x60];
    float f60;
    char pad64[0x1C];
    float f80;
    int w84;
    char pad88[4];
    float f8C;
    short h90;
    char b92;
    char b93;
    char pad94[4];
    short h98;
    short h9A;
    short h9C;
    short h9E;
} Gun_306890;

extern char *FUN_L00_0025a420(void *, int, int);
extern struct Hero hero_lui __asm__("D_0013F350") __attribute__((section(".data")));
extern char D_L12_00167240[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_0015ED64 __attribute__((section(".sdata")));
extern float D_0015ED6C_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float FUN_L00_00257c48(float, float);
extern float FUN_L00_0025bc00(float *, float, int, float);
extern float FUN_L00_0025bc98(void *, float, void *, int, float, float, float);
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int D_L12_00161F00;
extern int D_L12_00161F04;
extern int D_L12_00161F20 __attribute__((section(".sdata")));
extern int D_L12_00161F24 __attribute__((section(".sdata")));
extern int D_L12_00161F34 __attribute__((sda));
extern int D_L12_001FB670[];
extern int D_L12_001FB688[];
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_001fefc8(void *);
extern int FUN_L00_00257b90(int, int);
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void *FUN_L04_002aee30(void *, void *, void *, int);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void FUN_L00_0026ced0(void *, void *, int, int, float, int);
extern void FUN_L12_003064e8_c2(char *) __asm__("FUN_L12_003064e8");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L12_00306350_c2(struct Moby *moby, int slot) __asm__("FUN_L12_00306350");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float func_001F9B88_304310(float) __asm__("FUN_001f99c0");
extern char D_L12_00167240[];
extern int func_001F9908_304920(void *) __asm__("FUN_001f9740");
extern int func_001FA898_3055F8(float) __asm__("FUN_001fa6d0");
extern float func_001FA888_3055F8(int) __asm__("FUN_001fa6c0");
extern float D_0015ED64 __attribute__((section(".sdata")));
extern int D_L12_00161F00_gp __asm__("D_L12_00161F00") __attribute__((section(".sdata")));
extern int D_L12_00161F04_gp __asm__("D_L12_00161F04") __attribute__((section(".sdata")));
extern int D_L12_00161F20 __attribute__((sda));
extern int D_L12_00161F24 __attribute__((sda));
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

void FUN_L12_003054b0(char *moby) {
    char *d = *(char **)(moby + 0x78);
    int i;
    float v0[4];
    float v1[4];
    float v2[4];
    float v3[4];
    float *pv1;
    float *hp;

    for (i = 0; i < 5; i++) {
        int idx;
        char *ent;
        char *m;
        Gun_306890 *md;
        float yaw;
        float dist;
        float dz;
        int flag;
        float ang;
        float s;

        if (*(int *)(d - -(i * 4) + 0x5D0) != -1) {
        idx = *(int *)(d - -(i * 4) + 0x5D0);
        ent = d + idx * 0x30;
        m = (char *)(D_L12_0015FFD8_gp + (*(int *)(ent + 0x20) << 8));
        md = *(Gun_306890 **)(m + 0x78);
        qcopy(v0, D_L12_00167240);
        pv1 = v1;
        v0[2] -= 0.7f;
        qcopy_nc(pv1, m + 0x10);
        v1[2] += 1.5f;
        md->f60 = fast_arc_tan(vector_distance_xy_alt(v1, v0), v0[2] - v1[2]);
        update_joint_animation(m, (char *)md, 0, D_0015ED64 * 0.008f, D_0015ED64 * 0.3f);
        yaw = fast_subtract_rotations(fast_add_rotations(fast_arc_tan(((Hero *)((char *)&hero))->motion.pos.f[0] - *(float *)(m + 0x10),
                                                            ((Hero *)((char *)&hero))->motion.pos.f[1] - *(float *)(m + 0x14)),
                                          -1.5707964f),
                            *(float *)(moby + 0x48));
        if (md->h90 == 0) {
            if (FUN_L00_0025bc00((float *)(ent + 0x18), yaw, 0,
                                  D_0015ED6C_gp * 0.5235988f + D_0015ED6C_gp * 0.17453292f * (float)i) < 0.08726646f) {
                md->h90 = 1;
                md->f8C = 1.0f;
                if (i & 1) {
                    md->f8C = -1.0f;
                }
            }
        } else {
            float a = fast_add_rotations(yaw, md->f8C * 0.55850536f);
            if (func_001F9B88_304310(FUN_L00_0025bc98(ent + 0x18, a, (char *)md + 0x94, 0, D_0015ED64 * 0.022f, D_0015ED64 * 0.3f,
                                                D_0015ED6C_gp * 0.2617994f + (float)i * (D_0015ED6C_gp * 0.05235988f)))
                < 0.017453292f) {
                md->f8C = -md->f8C;
            }
        }
        flag = 0;
        hp = (float *)(((char *)&hero) + 0x80);
        dist = vector_distance_xy_alt(moby + 0x10, hp);
        dz = func_001F9B88_304310(hp[2] - *(float *)(moby + 0x18));
        if (scale_game_frames(0x4B0) < D_L12_00161F34) {
            if (dist < 80.0f && dz < 4.0f) {
                flag = 1;
            }
        }
        if (dist > 90.0f || dz > 7.0f) {
            D_L12_00161F34 = 0;
        }
        if ((D_L12_00161F00_gp != 0 || flag) && md->f80 > 0.0f) {
            fast_dec_timer_s16(&md->h98);
            fast_dec_timer_s16(&md->h9C);
            if (md->h98 == 0) {
                if (fast_difference_between_rotations(*(float *)(d + idx * 0x30 + 0x18), yaw) < 0.06981317f) {
                    md->h9E = scale_game_frames(0x14);
                }
                if (fast_dec_timer_s16(&md->h9E) == 0) {
                    if (FUN_L00_001fefc8(&md->b92) == 0) {
                        if (FUN_L00_001fefc8(&md->b93) != 0) {
                            allocate_voice_for_target_entry(0, 0, m);
                            md->b93 = scale_game_frames(9);
                        }
                    }
                    if (fast_dec_timer_s16(&md->h9C) != 0) {
                        float rz;
                        float pitch;
                        int n;
                        FUN_L00_0024f7c8(m, md->h9A + 1, v0);
                        md->h9A = md->h9A == 0;
                        rz = fast_add_rotations(*(float *)(m + 0x48), 1.5707964f);
                        if (D_L12_00161F00_gp != 0) {
                            qcopy(v2, D_L12_00167240);
                            v2[2] += random_float_between_c(-1.5f, 0.3f);
                        } else {
                            qcopy(v2, &((Hero *)((char *)&hero))->motion.unkD0);
                            v2[2] += FUN_L00_00257c48(0.0f, 0.5f);
                        }
                        n = D_L12_00161F20 - 3;
                        if (scale_game_frames(5) < n) {
                            n = scale_game_frames(5);
                        }
                        if (n < 0) {
                            n = 0;
                        }
                        v2[2] += random_float_between_c((float)n * -0.3f, (float)n * 0.3f);
                        pitch = fast_arc_tan(vector_distance_xy_alt(v0, v2), v2[2] - v0[2]);
                        build_spherical_offset(v1, D_0015ED6C_gp * 80.0f, rz, pitch);
                        if (FUN_L04_002aee30(m, v0, v1, scale_game_frames(0x96)) != 0) {
                            int j;
                            md->b92 = scale_game_frames(0x14);
                            for (j = 0; j < 9; j++) {
                                int c0;
                                int c1;
                                int life;
                                v3[0] = FUN_L00_00257c48(0.0f, D_0015ED6C_gp * 1.7f);
                                v3[1] = FUN_L00_00257c48(0.0f, D_0015ED6C_gp * 1.7f);
                                v3[2] = FUN_L00_00257c48(0.0f, D_0015ED6C_gp * 1.7f);
                                c0 = FUN_L00_00257b90(0xAA, 0xF0);
                                c1 = FUN_L00_00257b90(0x6E, 0xC8);
                                c1 = 0x7F000000 | (c1 << 8) | c0;
                                life = scale_game_frames(FUN_L00_00257b90(0xF, 0x1E));
                                FUN_L00_00269958(v0, v3, c1, 0xFF, life, 0x37, FUN_L00_00257b90(0xB4, 0x10E), 1);
                            }
                        }
                        md->h9C = scale_game_frames(7);
                        n = D_L12_00161F20;
                        if (scale_game_frames(0xB) < n) {
                            n = scale_game_frames(0xB);
                        }
                        if (n < 0) {
                            n = 0;
                        }
                        md->h9C += n;
                    }
                }
            }
        }
        if (D_L12_00161F04_gp == 0 && md->f80 > 0.0f) {
            if (func_001F9908_304920(&md->w84) != 0) {
                float dmg = 1.0f;
                char *r = FUN_L00_0025a420(m, 0x50000, 0);
                if (r == 0) {
                    int k = D_L12_001FB670[i];
                    char *t = 0;
                    if (k == 0x11) {
                        t = moby;
                    } else {
                        int id = *(int *)(d + k * 0x30 + 0x20);
                        if (id != -1) {
                            t = (char *)(D_L12_0015FFD8_gp + (id << 8));
                        }
                    }
                    if (t != 0) {
                        dmg = 0.2f;
                        r = FUN_L00_0025a420(t, 0x50000, 0);
                        ((unsigned char *)t)[0xA4] = 0xFF;
                    }
                    if (r == 0) {
                        goto done;
                    }
                }
                if (D_L12_00161F00_gp == 0) {
                    D_L12_00161F34 = scale_game_frames(0xFA0);
                    goto done;
                }
                md->w84 = scale_game_frames(0x14);
                md->f80 -= dmg;
                if (md->f80 <= 0.0f) {
                    int j;
                    int k;
                    *(short *)(d + 0x5FC) -= 0x14;
                    if (*(short *)(d + 0x5FC) < 0) {
                        *(short *)(d + 0x5FC) = 0;
                    }
                    allocate_voice_for_target_entry(1, 0, m);
                    if (*(short *)(d + 0x5FC) > 0) {
                        int k = D_L12_001FB670[i];
                        if (k != 0x11) {
                            D_L12_001FB688[k] = *(int *)(d + k * 0x30 + 0x20);
                            *(int *)(d + k * 0x30 + 0x20) = -1;
                        } else {
                            D_L12_001FB688[0x11] = 1;
                        }
                        FUN_L12_00306350_c2((struct Moby *)moby, k);
                        if (i == 4) {
                            for (j = 0; j < 0x11; j++) {
                                int id = *(int *)(d + j * 0x30 + 0x20);
                                if (id != -1 && *(short *)((id << 8) + (int)D_L12_0015FFD8_gp + 0xA6) == 0x4FD) {
                                    D_L12_001FB688[j] = id;
                                    *(int *)(d + j * 0x30 + 0x20) = -1;
                                    FUN_L12_00306350_c2((struct Moby *)moby, j);
                                }
                            }
                        }
                    }
                    for (k = 0; k < 0x82; k++) {
                        int c;
                        qcopy(v0, m + 0x10);
                        v0[2] += random_float_between_c(0.1f, 0.5f);
                        ang = random_angle_radians();
                        s = random_float_between_c(0.1f, 0.8f);
                        v0[0] += fast_cos(ang) * s;
                        v0[1] += fast_sin(ang) * s;
                        c = FUN_L00_00257b90(scale_game_frames(0x5A), scale_game_frames(0xB4));
                        s = random_float_between_c(D_0015ED6C_gp * 0.7f, D_0015ED6C_gp * 1.5f);
                        v1[0] = fast_cos(ang) * s;
                        v1[1] = fast_sin(ang) * s;
                        v1[2] = 0.0f;
                        v1[2] = random_float_between_c(D_0015ED6C_gp, D_0015ED6C_gp * 5.5f);
                        FUN_L00_0026ced0(v0, v1, 0x500A1414, 0xA0A0A, 420000.0f, c);
                    }
                    *(int *)(m + 0x94) = 0;
                    *(unsigned short *)(m + 0x34) |= 0x41;
                }
            }
        done:
            ((unsigned char *)m)[0xA4] = 0xFF;
        } else {
            ((unsigned char *)m)[0xA4] = 0xFF;
        }
        if (md->f80 > 0.0f && md->f80 < 10.0f) {
            int lvl = (9 - func_001FA898_3055F8(md->f80)) / 2;
            int j;
            int n = lvl + 3;
            for (j = 0; j < n; j++) {
                int c;
                qcopy(v0, m + 0x10);
                v0[2] += 0.7f;
                ang = random_angle_radians();
                s = random_float_between_c(0.1f, 0.4f);
                v0[0] += fast_cos(ang) * s;
                v0[1] += fast_sin(ang) * s;
                c = FUN_L00_00257b90(scale_game_frames(0x19), scale_game_frames(0x37));
                s = random_float_between_c(D_0015ED6C_gp * 0.5f, D_0015ED6C_gp * 3.0f);
                v1[0] = fast_cos(ang) * s;
                v1[1] = fast_sin(ang) * s;
                v1[2] = 0.0f;
                v1[2] = random_float_between_c(D_0015ED6C_gp * 7.0f, D_0015ED6C_gp * 17.0f) * func_001FA888_3055F8(lvl) * 1.5f
                        / func_001FA888_3055F8(5);
                FUN_L00_0026ced0(v0, v1, (FUN_L00_00257b90(0x46, 0xCD) << 8) | 0x460A00AA, 0xA0A46, 420000.0f, c);
            }
            for (j = 0; j < 3; j++) {
                int c;
                qcopy(v0, m + 0x10);
                ang = random_angle_radians();
                s = random_float_between_c(0.1f, 0.8f);
                v0[0] += fast_cos(ang) * s;
                v0[1] += fast_sin(ang) * s;
                c = FUN_L00_00257b90(scale_game_frames(0x28), scale_game_frames(0x46));
                s = random_float_between_c(D_0015ED6C_gp * 0.1f, D_0015ED6C_gp * 0.4f);
                v1[0] = fast_cos(ang) * s;
                v1[1] = fast_sin(ang) * s;
                v1[2] = 0.0f;
                v1[2] = random_float_between_c(D_0015ED6C_gp * 2.4f, D_0015ED6C_gp * 5.5f);
                FUN_L00_0026ced0(v0, v1, 0x460A1414, 0xA0A0A, 504000.03f, c);
            }
        }
        }
    }
    FUN_L12_003064e8_c2(moby);
    if (*(short *)(d + 0x5FC) <= 0 && D_L12_00161F04_gp == 0) {
        D_L12_00161F24 = 1;
        moby[0x20] = 4;
        moby[0xBC] = 0;
    }
}
extern int D_L12_001FB688[];
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");

void FUN_L12_00306350(struct Moby *moby, int slot) {
    char *data = (char *)moby->pvars;
    struct Moby *selected;
    float *speed;
    int offset;
    float initial_speed;
    float angle_scale;
    float first_angle;
    float second_scale;
    char *slot_data;

    if (slot == 0x11) {
        selected = moby;
        offset = 0x44;
    } else {
        selected = (struct Moby *)(D_L12_0015FFD8_p + D_L12_001FB688[slot] * 0x100);
        offset = slot * 4;
    }

    initial_speed = random_float_between(frame_time_sq * 3.7f, frame_time_sq * 5.0f);
    angle_scale = frame_time;
    {
        char *base = data + 0x570;
        speed = (float *)(base + offset);
    }
    *speed = initial_speed;
    first_angle = randf_sym(angle_scale * 0.05235988f, angle_scale * 0.13962634f);
    second_scale = frame_time;
    slot_data = data + slot * 0x10;
    *(float *)(slot_data + 0x450) = first_angle;
    *(float *)(slot_data + 0x454) = randf_sym(second_scale * 0.05235988f, second_scale * 0.17453292f);
    if (selected->oclass == 0x4fd) {
        float variation = random_float_between(0.001f, 0.0045f);
        float speed_scale = frame_time_sq;
        *(float *)(slot_data + 0x45c) = variation;
        *speed = random_float_between(speed_scale * 5.5f, speed_scale * 9.0f);
    } else if (selected->oclass == 0x500) {
        *speed = frame_time_sq * 13.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003078C8), where it is exact; names translated to the US level program. */

extern float D_L12_00161F40;
extern float D_L12_00161F50;
extern float D_L12_00161F60;
extern float D_L12_00161F70;
extern float FUN_L00_0025bc98(void *, float, void *, int, float, float, float);
extern float fast_add_rotations_c4(float, float) __asm__("func_001FA580");
extern void FUN_L00_00250df8_c(void *) __asm__("FUN_L00_00250df8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");

void FUN_L12_003064e8(char *moby) {
    int i;
    int off;
    char *data = *(char **)(moby + 0x78);
    int last = 17;
    for (i = 0, off = 0; i < 18; i++, off += 4) {
        int s = D_L12_001FB688[i];
        char *obj;
        float f1;
        if (s == 0) {
            continue;
        }
        if (i == last) {
            obj = moby;
        } else {
            if (s == -1) {
                continue;
            }
            obj = (char *)(D_L12_0015FFD8_gp + (s << 8));
        }
        if (i == last) {
            f1 = D_L12_00161F40;
            if (f1 == -1.0f) {
                qcopy((char *)&D_L12_00161F40, obj + 0x10);
                qcopy((char *)&D_L12_00161F50, obj + 0x40);
            }
            qcopy((char *)&D_L12_00161F60, obj + 0x10);
            qcopy((char *)&D_L12_00161F70, obj + 0x40);
            qcopy(obj + 0x10, (char *)&D_L12_00161F40);
            qcopy(obj + 0x40, (char *)&D_L12_00161F50);
            f1 = *(float *)(obj + 0x18);
        } else {
            f1 = *(float *)(obj + 0x18);
        }
        if (f1 < 20.0f) {
            if (i == last) {
                return;
            }
            mark_moby_for_removal_c(obj);
            D_L12_001FB688[i] = -1;
        }
        f1 = *(float *)(data + 0x338 + i * 16) - *(float *)(data + off + 0x570);
        *(float *)(data + 0x338 + i * 16) = f1;
        *(float *)(obj + 0x18) = *(float *)(obj + 0x18) + f1;
        if (*(short *)(obj + 0xA6) == 0x500) {
            *(float *)(obj + 0x40) = fast_add_rotations_c4(*(float *)(obj + 0x40), D_0015ED6C * 0.34906584f);
            FUN_L00_0025bc98(obj + 0x44, 1.5707964f, data + 0x454 + i * 16, 0, 0.00200000009f, 0.3f, 0.0f);
        } else if (*(short *)(obj + 0xA6) == 0x4FD) {
            *(float *)(obj + 0x40) = fast_add_rotations_c4(*(float *)(obj + 0x40), *(float *)(data + 0x450 + i * 16));
            FUN_L00_0025bc98(obj + 0x44, -1.5707964f, (float *)(data + 0x450 + i * 16) + 1, 0, *(float *)(data + 0x45C + i * 16), 0.3f, 0.0f);
        } else if (*(short *)(obj + 0xA6) == 0x4FA) {
            float t;
            *(float *)(obj + 0x40) = fast_add_rotations_c4(*(float *)(obj + 0x40), D_0015ED6C * -0.87266463f);
            t = fast_add_rotations_c4(*(float *)(obj + 0x44), *(float *)(data + 0x454 + i * 16));
            *(float *)(obj + 0x14) = *(float *)(obj + 0x14) + D_0015ED6C * -4.0f;
            *(float *)(obj + 0x44) = t;
        } else {
            *(float *)(obj + 0x14) = *(float *)(obj + 0x14) + D_0015ED6C * -4.0f;
            *(float *)(obj + 0x40) = fast_add_rotations_c4(*(float *)(obj + 0x40), *(float *)(data + 0x450 + i * 16));
            *(float *)(obj + 0x44) = fast_add_rotations_c4(*(float *)(obj + 0x44), *(float *)(data + 0x454 + i * 16));
        }
        if (i == last) {
            qcopy((char *)&D_L12_00161F40, obj + 0x10);
            qcopy((char *)&D_L12_00161F50, obj + 0x40);
        }
        FUN_L00_00250df8_c(obj);
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00307C08), where it is exact; names translated to the US level program. */

extern int D_L12_0015F5CC;
extern unsigned char * D_L12_0015FFD8;

void FUN_L12_00306828(struct Moby *moby) {
    int i;
    int alpha = 0x80000000;
    char *data = (char *)moby->pvars;
    for (i = 0; i < 18; i++) {
        struct Moby *other;
        int period;
        float sine;
        float color;
        int red;
        int green;
        int blue;
        int packed_color;
        float phase;
        float divisor;
        if (i == 17) {
            other = moby;
        } else {
            other = (struct Moby *)((char *)(*(struct Moby * *)&D_L12_0015FFD8_p) + ((unsigned int)*(int *)(data + i * 0x30 + 0x20) << 8));
        }
        if (other->oclass == 0x4FE) continue;
        {
            period = scale_game_frames(0xAA);
            divisor = convert_integer_to_float(period);
            phase = (float)(D_L12_0015F5CC % period) / divisor;
            sine = fast_sin((phase + phase) * 3.1415927f + -3.1415927f);
            color = sine * 70.0f;
            red = truncate_float_to_s32(color);
            green = truncate_float_to_s32(color);
            blue = truncate_float_to_s32(sine * 10.0f);
            packed_color = ((blue + 0x46) << 16) | alpha |
                ((green + 0xB4 > 0xFF ? 0xFF : green + 0xB4) << 8) |
                (red + 0xB4 > 0xFF ? 0xFF : red + 0xB4);
            other->flags |= 0x10;
            other->glow_color = packed_color;
        }
    }
    /* Fakematch: keeping alpha live past the loop gives retail's schedule of the loop body. */
    alpha++;
    alpha--;
}
/* Planetbuster part update: rides its path, drops its parts and hands its voice slot back when done. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00307DB0), where it is exact; names translated to the US level program. */

typedef struct { float v[4]; } P_7DB0;

typedef struct { char pad[0x80]; float f80; } F80_7DB0;

typedef struct { char pad[0x34]; unsigned short flags; } M34_7DB0;

typedef struct { unsigned char pad0[0x74]; unsigned char active; unsigned char pad75[0x13]; char *owner; } Voice_7DB0;

extern char D_0013E550[];
extern char *D_L12_001600EC __attribute__((section(".sdata")));
extern char D_0013F3D0[] __attribute__((section(".data")));
extern char D_L12_00167240[];
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 fast_arc_tan(f32, f32) __asm__("func_001F9E90");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float D_0015ED64 __attribute__((section(".sdata")));
extern float D_0015ED6C_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_0015ED70 __attribute__((section(".sdata")));
extern float D_L12_00161F40_gp __asm__("D_L12_00161F40") __attribute__((section(".sdata")));
extern float D_L12_00161F60_gp __asm__("D_L12_00161F60") __attribute__((section(".sdata")));
extern float D_L12_00161F70_gp __asm__("D_L12_00161F70") __attribute__((section(".sdata")));
extern float approach_value(float *, float, float);
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int D_L12_001619A0 __attribute__((section(".sdata")));
extern int D_L12_001619B4 __attribute__((section(".sdata")));
extern int D_L12_00161F00;
extern int D_L12_00161F24 __attribute__((section(".sdata")));
extern int D_L12_00161F34 __attribute__((sda));
extern int D_L12_001B0930;
extern int D_L12_0015F404 __attribute__((section(".sdata")));
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");

extern float D_L12_00161F28 __attribute__((sda));
extern float D_L12_00161F2C __attribute__((sda));
extern float D_L12_00161F30 __attribute__((sda));
#include "rnc/gameplay/state/game_globals.h"
extern void FUN_L00_00258278(char *moby, float target, float *vel, float k, float d, float max);
extern void vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void FUN_L00_00262b00(float, float, char *, float *, float *);
extern void FUN_L00_00262b80(void *, char *, char *, float, float, float);
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void FUN_L00_002eac18(int);
extern void FUN_L12_0027b268(char *, void *, int);
extern void FUN_L12_0027b370(char *, char *, int);
extern void FUN_L12_003064e8_c(char *) __asm__("FUN_L12_003064e8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void lerp_vector(void *, void *, void *, f32) __asm__("func_001F9A40");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_L12_003054b0_u(void *moby) __asm__("FUN_L12_003054b0");
void FUN_L12_00306350_c(struct Moby *moby, int slot) __asm__("FUN_L12_00306350");
extern int current_level_index_gp __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern char D_0013F3D0[];
extern int D_L12_00161F00_gp __asm__("D_L12_00161F00") __attribute__((section(".sdata")));
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int D_L12_001B0930;

void planetbuster_maximus_part_update(char *moby) __asm__("FUN_L12_003069d0");
void planetbuster_maximus_part_update(char *moby) {
    float v[4];
    float w[4];
    char *d = *(char **)(moby + 0x78);
    if (((unsigned char *)moby)[0x20] != 0 && D_L12_001FB688[17] != 0) {
        qcopy(moby + 0x10, (char *)&D_L12_00161F60_gp);
        qcopy(moby + 0x40, (char *)&D_L12_00161F70_gp);
    }
    switch ((unsigned char)moby[0x20]) {
    case 0:
        D_L12_001619B4 = 0;
        if (visited52[(unsigned char)moby[0xB0] + (current_level_index_gp << 4)] == 0xFF) {
            int i;
            for (i = 0; i < 17; i++) {
                int s = *(int *)(d + i * 0x30 + 0x20);
                if (s != -1) mark_moby_for_removal_c2((char *)D_L12_0015FFD8_gp + (s << 8));
            }
            mark_moby_for_removal_c2(moby);
            return;
        } else {
            P_7DB0 *path;
            int i;
            int j;
            D_L12_00161F24 = 0;
            FUN_L12_0027b268(moby, d, 0x11);
            path = ((P_7DB0 * *)&D_L12_001B0930)[*(int *)(d + 0x5C0)];
            qcopy(moby + 0x10, path + 1);
            *(float *)(d + 0x618) = *(float *)(moby + 0x18);
            *(float *)(moby + 0x48) = fast_arc_tan(path[2].v[0] - path[1].v[0], path[2].v[1] - path[1].v[1]);
            if (*(float *)(D_0013F3D0 + 0x8) < 52.0f) {
                *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + 3.0f;
            }
            for (i = 0; i < 5; i++) {
                int k = *(int *)(d + 0x5D0 + i * 4);
                char *o = (char *)D_L12_0015FFD8_gp + (*(int *)(d + k * 0x30 + 0x20) << 8);
                (*(F80_7DB0 **)(o + 0x78))->f80 = 10.0f;
            }
            D_L12_00161F40_gp = -1.0f;
            for (j = 17; j >= 0; j--) D_L12_001FB688[j] = 0;
            *(int *)(d + 0x604) = -1;
            *(short *)(d + 0x5FC) = 100;
            moby[0x20] = 1;
        }
        break;
    case 1: {
        char *h;
        int r;
        if (is_point_inside_clip_volume_c(D_L12_00167240, *(int *)(d + 0x624))) {
            int i;
            char *base = (char *)D_L12_0015FFD8_gp;
            *(unsigned short *)(moby + 0x34) |= 0x41;
            for (i = 0; i < 17; i++) {
                int s = *(int *)(d + i * 0x30 + 0x20);
                if (s != -1) ((M34_7DB0 *)(base + (s << 8)))->flags |= 0x41;
            }
        } else {
            int i;
            char *base = (char *)D_L12_0015FFD8_gp;
            *(unsigned short *)(moby + 0x34) &= ~0x41;
            for (i = 0; i < 17; i++) {
                int s = *(int *)(d + i * 0x30 + 0x20);
                if (s != -1) ((M34_7DB0 *)(base + (s << 8)))->flags &= ~0x41;
            }
        }
        h = D_0013F3D0;
        r = is_point_inside_clip_volume_c(h, *(int *)(d + 0x628));
        if (*(float *)(h + 8) > 52.0f && r == 0) {
            approach_value((float *)(moby + 0x18), *(float *)(d + 0x618), D_0015ED6C_gp + D_0015ED6C_gp);
        } else {
            float add = 3.0f;
            if (r) add = 10.0f;
            if (is_point_inside_clip_volume_c(D_0013F3D0, *(int *)(d + 0x61C))) {
                *(int *)(d + 0x620) = scale_game_frames(0x5A);
            }
            if (!tick_countdown_32_alt((int *)(d + 0x620))) add = 19.0f;
            vendor_spring((float *)(moby + 0x18), (float *)(d + 0x62C), *(float *)(d + 0x618) + add,
                              D_0015ED64 * 0.03f, D_0015ED64 * 0.3f, D_0015ED6C_gp * 2.7f);
        }
        FUN_L00_00262b00(0.7f, D_0015ED6C_gp * 1.2217305f, moby, (float *)(d + 0x5CC), (float *)(d + 0x5C8));
        FUN_L00_00262b80(moby, d + 0x5E4, d + 0x5E8, D_L12_00161F30,
                          D_L12_00161F28 * 0.017453292f * D_0015ED6C_gp,
                          D_L12_00161F2C * 0.017453292f * D_0015ED6C_gp);
        FUN_L12_0027b370(moby, d, 0x11);
        if (is_point_inside_clip_volume_c(D_0013F3D0, *(int *)(d + 0x5EC)) || D_L12_00161F00_gp) {
            int i;
            D_L12_001619B4 = 1;
            for (i = 0; i < 5; i++) {
                int k = *(int *)(d + 0x5D0 + i * 4);
                ((unsigned char *)D_L12_0015FFD8_gp + (*(int *)(d + k * 0x30 + 0x20) << 8))[0xA4] = 0xFF;
            }
            moby[0x20] = 2;
        }
        break;
    }
    case 2: {
        P_7DB0 *path;
        int nx;
        float f;
        FUN_L00_00262b00(0.7f, D_0015ED6C_gp * 1.2217305f, moby, (float *)(d + 0x5CC), (float *)(d + 0x5C8));
        FUN_L00_00262b80(moby, d + 0x5E4, d + 0x5E8, D_L12_00161F30,
                          D_L12_00161F28 * 0.017453292f * D_0015ED6C_gp,
                          D_L12_00161F2C * 0.017453292f * D_0015ED6C_gp);
        path = ((P_7DB0 * *)&D_L12_001B0930)[*(int *)(d + 0x5C0)];
        if (*(short *)(d + 0x5F8) < *(int *)path - 11) {
            approach_value((float *)(d + 0x5F0), D_0015ED6C_gp * 8.0f, D_0015ED70 * 10.0f);
        } else {
            approach_value((float *)(d + 0x5F0), 0.0f, D_0015ED70 * 8.0f);
            if (*(float *)(d + 0x5F0) == 0.0f) {
                moby[0x20] = 3;
                break;
            }
        }
        nx = (*(short *)(d + 0x5F8) + 1) % *(int *)path;
        subtract_vector_xyz(v, &path[nx + 1], &path[*(short *)(d + 0x5F8) + 1]);
        fast_vec_normalize(v, v, *(float *)(d + 0x5F0));
        add_vector_xyz(moby + 0x10, moby + 0x10, v);
        f = fast_arc_tan(*(float *)((char *)path + (nx << 4) + 0x10) - *(float *)((char *)path + (*(short *)(d + 0x5F8) << 4) + 0x10),
                              *(float *)((char *)path + (nx << 4) + 0x14) - *(float *)((char *)path + (*(short *)(d + 0x5F8) << 4) + 0x14));
        if (*(int *)path - 20 < *(short *)(d + 0x5F8)) f = 3.0543261f;
        FUN_L00_00258278(moby, f, (float *)(d + 0x5F4), D_0015ED64 * 0.0015f, D_0015ED64 * 0.3f, D_0015ED6C_gp * 0.17453292f);
        if (vector_distance_xy_alt(moby + 0x10, &path[*(short *)(d + 0x5F8) + 1]) < 5.0f
            || fast_difference_between_rotations(*(float *)(moby + 0x48),
                             fast_arc_tan(*(float *)((char *)path + (*(short *)(d + 0x5F8) << 4) + 0x10) - *(float *)(moby + 0x10),
                                               *(float *)((char *)path + (*(short *)(d + 0x5F8) << 4) + 0x14) - *(float *)(moby + 0x14))) > 1.5707964f) {
            *(short *)(d + 0x5F8) = (*(short *)(d + 0x5F8) + 1) % *(int *)path;
        }
        FUN_L12_0027b370(moby, d, 0x11);
        FUN_L12_003054b0_u(moby);
        break;
    }
    case 3:
        D_L12_00161F34++;
        FUN_L00_0025bc98(moby + 0x48, 3.0543261f, d + 0x5F4, -1, D_0015ED64 * 0.0015f, D_0015ED64 * 0.3f, D_0015ED6C_gp * 0.17453292f);
        FUN_L00_00262b00(0.7f, D_0015ED6C_gp * 1.2217305f, moby, (float *)(d + 0x5CC), (float *)(d + 0x5C8));
        FUN_L00_00262b80(moby, d + 0x5E4, d + 0x5E8, D_L12_00161F30,
                          D_L12_00161F28 * 0.017453292f * D_0015ED6C_gp,
                          D_L12_00161F2C * 0.017453292f * D_0015ED6C_gp);
        FUN_L12_0027b370(moby, d, 0x11);
        FUN_L12_003054b0_u(moby);
        break;
    case 4:
        if (*(int *)(d + 0x608) != -1 && *(int *)(d + 0x60C) != -1) {
            switch ((unsigned char)moby[0xBC]) {
            case 0:
                moby[0xBC] = 1;
                FUN_L00_002ea9d8(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x30);
                FUN_L00_002eaa30(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x70);
                D_L12_0015F404 = 1;
                D_L12_001619A0 = 1;
                break;
            case 1:
                if (*(float *)(d + 0x610) < 1.0f) {
                    advance_accelerated_scalar((float *)(d + 0x610), (float *)(d + 0x614), 1.0f,
                                  D_0015ED70 * 0.666f, D_0015ED70 * 0.666f, D_0015ED6C_gp * 0.4f);
                    lerp_vector(v, D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x30,
                                  D_L12_001600EC + *(int *)(d + 0x60C) * 128 + 0x30, *(float *)(d + 0x610));
                    clear_u64_value(w);
                    w[0] = fast_subtract_rotations(*(float *)(D_L12_001600EC + *(int *)(d + 0x60C) * 128 + 0x70),
                                         *(float *)(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x70)) * *(float *)(d + 0x610);
                    w[0] = fast_add_rotations(*(float *)(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x70), w[1]);
                    w[1] = fast_subtract_rotations(*(float *)(D_L12_001600EC + *(int *)(d + 0x60C) * 128 + 0x74),
                                         *(float *)(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x74)) * *(float *)(d + 0x610);
                    w[1] = fast_add_rotations(*(float *)(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x74), w[1]);
                    w[2] = fast_subtract_rotations(*(float *)(D_L12_001600EC + *(int *)(d + 0x60C) * 128 + 0x78),
                                         *(float *)(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x78)) * *(float *)(d + 0x610);
                    w[2] = fast_add_rotations(*(float *)(D_L12_001600EC + *(int *)(d + 0x608) * 128 + 0x78), w[2]);
                    FUN_L00_002ea9d8(v);
                    FUN_L00_002eaa30(w);
                } else {
                    moby[0xBC] = 2;
                    D_L12_0015F404 = 0;
                    FUN_L00_002eac18(3);
                }
                break;
            case 2: {
                int i;
                for (i = 0; i < 17; i++) {
                    int s = *(int *)(d + i * 0x30 + 0x20);
                    if (s != -1) {
                        char *o = (char *)D_L12_0015FFD8_gp + (s << 8);
                        if (o[0x20] >= 0) mark_moby_for_removal_c2(o);
                        *(int *)(d + i * 0x30 + 0x20) = -1;
                    } else if (D_L12_001FB688[i] > 0) {
                        char *o = (char *)D_L12_0015FFD8_gp + (D_L12_001FB688[i] << 8);
                        if (o[0x20] >= 0) mark_moby_for_removal_c2(o);
                    }
                }
                D_L12_001619B4 = 0;
                start_scene(7);
                mark_moby_for_removal_c2(moby);
                return;
            }
            }
        }
        if (*(short *)(d + 0x5FA) == 0) {
            int i;
            for (i = 0; i < 17; i++) {
                if (D_L12_001FB688[i] == 0) D_L12_001FB688[i] = *(int *)(d + i * 0x30 + 0x20);
                *(int *)(d + i * 0x30 + 0x20) = -1;
            }
            D_L12_001FB688[17] = 1;
            for (i = 0; i < 18; i++) FUN_L12_00306350_c((struct Moby *)moby, i);
        }
        *(short *)(d + 0x5FA) = *(short *)(d + 0x5FA) + 1;
        FUN_L12_003064e8_c(moby);
        break;
    }
    FUN_L12_00306828(moby);
    if ((unsigned char)moby[0x20] >= 1 && (unsigned char)moby[0x20] <= 3) {
        if (*(int *)(d + 0x604) == -1) *(int *)(d + 0x604) = allocate_voice_for_target_entry(0, 4, moby);
    } else {
        int s = *(int *)(d + 0x604);
        if (s != -1) {
            Voice_7DB0 *e = (Voice_7DB0 *)(((unsigned char *)D_0013E550) + s * 0x70);
            if (e->owner == moby && e->active != 0) release_voice_slot(s);
        }
        *(int *)(d + 0x604) = -1;
    }
}
#include "qcopy.h"

/* Spawns the next linked moby at this one's spot, sets it up and gives it a random spin. */
extern void FUN_L00_00250df8(unsigned char *);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_0026de90(char *, int, float);
extern short *suck_get_aim_height(unsigned char *) __asm__("FUN_002141f8");

void FUN_L12_003076e0(struct Moby *m) {
    char *d = (char *)m->pvars;
    int *s = (int *)(d + (*(int *)(d + 0xF0) << 2));
    struct Moby *o = (struct Moby *)((unsigned char *)D_L12_0015FFD8_p + (s[0x30] << 8));
    struct MobyClass *p;
    short *r;
    int i;
    qcopy(&o->pos, &m->pos);
    o->pos.z -= 0.35f;
    o->state = 0;
    o->command = 0;
    p = o->pclass;
    o->flags = p->flags;
    o->scale = p->scale;
    o->update_dist = 0xFF;
    o->visible = 1;
    o->occlusion = 0x7F80;
    o->draw_dist = 0xFF;
    o->unk71 = 0xFF;
    o->unk72 = 0xFF;
    o->hit_slot = 0xFF;
    o->collision = p->collision;
    o->spawn_frame = m->spawn_frame;
    FUN_L00_00250df8(o);
    o->parent = m;
    for (i = 0; i < 14; i++) {
        FUN_L00_0026de90(&m->pos, 0, random_float_between(frame_time * 3.0f, frame_time * 5.0f));
    }
    r = suck_get_aim_height(o);
    if (r != 0) {
        *(float *)r = r[2];
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Door/switch moby: clears its linked mobys, waits for the hero, opens, fires its list one by one, closes. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308C20), where it is exact; names translated to the US level program. */

extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
void FUN_L12_003076e0(struct Moby *m);
extern void FUN_L12_00307ab8(struct Moby *m);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void homing_mine_spawner_update(struct Moby *m) __asm__("FUN_L12_00307840");
void homing_mine_spawner_update(struct Moby *m) {
    char *d = (char *)m->pvars;
    int *slots;
    int i;
    FUN_L12_00307ab8(m);
    slots = (int *)(d + 0xC0);
    switch (m->state) {
    case 0:
        for (i = 0; i < 12; i++) {
            if (slots[i] != -1) {
                char *o = (D_L12_0015FFD8_p + (slots[i] << 8));
                if (o != 0 && ((unsigned char *)o)[0x20] != 0xFE &&
                    ((unsigned char *)o)[0x20] != 0xFD) {
                    mark_moby_for_removal(o);
                }
            }
        }
        m->state = 1;
        break;
    case 1:
        if (*(int *)(d + 0xB4) != 2) {
            if (vector_distance(&m->pos, d + 0x70) < *(float *)(d + 0xF8)) {
                m->state = 2;
            }
        }
        break;
    case 2:
        if (m->prev_seq != 1) {
            blend_moby_animation(m, 1, 0, scale_game_frames(2));
        }
        if (m->anim_flags & 2) {
            if (m->prev_seq != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(2));
            }
            m->state = 3;
        }
        break;
    case 3:
        if (fast_dec_timer_s16(d + 0xFC)) {
            if (slots[*(int *)(d + 0xF0)] != -1) {
                FUN_L12_003076e0(m);
                m->command = m->command == 0;
                *(short *)(d + 0xFC) = scale_game_frames(0x28);
            }
            *(int *)(d + 0xF0) += 1;
        }
        if (*(int *)(d + 0xF0) == 12 || slots[*(int *)(d + 0xF0)] == -1) {
            m->state = 4;
        }
        break;
    case 4:
        m->flags &= 0xEFFF;
        if (m->prev_seq != 3) {
            blend_moby_animation(m, 3, 0, scale_game_frames(2));
        }
        if (m->anim_flags & 2) {
            if (m->prev_seq != 0) {
                blend_moby_animation(m, 0, 0, scale_game_frames(2));
            }
            m->state = 5;
        }
        break;
    case 5:
        break;
    }
}
#include "qcopy.h"

/* per-frame update of a moby's timer and approach state */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308E98), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_0025fcb8(char *, char *, float);
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L12_00307ab8(struct Moby *m) {
    char *d = (char *)m->pvars;
    char *pad;
    if (m->state != 0) {
        if (*(int *)(d + 0x38) != 0) {
            *(short *)(d + 0xFE) = scale_game_frames(0xB4);
        }
        *(int *)(d + 0x38) = 0;
        if (fast_dec_timer_s16(d + 0xFE)) {
            *(float *)(d + 0xF8) = *(float *)(d + 0xF4);
        } else {
            *(float *)(d + 0xF8) = *(float *)(d + 0xF4) + 5.0f;
        }
        if (FUN_L00_0025fcb8(m, d + 0x70, *(float *)(d + 0xF8)) != 2) {
            if (absolute_float(m->pos.z - *(float *)(d + 0x78)) > 3.0f) {
                *(int *)(d + 0xB4) = 2;
            }
        }
        if (*(int *)(d + 0xB0) == 0) {
            pad = D_0013E533 + 0xE1D;
            *(int *)(d + 0xB0) = *(int *)(pad + 0x2080);
            qcopy(d + 0x70, pad + 0x80);
        }
    }
}
#include "rnc/gameplay/entities/moby.h"

#include "rnc/gameplay/state/game_globals.h"

extern int FUN_L01_00275690(void *, float);
extern int FUN_0022da68(int, int, void *);
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void FUN_L12_00308350(struct Moby *);

/* Pvars: d[0] is the moby to watch, d[1] the voice slot held for this moby. */
void FUN_L12_003081b0(struct Moby *m) {
    int st = m->state;
    int vis;
    int *d = (int *)m->pvars;

    switch (st) {
    case 0:
        if (m->mission != 0xFF && visited52[m->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(m);
            break;
        }
        m->state = 1;
        break;
    case 1:
        vis = 1;
        if (FUN_L01_00275690(m, 48.0f) == -1) {
            vis = 0;
        }
        if (vis) {
            enqueue_callback_list_1(FUN_L12_00308350, m);
        }
        if (d[1] == -1) {
            d[1] = FUN_0022da68(0, 4, m);
        }
        if (d[0] >= 0) {
            int i = d[1];
            if (i != -1) {
                char *e = ((char *)&voice_pool) + i * 0x70;
                if (*(struct Moby **)(e + 0x88) == m && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(i);
                }
            }
            d[1] = -1;
            if (((unsigned char *)D_L12_0015FFD8_p)[(d[0] << 8) + 0x20] == 7) {
                if (m->mission != 0xFF) {
                    jet_clear_persistent(m->mission);
                }
                FUN_0022da68(1, 0, m);
                m->collision = 0;
                m->state = 2;
            }
        }
        break;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L12_00308350.s", FUN_L12_00308350);
#else
#include "eetypes.h"

typedef struct {
    u128 vertices[4];
    int color[4];
    float uv[4][2];
    long tag;
    long texture;
    long texture_state;
    long gs_state;
} VendorDrawPacket;

extern int D_L12_00161FBC __attribute__((sda));
extern int D_L12_00161FC0 __attribute__((sda));
extern int D_L12_00161FC4 __attribute__((sda));
extern int D_L12_00161FC8 __attribute__((sda));
extern int D_L12_00161FCC __attribute__((sda));
extern int D_L12_00161FD0 __attribute__((sda));
extern int D_L12_00161FD4 __attribute__((sda));
extern int D_L12_00161FD8 __attribute__((sda));
extern unsigned int D_L12_00161F90 __attribute__((sda));
extern unsigned int D_L12_00161F94 __attribute__((sda));
extern float D_L12_001FB6D0[4][2];
extern u128 D_L12_001FB6F0[4];
extern u128 D_L12_001FB730[4];
extern void copy_moby_matrix(void *, void *) __asm__("FUN_001fa298");
extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad_l12(void *, void *, int) __asm__("FUN_001f7d30");

void FUN_L12_00308350(struct Moby *m) {
    VendorDrawPacket first, second;
    float matrix[4][4] __attribute__((aligned(16)));
    void *matrix_ptr;
    int i, j;
    int vertex;
    long flags, second_flags;

    matrix_ptr = matrix;
    copy_moby_matrix(matrix, &m->rot_row0);
    qcopy(&matrix[3], &m->pos);
    matrix[3][3] = 1.0f;
    first.texture = second.texture = get_effect_texture(0xe);
    first.texture_state = second.texture_state = 0xFF9000000260L;
    first.gs_state = (long)D_L12_00161FBC | ((long)D_L12_00161FC0 << 2) | ((long)D_L12_00161FC4 << 4) | ((long)D_L12_00161FC8 << 6) | 0x8000000000L;
    second.gs_state = (long)D_L12_00161FCC | ((long)D_L12_00161FD0 << 2) | ((long)D_L12_00161FD4 << 4) | ((long)D_L12_00161FD8 << 6) | 0x8000000000L;
    second.tag = 0;
    first.tag = 0;
    first.color[0] = first.color[1] = first.color[2] = first.color[3] = D_L12_00161F90;
    second.color[0] = second.color[1] = second.color[2] = second.color[3] = D_L12_00161F94;
    vertex = 0;
    do {
        float x, y;
        qcopy(&second.vertices[vertex], &D_L12_001FB6F0[vertex]);
        qcopy(&first.vertices[vertex], &D_L12_001FB730[vertex]);
        x = D_L12_001FB6D0[vertex][0];
        y = D_L12_001FB6D0[vertex][1];
        first.uv[vertex][0] = x;
        ((float *)first.uv)[vertex * 2 + 1] = y;
        second.uv[vertex][0] = x;
        second.uv[vertex][1] = y;
        vertex++;
    } while (vertex < 4);
    i = 0;
    do {
        float x_offset;
        float jitter;
        int row;
        u128 *first_vertex;
        u128 *second_vertex;
        x_offset = random_float_between(-0.5f, 0.5f);
        first_vertex = first.vertices;
        second_vertex = second.vertices;
        for (j = 0; j < 4; j++) {
            ((float *)&first.vertices[j])[2] += 1.0f;
            ((float *)&second_vertex[j])[2] += 1.0f;
            first.uv[j][0] = D_L12_001FB6D0[j][0] + x_offset;
            first.uv[j][1] = D_L12_001FB6D0[j][1];
        }
        jitter = 0.0f;
        for (row = -3; row < 3; row++) {
            float x = (float)row + jitter;
            ((float *)&first.vertices[0])[1] = x;
            ((float *)&first.vertices[1])[1] = x;
            jitter = row < 4 ? random_float_between(-0.2f, 0.2f) : 0.0f;
            x = (float)(row + 1) + jitter;
            ((float *)&first.vertices[2])[1] = x;
            ((float *)&first.vertices[3])[1] = x;
            draw_geometry_quad_l12(&first, matrix, 0);
        }
        i++;
    } while (i < 5);
}
#endif
#include "sda.h"

/* Vendor-counter moby update: once the shared block is ready, latch its data and copy two vectors. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030A7A8), where it is exact; names translated to the US level program. */

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

extern char D_0013D388[];
extern char D_L12_0016CCF0[];
extern int D_L12_0015F5C4;
extern int D_L12_0015F5CC;
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

void FUN_L12_003093c8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *base;
    char *p;
    char *q;
    if (D_L12_0015F5CC != 0 && D_L12_0015F5C4 == 0) {
        base = D_0013D388;
        if (*(unsigned char *)(*(int *)(data + 4) + base + 0x5C) != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        p = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(p, *(int *)data) != 0) {
            *(char *)(*(int *)(data + 4) + base + 0x5C) = 1;
            qcopy(D_L12_0016CCF0, p);
            q = D_L12_0016CCF0 - 0x10;
            *(float *)(q + 0x18) = *(float *)(p + 0x258);
            qcopy(D_L12_0016CCF0 + 0x10, p + 0x10);
            *(short *)(q + 0x46) = 1;
            start_scene(*(int *)(data + 8));
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L12_003094a0.s", FUN_L12_003094a0);
#else
typedef struct {
    char pad0[0x30];
    int phase;
    char pad34[0x144];
    int mobys[1];
} L12VendorMobyList;
extern L12VendorMobyList D_L12_0016CCE0;
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void FUN_L12_003094a0(struct Moby *moby) {
    int state = moby->state;
    int phase;
    int index;

    switch (state) {
    case 0:
        moby->update_dist = 0xff;
        moby->state = 1;
        break;
    case 1:
        if (D_L12_0015F5C4 != 2) break;

        phase = D_L12_0016CCE0.phase;
        switch (phase) {
        case 7: index = 0; break;
        case 1: index = 2; break;
        default: return;
        }
        cutscene_set_camera_target(D_L12_0016CCE0.mobys[index]);
        break;
    }
}
#endif

#define NOT_SDA

#define MACRO_ADDR

/* Draws the moby's 10-piece overlay when its trigger is visible, with its own GS state. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030D248), where it is exact; names translated to the US level program. */

extern float D_L12_00162108[2] __attribute__((section(".sdata")));
extern char D_L12_00208B00[];
extern int D_L12_001620D4;
extern int D_L12_001FBD10[];
extern int D_L12_002059E0[];
extern int D_L12_00205A08[];
extern int D_L12_00205A58[];
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);
extern void FUN_L08_002f0c18(int);
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void FUN_L12_0030be68(struct Moby *m) {
    int *d = (int *)m->pvars;
    int i;
    font_queue_vu_state();
    vu1_add_g_sregister(0x42, 0x7F00000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x5360A);
    if (is_point_inside_clip_volume(D_L12_00167240, d[0])) {
        vu1_add_g_sregister(6, get_effect_texture(0x2C));
        for (i = 0; i < 10; i++) {
            FUN_L08_00258090(D_L12_00208B00, D_L12_00205A08[i], D_L12_001FBD10[i], D_L12_00162108);
            FUN_L00_001fde98(D_L12_001FBD10[i], D_L12_002059E0[i], D_L12_00205A58[i],
                             D_L12_00208B00, 1);
        }
    }
    vu1_add_g_sregister(6, get_effect_texture(0x2F));
    FUN_L08_002f0c18(0);
    vu1_add_g_sregister(0x42, ((long)D_L12_001620D4 << 32) | 0x68);
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    FUN_L08_002f0c18(1);
}
/* Builds the colour tables once, then scrolls two pairs of wrapped phases each frame. */
extern int D_L12_001620F0 __attribute__((sda));
/* The ready flag is followed, 8 bytes on, by two pairs of phases. */
extern float D_L12_001620E0[1] __attribute__((sda));
extern float D_L12_001620D8[2] __attribute__((sda));
extern float frame_delta_time __asm__("D_0015ED7C") __attribute__((section(".sdata")));
extern int *D_L12_00208AE0[];
extern int D_L12_00205A80[];
extern void FUN_L08_002f0f68(void *, int, void *, float, float, float, float, int);
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");

typedef struct {
    char p[0x20];
    unsigned char state;
    char q[0xF];
    unsigned char b30;
    char r;
    short h32;
} M_bff0;

void FUN_L12_0030bff0(M_bff0 *m) {
    int i;
    switch (m->state) {
    case 0:
        if (D_L12_001620F0 == 0) {
            FUN_L08_002f0f68(D_L12_00208AE0, 5, D_L12_00205A80, 0.1f, 0.5f, 0.4f, 0.5f, 1);
            FUN_L08_002f0f68(D_L12_00205A58, 10, D_L12_001FBD10, 0.1f, 0.6f, 0.4f, 0.5f, 1);
            D_L12_001620F0 = 1;
        }
        m->state = 1;
        m->b30 = 0xFF;
        m->h32 = 0xFF;
        for (i = 0; i < 2; i++) {
            ((float *)&D_L12_001620F0)[i * 2 + 2] = 0.0f;
            ((float *)&D_L12_001620F0)[i * 2 + 3] = 0.0f;
        }
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            ((float *)&D_L12_001620F0)[i * 2 + 2] += D_L12_001620E0[i * 2] * frame_delta_time;
            if (((float *)&D_L12_001620F0)[i * 2 + 2] > 1.0f) {
                ((float *)&D_L12_001620F0)[i * 2 + 2] -= 1.0f;
            }
            if (((float *)&D_L12_001620F0)[i * 2 + 2] < -1.0f) {
                ((float *)&D_L12_001620F0)[i * 2 + 2] += 1.0f;
            }
            ((float *)&D_L12_001620F0)[i * 2 + 3] += D_L12_001620E0[i * 2 + 1] * frame_delta_time;
            if (((float *)&D_L12_001620F0)[i * 2 + 3] > 1.0f) {
                ((float *)&D_L12_001620F0)[i * 2 + 3] -= 1.0f;
            }
            if (((float *)&D_L12_001620F0)[i * 2 + 3] < -1.0f) {
                ((float *)&D_L12_001620F0)[i * 2 + 3] += 1.0f;
            }
        }
        D_L12_00162108[0] += D_L12_001620D8[0] * frame_delta_time;
        if (D_L12_00162108[0] > 1.0f) {
            D_L12_00162108[0] -= 1.0f;
        }
        if (D_L12_00162108[0] < -1.0f) {
            D_L12_00162108[0] += 1.0f;
        }
        D_L12_00162108[1] += D_L12_001620D8[1] * frame_delta_time;
        if (D_L12_00162108[1] > 1.0f) {
            D_L12_00162108[1] -= 1.0f;
        }
        if (D_L12_00162108[1] < -1.0f) {
            D_L12_00162108[1] += 1.0f;
        }
        enqueue_callback_list_1(FUN_L12_0030be68, m);
        break;
    }
}

s32 D_L12_001FB6CC[1] __attribute__((section(".data"))) = {0};
