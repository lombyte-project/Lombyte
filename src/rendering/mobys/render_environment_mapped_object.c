#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/globals.h"
#include "rnc/rendering/effect_texture.h"

typedef float FloatVector4[4] __attribute__((aligned(16)));

typedef struct {
    s16 vertex_index;
    s16 pad;
} QuadCornerIndex;

typedef struct {
    QuadCornerIndex corners[4];
} IndexedQuad;

typedef struct {
    u8    pad0[0x10];
    float position_x;
    float position_y;
    u8    pad18[0x8E];
    s16   class_id;
} EnvironmentMappedObject;

#include "rnc/rendering/mobys/render_environment_mapped_object_globals.h"
#include "rnc/rendering/mobys/render_environment_mapped_object_globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/camera/camera_globals.h"

extern s32          environment_mapping_was_active_gp __asm__("D_001604A8") __attribute__((sda));
extern s32          D_001604AC __attribute__((sda));
extern s32          environment_mesh_colors[2] __asm__("D_00160520") __attribute__((sda));
extern s32          environment_mesh_vertex_counts[2] __asm__("D_00160530") __attribute__((sda));
extern s32          environment_mesh_quad_counts[2] __asm__("D_00160540") __attribute__((sda));
extern Vec4        *environment_mesh_normals[2] __asm__("D_00160550") __attribute__((sda));
extern Vec4        *environment_mesh_positions[2] __asm__("D_00160560") __attribute__((sda));
extern IndexedQuad *environment_mesh_quads[2] __asm__("D_00160570") __attribute__((sda));

#include "rnc/ui/menus/menu_globals.h"

extern Vec4  environment_mapping_transformed_vertex_positions[] __asm__("D_001DC4E0");
extern float environment_mapping_vertex_uvs[][2] __asm__("D_001DCB40");
extern float environment_mapping_saved_vertex_uvs[][2] __asm__("D_001DCE70");

extern void  draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int   scale_game_frames(int) __asm__("func_001F96F8");
extern void  fast_dec_timer(s32 *) __asm__("func_001F9740");
extern float square_root(float) __asm__("func_001F9988");
extern float absolute_float(float) __asm__("func_001F99C0");
extern void  fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void  scale_vector(void *, void *, float) __asm__("func_001F9A68");
extern float fast_vec_dot(void *, void *) __asm__("func_001F9AB0");
extern void  fast_vec_normalize(void *, void *, float) __asm__("func_001F9BF8");
extern void  transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
extern void
build_moby_bone_transform(EnvironmentMappedObject *, int, void *) __asm__("func_0020CCA8");

void render_environment_mapped_object(EnvironmentMappedObject *object) __asm__("FUN_002327a0");

void render_environment_mapped_object(EnvironmentMappedObject *object) {
    Vec4         quad_positions[4];
    int          colors[4];
    float        texture_coordinates[4][2];
    u64          quad_state[4];
    FloatVector4 object_transform[4];
    FloatVector4 normal;
    FloatVector4 reflection;
    FloatVector4 view_direction;
    IndexedQuad *indexed_quads;
    Vec4        *positions;
    Vec4        *normals;
    int          quad_count;
    int          vertex_count;
    int          class_index;
    int          color;
    int          mapping_enabled;
    float        transition_fraction;
    float        sphere_denominator;
    float        coordinate;
    int          element_index;
    int          corner_index;

    class_index = object->class_id - 0x212;
    positions = environment_mesh_positions[class_index];
    normals = environment_mesh_normals[class_index];
    indexed_quads = environment_mesh_quads[class_index];
    quad_count = environment_mesh_quad_counts[class_index];
    vertex_count = environment_mesh_vertex_counts[class_index];
    if (game_mode == 6 && ship_substate[0] == 4) {
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
    build_moby_bone_transform(object, 0, object_transform);
    if (game_mode != 0 ||
        (absolute_float(preview_camera.position.f[0] - object->position_x) < 16.0f &&
         absolute_float(preview_camera.position.f[1] - object->position_y) < 16.0f)) {
        mapping_enabled = 1;
    }
    if (game_mode == 6 && ship_substate[0] == 4) {
        mapping_enabled = 0;
    }
    if (mapping_enabled != 0 || environment_mapping_uv_refresh_state == 1) {
        environment_mapping_was_active_gp = 1;
        fast_dec_timer(&D_001604AC);
        transition_fraction = convert_integer_to_float(D_001604AC) /
                              convert_integer_to_float(scale_game_frames(0x3C));
        for (element_index = 0; element_index < vertex_count; element_index++) {
            transform_vector(&environment_mapping_transformed_vertex_positions[element_index],
                             &positions[element_index],
                             object_transform);
            fast_vec_sub(view_direction,
                         &environment_mapping_transformed_vertex_positions[element_index],
                         camera_position);
            fast_vec_normalize(view_direction, view_direction, 1.0f);
            transform_vector(normal, &normals[element_index], object_transform);
            fast_vec_normalize(normal, normal, 0.1f);
            scale_vector(reflection, normal, fast_vec_dot(normal, view_direction) * 2.0f);
            fast_vec_sub(reflection, view_direction, reflection);
            fast_vec_normalize(reflection, reflection, 1.0f);
            reflection[2] += 1.0f;
            sphere_denominator = square_root(reflection[2] * 2.0f) * 2.0f;
            if (environment_mapping_uv_refresh_state == 1 || D_001604AC == 0) {
                environment_mapping_vertex_uvs[element_index][0] =
                    reflection[0] / sphere_denominator + 0.5f;
                environment_mapping_vertex_uvs[element_index][1] =
                    reflection[1] / sphere_denominator + 0.5f;
            } else {
                coordinate = reflection[0] / sphere_denominator + 0.5f;
                environment_mapping_vertex_uvs[element_index][0] =
                    coordinate +
                    (environment_mapping_saved_vertex_uvs[element_index][0] - coordinate) *
                        transition_fraction;
                coordinate = reflection[1] / sphere_denominator + 0.5f;
                environment_mapping_vertex_uvs[element_index][1] =
                    coordinate +
                    (environment_mapping_saved_vertex_uvs[element_index][1] - coordinate) *
                        transition_fraction;
            }
        }
        if (environment_mapping_uv_refresh_state == 1) {
            environment_mapping_uv_refresh_state = 2;
        }
    } else {
        if (environment_mapping_was_active_gp == 1) {
            environment_mapping_was_active_gp = 0;
            for (element_index = 0; element_index < vertex_count; element_index++) {
                environment_mapping_saved_vertex_uvs[element_index][0] =
                    environment_mapping_vertex_uvs[element_index][0];
                environment_mapping_saved_vertex_uvs[element_index][1] =
                    environment_mapping_vertex_uvs[element_index][1];
                transform_vector(&environment_mapping_transformed_vertex_positions[element_index],
                                 &positions[element_index],
                                 object_transform);
            }
        } else {
            for (element_index = 0; element_index < vertex_count; element_index++) {
                transform_vector(&environment_mapping_transformed_vertex_positions[element_index],
                                 &positions[element_index],
                                 object_transform);
            }
        }
        D_001604AC = scale_game_frames(0x3C);
    }
    for (element_index = 0; element_index < quad_count; element_index++) {
        for (corner_index = 0; corner_index < 4; corner_index++) {
            int vertex_index = indexed_quads[element_index].corners[corner_index].vertex_index;

            qcopy(&quad_positions[corner_index],
                  &environment_mapping_transformed_vertex_positions[vertex_index]);
            texture_coordinates[corner_index][0] = environment_mapping_vertex_uvs[vertex_index][0];
            texture_coordinates[corner_index][1] = environment_mapping_vertex_uvs[vertex_index][1];
        }
        draw_geometry_quad(quad_positions, 0, 0);
    }
}

extern __typeof__(render_environment_mapped_object) func_002327A0
    __attribute__((alias("FUN_002327a0")));
