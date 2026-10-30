/* Level 16 moby class reflection_mesh_trio (0x797): bounds and draw of three reflection meshes, and the reflection map helpers. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/rendering/vu1_registers.h"

extern char D_L16_001671C0[];
/* For each of n point pairs, reflects a direction and writes its 2D map coordinates. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA010), where it is exact; names translated to the US level program. */

#include "rnc/overlay/l16/mobys/reflection_mesh_trio.h"

void compute_level16_reflection_map_coordinates(int    point_count,
                                                Vec3  *direction_vectors,
                                                Vec3  *point_positions,
                                                int    unused,
                                                float *out) __asm__("FUN_L16_002e8b98");

void compute_level16_reflection_map_coordinates(

    int point_count, Vec3 *direction_vectors, Vec3 *point_positions, int unused, float *out) {
    int   i;
    float relative_direction[4];
    float point_position[4];
    float reflection_projection_scratch[4];
    float reflection_direction_scratch[4];

    for (i = 0; i < point_count; i++) {
        float direction_dot_product;
        float projection_denominator;

        point_position[3] = 1.0f;
        point_position[0] = point_positions[i].x;
        point_position[1] = point_positions[i].y;
        point_position[2] = point_positions[i].z;
        fast_vec_sub(relative_direction, point_position, D_L16_001671C0);
        fast_vec_normalize(relative_direction, relative_direction, 1.0f);
        reflection_direction_scratch[3] = 1.0f;
        reflection_direction_scratch[0] = direction_vectors[i].x;
        reflection_direction_scratch[1] = direction_vectors[i].y;
        reflection_direction_scratch[2] = direction_vectors[i].z;
        fast_vec_normalize(reflection_direction_scratch, reflection_direction_scratch, 1.0f);
        direction_dot_product = fast_vec_dot(reflection_direction_scratch, relative_direction);
        fast_vec_scale(reflection_projection_scratch,
                       reflection_direction_scratch,
                       direction_dot_product + direction_dot_product);
        fast_vec_sub(
            reflection_projection_scratch, relative_direction, reflection_projection_scratch);
        fast_vec_normalize(reflection_projection_scratch, reflection_projection_scratch, 1.0f);
        reflection_projection_scratch[2] = reflection_projection_scratch[2] + 1.0f;
        projection_denominator = fast_vec_length(reflection_projection_scratch);
        projection_denominator = projection_denominator + projection_denominator;
        out[i * 2] = (reflection_projection_scratch[0] / projection_denominator + 0.5f) * 2.0f;
        out[i * 2 + 1] = (reflection_projection_scratch[1] / projection_denominator + 0.5f) * 2.0f;
    }
}

#include "sda.h"

extern int get_effect_texture(int) __asm__("FUN_001f44b8");
/* Sets up the draw state, then draws the three entries of the level's table. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA1B8), where it is exact; names translated to the US level program. */

extern char D_L16_001DC600[][16] NOT_SDA;
extern char D_L16_001DC630[] NOT_SDA;
int         D_L16_00161EF0_gp[4] __asm__("D_L16_00161EF0")
    __attribute__((section(".sdata"))) = {148, 148, 25, 0};

extern float D_L16_001D98D0[];
extern float D_L16_001D9FC0[];
extern float D_L16_001DA6B0[];
float       *D_L16_00161F00_gp[4] __asm__("D_L16_00161F00")
    __attribute__((section(".sdata"))) = {D_L16_001D98D0, D_L16_001D9FC0, D_L16_001DA6B0, 0};

extern float D_L16_001DA7E0[];
extern float D_L16_001DAC80[];
extern float D_L16_001DB120[];
float       *D_L16_00161F10[4]
    __attribute__((section(".sdata"))) = {D_L16_001DA7E0, D_L16_001DAC80, D_L16_001DB120, 0};

extern float D_L16_001DB1E8[];
extern float D_L16_001DB8D8[];
extern float D_L16_001DBFC8[];
float       *D_L16_00161F20[4]
    __attribute__((section(".sdata"))) = {D_L16_001DB1E8, D_L16_001DB8D8, D_L16_001DBFC8, 0};

extern float D_L16_001DC0F8[];
extern float D_L16_001DC348[];
extern float D_L16_001DC598[];
float       *D_L16_00161F30[4]
    __attribute__((section(".sdata"))) = {D_L16_001DC0F8, D_L16_001DC348, D_L16_001DC598, 0};

extern void FUN_L00_001fde98(int, int, int, char *, int);

void env_map_a_draw(void) __asm__("FUN_L16_002e8d40");

void env_map_a_draw(void) {
    int *mesh_point_counts = (int *)D_L16_00161EF0_gp;
    int *mesh_point_positions = (int *)D_L16_00161F00_gp;
    int *mesh_unused_reflection_arguments = (int *)D_L16_00161F10;
    int *mesh_direction_vectors = (int *)D_L16_00161F20;
    int *mesh_point_word_streams = (int *)D_L16_00161F30;
    int  i;

    vu1_add_g_sregister(6, get_effect_texture(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        if (fast_bsphere_check(D_L16_001DC600[i], 512.0f) != -1) {
            compute_level16_reflection_map_coordinates(mesh_point_counts[i],
                                                       mesh_direction_vectors[i],
                                                       mesh_point_positions[i],
                                                       mesh_unused_reflection_arguments[i],
                                                       D_L16_001DC630);
            FUN_L00_001fde98(mesh_point_counts[i],
                             mesh_point_positions[i],
                             mesh_point_word_streams[i],
                             D_L16_001DC630,
                             1);
        }
    }
}

/* Bounding spheres for one mesh (three entries), then the renderer callback; same source as FUN_L16_002e93b0 without the 0x30/0x32 stores. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2e8e80;

extern int                    D_L16_00161EF0_gp[] __asm__("D_L16_00161EF0") __attribute__((sda));
extern float                 *D_L16_00161F00_gp[] __asm__("D_L16_00161F00") __attribute__((sda));
extern L16SingleSphere_2e8e80 D_L16_00161F60_bounds_2e8e80[] __asm__("D_L16_001DC600")
    __attribute__((section(".data")));

void update_reflection_mesh_trio(struct Moby *moby) __asm__("FUN_L16_002e8e80");

void update_reflection_mesh_trio(struct Moby *moby) {
    enum {
        MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 2; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00161EF0_gp[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00161F00_gp)[i] + k * 3;
                    x = verts[0];
                    if (x > maxx)
                        maxx = x;
                    y = verts[1];
                    if (y > maxy)
                        maxy = y;
                    z = verts[2];
                    if (z > maxz)
                        maxz = z;
                    if (x < minx)
                        minx = x;
                    if (y < miny)
                        miny = y;
                    if (z < minz)
                        minz = z;
                }
            }
            {
                L16SingleSphere_2e8e80 *base = D_L16_00161F60_bounds_2e8e80;
                L16SingleSphere_2e8e80 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00161EF0_gp[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(D_L16_00161F60_bounds_2e8e80 + i);
                vertex_position[0] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 0];
                vertex_position[1] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 1];
                vertex_position[2] = ((float **)D_L16_00161F00_gp)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        break;
    }
    case MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(env_map_a_draw, moby);
        break;
    }
}
