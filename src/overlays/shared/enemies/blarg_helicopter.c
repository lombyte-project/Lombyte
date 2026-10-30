/* Shared moby class blarg_helicopter: the gunship that flies its routes and fires homing sparks. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"

typedef float HVec49_2CDDC8[4] __attribute__((aligned(16)));

typedef unsigned int HQuad49_2CDDC8 __attribute__((mode(TI)));

typedef struct HMob49 HMob49;

typedef struct HData49 HData49;

typedef struct {
    float xyz[3], length;
} HNode49;

typedef struct {
    int     count;
    char    pad4[12];
    HNode49 points[1];
} HPath49;

typedef struct {
    int      index;
    char     pad4[12];
    HPath49 *path;
    int      path_id;
    char     pad18[24];
} HRoute49;

struct HData49 {
    char          pad0[0x20];
    float         health;
    short         health_word;
    char          pad26[2];
    unsigned char kind, flag29, pad2A[4], enabled, pad2F[9];
    int           request;
    char          pad3C[0x1C];
    unsigned char gun_kind, gun_enabled, gun_mode, pad5B[0xC5];
    HRoute49      route_a, route_b;
    float         gun_yaw_matrix[16], gun_pitch_matrix[16];
    float         yaw_velocity;
    short         shot_timer, reaction_timer;
    int           shoot_region, switch_region;
    float         bob, speed, pitch_velocity, roll_velocity, gun_yaw, gun_yaw_velocity, gun_pitch,
        gun_pitch_velocity;
    int     exit_path, other_exit_path, child_index, alternate;
    HMob49 *projectile;
    int     other_switch_region, activation_region, activation_path, sound, timer;
};

struct HMob49 {
    char           pad0[16];
    HVec49_2CDDC8  position;
    unsigned char  state, pad21[15];
    unsigned char  alpha, nearby;
    short          brightness;
    unsigned short flags;
    char           pad36[10];
    HVec49_2CDDC8  rotation;
    char           pad50[0x28];
    HData49       *data;
    char           pad7C[3];
    unsigned char  update_distance;
    char           pad80[0x14];
    int            animation;
    char           pad98[14];
    short          class_id;
    char           padA8[8];
    unsigned char  persistent, padB1;
    unsigned short pool_index;
    char           padB4[8];
    unsigned char  previous_state;
    char           padBD[3];
    float          transform[16];
};

typedef struct {
    HVec49_2CDDC8 position, rotation, from, to;
    HMob49       *moby;
    int           result, pad48, pad4C;
} HTarget49;

typedef struct {
    HVec49_2CDDC8  direction;
    HMob49        *source;
    int            flags;
    unsigned char  kind, enabled;
    unsigned short class_id;
    float          radius;
    int            allow;
    char           pad24[12];
} HQuery49;

typedef struct {
    HTarget49     target;
    HVec49_2CDDC8 movement, projected, offset;
    HQuery49      hit;
    HVec49_2CDDC8 direction, shot_velocity, shot_origin;
    int           segment_a;
    float         fraction_a;
    int           segment_b;
    float         fraction_b;
} HWork49;

void         mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
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
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern char  D_L03_00166EC0[];
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;

extern char *create_moby(int);
extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *spawn_homing_spark_projectile(char *a, char *pos, int c, int d, float f) __asm__("FUN_L03_002d5208");
extern char *D_L03_0015FFD8_c __asm__("D_L03_0015FFD8") __attribute__((sda));
extern f32   random_angle_radians(void) __asm__("FUN_00213308");
extern float D_0015ED6C;
extern float D_0015ED70;
extern int   frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;
extern float blarg_helicopter_bob_period __asm__("D_L03_00161994")
    __attribute__((sda)); /* no foreign declaration */
extern float blarg_helicopter_bob_amplitude __asm__("D_L03_00161998")
    __attribute__((sda)); /* no foreign declaration */

extern float blarg_helicopter_path_min_speed_gp __asm__("D_L03_0016199C") __attribute__((sda));
extern void  attach_manipulator(int, int, void *) __asm__("FUN_0020cb10");
extern void  detach_manipulator(int, void *) __asm__("func_0020CB88");
void         record_moby_hit(unsigned char *m, void *src) __asm__("FUN_L00_00259a88");
void         handle_blarg_helicopter_hits(char *m) __asm__("FUN_L03_002ce238");
extern int   D_L03_001BA5D0[] __asm__("D_L03_001BA5D0");
extern int select_moby_target_with_hero_fallback(void *, void *, float) __asm__("FUN_L03_0024e830");
extern void func_L00_00263950_2CDDC8(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void handle_blarg_helicopter_hits(char *) __asm__("FUN_L03_002ce238");
extern int  func_L00_0025EFC0_2CDDC8(
     void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern float func_00214D28_2CDDC8(float *, float, float) __asm__("FUN_00213ed8");
extern float func_001F9878_2CDDC8(float) __asm__("FUN_001f96b0");
extern void  raise_kernel_trap(void) __asm__("FUN_001f97b0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern char *
spawn_homing_spark_projectile(char *, char *, int, int, float) __asm__("FUN_L03_002d5208");
extern void func_L00_002514B8_2CDDC8(void *) __asm__("FUN_0020d580");
extern char D_0013E633_2CDDC8[] __asm__("D_0013E533");
extern void func_L00_002584A8_2CDDC8(void *, int, int) __asm__("FUN_L00_00257470");
extern void func_L00_00265050_2CDDC8(void *,
                                     int,
                                     void *,
                                     void *,
                                     int,
                                     int,
                                     float,
                                     void *,
                                     void *,
                                     void *) __asm__("FUN_L00_00263fd8");
extern void func_L00_0025B178_2CDDC8(void *) __asm__("FUN_L00_0025a120");

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");

/* Updates the gunship (helicopter) moby: activation path (1), patrol route with gun aiming and shots (2),
   exit path or climb-out (3), a short reaction after a route change (4) and destruction (5). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002CDDC8), where it is exact; names translated to the US level program. */

void blarg_helicopter_update(HMob49 *moby) __asm__("FUN_L03_002cca00");

void blarg_helicopter_update(HMob49 *moby) {
    enum {
        BLARG_HELICOPTER_INITIALIZE = 0,
        BLARG_HELICOPTER_ACTIVATION_PATH = 1,
        BLARG_HELICOPTER_PATROL_AND_ATTACK = 2,
        BLARG_HELICOPTER_EXIT = 3,
        BLARG_HELICOPTER_TIMED_REACTION = 4,
        BLARG_HELICOPTER_DESTROY = 5
    };
    HData49 *data = moby->data;
    HWork49  work;
    float    target_range = 40.0f;
    float    motion_scale, motion_gain;
    int      index;
    if (data->timer)
        target_range = 80.0f;
    select_moby_target_with_hero_fallback(moby, &work.target, target_range);
    index = (short)moby->pool_index;
    if ((D_L03_001BA5D0[index >> 5] >> (moby->pool_index & 31)) & 1) {
        if (data->child_index != -1)
            mark_moby_for_removal((HMob49 *)(D_L03_0015FFD8_c + (data->child_index << 8)));
        mark_moby_for_removal(moby);
        return;
    }
    if (data->route_a.path_id == -1 || (moby->state != 0 && data->enabled != 1)) {
        if (data->child_index != -1)
            mark_moby_for_removal((HMob49 *)(D_L03_0015FFD8_c + (data->child_index << 8)));
        mark_moby_for_removal(moby);
        return;
    }
    if (data->request) {
        data->request = 0;
        data->timer = scale_game_frames(240);
    }
    fast_dec_timer(&data->timer);
    if (data->child_index != -1) {
        HMob49 *child = (HMob49 *)(D_L03_0015FFD8_c + (data->child_index << 8));
        if (child->class_id == 0x23E && (signed char)child->state >= 0) {
            char *gun = (char *)child->data + 0x150;
            manipulator_set_scale(gun, 2.5f);
            func_L00_00263950_2CDDC8(
                (char *)child, gun, 2, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
        }
    }
    handle_blarg_helicopter_hits((char *)moby);
    switch (moby->state) {
    case BLARG_HELICOPTER_INITIALIZE: {
        HPath49 **paths;
        moby->update_distance = 0x20;
        data->health = 3.0f;
        data->health_word = truncate_float_to_s32(3.0f);
        data->enabled = 1;
        data->kind = 2;
        data->gun_kind = 25;
        data->gun_enabled = 1;
        data->gun_mode = 6;
        data->flag29 = 0;
        paths = ((HPath49 **)l03_paths);
        data->route_a.path = paths[data->route_a.path_id];
        if (data->route_b.path_id != -1)
            data->route_b.path = paths[data->route_b.path_id];
        qcopy(moby->position, data->route_a.path->points);
        data->route_a.index = 1;
        data->speed = 0;
        data->pitch_velocity = 0;
        data->roll_velocity = 0;
        data->alternate = 0;
        attach_manipulator((char *)moby, 0, data->gun_yaw_matrix);
        attach_manipulator((char *)moby, 1, data->gun_pitch_matrix);
        data->sound = -1;
        data->projectile = 0;
        data->gun_yaw = moby->rotation[2];
        data->gun_yaw_velocity = 0;
        data->gun_pitch = moby->rotation[1];
        data->gun_pitch_velocity = 0;
        if (moby->brightness < 128)
            moby->brightness = 128;
        if (data->child_index != -1) {
            HMob49 *child = (HMob49 *)(D_L03_0015FFD8_c + (data->child_index << 8));
            blend_moby_animation(child, 8, 0, 0);
            child->alpha = 0;
            child->animation = 0;
            child->flags &= 0xFFBE;
        }
        if (visited52[moby->persistent + (frame_scale_inv_macro[7] << 4)] != 255 &&
            data->activation_path != -1 && data->activation_region != -1) {
            HPath49 *path = paths[data->activation_path];
            int      i;
            for (i = 0; i < path->count - 2; i++)
                path->points[i].length = fast_vec_dist(&path->points[i], &path->points[i + 1]);
            *(HQuad49_2CDDC8 *)moby->position = *(HQuad49_2CDDC8 *)path->points;
            moby->state = 1;
            break;
        }
        moby->state = 2;
        break;
    }
    case BLARG_HELICOPTER_ACTIVATION_PATH: {
        HPath49 *path = ((HPath49 *)l03_paths[data->activation_path]);
        float    progress, distance, step, limit, angle;
        if (data->activation_region != -1 &&
            !is_point_inside_clip_volume(D_0013F3D0, data->activation_region))
            break;
        data->activation_region = -1;
        func_L00_0025EFC0_2CDDC8(path,
                                 moby->position,
                                 work.projected,
                                 &work.segment_a,
                                 &work.fraction_a,
                                 0,
                                 999.0f,
                                 5.0f,
                                 0.0f);
        progress = convert_integer_to_float(work.segment_a) * path->points[0].length +
                   work.fraction_a + 2.0f;
        distance = fast_vec_dist(moby->position, (char *)path + (path->count << 4));
        step = D_0015ED70 * 10.0f;
        if (distance <= data->speed * data->speed / (step + step)) {
            float speed = data->speed - step;
            limit = blarg_helicopter_path_min_speed_gp * D_0015ED6C;
            data->speed = speed;
            if (speed < limit)
                data->speed = limit;
        } else {
            float speed = data->speed + step;
            limit = D_0015ED6C * 20.0f;
            data->speed = speed;
            if (limit < speed)
                data->speed = limit;
        }
        work.segment_a = truncate_float_to_s32(progress / path->points[0].length);
        work.fraction_a =
            (progress - convert_integer_to_float(work.segment_a) * path->points[0].length) /
            path->points[0].length;
        if (work.segment_a >= path->count - 1) {
            if (data->route_a.path_id != -1 &&
                data->speed <= blarg_helicopter_path_min_speed_gp * D_0015ED6C) {
                data->alternate = 0;
                qcopy(moby->position, data->route_a.path->points);
                data->route_a.index = 1;
                moby->previous_state = 2;
                moby->state = 4;
                data->reaction_timer = scale_game_frames(15);
                break;
            }
            if (data->speed <= blarg_helicopter_path_min_speed_gp * D_0015ED6C) {
                raise_kernel_trap();
                return;
            }
            work.segment_a = path->count - 2;
            work.fraction_a = 1.0f;
        }
        fast_vec_sub(
            work.projected, &path->points[work.segment_a + 1], &path->points[work.segment_a]);
        fast_vec_scale(work.projected, work.projected, work.fraction_a);
        fast_vec_add(work.projected, work.projected, path->points + work.segment_a);
        if (work.segment_a + 1 < path->count - 8) {
            float *next = (float *)((char *)path + ((work.segment_a + 1) << 4));
            angle = fast_arc_tan(next[4] - moby->position[0], next[5] - moby->position[1]);
            moby->rotation[2] =
                spring_turn(&data->yaw_velocity, moby->rotation[2], angle, 0.01f, 0.3f, 0.1f);
        } else {
            angle = fast_arc_tan(work.target.position[0] - moby->position[0],
                                 work.target.position[1] - moby->position[1]);
            moby->rotation[2] =
                spring_turn(&data->yaw_velocity, moby->rotation[2], angle, 0.01f, 0.3f, 0.1f);
        }
        fast_vec_sub(work.movement, work.projected, moby->position);
        distance = fast_vec_length(work.movement);
        if (data->speed < distance)
            distance = data->speed;
        fast_vec_normalize(work.movement, work.movement, distance);
        motion_scale = 20.0f;
        motion_gain = 1.0471976f;
        {
            float delta, roll, pitch;
            fast_vec_add(moby->position, moby->position, work.movement);
            delta = fast_subtract_rotations(fast_arc_tan(work.movement[0], work.movement[1]),
                                            moby->rotation[2]);
            roll = data->speed * 0.34906584f * fast_cos(delta) / (D_0015ED6C * motion_scale);
            pitch = data->speed * -0.34906584f * fast_sin(delta);
            motion_scale *= D_0015ED6C;
            pitch /= motion_scale;
            moby->rotation[1] = spring_turn(&data->roll_velocity,
                                            moby->rotation[1],
                                            roll,
                                            D_0015ED70 * 0.5235988f,
                                            D_0015ED70 * motion_gain,
                                            D_0015ED6C * 0.7853982f);
            moby->rotation[0] = spring_turn(&data->pitch_velocity,
                                            moby->rotation[0],
                                            pitch,
                                            D_0015ED70 * motion_gain,
                                            D_0015ED70 * 2.0943952f,
                                            D_0015ED6C * 1.5707964f);
        }
        break;
    }
    case BLARG_HELICOPTER_PATROL_AND_ATTACK: {
        HRoute49 *route = data->alternate ? &data->route_b : &data->route_a;
        int       point = route->index;
        float     angle = fast_arc_tan(work.target.position[0] - moby->position[0],
                                   work.target.position[1] - moby->position[1]);
        HMob49   *projectile;
        float     distance, other_distance;
        moby->rotation[2] =
            spring_turn(&data->yaw_velocity, moby->rotation[2], angle, 0.01f, 0.3f, 0.1f);
        projectile = data->projectile;
        if (!projectile || projectile->class_id != 0x350 || projectile->state == 254 ||
            projectile->state == 253) {
            moby_point_position(moby, 0, work.movement);
            fast_vec_sub(work.direction, work.target.position, work.movement);
            *(HQuad49_2CDDC8 *)work.projected = *(HQuad49_2CDDC8 *)work.direction;
            transpose_rotation_matrix(work.offset, moby->transform);
            transform_vector(work.projected, work.projected, work.offset);
            angle = fast_arc_tan(work.projected[0], work.projected[1]);
            data->gun_yaw = spring_turn(&data->gun_yaw_velocity,
                                        data->gun_yaw,
                                        angle,
                                        D_0015ED70 * 6.2831855f,
                                        D_0015ED70 * 12.566371f,
                                        D_0015ED6C * 6.2831855f);
            angle = -fast_arc_tan(vector_length_xy(work.projected), work.projected[2]);
            data->gun_pitch = spring_turn(&data->gun_pitch_velocity,
                                          data->gun_pitch,
                                          angle,
                                          D_0015ED70 * 3.1415927f,
                                          D_0015ED70 * 6.2831855f,
                                          D_0015ED6C * 1.5707964f);
            rotation_2D5FA8(data->gun_yaw_matrix + 4, 2, data->gun_yaw);
            rotation_2D5FA8(data->gun_pitch_matrix + 4, 2, data->gun_pitch);
            data->projectile = 0;
        } else {
            moby_point_position(moby, 0, work.movement);
            fast_vec_sub(work.direction, data->projectile->position, work.movement);
            *(HQuad49_2CDDC8 *)work.projected = *(HQuad49_2CDDC8 *)work.direction;
            transpose_rotation_matrix(work.offset, moby->transform);
            transform_vector(work.projected, work.projected, work.offset);
            angle = fast_arc_tan(work.projected[0], work.projected[1]);
            data->gun_yaw = spring_turn(&data->gun_yaw_velocity,
                                        data->gun_yaw,
                                        angle,
                                        D_0015ED70 * 6.2831855f,
                                        D_0015ED70 * 12.566371f,
                                        D_0015ED6C * 6.2831855f);
            angle = -fast_arc_tan(vector_length_xy(work.projected), work.projected[2]);
            data->gun_pitch = spring_turn(&data->gun_pitch_velocity,
                                          data->gun_pitch,
                                          angle,
                                          D_0015ED70 * 3.1415927f,
                                          D_0015ED70 * 6.2831855f,
                                          D_0015ED6C * 1.5707964f);
            rotation_2D5FA8(data->gun_yaw_matrix + 4, 2, data->gun_yaw);
            rotation_2D5FA8(data->gun_pitch_matrix + 4, 2, data->gun_pitch);
        }
        if (point > 0 && point < route->path->count - 1) {
            float node_distance =
                vector_distance_xy(&route->path->points[point], work.target.position);
            if (node_distance < 20.0f &&
                absolute_float(work.target.position[2] - moby->position[2]) < 10.0f &&
                node_distance <
                    fast_vec_dist(&route->path->points[point + 1], work.target.position) &&
                point < route->path->count - 2)
                route->index++;
        }
        fast_vec_sub(work.direction, &route->path->points[point], moby->position);
        distance = fast_vec_length(work.direction);
        if (distance < data->speed * data->speed / ((D_0015ED70 * 4.0f) * 2.0f) ||
            fast_vec_dist(&route->path->points[point], work.target.position) <
                fast_vec_dist(moby->position, work.target.position)) {
            data->speed -= D_0015ED70 * 4.0f;
            if (data->speed < 0)
                data->speed = 0;
        } else {
            data->speed += D_0015ED70 * 4.0f;
            if (D_0015ED6C * 10.0f < data->speed)
                data->speed = D_0015ED6C * 10.0f;
        }
        distance = fast_vec_dist(&route->path->points[point], work.target.position);
        other_distance = fast_vec_dist(moby->position, work.target.position);
        if (distance < other_distance)
            fast_vec_normalize(work.direction, work.direction, -data->speed);
        else
            fast_vec_normalize(work.direction, work.direction, data->speed);
        motion_scale = 10.0f;
        motion_gain = 1.0471976f;
        {
            float delta, roll, pitch;
            fast_vec_add(moby->position, moby->position, work.direction);
            delta = fast_subtract_rotations(fast_arc_tan(work.direction[0], work.direction[1]),
                                            moby->rotation[2]);
            roll = data->speed * 0.34906584f * fast_cos(delta) / (D_0015ED6C * motion_scale);
            pitch = data->speed * -0.34906584f * fast_sin(delta);
            motion_scale *= D_0015ED6C;
            pitch /= motion_scale;
            moby->rotation[1] = spring_turn(&data->roll_velocity,
                                            moby->rotation[1],
                                            roll,
                                            D_0015ED70 * 0.5235988f,
                                            D_0015ED70 * motion_gain,
                                            D_0015ED6C * 0.7853982f);
            moby->rotation[0] = spring_turn(&data->pitch_velocity,
                                            moby->rotation[0],
                                            pitch,
                                            D_0015ED70 * motion_gain,
                                            D_0015ED70 * 2.0943952f,
                                            D_0015ED6C * 1.5707964f);
        }
        if (is_point_inside_clip_volume(work.target.position,
                                        data->alternate ? data->other_switch_region
                                                        : data->switch_region)) {
            if (data->alternate ? data->other_exit_path != -1 : data->exit_path != -1) {
                HPath49 *path = ((
                    HPath49 *)l03_paths[data->alternate ? data->other_exit_path : data->exit_path]);
                int      i;
                for (i = 0; i < path->count - 2; i++)
                    path->points[i].length = fast_vec_dist(&path->points[i], &path->points[i + 1]);
            }
            moby->previous_state = 3;
            moby->state = 4;
            data->reaction_timer = scale_game_frames(15);
            break;
        } else if (is_point_inside_clip_volume(work.target.position, data->shoot_region) &&
                   (fast_dec_timer_s16(&data->shot_timer) || (!data->projectile && data->timer))) {
            data->shot_timer = scale_game_frames(360);
            qcopy(work.shot_origin, work.target.position);
            fast_vec_sub(work.shot_velocity, work.shot_origin, work.movement);
            set_vector_length_xy(work.shot_velocity, work.shot_velocity, D_0015ED6C * 6.0f);
            work.shot_velocity[2] = 0;
            data->projectile = (HMob49 *)spawn_homing_spark_projectile((char *)work.shot_velocity,
                                                                       (char *)work.movement,
                                                                       (int)moby,
                                                                       scale_game_frames(240),
                                                                       work.target.position[2]);
        }
        break;
    }
    case BLARG_HELICOPTER_EXIT: {
        HPath49 *path;
        float    progress, distance, step, limit, angle;
        if (data->exit_path == -1) {
            func_00214D28_2CDDC8(&moby->position[2], 200.0f, D_0015ED6C * 8.0f);
            moby->rotation[1] = spring_turn(&data->roll_velocity,
                                            moby->rotation[1],
                                            0.0f,
                                            D_0015ED70 * 0.5235988f,
                                            D_0015ED70 * 1.0471976f,
                                            D_0015ED6C * 0.7853982f);
            moby->rotation[0] = spring_turn(&data->pitch_velocity,
                                            moby->rotation[0],
                                            0.0f,
                                            D_0015ED70 * 0.5235988f,
                                            D_0015ED70 * 1.0471976f,
                                            D_0015ED6C * 0.7853982f);
            break;
        }
        path = ((HPath49 *)l03_paths[data->alternate ? data->other_exit_path : data->exit_path]);
        func_L00_0025EFC0_2CDDC8(path,
                                 moby->position,
                                 work.projected,
                                 &work.segment_b,
                                 &work.fraction_b,
                                 0,
                                 999.0f,
                                 5.0f,
                                 0.0f);
        progress = convert_integer_to_float(work.segment_b) * path->points[0].length +
                   work.fraction_b + 2.0f;
        distance = fast_vec_dist(moby->position, (char *)path + (path->count << 4));
        step = D_0015ED70 * 10.0f;
        if (distance <= data->speed * data->speed / (step + step)) {
            float speed = data->speed - step;
            limit = blarg_helicopter_path_min_speed_gp * D_0015ED6C;
            data->speed = speed;
            if (speed < limit)
                data->speed = limit;
        } else {
            float speed = data->speed + step;
            limit = D_0015ED6C * 20.0f;
            data->speed = speed;
            if (limit < speed)
                data->speed = limit;
        }
        work.segment_b = truncate_float_to_s32(progress / path->points[0].length);
        work.fraction_b =
            (progress - convert_integer_to_float(work.segment_b) * path->points[0].length) /
            path->points[0].length;
        if (work.segment_b >= path->count - 1) {
            if (data->other_exit_path != -1 && data->route_b.path_id != -1 &&
                data->speed <= blarg_helicopter_path_min_speed_gp * D_0015ED6C) {
                HRoute49 *route;
                data->alternate ^= 1;
                route = data->alternate ? &data->route_b : &data->route_a;
                qcopy(moby->position, route->path->points);
                route->index = 1;
                moby->previous_state = 2;
                moby->state = 4;
                data->reaction_timer = scale_game_frames(15);
                break;
            }
            if (data->speed <= blarg_helicopter_path_min_speed_gp * D_0015ED6C) {
                raise_kernel_trap();
                return;
            }
            work.segment_b = path->count - 2;
            work.fraction_b = 1.0f;
        }
        fast_vec_sub(
            work.projected, &path->points[work.segment_b + 1], &path->points[work.segment_b]);
        fast_vec_scale(work.projected, work.projected, work.fraction_b);
        fast_vec_add(work.projected, work.projected, path->points + work.segment_b);
        angle = fast_arc_tan(work.target.position[0] - moby->position[0],
                             work.target.position[1] - moby->position[1]);
        moby->rotation[2] =
            spring_turn(&data->yaw_velocity, moby->rotation[2], angle, 0.01f, 0.3f, 0.1f);
        fast_vec_sub(work.movement, work.projected, moby->position);
        distance = fast_vec_length(work.movement);
        if (data->speed < distance)
            distance = data->speed;
        fast_vec_normalize(work.movement, work.movement, distance);
        motion_scale = 20.0f;
        motion_gain = 1.0471976f;
        {
            float delta, roll, pitch;
            fast_vec_add(moby->position, moby->position, work.movement);
            delta = fast_subtract_rotations(fast_arc_tan(work.movement[0], work.movement[1]),
                                            moby->rotation[2]);
            roll = data->speed * 0.34906584f * fast_cos(delta) / (D_0015ED6C * motion_scale);
            pitch = data->speed * -0.34906584f * fast_sin(delta);
            motion_scale *= D_0015ED6C;
            pitch /= motion_scale;
            moby->rotation[1] = spring_turn(&data->roll_velocity,
                                            moby->rotation[1],
                                            roll,
                                            D_0015ED70 * 0.5235988f,
                                            D_0015ED70 * motion_gain,
                                            D_0015ED6C * 0.7853982f);
            moby->rotation[0] = spring_turn(&data->pitch_velocity,
                                            moby->rotation[0],
                                            pitch,
                                            D_0015ED70 * motion_gain,
                                            D_0015ED70 * 2.0943952f,
                                            D_0015ED6C * 1.5707964f);
        }
        break;
    }
    case BLARG_HELICOPTER_TIMED_REACTION:
        if (fast_dec_timer_s16(&data->reaction_timer))
            moby->state = moby->previous_state;
        else {
            data->speed -= D_0015ED70 * 4.0f;
            if (data->speed < 0)
                data->speed = 0;
            motion_scale = 10.0f;
            motion_gain = 1.0471976f;
            work.movement[0] = fast_cos(moby->rotation[2]) * -data->speed;
            work.movement[1] = fast_sin(moby->rotation[2]) * -data->speed;
            work.movement[2] = 0;
            {
                float delta, roll, pitch;
                fast_vec_add(moby->position, moby->position, work.movement);
                delta = fast_subtract_rotations(fast_arc_tan(work.movement[0], work.movement[1]),
                                                moby->rotation[2]);
                roll = data->speed * 0.34906584f * fast_cos(delta) / (D_0015ED6C * motion_scale);
                pitch = data->speed * -0.34906584f * fast_sin(delta);
                motion_scale *= D_0015ED6C;
                pitch /= motion_scale;
                moby->rotation[1] = spring_turn(&data->roll_velocity,
                                                moby->rotation[1],
                                                roll,
                                                D_0015ED70 * 0.5235988f,
                                                D_0015ED70 * motion_gain,
                                                D_0015ED6C * 0.7853982f);
                moby->rotation[0] = spring_turn(&data->pitch_velocity,
                                                moby->rotation[0],
                                                pitch,
                                                D_0015ED70 * motion_gain,
                                                D_0015ED70 * 2.0943952f,
                                                D_0015ED6C * 1.5707964f);
            }
        }
        break;
    case BLARG_HELICOPTER_DESTROY: {
        float angle;
        int   i;
        *(HQuad49_2CDDC8 *)work.movement = 0;
        angle = random_angle_radians();
        moby_point_position(moby, 4, work.projected);
        for (i = 0; i < 4; i++) {
            work.offset[0] = fast_cos(angle) * 1.5f;
            work.offset[1] = fast_sin(angle) * 1.5f;
            work.offset[2] = 0;
            angle = fast_add_rotations(angle, 1.5707964f);
            add_vectors(work.hit.direction, work.offset, work.projected);
        }
        moby_point_position(moby, 5, work.offset);
        if (data->child_index != -1) {
            HMob49 *child = (HMob49 *)(D_L03_0015FFD8_c + (data->child_index << 8));
            ((unsigned char *)child->data)[0x262] = 2;
            child->alpha = 255;
            child->flags &= 0xFFF9;
            work.hit.radius = 20.0f;
            work.hit.flags = 0x830000;
            work.hit.allow = 1;
            work.hit.source = moby;
            fast_vec_sub(work.hit.direction, child->position, moby->position);
            set_vector_length_xy(work.hit.direction, work.hit.direction, 1.0f);
            work.hit.direction[2] = 1.0f;
            work.hit.direction[3] = 5627.925f;
            work.hit.kind = 3;
            work.hit.enabled = 3;
            work.hit.class_id = moby->class_id;
            record_moby_hit(child, &work.hit);
        }
        spawn_explosion(moby,
                        work.movement,
                        moby->position,
                        0.0f,
                        0.0f,
                        20,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        3.0f,
                        2,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        func_L00_002584A8_2CDDC8(moby, 0, -1);
        func_L00_00265050_2CDDC8(moby,
                                 0x637,
                                 moby->position,
                                 moby->rotation,
                                 0,
                                 0,
                                 0.0f,
                                 D_L03_0015F580_macro,
                                 D_L03_0015F580_macro,
                                 D_L03_0015F580_macro);
        func_L00_00265050_2CDDC8(moby,
                                 0x638,
                                 moby->position,
                                 moby->rotation,
                                 0,
                                 0,
                                 0.0f,
                                 D_L03_0015F580_macro,
                                 D_L03_0015F580_macro,
                                 D_L03_0015F580_macro);
        func_L00_00265050_2CDDC8(moby,
                                 0x639,
                                 moby->position,
                                 moby->rotation,
                                 0,
                                 0,
                                 0.0f,
                                 D_L03_0015F580_macro,
                                 D_L03_0015F580_macro,
                                 D_L03_0015F580_macro);
        detach_manipulator(moby, data->gun_yaw_matrix);
        detach_manipulator(moby, data->gun_pitch_matrix);
        if (data->sound != -1) {
            unsigned char *audio = (unsigned char *)(D_0013E633_2CDDC8 + 0x1D + data->sound * 0x70);
            if (*(HMob49 **)(audio + 0x88) == moby && audio[0x74])
                release_voice_slot(data->sound);
        }
        data->sound = -1;
        mark_moby_for_removal(moby);
        return;
    }
    default:
        break;
    }
    if (moby->nearby && fast_vec_dist(moby->position, D_L03_00166EC0) < 40.0f)
        func_L00_0025B178_2CDDC8(moby);
    moby->position[2] -= blarg_helicopter_bob_amplitude * fast_cos(data->bob);
    data->bob = fast_add_rotations(
        data->bob, 6.2831855f / func_001F9878_2CDDC8(blarg_helicopter_bob_period * 60.0f));
    moby->position[2] += blarg_helicopter_bob_amplitude * fast_cos(data->bob);
    if (data->child_index != -1) {
        HMob49 *child = (HMob49 *)(D_L03_0015FFD8_c + (data->child_index << 8));
        *(HQuad49_2CDDC8 *)child->position = *(HQuad49_2CDDC8 *)moby->position;
        *(HQuad49_2CDDC8 *)child->rotation = *(HQuad49_2CDDC8 *)moby->rotation;
        func_L00_002514B8_2CDDC8(child);
        update_entity(child);
        child->flags |= 6;
    }
}

/* Moby update: advance a timer from a collision query, switch state, aim head angles. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002CF600), where it is exact; names translated to the US level program. */

void handle_blarg_helicopter_hits(char *helicopter_moby) {
    int   hit_reaction_status;
    float fv;
    int   hit_reaction_result;
    char *helicopter_data;
    char *hit_record;
    float reaction_heading_angle, reaction_relative_heading_angle, reaction_roll_angle,
        reaction_pitch_angle;
    helicopter_data = ((char *)MOBY(helicopter_moby)->pvars);
    fv = 0.0f;
    hit_record = find_moby_hit(helicopter_moby, 0x330000, 0);
    hit_reaction_result = hit_reaction(
        helicopter_moby, hit_record, helicopter_data + 0x20, 0, &hit_reaction_status, &fv, 0, 4);
    if (hit_reaction_status != 1 && MOBY(helicopter_moby)->state != 5) {
        float remaining_health = *(float *)(helicopter_data + 0x20) - fv;
        *(float *)(helicopter_data + 0x20) = remaining_health;
        if (hit_reaction_result == 1 || remaining_health <= 0.0f) {
            MOBY(helicopter_moby)->state = 5;
        } else if (hit_reaction_result != 0xB) {
            *(unsigned char *)(helicopter_data + 0x67) = 0xB4;
            trooper_anim_update(helicopter_moby, (short *)(helicopter_data + 0x60));
            if (*(float *)(hit_record + 0x1C) == 5627.925f && MOBY(helicopter_moby)->state != 4) {
                reaction_heading_angle =
                    fast_arc_tan(MOBY(helicopter_moby)->pos.x - *(float *)(hit_record + 0),
                                 MOBY(helicopter_moby)->pos.y - *(float *)(hit_record + 4));
                reaction_relative_heading_angle =
                    fast_subtract_rotations(reaction_heading_angle, MOBY(helicopter_moby)->rot.z);
                reaction_roll_angle = fast_cos(reaction_relative_heading_angle) * 0.08726646f;
                reaction_pitch_angle = fast_sin(reaction_relative_heading_angle) * -0.34906584f;
                if (absolute_float(MOBY(helicopter_moby)->rot.x) <
                    absolute_float(reaction_pitch_angle)) {
                    MOBY(helicopter_moby)->rot.x = reaction_pitch_angle;
                    *(float *)(helicopter_data + 0x218) = 0.0f;
                }
                if (absolute_float(MOBY(helicopter_moby)->rot.y) <
                    absolute_float(reaction_roll_angle)) {
                    MOBY(helicopter_moby)->rot.y = reaction_roll_angle;
                    *(float *)(helicopter_data + 0x21C) = 0.0f;
                }
                *(float *)(helicopter_data + 0x214) = frame_time * 4.0f;
                *(short *)(helicopter_data + 0x206) = scale_game_frames(0x3C);
                MOBY(helicopter_moby)->command = MOBY(helicopter_moby)->state;
                MOBY(helicopter_moby)->state = 4;
                allocate_voice_for_target_entry(1, 0, (int)helicopter_moby);
            }
        }
    }
    MOBY(helicopter_moby)->hit_slot = 0xFF;
    trooper_anim_finish(helicopter_moby, helicopter_data + 0x60);
}

/* Spawns a class 0x350 moby and fills its data block from a position, vector and parameters. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002D65D0), where it is exact; names translated to the US level program. */

char *spawn_homing_spark_projectile(char *velocity,
                                    char *spawn_position,
                                    int   target_moby,
                                    int   lifetime_frames,

                                    float target_height) {
    char *spawned_moby = (char *)create_moby(0x350);
    char *data;
    if (spawned_moby != 0) {
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7E;
        MOBY(spawned_moby)->visible = 1;
        MOBY(spawned_moby)->state = 1;
        data = ((char *)MOBY(spawned_moby)->pvars);
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(data, velocity);
        *(float *)(data + 0xC) = target_height;
        *(int *)(data + 0x14) = lifetime_frames;
        *(int *)(data + 0x10) = target_moby;
        *(int *)(data + 0x18) = 0;
    }
    return spawned_moby;
}

char D_L05_002189E0 NOT_SDA = 0x57;
float               D_L07_00161934 = 2.0f;
float               D_L07_00161938 = 0.17f;
float               D_L07_0016193C = 0.1f;
int D_L07_001BA550  NOT_SDA = 0;
char D_L07_002110C0 NOT_SDA = 0x57;
char D_L13_001F4FD8 NOT_SDA = 0x57;
char D_L15_00174480 NOT_SDA = 0;
