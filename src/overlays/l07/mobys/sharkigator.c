/* Level 07 moby class 0x423: sharkigator. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   is_point_inside_clip_volume() __asm__("FUN_00214720");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030FFA8), where it is exact; names translated to the US level program. */
extern float FUN_L00_00259710(void *);
int classify_sharkigator_water_height_gap(char *moby, char *data) __asm__("FUN_L07_0030ebc8");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00310038), where it is exact; names translated to the US level program. */
void set_sharkigator_idle_state_from_water_height_gap(char *sharkigator_moby) __asm__("FUN_L07_0030ec58");

/* Step a moby's velocity toward a target: gravity when above a height, then ease and clamp. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003104D8), where it is exact; names translated to the US level program. */
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern int move_moby_with_collision_integer_radius(
    int a, int b, int g, int e, float c, float d) __asm__("FUN_L00_00258ad0");
void advance_sharkigator_swim_movement(char *moby,
                                       char *moby_data,
                                       float acceleration_step,
                                       float braking_step,
                                       float maximum_speed) __asm__("FUN_L07_0030f0f8");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003105C0), where it is exact; names translated to the US level program. */
int        update_sharkigator_horizontal_velocity_by_heading(char *moby,
                                                             char *moby_data,
                                                             float target_heading,
                                                             float lo,
                                                             float up,
                                                             float hi,
                                                             float down,
                                                             float vmax,
                                                             float vmin) __asm__("FUN_L07_0030f1e0");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003108B0), where it is exact; names translated to the US level program. */
extern int D_L07_00173ED8 NOT_SDA;
extern void               moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
int                       test_sharkigator_target_sphere_collision(void *moby,
                                                                   int   id,
                                                                   float scale) __asm__("FUN_L07_0030f4d0");

#define F(p, o) (*(float *)((p) + (o)))

/* Moves a moby along a path, easing between nodes and tracking the hero. */
extern char *D_L07_001B0530[];

#include "rnc/overlay/cuboid.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030C3E0), where it is exact; names translated to the US level program. */
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int   get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void  spawn_explosion(void *,
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
extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
extern Vec4  D_L07_00173EE0;
extern void  spawn_explosion(void *,
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

/* Ported from rac1-decomp src/overlays/l07_umbris/vendor_002CE470.c (func_L07_0030D8E8) */
#include "rnc/overlay/water_grid.h"

extern WaterGrid *D_L07_001612D0;
extern int        D_L07_001612D8;
extern s32        random_integer_below(s32) __asm__("func_00213260");

#include "qzero.h"

extern int D_L07_0015F5CC MACRO_ADDR;
extern void               FUN_L00_0024f7c8(void *, int, void *);
extern void               FUN_L00_00257470(void *, int, int);
extern void               spawn_explosion(void *,
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
extern void               get_target_sight(void *, void *, float) __asm__("FUN_L07_002886d8");
extern int                D_L07_0015F5CC;

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030F838), where it is exact; names translated to the US level program. */
extern int  D_L07_00173ED8;
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
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

/* Knockback/launch parameters handed to trooper_start_knockback. */
typedef struct {
    u8  pad0[0x10];
    f32 gravity;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s32 flags;
    s32 unk24;
    f32 unk28;
    u8  pad2C[4];
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8  pad3C;
    u8  unk3D;
    u8  pad3E[0xA];
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
} LaunchParams;

extern void trooper_knockback_arc(Vec4f *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(f32, struct Moby *, LaunchParams *, s32, s32, s32) __asm__("FUN_L00_0025c558");
void start_sharkigator_death_knockback(struct Moby  *moby,
                                       LaunchParams *knockback_state,
                                       Vec4f *knockback_direction) __asm__("FUN_L07_0030e720");
void start_sharkigator_hit_knockback(struct Moby  *moby,
                                     LaunchParams *knockback_state,
                                     Vec4f        *knockback_direction) __asm__("FUN_L07_0030e858");

/* Hit handler: reacts to the hit type, then takes damage and dies (state 0xE) when health runs out. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030FD78), where it is exact; names translated to the US level program. */
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
void handle_sharkigator_hit(struct Moby *moby, char *x, float *hp) __asm__("FUN_L07_0030e998");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030FF18), where it is exact; names translated to the US level program. */
void cache_path_segment_lengths(char *data) __asm__("FUN_L07_0030eb38");

char D_L07_00211720[] = "Illegal Bounding path for moby 1059 instance #%d.\n";

char D_L07_00211758[] = "Illegal Hero Detect path for moby 1059 instance #%d.\n";

char D_L07_00211790[] = "Illegal water moby for moby 1059 instance #%d.\n";

char D_L07_002117C0[] = "Illegal guard point cuboid for moby 1059 instance #%d.\n";

extern void stub_printf(char *, int) __asm__("FUN_001e93b0");
extern int  rand(void) __asm__("FUN_001160d8");

/* Sets up the walker from its path, target and spot ids; reports a missing id and returns 1, else returns 0. */
int initialize_sharkigator(char *moby, char *data) __asm__("FUN_L07_0030ed30");

/* Picks the walker heading: keeps going while the probe ahead stays on its path and clear,
 * else sweeps by its turn step for a clear heading, else faces the goal. */
struct WalkerPath {
    s32  count;
    u8   pad4[0xC];
    Vec4 pts[1];
};

struct WalkerData {
    u8                 pad0[0xF0];
    Vec4               goal; /* 0xF0 */
    u8                 pad100[0x28];
    struct WalkerPath *path; /* 0x128 */
    u8                 pad12C[8];
    s32                wander; /* 0x134 */
    u8                 pad138[4];
    f32                heading;  /* 0x13C */
    f32                turnStep; /* 0x140 */
};

extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   line_blocked_30eee8(void *, void *, int, struct Moby *, int) __asm__("FUN_001efa68");
void         select_sharkigator_navigation_heading(struct Moby       *moby,
                                                   struct WalkerData *data) __asm__("FUN_L07_0030eee8");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

/* In the given state and animation, when the walker is within range, pushes it along its
 * velocity; a hit on the target bounces it back up and switches to the next state. */
void handle_sharkigator_contact_attack(struct Moby *moby,
                                       char        *moby_data,
                                       int          state,
                                       int          anim,
                                       int          seq,
                                       int          frame,
                                       int          next,
                                       int          id,
                                       float        lo,
                                       float        hi,
                                       float        hit_direction_length,
                                       float        scale,
                                       float        radius) __asm__("FUN_L07_0030f308");

/* Finds another moby whose state lies within the inclusive range. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00310920), where it is exact; names translated to the US level program. */
int test_other_group_moby_state_range(unsigned char *moby,
                                      int            lower,
                                      int            upper) __asm__("FUN_L07_0030f540");

/* Update function of the sharkigator (class 1059): tracks the hero, swims its path, lunges and bites. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_003109D0), where it is exact; names translated to the US level program. */
struct WalkerTarget {
    float pos[4];
    u128  q10;
    u128  q20;
    u128  q30;
    int   moby;
    int   kind;
};

extern WaterGrid *D_L07_001612D0;
extern char       D_L07_002117F8[];
extern char       sharkigator_unexpected_init_message[] __asm__("D_L07_00211838");
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float         resolve_camera_surface_height(void *, int) __asm__("func_002135F0");
extern int           D_L07_001612D8;
extern int           DebugPrint_alt() __asm__("FUN_001e93b0");
extern int           collision_hit_material(void) __asm__("FUN_001f0b58");
extern int           FUN_L00_00259740(void *, void *, int);
extern int           step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int           classify_sharkigator_water_height_gap(char *moby,
                                                           char *data) __asm__("FUN_L07_0030ebc8");
extern s32           rand(void);
extern unsigned char l07_sharkigator_water_gap_transition_sequences[1] __asm__("D_L07_00161B50")
    __attribute__((sda));
extern unsigned char l07_sharkigator_water_gap_transition_states[1] __asm__("D_L07_00161B40")
    __attribute__((sda));
extern unsigned char *spawn_moving_resizing_height_snapped_alpha_fade_particle(
    void *, int, int, float, float) __asm__("FUN_L00_002715e8");
extern void FUN_L00_002223f8(int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_00263ac8(void *, int, void *, float);
extern void set_sharkigator_idle_state_from_water_height_gap(char *arg) __asm__("FUN_L07_0030ec58");
extern void noop_main_callback(void *, void *) __asm__("FUN_001e93e8");
int         update_sharkigator_horizontal_velocity_by_heading(char *moby,
                                                              char *obj,
                                                              float a,
                                                              float lo,
                                                              float up,
                                                              float hi,
                                                              float down,
                                                              float vmax,
                                                              float vmin) __asm__("FUN_L07_0030f1e0");
void        handle_sharkigator_hit(struct Moby *m, char *x, float *hp) __asm__("FUN_L07_0030e998");
void advance_sharkigator_swim_movement(char *a, char *b, float f12, float f13, float f14) __asm__("FUN_L07_0030f0f8");
void        handle_sharkigator_contact_attack(struct Moby *moby,
                                              char        *obj,
                                              int          state,
                                              int          anim,
                                              int          seq,
                                              int          frame,
                                              int          next,
                                              int          id,
                                              float        lo,
                                              float        hi,
                                              float        len,
                                              float        scale,
                                              float        radius) __asm__("FUN_L07_0030f308");
extern char D_0013F65C[];
extern char D_0013F350[];
extern void
func_L00_002A5158_30D8E8(void *, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");

#define func_L00_002592B0_t(m, a, v, k, d, x) \
    ((void (*)(char *, float, float *, float, float, float))FUN_L00_00258278)(m, a, v, k, d, x)

void         sharkigator_update(unsigned char *moby) __asm__("FUN_L07_0030f5f0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");

char D_L07_002117F8[] = "Initialization failed for moby 1059 instance #%d. Deleting.\n";

void start_sharkigator_death_knockback(struct Moby  *moby,
                                       LaunchParams *knockback_state,

                                       Vec4f *knockback_direction) {
    Vec4f knockback_direction_copy;
    f32   knockback_angle;

    knockback_state->gravity = frame_time_sq * 29.400002f;
    knockback_state->unk14 = 0.0005f;
    knockback_state->unk18 = frame_time * 9.0f;
    knockback_state->unk1C = frame_time * 7.0f;
    knockback_state->flags = 0x400;
    knockback_state->unk24 = 13;
    knockback_state->unk28 = 1.0f;
    knockback_state->unk30 = 0.24f;
    knockback_state->unk34 = 0.5f;
    knockback_state->unk38 = 0.65f;
    knockback_state->unk48 = 0.2f;
    knockback_state->unk4C = 0.01f;
    knockback_state->unk50 = 3.0f;
    knockback_state->unk54 = 9.0f;
    knockback_state->unk3D = 0;
    *(u128 *)&knockback_direction_copy = *(u128 *)knockback_direction;
    trooper_knockback_arc(&knockback_direction_copy,
                          &knockback_angle,
                          &knockback_state->unk18,
                          &knockback_state->unk1C);
    trooper_start_knockback(knockback_angle, moby, knockback_state, 12, 1, 0);
}

void start_sharkigator_hit_knockback(struct Moby  *moby,
                                     LaunchParams *knockback_state,

                                     Vec4f *knockback_direction) {
    Vec4f knockback_direction_copy;
    f32   knockback_angle;

    knockback_state->gravity = frame_time_sq * 29.400002f;
    knockback_state->unk14 = 0.0005f;
    knockback_state->unk18 = frame_time * 9.0f;
    knockback_state->unk1C = frame_time * 7.0f;
    knockback_state->flags = 0x599;
    knockback_state->unk24 = 13;
    knockback_state->unk28 = 1.4f;
    knockback_state->unk30 = 0.24f;
    knockback_state->unk34 = 0.5f;
    knockback_state->unk38 = 0.65f;
    knockback_state->unk48 = 0.2f;
    knockback_state->unk4C = 0.01f;
    knockback_state->unk50 = 8.0f;
    knockback_state->unk54 = 19.0f;
    knockback_state->unk3D = 0;
    *(u128 *)&knockback_direction_copy = *(u128 *)knockback_direction;
    trooper_knockback_arc(&knockback_direction_copy,
                          &knockback_angle,
                          &knockback_state->unk18,
                          &knockback_state->unk1C);
    trooper_start_knockback(knockback_angle, moby, knockback_state, 6, 1, 0);
}

void handle_sharkigator_hit(struct Moby *moby, char *x, float *hp) {
    enum {
        HIT_REACTION_NONE = 0,
        SHARKIGATOR_CLEAR_HEALTH_RESPONSE = 1,
        SHARKIGATOR_ALTERNATE_CLEAR_HEALTH_RESPONSE = 2,
        HIT_REACTION_START_KNOCKBACK = 3,
        SHARKIGATOR_KNOCKBACK_RESPONSE = 4,
        SHARKIGATOR_ALTERNATE_KNOCKBACK_RESPONSE = 5,
        SHARKIGATOR_START_HIT_KNOCKBACK_RECOVERY = 6,
        SHARKIGATOR_LAUNCH_HIT_KNOCKBACK_RECOVERY = 7,
        SHARKIGATOR_ENTER_HIT_KNOCKBACK_RECOVERY = 8,
        SHARKIGATOR_HIT_ANIMATION_RESPONSE = 9,
        SHARKIGATOR_ALTERNATE_HIT_ANIMATION_RESPONSE = 10,
        HIT_REACTION_NO_ANIMATION_RESPONSE = 11
    };
    char *info;
    int   hit;
    int   hit_reaction_result;
    if (((unsigned char *)moby)[0x20] == 0xE)
        return;
    info = find_moby_hit(moby, 0x330000, 0);
    hit_reaction_result = hit_reaction(moby, info, hp, 0, &hit, 0, 0, 4);
    if (((unsigned char *)moby)[0xBC] != 0) {
        switch (hit_reaction_result) {
        case HIT_REACTION_NONE:
            break;
        case SHARKIGATOR_CLEAR_HEALTH_RESPONSE:
        case SHARKIGATOR_ALTERNATE_CLEAR_HEALTH_RESPONSE:
            *hp = 0.0f;
            break;
        case HIT_REACTION_START_KNOCKBACK:
        case SHARKIGATOR_KNOCKBACK_RESPONSE:
        case SHARKIGATOR_ALTERNATE_KNOCKBACK_RESPONSE:
        case SHARKIGATOR_START_HIT_KNOCKBACK_RECOVERY:
        case SHARKIGATOR_LAUNCH_HIT_KNOCKBACK_RECOVERY:
        case SHARKIGATOR_ENTER_HIT_KNOCKBACK_RECOVERY:
            start_sharkigator_hit_knockback(moby, x + 0x70, info + 0x10);
            moby->state = 0xC;
            break;
        case SHARKIGATOR_HIT_ANIMATION_RESPONSE:
        case SHARKIGATOR_ALTERNATE_HIT_ANIMATION_RESPONSE:
            if (((unsigned char *)moby)[0x53] != 4) {
                blend_moby_animation(moby, 4, 4, 4);
            }
            moby->state = 0xD;
            break;
        case HIT_REACTION_NO_ANIMATION_RESPONSE:
            break;
        }
        if (hit >= 2) {
            if (*hp <= *(float *)(info + 0x2C)) {
                *hp = 0.0f;
                moby->flags &= 0xEFFF;
                start_sharkigator_death_knockback(moby, x + 0x70, info + 0x10);
                FUN_L00_00257470(moby, 0, -1);
                x[0x67] = 0x78;
                trooper_anim_update(moby, (short *)(x + 0x60));
                moby->state = 0xE;
            } else {
                *hp -= *(float *)(info + 0x2C);
                ((unsigned char *)x)[0x67] = 0xFA;
                *(short *)(x + 0x26) = scale_game_frames(0x3C);
                trooper_anim_update(moby, (short *)(x + 0x60));
            }
        }
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
}

void cache_path_segment_lengths(char *data) {
    int i, count;
    if (data != 0) {
        count = *(int *)data;
        i = 0;
        if (count > 0) {
            do {
                int point_byte_offset = i * 16;
                int next = (i + 1) % count;
                *(float *)(data + point_byte_offset + 0x1C) =
                    fast_vec_dist(data + (point_byte_offset + 0x10), data + (next * 16 + 0x10));
                i++;
                count = *(int *)data;
            } while (i < count);
        }
    }
}

int classify_sharkigator_water_height_gap(char *moby, char *data) {
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

void set_sharkigator_idle_state_from_water_height_gap(char *sharkigator_moby) {
    enum {
        SHARKIGATOR_LARGE_WATER_HEIGHT_GAP = 0,
        SHARKIGATOR_MEDIUM_WATER_HEIGHT_GAP = 1,
        SHARKIGATOR_SMALL_WATER_HEIGHT_GAP = 2
    };
    switch (((unsigned char *)sharkigator_moby)[0xBC]) {
    case SHARKIGATOR_LARGE_WATER_HEIGHT_GAP:
        if (((unsigned char *)sharkigator_moby)[0x53] != 0) {
            blend_moby_animation(sharkigator_moby, 0, 0, 10);
        }
        sharkigator_moby[0x20] = 1;
        break;
    case SHARKIGATOR_MEDIUM_WATER_HEIGHT_GAP:
        if (((unsigned char *)sharkigator_moby)[0x53] != 3) {
            blend_moby_animation(sharkigator_moby, 3, 0, 10);
        }
        sharkigator_moby[0x20] = 7;
        break;
    case SHARKIGATOR_SMALL_WATER_HEIGHT_GAP:
        if (((unsigned char *)sharkigator_moby)[0x53] != 9) {
            blend_moby_animation(sharkigator_moby, 9, 0, 10);
        }
        sharkigator_moby[0x20] = 0xF;
        break;
    }
}

int initialize_sharkigator(char *moby, char *data) {
    if (*(int *)(data + 0x110) == -1) {
        stub_printf(D_L07_00211720, (moby - level_mobys) >> 8);
        return 1;
    }
    cache_path_segment_lengths(*(char **)(data + 0x128) = D_L07_001B0530[*(int *)(data + 0x110)]);
    if (*(int *)(data + 0x144) == -1) {
        stub_printf(D_L07_00211758, (moby - level_mobys) >> 8);
        return 1;
    }
    cache_path_segment_lengths(*(char **)(data + 0x148) = D_L07_001B0530[*(int *)(data + 0x144)]);
    if (*(int *)(data + 0x118) == -1) {
        stub_printf(D_L07_00211790, (moby - level_mobys) >> 8);
        return 1;
    }
    *(char **)(data + 0x12C) = level_mobys + (*(int *)(data + 0x118) << 8) + 0x18;
    moby[0xBC] = classify_sharkigator_water_height_gap(moby, data);
    if (*(int *)(data + 0x114) == -1) {
        stub_printf(D_L07_002117C0, (moby - level_mobys) >> 8);
        return 1;
    }
    qcopy(data + 0xF0, (char *)l07_cuboids + *(int *)(data + 0x114) * 0x80 + 0x30);
    qcopy(data + 0xE0, moby + 0x10);
    ((void (*)(char *, char *))set_sharkigator_idle_state_from_water_height_gap)(moby, data);
    *(int *)(data + 0x134) = rand() % 0x78;
    if (rand() & 1) {
        *(float *)(data + 0x140) = 0.34906584f;
    } else {
        *(float *)(data + 0x140) = -0.34906584f;
    }
    *(int *)(data + 0x14C) = 0;
    data[0x29] = 1;
    return 0;
}

void select_sharkigator_navigation_heading(struct Moby *moby, struct WalkerData *data) {
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
        fast_vec_add(probe, probe, &moby->pos);
        if (point_in_path_xy(probe, data->path->pts, data->path->count) &&
            !line_blocked_30eee8(&moby->pos, probe, 0x24, moby, 0) &&
            !coll_sphere(1.5f, probe, 0x24, moby))
            return;
        step = absolute_float(data->turnStep);
        angle = moby->rot.z;
        for (swept = 0.0f; swept < 6.2831855f; swept += step) {
            angle = fast_add_rotations(angle, data->turnStep);
            probe[0] = fast_cos(angle) * 5.2f;
            probe[1] = fast_sin(angle) * 5.2f;
            *(s32 *)&probe[2] = 0;
            fast_vec_add(probe, probe, &moby->pos);
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

void advance_sharkigator_swim_movement(

    char *moby, char *moby_data, float acceleration_step, float braking_step, float maximum_speed) {
    float position_work[4];
    char *velocity;
    if (**(float **)(moby_data + 0x12C) - 0.3f < MOBY(moby)->pos.z) {
        *(float *)(moby_data + 0xD8) -= frame_time * 9.8f;
    }
    qcopy(position_work, moby + 0x10);
    advance_accelerated_scalar(position_work + 2,
                               (float *)(moby_data + 0xD8),
                               *(float *)(moby_data + 0x138),
                               acceleration_step,
                               braking_step,
                               maximum_speed);
    velocity = moby_data + 0xD0;
    if (move_moby_with_collision_integer_radius(
            (int)moby, (int)velocity, 0x400, 0x10, 1.0f, 0.0f)) {
        fast_vec_scale(velocity, velocity, 0.5f);
    }
}

int update_sharkigator_horizontal_velocity_by_heading(char *moby,
                                                      char *moby_data,
                                                      float target_heading,
                                                      float lo,
                                                      float up,
                                                      float hi,
                                                      float down,
                                                      float vmax,

                                                      float vmin) {
    float speed = vector_length_xy(moby_data + 0xD0);
    int   heading_below_lower_threshold = 0;
    float heading_difference = fast_difference_between_rotations(MOBY(moby)->rot.z, target_heading);
    if (heading_difference < lo) {
        speed += up;
        heading_below_lower_threshold = 1;
    } else if (hi < heading_difference) {
        speed -= down;
    }
    if (vmax < speed) {
        speed = vmax;
    } else if (speed < vmin) {
        speed = vmin;
    }
    *(float *)(moby_data + 0xD0) = fast_cos(MOBY(moby)->rot.z) * speed;
    *(float *)(moby_data + 0xD4) = fast_sin(MOBY(moby)->rot.z) * speed;
    return heading_below_lower_threshold;
}

void handle_sharkigator_contact_attack(struct Moby *moby,
                                       char        *moby_data,
                                       int          state,
                                       int          anim,
                                       int          seq,
                                       int          frame,
                                       int          next,
                                       int          id,
                                       float        lo,
                                       float        hi,
                                       float        hit_direction_length,
                                       float        scale,

                                       float radius) {
    float hit_direction[4];
    float hit_descriptor[12];
    float probe_center[4];
    float interpolated_record_value;
    char *moby_velocity;

    interpolated_record_value = compute_interpolated_record_value(moby);
    if (((unsigned char *)moby)[0x20] != state) {
        return;
    }
    if (((unsigned char *)moby)[0x52] != anim) {
        return;
    }
    if (!(lo < interpolated_record_value)) {
        return;
    }
    if (!(interpolated_record_value < hi)) {
        return;
    }
    moby_velocity = moby_data + 0xD0;
    fast_vec_normalize(hit_direction, moby_velocity, hit_direction_length);
    hit_direction[2] = 1.0f;
    make_hit_descriptor((char *)hit_descriptor, (int)moby, 1, hit_direction, scale);
    moby_point_position(moby, 0, probe_center);
    if (collect_mobies_in_sphere(probe_center, radius, 0x21, moby, hit_descriptor) == 0) {
        return;
    }
    if (D_L07_00173ED8 != id) {
        return;
    }
    fast_vec_scale(moby_velocity, moby_velocity, -1.0f);
    /* *&obj gives obj the register retail uses */
    *(float *)(*&moby_data + 0xD8) = -(frame_scale * 0.05f);
    if (((unsigned char *)moby)[0x53] != seq) {
        blend_moby_animation(moby, seq, frame, 5);
    }
    moby->state = next;
}

int test_sharkigator_target_sphere_collision(void *moby, int id, float scale) {
    char collision_center[16];
    moby_point_position(moby, 0, collision_center);
    if (coll_sphere(scale, collision_center, 0x21, moby) && D_L07_00173ED8 == id) {
        return 1;
    }
    return 0;
}

int test_other_group_moby_state_range(unsigned char *moby, int lower, int upper) {
    unsigned char *other = 0;
    if (!get_first_moby_in_group((int *)&other, moby[0x21], 0, 0)) {
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
                if (get_next_moby_in_group((int *)&other, (int)other, 0, 0))
                    break;
            } while (other);
        }
    }
    return 0;
found:
    return 1;
}

void sharkigator_update(unsigned char *moby) {
    enum {
        SHARKIGATOR_INITIALIZE = 0,
        SHARKIGATOR_SWIM_IDLE = 1,
        SHARKIGATOR_SWIM_WANDER = 2,
        SHARKIGATOR_TARGET_PURSUIT = 4,
        SHARKIGATOR_TARGET_LUNGE_INTERPOLATION = 5,
        SHARKIGATOR_POST_LUNGE_PATH_FOLLOW = 6,
        SHARKIGATOR_GRAVITY_IDLE = 7,
        SHARKIGATOR_GRAVITY_WANDER = 8,
        SHARKIGATOR_TARGET_APPROACH = 9,
        SHARKIGATOR_TARGET_CONTACT_ATTACK = 10,
        SHARKIGATOR_TARGET_CONTACT_RECOIL_RECOVERY = 11,
        SHARKIGATOR_HIT_KNOCKBACK_RECOVERY = 12,
        SHARKIGATOR_HIT_ANIMATION_RECOVERY = 13,
        SHARKIGATOR_DEATH_FALL_AND_REMOVE = 14,
        SHARKIGATOR_SMALL_WATER_GAP_GRAVITY_IDLE = 15,
        SHARKIGATOR_DAMPED_GRAVITY_RECOVERY = 16,
        SHARKIGATOR_DAMPED_RECOVERY = 17
    };
    char               *data;
    struct WalkerTarget target;
    float               point[4];
    float               offset[4];
    float               goal[4];
    float               delta[4];
    float               previous[4];
    int                 wander;
    int                 inside;
    int                 next_sequence;
    int                 next_state;
    unsigned char       old_command;
    unsigned char       command;
    char               *hero;

    if (moby == 0) {
        return;
    }
    data = ((char *)MOBY(moby)->pvars);
    if (data == 0) {
        return;
    }
    FUN_L00_00263ac8(moby, 0, data + 0x150, 2.2f);
    if (moby[0x20] == 0) {
        if (initialize_sharkigator((char *)moby, data)) {
            DebugPrint_alt(D_L07_002117F8, (int)((char *)moby - (char *)level_mobys) >> 8);
            mark_moby_for_removal(moby);
            return;
        }
        goto end;
    }
    get_target_sight(moby, &target, 30.0f);
    handle_sharkigator_hit((char *)moby, data, (float *)(data + 0x20));
    trooper_anim_finish(moby, data + 0x60);
    wander = fast_dec_timer((int *)(data + 0x134)) != 0;
    if (*(int *)(data + 0x110) != 0 && moby[0x20] >= 7) {
        char *path = *(char **)(data + 0x148);
        inside = FUN_L00_00259740(&target, path + 0x10, *(int *)path) != 0;
    } else {
        char *path = *(char **)(data + 0x148);
        hero = (char *)&D_0013F3D0;
        inside = FUN_L00_00259740(hero, path + 0x10, *(int *)path) != 0;
        if (vector_distance_xy(moby + 0x10, hero) < 30.0f) {
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
    moby[0xBC] = classify_sharkigator_water_height_gap((char *)moby, data);
    command = moby[0xBC];
    if (old_command != command) {
        next_state =
            ((struct { unsigned char t[3][3]; } *)l07_sharkigator_water_gap_transition_states)
                ->t[old_command][command];
        next_sequence =
            ((struct { unsigned char t[3][3]; } *)l07_sharkigator_water_gap_transition_sequences)
                ->t[old_command][command];
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
        if (0.5f < vector_distance_xy(moby + 0x10, (Vec4 *)&D_L07_00173EE0)) {
            scale = vector_distance_xy(moby + 0x10, (Vec4 *)&D_L07_00173EE0);
            scale += scale;
        } else {
            scale = 1.0f;
        }
        if (D_L07_0015F5CC % 45 == ((short)MOBY(moby)->save_id) && random_integer_below(2) == 0) {
            func_L00_002A5158_30D8E8(
                D_L07_001612D0, D_L07_001612D8, 1, F(moby, 0x10), F(moby, 0x14), scale, height);
        }
        if (D_L07_0015F5CC % ((rand() & 7) + 20) == 0) {
            float size = random_float_between(1.5f, 2.4f);
            point[0] += random_float_between(-0.15f, 0.15f);
            point[1] += random_float_between(-0.15f, 0.15f);
            point[2] = resolve_camera_surface_height((Vec4 *)&D_L07_00173EE0, 0);
            spawn_moving_resizing_height_snapped_alpha_fade_particle(point, 0, -1, size, 5250.0f);
        }
    }
    if (1.0f <= F(data, 0x14C)) {
        *(u128 *)&D_0013F3D0 = *(u128 *)(moby + 0x10);
    }

    switch (moby[0x20]) {
    case SHARKIGATOR_INITIALIZE:
        DebugPrint_alt(sharkigator_unexpected_init_message);
        goto end;
    case SHARKIGATOR_SWIM_IDLE:
        if ((moby[0x70] & 2) && moby[0x53] != 0) {
            blend_moby_animation(moby, 0, 0, 10);
        }
        if (inside && target.moby != 0 && target.moby == *(int *)&D_001413D0 &&
            !test_other_group_moby_state_range(moby, 4, 6)) {
            if (moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 10);
            }
            moby[0x20] = 4;
        }
        if (target.moby != 0 && target.moby == *(int *)&D_001413D0 &&
            test_sharkigator_target_sphere_collision(moby, target.moby, 1.5f)) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, 2);
            }
            moby[0x20] = 5;
        } else if (wander) {
            if (moby[0x53] != 1) {
                blend_moby_animation(moby, 1, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 200 + 300;
            F(data, 0x13C) = random_angle_radians();
            moby[0x20] = 2;
        }
        fast_vec_scale(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        advance_sharkigator_swim_movement(
            (char *)moby, data, frame_time_sq * 15.0f, frame_time_sq * 50.0f, frame_time * 8.0f);
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case SHARKIGATOR_SWIM_WANDER:
        if ((moby[0x70] & 2) && moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        if (inside && target.moby != 0 && target.moby == *(int *)&D_001413D0) {
            if (!test_other_group_moby_state_range(moby, 4, 6)) {
                if (moby[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, 10);
                }
                moby[0x20] = 4;
            }
        } else {
            float stiffness = 1.74532926f;
            float damping = 4.5378561f;
            FUN_L00_00258278((char *)moby,
                             (float *)(data + 0x120),
                             F(data, 0x13C),
                             frame_time_sq * stiffness,
                             frame_time_sq * damping,
                             frame_time * stiffness);
            F(moby, 0x44) = spring_turn((float *)(data + 0x124),
                                        F(moby, 0x44),
                                        0.0f,
                                        frame_time_sq * stiffness,
                                        frame_time_sq * damping,
                                        frame_time * stiffness);
            if (wander && is_point_inside_clip_volume(moby + 0x10, *(int *)(data + 0x114))) {
                if (moby[0x53] != 0) {
                    blend_moby_animation(moby, 0, 0, 10);
                }
                *(int *)(data + 0x134) = rand() % 120 + 60;
                moby[0x20] = 1;
            }
        }
        select_sharkigator_navigation_heading((struct Moby *)moby, (struct WalkerData *)data);
        if (update_sharkigator_horizontal_velocity_by_heading((char *)moby,
                                                              data,
                                                              F(data, 0x13C),
                                                              0.209439516f,
                                                              frame_time_sq * 6.0f,
                                                              0.52359879f,
                                                              frame_time_sq * 4.0f,
                                                              frame_time * 7.0f,
                                                              frame_time * 3.0f) &&
            wander) {
            if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 120 + 60;
            moby[0x20] = 1;
        }
        if (target.moby != 0 && target.moby == *(int *)&D_001413D0 &&
            test_sharkigator_target_sphere_collision(moby, target.moby, 1.5f)) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 0, 2);
            }
            moby[0x20] = 5;
        }
        advance_sharkigator_swim_movement(
            (char *)moby, data, frame_time_sq * 15.0f, frame_time_sq * 50.0f, frame_time * 8.0f);
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case SHARKIGATOR_TARGET_PURSUIT: {
        float angle;
        float pitch;
        int   close = 1;
        float pi4 = 12.566371f;
        float three = 3.0f;
        float zero = 0.0f;
        angle = fast_arc_tan(target.pos[0] - F(moby, 0x10), target.pos[1] - F(moby, 0x14));
        FUN_L00_00258278((char *)moby,
                         (float *)(data + 0x120),
                         angle,
                         frame_time_sq * pi4,
                         frame_time_sq * zero,
                         frame_time * pi4);
        pitch = vector_distance_xy(moby + 0x10, &target);
        pitch = fast_arc_tan(pitch, target.pos[2] - F(moby, 0x18));
        F(moby, 0x44) = spring_turn((float *)(data + 0x124),
                                    F(moby, 0x44),
                                    -pitch,
                                    frame_time_sq * pi4,
                                    frame_time_sq * zero,
                                    frame_time * pi4);
        if (!(vector_distance_xy(moby + 0x10, ((char *)&D_0013F3D0)) < three)) {
            close = 0;
        }
        if (close) {
            fast_vec_scale(data + 0xD0, data + 0xD0, frame_scale * (0.9f - 1.0f) + 1.0f);
        }
        if (update_sharkigator_horizontal_velocity_by_heading((char *)moby,
                                                              data,
                                                              angle,
                                                              0.209439516f,
                                                              frame_time_sq * 20.0f,
                                                              0.52359879f,
                                                              frame_time_sq * three,
                                                              frame_time * 20.0f,
                                                              frame_time * 5.0f) &&
            close) {
            if (moby[0x53] != 2) {
                blend_moby_animation(moby, 2, 4, 2);
            }
            MOBY(moby)->collision = 0;
            moby[0x20] = 5;
            F(data, 0x14C) = zero;
            qcopy(data + 0x100, moby + 0x10);
        }
        ((struct {
            char  pad[0x138];
            float f;
        } *)data)
            ->f = target.pos[2] - 0.25f;
        advance_sharkigator_swim_movement(
            (char *)moby, data, frame_time_sq * 38.0f, frame_time_sq * 0.0f, frame_time * 38.0f);
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
    case SHARKIGATOR_TARGET_LUNGE_INTERPOLATION: {
        float angle;
        char *mpos;
        char *start;
        float one;
        FUN_L00_0024f7c8(moby, 0, point);
        mpos = (char *)moby + 0x10;
        fast_vec_sub(offset, point, mpos);
        angle = fast_arc_tan(target.pos[0] - point[0], target.pos[1] - point[1]);
        func_L00_002592B0_t((char *)moby,
                            angle,
                            (float *)(data + 0x120),
                            frame_time_sq * 12.566371f,
                            frame_time_sq * 0.0f,
                            frame_time * 12.566371f);
        F(moby, 0x44) *= frame_scale * 0.7f;
        F(data, 0x14C) += 0.028f;
        qcopy(previous, mpos);
        fast_vec_sub(goal, &target, offset);
        start = data + 0x100;
        fast_vec_sub(delta, goal, start);
        one = 1.0f;
        fast_vec_scale(delta, delta, F(data, 0x14C) < one ? F(data, 0x14C) : one);
        fast_vec_add(mpos, delta, start);
        noop_main_callback(previous, mpos);
        noop_main_callback(start, mpos);
        fast_vec_sub(data + 0xD0, mpos, previous);
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
    case SHARKIGATOR_POST_LUNGE_PATH_FOLLOW: {
        float seven = 7.0f;
        float pi = 3.14159274f;
        float pi2 = 1.57079637f;
        select_sharkigator_navigation_heading((struct Moby *)moby, (struct WalkerData *)data);
        update_sharkigator_horizontal_velocity_by_heading((char *)moby,
                                                          data,
                                                          F(data, 0x13C),
                                                          0.209439516f,
                                                          frame_time_sq * 6.0f,
                                                          0.52359879f,
                                                          frame_time_sq * 4.0f,
                                                          frame_time * seven,
                                                          frame_time * 3.0f);
        advance_sharkigator_swim_movement(
            (char *)moby, data, frame_time_sq * 12.0f, frame_time_sq * 50.0f, frame_time * seven);
        FUN_L00_00258278((char *)moby,
                         (float *)(data + 0x120),
                         F(data, 0x13C),
                         frame_time_sq * pi2,
                         frame_time_sq * pi,
                         frame_time * pi);
        F(moby, 0x44) = spring_turn((float *)(data + 0x124),
                                    F(moby, 0x44),
                                    0.0f,
                                    frame_time_sq * pi2,
                                    frame_time_sq * pi,
                                    frame_time * pi);
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
    case SHARKIGATOR_GRAVITY_IDLE:
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
            F(data, 0x13C) = random_angle_radians();
            moby[0x20] = 8;
        }
        fast_vec_scale(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        F(data, 0xD8) -= frame_time * 9.8f;
        if (move_moby_with_collision_integer_radius(
                (int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f)) {
            fast_vec_scale(data + 0xD0, data + 0xD0, 0.5f);
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case SHARKIGATOR_GRAVITY_WANDER:
        if (inside) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            moby[0x20] = 9;
        } else {
            float stiffness = 0.785398185f;
            float damping = 1.57079637f;
            FUN_L00_00258278((char *)moby,
                             (float *)(data + 0x120),
                             F(data, 0x13C),
                             frame_time_sq * stiffness,
                             frame_time_sq * damping,
                             frame_time * stiffness);
            F(moby, 0x44) = spring_turn((float *)(data + 0x124),
                                        F(moby, 0x44),
                                        0.0f,
                                        frame_time_sq * stiffness,
                                        frame_time_sq * damping,
                                        frame_time * stiffness);
            if (wander && is_point_inside_clip_volume(moby + 0x10, *(int *)(data + 0x114))) {
                if (moby[0x53] != 3) {
                    blend_moby_animation(moby, 3, 0, 10);
                }
                *(int *)(data + 0x134) = rand() % 120 + 60;
                moby[0x20] = 7;
            }
        }
        select_sharkigator_navigation_heading((struct Moby *)moby, (struct WalkerData *)data);
        if (update_sharkigator_horizontal_velocity_by_heading((char *)moby,
                                                              data,
                                                              F(data, 0x13C),
                                                              0.34906584f,
                                                              frame_time_sq * 3.0f,
                                                              1.04719758f,
                                                              frame_time_sq * 9.0f,
                                                              frame_time,
                                                              frame_time) &&
            wander) {
            if (moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, 10);
            }
            *(int *)(data + 0x134) = rand() % 120 + 60;
            moby[0x20] = 7;
        }
        F(data, 0xD8) -= frame_time * 9.8f;
        if (move_moby_with_collision_integer_radius(
                (int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f)) {
            fast_vec_scale(data + 0xD0, data + 0xD0, 0.5f);
        }
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case SHARKIGATOR_TARGET_APPROACH:
    case SHARKIGATOR_TARGET_CONTACT_ATTACK: {
        float angle;
        float pi2 = 1.57079637f;
        angle = fast_arc_tan(target.pos[0] - F(moby, 0x10), target.pos[1] - F(moby, 0x14));
        func_L00_002592B0_t((char *)moby,
                            angle,
                            (float *)(data + 0x120),
                            frame_time_sq * 0.785398185f,
                            frame_time_sq * pi2,
                            frame_time * 0.785398185f);
        if (update_sharkigator_horizontal_velocity_by_heading((char *)moby,
                                                              data,
                                                              angle,
                                                              0.52359879f,
                                                              frame_time_sq * 4.0f,
                                                              pi2,
                                                              frame_time_sq * 9.0f,
                                                              frame_time * 1.5f,
                                                              frame_time) &&
            vector_distance_xy(moby + 0x10, &target) < 1.9f && moby[0x20] != 10) {
            if (moby[0x53] != 5) {
                blend_moby_animation(moby, 5, 0, 2);
            }
            moby[0x20] = 10;
        }
        F(data, 0xD8) -= frame_time * 9.8f;
        if (move_moby_with_collision_integer_radius(
                (int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f)) {
            fast_vec_scale(data + 0xD0, data + 0xD0, 0.5f);
        }
        data[0x58] = truncate_float_to_s32(8.0f);
        data[0x5A] = truncate_float_to_s32(9.6f);
        if (!inside) {
            if (moby[0x53] != 4) {
                blend_moby_animation(moby, 4, 0, 10);
            }
            moby[0x20] = 8;
        }
        ((void (*)(char *, float, char *, float, int, float, int, float, float, int, int, int, int))
             handle_sharkigator_contact_attack)(
            (char *)moby, 17.0f, data, 20.0f, 10, 1.0f, 5, 1.0f, 1.0f, 4, 4, 11, target.moby);
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
    case SHARKIGATOR_TARGET_CONTACT_RECOIL_RECOVERY:
        if (moby[0x70] & 2) {
            moby[0x20] = 7;
        }
        goto end;
    case SHARKIGATOR_HIT_KNOCKBACK_RECOVERY:
        if (step_moby_motion(moby, data + 0x70) & 1) {
            ((void (*)(char *, char *))set_sharkigator_idle_state_from_water_height_gap)(
                (char *)moby, data);
        }
        goto end;
    case SHARKIGATOR_HIT_ANIMATION_RECOVERY:
        if (moby[0x70] & 2) {
            ((void (*)(char *, char *))set_sharkigator_idle_state_from_water_height_gap)(
                (char *)moby, data);
        }
        goto end;
    case SHARKIGATOR_DEATH_FALL_AND_REMOVE:
        if (step_moby_motion(moby, data + 0x70) & (moby[0x70] & 2)) {
            spawn_explosion(moby,
                            data + 0xD0,
                            0,
                            0.0f,
                            0.0f,
                            3,
                            3,
                            5,
                            2.0f,
                            1.0f,
                            4.0f,
                            1.0f,
                            -1,
                            7.0f,
                            0,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        goto end;
    case SHARKIGATOR_DAMPED_GRAVITY_RECOVERY:
        fast_vec_scale(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        F(data, 0xD8) -= frame_time * 9.8f;
        move_moby_with_collision_integer_radius(
            (int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f);
        if (moby[0x70] & 2) {
            ((void (*)(char *, char *))set_sharkigator_idle_state_from_water_height_gap)(
                (char *)moby, data);
        }
        goto end;
    case SHARKIGATOR_SMALL_WATER_GAP_GRAVITY_IDLE:
        fast_vec_scale(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        F(data, 0xD8) -= frame_time * 9.8f;
        move_moby_with_collision_integer_radius(
            (int)moby, (int)(data + 0xD0), 0x400, 0x10, 1.0f, 0.0f);
        if (next_state != 0xFF) {
            if (moby[0x53] != next_sequence) {
                blend_moby_animation(moby, next_sequence, 0, 6);
            }
            moby[0x20] = next_state;
        }
        goto end;
    case SHARKIGATOR_DAMPED_RECOVERY:
        fast_vec_scale(data + 0xD0, data + 0xD0, frame_scale * (0.95f - 1.0f) + 1.0f);
        if (moby[0x70] & 2) {
            ((void (*)(char *, char *))set_sharkigator_idle_state_from_water_height_gap)(
                (char *)moby, data);
        }
        goto end;
    default:
        goto end;
    }
end:
    if (F(moby, 0x10) < 2.0f || 1021.0f < F(moby, 0x10) || F(moby, 0x14) < 2.0f ||
        1021.0f < F(moby, 0x14) || F(moby, 0x18) < 2.0f || 1021.0f < F(moby, 0x18)) {
        mark_moby_for_removal(moby);
    }
}
