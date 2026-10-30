/* Shared moby class wrench: swings, throws and returning collision sweeps. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/math/random.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/hero_tables.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/shared/pickups/bolt.h"

typedef int q_2a7780 __attribute__((mode(TI)));

typedef struct {
    s16       sel[4];
    u32       col[4][4];
    f32       f48;
    q_2a7780 *verts;
    s16 (*idx)[4][2];
    f32 *uv;
    s16  n;
    f32  f5C;
} E_2a7780;

typedef struct {
    q_2a7780 m[3];
    q_2a7780 m30;
    u8       pad[0x30];
    f32      f70;
    u8       pad2[4];
    s16      h78;
} R_2a7780;

typedef struct {
    u8        pad[0x78];
    R_2a7780 *r;
} O_2a7780;

typedef struct {
    u8  pad[0x2080];
    u8 *p2080;
} P_2a7780;

typedef struct {
    q_2a7780 pos[4];
    u32      col[4];
    f32      uv[4][2];
    u64      tag[4];
} V_2a7780;

typedef struct {
    char p0[0x50];
    u8   b50;
    u8   b51;
    u8   b52;
    u8   b53;
    f32  f54;
} O_002a7c70;

typedef struct {
    char        p0[0x198];
    s32         i198;
    char        p1[0x2080 - 0x19C];
    O_002a7c70 *p2080;
    s32         i2084;
    char        p2[8];
    s32         i2090;
} G_002a7c70;

typedef union {
    u128 q;
    f32  f[4];
} V2a8028;

typedef struct N_2a82c0 {
    char             p0[0x20];
    unsigned char    type;
    char             p1[3];
    char            *info;
    struct N_2a82c0 *next;
} N_2a82c0;

typedef unsigned int WrenchQuad __attribute__((mode(TI)));

typedef float WrenchVec[4] __attribute__((aligned(16)));

typedef struct WrenchMoby WrenchMoby;

typedef struct {
    char  pad0[0x46];
    short kind;
} WrenchClass;

typedef struct {
    char      pad0[0x40];
    WrenchVec direction, target;
    float     speed, acceleration;
    int       pad68, timer;
    float     hit_blend;
    short     animation_count, collision, animation78, pad7A, previous_count, aim_hit;
} WrenchVars;

struct WrenchMoby {
    char           pad0[0x10];
    WrenchVec      position;
    unsigned char  state, pad21[3];
    WrenchClass   *model;
    char           pad28[0x28];
    unsigned char  frame, frame_end, animation, current_animation;
    float          animation_time, animation_scale;
    char           pad5C[0x14];
    unsigned char  animation_flags;
    char           pad71[7];
    WrenchVars    *data;
    char           pad7C[0x2A];
    unsigned short class_id;
    char           padA8[0x10];
    WrenchMoby    *parent;
};

typedef struct {
    char           pad0[0x80];
    WrenchVec      position;
    char           pad90[8];
    float          yaw;
    char           pad9C[0xA4];
    WrenchVec      velocity;
    char           pad150[0x48];
    int            state_timer;
    char           pad19C[0x854];
    WrenchVec      sweep_start, sweep_end, previous_start, previous_end;
    char           padA30[0x30];
    int            attack_index, attack_cooldown;
    float          reaction_yaw;
    char           padA6C[0x10];
    unsigned short hit_index;
    char           padA7E[0x12];
    float          animation_scale;
    char           padA94[8];
    int            blocked;
    char           padAA0[8];
    float          animation_frame;
    unsigned char  padAAC[4];
    int            selected_weapon, weapon_animation;
    char           padAB8[0x5F2];
    unsigned char  thrown, pad10AB, wrench_state;
    char           pad10AD[7];
    int            mode;
    char           pad10B8[0xFC8];
    WrenchMoby    *moby;
    int            state, pad2088, move_mode;
    char           pad2090[0x1E];
    unsigned char  alternate;
    char           pad20AF[4];
    unsigned char  no_ground_adjust;
    char           pad20B4[0x168];
    int            thrown_sound;
} WrenchHero;

typedef struct {
    char      pad0[0x140];
    WrenchVec position;
    float     pad150, pitch, yaw;
} PlayerCamera;

typedef struct {
    char        pad0[0x18];
    WrenchMoby *moby;
    int         surface;
    WrenchVec   position;
} WrenchHit;

typedef struct {
    char  pad0[0x14];
    short hit_index;
    char  pad16[6];
    short hit_count;
} MobyHitState;

typedef struct {
    char           pad0[8];
    float          force, range;
    char           pad10[8];
    unsigned char  flags, enabled;
    unsigned short class_id;
} HitQuery;

typedef union {
    WrenchVec  v[8];
    WrenchQuad q[8];
    HitQuery   query;
    struct {
        WrenchVec pad0;
        HitQuery  query;
    } shifted;
} WrenchScratch;

extern float D_0015ED6C;
extern f32   D_0015ED6C;
extern s32   FUN_001efa68(void *, void *, s32, s32, s32);

extern int  game_frame __asm__("D_L00_0015F5CC");
extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void FUN_001f7d30_2a7780(void *, void *, int) __asm__("FUN_001f7d30");

extern E_2a7780 effects[] __asm__("D_L00_001D92E8");
extern int      last_frame __asm__("D_L00_001613C0");
extern f32      uv_scale __asm__("D_L00_001613BC") __attribute__((sda));

extern char *get_moby_target_info(void *) __asm__("FUN_002141f8");

char *FUN_L00_0025c338(int);

extern void
emit_2a7e20(float *, int, int, int, int, void *, float, float, float) __asm__("FUN_L00_00272060");

extern V2a8028 D_L00_00173E60_lui __asm__("D_L00_00173E60") NOT_SDA;
extern f32     D_L00_00173E40_lui[] __asm__("D_L00_00173E40") NOT_SDA;
extern void    spawn_two_point_effect_particle(void *, void *) __asm__("FUN_L00_0026c6e8");
extern void    spawn_burst(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern void    FUN_L00_002a7e20_c(void *, s32) __asm__("FUN_L00_002a7e20");

extern N_2a82c0 *D_L00_0015FFE4;
extern int       is_moby_class_e2_cc_de_3ee_d6_e1_d5_or_df(void *) __asm__("FUN_L00_0025e3f8");
extern int       try_collect_ammo_pickup(void *) __asm__("FUN_L00_002c5be8");

extern HeroTableRow2C attack_records[] __asm__("D_L00_0017BC28");
extern WrenchMoby    *D_L00_00177F00[];
extern char           D_0013E533[];
extern float          approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float          probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int            FUN_001efa68(void *, void *, int, int, int);
extern int            test_hero_animation_frame_window(float) __asm__("FUN_L00_00211870");
extern int
sweep_hit_segment(void *, void *, void *, void *, void *, void *, int) __asm__("FUN_L00_00259d08");
extern int is_nonnull_moby_outside_table_or_class_range_500_to_540_or_class_502(char *) __asm__("FUN_L00_0025e368");
extern void hero_set_attack_ticks(int) __asm__("FUN_L00_00232640");
extern void
offset_hero_position_against_gravity(float *, float *, float) __asm__("FUN_L00_002336e8");
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
extern void compose_target_euler_rotation(char *, float, float, float) __asm__("FUN_L00_00262480");
extern void update_moby_transform_history(void *, int) __asm__("FUN_L00_00262608");
extern void clear_moby_transform_history(char *) __asm__("FUN_L00_00262840");
extern void FUN_L00_002a7780_cb(void) __asm__("FUN_L00_002a7780");
void        FUN_L00_002a7c70_c(void *m) __asm__("FUN_L00_002a7c70");
void        emit_randomized_sparkle_pairs(float *pos, int n) __asm__("FUN_L00_002a7e20");
void        spawn_wrench_impact_effects(u8 *o) __asm__("FUN_L00_002a8028");
void        process_mobies_near_returning_wrench(unsigned char *m) __asm__("FUN_L00_002a82c0");
void        scale_wrench_collision_vector_by_heading(float *v) __asm__("FUN_L00_002a8418");

extern char      thrown_wrench_transform_history[] __asm__("D_00140B00");
extern char      wrench_return_position[] __asm__("D_001403C0");
extern WrenchHit collision_hit __asm__("D_L00_00173E40");
extern void
draw_target_reticle(int, int, float, float, int, int, int, float, int) __asm__("FUN_L00_001edf60");
extern int collide_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

extern char D_0013F350[];
#define P_2a7780_G (*(P_2a7780 *)&hero)

/* Draws the animated quad-strip effect attached to o; the effect set is chosen by state 0x17..0x19 / 0x2B. */

void draw_wrench_swing_effect(O_2a7780 *effect_owner) __asm__("FUN_L00_002a7780");

void draw_wrench_swing_effect(O_2a7780 *effect_owner) {
    R_2a7780 *r;
    E_2a7780 *effect_config;
    V_2a7780  quad;
    s32       sel0, sel1, sel2, sel3;
    u32       color;
    f32       scale, alpha;
    s32       effect_texture_id, idx, nv;
    s32       k;
    s32       i, j, elapsed_frames, current_frame;
    f32       effect_progress, uv_v_offset;

    r = effect_owner->r;
    if (r == 0)
        return;
    if (r->f70 == 0.0f)
        return;
    if (r->f70 == 1.0f) {
        if (P_2a7780_G.p2080[0x52] != P_2a7780_G.p2080[0x53]) {
            r->f70 = 0.0f;
            return;
        }
        if (P_2a7780_G.p2080[0x52] == 0xFF) {
            r->f70 = 0.0f;
            return;
        }
    }
    scale = 1.0f;
    effect_texture_id = 0x14;
    nv = 1;
    color = 0x808080;
    alpha = scale;
    if (!((u32)(P_2a7780_G.p2080[0x52] - 0x17) < 3 || P_2a7780_G.p2080[0x52] == 0x2B))
        k = r->h78;
    else
        k = P_2a7780_G.p2080[0x52];
    if (k == 0x17) {
        r->h78 = k;
        idx = 0;
    } else if (k == 0x18) {
        r->h78 = k;
        idx = 1;
    } else if (k == 0x19) {
        r->h78 = k;
        idx = 2;
    } else {
        r->h78 = 0x2B;
        alpha = 0.7f;
        idx = 3;
        scale = 1.25f;
        effect_texture_id = 0xC;
        nv = 2;
        color = 0x408080;
    }
    effect_progress = r->f70;
    effect_config = &effects[idx];
    sel0 = effect_config->sel[0];
    sel1 = effect_config->sel[1];
    sel2 = effect_config->sel[2];
    sel3 = effect_config->sel[3];
    if (effect_progress == 1.0f) {
        u8 *hero_moby = P_2a7780_G.p2080;
        qcopy(&r->m[0], hero_moby + 0xC0);
        qcopy(&r->m[1], hero_moby + 0xD0);
        qcopy(&r->m[2], hero_moby + 0xE0);
        last_frame = game_frame;
    }
    current_frame = game_frame;
    elapsed_frames = current_frame - last_frame;
    if (elapsed_frames <= 0)
        elapsed_frames = 1;
    r->f70 = effect_progress + (f32)elapsed_frames * effect_config->f48;
    last_frame = current_frame;
    if ((f32)(effect_config->n * 2) <= r->f70) {
        s32 k2 = P_2a7780_G.p2080[0x52];
        if ((u32)(k2 - 0x17) < 3)
            return;
        if (k2 != 0x2B)
            r->f70 = 0.0f;
        return;
    }
    quad.tag[1] = get_effect_texture(effect_texture_id);
    quad.tag[2] = 0xFF9000000260LL;
    quad.tag[0] = 5;
    quad.tag[3] = (nv << 2) | 0x8000000040LL;
    qcopy(&r->m30, P_2a7780_G.p2080 + 0x10);
    for (i = 0; i < effect_config->n; i++) {
        for (j = 0; j < 4; j++) {
            s32  vi = effect_config->idx[i][j][0];
            f32 *uv;
            if (i == sel0)
                quad.col[j] = effect_config->col[0][j];
            else if (i == sel1)
                quad.col[j] = effect_config->col[1][j];
            else if (i == sel2)
                quad.col[j] = effect_config->col[2][j];
            else if (i == sel3)
                quad.col[j] = effect_config->col[3][j];
            else
                quad.col[j] = 0x50808080;
            quad.col[j] = (truncate_float_to_s32(((s32)quad.col[j] >> 24) * alpha) << 24) | color;
            fast_vec_scale(&quad.pos[j], &effect_config->verts[vi], scale);
            uv = (f32 *)(effect_config->idx[i][j][1] * 8 + (s32)effect_config->uv);
            uv_v_offset = 1.0f - r->f70 * effect_config->f5C;
            quad.uv[j][0] = (uv[0] - 0.5f) * uv_scale + 0.5f;
            quad.uv[j][1] = (uv[1] - 0.5f) * uv_scale + 0.5f + uv_v_offset;
        }
        FUN_001f7d30_2a7780(&quad, r, 0);
    }
}
#undef P_2a7780_G

void update_wrench_animation_from_hero_state(O_002a7c70 *wrench_moby) __asm__("FUN_L00_002a7c70");

void update_wrench_animation_from_hero_state(O_002a7c70 *wrench_moby) {
    struct Hero *hero_context = &hero;
    if (hero_context->state.current == 0x3B) {
        u8  hero_current_animation = ((O_002a7c70 *)hero_context->moby)->b53;
        s32 k = 0xB;
        if (hero_current_animation == 0x40)
            k = 0x13;
        if (hero_current_animation == 0x41)
            k = 0x12;
        if (k == 0x12 || k == 0x13) {
            wrench_moby->b52 = k;
            wrench_moby->b53 = k;
            wrench_moby->f54 = ((O_002a7c70 *)hero_context->moby)->f54;
            wrench_moby->b50 = ((O_002a7c70 *)hero_context->moby)->b50;
            wrench_moby->b51 = ((O_002a7c70 *)hero_context->moby)->b51;
            if (((O_002a7c70 *)hero_context->moby)->b52 != ((O_002a7c70 *)hero_context->moby)->b53)
                wrench_moby->b50 = wrench_moby->b51;
            update_moby_animation_state(wrench_moby);
        } else {
            blend_moby_animation(wrench_moby, k, 0, scale_game_frames(7));
        }
    } else if (hero_context->state.prev == 0x3B && hero_context->state_timer == 1) {
        blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(0x11));
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A9030), where it is exact; names translated to the US level program. */

/* True when the record for coll_hit_moby (get_moby_target_info) has byte 9 equal to 1. */

int get_wrench_hit_sound_variant(void) __asm__("FUN_L00_002a7d90");

int get_wrench_hit_sound_variant(void) {
    int   record_byte_9_is_one = 0;
    char *x = 0;
    if (coll_hit_moby != 0)
        x = get_moby_target_info(coll_hit_moby);
    if (x != 0)
        record_byte_9_is_one = *(unsigned char *)(x + 9) == 1;
    return record_byte_9_is_one;
}

/* True when FUN_L00_0025c338((int)coll_hit_moby) is set and has flag 2 at 0x1E. */

int should_trigger_wrench_hit_reaction(void) __asm__("FUN_L00_002a7de0");

int should_trigger_wrench_hit_reaction(void) {
    char *flag_record = FUN_L00_0025c338((int)coll_hit_moby);
    if (flag_record && (*(unsigned short *)(flag_record + 0x1e) & 2))
        return 1;
    return 0;
}

void emit_randomized_sparkle_pairs(float *sparkle_spawn_position,
                                   int    emission_pair_count) __asm__("FUN_L00_002a7e20");

void emit_randomized_sparkle_pairs(float *sparkle_spawn_position, int emission_pair_count) {
    OvlQuad sparkle_initial_velocity[1];
    OvlQuad old[1];
    int     i;
    for (i = 0; i < emission_pair_count; i++) {
        float base_sparkle_size = random_float_between(0.1f, 2.0f);
        int   k = random_integer_below(2);
        float azimuth_angle, elevation_angle;
        if (k == 0)
            k--;
        azimuth_angle = random_angle_radians();
        elevation_angle = random_float_between(0.34906584f, 1.53588974f);
        build_spherical_offset(sparkle_initial_velocity,
                               random_float_between(0.05f, 0.1f),
                               azimuth_angle,
                               elevation_angle);
        qcopy(old, sparkle_spawn_position);
        sparkle_spawn_position[0] += randf_sym(0.0f, 0.15f);
        sparkle_spawn_position[1] += randf_sym(0.0f, 0.15f);
        sparkle_spawn_position[2] += randf_sym(0.0f, 0.15f);
        emit_2a7e20(sparkle_spawn_position,
                    scale_game_frames(0x1E),
                    0x7F207F7F,
                    0,
                    k,
                    sparkle_initial_velocity,
                    base_sparkle_size * 0.1f,
                    base_sparkle_size,
                    0.003f);
        k = -k;
        emit_2a7e20(sparkle_spawn_position,
                    scale_game_frames(0x1E),
                    0x7F7F7F7F,
                    1,
                    k,
                    sparkle_initial_velocity,
                    base_sparkle_size * 0.07f,
                    base_sparkle_size * 0.7f,
                    0.003f);
    }
}

void spawn_wrench_impact_effects(u8 *wrench_moby) __asm__("FUN_L00_002a8028");

void spawn_wrench_impact_effects(u8 *wrench_moby) {
    V2a8028 effect_direction_scratch;
    V2a8028 effect_spawn_position;
    f32     pitch;
    f32     randomized_direction_angle;
    s32     i;
    s32     random_frame_lower_bound;
    fast_vec_sub(&effect_direction_scratch, &g_debug_camera, &D_L00_00173E60_lui);
    pitch =
        -fast_arc_tan(vector_length_xy(&effect_direction_scratch), effect_direction_scratch.f[2]);
    fast_arc_tan(effect_direction_scratch.f[0], effect_direction_scratch.f[1]);
    for (i = 19; i >= 0; i--) {
        randomized_direction_angle = fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch);
        build_spherical_offset(&effect_direction_scratch,
                               0.2f,
                               fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch),
                               randomized_direction_angle);
        fast_vec_scale(
            &effect_spawn_position, &effect_direction_scratch, random_float_between(1.0f, 3.0f));
        fast_vec_add(&effect_spawn_position, &effect_spawn_position, &D_L00_00173E60_lui);
        spawn_two_point_effect_particle(&effect_spawn_position, &effect_direction_scratch);
    }
    for (i = 9; i >= 0; i--) {
        effect_spawn_position.f[0] = D_L00_00173E40_lui[8] + randf_sym(0.0f, 0.1f);
        effect_spawn_position.f[1] = D_L00_00173E40_lui[9] + randf_sym(0.0f, 0.1f);
        effect_spawn_position.f[2] = D_L00_00173E40_lui[10] + randf_sym(0.0f, 0.1f);
        randomized_direction_angle = fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch);
        build_spherical_offset(&effect_direction_scratch,
                               0.05f,
                               fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch),
                               randomized_direction_angle);
        random_frame_lower_bound = scale_game_frames(10);
        spawn_burst(&effect_spawn_position,
                    &effect_direction_scratch,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    random_int_between(random_frame_lower_bound, scale_game_frames(20)),
                    1,
                    20000.0f);
    }
    if (*(s16 *)(((u8 *)MOBY(wrench_moby)->pvars) + 0x78) == 0x2B) {
        FUN_L00_002a7e20_c(&D_L00_00173E60_lui, 10);
    }
}

void process_mobies_near_returning_wrench(unsigned char *wrench_moby) __asm__("FUN_L00_002a82c0");

void process_mobies_near_returning_wrench(unsigned char *wrench_moby) {
    N_2a82c0    *scanned_moby = D_L00_0015FFE4;
    struct Hero *hero_context;
    int          moby_processing_result;
    if (scanned_moby == 0)
        return;
    hero_context = &hero;
    do {
        if (scanned_moby->type == 0xFE)
            return;
        if (scanned_moby->type == 0xFD)
            return;
        if (scanned_moby && scanned_moby->info && *(short *)(scanned_moby->info + 0x46) == 0x13) {
            if (fast_vec_dist((char *)scanned_moby + 0x10, wrench_moby + 0x10) < 2.7f) {
                moby_processing_result =
                    begin_bolt_attraction(scanned_moby,
                                          ((void *)hero_context->moby),
                                          random_float_between(-30.0f, 30.0f) * DEG_TO_RAD,
                                          0.0f);
                goto check;
            }
        } else if (is_moby_class_e2_cc_de_3ee_d6_e1_d5_or_df(scanned_moby) &&
                   fast_vec_dist((char *)scanned_moby + 0x10, wrench_moby + 0x10) < 2.7f) {
            moby_processing_result = try_collect_ammo_pickup(scanned_moby);
        check:
            if (moby_processing_result)
                return;
        }
        scanned_moby = scanned_moby->next;
    } while (scanned_moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A96B8), where it is exact; names translated to the US level program. */

void scale_wrench_collision_vector_by_heading(float *in_out_vector) __asm__("FUN_L00_002a8418");

void scale_wrench_collision_vector_by_heading(float *in_out_vector) {
    float k = 1.8325957f;
    float vector_heading_angle = fast_arc_tan(in_out_vector[0], in_out_vector[1]);
    float clamped_heading_difference =
        fast_difference_between_rotations(hero.motion.rot.f[2], vector_heading_angle);
    if (clamped_heading_difference > k)
        clamped_heading_difference = k;
    fast_vec_normalize(in_out_vector,
                       in_out_vector,
                       fast_vec_length(in_out_vector) *
                           ((k - clamped_heading_difference) / k * 0.3f + 0.7f));
}

/* Updates normal wrench swings and its thrown/returning collision sweep. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A9768), where it is exact; names translated to the US level program. */

static __inline__ WrenchHero *wrench_hero(void) {
    return (WrenchHero *)D_0013F350;
}

static __inline__ float *sweep_end(void) {
    return (float *)(D_0013E533 + 0x181D);
}

static __inline__ unsigned long combo_step(int animation) {
    return (unsigned char)(animation - 15);
}

static __inline__ float *hero_velocity(void) {
    return (float *)(D_0013E533 + 0xF5D);
}

static __inline__ unsigned char *voice_slot(int slot) {
    return (unsigned char *)&voice_pool + slot * 0x70;
}

static __inline__ WrenchHero *sweep_hero(void) {
    return (WrenchHero *)((char *)sweep_end() - 0xA00);
}

void wrench_update(WrenchMoby *m) __asm__("FUN_L00_002a84c8");

void wrench_update(WrenchMoby *wrench_moby) {
    enum {
        WRENCH_NORMAL = 0,
        WRENCH_THROW_OUTBOUND = 10,
        WRENCH_THROW_RETURNING = 11
    };
    WrenchVars   *wrench_vars = wrench_moby->data;
    WrenchScratch work;
    WrenchHero   *player = wrench_hero();
    player->wrench_state = 0;
    if (player->mode == 3)
        return;
    switch (wrench_moby->state) {
    case WRENCH_NORMAL: {
        int             player_state;
        int             anim;
        WrenchMoby     *player_moby;
        HeroTableRow2C *attack;
        int             active;
        int             reaction;
        float           difference, yaw;
        wrench_moby->parent = player->moby;
        player_state = player->state;
        if (player_state == 1) {
            if (pad_held[0] & 0xA)
                draw_target_reticle(
                    (int)wrench_moby, 0xFF917267, 1.0f, 0.0f, 0, 0x25, -1, 90.0f, 4);
            wrench_vars->aim_hit = 0;
            build_spherical_offset(work.v[0], 10.0f, l00_camera.yaw, -l00_camera.pitch);
            fast_vec_add(work.v[0], work.v[0], l00_camera.pos.f);
            if (FUN_001efa68(l00_camera.pos.f, work.v[0], 4, (int)player->moby, 0)) {
                wrench_vars->aim_hit = player_state;
                qcopy(wrench_vars->target, collision_hit.position);
            }
        }
        add_draw_callback(FUN_L00_002a7780_cb, wrench_moby);
        anim = wrench_moby->current_animation;
        if ((unsigned char)anim != 0) {
            if ((unsigned char)anim == 14) {
                if (wrench_moby->animation_flags & 2)
                    blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(5));
                return;
            } else if (wrench_hero()->alternate) {
                if (combo_step(anim) >= 2U)
                    blend_moby_animation(wrench_moby, 15, 0, scale_game_frames(4));
                else if ((wrench_moby->animation_flags & 2) && (unsigned char)anim == 15)
                    blend_moby_animation(wrench_moby, 16, 0, scale_game_frames(5));
                return;
            } else if (combo_step(anim) < 2U) {
                blend_moby_animation(wrench_moby, 14, 0, scale_game_frames(2));
                return;
            }
        }
        if ((wrench_moby->animation_flags & 2) && wrench_hero()->state != 0x3B &&
            wrench_moby->current_animation != 1)
            blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(5));
        FUN_L00_002a7c70_c((char *)wrench_moby);
        {
            int current_state = wrench_hero()->state;
            if (current_state != 19 && current_state != 33 && current_state != 43 &&
                current_state != 112 && current_state != 20) {
                int current = wrench_moby->current_animation;
                if ((unsigned int)current >= 3 && wrench_moby->current_animation < 6)
                    blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(4));
            }
        }
        player_moby = wrench_hero()->moby;
        if (player_moby->current_animation != player_moby->animation ||
            (int)player_moby->frame_end - player_moby->frame >= 3)
            wrench_vars->animation_count++;
        wrench_moby->animation_scale = wrench_hero()->animation_scale;
        attack = &attack_records[wrench_hero()->attack_index];
        if (wrench_hero()->move_mode != 6 && wrench_hero()->state != 43)
            break;
        active = 1;
        reaction = 0;
        if (wrench_hero()->blocked || wrench_hero()->animation_frame < (float)attack->f1C ||
            (float)attack->f20 < wrench_hero()->animation_frame)
            active = 0;
        {
            int reset = 0;
            if (wrench_vars->previous_count != wrench_vars->animation_count) {
                WrenchMoby *pm = wrench_hero()->moby;
                int         player_animation = pm->animation;
                if (player_animation == pm->current_animation) {
                    if (player_animation == 23)
                        reset = 0.0f <= wrench_hero()->animation_frame;
                    else if (player_animation == 24)
                        reset = 0.0f <= wrench_hero()->animation_frame;
                    else if (player_animation == 25)
                        reset = 12.0f <= wrench_hero()->animation_frame;
                    else if (player_animation == 43)
                        reset = 23.0f <= wrench_hero()->animation_frame;
                }
            }
            if (reset) {
                wrench_vars->hit_blend = 1.0f;
                wrench_vars->previous_count = wrench_vars->animation_count;
            }
        }
        difference = 0.0f;
        yaw = wrench_hero()->yaw;
        qcopy(wrench_hero()->previous_end, wrench_hero()->sweep_end);
        qcopy(wrench_hero()->previous_start, wrench_hero()->sweep_start);
        moby_point_position(wrench_moby, 1, wrench_hero()->sweep_end);
        moby_point_position(wrench_hero()->moby, 0, wrench_hero()->sweep_start);
        if (wrench_hero()->state == 19 || wrench_hero()->state == 112 ||
            wrench_hero()->state == 43) {
            yaw = fast_arc_tan(wrench_hero()->sweep_end[0] - wrench_hero()->position[0],
                               wrench_hero()->sweep_end[1] - wrench_hero()->position[1]);
            if (attack_records[wrench_hero()->attack_index].kind == 1)
                yaw = fast_add_rotations(yaw, -1.5707964f);
            else
                yaw = fast_add_rotations(yaw, 1.5707964f);
        }
        fast_vec_sub(work.v[0], sweep_end(), (sweep_end() - 4));
        fast_vec_normalize(work.v[0], work.v[0], fast_vec_length(work.v[0]) + 0.17f);
        if (sweep_hero()->move_mode != 15 && sweep_hero()->state != 112 &&
            sweep_hero()->state != 20) {
            difference = fast_difference_between_rotations(
                sweep_hero()->yaw,
                fast_arc_tan(sweep_hero()->sweep_end[0] - sweep_hero()->position[0],
                             sweep_hero()->sweep_end[1] - sweep_hero()->position[1]));
            if (difference > 1.5707964f)
                active = 0;
            scale_wrench_collision_vector_by_heading(work.v[0]);
        }
        fast_vec_add(sweep_end(), (sweep_end() - 4), work.v[0]);
        if (sweep_hero()->state != 43 && sweep_hero()->state != 112 && difference < 1.2217305f &&
            scale_game_frames(10) < sweep_hero()->state_timer) {
            float angle;
            qcopy(work.v[0], sweep_hero()->position);
            work.v[0][2] += 0.5f;
            angle = fast_arc_tan(sweep_hero()->sweep_end[0] - sweep_hero()->position[0],
                                 sweep_hero()->sweep_end[1] - sweep_hero()->position[1]);
            qcopy(work.v[1], sweep_hero()->position);
            work.v[1][2] += 0.5f;
            work.v[2][0] = fast_cos(angle) * 1.4f;
            work.v[2][1] = fast_sin(angle) * 1.4f;
            work.v[2][2] = 0.0f;
            scale_wrench_collision_vector_by_heading(work.v[2]);
            fast_vec_add(work.v[1], work.v[1], work.v[2]);
            if (!sweep_hero()->attack_cooldown && FUN_001efa68(work.v[0], work.v[1], 2, 0, 0) &&
                collision_hit_material()) {
                if (!collision_hit.moby ||
                    is_nonnull_moby_outside_table_or_class_range_500_to_540_or_class_502(
                        (char *)collision_hit.moby)) {
                    sweep_hero()->attack_cooldown = 1;
                    allocate_voice_for_target_entry(2, 0, wrench_moby);
                    spawn_wrench_impact_effects((char *)wrench_moby);
                    if (collision_hit.moby) {
                        work.v[3][0] = fast_cos(yaw);
                        work.v[3][1] = fast_sin(yaw);
                        work.v[3][2] = 0.0f;
                        spawn_nearby_effect(collision_hit.moby,
                                            (int)wrench_moby,
                                            0x10000,
                                            collision_hit.position,
                                            work.v[3],
                                            1.0f);
                    }
                }
            }
        }
        if (wrench_hero()->state == 20 && test_hero_animation_frame_window(27.0f)) {
            allocate_voice_for_target_entry(2, 0, wrench_moby);
            qcopy(work.v[0], wrench_hero()->sweep_end);
            qcopy(work.v[1], work.v[0]);
            offset_hero_position_against_gravity(work.v[0], work.v[0], 0.7f);
            offset_hero_position_against_gravity(work.v[1], work.v[1], -0.9f);
            if (FUN_001efa68(work.v[0], work.v[1], 2, 0, 0))
                spawn_wrench_impact_effects((char *)wrench_moby);
        }
        if (active) {
            int   count = 1;
            float scale = 1.0f, radius;
            if (wrench_hero()->state == 20) {
                scale = 1.55f;
                count = 2;
            }
            work.v[3][0] = fast_cos(yaw);
            work.v[3][1] = fast_sin(yaw);
            work.v[3][2] = 0.0f;
            make_hit_descriptor(work.v[0], wrench_moby, 0x10000, (float)count, work.v[3]);
            set_vector_length_xy(work.v[0], work.v[0], scale);
            work.query.force = 1.0f;
            work.query.range = 5627.925f;
            work.query.enabled = 1;
            work.query.class_id = wrench_moby->class_id;
            work.query.flags = 0;
            if (sweep_hit_segment(wrench_hero()->sweep_start,
                                  wrench_hero()->sweep_end,
                                  wrench_hero()->previous_start,
                                  wrench_hero()->previous_end,
                                  wrench_hero()->moby,
                                  &work,
                                  5) &&
                collision_hit.moby) {
                if (should_trigger_wrench_hit_reaction())
                    reaction = 1;
                else if (!wrench_hero()->attack_cooldown) {
                    if (!collision_hit.moby || !collision_hit.moby->model ||
                        collision_hit.moby->model->kind != 20) {
                        wrench_hero()->attack_cooldown = 1;
                        allocate_voice_for_target_entry(
                            get_wrench_hit_sound_variant(), 0, wrench_moby);
                        spawn_wrench_impact_effects((char *)wrench_moby);
                    }
                }
            }
            fast_vec_sub(work.v[5], sweep_end(), (sweep_end() - 4));
            fast_vec_normalize(work.v[5], work.v[5], fast_vec_length(work.v[5]) - 0.085f);
            fast_vec_add(work.v[4], work.v[5], (sweep_end() - 4));
            radius = 0.35f;
            if (sweep_hero()->move_mode == 15)
                radius = 0.7f;
            if (sweep_hero()->state == 20)
                radius = 0.47f;
            if (collide_sphere(work.v[4], radius, 0, sweep_hero()->moby, work.v[0]) &&
                (!D_L00_00177F00[0] || !D_L00_00177F00[0]->model ||
                 D_L00_00177F00[0]->model->kind != 18)) {
                if (should_trigger_wrench_hit_reaction())
                    reaction = 1;
                else if (!wrench_hero()->attack_cooldown) {
                    wrench_hero()->attack_cooldown = 1;
                    allocate_voice_for_target_entry(get_wrench_hit_sound_variant(), 0, wrench_moby);
                    spawn_wrench_impact_effects((char *)wrench_moby);
                }
            }
        }
        if (reaction) {
            hero_set_state(33, 1);
            if (collision_hit.moby) {
                WrenchMoby *target = collision_hit.moby;
                wrench_hero()->reaction_yaw =
                    fast_arc_tan(wrench_hero()->position[0] - target->position[0],
                                 wrench_hero()->position[1] - target->position[1]);
            } else
                wrench_hero()->reaction_yaw = fast_add_rotations(wrench_hero()->yaw, 3.1415927f);
        }
        break;
    }
    case WRENCH_THROW_OUTBOUND:
    case WRENCH_THROW_RETURNING: {
        int    collided;
        float *position;
        float  radius;
        {
            char       *control = ((char *)&thrown_wrench_transform_history);
            WrenchHero *control_player = (WrenchHero *)(control - 0x17B0);
            update_moby_transform_history(control, 0);
            wrench_moby->parent = control_player->moby;
            control_player->wrench_state = 2;
            if (wrench_moby->state == 10 && control_player->thrown_sound == -1)
                control_player->thrown_sound =
                    allocate_voice_for_target_entry(14, 4, control_player->moby);
        }
        compose_target_euler_rotation((char *)wrench_moby, 0.0f, 0.0f, D_0015ED6C * 24.434608f);
        if (wrench_hero()->attack_cooldown && fast_dec_timer(&wrench_vars->timer))
            wrench_hero()->attack_cooldown = 0;
        position = wrench_moby->position;
        if (!wrench_hero()->no_ground_adjust) {
            float ground = probe_ground_height(position, 0, 0.5f);
            if (wrench_moby->position[2] - ground < 0.8f) {
                ground += 0.55f;
                if (wrench_moby->position[2] < ground)
                    approach_value(&wrench_moby->position[2], ground, D_0015ED6C * 4.0f);
                else
                    approach_value(&wrench_moby->position[2], ground, D_0015ED6C);
            }
        }
        qcopy(work.v[0], position);
        if (wrench_moby->state == 10) {
            qcopy(work.v[1], hero_velocity());
            fast_vec_scale(work.v[1], work.v[1], 0.7f);
            fast_vec_add(position, position, work.v[1]);
            fast_vec_normalize(work.v[1], wrench_vars->direction, wrench_vars->speed);
            fast_vec_add(position, position, work.v[1]);
            wrench_vars->acceleration += frame_time_cube * 170.0f;
            approach_value(&wrench_vars->speed, 0.0f, wrench_vars->acceleration);
            if (wrench_vars->speed == 0.0f)
                wrench_moby->state = 11;
        } else {
            int   arrived = 0;
            float length, steps;
            process_mobies_near_returning_wrench((char *)wrench_moby);
            fast_vec_sub(work.v[1], ((char *)&wrench_return_position), position);
            wrench_vars->acceleration += frame_time_sq * 0.9f;
            wrench_vars->speed += wrench_vars->acceleration;
            length = fast_vec_length(work.v[1]);
            steps = wrench_vars->speed;
            steps = length / steps;
            if (steps < (float)scale_game_frames(5) && wrench_moby->current_animation != 1)
                blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(5));
            if (length <= wrench_vars->speed) {
                wrench_vars->speed = length;
                arrived = 1;
            }
            fast_vec_normalize(work.v[1], work.v[1], wrench_vars->speed);
            fast_vec_add(position, position, work.v[1]);
            if (length / wrench_vars->speed < (float)scale_game_frames(4) &&
                wrench_hero()->selected_weapon != -1) {
                int sound;
                hero_set_attack_ticks(2);
                wrench_hero()->weapon_animation = 26;
                sound = wrench_hero()->thrown_sound;
                if (sound != -1) {
                    unsigned char *audio = voice_slot(sound);
                    if (*(WrenchMoby **)(audio + 0x88) == wrench_hero()->moby && audio[0x74])
                        release_voice_slot(sound);
                }
                wrench_hero()->thrown_sound = -1;
            }
            if (arrived) {
                int sound;
                allocate_hero_voice_for_entry(5, 0);
                sound = wrench_hero()->thrown_sound;
                if (sound != -1) {
                    unsigned char *audio = voice_slot(sound);
                    if (*(WrenchMoby **)(audio + 0x88) == wrench_hero()->moby && audio[0x74])
                        release_voice_slot(sound);
                }
                wrench_hero()->thrown_sound = -1;
                clear_moby_transform_history((char *)wrench_hero() + 0x17B0);
                wrench_hero()->thrown = 0;
                wrench_moby->state = 0;
            }
        }
        collided = 0;
        if (FUN_001efa68(work.v[0], position, 2, 0, 0) && collision_hit_material() &&
            (!collision_hit.moby || !is_moby_in_class_range_500_to_540((int)collision_hit.moby)))
            collided = collision_hit.surface != 0;
        if (collided || wrench_vars->collision) {
            wrench_vars->collision = 0;
            if (!wrench_hero()->attack_cooldown) {
                wrench_vars->timer = scale_game_frames(30);
                wrench_hero()->attack_cooldown = 1;
                allocate_voice_for_target_entry(2, 0, wrench_moby);
            }
            if (wrench_moby->state == 10) {
                spawn_wrench_impact_effects((char *)wrench_moby);
                wrench_vars->speed *= 0.5f;
                wrench_moby->state = 11;
            }
        }
        radius = 1.2f;
        if (wrench_moby->state == 11)
            radius = -0.37f;
        fast_vec_normalize(work.v[4], wrench_vars->direction, radius);
        make_hit_descriptor(work.v[1], wrench_moby, 0x10000, 1.0f, work.v[4]);
        work.shifted.query.force = 1.0f;
        work.shifted.query.range = 5627.925f;
        work.shifted.query.enabled = 1;
        work.shifted.query.class_id = wrench_hero()->hit_index + 1;
        work.shifted.query.flags = 0;
        moby_point_position(wrench_moby, 1, work.v[5]);
        if (collide_sphere(work.v[5], 0.4f, 0, wrench_hero()->moby, work.v[1]) ||
            (moby_point_position(wrench_moby, 0, work.v[5]),
             collide_sphere(work.v[5], 0.4f, 0, wrench_hero()->moby, work.v[1]))) {
            if (!wrench_hero()->attack_cooldown && collision_hit.moby &&
                (!collision_hit.moby->model || collision_hit.moby->model->kind != 20)) {
                MobyHitState *hit = (MobyHitState *)get_moby_target_info(collision_hit.moby);
                if (!hit || hit->hit_count < 2 || hit->hit_index != work.shifted.query.class_id) {
                    work.q[7] = *(WrenchQuad *)collision_hit.moby->position;
                    interpolate_vector(work.v[6], work.v[5], work.v[7], 0.5f);
                    emit_randomized_sparkle_pairs(work.v[6], 5);
                    wrench_vars->timer = scale_game_frames(30);
                    wrench_hero()->attack_cooldown = 1;
                    allocate_voice_for_target_entry(get_wrench_hit_sound_variant(), 0, wrench_moby);
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

int D_L01_001613C0 = 0;
f32 D_L01_001613BC = 1.1f;
int D_L02_00161394 = 0;
f32 D_L02_00161390 = 1.1f;
int D_L03_0016139C = 0;
f32 D_L03_00161398 = 1.1f;
int D_L04_00161394 = 0;
f32 D_L04_00161390 = 1.1f;
int D_L05_00161398 = 0;
f32 D_L05_00161394 = 1.1f;
int D_L06_00161394 = 0;
f32 D_L06_00161390 = 1.1f;
int D_L07_00161394 = 0;
f32 D_L07_00161390 = 1.1f;
int D_L08_00161394 = 0;
f32 D_L08_00161390 = 1.1f;
int D_L09_0016141C = 0;
f32 D_L09_00161418 = 1.1f;
int D_L10_00161398 = 0;
f32 D_L10_00161394 = 1.1f;
int D_L11_00161394 = 0;
f32 D_L11_00161390 = 1.1f;
int D_L12_00161398 = 0;
f32 D_L12_00161394 = 1.1f;
int D_L13_0016145C = 0;
f32 D_L13_00161458 = 1.1f;
int D_L14_001615A0 = 0;
f32 D_L14_0016159C = 1.1f;
int D_L15_0016147C = 0;
f32 D_L15_00161478 = 1.1f;
int D_L16_0016144C = 0;
f32 D_L16_00161448 = 1.1f;
int D_L17_0016141C = 0;
f32 D_L17_00161418 = 1.1f;
int D_L18_00161434 = 0;
f32 D_L18_00161430 = 1.1f;

int last_frame __asm__("D_L00_001613C0") = 0;
