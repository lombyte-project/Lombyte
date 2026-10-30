/* Level 06 moby class 0x43b: breakable window; shatters into fragments, runs its camera sequence and draws its backdrop mesh. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/overlay/l06/hero/hero_set_state.h"
#include "rnc/overlay/l06/scripted_camera_sequence.h"

extern float          D_L06_00167540[];
extern void           draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern char           D_0013F3D0[];
extern unsigned char *D_L06_0015FFD8_ffcb0 __asm__("D_L06_0015FFD8");

#include "rnc/overlay/cuboid.h"

extern float D_L06_0015F3FC __asm__("D_L06_0015F3FC");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void  FUN_L06_00312b40(float, float, float, float, float, float);
extern void  FUN_L06_00312a78(void *);
extern void  FUN_L06_00312ad0(void *);
extern void  FUN_L06_00312ce8(int);
extern void  set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
void         spawn_moby_breakup_effect(char *source_moby) __asm__("FUN_L06_00300278");
extern void  spawn_moby_breakup_effect(char *) __asm__("FUN_L06_00300278");
extern void  FUN_L06_00210fc8(int, char *);
extern void  FUN_L06_00300958(void);
extern void  effect_quad_draw_single(void) __asm__("FUN_L06_00300720");
extern void  FUN_L06_00300588(void);
extern void  draw_backdrop_mesh(void) __asm__("FUN_L06_003003f0");
extern int   D_L06_0015F408;
extern float camera_sequence_moby_scale_multiplier_gp __asm__("D_L06_00162048")
    __attribute__((sda));

#include "rnc/overlay/l06/mobys/breakable_window_data/update_breakable_window.h"

extern float camera_sequence_rotation_x_degrees_gp __asm__("D_L06_00162050") __attribute__((sda));
void         update_breakable_window(char *moby) __asm__("FUN_L06_002ffcb0");
extern void  update_entity(char *) __asm__("FUN_L00_00250df8");
extern unsigned char              *
spawn_spinning_moby_fragment(char *, float *, float *) __asm__("FUN_L06_00300c60");
extern float moby_breakup_z_offset_gp __asm__("D_L06_0016204C") __attribute__((sda));

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V3003f0;

typedef struct {
    float u, v;
} UV3003f0;

typedef struct {
    V3003f0      corner[4];
    unsigned int color[4];
    UV3003f0     uv[4];
    long         unk70, tex, unk80, unk88;
} Q3003f0;

typedef struct {
    char  pad[0x38];
    float f38;
    int   pad3c;
} B3003f0;

typedef struct {
    short v, uv;
} I3003f0;

#include "rnc/overlay/l06/mobys/breakable_window_data/draw_backdrop_mesh_2.h"

extern unsigned int l06_backdrop_mesh_vertex_color __asm__("D_L06_0016206C") __attribute__((sda));

#include "rnc/overlay/l06/mobys/breakable_window_data/draw_backdrop_mesh.h"

extern V3003f0  l06_backdrop_mesh_vertex_positions[] __asm__("D_L06_001F2EB0");
extern I3003f0  l06_backdrop_mesh_quad_corner_index_pairs[] __asm__("D_L06_001F34B0");
extern UV3003f0 l06_backdrop_mesh_texture_coordinates[] __asm__("D_L06_001F38B0");

/* Draws the 96 quads of the second backdrop mesh, skipping those that face away from D_L06_00167540. */
typedef float FVec4_l06[4] __attribute__((aligned(16)));

typedef struct {
    FVec4_l06 v;
} FRow_l06;

extern FRow_l06 D_L06_001FAFF0[] NOT_SDA;
extern FRow_l06 D_L06_001FA2F0[] NOT_SDA;
extern FRow_l06 D_L06_001F77B0[] NOT_SDA;
extern float    D_L06_001F84B0[][2] NOT_SDA;
extern short    D_L06_001F7EB0[][4][2] NOT_SDA;

extern char D_0013F3D0[] NOT_SDA;

void update_breakable_window(char *moby) {
    enum {
        INITIALIZE_DRAW_SELECTION = 0,
        WAIT_FOR_LINKED_MOBY_TRIGGER = 1,
        WAIT_CAMERA_START_DELAY = 2,
        BLEND_CAMERA_TO_HERO = 3,
        WAIT_FOR_NEARBY_HERO = 4
    };
    int  *data = ((int *)MOBY(moby)->pvars);
    float camera_position[4], camera_setup_rotation_or_position_target[4], from[4], target[4],
        offset[4];
    int base;
    int state;
    MOBY(moby)->scale =
        *(float *)(((char *)MOBY(moby)->pclass) + 0x24) * camera_sequence_moby_scale_multiplier_gp;
    switch ((unsigned char)moby[0x20]) {
    case INITIALIZE_DRAW_SELECTION:
        data[1] = 1;
        moby[0x20] = 1;
        break;
    case WAIT_FOR_LINKED_MOBY_TRIGGER:
        if ((data[2] < 0 || (unsigned char)D_L06_0015FFD8_ffcb0[(data[2] << 8) + 0xbc] == 0) &&
            data[8] != -1) {
            break;
        }
        if (vector_distance_xy(moby + 0x10, D_0013F3D0) < 32.0f) {
            moby[0x20] = 2;
            data[9] = scale_game_frames(30);
        } else {
            spawn_moby_breakup_effect(moby);
            moby[0x20] = 4;
        }
        break;
    case WAIT_CAMERA_START_DELAY:
        if (fast_dec_timer(data + 9)) {
            {
                int state;
                int base;
                moby[0x20] = 3;
                scripted_camera_sequence_active = 1;
                D_L06_0015F408 = 0x12;
                state = data[5];
                state <<= 7;
                base = (int)l06_cuboids;
                qcopy(camera_position, (void *)(state + base + 0x30));
                qcopy(camera_setup_rotation_or_position_target, (void *)(state + base + 0x70));
            }
            jet_mount(camera_position,
                      camera_setup_rotation_or_position_target,
                      1,
                      scale_game_frames(300),
                      0);
            FUN_L06_00312b40(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            FUN_L06_00312a78(camera_position);
            FUN_L06_00312ad0(camera_setup_rotation_or_position_target);
            {
                int   base;
                int   state;
                float facing;
                data[6] = 0;
                {
                    int base;
                    base = data[4];
                    base <<= 7;
                    base += (int)l06_cuboids;
                    facing = fast_arc_tan(MOBY(moby)->pos.x - *(float *)(base + 0x30),
                                          MOBY(moby)->pos.y - *(float *)(base + 0x34));
                }
                {
                    int state;
                    state = data[4];
                    state <<= 7;
                    base = (int)l06_cuboids;
                    *(float *)(state + base + 0x78) = facing;
                }
                state = data[4];
                state <<= 7;
                set_race_camera(
                    (void *)(base + state + 0x30), (void *)(base + state + 0x70), 0x72, 0);
            }
        }
        if (data[9] < camera_sequence_scalar_ramp_duration_frames_gp)
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(data[9]) /
                                        (float)camera_sequence_scalar_ramp_duration_frames_gp;
        break;
    case BLEND_CAMERA_TO_HERO:
        if (data[9] < camera_sequence_scalar_ramp_duration_frames_gp) {
            data[9]++;
            D_L06_0015F3FC = 1.0f - convert_integer_to_float(data[9]) /
                                        (float)camera_sequence_scalar_ramp_duration_frames_gp;
        }
        /* This field holds float bits; retail completes only on an ordered >= comparison. */
        if (*(float *)(data + 6) >= 0.99999f) {
            moby[0x20] = 5;
            hero_set_state(0, 0);
            FUN_L06_00312ce8(2);
            scripted_camera_sequence_active = 0;
        } else {
            if (*(float *)(data + 6) >= 0.05f && data[3] >= 0) {
                {
                    int state;
                    int base;
                    state = data[3];
                    state <<= 7;
                    base = (int)l06_cuboids;
                    qcopy((void *)(state + base + 0x30), D_0013F3D0);
                    *(float *)(state + base + 0x38) -= 0.5f;
                }
                data[3] = -1;
            }
            if (data[1] == 1 &&
                vector_distance_xy(moby + 0x10, D_L06_0015FFD8_ffcb0 + (data[8] << 8) + 0x10) >
                    0.1f) {
                allocate_voice_for_target_entry(0, 0, moby);
                spawn_moby_breakup_effect(moby);
            }
            advance_accelerated_scalar((float *)(data + 6),
                                       (float *)(data + 7),
                                       1.0f,
                                       frame_time_sq,
                                       frame_time_sq,
                                       frame_time * 1.5f);
            {
                int state;
                int base;
                state = data[5];
                state <<= 7;
                base = (int)l06_cuboids;
                qcopy(camera_position, (void *)(state + base + 0x30));
                qcopy(from, (void *)(state + base + 0x70));
            }
            from[0] = camera_sequence_rotation_x_degrees_gp * 0.017453292f;
            qcopy(camera_setup_rotation_or_position_target, D_0013F3D0);
            qcopy(target, D_0013F3D0 + 16);
            /* Retail takes this component from the camera rotation, not the hero position. */
            target[1] = from[1];
            fast_vec_normalize(offset, D_0013F3D0 - 0x80, -2.0f);
            fast_vec_add(camera_setup_rotation_or_position_target,
                         camera_setup_rotation_or_position_target,
                         offset);
            camera_setup_rotation_or_position_target[2] += 0.85f;
            lerp_vector(camera_position,
                        camera_position,
                        camera_setup_rotation_or_position_target,
                        *(float *)(data + 6));
            lerp_vector(from, from, target, *(float *)(data + 6));
            set_camera_position(camera_position);
            set_camera_rotation(from);
        }
        break;
    case WAIT_FOR_NEARBY_HERO:
        if (vector_distance_xy(moby + 0x10, D_0013F3D0) < 16.0f) {
            state = data[3];
            state <<= 7;
            base = (int)l06_cuboids;
            qcopy((void *)(state + base + 0x30), D_0013F3D0);
            *(float *)(state + base + 0x38) -= 0.5f;
            data[3] = -1;
            moby[0x20] = 5;
        }
        break;
    }
    if (data[1] != 0) {
        if (data[0] != 0)
            FUN_L06_00210fc8((int)effect_quad_draw_single, moby);
        else
            FUN_L06_00210fc8((int)FUN_L06_00300958, moby);
    } else {
        if (data[0] != 0)
            FUN_L06_00210fc8((int)draw_backdrop_mesh, moby);
        else
            FUN_L06_00210fc8((int)FUN_L06_00300588, moby);
    }
}

/* Breaks the moby apart: spawns a copy (class 0x43C) and throws 50
 * fragments out in random directions. */
void spawn_moby_breakup_effect(char *source_moby) {
    float fragment_position[4];
    float fragment_velocity[4];
    char *spawned_moby;
    float fragment_step_length;
    float fragment_angle;
    int   i;

    *(int *)(((char *)MOBY(source_moby)->pvars) + 4) = 0;
    spawned_moby = create_moby(0x43C);
    MOBY(spawned_moby)->draw_dist = 0x40;
    spawned_moby[0x31] = 1;
    *(long long *)(spawned_moby + 0x38) = *(long long *)(source_moby + 0x38);
    MOBY(spawned_moby)->flags = MOBY(source_moby)->flags;
    qcopy(spawned_moby + 0x10, source_moby + 0x10);
    qcopy(spawned_moby + 0x40, source_moby + 0x40);
    MOBY(spawned_moby)->scale = MOBY(source_moby)->scale;
    update_entity(spawned_moby);
    MOBY(source_moby)->flags |= 1;
    for (i = 0; i < 50; i++) {
        fragment_step_length = random_float_between(1.0f, 6.0f) * frame_time;
        fragment_angle = random_angle_radians();
        fragment_velocity[0] = fast_cos(fragment_angle) * 2.0f;
        fragment_velocity[1] = fast_sin(fragment_angle) * 2.0f;
        fragment_velocity[2] = 0.0f;
        fast_vec_add(fragment_position, source_moby + 0x10, fragment_velocity);
        fragment_position[2] +=
            random_float_between(moby_breakup_z_offset_gp + 0.5f, moby_breakup_z_offset_gp + 4.5f);
        fast_vec_normalize(fragment_velocity, fragment_velocity, fragment_step_length);
        spawn_spinning_moby_fragment(source_moby, fragment_position, fragment_velocity);
    }
}

/* Draws the 64 quads of the backdrop mesh, each corner scaled from the shared vertex table. */
void draw_backdrop_mesh(void) {
    Q3003f0      quad;
    B3003f0      backdrop_draw_transform;
    unsigned int shared_quad_vertex_color;
    int          i, j, k;
    I3003f0     *quad_corner_index_pair;

    quad.tex = get_effect_texture(backdrop_mesh_effect_texture_index_gp);
    quad.unk88 = (long)backdrop_mesh_blend_color_a_selector_gp |
                 ((long)backdrop_mesh_blend_color_b_selector_gp << 2) |
                 ((long)backdrop_mesh_blend_alpha_c_selector_gp << 4) |
                 ((long)backdrop_mesh_blend_color_d_selector_gp << 6) |
                 ((long)backdrop_mesh_blend_fixed_alpha_gp << 32);
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    set_identity_matrix(&backdrop_draw_transform);
    shared_quad_vertex_color = l06_backdrop_mesh_vertex_color;
    backdrop_draw_transform.f38 = moby_breakup_z_offset_gp;
    quad.color[3] = shared_quad_vertex_color;
    quad.color[2] = shared_quad_vertex_color;
    quad.color[1] = shared_quad_vertex_color;
    quad.color[0] = shared_quad_vertex_color;
    for (i = 0; i < 64; i++) {
        quad_corner_index_pair = &l06_backdrop_mesh_quad_corner_index_pairs[i * 4];
        for (j = 0; j < 4; j++) {
            k = quad_corner_index_pair->uv;
            fast_vec_scale(&quad.corner[j],
                           &l06_backdrop_mesh_vertex_positions[quad_corner_index_pair->v],
                           backdrop_mesh_vertex_position_scale_gp);
            quad_corner_index_pair++;
            quad.uv[j].u = l06_backdrop_mesh_texture_coordinates[k].u;
            quad.uv[j].v = l06_backdrop_mesh_texture_coordinates[k].v;
        }
        draw_geometry_quad(&quad, &backdrop_draw_transform, 0);
    }
}

void effect_quad_draw_single(void) {
    FRow_l06      quad_corner_positions[4];
    int           colors[4];
    float         uv[4][2];
    unsigned long pkt[4];
    float         backdrop_quad_transform_matrix[4][4];
    FVec4_l06     reference_to_backdrop_quad_unit_direction;
    FVec4_l06     center;
    float        *mp;
    int           i;
    int           j;

    pkt[1] = get_effect_texture(backdrop_mesh_effect_texture_index_gp);
    mp = backdrop_quad_transform_matrix[0];
    pkt[2] = 0xFF9000000260;
    pkt[3] = backdrop_mesh_blend_color_a_selector_gp | (long)backdrop_mesh_blend_color_b_selector_gp
                                                           << 2;
    pkt[3] |= (long)backdrop_mesh_blend_alpha_c_selector_gp << 4;
    pkt[3] |= (long)backdrop_mesh_blend_color_d_selector_gp << 6;
    pkt[3] |= (long)backdrop_mesh_blend_fixed_alpha_gp << 32;
    pkt[0] = 0;
    set_identity_matrix(mp);
    backdrop_quad_transform_matrix[3][2] = moby_breakup_z_offset_gp;
    colors[3] = l06_backdrop_mesh_vertex_color;
    colors[2] = l06_backdrop_mesh_vertex_color;
    colors[1] = l06_backdrop_mesh_vertex_color;
    colors[0] = l06_backdrop_mesh_vertex_color;
    for (i = 0; i < 96; i++) {
        fast_vec_scale(center, &D_L06_001FAFF0[i], backdrop_mesh_vertex_position_scale_gp);
        transform_vector(center, center, backdrop_quad_transform_matrix);
        fast_vec_sub(reference_to_backdrop_quad_unit_direction, center, D_L06_00167540);
        fast_vec_normalize(reference_to_backdrop_quad_unit_direction,
                           reference_to_backdrop_quad_unit_direction,
                           1.0f);
        transform_vector_by_basis(center, &D_L06_001FA2F0[i], mp);
        if (fast_vec_dot(reference_to_backdrop_quad_unit_direction, center) > 0.0f)
            continue;
        for (j = 0; j < 4; j++) {
            int quad_source_vertex_index = D_L06_001F7EB0[i][j][0];
            int quad_source_uv_index = D_L06_001F7EB0[i][j][1];
            fast_vec_scale(&quad_corner_positions[j],
                           &D_L06_001F77B0[quad_source_vertex_index],
                           backdrop_mesh_vertex_position_scale_gp);
            uv[j][0] = D_L06_001F84B0[quad_source_uv_index][0];
            uv[j][1] = D_L06_001F84B0[quad_source_uv_index][1];
        }
        draw_geometry_quad(quad_corner_positions, backdrop_quad_transform_matrix, 0);
    }
}
