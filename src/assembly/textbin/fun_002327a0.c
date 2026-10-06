#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002327a0/FUN_002327a0.s", FUN_002327a0);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef float FloatVector4[4] __attribute__((aligned(16)));

typedef struct {
    u128 q;
} Quadword;

typedef struct {
    s16 vertex_index;
    s16 pad;
} QuadCornerIndex;

typedef struct {
    QuadCornerIndex corners[4];
} IndexedQuad;

typedef struct {
    u8 pad0[0x10];
    float position_x;
    float position_y;
    u8 pad18[0x8E];
    s16 class_id;
} EnvironmentMappedObject;

typedef struct {
    u8 pad0[0x140];
    float position_x;
    float position_y;
} EnvironmentCameraState;

extern s32 D_0013E050[];
extern s32 game_stage __asm__("D_0015F604");
extern s32 D_001604A4 __attribute__((sda));
extern s32 D_001604A8 __attribute__((sda));
extern s32 D_001604AC __attribute__((sda));
extern s32 environment_mesh_colors[2] __asm__("D_00160520") __attribute__((sda));
extern s32 environment_mesh_vertex_counts[2] __asm__("D_00160530") __attribute__((sda));
extern s32 environment_mesh_quad_counts[2] __asm__("D_00160540") __attribute__((sda));
extern Quadword *environment_mesh_normals[2] __asm__("D_00160550") __attribute__((sda));
extern Quadword *environment_mesh_positions[2] __asm__("D_00160560") __attribute__((sda));
extern IndexedQuad *environment_mesh_quads[2] __asm__("D_00160570") __attribute__((sda));
extern EnvironmentCameraState D_00186F40;
extern FloatVector4 camera_position __asm__("D_00187080");
extern Quadword D_001DC4E0[];
extern float D_001DCB40[][2];
extern float D_001DCE70[][2];

extern u64 get_effect_texture(int) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int scale_ticks(int) __asm__("func_001F96F8");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
extern float square_root_float(float) __asm__("func_001F9988");
extern float AbsoluteFloat(float) __asm__("func_001F99C0");
extern void subtract_vectors(void *, void *, void *) __asm__("func_001F9A28");
extern void scale_vector(void *, void *, float) __asm__("func_001F9A68");
extern float vector_dot_product(void *, void *) __asm__("func_001F9AB0");
extern void normalize_vector(void *, void *, float) __asm__("func_001F9BF8");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern void calculate_object_transform(EnvironmentMappedObject *, int,
                                       void *) __asm__("func_0020CCA8");

void render_environment_mapped_object(EnvironmentMappedObject *object) __asm__("FUN_002327a0");

void render_environment_mapped_object(EnvironmentMappedObject *object) {
    Quadword quad_positions[4];
    int colors[4];
    float texture_coordinates[4][2];
    u64 quad_state[4];
    FloatVector4 object_transform[4];
    FloatVector4 normal;
    FloatVector4 reflection;
    FloatVector4 view_direction;
    IndexedQuad *indexed_quads;
    Quadword *positions;
    Quadword *normals;
    int quad_count;
    int vertex_count;
    int class_index;
    int color;
    int mapping_enabled;
    float transition_fraction;
    float sphere_denominator;
    float new_u;
    float new_v;
    float old_v;
    int element_index;
    int corner_index;

    class_index = object->class_id - 0x212;
    positions = environment_mesh_positions[class_index];
    normals = environment_mesh_normals[class_index];
    indexed_quads = environment_mesh_quads[class_index];
    quad_count = environment_mesh_quad_counts[class_index];
    vertex_count = environment_mesh_vertex_counts[class_index];
    if (game_stage == 6 && D_0013E050[0] == 4) {
        quad_state[1] = get_effect_texture(1);
    } else {
        quad_state[1] = get_effect_texture(0x15);
    }
    mapping_enabled = 0;
    color = environment_mesh_colors[class_index];
    quad_state[2] = 0xFF9000000260;
    quad_state[3] = 0x8000000044;
    quad_state[0] = 0;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    calculate_object_transform(object, 0, object_transform);
    if (game_stage != 0 || (AbsoluteFloat(D_00186F40.position_x - object->position_x) < 16.0f &&
                            AbsoluteFloat(D_00186F40.position_y - object->position_y) < 16.0f)) {
        mapping_enabled = 1;
    }
    if (game_stage == 6 && D_0013E050[0] == 4) {
        mapping_enabled = 0;
    }
    if (mapping_enabled != 0 || D_001604A4 == 1) {
        D_001604A8 = 1;
        tick_countdown_32(&D_001604AC);
        transition_fraction =
            convert_integer_to_float(D_001604AC) / convert_integer_to_float(scale_ticks(0x3C));
        for (element_index = 0; element_index < vertex_count; element_index++) {
            transform_vector(&D_001DC4E0[element_index], &positions[element_index],
                             object_transform);
            subtract_vectors(view_direction, &D_001DC4E0[element_index], camera_position);
            normalize_vector(view_direction, view_direction, 1.0f);
            transform_vector(normal, &normals[element_index], object_transform);
            normalize_vector(normal, normal, 0.1f);
            scale_vector(reflection, normal, vector_dot_product(normal, view_direction) * 2.0f);
            subtract_vectors(reflection, view_direction, reflection);
            normalize_vector(reflection, reflection, 1.0f);
            reflection[2] += 1.0f;
            sphere_denominator = square_root_float(reflection[2] * 2.0f) * 2.0f;
            if (D_001604A4 == 1 || D_001604AC == 0) {
                D_001DCB40[element_index][0] = reflection[0] / sphere_denominator + 0.5f;
                new_v = reflection[1] / sphere_denominator + 0.5f;
            } else {
                new_u = D_001DCE70[element_index][0];
                new_v = reflection[0] / sphere_denominator + 0.5f;
                D_001DCB40[element_index][0] = new_v + (new_u - new_v) * transition_fraction;
                new_v = reflection[1] / sphere_denominator + 0.5f;
                old_v = D_001DCE70[element_index][1];
                new_v = new_v + (old_v - new_v) * transition_fraction;
            }
            D_001DCB40[element_index][1] = new_v;
        }
        if (D_001604A4 == 1) {
            D_001604A4 = 2;
        }
    } else {
        if (D_001604A8 == 1) {
            D_001604A8 = 0;
            for (element_index = 0; element_index < vertex_count; element_index++) {
                D_001DCE70[element_index][0] = D_001DCB40[element_index][0];
                D_001DCE70[element_index][1] = D_001DCB40[element_index][1];
                transform_vector(&D_001DC4E0[element_index], &positions[element_index],
                                 object_transform);
            }
        } else {
            for (element_index = 0; element_index < vertex_count; element_index++) {
                transform_vector(&D_001DC4E0[element_index], &positions[element_index],
                                 object_transform);
            }
        }
        D_001604AC = scale_ticks(0x3C);
    }
    for (element_index = 0; element_index < quad_count; element_index++) {
        for (corner_index = 0; corner_index < 4; corner_index++) {
            int vertex_index = indexed_quads[element_index].corners[corner_index].vertex_index;

            qcopy(&quad_positions[corner_index], &D_001DC4E0[vertex_index]);
            texture_coordinates[corner_index][0] = D_001DCB40[vertex_index][0];
            texture_coordinates[corner_index][1] = D_001DCB40[vertex_index][1];
        }
        draw_geometry_quad(quad_positions, 0, 0);
    }
}
extern __typeof__(render_environment_mapped_object) func_002327A0
    __attribute__((alias("FUN_002327a0")));

#endif /* NON_MATCHING */
