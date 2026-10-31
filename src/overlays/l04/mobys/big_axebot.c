/* Level 04 moby class big_axebot: the large axe-swinging robot, and the spawn of its axe. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/normalize_vector_triplet.h"
#include "rnc/overlay/shared/enemies/trooper_set_knockback.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB670), where it is exact; names translated to the US level program. */

void blend_big_axebot_and_axe_animation(char *moby,
                                        void *new_seq,
                                        void *frame_index,
                                        void *blend_frames) __asm__("FUN_L04_002ba3e0");

void blend_big_axebot_and_axe_animation(char *moby,
                                        void *new_seq,
                                        void *frame_index,

                                        void *blend_frames) {
    char *axebot_pvars = ((char *)MOBY(moby)->pvars);
    void *linked_axe_moby;
    blend_moby_animation(moby, new_seq, frame_index, blend_frames);
    linked_axe_moby = *(void **)(axebot_pvars + 0x424);
    if (linked_axe_moby != 0) {
        blend_moby_animation(linked_axe_moby, new_seq, frame_index, blend_frames);
    }
}
/* Sets the float at 0x58 on the moby and on the linked moby at data+0x424, if any. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB6D8), where it is exact; names translated to the US level program. */

void set_big_axebot_and_axe_animation_speed(struct Moby *moby,
                                            float animation_speed) __asm__("FUN_L04_002ba448");

void set_big_axebot_and_axe_animation_speed(struct Moby *moby, float animation_speed) {
    char *big_axebot_pvars = ((char *)moby->pvars);
    moby->anim_speed = animation_speed;
    if (*(char **)(big_axebot_pvars + 0x424) != 0) {
        *(float *)(*(char **)(big_axebot_pvars + 0x424) + 0x58) = animation_speed;
    }
}
/* Spawns an effect on the moby at its marker vector. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB700), where it is exact; names translated to the US level program. */

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

void FUN_L04_002ba470(char *moby) {
    float explosion_position[4];
    float unused_explosion_argument[4];
    clear_u64_value(unused_explosion_argument);
    moby_point_position(moby, 6, explosion_position);
    spawn_explosion(moby,
                    unused_explosion_argument,
                    explosion_position,
                    0.0f,
                    0.0f,
                    10,
                    3,
                    4,
                    2.0f,
                    1.0f,
                    100000.0f,
                    -1,
                    1.5f,
                    15.0f,
                    1,
                    1,
                    -1,
                    0);
}

extern void select_moby_combat_target(char *, float *, float) __asm__("FUN_L00_0025fcb8");

/* Update of the big axebot: hit reaction, state machine (idle, alert, path, chase, return home, swing, stagger, turn, death), knockback, gravity, and its axe moby. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_0029FCF0.c: func_L04_002BB7B0), where it is exact; names translated to the US level program. */

typedef float V4_B7B0[4] __attribute__((aligned(16)));

typedef struct Manip_B7B0 {
    unsigned char id;
    unsigned char on;
    char          pad02[0xE];
    V4_B7B0       q;
    char          pad20[0x20];
} Manip_B7B0;

typedef struct D_B7B0 {
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
    char           pad5B[0x55];
    char           fB0[7];
    unsigned char  fB7;
    char           padB8[8];
    float          fC0[4];
    float          fD0;
    float          fD4;
    float          fD8;
    float          fDC;
    int            fE0;
    int            fE4;
    float          fE8;
    char           padEC[0x11];
    unsigned char  fFD;
    char           padFE[0x12];
    float          f110;
    float          f114;
    char           pad118[8];
    Manip_B7B0     m120;
    Manip_B7B0     m160;
    char           f1A0[0x250];
    V4_B7B0        home;
    unsigned char  f400;
    unsigned char  f401;
    short          f402;
    short          f404;
    unsigned char  f406;
    unsigned char  f407;
    float          f408;
    char           pad40C[4];
    float          f410;
    float          f414;
    float          f418;
    int            f41C;
    float          f420;
    struct M_B7B0 *f424;
    float          f428;
    float          f42C;
    int            f430;
    int            f434;
    short          f438;
    short          f43A;
    float          f43C;
    char           pad440[8];
    int            f448;
} D_B7B0;

typedef struct M_B7B0 {
    char           pad00[0x10];
    V4_B7B0        pos;
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  f31;
    char           pad32[2];
    unsigned short f34;
    char           pad36[0xA];
    V4_B7B0        rot;
    char           pad50[2];
    unsigned char  f52;
    unsigned char  f53;
    char           pad54[4];
    float          f58;
    char           pad5C[0x14];
    unsigned char  f70;
    char           pad71[7];
    D_B7B0        *data;
    char           pad7C[3];
    char           f7F;
    char           pad80[0x24];
    unsigned char  fA4;
    char           padA5;
    short          cls;
    char           padA8[0x14];
    unsigned char  fBC;
    char           padBD[3];
    V4_B7B0        fC0;
} M_B7B0;

typedef struct {
    V4_B7B0 pos;
    char    pad10[0x30];
    char   *f40;
    char    pad44[0xC];
} T_B7B0;

typedef struct {
    char    pad00[0x10];
    V4_B7B0 pos;
    M_B7B0 *att;
    char    pad24[8];
    float   f2C;
} Hit_B7B0;

typedef struct {
    int     count;
    int     pad[3];
    V4_B7B0 pts[1];
} P_B7B0;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float D_L04_0015F580[] MACRO_ADDR;
extern float D_L04_00166F40[4];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float spring_turn(float cur, float target, float *vel, float k, float d, float max) __asm__("FUN_L00_00258110");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   D_L04_0015F5C4;
extern int   FUN_L00_001fefc8(void *);
extern int   is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L04_00295de8_cf(void *, void *, void *, int, float, float, float, float) __asm__("FUN_L00_00258490");
extern int step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");
extern int allocate_voice_for_target_entry(int, int, char *);
extern int mark_moby_for_removal_cf() __asm__("FUN_L04_002ba470");
extern void       *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
void *spawn_big_axebot_axe(char *source_moby, char *spawn_position) __asm__("FUN_L04_002d34d8");
extern void *random_integer_below_cf(void *, int, unsigned int) __asm__("FUN_001153fc");
extern void  FUN_L00_001fff28(void *, int, float);
extern void  FUN_L00_00257470(void *, int, int);
extern void  leg_walker_update(void *, void *, float, float, float) __asm__("FUN_L04_00295778");
extern void  FUN_L00_0025a120(void *);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void reset_leg_rig_from_animation(void *, void *) __asm__("FUN_L04_00295de8");
extern void advance_moby_animation(void *);
extern void build_moby_bone_transform(char *, int, char *);
extern void spawn_nearby_effect(char *m, float t, int a, int b, void *pos, void *vel) __asm__("FUN_L00_00259bc8");
extern void copy_matrix3x4(void *, void *);
extern void refresh_moby_spatial_bounds_from_basis(void *);
void        blend_big_axebot_and_axe_animation(char *arg,
                                               void *a,
                                               void *b,
                                               void *c) __asm__("FUN_L04_002ba3e0");

void         big_axebot_update(M_B7B0 *moby) __asm__("FUN_L04_002ba520");
extern float fast_cos(float);
extern float fast_sin(float);

/* Moby class 217 */
void big_axebot_update(M_B7B0 *moby) {
    enum {
        BIG_AXEBOT_STATE_INITIALIZE = 0,
        BIG_AXEBOT_STATE_IDLE = 1,
        BIG_AXEBOT_STATE_ALERT = 2,
        BIG_AXEBOT_STATE_WAIT_FOR_TARGET = 3,
        BIG_AXEBOT_STATE_FOLLOW_PATH = 4,
        BIG_AXEBOT_STATE_CHASE_TARGET = 5,
        BIG_AXEBOT_STATE_RETURN_HOME = 6,
        BIG_AXEBOT_STATE_MELEE_ATTACK = 7,
        BIG_AXEBOT_STATE_HIT_RECOVERY = 8,
        BIG_AXEBOT_STATE_TURN = 9,
        BIG_AXEBOT_STATE_DYING = 10
    };
    enum {
        BIG_AXEBOT_HIT_REACTION_NONE = 0,
        BIG_AXEBOT_HIT_REACTION_FATAL = 1,
        BIG_AXEBOT_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE = 2,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_HIT_RECOVERY = 3,
        BIG_AXEBOT_HIT_REACTION_RETURN_TO_IDLE = 4,
        BIG_AXEBOT_HIT_REACTION_IDLE_RECOVERY = 5,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOIL_RECOVERY = 6,
        BIG_AXEBOT_HIT_REACTION_KNOCKBACK = 7,
        BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOVERY = 8,
        BIG_AXEBOT_HIT_REACTION_NO_ADDITIONAL_RESPONSE = 9,
        BIG_AXEBOT_HIT_REACTION_NO_FURTHER_RESPONSE = 10,
        BIG_AXEBOT_HIT_REACTION_IGNORED = 11
    };
    D_B7B0 *data = moby->data;
    T_B7B0  tg;
    V4_B7B0 initial_position_snapshot;
    V4_B7B0 dest;
    V4_B7B0 tpos;
    V4_B7B0 vector_quaternion_scratch;
    int     flag = 0;
    float   rot;
    char   *target_moby;
    float   target_distance;
    float   target_yaw;
    float   hd;

    if (data->f38 != 0 || data->f406 != 0) {
        flag = 1;
    }
    FUN_L00_00263ac8(2.5f, (int)moby, 0, (unsigned char *)&data->m120);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 27.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x15;
        }
    }
    select_moby_combat_target((char *)moby, (float *)&tg, flag ? data->f43C * 2.0f : data->f43C);
    target_moby = tg.f40;
    rot = moby->rot[2];
    target_distance = vector_distance_xy(moby->pos, (float *)&tg);
    target_yaw = fast_arc_tan(tg.pos[0] - moby->pos[0], tg.pos[1] - moby->pos[1]);
    hd = compute_interpolated_record_value(moby);
    if (D_L04_0015F5C4 == 2) {
        moby->f34 |= 0x41;
        if (data->f424 != 0) {
            data->f424->f34 |= 0x41;
        }
    } else if (moby->f34 & 1) {
        moby->f34 &= ~0x41;
        if (data->f424 != 0) {
            data->f424->f34 &= ~0x41;
        }
    }
    qcopy(initial_position_snapshot, moby->pos);
    qcopy(dest, moby->pos);
    qcopy(tpos, tg.pos);
    if (moby->f31 != 0) {
        if (fast_vec_dist(moby->pos, D_L04_00166F40) < 32.0f) {
            FUN_L00_0025a120(moby);
            moby->f7F = 0x1A;
        }
    }
    fast_dec_timer_s16(&data->f404);
    if (moby->state != 10 && moby->state != 0) {
        int       unused_hit_reaction_output;
        float     dmg;
        Hit_B7B0 *hit;
        int       hit_reaction_result;
        float     knockback_yaw;

        dmg = 0.0f;
        hit = (Hit_B7B0 *)find_moby_hit(moby, 0x210000, 0);
        hit_reaction_result =
            hit_reaction(moby, hit, &data->hp, 0, &unused_hit_reaction_output, &dmg, 0, 4);
        if (hit != 0 && dmg != 0.0f && moby->state != 0) {
            if (hit->att != 0) {
                struct Hero *h = &hero;
                if (hit->att != h->items[0].moby) {
                    knockback_yaw = fast_arc_tan(moby->pos[0] - hit->att->pos[0],
                                                 moby->pos[1] - hit->att->pos[1]);
                } else {
                    knockback_yaw = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                 moby->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                knockback_yaw = fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                             moby->pos[1] - h->motion.pos.f[1]);
            }
            data->fD4 = frame_time_sq * 12.0f;
            data->fD0 = frame_time_sq * 35.0f;
            data->fD8 = frame_time * 7.0f;
            data->fE4 = 9;
            data->fFD = 0;
            data->hp -= dmg;
            if (data->hp <= 0.0f) {
                hit_reaction_result = 1;
            }
            if (data->f404 != 0 && hit_reaction_result != 1) {
                if (data->f448 <= 0) {
                    data->f408 = -0.5235988f;
                }
                data->f448 = scale_game_frames(0x1E);
                data->fB7 = 0x78;
                trooper_anim_update(moby, (short *)data->fB0);
            } else {
                switch (hit_reaction_result) {
                case BIG_AXEBOT_HIT_REACTION_NONE:
                    break;
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_HIT_RECOVERY:
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOIL_RECOVERY:
                case BIG_AXEBOT_HIT_REACTION_CONDITIONAL_RECOVERY:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    if (data->f448 <= 0) {
                        data->f408 = -0.5235988f;
                    }
                    data->f448 = scale_game_frames(0x1E);
                    if ((hit->att->cls == 0 || hit->f2C > 1.0f) &&
                        (moby->f52 != 4 || hd < 17.0f || hd > 20.0f)) {
                        moby->fBC |= 5;
                        blend_big_axebot_and_axe_animation(
                            (char *)moby, (void *)5, 0, (void *)scale_game_frames(6));
                        set_big_axebot_and_axe_animation_speed(moby, 2.0f);
                        moby->state = 8;
                    }
                    data->f42C = 3.4f / scale_time(7.0f) * 0.5f;
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    data->f404 = scale_game_frames(0x46);
                    data->f406 = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_KNOCKBACK:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    if (data->f448 <= 0) {
                        data->f408 = -0.5235988f;
                    }
                    data->f448 = scale_game_frames(0x1E);
                    moby->fBC |= 5;
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)5, 0, (void *)scale_game_frames(6));
                    set_big_axebot_and_axe_animation_speed(moby, 2.0f);
                    moby->state = 8;
                    trooper_start_knockback(knockback_yaw, moby, data->fC0, 5, 1, 0);
                    data->f110 = 5.0f;
                    data->f114 = 10.0f;
                    data->f42C = 3.4f / scale_time(7.0f);
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    data->f404 = scale_game_frames(0x46);
                    data->f406 = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_RETURN_TO_IDLE:
                case BIG_AXEBOT_HIT_REACTION_IDLE_RECOVERY:
                    allocate_voice_for_target_entry(0xB, 0, (char *)moby);
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)6, (void *)2, (void *)scale_game_frames(4));
                    data->f402 = scale_game_frames(0x28);
                    moby->state = 1;
                    data->f42C = 3.4f / scale_time(7.0f);
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    data->f404 = scale_game_frames(0x46);
                    data->f406 = scale_game_frames(0xF0);
                    break;
                case BIG_AXEBOT_HIT_REACTION_FATAL:
                    data->fE8 = 1.0f;
                    data->fE0 = 0x400;
                    trooper_set_knockback(data->fC0, 5.7f, 4.0f);
                    *(OvlQuad *)vector_quaternion_scratch = *(OvlQuad *)hit->pos;
                    trooper_knockback_arc(
                        vector_quaternion_scratch, &knockback_yaw, &data->fD8, &data->fDC);
                    trooper_start_knockback(knockback_yaw, moby, data->fC0, 7, 1, 0);
                    data->f110 = 12.0f;
                    data->f114 = 25.0f;
                    moby->state = 10;
                    data->fB7 = 0x78;
                    trooper_anim_update(moby, (short *)data->fB0);
                    FUN_L00_00257470(moby, 0, -1);
                    break;
                case BIG_AXEBOT_HIT_REACTION_SKIP_ADDITIONAL_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_NO_ADDITIONAL_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_NO_FURTHER_RESPONSE:
                case BIG_AXEBOT_HIT_REACTION_IGNORED:
                    break;
                }
            }
        }
    }
    moby->fA4 = 0xFF;
    if (data->f38 != 0) {
        data->f406 = scale_game_frames(0xF0);
    }
    data->f38 = 0;
    FUN_L00_001fefc8(&data->f406);
    if (moby->state != 0 && data->f2E != 1) {
        if (data->f424 != 0) {
            mark_moby_for_removal(data->f424);
        }
        mark_moby_for_removal(moby);
        return;
    }
    fast_dec_timer_s16(&data->f402);
    switch (moby->state) {
    case BIG_AXEBOT_STATE_INITIALIZE:
        data->hp = 3.0f;
        data->f24 = 3;
        data->f28 = 2;
        data->f2E = 1;
        data->f29 = 0;
        data->f30 = 1.5f;
        moby->fBC = 0;
        data->f420 = 0.0f;
        data->f424 = 0;
        data->f407 = 0;
        qcopy(data->home, moby->pos);
        if (data->m120.on == 0) {
            attach_manipulator((char *)moby, 0, &data->m120);
        }
        attach_manipulator((char *)moby, 0xC, &data->m160);
        data->f428 = moby->rot[2];
        data->f58 = 0xA;
        data->f5A = 0xC;
        if (data->f400 != 0) {
            moby->state = 3;
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)0xA, 0, (void *)scale_game_frames(0xC));
        } else {
            moby->state = 1;
        }
        break;
    case BIG_AXEBOT_STATE_IDLE: {
        if (moby->f53 == 6 && data->f402 == scale_game_frames(0x25)) {
            allocate_voice_for_target_entry(2, 0, (char *)moby);
        }
        if ((moby->f70 & 2) &&
            (flag ? data->f43C * 2.0f < target_distance : data->f43C < target_distance)) {
            int arr[6];

            random_integer_below_cf(arr, 0, 0x18);
            arr[5] = 1;
            blend_big_axebot_and_axe_animation((char *)moby,
                                               (void *)arr[random_integer_below(6)],
                                               0,
                                               (void *)scale_game_frames(0x18));
        } else if (moby->f53 != 0xD && (moby->f53 != 6 || (moby->f70 & 2))) {
            if (flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C) {
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)0xD, 0, (void *)scale_game_frames(0x18));
            }
        }
        if (data->f430 != -1 && data->f434 != -1 && data->f43A == 0 &&
            (flag || is_point_inside_clip_volume(&hero.motion.pos, data->f430) != 0)) {
            moby->state = 4;
            moby->fBC |= 1;
            data->f43A = 1;
        } else {
            if (moby->f52 == 0xD && FUN_L00_001fefc8(&data->f407) != 0) {
                allocate_voice_for_target_entry(0xA, 0, (char *)moby);
                data->f407 = scale_game_frames(0xB4);
            }
            if (data->f402 == 0) {
                if (target_distance < 4.5f) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
                    moby->state = 7;
                } else if (flag ? target_distance < data->f43C * 2.0f
                                : target_distance < data->f43C) {
                    int nf = (moby->fBC & 1) == 0;
                    if (nf) {
                        blend_big_axebot_and_axe_animation(
                            (char *)moby, (void *)2, 0, (void *)scale_game_frames(0xC));
                        moby->state = 2;
                        moby->fBC |= 1;
                    }
                }
            }
        }
        break;
    }
    case BIG_AXEBOT_STATE_ALERT:
        if (moby->f52 == 2 && (moby->f70 & 2)) {
            blend_big_axebot_and_axe_animation(
                (char *)moby,
                (void *)((flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C)
                             ? 0xD
                             : 0),
                0,
                (void *)scale_game_frames(0xC));
            moby->state = 1;
        }
        break;
    case BIG_AXEBOT_STATE_WAIT_FOR_TARGET:
        if (moby->f52 != 0 && (moby->f70 & 2)) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)0xB, 0, (void *)scale_game_frames(0xC));
        }
        if (data->f402 == 0) {
            if (target_distance < 4.5f) {
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
                moby->state = 7;
            } else if (flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C) {
                int nf = (moby->fBC & 1) == 0;
                if (nf) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)2, 0, (void *)scale_game_frames(0xC));
                    moby->state = 2;
                    moby->fBC |= 1;
                }
            }
        }
        break;
    case BIG_AXEBOT_STATE_FOLLOW_PATH: {
        P_B7B0 *path = ((P_B7B0 *)l04_paths[data->f434]);
        float   a;

        if (moby->f53 != 3) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)3, 0, (void *)scale_game_frames(0xC));
        }
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = 7;
        }
        if (vector_distance_xy(path->pts[data->f438], moby->pos) < 1.0f) {
            data->f438++;
            if (data->f438 == path->count) {
                if (flag ? data->f43C * 2.0f < target_distance : data->f43C < target_distance) {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, 0, 0, (void *)scale_game_frames(0x18));
                } else {
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)0xD, 0, (void *)scale_game_frames(0x18));
                }
                moby->state = 1;
                break;
            }
        }
        a = fast_arc_tan(path->pts[data->f438][0] - moby->pos[0],
                         path->pts[data->f438][1] - moby->pos[1]);
        moby->rot[2] = spring_turn(moby->rot[2],
                                   a,
                                   &data->f410,
                                   frame_time_sq * 12.566371f,
                                   frame_time_sq * 6.2831855f,
                                   frame_time * 3.1415927f);
        vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * (frame_time * 6.0f);
        vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * (frame_time * 6.0f);
        vector_quaternion_scratch[2] = 0.0f;
        fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
        FUN_L04_00295de8_cf(
            moby, moby->pos, vector_quaternion_scratch, 1, 0.25f, 1.5f, 0.75f, 0.87266463f);
        break;
    }
    case BIG_AXEBOT_STATE_CHASE_TARGET:
        if (moby->fBC & 8) {
            moby->fBC &= ~8;
            reset_leg_rig_from_animation(moby, data->f1A0);
        }
        data->f414 = spring_turn(moby->rot[2],
                                 target_yaw,
                                 &data->f410,
                                 frame_time_sq * 6.2831855f,
                                 frame_time_sq * 125.663704f,
                                 frame_time * 12.566371f);
        leg_walker_update(moby, data->f1A0, frame_time * 4.0f, data->f414, frame_time_sq * 20.0f);
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = 7;
        } else if (data->f41C != -1) {
            vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * 2.0f;
            vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * 2.0f;
            vector_quaternion_scratch[2] = 0.0f;
            fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
            if (point_in_path_xy(vector_quaternion_scratch,
                                 ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                 ((P_B7B0 *)l04_paths[data->f41C])->count) == 0) {
                data->f410 = 0.0f;
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)8, 0, (void *)scale_game_frames(0xC));
                moby->state = 9;
            }
            if (point_in_path_xy(tpos,
                                 ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                 ((P_B7B0 *)l04_paths[data->f41C])->count) == 0) {
                moby->state = 6;
            }
        }
        break;
    case BIG_AXEBOT_STATE_RETURN_HOME:
        if (target_distance < 4.5f) {
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(0xC));
            moby->state = 7;
        } else {
            float a;

            if (moby->fBC & 8) {
                moby->fBC &= ~8;
                reset_leg_rig_from_animation(moby, data->f1A0);
            }
            a = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);
            data->f414 = spring_turn(moby->rot[2],
                                     a,
                                     &data->f410,
                                     frame_time_sq * 6.2831855f,
                                     frame_time_sq * 125.663704f,
                                     frame_time * 12.566371f);
            leg_walker_update(
                moby, data->f1A0, frame_time * 4.0f, data->f414, frame_time_sq * 20.0f);
            if (vector_distance_xy(moby->pos, data->home) < 2.0f) {
                moby->state = 1;
            }
            if (data->f41C != -1) {
                vector_quaternion_scratch[0] = fast_cos(moby->rot[2]) * 2.0f;
                vector_quaternion_scratch[1] = fast_sin(moby->rot[2]) * 2.0f;
                vector_quaternion_scratch[2] = 0.0f;
                fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
                if (point_in_path_xy(vector_quaternion_scratch,
                                     ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                     ((P_B7B0 *)l04_paths[data->f41C])->count) == 0) {
                    data->f410 = 0.0f;
                    blend_big_axebot_and_axe_animation(
                        (char *)moby, (void *)8, 0, (void *)scale_game_frames(0xC));
                    moby->state = 9;
                    moby->fBC |= 0x10;
                }
                if (point_in_path_xy(tpos,
                                     ((P_B7B0 *)l04_paths[data->f41C])->pts,
                                     ((P_B7B0 *)l04_paths[data->f41C])->count) != 0) {
                    moby->state = 1;
                }
            }
        }
        break;
    case BIG_AXEBOT_STATE_MELEE_ATTACK:
        if (moby->f52 == moby->f53 && compute_interpolated_record_value(moby) >= 17.0f &&
            compute_interpolated_record_value(moby) < 20.0f &&
            absolute_float(moby->pos[2] - tg.pos[2]) < 2.0f &&
            vector_distance_xy(moby->pos, (float *)&tg) < 4.5f) {
            float p[4];

            vector_quaternion_scratch[0] = fast_cos(target_yaw) * 0.2f;
            vector_quaternion_scratch[1] = fast_sin(target_yaw) * 0.2f;
            vector_quaternion_scratch[2] = 0.0f;
            qcopy(p, tg.pos);
            p[2] += 0.75f;
            if (target_moby != 0) {
                spawn_nearby_effect(target_moby, 1.0f, (int)moby, 1, p, vector_quaternion_scratch);
            }
        }
        if (hd > 20.0f) {
            if (moby->f58 > 1.0f) {
                float animation_speed = moby->f58 - frame_time * 3.0f;

                if (animation_speed < 1.0f) {
                    animation_speed = 1.0f;
                }
                set_big_axebot_and_axe_animation_speed(moby, animation_speed);
            }
        }
        if (moby->f52 == 4 && (moby->f70 & 2)) {
            data->f402 = scale_game_frames(0x5A);
            blend_big_axebot_and_axe_animation(
                (char *)moby,
                (void *)((flag ? target_distance < data->f43C * 2.0f : target_distance < data->f43C)
                             ? 0xD
                             : 0),
                0,
                (void *)scale_game_frames(0xC));
            moby->state = 1;
        }
        break;
    case BIG_AXEBOT_STATE_HIT_RECOVERY:
        if (!(moby->f53 == 5 && (moby->f52 != moby->f53 || hd <= 13.0f))) {
            if (moby->fBC & 4) {
                moby->fBC &= ~4;
                blend_big_axebot_and_axe_animation(
                    (char *)moby, (void *)4, (void *)4, (void *)scale_game_frames(6));
                set_big_axebot_and_axe_animation_speed(moby, 1.5f);
                moby->state = 7;
            } else if (moby->f53 == 8 && (hd < 28.0f || moby->f52 != moby->f53)) {
                float hit_recovery_displacement_limit = frame_scale * 0.11111111f;

                if (data->f418 < hit_recovery_displacement_limit) {
                    data->f418 += frame_time_sq * 20.0f;
                } else {
                    data->f418 = hit_recovery_displacement_limit;
                }
                build_spherical_offset(
                    vector_quaternion_scratch, data->f418, data->f414, moby->rot[1]);
                fast_vec_add(moby->pos, moby->pos, vector_quaternion_scratch);
                moby->rot[2] = spring_turn(moby->rot[2],
                                           target_yaw,
                                           &data->f410,
                                           frame_time_sq * 3.1415927f,
                                           frame_time_sq * 6.2831855f,
                                           frame_time * 4.1887903f);
            } else if (data->f418 > 0.0f) {
                data->f418 -= frame_time_sq * 200.0f;
                build_spherical_offset(
                    vector_quaternion_scratch, data->f418, data->f414, moby->rot[1]);
                fast_vec_add(moby->pos, moby->pos, vector_quaternion_scratch);
                moby->rot[2] = spring_turn(moby->rot[2],
                                           target_yaw,
                                           &data->f410,
                                           frame_time_sq * 3.1415927f,
                                           frame_time_sq * 6.2831855f,
                                           frame_time * 4.1887903f);
            } else {
                moby->state = 1;
                data->f402 = 0;
            }
        }
        break;
    case BIG_AXEBOT_STATE_TURN:
        if (moby->f52 == 8 && (moby->f70 & 2)) {
            moby->state = 5;
            moby->fBC = (moby->fBC | 8) & ~0x10;
            blend_big_axebot_and_axe_animation(
                (char *)moby, (void *)9, 0, (void *)scale_game_frames(0xC));
        } else if (moby->fBC & 0x10) {
            float a = fast_arc_tan(data->home[0] - moby->pos[0], data->home[1] - moby->pos[1]);

            moby->rot[2] = spring_turn(moby->rot[2],
                                       a,
                                       &data->f410,
                                       frame_time_sq * 3.1415927f,
                                       frame_time_sq * 6.2831855f,
                                       frame_time * 4.1887903f);
        } else {
            moby->rot[2] = spring_turn(moby->rot[2],
                                       target_yaw,
                                       &data->f410,
                                       frame_time_sq * 3.1415927f,
                                       frame_time_sq * 6.2831855f,
                                       frame_time * 4.1887903f);
        }
        break;
    case BIG_AXEBOT_STATE_DYING:
        if ((step_moby_motion(moby, data->fC0) & 0x40) || moby->pos[2] < 1.0f) {
            mark_moby_for_removal_cf((char *)moby);
            if (data->f424 != 0) {
                mark_moby_for_removal(data->f424);
            }
            FUN_L00_00257470(moby, 0, -1);
            FUN_L00_00263fd8((char *)moby,
                             0x798,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x799,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x79A,
                             moby->pos,
                             moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L04_0015F580,
                             D_L04_0015F580,
                             D_L04_0015F580);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, data->fB0);
    if (data->f42C > 0.0f) {
        struct Hero *h = &hero;

        vector_quaternion_scratch[0] = fast_cos(fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                             moby->pos[1] - h->motion.pos.f[1])) *
                                       data->f42C;
        vector_quaternion_scratch[1] = fast_sin(fast_arc_tan(moby->pos[0] - h->motion.pos.f[0],
                                                             moby->pos[1] - h->motion.pos.f[1])) *
                                       data->f42C;
        vector_quaternion_scratch[2] = 0.0f;
        fast_vec_add(vector_quaternion_scratch, vector_quaternion_scratch, moby->pos);
        if (data->f420 == 0.0f) {
            data->f42C += -3.4f / (scale_time(7.0f) * scale_time(7.0f));
        }
        FUN_L04_00295de8_cf(
            moby, moby->pos, vector_quaternion_scratch, 1, 0.25f, 1.5f, 0.75f, 0.87266463f);
    }
    data->f408 *= 0.96f;
    fast_dec_timer(&data->f448);
    FUN_L00_001fff28(vector_quaternion_scratch, 1, data->f408);
    FUN_L00_001fff28(data->m120.q, 2, 0.0f);
    multiply_quaternions(data->m120.q, data->m120.q, vector_quaternion_scratch);
    if (moby->state != 8) {
        data->f401 = moby->state;
    }
    data->f420 += frame_time_sq * 29.8f;
    moby->pos[2] -= data->f420 - 2.0f;
    {
        float ground_height = probe_ground_height(moby->pos, 0, 0.5f);

        moby->pos[2] -= 2.0f;
        if (moby->pos[2] < ground_height) {
            moby->pos[2] = ground_height;
            data->f420 = 0.0f;
        }
    }
    if (moby->pos[2] < data->home[2] - 8.0f) {
        mark_moby_for_removal_cf((char *)moby);
        if (data->f424 != 0) {
            mark_moby_for_removal(data->f424);
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (moby->state != 0) {
        float q[4];
        float bone_transform[4][4];
        float bone_euler_angles[4];
        {
            data->f428 = fast_add_rotations(data->f428, fast_subtract_rotations(moby->rot[2], rot));
            build_moby_bone_transform((char *)moby, 0xC, (char *)bone_transform);
            matrix_to_euler(bone_transform, bone_euler_angles);
            FUN_L00_001fff28(q, 2, fast_subtract_rotations(target_yaw, data->f428));
            data->f428 = target_yaw;
            multiply_quaternions(data->m160.q, data->m160.q, q);
        }
    }
    if (data->f424 != 0) {
        float axe_bone_transform[4][4];
        {
            M_B7B0 *axe;

            build_moby_bone_transform((char *)moby, 0xD, (char *)axe_bone_transform);
            axe = data->f424;
            qcopy(axe->pos, axe_bone_transform[3]);
            advance_moby_animation(axe);
            update_entity(data->f424);
            copy_matrix3x4(data->f424->fC0, axe_bone_transform);
            normalize_vector_triplet(data->f424->fC0);
            refresh_moby_spatial_bounds_from_basis(data->f424);
            data->f424->f34 |= 6;
        }
    } else {
        float p[4];

        moby_point_position(moby, 0xB, p);
        data->f424 = spawn_big_axebot_axe(moby, p);
    }
}

/* Creates the big axebot's axe (class 0x23A) at spawn_position, with the axebot's draw distance, yaw and spawn frame. */
void *spawn_big_axebot_axe(char *source_moby, char *spawn_position) {
    char  saved_position[16];
    char *saved_position_ptr = saved_position;
    char *created_moby;
    *(OvlQuad *)saved_position = *(OvlQuad *)spawn_position;
    created_moby = (char *)create_moby(0x23a);
    if (created_moby != 0) {
        MOBY(created_moby)->update_dist = 0xFF;
        MOBY(created_moby)->draw_dist = ((unsigned short)MOBY(source_moby)->draw_dist);
        created_moby[0x31] = 1;
        *(int *)(created_moby + 0x40) = 0;
        *(int *)(created_moby + 0x44) = 0;
        MOBY(created_moby)->rot.z = MOBY(source_moby)->rot.z;
        MOBY(created_moby)->spawn_frame = MOBY(source_moby)->spawn_frame;
        qcopy(created_moby + 0x10, saved_position_ptr);
        update_entity(created_moby);
    }
    return created_moby;
}
