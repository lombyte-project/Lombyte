/* Moby paths: clear path point masks, and following a path. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "asm.h"

/* 0x00276c40, 112 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Expands a bit mask through a table of per-bit masks until it meets another mask.
int expand_path_mask_to_intersection(int path_index,
                                     int from_mask,
                                     int mask) __asm__("FUN_L01_00276c40");

int expand_path_mask_to_intersection(int path_index, int from_mask, int mask) {
    char *path_data = ((int)l01_paths[path_index]);
    int   i;
    int   j;
    int   expanded_mask;
    for (i = 0; i < 32; i++) {
        if (from_mask & mask) {
            return from_mask & mask;
        }
        expanded_mask = mask;
        for (j = 0; j < 32; j++) {
            if ((mask >> j) & 1) {
                expanded_mask |= *(int *)(path_data + 0x1C + j * 16);
            }
        }
        mask = expanded_mask;
    }
    return 0;
}

#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

/* A path in D_L01_001B0930: point count, then the points. */
typedef struct {
    s32     count;
    u8      pad04[0xC];
    OvlVec4 pts[1];
} Path;

extern s32 test_path_segment_strict_crossing(s32, OvlVec4 *, OvlVec4 *) __asm__("FUN_L00_00261968");

/* Bit i set for each point i of the path that none of the n segments in ids
   reaches from pos (with a sideways margin of radius when it is nonzero). */
s32 compute_clear_path_point_mask(s32     *ids,
                                  s32      id_count,
                                  f32      radius,
                                  s32      path_index,
                                  OvlVec4 *segment_origin) __asm__("FUN_L01_00276a48");

s32 compute_clear_path_point_mask(

    s32 *ids, s32 id_count, f32 radius, s32 path_index, OvlVec4 *segment_origin) {
    Path   *path = ((Path *)l01_paths[path_index]);
    OvlVec4 side;
    OvlVec4 left;
    OvlVec4 right;
    OvlVec4 up;
    s32     mask = 0;
    s32     i;
    s32     j;
    s32     hit;

    for (i = 0; i < path->count; i++) {
        hit = 0;
        for (j = 0; j < id_count; j++) {
            if (radius != 0.0f) {
                up.q = 0;
                up.f[2] = 1.0f;
                fast_vec_sub(&side, segment_origin, &path->pts[i]);
                fast_vec_cross(&side, &side, &up);
                fast_vec_normalize(&side, &side, radius);
                fast_vec_sub(&left, segment_origin, &side);
                fast_vec_add(&right, segment_origin, &side);
                if (test_path_segment_strict_crossing(ids[j], &left, &path->pts[i])) {
                    hit = 1;
                } else if (test_path_segment_strict_crossing(ids[j], &right, &path->pts[i])) {
                    hit = 1;
                }
            } else if (test_path_segment_strict_crossing(ids[j], segment_origin, &path->pts[i])) {
                hit = 1;
            }
            if (hit) {
                break;
            }
        }
        if (!hit) {
            mask |= 1 << i;
        }
    }
    return mask;
}

s32 follow_path(s32     *ids,
                s32      n,
                s32      path_index,
                f32      radius,
                OvlVec4 *pos,
                OvlVec4 *target,
                OvlVec4 *out) __asm__("FUN_L01_00276fe8");
s32 follow_path(s32     *ids,
                s32      id_count,
                s32      path_index,
                f32      radius,
                OvlVec4 *start_position,
                OvlVec4 *target,

                OvlVec4 *path_goal_position) {
    OvlVec4 side;
    OvlVec4 left;
    OvlVec4 right;
    Path   *path;
    s32     hit = 0;
    s32     mask;
    s32     best;
    s32     i;
    s32     j;
    f32     nearest;
    f32     x;
    s32     from_mask, to_mask;

    for (j = 0; j < id_count; j++) {
        if (radius != 0.0f) {
            fast_vec_sub(&side, start_position, target);
            fast_vec_normalize(&side, &side, radius);
            x = side.f[0];
            side.f[3] = x;
            side.f[0] = side.f[1];
            side.f[1] = -x;
            fast_vec_sub(&left, start_position, &side);
            fast_vec_add(&right, start_position, &side);
            if (test_path_segment_strict_crossing(ids[j], &left, target)) {
                hit = 1;
            } else if (test_path_segment_strict_crossing(ids[j], &right, target)) {
                hit = 1;
            }
        } else if (test_path_segment_strict_crossing(ids[j], start_position, target)) {
            hit = 1;
        }
        if (hit)
            break;
    }
    if (hit) {
        path = ((Path *)l01_paths[path_index]);
        from_mask =
            compute_clear_path_point_mask(ids, id_count, radius, path_index, start_position);
        to_mask = compute_clear_path_point_mask(ids, id_count, radius, path_index, target);
        mask = expand_path_mask_to_intersection(path_index, from_mask, to_mask);
        if (mask != 0) {
            nearest = 10000.0f;
            best = -1;
            for (i = 0; i < path->count; i++) {
                if ((mask >> i) & 1) {
                    f32 distance = vector_distance_xy(&path->pts[i], target);
                    if (distance < nearest) {
                        nearest = distance;
                        best = i;
                    }
                }
            }
            if (best >= 0) {
                qcopy(path_goal_position, &path->pts[best]);
                return 1;
            }
        }
        return 0;
    }
    qcopy(path_goal_position, target);
    return 1;
}
