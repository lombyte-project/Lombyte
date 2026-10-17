/* Level 14 moby classes: platform, blarg_heavy_interceptor, machinegun_turret, minibomber, plasmabot. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/data_labels.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

#define NOT_SDA

#define MACRO_ADDR

/* Attach the callback during the six active phases of this level event. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/initonce_00246A90.c: func_L14_00246A90), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x30];
    int state;
    int phase;
    char pad38[0x144];
    void *moby;
} Level14State;

extern Level14State D_L14_0016CF60;
extern void FUN_L00_0023e008(void *);
extern void func_L00_0023E4C8(void *) __asm__("FUN_L14_002459c0");

void FUN_L14_002460f8(void) {
    void *moby;
    if (current_level_index != 14 || D_L14_0016CF60.state != 0) {
        return;
    }
    moby = D_L14_0016CF60.moby;
    if (moby != 0 && (unsigned)(D_L14_0016CF60.phase - 0x1C) < 6) {
        FUN_L00_0023e008(moby);
        add_draw_callback(func_L00_0023E4C8, moby);
    }
}

extern u16 *D_L14_001ABF40_u[] __asm__("D_L14_001ABF40");
extern u8 *D_L14_0015FFD8;
extern void FUN_L00_0023e838(s32);
extern void FUN_0020c828(void *);

/* Walk the record list at D_L14_001ABF40_u[index] (terminated by a negative
 * entry) and flag the objects whose state byte at 0x20 is 1. */
void FUN_L14_002b44a8(s32 index) {
    u16 *p = D_L14_001ABF40_u[index];
    u8 *base;
    u8 *o;

    if (p == 0) {
        return;
    }
    base = D_L14_0015FFD8;
    do {
        o = base + ((*p & 0x7FFF) << 8);
        if (o[0x20] == 1) {
            *(s16 *)(*(u8 **)(o + 0x78) + 0x88) = 1;
        }
    } while (*(s16 *)p++ >= 0);
}

/* Release the handle stored at 0x12 of the record, then hand the moby over. */
void FUN_L14_002bb4d8(u8 *self) {
    u8 *p = *(u8 **)(self + 0x78);
    s16 v = *(s16 *)(p + 0x12);

    if (v != -1) {
        FUN_L00_0023e838(v);
        *(s16 *)(p + 0x12) = -1;
    }
    FUN_0020c828(self);
}

#include "rnc/gameplay/entities/moby.h"
#include "rnc/audio/voice_pool.h"

/* Path mover gated by an activation trigger and its collectable flag: slides between the two ends of its path,
   eases towards the path point and carries what stands on it. */
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4 points[1];
} MoverPath;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    s16 unk24;
    u8 pad26[2];
    u8 unk28;
    u8 pad29[0x15];
    s16 unk3E;
    u8 pad40[0x20];
    u8 carry[0x40];      /* 0x60: carried-object state for FUN_L00_00260738 */
    u8 start_end;        /* 0xA0: 0 starts at the far end */
    u8 padA1[3];
    f32 t;               /* 0xA4: position along the path, 0 to 1 */
    f32 speed;           /* 0xA8 */
    f32 target_speed;    /* 0xAC: +-1 / travel frames */
    f32 travel_time;     /* 0xB0: seconds per trip */
    s32 path;            /* 0xB4: index into D_L14_001B0BB0, -1 for none */
    s16 retrigger;       /* 0xB8: set when the hero steps off while stopped */
    s16 wait;            /* 0xBA: frames before it may move again */
    f32 rise;            /* 0xBC: height gained this frame */
    s32 trigger;         /* 0xC0: clip volume that activates it, -1 once active */
    s32 voice;           /* 0xC4 */
    s32 silent;          /* 0xC8 */
    s32 show_volume;     /* 0xCC: clip volume that makes it visible, -1 once shown */
    f32 ease[3];         /* 0xD0: per-axis easing state for FUN_L00_0025b8c0 */
} PathMoverVars;

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L14LevelState;

extern int *D_L14_001B0BB0[];
extern L14LevelState D_L14_001BB930;
extern s32 D_L14_001BABD0[];
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32 f) __asm__("FUN_001fa6d0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9b48(void *, void *);
extern s32 FUN_001f9770(s16 *);
extern int FUN_L00_0028d8c0(void *, int);
extern int FUN_L01_00277fb8(void *);
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 FUN_L00_0025b8c0(f32 *, f32 *, f32, f32, f32, f32);
extern void FUN_L14_002ac2b8(struct Moby *);
extern void FUN_L00_00260738(void *, void *, void *, void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("FUN_001f99c0");

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
void FUN_L14_002aba80(struct Moby *moby) {
    PathMoverVars *vars = (PathMoverVars *)moby->pvars;
    MoverPath *path;
    Vec4 orig;
    Vec4 delta;
    Vec4 step;
    Vec4 goal;
    s32 on = 0;

    if (vars->path == -1) {
        return;
    }
    path = (MoverPath *)D_L14_001B0BB0[vars->path];
    if (vars->show_volume != -1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, vars->show_volume) == 0) {
            moby->update_dist = 0xFF;
            moby->collision = 0;
            moby->flags |= 0x41;
            return;
        }
        if (vars->show_volume != -1 && is_point_inside_clip_volume(&hero.motion.pos, vars->show_volume)) {
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            vars->show_volume = -1;
        }
    }
    {
        u16 id;

        if (vars->trigger == -1 || is_point_inside_clip_volume(&hero.motion.pos, vars->trigger) ||
            D_L14_001BB930.collected[(s16)(id = moby->save_id)] != 0 ||
            (D_0014C190_257218[current_level_index][(s16)id >> 5] >> (id & 0x1F)) & 1) {
            if (vars->trigger != -1) {
                D_0014C190_257218[current_level_index][(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                D_L14_001BABD0[(s16)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                vars->trigger = -1;
            }
            on = 1;
        }
    }
    qcopy(&orig, &moby->pos);
    FUN_001f9770(&vars->wait);
    if (moby->state == 0) {
        f32 t;
        Vec4 *points;

        vars->unk28 = 4;
        vars->unk3E = 5;
        vars->unk20 = 0;
        vars->unk24 = 0;
        t = 0.0f;
        if ((moby->command = vars->start_end) == 0) {
            t = 1.0f;
        }
        vars->t = t;
        points = path->points;
        /* the unsigned index keeps retail's (count - 1) << 4; a signed one folds the -1 into the pointer */
        qcopy(&moby->pos, truncate_float_to_s32(t) ? &points[(u32)(path->count - 1)] : points);
        vars->wait = 0;
        vars->speed = 0.0f;
        vars->rise = 0.0f;
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        vars->voice = -1;
    }
    switch (moby->command) {
    case 0:
        if (on) {
            f32 far;

            if ((vars->retrigger != 0 && FUN_L01_00277fb8(moby)) ||
                (far = FUN_001f9b48(&hero.motion.pos, &path->points[path->count - 1]),
                 FUN_001f9b48(&hero.motion.pos, &path->points[0]) < far)) {
                moby->command = 2;
                vars->target_speed = -1.0f / multiply_global_factor_ed64(vars->travel_time * 60.0f);
                vars->retrigger = 0;
            }
        }
        break;
    case 1:
        if (on) {
            f32 near;

            if ((vars->retrigger != 0 && FUN_L01_00277fb8(moby)) ||
                (near = FUN_001f9b48(&hero.motion.pos, &path->points[0]),
                 FUN_001f9b48(&hero.motion.pos, &path->points[path->count - 1]) < near)) {
                moby->command = 2;
                vars->target_speed = 1.0f / multiply_global_factor_ed64(vars->travel_time * 60.0f);
                vars->retrigger = 0;
            }
        }
        break;
    case 2:
        if (vars->silent == 0 && FUN_L01_00277fb8(moby)) {
            hero.unk1F2 = 4;
            hero.unk1F4 = 4;
        }
        if (vars->wait != 0 ||
            (vars->rise < 0.0f && FUN_001f9b80(&moby->pos, &hero.motion.pos) < 2.0f &&
             absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f &&
             moby->pos.z - hero.motion.pos.f[2] > 1.0f)) {
            s32 voice;

            if (vars->wait == 0) {
                vars->wait = scale_game_frames(30);
            }
            voice = vars->voice;
            vars->speed = 0.0f;
            if (voice != -1) {
                VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

                if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                    release_voice_slot(voice);
                }
            }
            vars->voice = -1;
        } else {
            s32 i;
            f32 frac;
            f32 fi;
            Vec4 *from;

            if (!FUN_L00_0028d8c0(moby, vars->voice)) {
                vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
            vars->speed += vars->target_speed * frame_time;
            if (absolute_float(vars->speed) > absolute_float(vars->target_speed)) {
                vars->speed = vars->target_speed;
            }
            vars->t = vars->t + vars->speed;
            if (0.5f < absolute_float(vars->t - 0.5f)) {
                s32 voice;

                if (0.0f < vars->target_speed) {
                    moby->command = 0;
                    vars->t = 1.0f;
                } else {
                    moby->command = 1;
                    vars->t = 0.0f;
                }
                vars->speed = 0.0f;
                vars->wait = scale_game_frames(15);
                voice = vars->voice;
                if (voice != -1) {
                    VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

                    if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                        release_voice_slot(voice);
                    }
                }
                vars->voice = -1;
            }
            frac = convert_integer_to_float(path->count - 1);
            i = truncate_float_to_s32(frac * vars->t);
            frac = convert_integer_to_float(path->count - 1);
            fi = convert_integer_to_float(i);
            frac *= vars->t;
            frac -= fi;
            if (i == path->count - 1) {
                qcopy(&goal, &path->points[i]);
            } else {
                from = &path->points[i];
                subtract_vector_xyz(&step, &path->points[i + 1], from);
                scale_vector_xyz(&step, &step, frac);
                add_vector_xyz(&goal, &step, from);
            }
            FUN_L00_0025b8c0(&moby->pos.x, &vars->ease[0], goal.f[0], 0.005f, 0.2f, 0.0f);
            FUN_L00_0025b8c0(&moby->pos.y, &vars->ease[1], goal.f[1], 0.005f, 0.2f, 0.0f);
            FUN_L00_0025b8c0(&moby->pos.z, &vars->ease[2], goal.f[2], 0.005f, 0.2f, 0.0f);
            vars->rise = moby->pos.z - orig.f[2];
        }
        break;
    }
    if (moby->command != 2) {
        FUN_L14_002ac2b8(moby);
    }
    subtract_vector_xyz(&delta, &moby->pos, &orig);
    FUN_L00_00260738(vars->carry, &delta, &moby->rot, &moby->rot);
    if (!FUN_L01_00277fb8(moby)) {
        if (2.0f < FUN_001f9b80(&moby->pos, &hero.motion.pos)) {
            if (vars->wait == 0) {
                vars->retrigger = 1;
            }
        }
    }
}
/* Eases the moby toward the first or last point of its path. */

extern int *D_L14_001B0BB0[];
extern f32 FUN_L00_0025b8c0(f32 *, f32 *, f32, f32, f32, f32);

void FUN_L14_002ac2b8(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    float t[4] __attribute__((aligned(16)));
    int *p = D_L14_001B0BB0[*(int *)(d + 0xB4)];
    if (*(float *)(d + 0xA4) > 0.5f)
        qcopy(t, (char *)p + p[0] * 16);
    else
        qcopy(t, (char *)p + 0x10);
    FUN_L00_0025b8c0(&moby->pos.x, (float *)(d + 0xD0), t[0], 0.005f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(&moby->pos.y, (float *)(d + 0xD4), t[1], 0.005f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(&moby->pos.z, (float *)(d + 0xD8), t[2], 0.005f, 0.2f, 0.0f);
}
/* Per-frame update of a path-following flyer, a state machine on m->state: 0 checks its three paths
 * and sets it up (or hides it, state 8), 1 patrols the path in pvars 0x78 and scans for a target,
 * 2 drops the engine sound and alerts its group, 3 chases the locked-on target, 4 waits on the
 * group trigger, 5 and 6 fly the path in 0x204 and rejoin the patrol path, 7 is the hit that sends it
 * tumbling and 9 the fall that ends in a burst of debris. 8 only draws. */
extern void FUN_L14_002ad8b8(struct Moby *);
extern void FUN_L14_002ae448(struct Moby *);
extern void FUN_L14_002ae6d8(struct Moby *);
extern void FUN_L14_002ae7d8(struct Moby *);
extern void FUN_L14_002ae900(struct Moby *, float, float, float);
extern void FUN_L14_002aea08_c618(struct Moby *) __asm__("FUN_L14_002aea08");
extern void FUN_L14_002aead8_c618(struct Moby *) __asm__("FUN_L14_002aead8");
extern int FUN_L14_002aeba0(struct Moby *, int);
extern void FUN_L14_002aed50_c618(struct Moby *) __asm__("FUN_L14_002aed50");
extern void FUN_L14_002aee28(struct Moby *);
extern int FUN_L14_002af048(struct Moby *);
extern int FUN_L14_002b4500(int);
extern int FUN_L14_002b4670(int);
extern void blend_moby_animation_c618(struct Moby *, int, int, int) __asm__("FUN_00212f90");
extern u8 *FUN_002141f8_c618(struct Moby *) __asm__("FUN_002141f8");
extern void FUN_L00_0025a120(void *);
extern void memset_c618(void *, int, int) __asm__("FUN_001f97e8");
extern void attach_manipulator_c618(void *, s32, void *) __asm__("FUN_0020cb10");
extern void lerp_vector_c618(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void FUN_001e93e8_c618(void *, float) __asm__("FUN_001e93e8");
extern int FUN_L00_001f2868_c618(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_001efa68_c618(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void FUN_L00_0024f7c8_c618(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_001fff28(void *, int, float);
extern f32 ease_c618(f32 *pos, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025b8c0");
extern int FUN_L00_0025df68_c618(void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern int FUN_L00_0025d808_c618(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern void FUN_L00_0025e450_c618(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void subtract_vector_c618(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_c618(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_c618(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_vectors_c618(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void build_look_at_matrix_c618(void *, void *, void *, float) __asm__("FUN_00214890");
extern f32 random_float_c618(f32, f32) __asm__("FUN_002132a8");
extern f32 randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern unsigned char *FUN_L00_0026d000_c618(void *pos, float r, float lo, float hi, int n, void *vel, float size, int color) __asm__("FUN_L00_0026d000");
extern void FUN_L00_0025c398_c618(void *, void *, int) __asm__("FUN_L00_0025c398");
extern s32 allocate_voice_c618(s32, s32, void *) __asm__("FUN_0022da68");
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern s32 random_integer_below(s32 n) __asm__("FUN_00213260");
extern f32 FUN_001fa6c0(s16);
extern s32 FUN_001fa728(u8 *, f32);
typedef struct {
    u8 pad0[0x18];
    struct Moby *target; /* 0x18 */
} LockOn_c618;
extern LockOn_c618 D_L14_00174540;
extern char D_L14_001674C0[];
extern f32 D_L14_0016139C __attribute__((sda));
extern char *D_001413D0_c618 __asm__("D_001413D0") __attribute__((section(".data")));
extern char D_L14_0015F580[] __attribute__((section(".sdata")));
extern f32 D_0015ED60_c618 __asm__("D_0015ED60");
extern s32 D_L14_001613D0 __attribute__((sda));
extern s32 D_L14_001613D4 __attribute__((sda));
extern f32 D_L14_001613D8 __attribute__((sda));
extern f32 D_L14_001613DC __attribute__((sda));
extern f32 D_L14_001613E0 __attribute__((sda));
extern f32 D_L14_001613E4 __attribute__((sda));
extern f32 D_L14_001613E8 __attribute__((sda));
extern f32 D_L14_001613EC __attribute__((sda));
extern f32 D_L14_001613F0 __attribute__((sda));
extern f32 D_L14_001613F4 __attribute__((sda));
extern f32 D_L14_001613F8 __attribute__((sda));
extern s32 D_L14_00161400 __attribute__((sda));
extern s32 D_L14_00161404 __attribute__((sda));
extern s32 D_L14_00161408 __attribute__((sda));
extern s32 D_L14_0016140C __attribute__((sda));
extern s32 D_L14_00161410 __attribute__((sda));
extern s32 D_L14_00161414 __attribute__((sda));
extern f32 D_L14_001613A0_c618 __asm__("D_L14_001613A0") __attribute__((sda));
extern f32 D_L14_001613A4_c618 __asm__("D_L14_001613A4") __attribute__((sda));

extern Vec4 D_L14_00174560;
void FUN_L14_002ac618(struct Moby *m) {
    char *d = (char *)m->pvars;
    float v[4];
    float w[4];
    int near_seg;
    float near_t;
    int path_seg;
    float path_t;

    if ((u8)d[0x2E] == 2) {
        *(int *)(d + 0x20C) = 0;
        FUN_0020c828(m);
        return;
    }
    if (m->state == 8) {
        FUN_L14_002aed50_c618(m);
        add_draw_callback(FUN_L14_002aee28, m);
        return;
    }
    *(int *)(d + 0x20C) = 0;
    if (m->anim_flags & 2) {
        if (m->seq == m->prev_seq) {
            if (m->seq == 2) {
                blend_moby_animation_c618(m, 3, 0, 3);
            }
            if (m->seq == 1) {
                blend_moby_animation_c618(m, 2, 0, 10);
            }
        }
    }
    FUN_L14_002ad8b8(m);
    if (m->state > 0 && m->state < 7) {
        if (FUN_001f9b48(&m->pos, D_L14_001674C0) < 40.0f) {
            FUN_L00_0025a120(m);
        }
        *(s16 *)(d + 0x200) = 0;
    }
    switch (m->state) {
    case 0:
        FUN_002141f8_c618(m)[0xE] = 1;
        if (*(s16 *)(d + 0x1B6) == 1) {
            m->flags = (m->flags | 0x41) & 0xEFFF;
            m->collision = 0;
            m->update_dist = 0xFF;
            m->state = 8;
            d[0x58] = truncate_float_to_s32(17.0f);
            d[0x5A] = truncate_float_to_s32(6.0f);
            *(s16 *)(d + 0x20A) = FUN_L14_002af048(m);
            return;
        }
        if (*(int *)(d + 0x74) == -1 || *D_L14_001B0BB0[*(int *)(d + 0x74)] == 0 ||
            *(int *)(d + 0x78) == -1 || *D_L14_001B0BB0[*(int *)(d + 0x78)] == 0 ||
            *(int *)(d + 0x70) < 0 ||
            *(int *)(d + 0x204) == -1 || *D_L14_001B0BB0[*(int *)(d + 0x204)] == 0) {
            FUN_0020c828(m);
            return;
        }
        m->shadow_range = 0x20;
        *(float *)(d + 0x20) = 4.0f;
        *(s16 *)(d + 0x24) = 4;
        d[0x28] = 3;
        qcopy(&m->pos, (char *)D_L14_001B0BB0[*(int *)(d + 0x74)] + 0x10);
        qcopy(d + 0x170, (char *)D_L14_001B0BB0[*(int *)(d + 0x78)] + 0x10);
        FUN_L14_002ae448(m);
        *(s16 *)(d + 0x88) = 1;
        *(float *)(d + 0x21C) = -1.0f;
        *(float *)(d + 0x180) = *(float *)(d + 0x184) = *(float *)(d + 0x188) = 0.0f;
        *(float *)(d + 0x194) = *(float *)(d + 0x198) = *(float *)(d + 0x19C) = 0.0f;
        *(float *)(d + 0x160) = 0.0f;
        *(int *)(d + 0x20C) = 0;
        *(float *)(d + 0x164) = 0.0f;
        *(float *)(d + 0x1A0) = 0.0f;
        *(float *)(d + 0x1A4) = 0.0f;
        *(float *)(d + 0x1A8) = 0.0f;
        *(float *)(d + 0x1AC) = 0.0f;
        *(float *)(d + 0x1B0) = m->rot.z;
        *(s16 *)(d + 0x1B4) = 0;
        *(float *)(d + 0x1B8) = 0.0f;
        memset_c618(d + 0xE0, 0, 0x40);
        memset_c618(d + 0x120, 0, 0x40);
        attach_manipulator_c618(m, 0, d + 0xE0);
        attach_manipulator_c618(m, 1, d + 0x120);
        *(float *)(d + 0x1C8) = *(float *)(d + 0x1C4) = 0.0f;
        *(int *)(d + 0x18C) = 0;
        m->state = 1;
        m->command = 0;
        break;
    case 1: {
        int found;
        int hit;

        m->glow_color = 0xFF000000 | (D_L14_00161408 << 16) | (D_L14_00161404 << 8) | D_L14_00161400;
        if (FUN_L14_002b4670(*(int *)(d + 0x70))) {
            m->state = 3;
            *(s16 *)(d + 0x1F2) = FUN_001f96f8(D_L14_001613D0);
        }
        *(s16 *)(d + 0x202) = 0;
        *(s16 *)(d + 0x200) = 1;
        found = 0;
        hit = FUN_L14_002aeba0(m, 1);
        FUN_L14_002ae6d8(m);
        FUN_L14_002ae7d8(m);
        FUN_L14_002aea08_c618(m);
        if (hit) {
            lerp_vector_c618(v, d + 0x90, d + 0xD0, 0.5f);
            qcopy(w, d + 0x90);
            FUN_001e93e8_c618(w, *(float *)(d + 0x214) * 0.8f);
            FUN_001e93e8_c618(v, *(float *)(d + 0x214) * 0.48f);
            if (FUN_L00_001f2868_c618(*(float *)(d + 0x214) * 0.48f, v, 0, 0, 0) ||
                FUN_L00_001f2868_c618(*(float *)(d + 0x214) * 0.9f, w, 0, 0, 0)) {
                struct Moby *t = D_L14_00174540.target;

                if ((char *)t == D_001413D0_c618 || (t != 0 && (t->oclass == 0xCB || t->oclass == 0x10E))) {
                    found = 1;
                    *(struct Moby **)(d + 0x18C) = D_L14_00174540.target;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    case 2: {
        s32 voice = *(s32 *)(d + 0x1D0);

        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == m && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)(d + 0x1D0) = -1;
        FUN_L14_002b44a8(*(int *)(d + 0x70));
        m->state = 3;
        *(s16 *)(d + 0x1F2) = FUN_001f96f8(D_L14_001613D0);
        break;
    }
    case 3:
        m->glow_color = 0xFF000000 | (D_L14_00161414 << 16) | (D_L14_00161410 << 8) | D_L14_0016140C;
        if (*(struct Moby **)(d + 0x18C) != 0) {
            *(s16 *)(d + 0x200) = 1;
            *(s16 *)(d + 0x202) = 1;
            FUN_L14_002aeba0(m, 1);
            qcopy(v, &(*(struct Moby **)(d + 0x18C))->pos);
            v[2] += 0.5f;
            qcopy(w, v);
            w[2] -= 4.0f;
            if (FUN_001efa68_c618(v, w, 0x12, 0, 0)) {
                qcopy(v, &D_L14_00174560);
            } else {
                v[2] -= 0.5f;
            }
            ease_c618((f32 *)(d + 0x170), v[0], (f32 *)(d + 0x180), 0.003f, 0.2f, 0.0f);
            ease_c618((f32 *)(d + 0x174), v[1], (f32 *)(d + 0x184), 0.003f, 0.2f, 0.0f);
            ease_c618((f32 *)(d + 0x178), v[2], (f32 *)(d + 0x188), 0.003f, 0.2f, 0.0f);
            FUN_L14_002ae7d8(m);
            if ((s8)(*(struct Moby **)(d + 0x18C))->state < 0 || FUN_001f9770((s16 *)(d + 0x1F2))) {
                *(float *)(d + 0x8C) = 0.0f;
                *(int *)(d + 0x80) = 0;
                *(s16 *)(d + 0x208) = 0;
                m->state = 4;
            }
        } else {
            *(int *)(d + 0x80) = 0;
            *(float *)(d + 0x8C) = 0.0f;
            *(s16 *)(d + 0x208) = 0;
            m->state = 4;
        }
    case 4:
        m->glow_color = 0xFF000000 | (D_L14_00161414 << 16) | (D_L14_00161410 << 8) | D_L14_0016140C;
        qcopy(v, &m->pos);
        v[3] = D_L14_0016139C;
        if (FUN_001fa728((u8 *)v, FUN_001fa6c0(m->draw_dist)) >= 0) {
            float *p;

            FUN_L00_0024f7c8_c618(m, 2, w);
            p = (float *)(d + 0x1D4);
            p[0] = w[0];
            p[1] = w[1];
            p[2] = w[2];
            *(int *)(d + 0x20C) |= 8;
            FUN_L14_002aea08_c618(m);
        }
        if (m->state == 4) {
            if (m->prev_seq == 0) {
                blend_moby_animation_c618(m, 2, 0, 5);
            }
            FUN_L14_002ae900(m, D_L14_001613F8, D_L14_001613F4, 1.0f);
            if (*(float *)(d + 0x1C0) != 0.0f || *(float *)(d + 0x1BC) != 0.0f) {
                ease_c618((f32 *)(d + 0x1C0), 0.0f, (f32 *)(d + 0x1C8), 0.004f, 0.2f, 0.0f);
                ease_c618((f32 *)(d + 0x1BC), 0.0f, (f32 *)(d + 0x1C4), 0.004f, 0.2f, 0.0f);
                if (absolute_float(*(float *)(d + 0x1C0)) < 0.0001f) {
                    *(float *)(d + 0x1C0) = 0.0f;
                }
                if (absolute_float(*(float *)(d + 0x1BC)) < 0.0001f) {
                    *(float *)(d + 0x1BC) = 0.0f;
                }
                FUN_L00_001fff28(d + 0xF0, 2, *(float *)(d + 0x1BC));
                FUN_L00_001fff28(d + 0x130, 1, *(float *)(d + 0x1C0));
            }
            if (FUN_L14_002b4500(*(int *)(d + 0x70))) {
                MoverPath *p = (MoverPath *)D_L14_001B0BB0[*(int *)(d + 0x204)];

                m->state = 5;
                *(int *)(d + 0x80) = p->count - 2;
                *(float *)(d + 0x8C) = p->points[*(int *)(d + 0x80)].f[3];
                *(s16 *)(d + 0x208) = 0;
                FUN_L14_002ae900(m, 0.01f, 5.0f, -1.0f);
                blend_moby_animation_c618(m, 0, 0, FUN_001f96f8(10));
            }
        }
        break;
    case 5:
        FUN_L14_002aea08_c618(m);
        FUN_L14_002ae900(m, 0.01f, 5.0f, -1.0f);
        if (*(s16 *)(d + 0x208) != 0) {
            int *tab = D_L14_001B0BB0[*(int *)(d + 0x78)];

            near_seg = 0;
            near_t = 0.0f;
            FUN_L00_0025df68_c618(tab, &m->pos, v, &near_seg, &near_t, 0, 40.0f, 5.0f, 0.0f);
            *(int *)(d + 0x84) = near_seg;
            *(float *)(d + 0x1CC) = near_t;
            *(s16 *)(d + 0x200) = 0;
            m->state = 6;
        }
        break;
    case 6: {
        int i;
        int n;
        float dist;
        char *seg;
        int *tab;
        float t;
        int dir;

        FUN_L14_002aea08_c618(m);
        n = *(int *)(d + 0x84);
        seg = (char *)D_L14_001B0BB0[*(int *)(d + 0x78)];
        dist = 0.0f;
        for (i = 0; i < n; i++) {
            dist += *(float *)(seg + i * 16 + 0x1C);
        }
        dist += *(float *)(d + 0x1CC);
        tab = D_L14_001B0BB0[*(int *)(d + 0x74)];
        t = dist / *(float *)(d + 0x16C) * *(float *)(d + 0x190);
        dir = *(s16 *)(d + 0x8A);
        path_seg = 0;
        path_t = 0.0f;
        FUN_L00_0025d808_c618(tab, v, &path_seg, &path_t, dir, t);
        ease_c618(&m->pos.x, v[0], (f32 *)(d + 0x194), 0.005f, D_L14_001613A4_c618, 0.0f);
        ease_c618(&m->pos.y, v[1], (f32 *)(d + 0x198), 0.005f, D_L14_001613A4_c618, 0.0f);
        ease_c618(&m->pos.z, v[2], (f32 *)(d + 0x19C), 0.005f, D_L14_001613A4_c618, 0.0f);
        FUN_L14_002aeba0(m, 0);
        if (FUN_001f9b48(&m->pos, v) < 0.5f) {
            m->state = 1;
        }
        FUN_L00_0025d808_c618(D_L14_001B0BB0[*(int *)(d + 0x78)], v, d + 0x84, d + 0x1CC, *(s16 *)(d + 0x8A), 0.0f);
        ease_c618((f32 *)(d + 0x170), v[0], (f32 *)(d + 0x180), D_L14_001613A0_c618, D_L14_001613A4_c618, 0.0f);
        ease_c618((f32 *)(d + 0x174), v[1], (f32 *)(d + 0x184), D_L14_001613A0_c618, D_L14_001613A4_c618, 0.0f);
        ease_c618((f32 *)(d + 0x178), v[2], (f32 *)(d + 0x188), D_L14_001613A0_c618, D_L14_001613A4_c618, 0.0f);
        break;
    }
    case 7: {
        s32 voice;
        float ang;

        *(s16 *)(d + 0x1F0) = FUN_001f96f8(D_L14_001613D4);
        FUN_L14_002b44a8(*(int *)(d + 0x70));
        voice = *(s32 *)(d + 0x1D0);
        if (voice != -1) {
            VoicePoolWindow *slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);

            if ((struct Moby *)slot->voice.owner == m && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        *(s32 *)(d + 0x1D0) = -1;
        FUN_L00_0025e450_c618(m, d + 0x40, &m->pos, 0.0f, 0.0f, 5, 2, 4, 4.0f, 2.0f, 9.0f, 1.0f, 1, 15.0f, 1, 5, -1, 0);
        subtract_vector_c618(v, &m->pos, &hero.motion.pos);
        v[2] = 0.0f;
        normalize_vector_c618(v, v, random_float_c618(D_L14_001613D8 * D_0015ED60_c618, D_L14_001613DC * D_0015ED60_c618));
        cross_vectors_c618(w, &hero.gravity_dir, v);
        ang = randf_sym(0.6981317f, 1.2217305f);
        build_look_at_matrix_c618(d + 0x1E0, v, &hero.gravity_dir, ang);
        *(float *)(d + 0x1E8) = random_float_c618(D_L14_001613E0 * D_0015ED60_c618, D_L14_001613E4 * D_0015ED60_c618);
        m->scale *= 0.1f;
        *(float *)(d + 0x140) = 10.0f;
        *(float *)(d + 0x144) = 10.0f;
        *(float *)(d + 0x148) = 10.0f;
        *(float *)(d + 0x1F4) = randf_sym(0.0f, 0.08726646f);
        *(float *)(d + 0x1F8) = randf_sym(0.0f, 0.08726646f);
        *(float *)(d + 0x1FC) = randf_sym(0.0f, 0.08726646f);
        m->rot.x = fast_add_rotations(m->rot.x, randf_sym(0.0f, 0.7853982f));
        m->rot.y = fast_add_rotations(m->rot.y, randf_sym(0.0f, 0.7853982f));
        m->state = 9;
        FUN_L14_002aead8_c618(m);
        return;
    }
    case 9: {
        int i;

        add_vector_c618(&m->pos, &m->pos, d + 0x1E0);
        if (absolute_float(*(float *)(d + 0x1E0)) > D_L14_001613F0 * D_0015ED60_c618) {
            *(float *)(d + 0x1E0) *= (D_L14_001613EC - 1.0f) * D_0015ED60_c618 + 1.0f;
        }
        if (absolute_float(*(float *)(d + 0x1E4)) > D_L14_001613F0 * D_0015ED60_c618) {
            *(float *)(d + 0x1E4) *= (D_L14_001613EC - 1.0f) * D_0015ED60_c618 + 1.0f;
        }
        if (*(float *)(d + 0x1E8) > 0.0f) {
            *(float *)(d + 0x1E8) *= (D_L14_001613EC - 1.0f) * D_0015ED60_c618 + 1.0f;
        }
        *(float *)(d + 0x1E8) -= D_L14_001613E8 * frame_scale_sq;
        m->rot.z = fast_add_rotations(m->rot.z, *(float *)(d + 0x1FC));
        *(float *)(d + 0x140) = 10.0f;
        *(float *)(d + 0x144) = 10.0f;
        *(float *)(d + 0x148) = 10.0f;
        if (m->pos.x < 2.0f || m->pos.x > 1021.0f || m->pos.y < 2.0f || m->pos.y > 1021.0f ||
            m->pos.z < 2.0f || m->pos.z > 1021.0f) {
            FUN_0020c828(m);
            return;
        }
        if (FUN_001f9770((s16 *)(d + 0x1F0)) || coll_sphere(0.5f, &m->pos, 0, m)) {
            FUN_L00_0025e450_c618(m, d + 0x40, &m->pos, 2.0f, 1.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 1, 15.0f, 1, 5, -1, 0);
            FUN_0020c828(m);
            return;
        }
        FUN_L00_0026cbb0(&m->pos, D_L14_0015F580, 0x4F007FFF, 0x1FFFFFFF,
                         random_int_between(FUN_001f96f8(10), FUN_001f96f8(20)), 1, 60000.0f);
        for (i = 0; i < 2; i++) {
            int a = random_integer_below(6);
            float size;
            int c;
            unsigned char *p;

            if (random_integer_below(2)) {
                a = -a;
            }
            size = random_float_c618(40000.0f, 100000.0f);
            c = random_int_between(0x30, 0xFF);
            p = FUN_L00_0026d000_c618(&m->pos, 0.025f, 1.0f, 1.0f, a, D_L14_0015F580, size, c | (0x60000000 | (c << 16) | (c << 8)));
            if (p != 0) {
                unsigned char *q;

                p[3] = 0x44;
                q = p + 0x20;
                *(s16 *)(p + 0xA) = FUN_001f96f8(60);
                *(int *)(q + 4) = 2;
                q[0xA] = 0x60;
                q[0xB] = p[0xA];
            }
        }
        FUN_L14_002aead8_c618(m);
        return;
    }
    }
    if ((s8)m->state >= 0 && m->prev_seq == 1) {
        if (m->command == 1) {
            FUN_L00_0025c398_c618(m, D_L14_0015F580, 0xFFFF);
        }
    } else {
        m->command = 0;
    }
    if (m->state == 1 && !FUN_L00_0028d8c0(m, *(s32 *)(d + 0x1D0))) {
        *(s32 *)(d + 0x1D0) = allocate_voice_c618(0, 4, m);
    }
    if ((m->state == 3 || m->state == 4) && !FUN_L00_0028d8c0(m, *(s32 *)(d + 0x1D0))) {
        *(s32 *)(d + 0x1D0) = allocate_voice_c618(2, 4, m);
    }
}
/* per-frame update: runs the hit query, decays a timer, then dispatches on the hit result */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AEAF8), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025d458(void *m, short *p);
extern void FUN_L00_0025d538(void *, void *);
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void FUN_L14_002ad8b8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int st;
    float t;
    int v;
    t = 0.0f;
    v = hit_reaction(moby, find_moby_hit(moby, 0x330000, 0), data + 0x20, 0, &st, &t, 0, 4);
    if (st != 1 && moby->state < 7) {
        float f = *(float *)(data + 0x20) - t;
        *(float *)(data + 0x20) = f;
        if (f <= 0.0f)
            v = 1;
        if (moby->state == 1)
            moby->state = 2;
        if (st == 3)
            moby->command = 1;
        switch (v) {
        case 9:
        case 10:
            data[0x67] = 0x64;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            ((unsigned char *)data)[0x67] = 0xC8;
            blend_moby_animation(moby, 1, 0, 5);
            break;
        case 1:
        case 2:
            moby->state = 7;
            moby->flags &= 0xEFFF;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case 0:
        case 11:
            break;
        }
        FUN_L00_0025d458(moby, (short *)(data + 0x60));
    }
    moby->hit_slot = 0xFF;
    FUN_L00_0025d538(moby, data + 0x60);
}
/* Draws the moby's ring of lights: builds a matrix facing the camera from the moby's axis, transforms the ring
 * vertices through it and draws them, then flips the matrix and draws the back face. */
extern s32 D_L14_00161418 __attribute__((sda));
extern float D_L14_001D7A60[][3];
s32 D_L14_001D80C8[1] __attribute__((section(".data"))) = {0x507f7f};
char D_L14_001D7E38[656] = {0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, -51, -52, -52, 0x3d, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -51, -52, -52, 0x3d, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -51, -52, -52, 0x3d, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -51, -52, -52, 0x3d, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3e, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -102, -103, -103, 0x3e, 0, 0, 0, 0x3f, -51, -52, -52, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, -52, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, -52, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, -52, 0x3e, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0x3f, -102, -103, 0x19, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -102, -103, 0x19, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -102, -103, 0x19, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -102, -103, 0x19, 0x3f, 0, 0, 0, 0x3f, 0x33, 0x33, 0x33, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, 0x33, 0x33, 0x33, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, 0x33, 0x33, 0x33, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, 0x33, 0x33, 0x33, 0x3f, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0x66, 0x66, 0x66, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0x66, 0x66, 0x66, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0x66, 0x66, 0x66, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0x66, 0x66, 0x66, 0x3f, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0x66, 0x66, 0x66, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0x3f, -51, -52, 0x4c, 0x3f, 0, 0, 0, 0x3f, 0, 0, -128, 0x3f};
extern char D_L14_001674C0_r[] __asm__("D_L14_001674C0");
extern void FUN_L14_002ae0a8(struct Moby *m);
extern void FUN_L00_001fde98(int, void *, void *, void *, int);
extern void cross_r(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void normalize_r(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_r(void *, void *, float) __asm__("FUN_001f9a68");
extern void sub_r(void *, void *, void *) __asm__("FUN_001f9a28");
extern void xform_r(void *, void *, void *) __asm__("FUN_001f9d20");
extern s64 tex_r(s32) __asm__("FUN_001f44b8");
extern void sreg_r(s32, s64) __asm__("FUN_00233980");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void FUN_L14_002ada18(char *moby) {
    char *d = *(char **)(moby + 0x78);
    struct {
        float mat[4][4];    /* camera-facing frame */
        float verts[82][3]; /* ring vertices transformed by mat */
    } f;
    u32 cols[84];
    float v[4];
    int i;
    int j;

    FUN_L14_002ae0a8((struct Moby *)moby);
    qcopy(f.mat[0], d + 0xA0);
    sub_r(f.mat[2], d + 0xD0, D_L14_001674C0_r);
    cross_r(f.mat[1], f.mat[0], f.mat[2]);
    normalize_r(f.mat[1], f.mat[1], 1.0f);
    cross_r(f.mat[2], f.mat[1], f.mat[0]);
    qcopy(f.mat[3], d + 0xD0);
    f.mat[3][3] = 1.0f;
    sreg_r(6, tex_r(0x16));
    sreg_r(0x42, 0x8000000048);
    sreg_r(8, 5);
    sreg_r(0x14, 0xFF9000000260);
    sreg_r(0x47, 0x50000);
    sreg_r(0x4A, 0);
    font_queue_vu_state();
    for (i = 0; i < D_L14_00161418; i++) {
        float *s = D_L14_001D7A60[i];
        float *o = f.verts[i];
        v[0] = s[0];
        v[1] = s[1];
        v[2] = s[2];
        v[3] = 1.0f;
        xform_r(v, v, f.mat);
        o[0] = v[0];
        o[1] = v[1];
        o[2] = v[2];
        if (*(short *)(d + 0x202) != 0) {
            cols[i] = D_L14_001D80C8[i] & 0xFF0000FF;
        }
    }
    if (*(short *)(d + 0x202) != 0) {
        FUN_L00_001fde98(D_L14_00161418, f.verts, cols, D_L14_001D7E38, 1);
    } else {
        FUN_L00_001fde98(D_L14_00161418, f.verts, D_L14_001D80C8, D_L14_001D7E38, 1);
    }
    scale_r(f.mat[1], f.mat[1], -1.0f);
    scale_r(f.mat[2], f.mat[2], -1.0f);
    for (j = 0; j < D_L14_00161418; j++) {
        float *o = f.verts[j];
        float *s = D_L14_001D7A60[j];
        v[0] = s[0];
        v[1] = s[1];
        v[2] = s[2];
        v[3] = 1.0f;
        xform_r(v, v, f.mat);
        o[0] = v[0];
        o[1] = v[1];
        o[2] = v[2];
    }
    if (*(short *)(d + 0x202) != 0) {
        FUN_L00_001fde98(D_L14_00161418, f.verts, cols, D_L14_001D7E38, 1);
    } else {
        FUN_L00_001fde98(D_L14_00161418, f.verts, D_L14_001D80C8, D_L14_001D7E38, 1);
    }
}
/* Draws the moby's spinning beam: sixteen camera-facing quads stepped out along its heading, each pair
 * fainter and larger than the last. */
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_2add48;

extern float D_L14_00161434 __attribute__((sda));
extern float D_L14_00161438 __attribute__((sda));
extern float D_L14_0016143C __attribute__((sda));
extern float D_L14_00161440 __attribute__((sda));
extern int D_L14_00161444 __attribute__((sda));
extern int D_L14_00161448 __attribute__((sda));
extern int D_L14_0016144C __attribute__((sda));
extern int D_L14_00161450 __attribute__((sda));
extern int D_L14_00161454 __attribute__((sda));
extern int D_L14_00161458 __attribute__((sda));
extern int D_L14_0016145C __attribute__((sda));
extern int D_L14_00161460 __attribute__((sda));
extern char D_L14_001674C0_b[] __asm__("D_L14_001674C0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void scale_b(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_b(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_b(void *, void *, void *) __asm__("FUN_001f9a28");
extern unsigned long tex_b(s32) __asm__("FUN_001f44b8");
extern void sreg_b(s32, s64) __asm__("FUN_00233980");
extern float atan2_b(float, float) __asm__("FUN_001f9e90");
extern float lenxy_b(void *) __asm__("FUN_001f9b20");
extern void rotmat_b(void *, void *) __asm__("FUN_001fa030");
extern void xform_b(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_b(void *, int, int) __asm__("FUN_001f7d30");

float D_L14_001D8600[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};
void FUN_L14_002add48(char *moby) {
    char *d = *(char **)(moby + 0x78);
    float src[4][4];
    Pk_2add48 pk;
    float mat[4][4];
    float t[4];
    float dir[4];
    float v[4];
    float rot[4];
    float rm[3][4];
    float size;
    float step;
    int r, g, b, a;
    u32 col;
    int i, j;
    float *p;

    p = (float *)(d + 0x1D4);
    src[3][0] = p[0];
    src[3][1] = p[1];
    src[3][2] = p[2] + 0.125f;
    src[3][3] = 1.0f;
    src[0][0] = fast_cos(*(float *)(d + 0x160));
    src[0][1] = fast_sin(*(float *)(d + 0x160));
    src[0][2] = 0.0f;
    *(float *)(d + 0x160) = fast_add_rotations(*(float *)(d + 0x160), -0.06981317f);
    pk.b = tex_b(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    qcopy(dir, src[0]);
    qcopy(mat[3], src[3]);
    mat[3][3] = 1.0f;
    sub_b(v, D_L14_001674C0_b, mat[3]);
    rot[2] = atan2_b(v[0], v[1]);
    rot[1] = -atan2_b(lenxy_b(v), v[2]);
    rot[0] = 0.0f;
    rotmat_b(rm, rot);
    qcopy(mat[0], rm[0]);
    qcopy(mat[1], rm[1]);
    qcopy(mat[2], rm[2]);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    size = D_L14_0016143C;
    step = D_L14_00161434;
    r = D_L14_00161444;
    g = D_L14_00161448;
    b = D_L14_0016144C;
    a = D_L14_00161450;
    sreg_b(0x47, 0x51001);
    for (i = 0; i < 16; i++) {
        col = (a << 24) | (b << 16) | (g << 8) | r;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
        scale_b(t, dir, step);
        for (j = 0; j < 4; j++) {
            scale_b(pk.m[j], D_L14_001D8600[j], size);
            xform_b(pk.m[j], pk.m[j], mat);
            add_b(pk.m[j], pk.m[j], t);
        }
        draw_b(&pk, 0, 0);
        scale_b(t, dir, step * -2.0f);
        for (j = 0; j < 4; j++) {
            add_b(pk.m[j], pk.m[j], t);
        }
        draw_b(&pk, 0, 0);
        r -= D_L14_00161454;
        g -= D_L14_00161458;
        b -= D_L14_0016145C;
        a -= D_L14_00161460;
        size += D_L14_00161440;
        step += D_L14_00161438;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
        if (a < 0)
            a = 0;
    }
}
/* Draws the moby's glow quad, turned to face the camera. */
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_2ae0a8;

extern char D_L14_001674C0_e[] __asm__("D_L14_001674C0");
extern void scale_e(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_e(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_e(void *, void *, void *) __asm__("FUN_001f9a28");
extern unsigned long tex_e(s32) __asm__("FUN_001f44b8");
extern void sreg_e(s32, s64) __asm__("FUN_00233980");
extern float atan2_e(float, float) __asm__("FUN_001f9e90");
extern float lenxy_e(void *) __asm__("FUN_001f9b20");
extern void rotmat_e(void *, void *) __asm__("FUN_001fa030");
extern void xform_e(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_e(void *, int, int) __asm__("FUN_001f7d30");

extern float D_L14_00161474 __attribute__((sda));
extern float D_L14_00161478 __attribute__((sda));
void FUN_L14_002ae0a8(struct Moby *m) {
    char *d = (char *)m->pvars;
    float mat[4][4];
    float t[4];
    Pk_2ae0a8 pk;
    float v[4];
    float rot[4];
    u32 col;
    int i;
    qcopy(mat[3], d + 0xD0);
    mat[3][3] = 1.0f;
    scale_e(t, d + 0xA0, D_L14_00161478);
    add_e(mat[3], mat[3], t);
    pk.b = tex_e(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    sreg_e(0x47, 0x51001);
    sub_e(v, D_L14_001674C0_e, mat[3]);
    rot[2] = atan2_e(v[0], v[1]);
    rot[1] = -atan2_e(lenxy_e(v), v[2]);
    rot[0] = 0.0f;
    rotmat_e(mat, rot);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    if (*(short *)(d + 0x202) != 0) {
        col = 0x7F40407F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    } else {
        col = 0x7F7F7F7F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    }
    for (i = 0; i < 4; i++) {
        scale_e(pk.m[i], D_L14_001D8600[i], D_L14_00161474);
        xform_e(pk.m[i], pk.m[i], mat);
    }
    draw_e(&pk, 0, 0);
}
/* Draws the moby's glow quad, pulled toward the camera so it is not hidden by the moby. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF4A0), where it is exact; names translated to the US level program. */

typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    u64 a, b, c, d;
} Pk_2af4a0;

extern char D_L14_001674C0_d[] __asm__("D_L14_001674C0") __attribute__((section(".data")));
float D_L14_001D8640[4][4] = {
    {-1.0f, 1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 0.0f, 1.0f},
};
extern float vector_length_xyz(void *a);
extern void add_vector_xyz(void *, void *, void *);
extern void draw_geometry_quad(void *, void *, int);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
u64 get_effect_texture_alt(s32) __asm__("FUN_001f44b8");

extern short D_L14_0016147C __attribute__((sda));
extern short D_L14_00161480 __attribute__((sda));
void FUN_L14_002ae260(struct Moby *m) {
    char *d = (char *)m->pvars;
    float pos[4];
    Pk_2af4a0 pk;
    float v[4];
    float w[4];
    float len;
    u32 col;
    int j;
    qcopy(pos, d + 0x90);
    pos[2] += 0.01f;
    pos[3] = 1.0f;
    pk.b = get_effect_texture_alt(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    subtract_vector_xyz(v, D_L14_001674C0_d, pos);
    len = vector_length_xyz(v);
    if (0.0f < len) {
        float s = *(float *)(d + 0x218);
        float f = len - *(float *)&D_L14_0016147C;
        if (f < s) {
            s = f;
            if (f < 0.0f)
                s = 0.0f;
        }
        normalize_vector_xyz(w, v, s);
        add_vector_xyz(pos, pos, w);
    }
    pk.uv[2] = 0.0f;
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    if (*(short *)(d + 0x202) != 0) {
        col = 0x7F40407F;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    } else {
        col = *(u32 *)&D_L14_00161480;
        pk.col[3] = col;
        pk.col[2] = col;
        pk.col[1] = col;
        pk.col[0] = col;
    }
    for (j = 0; j < 4; j++) {
        scale_vector_xyz(pk.m[j], D_L14_001D8640[j], *(float *)(d + 0x214));
        add_vector_xyz(pk.m[j], pk.m[j], pos);
    }
    draw_geometry_quad(&pk, 0, 0);
}
/* Measures the segments of the moby's three paths, sums two of them and places it on the first two. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF688), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);
extern int *D_L14_001B0BB0[];
extern void FUN_L00_0025d808(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");
extern int FUN_L00_0025d808_2af688(void *, void *, void *, void *, int,
                                    float) __asm__("FUN_L00_0025d808");

void FUN_L14_002ae448(struct Moby *m) {
    char *d = (char *)m->pvars;
    int *p;
    int i;
    float len;
    p = D_L14_001B0BB0[*(int *)(d + 0x78)];
    *(int *)(d + 0x16C) = 0;
    for (i = 0; i < p[0]; i++) {
        len = FUN_001f9b48((char *)p + (i * 16 + 0x10), (char *)p + (((i + 1) % p[0]) * 16 + 0x10));
        *(float *)((char *)p + (i << 4) + 0x1C) = len;
        if (i != p[0] - 1) {
            *(float *)(d + 0x16C) += len;
        }
    }
    p = D_L14_001B0BB0[*(int *)(d + 0x74)];
    *(int *)(d + 0x190) = 0;
    for (i = 0; i < p[0]; i++) {
        len = FUN_001f9b48((char *)p + (i * 16 + 0x10), (char *)p + (((i + 1) % p[0]) * 16 + 0x10));
        *(float *)((char *)p + (i << 4) + 0x1C) = len;
        if (i != p[0] - 1) {
            *(float *)(d + 0x190) += len;
        }
    }
    p = D_L14_001B0BB0[*(int *)(d + 0x204)];
    for (i = 0; i < p[0]; i++) {
        *(float *)((char *)p + (i << 4) + 0x1C) =
            FUN_001f9b48((char *)p + (i * 16 + 0x10), (char *)p + (((i + 1) % p[0]) * 16 + 0x10));
    }
    if (*(float *)(d + 0x210) != 0.0f) {
        int *q = D_L14_001B0BB0[*(int *)(d + 0x78)];
        *(int *)(d + 0x84) = 0;
        *(float *)(d + 0x1CC) = 0.0f;
        FUN_L00_0025d808_2af688(q, &m->pos, d + 0x84, d + 0x1CC, *(short *)(d + 0x8A),
                                 *(float *)(d + 0x210) * *(float *)(d + 0x16C));
        *(float *)(d + 0x8C) = 0.0f;
        *(int *)(d + 0x80) = 0;
        FUN_L00_0025d808_2af688(q, &m->pos, d + 0x80, d + 0x8C, *(short *)(d + 0x8A),
                                 *(float *)(d + 0x210) * *(float *)(d + 0x190));
    } else {
        *(int *)(d + 0x80) = 0;
        *(int *)(d + 0x84) = 0;
        *(float *)(d + 0x8C) = 0.0f;
        *(float *)(d + 0x1CC) = 0.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF918), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2,
                                float eps) __asm__("FUN_L00_0025b8c0");
extern int *D_L14_001B0BB0[];
extern f32 D_L14_001613A0_d __asm__("D_L14_001613A0") __attribute__((sda));
extern f32 D_L14_001613A4_d __asm__("D_L14_001613A4") __attribute__((sda));
extern void FUN_L00_0025d808(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");
extern int FUN_L00_0025d808_2af918(int *tab, float *out, int *a, float *b, int c,
                                    float d) __asm__("FUN_L00_0025d808");

void FUN_L14_002ae6d8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float v[4];
    float save = *(float *)(data + 0x1CC);
    int save2 = *(int *)(data + 0x84);
    int r = FUN_L00_0025d808_2af918(D_L14_001B0BB0[*(int *)(data + 0x78)], v, (int *)(data + 0x84),
                                     (float *)(data + 0x1CC), *(short *)(data + 0x8A),
                                     *(float *)(data + 0x168));
    FUN_L00_0025b8c0_c((float *)(data + 0x170), (float *)(data + 0x180), v[0],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c((float *)(data + 0x174), (float *)(data + 0x184), v[1],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c((float *)(data + 0x178), (float *)(data + 0x188), v[2],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    if (*(short *)(data + 0x8A) == 0) {
        if (r != 0) {
            *(float *)(data + 0x1CC) = save;
            *(int *)(data + 0x84) = save2;
            *(float *)(data + 0x168) = -*(float *)(data + 0x168);
        }
    }
}
/* Moves the moby to the point on its second path that matches how far along its first path it is. */
void FUN_L14_002ae7d8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *seg = (char *)D_L14_001B0BB0[*(int *)(data + 0x78)];
    int n = *(int *)(data + 0x84);
    float dist = 0.0f;
    float out[4];
    int idx;
    float frac;
    int i;
    int *tab;
    int dir;
    float t;

    for (i = 0; i < n; i++) {
        dist += *(float *)(seg + i * 16 + 0x1C);
    }
    dist += *(float *)(data + 0x1CC);
    tab = D_L14_001B0BB0[*(int *)(data + 0x74)];
    t = dist / *(float *)(data + 0x16C) * *(float *)(data + 0x190);
    dir = *(short *)(data + 0x8A);
    idx = 0;
    frac = 0.0f;
    FUN_L00_0025d808_2af918(tab, out, &idx, &frac, dir, t);
    FUN_L00_0025b8c0_c(&moby->pos.x, (float *)(data + 0x194), out[0],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.y, (float *)(data + 0x198), out[1],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.z, (float *)(data + 0x19C), out[2],
                       *(float *)&D_L14_001613A0_d, *(float *)&D_L14_001613A4_d, 0.0f);
}
/* Picks a target point from a list or searches for one, then eases the moby's position toward it per axis. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AFB40), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2,
                                float eps) __asm__("FUN_L00_0025b8c0");
extern int FUN_L00_0025d808_2afb40(void *, void *, void *, void *, int,
                                    float) __asm__("FUN_L00_0025d808");

extern short D_L14_001613FC __attribute__((sda));
void FUN_L14_002ae900(struct Moby *moby, float a, float b, float c) {
    char *data = (char *)moby->pvars;
    int *list = D_L14_001B0BB0[*(int *)(data + 0x204)];
    float tmp[4];
    if (*(short *)(data + 0x208) != 0) {
        qcopy(tmp, (char *)list + list[0] * 16);
    } else {
        if (FUN_L00_0025d808_2afb40(list, tmp, data + 0x80, data + 0x8C, 0, b * frame_time * c) !=
            0) {
            *(short *)(data + 0x208) = 1;
        }
    }
    FUN_L00_0025b8c0_c(&moby->pos.x, (float *)(data + 0x194), tmp[0], a,
                       *(float *)&D_L14_001613FC, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.y, (float *)(data + 0x198), tmp[1], a,
                       *(float *)&D_L14_001613FC, 0.0f);
    FUN_L00_0025b8c0_c(&moby->pos.z, (float *)(data + 0x19C), tmp[2], a,
                       *(float *)&D_L14_001613FC, 0.0f);
}
extern f32 D_L14_001613A8 __attribute__((sda));
extern f32 D_L14_001613AC __attribute__((sda));
extern f32 D_L14_001613B0 __attribute__((sda));
extern f32 D_L14_001613B4 __attribute__((sda));
extern f32 D_L14_001613B8 __attribute__((sda));
extern s32 D_L14_001613BC __attribute__((sda));
extern f32 D_L14_001613C0 __attribute__((sda));
extern f32 D_L14_001613C4 __attribute__((sda));
extern f32 D_L14_001613C8 __attribute__((sda));
extern f32 D_L14_001613CC __attribute__((sda));
extern void FUN_L00_00262b00(f32, f32, u8 *, f32 *, f32 *);
extern void FUN_L00_00262b80(u8 *, u8 *, u8 *, f32, f32, f32);
extern s32 FUN_001f9770(s16 *);
extern s32 FUN_001f96f8(s32);
extern f32 fast_add_rotations(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_L00_0025b8c0(f32 *, f32 *, f32, f32, f32, f32);

/* 0x002AEA08, 208 bytes.  Per-frame update of a moby's aim and turn state from
 * level tuning constants.  The ten constants are small data ($gp-relative),
 * declared with __attribute__((sda)).
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002AFC48), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002aea08(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    f32 t;
    FUN_L00_00262b00(D_L14_001613A8, D_L14_001613AC, moby, (f32 *)(data + 0x1A0),
                     (f32 *)(data + 0x1A4));
    FUN_L00_00262b80(moby, data + 0x1A8, data + 0x1AC, D_L14_001613B0, D_L14_001613B4,
                     D_L14_001613B8);
    if (*(u8 *)(moby + 0x20) == 1) {
        if (FUN_001f9770((s16 *)(data + 0x1B4))) {
            *(s16 *)(data + 0x1B4) = FUN_001f96f8(D_L14_001613BC);
            t = D_L14_001613C0;
            if (random_integer_below(2))
                t = -t;
            *(f32 *)(data + 0x1B0) = fast_add_rotations(*(f32 *)(moby + 0x48), t);
        }
        FUN_L00_0025b8c0((f32 *)(moby + 0x48), (f32 *)(data + 0x1B8), *(f32 *)(data + 0x1B0),
                         D_L14_001613C4, D_L14_001613C8, D_L14_001613CC);
    }
}
extern s32 random_integer_below(s32 n) __asm__("FUN_00213260");
extern f32 FUN_001fa6c0(s16);
extern s32 FUN_001fa728(u8 *, f32);
extern void build_moby_bone_transform(u8 *, s32, u8 *) __asm__("FUN_0020cca8");
extern void fast_vec_normalize(u8 *, u8 *, f32) __asm__("FUN_001f9bf8");

/* 0x002AEAD8, 200 bytes.  Checks a flag and a position test, then sets up
 * three vectors in the moby data.  The 16-byte copy is the qcopy idiom
 * (include/qcopy.h).
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002AFD18), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002aead8(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    u8 *p;
    f32 v[4];
    if (random_integer_below(5)) {
        qcopy(v, moby + 0x10);
        v[3] = D_L14_0016139C;
        if (FUN_001fa728((u8 *)v, FUN_001fa6c0(*(s16 *)(moby + 0x32))) >= 0) {
            p = data + 0xA0;
            build_moby_bone_transform(moby, 1, p);
            fast_vec_normalize(p, p, 1.0f);
            fast_vec_normalize(data + 0xB0, data + 0xB0, 1.0f);
            fast_vec_normalize(data + 0xC0, data + 0xC0, 1.0f);
            *(s32 *)(data + 0x20C) = 2;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AFDE0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int FUN_001fa728_c(char *, float) __asm__("FUN_001fa728");
extern void FUN_0020cca8_c(char *, int, void *) __asm__("FUN_0020cca8");
extern void FUN_L00_001fff28(void *, int, float);
extern void FUN_L14_002ae260_u(void) __asm__("FUN_L14_002ae260");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");

int FUN_L14_002aeba0(struct Moby *moby, int flag) {
    int ok = 0;
    char *data = (char *)moby->pvars;
    float w[4];
    float v[4];
    float t;
    subtract_vector_xyz(v, data + 0x170, data + 0xD0);
    scale_vector_xyz(v, v, 0.5f);
    add_vector_xyz(w, data + 0xD0, v);
    w[3] = vector_length_xyz(v) + 2.0f;
    if (FUN_001fa728_c((char *)w, convert_integer_to_float(moby->draw_dist)) >= 0) {
        FUN_0020cca8_c(moby, 1, data + 0xA0);
        ok = 1;
        normalize_vector_xyz(data + 0xA0, data + 0xA0, 1.0f);
        normalize_vector_xyz(data + 0xB0, data + 0xB0, 1.0f);
        normalize_vector_xyz(data + 0xC0, data + 0xC0, 1.0f);
        t = fast_subtract_rotations(fast_arc_tan(v[0], v[1]), moby->rot.z);
        FUN_L00_001fff28(data + 0xF0, 2, t);
        *(float *)(data + 0x1BC) = t;
        t = -fast_arc_tan(vector_length_xy(v), v[2]);
        FUN_L00_001fff28(data + 0x130, 1, t);
        *(float *)(data + 0x1C0) = t;
        qcopy(data + 0x90, data + 0x170);
        if (flag != 0) {
            enqueue_callback_list_1(FUN_L14_002ae260_u, moby);
            *(int *)(data + 0x20C) |= 2;
        }
    }
    return ok;
}
extern s32 truncate_float_to_s32(f32 f) __asm__("FUN_001fa6d0");
s32 D_L14_001D8210[1] __attribute__((section(".data"))) = {0};

/* 0x002AED50, 212 bytes.  Advances the blink phase and fades each light
 * entry's alpha up and down.  Its counter and its 1.0f constant are small
 * data ($gp-relative), declared with __attribute__((sda)).
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002AFF90), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002aed50(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    s32 step;
    s32 i;
    f32 f = *(f32 *)(data + 0x164) + 0.5f;
    *(f32 *)(data + 0x164) = f;
    step = truncate_float_to_s32(f);
    if (1.0f <= *(f32 *)(data + 0x164))
        *(s32 *)(data + 0x164) = 0;
    for (i = 0; i < D_L14_00161418; i++) {
        s32 v = D_L14_001D8210[i];
        if (v != 0) {
            v += step;
            if (v > 0x90)
                v -= 0x80;
            D_L14_001D8210[i] = v;
            if (v > 0x50)
                v = 0xA0 - v;
            D_L14_001D80C8[i] = (D_L14_001D80C8[i] & 0xFFFFFF) | (v << 24);
        }
    }
}
/* Resets the id table, then fills it with the ids of this moby's parts and starts each part by its flags. */
extern int D_L14_001D8680[];
/* Same table under a second name, so its address is loaded again. */
extern int D_L14_001D8680_b[] __asm__("D_L14_001D8680");
char *FUN_L14_002aef28(int index, int *ids);
extern void FUN_L14_002add48(char *);
extern void FUN_L14_002ada18(char *);
void FUN_L14_002aee28(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    int i;
    int *out;
    {
        int v = -1;
        for (i = 19; i >= 0; i--)
            D_L14_001D8680[i] = v;
    }
    i = 0;
    if (*(short *)(d + 0x20A) > 0) {
        int *ids = D_L14_001D8680_b;
        out = ids;
        do {
            char *m = FUN_L14_002aef28(moby->group, ids);
            char *md;
            *out++ = *(short *)(m + 0xB2);
            md = *(char **)(m + 0x78);
            if (*(int *)(md + 0x20C) & 8)
                FUN_L14_002add48(m);
            if (*(int *)(md + 0x20C) & 2)
                FUN_L14_002ada18(m);
            i++;
        } while (i < *(short *)(d + 0x20A));
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Returns the farthest moby in a list that is not in the excluded-id table, by distance from a point. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B0168), where it is exact; names translated to the US level program. */

extern char D_L14_001674C0[];
extern float FUN_001f9b80(void *, void *);
extern unsigned char *D_L14_0015FFD0_e[] __asm__("D_L14_0015FFD0")
    __attribute__((section(".sdata")));
extern int D_L14_001D8680[];
extern short *D_L14_001AC2C0_2B0168[] __asm__("D_L14_001ABF40");

char *FUN_L14_002aef28(int index, int *ids) {
    short *p = D_L14_001AC2C0_2B0168[index];
    char *best = 0;
    float bestd = 0.0f;
    if (p == 0)
        return 0;
    do {
        char *moby = (char *)D_L14_0015FFD0_e[2] + ((*(unsigned short *)p & 0x7FFF) << 8);
        int found = 0;
        int i;
        for (i = 0; i < 20; i++) {
            if (*(short *)(moby + 0xB2) == D_L14_001D8680[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            float d = FUN_001f9b80(D_L14_001674C0, moby + 0x10);
            if (bestd < d) {
                bestd = d;
                best = moby;
            }
        }
    } while (*p++ >= 0);
    return best;
}
#include "sda.h"

/* counts table entries through the first negative terminator */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B0288), where it is exact; names translated to the US level program. */


int FUN_L14_002af048(struct Moby *moby) {
    short *table = D_L14_001AC2C0_2B0168[moby->group];
    int count = 0;
    if (table == 0)
        return 0;
    do {
        count++;
    } while (*table++ >= 0);
    return count;
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B4E40). */

typedef struct Moby Moby;

char *FUN_L14_002eccf0(void *a, float *b, int c, int d);
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern int D_L14_0015F5CC __attribute__((section(".sdata"))); /* no foreign declaration */
extern int D_L14_001600EC; /* no foreign declaration */
extern float D_L14_00161508 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016150C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161510 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161514 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016151C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161520 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161524 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161528 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016152C __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161530 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161534 __attribute__((sda)); /* no foreign declaration */
extern int debug_print_alt() __asm__("FUN_001e93b0");
extern int FUN_001f9770_c2(void *) __asm__("FUN_001f9770");
extern int FUN_L00_002502f0();
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 random_integer_below_c2(s32) __asm__("func_00213260");
extern void FUN_L00_0025e450_c2(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c3(void *, void *, float) __asm__("FUN_001f9a68");
extern void set_moby_animation_c2(void *, s32, s32) __asm__("func_00212ED8");
int FUN_L14_00268038(char *a, char *o, float best);
void FUN_L14_002b4340_c(struct Moby *m) __asm__("FUN_L14_002b4340");
extern int func_001E9730_2B4E40() __asm__("FUN_001e93b0");
char D_L14_001FC0C0_2B4E40[40] __asm__("D_L14_001FBD50") = {0x54, 0x75, 0x72, 0x72, 0x65, 0x74, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x6e, 0x6f, 0x20, 0x61, 0x69, 0x6d, 0x69, 0x6e, 0x67, 0x20, 0x63, 0x75, 0x62, 0x6f, 0x69, 0x64, 0x2e, 0x20, 0x55, 0x49, 0x44, 0x3a, 0x20, 0x25, 0x64, 0xa, 0, 0};
char D_L14_001FC0E8_2B4E40[40] __asm__("D_L14_001FBD78") = {0x54, 0x75, 0x72, 0x72, 0x65, 0x74, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x6e, 0x6f, 0x20, 0x77, 0x61, 0x6b, 0x69, 0x6e, 0x67, 0x20, 0x63, 0x75, 0x62, 0x6f, 0x69, 0x64, 0x2e, 0x20, 0x55, 0x49, 0x44, 0x3a, 0x20, 0x25, 0x64, 0xa, 0, 0};
extern void func_L14_002B5590_2B4E40(unsigned char *m) __asm__("FUN_L14_002b4340");
extern void func_0020D678_2B4E40(void *) __asm__("FUN_0020c828");
extern int func_001F9908_2B4E40(void *) __asm__("FUN_001f9740");
extern void func_00213D28_2B4E40(void *, int, int) __asm__("FUN_00212ed8");
extern int func_00215570_2B4E40(void *, int) __asm__("FUN_00214720");
extern float func_001F9FA8_2B4E40(float) __asm__("FUN_001f9de0");
extern int func_L00_00260D30_2B4E40(void *, void *, float) __asm__("FUN_L14_00268038");
extern char *func_L14_002EE150_2B4E40(void *, float *, int, int) __asm__("FUN_L14_002eccf0");
extern void func_L00_00251328_2B4E40(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void func_L00_0025F4A8_2B4E40(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int func_001F9850_2B4E40(int) __asm__("FUN_001f96f8");
extern float func_001FA888_2B4E40(int) __asm__("FUN_001fa6c0");
extern int func_001F9938_2B4E40(void *) __asm__("FUN_001f9770");
extern float func_001FA748_2B4E40(float, float) __asm__("FUN_001fa580");
extern void func_001F9C30_2B4E40(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9BD8_2B4E40(void *, void *, void *) __asm__("FUN_001f9a10");
extern float func_L00_001FF860_2B4E40(float, float) __asm__("FUN_001f9e90");
extern int func_002140B0_2B4E40(int) __asm__("FUN_00213260");
extern void func_L00_001FF4B0_2B4E40(void *, void *, float) __asm__("FUN_001f9bf8");

void FUN_L14_002b3bf0(char *moby) {
    char *data;
    float v20[4];
    float v30[4];
    float v40[4];
    float v50[4];
    float v60[4];
    float v70[4];
    float m80[16];
    float g0;
    float t1, t2, t3, t4;
    int r, rem, sel, c17, m52, d80;
    char *p16;
    char *p6;

    data = *(char **)(moby + 0x78);
    func_L14_002B5590_2B4E40((unsigned char *)moby);
    switch (*(unsigned char *)(moby + 0x20)) {
    case 0:
        *(float *)(data + 0x70) = *(float *)(moby + 0x18);
        *(float *)(data + 0x84) = *(float *)(moby + 0x48);
        *(int *)(data + 0x8C) = 0;
        *(short *)(data + 0x74) = 0;
        if (*(int *)(data + 0x80) == -1) {
            func_001E9730_2B4E40(D_L14_001FC0C0_2B4E40, *(short *)(moby + 0xB2));
            func_0020D678_2B4E40(moby);
            return;
        }
        if (*(int *)(data + 0x90) == -1) {
            func_001E9730_2B4E40(D_L14_001FC0E8_2B4E40, *(short *)(moby + 0xB2));
            func_0020D678_2B4E40(moby);
            return;
        }
        r = func_001F9850_2B4E40(D_L14_00161534);
        *(short *)(data + 0x76) = r;
        *(unsigned char *)(data + 0x8B) = 0;
        if (*(short *)(data + 0x88) != 0) {
            *(unsigned char *)(moby + 0x20) = 3;
            *(unsigned char *)(data + 0x8A) = 1;
        } else {
            *(unsigned char *)(moby + 0x20) = 1;
            *(float *)(moby + 0x18) = *(float *)(moby + 0x18) - D_L14_00161508;
            *(int *)(moby + 0x94) = 0;
            *(unsigned char *)(data + 0x8A) = 0;
            *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) & 0xEFFF;
        }
        *(int *)(data + 0x98) = func_001F9850_2B4E40(*(int *)(data + 0x94));
        return;
    case 1:
        if (*(short *)(data + 0x88) == 0) {
            return;
        }
        if (func_001F9908_2B4E40(data + 0x98) == 0) {
            return;
        }
        *(unsigned char *)(moby + 0x20) = 2;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(D_L14_0016150C);
        *(float *)(data + 0x78) = 1.0f / func_001FA888_2B4E40(*(short *)(data + 0x74));
        func_00213D28_2B4E40(moby, 1, 0);
        *(float *)(moby + 0x58) = 1.0f / (func_001FA888_2B4E40(*(short *)(data + 0x74)) * D_L14_00161510);
        return;
    case 2:
        {
            float t = (float)*(short *)(data + 0x74) * *(float *)(data + 0x78);
            *(float *)(moby + 0x18) = *(float *)(data + 0x70) - (0.0f + (D_L14_00161508 - 0.0f) * t);
        }
        if (*(unsigned char *)(moby + 0x70) & 2) {
            if (*(unsigned char *)(moby + 0x52) == 1) {
                func_00213D28_2B4E40(moby, 2, 0);
                *(float *)(moby + 0x58) = 1.0f;
            }
        }
        if (*(short *)(data + 0x74) < func_001F9850_2B4E40(D_L14_0016150C >> 1)) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
            *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) | 0x1000;
        }
        if (func_001F9938_2B4E40(data + 0x74) == 0) {
            return;
        }
        *(short *)(data + 0x76) = func_001F9850_2B4E40(D_L14_00161534);
        *(unsigned char *)(moby + 0x20) = 3;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(60);
        *(float *)(moby + 0x18) = *(float *)(data + 0x70);
        *(int *)(data + 0x8C) = 0;
        return;
    case 3:
        r = func_L00_00260D30_2B4E40(moby, v20, 34.0f);
        if (r != 2) {
            if (func_00215570_2B4E40(v20, *(int *)(data + 0x90)) != 0) {
                *(unsigned char *)(moby + 0x20) = 4;
                func_00213D28_2B4E40(moby, 3, 0);
                *(short *)(data + 0x74) = func_001F9850_2B4E40(120);
                *(short *)(data + 0x9C) = func_001F9850_2B4E40(20);
                return;
            }
        }
        if (*(unsigned char *)(data + 0x8A) != 0) {
            return;
        }
        if (func_001F9938_2B4E40(data + 0x76) == 0) {
            return;
        }
        *(unsigned char *)(data + 0x8B) = 1;
        return;
    case 4:
        if (*(short *)(data + 0x9E) != 0) {
            g0 = -D_L14_00161514;
        } else {
            g0 = D_L14_00161514;
        }
        t1 = func_001FA748_2B4E40(*(float *)(data + 0x8C), g0);
        *(float *)(data + 0x8C) = t1;
        t2 = func_001FA748_2B4E40(t1, *(float *)(data + 0x84) * 0.01745329238474369f);
        d80 = *(int *)(data + 0x80);
        p16 = D_L14_001600EC + (d80 << 7) + 0x10;
        t3 = func_001F9FA8_2B4E40(t2);
        func_001F9C30_2B4E40(v20, p16, t3);
        d80 = *(int *)(data + 0x80);
        p6 = D_L14_001600EC + (d80 << 7) + 0x30;
        func_001F9BD8_2B4E40(v20, v20, p6);
        t4 = func_L00_001FF860_2B4E40(v20[0] - *(float *)(moby + 0x10), v20[1] - *(float *)(moby + 0x14));
        *(float *)(moby + 0x48) = t4;
        func_001F9938_2B4E40(data + 0x9C);
        rem = D_L14_0015F5CC % D_L14_00161530;
        if (rem == 0) {
            sel = func_002140B0_2B4E40(3);
            switch (sel) {
            case 0:
                func_L00_001FF4B0_2B4E40(v30, moby + 0xE0, D_L14_00161520);
                break;
            case 1:
                func_L00_001FF4B0_2B4E40(v30, moby + 0xE0, D_L14_00161524);
                func_L00_001FF4B0_2B4E40(v50, moby + 0xD0, D_L14_0016152C);
                func_001F9BD8_2B4E40(v30, v30, v50);
                break;
            default:
                func_L00_001FF4B0_2B4E40(v30, moby + 0xE0, D_L14_00161524);
                func_L00_001FF4B0_2B4E40(v50, moby + 0xD0, -D_L14_0016152C);
                func_001F9BD8_2B4E40(v30, v30, v50);
                break;
            }
            func_L00_001FF4B0_2B4E40(v40, moby + 0xC0, D_L14_00161528);
            func_001F9BD8_2B4E40(v30, v30, v40);
            func_001F9BD8_2B4E40(v30, v30, moby + 0x10);
            func_L00_001FF4B0_2B4E40(v40, moby + 0xC0, D_L14_0016151C * frame_time);
            if (*(short *)(data + 0x9C) == 0) {
                func_L00_00251328_2B4E40(func_L14_002EE150_2B4E40(v30, v40, (int)moby, 1), 0xE0, 0xE0, 0xE0);
            }
        }
        r = func_L00_00260D30_2B4E40(moby, v70, 34.0f);
        c17 = (r == 2 || func_00215570_2B4E40(v70, *(int *)(data + 0x90)) == 0);
        if (c17 == 0) {
            return;
        }
        if (func_001F9938_2B4E40(data + 0x74) == 0) {
            return;
        }
        if ((*(unsigned char *)(moby + 0x70) & 2) == 0) {
            return;
        }
        m52 = *(unsigned char *)(moby + 0x52);
        if (m52 != 3) {
            return;
        }
        *(short *)(data + 0x76) = func_001F9850_2B4E40(D_L14_00161534);
        *(unsigned char *)(moby + 0x20) = m52;
        func_00213D28_2B4E40(moby, 2, 0);
        *(float *)(moby + 0x58) = 1.0f;
        return;
    case 5:
        {
            float t = (float)*(short *)(data + 0x74) * *(float *)(data + 0x78);
            *(float *)(moby + 0x18) = *(float *)(data + 0x70) - (D_L14_00161508 + (0.0f - D_L14_00161508) * t);
            *(float *)(moby + 0x48) = *(float *)(data + 0x84) + (*(float *)(data + 0x8C) - *(float *)(data + 0x84)) * t;
        }
        if (*(unsigned char *)(moby + 0x70) & 2) {
            if (*(unsigned char *)(moby + 0x52) == 4) {
                func_00213D28_2B4E40(moby, 0, 0);
                *(float *)(moby + 0x58) = 1.0f;
            }
        }
        if (*(short *)(data + 0x74) < func_001F9850_2B4E40(D_L14_0016150C >> 1)) {
            *(int *)(moby + 0x94) = 0;
        }
        if (func_001F9938_2B4E40(data + 0x74) == 0) {
            return;
        }
        *(float *)(moby + 0x48) = *(float *)(data + 0x84);
        *(unsigned char *)(moby + 0x20) = 1;
        *(short *)(data + 0x74) = func_001F9850_2B4E40(60);
        *(short *)(data + 0x88) = 0;
        *(int *)(data + 0x98) = func_001F9850_2B4E40(*(int *)(data + 0x94));
        *(float *)(moby + 0x18) = *(float *)(data + 0x70) - D_L14_00161508;
        *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) & 0xEFFF;
        return;
    case 6:
        func_L00_0025F4A8_2B4E40(moby, data + 0x40, moby + 0x10, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 0, 1.0f, 15.0f, 1, 1, -1, 0);
        func_0020D678_2B4E40(moby);
        return;
    default:
        return;
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B5590), where it is exact; names translated to the US level program. */

void FUN_L14_002b4340(struct Moby *m) {
    unsigned char *d = m->pvars;
    int loc0;
    float f4 = 0.0f;
    char *q;
    int k;
    float t;
    q = find_moby_hit(m, 0x330000, 0);
    if (q != 0) {
        if (*(int *)(q + 0x20) != 0) {
            int a = *(short *)(*(int *)(q + 0x20) + 0xA6);
            if (a == m->oclass || a == 0x2A9)
                q = 0;
        }
    }
    k = hit_reaction(m, q, d + 0x20, 0, &loc0, &f4, 0, 4);
    if (loc0 != 1 && m->state != 6) {
        t = *(float *)(d + 0x20) - f4;
        *(float *)(d + 0x20) = t;
        if (t <= 0.0f)
            k = 1;
        switch (k) {
        case 0:
        case 11:
            break;
        case 9:
        case 10:
            d[0x67] = 100;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            d[0x67] = 200;
            break;
        case 1:
        case 2:
            m->state = 6;
            m->flags &= 0xEFFF;
            d[0x67] = 0xF0;
            FUN_L00_00257470(m, 0, -1);
            break;
        }
        FUN_L00_0025d458(m, (short *)(d + 0x60));
    }
    m->hit_slot = 0xFF;
    FUN_L00_0025d538(m, d + 0x60);
}
/* Returns 0 unless every object on the list is ready, then wakes the waiting ones and sets their timers. */
extern short *D_L14_001ABF40_b45[] __asm__("D_L14_001ABF40");
extern unsigned char *D_L14_0015FFD8_b45 __asm__("D_L14_0015FFD8");
extern unsigned char *D_L14_0015FFD0_m45[] __asm__("D_L14_0015FFD0") __attribute__((section(".sdata")));
extern int D_L14_0016150C_b45 __asm__("D_L14_0016150C") __attribute__((sda));
extern float D_L14_00161510_b45 __asm__("D_L14_00161510") __attribute__((sda));
extern void FUN_00212ed8_b45(unsigned char *, int, int) __asm__("FUN_00212ed8");
extern int FUN_001f96f8_b45(int) __asm__("FUN_001f96f8");
extern float FUN_001fa6c0_b45(short) __asm__("FUN_001fa6c0");

int FUN_L14_002b4500(int index) {
    short *p = D_L14_001ABF40_b45[index];
    unsigned char *base;
    unsigned char *q;
    unsigned char *o;
    unsigned char *d;
    int st;
    int t;
    float f;
    float g;
    if (p == 0) {
        return 0;
    }
    base = D_L14_0015FFD8_b45;
    do {
        st = *(unsigned short *)p & 0x7FFF;
        q = base + (st << 8);
        if ((*(unsigned char **)(q + 0x78))[0x8B] == 0) {
            st = q[0x20];
            if (st != 5 && st != 1) {
                return 0;
            }
        }
    } while (*p++ >= 0);
    p = D_L14_001ABF40_b45[index];
    do {
        t = *(unsigned short *)p & 0x7FFF;
        o = D_L14_0015FFD0_m45[2] + (t << 8);
        if (o[0x20] == 3) {
            d = *(unsigned char **)(o + 0x78);
            FUN_00212ed8_b45(o, 4, 0);
            o[0x20] = 5;
            t = FUN_001f96f8_b45(D_L14_0016150C_b45);
            *(short *)(d + 0x74) = t;
            f = 1.0f / FUN_001fa6c0_b45(t);
            g = 1.0f / (*(short *)(d + 0x74) * D_L14_00161510_b45);
            *(float *)(d + 0x78) = f;
            *(float *)(o + 0x58) = g;
            *(float *)(d + 0x8C) = *(float *)(o + 0x48);
            *(short *)(d + 0x88) = 0;
            d[0x8B] = 0;
        }
    } while (*p++ >= 0);
    return 1;
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B58C0), where it is exact; names translated to the US level program. */

extern char *D_L14_ffd0_e[] __asm__("D_L14_0015FFD0") __attribute__((section(".sdata")));
extern short *D_L14_001AC2C0_2B0168[] __asm__("D_L14_001ABF40");

int FUN_L14_002b4670(int index) {
    unsigned short *list = (unsigned short *)D_L14_001AC2C0_2B0168[index];
    int count = 0;
    char *base;
    if (list)
        goto scan;
success:
    return 1;
scan:
    base = D_L14_ffd0_e[2];
    do {
        unsigned short id = *list;
        char *moby = base + ((id & 0x7FFF) << 8);
        int state = *(signed char *)(moby + 0x20);
        char *data = *(char **)(moby + 0x78);
        if (state >= 0)
            count++;
        if (state >= 0 && *(short *)(data + 0x88))
            goto success;
        list++;
        if ((short)id < 0)
            return count == 0;
    } while (1);
}
/* Update of the minibomber (moby class 31): flies along its paths, turns at the ends, dives and drops bombs. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B5938), where it is exact; names translated to the US level program. */


typedef union PathNode_2B5938 {
    float v[4];
    int n;
} PathNode_2B5938;

typedef struct Class_2B5938 {
    char pad00[0x10];
    int f10;
} Class_2B5938;

typedef struct Moby_2B5938 {
    char pad00[0x10];
    float pos[4];               /* 0x10 */
    unsigned char state;        /* 0x20 */
    char pad21[3];
    Class_2B5938 *cls;          /* 0x24 */
    char pad28[8];
    unsigned char f30;          /* 0x30 */
    unsigned char f31;          /* 0x31 */
    char pad32[2];
    unsigned short f34;         /* 0x34 */
    char pad36[0x12];
    float rot;                  /* 0x48 */
    char pad4C[6];
    unsigned char f52;          /* 0x52 */
    unsigned char f53;          /* 0x53 */
    char pad54[0x1C];
    unsigned char f70;          /* 0x70 */
    char pad71[7];
    struct Data_2B5938 *d;      /* 0x78 */
    char pad7C[3];
    unsigned char f7F;          /* 0x7F */
    char pad80[0x14];
    int f94;                    /* 0x94 */
    char pad98[0x1A];
    short fB2;                  /* 0xB2 */
} Moby_2B5938;

typedef struct Data_2B5938 {
    char pad00[0x40];
    float f40[2];               /* 0x40 */
    float f48;                  /* 0x48 */
    char pad4C[0x64];
    Moby_2B5938 *fB0;           /* 0xB0 */
    int fB4;                    /* 0xB4 */
    char padB8[0x18];
    int fD0;                    /* 0xD0 */
    int fD4;                    /* 0xD4 */
    int fD8;                    /* 0xD8 */
    int fDC;                    /* 0xDC */
    char padE0[4];
    float fE4;                  /* 0xE4 */
    int fE8;                    /* 0xE8 */
    float fEC;                  /* 0xEC */
    float fF0[4];               /* 0xF0 */
    short f100;                 /* 0x100 */
    short f102;                 /* 0x102 */
    float f104;                 /* 0x104 */
    int f108;                   /* 0x108 */
    int f10C;                   /* 0x10C */
    char f110[0x10];            /* 0x110 */
    char f120[0x30];            /* 0x120 */
    int f150;                   /* 0x150 */
    float f154;                 /* 0x154 */
    float f158;                 /* 0x158 */
    float f15C;                 /* 0x15C */
    float f160;                 /* 0x160 */
    float f164;                 /* 0x164 */
    float f168;                 /* 0x168 */
    float f16C;                 /* 0x16C */
    float f170;                 /* 0x170 */
    float f174;                 /* 0x174 */
    float f178;                 /* 0x178 */
    float f17C;                 /* 0x17C */
    float f180[4];              /* 0x180 */
} Data_2B5938;

char *FUN_L14_003014e0(int a0, float *pos, float *a2, int a3, float f);
extern char D_0013E550_c[] __asm__("D_0013E550");
char D_L14_001FBDF0[] = "Hovertank with no introPath. UID: %d\n";
char D_L14_001FBE18[] = "Hovertank with a intro path that has no points. UID: %d\n";
char D_L14_001FBE58[] = "Hovertank with no boundPath. UID: %d\n";
char D_L14_001FBE80[] = "Hovertank with a bound path that has no points. UID: %d\n";
char D_L14_001FBEC0[] = "Hovertank with no bombPath. UID: %d\n";
char D_L14_001FBEE8[] = "Hovertank with a bomb path that has no points. UID: %d\n";
char D_L14_001FBF20[] = "Hovertank with no exit Path. UID: %d\n";
char D_L14_001FBF48[56] = {0x48, 0x6f, 0x76, 0x65, 0x72, 0x74, 0x61, 0x6e, 0x6b, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x61, 0x20, 0x65, 0x78, 0x69, 0x74, 0x20, 0x70, 0x61, 0x74, 0x68, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x68, 0x61, 0x73, 0x20, 0x6e, 0x6f, 0x20, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x73, 0x2e, 0x20, 0x55, 0x49, 0x44, 0x3a, 0x20, 0x25, 0x64, 0xa, 0};
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern float D_0015ED60;
extern float D_L14_00161568 __attribute__((sda));
extern float blend_scalar_alt(float, float, float) __asm__("FUN_002133d0");
extern float D_L14_0016154C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161550 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161554 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016155C __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161560 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_00161564 __attribute__((sda)); /* no foreign declaration */
extern int D_L14_0016156C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161570 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161574 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161578 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016157C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161580 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161584 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161588 __attribute__((sda)); /* no foreign declaration */
extern float D_L14_0016158C __attribute__((sda)); /* no foreign declaration */
extern float D_L14_00161590 __attribute__((sda)); /* no foreign declaration */
extern int debug_print_alt() __asm__("FUN_001e93b0");
extern int FUN_001f9770_c(void *) __asm__("FUN_001f9770");
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_c(int) __asm__("FUN_001f96f8");
extern float D_L14_00161540 __attribute__((sda));
extern unsigned char *FUN_L14_002bb310_c(char *owner, float angle, char *posp) __asm__("FUN_L14_002bb310");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L14_002b5290_c(unsigned char *m) __asm__("FUN_L14_002b5290");
extern void FUN_L14_002b5428_c(char *moby) __asm__("FUN_L14_002b5428");
extern void FUN_L14_002b5530_c(char *moby) __asm__("FUN_L14_002b5530");
extern void FUN_L14_002b55b0_c(char *moby) __asm__("FUN_L14_002b55b0");
extern void FUN_L14_002b5850_c(char *moby, float *target, float a, float b, float c, float d, float e, float f, float g) __asm__("FUN_L14_002b5850");
extern void FUN_L14_002b5968_u(char *m, float *input, float angle) __asm__("FUN_L14_002b5968");
extern void FUN_L14_002b5c30_c(char *moby) __asm__("FUN_L14_002b5c30");
extern void FUN_L14_002b5c98_c(char *moby) __asm__("FUN_L14_002b5c98");
extern void FUN_L14_002b5d18_c(char *moby) __asm__("FUN_L14_002b5d18");
extern void FUN_L14_002b5dd8_c(char *m) __asm__("FUN_L14_002b5dd8");
extern void fill_transfer_words(void *, s32, s32) __asm__("func_001F97E8");
extern void attach_manipulator_c(int, int, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void release_voice_slot_c(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void set_moby_animation_c(void *, s32, s32) __asm__("func_00212ED8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
int FUN_L14_002b5730_c(struct Moby *moby, char *a1, int a2, int a3, float *v, float *vel, float f12, float f13, float f14, float f15, float f16, float f17, float f18) __asm__("FUN_L14_002b5730");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
extern PathNode_2B5938 *D_L14_001B0F30_2B5938[] __asm__("D_L14_001B0BB0");
extern char *func_L14_00302968_2B5938(int a0, float *a1, float *a2, float f, int a3) __asm__("FUN_L14_003014e0");
extern void func_L00_0025F4A8_2B5938(void *, void *, void *, float, float, int, int, int, float, float, float, float,
                                     int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void FUN_L14_002b46e8(Moby_2B5938 *moby) {
    Data_2B5938 *d = moby->d;
    float v[12];
    float *tgt;
    char *buf;
    float tg;
    float a;
    float n;
    float g;
    float spd;

    if (moby->f70 & 2) {
        if (moby->f52 == moby->f53 && moby->f52 != 0) {
            if (moby->f52 == 2) {
                blend_moby_animation_c(moby, 0, 0, 10);
            } else {
                set_moby_animation_c(moby, 0, 0);
            }
        }
    }
    tgt = d->f180;
    FUN_L14_002b5290_c((unsigned char *)moby);
    FUN_L14_002b5530_c((char *)moby);
    FUN_L14_002b5850_c((char *)moby, tgt, d->f17C, d->f170, d->f174, d->f178, d->f154, d->f158, d->f15C);
    FUN_L14_002b5428_c((char *)moby);
    FUN_L14_002b5dd8_c((char *)moby);
    if (moby->f31 != 0) {
        if (distance_xyz(moby->pos, D_L14_001674C0) < 40.0f && moby->state != 0 && moby->state != 1 &&
            moby->state != 9) {
            FUN_L00_0025a120(moby);
        }
    }
    switch (moby->state) {
    case 0: {
        PathNode_2B5938 *path;
        if (d->fD0 == -1) {
            debug_print_alt(D_L14_001FBDF0, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        path = D_L14_001B0F30_2B5938[d->fD0];
        if (path->n == 0) {
            debug_print_alt(D_L14_001FBE18, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->fD4 == -1) {
            debug_print_alt(D_L14_001FBE58, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (D_L14_001B0F30_2B5938[d->fD4]->n == 0) {
            debug_print_alt(D_L14_001FBE80, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->fD8 == -1) {
            debug_print_alt(D_L14_001FBEC0, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (D_L14_001B0F30_2B5938[d->fD8]->n == 0) {
            debug_print_alt(D_L14_001FBEE8, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (d->f150 == -1) {
            debug_print_alt(D_L14_001FBF20, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        if (D_L14_001B0F30_2B5938[d->f150]->n == 0) {
            debug_print_alt(D_L14_001FBF48, moby->fB2);
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(moby->pos, path + 1);
        subtract_vector_xyz_c(v, path + 2, moby->pos);
        buf = d->f110;
        moby->rot = fast_arc_tan(v[0], v[1]);
        d->f160 = *(float *)&D_L14_00161540;
        d->f164 = 0.0f;
        d->f168 = 0.0f;
        d->f16C = 0.0f;
        FUN_L14_002b55b0_c((char *)moby);
        d->f108 = -1;
        moby->f30 = 0x80;
        moby->state = 1;
        FUN_L14_002b5c98_c((char *)moby);
        fill_transfer_words(buf, 0, 0x40);
        attach_manipulator_c(moby, 0, buf);
        moby->f7F = 0x20;
        break;
    }
    case 1:
        if (d->fB4 != 2) {
            moby->state = 2;
            moby->f34 &= 0xFFBE;
            moby->f94 = moby->cls->f10;
            FUN_L14_002b5d18_c((char *)moby);
        }
        break;
    case 2: {
        PathNode_2B5938 *path;
        if (d->f10C == 0) {
            path = D_L14_001B0F30_2B5938[d->fD0];
        } else {
            path = D_L14_001B0F30_2B5938[d->f150];
        }
        if (d->fB4 == 2) {
            moby->state = 3;
        } else if (FUN_L14_002b5730_c((char *)moby, (char *)path, (int)&d->fE8, (int)&d->fEC, d->fF0, &d->fE4,
                                     D_L14_0016154C * D_0015ED60, D_L14_00161574, D_L14_00161578, D_L14_0016157C,
                                     D_L14_00161580, D_L14_00161584, D_L14_00161588) != 0) {
            qcopy(d->f180, path + path->n);
            path = D_L14_001B0F30_2B5938[d->fDC];
            if (d->f10C == 0) {
                qcopy(v, path + 1);
                qcopy(v + 4, path + path->n);
            } else {
                qcopy(v, path + path->n);
                qcopy(v + 4, path + 1);
            }
            subtract_vector_xyz_c(v + 8, v + 4, v);
            d->f17C = fast_arc_tan(v[8], v[9]);
            d->f170 = D_L14_00161574;
            d->f174 = D_L14_00161578;
            d->f178 = D_L14_0016157C;
            d->f154 = D_L14_00161580;
            d->f158 = D_L14_00161584;
            d->f15C = D_L14_00161588;
            moby->state = 4;
            d->f104 = 0.0f;
            d->f102 = 0;
        }
        break;
    }
    case 3: {
        PathNode_2B5938 *path;
        if (d->f10C == 0) {
            path = D_L14_001B0F30_2B5938[d->fD0];
        } else {
            path = D_L14_001B0F30_2B5938[d->f150];
        }
        if (d->fB4 != 2) {
            moby->state = 2;
        } else if (FUN_L14_002b5730_c((char *)moby, (char *)path, (int)&d->fE8, (int)&d->fEC, d->fF0, &d->fE4,
                                     -(D_L14_00161554 * D_0015ED60), D_L14_0016158C, D_L14_00161578, D_L14_0016157C,
                                     D_L14_00161590, D_L14_00161584, D_L14_00161588) != 0) {
            moby->state = 1;
            FUN_L14_002b5c98_c((char *)moby);
        }
        break;
    }
    case 4:
        tg = 0.0f;
        a = blend_scalar_alt(tg, d->f104, (float)d->f102 / convert_integer_to_float(scale_game_frames_c(D_L14_00161560)));
        FUN_L00_001fff28(d->f120, 2, a);
        if (FUN_001f9b80(moby->pos, d->fB0->pos) < D_L14_00161570) {
            if (d->f10C == 0) {
                d->fE8 = 0;
                d->fEC = 0.0f;
            } else {
                PathNode_2B5938 *q;
                d->fE8 = D_L14_001B0F30_2B5938[d->fDC]->n - 2;
                q = D_L14_001B0F30_2B5938[d->fDC] + d->fE8;
                d->fEC = q[1].v[3];
            }
            moby->state = 7;
            d->fE4 = 0.0f;
            clear_u64_value_c(d->fF0);
        } else if (d->fB4 == 2) {
            moby->state = 3;
            FUN_L14_002b5c30_c((char *)moby);
        } else if (FUN_001f9770_c(&d->f102) != 0) {
            if (moby->f53 != 2) {
                moby->state = 5;
                d->f102 = scale_game_frames_c(D_L14_0016155C);
                d->f104 = 0.0f;
                d->f100 = 0;
            }
        }
        break;
    case 7:
        spd = D_L14_00161550 * D_0015ED60;
        if (d->f10C == 1) {
            spd = -spd;
        }
        if (FUN_L14_002b5730_c((char *)moby, (char *)D_L14_001B0F30_2B5938[d->fDC], (int)&d->fE8, (int)&d->fEC, d->fF0,
                              &d->fE4, spd, D_L14_0016158C, D_L14_00161578, D_L14_0016157C, D_L14_00161590,
                              D_L14_00161584, D_L14_00161588) != 0) {
            PathNode_2B5938 *path;
            moby->state = 4;
            if (d->f10C == 0) {
                d->f10C = 1;
            } else {
                d->f10C = 0;
            }
            path = D_L14_001B0F30_2B5938[d->fDC];
            if (d->f10C == 0) {
                qcopy(v, path + 1);
                qcopy(v + 4, path + path->n);
            } else {
                qcopy(v, path + path->n);
                qcopy(v + 4, path + 1);
            }
            d->f170 = D_L14_0016158C;
            d->f174 = D_L14_00161578;
            d->f178 = D_L14_0016157C;
            d->f154 = D_L14_00161590;
            d->f158 = D_L14_00161584;
            d->f15C = D_L14_00161588;
            subtract_vector_xyz_c(v + 8, v + 4, v);
            d->f17C = fast_arc_tan(v[8], v[9]);
            qcopy(d->f180, v);
        }
        break;
    case 5: {
        PathNode_2B5938 *q = D_L14_001B0F30_2B5938[d->fD8] + d->f100;
        tg = fast_subtract_rotations(fast_arc_tan(q[1].v[0] - moby->pos[0], q[1].v[1] - moby->pos[1]), moby->rot);
        a = blend_scalar_alt(tg, d->f104, (float)d->f102 / convert_integer_to_float(scale_game_frames_c(D_L14_0016155C)));
        FUN_L00_001fff28(d->f120, 2, a);
        if (moby->f53 == 2) {
            moby->state = 4;
            d->f102 = scale_game_frames_c(D_L14_00161560);
        } else if (FUN_001f9770_c(&d->f102) != 0) {
            d->f104 = a;
            moby->state = 6;
            set_moby_animation_c(moby, 1, 0);
        }
        break;
    }
    case 6:
        if (is_value_within_interpolated_window(moby, 3.0f) != 0) {
            PathNode_2B5938 *q;
            n = (float)scale_game_frames_c(D_L14_00161564);
            FUN_L00_001fff28(d->f120, 2, d->f104);
            moby_point_position(moby, 1, v);
            subtract_vector_xyz_c(v + 4, D_L14_001B0F30_2B5938[d->fD8] + (d->f100 + 1), v);
            v[6] = 0.0f;
            scale_vector_xyz_c(v + 4, v + 4, 1.0f / n);
            g = D_L14_00161568 * frame_time_sq;
            q = D_L14_001B0F30_2B5938[d->fD8] + d->f100;
            d->f48 = -((v[2] - q[1].v[2]) + -g * (n * n) * 0.5f) / n;
            func_L14_00302968_2B5938((int)moby, v, v + 4, g, scale_game_frames_c(D_L14_0016156C));
            FUN_L14_002b5968_u((char *)moby, v, d->f104);
            d->f100++;
            if (d->f100 >= D_L14_001B0F30_2B5938[d->fD8]->n) {
                moby->state = 4;
                d->f102 = scale_game_frames_c(D_L14_00161560);
            } else {
                d->f102 = scale_game_frames_c(D_L14_0016155C);
                moby->state = 5;
            }
        }
        break;
    case 9: {
        int i;
        moby_point_position(moby, 2, v);
        FUN_L14_002bb310_c((char *)moby, moby->rot, (char *)v);
        moby_point_position(moby, 3, v);
        FUN_L14_002bb310_c((char *)moby, fast_add_rotations_c(moby->rot, 3.1415927f), (char *)v);
        func_L00_0025F4A8_2B5938(moby, d->f40, moby->pos, 0.0f, 0.0f, 5, 2, 6, 4.0f, 1.0f, 9.0f, 1.0f, 1, 15.0f, 1, 1,
                                 -1, 0);
        i = d->f108;
        if (i != -1) {
            char *e = D_0013E550_c + i * 0x70;
            if (*(Moby_2B5938 **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                release_voice_slot_c(i);
            }
        }
        d->f108 = -1;
        mark_moby_for_removal(moby);
        return;
    }
    }
    if (moby->state > 1 && moby->state != 9) {
        if (FUN_L00_0028d8c0(moby, d->f108) == 0) {
            d->f108 = allocate_voice_for_target_entry_alt(0, 4, (int)moby);
        }
    }
}
/* Same source as the exact FUN_L14_002b4340: skips states 0 and 9, keeps the model scale, and hit reactions play animation 2. */
extern int FUN_L00_0025a478_c(void *, void *, void *, int, int *, float *, int,
                              int) __asm__("FUN_L00_0025a478");
extern void FUN_00212f90(void *, int, int, int);
extern void FUN_L00_0025d458_c(void *, void *) __asm__("FUN_L00_0025d458");

void FUN_L14_002b5290(struct Moby *m) {
    unsigned char *d = m->pvars;
    int loc0;
    float f4;
    char *q;
    int k;
    float t;
    if (m->state == 0 || m->state == 9)
        return;
    m->scale = m->pclass->scale;
    f4 = 0.0f;
    q = find_moby_hit(m, 0x330000, 0);
    if (q != 0) {
        if (*(int *)(q + 0x20) != 0) {
            int a = *(short *)(*(int *)(q + 0x20) + 0xA6);
            if (a == m->oclass || a == 0x370)
                q = 0;
        }
    }
    k = FUN_L00_0025a478_c(m, q, d + 0x20, 0, &loc0, &f4, 0, 4);
    if (loc0 != 1 && m->state != 9) {
        t = *(float *)(d + 0x20) - f4;
        *(float *)(d + 0x20) = t;
        if (t <= 0.0f)
            k = 1;
        switch (k) {
        case 0:
        case 11:
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            FUN_00212f90(m, 2, 0, 5);
            m->anim_speed = 1.75f;
        case 9:
        case 10:
            d[0x67] = 0xFA;
            break;
        case 1:
        case 2:
            m->state = 9;
            m->flags &= 0xEFFF;
            d[0x67] = 0xFA;
            FUN_L00_00257470(m, 0, -1);
            break;
        }
        FUN_L00_0025d458_c(m, d + 0x60);
    }
    m->hit_slot = 0xFF;
    FUN_L00_0025d538(m, d + 0x60);
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6678), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c(float *p, float *v, float t, float u1, float u2,
                                float eps) __asm__("FUN_L00_0025b8c0");
extern f32 D_L14_00161540_d __asm__("D_L14_00161540") __attribute__((sda));
extern void FUN_L00_00262b00_c(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void FUN_L00_00262b80_c(void *, char *, char *, float, float,
                               float) __asm__("FUN_L00_00262b80");

extern short D_L14_00161538 __attribute__((sda));
extern short D_L14_0016153C __attribute__((sda));
extern short D_L14_00161544 __attribute__((sda));
extern short D_L14_00161548 __attribute__((sda));
void FUN_L14_002b5428(struct Moby *moby) {
    int raw = moby->state;
    unsigned char state = raw;
    char *data = (char *)moby->pvars;
    float t;
    if (state == 0 || state == 9 || state == 1)
        return;
    t = *(float *)&D_L14_00161540_d;
    if ((raw >= 2 && raw <= 3) || state == 7) {
        t = t * 1.5f;
    } else {
        FUN_L00_0025b8c0_c((float *)(data + 0x168), (float *)(data + 0x16C),
                           *(float *)&D_L14_0016153C, 0.002f, 0.2f, 0.0f);
        FUN_L00_00262b00_c(*(float *)&D_L14_00161538, *(float *)(data + 0x168), moby,
                           (float *)(data + 0xC0), (float *)(data + 0xC4));
    }
    FUN_L00_0025b8c0_c((float *)(data + 0x160), (float *)(data + 0x164), t, 0.003f, 0.2f, 0.0f);
    FUN_L00_00262b80_c(moby, data + 0xC8, data + 0xCC, *(float *)(data + 0x160),
                       *(float *)&D_L14_00161544, *(float *)&D_L14_00161548);
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6780), where it is exact; names translated to the US level program. */

extern int *D_L14_001B0BB0[];
extern int FUN_L00_0025ff38(float a, char *m, char *b, int c, int d, int *e, int f);

void FUN_L14_002b5530(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->state != 0 && moby->state != 9) {
        int *p = D_L14_001B0BB0[*(int *)(data + 0xD4)];
        FUN_L00_0025ff38(255.0f, moby, data + 0x70, 0, 0, p + 4, *p);
        if (*(int *)(data + 0xB0) == 0) {
            *(int *)(data + 0xB0) = *(int *)(D_0013E533 + 0x2E9D);
        }
    }
}
/* Recomputes the per-segment lengths of three paths held by the moby, then clears a counter. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6800), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);

void FUN_L14_002b55b0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *p;
    int i;
    p = D_L14_001B0BB0[*(int *)(data + 0xD0)];
    for (i = 0; i < *(int *)p - 1; i++) {
        *(float *)(p + 0x1C + i * 0x10) = FUN_001f9b48(p + 0x10 + i * 0x10, p + 0x20 + i * 0x10);
    }
    p = D_L14_001B0BB0[*(int *)(data + 0x150)];
    for (i = 0; i < *(int *)p - 1; i++) {
        *(float *)(p + 0x1C + i * 0x10) = FUN_001f9b48(p + 0x10 + i * 0x10, p + 0x20 + i * 0x10);
    }
    p = D_L14_001B0BB0[*(int *)(data + 0xDC)];
    for (i = 0; i < *(int *)p - 1; i++) {
        *(float *)(p + 0x1C + i * 0x10) = FUN_001f9b48(p + 0x10 + i * 0x10, p + 0x20 + i * 0x10);
    }
    *(int *)(data + 0x10C) = 0;
}
#include "sda.h"

/* steer a moby's position toward a target and update its velocity */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6980), where it is exact; names translated to the US level program. */

extern float atan_5730(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025b8c0_5730(float *p, float *v, float t, float u1, float u2,
                                   float eps) __asm__("FUN_L00_0025b8c0");
extern void steer_5730(char *moby, float *vel, float target, float k, float d,
                       float max) __asm__("FUN_L00_00258278");

extern int FUN_L00_0025d808_5730() __asm__("FUN_L00_0025d808");
int FUN_L14_002b5730(struct Moby *moby, char *a1, int a2, int a3, float *v, float *vel, float f12,
                     float f13, float f14, float f15, float f16, float f17, float f18) {
    float tmp[3];
    int r = FUN_L00_0025d808_5730(a1, tmp, a2, a3, 0);
    FUN_L00_0025b8c0_5730(&moby->pos.x, v, tmp[0], f13, f14, f15);
    FUN_L00_0025b8c0_5730(&moby->pos.y, v + 1, tmp[1], f13, f14, f15);
    FUN_L00_0025b8c0_5730(&moby->pos.z, v + 2, tmp[2], f13, f14, f15);
    steer_5730(moby, vel, atan_5730(v[0], v[1]), f16, f17, f18);
    return r != 0;
}
/* eases a moby's position and heading toward targets while in states 4-6 or 8 */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002B6AA0), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025b8c0_c2(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void func_L00_002592B0_2B6AA0(char *moby, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258278");

void FUN_L14_002b5850(char *moby, float *target, float a, float b, float c, float d, float e, float f, float g) {
    int data;
    int s = moby[0x20] & 0xFF;
    if ((s >= 4 && s <= 6) || s == 8) {
        data = *(int *)(moby + 0x78);
        FUN_L00_0025b8c0_c2((float *)(moby + 0x10), (float *)(data + 0xF0), target[0], b, c, d);
        FUN_L00_0025b8c0_c2((float *)(moby + 0x14), (float *)(data + 0xF4), target[1], b, c, d);
        FUN_L00_0025b8c0_c2((float *)(moby + 0x18), (float *)(data + 0xF8), target[2], b, c, d);
        func_L00_002592B0_2B6AA0(moby, a, (float *)(data + 0xE4), e, f, g);
    }
}
typedef struct {
    unsigned char pad0[4];
    int w4;
    unsigned char pad8[2];
    unsigned char bA;
    unsigned char bB;
} PuffParticleTail;
typedef struct {
    unsigned char pad0[8];
    unsigned char b8;
    unsigned char b9;
    short life; /* 0xA: frames left (scale_game_frames) */
    unsigned char padC[0x14];
    PuffParticleTail tail;
} PuffParticle;
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void add_vector_xyz(void *, void *, void *);
extern void scale_vector_xyz(void *, void *, float);
extern int random_integer_below(int);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float random_float_between(float, float);
extern int scale_game_frames(int);
extern void build_look_at_matrix(void *, void *, void *, float);
extern void FUN_L00_0026c6e8(void *, void *);
extern void FUN_L00_002d3838(int, void *, int, int);
char D_L14_001D88D0[7768] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0x19, 0x19, 0, -1, 0, 0x19, 0x19, 0, 0x64, 0, 0x19, 0x19, 0, 0, 0, 0x40, 0, 0, 0, 0x40, 0, 0, 0, 0x40, 0xa, -41, 0x23, 0x3c, 0xa, -41, 0x23, 0x3c, 0x14, 0, 0, 0, 0, 0, 8, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -59, -56, 0x22, -66, 0x66, 0x2e, 0x44, 0x3f, -91, -11, 3, 0x3f, 0, 0, -128, 0x3f, 0x20, 0xb, 9, -65, 0x45, 0x2a, -58, 0x3f, -49, -66, 0x2a, 0x3f, 0, 0, -128, 0x3f, -39, 0x5a, 0x33, 0x3f, -48, 0x43, -67, -66, -110, -82, -31, 0x3e, 0, 0, -128, 0x3f, 0x41, 0x2e, -55, 0x3f, -62, -5, 0x6e, -65, -11, 0x47, 0x20, 0x3f, 0, 0, -128, 0x3f, -62, 0x50, 0x2b, 0x40, 0x50, -114, 0x22, -66, 0x74, -75, 0xd, 0x3f, 0, 0, -128, 0x3f, 0x20, -104, 0x6b, 0x3f, -71, 0x18, -61, 0x3d, -128, -15, -68, 0x3e, 0, 0, -128, 0x3f, -55, 0x21, -70, -66, 0x64, -82, 0x10, -65, -117, 0x51, 0x23, 0x3f, 0, 0, -128, 0x3f, -35, -19, 2, -65, -73, 0x45, -99, -65, 0x50, 0x72, 0x47, 0x3f, 0, 0, -128, 0x3f, 0x6e, 0x4e, -91, -67, 0x73, 0x2c, -93, -65, -115, 0x61, 0x3e, 0x3f, 0, 0, -128, 0x3f, 0x2f, -92, -61, -67, 0x31, 0x5f, 0x22, -65, 7, 0x24, 0x19, 0x3f, 0, 0, -128, 0x3f, -59, 0x37, 0x28, -65, -89, -108, -17, -66, 0x59, 0x31, 0x30, 0x3f, 0, 0, -128, 0x3f, -1, 0x25, 0x5d, -65, -47, 0x20, -119, -65, -4, -113, 0x50, 0x3f, 0, 0, -128, 0x3f, 0x12, -11, 0x42, 0x3e, 0x5e, -40, 0x22, -65, -21, 0x39, 0xd, 0x3f, 0, 0, -128, 0x3f, -125, -64, -78, 0x3e, -70, -9, -98, -65, -68, 0x24, 0x36, 0x3f, 0, 0, -128, 0x3f, 0x37, 0x6f, 0x60, 0x3f, 0x28, -127, -109, -65, -113, -115, 0x2c, 0x3f, 0, 0, -128, 0x3f, -61, 0x2d, -9, 0x3e, 0x4a, 8, 0xe, -65, -78, 0x49, -2, 0x3e, 0, 0, -128, 0x3f, 0x7a, 0x6e, 0x51, 0x3f, -72, -84, 0xe, 0x3f, -12, -116, -59, 0x3e, 0, 0, -128, 0x3f, 0x19, -113, 0xb, 0x40, 0x12, -95, -119, 0x3f, 0x44, 0x6a, 0x12, 0x3f, 0, 0, -128, 0x3f, 0x1b, 0x4c, -59, 0x3f, -127, 6, -73, 0x3f, 0x5d, -121, 0x16, 0x3f, 0, 0, -128, 0x3f, 0x79, -53, 0x21, 0x3f, 0x2c, -100, 0x3c, 0x3f, 0x61, -32, -39, 0x3e, 0, 0, -128, 0x3f, 0x58, -53, 0x25, 0x40, -118, 0x21, 0x15, 0x3f, -79, -121, 0xe, 0x3f, 0, 0, -128, 0x3f, 0x69, -114, 0x6c, 0x3f, -73, 0x29, -90, 0x3e, -41, -9, -71, 0x3e, 0, 0, -128, 0x3f, 0xd, 0x6d, -48, 0x3e, 5, -62, 0x56, 0x3f, -109, -113, -19, 0x3e, 0, 0, -128, 0x3f, 0x40, -94, 0x4d, 0x3f, 0x19, 0x57, -42, 0x3f, 0x64, 2, 0x16, 0x3f, 0, 0, -128, 0x3f, -86, 0x2c, 0xa, 0x3e, -118, -57, -31, 0x3f, 0x3d, 0x7d, 0x18, 0x3f, 0, 0, -128, 0x3f, -110, 0x3b, 0xc, 0x3e, 0x6a, -122, 0x5c, 0x3f, 0x24, -19, -10, 0x3e, 0, 0, -128, 0x3f, -21, 2, 0x66, -65, -61, 0x7d, -60, -66, 9, 0x53, 0x40, 0x3f, 0, 0, -128, 0x3f, -45, -36, -122, -65, -57, 0x48, 0x56, -65, 0x4c, 0x17, 0x56, 0x3f, 0, 0, -128, 0x3f, -93, -112, 0xd, 0x40, 0x4d, 0x10, 0x2d, -65, -29, -58, 0x15, 0x3f, 0, 0, -128, 0x3f, -85, 0x5d, 0x57, 0x3f, 6, -40, 0x17, -66, 0x37, -32, -53, 0x3e, 0, 0, -128, 0x3f, -15, -96, 0x7d, -65, -124, 0x2d, -108, 0x3f, 0x30, 0x2a, 0x35, 0x3f, 0, 0, -128, 0x3f, 0x30, 0x2b, -60, -66, -5, -107, 0x12, 0x3f, -1, -108, 0xa, 0x3f, 0, 0, -128, 0x3f, -49, -64, 0x48, -68, -17, 0x70, 0x53, 0x3f, 0x1b, 0x4a, -3, 0x3e, 0, 0, -128, 0x3f, -69, -73, 0x52, -66, 0x18, 0x5d, -40, 0x3f, -56, -74, 0x20, 0x3f, 0, 0, -128, 0x3f, 0x34, -42, -114, 0x3e, -14, -103, 0x5c, 0x3f, -40, -11, -13, 0x3e, 0, 0, -128, 0x3f, -4, -28, -24, 0x3e, -68, -23, -34, 0x3f, -73, -100, 0x17, 0x3f, 0, 0, -128, 0x3f, 0x26, 0x54, -110, 0x3f, 0x7d, 0x5c, -55, 0x3f, 0x53, 0x5a, 0x17, 0x3f, 0, 0, -128, 0x3f, 0x30, -98, 5, 0x3f, -105, -116, 0x4b, 0x3f, -47, -81, -27, 0x3e, 0, 0, -128, 0x3f, -68, 0x3f, -24, 0x3f, 0x10, -111, -90, 0x3f, -99, 0x11, 0x15, 0x3f, 0, 0, -128, 0x3f, 0x62, -66, 0x3c, 0x3f, 0x71, 0x54, 0x26, 0x3f, 0x3e, -82, -51, 0x3e, 0, 0, -128, 0x3f, 0x2c, 0xd, 0x19, 0x40, -124, 0xe, 0x5e, 0x3f, 7, -22, 0x10, 0x3f, 0, 0, -128, 0x3f, 0x5a, -97, 0x62, 0x3f, 0xe, -39, -32, 0x3e, -23, -103, -66, 0x3e, 0, 0, -128, 0x3f, 0x76, 0x35, 0x2b, 0x40, -66, -66, 0x66, 0x3e, -98, -78, 0xe, 0x3f, 0, 0, -128, 0x3f, -10, 0x41, 0x6e, 0x3f, 0x51, -64, 0x46, 0x3e, -28, -64, -69, 0x3e, 0, 0, -128, 0x3f, 0x30, -96, 0x1e, 0x40, -113, 0x19, -8, -66, 0x60, -51, 0x11, 0x3f, 0, 0, -128, 0x3f, -81, -75, 0x63, 0x3f, 0x43, -28, 0x74, -68, -106, -22, -62, 0x3e, 0, 0, -128, 0x3f, 0x72, 0x6d, -16, 0x3f, -86, 0x62, 0x52, -65, -88, 0x52, 0x1b, 0x3f, 0, 0, -128, 0x3f, -107, -42, 0x47, 0x3f, -120, 0x68, -116, -66, -123, 0x40, -42, 0x3e, 0, 0, -128, 0x3f, 0x4f, 0x5c, -100, 0x3f, 0x65, -3, -122, -65, -51, 0x23, 0x27, 0x3f, 0, 0, -128, 0x3f, 0x45, -72, 0x19, 0x3f, 0x68, -26, -15, -66, 0x7f, -68, -17, 0x3e, 0, 0, -128, 0x3f, 0x4f, 3, 0x16, 0x3f, -40, -14, -102, -65, -83, 0x4e, 0x32, 0x3f, 0, 0, -128, 0x3f, 0x65, -29, -87, 0x3e, 9, 0x53, 0x1c, -65, -117, -59, 7, 0x3f, 0, 0, -128, 0x3f, -33, -3, -15, 0x3d, -65, 0x7d, -95, -65, -102, 0x5e, 0x3a, 0x3f, 0, 0, -128, 0x3f, -91, 0x15, 0x1f, 0x3d, 0x6b, 0x2c, 0x25, -65, 3, 0x60, 0x14, 0x3f, 0, 0, -128, 0x3f, -32, -42, -107, -66, 7, -50, -95, -65, 0x6d, -113, 0x42, 0x3f, 0, 0, -128, 0x3f, -128, -16, 0x71, -66, -50, 0x36, 0x1b, -65, -87, 0x6a, 0x1e, 0x3f, 0, 0, -128, 0x3f, -109, 0x35, 0x32, -65, 0x2a, 0x3a, -108, -65, -28, 0x2c, 0x4c, 0x3f, 0, 0, -128, 0x3f, 0xa, -67, 2, -65, 0x78, -44, 4, -65, -57, 0x2e, 0x29, 0x3f, 0, 0, -128, 0x3f, 0x66, 0x4b, 0x7a, -65, -27, 0x45, 0x6e, -65, -18, 0x22, 0x54, 0x3f, 0, 0, -128, 0x3f, 0x22, 0x36, 0x48, -65, 0x50, -4, -40, -66, 0x14, 0x3e, 0x37, 0x3f, 0, 0, -128, 0x3f, 0x4e, 0x41, -94, -65, 0x3e, -76, 3, -65, 0x4c, -120, 0x59, 0x3f, 0, 0, -128, 0x3f, 0x77, 0x65, -105, -65, -128, -98, 0x76, -66, -73, 0x27, 0x4c, 0x3f, 0, 0, -128, 0x3f, 0x3a, 0, 2, 0, 0x3b, 0, 3, 0, 0x1b, 0, 5, 0, 0x1a, 0, 4, 0, 0x39, 0, 6, 0, 0xa, 0, 7, 0, 0x38, 0, 9, 0, 0xb, 0, 8, 0, 0x37, 0, 0xa, 0, 6, 0, 0xb, 0, 0x36, 0, 0xd, 0, 7, 0, 0xc, 0, 0x35, 0, 0xe, 0, 9, 0, 0xf, 0, 0x34, 0, 0x11, 0, 8, 0, 0x10, 0, 0x33, 0, 0x12, 0, 0xc, 0, 0x13, 0, 0x32, 0, 0x15, 0, 0xd, 0, 0x14, 0, 0x31, 0, 0x16, 0, 0xf, 0, 0x17, 0, 0x30, 0, 0x19, 0, 0xe, 0, 0x18, 0, 0x2f, 0, 0x1a, 0, 2, 0, 0x1b, 0, 0x2e, 0, 0x1d, 0, 3, 0, 0x1c, 0, 0x2c, 0, 0x1e, 0, 0x2d, 0, 0x1f, 0, 0x1c, 0, 0x21, 0, 0x1d, 0, 0x20, 0, 0x2b, 0, 0x22, 0, 5, 0, 0x23, 0, 0x2a, 0, 0x25, 0, 4, 0, 0x24, 0, 0x29, 0, 0x26, 0, 0x15, 0, 0x27, 0, 0x28, 0, 0x29, 0, 0x14, 0, 0x28, 0, 0x27, 0, 0x2a, 0, 0x10, 0, 0x2b, 0, 0x26, 0, 0x2d, 0, 0x11, 0, 0x2c, 0, 0x25, 0, 0x2e, 0, 0x13, 0, 0x2f, 0, 0x24, 0, 0x31, 0, 0x12, 0, 0x30, 0, 0x19, 0, 0x32, 0, 0x22, 0, 0x33, 0, 0x18, 0, 0x35, 0, 0x23, 0, 0x34, 0, 0, 0, 0x36, 0, 0x20, 0, 0x37, 0, 1, 0, 0x39, 0, 0x21, 0, 0x38, 0, 0x1f, 0, 0x3a, 0, 0, 0, 0x36, 0, 0x1e, 0, 0x3b, 0, 1, 0, 0x39, 0, 0x20, 0, 0x37, 0, 0x19, 0, 0x32, 0, 0x21, 0, 0x38, 0, 0x18, 0, 0x35, 0, 0x22, 0, 0x33, 0, 0x16, 0, 0x3c, 0, 0x23, 0, 0x34, 0, 0x17, 0, 0x3d, 0, 0x16, 0, 0x3c, 0, 0x25, 0, 0x2e, 0, 0x17, 0, 0x3d, 0, 0x24, 0, 0x31, 0, 0x13, 0, 0x2f, 0, 0x27, 0, 0x2a, 0, 0x12, 0, 0x30, 0, 0x26, 0, 0x2d, 0, 0x10, 0, 0x2b, 0, 0x29, 0, 0x26, 0, 0x11, 0, 0x2c, 0, 0x28, 0, 0x29, 0, 0x15, 0, 0x27, 0, 0x2b, 0, 0x22, 0, 0x14, 0, 0x28, 0, 0x2a, 0, 0x25, 0, 4, 0, 0x24, 0, 5, 0, 0x23, 0, 0x2c, 0, 0x1e, 0, 0x2d, 0, 0x1f, 0, 0x1d, 0, 0x20, 0, 0x2f, 0, 0x1a, 0, 0x1c, 0, 0x21, 0, 0x2e, 0, 0x1d, 0, 2, 0, 0x1b, 0, 0x31, 0, 0x16, 0, 3, 0, 0x1c, 0, 0x30, 0, 0x19, 0, 0xf, 0, 0x17, 0, 0x33, 0, 0x12, 0, 0xe, 0, 0x18, 0, 0x32, 0, 0x15, 0, 0xc, 0, 0x13, 0, 0x35, 0, 0xe, 0, 0xd, 0, 0x14, 0, 0x34, 0, 0x11, 0, 9, 0, 0xf, 0, 0x37, 0, 0xa, 0, 8, 0, 0x10, 0, 0x36, 0, 0xd, 0, 6, 0, 0xb, 0, 0x39, 0, 6, 0, 7, 0, 0xc, 0, 0x38, 0, 9, 0, 0xb, 0, 8, 0, 0xa, 0, 7, 0, 0x3a, 0, 2, 0, 0x3b, 0, 3, 0, 0x1b, 0, 5, 0, 0x1a, 0, 4, 0, 0x3c, 0, 0, 0, 0x3d, 0, 1, 0, -64, -50, -67, 0x3e, -93, -113, 0x79, 0x3a, -27, 0x42, 0x21, 0x3f, 0x6a, -67, 0x5f, 0x3b, -113, -5, -122, 0x3e, -114, -111, -84, 0x3d, 0x1a, -120, 0x41, 0x3f, -83, -95, -76, 0x3d, 0x77, 0x4d, 0x34, 0x3f, -46, -113, -122, 0x3d, 0x7c, -71, -105, 0x3e, -10, 0x5c, -122, 0x3d, 0x2b, -7, 0x4c, 0x3f, 0x7e, 0x51, 0x12, 0x3e, 0x3c, -7, 0x4c, 0x3f, -17, 0x3b, -26, 0x3d, 0x26, -60, 0x4c, 0x3e, 0x3b, -31, -27, 0x3d, 0x26, -60, 0x4c, 0x3e, -120, 0x11, 0x12, 0x3e, 0xa, -8, 0x49, 0x3f, -30, -28, 0x4e, 0x3e, 0x2b, -7, 0x4c, 0x3f, -29, -62, 0x31, 0x3e, 0x26, -60, 0x4c, 0x3e, -47, 0x75, 0x31, 0x3e, -17, -56, 0x58, 0x3e, 0x4a, -75, 0x4f, 0x3e, 0xa, -8, 0x49, 0x3f, 0x41, 0x2c, -125, 0x3e, 0xa, -8, 0x49, 0x3f, 0x43, 0x1f, 0x6c, 0x3e, -17, -56, 0x58, 0x3e, 0x63, 0xc, 0x6c, 0x3e, -17, -56, 0x58, 0x3e, -109, -84, -125, 0x3e, -117, -58, 0x4a, 0x3f, -65, -74, -98, 0x3e, 0x32, -81, 0x4b, 0x3f, -107, -98, -111, 0x3e, 0x4e, -20, 0x51, 0x3e, -35, 0x40, -111, 0x3e, -88, -114, 0x55, 0x3e, -61, -44, -98, 0x3e, 0x18, 0x23, 0x56, 0x3f, 0x3b, -114, -73, 0x3e, -126, 0x1d, 0x4f, 0x3f, 0x21, -82, -84, 0x3e, -54, 0x32, 0x44, 0x3e, 0x4a, -49, -84, 0x3e, -76, 0x1c, 0x28, 0x3e, 0x36, -109, -73, 0x3e, 0x31, 0x43, 0x67, 0x3f, -64, -77, -51, 0x3e, -79, -6, 0x5f, 0x3f, -95, -13, -62, 0x3e, 0xe, -66, 0, 0x3e, 0x65, 0x6d, -61, 0x3e, 0x1d, 0x38, -57, 0x3d, -100, -88, -51, 0x3e, -28, -121, -54, 0x3c, 0x61, 0x19, -21, 0x3e, -11, -43, 0x79, 0x3f, -76, 0x73, -22, 0x3e, 0x71, -109, 0x71, 0x3f, 0x79, 0x59, -37, 0x3e, 0x40, 0x6c, 0x69, 0x3d, 0x34, -11, -38, 0x3e, -25, 0x39, 0x7a, 0x3f, -124, -14, 2, 0x3f, -16, 0x6e, 0x7d, 0x3f, -79, 0x6c, -10, 0x3e, 0x39, -47, 0x2e, 0x3c, 0x48, -89, -10, 0x3e, -69, 9, -66, 0x3c, 0x65, 0x37, 3, 0x3f, -29, -116, 0x75, 0x3f, -40, -41, 0x12, 0x3f, -22, 0x42, 0x78, 0x3f, 0x4e, -48, 0xa, 0x3f, 0x60, -23, -4, 0x3c, -43, -21, 0xa, 0x3f, 0x1b, -43, 0x29, 0x3d, -111, 0x5f, 0x13, 0x3f, -106, -105, 0x6c, 0x3f, 0x59, 0x6e, 0x25, 0x3f, -27, -105, 0x75, 0x3f, -35, -20, 0x1b, 0x3f, -14, 0x24, 0x29, 0x3d, 0x7e, 0x1d, 0x1c, 0x3f, -9, -108, -100, 0x3d, -9, 0x1f, 0x25, 0x3f, -12, -31, 0x61, 0x3f, 0x57, -80, 0x39, 0x3f, 0x71, -24, 0x65, 0x3f, -44, -16, 0x2d, 0x3f, 0x1f, 0xe, -46, 0x3d, 0xe, -9, 0x2d, 0x3f, 7, 0x42, -14, 0x3d, 0x77, -122, 0x39, 0x3f, -121, 0x4f, 0x5e, 0x3f, 0xf, -74, 0x58, 0x3f, -121, 0x4f, 0x5e, 0x3f, 0x77, 0x11, 0x4e, 0x3f, -74, 0x6a, 7, 0x3e, -81, 0x42, 0x4e, 0x3f, -7, 0x6a, 7, 0x3e, -89, -51, 0x58, 0x3f, -121, 0x4f, 0x5e, 0x3f, -95, -9, 0x6e, 0x3f, -121, 0x4f, 0x5e, 0x3f, -108, -11, 0x63, 0x3f, -7, 0x6a, 7, 0x3e, -76, 5, 0x64, 0x3f, -74, 0x6a, 7, 0x3e, -3, -38, 0x6e, 0x3f, -121, 0x4f, 0x5e, 0x3f, 0x5c, -114, 0x7f, 0x3f, -74, 0x6a, 7, 0x3e, -13, -25, 0x7f, 0x3f, 0x18, 0x7b, 0x5f, 0x3f, 0x6d, -114, 0x43, 0x3f, 0x73, -68, 2, 0x3e, -60, -21, 0x42, 0x3f, -120, -13, 0x2c, 0x3f, 0x53, 0x75, 0x3f, -66, -123, -50, 7, 0x3f, 0, 0, -128, 0x3f, 0x2f, 0x15, 0x1e, 0x40, 0x2a, 2, -68, -67, -80, -86, -10, 0x3e, 0, 0, -128, 0x3f, -95, -8, -111, -67, 0x55, -8, -37, 0x3e, 0x79, -50, -42, 0x3e, 0, 0, -128, 0x3f, -48, 0x64, 0x3b, -65, -77, 0x42, 0x4d, 0x3f, 0xc, -24, -67, 0x3e, 0, 0, -128, 0x3f, -33, -8, -38, -66, -115, -106, -5, -66, 0x14, -20, 0x17, 0x3f, 0, 0, -128, 0x3f, 0x11, -30, 2, -65, -117, 0x6c, 0x7b, -65, 0x1e, -88, 0x2b, 0x3f, 0, 0, -128, 0x3f, -42, 0x35, 0x6a, 0x3e, -99, 0x63, 0x18, -65, 3, -108, 0xa, 0x3f, 0, 0, -128, 0x3f, 0x64, 0x58, -107, 0x3f, -42, -88, -65, -65, 0x36, -85, -18, 0x3e, 0, 0, -128, 0x3f, 3, 0xb, 0, 0x3f, 0x45, 0xe, -79, 0x3e, -106, -47, -16, 0x3e, 0, 0, -128, 0x3f, 0x27, -35, -114, 0x3f, 0x76, -58, -53, 0x3f, 0x53, -78, -4, 0x3e, 0, 0, -128, 0x3f, -20, 0x17, 0x6c, 0x3d, -1, 0x76, -77, 0x3f, -77, -19, -28, 0x3e, 0, 0, -128, 0x3f, -6, -47, 0x70, 0x3e, 3, -52, -28, 0x3e, 0x4e, -20, -31, 0x3e, 0, 0, -128, 0x3f, 0xc, 0x58, -14, -67, -78, 0x47, 0x10, -65, 0xe, -95, 6, 0x3f, 0, 0, -128, 0x3f, 0x7a, 0x18, 0x1a, 0x3e, 0x30, -71, -75, -65, -17, 0x38, 0xd, 0x3f, 0, 0, -128, 0x3f, 0x40, 0x6a, 0xb, 0x3f, 7, 0x5c, -25, -66, 2, -125, 0xc, 0x3f, 0, 0, -128, 0x3f, 0x2e, 0x1a, 6, 0x40, -40, 0xb, -123, -65, 0x2e, -57, -13, 0x3e, 0, 0, -128, 0x3f, 0x36, 0x3e, 0x2b, 0x3f, -1, -77, -26, 0x3d, -4, -90, 0, 0x3f, 0, 0, -128, 0x3f, -67, -90, 7, 0x40, -96, -91, -119, 0x3f, 0, 0x57, -6, 0x3e, 0, 0, -128, 0x3f, 0x61, 0x33, -72, -66, -10, -21, -114, 0x3f, -12, -120, -47, 0x3e, 0, 0, -128, 0x3f, 0x70, -108, -68, 0x3d, 9, 0x1b, -26, 0x3e, -31, -18, -36, 0x3e, 0, 0, -128, 0x3f, -109, 0x6e, 0x13, 0x3f, 0x20, -102, -53, 0x3f, -67, -56, -12, 0x3e, 0, 0, -128, 0x3f, 0x74, -101, -64, 0x3e, 0x3a, 0x3c, -44, 0x3e, -8, -4, -24, 0x3e, 0, 0, -128, 0x3f, 0x5c, -112, -45, 0x3f, 0x7d, 0x25, -74, 0x3f, -96, 0x6d, -3, 0x3e, 0, 0, -128, 0x3f, 0x22, -88, 0x1a, 0x3f, -86, 0x2d, 0x75, 0x3e, -69, -14, -7, 0x3e, 0, 0, -128, 0x3f, 0x58, 0x70, 0x1b, 0x40, -24, 0x14, 0, 0x3f, 9, -7, -8, 0x3e, 0, 0, -128, 0x3f, 0x49, -10, 0x30, 0x3f, 0x77, 0x4d, 8, -67, -80, 0x57, 4, 0x3f, 0, 0, -128, 0x3f, -70, -97, 0x15, 0x40, -72, 0x78, 0x24, -65, 0x36, 0x1f, -9, 0x3e, 0, 0, -128, 0x3f, -53, 0x12, 0x21, 0x3f, -111, -47, -95, -66, 0x49, -70, 0xa, 0x3f, 0, 0, -128, 0x3f, -106, 0x5a, -43, 0x3f, 0x6f, -11, -86, -65, 3, -75, -16, 0x3e, 0, 0, -128, 0x3f, 0x2e, 0x71, -44, 0x3e, 0x2a, -3, 0xc, -65, 0x76, -63, 0xc, 0x3f, 0, 0, -128, 0x3f, 0x3d, 0x64, 0x1e, 0x3f, 0x73, -123, -59, -65, -106, 0x3d, -7, 0x3e, 0, 0, -128, 0x3f, 0x5b, 0x26, 3, 0x3d, -22, 0x40, 0x16, -65, 0x77, -93, 7, 0x3f, 0, 0, -128, 0x3f, 0x1f, -122, 0x66, -66, -62, 0x13, -100, -65, 0x32, 0x1e, 0x1d, 0x3f, 0, 0, -128, 0x3f, 0xf, 0x27, -120, -66, 0x43, 0x1b, 8, -65, -22, 0x3f, 7, 0x3f, 0, 0, -128, 0x3f, -62, -64, 0x13, 0x40, 0x37, -58, 0x4e, 0x3f, 0x52, -101, -8, 0x3e, 0, 0, -128, 0x3f, 0x25, -50, 0x2e, 0x3f, -23, -42, 0x2b, 0x3d, 6, -124, 2, 0x3f, 0, 0, -128, 0x3f, 0x10, -21, -13, 0x3f, 0x52, 0x29, -94, 0x3f, -3, 0x49, -4, 0x3e, 0, 0, -128, 0x3f, -91, -96, 0x23, 0x3f, 0x33, 0x70, 0x30, 0x3e, -28, -9, -2, 0x3e, 0, 0, -128, 0x3f, 0x75, 0x57, -80, 0x3f, 0xa, 0x4b, -62, 0x3f, 0x3d, -74, -3, 0x3e, 0, 0, -128, 0x3f, 0x43, -26, 0xe, 0x3f, 0x20, 0x7c, -104, 0x3e, 0x4c, -92, -12, 0x3e, 0, 0, -128, 0x3f, 0x60, -111, 0x5b, 0x3f, -18, 8, -49, 0x3f, 0x53, -78, -4, 0x3e, 0, 0, -128, 0x3f, -60, -79, -26, 0x3e, -89, 0x20, -65, 0x3e, 0xe, -64, -18, 0x3e, 0, 0, -128, 0x3f, -24, 0x2f, -108, 0x3e, 0x72, 0x53, -63, 0x3f, 0x2d, -105, -19, 0x3e, 0, 0, -128, 0x3f, -1, -110, -100, 0x3e, -80, -81, -35, 0x3e, -39, -21, -27, 0x3e, 0, 0, -128, 0x3f, -23, -13, 0xd, -65, 0x31, -47, 0x74, 0x3f, 0x27, -94, -57, 0x3e, 0, 0, -128, 0x3f, -57, 0x2b, -112, 0x3c, 0x6a, 0x17, -29, 0x3e, 0x1f, 0xe, -38, 0x3e, 0, 0, -128, 0x3f, 0x4b, -85, 0x21, -66, 0x54, 0x73, -95, 0x3f, -125, 0x6a, -37, 0x3e, 0, 0, -128, 0x3f, -103, -96, 0x26, 0x3e, -85, 0x59, -25, 0x3e, -42, -88, -33, 0x3e, 0, 0, -128, 0x3f, 7, -77, 0x1e, 0x40, 7, 0x44, 0x28, 0x3e, -52, 0x46, -9, 0x3e, 0, 0, -128, 0x3f, -96, 0x35, 0x2f, 0x3f, -61, -12, -35, -67, -41, -122, 6, 0x3f, 0, 0, -128, 0x3f, 0x41, -14, 0x1a, 0x40, 0x1e, -118, -54, -66, 2, -100, -10, 0x3e, 0, 0, -128, 0x3f, -89, 0x75, 0x27, 0x3f, -65, -41, -128, -66, 0x23, -122, 9, 0x3f, 0, 0, -128, 0x3f, -1, -49, 0xd, 0x40, 0xd, -62, 0x5c, -65, -123, 0x7b, -11, 0x3e, 0, 0, -128, 0x3f, 0x3c, 0x16, 0x17, 0x3f, 0x1a, -9, -58, -66, -47, -106, 0xb, 0x3f, 0, 0, -128, 0x3f, -45, 0x50, -15, 0x3f, 0x58, -55, -103, -65, -49, 0x2f, -14, 0x3e, 0, 0, -128, 0x3f, 0x25, -83, -8, 0x3e, -71, -114, 1, -65, 0x4f, -27, 0xc, 0x3f, 0, 0, -128, 0x3f, 9, -59, -76, 0x3f, -33, 0x50, -72, -65, -13, 3, -17, 0x3e, 0, 0, -128, 0x3f, 0x30, 0x2a, -87, 0x3e, 0x3b, 0x72, 0x14, -65, -79, -33, 0xb, 0x3f, 0, 0, -128, 0x3f, -54, 0x6b, 0x65, 0x3f, 0x2b, 0x12, -59, -65, 0x53, -106, -15, 0x3e, 0, 0, -128, 0x3f, 0x48, -60, 4, 0x3e, -66, 0x31, 0x18, -65, -95, 0x2f, 9, 0x3f, 0, 0, -128, 0x3f, 0x19, 0x20, -71, 0x3e, 0xe, 0x14, -64, -65, 0x77, 0x49, 4, 0x3f, 0, 0, -128, 0x3f, 0x6c, -106, 0x4b, -67, 0x44, 0x52, 0x13, -65, 0x66, 0x2f, 7, 0x3f, 0, 0, -128, 0x3f, 0x6c, 9, 0x79, -67, -63, -111, -88, -65, -105, 0x6f, 0x15, 0x3f, 0, 0, -128, 0x3f, 0x69, -27, 0x3e, -66, 0x75, -109, 0xc, -65, -112, -67, 6, 0x3f, 0, 0, -128, 0x3f, -25, 0x51, -79, -66, -31, 0x41, 3, -65, -58, -118, 0xa, 0x3f, 0, 0, -128, 0x3f, 0x7e, 0x1a, -57, -66, 9, -87, -117, -65, 0xf, -48, 0x25, 0x3f, 0, 0, -128, 0x3f, 8, 0, 0, 0, 9, 0, 1, 0, 0x29, 0, 3, 0, 0x28, 0, 2, 0, 0x34, 0, 4, 0, 0x35, 0, 5, 0, 0xf, 0, 7, 0, 0xe, 0, 6, 0, 0x3c, 0, 8, 0, 0x3d, 0, 9, 0, 0xd, 0, 0xb, 0, 0xc, 0, 0xa, 0, 0x10, 0, 0xc, 0, 0x11, 0, 0xd, 0, 0x25, 0, 0xf, 0, 0x24, 0, 0xe, 0, 0xb, 0, 0x10, 0, 0xa, 0, 0x11, 0, 0x2f, 0, 0x13, 0, 0x2e, 0, 0x12, 0, 0x20, 0, 0x14, 0, 0x21, 0, 0x15, 0, 0x41, 0, 0x17, 0, 0x40, 0, 0x16, 0, 0x38, 0, 0x18, 0, 0x39, 0, 0x19, 0, 7, 0, 0x1b, 0, 6, 0, 0x1a, 0, 0, 0, 0x1c, 0, 1, 0, 0x1d, 0, 0x31, 0, 0x1f, 0, 0x30, 0, 0x1e, 0, 0x13, 0, 0x20, 0, 0x12, 0, 0x21, 0, 0x2d, 0, 0x23, 0, 0x2c, 0, 0x22, 0, 0x15, 0, 0x24, 0, 0x14, 0, 0x25, 0, 0x2b, 0, 0x27, 0, 0x2a, 0, 0x26, 0, 0x17, 0, 0x28, 0, 0x16, 0, 0x29, 0, 0x27, 0, 0x2b, 0, 0x26, 0, 0x2a, 0, 0x19, 0, 0x2c, 0, 0x18, 0, 0x2d, 0, 0x23, 0, 0x2f, 0, 0x22, 0, 0x2e, 0, 0x32, 0, 0x30, 0, 0x33, 0, 0x31, 0, 0x1a, 0, 0x33, 0, 0x1b, 0, 0x32, 0, 0x36, 0, 0x34, 0, 0x37, 0, 0x35, 0, 0x1c, 0, 0x37, 0, 0x1d, 0, 0x36, 0, 0x3a, 0, 0x38, 0, 0x3b, 0, 0x39, 0, 0x1e, 0, 0x3b, 0, 0x1f, 0, 0x3a, 0, 0x3e, 0, 0x3c, 0, 0x3f, 0, 0x3d, 0, 0x20, 0, 0x14, 0, 0x21, 0, 0x15, 0, 0x23, 0, 0x2f, 0, 0x22, 0, 0x2e, 0, 0x10, 0, 0xc, 0, 0x11, 0, 0xd, 0, 0x25, 0, 0xf, 0, 0x24, 0, 0xe, 0, 0x17, 0, 0x28, 0, 0x16, 0, 0x29, 0, 0x27, 0, 0x2b, 0, 0x26, 0, 0x2a, 0, 8, 0, 0, 0, 9, 0, 1, 0, 0x29, 0, 3, 0, 0x28, 0, 2, 0, 0x15, 0, 0x24, 0, 0x14, 0, 0x25, 0, 0x2b, 0, 0x27, 0, 0x2a, 0, 0x26, 0, 0xb, 0, 0x10, 0, 0xa, 0, 0x11, 0, 0x2d, 0, 0x23, 0, 0x2c, 0, 0x22, 0, 2, 0, 0x3f, 0, 3, 0, 0x3e, 0, 0x2f, 0, 0x13, 0, 0x2e, 0, 0x12, 0, 0x13, 0, 0x20, 0, 0x12, 0, 0x21, 0, 0x31, 0, 0x1f, 0, 0x30, 0, 0x1e, 0, 0x19, 0, 0x2c, 0, 0x18, 0, 0x2d, 0, 1, 0, 0x1d, 0, 0, 0, 0x1c, 0, 0x32, 0, 0x30, 0, 0x33, 0, 0x31, 0, 0x1a, 0, 0x33, 0, 0x1b, 0, 0x32, 0, 0x34, 0, 4, 0, 0x35, 0, 5, 0, 0xf, 0, 7, 0, 0xe, 0, 6, 0, 0x36, 0, 0x34, 0, 0x37, 0, 0x35, 0, 0x1c, 0, 0x37, 0, 0x1d, 0, 0x36, 0, 0x38, 0, 0x18, 0, 0x39, 0, 0x19, 0, 7, 0, 0x1b, 0, 6, 0, 0x1a, 0, 0x3a, 0, 0x38, 0, 0x3b, 0, 0x39, 0, 0x1e, 0, 0x3b, 0, 0x1f, 0, 0x3a, 0, 0x3c, 0, 8, 0, 0x3d, 0, 9, 0, 0xd, 0, 0xb, 0, 0xc, 0, 0xa, 0, 0x3e, 0, 0x3c, 0, 0x3f, 0, 0x3d, 0, 0x41, 0, 0x17, 0, 0x40, 0, 0x16, 0, 5, 0, 0x41, 0, 4, 0, 0x40, 0, 0x52, 0x47, 0x5f, 0x3f, 0xb, -20, -127, 0x3e, 0x3e, -50, 4, 0x3e, 0xb, -20, -127, 0x3e, -96, -31, 0xd, 0x3e, -68, 0x77, 0x64, 0x3e, 0x69, 2, 0x5d, 0x3f, -68, 0x77, 0x64, 0x3e, -73, 0x61, 0x54, 0x3d, -1, 0x3e, 0x17, 0x3f, -75, 0x34, 0x73, 0x3f, -1, 0x3e, 0x17, 0x3f, 0x6c, 0x7a, 0x74, 0x3f, 0x16, 0x17, 0x1f, 0x3f, 0x57, 7, 0x40, 0x3d, 0x16, 0x17, 0x1f, 0x3f, 0x13, 0x7e, 0x29, 0x3c, -59, -1, 0x55, 0x3f, -40, -44, 0x7d, 0x3f, -59, -1, 0x55, 0x3f, -7, -123, 0x7b, 0x3f, -36, -41, 0x5d, 0x3f, 0xf, -99, -98, 0x3c, -36, -41, 0x5d, 0x3f, 0x72, 0x6b, 0x6a, 0x3f, -63, -84, -64, 0x3e, -14, 0x7a, -80, 0x3d, -63, -84, -64, 0x3e, -112, 0x32, -62, 0x3d, -109, -4, -80, 0x3e, -113, 0x34, 0x68, 0x3f, -109, -4, -80, 0x3e, 0x6c, -75, 0x4f, 0x3f, -84, 0x56, 6, 0x3e, -41, 0x15, 0x43, 0x3e, -84, 0x56, 6, 0x3e, -52, -45, 0x59, 0x3e, -94, -20, -51, 0x3d, -18, 5, 0x4a, 0x3f, -94, -20, -51, 0x3d, -11, 0x64, 0x7e, 0x3d, 9, -120, 0x6d, 0x3f, -110, -108, 0x70, 0x3f, 9, -120, 0x6d, 0x3f, -97, -108, 0x65, 0x3f, 0x20, 0x60, 0x75, 0x3f, -112, 0x31, -41, 0x3d, 0x20, 0x60, 0x75, 0x3f, -122, -112, 0x33, 0x3d, 0x6b, -97, 0x36, 0x3f, -56, 0x41, 0x75, 0x3f, 0x6b, -97, 0x36, 0x3f, 0x7b, -12, 0x76, 0x3f, -127, 0x77, 0x3e, 0x3f, 0x52, 0x65, 0x18, 0x3d, -127, 0x77, 0x3e, 0x3f, -10, -104, 0x70, 0x3f, 0x76, 0x6d, -1, 0x3e, -76, 0x1e, 0x7e, 0x3d, 0x76, 0x6d, -1, 0x3e, 0x4c, 0x50, -125, 0x3d, 0x49, -67, -17, 0x3e, -40, 0x10, 0x70, 0x3f, 0x49, -67, -17, 0x3e, 0x72, 0x51, 0x45, 0x3f, -19, 0x2b, -113, 0x3d, -66, -91, 0x6c, 0x3e, -19, 0x2b, -113, 0x3d, 0, 0x74, -128, 0x3e, 0x63, -43, 0x20, 0x3d, -31, 0x40, 0x40, 0x3f, 0x63, -43, 0x20, 0x3d, -63, 0x52, 0x59, 0x3f, 0x61, 0x17, 0x45, 0x3e, 0x7f, -96, 0x1c, 0x3e, 0x61, 0x17, 0x45, 0x3e, -121, -60, 0x2d, 0x3e, 7, -73, 0x25, 0x3e, -64, 9, 0x55, 0x3f, 7, -73, 0x25, 0x3e, 0x3c, -124, 0x65, 0x3f, 0x66, 0x4c, -95, 0x3e, 0x28, -75, -41, 0x3d, 0x66, 0x4c, -95, 0x3e, -126, -29, -14, 0x3d, 0x39, -100, -111, 0x3e, 0x71, 0x1e, 0x62, 0x3f, 0x39, -100, -111, 0x3e, 0x2d, 0x41, 0x6e, 0x3f, 0x1b, 0xd, -32, 0x3e, -93, -51, -111, 0x3d, 0x1b, 0xd, -32, 0x3e, -127, 9, -100, 0x3d, -18, 0x5c, -48, 0x3e, -96, -7, 0x6c, 0x3f, -18, 0x5c, -48, 0x3e, 0x4d, -42, 0x68, 0x3d, -46, -114, 7, 0x3f, 0x7c, -19, 0x71, 0x3f, -46, -114, 7, 0x3f, 0x60, 0x3d, 0x72, 0x3f, -24, 0x66, 0xf, 0x3f, 0x17, -40, 0x63, 0x3d, -24, 0x66, 0xf, 0x3f, 0x71, -25, 0x42, 0x3d, 0x3d, -17, 0x26, 0x3f, 0x59, 0x4c, 0x74, 0x3f, 0x3d, -17, 0x26, 0x3f, -83, -122, 0x74, 0x3f, 0x54, -57, 0x2e, 0x3f, 0x42, 0x43, 0x3f, 0x3d, 0x54, -57, 0x2e, 0x3f, 0x61, -4, -76, 0x3c, -104, 0x4f, 0x46, 0x3f, -19, -46, 0x7a, 0x3f, -104, 0x4f, 0x46, 0x3f, -54, -32, 0x7c, 0x3f, -81, 0x27, 0x4e, 0x3f, -86, -127, 0x66, 0x3c, -81, 0x27, 0x4e, 0x3f, -80, -26, 0, 0x3d, -13, -81, 0x65, 0x3f, 0x66, 0x6c, 0x78, 0x3f, -13, -81, 0x65, 0x3f, -116, -93, -118, 0x3e, -33, 0x4f, 0xd, 0x3c, 0x1c, 0x29, 0x3b, 0x3f, -33, 0x4f, 0xd, 0x3c, 0x41, -72, 0x5a, 0x3f, 0x37, 0x38, 0x7d, 0x3f, -127, 0xa, 0x17, 0x3e, 0x37, 0x38, 0x7d, 0x3f, 0x19, -112, -67, -68, -32, -66, -18, -66, -99, 0x13, 3, 0x3f, 0, 0, -128, 0x3f, -22, 0x42, -20, -67, 0x46, -106, -20, -66, 0, -57, 2, 0x3f, 0, 0, -128, 0x3f, 0x30, 0x65, -96, -67, 0x3a, 3, -73, -65, -96, 0x50, 0x17, 0x3f, 0, 0, -128, 0x3f, 0x1f, -14, -42, 0x3d, 0x1d, -110, -68, -65, 0x52, -100, 0x17, 0x3f, 0, 0, -128, 0x3f, 5, 0x31, 0x28, 0x3f, 0x36, -111, 9, 0x3e, 0x3b, 0x34, 0x18, 0x3f, 0, 0, -128, 0x3f, 0x53, 0x3c, 0x26, 0x3f, 0xc, -25, -102, 0x3d, 0x62, -95, 0x16, 0x3f, 0, 0, -128, 0x3f, 0x55, -34, 3, 0x40, 0x78, 0xe, 0xd, 0x3f, 0x5f, 0x24, 0x30, 0x3f, 0, 0, -128, 0x3f, 0xe, -98, -15, 0x3f, -48, -16, 0x3a, 0x3f, 0x33, 0x6d, 0x2f, 0x3f, 0, 0, -128, 0x3f, -99, 0x66, -63, 0x3e, 0x2e, -59, -43, 0x3e, 0x2f, -36, 0x19, 0x3f, 0, 0, -128, 0x3f, 0, -112, -29, 0x3e, 0x7b, -64, -52, 0x3e, -126, -87, 0x1a, 0x3f, 0, 0, -128, 0x3f, -25, 0x72, -117, 0x3e, 0x1c, -48, -100, 0x3f, -119, -73, 0x22, 0x3f, 0, 0, -128, 0x3f, 0x63, 0x27, 0x7c, 0x3d, -70, -124, -107, 0x3f, 0x14, 0x7b, 0x20, 0x3f, 0, 0, -128, 0x3f, -117, -1, 0x7b, 0x3d, 0x16, 0x69, -38, 0x3e, 0x2a, -88, 0x14, 0x3f, 0, 0, -128, 0x3f, -37, -89, 0x13, 0x3e, 0x2e, 0x1e, -34, 0x3e, -70, 0x2c, 0x16, 0x3f, 0, 0, -128, 0x3f, 0x2e, -58, -16, -66, -63, 0x55, 0x52, 0x3f, 0x33, -64, 0x19, 0x3f, 0, 0, -128, 0x3f, -35, 0x25, 0x1d, -65, -44, -98, 0x2a, 0x3f, -4, -116, 0x17, 0x3f, 0, 0, -128, 0x3f, 1, -4, -45, -67, 0x35, 0x40, -55, 0x3e, 0x4a, 0x62, 0x11, 0x3f, 0, 0, -128, 0x3f, -28, -98, -82, -68, 0x10, 0x79, -45, 0x3e, 0x52, 0xd, 0x13, 0x3f, 0, 0, -128, 0x3f, -65, 0x46, 0x3e, -65, 0x14, 7, 0, 0x3f, 0x7d, 0x5e, 0x15, 0x3f, 0, 0, -128, 0x3f, -101, 0x54, 0x5c, -65, 0x60, 0x1d, -89, 0x3e, -7, 0x30, 0x13, 0x3f, 0, 0, -128, 0x3f, -71, -32, 0x3c, -66, -9, -111, -69, 0x3e, 0x43, -85, 0xf, 0x3f, 0, 0, -128, 0x3f, -46, -33, 0x77, -65, 0x24, 0x26, 0x18, 0x3e, 0x43, -1, 0x10, 0x3f, 0, 0, -128, 0x3f, -33, -4, 0x66, 0x3e, -18, -108, -34, 0x3e, 0x25, -109, 0x17, 0x3f, 0, 0, -128, 0x3f, -72, -81, -101, 0x3e, 0x19, -56, -37, 0x3e, 0x52, -47, 0x18, 0x3f, 0, 0, -128, 0x3f, -117, 0x6f, 8, -66, 0x43, 4, -118, 0x3f, -92, 0x39, 0x1e, 0x3f, 0, 0, -128, 0x3f, 0x5e, -9, -98, -66, -61, -12, 0x75, 0x3f, 9, -6, 0x1b, 0x3f, 0, 0, -128, 0x3f, -68, 0x78, 0x17, 0x3f, 0x1b, 0x7f, -94, 0x3e, 0x11, 0x71, 0x1b, 0x3f, 0, 0, -128, 0x3f, 0x70, 0x5f, 0x1f, 0x3f, 4, 0x59, -113, 0x3e, -97, 0x1e, 0x1b, 0x3f, 0, 0, -128, 0x3f, 0x5f, 0x5f, -101, 0x3f, -13, 0x74, -114, 0x3f, -117, -4, 0x2a, 0x3f, 0, 0, -128, 0x3f, 0x1d, -54, 0x78, 0x3f, 0x11, -33, -105, 0x3f, 0x23, 0x12, 0x29, 0x3f, 0, 0, -128, 0x3f, -12, -88, 0, 0x3f, 0x1f, 0x13, -63, 0x3e, -100, 0x32, 0x1b, 0x3f, 0, 0, -128, 0x3f, 0x1e, 0x36, 0xd, 0x3f, 0x4c, -1, -78, 0x3e, -72, 0x75, 0x1b, 0x3f, 0, 0, -128, 0x3f, 0x6c, 0x3d, 0x3b, 0x3f, -117, -88, -99, 0x3f, -35, 8, 0x27, 0x3f, 0, 0, -128, 0x3f, -103, 0x47, -2, 0x3e, 0x74, 0x7d, -97, 0x3f, 0x53, -24, 0x24, 0x3f, 0, 0, -128, 0x3f, -59, -58, 0x24, 0x3f, -119, -104, 0x72, 0x3e, 0x4f, 0x77, 0x1a, 0x3f, 0, 0, -128, 0x3f, 0x39, -75, 0x27, 0x3f, -63, -57, 0x40, 0x3e, 0x39, 0x7d, 0x19, 0x3f, 0, 0, -128, 0x3f, -13, 0x1e, -41, 0x3f, 7, 0x5d, 0x62, 0x3f, -18, 0x3f, 0x2e, 0x3f, 0, 0, -128, 0x3f, -98, -17, -71, 0x3f, -108, -119, -127, 0x3f, -99, -68, 0x2c, 0x3f, 0, 0, -128, 0x3f, 0x50, -57, -37, 0x3e, 0x1a, -91, -101, -66, -128, 0xf, 0xa, 0x3f, 0, 0, -128, 0x3f, -2, -128, -57, 0x3e, 6, 0x10, -82, -66, -61, -69, 8, 0x3f, 0, 0, -128, 0x3f, 0x31, 0x40, -60, 0x3f, -41, -6, 0x7e, -65, 0x57, 0x23, 0x23, 0x3f, 0, 0, -128, 0x3f, -19, 0x29, -37, 0x3f, -19, -127, 0x56, -65, 0xc, -113, 0x25, 0x3f, 0, 0, -128, 0x3f, -82, 0x64, 0x13, 0x3f, 0x4b, -22, -28, -67, 0x42, -47, 0x10, 0x3f, 0, 0, -128, 0x3f, -81, -109, 0xa, 0x3f, -51, -82, 0x2b, -66, 0x4f, -21, 0xe, 0x3f, 0, 0, -128, 0x3f, -9, 0x72, 8, 0x40, -85, -49, -107, -66, -122, 0x55, 0x2c, 0x3f, 0, 0, -128, 0x3f, 0x5d, 0x6a, 0xd, 0x40, -72, -26, -82, -67, 0x7f, 0x2e, 0x2e, 0x3f, 0, 0, -128, 0x3f, -67, -30, 0x21, 0x3f, 0x52, 0x62, 0x57, 0x3c, 0x2d, -50, 0x14, 0x3f, 0, 0, -128, 0x3f, -44, 0x63, 0x1b, 0x3f, 0x65, -116, 0x4f, -67, 0x39, -47, 0x12, 0x3f, 0, 0, -128, 0x3f, -21, -30, 0xe, 0x40, 0x20, 0x43, 7, 0x3e, -110, -111, 0x2f, 0x3f, 0, 0, -128, 0x3f, 0x3c, -91, 0xb, 0x40, -78, -71, -78, 0x3e, -101, 0x3d, 0x30, 0x3f, 0, 0, -128, 0x3f, 0x37, 0x4f, 1, 0x3f, -61, 0x48, 0x5f, -66, 0x7c, 0x28, 0xd, 0x3f, 0, 0, -128, 0x3f, 0x6c, 0x77, -17, 0x3e, -120, -38, -122, -66, -75, -119, 0xb, 0x3f, 0, 0, -128, 0x3f, 1, -10, -17, 0x3f, -78, 0x12, 0x2b, -65, 7, -17, 0x27, 0x3f, 0, 0, -128, 0x3f, 0x1f, 0x13, 1, 0x40, 5, 0x34, -7, -66, 4, 0x37, 0x2a, 0x3f, 0, 0, -128, 0x3f, 0x1c, 0x41, -126, 0x3e, -51, -83, -40, -66, 0x37, -119, 5, 0x3f, 0, 0, -128, 0x3f, -9, -114, 0x4a, 0x3e, 0x18, -51, -30, -66, -35, -79, 4, 0x3f, 0, 0, -128, 0x3f, 0x41, -73, 0x37, 0x3f, -37, 0x15, -70, -65, -111, 0x7f, 0x1a, 0x3f, 0, 0, -128, 0x3f, -71, -6, 0x6d, 0x3f, 0x32, 0x3e, -80, -65, -57, 0x4a, 0x1c, 0x3f, 0, 0, -128, 0x3f, 0x6d, 0x55, -78, 0x3e, 0x3b, 0x3a, -66, -66, -4, -116, 7, 0x3f, 0, 0, -128, 0x3f, -10, -103, -101, 0x3e, 0x6b, 0x63, -52, -66, -86, 0x7d, 6, 0x3f, 0, 0, -128, 0x3f, -102, -78, -111, 0x3f, -35, -102, -94, -65, -91, 0x67, 0x1e, 0x3f, 0, 0, -128, 0x3f, 0xa, -97, -85, 0x3f, -16, 0x14, -110, -65, 0x30, -70, 0x20, 0x3f, 0, 0, -128, 0x3f, -60, 0x43, 8, 0x3e, 0x69, 0x1c, -22, -66, 0x7a, -1, 3, 0x3f, 0, 0, -128, 0x3f, 0x46, -74, 0x73, 0x3d, 0x7a, 0x1c, -18, -66, -93, 0x76, 3, 0x3f, 0, 0, -128, 0x3f, 0x7f, -62, -103, 0x3e, 0x7e, 0, -64, -65, -73, 0x27, 0x18, 0x3f, 0, 0, -128, 0x3f, 8, -108, 1, 0x3f, 0x16, 0x6a, -65, -65, -54, 0x1a, 0x19, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 1, 0, 1, 0, 3, 0, 3, 0, 2, 0, 2, 0, 4, 0, 4, 0, 5, 0, 5, 0, 7, 0, 7, 0, 6, 0, 6, 0, 8, 0, 8, 0, 9, 0, 9, 0, 0xb, 0, 0xb, 0, 0xa, 0, 0xa, 0, 0xc, 0, 0xc, 0, 0xd, 0, 0xd, 0, 0xf, 0, 0xf, 0, 0xe, 0, 0xe, 0, 0x10, 0, 0x10, 0, 0x11, 0, 0x11, 0, 0x13, 0, 0x13, 0, 0x12, 0, 0x12, 0, 0x14, 0, 0x14, 0, 0x10, 0, 0x10, 0, 0x15, 0, 0x15, 0, 0x13, 0, 0x13, 0, 0x11, 0, 0x11, 0, 0xc, 0, 0xc, 0, 0x12, 0, 0x12, 0, 0xf, 0, 0xf, 0, 0x16, 0, 0x16, 0, 0x17, 0, 0x17, 0, 0x19, 0, 0x19, 0, 0x18, 0, 0x18, 0, 0xd, 0, 0xd, 0, 0x16, 0, 0x16, 0, 0xe, 0, 0xe, 0, 0x19, 0, 0x19, 0, 0x17, 0, 0x17, 0, 8, 0, 8, 0, 0x18, 0, 0x18, 0, 0xb, 0, 0xb, 0, 0x1a, 0, 0x1a, 0, 0x1b, 0, 0x1b, 0, 0x1d, 0, 0x1d, 0, 0x1c, 0, 0x1c, 0, 0x1e, 0, 0x1e, 0, 0x1f, 0, 0x1f, 0, 0x21, 0, 0x21, 0, 0x20, 0, 0x20, 0, 9, 0, 9, 0, 0x1e, 0, 0x1e, 0, 0xa, 0, 0xa, 0, 0x21, 0, 0x21, 0, 0x1f, 0, 0x1f, 0, 0x1a, 0, 0x1a, 0, 0x20, 0, 0x20, 0, 0x1d, 0, 0x1d, 0, 0x22, 0, 0x22, 0, 0x23, 0, 0x23, 0, 0x25, 0, 0x25, 0, 0x24, 0, 0x24, 0, 0x1b, 0, 0x1b, 0, 0x22, 0, 0x22, 0, 0x1c, 0, 0x1c, 0, 0x25, 0, 0x25, 0, 0x23, 0, 0x23, 0, 4, 0, 4, 0, 0x24, 0, 0x24, 0, 7, 0, 7, 0, 0x26, 0, 0x26, 0, 0x27, 0, 0x27, 0, 0x29, 0, 0x29, 0, 0x28, 0, 0x28, 0, 0x2a, 0, 0x2a, 0, 0x2b, 0, 0x2b, 0, 0x2d, 0, 0x2d, 0, 0x2c, 0, 0x2c, 0, 0x2e, 0, 0x2e, 0, 0x2f, 0, 0x2f, 0, 0x31, 0, 0x31, 0, 0x30, 0, 0x30, 0, 5, 0, 5, 0, 0x2e, 0, 0x2e, 0, 6, 0, 6, 0, 0x31, 0, 0x31, 0, 0x2f, 0, 0x2f, 0, 0x2a, 0, 0x2a, 0, 0x30, 0, 0x30, 0, 0x2d, 0, 0x2d, 0, 0x32, 0, 0x32, 0, 0x33, 0, 0x33, 0, 0x35, 0, 0x35, 0, 0x34, 0, 0x34, 0, 0x2b, 0, 0x2b, 0, 0x32, 0, 0x32, 0, 0x2c, 0, 0x2c, 0, 0x35, 0, 0x35, 0, 0x33, 0, 0x33, 0, 0x26, 0, 0x26, 0, 0x34, 0, 0x34, 0, 0x29, 0, 0x29, 0, 0x36, 0, 0x36, 0, 0x37, 0, 0x37, 0, 0x39, 0, 0x39, 0, 0x38, 0, 0x38, 0, 0x3a, 0, 0x3a, 0, 0x3b, 0, 0x3b, 0, 0x3d, 0, 0x3d, 0, 0x3c, 0, 0x3c, 0, 0x27, 0, 0x27, 0, 0x3a, 0, 0x3a, 0, 0x28, 0, 0x28, 0, 0x3d, 0, 0x3d, 0, 0x3b, 0, 0x3b, 0, 0x36, 0, 0x36, 0, 0x3c, 0, 0x3c, 0, 0x39, 0, 0x39, 0, 0x3e, 0, 0x3e, 0, 0x3f, 0, 0x3f, 0, 0x41, 0, 0x41, 0, 0x40, 0, 0x40, 0, 0x37, 0, 0x37, 0, 0x3e, 0, 0x3e, 0, 0x38, 0, 0x38, 0, 0x41, 0, 0x41, 0, 0x3f, 0, 0x3f, 0, 0, 0, 0, 0, 0x40, 0, 0x40, 0, 3, 0, 3, 0, 0x56, -128, 0x77, 0x3f, 0x31, 0x7d, 0x6f, 0x3f, 0x3d, 0x61, 0x75, 0x3f, 0x78, -44, 0x7c, 0x3f, 0x2d, -20, 0x29, 0x3d, 0x78, -44, 0x7c, 0x3f, -96, -6, 7, 0x3d, 0x31, 0x7d, 0x6f, 0x3f, 0x64, 0x3d, 0x69, 0x3f, 0x3c, 0x4b, -40, 0x3e, -98, 0x45, 0x6f, 0x3f, -3, 0x4a, -25, 0x3e, 0x13, -45, -123, 0x3d, -3, 0x4a, -25, 0x3e, -32, 0x14, -74, 0x3d, 0x3c, 0x4b, -40, 0x3e, 0x32, 0x56, 0x47, 0x3f, -53, -71, 0x54, 0x3e, -107, -42, 0x4b, 0x3f, -82, 0x11, 0x71, 0x3e, 0x6a, -91, 0x50, 0x3e, -82, 0x11, 0x71, 0x3e, 0x37, -89, 0x62, 0x3e, -53, -71, 0x54, 0x3e, -70, -39, 0x2f, 0x3f, -89, -50, -61, 0x3d, 0x4d, -70, 0x35, 0x3f, -84, -60, -4, 0x3d, 0x44, -117, -108, 0x3e, -84, -60, -4, 0x3d, -116, 0x4c, -96, 0x3e, -89, -50, -61, 0x3d, -86, -16, 0x23, 0x3f, -30, 1, 0x25, 0x3d, 0x4b, 4, 0x2a, 0x3f, -40, 0x28, -117, 0x3d, 0x6a, -9, -85, 0x3e, -40, 0x28, -117, 0x3d, -84, 0x1e, -72, 0x3e, -30, 1, 0x25, 0x3d, -112, -35, 0x1d, 0x3f, 0x11, -30, 0x4a, 0x3c, -31, 0x44, -60, 0x3e, 0x11, -30, 0x4a, 0x3c, -54, -35, 0x3b, 0x3f, 5, 0x17, 0x1b, 0x3e, 8, -27, 0x41, 0x3f, -73, -15, 0x37, 0x3e, -33, 0x6b, 0x78, 0x3e, -73, -15, 0x37, 0x3e, 0x6b, 0x44, -120, 0x3e, 5, 0x17, 0x1b, 0x3e, -41, -63, 0x55, 0x3f, -91, -8, -96, 0x3e, -74, -40, 0x59, 0x3f, 0x4d, 0x49, -82, 0x3e, 0x28, -99, 0x18, 0x3e, 0x4d, 0x49, -82, 0x3e, -91, -8, 0x28, 0x3e, -91, -8, -96, 0x3e, -123, 0x5e, 0x4f, 0x3f, -92, 0x50, -122, 0x3e, 0x28, 0x46, 0x52, 0x3f, 0x6b, -71, -109, 0x3e, 0x60, -25, 0x36, 0x3e, 0x6b, -71, -109, 0x3e, -20, -123, 0x42, 0x3e, -92, 0x50, -122, 0x3e, 0x5d, -32, 0x5e, 0x3f, -125, -34, -69, 0x3e, -71, -62, 0x63, 0x3f, -36, -41, -55, 0x3e, -81, -23, -31, 0x3d, -36, -41, -55, 0x3e, -115, 0x7e, 4, 0x3e, -125, -34, -69, 0x3e, 0xb, -20, 0x7d, 0x3f, -124, -126, 0x26, 0x3f, 0x2d, 0xb, 0x7e, 0x3f, 0x2d, 0x42, 0x2d, 0x3f, -66, 0x69, -6, 0x3b, 0x2d, 0x42, 0x2d, 0x3f, 0x2a, -3, 4, 0x3c, -124, -126, 0x26, 0x3f, 0x55, 0x14, 0x7b, 0x3f, 0x1f, -10, 0xa, 0x3f, -38, -113, 0x7c, 0x3f, 0x60, 0x39, 0x12, 0x3f, 0x4f, 5, 0x5c, 0x3c, 0x60, 0x39, 0x12, 0x3f, 0x5c, 0x75, -99, 0x3c, 0x1f, -10, 0xa, 0x3f, 0x46, 0x22, 0x74, 0x3f, 0x1b, -44, -10, 0x3e, -11, 0x47, 0x78, 0x3f, 0x2b, 0x4c, 3, 0x3f, 0x54, 1, -9, 0x3c, 0x2b, 0x4c, 3, 0x3f, -104, -37, 0x3d, 0x3d, 0x1b, -44, -10, 0x3e, -75, -93, 0x7c, 0x3f, -98, 0x24, 0x19, 0x3f, -77, 0x5d, 0x7d, 0x3f, 0x5e, -37, 0x1f, 0x3f, 0x32, -109, 0x28, 0x3c, 0x5e, -37, 0x1f, 0x3f, -95, 0x12, 0x57, 0x3c, -98, 0x24, 0x19, 0x3f, 0x34, 0x2d, 0x7d, 0x3f, -103, -16, 0x43, 0x3f, -58, -89, 0x7c, 0x3f, -104, 0x31, 0x4d, 0x3f, -107, 0xe, 0x56, 0x3c, -104, 0x31, 0x4d, 0x3f, -5, -78, 0x34, 0x3c, -103, -16, 0x43, 0x3f, 0x5a, 0x2f, 0x7e, 0x3f, -39, 0x42, 0x34, 0x3f, -13, 0x3b, 0x7d, 0x3f, -23, -73, 0x3b, 0x3f, 0x43, 3, 0x31, 0x3c, -23, -73, 0x3b, 0x3f, 0x3b, 0x53, -24, 0x3b, -39, 0x42, 0x34, 0x3f, -45, -68, 0x7b, 0x3f, 0x11, -112, 0x57, 0x3f, -104, 0x6a, 0x7a, 0x3f, -119, 8, 0x63, 0x3f, -5, -84, -78, 0x3c, -119, 8, 0x63, 0x3f, -107, 0x65, -120, 0x3c, 0x11, -112, 0x57, 0x3f, -107, -127, -21, -66, -64, -22, 0x68, 0x3e, 0x72, -33, -24, 0x3f, 0, 0, -128, 0x3f, 0x4f, -79, 0x3a, -66, 9, -88, -128, 0x3e, -118, -113, -17, 0x3f, 0, 0, -128, 0x3f, -112, -37, -121, -66, 4, 0x73, -68, 0x3e, -70, 0x68, 0x15, 0x40, 0, 0, -128, 0x3f, 0x70, 0x77, 0x2e, -65, -97, -24, -70, 0x3e, 0x58, 0x56, 0xf, 0x40, 0, 0, -128, 0x3f, 0x3b, 0x35, -41, 0x3d, -49, -10, -120, 0x3e, 9, -3, -18, 0x3f, 0, 0, -128, 0x3f, -105, -60, 0x19, 0x3e, 0x4d, -42, -72, 0x3e, 0xa, -71, 0x15, 0x40, 0, 0, -128, 0x3f, 0x61, 0x35, -58, 0x3e, -21, 0x6e, -122, 0x3e, -100, -36, -27, 0x3f, 0, 0, -128, 0x3f, 0x49, -122, 0xc, 0x3f, -111, 0x2c, -80, 0x3e, -110, -110, 0xf, 0x40, 0, 0, -128, 0x3f, -47, -53, 0x24, 0x3f, 0x15, 0x1a, 0x78, 0x3e, -26, 0x3f, -44, 0x3f, 0, 0, -128, 0x3f, -65, 0x61, 0x6a, 0x3f, 0x38, 0x32, -97, 0x3e, -36, -73, 2, 0x40, 0, 0, -128, 0x3f, -46, -3, 0x5c, 0x3f, -36, -15, 0x56, 0x3e, 0x6d, -57, -66, 0x3f, 0, 0, -128, 0x3f, 0x34, 0x4b, -100, 0x3f, 0x3c, -63, -122, 0x3e, -66, -6, -28, 0x3f, 0, 0, -128, 0x3f, 0xd, 0x17, -123, -65, -122, 2, -74, 0x3e, -128, 0x11, 6, 0x40, 0, 0, -128, 0x3f, 0x13, 0x64, 0x34, -65, 0x34, -40, 0x44, 0x3e, -26, 0x74, -35, 0x3f, 0, 0, -128, 0x3f, 0x5b, 0x23, -84, -65, -126, 0x1a, -82, 0x3e, 0x1b, -44, -18, 0x3f, 0, 0, -128, 0x3f, -61, -40, 0x6a, -65, 0x3a, 0x77, 0x1b, 0x3e, 0x56, 0x4a, -53, 0x3f, 0, 0, -128, 0x3f, 0x4c, 0x37, -49, -65, -83, -119, -91, 0x3e, 0x7c, -18, -62, 0x3f, 0, 0, -128, 0x3f, -51, -27, -116, -65, 0x6a, -120, -22, 0x3d, -40, 0x61, -80, 0x3f, 0, 0, -128, 0x3f, -113, 0x53, -34, -65, 0x63, 0x60, -99, 0x3e, 0x3e, 0x42, -113, 0x3f, 0, 0, -128, 0x3f, -17, 0x73, -108, -65, -42, 0x35, -38, 0x3d, -71, -117, -106, 0x3f, 0, 0, -128, 0x3f, 0x52, -17, -39, -65, 0x25, -106, -108, 0x3e, -70, -7, 0x2e, 0x3f, 0, 0, -128, 0x3f, -52, 0x41, -110, -65, 0x5a, -95, -56, 0x3d, 9, 0x52, 0x75, 0x3f, 0, 0, -128, 0x3f, -13, -29, -121, 0x3f, 0x27, -6, 0x2c, 0x3e, 4, 0x5a, -90, 0x3f, 0, 0, -128, 0x3f, 0x2a, 0x37, -67, 0x3f, 0x46, -78, 0x57, 0x3e, -118, 0x5b, -63, 0x3f, 0, 0, -128, 0x3f, 0x65, -115, -90, 0x3f, -65, -47, -50, 0x3d, 0x24, -105, 0x7f, 0x3f, 0, 0, -128, 0x3f, -14, 0x60, -29, 0x3f, -72, 0x1e, 0x15, 0x3e, -9, -24, -109, 0x3f, 0, 0, -128, 0x3f, 0x67, 0xf, -68, 0x3f, -22, -50, 0x13, 0x3d, -43, 0x21, 0x2b, 0x3f, 0, 0, -128, 0x3f, -52, 0x28, 3, 0x40, 0x18, 0xa, -104, 0x3d, 0x24, -72, 0x49, 0x3f, 0, 0, -128, 0x3f, 0x15, -31, -54, 0x3f, -25, 0x6e, -41, -68, -95, 0x4a, -83, 0x3e, 0, 0, -128, 0x3f, -9, 0x3b, 0x13, 0x40, -90, -72, -86, 0x3b, -117, 0x4e, -26, 0x3e, 0, 0, -128, 0x3f, 0x23, 0x68, -12, -66, 0x43, 0x1a, 0x15, -67, 0x26, 0x56, -88, 0x3f, 0, 0, -128, 0x3f, 0x5a, 0x29, 0xc, -65, -33, 0x15, -63, -67, 0x53, 9, -95, 0x3f, 0, 0, -128, 0x3f, 0x16, 0x34, -67, -66, -30, 0x5a, -19, 0x3c, -59, -58, -82, 0x3f, 0, 0, -128, 0x3f, -111, 0x28, 0x74, -66, 0xb, 9, -72, 0x3d, 0x34, 0x12, -77, 0x3f, 0, 0, -128, 0x3f, -4, 0x56, -53, -67, 0x1b, -70, 9, 0x3e, -95, 0x4d, -76, 0x3f, 0, 0, -128, 0x3f, -113, -62, 0x75, 0x3d, 0x5e, 0x2e, 0x32, 0x3e, -3, -121, -78, 0x3f, 0, 0, -128, 0x3f, 0x1f, -68, 0x66, 0x3e, -48, 0x62, 0x39, 0x3e, 0x14, -108, -84, 0x3f, 0, 0, -128, 0x3f, -24, 0x6b, -66, 0x3e, -68, -49, 0x31, 0x3e, 0x1b, 0x10, -93, 0x3f, 0, 0, -128, 0x3f, 0x4d, 0x65, 1, 0x3f, -124, 0x61, 0x20, 0x3e, 0x25, -108, -104, 0x3f, 0, 0, -128, 0x3f, 0x79, 0x21, 0x25, 0x3f, 7, 0x42, 2, 0x3e, -122, 0x58, -117, 0x3f, 0, 0, -128, 0x3f, -97, 0x73, 0x53, 0x3f, 0x1d, -52, 0x66, 0x3d, 0x6c, 0x5c, 0x57, 0x3f, 0, 0, -128, 0x3f, 0x5b, -102, 0x63, 0x3f, -13, 0x54, 7, -69, 0x76, -117, 0xc, 0x3f, 0, 0, -128, 0x3f, 0x68, -108, 0x5e, 0x3f, -4, -59, 0x6c, -67, -80, -115, 0x68, 0x3e, 0, 0, -128, 0x3f, -52, -72, 0x31, -65, -49, 0x2c, -119, -67, 0x7b, 0x67, -106, 0x3f, 0, 0, -128, 0x3f, -16, -123, 0x15, -65, -89, -20, -12, -67, 0, 0x73, -99, 0x3f, 0, 0, -128, 0x3f, 0, 0, 0, 0, 1, 0, 1, 0, 3, 0, 3, 0, 2, 0, 2, 0, 1, 0, 1, 0, 4, 0, 4, 0, 2, 0, 2, 0, 5, 0, 5, 0, 4, 0, 4, 0, 6, 0, 6, 0, 5, 0, 5, 0, 7, 0, 7, 0, 6, 0, 6, 0, 8, 0, 8, 0, 7, 0, 7, 0, 9, 0, 9, 0, 8, 0, 8, 0, 0xa, 0, 0xa, 0, 9, 0, 9, 0, 0xb, 0, 0xb, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0xd, 0, 0xd, 0, 0xc, 0, 0xc, 0, 0xd, 0, 0xd, 0, 0xc, 0, 0xc, 0, 0xf, 0, 0xf, 0, 0xe, 0, 0xe, 0, 0xf, 0, 0xf, 0, 0xe, 0, 0xe, 0, 0x11, 0, 0x11, 0, 0x10, 0, 0x10, 0, 0x11, 0, 0x11, 0, 0x10, 0, 0x10, 0, 0x13, 0, 0x13, 0, 0x12, 0, 0x12, 0, 0x13, 0, 0x13, 0, 0x12, 0, 0x12, 0, 0x15, 0, 0x15, 0, 0x14, 0, 0x14, 0, 0xb, 0, 0xb, 0, 0xa, 0, 0xa, 0, 0x17, 0, 0x17, 0, 0x16, 0, 0x16, 0, 0x17, 0, 0x17, 0, 0x16, 0, 0x16, 0, 0x19, 0, 0x19, 0, 0x18, 0, 0x18, 0, 0x19, 0, 0x19, 0, 0x18, 0, 0x18, 0, 0x1b, 0, 0x1b, 0, 0x1a, 0, 0x1a, 0, 0x1b, 0, 0x1b, 0, 0x1a, 0, 0x1a, 0, 0x1d, 0, 0x1d, 0, 0x1c, 0, 0x1c, 0, 0x1e, 0, 0x1e, 0, 0xf, 0, 0xf, 0, 0x1f, 0, 0x1f, 0, 0x11, 0, 0x11, 0, 0x20, 0, 0x20, 0, 0xd, 0, 0xd, 0, 0x1e, 0, 0x1e, 0, 0xf, 0, 0xf, 0, 0x21, 0, 0x21, 0, 0, 0, 0, 0, 0x20, 0, 0x20, 0, 0xd, 0, 0xd, 0, 0x21, 0, 0x21, 0, 0x22, 0, 0x22, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0x22, 0, 0x22, 0, 0x23, 0, 0x23, 0, 1, 0, 1, 0, 4, 0, 4, 0, 0x23, 0, 0x23, 0, 0x24, 0, 0x24, 0, 4, 0, 4, 0, 6, 0, 6, 0, 0x24, 0, 0x24, 0, 0x25, 0, 0x25, 0, 6, 0, 6, 0, 8, 0, 8, 0, 0x25, 0, 0x25, 0, 0x26, 0, 0x26, 0, 8, 0, 8, 0, 0xa, 0, 0xa, 0, 0x16, 0, 0x16, 0, 0xa, 0, 0xa, 0, 0x27, 0, 0x27, 0, 0x26, 0, 0x26, 0, 0x18, 0, 0x18, 0, 0x16, 0, 0x16, 0, 0x28, 0, 0x28, 0, 0x27, 0, 0x27, 0, 0x1a, 0, 0x1a, 0, 0x18, 0, 0x18, 0, 0x29, 0, 0x29, 0, 0x28, 0, 0x28, 0, 0x1c, 0, 0x1c, 0, 0x1a, 0, 0x1a, 0, 0x2a, 0, 0x2a, 0, 0x29, 0, 0x29, 0, 0x13, 0, 0x13, 0, 0x15, 0, 0x15, 0, 0x2c, 0, 0x2c, 0, 0x2b, 0, 0x2b, 0, 0x11, 0, 0x11, 0, 0x13, 0, 0x13, 0, 0x1f, 0, 0x1f, 0, 0x2c, 0, 0x2c, 0, 0, 0, 0, 0x3f, -82, 0x49, -73, 0x3e, 0, 0, 0, 0x3f, 0x75, -81, -37, 0x3e, 0xc, 0x1e, -128, 0x3f, 0x75, -81, -37, 0x3e, 0xc, 0x1e, -128, 0x3f, -82, 0x49, -73, 0x3e, 0, 0, 0, 0x3f, -98, 0xa, 0, 0x3f, 0xc, 0x1e, -128, 0x3f, -115, 0xa, 0, 0x3f, 0, 0, 0, 0x3f, -127, 0x3d, 0x12, 0x3f, 0xc, 0x1e, -128, 0x3f, -127, 0x3d, 0x12, 0x3f, 0, 0, 0, 0x3f, 0x66, -94, 0x24, 0x3f, 0xc, 0x1e, -128, 0x3f, 0x54, 0x70, 0x24, 0x3f, 0, 0, 0, 0x3f, 0x48, -93, 0x36, 0x3f, 0xc, 0x1e, -128, 0x3f, 0x48, -93, 0x36, 0x3f, 0xc, 0x1e, -128, 0x3f, -25, -29, -110, 0x3e, 0, 0, 0, 0x3f, -25, -29, -110, 0x3e, 0xc, 0x1e, -128, 0x3f, 0x40, -4, 0x5c, 0x3e, 0, 0, 0, 0x3f, 0x40, -4, 0x5c, 0x3e, 0xc, 0x1e, -128, 0x3f, -79, 0x30, 0x14, 0x3e, 0, 0, 0, 0x3f, -79, 0x30, 0x14, 0x3e, 0xc, 0x1e, -128, 0x3f, 0x47, -54, -106, 0x3d, 0x11, 0, 0, 0x3f, 0x3a, 0x3c, -60, 0x3d, 0xc, 0x1e, -128, 0x3f, -109, 0x54, 0x26, 0x3b, -67, -1, -1, 0x3e, -6, 0x2a, 0x39, 0x3d, 0, 0, 0, 0x3f, 0x2c, -42, 0x48, 0x3f, 0xc, 0x1e, -128, 0x3f, 0x2c, -42, 0x48, 0x3f, 0, 0, 0, 0x3f, 0xf, 9, 0x5b, 0x3f, 0xc, 0x1e, -128, 0x3f, 0xf, 9, 0x5b, 0x3f, 0, 0, 0, 0x3f, -13, 0x3b, 0x6d, 0x3f, -66, -122, -122, 0x3f, -13, 0x3b, 0x6d, 0x3f, 0, 0, 0, 0x3f, -42, 0x6e, 0x7f, 0x3f, 0x1a, 0x6b, -113, 0x3f, -42, 0x6e, 0x7f, 0x3f, -16, -91, 0x70, -70, 0x40, -4, 0x5c, 0x3e, -16, -91, 0x70, -70, 0x6e, 0x30, 0x14, 0x3e, -16, -91, 0x70, -70, -25, -29, -110, 0x3e, -16, -91, 0x70, -70, -82, 0x49, -73, 0x3e, -16, -91, 0x70, -70, 0x75, -81, -37, 0x3e, -16, -91, 0x70, -70, -98, 0xa, 0, 0x3f, -16, -91, 0x70, -70, -127, 0x3d, 0x12, 0x3f, -16, -91, 0x70, -70, 0x67, -44, 0x24, 0x3f, -16, -91, 0x70, -70, 0x48, -93, 0x36, 0x3f, -16, -91, 0x70, -70, 0x2c, -42, 0x48, 0x3f, -16, -91, 0x70, -70, 0xf, 9, 0x5b, 0x3f, -65, -41, 0x50, -67, -13, 0x3b, 0x6d, 0x3f, 0x1c, -79, -10, -67, -42, 0x6e, 0x7f, 0x3f, -102, -104, -50, 0x3d, 0x22, 0x53, -34, 0x3d, -79, -63, 0x42, 0x3c, -85, -76, 5, 0x3e};
extern float D_L14_0015F580_x[] __asm__("D_L14_0015F580") __attribute__((section(".sdata")));
extern PuffParticle *spawn_puff(void *, float, float, float, int, void *, float,
                                int) __asm__("FUN_L00_0026d000");

void FUN_L14_002b5968(struct Moby *moby, float *point, float angle) {
    float offset[4];
    float pos[4];
    float small_offset[4];
    float direction[4];
    float original_pos[4];
    float value;
    float puff_size;
    float step;
    PuffParticle *part;
    PuffParticleTail *tail;
    int i;
    int j;
    int n;
    int color;
    int packed_color;
    int low_color;
    int high_color;

    value = fast_add_rotations(angle, moby->rot.z);
    offset[0] = fast_cos(value) * 0.8f;
    offset[1] = fast_sin(value) * 0.8f;
    offset[2] = 0.0f;
    add_vector_xyz(pos, point, offset);
    for (i = 5; i >= 0; i--) {
        n = random_integer_below(2);
        if (random_integer_below(2)) n = -n;
        puff_size = random_float_between(40000.0f, 150000.0f);
        color = random_int_between(10, 0x7f);
        low_color = (color << 8) | 0x7f000000;
        high_color = color << 16;
        packed_color = color | (high_color | low_color);
        part = spawn_puff(pos, 0.2f, 1.0f, 1.01f, n, D_L14_0015F580_x, puff_size, packed_color);
        if (part) {
            tail = &part->tail;
            part->life = scale_game_frames(120);
            tail->w4 = 2;
            tail->bA = 0x7f;
            tail->bB = part->life;
        }
    }
    qcopy(small_offset, offset);
    scale_vector_xyz(offset, offset, 0.0625f);
    offset[2] = random_float_between(0.025f, 0.06f);
    qcopy(original_pos, pos);
    step = 0.6283f;
    for (j = 0; j < 16; j++) {
        value = randf_sym(0.0f, 0.5f);
        build_look_at_matrix(direction, offset, small_offset, (float)j * step + value);
        pos[0] = original_pos[0] + randf_sym(0.0f, 0.2f);
        pos[1] = original_pos[1] + randf_sym(0.0f, 0.2f);
        pos[2] = original_pos[2] + randf_sym(0.0f, 0.2f);
        FUN_L00_0026c6e8(pos, direction);
    }
    FUN_L00_002d3838((int)D_L14_001D88D0, original_pos, 0, 0);
}
/* Points the mover at the second-to-last node of its active path and clears its offset. */

typedef struct {
    char pad00[0xD0];
    int pathA;
    char padD4[0xE4 - 0xD4];
    int t;
    int node;
    float w;
    float off[4];
    char pad100[0x10C - 0x100];
    int alt;
    char pad110[0x150 - 0x110];
    int pathB;
} Mover_5c30;

extern int *D_L14_001B0BB0[];
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

void FUN_L14_002b5c30(struct Moby *moby) {
    Mover_5c30 *d = (Mover_5c30 *)moby->pvars;
    int *p;
    int n;
    if (d->alt == 0)
        p = D_L14_001B0BB0[d->pathA];
    else
        p = D_L14_001B0BB0[d->pathB];
    n = *p - 2;
    d->node = n;
    d->w = *(float *)((n << 4) + (char *)p + 0x1C);
    d->t = 0;
    clear_u64_value(d->off);
}
extern u8 D_0013E550[];
extern void release_voice_slot(s32 idx) __asm__("FUN_0022d798");

/* 0x002B5C98, 124 bytes.  Clears a moby's link to its slot entry, releasing
 * the slot if it still points back here.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002B6EE8), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002b5c98(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    s32 idx;
    *(u16 *)(moby + 0x34) |= 0x41;
    *(s32 *)(moby + 0x94) = 0;
    idx = *(s32 *)(data + 0x108);
    if (idx != -1) {
        u8 *e = D_0013E550 + idx * 0x70;
        if (*(u8 **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0) {
            release_voice_slot(idx);
        }
    }
    *(s32 *)(data + 0x108) = -1;
}
/* Resets the moby's path follow state and faces it along its current path. */

extern int *D_L14_001B0BB0[];
extern float D_L14_00161540 __attribute__((sda));
extern void FUN_001f9a28(void *, void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

void FUN_L14_002b5d18(struct Moby *moby) {
    float f = D_L14_00161540 * 1.5f;
    char *d = (char *)moby->pvars;
    char *p;
    float v[4] __attribute__((aligned(16)));
    *(int *)(d + 0xE8) = 0;
    *(float *)(d + 0x160) = f;
    *(int *)(d + 0xEC) = 0;
    *(int *)(d + 0x164) = 0;
    *(int *)(d + 0x168) = 0;
    *(int *)(d + 0x16C) = 0;
    *(int *)(d + 0xC0) = 0;
    *(int *)(d + 0xC4) = 0;
    if (*(int *)(d + 0x10C) == 0)
        p = (char *)D_L14_001B0BB0[*(int *)(d + 0xD0)];
    else
        p = (char *)D_L14_001B0BB0[*(int *)(d + 0x150)];
    qcopy(&moby->pos, p + 0x10);
    FUN_001f9a28(v, p + 0x20, &moby->pos);
    moby->rot.z = fast_arc_tan(v[0], v[1]);
    *(int *)(d + 0xE4) = 0;
    clear_u64_value(d + 0xF0);
}
/* Exhaust puffs: two red sparks and three white puffs of shrinking size behind the moby. */
extern int below_x(int) __asm__("FUN_00213260");
extern int frames_x(int) __asm__("FUN_001f96f8");
extern void scale_x(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_x(void *, void *, void *) __asm__("FUN_001f9a10");
extern float D_L14_0015F580_x[] __asm__("D_L14_0015F580") __attribute__((section(".sdata")));

void FUN_L14_002b5dd8(struct Moby *m) {
    float off[4];
    float pos[4];
    int i;
    int j;
    int n;
    int life;
    float size;
    if (m->state < 2 || m->state == 9)
        return;
    scale_x(off, &m->rot_row0, -1.25f);
    add_x(pos, &m->pos, off);
    for (i = 0; i < 2; i++) {
        PuffParticle *part;
        PuffParticleTail *tail;
        n = below_x(0x10);
        part = spawn_puff(pos, 0.2f, 1.0f, 0.75f, below_x(2) == 0 ? n : -n, D_L14_0015F580_x,
                          160000.0f, 0x7F3030FF);
        if (part) {
            tail = &part->tail;
            part->life = frames_x(6);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->life;
        }
    }
    scale_x(off, &m->rot_row0, -1.0f);
    add_x(pos, &m->pos, off);
    n = 0x10;
    life = frames_x(2);
    size = 100000.0f;
    for (j = 0; j < 3; j++) {
        PuffParticle *part;
        PuffParticleTail *tail;
        part = spawn_puff(pos, 0.05f, 1.0f, 1.0f, n, D_L14_0015F580_x, size, 0x7FFFFFFF);
        if (part) {
            tail = &part->tail;
            part->life = life;
            part->b8 = below_x(0xFF);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->life;
        }
        n = -n;
        life *= 2;
        size -= 20000.0f;
    }
}
/* Flying debris: moves by its velocity under drag and gravity; while it is airborne it tumbles and trails
 * smoke, sparks and a light. When it hits the world or reaches the hero it explodes; once its timer runs out
 * or it leaves the level box it is removed. */
extern f32 D_L14_001615D0 __attribute__((sda));
extern f32 D_L14_001615D4 __attribute__((sda));
extern f32 D_L14_001615D8 __attribute__((sda));
extern char D_L14_001809C0[];
extern int tick_countdown_16_d(void *) __asm__("FUN_001f9770");
extern void add_d(void *, void *, void *) __asm__("FUN_001f9a10");
extern void sub_d(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_d(void *, void *, float) __asm__("FUN_001f9a68");
extern void normalize_d(void *, void *, float) __asm__("FUN_001f9bf8");
extern void clear_d(void *) __asm__("FUN_001f99f8");
extern float abs_d(float) __asm__("FUN_001f99c0");
extern float add_rot_d(float, float) __asm__("FUN_001fa580");
extern int frames_d(int) __asm__("FUN_001f96f8");
extern int rand_below_d(int) __asm__("FUN_00213260");
extern float rand_between_d(float, float) __asm__("FUN_002132a8");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int FUN_L00_0028dc90(int, int, void *, int);
extern void e450_b78(void *, void *, void *, float, float, int, int, int, float, float,
                             float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern char *FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
extern int FUN_L00_0023e738(float *, float, float, float, float, float);
extern PuffParticle *spawn_puff_d(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

void FUN_L14_002bac78(struct Moby *m) {
    char *d = (char *)m->pvars;
    float joint[4];
    float ofs[4];
    float at[4];
    float dir[4];
    float push[4];
    int i;
    int n;
    int c;
    float size;
    float bright;
    u8 *p;
    u8 *q;
    float *l;
    float range = 3.0f;
    float green = 0.8f;
    float blue = 0.2f;

    if (tick_countdown_16_d(d + 0x10) != 0)
        { FUN_L14_002bb4d8((u8 *)m); return; }
    add_d(&m->pos, &m->pos, d);
    if (abs_d(*(float *)(d + 0x0)) > D_L14_001615D8 * frame_scale)
        *(float *)(d + 0x0) *= (D_L14_001615D4 - 1.0f) * frame_scale + 1.0f;
    if (abs_d(*(float *)(d + 0x4)) > D_L14_001615D8 * frame_scale)
        *(float *)(d + 0x4) *= (D_L14_001615D4 - 1.0f) * frame_scale + 1.0f;
    if (*(float *)(d + 0x8) > 0.0f)
        *(float *)(d + 0x8) *= (D_L14_001615D4 - 1.0f) * frame_scale + 1.0f;
    *(float *)(d + 0x8) -= D_L14_001615D0 * frame_scale_sq;
    if (m->pos.x < 2.0f || m->pos.x > 1021.0f || m->pos.y < 2.0f || m->pos.y > 1021.0f || m->pos.z < 2.0f ||
        m->pos.z > 1021.0f)
        { FUN_L14_002bb4d8((u8 *)m); return; }
    if (coll_sphere(0.75f, &m->pos, 0, 0) != 0 ||
        (abs_d(m->pos.x - hero.motion.pos.f[0]) < 10.0f && abs_d(m->pos.y - hero.motion.pos.f[1]) < 10.0f)) {
        clear_d(joint);
        FUN_L00_0028dc90(1, 0, m, (*(struct Moby **)(d + 0x20))->oclass);
        e450_b78(m, joint, &m->pos, 2.0f, 4.0f, 5, 2, 4, 1.0f, 0.5f, 9.0f, 1.0f, -1, 5.0f, 0, 2,
                         -1, 0);
        FUN_L14_002bb4d8((u8 *)m);
        return;
    }
    m->rot.x = add_rot_d(m->rot.x, *(float *)(d + 0x14));
    m->rot.y = add_rot_d(m->rot.y, *(float *)(d + 0x18));
    m->rot.z = add_rot_d(m->rot.z, *(float *)(d + 0x1C));
    FUN_L00_0024f7c8(m, 0, joint);
    size = randf_sym(0.01f, 0.5f);
    scale_d(ofs, (char *)m + 0xC0, size);
    add_d(at, joint, ofs);
    sub_d(dir, joint, &m->pos);
    normalize_d(dir, dir, 0.05f);
    FUN_L00_0026cbb0(at, dir, 0x4F007FFF, 0x1FFFFFFF, random_int_between(frames_d(10), frames_d(20)), 1, 40000.0f);
    for (i = 0; i < 3; i++) {
        if (i != 2) {
            scale_d(ofs, (char *)m + 0xC0, randf_sym(0.01f, 0.5f));
            add_d(at, &m->pos, ofs);
            scale_d(ofs, (char *)m + 0xD0, randf_sym(0.01f, 0.5f));
            add_d(at, &m->pos, ofs);
        }
        n = rand_below_d(6);
        if (rand_below_d(2) != 0)
            n = -n;
        size = rand_between_d(20000.0f, 100000.0f);
        c = random_int_between(0x10, 0xFF);
        p = (u8 *)spawn_puff_d(at, 0.025f, 1.0f, 1.0f, n, D_L14_0015F580, size,
                         c | (0x60000000 | (c << 16) | (c << 8)));
        if (p != NULL) {
            p[3] = 0x44;
            q = p + 0x20;
            *(s16 *)(p + 0xA) = frames_d(60);
            q[0xA] = 0x60;
            *(s32 *)(q + 4) = 2;
            q[0xB] = p[0xA];
        }
    }
    bright = 1.0f;
    sub_d(push, joint, &m->pos);
    scale_d(push, push, 2.5f);
    add_d(at, &m->pos, push);
    if (rand_below_d(2) != 0)
        scale_d(push, (char *)m + 0xE0, -1.0f);
    else
        qcopy(push, (char *)m + 0xE0);
    add_d(at, at, push);
    if (*(short *)(d + 0x12) == -1) {
        *(short *)(d + 0x12) = FUN_L00_0023e738(at, range, 0.0f, bright, green, blue);
    } else {
        l = (float *)(D_L14_001809C0 + *(short *)(d + 0x12) * 32);
        qcopy(l + 4, at);
        l[7] = range;
        l[0] = bright;
        l[1] = green;
        l[2] = blue;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002BC560), where it is exact; names translated to the US level program. */


extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int scale_game_frames(int);
extern void FUN_L00_00250df8(void *);
extern void build_look_at_matrix(void *dst, void *vec, void *axis,
                                 float angle) __asm__("FUN_00214890");
extern void cross_vectors_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern struct Moby *func_0020D348_m(int) __asm__("FUN_0020c4f8");

unsigned char *FUN_L14_002bb310(struct Moby *owner, float angle, char *posp) {
    float vec[12];
    unsigned char *m;
    char *d;
    char *g;
    float *v0 = vec;
    float *v1;
    *(OvlQuad *)v0 = *(OvlQuad *)posp;
    m = (unsigned char *)func_0020D348_m(0x51);
    if (m != 0) {
        d = *(char **)(m + 0x78);
        qcopy(m + 0x10, v0);
        v1 = vec + 4;
        *(float *)(m + 0x48) = angle;
        m[0x20] = 0;
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        subtract_vector_xyz(v1, v0, &owner->pos);
        vec[6] = 0.0f;
        normalize_vector_xyz(v1, v1, random_float_between(frame_scale * 0.075f, frame_scale * 0.2f));
        g = (char *)&hero.gravity_dir;
        cross_vectors_xyz(vec + 8, g, v1);
        build_look_at_matrix(d, v1, g, randf_sym(0.0f, 0.7853982f));
        *(float *)(d + 8) = random_float_between(frame_scale * 0.05f, frame_scale * 0.32f);
        *(float *)(d + 0x14) = randf_sym(0.0f, 0.034906585f);
        *(float *)(d + 0x18) = randf_sym(0.0f, 0.034906585f);
        *(float *)(d + 0x1C) = randf_sym(0.0f, 0.034906585f);
        *(short *)(d + 0x10) = scale_game_frames(200);
        *(char **)(d + 0x20) = owner;
        *(short *)(d + 0x12) = -1;
        FUN_L00_00250df8(m);
    }
    return m;
}

/* One record of the level's path table (D_L14_0015F70C), 0x20 bytes apart. */
struct L14PathRec {
    u8 pad0[0x10];
    s32 *points;       /* 0x10: first word is the point count */
    u8 pad14[0xC];
};

/* Pvars of a moby that waits for the race to start, rides its path and leaves at a trigger volume. */
struct L14RacerVars {
    u8 pad0[0x40];
    Vec4 trail;        /* 0x40: effect state (FUN_L00_0025e450) */
    u8 pad50[0x10];
    s32 path;          /* 0x60: index into D_L14_0015F70C (-1: none) */
    u8 pad64[8];
    s32 timer;         /* 0x6C */
    f32 inv_time;      /* 0x70: 1 / timer at the start of the run */
    u8 pad74[0xC];
    s32 delay;         /* 0x80: frames to wait once the race starts */
    s32 end_point;     /* 0x84: D_L14_001B0BB0 entry the run ends at */
    s32 start_point;   /* 0x88: D_L14_001B0BB0 entry the run starts from (-1: none) */
    u8 pad8C[0x64];
    s32 unkF0;
    s32 unkF4;
    u8 padF8[8];
    u8 manips[3][0x40]; /* 0x100: attached as manipulators 2..4 */
    u8 pad1C0[0xC];
    s32 volume;        /* 0x1CC: clip volume where the racer leaves (-1: none) */
};

/* Race controller; its phase reaches 0x14 when the race starts. */
struct L14RaceState {
    u8 pad0[0x86];
    s16 phase;         /* 0x86 */
};

extern struct L14PathRec *race_paths __asm__("D_L14_0015F70C") __attribute__((sda));
extern struct L14RaceState *race_state __asm__("D_L14_00167500") __attribute__((section(".data")));
extern s32 D_L14_00161A2C __attribute__((sda));
extern s32 D_L14_00161A34 __attribute__((sda));
extern s32 D_L14_00161A5C __attribute__((sda));
extern void memset_words(void *, int, int) __asm__("FUN_001f97e8");
extern void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");
extern int fast_dec_timer(s32 *) __asm__("FUN_001f9740");
extern void set_moby_animation(struct Moby *, s32, s32) __asm__("FUN_00212ed8");
extern f32 compute_interpolated_record_value(struct Moby *) __asm__("FUN_0020c9e0");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern unsigned char *FUN_L14_002dea98(char *owner, char *pos);
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void FUN_L14_002e0170(struct Moby *, s32 *, int);
extern void FUN_L00_0025e450(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int);
void FUN_L14_002d6358(struct Moby *m);
int FUN_L14_002d64d0(struct Moby *m);
void FUN_L14_002d67e8(struct Moby *m);
void FUN_L14_002d7198(struct Moby *m);
void FUN_L14_002d7490(struct Moby *m);

void FUN_L14_002d5f40(struct Moby *m) {
    struct L14RacerVars *d = (struct L14RacerVars *)m->pvars;
    u8 pos[16];
    f32 vel[3];
    unsigned char *spark;
    f32 *sv;

    switch (m->state) {
    case 0:
        if (d->path == -1 || *race_paths[d->path].points == 0 || d->start_point == -1 ||
            *D_L14_001B0BB0[d->start_point] != 3 || d->volume == -1) {
            FUN_0020c828(m);
            return;
        }
        FUN_L14_002d6358(m);
        m->state = 1;
        m->flags = (m->flags & 0xEFFF) | 0x41;
        m->collision = 0;
        d->unkF4 = -1;
        d->unkF0 = -1;
        memset_words(d->manips[0], 0, 0x40);
        attach_manipulator(m, 2, d->manips[0]);
        memset_words(d->manips[1], 0, 0x40);
        attach_manipulator(m, 3, d->manips[1]);
        memset_words(d->manips[2], 0, 0x40);
        attach_manipulator(m, 4, d->manips[2]);
        return;
    case 1:
        if (race_state->phase == 0x14) {
            d->timer = FUN_001f96f8(d->delay);
            m->state = 2;
        }
        return;
    case 2:
        if (!fast_dec_timer(&d->timer))
            return;
        m->flags = (m->flags | 0x1000) & 0xFFBE;
        m->collision = m->pclass->collision;
        m->state = 3;
        set_moby_animation(m, 2, 0);
        m->anim_speed = 0.5f;
        d->timer = FUN_001f96f8(D_L14_00161A2C);
        d->inv_time = 1.0f / convert_integer_to_float(d->timer);
        qcopy(&m->pos, (char *)D_L14_001B0BB0[d->end_point] + 0x10);
    case 3:
        if (compute_interpolated_record_value(m) >= 3.0f)
            FUN_L14_002d64d0(m);
        if ((m->anim_flags & 2) && m->seq == m->prev_seq) {
            m->state = 4;
            blend_moby_animation((MobyAnim *)m, 0, 0, 10);
            m->anim_speed = 1.0f;
        }
        break;
    case 4:
        if (FUN_L14_002d64d0(m)) {
            m->state = 5;
            d->timer = FUN_001f96f8(D_L14_00161A34);
        }
        break;
    case 5:
        FUN_L14_002d7198(m);
        break;
    case 6:
        FUN_L14_002d67e8(m);
        if (is_point_inside_clip_volume(&m->pos, d->volume)) {
            moby_point_position(m, 2, pos);
            spark = FUN_L14_002dea98((char *)m, (char *)pos);
            if (spark) {
                sv = *(f32 **)(spark + 0x78);
                vel[0] = fast_cos(m->rot.z) * (frame_scale * 0.4f);
                vel[1] = fast_sin(m->rot.z) * (frame_scale * 0.4f);
                vel[2] = 0.0f;
                sv[0] = vel[0];
                sv[1] = vel[1];
            }
            FUN_L14_002e0170(m, &D_L14_00161A5C, 0);
            FUN_L00_0025e450(m, &d->trail, &m->pos, 0.0f, 0.0f, 5, 2, 4, 4.0f, 2.0f, 9.0f, 1, 1.0f,
                             15.0f, 1, 5, -1, 0);
            FUN_0020c828(m);
            return;
        }
        break;
    }
    FUN_L14_002d7490(m);
}

u8 D_L14_001D86CC[4] __attribute__((section(".data"))) = {0};
