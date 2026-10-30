/* Level 18 drek_fight_missile: wander, trails and group activation. */
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
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

/* Drek missile Moby.state shared by its update and response helpers. */
enum DrekMissileState {
    DREK_MISSILE_INITIALIZE = 0x0,
    DREK_MISSILE_WANDER = 0x2,
    DREK_MISSILE_DIVE = 0x3,
    DREK_MISSILE_WAIT_TO_EXPLODE = 0x4
};

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

/* Wakes every waiting moby of class 0x54B in a group: moves it to pos and records the caller. */

extern short         *D_L18_001AC240[];
extern unsigned char *D_L18_0015FFD8;

#include "rnc/overlay/l18/mobys/drek_fight_missile.h"

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

short l18_drek_missile_wander_to_dive_random_bound_gp __asm__("D_L18_00162234") = 0x3e8;

float drek_missile_trail_texture_coordinates[4][2] __asm__("D_L18_001DA2F0") = {
    {0.0f, 0.5f},
    {1.0f, 0.5f},
    {0.0f, 0.5f},
    {1.0f, 0.5f},
};
