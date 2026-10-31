/* Particles of types 10, 14, 52 and 74, number drawing, and spline path helpers. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"

/* Spawns a type-10 particle at a position and sets its colour, size, life and velocity. */
/* Ported from rac1-decomp (src/overlays/shared/partupd_00280428.c: func_L01_00280428), where it is exact; names translated to the US level program. */

extern int FUN_L00_0025bfe0(float, float, float, float);

extern unsigned char *D_L01_001B250C NOT_SDA;
void                                 spawn_type_10_particle(char *spawn_position,
                                                            char *initial_particle_velocity) __asm__("FUN_L01_0027f660");

void spawn_type_10_particle(char *spawn_position, char *initial_particle_velocity) {
    unsigned char *particle = create_part(10);
    if (particle != 0) {
        char *particle_data = (char *)particle + 0x20;
        qcopy(particle + 0x10, spawn_position);
        *(int *)(particle + 4) = FUN_L00_0025bfe0(0.425f, 0.425f, 0.225f, 1.0f);
        particle[9] = truncate_float_to_s32(8.0f) - 0x60;
        particle[1] = 0;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = random_float_between(60900.0f, 90300.0f);
        particle[8] = 0;
        particle[2] = *D_L01_001B250C;
        *(float *)(particle_data + 0x10) = 0.425f;
        *(float *)(particle_data + 0x14) = 0.425f;
        *(float *)(particle_data + 0x18) = 0.225f;
        *(float *)(particle_data + 0x1C) = 1.0f;
        *(short *)(particle + 0xA) = scale_game_frames(0x57);
        qcopy(particle_data, initial_particle_velocity);
    }
}

extern unsigned char *D_L01_001B2538 NOT_SDA;
char *spawn_type_14_particle(int particle_value, char *position_source) __asm__("FUN_L01_00280970");

char *spawn_type_14_particle(int particle_value, char *position_source) {
    char *particle = create_part(0xE);
    int   particle_frame_offset;
    int  *particle_data;
    if (particle) {
        qcopy(particle + 0x10, position_source + 0x10);
        *(float *)(particle + 0x10) += randf_sym(0.0f, 0.03f);
        *(float *)(particle + 0x14) += randf_sym(0.0f, 0.03f);
        *(float *)(particle + 0x18) += randf_sym(0.0f, 0.03f);
        *(int *)(particle + 4) = -1;
        particle_frame_offset = truncate_float_to_s32(4.0f);
        particle[9] = particle_frame_offset - 0x60;
        particle[1] = 0;
        particle[3] = 0x48;
        *(float *)(particle + 0xC) = 29400.0f;
        particle[8] = random_integer_below(0xFF);
        particle[2] = *D_L01_001B2538;
        *(short *)(particle + 0xA) = scale_game_frames(6);
        particle_data = (int *)(particle + 0x20);
        particle_data[2] = (int)position_source;
        particle_data[1] = 1;
        if (particle_value == 0) {
            particle_data[0] = 0;
        } else if (random_integer_below(2)) {
            particle_data[0] = particle_value;
        } else {
            particle_data[0] = -particle_value;
        }
    }
    return particle;
}

extern char *number_particle_lookup_tables[] __asm__("D_L01_001B25A4");

/* Allocates a type 0x29 particle and fills it from the arguments; kind 0xFF
 * derives the frame from flags and picks one of two blend modes. */
char *draw_number(float         scale,
                  void         *pos,
                  int           a,
                  int           idx,
                  int           b,
                  unsigned char flags,
                  short         h,
                  unsigned char kind) __asm__("FUN_L01_00285768");
char *draw_number(float         scale,
                  void         *particle_spawn_position,
                  int           source_moby,
                  int           number_table_index,
                  int           particle_color,
                  unsigned char flags,
                  short         particle_lifetime,

                  unsigned char kind) {
    char *particle = create_part(0x29);
    if (particle != 0) {
        int *tail = (int *)(particle + 0x20);
        qcopy(particle + 0x10, particle_spawn_position);
        *(int *)(particle + 4) = particle_color;
        if (kind == 0xFF) {
            particle[9] = (flags >> 5) * 16 + truncate_float_to_s32(4.0f);
            if (flags & 1) {
                particle[3] = 0x48;
            } else {
                particle[3] = 0x44;
            }
        } else {
            particle[9] = flags;
            particle[3] = kind;
        }
        particle[1] = 0;
        particle[2] = number_particle_lookup_tables[0][number_table_index];
        *(float *)(particle + 0xC) = scale * 210000.0f;
        *(short *)(particle + 0xA) = particle_lifetime;
        ((unsigned char *)particle)[8] = 0xA0;
        /* taking the address changes register allocation to match */
        *tail = *&source_moby;
    }
    return particle;
}

#ifndef NOT_SDA
#endif
#ifndef MACRO_ADDR
#endif
extern unsigned char *D_L01_001B25D0 NOT_SDA;
unsigned char                       *spawn_type_52_particle(void *spawn_position,
                                                            int   b,
                                                            int   particle_data_value,
                                                            int   particle_lifetime,
                                                            float particle_scale,
                                                            float g) __asm__("FUN_L01_00287158");

unsigned char *spawn_type_52_particle(void *spawn_position,
                                      int   b,
                                      int   particle_data_value,
                                      int   particle_lifetime,
                                      float particle_scale,

                                      float g) {
    unsigned char *r = create_part(0x34);
    if (r != 0) {
        char *particle_data = (char *)r + 0x20;
        qcopy(r + 0x10, spawn_position);
        *(float *)(r + 0x18) += 0.05f;
        *(int *)(r + 4) = b;
        r[3] = 0x44;
        r[9] = truncate_float_to_s32(4.0f) + 0x20;
        r[1] = 1;
        r[2] = *D_L01_001B25D0;
        *(short *)(r + 0xA) = particle_lifetime;
        *(float *)(r + 0xC) = particle_scale * 210000.0f;
        r[8] = random_integer_below(0xFF);
        *(int *)(particle_data + 8) = b;
        *(int *)(particle_data + 0xC) = particle_data_value;
        *(float *)particle_data = particle_scale;
        *(float *)(particle_data + 4) = g;
        *(float *)(particle_data + 0x10) = 1.0f / convert_integer_to_float(particle_lifetime);
    }
    return r;
}

#include "qzero.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_path.h"

/* spawns a type-74 particle with transformed offset and velocity */
/* Ported from rac1-decomp (src/overlays/shared/partupd_00280428.c: func_L01_0028B570), where it is exact; names translated to the US level program. */

extern int           D_L01_001B2628; /* no foreign declaration */
extern s32           rand(void);
extern unsigned char D_L01_001B2928_b[] __asm__("D_L01_001B2628");

unsigned char *spawn_type_74_particle(char  *source_moby,
                                      char  *particle_spawn_position,
                                      float *initial_particle_velocity,
                                      char  *cfg,
                                      int    flag) __asm__("FUN_L01_0028a7a8");

unsigned char *spawn_type_74_particle(char  *source_moby,
                                      char  *particle_spawn_position,
                                      float *initial_particle_velocity,
                                      char  *cfg,

                                      int flag) {
    float          transformed_spawn_offset[4];
    float          spawn_offset_transform[16];
    unsigned char *particle = create_part(74);
    if (particle) {
        char *particle_data;
        particle[9] = truncate_float_to_s32(4.0f) - 0x60;
        if (flag)
            particle[3] = 0x48;
        else
            particle[3] = 0x44;
        particle[1] = 0;
        particle_data = (char *)particle + 0x20;
        particle[2] = **(unsigned char **)D_L01_001B2928_b;
        particle[8] = rand();
        *(int *)(particle + 0xc) = 0;
        qcopy(particle + 0x10, particle_spawn_position);
        *(float *)particle_data = initial_particle_velocity[0];
        *(float *)(particle_data + 4) = initial_particle_velocity[1];
        *(float *)(particle_data + 8) = initial_particle_velocity[2];
        fast_vec_sub(transformed_spawn_offset, particle_spawn_position, source_moby + 0x10);
        qzero(spawn_offset_transform);
        qzero(spawn_offset_transform + 4);
        qzero(spawn_offset_transform + 8);
        qzero(spawn_offset_transform + 12);
        transpose_rotation_matrix(spawn_offset_transform, source_moby + 0xc0);
        transform_vector(
            transformed_spawn_offset, transformed_spawn_offset, spawn_offset_transform);
        *(float *)(particle_data + 0x10) = transformed_spawn_offset[0];
        *(float *)(particle_data + 0x14) = transformed_spawn_offset[1];
        *(float *)(particle_data + 0x18) = transformed_spawn_offset[2];
        *(char **)(particle_data + 0xc) = source_moby;
        *(char **)(particle_data + 0x1c) = cfg;
        *(short *)(particle + 0xa) = *(unsigned short *)(cfg + 0xc);
        *(int *)(particle + 4) = *(int *)cfg;
    }
    return particle;
}
/* Ported from rac1-decomp (src/overlays/shared/partupd_00280428.c: func_L01_0028C1D8), where it is exact; names translated to the US level program. */

void initialize_path_descriptor(char *path_descriptor) __asm__("FUN_L01_0028b410");

void initialize_path_descriptor(char *path_descriptor) {
    int   i;
    int   path_node_byte_offset;
    char *path_data;
    path_descriptor[5] = 0;
    *(int *)(path_descriptor + 8) = 0;
    *(char **)(path_descriptor + 0x10) = ((char *)l01_paths[*(int *)(path_descriptor + 0x14)]);
    for (i = 0; i < **(int **)(path_descriptor + 0x10); i++) {
        path_data = *(char **)(path_descriptor + 0x10);
        path_node_byte_offset = i << 4;
        *(float *)(*(char **)(path_descriptor + 0x10) + path_node_byte_offset + 0x1C) =
            fast_vec_dist(path_data + (path_node_byte_offset + 0x10),
                          path_data + ((((i + 1) % *(int *)path_data) << 4) + 0x10));
        *(float *)(*(char **)(path_descriptor + 0x10) + path_node_byte_offset + 0x18) += 0.5f;
        *(float *)(*(char **)(path_descriptor + 0x10) + path_node_byte_offset + 0x18) -= 0.5f;
    }
}
/* Returns the index of the entry whose distance to arg0 is closest to a target value. */
/* Ported from rac1-decomp (src/overlays/shared/partupd_00280428.c: func_L01_0028C2D8), where it is exact; names translated to the US level program. */

typedef struct {
    int   count;
    int   pad[3];
    float e[1][4];
} List;

int select_point_index_by_distance(void *reference_point,
                                   List *point_list,
                                   float target) __asm__("FUN_L01_0028b510");

int select_point_index_by_distance(void *reference_point, List *point_list, float target) {
    int   best = 0;
    int   i;
    float bestDiff = 1e11f;
    for (i = 0; i < point_list->count; i++) {
        float point_distance = fast_vec_dist(point_list->e[i], reference_point);
        if (absolute_float(point_distance - target) < bestDiff) {
            bestDiff = point_distance;
            best = i;
        }
    }
    return best;
}

int select_list_point_toward_target(char *reference_object,
                                    List *list,
                                    char *pt,
                                    char *out) __asm__("FUN_L01_0028b5e0");

int select_list_point_toward_target(char *reference_object,
                                    List *list,
                                    char *pt,

                                    char *selected_point) {
    int   nearest_reference_entry_index = -1;
    int   nearest_eligible_entry_index = -1;
    float best0 = 10000.0f;
    float best1 = 10000.0f;
    float d_init = fast_vec_dist(reference_object + 0x10, pt);
    int   i;

    for (i = 0; i < list->count; i++) {
        float d_pt = fast_vec_dist(list->e[i], pt);
        float d_a0 = fast_vec_dist(list->e[i], reference_object + 0x10);
        if (d_a0 < best0) {
            best0 = d_a0;
            nearest_reference_entry_index = i;
        }
        if (d_pt < d_init) {
            if (d_pt < best1) {
                best1 = d_pt;
                nearest_eligible_entry_index = i;
            }
        }
    }
    if (nearest_eligible_entry_index != -1) {
        if (nearest_reference_entry_index == nearest_eligible_entry_index) {
            qcopy(selected_point, list->e[nearest_reference_entry_index]);
            return nearest_reference_entry_index;
        }
        if (nearest_reference_entry_index < nearest_eligible_entry_index) {
            qcopy(selected_point, list->e[nearest_reference_entry_index + 1]);
            return nearest_reference_entry_index + 1;
        }
        qcopy(selected_point, list->e[nearest_reference_entry_index - 1]);
        return nearest_reference_entry_index - 1;
    }
    qcopy(selected_point, pt);
    return -1;
}

/* Select a 16-byte entry using the descriptor's signed index rules. */
void *get_path_node(void *descriptor, s32 index) __asm__("FUN_L01_0028b7b0");

void *get_path_node(void *descriptor, s32 index) {
    char *path_storage_base;
    s32   last;

    if (*(s8 *)((char *)descriptor + 4) >= 0) {
        last = (s32)((u32) * *(s32 **)((char *)descriptor + 0x10) - 1);
        path_storage_base = *(char **)((char *)descriptor + 0x10);
    } else {
        path_storage_base = *(char **)((char *)descriptor + 0x10);
        last = 0;
    }
    if (index < 0)
        return path_storage_base + ((((u32)last + (u32)index) << 4) + 0x20);
    if (index > last)
        return path_storage_base + (((u32)(index % (s32)((u32)last + 1)) << 4) + 0x10);
    return path_storage_base + (((u32)index << 4) + 0x10);
}

typedef struct {
    char        pad0[4];
    signed char f4;
    char        pad1[0x10 - 5];
    int        *f10;
} T_28b828;

int resolve_path_node_index(T_28b828 *path, int node_index) __asm__("FUN_L01_0028b828");

int resolve_path_node_index(T_28b828 *path, int node_index) {
    int last_index;
    last_index = (path->f4 < 0) ? 0 : (*path->f10 - 1);
    if (node_index < 0) {
        return last_index + node_index;
    }
    if (last_index < node_index) {
        return node_index % (last_index + 1);
    }
    return node_index;
}

/* 0x0028b878, 76 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
int advance_path_node_index(char *path_descriptor) __asm__("FUN_L01_0028b878");

int advance_path_node_index(char *path_descriptor) {
    int max = 0;
    int next_node_index;
    if (path_descriptor[4] >= 0) {
        max = **(int **)(path_descriptor + 0x10) - 1;
    }
    next_node_index = *(int *)path_descriptor + 1;
    if (max < next_node_index) {
        next_node_index %= max + 1;
    }
    *(int *)path_descriptor = next_node_index;
    return next_node_index;
}

/* Ported from rac1-decomp (src/overlays/shared/partupd_00280428.c: func_L01_0028C690), where it is exact; names translated to the US level program. */

extern void *get_path_node(void *, int) __asm__("FUN_L01_0028b7b0");
extern void *memset(void *, int, unsigned int);
extern void *func_001153FC_c(void *, int, unsigned int) __asm__("FUN_001153fc");

void compute_path_node_tangent(void *out,
                               int  *path_descriptor,
                               int   node_index_offset,
                               float tangent_tension,
                               float tangent_bias) __asm__("FUN_L01_0028b8c8");

void compute_path_node_tangent(void *node_tangent,
                               int  *path_descriptor,
                               int   node_index_offset,
                               float tangent_tension,

                               float tangent_bias) {
    void *neighboring_points[3];
    float weighted_incoming_delta[4];
    float weighted_outgoing_delta[4];
    float blended_delta[4];
    float incoming_segment_distance, outgoing_segment_distance, delta_length_limit,
        blended_delta_length;
    float wa, wb;
    func_001153FC_c(neighboring_points, 0, 12);
    neighboring_points[0] =
        get_path_node(path_descriptor, *path_descriptor + node_index_offset - 1);
    neighboring_points[1] = get_path_node(path_descriptor, *path_descriptor + node_index_offset);
    neighboring_points[2] =
        get_path_node(path_descriptor, *path_descriptor + node_index_offset + 1);
    {
        wb = (1.0f - tangent_tension) * (tangent_bias + 1.0f) * 0.5f;
        wa = (1.0f - tangent_tension) * (1.0f - tangent_bias) * 0.5f;
    }
    incoming_segment_distance = fast_vec_dist(neighboring_points[0], neighboring_points[1]);
    outgoing_segment_distance = fast_vec_dist(neighboring_points[1], neighboring_points[2]);
    if (incoming_segment_distance < outgoing_segment_distance)
        delta_length_limit = incoming_segment_distance;
    else
        delta_length_limit = outgoing_segment_distance;
    fast_vec_sub(weighted_incoming_delta, neighboring_points[1], neighboring_points[0]);
    fast_vec_scale(weighted_incoming_delta, weighted_incoming_delta, wb);
    fast_vec_sub(weighted_outgoing_delta, neighboring_points[2], neighboring_points[1]);
    fast_vec_scale(weighted_outgoing_delta, weighted_outgoing_delta, wa);
    fast_vec_add(blended_delta, weighted_incoming_delta, weighted_outgoing_delta);
    delta_length_limit = delta_length_limit * 1.1f;
    blended_delta_length = fast_vec_length(blended_delta);
    if (delta_length_limit < blended_delta_length) {
        fast_vec_normalize(node_tangent, blended_delta, delta_length_limit);
    } else {
        *(OvlQuad *)node_tangent = *(OvlQuad *)blended_delta;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/partupd_00280428.c: func_L01_0028C848), where it is exact; names translated to the US level program. */

void evaluate_cubic_hermite_segment(float segment_parameter,
                                    void *out,
                                    void *start_point,
                                    void *end_point,
                                    void *start_tangent,
                                    void *end_tangent) __asm__("FUN_L01_0028ba80");

void evaluate_cubic_hermite_segment(float segment_parameter,
                                    void *evaluated_point,
                                    void *start_point,
                                    void *end_point,
                                    void *start_tangent,

                                    void *end_tangent) {
    char  weighted_vector[16];
    float t_squared = segment_parameter * segment_parameter;
    float t_cubed = t_squared * segment_parameter;
    float three_times_t_squared = t_squared * 3.0f;
    float twice_t_cubed = t_cubed + t_cubed;
    float twice_t_squared = t_squared + t_squared;
    fast_vec_scale(evaluated_point, start_point, twice_t_cubed - three_times_t_squared + 1.0f);
    fast_vec_scale(weighted_vector, start_tangent, t_cubed - twice_t_squared + segment_parameter);
    fast_vec_add(evaluated_point, evaluated_point, weighted_vector);
    fast_vec_scale(weighted_vector, end_tangent, t_cubed - t_squared);
    fast_vec_add(evaluated_point, evaluated_point, weighted_vector);
    fast_vec_scale(weighted_vector, end_point, three_times_t_squared - twice_t_cubed);
    fast_vec_add(evaluated_point, evaluated_point, weighted_vector);
}

float compute_sampled_spline_path_lengths(float     tangent_tension,
                                          float     tangent_bias,
                                          T_28b828 *path,
                                          int       particles,
                                          void     *moby) __asm__("FUN_L01_0028bb90");

float compute_sampled_spline_path_lengths(

    float tangent_tension, float tangent_bias, T_28b828 *path, int particles, void *moby) {
    OvlVec4 previousTangent;
    OvlVec4 nextTangent;
    OvlVec4 currentNode;
    OvlVec4 nextNode;
    OvlVec4 point;
    OvlVec4 previousPoint;
    int     count = *path->f10;
    int     savedIndex = *(int *)path;
    int     index;
    float   total = 0.0f;

    *(int *)path = 0;
    nextNode.q = *(OvlQuad *)((char *)path->f10 + 0x10);
    if (particles) {
        draw_number(0.666f, &nextNode, (int)moby, 0, 0x80101080, 0x7f, -1, 0xff);
    }
    /* The SN ABI assigns float arguments independently of the integer arguments.
     * Keep the retail argument setup order without changing the shared helper. */
    ((void (*)(void *, int *, float, float, int))compute_path_node_tangent)(
        &nextTangent, (int *)path, tangent_tension, tangent_bias, 0);

    index = 0;
    while (index < count) {
        float segment_parameter;
        float step;
        float segmentLength;
        int   nextIndex;
        currentNode.q = nextNode.q;
        previousTangent.q = nextTangent.q;
        if (particles) {
            draw_number(1.0f, &currentNode, (int)moby, index % 15 + 2, 0x80108010, 0x7f, -1, 0xff);
        }
        segment_parameter = 0.05f;
        nextIndex = resolve_path_node_index(path, index + 1);
        segmentLength = 0.0f;
        nextNode.q = *(OvlQuad *)((char *)path->f10 + nextIndex * 16 + 0x10);
        *(int *)path = index;
        ((void (*)(void *, int *, float, float, int))compute_path_node_tangent)(
            &nextTangent, (int *)path, tangent_tension, tangent_bias, 1);
        previousPoint.q = currentNode.q;
        step = segment_parameter;
        do {
            if (particles) {
                draw_number(0.2f, &previousPoint, (int)moby, 0, 0x80404080, 0x7f, -1, 0xff);
            }
            evaluate_cubic_hermite_segment(
                segment_parameter, &point, &currentNode, &nextNode, &previousTangent, &nextTangent);
            segment_parameter += step;
            segmentLength += fast_vec_dist(&previousPoint, &point);
            previousPoint.q = point.q;
        } while (segment_parameter <= 1.0f);
        total += segmentLength;
        {
            char *nodeData = (char *)path->f10;
            *(float *)(nodeData + (index << 4) + 0x1c) = segmentLength;
        }
        index++;
    }
    *(int *)path = savedIndex;
    return total;
}

s32 D_L04_001B220C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001B25D0[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001B2838[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001B28B8[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001B25D0[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001B2B50[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001B25D0[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001B2450[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001B278C[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001B26B8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001B29B8[1] __attribute__((section(".data"))) = {0};

int D_L02_001B27A8            NOT_SDA = 0;
unsigned char *D_L03_001B218C NOT_SDA = 0;
int D_L03_001B22A8            NOT_SDA = 0;
int D_L04_001B2328            NOT_SDA = 0;
int D_L05_001B2628            NOT_SDA = 0;
int D_L06_001B2928            NOT_SDA = 0;
int D_L08_001B29A8            NOT_SDA = 0;
unsigned char *D_L09_001B2238 NOT_SDA = 0;
int D_L11_001B2BA8            NOT_SDA = 0;
int D_L12_001B2628            NOT_SDA = 0;
int D_L13_001B24A8            NOT_SDA = 0;
int D_L14_001B28A8            NOT_SDA = 0;
int D_L17_001B2AA8            NOT_SDA = 0;

unsigned char *D_L01_001B250C NOT_SDA = 0;
unsigned char *D_L01_001B2538 NOT_SDA = 0;
unsigned char *D_L01_001B25D0 NOT_SDA = 0;
