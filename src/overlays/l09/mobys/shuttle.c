/* Level 09 moby class 0x6e6: shuttle. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/level_path.h"

extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern char D_0013E550[];

#include "rnc/mobys/moby.h"

extern int  D_L09_0015F5CC;
extern void FUN_L00_001fff28(void *, int, float);

extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern unsigned char D_0013F3D0[];

typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 node[1];
} PathNodes_00309c08;

typedef struct {
    u8  pad0[0x70];
    u8  unk70[0x38];
    f32 speed;
    s32 path;
    u8  padB0[8];
    s16 dir;
    s16 node;
    f32 yaw_offset;
    f32 spin;
    u8  padC4[8];
    f32 lag;
} PathFlyerVars_00309c08;

extern void               scale_vector_00309c08(f32, void *, void *) __asm__("FUN_001f9a68");
extern u8                 D_0013F3D0[];
extern u8                 D_0014C050[];
extern s32 D_L09_0015F5CC NOT_SDA;
extern int                advance_shuttle_along_path(struct Moby *) __asm__("FUN_L09_0030a298");
void                      shuttle_update(struct Moby *moby) __asm__("FUN_L09_00309c08");

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_0030B5E8.c: func_L09_0030B5E8), where it is exact; names translated to the US level program. */
void interpolate_rotation_angle(float *output_angle,
                                float  current_angle,
                                float  target_angle,
                                float  scale) __asm__("FUN_L09_0030a238");

typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 node[1];
} PathNodes;

typedef struct {
    u8  pad0[0x70];
    u8  unk70[0x38];
    f32 speed; /* 0xA8 */
    s32 path;  /* 0xAC: index into D_L09_001B0630 */
    u8  padB0[8];
    s16 dir;        /* 0xB8: > 0 runs the path forward, else backward */
    s16 node;       /* 0xBA: node being flown to */
    f32 yaw_offset; /* 0xBC */
    f32 spin;       /* 0xC0 */
    u8  padC4[8];
    f32 lag; /* 0xCC: eases to 0, slowing the turn while high */
} PathFlyerVars;

extern f32 D_L09_00161DF8 __attribute__((sda));
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern void add_random_offset_to_vector_xyz(void *, f32) __asm__("FUN_L00_0025f8e0");
extern void spawn_moving_resizing_height_snapped_alpha_fade_particle(
    void *, int, int, float, float) __asm__("FUN_L00_002715e8");

/* Moby class 1766 */
void shuttle_update(struct Moby *moby) {
    enum {
        SHUTTLE_STATE_INITIALIZE = 0,
        SHUTTLE_STATE_WAIT_FOR_DEPARTURE = 1,
        SHUTTLE_STATE_WAIT_FOR_DISMOUNT = 2,
        SHUTTLE_STATE_FOLLOW_PATH = 3
    };
    PathFlyerVars_00309c08 *shuttle_vars = (PathFlyerVars_00309c08 *)moby->pvars;
    PathNodes_00309c08     *path = (PathNodes_00309c08 *)l09_paths[shuttle_vars->path];
    Vec4                   *position = &moby->pos;
    Vec4                    original;
    Vec4                    rotation;
    s16                     dir;
    s32                     voice;

    scale_vector_00309c08(-1.0f, &original, position);
    qcopy(&rotation, &moby->rot);
    if (shuttle_vars == 0)
        return;

    switch (moby->state) {
    case SHUTTLE_STATE_INITIALIZE:
        moby->update_dist = 0xFF;
        shuttle_vars->yaw_offset = 3.1415927f;
        *(s32 *)((u8 *)shuttle_vars + 0xC4) = -1;
        *(s32 *)((u8 *)shuttle_vars + 0xC8) = 0;
        shuttle_vars->lag = 0.0f;
        if (*(s32 *)((u8 *)shuttle_vars + 0xA0) != 0) {
            PathNodes_00309c08 *path;
            Vec4               *endpoint;
            attach_manipulator(moby, 0, (u8 *)shuttle_vars + 0x60);
            path = (PathNodes_00309c08 *)l09_paths[shuttle_vars->path];
            moby->state = 1;
            if (D_0014C050[moby->mission + current_level_index * 16] == 0xFF) {
                shuttle_vars->dir = -1;
                shuttle_vars->node = path->count - 2;
                endpoint = (Vec4 *)((path->count << 4) + (s32)path);
            } else {
                shuttle_vars->dir = 1;
                endpoint = &path->node[0];
                shuttle_vars->node = 1;
            }
            *position = *endpoint;
            moby->rot.z = fast_arc_tan(path->node[shuttle_vars->node].f[0] - moby->pos.x,
                                       path->node[shuttle_vars->node].f[1] - moby->pos.y);
        }
        moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
        break;
    case SHUTTLE_STATE_WAIT_FOR_DISMOUNT:
        shuttle_vars->lag += (1.0f - shuttle_vars->lag) * 0.01f;
        update_moby_vertical_oscillation(shuttle_vars->lag * 0.14f,
                                         frame_time * 0.5235988f,
                                         (char *)moby,
                                         (f32 *)((u8 *)shuttle_vars + 0xD8),
                                         (f32 *)((u8 *)shuttle_vars + 0xDC));
        update_moby_rotation_oscillation(moby,
                                         (char *)shuttle_vars + 0xD0,
                                         (char *)shuttle_vars + 0xD4,
                                         shuttle_vars->lag * 0.08726646f,
                                         frame_time * 0.31415927f,
                                         frame_time * 0.4886922f);
        if (hero.ground_moby != moby)
            moby->state = 1;
        break;
    case SHUTTLE_STATE_WAIT_FOR_DEPARTURE:
        shuttle_vars->lag += (1.0f - shuttle_vars->lag) * 0.01f;
        update_moby_vertical_oscillation(shuttle_vars->lag * 0.14f,
                                         frame_time * 0.5235988f,
                                         (char *)moby,
                                         (f32 *)((u8 *)shuttle_vars + 0xD8),
                                         (f32 *)((u8 *)shuttle_vars + 0xDC));
        update_moby_rotation_oscillation(moby,
                                         (char *)shuttle_vars + 0xD0,
                                         (char *)shuttle_vars + 0xD4,
                                         shuttle_vars->lag * 0.08726646f,
                                         frame_time * 0.31415927f,
                                         frame_time * 0.4886922f);
        if (*(s32 *)((u8 *)shuttle_vars + 0xC8) != 0) {
            if ((shuttle_vars->dir > 0 && *(s32 *)((u8 *)shuttle_vars + 0xB4) != -1 &&
                 is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)shuttle_vars + 0xB4))) ||
                (shuttle_vars->dir < 0 && *(s32 *)((u8 *)shuttle_vars + 0xB0) != -1 &&
                 is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)shuttle_vars + 0xB0)))) {
                moby->state = 3;
            }
        }
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            *(s32 *)((u8 *)shuttle_vars + 0xC8) = 1;
            moby->state = 3;
        }
        break;
    case SHUTTLE_STATE_FOLLOW_PATH:
        if ((D_L09_0015F5CC & 7) == (((s32)moby >> 8) & 7)) {
            if (*(s32 *)((u8 *)shuttle_vars + 0xC4) == -1 ||
                (!voice_is_playing(moby, *(s32 *)((u8 *)shuttle_vars + 0xC4)) &&
                 *(s32 *)((u8 *)shuttle_vars + 0xC4) == -1)) {
                *(s32 *)((u8 *)shuttle_vars + 0xC4) = allocate_voice_for_target_entry(0, 4, moby);
            }
        }
        if (shuttle_vars->yaw_offset == 0.0f) {
            if (shuttle_vars->dir < 0) {
                if (*(s32 *)((u8 *)shuttle_vars + 0xB4) == -1 ||
                    !is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)shuttle_vars + 0xB4)))
                    goto test_forward;
                if (!(vector_distance_xy(&moby->pos, &path->node[path->count - 1]) > 8.0f) ||
                    !(vector_distance_xy(&moby->pos, &path->node[0]) > 8.0f))
                    goto advance_path;
                shuttle_vars->dir = 1;
                shuttle_vars->yaw_offset = 3.1415927f;
            } else {
            test_forward:
                if (shuttle_vars->dir > 0 && *(s32 *)((u8 *)shuttle_vars + 0xB0) != -1 &&
                    is_point_inside_clip_volume(D_0013F3D0, *(s32 *)((u8 *)shuttle_vars + 0xB0)) &&
                    vector_distance_xy(&moby->pos, &path->node[path->count - 1]) > 8.0f &&
                    vector_distance_xy(&moby->pos, &path->node[0]) > 8.0f) {
                    shuttle_vars->dir = -1;
                    shuttle_vars->yaw_offset = 3.1415927f;
                }
            }
        }
    advance_path:
        if (advance_shuttle_along_path(moby)) {
            voice = *(s32 *)((u8 *)shuttle_vars + 0xC4);
            if (voice != -1) {
                u8 *shuttle_voice_entry = (u8 *)D_0013E550 + voice * 0x70;
                if (*(struct Moby **)(shuttle_voice_entry + 0x88) == moby &&
                    shuttle_voice_entry[0x74] != 0)
                    release_voice_slot(voice);
            }
            *(s32 *)((u8 *)shuttle_vars + 0xC4) = -1;
            shuttle_vars->lag = 0.0f;
            *(s32 *)((u8 *)shuttle_vars + 0xDC) = 0;
            moby->state = 2;
            shuttle_vars->yaw_offset = 3.1415927f;
            dir = shuttle_vars->dir;
            shuttle_vars->dir = -dir;
            if ((s16)-dir > 0)
                shuttle_vars->node = 0;
            else
                shuttle_vars->node = path->count - 1;
        }
        break;
    }
    fast_vec_add(&original, &original, &moby->pos);
    update_entity_matrix((u8 *)shuttle_vars + 0x20, &original, &rotation, &moby->rot);
}

void interpolate_rotation_angle(float *output_angle,
                                float  current_angle,
                                float  target_angle,

                                float scale) {
    float scaled_angle_delta = fast_subtract_rotations(target_angle, current_angle) * scale;
    *output_angle = scaled_angle_delta;
    *output_angle = fast_add_rotations(scaled_angle_delta, current_angle);
}

/* Flies the moby along its path toward the current node, turning and banking
 * toward it, steps the node at each arrival and now and then drops a puff.
 * Returns nonzero once it has run off the end of the path. */
int advance_shuttle_along_path(struct Moby *moby) {
    PathFlyerVars *shuttle_path_state = (PathFlyerVars *)moby->pvars;
    PathNodes     *path;
    f32            endpoint_distance;
    Vec4           target;
    Vec4           step;
    Vec4           puff;
    int            done = 0;

    path = (PathNodes *)l09_paths[shuttle_path_state->path];
    shuttle_path_state->lag += (0.0f - shuttle_path_state->lag) * 0.005f;
    qcopy(&target, &path->node[shuttle_path_state->node]);
    target.f[2] += shuttle_path_state->speed * D_L09_00161DF8;
    fast_vec_sub(&step, &target, &moby->pos);
    fast_vec_normalize(&step, &step, shuttle_path_state->speed);
    fast_vec_add(&moby->pos, &moby->pos, &step);
    interpolate_rotation_angle(
        &moby->rot.z,
        moby->rot.z,
        fast_add_rotations(fast_arc_tan(path->node[shuttle_path_state->node].f[0] - moby->pos.x,
                                        path->node[shuttle_path_state->node].f[1] - moby->pos.y),
                           shuttle_path_state->yaw_offset),
        (1.0f - shuttle_path_state->lag) * 0.02f);
    interpolate_rotation_angle(&moby->rot.x, moby->rot.x, 0.0f, 0.02f);
    interpolate_rotation_angle(&moby->rot.y, moby->rot.y, 0.0f, 0.02f);
    if (shuttle_path_state->dir > 0) {
        f32 v;
        if (fast_vec_dist(&target, &moby->pos) <
            shuttle_path_state->speed + shuttle_path_state->speed) {
            shuttle_path_state->node++;
            if (shuttle_path_state->node == path->count)
                done = 1;
        }
        endpoint_distance = fast_vec_dist(&moby->pos, &path->node[path->count - 1]);
        v = 0.0f;
        advance_accelerated_scalar(endpoint_distance,
                                   frame_time_sq * 3.0f,
                                   frame_time_sq * 3.0f,
                                   frame_time * 10.0f,
                                   &v,
                                   &shuttle_path_state->speed);
    } else {
        f32 v;
        if (fast_vec_dist(&target, &moby->pos) <
            shuttle_path_state->speed + shuttle_path_state->speed) {
            shuttle_path_state->node--;
            if (shuttle_path_state->node == -1)
                done = 1;
        }
        endpoint_distance = fast_vec_dist(&moby->pos, &path->node[0]);
        v = 0.0f;
        advance_accelerated_scalar(endpoint_distance,
                                   frame_time_sq * 3.0f,
                                   frame_time_sq * 3.0f,
                                   frame_time * 10.0f,
                                   &v,
                                   &shuttle_path_state->speed);
    }
    if (path->node[shuttle_path_state->node].f[3] == 42.0f)
        shuttle_path_state->yaw_offset = 0.0f;
    shuttle_path_state->spin =
        fast_add_rotations(shuttle_path_state->spin, shuttle_path_state->speed);
    FUN_L00_001fff28(shuttle_path_state->unk70, 0, shuttle_path_state->spin);
    if (vector_distance_xy(&moby->pos, &path->node[path->count - 1]) > 8.0f &&
        vector_distance_xy(&moby->pos, &path->node[0]) > 8.0f && !random_integer_below(7)) {
        qcopy(&puff, &moby->pos);
        add_random_offset_to_vector_xyz(&puff, 0.5f);
        puff.f[2] = 25.01f;
        spawn_moving_resizing_height_snapped_alpha_fade_particle(&puff,
                                                                 0,
                                                                 0x40103080,
                                                                 shuttle_path_state->speed * 3.0f /
                                                                     (frame_time * 10.0f),
                                                                 12600.0f);
    }
    return done;
}
