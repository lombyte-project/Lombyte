/* Moby paths: the next path point and an advanced path position. */
#include "types.h"
#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"

typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 point[1];
} PathPoints;

static inline s32 path_next(PathPoints *path, s32 i, s32 dir) {
    if (i == path->count - 1) {
        return dir > 0 ? 0 : path->count - 2;
    }
    if (i == 0) {
        return dir > 0 ? i + 1 : path->count - 1;
    }
    return i + dir;
}

s32 compute_advanced_moby_path_position(struct Moby *moby,
                                        PathPoints  *path,
                                        Vec4        *out,
                                        s32          i,
                                        s32          path_index_step,
                                        f32          advance_distance) __asm__("FUN_L09_00294ae0");

s32 compute_advanced_moby_path_position(struct Moby *moby,
                                        PathPoints  *path,
                                        Vec4        *advanced_path_position,
                                        s32          i,
                                        s32          path_index_step,

                                        f32 advance_distance) {
    Vec4 seg;
    Vec4 rel;
    s32  next_path_point_index;
    f32  segment_length;
    f32  along;

    next_path_point_index = path_next(path, i, path_index_step);
    fast_vec_sub(&seg, &path->point[next_path_point_index], &path->point[i]);
    segment_length = fast_vec_length(&seg);
    fast_vec_normalize(&seg, &seg, 1.0f);
    fast_vec_sub(&rel, &moby->pos, &path->point[i]);
    along = fast_vec_dot(&rel, &seg) + advance_distance;
    if (segment_length < along) {
        i = next_path_point_index;
        next_path_point_index = path_next(path, i, path_index_step);
        fast_vec_sub(&seg, &path->point[next_path_point_index], &path->point[i]);
        along = along - segment_length;
        fast_vec_normalize(&seg, &seg, 1.0f);
    }
    fast_vec_normalize(&seg, &seg, along);
    fast_vec_add(advanced_path_position, &seg, &path->point[i]);
    return i;
}
