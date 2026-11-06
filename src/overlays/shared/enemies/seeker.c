/* Shared moby class seeker: its update and its damage and target handling. */
#include "types.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/mobys/moby_state.h"
#include "rnc/globals.h"
#include "qzero.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "rnc/overlay/shared/mobys/ground_move_control.h"

/* Per-instance variables of the oClass 52 moby (moby->pvars). */
enum {
    SEEKER_STATE_INITIALIZE = 0,
    SEEKER_STATE_IDLE_HOVER = 1,
    SEEKER_STATE_CHASE = 2,
    SEEKER_STATE_CHASE_ANIMATION_WAIT = 3,
    SEEKER_STATE_TIMED_CHASE = 4,
    SEEKER_STATE_HURT_KNOCKBACK = 5,
    SEEKER_STATE_FATAL_KNOCKBACK = 6,
    SEEKER_STATE_EXPLODE = 7
};

typedef struct {
    u8                       pad0[0x20];
    f32                      health;
    s16                      max_health;
    u8                       pad26[2];
    u8                       hit_response_class;
    u8                       pad29[0x2F];
    u8                       unk58;
    u8                       pad59;
    u8                       unk5A;
    u8                       pad5B[0x15];
    Vec4                     vel;
    u8                       pad80[0x50];
    struct GroundMoveControl move_control; /* 0xD0 */
    u8                       pad120[0x40];
    struct Moby             *target;
    s32                      target_selection_status;
    u8                       pad168[0x18];
    s32                      bound_path;
    s32                      timer;
    u8                       pad188[4];
    f32                      heading_speed;
    u8                       pad190[4];
    s32                      voice;
    s32                      moving;
    u8                       pad19C[0xE4];
    u8                       unk280[1];
} Moby52Vars;

#include "rnc/overlay/shared/enemies/seeker_globals.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/shared/world/level_path.h"

extern Vec4f D_L09_00166F40;
extern u8    seeker_invalid_bound_path_format[] __asm__("D_L09_00208320");

extern void debug_print(void *, s16) __asm__("FUN_001e93b0");
extern s32  FUN_00213260(s32);
extern void FUN_L00_00257470(struct Moby *, s32, s32);
extern s32  FUN_L00_00257b90(s32, s32);
extern void FUN_L00_00258830(struct Moby *, Vec4 *, s32, f32, f32, f32);
extern void FUN_L00_0025a120(struct Moby *);
extern void approach_angle(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025be00");
extern void spawn_explosion(struct Moby *,
                            Vec4 *,
                            void *,
                            s32,
                            f32,
                            f32,
                            f32,
                            f32,
                            f32,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            s32,
                            s32,
                            s32,
                            s32) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263ac8(struct Moby *, s32, void *, f32);
extern void FUN_L01_0026e090(s32, s32);
extern void update_seeker_damage_and_target(struct Moby *) __asm__("FUN_L09_002c6230");

void seeker_update(struct Moby *moby) __asm__("FUN_L09_002c5990");

/* Moby class 52 */
void seeker_update(struct Moby *moby) {
    enum {
        SEEKER_DISPATCH_MODE_SKIP_STATE_UPDATE = 2
    };
    Moby52Vars      *seeker_instance_vars = (Moby52Vars *)moby->pvars;
    struct Moby     *target;
    Vec4             vel;
    s32              mode;
    s32              voice;
    f32              target_distance_xy;
    f32              z;
    f32              height;
    f32              limit;
    f32              vertical_follow_adjustment;
    VoicePoolWindow *slot;

    update_seeker_damage_and_target(moby);
    target = seeker_instance_vars->target;
    FUN_L00_00263ac8(moby, 0, seeker_instance_vars->unk280, 2.7f);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, &D_L09_00166F40) < 29.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x17;
    }
    mode = seeker_state_dispatch_mode;
    if (mode == SEEKER_DISPATCH_MODE_SKIP_STATE_UPDATE) {
        return;
    }
    switch (moby->state) {
    case SEEKER_STATE_INITIALIZE:
        seeker_instance_vars->health = 3.0f;
        seeker_instance_vars->max_health = 3;
        seeker_instance_vars->hit_response_class = 2;
        seeker_instance_vars->unk5A = 6;
        seeker_instance_vars->unk58 = 8;
        if (seeker_instance_vars->bound_path == -1) {
            debug_print(seeker_invalid_bound_path_format, moby->save_id);
            mark_moby_for_removal(moby);
            break;
        }
        init_ground_move_control(&seeker_instance_vars->move_control);
        seeker_instance_vars->move_control.max_climb = 0.5f;
        seeker_instance_vars->move_control.max_drop = 0.5f;
        seeker_instance_vars->move_control.collision_radius_1024ths = truncate_float_to_s32(512.0f);
        seeker_instance_vars->move_control.max_speed = frame_time * 7.75f;
        moby->state = SEEKER_STATE_IDLE_HOVER;
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        seeker_instance_vars->voice = -1;
        break;
    case SEEKER_STATE_IDLE_HOVER:
        moby->pos.z += (probe_ground_height(&moby->pos, 0, 0.5f) + 0.5f - moby->pos.z) * 0.1f;
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &seeker_instance_vars->heading_speed,
                       frame_time_sq * 8.726646f,
                       frame_time_sq * 8.726646f,
                       frame_time * 12.566371f);
        if (moby->anim_flags & 2) {
            if (FUN_00213260(0x13) == 0) {
                if (moby->prev_seq != 2) {
                    blend_moby_animation(moby, 2, 0, FUN_L00_00257b90(7, 12));
                }
            } else if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, FUN_L00_00257b90(7, 14));
            }
            moby->anim_speed = random_float_between(0.95f, 1.05f);
        }
        if (seeker_instance_vars->target_selection_status != 2) {
            if (moby->group != 0xFF) {
                FUN_L01_0026e090(moby->group, 1);
            }
            if (seeker_instance_vars->voice == -1) {
                seeker_instance_vars->voice = allocate_voice_for_target_entry(1, 4, moby);
            }
            seeker_instance_vars->moving = 1;
            moby->state = SEEKER_STATE_CHASE;
        }
        break;
    case SEEKER_STATE_CHASE_ANIMATION_WAIT:
        if (moby->anim_flags & 2) {
            if (moby->group != 0xFF) {
                FUN_L01_0026e090(moby->group, 1);
            }
            seeker_instance_vars->moving = 1;
            moby->state = SEEKER_STATE_TIMED_CHASE;
            if (seeker_instance_vars->voice == -1) {
                seeker_instance_vars->voice = allocate_voice_for_target_entry(1, 4, moby);
            }
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
            if (seeker_instance_vars->timer == 0) {
                seeker_instance_vars->timer = scale_game_frames(180);
            }
        }
        break;
    case SEEKER_STATE_CHASE:
    case SEEKER_STATE_TIMED_CHASE:
        z = moby->pos.z;
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &seeker_instance_vars->heading_speed,
                       frame_time_sq * 8.726646f,
                       frame_time_sq * 8.726646f,
                       frame_time * 12.566371f);
        target_distance_xy = vector_distance_xy(&moby->pos, &target->pos);
        if (target_distance_xy > 1.9f) {
            seeker_instance_vars->moving = 1;
        }
        if (target_distance_xy < 1.4f) {
            seeker_instance_vars->moving = 0;
        }
        if (seeker_instance_vars->moving != 0) {
            vel.f[0] = fast_cos(moby->rot.z) * seeker_instance_vars->move_control.max_speed;
            vel.f[1] = fast_sin(moby->rot.z) * seeker_instance_vars->move_control.max_speed;
            vel.f[2] = 0.0f;
            vertical_follow_adjustment = hero.motion.pos.f[2] + 0.8f - z;
            if (moby->pos.z < hero.motion.pos.f[2] + 0.8f) {
                vertical_follow_adjustment *= 0.1f;
            } else {
                vertical_follow_adjustment *= 0.05f;
            }
            vel.f[2] += vertical_follow_adjustment;
            limit = frame_scale * 0.3f;
            if (vel.f[2] > limit) {
                vel.f[2] = limit;
            } else {
                limit = frame_scale * -0.3f;
                if (vel.f[2] < limit) {
                    vel.f[2] = limit;
                }
            }
            FUN_L00_00258830(moby, &vel, 0, 0.0f, 0.5f, 0.0f);
        } else {
            qzero(&vel);
            vertical_follow_adjustment = hero.motion.pos.f[2] + 0.2f - z;
            if (moby->pos.z < hero.motion.pos.f[2] + 0.2f) {
                vertical_follow_adjustment *= 0.1f;
            } else {
                vertical_follow_adjustment *= 0.05f;
            }
            vel.f[2] += vertical_follow_adjustment;
            limit = frame_scale * 0.3f;
            if (vel.f[2] > limit) {
                vel.f[2] = limit;
            } else {
                limit = frame_scale * -0.3f;
                if (vel.f[2] < limit) {
                    vel.f[2] = limit;
                }
            }
            FUN_L00_00258830(moby, &vel, 0, 0.0f, 0.5f, 0.0f);
        }
        if (target_distance_xy < 6.0f && moby->state == SEEKER_STATE_CHASE) {
            moby->state = SEEKER_STATE_CHASE_ANIMATION_WAIT;
            if (moby->prev_seq != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case SEEKER_STATE_HURT_KNOCKBACK:
        height = moby->pos.z;
        step_moby_motion(moby, &seeker_instance_vars->vel);
        moby->pos.z = height;
        seeker_instance_vars->vel.f[2] = 0.0f;
        if (fast_vec_length(&seeker_instance_vars->vel) < frame_time * 2.0f) {
            if (seeker_instance_vars->voice == -1) {
                seeker_instance_vars->voice = allocate_voice_for_target_entry(1, 4, moby);
            }
            seeker_instance_vars->moving = 1;
            moby->state = SEEKER_STATE_TIMED_CHASE;
            if (seeker_instance_vars->timer == 0) {
                seeker_instance_vars->timer = scale_game_frames(180);
            }
        }
        break;
    case SEEKER_STATE_FATAL_KNOCKBACK:
        voice = seeker_instance_vars->voice;
        if (voice != -1) {
            slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);
            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        seeker_instance_vars->voice = -1;
        if (step_moby_motion(moby, &seeker_instance_vars->vel) & 0x121) {
            cart_burst(moby, &moby->pos, 0, 0.5f, 13.0f);
            mark_moby_for_removal(moby);
        }
        break;
    case SEEKER_STATE_EXPLODE:
        FUN_L00_00257470(moby, 0, -1);
        voice = seeker_instance_vars->voice;
        if (voice != -1) {
            slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);
            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        seeker_instance_vars->voice = -1;
        clear_u64_value(&vel);
        spawn_explosion(moby,
                        &vel,
                        &moby->pos,
                        10,
                        2.0f,
                        1.0f,
                        4.0f,
                        2.0f,
                        9.0f,
                        1.0f,
                        15.0f,
                        3,
                        0x10,
                        0,
                        0,
                        0,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        break;
    }
}

#include "qcopy.h"

/* Enemy update while it chases: takes hits (the result of func_L00_0025B4D0 picks the reaction),
 * re-rolls its retreat timer and steers along its path or straight at the target.
 * In the steering arms the goal pointer and the "lost" code (2) are set in each arm: retail loads 2 in
 * both arms and keeps the goal in a saved register across the arm's call. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_002C73D0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int Q_2C73D0 __attribute__((mode(TI)));

typedef struct {
    char  p0[0x24];
    float scale;
} Cls;

typedef struct Data {
    char                   p0[0x20];
    float                  health;
    char                   p24[0x14];
    int                    f38;
    char                   p3C[0x24];
    struct HitColorFlash   hit_flash;
    struct KnockbackMotion knockback;
    char                   pad_60[0x50];
    float                  target[4];
    char                   p130[0x30];
    Moby                  *f160;
    int                    f164;
    char                   p168[0x18];
    int                    path;
    int                    timer;
    float                  reach;
    char                   p18C[4];
    int                    retreat;
} Data;

typedef struct {
    char     p0[0x10];
    Q_2C73D0 pos;
    Moby    *owner;
} Hit_2C73D0;

typedef struct {
    int   count;
    int   pad[3];
    float pts[1][4];
} Path;

typedef int OvlQuad __attribute__((mode(TI)));

extern char *FUN_L00_0025a420(void *, int, int);
extern float D_0015ED6C;
extern float D_0015ED70;

#include "rnc/overlay/shared/enemies/seeker_data/update_seeker_damage_and_target.h"

extern float D_L09_0016140C __attribute__((sda));
extern int   scale_time() __asm__("FUN_001f96b0");
extern void  FUN_L00_00257470_c(void *, int, int) __asm__("FUN_L00_00257470");
extern void  start_hit_color_flash(void *m, void *p) __asm__("FUN_L00_0025d458");
extern void  update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
int          FUN_L00_0025ff38(float best,
                              char *a,
                              char *o,
                              int  *excluded,
                              int   excluded_count,
                              void *region,
                              int   region_count);
int  select_moby_record_by_xy_distance(char *a, char *o, float best) __asm__("FUN_L04_00252e18");
void trooper_knockback_arc(void *v, float *ang, float *s1, float *s2) __asm__("FUN_L00_0025ab48");
void trooper_start_knockback(u8 *a, void *b, int c, int d, int e, float f) __asm__("FUN_L00_0025c558");
void        FUN_L01_0026e090_c(int i, int v) __asm__("FUN_L01_0026e090");
extern void start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void func_L00_0025BBA0_2C73D0(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
func_L00_0025D5B0_2C73D0(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void  func_L01_0026F040_2C73D0(int, int) __asm__("FUN_L01_0026e090");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern float func_001F9878_2C73D0(float) __asm__("FUN_001f96b0");
extern int   func_L00_00260D30_2C73D0(void *, void *, float) __asm__("FUN_L04_00252e18");
extern int
activate_effect_l08(void *, void *, float, int, int, void *, int) __asm__("FUN_L00_0025ff38");
extern char D_0013E633_2C73D0[] __asm__("D_001413D0");

void update_seeker_damage_and_target(Moby *moby) __asm__("FUN_L09_002c6230");

void update_seeker_damage_and_target(Moby *moby) {
    enum {
        RESPONSE_NONE = 0,
        RESPONSE_FATAL_KNOCKBACK = 1,
        RESPONSE_HURT_KNOCKBACK = 3,
        RESPONSE_REACTION_ONLY = 9,
        RESPONSE_NO_REACTION = 11
    };
    enum {
        SEEKER_TARGET_STATUS_LOST = 2
    };

    char        hit_pos[16];
    int         hit;
    float       damage;
    float       knock_angle;
    float       knock_angle_fatal;
    Data       *seeker_vars;
    Hit_2C73D0 *source;
    int         response;
    int         target_selection_status;
    int         lost_state;

    seeker_vars = (struct Data *)moby->pvars;
    if (moby->state == SEEKER_STATE_INITIALIZE) {
        return;
    }
    moby->scale =
        moby->scale + ((((Cls *)moby->pclass)->scale * D_L09_0016140C) - moby->scale) * 0.1f;
    if (fast_dec_timer(&seeker_vars->timer) == 2) {
        moby->state = SEEKER_STATE_EXPLODE;
        return;
    }
    if (seeker_vars->timer == scale_game_frames(0xA) ||
        seeker_vars->timer == scale_game_frames(0x14) ||
        seeker_vars->timer == scale_game_frames(0x28) ||
        seeker_vars->timer == scale_game_frames(0x3C) ||
        seeker_vars->timer == scale_game_frames(0x78) ||
        seeker_vars->timer == scale_game_frames(0xB3)) {
        seeker_vars->hit_flash.flash_red = 0xF0;
        start_hit_color_flash(moby, &seeker_vars->hit_flash);
    }
    damage = 0.0f;
    source = find_moby_hit(moby, 0x330000, 0);
    response =
        resolve_moby_hit_response(moby, source, &seeker_vars->health, 0, &hit, &damage, 0, 4);
    if (hit != 1 && moby->state != SEEKER_STATE_FATAL_KNOCKBACK) {
        seeker_vars->health = seeker_vars->health - damage;
        if (seeker_vars->health <= 0.0f) {
            response = 1;
        }
        seeker_vars->knockback.collision_radius_1024ths = truncate_float_to_s32(512.0f);
        seeker_vars->knockback.gravity = seeker_knockback_gravity_scale * D_0015ED70;
        seeker_vars->knockback.speed_approach_step = D_0015ED70 * 30.0f;
        seeker_vars->knockback.collision_z_offset = 0.0f;
        seeker_vars->knockback.flags = 1;
        seeker_vars->knockback.unk3D = 0;
        allocate_voice_for_target_entry(8, 0, moby);

        switch (response) {
        case RESPONSE_REACTION_ONLY:
        case 10:
            seeker_vars->hit_flash.flash_red = 0xFA;
            break;
        case RESPONSE_HURT_KNOCKBACK:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            seeker_vars->knockback.apex_animation_frame = 3.0f;
            seeker_vars->knockback.landing_animation_frame = 6.0f;
            seeker_vars->knockback.speed = seeker_hurt_knockback_xy_scale * D_0015ED6C;
            seeker_vars->knockback.vertical_speed = 0.0f;
            knock_angle = fast_arc_tan(moby->pos.x - source->owner->pos.x,
                                       moby->pos.y - source->owner->pos.y);
            *(Q_2C73D0 *)hit_pos = source->pos;
            func_L00_0025BBA0_2C73D0(hit_pos,
                                     &knock_angle,
                                     &seeker_vars->knockback.speed,
                                     &seeker_vars->knockback.vertical_speed);
            func_L00_0025D5B0_2C73D0(moby, &seeker_vars->knockback, knock_angle, 4, 1, 0);
            moby->state = SEEKER_STATE_HURT_KNOCKBACK;
            if (moby->group != 0xFF) {
                func_L01_0026F040_2C73D0(moby->group, 1);
            }
            seeker_vars->hit_flash.flash_red = 0x78;
            break;
        case RESPONSE_FATAL_KNOCKBACK:
        case 2:
            moby->flags &= 0xEFFF;
            seeker_vars->knockback.gravity = seeker_knockback_gravity_scale * D_0015ED70;
            seeker_vars->knockback.speed = seeker_fatal_knockback_xy_scale * D_0015ED6C;
            seeker_vars->knockback.vertical_speed = seeker_fatal_knockback_z_scale * D_0015ED6C;
            seeker_vars->knockback.apex_animation_frame = 14.0f;
            seeker_vars->knockback.landing_animation_frame = 28.0f;
            knock_angle_fatal = fast_arc_tan(moby->pos.x - source->owner->pos.x,
                                             moby->pos.y - source->owner->pos.y);
            *(Q_2C73D0 *)hit_pos = source->pos;
            func_L00_0025BBA0_2C73D0(hit_pos,
                                     &knock_angle_fatal,
                                     &seeker_vars->knockback.speed,
                                     &seeker_vars->knockback.vertical_speed);
            func_L00_0025D5B0_2C73D0(moby, &seeker_vars->knockback, knock_angle_fatal, 4, 1, 0);
            mark_moby_for_removal_cf(moby, 0, -1);
            moby->state = SEEKER_STATE_FATAL_KNOCKBACK;
            seeker_vars->hit_flash.flash_red = 0xF0;
            break;
        case RESPONSE_NONE:
        case RESPONSE_NO_REACTION:
        default:
            break;
        }
        start_hit_color_flash(moby, &seeker_vars->hit_flash);
    }
    moby->hit_slot = 0xFF;
    update_hit_color_flash(moby, &seeker_vars->hit_flash);
    if (seeker_vars->f38 != 0 || moby->command == 1) {
        seeker_vars->retreat =
            truncate_float_to_s32(func_001F9878_2C73D0(random_float_between(180.0f, 240.0f)));
        seeker_vars->f38 = 0;
        moby->command = 0;
    }
    fast_dec_timer(&seeker_vars->retreat);
    if (random_integer_below(4) == 0) {
        float *goal;
        if (seeker_vars->retreat != 0) {
            seeker_vars->reach = 24.0f;
            goal = seeker_vars->target;
            target_selection_status = func_L00_00260D30_2C73D0(moby, goal, 24.0f);
            lost_state = SEEKER_TARGET_STATUS_LOST;
        } else {
            Path *path;
            seeker_vars->reach = 12.0f;
            path = ((Path *)l09_paths[seeker_vars->path]);
            goal = seeker_vars->target;
            target_selection_status =
                activate_effect_l08(moby, goal, 12.0f, 0, 0, path->pts, path->count);
            lost_state = SEEKER_TARGET_STATUS_LOST;
        }
        if (target_selection_status != lost_state && seeker_vars->retreat == 0) {
            if (seeker_vars->reach < vector_distance_xy(&moby->pos, goal)) {
                seeker_vars->f164 = lost_state;
            } else if (3.0f < absolute_float(moby->pos.z - seeker_vars->target[2])) {
                seeker_vars->f164 = lost_state;
            }
        }
    } else {
        Moby *target = seeker_vars->f160;
        if (target != 0 && target->state != MOBY_STATE_REMOVAL_MARKER &&
            target->state != MOBY_STATE_ALTERNATE_REMOVAL_MARKER) {
            qcopy(seeker_vars->target, &target->pos);
        } else {
            seeker_vars->f160 = 0;
            seeker_vars->f164 = 2;
        }
    }
    if (seeker_vars->f160 == 0) {
        seeker_vars->f160 = *(Moby **)D_0013E633_2C73D0;
    }
}

float D_L16_0016143C = 0.5f;
