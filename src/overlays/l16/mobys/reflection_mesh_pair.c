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

extern short                  D_L16_00161F90_gp __asm__("D_L16_00161F90") __attribute__((sda));
extern short                  D_L16_00161F98_gp __asm__("D_L16_00161F98") __attribute__((sda));
extern L16SingleSphere_2e98e0 D_L16_00161F60_bounds_2e98e0[] __asm__("D_L16_001DF2B0")
    __attribute__((section(".data")));

#include "rnc/overlay/l16/mobys/reflection_mesh_trio.h"

void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

/* Sets up the draw state, then draws the two entries of the level's table. */

extern char  D_L16_001DF2D0[] NOT_SDA;
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern float D_L16_001DE720[];
extern float D_L16_001DEA90[];
float       *D_L16_00161FA0[2] = {D_L16_001DE720, D_L16_001DEA90};

extern float D_L16_001DEAF8[];
extern float D_L16_001DF020[];
float       *D_L16_00161FA8[2] = {D_L16_001DEAF8, D_L16_001DF020};

extern float D_L16_001DF0C0[];
extern float D_L16_001DF278[];
float       *D_L16_00161FB0[2] = {D_L16_001DF0C0, D_L16_001DF278};

extern void FUN_L00_001fde98(int, int, int, char *, int);

void draw_level16_visible_textured_batches(void) __asm__("FUN_L16_002e97a0");

void draw_level16_visible_textured_batches(void) {
    int *element_counts = (int *)&D_L16_00161F90_gp;
    int *packet_vector_tables = (int *)&D_L16_00161F98_gp;
    int *unused_reflection_argument_tables = (int *)D_L16_00161FA0;
    int *input_vector_tables = (int *)D_L16_00161FA8;
    int *packet_word_tables = (int *)D_L16_00161FB0;
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

void update_reflection_mesh_pair(struct Moby *moby) __asm__("FUN_L16_002e98e0");

void update_reflection_mesh_pair(struct Moby *moby) {
    enum {
        TWO_MESH_RENDER_INITIALIZE_BOUNDING_SPHERES = 0,
        TWO_MESH_RENDER_QUEUE_CALLBACK = 1
    };
    int    i, j;
    float  maxx, maxy, maxz, minx, miny, minz;
    float  vertex_position[4];
    float *out;
    float *verts;
    int    count;
    switch (moby->state) {
    case TWO_MESH_RENDER_INITIALIZE_BOUNDING_SPHERES: {
        for (i = 0; i <= 1; i++) {
            maxx = maxy = maxz = -1024.0f;
            minx = miny = minz = 1024.0f;
            {
                int k;
                for (k = 0; k < ((int *)&D_L16_00161F90_gp)[i]; k++) {
                    float x, y, z;
                    verts = ((float **)&D_L16_00161F98_gp)[i] + k * 3;
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
                L16SingleSphere_2e98e0 *base = D_L16_00161F60_bounds_2e98e0;
                L16SingleSphere_2e98e0 *sphere = base + i;
                sphere->radius = 0.0f;
                sphere->x = (maxx + minx) * 0.5f;
                sphere->y = (maxy + miny) * 0.5f;
                sphere->z = (maxz + minz) * 0.5f;
            }
            for (j = 0; j < ((int *)&D_L16_00161F90_gp)[i]; j++) {
                float vertex_to_center_distance;
                out = (float *)(D_L16_00161F60_bounds_2e98e0 + i);
                vertex_position[0] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 0];
                vertex_position[1] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 1];
                vertex_position[2] = ((float **)&D_L16_00161F98_gp)[i][j * 3 + 2];
                vertex_to_center_distance = fast_vec_dist(vertex_position, out);
                if (vertex_to_center_distance > out[3])
                    out[3] = vertex_to_center_distance;
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

int D_L16_00161FB8[4] __attribute__((section(".sdata"))) = {148, 148, 53, 0};

extern float D_L16_001DF7B0[];
extern float D_L16_001DFEA0[];
extern float D_L16_001E0590[];
float       *D_L16_00161FC8[4]
    __attribute__((section(".sdata"))) = {D_L16_001DF7B0, D_L16_001DFEA0, D_L16_001E0590, 0};

extern float D_L16_001E0810[];
extern float D_L16_001E0CB0[];
extern float D_L16_001E1150[];
float       *D_L16_00161FD8[4]
    __attribute__((section(".sdata"))) = {D_L16_001E0810, D_L16_001E0CB0, D_L16_001E1150, 0};

extern float D_L16_001E12F8[];
extern float D_L16_001E19E8[];
extern float D_L16_001E20D8[];
float       *D_L16_00161FE8[4]
    __attribute__((section(".sdata"))) = {D_L16_001E12F8, D_L16_001E19E8, D_L16_001E20D8, 0};

extern float D_L16_001E2358[];
extern float D_L16_001E25A8[];
extern float D_L16_001E27F8[];
float       *D_L16_00161FF8[4]
    __attribute__((section(".sdata"))) = {D_L16_001E2358, D_L16_001E25A8, D_L16_001E27F8, 0};

void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");

void draw_level16_reflection_mesh_batch(void) {
    int i;
    vu1_add_g_sregister(6, get_effect_texture(0x29));
    vu1_add_g_sregister(0x42, 0x4000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    for (i = 0; i < 3; i++) {
        compute_level16_reflection_map_coordinates(D_L16_00161FB8[i],
                                                   ((int *)D_L16_00161FE8)[i],
                                                   ((int *)D_L16_00161FC8)[i],
                                                   ((int *)D_L16_00161FD8)[i],
                                                   level16_reflection_mesh_coordinate_scratch);
        FUN_L00_001fde98(D_L16_00161FB8[i],
                         ((int *)D_L16_00161FC8)[i],
                         ((int *)D_L16_00161FF8)[i],
                         level16_reflection_mesh_coordinate_scratch,
                         1);
    }
}
