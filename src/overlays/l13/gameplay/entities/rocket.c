/* Level 13 moby classes: rocket, poison_vessel, blarg_space_commando, blarg_ship. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/quad.h"

/* Spawns a Gemlik moby that copies its look and transform from a source moby. */


extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);

char *FUN_L13_003058a8(struct Moby *src, short cls) {
    char *m = create_moby(0x4D1);
    char *d;
    char *pos;
    if (m != 0) {
        d = *(char **)(m + 0x78);
        m[0x31] = 1;
        ((unsigned char *)m)[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0x7F;
        m[0x20] = 1;
        *(char **)(d + 0x20) = src;
        *(short *)(d + 0x26) = cls;
        *(short *)(d + 0x24) = 1;
        pos = m + 0x10;
        moby_point_position(src, cls, pos);
        qcopy(m + 0x40, &src->rot);
        *(OvlQuad *)(m + 0xC0) = *(OvlQuad *)&src->rot_row0;
        *(OvlQuad *)(m + 0xD0) = *(OvlQuad *)&src->rot_row1;
        *(OvlQuad *)(m + 0xE0) = *(OvlQuad *)&src->rot_row2;
        qcopy(d + 0x10, pos);
        *(int *)(m + 0x94) = 0;
        FUN_L00_00250df8(m);
    }
    return m;
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Initializes a Gemlik moby from its data block, copying transforms and resetting state. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00306E20), where it is exact; names translated to the US level program. */


extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);

void FUN_L13_00305988(struct Moby *moby, char *pos, int distance_reference_moby, float max_travel_distance) {
    char *d = (char *)moby->pvars;
    char *s = *(char **)(d + 0x20);
    char *mp;
    *(float *)(d + 0x28) = max_travel_distance;
    qcopy(&moby->rot, s + 0x40);
    mp = &moby->pos;
    *(OvlQuad *)&moby->rot_row0 = *(OvlQuad *)(s + 0xC0);
    *(OvlQuad *)&moby->rot_row1 = *(OvlQuad *)(s + 0xD0);
    *(OvlQuad *)&moby->rot_row2 = *(OvlQuad *)(s + 0xE0);
    moby_point_position(*(void **)(d + 0x20), *(short *)(d + 0x26), mp);
    qcopy(d + 0x10, mp);
    qcopy(d, pos);
    *(int *)(d + 0x2C) = distance_reference_moby;
    moby->state = 0;
    moby->flags &= 0xFFFE;
    moby->collision = moby->pclass->collision;
    FUN_L00_00250df8(moby);
}
extern char *FUN_L00_0025a420(void *, int, int);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_001fa6d0(float);
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float FUN_002132a8(float, float);
extern int FUN_L00_002712b8(void *, void *, int, int, int, int, float, float, float, float, float);
extern void FUN_L00_00259888(void *, struct Moby *, int, void *, float);
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_0026dd20(void *, void *, int, int, float);
extern void FUN_L07_0029a2a0(void *, void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern float D_0015ED6C;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) L13Vector_5a58;

void FUN_L13_00305a58(struct Moby *moby);
void FUN_L13_00305a58(struct Moby *moby) {
    extern struct { char pad[0x18]; struct Moby *moby; int pad1; L13Vector_5a58 pos; L13Vector_5a58 pad2; L13Vector_5a58 matrix; } D_L13_00174140;
    extern int D_L13_0015F580;
    char *data = (char *)moby->pvars;
    struct Moby *source;
    struct Moby *target;
    struct {
        L13Vector_5a58 old_pos;
        L13Vector_5a58 velocity;
        L13Vector_5a58 direction;
        char hit[48];
        L13Vector_5a58 random_direction;
    } work;
    int i;
    int stationary;
    float length;
    float distance;
    float other_distance;

    qcopy(&work.old_pos, &moby->pos);
    if (moby->state == 1) {
        source = *(struct Moby **)(data + 0x20);
        if (source == 0 || source->oclass != 0x65 || source->state == 0xfe || source->state == 0xfd)
            goto remove;
        qcopy(&moby->rot, &source->rot);
        *(OvlQuad *)&moby->rot_row0 = *(OvlQuad *)&source->rot_row0;
        *(OvlQuad *)&moby->rot_row1 = *(OvlQuad *)&source->rot_row1;
        *(OvlQuad *)&moby->rot_row2 = *(OvlQuad *)&source->rot_row2;
        FUN_L00_0024f7c8(*(struct Moby **)(data + 0x20), *(short *)(data + 0x26), &moby->pos);
        qcopy(data + 0x10, &moby->pos);
        {
        struct Moby *source = *(struct Moby **)(data + 0x20);
        if (source != 0 && source->state != 0xfe && source->state != 0xfd && (source->flags & 1))
            moby->flags |= 1;
        else
            moby->flags &= 0xfffe;
        }
        goto bounds;
    }

    {
        char *target = FUN_L00_0025a420(moby, 0x230000, 0);
        if (target != 0 && *(struct Moby **)(target + 0x20) != 0 &&
            (*(struct Moby **)(target + 0x20))->oclass == 0x1df)
            goto special;
    }
    fast_vec_add(&moby->pos, &moby->pos, data);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(&work.direction, data, -convert_integer_to_float(i) * 0.25f);
        fast_vec_add(&work.velocity, &moby->pos, &work.direction);
        length = FUN_002132a8(25000.0f, 35000.0f);
        target = (struct Moby *)FUN_L00_002712b8(
            &work.velocity, &D_L13_0015F580, scale_game_frames(15), 0x60, 0x4040ff, 3,
            length, 500.0f, 1.0f, -0.0004f, 0.0f);
        if (target != 0)
            *((char *)target + 9) = FUN_001fa6d0(4.0f) - 0x80;
    }
    qcopy(&work.velocity, data);
    work.velocity.z = 0.0f;
    set_vector_length_xy(&work.velocity, &work.velocity, 1.0f);
    work.velocity.z = 1.0f;
    FUN_L00_00259888(work.hit, moby, 0x10001, &work.velocity, 1.0f);
    stationary = *(short *)(data + 0x24) == 0;
    distance = vector_distance_xy(&moby->pos, data + 0x10);
    other_distance = vector_distance_xy(*(char **)(data + 0x2c) + 0x10, data + 0x10);
    if (*(float *)(data + 0x28) < distance || other_distance < distance) {
        if (moby->visible != 0) {
            int i;
            for (i = 0; i < 3; i++) {
                *(OvlQuad *)&work.random_direction = 0;
                work.random_direction.x = FUN_002132a8(-1.0f, 1.0f);
                work.random_direction.y = FUN_002132a8(-1.0f, 1.0f);
                work.random_direction.z = FUN_002132a8(-1.0f, 1.0f);
                *(OvlQuad *)&work.direction = *(OvlQuad *)&work.random_direction;
                length = fast_vec_length(&work.random_direction);
                fast_vec_normalize(&work.direction, &work.direction, length * 0.1f);
                fast_vec_add(&work.direction, &work.random_direction, &work.direction);
                fast_vec_normalize(&work.direction, &work.direction,
                    FUN_002132a8(D_0015ED6C + D_0015ED6C, D_0015ED6C * 4.0f));
                FUN_L07_0029a2a0(&moby->pos, &work.direction);
            }
        }
        FUN_L13_003060b8(moby);
        goto remove;
    }
    if (collision_line(&work.old_pos, &moby->pos, stationary,
                     moby, work.hit) == 0)
        goto bounds;
    {
    struct Moby *target = D_L13_00174140.moby;
    if (target != 0 && target == *(struct Moby **)(data + 0x20))
        goto bounds;
    }
    qcopy(&moby->pos, &D_L13_00174140.pos);
    if (moby->visible != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)&work.direction = 0;
            work.direction.x = FUN_002132a8(-1.0f, 1.0f);
            work.direction.y = FUN_002132a8(-1.0f, 1.0f);
            work.direction.z = FUN_002132a8(-1.0f, 1.0f);
            *(OvlQuad *)&work.random_direction = *(OvlQuad *)&work.direction;
            FUN_L00_001ff660(&work.direction, data, &D_L13_00174140.matrix);
            length = fast_vec_length(&work.direction);
            fast_vec_normalize(&work.random_direction, &work.random_direction, length * 0.5f);
            fast_vec_add(&work.random_direction, &work.direction, &work.random_direction);
            fast_vec_normalize(&work.random_direction, &work.random_direction,
                FUN_002132a8(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
            FUN_L00_0026dd20(&moby->pos, &work.random_direction, 0x7f2f4f6f,
                random_int_between(scale_game_frames(10), scale_game_frames(15)), 30000.0f);
        }
    }
    {
    struct Moby *target = D_L13_00174140.moby;
    if (target != 0 && target->oclass == 0x1df) {
special:
        FUN_L13_00306148(moby);
    } else {
        FUN_L13_003060b8(moby);
    }
    }
remove:
    mark_moby_for_removal(moby);
    return;
bounds:
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f ||
        moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
        moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal(moby);
}
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00307550), where it is exact; names translated to the US level program. */

void FUN_L13_003060b8(struct Moby *moby) {
    char vector[16];
    clear_u64_value(vector);
    func_L00_0025F4A8_alt(moby, vector, &moby->pos, 2.0f, 1.0f, 12, 4, 18, 4.0f, 3.0f, 1.0f, 2.0f,
                          0, 20.0f, 1, 1, -1, 0);
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_003075E0), where it is exact; names translated to the US level program. */

extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void FUN_L13_00306148(struct Moby *moby) {
    char vector[16];
    clear_u64_value(vector);
    spawn_explosion(moby, vector, &moby->pos, 0.0f, 0.0f, 5, 1, 5, 0.0f, 0.0f, 0.0f, 1.0f, 0,
                     0.0f, 0, 1, -1, 0);
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00307660), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m_c(int);
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void FUN_L00_00250df8(void *);
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern char *func_0020D348_m_c(int) __asm__("FUN_0020c4f8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

char *FUN_L13_003061c8(float speed, float *pos, float *dir, void *arg) {
    char *m;
    char *p;
    float d;
    m = func_0020D348_m_c(0x4D6);
    if (m != 0) {
        *(int *)(m + 0x40) = 0;
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(short *)(m + 0x32) = 0x7F;
        m[0x31] = 1;
        p = *(char **)(m + 0x78);
        d = vector_length_xy(dir);
        *(float *)(m + 0x44) = -fast_arc_tan(d, dir[2]);
        *(float *)(m + 0x48) = fast_arc_tan(dir[0], dir[1]);
        qcopy(m + 0x10, pos);
        qcopy(p + 0x10, pos);
        qcopy(p, dir);
        blend_moby_animation(m, 1, 0, scale_game_frames(5));
        *(short *)(m + 0x34) = 0x208;
        *(int *)(p + 0x24) = 1;
        *(void **)(p + 0x20) = arg;
        *(float *)(p + 0x28) = speed;
        m[0x23] = 0x10;
        *(int *)(p + 0x2C) = func_001FA898_r(speed / fast_vec_length(p) + 1.0f);
        FUN_L00_00250df8(m);
    }
    return m;
}
#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00307798), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int OvlQuad_p __attribute__((mode(TI)));

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) L13Vec_p;


struct L13Ent00178480 {
    char pad0[0x24];
    int i24;
    unsigned char b28;
    unsigned char b29;
    unsigned short s2A;
    float f2C;
    int pad30;
    char *p34;
    char pad38[8];
};

extern char D_0013E533[];
extern f32 fast_vec_length_c(void *) __asm__("func_001F9AF0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_L00_00257b90(int, int);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern struct L13Ent00178480 D_L13_00178400[];
extern void FUN_L00_00259888_c(void *, struct Moby *, int, float, void *) __asm__("FUN_L00_00259888");
extern void FUN_L00_00259a88(void *, void *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void set_vector_length_xy_c(void *, void *, f32) __asm__("FUN_001f9c48");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

void FUN_L13_00306300(struct Moby *moby) {
    extern struct { char pad[0x18]; struct Moby *moby; int pad1; L13Vec_p pos; L13Vec_p pad2; L13Vec_p matrix; } D_L13_00174140;
    char *data = (char *)moby->pvars;
    struct {
        L13Vec_p old_pos;
        char hit[48];
        L13Vec_p velocity;
        L13Vec_p direction;
        L13Vec_p random_direction;
    } work;
    int i;
    int flags;
    int stationary;
    unsigned char *base;
    char *ent;
    struct L13Ent00178480 *e;

    qcopy(&work.old_pos, &moby->pos);
    add_vector_xyz(&moby->pos, &moby->pos, data);
    qcopy(&work.velocity, data);
    work.velocity.z = 0.0f;
    set_vector_length_xy_c(&work.velocity, &work.velocity, 1.0f);
    work.velocity.z = 1.0f;
    flags = 0x10001;
    base = (unsigned char *)((char *)((char *)&hero));
    if (base[0x1FF6] != 0 && *(float *)(data + 0xC) != 31337.6015625f &&
        fast_vec_dist(&moby->pos, base + 0x80) < 2.0f) {
        fast_vec_normalize_c(data, data, 0.5f);
        subtract_vector_xyz(&moby->pos, &moby->pos, data);
        qzero(data);
        *(int *)(data + 0x2C) = 5;
        *(float *)(data + 0xC) = 31337.6015625f;
        for (i = 0; i < 6; i++) {
            unsigned char *b = (unsigned char *)((char *)((char *)&hero));
            ent = ((char **)(b + 0x2020))[i];
            if (ent != 0) {
                int f2 = flags | 1;
                FUN_L00_00259888_c(work.hit, moby, f2, 1.0f, &work.velocity);
                work.hit[0x18] = 1;
                *(unsigned short *)(work.hit + 0x1A) = *(unsigned short *)((char *)moby + 0xA6);
                work.hit[0x19] = 1;
                if ((unsigned char)ent[0xA4] == 0xFF || (e = &D_L13_00178400[(unsigned char)ent[0xA4]])->p34 != ent) {
                    FUN_L00_00259a88(ent, work.hit);
                } else {
                    e->f2C += 1.0f;
                    D_L13_00178400[(unsigned char)ent[0xA4]].b28 = 1;
                    D_L13_00178400[(unsigned char)ent[0xA4]].b29 = 1;
                    D_L13_00178400[(unsigned char)ent[0xA4]].s2A = *(unsigned short *)((char *)moby + 0xA6);
                    { struct L13Ent00178480 *t = &D_L13_00178400[(unsigned char)ent[0xA4]]; t->i24 = f2; }
                }
                break;
            }
        }
    }

    FUN_L00_00259888_c(work.hit, moby, 0x10001, 1.0f, &work.velocity);
    *(unsigned short *)(work.hit + 0x1A) = *(unsigned short *)((char *)moby + 0xA6);
    work.hit[0x18] = 1;
    work.hit[0x19] = 1;
    stationary = *(int *)(data + 0x24) == 0;
    if (*(float *)(data + 0x28) < vector_distance_xy_alt(&moby->pos, data + 0x10) || tick_countdown_32_alt(data + 0x2C) != 0) {
        if (moby->visible != 0) {
            int i;
            for (i = 0; i < 3; i++) {
                *(OvlQuad_p *)&work.random_direction = 0;
                work.random_direction.x = random_float_between(-1.0f, 1.0f);
                work.random_direction.y = random_float_between(-1.0f, 1.0f);
                work.random_direction.z = random_float_between(-1.0f, 1.0f);
                *(OvlQuad_p *)&work.direction = *(OvlQuad_p *)&work.random_direction;
                fast_vec_normalize_c(&work.direction, &work.direction, fast_vec_length_c(&work.random_direction) * 0.1f);
                add_vector_xyz(&work.direction, &work.random_direction, &work.direction);
                fast_vec_normalize_c(&work.direction, &work.direction,
                    random_float_between(D_0015ED6C + D_0015ED6C, D_0015ED6C * 4.0f));
                FUN_L07_0029a2a0(&moby->pos, &work.direction);
            }
        }
        goto remove;
    }
    if (collision_line(&work.old_pos, &moby->pos, stationary, *(void **)(data + 0x20), work.hit) == 0)
        goto bounds;
    qcopy(&moby->pos, &D_L13_00174140.pos);
    if (moby->visible != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad_p *)&work.random_direction = 0;
            work.random_direction.x = random_float_between(-1.0f, 1.0f);
            work.random_direction.y = random_float_between(-1.0f, 1.0f);
            work.random_direction.z = random_float_between(-1.0f, 1.0f);
            *(OvlQuad_p *)&work.direction = *(OvlQuad_p *)&work.random_direction;
            FUN_L00_001ff660(&work.random_direction, data, &D_L13_00174140.matrix);
            fast_vec_normalize_c(&work.direction, &work.direction, fast_vec_length_c(&work.random_direction) * 0.5f);
            add_vector_xyz(&work.direction, &work.random_direction, &work.direction);
            fast_vec_normalize_c(&work.direction, &work.direction,
                random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
            FUN_L00_0026dd20(&moby->pos, &work.direction, 0x7F2F4F6F,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(15)), 30000.0f);
        }
    }
    {
        struct Moby *target = D_L13_00174140.moby;
        if (target != 0 && *(short *)((char *)target + 0xA6) == 0)
            allocate_voice_for_target_entry(0, 0, (int)moby);
    }
remove:
    mark_moby_for_removal_c(moby);
    return;
bounds:
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f ||
        moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
        moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal_c(moby);
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L13_003073c8.s", FUN_L13_003073c8);
#else
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern float dot_vector_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern char D_L13_001670C0[];
typedef struct { int v[6]; } L13EffectColors;
extern L13EffectColors D_L13_001F5560, D_L13_001F5578;
extern char *FUN_L13_0030c138(void *, void *, int, int);
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern char *FUN_L00_002df808(void *, void *, void *, float, int, unsigned char, unsigned char, unsigned char, int);

void FUN_L13_003073c8(struct Moby *moby, void *vars) {
    struct {
        OvlVec4 direction, velocity, axis, offset, pos, random_direction, random_vector;
    } work;
    int count;
    int duration;
    int *color, *second_color;
    float distance, twice_distance, extra, speed, amount;
    float scale;
    work.axis.q = 0;
    work.axis.f[2] = D_0015ED6C + D_0015ED6C;
    work.velocity.q = work.axis.q;
    work.axis.q = 0;
    scale = 1.5f;
    work.axis.f[2] = 1.0f;
    /* Retail offsets the copied position before emitting the effects. */
    qcopy(&work.pos, &moby->pos);
    work.pos.f[2] += 1.0f;
    scale_vector_xyz(&work.velocity, &work.velocity, scale);
    subtract_vector_xyz(&work.direction, D_L13_001670C0, &work.pos);
    distance = vector_length_xyz(&work.direction);
    {
        int i = 9;
        do {
            int lo, hi, duration;
            OvlVec4 *random_direction, *offset, *axis;
            work.random_vector.q = 0;
            work.random_vector.f[0] = FUN_002132a8(-1.0f, 1.0f);
            i--;
            work.random_vector.f[1] = FUN_002132a8(-1.0f, 1.0f);
            random_direction = &work.random_direction;
            offset = &work.offset;
            axis = &work.axis;
            random_direction->q = work.random_vector.q;
            amount = dot_vector_xyz(random_direction, axis);
            fast_vec_normalize(offset, axis, amount);
            subtract_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(offset, axis, FUN_002132a8(0.0f, 1.0f));
            add_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(random_direction, random_direction,
                scale * FUN_002132a8(3.5f, 6.5f) * D_0015ED6C);
            add_vector_xyz(random_direction, random_direction, &work.velocity);
            lo = scale_game_frames(60); hi = scale_game_frames(120);
            duration = random_int_between(lo, hi);
            FUN_L13_0030c138(&work.pos, random_direction, duration, 0);
        } while (i >= 0);
    }
    twice_distance = distance + distance;
    {
        int i = 3;
        do {
            int lo, hi, duration;
            OvlVec4 *random_direction, *axis, *offset;
            work.random_vector.q = 0;
            work.random_vector.f[0] = FUN_002132a8(-1.0f, 1.0f);
            i--;
            work.random_vector.f[1] = FUN_002132a8(-1.0f, 1.0f);
            random_direction = &work.random_direction;
            axis = &work.axis;
            offset = &work.offset;
            random_direction->q = work.random_vector.q;
            fast_vec_normalize(offset, axis, dot_vector_xyz(random_direction, axis));
            subtract_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(offset, axis, FUN_002132a8(0.0f, 1.0f));
            add_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(random_direction, random_direction,
                scale * FUN_002132a8(6.5f, 10.0f) * D_0015ED6C);
            add_vector_xyz(random_direction, random_direction, &work.velocity);
            lo = scale_game_frames(60); hi = scale_game_frames(90);
            duration = random_int_between(lo, hi);
            FUN_L13_0030c138(&work.pos, random_direction, duration, 1);
        } while (i >= 0);
    }
    work.random_vector.q = 0;
    work.random_vector.f[0] = FUN_002132a8(-1.0f, 1.0f);
    work.random_vector.f[1] = FUN_002132a8(-1.0f, 1.0f);
    work.random_vector.f[2] = FUN_002132a8(-1.0f, 1.0f);
    work.direction.f[2] += distance * 0.5f;
    work.random_direction.q = work.random_vector.q;
    fast_vec_normalize(&work.random_direction, &work.random_direction, (distance / 5.0f) * D_0015ED6C);
    fast_vec_normalize(&work.direction, &work.direction, twice_distance * D_0015ED6C);
    add_vector_xyz(&work.random_direction, &work.random_direction, &work.direction);
    clamp_vector_length(&work.random_direction, &work.random_direction, D_0015ED6C * 10.0f);
    {
        int lo = scale_game_frames(60);
        int hi = scale_game_frames(90);
        duration = random_int_between(lo, hi);
    }
    FUN_L13_0030c138(&work.pos, &work.random_direction, duration, 1);
    count = distance < 6.0f ? FUN_001fa6d0(distance) + 1 : 4;
    extra = 0.0f;
    if (distance < 7.0f) extra = 7.0f - distance;
    if (count > 0) {
        int i = count;
        L13EffectColors colors __attribute__((aligned(16)));
        L13EffectColors second_colors __attribute__((aligned(16)));
        float magnitude = 400000.0f;
        float amount;
        do {
            int lo, hi, duration, duration2;
            i--;
            speed = (scale * FUN_002132a8(8.0f, 10.0f)) * D_0015ED6C - extra * D_0015ED6C;
            colors = D_L13_001F5560;
            second_colors = D_L13_001F5578;
            amount = scale * magnitude;
            color = &colors.v[random_integer_below(6)];
            second_color = &second_colors.v[random_integer_below(6)];
            {
                int lo = scale_game_frames(15);
                int hi = scale_game_frames(20);
                duration = random_int_between(lo, hi);
            }
            {
                int lo = scale_game_frames(25);
                int hi = scale_game_frames(30);
                duration2 = random_int_between(lo, hi);
            }
            FUN_L00_0026a9f0(&work.pos, &work.velocity, *color, *second_color,
                amount, duration, duration2, 0, 0, speed);
        } while (i != 0);
    }
    if (distance > 9.0f) {
        float amount;
        amount = scale * 4.0f;
        duration = scale_game_frames(15);
        FUN_L00_002df808(moby, &work.pos, &work.velocity, amount, duration, 127, 127, 127, 32);
        duration = scale_game_frames(24);
        FUN_L00_002df808(moby, &work.pos, &work.velocity, amount, duration, 32, 127, 0, 32);
    }
    {
        float amount;
        amount = scale * 4.0f;
        duration = scale_game_frames(20);
        FUN_L00_002df808(moby, &work.pos, &work.velocity, amount, duration, 63, 127, 0, 48);
    }
    {
        float amount;
        amount = scale * 3.5f;
        duration = scale_game_frames(27);
        FUN_L00_002df808(moby, &work.pos, &work.velocity, amount, duration, 16, 96, 0, 64);
    }
    {
        float amount;
        amount = scale * 3.0f;
        duration = scale_game_frames(29);
        FUN_L00_002df808(moby, &work.pos, &work.velocity, amount, duration, 0, 32, 0, 32);
    }
}
#endif
struct BlastHit {
    u8 pad0[0x2C];
    f32 damage;
};

struct BlastVars {
    u8 pad0[0x20];
    f32 health;
    u8 pad24[2];
    s16 effect_timer;
    u8 pad28;
    u8 hit;
    u8 pad2A[0x36];
    u8 effects[7];
    u8 effect_level;
    u8 pad68[8];
    int timer;
};

char D_L13_001F5590[] = "Moby class %d UID:%d  Damage Received: %f\n";
extern char D_L13_00178200[];
extern int D_L13_00161E2C __attribute__((sda));
extern f32 D_L13_00161E30 __attribute__((sda));
extern f32 D_L13_00161E34 __attribute__((sda));
extern f32 D_L13_00161E38 __attribute__((sda));
extern struct BlastHit *find_moby_hit_7b10(void *, int, int) __asm__("FUN_L00_0025a420");
extern int fptodp_7b10(float) __asm__("FUN_00120478");
extern void debug_print_7b10() __asm__("FUN_001e93b0");
extern int hit_check_7b10(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void stop_moby_sound_7b10(void *, int, int) __asm__("FUN_L00_00257470");
extern void anim_reset_7b10(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_7b10(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L13_003073c8(struct Moby *, void *);
extern void FUN_L10_002e3de8(struct Moby *);
extern int allocate_voice_7b10(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_7b10(int) __asm__("FUN_001f96f8");
extern float random_float_7b10(float, float) __asm__("FUN_002132a8");
extern float int_to_float_7b10(int) __asm__("FUN_001fa6c0");
extern float FUN_L00_0025b6a8(float, float, float);
extern int collect_mobies_7b10(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void blast_mobys_7b10(struct Moby *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern float scale_time_7b10(float) __asm__("FUN_001f96b0");
extern void remove_7b10(struct Moby *) __asm__("FUN_0020c828");

void poison_vessel_update(struct Moby *moby) __asm__("FUN_L13_00307b10");
void poison_vessel_update(struct Moby *moby) {
    struct BlastVars *vars = (struct BlastVars *)moby->pvars;
    struct BlastHit *hit;
    float *health;
    void *effects;
    int kind;
    float pos[4];
    int hits;
    int timer;

    switch (moby->state) {
    case 0:
        vars->hit = 0;
        moby->rot.x *= 0.7f;
        moby->rot.y *= 0.7f;
        hit = find_moby_hit_7b10(moby, 0x330001, 0);
        health = &vars->health;
        if (hit != 0) {
            debug_print_7b10(D_L13_001F5590, moby->oclass, (s16)moby->save_id, fptodp_7b10(hit->damage));
        }
        switch (hit_check_7b10(moby, hit, health, 0, &kind, 0, 0, 4)) {
        case 1:
        case 2:
            *health = 0;
            break;
        case 0:
        case 3:
        case 11:
            break;
        }
        effects = vars->effects;
        if (kind >= 2) {
            if (*health <= hit->damage) {
                *health = 0;
                moby->flags &= 0xEFFF;
                stop_moby_sound_7b10(moby, 0, -1);
                vars->effect_level = 0x78;
                anim_reset_7b10(moby, effects);
                vars->timer = 0;
                FUN_L13_003073c8(moby, vars);
                FUN_L10_002e3de8(moby);
                allocate_voice_7b10(0, 0, (int)moby);
                moby->state = 1;
            } else {
                *health = *health - hit->damage;
                vars->effect_level = 0xFA;
                vars->effect_timer = scale_game_frames_7b10(0x3C);
                anim_reset_7b10(moby, effects);
                moby->rot.x = random_float_7b10(-0.034906585f, 0.034906585f);
                moby->rot.y = random_float_7b10(-0.034906585f, 0.034906585f);
            }
        }
        moby->hit_slot = 0xFF;
        anim_update_7b10(moby, effects);
        break;
    case 1:
        qcopy(pos, &moby->pos);
        pos[2] += 1.0f;
        hits = collect_mobies_7b10(FUN_L00_0025b6a8(D_L13_00161E30, D_L13_00161E34,
                                              int_to_float_7b10(vars->timer) /
                                                  int_to_float_7b10(scale_game_frames_7b10(D_L13_00161E2C))),
                                    pos, 0x10, moby, 0);
        blast_mobys_7b10(moby, pos, D_L13_00178200, hits, 0, 0x830001, 2, 1, D_L13_00161E38, 1.0f, 1.0f);
        if (vars->timer > scale_time_7b10((float)D_L13_00161E2C * 0.5f)) {
            moby->flags |= 0x41;
            FUN_L10_002e3de8(moby);
        } else {
            moby->alpha = moby->alpha * 7 / 8;
        }
        timer = vars->timer + 1;
        vars->timer = timer;
        if (scale_game_frames_7b10(D_L13_00161E2C) < timer) {
            FUN_L10_002e3de8(moby);
            remove_7b10(moby);
        }
        break;
    }
}
/* Update function of moby class 1262 (blarg space commando): turn, walk its path, shoot, die. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00309330), where it is exact; names translated to the US level program. */


typedef struct {
    u8 pad0[0x18];
    u8 active;
    u8 valid;
    s16 oclass;
    u8 pad1C[0x14];
} Shot_309330;

typedef struct {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 pad10[0x14];
    f32 unk24;
    u8 pad28[0x18];
} Ctl_309330;

typedef struct {
    u8 pad0[0x20];
    f32 unk20;
    s16 unk24;
    u8 pad26[2];
    u8 unk28;
    u8 pad29[7];
    f32 unk30;
    u8 pad34[0x24];
    u8 unk58;
    u8 pad59;
    u8 unk5A;
    u8 pad5B[5];
    Vec4 vel; /* 0x60 */
    u8 pad70[0x50];
    Ctl_309330 ctl; /* 0xC0 */
    u8 pad100[0x60];
    struct Moby *target; /* 0x160 */
    s32 unk164;
    u8 pad168[8];
    Vec4 home; /* 0x170 */
    u8 pad180[4];
    s32 unk184;
    f32 turn; /* 0x188 */
    u8 pad18C[8];
    s32 timer; /* 0x194 */
    s32 path; /* 0x198 */
    u8 pad19C[0xC];
    f32 unk1A8;
    u8 pad1AC[0x34];
    Vec4 unk1E0;
    u8 pad1F0[0x210];
    u8 unk400[1];
} Vars_309330;

extern char D_L13_001670C0[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_L13_00161E70;
extern int FUN_L00_00258b50(struct Moby *, void *, void *, void *, float);
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern s32 rand(void);
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_00258278(void *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_00259888_c2(void *, struct Moby *, int, float, void *) __asm__("FUN_L00_00259888");
extern void FUN_L00_00259d08(void *, void *, void *, void *, struct Moby *, Shot_309330 *, int);
extern void FUN_L00_0025a120(void *);
extern void approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00261d78(int, void *, void *, float);
extern void FUN_L00_00263ac8(float, struct Moby *, int, void *);
extern void FUN_L01_0026d930(void *);
extern void FUN_L13_00308d10_c(struct Moby *) __asm__("FUN_L13_00308d10");
extern void FUN_L13_00309370_c(char *m) __asm__("FUN_L13_00309370");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
extern void func_L00_0025A8C0_30A9B0(void *, void *, int, void *, float) __asm__("FUN_L13_00265990");
extern float D_L13_00161E58_gp __asm__("D_L13_00161E58") __attribute__((sda));
extern float D_L13_00161E74_gp __asm__("D_L13_00161E74") __attribute__((sda));
extern float D_L13_00161E78_gp __asm__("D_L13_00161E78") __attribute__((sda));
extern void func_L00_002592B0_309330(struct Moby *, float, float *, float, float, float) __asm__("FUN_L13_00263fc8");

void blarg_space_commando_update(struct Moby *moby) __asm__("FUN_L13_00307e98");
void blarg_space_commando_update(struct Moby *moby) {
    Vars_309330 *vars = (Vars_309330 *)moby->pvars;
    struct Moby *target;
    Vec4 point;
    Vec4 origin;
    Vec4 direction;
    Shot_309330 shot;
    s32 blocked;
    s32 path_blocked;
    s32 hit;

    FUN_L13_00308d10_c(moby);
    target = vars->target;
    FUN_L00_00263ac8(2.5f, moby, 1, vars->unk400);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L13_001670C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x16;
    }
    switch (moby->state) {
    case 0:
        vars->unk20 = 2.0f;
        vars->unk28 = 1;
        vars->unk58 = 12;
        vars->unk5A = 8;
        vars->unk24 = 2;
        vars->unk30 = 1.5f;
        FUN_L01_0026d930(&vars->ctl);
        vars->ctl.unk8 = 2.0f;
        vars->ctl.unkC = 0.5f;
        vars->ctl.unk0 = func_001FA898_r(D_L13_00161E58_gp * 716.8f);
        vars->ctl.unk4 = D_L13_00161E58_gp * 0.7f;
        vars->ctl.unk24 = D_L13_00161E78_gp * frame_time * random_float_between(0.95f, 1.05f);
        qcopy(&vars->home, &moby->pos);
        FUN_L00_0024f7c8(moby, 0, &vars->unk1E0);
        moby->state = 1;
        break;
    case 1:
        approach_angle(&moby->rot.z, fast_arc_tan_c(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                          &vars->turn, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f, frame_time * 3.1415927f);
        if (vars->unk164 != 2) {
            f32 dist = vector_distance_xy_alt(&moby->pos, &target->pos);
            if (D_L13_00161E74_gp < dist) {
                if (fast_difference_between_rotations(moby->rot.z, fast_arc_tan_c(target->pos.x - moby->pos.x,
                                                                 target->pos.y - moby->pos.y)) < 0.2617994f) {
                    moby->state = 2;
                    if (moby->prev_seq != 3) {
                        blend_moby_animation(moby, 3, 0, scale_game_frames(10));
                    }
                } else {
                    moby->state = 7;
                    if (moby->prev_seq != 1) {
                        blend_moby_animation(moby, 1, 0, scale_game_frames(20));
                    }
                }
            } else {
                moby->state = 5;
                if (moby->prev_seq != 6) {
                    blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                }
            }
        } else if (vars->unk184 != 0) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case 2:
        if (moby->anim_flags & 2) {
            moby->state = 3;
            FUN_L13_00309370_c((char *)moby);
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
        }
        break;
    case 3:
        if (moby->anim_flags & 2) {
            f32 dist = vector_distance_xy_alt(&moby->pos, &target->pos);
            if (vars->unk164 != 2 && dist < 15.0f) {
                moby->state = 4;
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, scale_game_frames(20));
                }
                vars->timer = scale_game_frames(D_L13_00161E70);
            } else {
                moby->state = 1;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(20));
                }
            }
        }
        break;
    case 4:
        approach_angle(&moby->rot.z, fast_arc_tan_c(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                          &vars->turn, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f, frame_time * 3.1415927f);
        if (tick_countdown_32_alt(&vars->timer) != 0) {
            moby->state = 3;
            FUN_L13_00309370_c((char *)moby);
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            }
        } else if (vector_distance_xy_alt(&moby->pos, &target->pos) < D_L13_00161E74_gp) {
            FUN_L00_0024f7c8(moby, 0, &vars->unk1E0);
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vars->unk184 != 0) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case 5:
        approach_angle(&moby->rot.z, fast_arc_tan_c(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                          &vars->turn, frame_time_sq * 6.2831855f, frame_time_sq * 6.2831855f, frame_time * 3.1415927f);
        FUN_L00_0024f7c8(moby, 0, &point);
        qcopy(&origin, &moby->pos);
        origin.f[2] += 0.3f;
        {
        f32 frame = compute_interpolated_record_value(moby);
        if (frame >= 9.0f && frame <= 15.0f) {
            direction.f[0] = fast_cos(moby->rot.z);
            direction.f[1] = fast_sin(moby->rot.z);
            direction.f[2] = 1.0f;
            direction.f[3] = 5627.925f;
            func_L00_0025A8C0_30A9B0(&shot, moby, 1, &direction, 1.0f);
            shot.active = 0;
            shot.valid = 1;
            shot.oclass = moby->oclass;
            FUN_L00_00259d08(&point, &origin, &vars->unk1E0, &origin, moby, &shot, 5);
        }
        }
        qcopy(&vars->unk1E0, &point);
        if (moby->anim_flags & 2) {
            moby->state = 6;
            moby->command = 1;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        }
        break;
    case 6:
        if (moby->anim_flags & 2) {
            moby->state = moby->command;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case 7:
        func_L00_002592B0_309330(moby,
                          fast_add_rotations_c(fast_arc_tan_c(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                                        vars->unk1A8), &vars->turn,
                          0.05f, 0.3f, frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        if (FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f) & 2) {
            rand();
            vars->unk1A8 = convert_integer_to_float(-1) * 1.0995574f;
        } else {
            vars->unk1A8 = 0.0f;
        }
        FUN_L00_00261d78(vars->path, &moby->pos, &moby->pos, 0.75f);
        path_blocked = 0;
        if (FUN_L00_00261b48(vars->path, &moby->pos, &target->pos, &direction) != 0 &&
            fast_vec_dist(&moby->pos, &direction) < 0.8f) {
            path_blocked = 1;
        }
        {
        f32 dist = vector_distance_xy_alt(&moby->pos, &target->pos);
        if (dist < 15.0f) {
            moby->state = 2;
            if (moby->prev_seq != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
        } else if (path_blocked != 0) {
            moby->state = 2;
            if (moby->prev_seq != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            }
        } else if (dist < D_L13_00161E74_gp) {
            moby->state = 5;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        }
        break;
    case 11:
        blocked = 0;
        func_L00_002592B0_309330(moby,
                          fast_arc_tan_c(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn, 0.05f, 0.3f,
                          frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f);
        FUN_L00_00261d78(vars->path, &moby->pos, &moby->pos, 0.75f);
        if (FUN_L00_00261b48(vars->path, &moby->pos, &target->pos, &direction) != 0 &&
            fast_vec_dist(&moby->pos, &direction) < 0.8f) {
            blocked = 1;
        }
        if (vars->unk184 == 0) {
            moby->state = 13;
        } else if (blocked != 0) {
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        } else if (vector_distance_xy_alt(&moby->pos, &target->pos) < D_L13_00161E74_gp) {
            FUN_L00_0024f7c8(moby, 0, &vars->unk1E0);
            moby->state = 12;
            if (moby->prev_seq != 6) {
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            }
        }
        break;
    case 12:
        FUN_L00_0024f7c8(moby, 0, &point);
        qcopy(&origin, &moby->pos);
        origin.f[2] += 1.5f;
        {
        f32 frame = compute_interpolated_record_value(moby);
        if (frame >= 9.0f && frame <= 15.0f) {
            direction.f[0] = fast_cos(moby->rot.z);
            direction.f[1] = fast_sin(moby->rot.z);
            direction.f[2] = 1.0f;
            direction.f[3] = 5627.925f;
            func_L00_0025A8C0_30A9B0(&shot, moby, 1, &direction, 1.0f);
            shot.active = 0;
            shot.valid = 1;
            shot.oclass = moby->oclass;
            FUN_L00_00259d08(&point, &origin, &vars->unk1E0, &origin, moby, &shot, 5);
        }
        }
        qcopy(&vars->unk1E0, &point);
        if (moby->anim_flags & 2) {
            moby->state = 11;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
        }
        break;
    case 13:
        func_L00_002592B0_309330(moby,
                          fast_arc_tan_c(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y), &vars->turn, 0.05f,
                          0.3f, frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        if (fast_difference_between_rotations(moby->rot.z, fast_arc_tan_c(vars->home.f[0] - moby->pos.x,
                                                         vars->home.f[1] - moby->pos.y)) < 0.7853982f) {
            FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f);
            FUN_L00_00261d78(vars->path, &moby->pos, &moby->pos, 0.75f);
        }
        if (fast_vec_dist(&vars->home, &moby->pos) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case 15:
        hit = mine_fall(moby, &vars->vel);
        if (hit & 0x40) {
            qzero(&vars->vel);
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        } else if (hit & 0x120) {
            mark_moby_for_removal_c2(moby);
        }
        break;
    case 16:
        if (mine_fall(moby, &vars->vel) & 0x160) {
            FUN_L00_0025e450_c(moby, &vars->vel, 0, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c2(moby);
        }
        break;
    }
}
#include "rnc/overlay/collision.h"

/* Vars of the level 13 walker FUN_L13_00308d10 drives. */
struct WalkerVars_8d10 {
    u8 pad0[0x20];
    f32 health;                    /* 0x20 */
    u8 pad24[2];
    s16 stun_timer;                /* 0x26 */
    u8 pad28[0x10];
    s32 init;                      /* 0x38: nonzero until the idle timer is first drawn */
    u8 pad3C[0x24];
    u8 anim[8];                    /* 0x60 */
    f32 fall_speed;                /* 0x68 */
    u8 pad6C[0xC];
    f32 knock_up;                  /* 0x78 */
    f32 knock_back;                /* 0x7C */
    s32 hit_sound;                 /* 0x80 */
    u8 pad84[4];
    f32 hit_scale;                 /* 0x88 */
    u8 pad8C[0x24];
    f32 knock_min;                 /* 0xB0 */
    f32 knock_max;                 /* 0xB4 */
    u8 padB8[0x58];
    u8 flash[7];                   /* 0x110 */
    u8 flash_time;                 /* 0x117 */
    u8 pad118[8];
    Vec4f target;                  /* 0x120 */
    u8 pad130[0x30];
    s32 start_frame;               /* 0x160 */
    s32 mode;                      /* 0x164 */
    u8 pad168[8];
    Vec4f home;                    /* 0x170 */
    s32 path;                      /* 0x180 */
    s32 idle_timer;                /* 0x184 */
    u8 pad188[8];
    f32 range;                     /* 0x190 */
    u8 pad194[8];
    f32 near_range;                /* 0x19C */
    f32 far_range;                 /* 0x1A0 */
    s32 target_moby;               /* 0x1A4: -1: none */
};

extern f32 D_L13_00161E58 __attribute__((sda));
extern f32 D_L13_00161E60 __attribute__((sda));
extern f32 D_L13_00161E64 __attribute__((sda));
extern f32 D_L13_00161E68 __attribute__((sda));
extern f32 D_L13_00161E6C __attribute__((sda));
extern s32 *walker_paths_8d10[] __asm__("D_L13_001B07B0");
extern char D_L13_00160680[] __attribute__((section(".sdata")));
extern s32 D_001413D0_lui[] __asm__("D_001413D0") __attribute__((section(".data")));
extern CollisionHit coll_hit_8d10 __asm__("D_L13_00174140");
extern void FUN_L13_003094f8(struct Moby *);
extern f32 random_float_between_8d10(f32, f32) __asm__("FUN_002132a8");
extern f32 FUN_001f96b0_8d10(f32) __asm__("FUN_001f96b0");
extern s32 float_to_int_8d10(f32) __asm__("FUN_001fa6d0");
extern s32 tick_countdown_32_8d10(s32 *) __asm__("FUN_001f9740");
extern s32 scale_game_frames_8d10(s32) __asm__("FUN_001f96f8");
extern char *find_moby_hit_8d10(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 hit_check_8d10(struct Moby *, char *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern f32 atan2_f_8d10(f32, f32) __asm__("FUN_001f9e90");
extern void knock_back_8d10(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void stop_moby_sound_8d10(struct Moby *, s32, s32) __asm__("FUN_L00_00257470");
extern void anim_reset_8d10(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_8d10(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern s32 find_target_8d10(f32, struct Moby *, void *, s32, s32, s32 *, s32) __asm__("FUN_L00_0025ff38");
extern f32 vector_distance_8d10(void *, void *) __asm__("FUN_001f9b80");
extern f32 absolute_float_8d10(f32) __asm__("FUN_001f99c0");
extern s32 FUN_00214720_8d10(void *, s32) __asm__("FUN_00214720");
extern f32 probe_ground_height_8d10(void *, s32, f32) __asm__("FUN_00213508");
extern s32 ground_material_8d10(void) __asm__("FUN_001f0b58");
extern f32 vector_length_xy_8d10(void *) __asm__("FUN_001f9b20");
extern void normalize_vector_xyz_8d10(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void add_vector_xyz_8d10(void *, void *, void *) __asm__("FUN_001f9a10");
extern void mark_moby_for_removal_8d10(struct Moby *) __asm__("FUN_0020c828");

/* Walker update: takes hits (knocked back, or dies at no health), picks its target and falls onto or slides off the ground. */
void FUN_L13_00308d10(struct Moby *moby) {
    enum {
        HIT_RESULT_DEATH = 1
    };
    struct WalkerVars_8d10 *vars = (struct WalkerVars_8d10 *)moby->pvars;
    char *hit;
    s32 code;
    s32 kind;
    f32 damage;
    f32 ground;
    s32 *path;
    Vec4f push;

    moby->scale = moby->pclass->scale * D_L13_00161E58;
    FUN_L13_003094f8(moby);
    if (vars->init != 0) {
        vars->idle_timer = float_to_int_8d10(FUN_001f96b0_8d10(random_float_between_8d10(180.0f, 240.0f)));
        vars->init = 0;
    }
    tick_countdown_32_8d10(&vars->idle_timer);
    damage = 0.0f;
    hit = find_moby_hit_8d10(moby, 0x330000, 0);
    code = hit_check_8d10(moby, hit, &vars->health, 0, &kind, &damage, 0, 4);
    if (kind != 1 && moby->state != 0x10) {
        if (hit != 0 && *(struct Moby **)(hit + 0x20) != 0 && (*(struct Moby **)(hit + 0x20))->oclass == 0x4EE) {
            damage = 0.0f;
        }
        vars->health -= damage;
        if (vars->health <= 0.0f) {
            code = 1;
        }
        vars->hit_sound = 0x2CD;
        vars->hit_scale = 0.7f;
        switch (code) {
        case 0:
        case 11:
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            moby->state = 0xF;
            vars->stun_timer = scale_game_frames_8d10(0x3C);
            vars->knock_up = D_L13_00161E60 * frame_time;
            vars->knock_back = D_L13_00161E64 * frame_time;
            vars->knock_min = 8.0f;
            vars->knock_max = 15.0f;
            knock_back_8d10(moby, vars->anim, 7, 1, 0, atan2_f_8d10(*(f32 *)(hit + 0x10), *(f32 *)(hit + 0x14)));
            vars->flash_time = 0xFA;
            break;
        case HIT_RESULT_DEATH:
        case 2:
            moby->state = 0x10;
            moby->flags &= ~0x1000;
            vars->knock_up = D_L13_00161E68 * frame_time;
            vars->knock_back = D_L13_00161E6C * frame_time;
            vars->knock_min = 9.0f;
            vars->knock_max = 17.0f;
            knock_back_8d10(moby, vars->anim, 8, 1, 0, atan2_f_8d10(*(f32 *)(hit + 0x10), *(f32 *)(hit + 0x14)));
            vars->flash_time = 0xFA;
            stop_moby_sound_8d10(moby, 0, -1);
            break;
        }
        anim_reset_8d10(moby, vars->flash);
    }
    moby->hit_slot = 0xFF;
    anim_update_8d10(moby, vars->flash);
    if (vars->idle_timer != 0) {
        vars->range = vars->far_range;
    } else {
        vars->range = vars->near_range;
    }
    path = walker_paths_8d10[vars->path];
    if (find_target_8d10(vars->range, moby, &vars->target, 0, 0, path + 4, *path) != 2) {
        if (vars->range < vector_distance_8d10(&vars->home, &vars->target)
            || absolute_float_8d10(moby->pos.z - vars->target.z) > 3.0f) {
            vars->mode = 2;
        }
        if (vars->target_moby != -1 && FUN_00214720_8d10(&vars->target, vars->target_moby) == 0) {
            vars->mode = 2;
        } else {
            vars->target_moby = -1;
        }
    }
    if (vars->start_frame == 0) {
        vars->start_frame = (*(int *)&D_001413D0_lui);
    }
    ground = probe_ground_height_8d10(&moby->pos, 0, 0.5f);
    if (ground != 0.0f) {
        switch (ground_material_8d10()) {
        case 0:
        case 1:
        case 3:
        case 11:
        case 13:
            moby->flags &= ~0x1000;
            stop_moby_sound_8d10(moby, 0, -1);
            spawn_explosion(moby, D_L13_00160680, 0, 0.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_8d10(moby);
            return;
        default:
            if (moby->state < 0xF) {
                if (atan2_f_8d10(coll_hit_8d10.normal_z, vector_length_xy_8d10(&coll_hit_8d10.normal_x)) > 0.6981317f) {
                    normalize_vector_xyz_8d10(&push, &coll_hit_8d10.normal_x, 0.05f);
                    add_vector_xyz_8d10(&moby->pos, &moby->pos, &push);
                }
                if (ground < moby->pos.z) {
                    vars->fall_speed -= frame_time_sq * 9.8f;
                    moby->pos.z += vars->fall_speed;
                    if (moby->pos.z < ground) {
                        vars->fall_speed = 0.0f;
                        moby->pos.z = ground;
                    }
                } else if (moby->pos.z < ground) {
                    moby->pos.z += 0.05f;
                }
            }
            break;
        }
    } else {
        if (moby->state < 0xF) {
            vars->fall_speed -= frame_time_sq * 9.8f;
            moby->pos.z += vars->fall_speed;
        }
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f
        || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal_8d10(moby);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_0030A828), where it is exact; names translated to the US level program. */

extern char D_L13_00174180[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern float D_L13_00161E80 __attribute__((sda));
int D_L13_00161E84 = 150;
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);

void FUN_L13_00309370(struct Moby *m) {
    char *d = (char *)m->pvars;
    float v[4];
    int i;
    probe_ground_height(&m->pos, 0, 0.5f);
    normalize_vector_xyz(d + 0x200, D_L13_00174180, 1.0f);
    moby_point_position(m, 0, d + 0x1F0);
    for (i = 0; i < 16; i++) {
        float a = (convert_integer_to_float(i) * 0.0625f - 0.5f) *
                  (D_L13_00161E80 * DEG_TO_RAD);
        v[0] = fast_cos(fast_add_rotations(a, m->rot.z)) * 0.5f;
        v[1] = fast_sin(fast_add_rotations(a, m->rot.z)) * 0.5f;
        v[2] = 0.0f;
        add_vector_xyz(d + 0x210 + i * 16, d + 0x1F0, v);
        if (i == 0 || i == 15) {
            *(int *)(d + 0x21C + i * 16) = 0;
        } else {
            *(float *)(d + 0x21C + i * 16) = 1.0f;
        }
    }
    *(float *)(d + 0x1FC) = scale_game_frames(D_L13_00161E84);
}
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern float dot_vector_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void make_hit_descriptor(void *, struct Moby *, int, void *, float) __asm__("FUN_L00_00259888");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L13_00309bc8(struct Moby *);
extern void FUN_L13_003098d0(struct Moby *);
extern float D_L13_00161E7C __attribute__((sda));
extern struct { char pad[0x28]; float height; } D_L13_00174140;
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) TrailPoint_94f8;
typedef struct {
    TrailPoint_94f8 vector;
    float p, q;
    unsigned char a, b;
    unsigned short c;
} __attribute__((aligned(16))) TrailHit_94f8;
typedef struct {
    char pad[0x1f0];
    TrailPoint_94f8 anchor;
    TrailPoint_94f8 direction;
    TrailPoint_94f8 point[17];
} TrailVars_94f8;

void FUN_L13_003094f8(struct Moby *moby) {
    TrailVars_94f8 *data = *(TrailVars_94f8 **)&moby->pvars;
    TrailPoint_94f8 next;
    union {
        struct {
            TrailPoint_94f8 delta;
            TrailPoint_94f8 scaled;
        } vectors;
        TrailHit_94f8 hit;
    } work;
    TrailPoint_94f8 upper, lower;
    int timer, i, j, clamp_count;
    char *clamp_cursor;
    float prior_height, dot;

    timer = func_001FA898_r(data->anchor.w);
    fast_dec_timer(&timer);
    data->anchor.w = (float)timer;
    if (!timer) return;

    if (timer == 20) {
        for (clamp_cursor = (char *)data, clamp_count = 15; clamp_count >= 0;
             clamp_count--, clamp_cursor += 16) {
            if (*(float *)(clamp_cursor + 0x21c) >= 1.0f)
                *(float *)(clamp_cursor + 0x21c) = 0.98f;
        }
    }

    i = 15;
    do {
        subtract_vector_xyz(&next, &data->point[15 - i], &data->anchor);
        next.z = 0.0f;
        normalize_vector_xyz(&next, &next, D_L13_00161E7C * frame_time);
        add_vector_xyz(&next, &data->point[15 - i], &next);
        subtract_vector_xyz(&work.vectors.delta, &next, &moby->pos);
        dot = dot_vector_xyz(&work.vectors.delta, &data->direction);
        normalize_vector_xyz(&work.vectors.scaled, &data->direction, -dot + 0.35f);
        add_vector_xyz(&work.vectors.delta, &work.vectors.delta, &work.vectors.scaled);
        prior_height = work.vectors.scaled.z;
        qcopy(&upper, &data->point[15 - i]);
        upper.z = upper.z + 1.0f;
        qcopy(&lower, &data->point[15 - i]);
        lower.z = lower.z - 3.0f;
        if (collision_line(&upper, &lower, 2, 0, 0) != 0 && prior_height < next.z) {
            approach_value(&next.z, D_L13_00174140.height + 0.35f,
                           frame_time * 4.0f);
        }
        if (1.0f <= data->point[15 - i].w &&
            collision_line(&data->point[15 - i], &next, 2, 0, 0)) {
            next.w = 0.98f;
        } else {
            if (next.w < 1.0f)
                approach_value(&next.w, 0.0f, frame_time + frame_time);
        }
        qcopy(&data->point[15 - i], &next);
        i--;
    } while (i >= 0);

    j = 0;
    do {
        if (data->point[j].w >= 0.98f && data->point[j + 1].w >= 0.98f) {
            next.x = fast_cos(moby->rot.z);
            next.y = fast_sin(moby->rot.z);
            next.z = 1.0f;
            next.w = 5627.925f;
            make_hit_descriptor(&work.hit, moby, 0x10001, &next, 1.0f);
            work.hit.c = (unsigned short)moby->oclass;
            work.hit.a = 0;
            work.hit.b = 1;
            collision_line(&data->point[j], &data->point[j + 1], 0, moby, &work.hit);
        }
        j++;
    } while (j < 15);
    enqueue_callback_list_1((void *)FUN_L13_003098d0, moby);
    FUN_L13_00309bc8(moby);
}
/* Drives the grid of 22 tiles: sets them up on the first frame, then moves the bobbing platform. */

typedef struct {
    float x;
    float y;
    float z;
    char pad0C[0x10];
    unsigned char c1C;
    unsigned char c1D;
    unsigned short c1E;
    char rest[0x1170];
} Tile_9e88;

typedef struct {
    float top;
    float bottom;
    float angle;
    float speed;
    int idx;
} Bob_9e88;

typedef struct {
    float f00, f04, f08, f0C, f10, f14, f18, f1C, f20, f24;
    int i28, i2C;
    float f30, f34, f38;
    unsigned char c3C, c3D;
} Light_9e88;

extern Tile_9e88 Tiles_9e88[] __asm__("D_L13_001D9BC0");
extern Light_9e88 D_L13_001CAB80;
extern float D_L13_001801C0[];
extern unsigned short D_L13_001F1E20[];
extern char D_L13_001F1E80[];
extern int D_L13_00161208;
extern float D_L13_0016120C;
extern float D_L13_00161210;
extern float D_L13_00161214;
extern void *D_L13_001612D0;
extern void *D_L13_001612D4;
extern int D_L13_001612D8;
int D_L13_00161EF0 = -1;
extern unsigned short D_L13_00161EF4 __attribute__((sda));
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float rand_range_9e88(float, float) __asm__("FUN_002132a8");
extern int random_below_9e88(int) __asm__("FUN_00213260");
extern void pool_init_tiles(void *, int) __asm__("FUN_L01_002b7a48");
extern void pool_bind_tile(void *, void *) __asm__("FUN_L01_002b7c68");
extern void pool_set_scale(float) __asm__("FUN_L01_002b7ef0");
extern void pool_flush(void *, int) __asm__("FUN_L01_002b7fe0");
extern void pool_ripple(float, float, float, float, void *, int) __asm__("FUN_L01_002b8288");
extern void pool_splash(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");
extern int FUN_L01_00275690(float, void *);
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L13_0030a460(void);
extern void FUN_L13_0030a488(int);

void FUN_L13_00309e88(struct Moby *moby) {
    Bob_9e88 *b;
    float f;
    int i, j, k, n;
    int cr, cg;

    struct Hero *g = &hero;
    char *p = ((char *)g->ship_moby);
    if (p != 0 && *(short *)(p + 0xA6) == 0x45 && (unsigned char)p[0x20] != 0xFE &&
        (unsigned char)p[0x20] != 0xFD && g->state.current == 0x32)
        return;
    b = (Bob_9e88 *)moby->pvars;
    if (b == 0)
        return;
    switch (moby->state) {
    case 0: {
        float rate = frame_time;
        moby->flags |= 1;
        b->angle = b->angle * DEG_TO_RAD;
        b->speed = b->speed * DEG_TO_RAD * rate;
        if (b->idx == 0) {
            Light_9e88 *l;
            char *tb;
            float a;
            pool_init_tiles(Tiles_9e88, 22);
            pool_set_scale(1.0f);
            moby->update_dist = 0xFF;
            l = &D_L13_001CAB80;
            l->f00 = 16.0f;
            l->f04 = 16.0f;
            l->f08 = -8.0f;
            l->f0C = -8.0f;
            l->f10 = 1.0f;
            l->f14 = 1.0f;
            l->f18 = 0.9f;
            l->f20 = 0.1f;
            a = fast_arc_tan(D_L13_001801C0[4], D_L13_001801C0[5]);
            l->f30 = fast_cos(a) * 0.57735f;
            l->f34 = fast_sin(a) * 0.57735f;
            l->f38 = -0.57735f;
            l->i28 = 0x2D;
            l->i2C = 0x2E;
            l->c3C = 0x60;
            l->c3D = 0x40;
            tb = D_L13_001F1E80;
            D_L13_001612D8 = 22;
            D_L13_0016120C = 32768.0f;
            D_L13_00161210 = 255.0f;
            D_L13_00161214 = 48.0f;
            D_L13_001612D0 = Tiles_9e88;
            D_L13_001612D4 = tb;
            D_L13_00161208 = 0;
            for (i = 0; i < 22; i++) {
                pool_bind_tile(&Tiles_9e88[i], tb + i * 16);
                Tiles_9e88[b->idx].c1E = D_L13_001F1E20[b->idx];
            }
            for (k = 0; k < 22; k++) {
                for (j = 0; j < 2; j++) {
                    Tile_9e88 *t = &Tiles_9e88[k];
                    float x = t->x + rand_range_9e88(-6.0f, 6.0f);
                    float y = t->y + rand_range_9e88(-6.0f, 6.0f);
                    pool_ripple(x, y, 2.0f, 0.2f, Tiles_9e88, 22);
                }
            }
            for (n = 0; n < 22; n++)
                FUN_L13_0030a488(n);
        }
        moby->draw_dist = 0;
        moby->state = 1;
        break;
    }
    case 1: {
        struct Hero *h;
        f = b->top;
        f -= b->bottom;
        f *= 0.5f;
        moby->pos.z = b->bottom + (fast_cos(b->angle) * f + f);
        h = &hero;
        if (h->state.control_mode != 0x14 && h->state.control_mode != 0x19 &&
            h->state.current != 0x3D && h->state.current != 0x7B &&
            h->state.current != 0x7C)
            b->angle = fast_add_rotations(b->angle, b->speed);
        Tiles_9e88[b->idx].c1C = D_L13_001CAB80.c3C;
        Tiles_9e88[b->idx].c1D = D_L13_001CAB80.c3D;
        if (random_below_9e88(200) == 0) {
            float x = Tiles_9e88[b->idx].x + rand_range_9e88(-4.0f, 4.0f);
            float y = rand_range_9e88(-4.0f, 4.0f);
            Tile_9e88 *t = &Tiles_9e88[b->idx];
            pool_splash(x, t->y + y, 1.0f, -0.05f, t, 1, 1);
        }
        break;
    }
    }
    if (b->idx >= 0) {
        f = 0.0f;
        (Tiles_9e88 + b->idx)->z = moby->pos.z;
        if (FUN_L01_00275690(48.0f, moby) != -1)
            f = 1.0f;
        if (f == 0.0f)
            Tiles_9e88[b->idx].c1E = 0;
        else
            Tiles_9e88[b->idx].c1E = D_L13_001F1E20[b->idx];
        if (b->idx == 0) {
            pool_flush(Tiles_9e88, 22);
            enqueue_callback_list_1(FUN_L13_0030a460, moby);
        }
        if (b->idx == D_L13_00161EF0) {
            Tiles_9e88[b->idx].c1E = D_L13_00161EF4;
            D_L13_001F1E20[b->idx] = D_L13_00161EF4;
        }
    }
}
/* Level overlay code after unclassified; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */

typedef struct {
    float x;
    float y;
    int pad;
    unsigned char n[0x14];
    char rest[0x1170];
} Ent;

extern Ent D_L13_001D9BC0[];
float absolute_float(float input) __asm__("func_001F99C0");

void FUN_L13_0030a488(int arg) {
    int i;
    D_L13_001D9BC0[arg].n[0] = 0xFF;
    D_L13_001D9BC0[arg].n[2] = 0xFF;
    D_L13_001D9BC0[arg].n[1] = 0xFF;
    D_L13_001D9BC0[arg].n[3] = 0xFF;
    D_L13_001D9BC0[arg].n[4] = 0xFF;
    D_L13_001D9BC0[arg].n[5] = 0xFF;
    D_L13_001D9BC0[arg].n[6] = 0xFF;
    D_L13_001D9BC0[arg].n[7] = 0xFF;
    for (i = 0; i < 22; i++) {
        if (arg != i) {
            Ent *t = D_L13_001D9BC0;
            Ent *e = &t[arg];
            float x = e->x - t[i].x;
            float y = e->y - t[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    e->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    e->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    e->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    e->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        e->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        e->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        e->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        e->n[6] = i;
                }
            }
        }
    }
}
typedef struct {
    u8 pad0[0x20];
    u8 path[0x40];
    f32 top;
    f32 bottom;
    f32 speed;
    s32 voice;
    f32 blend;
    f32 blend_speed;
    f32 pulse;
    f32 angle;
    u8 pad80[4];
    s32 timer;
} LiftVarsA6D8;

extern char D_L13_001670C0[];
extern float D_0015ED70;
extern float D_0015ED6C;
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int float_to_int(float) __asm__("FUN_001fa6d0");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int tick_timer(int *) __asm__("FUN_001f9740");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern float advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int allocate_voice(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18(int);
extern void FUN_L13_0030ad38(struct Moby *, float *, float *);

void FUN_L13_0030a6d8(struct Moby *moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_MOVE_TO_TOP = 2,
        LIFT_MOVE_TO_BOTTOM = 3
    };
    LiftVarsA6D8 *vars = (LiftVarsA6D8 *)moby->pvars;
    float delta[4];
    float rot[4];
    float a[4];
    float b[4];
    float c[4];

    scale_vector_xyz(delta, &moby->pos, -1.0f);
    qcopy(rot, &moby->rot);
    moby->rot.y = fast_add_rotations((1.0f - (moby->pos.z - vars->bottom) / (vars->top - vars->bottom)) * 3.1415927f, vars->angle * DEG_TO_RAD);
    switch (moby->state) {
    case LIFT_INITIALIZE:
        vars->pulse = random_angle_radians();
        vars->voice = -1;
        vars->timer = 0;
        moby->state = 4;
        break;
    case 1: {
        float old = vars->blend;
        advance_accelerated_scalar(&vars->blend, &vars->blend_speed, 1.0f, D_0015ED70 * 4.0f, D_0015ED70 * 4.0f, D_0015ED6C * 4.0f);
        if (vars->blend < 1.0f) {
            float t = (1.0f - vars->blend) / (1.0f - old);
            char *base;
            FUN_L13_0030ad38(moby, b, c);
            subtract_vector_xyz(a, D_L13_001670C0, b);
            base = D_L13_001670C0 - 0x140;
            scale_vector_xyz(a, a, t);
            add_vector_xyz(b, b, a);
            c[1] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x154), c[1]) * t, c[1]);
            c[2] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x158), c[2]) * t, c[2]);
            FUN_L00_002ea9d8(b);
            FUN_L00_002eaa30(c);
        } else {
            u8 s = moby->command;
            moby->command = 1;
            moby->state = s;
        }
        break;
    }
    case LIFT_MOVE_TO_TOP:
    case LIFT_MOVE_TO_BOTTOM: {
        float target;
        if (!voice_is_playing(moby, vars->voice))
            vars->voice = allocate_voice(0, 4, moby);
        if (moby->command != 0) {
            FUN_L13_0030ad38(moby, a, b);
            FUN_L00_002ea9d8(a);
            FUN_L00_002eaa30(b);
        }
        if (moby->state == 2)
            target = vars->top;
        else
            target = vars->bottom;
        advance_accelerated_scalar(&moby->pos.z, &vars->speed, target, D_0015ED70 * 8.0f, D_0015ED70 * 8.0f, D_0015ED6C * 40.0f);
        if (absolute_float(moby->pos.z - target) == 0.0f) {
            vars->timer = scale_game_frames(0x46);
            if (moby->state == 2)
                moby->state = 6;
            else
                moby->state = 7;
            if (moby->command != 0)
                FUN_L00_002eac18(2);
        }
        break;
    }
    case 6:
    case 7:
        if (vars->timer != 0 && tick_timer(&vars->timer))
            hero_set_state(0, 0);
        if (voice_is_playing(moby, vars->voice)) {
            release_voice_slot(vars->voice);
            vars->voice = -1;
        }
        if (hero.ground_moby != moby) {
            if (moby->state == 7)
                moby->state = 5;
            else
                moby->state = 4;
        }
        break;
    case 4:
    case 5: {
        int i;
        vars->pulse = fast_add_rotations(vars->pulse, D_0015ED6C * 6.2831855f);
        i = float_to_int((fast_sin(vars->pulse) * 4.0f - 3.0f) * 128.0f);
        if (i > 0x80) i = 0x80;
        else if (i < 0x20) i = 0x20;
        moby->glow_color = ((i << 16) | 0x80000000) | (i << 8) | i;
        if (hero.ground_moby == moby && hero.air_frames.s == 0 &&
            vector_distance(&hero.motion.pos, &moby->pos) < 0.5f) {
            moby->glow_color = 0x80208020;
            if (moby->state == 5)
                moby->command = 2;
            else
                moby->command = 3;
            moby->state = 1;
            hero_set_state(0x72, 1);
            jet_mount(D_L13_001670C0, D_L13_001670C0 + 0x10, 1, 0, 0);
            vars->blend = 0.0f;
            break;
        }
        if (moby->state == 4) {
            float d = vector_distance(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - vars->bottom) < 2.0f && d < 32.0f && d > 5.0f) {
                moby->command = 0;
                moby->state = 3;
                vars->speed = -(D_0015ED6C * 5.0f);
            }
        } else if (moby->state == 5) {
            float d = vector_distance(&moby->pos, &hero.motion.pos);
            if (absolute_float(hero.ground_z.f - vars->top) < 2.0f && d < 32.0f && d > 5.0f) {
                moby->command = 0;
                moby->state = 2;
                vars->speed = D_0015ED6C * 5.0f;
            }
        }
        break;
    }
    }
    add_vector_xyz(delta, delta, &moby->pos);
    update_entity_matrix(vars->path, delta, rot, &moby->rot);
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_0030C1F0), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");

void FUN_L13_0030ad38(struct Moby *moby, float *p, float *q) {
    float a, k;
    char *base;
    clear_u64_value(q);
    k = 2.25f;
    if ((moby->command >= 2 && (moby->command & 1)) ||
        (moby->state >= 2 && (moby->state & 1))) {
        a = -1.5707964f;
        q[0] = moby->rot.y;
    } else {
        a = 1.5707964f;
        q[0] = -moby->rot.y;
    }
    p[0] = fast_cos(a) * k;
    p[1] = fast_sin(a) * k;
    p[2] = 1.5f;
    transform_vector_by_basis(p, p, &moby->rot_row0);
    fast_vec_add(p, p, &moby->pos);
    base = D_0013E533 + 0xE1D;
    q[2] = fast_arc_tan(*(float *)(base + 0xD0) - p[0], *(float *)(base + 0xD4) - p[1]);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_0030C320), where it is exact; names translated to the US level program. */

extern char *FUN_L00_002db890(char *);
extern int D_L13_0015FFD8_m __asm__("D_L13_0015FFD8");
extern short *D_L13_001ABB40[];

int FUN_L13_0030ae68(int idx, int arg) {
    short *q = D_L13_001ABB40[idx];
    short *p;
    int count = 0;
    if (q == 0)
        return 0;
    p = q;
    do {
        int id = *p & 0x7FFF;
        char *m = (char *)(id << 8) + (int)D_L13_0015FFD8_m;
        if (m[0x20] >= 0) {
            char *f = FUN_L00_002db890(m);
            if (f == 0 || *(short *)(f + 0x68) < 4) {
                if (arg == -1 ||
                    *(unsigned char *)((char *)(id << 8) + (int)D_L13_0015FFD8_m + 0x20) != arg) {
                    count++;
                }
            }
        }
    } while (*p++ >= 0);
    return count;
}
/* Checks the moby's trigger lists and copies the configured state bytes into it. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_0030C408), where it is exact; names translated to the US level program. */

extern int is_point_inside_clip_volume_alt(void *, int) __asm__("FUN_00214720");

void FUN_L13_0030af50(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    int *p = (int *)d;
    int i;
    char *base;
    for (i = 0; i < 8; i++) {
        if (*p != -1) {
            if (FUN_L13_0030ae68(*p, -1) != 0)
                return;
            p++;
        }
    }
    base = D_0013E533 + 0xE1D;
    if (*(int *)(base + 0x15F0) != 0 && *(int *)(base + 0x2084) == 0x32)
        return;
    if (*(int *)(d + 0x28) != -1 &&
        is_point_inside_clip_volume_alt(base + 0x80, *(int *)(d + 0x28)) == 0)
        return;
    if (*(int *)(d + 0x20) != -1)
        moby->state = d[0x20];
    if (*(int *)(d + 0x24) == -1)
        return;
    moby->command = d[0x24];
}
extern int D_L13_0015F5C4;
extern int D_L13_0015FFD8_m __asm__("D_L13_0015FFD8");
extern char *D_L13_001600EC;
extern u8 alternate_item_available[128] __asm__("D_0013D388");
extern unsigned char D_001413F5 __attribute__((section(".data")));
extern float D_L13_0015F3FC;
extern int D_L13_0015F404;
extern float D_0013E090, D_0013E0A8, D_0013E094, D_0013E098;
extern struct {
    char pad0[0x60];
    float unk60, unk64, unk68;
    char pad6C[0xC];
    float unk78;
} D_0013E030;
extern unsigned char D_L13_0015FC88[] __attribute__((section(".sdata")));
extern char D_0013F3D0[];
extern int is_point_inside_clip_volume_alt(void *, int);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void jet_game_state(int, int) __asm__("FUN_L01_0027a248");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void FUN_L00_00299460(int);
extern void FUN_L00_002607d0(int);
extern void FUN_L00_0028e8a0(void *);
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");

void FUN_L13_0030b628(struct Moby *moby) {
    enum {
        START_INITIAL_SCENE = 1,
        CHECK_CLIP_VOLUME_FOR_SCENE = 4
    };
    int *data = (int *)moby->pvars;
    switch (moby->state) {
    case 0:
        if (D_L13_0015F5C4 == 0) moby->state = 1;
        break;
    case START_INITIAL_SCENE:
        if (alternate_item_available[0x64] == 0) {
            alternate_item_available[0x64] = 1;
            moby->state = 2;
            start_scene(0);
            return;
        }
        moby->state = 3;
        break;
    case 2:
        if (D_L13_0015F5C4 != 2) moby->state = 3;
        break;
    case 3:
        moby->state = 4;
        break;
    case CHECK_CLIP_VOLUME_FOR_SCENE: {
        if (alternate_item_available[0x65] == 0) {
            if (data[0] == -1)
                goto set_state_4;
            if (is_point_inside_clip_volume_alt(D_0013F3D0, data[0]) == 0)
                goto after_state_4;
            alternate_item_available[0x65] = 1;
            if (data[5] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[5] * 0x100);
                other->flags |= 0x41;
            }
            moby->state = 5;
            start_scene(1);
        } else {
            moby->state = 6;
            goto after_state_4;
set_state_4:
            moby->state = 7;
        }
after_state_4:
        if (moby->state != 4 && data[3] != -1) {
            mark_moby_for_removal((char *)(D_L13_0015FFD8_m + data[3] * 0x100));
        }
        break;
    }
    case 5: {
        char *object;
        char *entity;
        int index;
        if (D_L13_0015F5C4 != 2) {
            if (data[5] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[5] * 0x100);
                other->flags &= 0xffbe;
            }
            index = data[2];
            if (index != -1 && data[4] != -1) {
                entity = (char *)(D_L13_0015FFD8_m + data[4] * 0x100);
                object = D_L13_001600EC;
                index *= 0x80;
                qcopy(entity + 0x10, &object[index + 0x30]);
                object += index;
                FUN_L00_00216f90(object + 0x30, object + 0x70, 0, 1);
                D_001413F5 = 1;
                *(float *)(*(char **)(entity + 0x78) + 0x7c) = 0.99f;
                D_L13_0015F3FC = 0.99f;
                entity[0x20] = 2;
            }
            moby->state = 6;
        }
        break;
    }
    case 6:
        moby->state = 7;
        break;
    case 7: {
        char *object;
        if (alternate_item_available[0x66] == 0) {
            struct Moby *other;
            if (data[5] == -1) {
                moby->state = 13;
                break;
            }
            other = (struct Moby *)(D_L13_0015FFD8_m + data[5] * 0x100);
            if (other != 0 && other->oclass == 0x184 &&
                other->state != 0xfe && other->state != 0xfd) return;
            if (data[4] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[4] * 0x100);
                other->flags |= 2;
            }
            object = D_L13_001600EC + data[2] * 0x80;
            FUN_L00_00216f90(object + 0x30, object + 0x70, 0x72, 1);
            alternate_item_available[0x66] = 1;
            if (data[8] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[8] * 0x100);
                other->flags |= 0x41;
            }
            jet_game_state(0, 5);
            moby->state = 8;
            start_scene(2);
            return;
        }
        moby->state = 12;
        break;
    }
    case 8:
        if (D_L13_0015F5C4 != 2) {
            moby->state = 9;
            FUN_L00_00299460(14);
        }
        break;
    case 9:
        if (D_L13_0015F5C4 != 2) {
            start_scene(3);
            moby->state = 10;
        }
        break;
    case 10: {
        if (D_L13_0015F5C4 != 2) {
            if (data[4] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[4] * 0x100);
                other->flags &= 0xfffd;
            }
            FUN_L00_002607d0(14);
            if (data[8] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[8] * 0x100);
                other->flags &= 0xffbe;
                FUN_L00_0028e8a0(other);
                D_0013E030.unk60 = 464.66f;
                D_0013E030.unk64 = 580.68f;
                D_0013E030.unk68 = 316.72f;
                D_0013E030.unk78 = 2.77f;
            }
            D_L13_0015F404 = 0;
            moby->state = 11;
        }
        break;
    }
    case 11: {
        if (data[4] != -1) {
            struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[4] * 0x100);
            if (other != 0 && other->oclass == 0x45 &&
                other->state != 0xfe && other->state != 0xfd) return;
            memcard_save_data(0, -1);
            moby->state = 12;
            return;
        }
        moby->state = 12;
        break;
    }
    case 12:
        moby->state = 13;
        break;
    case 13:
        if (data[4] != -1) {
            if (D_L13_0015FC88[*(unsigned char *)(D_L13_0015FFD8_m + data[4] * 0x100 + 0xb0)] == 0xff) {
                if (data[8] != -1)
                    mark_moby_for_removal((char *)(D_L13_0015FFD8_m + data[8] * 0x100));
                moby->state = 14;
            }
        } else {
            mark_moby_for_removal(moby);
        }
        break;
    case 14:
        moby->flags |= 2;
        break;
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Follows its target moby along a path: snaps to the nearest path point, then walks the path segments. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D028), where it is exact; names translated to the US level program. */

extern char *D_L13_001B07B0[];
extern float FUN_001f9b48(void *, void *);
extern int D_L13_0015FFD8_m __asm__("D_L13_0015FFD8");
extern int FUN_L00_0025df68(void *, void *, float *, int *, float *, int, float, float, float);
extern int FUN_L09_00294ae0(void *, void *, void *, int, int, float);
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void FUN_L13_0030bb70(struct Moby *m) {
    int *d = (int *)m->pvars;
    char *path;
    char *pth;
    char *o;
    float v[4];
    int idx;
    float t;
    int i;
    if (m->state == 0) {
        m->update_dist = 0xFF;
        if (d[1] != -1) {
            path = D_L13_001B07B0[d[1]];
            for (i = 0; i < *(int *)path; i++) {
                *(float *)(path + (i << 4) + 0x1C) = FUN_001f9b48(
                    path + (i * 16 + 0x10), path + ((i + 1) % *(int *)path * 16 + 0x10));
            }
        }
        d[2] = 0;
        d[3] = 0;
        m->state = 1;
        d[4] = 0;
    }
    if (d[0] == -1)
        return;
    o = (char *)(D_L13_0015FFD8_m + (d[0] << 8));
    if (o == 0 || ((unsigned char *)o)[0x20] == 0xFE || ((unsigned char *)o)[0x20] == 0xFD) {
        mark_moby_for_removal(m);
        return;
    }
    if (((unsigned char *)o)[0x20] >= 7) {
        d[4]++;
        if (d[1] == -1)
            return;
        pth = D_L13_001B07B0[d[1]];
        if (m->state == 1) {
            FUN_L00_0025df68(pth, o + 0x10, v, &idx, &t, 0, 100.0f, 1.0f, 0.0f);
            if (idx > d[2] || t > ((float *)d)[3] || d[4] < 0x1E) {
                d[2] = idx;
                ((float *)d)[3] = t;
                qcopy(&m->pos, v);
            } else {
                m->state = 2;
            }
        }
        if (m->state == 2) {
            if (d[2] < *(int *)pth - 1) {
                int k = FUN_L09_00294ae0(m, pth, &m->pos, d[2], 1, frame_scale * 0.25f);
                d[2] = k;
                ((float *)d)[3] = FUN_001f9b48(&m->pos, pth + (k * 16 + 0x10));
            } else {
                m->state = 3;
            }
        }
    } else {
        qcopy(&m->pos, o + 0x10);
    }
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D2A8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x30];
    int state;
    char pad34[0x14C];
    int target;
} L13Action;

extern L13Action D_L13_0016CB60;
extern int D_L13_0015F5C4;
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void FUN_L13_0030bdf0(struct Moby *moby) {
    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case 1:
        if (D_L13_0015F5C4 == 2 && D_L13_0016CB60.state == 2) {
            cutscene_set_camera_target(D_L13_0016CB60.target);
        }
        break;
    }
}
/* Records the classes of the tracked mobys, then removes this moby once too few of them are left. */
void FUN_L13_0030be60(struct Moby *m) {
    int *d = (int *)m->pvars;
    if (m->state == 0) {
        int *ids = d + 4;
        int *cls = d + 12;
        char *base;
        int i;
        m->state = 1;
        base = (char *)D_L13_0015FFD8_m;
        for (i = 0; i < 8; i++) {
            unsigned char *o = (unsigned char *)(base + (ids[i] << 8));
            if (o != 0 && o[0x20] != 0xFE && o[0x20] != 0xFD)
                cls[i] = *(short *)(o + 0xA6);
            else
                cls[i] = -1;
        }
        if (d[0] == 0)
            m->state = 2;
    } else if (m->state == 1) {
        int *ids = d + 4;
        int *cls = d + 12;
        char *base = (char *)D_L13_0015FFD8_m;
        int n = 0;
        int i;
        for (i = 0; i < 8; i++) {
            int id = ids[i];
            int c;
            unsigned char *o;
            if (id != -1 && (c = cls[i]) != -1) {
                o = (unsigned char *)(base + (id << 8));
                if (o != 0 && *(short *)(o + 0xA6) == c && o[0x20] != 0xFE && o[0x20] != 0xFD) {
                    n++;
                    continue;
                }
            }
            cls[i] = -1;
        }
        if (n < d[0])
            mark_moby_for_removal(m);
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D468), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern void FUN_L00_00250df8(void *);

char *FUN_L13_0030bfb0(char *owner, void *position, float scale) {
    char *moby = func_0020D348_m(0x660);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(data + 0x60) = owner;
        *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * scale;
        qcopy(moby + 0x10, position);
        if (owner != 0) {
            *(int *)(data + 0x64) = *(short *)(owner + 0xA6);
        }
        FUN_L00_00250df8(moby);
    }
    return moby;
}
#include "qcopy.h"

/* updates the moby: deletes it when its owner is gone, else copies the owner's effect data and marks it */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D528), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void FUN_L00_00259a88(void *, void *);
void mark_moby_for_removal_u(struct Obj *obj) __asm__("FUN_0020c828");

void FUN_L13_0030c060(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *owner = *(char **)(data + 0x60);
    char *p;
    char buf[0x30];
    if (owner == 0 || *(short *)(owner + 0xA6) != *(int *)(data + 0x64) ||
        (unsigned char)owner[0x20] == 0xFE || (unsigned char)owner[0x20] == 0xFD) {
        mark_moby_for_removal_u(moby);
        return;
    }
    p = find_moby_hit(moby, -1, 0);
    if (p != 0) {
        qcopy(buf, p + 0x10);
        *(int *)(buf + 0x10) = *(int *)(p + 0x20);
        *(int *)(buf + 0x14) = *(int *)(p + 0x24);
        buf[0x18] = p[0x28];
        buf[0x19] = p[0x29];
        *(short *)(buf + 0x1A) = *(short *)(p + 0x2A);
        *(float *)(buf + 0x1C) = *(float *)(p + 0x2C);
        *(int *)(buf + 0x20) = *(int *)(p + 0x30);
        FUN_L00_00259a88(*(void **)(data + 0x60), buf);
    }
    moby->hit_slot = 0xFF;
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D600), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L00_00250df8(void *);

char *FUN_L13_0030c138(void *a, void *b, int c, int d) {
    unsigned char *m = (unsigned char *)func_0020D348_m(0x662);
    if (m != 0) {
        char *data;
        float r1, dd;
        data = *(char **)(m + 0x78);
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        m[0x23] = random_int_between(0x40, 0x80);
        set_moby_color(m, 0x20, 0x60, 0x10);
        qcopy(m + 0x10, a);
        qcopy(data, b);
        r1 = random_float_between_alt(frame_time * 6.2831855f, frame_time * 12.566371f);
        dd = frame_time;
        *(float *)(data + 0x10) = r1;
        *(float *)(data + 0x14) = random_float_between_alt(dd * 6.2831855f, dd * 12.566371f);
        *(short *)(data + 0x18) = c;
        *(short *)(data + 0x1A) = c;
        if (d == 0) {
            *(float *)(m + 0x2C) = *(float *)(m + 0x2C) * random_float_between_alt(0.5f, 0.75f);
        }
        *(float *)(data + 0x1C) = *(float *)(m + 0x2C);
        *(float *)(m + 0x2C) = *(float *)(m + 0x2C) * random_float_between_alt(2.0f, 5.0f);
        m[0xBC] = d;
        FUN_L00_00250df8(m);
    }
    return (char *)m;
}
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float FUN_001f96b0_c3b8(float) __asm__("FUN_L13_0020a9f8");
extern int FUN_001fa6d0(float);
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_0020c828(void *);
extern void FUN_L13_0030c2b0(void *);
extern void spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float D_0015ED70;
extern s32 D_001413D0_lui[] __asm__("D_001413D0") __attribute__((section(".data")));
extern float D_L13_00161F18 __attribute__((sda));
extern float D_L13_00161F1C __attribute__((sda));
extern float D_L13_00161F20 __attribute__((sda));
extern float D_L13_00161F24 __attribute__((sda));
extern float D_L13_00161F28 __attribute__((sda));
extern float D_L13_00161F2C __attribute__((sda));
extern float D_L13_00161F30 __attribute__((sda));
extern float D_L13_00161F34 __attribute__((sda));
extern float D_L13_00161F38 __attribute__((sda));
extern float D_L13_00161F3C __attribute__((sda));
extern float D_L13_00161F40 __attribute__((sda));
extern float D_L13_00161F44 __attribute__((sda));
extern float D_L13_00161F48 __attribute__((sda));
extern float D_L13_00161F4C __attribute__((sda));
extern float D_L13_00161F50 __attribute__((sda));
extern float D_L13_00161F54 __attribute__((sda));
extern float D_L13_00161F58 __attribute__((sda));
extern float D_L13_00161F5C __attribute__((sda));
extern float D_L13_00161F60 __attribute__((sda));
extern float D_L13_00161F64 __attribute__((sda));
extern float D_L13_00161F68 __attribute__((sda));
extern int D_L13_00161F08 __attribute__((sda));
extern int D_L13_00161F0C __attribute__((sda));
extern int D_L13_00161F10 __attribute__((sda));
extern int D_L13_00161F14 __attribute__((sda));

typedef union {
    OvlQuad quad;
    float f[4];
} L13EffectVector;

#include "qzero.h"

void FUN_L13_0030c3b8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    L13EffectVector old_pos, position, velocity, acceleration, second_pos;
    float angle, direction, amount;
    int a, b, c;
    int i;
    qcopy(&old_pos, &moby->pos);
    if (moby->command == 1) {
        velocity.f[3] = D_L13_00161F5C;
        acceleration.f[3] = D_L13_00161F60;
        i = 0;
        do {
            qcopy(&position, &moby->pos);
            position.f[0] += random_float_between_alt(-0.065f, 0.065f);
            position.f[1] += random_float_between_alt(-0.065f, 0.065f);
            position.f[2] += random_float_between_alt(-0.065f, 0.065f);
            qzero(&velocity);
            i++;
            velocity.f[2] = random_float_between_alt(D_L13_00161F30 * frame_time * 0.9f,
                                                       D_L13_00161F30 * frame_time * 1.1f);
            angle = random_float_between_alt(-D_L13_00161F50, D_L13_00161F50) * 0.017453292f;
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[0] = fast_cos(direction) * (D_L13_00161F24 * frame_time);
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[1] = fast_sin(direction) * (D_L13_00161F24 * frame_time);
            acceleration.f[2] = 0.0f;
            acceleration.f[2] = random_float_between_alt(D_L13_00161F28 * frame_time * 0.9f,
                                                          D_L13_00161F28 * frame_time * 1.1f);
            a = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(D_L13_00161F18,
                                                                     D_L13_00161F18 + D_L13_00161F18)));
            b = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(D_L13_00161F1C,
                                                                     D_L13_00161F1C * 1.2f)));
            c = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(D_L13_00161F20,
                                                                     D_L13_00161F20 * 1.2f)));
            spawn_spark(&position, &velocity, &acceleration, D_L13_00161F08,
                           D_L13_00161F0C, a, b, c, -1);
        } while (i < 2);
        amount = 0.017453292f;
        velocity.f[3] = D_L13_00161F64;
        acceleration.f[3] = D_L13_00161F68;
        i = 1;
        do {
            qcopy(&second_pos, &moby->pos);
            second_pos.f[0] += random_float_between_alt(-0.1f, 0.1f);
            second_pos.f[1] += random_float_between_alt(-0.1f, 0.1f);
            second_pos.f[2] += random_float_between_alt(-0.1f, 0.1f);
            random_float_between_alt(-D_L13_00161F58, D_L13_00161F58);
            qzero(&velocity);
            i--;
            velocity.f[2] = random_float_between_alt(D_L13_00161F44 * frame_time * 0.7f,
                                                       D_L13_00161F44 * frame_time * 1.5f);
            angle = random_float_between_alt(-D_L13_00161F54, D_L13_00161F54) * amount;
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)),
                                      angle);
            acceleration.f[0] = fast_cos(direction) * (D_L13_00161F48 * frame_time);
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)),
                                      angle);
            acceleration.f[1] = fast_sin(direction) * (D_L13_00161F48 * frame_time);
            acceleration.f[2] = 0.0f;
            acceleration.f[2] = random_float_between_alt(D_L13_00161F4C * frame_time * 0.7f,
                                                          D_L13_00161F4C * frame_time * 1.5f);
            a = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(D_L13_00161F34,
                                                                     D_L13_00161F34 + D_L13_00161F34)));
            b = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(D_L13_00161F38,
                                                                     D_L13_00161F38 * 1.2f)));
            c = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(D_L13_00161F3C,
                                                                     D_L13_00161F3C * 1.2f)));
            spawn_spark(&second_pos, &velocity, &acceleration, D_L13_00161F10,
                           D_L13_00161F14, a, b, c, -1);
        } while (i >= 0);
    }
    moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x10));
    moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x14));
    fast_vec_add(&moby->pos, &moby->pos, data);
    *(float *)(data + 8) -= D_0015ED70 * 14.6f;
    if (!(moby->pos.x < 0.0f) && !(moby->pos.y < 0.0f) && !(moby->pos.z < 0.0f)) {
        short life = *(short *)(data + 0x18);
        short duration = *(short *)(data + 0x1a);
        if (life < duration / 4) {
            *(float *)((char *)moby + 0x2c) = *(float *)(data + 0x1c) * convert_integer_to_float(life) /
                                               convert_integer_to_float(*(short *)(data + 0x1a) / 4);
        }
        if (fast_dec_timer_s16(data + 0x18) == 0) goto collision;
    }
    FUN_0020c828(moby);
    return;
collision:
    if (moby->command != 1) return;
    if (collision_line(&old_pos, &moby->pos, 2, (void *)(*(int *)&D_001413D0_lui), 0) == 0) return;
    FUN_L13_0030c2b0(moby);
    FUN_0020c828(moby);
}
#include "types.h"
#include "rnc/gameplay/state/item_state.h"
extern unsigned char D_0013D408[];
extern float D_L13_0015F580[] __attribute__((section(".sdata")));
extern void fun_00213928(void *, void *, void *, int, void *, int, int, int) __asm__("FUN_00213928");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int FUN_L01_0026e008(int, int);
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L01_002787a0(void *);
extern void FUN_L00_0025f8e0(void *, float);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void FUN_0020c828(void *);

#include "qzero.h"
extern s32 D_L13_001BA7D0[];
void blarg_ship_update(struct Moby *moby) __asm__("FUN_L13_0030cdb8");
void blarg_ship_update(struct Moby *moby) {
    char *data;
    void *hit;
    OvlVec4 vector;
    char hit_buffer[16];
    int i, j;
    if ((D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] >>
         (moby->save_id & 31) & 1) != 0) {
        FUN_0020c828(moby);
        return;
    }
    data = (char *)moby->pvars;
    hit = find_moby_hit(moby, 0x10000, 0);
    fun_00213928(moby, hit, data + 0x20, 0, hit_buffer, 0, 0, 4);
    switch (moby->state) {
    case 0:
        moby->state = 1;
        break;
    case 1:
        if (hit != 0) {
            float cost = *(float *)((char *)hit + 0x2c);
            if (*(float *)(data + 0x20) < cost) {
                moby->state = 2;
            } else {
                ((unsigned char *)data)[0x67] = 0xfa;
                *(float *)(data + 0x20) -= cost;
                trooper_anim_update(moby, data + 0x60);
            }
            moby->hit_slot = 0xff;
        }
        trooper_anim_finish(moby, data + 0x60);
        break;
    case 2: {
        void *pos;
        void *rot;
        void *zero_vector;
        if (moby->group != 0xff && FUN_L01_0026e008(moby->group, -1) == 1 && D_0013D408[0x16] == 0) {
            D_0013D408[0x16] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        allocate_voice_for_target_entry(0, 0, moby);
        pos = &moby->pos;
        rot = &moby->rot;
        FUN_L01_002787a0(moby);
        zero_vector = D_L13_0015F580;
        D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
            1 << (moby->save_id & 31);
        D_L13_001BA7D0[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
        for (j = 0; j < 2; j++) {
            for (i = 0x709; i < 0x70d; i++) {
                qzero(&vector);
                FUN_L00_0025f8e0(&vector, 1.5f);
                vector.f[2] += 1.7f;
                fast_vec_add(&vector, &vector, pos);
                FUN_L00_00263fd8(moby, i, &vector, rot, 0, 0, 0.0f, zero_vector, zero_vector, zero_vector);
            }
        }
        spawn_explosion(moby, pos, 0, 0.0f, 0.0f, 10, 3, 0x10,
                         6.0f, 3.0f, 9.0f, 1.1f, 0, 15.0f, 1, 3, -1, 0);
        FUN_0020c828(moby);
        break;
    }
    }
}

s32 D_L13_001CABA8[1] __attribute__((section(".data"))) = {0};
s32 D_L13_001CABAC[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001CABB0[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001CABB4[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001CABB8[1] __attribute__((section(".data"))) = {0};
