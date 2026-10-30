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
int         D_L16_00161F40[2] = {125, 0};

extern float D_L16_001DCAE0[];
float       *D_L16_00161F60[2] = {D_L16_001DCAE0, 0};

extern float D_L16_001DD0C0[];
float       *D_L16_00161F68[2] = {D_L16_001DD0C0, 0};

extern float D_L16_001DD4A8[];
float       *D_L16_00161F70[2] = {D_L16_001DD4A8, 0};

extern float D_L16_001DDA88[];
float       *D_L16_00161F78[2] = {D_L16_001DDA88, 0};

void draw_level16_visible_single_reflection_mesh(void) __asm__("FUN_L16_002e9270");

void draw_level16_visible_single_reflection_mesh(void) {
    int *point_counts = (int *)D_L16_00161F40;
    int *point_positions = (int *)D_L16_00161F60;
    int *unused_reflection_arguments = (int *)D_L16_00161F68;
    int *direction_vectors = (int *)D_L16_00161F70;
    int *point_word_streams = (int *)D_L16_00161F78;
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

void update_reflection_mesh(struct Moby *moby) __asm__("FUN_L16_002e93b0");

void update_reflection_mesh(struct Moby *moby) {
    enum {
        SINGLE_MESH_RENDER_INITIALIZE_BOUNDING_SPHERE = 0,
        SINGLE_MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case SINGLE_MESH_RENDER_INITIALIZE_BOUNDING_SPHERE: {
        for (i = 0; i <= 0; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < D_L16_00161F40[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00161F60)[i] + k * 3;
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
            for (j = 0; j < D_L16_00161F40[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(l16_single_reflection_mesh_bounds + i);
                vertex_position[0] = ((float **)D_L16_00161F60)[i][j * 3 + 0];
                vertex_position[1] = ((float **)D_L16_00161F60)[i][j * 3 + 1];
                vertex_position[2] = ((float **)D_L16_00161F60)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
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
