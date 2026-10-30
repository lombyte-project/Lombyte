/* Level 16 test_dummy: the training dummies. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"

#include "qcopy.h"

extern char *create_moby(int);
extern char *create_moby(int) __asm__("FUN_0020c4f8");

extern float D_0015ED6C;
f32          probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

s32         random_integer_below(s32 arg0) __asm__("FUN_00213260");
extern void FUN_L00_00257470(void *, int, int);

#include "sda.h"
#include "rnc/overlay/quad.h"

#include "rnc/overlay/l16/mobys/hit_descriptor.h"

extern float D_0015ED6C, frame_time;
extern float frame_time_sq;
extern float random_angle_radians(void) __asm__("FUN_00213308");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

extern char *D_L16_001B0930[];

/* Updates the test dummy (moby class 541): walks its path, throws at or sprays its target, bursts and respawns. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CF180), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad00[0x20];
    float         f20;
    short         h24;
    char          pad26[2];
    unsigned char b28;
    char          pad29[5];
    unsigned char b2E;
    char          pad2F[0x41];
    float         target[4];
    char          pad80[0x34];
    int           mode;
    char          padB8[8];
    int           kind;
    int           path;
    int           lives;
    int           iCC;
    int           aim[2];
    int           sound_path;
    int           node;
    float         spawn[4];
    int           timer;
    char          padF4[4];
    char         *route;
    float         turn;
    char          pad100[4];
    char         *child;
    int           step;
    float         f10C;
    unsigned char next_state;
    char          pad111[3];
    int           next_anim;
    char          pad118[4];
    float         f11C;
    char          slots[0x20];
    char          block140[1];
} L16DummyData;

extern char *D_L16_00174258 NOT_SDA;
extern float                frame_time_sq;
extern float                FUN_L00_0025abf0(void *, void *, void *, float, float);
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float probe_ground_height(void *, int, float);
extern int   D_L16_0015F580_c __asm__("D_L16_0015F580");
extern char *D_L16_001600EC_c __asm__("D_L16_001600EC");
extern int   D_L16_001619C8_gp __asm__("D_L16_001619C8") __attribute__((sda));
extern float test_dummy_bomb_horizontal_launch_speed_gp __asm__("D_L16_001619CC")
    __attribute__((sda));

#include "rnc/overlay/l16/mobys/test_dummy_data/test_dummy_update.h"

extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
int        move_test_dummy_toward_position_and_heading(char *moby,
                                                       void *target_position,
                                                       float angle) __asm__("FUN_L16_002ced20");

extern int
move_test_dummy_toward_position_and_heading(char *, void *, float) __asm__("FUN_L16_002ced20");
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern void *FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

#include "rnc/overlay/l16/mobys/test_dummy_door.h"

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025f730(void *, float);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void reset_shot_tracking(char *) __asm__("FUN_L02_00264e40");
extern void effect_slots_fire(void *, void *, void *, float) __asm__("FUN_L02_00264e70");
extern void collect_mobies_around_tracked_objects(char **, int, int) __asm__("FUN_L02_002651d0");
extern void update_test_dummy_damage_and_respawn(void *) __asm__("FUN_L16_002ce9f0");
extern unsigned char            *
func_L16_002C75D0_2cf180(int, void *, float *, float, int) __asm__("FUN_L16_002c6268");
extern int func_L00_0025A8E8_2cf180(
    void *, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");

void test_dummy_update(unsigned char *m) __asm__("FUN_L16_002cddb8");

void test_dummy_update(unsigned char *moby) {
    enum {
        TEST_DUMMY_STATE_INITIALIZE = 0,
        TEST_DUMMY_STATE_IDLE = 1,
        TEST_DUMMY_STATE_APPROACH_FIRST_PATH_NODE = 2,
        TEST_DUMMY_STATE_APPROACH_THROW_NODE = 3,
        TEST_DUMMY_STATE_THROW_AT_TARGET = 4,
        TEST_DUMMY_STATE_MOVE_ALONG_PATH = 5,
        TEST_DUMMY_STATE_TURN_TO_NEXT_PATH_NODE = 6,
        TEST_DUMMY_STATE_APPROACH_SPRAY_NODE = 7,
        TEST_DUMMY_STATE_SPRAY_FROM_CHILD = 8,
        TEST_DUMMY_STATE_TURN_AND_RESUME_SAVED_STATE = 9,
        TEST_DUMMY_STATE_BURST = 10,
        TEST_DUMMY_STATE_RESPAWN = 11
    };
    enum {
        TEST_DUMMY_KIND_THROWER = 0,
        TEST_DUMMY_KIND_PATH_MOVER = 1,
        TEST_DUMMY_KIND_SPRAYER = 2
    };
    L16QuadVector    position_or_delta, position_or_direction, launch_velocity;
    L16HitDescriptor fx;
    L16DummyData    *dummy_data = ((L16DummyData *)MOBY(moby)->pvars);
    /* A count, then 16-byte points from +0x10. A point copied whole is `path + i * 16 + 16`; its x and y are read
       through `path + (i << 4)`: retail forms index + base for the first and base + index for the second, which
       is what this compiler does with a multiplication and with a shift in an address. */
    char *path;

    update_test_dummy_damage_and_respawn(moby);
    path = D_L16_001B0930[dummy_data->path];
    FUN_L00_00263ac8(2.5f, (int)moby, 2, (unsigned char *)dummy_data->block140);
    switch (moby[0x20]) {
    case TEST_DUMMY_STATE_INITIALIZE: /* set up: lives, home position, the held child for the variant */
        dummy_data->b2E = 1;
        if (dummy_data->lives > 0) {
            MOBY(moby)->unkB4 /= dummy_data->lives;
        }
        moby[0x20] = 1;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
        if (random_integer_below(2)) {
            MOBY(moby)->flags |= 0x8000;
        }
        dummy_data->b28 = 1;
        dummy_data->h24 = 2;
        dummy_data->f20 = 2.0f;
        dummy_data->f11C = random_angle_radians();
        dummy_data->route = find_nearest_test_dummy_door(dummy_data->iCC, moby + 0x10);
        qcopy(dummy_data->spawn, moby + 0x10);
        switch (dummy_data->kind) {
        case TEST_DUMMY_KIND_THROWER:
            dummy_data->child = create_moby(0x35F);
            break;
        case TEST_DUMMY_KIND_PATH_MOVER:
            dummy_data->child = create_moby(0x374);
            break;
        case TEST_DUMMY_KIND_SPRAYER:
            dummy_data->child = create_moby(0x36C);
            reset_shot_tracking(dummy_data->slots);
            break;
        }
        if (dummy_data->child) {
            *(unsigned short *)(dummy_data->child + 0x34) |= 0x100;
            *(short *)(dummy_data->child + 0x32) = 0x40;
            *(unsigned char *)(dummy_data->child + 0x30) = 0;
            *(unsigned char *)(dummy_data->child + 0x31) = 1;
        }
        break;
    case TEST_DUMMY_STATE_IDLE:
        break;
    case TEST_DUMMY_STATE_APPROACH_FIRST_PATH_NODE: /* turn to the path's first node, then start by variant */
        refresh_test_dummy_door_timer(dummy_data->route);
        position_or_delta.quad = *(u128 *)(path + 0x10);
        if (move_test_dummy_toward_position_and_heading(
                moby, position_or_delta.f, MOBY(moby)->rot.z)) {
            dummy_data->timer = scale_game_frames(30);
            dummy_data->node = 1;
            dummy_data->step = 1;
            if (dummy_data->kind == 0) {
                moby[0x20] = 3;
            } else if (dummy_data->kind == 1) {
                moby[0x20] = 5;
            } else {
                moby[0x20] = 7;
            }
        }
        break;
    case TEST_DUMMY_STATE_APPROACH_THROW_NODE: /* thrower: wait, face the node, wind up */
        if (fast_dec_timer(&dummy_data->timer)) {
            position_or_delta.quad = *(u128 *)(path + dummy_data->node * 16 + 16);
            if (move_test_dummy_toward_position_and_heading(
                    moby,
                    position_or_delta.f,
                    fast_arc_tan(((float *)(path + (dummy_data->node << 4)))[4] - MOBY(moby)->pos.x,
                                 ((float *)(path + (dummy_data->node << 4)))[5] -
                                     MOBY(moby)->pos.y))) {
                dummy_data->timer = scale_game_frames(30);
                moby[0x20] = 4;
                if (moby[0x53] != 21) {
                    blend_moby_animation(moby, 21, 0, scale_game_frames(30));
                }
                if (random_integer_below(2)) {
                    char *p = D_L16_001B0930[dummy_data->sound_path];
                    FUN_L00_0025ff38(
                        12.0f, (char *)moby, dummy_data->target, 0, 0, p + 16, *(int *)p);
                } else {
                    dummy_data->mode = 2;
                }
            }
        }
        break;
    case TEST_DUMMY_STATE_THROW_AT_TARGET: /* thrower: launch the held object at the target, then face it or the next node */
        if (dummy_data->mode == 2) {
            qcopy(position_or_delta.f,
                  D_L16_001600EC_c + dummy_data->aim[dummy_data->node] * 0x80 + 0x30);
        } else {
            qcopy(position_or_delta.f, dummy_data->target);
        }
        if (is_value_within_interpolated_window(moby, 15.0f) && dummy_data->child) {
            float speed = test_dummy_bomb_horizontal_launch_speed_gp * D_0015ED6C;
            float gravity = frame_time_sq * 10.0f;
            position_or_delta.f[2] = probe_ground_height(position_or_delta.f, 0, 0.5f);
            moby_point_position(dummy_data->child, 0, position_or_direction.f);
            fast_vec_sub(launch_velocity.f, position_or_delta.f, position_or_direction.f);
            launch_velocity.f[2] = 0.0f;
            fast_vec_normalize(launch_velocity.f, launch_velocity.f, speed);
            launch_velocity.f[2] =
                FUN_L00_0025abf0(position_or_direction.f, position_or_delta.f, 0, speed, -gravity);
            func_L16_002C75D0_2cf180((int)moby,
                                     position_or_direction.f,
                                     launch_velocity.f,
                                     gravity,
                                     scale_game_frames(300));
        } else if (moby[0x70] & 2) {
            dummy_data->node = (dummy_data->node + 1) & 1;
            moby[0x20] = 3;
            if (moby[0x53] != 23) {
                blend_moby_animation(moby, 23, 0, scale_game_frames(30));
            }
        }
        if (compute_interpolated_record_value(moby) < 15.0f) {
            position_or_direction.quad = *(u128 *)(moby + 16);
            move_test_dummy_toward_position_and_heading(
                moby,
                position_or_direction.f,
                fast_arc_tan(position_or_delta.f[0] - MOBY(moby)->pos.x,
                             position_or_delta.f[1] - MOBY(moby)->pos.y));
        } else {
            int node = (dummy_data->node + 1) & 1;
            position_or_direction.quad = *(u128 *)(moby + 16);
            move_test_dummy_toward_position_and_heading(
                moby,
                position_or_direction.f,
                fast_arc_tan(((float *)(path + (node << 4)))[4] - MOBY(moby)->pos.x,
                             ((float *)(path + (node << 4)))[5] - MOBY(moby)->pos.y));
        }
        break;
    case TEST_DUMMY_STATE_MOVE_ALONG_PATH: /* slider: move toward the node, emit, stop on arrival */
        if (moby[0x53] == 10) {
            if (moby[0x70] & 2) {
                blend_moby_animation(moby, 11, 0, 1);
            }
        } else {
            char        *position = (char *)moby + 16;
            struct Hero *player;
            fast_vec_sub(position_or_delta.f, path + (dummy_data->node * 16 + 16), position);
            FUN_L00_0025f730(position_or_delta.f,
                             test_dummy_path_movement_speed_limit_gp * D_0015ED6C);
            fast_vec_add(position, position, position_or_delta.f);
            moby_point_position(moby, D_L16_001619C8_gp, position_or_direction.f);
            func_L00_0025A8E8_2cf180(moby, 0.333f, position_or_direction.f, 1, 1.0f, 1.0f, 0, 1, 0);
            player = &hero;
            if (D_L16_00174258 == ((char *)player->moby) && player->invulnerable_frames == 0) {
                allocate_voice_for_target_entry(6, 0, moby);
            }
            if (fast_vec_length(position_or_delta.f) < 0.0001f) {
                if (moby[0x53] != 9) {
                    blend_moby_animation(moby, 9, 0, scale_game_frames(20));
                }
                moby[0x20] = 6;
                dummy_data->timer = scale_game_frames(60);
                if (dummy_data->node == 0) {
                    dummy_data->step = 1;
                } else if (dummy_data->node == *(int *)path - 1) {
                    dummy_data->step = -1;
                }
            }
        }
        break;
    case TEST_DUMMY_STATE_TURN_TO_NEXT_PATH_NODE: { /* slider: turn toward the next node while the timer runs */
        float *yaw = (float *)&MOBY(moby)->rot.z;
        int    next = dummy_data->node + dummy_data->step;
        float  angle = fast_arc_tan(((float *)(path + (next << 4)))[4] - MOBY(moby)->pos.x,
                                   ((float *)(path + (next << 4)))[5] - MOBY(moby)->pos.y);
        approach_angle(yaw,
                       &dummy_data->turn,
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       D_0015ED6C * 25.132742f);
        if (fast_dec_timer(&dummy_data->timer)) {
            dummy_data->node = next;
            if (moby[0x53] != 10) {
                blend_moby_animation(moby, 10, 0, scale_game_frames(10));
            }
            moby[0x20] = 5;
        }
        break;
    }
    case TEST_DUMMY_STATE_APPROACH_SPRAY_NODE: /* sprayer: wait, face the node, start */
        if (fast_dec_timer(&dummy_data->timer)) {
            position_or_delta.quad = *(u128 *)(path + dummy_data->node * 16 + 16);
            if (move_test_dummy_toward_position_and_heading(
                    moby, position_or_delta.f, MOBY(moby)->rot.z)) {
                dummy_data->timer = scale_game_frames(30);
                moby[0x20] = 8;
                if (moby[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(30));
                }
                if (random_integer_below(2)) {
                    char *p = D_L16_001B0930[dummy_data->sound_path];
                    FUN_L00_0025ff38(
                        12.0f, (char *)moby, dummy_data->target, 0, 0, p + 16, *(int *)p);
                } else {
                    dummy_data->mode = 2;
                }
            }
        }
        break;
    case TEST_DUMMY_STATE_SPRAY_FROM_CHILD: /* sprayer: face the target and emit from the child toward it */
        if (dummy_data->mode != 2) {
            position_or_delta.quad = *(u128 *)(moby + 16);
            move_test_dummy_toward_position_and_heading(
                moby,
                position_or_delta.f,
                fast_arc_tan(dummy_data->target[0] - MOBY(moby)->pos.x,
                             dummy_data->target[1] - MOBY(moby)->pos.y));
        }
        if (dummy_data->child) {
            char *slots;
            moby_point_position(dummy_data->child, 0, position_or_delta.f);
            slots = dummy_data->slots;
            position_or_direction.f[0] =
                fast_cos(fast_arc_tan(position_or_delta.f[0] - MOBY(moby)->pos.x,
                                      position_or_delta.f[1] - MOBY(moby)->pos.y));
            position_or_direction.f[1] =
                fast_sin(fast_arc_tan(position_or_delta.f[0] - MOBY(moby)->pos.x,
                                      position_or_delta.f[1] - MOBY(moby)->pos.y));
            position_or_direction.f[2] = 0.0f;
            effect_slots_fire(
                slots, position_or_delta.f, position_or_direction.f, test_dummy_spray_range_gp);
            position_or_direction.f[3] = 5627.925f;
            position_or_direction.f[2] = 1.0f;
            make_hit_descriptor(&fx, moby, 0x10001, 1.0f, position_or_direction.f);
            fx.kind = 5;
            fx.enabled = 1;
            fx.cls = ((unsigned short)MOBY(moby)->oclass);
            collect_mobies_around_tracked_objects((char **)slots, (int)moby, (int)&fx);
        }
        if (moby[0x70] & 2) {
            dummy_data->node = (dummy_data->node + 1) & 1;
            moby[0x20] = 7;
            if (moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(30));
            }
        }
        break;
    case TEST_DUMMY_STATE_TURN_AND_RESUME_SAVED_STATE: /* turn back to the node, then resume the saved state and animation */
        position_or_delta.quad = *(u128 *)(path + dummy_data->node * 16 + 16);
        move_test_dummy_toward_position_and_heading(moby, position_or_delta.f, dummy_data->f10C);
        if (moby[0x70] & 2) {
            moby[0x20] = dummy_data->next_state;
            if (moby[0x53] != dummy_data->next_anim) {
                blend_moby_animation(moby, dummy_data->next_anim, 0, scale_game_frames(20));
            }
        }
        break;
    case TEST_DUMMY_STATE_BURST: { /* burst into three pieces; respawn at home or delete */
        char *position = (char *)moby + 16;
        char *moby_rotation = (char *)moby + 0x40;
        float burst;
        dummy_data->f20 = 2.0f;
        dummy_data->timer = scale_game_frames(60);
        burst = frame_time_sq * 12.0f;
        FUN_L00_00263fd8(moby,
                         0x655,
                         position,
                         moby_rotation,
                         0,
                         0,
                         burst,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c);
        FUN_L00_00263fd8(moby,
                         0x656,
                         position,
                         moby_rotation,
                         0,
                         0,
                         burst,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c);
        FUN_L00_00263fd8(moby,
                         0x657,
                         position,
                         moby_rotation,
                         0,
                         0,
                         burst,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c);
        qcopy(position_or_delta.f, position);
        position_or_delta.f[2] += 1.2f;
        cart_burst(moby, position_or_delta.f, -1, 0.5f, 10.0f);
        FUN_L00_00257470(moby, 0, -1);
        allocate_voice_for_target_entry(7, 0, moby);
        dummy_data->lives--;
        if (dummy_data->lives != -1) {
            moby[0x20] = 11;
            qcopy(position, dummy_data->spawn);
            dummy_data->timer = scale_game_frames(60);
        } else {
            if (dummy_data->child) {
                mark_moby_for_removal(dummy_data->child);
            }
            mark_moby_for_removal(moby);
        }
        break;
    }
    case TEST_DUMMY_STATE_RESPAWN: /* respawn: settle to the home height as the timer runs out */
        if (fast_dec_timer(&dummy_data->timer)) {
            dummy_data->turn = 0.0f;
            moby[0x20] = 2;
            if (moby[0x53] != 9) {
                blend_moby_animation(moby, 9, 0, scale_game_frames(10));
            }
            dummy_data->f20 = 2.0f;
            MOBY(moby)->flags |= 0x1000;
        }
        MOBY(moby)->pos.z = dummy_data->spawn[2] + D_0015ED6C * 3.0f * (float)dummy_data->timer;
        break;
    }
}

extern int D_L16_0015F5CC;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

typedef struct {
    char               pad00[0x20];
    float              health; /* 0x20 */
    char               pad24[10];
    unsigned char      substate; /* 0x2E: 2 asks for a respawn */
    char               pad2F[0x31];
    short              reaction[3]; /* 0x60: record func_L00_0025E4B0 and func_L00_0025E590 take */
    char               pad66;
    unsigned char      reaction_time; /* 0x67 */
    char               pad68[0x60];
    int                lives; /* 0xC8 */
    char               padCC[0x14];
    float              spawn[4]; /* 0xE0 */
    int                timer;    /* 0xF0 */
    char               padF4[0x10];
    struct L16HitMoby *child; /* 0x104 */
    char               pad108[4];
    float              heading;    /* 0x10C */
    int                next_state; /* 0x110: stored here as a word */
    int                next_anim;  /* 0x114 */
    int                frame;      /* 0x118: frame counter of the last func_L16_002D0238 */
} L16HitData;

typedef struct L16HitMoby {
    char           pad00[0x10];
    float          position[4]; /* 0x10 */
    unsigned char  state;       /* 0x20 */
    char           pad21[0x13];
    unsigned short flags; /* 0x34 */
    char           pad36[0x1D];
    unsigned char  animation; /* 0x53 */
    char           pad54[0x24];
    L16HitData    *data; /* 0x78 */
    char           pad7C[0x28];
    unsigned char  opacity; /* 0xA4 */
    char           padA5[0x1B];
    float          matrix[3][4]; /* 0xC0 */
} L16HitMoby;

extern int D_L16_001619C8_gp __asm__("D_L16_001619C8")
    __attribute__((sda)); /* no foreign declaration */
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L16_002cee70_c(char *) __asm__("FUN_L16_002cee70");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");

/* Per-frame update of a hit-point moby: handles a hit (respawn at the spawn point or removal when out of lives) and its state timers. */
void update_test_dummy_damage_and_respawn(void *moby) {
    L16HitMoby *hit_moby = moby;
    float       matrix[16];
    int         status;
    float       amount;
    L16HitData *hit_data;
    char       *record;

    if (hit_moby->state == 1) {
        return;
    }
    hit_data = hit_moby->data;
    if (hit_data->substate == 2) {
        hit_data->substate = 1;
        hit_data->health = 2.0f;
        hit_data->timer = scale_game_frames(60);
        FUN_L00_00257470(hit_moby, 0, -1);
        hit_moby->flags &= 0xEFFF;
        hit_data->lives--;
        if (hit_data->lives != -1) {
            hit_moby->state = 11;
            qcopy(hit_moby->position, hit_data->spawn);
            hit_data->timer = scale_game_frames(60);
        } else {
            if (hit_data->child) {
                mark_moby_for_removal_c2(hit_data->child);
            }
            mark_moby_for_removal_c2(hit_moby);
        }
        return;
    }
    if (hit_data->frame != D_L16_0015F5CC) {
        FUN_L16_002cee70_c((char *)hit_moby);
    }
    amount = 0.0f;
    record = find_moby_hit(hit_moby, 0x330000, 0);
    hit_reaction(hit_moby, record, &hit_data->health, 0, &status, &amount, 0, 4);
    if (status != 1 && hit_moby->state != 10 && hit_moby->state != 11) {
        hit_data->health -= amount;
        if (hit_data->health <= 0.0f) {
            hit_moby->state = 10;
        } else {
            if (hit_moby->state != 9) {
                hit_data->next_state = hit_moby->state;
                hit_data->next_anim = hit_moby->animation;
            }
            {
                float        x = hit_moby->position[0], y = hit_moby->position[1];
                struct Hero *player = &hero;
                hit_data->heading =
                    fast_arc_tan(player->motion.pos.f[0] - x, player->motion.pos.f[1] - y);
            }
            hit_moby->state = 9;
            /* No pointer local for the reaction record anywhere: d->reaction is written at each call and the
               compiler caches it itself (formed before the hit query, and formed again here with a copy after
               the call). It only forms it again here when this place cannot be reached along one straight path
               from the hit query, that is when the animation test has two arms while its common-subexpression
               pass runs: the else arm below is what supplies the second one. The assignment is dead and leaves
               no instruction; without it the function is two instructions short (see NOTES.md). */
            if (hit_moby->animation != 8) {
                blend_moby_animation(hit_moby, 8, 2, 1);
            } else {
                record = 0;
            }
            hit_data->reaction_time = 120;
            trooper_anim_update(hit_moby, hit_data->reaction);
        }
    }
    hit_moby->opacity = 255;
    trooper_anim_finish(hit_moby, hit_data->reaction);
    if (hit_data->child) {
        moby_point_position(hit_moby, D_L16_001619C8_gp, hit_data->child->position);
        build_moby_bone_transform((char *)hit_moby, D_L16_001619C8_gp, (char *)matrix);
        copy_matrix3x4(hit_data->child->matrix[0], matrix);
        fast_vec_normalize(hit_data->child->matrix[0], hit_data->child->matrix[0], 1.0f);
        fast_vec_normalize(hit_data->child->matrix[1], hit_data->child->matrix[1], 1.0f);
        fast_vec_normalize(hit_data->child->matrix[2], hit_data->child->matrix[2], 1.0f);
        update_entity(hit_data->child);
    }
    trooper_anim_finish(hit_moby, hit_data->reaction);
}

extern int D_L16_001ABCC0_u[] __asm__("D_L16_001ABCC0");
extern int D_L16_0015FFD8_d __asm__("D_L16_0015FFD8");

/* 0x002ceca8, 116 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
#include "rnc/overlay/l16/mobys/test_dummy.h"

void activate_test_dummy_group(int moby_group_index) __asm__("FUN_L16_002ceca8");

void activate_test_dummy_group(int moby_group_index) {
    short *group_entry = (short *)D_L16_001ABCC0_u[moby_group_index];
    if (group_entry != 0) {
        do {
            char *moby;
        again:
            moby = D_L16_0015FFD8_d + ((*group_entry & 0x7FFF) << 8);
            if (MOBY(moby)->oclass != 0x21D)
                goto again;
            {
                int            two = 2;
                unsigned short flags = MOBY(moby)->flags;
                moby[0x31] = 1;
                flags &= 0xFFFE;
                moby[0x20] = two;
                MOBY(moby)->flags = flags;
            }
        } while (*group_entry++ >= 0);
    }
}

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

int move_test_dummy_toward_position_and_heading(char *moby, void *target_position, float angle) {
    float target_position_copy[4] __attribute__((aligned(16)));
    float movement_delta[4] __attribute__((aligned(16)));
    float speed;
    float distance_to_target;
    char *data = ((char *)MOBY(moby)->pvars);
    *(OvlQuad *)target_position_copy = *(OvlQuad *)target_position;
    approach_angle((float *)&MOBY(moby)->rot.z,
                   (float *)(data + 0x100),
                   angle,
                   frame_time_sq * 12.566371f,
                   frame_time_sq * 12.566371f,
                   D_0015ED6C * 25.132742f);
    speed = 0.0f;
    fast_vec_sub(movement_delta, target_position_copy, moby + 0x10);
    distance_to_target = fast_vec_length(movement_delta);
    advance_accelerated_scalar(distance_to_target,
                               frame_time_sq * 12.0f,
                               frame_time_sq * 12.0f,
                               D_0015ED6C * 6.0f,
                               &speed,
                               (float *)(data + 0xFC));
    fast_vec_normalize(movement_delta, movement_delta, *(float *)(data + 0xFC));
    fast_vec_add(moby + 0x10, moby + 0x10, movement_delta);
    if (distance_to_target < 0.05f) {
        if (*(float *)(data + 0xFC) < 0.005f)
            return 1;
        else
            return 0;
    }
    return 0;
}

extern char *D_L16_0015FFD8;
extern void  add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");

#include "rnc/overlay/l16/mobys/test_dummy_globals.h"
/* spins the level's rotating parts by the current angle and queues the draw callback */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0238), where it is exact; names translated to the US level program. */

extern int *D_L16_001ABCC0_c2[] __asm__("D_L16_001ABCC0");
extern void draw_test_dummy_group_glows() __asm__("FUN_L16_002cef60");

void update_test_dummy_group_glow_phases(struct Moby *moby) __asm__("FUN_L16_002cee70");

void update_test_dummy_group_glow_phases(struct Moby *moby) {
    short *group_entry;
    float  angle_increment;
    char  *group_moby;
    char  *data;
    if (moby->group != 0xFF) {
        angle_increment = test_dummy_glow_phase_rate_degrees * DEG_TO_RAD * D_0015ED6C;
        group_entry = (short *)D_L16_001ABCC0_c2[moby->group];
        do {
        again:
            group_moby =
                (char *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) + (int)D_L16_0015FFD8);
            if (MOBY(group_moby)->oclass != 0x21D)
                goto again;
            data = ((char *)MOBY(group_moby)->pvars);
            *(int *)(data + 0x118) = D_L16_0015F5CC;
            *(float *)(data + 0x11C) =
                fast_add_rotations(*(float *)(data + 0x11C), angle_increment);
        } while (*group_entry++ >= 0);
        add_draw_callback(draw_test_dummy_group_glows, moby);
    }
}

extern char D_L16_001671C0[];
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
/* Draw callback queued by FUN_L16_002cee70: one camera-facing glow quad on each live part (class 541) of the group, its colour pulsing with the part's spin angle. */

typedef struct {
    float v[4][4];
    int   rgba[4];
    struct {
        float x, y;
    } uv[4];
    long z70;
    long tex;
    long q80;
    long alpha;
} GlowQuad;

float D_L16_001D3130[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

extern int  test_dummy_glow_pulse_color_start_gp __asm__("D_L16_001619E0") __attribute__((sda));
extern int  test_dummy_glow_pulse_color_end_gp __asm__("D_L16_001619E4") __attribute__((sda));
extern void draw_geometry_quad_alt(void *, void *, int) __asm__("FUN_001f7d30");

void draw_test_dummy_group_glows(char *moby) {
    GlowQuad glow_quad;
    float    glow_facing_transform[4][4] __attribute__((aligned(16)));
    short   *group_entry;
    char    *group_moby;
    int      color;
    int      i;

    glow_quad.tex = get_effect_texture(0xB);
    glow_quad.alpha = 0x8000000048L;
    glow_quad.q80 = 0xFF9000000260L;
    glow_quad.z70 = 5;
    glow_quad.uv[0].x = 0.0f;
    glow_quad.uv[0].y = 0.0f;
    glow_quad.uv[1].x = 0.0f;
    glow_quad.uv[1].y = 1.0f;
    glow_quad.uv[2].x = 1.0f;
    glow_quad.uv[2].y = 0.0f;
    glow_quad.uv[3].x = 1.0f;
    glow_quad.uv[3].y = 1.0f;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(glow_quad.v[i], D_L16_001D3130[i], 0.2f);
    }
    group_entry = (short *)D_L16_001ABCC0_c2[((unsigned char *)moby)[0x21]];
    for (;; group_entry++) {
        group_moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        color = fast_tween_color(
            test_dummy_glow_pulse_color_start_gp,
            test_dummy_glow_pulse_color_end_gp,
            (fast_sin(*(float *)(((char *)MOBY(group_moby)->pvars) + 0x11C)) + 1.0f) * 0.5f);
        glow_quad.rgba[3] = color;
        glow_quad.rgba[2] = color;
        glow_quad.rgba[1] = color;
        glow_quad.rgba[0] = color;
        if (group_moby != 0 && MOBY(group_moby)->oclass == 0x21D &&
            (unsigned char)group_moby[0x20] != 0xFE && (unsigned char)group_moby[0x20] != 0xFD) {
            /* basis facing the camera, origin just above the part */
            qcopy(glow_facing_transform[3], group_moby + 0x10);
            glow_facing_transform[3][2] += 0.2f;
            glow_facing_transform[3][3] = 1.0f;
            fast_vec_sub(glow_facing_transform[0], D_L16_001671C0, glow_facing_transform[3]);
            fast_vec_normalize(glow_facing_transform[0], glow_facing_transform[0], 1.0f);
            fast_vec_cross(
                glow_facing_transform[1], glow_facing_transform[0], (u8 *)&hero.gravity_dir);
            fast_vec_normalize(glow_facing_transform[1], glow_facing_transform[1], -1.0f);
            fast_vec_cross(
                glow_facing_transform[2], glow_facing_transform[1], glow_facing_transform[0]);
            draw_geometry_quad_alt(&glow_quad, glow_facing_transform, 0);
        }
        /* a negative entry ends the group */
        if (*group_entry < 0)
            return;
    }
}
