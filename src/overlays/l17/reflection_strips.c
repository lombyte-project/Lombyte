/* Level 17 reflection strips: sphere-map UVs for the reflective mesh strips and the draw callbacks that build and draw them. */
#include "types.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

/* Projects each direction pair into 2D texture coordinates (reflection mapping). */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7018), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z;
} V3;

typedef struct {
    V3    v;
    float w;
} V4;

void compute_reflection_sphere_map_uvs(int    vertex_count,
                                       V3    *input_directions,
                                       V3    *vertex_positions,
                                       int    unused,
                                       float *out) __asm__("FUN_L17_002f54d8");

void compute_reflection_sphere_map_uvs(

    int vertex_count, V3 *input_directions, V3 *vertex_positions, int unused, float *out) {
    V4  normalized_position_delta;
    V4  vertex_position;
    V4  reflection_mapping_vector;
    V4  normalized_input_direction;
    int i;
    for (i = 0; i < vertex_count; i++) {
        float direction_dot_product;
        float sphere_map_denominator;
        vertex_position.w = 1.0f;
        vertex_position.v.x = vertex_positions[i].x;
        vertex_position.v.y = vertex_positions[i].y;
        vertex_position.v.z = vertex_positions[i].z;
        fast_vec_sub(&normalized_position_delta, &vertex_position, D_L17_001676C0);
        fast_vec_normalize(&normalized_position_delta, &normalized_position_delta, 1.0f);
        normalized_input_direction.w = 1.0f;
        normalized_input_direction.v.x = input_directions[i].x;
        normalized_input_direction.v.y = input_directions[i].y;
        normalized_input_direction.v.z = input_directions[i].z;
        fast_vec_normalize(&normalized_input_direction, &normalized_input_direction, 1.0f);
        direction_dot_product =
            fast_vec_dot(&normalized_input_direction, &normalized_position_delta);
        fast_vec_scale(&reflection_mapping_vector,
                       &normalized_input_direction,
                       direction_dot_product + direction_dot_product);
        fast_vec_sub(
            &reflection_mapping_vector, &normalized_position_delta, &reflection_mapping_vector);
        fast_vec_normalize(&reflection_mapping_vector, &reflection_mapping_vector, 1.0f);
        reflection_mapping_vector.v.z = reflection_mapping_vector.v.z + 1.0f;
        sphere_map_denominator = fast_vec_length(&reflection_mapping_vector);
        sphere_map_denominator = sphere_map_denominator + sphere_map_denominator;
        out[i * 2] = (reflection_mapping_vector.v.x / sphere_map_denominator + 0.5f) * 4.0f;
        out[i * 2 + 1] = (reflection_mapping_vector.v.y / sphere_map_denominator + 0.5f) * 4.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F71D0), where it is exact; names translated to the US level program. */

extern char reflection_strip_uv_buffer[] __asm__("D_L17_001DE8A0");
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
void        compute_reflection_sphere_map_uvs(int n, V3 *b, V3 *a, int unused, float *out) __asm__("FUN_L17_002f54d8");

int D_L17_001623F0[2] = {26, 26};

extern float D_L17_001DE150[];
extern float D_L17_001DE288[];
float       *D_L17_001623F8[2] = {D_L17_001DE150, D_L17_001DE288};

extern float D_L17_001DE3C0[];
extern float D_L17_001DE490[];
float       *D_L17_00162400[2] = {D_L17_001DE3C0, D_L17_001DE490};

extern float D_L17_001DE560[];
extern float D_L17_001DE698[];
float       *D_L17_00162408[2] = {D_L17_001DE560, D_L17_001DE698};

extern float D_L17_001DE7D0[];
extern float D_L17_001DE838[];
float       *D_L17_00162410[2] = {D_L17_001DE7D0, D_L17_001DE838};

void draw_reflection_strips(void) __asm__("FUN_L17_002f5690");

void draw_reflection_strips(void) {
    int    i;
    float *reflection_uv_coordinates;
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    reflection_uv_coordinates = (float *)reflection_strip_uv_buffer;
    for (i = 0; i < 2; i++) {
        compute_reflection_sphere_map_uvs(D_L17_001623F0[i],
                                          ((int *)D_L17_00162408)[i],
                                          ((int *)D_L17_001623F8)[i],
                                          ((int *)D_L17_00162400)[i],
                                          reflection_uv_coordinates);
        FUN_L00_001fde98(D_L17_001623F0[i],
                         ((int *)D_L17_001623F8)[i],
                         ((int *)D_L17_00162410)[i],
                         reflection_uv_coordinates,
                         1);
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F74E8), where it is exact; names translated to the US level program. */

extern char  reflection_sphere_map_uv_buffer[] __asm__("D_L17_001DF6A0");
extern float D_L17_001DED50[];
float       *D_L17_00162440[2] = {D_L17_001DED50, 0};

extern float D_L17_001DF280[];
float       *D_L17_00162450[2] = {D_L17_001DF280, 0};

extern float D_L17_001DF598[];
float       *D_L17_00162458[2] = {D_L17_001DF598, 0};

int D_L17_00162420[2] = {66, 0};

extern float D_L17_001DF070[];
float       *D_L17_00162448[2] = {D_L17_001DF070, 0};

void draw_reflection_strip(void) __asm__("FUN_L17_002f59a8");

void draw_reflection_strip(void) {
    int    i;
    float *sphere_map_uv_coordinates;
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    sphere_map_uv_coordinates = (float *)reflection_sphere_map_uv_buffer;
    for (i = 0; i < 1; i++) {
        compute_reflection_sphere_map_uvs(D_L17_00162420[i],
                                          (V3 *)((int *)D_L17_00162450)[i],
                                          (V3 *)((int *)D_L17_00162440)[i],
                                          ((int *)D_L17_00162448)[i],
                                          sphere_map_uv_coordinates);
        FUN_L00_001fde98(D_L17_00162420[i],
                         ((int *)D_L17_00162440)[i],
                         ((int *)D_L17_00162458)[i],
                         sphere_map_uv_coordinates,
                         1);
    }
}
/* Computes a per-vertex 2D sphere-map texture coordinate from position and normal arrays. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7C78), where it is exact; names translated to the US level program. */

void compute_vertex_to_reference_sphere_map_uvs(int    vertex_count,
                                                char  *nrm,
                                                char  *vertex_positions,
                                                int    unused,
                                                float *out) __asm__("FUN_L17_002f6138");

void compute_vertex_to_reference_sphere_map_uvs(

    int vertex_count, char *nrm, char *vertex_positions, int unused, float *out) {
    float normalized_position_delta[4];
    float vertex_position[4];
    float reflection_mapping_vector[4];
    float normalized_normal[4];
    int   i;
    for (i = 0; i < vertex_count; i++) {
        float *position_element = (float *)(i * 12 + (int)vertex_positions);
        float *normal_element;
        float  sphere_map_denominator;
        vertex_position[3] = 1.0f;
        vertex_position[0] = position_element[0];
        vertex_position[1] = position_element[1];
        vertex_position[2] = position_element[2];
        fast_vec_sub(normalized_position_delta, D_L17_001676C0, vertex_position);
        fast_vec_normalize(normalized_position_delta, normalized_position_delta, 1.0f);
        normalized_normal[3] = 1.0f;
        normal_element = (float *)(i * 12 + (int)nrm);
        normalized_normal[0] = normal_element[0];
        normalized_normal[1] = normal_element[1];
        normalized_normal[2] = normal_element[2];
        fast_vec_normalize(normalized_normal, normalized_normal, 1.0f);
        fast_vec_scale(reflection_mapping_vector,
                       normalized_normal,
                       fast_vec_dot(normalized_normal, normalized_position_delta) * 2.0f);
        fast_vec_sub(
            reflection_mapping_vector, normalized_position_delta, reflection_mapping_vector);
        fast_vec_normalize(reflection_mapping_vector, reflection_mapping_vector, 1.0f);
        reflection_mapping_vector[2] += 1.0f;
        sphere_map_denominator = fast_vec_length(reflection_mapping_vector) * 2.0f;
        out[i * 2] = (reflection_mapping_vector[0] / sphere_map_denominator + 0.5f) * 4.0f;
        out[i * 2 + 1] = (reflection_mapping_vector[1] / sphere_map_denominator + 0.5f) * 4.0f;
    }
}
