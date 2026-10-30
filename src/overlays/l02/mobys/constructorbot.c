/* Level 02 moby class constructorbot (0x264): its update, and its damage handling and retargeting. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "sda.h"
#include "qcopy.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/l02/mobys/constructorbot_globals.h"

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");

extern float D_0015ED6C;
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern int   is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");

extern int
fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D8B80), where it is exact; names translated to the US level program. */

typedef struct {
    /* 0x000 */ char          pad000[0x20];
    /* 0x020 */ float         f20;
    /* 0x024 */ char          pad024[4];
    /* 0x028 */ char          b28;
    /* 0x029 */ char          b29;
    /* 0x02A */ char          pad02A[6];
    /* 0x030 */ float         f30;
    /* 0x034 */ char          pad034[0xC];
    /* 0x040 */ float         velocity[4];
    /* 0x050 */ char          pad050[8];
    /* 0x058 */ char          b58;
    /* 0x059 */ char          pad059;
    /* 0x05A */ char          b5A;
    /* 0x05B */ char          pad05B;
    /* 0x05C */ char          b5C;
    /* 0x05D */ char          pad05D[0x13];
    /* 0x070 */ char          knockback[0x60];
    /* 0x0D0 */ char          walk[8];
    /* 0x0D8 */ float         walk_d8;
    /* 0x0DC */ float         walk_dc;
    /* 0x0E0 */ char          pad0E0[0x14];
    /* 0x0F4 */ float         walk_step;
    /* 0x0F8 */ char          pad0F8[0x14];
    /* 0x10C */ float         walk_10c;
    /* 0x110 */ float         walk_110;
    /* 0x114 */ float         walk_114;
    /* 0x118 */ char          pad118[0x68];
    /* 0x180 */ float         attack_range;
    /* 0x184 */ float         wake_range;
    /* 0x188 */ int           post;
    /* 0x18C */ int           wake_volume;
    /* 0x190 */ char          pad190[4];
    /* 0x194 */ int           start;
    /* 0x198 */ char          pad198[8];
    /* 0x1A0 */ unsigned char blink[0x40];
    /* 0x1E0 */ float         wander[4];
    /* 0x1F0 */ char         *shots[9];
    /* 0x214 */ float         turn_speed;
    /* 0x218 */ char          pad218[8];
    /* 0x220 */ char         *target;
} L02BotData;

extern char  D_L02_001673C0[];
extern float D_L02_0015F580[] MACRO_ADDR;
extern int   D_L02_0015F5CC_i __asm__("D_L02_0015F5CC");

#include "rnc/overlay/l02/mobys/constructorbot_data/constructorbot_update.h"

int          D_L02_00161A88 = 0x40808080;
int          D_L02_00161A90 = 0x3c;
int          D_L02_00161A94 = 0x78;
extern float constructorbot_wake_spray_planar_magnitude_max_gp __asm__("D_L02_00161A98")
    __attribute__((sda));
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int   FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern s32   rand(void);
extern void *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float  scale,
                              float *v10,
                              float *v11,
                              float *v12);
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_00258278(char *moby, float target, float *vel, float k, float d, float max);
extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern void FUN_L00_0025a120(void *);
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
extern void FUN_L00_0025f730(void *a, float x);
extern void FUN_L00_00263ac8(float x, int a, int b, unsigned char *m);
extern void spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern void FUN_L01_0026d930(char *p);
extern void reset_shot_tracking(char *moby) __asm__("FUN_L02_00264e40");
extern void effect_slots_fire(void *, void *, void *, float) __asm__("FUN_L02_00264e70");
extern void
     collect_mobies_around_tracked_objects(char **slots, int a, int b) __asm__("FUN_L02_002651d0");
void handle_constructorbot_damage_and_retargeting(void *owner_moby) __asm__("FUN_L02_002d85b8");

extern void handle_constructorbot_damage_and_retargeting(void *) __asm__("FUN_L02_002d85b8");

void constructorbot_update(struct Moby *moby) __asm__("FUN_L02_002d7748");

void constructorbot_update(struct Moby *moby) {
    enum {
        L02_BOT_INITIALIZE = 0,
        L02_BOT_WAIT_FOR_WAKE_ANIMATION = 2,
        L02_BOT_ALIGN_TO_GROUND_AT_POST = 3,
        L02_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVE_TO_POST = 4,
        L02_BOT_MOVE_TO_POST = 5,
        L02_BOT_WANDER = 6,
        L02_BOT_SELECT_ATTACK_OR_WANDER = 7,
        L02_BOT_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT = 8,
        L02_BOT_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION = 9,
        L02_BOT_WAIT_FOR_WAKE_TRIGGER = 10,
        L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK = 11,
        L02_BOT_UPDATE_ATTACK_SHOTS = 12,
        L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK_RECHECK = 13,
        L02_BOT_HIT_KNOCKBACK_RECOVERY = 16,
        L02_BOT_DEATH_KNOCKBACK = 17
    };
    enum {
        L02_BOT_START_SELECT_ATTACK_OR_WANDER = 0,
        L02_BOT_START_WAIT_FOR_WAKE_TRIGGER = 1,
        L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT = 2,
        L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION = 3
    };
    float       position[4];
    float       direction[4];
    char        shot[0x30];
    L02BotData *data = (L02BotData *)moby->pvars;
    char       *target;
    float       angle;
    float       ground;
    float       wander_heading;
    float       spray_heading;
    float       spray_radius;
    float       step;
    int         triggered;

    FUN_L00_00263ac8(2.5f, (int)moby, 1, data->blink);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L02_001673C0) < 30.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x18;
    }
    handle_constructorbot_damage_and_retargeting(moby);
    switch (moby->state) {
    case L02_BOT_INITIALIZE:
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        moby->flags |= 0x1000;
        data->b5C = 1;
        data->f30 = 1.0f;
        data->b58 = 0xF;
        data->b5A = 8;
        data->b29 = 0;
        moby->spawn_frame = *(long *)(((char *)hero.moby) + 0x38);
        FUN_L01_0026d930(data->walk);
        step = D_0015ED6C * 4.0f;
        data->b28 = 2;
        data->walk_10c = 0.03f;
        data->walk_110 = 0.3f;
        data->walk_114 = 0.25f;
        data->walk_step = step;
        data->walk_dc = 2.0f;
        data->walk_d8 = 2.0f;
        data->f20 = 3.0f;
        clear_u64_value(data->velocity);
        reset_shot_tracking((char *)data->shots);
        switch (data->start) {
        case L02_BOT_START_SELECT_ATTACK_OR_WANDER:
        default:
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            moby->state = 7;
            break;
        case L02_BOT_START_WAIT_FOR_WAKE_TRIGGER:
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 0);
            }
            moby->state = 10;
            break;
        case L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT:
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, 0);
            }
            moby->state = 8;
            break;
        case L02_BOT_START_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION:
            if (moby->prev_seq != 15) {
                blend_moby_animation(moby, 15, 0, 0);
            }
            moby->state = 9;
            break;
        }
        break;
    case L02_BOT_WAIT_FOR_WAKE_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = 3;
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, 0);
            }
        }
        break;
    case L02_BOT_ALIGN_TO_GROUND_AT_POST:
        target = data->target;
        angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                             *(float *)(target + 0x14) - moby->pos.y);
        FUN_L00_00258278(
            (char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, D_0015ED6C * 1.5707964f);
        fast_vec_sub(position, &l02_cuboids[data->post].pos, &moby->pos);
        position[2] = 0.0f;
        FUN_L00_0025f730(position, D_0015ED6C * 6.0f);
        fast_vec_add(&moby->pos, &moby->pos, position);
        ground = probe_ground_height(&moby->pos, 0, 0.5f);
        approach_value(
            &data->velocity[2], D_0015ED6C * 40.0f, constructorbot_gravity_gp * frame_time_sq);
        approach_value(&moby->pos.z, ground, data->velocity[2]);
        if (moby->pos.z == ground) {
            moby->state = 4;
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 4, scale_game_frames(10));
            }
        }
        break;
    case L02_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVE_TO_POST:
        if (moby->anim_flags & 2) {
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_MOVE_TO_POST:
        if (FUN_L00_00259028((char *)moby,
                             data->walk,
                             (float *)&l02_cuboids[data->post].pos,
                             (char *)data->velocity) == 4) {
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(30));
            }
            moby->state = 7;
        }
        break;
    case L02_BOT_WANDER:
        if (FUN_L00_00259028((char *)moby, data->walk, data->wander, (char *)data->velocity) & 6) {
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            moby->state = 7;
        } else if (fast_vec_dist(data->target + 0x10, &l02_cuboids[data->post].pos) <
                       data->attack_range &&
                   absolute_float(*(float *)(data->target + 0x18) - moby->pos.z) < 4.0f) {
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case L02_BOT_SELECT_ATTACK_OR_WANDER:
        if (fast_vec_dist(data->target + 0x10, &l02_cuboids[data->post].pos) < data->attack_range &&
            absolute_float(*(float *)(data->target + 0x18) - moby->pos.z) < 4.0f) {
            if (fast_vec_dist(&moby->pos, &l02_cuboids[data->post].pos) < 1.0f) {
                target = data->target;
                angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                                     *(float *)(target + 0x14) - moby->pos.y);
                FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, 0.1f);
                if (fast_difference_between_rotations(moby->rot.z, angle) < 0.08726646f) {
                    moby->state = 0xB;
                    if (moby->prev_seq != 8) {
                        blend_moby_animation(moby, 8, 0, scale_game_frames(6));
                    }
                }
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                }
            }
        } else if (moby->anim_flags & 2) {
            wander_heading = random_angle_radians();
            data->wander[0] = fast_cos(wander_heading) * random_float_between(1.0f, 2.0f);
            data->wander[1] = fast_sin(wander_heading) * random_float_between(1.0f, 2.0f);
            data->wander[2] = 0.0f;
            fast_vec_add(data->wander, data->wander, &l02_cuboids[data->post].pos);
            moby->state = 6;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case L02_BOT_WAIT_FOR_WAKE_TRIGGER_BEFORE_GROUND_ALIGNMENT:
    case L02_BOT_WAIT_FOR_WAKE_TRIGGER_WITH_ALTERNATE_WAKE_ANIMATION:
        if (!(D_L02_0015F5CC_i & 3) && moby->visible != 0) {
            spray_radius =
                random_float_between(0.0f, constructorbot_wake_spray_planar_magnitude_max_gp) *
                D_0015ED6C;
            spray_heading = random_angle_radians();
            direction[0] = fast_cos(spray_heading) * spray_radius;
            direction[1] = fast_sin(spray_heading) * spray_radius;
            direction[2] = 0.0f;
            moby_point_position(moby, 0, position);
            spawn_burst(position,
                        direction,
                        D_L02_00161A88,
                        D_L02_00161A88,
                        random_int_between(scale_game_frames(D_L02_00161A90),
                                           scale_game_frames(D_L02_00161A94)),
                        1,
                        40000.0f);
        }
    case L02_BOT_WAIT_FOR_WAKE_TRIGGER:
        triggered = 0;
        if (data->wake_volume >= 0) {
            triggered = is_point_inside_clip_volume(&hero.ground_point, data->wake_volume) != 0;
        } else if (fast_vec_dist(&hero.motion.pos, &l02_cuboids[data->post].pos) <
                   data->wake_range) {
            triggered = 1;
        }
        if (triggered) {
            if (moby->state == 8) {
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                }
                moby->state = 2;
            } else if (moby->state == 9) {
                if (moby->prev_seq != 0x10) {
                    blend_moby_animation(moby, 0x10, 0, scale_game_frames(10));
                }
                moby->state = 2;
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(6));
                }
            }
        }
        break;
    case L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK:
        moby->anim_speed = 2.0f;
        target = data->target;
        angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                             *(float *)(target + 0x14) - moby->pos.y);
        FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, 0.1f);
        if (moby->anim_flags & 2) {
            moby->anim_speed = 1.0f;
            moby->state = 0xC;
            if (moby->prev_seq != 9) {
                blend_moby_animation(moby, 9, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_UPDATE_ATTACK_SHOTS:
        moby->anim_speed = 0.5f;
        moby_point_position(moby, 0, position);
        direction[0] = fast_cos(fast_arc_tan(position[0] - moby->pos.x, position[1] - moby->pos.y));
        direction[1] = fast_sin(fast_arc_tan(position[0] - moby->pos.x, position[1] - moby->pos.y));
        direction[2] = -0.1f;
        effect_slots_fire(
            data->shots, position, direction, constructorbot_attack_shot_nominal_length_gp);
        direction[3] = 5627.925f;
        direction[2] = 1.0f;
        make_hit_descriptor(shot, (int)moby, 0x10001, direction, 1.0f);
        shot[0x18] = 5;
        shot[0x19] = 1;
        *(short *)(shot + 0x1A) = (u16)moby->oclass;
        collect_mobies_around_tracked_objects(data->shots, (int)moby, (int)shot);
        if (moby->anim_flags & 2) {
            moby->anim_speed = 1.0f;
            moby->state = 0xD;
            if (random_integer_below(0xFF) & 1) {
                if (moby->prev_seq != 10) {
                    blend_moby_animation(moby, 10, 0, scale_game_frames(6));
                }
            } else if (moby->prev_seq != 8) {
                blend_moby_animation(moby, 8, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_WAIT_FOR_ANIMATION_BEFORE_ATTACK_RECHECK:
        target = data->target;
        angle = fast_arc_tan(*(float *)(target + 0x10) - moby->pos.x,
                             *(float *)(target + 0x14) - moby->pos.y);
        FUN_L00_00258278((char *)moby, angle, &data->turn_speed, 0.02f, 0.3f, 0.1f);
        if (moby->anim_flags & 2) {
            if (vector_distance_xy(&moby->pos, data->target + 0x10) < data->attack_range + 1.0f &&
                absolute_float(*(float *)(data->target + 0x18) - moby->pos.z) < 3.0f) {
                moby->state = 0xC;
                if (moby->prev_seq != 9) {
                    blend_moby_animation(moby, 9, 0, scale_game_frames(6));
                }
            } else {
                moby->state = 7;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(6));
                }
            }
        }
        break;
    case L02_BOT_HIT_KNOCKBACK_RECOVERY:
        if (step_moby_motion(moby, data->knockback) & 0x40) {
            moby->state = 7;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(6));
            }
        }
        break;
    case L02_BOT_DEATH_KNOCKBACK:
        if (step_moby_motion(moby, data->knockback) & 0x140) {
            qcopy(position, &moby->pos);
            position[2] += 1.0f;
            FUN_L00_00257470(moby, 0, -1);
            spawn_explosion(moby,
                            data->velocity,
                            position,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            -1,
                            1.0f,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00263fd8((char *)moby,
                             0x6D7,
                             (float *)(&moby->pos),
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x6D8,
                             (float *)(&moby->pos),
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x6D9,
                             (float *)(&moby->pos),
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x6E9,
                             (float *)(&moby->pos),
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x782,
                             (float *)(&moby->pos),
                             &moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    FUN_L00_0025a120(moby);
}

extern float constructorbot_death_knockback_horizontal_speed_gp __asm__("D_L02_00161A64")
    __attribute__((sda));
extern float constructorbot_death_knockback_vertical_speed_gp __asm__("D_L02_00161A68")
    __attribute__((sda));
extern float constructorbot_hit_knockback_horizontal_speed_gp __asm__("D_L02_00161A6C")
    __attribute__((sda));
extern float constructorbot_hit_knockback_vertical_speed_gp __asm__("D_L02_00161A70")
    __attribute__((sda));

#include "rnc/overlay/l02/mobys/constructorbot_data/handle_constructorbot_damage_and_retargeting.h"

extern int select_moby_target_goal(char *a, char *o, float best) __asm__("FUN_L02_00261c80");

/* pvars of the class FUN_L02_002d85b8 serves (only the fields it touches). */
typedef struct {
    char          pad0[0x20];
    float         hp; /* 0x20 */
    char          pad24[0x3C];
    short         anim[3]; /* 0x60: passed to trooper_anim_update / trooper_anim_finish */
    unsigned char pad66;
    unsigned char flash; /* 0x67 */
    char          pad68[8];
    char          knock[0x10]; /* 0x70: passed to trooper_start_knockback */
    float         f80;         /* 0x80 */
    float         f84;
    float         f88;
    float         f8C;
    int           i90; /* 0x90 */
    int           i94;
    float         f98;
    char          pad9C[4];
    float         fA0; /* 0xA0 */
    char          padA4[9];
    unsigned char bAD; /* 0xAD */
    char          padAE[0xE];
    float         fBC; /* 0xBC */
    float         fC0;
    float         fC4;
    char          padC8[0x158];
    char         *target; /* 0x220 */
} Vars85b8;

/* Hit reaction: takes damage, flinches or dies (knocked away from the hero), then retargets. */
void handle_constructorbot_damage_and_retargeting(void *owner_moby) {
    char     *moby = owner_moby;
    Vars85b8 *moby_vars = ((Vars85b8 *)MOBY(moby)->pvars);
    char     *hit_record;
    char     *current_target;
    int       target[20];
    int       hit;
    float     dmg;

    dmg = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0 && *(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x264) {
        hit_record = 0;
    }
    fun_00213928(moby, hit_record, &moby_vars->hp, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && ((unsigned char *)moby)[0x20] != 0x11) {
        moby_vars->hp -= dmg;
        moby_vars->i90 = truncate_float_to_s32(614.4f);
        moby_vars->f98 = 0.6f;
        if (0.0f < moby_vars->hp) {
            if (dmg < 1.0f) {
                moby_vars->flash = 100;
            } else {
                void *k = moby_vars->knock;
                moby_vars->flash = 0x78;
                moby_vars->f80 = constructorbot_gravity_gp * frame_time_sq;
                moby_vars->f84 =
                    constructorbot_hit_knockback_horizontal_speed_approach_rate_gp * frame_time_sq;
                moby_vars->f88 = constructorbot_hit_knockback_horizontal_speed_gp * D_0015ED6C;
                moby_vars->f8C = constructorbot_hit_knockback_vertical_speed_gp * D_0015ED6C;
                moby_vars->fBC = constructorbot_knockback_horizontal_target_speed_gp * D_0015ED6C;
                moby_vars->fA0 = 0.5f;
                moby_vars->i94 = 25;
                moby_vars->bAD = 0;
                trooper_start_knockback(fast_arc_tan(MOBY(moby)->pos.x - hero.motion.pos.f[0],
                                                     MOBY(moby)->pos.y - hero.motion.pos.f[1]),
                                        moby,
                                        k,
                                        0xB,
                                        1,
                                        0);
                moby_vars->fC0 = 6.5f;
                moby_vars->fC4 = 11.0f;
                moby[0x20] = 0x10;
            }
        } else {
            void *k2 = moby_vars->knock;
            moby_vars->f80 = constructorbot_gravity_gp * frame_time_sq;
            moby_vars->f84 =
                constructorbot_death_knockback_horizontal_speed_approach_rate_gp * frame_time_sq;
            moby_vars->f88 = constructorbot_death_knockback_horizontal_speed_gp * D_0015ED6C;
            moby_vars->f8C = constructorbot_death_knockback_vertical_speed_gp * D_0015ED6C;
            moby_vars->fBC = constructorbot_knockback_horizontal_target_speed_gp * D_0015ED6C;
            moby_vars->i94 = 9;
            moby_vars->bAD = 0;
            trooper_start_knockback(fast_arc_tan(MOBY(moby)->pos.x - hero.motion.pos.f[0],
                                                 MOBY(moby)->pos.y - hero.motion.pos.f[1]),
                                    moby,
                                    k2,
                                    0xE,
                                    1,
                                    0);
            moby_vars->fC0 = 6.5f;
            moby_vars->fC4 = 13.0f;
            MOBY(moby)->flags &= 0xEFFF;
            moby_vars->flash = 0xFA;
            moby[0x20] = 0x11;
        }
        trooper_anim_update(moby, moby_vars->anim);
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
    current_target = moby_vars->target;
    if (current_target == 0 || current_target[0x20] < 0 ||
        (*(short *)(current_target + 0xA6) != 0x10E && *(short *)(current_target + 0xA6) != 0xCB &&
         *(short *)(current_target + 0xA6) != 0) ||
        ((unsigned char *)moby)[0x20] != 0xC) {
        select_moby_target_goal(moby, (char *)target, 14.0f);
        moby_vars->target = (char *)target[16];
    }
    if (moby_vars->target == 0) {
        moby_vars->target = ((char *)hero.moby);
    }
    trooper_anim_finish(moby, moby_vars->anim);
}
