/* Shared moby class infobot: flies its path between planets, then plays the unlock sequence. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/shared/pickups/infobot_data/update_infobot_slot_path_motion.h"
#include "rnc/globals.h"
#include "rnc/game/level_state.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef struct Moby Moby;

typedef unsigned int u128_2d5988 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2d5988 q;
    float       f[4];
} Vec4_2d5988;

typedef struct {
    u8    p0[0x60];
    float v60;
    float v64;
    float v68;
    float v6C;
} St;

typedef struct {
    u8    p0[0x10];
    float v[3];
    u8    p1[0x20 - 0x1C];
    u8    flag;
    u8    p2[0x40 - 0x21];
    float f40;
    float f44;
    float f48;
    u8    p3[0x78 - 0x4C];
    St   *st;
} Obj;

typedef struct {
    s32         n;
    u8          pad[0x10 - 4];
    Vec4_2d5988 a[1];
    Vec4_2d5988 b[1];
} Path_2d5988;

typedef union {
    u128 q;
    f32  f[4];
    struct {
        f32 x, y, z, w;
    } v;
} Vec;

typedef struct {
    s32 count;
    u8  pad4[0xC];
    Vec pts[1]; /* pts[i].v.w: length of segment i */
} Path;

typedef struct {
    s32  arrived;  /* 0x00 */
    s32  planet;   /* 0x04 */
    s32  cutscene; /* 0x08 */
    s8   msg0;     /* 0x0C */
    s8   msg1;     /* 0x0D */
    s8   msg2;     /* 0x0E */
    u8   pad0F[0x41];
    s32  attach;   /* 0x50 */
    s32  carrier;  /* 0x54 */
    s32  path;     /* 0x58 */
    f32  bob;      /* 0x5C */
    f32  speed;    /* 0x60 */
    f32  spin[3];  /* 0x64: x, y, z turn rates */
    s32  slots[8]; /* 0x70 */
    u8   pad90[0x14];
    f32  sway;    /* 0xA4 */
    s32  seg;     /* 0xA8 */
    f32  yaw;     /* 0xAC */
    s32  waiting; /* 0xB0 */
    u8   padB4[0xC];
    u128 home; /* 0xC0 */
} Infobot;

typedef struct {
    Vec pos;
    u8  pad10[0x2218];
    s32 active; /* 0x2228 */
} Player;

typedef struct {
    u8  pad0[0x80];
    Vec target; /* 0x80 */
} Camera;

typedef struct {
    u8  pad0[0x30];
    Vec pos; /* 0x30 */
    u8  pad40[0x30];
    Vec dir; /* 0x70 */
} Cutscene;  /* 0x80 */

typedef struct {
    u8  p0[0x10];
    f32 r[4];
    f32 a[4];
    s32 c[4];
    f32 x40, x44, x48, x4c;
} M002d6bf0;

typedef struct {
    u8         p0[0x78];
    M002d6bf0 *m;
} O002d6bf0;

typedef struct {
    char  p0[0x10];
    float a[4];
    float b[4];
    int   c[4];
    float d[4];
} P_2d6cd0;

extern s32   random_integer_below(s32) __asm__("FUN_00213260");
extern float D_0015ED70;

float absolute_float(float input) __asm__("func_001F99C0");

extern s32 D_L00_0015F5C4;

extern float D_0015ED6C;
extern int   random_integer_below(int) __asm__("FUN_00213260");

extern void FUN_L00_0025df68_2d5988(
    void *, void *, void *, s32 *, f32 *, s32, f32, f32, f32) __asm__("FUN_L00_0025df68");
extern float turn_f(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern float FUN_001f9a68_2d5988(void *, void *, f32) __asm__("FUN_001f9a68");
extern float FUN_001f9af0_2d5988(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9bf8_2d5988(void *, void *, f32) __asm__("FUN_001f9bf8");
extern float FUN_001f9dc8_2d5988(f32) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2d5988(f32) __asm__("FUN_001f9de0");

extern Path        *D_L00_001B04B0[];
extern Player       D_0013F3D0;
extern Camera       D_0013F350;
extern f32          D_0015ED6C;
extern f32          D_0015ED70;
extern struct Moby *D_L00_0015FFD8; /* the level's moby array */
extern Cutscene    *D_L00_001600EC_2d5de8 __asm__("D_L00_001600EC");

extern void FUN_L00_002d6bf0_2d5de8(struct Moby *) __asm__("FUN_L00_002d6bf0");
extern void FUN_L00_002d6cd0_2d5de8(struct Moby *) __asm__("FUN_L00_002d6cd0");
extern void FUN_L00_002d5988_2d5de8(struct Moby *, Path *, s32) __asm__("FUN_L00_002d5988");
extern f32  spring_turn(f32  angle,
                        f32  target,
                        f32  accel,
                        f32  damping,
                        f32  limit,
                        f32 *speed) __asm__("FUN_L00_00258110");
extern void FUN_L00_0025df68(Path *, void *, void *, s32 *, f32 *, s32, f32, f32, f32);
extern s32  FUN_L00_00260400(struct Moby *, struct Moby *, void *, void *, void *, void *);
extern void unlock_planet(s32) __asm__("FUN_L00_002607d0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void start_scene(s32) __asm__("FUN_L00_00298840");
extern void queue_scene_movie_with_game_language(s32) __asm__("FUN_L00_00299460");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern void FUN_0020c828(struct Moby *);

s32 random_integer_below(s32) __asm__("FUN_00213260");

extern int  D_15F5C4_2d6cd0 __asm__("D_L00_0015F5C4");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void FUN_L00_00272f68(void *, int, int, int, int, int, int, float);

void update_infobot_slot_path_motion(Obj         *moby,
                                     Path_2d5988 *path,
                                     s32          arrival_state) __asm__("FUN_L00_002d5988");

void update_infobot_slot_path_motion(Obj *moby, Path_2d5988 *path, s32 arrival_state) {
    St         *st;
    s32         path_segment_index;
    f32         segment_offset_or_fraction;
    Vec4_2d5988 v;
    Vec4_2d5988 path_target_point;
    f32         distance_along_path;
    f32         w_;
    f32         current_speed;
    f32         speed_increment;
    f32         endpoint_distance_or_step_length;
    f32         target_yaw_or_movement_yaw_offset;
    f32         a;
    f32         b;

    st = moby->st;
    FUN_L00_0025df68_2d5988(path,
                            &moby->v[0],
                            &path_target_point,
                            &path_segment_index,
                            &segment_offset_or_fraction,
                            0,
                            999.0f,
                            5.0f,
                            0.0f);
    distance_along_path =
        convert_integer_to_float(path_segment_index) * path->a[0].f[3] + segment_offset_or_fraction;
    w_ = distance_along_path + 2.0f;
    endpoint_distance_or_step_length =
        fast_vec_dist(&moby->v[0], (void *)((char *)path + path->n * 16));
    speed_increment = frame_time_sq * 16.0f;
    current_speed = st->v60;
    if (endpoint_distance_or_step_length <=
        (current_speed * current_speed) / (speed_increment + speed_increment)) {
        f32 low = current_speed - speed_increment;
        f32 minimum_path_speed = infobot_slot_path_minimum_speed_gp * frame_time;
        st->v60 = low;
        if (low < minimum_path_speed)
            st->v60 = minimum_path_speed;
    } else {
        f32 high = current_speed + speed_increment;
        f32 maximum_path_speed = frame_time * 16.0f;
        st->v60 = high;
        if (maximum_path_speed < high)
            st->v60 = maximum_path_speed;
    }
    path_segment_index = truncate_float_to_s32(w_ / path->a[0].f[3]);
    segment_offset_or_fraction =
        (w_ - convert_integer_to_float(path_segment_index) * path->a[0].f[3]) / path->a[0].f[3];
    if (path_segment_index >= path->n - 1) {
        path_segment_index = path->n - 2;
        segment_offset_or_fraction = 1.0f;
    }
    fast_vec_sub(&path_target_point, &path->b[path_segment_index], &path->a[path_segment_index]);
    FUN_001f9a68_2d5988(&path_target_point, &path_target_point, segment_offset_or_fraction);
    fast_vec_add(&path_target_point, &path_target_point, &path->a[path_segment_index]);
    endpoint_distance_or_step_length =
        vector_distance_xy((void *)((char *)path + path->n * 16), &moby->v[0]);
    if (endpoint_distance_or_step_length < 1.0f) {
        if (st->v60 <= infobot_slot_path_minimum_speed_gp * frame_time) {
            moby->flag = arrival_state;
        }
    }
    if (path_segment_index + 1 < path->n - 8) {
        target_yaw_or_movement_yaw_offset =
            fast_arc_tan(path->a[path_segment_index + 1].f[0] - moby->v[0],
                         path->a[path_segment_index + 1].f[1] - moby->v[1]);
        moby->f48 =
            turn_f(&st->v6C, moby->f48, target_yaw_or_movement_yaw_offset, 0.01f, 0.3f, 0.1f);
    } else {
        target_yaw_or_movement_yaw_offset =
            fast_arc_tan(hero.motion.pos.f[0] - moby->v[0], hero.motion.pos.f[1] - moby->v[1]);
        moby->f48 =
            turn_f(&st->v6C, moby->f48, target_yaw_or_movement_yaw_offset, 0.01f, 0.3f, 0.1f);
    }
    fast_vec_sub(&v, &path_target_point, &moby->v[0]);
    endpoint_distance_or_step_length = FUN_001f9af0_2d5988(&v);
    if (st->v60 < endpoint_distance_or_step_length)
        endpoint_distance_or_step_length = st->v60;
    FUN_001f9bf8_2d5988(&v, &v, endpoint_distance_or_step_length);
    fast_vec_add(&moby->v[0], &moby->v[0], &v);
    target_yaw_or_movement_yaw_offset =
        fast_subtract_rotations(fast_arc_tan(v.f[0], v.f[1]), moby->f48);
    {
        f32 xa, xb;
        xa = st->v60 * 0.34906584f * FUN_001f9dc8_2d5988(target_yaw_or_movement_yaw_offset) /
             (frame_time * 16.0f);
        xb = st->v60 * -0.34906584f * FUN_001f9de0_2d5988(target_yaw_or_movement_yaw_offset) /
             (frame_time * 16.0f);
        {
            f32 updated_rotation_y, k70;
            updated_rotation_y = turn_f(&st->v68,
                                        moby->f44,
                                        xa,
                                        frame_time_sq * 0.52359879f,
                                        frame_time_sq * 1.04719758f,
                                        frame_time * 0.785398185f);
            k70 = frame_time_sq;
            moby->f44 = updated_rotation_y;
            moby->f40 = turn_f(&st->v64,
                               moby->f40,
                               xb,
                               k70 * 1.04719758f,
                               k70 * 2.09439516f,
                               frame_time * 1.57079637f);
        }
    }
}

/* The moby position, read and written as one quadword. */
#define MPOS(m) ((Vec *)&(m)->pos)

/* Infobot (class 750) update: flies the path set D_L00_001B04B0 between
   planets, then plays the unlock sequence. */

/* Store each segment's length in its start point's w. */

static inline void measure_path(Path *path) {
    s32 j;

    for (j = 0; j < path->count - 2; j++) {
        path->pts[j].v.w = fast_vec_dist(&path->pts[j], &path->pts[j + 1]);
    }
}

void infobot_update(struct Moby *moby) __asm__("FUN_L00_002d5de8");

/* Moby class 750 */
void infobot_update(struct Moby *moby) {
    enum {
        INFOBOT_INITIALIZE = 0,
        INFOBOT_IDLE = 1,
        INFOBOT_WAIT_FOR_PLAYER = 2,
        INFOBOT_WAIT_TO_FOLLOW_SLOT_PATH = 3,
        INFOBOT_FOLLOW_SLOT_PATH = 4,
        INFOBOT_ORBIT_CARRIER = 5,
        INFOBOT_FOLLOW_PATH = 6,
        INFOBOT_ORBIT_PATH_ENDPOINT = 7,
        INFOBOT_START_SCENE_SEQUENCE = 8,
        INFOBOT_WAIT_TO_DISPATCH_MSG1 = 9,
        INFOBOT_WAIT_TO_START_FINAL_SCENE = 10,
        INFOBOT_SAVE_AND_REMOVE = 11
    };
    Vec          offset;
    Vec          step;
    Vec          point;
    s32          segment;
    f32          fraction;
    Infobot     *infobot;
    Path        *path;
    Vec         *pos;
    struct Moby *carrier;
    Cutscene    *scene;
    s32          i;
    f32          range;
    f32          distance;
    f32          end;
    f32          path_speed_step;
    f32          speed;
    f32          heading;
    f32          tilt_y;
    f32          tilt_x;
    f32          max_speed;

    offset.q = 0;
    infobot = (Infobot *)moby->pvars;
    switch (moby->state) {
    case INFOBOT_INITIALIZE:
        moby->update_dist = 0xFF;
        infobot->yaw = moby->rot.z;
        infobot->home = MPOS(moby)->q;
        if (infobot->arrived != 0) {
            moby->state = 2;
            FUN_L00_002d6bf0_2d5de8(moby);
        } else {
            if (infobot->waiting != 0) {
                moby->state = 2;
            } else {
                moby->state = 1;
            }
            moby->flags |= 0x41;
        }
        if (infobot->attach != -1) {
            moby->state = 5;
            FUN_L00_002d6bf0_2d5de8(moby);
            break;
        }
        if (infobot->slots[0] == -1) {
            break;
        }
        for (i = 0; i < 8; i++) {
            if (infobot->slots[i] == -1) {
                continue;
            }
            measure_path(D_L00_001B04B0[infobot->slots[i]]);
        }
        MPOS(moby)->q = D_L00_001B04B0[infobot->slots[0]]->pts[0].q;
        infobot->seg = 0;
        moby->state = 3;
        break;
    case INFOBOT_IDLE:
        break;
    case INFOBOT_WAIT_FOR_PLAYER:
        range = 1.0f;
        if (infobot->waiting != 0 && (moby->flags & 1)) {
            range = 3.0f;
        }
        if (infobot->planet == 0xC) {
            range = 3.0f;
        }
        moby->rot.z = spring_turn(moby->rot.z, infobot->yaw, 0.01f, 0.3f, 0.1f, &infobot->spin[2]);
        moby->rot.y = spring_turn(moby->rot.y,
                                  0.0f,
                                  D_0015ED70 * 0.5235988f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED6C * 0.7853982f,
                                  &infobot->spin[1]);
        moby->rot.x = spring_turn(moby->rot.x,
                                  0.0f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED70 * 2.0943952f,
                                  D_0015ED6C * 1.5707964f,
                                  &infobot->spin[0]);
        FUN_L00_002d6cd0_2d5de8(moby);
        if (level_available[infobot->planet] != 0) {
            FUN_0020c828(moby);
            return;
        }
        if (fast_vec_dist(&D_0013F3D0, &moby->pos) < range && D_0013F3D0.active != 0) {
            moby->state = 8;
        }
        break;
    case INFOBOT_WAIT_TO_FOLLOW_SLOT_PATH:
        FUN_L00_002d6cd0_2d5de8(moby);
        if (level_available[infobot->planet] != 0) {
            FUN_0020c828(moby);
            return;
        }
        if (infobot->slots[infobot->seg] == -1) {
            moby->state = 2;
            break;
        }
        heading =
            fast_arc_tan(D_0013F350.target.v.x - moby->pos.x, D_0013F350.target.v.y - moby->pos.y);
        moby->rot.z = spring_turn(moby->rot.z, heading, 0.01f, 0.3f, 0.1f, &infobot->spin[2]);
        moby->rot.y = spring_turn(moby->rot.y,
                                  0.0f,
                                  D_0015ED70 * 0.5235988f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED6C * 0.7853982f,
                                  &infobot->spin[1]);
        moby->rot.x = spring_turn(moby->rot.x,
                                  0.0f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED70 * 2.0943952f,
                                  D_0015ED6C * 1.5707964f,
                                  &infobot->spin[0]);
        if (fast_vec_dist(&D_0013F350.target, &moby->pos) < 8.0f) {
            moby->state = 4;
            set_moby_dialogue_record_value(moby, 1);
        }
        break;
    case INFOBOT_FOLLOW_SLOT_PATH:
        FUN_L00_002d5988_2d5de8(moby, D_L00_001B04B0[infobot->slots[infobot->seg]], 3);
        FUN_L00_002d6cd0_2d5de8(moby);
        if (moby->state != 4) {
            infobot->seg++;
        }
        break;
    case INFOBOT_ORBIT_CARRIER:
        FUN_L00_002d6cd0_2d5de8(moby);
        if (level_available[infobot->planet] != 0) {
            FUN_0020c828(moby);
            return;
        }
        infobot->bob = fast_add_rotations(infobot->bob, D_0015ED6C * 3.1415927f);
        offset.v.x = fast_cos(infobot->bob);
        offset.v.y = fast_sin(infobot->bob);
        offset.v.z = 3.0f;
        moby->rot.z = fast_add_rotations(infobot->bob, 1.5707964f);
        if (infobot->carrier == -1) {
            break;
        }
        if (D_L00_0015FFD8[infobot->carrier].oclass != 0x336) {
            break;
        }
        if (D_L00_0015FFD8[infobot->carrier].command != 6) {
            break;
        }
        if (infobot->path == -1) {
            break;
        }
        moby->state = 6;
        infobot->attach = -1;
        MPOS(moby)->q = D_L00_001B04B0[infobot->path]->pts[0].q;
        moby->rot.z = fast_arc_tan(
            D_L00_001B04B0[infobot->path]->pts[1].v.x - D_L00_001B04B0[infobot->path]->pts[0].v.x,
            D_L00_001B04B0[infobot->path]->pts[1].v.y - D_L00_001B04B0[infobot->path]->pts[0].v.y);
        measure_path(D_L00_001B04B0[infobot->path]);
        break;
    case INFOBOT_FOLLOW_PATH:
        path = D_L00_001B04B0[infobot->path];
        pos = (Vec *)&moby->pos;
        FUN_L00_002d6cd0_2d5de8(moby);
        FUN_L00_0025df68(path, pos, &point, &segment, &fraction, 0, 999.0f, 5.0f, 0.0f);
        end = convert_integer_to_float(segment) * path->pts[0].v.w + fraction + 2.0f;
        distance = fast_vec_dist(pos, &path->pts[path->count - 1]);
        path_speed_step = D_0015ED70 * 16.0f;
        speed = infobot->speed;
        if (distance <= speed * speed / (path_speed_step + path_speed_step)) {
            infobot->speed = speed - path_speed_step;
            if (speed - path_speed_step < 0.0f) {
                infobot->speed = 0.0f;
            }
        } else {
            max_speed = D_0015ED6C * 16.0f;
            infobot->speed = speed + path_speed_step;
            if (max_speed < speed + path_speed_step) {
                infobot->speed = max_speed;
            }
        }
        segment = truncate_float_to_s32(end / path->pts[0].v.w);
        fraction = (end - convert_integer_to_float(segment) * path->pts[0].v.w) / path->pts[0].v.w;
        if (segment >= path->count - 1) {
            if (infobot->speed <= D_0015ED70 * 16.0f) {
                MPOS(moby)->q = ((Vec *)path)[path->count].q;
                moby->state = 7;
                break;
            }
            segment = path->count - 2;
            fraction = 1.0f;
        }
        fast_vec_sub(&point, &path->pts[segment + 1], &path->pts[segment]);
        fast_vec_scale(&point, &point, fraction);
        fast_vec_add(&point, &point, &path->pts[segment]);
        heading = fast_arc_tan(path->pts[segment + 1].v.x - path->pts[segment].v.x,
                               path->pts[segment + 1].v.y - path->pts[segment].v.y);
        moby->rot.z = spring_turn(moby->rot.z, heading, 0.01f, 0.3f, 0.1f, &infobot->spin[2]);
        pos = &step;
        fast_vec_sub(pos, &point, MPOS(moby));
        distance = fast_vec_length(pos);
        if (infobot->speed < distance) {
            distance = infobot->speed;
        }
        fast_vec_normalize(pos, pos, distance);
        fast_vec_add(MPOS(moby), MPOS(moby), pos);
        heading = fast_subtract_rotations(fast_arc_tan(step.v.x, step.v.y), moby->rot.z);
        tilt_y = infobot->speed * 0.34906584f * fast_cos(heading) / (D_0015ED6C * 16.0f);
        tilt_x = infobot->speed * -0.34906584f * fast_sin(heading) / (D_0015ED6C * 16.0f);
        moby->rot.y = spring_turn(moby->rot.y,
                                  tilt_y,
                                  D_0015ED70 * 0.5235988f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED6C * 0.7853982f,
                                  &infobot->spin[1]);
        moby->rot.x = spring_turn(moby->rot.x,
                                  tilt_x,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED70 * 2.0943952f,
                                  D_0015ED6C * 1.5707964f,
                                  &infobot->spin[0]);
        break;
    case INFOBOT_ORBIT_PATH_ENDPOINT: {
        Path *orbit_path = D_L00_001B04B0[infobot->path];
        FUN_L00_002d6cd0_2d5de8(moby);
        infobot->sway = infobot->sway + D_0015ED6C;
        if (1.0f < infobot->sway) {
            infobot->sway = 1.0f;
        }
        infobot->bob = fast_add_rotations(infobot->bob, D_0015ED6C * 3.1415927f);
        offset.v.x = fast_cos(infobot->bob) * infobot->sway;
        offset.v.y = fast_sin(infobot->bob) * infobot->sway;
        offset.v.z = 0.0f;
        heading = fast_add_rotations(infobot->bob, 1.5707964f);
        moby->rot.z = spring_turn(moby->rot.z,
                                  heading,
                                  D_0015ED6C * 25.132742f,
                                  D_0015ED6C * 25.132742f,
                                  D_0015ED6C * 12.566371f,
                                  &infobot->spin[2]);
        fast_vec_add(&step, &offset, &orbit_path->pts[orbit_path->count - 1]);
        MPOS(moby)->q = step.q;
        if (fast_vec_dist(&D_0013F3D0, &orbit_path->pts[orbit_path->count - 1]) < 2.0f &&
            D_0013F3D0.active != 0) {
            moby->state = 8;
        }
        moby->rot.y = spring_turn(moby->rot.y,
                                  0.0f,
                                  D_0015ED70 * 0.5235988f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED6C * 0.7853982f,
                                  &infobot->spin[1]);
        moby->rot.x = spring_turn(moby->rot.x,
                                  0.0f,
                                  D_0015ED70 * 1.0471976f,
                                  D_0015ED70 * 2.0943952f,
                                  D_0015ED6C * 1.5707964f,
                                  &infobot->spin[0]);
        break;
    }
    case INFOBOT_START_SCENE_SEQUENCE:
        moby->flags |= 0x41;
        unlock_planet(infobot->planet);
        jet_clear_persistent(moby->mission);
        if (infobot->cutscene != -1) {
            jet_reset_camera(&D_L00_001600EC_2d5de8[infobot->cutscene].pos,
                             &D_L00_001600EC_2d5de8[infobot->cutscene].dir);
        }
        moby->state = 9;
        if (infobot->msg0 != -1) {
            start_scene(infobot->msg0);
        }
        break;
    case INFOBOT_WAIT_TO_DISPATCH_MSG1:
        if (D_L00_0015F5C4 == 2) {
            break;
        }
        moby->state = 10;
        if (infobot->msg1 != -1) {
            queue_scene_movie_with_game_language(infobot->msg1);
        }
        break;
    case INFOBOT_WAIT_TO_START_FINAL_SCENE:
        if (D_L00_0015F5C4 == 2) {
            break;
        }
        if (infobot->msg2 != -1) {
            start_scene(infobot->msg2);
        }
        moby->state = 11;
        break;
    case INFOBOT_SAVE_AND_REMOVE:
        if (D_L00_0015F5C4 == 2) {
            return;
        }
        memcard_save_data(0, -1);
        queue_planet_unlock_message(infobot->planet);
        FUN_0020c828(moby);
        return;
    }
    if (infobot->attach != -1 && infobot->carrier != -1) {
        carrier = &D_L00_0015FFD8[infobot->carrier];
        if (carrier->oclass == 0x336) {
            step.q = 0;
            FUN_L00_00260400(moby,
                             carrier,
                             &D_L00_001600EC_2d5de8[infobot->attach].pos,
                             &step,
                             &D_L00_001600EC_2d5de8[infobot->attach].pos,
                             &step);
            fast_vec_add(&point, &offset, &D_L00_001600EC_2d5de8[infobot->attach].pos);
            MPOS(moby)->q = point.q;
        }
    }
}

void initialize_moby_particle_effect(O002d6bf0 *moby) __asm__("FUN_L00_002d6bf0");

void initialize_moby_particle_effect(O002d6bf0 *moby) {
    M002d6bf0 *m = moby->m;
    s32        i;
    for (i = 0; i < 4; i++)
        m->r[i] = random_integer_below(0xFF);
    m->a[0] = -1.0f;
    m->a[1] = -2.25f;
    m->a[2] = 1.25f;
    m->a[3] = 2.5f;
    for (i = 0; i < 4; i++)
        m->c[i] = scale_game_frames(i * 0x40 + 0x3F);
    m->x40 = 2.5f;
    m->x44 = 3.0f;
    m->x4c = 2.5f;
    m->x48 = 3.0f;
}

void update_infobot_particle_effect(char *moby) __asm__("FUN_L00_002d6cd0");

void update_infobot_particle_effect(char *moby) {
    P_2d6cd0 *infobot_particle_state = ((P_2d6cd0 *)MOBY(moby)->pvars);
    union {
        OvlQuad q;
        float   f[4];
    } v;
    float d[4] __attribute__((aligned(16)));
    float particle_position_step[4] __attribute__((aligned(16)));
    int   i, col;
    float f;

    if (MOBY(moby)->flags & 1) {
        return;
    }
    v.q = *(OvlQuad *)&MOBY(moby)->pos;
    v.f[2] += 0.333000004f;
    fast_vec_sub(d, g_debug_camera.position, moby + 0x10);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(particle_position_step, d, 0.1f);
    fast_vec_add(d, d, moby + 0x10);
    v.f[2] -= 0.333000004f;
    for (i = 0; i < 4; i++) {
        {
            float unwrapped_particle_byte_value =
                infobot_particle_state->a[i] + infobot_particle_state->b[i];
            infobot_particle_state->a[i] = unwrapped_particle_byte_value;
            if (255.0f <= unwrapped_particle_byte_value) {
                infobot_particle_state->a[i] = unwrapped_particle_byte_value - 255.0f;
            } else if (unwrapped_particle_byte_value <= 0.0f) {
                infobot_particle_state->a[i] = unwrapped_particle_byte_value + 255.0f;
            }
        }
        if (fast_dec_timer(&infobot_particle_state->c[i])) {
            infobot_particle_state->c[i] = scale_game_frames(0xFF);
        }
        f = convert_integer_to_float(scale_game_frames(0xFF) - infobot_particle_state->c[i]);
        f = f / (float)scale_game_frames(0xFF);
        col = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - f));
        if (D_15F5C4_2d6cd0 != 2) {
            FUN_L00_00272f68(d,
                             col,
                             truncate_float_to_s32(infobot_particle_state->a[i]) & 0xFF,
                             0x35,
                             1,
                             2,
                             0,
                             infobot_particle_state->d[i]);
        }
        fast_vec_add(d, d, particle_position_step);
    }
}
