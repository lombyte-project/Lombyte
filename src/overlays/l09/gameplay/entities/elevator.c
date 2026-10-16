/* Level 09 moby classes: elevator, refueling_station, airship_part, turret, barrier, shuttle. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_globals.h"
#define F(p, o) (*(float *)((p) + (o)))

/* Moves a moby along a path between its ends once its collectable flag is set, easing between nodes. */
extern char *D_L09_001B0630[];
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L09LevelState;
extern L09LevelState D_L09_001BB3B0;
extern int D_L09_001BA650[];
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9b48(void *, void *);
extern int FUN_001f9770(void *);
extern int FUN_L00_0028d8c0(void *, int);
extern int FUN_L01_00277fb8(void *);
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_00227638(void);
extern void FUN_L00_00260738(void *, void *, void *, void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013E550[];
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

#define F(p, o) (*(float *)((p) + (o)))

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
void FUN_L09_003033a0(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *path;
    float orig[4], delta[4], tmp[4];
    int on = 0;
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L09_001B0630[*(int *)(data + 0xB4)];
    if (*(int *)(data + 0xC0) != -1) {
        unsigned short id;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xC0)) ||
            D_L09_001BB3B0.collected[(short)(id = *(unsigned short *)(moby + 0xB2))] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            if (*(int *)(data + 0xC0) != -1) {
                D_0014C190_257218[current_level_index][(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                D_L09_001BA650[(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                *(int *)(data + 0xC0) = -1;
            }
            on = 1;
        }
    } else {
        on = 1;
    }
    qcopy(orig, moby + 0x10);
    FUN_001f9770(data + 0xBA);
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
    case 0:
        if (on) {
            float far;
            if ((*(short *)(data + 0xB8) != 0 && FUN_L01_00277fb8(moby)) ||
                (far = FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16),
                 FUN_001f9b48(&hero.motion.pos, path + 0x10) < far)) {
                if (*(int *)(data + 0xC8) == 0 && FUN_L01_00277fb8(moby)) {
                    hero_set_state(0x72, 1);
                }
                moby[0xBC] = 2;
                F(data, 0xAC) = -1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case 1:
        if (on) {
            float near;
            if ((*(short *)(data + 0xB8) != 0 && FUN_L01_00277fb8(moby)) ||
                (near = FUN_001f9b48(&hero.motion.pos, path + 0x10),
                 FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16) < near)) {
                if (*(int *)(data + 0xC8) == 0 && FUN_L01_00277fb8(moby)) {
                    hero_set_state(0x72, 1);
                }
                allocate_voice_for_target_entry(1, 0, moby);
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case 2:
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f &&
             FUN_001f9b80((moby + 0x10), &hero.motion.pos) < 2.0f &&
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
            if (!FUN_L00_0028d8c0(moby, *(int *)(data + 0xC4))) {
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
                    hero_set_state(HERO_STATE_IDLE, 1);
                }
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                allocate_voice_for_target_entry(2, 0, moby);
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
    FUN_L00_00260738(data + 0x60, delta, moby + 0x40, moby + 0x40);
    if (!FUN_L01_00277fb8(moby)) {
        if (2.0f < FUN_001f9b80((moby + 0x10), &hero.motion.pos)) {
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xB8) = 1;
            }
        }
    }
}
#include "rnc/gameplay/entities/moby.h"

/* Damage source returned by find_moby_hit. */
typedef struct {
    u8 pad0[0x2C];
    f32 damage;
} VendorHitSource;

typedef struct {
    u8 pad0[0x26];
    s16 hurt_timer;
    u8 pad28[0x38];
    u8 anim[8];
    u8 pad68[8];
    s32 link;
} VendorVars;

extern struct Moby *moby_list __asm__("D_L09_0015FFD8");
char D_L09_00208F90[] = "Moby class %d Damage Received: %f\n";
char D_L09_00208FF0[] = "Moby class %d Damage Received: %f\n";
extern VendorHitSource *find_moby_hit(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 fptodp(f32) __asm__("FUN_00120478");
extern void debug_print(char *, s32, s32) __asm__("FUN_001e93b0");
extern s32 FUN_00213928(struct Moby *, VendorHitSource *, f32 *, s32, s32 *, s32, s32, s32);
extern void FUN_L00_00257470(struct Moby *, s32, s32);
extern void FUN_L00_0025d458(struct Moby *, u8 *);
extern void FUN_L00_0025d538(struct Moby *, u8 *);
extern s32 FUN_001f96f8(s32);

void FUN_L09_00303b30(struct Moby *moby, VendorVars *vars, f32 *health) {
    VendorHitSource *src;
    struct Moby *link;
    s32 hit;

    if (moby->state != 3) {
        src = find_moby_hit(moby, 0x330000, 0);
        if (src != 0) {
            debug_print(D_L09_00208F90, moby->oclass, fptodp(src->damage));
        }
        switch (FUN_00213928(moby, src, health, 0, &hit, 0, 0, 4)) {
        case 1:
        case 2:
            *health = 0.0f;
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
        case 11:
            break;
        }
        if (hit >= 2) {
            if (*health <= src->damage) {
                *health = 0.0f;
                moby->flags &= ~0x1000;
                if (vars->link != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD
                        && (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                vars->anim[7] = 0x78;
                FUN_L00_0025d458(moby, vars->anim);
                moby->state = 3;
            } else {
                *health -= src->damage;
                vars->anim[7] = 0xFA;
                vars->hurt_timer = FUN_001f96f8(60);
                FUN_L00_0025d458(moby, vars->anim);
            }
        }
        moby->hit_slot = 0xFF;
    }
    FUN_L00_0025d538(moby, vars->anim);
}
/* Pressure plate: waits for its trigger slot, hums while armed, wakes its linked moby when stepped on, then shatters. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003050C0), where it is exact; names translated to the US level program. */

extern char D_0013D388[];
extern float D_L09_00166F40[];
extern int D_L09_0015F5CC;
extern int D_L09_0015FFD8_u __asm__("D_L09_0015FFD8");
extern int FUN_L00_0025e450_u() __asm__("FUN_L00_0025e450");
extern unsigned char voice_pool[] __asm__("D_0013E550");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
s32 allocate_voice_for_target_entry_c(s32 entry_index, s32 flags,
                                      void *target) __asm__("FUN_0022da68");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int func_0022ED80_r(int, int, char *) __asm__("FUN_0022da68");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");

void FUN_L09_00303d10(struct Moby *m) {
    char *d = (char *)m->pvars;
    float v[4];
    unsigned char *q;
    FUN_L09_00303b30(m, d, d + 0x20);
    switch (m->state) {
    case 0:
        if (*(short *)(d + 0x76) != -1 &&
            (q = (unsigned char *)D_0013D388 + *(short *)(d + 0x76))[0x39] != 0) {
            mark_moby_for_removal(m);
            return;
        }
        *(int *)(d + 0x7C) = -1;
        m->state = 1;
        break;
    case 1:
        if ((D_L09_0015F5CC & 7) == (((int)m >> 8) & 7)) {
            if (*(int *)(d + 0x7C) == -1 ||
                (FUN_L00_0028d8c0(m, *(int *)(d + 0x7C)) == 0 && *(int *)(d + 0x7C) == -1)) {
                *(int *)(d + 0x7C) = func_0022ED80_r(1, 4, m);
            }
        }
        if (m->command == 1) {
            if (*(int *)(d + 0x78) != -1) {
                unsigned char *o = (unsigned char *)(D_L09_0015FFD8_u + (*(int *)(d + 0x78) << 8));
                if (o[0x20] == 1)
                    o[0xBC] = o[0x20];
            }
            *(short *)(d + 0x74) = scale_game_frames(0xA);
            if (*(int *)(d + 0x7C) != -1) {
                char *e = voice_pool + *(int *)(d + 0x7C) * 0x70;
                if (*(char **)(e + 0x88) == m && ((unsigned char *)e)[0x74] != 0) {
                    release_voice_slot(*(int *)(d + 0x7C));
                }
            }
            *(int *)(d + 0x7C) = -1;
            m->update_dist = 0xFF;
            m->state = 2;
        }
        break;
    case 2:
        if (FUN_001f9770(d + 0x74)) {
            if (*(int *)(d + 0x70) != -1) {
                unsigned char *o = (unsigned char *)(D_L09_0015FFD8_u + (*(int *)(d + 0x70) << 8));
                if (o != 0 && o[0x20] != 0xFE && o[0x20] != 0xFD &&
                    (*(short *)(o + 0xA6) == 0x494 || *(short *)(o + 0xA6) == 0x49D ||
                     *(short *)(o + 0xA6) == 0x4A0)) {
                    o[0x30] = 0xFF;
                    o[0xBC] = 1;
                }
            }
            m->state = 3;
        }
        break;
    case 3:
        subtract_vector_xyz_c(v, D_L09_00166F40, &m->pos);
        spawn_explosion(m, v, 0, 0.0f, 0.0f, 0x14, 9, 0x20, 10.0f, 7.0f, 20.0f, 2.0f, 0,
                              40.0f, 0, 1, -1, 0);
        mark_moby_for_removal(m);
        break;
    }
}
typedef struct {
    u8 pad0[0x26];
    s16 hurt_timer;
    u8 pad28[0x38];
    s32 link;
    s32 link2;
} VendorVars2;

void FUN_L09_00303fc8(struct Moby *moby, VendorVars2 *vars, f32 *health) {
    VendorHitSource *src;
    struct Moby *link;
    s32 hit;

    if (moby->state != 3) {
        src = find_moby_hit(moby, 0x330000, 0);
        if (src != 0) {
            debug_print(D_L09_00208FF0, moby->oclass, fptodp(src->damage));
        }
        switch (FUN_00213928(moby, src, health, 0, &hit, 0, 0, 4)) {
        case 1:
        case 2:
            *health = 0.0f;
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
        case 11:
            break;
        }
        if (hit >= 2) {
            if (*health <= src->damage) {
                *health = 0.0f;
                moby->flags &= ~0x1000;
                if (vars->link != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD
                        && (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                /* Retail checks the 0x64 link but indexes with the 0x60 one. */
                if (vars->link2 != -1) {
                    link = &moby_list[vars->link];
                    if (link != 0 && link->state != 0xFE && link->state != 0xFD
                        && (link->oclass == 0x494 || link->oclass == 0x49D || link->oclass == 0x4A0)) {
                        link->command = 1;
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                moby->state = 3;
            } else {
                *health -= src->damage;
                vars->hurt_timer = FUN_001f96f8(60);
            }
        }
        moby->hit_slot = 0xFF;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00305580), where it is exact; names translated to the US level program. */


extern char *D_L09_0015FFE4;
extern float FUN_001f9b48(void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

char *FUN_L09_003041d0(char *self, char *pos, int bone) {
    char *best = 0;
    float bestd = 1000000000.0f;
    char tmp[16];
    char vec[16];
    char *m;
    char *tp = tmp;
    *(OvlQuad *)tmp = *(OvlQuad *)pos;
    for (m = D_L09_0015FFE4; m != 0; m = *(char **)(m + 0x28)) {
        if (m == self)
            continue;
        if (m != 0 && *(unsigned char *)(m + 0x20) != 0xFE &&
            *(unsigned char *)(m + 0x20) != 0xFD) {
            if (*(short *)(m + 0xA6) == 0x49D || *(short *)(m + 0xA6) == 0x494 ||
                *(short *)(m + 0xA6) == 0x4A0) {
                float d;
                if (bone != -1) {
                    moby_point_position(m, bone, vec);
                    d = FUN_001f9b48(tp, vec);
                } else {
                    d = FUN_001f9b48(tp, m + 0x10);
                }
                if (d < bestd) {
                    bestd = d;
                    best = m;
                }
            }
        }
    }
    if (best == 0)
        return 0;
    if (bestd < 0.5f)
        return best;
    if (*(short *)(best + 0xA6) == 0x4A0 && bestd < 10.0f)
        return best;
    return 0;
}
#ifndef NON_MATCHING
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Airship chain link (class 1181) update: finds the parts it hangs between, swings between them, and on its trigger drops, wakes its neighbours and bursts. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00305710), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int u128_305710 __attribute__((mode(TI)));

typedef struct {
    char pad0[0x60];
    int hull;                  /* 0x60: moby index of the part it hangs from, -1 if none */
    int next;                  /* 0x64: moby index of the next part, -1 if none */
    unsigned char loose;       /* 0x68 */
    signed char flag;          /* 0x69 */
    short timer;               /* 0x6A */
    int trigger;               /* 0x6C */
} Data_305710;

typedef struct {
    char pad0[0x39];
    unsigned char b39;
} Flags_305710;

char *FUN_L09_003041d0(char *self, char *pos, int bone);
extern Moby *D_L09_0015FFD8;
extern char D_0013D388[];
extern char D_L09_00209050[];
extern char D_L09_00209080[];
extern f32 FUN_001f9b80(void *, void *);
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern float D_L09_00166F40[];
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern float subtract_vector_xyz_alt(float, float) __asm__("FUN_001fa580");
extern float vector_length_xyz_alt(float, float) __asm__("FUN_001fa5c8");
extern int D_L09_0015F5CC;
extern int stub_printf() __asm__("FUN_001e93b0");
extern int FUN_001f9770(void *);
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
void FUN_L00_0025e450(f32 burst, f32 unused_float, f32 owner_effect, f32 owner_effect2, f32 cutoff, f32 scale, f32 flash, void *owner, s32 unused2, void *pos, s32 puff_count, s32 spark_count, s32 shard_count, s32 sound, s32 shake, s32 spray_count, s32 tint, u8 alternate);
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern Moby *func_L09_00305580_305710(void *, void *, int) __asm__("FUN_L09_003041d0");

void FUN_L09_00304360(Moby *m) {
    Data_305710 *d = (*(Data_305710 **)&m->pvars);
    float v20[4];
    float v30[4];
    float v40[4];
    Moby *hull;
    Moby *next;
    Moby *r;
    float dist;
    int n;
    int t;

    switch (m->state) {
    case 0:
        if (d->flag != -1 && ((Flags_305710 *)(D_0013D388 + d->flag))->b39 != 0) {
            mark_moby_for_removal(m);
            return;
        }
        if (d->hull == -1) {
            FUN_L00_0024f7c8(m, 0, v20);
            r = func_L09_00305580_305710(m, v20, -1);
            if (r != 0) {
                d->hull = r - D_L09_0015FFD8;
            } else {
                stub_printf(D_L09_00209050, (*(short *)&m->save_id));
            }
        }
        if (d->next == -1) {
            *(u128_305710 *)v20 = *(u128_305710 *)((f32 *)&m->pos);
            r = func_L09_00305580_305710(m, v20, 0);
            if (r != 0) {
                d->next = r - D_L09_0015FFD8;
            } else {
                stub_printf(D_L09_00209080, (*(short *)&m->save_id));
            }
        }
        m->state = 1;
        break;
    case 1:
        if (m->visible == 0 || distance_xyz(((f32 *)&m->pos), D_L09_00166F40) > 32.0f) {
            if (D_L09_0015F5CC % 8 != (short)((*(short *)&m->save_id) % 8)) {
                return;
            }
        }
        hull = 0;
        next = 0;
        if (d->hull != -1) {
            hull = &D_L09_0015FFD8[d->hull];
            if (hull == 0 || hull->state == 0xFE || hull->state == 0xFD) {
                hull = 0;
            }
        }
        if (d->next != -1) {
            next = &D_L09_0015FFD8[d->next];
            if (next == 0 || next->state == 0xFE || next->state == 0xFD) {
                d->loose = 0;
                next = 0;
            } else if ((*(Data_305710 **)&next->pvars)->loose == 0) {
                d->loose = 0;
            }
        }
        if (d->loose == 0 && hull != 0 && next != 0 && next->oclass == 0x49D) {
            if ((*(Data_305710 **)&next->pvars)->loose != 0) {
                dist = FUN_001f9b80(((f32 *)&m->pos), ((f32 *)&hull->pos));
                m->rot.y = subtract_vector_xyz_alt(m->rot.y, vector_length_xyz_alt(-angle_atan2(dist, hull->pos.z - m->pos.z), m->rot.y) * 0.1f);
                m->rot.z = subtract_vector_xyz_alt(m->rot.z, vector_length_xyz_alt(angle_atan2(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) * 0.1f);
            } else {
                subtract_vector_xyz(v20, ((f32 *)&hull->pos), ((f32 *)&m->pos));
                fast_vec_normalize(v20, v20, (vector_length_xyz(v20) - 1.3f) * 0.7f);
                if (next->oclass == 0x49D && next->state != 0xFE && next->state != 0xFD) {
                    subtract_vector_xyz(v30, ((f32 *)&next->pos), ((f32 *)&m->pos));
                    fast_vec_normalize(v30, v30, (vector_length_xyz(v30) - 1.3f) * 0.7f);
                    add_vector_xyz(v40, v20, v30);
                } else {
                    qcopy(v40, v20);
                }
                v40[2] -= frame_time_sq * 9.8f;
                add_vector_xyz(((f32 *)&m->pos), ((f32 *)&m->pos), v40);
                dist = FUN_001f9b80(((f32 *)&m->pos), ((f32 *)&hull->pos));
                m->rot.y = subtract_vector_xyz_alt(m->rot.y, vector_length_xyz_alt(-angle_atan2(dist, hull->pos.z - m->pos.z), m->rot.y) * 0.1f);
                m->rot.z = subtract_vector_xyz_alt(m->rot.z, vector_length_xyz_alt(angle_atan2(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) * 0.1f);
            }
        } else {
            d->loose = 1;
        }
        if (m->command == 1) {
            if (d->trigger != -1) {
                Moby *o = &D_L09_0015FFD8[d->trigger];
                if (o->state == 1) {
                    o->command = o->state;
                }
            }
            d->timer = scale_game_frames_alt(10);
            m->update_dist = 0xFF;
            m->state = 2;
        }
        break;
    case 2:
        m->update_dist = 0xFF;
        hull = 0;
        if (d->hull != -1) {
            hull = &D_L09_0015FFD8[d->hull];
            if (hull == 0 || hull->state == 0xFE || hull->state == 0xFD) {
                hull = 0;
            }
        }
        if (hull != 0) {
            subtract_vector_xyz(v20, ((f32 *)&hull->pos), ((f32 *)&m->pos));
            fast_vec_normalize(v20, v20, vector_length_xyz(v20) - 1.3f);
            qcopy(v30, v20);
            n = scale_game_frames_alt(10);
            t = d->timer - 3;
            v30[2] -= frame_time_sq * 9.8f * 2.0f * convert_integer_to_float(n - t);
            add_vector_xyz(((f32 *)&m->pos), ((f32 *)&m->pos), v30);
            dist = FUN_001f9b80(((f32 *)&m->pos), ((f32 *)&hull->pos));
            m->rot.y = subtract_vector_xyz_alt(m->rot.y, vector_length_xyz_alt(-angle_atan2(dist, hull->pos.z - m->pos.z), m->rot.y) * 0.5f);
            m->rot.z = subtract_vector_xyz_alt(m->rot.z, vector_length_xyz_alt(angle_atan2(hull->pos.x - m->pos.x, hull->pos.y - m->pos.y), m->rot.z) * 0.5f);
        }
        if (FUN_001f9770(&d->timer)) {
            if (d->hull != -1) {
                Moby *o = &D_L09_0015FFD8[d->hull];
                if (o != 0 && o->state != 0xFE && o->state != 0xFD
                    && (o->oclass == 0x494 || o->oclass == 0x49D || o->oclass == 0x4A0)) {
                    o->update_dist = 0xFF;
                    o->command = 1;
                }
            }
            if (d->next != -1) {
                Moby *o = &D_L09_0015FFD8[d->next];
                if (o != 0 && o->state != 0xFE && o->state != 0xFD
                    && (o->oclass == 0x494 || o->oclass == 0x49D || o->oclass == 0x4A0)) {
                    o->update_dist = 0xFF;
                    o->command = 1;
                }
            }
            m->state = 3;
        }
        break;
    case 3:
        subtract_vector_xyz(v20, D_L09_00166F40, ((f32 *)&m->pos));
        spawn_explosion(m, v20, 0, 0.0f, 0.0f, 10, 5, 8, 2.0f, 1.0f, 10.0f, 1.0f, 0, 10.0f, 0, 1, -1, 0);
        mark_moby_for_removal(m);
        break;
    }
}
#else
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float atan2_f(float, float) __asm__("FUN_001f9e90");
extern void scale_vector_00304360(float, void *, void *) __asm__("FUN_001f9bf8");
extern float FUN_001f9af0(void *);
extern char D_L09_00209050[];
extern char D_L09_00209080[];
extern void print_missing_link(char *, short) __asm__("FUN_001e93b0");
extern void effect_00304360(f32, f32, f32, f32, f32, f32, f32,
                             void *, void *, int, int, int, int, int, int, int, int, int) __asm__("FUN_L00_0025e450");

void FUN_L09_00304360(unsigned char *moby) {
    unsigned char *data = *(unsigned char **)(moby + 0x78);
    unsigned char *link;
    unsigned char *link2;
    float work[4], work2[4], work3[4];
    float angle;
    int index;
    int tick;
    short oclass;

    switch ((int)moby[0x20]) {
case 0:
    {
        unsigned char *link;
        if ((signed char)data[0x69] != -1 && ((unsigned char *)D_0013D388 + (signed char)data[0x69])[0x39] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)(data + 0x60) == -1) {
            FUN_L00_0024f7c8(moby, 0, work);
            link = (unsigned char *)FUN_L09_003041d0((char *)moby, (char *)work, -1);
            if (link != 0) {
                *(int *)(data + 0x60) = ((char *)link - (char *)moby_list) >> 8;
            } else {
                print_missing_link(D_L09_00209050, *(short *)(moby + 0xb2));
            }
        }
        if (*(int *)(data + 0x64) == -1) {
            *(OvlQuad *)work = *(OvlQuad *)(moby + 0x10);
            link = (unsigned char *)FUN_L09_003041d0((char *)moby, (char *)work, 0);
            if (link != 0) {
                *(int *)(data + 0x64) = ((char *)link - (char *)moby_list) >> 8;
            } else {
                print_missing_link(D_L09_00209080, *(short *)(moby + 0xb2));
            }
        }
        moby[0x20] = 1;
        return;
    }
case 1:
    {
        float rotation_scale;
        float height;
        if (moby[0x31] == 0 || FUN_001f9b48(moby + 0x10, D_L09_00166F40) > 32.0f) {
            int a = *(short *)(moby + 0xb2);
            int b = D_L09_0015F5CC;
            if (b % 8 != (short)(a % 8)) return;
        }
        link = 0;
        link2 = 0;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            link = (unsigned char *)((char *)D_L09_0015FFD8_u + (index << 8));
            if (link == 0 || link[0x20] == 0xfe || link[0x20] == 0xfd) link = 0;
        }
        index = *(int *)(data + 0x64);
        if (index != -1) {
            link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2 == 0 || link2[0x20] == 0xfe || link2[0x20] == 0xfd) {
                data[0x68] = 0;
                link2 = 0;
            } else if (*(unsigned char *)(*(char **)(link2 + 0x78) + 0x68) == 0) {
                data[0x68] = 0;
            }
        }
        if (*(unsigned char *)(data + 0x68) == 0) {
            if (link == 0) {
                data[0x68] = 1;
            } else if (link2 != 0 && *(short *)(link2 + 0xa6) == 0x49d) {
                if (*(unsigned char *)(*(char **)(link2 + 0x78) + 0x68) != 0) {
                    angle = FUN_001f9b80(moby + 0x10, link + 0x10);
                    height = F(link, 0x18);
                    rotation_scale = 0.1f;
                } else {
                    subtract_vector_xyz(work, link + 0x10, moby + 0x10);
                    scale_vector_00304360((FUN_001f9af0(work) - 1.3f) * 0.7f, work, work);
                    if (*(short *)(link2 + 0xa6) == 0x49d && link2[0x20] != 0xfe && link2[0x20] != 0xfd) {
                        subtract_vector_xyz(work2, link2 + 0x10, moby + 0x10);
                        scale_vector_00304360((FUN_001f9af0(work2) - 1.3f) * 0.7f, work2, work2);
                        add_vector_xyz(work3, work, work2);
                    } else {
                        qcopy(work3, work);
                    }
                    work3[2] = work3[2] - frame_time_sq * 9.8f;
                    rotation_scale = 0.1f;
                    add_vector_xyz(moby + 0x10, moby + 0x10, work3);
                    angle = FUN_001f9b80(moby + 0x10, link + 0x10);
                    height = F(link, 0x18);
                }
                angle = atan2_f(angle, height - F(moby, 0x18));
                F(moby, 0x44) = fast_add_rotations(F(moby, 0x44), fast_subtract_rotations(-angle, F(moby, 0x44)) * rotation_scale);
                angle = atan2_f(F(link, 0x10) - F(moby, 0x10), F(link, 0x14) - F(moby, 0x14));
                F(moby, 0x48) = fast_add_rotations(F(moby, 0x48), fast_subtract_rotations(angle, F(moby, 0x48)) * rotation_scale);
            } else {
                data[0x68] = 1;
            }
        } else {
            data[0x68] = 1;
        }
        if (moby[0xbc] != 1) return;
        index = *(int *)(data + 0x6c);
        if (index != -1) {
            unsigned char *link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2[0x20] == 1) link2[0xbc] = 1;
        }
        *(short *)(data + 0x6a) = FUN_001f96f8(10);
        moby[0x30] = 0xff;
        moby[0x20] = 2;
        return;
    }
case 2:
    {
        unsigned char *link;
        link = 0;
        moby[0x30] = 0xff;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            link = (unsigned char *)((char *)D_L09_0015FFD8_u + (index << 8));
            if (link == 0 || link[0x20] == 0xfe || link[0x20] == 0xfd) link = 0;
        }
        if (link != 0) {
            subtract_vector_xyz(work, link + 0x10, moby + 0x10);
            scale_vector_00304360(FUN_001f9af0(work) - 1.3f, work, work);
            qcopy(work2, work);
            tick = FUN_001f96f8(10);
            work2[2] -= (frame_time_sq * 9.8f + frame_time_sq * 9.8f) *
                        convert_integer_to_float(tick - (*(short *)(data + 0x6a) - 3));
            add_vector_xyz(moby + 0x10, moby + 0x10, work2);
            angle = FUN_001f9b80(moby + 0x10, link + 0x10);
            angle = atan2_f(angle, F(link, 0x18) - F(moby, 0x18));
            F(moby, 0x44) = fast_add_rotations(F(moby, 0x44), fast_subtract_rotations(-angle, F(moby, 0x44)) * 0.5f);
            angle = atan2_f(F(link, 0x10) - F(moby, 0x10), F(link, 0x14) - F(moby, 0x14));
            F(moby, 0x48) = fast_add_rotations(F(moby, 0x48), fast_subtract_rotations(angle, F(moby, 0x48)) * 0.5f);
        }
        if (FUN_001f9770(data + 0x6a) == 0) return;
        index = *(int *)(data + 0x60);
        if (index != -1) {
            unsigned char *link;
            link = (unsigned char *)((char *)moby_list + (index << 8));
            if (link != 0 && link[0x20] != 0xfe && link[0x20] != 0xfd) {
                oclass = *(short *)(link + 0xa6);
                if (oclass == 0x494 || oclass == 0x49d || oclass == 0x4a0) {
                    link[0x30] = 0xff;
                    link[0xbc] = 1;
                }
            }
        }
        index = *(int *)(data + 0x64);
        if (index != -1) {
            unsigned char *link2;
            link2 = (unsigned char *)((char *)moby_list + (index << 8));
            if (link2 != 0 && link2[0x20] != 0xfe && link2[0x20] != 0xfd) {
                oclass = *(short *)(link2 + 0xa6);
                if (oclass == 0x494 || oclass == 0x49d || oclass == 0x4a0) {
                    link2[0x30] = 0xff;
                    link2[0xbc] = 1;
                }
            }
        }
        moby[0x20] = 3;
        return;
    }
case 3:
    {
        subtract_vector_xyz(work, D_L09_00166F40, moby + 0x10);
        effect_00304360(0.0f, 0.0f, 2.0f, 1.0f, 10.0f, 1.0f, 10.0f,
                          moby, work, 0, 10, 5, 8, 0, 0, 1, -1, 0);
        mark_moby_for_removal(moby);
    }
    }
}
#endif
#include "rnc/input/pad_state.h"
/* Gaspar turret the hero mounts: aims with the stick, fires its two barrels in turn,
   and while idle swings toward the hero with its beam light on. */
typedef struct {
    u8 pad0[0x10];
    Vec4 aim;                         /* 0x10: FUN_L00_001fff28 target */
    u8 pad20[0x20];
    s16 unk40;
    s16 mount_delay;                  /* 0x42: frames before the hero can mount again */
    f32 pitch;                        /* 0x44 */
    s32 unk48;
    s32 unk4C;
    u8 pad50[2];
    s16 dismount_delay;               /* 0x52 */
    s16 barrel;                       /* 0x54: 0 fires barrel 1 next, 1 barrel 2 */
    u8 pad56[6];
    f32 rest_z;                       /* 0x5C */
    f32 yaw_speed;                    /* 0x60 */
    f32 pitch_speed;                  /* 0x64 */
    s32 beam;                         /* 0x68: D_L09_00180440 slot, -1: none */
    s32 beam_frames;                  /* 0x6C */
} TurretVars;

typedef struct {
    f32 size;
    u8 pad4[0xC];
    f32 pos[3];
    f32 alpha;
} TurretBeam;

typedef struct {
    u8 pad0[0x160];
    f32 unk160;
    u8 pad164[4];
    s32 unk168;
    u8 pad16C[4];
    f32 unk170;
    u8 pad174[4];
    s32 unk178;
} TurretShake;

/* moby the sound collision probe ignores (clamp_voice_position_to_collision) */
extern struct Moby *sound_probe_ignore[] __asm__("D_0013E5BC");
extern s32 D_0015EDDC;
extern f32 D_L09_00161CB0 __attribute__((sda));
extern f32 D_L09_00161CB4 __attribute__((sda));
extern f32 D_L09_00161CC0[2] __attribute__((sda)); /* a vector; declared small so Ps2EeAs reaches it through $gp */
extern TurretShake D_L09_00166E00;
extern TurretBeam D_L09_00180440[];
extern void FUN_L09_002efda8(struct Moby *, Vec4 *);
extern struct Moby *FUN_L09_00307ba8(void *, void *, void *, int);
extern void FUN_L08_00313ba8(void);
extern int FUN_L00_0023e738(void *, float, float, float, float, float);
extern void FUN_L00_0023e838(int);
extern void FUN_L00_0026dd20(void *, void *, int, int, float);
extern void FUN_L00_001ff378(void *, void *, float);
extern void FUN_L00_001edf60(void *, int, void *, int, int, int, float, float, float);
extern void FUN_L00_002eac18(int);
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void FUN_L00_002eaaa0(void *, void *, int, int, int);
extern void fast_mem_zero16(void *, int) __asm__("FUN_001f9810");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float FUN_001f9af0(void *);
extern float FUN_001f9dc8(float);
extern float FUN_001f9de0(float);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void attach_manipulator(struct Moby *, s32, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_L00_001fff28(void *, int, float);

void FUN_L09_00304c80(struct Moby *moby) {
    Vec4 seat;
    Vec4 offset;
    Vec4 tmp;
    Vec4 shot;
    Vec4 muzzle;
    Vec4 dir;
    Vec4 rnd;
    TurretVars *vars;
    TurretBeam *beam;
    float len;
    float stick_y;
    int n;
    int i;

    vars = (TurretVars *)moby->pvars;
    moby->flags &= ~4;
    moby_point_position(moby, 3, &seat);
    seat.f[2] += 0.4f;
    if (moby->state >= 4) {
        if (FUN_001f9770(&vars->mount_delay) != 0 && moby->command == 1 &&
            hero.state.control_mode != 0x14 && hero.state.control_mode != 7 && hero.state.current != 0x3D) {
            qcopy(&hero.motion.pos, &seat);
            qcopy(&hero.motion.rot, &moby->rot);
            hero.motion.rot.f[2] = fast_add_rotations(1.5707964f, hero.motion.rot.f[2]);
            offset.f[0] = FUN_001f9dc8(hero.motion.rot.f[2]) * -0.7f;
            offset.f[1] = FUN_001f9de0(hero.motion.rot.f[2]) * -0.7f;
            offset.f[2] = 0.0f;
            add_vector_xyz(&hero.motion.pos, &hero.motion.pos, &offset);
            fast_mem_zero16(&hero.motion.velocity, 0x90);
            if (D_0013CAE4 & 0x10) {
                FUN_L00_002eac18(0);
                hero_set_state(0, 1);
                moby->command = 2;
                moby->state = 4;
                blend_moby_animation(moby, 0, 0, 1);
            } else {
                if (moby->state != 5) {
                    vars->dismount_delay = 2;
                    FUN_L00_002eaaa0(&moby->pos, &moby->rot, 0, 0, 0);
                    if (moby->prev_seq != 5) {
                        blend_moby_animation(moby, 5, 0, scale_game_frames(5));
                    }
                }
                moby->state = 5;
            }
        } else if (moby->command == 2) {
            if (FUN_001f9b48(&seat, &hero.moby->pos) > 1.2f || hero.ground_moby != moby) {
                moby->command = 0;
                vars->mount_delay = scale_game_frames(60);
            }
        } else if (hero.state.control_mode != 0x14 && hero.state.control_mode != 7 &&
                   hero.state.current != 0x3D && hero.ground_moby == moby &&
                   FUN_001f9b48(&seat, &hero.moby->pos) < 1.2f && vars->mount_delay == 0) {
            moby->command = 1;
            hero_set_state(0x32, 1);
        }
    }
    switch (moby->state) {
    case 0:
        attach_manipulator(moby, 0, vars);
        tmp.q = *(u128 *)&moby->pos;
        FUN_L09_002efda8(moby, &tmp);
        moby->state = 4;
        vars->rest_z = moby->pos.z;
        vars->pitch = 0.0f;
        moby->rot.z = 0.0f;
        vars->beam = -1;
        vars->mount_delay = 0;
        vars->unk48 = 0;
        vars->unk4C = 0;
        vars->unk40 = 0;
        sound_probe_ignore[0] = moby;
        return;
    case 4:
        if (FUN_001f9b80(&moby->pos, &hero.motion.pos) < 4.0f || hero.ground_moby == moby) {
            moby->pos.z += ((vars->rest_z - 1.0f) - moby->pos.z) * 0.11f;
            vars->yaw_speed = fast_add_rotations(vars->yaw_speed,
                fast_add_rotations(fast_subtract_rotations(
                    fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x, hero.motion.pos.f[1] - moby->pos.y),
                    moby->rot.z), 1.5707964f) * 0.01f) * 0.9f;
            moby->rot.z = fast_add_rotations(moby->rot.z, vars->yaw_speed);
            transform_vector_by_basis(&tmp, D_L09_00161CC0, &moby->rot_row0);
            add_vector_xyz(&tmp, &tmp, &seat);
            if (vars->beam == -1) {
                vars->beam_frames = 0;
                vars->beam = FUN_L00_0023e738(&tmp, 0.4f, 0.0f, 4.0f, 4.0f, 4.0f);
                allocate_voice_for_target_entry(1, 0, moby);
            } else {
                beam = &D_L09_00180440[vars->beam];
                beam->pos[0] = tmp.f[0];
                beam->pos[1] = tmp.f[1];
                beam->pos[2] = tmp.f[2];
                n = ++vars->beam_frames;
                if (n > 40 || !(n & 4)) {
                    beam->size = 4.0f;
                    beam->alpha = 0.4f;
                } else if (n >= 25 && (n & 0xA) == 0xA) {
                    beam->size = 4.0f;
                    beam->alpha = 0.4f;
                } else {
                    beam->alpha = 0.0f;
                }
            }
        } else {
            if (moby->pos.z < vars->rest_z) {
                moby->pos.z += frame_time;
            } else {
                moby->pos.z = vars->rest_z;
            }
            if (hero.ground_moby != moby && vars->beam != -1) {
                allocate_voice_for_target_entry(2, 0, moby);
                FUN_L00_0023e838(vars->beam);
                vars->beam = -1;
            }
        }
        break;
    case 5:
        sound_probe_ignore[0] = moby;
        if (moby->pos.z < vars->rest_z) {
            moby->pos.z += frame_time;
        } else {
            moby->pos.z = vars->rest_z;
        }
        tmp.f[0] = hero.stick_x;
        stick_y = -hero.stick_y;
        if (!D_0015EDDC) {
            stick_y = hero.stick_y;
        }
        tmp.f[1] = stick_y;
        len = vector_length_xy(&tmp);
        if (len > 1.0f) {
            FUN_L00_001ff378(&tmp, &tmp, len);
        }
        vars->yaw_speed += frame_time_sq * 3.1415927f * -tmp.f[0];
        vars->pitch_speed += frame_time_sq * 0.7853982f * -tmp.f[1];
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->yaw_speed);
        vars->pitch = fast_add_rotations(vars->pitch, vars->pitch_speed);
        if (vars->pitch < -0.5235988f) {
            vars->pitch = -0.5235988f;
            vars->pitch_speed = 0.0f;
        } else if (vars->pitch > 0.5235988f) {
            vars->pitch = 0.5235988f;
            vars->pitch_speed = 0.0f;
        }
        vars->yaw_speed *= 0.97f;
        vars->pitch_speed *= 0.97f;
        FUN_L00_001fff28(&vars->aim, 1, -vars->pitch);
        if (controller_state.held & 0x20) {
            if (controller_state.pressed & 0x20) {
                blend_moby_animation(moby, vars->barrel == 0 ? 4 : 3, 0, 1);
            } else if (moby->seq == 3) {
                if (absolute_float(compute_interpolated_record_value(moby) - 1.0f) <= 0.25f) {
                    build_spherical_offset(&shot, frame_time * 60.0f, fast_add_rotations(1.5707964f, moby->rot.z), vars->pitch);
                    moby_point_position(moby, 1, &muzzle);
                    FUN_L09_00307ba8(moby, &muzzle, &shot, scale_game_frames(90));
                    allocate_voice_for_target_entry(0, 0, moby);
                    D_L09_00166E00.unk160 = 0.03f;
                    D_L09_00166E00.unk168 = scale_game_frames(10);
                    D_L09_00166E00.unk170 = 0.03f;
                    D_L09_00166E00.unk178 = scale_game_frames(7);
                    vars->barrel = 0;
                    for (i = 0; i < 5; i++) {
                        rnd.q = 0;
                        rnd.f[0] = random_float_between(-1.0f, 1.0f);
                        rnd.f[1] = random_float_between(-1.0f, 1.0f);
                        rnd.f[2] = random_float_between(-1.0f, 1.0f);
                        dir.q = rnd.q;
                        normalize_vector_xyz(&dir, &dir, FUN_001f9af0(&shot) * (frame_scale * (0.65f - 1.0f) + 1.0f));
                        add_vector_xyz(&dir, &shot, &dir);
                        normalize_vector_xyz(&dir, &dir, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                        FUN_L00_0026dd20(&muzzle, &dir, 0x5F2F4F6F, random_int_between(scale_game_frames(5), scale_game_frames(10)), 50000.0f);
                    }
                    normalize_vector_xyz(&shot, &shot, frame_time * 0.5f);
                    FUN_L00_0026dd20(&muzzle, &shot, 0x2F4F7F7F, random_int_between(scale_game_frames(4), scale_game_frames(7)), 500000.0f);
                } else if (moby->anim_flags & 2) {
                    blend_moby_animation(moby, 4, 0, 1);
                }
            } else if (moby->seq == 4) {
                if (absolute_float(compute_interpolated_record_value(moby) - 1.0f) <= 0.25f) {
                    build_spherical_offset(&shot, frame_time * 60.0f, fast_add_rotations(1.5707964f, moby->rot.z), vars->pitch);
                    moby_point_position(moby, 2, &muzzle);
                    FUN_L09_00307ba8(moby, &muzzle, &shot, scale_game_frames(90));
                    allocate_voice_for_target_entry(0, 0, moby);
                    D_L09_00166E00.unk160 = 0.03f;
                    D_L09_00166E00.unk168 = scale_game_frames(10);
                    D_L09_00166E00.unk170 = 0.03f;
                    D_L09_00166E00.unk178 = scale_game_frames(7);
                    vars->barrel = 1;
                    for (i = 0; i < 5; i++) {
                        rnd.q = 0;
                        rnd.f[0] = random_float_between(-1.0f, 1.0f);
                        rnd.f[1] = random_float_between(-1.0f, 1.0f);
                        rnd.f[2] = random_float_between(-1.0f, 1.0f);
                        dir.q = rnd.q;
                        normalize_vector_xyz(&dir, &dir, FUN_001f9af0(&shot) * (frame_scale * (0.65f - 1.0f) + 1.0f));
                        add_vector_xyz(&dir, &shot, &dir);
                        normalize_vector_xyz(&dir, &dir, random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                        FUN_L00_0026dd20(&muzzle, &dir, 0x5F2F4F6F, random_int_between(scale_game_frames(5), scale_game_frames(10)), 50000.0f);
                    }
                    normalize_vector_xyz(&shot, &shot, frame_time * 0.5f);
                    FUN_L00_0026dd20(&muzzle, &shot, 0x2F4F7F7F, random_int_between(scale_game_frames(4), scale_game_frames(7)), 500000.0f);
                } else if (moby->anim_flags & 2) {
                    blend_moby_animation(moby, 3, 0, 1);
                }
            }
        } else if (moby->prev_seq != 5 && (moby->anim_flags & 2)) {
            blend_moby_animation(moby, 5, 0, 1);
        }
        if (FUN_001f9770(&vars->dismount_delay) != 0) {
            FUN_L00_00250df8(moby);
            moby->flags |= 4;
            FUN_L08_00313ba8();
            moby_point_position(moby, 0, &tmp);
            build_spherical_offset(&shot, D_L09_00161CB0, fast_add_rotations(1.5707964f, moby->rot.z), vars->pitch);
            shot.f[2] += D_L09_00161CB4;
            add_vector_xyz(&muzzle, &tmp, &shot);
            FUN_L00_002ea9d8(&muzzle);
            dir.f[0] = 0.0f;
            dir.f[1] = -vars->pitch;
            dir.f[2] = fast_add_rotations(1.5707964f, moby->rot.z);
            FUN_L00_002eaa30(&dir);
        }
        FUN_L00_001edf60(moby, 0xFF0F0FFF, 0, 0xD, -1, 4, 1.0f, 0.0f, 90.0f);
        break;
    }
}
extern void FUN_001f9cf8(void *, void *, void *);
extern void FUN_001f9bf8(f32, void *, void *);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_L00_00259858(void *, void *, s32, f32);
extern s32 collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern char D_L09_002090B0[];

typedef struct { char pad[0x18]; char *hit; } CollisionState;
extern CollisionState D_L09_00173FC0;

void FUN_L09_00305a28(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float work[24];
    int i;
    float count_as_float;
    float start;
    float end;

    if (moby->state == 0) {
        moby->state = 1;
        *(s32 *)(data + 0x14) = -1;
    }
    if ((D_L09_0015F5CC & 7) == (((int)moby >> 8) & 7)) {
        if (*(s32 *)(data + 0x14) == -1 ||
            (FUN_L00_0028d8c0(moby, *(s32 *)(data + 0x14)) == 0 &&
             *(s32 *)(data + 0x14) == -1)) {
            *(s32 *)(data + 0x14) = allocate_voice_for_target_entry(0, 4, moby);
        }
    }
    count_as_float = convert_integer_to_float(*(s32 *)(data + 8));
    start = F(data, 0);
    end = F(data, 4);
    *(OvlQuad *)work = 0;
    work[2] = (end - (start + start)) / count_as_float;
    qcopy(work + 4, (char *)moby + 0x10);
    FUN_001f9cf8(work, work, (char *)moby + 0xc0);
    FUN_001f9bf8(F(data, 0), work + 8, work);
    FUN_001f9a10(work + 4, work + 4, work + 8);
    FUN_L00_00259858(work + 12, moby, *(s32 *)(data + 0x10), *(float *)(data + 0xc));
    i = 0;
    if (*(s32 *)(data + 8) > 0) do {
        if (collect_mobies_in_sphere(F(data, 0), work + 4, 1, moby, work + 12) != 0) {
            char *hit = D_L09_00173FC0.hit;
            stub_printf(D_L09_002090B0, *(s16 *)((char *)moby + 0xb2), *(s16 *)(hit + 0xa6),
                        *(s16 *)(hit + 0xb2), F(data, 0xc));
        }
        i++;
        FUN_001f9a10(work + 4, work + 4, work);
    } while (i < *(s32 *)(data + 8));
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* spawn a moby copying position, rotation and data word from owner */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_003095D0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern int FUN_001f96f8(int);
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_0025d1b8(void *);

char *FUN_L09_00308220(struct Moby *owner, int cls) {
    char *moby = func_0020D348_m(cls);
    if (moby != 0) {
        unsigned char state;
        ((unsigned char *)moby)[0x30] = 0xFF;
        state = 0xFF;
        *(short *)(moby + 0x32) = state;
        moby[0x31] = 1;
        moby[0x20] = 0;
        qcopy(moby + 0x10, &owner->pos);
        qcopy(moby + 0x40, &owner->rot);
        moby[0xBC] = FUN_001f96f8(10);
        FUN_L00_00250df8(moby);
        FUN_L00_0025d1b8(moby);
        *(float *)(moby + 0x2C) = owner->scale;
        *(unsigned short *)(moby + 0x34) = owner->flags;
        **(int **)(moby + 0x78) = *(int *)owner->pvars;
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_00309688), where it is exact; names translated to the US level program. */

typedef struct BarrierData_309688 {
    int idx;
} BarrierData_309688;

typedef struct Hit_309688 {
    char pad0[0x10];
    float pos[4];
} Hit_309688;

typedef struct Flags_309688 {
    char pad0[0x43];
    unsigned char f43;
} Flags_309688;

char *FUN_L09_00308220_c(struct Moby *owner, int cls) __asm__("FUN_L09_00308220");
extern char *FUN_L00_0025a420(void *, int, int);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
extern float D_L09_0015F5D0;
extern float D_L09_0015F5D4;
extern int FUN_L00_001fefc8();
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern unsigned char D_0014C050_c[] __asm__("D_0014C050");
extern void FUN_L00_0025f090(void *, void *, int, float, float);
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");
struct Moby *FUN_L09_002ef868(struct Moby *src, Vec4 *pos, Vec4 *vel, s32 kind, f32 scale);
void FUN_L00_0025e450(f32 burst, f32 unused_float, f32 owner_effect, f32 owner_effect2, f32 cutoff, f32 scale, f32 flash, void *owner, s32 unused2, void *pos, s32 puff_count, s32 spark_count, s32 shard_count, s32 sound, s32 shake, s32 spray_count, s32 tint, u8 alternate);
extern int func_L00_001FEF78_309688(void *) __asm__("FUN_L00_001fefc8");
extern void func_L00_00260108_309688(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern Hit_309688 *func_L00_0025B478_309688(void *, int, int) __asm__("FUN_L00_0025a420");
extern int func_L09_003095D0_309688(void *, int) __asm__("FUN_L09_00308220");
extern int func_001F9850_309688(int) __asm__("FUN_001f96f8");
extern int func_0022ED80_309688(int, int, void *) __asm__("FUN_0022da68");
extern float func_002140F8_309688(float, float) __asm__("FUN_002132a8");
extern float func_001F9F90_309688(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_309688(float) __asm__("FUN_001f9de0");
extern void func_001F9EC0_309688(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float func_001F9B88_309688(float) __asm__("FUN_001f99c0");
extern void func_L00_001FF4B0_309688(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_L09_002F0BB8_309688(void *, void *, void *, int, float) __asm__("FUN_L09_002ef868");
extern float func_001FA888_309688(int) __asm__("FUN_001fa6c0");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_L00_0025F4A8_309688(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void func_0020D678_309688(void *) __asm__("FUN_0020c828");
extern unsigned char D_0014171B_309688[] __asm__("D_0014C050");
extern char alternate_item_available[] __asm__("D_0013D388");

void FUN_L09_003082d8(Moby *moby)
{
    BarrierData_309688 *data = (*(BarrierData_309688 **)&moby->pvars);
    float origin[4], center[4], piece_pos[4], vel[4], push[4];
    Hit_309688 *hit;
    int has_next;
    int count;
    int i;
    float angle;
    float scale;

    if (moby->state == 0) {
        if (data->idx != -1) ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 0;
        moby->state = 1;
    }
    if (moby->mission != 0xFF) {
        int row = current_level_index * 16;
        if (D_0014171B_309688[moby->mission + row] == 0xFF) {
            if (data->idx != -1) ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 1;
            func_0020D678_309688(moby);
            return;
        }
    }
    has_next = 0;
    hit = func_L00_0025B478_309688(moby, 0x40000, 0);
    if (func_L00_001FEF78_309688(&moby->command) == 0) return;
    if (hit == 0) return;
    if (moby->oclass < 0x508) has_next = func_L09_003095D0_309688(moby, moby->oclass + 1) != 0;
    moby->command = func_001F9850_309688(10);
    qcopy(origin, ((f32 *)&moby->pos));
    qcopy(center, ((f32 *)&moby->pos));
    if (moby->oclass < 0x508) {
        func_0022ED80_309688(0, 0, moby);
    } else {
        func_0022ED80_309688(0, 0, moby);
    }
    moby->hit_slot = 0xFF;
    count = 40;
    if (0.85f < D_L09_0015F5D0 || 0.85f < D_L09_0015F5D4) count = 30;
    angle = -3.1415927f;
    for (i = 0; i < count; i++) {
        *(u128 *)vel = 0;
        vel[0] = func_002140F8_309688(-0.4f, 0.4f);
        vel[1] = func_002140F8_309688(-0.6f, 0.6f);
        vel[2] = func_002140F8_309688(-0.4f, 0.4f);
        *(u128 *)piece_pos = *(u128 *)vel;
        scale = func_002140F8_309688(0.3f, 2.6f);
        piece_pos[0] = piece_pos[0] + scale * func_001F9F90_309688(angle);
        scale = scale * func_001F9FA8_309688(angle);
        piece_pos[2] = piece_pos[2] + scale;
        func_001F9EC0_309688(piece_pos, piece_pos, ((f32 *)&moby->rot_row0));
        fast_vec_add(piece_pos, piece_pos, center);
        fast_vec_sub(vel, piece_pos, origin);
        scale = func_001F9B88_309688(6.0f - fast_vec_length(vel)) / 6.0f;
        func_L00_001FF4B0_309688(push, hit->pos, scale * scale * 8.0f);
        fast_vec_add(vel, vel, push);
        func_L00_001FF4B0_309688(vel, vel, scale * (func_002140F8_309688(5.0f, 8.0f) * frame_time));
        if (moby->oclass == 0x508) {
            int odd = i & 1;
            func_L09_002F0BB8_309688(moby, piece_pos, vel, odd, func_002140F8_309688(1.0f, 1.3f));
        }
        if (i % 3 == 0) {
            func_L00_00260108_309688(moby, piece_pos, -1, func_002140F8_309688(0.3f, 0.6f), 0.0f);
        }
        angle = angle + 6.2831855f / func_001FA888_309688(count);
    }
    fast_vec_scale(piece_pos, hit->pos, 0.5f);
    func_L00_0025F4A8_309688(moby, hit->pos, origin, 0.0f, 0.0f, 10, 3, 16, 3.0f, 2.0f, 1.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
    if (moby->oclass == 0x508) {
        if (data->idx != -1) ((Flags_309688 *)(alternate_item_available + data->idx))->f43 = 1;
    }
    if (moby->oclass == 0x508 || has_next) func_0020D678_309688(moby);
}
#include "qzero.h"

/* Level 9 moby update: 0 sets up and either rises from below or reports a missing target, 1 waits for the target
 * moby to die, 2 rides with the hero (wobbles data+0x60/0x64 towards the hero's offset) or settles back.
 * The settling branch stores the height through a struct member (Mpos): a member store does not make the compiler
 * reload D_0015EE6C, a cast store does. The hero block is offsets of D_0013E633 cast to a struct, as retail
 * derives 0xE1D from the register holding 0xE9D. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_0030A560), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef int u128_30A560 __attribute__((mode(TI)));

typedef struct {
    char pad0[0x18];
    float z;
} Mpos_30A560;

extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern int D_L09_0015FFD8_c __asm__("D_L09_0015FFD8");
extern float D_L09_00161DC4 __attribute__((sda));
extern float D_L09_00161DC8 __attribute__((sda));
extern float D_L09_00161DCC __attribute__((sda));
extern float D_L09_00161DD4 __attribute__((sda));
extern float D_L09_00161DD8 __attribute__((sda));
extern float D_L09_00161DDC __attribute__((sda));
extern float D_L09_00161DE0 __attribute__((sda));
extern float D_L09_00161DE4 __attribute__((sda));
extern float D_L09_00161DE8 __attribute__((sda));
extern int DebugPrint_alt() __asm__("FUN_001e93b0");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern unsigned char D_0013F350[];
extern void FUN_L00_00260738_c(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_00262b00(float, float, char *, float *, float *);
extern void FUN_L00_00262b80(void *, char *, char *, float, float, float);
extern void scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
float FUN_L00_0025bc98(float *p, float *q, int n, float x, float a, float b, float c);
void FUN_L07_00288968(void *a0, void *a1, void *a2, void *a3, void *a4);
extern void func_001F9C30_30A560(void *, void *, float) __asm__("FUN_001f9a68");
extern float func_002140F8_30A560(float, float) __asm__("FUN_002132a8");
extern void func_001E9730_30A560() __asm__("FUN_001e93b0");
extern int func_001F9908_30A560(int *) __asm__("FUN_001f9740");
extern int func_0022ED80_30A560(int, int, char *) __asm__("FUN_0022da68");
extern int func_001F9850_30A560(int) __asm__("FUN_001f96f8");
extern void func_001F9BF0_30A560(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern float func_L00_0025CCF0_30A560(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern void func_L07_00289960_30A560(void *a0, void *a1, void *a2, void *a3, void *a4) __asm__("FUN_L07_00288968");
extern void func_L00_00263B78_30A560(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void func_L00_00263BF8_30A560(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern float func_001FA748_30A560(float, float) __asm__("FUN_001fa580");
extern void func_001F9BD8_30A560(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_L00_002617B0_30A560(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern char D_L09_00209518_30A560[] __asm__("D_L09_00209228");
extern unsigned char D_0013E633_30A560[] __asm__("D_0013F350");
extern unsigned char D_0013F3D0[];

void FUN_L09_003091b0(char *moby) {
    char *data = *(char **)(moby + 0x78);
    float movement[4];
    float original_rotation[4];
    float hero_offset[4];
    float old_angles[4];
    float old_pos[4];
    float carried_pos[4];
    float start_z, new_z, sink_level, old_z;
    int state;
    char *linked;
    float side_dot, front_dot;
    unsigned char linked_state;

    func_001F9C30_30A560(movement, moby + 0x10, -1.0f);
    qcopy(original_rotation, moby + 0x40);
    old_z = *(float *)(moby + 0x18);
    sink_level = *(float *)(data + 0x84) - 0.300000012f;
    state = ((unsigned char *)moby)[0x20];
    switch (state) {
    case 0:
        *(int *)(data + 0x80) = 1;
        qcopy(data + 0x60, moby + 0x40);
        qzero(data + 0x70);
        start_z = *(float *)(moby + 0x18);
        *(float *)(data + 0x84) = start_z;
        if (*(int *)(data + 0x8C) != -1) {
            *(float *)(moby + 0x18) = start_z - 5.0f;
            *(float *)(moby + 0x40) = func_002140F8_30A560(-1.57079637f, 1.57079637f);
            *(float *)(moby + 0x44) = func_002140F8_30A560(-1.57079637f, 1.57079637f);
            moby[0x20] = 1;
        } else {
            func_001E9730_30A560(D_L09_00209518_30A560, *(short *)(moby + 0xB2), *(short *)(moby + 0xA6));
            moby[0x20] = 2;
        }
        break;
    case 1:
        linked = (char *)(D_L09_0015FFD8_c + (*(int *)(data + 0x8C) << 8));
        if (linked == 0) {
            goto set2;
        }
        linked_state = ((unsigned char *)linked)[0x20];
        if (linked_state == 0xFE) {
            goto set2;
        }
        if (linked_state != 0xFD) {
            break;
        }
    set2:
        moby[0x20] = 2;
        break;
    case 2:
        if (((Hero *)D_0013E633_30A560)->air_frames.s == 0 && ((char *)((Hero *)D_0013E633_30A560)->ground_moby) == moby) {
            if (*(int *)(data + 0x88) == 0) {
                func_0022ED80_30A560(0, 0, moby);
            }
            if (*(int *)(data + 0x88) < func_001F9850_30A560(30)) {
                *(int *)(data + 0x88) = func_001F9850_30A560(30);
            }
        }
        if (*(int *)(data + 0x88) != 0) {
            float deg_to_rad;
            float tilt;
            float pitch_target;
            func_001F9908_30A560((int *)(data + 0x88));
            func_001F9BF0_30A560(hero_offset, (D_0013E633_30A560 + 0x80), moby + 0x10);
            deg_to_rad = 0.0174532924f;
            side_dot = fast_vec_dot(hero_offset, moby + 0xC0);
            front_dot = fast_vec_dot(hero_offset, moby + 0xD0);
            tilt = D_L09_00161DC4;
            pitch_target = tilt * side_dot;
            *(u128_30A560 *)old_angles = *(u128_30A560 *)(data + 0x60);
            *(u128_30A560 *)old_pos = *(u128_30A560 *)(moby + 0x10);
            func_L00_0025CCF0_30A560(data + 0x60, -tilt * front_dot, data + 0x70, 0,
                              D_L09_00161DD4 * deg_to_rad * frame_time_sq,
                              D_L09_00161DD8 * frame_scale_sq,
                              D_L09_00161DDC * deg_to_rad * frame_time);
            func_L00_0025CCF0_30A560(data + 0x64, pitch_target, data + 0x74, 0,
                              D_L09_00161DD4 * deg_to_rad * frame_time_sq,
                              D_L09_00161DD8 * frame_scale_sq,
                              D_L09_00161DDC * deg_to_rad * frame_time);
            new_z = *(float *)(moby + 0x18) - D_L09_00161DC8 * frame_time;
            *(float *)(moby + 0x18) = new_z;
            if (sink_level < old_z && new_z <= sink_level) {
                func_0022ED80_30A560(1, 0, moby);
            }
            func_L07_00289960_30A560((D_0013E633_30A560 + 0x80), carried_pos, moby + 0x10, old_angles, data + 0x60);
            func_001F9BF0_30A560(D_0013E633_30A560 + 0xF0, carried_pos, (D_0013E633_30A560 + 0x80));
            ((Hero *)D_0013E633_30A560)->motion.platform_delta.f[2] = ((Hero *)D_0013E633_30A560)->motion.platform_delta.f[2] + (*(float *)(moby + 0x18) - old_pos[2]);
        } else {
            float rise;
            ((Mpos_30A560 *)moby)->z = ((Mpos_30A560 *)moby)->z + (rise = D_L09_00161DCC * frame_time) * (*(float *)(data + 0x84) - ((Mpos_30A560 *)moby)->z);
            if (((Mpos_30A560 *)moby)->z < *(float *)(data + 0x84) - 2.0f) {
                float deg_to_rad;
                ((Mpos_30A560 *)moby)->z = ((Mpos_30A560 *)moby)->z + rise;
                deg_to_rad = 0.0174532924f;
                func_L00_0025CCF0_30A560(data + 0x60, 0.0f, data + 0x70, 0,
                                  D_L09_00161DE0 * deg_to_rad * frame_time_sq * 3.0f,
                                  D_L09_00161DE4 * frame_scale_sq,
                                  frame_time * 1.57079637f);
                func_L00_0025CCF0_30A560(data + 0x64, 0.0f, data + 0x74, 0,
                                  D_L09_00161DE0 * deg_to_rad * frame_time_sq * 3.0f,
                                  D_L09_00161DE4 * frame_scale_sq,
                                  frame_time * 1.57079637f);
            } else {
                float deg_to_rad;
                deg_to_rad = 0.0174532924f;
                func_L00_0025CCF0_30A560(data + 0x60, 0.0f, data + 0x70, 0,
                                  D_L09_00161DE0 * deg_to_rad * frame_time_sq,
                                  D_L09_00161DE4 * frame_scale_sq,
                                  D_L09_00161DE8 * deg_to_rad * frame_time);
                func_L00_0025CCF0_30A560(data + 0x64, 0.0f, data + 0x74, 0,
                                  D_L09_00161DE0 * deg_to_rad * frame_time_sq,
                                  D_L09_00161DE4 * frame_scale_sq,
                                  D_L09_00161DE8 * deg_to_rad * frame_time);
            }
            if (((Mpos_30A560 *)moby)->z > *(float *)(data + 0x84)) {
                ((Mpos_30A560 *)moby)->z = *(float *)(data + 0x84);
            }
            if (old_z < sink_level) {
                if (sink_level <= ((Mpos_30A560 *)moby)->z) {
                    func_0022ED80_30A560(2, 0, moby);
                }
            }
        }
        func_L00_00263B78_30A560(0.140000001f, frame_time * 0.52359879f, moby, (float *)(data + 0x98), (float *)(data + 0x9C));
        func_L00_00263BF8_30A560(moby, data + 0x90, data + 0x94, 0.0872664601f,
                          frame_time * 0.436332315f, frame_time * 0.541052043f);
        *(float *)(moby + 0x40) = func_001FA748_30A560(*(float *)(data + 0x60), *(float *)(moby + 0x40));
        *(float *)(moby + 0x44) = func_001FA748_30A560(*(float *)(data + 0x64), *(float *)(moby + 0x44));
        func_001F9BD8_30A560(movement, movement, moby + 0x10);
        func_L00_002617B0_30A560(data + 0x20, movement, original_rotation, moby + 0x40);
        break;
    }
}
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 node[1];
} PathNodes_00309c08;
typedef struct {
    u8 pad0[0x70];
    u8 unk70[0x38];
    f32 speed;
    s32 path;
    u8 padB0[8];
    s16 dir;
    s16 node;
    f32 yaw_offset;
    f32 spin;
    u8 padC4[8];
    f32 lag;
} PathFlyerVars_00309c08;
extern void attach_manipulator(struct Moby *, s32, void *) __asm__("FUN_0020cb10");
extern void scale_vector_00309c08(f32, void *, void *) __asm__("FUN_001f9a68");
extern s32 check_voice_target_00309c08(void *, s32) __asm__("FUN_L00_0028d8c0");
extern void FUN_L00_00260738(void *, void *, void *, void *);
extern u8 D_0013F3D0[];
extern u8 D_0014C050[];
extern s32 D_L09_0015F5CC __attribute__((section(".data")));
extern f32 atan2_f(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_L09_0030a298(struct Moby *);

void FUN_L09_00309c08(struct Moby *moby) {
    PathFlyerVars_00309c08 *vars = (PathFlyerVars_00309c08 *)moby->pvars;
    PathNodes_00309c08 *path = (PathNodes_00309c08 *)D_L09_001B0630[vars->path];
    Vec4 *position = &moby->pos;
    Vec4 original;
    Vec4 rotation;
    s16 dir;
    s32 voice;

    scale_vector_00309c08(-1.0f, &original, position);
    qcopy(&rotation, &moby->rot);
    if (vars == 0) return;

    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        vars->yaw_offset = 3.1415927f;
        *(s32 *)((u8 *)vars + 0xC4) = -1;
        *(s32 *)((u8 *)vars + 0xC8) = 0;
        vars->lag = 0.0f;
        if (*(s32 *)((u8 *)vars + 0xA0) != 0) {
            PathNodes_00309c08 *path;
            Vec4 *endpoint;
            attach_manipulator(moby, 0, (u8 *)vars + 0x60);
            path = (PathNodes_00309c08 *)D_L09_001B0630[vars->path];
            moby->state = 1;
            if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
                vars->dir = -1;
                vars->node = path->count - 2;
                endpoint = (Vec4 *)((path->count << 4) + (s32)path);
            } else {
                vars->dir = 1;
                endpoint = &path->node[0];
                vars->node = 1;
            }
            *position = *endpoint;
            moby->rot.z = atan2_f(path->node[vars->node].f[0] - moby->pos.x,
                                   path->node[vars->node].f[1] - moby->pos.y);
        }
        moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
        break;
    case 2:
        vars->lag += (1.0f - vars->lag) * 0.01f;
        FUN_L00_00262b00(vars->lag * 0.14f, frame_time * 0.5235988f,
                          (char *)moby, (f32 *)((u8 *)vars + 0xD8), (f32 *)((u8 *)vars + 0xDC));
        FUN_L00_00262b80(moby, (char *)vars + 0xD0, (char *)vars + 0xD4,
                          vars->lag * 0.08726646f, frame_time * 0.31415927f,
                          frame_time * 0.4886922f);
        if (hero.ground_moby != moby) moby->state = 1;
        break;
    case 1:
        vars->lag += (1.0f - vars->lag) * 0.01f;
        FUN_L00_00262b00(vars->lag * 0.14f, frame_time * 0.5235988f,
                          (char *)moby, (f32 *)((u8 *)vars + 0xD8), (f32 *)((u8 *)vars + 0xDC));
        FUN_L00_00262b80(moby, (char *)vars + 0xD0, (char *)vars + 0xD4,
                          vars->lag * 0.08726646f, frame_time * 0.31415927f,
                          frame_time * 0.4886922f);
        if (*(s32 *)((u8 *)vars + 0xC8) != 0) {
            if ((vars->dir > 0 && *(s32 *)((u8 *)vars + 0xB4) != -1 && is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB4))) ||
                (vars->dir < 0 && *(s32 *)((u8 *)vars + 0xB0) != -1 && is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB0)))) {
                moby->state = 3;
            }
        }
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            *(s32 *)((u8 *)vars + 0xC8) = 1;
            moby->state = 3;
        }
        break;
    case 3:
        if ((D_L09_0015F5CC & 7) == (((s32)moby >> 8) & 7)) {
            if (*(s32 *)((u8 *)vars + 0xC4) == -1 ||
                (!check_voice_target_00309c08(moby, *(s32 *)((u8 *)vars + 0xC4)) &&
                 *(s32 *)((u8 *)vars + 0xC4) == -1)) {
                *(s32 *)((u8 *)vars + 0xC4) = allocate_voice_for_target_entry(0, 4, moby);
            }
        }
        if (vars->yaw_offset == 0.0f) {
            if (vars->dir < 0) {
                if (*(s32 *)((u8 *)vars + 0xB4) == -1 ||
                    !is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB4)))
                    goto test_forward;
                if (!(vector_distance(&moby->pos, &path->node[path->count - 1]) > 8.0f) ||
                    !(vector_distance(&moby->pos, &path->node[0]) > 8.0f))
                    goto advance_path;
                vars->dir = 1;
                vars->yaw_offset = 3.1415927f;
            } else {
            test_forward:
                if (vars->dir > 0 && *(s32 *)((u8 *)vars + 0xB0) != -1 &&
                    is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)vars + 0xB0)) &&
                    vector_distance(&moby->pos, &path->node[path->count - 1]) > 8.0f &&
                    vector_distance(&moby->pos, &path->node[0]) > 8.0f) {
                    vars->dir = -1;
                    vars->yaw_offset = 3.1415927f;
                }
            }
        }
    advance_path:
        if (FUN_L09_0030a298(moby)) {
            voice = *(s32 *)((u8 *)vars + 0xC4);
            if (voice != -1) {
                u8 *entry = (u8 *)D_0013E550 + voice * 0x70;
                if (*(struct Moby **)(entry + 0x88) == moby && entry[0x74] != 0)
                    release_voice_slot(voice);
            }
            *(s32 *)((u8 *)vars + 0xC4) = -1;
            vars->lag = 0.0f;
            *(s32 *)((u8 *)vars + 0xDC) = 0;
            moby->state = 2;
            vars->yaw_offset = 3.1415927f;
            dir = vars->dir;
            vars->dir = -dir;
            if ((s16)-dir > 0) vars->node = 0;
            else vars->node = path->count - 1;
        }
        break;
    }
    add_vector_xyz(&original, &original, &moby->pos);
    FUN_L00_00260738((u8 *)vars + 0x20, &original, &rotation, &moby->rot);
}
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_0030B5E8.c: func_L09_0030B5E8), where it is exact; names translated to the US level program. */

void FUN_L09_0030a238(float *dst, float a, float b, float scale) {
    float v = fast_subtract_rotations(b, a) * scale;
    *dst = v;
    *dst = fast_add_rotations(v, a);
}
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 node[1];
} PathNodes;

typedef struct {
    u8 pad0[0x70];
    u8 unk70[0x38];
    f32 speed;          /* 0xA8 */
    s32 path;           /* 0xAC: index into D_L09_001B0630 */
    u8 padB0[8];
    s16 dir;            /* 0xB8: > 0 runs the path forward, else backward */
    s16 node;           /* 0xBA: node being flown to */
    f32 yaw_offset;     /* 0xBC */
    f32 spin;           /* 0xC0 */
    u8 padC4[8];
    f32 lag;            /* 0xCC: eases to 0, slowing the turn while high */
} PathFlyerVars;

extern f32 D_L09_00161DF8 __attribute__((sda));
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern float atan2_f(float, float) __asm__("FUN_001f9e90");
extern float advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern void FUN_L00_001fff28(void *, int, float);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void FUN_L00_0025f8e0(void *, f32);
extern void FUN_L00_002715e8(void *, int, int, float, float);

/* Flies the moby along its path toward the current node, turning and banking
 * toward it, steps the node at each arrival and now and then drops a puff.
 * Returns nonzero once it has run off the end of the path. */
int FUN_L09_0030a298(struct Moby *moby) {
    PathFlyerVars *vars = (PathFlyerVars *)moby->pvars;
    PathNodes *path;
    f32 d;
    Vec4 target;
    Vec4 step;
    Vec4 puff;
    int done = 0;

    path = (PathNodes *)D_L09_001B0630[vars->path];
    vars->lag += (0.0f - vars->lag) * 0.005f;
    qcopy(&target, &path->node[vars->node]);
    target.f[2] += vars->speed * D_L09_00161DF8;
    subtract_vector_xyz(&step, &target, &moby->pos);
    normalize_vector_xyz(&step, &step, vars->speed);
    add_vector_xyz(&moby->pos, &moby->pos, &step);
    FUN_L09_0030a238(&moby->rot.z, moby->rot.z,
                     fast_add_rotations(atan2_f(path->node[vars->node].f[0] - moby->pos.x,
                                         path->node[vars->node].f[1] - moby->pos.y),
                                 vars->yaw_offset),
                     (1.0f - vars->lag) * 0.02f);
    FUN_L09_0030a238(&moby->rot.x, moby->rot.x, 0.0f, 0.02f);
    FUN_L09_0030a238(&moby->rot.y, moby->rot.y, 0.0f, 0.02f);
    if (vars->dir > 0) {
        f32 v;
        if (FUN_001f9b48(&target, &moby->pos) < vars->speed + vars->speed) {
            vars->node++;
            if (vars->node == path->count)
                done = 1;
        }
        d = FUN_001f9b48(&moby->pos, &path->node[path->count - 1]);
        v = 0.0f;
        advance_accelerated_scalar(d, frame_time_sq * 3.0f, frame_time_sq * 3.0f, frame_time * 10.0f, &v, &vars->speed);
    } else {
        f32 v;
        if (FUN_001f9b48(&target, &moby->pos) < vars->speed + vars->speed) {
            vars->node--;
            if (vars->node == -1)
                done = 1;
        }
        d = FUN_001f9b48(&moby->pos, &path->node[0]);
        v = 0.0f;
        advance_accelerated_scalar(d, frame_time_sq * 3.0f, frame_time_sq * 3.0f, frame_time * 10.0f, &v, &vars->speed);
    }
    if (path->node[vars->node].f[3] == 42.0f)
        vars->yaw_offset = 0.0f;
    vars->spin = fast_add_rotations(vars->spin, vars->speed);
    FUN_L00_001fff28(vars->unk70, 0, vars->spin);
    if (vector_distance(&moby->pos, &path->node[path->count - 1]) > 8.0f &&
        vector_distance(&moby->pos, &path->node[0]) > 8.0f && !random_integer_below(7)) {
        qcopy(&puff, &moby->pos);
        FUN_L00_0025f8e0(&puff, 0.5f);
        puff.f[2] = 25.01f;
        FUN_L00_002715e8(&puff, 0, 0x40103080, vars->speed * 3.0f / (frame_time * 10.0f), 12600.0f);
    }
    return done;
}
