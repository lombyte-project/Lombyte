/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

void probe_ground_height(void *, s32, f32) __asm__("FUN_00213508");
/* Calls probe_ground_height(a, 0x20, 0.5f). */
void FUN_L00_00259710(void *a);
void FUN_L00_00259710(void *position) {
    probe_ground_height(position, 0x20, 0.5f);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A778), where it is exact; names translated to the US level program. */

int point_in_path_xy(float *p, float *v, int n) __asm__("FUN_L00_00259740");
int point_in_path_xy(float *test_point, float *v, int vertex_count) {
    int r = 0;
    int i = 0;
    int k;
    for (k = 0; k < vertex_count; k++) {
        float y1 = v[k * 4 + 1];
        i++;
        if (i == vertex_count)
            i = 0;
        if ((y1 < test_point[1] && test_point[1] <= v[i * 4 + 1]) || (v[i * 4 + 1] < test_point[1] && test_point[1] <= y1)) {
            if (v[k * 4] +
                    (test_point[1] - v[k * 4 + 1]) / (v[i * 4 + 1] - v[k * 4 + 1]) * (v[i * 4] - v[k * 4]) <
                test_point[0]) {
                r = !r;
            }
        }
    }
    return r;
}
/* True when the pointer at a+0x94 is set and its word at +8 is non-zero. */
int FUN_L00_00259830(char *moby) {
    char *p = *(char **)(moby + 0x94);
    if (p && *(int *)(p + 8))
        return 1;
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025A890), where it is exact; names translated to the US level program. */

extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

/* Fills a record with b, c, d and flag 0, and clears the 64-bit value at its start. */
void FUN_L00_00259858(char *a, int b, int c, float d);
void FUN_L00_00259858(char *hit_descriptor, int b, int c, float d) {
    *(int *)(hit_descriptor + 0x10) = b;
    *(int *)(hit_descriptor + 0x14) = c;
    *(float *)(hit_descriptor + 0x1C) = d;
    *(int *)(hit_descriptor + 0x20) = 0;
    clear_u64_value(hit_descriptor);
}
#include "qcopy.h"
/* Fills a record with b, c, d and flag 1, and copies the vector q into its start. */
void make_hit_descriptor(char *a, int b, int c, float d, void *q) __asm__("FUN_L00_00259888");
void make_hit_descriptor(char *hit_descriptor, int b, int c, float d, void *direction_vector) {
    *(int *)(hit_descriptor + 0x10) = b;
    *(int *)(hit_descriptor + 0x14) = c;
    *(float *)(hit_descriptor + 0x1C) = d;
    *(int *)(hit_descriptor + 0x20) = 1;
    qcopy(hit_descriptor, direction_vector);
}
#include "eetypes.h"
#include "rnc/math/fast_math.h"
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
int play_entity_sound(void *moby, void *position, int c, float d, float e, float f, int g, int h, int i) __asm__("FUN_L00_002598b0");
int play_entity_sound(void *moby, void *position, int c, float sphere_radius, float e, float direction_scale, int g, int h, int i) {
    float direction[4] __attribute__((aligned(16)));
    char descriptor[0x30] __attribute__((aligned(16)));
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
    make_hit_descriptor(descriptor, (int)moby, c, e, direction);
    descriptor[0x18] = g;
    descriptor[0x19] = h;
    *(unsigned short *)(descriptor + 0x1a) = *(unsigned short *)((char *)moby + 0xa6);
    return collect_mobies_in_sphere(position, sphere_radius, i, moby, descriptor);
}
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int play_entity_sound(void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002598b0");
/* Calls play_entity_sound at a moby point; the f13 argument is ignored and c is passed again as a float. */
int FUN_L00_002599e8(void *moby, int point, int c, float sphere_radius, float unused, float direction_scale, int g, int h, int i) {
    float position[4] __attribute__((aligned(16)));
    moby_point_position(moby, point, position);
    return play_entity_sound(moby, position, c, sphere_radius, (float)c, direction_scale, g, h, i);
}
typedef unsigned int u128_259a88 __attribute__((mode(TI), aligned(16)));
typedef struct {
    u128_259a88 v;
    int x, y;
    unsigned char b0, b1;
    unsigned short s;
    float f;
    int flag;
    int pad[3];
} L_259a88;
typedef struct {
    u128_259a88 a;
    u128_259a88 b;
    int x, y;
    unsigned char b0, b1;
    unsigned short s;
    float f;
    int flag;
    void *owner;
    int z;
    int pad2;
} T_259a88;
typedef struct {
    char pad[0x14];
    int idx;
} G_259a88;
extern G_259a88 D_L00_00173E40_259a88 __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern T_259a88 D_L00_00178100_259a88[] __asm__("D_L00_00178100") __attribute__((section(".data")));
void FUN_001f99f8_259a88(void *) __asm__("FUN_001f99f8");
void FUN_L00_00259a88(unsigned char *m, L_259a88 *src);
void FUN_L00_00259a88(unsigned char *owner_moby, L_259a88 *src) {
    L_259a88 l;
    int i = D_L00_00173E40_259a88.idx;
    T_259a88 *hit_record;
    ((u128_259a88 *)&l)[0] = ((u128_259a88 *)src)[0];
    ((u128_259a88 *)&l)[1] = ((u128_259a88 *)src)[1];
    ((u128_259a88 *)&l)[2] = ((u128_259a88 *)src)[2];
    src = &l;
    if (owner_moby[0xA4] != 0xFF) {
        T_259a88 *existing_hit = &D_L00_00178100_259a88[owner_moby[0xA4]];
        if (existing_hit->owner == owner_moby && l.f < existing_hit->f)
            return;
    }
    hit_record = &D_L00_00178100_259a88[i];
    FUN_001f99f8_259a88(hit_record);
    qcopy(&D_L00_00178100_259a88[i].b, &src->v);
    hit_record->x = src->x;
    hit_record->y = src->y;
    hit_record->b0 = src->b0;
    hit_record->b1 = src->b1;
    hit_record->s = src->s;
    hit_record->f = src->f;
    hit_record->flag = src->flag;
    hit_record->owner = owner_moby;
    hit_record->z = 0;
    owner_moby[0xA4] = i;
    D_L00_00173E40_259a88.idx = (i + 1) & 0x3F;
}
#include "eetypes.h"
typedef struct {
    u128 a;
    u128 b;
    int x, y;
    int pad;
    float f;
    int flag;
    void *owner;
    int z;
    int pad2;
} T_259bc8;
typedef struct {
    char pad[0x14];
    int idx;
} G_259bc8;
extern G_259bc8 D_L00_00173E40 __attribute__((section(".data")));
extern T_259bc8 moby_hits[] __asm__("D_L00_00178100") __attribute__((section(".data")));
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");

void spawn_nearby_effect(unsigned char *m, int x, int y, u128 *p, u128 *q, float f) __asm__("FUN_L00_00259bc8");
void spawn_nearby_effect(unsigned char *owner_moby, int x, int y, u128 *p, u128 *q, float f) {
    int i = D_L00_00173E40.idx;
    T_259bc8 *hit_record;
    if (owner_moby[0xA4] != 0xFF) {
        T_259bc8 *existing_hit = &moby_hits[owner_moby[0xA4]];
        if (existing_hit->owner == owner_moby && f < existing_hit->f)
            return;
    }
    hit_record = &moby_hits[i];
    hit_record->x = x;
    hit_record->y = y;
    hit_record->f = f;
    hit_record->owner = owner_moby;
    hit_record->z = 0;
    if (fast_vec_length(q) > 0.0001f) {
        hit_record->flag = 1;
    } else {
        hit_record->flag = 0;
    }
    qcopy(&moby_hits[i].a, p);
    qcopy(&moby_hits[i].b, q);
    owner_moby[0xA4] = i;
    D_L00_00173E40.idx = (i + 1) & 0x3F;
}
typedef float V4_259d08[4] __attribute__((aligned(16)));
float FUN_001fa6c0_00259d08(int) __asm__("FUN_001fa6c0");
void FUN_001f9a68_00259d08(void *, void *, float) __asm__("FUN_001f9a68");
void FUN_001f9a10_00259d08(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9a28_00259d08(void *, void *, void *) __asm__("FUN_001f9a28");
int FUN_001efa68_00259d08(void *, void *, int, void *, int) __asm__("FUN_001efa68");
int sweep_hit_segment(void *a, void *b, void *c, void *d, void *e, int f, int n) __asm__("FUN_L00_00259d08");
int sweep_hit_segment(void *a, void *b, void *c, void *d, void *e, int f, int sample_count) {
    V4_259d08 da, db, va, vb;
    float k, t;
    int i = 0;
    k = 1.0f / FUN_001fa6c0_00259d08(sample_count);
    FUN_001f9a28_00259d08(da, a, c);
    FUN_001f9a28_00259d08(db, b, d);
    t = k;
    for (; i < sample_count; i++) {
        FUN_001f9a68_00259d08(va, da, t);
        FUN_001f9a68_00259d08(vb, db, t);
        FUN_001f9a10_00259d08(va, va, c);
        FUN_001f9a10_00259d08(vb, vb, d);
        if (FUN_001efa68_00259d08(va, vb, 0, e, f))
            return 1;
        t += k;
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025AFA8), where it is exact; names translated to the US level program. */

typedef struct {
    int a[4];
} Vk __attribute__((aligned(16)));

extern void multiply_quaternions(void *, void *, void *) __asm__("FUN_001fa3c0");
extern void FUN_L00_001fff28(void *, int, float);

void FUN_L00_00259f50(void *output_quaternion, float *v) {
    Vk z_axis_quaternion, y_axis_quaternion, x_axis_quaternion;
    FUN_L00_001fff28(&x_axis_quaternion, 0, v[0]);
    FUN_L00_001fff28(&y_axis_quaternion, 1, v[1]);
    FUN_L00_001fff28(&z_axis_quaternion, 2, v[2]);
    multiply_quaternions(output_quaternion, &x_axis_quaternion, &y_axis_quaternion);
    multiply_quaternions(output_quaternion, output_quaternion, &z_axis_quaternion);
}
#include "eetypes.h"
#include "rnc/overlay/quad.h"
typedef struct {
    union {
        u128 a_00259fe8;
        float f[4];
    } u_00259fe8;
    u128 b;
} E_259fe8_00259fe8;
extern int D_L00_0015F434 __asm__("D_L00_0015F434");
extern u128 D_L00_00173E80_00259fe8 __asm__("D_L00_00173E80") __attribute__((section(".data")));
extern float FUN_00213508_00259fe8(void *, int, float) __asm__("FUN_00213508");
extern int FUN_001f0b58_00259fe8(void) __asm__("FUN_001f0b58");
extern float FUN_001f99c0_00259fe8(float) __asm__("FUN_001f99c0");

extern E_259fe8_00259fe8 D_L00_0016E080[] __attribute__((section(".data")));
void jet_animate(unsigned char *m, float r) __asm__("FUN_L00_00259fe8");
void jet_animate(unsigned char *moby, float r) {
    float ground_height, y;
    if (moby[0x31] == 0)
        return;
    if (D_L00_0015F434 >= 32)
        return;
    qcopy(&D_L00_0016E080[D_L00_0015F434].u_00259fe8.a_00259fe8, moby + 0x10);
    ground_height = FUN_00213508_00259fe8(moby + 0x10, 0, 0.5f);
    if (FUN_001f0b58_00259fe8() == 0)
        return;
    D_L00_0016E080[D_L00_0015F434].u_00259fe8.f[2] = ground_height + 0.025f;
    qcopy(&D_L00_0016E080[D_L00_0015F434].b, &D_L00_00173E80_00259fe8);
    y = (8.0f - FUN_001f99c0_00259fe8(*(float *)(moby + 0x18) - ground_height)) * 0.125f;
    if (y < 0.25f)
        y = 0.25f;
    D_L00_0016E080[D_L00_0015F434].u_00259fe8.f[3] = r * y;
    D_L00_0015F434++;
}
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025B178), where it is exact; names translated to the US level program. */

extern int D_L00_00173E40_25a120[] __asm__("D_L00_00173E40");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");

void FUN_L00_0025a120(char *o);
void FUN_L00_0025a120(char *moby) {
    float probe_end[4];
    float b[4];
    void *moby_position = moby + 0x10;
    qcopy(probe_end, moby_position);
    probe_end[2] = probe_end[2] - 16.0f;
    if (probe_end[2] < 0.5f) {
        probe_end[2] = 0.5f;
    }
    qcopy(b, moby_position);
    b[2] = b[2] + 0.5f;
    if (collision_line(b, probe_end, 0x22, 0, 0) != 0) {
        float *t = (float *)D_L00_00173E40_25a120;
        *(float *)(moby + 0x84) = t[10] - 0.2f;
        *(float *)(moby + 0x88) = t[10] + 0.2f;
    } else {
        *(int *)(moby + 0x84) = 0;
        *(int *)(moby + 0x88) = 0;
    }
}
struct Ent_z {
    s32 a[13];
    u8 *f34;
    s32 b[2];
};
extern struct Ent_z D_L00_00178100_c[] __asm__("D_L00_00178100") __attribute__((section(".data")));
static inline u8 load_u8_0025a420(u8 *p) {
    return *p;
}
/* Returns the moby_hits entry linked from p+0xA4 when it matches mask; may unlink it. */
struct Ent_z *find_moby_hit(u8 *p, s32 mask, s32 flag) __asm__("FUN_L00_0025a420");
struct Ent_z *find_moby_hit(u8 *owner_moby, s32 mask, s32 flag) {
    struct Ent_z *hit_record;
    u8 *a = owner_moby + 0xA4;
    u8 i = load_u8_0025a420(a);
    if (load_u8_0025a420(a) == 0xFF)
        return 0;
    hit_record = &D_L00_00178100_c[i];
    if (hit_record->f34 != owner_moby)
        return 0;
    if (hit_record->a[9] & mask)
        return hit_record;
    if (!flag)
        hit_record->f34[0xA4] = 0xFF;
    return 0;
}
typedef unsigned int u128_25a478 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_25a478 q;
    f32 f[4];
} V_25a478;
typedef struct {
    u8 pad0[0xA6];
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
    u8 pad24[4];
    u8 state;
    u8 sub;
    u16 kind;
    f32 f2C;
} P_25a478;
typedef struct {
    u8 pad0[8];
    u8 b8;
    u8 pad9[0x14 - 9];
    s16 h14;
    s16 h16;
    u8 pad18[4];
    s16 h1C;
    u8 pad1e[0x2E - 0x1E];
    u8 b2E;
} Q_25a478;
typedef struct {
    u8 pad0[0x10];
    f32 x, y;
} O_25a478;
typedef struct {
    u8 pad0[0x80];
    f32 x, y;
} G_25a478;
extern f32 D_L00_00160080[2] __attribute__((sda));
extern f32 D_L00_001B01C8[][8];
extern u8 D_L00_001B0180[][4];
extern Q_25a478 *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern s32 fast_dec_timer_s16(s16 *) __asm__("FUN_001f9770");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern f32 FUN_001f9e90_c(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 FUN_001f9af0_c(void *) __asm__("FUN_001f9af0");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");

/* hit_reaction is the executable function of the same bytes (src/gameplay). */

typedef struct {
    f32 x, y, z, w;
    u8 *self;
    s32 i24;
    u8 b28;
    u8 b29;
    u16 h2A;
    f32 f2C;
    s32 i30;
} __attribute__((aligned(16))) M_0025a9f8;
f32 FUN_001f9e90_0025a9f8(f32, f32) __asm__("FUN_001f9e90");
f32 FUN_001f9dc8_0025a9f8(f32) __asm__("FUN_001f9dc8");
f32 FUN_001f9de0_0025a9f8(f32) __asm__("FUN_001f9de0");
void FUN_L00_00259a88_0025a9f8(u8 *, M_0025a9f8 *) __asm__("FUN_L00_00259a88");
void send_blast_hits(u8 *self, OvlQuad *pos, u8 **list, s32 n, u8 *skip, s32 k, s32 c0,
                      s32 c1, f32 f, f32 spd, f32 z) __asm__("FUN_L00_0025a9f8");
void send_blast_hits(u8 *self, OvlQuad *pos, u8 **list, s32 n, u8 *skip, s32 k, s32 c0,
                      s32 c1, f32 f, f32 spd, f32 z) {
    struct {
        OvlQuad p;
        M_0025a9f8 m;
    } l;
    f32 *pp = (f32 *)&l.p;
    s32 i;
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
        a = FUN_001f9e90_0025a9f8(*(f32 *)(e + 0x10) - pp[0], *(f32 *)(e + 0x14) - pp[1]);
        l.m.x = FUN_001f9dc8_0025a9f8(a) * spd;
        l.m.y = FUN_001f9de0_0025a9f8(a) * spd;
        l.m.z = z;
        l.m.w = 5627.925f;
        FUN_L00_00259a88_0025a9f8(list[i], &l.m);
    }
}
typedef union {
    OvlQuad q;
    float f[4];
} U25ab48;
float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
float vector_length_xy(void *) __asm__("FUN_001f9b20");
void trooper_knockback_arc(OvlQuad *v, float *ang, float *s1, float *s2) __asm__("FUN_L00_0025ab48");
void trooper_knockback_arc(OvlQuad *v, float *ang, float *xy_scaled_value, float *z_scaled_value) {
    U25ab48 input_vector_copy;
    input_vector_copy.q = *v;
    if (input_vector_copy.f[3] != 5627.9248046875f) {
        *ang = fast_arc_tan(input_vector_copy.f[0], input_vector_copy.f[1]);
        return;
    }
    *ang = fast_arc_tan(input_vector_copy.f[0], input_vector_copy.f[1]);
    *xy_scaled_value *= vector_length_xy(&input_vector_copy);
    input_vector_copy.f[3] = 0;
    *z_scaled_value *= input_vector_copy.f[2];
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025BC48), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z, w;
} Vc0 __attribute__((aligned(16)));

extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

float FUN_L00_0025abf0(float *a, float *b, float *out, float speed, float g);
float FUN_L00_0025abf0(float *start_position, float *b, float *out, float speed, float g) {
    Vc0 displacement;
    float travel_time;
    subtract_vector_xyz(&displacement, b, start_position);
    travel_time = vector_length_xy(&displacement) / speed;
    if (out) {
        *out = travel_time;
    }
    if (travel_time == 0.0f) {
        return 0.0f;
    }
    return -((start_position[2] - b[2]) + g * (travel_time * travel_time) * 0.5f) / travel_time;
}
typedef union {
    u128 q;
    float f[4];
} Vec4_25aca0 __attribute__((aligned(16)));

extern void FUN_002531e0_25aca0(void *, int, int) __asm__("FUN_0020fa90");
extern void FUN_001f9a10_25aca0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28_25aca0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a68_25aca0(float, void *, void *) __asm__("FUN_001f9a68");
extern void FUN_001f9a80_25aca0(float, void *, void *) __asm__("FUN_001f9a80");
extern void FUN_001251e8_25aca0(float, void *, void *) __asm__("FUN_001251e8");
extern float FUN_001f9af0_25aca0(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9988_25aca0(float) __asm__("FUN_001f9988");
extern void FUN_001f9cf8_25aca0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_L00_001ff378_25aca0(float, void *, void *) __asm__("FUN_L00_001ff378");
extern float FUN_001f9dc8_25aca0(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_25aca0(float) __asm__("FUN_001f9de0");
extern float FUN_002132a8_25aca0(float, float) __asm__("FUN_002132a8");
extern float FUN_00213308_25aca0(void) __asm__("FUN_00213308");

int FUN_L00_0025aca0(float density, unsigned char *entity, int maximum, Vec4_25aca0 *out, int mask) {
    float weights[20];
    unsigned char *entries[20];
    Vec4_25aca0 first, second, third, fourth, fifth;
    unsigned char *list;
    unsigned char *entry;
    int count, index, chosen, emitted, limit;
    unsigned int flags;
    unsigned int last;
    float total, weight, radius, extent, random;
    int type;

    {
        unsigned char *entry;
        entry = *(unsigned char **)(*(unsigned char **)(entity + 0x24) + 0x10);
        if (!entry) return 0;
        if (*(short *)(entry + 2)) FUN_002531e0_25aca0(entity, *(short *)(entry + 2), 0);
        if (*(int *)(entry + 4) <= 0) return 0;
        count = 0;
        index = 0;
        total = 0.0f;
        entry += 0x10;
        {
            unsigned char *list;
            list = entry + 4;
            do {
                float radius, extent;
                int type;
                unsigned int active;
                flags = *(unsigned int *)(list - 4);
                last = flags >> 31;
                type = flags & 0xff;
                active = flags & 0x20000;
                if (!(((mask >> index) ^ 1) & 1) && active) {
                    entries[count] = entry;
                    switch (type) {
                    case 0:
                    case 1:
                    case 2: {
                        if (type < 2) {
                            qcopy(&first, (void *)((entry + 0x10)));
                            FUN_001f9a80_25aca0(*(float *)(entity + 0x2c) * 0.0009765625f, &first, &first);
                            radius = first.f[3];
                        } else {
                            qcopy(&first, (void *)((0x70000000 + (*(int *)list << 4))));
                            qcopy(&second, (void *)((entry + 0x10)));
                            FUN_001f9a10_25aca0(&first, &first, &second);
                            FUN_001f9a68_25aca0(*(float *)(entity + 0x2c) * 0.0009765625f, &first, &first);
                            radius = *(float *)(entity + 0x2c);
                            radius = *(float *)(list + 8) * radius * 0.0009765625f;
                        }
                        weights[count] = radius * 4.1887903f * radius * radius;
                        break;
                    }
                    case 3: {
                        float radius, extent;
                        qcopy(&first, (void *)((entry + 0x10)));
                        FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
                        radius = first.f[3] * *(float *)(entity + 0x2c);
                        radius *= 0.0009765625f;
                        extent = *(float *)list * (*(float *)(entity + 0x2c) * 0.0009765625f);
                          weights[count] = radius * 4.187743f * radius * radius + radius * 1.5707964f * radius * extent;
                        break;
                    }
                    case 4: {
                        FUN_001251e8_25aca0(*(float *)(entity + 0x2c) * 0.0009765625f, &first, (void *)(0x70000000 + (*(short *)list << 4)));
                        FUN_001251e8_25aca0(*(float *)(entity + 0x2c) * 0.0009765625f, &second, (void *)(0x70000000 + (*(short *)(list + 2) << 4)));
                        FUN_001f9a28_25aca0(&third, &first, &second);
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
    if (maximum < limit) limit = maximum;
    for (emitted = 0; emitted < limit; emitted++) {
        random = FUN_002132a8_25aca0(0.0f, total);
        for (chosen = 0; chosen < count; chosen++) {
            if (random < weights[chosen]) break;
            random -= weights[chosen];
        }
        entry = entries[chosen];
        switch (type = *entry) {
        case 0:
        case 1:
        case 2: {
            float radius, extent;
            if (type < 2) {
                qcopy(&first, (void *)((entry + 0x10)));
                FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
                extent = first.f[3] * *(float *)(entity + 0x2c);
            } else {
                qcopy(&first, (void *)((0x70000000 + (*(int *)(entry + 4) << 4))));
                qcopy(&second, (void *)((entry + 0x10)));
                FUN_001f9a10_25aca0(&first, &first, &second);
                FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
                extent = *(float *)(entry + 0xc) * *(float *)(entity + 0x2c);
            }
            extent *= 0.0009765625f;
            FUN_L00_001ff378_25aca0(1024.0f, &first, &first);
            do {
                second.f[0] = FUN_002132a8_25aca0(-extent, extent);
                second.f[1] = FUN_002132a8_25aca0(-extent, extent);
                second.f[2] = FUN_002132a8_25aca0(-extent, extent);
            } while (!(FUN_001f9af0_25aca0(&second) <= extent));
            /* Fakematch: the do-while (0) raises the loop-weighted refs of entity + 0x10,
             * &out[emitted] and entity + 0xc0 so global alloc gives &second the last saved register. */
            do {
                FUN_001f9a10_25aca0(&second, &second, &first);
                FUN_001f9cf8_25aca0(&second, &second, entity + 0xc0);
                FUN_001f9a10_25aca0(&out[emitted], &second, entity + 0x10);
            } while (0);
            break;
        }
        case 3: {
            float radius, extent, weight, random, angle;
            qcopy(&first, (void *)((entry + 0x10)));
            FUN_001f9a68_25aca0(*(float *)(entity + 0x2c), &first, &first);
            radius = first.f[3] * *(float *)(entity + 0x2c);
            extent = *(float *)(entry + 4) * *(float *)(entity + 0x2c);
            FUN_L00_001ff378_25aca0(1024.0f, &first, &first);
            radius *= 0.0009765625f;
            extent *= 0.0009765625f;
            weight = FUN_002132a8_25aca0(0.0f, radius * radius);
            weight = FUN_001f9988_25aca0(weight);
            random = FUN_002132a8_25aca0(0.0f, extent);
            angle = FUN_00213308_25aca0();
            second.f[0] = FUN_001f9dc8_25aca0(angle) * weight;
            second.f[1] = FUN_001f9de0_25aca0(angle) * weight;
            second.f[2] = random;
            FUN_001f9a10_25aca0(&second, &second, entity + 0x10);
            FUN_001f9a10_25aca0(&out[emitted], &first, &second);
            break;
        }
        case 4: {
            float extent, weight;
            list = entry + 4;
            FUN_001251e8_25aca0(*(float *)(entity + 0x2c), &first, (void *)(0x70000000 + (*(short *)list << 4)));
            FUN_001251e8_25aca0(*(float *)(entity + 0x2c), &second, (void *)(0x70000000 + (*(short *)(list + 2) << 4)));
            FUN_001f9a28_25aca0(&third, &second, &first);
            weight = FUN_002132a8_25aca0(0.0f, 1.0f);
            FUN_001f9a68_25aca0(weight, &third, &third);
            FUN_001f9a10_25aca0(&fourth, &third, &first);
            FUN_L00_001ff378_25aca0(1024.0f, &fourth, &fourth);
            extent = *(float *)(entry + 0xc) * *(float *)(entity + 0x2c);
            extent = extent * 0.0009765625f;
            do {
                fifth.f[0] = FUN_002132a8_25aca0(-extent, extent);
                fifth.f[1] = FUN_002132a8_25aca0(-extent, extent);
                fifth.f[2] = FUN_002132a8_25aca0(-extent, extent);
            } while (!(FUN_001f9af0_25aca0(&fifth) <= extent));
            FUN_001f9a10_25aca0(&fifth, &fifth, &fourth);
            FUN_001f9cf8_25aca0(&fifth, &fifth, entity + 0xc0);
            FUN_001f9a10_25aca0(&out[emitted], &fifth, entity + 0x10);
            break;
        }
        }
    }
    return limit;
}
typedef struct {
    unsigned char pad0[8];
    float f8;
    unsigned char padC[4];
    float f10;
    float f14;
    float f18;
    float f1C;
    unsigned char pad20[4];
    float f24;
    float f28;
    unsigned char b2C;
    unsigned char b2D;
    unsigned char b2E;
    unsigned char state;
    float f30;
    float f34;
    float f38;
    float f3C;
} S_25b440;
typedef struct {
    unsigned char pad0[0x10];
    float pos[2];
    float z;
    unsigned char pad1C[0x52 - 0x1C];
    unsigned char b52;
    unsigned char pad53[0x70 - 0x53];
    unsigned char b70;
} O_25b440;
extern float FUN_0020c9e0_25b440(void *) __asm__("FUN_0020c9e0");
extern void FUN_00212f90_25b440(void *, int, int, int) __asm__("FUN_00212f90");
extern void FUN_L00_00258278_25b440(void *, void *, float, float, float,
                                    float) __asm__("FUN_L00_00258278");
extern float FUN_001f9dc8_25b440(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_25b440(float) __asm__("FUN_001f9de0");
extern void FUN_001f9a10_25b440(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f96b0_25b440(float) __asm__("FUN_001f96b0");
extern float FUN_00213508_25b440(void *, int, float) __asm__("FUN_00213508");

int FUN_L00_0025b440(O_25b440 *moving_object, S_25b440 *arc_motion_state) {
    enum {
        CHECK_ARC_ENTRY_GATES = 0,
        INTEGRATE_ARC_MOTION = 1,
        DESCEND_TO_GROUND = 2,
        WAIT_AFTER_GROUND_CLAMP = 3
    };
    float v[4];
    float t;
    float g;

    switch (arc_motion_state->state) {
    case CHECK_ARC_ENTRY_GATES:
        if (moving_object->b52 != arc_motion_state->b2C) {
            break;
        }
        if (FUN_0020c9e0_25b440(moving_object) <= arc_motion_state->f24) {
            break;
        }
        if (moving_object->b70 & 2) {
            FUN_00212f90_25b440(moving_object, arc_motion_state->b2D, 0, 10);
            arc_motion_state->state = 1;
        }
    case INTEGRATE_ARC_MOTION:
        FUN_L00_00258278_25b440(moving_object, &arc_motion_state->f30, arc_motion_state->f18, arc_motion_state->f34, arc_motion_state->f38, arc_motion_state->f3C);
        v[0] = FUN_001f9dc8_25b440(arc_motion_state->f18) * arc_motion_state->f14;
        v[1] = FUN_001f9de0_25b440(arc_motion_state->f18) * arc_motion_state->f14;
        v[2] = arc_motion_state->f10;
        FUN_001f9a10_25b440(moving_object->pos, moving_object->pos, v);
        arc_motion_state->f10 -= arc_motion_state->f1C;
        if (arc_motion_state->f10 < 0.0f) {
            t = FUN_001f96b0_25b440(arc_motion_state->f28 + arc_motion_state->f28 + 10.0f);
            if (moving_object->z <= arc_motion_state->f8 - (arc_motion_state->f10 * t - arc_motion_state->f1C * 0.5f * t * t)) {
                FUN_00212f90_25b440(moving_object, arc_motion_state->b2E, 0, 10);
                arc_motion_state->state = 2;
            }
        }
        break;
    case DESCEND_TO_GROUND:
        moving_object->z += arc_motion_state->f10;
        arc_motion_state->f10 -= arc_motion_state->f1C;
        g = FUN_00213508_25b440(moving_object->pos, 0, 0.5f);
        if (moving_object->z <= g) {
            moving_object->z = g;
            arc_motion_state->state = 3;
        } else {
            v[0] = FUN_001f9dc8_25b440(arc_motion_state->f18) * arc_motion_state->f14;
            v[1] = FUN_001f9de0_25b440(arc_motion_state->f18) * arc_motion_state->f14;
            v[2] = 0.0f;
            FUN_001f9a10_25b440(moving_object->pos, moving_object->pos, v);
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
f32 FUN_L00_0025b6a8(f32 a, f32 b, f32 t);
f32 FUN_L00_0025b6a8(f32 a, f32 b, f32 t) {
    return a + (b - a) * t;
}
#include "rnc/math/vector.h"

typedef struct {
    f32 *p;
} OutPtr;

/* Interpolates the four components of two copied vectors by t into dst. */
void FUN_L00_0025b6b8(f32 *dst, f32 *a, f32 *b, f32 t);
void FUN_L00_0025b6b8(f32 *dst, f32 *start_vector, f32 *b, f32 interpolation_factor) {
    Vec4 left, right;
    /* Fakematch: the struct copy of dst and the dead dst = 0 stores keep each store ahead of the next call's loads. */
    OutPtr out = { dst };

    dst = 0;
    left.q = *(u128 *)start_vector;
    right.q = *(u128 *)b;
    b = right.f;
    out.p[0] = FUN_L00_0025b6a8(left.f[0], b[0], interpolation_factor);
    dst = out.p;
    out = (OutPtr){ dst };
    dst = 0;
    out.p[1] = FUN_L00_0025b6a8(left.f[1], b[1], interpolation_factor);
    out.p[2] = FUN_L00_0025b6a8(left.f[2], b[2], interpolation_factor);
    out.p[3] = FUN_L00_0025b6a8(left.f[3], b[3], interpolation_factor);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025C7A8), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");

/* Turns from angle a toward angle b by fraction c of the difference. */
float FUN_L00_0025b750(float a, float b, float c);
float FUN_L00_0025b750(float start_angle, float b, float blend_fraction) {
    float r = fast_subtract_rotations(b, start_angle);
    return fast_add_rotations(start_angle, r * blend_fraction);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025C7F0), where it is exact; names translated to the US level program. */

float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L00_0025b798(float *ptr, float rate, float b, float damping_coefficient, float limit) {
    float v = *ptr + (b * rate - damping_coefficient * (*ptr));

    *ptr = v;
    if (0.0f < limit) {
        if (limit < v) {
            *ptr = limit;
        } else if (v < -limit) {
            *ptr = -limit;
        }
    }
    if (absolute_float(rate) < *ptr) {
        *ptr = absolute_float(rate);
    } else if (*ptr < -absolute_float(rate)) {
        *ptr = -absolute_float(rate);
    }
}

s32 D_L03_00173F54[1] __attribute__((section(".data"))) = {0};
s32 D_L09_00173FD4[1] __attribute__((section(".data"))) = {0};
s32 D_L14_00174554[1] __attribute__((section(".data"))) = {0};
