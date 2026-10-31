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

    int   point_count,
    Vec3 *direction_vectors,
    Vec3 *point_positions,
    int   unused,

    float *reflection_map_coordinates) {
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
        reflection_map_coordinates[i * 2] =
            (reflection_projection_scratch[0] / projection_denominator + 0.5f) * 2.0f;
        reflection_map_coordinates[i * 2 + 1] =
            (reflection_projection_scratch[1] / projection_denominator + 0.5f) * 2.0f;
    }
}

#include "sda.h"

extern int get_effect_texture(int) __asm__("FUN_001f44b8");
/* Sets up the draw state, then draws the three entries of the level's table. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA1B8), where it is exact; names translated to the US level program. */

extern char D_L16_001DC600[][16] NOT_SDA;
extern char D_L16_001DC630[] NOT_SDA;
int         reflection_mesh_trio_vertex_counts[4] __asm__("D_L16_00161EF0")
    __attribute__((section(".sdata"))) = {148, 148, 25, 0};

extern float
    reflection_mesh_trio_positions_0[444] __asm__("D_L16_001D98D0"); /* 148 points x 3 floats */
extern float
    reflection_mesh_trio_positions_1[444] __asm__("D_L16_001D9FC0"); /* 148 points x 3 floats */
extern float
       reflection_mesh_trio_positions_2[75] __asm__("D_L16_001DA6B0"); /* 25 points x 3 floats */
float *reflection_mesh_trio_position_tables[4] __asm__("D_L16_00161F00")

    __attribute__((section(".sdata"))) = {reflection_mesh_trio_positions_0,
                                          reflection_mesh_trio_positions_1,
                                          reflection_mesh_trio_positions_2,
                                          0};

extern float D_L16_001DA7E0[];
extern float D_L16_001DAC80[];
extern float D_L16_001DB120[];
float       *reflection_mesh_trio_unused_pair_tables[4] __asm__("D_L16_00161F10")
    __attribute__((section(".sdata"))) = {D_L16_001DA7E0, D_L16_001DAC80, D_L16_001DB120, 0};

extern float
    reflection_mesh_trio_directions_0[444] __asm__("D_L16_001DB1E8"); /* 148 points x 3 floats */
extern float
    reflection_mesh_trio_directions_1[444] __asm__("D_L16_001DB8D8"); /* 148 points x 3 floats */
extern float
       reflection_mesh_trio_directions_2[75] __asm__("D_L16_001DBFC8"); /* 25 points x 3 floats */
float *reflection_mesh_trio_direction_tables[4] __asm__("D_L16_00161F20")

    __attribute__((section(".sdata"))) = {reflection_mesh_trio_directions_0,
                                          reflection_mesh_trio_directions_1,
                                          reflection_mesh_trio_directions_2,
                                          0};

extern u32 reflection_mesh_trio_colours_0[148] __asm__("D_L16_001DC0F8"); /* 148 RGBA words, one per point */
extern u32 reflection_mesh_trio_colours_1[148] __asm__("D_L16_001DC348"); /* 148 RGBA words, one per point */
extern u32
    reflection_mesh_trio_colours_2[25] __asm__("D_L16_001DC598"); /* 25 RGBA words, one per point */
u32 *reflection_mesh_trio_colour_tables[4] __asm__("D_L16_00161F30")

    __attribute__((section(".sdata"))) = {reflection_mesh_trio_colours_0,
                                          reflection_mesh_trio_colours_1,
                                          reflection_mesh_trio_colours_2,
                                          0};

extern void FUN_L00_001fde98(int, int, int, char *, int);

void env_map_a_draw(void) __asm__("FUN_L16_002e8d40");

void env_map_a_draw(void) {
    int *mesh_point_counts = (int *)reflection_mesh_trio_vertex_counts;
    int *mesh_point_positions = (int *)reflection_mesh_trio_position_tables;
    int *mesh_unused_reflection_arguments = (int *)reflection_mesh_trio_unused_pair_tables;
    int *mesh_direction_vectors = (int *)reflection_mesh_trio_direction_tables;
    int *mesh_point_word_streams = (int *)reflection_mesh_trio_colour_tables;
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

extern int    reflection_mesh_trio_vertex_counts[] __asm__("D_L16_00161EF0") __attribute__((sda));
extern float *reflection_mesh_trio_position_tables[] __asm__("D_L16_00161F00") __attribute__((sda));
extern L16SingleSphere_2e8e80 D_L16_00161F60_bounds_2e8e80[] __asm__("D_L16_001DC600")
    __attribute__((section(".data")));

void reflection_mesh_trio_update(struct Moby *moby) __asm__("FUN_L16_002e8e80");

/* Moby class 1943 */
void reflection_mesh_trio_update(struct Moby *moby) {
    enum {
        MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *mesh_bounding_sphere_components;
    float *verts;
    int    count;
    switch (moby->state) {
    case MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 2; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < reflection_mesh_trio_vertex_counts[i]; k++) {
                    float x, y, z;
                    verts = ((float **)reflection_mesh_trio_position_tables)[i] + k * 3;
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
                L16SingleSphere_2e8e80 *mesh_bounding_spheres = D_L16_00161F60_bounds_2e8e80;
                L16SingleSphere_2e8e80 *sphere = mesh_bounding_spheres + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < reflection_mesh_trio_vertex_counts[i]; j++) {
                float vertex_to_center_distance;
                mesh_bounding_sphere_components = (float *)(D_L16_00161F60_bounds_2e8e80 + i);
                vertex_position[0] = ((float **)reflection_mesh_trio_position_tables)[i][j * 3 + 0];
                vertex_position[1] = ((float **)reflection_mesh_trio_position_tables)[i][j * 3 + 1];
                vertex_position[2] = ((float **)reflection_mesh_trio_position_tables)[i][j * 3 + 2];
                vertex_to_center_distance =
                    fast_vec_dist(vertex_position, mesh_bounding_sphere_components);
                if (vertex_to_center_distance > mesh_bounding_sphere_components[3])
                    mesh_bounding_sphere_components[3] = vertex_to_center_distance;
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

float reflection_mesh_trio_positions_0[444] __asm__("D_L16_001D98D0") = {
    154.81554f, 252.11551f, 1.2e+02f,   151.8481f,  253.34462f, 1.2e+02f,   155.45485f, 254.50136f,
    124.0f,     153.08313f, 255.48375f, 124.0f,     154.81555f, 252.11552f, 128.0f,     151.84811f,
    253.34465f, 128.0f,     154.81555f, 252.11552f, 128.0f,     153.08313f, 255.48375f, 132.0f,
    154.81555f, 252.11552f, 128.0f,     155.45483f, 254.50136f, 132.0f,     158.0f,     251.69624f,
    128.0f,     158.0f,     254.16629f, 132.0f,     161.18443f, 252.11552f, 128.0f,     160.54515f,
    254.50134f, 132.0f,     161.18443f, 252.11552f, 128.0f,     162.91685f, 255.48375f, 132.0f,
    161.18443f, 252.11552f, 128.0f,     164.15187f, 253.34465f, 128.0f,     160.54514f, 254.50136f,
    124.0f,     162.91685f, 255.48375f, 124.0f,     161.18442f, 252.11551f, 1.2e+02f,   162.91685f,
    255.48375f, 124.0f,     164.15186f, 253.34465f, 1.2e+02f,   164.95349f, 257.04602f, 124.0f,
    166.70004f, 255.29996f, 1.2e+02f,   166.51627f, 259.08215f, 124.0f,     168.65535f, 257.8481f,
    1.2e+02f,   166.51627f, 259.08215f, 124.0f,     169.88449f, 260.81555f, 1.2e+02f,   166.51627f,
    259.08215f, 124.0f,     167.49867f, 261.45386f, 124.0f,     166.51627f, 259.08215f, 124.0f,
    169.88449f, 260.8149f,  128.0f,     166.51627f, 259.08215f, 124.0f,     168.65536f, 257.8471f,
    128.0f,     164.95349f, 257.04602f, 124.0f,     166.70006f, 255.29945f, 128.0f,     162.91685f,
    255.48375f, 124.0f,     166.70006f, 255.29945f, 128.0f,     164.15187f, 253.34465f, 128.0f,
    166.70006f, 255.29945f, 128.0f,     162.91685f, 255.48375f, 132.0f,     166.70006f, 255.29945f,
    128.0f,     164.95349f, 257.04602f, 132.0f,     168.65536f, 257.8471f,  128.0f,     166.51627f,
    259.08215f, 132.0f,     169.88449f, 260.8149f,  128.0f,     167.49863f, 261.45383f, 132.0f,
    170.30371f, 264.0f,     128.0f,     167.83366f, 263.999f,   132.0f,     169.88449f, 267.18378f,
    128.0f,     167.49863f, 266.54413f, 132.0f,     169.88449f, 267.18378f, 128.0f,     166.51627f,
    268.91586f, 132.0f,     169.88449f, 267.18378f, 128.0f,     168.65536f, 270.15085f, 128.0f,
    167.49867f, 266.54413f, 124.0f,     166.51627f, 268.91586f, 124.0f,     169.88452f, 267.18442f,
    1.2e+02f,   166.51627f, 268.91586f, 124.0f,     168.65538f, 270.15186f, 1.2e+02f,   164.95349f,
    270.95297f, 124.0f,     166.70007f, 272.70004f, 1.2e+02f,   162.91685f, 272.51627f, 124.0f,
    164.15189f, 274.65533f, 1.2e+02f,   160.54515f, 273.49863f, 124.0f,     161.18445f, 275.88446f,
    1.2e+02f,   158.0f,     273.83368f, 124.0f,     158.0f,     276.3037f,  1.2e+02f,   158.0f,
    273.83368f, 124.0f,     154.81554f, 275.88446f, 1.2e+02f,   158.0f,     273.83368f, 124.0f,
    155.45483f, 273.4986f,  124.0f,     158.0f,     273.83368f, 124.0f,     154.81555f, 275.88446f,
    128.0f,     158.0f,     273.83368f, 124.0f,     158.0f,     276.3037f,  128.0f,     160.54515f,
    273.49863f, 124.0f,     161.18443f, 275.88446f, 128.0f,     162.91685f, 272.51627f, 124.0f,
    164.15187f, 274.65536f, 128.0f,     164.95349f, 270.95297f, 124.0f,     166.70006f, 272.69955f,
    128.0f,     166.51627f, 268.91586f, 124.0f,     166.70006f, 272.69955f, 128.0f,     168.65536f,
    270.15085f, 128.0f,     166.70006f, 272.69955f, 128.0f,     166.51627f, 268.91586f, 132.0f,
    166.70006f, 272.69955f, 128.0f,     164.95349f, 270.95297f, 132.0f,     164.15187f, 274.65536f,
    128.0f,     162.91685f, 272.51627f, 132.0f,     161.18443f, 275.88446f, 128.0f,     160.54515f,
    273.49863f, 132.0f,     158.0f,     276.3037f,  128.0f,     158.0f,     273.83368f, 132.0f,
    154.81555f, 275.88446f, 128.0f,     155.45483f, 273.49863f, 132.0f,     151.84811f, 274.6554f,
    128.0f,     153.08313f, 272.51627f, 132.0f,     149.29994f, 272.6994f,  128.0f,     151.0465f,
    270.95297f, 132.0f,     149.29994f, 272.6994f,  128.0f,     149.48375f, 268.91586f, 132.0f,
    149.29994f, 272.6994f,  128.0f,     147.34464f, 270.15085f, 128.0f,     151.04652f, 270.95248f,
    124.0f,     149.48375f, 268.91586f, 124.0f,     149.29994f, 272.69803f, 1.2e+02f,   149.48375f,
    268.91586f, 124.0f,     147.34464f, 270.14987f, 1.2e+02f,   149.48375f, 268.91586f, 124.0f,
    146.1155f,  267.1824f,  1.2e+02f,   149.48375f, 268.91586f, 124.0f,     148.50136f, 266.54413f,
    124.0f,     147.34464f, 270.15085f, 128.0f,     146.11551f, 267.1831f,  128.0f,     149.48375f,
    268.91586f, 132.0f,     146.11551f, 267.1831f,  128.0f,     148.50136f, 266.54413f, 132.0f,
    145.69624f, 263.99796f, 128.0f,     148.16629f, 263.999f,   132.0f,     146.11551f, 260.8142f,
    128.0f,     148.50136f, 261.45383f, 132.0f,     146.11551f, 260.8142f,  128.0f,     149.48375f,
    259.08215f, 132.0f,     146.11551f, 260.8142f,  128.0f,     147.34464f, 257.8471f,  128.0f,
    148.50136f, 261.45386f, 124.0f,     149.48375f, 259.08215f, 124.0f,     146.1155f,  260.81354f,
    1.2e+02f,   149.48375f, 259.08215f, 124.0f,     147.34464f, 257.8461f,  1.2e+02f,   151.0465f,
    257.04602f, 124.0f,     149.29993f, 255.29893f, 1.2e+02f,   151.0465f,  257.04602f, 124.0f,
    151.8481f,  253.34462f, 1.2e+02f,   151.0465f,  257.04602f, 124.0f,     153.08313f, 255.48375f,
    124.0f,     151.0465f,  257.04602f, 124.0f,     151.84811f, 253.34465f, 128.0f,     151.0465f,
    257.04602f, 124.0f,     149.29994f, 255.29945f, 128.0f,     149.48375f, 259.08215f, 124.0f,
    149.29994f, 255.29945f, 128.0f,     147.34464f, 257.8471f,  128.0f,     149.29994f, 255.29945f,
    128.0f,     149.48375f, 259.08215f, 132.0f};

float reflection_mesh_trio_positions_1[444] __asm__("D_L16_001D9FC0") = {
    149.29994f, 255.29945f, 128.0f,     149.48375f, 259.08215f, 132.0f,     149.29994f, 255.29945f,
    128.0f,     151.0465f,  257.04602f, 132.0f,     151.84811f, 253.34465f, 128.0f,     151.0465f,
    257.04602f, 132.0f,     153.08313f, 255.48375f, 132.0f,     149.29994f, 255.29945f, 136.0f,
    151.84811f, 253.34465f, 136.0f,     151.04651f, 257.0455f,  1.4e+02f,   151.84811f, 253.34465f,
    136.0f,     153.08311f, 255.48375f, 1.4e+02f,   151.84811f, 253.34465f, 136.0f,     155.45483f,
    254.50136f, 1.4e+02f,   151.84811f, 253.34465f, 136.0f,     154.81555f, 252.11551f, 136.0f,
    153.08313f, 255.48375f, 132.0f,     154.81555f, 252.11551f, 136.0f,     155.45483f, 254.50136f,
    132.0f,     154.81555f, 252.11551f, 136.0f,     158.0f,     254.16629f, 132.0f,     154.81555f,
    252.11551f, 136.0f,     158.0f,     251.69624f, 136.0f,     155.45483f, 254.50136f, 1.4e+02f,
    158.0f,     251.69624f, 136.0f,     158.0f,     254.16629f, 1.4e+02f,   158.0f,     251.69624f,
    136.0f,     160.54515f, 254.50136f, 1.4e+02f,   158.0f,     251.69624f, 136.0f,     161.18443f,
    252.11551f, 136.0f,     158.0f,     254.16629f, 132.0f,     161.18443f, 252.11551f, 136.0f,
    160.54515f, 254.50134f, 132.0f,     161.18443f, 252.11551f, 136.0f,     162.91685f, 255.48375f,
    132.0f,     161.18443f, 252.11551f, 136.0f,     164.15187f, 253.34465f, 136.0f,     160.54515f,
    254.50136f, 1.4e+02f,   164.15187f, 253.34465f, 136.0f,     162.91684f, 255.48375f, 1.4e+02f,
    164.15187f, 253.34465f, 136.0f,     164.95348f, 257.0465f,  1.4e+02f,   164.15187f, 253.34465f,
    136.0f,     166.70006f, 255.29945f, 136.0f,     162.91685f, 255.48375f, 132.0f,     166.70006f,
    255.29945f, 136.0f,     164.95349f, 257.04602f, 132.0f,     166.70006f, 255.29945f, 136.0f,
    166.51627f, 259.08215f, 132.0f,     166.70006f, 255.29945f, 136.0f,     168.65536f, 257.8471f,
    136.0f,     164.95348f, 257.0465f,  1.4e+02f,   168.65536f, 257.8471f,  136.0f,     166.51625f,
    259.08313f, 1.4e+02f,   168.65536f, 257.8471f,  136.0f,     167.49863f, 261.45483f, 1.4e+02f,
    168.65536f, 257.8471f,  136.0f,     169.88449f, 260.81454f, 136.0f,     166.51627f, 259.08215f,
    132.0f,     169.88449f, 260.81454f, 136.0f,     167.49863f, 261.45383f, 132.0f,     169.88449f,
    260.81454f, 136.0f,     167.83366f, 263.999f,   132.0f,     169.88449f, 260.81454f, 136.0f,
    170.30371f, 263.999f,   136.0f,     167.49863f, 261.45483f, 1.4e+02f,   170.30371f, 263.999f,
    136.0f,     167.83366f, 264.0f,     1.4e+02f,   170.30371f, 263.999f,   136.0f,     167.49863f,
    266.54514f, 1.4e+02f,   170.30371f, 263.999f,   136.0f,     169.88449f, 267.18344f, 136.0f,
    167.83366f, 263.999f,   132.0f,     169.88449f, 267.18344f, 136.0f,     167.49863f, 266.54413f,
    132.0f,     169.88449f, 267.18344f, 136.0f,     166.51627f, 268.91586f, 132.0f,     169.88449f,
    267.18344f, 136.0f,     168.65536f, 270.15085f, 136.0f,     167.49863f, 266.54514f, 1.4e+02f,
    168.65536f, 270.15085f, 136.0f,     166.51628f, 268.91684f, 1.4e+02f,   168.65536f, 270.15085f,
    136.0f,     164.9535f,  270.95346f, 1.4e+02f,   168.65536f, 270.15085f, 136.0f,     166.70006f,
    272.69955f, 136.0f,     166.51627f, 268.91586f, 132.0f,     166.70006f, 272.69955f, 136.0f,
    164.95349f, 270.95297f, 132.0f,     166.70006f, 272.69955f, 136.0f,     162.91685f, 272.51627f,
    132.0f,     166.70006f, 272.69955f, 136.0f,     164.15187f, 274.65536f, 136.0f,     164.9535f,
    270.95346f, 1.4e+02f,   164.15187f, 274.65536f, 136.0f,     162.91687f, 272.51624f, 1.4e+02f,
    164.15187f, 274.65536f, 136.0f,     160.54515f, 273.49863f, 1.4e+02f,   164.15187f, 274.65536f,
    136.0f,     161.18443f, 275.88446f, 136.0f,     162.91685f, 272.51627f, 132.0f,     161.18443f,
    275.88446f, 136.0f,     160.54515f, 273.49863f, 132.0f,     161.18443f, 275.88446f, 136.0f,
    158.0f,     273.83368f, 132.0f,     161.18443f, 275.88446f, 136.0f,     158.0f,     276.3037f,
    136.0f,     160.54515f, 273.49863f, 1.4e+02f,   158.0f,     276.3037f,  136.0f,     158.0f,
    273.83368f, 1.4e+02f,   158.0f,     276.3037f,  136.0f,     155.45483f, 273.49863f, 1.4e+02f,
    158.0f,     276.3037f,  136.0f,     154.81555f, 275.8845f,  136.0f,     158.0f,     273.83368f,
    132.0f,     154.81555f, 275.8845f,  136.0f,     155.45483f, 273.49863f, 132.0f,     154.81555f,
    275.8845f,  136.0f,     153.08313f, 272.51627f, 132.0f,     154.81555f, 275.8845f,  136.0f,
    151.84811f, 274.65536f, 136.0f,     155.45483f, 273.49863f, 1.4e+02f,   151.84811f, 274.65536f,
    136.0f,     153.08315f, 272.5163f,  1.4e+02f,   151.84811f, 274.65536f, 136.0f,     151.0465f,
    270.95248f, 1.4e+02f,   151.84811f, 274.65536f, 136.0f,     149.29994f, 272.69955f, 136.0f,
    153.08313f, 272.51627f, 132.0f,     149.29994f, 272.69955f, 136.0f,     151.0465f,  270.95297f,
    132.0f,     149.29994f, 272.69955f, 136.0f,     149.48375f, 268.91586f, 132.0f,     149.29994f,
    272.69955f, 136.0f,     147.34464f, 270.15085f, 136.0f,     151.0465f,  270.95248f, 1.4e+02f,
    147.34464f, 270.15085f, 136.0f,     149.48375f, 268.91483f, 1.4e+02f,   147.34464f, 270.15085f,
    136.0f,     148.50136f, 266.54312f, 1.4e+02f,   147.34464f, 270.15085f, 136.0f,     146.11551f,
    267.18344f, 136.0f,     149.48375f, 268.91586f, 132.0f,     146.11551f, 267.18344f, 136.0f,
    148.50136f, 266.54413f, 132.0f,     146.11551f, 267.18344f, 136.0f,     148.16629f, 263.999f,
    132.0f,     146.11551f, 267.18344f, 136.0f};

float reflection_mesh_trio_positions_2[75] __asm__("D_L16_001DA6B0") = {
    148.16629f, 263.999f,   132.0f,     146.11551f, 267.18344f, 136.0f,     145.69624f, 263.999f,
    136.0f,     148.50136f, 266.54312f, 1.4e+02f,   145.69624f, 263.999f,   136.0f,     148.16629f,
    263.99796f, 1.4e+02f,   145.69624f, 263.999f,   136.0f,     148.50136f, 261.45282f, 1.4e+02f,
    145.69624f, 263.999f,   136.0f,     146.11551f, 260.81454f, 136.0f,     148.16629f, 263.999f,
    132.0f,     146.11551f, 260.81454f, 136.0f,     148.50136f, 261.45383f, 132.0f,     147.34464f,
    257.8471f,  136.0f,     149.48375f, 259.08215f, 132.0f,     147.34464f, 257.8471f,  136.0f,
    151.0465f,  257.04602f, 132.0f,     147.34464f, 257.8471f,  136.0f,     149.29994f, 255.29945f,
    136.0f,     147.34464f, 257.8471f,  136.0f,     151.04651f, 257.0455f,  1.4e+02f,   147.34464f,
    257.8471f,  136.0f,     149.48375f, 259.08112f, 1.4e+02f,   146.11551f, 260.81454f, 136.0f,
    148.50136f, 261.45282f, 1.4e+02f};

float reflection_mesh_trio_directions_0[444] __asm__("D_L16_001DB1E8") = {
    -0.326366f, -0.787943f, 0.522141f,  -0.422693f, -0.732339f, 0.522149f,  -0.326367f, -0.787943f,
    1e-06f,     -0.422717f, -0.732326f, 7e-06f,     -0.25469f,  -0.807149f, 0.174047f,  -0.422739f,
    -0.732312f, 3e-06f,     -0.25469f,  -0.807149f, 0.174047f,  -0.42274f,  -0.732312f, 3e-06f,
    -0.25469f,  -0.807149f, 0.174047f,  -0.21885f,  -0.816752f, 0.0f,       1e-06f,     -0.845563f,
    0.522139f,  0.0f,       -0.845562f, -1e-06f,    0.25469f,   -0.80715f,  0.174046f,  0.218851f,
    -0.816753f, -1e-06f,    0.25469f,   -0.80715f,  0.174046f,  0.422739f,  -0.732312f, -2e-06f,
    0.25469f,   -0.80715f,  0.174046f,  0.422738f,  -0.732313f, -3e-06f,    0.326371f,  -0.787943f,
    1e-06f,     0.422765f,  -0.732303f, -1.7e-05f,  0.326373f,  -0.787941f, 0.522139f,  0.422765f,
    -0.732303f, -1.7e-05f,  0.422794f,  -0.732295f, 0.522108f,  0.597885f,  -0.597953f, -4.2e-05f,
    0.597937f,  -0.597939f, 0.522061f,  0.732289f,  -0.422802f, -4.1e-05f,  0.732319f,  -0.422809f,
    0.522061f,  0.732289f,  -0.422802f, -4.1e-05f,  0.787977f,  -0.326388f, 0.522076f,  0.732289f,
    -0.422802f, -4.1e-05f,  0.787967f,  -0.326361f, -3.2e-05f,  0.732289f,  -0.422802f, -4.1e-05f,
    0.807167f,  -0.254656f, 0.174036f,  0.732289f,  -0.422802f, -4.1e-05f,  0.732263f,  -0.422798f,
    -6e-06f,    0.597885f,  -0.597953f, -4.2e-05f,  0.597835f,  -0.59797f,  -5e-06f,    0.422765f,
    -0.732303f, -1.7e-05f,  0.597835f,  -0.59797f,  -5e-06f,    0.422738f,  -0.732313f, -3e-06f,
    0.597835f,  -0.59797f,  -5e-06f,    0.422739f,  -0.732312f, -2e-06f,    0.597835f,  -0.59797f,
    -5e-06f,    0.597835f,  -0.59797f,  -5e-06f,    0.732263f,  -0.422798f, -6e-06f,    0.732259f,
    -0.422806f, -7e-06f,    0.807167f,  -0.254656f, 0.174036f,  0.816762f,  -0.218829f, -9e-06f,
    0.845566f,  2.2e-05f,   0.522137f,  0.845564f,  1.1e-05f,   -3e-06f,    0.807134f,  0.254714f,
    0.174045f,  0.816746f,  0.218857f,  1e-06f,     0.807134f,  0.254714f,  0.174045f,  0.732301f,
    0.422756f,  -2e-06f,    0.807134f,  0.254714f,  0.174045f,  0.732296f,  0.422763f,  -5e-06f,
    0.787914f,  0.326379f,  2.3e-05f,   0.732272f,  0.422764f,  2.3e-05f,   0.787907f,  0.326359f,
    0.5222f,    0.732272f,  0.422764f,  2.3e-05f,   0.73225f,   0.422765f,  0.522195f,  0.59793f,
    0.597866f,  9e-06f,     0.597891f,  0.597896f,  0.522164f,  0.422798f,  0.732265f,  -3e-06f,
    0.422779f,  0.732282f,  0.522138f,  0.218845f,  0.816754f,  1e-06f,     0.218846f,  0.816753f,
    0.522139f,  0.0f,       0.845563f,  1e-06f,     0.0f,       0.845562f,  0.522141f,  0.0f,
    0.845563f,  1e-06f,     -0.111322f, 0.845562f,  0.522141f,  0.0f,       0.845563f,  1e-06f,
    -0.111323f, 0.845562f,  -0.0f,      0.0f,       0.845563f,  1e-06f,     -0.183f,    0.826358f,
    0.174046f,  0.0f,       0.845563f,  1e-06f,     0.0f,       0.845564f,  -0.0f,      0.218845f,
    0.816754f,  1e-06f,     0.218843f,  0.816755f,  1e-06f,     0.422798f,  0.732265f,  -3e-06f,
    0.422819f,  0.732249f,  -4e-06f,    0.59793f,   0.597866f,  9e-06f,     0.597972f,  0.597838f,
    -5e-06f,    0.732272f,  0.422764f,  2.3e-05f,   0.597972f,  0.597838f,  -5e-06f,    0.732296f,
    0.422763f,  -5e-06f,    0.597972f,  0.597838f,  -5e-06f,    0.732301f,  0.422756f,  -2e-06f,
    0.597972f,  0.597838f,  -5e-06f,    0.597972f,  0.597838f,  -5e-06f,    0.422819f,  0.732249f,
    -4e-06f,    0.422819f,  0.732249f,  -4e-06f,    0.218843f,  0.816755f,  1e-06f,     0.218843f,
    0.816755f,  1e-06f,     0.0f,       0.845564f,  -0.0f,      2e-06f,     0.845564f,  -0.0f,
    -0.183f,    0.826358f,  0.174046f,  -0.218841f, 0.816754f,  -1e-06f,    -0.422833f, 0.732239f,
    0.522137f,  -0.422828f, 0.732244f,  -0.0f,      -0.6242f,   0.571602f,  0.174034f,  -0.597978f,
    0.597839f,  1e-06f,     -0.6242f,   0.571602f,  0.174034f,  -0.732307f, 0.422746f,  -2e-06f,
    -0.6242f,   0.571602f,  0.174034f,  -0.732302f, 0.42274f,   -1.1e-05f,  -0.676649f, 0.519181f,
    -6.2e-05f,  -0.732307f, 0.422771f,  -4.5e-05f,  -0.67666f,  0.519229f,  0.522047f,  -0.732307f,
    0.422771f,  -4.5e-05f,  -0.732317f, 0.422806f,  0.522064f,  -0.732307f, 0.422771f,  -4.5e-05f,
    -0.787975f, 0.326384f,  0.522081f,  -0.732307f, 0.422771f,  -4.5e-05f,  -0.787965f, 0.326362f,
    -2.8e-05f,  -0.732302f, 0.42274f,   -1.1e-05f,  -0.807165f, 0.254663f,  0.174036f,  -0.732307f,
    0.422746f,  -2e-06f,    -0.807165f, 0.254663f,  0.174036f,  -0.816761f, 0.218837f,  -8e-06f,
    -0.845565f, -2.3e-05f,  0.522136f,  -0.845564f, -1.2e-05f,  -3e-06f,    -0.807133f, -0.254715f,
    0.174045f,  -0.816746f, -0.218862f, 3e-06f,     -0.807133f, -0.254715f, 0.174045f,  -0.732249f,
    -0.422822f, 2e-06f,     -0.807133f, -0.254715f, 0.174045f,  -0.732243f, -0.422828f, -2e-06f,
    -0.787912f, -0.326377f, 2.3e-05f,   -0.73222f,  -0.422828f, 2.6e-05f,   -0.787905f, -0.326358f,
    0.522204f,  -0.73222f,  -0.422828f, 2.6e-05f,   -0.732198f, -0.422828f, 0.522201f,  -0.597797f,
    -0.597994f, 2.1e-05f,   -0.597755f, -0.598016f, 0.522178f,  -0.597797f, -0.597994f, 2.1e-05f,
    -0.422693f, -0.732339f, 0.522149f,  -0.597797f, -0.597994f, 2.1e-05f,   -0.422717f, -0.732326f,
    7e-06f,     -0.597797f, -0.597994f, 2.1e-05f,   -0.422739f, -0.732312f, 3e-06f,     -0.597797f,
    -0.597994f, 2.1e-05f,   -0.597836f, -0.59797f,  4e-06f,     -0.73222f,  -0.422828f, 2.6e-05f,
    -0.597836f, -0.59797f,  4e-06f,     -0.732243f, -0.422828f, -2e-06f,    -0.597836f, -0.59797f,
    4e-06f,     -0.732249f, -0.422822f, 2e-06f};

float reflection_mesh_trio_directions_1[444] __asm__("D_L16_001DB8D8") = {
    -0.597836f, -0.59797f,  4e-06f,     -0.732249f, -0.422822f, 2e-06f,     -0.597836f, -0.59797f,
    4e-06f,     -0.597836f, -0.59797f,  4e-06f,     -0.422739f, -0.732312f, 3e-06f,     -0.597836f,
    -0.59797f,  4e-06f,     -0.42274f,  -0.732312f, 3e-06f,     -0.597842f, -0.597974f, -8e-06f,
    -0.422741f, -0.732311f, 3e-06f,     -0.597846f, -0.597977f, 0.52212f,   -0.422741f, -0.732311f,
    3e-06f,     -0.42274f,  -0.73231f,  0.522142f,  -0.422741f, -0.732311f, 3e-06f,     -0.21885f,
    -0.816751f, 0.522141f,  -0.422741f, -0.732311f, 3e-06f,     -0.21885f,  -0.816752f, 1e-06f,
    -0.42274f,  -0.732312f, 3e-06f,     -0.21885f,  -0.816752f, 1e-06f,     -0.21885f,  -0.816752f,
    0.0f,       -0.21885f,  -0.816752f, 1e-06f,     0.0f,       -0.845562f, -1e-06f,    -0.21885f,
    -0.816752f, 1e-06f,     0.0f,       -0.845562f, -0.0f,      -0.21885f,  -0.816751f, 0.522141f,
    0.0f,       -0.845562f, -0.0f,      0.0f,       -0.845562f, 0.522141f,  0.0f,       -0.845562f,
    -0.0f,      0.21885f,   -0.816752f, 0.52214f,   0.0f,       -0.845562f, -0.0f,      0.21885f,
    -0.816752f, -0.0f,      0.0f,       -0.845562f, -1e-06f,    0.21885f,   -0.816752f, -0.0f,
    0.218851f,  -0.816753f, -1e-06f,    0.21885f,   -0.816752f, -0.0f,      0.422739f,  -0.732312f,
    -2e-06f,    0.21885f,   -0.816752f, -0.0f,      0.422734f,  -0.732304f, 1.3e-05f,   0.21885f,
    -0.816752f, 0.52214f,   0.422734f,  -0.732304f, 1.3e-05f,   0.422729f,  -0.732298f, 0.522168f,
    0.422734f,  -0.732304f, 1.3e-05f,   0.597803f,  -0.59794f,  0.522213f,  0.422734f,  -0.732304f,
    1.3e-05f,   0.597818f,  -0.597954f, 3.4e-05f,   0.422739f,  -0.732312f, -2e-06f,    0.597818f,
    -0.597954f, 3.4e-05f,   0.597835f,  -0.59797f,  -5e-06f,    0.597818f,  -0.597954f, 3.4e-05f,
    0.732259f,  -0.422806f, -7e-06f,    0.597818f,  -0.597954f, 3.4e-05f,   0.732233f,  -0.422801f,
    3.7e-05f,   0.597803f,  -0.59794f,  0.522213f,  0.732233f,  -0.422801f, 3.7e-05f,   0.732214f,
    -0.42279f,  0.522216f,  0.732233f,  -0.422801f, 3.7e-05f,   0.816729f,  -0.218834f, 0.522183f,
    0.732233f,  -0.422801f, 3.7e-05f,   0.816741f,  -0.218838f, 2.1e-05f,   0.732259f,  -0.422806f,
    -7e-06f,    0.816741f,  -0.218838f, 2.1e-05f,   0.816762f,  -0.218829f, -9e-06f,    0.816741f,
    -0.218838f, 2.1e-05f,   0.845564f,  1.1e-05f,   -3e-06f,    0.816741f,  -0.218838f, 2.1e-05f,
    0.845563f,  1e-06f,     0.0f,       0.816729f,  -0.218834f, 0.522183f,  0.845563f,  1e-06f,
    0.0f,       0.845563f,  2e-06f,     0.522143f,  0.845563f,  1e-06f,     0.0f,       0.816776f,
    0.218852f,  0.522099f,  0.845563f,  1e-06f,     0.0f,       0.816763f,  0.218847f,  -2.2e-05f,
    0.845564f,  1.1e-05f,   -3e-06f,    0.816763f,  0.218847f,  -2.2e-05f,  0.816746f,  0.218857f,
    1e-06f,     0.816763f,  0.218847f,  -2.2e-05f,  0.732301f,  0.422756f,  -2e-06f,    0.816763f,
    0.218847f,  -2.2e-05f,  0.732319f,  0.422756f,  -3e-05f,    0.816776f,  0.218852f,  0.522099f,
    0.732319f,  0.422756f,  -3e-05f,    0.732335f,  0.422765f,  0.522083f,  0.732319f,  0.422756f,
    -3e-05f,    0.597985f,  0.597849f,  0.522112f,  0.732319f,  0.422756f,  -3e-05f,    0.597977f,
    0.597843f,  -1.6e-05f,  0.732301f,  0.422756f,  -2e-06f,    0.597977f,  0.597843f,  -1.6e-05f,
    0.597972f,  0.597838f,  -5e-06f,    0.597977f,  0.597843f,  -1.6e-05f,  0.422819f,  0.732249f,
    -4e-06f,    0.597977f,  0.597843f,  -1.6e-05f,  0.422818f,  0.732249f,  -3e-06f,    0.597985f,
    0.597849f,  0.522112f,  0.422818f,  0.732249f,  -3e-06f,    0.42282f,   0.732251f,  0.522137f,
    0.422818f,  0.732249f,  -3e-06f,    0.218843f,  0.816754f,  0.522139f,  0.422818f,  0.732249f,
    -3e-06f,    0.218843f,  0.816755f,  1e-06f,     0.422819f,  0.732249f,  -4e-06f,    0.218843f,
    0.816755f,  1e-06f,     0.218843f,  0.816755f,  1e-06f,     0.218843f,  0.816755f,  1e-06f,
    2e-06f,     0.845564f,  -0.0f,      0.218843f,  0.816755f,  1e-06f,     4e-06f,     0.845564f,
    1e-06f,     0.218843f,  0.816754f,  0.522139f,  4e-06f,     0.845564f,  1e-06f,     4e-06f,
    0.845563f,  0.522141f,  4e-06f,     0.845564f,  1e-06f,     -0.218843f, 0.816754f,  0.522138f,
    4e-06f,     0.845564f,  1e-06f,     -0.218842f, 0.816753f,  -2e-06f,    2e-06f,     0.845564f,
    -0.0f,      -0.218842f, 0.816753f,  -2e-06f,    -0.218841f, 0.816754f,  -1e-06f,    -0.218842f,
    0.816753f,  -2e-06f,    -0.422828f, 0.732244f,  -0.0f,      -0.218842f, 0.816753f,  -2e-06f,
    -0.422817f, 0.73224f,   1.7e-05f,   -0.218843f, 0.816754f,  0.522138f,  -0.422817f, 0.73224f,
    1.7e-05f,   -0.422811f, 0.732232f,  0.522171f,  -0.422817f, 0.73224f,   1.7e-05f,   -0.597935f,
    0.597803f,  0.522224f,  -0.422817f, 0.73224f,   1.7e-05f,   -0.597955f, 0.597821f,  4.5e-05f,
    -0.422828f, 0.732244f,  -0.0f,      -0.597955f, 0.597821f,  4.5e-05f,   -0.597978f, 0.597839f,
    1e-06f,     -0.597955f, 0.597821f,  4.5e-05f,   -0.732307f, 0.422746f,  -2e-06f,    -0.597955f,
    0.597821f,  4.5e-05f,   -0.732285f, 0.422736f,  4.2e-05f,   -0.597935f, 0.597803f,  0.522224f,
    -0.732285f, 0.422736f,  4.2e-05f,   -0.732263f, 0.422722f,  0.522221f,  -0.732285f, 0.422736f,
    4.2e-05f,   -0.816727f, 0.218842f,  0.522182f,  -0.732285f, 0.422736f,  4.2e-05f,   -0.81674f,
    0.218846f,  2.1e-05f,   -0.732307f, 0.422746f,  -2e-06f,    -0.81674f,  0.218846f,  2.1e-05f,
    -0.816761f, 0.218837f,  -8e-06f,    -0.81674f,  0.218846f,  2.1e-05f,   -0.845564f, -1.2e-05f,
    -3e-06f,    -0.81674f,  0.218846f,  2.1e-05f};

float reflection_mesh_trio_directions_2[75] __asm__("D_L16_001DBFC8") = {
    -0.845564f, -1.2e-05f,  -3e-06f,    -0.81674f,  0.218846f,  2.1e-05f,   -0.845562f, -1e-06f,
    0.0f,       -0.816727f, 0.218842f,  0.522182f,  -0.845562f, -1e-06f,    0.0f,       -0.845562f,
    -2e-06f,    0.522142f,  -0.845562f, -1e-06f,    0.0f,       -0.816775f, -0.218856f, 0.5221f,
    -0.845562f, -1e-06f,    0.0f,       -0.816764f, -0.218852f, -2e-05f,    -0.845564f, -1.2e-05f,
    -3e-06f,    -0.816764f, -0.218852f, -2e-05f,    -0.816746f, -0.218862f, 3e-06f,     -0.732267f,
    -0.422822f, -2.5e-05f,  -0.732249f, -0.422822f, 2e-06f,     -0.732267f, -0.422822f, -2.5e-05f,
    -0.597836f, -0.59797f,  4e-06f,     -0.732267f, -0.422822f, -2.5e-05f,  -0.597842f, -0.597974f,
    -8e-06f,    -0.732267f, -0.422822f, -2.5e-05f,  -0.597846f, -0.597977f, 0.52212f,   -0.732267f,
    -0.422822f, -2.5e-05f,  -0.732281f, -0.422829f, 0.522089f,  -0.816764f, -0.218852f, -2e-05f,
    -0.816775f, -0.218856f, 0.5221f};

u32 reflection_mesh_trio_colours_0[148] __asm__("D_L16_001DC0F8") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 reflection_mesh_trio_colours_1[148] __asm__("D_L16_001DC348") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 reflection_mesh_trio_colours_2[25] __asm__("D_L16_001DC598") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};
