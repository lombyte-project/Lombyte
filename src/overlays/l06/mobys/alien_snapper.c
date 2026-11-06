/* Level 06 moby class 0x418: alien snapper. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"

#include "rnc/rendering/clip_volume.h"

enum {
    ALIEN_SNAPPER_STATE_INITIALIZE = 0,
    ALIEN_SNAPPER_STATE_WAIT_FOR_ACTIVATION = 1,
    ALIEN_SNAPPER_STATE_ACTIVATION_SEQUENCE = 2,
    ALIEN_SNAPPER_STATE_WAIT_FOR_CLIP_TRIGGER = 3,
    ALIEN_SNAPPER_STATE_PREPARE_HOME_NODE_JUMP = 4,
    ALIEN_SNAPPER_STATE_JUMP_TO_HOME_NODE = 5,
    ALIEN_SNAPPER_STATE_FACE_GOAL_AND_WAIT = 6,
    ALIEN_SNAPPER_STATE_WAIT_TO_APPROACH = 7,
    ALIEN_SNAPPER_STATE_APPROACH_TARGET = 8,
    ALIEN_SNAPPER_STATE_REPOSITION_AROUND_TARGET = 9,
    ALIEN_SNAPPER_STATE_WAIT_FOR_REPOSITION_ANIMATION = 10,
    ALIEN_SNAPPER_STATE_BITE_TARGET = 11,
    ALIEN_SNAPPER_STATE_RETURN_HOME = 12,
    ALIEN_SNAPPER_STATE_RETREAT_FROM_TARGET = 13,
    ALIEN_SNAPPER_STATE_HIT_KNOCKBACK = 14,
    ALIEN_SNAPPER_STATE_FACE_HERO = 15,
    ALIEN_SNAPPER_STATE_BITE_HERO = 16,
    ALIEN_SNAPPER_STATE_DEATH_MOTION = 17
};

extern int D_L06_001ABFC0[];

/* 0x002f9698, 96 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
void transition_moby_group_state_1_to_2(char *group_source_moby) __asm__("FUN_L06_002f9698");

/* angle from a moby to its owner, clamped to the gaps between obstacles in its list */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FAB28), where it is exact; names translated to the US level program. */
extern char  *level_mobys_q __asm__("D_L06_0015FFD8");
extern char   D_0013E533[];
extern short *D_L06_001ABFC0_p[] __asm__("D_L06_001ABFC0");
extern short  alien_snapper_gap_angle_limit_degrees_gp __asm__("D_L06_00161ED8")
    __attribute__((sda));
float compute_alien_snapper_target_gap_angle(char *self) __asm__("FUN_L06_002f96f8");

#include "qcopy.h"

/* raise each listed moby's position z by a fixed amount and notify it */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC4C0), where it is exact; names translated to the US level program. */
extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Registers the moby's draw callbacks, choosing by its four slot ids. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FE4B8), where it is exact; names translated to the US level program. */

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002B5990), where it is exact; names translated to the US level program. */
#include "rnc/overlay/shared/world/cuboid.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002EB140), where it is exact; names translated to the US level program. */

/* Sparks thrown off the swinging hazard while the camera is near it. */
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern f32 scale_time(f32) __asm__("FUN_001f96b0");

/* Level 6 drawbridge (class 1028): spawns its two rows of deck pieces, waits for the hero's switch, then
   lowers through three hinge stages (D_L06_00161E08/0C/10) while the camera follows the path nodes. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F69D0), where it is exact; names translated to the US level program. */
typedef struct Hero Hero;
typedef struct Moby Moby;
extern char         D_0013F350[];

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F91A8), where it is exact; names translated to the US level program. */
typedef struct {
    u8    pad0[0x20];
    f32   health;     /* 0x20 */
    s16   max_health; /* 0x24 */
    u8    pad26[2];
    u8    hit_response_class; /* 0x28 */
    u8    f29;                /* 0x29 */
    u8    pad2A[0x2E];
    u8    f58; /* 0x58 */
    u8    pad59;
    u8    f5A; /* 0x5A */
    u8    pad5B[0x15];
    u8    f70[0x10];                    /* 0x70 */
    f32   gravity_step;                 /* 0x80 */
    f32   motion_speed_adjustment_step; /* 0x84 */
    f32   return_speed;                 /* 0x88 */
    f32   f8C;                          /* 0x8C */
    s32   collision_radius_fixed;       /* 0x90 */
    s32   motion_flags;                 /* 0x94 */
    f32   collision_height_offset;      /* 0x98 */
    u8    pad9C[0x11];
    u8    fAD; /* 0xAD */
    u8    padAE[0x22];
    s32   movement;                 /* 0xD0 */
    f32   fD4;                      /* 0xD4 */
    f32   max_ground_drop_distance; /* 0xD8 */
    f32   max_ground_rise_distance; /* 0xDC */
    u8    padE0[0x14];
    f32   speed; /* 0xF4 */
    u8    padF8[0x28];
    f32   goal_x; /* 0x120 */
    f32   goal_y; /* 0x124 */
    u8    pad128[0x38];
    Moby *target; /* 0x160 */
    s32   f164;   /* 0x164 */
    u8    pad168[8];
    Vec4f home; /* 0x170 */
    u8    pad180[0x10];
    f32   turn_speed;     /* 0x190 */
    f32   angle_jitter;   /* 0x194 */
    f32   jitter_degrees; /* 0x198 */
    u8    pad19C[4];
    f32   f1A0;                   /* 0x1A0 */
    f32   base_speed;             /* 0x1A4 */
    s32   timer;                  /* 0x1A8 */
    s32   f1AC;                   /* 0x1AC */
    s32   path;                   /* 0x1B0 */
    s32   activate_on_initialize; /* 0x1B4 */
    s32   timer_max;              /* 0x1B8 */
    s32   node;                   /* 0x1BC */
    s32   clip_volume;            /* 0x1C0 */
    f32   reach_distance;         /* 0x1C4 */
    f32   f1C8;                   /* 0x1C8 */
    s32   f1CC;                   /* 0x1CC */
    u8    pad1D0[0x10];
    u8    big_head_manipulator[0x10]; /* 0x1E0 */
} SnapperVars_91A8;

typedef struct {
    f32   dir[4]; /* 0x00 */
    Moby *moby;   /* 0x10 */
    s32   f14;    /* 0x14 */
    s32   f18;    /* 0x18 */
    f32   f1C;    /* 0x1C */
    s32   f20;    /* 0x20 */
    u8    pad24[0xC];
    f32   push[4]; /* 0x30 */
} SnapperBite_91A8;

extern float D_0015ED70;
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float D_L06_00161EB0 __attribute__((sda));
extern float l06_alien_snapper_gravity __asm__("D_L06_00161EB4") __attribute__((sda));
extern float l06_alien_snapper_bite_approach_radius __asm__("D_L06_00161EC4") __attribute__((sda));
extern float l06_alien_snapper_max_home_distance __asm__("D_L06_00161EC8") __attribute__((sda));
extern float l06_alien_snapper_base_targeting_radius __asm__("D_L06_00161ED0") __attribute__((sda));
extern float l06_alien_snapper_retreat_distance_limit __asm__("D_L06_00161ED4")
    __attribute__((sda));
extern int l06_alien_snapper_retreat_bite_override_enabled __asm__("D_L06_00161EDC")
    __attribute__((sda));
extern int l06_alien_snapper_retreat_bite_start_frame __asm__("D_L06_00161EE0")
    __attribute__((sda));
void        update_alien_snapper_hit_response_and_targeting(Moby *moby) __asm__("FUN_L06_002f91e8");
extern char D_0013F3D0[];
extern void update_alien_snapper_hit_response_and_targeting(Moby *) __asm__("FUN_L06_002f91e8");
extern void func_L00_00264B40_91A8(float, Moby *, int, void *) __asm__("FUN_L00_00263ac8");
extern void func_L00_0025B178_91A8(Moby *) __asm__("FUN_L00_0025a120");
extern void func_L01_0026E8E0_91A8(void *) __asm__("FUN_L01_0026d930");
extern void func_L06_002FAAC8_91A8(Moby *) __asm__("FUN_L06_002f9698");
extern float
func_L00_0025BC48_91A8(void *, void *, void *, float, float) __asm__("FUN_L00_0025abf0");
extern void
           func_L00_0025D5B0_91A8(Moby *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern int func_L00_0025D6F0_91A8(Moby *, void *) __asm__("FUN_L00_0025c698");
extern void
func_L00_002592B0_91A8(Moby *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern float func_L06_002FAB28_91A8(Moby *) __asm__("FUN_L06_002f96f8");
extern int   move_moby_with_ground_and_collision_checks(
      Moby *, void *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern void  func_L00_00262DF0_91A8(float, int, void *, void *) __asm__("FUN_L00_00261d78");
extern void  func_L00_00262DF0_b91A8(int, void *, void *, float) __asm__("FUN_L00_00261d78");
extern float compute_interpolated_record_value(Moby *) __asm__("FUN_0020c9e0");
extern void  func_L00_00250800_91A8(Moby *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int   func_L00_001F2BE8_91A8(float, void *, int, Moby *, void *) __asm__("FUN_L00_001f2868");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern void  func_L00_00260460_91A8(Moby *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern float
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern char D_L06_00167540_macro[] __asm__("D_L06_00167540");
extern int *D_L06_001B0C30_macro[] __asm__("D_L06_001B0C30");
void        alien_snapper_update(Moby *moby) __asm__("FUN_L06_002f7d78");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FA618), where it is exact; names translated to the US level program. */
typedef struct {
    char         pad00[0x10];
    float        x; /* 0x10 */
    float        y; /* 0x14 */
    float        z; /* 0x18 */
    char         pad1C[0x4];
    struct Moby *owner; /* 0x20 */
} Target;

typedef struct BlargVars {
    char                 pad00[0x20];
    float                health; /* 0x20 */
    char                 pad24[0x14];
    int                  targeting_boost_refresh_pending; /* 0x38 */
    char                 pad3C[0x24];
    struct HitColorFlash hit_flash;
    char                 f70[0x18];               /* 0x70 */
    float                horizontal_motion_speed; /* 0x88 */
    float                f8C;                     /* 0x8C */
    char                 pad90[0x30];
    float                motion_apex_animation_frame;    /* 0xC0 */
    float                motion_landing_animation_frame; /* 0xC4 */
    char                 padC8[0x58];
    char                 selected_target_xy[0x8]; /* 0x120 */
    float                selected_target_z;       /* 0x128 */
    char                 pad12C[0x34];
    int                  selected_target_moby; /* 0x160 */
    int                  f164;                 /* 0x164 */
    char                 pad168[0x8];
    char                 home_position[0x30]; /* 0x170 */
    float                f1A0;                /* 0x1A0 */
    char                 pad1A4[0x8];
    int                  f1AC;                /* 0x1AC */
    int                  movement_path_index; /* 0x1B0 */
    char                 pad1B4[0x14];
    float                f1C8; /* 0x1C8 */
    int                  f1CC; /* 0x1CC */
} BlargVars;

extern int level_frame_count __asm__("D_L06_0015F5CC");

/* no foreign declaration */
extern int D_001413D0[];

#include "rnc/overlay/l06/mobys/alien_snapper_data/update_alien_snapper_hit_response_and_targeting.h"

int l06_alien_snapper_glow_gray_color __asm__("D_L06_00161EE8") = 0x80808080;

int l06_alien_snapper_glow_tinted_color __asm__("D_L06_00161EEC") = 0x80604040;

extern int     resolve_moby_hit_response() __asm__("FUN_00213928");
void           start_hit_color_flash(void *a, void *s) __asm__("FUN_L00_0025d458");
extern float   func_001F9878_2FA618(float) __asm__("FUN_001f96b0");
extern Target *func_L00_0025B478_2FA618(void *, int, int) __asm__("FUN_L00_0025a420");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void func_L06_002FAAC8_2FA618(void *) __asm__("FUN_L06_002f9698");
extern void
func_L00_0025D5B0_2FA618(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void func_L00_0025A8E8_2FA618(
    void *, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern int func_001FA8A8_2FA618(int, int, float) __asm__("FUN_001fa6e0");
extern int
activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern int *D_L06_001B0FB0_2FA618[] __asm__("D_L06_001B0C30");

#include "sda.h"

extern f32
approach_angle(f32 *angle, f32 target, f32 *vel, f32, f32, f32) __asm__("FUN_L00_0025be00");

/* Moby class 1048 */
void alien_snapper_update(Moby *moby) {
    enum {
        ALIEN_SNAPPER_ANIM_ACTIVATION = 3,
        ALIEN_SNAPPER_ANIM_HOME_NODE_JUMP_WINDUP = 6
    };
    SnapperVars_91A8 *snapper_behavior = (SnapperVars_91A8 *)moby->pvars;
    Moby             *target;
    float             target_goal_radius;
    float             target_gap_angle_radians;
    float             saved_rot;
    int               start_frame;
    union {
        SnapperBite_91A8 bite;
        struct {
            float dir[4];
            float out[4];
            float goal[4];
            float path_point[4];
        } m;
    } scratch;
    float bite_query_position[4];

    update_alien_snapper_hit_response_and_targeting(moby);
    func_L00_00264B40_91A8(2.3f, moby, 1, snapper_behavior->big_head_manipulator);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L06_00167540_macro) < 27.0f) {
        func_L00_0025B178_91A8(moby);
        moby->shadow_range = 0x15;
    }
    target = snapper_behavior->target;
    moby->scale = moby->pclass->scale * D_L06_00161EB0;
    switch (moby->state) {
    case ALIEN_SNAPPER_STATE_INITIALIZE:
        snapper_behavior->hit_response_class = 3;
        snapper_behavior->f29 = 1;
        snapper_behavior->health = 4.0f;
        snapper_behavior->max_health = 4;
        snapper_behavior->f58 = truncate_float_to_s32(10.0f);
        snapper_behavior->f5A = truncate_float_to_s32(14.0f);
        moby->flags |= 0x5000;
        qcopy(&snapper_behavior->home, &moby->pos);
        snapper_behavior->reach_distance = random_float_between(0.0f, 1.0f);
        func_L01_0026E8E0_91A8(&snapper_behavior->movement);
        snapper_behavior->max_ground_rise_distance = 2.0f;
        snapper_behavior->max_ground_drop_distance = 2.0f;
        snapper_behavior->movement = truncate_float_to_s32(D_L06_00161EB0 * 1024.0f);
        snapper_behavior->fD4 = D_L06_00161EB0;
        snapper_behavior->speed = snapper_behavior->base_speed * frame_time;
        snapper_behavior->collision_radius_fixed = truncate_float_to_s32(768.0f);
        snapper_behavior->collision_height_offset = 0.75f;
        snapper_behavior->gravity_step = l06_alien_snapper_gravity * D_0015ED70;
        snapper_behavior->fAD = 0;
        snapper_behavior->motion_speed_adjustment_step = 0.0005f;
        snapper_behavior->motion_flags = 9;
        snapper_behavior->f1C8 = random_angle_radians();
        if (snapper_behavior->f1CC != 0) {
            moby->state = ALIEN_SNAPPER_STATE_FACE_HERO;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 0);
            }
        } else if (snapper_behavior->clip_volume >= 0) {
            moby->state = ALIEN_SNAPPER_STATE_WAIT_FOR_CLIP_TRIGGER;
        } else if (snapper_behavior->activate_on_initialize != 0) {
            moby->state = ALIEN_SNAPPER_STATE_ACTIVATION_SEQUENCE;
            snapper_behavior->timer = scale_game_frames(1);
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            if (moby->group != 0xFF) {
                func_L06_002FAAC8_91A8(moby);
            }
        } else {
            moby->state = ALIEN_SNAPPER_STATE_WAIT_FOR_ACTIVATION;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            snapper_behavior->f1A0 = l06_alien_snapper_base_targeting_radius;
        }
        break;
    case ALIEN_SNAPPER_STATE_WAIT_FOR_ACTIVATION:
        if (snapper_behavior->f164 != 2) {
            moby->state = ALIEN_SNAPPER_STATE_ACTIVATION_SEQUENCE;
            snapper_behavior->timer = scale_game_frames(1);
            if (moby->group != 0xFF) {
                func_L06_002FAAC8_91A8(moby);
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_ACTIVATION_SEQUENCE:
        if (moby->prev_seq == 0) {
            if (snapper_behavior->timer == 0) {
                snapper_behavior->timer =
                    truncate_float_to_s32(scale_time(random_float_between(0.0f, 20.0f)));
            } else if (fast_dec_timer(&snapper_behavior->timer) != 0) {
                if (moby->prev_seq != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                }
            }
        } else if (moby->anim_flags & 2) {
            if (vector_distance_xy(&moby->pos, ((char *)&D_0013F3D0)) <
                l06_alien_snapper_base_targeting_radius) {
                moby->state = ALIEN_SNAPPER_STATE_APPROACH_TARGET;
                snapper_behavior->angle_jitter =
                    random_float_between(snapper_behavior->jitter_degrees * 0.017453292f,
                                         -(snapper_behavior->jitter_degrees * 0.017453292f));
                if (moby->prev_seq != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            } else {
                int frames = *((u8 *)moby->pclass->seqs[2] + 0x10);

                moby->state = ALIEN_SNAPPER_STATE_FACE_GOAL_AND_WAIT;
                if (moby->prev_seq != 2) {
                    start_frame = random_float_between(0.0f, (float)frames - 1.0f);
                    blend_moby_animation(moby, 2, start_frame, scale_game_frames(30));
                }
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_WAIT_FOR_CLIP_TRIGGER:
        if (snapper_behavior->clip_volume >= 0) {
            if (is_point_inside_clip_volume(((char *)&D_0013F3D0), snapper_behavior->clip_volume) !=
                0) {
                moby->state = ALIEN_SNAPPER_STATE_PREPARE_HOME_NODE_JUMP;
                if (moby->prev_seq != 3) {
                    blend_moby_animation(moby, 3, 0, scale_game_frames(5));
                }
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_PREPARE_HOME_NODE_JUMP:
        if (moby->anim_flags & 2) {
            switch (moby->prev_seq) {
            case ALIEN_SNAPPER_ANIM_ACTIVATION:
                blend_moby_animation(moby, 6, 0, scale_game_frames(5));
                break;
            case ALIEN_SNAPPER_ANIM_HOME_NODE_JUMP_WINDUP:
                moby->state = ALIEN_SNAPPER_STATE_JUMP_TO_HOME_NODE;
                qcopy(&snapper_behavior->home, &l06_cuboids[snapper_behavior->node].pos);
                snapper_behavior->return_speed =
                    vector_distance_xy(&moby->pos, &snapper_behavior->home) /
                    (float)scale_game_frames(60);
                snapper_behavior->f8C = func_L00_0025BC48_91A8(&moby->pos,
                                                               &snapper_behavior->home,
                                                               0,
                                                               snapper_behavior->return_speed,
                                                               -snapper_behavior->gravity_step);
                snapper_behavior->motion_flags = 5;
                snapper_behavior->fAD = 3;
                func_L00_0025D5B0_91A8(moby,
                                       snapper_behavior->f70,
                                       7,
                                       1,
                                       0,
                                       fast_arc_tan(snapper_behavior->home.x - moby->pos.x,
                                                    snapper_behavior->home.y - moby->pos.y));
                if (moby->prev_seq != 7) {
                    blend_moby_animation(moby, 7, 0, 5);
                }
                break;
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_JUMP_TO_HOME_NODE:
        if (func_L00_0025D6F0_91A8(moby, snapper_behavior->f70) & 1) {
            moby->state = ALIEN_SNAPPER_STATE_FACE_GOAL_AND_WAIT;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 5);
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_FACE_GOAL_AND_WAIT:
        func_L00_002592B0_91A8(moby,
                               fast_arc_tan(snapper_behavior->goal_x - moby->pos.x,
                                            snapper_behavior->goal_y - moby->pos.y),
                               &snapper_behavior->turn_speed,
                               0.05f,
                               0.3f,
                               frame_time * 6.2831855f);
        if (snapper_behavior->f164 != 2) {
            moby->state = ALIEN_SNAPPER_STATE_WAIT_TO_APPROACH;
            snapper_behavior->timer = truncate_float_to_s32(
                random_float_between(0.0f, (float)snapper_behavior->timer_max));
        } else if (snapper_behavior->f1AC != 0) {
            moby->state = ALIEN_SNAPPER_STATE_REPOSITION_AROUND_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_REPOSITION_AROUND_TARGET: {
        int near;

        approach_value(
            &snapper_behavior->speed, snapper_behavior->base_speed * frame_time, D_0015ED70 * 6.0f);
        func_L00_002592B0_91A8(
            moby,
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
            &snapper_behavior->turn_speed,
            0.05f,
            0.3f,
            frame_time * 6.2831855f);
        target_goal_radius = vector_distance_xy(&moby->pos, &target->pos);
        if (l06_alien_snapper_bite_approach_radius < target_goal_radius) {
            target_goal_radius = l06_alien_snapper_bite_approach_radius;
        }
        target_gap_angle_radians = func_L06_002FAB28_91A8(moby);
        near = 0;
        scratch.m.goal[0] = fast_cos(target_gap_angle_radians) * target_goal_radius;
        scratch.m.goal[1] = fast_sin(target_gap_angle_radians) * target_goal_radius;
        scratch.m.goal[2] = 0.0f;
        fast_vec_add(scratch.m.goal, scratch.m.goal, &target->pos);
        saved_rot = moby->rot.z;
        moby->rot.z =
            fast_arc_tan(scratch.m.goal[0] - moby->pos.x, scratch.m.goal[1] - moby->pos.y);
        scratch.m.dir[0] = fast_cos(moby->rot.z) * 2.0f;
        scratch.m.dir[1] = fast_sin(moby->rot.z) * 2.0f;
        scratch.m.dir[2] = 0.0f;
        move_moby_with_ground_and_collision_checks(
            moby, &snapper_behavior->movement, scratch.m.dir, scratch.m.out, 1.0f);
        moby->rot.z = saved_rot;
        func_L00_00262DF0_91A8(0.5f, snapper_behavior->path, &moby->pos, &moby->pos);
        if (path_8808(snapper_behavior->path, &moby->pos, &target->pos, scratch.m.path_point) !=
                0 &&
            fast_vec_dist(&moby->pos, scratch.m.path_point) < snapper_behavior->reach_distance) {
            near = 1;
        }
        if (snapper_behavior->f1AC == 0) {
            moby->state = ALIEN_SNAPPER_STATE_RETURN_HOME;
        } else if (near) {
            moby->state = ALIEN_SNAPPER_STATE_WAIT_FOR_REPOSITION_ANIMATION;
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
        } else if (vector_distance_xy(&moby->pos, &target->pos) <
                   l06_alien_snapper_bite_approach_radius) {
            moby->state = ALIEN_SNAPPER_STATE_BITE_TARGET;
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
        }
        break;
    }
    case ALIEN_SNAPPER_STATE_WAIT_FOR_REPOSITION_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = ALIEN_SNAPPER_STATE_REPOSITION_AROUND_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_WAIT_TO_APPROACH:
        if (fast_dec_timer(&snapper_behavior->timer) != 0) {
            moby->state = ALIEN_SNAPPER_STATE_APPROACH_TARGET;
            snapper_behavior->angle_jitter =
                random_float_between(snapper_behavior->jitter_degrees * 0.017453292f,
                                     -(snapper_behavior->jitter_degrees * 0.017453292f));
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_APPROACH_TARGET: {
        if (l06_alien_snapper_bite_approach_radius < vector_distance_xy(&moby->pos, &target->pos)) {
            approach_value(&snapper_behavior->speed,
                           snapper_behavior->base_speed * frame_time,
                           D_0015ED70 * 6.0f);
        } else {
            approach_value(&snapper_behavior->speed, 0.0f, D_0015ED70 * 20.0f);
        }
        func_L00_002592B0_91A8(
            moby,
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
            &snapper_behavior->turn_speed,
            0.05f,
            0.3f,
            frame_time * 6.2831855f);
        target_goal_radius = vector_distance_xy(&moby->pos, &target->pos);
        if (l06_alien_snapper_bite_approach_radius < target_goal_radius) {
            target_goal_radius = l06_alien_snapper_bite_approach_radius;
        }
        target_gap_angle_radians = func_L06_002FAB28_91A8(moby);
        scratch.m.goal[0] = fast_cos(target_gap_angle_radians) * target_goal_radius;
        scratch.m.goal[1] = fast_sin(target_gap_angle_radians) * target_goal_radius;
        scratch.m.goal[2] = 0.0f;
        fast_vec_add(scratch.m.goal, scratch.m.goal, &target->pos);
        saved_rot = moby->rot.z;
        moby->rot.z =
            fast_arc_tan(scratch.m.goal[0] - moby->pos.x, scratch.m.goal[1] - moby->pos.y);
        scratch.m.dir[0] = fast_cos(moby->rot.z) * 2.0f;
        scratch.m.dir[1] = fast_sin(moby->rot.z) * 2.0f;
        scratch.m.dir[2] = 0.0f;
        move_moby_with_ground_and_collision_checks(
            moby, &snapper_behavior->movement, scratch.m.dir, scratch.m.out, 1.0f);
        moby->rot.z = saved_rot;
        func_L00_00262DF0_91A8(0.5f, snapper_behavior->path, &moby->pos, &moby->pos);
        if (l06_alien_snapper_max_home_distance <
                fast_vec_dist(&moby->pos, &snapper_behavior->home) ||
            snapper_behavior->f164 == 2) {
            moby->state = ALIEN_SNAPPER_STATE_RETURN_HOME;
        } else if (vector_distance_xy(&moby->pos, &target->pos) <
                   l06_alien_snapper_bite_approach_radius) {
            if (absolute_float(target->pos.z - moby->pos.z) < 1.0f) {
                char *hero = ((char *)&D_0013F350);

                if (*(int *)(hero + 0x1C0) < scale_game_frames(10)) {
                    moby->state = ALIEN_SNAPPER_STATE_BITE_TARGET;
                    if (moby->prev_seq != 4) {
                        blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                    }
                }
            }
        }
        break;
    }
    case ALIEN_SNAPPER_STATE_BITE_TARGET:
        func_L00_002592B0_91A8(
            moby,
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
            &snapper_behavior->turn_speed,
            0.05f,
            0.3f,
            frame_time * 6.2831855f);
        if (1.0f < compute_interpolated_record_value(moby) &&
            compute_interpolated_record_value(moby) < 8.0f) {
            scratch.bite.push[0] = fast_cos(moby->rot.z) * 0.2f;
            scratch.bite.push[1] = fast_sin(moby->rot.z) * 0.2f;
            scratch.bite.push[2] = 0.0f;
            func_L00_00250800_91A8(moby, 0, bite_query_position);
            scratch.bite.moby = moby;
            scratch.bite.f14 = 1;
            scratch.bite.f20 = 1;
            scratch.bite.f1C = 1.0f;
            scratch.bite.dir[0] = fast_cos(moby->rot.z);
            scratch.bite.dir[1] = fast_sin(moby->rot.z);
            scratch.bite.dir[2] = 0.0f;
            func_L00_001F2BE8_91A8(0.5f, bite_query_position, 0, moby, &scratch.bite);
        } else if (moby->anim_flags & 2) {
            moby->state = ALIEN_SNAPPER_STATE_APPROACH_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case ALIEN_SNAPPER_STATE_RETREAT_FROM_TARGET: {
        approach_value(&snapper_behavior->speed,
                       snapper_behavior->base_speed * 3.0f * frame_time,
                       D_0015ED70 * 20.0f);
        func_L00_002592B0_91A8(
            moby,
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
            &snapper_behavior->turn_speed,
            0.05f,
            0.3f,
            frame_time * 6.2831855f);
        saved_rot = moby->rot.z;
        moby->rot.z = fast_add_rotations(
            fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), 3.14159f);
        scratch.m.dir[0] = fast_cos(moby->rot.z) * 2.0f;
        scratch.m.dir[1] = fast_sin(moby->rot.z) * 2.0f;
        scratch.m.dir[2] = 0.0f;
        move_moby_with_ground_and_collision_checks(
            moby, &snapper_behavior->movement, scratch.m.dir, scratch.m.out, 1.0f);
        moby->rot.z = saved_rot;
        func_L00_00262DF0_91A8(0.5f, snapper_behavior->path, &moby->pos, &moby->pos);
        if (fast_dec_timer(&snapper_behavior->timer) != 0 ||
            l06_alien_snapper_retreat_distance_limit < fast_vec_dist(&moby->pos, &target->pos)) {
            moby->state = ALIEN_SNAPPER_STATE_BITE_TARGET;
            snapper_behavior->speed = frame_time * 5.0f;
            if (l06_alien_snapper_retreat_bite_override_enabled != 0) {
                if (moby->prev_seq != 4) {
                    blend_moby_animation(
                        moby, 4, l06_alien_snapper_retreat_bite_start_frame, scale_game_frames(5));
                }
            } else {
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 10, scale_game_frames(10));
                }
            }
        }
        break;
    }
    case ALIEN_SNAPPER_STATE_RETURN_HOME: {
        approach_value(
            &snapper_behavior->speed, snapper_behavior->base_speed * frame_time, D_0015ED70 * 6.0f);
        func_L00_002592B0_91A8(moby,
                               fast_arc_tan(snapper_behavior->home.x - moby->pos.x,
                                            snapper_behavior->home.y - moby->pos.y),
                               &snapper_behavior->turn_speed,
                               0.05f,
                               0.3f,
                               frame_time * 6.2831855f);
        scratch.m.dir[0] = fast_cos(moby->rot.z) * 2.0f;
        scratch.m.dir[1] = fast_sin(moby->rot.z) * 2.0f;
        scratch.m.dir[2] = 0.0f;
        move_moby_with_ground_and_collision_checks(
            moby, &snapper_behavior->movement, scratch.m.dir, scratch.m.out, 1.0f);
        func_L00_00262DF0_b91A8(snapper_behavior->path, &moby->pos, &moby->pos, 0.5f);
        if (fast_vec_dist(&moby->pos, &snapper_behavior->home) < 1.0f) {
            moby->state = ALIEN_SNAPPER_STATE_FACE_GOAL_AND_WAIT;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(20));
            }
        } else if (snapper_behavior->f164 != 2) {
            moby->state = ALIEN_SNAPPER_STATE_APPROACH_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    }
    case ALIEN_SNAPPER_STATE_FACE_HERO: {
        char *hero = ((char *)&D_0013F350);

        approach_angle(&moby->rot.z,
                       fast_arc_tan(*(float *)(hero + 0x80) - moby->pos.x,
                                    *(float *)(hero + 0x84) - moby->pos.y),
                       &snapper_behavior->turn_speed,
                       D_0015ED70 * 6.2831855f,
                       D_0015ED70 * 6.2831855f,
                       frame_time * 12.566371f);
        if (vector_distance_xy(&moby->pos, hero + 0x80) < 4.0f) {
            moby->state = ALIEN_SNAPPER_STATE_BITE_HERO;
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(20));
            }
        }
        break;
    }
    case ALIEN_SNAPPER_STATE_BITE_HERO: {
        char *hero = ((char *)&D_0013F350);

        approach_angle(&moby->rot.z,
                       fast_arc_tan(*(float *)(hero + 0x80) - moby->pos.x,
                                    *(float *)(hero + 0x84) - moby->pos.y),
                       &snapper_behavior->turn_speed,
                       D_0015ED70 * 6.2831855f,
                       D_0015ED70 * 6.2831855f,
                       frame_time * 12.566371f);
        if (1.0f < compute_interpolated_record_value(moby) &&
            compute_interpolated_record_value(moby) < 8.0f) {
            scratch.bite.push[0] = fast_cos(moby->rot.z) * 0.2f;
            scratch.bite.push[1] = fast_sin(moby->rot.z) * 0.2f;
            scratch.bite.push[2] = 0.0f;
            func_L00_00250800_91A8(moby, 0, bite_query_position);
            scratch.bite.moby = moby;
            scratch.bite.f14 = 1;
            scratch.bite.f20 = 1;
            scratch.bite.f1C = 1.0f;
            scratch.bite.dir[0] = fast_cos(moby->rot.z);
            scratch.bite.dir[1] = fast_sin(moby->rot.z);
            scratch.bite.dir[2] = 0.0f;
            func_L00_001F2BE8_91A8(0.5f, bite_query_position, 0, moby, &scratch.bite);
            collision_line(bite_query_position, &moby->pos, 0, moby, &scratch.bite);
        } else if (moby->anim_flags & 2) {
            moby->state = ALIEN_SNAPPER_STATE_FACE_HERO;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(20));
            }
        }
        break;
    }
    case ALIEN_SNAPPER_STATE_HIT_KNOCKBACK:
        if (func_L00_0025D6F0_91A8(moby, snapper_behavior->f70) & 0x41) {
            int *route = D_L06_001B0C30_macro[snapper_behavior->path];

            if (point_in_path_xy(&moby->pos, route + 4, *route) == 0) {
                mark_moby_for_removal(moby);
                break;
            }
            moby->state = ALIEN_SNAPPER_STATE_RETREAT_FROM_TARGET;
            moby->anim_speed = 1.0f;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
            snapper_behavior->timer = scale_game_frames(30);
        }
        func_L00_00262DF0_91A8(0.5f, snapper_behavior->path, &moby->pos, &moby->pos);
        break;
    case ALIEN_SNAPPER_STATE_DEATH_MOTION:
        if (func_L00_0025D6F0_91A8(moby, snapper_behavior->f70) & 0x40) {
            qcopy(scratch.m.dir, &moby->pos);
            func_L00_00260460_91A8(moby, scratch.m.dir, -1, 1.5f, 10.0f);
            spawn_moby_break_fragments(moby, 0x6A0, 2, 0x69F, 1, 5, 2);
            mark_moby_for_removal(moby);
        } else if (moby->pos.z < 2.0f) {
            mark_moby_for_removal(moby);
        }
        break;
    }
}

void update_alien_snapper_hit_response_and_targeting(Moby *moby) {
    enum {
        REACTION_SKIP_RESPONSE = 0,
        REACTION_DEATH = 1,
        HIT_RESPONSE_DEATH_SEQUENCE = 2,
        HIT_RESPONSE_NONLETHAL_REACTION = 3,
        HIT_RESPONSE_NONLETHAL_ANIMATION_REACTION = 4,
        HIT_RESPONSE_NONLETHAL_FALLBACK = 5,
        HIT_RESPONSE_NONLETHAL_ANIMATION_UPDATE = 6,
        HIT_RESPONSE_NONLETHAL_FEEDBACK = 7,
        HIT_RESPONSE_NONLETHAL_ANIMATION_MAINTENANCE = 8,
        HIT_RESPONSE_NONLETHAL_ANIMATION_FEEDBACK = 9,
        HIT_RESPONSE_NONLETHAL_KNOCKBACK = 10,
        REACTION_NO_RESPONSE = 11
    };
    enum {
        SNAPPER_APPROACH_TARGET = 8,
        SNAPPER_REPOSITION_AROUND_TARGET = 9,
        SNAPPER_RETURN_HOME = 12,
        SNAPPER_RETREAT_FROM_TARGET = 13
    };
    BlargVars *snapper_vars = (struct BlargVars *)moby->pvars;
    int        hit;
    float      damage;
    Target    *target;
    int        reaction;

    if (snapper_vars->targeting_boost_refresh_pending != 0) {
        snapper_vars->f1AC =
            truncate_float_to_s32(func_001F9878_2FA618(random_float_between(180.0f, 240.0f)));
        snapper_vars->targeting_boost_refresh_pending = 0;
    }
    fast_dec_timer(&snapper_vars->f1AC);
    damage = 0.0f;
    target = func_L00_0025B478_2FA618(moby, 0x330000, 0);
    if (target != 0 && target->owner != 0 && target->owner->oclass == 0x418) {
        target = 0;
    }
    reaction =
        resolve_moby_hit_response(moby, target, &snapper_vars->health, 0, &hit, &damage, 0, 4);

    if (hit != 1 && moby->state != ALIEN_SNAPPER_STATE_DEATH_MOTION &&
        moby->state != ALIEN_SNAPPER_STATE_WAIT_FOR_CLIP_TRIGGER) {
        snapper_vars->health = snapper_vars->health - damage;
        if (moby->group != 0xFF) {
            func_L06_002FAAC8_2FA618(moby);
        }
        if (snapper_vars->health <= 0.0f) {
            reaction = 1;
        } else if (snapper_vars->f1CC != 0) {
            reaction = 0xB;
            snapper_vars->hit_flash.flash_red = 0xFA;
        }

        switch (reaction) {
        case REACTION_SKIP_RESPONSE:
            break;
        case HIT_RESPONSE_NONLETHAL_REACTION:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_REACTION:
        case HIT_RESPONSE_NONLETHAL_FALLBACK:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_UPDATE:
        case HIT_RESPONSE_NONLETHAL_FEEDBACK:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_MAINTENANCE:
        case HIT_RESPONSE_NONLETHAL_ANIMATION_FEEDBACK:
        case HIT_RESPONSE_NONLETHAL_KNOCKBACK:
            moby->state = ALIEN_SNAPPER_STATE_HIT_KNOCKBACK;
            snapper_vars->horizontal_motion_speed = frame_time + frame_time;
            snapper_vars->f8C = frame_time;
            snapper_vars->motion_apex_animation_frame = 5.0f;
            snapper_vars->motion_landing_animation_frame = 8.5f;
            func_L00_0025D5B0_2FA618(
                moby, snapper_vars->f70, 9, 1, 0, fast_arc_tan(target->x, target->y));
            snapper_vars->hit_flash.flash_red = 0xFA;
            break;
        case REACTION_DEATH:
        case HIT_RESPONSE_DEATH_SEQUENCE:
            moby->state = ALIEN_SNAPPER_STATE_DEATH_MOTION;
            moby->flags &= 0xEFFF;
            snapper_vars->horizontal_motion_speed = frame_time * 8.0f;
            snapper_vars->f8C = frame_time * 5.0f;
            snapper_vars->motion_apex_animation_frame = 7.5f;
            snapper_vars->motion_landing_animation_frame = 14.5f;
            func_L00_0025D5B0_2FA618(
                moby, snapper_vars->f70, 10, 1, 0, fast_arc_tan(target->x, target->y));
            snapper_vars->hit_flash.flash_red = 0xFA;
            mark_moby_for_removal_cf(moby, 0, -1);
            break;
        case REACTION_NO_RESPONSE:
            break;
        }
        start_hit_color_flash(moby, &snapper_vars->hit_flash);
    }

    moby->hit_slot = 0xFF;
    update_hit_color_flash(moby, &snapper_vars->hit_flash);
    switch (moby->state) {
    case SNAPPER_APPROACH_TARGET:
    case SNAPPER_REPOSITION_AROUND_TARGET:
    case SNAPPER_RETURN_HOME:
    case SNAPPER_RETREAT_FROM_TARGET:
        if (level_frame_count % 4 == (((int)moby >> 8) & 3)) {
            func_L00_0025A8E8_2FA618(moby, 0.7f, &moby->pos.x, 0x10000, 0.9f, 1.0f, 0, 1, 0);
        }
        break;
    }
    snapper_vars->f1C8 = fast_add_rotations(
        snapper_vars->f1C8, alien_snapper_glow_phase_speed_degrees_gp * 0.017453292f * frame_time);
    moby->glow_color = func_001FA8A8_2FA618(l06_alien_snapper_glow_gray_color,
                                            l06_alien_snapper_glow_tinted_color,
                                            (fast_sin(snapper_vars->f1C8) + 1.0f) * 0.5f);

    if (snapper_vars->f1AC != 0) {
        snapper_vars->f1A0 = l06_alien_snapper_base_targeting_radius + 6.0f;
    } else if (moby->state == ALIEN_SNAPPER_STATE_WAIT_FOR_ACTIVATION) {
        if (moby == ((Moby *)hero.wall_moby) || moby == ((Moby *)hero.coll_hit_moby)) {
            snapper_vars->f1A0 = 6.0f;
        } else {
            snapper_vars->f1A0 = 0.0f;
        }
    } else {
        snapper_vars->f1A0 = l06_alien_snapper_base_targeting_radius;
    }

    if (activate_effect_l08(snapper_vars->f1A0,
                            moby,
                            snapper_vars->selected_target_xy,
                            0,
                            0,
                            D_L06_001B0FB0_2FA618[snapper_vars->movement_path_index] + 4,
                            *D_L06_001B0FB0_2FA618[snapper_vars->movement_path_index]) != 2) {
        if (snapper_vars->f1A0 <
                vector_distance_xy(snapper_vars->home_position, snapper_vars->selected_target_xy) ||
            3.0f < absolute_float(moby->pos.z - snapper_vars->selected_target_z)) {
            snapper_vars->f164 = 2;
        }
    }
    if (snapper_vars->selected_target_moby == 0) {
        snapper_vars->selected_target_moby = D_001413D0[0];
    }
}

void transition_moby_group_state_1_to_2(char *group_source_moby) {
    short *table = D_L06_001ABFC0[(unsigned char)group_source_moby[0x21]];
    if (table != 0) {
        short *moby_index_cursor = table;
        do {
            char *moby = (char *)(((*moby_index_cursor & 0x7FFF) << 8) + (int)level_mobys_q);
            if ((unsigned char)moby[0x20] == 1) {
                moby[0x20] = 2;
            }
        } while (*moby_index_cursor++ >= 0);
    }
}

float compute_alien_snapper_target_gap_angle(char *snapper_moby) {
    char *target_moby = *(char **)(*(char **)(snapper_moby + 0x78) + 0x160);
    float hi, lo, owner_to_self_angle_radians, owner_to_self_distance,
        translated_owner_to_self_vector[3];
    short *moby_index_cursor;
    if ((unsigned char)snapper_moby[0x21] == 0xFF) {
        return fast_arc_tan(*(float *)(snapper_moby + 0x10) - *(float *)(target_moby + 0x10),
                            *(float *)(snapper_moby + 0x14) - *(float *)(target_moby + 0x14));
    }
    lo = -(*(float *)&alien_snapper_gap_angle_limit_degrees_gp * DEG_TO_RAD);
    hi = *(float *)&alien_snapper_gap_angle_limit_degrees_gp * DEG_TO_RAD;
    owner_to_self_angle_radians =
        fast_arc_tan(*(float *)(snapper_moby + 0x10) - *(float *)(target_moby + 0x10),
                     *(float *)(snapper_moby + 0x14) - *(float *)(target_moby + 0x14));
    owner_to_self_distance = fast_vec_dist(target_moby + 0x10, snapper_moby + 0x10);
    translated_owner_to_self_vector[0] = fast_cos(owner_to_self_angle_radians) * 2.0f;
    translated_owner_to_self_vector[1] = fast_sin(owner_to_self_angle_radians) * 2.0f;
    translated_owner_to_self_vector[2] = 0.0f;
    fast_vec_add(
        translated_owner_to_self_vector, translated_owner_to_self_vector, D_0013E533 + 0xE9D);
    moby_index_cursor = D_L06_001ABFC0_p[(unsigned char)snapper_moby[0x21]];
    if (moby_index_cursor == 0) {
        return 0.0f;
    }
    do {
        int   listed_moby_index = *(unsigned short *)moby_index_cursor & 0x7FFF;
        char *listed_moby = (char *)(listed_moby_index * 256 + (int)level_mobys_q);
        if ((unsigned char)listed_moby[0x20] == 8 ||
            fast_vec_dist(listed_moby + 0x10, target_moby + 0x10) < owner_to_self_distance) {
            float d = fast_arc_tan(*(float *)(level_mobys_q + listed_moby_index * 256 + 0x10) -
                                       *(float *)(target_moby + 0x10),
                                   *(float *)(level_mobys_q + listed_moby_index * 256 + 0x14) -
                                       *(float *)(target_moby + 0x14));
            float listed_moby_angle_offset_radians =
                fast_subtract_rotations(d, owner_to_self_angle_radians);
            if (listed_moby_angle_offset_radians > 0.0f && listed_moby_angle_offset_radians < hi) {
                hi = listed_moby_angle_offset_radians;
            } else if (listed_moby_angle_offset_radians < 0.0f &&
                       listed_moby_angle_offset_radians > lo) {
                lo = listed_moby_angle_offset_radians;
            }
        }
    } while (*moby_index_cursor++ >= 0);
    return fast_add_rotations(
        fast_add_rotations(lo, fast_difference_between_rotations(hi, lo) * 0.5f),
        owner_to_self_angle_radians);
}
