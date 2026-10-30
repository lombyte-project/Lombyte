/* Level 18 moby class grind_rail_junction (0x246): pairs rail splines that meet and enables the one nearer the hero; spawns two nanotech balls. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "rnc/overlay/l18/mobys/grind_rail_junction_globals.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002E36B0), where it is exact; names translated to the US level program. */

char *create_nanotech_ball(void *vector) __asm__("FUN_L18_002e22c0");

/* pvars of FUN_L18_002d62e8: up to 4 pairs of paths that meet, the hero gets the nearer one */
typedef struct {
    struct {
        LevelSpline *first;
        LevelSpline *second;
    } pair[4]; /* 0x00: second starts where first's last point is */
    struct {
        f32 first;
        f32 second;
    } radius[4]; /* 0x20: their radii before they were cleared */
    s32 node[2]; /* 0x40: indexes into D_L18_001600EC */
} PathJunctionVars;

extern char *D_L18_001600EC;
extern char *create_nanotech_ball(void *vector) __asm__("FUN_L18_002e22c0");

void update_grind_rail_junction(struct Moby *moby) __asm__("FUN_L18_002d62e8");

void update_grind_rail_junction(struct Moby *moby) {
    enum {
        STATE_INITIALIZE_PATH_PAIRS = 0,
        STATE_SELECT_NEARER_PATH = 1
    };
    PathJunctionVars *vars = (PathJunctionVars *)moby->pvars;
    s32               pair_or_node_index;
    s32               i;
    s32               j;
    s32               k;
    f32               near_first;

    switch (moby->state) {
    case STATE_INITIALIZE_PATH_PAIRS:
        moby->state = 1;
        moby->update_dist = 0xFF;
        pair_or_node_index = 0;
        do {
            for (i = 0; i < path_link_count; i++) {
                if (l18_splines[i].bounds.f[3] == 0.0f) {
                    continue;
                }
                vars->pair[pair_or_node_index].first = &l18_splines[i];
                vars->radius[pair_or_node_index].first = l18_splines[i].bounds.f[3];
                for (j = 0; j < path_link_count; j++) {
                    if (l18_splines[j].bounds.f[3] == 0.0f) {
                        continue;
                    }
                    if (fast_vec_dist(
                            &l18_splines[i].path->points[0],
                            &l18_splines[j].path->points[l18_splines[j].path->count - 1]) < 0.5f) {
                        vars->pair[pair_or_node_index].second = &l18_splines[j];
                        vars->radius[pair_or_node_index].second = l18_splines[j].bounds.f[3];
                        l18_splines[j].bounds.f[3] = 0.0f;
                        l18_splines[i].bounds.f[3] = 0.0f;
                        pair_or_node_index++;
                        break;
                    }
                }
            }
        } while (pair_or_node_index < 4);
        for (pair_or_node_index = 0; pair_or_node_index < 2; pair_or_node_index++) {
            create_nanotech_ball(D_L18_001600EC + vars->node[pair_or_node_index] * 0x80 + 0x30);
        }
        break;
    case STATE_SELECT_NEARER_PATH:
        hero.rail_catch_grace = 5;
        for (k = 0; k < 4; k++) {
            if (hero.state.control_mode == 0xF &&
                (hero.rail_spline == (s32)vars->pair[k].first->path ||
                 hero.rail_spline == (s32)vars->pair[k].second->path)) {
                continue;
            }
            near_first =
                vector_distance_xy(&hero.motion.pos, &vars->pair[k].first->path->points[0]);
            if (near_first <
                vector_distance_xy(&hero.motion.pos, &vars->pair[k].second->path->points[0])) {
                vars->pair[k].first->bounds.f[3] = vars->radius[k].first;
                vars->pair[k].second->bounds.f[3] = 0.0f;
            } else {
                vars->pair[k].first->bounds.f[3] = 0.0f;
                vars->pair[k].second->bounds.f[3] = vars->radius[k].second;
            }
        }
        break;
    }
}

#include "qcopy.h"

extern void *create_moby() __asm__("FUN_0020c4f8");

char *create_nanotech_ball(void *vector) {
    char *moby = create_moby(0x326);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0x40;
        moby[0x20] = 0;
        *(char **)(data + 0x0C) = moby;
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags |= 0x41;
        qcopy(moby + 0x10, vector);
        clear_u64_value(moby + 0x40);
        euler_rotation_basis(moby + 0xC0, moby + 0x40);
    }
    return moby;
}
