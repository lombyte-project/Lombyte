/* Level 08 moby class 0x1b3: rocket. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/math/random.h"
#include "rnc/overlay/l08/mobys/rocket.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"
#include "rnc/overlay/level_path.h"

extern int  FUN_L08_002025d0(void *, int, int, void *, int);
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern char            *create_moby(int);
extern void             update_entity(void *) __asm__("FUN_L00_00250df8");
extern float D_0015ED6C MACRO_ADDR;
extern void             spawn_explosion(void *,
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
extern char             D_0013D388[] NOT_SDA;
extern float            D_0015ED6C;
extern void            *FUN_L00_0025a420(void *, int, int);
extern float            D_0015ED6C_lui __asm__("D_0015ED6C");

extern char  *D_L08_001600EC;
extern short *D_L08_001AC040[];
extern void
initialize_moby_launch_motion(void *, int, void *, void *, int) __asm__("FUN_L05_00318648");
extern int   FUN_L00_00257b90(int, int);
extern float D_L08_001675C0[];
extern void  FUN_L00_00258278(void *, void *, float, float, float, float);
extern void  FUN_L00_0025f730(void *, float);

/* Rocket guidance, route traversal, launch effects, and destruction. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DE738), where it is exact; names translated to the US level program. */
typedef struct {
    int   count;
    int   pad[3];
    float e[1][4];
} Route_2dd4f8_2dd3c0;

typedef unsigned int Q46 __attribute__((mode(TI)));
typedef float        V46[4] __attribute__((aligned(16)));
typedef struct M46   M46;
typedef struct D46   D46;

typedef struct {
    char  pad0[0x10];
    int   animation;
    char  pad14[0x10];
    float scale;
} Model46;

struct M46 {
    char           pad0[0xC];
    float          distance;
    V46            position;
    unsigned char  state, group, pad22[2];
    Model46       *model;
    float          pad28, scale;
    unsigned char  pad30[2];
    unsigned short flags;
    char           pad34[0xC];
    V46            rotation;
    char           pad50[0x28];
    D46           *data;
    char           pad7C[0x18];
    int            animation;
    char           pad98[0xE];
    unsigned short class_id;
    char           padA8[0x18];
    V46            matrix[4];
};

struct D46 {
    char                 pad0[0x20];
    V46                  velocity;
    char                 pad30[0x10];
    float                pitch, yaw, roll;
    M46                 *target;
    int                  waypoint;
    signed char          direction;
    char                 pad55[3];
    int                  health;
    int                  pad5C;
    Route_2dd4f8_2dd3c0 *path;
    int                  path_index;
    char                 pad68[0x18];
    float                yaw_velocity;
    int                  pad84, effect_path;
    float                activation_range;
    int                  persistent, timer;
    float                speed;
};

typedef struct {
    int                  waypoint;
    signed char          direction;
    char                 pad5[0xB];
    Route_2dd4f8_2dd3c0 *path;
} RouteState46;

typedef struct {
    char          pad0[0x2C];
    unsigned char active;
} Persistent46;

extern char *D_L08_0015FFD8_macro __asm__("D_L08_0015FFD8") MACRO_ADDR;
extern char  D_0013F3D0[] NOT_SDA;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

#include "rnc/overlay/l08/mobys/rocket_data/update_rocket_3.h"

extern float rocket_guidance_velocity_correction_limit_gp __asm__("D_L08_00161A04")
    __attribute__((sda));
extern float rocket_route_speed_gp __asm__("D_L08_00161A08") __attribute__((sda));
extern float rocket_target_hit_distance_gp __asm__("D_L08_00161A0C") __attribute__((sda));
extern float rocket_effect_radial_speed_range_end_gp __asm__("D_L08_00161A14") __attribute__((sda));

#include "rnc/overlay/l08/mobys/rocket_data/update_rocket_2.h"

extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern float approach_value(float *, float, float);
extern int   D_L08_0015F5C4_macro __asm__("D_L08_0015F5C4") MACRO_ADDR;

#include "rnc/overlay/l08/mobys/rocket_data/rocket_update.h"

extern unsigned int D_L08_0015FFDC MACRO_ADDR;
extern void                        FUN_L00_00262cf0(void *, void *, float, float, float);
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void  FUN_L08_002de528_u(void *, int, int, int) __asm__("FUN_L08_002de528");
void         rocket_update(M46 *moby) __asm__("FUN_L08_002dd3c0");
extern void  set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern int   rocket_color_red_gp __asm__("D_L08_00161A34") __attribute__((sda));
extern int   rocket_color_green_gp __asm__("D_L08_00161A38") __attribute__((sda));
extern int   rocket_color_blue_gp __asm__("D_L08_00161A3C") __attribute__((sda));
extern float rocket_scale_multiplier_gp __asm__("D_L08_00161A40") __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DF8A0), where it is exact; names translated to the US level program. */
void spawn_rocket_explosion_and_debris(char *moby,
                                       int   transform_index,
                                       int   spawn_count,
                                       int   flag) __asm__("FUN_L08_002de528");

/* Moby class 435 */
void rocket_update(M46 *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_APPROACH_TARGET_OFFSET = 1,
        ROCKET_STATE_HOME_TOWARD_TARGET = 2,
        ROCKET_STATE_WAIT_FOR_ACTIVATION = 3,
        ROCKET_STATE_FOLLOW_ROUTE = 4,
        ROCKET_STATE_FLY_CONFIGURED_HEADING = 5,
        ROCKET_STATE_EXPLODE_AND_REMOVE = 6
    };
    enum {
        LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_POSITIVE_Z_LAUNCH_OFFSET = 0,
        LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET = 1,
        LINKED_CLASS_NEGATIVE_X_NEGATIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET = 2,
        LINKED_CLASS_VERTICAL_LAUNCH_OFFSET = 3,
        LINKED_CLASS_NEGATIVE_Y_POSITIVE_Z_LAUNCH_OFFSET = 4,
        LINKED_CLASS_ZERO_LAUNCH_OFFSET = 5
    };
    V46  target, delta, velocity_correction_or_effect_velocity, effect, aux;
    D46 *data = moby->data;
    if (FUN_L00_0025a420(moby, 0x210000, 0) && moby->state != 4) {
        allocate_voice_for_target_entry(0, 0, moby);
        cart_burst(moby, moby->position, -1, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case ROCKET_STATE_INITIALIZE: {
        char         *persistence_base = D_0013D388;
        Persistent46 *state = (Persistent46 *)(persistence_base + data->persistent);
        if (state->active) {
            short *link = D_L08_001AC040[moby->group];
            do {
                mark_moby_for_removal(D_L08_0015FFD8_macro +
                                      ((*(unsigned short *)link & 0x7FFF) << 8));
            } while (*link++ >= -1);
            return;
        }
        if (!data->persistent)
            data->timer = scale_game_frames(10);
        data->path = ((Route_2dd4f8_2dd3c0 *)l08_paths[data->path_index]);
        qcopy(moby->position, data->path->e[0]);
        data->waypoint = 1;
        moby->state = 3;
        moby->animation = 0;
        moby->flags = 0;
        return;
    }
    case ROCKET_STATE_APPROACH_TARGET_OFFSET: {
        D46  *target_data = data->target->data;
        int   life, size0, size1;
        float yaw, pitch, radius, angle;
        moby->rotation[0] = fast_add_rotations(moby->rotation[0], D_0015ED6C * 6.2831855f);
        qcopy(target, data->target->position);
        target[2] += 5.0f;
        yaw = fast_arc_tan(moby->position[0] - target[0], moby->position[1] - target[1]);
        pitch =
            fast_arc_tan(vector_distance_xy(target, moby->position), moby->position[2] - target[2]);
        FUN_L00_00262cf0(target, target, data->roll, data->yaw, data->pitch);
        fast_vec_sub(delta, target, moby->position);
        FUN_L00_0025f730(delta, target_data->speed * D_0015ED6C);
        fast_vec_sub(velocity_correction_or_effect_velocity, delta, data->velocity);
        FUN_L00_0025f730(velocity_correction_or_effect_velocity,
                         rocket_guidance_velocity_correction_limit_gp * frame_time_sq_macro);
        fast_vec_add(data->velocity, data->velocity, velocity_correction_or_effect_velocity);
        fast_vec_add(moby->position, moby->position, data->velocity);
        moby->rotation[1] = -fast_arc_tan(vector_length_xy(data->velocity), data->velocity[2]);
        moby->rotation[2] = fast_arc_tan(data->velocity[0], data->velocity[1]);
        if (fast_difference_between_rotations(yaw, data->yaw) < 0.08726646f &&
            fast_difference_between_rotations(pitch, data->pitch) < 0.08726646f)
            moby->state = 2;
        radius = random_float_between(0.0f, rocket_effect_radial_speed_range_end_gp) * D_0015ED6C;
        angle = random_angle_radians();
        fast_vec_normalize(effect, moby->matrix[2], radius * fast_cos(angle));
        fast_vec_normalize(aux, moby->matrix[1], radius * fast_sin(angle));
        fast_vec_add(effect, effect, aux);
        qcopy(aux, aux);
        effect[3] = rocket_guidance_effect_velocity_w_gp;
        aux[3] = rocket_guidance_effect_auxiliary_vector_w_gp;
        life = scale_game_frames(rocket_guidance_effect_frame_count_gp);
        size0 = truncate_float_to_s32(rocket_guidance_effect_velocity_w_gp);
        size1 = truncate_float_to_s32(rocket_guidance_effect_auxiliary_vector_w_gp);
        spawn_effect(moby->position,
                     effect,
                     rocket_guidance_effect_initial_color_gp,
                     rocket_guidance_effect_second_color_gp,
                     life,
                     size0,
                     size1,
                     1);
        return;
    }
    case ROCKET_STATE_HOME_TOWARD_TARGET: {
        D46  *target_data = data->target->data;
        int   life, size0, size1;
        float scale, radius, angle;
        moby->rotation[0] = fast_add_rotations(moby->rotation[0], D_0015ED6C * 6.2831855f);
        scale = moby->model->scale * 3.0f;
        approach_value(&moby->scale, scale, moby->model->scale / (float)scale_game_frames(180));
        qcopy(target, data->target->position);
        target[2] += 5.0f;
        fast_vec_sub(delta, target, moby->position);
        FUN_L00_0025f730(delta, target_data->speed * D_0015ED6C);
        fast_vec_sub(velocity_correction_or_effect_velocity, delta, data->velocity);
        FUN_L00_0025f730(velocity_correction_or_effect_velocity,
                         rocket_guidance_velocity_correction_limit_gp * 4.0f * frame_time_sq_macro);
        fast_vec_add(data->velocity, data->velocity, velocity_correction_or_effect_velocity);
        fast_vec_add(moby->position, moby->position, data->velocity);
        moby->rotation[1] = -fast_arc_tan(vector_length_xy(data->velocity), data->velocity[2]);
        moby->rotation[2] = fast_arc_tan(data->velocity[0], data->velocity[1]);
        if (fast_vec_dist(moby->position, data->target->position) < rocket_target_hit_distance_gp) {
            target_data->health -= 60;
            clear_u64_value(data->velocity);
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 6;
        }
        radius = random_float_between(0.0f, rocket_effect_radial_speed_range_end_gp) * D_0015ED6C;
        angle = random_angle_radians();
        fast_vec_normalize(effect, moby->matrix[2], radius * fast_cos(angle));
        fast_vec_normalize(aux, moby->matrix[1], radius * fast_sin(angle));
        fast_vec_add(effect, effect, aux);
        qcopy(aux, aux);
        effect[3] = rocket_guidance_effect_velocity_w_gp;
        aux[3] = rocket_guidance_effect_auxiliary_vector_w_gp;
        life = scale_game_frames(rocket_guidance_effect_frame_count_gp);
        size0 = truncate_float_to_s32(rocket_guidance_effect_velocity_w_gp);
        size1 = truncate_float_to_s32(rocket_guidance_effect_auxiliary_vector_w_gp);
        spawn_effect(moby->position,
                     effect,
                     rocket_guidance_effect_initial_color_gp,
                     rocket_guidance_effect_second_color_gp,
                     life,
                     size0,
                     size1,
                     1);
        return;
    }
    case ROCKET_STATE_WAIT_FOR_ACTIVATION:
        if (D_L08_0015F5C4_macro)
            return;
        if (fast_vec_dist(D_0013F3D0, (char *)data->path + (data->path->count << 4)) <
                data->activation_range &&
            fast_dec_timer(&data->timer)) {
            moby->flags = 0x40;
            moby->state = 4;
            moby->animation = moby->model->animation;
            moby->rotation[2] = fast_arc_tan(data->path->e[1][0] - moby->position[0],
                                             data->path->e[1][1] - moby->position[1]);
            allocate_voice_for_target_entry(1, 0, moby);
        }
        return;
    case ROCKET_STATE_FOLLOW_ROUTE: {
        RouteState46 *route;
        int           next_route_waypoint_index;
        float         yaw;
        if (D_L08_0015F5C4_macro)
            return;
        route = (RouteState46 *)&data->waypoint;
        next_route_waypoint_index =
            (route->waypoint + route->path->count + route->direction) % route->path->count;
        yaw =
            fast_arc_tan(*(float *)((char *)route->path + (next_route_waypoint_index << 4) + 0x10) -
                             moby->position[0],
                         *(float *)((char *)route->path + (next_route_waypoint_index << 4) + 0x14) -
                             moby->position[1]);
        FUN_L00_00258278(moby, &data->yaw_velocity, yaw, 0.1f, 1.3f, 1.0f);
        moby->rotation[1] = fast_arc_tan(
            vector_distance_xy(route->path->e[next_route_waypoint_index], moby->position),
            moby->position[2] - route->path->e[next_route_waypoint_index][2]);
        qcopy_nc(target, route->path->e[next_route_waypoint_index]);
        if (route->waypoint != route->path->count - 1) {
            int life0, life1, size1;
            fast_vec_sub(delta, target, moby->position);
            fast_vec_normalize(delta, delta, rocket_route_speed_gp * D_0015ED6C);
            fast_vec_add(moby->position, moby->position, delta);
            if (fast_vec_dist(moby->position, target) < 0.5f)
                route->waypoint = next_route_waypoint_index;
            *(Q46 *)effect = (Q46)0;
            effect[0] = random_float_between(-1.0f, 1.0f);
            effect[1] = random_float_between(-1.0f, 1.0f);
            effect[2] = random_float_between(-1.0f, 1.0f);
            *(Q46 *)velocity_correction_or_effect_velocity = *(Q46 *)effect;
            FUN_L00_0025f730(velocity_correction_or_effect_velocity, D_0015ED6C + D_0015ED6C);
            life0 = scale_game_frames(FUN_L00_00257b90(30, 45));
            size1 = FUN_L00_00257b90(40, 70);
            spawn_effect(moby->position,
                         velocity_correction_or_effect_velocity,
                         0x6F00AFFF,
                         0xFF,
                         life0,
                         40,
                         size1,
                         1);
            *(Q46 *)aux = (Q46)0;
            aux[0] = random_float_between(-1.0f, 1.0f);
            aux[1] = random_float_between(-1.0f, 1.0f);
            aux[2] = random_float_between(-1.0f, 1.0f);
            *(Q46 *)effect = *(Q46 *)aux;
            FUN_L00_0025f730(effect, D_0015ED6C + D_0015ED6C);
            life1 = scale_game_frames(FUN_L00_00257b90(60, 120));
            size1 = FUN_L00_00257b90(100, 150);
            spawn_effect(moby->position, effect, 0x1FFFFFFF, 0x4F4F4F, life1, 40, size1, 0);
            return;
        } else {
            short *link = D_L08_001AC040[moby->group];
            long   special = 0;
            int    index;
            M46   *rocket_linked_group_moby;
            if (!link)
                return;
            index = *(unsigned short *)link & 0x7FFF;
            rocket_linked_group_moby = (M46 *)(D_L08_0015FFD8_macro + (index << 8));
            ((Persistent46 *)(D_0013D388 + data->persistent))->active = 1;
        linked_object:
            if (rocket_linked_group_moby->state < 0x7F && rocket_linked_group_moby != moby) {
                unsigned short linked_moby_class_id = rocket_linked_group_moby->class_id;
                if ((unsigned int)(linked_moby_class_id - 0x223) < 6) {
                    clear_u64_value(velocity_correction_or_effect_velocity);
                    switch ((short)(rocket_linked_group_moby->class_id - 0x223)) {
                    case LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_POSITIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[0] = -3.331f;
                        aux[1] = 3.331f;
                        aux[2] = 0.275f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[0] = -6.0f;
                        aux[1] = 6.0f;
                        aux[2] = -1.772f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_NEGATIVE_X_NEGATIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[0] = -2.0f;
                        aux[1] = -1.152f;
                        aux[2] = -1.772f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_VERTICAL_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[2] = 0.975f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_NEGATIVE_Y_POSITIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[1] = -1.665f;
                        aux[2] = 0.642f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_ZERO_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        qcopy(effect, aux);
                        break;
                    }
                    if (rocket_linked_group_moby->distance > 1500.0f)
                        mark_moby_for_removal(rocket_linked_group_moby);
                    else {
                        float  launch_yaw, center_yaw, min_frames, max_frames, angle_delta;
                        float *launch_offset;
                        fast_vec_sub(velocity_correction_or_effect_velocity,
                                     rocket_linked_group_moby->position,
                                     moby->position);
                        velocity_correction_or_effect_velocity[2] = 0.0f;
                        fast_vec_normalize(velocity_correction_or_effect_velocity,
                                           velocity_correction_or_effect_velocity,
                                           D_0015ED6C * 12.0f);
                        velocity_correction_or_effect_velocity[2] =
                            random_float_between(D_0015ED6C * 8.0f, D_0015ED6C * 12.0f);
                        rocket_linked_group_moby->animation = 0;
                        launch_yaw = fast_arc_tan(velocity_correction_or_effect_velocity[0],
                                                  velocity_correction_or_effect_velocity[1]);
                        center_yaw = fast_arc_tan(l08_camera.pos.f[0] - moby->position[0],
                                                  l08_camera.pos.f[1] - moby->position[1]);
                        angle_delta = fast_difference_between_rotations(launch_yaw, center_yaw);
                        launch_offset = effect;
                        if (angle_delta < 0.7853982f) {
                            min_frames = 10.0f;
                            max_frames = 20.0f;
                        } else {
                            min_frames = 60.0f;
                            max_frames = 90.0f;
                        }
                        initialize_moby_launch_motion(
                            rocket_linked_group_moby,
                            (int)moby,
                            velocity_correction_or_effect_velocity,
                            launch_offset,
                            truncate_float_to_s32(random_float_between(min_frames, max_frames)));
                    }
                } else if ((short)linked_moby_class_id == 0x1A8) {
                    special = 1;
                    clear_u64_value(velocity_correction_or_effect_velocity);
                    spawn_explosion(rocket_linked_group_moby,
                                    velocity_correction_or_effect_velocity,
                                    rocket_linked_group_moby->position,
                                    0.0f,
                                    0.0f,
                                    50,
                                    60,
                                    60,
                                    12.0f,
                                    8.0f,
                                    9.0f,
                                    1.0f,
                                    -1,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                    mark_moby_for_removal(rocket_linked_group_moby);
                }
            }
            if (*link++ >= -1) {
                index = *(unsigned short *)link & 0x7FFF;
                rocket_linked_group_moby = (M46 *)(D_L08_0015FFD8_macro + (index << 8));
                goto linked_object;
            }
            if (special)
                FUN_L08_002de528_u(moby, data->effect_path, 30, 1);
            else
                FUN_L08_002de528_u(moby, data->effect_path, 30, 0);
            allocate_voice_for_target_entry(2, 0, moby);
            moby->state = 6;
        }
        return;
    }
    case ROCKET_STATE_FLY_CONFIGURED_HEADING:
        step_angle_toward_target(&moby->rotation[2], 0, data->yaw, D_0015ED6C * 0.5235988f);
        step_angle_toward_target(&moby->rotation[1], 0, -data->pitch, D_0015ED6C * 0.5235988f);
        moby->rotation[0] = fast_add_rotations(moby->rotation[0], D_0015ED6C * 6.2831855f);
        build_spherical_offset(
            data->velocity,
            (rocket_configured_heading_speed_base_gp + rocket_configured_heading_speed_base_gp) *
                D_0015ED6C,
            moby->rotation[2],
            -moby->rotation[1]);
        fast_vec_add(moby->position, moby->position, data->velocity);
        if (FUN_L08_002025d0(moby->position, 0x400, 2, moby, 0) || moby->position[0] < 5.0f ||
            moby->position[0] > 500.0f || moby->position[1] < 5.0f || moby->position[1] > 500.0f ||
            moby->position[2] < 5.0f || moby->position[2] > 500.0f ||
            fast_vec_dist(D_0013F3D0, moby->position) < 16.0f)
            moby->state = 6;
        return;
    case ROCKET_STATE_EXPLODE_AND_REMOVE:
        if (D_L08_0015FFDC < (unsigned int)moby)
            spawn_explosion(moby,
                            data->velocity,
                            moby->position,
                            0.0f,
                            0.0f,
                            15,
                            10,
                            10,
                            3.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
        mark_moby_for_removal(moby);
        break;
    default:
        break;
    }
}

struct Moby *spawn_rocket(void        *pos,
                          struct Moby *target,
                          void        *velocity,
                          float        pitch,
                          float        yaw) __asm__("FUN_L08_002de3e0");

struct Moby *spawn_rocket(void *pos, struct Moby *target, void *velocity, float pitch, float yaw) {
    struct Moby *moby = create_moby(0x1B3);
    char        *projectile_vars;
    float       *reference;
    float        distance;
    float        scale;
    struct Moby *target_copy = target;
    void        *velocity_copy = velocity;
    if (moby != 0) {
        projectile_vars = (char *)moby->pvars;
        qcopy(&moby->pos, pos);
        reference = D_L08_001675C0;
        distance = vector_distance_xy(reference, &moby->pos);
        reference = (float *)((char *)reference - 0x140);
        moby->rot.y = -fast_arc_tan(distance, reference[82] - moby->pos.z);
        *(volatile float *)&moby->rot.z =
            fast_arc_tan(reference[80] - moby->pos.x, reference[81] - moby->pos.y);
        scale = moby->pclass->scale;
        scale *= rocket_scale_multiplier_gp;
        moby->update_dist = 0xFF;
        moby->visible = 1;
        *(volatile u8 *)&moby->state = 1;
        moby->draw_dist = 0xFF;
        moby->scale = scale;
        set_moby_color(moby, rocket_color_red_gp, rocket_color_green_gp, rocket_color_blue_gp);
        *(float *)(projectile_vars + 0x40) = pitch;
        *(float *)(projectile_vars + 0x48) = 70.0f;
        *(float *)(projectile_vars + 0x44) = yaw;
        *(struct Moby **)(projectile_vars + 0x4C) = target_copy;
        qcopy(projectile_vars + 0x20, velocity_copy);
        update_entity(moby);
    }
    return moby;
}

void spawn_rocket_explosion_and_debris(char *moby, int transform_index, int spawn_count, int flag) {
    float spawn_position[4];
    float local_rotation_offset[4];
    float launch_velocity[4];
    char *rocket_vars = ((char *)MOBY(moby)->pvars);
    int   i;

    if (flag) {
        spawn_explosion(moby,
                        rocket_vars + 0x20,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x1E,
                        0x14,
                        0x14,
                        6.0f,
                        4.0f,
                        9.0f,
                        1.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
    } else {
        spawn_explosion(moby,
                        rocket_vars + 0x20,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x2D,
                        0x1E,
                        0x1E,
                        10.0f,
                        6.0f,
                        9.0f,
                        1.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
    }
    for (i = 0; i < spawn_count; i++) {
        {
            char *spawned_moby;
            if (flag) {
                if (random_integer_below(0xFF) & 1)
                    spawned_moby = create_moby(0x30E);
                else
                    spawned_moby = create_moby(0x30F);
            } else {
                if (random_integer_below(0xFF) & 1)
                    spawned_moby = create_moby(0x310);
                else
                    spawned_moby = create_moby(0x311);
            }
            MOBY(spawned_moby)->draw_dist = 0x40;
            spawned_moby[0x31] = 1;
            spawned_moby[0x30] = 0x40;
            MOBY(spawned_moby)->spawn_frame = ((long)MOBY(moby)->spawn_frame);
            MOBY(spawned_moby)->rot.x = random_angle_radians();
            MOBY(spawned_moby)->rot.y = random_angle_radians();
            MOBY(spawned_moby)->rot.z = random_angle_radians();
            spawn_position[0] = random_float_between(-1.0f, 1.0f);
            spawn_position[1] = random_float_between(-1.0f, 1.0f);
            spawn_position[2] = random_float_between(-1.0f, 1.0f);
            spawn_position[3] = 1.0f;
            transform_vector(
                spawn_position, spawn_position, D_L08_001600EC + (transform_index << 7));
            qcopy(spawned_moby + 0x10, spawn_position);
            clear_u64_value(local_rotation_offset);
            fast_vec_sub(launch_velocity, spawn_position, moby + 0x10);
            set_vector_length_xy(
                launch_velocity,
                launch_velocity,
                random_float_between(D_0015ED6C_lui * 12.0f, D_0015ED6C_lui * 15.0f));
            launch_velocity[2] =
                random_float_between(D_0015ED6C_lui * 6.0f, D_0015ED6C_lui * 18.0f);
            initialize_moby_launch_motion(
                spawned_moby,
                (int)moby,
                launch_velocity,
                local_rotation_offset,
                truncate_float_to_s32(random_float_between(60.0f, 90.0f)));
        }
    }
}
