/* Level 01 moby class birdbot. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/l01/hero/hero_set_state.h"

#include "rnc/overlay/level_path.h"

typedef struct {
    u32 c[6];
} SplashColors;

typedef struct {
    u8   pad0[0x10];
    u128 pos;
    u8   pad20[0xA0];
    u8   mtx[0x40];
} SplashMoby_u;

SplashColors D_L01_0020AF50 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

SplashColors D_L01_0020AF68 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

extern void spawn_effect_particle_with_target_scalar(void *, void *, u32, u32, f32, s32) __asm__("FUN_L00_0026ced0");

typedef struct {
    char pad0[0x84];
    f32  lo;
    f32  hi;
} GroundRangeMoby;

/* Sets the range to x plus or minus 0.2. */
void set_range_centered_on_value(GroundRangeMoby *range, f32 x) __asm__("FUN_L01_0026eff8");

void set_range_centered_on_value(GroundRangeMoby *range, f32 x) {
    range->lo = x - 0.2f;
    range->hi = x + 0.2f;
}

void spawn_birdbot_movement_particles(SplashMoby_u *moby, u128 *origin) __asm__("FUN_L01_002e6518");

void spawn_birdbot_movement_particles(SplashMoby_u *moby, u128 *origin) {
    Vec4         previous_moby_position;
    Vec4         world_motion_delta;
    Vec4         vector_scratch;
    Vec4         rel;
    SplashColors inner;
    SplashColors outer;
    Vec4        *bp;
    u32         *inner_color_entry;
    u32         *outer_color_entry;
    f32          size;
    s32          i;

    previous_moby_position.q = *origin;
    bp = &previous_moby_position;
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
        spawn_effect_particle_with_target_scalar(&vector_scratch,
                                                 &world_motion_delta,
                                                 *inner_color_entry,
                                                 *outer_color_entry,
                                                 size,
                                                 scale_game_frames(random_int_between(5, 0xF)));
    }
}

#include "qcopy.h"

typedef struct {
    s32 count;
    u8  pad4[0xC];
    struct {
        f32 x, y, z, w;
    } pts[1];
} PathNodes;

typedef struct {
    s32        cur;
    s8         dir;
    u8         pad5[0xB];
    PathNodes *path;
} PathFollow;

typedef struct {
    u8         pad0[0x38];
    s32        w38;
    u8         pad3C[0x114];
    PathFollow follow;
    u8         pad164[0x3C];
    f32        radius;
    u8         pad1A4[8];
    s16        ordered;
    s16        stops_only;
    u8         pad1B0[0x32];
    s16        h1E2;
} PathPickVars;

typedef struct {
    u8            pad0[0x10];
    Vec4          pos;
    u8            pad20[0x58];
    PathPickVars *vars;
} PathPickMoby;

typedef struct {
    Vec4  pos;
    Vec4  v10;
    Vec4  v20;
    Vec4  v30;
    void *moby;
    s32   kind;
} PathPickTarget;

extern void find_point_ahead(void *, void *, f32) __asm__("FUN_L01_00274b78");

/* Picks the path node the follower should head for: an ordered path just steps to the next stop; otherwise the
 * nearest node and its two neighbours are scored (distance, or heading change for slow movers) and the best one
 * is written to *idx. Returns nonzero when the pick differs from the nearest node. */
int select_birdbot_path_node(void *path_follower_entity,
                             int  *selected_path_node_index) __asm__("FUN_L01_002e6790");

int select_birdbot_path_node(void *path_follower_entity, int *selected_path_node_index) {
    PathPickMoby  *path_follower_moby = path_follower_entity;
    PathPickTarget target;
    Vec4           pt;
    PathPickVars  *path_pick_vars;
    PathFollow    *f;
    s32            fast;
    s32            preceding_node_index;
    s32            successor_node_index;
    s32            best;
    s32            last;
    s32            i;
    f32            candidate_node_distance;
    f32            node_target_heading_difference;
    f32            best_d;
    f32            best_a;
    f32            target_search_radius;
    f32            node_follower_distance;

    path_pick_vars = path_follower_moby->vars;
    fast = 0;
    f = &path_pick_vars->follow;
    if (path_pick_vars->w38 != 0 || path_pick_vars->h1E2 != 0) {
        fast = 1;
    }
    target_search_radius = fast ? path_pick_vars->radius * 2.0f : path_pick_vars->radius;
    preceding_node_index = 0;
    successor_node_index = 0;
    find_point_ahead(path_follower_moby, &target, target_search_radius);
    best_d = 10000.0f;
    best_a = 0.0f;
    best = 0;
    if (path_pick_vars->ordered != 0) {
        do {
            if (*selected_path_node_index < f->path->count - 1) {
                *selected_path_node_index =
                    (*selected_path_node_index + f->path->count + f->dir) % f->path->count;
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
            node_follower_distance = vector_distance_xy(&f->path->pts[i], &path_follower_moby->pos);
            if (node_follower_distance < best_d) {
                preceding_node_index = last;
                best_d = node_follower_distance;
                best = i;
            } else if (last == best) {
                successor_node_index = i;
            }
            last = i;
        }
    }
    i = best;
    if (i == f->path->count - 1) {
        successor_node_index = 0;
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
        if (*selected_path_node_index == preceding_node_index || *selected_path_node_index == i ||
            *selected_path_node_index == successor_node_index) {
            if (fast) {
                qcopy(&pt, &f->path->pts[*selected_path_node_index]);
                candidate_node_distance = fast_vec_dist(&target, &pt);
                if ((path_pick_vars->stops_only == 0 || pt.f[3] == 1.0f) &&
                    candidate_node_distance < best_d) {
                    best = *selected_path_node_index;
                    best_d = candidate_node_distance;
                }
            } else {
                qcopy(&pt, &f->path->pts[*selected_path_node_index]);
                node_target_heading_difference = fast_difference_between_rotations(
                    fast_arc_tan(pt.f[0] - path_follower_moby->pos.f[0],
                                 pt.f[1] - path_follower_moby->pos.f[1]),
                    fast_arc_tan(target.pos.f[0] - path_follower_moby->pos.f[0],
                                 target.pos.f[1] - path_follower_moby->pos.f[1]));
                candidate_node_distance = fast_vec_dist(&path_follower_moby->pos, &pt);
                if (path_pick_vars->stops_only == 0 || pt.f[3] == 1.0f) {
                    if (node_target_heading_difference < 1.5707964f) {
                        if (best_a < node_target_heading_difference) {
                            best = *selected_path_node_index;
                            best_d = candidate_node_distance;
                            best_a = node_target_heading_difference;
                        }
                    } else if ((candidate_node_distance >= 12.0f &&
                                (candidate_node_distance < best_d || best_d < 12.0f)) ||
                               (candidate_node_distance < 12.0f &&
                                best_d < candidate_node_distance)) {
                        best = *selected_path_node_index;
                        best_d = candidate_node_distance;
                        best_a = node_target_heading_difference;
                    }
                }
            }
        }
        *selected_path_node_index =
            (*selected_path_node_index + f->path->count + f->dir) % f->path->count;
        if (*selected_path_node_index == 0) {
            *selected_path_node_index = best;
            break;
        }
    }
    /* i is reused for the result so it outlives best (keeps retail's copy of the nearest node) */
    i = best != i;
    return i;
}

/* Birdbot update: follows its path, turns on and shoots at the hero, flees when hurt, and blows up when it dies or leaves the level. */
/* Ported from rac1-decomp (src/overlays/l01_novalis/vendor_002BA898.c: func_L01_002E7FC8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    u128 pos;
    char pad20[0xA0];
    char mtx[0x40];
} SplashMoby;

typedef struct {
    Vec4  pos;
    Vec4  v10;
    Vec4  v20;
    Vec4  v30;
    void *moby;
    int   kind;
} PeTarget;

typedef struct {
    int         idx;
    signed char dir;
    char        pad5[0xB];
    LevelPath  *path;
    int         path_id;
} PeCursor;

typedef struct {
    char           pad0[0x20];
    float          f20;
    short          h24;
    short          h26;
    unsigned char  b28;
    unsigned char  b29;
    char           pad2A[0xE];
    int            w38;
    char           pad3C[4];
    Vec4           v40;
    char           pad50[8];
    unsigned char  b58;
    char           pad59;
    unsigned char  b5A;
    char           pad5B[5];
    char           x60[7];
    unsigned char  b67;
    char           pad68[8];
    char           x70[0x10];
    float          f80;
    float          f84;
    float          f88;
    float          f8C;
    int            w90;
    int            w94;
    float          f98;
    char           pad9C[0x11];
    unsigned char  bAD;
    char           padAE[0x12];
    float          fC0;
    float          fC4;
    char           padC8[8];
    char           xD0[8];
    float          fD8;
    float          fDC;
    char           padE0[8];
    float          fE8;
    float          fEC;
    char           padF0[4];
    float          fF4;
    char           padF8[0x14];
    float          f10C;
    float          f110;
    float          f114;
    char           pad118[8];
    PeCursor       side;
    char           pad138[0x18];
    PeCursor       main;
    char           pad168[0x18];
    Vec4           v180;
    int            w190;
    float          f194;
    float          f198;
    char           pad19C[4];
    float          f1A0;
    short          h1A4;
    short          h1A6;
    float          f1A8;
    short          h1AC;
    short          h1AE;
    char           pad1B0[4];
    int            w1B4;
    float          f1B8;
    float          f1BC;
    int            w1C0;
    int            w1C4;
    int            w1C8;
    float          f1CC;
    Vec4           v1D0;
    unsigned short h1E0;
    short          h1E2;
    int            w1E4;
    int            w1E8;
    short          h1EC;
    short          h1EE;
    char           pad1F0[0x10];
    unsigned char  x200[1];
} PeVars;

typedef struct PeMoby {
    char           pad0[0x10];
    Vec4           pos;
    unsigned char  state;
    char           pad21[3];
    char          *w24;
    char           pad28[9];
    unsigned char  b31;
    unsigned short h32;
    unsigned short h34;
    char           pad36[2];
    long           l38;
    Vec4           rot;
    char           pad50[2];
    unsigned char  b52;
    unsigned char  b53;
    char           pad54[4];
    float          f58;
    char           pad5C[0x14];
    unsigned char  b70;
    char           pad71[7];
    PeVars        *vars;
    char           pad7C[0x18];
    int            w94;
    char           pad98[0xC];
    unsigned char  bA4;
    char           padA5;
    short          hA6;
    char           padA8[0xA];
    short          hB2;
} PeMoby;

typedef struct {
    char           pad0[0x10];
    Vec4           pos;
    struct PeMoby *src;
} PeHit;

typedef struct {
    char           pad0[0x80];
    Vec4           pos;
    char           pad90[0x1000];
    struct PeMoby *f1090;
    char           pad1094[0xFEC];
    struct PeMoby *f2080;
} PeHero;

#include "rnc/overlay/cuboid.h"

#include "rnc/overlay/l01/mobys/birdbot_projectile.h"

extern char  birdbot_missing_path_format[] __asm__("D_L01_0020AF80");
extern float D_L01_0015F580[] MACRO_ADDR;
extern float FUN_L04_001f1978(float);
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float compute_interpolated_record_value(void *);
extern float probe_ground_height(void *, int, float);
extern int   cutscene_active __asm__("D_L01_0015F404");
extern int   DebugPrint();
extern int   collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int   collision_hit_material(void) __asm__("FUN_001f0b58");
extern int   FUN_L00_0025a478_u(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int   select_birdbot_path_node(void *, int *) __asm__("FUN_L01_002e6790");
extern int   scale_game_frames(int);
extern short birdbot_projectile_speed_gp __asm__("D_L01_00161950") __attribute__((sda));
extern short birdbot_path_speed_scale_gp __asm__("D_L01_00161954") __attribute__((sda));
extern short birdbot_path_entry_animation_speed_gp __asm__("D_L01_0016195C") __attribute__((sda));
extern void *FUN_L00_00263fd8_u(char *,
                                int,
                                float *,
                                void *,
                                int,
                                int,
                                float,
                                float *,
                                float *,
                                float *) __asm__("FUN_L00_00263fd8");
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_00258278_cf(
     char *moby, float *vel, float target, float k, float d, float max) __asm__("FUN_L00_00258278");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void FUN_L00_0025c558_u(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00263ac8_u(float, int, int, unsigned char *) __asm__("FUN_L00_00263ac8");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");
extern void FUN_L01_0026d930_u(char *p) __asm__("FUN_L01_0026d930");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

void birdbot_update(PeMoby *m) __asm__("FUN_L01_002e6bf0");

/* Moby class 459 */
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
    PeVars  *birdbot_vars;
    Vec4     birdbot_position_before_update;
    PeTarget selected_combat_target;
    Vec4     va;
    Vec4     vb;
    Vec4     vc;
    int      idx;
    int      hits;
    float    dmg;
    float    knockback_heading;
    int      flag;
    int      path_reposition_requested;
    float    frame;
    PeHit   *hit;
    int      kind;
    float    ground;
    float    target_search_radius;

    flag = 0;
    path_reposition_requested = 0;
    birdbot_vars = birdbot_moby->vars;
    idx = birdbot_vars->main.idx;
    frame = compute_interpolated_record_value(birdbot_moby);
    if (birdbot_vars->w38 != 0 || birdbot_vars->h1E2 != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8_u(2.5f, (int)birdbot_moby, 2, birdbot_vars->x200);
    qcopy(&birdbot_position_before_update, &birdbot_moby->pos);
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
                knockback_heading = fast_arc_tan(birdbot_moby->pos.f[0] - hit->src->pos.f[0],
                                                 birdbot_moby->pos.f[1] - hit->src->pos.f[1]);
            } else {
                knockback_heading = fast_arc_tan(birdbot_moby->pos.f[0] - player->motion.pos.f[0],
                                                 birdbot_moby->pos.f[1] - player->motion.pos.f[1]);
            }
        } else {
            struct Hero *player = &hero;

            knockback_heading = fast_arc_tan(birdbot_moby->pos.f[0] - player->motion.pos.f[0],
                                             birdbot_moby->pos.f[1] - player->motion.pos.f[1]);
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
            trooper_knockback_arc(&va, &knockback_heading, &birdbot_vars->f88, &birdbot_vars->f8C);
            FUN_L00_0025c558_u(
                knockback_heading, birdbot_moby, birdbot_vars->x70, 9, scale_game_frames(2), 0);
            birdbot_vars->b67 = 0xB4;
            birdbot_moby->state = 0xE;
            birdbot_moby->h34 &= 0xEFFF;
            trooper_anim_update(birdbot_moby, (short *)birdbot_vars->x60);
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
            trooper_knockback_arc(&va, &knockback_heading, &birdbot_vars->f88, &birdbot_vars->f8C);
            FUN_L00_0025c558_u(knockback_heading, birdbot_moby, birdbot_vars->x70, 0xC, 6, 0);
            if (birdbot_moby->b53 != 0xB) {
                blend_moby_animation(birdbot_moby, 0xB, 0, scale_game_frames(6));
            }
            birdbot_moby->state = 0xC;
            trooper_anim_update(birdbot_moby, (short *)birdbot_vars->x60);
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
            trooper_knockback_arc(&va, &knockback_heading, &birdbot_vars->f88, &birdbot_vars->f8C);
            FUN_L00_0025c558_u(knockback_heading, birdbot_moby, birdbot_vars->x70, 0xE, 6, 0);
            birdbot_moby->state = 0xC;
            trooper_anim_update(birdbot_moby, (short *)birdbot_vars->x60);
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
        int       i;
        float     fall;
        float     turn;

        c = &birdbot_vars->main;
        if (birdbot_vars->main.path_id == -1) {
            DebugPrint(birdbot_missing_path_format, birdbot_moby->hA6, birdbot_moby->hB2);
            mark_moby_for_removal(birdbot_moby);
            return;
        }
        birdbot_vars->main.path = l01_paths[birdbot_vars->main.path_id];
        c->idx = 0;
        if (birdbot_vars->side.path_id >= 0) {
            birdbot_vars->side.path = l01_paths[birdbot_vars->side.path_id];
            birdbot_vars->side.idx = 0;
            qcopy(&birdbot_moby->pos, &birdbot_vars->side.path->points[0]);
        } else {
            qcopy(&birdbot_moby->pos, &birdbot_vars->main.path->points[0]);
            if (birdbot_vars->w1B4 == 0) {
                birdbot_vars->side.path = 0;
                birdbot_moby->pos.f[2] += 20.0f;
            }
        }
        birdbot_vars->h1AE = 0;
        for (i = 0; i < c->path->count; i++) {
            if (c->path->points[i].f[3] == 1.0f) {
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
        fall = *(float *)&birdbot_path_speed_scale_gp * frame_time;
        birdbot_vars->f10C = 0.03f;
        birdbot_vars->f110 = 0.3f;
        birdbot_vars->f114 = 0.25f;
        birdbot_vars->fEC = turn;
        birdbot_vars->fF4 = fall;
        birdbot_vars->fDC = 2.0f;
        birdbot_vars->fD8 = 2.0f;
        birdbot_vars->f20 = 2.0f;
        birdbot_vars->h24 = truncate_float_to_s32(birdbot_vars->f20);
        birdbot_vars->b29 = 0;
        birdbot_vars->b28 = 1;
        clear_u64_value(&birdbot_vars->v40);
        birdbot_vars->w1C8 = -1;
        if (birdbot_moby->b53 != 0) {
            blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(5));
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
        float     d;
        float     a;

        c = &birdbot_vars->main;
        a = fast_arc_tan(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                         selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1]);
        FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.02f, 0.3f, 0.1f);
        fast_dec_timer(&birdbot_vars->w190);
        d = vector_distance_xy(&birdbot_moby->pos, &selected_combat_target);
        if ((flag ? d < birdbot_vars->f1A0 * 2.0f : d < birdbot_vars->f1A0) &&
            absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) < 2.0f &&
            birdbot_moby->b31 != 0 && (!flag || path_reposition_requested == 0) &&
            birdbot_vars->w190 == 0 &&
            fast_difference_between_rotations(
                birdbot_moby->rot.f[2],
                fast_arc_tan(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
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
                    blend_moby_animation(birdbot_moby, 4, 0, scale_game_frames(6));
                }
                birdbot_vars->w190 = 0;
            } else {
                birdbot_vars->w190 = scale_game_frames(0x3C);
            }
        } else if (flag ||
                   (vector_distance_xy(&birdbot_moby->pos, &selected_combat_target) < 4.0f &&
                    absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) <
                        6.0f)) {
            if (path_reposition_requested != 0 &&
                fast_vec_dist(&birdbot_moby->pos, (Vec4 *)c->path + (idx + 1)) > 0.25f) {
                if (birdbot_vars->h1AC == 0 || c->idx < c->path->count - 1) {
                    int   i;
                    float f;

                    if (birdbot_moby->b53 != 8) {
                        blend_moby_animation(birdbot_moby, 8, 0, scale_game_frames(10));
                    }
                    i = idx;
                    f = *(float *)&birdbot_path_entry_animation_speed_gp;
                    c->idx = i;
                    birdbot_moby->f58 = f;
                    birdbot_moby->state = 9;
                }
            }
        }
        if (birdbot_vars->w1C8 == 0 && birdbot_vars->w1C4 != -1) {
            jet_end_mount(0);
            l01_hero_set_state(HERO_STATE_IDLE, 1);
            cutscene_active = 0;
            birdbot_vars->w1C4 = -1;
        }
        break;
    }
    case BIRDBOT_STATE_WAIT_FOR_TRIGGER:
        if (vector_distance_xy(&birdbot_moby->pos, &selected_combat_target) < birdbot_vars->f1A8 ||
            is_point_inside_clip_volume(&hero.motion.pos, birdbot_vars->w1C0) != 0 ||
            birdbot_vars->w1E8 != 0) {
            if (birdbot_vars->w1C4 != -1) {
                LevelCuboid *p;

                l01_hero_set_state(0x1F, 0);
                p = &l01_cuboids[birdbot_vars->w1C4];
                jet_mount(&p->pos, &p->rot, 0, 0, 0);
                cutscene_active = 1;
                birdbot_vars->w1C8 = scale_game_frames(0x168);
            }
            birdbot_moby->w94 = *(int *)(birdbot_moby->w24 + 0x10);
            birdbot_moby->h34 &= 0xFFBE;
            if (birdbot_moby->b53 != 8) {
                blend_moby_animation(birdbot_moby, 8, 0, scale_game_frames(10));
            }
            birdbot_moby->state = 3;
        }
        break;
    case BIRDBOT_STATE_DEPLOY:
        if (birdbot_vars->side.path != 0) {
            PeCursor  *side_path_cursor;
            LevelPath *path;
            int        side_path_next_node_index;
            float      a;

            side_path_cursor = &birdbot_vars->side;
            side_path_next_node_index =
                (side_path_cursor->idx + side_path_cursor->path->count + side_path_cursor->dir) %
                side_path_cursor->path->count;
            a = fast_arc_tan(side_path_cursor->path->points[side_path_next_node_index].f[0] -
                                 birdbot_moby->pos.f[0],
                             side_path_cursor->path->points[side_path_next_node_index].f[1] -
                                 birdbot_moby->pos.f[1]);
            FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.01f, 0.3f, 0.1f);
            path = side_path_cursor->path;
            qcopy(&vb, &path->points[side_path_next_node_index]);
            if (side_path_cursor->idx == path->count - 1) {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            } else {
                fast_vec_sub(&va, &vb, &birdbot_moby->pos);
                fast_vec_normalize(&va, &va, frame_time * 5.0f);
                fast_vec_add(&birdbot_moby->pos, &birdbot_moby->pos, &va);
                if (fast_vec_dist(&vb, &birdbot_moby->pos) < 0.5f) {
                    side_path_cursor->idx = side_path_next_node_index;
                }
            }
        } else if (birdbot_vars->w1E4 != -1) {
            LevelCuboid *p;
            float        a;
            float        deployment_step_length;
            float        z;

            p = (LevelCuboid *)((birdbot_vars->w1E4 << 7) + (int)l01_cuboids);
            a = fast_arc_tan(p->pos.x - birdbot_moby->pos.f[0], p->pos.y - birdbot_moby->pos.f[1]);
            FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.01f, 0.3f, 0.1f);
            deployment_step_length = fast_vec_length(&birdbot_vars->v1D0);
            z = birdbot_vars->v1D0.f[2];
            fast_vec_normalize(
                &birdbot_vars->v1D0, &birdbot_vars->v1D0, deployment_step_length * 0.95f);
            fast_vec_sub(&vb, &l01_cuboids[birdbot_vars->w1E4].pos, &birdbot_moby->pos);
            fast_vec_normalize(&vb, &vb, deployment_step_length * 0.05f);
            fast_vec_add(&birdbot_vars->v1D0, &birdbot_vars->v1D0, &vb);
            if (frame_time * 10.0f < deployment_step_length) {
                fast_vec_normalize(&vb, &birdbot_vars->v1D0, -(frame_time_sq * 10.0f));
                fast_vec_add(&birdbot_vars->v1D0, &birdbot_vars->v1D0, &vb);
            } else if (deployment_step_length < frame_time * 5.0f) {
                fast_vec_normalize(&vb, &birdbot_vars->v1D0, frame_time_sq * 5.0f);
                fast_vec_add(&birdbot_vars->v1D0, &birdbot_vars->v1D0, &vb);
            }
            if (fast_dec_timer_s16(&birdbot_vars->h1EC) == 0) {
                birdbot_vars->v1D0.f[2] = z;
            }
            fast_vec_add(&birdbot_moby->pos, &birdbot_moby->pos, &birdbot_vars->v1D0);
            if (fast_vec_dist(&birdbot_moby->pos, &l01_cuboids[birdbot_vars->w1E4].pos) <
                frame_time * 10.0f) {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            }
        } else {
            clear_u64_value(&va);
            va.f[2] = probe_ground_height(&birdbot_moby->pos, 0, 0.5f) - birdbot_moby->pos.f[2];
            if (va.f[2] < -(frame_time * 10.0f)) {
                va.f[2] = -(frame_time * 10.0f);
            }
            fast_vec_add(&birdbot_moby->pos, &birdbot_moby->pos, &va);
            if (fast_vec_length(&va) < frame_time) {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
            }
        }
        spawn_birdbot_movement_particles((SplashMoby *)birdbot_moby,
                                         (u128 *)&birdbot_position_before_update);
        break;
    case BIRDBOT_STATE_FIRE_BURST:
        if (birdbot_vars->f1CC < 14.0f && frame >= 14.0f) {
            float a;
            float yaw;
            float pitch;

            a = birdbot_moby->rot.f[2];
            moby_point_position(birdbot_moby, 0, &va);
            qcopy(&birdbot_vars->v180, &va);
            if (fast_difference_between_rotations(
                    fast_arc_tan(selected_combat_target.pos.f[0] - va.f[0],
                                 selected_combat_target.pos.f[1] - va.f[1]),
                    a) < 0.08726646f) {
                a = fast_arc_tan(selected_combat_target.pos.f[0] - va.f[0],
                                 selected_combat_target.pos.f[1] - va.f[1]);
            }
            birdbot_vars->v180.f[0] += fast_cos(a) * birdbot_vars->f198;
            birdbot_vars->v180.f[1] += fast_sin(a) * birdbot_vars->f198;
            fast_vec_sub(&vb, &birdbot_vars->v180, &va);
            fast_vec_normalize(&vb, &vb, *(float *)&birdbot_projectile_speed_gp * frame_time);
            vb.f[2] += 0.0f;
            *(u128 *)&vc = *(u128 *)&birdbot_moby->pos;
            vc.f[2] = va.f[2];
            if (collision_line(&vc, &va, 0, birdbot_moby, 0) == 0) {
                yaw = fast_arc_tan(vb.f[0], vb.f[1]);
                pitch = fast_arc_tan(vector_length_xy(&vb), vb.f[2]);
                spawn_birdbot_projectile(birdbot_moby,
                                         &va,
                                         &vb,
                                         yaw,
                                         pitch,
                                         truncate_float_to_s32(FUN_L04_001f1978(10.0f)));
                birdbot_vars->h1A4++;
            } else {
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(5));
                }
                birdbot_moby->state = 1;
                break;
            }
        }
        if (frame < 2.0f) {
            if (birdbot_vars->h1A4 >= 4 ||
                vector_distance_xy(&birdbot_moby->pos, &selected_combat_target) >
                    (flag ? birdbot_vars->f1A0 * 2.0f : birdbot_vars->f1A0) ||
                absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) > 6.0f) {
                if (birdbot_moby->b53 != 5) {
                    blend_moby_animation(birdbot_moby, 5, 5, scale_game_frames(10));
                }
                birdbot_moby->f58 = 0.5f;
                birdbot_moby->state = 7;
            }
        }
        break;
    case BIRDBOT_STATE_RECOVER_FROM_BURST: {
        float a;

        a = fast_arc_tan(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                         selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1]);
        FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.02f, 0.3f, 0.1f);
        if (birdbot_moby->b53 == 0) {
            if (fast_dec_timer(&birdbot_vars->w190) != 0) {
                if (birdbot_moby->b53 != 5) {
                    blend_moby_animation(birdbot_moby, 5, 0, scale_game_frames(0xF));
                }
            }
        } else if (birdbot_moby->b70 & 2) {
            birdbot_moby->f58 = 1.0f;
            if ((flag || (vector_distance_xy(&birdbot_moby->pos, &selected_combat_target) < 8.0f &&
                          absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) <
                              6.0f)) &&
                path_reposition_requested != 0 &&
                fast_vec_dist(&birdbot_moby->pos, (Vec4 *)birdbot_vars->main.path + (idx + 1)) >
                    0.25f) {
                birdbot_vars->main.idx = idx;
                if (birdbot_moby->b53 != 8) {
                    blend_moby_animation(birdbot_moby, 8, 0, scale_game_frames(10));
                }
                birdbot_moby->f58 = *(float *)&birdbot_path_entry_animation_speed_gp;
                birdbot_moby->state = 9;
            } else {
                float d;

                d = vector_distance_xy(&birdbot_moby->pos, &selected_combat_target);
                if ((flag ? d < birdbot_vars->f1A0 * 2.0f : d < birdbot_vars->f1A0) &&
                    absolute_float(birdbot_moby->pos.f[2] - selected_combat_target.pos.f[2]) <
                        2.0f &&
                    birdbot_moby->b31 != 0 && (!flag || path_reposition_requested == 0) &&
                    birdbot_vars->w190 == 0 &&
                    fast_difference_between_rotations(
                        birdbot_moby->rot.f[2],
                        fast_arc_tan(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
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
                            blend_moby_animation(birdbot_moby, 4, 0, scale_game_frames(6));
                        }
                        birdbot_vars->w190 = 0;
                        break;
                    }
                    birdbot_vars->w190 = scale_game_frames(0x3C);
                }
                if (birdbot_moby->b53 != 0) {
                    blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(5));
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
        spawn_birdbot_movement_particles((SplashMoby *)birdbot_moby,
                                         (u128 *)&birdbot_position_before_update);
        break;
    case BIRDBOT_STATE_MOVE_TO_PATH_NODE: {
        PeCursor *c;
        float     path_node_height_offset;

        c = &birdbot_vars->main;
        if (birdbot_vars->h1AE != 0) {
            int   i;
            int   best;
            float bd;

            bd = 1e11f;
            best = 0;
            for (i = 0; i < c->path->count; i++) {
                float d;

                d = fast_vec_dist(&c->path->points[i], &birdbot_moby->pos);
                if (d < bd) {
                    bd = d;
                    best = i;
                }
            }
            birdbot_vars->h1A6 = c->idx < best ? best - 1 : best < c->idx ? best + 1 : c->idx;
        } else {
            birdbot_vars->h1A6 = c->idx;
        }
        qcopy(&va, &c->path->points[birdbot_vars->h1A6]);
        path_node_height_offset = 0.8f;
        va.f[2] += path_node_height_offset;
        FUN_L00_00258278_cf(
            (char *)birdbot_moby,
            &birdbot_vars->f194,
            fast_arc_tan(va.f[0] - birdbot_moby->pos.f[0], va.f[1] - birdbot_moby->pos.f[1]),
            frame_time_sq * 6.2831855f,
            frame_time_sq * 3.1415927f,
            frame_time * 6.2831855f);
        if (fast_difference_between_rotations(
                birdbot_moby->rot.f[2],
                fast_arc_tan(va.f[0] - birdbot_moby->pos.f[0], va.f[1] - birdbot_moby->pos.f[1])) <
            0.7853982f) {
            if (birdbot_vars->f1B8 < *(float *)&birdbot_path_speed_scale_gp * frame_time) {
                birdbot_vars->f1B8 =
                    birdbot_vars->f1B8 + *(float *)&birdbot_path_speed_scale_gp * frame_time_sq;
            }
            build_spherical_offset(&vb,
                                   birdbot_vars->f1B8,
                                   birdbot_moby->rot.f[2],
                                   fast_arc_tan(vector_distance_xy(&birdbot_moby->pos, &va),
                                                va.f[2] - birdbot_moby->pos.f[2]));
            fast_vec_add(&birdbot_moby->pos, &birdbot_moby->pos, &vb);
            birdbot_moby->rot.f[1] =
                spring_turn(&birdbot_vars->f1BC,
                            birdbot_moby->rot.f[1],
                            birdbot_vars->f1B8 /
                                (*(float *)&birdbot_path_speed_scale_gp * frame_time) * 0.5235988f,
                            frame_time_sq * 6.2831855f,
                            frame_time_sq * 3.1415927f,
                            frame_time * 4.712389f);
        }
        qcopy(&va, &c->path->points[c->idx]);
        va.f[2] += path_node_height_offset;
        if (fast_vec_dist(&birdbot_moby->pos, &va) < 0.25f) {
            birdbot_moby->state = 0xB;
        }
        spawn_birdbot_movement_particles((SplashMoby *)birdbot_moby,
                                         (u128 *)&birdbot_position_before_update);
        break;
    }
    case BIRDBOT_STATE_BRAKE_AND_FACE_TARGET:
        if (birdbot_vars->f1B8 > 0.0f && birdbot_vars->h1EE == 0) {
            birdbot_vars->f1B8 -= *(float *)&birdbot_path_speed_scale_gp * 3.0f * frame_time_sq;
            va.f[0] = fast_cos(birdbot_moby->rot.f[2]) * birdbot_vars->f1B8;
            va.f[1] = fast_sin(birdbot_moby->rot.f[2]) * birdbot_vars->f1B8;
            va.f[2] = 0.0f;
            fast_vec_add(&birdbot_moby->pos, &birdbot_moby->pos, &va);
            birdbot_moby->rot.f[1] =
                spring_turn(&birdbot_vars->f1BC,
                            birdbot_moby->rot.f[1],
                            birdbot_vars->f1B8 /
                                (*(float *)&birdbot_path_speed_scale_gp * frame_time) * 0.5235988f,
                            frame_time_sq * 6.2831855f,
                            frame_time_sq * 3.1415927f,
                            frame_time * 6.2831855f);
        } else {
            float a;
            float k;

            a = fast_arc_tan(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                             selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1]);
            FUN_L00_00258278_cf((char *)birdbot_moby, &birdbot_vars->f194, a, 0.02f, 0.3f, 0.1f);
            k = *(float *)&birdbot_path_speed_scale_gp * frame_time;
            birdbot_vars->f1B8 = 0.0f;
            birdbot_moby->rot.f[1] = spring_turn(&birdbot_vars->f1BC,
                                                 birdbot_moby->rot.f[1],
                                                 birdbot_vars->f1B8 / k * 0.5235988f,
                                                 frame_time_sq * 6.2831855f,
                                                 frame_time_sq * 3.1415927f,
                                                 frame_time * 6.2831855f);
        }
        if (birdbot_moby->b53 != 0xA && birdbot_vars->f1B8 <= 0.0f &&
            fast_difference_between_rotations(
                birdbot_moby->rot.f[2],
                fast_arc_tan(selected_combat_target.pos.f[0] - birdbot_moby->pos.f[0],
                             selected_combat_target.pos.f[1] - birdbot_moby->pos.f[1])) <
                0.034906585f) {
            birdbot_vars->f1B8 = 0.0f;
            if (birdbot_moby->b53 != 0xA) {
                blend_moby_animation(birdbot_moby, 0xA, 0, scale_game_frames(3));
            }
        } else if (birdbot_moby->b52 == 0xA && (birdbot_moby->b70 & 2)) {
            if (birdbot_moby->b53 != 0) {
                blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(6));
            }
            birdbot_moby->state = 1;
        }
        spawn_birdbot_movement_particles((SplashMoby *)birdbot_moby,
                                         (u128 *)&birdbot_position_before_update);
        break;
    case BIRDBOT_STATE_NONFATAL_KNOCKBACK:
        if (step_moby_motion(birdbot_moby, birdbot_vars->x70) & 0x40) {
            if (fast_vec_dist(&birdbot_moby->pos, (Vec4 *)birdbot_vars->main.path + (idx + 1)) >
                0.25f) {
                int   i;
                float f;

                if (birdbot_moby->b53 != 8) {
                    blend_moby_animation(birdbot_moby, 8, 0, scale_game_frames(10));
                }
                i = idx;
                f = *(float *)&birdbot_path_entry_animation_speed_gp;
                birdbot_vars->main.idx = i;
                birdbot_moby->f58 = f;
                birdbot_moby->state = 9;
                return;
            }
            if (birdbot_moby->b53 != 0) {
                blend_moby_animation(birdbot_moby, 0, 0, scale_game_frames(6));
            }
            birdbot_moby->state = 1;
        }
        if (birdbot_moby->pos.f[0] < 2.0f || birdbot_moby->pos.f[0] > 1021.0f ||
            birdbot_moby->pos.f[1] < 2.0f || birdbot_moby->pos.f[1] > 1021.0f ||
            birdbot_moby->pos.f[2] < 2.0f || birdbot_moby->pos.f[2] > 1021.0f) {
            spawn_explosion(birdbot_moby,
                            &birdbot_vars->v40,
                            &birdbot_moby->pos,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            4,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00257470(birdbot_moby, 0, -1);
            mark_moby_for_removal(birdbot_moby);
            return;
        }
        break;
    case BIRDBOT_STATE_FATAL_KNOCKBACK:
        birdbot_moby->h34 &= 0xEFFF;
        if ((step_moby_motion(birdbot_moby, birdbot_vars->x70) & 6) ||
            birdbot_moby->pos.f[0] < 2.0f || birdbot_moby->pos.f[0] > 1021.0f ||
            birdbot_moby->pos.f[1] < 2.0f || birdbot_moby->pos.f[1] > 1021.0f ||
            birdbot_moby->pos.f[2] < 2.0f || birdbot_moby->pos.f[2] > 1021.0f) {
            spawn_explosion(birdbot_moby,
                            &birdbot_vars->v40,
                            &birdbot_moby->pos,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            4,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00263fd8_u((char *)birdbot_moby,
                               0x6C8,
                               birdbot_moby->pos.f,
                               &birdbot_moby->rot,
                               0,
                               0,
                               0.0f,
                               D_L01_0015F580,
                               D_L01_0015F580,
                               D_L01_0015F580);
            FUN_L00_00263fd8_u((char *)birdbot_moby,
                               0x6C9,
                               birdbot_moby->pos.f,
                               &birdbot_moby->rot,
                               0,
                               0,
                               0.0f,
                               D_L01_0015F580,
                               D_L01_0015F580,
                               D_L01_0015F580);
            FUN_L00_00263fd8_u((char *)birdbot_moby,
                               0x6CA,
                               birdbot_moby->pos.f,
                               &birdbot_moby->rot,
                               0,
                               0,
                               0.0f,
                               D_L01_0015F580,
                               D_L01_0015F580,
                               D_L01_0015F580);
            FUN_L00_00263fd8_u((char *)birdbot_moby,
                               0x6EA,
                               birdbot_moby->pos.f,
                               &birdbot_moby->rot,
                               0,
                               0,
                               0.0f,
                               D_L01_0015F580,
                               D_L01_0015F580,
                               D_L01_0015F580);
            FUN_L00_00257470(birdbot_moby, 0, -1);
            mark_moby_for_removal(birdbot_moby);
            return;
        }
        break;
    }
    trooper_anim_finish(birdbot_moby, birdbot_vars->x60);
    birdbot_vars->f1CC = frame;
    if (birdbot_moby->state == 1 || birdbot_moby->state == 5 || birdbot_moby->state == 6 ||
        birdbot_moby->state == 7 || birdbot_moby->state == 0xB) {
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
    set_range_centered_on_value((GroundRangeMoby *)birdbot_moby, ground);
    if (collision_hit_material() == 0 && birdbot_moby->pos.f[2] < ground + 1.0f) {
        spawn_explosion(birdbot_moby,
                        &birdbot_vars->v40,
                        &birdbot_moby->pos,
                        0.0f,
                        0.0f,
                        5,
                        2,
                        4,
                        2.0f,
                        1.0f,
                        9.0f,
                        1.0f,
                        4,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        FUN_L00_00263fd8_u((char *)birdbot_moby,
                           0x6C8,
                           birdbot_moby->pos.f,
                           &birdbot_moby->rot,
                           0,
                           0,
                           0.0f,
                           D_L01_0015F580,
                           D_L01_0015F580,
                           D_L01_0015F580);
        FUN_L00_00263fd8_u((char *)birdbot_moby,
                           0x6C9,
                           birdbot_moby->pos.f,
                           &birdbot_moby->rot,
                           0,
                           0,
                           0.0f,
                           D_L01_0015F580,
                           D_L01_0015F580,
                           D_L01_0015F580);
        FUN_L00_00263fd8_u((char *)birdbot_moby,
                           0x6CA,
                           birdbot_moby->pos.f,
                           &birdbot_moby->rot,
                           0,
                           0,
                           0.0f,
                           D_L01_0015F580,
                           D_L01_0015F580,
                           D_L01_0015F580);
        FUN_L00_00263fd8_u((char *)birdbot_moby,
                           0x6EA,
                           birdbot_moby->pos.f,
                           &birdbot_moby->rot,
                           0,
                           0,
                           0.0f,
                           D_L01_0015F580,
                           D_L01_0015F580,
                           D_L01_0015F580);
        FUN_L00_00257470(birdbot_moby, 0, -1);
        mark_moby_for_removal(birdbot_moby);
    }
}

u8 D_L05_001CAC80 NOT_SDA = 1;
u8 D_L07_001CA880 NOT_SDA = 1;
u8 D_L11_001CB200 NOT_SDA = 1;
u8 D_L12_001CAC80 NOT_SDA = 1;
u8 D_L13_001CAB00 NOT_SDA = 1;
