/* Level 16 moby class reflection_mesh (0x79b): bounds and draw of one environment-mapped reflection mesh. */
#include "types.h"
#include "rnc/overlay/l16/mobys/reflection_mesh_trio.h"
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

#include "sda.h"

extern int get_effect_texture(int) __asm__("FUN_001f44b8");

extern void FUN_L00_001fde98(int, int, int, char *, int);
/* Sets up the draw state, then draws the one entry of the level's table; same source as env_map_a_draw. */

typedef struct {
    float x, y, z, radius;
} L16SingleSphere;

extern L16SingleSphere l16_single_reflection_mesh_bounds[] __asm__("D_L16_00161F80") MACRO_ADDR;
extern void            vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

extern char D_L16_001DDC80[] NOT_SDA;
int         single_reflection_mesh_vertex_counts[2] __asm__("D_L16_00161F40") = {125, 0};

extern float
    single_reflection_mesh_positions[375] __asm__("D_L16_001DCAE0"); /* 125 points x 3 floats */

float *single_reflection_mesh_position_tables[2] __asm__("D_L16_00161F60") = {
    single_reflection_mesh_positions, 0};

extern float D_L16_001DD0C0[];
float *single_reflection_mesh_unused_pair_tables[2] __asm__("D_L16_00161F68") = {D_L16_001DD0C0, 0};

extern float
    single_reflection_mesh_directions[375] __asm__("D_L16_001DD4A8"); /* 125 points x 3 floats */

float *single_reflection_mesh_direction_tables[2] __asm__("D_L16_00161F70") = {
    single_reflection_mesh_directions, 0};

extern u32 single_reflection_mesh_colours[125] __asm__("D_L16_001DDA88"); /* 125 RGBA words, one per point */

u32 *single_reflection_mesh_colour_tables[2] __asm__("D_L16_00161F78") = {
    single_reflection_mesh_colours, 0};

void draw_level16_visible_single_reflection_mesh(void) __asm__("FUN_L16_002e9270");

void draw_level16_visible_single_reflection_mesh(void) {
    int *point_counts = (int *)single_reflection_mesh_vertex_counts;
    int *point_positions = (int *)single_reflection_mesh_position_tables;
    int *unused_reflection_arguments = (int *)single_reflection_mesh_unused_pair_tables;
    int *direction_vectors = (int *)single_reflection_mesh_direction_tables;
    int *point_word_streams = (int *)single_reflection_mesh_colour_tables;
    int  i;

    vu1_add_g_sregister(6, get_effect_texture(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260);
    font_queue_vu_state();
    for (i = 0; i < 1; i++) {
        if (fast_bsphere_check(&l16_single_reflection_mesh_bounds[i], 512.0f) != -1) {
            compute_level16_reflection_map_coordinates(point_counts[i],
                                                       direction_vectors[i],
                                                       point_positions[i],
                                                       unused_reflection_arguments[i],
                                                       D_L16_001DDC80);
            FUN_L00_001fde98(
                point_counts[i], point_positions[i], point_word_streams[i], D_L16_001DDC80, 1);
        }
    }
}
/* Compute bounding spheres for one mesh and register their renderer. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002EA828), where it is exact; names translated to the US level program. */

extern void draw_level16_visible_single_reflection_mesh(void) __asm__("FUN_L16_002e9270");

void reflection_mesh_update(struct Moby *moby) __asm__("FUN_L16_002e93b0");

/* Moby class 1947 */
void reflection_mesh_update(struct Moby *moby) {
    enum {
        SINGLE_MESH_RENDER_INITIALIZE_BOUNDING_SPHERE = 0,
        SINGLE_MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *single_mesh_bounding_sphere_components;
    float *verts;
    int    count;
    switch (moby->state) {
    case SINGLE_MESH_RENDER_INITIALIZE_BOUNDING_SPHERE: {
        for (i = 0; i <= 0; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < single_reflection_mesh_vertex_counts[i]; k++) {
                    float x, y, z;
                    verts = ((float **)single_reflection_mesh_position_tables)[i] + k * 3;
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
                L16SingleSphere *sphere = l16_single_reflection_mesh_bounds + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < single_reflection_mesh_vertex_counts[i]; j++) {
                float vertex_to_center_distance;
                single_mesh_bounding_sphere_components =
                    (float *)(l16_single_reflection_mesh_bounds + i);
                vertex_position[0] =
                    ((float **)single_reflection_mesh_position_tables)[i][j * 3 + 0];
                vertex_position[1] =
                    ((float **)single_reflection_mesh_position_tables)[i][j * 3 + 1];
                vertex_position[2] =
                    ((float **)single_reflection_mesh_position_tables)[i][j * 3 + 2];
                vertex_to_center_distance =
                    fast_vec_dist(vertex_position, single_mesh_bounding_sphere_components);
                if (vertex_to_center_distance > single_mesh_bounding_sphere_components[3])
                    single_mesh_bounding_sphere_components[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        moby->update_dist = moby->draw_dist = 255;
        break;
    }
    case SINGLE_MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(draw_level16_visible_single_reflection_mesh, moby);
        break;
    }
}

float single_reflection_mesh_positions[375] __asm__("D_L16_001DCAE0") = {
    136.15062f,  128.2639f,   128.84532f,  137.99428f,  130.5911f,   128.84532f,  136.15062f,
    128.2639f,   132.56856f,  137.99428f,  130.5911f,   132.56856f,  136.15062f,  128.2639f,
    132.56856f,  137.99428f,  130.5911f,   136.29181f,  136.15062f,  128.2639f,   132.56856f,
    136.15062f,  128.2639f,   136.29181f,  136.15062f,  128.2639f,   132.56856f,  134.94156f,
    125.1803f,   136.29181f,  136.15062f,  128.2639f,   132.56856f,  134.94156f,  125.1803f,
    132.56856f,  136.15062f,  128.2639f,   128.84532f,  134.94156f,  125.1803f,   132.56856f,
    134.94156f,  125.1803f,   128.84532f,  134.94156f,  125.1803f,   132.56856f,  134.4722f,
    122.0f,      128.84532f,  134.94156f,  125.1803f,   132.56856f,  134.4722f,   122.0f,
    132.56856f,  134.94156f,  125.1803f,   136.29181f,  134.4722f,   122.0f,      132.56856f,
    134.4722f,   122.0f,      136.29181f,  134.4722f,   122.0f,      132.56856f,  134.95146f,
    118.76252f,  136.29181f,  134.4722f,   122.0f,      132.56856f,  134.95146f,  118.76252f,
    132.56856f,  134.4722f,   122.0f,      128.84532f,  134.95146f,  118.76252f,  132.56856f,
    134.95146f,  118.76252f,  128.84532f,  134.95146f,  118.76252f,  132.56856f,  136.15062f,
    115.7361f,   128.84532f,  134.95146f,  118.76252f,  132.56856f,  136.15062f,  115.7361f,
    132.56856f,  134.95146f,  118.76252f,  136.29181f,  136.15062f,  115.7361f,   132.56856f,
    136.15062f,  115.7361f,   136.29181f,  136.15062f,  115.7361f,   132.56856f,  138.17796f,
    113.17904f,  136.29181f,  136.15062f,  115.7361f,   132.56856f,  138.17796f,  113.17904f,
    132.56856f,  136.15062f,  115.7361f,   128.84532f,  138.17796f,  113.17904f,  132.56856f,
    138.17796f,  113.17904f,  128.84532f,  138.17796f,  113.17904f,  132.56856f,  140.7361f,
    111.15061f,  128.84532f,  138.17796f,  113.17904f,  132.56856f,  140.7361f,   111.15061f,
    132.56856f,  138.17796f,  113.17904f,  136.29181f,  140.7361f,   111.15061f,  132.56856f,
    140.7361f,   111.15061f,  136.29181f,  140.7361f,   111.15061f,  132.56856f,  143.7735f,
    109.95311f,  136.29181f,  140.7361f,   111.15061f,  132.56856f,  143.7735f,   109.95311f,
    132.56856f,  140.7361f,   111.15061f,  128.84532f,  143.7735f,   109.95311f,  132.56856f,
    143.7735f,   109.95311f,  128.84532f,  143.7735f,   109.95311f,  132.56856f,  147.0f,
    109.472206f, 128.84532f,  143.7735f,   109.95311f,  132.56856f,  147.0f,      109.472206f,
    132.56856f,  143.7735f,   109.95311f,  136.29181f,  147.0f,      109.472206f, 132.56856f,
    147.0f,      109.472206f, 136.29181f,  147.0f,      109.472206f, 132.56856f,  150.23747f,
    109.95146f,  136.29181f,  147.0f,      109.472206f, 132.56856f,  150.23747f,  109.95146f,
    132.56856f,  147.0f,      109.472206f, 128.84532f,  150.23747f,  109.95146f,  132.56856f,
    150.23747f,  109.95146f,  128.84532f,  150.23747f,  109.95146f,  132.56856f,  153.26389f,
    111.15061f,  128.84532f,  150.23747f,  109.95146f,  132.56856f,  153.26389f,  111.15061f,
    132.56856f,  150.23747f,  109.95146f,  136.29181f,  153.26389f,  111.15061f,  132.56856f,
    153.26389f,  111.15061f,  136.29181f,  153.26389f,  111.15061f,  132.56856f,  155.85126f,
    113.215034f, 136.29181f,  153.26389f,  111.15061f,  132.56856f,  155.85126f,  113.215034f,
    132.56856f,  153.26389f,  111.15061f,  128.84532f,  155.85126f,  113.215034f, 132.56856f,
    155.85126f,  113.215034f, 128.84532f,  155.85126f,  113.215034f, 132.56856f,  157.84938f,
    115.7361f,   128.84532f,  155.85126f,  113.215034f, 132.56856f,  157.84938f,  115.7361f,
    132.56856f,  155.85126f,  113.215034f, 136.29181f,  157.84938f,  115.7361f,   132.56856f,
    157.84938f,  115.7361f,   136.29181f,  157.84938f,  115.7361f,   132.56856f,  159.05333f,
    118.7723f,   136.29181f,  157.84938f,  115.7361f,   132.56856f,  159.05333f,  118.7723f,
    132.56856f,  157.84938f,  115.7361f,   128.84532f,  159.05333f,  118.7723f,   132.56856f,
    159.05333f,  118.7723f,   128.84532f,  159.05333f,  118.7723f,   132.56856f,  159.52779f,
    122.0f,      128.84532f,  159.05333f,  118.7723f,   132.56856f,  159.52779f,  122.0f,
    132.56856f,  159.05333f,  118.7723f,   136.29181f,  159.52779f,  122.0f,      132.56856f,
    159.52779f,  122.0f,      136.29181f,  159.52779f,  122.0f,      132.56856f,  159.05333f,
    125.22769f,  136.29181f,  159.52779f,  122.0f,      132.56856f,  159.05333f,  125.22769f,
    132.56856f,  159.52779f,  122.0f,      128.84532f,  159.05333f,  125.22769f,  132.56856f,
    159.05333f,  125.22769f,  128.84532f,  157.84938f,  128.2639f,   132.56856f,  157.84938f,
    128.2639f,   128.84532f,  157.84938f,  128.2639f,   132.56856f,  156.0057f,   130.5911f,
    128.84532f,  157.84938f,  128.2639f,   132.56856f,  156.0057f,   130.5911f,   132.56856f,
    157.84938f,  128.2639f,   132.56856f,  156.0057f,   130.5911f,   136.29181f,  157.84938f,
    128.2639f,   132.56856f,  157.84938f,  128.2639f,   136.29181f,  159.05333f,  125.22769f,
    132.56856f,  159.05333f,  125.22769f,  136.29181f};

float single_reflection_mesh_directions[375] __asm__("D_L16_001DD4A8") = {
    -0.857413f, 0.493004f,  0.0f, -0.783833f, 0.620971f,  0.0f, -0.857413f, 0.493004f,  0.0f,
    -0.783833f, 0.620971f,  0.0f, -0.857413f, 0.493004f,  0.0f, -0.783833f, 0.620971f,  0.0f,
    -0.857413f, 0.493004f,  0.0f, -0.857413f, 0.493004f,  0.0f, -0.857413f, 0.493004f,  0.0f,
    -0.960139f, 0.255519f,  0.0f, -0.857413f, 0.493004f,  0.0f, -0.960139f, 0.255519f,  0.0f,
    -0.857413f, 0.493004f,  0.0f, -0.960139f, 0.255519f,  0.0f, -0.960139f, 0.255519f,  0.0f,
    -0.960139f, 0.255519f,  0.0f, -0.989252f, -0.000219f, 0.0f, -0.960139f, 0.255519f,  0.0f,
    -0.989252f, -0.000219f, 0.0f, -0.960139f, 0.255519f,  0.0f, -0.989252f, -0.000219f, 0.0f,
    -0.989252f, -0.000219f, 0.0f, -0.989252f, -0.000219f, 0.0f, -0.95945f,  -0.257404f, 0.0f,
    -0.989252f, -0.000219f, 0.0f, -0.95945f,  -0.257404f, 0.0f, -0.989252f, -0.000219f, 0.0f,
    -0.95945f,  -0.257404f, 0.0f, -0.95945f,  -0.257403f, 0.0f, -0.95945f,  -0.257404f, 0.0f,
    -0.856639f, -0.494818f, 0.0f, -0.95945f,  -0.257404f, 0.0f, -0.856639f, -0.494818f, 0.0f,
    -0.95945f,  -0.257404f, 0.0f, -0.856639f, -0.494818f, 0.0f, -0.856639f, -0.494818f, 0.0f,
    -0.856639f, -0.494818f, 0.0f, -0.702455f, -0.702416f, 0.0f, -0.856639f, -0.494818f, 0.0f,
    -0.702455f, -0.702416f, 0.0f, -0.856639f, -0.494818f, 0.0f, -0.702455f, -0.702416f, 0.0f,
    -0.702455f, -0.702416f, 0.0f, -0.702455f, -0.702416f, 0.0f, -0.494044f, -0.856936f, 0.0f,
    -0.702455f, -0.702416f, 0.0f, -0.494044f, -0.856936f, 0.0f, -0.702455f, -0.702416f, 0.0f,
    -0.494044f, -0.856936f, 0.0f, -0.494044f, -0.856936f, 0.0f, -0.494044f, -0.856936f, 0.0f,
    -0.257098f, -0.959692f, 0.0f, -0.494044f, -0.856936f, 0.0f, -0.257098f, -0.959692f, 0.0f,
    -0.494044f, -0.856936f, 0.0f, -0.257098f, -0.959692f, 0.0f, -0.257098f, -0.959692f, 0.0f,
    -0.257098f, -0.959692f, 0.0f, -0.000491f, -0.989147f, 0.0f, -0.257098f, -0.959692f, 0.0f,
    -0.000491f, -0.989147f, 0.0f, -0.257098f, -0.959692f, 0.0f, -0.000491f, -0.989147f, 0.0f,
    -0.000491f, -0.989147f, 0.0f, -0.000491f, -0.989147f, 0.0f, 0.257402f,  -0.95945f,  0.0f,
    -0.000491f, -0.989147f, 0.0f, 0.257402f,  -0.95945f,  0.0f, -0.000491f, -0.989147f, 0.0f,
    0.257402f,  -0.95945f,  0.0f, 0.257402f,  -0.95945f,  0.0f, 0.257402f,  -0.95945f,  0.0f,
    0.496026f,  -0.855678f, 0.0f, 0.257402f,  -0.95945f,  0.0f, 0.496026f,  -0.855678f, 0.0f,
    0.257402f,  -0.95945f,  0.0f, 0.496026f,  -0.855678f, 0.0f, 0.496026f,  -0.855678f, 0.0f,
    0.496026f,  -0.855678f, 0.0f, 0.703693f,  -0.701407f, 0.0f, 0.496026f,  -0.855678f, 0.0f,
    0.703693f,  -0.701407f, 0.0f, 0.496026f,  -0.855678f, 0.0f, 0.703693f,  -0.701407f, 0.0f,
    0.703693f,  -0.701407f, 0.0f, 0.703693f,  -0.701407f, 0.0f, 0.856643f,  -0.494874f, 0.0f,
    0.703693f,  -0.701407f, 0.0f, 0.856643f,  -0.494874f, 0.0f, 0.703693f,  -0.701407f, 0.0f,
    0.856643f,  -0.494874f, 0.0f, 0.856643f,  -0.494874f, 0.0f, 0.856643f,  -0.494874f, 0.0f,
    0.959476f,  -0.257021f, 0.0f, 0.856643f,  -0.494874f, 0.0f, 0.959476f,  -0.257021f, 0.0f,
    0.856643f,  -0.494874f, 0.0f, 0.959476f,  -0.257021f, 0.0f, 0.959476f,  -0.257021f, 0.0f,
    0.959476f,  -0.257021f, 0.0f, 0.989368f,  0.0f,       0.0f, 0.959476f,  -0.257021f, 0.0f,
    0.989368f,  0.0f,       0.0f, 0.959476f,  -0.257021f, 0.0f, 0.989368f,  0.0f,       0.0f,
    0.989368f,  0.0f,       0.0f, 0.989368f,  0.0f,       0.0f, 0.959476f,  0.257021f,  0.0f,
    0.989368f,  0.0f,       0.0f, 0.959476f,  0.257021f,  0.0f, 0.989368f,  0.0f,       0.0f,
    0.959476f,  0.257021f,  0.0f, 0.959476f,  0.257021f,  0.0f, 0.856708f,  0.494792f,  0.0f,
    0.856708f,  0.494792f,  0.0f, 0.856708f,  0.494792f,  0.0f, 0.783831f,  0.620974f,  0.0f,
    0.856708f,  0.494792f,  0.0f, 0.783831f,  0.620974f,  0.0f, 0.856708f,  0.494792f,  0.0f,
    0.783831f,  0.620974f,  0.0f, 0.856708f,  0.494792f,  0.0f, 0.856708f,  0.494792f,  0.0f,
    0.959476f,  0.257021f,  0.0f, 0.959476f,  0.257021f,  0.0f};

u32 single_reflection_mesh_colours[125] __asm__("D_L16_001DDA88") = {
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
    0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f, 0x7f7f7f7f};
