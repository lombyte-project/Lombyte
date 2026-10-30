/* Level 16 test_dummy_on_hoverboard: path following and jumps. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

#include "qcopy.h"

/* Copy the selected 16-byte vendor vector. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0D98), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x2C0];
    char *vectors;
} Level16VendorVectorData_u;

typedef struct {
    char                       pad0[0x78];
    Level16VendorVectorData_u *data;
} Level16VendorVectorMoby_u;

void copy_hoverboard_dummy_path_node(Level16VendorVectorMoby_u *moby,
                                     int                        index,
                                     void                      *out) __asm__("FUN_L16_002cf9d0");

void copy_hoverboard_dummy_path_node(Level16VendorVectorMoby_u *moby, int index, void *out) {
    qcopy(out, moby->data->vectors + index * 16 + 16);
}

s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D1310), where it is exact; names translated to the US level program. */

extern int offset_path_index(char *, int, int, int) __asm__("FUN_L00_0025d7a0");

extern char *D_L16_001B0930[];
extern int   truncate_float_to_s32(float);

/* Choose the path node to steer for: step on while the current one is behind the moby or reached. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0DC0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x2C0];
    char *vectors;
} Level16VendorVectorData_c;

typedef struct {
    char                       pad0[0x78];
    Level16VendorVectorData_c *data;
} Level16VendorVectorMoby_c;

typedef struct {
    int   count; /* 0x00: number of nodes */
    char  pad04[0xC];
    float points[1][4]; /* 0x10: position; w marks the node */
} L16PatrolPath;

typedef struct {
    char           pad0[0x260];
    float          position[4]; /* 0x260 */
    char           pad270[0x20];
    int            base_path;      /* 0x290: index into D_L16_001B0C30 */
    int            branch_path[7]; /* 0x294: indexed by a node's mark - 11 */
    char           pad2B0[0x10];
    L16PatrolPath *path; /* 0x2C0: the path being followed */
    char           pad2C4[0x30];
    short          node; /* 0x2F4: the node being steered for */
} L16PatrolData;

typedef struct {
    char           pad0[0x10];
    float          position[4]; /* 0x10 */
    char           pad20[0x28];
    float          yaw; /* 0x48 */
    char           pad4C[0x2C];
    L16PatrolData *data; /* 0x78 */
} L16PatrolMoby;

/* Patrol step: advances the moby's path node until the next node is ahead and far enough, switching between the base path and its branches. */
void advance_hoverboard_dummy_path_node(char *patrol_moby) __asm__("FUN_L16_002cf9f8");

void advance_hoverboard_dummy_path_node(char *patrol_moby) {
    L16PatrolMoby *moby = (L16PatrolMoby *)patrol_moby;
    L16PatrolData *patrol_data = moby->data;
    float          target[4];
    float          heading, distance;

    copy_hoverboard_dummy_path_node((Level16VendorVectorMoby_c *)moby, patrol_data->node, target);
    heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    for (;;) {
        L16PatrolPath *path;
        L16PatrolPath *base;

        copy_hoverboard_dummy_path_node(
            (Level16VendorVectorMoby_c *)moby, patrol_data->node, target);
        distance = vector_distance_xy(target, patrol_data->position);
        if ((fast_difference_between_rotations(heading, moby->yaw) < 1.5707964f ||
             distance > 4.0f) &&
            distance > 2.0f) {
            break;
        }
        path = patrol_data->path;
        patrol_data->node = (patrol_data->node + 1) % path->count;
        base = (L16PatrolPath *)D_L16_001B0930[patrol_data->base_path];
        if (path != base) {
            /* At the last node of a branch: back to the base path, five nodes past its nearest one. */
            if (patrol_data->node == path->count - 1) {
                patrol_data->path = base;
                patrol_data->node =
                    select_point_index_by_distance(patrol_data->position, base, 0.0f);
                patrol_data->node =
                    offset_path_index((char *)patrol_data->path, patrol_data->node, 5, 1);
            }
        } else if (path->points[patrol_data->node][3] > 10.0f && random_integer_below(100) >= 31) {
            /* At a marked node of the base path: more often than not, onto the branch the mark names. */
            int branch =
                truncate_float_to_s32(patrol_data->path->points[patrol_data->node][3]) - 11;

            patrol_data->path = (L16PatrolPath *)D_L16_001B0930[patrol_data->branch_path[branch]];
            patrol_data->node = 0;
        }
        /* The same two statements as before the loop, written out again: the compiler merges the copies. */
        copy_hoverboard_dummy_path_node(
            (Level16VendorVectorMoby_c *)moby, patrol_data->node, target);
        heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    }
}

/* Once the race has run for 180 frames, adds boost time when the dummy passes through a hoverboard_ring (0x473)
   or over a class 0x474 moby of its hoverboard's groups; same routine as FUN_L05_00307010 with this level's
   data offsets. */
void collect_hoverboard_dummy_boosts(struct Moby *moby) __asm__("FUN_L16_002cfc00");

void collect_hoverboard_dummy_boosts(struct Moby *moby) {
    char        *data = (char *)moby->pvars;
    struct Hero *hero_state = &hero;
    float        displacement_vector[4];
    char        *enumerated_moby;
    char        *group_candidate_moby;
    char        *other;
    char        *od;

    if (hero_state->race_time_frames < scale_game_frames(0xB4)) {
        return;
    }
    if (!fast_dec_timer((int *)(data + 0x2DC))) {
        return;
    }
    other = ((char *)hero_state->hoverboard_moby);
    if (!other) {
        return;
    }
    od = *(char **)(other + 0x78);
    if (*(int *)(od + 0x44) != -1) {
        get_first_moby_in_group((int *)&enumerated_moby, *(int *)(od + 0x44), 0, 0);
        while (enumerated_moby) {
            if (MOBY(enumerated_moby)->oclass == 0x473) {
                float adjusted_position[4];
                float m[16];
                char *candidate_moby = enumerated_moby;
                qcopy(adjusted_position, data + 0x260);
                adjusted_position[2] += 0.8f;
                fast_vec_sub(displacement_vector, candidate_moby + 0x10, adjusted_position);
                if (!(8.0f < fast_vec_length(displacement_vector))) {
                    set_identity_matrix(m);
                    transpose_rotation_matrix(m, enumerated_moby + 0xC0);
                    transform_vector(displacement_vector, displacement_vector, m);
                    if (absolute_float(displacement_vector[0]) < 0.5f &&
                        absolute_float(displacement_vector[1]) < 2.5f &&
                        absolute_float(displacement_vector[2]) < 2.5f) {
                        *(unsigned short *)(data + 0x2F6) += scale_game_frames(0x78);
                        *(int *)(data + 0x2DC) = scale_game_frames(0x3C);
                    }
                }
            }
            get_next_moby_in_group((int *)&enumerated_moby, enumerated_moby, 0, 0);
        }
    }
    if (*(int *)(od + 0x48) != -1) {
        get_first_moby_in_group((int *)&group_candidate_moby, *(int *)(od + 0x48), 0, 0);
        while (group_candidate_moby) {
            if (MOBY(group_candidate_moby)->oclass == 0x474) {
                float m[16];
                fast_vec_sub(displacement_vector, group_candidate_moby + 0x10, data + 0x260);
                if (!(8.0f < fast_vec_length(displacement_vector))) {
                    set_identity_matrix(m);
                    transpose_rotation_matrix(m, group_candidate_moby + 0xC0);
                    transform_vector(displacement_vector, displacement_vector, m);
                    if (absolute_float(displacement_vector[0]) < 2.8f &&
                        absolute_float(displacement_vector[1]) < 1.4f &&
                        absolute_float(displacement_vector[2]) < 0.5f) {
                        *(unsigned short *)(data + 0x2F6) += scale_game_frames(0x78);
                        *(int *)(data + 0x2DC) = scale_game_frames(0x3C);
                    }
                }
            }
            get_next_moby_in_group((int *)&group_candidate_moby, group_candidate_moby, 0, 0);
        }
    }
}

void prepare_hoverboard_dummy_path_jump(struct Moby *moby) __asm__("FUN_L16_002cff48");

void prepare_hoverboard_dummy_path_jump(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (*(float *)(data + 0x2C8) <= 0.0f) {
        int   path_node_index = *(short *)(data + 0x2F4);
        char *tab = *(char **)(data + 0x2C0);
        int   path_node_byte_offset = path_node_index * 16;
        if (*(float *)(tab + path_node_byte_offset + 0x1C) == 1.0f) {
            while (1) {
                char *t;
                if (*(float *)(tab + path_node_byte_offset + 0x1C) == 2.0f)
                    break;
                path_node_index = offset_path_index(tab, path_node_index, 1, 1);
                tab = *(char **)(data + 0x2C0);
                path_node_byte_offset = path_node_index * 16;
            }
            *(short *)(data + 0x2F0) = path_node_index;
            qcopy(data + 0x270, *(char **)(data + 0x2C0) + (short)path_node_index * 16 + 0x10);
            *(int *)(data + 0x2E4) = 1;
        }
    }
}

extern float D_0015ED6C;
extern float D_0015ED6C, frame_time;
extern float frame_time_sq;
extern float
spring_angle(void *, void *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");

extern float frame_time_sq;
extern int   scale_game_frames(int);
/* Adjust animation and pitch to the remaining time in a jump. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D1420), where it is exact; names translated to the US level program. */

typedef struct {
    char  header[0x10];
    float points[1][4];
} L16LeapPath;

typedef struct {
    char  pad0[0x268];
    float height;
    char  pad26C[0xC];
    float target_height;
    char  pad27C[0x44];
    char *path;
    char  pad2C4[4];
    float vertical_speed;
    char  pad2CC[0x18];
    int   timer;
    char  pad2E8[0xC];
    short node;
    char  pad2F6[2];
    float pitch_speed;
    char  pad2FC[2];
    short sequence;
} L16LeapData_u;

typedef struct {
    char           pad0[0x44];
    float          pitch;
    char           pad48[0xA];
    unsigned char  animation, next_animation;
    char           pad54[4];
    float          animation_speed;
    char           pad5C[0x14];
    unsigned char  flags;
    char           pad71[7];
    L16LeapData_u *data;
} L16LeapMoby_u;

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");

#include "rnc/game/interpolated_record.h"

extern void func_L00_00237B70_anim(void *, int, int, int) __asm__("FUN_L00_002371e0");

void update_hoverboard_dummy_jump_animation_and_pitch(L16LeapMoby_u *leap_moby) __asm__("FUN_L16_002d0058");

void update_hoverboard_dummy_jump_animation_and_pitch(L16LeapMoby_u *leap_moby) {
    float          root0, root1;
    float          pitch;
    L16LeapData_u *leap_data = leap_moby->data;
    pitch = 0.0f;
    if (leap_data->timer == 0) {
        int   next;
        float distance;
        next = offset_path_index(leap_data->path, leap_data->node, 1, 1) * 16;
        distance = vector_distance_xy(leap_data->path + (leap_data->node * 16 + 0x10),
                                      leap_data->path + (next + 0x10));
        pitch = fast_arc_tan(distance,
                             *(float *)(leap_data->path + next + 0x18) -
                                 ((L16LeapPath *)leap_data->path)->points[leap_data->node][2]);
    }
    spring_angle(&leap_moby->pitch,
                 &leap_data->pitch_speed,
                 0,
                 pitch,
                 frame_scale_sq * 0.02f,
                 frame_scale_sq * 0.3f,
                 D_0015ED6C * 3.14159265f);
    if (leap_data->timer != 0) {
        float duration;
        float acceleration;
        if (leap_moby->next_animation != 5 && scale_game_frames(3) > leap_data->timer) {
            func_L00_00237B70_anim(leap_moby, 5, 4, scale_game_frames(5));
            leap_data->sequence = -1;
        }
        acceleration = frame_time_sq * 21.0f * -0.5f;
        duration = 30.0f;
        if (solve_quadratic_equation(&root0,
                                     &root1,
                                     acceleration,
                                     leap_data->vertical_speed - acceleration,
                                     leap_data->height - leap_data->target_height) > 0 &&
            root0 > 0.0f) {
            duration = truncate_float_to_s32(root0);
        }
        if (scale_game_frames(7) < leap_data->timer && scale_game_frames(11) > leap_data->timer &&
            leap_moby->next_animation == 5 && (float)scale_game_frames(27) < duration) {
            short sequence;
            int   chosen;
            chosen = random_int_between(0, 3) + 1;
            leap_data->sequence = chosen;
            sequence = chosen;
            func_L00_00237B70_anim(leap_moby, sequence, 3, scale_game_frames(7));
        }
        if (leap_data->sequence != -1) {
            if (leap_moby->animation == leap_moby->next_animation) {
                float frame = compute_interpolated_record_value(leap_moby);
                if (frame > 7.0f && frame < 20.0f && duration != 0.0f) {
                    float speed = 30.0f / duration;
                    if (speed > 1.7f)
                        speed = 1.7f;
                    if (speed < 0.3f)
                        speed = 0.3f;
                    leap_moby->animation_speed = speed;
                }
            }
            if (leap_data->sequence != -1) {
                if (duration < (float)scale_game_frames(5)) {
                    func_L00_00237B70_anim(leap_moby, 5, 0x33, scale_game_frames(5));
                    leap_data->sequence = -1;
                } else if (leap_moby->flags & 2) {
                    func_L00_00237B70_anim(leap_moby, 5, 0x1C, scale_game_frames(7));
                    leap_data->sequence = -1;
                }
            }
        }
        if (leap_moby->animation == leap_moby->next_animation && leap_moby->animation == 5 &&
            duration != 0.0f) {
            float remaining;
            remaining = 50.0f;
            remaining -= compute_interpolated_record_value(leap_moby);
            remaining /= duration;
            leap_moby->animation_speed = remaining;
        }
    } else {
        leap_moby->animation_speed = 1.0f;
        if ((leap_moby->flags & 2) && leap_moby->next_animation != 0) {
            func_L00_00237B70_anim(leap_moby, 0, 0, scale_game_frames(8));
        }
    }
}

f32        probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
extern int D_L16_0015F5CC;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

#include "sda.h"

extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void  spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
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
                             int,
                             float,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern float probe_ground_height(void *, int, float);

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");

extern float fast_add_rotations(float, float);
extern float fast_sin(float);

/* Update the hoverboard race competitor: wait for the race, follow the path, leap, and emit spray. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D1868), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x2C0];
    char *vectors;
} Level16VendorVectorData;

typedef struct {
    char                     pad0[0x78];
    Level16VendorVectorData *data;
} Level16VendorVectorMoby;

typedef struct {
    char  pad0[0x268];
    float height;
    char  pad26C[0xC];
    float target_height;
    char  pad27C[0x44];
    char *path;
    char  pad2C4[4];
    float vertical_speed;
    char  pad2CC[0x18];
    int   timer;
    char  pad2E8[0xC];
    short node;
    char  pad2F6[2];
    float pitch_speed;
    char  pad2FC[2];
    short sequence;
} L16LeapData;

typedef struct {
    char          pad0[0x44];
    float         pitch;
    char          pad48[0xA];
    unsigned char animation, next_animation;
    char          pad54[4];
    float         animation_speed;
    char          pad5C[0x14];
    unsigned char flags;
    char          pad71[7];
    L16LeapData  *data;
} L16LeapMoby;

typedef struct {
    char  pad0[0x140];
    float position[4];
    char  pad150[8];
    float yaw;
} L16RacePlayer;

extern L16RacePlayer D_L16_0015F580 MACRO_ADDR;
extern int                         *D_L16_00178300[];
extern float                        approach_value(float *p, float target, float maxstep);
extern int                          D_L16_00161A0C __attribute__((sda));
extern int                          D_0015EE3C_gp __asm__("D_0015EE3C") __attribute__((sda));
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int is_moby_in_class_range_500_to_540(int) __asm__("FUN_L00_0025e3b8");
extern unsigned char D_0013D4DF NOT_SDA;
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
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
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
void        advance_hoverboard_dummy_path_node(char *m) __asm__("FUN_L16_002cf9f8");
void        FUN_L16_002cf9d0_c(Level16VendorVectorMoby *moby,
                               int                      index,
                               void                    *out) __asm__("FUN_L16_002cf9d0");
void        FUN_L16_002d0058_c(L16LeapMoby *m) __asm__("FUN_L16_002d0058");

#include "rnc/overlay/l16/mobys/test_dummy_on_hoverboard_globals.h"

extern int hoverboard_dummy_spray_initial_color_gp __asm__("D_L16_00161A08") __attribute__((sda));
void       test_dummy_on_hoverboard_update(unsigned char *m) __asm__("FUN_L16_002d04a0");

void test_dummy_on_hoverboard_update(unsigned char *moby) {
    enum {
        HOVERBOARD_DUMMY_STATE_INITIALIZE = 0,
        HOVERBOARD_DUMMY_STATE_WAIT_FOR_RACE = 1,
        HOVERBOARD_DUMMY_STATE_RACING = 2,
        HOVERBOARD_DUMMY_STATE_WAIT_FOR_REACTIVATION = 3
    };
    float path_node_or_explosion_position[4], delta[4], ahead[4], impact[4], spray[4], lateral[4],
        dust[4];
    char *hoverboard_dummy_data = ((char *)MOBY(moby)->pvars);
    char *hero_base;
    int   i, nearby_moby_count, next;
    /* One mask for both collision calls. Held in a local it is one more instruction when GCSE sizes its
       expression table, and that size orders the stack slots of the cached addresses (sp+0x90..0xC4):
       with the literal at both calls every one of those slots is rotated by three. */
    int   mask = 0x30000;
    float distance, duration, target, z, gap, speed, angle, scale, rate, limit, offset, slow;

    /* No pointer locals: every address is written where it is used. The compiler caches them itself
       (d + 0x1E0 from here on, the others at the top of the active state). */
    manipulator_set_scale(hoverboard_dummy_data + 0x1E0, 2.5f);
    switch (moby[0x20]) {
    case HOVERBOARD_DUMMY_STATE_INITIALIZE: {
        float time;
        char *path;

        moby[0x30] = 24;
        MOBY(moby)->flags |= 0x41;
        path = D_L16_001B0930[*(int *)(hoverboard_dummy_data + 0x2B0)];
        *(char **)(hoverboard_dummy_data + 0x2C0) = path;
        FUN_L16_002cf9d0_c((Level16VendorVectorMoby *)moby, 0, path_node_or_explosion_position);
        qcopy(moby + 0x10, path_node_or_explosion_position);
        qcopy(hoverboard_dummy_data + 0x260, moby + 0x10);
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        MOBY(moby)->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                         *(float *)(path + 0x24) - *(float *)(path + 0x14));
        *(unsigned char *)(hoverboard_dummy_data + 0x2B) = 1;
        time = D_0015ED6C;
        offset = (float)*(int *)(hoverboard_dummy_data + 0x2CC) * (time * 1.55f);
        *(float *)(hoverboard_dummy_data + 0x2B4) = time * 17.0f + offset;
        *(float *)(hoverboard_dummy_data + 0x2B8) = time * 24.0f + offset;
        if (D_0013D4DF == 0) {
            slow = (float)(D_0015EE3C_gp - 5);
            if (slow < 0.0f) {
                slow = 0.0f;
            }
            slow = 1.0f - slow * 0.008f;
            if (slow < 0.8f) {
                slow = 0.8f;
            }
            *(float *)(hoverboard_dummy_data + 0x2B4) *= slow;
            *(float *)(hoverboard_dummy_data + 0x2B8) *= slow;
        }
        *(float *)(hoverboard_dummy_data + 0x2E8) = random_angle_radians();
        *(unsigned char *)(hoverboard_dummy_data + 0x29) = 0;
        moby[0x20] = 1;
        break;
    }
    case HOVERBOARD_DUMMY_STATE_WAIT_FOR_RACE:
        if (hero.state.control_mode == 22) {
            moby[0x30] = 255;
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            MOBY(moby)->flags &= ~0x41;
            *(short *)(hoverboard_dummy_data + 0x2F4) = 0;
            *(short *)(hoverboard_dummy_data + 0x2F6) = scale_game_frames(180);
            *(int *)(hoverboard_dummy_data + 0x2C8) = 0;
            *(int *)(hoverboard_dummy_data + 0x2C4) = 0;
            *(int *)(hoverboard_dummy_data + 0x2E4) = 0;
            *(int *)(hoverboard_dummy_data + 0x2DC) = 0;
            *(int *)(hoverboard_dummy_data + 0x2BC) = 0;
            if (*(int *)(hoverboard_dummy_data + 0x2CC) % 2 == 1) {
                MOBY(moby)->flags |= 0x8000;
            }
            moby[0xA4] = 255;
            moby[0x20] = 2;
        }
        break;
    case HOVERBOARD_DUMMY_STATE_WAIT_FOR_REACTIVATION:
        if (hero.state.control_mode != 22) {
            moby[0x20] = 0;
        } else {
            int   count = *(unsigned short *)(hoverboard_dummy_data + 0x2F2) + 1;
            short timer = count;

            *(short *)(hoverboard_dummy_data + 0x2F2) = count;
            if (scale_game_frames(240) < timer) {
                LevelCamera *player = &l16_camera;

                if (fast_difference_between_rotations(
                        fast_arc_tan(MOBY(moby)->pos.x - player->pos.f[0],
                                     MOBY(moby)->pos.y - player->pos.f[1]),
                        player->yaw) > 1.5707964f) {
                    MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
                    MOBY(moby)->flags &= ~0x41;
                    moby[0xA4] = 255;
                    moby[0x20] = 2;
                }
            }
        }
        break;
    case HOVERBOARD_DUMMY_STATE_RACING:
        hero_base = ((char *)&hero);
        if (*(int *)(hero_base + 0x208C) != 22) {
            moby[0x20] = 0;
            break;
        }
        if (find_moby_hit(moby, mask, 0)) {
            qcopy(path_node_or_explosion_position, moby + 0x10);
            path_node_or_explosion_position[2] += 0.5f;
            spawn_explosion(moby,
                            &D_L16_0015F580,
                            path_node_or_explosion_position,
                            0.0f,
                            0.0f,
                            5,
                            15,
                            25,
                            2.5f,
                            1.5f,
                            4.0f,
                            -1,
                            1.0f,
                            7.0f,
                            0,
                            9,
                            -1,
                            0);
            spawn_moby_break_fragments(moby, 0x655, 1, 0x656, 2, 2, 0);
            spawn_moby_break_fragments(moby, 0x6FA, 1, 0x6FB, 1, 1, 0);
            *(short *)(hoverboard_dummy_data + 0x2F2) = 0;
            *(int *)(hero_base + 0x8A8) += 250;
            moby[0x20] = 3;
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags |= 0x41;
            break;
        }
        moby[0xA4] = 255;
        advance_hoverboard_dummy_path_node((char *)moby);
        collect_hoverboard_dummy_boosts((struct Moby *)moby);
        prepare_hoverboard_dummy_path_jump((char *)moby);
        if (*(int *)(hoverboard_dummy_data + 0x2E4) != 0) {
            (*(int *)(hoverboard_dummy_data + 0x2E4))++;
            if (*(int *)(hoverboard_dummy_data + 0x2E4) < scale_game_frames(17)) {
                i = *(short *)(hoverboard_dummy_data + 0x2F4);
                distance = 0.0f;
                while (i < *(short *)(hoverboard_dummy_data + 0x2F0)) {
                    char *path;

                    next = offset_path_index(*(char **)(hoverboard_dummy_data + 0x2C0), i, 1, 1);
                    path = *(char **)(hoverboard_dummy_data + 0x2C0);
                    distance += vector_distance_xy(path + (i * 16 + 16), path + (next * 16 + 16));
                    i = next;
                }
                {
                    short last = *(short *)(hoverboard_dummy_data + 0x2F0);
                    short first = *(short *)(hoverboard_dummy_data + 0x2F4);
                    char *path = *(char **)(hoverboard_dummy_data + 0x2C0);
                    float rise = *(float *)(path - (-(last * 16)) + 0x18) -
                                 *(float *)(path - (-(first * 16)) + 0x18);

                    duration = distance / *(float *)(hoverboard_dummy_data + 0x2BC);
                    target = rise / duration + frame_time_sq * 21.0f * duration * 0.5f;
                }
                if (*(float *)(hoverboard_dummy_data + 0x2C8) < 0.0f) {
                    *(float *)(hoverboard_dummy_data + 0x2C8) = 0.0f;
                }
                approach_value(
                    (float *)(hoverboard_dummy_data + 0x2C8), target, frame_time_sq * 110.0f);
            }
        }
        FUN_L16_002d0058_c((L16LeapMoby *)moby);
        FUN_L16_002cf9d0_c((Level16VendorVectorMoby *)moby,
                           *(short *)(hoverboard_dummy_data + 0x2F4),
                           path_node_or_explosion_position);
        angle = fast_arc_tan(path_node_or_explosion_position[0] - MOBY(moby)->pos.x,
                             path_node_or_explosion_position[1] - MOBY(moby)->pos.y);
        spring_angle(moby + 0x48,
                     hoverboard_dummy_data + 0x2C4,
                     0,
                     angle,
                     frame_scale_sq * 0.035f,
                     frame_scale_sq * 0.3f,
                     D_0015ED6C * 6.981317f);
        if (*(float *)(hoverboard_dummy_data + 0x268) < path_node_or_explosion_position[2] - 5.0f) {
            *(float *)(hoverboard_dummy_data + 0x268) = path_node_or_explosion_position[2];
        }
        delta[0] = fast_cos(MOBY(moby)->rot.z) * *(float *)(hoverboard_dummy_data + 0x2BC);
        delta[1] = fast_sin(MOBY(moby)->rot.z) * *(float *)(hoverboard_dummy_data + 0x2BC);
        delta[2] = 0.0f;
        fast_vec_add(hoverboard_dummy_data + 0x260, hoverboard_dummy_data + 0x260, delta);
        fast_vec_scale(ahead, delta, 3.0f);
        fast_vec_add(ahead, ahead, hoverboard_dummy_data + 0x260);
        ahead[2] += 0.7f;
        nearby_moby_count = collect_mobies_in_sphere(0.7f, ahead, 0, moby, 0);
        if (nearby_moby_count) {
            fast_vec_scale(impact, delta, 7.0f);
            for (i = 0; i < nearby_moby_count; i++) {
                if (is_moby_in_class_range_500_to_540((int)D_L16_00178300[i])) {
                    spawn_nearby_effect(D_L16_00178300[i],
                                        (int)moby,
                                        mask,
                                        hoverboard_dummy_data + 0x260,
                                        impact,
                                        1.0f);
                }
            }
        }
        z = probe_ground_height(hoverboard_dummy_data + 0x260, 0, 0.5f);
        gap = *(float *)(hoverboard_dummy_data + 0x268) - z;
        if (gap > 0.0f || *(float *)(hoverboard_dummy_data + 0x2C8) > 0.0f) {
            *(float *)(hoverboard_dummy_data + 0x2C8) -= frame_time_sq * 21.0f;
        } else if (absolute_float(gap) < 0.5f) {
            *(float *)(hoverboard_dummy_data + 0x2C8) = 0.0f;
            *(float *)(hoverboard_dummy_data + 0x268) = z;
            *(int *)(hoverboard_dummy_data + 0x2E4) = 0;
        }
        *(float *)(hoverboard_dummy_data + 0x268) += *(float *)(hoverboard_dummy_data + 0x2C8);
        if (fast_dec_timer_s16(hoverboard_dummy_data + 0x2F6)) {
            speed = *(float *)(hoverboard_dummy_data + 0x2B4);
            if (z < *(float *)(hoverboard_dummy_data + 0x268)) {
                speed = D_0015ED6C * 19.0f;
            }
            hero_base = ((char *)&hero);
            if (*(int *)(hero_base + 0x894) < scale_game_frames(700)) {
                speed += D_0015ED6C * 3.0f;
            }
            if (*(short *)(hero_base - (-(*(int *)(hoverboard_dummy_data + 0x2CC) * 2)) + 0x7C0) <
                *(int *)(hero_base + 0x8C0)) {
                speed -= D_0015ED6C;
            } else {
                speed += D_0015ED6C;
            }
            if (speed < *(float *)(hoverboard_dummy_data + 0x2BC)) {
                approach_value(
                    (float *)(hoverboard_dummy_data + 0x2BC), speed, frame_time_sq * 7.0f);
            } else {
                approach_value(
                    (float *)(hoverboard_dummy_data + 0x2BC), speed, frame_time_sq * 15.0f);
            }
        } else {
            approach_value((float *)(hoverboard_dummy_data + 0x2BC),
                           *(float *)(hoverboard_dummy_data + 0x2B8),
                           frame_time_sq * 20.0f);
        }
        if (gap < 0.2f && *(float *)(hoverboard_dummy_data + 0x2C8) < 0.0f) {
            *(float *)(hoverboard_dummy_data + 0x268) = z;
            *(int *)(hoverboard_dummy_data + 0x2E4) = 0;
        }
        rate = *(float *)(hoverboard_dummy_data + 0x2C4);
        limit = D_0015ED6C * 2.6179938f;
        if (limit < rate) {
            rate = limit;
        } else if (rate < -limit) {
            rate = -limit;
        }
        if (MOBY(moby)->flags & 0x8000) {
            rate = -rate;
        }
        {
            float coefficient = frame_scale_sq;

            *(float *)(hoverboard_dummy_data + 0x248) = rate * 30.0f;
            *(float *)(hoverboard_dummy_data + 0xC8) = rate * 20.0f;
            *(float *)(hoverboard_dummy_data + 0x1C8) = rate * 50.0f;
            *(float *)(hoverboard_dummy_data + 0x144) = rate * 40.0f;
            update_joint_animation(
                coefficient * 0.02f, coefficient * 0.3f, moby, hoverboard_dummy_data + 0x1E0, 3);
        }
        update_joint_animation(
            frame_scale_sq * 0.02f, frame_scale_sq * 0.3f, moby, hoverboard_dummy_data + 0x60, 0);
        update_joint_animation(
            frame_scale_sq * 0.02f, frame_scale_sq * 0.3f, moby, hoverboard_dummy_data + 0x160, 1);
        update_joint_animation(
            frame_scale_sq * 0.015f, frame_scale_sq * 0.3f, moby, hoverboard_dummy_data + 0xE0, 2);
        qcopy(moby + 0x10, hoverboard_dummy_data + 0x260);
        {
            float bob_time = D_0015ED6C;

            MOBY(moby)->pos.z += 0.25f;
            *(float *)(hoverboard_dummy_data + 0x2E8) =
                fast_add_rotations(*(float *)(hoverboard_dummy_data + 0x2E8), bob_time * 6.108652f);
        }
        MOBY(moby)->pos.z += fast_sin(*(float *)(hoverboard_dummy_data + 0x2E8)) * 0.08f;
        fast_vec_sub(ahead, moby + 0x10, hoverboard_dummy_data + 0x280);
        qcopy(hoverboard_dummy_data + 0x280, moby + 0x10);
        moby_point_position(moby, (D_L16_0015F5CC & 1) | 4, impact);
        angle = random_angle_radians();
        scale = D_0015ED6C * 0.25f;
        fast_vec_normalize(spray, moby + 0xE0, scale * fast_cos(angle));
        fast_vec_normalize(lateral, moby + 0xD0, scale * fast_sin(angle));
        fast_vec_add(spray, spray, ahead);
        fast_vec_add(spray, spray, lateral);
        qcopy(spray, ahead);
        spray[3] = 120000.0f;
        if (*(short *)(hoverboard_dummy_data + 0x2F6) == 0) {
            int ticks = random_int_between(scale_game_frames(10), scale_game_frames(13));

            spawn_effect(impact,
                         spray,
                         hoverboard_dummy_spray_initial_color_gp,
                         D_L16_00161A0C,
                         ticks,
                         15,
                         15,
                         1);
        } else {
            int ticks;

            fast_vec_scale(dust, ahead, 0.92f);
            ticks = random_int_between(scale_game_frames(15), scale_game_frames(18));
            spawn_effect(impact, dust, 0x5032F0D2, D_L16_00161A0C, ticks, 30, 20, 1);
            ticks = random_int_between(scale_game_frames(8), scale_game_frames(17));
            spawn_burst(impact,
                        spray,
                        hoverboard_dummy_spray_initial_color_gp,
                        D_L16_00161A0C,
                        ticks,
                        1,
                        15000.0f);
        }
        break;
    }
}
