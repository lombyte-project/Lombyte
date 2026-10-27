/* Level 01 moby classes: birdbot, peckbot, water_current, cave_door, fighter_jet, door_half, stationary_ship, bomber_jet, water_lily, falling_rock, piston_platform. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"

#include "rnc/rendering/dma_tag.h"

/* One scrolling two-texture overlay layer (0x60 bytes). */
typedef struct {
    u8 pad0[8];
    void *bounds;         /* 0x08: sphere tested against the view */
    s32 shape;            /* 0x0C: what FUN_L01_0021fa98 draws */
    f32 rate0;            /* 0x10 */
    f32 rate1;            /* 0x14 */
    u8 pad18[4];
    s32 frame_step;       /* 0x1C */
    f32 du0;              /* 0x20 */
    f32 dv0;              /* 0x24 */
    f32 du1;              /* 0x28 */
    f32 dv1;              /* 0x2C */
    u8 pad30[4];
    s32 tex0;             /* 0x34 */
    s32 tex1;             /* 0x38 */
    u8 alpha0;            /* 0x3C */
    u8 alpha1;            /* 0x3D */
    u8 pad3E[2];
    f32 u0;               /* 0x40 */
    f32 v0;               /* 0x44 */
    f32 u1;               /* 0x48 */
    f32 v1;               /* 0x4C */
    s32 frame;            /* 0x50 */
    u8 pad54[0xC];
} ScrollLayer;

extern union PacketCursor l01_packet __asm__("D_L01_001611C0") __attribute__((section(".sdata")));
extern u8 D_L01_001CAC80[];
extern s32 sphere_visible(void *, f32) __asm__("FUN_001fa728");
extern u64 texture_tex0(s32) __asm__("FUN_001f44b8");
extern void FUN_L01_00262618(ScrollLayer *);
extern void FUN_L01_0021fa98(s32, s32, s32, s32, s32, s32);

/* Scrolls the UV offsets of count overlay layers (wrapping into [-1, 1]) and, for each one in view, queues its two-texture GS setup and draws it. */
void update_and_draw_scrolling_overlay_layers(s32 count, ScrollLayer *scroll_layer) __asm__("FUN_L01_002b96e0");

void update_and_draw_scrolling_overlay_layers(s32 count, ScrollLayer *scroll_layer) {
    struct DmaTag *tag;
    u64 *packet_payload;
    s32 vis;
    s32 i;

    for (i = 0; i < count; i++, scroll_layer++) {
        scroll_layer->u0 += scroll_layer->du0 * scroll_layer->rate0;
        scroll_layer->v0 += scroll_layer->dv0 * scroll_layer->rate0;
        if (scroll_layer->u0 > 1.0f)
            scroll_layer->u0 -= 1.0f;
        else if (scroll_layer->u0 < -1.0f)
            scroll_layer->u0 += 1.0f;
        if (scroll_layer->v0 > 1.0f)
            scroll_layer->v0 -= 1.0f;
        else if (scroll_layer->v0 < -1.0f)
            scroll_layer->v0 += 1.0f;
        scroll_layer->u1 += scroll_layer->du1 * scroll_layer->rate1;
        scroll_layer->v1 += scroll_layer->dv1 * scroll_layer->rate1;
        if (scroll_layer->u1 > 1.0f)
            scroll_layer->u1 -= 1.0f;
        else if (scroll_layer->u1 < -1.0f)
            scroll_layer->u1 += 1.0f;
        if (scroll_layer->v1 > 1.0f)
            scroll_layer->v1 -= 1.0f;
        else if (scroll_layer->v1 < -1.0f)
            scroll_layer->v1 += 1.0f;
        scroll_layer->frame = (scroll_layer->frame + scroll_layer->frame_step) & 0xFFFFFF;
        vis = sphere_visible(scroll_layer->bounds, 400.0f);
        if (vis == -1)
            continue;
        l01_packet.tag->tag = 0x30000007;
        l01_packet.tag->addr = (u32)D_L01_001CAC80;
        l01_packet.tag->vif0 = 0;
        l01_packet.tag->vif1 = 0x50000007;
        l01_packet.tag++;
        l01_packet.tag->tag = 0x10000005;
        l01_packet.tag->addr = 0;
        l01_packet.tag->vif0 = 0;
        l01_packet.tag->vif1 = 0x50000005;
        tag = l01_packet.tag;
        packet_payload = (u64 *)(tag + 1);
        l01_packet.tag = tag + 1;
        packet_payload[0] = 0x4000000000008001;
        packet_payload[1] = 0xEEEE;
        packet_payload[2] = ((u64)scroll_layer->alpha0 << 32) | 100;
        packet_payload[3] = 0x42;
        packet_payload[4] = ((u64)scroll_layer->alpha1 << 32) | 100;
        packet_payload[5] = 0x43;
        packet_payload[6] = texture_tex0(scroll_layer->tex0);
        packet_payload[7] = 6;
        packet_payload[8] = texture_tex0(scroll_layer->tex1);
        packet_payload[9] = 7;
        l01_packet.tag = tag + 6;
        FUN_L01_00262618(scroll_layer);
        FUN_L01_0021fa98(scroll_layer->shape, 0x70000000, 0x70003000, 0x70001000, 0x70002000, 1 - vis);
    }
}
#include "eetypes.h"

typedef union {
    u128 q;
    f32 f[4];
} EmitVec;

typedef struct {
    EmitVec r[4];
} EmitMtx;

typedef struct {
    u8 pad0[0x70];
    s32 flags;
    u8 pad74[8];
    void *owner;
    EmitVec vel;
    f32 vel_range[4];
    f32 life_min;
    f32 life_max;
    f32 size_min;
    f32 size_max;
    f32 fade[4];
    s32 tex;
    u8 count;
    u8 delay;
    u8 blend;
    u8 alpha;
    s32 timer;
    f32 spread[3];
} Emitter;

typedef struct {
    u8 pad0[0x10];
    EmitVec pos;
    u8 pad20[0x20];
    f32 rot[4];
    u8 pad50[0x28];
    Emitter *vars;
} EmitterMoby;

extern int fast_bsphere_check(void *, f32) __asm__("FUN_001fa728");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void create_spark_particle(void *, void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, s32, u8,
                             u8, u8) __asm__("FUN_L01_0027e900");

void update_spark_particle_emitter(EmitterMoby *self) __asm__("FUN_L01_002bd100");

void update_spark_particle_emitter(EmitterMoby *self) {
    Emitter *emitter_vars;
    EmitVec out;
    EmitVec position_scratch;
    EmitVec emission_position_offset;
    union {
        struct {
            EmitMtx m;
            EmitVec ang;
        } rot;
        struct {
            EmitVec vel;
            EmitMtx m;
        } lin;
    } u;
    f32 size;
    s32 i;

    emitter_vars = self->vars;
    if (current_level_index == 1) {
        position_scratch.q = self->pos.q;
        position_scratch.f[3] = 20.0f;
        if (fast_bsphere_check(&position_scratch, 240.0f) == -1) {
            return;
        }
    }
    emitter_vars->owner = self;
    if (emitter_vars->timer <= 0) {
        emitter_vars->timer = emitter_vars->delay;
        for (i = 0; i < emitter_vars->count; i++) {
            position_scratch.q = self->pos.q;
            emission_position_offset.f[0] = random_float_between(-emitter_vars->spread[0], emitter_vars->spread[0]);
            emission_position_offset.f[1] = random_float_between(-emitter_vars->spread[1], emitter_vars->spread[2]);
            emission_position_offset.f[2] = random_float_between(-emitter_vars->spread[2], emitter_vars->spread[2]);
            if (emitter_vars->flags & 0x2000) {
                euler_rotation_matrix(&u.rot.m, self->rot);
                transform_vector(&emission_position_offset, &emission_position_offset, &u.rot.m);
            }
            fast_vec_add(&position_scratch, &position_scratch, &emission_position_offset);
            if (emitter_vars->flags & 0x1000) {
                u.rot.ang.f[0] =
                    random_float_between(-emitter_vars->vel_range[0], emitter_vars->vel_range[0]) * 3.1415927f / 180.0f;
                u.rot.ang.f[1] =
                    random_float_between(-emitter_vars->vel_range[1], emitter_vars->vel_range[1]) * 3.1415927f / 180.0f;
                u.rot.ang.f[2] =
                    random_float_between(-emitter_vars->vel_range[2], emitter_vars->vel_range[2]) * 3.1415927f / 180.0f;
                if (emitter_vars->flags & 0x4000) {
                    u.rot.ang.f[0] = fast_add_rotations(u.rot.ang.f[0], self->rot[0]);
                    u.rot.ang.f[1] = fast_add_rotations(u.rot.ang.f[0], self->rot[1]);
                    u.rot.ang.f[2] = fast_add_rotations(u.rot.ang.f[0], self->rot[2]);
                }
                euler_rotation_matrix(&u.rot.m, &u.rot.ang);
                transform_vector(&out, &emitter_vars->vel, &u.rot.m);
                size = random_float_between(emitter_vars->size_min, emitter_vars->size_max);
                create_spark_particle(emitter_vars, &position_scratch, out.f[0], out.f[1], out.f[2], size,
                                 random_float_between(emitter_vars->life_min, emitter_vars->life_max), emitter_vars->fade[0],
                                 emitter_vars->fade[1], emitter_vars->fade[2], emitter_vars->fade[3], emitter_vars->tex, emitter_vars->blend, emitter_vars->alpha,
                                 0xFF);
            } else {
                u.lin.vel.f[0] = random_float_between(emitter_vars->vel.f[0], emitter_vars->vel_range[0]) * frame_scale;
                u.lin.vel.f[1] = random_float_between(emitter_vars->vel.f[1], emitter_vars->vel_range[1]) * frame_scale;
                u.lin.vel.f[2] = random_float_between(emitter_vars->vel.f[2], emitter_vars->vel_range[2]) * frame_scale;
                if (emitter_vars->flags & 0x4000) {
                    euler_rotation_matrix(&u.lin.m, self->rot);
                    transform_vector(&u.lin.vel, &u.lin.vel, &u.lin.m);
                }
                size = random_float_between(emitter_vars->size_min, emitter_vars->size_max);
                create_spark_particle(emitter_vars, &position_scratch, u.lin.vel.f[0], u.lin.vel.f[1], u.lin.vel.f[2], size,
                                 random_float_between(emitter_vars->life_min, emitter_vars->life_max), emitter_vars->fade[0],
                                 emitter_vars->fade[1], emitter_vars->fade[2], emitter_vars->fade[3], emitter_vars->tex, emitter_vars->blend, emitter_vars->alpha,
                                 0xFF);
            }
        }
    } else {
        emitter_vars->timer--;
    }
}
#include "eetypes.h"

typedef union {
    u128 q;
    f32 f[4];
} SplashVec;

typedef struct {
    u32 c[6];
} SplashColors;

typedef struct {
    u8 pad0[0x10];
    u128 pos;
    u8 pad20[0xA0];
    u8 mtx[0x40];
} SplashMoby_u;

SplashColors D_L01_0020AF50 = {{0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}};
SplashColors D_L01_0020AF68 = {{0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}};
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int random_int_between(int lo, int hi) __asm__("FUN_L00_00257b90");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void spawn_effect_particle_with_target_scalar(void *, void *, u32, u32, f32, s32) __asm__("FUN_L00_0026ced0");

void spawn_birdbot_movement_particles(SplashMoby_u *moby, u128 *origin) __asm__("FUN_L01_002e6518");

void spawn_birdbot_movement_particles(SplashMoby_u *moby, u128 *origin) {
    SplashVec base;
    SplashVec world_motion_delta;
    SplashVec vector_scratch;
    SplashVec rel;
    SplashColors inner;
    SplashColors outer;
    SplashVec *bp;
    u32 *inner_color_entry;
    u32 *outer_color_entry;
    f32 size;
    s32 i;

    base.q = *origin;
    bp = &base;
    for (i = 2; i >= 0; i--) {
        vector_scratch.q = 0;
        vector_scratch.f[0] = random_float_between(1.0f, -1.0f) * (frame_time * 0.2f);
        vector_scratch.f[1] = random_float_between(1.0f, -1.0f) * (frame_time * 0.2f);
        vector_scratch.f[2] = random_float_between(0.8f, 1.2f) * (frame_time * -7.0f);
        world_motion_delta.q = vector_scratch.q;
        inner = D_L01_0020AF50;
        outer = D_L01_0020AF68;
        moby_point_position(moby, 1, &vector_scratch);
        transform_vector_by_basis(&world_motion_delta, &world_motion_delta, moby->mtx);
        fast_vec_sub(&rel, &moby->pos, bp);
        fast_vec_add(&world_motion_delta, &world_motion_delta, &rel);
        inner_color_entry = &inner.c[random_integer_below(6)];
        outer_color_entry = &outer.c[random_integer_below(6)];
        size = random_float_between(105000.0f, 157500.0f);
        spawn_effect_particle_with_target_scalar(&vector_scratch, &world_motion_delta, *inner_color_entry, *outer_color_entry, size, scale_game_frames(random_int_between(5, 0xF)));
    }
}
#include "qcopy.h"

typedef struct {
    s32 count;
    u8 pad4[0xC];
    struct {
        f32 x, y, z, w;
    } pts[1];
} PathNodes;

typedef struct {
    s32 cur;
    s8 dir;
    u8 pad5[0xB];
    PathNodes *path;
} PathFollow;

typedef struct {
    u8 pad0[0x38];
    s32 w38;
    u8 pad3C[0x114];
    PathFollow follow;
    u8 pad164[0x3C];
    f32 radius;
    u8 pad1A4[8];
    s16 ordered;
    s16 stops_only;
    u8 pad1B0[0x32];
    s16 h1E2;
} PathPickVars;

typedef struct {
    u8 pad0[0x10];
    SplashVec pos;
    u8 pad20[0x58];
    PathPickVars *vars;
} PathPickMoby;

typedef struct {
    SplashVec pos;
    SplashVec v10;
    SplashVec v20;
    SplashVec v30;
    void *moby;
    s32 kind;
} PathPickTarget;

extern void FUN_L01_00274b78(void *, void *, f32);
extern float FUN_001f9b48(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");

/* Picks the path node the follower should head for: an ordered path just steps to the next stop; otherwise the
 * nearest node and its two neighbours are scored (distance, or heading change for slow movers) and the best one
 * is written to *idx. Returns nonzero when the pick differs from the nearest node. */
int select_birdbot_path_node(void *path_follower_entity, int *selected_path_node_index) __asm__("FUN_L01_002e6790");

int select_birdbot_path_node(void *path_follower_entity, int *selected_path_node_index) {
    PathPickMoby *path_follower_moby = path_follower_entity;
    PathPickTarget target;
    SplashVec pt;
    PathPickVars *path_pick_vars;
    PathFollow *f;
    s32 fast;
    s32 preceding_node_index;
    s32 next;
    s32 best;
    s32 last;
    s32 i;
    f32 candidate_node_distance;
    f32 node_target_heading_difference;
    f32 best_d;
    f32 best_a;
    f32 target_search_radius;
    f32 node_follower_distance;

    path_pick_vars = path_follower_moby->vars;
    fast = 0;
    f = &path_pick_vars->follow;
    if (path_pick_vars->w38 != 0 || path_pick_vars->h1E2 != 0) {
        fast = 1;
    }
    target_search_radius = fast ? path_pick_vars->radius * 2.0f : path_pick_vars->radius;
    preceding_node_index = 0;
    next = 0;
    FUN_L01_00274b78(path_follower_moby, &target, target_search_radius);
    best_d = 10000.0f;
    best_a = 0.0f;
    best = 0;
    if (path_pick_vars->ordered != 0) {
        do {
            if (*selected_path_node_index < f->path->count - 1) {
                *selected_path_node_index = (*selected_path_node_index + f->path->count + f->dir) % f->path->count;
            }
            if (*selected_path_node_index == f->path->count - 1) {
                break;
            }
        } while (f->path->pts[*selected_path_node_index].w != 1.0f);
        return 1;
    }
    last = 0;
    for (i = 0; i < f->path->count; i++) {
        if (path_pick_vars->stops_only == 0 || f->path->pts[i].w == 1.0f) {
            node_follower_distance = vector_distance(&f->path->pts[i], &path_follower_moby->pos);
            if (node_follower_distance < best_d) {
                preceding_node_index = last;
                best_d = node_follower_distance;
                best = i;
            } else if (last == best) {
                next = i;
            }
            last = i;
        }
    }
    i = best;
    if (i == f->path->count - 1) {
        next = 0;
    } else if (i == 0) {
        preceding_node_index = f->path->count - 1;
    }
    best_d = 0.0f;
    best = 0;
    if (fast) {
        best_d = 10000.0f;
    }
    *selected_path_node_index = 0;
    for (;;) {
        if (*selected_path_node_index == preceding_node_index || *selected_path_node_index == i || *selected_path_node_index == next) {
            if (fast) {
                qcopy(&pt, &f->path->pts[*selected_path_node_index]);
                candidate_node_distance = FUN_001f9b48(&target, &pt);
                if ((path_pick_vars->stops_only == 0 || pt.f[3] == 1.0f) && candidate_node_distance < best_d) {
                    best = *selected_path_node_index;
                    best_d = candidate_node_distance;
                }
            } else {
                qcopy(&pt, &f->path->pts[*selected_path_node_index]);
                node_target_heading_difference = fast_difference_between_rotations(fast_arc_tan(pt.f[0] - path_follower_moby->pos.f[0], pt.f[1] - path_follower_moby->pos.f[1]),
                                                      fast_arc_tan(target.pos.f[0] - path_follower_moby->pos.f[0],
                                                                   target.pos.f[1] - path_follower_moby->pos.f[1]));
                candidate_node_distance = FUN_001f9b48(&path_follower_moby->pos, &pt);
                if (path_pick_vars->stops_only == 0 || pt.f[3] == 1.0f) {
                    if (node_target_heading_difference < 1.5707964f) {
                        if (best_a < node_target_heading_difference) {
                            best = *selected_path_node_index;
                            best_d = candidate_node_distance;
                            best_a = node_target_heading_difference;
                        }
                    } else if ((candidate_node_distance >= 12.0f && (candidate_node_distance < best_d || best_d < 12.0f)) || (candidate_node_distance < 12.0f && best_d < candidate_node_distance)) {
                        best = *selected_path_node_index;
                        best_d = candidate_node_distance;
                        best_a = node_target_heading_difference;
                    }
                }
            }
        }
        *selected_path_node_index = (*selected_path_node_index + f->path->count + f->dir) % f->path->count;
        if (*selected_path_node_index == 0) {
            *selected_path_node_index = best;
            break;
        }
    }
    /* i is reused for the result so it outlives best (keeps retail's copy of the nearest node) */
    i = best != i;
    return i;
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Birdbot update: follows its path, turns on and shoots at the hero, flees when hurt, and blows up when it dies or leaves the level. */
/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002BA898.c: func_L01_002E7FC8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    u128 pos;
    char pad20[0xA0];
    char mtx[0x40];
} SplashMoby;

typedef struct {
    float f[4];
} PeVec __attribute__((aligned(16)));

typedef struct {
    PeVec pos;
    PeVec v10;
    PeVec v20;
    PeVec v30;
    void *moby;
    int kind;
} PeTarget;

typedef struct {
    int count;
    int pad4[3];
    PeVec pts[1];
} PePath;

typedef struct {
    int idx;
    signed char dir;
    char pad5[0xB];
    PePath *path;
    int path_id;
} PeCursor;

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
    PeVec v40;
    char pad50[8];
    unsigned char b58;
    char pad59;
    unsigned char b5A;
    char pad5B[5];
    char x60[7];
    unsigned char b67;
    char pad68[8];
    char x70[0x10];
    float f80;
    float f84;
    float f88;
    float f8C;
    int w90;
    int w94;
    float f98;
    char pad9C[0x11];
    unsigned char bAD;
    char padAE[0x12];
    float fC0;
    float fC4;
    char padC8[8];
    char xD0[8];
    float fD8;
    float fDC;
    char padE0[8];
    float fE8;
    float fEC;
    char padF0[4];
    float fF4;
    char padF8[0x14];
    float f10C;
    float f110;
    float f114;
    char pad118[8];
    PeCursor side;
    char pad138[0x18];
    PeCursor main;
    char pad168[0x18];
    PeVec v180;
    int w190;
    float f194;
    float f198;
    char pad19C[4];
    float f1A0;
    short h1A4;
    short h1A6;
    float f1A8;
    short h1AC;
    short h1AE;
    char pad1B0[4];
    int w1B4;
    float f1B8;
    float f1BC;
    int w1C0;
    int w1C4;
    int w1C8;
    float f1CC;
    PeVec v1D0;
    unsigned short h1E0;
    short h1E2;
    int w1E4;
    int w1E8;
    short h1EC;
    short h1EE;
    char pad1F0[0x10];
    unsigned char x200[1];
} PeVars;

typedef struct PeMoby {
    char pad0[0x10];
    PeVec pos;
    unsigned char state;
    char pad21[3];
    char *w24;
    char pad28[9];
    unsigned char b31;
    unsigned short h32;
    unsigned short h34;
    char pad36[2];
    long l38;
    PeVec rot;
    char pad50[2];
    unsigned char b52;
    unsigned char b53;
    char pad54[4];
    float f58;
    char pad5C[0x14];
    unsigned char b70;
    char pad71[7];
    PeVars *vars;
    char pad7C[0x18];
    int w94;
    char pad98[0xC];
    unsigned char bA4;
    char padA5;
    short hA6;
    char padA8[0xA];
    short hB2;
} PeMoby;

typedef struct {
    char pad0[0x10];
    PeVec pos;
    struct PeMoby *src;
} PeHit;

typedef struct {
    char pad0[0x80];
    PeVec pos;
    char pad90[0x1000];
    struct PeMoby *f1090;
    char pad1094[0xFEC];
    struct PeMoby *f2080;
} PeHero;

extern char *D_L01_001B0930_u[] __asm__("D_L01_001B0930");
extern char *D_L01_001600EC;
extern char *spawn_colored_half_scale_moby(void *, void *, void *, float, float, int) __asm__("FUN_L01_002fa068");
extern char D_L01_0020AF80[];
extern float absolute_float(float) __asm__("func_001F99C0");
extern float D_L01_0015F580[] __attribute__((section(".sdata")));
extern float FUN_L04_001f1978(float);
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_001f9b48(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_cf(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float compute_interpolated_record_value(void *);
extern float fast_cos(float);
extern float fast_sin(float);
extern float probe_ground_height(void *, int, float);
extern float vector_length_xy_cf(void *) __asm__("FUN_001f9b20");
extern float vector_length_xyz(void *);
extern int cutscene_active __asm__("D_L01_0015F404");
extern int DebugPrint();
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_0025a478_u(void *, void *, void *, int, int *, float *, int,
                              int) __asm__("FUN_L00_0025a478");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int hero_set_state(int, int) __asm__("FUN_L01_0023cf98");
extern int select_birdbot_path_node(void *, int *) __asm__("FUN_L01_002e6790");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int scale_game_frames(int);
extern int tick_countdown_32(void *);
extern int truncate_float_to_s32() __asm__("FUN_001fa6d0");
extern short D_L01_00161950 __attribute__((sda));
extern short D_L01_00161954 __attribute__((sda));
extern short D_L01_0016195C __attribute__((sda));
extern void *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void *FUN_L00_00263fd8_u(char *, int, float *, void *, int, int, float, float *, float *,
                                float *) __asm__("FUN_L00_00263fd8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278_cf(char *moby, float *vel, float target, float k, float d,
                                float max) __asm__("FUN_L00_00258278");
extern void FUN_L00_0025ab48_u(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void FUN_L00_0025c558_u(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void FUN_L00_0025d458_u(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00263ac8_u(float, int, int, unsigned char *) __asm__("FUN_L00_00263ac8");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern void FUN_L01_0026d930_u(char *p) __asm__("FUN_L01_0026d930");
extern void set_range_centered_on_value(char *arg, float value) __asm__("FUN_L01_0026eff8");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation_cf(void *, int, int, int) __asm__("FUN_00212f90");
extern void build_spherical_offset_cf(void *, float, float, float) __asm__("FUN_00214db0");
extern void clear_vector(void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void normalize_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
void FUN_L01_002e6518_c(SplashMoby *m, u128 *origin) __asm__("FUN_L01_002e6518");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void birdbot_update(PeMoby *m) __asm__("FUN_L01_002e6bf0");
void birdbot_update(PeMoby *birdbot_moby) {
    enum {
        BIRDBOT_STATE_INITIALIZE = 0,
        BIRDBOT_STATE_TRACK_TARGET_AND_SELECT_ACTION = 1,
        BIRDBOT_STATE_WAIT_FOR_TRIGGER = 2,
        BIRDBOT_STATE_DEPLOY = 3,
        BIRDBOT_STATE_FIRE_BURST = 6,
        BIRDBOT_STATE_RECOVER_FROM_BURST = 7,
        BIRDBOT_STATE_BEGIN_PATH_MOVEMENT = 9,
        BIRDBOT_STATE_MOVE_TO_PATH_NODE = 10,
        BIRDBOT_STATE_BRAKE_AND_FACE_TARGET = 11,
        BIRDBOT_STATE_NONFATAL_KNOCKBACK = 12,
        BIRDBOT_STATE_FATAL_KNOCKBACK = 14
    };
    enum {
        BIRDBOT_HIT_REACTION_KEEP_CURRENT_STATE = 0,
        BIRDBOT_HIT_REACTION_FATAL_KNOCKBACK = 1,
        BIRDBOT_HIT_REACTION_FORCE_FATAL_KNOCKBACK = 2,
        BIRDBOT_HIT_REACTION_NONFATAL_KNOCKBACK = 3,
        BIRDBOT_HIT_REACTION_LOW_SPEED_NONFATAL_KNOCKBACK = 4,
        BIRDBOT_HIT_REACTION_LOW_SPEED_NONFATAL_KNOCKBACK_NO_BLEND = 5,
        BIRDBOT_HIT_REACTION_ANIMATED_NONFATAL_KNOCKBACK = 6,
        BIRDBOT_HIT_REACTION_NO_KNOCKBACK = 7,
        BIRDBOT_HIT_REACTION_SKIP_KNOCKBACK_SETUP = 8,
        BIRDBOT_HIT_REACTION_CONTINUE_WITHOUT_KNOCKBACK_SETUP = 9,
        BIRDBOT_HIT_REACTION_BYPASS_KNOCKBACK_SETUP = 10,
        BIRDBOT_HIT_REACTION_OMIT_KNOCKBACK_SETUP = 11
    };
    PeVars *birdbot_vars;
    PeVec old;
    PeTarget selected_combat_target;
    PeVec va;
    PeVec vb;
    PeVec vc;
    int idx;
    int hits;
    float dmg;
    float knockback_heading;
    int flag;
    int path_reposition_requested;
    float frame;
    PeHit *hit;
    int kind;
    float ground;
    float target_search_radius;

    flag = 0;
    path_reposition_requested = 0;
    birdbot_vars = birdbot_moby->vars;
    idx = birdbot_vars->main.idx;
    frame = compute_interpolated_record_value(birdbot_moby);
    if (birdbot_vars->w38 != 0 || birdbot_vars->h1E2 != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8_u(2.5f, (int)birdbot_moby, 2, birdbot_vars->x200);
    qcopy(&old, &birdbot_moby->pos);
    if (birdbot_vars->w1C8 > 0) {
        birdbot_vars->w1C8--;
    }
    if (birdbot_moby->state != 0) {
        path_reposition_requested = select_birdbot_path_node(birdbot_moby, &idx);
    }
    target_search_radius = flag ? birdbot_vars->f1A0 * 2.0f : birdbot_vars->f1A0;
    select_moby_combat_target(birdbot_moby, &selected_combat_target, target_search_radius);
    hit = find_moby_hit(birdbot_moby, 0x210000, 0);
    kind = FUN_L00_0025a478_u(birdbot_moby, hit, &birdbot_vars->f20, 0, &hits, &dmg, 0, 4);
    fast_dec_timer_s16(&birdbot_vars->h26);
    if (hit != 0 && birdbot_vars->h26 == 0) {
        if (hit->src != 0) {
            struct Hero *player = &hero;

            if (hit->src != player->items[0].moby) {
                knockback_heading = FUN_001f9e90_cf(birdbot_moby->pos.f[0] - hit->src->pos.f[0],
                                      birdbot_moby->pos.f[1] - hit->src->pos.f[1]);
            } else {
                knockback_heading = FUN_001f9e90_cf(birdbot_moby->pos.f[0] - player->motion.pos.f[0], birdbot_moby->pos.f[1] - player->motion.pos.f[1]);
            }
        } else {
            struct Hero *player = &hero;

            knockback_heading = FUN_001f9e90_cf(birdbot_moby->pos.f[0] - player->motion.pos.f[0], birdbot_moby->pos.f[1] - player->motion.pos.f[1]);
        }
        birdbot_vars->h1E2 = scale_game_frames(0x258);
        birdbot_vars->f20 -= dmg;
        if (birdbot_vars->f20 <= 0.0f) {
            kind = 1;
        }
        switch (kind) {
        case BIRDBOT_HIT_REACTION_KEEP_CURRENT_STATE:
        case BIRDBOT_HIT_REACTION_NO_KNOCKBACK:
        case BIRDBOT_HIT_REACTION_SKIP_KNOCKBACK_SETUP:
        case BIRDBOT_HIT_REACTION_CONTINUE_WITHOUT_KNOCKBACK_SETUP:
        case BIRDBOT_HIT_REACTION_BYPASS_KNOCKBACK_SETUP:
        case BIRDBOT_HIT_REACTION_OMIT_KNOCKBACK_SETUP:
            break;
        case BIRDBOT_HIT_REACTION_FATAL_KNOCKBACK:
        case BIRDBOT_HIT_REACTION_FORCE_FATAL_KNOCKBACK:
            birdbot_vars->f98 = 0.3f;
            birdbot_vars->f84 = frame_time_sq * 10.0f;
            birdbot_vars->f80 = frame_time_sq * 40.0f;
            birdbot_vars->f88 = square_root(birdbot_vars->f84 * 2.0f * 10.0f);
            birdbot_vars->f8C = frame_time * 15.0f;
            birdbot_vars->fC0 = 5.0f;
            birdbot_vars->fC4 = 10.0f;
            birdbot_vars->w94 = 1;
            birdbot_vars->bAD = 0;
            *(u128 *)&va = *(u128 *)&hit->pos;
            FUN_L00_0025ab48_u(&va, &knockback_heading, &birdbot_vars->f88, &birdbot_vars->f8C);
            FUN_L00_0025c558_u(knockback_heading, birdbot_moby, birdbot_vars->x70, 9, scale_game_frames(2), 0);
            birdbot_vars->b67 = 0xB4;
            birdbot_moby->state = 0xE;
            birdbot_moby->h34 &= 0xEFFF;
            FUN_L00_0025d458_u(birdbot_moby, (short *)birdbot_vars->x60);
            birdbot_vars->h26 = scale_game_frames(0x3C);
            break;
        case BIRDBOT_HIT_REACTION_NONFATAL_KNOCKBACK:
        case BIRDBOT_HIT_REACTION_ANIMATED_NONFATAL_KNOCKBACK:
            birdbot_vars->w90 = 0x200;
            birdbot_vars->b67 = 0x78;
            birdbot_vars->f98 = 0.5f;
            birdbot_vars->f80 = 0.008f;
            birdbot_vars->f88 = square_root(birdbot_vars->f84 * 2.0f * 1.5f);
            birdbot_vars->w94 = 1;
            birdbot_vars->fC0 = 5.0f;
            birdbot_vars->f8C = frame_time * 8.5f;
            birdbot_vars->fC4 = 10.0f;
            birdbot_vars->bAD = 0;
            *(u128 *)&va = *(u128 *)&hit->pos;
            FUN_L00_0025ab48_u(&va, &knockback_heading, &birdbot_vars->f88, &birdbot_vars->f8C);
            FUN_L00_0025c558_u(knockback_heading, birdbot_moby, birdbot_vars->x70, 0xC, 6, 0);
            if (birdbot_moby->b53 != 0xB) {
                blend_moby_animation_cf(birdbot_moby, 0xB, 0, scale_game_frames(6));
            }
            birdbot_moby->state = 0xC;
            FUN_L00_0025d458_u(birdbot_moby, (short *)birdbot_vars->x60);
            break;
        case BIRDBOT_HIT_REACTION_LOW_SPEED_NONFATAL_KNOCKBACK:
        case BIRDBOT_HIT_REACTION_LOW_SPEED_NONFATAL_KNOCKBACK_NO_BLEND:
            birdbot_vars->w90 = 0x200;
            birdbot_vars->b67 = 0xFA;
            birdbot_vars->f80 = 0.008f;
            birdbot_vars->f88 = square_root(birdbot_vars->f84 * 2.0f * 1.5f);
            birdbot_vars->w94 = 1;
            birdbot_vars->f8C = frame_time * 5.0f;
            birdbot_vars->fC0 = 7.0f;
            birdbot_vars->fC4 = 13.5f;
            birdbot_vars->bAD = 0;
            *(u128 *)&va = *(u128 *)&hit->pos;
            FUN_L00_0025ab48_u(&va, &knockback_heading, &birdbot_vars->f88, &birdbot_vars->f8C);
            FUN_L00_0025c558_u(knockback_heading, birdbot_moby, birdbot_vars->x70, 0xE, 6, 0);
            birdbot_moby->state = 0xC;
            FUN_L00_0025d458_u(birdbot_moby, (short *)birdbot_vars->x60);
            break;
        }
    }
    birdbot_moby->bA4 = 0xFF;
    if (birdbot_vars->w38 != 0) {
        birdbot_vars->h1E2 = scale_game_frames(0x258);
    }
    birdbot_vars->w38 = 0;
    fast_dec_timer_s16(&birdbot_vars->h1E2);
    switch (birdbot_moby->state) {
    case BIRDBOT_STATE_INITIALIZE: {
        PeCursor *c;
        int i;
        float fall;
        float turn;

        c = &birdbot_vars->main;
        if (birdbot_vars->main.path_id == -1) {
            DebugPrint(D_L01_0020AF80, birdbot_moby->hA6, birdbot_moby->hB2);
            mark_moby_for_removal(birdbot_moby);
            return;
        }
        birdbot_vars->main.path = (PePath *)D_L01_001B0930_u[birdbot_vars->main.path_id];
        c->idx = 0;
        if (birdbot_vars->side.path_id >= 0) {
            birdbot_vars->side.path = (PePath *)D_L01_001B0930_u[birdbot_vars->side.path_id];
            birdbot_vars->side.idx = 0;
            qcopy(&birdbot_moby->pos, &birdbot_vars->side.path->pts[0]);
        } else {
            qcopy(&birdbot_moby->pos, &birdbot_vars->main.path->pts[0]);
            if (birdbot_vars->w1B4 == 0) {
                birdbot_vars->side.path = 0;
                birdbot_moby->pos.f[2] += 20.0f;
            }
        }
        birdbot_vars->h1AE = 0;
        for (i = 0; i < c->path->count; i++) {
            if (c->path->pts[i].f[3] == 1.0f) {
                birdbot_vars->h1AE = 1;
            }
        }
        birdbot_moby->h34 |= 0x1000;
        birdbot_vars->b29 = 0;
        birdbot_moby->l38 = ((struct PeMoby *)hero.moby)->l38;
        birdbot_vars->b58 = 8;
        birdbot_vars->b5A = 8;
        FUN_L01_0026d930_u(birdbot_vars->xD0);
        turn = frame_time_sq * 5.0f;
        fall = *(float *)&D_L01_00161954 * frame_time;
        birdbot_vars->f10C = 0.03f;
        birdbot_vars->f110 = 0.3f;
        birdbot_vars->f114 = 0.25f;
        birdbot_vars->fEC = turn;
        birdbot_vars->fF4 = fall;
        birdbot_vars->fDC = 2.0f;
        birdbot_vars->fD8 = 2.0f;
        birdbot_vars->f20 = 2.0f;
        birdbot_vars->h24 = func_001FA898_r(birdbot_vars->f20);
        birdbot_vars->b29 = 0;
        birdbot_vars->b28 = 1;
        clear_vector(&birdbot_vars->v40);
        birdbot_vars->w1C8 = -1;
        if (birdbot_moby->b53 != 0) {
            blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(5));
        }
        birdbot_vars->h1E0 = birdbot_moby->h32;
        if (birdbot_moby->pos.f[2] - probe_ground_height(&birdbot_moby->pos, 0, 0.5f) > 0.5f) {
            birdbot_moby->w94 = 0;
            birdbot_moby->h34 |= 0x41;
        }
        birdbot_moby->state = 2;
        break;
    }
    case BIRDBOT_STATE_TRACK_TARGET_AND_SELECT_ACTION: {
        PeCursor *c;
        float d;
        float a;

        c = &birdbot_vars->main;
        a = FUN_001f9e90_cf(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0], selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1]);
        FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.02f, 0.3f, 0.1f);
        tick_countdown_32(&birdbot_vars->w190);
        d = vector_distance(&birdbot_moby->pos, &selected_combat_target);
        if ((flag ? d < birdbot_vars->f1A0 * 2.0f : d < birdbot_vars->f1A0) &&
            absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) < 2.0f && birdbot_moby->b31 != 0 &&
            (!flag || path_reposition_requested == 0) && birdbot_vars->w190 == 0 &&
            fast_difference_between_rotations(birdbot_moby->rot.f[2], FUN_001f9e90_cf(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                                                      selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1])) < 0.08726646f) {
            moby_point_position(birdbot_moby, 0, &va);
            *(u128 *)&vb = *(u128 *)&birdbot_moby->pos;
            vb.f[2] = va.f[2];
            if (collision_line(&vb, &va, 0, birdbot_moby, 0) == 0) {
                birdbot_vars->h1A4 = 0;
                birdbot_vars->f198 = 4.0f;
                birdbot_moby->state = 6;
                if (birdbot_moby->b53 != 4) {
                    blend_moby_animation_cf(birdbot_moby, 4, 0, scale_game_frames(6));
                }
                birdbot_vars->w190 = 0;
            } else {
                birdbot_vars->w190 = scale_game_frames(0x3C);
            }
        } else if (flag || (vector_distance(&birdbot_moby->pos, &selected_combat_target) < 4.0f &&
                            absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) < 6.0f)) {
            if (path_reposition_requested != 0 && FUN_001f9b48(&birdbot_moby->pos, (PeVec *)c->path + (idx + 1)) > 0.25f) {
                if (birdbot_vars->h1AC == 0 || c->idx < c->path->count - 1) {
                    int i;
                    float f;

                    if (birdbot_moby->b53 != 8) {
                        blend_moby_animation_cf(birdbot_moby, 8, 0, scale_game_frames(10));
                    }
                    i = idx;
                    f = *(float *)&D_L01_0016195C;
                    c->idx = i;
                    birdbot_moby->f58 = f;
                    birdbot_moby->state = 9;
                }
            }
        }
        if (birdbot_vars->w1C8 == 0 && birdbot_vars->w1C4 != -1) {
            FUN_L00_002eac18(0);
            hero_set_state(HERO_STATE_IDLE, 1);
            cutscene_active = 0;
            birdbot_vars->w1C4 = -1;
        }
        break;
    }
    case BIRDBOT_STATE_WAIT_FOR_TRIGGER:
        if (vector_distance(&birdbot_moby->pos, &selected_combat_target) < birdbot_vars->f1A8 ||
            is_point_inside_clip_volume(&hero.motion.pos, birdbot_vars->w1C0) != 0 || birdbot_vars->w1E8 != 0) {
            if (birdbot_vars->w1C4 != -1) {
                char *p;

                hero_set_state(0x1F, 0);
                p = D_L01_001600EC + (birdbot_vars->w1C4 << 7);
                jet_mount(p + 0x30, p + 0x70, 0, 0, 0);
                cutscene_active = 1;
                birdbot_vars->w1C8 = scale_game_frames(0x168);
            }
            birdbot_moby->w94 = *(int *)(birdbot_moby->w24 + 0x10);
            birdbot_moby->h34 &= 0xFFBE;
            if (birdbot_moby->b53 != 8) {
                blend_moby_animation_cf(birdbot_moby, 8, 0, scale_game_frames(10));
            }
            birdbot_moby->state = 3;
        }
        break;
    case BIRDBOT_STATE_DEPLOY:
        if (birdbot_vars->side.path != 0) {
            PeCursor *side_path_cursor;
            PePath *path;
            int next;
            float a;

            side_path_cursor = &birdbot_vars->side;
            next = (side_path_cursor->idx + side_path_cursor->path->count + side_path_cursor->dir) % side_path_cursor->path->count;
            a = FUN_001f9e90_cf(side_path_cursor->path->pts[next].f[0] - birdbot_moby->pos.f[0],
                                side_path_cursor->path->pts[next].f[1] - birdbot_moby->pos.f[1]);
            FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.01f, 0.3f, 0.1f);
            path = side_path_cursor->path;
            qcopy(&vb, &path->pts[next]);
            if (side_path_cursor->idx == path->count - 1) {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            } else {
                subtract_vector_xyz(&va, &vb, &birdbot_moby->pos);
                normalize_vector_xyz(&va, &va, frame_time * 5.0f);
                add_vector_xyz(&birdbot_moby->pos, &birdbot_moby->pos, &va);
                if (FUN_001f9b48(&vb, &birdbot_moby->pos) < 0.5f) {
                    side_path_cursor->idx = next;
                }
            }
        } else if (birdbot_vars->w1E4 != -1) {
            char *p;
            float a;
            float deployment_step_length;
            float z;

            p = (char *)((birdbot_vars->w1E4 << 7) + (int)D_L01_001600EC);
            a = FUN_001f9e90_cf(*(float *)(p + 0x30) - birdbot_moby->pos.f[0],
                                *(float *)(p + 0x34) - birdbot_moby->pos.f[1]);
            FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.01f, 0.3f, 0.1f);
            deployment_step_length = vector_length_xyz(&birdbot_vars->v1D0);
            z = birdbot_vars->v1D0.f[2];
            normalize_vector_xyz(&birdbot_vars->v1D0, &birdbot_vars->v1D0, deployment_step_length * 0.95f);
            subtract_vector_xyz(&vb, D_L01_001600EC + (birdbot_vars->w1E4 << 7) + 0x30, &birdbot_moby->pos);
            normalize_vector_xyz(&vb, &vb, deployment_step_length * 0.05f);
            add_vector_xyz(&birdbot_vars->v1D0, &birdbot_vars->v1D0, &vb);
            if (frame_time * 10.0f < deployment_step_length) {
                normalize_vector_xyz(&vb, &birdbot_vars->v1D0, -(frame_time_sq * 10.0f));
                add_vector_xyz(&birdbot_vars->v1D0, &birdbot_vars->v1D0, &vb);
            } else if (deployment_step_length < frame_time * 5.0f) {
                normalize_vector_xyz(&vb, &birdbot_vars->v1D0, frame_time_sq * 5.0f);
                add_vector_xyz(&birdbot_vars->v1D0, &birdbot_vars->v1D0, &vb);
            }
            if (fast_dec_timer_s16(&birdbot_vars->h1EC) == 0) {
                birdbot_vars->v1D0.f[2] = z;
            }
            add_vector_xyz(&birdbot_moby->pos, &birdbot_moby->pos, &birdbot_vars->v1D0);
            if (FUN_001f9b48(&birdbot_moby->pos, D_L01_001600EC + (birdbot_vars->w1E4 << 7) + 0x30) <
                frame_time * 10.0f) {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            }
        } else {
            clear_vector(&va);
            va.f[2] = probe_ground_height(&birdbot_moby->pos, 0, 0.5f) - birdbot_moby->pos.f[2];
            if (va.f[2] < -(frame_time * 10.0f)) {
                va.f[2] = -(frame_time * 10.0f);
            }
            add_vector_xyz(&birdbot_moby->pos, &birdbot_moby->pos, &va);
            if (vector_length_xyz(&va) < frame_time) {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            }
        }
        FUN_L01_002e6518_c((SplashMoby *)birdbot_moby, (u128 *)&old);
        break;
    case BIRDBOT_STATE_FIRE_BURST:
        if (birdbot_vars->f1CC < 14.0f && frame >= 14.0f) {
            float a;
            float yaw;
            float pitch;

            a = birdbot_moby->rot.f[2];
            moby_point_position(birdbot_moby, 0, &va);
            qcopy(&birdbot_vars->v180, &va);
            if (fast_difference_between_rotations(FUN_001f9e90_cf(selected_combat_target.pos.f[0] - va.f[0], selected_combat_target.pos.f[1] - va.f[1]), a) <
                0.08726646f) {
                a = FUN_001f9e90_cf(selected_combat_target.pos.f[0] - va.f[0], selected_combat_target.pos.f[1] - va.f[1]);
            }
            birdbot_vars->v180.f[0] += fast_cos(a) * birdbot_vars->f198;
            birdbot_vars->v180.f[1] += fast_sin(a) * birdbot_vars->f198;
            subtract_vector_xyz(&vb, &birdbot_vars->v180, &va);
            normalize_vector_xyz(&vb, &vb, *(float *)&D_L01_00161950 * frame_time);
            vb.f[2] += 0.0f;
            *(u128 *)&vc = *(u128 *)&birdbot_moby->pos;
            vc.f[2] = va.f[2];
            if (collision_line(&vc, &va, 0, birdbot_moby, 0) == 0) {
                yaw = FUN_001f9e90_cf(vb.f[0], vb.f[1]);
                pitch = FUN_001f9e90_cf(vector_length_xy_cf(&vb), vb.f[2]);
                spawn_colored_half_scale_moby(birdbot_moby, &va, &vb, yaw, pitch, func_001FA898_r(FUN_L04_001f1978(10.0f)));
                birdbot_vars->h1A4++;
            } else {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
                break;
            }
        }
        if (frame < 2.0f) {
            if (birdbot_vars->h1A4 >= 4 || vector_distance(&birdbot_moby->pos, &selected_combat_target) > (flag ? birdbot_vars->f1A0 * 2.0f : birdbot_vars->f1A0) ||
                absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) > 6.0f) {
                if (birdbot_moby->b53 != 5) {
                    blend_moby_animation_cf(birdbot_moby, 5, 5, scale_game_frames(10));
                }
                birdbot_moby->f58 = 0.5f;
                birdbot_moby->state = 7;
            }
        }
        break;
    case BIRDBOT_STATE_RECOVER_FROM_BURST: {
        float a;

        a = FUN_001f9e90_cf(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0], selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1]);
        FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.02f, 0.3f, 0.1f);
        if (birdbot_moby->b53 == 0) {
            if (tick_countdown_32(&birdbot_vars->w190) != 0) {
                if (birdbot_moby->b53 != 5) {
                    blend_moby_animation_cf(birdbot_moby, 5, 0, scale_game_frames(0xF));
                }
            }
        } else if (birdbot_moby->b70 & 2) {
            birdbot_moby->f58 = 1.0f;
            if ((flag || (vector_distance(&birdbot_moby->pos, &selected_combat_target) < 8.0f &&
                          absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) < 6.0f)) &&
                path_reposition_requested != 0 && FUN_001f9b48(&birdbot_moby->pos, (PeVec *)birdbot_vars->main.path + (idx + 1)) > 0.25f) {
                birdbot_vars->main.idx = idx;
                if (birdbot_moby->b53 != 8) {
                    blend_moby_animation_cf(birdbot_moby, 8, 0, scale_game_frames(10));
                }
                birdbot_moby->f58 = *(float *)&D_L01_0016195C;
                birdbot_moby->state = 9;
            } else {
                float d;

                d = vector_distance(&birdbot_moby->pos, &selected_combat_target);
                if ((flag ? d < birdbot_vars->f1A0 * 2.0f : d < birdbot_vars->f1A0) &&
                    absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) < 2.0f && birdbot_moby->b31 != 0 &&
                    (!flag || path_reposition_requested == 0) && birdbot_vars->w190 == 0 &&
                    fast_difference_between_rotations(birdbot_moby->rot.f[2], FUN_001f9e90_cf(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                                                              selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1])) <
                        0.08726646f) {
                    moby_point_position(birdbot_moby, 0, &va);
                    *(u128 *)&vb = *(u128 *)&birdbot_moby->pos;
                    vb.f[2] = va.f[2];
                    if (collision_line(&vb, &va, 0, birdbot_moby, 0) == 0) {
                        birdbot_vars->h1A4 = 0;
                        birdbot_vars->f198 = 4.0f;
                        birdbot_moby->state = 6;
                        if (birdbot_moby->b53 != 4) {
                            blend_moby_animation_cf(birdbot_moby, 4, 0, scale_game_frames(6));
                        }
                        birdbot_vars->w190 = 0;
                        break;
                    }
                    birdbot_vars->w190 = scale_game_frames(0x3C);
                }
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            }
        }
        break;
    }
    case BIRDBOT_STATE_BEGIN_PATH_MOVEMENT:
        birdbot_moby->f58 = 1.0f;
        birdbot_vars->f1B8 = 0.0f;
        birdbot_vars->f1BC = 0.0f;
        birdbot_moby->state = 0xA;
        FUN_L01_002e6518_c((SplashMoby *)birdbot_moby, (u128 *)&old);
        break;
    case BIRDBOT_STATE_MOVE_TO_PATH_NODE: {
        PeCursor *c;
        float path_node_height_offset;

        c = &birdbot_vars->main;
        if (birdbot_vars->h1AE != 0) {
            int i;
            int best;
            float bd;

            bd = 1e11f;
            best = 0;
            for (i = 0; i < c->path->count; i++) {
                float d;

                d = FUN_001f9b48(&c->path->pts[i], &birdbot_moby->pos);
                if (d < bd) {
                    bd = d;
                    best = i;
                }
            }
            birdbot_vars->h1A6 = c->idx < best ? best - 1 : best < c->idx ? best + 1 : c->idx;
        } else {
            birdbot_vars->h1A6 = c->idx;
        }
        qcopy(&va, &c->path->pts[birdbot_vars->h1A6]);
        path_node_height_offset = 0.8f;
        va.f[2] += path_node_height_offset;
        FUN_L00_00258278_cf(
            (char *)birdbot_moby, &birdbot_vars->f194, FUN_001f9e90_cf(va.f[0] - birdbot_moby->pos.f[0], va.f[1] - birdbot_moby->pos.f[1]),
            frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 6.2831855f);
        if (fast_difference_between_rotations(birdbot_moby->rot.f[2], FUN_001f9e90_cf(va.f[0] - birdbot_moby->pos.f[0],
                                                      va.f[1] - birdbot_moby->pos.f[1])) < 0.7853982f) {
            if (birdbot_vars->f1B8 < *(float *)&D_L01_00161954 * frame_time) {
                birdbot_vars->f1B8 = birdbot_vars->f1B8 + *(float *)&D_L01_00161954 * frame_time_sq;
            }
            build_spherical_offset_cf(
                &vb, birdbot_vars->f1B8, birdbot_moby->rot.f[2],
                FUN_001f9e90_cf(vector_distance(&birdbot_moby->pos, &va), va.f[2] - birdbot_moby->pos.f[2]));
            add_vector_xyz(&birdbot_moby->pos, &birdbot_moby->pos, &vb);
            birdbot_moby->rot.f[1] = spring_turn(
                &birdbot_vars->f1BC, birdbot_moby->rot.f[1],
                birdbot_vars->f1B8 / (*(float *)&D_L01_00161954 * frame_time) * 0.5235988f,
                frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 4.712389f);
        }
        qcopy(&va, &c->path->pts[c->idx]);
        va.f[2] += path_node_height_offset;
        if (FUN_001f9b48(&birdbot_moby->pos, &va) < 0.25f) {
            birdbot_moby->state = 0xB;
        }
        FUN_L01_002e6518_c((SplashMoby *)birdbot_moby, (u128 *)&old);
        break;
    }
    case BIRDBOT_STATE_BRAKE_AND_FACE_TARGET:
        if (birdbot_vars->f1B8 > 0.0f && birdbot_vars->h1EE == 0) {
            birdbot_vars->f1B8 -= *(float *)&D_L01_00161954 * 3.0f * frame_time_sq;
            va.f[0] = fast_cos(birdbot_moby->rot.f[2]) * birdbot_vars->f1B8;
            va.f[1] = fast_sin(birdbot_moby->rot.f[2]) * birdbot_vars->f1B8;
            va.f[2] = 0.0f;
            add_vector_xyz(&birdbot_moby->pos, &birdbot_moby->pos, &va);
            birdbot_moby->rot.f[1] = spring_turn(
                &birdbot_vars->f1BC, birdbot_moby->rot.f[1],
                birdbot_vars->f1B8 / (*(float *)&D_L01_00161954 * frame_time) * 0.5235988f,
                frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f, frame_time * 6.2831855f);
        } else {
            float a;
            float k;

            a = FUN_001f9e90_cf(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0], selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1]);
            FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.02f, 0.3f, 0.1f);
            k = *(float *)&D_L01_00161954 * frame_time;
            birdbot_vars->f1B8 = 0.0f;
            birdbot_moby->rot.f[1] = spring_turn(&birdbot_vars->f1BC, birdbot_moby->rot.f[1], birdbot_vars->f1B8 / k * 0.5235988f,
                                           frame_time_sq * 6.2831855f, frame_time_sq * 3.1415927f,
                                           frame_time * 6.2831855f);
        }
        if (birdbot_moby->b53 != 0xA && birdbot_vars->f1B8 <= 0.0f &&
            fast_difference_between_rotations(birdbot_moby->rot.f[2], FUN_001f9e90_cf(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                                                      selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1])) < 0.034906585f) {
            birdbot_vars->f1B8 = 0.0f;
            if (birdbot_moby->b53 != 0xA) {
                blend_moby_animation_cf(birdbot_moby, 0xA, 0, scale_game_frames(3));
            }
        } else if (birdbot_moby->b52 == 0xA && (birdbot_moby->b70 & 2)) {
            if (birdbot_moby->b53 != 0) {
                blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(6));
            }
            birdbot_moby->state = 1;
        }
        FUN_L01_002e6518_c((SplashMoby *)birdbot_moby, (u128 *)&old);
        break;
    case BIRDBOT_STATE_NONFATAL_KNOCKBACK:
        if (mine_fall(birdbot_moby, birdbot_vars->x70) & 0x40) {
            if (FUN_001f9b48(&birdbot_moby->pos, (PeVec *)birdbot_vars->main.path + (idx + 1)) > 0.25f) {
                int i;
                float f;

                if (birdbot_moby->b53 != 8) {
                    blend_moby_animation_cf(birdbot_moby, 8, 0, scale_game_frames(10));
                }
                i = idx;
                f = *(float *)&D_L01_0016195C;
                birdbot_vars->main.idx = i;
                birdbot_moby->f58 = f;
                birdbot_moby->state = 9;
                return;
            }
            if (birdbot_moby->b53 != 0) {
                blend_moby_animation_cf(birdbot_moby, 0, 0, scale_game_frames(6));
            }
            birdbot_moby->state = 1;
        }
        if (birdbot_moby->pos.f[0] < 2.0f || birdbot_moby->pos.f[0] > 1021.0f || birdbot_moby->pos.f[1] < 2.0f ||
            birdbot_moby->pos.f[1] > 1021.0f || birdbot_moby->pos.f[2] < 2.0f || birdbot_moby->pos.f[2] > 1021.0f) {
            spawn_explosion(birdbot_moby, &birdbot_vars->v40, &birdbot_moby->pos, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 4,
                             15.0f, 1, 1, -1, 0);
            FUN_L00_00257470(birdbot_moby, 0, -1);
            mark_moby_for_removal(birdbot_moby);
            return;
        }
        break;
    case BIRDBOT_STATE_FATAL_KNOCKBACK:
        birdbot_moby->h34 &= 0xEFFF;
        if ((mine_fall(birdbot_moby, birdbot_vars->x70) & 6) || birdbot_moby->pos.f[0] < 2.0f || birdbot_moby->pos.f[0] > 1021.0f ||
            birdbot_moby->pos.f[1] < 2.0f || birdbot_moby->pos.f[1] > 1021.0f || birdbot_moby->pos.f[2] < 2.0f ||
            birdbot_moby->pos.f[2] > 1021.0f) {
            spawn_explosion(birdbot_moby, &birdbot_vars->v40, &birdbot_moby->pos, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 4,
                             15.0f, 1, 1, -1, 0);
            FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6C8, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                               D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6C9, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                               D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6CA, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                               D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6EA, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                               D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00257470(birdbot_moby, 0, -1);
            mark_moby_for_removal(birdbot_moby);
            return;
        }
        break;
    }
    trooper_anim_finish(birdbot_moby, birdbot_vars->x60);
    birdbot_vars->f1CC = frame;
    if (birdbot_moby->state == 1 || birdbot_moby->state == 5 || birdbot_moby->state == 6 || birdbot_moby->state == 7 || birdbot_moby->state == 0xB) {
        birdbot_vars->fE8 += frame_time_sq * 9.8f;
        birdbot_moby->pos.f[2] -= birdbot_vars->fE8 - 2.0f;
        ground = probe_ground_height(&birdbot_moby->pos, 0, 0.5f);
        birdbot_moby->pos.f[2] -= 2.0f;
        if (birdbot_moby->pos.f[2] < ground) {
            birdbot_moby->pos.f[2] = ground;
            birdbot_vars->fE8 = 0.0f;
        }
    } else {
        ground = probe_ground_height(&birdbot_moby->pos, 0, 0.5f);
    }
    set_range_centered_on_value((char *)birdbot_moby, ground);
    if (collision_hit_material() == 0 && birdbot_moby->pos.f[2] < ground + 1.0f) {
        spawn_explosion(birdbot_moby, &birdbot_vars->v40, &birdbot_moby->pos, 0.0f, 0.0f, 5, 2, 4, 2.0f, 1.0f, 9.0f, 1.0f, 4, 15.0f,
                         1, 1, -1, 0);
        FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6C8, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                           D_L01_0015F580, D_L01_0015F580);
        FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6C9, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                           D_L01_0015F580, D_L01_0015F580);
        FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6CA, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                           D_L01_0015F580, D_L01_0015F580);
        FUN_L00_00263fd8_u((char *)birdbot_moby, 0x6EA, birdbot_moby->pos.f, &birdbot_moby->rot, 0, 0, 0.0f, D_L01_0015F580,
                           D_L01_0015F580, D_L01_0015F580);
        FUN_L00_00257470(birdbot_moby, 0, -1);
        mark_moby_for_removal(birdbot_moby);
    }
}
#include "sda.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union {
    u128 q;
    f32 f[4];
} Vec577;

typedef struct {
    u8 pad0[0xA6];
    u16 oclass;
} Mob577Class;

typedef struct {
    u8 pad0[0x10];
    u128 pos;
    Mob577Class *cls;
    u8 pad24[0x8];
    f32 dmg;
} Mob577Hit;

typedef struct {
    u8 pad0[0x10];
    s32 id;
} Mob577Ref;

typedef struct Mob577 {
    u8 pad0[0x10];
    Vec577 pos;
    u8 state;
    u8 group;
    u8 pad22[0x2];
    Mob577Ref *ref;
    struct Mob577 *next;
    u8 pad2c[0x8];
    u16 flags;
    u8 pad36[0xA];
    Vec577 rot;
    u8 pad50[0x2];
    u8 anim_next;
    u8 anim;
    u8 pad54[0x1C];
    u8 anim_flags;
    u8 pad71[0x7];
    void *vars;
    u8 pad7c[0x18];
    s32 id;
    u8 pad98[0xC];
    u8 xA4;
    u8 padA5;
    s16 oclass;
    u8 padA8[0x14];
    u8 hit;
} Mob577;

typedef struct MobyNode {
    u8 pad0[0x10];
    Vec577 pos;
    s8 state;
    u8 pad21[0x7];
    struct MobyNode *next;
    u8 pad2c[0x7A];
    s16 oclass;
} MobyNode;

typedef struct {
    u8 pad0[0x20];
    f32 health;
    s16 x24;
    s16 flee_timer;
    u8 pad28;
    u8 x29;
    u8 pad2a[0xE];
    s32 x38;
    u8 pad3c[0x4];
    Vec577 vel;
    u8 pad50[0x8];
    u8 x58;
    u8 pad59;
    u8 x5A;
    u8 pad5b[0x6D];
    s16 xC8;
    u8 padca[0x6];
    void *xD0;
    u8 padd4[0x3C];
    u8 fx110[0x7];
    u8 x117;
    u8 pad118[0x8];
    u8 knock[0x10];
    f32 x130;
    f32 x134;
    f32 x138;
    f32 x13C;
    u8 pad140[0x4];
    s32 x144;
    u8 pad148[0x15];
    u8 x15D;
    u8 pad15e[0x12];
    f32 x170;
    f32 x174;
    u8 pad178[0x8];
    s32 x180;
    u8 pad184[0x4];
    f32 x188;
    f32 x18C;
    u8 pad190[0x8];
    f32 x198;
    u8 pad19c[0x8];
    f32 x1A4;
    u8 pad1a8[0x28];
    Vec577 home;
    Vec577 dest;
    u8 turn[0x4];
    f32 turn_off;
    f32 turn_deg;
    f32 range;
    u8 pad200[0x4];
    f32 speed;
    s16 x208;
    s16 x20A;
    s32 idle_timer;
    s32 turn_timer;
    s16 x214;
    s16 x216;
    Mob577 *leader;
    s32 path;
    u8 pad220[0x8];
    s32 x228;
    u8 pad22c[0x4];
    u8 x230[0x10];
} Mob577Vars;

typedef struct {
    Vec577 pos;
    u8 pad10[0x30];
    void *p40;
    u8 pad44[0xC];
} Mob577Target;

extern s32 cutscene_mode __asm__("D_L01_0015F5C4");
#include "rnc/overlay/l01/gameplay/entities/birdbot_data/peckbot_update.h"
s32 D_L01_00161A70 = 0x9020202;
extern s32 D_L01_001B0930[];
extern MobyNode *D_L01_0015FFE4;
extern u8 D_L01_00161A80[] __attribute__((section(".sdata")));

extern s32 FUN_001160d8();
extern f32 FUN_L04_001f1978(f32);
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern int tick_countdown_32(void *) __asm__("FUN_001f9740");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
f32 FUN_001f99c0(f32);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("FUN_001fa5c8");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32 FUN_0020c9e0(void *);
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern f32 FUN_00213508(void *, s32, f32);
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32 coll_sphere(void *, s32, void *, f32) __asm__("FUN_L00_001f0d60");
extern s32 FUN_L00_001f35d8_cf(void) __asm__("FUN_001f0b58");
extern void FUN_L00_001ff500_cf(void *, void *, float) __asm__("FUN_001f9bf8");
extern f32 FUN_L00_001ff8b0_cf(f32, f32) __asm__("FUN_001f9e90");
extern void FUN_L00_00257470(void *, s32, s32);
extern void FUN_L00_00258278(void *, void *, f32, f32, f32, f32);
extern s32 FUN_L00_00258b50(void *, void *, void *, void *, f32);
extern void spawn_nearby_effect(void *, void *, s32, void *, void *, f32) __asm__("FUN_L00_00259bc8");
extern void FUN_L00_0025a120(void *);
extern void *find_moby_hit(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern void hit_reaction(void *, void *, void *, s32, void *, void *, s32, s32) __asm__("FUN_L00_0025a478");
extern void trooper_knockback_arc(void *, void *, void *, void *) __asm__("FUN_L00_0025ab48");
extern f32 FUN_L00_0025abf0(void *, void *, void *, f32, f32);
extern void trooper_start_knockback(void *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern s32 mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void cart_burst(void *, void *, s32, f32, f32) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025f730(void *, f32);
extern void select_moby_combat_target(void *, void *, f32) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00263ac8(void *, s32, void *, f32);
extern void FUN_L00_00263fd8(void *, s32, void *, void *, s32, s32, f32, void *, void *, void *);
extern s32 FUN_L00_002dc9f0(void *, void *);
extern void FUN_L01_0026d930(void *);
extern void FUN_L01_0026e090(s32, s32);
extern void select_list_point_toward_target(void *, s32, void *, void *) __asm__("FUN_L01_0028b5e0");
extern s32 reserve_expired_frame_slot(void *, s32, s32) __asm__("FUN_L01_002efbf8");

#define SET_ANIM(m, a, b, c)                                                                       \
    if ((m)->anim != (a))                                                                          \
    blend_moby_animation((m), (a), (b), (c))

void peckbot_update(Mob577 *self) __asm__("FUN_L01_002efc60");
void peckbot_update(Mob577 *self) {
    enum {
        PECKBOT_STATE_INITIALIZE = 0,
        PECKBOT_STATE_GROUNDED_IDLE = 1,
        PECKBOT_STATE_FACE_AWAY_FROM_TARGET = 2,
        PECKBOT_STATE_CHASE_TARGET = 3,
        PECKBOT_STATE_ATTACK_TARGET = 4,
        PECKBOT_STATE_RETURN_HOME = 5,
        PECKBOT_STATE_NONFATAL_KNOCKBACK = 6,
        PECKBOT_STATE_WAIT_FOR_CLOSE_TARGET = 8,
        PECKBOT_STATE_FACE_HOME_AND_WAIT = 10,
        PECKBOT_STATE_HOME_TRIGGERED_KNOCKBACK = 11,
        PECKBOT_STATE_RECOVER_FROM_BLOCKED_MOVEMENT = 12,
        PECKBOT_STATE_MOVE_FORWARD_NEAR_LEADER = 13,
        PECKBOT_STATE_AIRBORNE_IDLE = 14,
        PECKBOT_STATE_MOVE_TO_DESTINATION = 15,
        PECKBOT_STATE_ASCEND = 16,
        PECKBOT_STATE_DESCEND = 17,
        PECKBOT_STATE_FATAL_KNOCKBACK = 0x63
    };
    Vec577 oldpos;
    Mob577Target target;
    Vec577 vector_scratch;
    Vec577 position_scratch;
    Vec577 home_to_target;
    s32 hitA0;
    f32 hitA4;
    f32 angle;
    f32 knockback_travel_time;
    Mob577Vars *peckbot_vars;
    Mob577Hit *hit;
    MobyNode *candidate_moby;
    void *target_moby;
    Vec577 *mp;
    Vec577 *np;
    s32 movement_result_flags;
    s32 recovery_movement_result_flags;
    s32 frame_count_or_animation_id;
    f32 hp;
    f32 rad;
    f32 idle_destination_azimuth;
    f32 descent_ground_height;
    f32 ascent_ground_height;
    f32 chase_target_height_distance_ratio;
    f32 attack_ground_height;
    f32 t99;
    f32 recovery_target_height_distance_ratio;
    f32 nearby_moby_heading_difference;
    f32 turn;
    f32 z;
    f32 forward_movement_ground_height;
    f32 desired_heading;

    peckbot_vars = self->vars;
    FUN_L00_00263ac8(self, 0, peckbot_vars->x230, 1.5f);
    if (cutscene_mode == 2) {
        self->id = 0;
        self->flags |= 0x41;
        return;
    }
    self->id = self->ref->id;
    self->flags &= ~0x41;
    select_moby_combat_target(self, &target, peckbot_vars->range + 4.0f);
    target_moby = target.p40;
    qcopy(&oldpos, &self->pos);
    if (peckbot_vars->x38 != 0) {
        peckbot_vars->x214 = scale_game_frames(0xF0);
    }
    peckbot_vars->x38 = 0;
    fast_dec_timer_s16(&peckbot_vars->x208);
    if (fast_dec_timer_s16(&peckbot_vars->x214) != 0) {
        peckbot_vars->range = 12.0f;
    } else {
        peckbot_vars->range = 24.0f;
    }
    hitA4 = 0.0f;
    hit = find_moby_hit(self, 0x330000, 0);
    hit_reaction(self, hit, &peckbot_vars->health, 0, &hitA0, &hitA4, 0, 4);
    fast_dec_timer_s16(&peckbot_vars->flee_timer);
    if (peckbot_vars->flee_timer < scale_game_frames(0x2D) && hit != NULL && self->state != 0x63) {
        if ((hit->cls->oclass == 0xB0 || hit->cls->oclass == 0xB1) && peckbot_vars->flee_timer != 0) {
            hit->dmg = 0.0f;
        }
        if (self->group != 0xFF) {
            FUN_L01_0026e090(self->group, 1);
        }
        if (hit->dmg != 0.0f) {
            angle = FUN_L00_001ff8b0_cf(self->pos.f[0] - target.pos.f[0],
                                        self->pos.f[1] - target.pos.f[1]);
            hp = peckbot_vars->health - hit->dmg;
            peckbot_vars->x130 = 0.008f;
            peckbot_vars->x134 = 0.0005f;
            peckbot_vars->x138 = peckbot_nonfatal_knockback_horizontal_speed_gp * frame_time;
            peckbot_vars->x13C = peckbot_nonfatal_knockback_vertical_speed_gp * frame_time;
            peckbot_vars->x144 = 9;
            peckbot_vars->health = hp;
            peckbot_vars->x15D = 0;
            if (hp <= 0.0f) {
                self->flags &= ~0x1000;
                peckbot_vars->x13C = peckbot_vars->x138 = frame_time * 11.0f;
                vector_scratch.q = hit->pos;
                trooper_knockback_arc(&vector_scratch, &angle, &peckbot_vars->x138, &peckbot_vars->x13C);
                trooper_start_knockback(self, peckbot_vars->knock, 7, 1, 0, angle);
                peckbot_vars->x170 = 11.0f;
                peckbot_vars->x174 = 18.0f;
                self->state = 0x63;
                peckbot_vars->x117 = 0x78;
                trooper_anim_update(self, peckbot_vars->fx110);
                FUN_L00_00257470(self, 0, -1);
            } else {
                vector_scratch.q = hit->pos;
                trooper_knockback_arc(&vector_scratch, &angle, &peckbot_vars->x138, &peckbot_vars->x13C);
                trooper_start_knockback(self, peckbot_vars->knock, 6, 1, 0, angle);
                peckbot_vars->x170 = 5.0f;
                peckbot_vars->x174 = 10.0f;
                self->state = 6;
                peckbot_vars->x117 = 0xFA;
                peckbot_vars->flee_timer = scale_game_frames(0x3C);
                trooper_anim_update(self, peckbot_vars->fx110);
            }
        }
    }
    self->xA4 = 0xFF;
    switch (self->state) {
    case PECKBOT_STATE_INITIALIZE:
        qcopy(&peckbot_vars->home, &self->pos);
        peckbot_vars->x29 = 0;
        if (peckbot_vars->x20A != 0) {
            self->state = 1;
            SET_ANIM(self, 4, 0, 0);
        } else {
            self->state = 14;
            self->pos.f[2] = self->pos.f[2] + 6.0f;
            SET_ANIM(self, 0, 0, 0);
        }
        peckbot_vars->x214 = 0;
        peckbot_vars->x208 = 0;
        peckbot_vars->health = 1.0f;
        peckbot_vars->x24 = 1;
        peckbot_vars->x58 = 8;
        peckbot_vars->x5A = 4;
        FUN_L01_0026d930(&peckbot_vars->x180);
        peckbot_vars->x180 = 0x38D;
        peckbot_vars->x18C = 2.0f;
        peckbot_vars->x188 = 2.0f;
        peckbot_vars->x1A4 = peckbot_vars->speed * frame_time;
        if (FUN_001160d8() & 1) {
            self->flags |= 0x8000;
        }
        peckbot_vars->xD0 = &D_L01_00161A70;
        break;
    case PECKBOT_STATE_AIRBORNE_IDLE:
        fast_vec_sub(&vector_scratch, &target.pos, &self->pos);
        if (tick_countdown_32(&peckbot_vars->x228) != 0 && fast_vec_length(&vector_scratch) < peckbot_vars->range + 4.0f &&
            FUN_001f99c0(self->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            self->state = 17;
        } else if (peckbot_vars->x228 == 0 && self->hit == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            self->state = 17;
            clear_u64_value(&peckbot_vars->vel);
            self->hit = 0;
        } else if (tick_countdown_32(&peckbot_vars->idle_timer) != 0) {
            peckbot_vars->idle_timer = scale_game_frames(0x78);
            rad = random_float_between(0.0f, 3.0f);
            idle_destination_azimuth = random_angle_radians();
            position_scratch.f[0] = fast_cos(idle_destination_azimuth) * rad;
            position_scratch.f[1] = fast_sin(idle_destination_azimuth) * rad;
            position_scratch.f[2] = 0.0f;
            position_scratch.f[2] = random_float_between(5.0f, 7.0f);
            fast_vec_add(&peckbot_vars->dest, &position_scratch, &peckbot_vars->home);
            self->state = 15;
        }
        break;
    case PECKBOT_STATE_MOVE_TO_DESTINATION:
        desired_heading = FUN_L00_001ff8b0_cf(peckbot_vars->dest.f[0] - self->pos.f[0], peckbot_vars->dest.f[1] - self->pos.f[1]);
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        fast_vec_sub(&peckbot_vars->vel, &peckbot_vars->dest, &self->pos);
        FUN_L00_0025f730(&peckbot_vars->vel, frame_time * 4.0f);
        fast_vec_add(&vector_scratch, &peckbot_vars->vel, &self->pos);
        if (coll_sphere(&vector_scratch, 0, self, 0.4f) != 0) {
            clear_u64_value(&peckbot_vars->vel);
            self->state = 14;
        } else {
            qcopy(&self->pos, &vector_scratch);
        }
        if (fast_vec_length(&peckbot_vars->vel) == 0.0f) {
            self->state = 14;
        }
        break;
    case PECKBOT_STATE_DESCEND:
        descent_ground_height = FUN_00213508(&self->pos, 0, 0.5f);
        approach_value(&peckbot_vars->vel.f[2], -(frame_time * 8.0f), frame_time_sq * 10.8f);
        self->pos.f[2] = self->pos.f[2] + peckbot_vars->vel.f[2];
        if (self->anim != 1 && self->pos.f[2] - descent_ground_height <= frame_time * 8.0f * FUN_L04_001f1978(45.0f)) {
            SET_ANIM(self, 1, 0, scale_game_frames(5));
        }
        if (self->pos.f[2] <= descent_ground_height) {
            self->pos.f[2] = descent_ground_height;
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            self->state = 3;
            clear_u64_value(&peckbot_vars->vel);
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_ASCEND:
        ascent_ground_height = FUN_00213508(&self->pos, 0, 0.5f);
        approach_value(&peckbot_vars->vel.f[2], frame_time * 8.0f, frame_time_sq * 10.8f);
        self->pos.f[2] += peckbot_vars->vel.f[2];
        if (ascent_ground_height + 6.0f <= self->pos.f[2]) {
            self->pos.f[2] = ascent_ground_height + 6.0f;
            self->state = 14;
            peckbot_vars->x228 = scale_game_frames(0xB4);
            clear_u64_value(&peckbot_vars->vel);
        }
        break;
    case PECKBOT_STATE_GROUNDED_IDLE:
        fast_vec_sub(&vector_scratch, &target.pos, &self->pos);
        if (fast_vec_length(&vector_scratch) < peckbot_vars->range + 4.0f &&
            FUN_001f99c0(self->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            self->state = 2;
            SET_ANIM(self, 5, 0, scale_game_frames(3));
        } else if (self->hit == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
            self->hit = 0;
        } else if (fast_vec_length(&vector_scratch) > peckbot_vars->range + 6.0f && peckbot_vars->x20A == 0) {
            self->state = 16;
            SET_ANIM(self, 0, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_WAIT_FOR_CLOSE_TARGET:
        fast_vec_sub(&vector_scratch, &target.pos, &self->pos);
        if (fast_vec_length(&vector_scratch) < peckbot_vars->range - 4.0f &&
            FUN_001f99c0(self->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            self->state = 2;
            self->pos.f[2] = self->pos.f[2] + 1.0f;
            SET_ANIM(self, 5, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_FACE_HOME_AND_WAIT:
        desired_heading = FUN_L00_001ff8b0_cf(peckbot_vars->home.f[0] - self->pos.f[0], peckbot_vars->home.f[1] - self->pos.f[1]);
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        fast_vec_sub(&vector_scratch, &target.pos, &peckbot_vars->home);
        if ((fast_vec_length(&vector_scratch) < peckbot_vars->range &&
             FUN_001f99c0(peckbot_vars->home.f[2] - target.pos.f[2]) < 8.0f) ||
            self->hit == 2) {
            if (self->group != 0xFF) {
                FUN_L01_0026e090(self->group, 2);
            }
            SET_ANIM(self, 6, 0, scale_game_frames(3));
            peckbot_vars->x134 = 0.0f;
            peckbot_vars->x130 = peckbot_home_knockback_acceleration_gp;
            peckbot_vars->x138 = peckbot_home_knockback_speed_gp * frame_time;
            peckbot_vars->x13C = FUN_L00_0025abf0(&self->pos, &peckbot_vars->home, &knockback_travel_time, peckbot_vars->x138, -peckbot_home_knockback_acceleration_gp);
            peckbot_vars->x144 = 13;
            peckbot_vars->x15D = 0;
            trooper_start_knockback(self, peckbot_vars->knock, 6, 1, 0, self->rot.f[2]);
            peckbot_vars->x170 = 5.0f;
            peckbot_vars->x174 = 10.0f;
            self->state = 11;
        }
        break;
    case PECKBOT_STATE_HOME_TRIGGERED_KNOCKBACK:
        if (mine_fall(self, peckbot_vars->knock) & 1) {
            self->state = 3;
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            SET_ANIM(self, 2, 8, scale_game_frames(3));
            return;
        } else if (self->pos.f[2] < 5.0f) {
            FUN_L00_00257470(self, 0, -1);
            mark_moby_for_removal(self);
            return;
        }
        break;
    case PECKBOT_STATE_FACE_AWAY_FROM_TARGET:
        desired_heading =
            FUN_L00_001ff8b0_cf(self->pos.f[0] - target.pos.f[0], self->pos.f[1] - target.pos.f[1]);
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.02f, 0.3f, 0.1f);
        fast_vec_sub(&vector_scratch, &target.pos, &self->pos);
        if (fast_vec_length(&vector_scratch) < peckbot_vars->range &&
            FUN_001f99c0(self->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            if (self->group != 0xFF) {
                FUN_L01_0026e090(self->group, 1);
            }
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
            self->hit = 0;
        } else if (self->hit == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
            self->hit = 0;
        } else if (fast_vec_length(&vector_scratch) > peckbot_vars->range + 6.0f && peckbot_vars->x20A == 0) {
            self->state = 16;
            SET_ANIM(self, 0, 0, scale_game_frames(3));
        }
        if (self->anim == 5 && (self->anim_flags & 2)) {
            blend_moby_animation(self, 4, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_CHASE_TARGET:
        chase_target_height_distance_ratio = FUN_001f99c0(self->pos.f[2] - target.pos.f[2]);
        chase_target_height_distance_ratio = chase_target_height_distance_ratio / vector_distance(&self->pos, &target.pos);
        if (peckbot_vars->path != -1) {
            select_list_point_toward_target(self, D_L01_001B0930[peckbot_vars->path], &target.pos, &position_scratch);
        } else {
            qcopy(&position_scratch, &target.pos);
        }
        desired_heading =
            FUN_L00_001ff8b0_cf(position_scratch.f[0] - self->pos.f[0], position_scratch.f[1] - self->pos.f[1]) + peckbot_vars->turn_off;
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        vector_scratch.f[0] = fast_cos(self->rot.f[2]) * 2.0f;
        vector_scratch.f[1] = fast_sin(self->rot.f[2]) * 2.0f;
        vector_scratch.f[2] = 0.0f;
        movement_result_flags = FUN_L00_00258b50(self, &peckbot_vars->x180, &vector_scratch, &position_scratch, 1.0f);
        if (tick_countdown_32(&peckbot_vars->turn_timer) != 0) {
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_vars->turn_off = -peckbot_vars->turn_off;
        }
        fast_vec_sub(&home_to_target, &target.pos, &peckbot_vars->home);
        if (vector_distance(&self->pos, &target.pos) < 1.5f && !(movement_result_flags & 2)) {
            self->state = 4;
            SET_ANIM(self, 3, 0, scale_game_frames(3));
        } else if (chase_target_height_distance_ratio > 1.0f || (movement_result_flags & 2)) {
            self->pos.f[0] = oldpos.f[0];
            self->pos.f[1] = oldpos.f[1];
            self->state = 12;
            SET_ANIM(self, 5, 0, scale_game_frames(3));
        } else if (fast_vec_length(&home_to_target) < peckbot_vars->range + 4.0f &&
                   FUN_001f99c0(self->pos.f[2] - target.pos.f[2]) < 8.0f) {
            if (self->group != 0xFF) {
                FUN_L01_0026e090(self->group, 1);
            }
        } else if (self->hit != 1) {
            self->state = 5;
        }
        FUN_00213508(&self->pos, 0, 0.5f);
        if (FUN_L00_001f35d8_cf() == 0 || FUN_L00_001f35d8_cf() == 8) {
            self->state = 16;
            SET_ANIM(self, 0, 0, scale_game_frames(3));
        }
        self->hit = 0;
        break;
    case PECKBOT_STATE_ATTACK_TARGET:
        desired_heading =
            FUN_L00_001ff8b0_cf(target.pos.f[0] - self->pos.f[0], target.pos.f[1] - self->pos.f[1]);
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        if (self->anim_next == self->anim && FUN_0020c9e0(self) == 13.0f &&
            FUN_001f99c0(self->pos.f[2] - target.pos.f[2]) < 0.5f &&
            vector_distance(&self->pos, &target.pos) < 2.0f &&
            fast_difference_between_rotations(FUN_L00_001ff8b0_cf(target.pos.f[0] - self->pos.f[0],
                                             target.pos.f[1] - self->pos.f[1]),
                         self->rot.f[2]) < 0.2617994f) {
            vector_scratch.f[0] = fast_cos(self->rot.f[2]) * 0.2f;
            vector_scratch.f[1] = fast_sin(self->rot.f[2]) * 0.2f;
            vector_scratch.f[2] = 0.0f;
            qcopy(&position_scratch, &target.pos);
            position_scratch.f[2] = position_scratch.f[2] + 0.75f;
            if (target_moby != NULL) {
                spawn_nearby_effect(target_moby, self, 1, &position_scratch, &vector_scratch, 1.0f);
            }
        }
        if ((self->anim_flags & 2) && vector_distance(&self->pos, &target.pos) > 1.5f) {
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        }
        attack_ground_height = FUN_00213508(&self->pos, 0, 0.5f);
        peckbot_vars->x198 = peckbot_vars->x198 - frame_time_sq * 9.8f;
        self->pos.f[2] = self->pos.f[2] + peckbot_vars->x198;
        if (self->pos.f[2] < attack_ground_height) {
            peckbot_vars->x198 = 0.0f;
            self->pos.f[2] = attack_ground_height;
        }
        break;
    case PECKBOT_STATE_RETURN_HOME:
        if (peckbot_vars->path != -1) {
            select_list_point_toward_target(self, D_L01_001B0930[peckbot_vars->path], &peckbot_vars->home, &position_scratch);
        } else {
            qcopy(&position_scratch, &peckbot_vars->home);
        }
        desired_heading = FUN_L00_001ff8b0_cf(position_scratch.f[0] - self->pos.f[0], position_scratch.f[1] - self->pos.f[1]);
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.02f, 0.3f, 0.1f);
        vector_scratch.f[0] = fast_cos(self->rot.f[2]) * 2.0f;
        vector_scratch.f[1] = fast_sin(self->rot.f[2]) * 2.0f;
        vector_scratch.f[2] = 0.0f;
        movement_result_flags = FUN_L00_00258b50(self, &peckbot_vars->x180, &vector_scratch, &position_scratch, 1.0f);
        if (vector_distance(&self->pos, &peckbot_vars->home) < 2.0f) {
            self->state = 1;
            SET_ANIM(self, 4, 0, scale_game_frames(3));
        }
        fast_vec_sub(&home_to_target, &target.pos, &peckbot_vars->home);
        if (fast_vec_length(&home_to_target) < peckbot_vars->range) {
            if (self->group != 0xFF) {
                FUN_L01_0026e090(self->group, 1);
            }
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        } else if (self->hit == 1) {
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        } else if (movement_result_flags & 2) {
            self->pos.f[0] = oldpos.f[0];
            self->pos.f[1] = oldpos.f[1];
            self->state = 12;
            SET_ANIM(self, 5, 0, scale_game_frames(3));
        }
        FUN_00213508(&self->pos, 0, 0.5f);
        if (FUN_L00_001f35d8_cf() == 0 || FUN_L00_001f35d8_cf() == 8) {
            self->state = 16;
            SET_ANIM(self, 0, 0, scale_game_frames(3));
        }
        self->hit = 0;
        break;
    case PECKBOT_STATE_NONFATAL_KNOCKBACK:
        if (mine_fall(self, peckbot_vars->knock) & 1) {
            self->state = 3;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
            return;
        } else if (self->pos.f[2] < 5.0f) {
            FUN_L00_00257470(self, 0, -1);
            mark_moby_for_removal(self);
            return;
        }
        break;
    case 7:
        if (FUN_L00_002dc9f0(self, peckbot_vars->knock) != 0) {
            self->state = 14;
            peckbot_vars->x228 = scale_game_frames(0xB4);
            SET_ANIM(self, 0, 0, 0);
            peckbot_vars->xC8 = 0;
        }
        break;
    case PECKBOT_STATE_FATAL_KNOCKBACK:
        self->flags &= ~0x1000;
        if (mine_fall(self, peckbot_vars->knock) & 0x140) {
            clear_u64_value(&vector_scratch);
            frame_count_or_animation_id = scale_game_frames(1) * 60.0f;
            if (reserve_expired_frame_slot(D_L01_00161A80, 3, frame_count_or_animation_id) >= 0) {
                cart_burst(self, &self->pos, 4, 0.5f, 13.0f);
            }
            FUN_L00_00263fd8(self, 0x6D3, &self->pos, &self->rot, 0, 0, 0.0f, D_L01_0015F580,
                             D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8(self, 0x6D4, &self->pos, &self->rot, 0, 0, 0.0f, D_L01_0015F580,
                             D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8(self, 0x6D5, &self->pos, &self->rot, 0, 0, 0.0f, D_L01_0015F580,
                             D_L01_0015F580, D_L01_0015F580);
            mark_moby_for_removal(self);
            return;
        } else if (self->pos.f[2] < 0.0f) {
            FUN_L00_00263fd8(self, 0x6D3, &self->pos, &self->rot, 0, 0, 0.0f, D_L01_0015F580,
                             D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8(self, 0x6D4, &self->pos, &self->rot, 0, 0, 0.0f, D_L01_0015F580,
                             D_L01_0015F580, D_L01_0015F580);
            FUN_L00_00263fd8(self, 0x6D5, &self->pos, &self->rot, 0, 0, 0.0f, D_L01_0015F580,
                             D_L01_0015F580, D_L01_0015F580);
            mark_moby_for_removal(self);
            return;
        }
        break;
    case PECKBOT_STATE_RECOVER_FROM_BLOCKED_MOVEMENT:
        recovery_target_height_distance_ratio = FUN_001f99c0(self->pos.f[2] - target.pos.f[2]);
        recovery_target_height_distance_ratio = recovery_target_height_distance_ratio / vector_distance(&self->pos, &target.pos);
        if (peckbot_vars->path != -1) {
            select_list_point_toward_target(self, D_L01_001B0930[peckbot_vars->path], &peckbot_vars->home, &position_scratch);
        } else {
            qcopy(&position_scratch, &peckbot_vars->home);
        }
        desired_heading = FUN_L00_001ff8b0_cf(position_scratch.f[0] - self->pos.f[0], position_scratch.f[1] - self->pos.f[1]);
        FUN_L00_00258278(self, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        vector_scratch.f[0] = fast_cos(self->rot.f[2]) * 2.0f;
        vector_scratch.f[1] = fast_sin(self->rot.f[2]) * 2.0f;
        vector_scratch.f[2] = 0.0f;
        recovery_movement_result_flags = FUN_L00_00258b50(self, &peckbot_vars->x180, &vector_scratch, &position_scratch, 1.0f);
        fast_vec_sub(&home_to_target, &target.pos, &peckbot_vars->home);
        if (vector_distance(&self->pos, &target.pos) < 1.5f && recovery_target_height_distance_ratio <= 1.0f) {
            self->state = 4;
            SET_ANIM(self, 3, 0, scale_game_frames(3));
        } else if (recovery_target_height_distance_ratio <= 1.0f && !(recovery_movement_result_flags & 2)) {
            self->state = 5;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        } else if (vector_distance(&self->pos, &target.pos) > peckbot_vars->range + 6.0f) {
            self->state = 16;
            SET_ANIM(self, 0, 0, scale_game_frames(3));
        } else if (self->anim_flags & 2) {
            random_integer_below(4);
            frame_count_or_animation_id = random_integer_below(4) ? 4 : 5;
            blend_moby_animation(self, frame_count_or_animation_id, 0, scale_game_frames(3));
        }
        FUN_00213508(&self->pos, 0, 0.5f);
        if (FUN_L00_001f35d8_cf() == 0 || FUN_L00_001f35d8_cf() == 8) {
            self->state = 16;
            SET_ANIM(self, 0, 0, scale_game_frames(3));
        }
        self->hit = 0;
        break;
    case PECKBOT_STATE_MOVE_FORWARD_NEAR_LEADER:
        if (self->anim_next != 2) {
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        }
        vector_scratch.f[0] = fast_cos(self->rot.f[2]) * (peckbot_vars->speed * frame_time);
        vector_scratch.f[1] = fast_sin(self->rot.f[2]) * (peckbot_vars->speed * frame_time);
        vector_scratch.f[2] = 0.0f;
        fast_vec_add(&position_scratch, &self->pos, &vector_scratch);
        for (candidate_moby = D_L01_0015FFE4; candidate_moby != NULL; candidate_moby = candidate_moby->next) {
            if (candidate_moby == (MobyNode *)self || candidate_moby->state < 0) {
                continue;
            }
            if (candidate_moby->oclass == 0 || candidate_moby->oclass == 0x241) {
                if (vector_distance(&position_scratch, &candidate_moby->pos) < 2.0f) {
                    z = self->pos.f[2];
                    nearby_moby_heading_difference = fast_subtract_rotations(
                        self->rot.f[2], FUN_L00_001ff8b0_cf(candidate_moby->pos.f[0] - self->pos.f[0],
                                                            candidate_moby->pos.f[1] - self->pos.f[1]));
                    turn = frame_time * 1.5707964f;
                    if (!(nearby_moby_heading_difference > 0.0f)) {
                        turn = -turn;
                    }
                    self->rot.f[2] = fast_add_rotations(self->rot.f[2], turn);
                    fast_vec_sub(&position_scratch, &self->pos, &candidate_moby->pos);
                    FUN_L00_001ff500_cf(&position_scratch, &position_scratch, 2.0f);
                    fast_vec_add(&position_scratch, &position_scratch, &candidate_moby->pos);
                    position_scratch.f[2] = z;
                }
            }
        }
        qcopy(&self->pos, &position_scratch);
        forward_movement_ground_height = FUN_00213508(&self->pos, 0, 0.5f);
        peckbot_vars->x198 = peckbot_vars->x198 - frame_time_sq * 9.8f;
        self->pos.f[2] = self->pos.f[2] + peckbot_vars->x198;
        if (self->pos.f[2] < forward_movement_ground_height) {
            peckbot_vars->x198 = 0.0f;
            self->pos.f[2] = forward_movement_ground_height;
        }
        if (peckbot_vars->leader == NULL ||
            vector_distance(&self->pos, &peckbot_vars->leader->pos) > convert_integer_to_float(peckbot_vars->x216)) {
            self->state = 5;
            SET_ANIM(self, 2, 0, scale_game_frames(3));
        }
        self->hit = 0;
        break;
    }
    trooper_anim_finish(self, peckbot_vars->fx110);
    FUN_L00_0025a120(self);
}
typedef struct Moby {
    Vec4f bsphere;                 /* 0x00 */
    Vec4f pos;                     /* 0x10 */
    u8 state;
    u8 pad21[0xF];
    u8 alpha;
    u8 pad31[0xF];
    f32 rot;
    u8 pad44[0x34];
    void *vars;
    u8 pad7c[0x2A];
    s16 oclass;
    u8 padA8[0x58];
} Moby;

/*
 * Path that carries the hero along (uses the unit's Moby, PePath and
 * D_L01_001B0930_u declared above). While the hero's control mode is 0x11 or
 * 0x12 and he is within `radius` of the path, he is pushed along
 * the path (and toward it when `pull` is set), capped at max_speed; the
 * nearest such path this frame wins through hero.current_path_dist. When he left it
 * (prev control mode 0x12, now 4 or 5) the push is damped instead.
 */
typedef struct {
    Vec4 vel;                      /* 0x00: path velocity, copied to the hero push (motion.unkF0) */
    u8 pad_10[0x10];
    s32 path;                      /* 0x20: index into D_L01_001B0930, -1 none */
    f32 max_speed;                 /* 0x24 */
    f32 radius;                    /* 0x28: reach from the path */
    s16 pull;                      /* 0x2C: nonzero pulls the hero onto the path */
    s16 ready;                     /* 0x2E: segment lengths computed */
} PathCarrierVars;

extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
/* nearest point of the path to pos, with its segment and position on it */
extern void FUN_L00_0025df68(PePath *, void *, void *, f32 *, s32 *, s32, f32, f32, f32);
/* the point a step further along the path; nonzero at the path's end */
extern s32 sample_path(PePath *, void *, f32 *, s32 *, s32, f32) __asm__("FUN_L00_0025d808");

void water_current_update(Moby *self) __asm__("FUN_L01_002f3120");
void water_current_update(Moby *self)
{
    PathCarrierVars *water_current_vars;
    PePath *path;
    Vec4 nearest;
    Vec4 ahead;
    Vec4 current_acceleration;
    Vec4 pull;
    f32 path_segment_index;
    s32 seg;
    s32 at_end;
    f32 hero_path_distance;
    f32 limit;
    f32 current_speed;
    f32 cap;
    s32 i;

    water_current_vars = self->vars;
    if (water_current_vars == NULL || water_current_vars->path == -1) {
        return;
    }
    path = (PePath *)D_L01_001B0930_u[water_current_vars->path];
    if (water_current_vars->ready == 0) {
        /* store each segment's length in its start point's w */
        water_current_vars->ready = 1;
        for (i = 0; i < path->count - 1; i++) {
            path->pts[i].f[3] = FUN_001f9b48(&path->pts[i], &path->pts[i + 1]);
        }
        path->pts[i].f[3] = FUN_001f9b48(&path->pts[i], &path->pts[0]);
        self->alpha = 0xFF;
    }

    if ((u32)(hero.state.control_mode - 0x11) >= 2 && hero.state.prev_control_mode != 0x11
        && hero.state.prev_control_mode != 0x12 && hero.state.current != 0x12
        && hero.state.prev != 0x12) {
        return;
    }
    FUN_L00_0025df68(path, &hero.motion.pos, &nearest, &path_segment_index, &seg, 0, 999.0f, 5.0f, 0.0f);
    at_end = sample_path(path, &ahead, &path_segment_index, &seg, 0, 2.0f);
    hero_path_distance = vector_distance(&hero.motion.pos, &nearest);
    if (water_current_vars->radius < hero_path_distance) {
        return;
    }

    if ((u32)(hero.state.control_mode - 0x11) < 2) {
        if (hero.current_path_dist < hero_path_distance) {
            return;
        }
        hero.current_path_dist = hero_path_distance;
        if (at_end == 0) {
            subtract_vector_xyz(&current_acceleration, &ahead, &nearest);
            normalize_vector_xyz(&current_acceleration, &current_acceleration, frame_time_sq * 7.0f);
            add_vector_xyz(water_current_vars, water_current_vars, &current_acceleration);
            water_current_vars->vel.f[2] = 0.0f;
            if (water_current_vars->max_speed < vector_length_xyz(water_current_vars)) {
                normalize_vector_xyz(water_current_vars, water_current_vars, water_current_vars->max_speed);
            }
            qcopy(&hero.motion.platform_delta, water_current_vars);
            if (water_current_vars->pull != 0) {
                subtract_vector_xyz(&pull, &nearest, &hero.motion.pos);
                pull.f[2] = 0.0f;
                limit = water_current_vars->max_speed * 0.4f;
                if (limit < vector_length_xyz(&pull)) {
                    normalize_vector_xyz(&pull, &pull, limit);
                }
                add_vector_xyz(&hero.motion.platform_delta, &hero.motion.platform_delta, &pull);
            }
        } else {
            /* past the end: slow down (factor 1 - 0.005 per frame unit) */
            current_speed = vector_length_xyz(water_current_vars);
            normalize_vector_xyz(water_current_vars, water_current_vars, frame_scale * -0.004999995231628418f * current_speed + current_speed);
            qcopy(&hero.motion.platform_delta, water_current_vars);
        }
        if (hero.state.control_mode == 0x12 && hero.state.prev == hero.state.control_mode) {
            if (hero.state_timer < scale_game_frames(3)) {
                cap = frame_time * 1.5f;
                if (cap < hero.ground_speed) {
                    hero.ground_speed = cap;
                }
                clear_vector(&hero.motion.momentum);
            }
        }
    } else if ((hero.state.prev_control_mode == 0x12 || hero.state.prev2_control_mode == 0x12)
               && (u32)(hero.state.control_mode - 4) < 2) {
        current_speed = vector_length_xyz(water_current_vars);
        normalize_vector_xyz(water_current_vars, water_current_vars, frame_scale * -0.014999985694885254f * current_speed + current_speed);
        qcopy(&hero.motion.platform_delta, water_current_vars);
        if (hero.jump_lockout_timer < scale_game_frames(0x23)) {
            hero.jump_lockout_timer = scale_game_frames(0x23);
        }
    }
}
#include "sda.h"

typedef struct {
    s32 target;
    f32 angle;
    f32 speed;
} RotVars;

extern Moby *D_L01_0015FFD8;
f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
f32 fast_normalize_angle(f32 a) __asm__("FUN_001fa610");

void cave_door_update(Moby *self) __asm__("FUN_L01_002f4348");
void cave_door_update(Moby *self) {
    enum {
        CAVE_DOOR_STATE_INITIALIZE = 0,
        CAVE_DOOR_STATE_UPDATE_ROTATION = 1
    };
    RotVars *cave_door_vars;
    Moby *target_moby;
    f32 *mv;

    cave_door_vars = self->vars;
    switch (self->state) {
    case CAVE_DOOR_STATE_INITIALIZE:
        cave_door_vars->angle = self->rot;
        cave_door_vars->speed = cave_door_vars->speed * DEG_TO_RAD;
        self->state = 1;
        break;
    case CAVE_DOOR_STATE_UPDATE_ROTATION:
        if (cave_door_vars->target != -1) {
            target_moby = &D_L01_0015FFD8[cave_door_vars->target];
            if (target_moby->oclass == 0x118) {
                mv = target_moby->vars;
                target_moby->alpha = 0xFF;
                self->rot = fast_add_rotations(
                    cave_door_vars->angle,
                    fast_normalize_angle(fast_add_rotations(cave_door_vars->speed, -cave_door_vars->angle) * mv[0]));
            }
        }
        break;
    }
}



/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002BA898.c: func_L01_002F5800), where it is exact; names translated to the US level program. */


extern char D_L01_00167240[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern unsigned char D_0013D408 __attribute__((section(".data")));
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern short D_L01_0015FFD8_c __asm__("D_L01_0015FFD8");
extern short D_L01_00161AF4 __attribute__((sda));
extern short D_L01_00161AF8 __attribute__((sda));
extern short D_L01_00161AFC __attribute__((sda));
extern void *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263d40(int, int);
extern void gunship_follow_path(char *) __asm__("FUN_L01_002f5168");
extern void FUN_L01_0030be70(char *, void *, float);
extern void allocate_voice_for_bank_entry_alt(int, int, int) __asm__("FUN_0022db10");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

/* Per-frame update of a breakable moby: scales its speed by distance, and on a hit spawns the break effect, frees its child mobys and removes itself; otherwise fades its alpha by height. */
void fighter_jet_update(char *moby) __asm__("FUN_L01_002f4428");
void fighter_jet_update(char *moby) {
    float hit_dir[4];
    float hit_vec[4];
    char *data = *(char **)(moby + 0x78);
    void *hit = find_moby_hit(moby, 0x800000, 0);
    float speed_scale;
    ((unsigned char *)moby)[0xA4] = 0xFF;
    if (((unsigned char *)moby)[0x20] == 0) {
        *(float *)(data + 0x170) = *(float *)(moby + 0x2C);
        *(float *)(data + 0x174) = *(float *)(data + 0xFC);
    }
    {
        float camera_distance = distance_xyz(moby + 0x10, D_L01_00167240);
        float min_scale = *(float *)&D_L01_00161AF4;
        float near_dist = *(float *)&D_L01_00161AFC;
        float slope = -((1.0f - min_scale) / (*(float *)&D_L01_00161AF8 - near_dist));
        speed_scale = slope * camera_distance + (1.0f - slope * near_dist);
        if (1.0f < speed_scale) speed_scale = 1.0f;
        if (speed_scale < min_scale) speed_scale = min_scale;
    }
    *(float *)(moby + 0x2C) = *(float *)(data + 0x170) * speed_scale;
    *(float *)(data + 0xFC) = *(float *)(data + 0x174) * speed_scale;
    if (hit != 0) {
        int i;
        int *child_ids;
        if (((unsigned char *)moby)[0x20] == 0x65) return;
        if (D_0013D408 == 0) {
            D_0013D408 = 1;
            allocate_voice_for_bank_entry_alt(1, 0, 0);
            FUN_L00_00263d40(0x53D6, -1);
        }
        subtract_vector_xyz(hit_vec, data + 0xD0, data + 0xE0);
        moby[0x20] = 0x65;
        *(u128 *)hit_dir = *(u128 *)hit_vec;
        spawn_explosion(moby, hit_dir, moby + 0x10, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, 1, 15.0f, 1, 1, -1, 0);
        FUN_L01_0030be70(moby, hit_dir, speed_scale);
        child_ids = (int *)(data + 0x140);
        for (i = 0; i < 4; i++) {
            if (child_ids[i] != -1) {
                mark_moby_for_removal((char *)(*(int *)&D_L01_0015FFD8_c + (child_ids[i] << 8)));
            }
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (((unsigned char *)moby)[0x20] == 0x65) return;
    gunship_follow_path(moby);
    *(unsigned short *)(moby + 0x34) |= 0x1000;
    data[0x2B] = 1;
    data[0x2C] = 0x64;
    if (175.0f <= *(float *)(moby + 0x18)) {
        moby[0x23] = 0;
    } else if (125.0f < *(float *)(moby + 0x18) && *(float *)(moby + 0x18) < 175.0f) {
        moby[0x23] = func_001FA898_r((175.0f - *(float *)(moby + 0x18)) * 128.0f / 50.0f);
    } else if (*(float *)(moby + 0x18) <= 125.0f) {
        ((unsigned char *)moby)[0x23] = 0x80;
    }
}

/*
 * A moby that slides between two points following another moby's progress:
 * the target (class 0x118) keeps a 0..1 progress in its first pvar. While the
 * progress changes, a looping sound plays; at either end the loop stops and a
 * one-shot sound plays.
 */
struct SliderVars {
    s32 target;                    /* moby index of the driver, -1 none */
    f32 start_x;                   /* position at spawn */
    f32 start_y;
    f32 end_x;                     /* 0 = not set up */
    f32 end_y;
    s32 sound;                     /* loop voice handle, -1 none */
    f32 last_progress;
};

/* One voice of the sound system's pool (0x70 bytes, after a 0x70-byte header). */
struct Voice {
    u32 handle;
    u8 state;                      /* 0 free */
    u8 pad_05[0x13];
    Moby *owner;                   /* 0x18 */
    u8 pad_1C[0x54];
};

/* Retail addresses voice i as pool base + i * 0x70 plus the header and field
   offset (not as &voices[i]), so the pool is viewed through this window. */
struct VoicePoolWindow {
    u8 header[0x70];
    struct Voice voice;
};

extern u8 voice_pool[] __asm__("D_0013E550");
/* nonzero while voice `handle` plays (state 1 or 2) for `moby` */
extern int voice_is_playing_for(void *moby, int handle) __asm__("FUN_L00_0028d8c0");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_0022da68");

void door_half_update(Moby *self) __asm__("FUN_L01_002f4710");
void door_half_update(Moby *self)
{
    enum {
        DOOR_HALF_STATE_INITIALIZE = 0,
        DOOR_HALF_STATE_FOLLOW_DRIVER = 1
    };
    struct SliderVars *door_half_vars;
    Moby *driver;
    f32 *progress;
    s32 handle;
    struct VoicePoolWindow *window;

    door_half_vars = (struct SliderVars *)self->vars;
    switch (self->state) {
    case DOOR_HALF_STATE_INITIALIZE:
        door_half_vars->start_x = self->pos.x;
        door_half_vars->start_y = self->pos.y;
        door_half_vars->sound = -1;
        door_half_vars->last_progress = 0.0f;
        self->state = 1;
        break;
    case DOOR_HALF_STATE_FOLLOW_DRIVER:
        if (door_half_vars->target == -1 || door_half_vars->end_x == 0.0f || door_half_vars->end_y == 0.0f)
            break;
        driver = &D_L01_0015FFD8[door_half_vars->target];
        if (driver->oclass != 0x118)
            break;

        progress = (f32 *)driver->vars;
        self->pos.x = door_half_vars->start_x + (door_half_vars->end_x - door_half_vars->start_x) * progress[0];
        self->pos.y = door_half_vars->start_y + (door_half_vars->end_y - door_half_vars->start_y) * progress[0];
        if (progress[0] != door_half_vars->last_progress) {
            if (progress[0] == 1.0f || progress[0] == 0.0f) {
                /* reached an end: stop the loop, play the stop sound */
                if (voice_is_playing_for(self, door_half_vars->sound)) {
                    handle = door_half_vars->sound;
                    if (handle != -1) {
                        window = (struct VoicePoolWindow *)(voice_pool + handle * sizeof(struct Voice));
                        if (window->voice.owner == self && window->voice.state != 0)
                            release_voice_slot(handle);
                    }
                    door_half_vars->sound = -1;
                }
                allocate_voice_for_target_entry(1, 0, self);
            } else if (!voice_is_playing_for(self, door_half_vars->sound)) {
                door_half_vars->sound = allocate_voice_for_target_entry(0, 4, self);
            }
        } else {
            /* not moving: stop the loop */
            if (voice_is_playing_for(self, door_half_vars->sound)) {
                handle = door_half_vars->sound;
                if (handle != -1) {
                    window = (struct VoicePoolWindow *)(voice_pool + handle * sizeof(struct Voice));
                    if (window->voice.owner == self && window->voice.state != 0)
                        release_voice_slot(handle);
                }
                door_half_vars->sound = -1;
            }
        }
        door_half_vars->last_progress = progress[0];
        break;
    }
}
/* pvars of the rider carrier stationary_ship_update runs */
typedef struct {
    u8 pad0[0x60];
    s32 node;                 /* 0x60: path node it is at */
    u8 pad64[0x10];
    s32 path;                 /* 0x74: index into D_L01_001B0930, -1 none */
    u8 pad78[0x58];
    Vec4 dropoff;             /* 0xD0 */
    u8 padE0[0x70];
    s32 riders[8];            /* 0x150: moby indices, -1 empty */
    s32 volume;               /* 0x170: clip volume that starts it, -1 none */
    s16 state;                /* 0x174 */
    s16 timer;                /* 0x176 */
    s32 arrived;              /* 0x178 */
    s32 stop;                 /* 0x17C: first marked node of the path */
    s32 camera;               /* 0x180: moby index of the camera, -1 none */
    s32 keep;                 /* 0x184 */
    f32 seconds;              /* 0x188 */
    f32 turn;                 /* 0x18C */
    f32 max_deg;              /* 0x190 */
    s32 unk194;               /* 0x194 */
} CarrierVars;

typedef struct {
    u8 pad0[0x10];
    f32 x;                    /* 0x10 */
    f32 y;                    /* 0x14 */
    f32 yaw;                  /* 0x18 */
    u8 pad1C[8];
    s32 node;                 /* 0x24: index into D_L01_001600EC */
    u8 pad28[4];
    s32 frames;               /* 0x2C */
} CarrierCamVars;

typedef struct {
    u8 pad0[0x140];
    f32 x;                    /* 0x140 */
    f32 y;                    /* 0x144 */
    u8 pad148[0x10];
    f32 yaw;                  /* 0x158 */
} CarrierCamera;

typedef struct {
    u8 pad0[0x1D0];
    Vec4 drop;                /* 0x1D0 */
    u8 pad1E0[8];
    s32 dropped;              /* 0x1E8 */
    s16 drop_timer;           /* 0x1EC */
} CarrierRiderVars;

typedef struct {
    Vec4 v[8];
} CarrierOffsets;

/* One 0x80-byte camera record of the level table at D_L01_001600EC. */
typedef struct {
    u8 pad0[0x70];
    f32 x;                    /* 0x70 */
    f32 y;                    /* 0x74 */
    f32 yaw;                  /* 0x78 */
    u8 pad7C[4];
} CarrierCamPoint;

/* struct Moby fields this function uses (the unit keeps a file-local struct Moby of its own, so
   rnc/gameplay/entities/moby.h cannot be included here). */
typedef struct {
    u8 pad0[0x10];
    Vec4f pos;                /* 0x10 */
    u8 state;                 /* 0x20 */
    u8 pad21[0xF];
    u8 unk30;                 /* 0x30 */
    u8 pad31[3];
    u16 flags;                /* 0x34 */
    u8 pad36[0xA];
    Vec4f rot;                /* 0x40 */
    u8 pad50[2];
    u8 seq;                   /* 0x52 */
    u8 prev_seq;              /* 0x53 */
    u8 pad54[0x24];
    void *pvars;              /* 0x78 */
    u8 pad7C[0x2A];
    s16 oclass;               /* 0xA6 */
    u8 padA8[0x14];
    u8 unkBC;                 /* 0xBC */
    u8 padBD[3];
    Vec4f unkC0;              /* 0xC0 */
    u8 padD0[0x30];
} CarrierMoby;

extern CarrierMoby *D_L01_0015FFD8_m __asm__("D_L01_0015FFD8");
extern CarrierCamPoint *D_L01_001600EC_p __asm__("D_L01_001600EC");
extern CarrierCamera D_L01_00167100_c __asm__("D_L01_00167100");
extern CarrierOffsets D_L01_0020B360;
extern s32 truncate_float_f49(f32) __asm__("FUN_001fa6d0");
extern f32 fast_subtract_rotations_f49(f32, f32) __asm__("FUN_001fa5c8");
extern void transform_vector_f49(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void FUN_L00_00250df8(void *);
extern void gunship_follow_path(char *) __asm__("FUN_L01_002f5168");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

/* Rider carrier: waits for the hero in its clip volume, then follows its path carrying up to eight rider mobys (and steers its camera moby), drops them off one by one at the marked node and removes itself at the end of the path. */
void stationary_ship_update(CarrierMoby *m) __asm__("FUN_L01_002f4960");
void stationary_ship_update(CarrierMoby *carrier_moby) {
    enum {
        CARRIER_STATE_INITIALIZE = 0,
        CARRIER_STATE_WAIT_FOR_TRIGGER = 1,
        CARRIER_STATE_FOLLOW_PATH = 2
    };
    enum {
        CARRIER_RIDER_CLASS_TURRET = 0x1CB
    };
    CarrierVars *v;
    CarrierCamVars *cv;
    PePath *carrier_path;
    CarrierMoby *level_moby_array;
    CarrierRiderVars *rv;
    CarrierOffsets offs;
    s32 i;
    s32 j;
    s32 volume_or_path_node_index;
    s32 k;
    f32 camera_target_yaw;
    f32 camera_node_yaw_delta;

    v = (CarrierVars *)carrier_moby->pvars;
    volume_or_path_node_index = v->volume;
    if (volume_or_path_node_index == -1) {
        if (carrier_moby->prev_seq != 5) {
            blend_moby_animation_cf(carrier_moby, 5, 0, 0);
        }
        return;
    }
    switch (v->state) {
    case CARRIER_STATE_INITIALIZE:
        v->unk194 = -1;
        v->state = 1;
        v->arrived = 0;
        v->keep = 0;
        carrier_moby->unk30 = 0xFF;
        carrier_moby->flags |= 0x41;
        return;
    case CARRIER_STATE_WAIT_FOR_TRIGGER:
        if (is_point_inside_clip_volume(&hero.motion.pos, volume_or_path_node_index) != 0) {
            v->state = 2;
            carrier_moby->flags &= ~0x41;
            if (v->camera != -1) {
                cv = (CarrierCamVars *)D_L01_0015FFD8_m[v->camera].pvars;
                cv->frames = scale_game_frames(truncate_float_f49(v->seconds * 60.0f));
                D_L01_0015FFD8_m[v->camera].unkBC = 1;
            }
            if (v->path != -1) {
                carrier_path = (PePath *)D_L01_001B0930_u[v->path];
                for (volume_or_path_node_index = 0; volume_or_path_node_index < carrier_path->count; volume_or_path_node_index++) {
                    if (carrier_path->pts[volume_or_path_node_index].f[3] == 1.0f) {
                        v->stop = volume_or_path_node_index;
                        break;
                    }
                }
            }
        }
        break;
    case CARRIER_STATE_FOLLOW_PATH:
        gunship_follow_path((char *)carrier_moby);
        if (v->camera != -1) {
            cv = (CarrierCamVars *)D_L01_0015FFD8_m[v->camera].pvars;
            camera_target_yaw = FUN_001f9e90_cf(carrier_moby->pos.x - D_L01_00167100_c.x, carrier_moby->pos.y - D_L01_00167100_c.y);
            camera_node_yaw_delta = fast_subtract_rotations_f49(D_L01_001600EC_p[cv->node].yaw, camera_target_yaw);
            if (camera_node_yaw_delta < 0.0f) {
                camera_target_yaw = D_L01_001600EC_p[cv->node].yaw;
            } else if (v->max_deg * 0.017453292f < camera_node_yaw_delta) {
                camera_target_yaw = fast_add_rotations(D_L01_001600EC_p[cv->node].yaw, -v->max_deg * 0.017453292f);
            }
            cv->x = D_L01_001600EC_p[cv->node].x;
            cv->y = D_L01_001600EC_p[cv->node].y;
            cv->yaw = spring_turn(&v->turn, D_L01_00167100_c.yaw, camera_target_yaw, frame_time_sq * 12.566371f,
                                       frame_time_sq * 125.6637039f, frame_time * 3.1415927f);
        }
        if (v->path != -1) {
            carrier_path = (PePath *)D_L01_001B0930_u[v->path];
            k = v->node;
            if (k == v->stop && v->arrived == 0) {
                if (carrier_moby->prev_seq != 2) {
                    blend_moby_animation_cf(carrier_moby, 2, 0, scale_game_frames(0x14));
                }
                v->timer = 0;
                v->arrived = 1;
            } else if (k == carrier_path->count - 2 && v->keep == 0) {
                mark_moby_for_removal(carrier_moby);
                return;
            }
        }
        break;
    }
    fast_dec_timer_s16(&v->timer);
    if (v->arrived == 0 || v->timer != 0 || carrier_moby->seq != 2) {
        for (i = 0; i < 8; i++) {
            offs = D_L01_0020B360;
            if (v->riders[i] != -1) {
                if (v->state != 1) {
                    D_L01_0015FFD8_m[v->riders[i]].flags &= ~0x41;
                }
                transform_vector_f49(&offs.v[i], &offs.v[i], &carrier_moby->unkC0);
                fast_vec_add(&D_L01_0015FFD8_m[v->riders[i]].pos, &carrier_moby->pos, &offs.v[i]);
                qcopy_nc(&D_L01_0015FFD8_m[v->riders[i]].rot, &carrier_moby->rot);
                D_L01_0015FFD8_m[v->riders[i]].rot.z =
                    fast_add_rotations(D_L01_0015FFD8_m[v->riders[i]].rot.z, 3.1415927f);
                advance_moby_animation(&D_L01_0015FFD8_m[v->riders[i]]);
                FUN_L00_00250df8(&D_L01_0015FFD8_m[v->riders[i]]);
                D_L01_0015FFD8_m[v->riders[i]].flags |= 6;
            }
        }
        return;
    }
    for (j = 0; j < 8; j++) {
        if (v->riders[j] != -1) {
            level_moby_array = D_L01_0015FFD8_m;
            level_moby_array[v->riders[j]].flags &= ~6;
            clear_vector(&level_moby_array[v->riders[j]].rot);
            D_L01_0015FFD8_m[v->riders[j]].rot.z = FUN_001f9e90_cf(hero.motion.pos.f[0] - carrier_moby->pos.x, hero.motion.pos.f[1] - carrier_moby->pos.y);
            switch (D_L01_0015FFD8_m[v->riders[j]].oclass) {
            case 0x154:
                D_L01_0015FFD8_m[v->riders[j]].state = 2;
                break;
            case 0x1AB:
                D_L01_0015FFD8_m[v->riders[j]].state = 4;
                break;
            case CARRIER_RIDER_CLASS_TURRET:
                rv = (CarrierRiderVars *)D_L01_0015FFD8_m[v->riders[j]].pvars;
                rv->dropped = 1;
                rv->drop_timer = scale_game_frames(0x1E);
                subtract_vector_xyz(&rv->drop, &v->dropoff, &carrier_moby->pos);
                break;
            }
            v->riders[j] = -1;
            v->timer = random_int_between(0x14, 0x1E);
            return;
        }
    }
    if (carrier_moby->prev_seq != 0) {
        blend_moby_animation_cf(carrier_moby, 0, 0, scale_game_frames(0x14));
    }
}
extern ScrollLayer D_L01_001E2FC0[];
void mine_blend_color(void) __asm__("FUN_L00_002371e0");
void update_and_draw_scrolling_overlay_layers(s32, ScrollLayer *) __asm__("FUN_L01_002b96e0");

void update_and_draw_level01_scrolling_layers(void) __asm__("FUN_L01_002f60f8");

void update_and_draw_level01_scrolling_layers(void) {
    mine_blend_color();
    update_and_draw_scrolling_overlay_layers(8, D_L01_001E2FC0);
}
#include "eetypes.h"
#include "qzero.h"
#include "qcopy.h"

#include "rnc/math/vector.h"

typedef struct {
    Vec4 *pts;
    u8 pad4[4];
    Vec4 *out;
    s32 count;
    u8 pad10[0x50];
} BoundGroup;

s32 D_L01_00161BA0 = 0;
extern BoundGroup D_L01_001E3380[];
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern void draw_two_scroll_layers(void) __asm__("FUN_L01_002f6150");
void enqueue_callback_list_1(void *fn, void *arg) __asm__("FUN_001f4600");

void initialize_bounds_and_enqueue_scroll_layers(void *callback_argument) __asm__("FUN_L01_002f6180");

void initialize_bounds_and_enqueue_scroll_layers(void *callback_argument) {
    Vec4 bounding_sphere;
    Vec4 point_to_center;
    Vec4 *point_cursor;
    s32 i;
    s32 j;
    s32 k;
    f32 r;

    if (D_L01_00161BA0 == 0) {
        D_L01_00161BA0 = 1;
        for (i = 0; i < 2; i++) {
            qzero(&bounding_sphere);
            point_cursor = (D_L01_001E3380 + i)->pts;
            for (j = 0; j < (D_L01_001E3380 + i)->count; j++) {
                fast_vec_add(&bounding_sphere, &bounding_sphere, point_cursor);
                point_cursor++;
            }
            fast_vec_scale(&bounding_sphere, &bounding_sphere, 1.0f / convert_integer_to_float((D_L01_001E3380 + i)->count));
            bounding_sphere.f[3] = 0.0f;
            point_cursor = (D_L01_001E3380 + i)->pts;
            for (k = 0; k < (D_L01_001E3380 + i)->count; k++) {
                fast_vec_sub(&point_to_center, &bounding_sphere, point_cursor);
                r = fast_vec_length(&point_to_center);
                if (bounding_sphere.f[3] < r) {
                    bounding_sphere.f[3] = r;
                }
                point_cursor++;
            }
            qcopy((D_L01_001E3380 + i)->out, &bounding_sphere);
        }
    }
    enqueue_callback_list_1(draw_two_scroll_layers, callback_argument);
}
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    Vec4 dir;
    u128 color;
    u8 pad20[0x10];
    void *owner;
    s32 kind;
    f32 len;
} BeamVars;

typedef struct {
    u8 pad0[0x10];
    u128 pos;
    u8 state;
    u8 pad21[0xB];
    f32 scale;
    u8 b30;
    u8 b31;
    s16 h32;
    u8 pad34[0xC];
    s32 w40;
    f32 rot_x;
    f32 rot_z;
    u8 pad4c[0x2C];
    BeamVars *vars;
} BeamMoby;

extern void *create_moby(s32 oclass) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8();
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void spawn_source_attached_fading_effect_particle(void *, u32, s32, s32, f32) __asm__("FUN_L00_0026daa0");
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("FUN_0022da68");

BeamMoby *FUN_L01_002f68a0(void *owner, u128 *pos, u128 *dir, u128 *color, s32 kind) {
    Vec4 position_copy;
    Vec4 direction_copy;
    Vec4 target_position_copy;
    BeamMoby *created_moby;
    BeamVars *beam_vars;
    Vec4 *pp;
    Vec4 *pd;
    Vec4 *pc;

    pp = &position_copy;
    pd = &direction_copy;
    pc = &target_position_copy;
    position_copy.q = *pos;
    direction_copy.q = *dir;
    target_position_copy.q = *color;
    created_moby = create_moby(0x2AE);
    if (created_moby != NULL) {
        beam_vars = created_moby->vars;
        beam_vars->owner = owner;
        created_moby->w40 = 0;
        created_moby->b30 = 0xFF;
        created_moby->h32 = 0xFF;
        created_moby->b31 = 1;
        created_moby->scale = created_moby->scale * 0.25f;
        created_moby->rot_x = -fast_arc_tan(vector_length_xy(pd), pd->f[2]);
        created_moby->rot_z = fast_arc_tan(direction_copy.f[0], pd->f[1]);
        qcopy(&created_moby->pos, pp);
        qcopy(&beam_vars->dir, pd);
        qcopy(&beam_vars->color, pc);
        beam_vars->kind = kind;
        beam_vars->len = fast_vec_length(pd);
        spawn_source_attached_fading_effect_particle(created_moby, 0x2F4F7F7F, scale_game_frames(0x78), -1, 420000.0f);
        spawn_source_attached_fading_effect_particle(created_moby, 0x4F6F7F7F, scale_game_frames(0x78), -1, 210000.0f);
        allocate_voice_for_target_entry(0, 0, created_moby);
        FUN_L00_00250df8(created_moby);
    }
    return created_moby;
}
typedef struct {
    s32 v[4];
} GunshipTargets;

typedef struct {
    s32 cur;
    u8 pad4[0x10];
    s32 path;
} GunshipFollow;

typedef struct {
    u8 pad0[0x2B];
    u8 b2B;
    u8 b2C;
    u8 pad2D[0x33];
    GunshipFollow sub;
    u8 pad78[0x58];
    f32 vD0[4];
    f32 vE0[4];
    u8 padF0[0xC];
    f32 speed;
    u8 pad100[0x40];
    s32 tethers[4];
    u8 pad150[0x1C];
    s32 last_fire_node;
    s32 trigger_volume;
    s32 cutscene_camera;
    f32 base_scale;
    f32 base_speed;
    u8 stop_nodes[4];
    s16 gun_side;
    s16 cutscene_timer;
    s32 camera_block_volume;
    s32 beam_volume;
    s32 targets[4][4];
    s32 guards[8];
    s32 turrets[4];
} GunshipVars;

typedef struct {
    u8 pad0[0x10];
    f32 pos[4];
    u8 state;
    u8 pad21[2];
    u8 alpha;
    u8 pad24[8];
    f32 scale;
    u8 b30;
    u8 pad31;
    s16 h32;
    u16 flags;
    u8 pad36[0x42];
    void *vars;
    u8 pad7C[0x18];
    s32 w94;
    u8 pad98[0xC];
    u8 bA4;
    u8 padA5;
    s16 oclass;
    u8 padA8[0xA];
    u16 uid;
    u8 padB4[0x4C];
} GunshipMoby;

typedef struct {
    u8 pad0[0x1E8];
    s32 activated;
    s16 activate_delay;
} GunshipTurretVars;

typedef struct {
    s32 f0;
    s16 f4;
    s16 pad6;
    s32 f8;
} GunshipGuardVars;

typedef struct {
    s32 count;
    u8 pad4[0x18];
    f32 w;
} GunshipPath;

typedef struct {
    u8 pad0[0x18];
    GunshipMoby *moby;
} GunshipHit;

extern f32 gunship_min_scale __asm__("D_L01_00161BD0") __attribute__((sda));
extern f32 gunship_far_dist __asm__("D_L01_00161BD4") __attribute__((sda));
extern f32 gunship_near_dist __asm__("D_L01_00161BD8") __attribute__((sda));
GunshipTargets gunship_no_targets __asm__("D_L01_00161BE0") = {{-1, -1, -1, -1}};
extern char camera_pos[] __asm__("D_L01_00167240");
extern u8 gunship_banner_shown __asm__("D_0013D408") NOT_SDA;
extern char gunship_no_path_fmt[] __asm__("D_L01_0020B4A0");
extern u8 moby_collected[] __asm__("D_L01_001BBB04");
extern GunshipHit collision_hit __asm__("D_L01_001742C0");
extern s32 level_moby_killed[][64] __asm__("D_0014C190");
extern s32 moby_killed[] __asm__("D_L01_001BA950");
extern s32 moby_array __asm__("D_L01_0015FFD8") MACRO_ADDR;
extern char *camera_points __asm__("D_L01_001600EC") MACRO_ADDR;
extern char *paths[] __asm__("D_L01_001B0930");
extern s32 cutscene_active __asm__("D_L01_0015F404") MACRO_ADDR;

extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern void *find_moby_hit(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern void allocate_voice_for_bank_entry(s32, s32, s32) __asm__("FUN_0022db10");
extern void queue_dialogue_message(s32, s32) __asm__("FUN_L00_00263d40");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void spawn_explosion(void *, void *, void *, f32, f32, s32, s32, s32, f32, f32, f32, f32, s32, f32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern void spawn_debris(char *, void *, f32) __asm__("FUN_L01_0030be70");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int stub_printf() __asm__("FUN_001e93b0");
extern s32 truncate_float_to_s32_f(f32) __asm__("FUN_001fa6d0");
extern void gunship_follow_path(char *) __asm__("FUN_L01_002f5168");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern s32 hero_set_state(s32, s32) __asm__("FUN_L01_0023cf98");
extern void cutscene_camera_start(void *, void *, s32, s32, s32) __asm__("FUN_L00_002eaaa0");
extern void cutscene_camera_stop(s32) __asm__("FUN_L00_002eac18");
extern s32 fast_dec_timer(void *) __asm__("FUN_001f9770");
extern s32 collision_line(void *, void *, s32, void *, void *) __asm__("FUN_001efa68");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void moby_point_position(void *, s32, void *) __asm__("FUN_L00_0024f7c8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern GunshipMoby *spawn_beam(void *, void *, void *, void *, s32, s32) __asm__("FUN_L01_002f68a0");

extern s32 current_level_index_macro __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern f32 frame_time_macro __asm__("D_0015ED6C") __attribute__((section(".sdata")));
/* Ported from rac1-decomp src/overlays/l01_novalis/vendor_002BA898.c (func_L01_002F8B00) */
void bomber_jet_update(GunshipMoby *m) __asm__("FUN_L01_002f7728");
void bomber_jet_update(GunshipMoby *bomber_jet_moby) {
    GunshipVars *gunship_vars = bomber_jet_moby->vars;
    GunshipFollow *sub = &gunship_vars->sub;
    float distance_scale;
    void *hit;
    int i;
    int j;
    GunshipTargets explosion_vector_or_target_indices;
    GunshipTargets explosion_vector_or_filtered_target_indices;
    SplashVec beam_step_vector;
    SplashVec pos;
    SplashVec col;
    {
        float camera_distance = distance_xyz(bomber_jet_moby->pos, camera_pos);
        float x0 = gunship_near_dist;
        float k = -((1.0f - gunship_min_scale) / (gunship_far_dist - x0));
        distance_scale = k * camera_distance + (1.0f - k * x0);
    }
    hit = find_moby_hit(bomber_jet_moby, 0x800000, 0);
    bomber_jet_moby->bA4 = 0xFF;
    if (hit != 0) {
        if (gunship_banner_shown == 0) {
            gunship_banner_shown = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        subtract_vector_xyz(&explosion_vector_or_filtered_target_indices, gunship_vars->vD0, gunship_vars->vE0);
        *(u128 *)&explosion_vector_or_target_indices = *(u128 *)&explosion_vector_or_filtered_target_indices;
        spawn_explosion(bomber_jet_moby, &explosion_vector_or_target_indices, bomber_jet_moby->pos, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, 1, 15.0f, 1, 1, -1, 0);
        spawn_debris((char *)bomber_jet_moby, &explosion_vector_or_target_indices, distance_scale * 2.0f);
        for (i = 0; i < 4; i++) {
            if (gunship_vars->tethers[i] != -1) {
                mark_moby_for_removal((GunshipMoby *)(moby_array + (gunship_vars->tethers[i] << 8)));
            }
        }
        mark_moby_for_removal(bomber_jet_moby);
        return;
    }
    do {
        if (bomber_jet_moby->state == 0) {
            if (sub->path != -1) {
                GunshipPath *path = (GunshipPath *)paths[sub->path];
                j = 0;
                gunship_vars->speed = gunship_vars->speed * 2.0f;
                if (distance_xyz((char *)path + 0x10, (char *)path + (path->count << 4)) < 0.5f) {
                    path->count--;
                }
                for (i = 0; i < path->count; i++) {
                    if (((GunshipPath *)((char *)path + (i << 4)))->w == 1.0f) {
                        gunship_vars->stop_nodes[j] = i;
                        j++;
                        if (j >= 4) break;
                    }
                }
                gunship_vars->gun_side = 0;
                gunship_vars->last_fire_node = -1;
                gunship_vars->cutscene_timer = -1;
                gunship_vars->base_scale = bomber_jet_moby->scale;
                gunship_vars->base_speed = gunship_vars->speed;
            } else {
                stub_printf(gunship_no_path_fmt, bomber_jet_moby->oclass, truncate_float_to_s32_f((float)(((int)bomber_jet_moby - moby_array) >> 8)));
            }
            bomber_jet_moby->b30 = 0xFF;
            bomber_jet_moby->h32 = 0xFF;
        }
        if (1.0f < distance_scale) distance_scale = 1.0f;
        if (distance_scale < gunship_min_scale) distance_scale = gunship_min_scale;
        bomber_jet_moby->scale = gunship_vars->base_scale * distance_scale;
        gunship_vars->speed = gunship_vars->base_speed * distance_scale;
        if (175.0f <= bomber_jet_moby->pos[2]) {
            bomber_jet_moby->alpha = 0;
        } else if (125.0f < bomber_jet_moby->pos[2] && bomber_jet_moby->pos[2] < 175.0f) {
            bomber_jet_moby->alpha = truncate_float_to_s32_f((175.0f - bomber_jet_moby->pos[2]) * 128.0f / 50.0f);
        } else if (bomber_jet_moby->pos[2] <= 125.0f) {
            bomber_jet_moby->alpha = 0x80;
        }
        if (gunship_vars->trigger_volume != -1) {
            if (bomber_jet_moby->state == 0) {
                gunship_follow_path((char *)bomber_jet_moby);
            }
            if (gunship_vars->cutscene_timer == -1 && is_point_inside_clip_volume(&hero.motion.pos, gunship_vars->trigger_volume)) {
                if (moby_collected[(short)bomber_jet_moby->uid] != 0
                    || (level_moby_killed[current_level_index_macro][(short)bomber_jet_moby->uid >> 5] >> (bomber_jet_moby->uid & 0x1F)) & 1) {
                    for (j = 0; j < 4; j++) {
                        if (gunship_vars->turrets[j] != -1) {
                            GunshipMoby *o = (GunshipMoby *)(moby_array + (gunship_vars->turrets[j] << 8));
                            if (o != 0 && o->oclass == 0x1CB && o->state != 0xFE && o->state != 0xFD) {
                                GunshipTurretVars *t = o->vars;
                                t->activated = 1;
                                t->activate_delay = scale_game_frames(2);
                            }
                        }
                    }
                    mark_moby_for_removal(bomber_jet_moby);
                    return;
                }
                bomber_jet_moby->flags &= 0xFFBE;
                if (gunship_vars->cutscene_camera != -1) {
                    hero_set_state(0x72, 0);
                    cutscene_camera_start(camera_points + (gunship_vars->cutscene_camera << 7) + 0x30, camera_points + (gunship_vars->cutscene_camera << 7) + 0x70, 0, 0, 0);
                    cutscene_active = 1;
                    bomber_jet_moby->w94 = 0;
                    bomber_jet_moby->flags &= 0xEFFF;
                    gunship_vars->cutscene_timer = scale_game_frames(0x1FE);
                }
            } else if (gunship_vars->cutscene_timer != -1) {
                if (fast_dec_timer(&gunship_vars->cutscene_timer)) {
                    cutscene_camera_stop(0);
                    hero_set_state(0, 1);
                    cutscene_active = 0;
                    level_moby_killed[current_level_index_macro][(short)bomber_jet_moby->uid >> 5] |= 1 << (bomber_jet_moby->uid & 0x1F);
                    moby_killed[(short)bomber_jet_moby->uid >> 5] |= 1 << (bomber_jet_moby->uid & 0x1F);
                    mark_moby_for_removal(bomber_jet_moby);
                    return;
                }
                if (gunship_vars->cutscene_timer == scale_game_frames(0x3C)) {
                    for (j = 0; j < 4; j++) {
                        if (gunship_vars->turrets[j] != -1) {
                            GunshipMoby *o = (GunshipMoby *)((gunship_vars->turrets[j] << 8) + moby_array);
                            if (o->oclass == 0x1CB) {
                                GunshipTurretVars *t = o->vars;
                                t->activated = 1;
                                t->activate_delay = scale_game_frames(2);
                            }
                        }
                    }
                }
                gunship_follow_path((char *)bomber_jet_moby);
            } else {
                bomber_jet_moby->flags |= 0x41;
                break;
            }
        } else {
            gunship_follow_path((char *)bomber_jet_moby);
        }
        bomber_jet_moby->flags |= 0x1000;
        gunship_vars->b2B = 1;
        gunship_vars->b2C = 0x2F;
        if (gunship_vars->last_fire_node != gunship_vars->sub.cur) {
            int found = -1;
            for (i = 0; i < 4; i++) {
                unsigned char *t = gunship_vars->stop_nodes;
                if (gunship_vars->sub.cur == t[i]) found = i;
            }
            if (found != -1) {
                int remaining_target_count;
                explosion_vector_or_target_indices = gunship_no_targets;
                j = 0;
                explosion_vector_or_filtered_target_indices = gunship_no_targets;
                remaining_target_count = 0;
                for (i = 0; i < 4; i++) {
                    if (gunship_vars->targets[found][i] != -1) {
                        explosion_vector_or_target_indices.v[j] = gunship_vars->targets[found][i];
                        j++;
                        explosion_vector_or_filtered_target_indices.v[remaining_target_count] = gunship_vars->targets[found][i];
                        remaining_target_count++;
                    }
                }
                if (j == 0) break;
                if (is_point_inside_clip_volume(camera_pos, gunship_vars->camera_block_volume)) break;
                for (i = j - 1; i >= 0; i--) {
                    if (collision_line(camera_pos, camera_points + (explosion_vector_or_target_indices.v[i] << 7) + 0x30, 0x86, bomber_jet_moby, 0)) {
                        if (collision_hit.moby == 0 || collision_hit.moby != bomber_jet_moby) {
                            explosion_vector_or_filtered_target_indices.v[i] = -1;
                            remaining_target_count--;
                            break;
                        }
                    }
                }
                if (remaining_target_count != 0) {
                    int k = 0;
                    for (i = 0; i < j; i++) {
                        if (explosion_vector_or_filtered_target_indices.v[i] != -1) {
                            explosion_vector_or_target_indices.v[k] = explosion_vector_or_filtered_target_indices.v[i];
                            k++;
                        }
                    }
                    j = remaining_target_count;
                }
                if (j != 0) {
                    GunshipMoby *bm;
                    i = explosion_vector_or_target_indices.v[random_integer_below(j)];
                    gunship_vars->gun_side ^= 1;
                    moby_point_position(bomber_jet_moby, gunship_vars->gun_side, &pos);
                    subtract_vector_xyz(&beam_step_vector, camera_points + (i << 7) + 0x30, &pos);
                    normalize_vector_xyz(&beam_step_vector, &beam_step_vector, frame_time_macro * 160.0f);
                    col.q = *(u128 *)((i << 7) + (int)camera_points + 0x30);
                    bm = spawn_beam(bomber_jet_moby, &pos, &beam_step_vector, &col, is_point_inside_clip_volume(&hero.motion.pos, gunship_vars->beam_volume), gunship_vars->trigger_volume != -1);
                    if (bm != 0) {
                        int k = 0;
                        int *out = (int *)((char *)bm->vars + 0x20);
                        {
                            unsigned char *t = gunship_vars->stop_nodes;
                            gunship_vars->last_fire_node = t[found];
                        }
                        for (i = 0; i < 8; i++) {
                            if (gunship_vars->guards[i] != -1) {
                                GunshipMoby *o = (GunshipMoby *)((gunship_vars->guards[i] << 8) + moby_array);
                                if (o->oclass == 0x2BC) {
                                    GunshipGuardVars *guard_vars = o->vars;
                                    if (guard_vars->f4 == 0 && guard_vars->f0 == 0 && random_integer_below(0x64) < 0x64
                                        && is_point_inside_clip_volume(&hero.motion.pos, guard_vars->f8)
                                        && is_point_inside_clip_volume(camera_pos, guard_vars->f8)) {
                                        out[k] = gunship_vars->guards[i];
                                        k++;
                                    }
                                    if (k >= 4) break;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (gunship_vars->cutscene_timer > scale_game_frames(0x1FE) - scale_game_frames(0x78));
}
f32 FUN_001f99c0(f32);

f32 FUN_L01_002f81c8(f32 *spring_axis_position, f32 *spring_axis_velocity, f32 target, f32 velocity_adjustment_scale, f32 damp, f32 range) {
    f32 velocity_adjustment;
    f32 f;

    range = range * range;
    damp = ((1.0f - damp) - 1.0f) * frame_scale + 1.0f;
    *spring_axis_velocity = *spring_axis_velocity * damp;
    velocity_adjustment = *spring_axis_position - target;
    velocity_adjustment = velocity_adjustment * velocity_adjustment;
    if (range < velocity_adjustment) {
        velocity_adjustment = range;
    }
    velocity_adjustment = velocity_adjustment * velocity_adjustment_scale / range;
    if (target < *spring_axis_position) {
        *spring_axis_velocity = *spring_axis_velocity - velocity_adjustment;
    } else {
        *spring_axis_velocity = *spring_axis_velocity + velocity_adjustment;
    }
    *spring_axis_position = *spring_axis_position + *spring_axis_velocity;
    return FUN_001f99c0(target - *spring_axis_position);
}
typedef union {
    u128 q;
    struct {
        f32 x, y, z, w;
    } v;
} HoverVec;

typedef struct {
    f32 home_x;
    f32 home_y;
    f32 pad8[2];
    f32 vel_x;
    f32 vel_y;
} HoverVars;

typedef struct {
    u8 pad0[0x10];
    HoverVec pos;
    u8 state;
    u8 pad21[0x57];
    HoverVars *pvars;
} HoverMoby;


#include "rnc/overlay/l01/gameplay/entities/birdbot_globals.h"
extern f32 water_lily_spring_velocity_adjustment_scale_gp __asm__("D_L01_00161BF0") __attribute__((sda));
extern f32 water_lily_spring_damping_gp __asm__("D_L01_00161BF4") __attribute__((sda));
extern f32 water_lily_hero_separation_radius_gp __asm__("D_L01_00161BF8") __attribute__((sda));
extern f32 water_lily_max_horizontal_speed_gp __asm__("D_L01_00161BFC") __attribute__((sda));
extern f32 water_lily_collision_radius_gp __asm__("D_L01_00161C00") __attribute__((sda));
extern f32 spring_axis_to(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L01_002f81c8");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
f32 FUN_001f99c0(f32);
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void FUN_L00_00258490(void *, void *, void *, s32, f32, f32, f32, f32);
extern f32 FUN_001f9b48(void *, void *);
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 resolve_camera_surface_height(void *, s32) __asm__("FUN_002135f0");

void water_lily_update(HoverMoby *m) __asm__("FUN_L01_002f8268");
void water_lily_update(HoverMoby *water_lily) {
    enum {
        WATER_LILY_STATE_INITIALIZE = 0,
        WATER_LILY_STATE_UPDATE_POSITION = 1
    };
    HoverVars *v;
    struct Hero *pl;
    HoverVec old;
    HoverVec collision_adjusted_position;
    HoverVec step;
    HoverVec vector_scratch;
    f32 hero_separation_angle;
    f32 dz;
    f32 max_vertical_step;

    old.q = water_lily->pos.q;
    v = water_lily->pvars;
    collision_adjusted_position.q = old.q;
    if (v == 0) {
        return;
    }
    switch (water_lily->state) {
    case WATER_LILY_STATE_INITIALIZE:
        *(u128 *)v = old.q;
        water_lily->state = 1;
        break;
    case WATER_LILY_STATE_UPDATE_POSITION:
        spring_axis_to(&water_lily->pos.v.x, v->home_x, &v->vel_x, water_lily_spring_velocity_adjustment_scale_gp * frame_time_sq,
                       water_lily_spring_damping_gp, 3.0f);
        spring_axis_to(&water_lily->pos.v.y, v->home_y, &v->vel_y, water_lily_spring_velocity_adjustment_scale_gp * frame_time_sq,
                       water_lily_spring_damping_gp, 3.0f);
        if (vector_distance(&water_lily->pos, &hero.motion.pos) < water_lily_hero_separation_radius_gp) {
            if (FUN_001f99c0(water_lily->pos.v.z - hero.motion.pos.f[2]) < 0.7f) {
                hero_separation_angle = fast_arc_tan(water_lily->pos.v.x - hero.motion.pos.f[0],
                                   water_lily->pos.v.y - hero.motion.pos.f[1]);
                water_lily->pos.v.x = fast_cos(hero_separation_angle) * water_lily_hero_separation_radius_gp;
                water_lily->pos.v.y = fast_sin(hero_separation_angle) * water_lily_hero_separation_radius_gp;
                water_lily->pos.v.x += hero.motion.pos.f[0];
                water_lily->pos.v.y += hero.motion.pos.f[1];
            }
        }
        FUN_L00_00258490(water_lily, &collision_adjusted_position, &water_lily->pos, 3, 0.1f, water_lily_collision_radius_gp, 600.0f, 1.5707964f);
        water_lily->pos.v.x = collision_adjusted_position.v.x;
        water_lily->pos.v.y = collision_adjusted_position.v.y;
        water_lily->pos.v.z = old.v.z;
        if (FUN_001f9b48(&old, &water_lily->pos) > water_lily_max_horizontal_speed_gp * frame_time_macro) {
            fast_vec_sub(&vector_scratch, &water_lily->pos, &old);
            step.q = vector_scratch.q;
            fast_vec_normalize(&step, &step, water_lily_max_horizontal_speed_gp * frame_time_macro);
            fast_vec_add(&vector_scratch, &old, &step);
            water_lily->pos.q = vector_scratch.q;
        }
        dz = resolve_camera_surface_height(&water_lily->pos, 0) - water_lily->pos.v.z;
        max_vertical_step = frame_time_macro * 3.0f;
        if (dz > max_vertical_step) {
            dz = max_vertical_step;
        } else if (dz < -max_vertical_step) {
            dz = -max_vertical_step;
        }
        water_lily->pos.v.z += dz;
        break;
    }
}
typedef union {
    u128 q;
    f32 f[4];
} SteamVec;

typedef struct {
    u32 c[3];
} SteamColors;

typedef struct {
    s32 active;
    s16 puff_timer;
    s16 burst_timer;
    s32 x8;
    f32 height;
} SteamVars;

typedef struct {
    u8 pad0[0x10];
    SteamVec pos;
    u8 pad20[0x58];
    SteamVars *pvars;
} SteamMoby;

typedef struct {
    u8 pad0[0x2C];
    f32 floor_z;
} SteamParticle;

SteamColors D_L01_00161C08 = {{0x2b8, 0x2b9, 0x2ba}};
extern SteamColors D_L01_00161C18;
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern s32 tick_countdown_32(void *) __asm__("FUN_001f9740");
extern f32 random_angle_radians(void) __asm__("FUN_00213308");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void spawn_debris_chunk(void *, void *, u32, s32, f32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");
extern void *mine_debris(void *, void *, u32, u32, f32, s32, u32) __asm__("FUN_L00_0026bed0");

void update_rising_particle_and_debris_emitter(SteamMoby *steam_moby) __asm__("FUN_L01_002f8c58");

void update_rising_particle_and_debris_emitter(SteamMoby *steam_moby) {
    SteamVars *steam_vars;
    SteamColors colors;
    SteamVec particle_initial_velocity;
    SteamVec pos;
    SteamVec puff;
    SteamParticle *debris_particle;
    f32 spawn_radius_or_vertical_velocity_increment;
    f32 spawn_offset_angle_radians;
    f32 size;
    s32 life;
    s32 i;

    steam_vars = steam_moby->pvars;
    fast_dec_timer_s16(&steam_vars->puff_timer);
    fast_dec_timer_s16(&steam_vars->burst_timer);
    if (steam_vars->active == 0) {
        return;
    }
    if (tick_countdown_32(steam_vars)) {
        steam_vars->puff_timer = scale_game_frames(0x78);
    }
    if (steam_vars->burst_timer == 0 && random_integer_below(100) < 5) {
        colors = D_L01_00161C08;
        pos.q = 0;
        pos.f[0] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        pos.f[1] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        particle_initial_velocity.q = pos.q;
        spawn_radius_or_vertical_velocity_increment = random_float_between(0.0f, 0.25f);
        spawn_offset_angle_radians = random_angle_radians();
        pos.f[0] = fast_cos(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        pos.f[1] = fast_sin(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &steam_moby->pos);
        pos.f[2] += steam_vars->height;
        spawn_debris_chunk(&pos, &particle_initial_velocity, colors.c[random_integer_below(3)],
                         random_int_between(0xB4, 0x12C), 0.05f, 1.0f, 1.0f, 0.75f, 0);
    }
    for (i = 0; i < 2; i++) {
        colors = D_L01_00161C18;
        puff.q = 0;
        puff.f[0] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        puff.f[1] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        puff.f[2] = random_float_between(frame_time_macro * 0.0f, frame_time_macro * 3.0f);
        particle_initial_velocity.q = puff.q;
        spawn_radius_or_vertical_velocity_increment = random_float_between(0.0f, 0.25f);
        spawn_offset_angle_radians = random_angle_radians();
        puff.f[0] = fast_cos(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        puff.f[1] = fast_sin(spawn_offset_angle_radians) * spawn_radius_or_vertical_velocity_increment;
        puff.f[2] = 0.0f;
        fast_vec_add(&puff, &puff, &steam_moby->pos);
        spawn_radius_or_vertical_velocity_increment *= frame_time_macro;
        spawn_radius_or_vertical_velocity_increment *= 8.0f;
        puff.f[2] += steam_vars->height;
        particle_initial_velocity.f[2] += spawn_radius_or_vertical_velocity_increment;
        size = random_float_between(70000.0f, 140000.0f);
        life = scale_game_frames(0xB4);
        debris_particle = mine_debris(&puff, &particle_initial_velocity, 0x0F081020, 0x00081020, size, life,
                             colors.c[random_integer_below(3)]);
        if (debris_particle != 0) {
            debris_particle->floor_z = steam_moby->pos.f[2] - 0.5f;
        }
    }
}
typedef struct {
    u8 pad0[0x80];
    s32 idx;
} Sub;

typedef struct {
    u8 pad0[0x78];
    Sub *sub;
} Obj;

extern u8 *D_L01_001600EC_u __asm__("D_L01_001600EC");
extern f32 falling_rock_camera_rotation_angle_gp __asm__("D_L01_00161C28") __attribute__((sda));
extern f32 falling_rock_camera_distance_gp __asm__("D_L01_00161C2C") __attribute__((sda));
extern f32 falling_rock_camera_height_gp __asm__("D_L01_00161C30") __attribute__((sda));
extern f32 falling_rock_camera_offset_scale_gp __asm__("D_L01_00161C34") __attribute__((sda));
extern f32 falling_rock_camera_override_companion_value_gp __asm__("D_L01_00161C38") __attribute__((sda));

extern void build_look_at_matrix(void *dst, void *vec, void *axis,
                                 float angle) __asm__("FUN_00214890");
void set_follow_camera_angle_blend_from_direction(void *, f32, f32) __asm__("FUN_L00_002e8918");
void follow_camera_set_distance(s32, f32, f32) __asm__("FUN_L00_002e8450");
void follow_camera_set_height(f32, f32) __asm__("FUN_L00_002e84b8");
void follow_camera_set_offset_scale(s32, f32, f32) __asm__("FUN_L00_002e84f0");

void apply_falling_rock_camera_overrides(Obj *falling_rock_moby) __asm__("FUN_L01_002f9000");

void apply_falling_rock_camera_overrides(Obj *falling_rock_moby) {
    u8 rotated_vector[16];
    u8 *camera_record;

    camera_record = D_L01_001600EC_u + falling_rock_moby->sub->idx * 128;
    build_look_at_matrix(rotated_vector, camera_record, camera_record + 0x20, falling_rock_camera_rotation_angle_gp);
    set_follow_camera_angle_blend_from_direction(rotated_vector, 0.13962634f, 0.0f);
    follow_camera_set_distance(0, falling_rock_camera_distance_gp, falling_rock_camera_override_companion_value_gp);
    follow_camera_set_height(falling_rock_camera_height_gp, falling_rock_camera_override_companion_value_gp);
    follow_camera_set_offset_scale(0, falling_rock_camera_offset_scale_gp, falling_rock_camera_override_companion_value_gp);
}
typedef union {
    u128 q;
    f32 f[4];
} CapVec;

typedef struct {
    u32 c[3];
} CapColors;

typedef struct {
    u8 pad0[0x20];
    s32 w20;
    s16 h24;
    u8 pad26[2];
    u8 b28;
    u8 pad29[0x15];
    s16 h3E;
    u8 pad40[0x20];
    CapVec rot;
    CapVec home;
    s32 cam;
    s16 wait;
    s16 hold;
    s16 rise_len;
    s16 rise;
    s32 music;
} CapVars;

typedef struct {
    u8 pad0[0x10];
    CapVec pos;
    u8 state;
    u8 pad21[0x57];
    CapVars *pvars;
    u8 pad7c[0x36];
    s16 id;
} CapMoby;

typedef struct {
    f32 x, y, z;
} CapVec3;

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} CapLevelState;

typedef struct {
    u8 pad0[0x160];
    f32 shake;
    u8 pad164[4];
    s32 shake_time;
} CapCamera;

extern CapLevelState D_L01_001BB6B0;
extern CapCamera D_L01_00167100;
CapColors D_L01_00161C40 = {{0x2b8, 0x2b9, 0x2ba}};
extern s32 D_0014C190[][64];
extern s32 moby_killed[] __asm__("D_L01_001BA950");
extern s32 cutscene_active __asm__("D_L01_0015F404");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
void apply_falling_rock_camera_overrides(Obj *obj) __asm__("FUN_L01_002f9000");
extern void FUN_L00_002eac18(s32);
extern int hero_set_state(int, int) __asm__("FUN_L01_0023cf98");

void falling_rock_update(CapMoby *m) __asm__("FUN_L01_002f9080");
void falling_rock_update(CapMoby *falling_rock) {
    enum {
        FALLING_ROCK_STATE_INITIALIZE = 0,
        FALLING_ROCK_STATE_WAIT_FOR_TRIGGER = 1,
        FALLING_ROCK_STATE_WAIT_BEFORE_FALL = 2,
        FALLING_ROCK_STATE_FALLING = 3,
        FALLING_ROCK_STATE_FALLEN = 4
    };
    CapVars *falling_rock_vars;
    u8 *camera_record;

    falling_rock_vars = falling_rock->pvars;
    if (falling_rock->state != 1 && !fast_dec_timer_s16(&falling_rock_vars->wait) && random_integer_below(100) < 5) {
        CapColors colors;
        CapVec debris_initial_velocity;
        CapVec pos;
        f32 debris_spawn_radius;
        f32 debris_spawn_angle;

        colors = D_L01_00161C40;
        pos.q = 0;
        pos.f[0] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        pos.f[1] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        debris_initial_velocity.q = pos.q;
        debris_spawn_radius = random_float_between(2.0f, 3.0f);
        debris_spawn_angle = random_angle_radians();
        pos.f[0] = fast_cos(debris_spawn_angle) * debris_spawn_radius;
        pos.f[1] = fast_sin(debris_spawn_angle) * debris_spawn_radius;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &falling_rock_vars->home);
        spawn_debris_chunk(&pos, &debris_initial_velocity, colors.c[random_integer_below(3)],
                         random_int_between(0xB4, 0x12C), 0.05f, 1.0f, 1.0f, 0.75f, 0);
    }
    switch (falling_rock->state) {
    case FALLING_ROCK_STATE_INITIALIZE:
        falling_rock_vars->w20 = 0;
        falling_rock_vars->h24 = 0;
        falling_rock_vars->b28 = 4;
        falling_rock_vars->h3E = 5;
        qcopy(&falling_rock_vars->home, &falling_rock->pos);
        falling_rock->state = 1;
        if (D_L01_001BB6B0.collected[falling_rock->id] != 0 ||
            (D_0014C190[current_level_index_macro][falling_rock->id >> 5] >> (falling_rock->id & 0x1F)) & 1) {
            clear_u64_value(&falling_rock_vars->rot);
            falling_rock->state = 4;
            falling_rock->pos.f[2] = falling_rock_vars->home.f[2] - 20.0f;
            falling_rock_vars->music = -1;
        }
        break;
    case FALLING_ROCK_STATE_WAIT_FOR_TRIGGER:
        if (falling_rock_vars->cam != -1) {
            camera_record = (u8 *)(falling_rock_vars->cam * 128 + (s32)D_L01_001600EC_u);
            if (hero.motion.pos.f[2] >= *(f32 *)(camera_record + 0x38) &&
                vector_distance(&hero.motion.pos, camera_record + 0x30) < 10.0f &&
                ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9)) {
                falling_rock_vars->wait = random_int_between(scale_game_frames(300), scale_game_frames(600));
                falling_rock_vars->rise = scale_game_frames(falling_rock_vars->rise_len);
                falling_rock->state = 2;
                D_L01_00167100.shake = 0.4f;
                D_L01_00167100.shake_time = scale_game_frames(30);
                allocate_voice_for_target_entry(1, 0, falling_rock);
            }
        }
        break;
    case FALLING_ROCK_STATE_WAIT_BEFORE_FALL:
        apply_falling_rock_camera_overrides((Obj *)falling_rock);
        if (fast_dec_timer_s16(&falling_rock_vars->rise)) {
            clear_u64_value(&falling_rock_vars->rot);
            falling_rock_vars->hold = scale_game_frames(0x2D);
            falling_rock->state = 3;
        }
        break;
    case FALLING_ROCK_STATE_FALLING: {
        CapVec3 old;

        apply_falling_rock_camera_overrides((Obj *)falling_rock);
        falling_rock->pos.f[2] -= 0.0f;
        qcopy(&old, &falling_rock->pos);
        falling_rock_vars->rot.f[2] -= frame_time_sq * 30.0f;
        fast_vec_add(&falling_rock->pos, &falling_rock->pos, &falling_rock_vars->rot);
        if (fast_dec_timer_s16(&falling_rock_vars->hold) && 20.0f < falling_rock_vars->home.f[2] - falling_rock->pos.f[2]) {
            D_L01_00167100.shake = 0.1f;
            D_L01_00167100.shake_time = scale_game_frames(20);
            clear_u64_value(&falling_rock_vars->rot);
            falling_rock->state = 4;
            allocate_voice_for_target_entry(0, 0, falling_rock);
            falling_rock->pos.f[2] = falling_rock_vars->home.f[2] - 20.0f;
            D_0014C190[current_level_index_macro][falling_rock->id >> 5] |= 1 << (falling_rock->id & 0x1F);
            moby_killed[falling_rock->id >> 5] |= 1 << (falling_rock->id & 0x1F);
        }
        falling_rock->pos.f[2] += 0.0f;
        break;
    }
    case FALLING_ROCK_STATE_FALLEN:
        if (falling_rock_vars->wait == 0 && falling_rock_vars->music != -1) {
            FUN_L00_002eac18(0);
            hero_set_state(HERO_STATE_IDLE, 1);
            cutscene_active = 0;
            falling_rock_vars->music = -1;
        }
        break;
    }
}
typedef struct {
    u8 pad0[0x20];
    s32 x20;
    s16 x24;
    u8 pad26[2];
    u8 x28;
    u8 pad29[0x15];
    s16 x3e;
    u8 pad40[0x20];
    u8 x60[0x40];
    f32 home_z;
    u8 home_mode;
    u8 padA5;
    s16 timer;
    f32 travel;
    f32 speed;
    f32 period;
    s32 xb4;
} LiftVars;

typedef struct {
    u8 pad0[0x10];
    u128 pos;
    u8 state;
    u8 pad21[0xF];
    u8 b30;
    u8 pad31;
    s16 h32;
    u8 pad34[0xC];
    u8 x40[0x38];
    LiftVars *pvars;
    u8 pad7c[0x40];
    u8 mode;
} LiftMoby;

extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern s32 truncate_float_to_s32_u(f32) __asm__("FUN_001fa6d0");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");

void piston_platform_update(LiftMoby *m) __asm__("FUN_L01_002f95c0");
void piston_platform_update(LiftMoby *piston_platform) {
    enum {
        PISTON_PLATFORM_MODE_WAIT_BEFORE_SUBTRACT_TRAVEL = 0,
        PISTON_PLATFORM_MODE_WAIT_BEFORE_ADD_TRAVEL = 1,
        PISTON_PLATFORM_MODE_MOVING = 2
    };
    LiftVars *piston_platform_vars;
    u128 old;
    u128 delta;
    u128 *dp;

    piston_platform_vars = piston_platform->pvars;
    qcopy(&old, &piston_platform->pos);
    fast_dec_timer_s16(&piston_platform_vars->timer);
    if (piston_platform->state == 0) {
        piston_platform_vars->x28 = 4;
        piston_platform_vars->x3e = 0xD;
        piston_platform_vars->x20 = 0;
        piston_platform_vars->x24 = 0;
        piston_platform_vars->home_z = ((f32 *)&piston_platform->pos)[2];
        piston_platform->mode = piston_platform_vars->home_mode;
        piston_platform_vars->timer = scale_game_frames(0x78);
        piston_platform->state = 1;
        piston_platform->b30 = 0xFF;
        piston_platform->h32 = 0xFF;
        piston_platform_vars->xb4 = -1;
    }
    switch (piston_platform->mode) {
    case PISTON_PLATFORM_MODE_WAIT_BEFORE_SUBTRACT_TRAVEL:
        if (piston_platform_vars->timer == 0) {
            piston_platform->mode = 2;
            piston_platform_vars->speed =
                piston_platform_vars->travel / convert_integer_to_float(scale_game_frames(truncate_float_to_s32_u(piston_platform_vars->period * 60.0f)));
            piston_platform_vars->timer = scale_game_frames(truncate_float_to_s32_u(piston_platform_vars->period * 60.0f));
            allocate_voice_for_target_entry(0, 0, piston_platform);
        }
        break;
    case PISTON_PLATFORM_MODE_WAIT_BEFORE_ADD_TRAVEL:
        if (piston_platform_vars->timer == 0) {
            piston_platform->mode = 2;
            piston_platform_vars->speed = -(piston_platform_vars->travel /
                         convert_integer_to_float(scale_game_frames(truncate_float_to_s32_u(piston_platform_vars->period * 60.0f))));
            piston_platform_vars->timer = scale_game_frames(truncate_float_to_s32_u(piston_platform_vars->period * 60.0f));
            allocate_voice_for_target_entry(1, 0, piston_platform);
        }
        break;
    case PISTON_PLATFORM_MODE_MOVING:
        ((f32 *)&piston_platform->pos)[2] -= piston_platform_vars->speed;
        if (piston_platform_vars->timer == 0) {
            if (piston_platform_vars->speed < 0.0f) {
                piston_platform->mode = 0;
            } else {
                piston_platform->mode = 1;
            }
            if (piston_platform->mode == piston_platform_vars->home_mode) {
                ((f32 *)&piston_platform->pos)[2] = piston_platform_vars->home_z;
            }
            piston_platform_vars->timer = scale_game_frames(0x78);
        }
        break;
    }
    dp = &delta;
    fast_vec_sub(dp, &piston_platform->pos, &old);
    update_entity_matrix(piston_platform_vars->x60, dp, piston_platform->x40, piston_platform->x40);
}
