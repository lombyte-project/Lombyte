/* Level 04 moby class blarg_trooper. */
#include "types.h"
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
#include "rnc/math/vectors/normalize_vector_triplet.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C5B88), where it is exact; names translated to the US level program. */

extern void spawn_moby_table_puff_pattern(void *) __asm__("FUN_L04_002c46d0");

void destroy_blarg_trooper(struct Moby *moby) __asm__("FUN_L04_002c4808");

void destroy_blarg_trooper(struct Moby *moby) {
    char *data = ((char *)moby->pvars);
    void *other = *(void **)(data + 0xF8);
    if (other != 0) {
        mark_moby_for_removal(other);
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
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
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
    float f8;
    float fC;
    void *p10;
    int   v14;
    char  c18;
    char  c19;
    short h1A;
    float f1C;
    int   pad[4];
};

extern char                *D_L04_0015FFD8;
extern char *D_L04_00173FD8 NOT_SDA;
extern char                 D_L04_00166E00[];
extern char                 blarg_trooper_missing_path_fields_format[] __asm__("D_L04_001DBC80");
extern float                vector_length_xy(void *);
extern int                  DebugPrint();
extern int                  rand(void);
extern void                 record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void spawn_blarg_trooper_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");
void        destroy_blarg_trooper(struct Moby *arg) __asm__("FUN_L04_002c4808");
extern int  func_L00_002594C8_o(void *, void *, void *, float, float, float, float, int) __asm__("FUN_L00_00258490");

void blarg_trooper_update(struct Moby *moby) __asm__("FUN_L04_002c4850");

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
    int              hit_reaction_status;
    float            dmg;
    float            fatal_knockback_yaw;
    float            recovery_knockback_yaw;
    float            one;
    char            *data;
    int              flag;
    float           *pos;
    char            *target;
    float            target_distance;
    float            alive;
    float            best;
    float            target_selection_distance_limit;
    int              path_index_when_selector_nonzero;
    int              path_index_when_selector_zero;
    int              bi;
    int              i;
    struct Path5BD0 *path;
    char            *hit;
    int              r;

    flag = 0;
    data = (char *)moby->pvars;
    if (*(int *)(data + 0x38) != 0 || *(unsigned char *)(data + 0xFE) != 0) {
        flag = 1;
    }
    pos = (float *)&moby->pos;
    manipulator_set_scale(data + 0x110, 3.0f);
    if (moby->visible != 0) {
        if (fast_vec_dist(pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x15;
        }
    }
    if (flag) {
        float x = *(float *)(data + 0xDC);
        target_selection_distance_limit = x + x;
    } else {
        target_selection_distance_limit = *(float *)(data + 0xDC);
    }
    select_moby_combat_target(moby, update_scratch.tgt, target_selection_distance_limit);
    target = *(char **)&update_scratch.tgt[16];
    target_distance = vector_distance_xy(pos, update_scratch.tgt);
    dmg = 0.0f;
    alive = compute_interpolated_record_value(moby);
    if (moby->state != 0 && *(unsigned char *)(data + 0x2E) != 1) {
        char *o = *(char **)(data + 0xF8);
        if (o != 0) {
            mark_moby_for_removal(o);
        }
        mark_moby_for_removal(moby);
        return;
    }
    path_index_when_selector_nonzero = *(int *)(data + 0xE0);
    if (path_index_when_selector_nonzero == -1 ||
        (path_index_when_selector_zero = *(int *)(data + 0xE4),
         path_index_when_selector_zero == -1)) {
        DebugPrint(blarg_trooper_missing_path_fields_format,
                   truncate_float_to_s32((float)(((char *)moby - D_L04_0015FFD8) >> 8)));
        destroy_blarg_trooper(moby);
        return;
    }
    bi = -1;
    best = 0.0f;
    qcopy(update_scratch.v50, &update_scratch.tgt[8]);
    path = ((
        struct Path5BD0 *)l04_paths[*(short *)(data + 0xE8) != 0 ? path_index_when_selector_nonzero
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
    r = hit_reaction(moby, hit, data + 0x20, 0, &hit_reaction_status, &dmg, 0, 4);
    fast_dec_timer_s16(data + 0xF0);
    fast_dec_timer_s16(data + 0xF2);
    if (hit != 0 && moby->state < 0xB && *(short *)(data + 0xF0) == 0) {
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
        t = *(float *)(data + 0x20) - dmg;
        *(float *)(data + 0x20) = t;
        if (t <= 0.0f) {
            r = 1;
        }
        *(unsigned char *)(data + 0xFE) = scale_game_frames(0xF0);
        switch (r) {
        case BLARG_TROOPER_HIT_REACTION_NONE:
            break;
        case BLARG_TROOPER_HIT_REACTION_FATAL:
        case BLARG_TROOPER_HIT_REACTION_FATAL_KNOCKBACK:
            moby->flags &= 0xEFFF;
            *(unsigned char *)(data + 0x67) = 0xFA;
            {
                float a = frame_time_sq * 37.0f;
                float b = frame_time * 17.0f;
                float fatal_knockback_horizontal_speed_adjustment = frame_time_sq * 20.0f;
                float d = frame_time * 13.0f;
                *(float *)(data + 0x80) = a;
                *(float *)(data + 0x88) = b;
                *(float *)(data + 0x84) = fatal_knockback_horizontal_speed_adjustment;
                *(float *)(data + 0x8C) = d;
            }
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                update_scratch.w.p.v70, &fatal_knockback_yaw, data + 0x88, data + 0x8C);
            trooper_start_knockback(fatal_knockback_yaw, moby, data + 0x70, 0xD, 1, 0);
            *(float *)(data + 0xC0) = 12.0f;
            *(float *)(data + 0xC4) = 25.0f;
            moby->state = 0xB;
            FUN_L00_00257470(moby, 0, -1);
            break;
        case BLARG_TROOPER_HIT_REACTION_START_HIT_RECOVERY:
        case BLARG_TROOPER_HIT_REACTION_RECOIL_RECOVERY: {
            int attacker_class;
            *(unsigned char *)(data + 0x67) = 0x96;
            blend_moby_animation(moby, (void *)8, 0, (void *)scale_game_frames(5));
            moby->state = 8;
            attacker_class = *(short *)(*(char **)(hit + 0x20) + 0xA6);
            if (attacker_class == 0x131 || attacker_class == 0xB0) {
                *(float *)(data + 0xF4) = 2.0f / scale_time(15.0f) * 0.5f;
            } else {
                *(float *)(data + 0xF4) = 2.0f / scale_time(15.0f);
            }
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                update_scratch.w.p.v70, &recovery_knockback_yaw, data + 0xF4, &one);
            moby->rot.z = fast_add_rotations(3.1415927f, recovery_knockback_yaw);
            break;
        }
        case BLARG_TROOPER_HIT_REACTION_START_KNOCKBACK_RECOVERY:
        case BLARG_TROOPER_HIT_REACTION_KNOCKBACK_RECOVERY:
            *(unsigned char *)(data + 0x67) = 0xFA;
            blend_moby_animation(moby, (void *)0xC, 0, (void *)scale_game_frames(5));
            moby->state = 0xA;
            *(float *)(data + 0xF4) = 2.0f / scale_time(15.0f);
            *(OvlQuad *)update_scratch.w.p.v70 = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                update_scratch.w.p.v70, &recovery_knockback_yaw, data + 0xF4, &one);
            moby->rot.z = fast_add_rotations(3.1415927f, recovery_knockback_yaw);
            break;
        case BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_NO_ADDITIONAL_REACTION:
        case BLARG_TROOPER_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_NO_FURTHER_RESPONSE:
        case BLARG_TROOPER_HIT_REACTION_IGNORED:
            break;
        }
        trooper_anim_update(moby, (short *)(data + 0x60));
    }
    moby->hit_slot = 0xFF;
    if (*(int *)(data + 0x38) != 0) {
        *(unsigned char *)(data + 0xFE) = scale_game_frames(0xF0);
    }
    *(int *)(data + 0x38) = 0;
    FUN_L00_001fefc8(data + 0xFE);
    fast_dec_timer((int *)(data + 0x108));
    switch (moby->state) {
    case BLARG_TROOPER_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        *(float *)(data + 0x20) = 2.0f;
        *(short *)(data + 0x24) = 2;
        *(unsigned char *)(data + 0x28) = 1;
        *(unsigned char *)(data + 0x2E) = 1;
        *(unsigned char *)(data + 0x29) = 1;
        *(short *)(data + 0xF0) = 0;
        *(unsigned char *)(data + 0xFE) = 0;
        *(unsigned char *)(data + 0xFF) = 0;
        *(float *)(data + 0xD0) = 0.0f;
        {
            float z = moby->pos.z;
            *(unsigned char *)(data + 0x58) = 8;
            *(float *)(data + 0xD4) = z;
            *(unsigned char *)(data + 0x5A) = 0xA;
        }
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        if (*(unsigned char *)(data + 0xFC) != 0) {
            blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(0xA));
            moby->state = 5;
        } else {
            blend_moby_animation(
                moby, (void *)((rand() ^ 1) & 1), 0, (void *)scale_game_frames(0xA));
            moby->state = 1;
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
                             (float *)(data + 0xEC),
                             a,
                             frame_scale_sq * 0.04f,
                             frame_scale_sq * 0.3f,
                             frame_time * 7.330382823944092f);
        }
        if (flag) {
            blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(8));
            moby->state = 3;
            break;
        }
        if (target_distance > 2.5f && best > 1.0f) {
            if (target_distance > 14.0f) {
                blend_moby_animation(moby, (void *)0xA, 0, (void *)scale_game_frames(8));
                moby->state = 2;
                break;
            }
            if (target_distance < 8.0f && *(int *)(data + 0x108) == 0) {
                blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(8));
                moby->state = 3;
                break;
            }
        }
        if (target_distance < 14.0f && target_distance >= 2.5f &&
            absolute_float(moby->pos.z - update_scratch.tgt[2]) < 8.0f) {
            if (*(short *)(data + 0xF2) == 0) {
                *(OvlQuad *)update_scratch.w.p.v90 = 0;
                update_scratch.w.p.v90[2] = 0.5f;
                fast_vec_add(update_scratch.w.p.v70, pos, update_scratch.w.p.v90);
                fast_vec_add(update_scratch.w.p.v80, update_scratch.v50, update_scratch.w.p.v90);
                if (collision_line(update_scratch.w.p.v70, update_scratch.w.p.v80, 0, moby, 0) !=
                        0 &&
                    D_L04_00173FD8 == target) {
                    moby->state = 5;
                    blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
                }
            }
        } else if (target_distance <= 2.5f) {
            int t = *(short *)(data + 0xF2);
            if (t <= 0) {
                moby->state = 7;
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            } else if (scale_game_frames(0x14) < t) {
                *(short *)(data + 0xF2) = scale_game_frames(0x14);
            }
        }
        break;
    case BLARG_TROOPER_STATE_MOVE_TO_PATH_POINT:
        if (target_distance < 13.0f || best < 1.0f) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
            moby->state = 1;
        } else {
            float f;
            {
                float a = fast_arc_tan(update_scratch.v60[0] - moby->pos.x,
                                       update_scratch.v60[1] - moby->pos.y);
                FUN_L00_00258278(moby,
                                 (float *)(data + 0xEC),
                                 a,
                                 frame_scale_sq * 0.03f,
                                 frame_scale_sq * 0.3f,
                                 frame_time * 4.712389f);
            }
            f = *(float *)(data + 0xEC) * 30.0f;
            if (f > 0.7853982f) {
                f = 0.7853982f;
            } else if (f < -0.7853982f) {
                f = -0.7853982f;
            }
            *(float *)(data + 0x178) = f * 0.7f;
            *(float *)(data + 0x1F8) = f * 0.4f;
            update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 2.5f);
            update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 2.5f);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            func_L00_002594C8_o(moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 0);
        }
        break;
    case BLARG_TROOPER_STATE_RUN_TO_TARGET_OR_PATH_POINT:
        *(int *)(data + 0x108) = scale_game_frames(0x64);
        if (flag) {
            int k;
            {
                float a = fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                       update_scratch.v50[1] - moby->pos.y);
                FUN_L00_00258278(moby,
                                 (float *)(data + 0xEC),
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
            if (func_L00_002594C8_o(moby,
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
            if (FUN_L00_001fefc8(data + 0xFF) != 0 && k != moby->prev_seq) {
                blend_moby_animation(moby, (void *)k, 0, (void *)scale_game_frames(0xA));
                *(unsigned char *)(data + 0xFF) = scale_game_frames(0xF);
            }
            if (*(short *)(data + 0xF2) == 0) {
                if (target_distance < 1.0f && fast_difference_between_rotations(
                                                  fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                               update_scratch.v50[1] - moby->pos.y),
                                                  moby->rot.z) < 0.2617994f) {
                    moby->state = 7;
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
                    *(unsigned char *)(data + 0xFF) = 0;
                } else if (target_distance < 14.0f) {
                    moby->state = 5;
                    blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
                    *(unsigned char *)(data + 0xFF) = 0;
                }
            }
        } else {
            if (target_distance < 1.0f &&
                fast_difference_between_rotations(fast_arc_tan(update_scratch.v50[0] - moby->pos.x,
                                                               update_scratch.v50[1] - moby->pos.y),
                                                  moby->rot.z) < 0.2617994f) {
                moby->state = 7;
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            } else if (target_distance > 9.0f) {
                moby->state = 5;
                blend_moby_animation(moby, (void *)3, 0, (void *)scale_game_frames(7));
            } else if (best < 1.0f) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
                moby->state = 1;
            } else {
                {
                    float a = fast_arc_tan(update_scratch.v60[0] - moby->pos.x,
                                           update_scratch.v60[1] - moby->pos.y);
                    FUN_L00_00258278(moby,
                                     (float *)(data + 0xEC),
                                     a,
                                     frame_scale_sq * 0.035f,
                                     frame_scale_sq * 0.3f,
                                     frame_time * 4.712389f);
                }
                update_scratch.w.p.v70[0] = fast_cos(moby->rot.z) * (frame_time * 6.0f);
                update_scratch.w.p.v70[1] = fast_sin(moby->rot.z) * (frame_time * 6.0f);
                update_scratch.w.p.v70[2] = 0.0f;
                fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
                func_L00_002594C8_o(
                    moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 0);
            }
        }
        break;
    case BLARG_TROOPER_STATE_JUMP:
        if (moby->prev_seq != 2) {
            blend_moby_animation(moby, (void *)2, 0, (void *)scale_game_frames(7));
            *(float *)(data + 0xD0) = -(frame_time * 7.668f);
        } else if (moby->anim_flags & 2) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(0xA));
            moby->state = 1;
        }
        break;
    case BLARG_TROOPER_STATE_AIM: {
        float a =
            fast_arc_tan(update_scratch.v50[0] - moby->pos.x, update_scratch.v50[1] - moby->pos.y);
        FUN_L00_00258278(moby,
                         (float *)(data + 0xEC),
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
                moby->state = 2;
                break;
            }
            if (target_distance < 8.0f && *(int *)(data + 0x108) == 0) {
                blend_moby_animation(moby, (void *)0xB, 0, (void *)scale_game_frames(5));
                moby->state = 3;
                break;
            }
        }
        if (target_distance < 2.5f) {
            moby->state = 7;
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
            break;
        }
        if (*(short *)(data + 0xF2) == 0 && target_distance < 14.0f &&
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
                moby->state = 6;
                blend_moby_animation(moby, (void *)5, 0, (void *)scale_game_frames(7));
                *(short *)(data + 0xF2) = scale_game_frames(0x1E);
                *(unsigned char *)(data + 0xFD) = 3;
                break;
            }
        }
        if (target_distance > 14.0f) {
            moby->state = flag ? 3 : 1;
            blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(0xA));
            *(short *)(data + 0xF2) = scale_game_frames(0x1E);
        }
        break;
    case BLARG_TROOPER_STATE_FIRE: {
        char *o;
        {
            float a = fast_arc_tan(update_scratch.tgt[8] - moby->pos.x,
                                   update_scratch.tgt[9] - moby->pos.y);
            FUN_L00_00258278(moby,
                             (float *)(data + 0xEC),
                             a,
                             frame_scale_sq * 0.04f,
                             frame_scale_sq * 0.3f,
                             frame_time * 4.712389f);
        }
        if (target_distance < 2.5f) {
            moby->state = 7;
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(7));
        }
        if (moby->prev_seq == 5 && *(float *)(data + 0xD8) < 2.0f && alive >= 2.0f &&
            (o = *(char **)(data + 0xF8)) != 0) {
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
            FUN_L00_001fefc8(data + 0xFD);
        } else if ((moby->anim_flags & 2) && *(short *)(data + 0xF2) == 0 &&
                   *(unsigned char *)(data + 0xFD) == 0) {
            *(short *)(data + 0xF2) = scale_game_frames(0xB4);
            moby->state = 5;
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(0xA));
        }
        break;
    }
    case BLARG_TROOPER_STATE_MELEE_ATTACK: {
        float a =
            fast_arc_tan(update_scratch.v50[0] - moby->pos.x, update_scratch.v50[1] - moby->pos.y);
        FUN_L00_00258278(moby,
                         (float *)(data + 0xEC),
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
            update_scratch.w.p.blk.f8 = 0.0f;
            update_scratch.w.p.blk.p10 = moby;
            update_scratch.w.p.blk.v14 = 1;
            update_scratch.w.p.blk.f1C = 1.0f;
            set_vector_length_xy(&update_scratch.w.p.blk, &update_scratch.w.p.blk, 1.0f);
            update_scratch.w.p.blk.f8 = 1.0f;
            update_scratch.w.p.blk.fC = 5627.925f;
            update_scratch.w.p.blk.c19 = 1;
            update_scratch.w.p.blk.h1A = (u16)moby->oclass;
            update_scratch.w.p.blk.c18 = 0;
            qcopy(update_scratch.w.p.v70, update_scratch.v50);
            update_scratch.w.p.v70[2] = update_scratch.w.p.v70[2] + 0.75f;
            if (target != 0) {
                record_moby_hit(target, &update_scratch.w.p.blk);
            }
        }
        if (moby->anim_flags & 2) {
            moby->state = 5;
            *(short *)(data + 0xF2) = scale_game_frames(0x14);
            blend_moby_animation(moby, (void *)4, 0, (void *)scale_game_frames(7));
        }
        break;
    case BLARG_TROOPER_STATE_HIT_RECOVERY:
    case BLARG_TROOPER_STATE_BACKWARD_RECOVERY:
    case BLARG_TROOPER_STATE_KNOCKBACK_RECOVERY:
        if (*(float *)(data + 0xF4) > 0.0f) {
            update_scratch.w.p.v70[0] =
                fast_cos(fast_add_rotations(moby->rot.z, 3.1415927f)) * *(float *)(data + 0xF4);
            update_scratch.w.p.v70[1] =
                fast_sin(fast_add_rotations(moby->rot.z, 3.1415927f)) * *(float *)(data + 0xF4);
            update_scratch.w.p.v70[2] = 0.0f;
            fast_vec_add(update_scratch.w.p.v70, update_scratch.w.p.v70, pos);
            *(float *)(data + 0xF4) =
                *(float *)(data + 0xF4) + -2.0f / (scale_time(15.0f) * scale_time(15.0f));
            func_L00_002594C8_o(moby, pos, update_scratch.w.p.v70, 0.5f, 1.0f, 0.5f, 0.5235988f, 1);
        }
        if (moby->anim_flags & 2) {
            *(unsigned short *)(data + 0xE8) ^= 1;
            blend_moby_animation(
                moby, (void *)(moby->state == 9 ? 9 : 0), 0, (void *)scale_game_frames(9));
            moby->state = 1;
            *(int *)(data + 0x108) = scale_game_frames(0x78);
        }
        break;
    case BLARG_TROOPER_STATE_DYING:
        if (step_moby_motion(moby, data + 0x70) & 0x60) {
            destroy_blarg_trooper(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, data + 0x60);
    {
        int current_state = moby->state;
        if (current_state == 1 || current_state == 3 || current_state == 4 || current_state == 5 ||
            current_state == 6) {
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
                *(float *)(data + 0x178) = f * 0.5f;
                *(float *)(data + 0x1F8) = f * 0.5f;
            }
        }
    }
    if (moby->flags & 0x8000) {
        *(float *)(data + 0x178) = -*(float *)(data + 0x178);
        *(float *)(data + 0x1F8) = -*(float *)(data + 0x1F8);
    }
    update_joint_animation(moby, data + 0x110, 5, frame_scale_sq * 0.018f, frame_scale_sq * 0.3f);
    update_joint_animation(moby, data + 0x190, 6, frame_scale_sq * 0.024f, frame_scale_sq * 0.3f);
    if (*(char **)(data + 0xF8) != 0) {
        char *o;
        build_moby_bone_transform(moby, 1, (char *)update_scratch.w.m);
        o = *(char **)(data + 0xF8);
        qcopy(o + 0x10, update_scratch.w.m[3]);
        advance_moby_animation(o);
        update_entity(*(char **)(data + 0xF8));
        copy_matrix3x4(*(char **)(data + 0xF8) + 0xC0, update_scratch.w.m);
        normalize_vector_triplet(*(char **)(data + 0xF8) + 0xC0);
        {
            char *attached_gun_moby = *(char **)(data + 0xF8);
            if (moby->flags & 1) {
                MOBY(attached_gun_moby)->flags |= 0x41;
            } else {
                MOBY(attached_gun_moby)->flags &= 0xFFBE;
            }
        }
        refresh_moby_spatial_bounds_from_basis(*(char **)(data + 0xF8));
        *(unsigned short *)(*(char **)(data + 0xF8) + 0x34) |= 6;
    } else {
        moby_point_position(moby, 1, update_scratch.w.m);
        *(char **)(data + 0xF8) = spawn_moby_attachment(moby, update_scratch.w.m);
    }
    *(float *)(data + 0xD0) = *(float *)(data + 0xD0) + frame_time_sq * 9.8f;
    moby->pos.z = moby->pos.z - (*(float *)(data + 0xD0) - 2.0f);
    {
        float g = probe_ground_height(pos, 0, 0.5f);
        moby->pos.z = moby->pos.z - 2.0f;
        if (moby->pos.z < g) {
            moby->pos.z = g;
            *(float *)(data + 0xD0) = 0.0f;
        }
    }
    if (moby->pos.z < *(float *)(data + 0xD4) - 5.0f) {
        FUN_L00_00257470(moby, 0, -1);
        destroy_blarg_trooper(moby);
        return;
    }
    *(float *)(data + 0xD8) = alive;
}
