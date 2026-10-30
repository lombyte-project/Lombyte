/* Level 08 batalia bolt pickup: update, surface pose and rotating quad (no caller or class table entry found). */
#include "types.h"
#include "rnc/game/bolt_sources.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

/* finds the first of eight linked entries whose direction lines up with the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E15A0), where it is exact; names translated to the US level program. */
extern char D_L08_001675C0[];

#include "qcopy.h"

/* builds a basis from a direction vector and writes the orientation into the moby */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E6130), where it is exact; names translated to the US level program. */
void set_batalia_bolt_pickup_surface_rotation(struct Moby *moby,
                                              float *direction_vector) __asm__("FUN_L08_002e4db8");

#include "rnc/overlay/l08/mobys/batalia_bolt_pickup_globals.h"
#include "rnc/hero/hero.h"

extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern int   D_L08_0015F5CC __asm__("D_L08_0015F5CC");
extern u8    D_0014C050[];
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

typedef int u128_4BE8_p __attribute__((mode(TI)));

typedef struct {
    float anchor[4];              /* 0x00 */
    float v10[4];                 /* 0x10 */
    float v20[4];                 /* 0x20 */
    float v30[4];                 /* 0x30 */
    float velocity[4];            /* 0x40 */
    float spin_x, spin_y, spin_z; /* 0x50 */
    int   pad5C;
    float f60, f64; /* 0x60 */
    int   timer;    /* 0x68 */
    short s6C, s6E; /* 0x6C */
    union {
        int   i;
        float f;
    } u70;
    float f74; /* 0x74 */
    union {
        int   i;
        char *p;
    } u78;
    union {
        int   i;
        float f;
    } u7C;
} Vars_4BE8;

typedef struct {
    char  pad0[0x18];
    int   moby; /* 0x18 */
    char  pad1C[4];
    float pos[4]; /* 0x20 */
    char  pad30[0x10];
    float n[4]; /* 0x40 */
} Coll_4BE8;

typedef struct {
    u8 pad_00_p[0xB1];
    u8 id;
} BoltSource_4BE8;

typedef struct {
    unsigned char pad[0x454];
    unsigned char killed[1];
} KillFlags_4BE8;

typedef struct {
    float pos[4]; /* 0x00 */
    char  pad10[0x10];
    float n[4]; /* 0x20 */
} CollPos_4BE8;

extern float          D_L08_00174660[];
extern KillFlags_4BE8 l08_bolt_pickup_collection_marks_by_save_id __asm__("D_L08_001BADD0");
extern KillFlags_4BE8
    l08_bolt_pickup_mission_gated_collection_marks_by_save_id __asm__("D_L08_001BBA30");
extern struct Moby *D_L08_0015FFDC;
extern float l08_bolt_pickup_collection_target_speed __asm__("D_L08_00161C88") __attribute__((sda));
extern float l08_bolt_pickup_collection_acceleration __asm__("D_L08_00161C8C") __attribute__((sda));
extern float l08_bolt_pickup_collection_launch_spin_speed_degrees __asm__("D_L08_00161C94")
    __attribute__((sda));
extern float l08_bolt_pickup_detached_fall_spin_speed_degrees __asm__("D_L08_00161C98")
    __attribute__((sda));
extern float l08_bolt_pickup_default_surface_roll_speed_degrees __asm__("D_L08_00161CA0")
    __attribute__((sda));
extern float l08_five_bolt_pickup_surface_orbit_speed_degrees __asm__("D_L08_00161CA4")
    __attribute__((sda));
extern float l08_ten_bolt_pickup_surface_orbit_speed_degrees __asm__("D_L08_00161CAC")
    __attribute__((sda));
extern float D_L08_00161CB4_gp __asm__("D_L08_00161CB4") __attribute__((sda));
extern float D_L08_00161CB8;
extern float D_L08_00161CBC_gp __asm__("D_L08_00161CBC") __attribute__((sda));
extern float l08_bolt_pickup_surface_return_base_duration __asm__("D_L08_00161CD0")
    __attribute__((sda));
extern float FUN_L04_001f1978(float);
extern float step_angle_toward_target(float *, float, float, int) __asm__("FUN_L00_0025bc00");
extern int   l08_bolt_pickup_collection_scale_fade_enabled __asm__("D_L08_00161CC4")
    __attribute__((sda));
extern int l08_bolt_pickup_initial_rotating_quad_color __asm__("D_L08_00161CCC")
    __attribute__((sda));
extern int l08_bolt_pickup_last_collection_sound_frame __asm__("D_L08_00161CD4")
    __attribute__((sda));
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int
transform_attachment_pose(void *, int, void *, void *, void *, void *) __asm__("FUN_L00_002604f0");
extern int
queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
extern s32            level_bolts[] __asm__("D_0013DF38");
extern unsigned char *FUN_L00_002603d0(int);
extern unsigned char *FUN_L00_00272060(
    float *pos, int a, float f0, float f1, int col, int mode, int b, float *vel, float f2);
extern unsigned char D_0014BEC0[][4];
extern unsigned char D_L08_0015FC88_macro[] __asm__("D_L08_0015FC88") MACRO_ADDR;
extern void          FUN_L00_0025f730_c(void *a, float x) __asm__("FUN_L00_0025f730");
extern void convert_pose_to_reference_local(void *, int, void *, void *, void *, void *) __asm__("FUN_L00_00260668");
extern void build_axis_roll_matrix(void *, void *, float) __asm__("FUN_L08_0026a0f8");
extern void FUN_L08_002e4e90_c(char *m, float *out, float *rot) __asm__("FUN_L08_002e4e90");
extern void FUN_L08_002e5188_c(char *m) __asm__("FUN_L08_002e5188");
extern char D_0013F3D0[];
extern char D_0013F490[];
extern void func_L00_0023A658_4BE8(void) __asm__("FUN_L08_002444a8");
extern void func_L00_0023A690_4BE8(void) __asm__("FUN_L08_002444e0");
extern void func_L00_0023A788_4BE8(void) __asm__("FUN_L08_002445d8");
extern int  D_L08_00174658_lui __asm__("D_L08_00174658") NOT_SDA;
void        update_batalia_bolt_pickup(struct Moby *moby) __asm__("FUN_L08_002e3870");

/* aims a turret: builds its barrel frame from the base rotation, yaw and pitch, writes the barrel rotation
 * and the muzzle point (offset per turret class) */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002E0258.c: func_L08_002E6208), where it is exact; names translated to the US level program. */
extern float bolt_pickup_default_surface_base_tilt_degrees_gp __asm__("D_L08_00161C64")
    __attribute__((sda));
extern float bolt_pickup_default_surface_planar_offset_per_scale_gp __asm__("D_L08_00161C7C")
    __attribute__((sda));
extern float five_bolt_pickup_surface_planar_offset_per_scale_gp __asm__("D_L08_00161C80")
    __attribute__((sda));
extern float ten_bolt_pickup_surface_normal_offset_per_scale_gp __asm__("D_L08_00161C78")
    __attribute__((sda));
extern float D_L08_00161CB8 __attribute__((sda));
extern float D_L08_00161CBC __attribute__((sda));
extern float ten_bolt_pickup_surface_planar_offset_per_scale_gp __asm__("D_L08_00161C84")
    __attribute__((sda));
extern float five_bolt_pickup_surface_base_tilt_degrees_gp __asm__("D_L08_00161C68")
    __attribute__((sda));
extern float ten_bolt_pickup_surface_base_tilt_degrees_gp __asm__("D_L08_00161C6C")
    __attribute__((sda));
extern float D_L08_00161CB4 __attribute__((sda));
extern float bolt_pickup_default_surface_normal_offset_per_scale_gp __asm__("D_L08_00161C70")
    __attribute__((sda));
extern float five_bolt_pickup_surface_normal_offset_per_scale_gp __asm__("D_L08_00161C74")
    __attribute__((sda));
void compute_batalia_bolt_pickup_surface_pose(
    struct Moby *moby, float *out, float *output_euler_angles) __asm__("FUN_L08_002e4e90");

typedef struct {
    s64 vertices[4];
    u32 colors[4];
    u32 texture_coordinates[4];
} Quad_5188;

extern void draw_geometry_quad_5188(void *, void *, void *, u64, int) __asm__("FUN_L02_0020bc88");

#include "rnc/overlay/l08/mobys/batalia_bolt_pickup_data/draw_batalia_bolt_pickup_rotating_quad.h"

extern char D_L08_0016D240[] NOT_SDA;
void        draw_batalia_bolt_pickup_rotating_quad(struct Moby *moby) __asm__("FUN_L08_002e5188");

void update_batalia_bolt_pickup(struct Moby *moby) {
    enum {
        BOLT_PICKUP_INITIALIZE = 0,
        BOLT_PICKUP_WAIT_FOR_HERO = 1,
        BOLT_PICKUP_TURN_TOWARD_HERO = 2,
        BOLT_PICKUP_FLY_TO_HERO_AND_CREDIT_BOLTS = 3,
        BOLT_PICKUP_FALL_AND_BOUNCE = 4,
        BOLT_PICKUP_RETURN_TO_SURFACE = 5
    };

    char *collection_motion_reference;
    float direction[4];
    float point[4];
    float local[4];
    float scratch[16];

    switch (moby->state) {
    case BOLT_PICKUP_INITIALIZE: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
        int        linked;

        switch (moby->oclass) {
        case MOBY_CLASS_ONE_BOLT_PICKUP:
            moby->scale = moby->pclass->scale * D_L08_00161CB4_gp;
            break;
        case MOBY_CLASS_FIVE_BOLT_PICKUP:
            moby->scale = moby->pclass->scale * D_L08_00161CB8;
            break;
        case MOBY_CLASS_TEN_BOLT_PICKUP:
            moby->scale = moby->pclass->scale * D_L08_00161CBC_gp;
            break;
        }
        linked = 0;
        if (l08_bolt_pickup_last_collection_sound_frame > D_L08_0015F5CC) {
            l08_bolt_pickup_last_collection_sound_frame = D_L08_0015F5CC;
        }
        moby->flags &= ~0x100;
        qcopy(vars->anchor, &moby->pos);
        if (vars->s6E == 0) {
            fast_vec_normalize(point, &moby->rot_row2, 0.1f);
            fast_vec_add(point, point, &moby->pos);
            fast_vec_scale(local, &moby->rot_row2, -2.0f);
            fast_vec_add(local, local, &moby->pos);
            if (collision_line(point, local, 2, 0, 0) != 0) {
                CollPos_4BE8 *hit = (CollPos_4BE8 *)D_L08_00174660;
                qcopy(vars->anchor, hit->pos);
                linked = ((int *)hit)[-2];
                fast_vec_normalize(direction, hit->n, 1.0f);
            } else {
                qcopy(vars->anchor, &moby->pos);
                clear_u64_value(direction);
                direction[2] = 1.0f;
            }
        }
        set_batalia_bolt_pickup_surface_rotation((char *)moby, direction);
        vars->f60 = random_angle_radians();
        vars->f64 = moby->rot.x;
        FUN_L08_002e4e90_c((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        if (linked != 0 && FUN_L00_002603d0(linked) != 0) {
            vars->u7C.i = linked;
            convert_pose_to_reference_local(
                moby, linked, vars->anchor, vars->v10, vars->v20, vars->v30);
        }
        moby->state = 1;
        vars->s6C = 0;
        vars->u70.i =
            truncate_float_to_s32(random_float_between(0.0f, (float)scale_game_frames(600)));
        vars->u78.i = l08_bolt_pickup_initial_rotating_quad_color;
        break;
    }
    case BOLT_PICKUP_WAIT_FOR_HERO: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        if (vars->u7C.i != 0) {
            transform_attachment_pose(
                moby, vars->u7C.i, vars->v20, vars->v30, vars->anchor, vars->v10);
            if ((signed char)((struct Moby *)vars->u7C.i)->state < 0) {
                vars = (Vars_4BE8 *)moby->pvars;
                clear_u64_value(vars->velocity);
                build_axis_roll_matrix(vars,
                                       &moby->rot_row1,
                                       l08_bolt_pickup_detached_fall_spin_speed_degrees *
                                           0.017453292f * frame_time);
                moby->state = 4;
                break;
            }
        }
        if (absolute_float(moby->pos.z - hero.motion.pos.f[2]) < hero.reach_distance &&
            vector_distance_xy(&moby->pos, hero.motion.pos.f) < hero.reach_height) {
            vars->timer = scale_game_frames(1);
            moby->state = 2;
        }
        if (moby->visible == 0) {
            if (moby > D_L08_0015FFDC &&
                vector_distance_xy(&moby->pos, ((char *)&D_0013F3D0)) > 24.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            break;
        }
        switch (moby->oclass) {
        case MOBY_CLASS_TEN_BOLT_PICKUP:
            vars->f60 = fast_add_rotations(vars->f60,
                                           l08_ten_bolt_pickup_surface_orbit_speed_degrees *
                                               0.017453292f * frame_time);
            vars->f64 = fast_add_rotations(
                vars->f64, ten_bolt_pickup_surface_roll_speed_degrees * 0.017453292f * frame_time);
            break;
        case MOBY_CLASS_FIVE_BOLT_PICKUP:
            vars->f60 = fast_add_rotations(vars->f60,
                                           l08_five_bolt_pickup_surface_orbit_speed_degrees *
                                               0.017453292f * frame_time);
            vars->f64 = fast_add_rotations(
                vars->f64, five_bolt_pickup_surface_roll_speed_degrees * 0.017453292f * frame_time);
            break;
        default:
            vars->f60 = fast_add_rotations(vars->f60,
                                           bolt_pickup_default_surface_orbit_speed_degrees *
                                               0.017453292f * frame_time);
            vars->f64 = fast_add_rotations(vars->f64,
                                           l08_bolt_pickup_default_surface_roll_speed_degrees *
                                               0.017453292f * frame_time);
            break;
        }
        FUN_L08_002e4e90_c((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        if (vars->s6C != 0) {
            vars->f74 = fast_add_rotations(vars->f74, frame_time * 5.2359877f);
            if (vars->u70.i > 12) {
                vars->u78.i += 0x5000000;
            } else {
                vars->u78.i -= 0x5000000;
            }
            if (fast_dec_timer(&vars->u70.i) != 0) {
                vars->s6C = 0;
                vars->u70.i = truncate_float_to_s32(random_float_between(
                    (float)scale_game_frames(300), (float)scale_game_frames(600)));
            } else if (moby->visible != 0 && bolt_pickup_rotating_quad_enabled != 0) {
                add_draw_callback(FUN_L08_002e5188_c, moby);
                return;
            }
        } else if (fast_dec_timer(&vars->u70.i) != 0) {
            vars->u70.i = 24;
            vars->f74 = random_angle_radians();
            vars->s6C = 1;
        }
        break;
    }
    case BOLT_PICKUP_TURN_TOWARD_HERO: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        euler_rotation_matrix(scratch, vars->v10);
        qcopy(direction, scratch + 8);
        if (fast_dec_timer(&vars->timer) != 0) {
            Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
            float      deg_to_rad = 0.017453292f;
            char      *hero_pos = D_0013F3D0;

            fast_vec_sub(point, &moby->pos, hero_pos);
            fast_vec_normalize(point, point, 1.0f);
            fast_vec_cross(point, direction, point);
            build_axis_roll_matrix(vars,
                                   point,
                                   l08_bolt_pickup_collection_launch_spin_speed_degrees *
                                       deg_to_rad * frame_time);
            moby->flags |= 0x100;
            fast_vec_normalize(vars->velocity, direction, frame_time * 8.0f);
            vars->f74 = 0.0f;
            vars->u70.f = frame_time * 8.0f;
            vars->u78.p = *(char **)(hero_pos + 0x2000);
            vars->u7C.f = 0.5f;
            if (l08_bolt_pickup_last_collection_sound_frame + scale_game_frames(3) <
                D_L08_0015F5CC) {
                l08_bolt_pickup_last_collection_sound_frame = D_L08_0015F5CC;
                allocate_voice_for_target_entry(0, 0x20, moby);
                vars->f64 = 0.0f;
            } else {
                vars->f64 = random_float_between(-30.0f, 30.0f) * deg_to_rad;
            }
            moby->state = 3;
        } else {
            if (scale_game_frames(10) < vars->timer) {
                float goal_yaw = fast_add_rotations(
                    vars->f60, fast_sin((float)(vars->timer % 8 - 4) * 0.7854f) * 0.17453292f);

                step_angle_toward_target(&vars->f60, goal_yaw, 0.5235988f, 0);
            } else {
                float goal_yaw =
                    fast_add_rotations(fast_arc_tan(hero.motion.pos.f[0] - moby->pos.x,
                                                    hero.motion.pos.f[1] - moby->pos.y),
                                       1.5707964f);

                step_angle_toward_target(&vars->f60, goal_yaw, 0.08726646f, 0);
                if (fast_difference_between_rotations(vars->f60, goal_yaw) < 0.08726646f) {
                    vars->timer = 0;
                }
            }
            FUN_L08_002e4e90_c((char *)moby, (float *)&moby->pos, (float *)&moby->rot);
        }
        break;
    }
    case BOLT_PICKUP_FLY_TO_HERO_AND_CREDIT_BOLTS: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;
        float      collection_target_distance;
        float      fade_range;

        if (vars->u78.p == 0) {
            break;
        }
        fast_vec_normalize(point, vars->u78.p + 0xE0, vars->u7C.f);
        fast_vec_add(point, point, vars->u78.p + 0x10);
        fast_vec_sub(direction, point, &moby->pos);
        if (vars->f64 != 0.0f) {
            float sine = fast_sin(vars->f64);
            float cosine = fast_cos(vars->f64);

            direction[0] = cosine * direction[0] + sine * direction[1];
            direction[1] = cosine * direction[1] - sine * direction[0];
        }
        collection_target_distance = fast_vec_dist(point, &moby->pos);
        fast_vec_normalize(vars->velocity, vars->velocity, vars->u70.f);
        fast_vec_add(&moby->pos, &moby->pos, vars->velocity);
        fast_vec_normalize(direction, direction, vars->f74);
        fast_vec_add(&moby->pos, &moby->pos, direction);
        collection_motion_reference = D_0013F490;
        fast_vec_add(&moby->pos, &moby->pos, collection_motion_reference);
        fast_vec_sub(local, &moby->pos, collection_motion_reference + 0x160);
        transform_vector(local, local, collection_motion_reference - 0x100);
        if (((struct Moby *)vars->u78.p)->oclass != 0 || local[2] > 0.0f) {
            approach_value(
                &vars->u70.f, 0.0f, l08_bolt_pickup_collection_acceleration * frame_time_sq);
            approach_value(&vars->f74,
                           l08_bolt_pickup_collection_target_speed * frame_time,
                           l08_bolt_pickup_collection_acceleration * frame_time_sq);
        } else {
            fast_vec_normalize(scratch, collection_motion_reference - 0x120, vars->u70.f);
            fast_vec_sub(scratch + 4, scratch, vars->velocity);
            FUN_L00_0025f730_c(scratch + 4,
                               l08_bolt_pickup_collection_acceleration * frame_time_sq);
            fast_vec_add(vars->velocity, vars->velocity, scratch + 4);
        }
        if (collection_target_distance < vars->f74) {
            vars->f74 = collection_target_distance;
        }
        fade_range = vars->f74 * (float)scale_game_frames(20);
        if (collection_target_distance < fade_range) {
            if (fast_vec_dist(point, &moby->pos) > 0.52f) {
                if (bolt_pickup_collection_alpha_fade_enabled != 0) {
                    moby->alpha = (fast_vec_dist(point, &moby->pos) + fade_range) * 128.0f /
                                  (fade_range + fade_range);
                }
                if (l08_bolt_pickup_collection_scale_fade_enabled != 0) {
                    moby->scale = moby->pclass->scale * D_L08_00161CB4_gp *
                                  (fast_vec_dist(point, &moby->pos) + fade_range) /
                                  (fade_range + fade_range);
                }
            } else {
                int              value;
                int              spin;
                float            shrink;
                float            size;
                BoltSource_4BE8 *source;

                switch (moby->oclass) {
                case MOBY_CLASS_COLLECTED_COUNT_BOLT_BONUS_PICKUP: {
                    int i;
                    int j;
                    int n;

                    D_0014BEC0[current_level_index][moby->command] = 1;
                    n = 0;
                    for (i = 0; i < 20; i++) {
                        for (j = 0; j < 4; j++) {
                            if (D_0014BEC0[i][j] != 0) {
                                n++;
                            }
                        }
                    }
                    value = n * 100;
                    break;
                }
                case MOBY_CLASS_TEN_BOLT_PICKUP:
                    value = 10;
                    break;
                case MOBY_CLASS_FIVE_BOLT_PICKUP:
                    value = 5;
                    break;
                default:
                    value = 1;
                    break;
                }
                shrink = 0.7f;
                random_vector(scratch, frame_time * shrink, frame_time);
                fast_vec_scale(scratch + 4, scratch, 10.0f);
                fast_vec_add(scratch + 4, scratch + 4, &moby->pos);
                spin = random_integer_below(2);
                if (spin == 0) {
                    spin--;
                }
                size = random_float_between(0.4f, 0.5f);
                FUN_L00_00272060(scratch + 4,
                                 scale_game_frames(0x19),
                                 size * 0.2f,
                                 size,
                                 0x7F207F7F,
                                 0,
                                 spin,
                                 scratch,
                                 0.0f);
                spin = -spin;
                FUN_L00_00272060(scratch + 4,
                                 scale_game_frames(0x19),
                                 size * 0.14f,
                                 size * shrink,
                                 0x7F7F7F7F,
                                 1,
                                 spin,
                                 scratch,
                                 0.0f);
                source = moby->parent;
                if (source && !(source->id & 0x80)) {
                    bolt_source_stats[current_level_index][source->id].bolts += value;
                }
                current_bolt_count += value;
                level_bolts[current_level_index] += value;
                queue_animation_update(2,
                                       0x754E,
                                       func_L00_0023A658_4BE8,
                                       func_L00_0023A690_4BE8,
                                       func_L00_0023A788_4BE8,
                                       &current_bolt_count,
                                       9999999);
                if (moby < D_L08_0015FFDC) {
                    l08_bolt_pickup_collection_marks_by_save_id.killed[(short)moby->save_id] =
                        moby->mission + 2;
                    if (moby->mission == 0xFF ||
                        (D_L08_0015FC88_macro[moby->mission] != 0xFF &&
                         D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
                        l08_bolt_pickup_mission_gated_collection_marks_by_save_id
                            .killed[(short)moby->save_id] = moby->mission + 2;
                    }
                }
                mark_moby_for_removal(moby);
                return;
            }
        }
        multiply_matrix3x3(&moby->rot_row0, vars, &moby->rot_row0);
        break;
    }
    case BOLT_PICKUP_FALL_AND_BOUNCE: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        fast_vec_add(&moby->pos, &moby->pos, vars->velocity);
        vars->velocity[2] -= frame_time_sq * 10.8f;
        multiply_matrix3x3(&moby->rot_row0, vars, &moby->rot_row0);
        if (moby->pos.z < 2.0f || moby->pos.x > 510.0f || moby->pos.x < 2.0f ||
            moby->pos.y > 510.0f || moby->pos.y < 2.0f) {
            int              value;
            BoltSource_4BE8 *source;

            switch (moby->oclass) {
            case MOBY_CLASS_TEN_BOLT_PICKUP:
                value = 10;
                break;
            case MOBY_CLASS_FIVE_BOLT_PICKUP:
                value = 5;
                break;
            default:
                value = 1;
                break;
            }
            source = moby->parent;
            if (source && !(source->id & 0x80)) {
                bolt_source_stats[current_level_index][source->id].bolts += value;
            }
            current_bolt_count += value;
            level_bolts[current_level_index] += value;
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->pos.z < vars->f60 - 4.0f && moby->pos.z < hero.motion.pos.f[2] - 4.0f) {
            Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

            *(u128_4BE8_p *)vars->v10 = *(u128_4BE8_p *)((char *)&hero + 0x90);
            vars->timer = scale_game_frames(1);
            moby->state = 2;
            break;
        }
        if (coll_sphere(0.25f, &moby->pos, 0x22, moby) != 0) {
            fast_vec_sub(direction, &moby->pos, ((CollPos_4BE8 *)D_L08_00174660)->pos);
            reflect_vector(vars->velocity, vars->velocity, direction);
            if (fast_arc_tan(direction[2], vector_length_xy(direction)) < 0.7853982f &&
                fast_arc_tan(((Coll_4BE8 *)((char *)D_L08_00174660 - 0x20))->n[2],
                             vector_length_xy(((CollPos_4BE8 *)D_L08_00174660)->n)) < 0.7853982f) {
                Coll_4BE8 *coll = (Coll_4BE8 *)((char *)D_L08_00174660 - 0x20);
                int        frames;
                float      per_frame;

                moby->flags &= ~0x100;
                vars = (Vars_4BE8 *)moby->pvars;
                frames = truncate_float_to_s32(
                    FUN_L04_001f1978(l08_bolt_pickup_surface_return_base_duration +
                                     random_float_between(-10.0f, 10.0f)));
                per_frame = 1.0f / convert_integer_to_float(frames);
                qcopy(vars->anchor, coll->pos);
                matrix_to_euler(&moby->rot_row0, &moby->rot);
                fast_vec_normalize(direction, coll->n, 1.0f);
                set_batalia_bolt_pickup_surface_rotation((char *)moby, direction);
                vars->f60 = random_angle_radians();
                vars->f64 = random_angle_radians();
                FUN_L08_002e4e90_c((char *)moby, vars->v20, vars->v30);
                fast_vec_sub(vars->velocity, vars->v20, &moby->pos);
                fast_vec_scale(vars->velocity, vars->velocity, per_frame);
                if (coll->moby != 0) {
                    unsigned char *carrier = FUN_L00_002603d0(coll->moby);

                    if (carrier != 0) {
                        fast_vec_add(vars->velocity, vars->velocity, carrier + 0x10);
                        fast_vec_scale(point, carrier + 0x10, convert_integer_to_float(frames));
                        fast_vec_add(vars->v20, vars->v20, point);
                        fast_vec_add(vars->anchor, vars->anchor, point);
                    }
                }
                vars->velocity[2] += frame_time_sq * 10.8f * (float)(frames >> 1);
                vars->spin_x = fast_subtract_rotations(vars->v30[0], moby->rot.x) * per_frame;
                vars->spin_y = fast_subtract_rotations(vars->v30[1], moby->rot.y) * per_frame;
                vars->spin_z = fast_subtract_rotations(vars->v30[2], moby->rot.z) * per_frame;
                moby->state = 5;
                vars->timer = frames;
                vars->u7C.i = D_L08_00174658_lui;
                if (absolute_float(moby->pos.z - hero.motion.pos.f[2]) < hero.reach_distance &&
                    vector_distance_xy(&moby->pos, hero.motion.pos.f) < hero.reach_height) {
                    Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

                    vars->timer = scale_game_frames(1);
                    moby->state = 2;
                }
            }
        }
        break;
    }
    case BOLT_PICKUP_RETURN_TO_SURFACE: {
        Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

        moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin_x);
        moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin_y);
        moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin_z);
        fast_vec_add(&moby->pos, &moby->pos, vars->velocity);
        vars->velocity[2] -= frame_time_sq * 10.8f;
        if (fast_dec_timer(&vars->timer) != 0) {
            int   frames;
            float per_frame;

            if (absolute_float(moby->pos.z - hero.motion.pos.f[2]) < hero.reach_distance &&
                vector_distance_xy(&moby->pos, hero.motion.pos.f) < hero.reach_height) {
                vars->timer = scale_game_frames(1);
                moby->state = 2;
                break;
            }
            if (moby->command != 0) {
                moby->state = 1;
                if (vars->u7C.i != 0) {
                    Vars_4BE8 *vars = (Vars_4BE8 *)moby->pvars;

                    convert_pose_to_reference_local(
                        moby, vars->u7C.i, vars->anchor, vars->v10, vars->v20, vars->v30);
                } else {
                    qcopy(&moby->pos, vars->v20);
                    qcopy(&moby->rot, vars->v30);
                }
                break;
            }
            frames = truncate_float_to_s32((float)scale_game_frames(20));
            per_frame = 1.0f / convert_integer_to_float(frames);
            moby->command++;
            vars->spin_x = fast_subtract_rotations(vars->v30[0], moby->rot.x) * per_frame;
            vars->spin_y = fast_subtract_rotations(vars->v30[1], moby->rot.y) * per_frame;
            vars->spin_z = fast_subtract_rotations(vars->v30[2], moby->rot.z) * per_frame;
            fast_vec_sub(vars->velocity, vars->v20, &moby->pos);
            fast_vec_scale(vars->velocity, vars->velocity, per_frame);
            vars->timer = frames;
            if (vars->u7C.i != 0) {
                unsigned char *carrier = FUN_L00_002603d0(vars->u7C.i);

                if (carrier != 0) {
                    fast_vec_add(vars->velocity, vars->velocity, carrier + 0x10);
                    fast_vec_scale(direction, carrier + 0x10, convert_integer_to_float(frames));
                    fast_vec_add(vars->v20, vars->v20, direction);
                    fast_vec_add(vars->anchor, vars->anchor, direction);
                }
            }
            vars->velocity[2] += frame_time_sq * 10.8f * (float)(frames >> 1);
        }
        break;
    }
    }
}

void set_batalia_bolt_pickup_surface_rotation(struct Moby *moby, float *direction_vector) {
    float direction_basis_matrix[16];
    float reconstructed_rotation_matrix[16];
    float first_basis_vector[4];
    float second_basis_vector[4];
    char *pvars_cursor = (char *)moby->pvars;
    first_basis_vector[1] = direction_vector[2];
    first_basis_vector[2] = direction_vector[0];
    first_basis_vector[0] = -direction_vector[1];
    fast_vec_cross(first_basis_vector, direction_vector, first_basis_vector);
    fast_vec_cross(second_basis_vector, first_basis_vector, direction_vector);
    qcopy(direction_basis_matrix, first_basis_vector);
    qcopy(direction_basis_matrix + 4, second_basis_vector);
    qcopy(direction_basis_matrix + 8, direction_vector);
    clear_u64_value(direction_basis_matrix + 12);
    pvars_cursor += 0x10;
    direction_basis_matrix[15] = 1.0f;
    matrix_to_euler(direction_basis_matrix, pvars_cursor);
    euler_rotation_matrix(reconstructed_rotation_matrix, (float *)pvars_cursor);
    reconstructed_rotation_matrix[15] = 42.0f;
}

void compute_batalia_bolt_pickup_surface_pose(struct Moby *moby,
                                              float       *out,

                                              float *output_euler_angles) {
    float output_rotation_matrix[16];
    float base_rotation_matrix[16];
    float up[4];
    float side[4];
    float fwd[4];
    float planar_offset_direction[4];
    float u[4];
    float v[4];
    char *data = (char *)moby->pvars;
    float sx, sy, sz;
    float basis_tilt_radians;
    switch (moby->oclass) {
    case MOBY_CLASS_FIVE_BOLT_PICKUP:
        sx = five_bolt_pickup_surface_base_tilt_degrees_gp * D_L08_00161CB8;
        sy = five_bolt_pickup_surface_normal_offset_per_scale_gp * D_L08_00161CB8;
        sz = five_bolt_pickup_surface_planar_offset_per_scale_gp * D_L08_00161CB8;
        break;
    case MOBY_CLASS_TEN_BOLT_PICKUP:
        sx = ten_bolt_pickup_surface_base_tilt_degrees_gp * D_L08_00161CBC;
        sy = ten_bolt_pickup_surface_normal_offset_per_scale_gp * D_L08_00161CBC;
        sz = ten_bolt_pickup_surface_planar_offset_per_scale_gp * D_L08_00161CBC;
        break;
    default:
        sx = bolt_pickup_default_surface_base_tilt_degrees_gp * D_L08_00161CB4;
        sy = bolt_pickup_default_surface_normal_offset_per_scale_gp * D_L08_00161CB4;
        sz = bolt_pickup_default_surface_planar_offset_per_scale_gp * D_L08_00161CB4;
        break;
    }
    euler_rotation_matrix(base_rotation_matrix, (float *)(data + 0x10));
    fast_vec_normalize(u, base_rotation_matrix, -fast_sin(*(float *)(data + 0x60)));
    fast_vec_normalize(v, base_rotation_matrix + 4, fast_cos(*(float *)(data + 0x60)));
    fast_vec_add(planar_offset_direction, u, v);
    basis_tilt_radians = sx * DEG_TO_RAD;
    fast_vec_normalize(u, planar_offset_direction, fast_cos(basis_tilt_radians));
    fast_vec_normalize(v, base_rotation_matrix + 8, fast_sin(basis_tilt_radians));
    fast_vec_add(up, u, v);
    fast_vec_normalize(u, base_rotation_matrix, -fast_cos(*(float *)(data + 0x60)));
    fast_vec_normalize(v, base_rotation_matrix + 4, -fast_sin(*(float *)(data + 0x60)));
    fast_vec_add(fwd, u, v);
    fast_vec_cross(side, up, fwd);
    fast_vec_normalize(u, side, fast_cos(*(float *)(data + 0x64)));
    fast_vec_normalize(v, fwd, fast_sin(*(float *)(data + 0x64)));
    fast_vec_add(output_rotation_matrix, u, v);
    fast_vec_normalize(u, side, -fast_sin(*(float *)(data + 0x64)));
    fast_vec_normalize(v, fwd, fast_cos(*(float *)(data + 0x64)));
    fast_vec_add(output_rotation_matrix + 4, u, v);
    qcopy(output_rotation_matrix + 8, up);
    output_rotation_matrix[11] = 0.0f;
    clear_u64_value(output_rotation_matrix + 12);
    output_rotation_matrix[15] = 1.0f;
    matrix_to_euler(output_rotation_matrix, output_euler_angles);
    output_euler_angles[3] = 0.0f;
    fast_vec_normalize(u, base_rotation_matrix + 8, sy);
    fast_vec_add(out, data, u);
    fast_vec_normalize(v, planar_offset_direction, sz);
    fast_vec_add(out, out, v);
}

void draw_batalia_bolt_pickup_rotating_quad(struct Moby *moby) {
    char     *data = (char *)moby->pvars;
    Quad_5188 quad;
    float     pos[4];
    float     direction[4];
    float     length;
    float     radius;
    float     x0, x1, x2, x3, y0, y1, y2, y3;
    int       base_x, base_y;
    s64       base_z;
    u32       color;
    char     *projection_constants;

    if (*(short *)(data + 0x6c) == 0)
        return;

    qcopy(pos, &moby->pos);
    fast_vec_normalize(
        direction, (char *)moby + 0xe0, bolt_pickup_quad_center_offset_along_moby_e0);
    fast_vec_add(pos, pos, direction);
    fast_vec_normalize(
        direction, (char *)moby + 0xc0, bolt_pickup_quad_center_offset_along_moby_c0);
    fast_vec_add(pos, pos, direction);
    fast_vec_sub(pos, pos, D_L08_001675C0);
    length = fast_vec_length(pos);
    pos[3] = 1.0f;
    fast_vec_scale(pos, pos, 1024.0f);
    transform_vector(pos, pos, D_L08_001675C0 - 0x100);
    projection_constants = D_L08_0016D240;
    fast_vec_scale(pos, pos, *(float *)(projection_constants + 0x210) / pos[3]);

    base_x = truncate_float_to_s32(pos[0] * 16.0f) + 0x8000;
    base_y = (truncate_float_to_s32(pos[1] * 16.0f) + 0x8000) << 16;
    base_z = (s64)truncate_float_to_s32(pos[2] * 0.9997f + *(float *)(projection_constants + 0x1a8))
             << 32;
    if (length > 16.0f)
        length = 16.0f;
    else if (length < 4.0f)
        length = 4.0f;
    radius = convert_integer_to_float(*(signed char *)(data + 0x7b) + 16) * 0.015625f *
             (50.0f - length * 2.5f);
    x0 = radius * fast_sin(*(float *)(data + 0x74));
    y0 = radius * fast_cos(*(float *)(data + 0x74));
    x1 = radius * fast_sin(fast_add_rotations(*(float *)(data + 0x74), 1.57079637f));
    y1 = radius * fast_cos(fast_add_rotations(*(float *)(data + 0x74), 1.57079637f));
    x2 = radius * fast_sin(fast_add_rotations(*(float *)(data + 0x74), 4.71238899f));
    y2 = radius * fast_cos(fast_add_rotations(*(float *)(data + 0x74), 4.71238899f));
    x3 = radius * fast_sin(fast_add_rotations(*(float *)(data + 0x74), 3.14159274f));
    y3 = radius * fast_cos(fast_add_rotations(*(float *)(data + 0x74), 3.14159274f));

    quad.texture_coordinates[0] = 0;
    quad.texture_coordinates[1] = 0x200;
    quad.texture_coordinates[2] = 0x2000000;
    quad.texture_coordinates[3] = 0x2000200;
    color = *(u32 *)(data + 0x78);
    quad.colors[3] = color;
    quad.colors[2] = color;
    quad.colors[1] = color;
    quad.colors[0] = color;
    quad.vertices[3] = base_z;
    quad.vertices[2] = base_z;
    quad.vertices[1] = base_z;
    quad.vertices[0] = base_z;
    quad.vertices[0] += (truncate_float_to_s32(y0) << 20) + base_y;
    quad.vertices[0] += (truncate_float_to_s32(x0) << 4) + base_x;
    quad.vertices[1] += (truncate_float_to_s32(y1) << 20) + base_y;
    quad.vertices[1] += (truncate_float_to_s32(x1) << 4) + base_x;
    quad.vertices[2] += (truncate_float_to_s32(y2) << 20) + base_y;
    quad.vertices[2] += (truncate_float_to_s32(x2) << 4) + base_x;
    quad.vertices[3] += (truncate_float_to_s32(y3) << 20) + base_y;
    quad.vertices[3] += (truncate_float_to_s32(x3) << 4) + base_x;
    vu1_add_g_sregister(0x42, 0x8000000048LL);
    draw_geometry_quad_5188(
        quad.vertices, quad.texture_coordinates, quad.colors, get_effect_texture(0x13), 1);
    vu1_add_g_sregister(0x42, 0x8000000044LL);
}
