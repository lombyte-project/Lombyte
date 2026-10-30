/* Level 05 moby class skid_mcmarx: the hoverboard racer, its path and jump animations. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "qcopy.h"

extern void *create_moby() __asm__("FUN_0020c4f8");

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002F9B10), where it is exact; names translated to the US level program. */

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

char *spawn_racer_child(char *owner) __asm__("FUN_L05_002f8718");

char *spawn_racer_child(char *owner) {
    char *moby = create_moby(0x1B7);
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 2;
        qcopy(moby + 0x10, owner + 0x10);
        qcopy(moby + 0x40, owner + 0x40);
        MOBY(moby)->spawn_frame = *(unsigned long *)(owner + 0x38);
        update_entity(moby);
    }
    return moby;
}
/* Moves a point along a looped path of 16-byte nodes stored in the moby's data. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00308188), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");

void copy_racer_path_node(char *moby, int path_node_index, float *out) __asm__("FUN_L05_00306d30");

void copy_racer_path_node(char *moby, int path_node_index, float *out) {
    char *path = *(char **)(((char *)MOBY(moby)->pvars) + 0x244);
    float next_path_node[4];
    float perpendicular_heading;
    qcopy(out, path + 0x10 + path_node_index * 16);
    qcopy(next_path_node, path + 0x10 + ((path_node_index + 1) % *(int *)path) * 16);
    perpendicular_heading = fast_add_rotations(
        fast_arc_tan(next_path_node[0] - out[0], next_path_node[1] - out[1]), 1.5707964f);
    out[0] = out[0] + fast_cos(perpendicular_heading) * 0.0f;
    out[1] = out[1] + fast_sin(perpendicular_heading) * 0.0f;
}

extern int   offset_path_index(char *, int, int, int) __asm__("FUN_L00_0025d7a0");
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");

extern int select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
void       copy_racer_path_node(char *moby, int idx, float *out) __asm__("FUN_L05_00306d30");

typedef struct {
    int   count; /* 0x00: number of nodes */
    char  pad04[0xC];
    float points[1][4]; /* 0x10: position; w marks the node */
} L16PatrolPath;

typedef struct {
    char           pad0[0x200];
    float          position[4]; /* 0x200 */
    char           pad210[0x10];
    int            base_path;      /* 0x220: index into D_L05_001B0930 */
    int            branch_path[7]; /* 0x224: indexed by a node's mark - 1 */
    char           pad240[4];
    L16PatrolPath *path; /* 0x244: the path being followed */
    char           pad248[0x10];
    short          node; /* 0x258: the node being steered for */
} L05PatrolData;

typedef struct {
    char           pad0[0x10];
    float          position[4]; /* 0x10 */
    char           pad20[0x28];
    float          yaw; /* 0x48 */
    char           pad4C[0x2C];
    L05PatrolData *data; /* 0x78 */
} L16PatrolMoby;

/* Patrol step: advances the moby's path node until the next node is ahead and far enough, switching between the base path and its branches. */
/* Same routine as FUN_L16_002cf9f8, with this level's data layout. */
void advance_racer_path_node(char *patrol_moby) __asm__("FUN_L05_00306e10");

void advance_racer_path_node(char *patrol_moby) {
    L16PatrolMoby *moby = (L16PatrolMoby *)patrol_moby;
    L05PatrolData *patrol_data = moby->data;
    float          target[4];
    float          heading, distance;

    copy_racer_path_node((char *)moby, patrol_data->node, target);
    heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    for (;;) {
        L16PatrolPath *path;
        L16PatrolPath *base;

        copy_racer_path_node((char *)moby, patrol_data->node, target);
        distance = vector_distance_xy(target, patrol_data->position);
        if ((fast_difference_between_rotations(heading, moby->yaw) < 1.5707964f ||
             distance > 4.0f) &&
            distance > 2.0f) {
            break;
        }
        path = patrol_data->path;
        patrol_data->node = (patrol_data->node + 1) % path->count;
        base = (L16PatrolPath *)l05_paths[patrol_data->base_path];
        if (path != base) {
            /* At the last node of a branch: back to the base path, five nodes past its nearest one. */
            if (patrol_data->node == path->count - 1) {
                patrol_data->path = base;
                patrol_data->node =
                    select_point_index_by_distance(patrol_data->position, base, 0.0f);
                patrol_data->node =
                    offset_path_index((char *)patrol_data->path, patrol_data->node, 5, 1);
            }
        } else if (path->points[patrol_data->node][3] > 0.0f && random_integer_below(100) > 0) {
            /* At a marked node of the base path: more often than not, onto the branch the mark names. */
            int branch = truncate_float_to_s32(patrol_data->path->points[patrol_data->node][3]) - 1;

            patrol_data->path = (L16PatrolPath *)l05_paths[patrol_data->branch_path[branch]];
            patrol_data->node = 0;
        }
        /* The same two statements as before the loop, written out again: the compiler merges the copies. */
        copy_racer_path_node((char *)moby, patrol_data->node, target);
        heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    }
}
/* advances a moby's path waypoint until it reaches one flagged 2.0 */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003087B0), where it is exact; names translated to the US level program. */

void start_racer_branch_jump(char *moby) __asm__("FUN_L05_00307358");

void start_racer_branch_jump(char *moby) {
    char *moby_data = ((char *)MOBY(moby)->pvars);
    char *active_path = *(char **)(moby_data + 0x244);
    int   path_node_index;
    int   path_node_byte_offset;
    if (active_path != ((char *)l05_paths[*(int *)(moby_data + 0x220)])) {
        if (*(float *)(moby_data + 0x24C) <= 0.0f) {
            path_node_index = *(short *)(moby_data + 0x258);
            path_node_byte_offset = path_node_index * 16;
            if (*(float *)(active_path + path_node_byte_offset + 0x1C) == 1.0f) {
                while (*(float *)(active_path + path_node_byte_offset + 0x1C) != 2.0f) {
                    path_node_index = offset_path_index(active_path, path_node_index, 1, 1);
                    active_path = *(char **)(moby_data + 0x244);
                    path_node_byte_offset = path_node_index * 16;
                }
                *(int *)(moby_data + 0x27C) = path_node_index;
                qcopy(moby_data + 0x210,
                      *(char **)(moby_data + 0x244) + path_node_index * 16 + 0x10);
                *(int *)(moby_data + 0x26C) = 1;
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003088C8), where it is exact; names translated to the US level program. */

int map_racer_animation_to_child_index(int animation_index) __asm__("FUN_L05_00307470");

int map_racer_animation_to_child_index(int animation_index) {
    return animation_index == 5 ? 6 : animation_index;
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_003088D8), where it is exact; names translated to the US level program. */

extern int  map_racer_animation_to_child_index(int) __asm__("FUN_L05_00307470");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

void blend_racer_and_child_animations(char *moby,
                                      int   animation_index,
                                      int   frame_index,
                                      int   blend_frames) __asm__("FUN_L05_00307480");

void blend_racer_and_child_animations(char *moby,
                                      int   animation_index,
                                      int   frame_index,

                                      int blend_frames) {
    char *data;
    char *child;
    float ratio;
    float child_frame_index;
    float max;
    int   child_animation_index;
    blend_moby_animation(moby, animation_index, frame_index, blend_frames);
    data = ((char *)MOBY(moby)->pvars);
    child = *(char **)(data + 0x268);
    if (child != 0) {
        ratio = *(float *)(child + 0x5C) / *(float *)(moby + 0x5C);
        child_animation_index = map_racer_animation_to_child_index(animation_index);
        child_frame_index = (float)frame_index * ratio;
        max = (float)*(unsigned char *)(*(char **)(*(char **)(*(char **)(data + 0x268) + 0x24) -
                                                   (-(child_animation_index * 4)) + 0x48) +
                                        0x10);
        if (max < child_frame_index)
            child_frame_index = max;
        else if (child_frame_index < 0.0f)
            child_frame_index = 0.0f;
        blend_moby_animation(
            *(char **)(data + 0x268), child_animation_index, (int)child_frame_index, blend_frames);
    }
}

extern int          scale_game_frames(int) __asm__("FUN_001f96f8");
extern int          random_int_between(int, int) __asm__("FUN_L00_00257b90");
typedef struct Moby Moby;

/* Same routine as FUN_L16_002d0058 without the pitch step: times the leap animation to the
 * remaining jump, then copies the animation speed to the rider. */
struct LeapVars {
    u8           pad0[0x208];
    f32          height; /* 0x208 */
    u8           pad20C[0xC];
    f32          targetHeight; /* 0x218 */
    u8           pad21C[0x30];
    f32          verticalSpeed; /* 0x24C */
    u8           pad250[0x18];
    struct Moby *rider; /* 0x268 */
    s32          timer; /* 0x26C */
    u8           pad270[2];
    s16          sequence; /* 0x272 */
};

extern int
solve_quadratic_equation(float *, float *, float, float, float) __asm__("FUN_L00_002595a0");
extern float FUN_0020c9e0(struct Moby *);

void update_racer_jump_animations(struct Moby *moby) __asm__("FUN_L05_00307570");

void update_racer_jump_animations(struct Moby *moby) {
    float            root0, root1;
    struct LeapVars *leap_data = (struct LeapVars *)moby->pvars;

    if (leap_data->timer != 0) {
        float duration;
        float acceleration;
        if (moby->prev_seq != 5 && scale_game_frames(3) >= leap_data->timer) {
            blend_racer_and_child_animations(moby, 5, 1, scale_game_frames(5));
            leap_data->sequence = -1;
        }
        acceleration = frame_time_sq * 21.0f * -0.5f;
        duration = 30.0f;
        if (solve_quadratic_equation(&root0,
                                     &root1,
                                     acceleration,
                                     leap_data->verticalSpeed - acceleration,
                                     leap_data->height - leap_data->targetHeight) > 0 &&
            root0 > 0.0f) {
            duration = truncate_float_to_s32(root0);
        }
        if (scale_game_frames(7) < leap_data->timer && scale_game_frames(11) > leap_data->timer &&
            moby->prev_seq == 5 && (float)scale_game_frames(27) < duration) {
            short sequence;
            int   chosen;
            chosen = random_int_between(0, 3) + 1;
            leap_data->sequence = chosen;
            sequence = chosen;
            blend_racer_and_child_animations(moby, sequence, 1, scale_game_frames(7));
        }
        if (leap_data->sequence != -1) {
            if (moby->seq == moby->prev_seq) {
                float frame = FUN_0020c9e0(moby);
                if (frame > 7.0f && frame < 20.0f && duration != 0.0f) {
                    float speed = 30.0f / duration;
                    if (speed > 1.7f)
                        speed = 1.7f;
                    if (speed < 0.3f)
                        speed = 0.3f;
                    moby->anim_speed = speed;
                }
            }
            if (leap_data->sequence != -1) {
                if (duration < (float)scale_game_frames(9)) {
                    blend_racer_and_child_animations(moby, 5, 3, scale_game_frames(7));
                    leap_data->sequence = -1;
                } else if (moby->anim_flags & 2) {
                    blend_racer_and_child_animations(moby, 5, 2, scale_game_frames(7));
                    leap_data->sequence = -1;
                }
            }
        }
        if (moby->seq == moby->prev_seq && moby->seq == 5 && duration != 0.0f) {
            float remaining;
            remaining = 25.0f;
            remaining -= FUN_0020c9e0(moby);
            remaining /= duration * 0.5f;
            moby->anim_speed = remaining;
        }
    } else {
        moby->anim_speed = 1.0f;
        if ((moby->anim_flags & 2) && moby->prev_seq != 0) {
            blend_racer_and_child_animations(moby, 0, 0, scale_game_frames(8));
        }
    }
    if (leap_data->rider != 0)
        leap_data->rider->anim_speed = moby->anim_speed;
}

extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float fast_cos(float);
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
float        absolute_float(float input) __asm__("func_001F99C0");
extern char  D_L05_001671C0[];
extern float fast_vec_dist(void *, void *);
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void  FUN_L00_0025a120(void *);
extern void  FUN_L00_002628d8(char *, char *, int, float, float);
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern char  D_0013F350[];

/* Update function of moby class 717 (skid_mcmarx): a hoverboard racer that follows its path, jumps, and steers. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_00308D68), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) Vec4_308D68;

typedef struct {
    int         count;
    int         pad[3];
    Vec4_308D68 points[1];
} Path;

typedef struct {
    char  p00[0x64];
    float f64;
    float f68;
    char  p6C[0x14];
} Manip_308D68;

typedef struct {
    Manip_308D68 m[4];
    float        pos[4];
    float        f210[4];
    int          f220[5];
    int          path_id;
    float        cruise_speed;
    float        launch_speed;
    float        speed;
    Path        *path;
    float        yaw_speed;
    float        vertical_speed;
    int          racer_index;
    int          f254;
    short        point_index;
    short        launch_timer;
    int          f25C;
    int          f260;
    int          f264;
    Moby        *child;
    int          jump_frames;
    short        f270;
    short        f272;
    float        bob_phase;
    int          f278;
    int          jump_point;
} Data;

typedef struct {
    char p00[0x50];
    int  f50;
} ChildData;

extern float frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern float
spring_angle(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int   D_0015EE38_macro __asm__("D_0015EE38") MACRO_ADDR;
extern int   D_L05_00178300[];
extern int   is_moby_in_class_range_500_to_540(int) __asm__("FUN_L00_0025e3b8");
extern void  copy_racer_path_node(char *moby, int idx, float *out) __asm__("FUN_L05_00306d30");
extern void  add_moby_frames_for_group_proximity(char *moby) __asm__("FUN_L05_00307010");
void         advance_racer_path_node(char *obj) __asm__("FUN_L05_00306e10");
void         FUN_L05_00307570_c(struct Moby *m) __asm__("FUN_L05_00307570");
extern char  D_001413DC[];
extern int func_L00_001F2BE8_308D68(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");

void skid_mcmarx_update(Moby *moby) __asm__("FUN_L05_00307910");

void skid_mcmarx_update(Moby *moby) {
    enum {
        SKID_MCMARX_INITIALIZE = 0,
        SKID_MCMARX_WAIT_FOR_RACE_MODE = 1,
        SKID_MCMARX_FOLLOW_PATH = 2
    };
    float path_point[4];
    float step[4];
    float probe[4];
    float push[4];
    Data *data = (Data *)*(void **)&moby->pvars;

    if (moby->visible != 0 && fast_vec_dist(((float *)&moby->pos), D_L05_001671C0) < 29.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x17;
    }
    switch (moby->state) {
    case SKID_MCMARX_INITIALIZE: {
        Path *path;

        moby->update_dist = 0x18;
        moby->flags |= 0x41;
        path = (Path *)l05_paths[data->path_id];
        data->path = path;
        copy_racer_path_node((char *)moby, 0, path_point);
        qcopy(((float *)&moby->pos), path_point);
        qcopy(data->pos, ((float *)&moby->pos));
        moby->rot.x = 0.0f;
        moby->rot.y = 0.0f;
        moby->rot.z = fast_arc_tan(path->points[1].x - path->points[0].x,
                                   path->points[1].y - path->points[0].y);
        data->cruise_speed =
            D_0015ED6C_gp * 17.0f + (float)data->racer_index * (D_0015ED6C_gp * 1.55f);
        data->launch_speed =
            D_0015ED6C_gp * 24.0f + (float)data->racer_index * (D_0015ED6C_gp * 1.55f);
        if (((unsigned char *)alternate_item_available)[0] == 0) {
            float handicap = (float)(D_0015EE38_macro - 4);

            if (handicap < 0.0f) {
                handicap = 0.0f;
            }
            handicap = 1.0f - handicap * 0.01f;
            if (handicap < 0.77f) {
                handicap = 0.77f;
            }
            data->cruise_speed *= handicap;
            data->launch_speed *= handicap;
        }
        data->bob_phase = random_angle_radians();
        moby->state = 1;
        break;
    }
    case SKID_MCMARX_WAIT_FOR_RACE_MODE:
        if (*(int *)(((char *)&D_001413DC)) == 0x16) {
            moby->update_dist = 0xFF;
            moby->flags &= ~0x41;
            data->point_index = 0;
            data->launch_timer = scale_game_frames(0xB4);
            data->vertical_speed = 0.0f;
            data->yaw_speed = 0.0f;
            data->jump_frames = 0;
            data->f264 = 0;
            data->speed = 0.0f;
            if (data->child == 0) {
                Moby *child = (Moby *)spawn_racer_child((char *)moby);

                data->child = child;
                if (child != 0) {
                    ((ChildData *)*(void **)&child->pvars)->f50 = 1;
                }
            }
            if (data->racer_index % 2 == 1) {
                moby->flags |= 0x8000;
                if (data->child != 0) {
                    data->child->flags |= 0x8000;
                }
            }
            moby->state = 2;
        }
        break;
    case SKID_MCMARX_FOLLOW_PATH: {
        Hero *hero = (Hero *)(((char *)&D_0013F350));

        if (hero->state.control_mode != 0x16) {
            moby->state = 0;
        } else {
            float ground;
            float height_above_ground;
            float max_turn;
            float turn;
            int   hit_count;
            int   j;

            advance_racer_path_node((char *)moby);
            add_moby_frames_for_group_proximity((char *)moby);
            start_racer_branch_jump((char *)moby);
            if (data->jump_frames != 0) {
                data->jump_frames++;
                if (data->jump_frames < scale_game_frames(0x11)) {
                    float path_length = 0.0f;
                    float air_time;
                    float jump_speed;
                    int   i;
                    int   next;

                    for (i = data->point_index; i < data->jump_point; i = next) {
                        next = offset_path_index((char *)data->path, i, 1, 1);
                        path_length += vector_distance_xy((float(*)[4])data->path + (i + 1),
                                                          (float(*)[4])data->path + (next + 1));
                    }
                    air_time = path_length / data->speed;
                    jump_speed = (data->path->points[data->jump_point].z -
                                  data->path->points[data->point_index].z) /
                                     air_time +
                                 frame_time_sq_macro * 21.0f * air_time * 0.5f;
                    if (data->vertical_speed < 0.0f) {
                        data->vertical_speed = 0.0f;
                    }
                    approach_value(&data->vertical_speed, jump_speed, frame_time_sq_macro * 110.0f);
                }
            }
            FUN_L05_00307570_c((unsigned char *)moby);
            copy_racer_path_node((char *)moby, data->point_index, path_point);
            spring_angle(&moby->rot.z,
                         fast_arc_tan(path_point[0] - moby->pos.x, path_point[1] - moby->pos.y),
                         &data->yaw_speed,
                         0,
                         frame_scale_sq_macro * 0.035f,
                         frame_scale_sq_macro * 0.3f,
                         D_0015ED6C_gp * 6.981317f);
            if (data->pos[2] < path_point[2] - 5.0f) {
                data->pos[2] = path_point[2];
            }
            step[0] = fast_cos(moby->rot.z) * data->speed;
            step[1] = fast_sin(moby->rot.z) * data->speed;
            step[2] = 0.0f;
            fast_vec_add(data->pos, data->pos, step);
            fast_vec_scale(probe, step, 4.0f);
            fast_vec_add(probe, probe, data->pos);
            hit_count = func_L00_001F2BE8_308D68(probe, 0, moby, 0, 1.0f);
            if (hit_count != 0) {
                fast_vec_scale(push, step, 7.0f);
                for (j = 0; j < hit_count; j++) {
                    if (is_moby_in_class_range_500_to_540(D_L05_00178300[j]) != 0) {
                        spawn_nearby_effect(
                            (void *)D_L05_00178300[j], (int)moby, 0x30000, data->pos, push, 1.0f);
                    }
                }
            }
            ground = probe_ground_height(data->pos, 0, 0.5f);
            height_above_ground = data->pos[2] - ground;
            if (height_above_ground > 0.0f || data->vertical_speed > 0.0f) {
                data->vertical_speed -= frame_time_sq_macro * 21.0f;
            } else if (absolute_float(height_above_ground) < 0.5f) {
                data->vertical_speed = 0.0f;
                data->pos[2] = ground;
                data->jump_frames = 0;
            }
            data->pos[2] += data->vertical_speed;
            if (fast_dec_timer_s16(&data->launch_timer) != 0) {
                float target = data->cruise_speed;
                Hero *hero;

                if (ground < data->pos[2]) {
                    target = D_0015ED6C_gp * 19.0f;
                }
                hero = (Hero *)(((char *)&D_0013F350));
                if (hero->race_time_frames < scale_game_frames(0x2BC)) {
                    target += D_0015ED6C_gp * 3.0f;
                }
                if (hero->race_rank[data->racer_index] < hero->race_position) {
                    target -= D_0015ED6C_gp * 1.5f;
                } else {
                    target += D_0015ED6C_gp;
                }
                if (target < data->speed) {
                    approach_value(&data->speed, target, frame_time_sq_macro * 7.0f);
                } else {
                    approach_value(&data->speed, target, frame_time_sq_macro * 15.0f);
                }
            } else {
                approach_value(&data->speed, data->launch_speed, frame_time_sq_macro * 20.0f);
            }
            if (height_above_ground < 0.2f && data->vertical_speed < 0.0f) {
                data->pos[2] = ground;
                data->jump_frames = 0;
            }
            manipulator_set_scale(&data->m[3], 3.0f);
            max_turn = D_0015ED6C_gp * 2.6179938f;
            turn = data->yaw_speed;
            if (max_turn < turn) {
                turn = max_turn;
            } else {
                max_turn = -max_turn;
                if (turn < max_turn) {
                    turn = max_turn;
                }
            }
            if (moby->flags & 0x8000) {
                turn = -turn;
            }
            data->m[3].f68 = turn * 30.0f;
            data->m[0].f68 = turn * 20.0f;
            data->m[2].f68 = turn * 50.0f;
            data->m[1].f64 = turn * 40.0f;
            FUN_L00_002628d8((char *)moby,
                             (char *)&data->m[3],
                             3,
                             frame_scale_sq_macro * 0.02f,
                             frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby,
                             (char *)data,
                             0,
                             frame_scale_sq_macro * 0.02f,
                             frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby,
                             (char *)&data->m[2],
                             1,
                             frame_scale_sq_macro * 0.02f,
                             frame_scale_sq_macro * 0.3f);
            FUN_L00_002628d8((char *)moby,
                             (char *)&data->m[1],
                             2,
                             frame_scale_sq_macro * 0.015f,
                             frame_scale_sq_macro * 0.3f);
            qcopy(((float *)&moby->pos), data->pos);
            moby->pos.z += 0.25f;
            data->bob_phase = fast_add_rotations(data->bob_phase, D_0015ED6C_gp * 6.108652f);
            moby->pos.z += fast_sin(data->bob_phase) * 0.08f;
            if (data->child != 0) {
                Moby *child;

                *(long *)&data->child->spawn_frame = *(long *)&moby->spawn_frame;
                child = data->child;
                qcopy(((float *)&child->pos), ((float *)&moby->pos));
                qcopy(((float *)&child->rot), ((float *)&moby->rot));
                if (data->launch_timer != 0) {
                    child->command = 1;
                }
            }
        }
        break;
    }
    }
}
