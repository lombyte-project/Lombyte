/* Level 04 moby class blarg_trooper. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/normalize_vector_triplet.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/math/quad.h"

typedef struct BlargTrooperVars {
    u8                     pad_0[0x20];
    f32                    health;
    s16                    max_health;
    u8                     pad_26[0x2];
    u8                     hit_response_class;
    u8                     unk29;
    u8                     pad_2A[0x4];
    u8                     unk2E;
    u8                     pad_2F[0x9];
    s32                    retimer;
    u8                     pad_3C[0x1C];
    u8                     unk58;
    u8                     pad_59[0x1];
    u8                     unk5A;
    u8                     pad_5B[0x5];
    struct HitColorFlash   hit_flash;
    struct KnockbackMotion knockback;
    f32                    fall_speed;
    f32                    spawn_z; /* removed once it falls 5 below it */
    f32                    prev_anim_frame;
    f32                    sight_range; /* doubled while alerted */
    s32                    path_when_selected;
    s32                    path_when_unselected;
    s16                    path_selector;
    u8                     pad_EA[0x2];
    f32                    yaw_turn_velocity;
    s16                    timer_F0;
    s16                    timer_F2;
    f32                    recoil_speed;
    char                  *gun_moby;
    u8                     starts_aiming;
    u8                     shots_left;
    u8                     alert_timer; /* 240 frames after a hit; doubles sight_range */
    u8                     idle_anim_timer;
    u8                     pad_100[0x8];
    s32                    timer_108;
    u8                     pad_10C[0x4];
    f32                    manipulator0[4];
    u8                     pad_120[0x58];
    f32                    manipulator0_turn; /* rot target z of the joint 5 ease */
    u8                     pad_17C[0x14];
    f32                    manipulator1[4];
    u8                     pad_1A0[0x58];
    f32                    manipulator1_turn; /* rot target z of the joint 6 ease */
} BlargTrooperVars;

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C5B88), where it is exact; names translated to the US level program. */

extern void spawn_moby_table_puff_pattern(void *) __asm__("FUN_L04_002c46d0");

void destroy_blarg_trooper(struct Moby *moby) __asm__("FUN_L04_002c4808");

void destroy_blarg_trooper(struct Moby *moby) {
    BlargTrooperVars *trooper_pvars = (BlargTrooperVars *)moby->pvars;
    void             *trooper_gun_moby = trooper_pvars->gun_moby;
    if (trooper_gun_moby != 0) {
        mark_moby_for_removal(trooper_gun_moby);
    }
    spawn_moby_table_puff_pattern(moby);
    mark_moby_for_removal(moby);
}

extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void  select_moby_combat_target(char *, float *, float) __asm__("FUN_L00_0025fcb8");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float D_L04_00166F40[4];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   FUN_L00_001fefc8(void *);
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int   allocate_voice_for_target_entry(int, int, char *);
extern void *spawn_moby_attachment(void *, void *) __asm__("FUN_L04_002ce080");
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_0025a120(void *);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void  start_hit_color_flash(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void  advance_moby_animation(void *);
extern void  build_moby_bone_transform(char *, int, char *);
extern void  copy_matrix3x4(void *, void *);
extern void  refresh_moby_spatial_bounds_from_basis(void *);
extern float fast_cos(float);
extern float fast_sin(float);
extern void  FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);

/* Blarg trooper update: takes hits, walks its path toward the target, fires, and keeps its gun moby attached. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C5BD0), where it is exact; names translated to the US level program. */

struct Path5BD0 {
    int   count;
    int   pad[3];
    float pts[1][4];
};

struct Shot5BD0 {
    float f0;
    float f4;
    float hit_direction_z;
    float fC;
    void *p10;
    int   v14;
    char  c18;
    char  c19;
    short hit_source_class;
    float f1C;
    int   pad[4];
};

extern char                *D_L04_0015FFD8;
extern char *D_L04_00173FD8 NOT_SDA;
extern char                 D_L04_00166E00[];

char blarg_trooper_missing_path_fields_format[86] __asm__("D_L04_001DBC80") =
    "moby 427, instance %d requires both path fields to run!!! DELETING MOBY FROM LEVEL!!\n";

extern float vector_length_xy(void *);
extern int   DebugPrint();
extern int   rand(void);
extern void  record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void  update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void spawn_blarg_trooper_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");
void        destroy_blarg_trooper(struct Moby *arg) __asm__("FUN_L04_002c4808");
extern int  move_moby_xy_with_collision_constraints(
     void *, void *, void *, float, float, float, float, int) __asm__("FUN_L00_00258490");

void blarg_trooper_update(struct Moby *moby) __asm__("FUN_L04_002c4850");

/* Moby class 427 */
void blarg_trooper_update(struct Moby *moby) {
    enum {
        BLARG_TROOPER_STATE_INITIALIZE = 0,
        BLARG_TROOPER_STATE_IDLE = 1,
        BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT = 2,
        BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT = 3,
        BLARG_TROOPER_STATE_JUMP = 4,
        BLARG_TROOPER_STATE_AIM = 5,
        BLARG_TROOPER_STATE_FIRE = 6,
        BLARG_TROOPER_STATE_MELEE_ATTACK = 7,
        BLARG_TROOPER_STATE_HIT_RECOVERY = 8,
        BLARG_TROOPER_STATE_BACKWARD_RECOVERY = 9,
        BLARG_TROOPER_STATE_KNOCKBACK_RECOVERY = 10,
        BLARG_TROOPER_STATE_DYING = 11
    };
    enum {
        BLARG_TROOPER_HIT_REACTION_NONE = 0,
        BLARG_TROOPER_HIT_REACTION_FATAL = 1,
        BLARG_TROOPER_HIT_REACTION_FATAL_KNOCKBACK = 2,
        BLARG_TROOPER_HIT_REACTION_START_HIT_RECOVERY = 3,
        BLARG_TROOPER_HIT_REACTION_START_KNOCKBACK_RECOVERY = 4,
        BLARG_TROOPER_HIT_REACTION_KNOCKBACK_RECOVERY = 5,
        BLARG_TROOPER_HIT_REACTION_RECOIL_RECOVERY = 6,
        BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_RESPONSE = 7,
        BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_REACTION = 8,
        BLARG_TROOPER_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE = 9,
        BLARG_TROOPER_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        BLARG_TROOPER_HIT_REACTION_IGNORED = 11
    };
    struct {
        float tgt[20];
        float v50[4];
        float v60[4];
        union {
            float m[4][4];
            struct {
                float           v70[4];
                float           v80[4];
                float           v90[4];
                struct Shot5BD0 blk;
            } p;
        } w;
    } update_scratch;
    int               hit_reaction_status;
    float             dmg;
    float             fatal_knockback_yaw;
    float             recovery_knockback_yaw;
    float             one;
    BlargTrooperVars *blarg_trooper_pvars;
    int               flag;
    float            *pos;
    char             *target;
    float             target_distance;
    float             alive;
    float             best;
    float             target_selection_distance_limit;
    int               path_index_when_selector_nonzero;
    int               path_index_when_selector_zero;
    int               bi;
    int               i;
    struct Path5BD0  *path;
    char             *hit;
    int               r;

    flag = 0;
    blarg_trooper_pvars = (char *)moby->pvars;
    if (blarg_trooper_pvars->retimer != 0 || blarg_trooper_pvars->alert_timer != 0) {
        flag = 1;
    }
    pos = (float *)&moby->pos;
    manipulator_set_scale(blarg_trooper_pvars->manipulator0, 3.0f);
    if (moby->visible != 0) {
        if (fast_vec_dist(pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x15;
        }
    }
    if (flag) {
        float x = blarg_trooper_pvars->sight_range;
        target_selection_distance_limit = x + x;
    } else {
        target_selection_distance_limit = blarg_trooper_pvars->sight_range;
    }
    select_moby_combat_target(moby, update_scratch.tgt, target_selection_distance_limit);
    target = *(char **)&update_scratch.tgt[16];
    target_distance = vector_distance_xy(pos, update_scratch.tgt);
    dmg = 0.0f;
    alive = compute_interpolated_record_value(moby);
    if (moby->state != BLARG_TROOPER_STATE_INITIALIZE && blarg_trooper_pvars->unk2E != 1) {
        char *o = blarg_trooper_pvars->gun_moby;
        if (o != 0) {
            mark_moby_for_removal(o);
        }
        mark_moby_for_removal(moby);
        return;
    }
    path_index_when_selector_nonzero = blarg_trooper_pvars->path_when_selected;
    if (path_index_when_selector_nonzero == -1 ||
        (path_index_when_selector_zero = blarg_trooper_pvars->path_when_unselected,
         path_index_when_selector_zero == -1)) {
        DebugPrint(blarg_trooper_missing_path_fields_format,
                   truncate_float_to_s32((float)(((char *)moby - D_L04_0015FFD8) >> 8)));
        destroy_blarg_trooper(moby);
        return;
    }
    bi = -1;
    best = 0.0f;
    qcopy(update_scratch.v50, &update_scratch.tgt[8]);
    path = ((struct Path5BD0 *)
                l04_paths[blarg_trooper_pvars->path_selector != 0 ? path_index_when_selector_nonzero
                                                                  : path_index_when_selector_zero]);
    for (i = 0; i < path->count; i++) {
        float d = vector_distance_xy(update_scratch.v50, path->pts[i]);
        if (d > best) {
            best = d;
            bi = i;
        }
    }
    if (bi != -1) {
        qcopy(update_scratch.v60, path->pts[bi]);
        best = vector_distance_xy(update_scratch.v60, pos);
    } else {
        qcopy(update_scratch.v60, pos);
        best = 0.0f;
    }
    hit = find_moby_hit(moby, 0x210000, 0);
    r = hit_reaction(moby, hit, &blarg_trooper_pvars->health, 0, &hit_reaction_status, &dmg, 0, 4);
    fast_dec_timer_s16(&blarg_trooper_pvars->timer_F0);
    fast_dec_timer_s16(&blarg_trooper_pvars->timer_F2);
    if (hit != 0 && moby->state < BLARG_TROOPER_STATE_DYING && blarg_trooper_pvars->timer_F0 == 0) {
        char *o;
        float t;
        o = *(char **)(hit + 0x20);
        one = 1.0f;
        if (o != 0) {
            struct Hero *g = &hero;
            if (o != ((char *)g->items[0].moby)) {
                recovery_knockback_yaw = fast_arc_tan(moby->pos.x - *(float *)(o + 0x10),
                                                      moby->pos.y - *(float *)(o + 0x14));
            } else {
                recovery_knockback_yaw = fast_arc_tan(moby->pos.x - g->motion.pos.f[0],
                                                      moby->pos.y - g->motion.pos.f[1]);
            }
        } else {
            struct Hero *g = &hero;
            recovery_knockback_yaw =
                fast_arc_tan(moby->pos.x - g->motion.pos.f[0], moby->pos.y - g->motion.pos.f[1]);
        }
        t = blarg_trooper_pvars->health - dmg;
        blarg_trooper_pvars->health = t;
        if (t <= 0.0f) {
            r = 1;
        }
        blarg_trooper_pvars->alert_timer = scale_game_frames(0xF0);
        switch (r) {
        case BLARG_TROOPER_HIT_REACTION_NONE:
            break;
        case BLARG_TROOPER_HIT_REACTION_FATAL:
        case BLARG_TROOPER_HIT_REACTION_FATAL_KNOCKBACK:
            moby->flags &= 0xEFFF;
            blarg_trooper_pvars->hit_flash.flash_red = 0xFA;
            {
                float a = frame_time_sq * 37.0f;
                float b = frame_time * 17.0f;
                float fatal_knockback_horizontal_speed_adjustment = frame_time_sq * 20.0f;
                float d = frame_time * 13.0f;
                blarg_trooper_pvars->knockback.gravity = a;
                blarg_trooper_pvars->knockback.speed = b;
                blarg_trooper_pvars->knockback.speed_approach_step =
                    fatal_knockback_horizontal_speed_adjustment;
                blarg_trooper_pvars->knockback.vertical_speed = d;
            }
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(update_scratch.w.p.v70,
                                  &fatal_knockback_yaw,
                                  &blarg_trooper_pvars->knockback.speed,
                                  &blarg_trooper_pvars->knockback.vertical_speed);
            trooper_start_knockback(
                fatal_knockback_yaw, moby, &blarg_trooper_pvars->knockback, 0xD, 1, 0);
            blarg_trooper_pvars->knockback.apex_animation_frame = 12.0f;
            blarg_trooper_pvars->knockback.landing_animation_frame = 25.0f;
            moby->state = BLARG_TROOPER_STATE_DYING;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case BLARG_TROOPER_HIT_REACTION_START_HIT_RECOVERY:
        case BLARG_TROOPER_HIT_REACTION_RECOIL_RECOVERY: {
            int attacker_class;
            blarg_trooper_pvars->hit_flash.flash_red = 0x96;
            blend_moby_animation(moby, (void *)8, 0, (void *)scale_game_frames(5));
            moby->state = BLARG_TROOPER_STATE_HIT_RECOVERY;
            attacker_class = *(short *)(*(char **)(hit + 0x20) + 0xA6);
            if (attacker_class == 0x131 || attacker_class == 0xB0) {
                blarg_trooper_pvars->recoil_speed = 2.0f / scale_time(15.0f) * 0.5f;
            } else {
                blarg_trooper_pvars->recoil_speed = 2.0f / scale_time(15.0f);
            }
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(update_scratch.w.p.v70,
                                  &recovery_knockback_yaw,
                                  &blarg_trooper_pvars->recoil_speed,
                                  &one);
            moby->rot.z = fast_add_rotations(3.1415927f, recovery_knockback_yaw);
            break;
        }
        case BLARG_TROOPER_HIT_REACTION_START_KNOCKBACK_RECOVERY:
        case BLARG_TROOPER_HIT_REACTION_KNOCKBACK_RECOVERY:
            blarg_trooper_pvars->hit_flash.flash_red = 0xFA;
            blend_moby_animation(moby, (void *)0xC, 0, (void *)scale_game_frames(5));
            moby->state = BLARG_TROOPER_STATE_KNOCKBACK_RECOVERY;
            blarg_trooper_pvars->recoil_speed = 2.0f / scale_time(15.0f);
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(update_scratch.w.p.v70,
                                  &recovery_knockback_yaw,
                                  &blarg_trooper_pvars->recoil_speed,
                                  &one);
            moby->rot.z = fast_add_rotations(3.1415927f, recovery_knockback_yaw);
            break;
        case BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_REACTION:
        case BLARG_TROOPER_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_NO_FURTHER_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_IGNORED:
            break;
        }
        start_hit_color_flash(moby, &blarg_trooper_pvars->hit_flash);
    }
    moby->hit_slot = 0xFF;
    if (blarg_trooper_pvars->retimer != 0) {
        blarg_trooper_pvars->alert_timer = scale_game_frames(0xF0);
    }
    blarg_trooper_pvars->retimer = 0;
    FUN_L00_001fefc8(&blarg_trooper_pvars->alert_timer);
    fast_dec_timer(&blarg_trooper_pvars->timer_108);
    switch (moby->state) {
    case BLARG_TROOPER_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        blarg_trooper_pvars->health = 2.0f;
        blarg_trooper_pvars->max_health = 2;
        blarg_trooper_pvars->hit_response_class = 1;
        blarg_trooper_pvars->unk2E = 1;
        blarg_trooper_pvars->unk29 = 1;
        blarg_trooper_pvars->timer_F0 = 0;
        blarg_trooper_pvars->alert_timer = 0;
        blarg_trooper_pvars->idle_anim_timer = 0;
        blarg_trooper_pvars->fall_speed = 0.0f;
        {
            float z = moby->pos.z;
            blarg_trooper_pvars->unk58 = 8;
            blarg_trooper_pvars->spawn_z = z;
            blarg_trooper_pvars->unk5A = 0xA;
        }
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        if (blarg_trooper_pvars->starts_aiming != 0) {
            blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(0xA));
            moby->state = BLARG_TROOPER_STATE_AIM;
        } else {
            blend_moby_animation(
                moby, (void *)((rand() ^ 1) & 1), 0, (void *)scale_game_frames(0xA));
            moby->state = BLARG_TROOPER_STATE_IDLE;
        }
        break;
    case BLARG_TROOPER_STATE_IDLE:
        if (moby->anim_flags & 2) {
            blend_moby_animation(
                moby, (void *)((rand() ^ 1) & 1), 0, (void *)scale_game_frames(0xA));
        }
        {
            float a = fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                   update_scratch.v50[1] - moby->pos.y);
            FUN_L00_00258278(moby,
                             &blarg_trooper_pvars->yaw_turn_velocity,
                             a,
                             frame_scale_sq * 0.04f,
                             frame_scale_sq * 0.3f,
                             frame_time * 7.330382823944092f);
        }
        if (flag) {
            blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(8));
            moby->state = BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT;
            break;
        }
        if (target_distance > 2.5f && best > 1.0f) {
            if (target_distance > 14.0f) {
                blend_moby_animation(moby, (void *)0xA, 0, (void *)scale_game_frames(8));
                moby->state = BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT;
                break;
            }
            if (target_distance < 8.0f && blarg_trooper_pvars->timer_108 == 0) {
                blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(8));
                moby->state = BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT;
                break;
            }
        }
        if (target_distance < 14.0f && target_distance >= 2.5f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 8.0f) {
            if (blarg_trooper_pvars->timer_F2 == 0) {
                *(OvlQuad *)update_scratch.w.p.v90 = 0;
                update_scratch.w.p.v90[2] = 0.5f;
                fast_vec_add(update_scratch.w.p.v70, pos, update_scratch.w.p.v90);
                fast_vec_add(update_scratch.w.p.v80, update_scratch.v50, update_scratch.w.p.v90);
                if (collision_line(update_scratch.w.p.v70, update_scratch.w.p.v80, 0, moby, 0) !=
                        0 &&
                    D_L04_00173FD8 == target) {
                    moby->state = BLARG_TROOPER_STATE_AIM;
                    blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
                }
            }
        } else if (target_distance <= 2.5f) {
            int t = blarg_trooper_pvars->timer_F2;
            if (t <= 0) {
                moby->state = BLARG_TROOPER_STATE_MELEE_ATTACK;
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            } else if (scale_game_frames(0x14) < t) {
                blarg_trooper_pvars->timer_F2 = scale_game_frames(0x14);
            }
        }
        break;
    case BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT:
        if (target_distance < 13.0f || best < 1.0f) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
            moby->state = BLARG_TROOPER_STATE_IDLE;
        } else {
            float f;
            {
                float a = fast_arc_tan(update_scratch.v60[0] - moby->pos.x,
                                       update_scratch.v60[1] - moby->pos.y);
                FUN_L00_00258278(moby,
                                 &blarg_trooper_pvars->yaw_turn_velocity,
                                 a,
                                 frame_scale_sq * 0.03f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 4.712389f);
            }
            f = blarg_trooper_pvars->yaw_turn_velocity * 30.0f;
            if (f > 0.7853982f) {
                f = 0.7853982f;
            } else if (f < -0.7853982f) {
                f = -0.7853982f;
            }
            blarg_trooper_pvars->manipulator0_turn = f * 0.7f;
            blarg_trooper_pvars->manipulator1_turn = f * 0.4f;
            update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 2.5f);
            update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 2.5f);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            move_moby_xy_with_collision_constraints(
                moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 0);
        }
        break;
    case BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT:
        blarg_trooper_pvars->timer_108 = scale_game_frames(0x64);
        if (flag) {
            int k;
            {
                float a = fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                       update_scratch.v50[1] - moby->pos.y);
                FUN_L00_00258278(moby,
                                 &blarg_trooper_pvars->yaw_turn_velocity,
                                 a,
                                 frame_scale_sq * 0.04f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 4.886922f);
            }
            update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 6.0f);
            update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 6.0f);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            qcopy(update_scratch.w.p.v80, pos);
            if (move_moby_xy_with_collision_constraints(moby,
                                                        update_scratch.w.p.v80,
                                                        update_scratch.w.p.v70,
                                                        0.5f,
                                                        1.0f,
                                                        0.5f,
                                                        0.5235988f,
                                                        0) != 0) {
                if (point_in_path_xy(update_scratch.w.p.v80, path->pts, path->count) != 0) {
                    qcopy(pos, update_scratch.w.p.v80);
                    k = 0xB;
                } else {
                    k = 0;
                }
            } else {
                k = 0;
            }
            if (FUN_L00_001fefc8(&blarg_trooper_pvars->idle_anim_timer) != 0 &&
                k != moby->prev_seq) {
                blend_moby_animation(moby, (void *)k, 0, (void *)scale_game_frames(0xA));
                blarg_trooper_pvars->idle_anim_timer = scale_game_frames(0xF);
            }
            if (blarg_trooper_pvars->timer_F2 == 0) {
                if (target_distance < 1.0f && fast_difference_between_rotations(
                                                  fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                               update_scratch.v50[1] - moby->pos.y),
                                                  moby->rot.z) < 0.2617994f) {
                    moby->state = BLARG_TROOPER_STATE_MELEE_ATTACK;
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
                    blarg_trooper_pvars->idle_anim_timer = 0;
                } else if (target_distance < 14.0f) {
                    moby->state = BLARG_TROOPER_STATE_AIM;
                    blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
                    blarg_trooper_pvars->idle_anim_timer = 0;
                }
            }
        } else {
            if (target_distance < 1.0f &&
                fast_difference_between_rotations(fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                               update_scratch.v50[1] - moby->pos.y),
                                                  moby->rot.z) < 0.2617994f) {
                moby->state = BLARG_TROOPER_STATE_MELEE_ATTACK;
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            } else if (target_distance > 9.0f) {
                moby->state = BLARG_TROOPER_STATE_AIM;
                blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
            } else if (best < 1.0f) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
                moby->state = BLARG_TROOPER_STATE_IDLE;
            } else {
                {
                    float a = fast_arc_tan(update_scratch.v60[0] - moby->pos.x,
                                           update_scratch.v60[1] - moby->pos.y);
                    FUN_L00_00258278(moby,
                                     &blarg_trooper_pvars->yaw_turn_velocity,
                                     a,
                                     frame_scale_sq * 0.035f,
                                     frame_scale_sq * 0.3f,
                                     frame_time * 4.712389f);
                }
                update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 6.0f);
                update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 6.0f);
                update_scratch.w.p.v70[2] = 0.0f;
                fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
                move_moby_xy_with_collision_constraints(
                    moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 0);
            }
        }
        break;
    case BLARG_TROOPER_STATE_JUMP:
        if (moby->prev_seq != 2) {
            blend_moby_animation(moby, (void *)2, 0, (void *)scale_game_frames(7));
            blarg_trooper_pvars->fall_speed = -(frame_time * 7.668f);
        } else if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
            moby->state = BLARG_TROOPER_STATE_IDLE;
        }
        break;
    case BLARG_TROOPER_STATE_AIM: {
        float a =
            fast_arc_tan(update_scratch.v50[0] - moby->pos.x, update_scratch.v50[1] - moby->pos.y);
        FUN_L00_00258278(moby,
                         &blarg_trooper_pvars->yaw_turn_velocity,
                         a,
                         frame_scale_sq * 0.03f,
                         frame_scale_sq * 0.3f,
                         frame_time * 5.585053443908691f);
    }
        if ((moby->anim_flags & 2) && moby->seq != 4) {
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(7));
        }
        if (!flag && target_distance > 2.5f && best > 1.0f) {
            if (target_distance > 14.0f) {
                blend_moby_animation(moby, (void *)0xA, 0, (void *)scale_game_frames(9));
                moby->state = BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT;
                break;
            }
            if (target_distance < 8.0f && blarg_trooper_pvars->timer_108 == 0) {
                blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(5));
                moby->state = BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT;
                break;
            }
        }
        if (target_distance < 2.5f) {
            moby->state = BLARG_TROOPER_STATE_MELEE_ATTACK;
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            break;
        }
        if (blarg_trooper_pvars->timer_F2 == 0 && target_distance < 14.0f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 4.0f) {
            char *g = D_L04_00166E00;
            if (fast_difference_between_rotations(
                    *(float *)(g + 0x158),
                    fast_arc_tan(moby->pos.x - *(float *)(g + 0x140),
                                 moby->pos.y - *(float *)(g + 0x144))) < 0.61086524f &&
                fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                 update_scratch.v50[1] - moby->pos.y)) < 0.7853982f) {
                moby->state = BLARG_TROOPER_STATE_FIRE;
                blend_moby_animation(moby, (void *)5, 0, (void *)scale_game_frames(7));
                blarg_trooper_pvars->timer_F2 = scale_game_frames(0x1E);
                blarg_trooper_pvars->shots_left = 3;
                break;
            }
        }
        if (target_distance > 14.0f) {
            moby->state = flag ? 3 : 1;
            blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(0xA));
            blarg_trooper_pvars->timer_F2 = scale_game_frames(0x1E);
        }
        break;
    case BLARG_TROOPER_STATE_FIRE: {
        char *o;
        {
            float a = fast_arc_tan(update_scratch.tgt[8] - moby->pos.x,
                                   update_scratch.tgt[9] - moby->pos.y);
            FUN_L00_00258278(moby,
                             &blarg_trooper_pvars->yaw_turn_velocity,
                             a,
                             frame_scale_sq * 0.04f,
                             frame_scale_sq * 0.3f,
                             frame_time * 4.712389f);
        }
        if (target_distance < 2.5f) {
            moby->state = BLARG_TROOPER_STATE_MELEE_ATTACK;
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
        }
        if (moby->prev_seq == 5 && blarg_trooper_pvars->prev_anim_frame < 2.0f && alive >= 2.0f &&
            (o = blarg_trooper_pvars->gun_moby) != 0) {
            float a;
            moby_point_position(o, 0, update_scratch.w.p.v70);
            fast_vec_sub(update_scratch.w.p.v80, update_scratch.v50, update_scratch.w.p.v70);
            a = fast_arc_tan(update_scratch.w.p.v80[0], update_scratch.w.p.v80[1]);
            a = (fast_difference_between_rotations(a, moby->rot.z) > 0.2617994f) ? moby->rot.z : a;
            {
                float p = fast_arc_tan(vector_length_xy(update_scratch.w.p.v80),
                                       update_scratch.w.p.v80[2]);
                build_spherical_offset(update_scratch.w.p.v80, frame_time * 10.0f, a, p);
            }
            allocate_voice_for_target_entry(7, 0, moby);
            spawn_blarg_trooper_projectile(moby,
                                           update_scratch.w.p.v70,
                                           update_scratch.w.p.v80,
                                           truncate_float_to_s32(14.0f / (frame_time * 10.0f)));
            FUN_L00_001fefc8(&blarg_trooper_pvars->shots_left);
        } else if ((moby->anim_flags & 2) && blarg_trooper_pvars->timer_F2 == 0 &&
                   blarg_trooper_pvars->shots_left == 0) {
            blarg_trooper_pvars->timer_F2 = scale_game_frames(0xB4);
            moby->state = BLARG_TROOPER_STATE_AIM;
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(0xA));
        }
        break;
    }
    case BLARG_TROOPER_STATE_MELEE_ATTACK: {
        float a =
            fast_arc_tan(update_scratch.v50[0] - moby->pos.x, update_scratch.v50[1] - moby->pos.y);
        FUN_L00_00258278(moby,
                         &blarg_trooper_pvars->yaw_turn_velocity,
                         a,
                         frame_scale_sq * 0.04f,
                         frame_scale_sq * 0.3f,
                         frame_time * 5.585053443908691f);
    }
        if (moby->seq == moby->prev_seq && alive >= 12.0f && alive < 13.0f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 2.0f &&
            vector_distance_xy(pos, update_scratch.v50) < 1.0f &&
            fast_difference_between_rotations(fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                           update_scratch.v50[1] - moby->pos.y),
                                              moby->rot.z) < 0.2617994f) {
            update_scratch.w.p.blk.f0 = fast_cos(moby->rot.z) * 0.2f;
            update_scratch.w.p.blk.f4 = fast_sin(moby->rot.z) * 0.2f;
            update_scratch.w.p.blk.hit_direction_z = 0.0f;
            update_scratch.w.p.blk.p10 = moby;
            update_scratch.w.p.blk.v14 = 1;
            update_scratch.w.p.blk.f1C = 1.0f;
            set_vector_length_xy(&update_scratch.w.p.blk, &update_scratch.w.p.blk, 1.0f);
            update_scratch.w.p.blk.hit_direction_z = 1.0f;
            update_scratch.w.p.blk.fC = 5627.925f;
            update_scratch.w.p.blk.c19 = 1;
            update_scratch.w.p.blk.hit_source_class = (u16)moby->oclass;
            update_scratch.w.p.blk.c18 = 0;
            qcopy(update_scratch.w.p.v70, update_scratch.v50);
            update_scratch.w.p.v70[2] = update_scratch.w.p.v70[2] + 0.75f;
            if (target != 0) {
                record_moby_hit(target, &update_scratch.w.p.blk);
            }
        }
        if (moby->anim_flags & 2) {
            moby->state = BLARG_TROOPER_STATE_AIM;
            blarg_trooper_pvars->timer_F2 = scale_game_frames(0x14);
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(7));
        }
        break;
    case BLARG_TROOPER_STATE_HIT_RECOVERY:
    case BLARG_TROOPER_STATE_BACKWARD_RECOVERY:
    case BLARG_TROOPER_STATE_KNOCKBACK_RECOVERY:
        if (blarg_trooper_pvars->recoil_speed > 0.0f) {
            update_scratch.w.p.v70[0] = fast_cos(fast_add_rotations(moby->rot.z, 3.1415927f)) *
                                        blarg_trooper_pvars->recoil_speed;
            update_scratch.w.p.v70[1] = fast_sin(fast_add_rotations(moby->rot.z, 3.1415927f)) *
                                        blarg_trooper_pvars->recoil_speed;
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            blarg_trooper_pvars->recoil_speed =
                blarg_trooper_pvars->recoil_speed + -2.0f / (scale_time(15.0f) * scale_time(15.0f));
            move_moby_xy_with_collision_constraints(
                moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 1);
        }
        if (moby->anim_flags & 2) {
            *(unsigned short *)&blarg_trooper_pvars->path_selector ^= 1;
            blend_moby_animation(
                moby,
                (void *)(moby->state == BLARG_TROOPER_STATE_BACKWARD_RECOVERY ? 9 : 0),
                0,
                (void *)scale_game_frames(9));
            moby->state = BLARG_TROOPER_STATE_IDLE;
            blarg_trooper_pvars->timer_108 = scale_game_frames(0x78);
        }
        break;
    case BLARG_TROOPER_STATE_DYING:
        if (step_moby_motion(moby, &blarg_trooper_pvars->knockback) & 0x60) {
            destroy_blarg_trooper(moby);
            return;
        }
        break;
    }
    update_hit_color_flash(moby, &blarg_trooper_pvars->hit_flash);
    {
        int current_state = moby->state;
        if (current_state == BLARG_TROOPER_STATE_IDLE ||
            current_state == BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT ||
            current_state == BLARG_TROOPER_STATE_JUMP || current_state == BLARG_TROOPER_STATE_AIM ||
            current_state == BLARG_TROOPER_STATE_FIRE) {
            char *t = target;
            if (t == 0) {
                t = *(char **)((char *)(&hero.moby));
            }
            if (vector_distance_xy(pos, (float *)(t + 0x10)) < 24.0f) {
                float f = fast_subtract_rotations(fast_arc_tan(*(float *)(t + 0x10) - moby->pos.x,
                                                               *(float *)(t + 0x14) - moby->pos.y),
                                                  moby->rot.z);
                if (f > 0.9599311f) {
                    f = 0.9599311f;
                } else if (f < -0.9599311f) {
                    f = -0.9599311f;
                }
                blarg_trooper_pvars->manipulator0_turn = f * 0.5f;
                blarg_trooper_pvars->manipulator1_turn = f * 0.5f;
            }
        }
    }
    if (moby->flags & 0x8000) {
        blarg_trooper_pvars->manipulator0_turn = -blarg_trooper_pvars->manipulator0_turn;
        blarg_trooper_pvars->manipulator1_turn = -blarg_trooper_pvars->manipulator1_turn;
    }
    update_joint_animation(
        moby, blarg_trooper_pvars->manipulator0, 5, frame_scale_sq * 0.018f, frame_scale_sq * 0.3f);
    update_joint_animation(
        moby, blarg_trooper_pvars->manipulator1, 6, frame_scale_sq * 0.024f, frame_scale_sq * 0.3f);
    if (blarg_trooper_pvars->gun_moby != 0) {
        char *o;
        build_moby_bone_transform(moby, 1, (char *)update_scratch.w.m);
        o = blarg_trooper_pvars->gun_moby;
        qcopy(o + 0x10, update_scratch.w.m[3]);
        advance_moby_animation(o);
        update_entity(blarg_trooper_pvars->gun_moby);
        copy_matrix3x4(blarg_trooper_pvars->gun_moby + 0xC0, update_scratch.w.m);
        normalize_vector_triplet(blarg_trooper_pvars->gun_moby + 0xC0);
        {
            char *attached_gun_moby = blarg_trooper_pvars->gun_moby;
            if (moby->flags & 1) {
                MOBY(attached_gun_moby)->flags |= 0x41;
            } else {
                MOBY(attached_gun_moby)->flags &= 0xFFBE;
            }
        }
        refresh_moby_spatial_bounds_from_basis(blarg_trooper_pvars->gun_moby);
        *(unsigned short *)(blarg_trooper_pvars->gun_moby + 0x34) |= 6;
    } else {
        moby_point_position(moby, 1, update_scratch.w.m);
        blarg_trooper_pvars->gun_moby = spawn_moby_attachment(moby, update_scratch.w.m);
    }
    blarg_trooper_pvars->fall_speed = blarg_trooper_pvars->fall_speed + frame_time_sq * 9.8f;
    moby->pos.z = moby->pos.z - (blarg_trooper_pvars->fall_speed - 2.0f);
    {
        float g = probe_ground_height(pos, 0, 0.5f);
        moby->pos.z = moby->pos.z - 2.0f;
        if (moby->pos.z < g) {
            moby->pos.z = g;
            blarg_trooper_pvars->fall_speed = 0.0f;
        }
    }
    if (moby->pos.z < blarg_trooper_pvars->spawn_z - 5.0f) {
        FUN_L00_00257470(moby, 0, -1);
        destroy_blarg_trooper(moby);
        return;
    }
    blarg_trooper_pvars->prev_anim_frame = alive;
}
