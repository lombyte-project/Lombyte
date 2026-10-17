/* Level 16 moby classes: help_desk_lady, bolt_thief, taxi, grindrail_gate, grindrail_button, chomper, gadgetron_ceo, elevator, gadgetron_sign. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"

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
    int activation_region;
    int entry_region;
    int moving_timer;
    int target_timer;
    float target[4];
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

extern char D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[];
float D_L16_001D96C0[8] = {300.73f, 206.04f, 123.22f, 0.0f, 0.0f, 0.11f, 0.0f, 0.0f};
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9e90_cf(float, float) __asm__("FUN_001fa688");
extern float FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);
extern float truncate_float_to_s32_cf(float, float) __asm__("FUN_002132a8");
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *);
extern int help_request(int, int) __asm__("FUN_L00_00203908");
extern int FUN_L00_00266448(void *, void *);
extern int FUN_L00_002668a0(void *, void *);
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");
extern int random_float_between_cf(float) __asm__("FUN_001fa6d0");
extern int scale_game_frames(int);
extern int tick_countdown_32(int *);
extern s32 D_L16_0015F5C4 ;
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
extern L16WatchFlags D_0013D355_watch __asm__("D_0013D24D");

void FUN_L16_002e3190(L16WatchMoby *m) {
    L16WatchScratch scratch;
    L16WatchData *d = m->data;
    L16WatchFlags *flags;
    float rate;
    float head_rate;
    int tracking;
    void *activation;
    FUN_L12_002e6b60((char *)m);
    activation = &d->activation;
    switch (m->state) {
    case 0:
        FUN_L00_002668a0(m, activation);
        m->state = 1;
        d->activation.radius = 20.0f;
        m->opacity = 0xFF;
        break;
    case 1: {
        L16WatchPlayer *player = (L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D);
        if (player->disabled == 0 && player->mode != 15 && player->mode != 22) {
            if (is_point_inside_clip_volume((char *)player + 0x80, d->activation_region) &&
                FUN_L00_00266448(m, activation))
                m->state = 2;
        }
        break;
    }
    case 2:
        if (*(int *)&D_L16_0015F5C4 != 2) {
            char *player;
            m->state = 1;
            FUN_L00_00216f90(D_L16_001D96C0, D_L16_001D96C0 + 4, 0, 1);
            help_request(0x3E82, 0x84);
            FUN_L00_00260860(0x21, 1);
            FUN_L00_002502a0(m->widget);
            player = (char *)&D_0013E633_watch + 0xE9D;
            FUN_L00_00284e50(player, player + 0x10);
            memcard_save_data(0, -1);
            m->state = 3;
        }
        break;
    }
    flags = (L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B);
    if (!flags->entered &&
        is_point_inside_clip_volume((char *)&D_0013E633_watch + 0xE9D, d->entry_region))
        flags->entered = 1;
    if (!((L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B))->mission &&
        hero.state.control_mode == 15)
        ((L16WatchFlags *)((char *)&D_0013D355_watch + 0x13B))->mission = 1;
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
        scratch.eye[2] += 1.5f;
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
        d->yaw = yaw * 0.7f;
        d->head_yaw = yaw * 0.3f;
    }
    if (D_0015EDB0_b)
        d->height = 2.75f;
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->body, 0);
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->head, 2);
}
#define NOT_SDA

#define MACRO_ADDR

/* UpdateMoby: on the first tick scales two data floats by 0.25 into the moby and sets its state. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4B50), where it is exact; names translated to the US level program. */

extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_00250df8(void *);

void FUN_L16_002e36e8(struct Moby *moby) {
    float *d = (float *)moby->pvars;
    if (moby->state == 0) {
        fast_vec_normalize(&moby->rot_row1, &moby->rot_row1, d[0] * 0.25f);
        fast_vec_normalize(&moby->rot_row2, &moby->rot_row2, d[1] * 0.25f);
        moby->state = 1;
        moby->flags |= 0x101;
        FUN_L00_00250df8(moby);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4BD8), where it is exact; names translated to the US level program. */

void FUN_L16_002e3770(void *moby, int on) {
    if (*(short *)((char *)moby + 0xA6) == 0x56B) {
        if (on != 0) {
            *(int *)((char *)moby + 0x94) = *(int *)(*(char **)((char *)moby + 0x24) + 0x10);
        } else {
            *(int *)((char *)moby + 0x94) = 0;
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Update of the crate platform: hit reaction, then its seven states. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4C08), where it is exact; names translated to the US level program. */

typedef struct {
    char pad00[0x60];
    char effect[7];             /* 0x60: record func_L00_0025E590 updates */
    unsigned char ticks;        /* 0x67 */
    char pad68[8];
    float knock[4];             /* 0x70: record func_L00_0025D5B0 fills (velocity first) */
    float knock_drag;           /* 0x80 */
    float knock_gravity;        /* 0x84 */
    float knock_speed;          /* 0x88 */
    float knock_rise;           /* 0x8C */
    int unknown90;
    int knock_flags;            /* 0x94 */
    char pad98[0x15];
    unsigned char knock_bAD;    /* 0xAD */
    char padAE[0x22];
    int counter;                /* 0xD0 */
    float timer;                /* 0xD4 */
    int path;                   /* 0xD8: index into D_L16_001B0C30 */
    int other_path;             /* 0xDC */
    int trigger;                /* 0xE0 */
    float vx;                   /* 0xE4 */
    float vy;                   /* 0xE8 */
    float vz;                   /* 0xEC */
    float turn;                 /* 0xF0 */
    char *child;                /* 0xF4 */
    int unknownF8;
    int mode;                   /* 0xFC */
} L16CrateData;

typedef struct {
    char pad00[0x10];
    int bounds;
} L16CrateModel;

typedef struct {
    char pad00[0x10];
    float position[4];          /* 0x10 */
    unsigned char state;        /* 0x20 */
    char pad21[3];
    L16CrateModel *model;       /* 0x24 */
    char pad28[8];
    unsigned char collision;    /* 0x30 */
    char pad31[3];
    unsigned short flags;       /* 0x34 */
    char pad36[10];
    float rotation[4];          /* 0x40 */
    char pad50[2];
    unsigned char frame;        /* 0x52 */
    unsigned char animation;    /* 0x53 */
    char pad54[0x1C];
    unsigned char status;       /* 0x70 */
    char pad71[7];
    L16CrateData *data;         /* 0x78 */
    char pad7C[0x18];
    int bounds;                 /* 0x94 */
    char pad98[0xC];
    unsigned char opacity;      /* 0xA4 */
    char padA5[0xD];
    short id;                   /* 0xB2 */
} L16CrateMoby;

typedef struct {
    char pad00[0x80];
    float position[4];          /* 0x80 */
    char pad90[0x1AC];
    void *standing;             /* 0x23C: the moby the hero stands on */
} L16CratePlayer;

extern char *D_L16_001B0930[];
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *FUN_L14_002ffaf8(void *, void *);
extern char D_L16_001E86A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8A58[], D_L16_001E8AA0[];
extern char D_L16_001E89A0[], D_L16_001E86D8[], D_L16_001E8A20[], D_L16_001E8A58[], D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8720[], D_L16_001E8A58[], D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8758[], D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8A58[], D_L16_001E87A0[];
extern float frame_time ,D_0015EE70;
extern float fast_arc_tan(float,float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);
extern int debug_print_alt() __asm__("FUN_001e93b0");
extern int FUN_L00_0025c698(void *,void *);
extern int FUN_L16_002e3fa0(struct Moby *moby);
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("func_00214720");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 D_L16_00161D94;
extern f32 D_L16_00161D98_gp __asm__("D_L16_00161D98") __attribute__((sda));
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025c558(void *, void *, int, int, int, float);
extern void FUN_L00_0025d538(void *, void *);
extern void FUN_L00_0025f090(void *, void *, int, float, float);
extern void FUN_L03_002c6c90(void *);
extern void FUN_L14_002fe900(void *);
extern void FUN_L14_002fec28(void *);
extern void FUN_L14_002feca8(void *);
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void set_moby_animation_alt(void *, int, int) __asm__("FUN_00212ed8");
void FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
extern void func_L00_002592B0_path54(char*,float,float*,float,float,float) __asm__("FUN_L00_00258278");

void FUN_L16_002e37a0(L16CrateMoby *moby) {
    L16CrateData *d = moby->data;
    char *hit;

    if ((moby->status & 2) && moby->frame == moby->animation && moby->frame == 1)
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
    if (hero.coll_hit_moby == moby && moby->animation != 1 && moby->frame != 1)
        blend_moby_animation(moby, 1, 0, scale_game_frames(5));
    hit = find_moby_hit(moby, 0x210000, 0);
    if (!hit && d->child)
        hit = find_moby_hit(d->child, 0x210000, 0);
    if (hit && moby->state != 6) {
        struct Hero *player = &hero;

        d->knock_drag = 0.008f;
        d->knock_gravity = 0.0005f;
        d->knock_speed = frame_time * 24.0f;
        d->knock_rise = frame_time * 12.0f;
        d->knock_flags = 1;
        d->knock_bAD = 0;
        FUN_L00_0025c558(moby, d->knock, 1, 1, 0,
                          fast_arc_tan(moby->position[0] - player->motion.pos.f[0], moby->position[1] - player->motion.pos.f[1]));
        d->ticks = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 6;
    }
    moby->opacity = 0xFF;
    switch (moby->state) {
    case 0:
        if (d->mode == 2) {
            moby->state = 1;
            d->turn = 0.0f;
            set_moby_animation_alt(moby, 4, 0);
        } else {
            if (d->path == -1) {
                debug_print_alt(D_L16_001E86A0, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (*(int *)D_L16_001B0930[d->path] == 0) {
                debug_print_alt(D_L16_001E86D8, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (d->mode != 1) {
                if (d->other_path == -1) {
                    debug_print_alt(D_L16_001E8720, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
                if (*(int *)D_L16_001B0930[d->other_path] == 0) {
                    debug_print_alt(D_L16_001E8758, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
            }
            if (d->trigger == -1) {
                debug_print_alt(D_L16_001E87A0, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            FUN_L14_002fe900(moby);
            moby->state = 2;
            moby->collision = 0x80;
            moby->flags |= 0x41;
            moby->bounds = 0;
        }
        d->child = 0;
        break;
    case 1: {
        struct Hero *player = &hero;

        func_L00_002592B0_path54((char *)moby,
                                 fast_arc_tan(player->motion.pos.f[0] - moby->position[0], player->motion.pos.f[1] - moby->position[1]),
                                 &d->turn, 0.005f, 0.2f, 0.0f);
        FUN_L14_002feca8(moby);
        break;
    }
    case 2:
        if (is_point_inside_clip_volume(&hero.motion.pos, d->trigger)) {
            if (d->mode == 1) {
                set_moby_animation_alt(moby, 0, 0);
            } else {
                d->child = FUN_L14_002ffaf8(moby, moby->position);
                FUN_L14_002fec28(moby);
                set_moby_animation_alt(moby, 2, 0);
            }
            moby->flags &= 0xFFBE;
            moby->bounds = moby->model->bounds;
            moby->state = 4;
        }
        break;
    case 4:
        if (d->mode == 0)
            FUN_L14_002fec28(moby);
        if (FUN_L16_002e3fa0((char *)moby)) {
            moby->state = 3;
            blend_moby_animation(moby, 3, 0, 5);
        }
        FUN_L14_002feca8(moby);
        break;
    case 3: {
        L16CrateData *a = moby->data;
        char *path = D_L16_001B0930[a->path];
        float target[4];
        float zero;

        if (a->mode == 0)
            FUN_L14_002fec28(moby);
        qcopy(target, path + *(int *)path * 16);
        zero = 0.0f;
        FUN_L00_0025b8c0(moby->position, &a->vx, target[0], *(float *)&D_L16_00161D94, *(float *)&D_L16_00161D98_gp, zero);
        FUN_L00_0025b8c0(moby->position + 1, &a->vy, target[1], *(float *)&D_L16_00161D94, *(float *)&D_L16_00161D98_gp, zero);
        FUN_L00_0025b8c0(moby->position + 2, &a->vz, target[2], *(float *)&D_L16_00161D94, *(float *)&D_L16_00161D98_gp, zero);
        if ((moby->status & 2) && moby->frame == moby->animation) {
            if (a->mode == 1) {
                moby->state = 1;
                a->turn = zero;
                set_moby_animation_alt(moby, 4, 0);
            } else {
                if (a->child) {
                    FUN_L03_002c6c90(a->child);
                    a->child = 0;
                }
                moby->state = 5;
                blend_moby_animation(moby, 0, 0, 20);
                a->timer = zero;
                a->counter = 0;
            }
        }
        FUN_L14_002feca8(moby);
        break;
    }
    case 5:
        if (FUN_L16_002e3fa0((char *)moby)) {
            char *path;

            moby->state = 2;
            moby->bounds = 0;
            moby->flags |= 0x41;
            d->timer = 0.0f;
            d->counter = 0;
            d->turn = d->vx = d->vy = d->vz = d->timer;
            path = D_L16_001B0930[d->path];
            qcopy(moby->position, path + 0x10);
            moby->rotation[2] = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                               *(float *)(path + 0x24) - *(float *)(path + 0x14));
        } else {
            FUN_L14_002feca8(moby);
        }
        break;
    case 6:
        if (FUN_L00_0025c698(moby, d->knock) & 3) {
            FUN_L00_0025f090(moby, moby->position, -1, 1.0f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->position[2] < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    FUN_L00_0025d538(moby, d->effect);
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5408), where it is exact; names translated to the US level program. */

extern char *D_L16_001B0930[];
extern float absolute_float(float) __asm__("func_001F99C0");
extern float frame_time, D_0015EE70;
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);
extern int FUN_L00_0025d808() __asm__("FUN_L00_0025d808");
extern float D_L16_00161D90 __attribute__((sda));
extern float D_L16_00161D94_gp __asm__("D_L16_00161D94") __attribute__((sda));
extern float D_L16_00161D98_gp __asm__("D_L16_00161D98") __attribute__((sda));
extern void subtract_vector_xyz(void *, void *, void *);
void FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
extern int FUN_L00_0025d808_path54(char *, float *, float *, float *, int,
                                    float) __asm__("FUN_L00_0025d808");
extern void func_L00_002592B0_path54(char *, float, float *, float, float,
                                     float) __asm__("FUN_L00_00258278");
extern char *D_L16_001B0930[];

int FUN_L16_002e3fa0(struct Moby *moby) {
    float pos[4];
    float delta[4];
    char *data = (char *)moby->pvars;
    float t = D_L16_00161D90 * frame_time;
    int r;
    char *path;
    if (moby->state == 5)
        path = D_L16_001B0930[*(int *)(data + 0xDC)];
    else
        path = D_L16_001B0930[*(int *)(data + 0xD8)];
    r = FUN_L00_0025d808_path54(path, pos, (float *)(data + 0xD0), (float *)(data + 0xD4), 0, t);
    FUN_L00_0025b8c0(&moby->pos.x, (float *)(data + 0xE4), pos[0], D_L16_00161D94_gp,
                     D_L16_00161D98_gp, 0.0f);
    FUN_L00_0025b8c0(&moby->pos.y, (float *)(data + 0xE8), pos[1], D_L16_00161D94_gp,
                     D_L16_00161D98_gp, 0.0f);
    FUN_L00_0025b8c0(&moby->pos.z, (float *)(data + 0xEC), pos[2], D_L16_00161D94_gp,
                     D_L16_00161D98_gp, 0.0f);
    subtract_vector_xyz(delta, pos, &moby->pos);
    if (absolute_float(delta[0]) > 0.01f) {
        if (absolute_float(delta[1]) > 0.01f) {
            func_L00_002592B0_path54(moby, fast_arc_tan(delta[0], delta[1]), (float *)(data + 0xF0),
                                     D_L16_00161D94_gp, D_L16_00161D98_gp, 0.0f);
        }
    }
    return r;
}
#include "sda.h"

#include "qcopy.h"

/* Update the moving pickup, interpolate its path transitions, and manage its sound handle. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5848), where it is exact; names translated to the US level program. */

typedef struct {
    char pad00[0x30];
    float position[4];
    char pad40[0x30];
    float direction[4];
} L16PickupPose;

extern L16PickupPose *D_L16_001600EC;
extern char *FUN_L05_00319598(void *, int);
extern char D_0013CAE4[];
extern char D_0013E550[];
extern float advance_accelerated_scalar_c(float, float, float, float, float *,
                                          float *) __asm__("FUN_00213f38");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float frame_time,
    D_0015EE70_c __asm__("D_0015EE70");
extern float D_0015EE6C_u,
    D_0015ED70_c2 __asm__("D_0015ED70");
extern int FUN_L00_0028d8c0(void *, int);
extern int FUN_L05_003195f8(char *);
extern s32 D_L16_0015F594;
extern unsigned char D_0013D4E1 __attribute__((section(".data")));
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void FUN_001f9d20(void *, void *, void *);
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L00_00260738(char *a, void *b, void *c, void *d);
extern void FUN_L00_00266858(void *, int);
extern void FUN_L05_00319510(void *);
extern void FUN_L05_00319690(char *);
extern void FUN_L05_00319740(char *);
extern void FUN_L05_003198e8(char *);
extern void FUN_L16_002e4900_c(void *) __asm__("FUN_L16_002e4900");
extern void FUN_L16_002e4a58_c(char *) __asm__("FUN_L16_002e4a58");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void sample_camera_path_u(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern int func_0022ED80_i(int, int, void *) __asm__("FUN_0022da68");
extern void func_00215CA8_f5848(float, int *, int, void *, float *, int) __asm__("FUN_00214e58");

void FUN_L16_002e43e0(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    float v00[4], v10[4], v20[4], v30[4], v40[4], v50[4], v60[4], v70[4];
    struct Hero *g;
    struct Hero *g2;
    float z;
    char *m;
    char *e;
    L16PickupPose *tab;
    float *qa;
    int h;
    int s;
    qcopy(v00, &moby->pos);
    qcopy(v10, &moby->rot);
    FUN_L00_00266858(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case 0:
        *(short *)(d + 0xAA) = -1;
        FUN_L16_002e4900_c(moby);
        break;
    case 1:
        g = &hero;
        if (((char *)g->ground_moby) == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(v30, &moby->rot_row0);
            subtract_vector_xyz(v70, &g->motion.pos, &moby->pos);
            v70[3] = 0;
            FUN_001f9d20(v70, v70, v30);
            if (absolute_float(v70[1]) < 1.2f) {
                FUN_L16_002e4a58_c(moby);
                FUN_L05_00319690(moby);
                if ((*(int *)(D_0013CAE4) & 0x10) && *(int *)&D_L16_0015F594 == 8) {
                    moby->state = 3;
                    m = FUN_L05_00319598(moby, *(short *)(d + 0xAE));
                    if (m != 0) {
                        m[0x20] = 2;
                        *(unsigned short *)(m + 0x34) |= 1;
                        m[0x31] = 0;
                        *(int *)(m + 0x94) = 0;
                    }
                }
            }
        }
        break;
    case 3:
        advance_accelerated_scalar_c(1.0f, D_0015ED70_c2, D_0015ED70_c2, frame_time * 0.5f,
                                     (float *)(d + 0xB0), (float *)(d + 0xB4));
        qa = v30;
        s = *(int *)(d + 0x80 - (-(*(short *)(d + 0xAC) * 4)));
        tab = D_L16_001600EC;
        qcopy(qa, tab[s].position);
        qcopy(v50, tab[s].direction);
        z = 0.0f;
        func_00215CA8_f5848(z, *(int **)(d + 0xA4), 0, v40, v60, 0);
        lerp_vector(d + 0x60, v30, v40, *(float *)(d + 0xB0));
        *(float *)(d + 0x74) = fast_add_rotations(
            fast_subtract_rotations(v60[1], v50[1]) * *(float *)(d + 0xB0), v50[1]);
        *(float *)(d + 0x78) = fast_add_rotations(
            fast_subtract_rotations(v60[2], v50[2]) * *(float *)(d + 0xB0), v50[2]);
        if (1.0f <= *(float *)(d + 0xB0)) {
            *(float *)(d + 0xB0) = z;
            *(float *)(d + 0xB4) = z;
            moby->state = 4;
        }
        break;
    case 4:
        if (FUN_L05_003195f8(moby)) {
            *(int *)(d + 0xB0) = 0;
            *(int *)(d + 0xB4) = 0;
            moby->state = 5;
        }
        break;
    case 5:
        advance_accelerated_scalar_c(1.0f, D_0015ED70_c2, D_0015ED70_c2, frame_time * 0.5f,
                                     (float *)(d + 0xB0), (float *)(d + 0xB4));
        qa = v40;
        s = *(int *)(d + 0x80 - (-(*(short *)(d + 0xAE) * 4)));
        tab = D_L16_001600EC;
        qcopy(qa, tab[s].position);
        qcopy(v60, tab[s].direction);
        sample_camera_path_u(*(int **)(d + 0xA4), 0, v30, v50, 0, (float)**(int **)(d + 0xA4));
        lerp_vector(d + 0x60, v30, v40, *(float *)(d + 0xB0));
        *(float *)(d + 0x74) = fast_add_rotations(
            fast_subtract_rotations(v60[1], v50[1]) * *(float *)(d + 0xB0), v50[1]);
        *(float *)(d + 0x78) = fast_add_rotations(
            fast_subtract_rotations(v60[2], v50[2]) * *(float *)(d + 0xB0), v50[2]);
        if (1.0f <= *(float *)(d + 0xB0)) {
            *(int *)(d + 0xB0) = 0;
            *(int *)(d + 0xB4) = 0;
            moby->state = 1;
            FUN_L05_00319510(moby);
        }
        break;
    case 6:
        if (D_0013D4E1) {
            char *model = moby->pclass;
            unsigned short flags = moby->flags;
            int value = *(int *)(model + 0x10);
            moby->visible = 1;
            moby->collision = value;
            moby->flags = flags & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    FUN_L05_00319740(moby);
    if (moby->state >= 3 && moby->state <= 5) {
        g2 = &hero;
        g2->unk1F2 = 2;
        g2->unk1F4 = 2;
        FUN_L05_003198e8(moby);
        if (FUN_L00_0028d8c0(moby, *(short *)(d + 0xAA)) == 0) {
            *(short *)(d + 0xAA) = func_0022ED80_i(0, 4, moby);
        }
    } else if (FUN_L00_0028d8c0(moby, *(short *)(d + 0xAA)) != 0) {
        h = *(short *)(d + 0xAA);
        if (h != -1) {
            e = D_0013E550 + h * 0x70;
            if (*(char **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                release_voice_slot(h);
            }
        }
        *(short *)(d + 0xAA) = -1;
    }
    subtract_vector_xyz(v20, &moby->pos, v00);
    FUN_L00_00260738(d + 0x20, v20, v10, &moby->rot);
}
#include "sda.h"
#include "qcopy.h"
extern float func_001F9D10_e4900(void *, void *) __asm__("FUN_001f9b48");
extern char *D_L16_001601AC_slots_e4900 __asm__("D_L16_001600EC");
extern unsigned char D_0013D5C8_b_e4900[] __asm__("D_0013D4C0");
void FUN_L16_002e4900(void *moby_v) {
    char *moby = moby_v;
    char *data = *(char **)(moby + 0x78);
    int *list = (int *)(data + 0x80);
    int i;
    int idx;
    char *g;
    *(short *)(data + 0xAC) = -1;
    for (i = 0; i < 3; i++) {
        if (func_001F9D10_e4900(moby + 0x10, D_L16_001601AC_slots_e4900 + (list[i] << 7) + 0x30) <
            1.0f) {
            *(short *)(data + 0xAC) = i;
        }
    }
    {
        if (D_0013D5C8_b_e4900[0x21] != 0 || *(short *)(data + 0xAC) == 2) {
            int v = *(int *)(*(char **)(moby + 0x24) + 0x10);
            moby[0x31] = 1;
            *(int *)(moby + 0x94) = v;
            *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) & 0xFFFE;
            moby[0x20] = 1;
        } else {
            moby[0x20] = 6;
            moby[0x31] = 0;
            *(unsigned short *)(moby + 0x34) = *(unsigned short *)(moby + 0x34) | 1;
            *(int *)(moby + 0x94) = 0;
        }
    }
    {
        int *selected = list;
        selected += *(short *)(data + 0xAC);
        idx = *selected << 7;
    }
    g = D_L16_001601AC_slots_e4900;
    qcopy(data + 0x60, (void *)(idx + (int)g + 0x30));
    qcopy(data + 0x70, (void *)(idx + (int)g + 0x70));
}
/* Pick the path for the current segment and set the travel speeds from its length. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5EC0), where it is exact; names translated to the US level program. */

extern char *D_L16_001B0930[];
extern float FUN_001f9b48(void *, void *);

void FUN_L16_002e4a58(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    char *p;
    float inv, a, b, r;
    switch (*(short *)(d + 0xAC)) {
    case 0:
        *(char **)(d + 0xA4) = D_L16_001B0930[*(int *)(d + 0x90)];
        *(short *)(d + 0xAE) = 1;
        break;
    case 1:
        *(char **)(d + 0xA4) = D_L16_001B0930[*(int *)(d + 0x94)];
        *(short *)(d + 0xAE) = 0;
        break;
    case 2:
        *(char **)(d + 0xA4) = D_L16_001B0930[*(int *)(d + 0x9C)];
        *(short *)(d + 0xAE) = 1;
        break;
    }
    p = *(char **)(d + 0xA4);
    r = FUN_001f9b48(p + 0x10, p + 0x20);
    a = frame_time * 20.0f;
    b = frame_time_sq * 5.0f;
    inv = 1.0f / (r * (float)**(int **)(d + 0xA4));
    *(int *)(d + 0xB4) = 0;
    *(int *)(d + 0xB0) = 0;
    *(float *)(d + 0xC4) = a * inv;
    *(float *)(d + 0xC8) = b * inv;
}
/* Moves the moby through its trigger states and checks its local bounds. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E6478), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
float D_L16_001D9750[1] __attribute__((section(".data"))) = {-4.0f};
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int D_L16_0015F5CC;
extern int FUN_001fa6e0_u(float, int, int) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void FUN_001f9d20(void *, void *, void *);
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern int tick_countdown_32_u(void *) __asm__("func_001F9740");
extern int func_0022ED80_6478(int, int, void *) __asm__("FUN_0022da68");

int D_L16_00161DD8 = 600;
int D_L16_00161DDC = 0x80808080;
int D_L16_00161DE0 = 0x80202020;
void FUN_L16_002e5010(struct Moby *m) {
    char *d = (char *)m->pvars;
    char *g;
    float pos[16];
    float mat[4];
    float f;
    int period = scale_game_frames(30);
    f = convert_integer_to_float(D_L16_0015F5CC % period) / convert_integer_to_float(period);
    m->glow_color = FUN_001fa6e0_u(fast_sin(f * 6.28318f - 3.14159f) * 0.5f + 0.5f,
                                        D_L16_00161DDC, D_L16_00161DE0);
    switch (m->state) {
    case 0:
        m->state = 1;
        m->update_dist = 0xFF;
        break;
    case 1:
        g = (char *)&hero.motion.pos;
        if (FUN_001f9b48(&m->pos, g) < 16.0f) {
            transpose_rotation_matrix(pos, &m->rot_row0);
            subtract_vector_xyz(mat, g, &m->pos);
            FUN_001f9d20(mat, mat, pos);
            if (D_L16_001D9750[0] < mat[0] && mat[0] < D_L16_001D9750[1] &&
                D_L16_001D9750[2] < mat[1] && mat[1] < D_L16_001D9750[3] &&
                D_L16_001D9750[4] < mat[2] && mat[2] < D_L16_001D9750[5]) {
                m->command = 1;
                m->state = 2;
                *(int *)d = scale_game_frames(D_L16_00161DD8);
                func_0022ED80_6478(1, 0, m);
            }
        }
        break;
    case 2:
        if (tick_countdown_32_u((int *)d)) {
            m->state = 1;
            m->command = 0;
        }
        break;
    }
}
/* Moves the moby through its trigger states and checks its local bounds. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E66C0), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float_q(s32) __asm__("FUN_001fa6c0");
extern float FUN_001f9b48_q(void *, void *) __asm__("FUN_001f9b48");
extern float fast_sin_q(float) __asm__("FUN_001f9de0");
extern int D_L16_0015F5CC;
extern int FUN_001fa6e0_q(float, int, int) __asm__("FUN_001fa6e0");
extern s32 scale_game_frames_q(s32) __asm__("FUN_001f96f8");
extern void FUN_001f9d20_q(void *, void *, void *) __asm__("FUN_001f9d20");
extern void FUN_001fa2d8_q(void *, void *) __asm__("FUN_001fa2d8");
extern void FUN_0022da68_q(int, int, void *) __asm__("FUN_0022da68");
extern void subtract_vector_xyz_q(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern int tick_countdown_32_q(void *) __asm__("FUN_001f9740");
extern int func_0022ED80_66C0_q(int, int, void *) __asm__("FUN_0022da68");

float D_L16_001D9768[1] __attribute__((section(".data"))) = {-3.0f};
int D_L16_00161DE4 = 600;
int D_L16_00161DE8 = 0x80202020;
int D_L16_00161DEC = 0x80808080;
extern char D_0013F3D0[] __attribute__((section(".data")));
void FUN_L16_002e5258(struct Moby *m) {
    char *d = (char *)m->pvars;
    char *g;
    float pos[16];
    float mat[4];
    float f;
    int period = scale_game_frames_q(30);
    f = convert_integer_to_float_q(D_L16_0015F5CC % period) / convert_integer_to_float_q(period);
    m->glow_color = FUN_001fa6e0_q(fast_sin_q(f * 6.28318f - 3.14159f) * 0.5f + 0.5f,
                                        D_L16_00161DE8, D_L16_00161DEC);
    switch (m->state) {
    case 0:
        m->state = 1;
        m->update_dist = 0xFF;
        break;
    case 1:
        g = D_0013F3D0;
        if (FUN_001f9b48_q(&m->pos, g) < 16.0f) {
            FUN_001fa2d8_q(pos, &m->rot_row0);
            subtract_vector_xyz_q(mat, g, &m->pos);
            mat[3] = 0.0f;
            FUN_001f9d20_q(mat, mat, pos);
            if (D_L16_001D9768[0] < mat[0] && mat[0] < D_L16_001D9768[1] &&
                D_L16_001D9768[2] < mat[1] && mat[1] < D_L16_001D9768[3] &&
                D_L16_001D9768[4] < mat[2] && mat[2] < D_L16_001D9768[5]) {
                func_0022ED80_66C0_q(1, 0, m);
                m->command = 1;
                m->state = 2;
                *(int *)d = scale_game_frames_q(D_L16_00161DE4);
            }
        }
        break;
    case 2:
        if (tick_countdown_32_q((int *)d)) {
            m->state = 1;
            m->command = 0;
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E6908), where it is exact; names translated to the US level program. */

typedef struct {
    float value;
} Level16VendorTurnData;

typedef struct {
    char pad0[0x20];
    unsigned char state;
    char pad21[0x57];
    Level16VendorTurnData *data;
    char pad7C[0x40];
    unsigned char active;
} Level16VendorTurnMoby;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float FUN_001f9b80(void *, void *);
extern float FUN_001f9de0(float);
extern float FUN_001fa580(float, float);
extern float random_angle_radians_alt(void) __asm__("FUN_00213308");
extern int FUN_001f9740(int *);
extern int FUN_001fa6e0_u(float, int, int) __asm__("FUN_001fa6e0");
float D_L16_00161DF0[4] __attribute__((section(".sdata"))) = {0.0f, -1.0f, 1.0f, 0.0f};
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_001f9cf8(void *, void *, void *);
extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L16_002e56e0_c(Level16VendorTurnMoby *moby) __asm__("FUN_L16_002e56e0");
extern void func_L16_002E6B48_6908(void *) __asm__("FUN_L16_002e56e0");
extern int func_0022ED80_6908(int, int, void *) __asm__("FUN_0022da68");

void FUN_L16_002e54a0(struct Moby *m) {
    char *d = (char *)m->pvars;
    char *g;
    char *base;
    float v[4];
    float f;
    switch (m->state) {
    case 0:
        m->state = 1;
        *(float *)d = random_angle_radians_alt();
        break;
    case 1:
        if (FUN_001f9740((int *)(d + 4))) {
            g = (char *)&hero.motion.pos;
            if (FUN_001f9b80(&m->pos, g) < 4.0f) {
                base = g - 0x80;
                if (*(int *)(base + 0x2084) == 0x28 && *(int *)(base + 0x5A8) != -1) {
                    FUN_001f9cf8(v, &D_L16_00161DF0, &m->rot_row0);
                    FUN_001f9a10(v, v, &m->pos);
                    FUN_001f9a28(v, v, g);
                    FUN_001f9cf8(v, v, g - 0x40);
                    if (absolute_float(v[1]) < 0.1f) {
                        if ((v[0] > 0.0f && *(int *)(base + 0x5A8) == 3) ||
                            (v[0] < 0.0f && *(int *)(base + 0x5A8) == 2)) {
                            func_L16_002E6B48_6908(m);
                            func_0022ED80_6908(1, 0, m);
                        }
                    }
                }
            }
        }
        if (find_moby_hit(m, 0x330000, 0)) {
            func_L16_002E6B48_6908(m);
            func_0022ED80_6908(0, 0, m);
            m->hit_slot = 0xFF;
        }
        break;
    }
    *(float *)d = FUN_001fa580(*(float *)d, frame_time * 12.566371f);
    f = (FUN_001f9de0(*(float *)d) + 1.0f) * 0.5f;
    if (m->command) {
        m->glow_color = FUN_001fa6e0_u(f, 0x80208020, 0x80202020);
    } else {
        m->glow_color = FUN_001fa6e0_u(f, 0x80202080, 0x80202020);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Start the vendor moby's turn and mark it active. */
/* Ported from rac1-decomp (src/overlays/l16/vendor_002A50F0.c: func_L16_002E6B48), where it is exact; names translated to the US level program. */

void FUN_L16_002e56e0(Level16VendorTurnMoby *moby) {
    moby->data->value = -3.1415925f;
    moby->active = 1;
    moby->state = 2;
}
#include "qcopy.h"

/* Update the wandering helper's motion, animations, interaction and damage states. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7270), where it is exact; names translated to the US level program. */

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

extern char D_L16_001671C0[];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern float FUN_L16_002e6428_u(void *, void *) __asm__("FUN_L16_002e6428");
extern int FUN_L00_0025c698(void *, void *);
extern int FUN_L00_002dc9f0(void *, void *);
extern s32 rand();
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_002599e8(void *, int, int, float, float, float, int, int, int);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025f090(void *, void *, int, float, float);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
void FUN_L16_002e6208(void *arg);
s32 is_value_within_interpolated_window(struct InterpolatedStateEntry *arg0, f32 fparg0) __asm__("FUN_00214cc8");
void blend_moby_animation_u(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");
void mark_moby_for_removal_u(struct Obj *obj) __asm__("FUN_0020c828");
extern int func_001FA898_caa18(float) __asm__("FUN_001fa6d0");

short D_L16_00161E48 = 0;
void FUN_L16_002e5e08(struct Moby *m) {
    char *d = (char *)m->pvars;
    FUN_L16_002e6208(m);
    FUN_L00_00263ac8(2.1f, (int)m, 1, (unsigned char *)(d + 0x220));
    if (m->visible != 0 && FUN_001f9b48(&m->pos, D_L16_001671C0) < 29.0f) {
        FUN_L00_0025a120(m);
        m->shadow_range = 23;
    }
    switch (m->state) {
    case 0:
        if (*(int *)(d + 0x1F0) == -1) {
            mark_moby_for_removal_u(m);
            return;
        }
        *(unsigned short *)(d + 0x24) = 1;
        *(unsigned char *)(d + 0x28) = 0;
        *(float *)(d + 0x20) = 1.0f;
        *(int *)(d + 0xD0) = (int)&D_L16_00161E48;
        if (rand() & 1)
            m->flags |= 0x8000;
        qcopy(d + 0x1D0, &m->pos);
        m->state = 1;
        if (m->prev_seq != 2)
            blend_moby_animation_u(m, 2, 0, scale_game_frames(20));
        *(unsigned char *)(d + 0x58) = func_001FA898_caa18(8.0f);
        *(unsigned char *)(d + 0x5A) = func_001FA898_caa18(3.2f);
        break;
    case 1:
        if (*(int *)(d + 0x1C4) != 2) {
            m->state = 4;
            if (m->prev_seq != 0)
                blend_moby_animation_u(m, 0, 0, scale_game_frames(20));
        }
        break;
    case 2:
    case 3:
        break;
    case 4:
        if (FUN_L16_002e6428_u(m, d + 0x180) < 1.25f) {
            m->state = 5;
            if (m->prev_seq != 1)
                blend_moby_animation_u(m, 1, 0, scale_game_frames(20));
        } else if (*(int *)(d + 0x1C4) == 2)
            m->state = 6;
        break;
    case 5: {
        float *yaw = &m->rot.z;
        float angle = fast_arc_tan(*(float *)(d + 0x180) - m->pos.x,
                                   *(float *)(d + 0x184) - m->pos.y);
        FUN_L00_0025be00(yaw, (float *)(d + 0x1F4), angle, frame_time_sq * 12.566371f,
                         frame_time_sq * 12.566371f, frame_time * 12.566371f);
        if (is_value_within_interpolated_window(m, 13.0f)) {
            FUN_L00_002599e8(m, 0, 1, 0.333f, 1.0f, 1.0f, 0, 1, 0);
        } else if ((m->anim_flags & 2) && FUN_001f9b48(&m->pos, d + 0x180) > 1.5f) {
            m->state = 4;
            if (m->prev_seq != 0)
                blend_moby_animation_u(m, 0, 0, scale_game_frames(20));
        }
        break;
    }
    case 6:
        if (FUN_L16_002e6428_u(m, d + 0x1D0) < 1.0f) {
            m->state = 1;
            if (m->prev_seq != 2)
                blend_moby_animation_u(m, 2, 0, scale_game_frames(20));
        } else if (*(int *)(d + 0x1C4) != 2)
            m->state = 4;
        break;
    case 7:
        if (FUN_L00_002dc9f0(m, d + 0x120)) {
            m->state = 1;
            *(unsigned short *)(d + 0xC8) = 0;
        }
        break;
    case 8:
        if (FUN_L00_0025c698(m, d + 0x120) & 0x140) {
            FUN_L00_0025f090(m, &m->pos, 6, 0.5f, 13.0f);
            FUN_L00_00257470(m, 0, -1);
            mark_moby_for_removal_u(m);
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Start the knock-back when the moby is hit, then steer it along its path and note when it strays. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7670), where it is exact; names translated to the US level program. */


typedef struct {
    char pad0[0x110];
    short reaction[3];          /* 0x110: record func_L00_0025E4B0 and func_L00_0025E590 take */
    char pad116;
    unsigned char reaction_time; /* 0x117 */
    char pad118[8];
    float knock[4];             /* 0x120: record func_L00_0025D5B0 fills */
    float speed;                /* 0x130 */
    float acceleration;         /* 0x134 */
    float jump_speed;           /* 0x138 */
    float turn_speed;           /* 0x13C */
    int flags;                  /* 0x140 */
    int kind;                   /* 0x144 */
    float strength;             /* 0x148 */
    char pad14C[0x11];
    unsigned char pending;      /* 0x15D */
    char pad15E[0xE];
    float timer;                /* 0x16C */
    float f170;
    float f174;
    char pad178[8];
    float target[4];            /* 0x180 */
    char pad190[0x30];
    int i1C0;
    int i1C4;
    char pad1C8[8];
    float home[4];              /* 0x1D0 */
    char pad1E0[0x10];
    int path;                   /* 0x1F0: index into D_L16_001B0C30 */
} L16PursuitData;

typedef struct {
    char pad0[0x10];
    float position[4];          /* 0x10 */
    unsigned char state;        /* 0x20 */
    char pad21[0x13];
    unsigned short flags;       /* 0x34 */
    char pad36[0x42];
    L16PursuitData *data;       /* 0x78 */
    char pad7C[0x18];
    int i94;
    char pad98[0xC];
    unsigned char bA4;
} L16PursuitMoby;

extern char *D_L16_001B0930[];
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float D_L16_00161E58 __attribute__((sda));
extern float D_0015EE70;
extern float FUN_001f9b80(void*,void*);
extern float D_L16_00161E54 __attribute__((sda));
extern float D_L16_00161E5C __attribute__((sda));
extern float D_L16_00161E60 __attribute__((sda));
extern int FUN_L00_0025ff38(void *, void *, float, int, int, void *, int);
extern void FUN_L00_0025ab48(void *, float *, void *, void *);
extern void FUN_L00_0025c558(void *, void *, int, int, int, float);
extern void FUN_L00_0025d458(void *m, short *p);
extern void FUN_L00_0025d538(void *, void *);
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L16_002e6208(void *arg) {
    L16PursuitMoby *m = arg;
    L16PursuitData *d = m->data;
    char *hit;
    int a;                      /* the reaction kind, later the path */
    int b;                      /* the reaction flags, later the path's slot in the table */
    float scratch[8];           /* [0..3] where the hit came from, [4] the heading that gives */

    if (m->state == 0) {
        return;
    }
    hit = find_moby_hit(m, 0x330000, 0);
    if (hit) {
        b = 0x200;
        d->flags = b;
        d->strength = 0.5f;
        d->speed = D_L16_00161E54 * frame_time_sq;
        d->acceleration = D_L16_00161E58 * frame_time_sq;
        d->jump_speed = D_L16_00161E60 * frame_time;
        d->turn_speed = D_L16_00161E5C * frame_time;
        a = 0x29;
        d->kind = a;
        d->pending = 0;
        d->timer = frame_time * 2.0f;
        m->flags &= ~0x1000;
        *(OvlQuad *)scratch = *(OvlQuad *)(hit + 0x10);
        FUN_L00_0025ab48(scratch, scratch + 4, &d->jump_speed, &d->turn_speed);
        FUN_L00_0025c558(m, d->knock, 5, 1, 0, scratch[4]);
        d->f170 = 8.0f;
        d->f174 = 16.0f;
        m->state = 8;
        m->i94 = 0;
        d->reaction_time = 0x78;
        FUN_L00_0025d458(m, d->reaction);
    }
    m->bA4 = 0xFF;
    FUN_L00_0025d538(m, d->reaction);
    b = (int)&D_L16_001B0930[d->path];
    a = *(int *)b;
    if (FUN_L00_0025ff38(m, d->target, 16.0f, 0, 0, (void *)(a + 0x10), *(int *)a) != 2) {
        if (FUN_001f9b80(d->home, d->target) > 16.0f ||
            absolute_float(m->position[2] - d->target[2]) > 3.0f) {
            d->i1C4 = 2;
        }
    }
    if (d->i1C0 == 0) {
        struct Hero *h = &hero;
        d->i1C0 = (int)h->moby;
        qcopy(d->target, &h->motion.pos);
    }
}
/* Steers and moves a moby toward a target point, returning the distance. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7890), where it is exact; names translated to the US level program. */

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float frame_time, D_0015EE70;
extern float D_0015EE6C_u, D_0015ED70_c __asm__("D_0015ED70");
extern float FUN_001f9e90_c(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025be00_c(float *, float *, float, float, float,
                                float) __asm__("FUN_L00_0025be00");
extern void FUN_L00_00258830(void *, void *, float, float, float, int);
extern void FUN_L00_00261d78(float, void *, void *, void *);
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern float func_00214D88_7890(float *, float, float, float, float,
                                float *) __asm__("FUN_00213f38");

float FUN_L16_002e6428(void *m_v, void *t_v) {
    char *m = m_v;
    float *t = t_v;
    char *d = *(char **)(m + 0x78);
    float vel;
    float vec[4];
    float dist;
    float oldz;
    float a = FUN_001f9e90_c(t[0] - *(float *)(m + 0x10), t[1] - *(float *)(m + 0x14));
    a = fast_add_rotations(a, *(float *)(d + 0x1F8));
    oldz = *(float *)(m + 0x18);
    vel = 0.0f;
    dist = FUN_001f9b48(m + 0x10, t);
    FUN_L00_0025be00_c((float *)(m + 0x48), (float *)(d + 0x1F4), a, D_0015ED70_c * 12.566371f,
                       D_0015ED70_c * 12.566371f, frame_time * 12.566371f);
    func_00214D88_7890(&vel, dist, D_0015ED70_c * 8.0f, D_0015ED70_c * 12.0f, frame_time * 4.0f,
                       (float *)(d + 0x204));
    normalize_vector_xyz(vec, m + 0xC0, *(float *)(d + 0x204));
    vec[2] = *(float *)(d + 0x208) - D_0015ED70_c * 10.0f;
    FUN_L00_00258830(m, vec, 0.5f, 0.5f, 0.0f, 0x10);
    FUN_L00_00261d78(0.333f, *(void **)(d + 0x1F0), m + 0x10, m + 0x10);
    *(float *)(d + 0x208) = oldz - *(float *)(m + 0x18);
    return dist;
}

#include "qcopy.h"

/* Update the challenge NPC's interaction state and gaze. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E7C70), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[4];
    short previous;
    char pad6[2];
    unsigned char flag;
    char pad9[0x2D];
    short mode;
    int pending;
} L16ChallengeActivation;

typedef struct {
    char pad00[0x20];
    char activation[0x40];
    char pad60[0x64];
    float initial_yaw;
    int moving_timer, target_timer;
    float target[4];
    char body[0x60];
    char pad140[4];
    float pitch, yaw;
    char pad14C[4];
    float height;
    char pad154[0xC];
    char head[0x60];
    char pad1C0[8];
    float head_yaw;
} L16ChallengeData;

typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0xF];
    unsigned char opacity, visible;
    char pad32[0x16];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x24];
    L16ChallengeData *data;
    char pad7C[3];
    unsigned char distance_class;
    char pad80[0x3C];
    unsigned char event;
} L16ChallengeMoby;

typedef struct {
    char pad00[0x80];
    float position[4];
    char pad90[0x40];
    float aim[4];
    char padE0[0x20];
    float velocity[4];
    char pad110[0x798];
    int points;
    char pad8AC[0x17D8];
    int state;
    char pad2088[4];
    int mode;
} L16ChallengePlayer;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L16ChallengeScratch;

extern char D_0013D50F[], D_0014161B[];
char D_L16_001D9840[16] = {0, 0, 0xe, 0x43, 0x33, -77, -76, 0x42, 0, 0, -90, 0x42, 0, 0, 0, 0};
extern char D_L16_001671C0[];
char D_L16_001D9860[16] = {0x66, -26, -16, 0x42, -102, 0x79, -113, 0x43, 0, 0, -17, 0x42, 0, 0, 0, 0};
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b48(void *, void *);
extern float FUN_001f9b80(void *, void *);
extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *);
extern int freeze_stats_score_ref_b[] __asm__("D_0015EE6C") __attribute__((section(".sdata")));
extern volatile int D_0015EE6C_p __asm__("D_0015EE6C");
extern int D_L16_0015F5CC;
extern float D_L16_0015F638;
extern int D_0015EEA4;
extern int help_request(int, int) __asm__("FUN_L00_00203908");
extern int FUN_L00_00266448(void *, void *);
extern int FUN_L00_002668a0(void *, void *);
extern int allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");
extern int scale_game_frames(int);
extern int tick_countdown_32_c(void *) __asm__("tick_countdown_32");
extern unsigned char D_0013D4DE __attribute__((section(".data")));
extern unsigned char D_0013D507 __attribute__((section(".data")));
extern unsigned char D_0013D408[];
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00260860(int, int);
extern void FUN_L00_002628d8(float, float, void *, void *, int);
extern void FUN_L00_00263d40(int, int);
extern void FUN_L00_00299460(int);
extern void FUN_L16_002923d0(int);
extern void FUN_L01_002783a8(float, void *);
extern void FUN_L02_002e0cd8(void *);
extern void FUN_L16_002e70c0(struct Moby *);
extern void FUN_L16_002e7168(struct Moby *);
extern void add_vector_xyz(void *, void *, void *);
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void subtract_vector_xyz(void *, void *, void *);
extern char D_00141968[];
extern L16ChallengePlayer D_0013E633_challenge __asm__("D_0013E533");

void FUN_L16_002e6808(L16ChallengeMoby *m) {
    L16ChallengeScratch scratch;
    L16ChallengeData *d = m->data;
    L16ChallengeActivation *activation = (L16ChallengeActivation *)d->activation;
    float rate, head_rate;
    int tracking;
    if (m->visible && FUN_001f9b48(m->position, D_L16_001671C0) < 28.0f) {
        FUN_L00_0025a120(m);
        m->distance_class = 22;
    }
    FUN_L02_002e0cd8(m);
    switch (m->state) {
    case 0:
        m->opacity = 255;
        m->state = 1;
        FUN_L00_002668a0(m, activation);
        *(float *)((char *)d + 0x2C) = 3.7f;
        d->initial_yaw = m->yaw;
        if (*(int *)(D_0013D408 + 0x814) == 0)
            m->yaw = fast_add_rotations(m->yaw, 3.14159265f);
        FUN_L16_002e7168((char *)m);
        break;
    case 1: {
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode != 15)
            D_L16_0015F638 = 115.0f;
        else
            D_L16_0015F638 = 100.0f;
        if (FUN_L00_00266448(m, (char *)d + 0x20)) {
            m->yaw = d->initial_yaw;
            FUN_L01_002783a8(3.5f, m);
            m->state = 2;
        }
        if (!D_0013D4DE && FUN_001f9b80(&hero.motion.pos, m->position) < 3.5f) {
            int elapsed, period;
            char *statistics;
            elapsed = scale_game_frames(freeze_stats_score_ref_b[14]);
            period = 600;
            statistics = ((char *)&D_00141968);
            elapsed -= *(unsigned short *)(statistics + 0x43A) * period;
            if (elapsed > (int)((float)scale_game_frames(18) * 60.0f) ||
                *(unsigned short *)(statistics + 0x43A) * period == 0) {
                help_request(0x3E83, 0x87);
            } else if (scale_game_frames(D_0015EEA4) / 600 >
                       *(unsigned short *)(statistics + 0x43A)) {
                *(unsigned short *)(statistics + 0x43A) = scale_game_frames(D_0015EEA4) / 600;
            }
        }
        if (activation->mode == 3) {
            char *camera;
            char *pose;
            int pending;
            pending = D_L16_0015F5CC;
            activation->previous = (unsigned short)activation->mode;
            activation->mode = 2;
            activation->pending = pending;
            m->event = 0;
            pose = D_L16_001D9840;
            FUN_L00_00216f90(pose, pose + 0x10, 0, 1);
            camera = (char *)hero.unk1D00;
            qcopy(camera, pose + 0x20);
            qcopy(camera + 0x10, pose + 0x30);
            m->state = 3;
            FUN_L16_002e70c0((char *)m);
        }
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode == 22) {
            m->state = 3;
            FUN_L16_002e70c0((char *)m);
        }
        break;
    }
    case 2:
        if (*(int *)&D_L16_0015F5C4 != 2) {
            int previous;
            m->state = 1;
            previous = activation->previous;
            if (previous == 4) {
                FUN_L16_002923d0(0);
                m->state = previous;
            }
        }
        break;
    case 4:
        if (*(int *)&D_L16_0015F5C4 == 0) {
            m->state = 1;
            FUN_L00_00260860(31, 1);
            memcard_save_data(0, -1);
        }
        break;
    case 3:
    default: {
        L16ChallengePlayer *player = (L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D);
        unsigned char event;
        if (player->points >= 4500) {
            unsigned char *flags = D_0013D408;
            if (!flags[0x1A]) {
                flags[0x1A] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                FUN_L00_00263d40(0x53D6, -1);
            }
            if (D_0015EE6C_p < 4500)
                D_0015EE6C_p = player->points;
        }
        if (((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->mode == 0) {
            m->state = 1;
            FUN_L16_002e7168((char *)m);
        }
        event = m->event;
        if (event == 1) {
            m->state = event;
            FUN_L00_00216f90(D_L16_001D9860, D_L16_001D9860 + 0x10, 0, 1);
            m->event = 0;
            if (!D_0013D507) {
                int pending;
                unsigned short previous;
                previous = activation->mode;
                pending = D_L16_0015F5CC;
                activation->previous = previous;
                activation->mode = 4;
                activation->pending = pending;
                activation->flag = event;
            }
            FUN_L16_002e7168((char *)m);
        }
        if (m->event == 2) {
            m->state = 1;
            FUN_L00_00216f90(D_L16_001D9860, D_L16_001D9860 + 0x10, 0, 1);
            m->event = 0;
            FUN_L16_002e7168((char *)m);
        }
        if (m->event == 3) {
            char *camera;
            char *pose = D_L16_001D9840;
            m->event = 0;
            FUN_L00_00216f90(pose, pose + 0x10, 0, 1);
            camera = (char *)hero.unk1D00;
            qcopy(camera, pose + 0x20);
            qcopy(camera + 0x10, pose + 0x30);
        }
        break;
    }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (m->animation == 0) {
        char *player = (char *)&D_0013E633_challenge + 0xE9D;
        tracking = 1;
        if (FUN_001f9b80(m->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                m->yaw,
                FUN_001fa688_cf(
                    ((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->aim[0] -
                        m->position[0],
                    ((L16ChallengePlayer *)((char *)&D_0013E633_challenge + 0xE1D))->aim[1] -
                        m->position[1])) < 1.5707964f) {
            if (vector_length_xyz(player + 0x80) > 0.01f)
                d->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32_c(&d->moving_timer);
        } else if (d->moving_timer) {
            d->moving_timer = 0;
            qcopy(d->target, (char *)&D_0013E633_challenge + 0xEED);
        }
        if (tick_countdown_32_c(&d->target_timer)) {
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
            qcopy(scratch.target, (char *)&D_0013E633_challenge + 0xEED);
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
        d->yaw = yaw * 0.7f;
        d->head_yaw = yaw * 0.3f;
    }
    if (D_0015EDB0_b)
        *(float *)((char *)d + 0x150) = 2.75f;
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->body, 0);
    FUN_L00_002628d8(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->head, 1);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8538), where it is exact; names translated to the US level program. */

extern f32 D_L16_0015F638 ;
extern void FUN_L01_0027a248(int, int);
extern void FUN_L13_002650a8(int, int, int, int);
extern void FUN_L16_002de3b8(float f);

void FUN_L16_002e70c0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int *p = (int *)(data + 0x60);
    int i;
    for (i = 15; i >= 0; i--) {
        FUN_L13_002650a8(*p, 1, 1, 1);
        p++;
    }
    p = (int *)(data + 0xA0);
    for (i = 7; i >= 0; i--) {
        FUN_L13_002650a8(*p, 0, 0, 0);
        p++;
    }
    *(float *)&D_L16_0015F638 = 74.0f;
    FUN_L16_002de3b8(76.0f);
    FUN_L01_0027a248(2, 6);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E85E0), where it is exact; names translated to the US level program. */

void FUN_L16_002e7168(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int *p;
    int i;
    p = (int *)(data + 0x60);
    for (i = 15; i >= 0; i--) {
        FUN_L13_002650a8(p[0], 0, 0, 0);
        p++;
    }
    p = (int *)(data + 0xA0);
    for (i = 7; i >= 0; i--) {
        FUN_L13_002650a8(p[0], 1, 1, 0);
        p++;
    }
    FUN_L16_002de3b8(100.0f);
    FUN_L01_0027a248(0, 8);
}
#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern int func_00215570_e7208(void *, int) __asm__("FUN_00214720");
extern int func_0022EE28_e7208(int, int, int) __asm__("FUN_0022db10");
extern void func_L00_00264DB8_e7208(int, int) __asm__("FUN_L00_00263d40");
extern char D_0013D50F_e7208[] __asm__("D_0013D408");
void FUN_L16_002e7208(char *moby) {
    char *data = *(char **)(moby + 0x78);
    switch (*(int *)(data + 8)) {
    case 0: {
        char *g = (char *)&hero.motion.pos;
        if (func_00215570_e7208(g, *(int *)data) != 0) {
            if (*(int *)(g + 0x200C) == 0xF) {
                *(int *)(data + 8) = 1;
            }
        }
        break;
    }
    case 1: {
        struct Hero *h = &hero;
        if (h->state.current == 0x42) {
            *(int *)(data + 8) = 0;
        } else if (func_00215570_e7208(&h->motion.pos, *(int *)(data + 4)) != 0) {
            unsigned char *f = (unsigned char *)D_0013D50F_e7208;
            if (f[0x19] == 0) {
                f[0x19] = 1;
                func_0022EE28_e7208(1, 0, 0);
                func_L00_00264DB8_e7208(0x53D6, -1);
            }
        }
        break;
    }
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Update of a path walker: keeps its sound alive, follows the path behind its owner and jumps ahead while unobserved. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8B30), where it is exact; names translated to the US level program. */

typedef struct {
    int path_index;
    float parameter;
    float speed;
    float period;
    unsigned char *owner;
    float height_step;
    float target_speed;
    short countdown;
    short sound;
} L16WalkerData_c;

extern char *D_L16_001B0930[];
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float approach_value_alt(float *, float, float) __asm__("FUN_00213ed8");
extern float D_L16_00161E68 __attribute__((sda));
extern float D_L16_00161E6C __attribute__((sda));
extern float D_L16_00161E70 __attribute__((sda));
extern float D_L16_00161E74 __attribute__((sda));
extern float D_L16_00161E78 __attribute__((sda));
extern float D_L16_00161E7C __attribute__((sda));
extern float D_L16_00161E80 __attribute__((sda));
extern float D_L16_00161E8C __attribute__((sda));
extern int D_L16_00161E90 __attribute__((sda));
extern int FUN_001fa728(char *, float);
extern int FUN_L00_0028d8c0(void *, int);
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L16_002e7a30(unsigned char *, void *);
extern void FUN_L16_002e7ba0(struct Moby *);
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void sample_camera_path(void *, f32, s32, void *, void *, s32) __asm__("func_00214E58");

void FUN_L16_002e76b8(unsigned char *m) {
    float old[4], rotation[4], trial[4];
    char *path;
    L16WalkerData_c *d;
    int playing = 0;

    d = *(L16WalkerData_c **)(m + 0x78);
    path = D_L16_001B0930[d->path_index];
    if (((*(unsigned short *)(m + 0xA6) ^ 1) & 1) != 0) {
        if (FUN_L00_0028d8c0(m, d->sound) == 0) {
            d->sound = allocate_voice_for_target_entry_alt(0, 4, (int)m);
        } else {
            playing = 1;
        }
    } else {
        qcopy(old, m + 0x10);
    }
    switch (m[0x20]) {
    case 0:
        if (m[0x21] == 255) {
            mark_moby_for_removal(m);
            return;
        }
        FUN_L16_002e7ba0(m);
        break;
    case 1: {
        L16WalkerData_c *other = *(L16WalkerData_c **)(d->owner + 0x78);
        float gap, step, next, height, acceleration;
        /* `playing` again, in a variable of case 1's own. The logic does not need it: tested directly,
           `playing` is one value from the top of the function to the test, the compiler ranks it below
           `path` and gives them $s5 and $s4, the reverse of retail. The narrowing copy ends that value
           here and starts another; each ranks above `path`, both get $s4 and the copy itself leaves no
           instruction. A same-width copy (int) is folded away and does not do it. */
        char blocked = playing;

        sample_camera_path(path, d->parameter, 0, m + 0x10, (float *)(m + 0x40), 0);
        acceleration = D_L16_00161E78 * frame_time_sq;
        *(float *)(m + 0x18) += d->height_step;
        d->parameter += d->speed;
        approach_value_alt(&d->speed, d->target_speed, acceleration);
        gap = other->parameter - d->parameter;
        if (gap < 0.0f) {
            gap += d->period;
        }
        if (gap < D_L16_00161E68 && d->target_speed > other->target_speed) {
            float saved = d->target_speed;
            float theirs = other->target_speed;

            other->target_speed = saved;
            d->target_speed = theirs;
        }
        if (gap < 1.5f) {
            step = frame_time_sq * 50.0f;
            approach_value_alt(&d->speed, d->target_speed, step);
            approach_value_alt(&other->speed, other->target_speed, step);
        }
        if (m[0x31] == 0 && blocked == 0) {
            if (gap > D_L16_00161E6C + 1.0f) {
                height = random_float_between(D_L16_00161E7C, D_L16_00161E80);
                if (d->owner[0x31] != 0 && d->countdown != 0) {
                    next = other->parameter - random_float_between(D_L16_00161E68, D_L16_00161E6C);
                    d->countdown--;
                } else {
                    next = d->parameter + D_L16_00161E8C * frame_time;
                }
                if (next < 0.0f) {
                    next += d->period;
                }
                sample_camera_path(path, next, 0, trial, rotation, 1);
                trial[2] += height;
                trial[3] = 2.0f;
                if (FUN_001fa728((char *)trial, (float)D_L16_00161E90) == -1) {
                    float random = random_float_between(D_L16_00161E70, D_L16_00161E74);

                    d->parameter = next;
                    d->height_step = height;
                    d->target_speed = random * frame_time;
                }
            }
        } else {
            d->countdown = 30;
        }
        if (d->parameter > d->period) {
            d->parameter -= d->period;
        }
        break;
    }
    }
    if (*(unsigned short *)(m + 0xA6) & 1) {
        FUN_L16_002e7a30(m, old);
    }
}
/* Emits a particle near an active moby when the target is close. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8EA8), where it is exact; names translated to the US level program. */


extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int D_L16_0015F5CC;
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
void FUN_L00_0026ced0(OvlQuad *p, OvlQuad *q, int c, int d, int n, float s);
void build_spherical_offset_u(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
extern void func_L00_0026DD70_emit(void *, void *, int, int, float,
                                   int) __asm__("FUN_L00_0026ced0");

int D_L16_00161E98 = 0x60d0d080;
int D_L16_00161E9C = 0x801000;
extern float D_L16_00161EA0 __attribute__((sda));
extern float D_L16_00161EA4 __attribute__((sda));
extern float D_L16_00161EA8 __attribute__((sda));
void FUN_L16_002e7a30(unsigned char *m, void *v) {
    float input[4];
    float offset[4];
    float out[4];
    float velocity[4];
    float a, b, size, r, final_size;
    *(OvlQuad *)input = *(OvlQuad *)v;
    if ((unsigned char)m[0x31] == 0)
        return;
    if ((D_L16_0015F5CC & 1) != (((int)m >> 8) & 1))
        return;
    if (!(FUN_001f9b48(m + 0x10, D_L16_001671C0) < 75.0f))
        return;
    r = FUN_001f9b48(m + 0x10, input);
    scale_vector_xyz(out, m + 0xC0, D_L16_00161EA0 * r);
    size = D_L16_00161EA4;
    size *= frame_time;
    normalize_vector_xyz(offset, m + 0xC0, -0.5f);
    add_vector_xyz(offset, offset, m + 0x10);
    a = random_angle_radians();
    b = random_angle_radians();
    build_spherical_offset_u(velocity, size, a, b);
    add_vector_xyz(velocity, velocity, out);
    final_size = D_L16_00161EA8;
    final_size *= 210000.0f;
    func_L00_0026DD70_emit(offset, velocity, D_L16_00161E98, D_L16_00161E9C,
                           final_size, scale_game_frames(0x23));
}

#define NOT_SDA

#define MACRO_ADDR

/* Start of a walker group: lists the group's mobys and spaces them along the path in random order. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9018), where it is exact; names translated to the US level program. */

typedef struct {
    int path_index;
    float parameter;
    float speed;
    float period;
    unsigned char *owner;
    float height_step;
    float target_speed;
    short countdown;
    short sound;
} L16WalkerData;

extern char *D_L16_0015FFD8;
extern char *D_L16_001B0930[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float D_L16_00161E94 __attribute__((sda)); /* no foreign declaration */
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern short *D_L16_001ABCC0[];

void FUN_L16_002e7ba0(struct Moby *moby) {
    unsigned char *objects[16];
    unsigned char *previous;
    L16WalkerData *d = (L16WalkerData *)moby->pvars;
    short *list = D_L16_001ABCC0[moby->group];
    short count = 0, rank;
    int i;
    float period;

    if (list == 0 || d->path_index == -1) {
        return;
    }
    period = (float)*(int *)D_L16_001B0930[d->path_index] - 1.0f;
    do {
        objects[count] = (unsigned char *)(D_L16_0015FFD8 + ((*(unsigned short *)list & 0x7FFF) << 8));
        count++;
    } while (*list++ >= 0);
    previous = objects[count - 1];
    rank = count;
    for (i = 0; i < count; i++) {
        unsigned char *current;
        L16WalkerData *data;
        unsigned short flags;

        if (i == count - 1) {
            /* the last one takes the slot the loop index is on */
            current = objects[i];
        } else {
            /* the others take a random slot, or the next one still in use after it */
            int selected = random_integer_below(count - 1);

            for (;;) {
                if (objects[selected] != 0) {
                    current = objects[selected];
                    objects[selected] = 0;
                    break;
                }
                selected = (selected + 1) % (count - 1);
            }
        }
        flags = D_L16_00161E90;
        current[0x30] = 255;
        *(float *)(current + 0x2C) *= D_L16_00161E94;
        *(int *)(current + 0x94) = 0;
        *(unsigned short *)(current + 0x32) = flags;
        data = *(L16WalkerData **)(current + 0x78);
        data->parameter = (float)rank * (period / convert_integer_to_float(count));
        data->owner = previous;
        data->period = period;
        previous = current;
        data->height_step = random_float_between(D_L16_00161E7C, D_L16_00161E80);
        data->speed = random_float_between(D_L16_00161E70, D_L16_00161E74) * frame_time;
        data->target_speed = data->speed;
        current[0x20] = 1;
        rank--;
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Waits one tick, then pushes two pairs of the player's points apart along their difference. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9278), where it is exact; names translated to the US level program. */

typedef float W[4] __attribute__((aligned(16)));

extern char D_L16_0016CC60[];
extern int D_L16_0015F5C4;
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00263b70(void *, void *, void *, float, float);
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);

void FUN_L16_002e7e00(struct Moby *moby) {
    W a, b, c, d, e, f;
    float k1 = 0.075f, k2 = 20000.0f, k3 = 1000.0f;
    float j1 = 0.075f, j2 = 20000.0f, j3 = 1000.0f;
    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case 1:
        if (D_L16_0015F5C4 == 2) {
            char *g = D_L16_0016CC60;
            if (*(int *)(g + 0x30) == 0) {
                char *p = *(char **)(g + 0x184);
                moby_point_position(p, 6, a);
                moby_point_position(p, 4, c);
                subtract_vector_xyz(e, a, c);
                normalize_vector_xyz(e, e, k1);
                FUN_L00_00263b70(a, a, e, k2, k3);
                moby_point_position(p, 7, b);
                moby_point_position(p, 5, d);
                subtract_vector_xyz(f, b, d);
                normalize_vector_xyz(f, f, j1);
                FUN_L00_00263b70(b, b, f, j2, j3);
            }
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Moves the platform between two heights above its joint, with a particle while it moves. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E93F8), where it is exact; names translated to the US level program. */


typedef union {
    OvlQuad q;
    float v[4];
} L16PlatformVector;

typedef struct {
    char pad00[0x60];
    float basis[4][4];
    int joint;
    int padA4;
    float goal;
    int padAC;
    float speed;
    int particle;
} L16PlatformData;

typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0x1F];
    float rotation[4];
    char pad50[0x28];
    L16PlatformData *data;
    char pad7C[0x40];
    unsigned char reverse;
} L16PlatformMoby;

typedef struct {
    char pad00[0x80];
    float position[4];
    char pad90[0x26C];
    L16PlatformMoby *contact;
    char pad300[0xE];
    short grounded;
} L16PlatformHero;

extern char *D_L16_001600FC;
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float FUN_001f9b80(void *, void *);
extern float D_L16_00161EAC __attribute__((sda)); /* no foreign declaration */
extern float D_L16_00161EBC __attribute__((sda)); /* no foreign declaration */
extern int FUN_L00_0028d8c0(void *, int);
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern f32 D_L16_00161EB8_gp __asm__("D_L16_00161EB8") __attribute__((sda));
extern void FUN_L00_00260738(char *, void *, void *, void *);
extern void FUN_L16_002e84e8(struct Moby *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L16_002e7f80(L16PlatformMoby *moby) {
    L16PlatformVector delta, old_rotation, motion;
    L16PlatformData *d = moby->data;
    float *position = moby->position;
    float *rotation = moby->rotation;

    scale_vector_xyz(delta.v, position, -1.0f);
    qcopy(old_rotation.v, rotation);
    if (d) {
        switch (moby->state) {
        case 0:
            motion.q = 0;
            motion.v[2] = 1.0f;
            d->particle = -1;
            transform_vector_by_basis(motion.v, motion.v, D_L16_001600FC + (d->joint << 7));
            d->goal = motion.v[2] + *(float *)(D_L16_001600FC + d->joint * 0x80 + 0x38);
            moby->position[2] = motion.v[2] + *(float *)(D_L16_001600FC + d->joint * 0x80 + 0x38) + D_L16_00161EAC;
            moby->reverse = 1;
            moby->state = 2;
            break;
        case 2:
            if (FUN_L00_0028d8c0(moby, d->particle)) {
                release_voice_slot(d->particle);
                d->particle = -1;
            }
            if (FUN_001f9b80(position, hero.motion.pos.f) < 20.0f) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                transform_vector(motion.v, motion.v, D_L16_001600FC + (d->joint << 7));
                motion.v[2] = motion.v[2] + *(float *)(D_L16_001600FC + d->joint * 0x80 + 0x38) + D_L16_00161EAC;
                if (absolute_float(hero.motion.pos.f[2] - motion.v[2]) < 1.0f) {
                    moby->reverse = (moby->reverse + 1) & 1;
                    d->goal = motion.v[2];
                    d->speed = 0.0f;
                    moby->state = 3;
                    break;
                }
            }
            if (hero.ground_moby == moby && hero.air_frames.s == 0) {
                clear_u64_value(motion.v);
                if (moby->reverse) {
                    motion.v[2] = -1.0f;
                } else {
                    motion.v[2] = 1.0f;
                }
                moby->reverse = (moby->reverse + 1) & 1;
                transform_vector(motion.v, motion.v, D_L16_001600FC + (d->joint << 7));
                d->goal = motion.v[2] + *(float *)(D_L16_001600FC + d->joint * 0x80 + 0x38) + D_L16_00161EAC;
                d->speed = 0.0f;
                moby->state = 3;
            }
            break;
        case 3: {
            float *hero_position = hero.motion.pos.f;
            float speed;

            if (FUN_001f9b80(hero_position, D_L16_001600FC + (d->joint << 7) + 0x30) > 2.25f
                || absolute_float(moby->position[2] - hero_position[2] - 2.0f) > 1.0f
                || moby->position[2] < d->goal) {
                speed = advance_accelerated_scalar(&moby->position[2], &d->speed, d->goal,
                                      D_L16_00161EBC * frame_time_sq, D_L16_00161EBC * frame_time_sq,
                                      *(float *)&D_L16_00161EB8_gp * frame_time);
            } else {
                speed = advance_accelerated_scalar(&moby->position[2], &d->speed, moby->position[2],
                                      D_L16_00161EBC * 4.0f * frame_time_sq, D_L16_00161EBC * 4.0f * frame_time_sq,
                                      *(float *)&D_L16_00161EB8_gp * frame_time);
            }
            clear_u64_value(motion.v);
            motion.v[2] = speed;
            FUN_L00_00260738((char *)d->basis, motion.v, moby->rotation, moby->rotation);
            if (motion.v[2] != 0.0f) {
                if (!FUN_L00_0028d8c0(moby, d->particle)) {
                    d->particle = allocate_voice_for_target_entry_alt(0, 4, (int)moby);
                }
            } else if (FUN_L00_0028d8c0(moby, d->particle)) {
                release_voice_slot(d->particle);
                d->particle = -1;
            }
            if (moby->position[2] == d->goal && (hero.ground_moby != moby || hero.air_frames.s != 0)) {
                clear_u64_value(d->basis[1]);
                moby->state = 2;
                break;
            }
            if (d->speed != 0.0f) {
                enqueue_callback_list_1(FUN_L16_002e84e8, moby);
            }
            break;
        }
        }
        add_vector_xyz(delta.v, delta.v, moby->position);
        FUN_L00_00260738((char *)d->basis, delta.v, old_rotation.v, moby->rotation);
    }
}

#include "qcopy.h"

/* Draw stacked alternating rings with a rotating transformation and interpolated tint. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9960), where it is exact; names translated to the US level program. */

typedef float L16RingVector[4] __attribute__((aligned(16)));

typedef struct {
    L16RingVector position[4];
    int color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RingPacket;

typedef struct {
    L16RingVector basis[3];
    L16RingVector position;
} L16RingMatrix;

extern float absolute_float(float) __asm__("func_001F99C0");
extern float D_L16_001D9880[][2];
extern float fast_cos(float);
extern float fast_sin(float);
extern float wrap_angle(float);
extern int D_L16_0015F5CC;
extern float D_L16_00161EB8_gp __asm__("D_L16_00161EB8") __attribute__((sda));
extern float D_L16_00161EC4 __attribute__((sda));
extern float D_L16_00161EC8 __attribute__((sda));
extern float D_L16_00161EE8 __attribute__((sda));
int D_L16_00161ECC = 0;
int D_L16_00161ED0 = 2;
int D_L16_00161ED4 = 0;
int D_L16_00161ED8 = 1;
int D_L16_00161EDC = 0x408000;
int D_L16_00161EE0 = 0x40c08030;
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
extern void draw_geometry_quad(void *, void *, int);

void FUN_L16_002e84e8(struct Moby *m) {
    L16RingPacket packet;
    L16RingMatrix matrix;
    char *d = (char *)m->pvars;
    int bound_index = (*(int *)((char *)d + 0xA0)) << 7;
    char *entry = (char *)(bound_index + (int)D_L16_001600FC);
    float translation = (*(float *)((char *)d + 0xB8));
    float lower = (*(float *)((char *)entry + 0x38));
    float half = (*(float *)((char *)entry + 0x28));
    float upper = lower + half + translation;
    int color;
    float(*uv)[2];
    L16RingVector *point;
    float *v, *u;
    L16RingMatrix *transform;
    float radius, index, phase;
    int i, j, sign, period;
    lower = lower - half;
    lower += translation;
    color =
        fast_tween_color(D_L16_00161EDC, D_L16_00161EE0,
                     absolute_float((*(float *)((char *)d + 0xB0))) / (D_L16_00161EB8_gp * frame_time));
    copy_matrix_rotation(&matrix, &m->rot_row0);
    qcopy(matrix.position, &m->pos);
    matrix.position[2] = lower;
    matrix.position[3] = 1.0f;
    packet.texture = get_effect_texture(14);
    packet.flags = 0xFF9000000260L;
    packet.mode = (long)D_L16_00161ECC | ((long)D_L16_00161ED0 << 2) | ((long)D_L16_00161ED4 << 4) |
                  ((long)D_L16_00161ED8 << 6) | 0x8000000000L;
    packet.zero = 0;
    uv = D_L16_001D9880;
    point = packet.position;
    v = &packet.uv[0].v;
    u = &packet.uv[0].u;
    for (i = 0; i < 4; i++) {
        *u = (*uv)[0];
        *v = (*uv)[1];
        if (i & 1) {
            (*point)[2] = D_L16_00161EC8;
            radius = D_L16_00161EC4;
        } else {
            (*point)[2] = -D_L16_00161EC8;
            radius = D_L16_00161EC4 - 0.05f;
        }
        index = (float)(i >> 1);
        (*point)[0] = fast_cos(index * (D_L16_00161EE8 * DEG_TO_RAD)) * radius;
        (*point)[1] = fast_sin(index * (D_L16_00161EE8 * DEG_TO_RAD)) * radius;
        (*point)[3] = 1.0f;
        v += 2;
        u += 2;
        uv++;
        point++;
    }
    transform = &matrix;
    sign = 1;
    if (matrix.position[2] < upper) {
        period = 120;
        do {
            phase = (float)(D_L16_0015F5CC % period) * 0.02617991715669632f;
            sign = -sign;
            for (j = 0; (float)j < 360.0f / D_L16_00161EE8; j++) {
                matrix.basis[0][0] = fast_cos(
                    wrap_angle((D_L16_00161EE8 * DEG_TO_RAD) * (float)j + phase * (float)sign));
                matrix.basis[0][1] = fast_sin(
                    wrap_angle((D_L16_00161EE8 * DEG_TO_RAD) * (float)j + phase * (float)sign));
                matrix.basis[0][2] = 0.0f;
                matrix.basis[1][0] = fast_cos(wrap_angle(
                    (D_L16_00161EE8 * DEG_TO_RAD) * (float)j + 1.5707964f + phase * (float)sign));
                matrix.basis[1][1] = fast_sin(wrap_angle(
                    (D_L16_00161EE8 * DEG_TO_RAD) * (float)j + 1.5707964f + phase * (float)sign));
                matrix.basis[1][2] = 0.0f;
                matrix.basis[2][2] = 1.0f;
                packet.color[3] = color;
                packet.color[2] = color;
                packet.color[1] = color;
                packet.color[0] = color;
                draw_geometry_quad(&packet, transform, 0);
            }
            matrix.position[2] += (*(float *)((char *)d + 0xBC));
        } while (matrix.position[2] < upper);
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9D48), where it is exact; names translated to the US level program. */

void FUN_L16_002e88d0(struct Moby *moby) {
    switch (moby->state) {
    case 0: {
        float size = moby->scale * 4.0f;
        int ff = 0xFF;
        int twohundred = 0x200;
        moby->state = 1;
        moby->update_dist = ff;
        moby->draw_dist = twohundred;
        moby->pos.z = 160.0f;
        moby->scale = size;
    } break;
    case 1:
        moby->rot.z =
            FUN_001fa580(moby->rot.z, frame_time * -0.2617993950843811f);
        break;
    }
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9DE8), where it is exact; names translated to the US level program. */

extern char *D_L16_0015FFD8;
char *FUN_L16_002e8b08(int idx);
extern int FUN_001f96f8(int);
extern int FUN_001f9740_c(void *) __asm__("FUN_001f9740");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L16_002e0e28(void *);
extern void FUN_L16_002c44f0(void *, void *);
extern void FUN_L16_002e0de0(void *);
f32 probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
s32 is_point_inside_clip_volume_u(s32 arg0, s32 arg1) __asm__("FUN_00214720");

void FUN_L16_002e8970(struct Moby *moby) {
    char *g = (char *)&hero.motion.pos;
    char *data = (char *)moby->pvars;
    char *target;
    int n;
    float pos[4];
    if (!is_point_inside_clip_volume_u(g, *(int *)(data + 8)))
        return;
    target = D_L16_0015FFD8 + (*(int *)(data + 4) << 8);
    switch (moby->state) {
    case 0:
        moby->state = 1;
        *(int *)(data + 0xC) = FUN_001f96f8(random_int_between(0xB4, 0x168));
        break;
    case 1:
        if (FUN_001f9b80(g, target + 0x10) > 3.0f) {
            if (FUN_L16_002e8b08(*(int *)data) != 0) {
                if (FUN_001f9740_c(data + 0xC)) {
                    moby->state = 2;
                    FUN_L16_002e0de0(target);
                    *(int *)(data + 0xC) = FUN_001f96f8(random_int_between(0xB4, 0x168));
                }
            }
        }
        break;
    case 2:
        n = FUN_L16_002e0e28(target);
        switch (n) {
        case 0:
            moby->state = 1;
            break;
        case 2:
            moby->state = 1;
            {
                char *other = FUN_L16_002e8b08(*(int *)data);
                if (other != 0) {
                    qcopy(pos, target + 0x10);
                    pos[2] += 1.0f;
                    pos[2] = probe_ground_height(pos, 0, 0.5f);
                    FUN_L16_002c44f0(other, pos);
                }
            }
            break;
        }
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9F80), where it is exact; names translated to the US level program. */

extern char *D_L16_0015FFD8;
extern int FUN_L16_002c4710(void *);
extern short *D_L16_001ABCC0[];

char *FUN_L16_002e8b08(int idx) {
    short *p = D_L16_001ABCC0[idx];
    char *moby;
    if (p == 0)
        return 0;
    for (;;) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)p & 0x7FFF) << 8);
        if (*(short *)(moby + 0xA6) == 0x10E && FUN_L16_002c4710(moby))
            return moby;
        if (*p++ < 0)
            return 0;
    }
}
/* For each of n point pairs, reflects a direction and writes its 2D map coordinates. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA010), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z;
} Vec3f;

extern char D_L16_001671C0[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9a28(void *, void *, void *);
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void FUN_L16_002e8b98(int n, Vec3f *p1, Vec3f *p2, int unused, float *out) {
    int i;
    float a[4];
    float v[4];
    float t[4];
    float w[4];

    for (i = 0; i < n; i++) {
        float d;
        float len;

        v[3] = 1.0f;
        v[0] = p2[i].x;
        v[1] = p2[i].y;
        v[2] = p2[i].z;
        FUN_001f9a28(a, v, D_L16_001671C0);
        fast_vec_normalize(a, a, 1.0f);
        w[3] = 1.0f;
        w[0] = p1[i].x;
        w[1] = p1[i].y;
        w[2] = p1[i].z;
        fast_vec_normalize(w, w, 1.0f);
        d = fast_vec_dot(w, a);
        fast_vec_scale(t, w, d + d);
        FUN_001f9a28(t, a, t);
        fast_vec_normalize(t, t, 1.0f);
        t[2] = t[2] + 1.0f;
        len = fast_vec_length(t);
        len = len + len;
        out[i * 2] = (t[0] / len + 0.5f) * 2.0f;
        out[i * 2 + 1] = (t[1] / len + 0.5f) * 2.0f;
    }
}
/* Sets up the draw state, then draws the three entries of the level's table. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA1B8), where it is exact; names translated to the US level program. */

extern char D_L16_001DC600[][16] __attribute__((section(".data")));
char D_L16_001DC630[5712] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, 0x67, -40, 0, 0x43, -119, -2, 9, 0x43, 0x52, -105, 2, 0x43, 0x67, -40, 0, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, -119, -2, 9, 0x43, 0x52, -105, 2, 0x43, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, -119, -2, 9, 0x43, 0x52, -105, 2, 0x43, -76, 0x4a, 8, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, -76, 0x4a, 8, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, -76, 0x4a, 8, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -113, 0x43, 0, 0x43, 0x67, -40, 0, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, -115, -111, 4, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, 0x67, -40, 0, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, -115, -111, 4, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, 0x67, -40, 0, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, -115, -111, 4, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, 0xa, -15, 6, 0x43, 0x50, 0x5c, -6, 0x42, -76, 0x4a, 8, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, -76, 0x4a, 8, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, -76, 0x4a, 8, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, -115, -111, 4, 0x43, -30, 0x78, 6, 0x43, 0, 0, -12, 0x42, 0x67, -40, 0, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, -115, -111, 4, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, 0x67, -40, 0, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, 0x67, -40, 0, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -109, -13, 6, 0x43, 0x69, -122, -19, 0x42, -76, 0x4a, 8, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, -76, 0x4a, 8, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, -76, 0x4a, 8, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, -115, -111, 4, 0x43, -113, 0x26, 8, 0x43, -30, 0x78, -25, 0x42, 0x67, -40, 0, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, -115, -111, 4, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, 0x67, -40, 0, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, -115, -111, 4, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, 0x67, -40, 0, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, -115, -111, 4, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, -113, 0x2d, 0xa, 0x43, -85, 0x5b, -30, 0x42, -76, 0x4a, 8, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, -76, 0x4a, 8, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, -76, 0x4a, 8, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, -115, -111, 4, 0x43, 0x71, -68, 0xc, 0x43, 0x1d, 0x4d, -34, 0x42, 0x67, -40, 0, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, -115, -111, 4, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, 0x67, -40, 0, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, -115, -111, 4, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, 0x67, -40, 0, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, -115, -111, 4, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, -115, -111, 4, 0x43, 4, -58, 0xf, 0x43, -2, -25, -37, 0x42, -76, 0x4a, 8, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, -115, -111, 4, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, -76, 0x4a, 8, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, -115, -111, 4, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, -76, 0x4a, 8, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, -115, -111, 4, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, -115, -111, 4, 0x43, 0, 0, 0x13, 0x43, -59, -15, -38, 0x42, 0x67, -40, 0, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, -115, -111, 4, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, 0x67, -40, 0, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, -115, -111, 4, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, 0x67, -40, 0, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, -115, -111, 4, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, -53, 0x3c, 0x16, 0x43, 0x26, -25, -37, 0x42, -76, 0x4a, 8, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, -76, 0x4a, 8, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, -76, 0x4a, 8, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, -115, -111, 4, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, -115, -111, 4, 0x43, -114, 0x43, 0x19, 0x43, 0x1d, 0x4d, -34, 0x42, 0x67, -40, 0, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, -115, -111, 4, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, 0x67, -40, 0, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, 0x67, -40, 0, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -20, -39, 0x1b, 0x43, 0x19, 0x6e, -30, 0x42, -76, 0x4a, 8, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, -76, 0x4a, 8, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, -76, 0x4a, 8, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -30, 0x78, -25, 0x42, 0x67, -40, 0, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, 0x67, -40, 0, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, -115, -111, 4, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, 0x67, -40, 0, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, -115, -111, 4, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, 0x6b, -117, -19, 0x42, -76, 0x4a, 8, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, -76, 0x4a, 8, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, -108, 0x74, -6, 0x42, -76, 0x4a, 8, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, -108, 0x74, -6, 0x42, -115, -111, 4, 0x43, 0x1d, -121, 0x1f, 0x43, 0, 0, -12, 0x42, 0x67, -40, 0, 0x43, -89, 0xd, 0x1f, 0x43, -108, 0x74, -6, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, -108, 0x74, -6, 0x42, 0x67, -40, 0, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, 0x67, -40, 0, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0x76, 1, 0x1c, 0x43, 0x52, -105, 2, 0x43, 0x67, -40, 0, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0x76, 1, 0x1c, 0x43, 0x52, -105, 2, 0x43, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0x76, 1, 0x1c, 0x43, 0x52, -105, 2, 0x43, -76, 0x4a, 8, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, -115, -111, 4, 0x43, 0x71, -39, 0x1d, 0x43, -113, 0x43, 0, 0x43, -76, 0x4a, 8, 0x43, -89, 0xd, 0x1f, 0x43, -108, 0x74, -6, 0x42, -115, -111, 4, 0x43, -89, 0xd, 0x1f, 0x43, -108, 0x74, -6, 0x42, -76, 0x4a, 8, 0x43, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, 0x48, -87, 0x48, -65, -11, -9, 0x1e, 0x3f, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, 0x48, -87, 0x48, -65, -11, -9, 0x1e, 0x3f, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, 0x48, -87, 0x48, -65, -11, -9, 0x1e, 0x3f, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, 0x6b, 0x7f, 0x5b, -65, 5, 0x6b, -4, 0x3e, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -85, -53, 0x75, -65, 0x63, -45, -126, 0x3e, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x75, -54, -125, -66, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x75, -54, -125, -66, 0, 0, 0, 0, -98, 0x3f, 0x7d, -65, 0x5d, -93, 0x65, -71, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x75, -54, -125, -66, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x53, -54, -125, -66, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x75, -54, -125, -66, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x75, -54, -125, -66, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, -124, -98, 0x75, -65, 0x75, -54, -125, -66, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, -78, 0x4c, 0x5b, -65, -55, 0x58, -3, -66, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x17, -44, 0x33, -65, -119, -47, 0x33, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x56, -13, -4, -66, 0x28, 0x60, 0x5b, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x59, -94, -125, -66, 0x60, -82, 0x75, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x74, -74, 0, -70, -67, 0x38, 0x7d, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x32, -54, -125, 0x3e, -124, -98, 0x75, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, 0x1f, -9, -3, 0x3e, -73, 0xd, 0x5b, -65, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, 0x39, 0x25, 0x34, 0x3f, 0x69, -113, 0x33, -65, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, -11, 0x4c, 0x5b, 0x3f, 0x20, 0x60, -3, -66, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, -66, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, 0x3e, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, 0x3e, 0, 0, 0, 0, 0x39, 0x47, 0x7d, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, 0x3e, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, 0x3e, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x26, -87, 0x48, 0x3f, 0x27, -8, 0x1e, 0x3f, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x26, -87, 0x48, 0x3f, 0x27, -8, 0x1e, 0x3f, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x26, -87, 0x48, 0x3f, 0x27, -8, 0x1e, 0x3f, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x37, 0x51, 0x5b, 0x3f, 0x61, 0x55, -3, 0x3e, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, 0x3e, 0, 0, 0, 0, 0x38, -96, 0x75, 0x3f, 0x42, -104, -125, 0x3e, 0, 0, 0, 0, 0, 0, 0, 0, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0, 0, 0, 0};
extern int FUN_001fa728(char *, float);
extern int get_effect_texture_l16(int) __asm__("FUN_001f44b8");
int D_L16_00161EF0_gp[4] __asm__("D_L16_00161EF0") __attribute__((section(".sdata"))) = {148, 148, 25, 0};
extern float D_L16_001D98D0[];
extern float D_L16_001D9FC0[];
extern float D_L16_001DA6B0[];
float *D_L16_00161F00_gp[4] __asm__("D_L16_00161F00") __attribute__((section(".sdata"))) = {D_L16_001D98D0, D_L16_001D9FC0, D_L16_001DA6B0, 0};
extern float D_L16_001DA7E0[];
extern float D_L16_001DAC80[];
extern float D_L16_001DB120[];
float *D_L16_00161F10[4] __attribute__((section(".sdata"))) = {D_L16_001DA7E0, D_L16_001DAC80, D_L16_001DB120, 0};
extern float D_L16_001DB1E8[];
extern float D_L16_001DB8D8[];
extern float D_L16_001DBFC8[];
float *D_L16_00161F20[4] __attribute__((section(".sdata"))) = {D_L16_001DB1E8, D_L16_001DB8D8, D_L16_001DBFC8, 0};
extern float D_L16_001DC0F8[];
extern float D_L16_001DC348[];
extern float D_L16_001DC598[];
float *D_L16_00161F30[4] __attribute__((section(".sdata"))) = {D_L16_001DC0F8, D_L16_001DC348, D_L16_001DC598, 0};
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void FUN_L00_001fde98(int, int, int, char *, int);
extern void FUN_L16_002e8b98_i(int, int, int, int, char *) __asm__("FUN_L16_002e8b98");
extern void vu1_add_g_sregister_l16(int, long) __asm__("FUN_00233980");

void FUN_L16_002e8d40(void) {
    int *a = (int *)D_L16_00161EF0_gp;
    int *b = (int *)D_L16_00161F00_gp;
    int *c = (int *)D_L16_00161F10;
    int *d = (int *)D_L16_00161F20;
    int *e = (int *)D_L16_00161F30;
    int i;

    vu1_add_g_sregister_l16(6, get_effect_texture_l16(0x29));
    vu1_add_g_sregister_l16(0x42, 0x4000000064);
    vu1_add_g_sregister_l16(8, 0);
    vu1_add_g_sregister_l16(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        if (FUN_001fa728(D_L16_001DC600[i], 512.0f) != -1) {
            FUN_L16_002e8b98_i(a[i], d[i], b[i], c[i], D_L16_001DC630);
            FUN_L00_001fde98(a[i], b[i], e[i], D_L16_001DC630, 1);
        }
    }
}
/* Bounding spheres for one mesh (three entries), then the renderer callback; same source as FUN_L16_002e93b0 without the 0x30/0x32 stores. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2e8e80;

void enqueue_callback_list_1_2e8e80(int arg0, int arg1) __asm__("FUN_001f4600");
extern int D_L16_00161EF0_gp[] __asm__("D_L16_00161EF0") __attribute__((sda));
extern float *D_L16_00161F00_gp[] __asm__("D_L16_00161F00") __attribute__((sda));
extern L16SingleSphere_2e8e80 D_L16_00161F60_bounds_2e8e80[] __asm__("D_L16_001DC600")
    __attribute__((section(".data")));

extern void FUN_L16_002e8d40_c(void *) __asm__("FUN_L16_002e8d40");
extern void FUN_L16_002e97a0(void);
void FUN_L16_002e8e80(struct Moby *m) {
    int i, j;
    float maxx, maxy, maxz, minx, miny, minz;
    float p[4];
    float *out;
    float *verts;
    int count;
    switch (m->state) {
    case 0: {
        for (i = 0; i <= 2; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00161EF0_gp[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00161F00_gp)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2e8e80 *base = D_L16_00161F60_bounds_2e8e80;
                L16SingleSphere_2e8e80 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00161EF0_gp[i]; j++) {
                float dist;
                out = (float *)(D_L16_00161F60_bounds_2e8e80 + i);
                p[0] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 0];
                p[1] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 1];
                p[2] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 2];
                dist = FUN_001f9b48(p, out);
                if (dist > out[3])
                    out[3] = dist;
            }
        }
        m->state = 1;
        break;
    }
    case 1:
        enqueue_callback_list_1_2e8e80(FUN_L16_002e8d40_c, m);
        break;
    }
}
/* Sets up the draw state, then draws the one entry of the level's table; same source as FUN_L16_002e8d40. */

extern char D_L16_00161F80_t[][16] __asm__("D_L16_00161F80") __attribute__((section(".sdata")));
extern int get_effect_texture_2e9270(int) __asm__("FUN_001f44b8");
extern void FUN_L16_002e8b98_t(int, int, int, int, char *) __asm__("FUN_L16_002e8b98");
extern void vu1_add_g_sregister_2e9270(int, long) __asm__("FUN_00233980");

char D_L16_001DDC80[5680] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xa, -95, 0x7d, 0x43, 0x18, 0x50, 0x69, 0x43, -128, 0x3b, 2, 0x43, -33, -14, -112, 0x41, 0x45, 0x5f, 0x77, 0x43, 0x6a, 0x64, 0x78, 0x43, -41, -9, 1, 0x43, 0x11, -17, -78, 0x40, 0x2b, -56, -125, 0x43, -35, -46, 0x76, 0x43, -29, -105, -3, 0x42, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, -29, -105, -3, 0x42, 0x2b, -56, -125, 0x43, -35, -46, 0x76, 0x43, -128, 0x3b, 2, 0x43, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, -128, 0x3b, 2, 0x43, 0x2b, -56, -125, 0x43, -35, -46, 0x76, 0x43, 0xf, -85, 5, 0x43, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, -128, 0x3b, 2, 0x43, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, 0xf, -85, 5, 0x43, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, -128, 0x3b, 2, 0x43, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, 0xf, -85, 5, 0x43, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, -128, 0x3b, 2, 0x43, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, -128, 0x3b, 2, 0x43, 0x52, -5, -125, 0x43, -4, 0x1c, 0x74, 0x43, -29, -105, -3, 0x42, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, -128, 0x3b, 2, 0x43, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, -29, -105, -3, 0x42, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, -128, 0x3b, 2, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, -29, -105, -3, 0x42, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, -128, 0x3b, 2, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, -128, 0x3b, 2, 0x43, -18, -67, -125, 0x43, 0x35, -35, 0x70, 0x43, 0xf, -85, 5, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, -128, 0x3b, 2, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, 0xf, -85, 5, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, -128, 0x3b, 2, 0x43, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, 0xf, -85, 5, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, -128, 0x3b, 2, 0x43, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, -128, 0x3b, 2, 0x43, -42, -38, -125, 0x43, 0x35, 0x22, 0x6e, 0x43, -29, -105, -3, 0x42, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, -128, 0x3b, 2, 0x43, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, -29, -105, -3, 0x42, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, -128, 0x3b, 2, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, -29, -105, -3, 0x42, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, -128, 0x3b, 2, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, -128, 0x3b, 2, 0x43, 0x6d, -127, -125, 0x43, 0x7d, -66, 0x6a, 0x43, 0xf, -85, 5, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, -128, 0x3b, 2, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, 0xf, -85, 5, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, -128, 0x3b, 2, 0x43, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, 0xf, -85, 5, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, -128, 0x3b, 2, 0x43, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, -128, 0x3b, 2, 0x43, -37, 4, -125, 0x43, -16, 0x43, 0x68, 0x43, -29, -105, -3, 0x42, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, -128, 0x3b, 2, 0x43, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, -29, -105, -3, 0x42, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, -128, 0x3b, 2, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, -29, -105, -3, 0x42, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, -128, 0x3b, 2, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, -128, 0x3b, 2, 0x43, 0x5d, 0x10, -126, 0x43, 0x45, -40, 0x65, 0x43, 0xf, -85, 5, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, -128, 0x3b, 2, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, 0xf, -85, 5, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, -128, 0x3b, 2, 0x43, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, 0xf, -85, 5, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, -128, 0x3b, 2, 0x43, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, -128, 0x3b, 2, 0x43, -76, -23, -128, 0x43, -60, 3, 0x64, 0x43, -29, -105, -3, 0x42, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, -128, 0x3b, 2, 0x43, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, -29, -105, -3, 0x42, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, -128, 0x3b, 2, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, -29, -105, -3, 0x42, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, -128, 0x3b, 2, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, -128, 0x3b, 2, 0x43, 0x1e, 8, 0x7f, 0x43, 0x13, -27, 0x62, 0x43, 0xf, -85, 5, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, -128, 0x3b, 2, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, 0xf, -85, 5, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, -128, 0x3b, 2, 0x43, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, 0xf, -85, 5, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, -128, 0x3b, 2, 0x43, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, -128, 0x3b, 2, 0x43, 0x79, 0xd, 0x7c, 0x43, -119, 0x70, 0x62, 0x43, -29, -105, -3, 0x42, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, -128, 0x3b, 2, 0x43, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, -29, -105, -3, 0x42, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, -128, 0x3b, 2, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, -29, -105, -3, 0x42, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, -128, 0x3b, 2, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, -128, 0x3b, 2, 0x43, -52, 5, 0x79, 0x43, -60, -32, 0x62, 0x43, 0xf, -85, 5, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, -128, 0x3b, 2, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, 0xf, -85, 5, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, -128, 0x3b, 2, 0x43, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, 0xf, -85, 5, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, -128, 0x3b, 2, 0x43, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, -128, 0x3b, 2, 0x43, -16, 0x43, 0x76, 0x43, 0x4a, -10, 0x63, 0x43, -29, -105, -3, 0x42, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, -128, 0x3b, 2, 0x43, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, -29, -105, -3, 0x42, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, -128, 0x3b, 2, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, -29, -105, -3, 0x42, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, -128, 0x3b, 2, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, -128, 0x3b, 2, 0x43, -9, -13, 0x73, 0x43, -108, -61, 0x65, 0x43, 0xf, -85, 5, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, -128, 0x3b, 2, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, 0xf, -85, 5, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, -128, 0x3b, 2, 0x43, -40, -25, 0x70, 0x43, 0x57, -14, 0x6a, 0x43, 0xf, -85, 5, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, -128, 0x3b, 2, 0x43, -40, -25, 0x70, 0x43, 0x57, -14, 0x6a, 0x43, -128, 0x3b, 2, 0x43, -61, 3, 0x72, 0x43, -104, 0x2c, 0x68, 0x43, -29, -105, -3, 0x42, -40, -25, 0x70, 0x43, 0x57, -14, 0x6a, 0x43, -128, 0x3b, 2, 0x43, -40, -25, 0x70, 0x43, 0x57, -14, 0x6a, 0x43, -29, -105, -3, 0x42, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, -128, 0x3b, 2, 0x43, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, -29, -105, -3, 0x42, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, -128, 0x3b, 2, 0x43, -82, -45, 0x70, 0x43, -36, -88, 0x70, 0x43, -29, -105, -3, 0x42, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, -128, 0x3b, 2, 0x43, -82, -45, 0x70, 0x43, -36, -88, 0x70, 0x43, -128, 0x3b, 2, 0x43, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, -128, 0x3b, 2, 0x43, -82, -45, 0x70, 0x43, -36, -88, 0x70, 0x43, 0xf, -85, 5, 0x43, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, -128, 0x3b, 2, 0x43, -119, 0x70, 0x70, 0x43, -122, -14, 0x6d, 0x43, 0xf, -85, 5, 0x43, -40, -25, 0x70, 0x43, 0x57, -14, 0x6a, 0x43, -128, 0x3b, 2, 0x43, -40, -25, 0x70, 0x43, 0x57, -14, 0x6a, 0x43, 0xf, -85, 5, 0x43, -49, -5, 0x74, 0x43, -48, 0x17, 0x77, 0x43, -29, -105, -3, 0x42, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, -29, -105, -3, 0x42, -49, -5, 0x74, 0x43, -48, 0x17, 0x77, 0x43, -128, 0x3b, 2, 0x43, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, -128, 0x3b, 2, 0x43, -49, -5, 0x74, 0x43, -48, 0x17, 0x77, 0x43, 0xf, -85, 5, 0x43, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, -128, 0x3b, 2, 0x43, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, 0xf, -85, 5, 0x43, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, -128, 0x3b, 2, 0x43, -116, -17, 0x79, 0x43, 0x5c, 0x4c, 0x79, 0x43, 0xf, -85, 5, 0x43, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, -128, 0x3b, 2, 0x43, -116, -17, 0x79, 0x43, 0x5c, 0x4c, 0x79, 0x43, -128, 0x3b, 2, 0x43, 0xf, 0x4c, 0x77, 0x43, -117, -113, 0x78, 0x43, -29, -105, -3, 0x42, -116, -17, 0x79, 0x43, 0x5c, 0x4c, 0x79, 0x43, -29, -105, -3, 0x42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0x18, 0x43, 0x7d, 0x3f, -39, 0x5c, 0x15, 0x3e, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x18, 0x43, 0x7d, 0x3f, -39, 0x5c, 0x15, 0x3e, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x18, 0x43, 0x7d, 0x3f, -39, 0x5c, 0x15, 0x3e, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x4e, 0x42, 0x7d, 0x3f, 0x28, 0x29, -80, -72, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, 0x40, 0x31, 0x7e, 0x3f, -48, 0x42, 2, -67, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, 0x18, -18, 0x7c, 0x3f, 0x54, -113, 0x74, -67, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, -32, -126, 0x74, 0x3f, -42, 0x3a, -111, -66, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, -24, -96, 0x5b, 0x3f, -127, 0x24, -4, -66, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, 0x31, 0x23, 0x34, 0x3f, -52, 0x7c, 0x33, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, -50, -116, -2, 0x3e, -21, 1, 0x5b, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, 0x3a, -23, -123, 0x3e, -34, 0x57, 0x75, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, 0x3b, -57, -128, 0x3b, 0x50, 0x36, 0x7d, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, -126, 0x54, -126, -66, -26, -51, 0x75, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x3e, 6, -5, -66, -66, 0x16, 0x5c, -65, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, 0x4c, 0x6c, 0x32, -65, 0x73, 0x2e, 0x35, -65, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, -50, 0x53, 0x75, -65, -18, 0x5e, -122, -66, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, -50, 0x53, 0x75, -65, -18, 0x5e, -122, -66, 0, 0, 0, 0, 0x29, -105, 0x5a, -65, 0x7b, -125, -1, -66, 0, 0, 0, 0, -50, 0x53, 0x75, -65, -18, 0x5e, -122, -66, 0, 0, 0, 0, -50, 0x53, 0x75, -65, -18, 0x5e, -122, -66, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -12, 0x6d, 0x7d, -65, -28, -65, 0x10, 0x3e, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -12, 0x6d, 0x7d, -65, -28, -65, 0x10, 0x3e, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -12, 0x6d, 0x7d, -65, -28, -65, 0x10, 0x3e, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -58, 0x32, 0x7d, -65, 0x30, -126, -58, -69, 0, 0, 0, 0, -50, 0x53, 0x75, -65, -18, 0x5e, -122, -66, 0, 0, 0, 0, -50, 0x53, 0x75, -65, -18, 0x5e, -122, -66, 0, 0, 0, 0, -60, 0x23, 9, -65, 0xf, 0x2b, 0x58, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, -60, 0x23, 9, -65, 0xf, 0x2b, 0x58, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, -60, 0x23, 9, -65, 0xf, 0x2b, 0x58, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, 0x5e, -43, -119, -66, -117, -116, 0x76, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, 0x5e, -43, -119, -66, -100, -116, 0x76, 0x3f, 0, 0, 0, 0, 0x73, 0xe, -50, -66, -43, 0x5b, 0x67, 0x3f, 0, 0, 0, 0, 0x5e, -43, -119, -66, -100, -116, 0x76, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0, 0, 0, 0};
int D_L16_00161F40[2] = {125, 0};
extern float D_L16_001DCAE0[];
float *D_L16_00161F60[2] = {D_L16_001DCAE0, 0};
extern float D_L16_001DD0C0[];
float *D_L16_00161F68[2] = {D_L16_001DD0C0, 0};
extern float D_L16_001DD4A8[];
float *D_L16_00161F70[2] = {D_L16_001DD4A8, 0};
extern float D_L16_001DDA88[];
float *D_L16_00161F78[2] = {D_L16_001DDA88, 0};
void FUN_L16_002e9270(void) {
    int *a = (int *)D_L16_00161F40;
    int *b = (int *)D_L16_00161F60;
    int *c = (int *)D_L16_00161F68;
    int *d = (int *)D_L16_00161F70;
    int *e = (int *)D_L16_00161F78;
    int i;

    vu1_add_g_sregister_2e9270(6, get_effect_texture_2e9270(0x29));
    vu1_add_g_sregister_2e9270(0x42, 0x4000000064);
    vu1_add_g_sregister_2e9270(8, 0);
    vu1_add_g_sregister_2e9270(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 1; i++) {
        if (FUN_001fa728(D_L16_00161F80_t[i], 512.0f) != -1) {
            FUN_L16_002e8b98_t(a[i], d[i], b[i], c[i], D_L16_001DDC80);
            FUN_L00_001fde98(a[i], b[i], e[i], D_L16_001DDC80, 1);
        }
    }
}
/* Compute bounding spheres for one mesh and register their renderer. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA828), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, radius;
} L16SingleSphere;

extern void FUN_L16_002e9270_u(void) __asm__("FUN_L16_002e9270");
void enqueue_callback_list_1_u(int arg0, int arg1) __asm__("FUN_001f4600");
extern L16SingleSphere D_L16_00161F60_bounds[] __asm__("D_L16_00161F80")
    __attribute__((section(".sdata")));

void FUN_L16_002e93b0(unsigned char *m) {
    int i, j;
    float maxx, maxy, maxz, minx, miny, minz;
    float p[4];
    float *out;
    float *verts;
    int count;
    switch (m[0x20]) {
    case 0: {
        for (i = 0; i <= 0; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00161F40[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00161F60)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere *sphere = D_L16_00161F60_bounds + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00161F40[i]; j++) {
                float dist;
                out = (float *)(D_L16_00161F60_bounds + i);
                p[0] = ((float **)D_L16_00161F60)[i][j * 3 + 0];
                p[1] = ((float **)D_L16_00161F60)[i][j * 3 + 1];
                p[2] = ((float **)D_L16_00161F60)[i][j * 3 + 2];
                dist = FUN_001f9b48(p, out);
                if (dist > out[3])
                    out[3] = dist;
            }
        }
        m[0x20] = 1;
        m[0x30] = *(unsigned short *)(m + 0x32) = 255;
        break;
    }
    case 1:
        enqueue_callback_list_1_u(FUN_L16_002e9270_u, m);
        break;
    }
}
/* Bounding spheres for one mesh (two or three entries), then the renderer callback; same source as FUN_L16_002e93b0. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2e98e0;

void enqueue_callback_list_1_2e98e0(int arg0, int arg1) __asm__("FUN_001f4600");
extern short D_L16_00161F90_gp __asm__("D_L16_00161F90") __attribute__((sda));
extern short D_L16_00161F98_gp __asm__("D_L16_00161F98") __attribute__((sda));
extern L16SingleSphere_2e98e0 D_L16_00161F60_bounds_2e98e0[] __asm__("D_L16_001DF2B0")
    __attribute__((section(".data")));

void FUN_L16_002e98e0(unsigned char *m) {
    int i, j;
    float maxx, maxy, maxz, minx, miny, minz;
    float p[4];
    float *out;
    float *verts;
    int count;
    switch (m[0x20]) {
    case 0: {
        for (i = 0; i <= 1; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < ((int *)&D_L16_00161F90_gp)[i]; k++) {
                    float x, y, z;
                    verts = ((float **)&D_L16_00161F98_gp)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2e98e0 *base = D_L16_00161F60_bounds_2e98e0;
                L16SingleSphere_2e98e0 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < ((int *)&D_L16_00161F90_gp)[i]; j++) {
                float dist;
                out = (float *)(D_L16_00161F60_bounds_2e98e0 + i);
                p[0] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 0];
                p[1] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 1];
                p[2] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 2];
                dist = FUN_001f9b48(p, out);
                if (dist > out[3])
                    out[3] = dist;
            }
        }
        m[0x20] = 1;
        m[0x30] = *(unsigned short *)(m + 0x32) = 255;
        break;
    }
    case 1:
        enqueue_callback_list_1_2e98e0(FUN_L16_002e97a0, m);
        break;
    }
}
/* Sets up the draw state then runs both per-slot calls over three entries. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EB158), where it is exact; names translated to the US level program. */

char D_L16_001E28D0[10944] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, 0x75, -111, 0x43, -54, 0x11, 0x56, 0x43, -24, -115, -4, 0x42, -12, 0x77, -43, 0x41, -49, 0xf, -117, 0x43, -28, -124, 0x4f, 0x43, -72, -121, -4, 0x42, -81, 0x70, -114, 0x41, -27, -52, -105, 0x43, 6, 0x31, 0x42, 0x43, 0x66, 0x4c, -4, 0x42, -20, -115, 0xa, 0x41, 0x3c, 0x60, -102, 0x43, 0x2a, -16, 0x54, 0x43, -34, 0x7c, -10, 0x42, 0, 0x2e, -102, 0x43, -114, -58, 0x54, 0x43, 0x7e, -123, -7, 0x42, -58, -119, -103, 0x43, -84, 0x12, 0x58, 0x43, -34, 0x7c, -10, 0x42, -112, 0x5c, -103, 0x43, 0x41, -42, 0x57, 0x43, 0x7e, -123, -7, 0x42, 0x26, 0x69, -104, 0x43, 0x1c, -46, 0x5a, 0x43, -34, 0x7c, -10, 0x42, -77, 0x42, -104, 0x43, 0x37, -123, 0x5a, 0x43, 0x7e, -123, -7, 0x42, 0x72, 9, -105, 0x43, 0x66, 0x13, 0x5d, 0x43, -34, 0x7c, -10, 0x42, 0x3d, -21, -106, 0x43, -7, -72, 0x5c, 0x43, 0x7e, -123, -7, 0x42, 0x30, 0x78, -107, 0x43, 0x60, -64, 0x5e, 0x43, -34, 0x7c, -10, 0x42, 0x62, 0x63, -107, 0x43, -25, 0x5b, 0x5e, 0x43, 0x7e, -123, -7, 0x42, -51, -60, -109, 0x43, -119, -56, 0x5f, 0x43, -34, 0x7c, -10, 0x42, 0x32, -70, -109, 0x43, -32, 0x5d, 0x5f, 0x43, 0x7e, -123, -7, 0x42, 2, 0, -110, 0x43, -67, 0x21, 0x60, 0x43, -34, 0x7c, -10, 0x42, 2, 0, -110, 0x43, -3, -76, 0x5f, 0x43, 0x7e, -123, -7, 0x42, 0x37, 0x3b, -112, 0x43, -117, -56, 0x5f, 0x43, -34, 0x7c, -10, 0x42, -45, 0x45, -112, 0x43, -30, 0x5d, 0x5f, 0x43, 0x7e, -123, -7, 0x42, -46, -121, -114, 0x43, 0x65, -64, 0x5e, 0x43, -34, 0x7c, -10, 0x42, -96, -100, -114, 0x43, -20, 0x5b, 0x5e, 0x43, 0x7e, -123, -7, 0x42, -112, -10, -116, 0x43, 0x6e, 0x13, 0x5d, 0x43, -34, 0x7c, -10, 0x42, -58, 0x14, -115, 0x43, 2, -71, 0x5c, 0x43, 0x7e, -123, -7, 0x42, -37, -106, -117, 0x43, 0x24, -46, 0x5a, 0x43, -34, 0x7c, -10, 0x42, 0x4e, -67, -117, 0x43, 0x40, -123, 0x5a, 0x43, 0x7e, -123, -7, 0x42, 0x3a, 0x76, -118, 0x43, -73, 0x12, 0x58, 0x43, -34, 0x7c, -10, 0x42, 0x70, -93, -118, 0x43, 0x4e, -42, 0x57, 0x43, 0x7e, -123, -7, 0x42, -62, -97, -119, 0x43, 0x2f, -16, 0x54, 0x43, -34, 0x7c, -10, 0x42, -1, -47, -119, 0x43, -110, -58, 0x54, 0x43, 0x7e, -123, -7, 0x42, -76, 0x1b, -119, 0x43, 0x64, -119, 0x51, 0x43, -34, 0x7c, -10, 0x42, 8, 0x51, -119, 0x43, 0x2e, 0x74, 0x51, 0x43, 0x7e, -123, -7, 0x42, 0x1f, -17, -120, 0x43, -52, -1, 0x4d, 0x43, -34, 0x7c, -10, 0x42, 0x7f, 0x25, -119, 0x43, -50, -1, 0x4d, 0x43, 0x7e, -123, -7, 0x42, -67, 0x1b, -119, 0x43, 0x38, 0x76, 0x4a, 0x43, -34, 0x7c, -10, 0x42, 0x12, 0x51, -119, 0x43, 0x72, -117, 0x4a, 0x43, 0x7e, -123, -7, 0x42, -42, -97, -119, 0x43, 0x70, 0xf, 0x47, 0x43, -34, 0x7c, -10, 0x42, 0x12, -46, -119, 0x43, 0x10, 0x39, 0x47, 0x43, 0x7e, -123, -7, 0x42, 0x57, 0x76, -118, 0x43, -14, -20, 0x43, 0x43, -34, 0x7c, -10, 0x42, -116, -93, -118, 0x43, 0x60, 0x29, 0x44, 0x43, 0x7e, -123, -7, 0x42, -1, -106, -117, 0x43, -112, 0x2d, 0x41, 0x43, -34, 0x7c, -10, 0x42, 0x70, -67, -117, 0x43, 0x78, 0x7a, 0x41, 0x43, 0x7e, -123, -7, 0x42, -73, -10, -116, 0x43, 0x57, -20, 0x3e, 0x43, -34, 0x7c, -10, 0x42, -20, 0x14, -115, 0x43, -60, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, -5, -121, -114, 0x43, 0x72, 0x3f, 0x3d, 0x43, -34, 0x7c, -10, 0x42, -20, 0x14, -115, 0x43, -60, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, -55, -100, -114, 0x43, -21, -93, 0x3d, 0x43, 0x7e, -123, -7, 0x42, 0x12, 0x42, -115, 0x43, -19, -51, 0x3f, 0x43, 0x1d, -114, -4, 0x42, -30, -69, -114, 0x43, 0x19, 0x3a, 0x3e, 0x43, 0x1d, -114, -4, 0x42, 0x12, 0x79, -115, 0x43, -109, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, -60, -31, -114, 0x43, 0xb, -15, 0x3e, 0x43, -30, 0x78, -1, 0x42, 0x12, 0x79, -115, 0x43, -109, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, -16, 0x11, -113, 0x43, -76, -39, 0x3f, 0x43, -43, 0x31, 1, 0x43, 0x12, 0x79, -115, 0x43, -109, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, 4, -65, -115, 0x43, -11, 0x43, 0x41, 0x43, -43, 0x31, 1, 0x43, -23, 0x3c, -116, 0x43, 0x70, 0x79, 0x42, 0x43, -30, 0x78, -1, 0x42, -17, -107, -116, 0x43, -124, 0x2b, 0x43, 0x43, -43, 0x31, 1, 0x43, 0x70, 0x39, -117, 0x43, -75, -15, 0x44, 0x43, -30, 0x78, -1, 0x42, 0x1f, -94, -117, 0x43, -98, 0x7d, 0x45, 0x43, -43, 0x31, 1, 0x43, -96, 0x78, -118, 0x43, 0xf, -61, 0x47, 0x43, -30, 0x78, -1, 0x42, -14, -20, -118, 0x43, 0x70, 0x23, 0x48, 0x43, -43, 0x31, 1, 0x43, -30, 1, -118, 0x43, -51, -47, 0x4a, 0x43, -30, 0x78, -1, 0x42, 0x5f, 0x7d, -118, 0x43, -17, 2, 0x4b, 0x43, -43, 0x31, 1, 0x43, -57, -39, -119, 0x43, -46, -1, 0x4d, 0x43, -30, 0x78, -1, 0x42, -81, 0x57, -118, 0x43, -44, -1, 0x4d, 0x43, -43, 0x31, 1, 0x43, -39, 1, -118, 0x43, -39, 0x2d, 0x51, 0x43, -30, 0x78, -1, 0x42, 0x56, 0x7d, -118, 0x43, -69, -4, 0x50, 0x43, -43, 0x31, 1, 0x43, -114, 0x78, -118, 0x43, -103, 0x3c, 0x54, 0x43, -30, 0x78, -1, 0x42, -31, -20, -118, 0x43, 0x3d, -36, 0x53, 0x43, -43, 0x31, 1, 0x43, 0x57, 0x39, -117, 0x43, -2, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, 7, -94, -117, 0x43, 0x18, -126, 0x56, 0x43, -43, 0x31, 1, 0x43, -55, 0x3c, -116, 0x43, 0x4c, -122, 0x59, 0x43, -30, 0x78, -1, 0x42, -46, -107, -116, 0x43, 0x3d, -44, 0x58, 0x43, -43, 0x31, 1, 0x43, -17, 0x78, -115, 0x43, 0x37, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, -30, -66, -115, 0x43, -40, -69, 0x5a, 0x43, -43, 0x31, 1, 0x43, -97, -31, -114, 0x43, -49, 0xe, 0x5d, 0x43, -30, 0x78, -1, 0x42, -50, 0x11, -113, 0x43, 0x29, 0x26, 0x5c, 0x43, -43, 0x31, 1, 0x43, -1, 0x68, -112, 0x43, 0x41, -4, 0x5d, 0x43, -30, 0x78, -1, 0x42, -113, -127, -112, 0x43, 0x47, 5, 0x5d, 0x43, -43, 0x31, 1, 0x43, 1, 0, -110, 0x43, 0x6e, 0x4c, 0x5e, 0x43, -30, 0x78, -1, 0x42, 1, 0, -110, 0x43, -100, 0x50, 0x5d, 0x43, -43, 0x31, 1, 0x43, 5, -105, -109, 0x43, 0x3e, -4, 0x5d, 0x43, -30, 0x78, -1, 0x42, 0x74, 0x7e, -109, 0x43, 0x45, 5, 0x5d, 0x43, -43, 0x31, 1, 0x43, 0x64, 0x1e, -107, 0x43, -53, 0xe, 0x5d, 0x43, -30, 0x78, -1, 0x42, 0x34, -18, -108, 0x43, 0x25, 0x26, 0x5c, 0x43, -43, 0x31, 1, 0x43, 0x14, -121, -106, 0x43, 0x2f, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, 0x1f, 0x41, -106, 0x43, -48, -69, 0x5a, 0x43, -43, 0x31, 1, 0x43, 0x37, -61, -105, 0x43, 0x45, -122, 0x59, 0x43, -30, 0x78, -1, 0x42, 0x2f, 0x6a, -105, 0x43, 0x35, -44, 0x58, 0x43, -43, 0x31, 1, 0x43, -87, -58, -104, 0x43, -12, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, -7, 0x5d, -104, 0x43, 0xf, -126, 0x56, 0x43, -43, 0x31, 1, 0x43, -87, -58, -104, 0x43, -12, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, 0x1d, 0x13, -103, 0x43, 0x39, -36, 0x53, 0x43, -43, 0x31, 1, 0x43, -87, -58, -104, 0x43, -12, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, 0x70, -121, -103, 0x43, -107, 0x3c, 0x54, 0x43, -30, 0x78, -1, 0x42, -4, 0x18, -103, 0x43, -12, 0x7b, 0x57, 0x43, 0x1d, -114, -4, 0x42, -23, -30, -103, 0x43, 0x5b, -120, 0x54, 0x43, 0x1d, -114, -4, 0x42, -4, 0x18, -103, 0x43, -12, 0x7b, 0x57, 0x43, 0x1d, -114, -4, 0x42, 0, 0x2e, -102, 0x43, -114, -58, 0x54, 0x43, 0x7e, -123, -7, 0x42, -4, 0x18, -103, 0x43, -12, 0x7b, 0x57, 0x43, 0x1d, -114, -4, 0x42, -112, 0x5c, -103, 0x43, 0x41, -42, 0x57, 0x43, 0x7e, -123, -7, 0x42, -4, 0x18, -103, 0x43, -12, 0x7b, 0x57, 0x43, 0x1d, -114, -4, 0x42, -77, 0x42, -104, 0x43, 0x37, -123, 0x5a, 0x43, 0x7e, -123, -7, 0x42, -4, 0x18, -103, 0x43, -12, 0x7b, 0x57, 0x43, 0x1d, -114, -4, 0x42, 0x3a, 9, -104, 0x43, 0x47, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, -87, -58, -104, 0x43, -12, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, 0x3a, 9, -104, 0x43, 0x47, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, 0x37, -61, -105, 0x43, 0x45, -122, 0x59, 0x43, -30, 0x78, -1, 0x42, 0x3a, 9, -104, 0x43, 0x47, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, 0x14, -121, -106, 0x43, 0x2f, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, 0x3a, 9, -104, 0x43, 0x47, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, 0x16, -66, -106, 0x43, -45, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, -77, 0x42, -104, 0x43, 0x37, -123, 0x5a, 0x43, 0x7e, -123, -7, 0x42, 0x16, -66, -106, 0x43, -45, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, 0x3d, -21, -106, 0x43, -7, -72, 0x5c, 0x43, 0x7e, -123, -7, 0x42, 0x16, -66, -106, 0x43, -45, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, 0x62, 0x63, -107, 0x43, -25, 0x5b, 0x5e, 0x43, 0x7e, -123, -7, 0x42, 0x16, -66, -106, 0x43, -45, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, 0x47, 0x44, -107, 0x43, -69, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, 0x14, -121, -106, 0x43, 0x2f, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, 0x47, 0x44, -107, 0x43, -69, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, 0x64, 0x1e, -107, 0x43, -53, 0xe, 0x5d, 0x43, -30, 0x78, -1, 0x42, 0x47, 0x44, -107, 0x43, -69, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, 5, -105, -109, 0x43, 0x3e, -4, 0x5d, 0x43, -30, 0x78, -1, 0x42, 0x47, 0x44, -107, 0x43, -69, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, 0x56, -86, -109, 0x43, 0x73, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, 0x62, 0x63, -107, 0x43, -25, 0x5b, 0x5e, 0x43, 0x7e, -123, -7, 0x42, 0x56, -86, -109, 0x43, 0x73, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, 0x32, -70, -109, 0x43, -32, 0x5d, 0x5f, 0x43, 0x7e, -123, -7, 0x42, 0x56, -86, -109, 0x43, 0x73, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, 2, 0, -110, 0x43, -3, -76, 0x5f, 0x43, 0x7e, -123, -7, 0x42, 0x56, -86, -109, 0x43, 0x73, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, 2, 0, -110, 0x43, 0x70, 0x12, 0x5f, 0x43, 0x1d, -114, -4, 0x42, 5, -105, -109, 0x43, 0x3e, -4, 0x5d, 0x43, -30, 0x78, -1, 0x42, 2, 0, -110, 0x43, 0x70, 0x12, 0x5f, 0x43, 0x1d, -114, -4, 0x42, 1, 0, -110, 0x43, 0x6e, 0x4c, 0x5e, 0x43, -30, 0x78, -1, 0x42, 2, 0, -110, 0x43, 0x70, 0x12, 0x5f, 0x43, 0x1d, -114, -4, 0x42, -1, 0x68, -112, 0x43, 0x41, -4, 0x5d, 0x43, -30, 0x78, -1, 0x42, 2, 0, -110, 0x43, 0x70, 0x12, 0x5f, 0x43, 0x1d, -114, -4, 0x42, -82, 0x55, -112, 0x43, 0x77, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, 2, 0, -110, 0x43, -3, -76, 0x5f, 0x43, 0x7e, -123, -7, 0x42, -82, 0x55, -112, 0x43, 0x77, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, -45, 0x45, -112, 0x43, -30, 0x5d, 0x5f, 0x43, 0x7e, -123, -7, 0x42, -82, 0x55, -112, 0x43, 0x77, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, -96, -100, -114, 0x43, -20, 0x5b, 0x5e, 0x43, 0x7e, -123, -7, 0x42, -82, 0x55, -112, 0x43, 0x77, -66, 0x5e, 0x43, 0x1d, -114, -4, 0x42, -69, -69, -114, 0x43, -64, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, -1, 0x68, -112, 0x43, 0x41, -4, 0x5d, 0x43, -30, 0x78, -1, 0x42, -69, -69, -114, 0x43, -64, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, -97, -31, -114, 0x43, -49, 0xe, 0x5d, 0x43, -30, 0x78, -1, 0x42, -69, -69, -114, 0x43, -64, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, -17, 0x78, -115, 0x43, 0x37, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, -69, -69, -114, 0x43, -64, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, -17, 0x78, -115, 0x43, 0x37, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, -69, -69, -114, 0x43, -64, -59, 0x5d, 0x43, 0x1d, -114, -4, 0x42, -19, 0x41, -115, 0x43, -36, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, -96, -100, -114, 0x43, -20, 0x5b, 0x5e, 0x43, 0x7e, -123, -7, 0x42, -19, 0x41, -115, 0x43, -36, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, -58, 0x14, -115, 0x43, 2, -71, 0x5c, 0x43, 0x7e, -123, -7, 0x42, -19, 0x41, -115, 0x43, -36, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, 0x4e, -67, -117, 0x43, 0x40, -123, 0x5a, 0x43, 0x7e, -123, -7, 0x42, -19, 0x41, -115, 0x43, -36, 0x31, 0x5c, 0x43, 0x1d, -114, -4, 0x42, -57, -10, -117, 0x43, 0x4f, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, -17, 0x78, -115, 0x43, 0x37, -115, 0x5b, 0x43, -30, 0x78, -1, 0x42, -57, -10, -117, 0x43, 0x4f, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, -55, 0x3c, -116, 0x43, 0x4c, -122, 0x59, 0x43, -30, 0x78, -1, 0x42, -57, -10, -117, 0x43, 0x4f, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, 0x57, 0x39, -117, 0x43, -2, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, -57, -10, -117, 0x43, 0x4f, 0x12, 0x5a, 0x43, 0x1d, -114, -4, 0x42, 4, -25, -118, 0x43, 0, 0x7c, 0x57, 0x43, 0x1d, -114, -4, 0x42, 0x4e, -67, -117, 0x43, 0x40, -123, 0x5a, 0x43, 0x7e, -123, -7, 0x42, 4, -25, -118, 0x43, 0, 0x7c, 0x57, 0x43, 0x1d, -114, -4, 0x42, 0x70, -93, -118, 0x43, 0x4e, -42, 0x57, 0x43, 0x7e, -123, -7, 0x42, 4, -25, -118, 0x43, 0, 0x7c, 0x57, 0x43, 0x1d, -114, -4, 0x42, -1, -47, -119, 0x43, -110, -58, 0x54, 0x43, 0x7e, -123, -7, 0x42, 4, -25, -118, 0x43, 0, 0x7c, 0x57, 0x43, 0x1d, -114, -4, 0x42, 0x16, 0x1d, -118, 0x43, 0x5e, -120, 0x54, 0x43, 0x1d, -114, -4, 0x42, 0x57, 0x39, -117, 0x43, -2, 0xd, 0x57, 0x43, -30, 0x78, -1, 0x42, 0x16, 0x1d, -118, 0x43, 0x5e, -120, 0x54, 0x43, 0x1d, -114, -4, 0x42, -114, 0x78, -118, 0x43, -103, 0x3c, 0x54, 0x43, -30, 0x78, -1, 0x42, 0x16, 0x1d, -118, 0x43, 0x5e, -120, 0x54, 0x43, 0x1d, -114, -4, 0x42, -39, 1, -118, 0x43, -39, 0x2d, 0x51, 0x43, -30, 0x78, -1, 0x42, 0x16, 0x1d, -118, 0x43, 0x5e, -120, 0x54, 0x43, 0x1d, -114, -4, 0x42, -65, -96, -119, 0x43, 0x79, 0x54, 0x51, 0x43, 0x1d, -114, -4, 0x42, -1, -47, -119, 0x43, -110, -58, 0x54, 0x43, 0x7e, -123, -7, 0x42, -65, -96, -119, 0x43, 0x79, 0x54, 0x51, 0x43, 0x1d, -114, -4, 0x42, 8, 0x51, -119, 0x43, 0x2e, 0x74, 0x51, 0x43, 0x7e, -123, -7, 0x42, -65, -96, -119, 0x43, 0x79, 0x54, 0x51, 0x43, 0x1d, -114, -4, 0x42, 0x7f, 0x25, -119, 0x43, -50, -1, 0x4d, 0x43, 0x7e, -123, -7, 0x42, -65, -96, -119, 0x43, 0x79, 0x54, 0x51, 0x43, 0x1d, -114, -4, 0x42, -59, 0x76, -119, 0x43, -49, -1, 0x4d, 0x43, 0x1d, -114, -4, 0x42, -39, 1, -118, 0x43, -39, 0x2d, 0x51, 0x43, -30, 0x78, -1, 0x42, -59, 0x76, -119, 0x43, -49, -1, 0x4d, 0x43, 0x1d, -114, -4, 0x42, -57, -39, -119, 0x43, -46, -1, 0x4d, 0x43, -30, 0x78, -1, 0x42, -59, 0x76, -119, 0x43, -49, -1, 0x4d, 0x43, 0x1d, -114, -4, 0x42, -30, 1, -118, 0x43, -51, -47, 0x4a, 0x43, -30, 0x78, -1, 0x42, -59, 0x76, -119, 0x43, -49, -1, 0x4d, 0x43, 0x1d, -114, -4, 0x42, -57, -96, -119, 0x43, 0x2a, -85, 0x4a, 0x43, 0x1d, -114, -4, 0x42, 0x7f, 0x25, -119, 0x43, -50, -1, 0x4d, 0x43, 0x7e, -123, -7, 0x42, -57, -96, -119, 0x43, 0x2a, -85, 0x4a, 0x43, 0x1d, -114, -4, 0x42, 0x12, 0x51, -119, 0x43, 0x72, -117, 0x4a, 0x43, 0x7e, -123, -7, 0x42, -57, -96, -119, 0x43, 0x2a, -85, 0x4a, 0x43, 0x1d, -114, -4, 0x42, 0x12, -46, -119, 0x43, 0x10, 0x39, 0x47, 0x43, 0x7e, -123, -7, 0x42, -57, -96, -119, 0x43, 0x2a, -85, 0x4a, 0x43, 0x1d, -114, -4, 0x42, 0x27, 0x1d, -118, 0x43, 0x47, 0x77, 0x47, 0x43, 0x1d, -114, -4, 0x42, -30, 1, -118, 0x43, -51, -47, 0x4a, 0x43, -30, 0x78, -1, 0x42, 0x27, 0x1d, -118, 0x43, 0x47, 0x77, 0x47, 0x43, 0x1d, -114, -4, 0x42, -96, 0x78, -118, 0x43, 0xf, -61, 0x47, 0x43, -30, 0x78, -1, 0x42, 0x27, 0x1d, -118, 0x43, 0x47, 0x77, 0x47, 0x43, 0x1d, -114, -4, 0x42, 0x70, 0x39, -117, 0x43, -75, -15, 0x44, 0x43, -30, 0x78, -1, 0x42, 0x27, 0x1d, -118, 0x43, 0x47, 0x77, 0x47, 0x43, 0x1d, -114, -4, 0x42, 0x1f, -25, -118, 0x43, -80, -125, 0x44, 0x43, 0x1d, -114, -4, 0x42, 0x12, -46, -119, 0x43, 0x10, 0x39, 0x47, 0x43, 0x7e, -123, -7, 0x42, 0x1f, -25, -118, 0x43, -80, -125, 0x44, 0x43, 0x1d, -114, -4, 0x42, -116, -93, -118, 0x43, 0x60, 0x29, 0x44, 0x43, 0x7e, -123, -7, 0x42, -24, -10, -117, 0x43, 0x6a, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, 0x70, -67, -117, 0x43, 0x78, 0x7a, 0x41, 0x43, 0x7e, -123, -7, 0x42, -24, -10, -117, 0x43, 0x6a, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, -20, 0x14, -115, 0x43, -60, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, -24, -10, -117, 0x43, 0x6a, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, 0x12, 0x42, -115, 0x43, -19, -51, 0x3f, 0x43, 0x1d, -114, -4, 0x42, -24, -10, -117, 0x43, 0x6a, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, 0x12, 0x79, -115, 0x43, -109, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, -24, -10, -117, 0x43, 0x6a, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, -23, 0x3c, -116, 0x43, 0x70, 0x79, 0x42, 0x43, -30, 0x78, -1, 0x42, 0x1f, -25, -118, 0x43, -80, -125, 0x44, 0x43, 0x1d, -114, -4, 0x42, 0x70, 0x39, -117, 0x43, -75, -15, 0x44, 0x43, -30, 0x78, -1, 0x42, 0, 0, 0, 0, 0x24, 0x60, -102, 0x43, 0x61, 0xf, 0x47, 0x43, -34, 0x7c, -10, 0x42, -92, -119, -103, 0x43, -23, -20, 0x43, 0x43, -34, 0x7c, -10, 0x42, -23, 0x2d, -102, 0x43, 1, 0x39, 0x47, 0x43, 0x7e, -123, -7, 0x42, 0x6e, 0x5c, -103, 0x43, 0x54, 0x29, 0x44, 0x43, 0x7e, -123, -7, 0x42, -45, -30, -103, 0x43, 0x37, 0x77, 0x47, 0x43, 0x1d, -114, -4, 0x42, -37, 0x18, -103, 0x43, -90, -125, 0x44, 0x43, 0x1d, -114, -4, 0x42, 0x5b, -121, -103, 0x43, 2, -61, 0x47, 0x43, -30, 0x78, -1, 0x42, -118, -58, -104, 0x43, -85, -15, 0x44, 0x43, -30, 0x78, -1, 0x42, 9, 0x13, -103, 0x43, 0x62, 0x23, 0x48, 0x43, -43, 0x31, 1, 0x43, -118, -58, -104, 0x43, -85, -15, 0x44, 0x43, -30, 0x78, -1, 0x42, -37, 0x5d, -104, 0x43, -105, 0x7d, 0x45, 0x43, -43, 0x31, 1, 0x43, 0x10, -61, -105, 0x43, 0x69, 0x79, 0x42, 0x43, -30, 0x78, -1, 0x42, 0xa, 0x6a, -105, 0x43, 0x7d, 0x2b, 0x43, 0x43, -43, 0x31, 1, 0x43, -27, -122, -106, 0x43, -115, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, -12, 0x40, -106, 0x43, -17, 0x43, 0x41, 0x43, -43, 0x31, 1, 0x43, -27, -122, -106, 0x43, -115, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, 7, -18, -108, 0x43, -81, -39, 0x3f, 0x43, -43, 0x31, 1, 0x43, -27, -122, -106, 0x43, -115, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, 0x33, 0x1e, -107, 0x43, 7, -15, 0x3e, 0x43, -30, 0x78, -1, 0x42, -28, -67, -106, 0x43, -27, -51, 0x3f, 0x43, 0x1d, -114, -4, 0x42, 0x15, 0x44, -107, 0x43, 0x15, 0x3a, 0x3e, 0x43, 0x1d, -114, -4, 0x42, 0xa, -21, -106, 0x43, -68, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, 0x2d, 0x63, -107, 0x43, -25, -93, 0x3d, 0x43, 0x7e, -123, -7, 0x42, 0xa, -21, -106, 0x43, -68, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, -5, 0x77, -107, 0x43, 0x6e, 0x3f, 0x3d, 0x43, -34, 0x7c, -10, 0x42, 0xa, -21, -106, 0x43, -68, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, 0x3f, 9, -105, 0x43, 0x4f, -20, 0x3e, 0x43, -34, 0x7c, -10, 0x42, -119, 0x42, -104, 0x43, 0x70, 0x7a, 0x41, 0x43, 0x7e, -123, -7, 0x42, -5, 0x68, -104, 0x43, -119, 0x2d, 0x41, 0x43, -34, 0x7c, -10, 0x42, -119, 0x42, -104, 0x43, 0x70, 0x7a, 0x41, 0x43, 0x7e, -123, -7, 0x42, -92, -119, -103, 0x43, -23, -20, 0x43, 0x43, -34, 0x7c, -10, 0x42, -119, 0x42, -104, 0x43, 0x70, 0x7a, 0x41, 0x43, 0x7e, -123, -7, 0x42, 0x6e, 0x5c, -103, 0x43, 0x54, 0x29, 0x44, 0x43, 0x7e, -123, -7, 0x42, 0x11, 9, -104, 0x43, 0x62, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, -37, 0x18, -103, 0x43, -90, -125, 0x44, 0x43, 0x1d, -114, -4, 0x42, 0x11, 9, -104, 0x43, 0x62, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, -118, -58, -104, 0x43, -85, -15, 0x44, 0x43, -30, 0x78, -1, 0x42, 0x11, 9, -104, 0x43, 0x62, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, 0x10, -61, -105, 0x43, 0x69, 0x79, 0x42, 0x43, -30, 0x78, -1, 0x42, 0x11, 9, -104, 0x43, 0x62, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, -27, -122, -106, 0x43, -115, 0x72, 0x40, 0x43, -30, 0x78, -1, 0x42, 0x11, 9, -104, 0x43, 0x62, -19, 0x41, 0x43, 0x1d, -114, -4, 0x42, -28, -67, -106, 0x43, -27, -51, 0x3f, 0x43, 0x1d, -114, -4, 0x42, -119, 0x42, -104, 0x43, 0x70, 0x7a, 0x41, 0x43, 0x7e, -123, -7, 0x42, 0xa, -21, -106, 0x43, -68, 0x46, 0x3f, 0x43, 0x7e, -123, -7, 0x42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0, 0, 0, -128, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, -128, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, -128, -65, 0, 0, 0, 0, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, 0, 0, 0, 0, 0, -65, 0, 0, 0, 0, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, -128, -66, 0, 0, -128, 0x3f, 0, 0, 0, -65, 0, 0, 0, 0x3f, 0, 0, 0, -65, 0, 0, -128, 0x3f, 0, 0, 0x40, -65, 0, 0, 0, 0x3f, 0, 0, 0x40, -65, 0x70, 0x7c, 0x59, 0x3f, -102, 0x7b, -24, 0x3e, -114, 0x75, -119, 0x3e, 0x13, -13, 0x54, 0x3f, 0x7b, -94, -29, 0x3e, -74, 0x2c, -89, 0x3e, 0x13, 0xf, 0x4c, 0x3f, -109, 0x56, 8, 0x3f, 0x6d, 0x75, -119, 0x3e, 0x79, -51, 0x47, 0x3f, -103, 0x7e, 5, 0x3f, -7, 0x2c, -89, 0x3e, -33, -119, 0x2d, 0x3f, 0x62, -121, 0x2d, 0x3f, 0x6d, 0x75, -119, 0x3e, 0x42, -21, 0x29, 0x3f, -43, -24, 0x29, 0x3f, -74, 0x2c, -89, 0x3e, 0x43, 0x59, 8, 0x3f, 0x3d, 0xd, 0x4c, 0x3f, 0x4b, 0x75, -119, 0x3e, 0x49, -127, 5, 0x3f, -59, -53, 0x47, 0x3f, 0x52, 0x2c, -89, 0x3e, 0xa, -41, -69, 0x3e, 0x34, -69, 0x62, 0x3f, 8, 0x75, -119, 0x3e, 0x35, -20, -73, 0x3e, 0x7e, 0, 0x5e, 0x3f, -40, 0x2c, -89, 0x3e, 0x1a, -122, 0x3f, 0x3e, -89, -78, 0x70, 0x3f, -114, 0x75, -119, 0x3e, 0x7b, -121, 0x3b, 0x3e, 0x4b, -83, 0x6b, 0x3f, 0x3c, 0x2d, -89, 0x3e, -67, 0x37, -122, 0x36, -28, 0x69, 0x75, 0x3f, -47, 0x75, -119, 0x3e, -67, 0x37, -122, 0x36, -112, 0x4b, 0x70, 0x3f, -74, 0x2c, -89, 0x3e, -11, -126, 0x3f, -66, -56, -78, 0x70, 0x3f, -114, 0x75, -119, 0x3e, 0x55, -124, 0x3b, -66, -80, -83, 0x6b, 0x3f, 0x52, 0x2c, -89, 0x3e, 0x56, -43, -69, -66, 0x77, -69, 0x62, 0x3f, -47, 0x75, -119, 0x3e, -127, -22, -73, -66, -30, 0, 0x5e, 0x3f, -107, 0x2c, -89, 0x3e, -51, 0x58, 8, -65, -111, 0xd, 0x4c, 0x3f, 0x4b, 0x75, -119, 0x3e, -61, -128, 5, -65, 8, -52, 0x47, 0x3f, -74, 0x2c, -89, 0x3e, -117, -119, 0x2d, -65, -7, -121, 0x2d, 0x3f, 0x3f, 0x74, -119, 0x3e, -85, -22, 0x29, -65, 0x5b, -23, 0x29, 0x3f, -74, 0x2c, -89, 0x3e, -65, 0xe, 0x4c, -65, 8, 0x57, 8, 0x3f, 0x6d, 0x75, -119, 0x3e, 0x25, -51, 0x47, -65, -36, 0x7e, 5, 0x3f, 0x1b, 0x2d, -89, 0x3e, 0x62, -68, 0x62, -65, -31, -48, -69, 0x3e, 0x6d, 0x75, -119, 0x3e, -84, 1, 0x5e, -65, -21, -27, -73, 0x3e, 0x5e, 0x2d, -89, 0x3e, 0x5f, -77, 0x70, -65, -123, 0x79, 0x3f, 0x3e, 8, 0x75, -119, 0x3e, 4, -82, 0x6b, -65, 0x6c, 0x7b, 0x3b, 0x3e, -74, 0x2c, -89, 0x3e, -62, 0x69, 0x75, -65, 0x73, 0x30, 0x1b, -72, 0x79, 0x76, -119, 0x3e, -112, 0x4b, 0x70, -65, -18, -109, 0x23, -72, -74, 0x2c, -89, 0x3e, 0x10, -78, 0x70, -65, -63, -114, 0x3f, -66, -101, 0x76, -119, 0x3e, 0x2a, -83, 0x6b, -65, -34, -113, 0x3b, -66, 0x52, 0x2c, -89, 0x3e, 0x38, -70, 0x62, -65, 0x7f, -37, -69, -66, -114, 0x75, -119, 0x3e, -75, -1, 0x5d, -65, 0x45, -16, -73, -66, 0x52, 0x2c, -89, 0x3e, -2, 0xb, 0x4c, -65, 0x5b, 0x5b, 8, -65, -59, 0x74, -119, 0x3e, 0x75, -54, 0x47, -65, 0x1f, -125, 5, -65, -107, 0x2c, -89, 0x3e, 0x23, -122, 0x2d, -65, 0x3f, -117, 0x2d, -65, 8, 0x75, -119, 0x3e, -106, -25, 0x29, -65, -127, -20, 0x29, -65, -40, 0x2c, -89, 0x3e, -34, 0x54, 8, -65, 0x1f, 0x10, 0x4c, -65, -114, 0x75, -119, 0x3e, -27, 0x7c, 5, -65, -123, -50, 0x47, -65, -7, 0x2c, -89, 0x3e, 0x47, 0x77, -24, -66, -98, 0x7d, 0x59, -65, 0x29, 0x75, -119, 0x3e, -27, 0x7c, 5, -65, -123, -50, 0x47, -65, -7, 0x2c, -89, 0x3e, 0x28, -98, -29, -66, 0x41, -12, 0x54, -65, -40, 0x2c, -89, 0x3e, 0x5f, -49, -1, -66, 0x2a, 0x73, 0x3f, -65, 0xf, -17, -39, 0x3e, 1, 0x19, -38, -66, 0xf, 0xc, 0x4c, -65, 0x31, -17, -39, 0x3e, -26, -83, -14, -66, 0x6f, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, -5, -26, -50, -66, -55, -110, 0x41, -65, -116, 0x13, 3, 0x3f, -26, -83, -14, -66, 0x6f, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, -35, 0x60, -56, -66, 0x57, 0x78, 0x3b, -65, 8, -86, 0xe, 0x3f, -26, -83, -14, -66, 0x6f, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, 7, 7, -21, -66, 0x75, -27, 0x2f, -65, -42, -87, 0xe, 0x3f, 0x26, 0x71, 0x1a, -65, -68, 0x75, 0x1a, -65, 0x59, 0x13, 3, 0x3f, -122, -110, 0x15, -65, 0xb, -105, 0x15, -65, -42, -87, 0xe, 0x3f, -61, -101, 0x35, -65, 0x4e, -71, -14, -66, 0x38, 0x13, 3, 0x3f, -4, -31, 0x2f, -65, 0xa, 0x12, -21, -66, -75, -87, 0xe, 0x3f, -118, -53, 0x49, -65, -33, 0x32, -89, -66, 0x6a, 0x13, 3, 0x3f, -8, 0x6e, 0x43, -65, 0x5b, -19, -95, -66, -126, -87, 0xe, 0x3f, 0x19, 0x3a, 0x56, -65, 0x67, 0x7d, 0x2a, -66, -83, 0x13, 3, 0x3f, 0x31, 0x79, 0x4f, -65, -17, 0x1d, 0x25, -66, -59, -87, 0xe, 0x3f, 0x26, 0x6d, 0x5a, -65, 0x39, -101, 0xe, -72, 0x7b, 0x13, 3, 0x3f, 0x4c, -118, 0x53, -65, -75, -2, 0x16, -72, -92, -87, 0xe, 0x3f, 0x58, 0x3b, 0x56, -65, -41, 0x6b, 0x2a, 0x3e, 0x16, 0x13, 3, 0x3f, 0x3e, 0x7a, 0x4f, -65, -40, 0xb, 0x25, 0x3e, -126, -87, 0xe, 0x3f, -77, -51, 0x49, -65, -10, 0x29, -89, 0x3e, -28, 0x12, 3, 0x3f, -84, 0x70, 0x43, -65, -42, -28, -95, 0x3e, -109, -87, 0xe, 0x3f, 0x62, -98, 0x35, -65, -110, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, 0x7a, -28, 0x2f, -65, -44, 0xa, -21, 0x3e, -109, -87, 0xe, 0x3f, 8, 0x74, 0x1a, -65, -55, 0x72, 0x1a, 0x3f, 0x6a, 0x13, 3, 0x3f, 0x57, -107, 0x15, -65, 0x18, -108, 0x15, 0x3f, -42, -87, 0xe, 0x3f, -39, -76, -14, -66, 0x24, -99, 0x35, 0x3f, 0x59, 0x13, 3, 0x3f, -73, 0xd, -21, -66, 0x6d, -29, 0x2f, 0x3f, -75, -87, 0xe, 0x3f, -61, 0x2d, -89, -66, -106, -52, 0x49, 0x3f, 0x7b, 0x13, 3, 0x3f, -93, -24, -95, -66, -12, 0x6f, 0x43, 0x3f, -126, -87, 0xe, 0x3f, 0x71, 0x73, 0x2a, -66, -80, 0x3a, 0x56, 0x3f, -116, 0x13, 3, 0x3f, 0x3c, 0x14, 0x25, -66, -89, 0x79, 0x4f, 0x3f, -59, -87, 0xe, 0x3f, -117, -31, -22, 0x36, 0x26, 0x6d, 0x5a, 0x3f, 0x59, 0x13, 3, 0x3f, -84, -59, -89, 0x36, 9, -118, 0x53, 0x3f, -8, -87, 0xe, 0x3f, 0x10, 0x76, 0x2a, 0x3e, -63, 0x3a, 0x56, 0x3f, 0x38, 0x13, 3, 0x3f, 0x55, 0x16, 0x25, 0x3e, -123, 0x79, 0x4f, 0x3f, -42, -87, 0xe, 0x3f, -15, 0x2e, -89, 0x3e, 0x75, -52, 0x49, 0x3f, 0x38, 0x13, 3, 0x3f, -114, -23, -95, 0x3e, 0x7e, 0x6f, 0x43, 0x3f, -42, -87, 0xe, 0x3f, -27, -75, -14, 0x3e, -82, -100, 0x35, 0x3f, 0x7b, 0x13, 3, 0x3f, -27, 0xe, -21, 0x3e, -92, -30, 0x2f, 0x3f, 0x2a, -86, 0xe, 0x3f, 0x7e, 0x74, 0x1a, 0x3f, 0x32, 0x72, 0x1a, 0x3f, 0x7b, 0x13, 3, 0x3f, -68, -107, 0x15, 0x3f, -93, -109, 0x15, 0x3f, -8, -87, 0xe, 0x3f, -124, -98, 0x35, 0x3f, 0xc, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, -84, -28, 0x2f, 0x3f, 0x2d, 0xa, -21, 0x3e, -109, -87, 0xe, 0x3f, -124, -98, 0x35, 0x3f, 0xc, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, -30, 0x77, 0x3b, 0x3f, -19, 0x64, -56, 0x3e, 0x2e, -87, 0xe, 0x3f, -124, -98, 0x35, 0x3f, 0xc, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, 0x10, -110, 0x41, 0x3f, 0xb, -21, -50, 0x3e, 0x16, 0x13, 3, 0x3f, 0x1d, 0x72, 0x3f, 0x3f, -90, -46, -1, 0x3e, 0x31, -17, -39, 0x3e, 3, 0xb, 0x4c, 0x3f, 0x54, 0x1d, -38, 0x3e, -18, -18, -39, 0x3e, 0x1d, 0x72, 0x3f, 0x3f, -90, -46, -1, 0x3e, 0x31, -17, -39, 0x3e, 0x13, -13, 0x54, 0x3f, 0x7b, -94, -29, 0x3e, -74, 0x2c, -89, 0x3e, 0x1d, 0x72, 0x3f, 0x3f, -90, -46, -1, 0x3e, 0x31, -17, -39, 0x3e, 0x79, -51, 0x47, 0x3f, -103, 0x7e, 5, 0x3f, -7, 0x2c, -89, 0x3e, 0x1d, 0x72, 0x3f, 0x3f, -90, -46, -1, 0x3e, 0x31, -17, -39, 0x3e, 0x42, -21, 0x29, 0x3f, -43, -24, 0x29, 0x3f, -74, 0x2c, -89, 0x3e, 0x1d, 0x72, 0x3f, 0x3f, -90, -46, -1, 0x3e, 0x31, -17, -39, 0x3e, -22, -49, 0x22, 0x3f, -114, -51, 0x22, 0x3f, -18, -18, -39, 0x3e, -124, -98, 0x35, 0x3f, 0xc, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, -22, -49, 0x22, 0x3f, -114, -51, 0x22, 0x3f, -18, -18, -39, 0x3e, 0x7e, 0x74, 0x1a, 0x3f, 0x32, 0x72, 0x1a, 0x3f, 0x7b, 0x13, 3, 0x3f, -22, -49, 0x22, 0x3f, -114, -51, 0x22, 0x3f, -18, -18, -39, 0x3e, -27, -75, -14, 0x3e, -82, -100, 0x35, 0x3f, 0x7b, 0x13, 3, 0x3f, -22, -49, 0x22, 0x3f, -114, -51, 0x22, 0x3f, -18, -18, -39, 0x3e, -28, -41, -1, 0x3e, 0x7a, 0x70, 0x3f, 0x3f, -119, -18, -39, 0x3e, 0x42, -21, 0x29, 0x3f, -43, -24, 0x29, 0x3f, -74, 0x2c, -89, 0x3e, -28, -41, -1, 0x3e, 0x7a, 0x70, 0x3f, 0x3f, -119, -18, -39, 0x3e, 0x49, -127, 5, 0x3f, -59, -53, 0x47, 0x3f, 0x52, 0x2c, -89, 0x3e, -28, -41, -1, 0x3e, 0x7a, 0x70, 0x3f, 0x3f, -119, -18, -39, 0x3e, 0x35, -20, -73, 0x3e, 0x7e, 0, 0x5e, 0x3f, -40, 0x2c, -89, 0x3e, -28, -41, -1, 0x3e, 0x7a, 0x70, 0x3f, 0x3f, -119, -18, -39, 0x3e, -38, 0x3a, -80, 0x3e, -111, -73, 0x54, 0x3f, -18, -18, -39, 0x3e, -27, -75, -14, 0x3e, -82, -100, 0x35, 0x3f, 0x7b, 0x13, 3, 0x3f, -38, 0x3a, -80, 0x3e, -111, -73, 0x54, 0x3f, -18, -18, -39, 0x3e, -15, 0x2e, -89, 0x3e, 0x75, -52, 0x49, 0x3f, 0x38, 0x13, 3, 0x3f, -38, 0x3a, -80, 0x3e, -111, -73, 0x54, 0x3f, -18, -18, -39, 0x3e, 0x10, 0x76, 0x2a, 0x3e, -63, 0x3a, 0x56, 0x3f, 0x38, 0x13, 3, 0x3f, -38, 0x3a, -80, 0x3e, -111, -73, 0x54, 0x3f, -18, -18, -39, 0x3e, 0x75, -81, 0x33, 0x3e, 7, -46, 0x61, 0x3f, 0xf, -17, -39, 0x3e, 0x35, -20, -73, 0x3e, 0x7e, 0, 0x5e, 0x3f, -40, 0x2c, -89, 0x3e, 0x75, -81, 0x33, 0x3e, 7, -46, 0x61, 0x3f, 0xf, -17, -39, 0x3e, 0x7b, -121, 0x3b, 0x3e, 0x4b, -83, 0x6b, 0x3f, 0x3c, 0x2d, -89, 0x3e, 0x75, -81, 0x33, 0x3e, 7, -46, 0x61, 0x3f, 0xf, -17, -39, 0x3e, -67, 0x37, -122, 0x36, -112, 0x4b, 0x70, 0x3f, -74, 0x2c, -89, 0x3e, 0x75, -81, 0x33, 0x3e, 7, -46, 0x61, 0x3f, 0xf, -17, -39, 0x3e, -100, 0x53, -55, 0x36, -64, 0x3e, 0x66, 0x3f, -86, -18, -39, 0x3e, 0x10, 0x76, 0x2a, 0x3e, -63, 0x3a, 0x56, 0x3f, 0x38, 0x13, 3, 0x3f, -100, 0x53, -55, 0x36, -64, 0x3e, 0x66, 0x3f, -86, -18, -39, 0x3e, -117, -31, -22, 0x36, 0x26, 0x6d, 0x5a, 0x3f, 0x59, 0x13, 3, 0x3f, -100, 0x53, -55, 0x36, -64, 0x3e, 0x66, 0x3f, -86, -18, -39, 0x3e, 0x71, 0x73, 0x2a, -66, -80, 0x3a, 0x56, 0x3f, -116, 0x13, 3, 0x3f, -100, 0x53, -55, 0x36, -64, 0x3e, 0x66, 0x3f, -86, -18, -39, 0x3e, 0xc, -84, 0x33, -66, 0x28, -46, 0x61, 0x3f, -52, -18, -39, 0x3e, -67, 0x37, -122, 0x36, -112, 0x4b, 0x70, 0x3f, -74, 0x2c, -89, 0x3e, 0xc, -84, 0x33, -66, 0x28, -46, 0x61, 0x3f, -52, -18, -39, 0x3e, 0x55, -124, 0x3b, -66, -80, -83, 0x6b, 0x3f, 0x52, 0x2c, -89, 0x3e, 0xc, -84, 0x33, -66, 0x28, -46, 0x61, 0x3f, -52, -18, -39, 0x3e, -127, -22, -73, -66, -30, 0, 0x5e, 0x3f, -107, 0x2c, -89, 0x3e, 0xc, -84, 0x33, -66, 0x28, -46, 0x61, 0x3f, -52, -18, -39, 0x3e, 0x47, 0x39, -80, -66, -61, -73, 0x54, 0x3f, 0xf, -17, -39, 0x3e, 0x71, 0x73, 0x2a, -66, -80, 0x3a, 0x56, 0x3f, -116, 0x13, 3, 0x3f, 0x47, 0x39, -80, -66, -61, -73, 0x54, 0x3f, 0xf, -17, -39, 0x3e, -61, 0x2d, -89, -66, -106, -52, 0x49, 0x3f, 0x7b, 0x13, 3, 0x3f, 0x47, 0x39, -80, -66, -61, -73, 0x54, 0x3f, 0xf, -17, -39, 0x3e, -39, -76, -14, -66, 0x24, -99, 0x35, 0x3f, 0x59, 0x13, 3, 0x3f, 0x47, 0x39, -80, -66, -61, -73, 0x54, 0x3f, 0xf, -17, -39, 0x3e, -39, -76, -14, -66, 0x24, -99, 0x35, 0x3f, 0x59, 0x13, 3, 0x3f, 0x47, 0x39, -80, -66, -61, -73, 0x54, 0x3f, 0xf, -17, -39, 0x3e, -74, -42, -1, -66, -84, 0x70, 0x3f, 0x3f, 0x31, -17, -39, 0x3e, -127, -22, -73, -66, -30, 0, 0x5e, 0x3f, -107, 0x2c, -89, 0x3e, -74, -42, -1, -66, -84, 0x70, 0x3f, 0x3f, 0x31, -17, -39, 0x3e, -61, -128, 5, -65, 8, -52, 0x47, 0x3f, -74, 0x2c, -89, 0x3e, -74, -42, -1, -66, -84, 0x70, 0x3f, 0x3f, 0x31, -17, -39, 0x3e, -85, -22, 0x29, -65, 0x5b, -23, 0x29, 0x3f, -74, 0x2c, -89, 0x3e, -74, -42, -1, -66, -84, 0x70, 0x3f, 0x3f, 0x31, -17, -39, 0x3e, 0x53, -49, 0x22, -65, 0x14, -50, 0x22, 0x3f, 0x74, -17, -39, 0x3e, -39, -76, -14, -66, 0x24, -99, 0x35, 0x3f, 0x59, 0x13, 3, 0x3f, 0x53, -49, 0x22, -65, 0x14, -50, 0x22, 0x3f, 0x74, -17, -39, 0x3e, 8, 0x74, 0x1a, -65, -55, 0x72, 0x1a, 0x3f, 0x6a, 0x13, 3, 0x3f, 0x53, -49, 0x22, -65, 0x14, -50, 0x22, 0x3f, 0x74, -17, -39, 0x3e, 0x62, -98, 0x35, -65, -110, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, 0x53, -49, 0x22, -65, 0x14, -50, 0x22, 0x3f, 0x74, -17, -39, 0x3e, -38, 0x71, 0x3f, -65, -56, -46, -1, 0x3e, 0x52, -17, -39, 0x3e, -85, -22, 0x29, -65, 0x5b, -23, 0x29, 0x3f, -74, 0x2c, -89, 0x3e, -38, 0x71, 0x3f, -65, -56, -46, -1, 0x3e, 0x52, -17, -39, 0x3e, 0x25, -51, 0x47, -65, -36, 0x7e, 5, 0x3f, 0x1b, 0x2d, -89, 0x3e, -38, 0x71, 0x3f, -65, -56, -46, -1, 0x3e, 0x52, -17, -39, 0x3e, -84, 1, 0x5e, -65, -21, -27, -73, 0x3e, 0x5e, 0x2d, -89, 0x3e, -38, 0x71, 0x3f, -65, -56, -46, -1, 0x3e, 0x52, -17, -39, 0x3e, -48, -72, 0x54, -65, 0x16, 0x35, -80, 0x3e, -18, -18, -39, 0x3e, 0x62, -98, 0x35, -65, -110, -79, -14, 0x3e, 0x38, 0x13, 3, 0x3f, -48, -72, 0x54, -65, 0x16, 0x35, -80, 0x3e, -18, -18, -39, 0x3e, -77, -51, 0x49, -65, -10, 0x29, -89, 0x3e, -28, 0x12, 3, 0x3f, -48, -72, 0x54, -65, 0x16, 0x35, -80, 0x3e, -18, -18, -39, 0x3e, 0x58, 0x3b, 0x56, -65, -41, 0x6b, 0x2a, 0x3e, 0x16, 0x13, 3, 0x3f, -48, -72, 0x54, -65, 0x16, 0x35, -80, 0x3e, -18, -18, -39, 0x3e, -115, -46, 0x61, -65, 0x72, -92, 0x33, 0x3e, -18, -18, -39, 0x3e, -84, 1, 0x5e, -65, -21, -27, -73, 0x3e, 0x5e, 0x2d, -89, 0x3e, -115, -46, 0x61, -65, 0x72, -92, 0x33, 0x3e, -18, -18, -39, 0x3e, 4, -82, 0x6b, -65, 0x6c, 0x7b, 0x3b, 0x3e, -74, 0x2c, -89, 0x3e, -115, -46, 0x61, -65, 0x72, -92, 0x33, 0x3e, -18, -18, -39, 0x3e, -112, 0x4b, 0x70, -65, -18, -109, 0x23, -72, -74, 0x2c, -89, 0x3e, -115, -46, 0x61, -65, 0x72, -92, 0x33, 0x3e, -18, -18, -39, 0x3e, -81, 0x3e, 0x66, -65, 0x73, 0x30, 0x1b, -72, -86, -18, -39, 0x3e, 0x58, 0x3b, 0x56, -65, -41, 0x6b, 0x2a, 0x3e, 0x16, 0x13, 3, 0x3f, -81, 0x3e, 0x66, -65, 0x73, 0x30, 0x1b, -72, -86, -18, -39, 0x3e, 0x26, 0x6d, 0x5a, -65, 0x39, -101, 0xe, -72, 0x7b, 0x13, 3, 0x3f, -81, 0x3e, 0x66, -65, 0x73, 0x30, 0x1b, -72, -86, -18, -39, 0x3e, 0x19, 0x3a, 0x56, -65, 0x67, 0x7d, 0x2a, -66, -83, 0x13, 3, 0x3f, -81, 0x3e, 0x66, -65, 0x73, 0x30, 0x1b, -72, -86, -18, -39, 0x3e, -94, -47, 0x61, -65, 0xf, -73, 0x33, -66, -86, -18, -39, 0x3e, -112, 0x4b, 0x70, -65, -18, -109, 0x23, -72, -74, 0x2c, -89, 0x3e, -94, -47, 0x61, -65, 0xf, -73, 0x33, -66, -86, -18, -39, 0x3e, 0x2a, -83, 0x6b, -65, -34, -113, 0x3b, -66, 0x52, 0x2c, -89, 0x3e, -94, -47, 0x61, -65, 0xf, -73, 0x33, -66, -86, -18, -39, 0x3e, -75, -1, 0x5d, -65, 0x45, -16, -73, -66, 0x52, 0x2c, -89, 0x3e, -94, -47, 0x61, -65, 0xf, -73, 0x33, -66, -86, -18, -39, 0x3e, -73, -74, 0x54, -65, -89, 0x3e, -80, -66, -52, -18, -39, 0x3e, 0x19, 0x3a, 0x56, -65, 0x67, 0x7d, 0x2a, -66, -83, 0x13, 3, 0x3f, -73, -74, 0x54, -65, -89, 0x3e, -80, -66, -52, -18, -39, 0x3e, -118, -53, 0x49, -65, -33, 0x32, -89, -66, 0x6a, 0x13, 3, 0x3f, -73, -74, 0x54, -65, -89, 0x3e, -80, -66, -52, -18, -39, 0x3e, -61, -101, 0x35, -65, 0x4e, -71, -14, -66, 0x38, 0x13, 3, 0x3f, -73, -74, 0x54, -65, -89, 0x3e, -80, -66, -52, -18, -39, 0x3e, 0x4c, 0x6f, 0x3f, -65, 0x4d, -37, -1, -66, -18, -18, -39, 0x3e, -75, -1, 0x5d, -65, 0x45, -16, -73, -66, 0x52, 0x2c, -89, 0x3e, 0x4c, 0x6f, 0x3f, -65, 0x4d, -37, -1, -66, -18, -18, -39, 0x3e, 0x75, -54, 0x47, -65, 0x1f, -125, 5, -65, -107, 0x2c, -89, 0x3e, 0x60, -52, 0x22, -65, 0x18, -47, 0x22, -65, 0x31, -17, -39, 0x3e, -106, -25, 0x29, -65, -127, -20, 0x29, -65, -40, 0x2c, -89, 0x3e, 0x60, -52, 0x22, -65, 0x18, -47, 0x22, -65, 0x31, -17, -39, 0x3e, -27, 0x7c, 5, -65, -123, -50, 0x47, -65, -7, 0x2c, -89, 0x3e, 0x60, -52, 0x22, -65, 0x18, -47, 0x22, -65, 0x31, -17, -39, 0x3e, 0x5f, -49, -1, -66, 0x2a, 0x73, 0x3f, -65, 0xf, -17, -39, 0x3e, 0x60, -52, 0x22, -65, 0x18, -47, 0x22, -65, 0x31, -17, -39, 0x3e, -26, -83, -14, -66, 0x6f, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, 0x60, -52, 0x22, -65, 0x18, -47, 0x22, -65, 0x31, -17, -39, 0x3e, 0x26, 0x71, 0x1a, -65, -68, 0x75, 0x1a, -65, 0x59, 0x13, 3, 0x3f, 0x4c, 0x6f, 0x3f, -65, 0x4d, -37, -1, -66, -18, -18, -39, 0x3e, -61, -101, 0x35, -65, 0x4e, -71, -14, -66, 0x38, 0x13, 3, 0x3f, 0, 0, 0, 0, -47, 0x79, 0x59, 0x3f, 0x7b, -122, -24, -66, -38, 0x73, -119, 0x3e, -86, 0xb, 0x4c, 0x3f, -47, 0x5b, 8, -65, -93, 0x74, -119, 0x3e, 0x63, -16, 0x54, 0x3f, 0x3a, -83, -29, -66, -19, 0x2b, -89, 0x3e, 0x10, -54, 0x47, 0x3f, -74, -125, 5, -65, -107, 0x2c, -89, 0x3e, 0x10, 8, 0x4c, 0x3f, -48, 0x27, -38, -66, 0x52, -17, -39, 0x3e, -75, 0x6e, 0x3f, 0x3f, -100, -36, -1, -66, 0x52, -17, -39, 0x3e, 0x1d, -113, 0x41, 0x3f, 0x23, -11, -50, -66, 0x6a, 0x13, 3, 0x3f, 0x3d, -101, 0x35, 0x3f, -99, -70, -14, -66, 0x6a, 0x13, 3, 0x3f, 0x21, 0x75, 0x3b, 0x3f, -96, 0x6e, -56, -66, 0x71, -87, 0xe, 0x3f, 0x3d, -101, 0x35, 0x3f, -99, -70, -14, -66, 0x6a, 0x13, 3, 0x3f, 0x76, -31, 0x2f, 0x3f, 0x59, 0x13, -21, -66, -59, -87, 0xe, 0x3f, 0x7e, 0x70, 0x1a, 0x3f, 0x42, 0x76, 0x1a, -65, 0x7b, 0x13, 3, 0x3f, -51, -111, 0x15, 0x3f, -127, -105, 0x15, -65, 0x19, -86, 0xe, 0x3f, -72, -84, -14, 0x3e, -28, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, 0x1c, 6, -21, 0x3e, -5, -27, 0x2f, -65, -92, -87, 0xe, 0x3f, -72, -84, -14, 0x3e, -28, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, -102, 0x60, -56, 0x3e, 0x10, 0x79, 0x3b, -65, 0x2e, -87, 0xe, 0x3f, -72, -84, -14, 0x3e, -28, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, 0x32, -26, -50, 0x3e, 0x3e, -109, 0x41, -65, 0x38, 0x13, 3, 0x3f, -85, -51, -1, 0x3e, -97, 0x73, 0x3f, -65, 0x74, -17, -39, 0x3e, -113, 0x17, -38, 0x3e, 0x63, 0xc, 0x4c, -65, 0x52, -17, -39, 0x3e, 0xb, 0x7c, 5, 0x3f, 0xb, -49, 0x47, -65, -7, 0x2c, -89, 0x3e, 0x73, -100, -29, 0x3e, -74, -12, 0x54, -65, -7, 0x2c, -89, 0x3e, 0xb, 0x7c, 5, 0x3f, 0xb, -49, 0x47, -65, -7, 0x2c, -89, 0x3e, -110, 0x75, -24, 0x3e, 2, 0x7e, 0x59, -65, -47, 0x75, -119, 0x3e, 0xb, 0x7c, 5, 0x3f, 0xb, -49, 0x47, -65, -7, 0x2c, -89, 0x3e, 4, 0x54, 8, 0x3f, -74, 0x10, 0x4c, -65, -114, 0x75, -119, 0x3e, -51, -26, 0x29, 0x3f, 0x29, -19, 0x29, -65, 0x1b, 0x2d, -89, 0x3e, 0x6a, -123, 0x2d, 0x3f, -25, -117, 0x2d, -65, 0x4b, 0x75, -119, 0x3e, -51, -26, 0x29, 0x3f, 0x29, -19, 0x29, -65, 0x1b, 0x2d, -89, 0x3e, -86, 0xb, 0x4c, 0x3f, -47, 0x5b, 8, -65, -93, 0x74, -119, 0x3e, -51, -26, 0x29, 0x3f, 0x29, -19, 0x29, -65, 0x1b, 0x2d, -89, 0x3e, 0x10, -54, 0x47, 0x3f, -74, -125, 5, -65, -107, 0x2c, -89, 0x3e, -89, -53, 0x22, 0x3f, -65, -47, 0x22, -65, 0x52, -17, -39, 0x3e, -75, 0x6e, 0x3f, 0x3f, -100, -36, -1, -66, 0x52, -17, -39, 0x3e, -89, -53, 0x22, 0x3f, -65, -47, 0x22, -65, 0x52, -17, -39, 0x3e, 0x3d, -101, 0x35, 0x3f, -99, -70, -14, -66, 0x6a, 0x13, 3, 0x3f, -89, -53, 0x22, 0x3f, -65, -47, 0x22, -65, 0x52, -17, -39, 0x3e, 0x7e, 0x70, 0x1a, 0x3f, 0x42, 0x76, 0x1a, -65, 0x7b, 0x13, 3, 0x3f, -89, -53, 0x22, 0x3f, -65, -47, 0x22, -65, 0x52, -17, -39, 0x3e, -72, -84, -14, 0x3e, -28, -97, 0x35, -65, 0x6a, 0x13, 3, 0x3f, -89, -53, 0x22, 0x3f, -65, -47, 0x22, -65, 0x52, -17, -39, 0x3e, -85, -51, -1, 0x3e, -97, 0x73, 0x3f, -65, 0x74, -17, -39, 0x3e, -51, -26, 0x29, 0x3f, 0x29, -19, 0x29, -65, 0x1b, 0x2d, -89, 0x3e, 0xb, 0x7c, 5, 0x3f, 0xb, -49, 0x47, -65, -7, 0x2c, -89, 0x3e, 0, 0, 0, 0, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0, 0, 0, 0, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void FUN_L00_001fde98(int, int, int, char *, int);
void FUN_L16_002e8b98(int n, Vec3f *p1, Vec3f *p2, int unused, float *out);
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

int D_L16_00161FB8[4] __attribute__((section(".sdata"))) = {148, 148, 53, 0};
extern float D_L16_001DF7B0[];
extern float D_L16_001DFEA0[];
extern float D_L16_001E0590[];
float *D_L16_00161FC8[4] __attribute__((section(".sdata"))) = {D_L16_001DF7B0, D_L16_001DFEA0, D_L16_001E0590, 0};
extern float D_L16_001E0810[];
extern float D_L16_001E0CB0[];
extern float D_L16_001E1150[];
float *D_L16_00161FD8[4] __attribute__((section(".sdata"))) = {D_L16_001E0810, D_L16_001E0CB0, D_L16_001E1150, 0};
extern float D_L16_001E12F8[];
extern float D_L16_001E19E8[];
extern float D_L16_001E20D8[];
float *D_L16_00161FE8[4] __attribute__((section(".sdata"))) = {D_L16_001E12F8, D_L16_001E19E8, D_L16_001E20D8, 0};
extern float D_L16_001E2358[];
extern float D_L16_001E25A8[];
extern float D_L16_001E27F8[];
float *D_L16_00161FF8[4] __attribute__((section(".sdata"))) = {D_L16_001E2358, D_L16_001E25A8, D_L16_001E27F8, 0};
void FUN_L16_002e9ce0(void) {
    int i;
    vu1_add_g_sregister(6, get_effect_texture_alt(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        FUN_L16_002e8b98(D_L16_00161FB8[i], ((int *)D_L16_00161FE8)[i],
                         ((int *)D_L16_00161FC8)[i], ((int *)D_L16_00161FD8)[i],
                         D_L16_001E28D0);
        FUN_L00_001fde98(D_L16_00161FB8[i], ((int *)D_L16_00161FC8)[i],
                         ((int *)D_L16_00161FF8)[i], D_L16_001E28D0, 1);
    }
}
/* Bounding spheres for one mesh (two or three entries), then the renderer callback; same source as FUN_L16_002e93b0. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2ea128;

void enqueue_callback_list_1_2ea128(int arg0, int arg1) __asm__("FUN_001f4600");

int D_L16_00162008[4] __attribute__((section(".sdata"))) = {148, 75, 45, 0};
extern float D_L16_001E2DB0[];
extern float D_L16_001E34A0[];
extern float D_L16_001E3828[];
float *D_L16_00162018[4] __attribute__((section(".sdata"))) = {D_L16_001E2DB0, D_L16_001E34A0, D_L16_001E3828, 0};
extern L16SingleSphere_2ea128 D_L16_001E5390[] __attribute__((section(".data")));
void FUN_L16_002ea128(unsigned char *m) {
    int i, j;
    float maxx, maxy, maxz, minx, miny, minz;
    float p[4];
    float *out;
    float *verts;
    int count;
    switch (m[0x20]) {
    case 0: {
        for (i = 0; i <= 2; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00162008[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00162018)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2ea128 *base = D_L16_001E5390;
                L16SingleSphere_2ea128 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00162008[i]; j++) {
                float dist;
                out = (float *)(D_L16_001E5390 + i);
                p[0] = ((float **)D_L16_00162018)[i][j * 3 + 0];
                p[1] = ((float **)D_L16_00162018)[i][j * 3 + 1];
                p[2] = ((float **)D_L16_00162018)[i][j * 3 + 2];
                dist = FUN_001f9b48(p, out);
                if (dist > out[3])
                    out[3] = dist;
            }
        }
        m[0x20] = 1;
        m[0x30] = *(unsigned short *)(m + 0x32) = 255;
        break;
    }
    case 1:
        enqueue_callback_list_1_2ea128(FUN_L16_002e8d40, m);
        break;
    }
}

s32 D_L16_0016CDE4[1] __attribute__((section(".data"))) = {0};
u8 D_L16_001E53C0[1200] = {0};
