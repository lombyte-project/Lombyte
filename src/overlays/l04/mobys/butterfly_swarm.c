/* Level 04 moby class butterfly_swarm (0x56): steers the butterflies (class 0x55) along a path or a circle, with its steering, nearest-point, sphere-collision and polygon-edge helpers. */
#include "types.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/world/collision/collision_line.h"

/* index of the nearest of n vectors to a + 0x10 */
/* Ported from rac1-decomp (src/overlays/l04_eudora/fastfunc_001F2F68.c: func_L04_001F2F68), where it is exact; names translated to the US level program. */

int find_nearest_point_index_xy(char *moby, char *points, int count) __asm__("FUN_L04_001f3038");

int find_nearest_point_index_xy(char *moby, char *points, int count) {
    int   best = -1;
    int   i;
    float best_distance = 32767.0f;
    for (i = 0; i < count; i++) {
        float distance = vector_distance_xy(moby + 0x10, points + i * 0x10);
        if (distance < best_distance) {
            best_distance = distance;
            best = i;
        }
    }
    return best;
}

#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"
#include "rnc/overlay/level_path.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/fastfunc_001F2F68.c: func_L04_001F3010), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_1F3010;

typedef union {
    Vec_1F3010 v;
    int        n;
} PathEl;

typedef struct {
    Vec_1F3010  vel;
    Vec_1F3010  pos;
    Vec_1F3010  dir;
    Vec_1F3010  drift;
    float       range;
    short       timer;
    short       pad46;
    float       f48;
    float       near;
    Moby       *target;
    Vec_1F3010 *point;
    int         pad58;
    short       path;
    short       count;
    short       classes[8];
    float       weight[8];
    float       scale[8];
} Steer;

extern s32 D_L04_00173FF0[];
extern int collision_line() __asm__("FUN_001efa68");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
int        find_closest_polygon_edge_point(float *out,
                                           float *p,
                                           float *poly,
                                           int    n) __asm__("FUN_L04_0024c7d8");

#include "rnc/overlay/collision.h"

extern CollisionHit coll_hit __asm__("D_L04_00173FC0");

void steer_child_moby(Moby **others,
                      int    other_count,
                      Moby  *moby,
                      Steer *steer,
                      float *params) __asm__("FUN_L04_001f30e0");

void steer_child_moby(

    Moby **others, int other_count, Moby *moby, Steer *steer, float *steering_settings) {
    Vec_1F3010 path_hit;
    Vec_1F3010 force;
    Vec_1F3010 uninitialized_point;
    Vec_1F3010 ray_start;
    Vec_1F3010 ray_end;
    Vec_1F3010 surface_normal;
    PathEl    *path;
    Moby      *repulsion_candidate;
    int        hit;
    Moby      *too_close;
    int        nearest_index;
    int        i;
    int        best_index;
    int        class_index;
    int        next_index;
    int        previous_index;
    int        point_count;
    float      distance;
    float      length;
    float      next_distance;
    float      repulsion;

    clear_u64_value(steer);
    distance = fast_vec_dist(&moby->pos, &uninitialized_point);
    nearest_index = 0;
    path = 0;
    hit = 0;
    too_close = 0;
    if (steer->path != -1) {
        path = ((PathEl *)l04_paths[steer->path]);
        nearest_index = find_nearest_point_index_xy(moby, &path[1], path[0].n);
        hit = find_closest_polygon_edge_point(&path_hit, &moby->pos, &path[1], path[0].n);
        if (hit == 0) {
            fast_vec_sub(&force, &path_hit, &moby->pos);
            length = fast_vec_length(&force);
            if (length != 0.0f) {
                length = -steering_settings[2] / (length * length);
            } else {
                length = -1000.0f;
            }
        } else {
            fast_vec_sub(&force, &path_hit, &moby->pos);
            length = fast_vec_length(&force) + 3.0f;
            if (distance < length) {
                length = length * (steering_settings[2] * length);
            } else {
                length = 10000.0f;
            }
        }
        fast_vec_normalize(&force, &force, length);
        fast_vec_add(steer, steer, &force);
    }
    if (steering_settings[0] != 0.0f) {
        fast_vec_normalize(&force, &steer->dir, steering_settings[0]);
        fast_vec_add(steer, steer, &force);
    }
    if (steer->target == moby) {
        qcopy(&force, &steer->pos);
        fast_vec_normalize(&force, &force, steering_settings[1]);
        fast_vec_add(steer, steer, &force);
    } else {
        if (steer->path != -1) {
            best_index = nearest_index;
            length = fast_vec_dist(&path[nearest_index + 1], &uninitialized_point);
            next_index = 0;
            if (nearest_index + 1 < path[0].n) {
                next_index = nearest_index + 1;
            }
            next_distance = fast_vec_dist(&path[next_index + 1], &uninitialized_point);
            if (length < next_distance) {
                best_index = next_index;
                length = next_distance;
            }
            next_index = nearest_index - 1;
            if (next_index < 0) {
                next_index = path[0].n - 1;
            }
            if (length < fast_vec_dist(&path[next_index + 1], &uninitialized_point)) {
                best_index = next_index;
            }
            next_index = 0;
            point_count = path[0].n;
            if (best_index + 1 < point_count) {
                next_index = best_index + 1;
            }
            previous_index = best_index - 1;
            if (previous_index <= -1) {
                previous_index = point_count - 1;
            }
            fast_vec_add(&force, &path[next_index + 1], &path[previous_index + 1]);
            fast_vec_add(&force, &force, &path[best_index + 1]);
            fast_vec_scale(&force, &force, 0.33333334f);
            if (hit == 0) {
                fast_vec_sub(&force, &force, &moby->pos);
                length = fast_vec_length(&force);
                fast_vec_normalize(
                    &force, &force, length * (length * length * steering_settings[2]));
                fast_vec_add(steer, steer, &force);
            }
        }
        if (steer->target != 0) {
            fast_vec_sub(&force, &steer->target->pos, &moby->pos);
            if (fast_vec_length(&force) < distance) {
                length = fast_vec_length(&force);
                fast_vec_normalize(
                    &force, &force, length * (length * length * steering_settings[1]));
                fast_vec_add(steer, steer, &force);
            }
        }
        if (steer->point != 0) {
            force.x = steer->point->x - moby->pos.x;
            force.y = steer->point->y - moby->pos.y;
            force.z = steer->point->z - moby->pos.z;
            if (fast_vec_length(&force) < distance) {
                length = fast_vec_length(&force);
                fast_vec_normalize(
                    &force, &force, length * (length * length * steering_settings[1]));
                fast_vec_add(steer, steer, &force);
            }
        }
        for (i = 0; i < other_count + 1; i++) {
            if (i < other_count) {
                repulsion_candidate = others[i];
            } else {
                repulsion_candidate = hero.moby;
            }
            class_index = 0;
            if ((*(signed char *)&repulsion_candidate->state) < 0 || moby == repulsion_candidate) {
                continue;
            }
            length = fast_vec_dist(&moby->pos, &repulsion_candidate->pos);
            if (steer->near < length) {
                continue;
            }
            do {
                if (repulsion_candidate->oclass == steer->classes[class_index]) {
                    break;
                }
                class_index++;
            } while (class_index < steer->count);
            if (class_index < steer->count) {
                if (length < steering_settings[4]) {
                    too_close = repulsion_candidate;
                    break;
                }
                repulsion = length / steer->scale[class_index];
                if (repulsion != 0.0f) {
                    repulsion = steer->weight[class_index] / (repulsion * repulsion);
                } else {
                    repulsion = 1000.0f;
                }
                fast_vec_sub(&force, &moby->pos, &repulsion_candidate->pos);
                fast_vec_normalize(&force, &force, repulsion);
                fast_vec_add(steer, steer, &force);
            }
        }
    }
    if (steer->f48 == 0.0f) {
        fast_vec_add(&ray_start, &moby->pos, &steer->pos);
        fast_vec_add(&ray_end, &moby->pos, &steer->pos);
        ray_start.z = ray_start.z + 1.5f;
        ray_end.z = 0.01f;
        if (collision_line(&ray_start, &ray_end, 2, moby, 0) != 0) {
            if (coll_hit.unk1C > 0 && coll_hit.point.f[2] > moby->pos.z) {
                qcopy(&surface_normal, &coll_hit.normal_x);
                length = fast_arc_tan(coll_hit.normal_z, vector_length_xy(&coll_hit.normal_x));
                set_vector_length_xy(&surface_normal, &surface_normal, 1.0f);
                fast_vec_scale(&surface_normal, &surface_normal, steering_settings[3]);
                fast_vec_add(steer, steer, &surface_normal);
                if (0.4f < length) {
                    fast_vec_scale(&surface_normal,
                                   &surface_normal,
                                   steering_settings[3] * (length * 10000.0f));
                    fast_vec_add(steer, steer, &surface_normal);
                }
                if (0.2f < moby->pos.z - coll_hit.point.f[2]) {
                    set_vector_length_xy(
                        &surface_normal, &steer->pos, -(steering_settings[3] * 1000.0f));
                    fast_vec_add(steer, steer, &surface_normal);
                }
            }
        }
    } else {
        if (coll_sphere(steering_settings[4], &moby->pos, 4, moby) != 0) {
            fast_vec_sub(&force, D_L04_00173FF0, &moby->pos);
            force.z = force.z + steering_settings[3];
            fast_vec_normalize(&force, &force, steering_settings[3]);
            fast_vec_add(steer, steer, &force);
        }
    }
    if (steer->f48 == 0.0f) {
        set_vector_length_xy(steer, steer, steering_settings[6]);
        steer->vel.z = 0.0f;
    } else {
        fast_vec_normalize(steer, steer, steering_settings[6]);
        steer->vel.z = steer->vel.z / steer->f48;
    }
    if (fast_dec_timer_s16(&steer->timer) != 0) {
        steer->timer = truncate_float_to_s32(random_float_between(45.0f, 90.0f));
        steer->drift.x = random_float_between(0.0f, steer->range) - steer->range * 0.5f;
        steer->drift.y = random_float_between(0.0f, steer->range) - steer->range * 0.5f;
        if (steer->f48 != 0.0f) {
            steer->drift.z = random_float_between(0.0f, steer->range) - steer->range * 0.5f;
        } else {
            steer->drift.z = 0.0f;
        }
    }
    fast_vec_add(steer, steer, &steer->drift);
    if (too_close != 0) {
        fast_vec_sub(&force, &moby->pos, &too_close->pos);
        if (steer->f48 != 0.0f) {
            fast_vec_normalize(steer, &force, steering_settings[6]);
        } else {
            set_vector_length_xy(steer, &force, steering_settings[6]);
        }
    }
}

/* steps a moby's position out of collision and aims it */
/* Ported from rac1-decomp (src/overlays/l04_eudora/fastfunc_001F2F68.c: func_L04_001F39B0), where it is exact; names translated to the US level program. */

void update_moby_movement_with_sphere_collision(float collision_z_offset,
                                                char *moby,
                                                char *movement_state,
                                                char *movement_params) __asm__("FUN_L04_001f3a80");

void update_moby_movement_with_sphere_collision(float collision_z_offset,
                                                char *moby,
                                                char *movement_state,

                                                char *movement_params) {
    int   i;
    float collision_probe_position[4];
    char *movement_delta = movement_state + 0x10;
    fast_vec_add(movement_delta, movement_delta, movement_state);
    fast_vec_normalize(movement_delta, movement_delta, *(float *)(movement_params + 0x1C));
    *(float *)(movement_state + 0x18) =
        *(float *)(movement_state + 0x18) / *(float *)(movement_state + 0x48);
    fast_vec_add(moby + 0x10, moby + 0x10, movement_delta);
    for (i = 0; i < 6; i++) {
        qcopy(collision_probe_position, moby + 0x10);
        collision_probe_position[2] = collision_probe_position[2] + collision_z_offset;
        if (coll_sphere(
                *(float *)(movement_params + 0x10) * 0.25f, collision_probe_position, 4, moby)) {
            qcopy(moby + 0x10, D_L04_00173FF0);
            MOBY(moby)->pos.z = MOBY(moby)->pos.z - collision_z_offset;
        } else {
            break;
        }
    }
    MOBY(moby)->rot.z = fast_add_rotations(
        fast_arc_tan(*(float *)(movement_state + 0x10), *(float *)(movement_state + 0x14)),
        3.14159265f);
}

typedef struct {
    float x, y, z, w;
} V_24c4f0;

int find_first_positive_polygon_edge_cross_xy(float *query_point,
                                              float *poly,
                                              int    vertex_count) __asm__("FUN_L04_0024c4f0");

int find_first_positive_polygon_edge_cross_xy(float *query_point, float *poly, int vertex_count) {
    V_24c4f0 *polygon_vertices = (V_24c4f0 *)poly;
    int       i;

    for (i = 0; i < vertex_count; i++) {
        if ((polygon_vertices[(i + 1) % vertex_count].x - polygon_vertices[i].x) *
                    (query_point[1] - polygon_vertices[i].y) -
                (polygon_vertices[(i + 1) % vertex_count].y - polygon_vertices[i].y) *
                    (query_point[0] - polygon_vertices[i].x) >
            0.0f) {
            return i + 1;
        }
    }
    return 0;
}

/* closest point on a line to a point: writes the point, returns the parameter */
/* Ported from rac1-decomp (src/overlays/l04_eudora/mobyutil_0024D4A8.c: func_L04_0024D618), where it is exact; names translated to the US level program. */

float find_closest_point_on_segment_xy(float *closest_point,
                                       float *segment_start,
                                       float *segment_end,
                                       float *query_point) __asm__("FUN_L04_0024c660");

float find_closest_point_on_segment_xy(float *closest_point,
                                       float *segment_start,
                                       float *segment_end,

                                       float *query_point) {
    float segment_direction[4];
    float perpendicular_direction_xy[4];
    float query_to_start_offset[4];
    float perpendicular_offset_point_xy[4];
    float det, unclamped_segment_parameter, clamped_segment_parameter;

    fast_vec_sub(segment_direction, segment_end, segment_start);
    fast_vec_sub(query_to_start_offset, segment_start, query_point);
    perpendicular_offset_point_xy[0] = segment_direction[1];
    perpendicular_offset_point_xy[1] = -segment_direction[0];
    fast_vec_add(perpendicular_offset_point_xy, query_point, perpendicular_offset_point_xy);
    fast_vec_sub(perpendicular_direction_xy, query_point, perpendicular_offset_point_xy);
    det = segment_direction[1] * perpendicular_direction_xy[0] -
          segment_direction[0] * perpendicular_direction_xy[1];
    if (absolute_float(det) < 0.0001f) {
        qcopy(closest_point, segment_start);
        return 0.0f;
    }
    det = 1.0f / det;
    unclamped_segment_parameter = perpendicular_direction_xy[1] * query_to_start_offset[0] -
                                  perpendicular_direction_xy[0] * query_to_start_offset[1];
    unclamped_segment_parameter = unclamped_segment_parameter * det;
    clamped_segment_parameter = unclamped_segment_parameter;
    if (clamped_segment_parameter < 0.0f)
        clamped_segment_parameter = 0.0f;
    if (1.0f < clamped_segment_parameter)
        clamped_segment_parameter = 1.0f;
    fast_vec_scale(closest_point, segment_direction, clamped_segment_parameter);
    fast_vec_add(closest_point, closest_point, segment_start);
    return unclamped_segment_parameter;
}

/* finds the closest point on a polygon's edges to a point, returns whether the first test hit */
/* Ported from rac1-decomp (src/overlays/l04_eudora/mobyutil_0024D4A8.c: func_L04_0024D790), where it is exact; names translated to the US level program. */

extern int
find_first_positive_polygon_edge_cross_xy(float *, float *, int) __asm__("FUN_L04_0024c4f0");

int find_closest_polygon_edge_point(float *out,
                                    float *query_point,
                                    float *poly,
                                    int    vertex_count) __asm__("FUN_L04_0024c7d8");

int find_closest_polygon_edge_point(float *closest_polygon_edge_point,
                                    float *query_point,
                                    float *poly,

                                    int vertex_count) {
    float edge_closest_point[4];
    float edge_to_query_offset[4];
    float best;
    int   i;
    int   has_positive_edge_cross =
        find_first_positive_polygon_edge_cross_xy(query_point, poly, vertex_count) != 0;
    best = 0;
    for (i = 0; i < vertex_count; i++) {
        float distance_to_edge;
        find_closest_point_on_segment_xy(
            edge_closest_point, poly + i * 4, poly + ((i + 1) % vertex_count) * 4, query_point);
        fast_vec_sub(edge_to_query_offset, query_point, edge_closest_point);
        distance_to_edge = fast_vec_length(edge_to_query_offset);
        if (i == 0 || distance_to_edge < best) {
            best = distance_to_edge;
            qcopy(closest_polygon_edge_point, edge_closest_point);
        }
    }
    return has_positive_edge_cross;
}

#include "rnc/globals.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/l04/mobys/butterfly_swarm_globals.h"

typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 point[1];
} PathPoints_eb20;

static inline s32 path_next_eb20(PathPoints_eb20 *path, s32 i, s32 dir) {
    if (i == path->count - 1) {
        return dir > 0 ? 0 : path->count - 2;
    }
    if (i == 0) {
        return dir > 0 ? i + 1 : path->count - 1;
    }
    return i + dir;
}

s32 advance_moby_along_path(struct Moby     *moby,
                            PathPoints_eb20 *path,
                            s32              i,
                            s32              path_index_step,
                            f32              advance_distance) __asm__("FUN_L04_0029eb20");

s32 advance_moby_along_path(

    struct Moby *moby, PathPoints_eb20 *path, s32 i, s32 path_index_step, f32 advance_distance) {
    Vec4 seg;
    Vec4 rel;
    s32  next_path_point_index;
    f32  segment_length;
    f32  along;

    next_path_point_index = path_next_eb20(path, i, path_index_step);
    fast_vec_sub(&seg, &path->point[next_path_point_index], &path->point[i]);
    segment_length = fast_vec_length(&seg);
    fast_vec_normalize(&seg, &seg, 1.0f);
    fast_vec_sub(&rel, &moby->pos, &path->point[i]);
    along = fast_vec_dot(&rel, &seg) + advance_distance;
    if (segment_length < along) {
        i = next_path_point_index;
        next_path_point_index = path_next_eb20(path, i, path_index_step);
        fast_vec_sub(&seg, &path->point[next_path_point_index], &path->point[i]);
        along = along - segment_length;
        fast_vec_normalize(&seg, &seg, 1.0f);
    }
    fast_vec_normalize(&seg, &seg, along);
    fast_vec_add(&moby->pos, &seg, &path->point[i]);
    return i;
}

extern char *D_L04_0015FFE4;

extern void            *memset(void *, int, int) __asm__("FUN_001153fc");
extern PathPoints_eb20 *D_L04_001b0630[];
f32                     D_L04_00166130[8] __attribute__((section(".bss")));

extern s32 swarm_motion_parameters_initialized_gp __asm__("D_L04_001513C0") __attribute__((sda));

/* Variables of a swarm child (class 0x55, butterfly). */
typedef struct SwarmPartVars {
    u8     pad0[0x10];
    Vec4f  velocity;
    u8     pad20[0x20];
    f32    unk40;
    s16    unk44;
    u8     pad46[2];
    f32    unk48;
    f32    unk4C;
    s32    unk50;
    Vec4f *leader_pos;
    s32    unk58;
    s16    unk5C;
    s16    unk5E;
    s16    oclass;
    s16    unk62;
    u8     pad64[0xC];
    f32    unk70;
    f32    unk74;
    u8     pad78[0x18];
    f32    unk90;
    f32    unk94;
} SwarmPartVars;

typedef struct PVars {
    Vec4f        origin;
    struct Moby *children[32];
    s32          count;
    s32          initialized;
    f32          angle;
    f32          radius;
    s32          path_index;
    s32          direction;
    s32          child_count;
} PVars;

void         butterfly_swarm_update(struct Moby *moby) __asm__("FUN_L04_0029ecf8");
extern float fast_cos(float);
extern float fast_sin(float);

/* Moby class 86 */
void butterfly_swarm_update(struct Moby *moby) {
    PVars        *swarm_controller_pvars = (PVars *)moby->pvars;
    struct Moby **children;
    struct Moby  *population_scan_moby;
    f32           distance;
    f32           angle;
    f32           angle2;
    f32           speed;
    f32           size;
    f32           average;
    f32           effect[8];

    if (swarm_motion_parameters_initialized_gp == 0) {
        f32 *effect_dest = D_L04_00166130;
        memset(effect, 0, 0x20);
        effect[1] = convert_integer_to_float(swarm_controller_pvars->child_count) * 2.073f;
        effect[3] = 2.0f;
        effect[4] = 1.0f;
        effect[5] = 1.0f;
        swarm_motion_parameters_initialized_gp = 1;
        __builtin_memcpy(effect_dest, effect, 0x20);
    }
    if (swarm_controller_pvars->initialized == 0) {
        qcopy(&swarm_controller_pvars->origin, &moby->pos);
        swarm_controller_pvars->angle = 0.0f;
    } else if (swarm_controller_pvars->child_count == 0) {
        for (population_scan_moby = (struct Moby *)D_L04_0015FFE4; population_scan_moby != 0;
             population_scan_moby = population_scan_moby->next) {
            if ((s8)population_scan_moby->state >= 0 && population_scan_moby->oclass == 0x55) {
                swarm_controller_pvars->child_count++;
            }
        }
        D_L04_00166130[1] = convert_integer_to_float(swarm_controller_pvars->child_count) * 2.073f;
    }
    if (swarm_controller_pvars->path_index == -1) {
        swarm_controller_pvars->angle =
            fast_add_rotations(swarm_controller_pvars->angle, frame_scale * 0.0034906587f);
        moby->pos.x = swarm_controller_pvars->origin.x +
                      swarm_controller_pvars->radius * fast_cos(swarm_controller_pvars->angle);
        moby->pos.y = swarm_controller_pvars->origin.y +
                      swarm_controller_pvars->radius * fast_sin(swarm_controller_pvars->angle);
    } else if (swarm_controller_pvars->initialized != 0) {
        s32 i;
        distance = 10000.0f;
        for (i = 0; i < swarm_controller_pvars->count; i++) {
            f32 child_distance =
                vector_distance_xy(&moby->pos, &swarm_controller_pvars->children[i]->pos);
            if (child_distance < distance)
                distance = child_distance;
        }
        speed = 0.0f;
        if (distance < 3.0f)
            speed = 3.0f - distance;
        swarm_controller_pvars->direction =
            advance_moby_along_path(moby,
                                    D_L04_001b0630[swarm_controller_pvars->path_index],
                                    swarm_controller_pvars->direction,
                                    1,
                                    speed);
    }
    if (swarm_controller_pvars->initialized == 0) {
        s32          i;
        struct Moby *child;
        swarm_controller_pvars->initialized = 1;
        swarm_controller_pvars->child_count = 0;
        for (i = 0; i < swarm_controller_pvars->count; i++) {
            swarm_controller_pvars->children[i] = 0;
            child = create_moby(0x55);
            if (child != 0) {
                SwarmPartVars *part = (SwarmPartVars *)child->pvars;
                swarm_controller_pvars->children[i] = child;
                child->update_dist = 0x7f;
                child->draw_dist = 0x7f;
                child->scale *= random_float_between(0.7f, 1.3f);
                child->rot.z = random_angle_radians();
                child->anim_speed = random_float_between(0.6f, 1.2f);
                clear_u64_value(&part->velocity);
                part->unk44 = 0;
                part->unk4C = 10.0f;
                qcopy(&child->pos, &moby->pos);
                angle = random_angle_radians();
                angle2 = random_angle_radians();
                size = random_float_between(0.6f, 0.9f);
                child->pos.x += fast_cos(angle) * fast_sin(angle2) * size;
                child->pos.y += fast_sin(angle) * fast_sin(angle2) * size;
                child->pos.z += fast_cos(angle2) * size;
                part->unk50 = 0;
                part->leader_pos = &moby->pos;
                part->unk58 = 0;
                part->unk70 = 1.694f;
                part->unk74 = 1.694f;
                part->unk48 = 1.056f;
                part->unk5C = -1;
                part->unk40 = 0.003f;
                part->unk5E = 2;
                part->oclass = 0x55;
                part->unk90 = 1.0f;
                part->unk62 = 0x1ee;
                part->unk94 = 5.0f;
                update_entity(child);
            }
        }
    } else {
        s32          i;
        struct Moby *child;
        for (i = 0; i < swarm_controller_pvars->count; i++) {
            void *part;
            children = swarm_controller_pvars->children;
            child = children[i];
            part = child->pvars;
            D_L04_00166130[6] = ((swarm_controller_pvars->radius * 7.753f) / 1800.0f) *
                                frame_scale * ((child->anim_speed + 1.0f) * 0.5f) * 0.05f *
                                frame_scale_sq * ((child->anim_speed + 1.0f) * 0.5f);
            D_L04_00166130[7] =
                (((swarm_controller_pvars->radius * 7.753f) / 1800.0f) * frame_scale) *
                ((child->anim_speed + 1.0f) * 0.5f);
            steer_child_moby(children, swarm_controller_pvars->count, child, part, D_L04_00166130);
            update_moby_movement_with_sphere_collision(0.0f, child, part, D_L04_00166130);
        }
    }
}
