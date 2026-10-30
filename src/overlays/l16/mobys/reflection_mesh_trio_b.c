/* Level 16 moby class reflection_mesh_trio_b (0x7a1): bounds of a second set of three reflection meshes, drawn with the reflection_mesh_trio draw. */
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

#include "sda.h"

#include "rnc/overlay/l16/mobys/reflection_mesh_trio.h"

/* Bounding spheres for one mesh (two or three entries), then the renderer callback; same source as FUN_L16_002e93b0. */
typedef struct {
    float x, y, z, radius;
} L16SingleSphere_2ea128;

int D_L16_00162008[4] __attribute__((section(".sdata"))) = {148, 75, 45, 0};

extern float D_L16_001E2DB0[];
extern float D_L16_001E34A0[];
extern float D_L16_001E3828[];
float       *D_L16_00162018[4]
    __attribute__((section(".sdata"))) = {D_L16_001E2DB0, D_L16_001E34A0, D_L16_001E3828, 0};

extern L16SingleSphere_2ea128 D_L16_001E5390[] NOT_SDA;
void update_reflection_mesh_trio_b(struct Moby *moby) __asm__("FUN_L16_002ea128");

void update_reflection_mesh_trio_b(struct Moby *moby) {
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
                for (k = 0; k < D_L16_00162008[i]; k++) {
                    float x, y, z;
                    verts = ((float **)D_L16_00162018)[i] + k * 3;
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
                L16SingleSphere_2ea128 *base = D_L16_001E5390;
                L16SingleSphere_2ea128 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < D_L16_00162008[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(D_L16_001E5390 + i);
                vertex_position[0] = ((float **)D_L16_00162018)[i][j * 3 + 0];
                vertex_position[1] = ((float **)D_L16_00162018)[i][j * 3 + 1];
                vertex_position[2] = ((float **)D_L16_00162018)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
            }
        }
        moby->state = 1;
        moby->update_dist = moby->draw_dist = 255;
        break;
    }
    case MESH_RENDER_QUEUE_CALLBACK:
        add_draw_callback(env_map_a_draw, moby);
        break;
    }
}

s32 D_L16_0016CDE4[1] __attribute__((section(".data"))) = {0};
