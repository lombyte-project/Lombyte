/* Level 18 moby classes: drek_fight_missile, drek_final_boss_robot, deplanetizer_gun, rocket_tank. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_check_damage.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/game/jet_reset_camera.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/mobys/draw_trail.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"

/* Drek missile Moby.state shared by its update and response helpers. */
enum DrekMissileState {
    DREK_MISSILE_INITIALIZE = 0x0,
    DREK_MISSILE_WANDER = 0x2,
    DREK_MISSILE_DIVE = 0x3,
    DREK_MISSILE_WAIT_TO_EXPLODE = 0x4
};

#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

/* State machine for the class 1355 moby: dispatches on moby state (0 init, 2 wander, 3 dive, 4 land). */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002F0F78), where it is exact; names translated to the US level program. */

extern f32   random_angle_radians(void) __asm__("FUN_00213308");
extern float frame_time_macro[] __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_L18_001677C0[];
extern float
step_angle_toward_target(float *p, int n, float x, float y) __asm__("FUN_L00_0025bc00");
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float D_L18_0015F580[] MACRO_ADDR;
extern int   D_L18_0015F5C4;
extern int   D_L18_0016D290_lui __asm__("D_L18_0016D290") NOT_SDA;
int update_drek_fight_missile_wander_movement(char *moby, float *wander_target_position) __asm__("FUN_L18_002f0120");

extern int   update_drek_fight_missile_wander_movement(char  *moby,
                                                       float *vec) __asm__("FUN_L18_002f0120");
extern s32   random_integer_below(s32) __asm__("FUN_00213260");
extern short l18_drek_missile_roll_rate_degrees_gp __asm__("D_L18_00162210") __attribute__((sda));
extern short D_L18_00162214_gp __asm__("D_L18_00162214") __attribute__((sda));
extern short l18_drek_missile_dive_speed_gp __asm__("D_L18_00162218") __attribute__((sda));
extern short l18_drek_missile_wander_to_dive_random_bound_gp __asm__("D_L18_00162234")
    __attribute__((sda));
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
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
void        select_drek_fight_missile_wander_target(struct Moby *moby,
                                                    void        *out) __asm__("FUN_L18_002f02e8");

extern void select_drek_fight_missile_wander_target(struct Moby *moby,
                                                    void        *out) __asm__("FUN_L18_002f02e8");
void        update_drek_fight_missile_trail_and_queue_effects(struct Moby *moby) __asm__("FUN_L18_002f0848");

extern void
update_drek_fight_missile_trail_and_queue_effects(struct Moby *moby) __asm__("FUN_L18_002f0848");
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int  func_0022ED80_2F0F78(int, int, int) __asm__("FUN_0022da68");

void drek_fight_missile_update(struct Moby *moby) __asm__("FUN_L18_002efb88");

void drek_fight_missile_update(struct Moby *moby) {
    char *missile_vars = (char *)moby->pvars;
    if (moby->state != 4) {
        moby->rot.x = fast_add_rotations(moby->rot.x,
                                         *(float *)&l18_drek_missile_roll_rate_degrees_gp *
                                             DEG_TO_RAD * frame_time);
    }
    if (D_L18_0015F5C4 == 2 && D_L18_0016D290_lui == 3) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case DREK_MISSILE_INITIALIZE: {
        float wander_acceleration_scale = frame_time_macro[1];
        float initialization_frame_time = frame_time;
        *(float *)(missile_vars + 0x158) = wander_acceleration_scale * 10.0f;
        *(float *)(missile_vars + 0x154) = initialization_frame_time * 12.566371f;
        *(float *)(missile_vars + 0x150) = wander_acceleration_scale * 3.1415927f;
        *(float *)(missile_vars + 0x15C) = initialization_frame_time * 10.0f;
        *(float *)(missile_vars + 0x168) = random_angle_radians();
        moby->draw_dist = 0xFF;
        qcopy(missile_vars + 0x120, &moby->pos);
        *(float *)(missile_vars + 0x128) += 5.0f;
        *(float *)(missile_vars + 0x128) = probe_ground_height(missile_vars + 0x120, 0, 0.5f);
        select_drek_fight_missile_wander_target(moby, missile_vars + 0x110);
        moby->state = 1;
        moby->flags |= 1;
        moby->visible = 0;
        moby->collision = 0;
        break;
    }
    case DREK_MISSILE_WANDER: {
        char *wander_target_position = missile_vars + 0x110;
        if (update_drek_fight_missile_wander_movement(moby, (float *)wander_target_position)) {
            select_drek_fight_missile_wander_target(moby, wander_target_position);
        }
        update_drek_fight_missile_trail_and_queue_effects(moby);
        if (random_integer_below(*(int *)&l18_drek_missile_wander_to_dive_random_bound_gp)) {
            return;
        }
        moby->state = 3;
        *(float *)(missile_vars + 0x130) =
            fast_cos(moby->rot.z) * (*(float *)&D_L18_00162214_gp * frame_time);
        *(float *)(missile_vars + 0x134) =
            fast_sin(moby->rot.z) * (*(float *)&D_L18_00162214_gp * frame_time);
        *(float *)(missile_vars + 0x138) = *(float *)(missile_vars + 0x144);
        *(int *)(missile_vars + 0x14C) = 0;
        break;
    }
    case DREK_MISSILE_DIVE: {
        float dive_movement_step[4];
        float ground_check_position[4];
        fast_vec_sub(dive_movement_step, missile_vars + 0x120, &moby->pos);
        fast_vec_normalize(dive_movement_step,
                           dive_movement_step,
                           *(float *)&l18_drek_missile_dive_speed_gp * frame_time);
        approach_value((float *)(missile_vars + 0x14C), 1.0f, frame_time);
        lerp_vector(dive_movement_step,
                    missile_vars + 0x130,
                    dive_movement_step,
                    *(float *)(missile_vars + 0x14C));
        fast_vec_add(&moby->pos, &moby->pos, dive_movement_step);
        moby->rot.z = fast_arc_tan(dive_movement_step[0], dive_movement_step[1]);
        moby->rot.y = -fast_arc_tan(vector_length_xy(dive_movement_step), dive_movement_step[2]);
        fast_vec_normalize(ground_check_position, &moby->rot_row0, 1.0f);
        fast_vec_add(ground_check_position, ground_check_position, &moby->pos);
        update_drek_fight_missile_trail_and_queue_effects(moby);
        if (ground_check_position[2] < *(float *)(missile_vars + 0x128)) {
            float minimum_missile_height;
            func_0022ED80_2F0F78(1, 0, (int)moby);
            moby->state = 4;
            *(int *)(missile_vars + 0x160) = scale_game_frames(0x5A);
            minimum_missile_height = *(float *)(missile_vars + 0x128) + 0.7f;
            if (moby->pos.z < minimum_missile_height) {
                moby->pos.z = minimum_missile_height;
            }
        }
        break;
    }
    case DREK_MISSILE_WAIT_TO_EXPLODE: {
        float x = moby->rot.y;
        if (x < 0.87266463f) {
            x = 0.87266463f;
        }
        step_angle_toward_target(&moby->rot.y, 0, x, frame_time * 12.566371f);
        if (fast_dec_timer((int *)(missile_vars + 0x160))) {
            int   i;
            char *pos = &moby->pos;
            moby->state = 2;
            *(short *)(missile_vars + 0x164) = 0;
            spawn_explosion(*(void **)(missile_vars + 0x16C),
                            D_L18_0015F580,
                            pos,
                            1.0f,
                            1.0f,
                            10,
                            3,
                            16,
                            1.5f,
                            0.5f,
                            9.0f,
                            1.0f,
                            2,
                            5.0f,
                            0,
                            0,
                            -1,
                            0);
            qcopy(pos, D_L18_001677C0);
            moby->pos.z += 20.0f;
            for (i = 0; (float)i < 4.0f; i++) {
                qcopy(missile_vars + 0x10 + i * 16, pos);
            }
        } else {
            update_drek_fight_missile_trail_and_queue_effects(moby);
        }
        break;
    }
    }
}

#include "qcopy.h"

/* Wakes every waiting moby of class 0x54B in a group: moves it to pos and records the caller. */

extern short         *D_L18_001AC240[];
extern unsigned char *D_L18_0015FFD8;

int activate_drek_fight_missiles_in_group(
    char *moby, int group, float *missile_activation_position) __asm__("FUN_L18_002f0030");

int activate_drek_fight_missiles_in_group(char *moby,
                                          int   group,

                                          float *missile_activation_position) {
    short *group_entry = D_L18_001AC240[group];
    int    idle;
    int    i;
    if (group_entry == 0)
        return 1;
    idle = 1;
    while (1) {
        unsigned char *group_moby =
            (unsigned char *)(((*group_entry & 0x7FFF) << 8) + (int)D_L18_0015FFD8);
        if (MOBY(group_moby)->oclass == 0x54B && group_moby[0x20] == 1) {
            unsigned char *missile_vars = ((unsigned char *)MOBY(group_moby)->pvars);
            idle = 0;
            qcopy(group_moby + 0x10, missile_activation_position);
            i = 0;
            do {
                qcopy(missile_vars + 0x10 + i * 16, group_moby + 0x10);
                i++;
            } while ((float)i < 4.0f);
            *(char **)(missile_vars + 0x16C) = moby;
            group_moby[0x31] = 1;
            MOBY(group_moby)->flags &= 0xFFFE;
            MOBY(group_moby)->collision = *(int *)(((char *)MOBY(group_moby)->pclass) + 0x10);
            group_moby[0x20] = 2;
        }
        if (*group_entry++ < 0)
            return idle;
    }
    return idle;
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002F1510), where it is exact; names translated to the US level program. */

extern float FUN_00213f38_c(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern short drek_missile_wander_vertical_velocity_to_pitch_scale_gp __asm__("D_L18_0016222C")
    __attribute__((sda));

int update_drek_fight_missile_wander_movement(char *moby, float *wander_target_position) {
    float target_displacement[4];
    float redirected_target_position[4];
    char *missile_vars = ((char *)MOBY(moby)->pvars);
    char *linked_moby;
    float yaw;

    fast_vec_sub(target_displacement, wander_target_position, moby + 0x10);
    yaw = fast_add_rotations(fast_arc_tan(target_displacement[0], target_displacement[1]),
                             *(float *)(missile_vars + 0x148));
    approach_angle((float *)&MOBY(moby)->rot.z,
                   (float *)(missile_vars + 0x140),
                   yaw,
                   *(float *)(missile_vars + 0x150),
                   *(float *)(missile_vars + 0x150),
                   *(float *)(missile_vars + 0x154));
    MOBY(moby)->pos.x += fast_cos(MOBY(moby)->rot.z) * (*(float *)&D_L18_00162214_gp * frame_time);
    MOBY(moby)->pos.y += fast_sin(MOBY(moby)->rot.z) * (*(float *)&D_L18_00162214_gp * frame_time);
    FUN_00213f38_c((float *)&MOBY(moby)->pos.z,
                   (float *)(missile_vars + 0x144),
                   wander_target_position[2],
                   *(float *)(missile_vars + 0x158),
                   *(float *)(missile_vars + 0x158),
                   *(float *)(missile_vars + 0x15C));
    MOBY(moby)->rot.y = *(float *)&drek_missile_wander_vertical_velocity_to_pitch_scale_gp *
                        *(float *)(missile_vars + 0x144);
    if (fast_vec_length(target_displacement) < 2.0f) {
        return 1;
    }
    if (fast_dec_timer((int *)(missile_vars + 0x160))) {
        return 1;
    }
    linked_moby = *(char **)(missile_vars + 0x16C);
    if (linked_moby) {
        fast_vec_sub(redirected_target_position, moby + 0x10, linked_moby + 0x10);
        if (!(vector_length_xy(redirected_target_position) < 4.5f)) {
            return 0;
        }
        set_vector_length_xy(redirected_target_position,
                             redirected_target_position,
                             random_float_between(15.0f, 20.0f));
        fast_vec_add(redirected_target_position,
                     redirected_target_position,
                     *(char **)(missile_vars + 0x16C) + 0x10);
        redirected_target_position[2] = wander_target_position[2];
        qcopy(wander_target_position, redirected_target_position);
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002F16D8), where it is exact; names translated to the US level program. */

extern float random_angle_radians(void) __asm__("FUN_00213308");

extern short drek_missile_wander_z_offset_range_start_gp __asm__("D_L18_0016221C")
    __attribute__((sda));
extern short drek_missile_wander_z_offset_range_end_gp __asm__("D_L18_00162220")
    __attribute__((sda));
extern short drek_missile_wander_radius_range_start_gp __asm__("D_L18_00162224")
    __attribute__((sda));
extern short drek_missile_wander_radius_range_end_gp __asm__("D_L18_00162228") __attribute__((sda));

void select_drek_fight_missile_wander_target(struct Moby *moby, void *out) {
    char *data = ((char *)moby->pvars);
    float wander_target_offset[3];
    float wander_radius = random_float_between(*(float *)&drek_missile_wander_radius_range_start_gp,
                                               *(float *)&drek_missile_wander_radius_range_end_gp);
    float wander_target_angle = random_angle_radians();
    wander_target_offset[0] = fast_cos(wander_target_angle) * wander_radius;
    wander_target_offset[1] = fast_sin(wander_target_angle) * wander_radius;
    wander_target_offset[2] = 0.0f;
    wander_target_offset[2] =
        random_float_between(*(float *)&drek_missile_wander_z_offset_range_start_gp,
                             *(float *)&drek_missile_wander_z_offset_range_end_gp);
    fast_vec_add(out, wander_target_offset, data + 0x120);
    *(int *)(data + 0x160) = scale_game_frames(180);
}

extern int D_L18_00162238_gp __asm__("D_L18_00162238") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_fight_missile_globals.h"

extern int drek_missile_trail_draw_disabled_gp __asm__("D_L18_0016223C") __attribute__((sda));
extern int drek_missile_trail_blend_minuend_selector_gp __asm__("D_L18_00162240")
    __attribute__((sda));
extern int drek_missile_trail_blend_subtrahend_selector_gp __asm__("D_L18_00162244")
    __attribute__((sda));
extern int drek_missile_trail_blend_factor_selector_gp __asm__("D_L18_00162248")
    __attribute__((sda));
extern int drek_missile_trail_blend_addend_selector_gp __asm__("D_L18_0016224C")
    __attribute__((sda));
extern int   drek_missile_trail_blend_start_color_gp __asm__("D_L18_00162250") __attribute__((sda));
extern int   drek_missile_trail_blend_end_color_gp __asm__("D_L18_00162254") __attribute__((sda));
extern int   game_frame __asm__("D_L18_0015F5CC");
extern float drek_missile_trail_texture_coordinates[4][2] __asm__("D_L18_001DA2F0");
extern long  get_effect_texture(int) __asm__("FUN_001f44b8");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void  draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern void  memset(void *, int, int) __asm__("FUN_001153fc");

/* Draw callback: builds a 16-segment glow trail and draws it behind every class 0x54B moby in the group. */
void draw_drek_fight_missile_group_trails(struct Moby *moby) __asm__("FUN_L18_002f0390");

void draw_drek_fight_missile_group_trails(struct Moby *moby) {
    struct GeometryQuad quads[16];
    float               extra[8];
    short              *group_entry;
    struct Moby        *missile_moby;
    char               *data;
    float               offset;
    float               color_blend_factor;
    int                 i;
    int                 j;
    int                 k;
    int                 trail_segment_index;
    int                 base;

    if (D_L18_00162238_gp != 0) {
        return;
    }
    for (i = 0; i < 16; i++) {
        quads[i].texture = get_effect_texture(19);
        quads[i].reserved.value = 0;
        quads[i].texture_state = 0xFF9000000260ULL;
        quads[i].primitive = (u64)drek_missile_trail_blend_minuend_selector_gp |
                             ((u64)drek_missile_trail_blend_subtrahend_selector_gp << 2) |
                             ((u64)drek_missile_trail_blend_factor_selector_gp << 4) |
                             ((u64)drek_missile_trail_blend_addend_selector_gp << 6) |
                             0x8000000000ULL;
        offset = convert_integer_to_float((game_frame + 3) & 3) * 0.0625f;
        for (j = 0; j < 4; j++) {
            quads[i].texture_coordinates[j][0] = drek_missile_trail_texture_coordinates[j][0];
            quads[i].texture_coordinates[j][1] = drek_missile_trail_texture_coordinates[j][1];
            color_blend_factor = convert_integer_to_float(i + 1 - (j >> 1)) * 0.25f + offset;
            if (color_blend_factor > 1.0f) {
                color_blend_factor = 1.0f;
            } else if (color_blend_factor < 0.0f) {
                color_blend_factor = 0.0f;
            }
            quads[i].colors[j] = fast_tween_color(drek_missile_trail_blend_start_color_gp,
                                                  drek_missile_trail_blend_end_color_gp,
                                                  color_blend_factor);
        }
    }
    memset(extra, 0, 0x20);
    extra[2] = 0.15f;
    extra[6] = -0.15f;
    group_entry = D_L18_001AC240[moby->group];
    do {
    again:
        missile_moby = (struct Moby *)(((*group_entry & 0x7FFF) << 8) + (int)D_L18_0015FFD8);
        if (missile_moby->oclass != 0x54B) {
            goto again;
        }
        data = (char *)missile_moby->pvars;
        for (trail_segment_index = 0; trail_segment_index < *(short *)(data + 0x164);
             trail_segment_index++) {
            base = (*(short *)(data + 0x166) - trail_segment_index + 15) & 15;
            if (drek_missile_trail_draw_disabled_gp == 0) {
                for (k = 0; k < 4; k++) {
                    fast_vec_add(&quads[trail_segment_index].positions[k],
                                 data + 0x10 + (((base + (k >> 1)) & 15) << 4),
                                 extra + ((k & 1) << 2));
                }
                draw_geometry_quad(&quads[trail_segment_index], 0, 0);
            }
        }
    } while (*group_entry++ >= 0);
}

/* Per-frame callback: swings every class 0x54B moby in the group around its base point by its state. */
extern float          D_0015ED6C_c7 __asm__("D_0015ED6C");
extern short         *D_L18_001AC240_c7[] __asm__("D_L18_001AC240");
extern unsigned char *D_L18_0015FFD8_c7 __asm__("D_L18_0015FFD8");
extern int            FUN_001fa6e0_c7(float, int, int) __asm__("FUN_001fa6e0");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

extern float drek_missile_moving_effect_phase_rate_degrees_gp __asm__("D_L18_00162258")
    __attribute__((sda));
extern float drek_missile_wait_to_explode_effect_phase_rate_degrees_gp __asm__("D_L18_0016225C")
    __attribute__((sda));
extern int drek_missile_effect_blend_start_color_gp __asm__("D_L18_00162260") __attribute__((sda));
extern int drek_missile_effect_blend_end_color_gp __asm__("D_L18_00162264") __attribute__((sda));
extern float drek_missile_effect_quad_scale_gp __asm__("D_L18_00162268") __attribute__((sda));
extern float drek_missile_effect_position_offset_length_gp __asm__("D_L18_0016226C")
    __attribute__((sda));
void render_drek_fight_missile_group_effects(struct Moby *group_moby) __asm__("FUN_L18_002f06c0");

void render_drek_fight_missile_group_effects(struct Moby *group_moby) {
    float          effect_world_position[4];
    short         *group_entry;
    unsigned char *missile_moby;
    char          *missile_vars;
    float          moving_phase_step;
    float          wait_to_explode_phase_step;
    int            phase_blended_color;
    if (D_L18_00162238_gp != 0) {
        return;
    }
    group_entry = D_L18_001AC240_c7[group_moby->group];
    moving_phase_step =
        drek_missile_moving_effect_phase_rate_degrees_gp * DEG_TO_RAD * D_0015ED6C_c7;
    wait_to_explode_phase_step =
        drek_missile_wait_to_explode_effect_phase_rate_degrees_gp * DEG_TO_RAD * D_0015ED6C_c7;
    do {
        missile_moby = D_L18_0015FFD8_c7 + ((*group_entry & 0x7FFF) << 8);
        if (MOBY(missile_moby)->oclass == 0x54B) {
            missile_vars = ((char *)MOBY(missile_moby)->pvars);
            phase_blended_color =
                FUN_001fa6e0_c7(fast_sin(*(float *)(missile_vars + 0x168)) * 0.5f + 0.5f,
                                drek_missile_effect_blend_start_color_gp,
                                drek_missile_effect_blend_end_color_gp);
            fast_vec_normalize(effect_world_position,
                               missile_moby + 0xC0,
                               drek_missile_effect_position_offset_length_gp);
            fast_vec_add(effect_world_position, effect_world_position, missile_moby + 0x10);
            switch (missile_moby[0x20]) {
            case DREK_MISSILE_WANDER:
            case DREK_MISSILE_DIVE:
                draw_offset_textured_effect_quad(effect_world_position,
                                                 phase_blended_color,
                                                 drek_missile_effect_quad_scale_gp,
                                                 0.0f);
                *(float *)(missile_vars + 0x168) =
                    fast_add_rotations(*(float *)(missile_vars + 0x168), moving_phase_step);
                break;
            case DREK_MISSILE_WAIT_TO_EXPLODE:
                draw_offset_textured_effect_quad(effect_world_position,
                                                 phase_blended_color,
                                                 drek_missile_effect_quad_scale_gp,
                                                 0.0f);
                *(float *)(missile_vars + 0x168) = fast_add_rotations(
                    *(float *)(missile_vars + 0x168), wait_to_explode_phase_step);
                break;
            }
        }
    } while (*group_entry++ >= 0);
}

#include "qcopy.h"

/* Advance a ring of recorded positions, store the moby's position, and register draw callbacks once per frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002F1C38), where it is exact; names translated to the US level program. */

extern void draw_drek_fight_missile_group_trails(struct Moby *moby) __asm__("FUN_L18_002f0390");
void        render_drek_fight_missile_group_effects(struct Moby *m) __asm__("FUN_L18_002f06c0");

extern short drek_missile_effect_callbacks_last_queued_frame_gp __asm__("D_L18_00162230")
    __attribute__((sda));

void update_drek_fight_missile_trail_and_queue_effects(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   trail_slot_byte_offset;
    int   current_game_frame;
    int   old;
    if ((game_frame & 3) == 0) {
        *(unsigned short *)(data + 0x166) = (*(unsigned short *)(data + 0x166) + 1) & 0xF;
        if ((float)*(short *)(data + 0x164) < 4.0f) {
            *(unsigned short *)(data + 0x164) = *(unsigned short *)(data + 0x164) + 1;
        }
    }
    trail_slot_byte_offset = *(short *)(data + 0x166) << 4;
    qcopy((char *)(trail_slot_byte_offset + (int)data) + 0x10, &moby->pos);
    current_game_frame = game_frame;
    old = *(int *)&drek_missile_effect_callbacks_last_queued_frame_gp;
    *(float *)(data + trail_slot_byte_offset + 0x1C) = 1.0f;
    if (current_game_frame != old) {
        *(int *)&drek_missile_effect_callbacks_last_queued_frame_gp = current_game_frame;
        add_draw_callback(draw_drek_fight_missile_group_trails, moby);
        add_draw_callback(render_drek_fight_missile_group_effects, moby);
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F2AE0), where it is exact; names translated to the US level program. */

typedef struct {
    int  idx;
    char pad[0x2C];
} VEnt;

typedef struct {
    int   pad0;
    int   i4;
    int   i8;
    char  padC[4];
    char  v10[0x10];
    char  v20[0x10];
    char  pad30[0x20];
    VEnt  ent[15];
    char  pad320[0x10];
    float f330;
    float f334;
    int   cnt;
    char  pad33C[4];
    float f340;
    float f344;
    int   f348;
    int   f34C;
    int   f350;
} VData;

typedef struct {
    char           pad[0x2FC];
    unsigned char *p2FC;
    char           pad2[14];
    short          s30E;
} VG;

typedef struct {
    char pad0[0x30];
    char update_data[0x300];
    int  field330;
    char pad334[4];
    int  field338;
    char pad33C[8];
    int  field344;
} Level18VendorData;

typedef struct {
    char               pad0[0x20];
    unsigned char      state;
    char               pad21[0x57];
    Level18VendorData *data;
    char               pad7C[0x2A];
    short              class_id;
} Level18VendorMoby;

extern char *D_L18_001600EC;

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_linked_moby_group_transform_and_fall_2.h"

extern float D_L18_001677D8 NOT_SDA;
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float fast_add_rotations(float, float);
extern float fast_cos(float);
extern float fast_sin(float);
extern int   random_integer_below(int);
extern int   scale_game_frames(int);
extern float D_L18_0015F580_macro[] __asm__("D_L18_0015F580") MACRO_ADDR;

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_linked_moby_group_transform_and_fall.h"

extern void spawn_debris_chunk(
    void *, void *, unsigned int, int, float, float, float, float, int) __asm__("FUN_L01_002f8530");
extern void
capture_linked_moby_relative_transforms(char *, void *, int) __asm__("FUN_L12_0027b268");
extern void mark_class_0630_moby_and_child_for_removal(void *) __asm__("FUN_L18_002fa8e0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

void update_linked_moby_group_transform_and_fall(struct Moby *moby) __asm__("FUN_L18_002f16f0");

void update_linked_moby_group_transform_and_fall(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_APPROACH_TARGET_TRANSFORM = 2,
        MOBY_FALL_AND_REMOVE = 4
    };
    VData *moby_vars = (VData *)moby->pvars;
    if (moby_vars) {
        float initial_acceleration_step = group_transform_initial_acceleration_gp * frame_time_sq;
        float initial_braking_step = group_transform_initial_braking_gp * frame_time_sq;
        float initial_maximum_speed = group_transform_initial_maximum_speed_gp * frame_time;
        float later_acceleration_step = group_transform_later_acceleration_gp * frame_time_sq;
        float later_braking_step = group_transform_later_braking_gp * frame_time_sq;
        float later_maximum_speed = group_transform_later_maximum_speed_gp * frame_time;
        switch (moby->state) {
        case MOBY_INITIALIZE: {
            short **tp;
            short  *t;
            int     j;
            qcopy(moby_vars->v10, &moby->pos);
            qcopy(moby_vars->v20, &moby->rot);
            tp = D_L18_001AC240 + moby->group;
            j = 0;
            t = *tp;
            if (t == 0)
                return;
            while (j < 16) {
                int idx = (unsigned short)*t & 0x7FFF;
                if (*(short *)((idx << 8) + (int)D_L18_0015FFD8 + 0xA6) == 0x630) {
                    moby_vars->ent[j].idx = idx;
                    j++;
                }
                if (*t++ < 0)
                    break;
            }
            moby_vars->f348 = 0;
            moby_vars->f34C = scale_game_frames(30);
            moby_vars->cnt = j;
            capture_linked_moby_relative_transforms(moby, (char *)moby_vars + 0x30, j);
            moby->state = 1;
            break;
        }
        case MOBY_APPROACH_TARGET_TRANSFORM: {
            int            hero_on_group_or_class_764_moby;
            int            i;
            unsigned char *hero_ground_moby;
            if (group_transform_advance_enabled_gp != 0) {
                if (moby_vars->f330 < 1.5f) {
                    advance_accelerated_scalar(1.5f,
                                               initial_acceleration_step,
                                               initial_braking_step,
                                               initial_maximum_speed,
                                               &moby_vars->f330,
                                               &moby_vars->f334);
                } else {
                    advance_accelerated_scalar(2.0f,
                                               later_acceleration_step,
                                               later_braking_step,
                                               later_maximum_speed,
                                               &moby_vars->f330,
                                               &moby_vars->f334);
                }
            }
            if (moby_vars->f330 > group_transform_debris_start_progress_threshold_gp &&
                moby_vars->f344 == 0.0f) {
                moby_vars->f344 = *(int *)&group_transform_debris_initial_random_bound_gp;
            }
            if (moby_vars->f330 > group_transform_fall_progress_threshold_gp &&
                moby_vars->i8 != 0) {
                moby->state = 4;
            }
            hero_on_group_or_class_764_moby = 0;
            for (i = 0; i < moby_vars->cnt; i++) {
                unsigned char *group_moby = D_L18_0015FFD8 + (moby_vars->ent[i].idx << 8);
                if (hero.ground_moby == group_moby) {
                    moby_vars->f348 = 1;
                    hero_on_group_or_class_764_moby = 1;
                    break;
                }
            }
            hero_ground_moby = hero.ground_moby;
            if (hero_ground_moby != 0) {
                short s = MOBY(hero_ground_moby)->oclass;
                if (s == 0x764)
                    hero_on_group_or_class_764_moby = 1;
            }
            if (moby_vars->f348 != 0 && hero_on_group_or_class_764_moby == 0 &&
                hero.air_frames.s == 0) {
                if (fast_dec_timer(&moby_vars->f34C))
                    moby->state = 4;
            }
            break;
        }
        case MOBY_FALL_AND_REMOVE: {
            float t = frame_time_sq * 5.0f;
            moby->pos.z += moby_vars->f340;
            moby_vars->f340 = moby_vars->f340 - t;
            if (moby->pos.z < 20.0f) {
                int i;
                for (i = 0; i < moby_vars->cnt; i++) {
                    mark_class_0630_moby_and_child_for_removal(D_L18_0015FFD8 +
                                                               (moby_vars->ent[i].idx << 8));
                }
                mark_moby_for_removal(moby);
                return;
            }
            break;
        }
        }
        if ((unsigned char)(moby->state - 1) < 2) {
            lerp_vector(&moby->pos,
                        moby_vars->v10,
                        D_L18_001600EC + (moby_vars->i4 << 7) + 0x30,
                        moby_vars->f330 * 0.5f);
            lerp_vector(&moby->rot,
                        moby_vars->v20,
                        D_L18_001600EC + (moby_vars->i4 << 7) + 0x70,
                        moby_vars->f330 * 0.5f);
        }
        apply_linked_moby_relative_transforms(moby, (char *)moby_vars + 0x30, moby_vars->cnt);
        if (moby_vars->f344 != 0.0f) {
            if (moby->state != 4) {
                float debris_spawn_position[4];
                float s, z, debris_spawn_angle;
                int   debris_spawn_frames;
                moby_vars->f344 = moby_vars->f344 + 0.25f;
                if (random_integer_below(truncate_float_to_s32(moby_vars->f344)) == 0) {
                    s = random_float_between(-1.0471976f, 1.0471976f);
                    debris_spawn_angle = fast_add_rotations(s, D_L18_001677D8);
                    z = random_float_between(0.0f, 20.0f);
                    debris_spawn_position[0] = fast_cos(debris_spawn_angle) * z;
                    debris_spawn_position[1] = fast_sin(debris_spawn_angle) * z;
                    debris_spawn_position[2] = 15.0f;
                    fast_vec_add(debris_spawn_position, debris_spawn_position, &hero.motion.pos);
                    debris_spawn_frames = scale_game_frames(240);
                    spawn_debris_chunk(debris_spawn_position,
                                       D_L18_0015F580_macro,
                                       0x2B8,
                                       debris_spawn_frames,
                                       group_transform_debris_scale_multiplier_gp,
                                       2.0f,
                                       1.0f,
                                       0.75f,
                                       0);
                }
            }
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3038), where it is exact; names translated to the US level program. */

extern void mark_block_entries_within_transform_xy_bounds(int, int) __asm__("FUN_L18_002f1d08");
extern void set_group_class_0630_moby_byte_bc(int, int) __asm__("FUN_L18_002fa888");

void activate_drek_boss_class_0565_moby(struct Moby *moby) __asm__("FUN_L18_002f1c38");

void activate_drek_boss_class_0565_moby(struct Moby *moby) {
    char *state = (char *)moby->pvars;
    if (moby->oclass == 0x565 && moby->state == 1) {
        if (*(int *)(state + 0xC) != -1) {
            mark_block_entries_within_transform_xy_bounds(0x802, *(int *)(state + 0xC));
            mark_block_entries_within_transform_xy_bounds(0x908, *(int *)(state + 0xC));
            mark_block_entries_within_transform_xy_bounds(0x954, *(int *)(state + 0xC));
        }
        moby->state = 2;
        set_group_class_0630_moby_byte_bc(moby->group, 1);
    }
}
/* Reset this vendor moby's counters before its per-frame update. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F30C8), where it is exact; names translated to the US level program. */

typedef struct {
    char               pad0[0x20];
    unsigned char      state;
    char               pad21[0x57];
    struct L18VD_1cc8 *data;
    char               pad7C[0x2A];
    short              class_id;
} L18VM_1cc8;

typedef struct L18VD_1cc8 {
    char pad0[0x30];
    char update_data[0x300];
    int  field330;
    char pad334[4];
    int  field338;
    char pad33C[8];
    int  field344;
} L18VD_1cc8;

void reset_drek_boss_class_0565_moby(L18VM_1cc8 *moby) __asm__("FUN_L18_002f1cc8");

void reset_drek_boss_class_0565_moby(L18VM_1cc8 *moby) {
    L18VD_1cc8 *data = moby->data;
    if (moby->class_id == 0x565) {
        moby->state = 1;
        data->field330 = 0;
        data->field344 = 0;
    }
    apply_linked_moby_relative_transforms(moby, data->update_data, data->field338);
}
/* For each entry of block D_L18_001C6480[D_L18_001C6780[A]]: runs func_001F9BF0 /
   func_001F9EC0 against the records at D_L18_001600EC + B * 0x80 (+0x30, +0x40)
   for the entry's vector and, when both of the result's first two components are
   within 1.0, clears the entry's +0x0C and sets its +0x14 to 1.0. The index is
   copied (k = i) for the two stores and the loop tests `n > i`, as decomp-permuter
   found for retail's register use. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3108), where it is exact; names translated to the US level program. */

typedef struct {
    int   pad[3];
    int   w0c;
    int   pad2;
    float w14;
    int   pad3[2];
} Ent;

typedef struct {
    char  pad[0x26];
    short n;
    Ent  *e;
} Blk;

extern char         *D_L18_001C6200[];
extern unsigned char l18_block_indices_by_lookup_index[] __asm__("D_L18_001C6500");

void mark_block_entries_within_transform_xy_bounds(int block_lookup_index,

                                                   int transform_record_index) {
    int i = 0;
    int k;
    if (((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])->n > i)
        do {
            float entry_offset[4];
            fast_vec_sub(
                entry_offset,
                ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])->e +
                    i,
                D_L18_001600EC + (transform_record_index << 7) + 0x30);
            transform_vector_by_basis(
                entry_offset, entry_offset, D_L18_001600EC + (transform_record_index << 7) + 0x40);
            k = i;
            if (absolute_float(entry_offset[0]) < 1.0f && absolute_float(entry_offset[1]) < 1.0f) {
                ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])
                    ->e[k]
                    .w0c = 0;
                ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])
                    ->e[k]
                    .w14 = 1.0f;
            }
        } while (++i <
                 ((Blk *)D_L18_001C6200[l18_block_indices_by_lookup_index[block_lookup_index]])->n);
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3268), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern int FUN_0022da68(int, int, int);
void       draw_glow_moby_layered_quads(struct Moby *moby) __asm__("FUN_L18_002f1fd8");

extern void draw_glow_moby_layered_quads(struct Moby *) __asm__("FUN_L18_002f1fd8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

void update_glow_moby_state_and_voice(struct Moby *moby) __asm__("FUN_L18_002f1e68");

void update_glow_moby_state_and_voice(struct Moby *moby) {
    enum {
        GLOW_INITIALIZE = 0,
        GLOW_ACTIVE = 1,
        GLOW_FADE_OUT = 2
    };
    int *moby_vars = (int *)moby->pvars;
    switch (moby->state) {
    case GLOW_INITIALIZE:
        moby->state = 1;
        ((float *)moby_vars)[3] = 1.0f;
        break;
    case GLOW_ACTIVE:
        add_draw_callback((void (*)(void))draw_glow_moby_layered_quads, moby);
        if (voice_is_playing(moby, moby_vars[1]) == 0) {
            moby_vars[1] = FUN_0022da68(0, 4, (int)moby);
        }
        if (moby_vars[0] != -1) {
            unsigned char *linked_moby = D_L18_0015FFD8 + (moby_vars[0] << 8);
            if (MOBY(linked_moby)->oclass == 0x267 && linked_moby[0x20] >= 3) {
                moby->collision = 0;
                moby->state = 2;
                if (voice_is_playing(moby, moby_vars[1]) != 0) {
                    int i = moby_vars[1];
                    if (i != -1) {
                        char *voice_slot_entry = ((char *)&voice_pool) + i * 0x70;
                        if (*(unsigned char **)(voice_slot_entry + 0x88) == moby &&
                            *(unsigned char *)(voice_slot_entry + 0x74) != 0) {
                            release_voice_slot(i);
                        }
                    }
                    moby_vars[1] = -1;
                }
            }
        }
        break;
    case GLOW_FADE_OUT:
        approach_value((float *)(moby_vars + 3), 0.0f, frame_time + frame_time);
        break;
    }
}

#include "rnc/rendering/geometry_quad.h"

struct GlowVars {
    u8  pad0[8];
    f32 scroll;
    f32 max_alpha;
};

extern u64   get_effect_texture_1fd8(int) __asm__("FUN_001f44b8");
extern void  draw_quad_1fd8(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_0015ED6C;
extern f32   glow_moby_phase_rate_gp __asm__("D_L18_001622E0") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/draw_glow_moby_layered_quads.h"

extern int glow_moby_phase_blend_start_color_gp __asm__("D_L18_001622C8") __attribute__((sda));
extern int glow_moby_phase_blend_end_color_gp __asm__("D_L18_001622CC") __attribute__((sda));
extern f32 glow_moby_layer_z_step_gp __asm__("D_L18_001622D0") __attribute__((sda));
extern f32 D_L18_001622D4 __attribute__((sda));

f32 D_L18_001DA350[4][2] = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {7.0f, 0.0f},
    {7.0f, 1.0f},
};

extern u128 glow_moby_quad_base_positions[4] __asm__("D_L18_001DA370");

void draw_glow_moby_layered_quads(struct Moby *moby) {
    struct GeometryQuad quads[4];
    float               matrix[4][4] __attribute__((aligned(16)));
    struct GlowVars    *vars;
    float               alpha;
    float               step;
    float               phase;
    int                 color;
    int                 i;
    int                 j;
    int                 pass;

    vars = (struct GlowVars *)moby->pvars;
    copy_matrix_rotation(matrix, &moby->rot_row0);
    qcopy(matrix[3], &moby->pos);
    matrix[3][3] = 1.0f;
    vars->scroll += glow_moby_phase_rate_gp * D_0015ED6C;
    if (1.0f < vars->scroll) {
        vars->scroll -= 1.0f;
    }
    alpha = (48.0f - fast_vec_dist(&moby->pos, D_L18_001677C0)) * 0.25f;
    if (vars->max_alpha < alpha) {
        alpha = vars->max_alpha;
    } else if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    step = 0.25f;
    for (i = 0; i < 4; i++) {
        quads[i].texture = get_effect_texture_1fd8(0xE);
        quads[i].reserved.value = 0;
        quads[i].texture_state = 0xFF9000000260ULL;
        quads[i].primitive = (u64)glow_moby_blend_minuend_selector_gp |
                             ((u64)glow_moby_blend_subtrahend_selector_gp << 2) |
                             ((u64)glow_moby_blend_factor_selector_gp << 4) |
                             ((u64)glow_moby_blend_addend_selector_gp << 6) | 0x8000000000ULL;
        phase = vars->scroll + i * step;
        phase = phase - (float)truncate_float_to_s32(phase);
        color = fast_tween_color(
            glow_moby_phase_blend_start_color_gp, glow_moby_phase_blend_end_color_gp, phase);
        color = fast_tween_color(color & 0xFFFFFF, color, alpha);
        for (j = 0; j < 4; j++) {
            quads[i].texture_coordinates[j][0] = D_L18_001DA350[j][0] + i * step;
            quads[i].texture_coordinates[j][1] = D_L18_001DA350[j][1];
            quads[i].colors[j] = color;
            qcopy_nc(&quads[i].positions[j], &glow_moby_quad_base_positions[j]);
            if (j & 1) {
                quads[i].positions[j].f[2] += phase * D_L18_001622D4;
            } else {
                quads[i].positions[j].f[2] -= phase * D_L18_001622D4;
            }
        }
    }
    for (pass = 2; pass >= 0; pass--) {
        matrix[3][2] += glow_moby_layer_z_step_gp;
        draw_quad_1fd8(&quads[0], matrix, 0);
        draw_quad_1fd8(&quads[1], matrix, 0);
        draw_quad_1fd8(&quads[2], matrix, 0);
        draw_quad_1fd8(&quads[3], matrix, 0);
    }
}

/* Runs the lift's state machine, then queues the draw callback for its current look. */
typedef struct {
    char           p0[0x34];
    unsigned short h34;
} M_2310;

extern float D_L18_00162308_gp __asm__("D_L18_00162308") __attribute__((sda));
extern float D_0015ED6C_2310 __asm__("D_0015ED6C");
extern float approach_2310(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
void         synchronize_lift_group_height(void *source_moby) __asm__("FUN_L18_002f2598");

extern void synchronize_lift_group_height(void *a) __asm__("FUN_L18_002f2598");
void        draw_lift_25_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2620");

extern void draw_lift_25_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2620");
void        draw_lift_38_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f27c8");

extern void draw_lift_38_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f27c8");
void        draw_lift_22_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2970");

extern void draw_lift_22_quad_mesh(struct Moby *moby) __asm__("FUN_L18_002f2970");

extern int   lift_alternate_height_requested_gp __asm__("D_L18_001622FC") __attribute__((sda));
extern int   next_lift_instance_index_gp __asm__("D_L18_001622F8") __attribute__((sda));
extern float lift_texture_u_scroll_rate_gp __asm__("D_L18_0016230C") __attribute__((sda));
extern unsigned char D_00141400 NOT_SDA;
extern unsigned char D_0013D406 NOT_SDA;
void update_lift_state_and_queue_draw(struct Moby *lift_moby) __asm__("FUN_L18_002f2310");

void update_lift_state_and_queue_draw(struct Moby *lift_moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_SELECT_INITIAL_HEIGHT = 1,
        LIFT_APPROACH_BASE_HEIGHT = 2,
        LIFT_APPROACH_ALTERNATE_HEIGHT = 3
    };
    enum {
        LIFT_DRAW_22_QUAD_MESH = 0,
        LIFT_DRAW_25_QUAD_MESH = 1,
        LIFT_DRAW_38_QUAD_MESH = 2
    };
    char *lift_vars = (char *)lift_moby->pvars;
    int   initial_height_selector;
    switch (lift_moby->state) {
    case LIFT_INITIALIZE:
        if (lift_alternate_height_requested_gp != 0) {
            D_00141400 = *(unsigned char *)&lift_alternate_height_requested_gp;
        }
        *(int *)(lift_vars + 0x1C) = next_lift_instance_index_gp++;
        lift_moby->update_dist = 0xFF;
        lift_moby->state = 1;
        ((M_2310 *)lift_moby)->h34 |= 1;
        lift_moby->draw_dist = 0;
        lift_moby->collision = 0;
        break;
    case LIFT_SELECT_INITIAL_HEIGHT:
        if (D_0013D406 != 0) {
            *(int *)(lift_vars + 8) = (*(int *)(lift_vars + 8) + 1) & 1;
        }
        initial_height_selector = *(int *)(lift_vars + 8);
        next_lift_instance_index_gp = 0;
        if (initial_height_selector != 0 && D_00141400 == 0) {
            lift_moby->state = 2;
            lift_moby->pos.z = *(float *)(lift_vars + 0);
        } else {
            lift_moby->state = 3;
            lift_moby->pos.z = *(float *)(lift_vars + 4);
        }
        break;
    case LIFT_APPROACH_ALTERNATE_HEIGHT:
        if (lift_moby->command & 4) {
            lift_alternate_height_requested_gp = 0;
            lift_moby->state = 2;
            if (*(int *)(lift_vars + 8) == 0) {
                D_0013D406 = 1;
            }
        }
        approach_2310(&lift_moby->pos.z,
                      (float *)(lift_vars + 0x18),
                      *(float *)(lift_vars + 4),
                      frame_time_sq * 4.0f,
                      frame_time_sq * 4.0f,
                      D_0015ED6C_2310 * 4.0f);
        synchronize_lift_group_height(lift_moby);
        lift_moby->command = 1;
        break;
    case LIFT_APPROACH_BASE_HEIGHT:
        if (lift_moby->command & 8) {
            lift_moby->state = 3;
            lift_alternate_height_requested_gp = 1;
        }
        lift_moby->command = 2;
        synchronize_lift_group_height(lift_moby);
        approach_2310(&lift_moby->pos.z,
                      (float *)(lift_vars + 0x18),
                      *(float *)(lift_vars + 0),
                      frame_time_sq * 4.0f,
                      frame_time_sq * 4.0f,
                      D_0015ED6C_2310 * 4.0f);
        break;
    }
    switch (*(int *)(lift_vars + 0x14)) {
    case LIFT_DRAW_25_QUAD_MESH:
        D_L18_00162308_gp = D_L18_00162308_gp + lift_texture_u_scroll_rate_gp * D_0015ED6C_2310;
        if (1.0f < D_L18_00162308_gp) {
            D_L18_00162308_gp -= 1.0f;
        }
        add_draw_callback(draw_lift_25_quad_mesh, lift_moby);
        break;
    case LIFT_DRAW_38_QUAD_MESH:
        add_draw_callback(draw_lift_38_quad_mesh, lift_moby);
        break;
    case LIFT_DRAW_22_QUAD_MESH:
        add_draw_callback(draw_lift_22_quad_mesh, lift_moby);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F39F8), where it is exact; names translated to the US level program. */

typedef struct {
    int pad[7];
    int idx;
} Lx;

typedef struct {
    char  pad[0x18];
    float f;
    char  pad1[0x78 - 0x1C];
    Lx   *l;
    char  pad2[0xA6 - 0x7C];
    short kind;
    char  pad3[0x100 - 0xA8];
} Mx;

typedef struct {
    int   pad[2];
    float f;
    char  pad2[0x1190 - 12];
} E;

extern E      l18_lift_height_records[] __asm__("D_L18_001DB500");
extern short *D_L18_001AC240_c[] __asm__("D_L18_001AC240");

void synchronize_lift_group_height(void *source_moby) {
    unsigned char *source_moby_bytes = source_moby;
    short         *group_entry = D_L18_001AC240_c[source_moby_bytes[0x21]];
    E             *height_records = l18_lift_height_records;
    do {
        Mx *group_moby =
            (Mx *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) + (int)D_L18_0015FFD8);
        group_moby->f = *(float *)(source_moby_bytes + 0x18);
        if (group_moby->kind == 0x57A) {
            height_records[group_moby->l->idx].f = *(float *)(source_moby_bytes + 0x18);
        }
    } while (*group_entry++ >= 0);
}

#include "qcopy.h"

/* Draws 25 textured quads from the L18 tables, in the moby's frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3A80), where it is exact; names translated to the US level program. */

typedef float FVec4[4] __attribute__((aligned(16)));

typedef struct {
    FVec4 v;
} FRow;

extern short lift_mesh_blend_minuend_selector_gp __asm__("D_L18_00162310") __attribute__((sda));
extern short lift_mesh_blend_subtrahend_selector_gp __asm__("D_L18_00162314") __attribute__((sda));
extern short lift_mesh_blend_factor_selector_gp __asm__("D_L18_00162318") __attribute__((sda));
extern short lift_mesh_blend_addend_selector_gp __asm__("D_L18_0016231C") __attribute__((sda));
extern short lift_mesh_blend_upper_word_gp __asm__("D_L18_00162320") __attribute__((sda));
extern short lift_mesh_effect_texture_index_gp __asm__("D_L18_00162324") __attribute__((sda));
extern short lift_mesh_quad_color_gp __asm__("D_L18_00162328") __attribute__((sda));
extern f32   D_L18_00162308_gp __asm__("D_L18_00162308") __attribute__((sda));
extern float D_L18_001DA7A0[][2] NOT_SDA;
extern FRow  D_L18_001DA3C0[] NOT_SDA;
extern short D_L18_001DA610[][4][2] NOT_SDA;
extern int   get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void  fast_draw_quad_real(void *, void *, int) __asm__("FUN_001f7d30");

void draw_lift_25_quad_mesh(struct Moby *moby) {
    FRow          quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *data = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture_alt((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)data;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 25; i++) {
        float ofs = *(float *)&D_L18_00162308_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = D_L18_001DA610[i][j][0];
            int uv_index = D_L18_001DA610[i][j][1];
            qcopy(&quad_positions[j], &D_L18_001DA3C0[vertex_index]);
            uv[j][0] = D_L18_001DA7A0[uv_index][0] + ofs;
            uv[j][1] = D_L18_001DA7A0[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

typedef float FVec4_l18[4] __attribute__((aligned(16)));

typedef struct {
    FVec4_l18 v;
} FRow_l18;

extern float    D_L18_001DAE80[][2] NOT_SDA;
extern FRow_l18 D_L18_001DA8D0[] NOT_SDA;
extern short    D_L18_001DAC20[][4][2] NOT_SDA;
extern int      get_effect_texture_l18(int) __asm__("FUN_001f44b8");

void draw_lift_38_quad_mesh(struct Moby *moby) {
    FRow_l18      quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *data = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture_l18((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)data;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 38; i++) {
        float ofs = *(float *)&D_L18_00162308_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = D_L18_001DAC20[i][j][0];
            int uv_index = D_L18_001DAC20[i][j][1];

            qcopy(&quad_positions[j], &D_L18_001DA8D0[vertex_index]);
            uv[j][0] = D_L18_001DAE80[uv_index][0] + ofs;
            uv[j][1] = D_L18_001DAE80[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

#include "qcopy.h"

/* Draws 22 textured quads from the L18 tables, in the moby's frame. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3DD0), where it is exact; names translated to the US level program. */

extern float    D_L18_001DB3A0[][2] NOT_SDA;
extern FRow_l18 D_L18_001DB030[] NOT_SDA;
extern short    D_L18_001DB240[][4][2] NOT_SDA;

void draw_lift_22_quad_mesh(struct Moby *moby) {
    FRow_l18      quad_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         height_offset_transform[4][4];
    char         *data = (char *)moby->pvars;
    int           i;
    int           j;

    pkt[1] = get_effect_texture_l18((*(int *)&lift_mesh_effect_texture_index_gp));
    pkt[3] = (*(int *)&lift_mesh_blend_minuend_selector_gp) |
             (long)(*(int *)&lift_mesh_blend_subtrahend_selector_gp) << 2;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_factor_selector_gp) << 4;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_addend_selector_gp) << 6;
    pkt[3] |= (long)(*(int *)&lift_mesh_blend_upper_word_gp) << 32;
    pkt[2] = 0xFF9000000260;
    pkt[0] = 0;
    set_identity_matrix(height_offset_transform);
    height_offset_transform[3][2] = moby->pos.z - *(float *)data;
    colors[3] = (*(int *)&lift_mesh_quad_color_gp);
    colors[2] = (*(int *)&lift_mesh_quad_color_gp);
    colors[1] = (*(int *)&lift_mesh_quad_color_gp);
    colors[0] = (*(int *)&lift_mesh_quad_color_gp);
    for (i = 0; i < 22; i++) {
        float ofs = *(float *)&D_L18_00162308_gp;
        for (j = 0; j < 4; j++) {
            int vertex_index = D_L18_001DB240[i][j][0];
            int uv_index = D_L18_001DB240[i][j][1];
            qcopy(&quad_positions[j], &D_L18_001DB030[vertex_index]);
            uv[j][0] = D_L18_001DB3A0[uv_index][0] + ofs;
            uv[j][1] = D_L18_001DB3A0[uv_index][1];
        }
        fast_draw_quad_real(quad_positions, height_offset_transform, 0);
    }
}

typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobys[1];
} ML;

extern ML            D_L18_0016D260;
extern unsigned char l18_drek_boss_big_head_manipulator[] __asm__("D_L18_0017CE40");
extern short drek_boss_big_head_manipulator_scale_gp __asm__("D_L18_001623A4") __attribute__((sda));

void attach_drek_boss_big_head_manipulator(char *moby) __asm__("FUN_L18_002f2b18");

void attach_drek_boss_big_head_manipulator(char *moby) {
    int i;

    if (D_L18_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L18_0016D260.count; i++) {
            char *listed_moby = D_L18_0016D260.mobys[i];

            if (MOBY(listed_moby)->oclass == 0x4E1) {
                unsigned char *big_head_manipulator = l18_drek_boss_big_head_manipulator;

                if (big_head_manipulator[1] == 0) {
                    attach_manipulator(listed_moby, 0, big_head_manipulator);
                    *(float *)(big_head_manipulator + 0x20) =
                        *(float *)&drek_boss_big_head_manipulator_scale_gp;
                    *(float *)(big_head_manipulator + 0x24) =
                        *(float *)&drek_boss_big_head_manipulator_scale_gp;
                    *(float *)(big_head_manipulator + 0x28) =
                        *(float *)&drek_boss_big_head_manipulator_scale_gp;
                }
            }
        }
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Update of the final boss robot (class 1422): one step of its state machine, then its bob, aim and tint. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F4050), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobs[1];
} MobyList;

typedef struct {
    float v[4];
} __attribute__((aligned(16))) BossQ;

typedef unsigned int BossTI __attribute__((mode(TI), aligned(16)));

typedef struct BossMoby {
    char           p0[0x10];
    float          f10[4];
    unsigned char  f20;
    char           p21[3];
    int           *f24;
    char           p28[8];
    unsigned char  f30;
    unsigned char  f31;
    short          f32;
    unsigned short f34;
    char           p36[0xA];
    float          f40[4];
    unsigned char  f50;
    char           p51;
    unsigned char  f52;
    unsigned char  f53;
    char           p54[0x1C];
    unsigned char  f70;
    char           p71[7];
    char          *f78;
    char           p7C[3];
    unsigned char  f7F;
    char           p80[0x10];
    int            f90;
    int            f94;
    char           p98[0xE];
    short          fA6;
    char           pA8[0x18];
    float          fC0[4];
    float          fD0[4];
} BossMoby;

typedef struct BossData {
    char           p0[0x20];
    float          f20;
    short          f24;
    char           p26[2];
    unsigned char  f28;
    char           p29[7];
    float          f30;
    char           p34[0x3C];
    float          f70[4];
    char           p80[0x30];
    BossMoby      *fB0;
    int            fB4;
    char           pB8[8];
    char           fC0[0x68];
    float          f128;
    char           p12C[0x14];
    char           f140[0x80];
    unsigned char  f1C0[0x40];
    int            f200;
    int            f204[3];
    int            f210;
    int            f214;
    char           p218[4];
    int            f21C;
    int            f220;
    char           p224[0xC];
    int            f230[4];
    int            f240[4];
    int            f250[4];
    int            f260[4];
    int            f270;
    int            f274;
    char           p278[8];
    int            f280[3];
    char           p28C[8];
    int            f294;
    int            f298;
    int            f29C;
    int            f2A0;
    char           p2A4[0xC];
    int            f2B0;
    char           p2B4[0x4C];
    float          f300[4];
    float          f310[4];
    float          f320[4];
    BossMoby      *f330;
    unsigned char *f334;
    char          *f338;
    unsigned char *f33C;
    int           *f340;
    int            f344;
    int            f348;
    int            f34C;
    int            f350;
    int            f354;
    int            f358;
    int            f35C;
    int            f360;
    int            f364;
    int            f368;
    char           p36C[4];
    float          f370;
    float          f374;
    char           p378[0x14];
    float          f38C;
    float          f390;
    char           p394[4];
    int            f398;
    int            f39C;
    void          *f3A0;
    char          *f3A4;
    int            f3A8;
    int            f3AC;
    int            f3B0;
    float          f3B4;
    int            f3B8;
    float          f3BC;
    float          f3C0[4];
    float          f3D0;
    float          f3D4;
    float          f3D8;
    float          f3DC;
    float          f3E0;
} BossData;

typedef struct BossHero {
    char          p0[0x80];
    float         f80[4];
    float         f90[4];
    char          pA0[0x20];
    float         fC0[4];
    char          pD0[0x22C];
    BossMoby     *f2FC;
    char          p300[0xE];
    short         f30E;
    char          p310[0x250];
    int          *f560;
    char          p564[0x74];
    BossMoby     *f5D8;
    char          p5DC[0x1AA4];
    BossMoby     *f2080;
    int           f2084;
    int           f2088;
    int           f208C;
    char          p2090[0x14];
    unsigned char f20A4;
    char          p20A5[0x1F];
    int           f20C4;
} BossHero;

typedef struct BossShared {
    char          p0[0x10];
    float         f10[4];
    float         f20[4];
    int           f30;
    int           f34;
    char          p38[0xE];
    short         f46;
    char          p48[2];
    unsigned char f4A;
} BossShared;

extern MobyList D_L18_0016D260_c __asm__("D_L18_0016D260");
extern char    *create_rocket_moby(float *pos,
                                   float *dir,
                                   float *target,
                                   float  a,
                                   float  b,
                                   int    frames,
                                   int    kind) __asm__("FUN_L18_002d5348");
extern char    *launch_class_238_moby_from_list(int    owner,
                                                int    idx,
                                                float *from,
                                                float *to,
                                                float  speed,
                                                int    frames) __asm__("FUN_L18_002d5cf8");
extern char    *find_nearest_class_247_moby_in_list(void *pos, int idx) __asm__("FUN_L18_002d6c80");
extern char *
create_class_274_ring_effect(void *owner, void *vector, float value) __asm__("FUN_L18_002dc3e0");
extern char            *
create_class_3d7_moby(void *owner, void *vector, int value) __asm__("FUN_L18_002ea0f0");
extern f32 aas_c(f32, f32, f32, f32, f32 *, f32 *) __asm__("func_00213F38");
extern float D_0015ED6C_e[] __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_L18_0015F3FC;
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float get_position_and_rotation_transition_progress(void *moby) __asm__("FUN_L18_002dfb90");
extern int  *D_L18_001B0EB0[];
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int   FUN_L00_0025e450_u() __asm__("FUN_L00_0025e450");
extern int   classify_moby_state_two_or_three(void *moby) __asm__("FUN_L18_002dfba0");
extern int
activate_drek_fight_missiles_in_group(char *moby, int idx, float *pos) __asm__("FUN_L18_002f0030");
extern int advance_drek_boss_planar_position_toward_target(char *moby,
                                                           float arg,
                                                           void *x) __asm__("FUN_L18_002f6878");
int        select_drek_boss_next_action(struct Moby *moby) __asm__("FUN_L18_002f70b8");

extern int   select_drek_boss_next_action(struct Moby *moby) __asm__("FUN_L18_002f70b8");
extern int   launch_first_idle_group_class_0772_moby(int    idx,
                                                     float *pos,
                                                     float *dir,
                                                     float  speed) __asm__("FUN_L18_002fc668");
extern int   count_group_class_0772_mobys_not_in_state_8(int idx) __asm__("FUN_L18_002fc7e0");
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern s32   allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern short D_L18_00162378_gp __asm__("D_L18_00162378") __attribute__((sda));
extern short l18_drek_boss_scene_2_started_gp __asm__("D_L18_00162360") __attribute__((sda));
extern short D_L18_00162390 __attribute__((sda));
extern short l18_drek_boss_paired_transition_duration_frames_gp __asm__("D_L18_00162394")
    __attribute__((sda));
extern short D_L18_00162398 __attribute__((sda));
extern s32   D_L18_001623A0 __asm__("D_L18_001623A0");
extern short l18_drek_boss_aimed_class_3d7_spawn_frame_count_gp __asm__("D_L18_0016239C")
    __attribute__((sda));
unsigned char *
select_drek_boss_platform_firing_target(struct Moby *self) __asm__("FUN_L18_002f6e10");

extern unsigned char              *
select_drek_boss_platform_firing_target(struct Moby *self) __asm__("FUN_L18_002f6e10");
extern void *find_class_247_moby_with_largest_xy_angle_in_list(float *a,
                                                               float *b,
                                                               int idx) __asm__("FUN_L18_002d6b58");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  FUN_L00_0025a120(void *);
extern void  FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void  queue_movie_with_game_language(int i) __asm__("FUN_L00_00299560");
extern void  start_slideshow(void) __asm__("FUN_L00_00299a68");
extern void  set_class_247_list_data_fields(int idx) __asm__("FUN_L18_002d6d50");
extern void  moby_init_action_mode(void *moby) __asm__("FUN_L18_002d6dc0");
extern void  request_thruster_pack_lock_red_wait(unsigned char *arg,
                                                 int            value) __asm__("FUN_L18_002d7f68");
extern void  end_thruster_pack_lock_red_wait(unsigned char *arg) __asm__("FUN_L18_002d7fd0");
extern void  spawn_class_270_moby(char *pos, float *vec, int arg, float fa, float fb) __asm__("FUN_L18_002db938");
extern void refresh_ring_effect(char *moby, void *pos, float f) __asm__("FUN_L18_002dc458");
extern void start_position_and_rotation_transition(char *moby,
                                                   void *v0,
                                                   void *v1,
                                                   void *v2,
                                                   void *v3,
                                                   void *v4,
                                                   int   a6,
                                                   int   a7,
                                                   int   a8) __asm__("FUN_L18_002dfaa0");
extern void refresh_moby_position_and_activate(unsigned char *arg,
                                               void          *src,
                                               void          *position,
                                               int            active,
                                               float          speed) __asm__("FUN_L18_002ea168");
void        count_drek_boss_platform_event_once(struct Moby *moby) __asm__("FUN_L18_002f5e18");

extern void count_drek_boss_platform_event_once(struct Moby *moby) __asm__("FUN_L18_002f5e18");
extern void update_drek_boss_damage_and_phase_transitions(char *moby) __asm__("FUN_L18_002f5e80");
void        update_drek_boss_hero_follow_movement(struct Moby *moby) __asm__("FUN_L18_002f6960");

extern void update_drek_boss_hero_follow_movement(struct Moby *m) __asm__("FUN_L18_002f6960");
void        update_drek_boss_arena_circling_movement(struct Moby *moby) __asm__("FUN_L18_002f6aa0");

extern void update_drek_boss_arena_circling_movement(struct Moby *moby) __asm__("FUN_L18_002f6aa0");
void        hide_drek_boss_moby_group(int unused, int group_index) __asm__("FUN_L18_002f6fa8");

extern void hide_drek_boss_moby_group(int unused, int idx) __asm__("FUN_L18_002f6fa8");
void        show_drek_boss_moby_group(int unused, int group_index) __asm__("FUN_L18_002f7028");

extern void show_drek_boss_moby_group(int unused, int group_index) __asm__("FUN_L18_002f7028");
extern void set_drek_boss_relative_aim_yaw() __asm__("FUN_L18_002f7220");
void        update_drek_boss_state_dependent_scalars(char *moby) __asm__("FUN_L18_002f7288");

extern void update_drek_boss_state_dependent_scalars(char *moby) __asm__("FUN_L18_002f7288");
void        render_drek_boss_joint_glows(struct Moby *moby) __asm__("FUN_L18_002f7880");

extern void render_drek_boss_joint_glows(struct Moby *moby) __asm__("FUN_L18_002f7880");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void pause_all_sounds(s32) __asm__("FUN_00218d78");
f32         probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
s32         is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");
s32         is_value_within_interpolated_window(void *arg0, f32 fparg0) __asm__("FUN_00214cc8");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
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

void drek_final_boss_robot_update(BossMoby *moby) __asm__("FUN_L18_002f2bf0");
void spawn_drek_boss_firing_effects(char *moby, void *effect_position) __asm__("FUN_L18_002f76a0");

void drek_final_boss_robot_update(BossMoby *moby) {
    enum {
        DREK_BOSS_INITIALIZE = 0,
        DREK_BOSS_WAIT_FOR_ENCOUNTER_TRIGGER = 1,
        DREK_BOSS_CIRCLE_AND_FIRE_AT_HERO = 2,
        DREK_BOSS_WAIT_TO_RESTORE_SAVED_TRANSFORM = 3,
        DREK_BOSS_WAIT_FOR_GROUNDED_HERO_TO_START_SCENE = 4,
        DREK_BOSS_INITIALIZE_PAIRED_TRANSFORM_TRANSITION = 5,
        DREK_BOSS_WAIT_FOR_SCENE = 7,
        DREK_BOSS_WAIT_FOR_PAIRED_TRANSFORM_TRANSITION = 8,
        DREK_BOSS_APPROACH_PATH_START = 9,
        DREK_BOSS_FOLLOW_PATH_AND_FIRE_AT_MOBYS = 10,
        DREK_BOSS_FINISH_PATH_AND_WAIT_FOR_HERO = 0xB,
        DREK_BOSS_SELECT_NEXT_ACTION = 0xC,
        DREK_BOSS_LAUNCH_FORWARD_WITH_LATERAL_OFFSET = 0xD,
        DREK_BOSS_ACTION_REPOSITION_AND_AIM = 0xE,
        DREK_BOSS_TARGET_GROUND_POINTS = 0xF,
        DREK_BOSS_TARGET_RANDOM_ARENA_POINTS = 0x10,
        DREK_BOSS_TARGET_SIDE_GROUND_POINTS = 0x11,
        DREK_BOSS_APPROACH_AIM_NODE = 0x12,
        DREK_BOSS_FOLLOW_HERO = 0x13,
        DREK_BOSS_ACTIVATE_MISSILE_GROUP = 0x14,
        DREK_BOSS_APPROACH_SELECTED_MOBY = 0x15,
        DREK_BOSS_DEATH_EXPLOSIONS = 0x1A,
        DREK_BOSS_RUN_ENDING_SEQUENCE = 0x1B
    };
    enum {
        DREK_BOSS_PHASE_FIRST_ARENA_PATH = 0,
        DREK_BOSS_PHASE_SECOND_ARENA_PATH = 1,
        DREK_BOSS_PHASE_THIRD_ARENA_PATH = 2,
        DREK_BOSS_PHASE_AFTER_THIRD_ARENA_PATH = 3,
        DREK_BOSS_PHASE_BEFORE_MISSILE_GROUP = 4,
        DREK_BOSS_PHASE_AFTER_MISSILE_GROUP_START = 5
    };
    BossData *boss_data = (BossData *)moby->f78;
    BossMoby *aim_target_moby;
    float     position_and_displacement_scratch[4];
    float     position_displacement_direction_and_rotation_scratch[4];
    float     position_direction_and_rotation_scratch[4];
    float     second_rotation_angles[4];
    float     node_position_and_yaw[4];
    float     slow;
    float     desired_yaw;
    float    *aim;
    float     fast;

    count_drek_boss_platform_event_once((char *)moby);
    update_drek_boss_damage_and_phase_transitions((char *)moby);
    aim_target_moby = boss_data->fB0;
    /* The file defines this one without parameters; retail passes the moby. */
    attach_drek_boss_big_head_manipulator((char *)moby);
    {
        char *manipulator_owner_moby = boss_data->f3A4;
        if (manipulator_owner_moby != 0 && manipulator_owner_moby[0x20] >= 0) {
            FUN_L00_00263ac8(*(float *)&drek_boss_big_head_manipulator_scale_gp,
                             (int)manipulator_owner_moby,
                             0,
                             boss_data->f1C0);
        }
    }
    if (moby->f31 != 0 && fast_vec_dist(moby->f10, D_L18_001677C0) < 32.0f) {
        FUN_L00_0025a120(moby);
        moby->f7F = 0x1A;
    }
    switch (moby->f20) {
    case DREK_BOSS_INITIALIZE: {
        int i;

        qcopy(boss_data->f3C0, moby->f10);
        moby->f20 = 1;
        moby->f30 = 0xFF;
        moby->f32 = 0x200;
        moby->f34 |= 0x41;
        moby->f94 = 0;
        boss_data->f28 = 3;
        boss_data->f20 = 500.0f;
        boss_data->f30 = 4.0f;
        boss_data->f24 = 500;
        boss_data->f340 = D_L18_001B0EB0[boss_data->f260[3]];
        boss_data->f398 = boss_data->f204[0];
        boss_data->f348 = 1;
        boss_data->f368 = -1;
        boss_data->f38C = 350.0f;
        boss_data->f334 = D_L18_0015FFD8 + boss_data->f220 * 0x100;
        boss_data->f3D0 = 0.75f;
        boss_data->f374 = 0.0f;
        for (i = 0; i < 3; i++) {
            hide_drek_boss_moby_group((int)moby, boss_data->f204[i]);
        }
        break;
    }
    case DREK_BOSS_WAIT_FOR_SCENE:
        if (D_L18_0015F5C4 != 2) {
            moby->f34 &= 0xFFBE;
            moby->f94 = moby->f24[4];
            moby->f20 = *(unsigned char *)&boss_data->f39C;
            if (moby->f20 == 5) {
                D_L18_0015F3FC = 1.0f;
            }
            if (moby->f20 == 0x19) {
                hero_respawn();
            }
            break;
        }
        if (((BossShared *)&D_L18_0016D260_c)->f30 == 2) {
            if (scale_game_frames(0xA28) <= ((BossShared *)&D_L18_0016D260_c)->f34 &&
                ((BossShared *)&D_L18_0016D260_c)->f34 <= scale_game_frames(0xB36)) {
                moby_init_action_mode(boss_data->f330);
            }
        } else if (((BossShared *)&D_L18_0016D260_c)->f30 == 3) {
            if (scale_game_frames(0x3B6) < ((BossShared *)&D_L18_0016D260_c)->f34 &&
                ((BossShared *)&D_L18_0016D260_c)->f34 < scale_game_frames(0x3C0)) {
                set_class_247_list_data_fields(boss_data->f200);
            }
            if (*(int *)&D_L18_001623A0 == 0) {
                char *b = D_L18_001600EC + boss_data->f294 * 0x80;
                jet_reset_camera(b + 0x30, b + 0x70);
                *(int *)&D_L18_001623A0 = 1;
            }
        }
        break;
    case DREK_BOSS_WAIT_FOR_ENCOUNTER_TRIGGER:
        if (is_point_inside_clip_volume(hero.motion.pos.f, boss_data->f274) != 0 &&
            hero.vehicle_mode == 2) {
            moby->f20 = 7;
            moby->f34 |= 0x41;
            start_scene(0);
            boss_data->f39C = 2;
            boss_data->f358 = scale_game_frames(600);
            boss_data->f344 = boss_data->f250[3];
            if (moby->f53 != 10) {
                blend_moby_animation(moby, 10, 0, scale_game_frames(10));
            }
        } else {
            int *encounter_trigger_path = D_L18_001B0EB0[boss_data->f270];
            if (point_in_path_xy(
                    hero.motion.pos.f, encounter_trigger_path + 4, *encounter_trigger_path) != 0) {
                moby->f34 &= 0xFFBE;
                moby->f94 = moby->f24[4];
                moby->f20 = 10;
                {
                    int *b = D_L18_001B0EB0[boss_data->f260[0]];
                    boss_data->f340 = b;
                    qcopy(boss_data->f3C0, b + 4);
                }
                moby->f40[2] = fast_arc_tan(hero.motion.pos.f[0] - boss_data->f3C0[0],
                                            hero.motion.pos.f[1] - boss_data->f3C0[1]);
                show_drek_boss_moby_group((int)moby, boss_data->f204[0]);
                boss_data->f38C = boss_data->f38C * 0.85714287f;
            } else if (fast_vec_dist(hero.motion.pos.f,
                                     D_L18_001600EC + (boss_data->f250[0] << 7) + 0x30) < 50.0f) {
                moby->f34 &= 0xFFBE;
                moby->f94 = moby->f24[4];
                moby->f20 = 0xC;
                {
                    int *p = D_L18_001B0EB0[boss_data->f260[0]];
                    boss_data->f340 = p;
                    qcopy(boss_data->f3C0, (char *)p + *p * 16);
                }
                boss_data->f344 = boss_data->f250[0];
                boss_data->f38C = boss_data->f38C * 0.85714287f;
            } else if (fast_vec_dist(hero.motion.pos.f,
                                     D_L18_001600EC + (boss_data->f250[1] << 7) + 0x30) < 50.0f) {
                moby->f34 &= 0xFFBE;
                moby->f94 = moby->f24[4];
                boss_data->f34C = 1;
                boss_data->f38C = boss_data->f38C * 0.71428573f;
                moby->f20 = 0xC;
                {
                    int *p = D_L18_001B0EB0[boss_data->f260[1]];
                    boss_data->f340 = p;
                    qcopy(boss_data->f3C0, (char *)p + *p * 16);
                }
                boss_data->f344 = boss_data->f250[1];
            } else if (fast_vec_dist(hero.motion.pos.f,
                                     D_L18_001600EC + (boss_data->f250[2] << 7) + 0x30) < 50.0f) {
                if (*(int *)&D_L18_001623A0 != 0) {
                    set_class_247_list_data_fields(boss_data->f200);
                    if (voice_is_playing(moby, boss_data->f3B0) != 0) {
                        int i = boss_data->f3B0;
                        if (i != -1) {
                            char *e = ((char *)&voice_pool) + i * 0x70;
                            if (*(BossMoby **)(e + 0x88) == moby &&
                                *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(i);
                            }
                        }
                        boss_data->f3B0 = -1;
                    }
                    moby->f20 = 0x1B;
                    moby->f94 = 0;
                    moby->f34 |= 0x41;
                    boss_data->f34C = 7;
                } else {
                    moby->f34 &= 0xFFBE;
                    moby->f94 = moby->f24[4];
                    boss_data->f34C = 2;
                    boss_data->f38C = boss_data->f38C * 0.5714286f;
                    moby->f20 = 0xC;
                    {
                        int *p = D_L18_001B0EB0[boss_data->f260[2]];
                        boss_data->f340 = p;
                        qcopy(boss_data->f3C0, (char *)p + *p * 16);
                    }
                    boss_data->f344 = boss_data->f250[2];
                    boss_data->f358 = scale_game_frames(0x168);
                }
            }
        }
        break;
    case DREK_BOSS_CIRCLE_AND_FIRE_AT_HERO:
        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        if (boss_data->f38C <= 301.0f) {
            if (hero.vehicle_mode != 0) {
                ((BossMoby *)hero.moby)->f94 = 0;
                ((BossMoby *)hero.moby)->f34 |= 0x41;
            }
            restore_saved_hero_moby_and_clear_vehicle_mode();
            {
                char *b = D_L18_001600EC + boss_data->f2A0 * 0x80;
                set_race_camera(b + 0x30, b + 0x70, 0, 1);
            }
            jet_clear_persistent(boss_data->f2B0);
            jet_reset_camera(hero.motion.pos.f, hero.motion.rot.f);
            show_drek_boss_moby_group((int)moby, boss_data->f204[0]);
            start_scene(1);
            moby->f20 = 7;
            moby->f34 |= 0x41;
            if (moby->f53 != 10) {
                blend_moby_animation(moby, 10, 0, scale_game_frames(0x14));
            }
            boss_data->f39C = 10;
            boss_data->f348 = 0;
            boss_data->f374 = 0.0f;
            {
                int *b = D_L18_001B0EB0[boss_data->f260[0]];
                boss_data->f340 = b;
                qcopy(boss_data->f3C0, b + 4);
            }
            moby->f40[2] = fast_arc_tan(hero.motion.pos.f[0] - boss_data->f3C0[0],
                                        hero.motion.pos.f[1] - boss_data->f3C0[1]);
            boss_data->f368 = -1;
            break;
        }
        if (moby->f53 == 10 && (is_value_within_interpolated_window((char *)moby, 1.0f) != 0 ||
                                is_value_within_interpolated_window((char *)moby, 16.0f) != 0)) {
            qcopy(position_and_displacement_scratch, hero.motion.near_ground_point.f);
            if (absolute_float(position_and_displacement_scratch[2] - boss_data->f3C0[2]) < 5.0f) {
                if ((float)moby->f50 < 15.0f) {
                    moby_point_position(
                        moby, 2, position_displacement_direction_and_rotation_scratch);
                } else {
                    moby_point_position(
                        moby, 3, position_displacement_direction_and_rotation_scratch);
                }
                fast_vec_normalize(
                    position_direction_and_rotation_scratch, moby->fC0, frame_time * 20.0f);
                {
                    float a = randf_sym(0.08726646f, 0.5235988f);
                    float c = -random_float_between(0.08726646f, 0.17453292f);
                    float e = vector_distance_xy(moby->f10, hero.motion.pos.f) * 3.0f;
                    if (e > 120.0f) {
                        e = 120.0f;
                    } else if (e < 60.0f) {
                        e = 60.0f;
                    }
                    create_rocket_moby(position_displacement_direction_and_rotation_scratch,
                                       position_direction_and_rotation_scratch,
                                       position_and_displacement_scratch,
                                       a,
                                       c,
                                       truncate_float_to_s32(scale_time(e)),
                                       0);
                }
            }
        }
        if ((moby->f70 & 2) != 0 && moby->f53 != 10) {
            blend_moby_animation(moby, 10, 0, scale_game_frames(0x14));
        }
        break;
    case DREK_BOSS_APPROACH_PATH_START: {
        float lvl;
        int   hit;

        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        qcopy(position_and_displacement_scratch, boss_data->f340 + 4);
        hit = advance_drek_boss_planar_position_toward_target(
            (char *)moby, 20.0f, position_and_displacement_scratch);
        lvl = get_position_and_rotation_transition_progress(boss_data->f334);
        if (moby->f53 == 7) {
            if ((moby->f70 & 2) != 0) {
                blend_moby_animation(moby, 8, 0, scale_game_frames(10));
            }
        } else {
            fast_vec_sub(position_displacement_direction_and_rotation_scratch,
                         boss_data->f340 + 4,
                         boss_data->f3C0);
            fast_vec_normalize(position_displacement_direction_and_rotation_scratch,
                               position_displacement_direction_and_rotation_scratch,
                               frame_time * 5.0f);
            fast_vec_add(boss_data->f3C0,
                         boss_data->f3C0,
                         position_displacement_direction_and_rotation_scratch);
        }
        if (lvl > 0.5f && lvl < 0.7f) {
            if (moby->f53 != 9) {
                blend_moby_animation(moby, 9, 0, 0);
            }
            if (boss_data->f33C == 0) {
                break;
            }
            moby_point_position(moby, 1, position_displacement_direction_and_rotation_scratch);
            qcopy(position_direction_and_rotation_scratch,
                  D_L18_001600EC + boss_data->f344 * 0x80 + 0x30);
            position_direction_and_rotation_scratch[2] =
                position_direction_and_rotation_scratch[2] + 10.0f;
            position_direction_and_rotation_scratch[2] =
                probe_ground_height(position_direction_and_rotation_scratch, 0, 0.5f);
            refresh_moby_position_and_activate(boss_data->f33C,
                                               position_displacement_direction_and_rotation_scratch,
                                               D_L18_001600EC + boss_data->f344 * 0x80 + 0x30,
                                               1,
                                               1.0f);
            boss_data->f33C = 0;
        } else if (lvl >= 0.729f) {
            int i;

            if (moby->f53 != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(0x14));
            }
            if (hit != 0) {
                moby->f20 = 10;
            }
            for (i = 0; i < 4; i++) {
                int k = boss_data->f34C == 1 ? boss_data->f230[i] : boss_data->f240[i];

                activate_drek_boss_class_0565_moby(D_L18_0015FFD8 + k * 0x100);
            }
        }
        if (boss_data->f33C != 0) {
            float f;

            moby_point_position(moby, 1, position_displacement_direction_and_rotation_scratch);
            f = lvl + lvl;
            if (f > 1.0f) {
                f = 1.0f;
            }
            refresh_moby_position_and_activate(boss_data->f33C,
                                               position_displacement_direction_and_rotation_scratch,
                                               D_L18_0015F580,
                                               0,
                                               f);
        }
        break;
    }
    case DREK_BOSS_FOLLOW_PATH_AND_FIRE_AT_MOBYS: {
        unsigned char *firing_target_moby;

        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        approach_value(&boss_data->f374, frame_time * 12.0f, D_0015ED6C_e[1] * 20.0f);
        if (boss_data->f374 != 0.0f) {
            fast_vec_sub(position_and_displacement_scratch,
                         (char *)boss_data->f340 + (boss_data->f348 * 0x10 + 0x10),
                         boss_data->f3C0);
            if (fast_vec_length(position_and_displacement_scratch) <= boss_data->f374) {
                boss_data->f348 = boss_data->f348 + 1;
            } else {
                fast_vec_normalize(position_and_displacement_scratch,
                                   position_and_displacement_scratch,
                                   boss_data->f374);
            }
            fast_vec_add(boss_data->f3C0, boss_data->f3C0, position_and_displacement_scratch);
        }
        firing_target_moby = select_drek_boss_platform_firing_target((char *)moby);
        if (firing_target_moby != 0 &&
            (is_value_within_interpolated_window((char *)moby, 1.0f) != 0 ||
             is_value_within_interpolated_window((char *)moby, 16.0f) != 0)) {
            qcopy(position_direction_and_rotation_scratch, firing_target_moby + 0x10);
            qcopy(position_and_displacement_scratch, boss_data->f3C0);
            fast_vec_add(
                position_and_displacement_scratch, position_and_displacement_scratch, moby->fC0);
            position_and_displacement_scratch[2] = position_and_displacement_scratch[2] + 6.0f;
            fast_vec_normalize(position_displacement_direction_and_rotation_scratch,
                               moby->fC0,
                               frame_time * 10.0f);
            {
                float a = randf_sym(0.5235988f, 0.7853982f);
                float c = -random_float_between(0.34906584f, 0.5235988f);
                create_rocket_moby(position_and_displacement_scratch,
                                   position_displacement_direction_and_rotation_scratch,
                                   position_direction_and_rotation_scratch,
                                   a,
                                   c,
                                   scale_game_frames(0x3C),
                                   5);
            }
        }
        if (boss_data->f348 == *boss_data->f340 - 2) {
            if (moby->f53 != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(0x14));
            }
            moby->f20 = 0xB;
        }
        break;
    }
    case DREK_BOSS_FINISH_PATH_AND_WAIT_FOR_HERO:
        if (boss_data->f348 != *boss_data->f340) {
            approach_value(&boss_data->f374, frame_time * 10.0f, D_0015ED6C_e[1] * 20.0f);
            fast_vec_sub(position_and_displacement_scratch,
                         (char *)boss_data->f340 + (boss_data->f348 * 0x10 + 0x10),
                         boss_data->f3C0);
            if (fast_vec_length(position_and_displacement_scratch) <= boss_data->f374) {
                boss_data->f348 = boss_data->f348 + 1;
            } else {
                fast_vec_normalize(position_and_displacement_scratch,
                                   position_and_displacement_scratch,
                                   boss_data->f374);
            }
            fast_vec_add(boss_data->f3C0, boss_data->f3C0, position_and_displacement_scratch);
            break;
        }
        if (vector_distance_xy(moby->f10, hero.motion.pos.f) < 50.0f && hero.air_frames.s == 0 &&
            (((BossMoby *)hero.ground_moby) == 0 || ((BossMoby *)hero.ground_moby)->fA6 != 0x24B)) {
            int *p = boss_data->f340;

            boss_data->f374 = 0.0f;
            qcopy(boss_data->f3C0, (char *)p + *p * 16);
            boss_data->f344 = boss_data->f250[boss_data->f34C];
            if (boss_data->f34C == 2) {
                if (*(int *)&l18_drek_boss_scene_2_started_gp == 0) {
                    moby->f20 = 4;
                } else {
                    moby->f20 = 0x15;
                    boss_data->f330 = find_class_247_moby_with_largest_xy_angle_in_list(
                        boss_data->f3C0, hero.motion.pos.f, boss_data->f200);
                }
                break;
            }
            moby->f20 = 0xC;
        }
        break;
    case DREK_BOSS_WAIT_FOR_GROUNDED_HERO_TO_START_SCENE:
        if (hero.air_frames.s == 0 &&
            (((BossMoby *)hero.ground_moby) == 0 || ((BossMoby *)hero.ground_moby)->fA6 != 0x24B)) {
            int i;

            *(int *)&l18_drek_boss_scene_2_started_gp = 1;
            moby->f20 = 7;
            boss_data->f39C = 5;
            boss_data->f330 =
                (BossMoby *)find_nearest_class_247_moby_in_list(hero.motion.pos.f, boss_data->f200);
            moby->f34 |= 0x41;
            start_scene(2);
            for (i = 0; i < 3; i++) {
                hide_drek_boss_moby_group((int)moby, boss_data->f204[i]);
            }
        }
        break;
    case DREK_BOSS_INITIALIZE_PAIRED_TRANSFORM_TRANSITION: {
        unsigned char *b = D_L18_0015FFD8 + boss_data->f21C * 0x100;

        b += 0x10;
        *(int *)&D_L18_00162390 = 0;
        position_and_displacement_scratch[0] = fast_cos(0.0f) * 6.0f;
        position_and_displacement_scratch[1] = fast_sin(0.0f) * 6.0f;
        position_and_displacement_scratch[2] = 0.0f;
        position_displacement_direction_and_rotation_scratch[0] = fast_cos(0.7853982f) * 6.0f;
        position_displacement_direction_and_rotation_scratch[1] = fast_sin(0.7853982f) * 6.0f;
        position_displacement_direction_and_rotation_scratch[2] = 0.0f;
        fast_vec_add(position_and_displacement_scratch, position_and_displacement_scratch, b);
        fast_vec_add(position_displacement_direction_and_rotation_scratch,
                     position_displacement_direction_and_rotation_scratch,
                     b);
        position_and_displacement_scratch[2] = position_and_displacement_scratch[2] + 5.0f;
        position_displacement_direction_and_rotation_scratch[2] =
            position_displacement_direction_and_rotation_scratch[2] + 7.0f;
        clear_u64_value(position_direction_and_rotation_scratch);
        position_direction_and_rotation_scratch[2] = 3.1415927f;
        clear_u64_value(second_rotation_angles);
        second_rotation_angles[2] = 3.9269907f;
        position_direction_and_rotation_scratch[1] = 0.17453292f;
        second_rotation_angles[1] = 0.5235988f;
        {
            /* In locals: retail does not load them again after the copy. */
            char *recs = D_L18_001600EC;
            int   idx = boss_data->f298;

            qcopy(node_position_and_yaw, recs + idx * 0x80 + 0x30);
            node_position_and_yaw[3] = *(float *)(recs + idx * 0x80 + 0x78);
        }
        start_position_and_rotation_transition(
            (char *)boss_data->f334,
            position_and_displacement_scratch,
            position_displacement_direction_and_rotation_scratch,
            position_direction_and_rotation_scratch,
            second_rotation_angles,
            node_position_and_yaw,
            *(int *)&l18_drek_boss_paired_transition_duration_frames_gp,
            *(int *)&D_L18_00162398,
            1);
        hero.pending_item[3] = 3;
        moby->f20 = 8;
        request_thruster_pack_lock_red_wait(D_L18_0015FFD8 + boss_data->f21C * 0x100,
                                            scale_game_frames(0x708));
        break;
    }
    case DREK_BOSS_WAIT_TO_RESTORE_SAVED_TRANSFORM:
        if (classify_moby_state_two_or_three(boss_data->f334) == 2) {
            qcopy(boss_data->f3C0, boss_data->f300);
            qcopy(moby->f40, boss_data->f310);
            moby->f20 = 9;
            if (moby->f53 != 7) {
                blend_moby_animation(moby, 7, 0, scale_game_frames(0x14));
            }
            show_drek_boss_moby_group((int)moby, boss_data->f398);
            hide_drek_boss_moby_group((int)moby, boss_data->f204[boss_data->f34C - 1]);
            moby_point_position(moby, 1, position_and_displacement_scratch);
            boss_data->f33C = (unsigned char *)create_class_3d7_moby(
                moby, position_and_displacement_scratch, 300);
        }
        break;
    case DREK_BOSS_WAIT_FOR_PAIRED_TRANSFORM_TRANSITION:
        moby->f34 |= 0x41;
        if (get_position_and_rotation_transition_progress(boss_data->f334) >= 1.0f) {
            moby->f20 = 0xC;
            moby->f34 &= 0xFFBE;
            if (moby->f53 != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case DREK_BOSS_ACTIVATE_MISSILE_GROUP:
        if (fast_dec_timer(&boss_data->f354) != 0) {
            boss_data->f354 = scale_game_frames(0x14);
            qcopy(position_and_displacement_scratch, boss_data->f3C0);
            position_and_displacement_scratch[2] = position_and_displacement_scratch[2] + 4.0f;
            if (activate_drek_fight_missiles_in_group(
                    (char *)moby, boss_data->f214, position_and_displacement_scratch) != 0) {
                moby->f20 = 0xC;
            }
        }
        break;
    case DREK_BOSS_SELECT_NEXT_ACTION:
        aim = &moby->f40[2];
        desired_yaw = fast_arc_tan(aim_target_moby->f10[0] - moby->f10[0],
                                   aim_target_moby->f10[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        if ((moby->f70 & 2) == 0) {
            break;
        }
        switch (select_drek_boss_next_action((char *)moby)) {
        case DREK_BOSS_TARGET_GROUND_POINTS:
            moby->f20 = 0xF;
            if (moby->f53 != 10) {
                blend_moby_animation(moby, 10, 0, scale_game_frames(0x14));
            }
            boss_data->f354 = 0;
            boss_data->f358 = scale_game_frames(600);
            boss_data->f35C = scale_game_frames(0x168);
            break;
        case DREK_BOSS_LAUNCH_FORWARD_WITH_LATERAL_OFFSET:
            moby->f20 = 0xD;
            if (moby->f53 != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(0x14));
            }
            boss_data->f354 = 0;
            boss_data->f358 = scale_game_frames(random_integer_below(3) * 0x5A + 0x14A);
            boss_data->f35C = scale_game_frames((6 - boss_data->f34C) * 0x1E);
            boss_data->f3BC = random_float_between(-0.333f, 0.333f);
            break;
        case DREK_BOSS_TARGET_SIDE_GROUND_POINTS:
            moby->f20 = 0x11;
            if (moby->f53 != 11) {
                blend_moby_animation(moby, 0xB, 0, scale_game_frames(0x14));
            }
            boss_data->f354 = scale_game_frames(10);
            boss_data->f358 = scale_game_frames(600);
            break;
        default:
            moby->f20 = 0x10;
            if (moby->f53 != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(0x14));
            }
            boss_data->f354 = scale_game_frames(0xF);
            boss_data->f358 = scale_game_frames(0x168);
            break;
        case DREK_BOSS_ACTION_REPOSITION_AND_AIM:
            moby->f20 = 0x12;
            if (moby->f53 != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(0x14));
            }
            {
                float best = 0.0f;
                int   i;

                for (i = 0; i < 3; i++) {
                    float w;

                    qcopy(position_and_displacement_scratch,
                          D_L18_001600EC + boss_data->f280[i] * 0x80 + 0x30);
                    w = fast_difference_between_rotations(
                        fast_arc_tan(boss_data->f3C0[0] - hero.motion.pos.f[0],
                                     boss_data->f3C0[1] - hero.motion.pos.f[1]),
                        fast_arc_tan(boss_data->f3C0[0] - position_and_displacement_scratch[0],
                                     boss_data->f3C0[1] - position_and_displacement_scratch[1]));
                    if (best < w) {
                        boss_data->f3A8 = boss_data->f280[i];
                        best = w;
                    }
                }
            }
            boss_data->f358 = scale_game_frames(900);
            boss_data->f354 = scale_game_frames(0xB4);
            break;
        case DREK_BOSS_FOLLOW_HERO:
            moby->f20 = 0x13;
            if (moby->f53 != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
            }
            if (vector_distance_xy(moby->f10, hero.motion.pos.f) < 16.0f) {
                boss_data->f354 = scale_game_frames(0x5A);
            } else {
                boss_data->f354 = 0;
            }
            boss_data->f358 = scale_game_frames(600);
            boss_data->f35C = scale_game_frames(600);
            boss_data->f338 = create_class_274_ring_effect(moby, boss_data->f3C0, 5.8f);
            break;
        }
        break;
    case DREK_BOSS_ACTION_REPOSITION_AND_AIM:
        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(boss_data->f320[0] - moby->f10[0], boss_data->f320[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        moby_point_position(moby, 0x13, position_and_displacement_scratch);
        *(BossTI *)position_displacement_direction_and_rotation_scratch =
            *(BossTI *)boss_data->f320;
        set_drek_boss_relative_aim_yaw((char *)moby,
                                       boss_data->f140,
                                       position_and_displacement_scratch,
                                       position_displacement_direction_and_rotation_scratch);
        boss_data->f3D8 = -3.0f;
        if ((moby->f70 & 2) != 0 && moby->f53 != 8) {
            blend_moby_animation(moby, 8, 0, scale_game_frames(0x14));
        }
        if (boss_data->f354 != 0) {
            if (boss_data->f33C == 0) {
                moby_point_position(moby, 1, position_displacement_direction_and_rotation_scratch);
                boss_data->f33C = (unsigned char *)create_class_3d7_moby(
                    moby,
                    position_displacement_direction_and_rotation_scratch,
                    scale_game_frames(*(int *)&l18_drek_boss_aimed_class_3d7_spawn_frame_count_gp));
            } else if (fast_dec_timer(&boss_data->f354) != 0) {
                if (moby->f53 != 9) {
                    blend_moby_animation(moby, 9, 0, 0);
                }
                boss_data->f354 = scale_game_frames(0x78);
                moby_point_position(moby, 1, position_displacement_direction_and_rotation_scratch);
                fast_vec_sub(position_direction_and_rotation_scratch,
                             boss_data->f320,
                             position_displacement_direction_and_rotation_scratch);
                position_direction_and_rotation_scratch[2] = 2.0f;
                set_vector_length_xy(position_direction_and_rotation_scratch,
                                     position_direction_and_rotation_scratch,
                                     180.0f);
                fast_vec_add(position_direction_and_rotation_scratch,
                             position_direction_and_rotation_scratch,
                             position_displacement_direction_and_rotation_scratch);
                refresh_moby_position_and_activate(
                    boss_data->f33C,
                    position_displacement_direction_and_rotation_scratch,
                    position_direction_and_rotation_scratch,
                    1,
                    1.0f);
                boss_data->f33C = 0;
                if (boss_data->f358 == 0) {
                    boss_data->f350 = 0xE;
                    moby->f20 = 0xC;
                }
            } else {
                float f = 1.0f - convert_integer_to_float(boss_data->f354) / scale_time(90.0f);

                if (f > 1.0f) {
                    f = 1.0f;
                } else if (f < 0.0f) {
                    f = 0.0f;
                }
                moby_point_position(moby, 1, position_displacement_direction_and_rotation_scratch);
                refresh_moby_position_and_activate(
                    boss_data->f33C,
                    position_displacement_direction_and_rotation_scratch,
                    boss_data->f320,
                    0,
                    f);
                if (scale_game_frames(0xF) < boss_data->f354) {
                    qcopy(boss_data->f320, aim_target_moby->f10);
                }
            }
        }
        fast_dec_timer(&boss_data->f358);
        break;
    case DREK_BOSS_TARGET_GROUND_POINTS:
        aim = &moby->f40[2];
        desired_yaw = fast_arc_tan(aim_target_moby->f10[0] - moby->f10[0],
                                   aim_target_moby->f10[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        if (is_value_within_interpolated_window((char *)moby, 1.0f) != 0 ||
            is_value_within_interpolated_window((char *)moby, 16.0f) != 0) {
            if (random_integer_below(5) != 0 || boss_data->fB4 == 2) {
                float a;
                float r;

                if (boss_data->fB4 != 2) {
                    qcopy(position_displacement_direction_and_rotation_scratch, boss_data->f70);
                } else {
                    qcopy(position_displacement_direction_and_rotation_scratch, hero.motion.pos.f);
                }
                a = random_angle_radians();
                r = random_float_between(1.0f, 10.0f);
                position_and_displacement_scratch[0] = fast_cos(a) * r;
                position_and_displacement_scratch[1] = fast_sin(a) * r;
                position_and_displacement_scratch[2] = 0.0f;
                fast_vec_add(position_and_displacement_scratch,
                             position_and_displacement_scratch,
                             position_displacement_direction_and_rotation_scratch);
            } else {
                qcopy(position_and_displacement_scratch, boss_data->f70);
            }
            if (hero.state.control_mode == 0xF) {
                qcopy(position_displacement_direction_and_rotation_scratch,
                      D_L18_001600EC + boss_data->f344 * 0x80 + 0x30);
                fast_vec_sub(position_and_displacement_scratch,
                             position_and_displacement_scratch,
                             position_displacement_direction_and_rotation_scratch);
                if (vector_length_xy(position_and_displacement_scratch) > 29.0f) {
                    set_vector_length_xy(position_and_displacement_scratch,
                                         position_and_displacement_scratch,
                                         29.0f);
                }
                fast_vec_add(position_and_displacement_scratch,
                             position_and_displacement_scratch,
                             position_displacement_direction_and_rotation_scratch);
            }
            position_and_displacement_scratch[2] =
                probe_ground_height(position_and_displacement_scratch, 0, 0.5f);
            if (absolute_float(position_and_displacement_scratch[2] - boss_data->f3C0[2]) < 5.0f) {
                float a;
                float c;
                float e;

                if (is_value_within_interpolated_window((char *)moby, 1.0f) != 0) {
                    moby_point_position(
                        moby, 2, position_displacement_direction_and_rotation_scratch);
                } else {
                    moby_point_position(
                        moby, 3, position_displacement_direction_and_rotation_scratch);
                }
                fast_vec_normalize(
                    position_direction_and_rotation_scratch, moby->fC0, frame_time * 20.0f);
                a = randf_sym(0.08726646f, 0.5235988f);
                c = -random_float_between(0.08726646f, 0.17453292f);
                e = vector_distance_xy(moby->f10, hero.motion.pos.f) * 3.0f;
                if (e > 120.0f) {
                    e = 120.0f;
                } else if (e < 60.0f) {
                    e = 60.0f;
                }
                create_rocket_moby(position_displacement_direction_and_rotation_scratch,
                                   position_direction_and_rotation_scratch,
                                   position_and_displacement_scratch,
                                   a,
                                   c,
                                   truncate_float_to_s32(scale_time(e)),
                                   2);
                spawn_drek_boss_firing_effects(
                    (char *)moby, position_displacement_direction_and_rotation_scratch);
            }
        }
        if (fast_dec_timer(&boss_data->f358) != 0 ||
            (boss_data->f390 > 25.0f && (float)boss_data->f34C < 4.0f)) {
            boss_data->f350 = 0xF;
            moby->f20 = 0xC;
        }
        break;
    case DREK_BOSS_LAUNCH_FORWARD_WITH_LATERAL_OFFSET:
        aim = &moby->f40[2];
        desired_yaw = fast_arc_tan(aim_target_moby->f10[0] - moby->f10[0],
                                   aim_target_moby->f10[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        if (boss_data->f354 < boss_data->f358 + boss_data->f35C) {
            boss_data->f128 = fast_arc_tan(1.0f, boss_data->f3BC);
        }
        if (moby->f53 == 2) {
            if ((moby->f70 & 2) != 0) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
            }
        } else if (fast_dec_timer(&boss_data->f354) != 0 && boss_data->f358 != 0 &&
                   moby->f53 != 4) {
            blend_moby_animation(moby, 4, 0, scale_game_frames(0x14));
        }
        if (moby->f53 == 4) {
            if (is_value_within_interpolated_window((char *)moby, 1.0f) != 0) {
                float f;

                boss_data->f354 = scale_game_frames(0x5A);
                boss_data->f360 = scale_game_frames(300);
                moby_point_position(moby, 0, position_and_displacement_scratch);
                qcopy(position_displacement_direction_and_rotation_scratch, moby->fC0);
                fast_vec_normalize(
                    position_direction_and_rotation_scratch, moby->fD0, boss_data->f3BC);
                fast_vec_add(position_displacement_direction_and_rotation_scratch,
                             position_displacement_direction_and_rotation_scratch,
                             position_direction_and_rotation_scratch);
                f = vector_distance_xy(position_and_displacement_scratch, aim_target_moby->f10) *
                    0.5f;
                if (f > 30.0f) {
                    f = 30.0f;
                } else if (f < 0.0f) {
                    f = 0.0f;
                }
                fast_vec_normalize(position_displacement_direction_and_rotation_scratch,
                                   position_displacement_direction_and_rotation_scratch,
                                   f * frame_time);
                spawn_class_270_moby((char *)position_and_displacement_scratch,
                                     position_displacement_direction_and_rotation_scratch,
                                     (int)moby,
                                     64.0f,
                                     frame_time * 18.0f);
                spawn_drek_boss_firing_effects((char *)moby, position_and_displacement_scratch);
                boss_data->f3BC = randf_sym(0.133f, 0.333f);
            } else if ((moby->f70 & 2) != 0 && moby->f53 != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
            }
        }
        if (fast_dec_timer(&boss_data->f358) != 0 && fast_dec_timer(&boss_data->f35C) != 0) {
            boss_data->f350 = 0xD;
            moby->f20 = 0xC;
            if (moby->f53 != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(0x14));
            }
        }
        break;
    case DREK_BOSS_TARGET_RANDOM_ARENA_POINTS:
        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        if (fast_dec_timer(&boss_data->f354) != 0) {
            float a;
            float r;

            qcopy(position_and_displacement_scratch,
                  D_L18_001600EC + boss_data->f344 * 0x80 + 0x30);
            fast_vec_normalize(
                position_displacement_direction_and_rotation_scratch, moby->fC0, 3.0f);
            fast_vec_add(position_displacement_direction_and_rotation_scratch,
                         position_displacement_direction_and_rotation_scratch,
                         moby->f10);
            position_displacement_direction_and_rotation_scratch[2] =
                position_displacement_direction_and_rotation_scratch[2] + 3.0f;
            a = random_angle_radians();
            r = random_float_between(3.0f, 29.0f);
            position_direction_and_rotation_scratch[0] = fast_cos(a) * r;
            position_direction_and_rotation_scratch[1] = fast_sin(a) * r;
            position_direction_and_rotation_scratch[2] = 0.0f;
            fast_vec_add(position_direction_and_rotation_scratch,
                         position_direction_and_rotation_scratch,
                         position_and_displacement_scratch);
            position_direction_and_rotation_scratch[2] =
                position_direction_and_rotation_scratch[2] + 5.0f;
            position_direction_and_rotation_scratch[2] =
                probe_ground_height(position_direction_and_rotation_scratch, 0, 0.5f);
            if (absolute_float(position_direction_and_rotation_scratch[2] - boss_data->f3C0[2]) <
                5.0f) {
                float speed =
                    vector_distance_xy(position_direction_and_rotation_scratch,
                                       position_displacement_direction_and_rotation_scratch) /
                    (float)scale_game_frames(0x3C);
                float w = random_float_between(18.0f, 20.0f);

                launch_class_238_moby_from_list(
                    (int)moby,
                    boss_data->f29C,
                    position_displacement_direction_and_rotation_scratch,
                    position_direction_and_rotation_scratch,
                    speed,
                    truncate_float_to_s32(scale_time(w * 60.0f)));
            }
            boss_data->f354 = scale_game_frames(0xF);
        }
        if (fast_dec_timer(&boss_data->f358) != 0) {
            boss_data->f350 = 0x10;
            moby->f20 = 0xC;
        }
        break;
    case DREK_BOSS_TARGET_SIDE_GROUND_POINTS: {
        int n;

        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        if ((moby->f70 & 2) != 0 && moby->f53 != 12) {
            blend_moby_animation(moby, 0xC, 0, scale_game_frames(0x14));
        }
        if ((moby->f53 == 12 && fast_dec_timer(&boss_data->f354) != 0) ||
            boss_data->f354 == scale_game_frames(0x1E)) {
            float f = boss_data->f354 != 0 ? 4.3f : -4.3f;

            fast_vec_normalize(position_and_displacement_scratch, moby->fD0, f);
            fast_vec_add(position_and_displacement_scratch,
                         position_and_displacement_scratch,
                         boss_data->f3C0);
            position_and_displacement_scratch[2] =
                probe_ground_height(position_and_displacement_scratch, 0, 0.5f);
            if (absolute_float(position_and_displacement_scratch[2] - boss_data->f3C0[2]) < 1.0f) {
                /* v1 is not written in this state; retail passes it all the same. */
                float speed =
                    vector_distance_xy(position_and_displacement_scratch,
                                       position_displacement_direction_and_rotation_scratch) /
                    (float)scale_game_frames(0x3C);

                if (launch_first_idle_group_class_0772_moby(boss_data->f210,
                                                            position_and_displacement_scratch,
                                                            position_and_displacement_scratch,
                                                            speed) != 0 &&
                    boss_data->f354 == 0) {
                    boss_data->f354 = scale_game_frames(0x3C);
                }
            }
        }
        n = count_group_class_0772_mobys_not_in_state_8(boss_data->f210);
        if (n >= 11 && scale_game_frames(0x3C) > boss_data->f354) {
            boss_data->f354 = scale_game_frames(1000);
        } else if (n < 3 && scale_game_frames(0x3C) < boss_data->f354) {
            boss_data->f358 = 0;
        }
        if (fast_dec_timer(&boss_data->f358) != 0) {
            boss_data->f350 = 0x11;
            moby->f20 = 0xC;
            if (moby->f53 != 13) {
                blend_moby_animation(moby, 0xD, 0, scale_game_frames(0x14));
            }
        }
        break;
    }
    case DREK_BOSS_FOLLOW_HERO: {
        int fire;

        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        if (fast_dec_timer(&boss_data->f354) != 0) {
            update_drek_boss_hero_follow_movement((char *)moby);
        }
        refresh_ring_effect(boss_data->f338, boss_data->f3C0, 5.8f);
        if (voice_is_playing(moby, boss_data->f3B0) == 0) {
            boss_data->f3B0 = allocate_voice_for_target_entry(0xE, 4, (int)moby);
        }
        fire = 0;
        if (fast_dec_timer(&boss_data->f358) != 0 &&
            (hero.state.control_mode != 0xF || fast_dec_timer(&boss_data->f35C) != 0)) {
            fire = 1;
        }
        if (hero.state.control_mode == 0xF) {
            int *list = ((int *)hero.rail_spline);

            if (vector_distance_xy(hero.motion.pos.f, (char *)list + *list * 16) < 3.0f) {
                fire = 1;
            }
        }
        if (hero.state.current == HERO_STATE_HURT &&
            vector_distance_xy(moby->f10, hero.motion.pos.f) < 12.0f) {
            fire = 1;
        }
        if (vector_distance_xy(hero.motion.pos.f,
                               D_L18_001600EC + boss_data->f250[2] * 0x80 + 0x30) >
            *(float *)&D_L18_00162378_gp + 5.8f) {
            fire = 1;
        }
        if (fire) {
            int i = boss_data->f3B0;

            if (i != -1) {
                char *e = ((char *)&voice_pool) + i * 0x70;
                if (*(BossMoby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(i);
                }
            }
            boss_data->f3B0 = -1;
            moby->f20 = 0xC;
            if (moby->f53 != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(0x14));
            }
            boss_data->f350 = 0x13;
        }
        break;
    }
    case DREK_BOSS_APPROACH_AIM_NODE:
        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(boss_data->f70[0] - moby->f10[0], boss_data->f70[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        {
            /* Retail scales the index before it sets up the call. */
            int aim_node_byte_offset = boss_data->f3A8 * 0x80;

            if (advance_drek_boss_planar_position_toward_target(
                    (char *)moby,
                    frame_time * 12.0f,
                    D_L18_001600EC + aim_node_byte_offset + 0x30) != 0) {
                moby->f20 = 0xE;
                if (moby->f53 != 7) {
                    blend_moby_animation(moby, 7, 0, scale_game_frames(0x14));
                }
                qcopy(boss_data->f320, aim_target_moby->f10);
            }
        }
        break;
    case DREK_BOSS_APPROACH_SELECTED_MOBY:
        aim = &moby->f40[2];
        desired_yaw = fast_arc_tan(boss_data->f330->f10[0] - moby->f10[0],
                                   boss_data->f330->f10[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        moby_init_action_mode(boss_data->f330);
        if (advance_drek_boss_planar_position_toward_target(
                (char *)moby, frame_time * 12.0f, boss_data->f330->f10) != 0) {
            int ticks;

            switch (boss_data->f34C) {
            case DREK_BOSS_PHASE_FIRST_ARENA_PATH:
                ticks = scale_game_frames(0xE10);
                break;
            case DREK_BOSS_PHASE_SECOND_ARENA_PATH:
                ticks = scale_game_frames(0xA8C);
                break;
            case DREK_BOSS_PHASE_THIRD_ARENA_PATH:
                ticks = scale_game_frames(0x708);
                break;
            case DREK_BOSS_PHASE_AFTER_THIRD_ARENA_PATH:
                ticks = scale_game_frames(0x4B0);
                break;
            case DREK_BOSS_PHASE_BEFORE_MISSILE_GROUP:
                ticks = scale_game_frames(900);
                break;
            case DREK_BOSS_PHASE_AFTER_MISSILE_GROUP_START:
            default:
                ticks = scale_game_frames(600);
                break;
            }
            request_thruster_pack_lock_red_wait(D_L18_0015FFD8 + boss_data->f21C * 0x100,
                                                scale_game_frames(ticks));
            boss_data->f354 = 0;
            if (boss_data->f34C == 4) {
                moby->f20 = 0x14;
            } else {
                moby->f20 = 0xC;
            }
            boss_data->f364 = 1;
            boss_data->f34C = boss_data->f34C + 1;
        }
        if (*(int *)&D_L18_00162390 != 0) {
            moby->f20 = 5;
        }
        break;
    case DREK_BOSS_DEATH_EXPLOSIONS: {
        int n;

        aim = &moby->f40[2];
        desired_yaw =
            fast_arc_tan(hero.motion.pos.f[0] - moby->f10[0], hero.motion.pos.f[1] - moby->f10[1]);
        approach_angle(aim,
                       &boss_data->f370,
                       desired_yaw,
                       D_0015ED6C_e[1] * 6.2831855f,
                       D_0015ED6C_e[1] * 6.2831855f,
                       frame_time * 6.2831855f);
        update_drek_boss_arena_circling_movement((struct Moby *)moby);
        boss_data->f358 = boss_data->f358 + 1;
        n = scale_game_frames(0x14);
        if (boss_data->f358 % n == 0) {
            float a =
                fast_add_rotations(moby->f40[2], random_float_between(-30.0f, 30.0f) * DEG_TO_RAD);

            position_and_displacement_scratch[0] = fast_cos(a) * 6.0f;
            position_and_displacement_scratch[1] = fast_sin(a) * 6.0f;
            position_and_displacement_scratch[2] = 0.0f;
            position_and_displacement_scratch[2] = random_float_between(3.0f, 8.0f);
            fast_vec_add(position_and_displacement_scratch,
                         position_and_displacement_scratch,
                         boss_data->f3C0);
            func_L00_0025F4A8_alt(moby,
                                  D_L18_0015F580,
                                  position_and_displacement_scratch,
                                  0.0f,
                                  0.0f,
                                  0x14,
                                  6,
                                  0x20,
                                  1.0f,
                                  2.5f,
                                  9.0f,
                                  1.0f,
                                  -1,
                                  0.0f,
                                  0,
                                  0,
                                  -1,
                                  0);
        }
        if (boss_data->f358 == scale_game_frames(0x78)) {
            if (moby->f53 != 14) {
                blend_moby_animation(moby, 0xE, 5, 1);
            }
            position_and_displacement_scratch[0] = fast_cos(moby->f40[2]) * 3.0f;
            position_and_displacement_scratch[1] = fast_sin(moby->f40[2]) * 3.0f;
            position_and_displacement_scratch[2] = 6.0f;
            fast_vec_add(position_and_displacement_scratch,
                         position_and_displacement_scratch,
                         boss_data->f3C0);
            func_L00_0025F4A8_alt(moby,
                                  D_L18_0015F580,
                                  position_and_displacement_scratch,
                                  0.0f,
                                  0.0f,
                                  0x28,
                                  0xA,
                                  0x20,
                                  2.5f,
                                  5.0f,
                                  9.0f,
                                  2.0f,
                                  -1,
                                  10.0f,
                                  0,
                                  0,
                                  -1,
                                  0);
        }
        if (moby->f52 == moby->f53 && moby->f52 == 14 &&
            is_value_within_interpolated_window((char *)moby, 30.0f) != 0) {
            moby->f20 = 0x1B;
            end_thruster_pack_lock_red_wait(D_L18_0015FFD8 + boss_data->f21C * 0x100);
        }
        break;
    }
    case DREK_BOSS_RUN_ENDING_SEQUENCE:
        moby->f94 = 0;
        moby->f34 |= 0x41;
        moby->f34 &= 0xEFFF;
        if (boss_data->f34C < 7) {
            start_scene(3);
            boss_data->f34C = 7;
            ((BossShared *)&D_L18_0016D260_c)->f46 = 1;
            {
                /* In locals: retail does not load them again after the first copy. */
                char *recs = D_L18_001600EC;
                int   idx = boss_data->f294;

                qcopy(((BossShared *)&D_L18_0016D260_c)->f10, recs + idx * 0x80 + 0x30);
                qcopy(((BossShared *)&D_L18_0016D260_c)->f20, recs + idx * 0x80 + 0x70);
            }
            moby->f20 = 7;
            boss_data->f39C = 0x1B;
        } else if (boss_data->f34C == 7) {
            if (((BossMoby *)hero.ground_moby) != 0 && hero.air_frames.s == 0 &&
                ((BossMoby *)hero.ground_moby)->fA6 == 0x247 && hero.state.current == 0x22) {
                if (boss_data->f3AC == 0) {
                    boss_data->f3AC = 1;
                }
            } else if (boss_data->f3AC == 0) {
                set_class_247_list_data_fields(boss_data->f200);
            }
            if (boss_data->f3AC != 0) {
                int n = boss_data->f3AC++;

                if (scale_game_frames(0x28) < n) {
                    start_scene(4);
                    boss_data->f34C = 8;
                    moby->f20 = 7;
                    boss_data->f39C = 0x1B;
                }
            }
        } else if (boss_data->f34C == 8) {
            *(OvlQuad *)position_and_displacement_scratch = 0;
            *(OvlQuad *)position_displacement_direction_and_rotation_scratch = 0;
            position_and_displacement_scratch[0] = 660.6f;
            position_and_displacement_scratch[1] = 481.4f;
            position_and_displacement_scratch[2] = 112.6f;
            position_displacement_direction_and_rotation_scratch[1] = -0.12f;
            position_displacement_direction_and_rotation_scratch[2] = -2.76f;
            jet_mount(position_and_displacement_scratch,
                      position_displacement_direction_and_rotation_scratch,
                      1,
                      0,
                      0);
            start_scene(5);
            boss_data->f34C = 9;
        } else if (boss_data->f34C == 9 && D_L18_0015F5C4 == 0) {
            start_slideshow();
            boss_data->f34C = 10;
        } else {
            int k = boss_data->f34C;

            if (k == 10 && D_L18_0015F5C4 == 0) {
                queue_movie_with_game_language(0xB);
                boss_data->f34C = 0xB;
            } else if (k == 0xB && D_L18_0015F5C4 == 0) {
                *(int *)&l18_drek_boss_scene_2_started_gp = 0;
                *(int *)&D_L18_001623A0 = 0;
                pause_all_sounds(0x21);
                mark_moby_for_removal(boss_data->f3A0);
                if (boss_data->f3A4 != 0) {
                    mark_moby_for_removal(boss_data->f3A4);
                }
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    }
    update_drek_boss_state_dependent_scalars((char *)moby);
    if (moby->f31 != 0) {
        moby_point_position(moby, 8, position_and_displacement_scratch);
        moby_point_position(moby, 9, position_displacement_direction_and_rotation_scratch);
        fast_vec_sub(position_direction_and_rotation_scratch,
                     position_and_displacement_scratch,
                     position_displacement_direction_and_rotation_scratch);
        fast_vec_normalize(
            position_direction_and_rotation_scratch, position_direction_and_rotation_scratch, 0.1f);
        draw_trail(position_and_displacement_scratch,
                   position_and_displacement_scratch,
                   position_direction_and_rotation_scratch,
                   600000.0f,
                   0.0f);
    }
    if (boss_data->f34C > 1) {
        hero.grind_look_moby = (struct Moby *)moby;
    }
    qcopy(moby->f10, boss_data->f3C0);
    slow = 0.3f;
    fast = 0.03f;
    boss_data->f3D4 = fast_add_rotations(boss_data->f3D4, frame_time * 3.1415927f);
    aas_c(boss_data->f3D0 * fast_sin(boss_data->f3D4) + boss_data->f3D8,
          D_0015ED6C_e[1] * 4.0f,
          D_0015ED6C_e[1] * 4.0f,
          frame_time * 4.0f,
          &boss_data->f3DC,
          &boss_data->f3E0);
    moby->f10[2] = moby->f10[2] + boss_data->f3DC;
    boss_data->f3D8 = 0.0f;
    update_joint_animation(
        (char *)moby, boss_data->fC0, 7, frame_scale_sq * fast, frame_scale_sq * slow);
    update_joint_animation(
        (char *)moby, boss_data->f140, 0x13, frame_scale_sq * fast, frame_scale_sq * slow);
    boss_data->f3B4 = fast_add_rotations(boss_data->f3B4, frame_time * 3.4906585f);
    {
        int c = fast_tween_color(0x30C8C8C8, 0x1C393939, fast_sin(boss_data->f3B4) * 0.5f + 0.5f);
        int r = c & 0xFF;
        int g = (c >> 8) & 0xFF;
        int b = (c >> 16) & 0xFF;
        int a = (unsigned int)c >> 24;

        moby->f90 = c;
        boss_data->f3B8 = (a << 24) | ((b / 3) << 16) | (g << 8) | (r / 3);
    }
    if (moby->f31 != 0) {
        add_draw_callback((void (*)(void))render_drek_boss_joint_glows, moby);
    }
}
/* Latches the target from the global block once, and counts a class 0x24B hit. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F7278), where it is exact; names translated to the US level program. */

extern s32 D_L18_001623A8_gp __asm__("D_L18_001623A8") __attribute__((sda));

void count_drek_boss_platform_event_once(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    if (*(int *)(moby_vars + 0x3E4) == 0) {
        char *latch_source_data = D_0013E533 + 0xE1D;
        if (*(int *)(latch_source_data + 0x300) != 0) {
            *(int *)(moby_vars + 0x3E8) = *(int *)(latch_source_data + 0x2FC);
        }
        if (*(int *)(latch_source_data + 0x2084) == 0x77) {
            char *latched_moby = *(char **)(moby_vars + 0x3E8);
            *(int *)(moby_vars + 0x3E4) = 1;
            if (latched_moby != 0) {
                if (MOBY(latched_moby)->oclass == 0x24B) {
                    *(int *)&D_L18_001623A8_gp = *(int *)&D_L18_001623A8_gp + 1;
                }
            }
        }
    }
}

extern int            D_L18_00162384_gp __asm__("D_L18_00162384") __attribute__((sda));
extern unsigned char *D_L18_0015FFD8_5e80 __asm__("D_L18_0015FFD8");

typedef struct {
    char  pad0[0x204];
    int   f204[3];
    char  pad210[0x18];
    int   f228[2];
    int   f230[4];
    char  pad240[0x10];
    int   f250[4];
    int   f260[4];
    char  pad270[0x90];
    float f300[4];
} VendorStage_5e80;

typedef struct {
    char  pad0[0x30];
    float pos[4];
    char  pad40[0x40];
} VendorNode_5e80;

extern void set_moby_attached_effect_alpha_from_distance(void *, float) __asm__("FUN_L18_00265ff8");
extern int
fun_00213928(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_00213928");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern int   FUN_L00_0025ff38_5e80(float a, char *m, char *b, int c, int d, int *e, int f) __asm__("FUN_L00_0025ff38");
extern int   poll_moby_state_and_rearm(char *) __asm__("FUN_L18_002d8050");
extern char *create_class_274_ring_effect(void *, void *, float) __asm__("FUN_L18_002dc3e0");
extern void  FUN_L18_002f1cc8_5e80(void *) __asm__("FUN_L18_002f1cc8");
extern void  start_position_and_rotation_transition(
     char *, void *, void *, void *, void *, void *, int, int, int) __asm__("FUN_L18_002dfaa0");
extern void  release_voice_slot(int) __asm__("FUN_0022d798");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");

extern short l18_drek_boss_class_scale_multiplier_gp __asm__("D_L18_00162330") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_drek_boss_damage_and_phase_transitions.h"

/* Ported from rac1-decomp src/overlays/l18_veldin2/vendor_002F2AE0.c (func_L18_002F72E0) */
void update_drek_boss_damage_and_phase_transitions(char *moby) {
    char          *boss_data = ((char *)MOBY(moby)->pvars);
    int            hit_processing_status;
    float          step;
    char          *hit_record;
    int            crossed = 0;
    float          health_floor_or_transition_offset;
    unsigned char *spawned_child_moby;
    float          V10[16];
    float          V50[4];
    float          V60[4];
    float          V70[4];
    float          V80[4];
    float          V90[4];
    float          VA0[4];

    MOBY(moby)->scale = *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
                        *(float *)&l18_drek_boss_class_scale_multiplier_gp;
    set_moby_attached_effect_alpha_from_distance(moby, 0.5f);
    health_floor_or_transition_offset =
        (7.0f - (float)*(int *)(boss_data + 0x34C) - 2.0f) / 7.0f * 350.0f;
    step = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0) {
        char *o = *(char **)(hit_record + 0x20);
        if (o != 0) {
            short h = *(short *)(o + 0xA6);
            if (h == 0x270 || h == 0x234 || (h == 0x238 && *(unsigned char *)(o + 0xBC) == 0)) {
                hit_record = 0;
            }
        }
    }
    fun_00213928(moby, hit_record, boss_data + 0x20, 0, &hit_processing_status, &step, 0, 4);
    if (hit_processing_status != 1) {
        int type = MOBY(moby)->state;
        if ((type >= 0xC && type <= 0x13) || type == 2) {
            char *o = *(char **)(hit_record + 0x20);
            if (o != 0) {
                short h = *(short *)(o + 0xA6);
                if (h != 0xB1) {
                    if (h == 0x131) {
                        step = step * 0.666f;
                    }
                } else {
                    step = step * 3.0f;
                }
            }
            if (hero.state.control_mode == 0xF) {
                *(float *)(boss_data + 0x390) = *(float *)(boss_data + 0x390) + step;
            } else {
                *(float *)(boss_data + 0x394) = *(float *)(boss_data + 0x394) + step;
            }
            if (step != 0.0f) {
                float unclamped_health;
                if (MOBY(moby)->state == 2) {
                    if (hit_record != 0 && (*(int *)(hit_record + 0x24) & 0x80000) &&
                        MOBY(moby)->prev_seq != 0xE) {
                        blend_moby_animation(moby, 0xE, 5, 1);
                    }
                }
                if (health_floor_or_transition_offset <= *(float *)(boss_data + 0x38C) &&
                    *(float *)(boss_data + 0x38C) - step < health_floor_or_transition_offset) {
                    crossed = 1;
                }
                unclamped_health = *(float *)(boss_data + 0x38C) - step;
                *(float *)(boss_data + 0x38C) = unclamped_health;
                if (unclamped_health < health_floor_or_transition_offset) {
                    *(float *)(boss_data + 0x38C) = health_floor_or_transition_offset;
                }
                *(unsigned char *)(boss_data + 0x67) = 0xF0;
                trooper_anim_update(moby, (short *)(boss_data + 0x60));
                if (*(float *)(boss_data + 0x38C) == 0.0f) {
                    if (*(int *)(boss_data + 0x368) != -1) {
                        set_animation_parameter(*(int *)(boss_data + 0x368), 0);
                        *(int *)(boss_data + 0x368) = -1;
                    }
                    moby[0x20] = 0x1A;
                    if (MOBY(moby)->prev_seq != 0) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(0x14));
                    }
                    if (voice_is_playing(moby, *(int *)(boss_data + 0x3B0)) != 0) {
                        int i = *(int *)(boss_data + 0x3B0);
                        if (i != -1) {
                            char *e = ((char *)&voice_pool) + i * 0x70;
                            if (*(char **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(i);
                            }
                        }
                        *(int *)(boss_data + 0x3B0) = -1;
                    }
                    *(int *)(boss_data + 0x358) = 0;
                    crossed = 0;
                }
            }
        }
    }
    MOBY(moby)->hit_slot = 0xFF;
    *(int *)(boss_data + 0x36C) = truncate_float_to_s32(*(float *)(boss_data + 0x38C));
    trooper_anim_finish(moby, boss_data + 0x60);
    if (*(int *)(boss_data + 0x3A0) == 0) {
        spawned_child_moby = (unsigned char *)create_moby(0x58F);
        *(unsigned char **)(boss_data + 0x3A0) = spawned_child_moby;
        spawned_child_moby[0x30] = 0xFF;
        (*(char **)(boss_data + 0x3A0))[0x31] = 1;
        blend_moby_animation(*(char **)(boss_data + 0x3A0), 1, 0, 0);
    }
    *(short *)(*(char **)(boss_data + 0x3A0) + 0x32) = MOBY(moby)->draw_dist;
    *(unsigned short *)(*(char **)(boss_data + 0x3A0) + 0x34) = MOBY(moby)->flags | 0x100;
    build_moby_bone_transform(moby, 4, V10);
    copy_matrix3x4(*(char **)(boss_data + 0x3A0) + 0xC0, V10);
    moby_point_position(moby, 4, *(char **)(boss_data + 0x3A0) + 0x10);
    update_entity(*(char **)(boss_data + 0x3A0));
    if (*(int *)(boss_data + 0x3A4) == 0) {
        spawned_child_moby = (unsigned char *)create_moby(0x4E1);
        *(unsigned char **)(boss_data + 0x3A4) = spawned_child_moby;
        spawned_child_moby[0x30] = 0xFF;
        (*(char **)(boss_data + 0x3A4))[0x31] = 1;
        blend_moby_animation(*(char **)(boss_data + 0x3A4), 1, 0, 0);
    }
    *(short *)(*(char **)(boss_data + 0x3A4) + 0x32) = MOBY(moby)->draw_dist;
    *(unsigned short *)(*(char **)(boss_data + 0x3A4) + 0x34) = MOBY(moby)->flags | 0x100;
    build_moby_bone_transform(*(char **)(boss_data + 0x3A0), 0, V10);
    copy_matrix3x4(*(char **)(boss_data + 0x3A4) + 0xC0, V10);
    moby_point_position(*(char **)(boss_data + 0x3A0), 0, *(char **)(boss_data + 0x3A4) + 0x10);
    update_entity(*(char **)(boss_data + 0x3A4));
    {
        int *e = D_L18_001B0EB0[*(int *)(boss_data + 0x218)];
        FUN_L00_0025ff38_5e80(512.0f, moby, boss_data + 0x70, 0, 0, e + 4, e[0]);
    }
    if (*(int *)(boss_data + 0xB0) == 0) {
        *(struct Moby **)(boss_data + 0xB0) = hero.moby;
    }
    {
        int type = MOBY(moby)->state;
        if (type >= 0xC && type <= 0x13) {
            int linked_moby_state_transition_status = poll_moby_state_and_rearm(
                (char *)*(int *)&D_L18_0015FFD8_5e80 + (*(int *)(boss_data + 0x21C) << 8));
            if (linked_moby_state_transition_status == 1) {
                moby[0x20] = 0x13;
                if (MOBY(moby)->prev_seq != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(0x14));
                }
                *(int *)(boss_data + 0x358) = scale_game_frames(0x258);
                *(int *)(boss_data + 0x354) = scale_game_frames(0x14);
                *(int *)(boss_data + 0x364) = 0;
                *(char **)(boss_data + 0x338) =
                    create_class_274_ring_effect(moby, boss_data + 0x3C0, 5.8f);
            } else if (linked_moby_state_transition_status == -1) {
                MOBY(moby)->flags |= 0x41;
                moby[0x20] = 7;
                start_scene(7);
                *(int *)(boss_data + 0x39C) = 0x19;
            }
        }
    }
    if (crossed != 0 || *(int *)&drek_boss_arena_phase_reset_requested_gp != 0) {
        if (*(int *)(D_001413F5 + 0x203) != 0) {
            if (*(int *)&drek_boss_arena_phase_reset_requested_gp != 0) {
                int i;
                *(int *)(boss_data + 0x34C) = 0;
                for (i = 0; i < 4; i++) {
                    FUN_L18_002f1cc8_5e80(D_L18_0015FFD8_5e80 +
                                          (*(int *)(boss_data + 0x230 + i * 4) << 8));
                }
                *(int *)&drek_boss_arena_phase_reset_requested_gp = 0;
            }
            if (*(int *)(boss_data + 0x34C) < 2) {
                int   previous_phase_index;
                int   k;
                int  *next_phase_path;
                float previous_arena_node_yaw;
                moby[0x20] = 3;
                previous_phase_index = *(int *)(boss_data + 0x34C);
                k = previous_phase_index + 1;
                *(int *)(boss_data + 0x34C) = k;
                next_phase_path = D_L18_001B0EB0[((VendorStage_5e80 *)boss_data)->f260[k]];
                *(int **)(boss_data + 0x340) = next_phase_path;
                *(int *)(boss_data + 0x398) = ((VendorStage_5e80 *)boss_data)->f204[k];
                *(int *)(boss_data + 0x348) = 1;
                *(int *)(boss_data + 0x374) = 0;
                qcopy(boss_data + 0x300, (char *)next_phase_path + 0x10);
                fast_vec_sub(
                    boss_data + 0x300,
                    boss_data + 0x300,
                    D_L18_001600EC +
                        (((VendorStage_5e80 *)boss_data)->f250[previous_phase_index] << 7) + 0x30);
                set_vector_length_xy(boss_data + 0x300, boss_data + 0x300, 25.0f);
                ((VendorStage_5e80 *)boss_data)->f300[2] = 0;
                fast_vec_add(
                    boss_data + 0x300,
                    boss_data + 0x300,
                    D_L18_001600EC +
                        (((VendorStage_5e80 *)boss_data)->f250[*(int *)(boss_data + 0x34C) - 1]
                         << 7) +
                        0x30);
                clear_u64_value(boss_data + 0x310);
                previous_arena_node_yaw = fast_arc_tan(
                    ((VendorNode_5e80 *)D_L18_001600EC)[((VendorStage_5e80 *)boss_data)
                                                            ->f250[*(int *)(boss_data + 0x34C) - 1]]
                            .pos[0] -
                        *(float *)(boss_data + 0x300),
                    ((VendorNode_5e80 *)D_L18_001600EC)[((VendorStage_5e80 *)boss_data)
                                                            ->f250[*(int *)(boss_data + 0x34C) - 1]]
                            .pos[1] -
                        *(float *)(boss_data + 0x304));
                *(float *)(boss_data + 0x318) = previous_arena_node_yaw;
                qcopy(V50, boss_data + 0x300);
                health_floor_or_transition_offset = 20.0f;
                V50[0] += fast_cos(previous_arena_node_yaw) * health_floor_or_transition_offset;
                V50[1] +=
                    fast_sin(*(float *)(boss_data + 0x318)) * health_floor_or_transition_offset;
                V50[2] += 3.0f;
                qcopy(
                    V90,
                    ((VendorNode_5e80 *)D_L18_001600EC)[((VendorStage_5e80 *)boss_data)
                                                            ->f228[*(int *)(boss_data + 0x34C) - 1]]
                        .pos);
                V90[2] += health_floor_or_transition_offset;
                V90[2] = probe_ground_height(V90, 0, 0.5f);
                qcopy(V60, V90);
                fast_vec_sub(
                    VA0,
                    D_L18_001600EC +
                        (((VendorStage_5e80 *)boss_data)->f250[*(int *)(boss_data + 0x34C) - 1]
                         << 7) +
                        0x30,
                    V60);
                VA0[2] = 0;
                fast_vec_normalize(VA0, VA0, -15.0f);
                fast_vec_add(V60, V60, VA0);
                V60[2] += 15.0f;
                V90[3] = fast_arc_tan(V90[0] - V60[0], V90[1] - V60[1]);
                clear_u64_value(V70);
                V70[2] = fast_arc_tan(*(float *)(boss_data + 0x300) - V50[0],
                                      *(float *)(boss_data + 0x304) - V50[1]);
                V70[1] = -0.17453292f;
                clear_u64_value(V80);
                {
                    float *current_path = *(float **)(boss_data + 0x340);
                    V80[2] = fast_arc_tan(current_path[4] - V60[0], current_path[5] - V60[1]);
                }
                V80[1] = 0.34906584f;
                start_position_and_rotation_transition(*(char **)(boss_data + 0x334),
                                                       V50,
                                                       V60,
                                                       V70,
                                                       V80,
                                                       V90,
                                                       D_L18_00162384_gp,
                                                       drek_boss_arena_transition_frame_count_gp,
                                                       0);
            } else {
                moby[0x20] = 0x15;
                if (voice_is_playing(moby, *(int *)(boss_data + 0x3B0)) != 0) {
                    int i = *(int *)(boss_data + 0x3B0);
                    if (i != -1) {
                        char *e = ((char *)&voice_pool) + i * 0x70;
                        if (*(char **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                            release_voice_slot(i);
                        }
                    }
                    *(int *)(boss_data + 0x3B0) = -1;
                }
                if (MOBY(moby)->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(0x14));
                }
                *(void **)(boss_data + 0x330) = find_class_247_moby_with_largest_xy_angle_in_list(
                    (float *)(boss_data + 0x3C0), hero.motion.pos.f, *(int *)(boss_data + 0x200));
                *(int *)(boss_data + 0x390) = 0;
                *(int *)(boss_data + 0x394) = 0;
            }
        }
    }
}
/* steps a moby's offset vector toward a target, returns whether both settled */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F7CD8), where it is exact; names translated to the US level program. */

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

int advance_drek_boss_planar_position_toward_target(char *moby,
                                                    float movement_speed_limit,

                                                    void *x) {
    char *data = ((char *)MOBY(moby)->pvars);
    char *boss_position = data + 0x3C0;
    float planar_displacement[4];
    float distance_progress[4];
    float initialization_zero = 0.0f;
    float planar_target_distance;
    float acceleration_and_braking_step;
    distance_progress[0] = initialization_zero;
    fast_vec_sub(planar_displacement, x, boss_position);
    planar_displacement[2] = initialization_zero;
    planar_target_distance = fast_vec_length(planar_displacement);
    acceleration_and_braking_step = D_0015ED6C_e[1] * 10.0f;
    advance_accelerated_scalar(planar_target_distance,
                               acceleration_and_braking_step,
                               acceleration_and_braking_step,
                               movement_speed_limit,
                               distance_progress,
                               (float *)(data + 0x374));
    fast_vec_normalize(planar_displacement, planar_displacement, *(float *)(data + 0x374));
    fast_vec_add(boss_position, boss_position, planar_displacement);
    if (planar_target_distance < 0.1f) {
        if (*(float *)(data + 0x374) < 0.1f) {
            return 1;
        }
        return 0;
    }
    return 0;
}

#include "sda.h"

/* Moves a moby's follow distance toward a target, clamped to a range, then applies it. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F7DC0), where it is exact; names translated to the US level program. */

extern float freeze_stats_score_ref_b __asm__("D_0015ED6C");
extern short D_L18_0016234C_gp __asm__("D_L18_0016234C") __attribute__((sda));
extern void  FUN_L00_0025f730(void *a, float x);
int          advance_drek_boss_planar_position_toward_target(char *moby,
                                                             float arg,
                                                             void *x) __asm__("FUN_L18_002f6878");

extern short drek_boss_hero_follow_mode_0f_radius_gp __asm__("D_L18_0016237C") __attribute__((sda));

void update_drek_boss_hero_follow_movement(struct Moby *moby) {
    float follow_target_position[4];
    char *moby_vars = (char *)moby->pvars;
    char *hero_position_bytes = (char *)&hero.motion.pos;
    float follow_speed_limit;
    float lo, hi;
    fast_vec_sub(follow_target_position,
                 hero_position_bytes,
                 D_L18_001600EC + (*(int *)(moby_vars + 0x344) << 7) + 0x30);
    follow_target_position[2] = 0.0f;
    if (*(int *)(hero_position_bytes + 0x200C) == 0xF) {
        FUN_L00_0025f730(follow_target_position,
                         *(float *)&drek_boss_hero_follow_mode_0f_radius_gp);
    } else {
        FUN_L00_0025f730(follow_target_position, *(float *)&D_L18_00162378_gp);
    }
    fast_vec_add(follow_target_position,
                 follow_target_position,
                 D_L18_001600EC + (*(int *)(moby_vars + 0x344) << 7) + 0x30);
    follow_speed_limit = hero.motion.speed_xy;
    follow_speed_limit +=
        (vector_distance_xy(&moby->pos, follow_target_position) - *(float *)&D_L18_0016234C_gp) /
        *(float *)&D_L18_0016234C_gp * (freeze_stats_score_ref_b * 4.75f);
    hi = freeze_stats_score_ref_b * 20.0f;
    lo = freeze_stats_score_ref_b * 4.75f;
    if (follow_speed_limit > hi) {
        follow_speed_limit = hi;
    } else if (follow_speed_limit < lo) {
        follow_speed_limit = lo;
    }
    advance_drek_boss_planar_position_toward_target(
        moby, follow_speed_limit, follow_target_position);
}

/* Pvars of the boss while it circles its arena node. */
typedef struct {
    u8   pad0[0x344];
    s32  node; /* 0x344: index into the level's node table */
    u8   pad348[0x78];
    Vec4 pos; /* 0x3C0: position it steers */
} BossCircleVars;

/* One 0x80-byte arena node. */
typedef struct {
    u8   pad0[0x30];
    Vec4 center; /* 0x30 */
    u8   pad40[0x40];
} ArenaNode;

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_drek_boss_arena_circling_movement.h"

/* Steers the boss around its node on the far side from the hero (at most 10
 * degrees per step), faster the closer the hero is, and eases it to the
 * node's height. */
void update_drek_boss_arena_circling_movement(struct Moby *moby) {
    BossCircleVars *boss_circle_vars = (BossCircleVars *)moby->pvars;
    Vec4            goal;
    f32             circling_goal_angle;
    f32             self_ang;
    f32             near, far, slow, fast;
    f32             speed;
    f32             hero_distance;

    circling_goal_angle = fast_arc_tan(
        hero.motion.pos.f[0] - ((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center.f[0],
        hero.motion.pos.f[1] - ((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center.f[1]);
    self_ang = fast_arc_tan(boss_circle_vars->pos.f[0] -
                                ((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center.f[0],
                            boss_circle_vars->pos.f[1] -
                                ((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center.f[1]);
    if (fast_subtract_rotations(self_ang, circling_goal_angle) < 0.0f)
        circling_goal_angle = fast_add_rotations(circling_goal_angle, -2.3561945f);
    else
        circling_goal_angle = fast_add_rotations(circling_goal_angle, 2.3561945f);
    circling_goal_angle = fast_subtract_rotations(circling_goal_angle, self_ang);
    if (circling_goal_angle > 0.17453292f)
        circling_goal_angle = 0.17453292f;
    else if (circling_goal_angle < -0.17453292f)
        circling_goal_angle = -0.17453292f;
    circling_goal_angle = fast_add_rotations(circling_goal_angle, self_ang);
    if (moby->state == 2) {
        goal.f[0] = fast_cos(circling_goal_angle) * drek_boss_circle_and_fire_radius_gp;
        goal.f[1] = fast_sin(circling_goal_angle) * drek_boss_circle_and_fire_radius_gp;
        goal.f[2] = 0.0f;
    } else {
        goal.f[0] = fast_cos(circling_goal_angle) * drek_boss_default_circling_radius_gp;
        goal.f[1] = fast_sin(circling_goal_angle) * drek_boss_default_circling_radius_gp;
        goal.f[2] = 0.0f;
    }
    fast_vec_add(&goal, &goal, &((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center);
    if (vector_distance_xy(&boss_circle_vars->pos,
                           &((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center) >
        drek_boss_default_circling_radius_gp + 1.0f) {
        speed = 10.0f;
    } else if (hero.state.control_mode != 0xF) {
        near = drek_boss_circling_base_near_hero_distance_gp;
        far = drek_boss_circling_base_far_hero_distance_gp;
        slow = drek_boss_circling_default_far_hero_speed_gp;
        fast = drek_boss_circling_default_near_hero_speed_gp;
        if (moby->state == 0xD) {
            slow = 2.0f;
            fast = 5.5f;
            near *= 0.5f;
            far *= 0.5f;
        } else if (moby->state == 0xF) {
            slow = 6.0f;
            near *= 1.5f;
            fast = 10.0f;
            far *= 1.5f;
        }
        hero_distance = vector_distance_xy(&moby->pos, &hero.motion.pos);
        if (far < hero_distance)
            hero_distance = far;
        else if (hero_distance < near)
            hero_distance = near;
        speed = 1.0f - (hero_distance - near) / (far - near);
        speed = (fast - slow) * speed + slow;
    } else {
        speed = 20.0f;
    }
    advance_drek_boss_planar_position_toward_target((char *)moby, speed * frame_time, &goal);
    approach_value(&boss_circle_vars->pos.f[2],
                   ((ArenaNode *)D_L18_001600EC)[boss_circle_vars->node].center.f[2],
                   frame_time * 4.0f);
}

/* Finds the nearest moby in the self's list within range and returns it. */
extern int    request_floating_platform_sink(void *obj) __asm__("FUN_L18_002d8858");
extern short *D_L18_001AC240_c2[] __asm__("D_L18_001AC240");
extern short  drek_boss_platform_target_range_gp __asm__("D_L18_00162338") __attribute__((sda));
extern short  drek_boss_platform_tilt_frame_count_gp __asm__("D_L18_0016233C") __attribute__((sda));
extern void   request_floating_platform_tilt(void *, int, int) __asm__("FUN_L18_002d8888");

unsigned char *select_drek_boss_platform_firing_target(struct Moby *self) {
    char          *data = (char *)self->pvars;
    short         *group_entry = D_L18_001AC240_c2[*(int *)(data + 0x398)];
    unsigned char *best = 0;
    float          bestd = 1024.0f;
    float          candidate_displacement[4];
    if (group_entry == 0) {
        return 0;
    }
    goto start;
    for (;;) {
    start: {
        int            group_moby_byte_offset = (*(unsigned short *)group_entry & 0x7FFF) << 8;
        unsigned char *listed_moby =
            (unsigned char *)((int)group_moby_byte_offset + (int)D_L18_0015FFD8);
        if (MOBY(listed_moby)->oclass == 0x24B) {
            char          *selection_origin = data + 0x3C0;
            unsigned char *candidate_moby;
            request_floating_platform_sink(listed_moby);
            candidate_moby = D_L18_0015FFD8 + group_moby_byte_offset;
            if (vector_distance_xy(candidate_moby + 0x10, selection_origin) <
                *(float *)&drek_boss_platform_target_range_gp) {
                unsigned char *in_range_moby;
                int            tilt_delay_frames, scaled_frame_count;
                float          dot;

                fast_vec_sub(candidate_displacement, candidate_moby + 0x10, selection_origin);
                in_range_moby = D_L18_0015FFD8 + group_moby_byte_offset;
                tilt_delay_frames = scale_game_frames(0x3C);
                scaled_frame_count =
                    scale_game_frames(*(int *)&drek_boss_platform_tilt_frame_count_gp);
                request_floating_platform_tilt(
                    in_range_moby, tilt_delay_frames, scaled_frame_count);
                dot = fast_vec_dot(candidate_displacement, &self->rot_row0);
                if (0.333f < dot) {
                    float candidate_distance = fast_vec_length(candidate_displacement);
                    if (candidate_distance < bestd) {
                        bestd = candidate_distance;
                        best = candidate_moby;
                    }
                }
            }
        }
    }
        if (*group_entry++ < 0) {
            break;
        }
    }
    return best;
}

extern int D_L18_001AC240_u[] __asm__("D_L18_001AC240");
extern int D_L18_0015FFD8_d __asm__("D_L18_0015FFD8");

/* 0x002f6fa8, 124 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
void hide_drek_boss_moby_group(int unused, int group_index) {
    short *group_entry = D_L18_001AC240_u[group_index];
    if (group_entry != 0) {
        do {
            unsigned char *group_moby =
                D_L18_0015FFD8_d + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
            if (group_moby != 0 && group_moby[0x20] != 0xFE && group_moby[0x20] != 0xFD) {
                unsigned short flags = MOBY(group_moby)->flags;
                MOBY(group_moby)->collision = 0;
                flags |= 1;
                group_moby[0x31] = 0;
                MOBY(group_moby)->flags = flags;
            }
        } while (*group_entry++ >= 0);
    }
}

/* 0x002f7028, 140 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Walks a list of moby indices and resets the flags of each live moby.
void show_drek_boss_moby_group(int unused, int group_index) {
    short *group_entry = D_L18_001AC240_u[group_index];
    if (group_entry != 0) {
        do {
            unsigned char *group_moby =
                D_L18_0015FFD8_d + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
            if (group_moby != 0 && group_moby[0x20] != 0xFE && group_moby[0x20] != 0xFD) {
                group_moby[0x31] = 1;
                MOBY(group_moby)->flags &= 0xFFFE;
                MOBY(group_moby)->collision = *(int *)(((char *)MOBY(group_moby)->pclass) + 0x10);
            }
        } while (*group_entry++ >= 0);
    }
}

/* Picks a new state for the moby at random among the allowed transitions, retrying up to 100 times until one is accepted. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F8518), where it is exact; names translated to the US level program. */

extern short drek_boss_action_eligibility_by_phase[] __asm__("D_L18_001DFB40");
s32          random_integer_below(s32 arg0) __asm__("FUN_00213260");

int select_drek_boss_next_action(struct Moby *moby) {
    enum {
        BOSS_ACTION_CHOICE_LAUNCH_FORWARD_WITH_LATERAL_OFFSET = 0,
        BOSS_ACTION_CHOICE_REPOSITION_AND_AIM = 1,
        BOSS_ACTION_CHOICE_TARGET_GROUND_POINTS = 2,
        BOSS_ACTION_CHOICE_TARGET_RANDOM_ARENA_POINTS = 3,
        BOSS_ACTION_CHOICE_TARGET_SIDE_GROUND_POINTS = 4,
        BOSS_ACTION_CHOICE_FOLLOW_HERO = 5
    };
    char *data = (char *)moby->pvars;
    int   state = *(int *)(data + 0x350);
    int   tries = 0;
    int   boss_action_choice_index;
    int   ok;
    do {
        boss_action_choice_index = random_integer_below(6);
        switch (boss_action_choice_index) {
        case BOSS_ACTION_CHOICE_LAUNCH_FORWARD_WITH_LATERAL_OFFSET:
            state = 0xD;
            break;
        case BOSS_ACTION_CHOICE_REPOSITION_AND_AIM:
            state = 0xE;
            break;
        case BOSS_ACTION_CHOICE_TARGET_GROUND_POINTS:
            state = 0xF;
            break;
        case BOSS_ACTION_CHOICE_TARGET_RANDOM_ARENA_POINTS:
            state = 0x10;
            break;
        case BOSS_ACTION_CHOICE_TARGET_SIDE_GROUND_POINTS:
            state = 0x11;
            break;
        case BOSS_ACTION_CHOICE_FOLLOW_HERO:
            state = 0x13;
            break;
        }
        ok = *(short *)((char *)drek_boss_action_eligibility_by_phase +
                        boss_action_choice_index * 2 + *(int *)(data + 0x34C) * 12);
        if (state == *(int *)(data + 0x350))
            ok = 0;
        if (state == 0x13) {
            if (*(int *)(data + 0x364) != 0) {
                ok = 0;
            } else {
                float d = vector_distance_xy(D_0013E533 + 0xE9D,
                                             D_L18_001600EC + *(int *)(data + 0x25C) * 128 + 0x30);
                if (*(float *)&D_L18_00162378_gp + 2.0f < d)
                    ok = 0;
            }
        }
        if (state == 0x11) {
            if (*(int *)(data + 0x350) == 0xE)
                ok = 0;
        }
        if (ok != 0)
            break;
        tries++;
    } while (tries < 100);
    return state;
}

typedef unsigned int u128_f7220 __attribute__((mode(TI)));

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F8680), where it is exact; names translated to the US level program. */

typedef union {
    u128_f7220 quad;
    float      f[4];
} L18Vector;

void set_drek_boss_relative_aim_yaw(struct Moby *source,
                                    char        *dest,
                                    L18Vector   *from,

                                    L18Vector *to) {
    L18Vector from_position, to_position;
    from_position.quad = from->quad;
    to_position.quad = to->quad;
    *(float *)(dest + 0x68) = fast_subtract_rotations(
        fast_arc_tan(to_position.f[0] - from_position.f[0], to_position.f[1] - from_position.f[1]),
        source->rot.z);
}

extern char *l18_drek_boss_scalar_record_lookup_table __asm__("D_L18_0015EF50");
extern int   D_L18_001623A0_gp __asm__("D_L18_001623A0") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_drek_boss_state_dependent_scalars_4.h"

extern short l18_drek_boss_scalar_acceleration_and_braking_gp __asm__("D_L18_001623B4")
    __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_drek_boss_state_dependent_scalars_3.h"

extern float D_L18_001623DC_gp __asm__("D_L18_001623DC") __attribute__((sda));

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_drek_boss_state_dependent_scalars_2.h"

extern int
queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void  follow_camera_set_distance(float, float, int) __asm__("FUN_L00_002e8450");
extern void  follow_camera_set_height(float, float) __asm__("FUN_L00_002e84b8");
extern void  FUN_L06_00316330(void *);
extern int   drek_boss_scalar_scan_entry_gates[] __asm__("D_L18_0016A290");
extern char  l18_drek_boss_scalar_scan_records[] __asm__("D_L18_00167B50");
extern void  reset_numeric_hud_element(void) __asm__("FUN_L00_00235d80");
extern void  FUN_L06_00249738(void);
extern float advance_accelerated_scalar_7288(float *, float, float *, float, float, float) __asm__("FUN_00213f38");

#include "rnc/overlay/l18/mobys/drek_fight_missile_data/update_drek_boss_state_dependent_scalars.h"

/* Ported from rac1-decomp src/overlays/l18_veldin2/vendor_002F2AE0.c (func_L18_002F86E8) */
void update_drek_boss_state_dependent_scalars(char *moby) {
    char         *boss_data = ((char *)MOBY(moby)->pvars);
    char         *selected_scalar_record = *(char **)(l18_drek_boss_scalar_record_lookup_table +
                                              *(int *)(boss_data + 0x224) * 32 + 0x1c);
    float         f;
    float         x;
    float         y;
    float        *first_scalar_value;
    float        *second_scalar_value;
    float        *first_scalar_update_state;
    float        *second_scalar_update_state;
    unsigned char boss_state;

    fast_dec_timer((int *)(boss_data + 0x360));
    f = vector_distance_xy(hero.motion.pos.f, boss_data + 0x3c0);
    if (f > drek_boss_scalar_blend_max_hero_distance_gp) {
        f = drek_boss_scalar_blend_max_hero_distance_gp;
    } else if (f < drek_boss_scalar_blend_min_hero_distance_gp) {
        f = drek_boss_scalar_blend_min_hero_distance_gp;
    }
    f = f - drek_boss_scalar_blend_min_hero_distance_gp;
    f = f /
        (drek_boss_scalar_blend_max_hero_distance_gp - drek_boss_scalar_blend_min_hero_distance_gp);
    x = (drek_boss_first_scalar_far_hero_target_gp - drek_boss_first_scalar_near_hero_target_gp) *
            f +
        drek_boss_first_scalar_near_hero_target_gp;
    y = (drek_boss_second_scalar_far_hero_target_gp - drek_boss_second_scalar_near_hero_target_gp) *
            f +
        drek_boss_second_scalar_near_hero_target_gp;
    boss_state = MOBY(moby)->state;
    if (boss_state == 0x13) {
        char *g = D_0013E533 + 0xe1d;
        if ((*(int *)(g + 0x208c) == 0xf || *(int *)(g + 0x2084) == 0x42) &&
            *(int *)(boss_data + 0x364) == 0 && *(float *)(boss_data + 0x390) <= 25.0f &&
            *(int *)(boss_data + 0x360) == 0) {
            x = drek_boss_first_scalar_gated_follow_target_gp;
            y = drek_boss_second_scalar_gated_follow_target_gp;
            first_scalar_value = (float *)(selected_scalar_record + 0x34);
            second_scalar_value = (float *)(selected_scalar_record + 0x38);
            first_scalar_update_state = (float *)(boss_data + 0x378);
            second_scalar_update_state = (float *)(boss_data + 0x37c);
        } else {
            first_scalar_value = (float *)(selected_scalar_record + 0x34);
            second_scalar_value = (float *)(selected_scalar_record + 0x38);
            second_scalar_update_state = (float *)(boss_data + 0x37c);
            first_scalar_update_state = (float *)(boss_data + 0x378);
            if (vector_distance_xy(hero.motion.pos.f,
                                   D_L18_001600EC + (*(int *)(boss_data + 0x344) << 7) + 0x30) <
                28.0f) {
                x = drek_boss_first_scalar_near_node_follow_target_gp;
                y = D_L18_001623DC_gp;
            }
        }
    } else if (boss_state == 9 || boss_state == 6) {
        x = drek_boss_follow_camera_distance_target_gp;
        y = drek_boss_follow_camera_height_target_gp;
        first_scalar_value = (float *)(selected_scalar_record + 0x34);
        second_scalar_value = (float *)(selected_scalar_record + 0x38);
        first_scalar_update_state = (float *)(boss_data + 0x378);
        second_scalar_update_state = (float *)(boss_data + 0x37c);
    } else if (boss_state == 0 || boss_state == 10 || boss_state == 11 || boss_state == 1 ||
               boss_state == 0x1b) {
        int   i;
        char *g;
        char *follow_camera_gate_source;
        first_scalar_value = (float *)(selected_scalar_record + 0x34);
        second_scalar_value = (float *)(selected_scalar_record + 0x38);
        for (i = 0,
            first_scalar_update_state = (float *)(boss_data + 0x378),
            second_scalar_update_state = (float *)(boss_data + 0x37c);
             i < 0x30;
             i++) {
            char *scanned_entry;
            if (*(int *)(boss_data + 0x3ec) == 0) {
                char *pl;
                g = D_0013E533 + 0xe1d;
                pl = *(char **)(g + 0x2fc);
                if (pl != 0 && *(short *)(pl + 0xa6) == 0x24b) {
                    *(int *)(boss_data + 0x3ec) = 1;
                }
            }
            if (drek_boss_scalar_scan_entry_gates[i] != 0) {
                scanned_entry = l18_drek_boss_scalar_scan_records + i * 0xa0;
                if (*(short *)(scanned_entry + 0x86) == 0x12) {
                    FUN_L06_00316330(scanned_entry);
                    if (*(int *)(boss_data + 0x368) != -1) {
                        set_animation_parameter(*(int *)(boss_data + 0x368), 0);
                        *(int *)(boss_data + 0x368) = -1;
                    }
                    if (MOBY(moby)->state >= 2) {
                        if (*(int *)(boss_data + 0x3ec) != 0 || D_L18_001623A0_gp != 0) {
                            char *follow_camera_gate_record;
                            follow_camera_gate_source = (char *)&l18_camera;
                            follow_camera_gate_record =
                                *(char **)(follow_camera_gate_source + 0x180);
                            if (follow_camera_gate_record != 0 &&
                                *(short *)(follow_camera_gate_record + 0x86) == 0) {
                                follow_camera_set_distance(
                                    drek_boss_follow_camera_distance_target_gp, 0.003f, 0);
                                follow_camera_set_height(drek_boss_follow_camera_height_target_gp,
                                                         0.003f);
                            }
                        }
                    }
                    break;
                }
            }
        }
    } else {
        first_scalar_value = (float *)(selected_scalar_record + 0x34);
        second_scalar_value = (float *)(selected_scalar_record + 0x38);
        first_scalar_update_state = (float *)(boss_data + 0x378);
        second_scalar_update_state = (float *)(boss_data + 0x37c);
        if (*(float *)(boss_data + 0x38c) > 0.0f) {
            *(int *)(boss_data + 0x368) = queue_animation_update(0x16,
                                                                 0xffff,
                                                                 reset_numeric_hud_element,
                                                                 hud_update_counter,
                                                                 FUN_L06_00249738,
                                                                 boss_data + 0x36c,
                                                                 0x15e);
        }
    }
    advance_accelerated_scalar_7288(
        first_scalar_value,
        x,
        first_scalar_update_state,
        *(float *)&l18_drek_boss_scalar_acceleration_and_braking_gp * D_0015ED6C_e[1],
        *(float *)&l18_drek_boss_scalar_acceleration_and_braking_gp * D_0015ED6C_e[1],
        drek_boss_scalar_maximum_speed_gp * D_0015ED6C_e[0]);
    advance_accelerated_scalar_7288(
        second_scalar_value,
        y,
        second_scalar_update_state,
        *(float *)&l18_drek_boss_scalar_acceleration_and_braking_gp * D_0015ED6C_e[1],
        *(float *)&l18_drek_boss_scalar_acceleration_and_braking_gp * D_0015ED6C_e[1],
        drek_boss_scalar_maximum_speed_gp * D_0015ED6C_e[0]);
}

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F8B00), where it is exact; names translated to the US level program. */

typedef unsigned int Q128 __attribute__((mode(TI), aligned(16)));

typedef union {
    Q128  q;
    float v[4];
} QVec;

extern char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");
extern int  FUN_L00_001fefc8(void *);
extern char drek_boss_firing_secondary_effect_initial_color_gp[4] __asm__("D_L18_001623EC")
    __attribute__((sda));
extern char drek_boss_firing_secondary_effect_final_color_gp[4] __asm__("D_L18_001623F0")
    __attribute__((sda));
extern char drek_boss_firing_particle_count_gp[4] __asm__("D_L18_00162408") __attribute__((sda));
extern char drek_boss_firing_particle_speed_range_start_gp[4] __asm__("D_L18_0016240C")
    __attribute__((sda));
extern char drek_boss_firing_particle_speed_range_end_gp[4] __asm__("D_L18_00162410")
    __attribute__((sda));
extern char drek_boss_firing_particle_initial_size_gp[4] __asm__("D_L18_00162414")
    __attribute__((sda));
extern char drek_boss_firing_particle_size_increment_gp[4] __asm__("D_L18_00162418")
    __attribute__((sda));
extern char drek_boss_firing_particle_gravity_direction_scale_gp[4] __asm__("D_L18_0016241C")
    __attribute__((sda));
extern char drek_boss_firing_particle_increment_range_start_gp[4] __asm__("D_L18_00162420")
    __attribute__((sda));
extern char drek_boss_firing_particle_increment_range_end_gp[4] __asm__("D_L18_00162424")
    __attribute__((sda));
extern char drek_boss_firing_particle_lifetime_range_start_frames_gp[4] __asm__("D_L18_00162428")
    __attribute__((sda));
extern char drek_boss_firing_particle_lifetime_range_end_frames_gp[4] __asm__("D_L18_0016242C")
    __attribute__((sda));
extern char drek_boss_firing_particle_velocity_multiplier_gp[4] __asm__("D_L18_00162430")
    __attribute__((sda));
extern char drek_boss_firing_secondary_effect_count_gp[4] __asm__("D_L18_00162438")
    __attribute__((sda));
extern char
    drek_boss_firing_secondary_effect_emission_length_range_start_gp[4] __asm__("D_L18_0016243C")
        __attribute__((sda));
extern char
    drek_boss_firing_secondary_effect_emission_length_range_end_gp[4] __asm__("D_L18_00162440")
        __attribute__((sda));
extern char
    drek_boss_firing_secondary_effect_lifetime_range_start_frames_gp[4] __asm__("D_L18_00162444")
        __attribute__((sda));
extern char
    drek_boss_firing_secondary_effect_lifetime_range_end_frames_gp[4] __asm__("D_L18_00162448")
        __attribute__((sda));
extern char drek_boss_firing_secondary_effect_initial_size_gp[4] __asm__("D_L18_0016244C")
    __attribute__((sda));
extern void    transform_vector_by_basis(void *, void *, void *);
unsigned char *FUN_L00_002712b8(void  *pos,
                                float *vec,
                                int    s,
                                int    a,
                                int    col,
                                int    n,
                                float  x,
                                float  y,
                                float  z,
                                float  w,
                                float  pw);

void spawn_drek_boss_firing_effects(char *moby, void *effect_position) {
    QVec  emission_direction;
    QVec  position_offset;
    float offset_world_position[4];
    float scaled_emission_direction[4];
    emission_direction.q = 0;
    position_offset.q = 0;
    emission_direction.v[0] = 1.0f;
    emission_direction.v[3] = 1.0f;
    position_offset.v[0] = 0.23f;
    position_offset.v[3] = 1.0f;
    FUN_L00_001fefc8(moby + 0xBC);
    transform_vector_by_basis(&position_offset, &position_offset, moby + 0xC0);
    fast_vec_add(offset_world_position, moby + 0x10, &position_offset);
    transform_vector_by_basis(&emission_direction, &emission_direction, moby + 0xC0);
    {
        int i;
        for (i = 0; i < *(int *)&drek_boss_firing_particle_count_gp; i++) {
            int random_particle_parameter;
            fast_vec_normalize(
                scaled_emission_direction,
                &emission_direction,
                random_float_between(*(float *)&drek_boss_firing_particle_speed_range_start_gp,
                                     *(float *)&drek_boss_firing_particle_speed_range_end_gp));
            random_particle_parameter = truncate_float_to_s32(
                randf_sym((float)*(int *)&drek_boss_firing_particle_increment_range_start_gp,
                          (float)*(int *)&drek_boss_firing_particle_increment_range_end_gp));
            FUN_L00_002712b8(effect_position,
                             scaled_emission_direction,
                             scale_game_frames(random_int_between(
                                 *(int *)&drek_boss_firing_particle_lifetime_range_start_frames_gp,
                                 *(int *)&drek_boss_firing_particle_lifetime_range_end_frames_gp)),
                             0x7F,
                             0xFFFFFF,
                             random_particle_parameter,
                             *(float *)&drek_boss_firing_particle_initial_size_gp,
                             *(float *)&drek_boss_firing_particle_size_increment_gp,
                             *(float *)&drek_boss_firing_particle_velocity_multiplier_gp,
                             *(float *)&drek_boss_firing_particle_gravity_direction_scale_gp,
                             0.85f);
        }
    }
    {
        int j;
        for (j = 0; j < *(int *)&drek_boss_firing_secondary_effect_count_gp; j++) {
            fast_vec_normalize(
                scaled_emission_direction,
                &emission_direction,
                random_float_between(
                    *(float *)&drek_boss_firing_secondary_effect_emission_length_range_start_gp,
                    *(float *)&drek_boss_firing_secondary_effect_emission_length_range_end_gp));
            spawn_burst(
                effect_position,
                scaled_emission_direction,
                *(int *)&drek_boss_firing_secondary_effect_initial_color_gp,
                *(int *)&drek_boss_firing_secondary_effect_final_color_gp,
                scale_game_frames(random_int_between(
                    *(int *)&drek_boss_firing_secondary_effect_lifetime_range_start_frames_gp,
                    *(int *)&drek_boss_firing_secondary_effect_lifetime_range_end_frames_gp)),
                1,
                *(float *)&drek_boss_firing_secondary_effect_initial_size_gp);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F8CE0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x3A0];
    void *p3A0;
    char  pad3A4[0x14];
    int   p3B8;
} L18Q;

extern f32 D_L18_00162450_gp __asm__("D_L18_00162450") __attribute__((sda));
extern f32 D_L18_00162454_gp __asm__("D_L18_00162454") __attribute__((sda));
extern s32 D_L18_00162458_gp __asm__("D_L18_00162458") __attribute__((sda));
extern s32 D_L18_0016245C_gp __asm__("D_L18_0016245C") __attribute__((sda));
extern s32 D_L18_00162460_gp __asm__("D_L18_00162460") __attribute__((sda));

extern short drek_boss_animation_glow_quad_scale_gp __asm__("D_L18_00162464") __attribute__((sda));
extern short drek_boss_animation_glow_position_offset_scale_gp __asm__("D_L18_00162468")
    __attribute__((sda));
extern short drek_boss_animation_glow_blend_start_color_gp __asm__("D_L18_0016246C")
    __attribute__((sda));
extern short drek_boss_animation_glow_blend_end_color_gp __asm__("D_L18_00162470")
    __attribute__((sda));
extern short drek_boss_animation_glow_cycle_period_frames_gp __asm__("D_L18_00162474")
    __attribute__((sda));

void render_drek_boss_joint_glows(struct Moby *moby) {
    char  joint_world_position[16];
    char  animation_glow_world_position[16];
    L18Q *boss_vars = (L18Q *)moby->pvars;
    int   attached_glow_period_frames = scale_game_frames(*(int *)&D_L18_00162460_gp);
    float attached_glow_cycle_value =
        convert_integer_to_float(game_frame % attached_glow_period_frames);
    int attached_glow_color;
    int i;
    attached_glow_cycle_value =
        attached_glow_cycle_value / convert_integer_to_float(attached_glow_period_frames);
    attached_glow_cycle_value = attached_glow_cycle_value * 6.18318f;
    attached_glow_cycle_value = fast_sin(attached_glow_cycle_value - 3.14159f);
    attached_glow_color = fast_tween_color(*(int *)&D_L18_00162458_gp,
                                           *(int *)&D_L18_0016245C_gp,
                                           attached_glow_cycle_value * 0.5f + 0.5f);
    moby_point_position(boss_vars->p3A0, 1, joint_world_position);
    draw_offset_textured_effect_quad(joint_world_position,
                                     attached_glow_color,
                                     *(float *)&D_L18_00162450_gp,
                                     *(float *)&D_L18_00162454_gp);
    moby_point_position(boss_vars->p3A0, 2, joint_world_position);
    draw_offset_textured_effect_quad(joint_world_position,
                                     attached_glow_color,
                                     *(float *)&D_L18_00162450_gp,
                                     *(float *)&D_L18_00162454_gp);
    if ((unsigned)(moby->prev_seq - 11) < 3) {
        int animation_glow_period_frames =
            scale_game_frames(*(int *)&drek_boss_animation_glow_cycle_period_frames_gp);
        float animation_glow_cycle_value =
            convert_integer_to_float(game_frame % animation_glow_period_frames);
        int animation_glow_color;
        animation_glow_cycle_value =
            animation_glow_cycle_value / convert_integer_to_float(animation_glow_period_frames);
        animation_glow_cycle_value = animation_glow_cycle_value * 6.18318f;
        animation_glow_cycle_value = fast_sin(animation_glow_cycle_value - 3.14159f);
        animation_glow_color =
            fast_tween_color(*(int *)&drek_boss_animation_glow_blend_start_color_gp,
                             *(int *)&drek_boss_animation_glow_blend_end_color_gp,
                             animation_glow_cycle_value * 0.5f + 0.5f);
        moby_point_position(moby, 5, animation_glow_world_position);
        draw_offset_textured_effect_quad(
            animation_glow_world_position,
            animation_glow_color,
            *(float *)&drek_boss_animation_glow_quad_scale_gp,
            *(float *)&drek_boss_animation_glow_position_offset_scale_gp);
        moby_point_position(moby, 6, animation_glow_world_position);
        draw_offset_textured_effect_quad(
            animation_glow_world_position,
            animation_glow_color,
            *(float *)&drek_boss_animation_glow_quad_scale_gp,
            *(float *)&drek_boss_animation_glow_position_offset_scale_gp);
    }
    for (i = 0; i < 8; i++) {
        moby_point_position(moby, 11 + i, joint_world_position);
        draw_offset_textured_effect_quad(joint_world_position, boss_vars->p3B8, 0.7f, 0.5f);
    }
}

/* Activate the vendor moby and set its initial flags. */
/* Ported from rac1-decomp (src/overlays/l18/vendor_002F2AE0.c: func_L18_002F8F10), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x20];
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  substate;
    char           pad32[2];
    unsigned short flags;
} Level18VendorMobyShort;

void initialize_moby_state_and_flags(Level18VendorMobyShort *moby) __asm__("FUN_L18_002f7ab0");

void initialize_moby_state_and_flags(Level18VendorMobyShort *moby) {
    if (moby->state == 0) {
        moby->state = 1;
        moby->flags |= 3;
        moby->substate = 0;
    }
}

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F8F38), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x18];
    float          f18;
    char           pad1C[4];
    unsigned char  state;
    char           pad21[0x10];
    unsigned char  substate;
    char           pad32[2];
    unsigned short flags;
    char           pad36[0x0A];
    float          f40;
} M;

void update_moby_scene_visibility_and_height_reflection(M *moby) __asm__("FUN_L18_002f7ad8");

void update_moby_scene_visibility_and_height_reflection(M *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_WAIT_FOR_SCENE_TRIGGER = 1,
        MOBY_WAIT_FOR_SCENE_END = 2
    };
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->state = 1;
    case MOBY_WAIT_FOR_SCENE_TRIGGER:
        if ((D_L18_0015F5C4 == 2 && D_L18_0016D290_lui == 3) ||
            (game_frame < 5 && D_L18_001623A0_gp != 0 && moby->f40 == 0.0f)) {
            moby->state = 2;
            moby->flags |= 1;
            moby->substate = 0;
        }
        break;
    case MOBY_WAIT_FOR_SCENE_END:
        /* retail reaches D_L18_001623A0 here as the word 0x1C past D_L18_00162384 */
        if ((&D_L18_00162384_gp)[7] != 0 && moby->f40 == 0.0f) {
            float height_reflection_delta = 101.6f - moby->f18;
            moby->f40 = fast_add_rotations(moby->f40, 3.14159274f);
            moby->f18 += height_reflection_delta + height_reflection_delta;
        }
        if (D_L18_0015F5C4 != 2) {
            moby->substate = 1;
            moby->state = 1;
            moby->flags &= 0xFFFE;
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F90A0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0xC0];
    char  buf[0x50];
    float f110;
    float pad114;
    float f118;
    char  pad11C[4];
    int   w120;
    int   w124;
    char  pad128[8];
    float f130;
    float f134;
    char *c138;
    char *c13c;
} D1454;

typedef struct {
    char          pad0[0x20];
    unsigned char state;
    char          pad21[0x32];
    unsigned char b53;
    char          pad54[0x24];
    D1454        *data;
} M1454;

extern char *
create_class_29_moby(char *owner, float *pos, float *vec, int arg3) __asm__("FUN_L18_002a75f0");
extern char *create_moby(int);
extern int   FUN_L18_002f8658_u() __asm__("FUN_L18_002f8658");
extern short D_L18_0016247C_gp __asm__("D_L18_0016247C") __attribute__((sda));
extern void  emit_radial_particle_burst(void *, void *, int) __asm__("FUN_L03_00250ae8");
f32          compute_interpolated_record_value(void *arg0) __asm__("FUN_0020c9e0");
extern short D_L18_0016247C __asm__("D_L18_0016247C");
extern void  func_L18_002F9AB8_v(void *) __asm__("FUN_L18_002f8658");

void rocket_tank_update(M1454 *m) __asm__("FUN_L18_002f7c40");
void update_rocket_tank_damage_and_aim(void *mv) __asm__("FUN_L18_002f8050");

void rocket_tank_update(M1454 *moby) {
    enum {
        ROCKET_TANK_INITIALIZE = 0,
        ROCKET_TANK_PATROLLING = 1,
        ROCKET_TANK_FIRING = 2
    };
    D1454 *tank_data = moby->data;
    update_rocket_tank_damage_and_aim(moby);
    switch (moby->state) {
    case ROCKET_TANK_INITIALIZE:
        attach_manipulator((char *)moby, 0, tank_data->buf);
        *(short *)((char *)tank_data + 0x24) = 6;
        *(unsigned char *)((char *)tank_data + 0x28) = 3;
        *(float *)((char *)tank_data + 0x20) = 6.0f;
        tank_data->w124 = scale_game_frames(240);
        tank_data->w120 = 0;
        if (tank_data->c138 == 0) {
            char *c = create_moby(0x14B);
            tank_data->c138 = c;
            c[0x30] = 0x40;
            *(short *)(tank_data->c138 + 0x32) = 0x40;
            tank_data->c138[0x31] = 1;
            {
                char *pl = ((char *)hero.moby);
                long  t = *(long *)(pl + 0x38);
                *(long *)(tank_data->c138 + 0x38) = t;
            }
            *(unsigned short *)(tank_data->c138 + 0x34) = 0x4000;
            {
                char *t = tank_data->c138;
                qcopy(t + 0x10, (char *)moby + 0x10);
                *(int *)(t + 0x94) = 0;
            }
            blend_moby_animation(tank_data->c138, 0, 0, 0);
            update_entity(tank_data->c138);
        }
        if (tank_data->c13c == 0) {
            char *c = create_moby(0x14B);
            tank_data->c13c = c;
            c[0x30] = 0x40;
            *(short *)(tank_data->c13c + 0x32) = 0x40;
            tank_data->c13c[0x31] = 1;
            {
                char *pl = ((char *)hero.moby);
                long  t = *(long *)(pl + 0x38);
                *(long *)(tank_data->c13c + 0x38) = t;
            }
            *(unsigned short *)(tank_data->c13c + 0x34) = 0x8000;
            {
                char *t = tank_data->c13c;
                qcopy(t + 0x10, (char *)moby + 0x10);
                *(int *)(t + 0x94) = 0;
            }
            blend_moby_animation(tank_data->c13c, 0, 0, 0);
            update_entity(tank_data->c13c);
        }
        if (moby->b53 != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        moby->state = 1;
        break;
    case ROCKET_TANK_PATROLLING:
        func_L18_002F9AB8_v(moby);
        if (*(int *)((char *)tank_data + 0xB4) != 2) {
            tank_data->f134 = 0;
            tank_data->f130 = 10.0f;
            if (moby->b53 != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(20));
            }
            moby->state = 2;
        }
        break;
    case ROCKET_TANK_FIRING:
        func_L18_002F9AB8_v(moby);
        approach_value(&tank_data->f130, 30.0f, frame_time * 6.0f);
        tank_data->f134 = fast_add_rotations(tank_data->f134, frame_time * 6.2831855f);
        if (is_value_within_interpolated_window((char *)moby, 4.0f) ||
            is_value_within_interpolated_window((char *)moby, 10.0f)) {
            float firing_vector[4];
            float projectile_spawn_position[4];
            float firing_vector_scale;
            if (compute_interpolated_record_value(moby) < 7.0f) {
                moby_point_position(moby, 1, projectile_spawn_position);
                firing_vector_scale = *(float *)&D_L18_0016247C_gp;
            } else {
                moby_point_position(moby, 2, projectile_spawn_position);
                firing_vector_scale = *(float *)&D_L18_0016247C;
            }
            firing_vector_scale *= frame_time;
            firing_vector[0] =
                fast_cos(tank_data->f110) * (fast_cos(tank_data->f118) * firing_vector_scale);
            firing_vector[1] =
                fast_sin(tank_data->f110) * (fast_cos(tank_data->f118) * firing_vector_scale);
            firing_vector[2] = 0;
            firing_vector[2] = -fast_sin(tank_data->f118) * firing_vector_scale;
            allocate_voice_for_target_entry(0, 0, (int)moby);
            emit_radial_particle_burst(moby, projectile_spawn_position, 0);
            if (vector_distance_xy((char *)moby + 0x10, &hero.motion.pos) > 10.0f) {
                create_class_29_moby((char *)moby, projectile_spawn_position, firing_vector, 0);
            } else {
                create_class_29_moby((char *)moby, projectile_spawn_position, firing_vector, 1);
            }
        }
        if (*(int *)((char *)tank_data + 0xB4) == 2) {
            if (moby->b53 != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
            moby->state = 1;
        }
        break;
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F8F38.c: func_L18_002F94B0), where it is exact; names translated to the US level program. */

extern float D_L18_0015F580[];
extern float vector_length_xy(void *);
extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern int   select_moby_combat_target(unsigned char *, char *, float) __asm__("FUN_L00_0025fcb8");
extern void *FUN_L00_00257470_cf(char  *src,
                                 int    cls,
                                 float *pos,
                                 void  *mat,
                                 int    a8,
                                 int    a9,
                                 float  scale,
                                 float *v10,
                                 float *v11,
                                 float *v12) __asm__("FUN_L00_00263fd8");
extern void  FUN_L00_001fff28(void *, int, float);
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void  func_L00_0025F4A8_alt(void *,
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

void update_rocket_tank_damage_and_aim(void *mv) {
    unsigned char *moby_bytes = mv;
    char          *tank_data = *(char **)(moby_bytes + 0x78);
    int            r;
    float          damage_amount;
    float          explosion_and_aim_scratch[4];
    int            hit_reaction_status;
    hit_reaction(moby_bytes,
                 find_moby_hit(moby_bytes, 0x330000, 0),
                 tank_data + 0x20,
                 0,
                 &hit_reaction_status,
                 &damage_amount,
                 0,
                 4);
    if (hit_reaction_status != 1 && moby_bytes[0x20] != 0) {
        float remaining_health = *(float *)(tank_data + 0x20) - damage_amount;
        *(float *)(tank_data + 0x20) = remaining_health;
        if (remaining_health > 0.0f) {
            tank_data[0x67] = 0x78;
        } else {
            float *pa;
            float *dv;
            qcopy(explosion_and_aim_scratch, moby_bytes + 0x10);
            explosion_and_aim_scratch[2] = explosion_and_aim_scratch[2] + 1.0f;
            pa = (float *)(moby_bytes + 0x10);
            func_L00_0025F4A8_alt(moby_bytes,
                                  tank_data + 0x40,
                                  explosion_and_aim_scratch,
                                  0.0f,
                                  0.0f,
                                  20,
                                  8,
                                  20,
                                  4.0f,
                                  2.0f,
                                  9.0f,
                                  1.0f,
                                  -1,
                                  15.0f,
                                  1,
                                  1,
                                  -1,
                                  0);
            dv = D_L18_0015F580;
            FUN_L00_00257470_cf(moby_bytes, 0x700, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x701, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x702, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x704, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FE, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FE, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FF, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FF, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FF, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            mark_moby_for_removal_cf(moby_bytes, 0, -1);
            mark_moby_for_removal(moby_bytes);
            if (*(char **)(tank_data + 0x138)) {
                mark_moby_for_removal(*(char **)(tank_data + 0x138));
            }
            if (*(char **)(tank_data + 0x13C)) {
                mark_moby_for_removal(*(char **)(tank_data + 0x13C));
            }
            return;
        }
        trooper_anim_update(moby_bytes, (short *)(tank_data + 0x60));
    }
    moby_bytes[0xA4] = 0xFF;
    if (select_moby_combat_target(moby_bytes, tank_data + 0x70, 30.0f) != 2) {
        float *pa = (float *)(moby_bytes + 0x10);
        char  *pl = (char *)&hero.motion.pos;
        if (vector_distance_xy(pa, pl) < 10.0f) {
            char *hero_base_bytes = pl - 0x80;
            *(char **)(tank_data + 0xB0) = *(char **)(hero_base_bytes + 0x2080);
            qcopy(tank_data + 0x70, *(char **)(hero_base_bytes + 0x2080) + 0x10);
        } else if (*(char **)(tank_data + 0xB0)) {
            if (30.0f < vector_distance_xy(pa, *(char **)(tank_data + 0xB0) + 0x10)) {
                *(int *)(tank_data + 0xB4) = 2;
            }
        }
    }
    if (*(int *)(tank_data + 0xB4) != 2) {
        float aim_yaw, aim_pitch;
        fast_vec_sub(explosion_and_aim_scratch, tank_data + 0x70, moby_bytes + 0x10);
        explosion_and_aim_scratch[2] = explosion_and_aim_scratch[2] - 3.0f;
        if (fast_vec_length(explosion_and_aim_scratch) > *(float *)(tank_data + 0x130)) {
            set_vector_length_xy(explosion_and_aim_scratch,
                                 explosion_and_aim_scratch,
                                 *(float *)(tank_data + 0x130));
        }
        aim_yaw = fast_sin(*(float *)(tank_data + 0x134)) * 0.08726646f;
        aim_yaw = fast_add_rotations(
            fast_arc_tan(explosion_and_aim_scratch[0], explosion_and_aim_scratch[1]), aim_yaw);
        aim_pitch = -fast_arc_tan(vector_length_xy(explosion_and_aim_scratch),
                                  explosion_and_aim_scratch[2]);
        if (aim_pitch > 0.34906584f) {
            aim_pitch = 0.34906584f;
        } else if (aim_pitch < -0.7853982f) {
            aim_pitch = -0.7853982f;
        }
        approach_angle((float *)(tank_data + 0x110),
                       (float *)(tank_data + 0x114),
                       aim_yaw,
                       D_0015ED6C_e[1] * 12.566371f,
                       D_0015ED6C_e[1] * 12.566371f,
                       frame_time * 9.424778f);
        approach_angle((float *)(tank_data + 0x118),
                       (float *)(tank_data + 0x11C),
                       aim_pitch,
                       D_0015ED6C_e[1] * 12.566371f,
                       D_0015ED6C_e[1] * 12.566371f,
                       frame_time * 9.424778f);
    } else {
        approach_angle((float *)(tank_data + 0x110),
                       (float *)(tank_data + 0x114),
                       *(float *)(moby_bytes + 0x48),
                       D_0015ED6C_e[1] * 12.566371f,
                       D_0015ED6C_e[1] * 12.566371f,
                       frame_time * 9.424778f);
        approach_angle((float *)(tank_data + 0x118),
                       (float *)(tank_data + 0x11C),
                       0.0f,
                       D_0015ED6C_e[1] * 12.566371f,
                       D_0015ED6C_e[1] * 12.566371f,
                       frame_time * 9.424778f);
    }
    FUN_L00_001fff28(explosion_and_aim_scratch, 1, *(float *)(tank_data + 0x118));
    FUN_L00_001fff28(
        tank_data + 0xD0,
        2,
        fast_subtract_rotations(*(float *)(tank_data + 0x110), *(float *)(moby_bytes + 0x48)));
    multiply_quaternions(tank_data + 0xD0, explosion_and_aim_scratch, tank_data + 0xD0);
    trooper_anim_finish(moby_bytes, tank_data + 0x60);
}

/* Steers the moby along its path toward the next node, slowing on sharp turns. */
extern float D_0015ED6C_g __asm__("D_0015ED6C");
extern short rocket_tank_path_target_speed_gp __asm__("D_L18_00162478") __attribute__((sda));
extern short rocket_tank_attached_moby_speed_reference_gp __asm__("D_L18_00162480")
    __attribute__((sda));
extern void FUN_L00_00258830(int, int, float, float, float, int);

void update_rocket_tank_path_movement(struct Moby *moby) __asm__("FUN_L18_002f8658");

void update_rocket_tank_path_movement(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float path_target_position[4];
    float movement_step[4];
    float path_target_yaw, target_movement_step, z, sc;
    char *attached_moby;
    qcopy(path_target_position,
          (char *)D_L18_001B0EB0[*(int *)(moby_vars + 0x100)] + *(int *)(moby_vars + 0x120) * 16 +
              0x10);
    path_target_yaw =
        fast_arc_tan(path_target_position[0] - moby->pos.x, path_target_position[1] - moby->pos.y);
    approach_angle(&moby->rot.z,
                   (float *)(moby_vars + 0x128),
                   path_target_yaw,
                   D_0015ED6C_e[1] * 3.1415927f,
                   D_0015ED6C_e[1] * 3.1415927f,
                   frame_time * 6.2831855f);
    target_movement_step = *(float *)&rocket_tank_path_target_speed_gp * frame_time;
    if (fast_difference_between_rotations(path_target_yaw, moby->rot.z) > 0.2617994f) {
        target_movement_step = D_0015ED6C_g;
    }
    approach_value((float *)(moby_vars + 0x12C), target_movement_step, D_0015ED6C_e[1]);
    z = 0.0f;
    fast_vec_normalize(movement_step, &moby->rot_row0, *(float *)(moby_vars + 0x12C));
    FUN_L00_00258830((int)moby, (int)movement_step, 0.5f, 0.5f, z, 0);
    if (fast_vec_dist(&moby->pos, path_target_position) < 1.0f) {
        int k = random_integer_below(*D_L18_001B0EB0[*(int *)(moby_vars + 0x100)] - 1) + 1;
        *(int *)(moby_vars + 0x120) =
            (*(int *)(moby_vars + 0x120) + k) % *D_L18_001B0EB0[*(int *)(moby_vars + 0x100)];
    }
    sc = *(float *)(moby_vars + 0x12C) /
         (*(float *)&rocket_tank_attached_moby_speed_reference_gp * frame_time);
    if (sc > 2.0f)
        sc = 2.0f;
    else if (sc < z)
        sc = z;
    attached_moby = *(char **)(moby_vars + 0x138);
    if (attached_moby) {
        char *q;
        MOBY(attached_moby)->anim_speed = sc;
        q = *(char **)(moby_vars + 0x138);
        qcopy(q + 0x40, &moby->rot);
        qcopy(q + 0x10, &moby->pos);
    }
    attached_moby = *(char **)(moby_vars + 0x13C);
    if (attached_moby) {
        char *q;
        MOBY(attached_moby)->anim_speed = sc;
        q = *(char **)(moby_vars + 0x13C);
        qcopy(q + 0x40, &moby->rot);
        qcopy(q + 0x10, &moby->pos);
    }
}

short l18_drek_missile_wander_to_dive_random_bound_gp __asm__("D_L18_00162234") = 0x3e8;

float drek_missile_trail_texture_coordinates[4][2] __asm__("D_L18_001DA2F0") = {
    {0.0f, 0.5f},
    {1.0f, 0.5f},
    {0.0f, 0.5f},
    {1.0f, 0.5f},
};

short l18_drek_boss_aimed_class_3d7_spawn_frame_count_gp __asm__("D_L18_0016239C") = 0x55;
