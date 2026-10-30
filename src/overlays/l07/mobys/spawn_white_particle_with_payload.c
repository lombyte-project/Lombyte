/* Level 07 particle helpers: spawns a white particle with a payload, and its segment-sphere helper. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"

/* Spawn a particle moby at pos with a colour, lifetime and size. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/partupd_0029D2B8.c: func_L07_0029D2B8), where it is exact; names translated to the US level program. */

extern int FUN_L00_0025bfe0(float, float, float, float);

extern unsigned char *D_L07_001B2164 NOT_SDA;
void spawn_white_particle_with_payload(void *particle_position_source,
                                       void *particle_payload_source) __asm__("FUN_L07_0029c4e8");

void spawn_white_particle_with_payload(void *particle_position_source,

                                       void *particle_payload_source) {
    unsigned char *particle = create_part(0x41);
    if (particle) {
        float *particle_payload = (float *)(particle + 0x20);
        qcopy(particle + 0x10, particle_position_source);
        *(int *)(particle + 4) = FUN_L00_0025bfe0(1.0f, 1.0f, 1.0f, 0.6f);
        particle[9] = truncate_float_to_s32(4.0f) + 0x40;
        particle[1] = 0;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = random_float_between(20000.0f, 120000.0f);
        particle[8] = 0;
        particle[2] = *D_L07_001B2164;
        particle_payload[4] = 1.0f;
        particle_payload[5] = 1.0f;
        particle_payload[6] = 1.0f;
        particle_payload[7] = 0.6f;
        *(short *)(particle + 0xA) = scale_game_frames(0x50);
        qcopy(particle_payload, particle_payload_source);
    }
}

#include "rnc/math/vector.h"

/* Finds the point of segment a-b nearest to the surface of the sphere (center, radius), preferring
   the one closer to ref when both hits are equally good. Writes it to out and returns its distance
   from the surface (0 when within 1% of the radius). */
f32 select_segment_point_near_sphere_surface(Vec4f *out,
                                             Vec4f *ref,
                                             Vec4f *center,
                                             Vec4f *a,
                                             Vec4f *b,
                                             f32    radius) __asm__("FUN_L07_0029e6d8");

f32 select_segment_point_near_sphere_surface(

    Vec4f *out, Vec4f *ref, Vec4f *center, Vec4f *segment_start, Vec4f *segment_end, f32 radius) {
    Vec4f center_to_segment_start, segment_displacement, line_projection_point, lower_segment_point,
        upper_segment_point;
    f32 line_projection_t, lower_segment_t, upper_segment_t, intersection_t_offset, dd,
        center_distance_or_surface_error, lower_surface_error, upper_surface_error, eps;
    Vec4f *upper_segment_point_ptr;

    fast_vec_sub(&center_to_segment_start, segment_start, center);
    fast_vec_sub(&segment_displacement, segment_end, segment_start);
    dd = fast_vec_dot(&segment_displacement, &segment_displacement);
    line_projection_t = -fast_vec_dot(&center_to_segment_start, &segment_displacement) / dd;
    fast_vec_scale(&line_projection_point, &segment_displacement, line_projection_t);
    fast_vec_add(&line_projection_point, &line_projection_point, segment_start);
    center_distance_or_surface_error = fast_vec_dist(&line_projection_point, center);
    if (center_distance_or_surface_error < radius) {
        intersection_t_offset = square_root(radius * radius - center_distance_or_surface_error *
                                                                  center_distance_or_surface_error);
        intersection_t_offset /= fast_vec_length(&segment_displacement);
        lower_segment_t = line_projection_t - intersection_t_offset;
        if (lower_segment_t < 0.0f)
            lower_segment_t = 0.0f;
        else if (1.0f < lower_segment_t)
            lower_segment_t = 1.0f;
        upper_segment_t = line_projection_t + intersection_t_offset;
        if (upper_segment_t < 0.0f)
            upper_segment_t = 0.0f;
        else if (1.0f < upper_segment_t)
            upper_segment_t = 1.0f;
        fast_vec_scale(&lower_segment_point, &segment_displacement, lower_segment_t);
        fast_vec_add(&lower_segment_point, &lower_segment_point, segment_start);
        lower_surface_error = absolute_float(fast_vec_dist(&lower_segment_point, center) - radius);
        eps = radius * 0.01f;
        if (lower_surface_error < eps)
            lower_surface_error = 0.0f;
        upper_segment_point_ptr = &upper_segment_point;
        fast_vec_scale(upper_segment_point_ptr, &segment_displacement, upper_segment_t);
        fast_vec_add(upper_segment_point_ptr, upper_segment_point_ptr, segment_start);
        upper_surface_error =
            absolute_float(fast_vec_dist(upper_segment_point_ptr, center) - radius);
        if (upper_surface_error < eps)
            upper_surface_error = 0.0f;
        if (lower_surface_error < upper_surface_error ||
            (lower_surface_error == upper_surface_error &&
             fast_vec_dist(&lower_segment_point, ref) <
                 fast_vec_dist(upper_segment_point_ptr, ref))) {
            qcopy(out, &lower_segment_point);
            center_distance_or_surface_error = lower_surface_error;
        } else {
            qcopy(out, upper_segment_point_ptr);
            center_distance_or_surface_error = upper_surface_error;
        }
    } else {
        if (line_projection_t < 0.0f) {
            qcopy(out, segment_start);
            center_distance_or_surface_error = absolute_float(fast_vec_dist(out, center) - radius);
        } else if (1.0f < line_projection_t) {
            qcopy(out, segment_end);
            center_distance_or_surface_error = absolute_float(fast_vec_dist(out, center) - radius);
        } else {
            qcopy(out, &line_projection_point);
        }
        if (center_distance_or_surface_error < radius * 0.01f)
            center_distance_or_surface_error = 0.0f;
    }
    return center_distance_or_surface_error;
}
