/* Level 13 fighter jet moby class: moby group switches, flight, targeting, HUD, radar, guns and rockets. */
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/rendering/effect_texture.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
#include "qcopy.h"
#include "qzero.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/ui/help/get_help_message_text.h"

extern int debug_print() __asm__("FUN_001e93b0");

extern void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");

extern void set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");

extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");

extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");

extern unsigned char D_0013E533[];

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

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

extern char *level13_moby_list_head __asm__("D_L13_0015FFE4");

extern float approach_value(float *, float, float);

extern int D_L13_0015F5C4;

extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");

extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");

extern void
spawn_type_74_particle(void *, float *, float *, void *, int) __asm__("FUN_L01_0028a7a8");

short D_L13_001613D8 = 0xff;

short D_L13_001613E8 = -1;

short D_L13_001613F8 = 0x7f4f;

short D_L13_00161408 = -32513;

short D_L13_00161418 = 0x7f00;

/* Sets the state of every live moby of class id whose state is not negative. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/mobyutil_00266128.c: func_L13_00266128), where it is exact; names translated to the US level program. */

extern char *D_L13_0015FFE0;

void jet_group_flag(int id, int state) __asm__("FUN_L13_00265170");

void jet_group_flag(int id, int state) {
    char *moby = (char *)moby_base;
    if ((unsigned long)D_L13_0015FFE0 >= (unsigned long)moby) {
        do {
            if (MOBY(moby)->oclass == id) {
                if (moby[0x20] >= 0)
                    moby[0x20] = state;
            }
            moby += 0x100;
        } while ((unsigned long)D_L13_0015FFE0 >= (unsigned long)moby);
    }
}

/* 0x002651c8, 268 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Sets flag bits and a data pointer on every live object in the pool whose id appears in a zero-terminated list.
void jet_group_switch(short *ids, int a, int b, int c) __asm__("FUN_L13_002651c8");
void jet_group_switch(short *ids,
                      int    clear_flag_bit_1,
                      int    clear_flag_bit_0,

                      int copy_data_pointer) {
    char *moby;
    for (moby = (char *)moby_base; moby <= D_L13_0015FFE0; moby += 0x100) {
        int id_index;
        for (id_index = 0; ids[id_index] != 0 && ids[id_index] < 0x800 && id_index < 0xE0;
             id_index++) {
            if (MOBY(moby)->oclass == ids[id_index] && moby[0x20] >= 0) {
                unsigned short object_flags;
                if (clear_flag_bit_1 != -1) {
                    object_flags = MOBY(moby)->flags;
                    if (clear_flag_bit_1 != 0)
                        object_flags &= 0xFFFD;
                    else
                        object_flags |= 2;
                    MOBY(moby)->flags = object_flags;
                }
                if (clear_flag_bit_0 != -1) {
                    object_flags = MOBY(moby)->flags;
                    if (clear_flag_bit_0 != 0) {
                        moby[0x31] = 1;
                        object_flags &= 0xFFFE;
                    } else {
                        moby[0x31] = 0;
                        object_flags |= 1;
                    }
                    MOBY(moby)->flags = object_flags;
                }
                if (copy_data_pointer != -1) {
                    if (copy_data_pointer != 0)
                        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
                    else
                        MOBY(moby)->collision = 0;
                }
            }
        }
    }
}

extern void append_rotated_textured_screen_quad(float,
                                                float,
                                                float,
                                                float,
                                                float,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int,
                                                int) __asm__("FUN_L11_00310cf8");

/* Draws the target lock marker: four quads a quarter turn apart; same routine as FUN_L11_00311048 with this
   level's texture. */
void draw_fighter_jet_lock_marker(float         x,
                                  float         y,
                                  float         size_scale,
                                  float         quad_rotation_angle,
                                  unsigned char red,
                                  unsigned char green,
                                  unsigned char blue,
                                  unsigned char alpha) __asm__("FUN_L13_002b8320");

void draw_fighter_jet_lock_marker(float         x,
                                  float         y,
                                  float         size_scale,
                                  float         quad_rotation_angle,
                                  unsigned char red,
                                  unsigned char green,
                                  unsigned char blue,

                                  unsigned char alpha) {
    int   effect_texture;
    float quad_size;
    /* Fakematch: retail uses only the low word of the texture; the cast keeps it. */
    effect_texture = ((s32(*)(s32))get_effect_texture)(0x37);
    quad_size = size_scale * 20.0f;
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_angle,
                                        0x3F,
                                        0x3F,
                                        effect_texture,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
    quad_rotation_angle = fast_add_rotations(quad_rotation_angle, 1.5707964f);
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_angle,
                                        0x3F,
                                        0x3F,
                                        effect_texture,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
    quad_rotation_angle = fast_add_rotations(quad_rotation_angle, 1.5707964f);
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_angle,
                                        0x3F,
                                        0x3F,
                                        effect_texture,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
    quad_rotation_angle = fast_add_rotations(quad_rotation_angle, 1.5707964f);
    append_rotated_textured_screen_quad(x,
                                        y,
                                        quad_size,
                                        quad_size,
                                        quad_rotation_angle,
                                        0x3F,
                                        0x3F,
                                        effect_texture,
                                        0xFFFFF3,
                                        red,
                                        green,
                                        blue,
                                        alpha);
}

void spawn_fighter_jet_joint_particles(void *moby, char *flight_data) __asm__("FUN_L13_002b8590");

void spawn_fighter_jet_joint_particles(void *moby, char *flight_data) {
    float initial_particle_velocity[4];
    float joint_world_position[4];
    float first_particle_spawn_position[4];
    float second_particle_spawn_position[4];
    float third_particle_spawn_position[4];
    float particle_velocity_scale;
    int   i;

    for (i = 1; i < 3; i++) {
        *(OvlQuad *)initial_particle_velocity = 0;
        particle_velocity_scale = *(float *)(flight_data + 0x64) / (frame_time * 24.0f);
        if (particle_velocity_scale > 1.0f)
            particle_velocity_scale = 1.0f;
        else if (particle_velocity_scale < 0.35f)
            particle_velocity_scale = 0.35f;
        moby_point_position(moby, i, joint_world_position);
        joint_world_position[3] = 0;
        qcopy(first_particle_spawn_position, joint_world_position);
        qcopy(second_particle_spawn_position, joint_world_position);
        qcopy(third_particle_spawn_position, joint_world_position);
        add_random_offset_to_vector_xyz(first_particle_spawn_position,
                                        *(float *)(flight_data + 0xC4) * 0.02f + 0.02f);
        add_random_offset_to_vector_xyz(second_particle_spawn_position,
                                        *(float *)(flight_data + 0xC4) * 0.02f + 0.02f);
        add_random_offset_to_vector_xyz(third_particle_spawn_position,
                                        *(float *)(flight_data + 0xC4) * 0.01f + 0.02f);
        initial_particle_velocity[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 2.0f) *
              frame_time) *
            particle_velocity_scale;
        spawn_type_74_particle(
            moby, first_particle_spawn_position, initial_particle_velocity, &D_L13_001613D8, 1);
        initial_particle_velocity[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 3.0f) *
              frame_time) *
            particle_velocity_scale;
        spawn_type_74_particle(
            moby, second_particle_spawn_position, initial_particle_velocity, &D_L13_001613E8, 1);
        initial_particle_velocity[0] =
            -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 4.0f) *
              frame_time) *
            particle_velocity_scale;
        spawn_type_74_particle(
            moby, third_particle_spawn_position, initial_particle_velocity, &D_L13_001613F8, 1);
        if (*(float *)(flight_data + 0xC4) > 0.5f) {
            add_random_offset_to_vector_xyz(first_particle_spawn_position,
                                            *(float *)(flight_data + 0xC4) * 0.02f + 0.01f);
            add_random_offset_to_vector_xyz(second_particle_spawn_position,
                                            *(float *)(flight_data + 0xC4) * 0.02f + 0.01f);
            add_random_offset_to_vector_xyz(third_particle_spawn_position,
                                            *(float *)(flight_data + 0xC4) * 0.01f + 0.01f);
            initial_particle_velocity[0] =
                -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 2.0f) *
                  frame_time) *
                particle_velocity_scale;
            spawn_type_74_particle(
                moby, first_particle_spawn_position, initial_particle_velocity, &D_L13_001613D8, 1);
            initial_particle_velocity[0] =
                -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 3.0f) *
                  frame_time) *
                particle_velocity_scale;
            spawn_type_74_particle(moby,
                                   second_particle_spawn_position,
                                   initial_particle_velocity,
                                   &D_L13_001613E8,
                                   1);
            initial_particle_velocity[0] =
                -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 4.0f) *
                  frame_time) *
                particle_velocity_scale;
            spawn_type_74_particle(
                moby, third_particle_spawn_position, initial_particle_velocity, &D_L13_001613F8, 1);
            initial_particle_velocity[0] =
                -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 2.0f) *
                  frame_time) *
                particle_velocity_scale;
            spawn_type_74_particle(
                moby, first_particle_spawn_position, initial_particle_velocity, &D_L13_00161408, 1);
            initial_particle_velocity[0] =
                -((random_float_between(6.0f, 7.0f) + *(float *)(flight_data + 0xC4) * 3.0f) *
                  frame_time) *
                particle_velocity_scale;
            spawn_type_74_particle(
                moby, joint_world_position, initial_particle_velocity, &D_L13_00161418, 1);
        }
    }
}

typedef struct {
    short tex;
    short u;
    short v;
    short w;
    short h;
    short fA;
    short x;
    short y;
} IconEC5D0;

extern IconEC5D0 l13_fighter_jet_radar_icon_definitions[] __asm__("D_L13_001CBDB8");

extern unsigned char frame_size[] __asm__("D_0013E504");

extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");

void draw_fighter_jet_radar_icon(char *radar_rotation,
                                 char *from,
                                 char *moby,
                                 int   icon,
                                 long  color,
                                 int   tex) __asm__("FUN_L13_002b89f8");

void draw_fighter_jet_radar_icon(

    char *radar_rotation, char *from, char *moby, int icon, long color, int tex) {
    float      turn[4];
    float      radar_rotation_matrix[16];
    float      delta[4];
    float      radar_offset[4];
    float      radar_distance;
    int        x, y;
    IconEC5D0 *icon_definition;

    qzero(turn);
    turn[2] = *(float *)(radar_rotation + 8);
    turn[2] = fast_subtract_rotations(1.5707964f, turn[2]);
    euler_rotation_matrix(radar_rotation_matrix, turn);
    fast_vec_sub(delta, moby + 0x10, from);
    delta[2] = 0.0f;
    transform_vector(radar_offset, delta, radar_rotation_matrix);
    fast_vec_scale(radar_offset, radar_offset, 0.0714285746f);
    radar_distance = fast_vec_length(radar_offset);
    x = truncate_float_to_s32(radar_offset[0]);
    y = truncate_float_to_s32(-radar_offset[1]);
    if (radar_distance < 46.0f) {
        if (radar_distance > 38.0f) {
            float distance_weighted_alpha;
            radar_distance = 46.0f - radar_distance;
            distance_weighted_alpha =
                convert_integer_to_float(((unsigned long)color >> 24) & 0xFF) * radar_distance;
            color &= 0xFFFFFF;
            color = (truncate_float_to_s32(distance_weighted_alpha * 0.125f) << 24) | color;
        }
        icon_definition = &l13_fighter_jet_radar_icon_definitions[icon];
        y -= 0x50;
        y += *(int *)(frame_size);
        x += 0x1B0;
        draw_textured_quad(x - icon_definition->x,
                           y - icon_definition->y,
                           icon_definition->w,
                           icon_definition->h,
                           icon_definition->u,
                           icon_definition->v,
                           icon_definition->w,
                           icon_definition->h,
                           color,
                           get_effect_texture(tex + 0x28 + icon_definition->tex));
    }
}

typedef struct {
    char           pad0[0x34];
    unsigned short f34;
    char           pad36[2];
    long           f38;
} Moby_2B9E50;

extern char fighter_jet_offscreen_arrow_reference_rotation_matrix[] __asm__("D_L13_001672D0");

extern char fighter_jet_target_marker_vertices[] __asm__("D_L13_001CBD40");

extern float fighter_jet_target_selection_offset_vector[] __asm__("D_L13_001672E0");

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

short D_L13_001613C8 = 2;

extern unsigned char D_0013E500[];

extern unsigned char D_0013F3D0[];

extern void
project_world_point_to_screen_pixels(void *, void *, void *, int) __asm__("FUN_L11_00311210");

extern void emit_transformed_2d_vertex_packet(
    void *, int, int, long, float, float, float, float) __asm__("FUN_L13_002b7d50");

extern void draw_fighter_jet_radar_icon(
    char *rot, char *from, char *moby, int icon, long color, int tex) __asm__("FUN_L13_002b89f8");

extern void project_to_screen(f32 *, void *) __asm__("func_001F2070");

int select_fighter_jet_target_and_draw_indicators(
    int   source_moby,
    char *moby_vars,
    char *radar_origin_position,
    void *radar_rotation,
    float f0,
    float f1,
    float f2,
    char *current_target_moby,
    char *excluded_target_moby) __asm__("FUN_L13_002b8be0");

int select_fighter_jet_target_and_draw_indicators(int   source_moby,
                                                  char *moby_vars,
                                                  char *radar_origin_position,
                                                  void *radar_rotation,
                                                  float f0,
                                                  float f1,
                                                  float f2,
                                                  char *current_target_moby,

                                                  char *excluded_target_moby) {
    float v[4];
    float offscreen_arrow_rotation_matrix[16];
    float u[4];
    int   sx, sy, tx, ty;
    char *candidate_moby;
    char *selected_target_moby;
    int   best;
    int  *scr;
    int   dx, dy, target_selection_radius, screen_distance_squared;
    float ang, source_to_candidate_distance, yaw_error, k, angular_error_magnitude,
        arrow_pulse_phase;
    char *tab;

    candidate_moby = level13_moby_list_head;
    selected_target_moby = 0;
    best = 0x7FFFFFFF;
    *(int *)(moby_vars + 0xE8) = 0;
    while (candidate_moby != 0) {
        tab = fighter_jet_target_marker_vertices;
        scr = (int *)(D_0013E500);
        if ((((Moby_2B9E50 *)candidate_moby)->f34 & 0x1000) &&
            !(((Moby_2B9E50 *)candidate_moby)->f34 & 3) && MOBY(candidate_moby)->visible != 0 &&
            candidate_moby != 0 && ((char *)MOBY(candidate_moby)->pclass) != 0 &&
            *(short *)(((char *)MOBY(candidate_moby)->pclass) + 0x46) == 5) {
            project_world_point_to_screen_pixels(candidate_moby + 0x10, &sx, &sy, 0);
            dx = sx - *(int *)(moby_vars + 0xE0);
            dy = sy - *(int *)(moby_vars + 0xE4);
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;
            qcopy(v, fighter_jet_target_selection_offset_vector);
            fast_vec_scale(v,
                           fighter_jet_target_selection_offset_vector,
                           MOBY(candidate_moby)->bsphere.w / 1280.0f);
            fast_vec_add(v, v, candidate_moby + 0x10);
            project_world_point_to_screen_pixels(v, &tx, &ty, 0);
            tx -= sx;
            ty -= sy;
            target_selection_radius =
                truncate_float_to_s32(square_root(convert_integer_to_float(tx * tx + ty * ty))) +
                0x1C;
            if (dx < target_selection_radius && dy < target_selection_radius) {
                if (current_target_moby != 0) {
                    if (candidate_moby == current_target_moby) {
                        best = 0;
                        selected_target_moby = candidate_moby;
                    }
                } else if (candidate_moby != excluded_target_moby) {
                    screen_distance_squared = dx * dx + dy * dy;
                    if (screen_distance_squared < best) {
                        best = screen_distance_squared;
                        selected_target_moby = candidate_moby;
                    }
                }
            }
        }
        switch (MOBY(candidate_moby)->oclass) {
        case 0xE0:
        case 0x4C2:
            draw_fighter_jet_radar_icon(radar_rotation,
                                        radar_origin_position,
                                        candidate_moby,
                                        2,
                                        0x75808080,
                                        *(int *)(moby_vars + 0x104));
            break;
        case 0xE4:
        case 0x4C4:
            draw_fighter_jet_radar_icon(radar_rotation,
                                        radar_origin_position,
                                        candidate_moby,
                                        2,
                                        0x75808080,
                                        *(int *)(moby_vars + 0x104));
            break;
        case 0x52:
            draw_fighter_jet_radar_icon(radar_rotation,
                                        radar_origin_position,
                                        candidate_moby,
                                        1,
                                        0x75004080,
                                        *(int *)(moby_vars + 0x104));
            break;
        case 0x53:
            draw_fighter_jet_radar_icon(radar_rotation,
                                        radar_origin_position,
                                        candidate_moby,
                                        1,
                                        0x75007080,
                                        *(int *)(moby_vars + 0x104));
            break;
        case 0x184:
        case 0x191:
        case 0x3EB:
        case 0x504:
            project_to_screen(v, candidate_moby + 0x10);
            if (MOBY(candidate_moby)->oclass != 0x184) {
                if (MOBY(candidate_moby)->visible != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(
                            tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f, ang);
                    }
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(
                            tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f, -ang);
                    }
                }
            }
            if (MOBY(candidate_moby)->oclass != 0x191) {
                if (MOBY(candidate_moby)->visible == 0) {
                    char *np = candidate_moby + 0x10;
                    int   arrow_alpha;
                    transpose_rotation_matrix(
                        offscreen_arrow_rotation_matrix,
                        fighter_jet_offscreen_arrow_reference_rotation_matrix);
                    fast_vec_sub(u, np, D_0013F3D0);
                    transform_vector(u, u, offscreen_arrow_rotation_matrix);
                    u[0] = -u[1];
                    u[1] = -u[2];
                    u[2] = 0.0f;
                    fast_vec_normalize(u, u, 160.0f);
                    ang = fast_add_rotations(fast_arc_tan(u[0], u[1]), 1.5707964f);
                    u[0] += 256.0f;
                    u[1] += 208.0f;
                    source_to_candidate_distance = fast_vec_dist((char *)source_moby + 0x10, np);
                    yaw_error = absolute_float(fast_subtract_rotations(
                        fast_arc_tan(
                            MOBY(candidate_moby)->pos.x - *(float *)((char *)source_moby + 0x10),
                            MOBY(candidate_moby)->pos.y - *(float *)((char *)source_moby + 0x14)),
                        *(float *)(moby_vars + 0x38)));
                    k = absolute_float(fast_subtract_rotations(
                        -fast_arc_tan(vector_distance_xy((char *)source_moby + 0x10, np),
                                      MOBY(candidate_moby)->pos.z -
                                          *(float *)((char *)source_moby + 0x18)),
                        *(float *)(moby_vars + 0x34)));
                    angular_error_magnitude = square_root(yaw_error * yaw_error + k * k);
                    arrow_pulse_phase =
                        fast_add_rotations(*(float *)(moby_vars + 0x114),
                                           0.1f / (angular_error_magnitude / 3.1415927f));
                    *(float *)(moby_vars + 0x114) = arrow_pulse_phase;
                    arrow_alpha =
                        truncate_float_to_s32((fast_sin(arrow_pulse_phase) + 1.4f) * 53.0f);
                    if (yaw_error < 0.7853982f && k < 0.7853982f) {
                        {
                            float x = (v[0] - scr[4]) * 0.0625f;
                            float y = (v[1] - scr[5]) * 0.0625f;
                            float ang =
                                normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                            emit_transformed_2d_vertex_packet(
                                tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.0f, ang);
                        }
                        {
                            float x = (v[0] - scr[4]) * 0.0625f;
                            float y = (v[1] - scr[5]) * 0.0625f;
                            float ang =
                                normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                            emit_transformed_2d_vertex_packet(
                                tab, 0x1E, 0xFFFFF3, 0x500000FF, x, y, 1.3f, -ang);
                        }
                    } else {
                        emit_transformed_2d_vertex_packet(&D_L13_001613C8,
                                                          3,
                                                          0xFFFFF3,
                                                          (arrow_alpha << 24) | 0x8000,
                                                          u[0],
                                                          u[1],
                                                          240.0f / source_to_candidate_distance +
                                                              1.5f,
                                                          ang);
                    }
                }
                draw_fighter_jet_radar_icon(radar_rotation,
                                            radar_origin_position,
                                            candidate_moby,
                                            0,
                                            0x75808080,
                                            *(int *)(moby_vars + 0x104));
                if (MOBY(candidate_moby)->oclass != 0x191) {
                    *(int *)(moby_vars + 0xE8) = *(int *)(moby_vars + 0xE8) + 1;
                }
            }
            break;
        case 0x6F:
        case 0x4BE:
            if (!(MOBY(candidate_moby)->flags & 1)) {
                project_to_screen(v, candidate_moby + 0x10);
                if (MOBY(candidate_moby)->visible != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(fighter_jet_target_marker_vertices,
                                                          0x1E,
                                                          0xFFFFF3,
                                                          0xFF00FF00,
                                                          x,
                                                          y,
                                                          1.0f,
                                                          -ang);
                    }
                }
                draw_fighter_jet_radar_icon(radar_rotation,
                                            radar_origin_position,
                                            candidate_moby,
                                            1,
                                            0x75808080,
                                            *(int *)(moby_vars + 0x104));
            }
            break;
        case 0x4BF:
            if (!(MOBY(candidate_moby)->flags & 1) && MOBY(candidate_moby)->state >= 2) {
                project_to_screen(v, candidate_moby + 0x10);
                if (MOBY(candidate_moby)->visible != 0) {
                    {
                        float x = (v[0] - scr[4]) * 0.0625f;
                        float y = (v[1] - scr[5]) * 0.0625f;
                        float ang = normalize_angle(convert_integer_to_float(frame_count) / 20.0f);
                        emit_transformed_2d_vertex_packet(fighter_jet_target_marker_vertices,
                                                          0x1E,
                                                          0xFFFFF3,
                                                          0xFF00FF00,
                                                          x,
                                                          y,
                                                          1.0f,
                                                          -ang);
                    }
                }
                draw_fighter_jet_radar_icon(radar_rotation,
                                            radar_origin_position,
                                            candidate_moby,
                                            1,
                                            0x75808080,
                                            *(int *)(moby_vars + 0x104));
            }
            break;
        case 0x185:
        case 0x186:
        case 0x187:
        case 0x188:
        case 0x189:
        case 0x18A:
        case 0x18B:
        case 0x18C:
        case 0x18F:
        case 0x190:
        case 0x534:
        case 0x535:
        case 0x536:
        case 0x537:
        case 0x538:
        case 0x539:
        case 0x53A:
            break;
        }
        candidate_moby = ((char *)MOBY(candidate_moby)->next);
    }
    return (int)selected_target_moby;
}

extern char fighter_jet_radar_rotation[] __asm__("D_L13_001670D0");

extern char D_0013C940[];

int select_fighter_jet_target_and_draw_indicators(int   a,
                                                  char *p,
                                                  char *a2,
                                                  void *a3,
                                                  float f0,
                                                  float f1,
                                                  float f2,
                                                  char *cur,
                                                  char *g) __asm__("FUN_L13_002b8be0");

#define W(p, o) (*(int *)((char *)(p) + (o)))

#define P(p, o) (*(char **)((char *)(p) + (o)))

#define H(p, o) (*(short *)((char *)(p) + (o)))

#define B(p, o) (*(unsigned char *)((char *)(p) + (o)))

#define FLAG hero.ship_ammo

void update_fighter_jet_target_tracking(int   source_moby,
                                        char *moby_vars) __asm__("FUN_L13_002b95b8");

void update_fighter_jet_target_tracking(int source_moby, char *moby_vars) {
    char *excluded_target_moby = 0;
    char *pad;
    char *current_target_moby;
    char *selected_target_moby;
    float radar_rotation_copy[4];
    if (!(D_L13_0015F5C4 >= 3 && D_L13_0015F5C4 <= 4)) {
        fast_dec_timer((int *)(moby_vars + 0x8C));
    }
    if (W(moby_vars, 0x8C) != 0 && P(moby_vars, 0x88) != 0 &&
        H(P(moby_vars, 0x88), 0xA6) == W(moby_vars, 0x124) && B(P(moby_vars, 0x88), 0x20) != 0xFE &&
        B(P(moby_vars, 0x88), 0x20) != 0xFD) {
        pad = D_0013C940;
        if (B(P(moby_vars, 0x88), 0x31) != 0) {
            if (W(pad, 0x1B4) & 0x2000) {
                goto take;
            }
            if (FLAG != 0) {
                goto skip;
            }
        }
    }
    pad = D_0013C940;
    if (W(pad, 0x1B4) & 0x2000) {
    take:
        excluded_target_moby = P(moby_vars, 0x88);
    }
    W(moby_vars, 0xEC) = 0;
    W(moby_vars, 0x124) = -1;
    W(moby_vars, 0x88) = 0;
skip:
    qcopy(radar_rotation_copy, fighter_jet_radar_rotation);
    selected_target_moby =
        (char *)select_fighter_jet_target_and_draw_indicators(source_moby,
                                                              moby_vars,
                                                              fighter_jet_radar_rotation - 0x10,
                                                              radar_rotation_copy,
                                                              0.19634955f,
                                                              0.19634955f,
                                                              255.0f,
                                                              P(moby_vars, 0x88),
                                                              excluded_target_moby);
    if ((selected_target_moby != P(moby_vars, 0x88) && W(moby_vars, 0x8C) >= 0x1F5) || FLAG == 0) {
        W(moby_vars, 0xEC) = 0;
        W(moby_vars, 0x124) = -1;
        W(moby_vars, 0x88) = 0;
    }
    if (selected_target_moby != 0 &&
        (P(moby_vars, 0x88) == 0 ||
         (H(P(moby_vars, 0x88), 0xA6) != 0x3EB && H(selected_target_moby, 0xA6) == 0x3EB)) &&
        FLAG != 0) {
        P(moby_vars, 0x88) = selected_target_moby;
        W(moby_vars, 0x124) = H(selected_target_moby, 0xA6);
        W(moby_vars, 0x8C) = W(moby_vars, 0x138);
    }
    if (selected_target_moby != P(moby_vars, 0x88)) {
        W(moby_vars, 0x118) = W(moby_vars, 0x118) + 1;
        if (scale_game_frames(0x3C) < W(moby_vars, 0x118)) {
            W(moby_vars, 0xEC) = 0;
            W(moby_vars, 0x124) = -1;
            W(moby_vars, 0x88) = 0;
        }
    } else {
        W(moby_vars, 0x118) = 0;
    }
    current_target_moby = P(moby_vars, 0x88);
    if (current_target_moby != 0 && W(moby_vars, 0x84) == 0) {
        if (H(current_target_moby, 0xA6) != W(moby_vars, 0x124) ||
            B(current_target_moby, 0x20) == 0xFE || B(current_target_moby, 0x20) == 0xFD) {
            W(moby_vars, 0x88) = 0;
            W(moby_vars, 0x124) = -1;
        }
    }
}

extern char fighter_jet_ammo_icon_vertices[] __asm__("D_L13_001CBC60");

extern int fighter_jet_health_palette_backup[] __asm__("D_L13_001CBE70");

int *hp_bar_clut[1] __asm__("D_L13_001613C0") = {0};

extern unsigned char D_0013F350[];

extern void append_scaled_screen_triangle_strip(
    float, float, float, void *, int, int, long) __asm__("FUN_L11_00310ad0");

extern void append_rotated_sprite_quad(
    float, float, float, float, float, int, int, int, int, int, int, int, float, float);

void update_fighter_jet_target_tracking(int a, char *p) __asm__("FUN_L13_002b95b8");

extern int font_print_center_small(int, int, long, void *, int) __asm__("FUN_001f6b88");

void draw_fighter_jet_hud(struct Moby *moby) __asm__("FUN_L13_002b97f8");

void draw_fighter_jet_hud(struct Moby *moby) {
    char          *moby_vars = ((char *)moby->pvars);
    char          *screen_dimensions;
    unsigned char *player_ammo_capacity_state;
    unsigned char *player_ammo_state;
    unsigned char *player_health_state;
    unsigned char *health_warning_player_state;
    int            target_screen_x;
    int            target_screen_y;
    int            aim_reticle_texture;
    int            i;
    unsigned char  ammo_icon_count;
    unsigned char  target_marker_alpha;
    int            j;
    int            x;
    int            y;
    int            ex;
    int            ey;
    int            target_marker_green;
    int            health_palette_fill_end;
    int            lo;
    int            hi;
    int            health_palette_index;
    float          marker_transition_fraction;
    float          k;
    float          marker_opacity_factor;
    float          hud_health_step;
    float          hud_health_step_magnitude;

    update_fighter_jet_target_tracking((int)moby, moby_vars);
    if (*(int *)(moby_vars + 0x88) != 0) {
        project_world_point_to_screen_pixels(
            *(char **)(moby_vars + 0x88) + 0x10, &target_screen_x, &target_screen_y, 0);
        if (*(int *)(moby_vars + 0x8C) > scale_game_frames(0x1F4)) {
            marker_transition_fraction =
                convert_integer_to_float(*(int *)(moby_vars + 0x8C) - scale_game_frames(0x1F4));
            marker_transition_fraction =
                marker_transition_fraction / convert_integer_to_float(scale_game_frames(0x4B));
            marker_opacity_factor = 2.0f * (1.0f - marker_transition_fraction);
            k = marker_transition_fraction * 5.0f + 1.0f;
            if (marker_opacity_factor > 1.0f)
                marker_opacity_factor = 1.0f;
            target_marker_alpha = truncate_float_to_s32(marker_opacity_factor * 96.0f);
            draw_fighter_jet_lock_marker(
                convert_integer_to_float(target_screen_x),
                convert_integer_to_float(target_screen_y),
                k,
                normalize_angle(convert_integer_to_float(frame_count) / 30.0f),
                0,
                0xFF,
                0,
                target_marker_alpha);
            *(int *)(moby_vars + 0xEC) = 0;
        } else {
            target_marker_green = 0xFF;
            if ((*(int *)(moby_vars + 0x8C) / scale_game_frames(0x14)) & 1)
                target_marker_green = 0;
            draw_fighter_jet_lock_marker(convert_integer_to_float(target_screen_x),
                                         convert_integer_to_float(target_screen_y),
                                         1.0f,
                                         0.0f,
                                         0xFF,
                                         target_marker_green,
                                         0,
                                         0x60);
            *(int *)(moby_vars + 0xEC) = *(int *)(moby_vars + 0x88);
        }
    }
    y = 0x40;
    x = 0x18;
    player_ammo_capacity_state = D_0013F350;
    ammo_icon_count = player_ammo_capacity_state[0x15F7];
    for (i = 0; i < ammo_icon_count; i++) {
        player_ammo_state = D_0013F350;
        if (i < player_ammo_state[0x15F6]) {
            append_scaled_screen_triangle_strip(
                x, y, 0.5f, fighter_jet_ammo_icon_vertices, 0x19, 0xFFFFF3, 0x50008F00);
        } else {
            append_scaled_screen_triangle_strip(
                x, y, 0.5f, fighter_jet_ammo_icon_vertices, 0x19, 0xFFFFF3, 0x20004F00);
        }
        ammo_icon_count = player_ammo_state[0x15F7];
        if (i == ammo_icon_count >> 1) {
            y = 0x2E;
            x += 0x1E;
        }
        y += 0x12;
    }
    ex = *(int *)(moby_vars + 0xE0);
    ey = *(int *)(moby_vars + 0xE4);
    /* Fakematch: retail uses only the low word of the texture; the cast keeps it. */
    aim_reticle_texture = ((s32(*)(s32))get_effect_texture)(0x11);
    append_rotated_sprite_quad(ex,
                               ey,
                               40.0f,
                               40.0f,
                               0.0f,
                               0x3F,
                               0x3F,
                               aim_reticle_texture,
                               0xFFFFF3,
                               0xFF20FF20,
                               0,
                               0,
                               0.5f,
                               0.5f);
    append_rotated_sprite_quad(
        ex,
        ey,
        40.0f,
        40.0f,
        0.0f,
        0x3F,
        0x3F,
        /* Fakematch: retail uses only the low word of the texture; the cast keeps it. */
        ((s32(*)(s32))get_effect_texture)(0x12),
        0xFFFFF3,
        0xFF20FF20,
        0,
        0,
        0.5f,
        0.5f);
    append_rotated_sprite_quad(
        ex,
        ey,
        10.0f,
        10.0f,
        0.0f,
        0x1F,
        0x1F,
        /* Fakematch: retail uses only the low word of the texture; the cast keeps it. */
        ((s32(*)(s32))get_effect_texture)(8),
        0xFFFFF3,
        0xFF20FF20,
        0,
        0,
        0.5f,
        0.5f);
    health_warning_player_state = D_0013F350;
    if (*(float *)(health_warning_player_state + 0x15FC) <
            *(float *)(health_warning_player_state + 0x1600) / 10.0f &&
        (frame_count / 90) & 1) {
        font_print_center_small(0x100, 0x186, 0x80000080L, get_help_message_text(0x5269), 0x64);
    }
    player_health_state = D_0013F350;
    hud_health_step = *(float *)(player_health_state + 0x15FC) - *(float *)(moby_vars + 0xF0);
    hud_health_step *= 0.2f;
    hud_health_step_magnitude = absolute_float(hud_health_step);
    if (hud_health_step_magnitude > 1.0f)
        hud_health_step /= hud_health_step_magnitude;
    *(float *)(moby_vars + 0xF0) += hud_health_step;
    health_palette_fill_end = truncate_float_to_s32(*(float *)(moby_vars + 0xF0) * 251.0f /
                                                    *(float *)(player_health_state + 0x1600)) +
                              2;
    if (health_palette_fill_end > 0xFD)
        health_palette_fill_end = 0xFD;
    if (health_palette_fill_end < 2)
        health_palette_fill_end = 2;
    lo = *(int *)(moby_vars + 0xF4) <= health_palette_fill_end ? *(int *)(moby_vars + 0xF4)
                                                               : health_palette_fill_end;
    hi = *(int *)(moby_vars + 0xF4) < health_palette_fill_end ? health_palette_fill_end
                                                              : *(int *)(moby_vars + 0xF4);
    for (j = lo; j <= hi; j++) {
        health_palette_index = (j & 0xE7) | ((j & 0x10) >> 1) | ((j & 8) << 1);
        if (j < health_palette_fill_end) {
            hp_bar_clut[0][health_palette_index] =
                fighter_jet_health_palette_backup[health_palette_index];
        } else {
            hp_bar_clut[0][health_palette_index] = 0x80000000;
        }
    }
    screen_dimensions = (char *)D_0013E500;
    draw_textured_quad(0x170,
                       *(int *)(screen_dimensions + 4) - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(*(int *)(moby_vars + 0x104) + 0x28));
    draw_textured_quad(0x170,
                       *(int *)(screen_dimensions + 4) - 0x90,
                       0x80,
                       0x80,
                       0,
                       0,
                       0x80,
                       0x80,
                       0x70808080,
                       get_effect_texture(*(int *)(moby_vars + 0x104) + 0x29));
    *(int *)(moby_vars + 0xF4) = health_palette_fill_end;
}

extern int
spawn_fighter_jet_missile(void *, void *, void *, float, float) __asm__("FUN_L11_00308848");

extern unsigned char D_0013A4E0[];

void update_fighter_jet_gun_and_aim(struct Moby *moby,
                                    char        *flight_data,
                                    float        azimuth_angle,
                                    float        elevation_angle) __asm__("FUN_L13_002b9df8");

void update_fighter_jet_gun_and_aim(struct Moby *moby,
                                    char        *flight_data,
                                    float        azimuth_angle,

                                    float elevation_angle) {
    char aim_vector[16];
    char projectile_spawn_position[16];
    char muzzle_offset[16];
    char aim_world_position[16];
    build_spherical_offset(aim_vector, frame_time * 400.0f, azimuth_angle, elevation_angle);
    fast_vec_add(aim_vector, aim_vector, flight_data);
    if (fast_dec_timer(flight_data + 0x80) && (*(int *)(D_0013A4E0 + 0x2610) & 0x84)) {
        allocate_voice_for_target_entry(3, 0, (int)moby);
        fast_vec_normalize(muzzle_offset, aim_vector, 2.2f);
        moby_point_position(moby, 3 + (flight_data[0x61] & 1), projectile_spawn_position);
        fast_vec_add(projectile_spawn_position, projectile_spawn_position, muzzle_offset);
        spawn_fighter_jet_missile(moby, aim_vector, projectile_spawn_position, 200.0f, -1.0f);
        *(int *)(flight_data + 0x80) = scale_game_frames(4);
        flight_data[0x61] ^= 1;
    }
    fast_vec_normalize(aim_vector, aim_vector, 23.0f);
    fast_vec_add(aim_world_position, aim_vector, &moby->pos);
    project_world_point_to_screen_pixels(
        aim_world_position, flight_data + 0xE0, flight_data + 0xE4, 0);
}

extern int
spawn_fighter_jet_rocket(void *, void *, float, int, void *, int) __asm__("FUN_L13_002e6a58");

void fire_fighter_jet_rocket(struct Moby *moby, char *flight_data) __asm__("FUN_L13_002b9f58");

void fire_fighter_jet_rocket(struct Moby *moby, char *flight_data) {
    if (fast_dec_timer(flight_data + 0x84) && (*(int *)(D_0013A4E0 + 0x2610) & 0x28)) {
        unsigned char *player_ammo_state = D_0013E533 + 0xE1D;
        if (player_ammo_state[0x15F6] != 0) {
            char  projectile_spawn_position[16];
            char  projectile_spawn_offset[16];
            float projectile_scale;
            int   projectile_frame_count;
            char *projectile_initial_rotation = &moby->rot;
            short t = (*(unsigned short *)(flight_data + 0x6A) + 1) & 3;
            *(short *)(flight_data + 0x6A) = t;
            moby_point_position(moby, t + 7, projectile_spawn_position);
            fast_vec_normalize(projectile_spawn_offset, flight_data, 0.3f);
            fast_vec_add(
                projectile_spawn_position, projectile_spawn_position, projectile_spawn_offset);
            projectile_scale = frame_time * 100.0f;
            projectile_frame_count = scale_game_frames(300);
            if (spawn_fighter_jet_rocket(moby,
                                         projectile_spawn_position,
                                         projectile_scale,
                                         *(int *)(flight_data + 0xEC),
                                         projectile_initial_rotation,
                                         projectile_frame_count) != 0) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                player_ammo_state[0x15F6]--;
                *(int *)(flight_data + 0x84) = scale_game_frames(30);
            }
        }
    }
}

extern float D_L13_0016CDF0 __attribute__((section(".data")));

extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");

typedef float FlightVec[4] __attribute__((aligned(16)));

typedef struct {
    char           pad00[0x10];
    FlightVec      pos;
    unsigned char  state;
    char           pad21[0x13];
    unsigned short flags;
    char           pad36[0xA];
    float          rot_x;
    float          rot_y;
    float          rot_z;
    char           pad4C[0x70];
    unsigned char  unkBC;
} FlightMoby;

typedef struct {
    char          pad00[0x10];
    FlightVec     direction;
    float         camera_roll, camera_pitch, camera_yaw, pad2C;
    float         roll, pitch, yaw, pad3C;
    char          pad40[0x20];
    unsigned char camera_mode;
    unsigned char gun_side;
    short         death_timer;
    float         speed;
    short         launch_frames;
    short         gun_index;
    float         steer_x;
    float         steer_y;
    char          pad74[0x1C];
    float         steer_x_velocity, steer_y_velocity, roll_velocity, pad9C;
    float         camera_roll_velocity, camera_pitch_velocity, camera_yaw_velocity, padAC;
    float         fov, camera_offset_x, camera_offset_y, camera_offset_z;
    float         fov_velocity, speed_fraction, camera_offset_x_velocity, camera_offset_y_velocity;
    float         camera_offset_z_velocity, padD4, padD8;
    int           hit_cooldown;
    char          padE0[0x2C];
    int           boost_sound;
    int           target_path;
} FlightData;

extern LevelCuboid *l13_cuboids_gp __asm__("D_L13_001600EC") __attribute__((section(".sdata")));

extern f32 frame_scale_gp __asm__("D_0015ED60") __attribute__((section(".sdata")));

extern f32 frame_scale_sq_gp __asm__("D_0015ED64") __attribute__((section(".sdata")));

extern f32 frame_time_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));

u64 fighter_jet_steering_reference_position __asm__("D_L13_00161430") = 0x4400000044000000;

extern char *D_L13_00167100 __attribute__((section(".data")));

extern char coll_hit_1638[] __asm__("D_L13_00174140");

extern char fighter_jet_camera_mode_correction_message[] __asm__("D_L13_001F4450");

extern float
spring_angle(char *, char *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");

extern float interpolate_value_with_repeated_squaring(
    int, int, float, float, float, float, float) __asm__("FUN_L13_002ba088");

extern void update_view_context(void) __asm__("FUN_001f2d98");

void update_fighter_jet_flight(void *moby_arg, void *data_arg) __asm__("FUN_L13_002ba178");

void update_fighter_jet_flight(void *moby_arg, void *data_arg) {
    FlightMoby *moby = moby_arg;
    FlightData *jet_flight_state = data_arg;
    FlightVec   old_pos;
    FlightVec   impulse;
    FlightVec   velocity;
    FlightVec   cross;
    FlightVec   vertical;
    FlightVec   camera_angles;
    FlightVec   target;
    FlightVec   offset;
    float       steer_x;
    float       steer_y;
    float       steer_rate_x;
    float       steer_rate_y;
    float       pitch_limit;
    float       yaw_limit;
    float       distance;
    float       height, height_again;
    float       half_fov, tan_half_fov;
    float       speed_fraction;
    char       *pad_state;
    char       *player;
    char       *voice;
    char       *hit;
    int         voice_index;
    int         hit_class;

    *(u128 *)velocity = 0;
    *(u128 *)vertical = 0;
    vertical[2] = 1.0f;
    velocity[2] = frame_time_gp * 8.0f;
    moby->flags &= 0xFEFF;
    *(u128 *)impulse = *(u128 *)velocity;
    if (moby->unkBC == 0) {
        if (jet_flight_state->launch_frames > scale_game_frames(120)) {
            pad_state = (char *)D_0013A4E0 + 0x2460;
            if (*(int *)(pad_state + 0x1B4) & 0x40) {
                voice_index = jet_flight_state->boost_sound;
                if (voice_index != -1) {
                    voice = (char *)D_0013E533 + 0x1D + voice_index * 0x70;
                    if (*(char **)(voice + 0x88) == (char *)moby &&
                        *(unsigned char *)(voice + 0x74) != 0) {
                        release_voice_slot(voice_index);
                    }
                }
                jet_flight_state->boost_sound = -1;
                jet_flight_state->boost_sound = allocate_voice_for_target_entry(1, 4, (int)moby);
            } else if (*(int *)(pad_state + 0x1B8) & 0x40) {
                voice_index = jet_flight_state->boost_sound;
                if (voice_index != -1) {
                    voice = (char *)D_0013E533 + 0x1D + voice_index * 0x70;
                    if (*(char **)(voice + 0x88) == (char *)moby &&
                        *(unsigned char *)(voice + 0x74) != 0) {
                        release_voice_slot(voice_index);
                    }
                }
                jet_flight_state->boost_sound = -1;
            }
            pad_state = (char *)D_0013A4E0 + 0x2460;
            if (*(int *)(pad_state + 0x1B0) & 0x40) {
                float boosted = jet_flight_state->speed + frame_scale_gp * 0.035f;
                float max_speed = frame_time_gp * 30.0f;
                jet_flight_state->speed = boosted;
                if (max_speed < boosted) {
                    jet_flight_state->speed = max_speed;
                }
            } else {
                float cruise_speed = frame_time_gp * 24.0f;
                float step = frame_scale_gp * 0.025f;
                float speed = jet_flight_state->speed;
                if (speed < cruise_speed - step) {
                    jet_flight_state->speed = speed + step;
                } else if (cruise_speed < speed) {
                    jet_flight_state->speed =
                        speed + (cruise_speed - speed) * (frame_scale_gp * 0.1f);
                } else {
                    jet_flight_state->speed = cruise_speed;
                }
            }
        } else {
            jet_flight_state->launch_frames += 1;
            jet_flight_state->speed += frame_scale_gp * 0.003f;
        }
    } else {
        jet_flight_state->speed = 0.0001f;
    }

    if (jet_flight_state->launch_frames > scale_game_frames(5)) {
        char *player = (char *)D_0013E533 + 0xE1D;
        steer_rate_x = frame_scale_gp * 0.1f;
        steer_rate_y = frame_scale_gp * 0.25f;
        steer_x = *(float *)(player + 0x1D20);
        steer_y = *(float *)(player + 0x1D24);
        if (*(int *)(D_0013A4E0 + 0x2600) & 0x200) {
            steer_x = steer_x / (frame_scale_gp * 0.012271847f);
            steer_x = steer_x * (frame_scale_gp * 0.015707964f);
            steer_y = steer_y / (frame_scale_gp * 0.012271847f);
            steer_y = steer_y * (frame_scale_gp * 0.015707964f);
            steer_rate_x = frame_scale_gp * 0.08f;
            steer_rate_y = frame_scale_gp * 0.08f;
        }
        vendor_spring(&jet_flight_state->steer_x,
                      &jet_flight_state->steer_x_velocity,
                      steer_x,
                      frame_scale_sq_gp * 0.03f,
                      frame_scale_sq_gp,
                      steer_rate_x);
        vendor_spring(&jet_flight_state->steer_y,
                      &jet_flight_state->steer_y_velocity,
                      steer_y,
                      frame_scale_sq_gp * 0.03f,
                      frame_scale_sq_gp,
                      steer_rate_y);
        spring_angle((char *)&jet_flight_state->roll,
                     (char *)&jet_flight_state->roll_velocity,
                     0,
                     jet_flight_state->steer_x,
                     frame_scale_sq_gp * 0.05f,
                     frame_scale_sq_gp,
                     frame_scale_gp);
        jet_flight_state->pitch = fast_subtract_rotations(
            jet_flight_state->pitch, jet_flight_state->steer_y * (frame_scale_gp * 0.012271847f));
        jet_flight_state->yaw = fast_subtract_rotations(
            jet_flight_state->yaw, jet_flight_state->steer_x * (frame_scale_gp * 0.012271847f));
        if (jet_flight_state->pitch > 1.3962634f) {
            jet_flight_state->pitch = 1.3962634f;
        } else if (jet_flight_state->pitch < -1.3962634f) {
            jet_flight_state->pitch = -1.3962634f;
        }
        if (moby->rot_y > 1.3962634f) {
            moby->rot_y = 1.3962634f;
        } else if (moby->rot_y < -1.3962634f) {
            moby->rot_y = -1.3962634f;
        }
    }

    pitch_limit = 310.0f;
    yaw_limit = 500.0f;
    if (jet_flight_state->target_path != -1) {
        int   path_offset = jet_flight_state->target_path << 7;
        char *paths = (char *)l13_cuboids_gp;
        qcopy(target, ((char(*)[128])paths)[jet_flight_state->target_path] + 0x30);
        pitch_limit = fast_vec_length(paths + path_offset + 0x20);
        fast_vec_add(offset,
                     (jet_flight_state->target_path << 7) + (char *)l13_cuboids_gp,
                     (jet_flight_state->target_path << 7) + (char *)l13_cuboids_gp + 0x10);
        yaw_limit = fast_vec_length(offset);
    } else {
        qcopy(target, &fighter_jet_steering_reference_position);
    }

    distance = vector_distance_xy(moby->pos, &fighter_jet_steering_reference_position);
    jet_flight_state->yaw = interpolate_value_with_repeated_squaring(
        1,
        5,
        distance,
        jet_flight_state->yaw,
        fast_arc_tan(target[0] - moby->pos[0], target[1] - moby->pos[1]),
        50.0f,
        yaw_limit);
    moby->rot_z = interpolate_value_with_repeated_squaring(
        1,
        5,
        distance,
        moby->rot_z,
        fast_arc_tan(target[0] - moby->pos[0], target[1] - moby->pos[1]),
        50.0f,
        yaw_limit);
    height = absolute_float(target[2] - moby->pos[2]);
    jet_flight_state->pitch = interpolate_value_with_repeated_squaring(
        1,
        5,
        height,
        jet_flight_state->pitch,
        -fast_arc_tan(vector_distance_xy(moby->pos, target), target[2] - moby->pos[2]),
        50.0f,
        pitch_limit);
    height_again = absolute_float(target[2] - moby->pos[2]);
    moby->rot_y = interpolate_value_with_repeated_squaring(
        1,
        5,
        height_again,
        moby->rot_y,
        -fast_arc_tan(vector_distance_xy(moby->pos, target), target[2] - moby->pos[2]),
        50.0f,
        pitch_limit);
    build_spherical_offset(
        velocity, jet_flight_state->speed, jet_flight_state->yaw, -jet_flight_state->pitch);
    fast_vec_normalize(jet_flight_state->direction, velocity, 1.0f);
    qcopy(old_pos, moby->pos);
    if (moby->unkBC == 0) {
        fast_vec_add(moby->pos, moby->pos, velocity);
    }
    {
        float min_speed = frame_time_gp * 24.0f;
        float excess = jet_flight_state->speed - min_speed;
        float range = frame_time_gp * 30.0f - min_speed;
        if (excess < 0.0f) {
            speed_fraction = 0.0f / range;
        } else {
            speed_fraction = excess / range;
        }
    }
    jet_flight_state->speed_fraction = speed_fraction;
    {
        FlightVec camera_offsets[2];
        /* Fakematch: retail's compiler expands the offset table initializer in place (a clobber,
         * then a quadword clear per row); our cc1 calls memset for it, and this union
         * initializer gives retail's sequence. */
        union {
            u128 q;
            struct {
                FlightVec pos;
                FlightVec rot;
            } v;
        } camera = {0};
        float basis[12];
        camera.v.pos[0] = (-15.0f - jet_flight_state->steer_y - speed_fraction * 4.0f) * 0.25f;
        camera.v.pos[1] = jet_flight_state->steer_x * -3.0f * 0.25f;
        camera.v.pos[2] = (4.0f + jet_flight_state->steer_y * 2.0f) * 0.25f;
        *(u128 *)camera.v.rot = 0;
        camera.v.rot[0] = -1.275f;
        camera.v.rot[2] = 0.65f;
        *(u128 *)camera_offsets[0] = *(u128 *)camera.v.pos;
        *(u128 *)camera_offsets[1] = *(u128 *)camera.v.rot;
        if (moby->state == 2) {
            jet_flight_state->camera_offset_x = camera_offsets[jet_flight_state->camera_mode][0];
            jet_flight_state->camera_offset_y = camera_offsets[jet_flight_state->camera_mode][1];
            jet_flight_state->camera_offset_z = camera_offsets[jet_flight_state->camera_mode][2];
        } else {
            vendor_spring(&jet_flight_state->camera_offset_x,
                          &jet_flight_state->camera_offset_x_velocity,
                          camera_offsets[jet_flight_state->camera_mode][0],
                          frame_scale_sq_gp * 0.15f,
                          frame_scale_sq_gp * 0.8f,
                          frame_scale_gp);
            vendor_spring(&jet_flight_state->camera_offset_y,
                          &jet_flight_state->camera_offset_y_velocity,
                          camera_offsets[jet_flight_state->camera_mode][1],
                          frame_scale_sq_gp * 0.15f,
                          frame_scale_sq_gp * 0.8f,
                          frame_scale_gp);
            vendor_spring(&jet_flight_state->camera_offset_z,
                          &jet_flight_state->camera_offset_z_velocity,
                          camera_offsets[jet_flight_state->camera_mode][2],
                          frame_scale_sq_gp * 0.15f,
                          frame_scale_sq_gp * 0.8f,
                          frame_scale_gp);
        }
        offset[0] = jet_flight_state->camera_offset_x;
        offset[1] = jet_flight_state->camera_offset_y;
        offset[2] = jet_flight_state->camera_offset_z;
        offset[3] = 0.0f;
        euler_rotation_basis(basis, (char *)&jet_flight_state->roll);
        transform_vector_by_basis(target, offset, basis);
        fast_vec_add(camera.v.pos, moby->pos, target);
        set_camera_position(camera.v.pos);
        camera.v.rot[0] =
            fast_add_rotations(jet_flight_state->roll * 0.5f, jet_flight_state->steer_x * 0.03125f);
        camera.v.rot[1] =
            fast_subtract_rotations(fast_add_rotations(jet_flight_state->pitch, 0.09817477f),
                                    jet_flight_state->steer_y * (frame_scale_gp * 0.012271847f));
        camera.v.rot[2] = jet_flight_state->yaw;
        if (moby->state == 2) {
            qcopy(&jet_flight_state->camera_roll, camera.v.rot);
        } else {
            spring_angle((char *)&jet_flight_state->camera_roll,
                         (char *)&jet_flight_state->camera_roll_velocity,
                         0,
                         camera.v.rot[0],
                         frame_scale_sq_gp * 0.15f,
                         frame_scale_sq_gp * 0.8f,
                         frame_scale_gp);
            spring_angle((char *)&jet_flight_state->camera_pitch,
                         (char *)&jet_flight_state->camera_pitch_velocity,
                         0,
                         camera.v.rot[1],
                         frame_scale_sq_gp * 0.15f,
                         frame_scale_sq_gp * 0.8f,
                         frame_scale_gp);
            spring_angle((char *)&jet_flight_state->camera_yaw,
                         (char *)&jet_flight_state->camera_yaw_velocity,
                         0,
                         camera.v.rot[2],
                         frame_scale_sq_gp * 0.15f,
                         frame_scale_sq_gp * 0.8f,
                         frame_scale_gp);
        }
        camera_angles[0] = jet_flight_state->camera_roll;
        camera_angles[2] = jet_flight_state->camera_yaw;
        camera_angles[1] = jet_flight_state->camera_pitch;
        set_camera_rotation(camera_angles);
        if (*(short *)(D_L13_00167100 + 0x86) != 5) {
            debug_print(fighter_jet_camera_mode_correction_message);
            jet_mount(camera.v.pos, camera_angles, 0, 0, 0);
        }
    }
    vendor_spring(&jet_flight_state->fov,
                  &jet_flight_state->fov_velocity,
                  1.134464f + (1.4835298f - 1.134464f) * speed_fraction,
                  0.05f,
                  1.0f,
                  1.0f);
    half_fov = jet_flight_state->fov * 0.5f;
    tan_half_fov = fast_sin(half_fov) / fast_cos(half_fov);
    D_L13_0016CDF0 = tan_half_fov;
    update_view_context();
    moby->rot_x = fast_add_rotations(jet_flight_state->roll, jet_flight_state->steer_x * 0.5f);
    moby->rot_y = fast_add_rotations(jet_flight_state->pitch, -jet_flight_state->steer_y * 0.5f);
    moby->rot_z = fast_add_rotations(jet_flight_state->yaw, -jet_flight_state->steer_x * 0.0f);
    if (coll_sphere(1.2f, moby->pos, 0, moby)) {
        hit = coll_hit_1638;
        if (*(char **)(hit + 0x18) == 0 ||
            (hit_class = *(short *)(*(char **)(hit + 0x18) + 0xA6),
             hit_class != 0 && hit_class != 0x192 && hit_class != 0x4C2 && hit_class != 0x4C3 &&
                 hit_class != 0x52 && hit_class != 0x53)) {
            qcopy(moby->pos, hit + 0x30);
            reflect_vector(velocity, velocity, hit + 0x40);
            jet_flight_state->yaw =
                fast_add_rotations(jet_flight_state->yaw,
                                   fast_subtract_rotations(fast_arc_tan(velocity[0], velocity[1]),
                                                           jet_flight_state->yaw) *
                                       (frame_scale_gp * 0.1f));
            jet_flight_state->pitch = fast_add_rotations(
                jet_flight_state->pitch,
                fast_subtract_rotations(-fast_arc_tan(vector_length_xy(velocity), velocity[2]),
                                        jet_flight_state->pitch) *
                    (frame_scale_gp * 0.1f));
            jet_flight_state->speed = jet_flight_state->speed * 0.7f;
            if (*(char **)(hit + 0x18) != 0) {
                if (jet_flight_state->hit_cooldown == 0) {
                    player = (char *)D_0013E533 + 0xE1D;
                    *(float *)(player + 0x15FC) = *(float *)(player + 0x15FC) - 5.0f;
                    if (*(float *)(player + 0x15FC) < 0.0f) {
                        fast_vec_sub(cross, hit + 0x50, hit + 0x60);
                        fast_vec_normalize(cross, cross, 1.0f);
                        reflect_vector(impulse, velocity, hit + 0x40);
                        fast_vec_normalize(impulse, impulse, frame_time_gp + frame_time_gp);
                        fast_vec_normalize(vertical, hit + 0x40, 1.0f);
                        spawn_explosion(moby,
                                        impulse,
                                        0,
                                        8.0f,
                                        100.0f,
                                        0x1E,
                                        0xA,
                                        0x18,
                                        16.0f,
                                        8.0f,
                                        9.0f,
                                        6,
                                        1.0f,
                                        50.0f,
                                        1,
                                        1,
                                        -1,
                                        0);
                        jet_flight_state->death_timer = scale_game_frames(240);
                        moby->state = 6;
                    }
                    jet_flight_state->hit_cooldown = scale_game_frames(10);
                }
            } else {
                *(float *)(D_0013E533 + 0x2419) = 0.0f;
                spawn_explosion(moby,
                                impulse,
                                0,
                                8.0f,
                                100.0f,
                                0x1E,
                                0xA,
                                0x18,
                                16.0f,
                                8.0f,
                                9.0f,
                                6,
                                1.0f,
                                50.0f,
                                1,
                                1,
                                -1,
                                0);
                moby->flags |= 0x41;
                jet_flight_state->death_timer = scale_game_frames(240);
                moby->state = 6;
            }
        }
    }
    add_draw_callback(draw_fighter_jet_hud, moby);
    fire_fighter_jet_rocket((struct Moby *)moby, (char *)jet_flight_state);
    update_fighter_jet_gun_and_aim(
        (struct Moby *)moby,
        (char *)jet_flight_state,
        fast_add_rotations(jet_flight_state->camera_yaw, -jet_flight_state->steer_x / 7.0f),
        -fast_add_rotations(jet_flight_state->camera_pitch,
                            -(jet_flight_state->steer_y / 7.0f) - 0.1f));
}

typedef float JetVec[4] __attribute__((aligned(16)));

typedef struct Jet Jet;

typedef struct JetData JetData;

typedef struct {
    char  pad0[0x24];
    float scale;
} JetModel;

struct Jet {
    char           pad0[0x10];
    JetVec         position;
    unsigned char  state, pad21[3];
    JetModel      *model;
    float          pad28, scale;
    unsigned char  alpha, pad31;
    unsigned short render_flags, flags;
    char           pad36[0xA];
    JetVec         rotation;
    char           pad50[0x28];
    JetData       *data;
    char           pad7C[0x18];
    int            animation;
    char           pad98[0xC];
    unsigned char  status, padA5;
    unsigned short class_id;
    char           padA8[8];
    unsigned char  persistent;
};

struct JetData {
    JetVec        velocity, pad10, previous_rotation, angles, spawn_position, spawn_rotation;
    unsigned char animation_frame, gun_side;
    short         death_timer;
    float         speed;
    short         missile_index, gun_index;
    int           counter6C, counter70, pad74, pad78;
    float         blend;
    int           gun_timer, missile_timer;
    char          pad88[0x18];
    float         angle_a, angle_b, angle_c, limit_a, limit_b;
    char          padB4[0xC];
    int           counterC0, padC4;
    float         angle_d, angle_e, angle_f;
    char          padD4[8];
    int           timer;
    int           hud_x, hud_y, energy, padEC;
    float         hud_health;
    int           hud_previous, wait_timer;
    int           destination_path, escape_path, texture_id, engine_sound, weapon_sound;
    char          pad110[0xC];
    int group_a, hit_class, target_class, lock_index, exit_path, entry_path, trigger, start_timer,
        group_b;
};

typedef struct {
    char           pad0[0x15F0];
    Jet           *craft;
    unsigned short class_id;
    unsigned char  ammo, ammo2, ammo3, ammo4, ammo5, ammo6;
    float          health, max_health, health_scale;
    int            health_meter;
    short          weapon;
    unsigned char  pad160E, pad160F;
    char           pad1610[0xA70];
    Jet           *moby;
    int            state;
    char           pad2088[0x1C];
    unsigned char  busy, in_craft;
} JetPlayer;

typedef struct {
    char  pad0[0x160];
    float flash;
    int   pad164, ticks;
} JetCamera;

typedef struct {
    char           pad0[0xA];
    unsigned short palette;
    char           padC[4];
} JetTexture;

typedef struct {
    char   pad0[0x10];
    JetVec velocity;
    Jet   *source;
    int    pad24, pad28;
    float  damage;
    int    flags;
} JetHit;

extern JetCamera jet_camera47 __asm__("D_L13_00166F80");

#include "rnc/overlay/l13/mobys/fighter_jet_data/fighter_jet_update.h"

extern JetTexture l13_texture_descriptors[] __asm__("D_L13_0016D480");

extern unsigned int l13_texture_palette_data_base __asm__("D_L13_0015F420");

int hp_bar_saved __asm__("D_L13_001613C4") = 0;

extern short fighter_jet_hidden_moby_classes[] __asm__("D_L13_001CBDE8");

short D_L13_00161440 = 0x7f;

extern float D_L13_0015F3FC;

extern int D_L13_0015F608;

extern char D_L13_001F4470[], D_L13_001F44A8[];

extern void jet_group_state(int, int, int, int) __asm__("FUN_L13_002650a8");

extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");

extern void update_fighter_jet_flight(void *, void *) __asm__("FUN_L13_002ba178");

extern void jet_camera_mode(void) __asm__("FUN_L00_00203b18");

extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");

extern void jet_set_hero_state(int, int) __asm__("FUN_L00_002223f8");

extern void set_animation_parameter(int, int) __asm__("FUN_001ff570");

extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");

extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");

extern void force_help_message(int, int) __asm__("FUN_002151d8");

extern int try_set_help_message(int, int) __asm__("FUN_00215130");

void fighter_jet_update(Jet *m) __asm__("FUN_L13_002bb068");

/* Moby class 69 */
void fighter_jet_update(Jet *jet_moby) {
    enum {
        FIGHTER_JET_STATE_INITIALIZE = 0,
        FIGHTER_JET_STATE_WAIT_FOR_ENTRY = 1,
        FIGHTER_JET_STATE_MOUNT_PLAYER = 2,
        FIGHTER_JET_STATE_FLIGHT = 4,
        FIGHTER_JET_STATE_EXIT_PLAYER = 5,
        FIGHTER_JET_STATE_STOP_DEATH_SOUNDS = 6,
        FIGHTER_JET_STATE_WAIT_FOR_DEATH_RESET = 7,
        FIGHTER_JET_STATE_FORCED_EXIT = 8,
        FIGHTER_JET_STATE_DELETE = 9
    };
    JetData *jet_data;
    JetVec   impulse;
    int      state, initial_state;
    if (((JetPlayer *)(&hero))->state == 0x72)
        return;
    jet_data = jet_moby->data;
    jet_data->hit_class = 0;
    fast_dec_timer((int *)&jet_data->timer);
    initial_state = jet_moby->state;
    if ((unsigned int)(initial_state - 6) >= 2 && (unsigned char)initial_state != 1 &&
        (unsigned char)initial_state != 2) {
        JetHit *hit = (JetHit *)find_moby_hit(jet_moby, 0x30001, 0);
        if (hit && jet_moby->state == 4) {
            if (((JetPlayer *)(&hero))->health > hit->damage) {
                ((JetPlayer *)(&hero))->health -= hit->damage;
                if (hit->source && (unsigned int)(hit->source->class_id - 0x52) < 2)
                    jet_data->hit_class = (short)hit->source->class_id;
                if (hit->flags & 1) {
                    float weight, yaw_delta, pitch_delta;
                    jet_camera47.flash = 1.0f;
                    jet_camera47.ticks = scale_game_frames(4);
                    weight = hit->damage * 0.05f;
                    build_spherical_offset(
                        impulse, jet_data->speed, jet_data->angles[2], -jet_data->angles[1]);
                    clamp_vector_length(hit->velocity, hit->velocity, frame_time * 3.0f);
                    fast_vec_add(jet_moby->position, jet_moby->position, hit->velocity);
                    reflect_vector(impulse, impulse, hit->velocity);
                    yaw_delta = fast_subtract_rotations(fast_arc_tan(impulse[0], impulse[1]),
                                                        jet_data->angles[2]);
                    jet_data->angles[2] = fast_add_rotations(
                        jet_data->angles[2], yaw_delta * (fighter_jet_frame_scale * 0.1f) * weight);
                    pitch_delta = fast_subtract_rotations(
                        fast_arc_tan(vector_length_xy(impulse), impulse[2]), jet_data->angles[1]);
                    jet_data->angles[1] =
                        fast_add_rotations(jet_data->angles[1],
                                           pitch_delta * (fighter_jet_frame_scale * 0.1f) * weight);
                }
            } else {
                build_spherical_offset(
                    impulse, jet_data->speed, jet_data->angles[2], -jet_data->angles[1]);
                if (hit->source && (unsigned int)(hit->source->class_id - 0x52) < 2)
                    jet_data->hit_class = (short)hit->source->class_id + 10000;
                jet_camera47.flash = 0.2f;
                jet_camera47.ticks = scale_game_frames(5);
                ((JetPlayer *)(&hero))->health = 0.0f;
                jet_moby->flags |= 0x41;
                spawn_explosion(jet_moby,
                                impulse,
                                0,
                                0.0f,
                                0.0f,
                                10,
                                3,
                                16,
                                4.0f,
                                2.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                jet_moby->animation = 0;
                jet_data->death_timer = scale_game_frames(240);
                jet_moby->state = 6;
            }
        }
    }
    jet_moby->status = 0xFF;
    if (((JetPlayer *)(&hero))->craft == jet_moby)
        ((JetPlayer *)(&hero))->health_meter =
            (int)(((JetPlayer *)(&hero))->health * 0.00390625f * 200.0f);
    switch (jet_moby->state) {
    case FIGHTER_JET_STATE_INITIALIZE: {
        float        limit;
        int         *palette;
        JetTexture  *texture_table;
        unsigned int palette_data;
        int          texture_index, initialized;
        jet_moby->state = 1;
        jet_data->animation_frame = 0;
        jet_moby->scale = jet_moby->model->scale;
        *(float *)((char *)jet_data + 0x20) = jet_data->angles[0];
        *(float *)((char *)jet_data + 0x24) = jet_data->angles[1];
        limit = fast_arc_tan(1.0f, D_L13_0016CDF0);
        limit += limit;
        jet_data->counterC0 = 0;
        jet_data->hit_class = 0;
        *(float *)((char *)jet_data + 0xAC) = limit;
        *(float *)((char *)jet_data + 0xB0) = limit;
        qcopy(jet_data->angles, jet_moby->rotation);
        qcopy(jet_data->spawn_rotation, jet_moby->rotation);
        qcopy(jet_data->spawn_position, jet_moby->position);
        texture_index = jet_data->texture_id;
        texture_table = l13_texture_descriptors;
        jet_data->blend = 0.0f;
        texture_index += 40;
        palette_data = l13_texture_palette_data_base;
        initialized = hp_bar_saved;
        palette = (int *)(palette_data + (texture_table[texture_index].palette << 4));
        hp_bar_clut[0] = palette;
        if (!initialized)
            __builtin_memcpy(fighter_jet_health_palette_backup, palette, 1024);
        hp_bar_saved = 1;
        jet_data->wait_timer = 1;
        jet_data->energy = 1;
        if (jet_moby->persistent != 0xFF &&
            (visited52)[jet_moby->persistent + (persistence_table_row_index << 4)] == 0xFF)
            jet_moby->state = 9;
        if (jet_data->group_a > 0)
            jet_group_state(jet_data->group_a, 0, 0, 0);
        if (jet_data->group_b > 0)
            jet_group_state(jet_data->group_b, 0, 0, 0);
        jet_group_switch(fighter_jet_hidden_moby_classes, 1, 1, -1);
        jet_group_switch(&D_L13_00161440, 1, -1, -1);
        jet_group_flag(0x102, 0);
        jet_data->lock_index = -1;
        jet_data->start_timer = scale_game_frames(575);
        break;
    }
    case FIGHTER_JET_STATE_WAIT_FOR_ENTRY: {
        short enter = 0;
        if (jet_moby->persistent != 0xFF &&
            (visited52)[jet_moby->persistent + (persistence_table_row_index << 4)] == 0xFF)
            jet_moby->state = 9;
        else if (jet_data->trigger != -1 &&
                 is_point_inside_clip_volume(D_0013F3D0, jet_data->trigger)) {
            jet_data->blend = 0.99f;
            D_L13_0015F3FC = 0.99f;
            enter = 1;
        } else if (jet_distance(jet_moby, ((JetPlayer *)(&hero))->moby) < 2.0f) {
            int pressed = try_set_help_message(7, 0x53E4) != 0;
            if (*(int *)(D_0013A4E0 + 0x2604) & 0x10)
                enter = pressed;
        }
        if (enter) {
            float limit;
            jet_moby->scale = jet_moby->model->scale;
            *(float *)((char *)jet_data + 0x20) = jet_data->angles[0];
            *(float *)((char *)jet_data + 0x24) = jet_data->angles[1];
            limit = fast_arc_tan(1.0f, D_L13_0016CDF0);
            limit += limit;
            jet_data->counterC0 = 0;
            jet_data->hit_class = 0;
            *(float *)((char *)jet_data + 0xAC) = limit;
            *(float *)((char *)jet_data + 0xB0) = limit;
            qcopy(jet_data->angles, jet_moby->rotation);
            jet_data->wait_timer = 1;
            jet_data->energy = 1;
            jet_moby->state = 2;
            jet_data->lock_index = -1;
        } else if (jet_data->blend != 0.0f) {
            approach_value(&jet_data->blend, 0.0f, frame_time * 4.0f);
            D_L13_0015F3FC = jet_data->blend;
        }
        jet_animate((unsigned char *)jet_moby, 1.5f);
        break;
    }
    case FIGHTER_JET_STATE_MOUNT_PLAYER:
        approach_value(&jet_data->blend, 1.0f, frame_time * 4.0f);
        D_L13_0015F3FC = jet_data->blend;
        if (jet_data->blend == 1.0f) {
            if (jet_data->entry_path != -1) {
                LevelCuboid *path = &l13_cuboids[jet_data->entry_path];
                set_race_camera(&path->pos, &path->rot, 50, 1);
                {
                    int          index = jet_data->entry_path;
                    LevelCuboid *paths = l13_cuboids;
                    qcopy(jet_moby->position, &paths[index].pos);
                    qcopy(jet_moby->rotation, &paths[index].rot);
                }
            }
            jet_set_hero_state(0x32, 1);
            jet_mount(jet_moby->position, jet_moby->rotation, 0, 0, 0);
            jet_data->missile_index = 0;
            jet_data->counter6C = 0;
            jet_data->counter70 = 0;
            jet_data->missile_timer = 0;
            jet_data->speed = fighter_jet_frame_scale * 0.1f;
            jet_data->gun_timer = 0;
            jet_moby->alpha = jet_moby->render_flags = 0xFF;
            *(int *)((char *)((JetPlayer *)(&hero))->moby + 0x98) = -1;
            qcopy(jet_data->previous_rotation, jet_moby->rotation);
            jet_data->angle_a = 0;
            jet_data->angle_b = 0;
            jet_data->angle_c = 0;
            jet_data->angle_d = 0;
            jet_data->angle_e = 0;
            jet_data->angle_f = 0;
            ((JetPlayer *)(&hero))->craft = jet_moby;
            {
                unsigned short mounted_craft_class_id = jet_moby->class_id;
                ((JetPlayer *)(&hero))->health = 255.0f;
                ((JetPlayer *)(&hero))->ammo = 10;
                ((JetPlayer *)(&hero))->pad160E = 1;
                ((JetPlayer *)(&hero))->ammo2 = 20;
                ((JetPlayer *)(&hero))->class_id = mounted_craft_class_id;
                ((JetPlayer *)(&hero))->ammo3 = 20;
                jet_data->hud_previous = 255;
                jet_data->hud_health = 0;
                ((JetPlayer *)(&hero))->ammo4 = 3;
                ((JetPlayer *)(&hero))->ammo5 = 3;
                ((JetPlayer *)(&hero))->ammo6 = 3;
                ((JetPlayer *)(&hero))->pad160F = 0;
                ((JetPlayer *)(&hero))->max_health = 256.0f;
                ((JetPlayer *)(&hero))->health_scale = 100.0f;
            }
            build_spherical_offset(jet_data,
                                   fighter_jet_frame_scale * 0.1f,
                                   jet_data->angles[2],
                                   -jet_data->angles[1]);
            if (jet_data->group_a > 0)
                jet_group_state(jet_data->group_a, 1, 1, 1);
            if (jet_data->group_b > 0)
                jet_group_state(jet_data->group_b, 1, 1, 1);
            jet_group_switch(fighter_jet_hidden_moby_classes, 0, 0, -1);
            jet_group_switch(&D_L13_00161440, 0, -1, -1);
            jet_group_flag(0x102, 0);
            jet_moby->position[2] += 3.0f;
            jet_moby->scale = jet_moby->model->scale * 0.25f;
            ((JetPlayer *)(&hero))->in_craft = 1;
            if (jet_data->engine_sound == -1)
                jet_data->engine_sound = allocate_voice_for_target_entry(0, 4, (int)jet_moby);
            D_L13_0015F608 = 2;
            update_fighter_jet_flight(jet_moby, jet_data);
            jet_camera_mode();
            request_music_track(2, 4);
            jet_moby->state = 4;
        }
        break;
    case FIGHTER_JET_STATE_FLIGHT: {
        unsigned char *audio;
        if (jet_data->blend != 0.0f) {
            approach_value(&jet_data->blend, 0.0f, frame_time * 4.0f);
            D_L13_0015F3FC = jet_data->blend;
        }
        force_help_message(6, 0);
        D_L13_0015F608 = 2;
        audio = (unsigned char *)&voice_pool + jet_data->engine_sound * 0x70;
        if (*(Jet **)(audio + 0x88) != jet_moby || !audio[0x74]) {
            jet_data->engine_sound = -1;
            jet_data->engine_sound = allocate_voice_for_target_entry(0, 4, (int)jet_moby);
        }
        update_fighter_jet_flight(jet_moby, jet_data);
        if (((JetPlayer *)(&hero))->pad160F & 1)
            jet_moby->state = 5;
        if (jet_data->energy <= 0) {
            jet_data->wait_timer = scale_game_frames(1200);
            jet_moby->state = 8;
        }
        break;
    }
    case FIGHTER_JET_STATE_EXIT_PLAYER:
        ((JetPlayer *)(&hero))->in_craft = 0;
        *(int *)((char *)((JetPlayer *)(&hero))->moby + 0x98) = 0;
        jet_set_hero_state(0, 1);
        jet_end_mount(0);
        set_animation_parameter(((JetPlayer *)(&hero))->weapon, 0);
        qcopy(jet_moby->position, jet_data->spawn_position);
        qcopy(jet_moby->rotation, jet_data->spawn_rotation);
        jet_moby->scale = jet_moby->model->scale;
        refresh_moby_spatial_bounds_from_basis(jet_moby);
        if (jet_data->group_a > 0)
            jet_group_state(jet_data->group_a, 0, 0, 0);
        if (jet_data->group_b > 0)
            jet_group_state(jet_data->group_b, 0, 0, 0);
        jet_group_switch(fighter_jet_hidden_moby_classes, 1, 1, -1);
        jet_group_switch(&D_L13_00161440, 1, -1, -1);
        jet_group_flag(0x102, 0);
        if (jet_data->exit_path != -1) {
            LevelCuboid *path = &l13_cuboids[jet_data->exit_path];
            set_race_camera(&path->pos, &path->rot, 0, 1);
        } else
            set_race_camera(
                jet_data->spawn_position, &l13_cuboids[jet_data->destination_path].rot, 0, 1);
        {
            int            id = jet_data->engine_sound;
            unsigned char *audio;
            if (id != -1) {
                audio = (unsigned char *)&voice_pool + id * 0x70;
                if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                    release_voice_slot(id);
            }
            jet_data->engine_sound = -1;
            id = jet_data->weapon_sound;
            if (id != -1) {
                audio = (unsigned char *)&voice_pool + id * 0x70;
                if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                    release_voice_slot(id);
            }
            jet_data->weapon_sound = -1;
        }
        request_music_track(0, 5);
        jet_data->blend = 0.99f;
        D_L13_0015F3FC = 0.99f;
        jet_moby->state = 1;
        break;
    case FIGHTER_JET_STATE_STOP_DEATH_SOUNDS:
        D_L13_0015F608 = 2;
        {
            int            id = jet_data->engine_sound;
            unsigned char *audio;
            if (id != -1) {
                audio = (unsigned char *)&voice_pool + id * 0x70;
                if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                    release_voice_slot(id);
            }
            jet_data->engine_sound = -1;
            id = jet_data->weapon_sound;
            if (id != -1) {
                audio = (unsigned char *)&voice_pool + id * 0x70;
                if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                    release_voice_slot(id);
            }
            jet_data->weapon_sound = -1;
        }
        jet_moby->state = 7;
        break;
    case FIGHTER_JET_STATE_WAIT_FOR_DEATH_RESET: {
        struct MusicStreamState *transition;
        D_L13_0015F608 = 2;
        if (fast_dec_timer_s16(&jet_data->death_timer)) {
            transition = &music_stream_state;
            if (!transition->secondary.handle) {
                int active = transition->queued_secondary_track;
                if (active == -1) {
                    hero_respawn();
                    jet_end_mount(0);
                    qcopy(jet_moby->position, jet_data->spawn_position);
                    qcopy(jet_moby->rotation, jet_data->spawn_rotation);
                    refresh_moby_spatial_bounds_from_basis(jet_moby);
                    {
                        int            id = jet_data->engine_sound;
                        unsigned char *audio;
                        if (id != active) {
                            audio = (unsigned char *)&voice_pool + id * 0x70;
                            if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                                release_voice_slot(id);
                        }
                        jet_data->engine_sound = -1;
                        id = jet_data->weapon_sound;
                        if (id != -1) {
                            audio = (unsigned char *)&voice_pool + id * 0x70;
                            if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                                release_voice_slot(id);
                        }
                        jet_data->weapon_sound = -1;
                    }
                    jet_moby->state = 0;
                }
            }
        }
        break;
    }
    case FIGHTER_JET_STATE_FORCED_EXIT:
        ((JetPlayer *)(&hero))->in_craft = 0;
        *(int *)((char *)((JetPlayer *)(&hero))->moby + 0x98) = 0;
        jet_set_hero_state(0, 1);
        jet_end_mount(0);
        if (jet_data->escape_path == -1) {
            debug_print(D_L13_001F4470);
            qcopy(jet_moby->position, jet_data->spawn_position);
            qcopy(jet_moby->rotation, jet_data->spawn_rotation);
        } else {
            int          index = jet_data->escape_path;
            LevelCuboid *paths = l13_cuboids;
            qcopy(jet_moby->position, &paths[index].pos);
            qcopy(jet_moby->rotation, &paths[index].rot);
            jet_moby->position[2] += 0.57318f;
        }
        jet_moby->scale = jet_moby->model->scale;
        refresh_moby_spatial_bounds_from_basis(jet_moby);
        if (jet_data->destination_path == -1)
            debug_print(D_L13_001F44A8);
        else {
            LevelCuboid *path;
            if (jet_data->group_a > 0)
                jet_group_state(jet_data->group_a, 0, 0, 0);
            if (jet_data->group_b > 0)
                jet_group_state(jet_data->group_b, 0, 0, 0);
            jet_group_switch(fighter_jet_hidden_moby_classes, 1, 1, -1);
            jet_group_switch(&D_L13_00161440, 1, -1, -1);
            jet_group_flag(0x102, 0);
            path = &l13_cuboids[jet_data->destination_path];
            set_race_camera(&path->pos, &path->rot, 0, 1);
        }
        if (jet_moby->persistent != 0xFF)
            jet_clear_persistent(jet_moby->persistent);
        {
            int            id = jet_data->engine_sound;
            unsigned char *audio;
            if (id != -1) {
                audio = (unsigned char *)&voice_pool + id * 0x70;
                if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                    release_voice_slot(id);
            }
            jet_data->engine_sound = -1;
            id = jet_data->weapon_sound;
            if (id != -1) {
                audio = (unsigned char *)&voice_pool + id * 0x70;
                if (*(Jet **)(audio + 0x88) == jet_moby && audio[0x74])
                    release_voice_slot(id);
            }
            jet_data->weapon_sound = -1;
        }
        jet_reset_camera((char *)&hero + 0x80, (char *)&hero + 0x90);
        jet_moby->state = 9;
        break;
    case FIGHTER_JET_STATE_DELETE:
        mark_moby_for_removal(jet_moby);
        return;
    case 10:
        break;
    default:
        break;
    }
    if (jet_moby->position[0] < 15.0f)
        jet_moby->position[0] = 15.0f;
    if (jet_moby->position[0] > 1008.0f)
        jet_moby->position[0] = 1008.0f;
    if (jet_moby->position[1] < 15.0f)
        jet_moby->position[1] = 15.0f;
    if (jet_moby->position[1] > 1008.0f)
        jet_moby->position[1] = 1008.0f;
    if (jet_moby->position[2] < 15.0f)
        jet_moby->position[2] = 15.0f;
    if (jet_moby->position[2] > 1008.0f)
        jet_moby->position[2] = 1008.0f;
    state = jet_moby->state;
    if ((unsigned char)state == 4) {
        qcopy_nc((char *)&hero + 0x80, jet_moby->position);
        qcopy_nc((char *)&hero + 0x90, jet_data->angles);
    }
    if ((unsigned int)(state - 3) < 2 || (unsigned char)state == 8)
        spawn_fighter_jet_joint_particles(jet_moby, (char *)jet_data);
}
