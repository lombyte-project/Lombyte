#include <math.h>
#include <string.h>
#include "types.h"
#include "rnc/overlay/shared/world/collision.h"

#define COLLISION_CAPACITY (1024u * 1024u)
#define CELL_BUDGET 65536u
#define ENTRY_BUDGET 262144u
#define FACE_BUDGET 2000000u

static struct {
    u8 bytes[COLLISION_CAPACITY];
    u8 guard[16];
} storage __attribute__((aligned(16)));
static u32 loaded_bytes;
static u32 mesh_start;
static u32 mesh_bytes;
static u32 cell_count;
static u32 face_count;
static u32 group_start;
static u32 group_count;
CollisionHit browser_collision_hit;
#define hit browser_collision_hit
static f32 hit_fraction;
static f32 hit_depth;
static s32 hit_material;
f32 collision_point[4] __asm__("D_00194120");

extern f32 probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");

/* The queries below model the static world only. The gated modules trap on any argument whose
   retail meaning is unverified (other masks, an ignored moby, options). The whole-tree spike
   defines RNC_SPIKE_COLLISION instead: such calls are answered from the static world alone and
   counted, and the runner reports the count on every run. */
#ifdef RNC_SPIKE_COLLISION
u32 spike_collision_unverified_calls;
u32 spike_collision_unverified(void) { return spike_collision_unverified_calls; }
#define UNSUPPORTED_ARGUMENTS(condition) do { if (condition) spike_collision_unverified_calls++; } while (0)
u32 spike_collision_bad_segments;
f32 spike_collision_first_bad_segment[6];
#else
#define UNSUPPORTED_ARGUMENTS(condition) do { if (condition) __builtin_trap(); } while (0)
#endif

_Static_assert(__builtin_offsetof(CollisionHit, point) == 0x20, "Collision point ABI");
_Static_assert(__builtin_offsetof(CollisionHit, normal_x) == 0x40, "Collision normal ABI");

static u32 half(u32 offset) {
    return storage.bytes[offset] | ((u32)storage.bytes[offset + 1] << 8);
}

static s32 signed_half(u32 offset) { return (s16)half(offset); }

static u32 word(u32 offset) {
    return half(offset) | (half(offset + 2) << 16);
}

static s32 valid_mesh_range(u32 offset, u32 length) {
    return offset <= mesh_bytes && length <= mesh_bytes - offset;
}

s32 runtime_collision_guards_valid(void) {
    for (u32 index = 0; index < 16; index++) if (storage.guard[index] != 0xA5) return 0;
    return 1;
}

static void clear_hit(void) {
    memset(&hit, 0, sizeof(hit));
    memset(collision_point, 0, sizeof(collision_point));
    hit_fraction = 1;
    hit_depth = 0;
    hit_material = -1;
}

void runtime_collision_reset(void) {
    memset(&storage, 0, sizeof(storage));
    memset(storage.guard, 0xA5, sizeof(storage.guard));
    loaded_bytes = cell_count = face_count = group_count = 0;
    clear_hit();
}

u32 runtime_collision_stage_address(void) { return (u32)storage.bytes; }
u32 runtime_collision_capacity(void) { return COLLISION_CAPACITY; }
s32 runtime_collision_ready(void) { return loaded_bytes != 0 && runtime_collision_guards_valid(); }
u32 runtime_collision_cells(void) { return loaded_bytes ? cell_count : 0; }
u32 runtime_collision_faces(void) { return loaded_bytes ? face_count : 0; }

s32 runtime_collision_load(u32 size) {
    loaded_bytes = cell_count = face_count = group_count = 0;
    clear_hit();
    if (!runtime_collision_guards_valid() || size < 8 || size > COLLISION_CAPACITY) return -1;
    mesh_start = word(0);
    u32 groups = word(4);
    if (mesh_start < 8 || mesh_start % 4 || mesh_start >= size ||
        (groups && (groups < mesh_start || groups > size))) return -2;
    mesh_bytes = (groups ? groups : size) - mesh_start;
    if (!valid_mesh_range(0, 4)) return -2;
    u32 layers = half(mesh_start + 2);
    if (layers > 512 || !valid_mesh_range(4, layers * 2)) return -2;
    u32 entries = layers;
    for (u32 layer = 0; layer < layers; layer++) {
        u32 row_table = half(mesh_start + 4 + layer * 2) * 4;
        if (!row_table) continue;
        if (!valid_mesh_range(row_table, 4)) return -3;
        u32 rows = half(mesh_start + row_table + 2);
        if (rows > 512 || !valid_mesh_range(row_table + 4, rows * 4)) return -3;
        if (rows > ENTRY_BUDGET - entries) goto invalid_cell;
        entries += rows;
        for (u32 row = 0; row < rows; row++) {
            u32 cell_table = word(mesh_start + row_table + 4 + row * 4);
            if (!cell_table) continue;
            if (cell_table % 4 || !valid_mesh_range(cell_table, 4)) return -4;
            u32 columns = half(mesh_start + cell_table + 2);
            if (columns > 512 || !valid_mesh_range(cell_table + 4, columns * 4)) return -4;
            if (columns > ENTRY_BUDGET - entries) goto invalid_cell;
            entries += columns;
            for (u32 column = 0; column < columns; column++) {
                u32 entry = word(mesh_start + cell_table + 4 + column * 4);
                u32 cell = entry >> 8;
                if (!cell) continue;
                u32 size_bytes = (entry & 255) * 16;
                if (++cell_count > CELL_BUDGET || cell % 16 || size_bytes < 4 ||
                    !valid_mesh_range(cell, size_bytes)) goto invalid_cell;
                u32 start = mesh_start + cell;
                u32 faces = half(start);
                u32 vertices = storage.bytes[start + 2];
                u32 quads = storage.bytes[start + 3];
                if (!vertices || quads > faces || 4 + vertices * 4 + faces * 4 + quads > size_bytes ||
                    faces > FACE_BUDGET - face_count) goto invalid_cell;
                face_count += faces;
                u32 indices = start + 4 + vertices * 4;
                for (u32 face = 0; face < faces; face++) {
                    for (u32 corner = 0; corner < 3; corner++) {
                        if (storage.bytes[indices + face * 4 + corner] >= vertices) goto invalid_cell;
                    }
                    if (face < quads && storage.bytes[indices + faces * 4 + face] >= vertices) goto invalid_cell;
                }
            }
        }
    }
    group_start = groups;
    if (groups) {
        if (size - groups < 16) goto invalid_cell;
        group_count = word(groups);
        if (group_count > 4096 || group_count * 16 > size - groups - 16) goto invalid_cell;
        u32 triangles_total = face_count;
        for (u32 group = 0; group < group_count; group++) {
            u32 record = groups + 16 + group * 16;
            u32 triangles = half(record + 8), vertices = half(record + 10);
            u32 offset = word(record + 12);
            u32 length = vertices * 8 + triangles * 4;
            if (!vertices || offset < 16 + group_count * 16 || offset > size - groups ||
                length > size - groups - offset || triangles > FACE_BUDGET - triangles_total) goto invalid_cell;
            triangles_total += triangles;
            for (u32 vertex_index = 0; vertex_index < vertices; vertex_index++) {
                if (half(groups + offset + vertex_index * 8 + 6)) goto invalid_cell;
            }
            u32 indices = groups + offset + vertices * 8;
            for (u32 face = 0; face < triangles; face++) {
                for (u32 corner = 0; corner < 3; corner++) {
                    if (storage.bytes[indices + face * 4 + corner] >= vertices) goto invalid_cell;
                }
                if (storage.bytes[indices + face * 4 + 3]) goto invalid_cell;
            }
        }
    }
    loaded_bytes = size;
    return 1;
invalid_cell:
    cell_count = face_count = group_count = 0;
    return -5;
}

static f32 dot(const f32 *first, const f32 *second) {
    return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
}

static void cross(f32 *output, const f32 *first, const f32 *second) {
    output[0] = first[1] * second[2] - first[2] * second[1];
    output[1] = first[2] * second[0] - first[0] * second[2];
    output[2] = first[0] * second[1] - first[1] * second[0];
}

static void vertex(f32 *output, u32 start, u32 index, const f32 *origin) {
    u32 packed = word(start + 4 + index * 4);
    output[0] = origin[0] + ((s32)(packed << 22) >> 22) / 16.0f;
    output[1] = origin[1] + ((s32)(packed << 12) >> 22) / 16.0f;
    output[2] = origin[2] + ((s32)packed >> 20) / 64.0f;
}

static void triangle(const f32 *start, const f32 *direction, const f32 points[3][3], s32 material) {
    f32 edge_first[3], edge_second[3], determinant_cross[3], relative[3], relative_cross[3];
    for (u32 axis = 0; axis < 3; axis++) {
        edge_first[axis] = points[1][axis] - points[0][axis];
        edge_second[axis] = points[2][axis] - points[0][axis];
        relative[axis] = start[axis] - points[0][axis];
    }
    cross(determinant_cross, direction, edge_second);
    f32 determinant = dot(edge_first, determinant_cross);
    if (fabsf(determinant) < 0.0000001f) return;
    f32 inverse = 1 / determinant;
    f32 first_weight = dot(relative, determinant_cross) * inverse;
    if (first_weight < -0.000001f || first_weight > 1.000001f) return;
    cross(relative_cross, relative, edge_first);
    f32 second_weight = dot(direction, relative_cross) * inverse;
    if (second_weight < -0.000001f || first_weight + second_weight > 1.000001f) return;
    f32 fraction = dot(edge_second, relative_cross) * inverse;
    if (fraction < 0 || fraction > hit_fraction) return;
    f32 normal[3];
    cross(normal, edge_first, edge_second);
    f32 length = sqrtf(dot(normal, normal));
    if (length < 0.0000001f) return;
    hit_fraction = fraction;
    hit_material = material;
    hit.unk1C = 1;
    for (u32 axis = 0; axis < 3; axis++) hit.point.f[axis] = start[axis] + direction[axis] * fraction;
    hit.normal_x = normal[0] / length;
    hit.normal_y = normal[1] / length;
    hit.normal_z = normal[2] / length;
}

static s32 intersects_cell(const f32 *start, const f32 *direction, const f32 *origin, f32 radius) {
    f32 near_fraction = 0, far_fraction = hit_fraction;
    for (u32 axis = 0; axis < 3; axis++) {
        f32 lower = origin[axis] - 2 - radius, upper = origin[axis] + 2 + radius;
        if (fabsf(direction[axis]) < 0.0000001f) {
            if (start[axis] < lower || start[axis] > upper) return 0;
        } else {
            f32 first = (lower - start[axis]) / direction[axis];
            f32 second = (upper - start[axis]) / direction[axis];
            near_fraction = fmaxf(near_fraction, fminf(first, second));
            far_fraction = fminf(far_fraction, fmaxf(first, second));
            if (near_fraction > far_fraction) return 0;
        }
    }
    return 1;
}

static void subtract(f32 *output, const f32 *first, const f32 *second) {
    for (u32 axis = 0; axis < 3; axis++) output[axis] = first[axis] - second[axis];
}

static void closest_triangle_point(f32 *output, const f32 *point, const f32 points[3][3]) {
    f32 first_edge[3], second_edge[3], relative[3];
    subtract(first_edge, points[1], points[0]);
    subtract(second_edge, points[2], points[0]);
    subtract(relative, point, points[0]);
    f32 first_dot = dot(first_edge, relative), second_dot = dot(second_edge, relative);
    f32 first_weight = 0, second_weight = 0;
    if (first_dot <= 0 && second_dot <= 0) goto result;
    subtract(relative, point, points[1]);
    f32 third_dot = dot(first_edge, relative), fourth_dot = dot(second_edge, relative);
    if (third_dot >= 0 && fourth_dot <= third_dot) { first_weight = 1; goto result; }
    f32 first_area = first_dot * fourth_dot - third_dot * second_dot;
    if (first_area <= 0 && first_dot >= 0 && third_dot <= 0) {
        first_weight = first_dot / (first_dot - third_dot);
        goto result;
    }
    subtract(relative, point, points[2]);
    f32 fifth_dot = dot(first_edge, relative), sixth_dot = dot(second_edge, relative);
    if (sixth_dot >= 0 && fifth_dot <= sixth_dot) { second_weight = 1; goto result; }
    f32 second_area = fifth_dot * second_dot - first_dot * sixth_dot;
    if (second_area <= 0 && second_dot >= 0 && sixth_dot <= 0) {
        second_weight = second_dot / (second_dot - sixth_dot);
        goto result;
    }
    f32 third_area = third_dot * sixth_dot - fifth_dot * fourth_dot;
    if (third_area <= 0 && fourth_dot - third_dot >= 0 && fifth_dot - sixth_dot >= 0) {
        second_weight = (fourth_dot - third_dot) / (fourth_dot - third_dot + fifth_dot - sixth_dot);
        first_weight = 1 - second_weight;
        goto result;
    }
    first_weight = second_area / (first_area + second_area + third_area);
    second_weight = first_area / (first_area + second_area + third_area);
result:
    for (u32 axis = 0; axis < 3; axis++) {
        output[axis] = points[0][axis] + first_edge[axis] * first_weight + second_edge[axis] * second_weight;
    }
}

static f32 clamp_unit(f32 value) { return fminf(1, fmaxf(0, value)); }

static void closest_segments(f32 *axis_point, f32 *edge_point, const f32 *start,
                             const f32 *direction, const f32 *edge_start, const f32 *edge_end) {
    f32 edge[3], relative[3];
    subtract(edge, edge_end, edge_start);
    subtract(relative, start, edge_start);
    f32 axis_length = dot(direction, direction), edge_length = dot(edge, edge);
    f32 projection = dot(edge, relative), axis_fraction = 0, edge_fraction = 0;
    if (axis_length <= 0.00000001f) {
        if (edge_length > 0.00000001f) edge_fraction = clamp_unit(projection / edge_length);
    } else {
        f32 offset = dot(direction, relative);
        if (edge_length <= 0.00000001f) axis_fraction = clamp_unit(-offset / axis_length);
        else {
            f32 alignment = dot(direction, edge);
            f32 denominator = axis_length * edge_length - alignment * alignment;
            if (denominator > 0.00000001f) {
                axis_fraction = clamp_unit((alignment * projection - offset * edge_length) / denominator);
            }
            edge_fraction = (alignment * axis_fraction + projection) / edge_length;
            if (edge_fraction < 0) {
                edge_fraction = 0;
                axis_fraction = clamp_unit(-offset / axis_length);
            } else if (edge_fraction > 1) {
                edge_fraction = 1;
                axis_fraction = clamp_unit((alignment - offset) / axis_length);
            }
        }
    }
    for (u32 axis = 0; axis < 3; axis++) {
        axis_point[axis] = start[axis] + direction[axis] * axis_fraction;
        edge_point[axis] = edge_start[axis] + edge[axis] * edge_fraction;
    }
}

static void capsule_triangle(const f32 *start, const f32 *direction, const f32 points[3][3],
                              s32 material, f32 radius) {
    f32 first_edge[3], second_edge[3], normal[3];
    subtract(first_edge, points[1], points[0]);
    subtract(second_edge, points[2], points[0]);
    cross(normal, first_edge, second_edge);
    f32 normal_length = sqrtf(dot(normal, normal));
    if (normal_length < 0.0000001f) return;
    f32 best_distance = INFINITY, best_axis[3], best_surface[3];
    for (u32 candidate = 0; candidate < 6; candidate++) {
        f32 axis_point[3], surface_point[3], relative[3];
        if (candidate < 2) {
            for (u32 axis = 0; axis < 3; axis++) axis_point[axis] = start[axis] + candidate * direction[axis];
            closest_triangle_point(surface_point, axis_point, points);
        } else if (candidate < 5) {
            u32 edge = candidate - 2;
            closest_segments(axis_point, surface_point, start, direction, points[edge], points[(edge + 1) % 3]);
        } else {
            f32 denominator = dot(normal, direction);
            if (fabsf(denominator) < 0.0000001f) continue;
            subtract(relative, points[0], start);
            f32 fraction = dot(normal, relative) / denominator;
            if (fraction < 0 || fraction > 1) continue;
            for (u32 axis = 0; axis < 3; axis++) axis_point[axis] = start[axis] + fraction * direction[axis];
            closest_triangle_point(surface_point, axis_point, points);
        }
        subtract(relative, axis_point, surface_point);
        f32 distance = dot(relative, relative);
        if (distance < best_distance) {
            best_distance = distance;
            memcpy(best_axis, axis_point, sizeof(best_axis));
            memcpy(best_surface, surface_point, sizeof(best_surface));
        }
    }
    f32 distance = sqrtf(best_distance), depth = radius - distance;
    if (depth <= hit_depth) return;
    if (distance > 0.000001f) {
        for (u32 axis = 0; axis < 3; axis++) normal[axis] = (best_axis[axis] - best_surface[axis]) / distance;
    } else {
        f32 relative[3];
        for (u32 axis = 0; axis < 3; axis++) relative[axis] = start[axis] + direction[axis] * 0.5f - points[0][axis];
        f32 sign = dot(relative, normal) < 0 ? -1 : 1;
        for (u32 axis = 0; axis < 3; axis++) normal[axis] *= sign / normal_length;
    }
    hit_depth = depth;
    hit_material = material;
    hit.unk1C = 1;
    f32 corrected[4] = { 0, 0, 0, 0 };
    for (u32 axis = 0; axis < 3; axis++) {
        hit.point.f[axis] = best_surface[axis];
        corrected[axis] = start[axis] + normal[axis] * (depth + 0.00001f);
    }
    memcpy(hit.pad30, corrected, sizeof(corrected));
    hit.normal_x = normal[0];
    hit.normal_y = normal[1];
    hit.normal_z = normal[2];
}

static s32 query_mesh(const f32 *start, const f32 *direction, f32 radius) {
    u32 layers = half(mesh_start + 2);
    for (u32 layer = 0; layer < layers; layer++) {
        u32 row_table = half(mesh_start + 4 + layer * 2) * 4;
        if (!row_table) continue;
        u32 rows = half(mesh_start + row_table + 2);
        for (u32 row = 0; row < rows; row++) {
            u32 cell_table = word(mesh_start + row_table + 4 + row * 4);
            if (!cell_table) continue;
            u32 columns = half(mesh_start + cell_table + 2);
            for (u32 column = 0; column < columns; column++) {
                u32 cell = word(mesh_start + cell_table + 4 + column * 4) >> 8;
                if (!cell) continue;
                f32 origin[3] = { (signed_half(mesh_start + cell_table) + (s32)column) * 4 + 2,
                                  (signed_half(mesh_start + row_table) + (s32)row) * 4 + 2,
                                  (signed_half(mesh_start) + (s32)layer) * 4 + 2 };
                if (!intersects_cell(start, direction, origin, radius)) continue;
                u32 record = mesh_start + cell;
                u32 faces = half(record), vertices = storage.bytes[record + 2];
                u32 quads = storage.bytes[record + 3];
                u32 indices = record + 4 + vertices * 4;
                for (u32 face = 0; face < faces; face++) {
                    u32 entry = indices + face * 4;
                    f32 points[3][3];
                    for (u32 corner = 0; corner < 3; corner++) {
                        vertex(points[corner], record, storage.bytes[entry + 2 - corner], origin);
                    }
                    if (radius) capsule_triangle(start, direction, points, storage.bytes[entry + 3], radius);
                    else triangle(start, direction, points, storage.bytes[entry + 3]);
                    if (face < quads) {
                        vertex(points[0], record, storage.bytes[entry], origin);
                        vertex(points[1], record, storage.bytes[indices + faces * 4 + face], origin);
                        vertex(points[2], record, storage.bytes[entry + 2], origin);
                        if (radius) capsule_triangle(start, direction, points, storage.bytes[entry + 3], radius);
                        else triangle(start, direction, points, storage.bytes[entry + 3]);
                    }
                }
            }
        }
    }
    return hit.unk1C;
}

s32 runtime_collision_segment(f32 start_x, f32 start_y, f32 start_z,
                              f32 end_x, f32 end_y, f32 end_z) {
    clear_hit();
    if (!loaded_bytes || !runtime_collision_guards_valid()) return -1;
    f32 start[3] = { start_x, start_y, start_z };
    f32 end[3] = { end_x, end_y, end_z };
    f32 direction[3];
    for (u32 axis = 0; axis < 3; axis++) {
        if (!isfinite(start[axis]) || !isfinite(end[axis]) ||
            fabsf(start[axis]) > 1048576 || fabsf(end[axis]) > 1048576) return -2;
        direction[axis] = end[axis] - start[axis];
    }
    if (dot(direction, direction) == 0) return 0;
    return query_mesh(start, direction, 0);
}

s32 runtime_collision_capsule(f32 pos_x, f32 pos_y, f32 pos_z, f32 height, f32 radius) {
    clear_hit();
    if (!loaded_bytes || !runtime_collision_guards_valid()) return -1;
    f32 start[3] = { pos_x, pos_y, pos_z }, direction[3] = { 0, 0, height };
    if (!isfinite(height) || !isfinite(radius) || height < 0 || height > 16 ||
        radius <= 0 || radius > 16 || fabsf(pos_z + height) > 1048576) return -2;
    for (u32 axis = 0; axis < 3; axis++) {
        if (!isfinite(start[axis]) || fabsf(start[axis]) > 1048576) return -2;
    }
    return query_mesh(start, direction, radius);
}

static s32 query_hero_groups(const f32 *position, f32 radius) {
    f32 previous_depth = hit_depth;
    const f32 direction[3] = { 0, 0, 0 };
    for (u32 group = 0; group < group_count; group++) {
        u32 record = group_start + 16 + group * 16;
        f32 relative[3];
        for (u32 axis = 0; axis < 3; axis++) relative[axis] = position[axis] - half(record + axis * 2) / 64.0f;
        f32 reach = radius + half(record + 6) / 64.0f;
        if (dot(relative, relative) > reach * reach) continue;
        u32 triangles = half(record + 8), vertices = half(record + 10);
        u32 data = group_start + word(record + 12);
        u32 indices = data + vertices * 8;
        for (u32 face = 0; face < triangles; face++) {
            f32 points[3][3];
            for (u32 corner = 0; corner < 3; corner++) {
                u32 entry = data + storage.bytes[indices + face * 4 + 2 - corner] * 8;
                for (u32 axis = 0; axis < 3; axis++) points[corner][axis] = half(entry + axis * 2) / 64.0f;
            }
            capsule_triangle(position, direction, points, 256, radius);
        }
    }
    return hit_depth > previous_depth;
}

s32 collision_sphere(f32 *position, s32 flags, s32 ignored_moby, f32 radius) __asm__("FUN_L00_001f0d60");
s32 collision_rig(f32 *position, s32 flags, f32 radius, s32 ignored_moby, f32 height) __asm__("FUN_L00_001f19a0");
s32 collision_hero_groups(f32 *position, f32 radius) __asm__("FUN_L00_001f3170");

s32 collision_sphere(f32 *position, s32 flags, s32 ignored_moby, f32 radius) {
    if (!position) __builtin_trap();
    UNSUPPORTED_ARGUMENTS(flags != 0x24 || ignored_moby);
    s32 result = runtime_collision_capsule(position[0], position[1], position[2], 0, radius);
    if (result < 0) __builtin_trap();
    return result;
}

s32 collision_rig(f32 *position, s32 flags, f32 radius, s32 ignored_moby, f32 height) {
    if (!position) __builtin_trap();
    UNSUPPORTED_ARGUMENTS(flags != 0x24 || ignored_moby);
    s32 result = runtime_collision_capsule(position[0], position[1], position[2], height, radius);
    if (result < 0) __builtin_trap();
    return result;
}

s32 collision_hero_groups(f32 *position, f32 radius) {
    if (!loaded_bytes || !runtime_collision_guards_valid() || !position ||
        !isfinite(radius) || radius <= 0 || radius > 16) __builtin_trap();
    for (u32 axis = 0; axis < 3; axis++) {
        if (!isfinite(position[axis]) || fabsf(position[axis]) > 1048576) __builtin_trap();
    }
    return query_hero_groups(position, radius);
}

f32 runtime_collision_hit_position(s32 axis) {
    if (axis < 0 || axis > 2) return NAN;
    f32 position;
    memcpy(&position, hit.pad30 + axis * sizeof(f32), sizeof(position));
    return position;
}

f32 runtime_collision_hit_fraction(void) { return hit_fraction; }
s32 runtime_collision_hit_material(void) { return hit_material; }
f32 runtime_collision_hit_point(s32 axis) { return axis >= 0 && axis < 3 ? hit.point.f[axis] : NAN; }
f32 runtime_collision_hit_normal(s32 axis) {
    return axis == 0 ? hit.normal_x : axis == 1 ? hit.normal_y : axis == 2 ? hit.normal_z : NAN;
}

/* The two accessors the game uses to classify the last hit. On the console both read the word at
   hit record +0x1C (boot 0x1f0b58 / level 0x1f3608: lw 0x1c; bltz -> none; mask; all-ones -> none):
   bits 0-4 are the material, bits 5-6 the sound class. This adapter keeps +0x1C as its 0/1 hit
   flag, so they read the face's stored type byte instead. That the byte equals the retail word is
   an assumption drawn from the masks, not a compared value. */
s32 collision_hit_material(void) __asm__("FUN_001f0b58");
s32 collision_hit_sound_class(void) __asm__("FUN_L00_001f3608");

s32 collision_hit_material(void) {
    s32 type = hit_material;
    if (type < 0 || (type & 0x1F) == 0x1F) return -1;
    return type & 0x1F;
}

s32 collision_hit_sound_class(void) {
    s32 type = hit_material;
    if (type < 0 || (type & 0x60) == 0x60) return 0;
    return (type & 0x60) >> 5;
}

s32 collision_line(f32 *start, f32 *end, s32 flags, s32 ignored_moby, s32 options) __asm__("FUN_001efa68");

s32 collision_line(f32 *start, f32 *end, s32 flags, s32 ignored_moby, s32 options) {
    if (!start || !end) __builtin_trap();
    UNSUPPORTED_ARGUMENTS(flags != 2 || ignored_moby || options);
    s32 result = runtime_collision_segment(start[0], start[1], start[2], end[0], end[1], end[2]);
#ifdef RNC_SPIKE_COLLISION
    /* A segment with a non-finite or far-out end (-2). The EE has no NaN or infinity, so the
       retail query sees ordinary (huge) numbers and finds no face; here it answers "no hit" and
       keeps the first such segment for diagnosis (first seen: a wrench swing's hit sweep after a
       weapon switch, find_moby_hit 0x259d08). */
    if (result == -2) {
        if (spike_collision_bad_segments++ == 0)
            for (u32 i = 0; i < 3; i++) { spike_collision_first_bad_segment[i] = start[i]; spike_collision_first_bad_segment[3 + i] = end[i]; }
        result = 0;
    }
#endif
    if (result < 0) __builtin_trap();
    memcpy(collision_point, hit.point.f, sizeof(collision_point));
    return result;
}
#ifdef RNC_SPIKE_COLLISION
/* Level 0's copy of coll_line_fix (0x1efc70; config/us/levels/functions.tsv places FUN_001efa68
   there). Its own data references are the level's load descriptor; the query here reads the
   staged static world either way. */
s32 collision_line_level0(f32 *start, f32 *end, s32 flags, s32 ignored_moby, s32 options)
    __asm__("FUN_L00_001efc70") __attribute__((alias("FUN_001efa68")));
#endif

f32 runtime_collision_ground_height(f32 pos_x, f32 pos_y, f32 pos_z, f32 height_offset) {
    f32 position[4] = { pos_x, pos_y, pos_z, 0 };
    if (!loaded_bytes || !runtime_collision_guards_valid() || !isfinite(height_offset) ||
        !isfinite(pos_z + height_offset) || fabsf(pos_z + height_offset) > 1048576) return NAN;
    for (u32 axis = 0; axis < 3; axis++) {
        if (!isfinite(position[axis]) || fabsf(position[axis]) > 1048576) return NAN;
    }
    return probe_ground_height(position, 0, height_offset);
}