/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/overlay/moby_hit.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"

#include "asm.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/world/collision/collision_hit_material.h"

void probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");
/* Calls probe_ground_height(a, 0x20, 0.5f). */
void FUN_L00_00259710(void *a);

void FUN_L00_00259710(void *position) {
    probe_ground_height(position, 0x20, 0.5f);
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A778), where it is exact; names translated to the US level program. */

int point_in_path_xy(float *p, float *v, int n) __asm__("FUN_L00_00259740");

int point_in_path_xy(float *test_point, float *path_vertices, int vertex_count) {
    int crossing_parity = 0;
    int i = 0;
    int k;
    for (k = 0; k < vertex_count; k++) {
        float y1 = path_vertices[k * 4 + 1];
        i++;
        if (i == vertex_count)
            i = 0;
        if ((y1 < test_point[1] && test_point[1] <= path_vertices[i * 4 + 1]) ||
            (path_vertices[i * 4 + 1] < test_point[1] && test_point[1] <= y1)) {
            if (path_vertices[k * 4] + (test_point[1] - path_vertices[k * 4 + 1]) /
                                           (path_vertices[i * 4 + 1] - path_vertices[k * 4 + 1]) *
                                           (path_vertices[i * 4] - path_vertices[k * 4]) <
                test_point[0]) {
                crossing_parity = !crossing_parity;
            }
        }
    }
    return crossing_parity;
}

/* True when the pointer at a+0x94 is set and its word at +8 is non-zero. */
int test_moby_linked_record_word8(char *moby) __asm__("FUN_L00_00259830");

int test_moby_linked_record_word8(char *moby) {
    char *moby_linked_record = *(char **)(moby + 0x94);
    if (moby_linked_record && *(int *)(moby_linked_record + 8))
        return 1;
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A890), where it is exact; names translated to the US level program. */

/* Fills a record with b, c, d and flag 0, and clears the 64-bit value at its start. */
void initialize_hit_descriptor(char *a, int b, int c, float d) __asm__("FUN_L00_00259858");
void initialize_hit_descriptor(char *hit_descriptor,
                               int   source_moby,
                               int   hit_mask,

                               float hit_priority) {
    *(int *)(hit_descriptor + 0x10) = source_moby;
    *(int *)(hit_descriptor + 0x14) = hit_mask;
    *(float *)(hit_descriptor + 0x1C) = hit_priority;
    *(int *)(hit_descriptor + 0x20) = 0;
    clear_u64_value(hit_descriptor);
}

#include "qcopy.h"

/* Fills a record with b, c, d and flag 1, and copies the vector q into its start. */
void make_hit_descriptor(char *a, int b, int c, float d, void *q) __asm__("FUN_L00_00259888");
void make_hit_descriptor(char *hit_descriptor,
                         int   source_moby,
                         int   hit_mask,
                         float hit_priority,

                         void *direction_vector) {
    *(int *)(hit_descriptor + 0x10) = source_moby;
    *(int *)(hit_descriptor + 0x14) = hit_mask;
    *(float *)(hit_descriptor + 0x1C) = hit_priority;
    *(int *)(hit_descriptor + 0x20) = 1;
    qcopy(hit_descriptor, direction_vector);
}

#include "eetypes.h"
#include "rnc/mobys/moby.h"

extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
int        play_entity_sound(void *moby,
                             void *position,
                             int   c,
                             float d,
                             float e,
                             float f,
                             int   g,
                             int   h,
                             int   i) __asm__("FUN_L00_002598b0");
int        play_entity_sound(void *moby,
                             void *position,
                             int   hit_mask,
                             float sphere_radius,
                             float hit_priority,
                             float direction_scale,
                             int   hit_descriptor_payload_byte,
                             int   hit_payload_byte,

                             int i) {
    float direction[4] __attribute__((aligned(16)));
    char  descriptor[0x30] __attribute__((aligned(16)));
    if (*(unsigned short *)((char *)moby + 0x34) & 0x100) {
        qcopy(direction, (char *)moby + 0xc0);
    } else {
        direction[0] = fast_cos(*(float *)((char *)moby + 0x48));
        direction[1] = fast_sin(*(float *)((char *)moby + 0x48));
        direction[2] = 0.0f;
    }
    fast_vec_scale(direction, direction, direction_scale);
    direction[2] = 1.0f;
    direction[3] = 5627.9248046875f;
    make_hit_descriptor(descriptor, (int)moby, hit_mask, hit_priority, direction);
    descriptor[0x18] = hit_descriptor_payload_byte;
    descriptor[0x19] = hit_payload_byte;
    *(unsigned short *)(descriptor + 0x1a) = *(unsigned short *)((char *)moby + 0xa6);
    return collect_mobies_in_sphere(position, sphere_radius, i, moby, descriptor);
}

extern int play_entity_sound(void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002598b0");
/* Calls play_entity_sound at a moby point; the f13 argument is ignored and c is passed again as a float. */
int collect_mobies_in_sphere_at_moby_point(void *moby,
                                           int   point,
                                           int   hit_mask,
                                           float sphere_radius,
                                           float unused,
                                           float direction_scale,
                                           int   hit_descriptor_payload_byte,
                                           int   hit_payload_byte,
                                           int   i) __asm__("FUN_L00_002599e8");

int collect_mobies_in_sphere_at_moby_point(void *moby,
                                           int   point,
                                           int   hit_mask,
                                           float sphere_radius,
                                           float unused,
                                           float direction_scale,
                                           int   hit_descriptor_payload_byte,
                                           int   hit_payload_byte,

                                           int i) {
    float position[4] __attribute__((aligned(16)));
    moby_point_position(moby, point, position);
    return play_entity_sound(moby,
                             position,
                             hit_mask,
                             sphere_radius,
                             (float)hit_mask,
                             direction_scale,
                             hit_descriptor_payload_byte,
                             hit_payload_byte,
                             i);
}

typedef unsigned int u128_259a88 __attribute__((mode(TI), aligned(16)));

typedef struct {
    u128_259a88    v;
    int            x, y;
    unsigned char  b0, b1;
    unsigned short s;
    float          f;
    int            flag;
    int            pad[3];
} L_259a88;

typedef struct {
    char pad[0x14];
    int  idx;
} G_259a88;

extern G_259a88 D_L00_00173E40_lui __asm__("D_L00_00173E40") NOT_SDA;
extern MobyHit  moby_hits[] __asm__("D_L00_00178100") NOT_SDA;
void            record_moby_hit(struct Moby *m, L_259a88 *src) __asm__("FUN_L00_00259a88");

void record_moby_hit(struct Moby *owner_moby, L_259a88 *hit_descriptor) {
    L_259a88 hit_descriptor_copy;
    int      i = D_L00_00173E40_lui.idx;
    MobyHit *hit_record;
    ((u128_259a88 *)&hit_descriptor_copy)[0] = ((u128_259a88 *)hit_descriptor)[0];
    ((u128_259a88 *)&hit_descriptor_copy)[1] = ((u128_259a88 *)hit_descriptor)[1];
    ((u128_259a88 *)&hit_descriptor_copy)[2] = ((u128_259a88 *)hit_descriptor)[2];
    hit_descriptor = &hit_descriptor_copy;
    if (owner_moby->hit_slot != 0xFF) {
        MobyHit *existing_hit = &moby_hits[owner_moby->hit_slot];
        if (existing_hit->owner == owner_moby && hit_descriptor_copy.f < existing_hit->damage)
            return;
    }
    hit_record = &moby_hits[i];
    clear_u64_value(hit_record);
    qcopy(&moby_hits[i].direction, &hit_descriptor->v);
    hit_record->source_moby = (u8 *)hit_descriptor->x;
    hit_record->flags = hit_descriptor->y;
    hit_record->unk28 = hit_descriptor->b0;
    hit_record->unk29 = hit_descriptor->b1;
    hit_record->unk2A = hit_descriptor->s;
    hit_record->damage = hit_descriptor->f;
    hit_record->has_direction = hit_descriptor->flag;
    hit_record->owner = owner_moby;
    hit_record->unk38 = 0;
    owner_moby->hit_slot = i;
    D_L00_00173E40_lui.idx = (i + 1) & 0x3F;
}

#include "eetypes.h"

typedef struct {
    char pad[0x14];
    int  idx;
} G_259bc8;

extern G_259bc8 D_L00_00173E40 NOT_SDA;
extern MobyHit                 moby_hits[] __asm__("D_L00_00178100") NOT_SDA;

void spawn_nearby_effect(unsigned char *m, int x, int y, u128 *p, u128 *q, float f) __asm__("FUN_L00_00259bc8");
void spawn_nearby_effect(unsigned char *owner_moby,
                         int            x,
                         int            y,
                         u128          *hit_vector_source,
                         u128          *direction_vector,

                         float f) {
    int      i = D_L00_00173E40.idx;
    MobyHit *hit_record;
    if (owner_moby[0xA4] != 0xFF) {
        MobyHit *existing_hit = &moby_hits[owner_moby[0xA4]];
        if (existing_hit->owner == owner_moby && f < existing_hit->damage)
            return;
    }
    hit_record = &moby_hits[i];
    hit_record->source_moby = (u8 *)x;
    hit_record->flags = y;
    hit_record->damage = f;
    hit_record->owner = owner_moby;
    hit_record->unk38 = 0;
    if (fast_vec_length(direction_vector) > 0.0001f) {
        hit_record->has_direction = 1;
    } else {
        hit_record->has_direction = 0;
    }
    qcopy(&moby_hits[i].position, hit_vector_source);
    qcopy(&moby_hits[i].direction, direction_vector);
    owner_moby[0xA4] = i;
    D_L00_00173E40.idx = (i + 1) & 0x3F;
}

typedef float V4_259d08[4] __attribute__((aligned(16)));
void          FUN_001f9a68_00259d08(void *, void *, float) __asm__("FUN_001f9a68");
int           FUN_001efa68_00259d08(void *, void *, int, void *, int) __asm__("FUN_001efa68");
int           sweep_hit_segment(void *a, void *b, void *c, void *d, void *e, int f, int n) __asm__("FUN_L00_00259d08");
int sweep_hit_segment(void *first_endpoint_end,
                      void *second_endpoint_end,
                      void *first_endpoint_start,
                      void *second_endpoint_start,
                      void *hit_source_moby,
                      int   hit_descriptor_address,

                      int sample_count) {
    V4_259d08 da, db, va, vb;
    float     k, sweep_fraction;
    int       i = 0;
    k = 1.0f / convert_integer_to_float(sample_count);
    fast_vec_sub(da, first_endpoint_end, first_endpoint_start);
    fast_vec_sub(db, second_endpoint_end, second_endpoint_start);
    sweep_fraction = k;
    for (; i < sample_count; i++) {
        FUN_001f9a68_00259d08(va, da, sweep_fraction);
        FUN_001f9a68_00259d08(vb, db, sweep_fraction);
        fast_vec_add(va, va, first_endpoint_start);
        fast_vec_add(vb, vb, second_endpoint_start);
        if (FUN_001efa68_00259d08(va, vb, 0, hit_source_moby, hit_descriptor_address))
            return 1;
        sweep_fraction += k;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025AFA8), where it is exact; names translated to the US level program. */

typedef struct {
    int a[4];
} Vk __attribute__((aligned(16)));

extern void FUN_L00_001fff28(void *, int, float);

void build_quaternion_from_axis_rotations(void  *output_quaternion,
                                          float *axis_rotation_angles) __asm__("FUN_L00_00259f50");

void build_quaternion_from_axis_rotations(void *output_quaternion, float *axis_rotation_angles) {
    Vk z_axis_quaternion, y_axis_quaternion, x_axis_quaternion;
    FUN_L00_001fff28(&x_axis_quaternion, 0, axis_rotation_angles[0]);
    FUN_L00_001fff28(&y_axis_quaternion, 1, axis_rotation_angles[1]);
    FUN_L00_001fff28(&z_axis_quaternion, 2, axis_rotation_angles[2]);
    multiply_quaternions(output_quaternion, &x_axis_quaternion, &y_axis_quaternion);
    multiply_quaternions(output_quaternion, output_quaternion, &z_axis_quaternion);
}

#include "eetypes.h"
#include "rnc/overlay/quad.h"

typedef struct {
    union {
        u128  a_00259fe8;
        float f[4];
    } u_00259fe8;
    u128 b;
} E_259fe8_00259fe8;

extern int D_L00_0015F434 __asm__("D_L00_0015F434");

#include "rnc/overlay/collision.h"

extern float FUN_00213508_00259fe8(void *, int, float) __asm__("FUN_00213508");

extern E_259fe8_00259fe8 D_L00_0016E080[] NOT_SDA;
void                     jet_animate(unsigned char *m, float r) __asm__("FUN_L00_00259fe8");

void jet_animate(unsigned char *moby, float output_scale) {
    float ground_height, y;
    if (moby[0x31] == 0)
        return;
    if (D_L00_0015F434 >= 32)
        return;
    qcopy(&D_L00_0016E080[D_L00_0015F434].u_00259fe8.a_00259fe8, moby + 0x10);
    ground_height = FUN_00213508_00259fe8(moby + 0x10, 0, 0.5f);
    if (collision_hit_material() == 0)
        return;
    D_L00_0016E080[D_L00_0015F434].u_00259fe8.f[2] = ground_height + 0.025f;
    qcopy(&D_L00_0016E080[D_L00_0015F434].b, &coll_hit_normal);
    y = (8.0f - absolute_float(MOBY(moby)->pos.z - ground_height)) * 0.125f;
    if (y < 0.25f)
        y = 0.25f;
    D_L00_0016E080[D_L00_0015F434].u_00259fe8.f[3] = output_scale * y;
    D_L00_0015F434++;
}
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025B178), where it is exact; names translated to the US level program. */

extern int D_L00_00173E40_25a120[] __asm__("D_L00_00173E40");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");

void FUN_L00_0025a120(char *o);

void FUN_L00_0025a120(char *moby) {
    float probe_end[4];
    float probe_start[4];
    void *moby_position = moby + 0x10;
    qcopy(probe_end, moby_position);
    probe_end[2] = probe_end[2] - 16.0f;
    if (probe_end[2] < 0.5f) {
        probe_end[2] = 0.5f;
    }
    qcopy(probe_start, moby_position);
    probe_start[2] = probe_start[2] + 0.5f;
    if (collision_line(probe_start, probe_end, 0x22, 0, 0) != 0) {
        float *output_bounds_source = (float *)D_L00_00173E40_25a120;
        *(float *)(moby + 0x84) = output_bounds_source[10] - 0.2f;
        *(float *)(moby + 0x88) = output_bounds_source[10] + 0.2f;
    } else {
        *(int *)(moby + 0x84) = 0;
        *(int *)(moby + 0x88) = 0;
    }
}

extern MobyHit moby_hits[] __asm__("D_L00_00178100") NOT_SDA;

static inline u8 load_u8_0025a420(u8 *p) {
    return *p;
}

/* Returns the moby_hits entry linked from p+0xA4 when it matches mask; may unlink it. */
MobyHit *find_moby_hit(u8 *p, s32 mask, s32 flag) __asm__("FUN_L00_0025a420");

MobyHit *find_moby_hit(u8 *owner_moby, s32 mask, s32 flag) {
    MobyHit *hit_record;
    u8      *a = owner_moby + 0xA4;
    u8       i = load_u8_0025a420(a);
    if (load_u8_0025a420(a) == 0xFF)
        return 0;
    hit_record = &moby_hits[i];
    if (hit_record->owner != owner_moby)
        return 0;
    if (hit_record->flags & mask)
        return hit_record;
    if (!flag)
        ((u8 *)hit_record->owner)[0xA4] = 0xFF;
    return 0;
}

typedef unsigned int u128_25a478 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_25a478 q;
    f32         f[4];
} V_25a478;

typedef struct {
    u8  pad0[0xA6];
    s16 hA6;
} T_25a478;

typedef struct {
    u8 pad0[0x10];
    union {
        V_25a478 pos;
        struct {
            f32 x, y, z, f1C;
        } s;
    } u;
    T_25a478 *tgt;
    u8        pad24[4];
    u8        state;
    u8        sub;
    u16       kind;
    f32       f2C;
} P_25a478;

typedef struct {
    u8  pad0[8];
    u8  b8;
    u8  pad9[0x14 - 9];
    s16 h14;
    s16 h16;
    u8  pad18[4];
    s16 h1C;
    u8  pad1e[0x2E - 0x1E];
    u8  b2E;
} Q_25a478;

typedef struct {
    u8  pad0[0x10];
    f32 x, y;
} O_25a478;

typedef struct {
    u8  pad0[0x80];
    f32 x, y;
} G_25a478;

extern f32       D_L00_00160080[2] __attribute__((sda));
extern f32       D_L00_001B01C8[][8];
extern u8        D_L00_001B0180[][4];
extern Q_25a478 *suck_get_aim_height(void *) __asm__("FUN_002141f8");

/* hit_reaction is the executable function of the same bytes (src/gameplay). */

typedef struct {
    f32 x, y, z, w;
    u8 *self;
    s32 i24;
    u8  b28;
    u8  b29;
    u16 h2A;
    f32 f2C;
    s32 i30;
} __attribute__((aligned(16))) M_0025a9f8;

void FUN_L00_00259a88_0025a9f8(u8 *, M_0025a9f8 *) __asm__("FUN_L00_00259a88");
void send_blast_hits(u8      *self,
                     OvlQuad *pos,
                     u8     **list,
                     s32      n,
                     u8      *skip,
                     s32      k,
                     s32      c0,
                     s32      c1,
                     f32      f,
                     f32      spd,
                     f32      z) __asm__("FUN_L00_0025a9f8");
void send_blast_hits(u8      *self,
                     OvlQuad *pos,
                     u8     **list,
                     s32      n,
                     u8      *skip,
                     s32      k,
                     s32      c0,
                     s32      c1,
                     f32      f,
                     f32      spd,

                     f32 z) {
    struct {
        OvlQuad    p;
        M_0025a9f8 m;
    } l;
    f32 *pp = (f32 *)&l.p;
    s32  i;
    l.p = *pos;
    for (i = 0; i < n; i++) {
        u8 *e = list[i];
        f32 a;
        if (e == skip)
            continue;
        l.m.self = self;
        l.m.h2A = *(u16 *)(self + 0xA6);
        l.m.i24 = k;
        l.m.f2C = f;
        l.m.i30 = k;
        l.m.b28 = c0;
        l.m.b29 = c1;
        a = fast_arc_tan(*(f32 *)(e + 0x10) - pp[0], *(f32 *)(e + 0x14) - pp[1]);
        l.m.x = fast_cos(a) * spd;
        l.m.y = fast_sin(a) * spd;
        l.m.z = z;
        l.m.w = 5627.925f;
        FUN_L00_00259a88_0025a9f8(list[i], &l.m);
    }
}

typedef union {
    OvlQuad q;
    float   f[4];
} U25ab48;

void trooper_knockback_arc(OvlQuad *v,
                           float   *ang,
                           float   *s1,
                           float   *s2) __asm__("FUN_L00_0025ab48");
void trooper_knockback_arc(OvlQuad *input_vector,
                           float   *output_xy_angle,
                           float   *xy_scaled_value,

                           float *z_scaled_value) {
    U25ab48 input_vector_copy;
    input_vector_copy.q = *input_vector;
    if (input_vector_copy.f[3] != 5627.9248046875f) {
        *output_xy_angle = fast_arc_tan(input_vector_copy.f[0], input_vector_copy.f[1]);
        return;
    }
    *output_xy_angle = fast_arc_tan(input_vector_copy.f[0], input_vector_copy.f[1]);
    *xy_scaled_value *= vector_length_xy(&input_vector_copy);
    input_vector_copy.f[3] = 0;
    *z_scaled_value *= input_vector_copy.f[2];
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025BC48), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vc0 __attribute__((aligned(16)));

float FUN_L00_0025abf0(float *a, float *b, float *out, float speed, float g);
float FUN_L00_0025abf0(float *start_position,
                       float *target_position,
                       float *out,
                       float  speed,

                       float vertical_acceleration) {
    Vc0   displacement;
    float travel_time;
    fast_vec_sub(&displacement, target_position, start_position);
    travel_time = vector_length_xy(&displacement) / speed;
    if (out) {
        *out = travel_time;
    }
    if (travel_time == 0.0f) {
        return 0.0f;
    }
    return -((start_position[2] - target_position[2]) +
             vertical_acceleration * (travel_time * travel_time) * 0.5f) /
           travel_time;
}

typedef union {
    u128  q;
    float f[4];
} Vec4_25aca0 __attribute__((aligned(16)));

extern void  compute_moby_joint_matrices(void *, int, int) __asm__("FUN_0020fa90");
extern void  FUN_001f9a68_25aca0(float, void *, void *) __asm__("FUN_001f9a68");
extern void  scale_vector(float, void *, void *) __asm__("FUN_001f9a80");
extern void  FUN_001251e8_25aca0(float, void *, void *) __asm__("FUN_001251e8");
extern float FUN_001f9af0_25aca0(void *) __asm__("FUN_001f9af0");
extern void  divide_vector_xyz(float, void *, void *) __asm__("FUN_L00_001ff378");
extern float FUN_001f9dc8_25aca0(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_25aca0(float) __asm__("FUN_001f9de0");

int generate_random_points_from_shape_entries(float          density,
                                              unsigned char *entity,
                                              int            maximum,
                                              Vec4_25aca0   *out,
                                              int            mask) __asm__("FUN_L00_0025aca0");

int generate_random_points_from_shape_entries(

    float density, unsigned char *entity, int maximum, Vec4_25aca0 *out, int mask) {
    enum HitShape {
        SHAPE_INLINE_CENTER_SPHERE = 0x0,
        SHAPE_INDEXED_CENTER_SPHERE = 0x2,
        SHAPE_XY_DISK_WITH_Z_EXTENT = 0x3,
        SHAPE_ENDPOINT_SEGMENT_WITH_BALL_OFFSET = 0x4
    };

    float          weights[20];
    unsigned char *entries[20];
    Vec4_25aca0    first, second, third, fourth, fifth;
    unsigned char *list;
    unsigned char *entry;
    int            count, index, chosen, emitted, limit;
    unsigned int   flags;
    unsigned int   last;
    float          total, weight, radius, extent, random;
    int            type;

    {
        unsigned char *entry;
        entry = *(unsigned char **)(*(unsigned char **)(entity + 0x24) + 0x10);
        if (!entry)
            return 0;
        if (*(short *)(entry + 2))
            compute_moby_joint_matrices(entity, *(short *)(entry + 2), 0);
        if (*(int *)(entry + 4) <= 0)
            return 0;
        count = 0;
        index = 0;
        total = 0.0f;
        entry += 0x10;
        {
            unsigned char *list;
            list = entry + 4;
            do {
                float        radius, extent;
                int          type;
                unsigned int active;
                flags = *(unsigned int *)(list - 4);
                last = flags >> 31;
                type = flags & 0xff;
                active = flags & 0x20000;
                if (!(((mask >> index) ^ 1) & 1) && active) {
                    entries[count] = entry;
                    switch (type) {
                    case SHAPE_INLINE_CENTER_SPHERE:
                    case 1:
                    case SHAPE_INDEXED_CENTER_SPHERE: {
                        if (type < 2) {
                            qcopy(&first, (void *)((entry + 0x10)));
                            scale_vector(*(float *)(entity + 0x2c) * 0.0009765625f, &first, &first);
                            radius = first.f[3];
                        } else {
                            qcopy(&first, (void *)((0x70000000 + (*(int *)list << 4))));
                            qcopy(&second, (void *)((entry + 0x10)));
                            fast_vec_add(&first, &first, &second);
                            FUN_001f9a68_25aca0(
                                *(float *)(entity + 0x2c) * 0.0009765625f, &first, &first);
                            radius = *(float *)(entity + 0x2c);
                            radius = *(float *)(list + 8) * radius * 0.0009765625f;
                        }
                        weights[count] = radius * 4.1887903f * radius * radius;
                        break;
                    }
                    case SHAPE_XY_DISK_WITH_Z_EXTENT: {
                        float radius, extent;
                        qcopy(&first, (void *)((entry + 0x10)));
                        FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
                        radius = first.f[3] * *(float *)(entity + 0x2c);
                        radius *= 0.0009765625f;
                        extent = *(float *)list * (*(float *)(entity + 0x2c) * 0.0009765625f);
                        weights[count] = radius * 4.187743f * radius * radius +
                                         radius * 1.5707964f * radius * extent;
                        break;
                    }
                    case SHAPE_ENDPOINT_SEGMENT_WITH_BALL_OFFSET: {
                        FUN_001251e8_25aca0(*(float *)(entity + 0x2c) * 0.0009765625f,
                                            &first,
                                            (void *)(0x70000000 + (*(short *)list << 4)));
                        FUN_001251e8_25aca0(*(float *)(entity + 0x2c) * 0.0009765625f,
                                            &second,
                                            (void *)(0x70000000 + (*(short *)(list + 2) << 4)));
                        fast_vec_sub(&third, &first, &second);
                        radius = FUN_001f9af0_25aca0(&third);
                        extent = *(float *)(list + 8) * *(float *)(entity + 0x2c) * 0.0009765625f;
                        weights[count] = extent * 1.5707964f * extent * radius;
                        break;
                    }
                    }
                    total += weights[count];
                    count++;
                }
                index++;
                list += 0x20;
                entry += 0x20;
            } while (!last);
        }
    }
    limit = (int)(density * total + 0.5f);
    if (maximum < limit)
        limit = maximum;
    for (emitted = 0; emitted < limit; emitted++) {
        random = random_float_between(0.0f, total);
        for (chosen = 0; chosen < count; chosen++) {
            if (random < weights[chosen])
                break;
            random -= weights[chosen];
        }
        entry = entries[chosen];
        switch (type = *entry) {
        case SHAPE_INLINE_CENTER_SPHERE:
        case 1:
        case SHAPE_INDEXED_CENTER_SPHERE: {
            float radius, extent;
            if (type < 2) {
                qcopy(&first, (void *)((entry + 0x10)));
                FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
                extent = first.f[3] * *(float *)(entity + 0x2c);
            } else {
                qcopy(&first, (void *)((0x70000000 + (*(int *)(entry + 4) << 4))));
                qcopy(&second, (void *)((entry + 0x10)));
                fast_vec_add(&first, &first, &second);
                FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
                extent = *(float *)(entry + 0xc) * *(float *)(entity + 0x2c);
            }
            extent *= 0.0009765625f;
            divide_vector_xyz(1024.0f, &first, &first);
            do {
                second.f[0] = random_float_between(-extent, extent);
                second.f[1] = random_float_between(-extent, extent);
                second.f[2] = random_float_between(-extent, extent);
            } while (!(FUN_001f9af0_25aca0(&second) <= extent));
            /* Fakematch: the do-while (0) raises the loop-weighted refs of entity + 0x10,
             * &out[emitted] and entity + 0xc0 so global alloc gives &second the last saved register. */
            do {
                fast_vec_add(&second, &second, &first);
                transform_vector_by_basis(&second, &second, entity + 0xc0);
                fast_vec_add(&out[emitted], &second, entity + 0x10);
            } while (0);
            break;
        }
        case SHAPE_XY_DISK_WITH_Z_EXTENT: {
            float radius, extent, weight, random, angle;
            qcopy(&first, (void *)((entry + 0x10)));
            FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
            radius = first.f[3] * *(float *)(entity + 0x2c);
            extent = *(float *)(entry + 4) * *(float *)(entity + 0x2c);
            divide_vector_xyz(1024.0f, &first, &first);
            radius *= 0.0009765625f;
            extent *= 0.0009765625f;
            weight = random_float_between(0.0f, radius * radius);
            weight = square_root(weight);
            random = random_float_between(0.0f, extent);
            angle = random_angle_radians();
            second.f[0] = FUN_001f9dc8_25aca0(angle) * weight;
            second.f[1] = FUN_001f9de0_25aca0(angle) * weight;
            second.f[2] = random;
            fast_vec_add(&second, &second, entity + 0x10);
            fast_vec_add(&out[emitted], &first, &second);
            break;
        }
        case SHAPE_ENDPOINT_SEGMENT_WITH_BALL_OFFSET: {
            float extent, weight;
            list = entry + 4;
            FUN_001251e8_25aca0(
                *(float *)(entity + 0x2c), &first, (void *)(0x70000000 + (*(short *)list << 4)));
            FUN_001251e8_25aca0(*(float *)(entity + 0x2c),
                                &second,
                                (void *)(0x70000000 + (*(short *)(list + 2) << 4)));
            fast_vec_sub(&third, &second, &first);
            weight = random_float_between(0.0f, 1.0f);
            FUN_001f9a68_25aca0(weight, &third, &third);
            fast_vec_add(&fourth, &third, &first);
            divide_vector_xyz(1024.0f, &fourth, &fourth);
            extent = *(float *)(entry + 0xc) * *(float *)(entity + 0x2c);
            extent = extent * 0.0009765625f;
            do {
                fifth.f[0] = random_float_between(-extent, extent);
                fifth.f[1] = random_float_between(-extent, extent);
                fifth.f[2] = random_float_between(-extent, extent);
            } while (!(FUN_001f9af0_25aca0(&fifth) <= extent));
            fast_vec_add(&fifth, &fifth, &fourth);
            transform_vector_by_basis(&fifth, &fifth, entity + 0xc0);
            fast_vec_add(&out[emitted], &fifth, entity + 0x10);
            break;
        }
        }
    }
    return limit;
}

typedef struct {
    unsigned char pad0[8];
    float         f8;
    unsigned char padC[4];
    float         f10;
    float         f14;
    float         f18;
    float         f1C;
    unsigned char pad20[4];
    float         f24;
    float         f28;
    unsigned char b2C;
    unsigned char b2D;
    unsigned char b2E;
    unsigned char state;
    float         f30;
    float         f34;
    float         f38;
    float         f3C;
} S_25b440;

typedef struct {
    unsigned char pad0[0x10];
    float         pos[2];
    float         z;
    unsigned char pad1C[0x52 - 0x1C];
    unsigned char b52;
    unsigned char pad53[0x70 - 0x53];
    unsigned char b70;
} O_25b440;

extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void  approach_w(void *, void *, float, float, float, float) __asm__("FUN_L00_00258278");
extern float FUN_001f9dc8_25b440(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_25b440(float) __asm__("FUN_001f9de0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float FUN_00213508_25b440(void *, int, float) __asm__("FUN_00213508");

int update_arc_motion(O_25b440 *moving_object,
                      S_25b440 *arc_motion_state) __asm__("FUN_L00_0025b440");

int update_arc_motion(O_25b440 *moving_object, S_25b440 *arc_motion_state) {
    enum {
        CHECK_ARC_ENTRY_GATES = 0,
        INTEGRATE_ARC_MOTION = 1,
        DESCEND_TO_GROUND = 2,
        WAIT_AFTER_GROUND_CLAMP = 3
    };
    float motion_displacement[4];
    float prediction_time;
    float ground_height;

    switch (arc_motion_state->state) {
    case CHECK_ARC_ENTRY_GATES:
        if (moving_object->b52 != arc_motion_state->b2C) {
            break;
        }
        if (compute_interpolated_record_value(moving_object) <= arc_motion_state->f24) {
            break;
        }
        if (moving_object->b70 & 2) {
            blend_moby_animation(moving_object, arc_motion_state->b2D, 0, 10);
            arc_motion_state->state = 1;
        }
    case INTEGRATE_ARC_MOTION:
        approach_w(moving_object,
                   &arc_motion_state->f30,
                   arc_motion_state->f18,
                   arc_motion_state->f34,
                   arc_motion_state->f38,
                   arc_motion_state->f3C);
        motion_displacement[0] = FUN_001f9dc8_25b440(arc_motion_state->f18) * arc_motion_state->f14;
        motion_displacement[1] = FUN_001f9de0_25b440(arc_motion_state->f18) * arc_motion_state->f14;
        motion_displacement[2] = arc_motion_state->f10;
        fast_vec_add(moving_object->pos, moving_object->pos, motion_displacement);
        arc_motion_state->f10 -= arc_motion_state->f1C;
        if (arc_motion_state->f10 < 0.0f) {
            prediction_time = scale_time(arc_motion_state->f28 + arc_motion_state->f28 + 10.0f);
            if (moving_object->z <=
                arc_motion_state->f8 -
                    (arc_motion_state->f10 * prediction_time -
                     arc_motion_state->f1C * 0.5f * prediction_time * prediction_time)) {
                blend_moby_animation(moving_object, arc_motion_state->b2E, 0, 10);
                arc_motion_state->state = 2;
            }
        }
        break;
    case DESCEND_TO_GROUND:
        moving_object->z += arc_motion_state->f10;
        arc_motion_state->f10 -= arc_motion_state->f1C;
        ground_height = FUN_00213508_25b440(moving_object->pos, 0, 0.5f);
        if (moving_object->z <= ground_height) {
            moving_object->z = ground_height;
            arc_motion_state->state = 3;
        } else {
            motion_displacement[0] =
                FUN_001f9dc8_25b440(arc_motion_state->f18) * arc_motion_state->f14;
            motion_displacement[1] =
                FUN_001f9de0_25b440(arc_motion_state->f18) * arc_motion_state->f14;
            motion_displacement[2] = 0.0f;
            fast_vec_add(moving_object->pos, moving_object->pos, motion_displacement);
        }
    case WAIT_AFTER_GROUND_CLAMP:
        if (moving_object->b70 & 2) {
            arc_motion_state->state = 4;
        }
        break;
    }
    return arc_motion_state->state;
}

/* Linear interpolation: a + (b - a) * t. */
f32 interpolate_scalar_linear(f32 a, f32 b, f32 t) __asm__("FUN_L00_0025b6a8");

f32 interpolate_scalar_linear(f32 start_value, f32 end_value, f32 interpolation_factor) {
    return start_value + (end_value - start_value) * interpolation_factor;
}

#include "rnc/math/vector.h"

typedef struct {
    f32 *p;
} OutPtr;

/* Interpolates the four components of two copied vectors by t into dst. */
void interpolate_vector(f32 *dst, f32 *a, f32 *b, f32 t) __asm__("FUN_L00_0025b6b8");
void interpolate_vector(f32 *interpolated_vector,
                        f32 *start_vector,
                        f32 *end_vector,
                        f32  interpolation_factor) __asm__("FUN_L00_0025b6b8");

void interpolate_vector(f32 *interpolated_vector,
                        f32 *start_vector,
                        f32 *end_vector,

                        f32 interpolation_factor) {
    Vec4 left, right;
    /* Fakematch: the struct copy of dst and the dead dst = 0 stores keep each store ahead of the next call's loads. */
    OutPtr out = {interpolated_vector};

    interpolated_vector = 0;
    left.q = *(u128 *)start_vector;
    right.q = *(u128 *)end_vector;
    end_vector = right.f;
    out.p[0] = interpolate_scalar_linear(left.f[0], end_vector[0], interpolation_factor);
    interpolated_vector = out.p;
    out = (OutPtr){interpolated_vector};
    interpolated_vector = 0;
    out.p[1] = interpolate_scalar_linear(left.f[1], end_vector[1], interpolation_factor);
    out.p[2] = interpolate_scalar_linear(left.f[2], end_vector[2], interpolation_factor);
    out.p[3] = interpolate_scalar_linear(left.f[3], end_vector[3], interpolation_factor);
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025C7A8), where it is exact; names translated to the US level program. */

/* Turns from angle a toward angle b by fraction c of the difference. */
float interpolate_angle(float a, float b, float c) __asm__("FUN_L00_0025b750");

float interpolate_angle(float start_angle, float target_angle, float blend_fraction) {
    float angle_difference = fast_subtract_rotations(target_angle, start_angle);
    return fast_add_rotations(start_angle, angle_difference * blend_fraction);
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025C7F0), where it is exact; names translated to the US level program. */

void advance_damped_scalar_velocity(float *velocity,
                                    float  rate,
                                    float  error_gain,
                                    float  damping_coefficient,
                                    float  limit) __asm__("FUN_L00_0025b798");

void advance_damped_scalar_velocity(

    float *velocity, float rate, float error_gain, float damping_coefficient, float limit) {
    float unclamped_velocity = *velocity + (error_gain * rate - damping_coefficient * (*velocity));

    *velocity = unclamped_velocity;
    if (0.0f < limit) {
        if (limit < unclamped_velocity) {
            *velocity = limit;
        } else if (unclamped_velocity < -limit) {
            *velocity = -limit;
        }
    }
    if (absolute_float(rate) < *velocity) {
        *velocity = absolute_float(rate);
    } else if (*velocity < -absolute_float(rate)) {
        *velocity = -absolute_float(rate);
    }
}

s32 D_L03_00173F54[1] __attribute__((section(".data"))) = {0};

s32 D_L09_00173FD4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_00174554[1] __attribute__((section(".data"))) = {0};

E_259fe8_00259fe8 D_L01_0016E500 = {0};

E_259fe8_00259fe8 D_L02_0016E680 = {0};

E_259fe8_00259fe8 D_L03_0016E180 = {0};

E_259fe8_00259fe8 D_L04_0016E200 = {0};

E_259fe8_00259fe8 D_L05_0016E480 = {0};

E_259fe8_00259fe8 D_L06_0016E800 = {0};

E_259fe8_00259fe8 D_L07_0016E100 = {0};

E_259fe8_00259fe8 D_L08_0016E880 = {0};

E_259fe8_00259fe8 D_L09_0016E200 = {0};

E_259fe8_00259fe8 D_L10_0016E500 = {0};

E_259fe8_00259fe8 D_L11_0016EA80 = {0};

E_259fe8_00259fe8 D_L12_0016E500 = {0};

E_259fe8_00259fe8 D_L13_0016E380 = {0};

E_259fe8_00259fe8 D_L14_0016E780 = {0};

E_259fe8_00259fe8 D_L15_0016E680 = {0};

E_259fe8_00259fe8 D_L16_0016E480 = {0};

E_259fe8_00259fe8 D_L17_0016E980 = {0};

E_259fe8_00259fe8 D_L18_0016EA80 = {0};
