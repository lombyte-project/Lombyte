/* Level 04 moby class circular_saw_enemy: flying saw blades that flock and attack, and their suck cannon methods. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/overlay/shared/mobys/update_object_wander_movement.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l04/mobys/circular_saw_enemy_globals.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/shared/weapons/suckcannon.h"
#include "rnc/audio/voice_alloc.h"

/* The engine-sound fields of the moby's pvars. */
typedef struct {
    /* 0x000 */ u8  pad0[0x230];
    /* 0x230 */ f32 volume;
    /* 0x234 */ f32 pitch;
    /* 0x238 */ s32 voice;
} EngineSoundVars;

extern s32  voice_is_playing(long moby, s32 voice) __asm__("FUN_L00_0028d8c0");
extern f32  approach_value(f32 *value, f32 target, f32 step) __asm__("FUN_00213ed8");
extern void vendor_volume(s32 voice, s32 volume) __asm__("FUN_L01_002a1968");
extern void engine_voice_set_pitch(s32 voice, s32 pitch) __asm__("FUN_L00_0028df38");

/* Keeps the moby's engine voice playing, easing its volume and pitch toward the animation's levels. */
void update_circular_saw_engine_sound(struct Moby *moby) __asm__("FUN_L04_002c1b80");

void update_circular_saw_engine_sound(struct Moby *moby) {
    enum {
        CIRCULAR_SAW_ANIM_IDLE = 0,
        CIRCULAR_SAW_ANIM_MOVE_FORWARD = 1,
        CIRCULAR_SAW_ANIM_MOVE_BACKWARD = 2,
        CIRCULAR_SAW_ANIM_STRAFE_NEGATIVE_YAW = 3,
        CIRCULAR_SAW_ANIM_STRAFE_POSITIVE_YAW = 4,
        CIRCULAR_SAW_ANIM_JUMP = 5,
        CIRCULAR_SAW_ANIM_ALERT = 6,
        CIRCULAR_SAW_ANIM_ATTACK = 7
    };
    EngineSoundVars *engine_sound_vars = (EngineSoundVars *)moby->pvars;
    f32              volume;
    f32              pitch;

    if (moby->state != 9) {
        if (voice_is_playing((long)moby, engine_sound_vars->voice) == 0 && moby->prev_seq < 8) {
            engine_sound_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            engine_sound_vars->volume = convert_integer_to_float(0x400);
            engine_sound_vars->pitch = 0;
        }
        switch (moby->prev_seq) {
        case CIRCULAR_SAW_ANIM_IDLE:
            pitch = 0.0f;
            volume = convert_integer_to_float(0x400) * 0.6f;
            break;
        case CIRCULAR_SAW_ANIM_MOVE_FORWARD:
            pitch = 3.0f;
            volume = convert_integer_to_float(0x400);
            break;
        case CIRCULAR_SAW_ANIM_MOVE_BACKWARD:
            pitch = -2.0f;
            volume = convert_integer_to_float(0x400);
            break;
        case CIRCULAR_SAW_ANIM_STRAFE_NEGATIVE_YAW:
        case CIRCULAR_SAW_ANIM_STRAFE_POSITIVE_YAW:
            pitch = 4.0f;
            volume = convert_integer_to_float(0x400);
            break;
        case CIRCULAR_SAW_ANIM_JUMP:
        case CIRCULAR_SAW_ANIM_ALERT:
        case CIRCULAR_SAW_ANIM_ATTACK:
            pitch = 0.0f;
            volume = convert_integer_to_float(0x400) * 0.8f;
            break;
        default:
            goto release;
        }
    } else {
    release:
        if (engine_sound_vars->voice != -1) {
            VoicePoolWindow *voice_slot_window =
                (VoicePoolWindow *)((u8 *)&voice_pool + engine_sound_vars->voice * 0x70);
            if (voice_slot_window->voice.owner == (VoiceMoby *)moby &&
                voice_slot_window->voice.state != 0)
                release_voice_slot(engine_sound_vars->voice);
        }
        engine_sound_vars->voice = -1;
        return;
    }
    approach_value(
        &engine_sound_vars->volume, volume, convert_integer_to_float(0x400) * frame_time * 2.0f);
    approach_value(&engine_sound_vars->pitch, pitch, frame_time * 8.0f);
    vendor_volume(engine_sound_vars->voice, truncate_float_to_s32(engine_sound_vars->volume));
    engine_voice_set_pitch(engine_sound_vars->voice,
                           truncate_float_to_s32(engine_sound_vars->pitch));
}

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00257470(void *, int, int);

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C30F0), where it is exact; names translated to the US level program. */

extern void FUN_0022da68(int, int, char *);
extern void func_L00_0025F4A8_alt(void *,
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

void spawn_circular_saw_destruction_effects(struct Moby *moby) __asm__("FUN_L04_002c1d70");

void spawn_circular_saw_destruction_effects(struct Moby *moby) {
    float explosion_position[4];
    float unused_destruction_effect_argument[4];
    char *circular_saw_enemy_pvars = (char *)moby->pvars;
    int   engine_voice_index;
    int   invalid_voice_index;

    clear_u64_value(unused_destruction_effect_argument);
    engine_voice_index = *(int *)(circular_saw_enemy_pvars + 0x238);
    if (engine_voice_index != -1) {
        char *voice_slot_window = (char *)&voice_pool + engine_voice_index * 0x70;
        if (*(char **)(voice_slot_window + 0x88) == moby) {
            if (*(unsigned char *)(voice_slot_window + 0x74) != 0)
                release_voice_slot(engine_voice_index);
        }
    }
    invalid_voice_index = -1;
    *(int *)(circular_saw_enemy_pvars + 0x238) = invalid_voice_index;
    FUN_L00_00257470(moby, 0, -1);
    moby_point_position(moby, 5, explosion_position);
    func_L00_0025F4A8_alt(moby,
                          unused_destruction_effect_argument,
                          explosion_position,
                          0.0f,
                          0.0f,
                          10,
                          3,
                          4,
                          2.0f,
                          1.0f,
                          100000.0f,
                          1.0f,
                          -1,
                          15.0f,
                          1,
                          1,
                          invalid_voice_index,
                          0);
    FUN_0022da68(1, 0, moby);
}
/* Check that a moby's spot is clear of the other mobys of class 0x154. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C3218), where it is exact; names translated to the US level program. */

extern char *D_L04_0015FFE4;
extern void  select_moby_combat_target(char *, float *, float) __asm__("FUN_L00_0025fcb8");

int check_circular_saw_melee_attack_spacing(struct Moby *moby) __asm__("FUN_L04_002c1e98");

int check_circular_saw_melee_attack_spacing(struct Moby *moby) {
    char *circular_saw_pvars = (char *)moby->pvars;
    float target_selection_output[20];
    float target_distance;
    float target_selection_distance_limit;
    char *other_moby;
    int   flag = 0;
    if (*(int *)(circular_saw_pvars + 0x38) != 0 || *(short *)(circular_saw_pvars + 0x136) != 0) {
        flag = 1;
    }
    if (flag) {
        float x = *(float *)(circular_saw_pvars + 0x140);
        target_selection_distance_limit = x + x;
    } else {
        target_selection_distance_limit = *(float *)(circular_saw_pvars + 0x140);
    }
    select_moby_combat_target(moby, target_selection_output, target_selection_distance_limit);
    target_distance = vector_distance_xy((float *)&moby->pos, target_selection_output);
    for (other_moby = D_L04_0015FFE4; other_moby != 0;
         other_moby = ((char *)MOBY(other_moby)->next)) {
        if (other_moby != moby && MOBY(other_moby)->oclass == 0x154) {
            float *other_position = (float *)&MOBY(other_moby)->pos;
            if (vector_distance_xy((float *)&moby->pos, other_position) < 3.0f ||
                vector_distance_xy(other_position, target_selection_output) < target_distance) {
                return 0;
            }
        }
    }
    return 1;
}
/* Flock steering for moby m: picks its path target, pushes away from same-class mobys within 3 units,
 * keeps its spacing to the target unless another flock member is closer to it, writes the steer point
 * to out and returns the yaw toward it. (The passed speed is recomputed from the moby's state.) */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C3338), where it is exact; names translated to the US level program. */

float circular_saw_flock_separation_weight_gp __asm__("D_L04_00161884") = 6.0f;
float compute_circular_saw_flock_steering(struct Moby *moby,
                                          void        *out,
                                          float        speed) __asm__("FUN_L04_002c1fb8");

float compute_circular_saw_flock_steering(struct Moby *moby,
                                          void        *steering_point_output,

                                          float speed) {
    float away[4];
    float target[4];
    float pad[4][4];
    char *circular_saw_flock_pvars = (char *)moby->pvars;
    char *candidate_moby;
    float target_dist;
    float weight;
    int   is_closest;
    int   is_alert = 0;
    if (*(int *)(circular_saw_flock_pvars + 0x38) != 0 ||
        *(short *)(circular_saw_flock_pvars + 0x136) != 0)
        is_alert = 1;
    if (is_alert) {
        speed = *(float *)(circular_saw_flock_pvars + 0x140) +
                *(float *)(circular_saw_flock_pvars + 0x140);
    } else {
        speed = *(float *)(circular_saw_flock_pvars + 0x140);
    }
    select_moby_combat_target(moby, target, speed);
    target_dist = vector_distance_xy((float *)&moby->pos, target);
    clear_u64_value(steering_point_output);
    is_closest = 1;
    candidate_moby = D_L04_0015FFE4;
    weight = 0.0f;
    for (; candidate_moby != 0; candidate_moby = *(char **)(candidate_moby + 0x28)) {
        if (candidate_moby == moby || *(short *)(candidate_moby + 0xA6) != moby->oclass)
            continue;
        if (vector_distance_xy((float *)(candidate_moby + 0x10), (float *)&moby->pos) < 3.0f) {
            fast_vec_sub(away, &moby->pos, candidate_moby + 0x10);
            fast_vec_scale(away, away, circular_saw_flock_separation_weight_gp);
            fast_vec_add(steering_point_output, steering_point_output, away);
            weight += circular_saw_flock_separation_weight_gp;
        }
        if (vector_distance_xy((float *)(candidate_moby + 0x10), target) < target_dist)
            is_closest = 0;
    }
    if (!is_closest) {
        float offset_dir;
        fast_vec_sub(away, &moby->pos, target);
        if (target_dist < 4.5f) {
            offset_dir = 5.0f;
        } else if (5.5f < target_dist) {
            offset_dir = -5.0f;
        } else {
            offset_dir = 0.0f;
        }
        fast_vec_normalize(away, away, offset_dir * 10.0f);
        weight += 10.0f;
        fast_vec_add(steering_point_output, steering_point_output, away);
        fast_vec_scale(steering_point_output, steering_point_output, 1.0f / weight);
        fast_vec_add(steering_point_output, steering_point_output, &moby->pos);
    } else {
        qcopy(steering_point_output, target);
    }
    if (vector_distance_xy((float *)&moby->pos, (float *)steering_point_output) < 1.0f) {
        qcopy(steering_point_output, &moby->pos);
        return fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
    }
    return fast_arc_tan(((float *)steering_point_output)[0] - moby->pos.x,
                        ((float *)steering_point_output)[1] - moby->pos.y);
}

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float D_L04_00166F40[4];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   FUN_L00_001fefc8(void *);
extern int   is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L00_0025a120(void *);
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);

/* Update of the circular saw enemy: hit reaction, state machine (idle, chase, patrol, attack), movement and gravity. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002C35F0), where it is exact; names translated to the US level program. */

typedef float V4_35F0[4] __attribute__((aligned(16)));

typedef struct D_35F0 {
    char           pad00[0x20];
    float          hp;
    short          f24;
    char           pad26[2];
    unsigned char  f28;
    unsigned char  f29;
    char           pad2A[4];
    unsigned char  f2E;
    char           pad2F;
    float          f30;
    char           pad34[4];
    int            f38;
    char           pad3C[0x1C];
    unsigned char  f58;
    char           pad59;
    unsigned char  f5A;
    char           pad5B[5];
    int            f60;
    float          f64;
    float          f68;
    float          f6C;
    char           pad70[8];
    float          f78;
    char           pad7C[8];
    float          f84;
    char           pad88[0x10];
    int            f98;
    char           pad9C[0x14];
    char           fB0[7];
    unsigned char  fB7;
    char           padB8[8];
    char           fC0[0x10];
    float          fD0;
    float          fD4;
    float          fD8;
    char           padDC[8];
    int            fE4;
    char           padE8[0x15];
    unsigned char  fFD;
    char           padFE[0x22];
    V4_35F0        f120;
    float          f130;
    char           pad134[2];
    short          f136;
    int            f138;
    float          f13C;
    float          f140;
    short          f144;
    short          f146;
    char           pad148[4];
    unsigned char  f14C;
    unsigned char  f14D;
    unsigned short f14E;
    int            f150;
    int            f154;
    short          f158;
    short          f15A;
    char           pad15C[4];
    int            f160[4];
    char           pad170[0x78];
    short          f1E8;
    char           pad1EA[6];
    short         *f1F0;
    char           pad1F4[0x44];
    int            f238;
    char           pad23C[4];
    V4_35F0        f240;
    char           pad250[4];
    float          f254;
    float          f258;
    float          f25C;
    char           pad260[0x10];
    unsigned char  f270[4];
} D_35F0;

typedef struct M_35F0 {
    char          pad00[0x10];
    V4_35F0       pos;
    unsigned char state;
    char          pad21[0xF];
    unsigned char f30;
    unsigned char f31;
    char          pad32[0x16];
    float         rot;
    char          pad4C[6];
    unsigned char f52;
    unsigned char f53;
    char          pad54[0x1C];
    unsigned char f70;
    char          pad71[7];
    D_35F0       *data;
    char          pad7C[3];
    char          f7F;
    char          pad80[0x24];
    unsigned char fA4;
    char          padA5;
    union {
        short          s;
        unsigned short u;
    } cls;
    char    padA8[0x18];
    V4_35F0 fC0;
} M_35F0;

typedef struct {
    char    pad00[0x80];
    V4_35F0 pos;
    char    pad90[0x1B0];
    M_35F0 *f240;
    char    pad244[0xE4C];
    M_35F0 *f1090;
} H_35F0;

typedef struct {
    V4_35F0 pos;
    char    pad10[0x30];
    int     f40;
    char    pad44[0xC];
} T_35F0;

typedef struct {
    V4_35F0 v;
    M_35F0 *moby;
    int     f14;
    int     f18;
    float   f1C;
    int     f20;
    char    pad24[0xC];
} S_35F0;

typedef struct {
    char    pad00[0x20];
    M_35F0 *att;
    char    pad24[8];
    float   f2C;
} Hit_35F0;

typedef struct {
    int     count;
    int     pad[3];
    V4_35F0 pts[1];
} P_35F0;

typedef struct {
    int v[4];
} I4_35F0;

extern CollisionHit coll_hit __asm__("D_L04_00173FC0");

I4_35F0 D_L04_001618A0 = {
    {1, 4, 2, 3}
};

I4_35F0 D_L04_00161890 = {
    {1, 2, 3, 4}
};

extern float fast_cos(float);
extern float fast_sin(float);
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern int FUN_L00_002dc9f0(void *, void *);
short      D_L04_00161878 = 0x101;
extern void FUN_L01_0026d930(char *p);
extern void update_circular_saw_engine_sound(struct Moby *) __asm__("FUN_L04_002c1b80");
extern void release_voice_slot(int);
extern void transform_vector_by_basis(void *, void *, void *);
extern char D_0013E550[];
extern void func_L00_002592B0_b(
    char *moby, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258278");

void circular_saw_enemy_update(M_35F0 *moby) __asm__("FUN_L04_002c2270");

/* Moby class 340 */
void circular_saw_enemy_update(M_35F0 *moby) {
    enum {
        CIRCULAR_SAW_STATE_INITIALIZE = 0,
        CIRCULAR_SAW_STATE_IDLE = 1,
        CIRCULAR_SAW_STATE_JUMP = 2,
        CIRCULAR_SAW_STATE_CHASE_TARGET = 3,
        CIRCULAR_SAW_STATE_FOLLOW_PATH = 4,
        CIRCULAR_SAW_STATE_POINT_ATTACK = 5,
        CIRCULAR_SAW_STATE_HIT_RECOVERY = 6,
        CIRCULAR_SAW_STATE_MELEE_ATTACK = 7,
        CIRCULAR_SAW_STATE_DYING = 8
    };
    enum {
        CIRCULAR_SAW_HIT_REACTION_NONE = 0,
        CIRCULAR_SAW_HIT_REACTION_FATAL = 1,
        CIRCULAR_SAW_HIT_REACTION_DESTROY_IMMEDIATELY = 2,
        CIRCULAR_SAW_HIT_REACTION_START_HIT_RECOVERY = 3,
        CIRCULAR_SAW_HIT_REACTION_KNOCKBACK_RECOVERY = 4,
        CIRCULAR_SAW_HIT_REACTION_BACKWARD_RECOIL = 5,
        CIRCULAR_SAW_HIT_REACTION_RECOIL_RECOVERY = 6,
        CIRCULAR_SAW_HIT_REACTION_IGNORED = 7,
        CIRCULAR_SAW_HIT_REACTION_NO_ADDITIONAL_REACTION = 8,
        CIRCULAR_SAW_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE = 9,
        CIRCULAR_SAW_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        CIRCULAR_SAW_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE = 11
    };
    enum {
        CIRCULAR_SAW_POINT_ATTACK_APPROACH_AND_ALIGN = 0,
        CIRCULAR_SAW_POINT_ATTACK_ATTACK = 1,
        CIRCULAR_SAW_POINT_ATTACK_TURN_TO_PATH = 2
    };
    D_35F0 *data = moby->data;
    T_35F0  tg;
    V4_35F0 initial_position_snapshot;
    V4_35F0 dest;
    int     flag = 0;
    float   rot = moby->rot;
    int     target_moby;
    float   hd;
    float   target_distance;
    float   ta;

    if (data->f38 != 0 || data->f136 != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8(2.9f, (int)moby, 7, data->f270);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x15;
        }
    }
    select_moby_combat_target((char *)moby, (float *)&tg, flag ? data->f140 * 2.0f : data->f140);
    target_moby = tg.f40;
    hd = compute_interpolated_record_value(moby);
    target_distance = vector_distance_xy(moby->pos, (float *)&tg);
    qcopy(initial_position_snapshot, moby->pos);
    qcopy(dest, moby->pos);
    moby->f30 = 0x20;
    update_circular_saw_engine_sound((struct Moby *)moby);
    if (moby->state == 1 || moby->state == 3) {
        H_35F0 *h = (H_35F0 *)(((char *)((char *)&hero)));
        if (h->f240 == moby || fast_vec_dist(moby->pos, h->pos) < 1.5f) {
            blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
            data->f146 = scale_game_frames(0x3C);
            moby->state = 7;
        }
    }
    FUN_L00_001fefc8(&data->f14D);
    if (moby->state != 8 && moby->state != 0) {
        int       unused_hit_reaction_output;
        float     dmg;
        Hit_35F0 *hit;
        int       r;
        float     ang;

        dmg = 0.0f;
        hit = find_moby_hit(moby, 0x210000, 0);
        r = hit_reaction(moby, hit, &data->hp, 0, &unused_hit_reaction_output, &dmg, 0, 4);
        if (hit != 0 && moby->state != 0 && data->f14D == 0) {
            if (hit->att != 0) {
                struct Hero *h = &hero;
                if (hit->att != h->items[0].moby) {
                    ang = fast_arc_tan(moby->pos[0] - hit->att->pos[0],
                                       moby->pos[1] - hit->att->pos[1]);
                } else {
                    ang = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                       moby->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                ang = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                   moby->pos[1] - h->motion.pos.f[1]);
            }
            data->fD0 = 0.008f;
            data->fD4 = 0.0005f;
            data->fD8 = frame_time * 4.0f;
            data->fE4 = 9;
            data->fFD = 0;
            if ((unsigned int)(hit->att->cls.u - 0xB0) < 2) {
                hit->f2C = 1.0f;
                dmg = 1.0f;
            }
            data->hp -= dmg;
            if (data->hp <= 0.0f) {
                r = 1;
            }
            data->f136 = scale_game_frames(0xF0);
            switch (r) {
            case CIRCULAR_SAW_HIT_REACTION_NONE:
                break;
            case CIRCULAR_SAW_HIT_REACTION_FATAL:
            case CIRCULAR_SAW_HIT_REACTION_DESTROY_IMMEDIATELY:
                spawn_circular_saw_destruction_effects((char *)moby);
                mark_moby_for_removal(moby);
                return;
            case CIRCULAR_SAW_HIT_REACTION_START_HIT_RECOVERY:
            case CIRCULAR_SAW_HIT_REACTION_KNOCKBACK_RECOVERY:
            case CIRCULAR_SAW_HIT_REACTION_BACKWARD_RECOIL:
            case CIRCULAR_SAW_HIT_REACTION_RECOIL_RECOVERY:
                blend_moby_animation(moby, (void *)8, 0, (void *)scale_game_frames(5));
                if (hit->att->cls.s == 0x131 || hit->att->cls.s == 0xB0) {
                    data->f13C = 10.0f / scale_time(15.0f) * 0.5f;
                } else {
                    data->f13C = 10.0f / scale_time(15.0f);
                }
                moby->state = 6;
                moby->rot = fast_add_rotations(3.1415927f, ang);
                data->fB7 = 0x78;
                trooper_anim_update(moby, (short *)data->fB0);
                data->f14D = scale_game_frames(0x14);
                data->fB7 = 0x78;
                trooper_anim_update(moby, (short *)data->fB0);
                data->f14D = scale_game_frames(0x14);
                break;
            case CIRCULAR_SAW_HIT_REACTION_IGNORED:
            case CIRCULAR_SAW_HIT_REACTION_NO_ADDITIONAL_REACTION:
            case CIRCULAR_SAW_HIT_REACTION_CONTINUE_WITHOUT_RESPONSE:
            case CIRCULAR_SAW_HIT_REACTION_NO_FURTHER_RESPONSE:
            case CIRCULAR_SAW_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE:
                break;
            }
        }
    }
    moby->fA4 = 0xFF;
    if (data->f38 != 0) {
        data->f136 = scale_game_frames(0xF0);
    }
    data->f38 = 0;
    fast_dec_timer_s16(&data->f136);
    if (moby->state == 7 && moby->f52 == 7 && hd > 15.0f && hd < 44.0f) {
        int i;
        for (i = 0; i < 4; i++) {
            S_35F0  melee_hit_parameters;
            V4_35F0 jp;
            I4_35F0 joints = D_L04_00161890;
            {
                moby_point_position(moby, joints.v[i], jp);
                melee_hit_parameters.moby = moby;
                melee_hit_parameters.f14 = 0x10001;
                melee_hit_parameters.f1C = 1.0f;
                fast_vec_sub(melee_hit_parameters.v, &hero.motion.pos, moby->pos);
                fast_vec_normalize(
                    melee_hit_parameters.v, melee_hit_parameters.v, frame_time * 10.0f);
                melee_hit_parameters.f20 = 1;
                collect_mobies_in_sphere(jp, 0x10, moby, &melee_hit_parameters, 0.5f);
                if (coll_hit.moby == target_moby) {
                    data->f146 =
                        data->f146 < scale_game_frames(0x3C) ? data->f146 : scale_game_frames(0x3C);
                }
            }
        }
    }
    if (moby->state != 0 && data->f2E != 1) {
        int engine_voice_index = data->f238;
        if (engine_voice_index != -1) {
            char *e = (char *)((char *)&D_0013E550) + engine_voice_index * 0x70;
            if (*(M_35F0 **)(e + 0x88) == moby) {
                if (*(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(engine_voice_index);
                }
            }
        }
        data->f238 = -1;
        mark_moby_for_removal(moby);
        return;
    }
    fast_dec_timer_s16(&data->f146);
    switch (moby->state) {
    case CIRCULAR_SAW_STATE_INITIALIZE: {
        float k;
        data->hp = 1.0f;
        data->f24 = 1;
        data->f28 = 0;
        data->f2E = 1;
        data->f29 = 0;
        data->f30 = 0.75f;
        data->f58 = 8;
        data->f5A = 6;
        FUN_L01_0026d930((char *)&data->f60);
        k = frame_time;
        data->f6C = 0.15f;
        data->f68 = 0.25f;
        data->f60 = 0x200;
        data->f84 = k * 3.0f;
        data->f64 = 1.0f;
        data->f98 = 4;
        qcopy(data->f240, moby->pos);
        data->f25C = 2.0f;
        data->f254 = k * 2.0f;
        data->f258 = k * 2.9670596f;
        qcopy(data->f120, moby->pos);
        data->f146 = scale_game_frames(0x3C);
        data->f1F0 = &D_L04_00161878;
        data->f136 = 0;
        data->f144 = 0;
        if (data->f150 == -1) {
            moby->state = 1;
        } else {
            moby->state = 4;
        }
        break;
    }
    case CIRCULAR_SAW_STATE_IDLE: {
        V4_35F0 save;
        {
            qcopy(save, moby->pos);
            update_object_wander_movement((char *)moby, (char *)data->f240, 0.35f, 0.25f);
            if (data->f138 != -1) {
                if (point_in_path_xy(moby->pos,
                                     ((P_35F0 *)l04_paths[data->f138])->pts,
                                     ((P_35F0 *)l04_paths[data->f138])->count) == 0) {
                    qcopy(moby->pos, save);
                }
            }
            if (moby->f52 != 0 && (moby->f70 & 2)) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(5));
            }
            if (flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) {
                if (flag || data->f138 == -1 ||
                    point_in_path_xy(&tg,
                                     ((P_35F0 *)l04_paths[data->f138])->pts,
                                     ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                    int nf = (data->f14E & 1) == 0;
                    if (nf) {
                        data->f14E |= 1;
                        blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(10));
                        data->f146 = scale_game_frames(0x3C);
                    }
                    ta = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
                    if (target_distance < 2.0f && data->f146 == 0 &&
                        check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                        blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                        data->f146 = scale_game_frames(0x3C);
                        moby->state = 7;
                    } else if (flag ? target_distance < data->f140 * 2.0f
                                    : target_distance < data->f140) {
                        if (data->f146 == 0) {
                            blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
                            data->f14C = 0;
                            moby->state = 3;
                        }
                    }
                }
            }
        }
        break;
    }
    case CIRCULAR_SAW_STATE_JUMP:
        if (moby->f53 != 5) {
            blend_moby_animation(moby, (void *)5, 0, (void *)scale_game_frames(5));
            data->f78 = -(frame_time * 7.668f);
        } else if (moby->f70 & 2) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
            moby->state = 1;
        }
        break;
    case CIRCULAR_SAW_STATE_CHASE_TARGET:
        if ((flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) &&
            (flag || data->f138 == -1 ||
             point_in_path_xy(&tg,
                              ((P_35F0 *)l04_paths[data->f138])->pts,
                              ((P_35F0 *)l04_paths[data->f138])->count) != 0)) {
            rot = compute_circular_saw_flock_steering(
                moby, dest, flag ? data->f140 * 2.0f : data->f140);
            if (target_distance < 2.0f && data->f146 == 0 &&
                check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                data->f146 = scale_game_frames(0x3C);
                moby->state = 7;
            }
        } else if (flag == 0) {
            qcopy(dest, data->f120);
            if (vector_distance_xy(moby->pos, data->f120) < 1.0f) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
                moby->state = 1;
            }
        }
        func_L00_002592B0_b((char *)moby,
                            rot,
                            &data->f130,
                            frame_time_sq * 1.0471976f,
                            frame_time_sq * 3.1415927f,
                            frame_time * 3.1415927f);
        break;
    case CIRCULAR_SAW_STATE_FOLLOW_PATH: {
        P_35F0 *path = ((P_35F0 *)l04_paths[data->f150]);
        int     i;
        float  *yaw_velocity;

        if (flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) {
            if (flag || data->f138 == -1 ||
                point_in_path_xy(&tg,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                int nf = (data->f14E & 1) == 0;
                if (nf) {
                    data->f14E |= 1;
                    blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                }
                ta = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
                if (target_distance < 2.0f && data->f146 == 0 &&
                    check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                    moby->state = 7;
                } else if (flag ? target_distance < data->f140 * 2.0f
                                : target_distance < data->f140) {
                    if (data->f146 == 0) {
                        blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
                        data->f14C = 0;
                        moby->state = 3;
                    }
                }
            }
        }
        if (vector_distance_xy(path->pts[data->f154], moby->pos) < 1.0f ||
            fast_difference_between_rotations(
                moby->rot,
                fast_arc_tan(path->pts[data->f154][0] - moby->pos[0],
                             path->pts[data->f154][1] - moby->pos[1])) > 2.3561945f) {
            data->f154++;
            if (data->f154 == path->count) {
                data->f154 = 0;
            }
        }
        i = 0;
        yaw_velocity = &data->f130;
        for (; i < 4; i++) {
            if (data->f146 == 0 && data->f160[i] != -1 &&
                is_point_inside_clip_volume(moby->pos, data->f160[i]) != 0) {
                data->f158 = data->f160[i];
                moby->state = 5;
                data->f15A = 0;
                break;
            }
        }
        {
            int n = data->f154;
            qcopy(dest, path->pts[n]);
            ta = fast_arc_tan(path->pts[n][0] - moby->pos[0], path->pts[n][1] - moby->pos[1]);
            func_L00_002592B0_b((char *)moby,
                                ta,
                                yaw_velocity,
                                frame_time_sq * 1.0471976f,
                                frame_time_sq * 3.1415927f,
                                frame_time * 3.1415927f);
        }
        break;
    }
    case CIRCULAR_SAW_STATE_POINT_ATTACK: {
        P_35F0 *path = ((P_35F0 *)l04_paths[data->f150]);

        if (flag ? target_distance < data->f140 * 2.0f : target_distance < data->f140) {
            if (flag || data->f138 == -1 ||
                point_in_path_xy(&tg,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                int nf = (data->f14E & 1) == 0;
                if (nf) {
                    data->f14E |= 1;
                    blend_moby_animation(moby, (void *)6, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                }
                ta = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
                if (target_distance < 2.0f && data->f146 == 0 &&
                    check_circular_saw_melee_attack_spacing((char *)moby) != 0) {
                    blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
                    data->f146 = scale_game_frames(0x3C);
                    moby->state = 7;
                    data->f130 = 0.0f;
                } else if (flag ? target_distance < data->f140 * 2.0f
                                : target_distance < data->f140) {
                    if (data->f146 == 0) {
                        blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
                        data->f14C = 0;
                        moby->state = 3;
                    }
                }
            }
        }
        switch (data->f15A) {
        case CIRCULAR_SAW_POINT_ATTACK_APPROACH_AND_ALIGN: {
            LevelCuboid *e = l04_cuboids;
            int          n = data->f158;
            qcopy(dest, &e[n].pos);
            func_L00_002592B0_b((char *)moby,
                                e[n].rot.z,
                                &data->f130,
                                frame_time_sq * 1.0471976f,
                                frame_time_sq * 3.1415927f,
                                frame_time * 3.1415927f);
            if (moby->f53 != 1) {
                blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
            }
            if (vector_distance_xy(moby->pos, dest) < 1.0f) {
                if (fast_difference_between_rotations(moby->rot, l04_cuboids[data->f158].rot.z) <
                    0.17453292f) {
                    data->f15A = 1;
                }
            }
            break;
        }
        case CIRCULAR_SAW_POINT_ATTACK_ATTACK:
            qcopy(dest, &l04_cuboids[data->f158].pos);
            if (moby->f53 != 7) {
                blend_moby_animation(moby, (void *)7, 0, (void *)scale_game_frames(10));
            } else if (moby->f70 & 2) {
                blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
                data->f130 = 0.0f;
                data->f15A = 2;
            }
            break;
        case CIRCULAR_SAW_POINT_ATTACK_TURN_TO_PATH: {
            qcopy(dest, &l04_cuboids[data->f158].pos);
            ta = fast_arc_tan(path->pts[data->f154][0] - moby->pos[0],
                              path->pts[data->f154][1] - moby->pos[1]);
            func_L00_002592B0_b((char *)moby,
                                ta,
                                &data->f130,
                                frame_time_sq * 1.0471976f,
                                frame_time_sq * 3.1415927f,
                                frame_time * 3.1415927f);
            if (fast_difference_between_rotations(
                    moby->rot,
                    fast_arc_tan(path->pts[data->f154][0] - moby->pos[0],
                                 path->pts[data->f154][1] - moby->pos[1])) < 0.17453292f) {
                moby->state = 4;
                data->f146 = scale_game_frames(0xF0);
            }
            break;
        }
        }
        break;
    }
    case CIRCULAR_SAW_STATE_MELEE_ATTACK:
        rot =
            compute_circular_saw_flock_steering(moby, dest, flag ? data->f140 * 2.0f : data->f140);
        if (data->f146 == 0 && (hd > 44.0f || (moby->f70 & 2))) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(5));
            data->f146 = scale_game_frames(0x78);
            moby->state = 1;
            data->f14E |= 2;
        }
        func_L00_002592B0_b((char *)moby,
                            rot,
                            &data->f130,
                            frame_time_sq * 1.0471976f,
                            frame_time_sq * 3.1415927f,
                            frame_time * 3.1415927f);
        break;
    case CIRCULAR_SAW_STATE_HIT_RECOVERY:
        if ((moby->f52 == 8 && compute_interpolated_record_value(moby) > 30.0f) ||
            (moby->f70 & 2)) {
            blend_moby_animation(moby, 0, 0, (void *)scale_game_frames(10));
            moby->state = 1;
            data->f146 = 0;
        }
        break;
    case CIRCULAR_SAW_STATE_DYING:
        if (moby->f70 & 2) {
            spawn_circular_saw_destruction_effects((char *)moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case 9:
        if (FUN_L00_002dc9f0(moby, data->fC0) != 0) {
            moby->state = 0;
            data->f1E8 = 0;
        }
        break;
    }
    if (data->f13C > 0.0f) {
        V4_35F0 v;
        {
            v[0] = fast_cos(fast_add_rotations(moby->rot, 3.1415927f)) * data->f13C;
            v[1] = fast_sin(fast_add_rotations(moby->rot, 3.1415927f)) * data->f13C;
            v[2] = 0.0f;
            fast_vec_add(v, v, moby->pos);
            data->f13C += -10.0f / (scale_time(15.0f) * scale_time(15.0f));
            FUN_L00_00258490(moby, moby->pos, v, 1, 0.25f, 0.57f, 0.5f, 0.5235988f);
        }
    }
    if (moby->state == 9) {
        return;
    }
    if (moby->state == 4) {
        V4_35F0 v;
        V4_35F0 np;
        {
            if (moby->f53 != 1) {
                blend_moby_animation(moby, (void *)1, 0, (void *)scale_game_frames(10));
            }
            v[0] = fast_cos(moby->rot) * (frame_time * 2.0f);
            v[1] = fast_sin(moby->rot) * (frame_time * 2.0f);
            v[2] = 0.0f;
            fast_vec_add(v, v, moby->pos);
            qcopy(np, moby->pos);
            FUN_L00_00258490(moby, np, v, 0, 0.25f, 0.57f, 0.5f, 0.5235988f);
            if (data->f138 == -1 ||
                point_in_path_xy(np,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                qcopy(moby->pos, np);
            }
        }
    } else if (moby->state == 3) {
        V4_35F0 v;
        V4_35F0 np;
        float   ang = fast_arc_tan(dest[0] - moby->pos[0], dest[1] - moby->pos[1]);
        int     anim = moby->f53;
        I4_35F0 movement_animation_by_direction = D_L04_001618A0;
        int     moved;
        {
            if (vector_distance_xy(moby->pos, dest) >= frame_time * 3.0f * 2.0f) {
                float rel = fast_subtract_rotations(ang, moby->rot);
                float biased_relative_yaw = fast_add_rotations(rel, 0.3926991f);
                float movement_relative_yaw;
                if (biased_relative_yaw < 0.0f) {
                    biased_relative_yaw += 6.2831855f;
                }
                if (fast_difference_between_rotations(
                        fast_normalize_angle(fast_normalize_angle(
                            convert_integer_to_float(data->f14C) * 1.5707964f)),
                        rel) > 1.7453293f) {
                    data->f14C = truncate_float_to_s32(biased_relative_yaw * 1.999f / 3.1415927f);
                    anim = movement_animation_by_direction.v[data->f14C];
                }
                movement_relative_yaw =
                    fast_normalize_angle(convert_integer_to_float(data->f14C) * 1.5707964f);
                v[0] = fast_cos(fast_add_rotations(moby->rot, movement_relative_yaw)) *
                       (frame_time * 3.0f);
                v[1] = fast_sin(fast_add_rotations(moby->rot, movement_relative_yaw)) *
                       (frame_time * 3.0f);
                v[2] = 0.0f;
                fast_vec_add(v, v, moby->pos);
            } else {
                qcopy(v, moby->pos);
                anim = 0;
            }
            qcopy(np, moby->pos);
            moved = FUN_L00_00258490(moby, np, v, 0, 0.25f, 0.57f, 0.5f, 0.5235988f);
            if (data->f138 == -1 ||
                point_in_path_xy(np,
                                 ((P_35F0 *)l04_paths[data->f138])->pts,
                                 ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                qcopy(moby->pos, np);
                if (moved == 0) {
                    anim = 0;
                }
            } else {
                anim = 0;
            }
            if (fast_dec_timer_s16(&data->f144) != 0 && anim != moby->f53) {
                blend_moby_animation(moby, (void *)anim, 0, (void *)scale_game_frames(10));
                data->f144 = scale_game_frames(0xF);
            }
        }
    } else if (moby->state == 5 || (moby->state == 7 && moby->f52 == 7 && hd > 15.0f)) {
        if (vector_distance_xy(moby->pos, dest) > frame_time * 2.0f) {
            V4_35F0 v;
            V4_35F0 np;
            {
                v[0] = fast_cos(fast_arc_tan(dest[0] - moby->pos[0], dest[1] - moby->pos[1])) *
                       (frame_time * 2.0f);
                v[1] = fast_sin(fast_arc_tan(dest[0] - moby->pos[0], dest[1] - moby->pos[1])) *
                       (frame_time * 2.0f);
                v[2] = 0.0f;
                fast_vec_add(v, v, moby->pos);
                qcopy(np, moby->pos);
                FUN_L00_00258490(moby, np, v, 0, 0.25f, 1.25f, 0.5f, 0.5235988f);
                if (data->f138 == -1 ||
                    point_in_path_xy(np,
                                     ((P_35F0 *)l04_paths[data->f138])->pts,
                                     ((P_35F0 *)l04_paths[data->f138])->count) != 0) {
                    qcopy(moby->pos, np);
                }
            }
        }
    } else if (moby->state != 6 && moby->state != 8) {
        V4_35F0 v;
        {
            qcopy(v, moby->pos);
            FUN_L00_00258490(moby, moby->pos, v, 0, 0.25f, 0.57f, 0.5f, 0.5235988f);
        }
    }
    data->f78 += frame_time_sq * 9.8f;
    moby->pos[2] -= data->f78 - 2.0f;
    {
        float g = probe_ground_height(moby->pos, 0, 0.5f);
        moby->pos[2] -= 2.0f;
        if (moby->pos[2] < g) {
            moby->pos[2] = g;
            data->f78 = 0.0f;
        }
    }
    if (moby->pos[2] < data->f120[2] - 5.0f) {
        spawn_circular_saw_destruction_effects((char *)moby);
        mark_moby_for_removal(moby);
        return;
    }
    if (vector_distance_xy(moby->pos, D_L04_00166F40) < 20.0f) {
        V4_35F0 b;
        V4_35F0 joint_position_scratch;
        int     alt;
        {
            *(OvlQuad *)joint_position_scratch = 0;
            joint_position_scratch[0] = random_float_between(1.0f, -1.0f) * (frame_time * 0.1f);
            joint_position_scratch[1] = random_float_between(1.0f, -1.0f) * (frame_time * 0.1f);
            joint_position_scratch[2] = random_float_between(0.8f, 1.2f) * (frame_time * -2.5f);
            *(OvlQuad *)b = *(OvlQuad *)joint_position_scratch;
            alt = random_integer_below(100) & 1;
            moby_point_position(moby, 0, joint_position_scratch);
            transform_vector_by_basis(b, b, moby->fC0);
            spawn_effect_particle_with_target_scalar(
                joint_position_scratch,
                b,
                alt ? 0x400F0F7F : 0x407F7F7F,
                0x272727,
                random_float_between(75000.0f, 100000.0f),
                scale_game_frames(alt ? random_int_between(10, 15) : random_int_between(20, 30)));
        }
    }
    trooper_anim_finish(moby, data->fB0);
}

extern int launch_suck_cannon_projectile(char *, int, int, float) __asm__("FUN_L00_002dbc30");

/* Suck cannon method: keeps the saw in state 9 while it flies as a suck cannon projectile, back to state 1 after; plays a voice when the record's short at 0x68 is 6. */
int launch_circular_saw_enemy_from_suck_cannon(struct Moby *moby,
                                               int          input_quad_address,
                                               int          b,
                                               float        f) __asm__("FUN_L04_002c40d0");

int launch_circular_saw_enemy_from_suck_cannon(struct Moby *moby,
                                               int          input_quad_address,
                                               int          launch_position_source,

                                               float launch_position_offset) {
    char *moby_callback_result = get_moby_suck_cannon_record(moby);
    int   hit_processing_result = launch_suck_cannon_projectile(
        moby, input_quad_address, launch_position_source, launch_position_offset);
    if (hit_processing_result != 0) {
        moby->state = 0x9;
    } else if ((unsigned char)moby->state == 0x9) {
        moby->state = 1;
    }
    if (*(short *)(moby_callback_result + 0x68) == 6) {
        allocate_voice_for_target_entry(1, 0, moby);
    }
    return hit_processing_result;
}

typedef struct {
    char pad[0x78];
    s32  f;
} T6;

/* Suck cannon method: the saw's suck cannon record, its pvars plus 0x180 (0 without pvars). */
s32 get_circular_saw_enemy_suck_cannon_record(T6 *data_owner) __asm__("FUN_L04_002c41b0");

s32 get_circular_saw_enemy_suck_cannon_record(T6 *data_owner) {
    s32 data_base;
    if (data_owner != 0) {
        data_base = data_owner->f;
        if (data_base != 0) {
            return data_base + 0x180;
        }
        return 0;
    }
    return 0;
}
