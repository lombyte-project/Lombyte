/* Shared moby classes: seeker. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qzero.h"
#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/game/fun_00213928.h"

/* Per-instance variables of the oClass 52 moby (moby->pvars). */
typedef struct {
    u8           pad0[0x20];
    f32          unk20;
    s16          unk24;
    u8           pad26[2];
    u8           unk28;
    u8           pad29[0x2F];
    u8           unk58;
    u8           pad59;
    u8           unk5A;
    u8           pad5B[0x15];
    Vec4         vel;
    u8           pad80[0x50];
    s32          unkD0;
    u8           padD4[4];
    f32          unkD8;
    f32          unkDC;
    u8           padE0[0x14];
    f32          speed;
    u8           padF8[0x68];
    struct Moby *target;
    s32          unk164;
    u8           pad168[0x18];
    s32          unk180;
    s32          timer;
    u8           pad188[4];
    f32          heading_speed;
    u8           pad190[4];
    s32          voice;
    s32          moving;
    u8           pad19C[0xE4];
    u8           unk280[1];
} Moby52Vars;

#include "rnc/overlay/shared/enemies/seeker_globals.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_path.h"

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
extern void FUN_L01_0026d930(void *);
extern void FUN_L01_0026e090(s32, s32);
extern void update_seeker_damage_and_target(struct Moby *) __asm__("FUN_L09_002c6230");

void seeker_update(struct Moby *moby) __asm__("FUN_L09_002c5990");

void seeker_update(struct Moby *moby) {
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
    Moby52Vars      *vars = (Moby52Vars *)moby->pvars;
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
    target = vars->target;
    FUN_L00_00263ac8(moby, 0, vars->unk280, 2.7f);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, &D_L09_00166F40) < 29.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x17;
    }
    mode = seeker_state_dispatch_mode;
    if (mode == 2) {
        return;
    }
    switch (moby->state) {
    case SEEKER_STATE_INITIALIZE:
        vars->unk20 = 3.0f;
        vars->unk24 = 3;
        vars->unk28 = 2;
        vars->unk5A = 6;
        vars->unk58 = 8;
        if (vars->unk180 == -1) {
            debug_print(seeker_invalid_bound_path_format, moby->save_id);
            mark_moby_for_removal(moby);
            break;
        }
        FUN_L01_0026d930(&vars->unkD0);
        vars->unkDC = 0.5f;
        vars->unkD8 = 0.5f;
        vars->unkD0 = truncate_float_to_s32(512.0f);
        vars->speed = frame_time * 7.75f;
        moby->state = 1;
        if (moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        vars->voice = -1;
        break;
    case SEEKER_STATE_IDLE_HOVER:
        moby->pos.z += (probe_ground_height(&moby->pos, 0, 0.5f) + 0.5f - moby->pos.z) * 0.1f;
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->heading_speed,
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
        if (vars->unk164 != 2) {
            if (moby->group != 0xFF) {
                FUN_L01_0026e090(moby->group, 1);
            }
            if (vars->voice == -1) {
                vars->voice = allocate_voice_for_target_entry(1, 4, moby);
            }
            vars->moving = 1;
            moby->state = 2;
        }
        break;
    case SEEKER_STATE_CHASE_ANIMATION_WAIT:
        if (moby->anim_flags & 2) {
            if (moby->group != 0xFF) {
                FUN_L01_0026e090(moby->group, 1);
            }
            vars->moving = 1;
            moby->state = 4;
            if (vars->voice == -1) {
                vars->voice = allocate_voice_for_target_entry(1, 4, moby);
            }
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
            if (vars->timer == 0) {
                vars->timer = scale_game_frames(180);
            }
        }
        break;
    case SEEKER_STATE_CHASE:
    case SEEKER_STATE_TIMED_CHASE:
        z = moby->pos.z;
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                       &vars->heading_speed,
                       frame_time_sq * 8.726646f,
                       frame_time_sq * 8.726646f,
                       frame_time * 12.566371f);
        target_distance_xy = vector_distance_xy(&moby->pos, &target->pos);
        if (target_distance_xy > 1.9f) {
            vars->moving = 1;
        }
        if (target_distance_xy < 1.4f) {
            vars->moving = 0;
        }
        if (vars->moving != 0) {
            vel.f[0] = fast_cos(moby->rot.z) * vars->speed;
            vel.f[1] = fast_sin(moby->rot.z) * vars->speed;
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
        if (target_distance_xy < 6.0f && moby->state == 2) {
            moby->state = 3;
            if (moby->prev_seq != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case SEEKER_STATE_HURT_KNOCKBACK:
        height = moby->pos.z;
        mine_fall(moby, &vars->vel);
        moby->pos.z = height;
        vars->vel.f[2] = 0.0f;
        if (fast_vec_length(&vars->vel) < frame_time * 2.0f) {
            if (vars->voice == -1) {
                vars->voice = allocate_voice_for_target_entry(1, 4, moby);
            }
            vars->moving = 1;
            moby->state = 4;
            if (vars->timer == 0) {
                vars->timer = scale_game_frames(180);
            }
        }
        break;
    case SEEKER_STATE_FATAL_KNOCKBACK:
        voice = vars->voice;
        if (voice != -1) {
            slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);
            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        vars->voice = -1;
        if (mine_fall(moby, &vars->vel) & 0x121) {
            cart_burst(moby, &moby->pos, 0, 0.5f, 13.0f);
            mark_moby_for_removal(moby);
        }
        break;
    case SEEKER_STATE_EXPLODE:
        FUN_L00_00257470(moby, 0, -1);
        voice = vars->voice;
        if (voice != -1) {
            slot = (VoicePoolWindow *)((u8 *)&voice_pool + voice * 0x70);
            if ((struct Moby *)slot->voice.owner == moby && slot->voice.state != 0) {
                release_voice_slot(voice);
            }
        }
        vars->voice = -1;
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
    char          p0[0x20];
    float         health;
    char          p24[0x14];
    int           f38;
    char          p3C[0x24];
    char          react[7];
    unsigned char reactTime;
    char          p68[8];
    float         f70[4];
    float         f80;
    float         f84;
    float         f88;
    float         f8C;
    int           f90;
    int           f94;
    float         f98;
    char          p9C[0x11];
    unsigned char bAD;
    char          pAE[0x12];
    float         fC0;
    float         fC4;
    char          pC8[0x58];
    float         target[4];
    char          p130[0x30];
    Moby         *f160;
    int           f164;
    char          p168[0x18];
    int           path;
    int           timer;
    float         reach;
    char          p18C[4];
    int           retreat;
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
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
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
extern void func_L00_0025E4B0_2C73D0(void *, void *) __asm__("FUN_L00_0025d458");
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
    char        hit_pos[16];
    int         hit;
    float       damage;
    float       knock_angle;
    float       knock_angle_fatal;
    Data       *data;
    Hit_2C73D0 *source;
    int         response;
    int         result;
    int         lost_state;

    data = (struct Data *)moby->pvars;
    if (moby->state == 0) {
        return;
    }
    moby->scale =
        moby->scale + ((((Cls *)moby->pclass)->scale * D_L09_0016140C) - moby->scale) * 0.1f;
    if (fast_dec_timer(&data->timer) == 2) {
        moby->state = 7;
        return;
    }
    if (data->timer == scale_game_frames(0xA) || data->timer == scale_game_frames(0x14) ||
        data->timer == scale_game_frames(0x28) || data->timer == scale_game_frames(0x3C) ||
        data->timer == scale_game_frames(0x78) || data->timer == scale_game_frames(0xB3)) {
        data->reactTime = 0xF0;
        func_L00_0025E4B0_2C73D0(moby, data->react);
    }
    damage = 0.0f;
    source = find_moby_hit(moby, 0x330000, 0);
    response = fun_00213928(moby, source, &data->health, 0, &hit, &damage, 0, 4);
    if (hit != 1 && moby->state != 6) {
        data->health = data->health - damage;
        if (data->health <= 0.0f) {
            response = 1;
        }
        data->f90 = truncate_float_to_s32(512.0f);
        data->f80 = seeker_knockback_gravity_scale * D_0015ED70;
        data->f84 = D_0015ED70 * 30.0f;
        data->f98 = 0.0f;
        data->f94 = 1;
        data->bAD = 0;
        allocate_voice_for_target_entry(8, 0, moby);

        switch (response) {
        case RESPONSE_REACTION_ONLY:
        case 10:
            data->reactTime = 0xFA;
            break;
        case RESPONSE_HURT_KNOCKBACK:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            data->fC0 = 3.0f;
            data->fC4 = 6.0f;
            data->f88 = seeker_hurt_knockback_xy_scale * D_0015ED6C;
            data->f8C = 0.0f;
            knock_angle = fast_arc_tan(moby->pos.x - source->owner->pos.x,
                                       moby->pos.y - source->owner->pos.y);
            *(Q_2C73D0 *)hit_pos = source->pos;
            func_L00_0025BBA0_2C73D0(hit_pos, &knock_angle, &data->f88, &data->f8C);
            func_L00_0025D5B0_2C73D0(moby, data->f70, knock_angle, 4, 1, 0);
            moby->state = 5;
            if (moby->group != 0xFF) {
                func_L01_0026F040_2C73D0(moby->group, 1);
            }
            data->reactTime = 0x78;
            break;
        case RESPONSE_FATAL_KNOCKBACK:
        case 2:
            moby->flags &= 0xEFFF;
            data->f80 = seeker_knockback_gravity_scale * D_0015ED70;
            data->f88 = seeker_fatal_knockback_xy_scale * D_0015ED6C;
            data->f8C = seeker_fatal_knockback_z_scale * D_0015ED6C;
            data->fC0 = 14.0f;
            data->fC4 = 28.0f;
            knock_angle_fatal = fast_arc_tan(moby->pos.x - source->owner->pos.x,
                                             moby->pos.y - source->owner->pos.y);
            *(Q_2C73D0 *)hit_pos = source->pos;
            func_L00_0025BBA0_2C73D0(hit_pos, &knock_angle_fatal, &data->f88, &data->f8C);
            func_L00_0025D5B0_2C73D0(moby, data->f70, knock_angle_fatal, 4, 1, 0);
            mark_moby_for_removal_cf(moby, 0, -1);
            moby->state = 6;
            data->reactTime = 0xF0;
            break;
        case RESPONSE_NONE:
        case RESPONSE_NO_REACTION:
        default:
            break;
        }
        func_L00_0025E4B0_2C73D0(moby, data->react);
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, data->react);
    if (data->f38 != 0 || moby->command == 1) {
        data->retreat =
            truncate_float_to_s32(func_001F9878_2C73D0(random_float_between(180.0f, 240.0f)));
        data->f38 = 0;
        moby->command = 0;
    }
    fast_dec_timer(&data->retreat);
    if (random_integer_below(4) == 0) {
        float *goal;
        if (data->retreat != 0) {
            data->reach = 24.0f;
            goal = data->target;
            result = func_L00_00260D30_2C73D0(moby, goal, 24.0f);
            lost_state = 2;
        } else {
            Path *path;
            data->reach = 12.0f;
            path = ((Path *)l09_paths[data->path]);
            goal = data->target;
            result = activate_effect_l08(moby, goal, 12.0f, 0, 0, path->pts, path->count);
            lost_state = 2;
        }
        if (result != lost_state && data->retreat == 0) {
            if (data->reach < vector_distance_xy(&moby->pos, goal)) {
                data->f164 = lost_state;
            } else if (3.0f < absolute_float(moby->pos.z - data->target[2])) {
                data->f164 = lost_state;
            }
        }
    } else {
        Moby *target = data->f160;
        if (target != 0 && target->state != 0xFE && target->state != 0xFD) {
            qcopy(data->target, &target->pos);
        } else {
            data->f160 = 0;
            data->f164 = 2;
        }
    }
    if (data->f160 == 0) {
        data->f160 = *(Moby **)D_0013E633_2C73D0;
    }
}
/* Ground enemy update: damage reaction, leash and wake checks, then its chase / attack / return state machine. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C6B30.c: func_L09_002E39E0), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2E39E0;

typedef struct {
    char           pad0[0x20];
    float          health;
    short          f24;
    short          cooldown;
    char           pad28[1];
    unsigned char  f29;
    char           pad2A[0xE];
    int            hurt;
    char           pad3C[0x1C];
    unsigned char  f58;
    char           pad59[1];
    unsigned char  f5A;
    char           pad5B[0x6D];
    short          fC8;
    char           padCA[6];
    unsigned char *settings;
    char           padD4[0x3C];
    char           effects[7];
    unsigned char  f117;
    char           pad118[8];
    char           motion[0x10];
    float          f130;
    float          f134;
    float          up;
    float          back;
    char           pad140[4];
    int            f144;
    char           pad148[0x15];
    unsigned char  f15D;
    char           pad15E[0x12];
    float          f170;
    float          f174;
    char           pad178[8];
    int            steer;
    char           pad184[4];
    float          f188;
    float          f18C;
    float          f190;
    char           pad194[4];
    float          fall;
    char           pad19C[8];
    float          f1A4;
    char           pad1A8[0x28];
    V_2E39E0       home;
    char           pad1E0[0x10];
    char           turn[4];
    float          side;
    float          side_deg;
    float          range;
    char           pad200[4];
    float          speed;
    int            f208;
    char           pad20C[4];
    int            side_timer;
    short          alert;
    short          leash;
    Moby          *anchor;
    int            path;
    char           pad220[4];
    int            home_timer;
    int            hidden;
    char           pad22C[4];
    char           sound[4];
} Data_2E39E0;

typedef struct {
    char           pad0[0x20];
    Moby          *moby;
    char           pad24[6];
    unsigned short kind;
    float          damage;
} Hit_2E39E0;

typedef struct {
    float x, y, z;
    char  padC[0x34];
    Moby *who;
    char  pad44[0xC];
} __attribute__((aligned(16))) Target_2E39E0;

extern int  D_L09_001618D8; /* no foreign declaration */
extern int  FUN_L00_002dc9f0();
extern s32  rand(void);
extern void FUN_L00_00257470_c2(void *, int, int) __asm__("FUN_L00_00257470");
int         select_moby_combat_target(char *a, char *o, float best) __asm__("FUN_L00_0025fcb8");
int         select_list_point_toward_target(char *a0,
                                            void *list,
                                            char *pt,
                                            char *out) __asm__("FUN_L01_0028b5e0");
s32         FUN_L00_00258b50(void *m, void *c, void *v, Vec4 *out, f32 f);
void        FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void        FUN_L00_0025a120_c(char *o) __asm__("FUN_L00_0025a120");
void        FUN_L00_00263ac8_c(float x, int a, int b, unsigned char *p) __asm__("FUN_L00_00263ac8");
void        FUN_L01_0026d930_c(char *p) __asm__("FUN_L01_0026d930");
void        FUN_L01_0026e090_c2(int i, int v) __asm__("FUN_L01_0026e090");
extern char D_0013E633_2E39E0[] __asm__("D_0013F3D0");
extern unsigned char D_001413F4[];
extern float         D_0015EE6C_2E39E0 __asm__("D_0015ED6C");
extern float         D_0015EE70_2E39E0 __asm__("D_0015ED70");
extern float         D_L09_00166FC0_2E39E0[] __asm__("D_L09_00166F40");
extern Moby         *D_L09_00160064_2E39E0 __asm__("D_L09_0015FFE4");
extern float         impulse __asm__("D_L09_001618C8") __attribute__((sda));
extern float         lift_2E39E0 __asm__("D_L09_001618CC") __attribute__((sda));
extern unsigned char settings_2E39E0 __asm__("D_L09_001618D8") __attribute__((sda));
extern void          FUN_L01_0026e090(int, int);
extern void          knock_2E39E0(Moby *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void          explode_2E39E0(Moby *, int, int) __asm__("FUN_L00_00257470");
extern void          effects_start_2E39E0(Moby *, void *) __asm__("FUN_L00_0025d458");
extern void          sound_2E39E0(float, Moby *, int, void *) __asm__("FUN_L00_00263ac8");
extern void          wake_2E39E0(Moby *) __asm__("FUN_L00_0025a120");
extern int           FUN_L04_00252e18_c(Moby *, void *, float) __asm__("FUN_L04_00252e18");
extern void          steer_init_2E39E0(int *) __asm__("FUN_L01_0026d930");
extern int           rand_2E39E0(void) __asm__("FUN_001160d8");
extern void approach_w(Moby *, float, void *, float, float, float) __asm__("FUN_L00_00258278");
extern void FUN_L01_0028b5e0_c(Moby *, char *, void *, void *) __asm__("FUN_L01_0028b5e0");
extern int  move_w(Moby *, int *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern int  recover_2E39E0(Moby *, void *) __asm__("FUN_L00_002dc9f0");
extern void effects_update_2E39E0(Moby *, void *) __asm__("FUN_L00_0025d538");

void update_ground_melee_enemy(Moby *moby) __asm__("FUN_L09_002e27d8");

void update_ground_melee_enemy(Moby *moby) {
    enum {
        GROUND_ENEMY_STATE_INITIALIZE = 0,
        GROUND_ENEMY_STATE_IDLE = 1,
        GROUND_ENEMY_STATE_ALERT_TURN = 2,
        GROUND_ENEMY_STATE_CHASE = 3,
        GROUND_ENEMY_STATE_MELEE_ATTACK = 4,
        GROUND_ENEMY_STATE_RETURN_HOME = 5,
        GROUND_ENEMY_STATE_HURT_KNOCKBACK = 6,
        GROUND_ENEMY_STATE_RECOVER_TO_IDLE = 7,
        GROUND_ENEMY_STATE_HURT_RECOVERY = 9,
        GROUND_ENEMY_STATE_BLOCKED_RETURN_HOME = 12,
        GROUND_ENEMY_STATE_LEASHED_FORWARD_WALK = 13,
        GROUND_ENEMY_STATE_FATAL_KNOCKBACK = 99
    };
    V_2E39E0      previous;
    Target_2E39E0 query;
    V_2E39E0      step;
    V_2E39E0      goal;
    V_2E39E0      away;
    int           hit_index;
    float         damage;
    Data_2E39E0  *data = (Data_2E39E0 *)moby->pvars;
    int           kind = 2;
    int           expired;
    float         ground;

    if (moby->state != 99 && moby->state != 7) {
        Hit_2E39E0 *hit;
        damage = 0.0f;
        hit = find_moby_hit(moby, 0x330000, 0);
        fun_00213928(moby, hit, &data->health, 0, &hit_index, &damage, 0, 4);
        fast_dec_timer_s16(&data->cooldown);
        if (data->cooldown < scale_game_frames(45) && hit != 0) {
            if ((unsigned short)(hit->moby->oclass - 0xB0) < 2) {
                if (data->cooldown != 0)
                    hit->damage = 0.0f;
                else
                    hit->damage = 1.0f;
            }
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
            if (hit->damage != 0.0f) {
                float health = data->health - hit->damage;
                float lift = lift_2E39E0 * D_0015EE6C_2E39E0;
                float back = impulse * D_0015EE6C_2E39E0;
                data->f144 = 9;
                data->f130 = 0.008f;
                data->f134 = 0.0005f;
                data->health = health;
                data->up = lift;
                data->back = back;
                data->f15D = 0;
                if (health <= 0.0f) {
                    float up_die = D_0015EE6C_2E39E0 * 8.0f;
                    float back_die = D_0015EE6C_2E39E0 * 10.0f;
                    moby->flags &= 0xEFFF;
                    data->up = up_die;
                    data->back = back_die;
                    if (hit->kind == 180) {
                        data->up = up_die * 1.35f;
                        data->back = back_die * 1.35f;
                    }
                    knock_2E39E0(moby,
                                 data->motion,
                                 fast_arc_tan(moby->pos.x - query.x, moby->pos.y - query.y),
                                 5,
                                 1,
                                 0);
                    data->f170 = 11.0f;
                    data->f174 = 18.0f;
                    if (D_001413F4[0] == 2)
                        explode_2E39E0(moby, 0x200, -1);
                    else
                        explode_2E39E0(moby, 0, -1);
                    moby->collision = 0;
                    moby->state = 99;
                    data->f117 = 120;
                    effects_start_2E39E0(moby, data->effects);
                } else {
                    if (hit->kind == 180) {
                        data->up = lift * 1.35f;
                        data->back = back * 1.35f;
                    }
                    knock_2E39E0(moby,
                                 data->motion,
                                 fast_arc_tan(moby->pos.x - query.x, moby->pos.y - query.y),
                                 6,
                                 1,
                                 0);
                    data->f170 = 5.0f;
                    data->f174 = 10.0f;
                    moby->state = 6;
                    data->f117 = 250;
                    data->cooldown = scale_game_frames(60);
                    effects_start_2E39E0(moby, data->effects);
                }
            }
        }
        moby->hit_slot = 0xFF;
        sound_2E39E0(2.1f, moby, 0, data->sound);
        if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L09_00166FC0_2E39E0) < 38.0f) {
            wake_2E39E0(moby);
            moby->shadow_range = 30;
        }
        if (vector_distance_xy(&moby->pos, D_0013E633_2E39E0) > 40.0f)
            return;
        if (data->hidden != 0) {
            if ((*(unsigned char *)&((Hero *)D_0013E633_2E39E0)->unk2020[1]) != 2) {
                moby->update_dist = 0xFF;
                moby->collision = 0;
                moby->flags = (moby->flags | 0x41) & 0xEFFF;
                return;
            }
            moby->flags &= 0xFFBE;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
            moby->update_dist = 0x40;
            moby->flags |= 0x1000;
        }
        if (data->hurt != 0)
            data->alert = scale_game_frames(240);
        data->hurt = 0;
        fast_dec_timer(&data->f208);
        if (fast_dec_timer_s16(&data->alert))
            data->range = 12.0f;
        else
            data->range = 20.0f;
        kind = FUN_L04_00252e18_c(moby, &query, data->range);
    }
    qcopy(&previous, &moby->pos);
    expired = fast_dec_timer(&data->home_timer) != 0;
    ground = -1.0f;
    switch (moby->state) {
    case GROUND_ENEMY_STATE_INITIALIZE:
        qcopy(&data->home, &moby->pos);
        moby->state = 1;
        data->f29 = 0;
        data->health = 1.0f;
        data->f24 = 1;
        data->f5A = 2;
        data->f58 = 8;
        data->alert = 0;
        data->f208 = 0;
        steer_init_2E39E0(&data->steer);
        data->steer = 0x38D;
        data->f188 = data->f18C = 2.0f;
        data->f1A4 = data->speed * D_0015EE6C_2E39E0;
        if (rand_2E39E0() & 1)
            moby->flags |= 0x8000;
        data->settings = &settings_2E39E0;
        if (moby->prev_seq != 2)
            blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        ground = moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
        break;
    case GROUND_ENEMY_STATE_IDLE:
        fast_vec_sub(&step, &query, &moby->pos);
        if ((kind < 2 || fast_vec_length(&step) < data->range + 4.0f) &&
            absolute_float(moby->pos.z - query.z) < 8.0f && !random_integer_below(19)) {
            moby->state = 2;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (moby->command == 1 && !random_integer_below(19)) {
            if (random_integer_below(0x100) & 1)
                data->side_deg = -data->side_deg;
            data->side = data->side_deg * 0.017453292f;
            data->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            moby->command = 0;
        }
        break;
    case GROUND_ENEMY_STATE_ALERT_TURN:
    case 8:
    case 10:
    case 11:
        approach_w(moby,
                   fast_arc_tan(query.x - moby->pos.x, query.y - moby->pos.y),
                   data->turn,
                   0.02f,
                   0.3f,
                   0.1f);
        fast_vec_sub(&step, &query, &moby->pos);
        if ((kind < 2 || fast_vec_length(&step) < data->range) &&
            absolute_float(moby->pos.z - query.z) < 8.0f && !random_integer_below(19)) {
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
            if (random_integer_below(0x100) & 1)
                data->side_deg = -data->side_deg;
            data->side = data->side_deg * 0.017453292f;
            data->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            moby->command = 0;
        } else if (moby->command == 1 && !random_integer_below(19)) {
            if (random_integer_below(0x100) & 1)
                data->side_deg = -data->side_deg;
            data->side = data->side_deg * 0.017453292f;
            data->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            moby->command = 0;
        }
        if (moby->prev_seq == 4 && (moby->anim_flags & 2))
            blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        break;
    case GROUND_ENEMY_STATE_CHASE: {
        int   flags;
        float steep;
        steep = absolute_float(moby->pos.z - query.z);
        steep /= vector_distance_xy(&moby->pos, &query);
        if (data->path != -1)
            FUN_L01_0028b5e0_c(moby, ((char *)l09_paths[data->path]), &query, &goal);
        else
            qcopy(&goal, &query);
        approach_w(moby,
                   fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y) + data->side,
                   data->turn,
                   0.05f,
                   0.3f,
                   0.2f);
        step.x = 2.0f * fast_cos(moby->rot.z);
        step.y = 2.0f * fast_sin(moby->rot.z);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        flags = move_w(moby, &data->steer, &goal, &step, 1.0f);
        if (fast_dec_timer(&data->side_timer)) {
            data->side_timer = scale_game_frames((int)random_float_between(30.0f, 90.0f));
            data->side = -data->side;
        }
        fast_vec_sub(&away, &query, &data->home);
        if ((flags & 4) || (vector_distance_xy(&moby->pos, &query) < 1.5f && !(flags & 2))) {
            moby->state = 4;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (data->f190 < D_0015EE6C_2E39E0 || 1.0f < steep || (flags & 2)) {
            data->home_timer = scale_game_frames(50);
            moby->state = 5;
        } else if (fast_vec_length(&away) < data->range + 4.0f &&
                   absolute_float(moby->pos.z - query.z) < 8.0f) {
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
        } else if (moby->command != 1) {
            moby->state = 5;
        }
        moby->command = 0;
        break;
    }
    case GROUND_ENEMY_STATE_MELEE_ATTACK:
        approach_w(moby,
                   fast_arc_tan(query.x - moby->pos.x, query.y - moby->pos.y),
                   data->turn,
                   0.05f,
                   0.3f,
                   0.2f);
        if (moby->seq == moby->prev_seq && compute_interpolated_record_value(moby) == 13.0f &&
            absolute_float(moby->pos.z - query.z) < 0.5f &&
            vector_distance_xy(&moby->pos, &query) < 2.0f &&
            fast_difference_between_rotations(
                fast_arc_tan(query.x - moby->pos.x, query.y - moby->pos.y), moby->rot.z) <
                0.2617994f) {
            step.x = fast_cos(moby->rot.z) * 0.2f;
            step.y = fast_sin(moby->rot.z) * 0.2f;
            step.z = 0.0f;
            qcopy(&goal, &query);
            goal.z += 0.75f;
            spawn_nearby_effect(query.who, moby, 1, &goal, &step, 1.0f);
        }
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &query) > 1.5f) {
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        }
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        data->fall = data->fall - D_0015EE70_2E39E0 * 9.8f;
        moby->pos.z = moby->pos.z + data->fall;
        if (moby->pos.z < ground) {
            data->fall = 0.0f;
            moby->pos.z = ground;
        }
        break;
    case GROUND_ENEMY_STATE_RETURN_HOME: {
        int flags;
        if (data->path != -1)
            FUN_L01_0028b5e0_c(moby, ((char *)l09_paths[data->path]), &data->home, &goal);
        else
            qcopy(&goal, &data->home);
        approach_w(moby,
                   fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y),
                   data->turn,
                   0.02f,
                   0.3f,
                   0.1f);
        step.x = 2.0f * fast_cos(moby->rot.z);
        step.y = 2.0f * fast_sin(moby->rot.z);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        flags = move_w(moby, &data->steer, &goal, &step, 1.0f);
        if (vector_distance_xy(&moby->pos, &data->home) < 2.0f) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        }
        fast_vec_sub(&away, &query, &data->home);
        if (expired && fast_vec_length(&away) < data->range) {
            if (moby->group != 0xFF)
                FUN_L01_0026e090(moby->group, 1);
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (expired && moby->command == 1) {
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (flags & 2) {
            moby->pos.x = previous.x;
            moby->pos.y = previous.y;
            moby->state = 12;
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(random_int_between(10, 13)));
        }
        moby->command = 0;
        break;
    }
    case GROUND_ENEMY_STATE_HURT_KNOCKBACK:
        if (mine_fall(moby, data->motion) & 1) {
            moby->state = 9;
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(random_int_between(10, 13)));
            return;
        }
        if (moby->pos.z < 0.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case GROUND_ENEMY_STATE_RECOVER_TO_IDLE:
        if (recover_2E39E0(moby, data->motion)) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, 0);
            data->fC8 = 0;
        }
        break;
    case GROUND_ENEMY_STATE_HURT_RECOVERY:
        if (compute_interpolated_record_value(moby) > 29.0f) {
            moby->state = 3;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
            return;
        }
        break;
    case GROUND_ENEMY_STATE_FATAL_KNOCKBACK:
        if (mine_fall(moby, data->motion) & 0x40) {
            clear_u64_value(&step);
            cart_burst(moby, &moby->pos, 6, 0.5f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < 0.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case GROUND_ENEMY_STATE_BLOCKED_RETURN_HOME: {
        int   flags;
        float steep;
        steep = absolute_float(moby->pos.z - query.z);
        steep /= vector_distance_xy(&moby->pos, &query);
        if (data->path != -1)
            FUN_L01_0028b5e0_c(moby, ((char *)l09_paths[data->path]), &data->home, &goal);
        else
            qcopy(&goal, &data->home);
        approach_w(moby,
                   fast_arc_tan(goal.x - moby->pos.x, goal.y - moby->pos.y),
                   data->turn,
                   0.05f,
                   0.3f,
                   0.2f);
        step.x = 2.0f * fast_cos(moby->rot.z);
        step.y = 2.0f * fast_sin(moby->rot.z);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        flags = move_w(moby, &data->steer, &goal, &step, 1.0f);
        fast_vec_sub(&away, &query, &data->home);
        if ((flags & 4) || (vector_distance_xy(&moby->pos, &query) < 1.5f && steep <= 1.0f)) {
            moby->state = 4;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (steep <= 1.0f && !(flags & 2)) {
            moby->state = 5;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        } else if ((flags & 2) && expired) {
            moby->state = 1;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(random_int_between(10, 13)));
        } else if (moby->anim_flags & 2) {
            int animation;
            random_integer_below(4);
            animation = random_integer_below(4) ? 2 : 4;
            blend_moby_animation(moby, animation, 0, scale_game_frames(random_int_between(10, 13)));
        }
        moby->command = 0;
        break;
    }
    case GROUND_ENEMY_STATE_LEASHED_FORWARD_WALK: {
        Moby *other;
        if (moby->seq != 0 && moby->prev_seq != 0)
            blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        step.x = fast_cos(moby->rot.z) * (data->speed * D_0015EE6C_2E39E0);
        step.y = fast_sin(moby->rot.z) * (data->speed * D_0015EE6C_2E39E0);
        step.z = 0.0f;
        fast_vec_add(&goal, &moby->pos, &step);
        for (other = D_L09_00160064_2E39E0; other != 0; other = other->next) {
            if (other != moby && (signed char)other->state >= 0 &&
                (other->oclass == 0 || other->oclass == 193)) {
                if (vector_distance_xy(&goal, &other->pos) < 2.0f) {
                    float old_z = moby->pos.z;
                    float bearing =
                        fast_arc_tan(other->pos.x - moby->pos.x, other->pos.y - moby->pos.y);
                    float turn_side = fast_subtract_rotations(moby->rot.z, bearing);
                    float turn = D_0015EE6C_2E39E0 * 1.5707964f;
                    if (!(turn_side > 0.0f))
                        turn = -turn;
                    moby->rot.z = fast_add_rotations(moby->rot.z, turn);
                    fast_vec_sub(&goal, &moby->pos, &other->pos);
                    fast_vec_normalize(&goal, &goal, 2.0f);
                    fast_vec_add(&goal, &goal, &other->pos);
                    goal.z = old_z;
                }
            }
        }
        qcopy(&moby->pos, &goal);
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        data->fall = data->fall - D_0015EE70_2E39E0 * 9.8f;
        moby->pos.z = moby->pos.z + data->fall;
        if (moby->pos.z < ground) {
            data->fall = 0.0f;
            moby->pos.z = ground;
        }
        if (data->anchor == 0 || vector_distance_xy(&moby->pos, &data->anchor->pos) >
                                     convert_integer_to_float(data->leash)) {
            moby->state = 5;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(random_int_between(10, 13)));
        }
        moby->command = 0;
        break;
    }
    }
    if (ground == -1.0f)
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
    if (ground != 0.0f && moby->pos.z - ground < 0.1f && collision_hit_material() == 1) {
        cart_burst(moby, &moby->pos, 6, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    effects_update_2E39E0(moby, data->effects);
}

int   D_L15_001619D8 = 0x7000000;
float D_L15_001619C8 = 7.0f;
float D_L15_001619CC = 3.0f;
float D_L16_0016143C = 0.5f;
