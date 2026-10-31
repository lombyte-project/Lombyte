/* Level 16 moby class reflection_mesh_pair (0x79c): bounds and draw of two reflection meshes, and the reflection mesh batch draw. */
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

/* Bounding spheres for one mesh (two or three entries), then the renderer callback; same source as FUN_L16_002e93b0. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2e98e0;

/* The code reads both as int tables of two entries (counts {110, 13}, positions
   {reflection_mesh_pair_positions_0, reflection_mesh_pair_positions_1}); only
   their first halfword is in C so far. */
extern short reflection_mesh_pair_vertex_counts __asm__("D_L16_00161F90") __attribute__((sda));
extern short reflection_mesh_pair_position_tables __asm__("D_L16_00161F98")
    __attribute__((sda)); /* low half of &reflection_mesh_pair_positions_0 (0x001DE150) */
extern L16SingleSphere_2e98e0 D_L16_00161F60_bounds_2e98e0[] __asm__("D_L16_001DF2B0")
    __attribute__((section(".data")));

#include "rnc/overlay/l16/mobys/reflection_mesh_trio.h"

void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

/* Sets up the draw state, then draws the two entries of the level's table. */

extern char D_L16_001DF2D0[] NOT_SDA;
/* Point positions of the two meshes. */
extern float
    reflection_mesh_pair_positions_0[330] __asm__("D_L16_001DE150"); /* 110 points x 3 floats */
extern float
    reflection_mesh_pair_positions_1[39] __asm__("D_L16_001DE678"); /* 13 points x 3 floats */
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern float D_L16_001DE720[];
extern float D_L16_001DEA90[];

float *reflection_mesh_pair_unused_pair_tables[2] __asm__("D_L16_00161FA0") = {D_L16_001DE720,
                                                                               D_L16_001DEA90};

extern float
    reflection_mesh_pair_directions_0[330] __asm__("D_L16_001DEAF8"); /* 110 points x 3 floats */
extern float
    reflection_mesh_pair_directions_1[39] __asm__("D_L16_001DF020"); /* 13 points x 3 floats */

float *reflection_mesh_pair_direction_tables[2] __asm__("D_L16_00161FA8") = {
    reflection_mesh_pair_directions_0, reflection_mesh_pair_directions_1};

extern u32 reflection_mesh_pair_colours_0[110] __asm__("D_L16_001DF0C0"); /* 110 RGBA words, one per point */
extern u32
    reflection_mesh_pair_colours_1[13] __asm__("D_L16_001DF278"); /* 13 RGBA words, one per point */

u32 *reflection_mesh_pair_colour_tables[2] __asm__("D_L16_00161FB0") = {
    reflection_mesh_pair_colours_0, reflection_mesh_pair_colours_1};

extern void FUN_L00_001fde98(int, int, int, char *, int);

void draw_level16_visible_textured_batches(void) __asm__("FUN_L16_002e97a0");

void draw_level16_visible_textured_batches(void) {
    int *element_counts = (int *)&reflection_mesh_pair_vertex_counts;
    int *packet_vector_tables = (int *)&reflection_mesh_pair_position_tables;
    int *unused_reflection_argument_tables = (int *)reflection_mesh_pair_unused_pair_tables;
    int *input_vector_tables = (int *)reflection_mesh_pair_direction_tables;
    int *packet_word_tables = (int *)reflection_mesh_pair_colour_tables;
    int  i;

    vu1_add_g_sregister(6, get_effect_texture(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 0x2; i++) {
        if (fast_bsphere_check(&D_L16_00161F60_bounds_2e98e0[i], 512.0f) != -1) {
            compute_level16_reflection_map_coordinates(element_counts[i],
                                                       input_vector_tables[i],
                                                       packet_vector_tables[i],
                                                       unused_reflection_argument_tables[i],
                                                       D_L16_001DF2D0);
            FUN_L00_001fde98(element_counts[i],
                             packet_vector_tables[i],
                             packet_word_tables[i],
                             D_L16_001DF2D0,
                             1);
        }
    }
}

void reflection_mesh_pair_update(struct Moby *moby) __asm__("FUN_L16_002e98e0");

/* Moby class 1948 */
void reflection_mesh_pair_update(struct Moby *moby) {
    enum {
        TWO_MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        TWO_MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *two_mesh_bounding_sphere_components;
    float *verts;
    int    count;
    switch (moby->state) {
    case TWO_MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 1; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < ((int *)&reflection_mesh_pair_vertex_counts)[i]; k++) {
                    float x, y, z;
                    verts = ((float **)&reflection_mesh_pair_position_tables)[i] + k * 3;
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
                L16SingleSphere_2e98e0 *two_mesh_bounding_spheres = D_L16_00161F60_bounds_2e98e0;
                L16SingleSphere_2e98e0 *sphere = two_mesh_bounding_spheres + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < ((int *)&reflection_mesh_pair_vertex_counts)[i]; j++) {
                float vertex_to_center_distance;
                two_mesh_bounding_sphere_components = (float *)(D_L16_00161F60_bounds_2e98e0 + i);
                vertex_position[0] =
                    ((float **)&reflection_mesh_pair_position_tables)[i][j * 3 + 0];
                vertex_position[1] =
                    ((float **)&reflection_mesh_pair_position_tables)[i][j * 3 + 1];
                vertex_position[2] =
                    ((float **)&reflection_mesh_pair_position_tables)[i][j * 3 + 2];
                vertex_to_center_distance =
                    fast_vec_dist(vertex_position, two_mesh_bounding_sphere_components);
                if (vertex_to_center_distance > two_mesh_bounding_sphere_components[3])
                    two_mesh_bounding_sphere_components[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        moby->update_dist = moby->draw_dist = 255;
        break;
    }
    case TWO_MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(draw_level16_visible_textured_batches, moby);
        break;
    }
}

extern int get_effect_texture(int) __asm__("FUN_001f44b8");

extern void FUN_L00_001fde98(int, int, int, char *, int);
/* Sets up the draw state then runs both per-slot calls over three entries. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EB158), where it is exact; names translated to the US level program. */

extern char level16_reflection_mesh_coordinate_scratch[] __asm__("D_L16_001E28D0");

int reflection_mesh_batch_vertex_counts[4] __asm__("D_L16_00161FB8")
    __attribute__((section(".sdata"))) = {148, 148, 53, 0};

extern float
    reflection_mesh_batch_positions_0[444] __asm__("D_L16_001DF7B0"); /* 148 points x 3 floats */
extern float
    reflection_mesh_batch_positions_1[444] __asm__("D_L16_001DFEA0"); /* 148 points x 3 floats */
extern float
       reflection_mesh_batch_positions_2[159] __asm__("D_L16_001E0590"); /* 53 points x 3 floats */
float *reflection_mesh_batch_position_tables[4] __asm__("D_L16_00161FC8")

    __attribute__((section(".sdata"))) = {reflection_mesh_batch_positions_0,
                                          reflection_mesh_batch_positions_1,
                                          reflection_mesh_batch_positions_2,
                                          0};

extern float D_L16_001E0810[];
extern float D_L16_001E0CB0[];
extern float D_L16_001E1150[];
float       *reflection_mesh_batch_unused_pair_tables[4] __asm__("D_L16_00161FD8")
    __attribute__((section(".sdata"))) = {D_L16_001E0810, D_L16_001E0CB0, D_L16_001E1150, 0};

extern float
    reflection_mesh_batch_directions_0[444] __asm__("D_L16_001E12F8"); /* 148 points x 3 floats */
extern float
    reflection_mesh_batch_directions_1[444] __asm__("D_L16_001E19E8"); /* 148 points x 3 floats */
extern float
       reflection_mesh_batch_directions_2[159] __asm__("D_L16_001E20D8"); /* 53 points x 3 floats */
float *reflection_mesh_batch_direction_tables[4] __asm__("D_L16_00161FE8")

    __attribute__((section(".sdata"))) = {reflection_mesh_batch_directions_0,
                                          reflection_mesh_batch_directions_1,
                                          reflection_mesh_batch_directions_2,
                                          0};

extern u32 reflection_mesh_batch_colours_0[148] __asm__("D_L16_001E2358"); /* 148 RGBA words, one per point */
extern u32 reflection_mesh_batch_colours_1[148] __asm__("D_L16_001E25A8"); /* 148 RGBA words, one per point */
extern u32 reflection_mesh_batch_colours_2[53] __asm__("D_L16_001E27F8"); /* 53 RGBA words, one per point */
u32 *reflection_mesh_batch_colour_tables[4] __asm__("D_L16_00161FF8")

    __attribute__((section(".sdata"))) = {reflection_mesh_batch_colours_0,
                                          reflection_mesh_batch_colours_1,
                                          reflection_mesh_batch_colours_2,
                                          0};

void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");

void draw_level16_reflection_mesh_batch(void) {
    int i;
    vu1_add_g_sregister(6, get_effect_texture(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        compute_level16_reflection_map_coordinates(
            reflection_mesh_batch_vertex_counts[i],
            ((int *)reflection_mesh_batch_direction_tables)[i],
            ((int *)reflection_mesh_batch_position_tables)[i],
            ((int *)reflection_mesh_batch_unused_pair_tables)[i],
            level16_reflection_mesh_coordinate_scratch);
        FUN_L00_001fde98(reflection_mesh_batch_vertex_counts[i],
                         ((int *)reflection_mesh_batch_position_tables)[i],
                         ((int *)reflection_mesh_batch_colour_tables)[i],
                         level16_reflection_mesh_coordinate_scratch,
                         1);
    }
}

float reflection_mesh_pair_positions_0[330] __asm__("D_L16_001DE150") = {
    263.5638f,  246.82368f, 126.79665f, 263.96344f, 244.11322f, 126.79665f, 263.5638f,  246.82368f,
    130.23242f, 263.96344f, 244.11322f, 130.23242f, 263.5638f,  246.82368f, 133.6682f,  263.96344f,
    244.11322f, 130.23242f, 263.96344f, 244.11322f, 133.6682f,  263.96344f, 244.11322f, 130.23242f,
    263.48383f, 240.86409f, 133.6682f,  263.96344f, 244.11322f, 130.23242f, 263.48383f, 240.86409f,
    130.23242f, 263.96344f, 244.11322f, 126.79665f, 263.48383f, 240.86409f, 130.23242f, 263.48383f,
    240.86409f, 126.79665f, 263.48383f, 240.86409f, 130.23242f, 263.70966f, 238.13362f, 126.79665f,
    263.48383f, 240.86409f, 130.23242f, 263.70966f, 238.13362f, 130.23242f, 263.48383f, 240.86409f,
    133.6682f,  263.70966f, 238.13362f, 130.23242f, 263.70966f, 238.13362f, 133.6682f,  263.70966f,
    238.13362f, 130.23242f, 263.01114f, 234.7441f,  133.6682f,  263.70966f, 238.13362f, 130.23242f,
    263.01114f, 234.7441f,  130.23242f, 263.70966f, 238.13362f, 126.79665f, 263.01114f, 234.7441f,
    130.23242f, 263.01114f, 234.7441f,  126.79665f, 263.01114f, 234.7441f,  130.23242f, 262.03793f,
    232.26538f, 126.79665f, 263.01114f, 234.7441f,  130.23242f, 262.03793f, 232.26538f, 130.23242f,
    263.01114f, 234.7441f,  133.6682f,  262.03793f, 232.26538f, 130.23242f, 262.03793f, 232.26538f,
    133.6682f,  262.03793f, 232.26538f, 130.23242f, 260.12784f, 229.8448f,  133.6682f,  262.03793f,
    232.26538f, 130.23242f, 260.12784f, 229.8448f,  130.23242f, 262.03793f, 232.26538f, 126.79665f,
    260.12784f, 229.8448f,  130.23242f, 260.12784f, 229.8448f,  126.79665f, 260.12784f, 229.8448f,
    130.23242f, 257.8258f,  228.01471f, 126.79665f, 260.12784f, 229.8448f,  130.23242f, 257.8258f,
    228.01471f, 130.23242f, 260.12784f, 229.8448f,  133.6682f,  257.8258f,  228.01471f, 130.23242f,
    257.8258f,  228.01471f, 133.6682f,  257.8258f,  228.01471f, 130.23242f, 255.03171f, 226.89482f,
    133.6682f,  257.8258f,  228.01471f, 130.23242f, 255.03171f, 226.89482f, 130.23242f, 257.8258f,
    228.01471f, 126.79665f, 255.03171f, 226.89482f, 130.23242f, 255.03171f, 226.89482f, 126.79665f,
    255.03171f, 226.89482f, 130.23242f, 252.05263f, 226.43959f, 126.79665f, 255.03171f, 226.89482f,
    130.23242f, 252.05263f, 226.43959f, 130.23242f, 255.03171f, 226.89482f, 133.6682f,  252.05263f,
    226.43959f, 130.23242f, 252.05263f, 226.43959f, 133.6682f,  252.05263f, 226.43959f, 130.23242f,
    249.02264f, 226.87799f, 133.6682f,  252.05263f, 226.43959f, 130.23242f, 249.02264f, 226.87799f,
    130.23242f, 252.05263f, 226.43959f, 126.79665f, 249.02264f, 226.87799f, 130.23242f, 249.02264f,
    226.87799f, 126.79665f, 249.02264f, 226.87799f, 130.23242f, 246.26538f, 227.96207f, 126.79665f,
    249.02264f, 226.87799f, 130.23242f, 246.26538f, 227.96207f, 130.23242f, 249.02264f, 226.87799f,
    133.6682f,  246.26538f, 227.96207f, 130.23242f, 246.26538f, 227.96207f, 133.6682f,  246.26538f,
    227.96207f, 130.23242f, 243.95299f, 229.76398f, 133.6682f,  246.26538f, 227.96207f, 130.23242f,
    243.95299f, 229.76398f, 130.23242f, 246.26538f, 227.96207f, 126.79665f, 243.95299f, 229.76398f,
    130.23242f, 243.95299f, 229.76398f, 126.79665f, 243.95299f, 229.76398f, 130.23242f, 242.0147f,
    232.1742f,  126.79665f, 243.95299f, 229.76398f, 130.23242f, 242.0147f,  232.1742f,  130.23242f,
    243.95299f, 229.76398f, 133.6682f,  242.0147f,  232.1742f,  130.23242f, 242.0147f,  232.1742f,
    133.6682f,  242.0147f,  232.1742f,  130.23242f, 240.90564f, 234.94664f, 133.6682f,  242.0147f,
    232.1742f,  130.23242f, 240.90564f, 234.94664f, 130.23242f, 242.0147f,  232.1742f,  126.79665f,
    240.90564f, 234.94664f, 130.23242f, 240.90564f, 234.94664f, 126.79665f, 240.43959f, 237.94736f,
    130.23242f, 240.43959f, 237.94736f, 126.79665f, 240.43959f, 237.94736f, 130.23242f, 240.82687f,
    240.6596f,  126.79665f, 240.43959f, 237.94736f, 130.23242f, 240.82687f, 240.6596f,  130.23242f,
    240.43959f, 237.94736f, 130.23242f, 240.82687f, 240.6596f,  133.6682f,  240.43959f, 237.94736f,
    130.23242f, 240.43959f, 237.94736f, 133.6682f,  240.90564f, 234.94664f, 130.23242f, 240.90564f,
    234.94664f, 133.6682f};

float reflection_mesh_pair_positions_1[39] __asm__("D_L16_001DE678") = {
    244.98363f, 247.09302f, 126.79665f, 247.2971f,  248.56071f, 126.79665f, 244.98363f, 247.09302f,
    130.23242f, 247.2971f,  248.56071f, 130.23242f, 244.98363f, 247.09302f, 133.6682f,  247.2971f,
    248.56071f, 130.23242f, 247.2971f,  248.56071f, 133.6682f,  247.2971f,  248.56071f, 130.23242f,
    249.93573f, 249.29828f, 133.6682f,  247.2971f,  248.56071f, 130.23242f, 249.93573f, 249.29828f,
    130.23242f, 247.2971f,  248.56071f, 126.79665f, 249.93573f, 249.29828f, 126.79665f};

float reflection_mesh_pair_directions_0[330] __asm__("D_L16_001DEAF8") = {
    0.989305f,  0.145862f,  0.0f, 0.989293f,  -8.4e-05f,  0.0f, 0.989305f,  0.145862f,  0.0f,
    0.989293f,  -8.4e-05f,  0.0f, 0.989305f,  0.145862f,  0.0f, 0.989293f,  -8.4e-05f,  0.0f,
    0.989293f,  -8.4e-05f,  0.0f, 0.989293f,  -8.4e-05f,  0.0f, 0.992939f,  -0.031802f, 0.0f,
    0.989293f,  -8.4e-05f,  0.0f, 0.992939f,  -0.031802f, 0.0f, 0.989293f,  -8.4e-05f,  0.0f,
    0.992939f,  -0.031802f, 0.0f, 0.992939f,  -0.031802f, 0.0f, 0.992939f,  -0.031802f, 0.0f,
    0.988008f,  -0.059707f, 0.0f, 0.992939f,  -0.031802f, 0.0f, 0.988008f,  -0.059707f, 0.0f,
    0.992939f,  -0.031802f, 0.0f, 0.988008f,  -0.059707f, 0.0f, 0.988008f,  -0.059707f, 0.0f,
    0.988008f,  -0.059707f, 0.0f, 0.955122f,  -0.283652f, 0.0f, 0.988008f,  -0.059707f, 0.0f,
    0.955122f,  -0.283652f, 0.0f, 0.988008f,  -0.059707f, 0.0f, 0.955122f,  -0.283652f, 0.0f,
    0.955122f,  -0.283652f, 0.0f, 0.955122f,  -0.283652f, 0.0f, 0.857924f,  -0.492466f, 0.0f,
    0.955122f,  -0.283652f, 0.0f, 0.857924f,  -0.492466f, 0.0f, 0.955122f,  -0.283652f, 0.0f,
    0.857924f,  -0.492466f, 0.0f, 0.857924f,  -0.492466f, 0.0f, 0.857924f,  -0.492466f, 0.0f,
    0.703662f,  -0.701123f, 0.0f, 0.857924f,  -0.492466f, 0.0f, 0.703662f,  -0.701123f, 0.0f,
    0.857924f,  -0.492466f, 0.0f, 0.703662f,  -0.701123f, 0.0f, 0.703662f,  -0.701123f, 0.0f,
    0.703662f,  -0.701123f, 0.0f, 0.497168f,  -0.855498f, 0.0f, 0.703662f,  -0.701123f, 0.0f,
    0.497168f,  -0.855498f, 0.0f, 0.703662f,  -0.701123f, 0.0f, 0.497168f,  -0.855498f, 0.0f,
    0.497168f,  -0.855498f, 0.0f, 0.497168f,  -0.855498f, 0.0f, 0.261545f,  -0.958372f, 0.0f,
    0.497168f,  -0.855498f, 0.0f, 0.261545f,  -0.958372f, 0.0f, 0.497168f,  -0.855498f, 0.0f,
    0.261545f,  -0.958372f, 0.0f, 0.261545f,  -0.958372f, 0.0f, 0.261545f,  -0.958372f, 0.0f,
    0.00393f,   -0.98911f,  0.0f, 0.261545f,  -0.958372f, 0.0f, 0.00393f,   -0.98911f,  0.0f,
    0.261545f,  -0.958372f, 0.0f, 0.00393f,   -0.98911f,  0.0f, 0.00393f,   -0.98911f,  0.0f,
    0.00393f,   -0.98911f,  0.0f, -0.254551f, -0.960173f, 0.0f, 0.00393f,   -0.98911f,  0.0f,
    -0.254551f, -0.960173f, 0.0f, 0.00393f,   -0.98911f,  0.0f, -0.254551f, -0.960173f, 0.0f,
    -0.254551f, -0.960173f, 0.0f, -0.254551f, -0.960173f, 0.0f, -0.490282f, -0.859722f, 0.0f,
    -0.254551f, -0.960173f, 0.0f, -0.490282f, -0.859722f, 0.0f, -0.254551f, -0.960173f, 0.0f,
    -0.490282f, -0.859722f, 0.0f, -0.490282f, -0.859722f, 0.0f, -0.490282f, -0.859722f, 0.0f,
    -0.696965f, -0.70774f,  0.0f, -0.490282f, -0.859722f, 0.0f, -0.696965f, -0.70774f,  0.0f,
    -0.490282f, -0.859722f, 0.0f, -0.696965f, -0.70774f,  0.0f, -0.696965f, -0.70774f,  0.0f,
    -0.696965f, -0.70774f,  0.0f, -0.853869f, -0.49905f,  0.0f, -0.696965f, -0.70774f,  0.0f,
    -0.853869f, -0.49905f,  0.0f, -0.696965f, -0.70774f,  0.0f, -0.853869f, -0.49905f,  0.0f,
    -0.853869f, -0.49905f,  0.0f, -0.853869f, -0.49905f,  0.0f, -0.95831f,  -0.262443f, 0.0f,
    -0.853869f, -0.49905f,  0.0f, -0.95831f,  -0.262443f, 0.0f, -0.853869f, -0.49905f,  0.0f,
    -0.95831f,  -0.262443f, 0.0f, -0.95831f,  -0.262443f, 0.0f, -0.989056f, -0.006058f, 0.0f,
    -0.989056f, -0.006058f, 0.0f, -0.989056f, -0.006058f, 0.0f, -0.989959f, 0.141357f,  0.0f,
    -0.989056f, -0.006058f, 0.0f, -0.989959f, 0.141357f,  0.0f, -0.989056f, -0.006058f, 0.0f,
    -0.989959f, 0.141357f,  0.0f, -0.989056f, -0.006058f, 0.0f, -0.989056f, -0.006058f, 0.0f,
    -0.95831f,  -0.262443f, 0.0f, -0.95831f,  -0.262443f, 0.0f};

float reflection_mesh_pair_directions_1[39] __asm__("D_L16_001DF020") = {
    -0.535702f, 0.844407f,  0.0f,       -0.402454f, 0.903745f,  0.0f,       -0.535702f, 0.844407f,
    0.0f,       -0.402454f, 0.903745f,  0.0f,       -0.535702f, 0.844407f,  0.0f,       -0.402454f,
    0.903745f,  0.0f,       -0.402454f, 0.903745f,  0.0f,       -0.402454f, 0.903745f,  0.0f,
    -0.269206f, 0.963082f,  0.0f,       -0.402454f, 0.903745f,  0.0f,       -0.269206f, 0.963083f,
    0.0f,       -0.402454f, 0.903745f,  0.0f,       -0.269206f, 0.963083f,  0.0f};

u32 reflection_mesh_pair_colours_0[110] __asm__("D_L16_001DF0C0") = {
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
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};

u32 reflection_mesh_pair_colours_1[13] __asm__("D_L16_001DF278") = {0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f,
                                                                    0x7f7f7f7f};

float reflection_mesh_batch_positions_0[444] __asm__("D_L16_001DF7B0") = {
    129.0f,      275.25998f,  123.79044f,  129.0f,      273.15128f,  119.68145f,  123.35012f,
    275.25998f,  123.79044f,  123.35012f,  273.15128f,  119.68145f,  118.7275f,   275.26f,
    123.79044f,  118.7275f,   273.1513f,   119.68145f,  116.16972f,  275.48563f,  123.79044f,
    116.05321f,  273.41745f,  119.68145f,  113.96739f,  276.15366f,  123.79044f,  113.4553f,
    274.20544f,  119.68145f,  111.937706f, 277.2385f,   123.79044f,  111.06104f,  275.48514f,
    119.68145f,  110.15869f,  278.69852f,  123.79044f,  108.96248f,  277.20746f,  119.68145f,
    108.69869f,  280.47757f,  123.79044f,  107.24024f,  279.30603f,  119.68145f,  107.61381f,
    282.50723f,  123.79044f,  105.96049f,  281.70026f,  119.68145f,  106.94574f,  284.70956f,
    123.79044f,  105.17243f,  284.2982f,   119.68145f,  106.72015f,  286.99988f,  123.79044f,
    104.90633f,  286.99985f,  119.68145f,  106.94573f,  289.29022f,  123.79044f,  105.17242f,
    289.70163f,  119.68145f,  107.61381f,  291.49255f,  123.79044f,  105.96049f,  292.2995f,
    119.68145f,  108.69869f,  293.52222f,  123.79044f,  107.24024f,  294.69376f,  119.68145f,
    110.15871f,  295.30124f,  123.79044f,  108.96251f,  296.79233f,  119.68145f,  111.93772f,
    296.7613f,   123.79044f,  111.061066f, 298.51462f,  119.68145f,  113.9674f,   297.84616f,
    123.79044f,  113.45531f,  299.79434f,  119.68145f,  116.169716f, 298.51422f,  123.79044f,
    116.0532f,   300.5824f,   119.68145f,  118.7275f,   298.73978f,  123.79044f,  118.7275f,
    300.84848f,  119.68145f,  123.35012f,  298.73978f,  123.79044f,  123.35012f,  300.84848f,
    119.68145f,  123.35012f,  298.73978f,  123.79044f,  129.0f,      300.84848f,  119.68145f,
    123.35012f,  298.73978f,  123.79044f,  129.0f,      298.73978f,  123.79044f,  123.91656f,
    299.46008f,  124.30547f,  129.0f,      299.46008f,  124.30547f,  123.91656f,  297.56284f,
    128.00253f,  129.0f,      297.56284f,  128.00253f,  124.48783f,  298.05978f,  128.63208f,
    129.0f,      298.05978f,  128.63208f,  124.48783f,  296.37573f,  131.91367f,  129.0f,
    296.37573f,  131.91367f,  125.027626f, 296.73672f,  132.57028f,  129.0f,      296.73672f,
    132.57028f,  125.027626f, 296.73672f,  132.57028f,  129.0f,      295.25412f,  135.45927f,
    125.027626f, 296.73672f,  132.57028f,  125.027626f, 295.25412f,  135.45927f,  121.777504f,
    296.73672f,  132.57028f,  121.777504f, 295.25412f,  135.45927f,  119.89723f,  296.54962f,
    132.57028f,  119.97916f,  295.0955f,   135.45927f,  118.070694f, 295.99557f,  132.57028f,
    118.43073f,  294.6258f,   135.45927f,  116.38732f,  295.0958f,   132.57028f,  117.003685f,
    293.86304f,  135.45927f,  114.91184f,  293.8849f,   132.57028f,  115.75289f,  292.83652f,
    135.45927f,  113.700935f, 292.4094f,   132.57028f,  114.72636f,  291.5857f,   135.45927f,
    112.80116f,  290.72604f,  132.57028f,  113.96359f,  290.15866f,  135.45927f,  112.24708f,
    288.89948f,  132.57028f,  113.49387f,  288.61023f,  135.45927f,  112.05999f,  286.9999f,
    132.57028f,  113.33527f,  286.9999f,   135.45927f,  112.24708f,  285.10037f,  132.57028f,
    113.49387f,  285.38962f,  135.45927f,  112.80116f,  283.27383f,  132.57028f,  113.96359f,
    283.8412f,   135.45927f,  113.68411f,  281.61377f,  132.57028f,  114.709526f, 282.43747f,
    135.45927f,  114.895004f, 280.13828f,  132.57028f,  115.736046f, 281.18665f,  135.45927f,
    116.3873f,   278.90402f,  132.57028f,  117.00368f,  280.13678f,  135.45927f,  118.07068f,
    278.00427f,  132.57028f,  118.43073f,  279.37402f,  135.45927f,  119.89724f,  277.45023f,
    132.57028f,  119.979164f, 278.90436f,  135.45927f,  121.777504f, 277.26312f,  132.57028f,
    121.777504f, 278.74573f,  135.45927f,  125.027626f, 277.2631f,   132.57028f,  125.027626f,
    278.74573f,  135.45927f,  125.027626f, 277.2631f,   132.57028f,  129.0f,      278.74573f,
    135.45927f,  125.027626f, 277.2631f,   132.57028f,  129.0f,      277.2631f,   132.57028f,
    124.48783f,  277.62402f,  131.91367f,  129.0f,      277.62402f,  131.91367f,  124.48783f,
    275.94f,     128.63208f,  129.0f,      275.94f,     128.63208f,  123.91656f,  276.437f,
    128.00253f,  129.0f,      276.437f,    128.00253f,  123.91656f,  274.5397f,   124.30547f,
    129.0f,      274.5397f,   124.30547f,  123.91656f,  274.5397f,   124.30547f,  129.0f,
    275.25998f,  123.79044f,  123.91656f,  274.5397f,   124.30547f,  123.35012f,  275.25998f,
    123.79044f,  119.75738f,  274.5397f,   124.30547f,  118.7275f,   275.26f,     123.79044f,
    117.35121f,  274.77914f,  124.30547f,  116.16972f,  275.48563f,  123.79044f,  115.01375f,
    275.4882f,   124.30547f,  113.96739f,  276.15366f,  123.79044f,  112.85953f,  276.63962f,
    124.30547f,  111.937706f, 277.2385f,   123.79044f,  110.971344f, 278.1892f,   124.30547f,
    110.15869f,  278.69852f,  123.79044f,  109.421776f, 280.07742f,  124.30547f,  108.69869f,
    280.47757f,  123.79044f,  108.27032f,  282.2316f,   124.30547f,  107.61381f,  282.50723f,
    123.79044f,  107.561264f, 284.56906f,  124.30547f,  106.94574f,  284.70956f,  123.79044f,
    107.321846f, 286.99988f,  124.30547f,  106.72015f,  286.99988f,  123.79044f,  107.56126f,
    289.4308f,   124.30547f,  106.94573f,  289.29022f,  123.79044f,  108.27032f,  291.76822f,
    124.30547f,  107.61381f,  291.49255f,  123.79044f,  109.421776f, 293.92242f,  124.30547f,
    108.69869f,  293.52222f,  123.79044f,  110.971375f, 295.81058f,  124.30547f,  110.15871f,
    295.30124f,  123.79044f,  112.859535f, 297.3602f,   124.30547f,  111.93772f,  296.7613f,
    123.79044f,  115.01375f,  298.51166f,  124.30547f,  113.9674f,   297.84616f,  123.79044f,
    117.35118f,  299.2207f,   124.30547f,  116.169716f, 298.51422f,  123.79044f,  119.75738f,
    299.46008f,  124.30547f,  118.7275f,   298.73978f,  123.79044f,  119.75738f,  299.46008f,
    124.30547f,  123.35012f,  298.73978f,  123.79044f,  119.75738f,  299.46008f,  124.30547f,
    123.91656f,  299.46008f,  124.30547f};

float reflection_mesh_batch_positions_1[444] __asm__("D_L16_001DFEA0") = {
    119.75738f,  299.46008f,  124.30547f,  123.91656f,  299.46008f,  124.30547f,  119.75738f,
    297.56284f,  128.00253f,  123.91656f,  297.56284f,  128.00253f,  120.79606f,  298.05978f,
    128.63208f,  124.48783f,  298.05978f,  128.63208f,  120.79606f,  296.37573f,  131.91367f,
    124.48783f,  296.37573f,  131.91367f,  120.79606f,  296.37573f,  131.91367f,  125.027626f,
    296.73672f,  132.57028f,  120.79606f,  296.37573f,  131.91367f,  121.777504f, 296.73672f,
    132.57028f,  118.753334f, 296.1956f,   131.91367f,  119.89723f,  296.54962f,  132.57028f,
    116.9945f,   295.66205f,  131.91367f,  118.070694f, 295.99557f,  132.57028f,  115.37354f,
    294.79562f,  131.91367f,  116.38732f,  295.0958f,   132.57028f,  113.952774f, 293.6296f,
    131.91367f,  114.91184f,  293.8849f,   132.57028f,  112.78676f,  292.20883f,  131.91367f,
    113.700935f, 292.4094f,   132.57028f,  111.92033f,  290.58786f,  131.91367f,  112.80116f,
    290.72604f,  132.57028f,  111.38679f,  288.829f,    131.91367f,  112.24708f,  288.89948f,
    132.57028f,  111.206635f, 286.99988f,  131.91367f,  112.05999f,  286.9999f,   132.57028f,
    111.386795f, 285.17075f,  131.91367f,  112.24708f,  285.10037f,  132.57028f,  111.92033f,
    283.41193f,  131.91367f,  112.80116f,  283.27383f,  132.57028f,  112.78676f,  281.79095f,
    131.91367f,  113.68411f,  281.61377f,  132.57028f,  113.926476f, 280.3809f,   131.91367f,
    114.895004f, 280.13828f,  132.57028f,  115.347244f, 279.21484f,  131.91367f,  116.3873f,
    278.90402f,  132.57028f,  116.9945f,   278.33774f,  131.91367f,  118.07068f,  278.00427f,
    132.57028f,  118.75335f,  277.80423f,  131.91367f,  119.89724f,  277.45023f,  132.57028f,
    120.79606f,  277.62405f,  131.91367f,  121.777504f, 277.26312f,  132.57028f,  120.79606f,
    277.62405f,  131.91367f,  125.027626f, 277.2631f,   132.57028f,  120.79606f,  277.62405f,
    131.91367f,  124.48783f,  277.62402f,  131.91367f,  120.79606f,  275.94f,     128.63208f,
    124.48783f,  275.94f,     128.63208f,  119.75738f,  276.437f,    128.00253f,  123.91656f,
    276.437f,    128.00253f,  119.75738f,  276.437f,    128.00253f,  123.91656f,  274.5397f,
    124.30547f,  119.75738f,  276.437f,    128.00253f,  119.75738f,  274.5397f,   124.30547f,
    117.45603f,  276.64f,     128.00253f,  117.35121f,  274.77914f,  124.30547f,  115.4745f,
    277.24106f,  128.00253f,  115.01375f,  275.4882f,   124.30547f,  113.6483f,   278.21713f,
    128.00253f,  112.85953f,  276.63962f,  124.30547f,  112.047646f, 279.53082f,  128.00253f,
    110.971344f, 278.1892f,   124.30547f,  110.73401f,  281.1315f,   128.00253f,  109.421776f,
    280.07742f,  124.30547f,  109.7579f,   282.9577f,   128.00253f,  108.27032f,  282.2316f,
    124.30547f,  109.15681f,  284.93918f,  128.00253f,  107.561264f, 284.56906f,  124.30547f,
    108.953835f, 286.9999f,   128.00253f,  107.321846f, 286.99988f,  124.30547f,  109.15681f,
    289.06064f,  128.00253f,  107.56126f,  289.4308f,   124.30547f,  109.7579f,   291.04218f,
    128.00253f,  108.27032f,  291.76822f,  124.30547f,  110.73401f,  292.86835f,  128.00253f,
    109.421776f, 293.92242f,  124.30547f,  112.04765f,  294.46902f,  128.00253f,  110.971375f,
    295.81058f,  124.30547f,  113.648315f, 295.78268f,  128.00253f,  112.859535f, 297.3602f,
    124.30547f,  115.47451f,  296.7588f,   128.00253f,  115.01375f,  298.51166f,  124.30547f,
    117.456024f, 297.35986f,  128.00253f,  117.35118f,  299.2207f,   124.30547f,  117.456024f,
    297.35986f,  128.00253f,  119.75738f,  299.46008f,  124.30547f,  117.456024f, 297.35986f,
    128.00253f,  119.75738f,  297.56284f,  128.00253f,  118.66029f,  297.8473f,   128.63208f,
    120.79606f,  298.05978f,  128.63208f,  118.66029f,  297.8473f,   128.63208f,  120.79606f,
    296.37573f,  131.91367f,  118.66029f,  297.8473f,   128.63208f,  118.753334f, 296.1956f,
    131.91367f,  118.66029f,  297.8473f,   128.63208f,  116.9945f,   295.66205f,  131.91367f,
    118.66029f,  297.8473f,   128.63208f,  116.58553f,  297.21793f,  128.63208f,  117.456024f,
    297.35986f,  128.00253f,  116.58553f,  297.21793f,  128.63208f,  115.47451f,  296.7588f,
    128.00253f,  116.58553f,  297.21793f,  128.63208f,  113.648315f, 295.78268f,  128.00253f,
    116.58553f,  297.21793f,  128.63208f,  114.673416f, 296.1959f,   128.63208f,  116.9945f,
    295.66205f,  131.91367f,  114.673416f, 296.1959f,   128.63208f,  115.37354f,  294.79562f,
    131.91367f,  114.673416f, 296.1959f,   128.63208f,  113.952774f, 293.6296f,   131.91367f,
    114.673416f, 296.1959f,   128.63208f,  112.99744f,  294.82043f,  128.63208f,  113.648315f,
    295.78268f,  128.00253f,  112.99744f,  294.82043f,  128.63208f,  112.04765f,  294.46902f,
    128.00253f,  112.99744f,  294.82043f,  128.63208f,  110.73401f,  292.86835f,  128.00253f,
    112.99744f,  294.82043f,  128.63208f,  111.62199f,  293.14444f,  128.63208f,  113.952774f,
    293.6296f,   131.91367f,  111.62199f,  293.14444f,  128.63208f,  112.78676f,  292.20883f,
    131.91367f,  111.62199f,  293.14444f,  128.63208f,  111.92033f,  290.58786f,  131.91367f,
    111.62199f,  293.14444f,  128.63208f,  110.59994f,  291.23233f,  128.63208f,  110.73401f,
    292.86835f,  128.00253f,  110.59994f,  291.23233f,  128.63208f,  109.7579f,   291.04218f,
    128.00253f,  110.59994f,  291.23233f,  128.63208f,  109.15681f,  289.06064f,  128.00253f,
    110.59994f,  291.23233f,  128.63208f,  109.970566f, 289.1576f,   128.63208f,  111.92033f,
    290.58786f,  131.91367f,  109.970566f, 289.1576f,   128.63208f,  111.38679f,  288.829f,
    131.91367f,  109.970566f, 289.1576f,   128.63208f,  111.206635f, 286.99988f,  131.91367f,
    109.970566f, 289.1576f,   128.63208f,  109.75806f,  286.99988f,  128.63208f,  109.15681f,
    289.06064f,  128.00253f,  109.75806f,  286.99988f,  128.63208f,  108.953835f, 286.9999f,
    128.00253f,  109.75806f,  286.99988f,  128.63208f,  109.15681f,  284.93918f,  128.00253f,
    109.75806f,  286.99988f,  128.63208f};

float reflection_mesh_batch_positions_2[159] __asm__("D_L16_001E0590") = {
    109.15681f,  284.93918f,  128.00253f,  109.75806f, 286.99988f, 128.63208f,  109.97057f,
    284.8422f,   128.63208f,  111.206635f, 286.99988f, 131.91367f, 109.97057f,  284.8422f,
    128.63208f,  111.386795f, 285.17075f,  131.91367f, 109.97057f, 284.8422f,   128.63208f,
    111.92033f,  283.41193f,  131.91367f,  109.97057f, 284.8422f,  128.63208f,  110.59994f,
    282.76746f,  128.63208f,  109.15681f,  284.93918f, 128.00253f, 110.59994f,  282.76746f,
    128.63208f,  109.7579f,   282.9577f,   128.00253f, 110.59994f, 282.76746f,  128.63208f,
    110.73401f,  281.1315f,   128.00253f,  110.59994f, 282.76746f, 128.63208f,  111.62199f,
    280.85535f,  128.63208f,  111.92033f,  283.41193f, 131.91367f, 111.62199f,  280.85535f,
    128.63208f,  112.78676f,  281.79095f,  131.91367f, 111.62199f, 280.85535f,  128.63208f,
    113.926476f, 280.3809f,   131.91367f,  111.62199f, 280.85535f, 128.63208f,  112.97114f,
    279.1901f,   128.63208f,  110.73401f,  281.1315f,  128.00253f, 112.97114f,  279.1901f,
    128.63208f,  112.047646f, 279.53082f,  128.00253f, 112.97114f, 279.1901f,   128.63208f,
    113.6483f,   278.21713f,  128.00253f,  112.97114f, 279.1901f,  128.63208f,  114.64712f,
    277.81458f,  128.63208f,  113.926476f, 280.3809f,  131.91367f, 114.64712f,  277.81458f,
    128.63208f,  115.347244f, 279.21484f,  131.91367f, 114.64712f, 277.81458f,  128.63208f,
    116.9945f,   278.33774f,  131.91367f,  114.64712f, 277.81458f, 128.63208f,  116.585526f,
    276.78186f,  128.63208f,  113.6483f,   278.21713f, 128.00253f, 116.585526f, 276.78186f,
    128.63208f,  115.4745f,   277.24106f,  128.00253f, 118.6603f,  276.15253f,  128.63208f,
    117.45603f,  276.64f,     128.00253f,  118.6603f,  276.15253f, 128.63208f,  119.75738f,
    276.437f,    128.00253f,  118.6603f,   276.15253f, 128.63208f, 120.79606f,  275.94f,
    128.63208f,  118.6603f,   276.15253f,  128.63208f, 120.79606f, 277.62405f,  131.91367f,
    118.6603f,   276.15253f,  128.63208f,  118.75335f, 277.80423f, 131.91367f,  116.585526f,
    276.78186f,  128.63208f,  116.9945f,   278.33774f, 131.91367f};

float reflection_mesh_batch_directions_0[444] __asm__("D_L16_001E12F8") = {
    0.0f,       -0.735665f, -0.178431f, 0.0f,       -0.889683f, 0.456578f,  -2e-06f,    -0.735666f,
    -0.178431f, -3e-06f,    -0.889683f, 0.456578f,  -0.035994f, -0.747453f, -0.168701f, -0.044101f,
    -0.88795f,  0.455689f,  -0.16209f,  -0.795342f, -0.105275f, -0.174429f, -0.872792f, 0.447368f,
    -0.348409f, -0.831657f, 0.037093f,  -0.343067f, -0.827704f, 0.435196f,  -0.520453f, -0.779211f,
    0.225896f,  -0.500343f, -0.748456f, 0.426124f,  -0.640296f, -0.644071f, 0.394015f,  -0.639377f,
    -0.639123f, 0.418062f,  -0.709588f, -0.477194f, 0.497403f,  -0.754276f, -0.503834f, 0.411373f,
    -0.750047f, -0.312462f, 0.549687f,  -0.840127f, -0.347894f, 0.406362f,  -0.772637f, -0.154479f,
    0.573371f,  -0.893191f, -0.17762f,  0.403256f,  -0.780046f, -3e-06f,    0.580173f,  -0.911142f,
    -5e-06f,    0.402204f,  -0.772637f, 0.154479f,  0.57337f,   -0.89319f,  0.177621f,  0.403256f,
    -0.750054f, 0.312467f,  0.549678f,  -0.840127f, 0.347897f,  0.406361f,  -0.709596f, 0.477204f,
    0.497385f,  -0.754275f, 0.503834f,  0.411373f,  -0.640301f, 0.64408f,   0.393993f,  -0.639372f,
    0.639128f,  0.418062f,  -0.520461f, 0.779211f,  0.225888f,  -0.500347f, 0.748457f,  0.42612f,
    -0.348424f, 0.831655f,  0.037099f,  -0.343083f, 0.8277f,    0.435192f,  -0.162517f, 0.800113f,
    -0.103022f, -0.174554f, 0.874044f,  0.444882f,  -0.03641f,  0.752219f,  -0.166455f, -0.044212f,
    0.889205f,  0.453204f,  0.0f,       0.735657f,  -0.178437f, 0.0f,       0.889683f,  0.456578f,
    0.0f,       0.735657f,  -0.178437f, 0.0f,       0.889683f,  0.456578f,  0.0f,       0.735657f,
    -0.178437f, 0.0f,       0.735657f,  -0.178437f, 0.0f,       0.73566f,   -0.178442f, 0.0f,
    0.73566f,   -0.178442f, 0.0f,       0.837305f,  -0.081514f, 0.0f,       0.837305f,  -0.081514f,
    0.0f,       0.837304f,  -0.081512f, 0.0f,       0.837304f,  -0.081512f, 0.0f,       0.882993f,
    -0.012599f, 0.0f,       0.882993f,  -0.012599f, 0.0f,       0.882991f,  -0.012595f, 0.0f,
    0.882991f,  -0.012596f, 0.0f,       0.882991f,  -0.012595f, 0.0f,       0.889684f,  0.456578f,
    0.0f,       0.882991f,  -0.012595f, 0.0f,       0.889684f,  0.456578f,  -0.042611f, 0.896154f,
    0.01653f,   -0.044218f, 0.889203f,  0.453206f,  -0.180346f, 0.908721f,  0.130239f,  -0.174553f,
    0.874042f,  0.444888f,  -0.356198f, 0.862777f,  0.29222f,   -0.34308f,  0.827698f,  0.435197f,
    -0.49669f,  0.747731f,  0.422922f,  -0.500346f, 0.748457f,  0.426122f,  -0.599012f, 0.602279f,
    0.507113f,  -0.63937f,  0.639128f,  0.418063f,  -0.671545f, 0.450749f,  0.554846f,  -0.754275f,
    0.503834f,  0.411375f,  -0.721034f, 0.299807f,  0.580438f,  -0.840127f, 0.347896f,  0.406362f,
    -0.75024f,  0.149744f,  0.59292f,   -0.893191f, 0.17762f,   0.403256f,  -0.759934f, -1e-06f,
    0.596658f,  -0.911142f, -1e-06f,    0.402204f,  -0.750236f, -0.149745f, 0.592923f,  -0.89319f,
    -0.177623f, 0.403255f,  -0.721698f, -0.299633f, 0.579952f,  -0.840593f, -0.347085f, 0.406294f,
    -0.670765f, -0.447603f, 0.557695f,  -0.754744f, -0.503019f, 0.411308f,  -0.597475f, -0.597834f,
    0.512319f,  -0.640749f, -0.638082f, 0.417912f,  -0.496349f, -0.746597f, 0.424228f,  -0.501718f,
    -0.747416f, 0.425971f,  -0.355941f, -0.86312f,  0.29167f,   -0.343072f, -0.827702f, 0.435196f,
    -0.180056f, -0.905531f, 0.125056f,  -0.174428f, -0.87279f,  0.447369f,  -0.04233f,  -0.892971f,
    0.011353f,  -0.044097f, -0.887949f, 0.455692f,  -4e-06f,    -0.883007f, -0.012559f, -4e-06f,
    -0.88968f,  0.456585f,  -4e-06f,    -0.883007f, -0.012559f, 0.0f,       -0.88968f,  0.456585f,
    -4e-06f,    -0.883007f, -0.012559f, 0.0f,       -0.883006f, -0.012561f, -2e-06f,    -0.883012f,
    -0.012569f, 0.0f,       -0.883012f, -0.012571f, 0.0f,       -0.837287f, -0.081539f, 0.0f,
    -0.837287f, -0.081539f, 0.0f,       -0.837283f, -0.081532f, 0.0f,       -0.837283f, -0.081532f,
    -1e-06f,    -0.735665f, -0.17843f,  0.0f,       -0.735665f, -0.178431f, -1e-06f,    -0.735665f,
    -0.17843f,  0.0f,       -0.735665f, -0.178431f, -1e-06f,    -0.735665f, -0.17843f,  -2e-06f,
    -0.735666f, -0.178431f, -0.035991f, -0.747453f, -0.168701f, -0.035994f, -0.747453f, -0.168701f,
    -0.162092f, -0.79534f,  -0.105274f, -0.16209f,  -0.795342f, -0.105275f, -0.348416f, -0.831655f,
    0.037093f,  -0.348409f, -0.831657f, 0.037093f,  -0.520452f, -0.779214f, 0.225893f,  -0.520453f,
    -0.779211f, 0.225896f,  -0.640294f, -0.644073f, 0.394014f,  -0.640296f, -0.644071f, 0.394015f,
    -0.709589f, -0.477193f, 0.497404f,  -0.709588f, -0.477194f, 0.497403f,  -0.750046f, -0.312464f,
    0.549688f,  -0.750047f, -0.312462f, 0.549687f,  -0.772636f, -0.154478f, 0.573372f,  -0.772637f,
    -0.154479f, 0.573371f,  -0.780046f, -2e-06f,    0.580173f,  -0.780046f, -3e-06f,    0.580173f,
    -0.772637f, 0.154479f,  0.57337f,   -0.772637f, 0.154479f,  0.57337f,   -0.750053f, 0.312468f,
    0.549679f,  -0.750054f, 0.312467f,  0.549678f,  -0.709595f, 0.477206f,  0.497385f,  -0.709596f,
    0.477204f,  0.497385f,  -0.640301f, 0.644082f,  0.39399f,   -0.640301f, 0.64408f,   0.393993f,
    -0.520464f, 0.779211f,  0.225884f,  -0.520461f, 0.779211f,  0.225888f,  -0.348426f, 0.831655f,
    0.037097f,  -0.348424f, 0.831655f,  0.037099f,  -0.162515f, 0.800113f,  -0.103022f, -0.162517f,
    0.800113f,  -0.103022f, -0.036408f, 0.752221f,  -0.166457f, -0.03641f,  0.752219f,  -0.166455f,
    -0.036408f, 0.752221f,  -0.166457f, 0.0f,       0.735657f,  -0.178437f, -0.036408f, 0.752221f,
    -0.166457f, 0.0f,       0.73566f,   -0.178442f};

float reflection_mesh_batch_directions_1[444] __asm__("D_L16_001E19E8") = {
    -0.036408f, 0.752221f,  -0.166457f, 0.0f,       0.73566f,   -0.178442f, -0.040911f, 0.854077f,
    -0.057426f, 0.0f,       0.837305f,  -0.081514f, -0.040913f, 0.854077f,  -0.057426f, 0.0f,
    0.837304f,  -0.081512f, -0.042608f, 0.896156f,  0.016526f,  0.0f,       0.882993f,  -0.012599f,
    -0.042608f, 0.896156f,  0.016526f,  0.0f,       0.882991f,  -0.012595f, -0.042608f, 0.896156f,
    0.016526f,  -0.042611f, 0.896154f,  0.01653f,   -0.180346f, 0.908723f,  0.130235f,  -0.180346f,
    0.908721f,  0.130239f,  -0.3562f,   0.862778f,  0.292217f,  -0.356198f, 0.862777f,  0.29222f,
    -0.49669f,  0.747732f,  0.422921f,  -0.49669f,  0.747731f,  0.422922f,  -0.599013f, 0.602279f,
    0.507111f,  -0.599012f, 0.602279f,  0.507113f,  -0.671546f, 0.450749f,  0.554844f,  -0.671545f,
    0.450749f,  0.554846f,  -0.721034f, 0.299808f,  0.580438f,  -0.721034f, 0.299807f,  0.580438f,
    -0.75024f,  0.149744f,  0.59292f,   -0.75024f,  0.149744f,  0.59292f,   -0.759934f, -2e-06f,
    0.596657f,  -0.759934f, -1e-06f,    0.596658f,  -0.750237f, -0.149744f, 0.592923f,  -0.750236f,
    -0.149745f, 0.592923f,  -0.721465f, -0.300038f, 0.579985f,  -0.721698f, -0.299633f, 0.579952f,
    -0.671478f, -0.446932f, 0.557611f,  -0.670765f, -0.447603f, 0.557695f,  -0.597738f, -0.597275f,
    0.512276f,  -0.597475f, -0.597834f, 0.512319f,  -0.495391f, -0.747248f, 0.42433f,   -0.496349f,
    -0.746597f, 0.424228f,  -0.355667f, -0.863252f, 0.291695f,  -0.355941f, -0.86312f,  0.29167f,
    -0.180056f, -0.905533f, 0.125052f,  -0.180056f, -0.905531f, 0.125056f,  -0.042328f, -0.892974f,
    0.011346f,  -0.04233f,  -0.892971f, 0.011353f,  -0.042328f, -0.892974f, 0.011346f,  -4e-06f,
    -0.883007f, -0.012559f, -0.042328f, -0.892974f, 0.011346f,  -2e-06f,    -0.883012f, -0.012569f,
    -0.040537f, -0.849815f, -0.06205f,  0.0f,       -0.837287f, -0.081539f, -0.040537f, -0.849812f,
    -0.062044f, 0.0f,       -0.837283f, -0.081532f, -0.040537f, -0.849812f, -0.062044f, -1e-06f,
    -0.735665f, -0.17843f,  -0.040537f, -0.849812f, -0.062044f, -0.035991f, -0.747453f, -0.168701f,
    -0.177053f, -0.881223f, 0.043048f,  -0.162092f, -0.79534f,  -0.105274f, -0.358689f, -0.865726f,
    0.218271f,  -0.348416f, -0.831655f, 0.037093f,  -0.507134f, -0.76289f,  0.37337f,   -0.520452f,
    -0.779214f, 0.225893f,  -0.612445f, -0.615972f, 0.478056f,  -0.640294f, -0.644073f, 0.394014f,
    -0.68177f,  -0.458126f, 0.540706f,  -0.709589f, -0.477193f, 0.497404f,  -0.726838f, -0.302384f,
    0.574696f,  -0.750046f, -0.312464f, 0.549688f,  -0.754156f, -0.150598f, 0.589666f,  -0.772636f,
    -0.154478f, 0.573372f,  -0.763206f, -2e-06f,    0.594094f,  -0.780046f, -2e-06f,    0.580173f,
    -0.754152f, 0.150596f,  0.589669f,  -0.772637f, 0.154479f,  0.57337f,   -0.726832f, 0.302382f,
    0.574701f,  -0.750053f, 0.312468f,  0.549679f,  -0.68018f,  0.45683f,   0.543265f,  -0.709595f,
    0.477206f,  0.497385f,  -0.610089f, 0.61374f,   0.482948f,  -0.640301f, 0.644082f,  0.39399f,
    -0.506369f, 0.761946f,  0.375733f,  -0.520464f, 0.779211f,  0.225884f,  -0.358698f, 0.865724f,
    0.218328f,  -0.348426f, 0.831655f,  0.037097f,  -0.177433f, 0.885483f,  0.047687f,  -0.162515f,
    0.800113f,  -0.103022f, -0.177433f, 0.885483f,  0.047687f,  -0.036408f, 0.752221f,  -0.166457f,
    -0.177433f, 0.885483f,  0.047687f,  -0.040911f, 0.854077f,  -0.057426f, -0.177434f, 0.885483f,
    0.047686f,  -0.040913f, 0.854077f,  -0.057426f, -0.177434f, 0.885483f,  0.047686f,  -0.042608f,
    0.896156f,  0.016526f,  -0.177434f, 0.885483f,  0.047686f,  -0.180346f, 0.908723f,  0.130235f,
    -0.177434f, 0.885483f,  0.047686f,  -0.3562f,   0.862778f,  0.292217f,  -0.177434f, 0.885483f,
    0.047686f,  -0.358696f, 0.865724f,  0.218329f,  -0.177433f, 0.885483f,  0.047687f,  -0.358696f,
    0.865724f,  0.218329f,  -0.358698f, 0.865724f,  0.218328f,  -0.358696f, 0.865724f,  0.218329f,
    -0.506369f, 0.761946f,  0.375733f,  -0.358696f, 0.865724f,  0.218329f,  -0.506365f, 0.761946f,
    0.375737f,  -0.3562f,   0.862778f,  0.292217f,  -0.506365f, 0.761946f,  0.375737f,  -0.49669f,
    0.747732f,  0.422921f,  -0.506365f, 0.761946f,  0.375737f,  -0.599013f, 0.602279f,  0.507111f,
    -0.506365f, 0.761946f,  0.375737f,  -0.610089f, 0.613738f,  0.482949f,  -0.506369f, 0.761946f,
    0.375733f,  -0.610089f, 0.613738f,  0.482949f,  -0.610089f, 0.61374f,   0.482948f,  -0.610089f,
    0.613738f,  0.482949f,  -0.68018f,  0.45683f,   0.543265f,  -0.610089f, 0.613738f,  0.482949f,
    -0.680182f, 0.456827f,  0.543265f,  -0.599013f, 0.602279f,  0.507111f,  -0.680182f, 0.456827f,
    0.543265f,  -0.671546f, 0.450749f,  0.554844f,  -0.680182f, 0.456827f,  0.543265f,  -0.721034f,
    0.299808f,  0.580438f,  -0.680182f, 0.456827f,  0.543265f,  -0.726833f, 0.302381f,  0.5747f,
    -0.68018f,  0.45683f,   0.543265f,  -0.726833f, 0.302381f,  0.5747f,    -0.726832f, 0.302382f,
    0.574701f,  -0.726833f, 0.302381f,  0.5747f,    -0.754152f, 0.150596f,  0.589669f,  -0.726833f,
    0.302381f,  0.5747f,    -0.754153f, 0.150597f,  0.589667f,  -0.721034f, 0.299808f,  0.580438f,
    -0.754153f, 0.150597f,  0.589667f,  -0.75024f,  0.149744f,  0.59292f,   -0.754153f, 0.150597f,
    0.589667f,  -0.759934f, -2e-06f,    0.596657f,  -0.754153f, 0.150597f,  0.589667f,  -0.763207f,
    -1e-06f,    0.594093f,  -0.754152f, 0.150596f,  0.589669f,  -0.763207f, -1e-06f,    0.594093f,
    -0.763206f, -2e-06f,    0.594094f,  -0.763207f, -1e-06f,    0.594093f,  -0.754156f, -0.150598f,
    0.589666f,  -0.763207f, -1e-06f,    0.594093f};

float reflection_mesh_batch_directions_2[159] __asm__("D_L16_001E20D8") = {
    -0.754156f, -0.150598f, 0.589666f,  -0.763207f, -1e-06f,    0.594093f,  -0.754156f, -0.150599f,
    0.589664f,  -0.759934f, -2e-06f,    0.596657f,  -0.754156f, -0.150599f, 0.589664f,  -0.750237f,
    -0.149744f, 0.592923f,  -0.754156f, -0.150599f, 0.589664f,  -0.721465f, -0.300038f, 0.579985f,
    -0.754156f, -0.150599f, 0.589664f,  -0.726839f, -0.302383f, 0.574694f,  -0.754156f, -0.150598f,
    0.589666f,  -0.726839f, -0.302383f, 0.574694f,  -0.726838f, -0.302384f, 0.574696f,  -0.726839f,
    -0.302383f, 0.574694f,  -0.68177f,  -0.458126f, 0.540706f,  -0.726839f, -0.302383f, 0.574694f,
    -0.682717f, -0.457048f, 0.540589f,  -0.721465f, -0.300038f, 0.579985f,  -0.682717f, -0.457048f,
    0.540589f,  -0.671478f, -0.446932f, 0.557611f,  -0.682717f, -0.457048f, 0.540589f,  -0.597738f,
    -0.597275f, 0.512276f,  -0.682717f, -0.457048f, 0.540589f,  -0.613396f, -0.614891f, 0.477939f,
    -0.68177f,  -0.458126f, 0.540706f,  -0.613396f, -0.614891f, 0.477939f,  -0.612445f, -0.615972f,
    0.478056f,  -0.613396f, -0.614891f, 0.477939f,  -0.507134f, -0.76289f,  0.37337f,   -0.613396f,
    -0.614891f, 0.477939f,  -0.506864f, -0.763018f, 0.373398f,  -0.597738f, -0.597275f, 0.512276f,
    -0.506864f, -0.763018f, 0.373398f,  -0.495391f, -0.747248f, 0.42433f,   -0.506864f, -0.763018f,
    0.373398f,  -0.355667f, -0.863252f, 0.291695f,  -0.506864f, -0.763018f, 0.373398f,  -0.358411f,
    -0.86586f,  0.218297f,  -0.507134f, -0.76289f,  0.37337f,   -0.358411f, -0.86586f,  0.218297f,
    -0.358689f, -0.865726f, 0.218271f,  -0.177049f, -0.881226f, 0.043044f,  -0.177053f, -0.881223f,
    0.043048f,  -0.177049f, -0.881226f, 0.043044f,  -0.040537f, -0.849812f, -0.062044f, -0.177049f,
    -0.881226f, 0.043044f,  -0.040537f, -0.849815f, -0.06205f,  -0.177049f, -0.881226f, 0.043044f,
    -0.042328f, -0.892974f, 0.011346f,  -0.177049f, -0.881226f, 0.043044f,  -0.180056f, -0.905533f,
    0.125052f,  -0.358411f, -0.86586f,  0.218297f,  -0.355667f, -0.863252f, 0.291695f};

u32 reflection_mesh_batch_colours_0[148] __asm__("D_L16_001E2358") = {
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

u32 reflection_mesh_batch_colours_1[148] __asm__("D_L16_001E25A8") = {
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

u32 reflection_mesh_batch_colours_2[53] __asm__("D_L16_001E27F8") = {
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f,
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};
