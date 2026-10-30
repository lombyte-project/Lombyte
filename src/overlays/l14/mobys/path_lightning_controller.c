/* Level 14 path lightning controller moby class and its branched lightning arcs. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/math/random.h"
#include "sda.h"

extern char D_L14_001674C0[];

extern s64 get_effect_texture(s32) __asm__("func_001F44B8");

extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");

extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

#include "rnc/overlay/l14/mobys/path_lightning_controller_globals.h"

extern float FUN_L00_00257c48(float, float);

extern char D_0013D24D[];

extern float D_L14_00161CC0[] MACRO_ADDR;

extern float D_L14_00161CD0[];

extern int D_L14_0015F5C4;

extern short lightning_arc_interval_remaining_frames_gp __asm__("D_L14_00161C50")
    __attribute__((sda));

extern short lightning_arc_interval_frames_gp __asm__("D_L14_00161C54") __attribute__((sda));

extern short lightning_arc_initialized_gp __asm__("D_L14_00161C58") __attribute__((sda));

extern short lightning_displaced_endpoint_z_setting_gp __asm__("D_L14_00161C5C")
    __attribute__((sda));

extern short lightning_visibility_test_point_w_gp __asm__("D_L14_00161C60") __attribute__((sda));

extern short lightning_visibility_test_z_offset_gp __asm__("D_L14_00161C64") __attribute__((sda));

extern void deactivate_recorded_moby(void) __asm__("FUN_L00_0028e990");

extern void start_scene(int) __asm__("FUN_L00_00298840");

extern void show_tracked_moby(void) __asm__("FUN_L01_002a2480");

extern void update_branched_lightning_arc(char *) __asm__("FUN_L14_002ed530");

extern float D_L14_00161CD0_f __asm__("D_L14_00161CD0");

void update_path_lightning_controller(struct Moby *moby) __asm__("FUN_L14_002ed280");

void update_path_lightning_controller(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int   visible_path_point_indices[5];
    float visibility_test_point[4];
    int   moby_state;
    if (MOBY(moby)->state == 0) {
        if (current_level_index == 0xE)
            hero.pack_disabled = 1;
        MOBY(moby)->state = 1;
        moby->update_dist = 0xFF;
        *(int *)&lightning_arc_initialized_gp = 0;
        *(int *)&lightning_arc_interval_remaining_frames_gp = 0;
    }
    moby_state = MOBY(moby)->state;
    if (moby_state == 1) {
        unsigned char *scene_start_guard_base;
        if (D_L14_0015F5C4 != 0)
            return;
        scene_start_guard_base = (unsigned char *)D_0013D24D + 0x13B;
        if (scene_start_guard_base[0x68] != 0)
            return;
        start_scene(0);
        scene_start_guard_base[0x68] = moby_state;
        MOBY(moby)->state = 2;
        deactivate_recorded_moby();
        return;
    }
    if (moby_state == 2) {
        if (D_L14_0015F5C4 == moby_state)
            return;
        MOBY(moby)->state = 3;
        show_tracked_moby();
        return;
    }
    if (fast_dec_timer(&lightning_arc_interval_remaining_frames_gp) == 0)
        return;
    if (*(int *)&lightning_arc_initialized_gp == 0) {
        int *path = ((int *)l14_paths[*(int *)moby_vars]);
        int  i;
        int  visible_point_count = 0;
        for (i = 0; i < path[0] && visible_point_count < 5; i++) {
            qcopy(visibility_test_point, (char *)path + i * 16 + 0x10);
            visibility_test_point[2] += *(float *)&lightning_visibility_test_z_offset_gp;
            visibility_test_point[3] = *(float *)&lightning_visibility_test_point_w_gp;
            if (fast_bsphere_check(visibility_test_point, 255.0f) > 0) {
                visible_path_point_indices[visible_point_count++] = i;
            }
        }
        if (visible_point_count > 0) {
            int k = random_integer_below(visible_point_count);
            qcopy(D_L14_00161CC0, (char *)path + visible_path_point_indices[k] * 16 + 0x10);
            lightning_path_endpoint_w = 1.0f;
            qcopy(D_L14_00161CD0, D_L14_00161CC0);
            lightning_displaced_endpoint_z = *(float *)&lightning_displaced_endpoint_z_setting_gp;
            D_L14_00161CD0_f += FUN_L00_00257c48(0.0f, 20.0f);
            lightning_displaced_endpoint_y += FUN_L00_00257c48(0.0f, 20.0f);
            qcopy(&moby->pos, D_L14_00161CC0);
        }
    }
    update_branched_lightning_arc(moby);
    if (lightning_arc_remaining_frames == 0) {
        *(int *)&lightning_arc_interval_remaining_frames_gp =
            scale_game_frames(*(int *)&lightning_arc_interval_frames_gp);
        *(int *)&lightning_arc_initialized_gp = 0;
    }
}

extern f32 lightning_arc_alpha_pulse_angle_step_gp __asm__("D_L14_00161C68") __attribute__((sda));

extern f32 lightning_arc_alpha_pulse_sweep_gp __asm__("D_L14_00161C6C") __attribute__((sda));

extern s32 D_L14_00161C70_gp __asm__("D_L14_00161C70") __attribute__((sda));

extern f32 lightning_main_arc_point_spacing_gp __asm__("D_L14_00161C74") __attribute__((sda));

#include "rnc/overlay/l14/mobys/path_lightning_controller_data/update_branched_lightning_arc.h"

extern f32 lightning_main_arc_z_displacement_limit_gp __asm__("D_L14_00161C94")
    __attribute__((sda));

extern s32 lightning_arc_lifetime_range_start_gp __asm__("D_L14_00161C98") __attribute__((sda));

extern s32 lightning_arc_lifetime_range_end_gp __asm__("D_L14_00161C9C") __attribute__((sda));

extern u8 lightning_arc_alpha_fade __asm__("D_L14_00161CE0");

extern char *lightning_arc_owner_moby __asm__("D_L14_00161CE8");

extern s32 D_L14_00161CF8_macro[] __asm__("D_L14_00161CF8") MACRO_ADDR;

f32 lightning_main_arc_points[40][4] __asm__("D_L14_001DFD80") = {0};

extern s16 D_L14_001E0000[40];

f32 D_L14_001E0050_d530[3][16][4] __asm__("D_L14_001E0050") = {0};

u16 D_L14_001E0350_d530[3][16] __asm__("D_L14_001E0350") = {0};

extern int arc_ftoi(float) __asm__("FUN_001fa6d0");

extern void allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");

void spawn_lightning_arc_particle_pairs(float *particle_spawn_position,
                                        int    spawn_pair_count) __asm__("FUN_L14_002ee6b0");

void spawn_lightning_arc_particle_pairs(float *pos, int n) __asm__("FUN_L14_002ee6b0");

void draw_branched_lightning_arc() __asm__("FUN_L14_002edc18");

extern float D_L14_00161CD0_macro[] __asm__("D_L14_00161CD0") MACRO_ADDR;

extern float lightning_main_arc_penultimate_point[] __asm__("D_L14_001DFFE0");

void update_branched_lightning_arc(char *moby) {
    Vec4   delta;
    float  arc_endpoint_distance;
    float  path_fraction;
    float  sign;
    float  branch_segment_angle;
    float  r;
    int    i;
    int    j;
    int    branch_age_index;
    float *main_arc_point;
    s32   *starts;
    u16   *main_arc_age;

    if (*(int *)&lightning_arc_initialized_gp == 0) {
        *(int *)&lightning_arc_initialized_gp = 1;
        lightning_arc_sound_pending = 1;
        lightning_arc_owner_moby = moby;
        i = 0;
        starts = D_L14_00161CF8_macro;
        *(int *)&lightning_arc_alpha_pulse_sweep_gp = 0;
        fast_vec_sub(&delta, D_L14_00161CD0_macro, D_L14_00161CC0);
        arc_endpoint_distance = fast_vec_length(&delta);
        lightning_main_arc_point_spacing_gp = arc_endpoint_distance / convert_integer_to_float(40);
        lightning_arc_alpha_fade = 0;
        for (; i < 40; i++) {
            main_arc_age = &D_L14_001E0000[i];
            main_arc_point = lightning_main_arc_points[i];
            main_arc_point[1] = main_arc_point[2] = 0.0f;
            main_arc_point[3] = 1.0f;
            main_arc_point[0] = lightning_main_arc_point_spacing_gp * (float)i;
            if (i < 39) {
                main_arc_point[2] =
                    main_arc_point[-2] + FUN_L00_00257c48(lightning_main_arc_z_step_range_start_gp,
                                                          lightning_main_arc_z_step_range_end_gp);
                if (lightning_main_arc_z_displacement_limit_gp < (*&main_arc_point)[2]) {
                    main_arc_point[2] = lightning_main_arc_z_displacement_limit_gp;
                } else if (main_arc_point[2] < -lightning_main_arc_z_displacement_limit_gp) {
                    main_arc_point[2] = -lightning_main_arc_z_displacement_limit_gp;
                }
            }
            path_fraction = convert_integer_to_float(i) / convert_integer_to_float(40);
            *main_arc_age =
                arc_ftoi((float)-scale_game_frames(D_L14_00161C70_gp) * path_fraction + 0.0f);
        }
        lerp_vector(lightning_main_arc_penultimate_point,
                    lightning_main_arc_penultimate_point - 4,
                    lightning_main_arc_penultimate_point + 4,
                    0.5f);
        sign = -1.0f;
        lightning_arc_remaining_frames = lightning_arc_initial_lifetime_frames =
            scale_game_frames(random_int_between(lightning_arc_lifetime_range_start_gp,
                                                 lightning_arc_lifetime_range_end_gp));
        lightning_arc_inverse_fade_duration =
            1.0f / convert_integer_to_float(lightning_arc_remaining_frames -
                                            scale_game_frames(D_L14_00161C70_gp));
        if (random_integer_below(2) != 0) {
            sign = 1.0f;
        }
        for (j = 0; j < 3; j++) {
            int k = random_int_between(5, 22);
            int branch_point_index;
            float(*row)[4] = D_L14_001E0050_d530[j];
            u16 *sq = D_L14_001E0350_d530[j];
            starts[j] = k;
            sq[0] = D_L14_001E0000[k];
            row[0][0] = 0.0f;
            row[0][2] = 0.0f;
            r = random_float_between(1.0f, 10.0f);
            branch_segment_angle = random_float_between(0.5235988f, 1.3962634f) * sign;
            row[0][0] = r * fast_cos(branch_segment_angle);
            row[0][2] = r * fast_sin(branch_segment_angle);
            for (branch_point_index = 1; branch_point_index < 16; branch_point_index++) {
                float angle_step;
                float na;
                r = random_float_between(1.0f, 10.0f);
                angle_step = FUN_L00_00257c48(0.0f, 40.0f) * 0.017453292f;
                na = fast_add_rotations(branch_segment_angle, angle_step);
                if (absolute_float(na) > 1.3962634f || (branch_segment_angle > 0.0f && na < 0.0f) ||
                    (branch_segment_angle < 0.0f && na > 0.0f)) {
                    branch_segment_angle = fast_add_rotations(branch_segment_angle, -angle_step);
                } else {
                    branch_segment_angle = na;
                }
                D_L14_001E0050_d530[j][branch_point_index][0] = r * fast_cos(branch_segment_angle);
                D_L14_001E0050_d530[j][branch_point_index][2] = r * fast_sin(branch_segment_angle);
                D_L14_001E0350_d530[j][branch_point_index] =
                    D_L14_001E0000[starts[j] + branch_point_index];
            }
            sign = -sign;
        }
    }
    if (moby == lightning_arc_owner_moby) {
        fast_dec_timer(&lightning_arc_remaining_frames);
        for (i = 0; i < 40; i++) {
            s16 v = ++D_L14_001E0000[i];
            if (scale_game_frames(D_L14_00161C70_gp) < v) {
                D_L14_001E0000[i] = scale_game_frames(D_L14_00161C70_gp);
            }
            if (i != 0) {
                if (i >= 35) {
                    float  k5 = 5.0f;
                    float *pp = lightning_main_arc_points[i];
                    pp[2] = pp[2] + (0.0f - pp[2]) * (convert_integer_to_float(i - 35) / k5);
                }
            }
        }
        scale_game_frames(D_L14_00161C70_gp);
        for (j = 0; j < 3; j++) {
            for (branch_age_index = 0; branch_age_index < 16; branch_age_index++) {
                s16 v = ++D_L14_001E0350_d530[j][branch_age_index];
                if (scale_game_frames(D_L14_00161C70_gp) < v) {
                    D_L14_001E0350_d530[j][branch_age_index] = scale_game_frames(D_L14_00161C70_gp);
                }
            }
        }
        if (lightning_arc_remaining_frames <
            lightning_arc_initial_lifetime_frames - scale_game_frames(D_L14_00161C70_gp)) {
            if (scale_game_frames(60) < lightning_arc_initial_lifetime_frames) {
                if (lightning_arc_alpha_pulse_sweep_gp < 6.2831855f) {
                    lightning_arc_alpha_pulse_offset =
                        arc_ftoi(fast_sin(lightning_arc_alpha_pulse_phase) * 128.0f);
                    lightning_arc_alpha_pulse_phase = fast_add_rotations(
                        lightning_arc_alpha_pulse_phase, lightning_arc_alpha_pulse_angle_step_gp);
                    lightning_arc_alpha_pulse_sweep_gp +=
                        lightning_arc_alpha_pulse_angle_step_gp * frame_scale;
                } else {
                    lightning_arc_alpha_pulse_offset = 0;
                }
            } else {
                lightning_arc_alpha_pulse_offset = 0;
            }
            lightning_arc_alpha_fade =
                (u8)lightning_arc_alpha_pulse_offset +
                arc_ftoi(convert_integer_to_float(lightning_arc_remaining_frames) *
                             lightning_arc_inverse_fade_duration * -255.0f +
                         255.0f);
            if (lightning_arc_sound_pending != 0) {
                allocate_voice_for_group_entry(0, 0, moby);
                lightning_arc_sound_pending = 0;
            }
            if (lightning_arc_alpha_fade < 50) {
                spawn_lightning_arc_particle_pairs(D_L14_00161CC0, 3);
            }
        } else {
            lightning_arc_alpha_fade = 0;
        }
        add_draw_callback(draw_branched_lightning_arc, moby);
    }
}

typedef int u128_2EF078 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2EF078;

typedef struct {
    V_2EF078 v[4];
    int      col[4];
    float    uv[4][2];
    long     r70;
    long     tex;
    long     r80;
    long     r88;
} Q_2EF078;

extern V_2EF078 D_L14_001DFD80[];

extern V_2EF078 D_L14_001E0050[];

extern float D_L14_00161CD0_c[] __asm__("D_L14_00161CD0") MACRO_ADDR;

extern int D_L14_00161C70_gp __asm__("D_L14_00161C70") __attribute__((sda));

#include "rnc/overlay/l14/mobys/path_lightning_controller_data/draw_branched_lightning_arc.h"

extern float D_L14_00161C88 __attribute__((sda));

extern int D_L14_00161CF8_macro[] __asm__("D_L14_00161CF8") MACRO_ADDR;

extern s16 D_L14_001E0000[];

extern short D_L14_001E0350[];

extern unsigned char D_L14_00161CE0[] MACRO_ADDR;

extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void draw_branched_lightning_arc(void) {
    Q_2EF078  layer_quads[2];
    V_2EF078  arc_to_world[4];
    V_2EF078  sz[2];
    V_2EF078  world_arc_points[2];
    V_2EF078  quad_to_world[4];
    V_2EF078  branch_position_scratch[3];
    int       outer_vertex_alpha;
    int       i;
    int       j;
    V_2EF078 *branch_offset_cursor;
    short    *branch_age_cursor;
    int       outer_vertex_color;
    V_2EF078 *nd;

    layer_quads[0].tex = get_effect_texture(0x2D);
    layer_quads[0].r88 = 0x8000000048L;
    layer_quads[0].r80 = 0xFF9000000260L;
    layer_quads[0].r70 = 0;
    outer_vertex_alpha =
        func_001FA898_r(convert_integer_to_float(D_L14_001E0000[0]) /
                        convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp)) * 255.0f) -
        D_L14_00161CE0[0];
    if (outer_vertex_alpha < 0)
        outer_vertex_alpha = 0;
    if (outer_vertex_alpha > 0xFF)
        outer_vertex_alpha = 0xFF;
    outer_vertex_color = lightning_arc_outer_base_color_gp | (outer_vertex_alpha << 24);
    layer_quads[0].col[1] = outer_vertex_color;
    layer_quads[0].col[0] = outer_vertex_color;
    layer_quads[0].uv[0][0] = 0.0f;
    layer_quads[0].uv[0][1] = 0.0f;
    layer_quads[0].uv[1][0] = 0.0f;
    layer_quads[0].uv[1][1] = 1.0f;
    layer_quads[0].uv[2][0] = 1.0f;
    layer_quads[0].uv[2][1] = 0.0f;
    layer_quads[0].uv[3][0] = 1.0f;
    layer_quads[0].uv[3][1] = 1.0f;
    fast_mem_copy(&layer_quads[1], &layer_quads[0], 0x90);
    layer_quads[1].tex = get_effect_texture(0x2C);
    layer_quads[1].col[0] = layer_quads[1].col[1] =
        lightning_arc_inner_base_color_gp | ((outer_vertex_alpha >> 1) << 24);
    fast_vec_sub(&arc_to_world[0], D_L14_00161CC0, D_L14_00161CD0_c);
    fast_vec_normalize(&arc_to_world[0], &arc_to_world[0], 1.0f);
    arc_to_world[0].w = 0.0f;
    arc_to_world[2].x = 1.0f;
    arc_to_world[2].y = 0.0f;
    arc_to_world[2].z = 0.0f;
    arc_to_world[2].w = 0.0f;
    fast_vec_cross(&arc_to_world[1], &arc_to_world[0], &arc_to_world[2]);
    fast_vec_normalize(&arc_to_world[1], &arc_to_world[1], 1.0f);
    arc_to_world[1].w = 0.0f;
    fast_vec_cross(&arc_to_world[2], &arc_to_world[1], &arc_to_world[0]);
    arc_to_world[2].w = 0.0f;
    qcopy(&arc_to_world[3], D_L14_00161CD0_c);
    *(u128_2EF078 *)&sz[0] = 0;
    sz[0].z = lightning_arc_outer_half_width_start_gp;
    sz[0].w = 1.0f;
    *(u128_2EF078 *)&sz[1] = 0;
    sz[1].z = lightning_arc_outer_half_width_start_gp * D_L14_00161C88;
    sz[1].w = 1.0f;
    qcopy(&layer_quads[0].v[0], &sz[0]);
    fast_vec_scale(&layer_quads[0].v[1], &layer_quads[0].v[0], -1.0f);
    qcopy_nc(&layer_quads[1].v[0], &sz[1]);
    fast_vec_scale(&layer_quads[1].v[1], &layer_quads[1].v[0], -1.0f);
    nd = &D_L14_001DFD80[1];
    transform_vector(&world_arc_points[0], nd, &arc_to_world[0]);
    transform_vector(&world_arc_points[1], nd + 1, &arc_to_world[0]);
    qcopy(&quad_to_world[0], &arc_to_world[0]);
    fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &world_arc_points[0]);
    fast_vec_normalize(&quad_to_world[1], &quad_to_world[1], 1.0f);
    fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
    qcopy(&quad_to_world[3], &world_arc_points[0]);
    transform_vector(&layer_quads[0].v[0], &layer_quads[0].v[0], &quad_to_world[0]);
    transform_vector(&layer_quads[0].v[1], &layer_quads[0].v[1], &quad_to_world[0]);
    transform_vector(&layer_quads[1].v[0], &layer_quads[1].v[0], &quad_to_world[0]);
    transform_vector(&layer_quads[1].v[1], &layer_quads[1].v[1], &quad_to_world[0]);
    for (i = 1; i < 0x28; i++) {
        int    main_arc_end_vertex_alpha;
        float *arc_endpoint = D_L14_00161CC0;
        qcopy(&layer_quads[0].v[2], &sz[0]);
        fast_vec_scale(&layer_quads[0].v[3], &layer_quads[0].v[2], -1.0f);
        qcopy(&layer_quads[1].v[2], &sz[1]);
        fast_vec_scale(&layer_quads[1].v[3], &layer_quads[1].v[2], -1.0f);
        if (i == 0x27) {
            qcopy(&world_arc_points[0], arc_endpoint);
        } else {
            transform_vector(&world_arc_points[0], &D_L14_001DFD80[i], &arc_to_world[0]);
        }
        qcopy(&quad_to_world[0], &arc_to_world[0]);
        fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &world_arc_points[0]);
        fast_vec_normalize(&quad_to_world[1], &quad_to_world[1], 1.0f);
        fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
        qcopy(&quad_to_world[3], &world_arc_points[0]);
        transform_vector(&layer_quads[0].v[2], &layer_quads[0].v[2], &quad_to_world[0]);
        transform_vector(&layer_quads[0].v[3], &layer_quads[0].v[3], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[2], &layer_quads[1].v[2], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[3], &layer_quads[1].v[3], &quad_to_world[0]);
        if (i == 0x27) {
            main_arc_end_vertex_alpha = 0;
        } else {
            main_arc_end_vertex_alpha =
                func_001FA898_r(convert_integer_to_float(D_L14_001E0000[i]) /
                                convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp)) *
                                255.0f) -
                D_L14_00161CE0[0];
            if (main_arc_end_vertex_alpha < 0)
                main_arc_end_vertex_alpha = 0;
            if (main_arc_end_vertex_alpha > 0xFF)
                main_arc_end_vertex_alpha = 0xFF;
        }
        layer_quads[0].col[2] = layer_quads[0].col[3] =
            lightning_arc_outer_base_color_gp | (main_arc_end_vertex_alpha << 24);
        layer_quads[1].col[2] = layer_quads[1].col[3] =
            lightning_arc_inner_base_color_gp | ((main_arc_end_vertex_alpha >> 1) << 24);
        draw_geometry_quad(&layer_quads[0], 0, 0);
        draw_geometry_quad(&layer_quads[1], 0, 0);
        layer_quads[0].col[0] = layer_quads[0].col[1] = layer_quads[0].col[2];
        layer_quads[1].col[0] = layer_quads[1].col[1] = layer_quads[1].col[2];
        qcopy(&layer_quads[0].v[0], &layer_quads[0].v[2]);
        qcopy(&layer_quads[0].v[1], &layer_quads[0].v[3]);
        qcopy(&layer_quads[1].v[0], &layer_quads[1].v[2]);
        qcopy(&layer_quads[1].v[1], &layer_quads[1].v[3]);
        sz[0].z =
            lightning_arc_outer_half_width_start_gp +
            (lightning_arc_outer_half_width_end_gp - lightning_arc_outer_half_width_start_gp) *
                cubic_hermite_scalar(-1.0f,
                                     0.0f,
                                     1.0f,
                                     0.0f,
                                     convert_integer_to_float(i) / convert_integer_to_float(0x28));
        sz[1].z = sz[0].z * D_L14_00161C88;
    }
    qcopy(&quad_to_world[0], &arc_to_world[0]);
    qcopy(&quad_to_world[1], &arc_to_world[1]);
    qcopy(&quad_to_world[2], &arc_to_world[2]);
    for (j = 0; j < 3; j++) {
        float     branch_age_fraction;
        short    *as = D_L14_001E0350;
        V_2EF078 *os = D_L14_001E0050;
        V_2EF078 *branch_offsets;

        branch_age_fraction = convert_integer_to_float(as[j * 16]) /
                              convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp));
        outer_vertex_alpha = func_001FA898_r(branch_age_fraction * 255.0f) - D_L14_00161CE0[0];
        if (outer_vertex_alpha < 0)
            outer_vertex_alpha = 0;
        if (outer_vertex_alpha > 0xFF)
            outer_vertex_alpha = 0xFF;
        outer_vertex_color = lightning_arc_outer_base_color_gp | (outer_vertex_alpha << 24);
        layer_quads[0].col[1] = outer_vertex_color;
        layer_quads[0].col[0] = outer_vertex_color;
        layer_quads[1].col[0] = layer_quads[1].col[1] =
            lightning_arc_inner_base_color_gp | ((outer_vertex_alpha >> 1) << 24);
        sz[0].z =
            (lightning_arc_outer_half_width_start_gp +
             (lightning_arc_outer_half_width_end_gp - lightning_arc_outer_half_width_start_gp) *
                 cubic_hermite_scalar(-1.0f,
                                      0.0f,
                                      1.0f,
                                      0.0f,
                                      convert_integer_to_float(D_L14_00161CF8_macro[j]) /
                                          convert_integer_to_float(0x28))) *
            0.75f;
        sz[1].z = sz[0].z * 4.0f;
        qcopy(&layer_quads[0].v[0], &sz[0]);
        fast_vec_scale(&layer_quads[0].v[1], &layer_quads[0].v[0], -1.0f);
        qcopy(&layer_quads[1].v[0], &sz[1]);
        fast_vec_scale(&layer_quads[1].v[1], &layer_quads[1].v[0], -1.0f);
        qcopy(&branch_position_scratch[0], &D_L14_001DFD80[D_L14_00161CF8_macro[j]]);
        transform_vector(
            &branch_position_scratch[1], &branch_position_scratch[0], &arc_to_world[0]);
        branch_offsets = &os[j * 16];
        branch_position_scratch[0].x += branch_offsets->x;
        branch_position_scratch[0].z += branch_offsets->z;
        transform_vector(
            &branch_position_scratch[2], &branch_position_scratch[0], &arc_to_world[0]);
        qcopy(&quad_to_world[3], &branch_position_scratch[1]);
        transform_vector(&layer_quads[0].v[0], &layer_quads[0].v[0], &quad_to_world[0]);
        transform_vector(&layer_quads[0].v[1], &layer_quads[0].v[1], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[0], &layer_quads[1].v[0], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[1], &layer_quads[1].v[1], &quad_to_world[0]);
        branch_offset_cursor = branch_offsets + 1;
        branch_age_cursor = &as[j * 16 + 1];
        for (i = 1; i < 16; i++) {
            qcopy(&layer_quads[0].v[2], &sz[0]);
            fast_vec_scale(&layer_quads[0].v[3], &layer_quads[0].v[2], -1.0f);
            qcopy(&layer_quads[1].v[2], &sz[1]);
            fast_vec_scale(&layer_quads[1].v[3], &layer_quads[1].v[2], -1.0f);
            qcopy_nc(&branch_position_scratch[1], &branch_position_scratch[2]);
            branch_position_scratch[0].x += branch_offset_cursor->x;
            branch_position_scratch[0].z += branch_offset_cursor->z;
            transform_vector(
                &branch_position_scratch[2], &branch_position_scratch[0], &arc_to_world[0]);
            qcopy(&quad_to_world[3], &branch_position_scratch[1]);
            transform_vector(&layer_quads[0].v[2], &layer_quads[0].v[2], &quad_to_world[0]);
            transform_vector(&layer_quads[0].v[3], &layer_quads[0].v[3], &quad_to_world[0]);
            transform_vector(&layer_quads[1].v[2], &layer_quads[1].v[2], &quad_to_world[0]);
            transform_vector(&layer_quads[1].v[3], &layer_quads[1].v[3], &quad_to_world[0]);
            branch_age_fraction = convert_integer_to_float(*branch_age_cursor) /
                                  convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp));
            outer_vertex_alpha = func_001FA898_r(branch_age_fraction * 255.0f) - D_L14_00161CE0[0];
            if (outer_vertex_alpha < 0)
                outer_vertex_alpha = 0;
            if (outer_vertex_alpha > 0xFF)
                outer_vertex_alpha = 0xFF;
            outer_vertex_color = lightning_arc_outer_base_color_gp | (outer_vertex_alpha << 24);
            layer_quads[0].col[3] = outer_vertex_color;
            layer_quads[0].col[2] = outer_vertex_color;
            layer_quads[1].col[2] = layer_quads[1].col[3] =
                lightning_arc_inner_base_color_gp | ((outer_vertex_alpha >> 1) << 24);
            draw_geometry_quad(&layer_quads[0], 0, 0);
            draw_geometry_quad(&layer_quads[1], 0, 0);
            layer_quads[0].col[0] = layer_quads[0].col[1] = layer_quads[0].col[2];
            layer_quads[1].col[0] = layer_quads[1].col[1] = layer_quads[1].col[2];
            qcopy(&layer_quads[0].v[0], &layer_quads[0].v[2]);
            qcopy(&layer_quads[0].v[1], &layer_quads[0].v[3]);
            qcopy(&layer_quads[1].v[0], &layer_quads[1].v[2]);
            qcopy(&layer_quads[1].v[1], &layer_quads[1].v[3]);
            sz[0].z *= 0.95f;
            sz[1].z = sz[0].z * 4.0f;
            branch_age_cursor++;
            branch_offset_cursor++;
        }
    }
}

extern unsigned char *FUN_L00_00272060(
    float *pos, int a, float f0, float f1, int col, int mode, int b, float *vel, float f2);

extern short lightning_arc_particle_speed_range_start_gp __asm__("D_L14_00161CA0")
    __attribute__((sda));

extern short lightning_arc_particle_speed_range_end_gp __asm__("D_L14_00161CA4")
    __attribute__((sda));

extern short D_L14_00161CA8 __attribute__((sda));

extern short lightning_arc_particle_scale_range_start_gp __asm__("D_L14_00161CAC")
    __attribute__((sda));

extern short lightning_arc_particle_scale_range_end_gp __asm__("D_L14_00161CB0")
    __attribute__((sda));

void spawn_lightning_arc_particle_pairs(float *particle_spawn_position, int spawn_pair_count) {
    float          initial_velocity[4];
    float          saved_position[4];
    float          particle_scale, velocity_horizontal_angle, velocity_vertical_angle;
    int            random_sign, r0, i, id;
    unsigned char *particle;
    if (spawn_pair_count > 0) {
        i = spawn_pair_count;
        do {
            particle_scale =
                random_float_between(*(float *)&lightning_arc_particle_scale_range_start_gp,
                                     *(float *)&lightning_arc_particle_scale_range_end_gp);
            random_sign = random_integer_below(2);
            if (random_sign == 0)
                random_sign--;
            velocity_horizontal_angle = random_angle_radians();
            velocity_vertical_angle = random_float_between(0.34906584f, 1.5358897f);
            build_spherical_offset(
                initial_velocity,
                random_float_between(*(float *)&lightning_arc_particle_speed_range_start_gp,
                                     *(float *)&lightning_arc_particle_speed_range_end_gp),
                velocity_horizontal_angle,
                velocity_vertical_angle);
            qcopy(saved_position, particle_spawn_position);
            particle_spawn_position[0] += FUN_L00_00257c48(0.0f, 0.15f);
            particle_spawn_position[1] += FUN_L00_00257c48(0.0f, 0.15f);
            particle_spawn_position[2] += FUN_L00_00257c48(0.0f, 0.15f);
            id = scale_game_frames(0x3C);
            particle = FUN_L00_00272060(particle_spawn_position,
                                        id,
                                        particle_scale * 0.1f,
                                        particle_scale,
                                        0x7F7F2020,
                                        0,
                                        random_sign,
                                        initial_velocity,
                                        *(float *)&D_L14_00161CA8);
            if (particle != 0) {
                particle[9] = func_001FA898_r(4.0f) + 0x70;
            }
            random_sign = -random_sign;
            particle = FUN_L00_00272060(particle_spawn_position,
                                        scale_game_frames(0x3C),
                                        particle_scale * 0.07f,
                                        particle_scale * 0.7f,
                                        0x7F7F7F7F,
                                        1,
                                        random_sign,
                                        initial_velocity,
                                        *(float *)&D_L14_00161CA8);
            if (particle != 0) {
                particle[9] = func_001FA898_r(4.0f) + 0x70;
            }
        } while (--i != 0);
    }
}

extern float fast_sin(float);

float absolute_float(float input) __asm__("func_001F99C0");

short lightning_arc_interval_frames_gp __asm__("D_L14_00161C54") = 0xb4;

s32 lightning_arc_lifetime_range_start_gp __asm__("D_L14_00161C98") = 0x38;

u8 lightning_arc_alpha_fade __asm__("D_L14_00161CE0") = 0;

char *lightning_arc_owner_moby __asm__("D_L14_00161CE8") = 0;
