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

char *create_attached_rocket(struct Moby *source_moby, short joint_index) __asm__("FUN_L13_003058a8");

char *create_attached_rocket(struct Moby *source_moby, short joint_index) {
    char *moby = create_moby(0x4D1);
    char *moby_vars;
    char *spawned_moby_position;
    if (moby != 0) {
        moby_vars = *(char **)(moby + 0x78);
        moby[0x31] = 1;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x20] = 1;
        *(char **)(moby_vars + 0x20) = source_moby;
        *(short *)(moby_vars + 0x26) = joint_index;
        *(short *)(moby_vars + 0x24) = 1;
        spawned_moby_position = moby + 0x10;
        moby_point_position(source_moby, joint_index, spawned_moby_position);
        qcopy(moby + 0x40, &source_moby->rot);
        *(OvlQuad *)(moby + 0xC0) = *(OvlQuad *)&source_moby->rot_row0;
        *(OvlQuad *)(moby + 0xD0) = *(OvlQuad *)&source_moby->rot_row1;
        *(OvlQuad *)(moby + 0xE0) = *(OvlQuad *)&source_moby->rot_row2;
        qcopy(moby_vars + 0x10, spawned_moby_position);
        *(int *)(moby + 0x94) = 0;
        FUN_L00_00250df8(moby);
    }
    return moby;
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Initializes a Gemlik moby from its data block, copying transforms and resetting state. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00306E20), where it is exact; names translated to the US level program. */


extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00250df8(void *);

void launch_attached_rocket(struct Moby *moby, char *pos, int distance_reference_moby, float max_travel_distance) __asm__("FUN_L13_00305988");

void launch_attached_rocket(struct Moby *moby, char *pos, int distance_reference_moby, float max_travel_distance) {
    char *moby_vars = (char *)moby->pvars;
    char *source_moby = *(char **)(moby_vars + 0x20);
    char *mp;
    *(float *)(moby_vars + 0x28) = max_travel_distance;
    qcopy(&moby->rot, source_moby + 0x40);
    mp = &moby->pos;
    *(OvlQuad *)&moby->rot_row0 = *(OvlQuad *)(source_moby + 0xC0);
    *(OvlQuad *)&moby->rot_row1 = *(OvlQuad *)(source_moby + 0xD0);
    *(OvlQuad *)&moby->rot_row2 = *(OvlQuad *)(source_moby + 0xE0);
    moby_point_position(*(void **)(moby_vars + 0x20), *(short *)(moby_vars + 0x26), mp);
    qcopy(moby_vars + 0x10, mp);
    qcopy(moby_vars, pos);
    *(int *)(moby_vars + 0x2C) = distance_reference_moby;
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
extern void make_hit_descriptor(void *, struct Moby *, int, float, void *) __asm__("FUN_L00_00259888");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_0026dd20(void *, void *, int, int, float);
extern void spawn_projectile_expiry_particle(void *, void *) __asm__("FUN_L07_0029a2a0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern float D_0015ED6C;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) L13Vector_5a58;

void update_attached_rocket(struct Moby *moby) __asm__("FUN_L13_00305a58");

void update_attached_rocket(struct Moby *moby) __asm__("FUN_L13_00305a58");
void spawn_attached_rocket_non_damaging_explosion(struct Moby *moby) __asm__("FUN_L13_00306148");

void spawn_attached_rocket_explosion(struct Moby *moby) __asm__("FUN_L13_003060b8");

void update_attached_rocket(struct Moby *moby) {
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
    make_hit_descriptor(work.hit, moby, 0x10001, 1.0f, &work.velocity);
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
                spawn_projectile_expiry_particle(&moby->pos, &work.direction);
            }
        }
        spawn_attached_rocket_explosion(moby);
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
        spawn_attached_rocket_non_damaging_explosion(moby);
    } else {
        spawn_attached_rocket_explosion(moby);
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

void spawn_attached_rocket_explosion(struct Moby *moby) {
    char vector[16];
    clear_u64_value(vector);
    func_L00_0025F4A8_alt(moby, vector, &moby->pos, 2.0f, 1.0f, 12, 4, 18, 4.0f, 3.0f, 1.0f, 2.0f,
                          0, 20.0f, 1, 1, -1, 0);
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_003075E0), where it is exact; names translated to the US level program. */

extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void spawn_attached_rocket_non_damaging_explosion(struct Moby *moby) {
    char vector[16];
    clear_u64_value(vector);
    spawn_explosion(moby, vector, &moby->pos, 0.0f, 0.0f, 5, 1, 5, 0.0f, 0.0f, 0.0f, 1.0f, 0,
                     0.0f, 0, 1, -1, 0);
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_00307660), where it is exact; names translated to the US level program. */

extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void FUN_L00_00250df8(void *);
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

char *spawn_gemlik_projectile(float speed, float *spawn_position, float *launch_displacement, void *source_moby) __asm__("FUN_L13_003061c8");

char *spawn_gemlik_projectile(float speed, float *spawn_position, float *launch_displacement, void *source_moby) {
    char *moby;
    char *moby_vars;
    float direction_length_xy;
    moby = create_moby(0x4D6);
    if (moby != 0) {
        *(int *)(moby + 0x40) = 0;
        *(unsigned char *)(moby + 0x30) = 0xFF;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        moby_vars = *(char **)(moby + 0x78);
        direction_length_xy = vector_length_xy(launch_displacement);
        *(float *)(moby + 0x44) = -fast_arc_tan(direction_length_xy, launch_displacement[2]);
        *(float *)(moby + 0x48) = fast_arc_tan(launch_displacement[0], launch_displacement[1]);
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby_vars + 0x10, spawn_position);
        qcopy(moby_vars, launch_displacement);
        blend_moby_animation(moby, 1, 0, scale_game_frames(5));
        *(short *)(moby + 0x34) = 0x208;
        *(int *)(moby_vars + 0x24) = 1;
        *(void **)(moby_vars + 0x20) = source_moby;
        *(float *)(moby_vars + 0x28) = speed;
        moby[0x23] = 0x10;
        *(int *)(moby_vars + 0x2C) = func_001FA898_r(speed / fast_vec_length(moby_vars) + 1.0f);
        FUN_L00_00250df8(moby);
    }
    return moby;
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
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_L00_00257b90(int, int);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern struct L13Ent00178480 D_L13_00178400[];
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");

void update_gemlik_projectile(struct Moby *moby) __asm__("FUN_L13_00306300");

void update_gemlik_projectile(struct Moby *moby) {
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
    struct L13Ent00178480 *target_hit_entry;

    qcopy(&work.old_pos, &moby->pos);
    add_vector_xyz(&moby->pos, &moby->pos, data);
    qcopy(&work.velocity, data);
    work.velocity.z = 0.0f;
    set_vector_length_xy(&work.velocity, &work.velocity, 1.0f);
    work.velocity.z = 1.0f;
    flags = 0x10001;
    base = (unsigned char *)((char *)((char *)&hero));
    if (base[0x1FF6] != 0 && *(float *)(data + 0xC) != 31337.6015625f &&
        fast_vec_dist(&moby->pos, base + 0x80) < 2.0f) {
        fast_vec_normalize(data, data, 0.5f);
        subtract_vector_xyz(&moby->pos, &moby->pos, data);
        qzero(data);
        *(int *)(data + 0x2C) = 5;
        *(float *)(data + 0xC) = 31337.6015625f;
        for (i = 0; i < 6; i++) {
            unsigned char *hero_base = (unsigned char *)((char *)((char *)&hero));
            ent = ((char **)(hero_base + 0x2020))[i];
            if (ent != 0) {
                int hit_mask = flags | 1;
                make_hit_descriptor(work.hit, moby, hit_mask, 1.0f, &work.velocity);
                work.hit[0x18] = 1;
                *(unsigned short *)(work.hit + 0x1A) = *(unsigned short *)((char *)moby + 0xA6);
                work.hit[0x19] = 1;
                if ((unsigned char)ent[0xA4] == 0xFF || (target_hit_entry = &D_L13_00178400[(unsigned char)ent[0xA4]])->p34 != ent) {
                    record_moby_hit(ent, work.hit);
                } else {
                    target_hit_entry->f2C += 1.0f;
                    D_L13_00178400[(unsigned char)ent[0xA4]].b28 = 1;
                    D_L13_00178400[(unsigned char)ent[0xA4]].b29 = 1;
                    D_L13_00178400[(unsigned char)ent[0xA4]].s2A = *(unsigned short *)((char *)moby + 0xA6);
                    { struct L13Ent00178480 *t = &D_L13_00178400[(unsigned char)ent[0xA4]]; t->i24 = hit_mask; }
                }
                break;
            }
        }
    }

    make_hit_descriptor(work.hit, moby, 0x10001, 1.0f, &work.velocity);
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
                fast_vec_normalize(&work.direction, &work.direction, fast_vec_length(&work.random_direction) * 0.1f);
                add_vector_xyz(&work.direction, &work.random_direction, &work.direction);
                fast_vec_normalize(&work.direction, &work.direction,
                    random_float_between(D_0015ED6C + D_0015ED6C, D_0015ED6C * 4.0f));
                spawn_projectile_expiry_particle(&moby->pos, &work.direction);
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
            fast_vec_normalize(&work.direction, &work.direction, fast_vec_length(&work.random_direction) * 0.5f);
            add_vector_xyz(&work.direction, &work.random_direction, &work.direction);
            fast_vec_normalize(&work.direction, &work.direction,
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
    mark_moby_for_removal(moby);
    return;
bounds:
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f ||
        moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
        moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal(moby);
}

extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern float dot_vector_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern int random_integer_below(int) __asm__("FUN_00213260");
extern char D_L13_001670C0[];
typedef struct { int v[6]; } L13EffectColors;
extern L13EffectColors D_L13_001F5560;
extern L13EffectColors D_L13_001F5578;
char *spawn_poison_vessel_effect_moby(void *position, void *initial_velocity, int duration_frames, int effect_mode) __asm__("FUN_L13_0030c138");

extern char *spawn_poison_vessel_effect_moby(void *, void *, int, int) __asm__("FUN_L13_0030c138");
extern void spawn_particle(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern char *spawn_randomly_rotated_colored_moby(void *, void *, void *, float, int, unsigned char, unsigned char, unsigned char, int) __asm__("FUN_L00_002df808");

void spawn_poison_vessel_destruction_effects(struct Moby *moby, void *vars) __asm__("FUN_L13_003073c8");

void spawn_poison_vessel_destruction_effects(struct Moby *moby, void *vars) {
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
        int i;
        for (i = 0; i < 10; i++) {
            int lo, hi, duration;
            OvlVec4 *random_direction, *offset, *axis;
            work.random_vector.q = 0;
            work.random_vector.f[0] = random_float_between(-1.0f, 1.0f);
            work.random_vector.f[1] = random_float_between(-1.0f, 1.0f);
            random_direction = &work.random_direction;
            axis = &work.axis;
            offset = &work.offset;
            random_direction->q = work.random_vector.q;
            amount = dot_vector_xyz(random_direction, axis);
            fast_vec_normalize(offset, axis, amount);
            subtract_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(offset, axis, random_float_between(0.0f, 1.0f));
            add_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(random_direction, random_direction,
                scale * random_float_between(3.5f, 6.5f) * D_0015ED6C);
            add_vector_xyz(random_direction, random_direction, &work.velocity);
            lo = scale_game_frames(60); hi = scale_game_frames(120);
            duration = random_int_between(lo, hi);
            spawn_poison_vessel_effect_moby(&work.pos, random_direction, duration, 0);
        }
    }
    twice_distance = distance + distance;
    {
        int i;
        for (i = 0; i < 4; i++) {
            int lo, hi, duration;
            OvlVec4 *random_direction, *axis, *offset;
            random_direction = &work.random_direction;
            axis = &work.axis;
            work.random_vector.q = 0;
            work.random_vector.f[0] = random_float_between(-1.0f, 1.0f);
            work.random_vector.f[1] = random_float_between(-1.0f, 1.0f);
            offset = &work.offset;
            random_direction->q = work.random_vector.q;
            fast_vec_normalize(offset, axis, dot_vector_xyz(random_direction, axis));
            subtract_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(offset, axis, random_float_between(0.0f, 1.0f));
            add_vector_xyz(random_direction, random_direction, offset);
            fast_vec_normalize(random_direction, random_direction,
                scale * random_float_between(6.5f, 10.0f) * D_0015ED6C);
            add_vector_xyz(random_direction, random_direction, &work.velocity);
            lo = scale_game_frames(60); hi = scale_game_frames(90);
            duration = random_int_between(lo, hi);
            spawn_poison_vessel_effect_moby(&work.pos, random_direction, duration, 1);
        }
    }
    work.random_vector.q = 0;
    work.random_vector.f[0] = random_float_between(-1.0f, 1.0f);
    work.random_vector.f[1] = random_float_between(-1.0f, 1.0f);
    work.random_vector.f[2] = random_float_between(-1.0f, 1.0f);
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
    spawn_poison_vessel_effect_moby(&work.pos, &work.random_direction, duration, 1);
    count = distance < 6.0f ? truncate_float_to_s32(distance) + 1 : 4;
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
            speed = (scale * random_float_between(8.0f, 10.0f)) * D_0015ED6C - extra * D_0015ED6C;
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
            spawn_particle(&work.pos, &work.velocity, *color, *second_color,
                amount, duration, duration2, 0, 0, speed);
        } while (i != 0);
    }
    if (distance > 9.0f) {
        float amount;
        amount = scale * 4.0f;
        duration = scale_game_frames(15);
        spawn_randomly_rotated_colored_moby(moby, &work.pos, &work.velocity, amount, duration, 127, 127, 127, 32);
        duration = scale_game_frames(24);
        spawn_randomly_rotated_colored_moby(moby, &work.pos, &work.velocity, amount, duration, 32, 127, 0, 32);
    }
    {
        float amount;
        amount = scale * 4.0f;
        duration = scale_game_frames(20);
        spawn_randomly_rotated_colored_moby(moby, &work.pos, &work.velocity, amount, duration, 63, 127, 0, 48);
    }
    {
        float amount;
        amount = scale * 3.5f;
        duration = scale_game_frames(27);
        spawn_randomly_rotated_colored_moby(moby, &work.pos, &work.velocity, amount, duration, 16, 96, 0, 64);
    }
    {
        float amount;
        amount = scale * 3.0f;
        duration = scale_game_frames(29);
        spawn_randomly_rotated_colored_moby(moby, &work.pos, &work.velocity, amount, duration, 0, 32, 0, 32);
    }
}
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
#include "rnc/overlay/l13/gameplay/entities/rocket_globals.h"
extern int poison_vessel_blast_duration_frames_gp __asm__("D_L13_00161E2C") __attribute__((sda));
extern f32 poison_vessel_blast_start_radius_gp __asm__("D_L13_00161E30") __attribute__((sda));
extern f32 poison_vessel_blast_end_radius_gp __asm__("D_L13_00161E34") __attribute__((sda));
extern f32 poison_vessel_blast_damage_gp __asm__("D_L13_00161E38") __attribute__((sda));
extern struct BlastHit *find_moby_hit_7b10(void *, int, int) __asm__("FUN_L00_0025a420");
extern int fptodp_7b10(float) __asm__("FUN_00120478");
extern void debug_print_7b10() __asm__("FUN_001e93b0");
extern int hit_check_7b10(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void stop_moby_sound_7b10(void *, int, int) __asm__("FUN_L00_00257470");
extern void anim_reset_7b10(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_7b10(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void spawn_type_52_particle_near_moby(struct Moby *) __asm__("FUN_L10_002e3de8");
extern int allocate_voice_7b10(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_7b10(int) __asm__("FUN_001f96f8");
extern float random_float_7b10(float, float) __asm__("FUN_002132a8");
extern float int_to_float_7b10(int) __asm__("FUN_001fa6c0");
extern float interpolate_scalar_linear(float, float, float) __asm__("FUN_L00_0025b6a8");
extern int collect_mobies_7b10(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void blast_mobys_7b10(struct Moby *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern float scale_time_7b10(float) __asm__("FUN_001f96b0");
extern void remove_7b10(struct Moby *) __asm__("FUN_0020c828");

void poison_vessel_update(struct Moby *moby) __asm__("FUN_L13_00307b10");
void poison_vessel_update(struct Moby *moby) {
    enum {
        POISON_VESSEL_INTACT = 0,
        POISON_VESSEL_BLASTING = 1
    };
    enum {
        HIT_RESULT_NONE = 0,
        HIT_RESULT_DEATH = 1,
        HIT_RESULT_DYING_KNOCKBACK = 2,
        HIT_RESULT_STUN_KNOCKBACK = 3,
        HIT_RESULT_NO_REACTION = 11
    };
    struct BlastVars *vars = (struct BlastVars *)moby->pvars;
    struct BlastHit *hit;
    float *health;
    void *effects;
    int kind;
    float pos[4];
    int hits;
    int timer;

    switch (moby->state) {
    case POISON_VESSEL_INTACT:
        vars->hit = 0;
        moby->rot.x *= 0.7f;
        moby->rot.y *= 0.7f;
        hit = find_moby_hit_7b10(moby, 0x330001, 0);
        health = &vars->health;
        if (hit != 0) {
            debug_print_7b10(D_L13_001F5590, moby->oclass, (s16)moby->save_id, fptodp_7b10(hit->damage));
        }
        switch (hit_check_7b10(moby, hit, health, 0, &kind, 0, 0, 4)) {
        case HIT_RESULT_DEATH:
        case HIT_RESULT_DYING_KNOCKBACK:
            *health = 0;
            break;
        case HIT_RESULT_NONE:
        case HIT_RESULT_STUN_KNOCKBACK:
        case HIT_RESULT_NO_REACTION:
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
                spawn_poison_vessel_destruction_effects(moby, vars);
                spawn_type_52_particle_near_moby(moby);
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
    case POISON_VESSEL_BLASTING:
        qcopy(pos, &moby->pos);
        pos[2] += 1.0f;
        hits = collect_mobies_7b10(interpolate_scalar_linear(poison_vessel_blast_start_radius_gp, poison_vessel_blast_end_radius_gp,
                                              int_to_float_7b10(vars->timer) /
                                                  int_to_float_7b10(scale_game_frames_7b10(poison_vessel_blast_duration_frames_gp))),
                                    pos, 0x10, moby, 0);
        blast_mobys_7b10(moby, pos, D_L13_00178200, hits, 0, 0x830001, 2, 1, poison_vessel_blast_damage_gp, 1.0f, 1.0f);
        if (vars->timer > scale_time_7b10((float)poison_vessel_blast_duration_frames_gp * 0.5f)) {
            moby->flags |= 0x41;
            spawn_type_52_particle_near_moby(moby);
        } else {
            moby->alpha = moby->alpha * 7 / 8;
        }
        timer = vars->timer + 1;
        vars->timer = timer;
        if (scale_game_frames_7b10(poison_vessel_blast_duration_frames_gp) < timer) {
            spawn_type_52_particle_near_moby(moby);
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
extern int FUN_L00_00258b50(struct Moby *, void *, void *, void *, float);
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern s32 rand(void);
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_00258278(void *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_00259d08(void *, void *, void *, void *, struct Moby *, Shot_309330 *, int);
extern void FUN_L00_0025a120(void *);
extern void approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void resolve_point_path_segment_xy_clearance(int, void *, void *, float) __asm__("FUN_L00_00261d78");
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
    enum {
        BLARG_SPACE_COMMANDO_INITIALIZE = 0,
        BLARG_SPACE_COMMANDO_FACE_TARGET_AND_SELECT_ACTION = 1,
        BLARG_SPACE_COMMANDO_ENTER_ACTION_SELECTION_AFTER_ANIMATION = 2,
        BLARG_SPACE_COMMANDO_SELECT_ACTION_AFTER_ANIMATION = 3,
        BLARG_SPACE_COMMANDO_FACE_TARGET_AND_WAIT_FOR_TIMER = 4,
        BLARG_SPACE_COMMANDO_FACE_TARGET_AND_PROBE_DURING_ANIMATION = 5,
        BLARG_SPACE_COMMANDO_RESUME_SAVED_STATE_AFTER_ANIMATION = 6,
        BLARG_SPACE_COMMANDO_APPROACH_TARGET_WITH_TURN_OFFSET = 7,
        BLARG_SPACE_COMMANDO_APPROACH_TARGET_UNTIL_BLOCKED_OR_NEAR = 11,
        BLARG_SPACE_COMMANDO_PROBE_DURING_ANIMATION_THEN_RESUME_APPROACH = 12,
        BLARG_SPACE_COMMANDO_RETURN_HOME = 13,
        BLARG_SPACE_COMMANDO_KNOCKBACK = 15,
        BLARG_SPACE_COMMANDO_DYING = 16
    };
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
    case BLARG_SPACE_COMMANDO_INITIALIZE:
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
    case BLARG_SPACE_COMMANDO_FACE_TARGET_AND_SELECT_ACTION:
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
    case BLARG_SPACE_COMMANDO_ENTER_ACTION_SELECTION_AFTER_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = 3;
            FUN_L13_00309370_c((char *)moby);
            if (moby->prev_seq != 5) {
                blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_SELECT_ACTION_AFTER_ANIMATION:
        if (moby->anim_flags & 2) {
            f32 dist = vector_distance_xy_alt(&moby->pos, &target->pos);
            if (vars->unk164 != 2 && dist < 15.0f) {
                moby->state = 4;
                if (moby->prev_seq != 4) {
                    blend_moby_animation(moby, 4, 0, scale_game_frames(20));
                }
                vars->timer = scale_game_frames(blarg_space_commando_face_target_wait_frames);
            } else {
                moby->state = 1;
                if (moby->prev_seq != 0) {
                    blend_moby_animation(moby, 0, 0, scale_game_frames(20));
                }
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_FACE_TARGET_AND_WAIT_FOR_TIMER:
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
    case BLARG_SPACE_COMMANDO_FACE_TARGET_AND_PROBE_DURING_ANIMATION:
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
    case BLARG_SPACE_COMMANDO_RESUME_SAVED_STATE_AFTER_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = moby->command;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_APPROACH_TARGET_WITH_TURN_OFFSET:
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
        resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
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
    case BLARG_SPACE_COMMANDO_APPROACH_TARGET_UNTIL_BLOCKED_OR_NEAR:
        blocked = 0;
        func_L00_002592B0_309330(moby,
                          fast_arc_tan_c(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), &vars->turn, 0.05f, 0.3f,
                          frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f);
        resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
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
    case BLARG_SPACE_COMMANDO_PROBE_DURING_ANIMATION_THEN_RESUME_APPROACH:
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
    case BLARG_SPACE_COMMANDO_RETURN_HOME:
        func_L00_002592B0_309330(moby,
                          fast_arc_tan_c(vars->home.f[0] - moby->pos.x, vars->home.f[1] - moby->pos.y), &vars->turn, 0.05f,
                          0.3f, frame_time * 6.2831855f);
        point.f[0] = 2.0f * fast_cos(moby->rot.z);
        point.f[1] = 2.0f * fast_sin(moby->rot.z);
        point.f[2] = 0.0f;
        if (fast_difference_between_rotations(moby->rot.z, fast_arc_tan_c(vars->home.f[0] - moby->pos.x,
                                                         vars->home.f[1] - moby->pos.y)) < 0.7853982f) {
            FUN_L00_00258b50(moby, &vars->ctl, &point, &origin, 1.0f);
            resolve_point_path_segment_xy_clearance(vars->path, &moby->pos, &moby->pos, 0.75f);
        }
        if (fast_vec_dist(&vars->home, &moby->pos) < 1.0f) {
            moby->state = 1;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case BLARG_SPACE_COMMANDO_KNOCKBACK:
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
    case BLARG_SPACE_COMMANDO_DYING:
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
#include "rnc/overlay/l13/gameplay/entities/rocket_data/update_blarg_space_commando_damage_targeting_and_ground_motion.h"
extern s32 *walker_paths_8d10[] __asm__("D_L13_001B07B0");
extern char D_L13_00160680[] __attribute__((section(".sdata")));
extern s32 D_001413D0_lui[] __asm__("D_001413D0") __attribute__((section(".data")));
extern CollisionHit coll_hit_8d10 __asm__("D_L13_00174140");
void update_gemlik_trail(struct Moby *moby) __asm__("FUN_L13_003094f8");

extern void update_gemlik_trail(struct Moby *) __asm__("FUN_L13_003094f8");
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
void update_blarg_space_commando_damage_targeting_and_ground_motion(struct Moby *moby) __asm__("FUN_L13_00308d10");

void update_blarg_space_commando_damage_targeting_and_ground_motion(struct Moby *moby) {
    enum {
        HIT_RESULT_NONE = 0,
        HIT_RESULT_DEATH = 1,
        HIT_RESULT_DYING_KNOCKBACK = 2,
        HIT_RESULT_STUN_KNOCKBACK = 3,
        HIT_RESULT_NO_REACTION = 11
    };
    enum {
        GROUND_MATERIAL_DESTROYS_WALKER = 0,
        GROUND_MATERIAL_TRIGGERS_WALKER_EXPLOSION = 1,
        GROUND_MATERIAL_EXPLODES_AND_REMOVES_WALKER = 3,
        GROUND_MATERIAL_KILLS_WALKER_WITH_EXPLOSION = 11,
        GROUND_MATERIAL_EXPLODES_WALKER = 13
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
    update_gemlik_trail(moby);
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
        case HIT_RESULT_NONE:
        case HIT_RESULT_NO_REACTION:
            break;
        case HIT_RESULT_STUN_KNOCKBACK:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            moby->state = 0xF;
            vars->stun_timer = scale_game_frames_8d10(0x3C);
            vars->knock_up = blarg_space_commando_stun_horizontal_knockback_speed * frame_time;
            vars->knock_back = blarg_space_commando_stun_vertical_knockback_speed * frame_time;
            vars->knock_min = 8.0f;
            vars->knock_max = 15.0f;
            knock_back_8d10(moby, vars->anim, 7, 1, 0, atan2_f_8d10(*(f32 *)(hit + 0x10), *(f32 *)(hit + 0x14)));
            vars->flash_time = 0xFA;
            break;
        case HIT_RESULT_DEATH:
        case HIT_RESULT_DYING_KNOCKBACK:
            moby->state = 0x10;
            moby->flags &= ~0x1000;
            vars->knock_up = blarg_space_commando_dying_horizontal_knockback_speed * frame_time;
            vars->knock_back = blarg_space_commando_dying_vertical_knockback_speed * frame_time;
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
        /* These material IDs all destroy the walker; their material names are unknown. */
        case GROUND_MATERIAL_DESTROYS_WALKER:
        case GROUND_MATERIAL_TRIGGERS_WALKER_EXPLOSION:
        case GROUND_MATERIAL_EXPLODES_AND_REMOVES_WALKER:
        case GROUND_MATERIAL_KILLS_WALKER_WITH_EXPLOSION:
        case GROUND_MATERIAL_EXPLODES_WALKER:
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
extern float gemlik_trail_spread_degrees_gp __asm__("D_L13_00161E80") __attribute__((sda));
int D_L13_00161E84 = 150;
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_vector_xyz(void *, void *, void *);
extern void normalize_vector_xyz(void *, void *, float);

void initialize_gemlik_trail(struct Moby *moby) __asm__("FUN_L13_00309370");

void initialize_gemlik_trail(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float point_offset[4];
    int i;
    probe_ground_height(&moby->pos, 0, 0.5f);
    normalize_vector_xyz(moby_vars + 0x200, D_L13_00174180, 1.0f);
    moby_point_position(moby, 0, moby_vars + 0x1F0);
    for (i = 0; i < 16; i++) {
        float a = (convert_integer_to_float(i) * 0.0625f - 0.5f) *
                  (gemlik_trail_spread_degrees_gp * DEG_TO_RAD);
        point_offset[0] = fast_cos(fast_add_rotations(a, moby->rot.z)) * 0.5f;
        point_offset[1] = fast_sin(fast_add_rotations(a, moby->rot.z)) * 0.5f;
        point_offset[2] = 0.0f;
        add_vector_xyz(moby_vars + 0x210 + i * 16, moby_vars + 0x1F0, point_offset);
        if (i == 0 || i == 15) {
            *(int *)(moby_vars + 0x21C + i * 16) = 0;
        } else {
            *(float *)(moby_vars + 0x21C + i * 16) = 1.0f;
        }
    }
    *(float *)(moby_vars + 0x1FC) = scale_game_frames(D_L13_00161E84);
}
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern float dot_vector_xyz(void *, void *) __asm__("FUN_001f9ab0");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void spawn_point_table_sparks(struct Moby *) __asm__("FUN_L13_00309bc8");
extern void draw_point_table_quads(struct Moby *) __asm__("FUN_L13_003098d0");
extern float gemlik_trail_point_radial_speed_gp __asm__("D_L13_00161E7C") __attribute__((sda));
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

void update_gemlik_trail(struct Moby *moby) {
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
        normalize_vector_xyz(&next, &next, gemlik_trail_point_radial_speed_gp * frame_time);
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
            make_hit_descriptor(&work.hit, moby, 0x10001, 1.0f, &next);
            work.hit.c = (unsigned short)moby->oclass;
            work.hit.a = 0;
            work.hit.b = 1;
            collision_line(&data->point[j], &data->point[j + 1], 0, moby, &work.hit);
        }
        j++;
    } while (j < 15);
    enqueue_callback_list_1((void *)draw_point_table_quads, moby);
    spawn_point_table_sparks(moby);
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
extern int check_scaled_vector_bsphere(float, void *) __asm__("FUN_L01_00275690");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void render_level13_water_grids(void) __asm__("FUN_L13_0030a460");
void initialize_gemlik_pool_tile_neighbors(int tile_index) __asm__("FUN_L13_0030a488");

extern void initialize_gemlik_pool_tile_neighbors(int) __asm__("FUN_L13_0030a488");

void update_gemlik_bobbing_pool_tile(struct Moby *moby) __asm__("FUN_L13_00309e88");

void update_gemlik_bobbing_pool_tile(struct Moby *moby) {
    enum {
        BOBBING_PLATFORM_INITIALIZE = 0,
        BOBBING_PLATFORM_UPDATE = 1
    };
    Bob_9e88 *bobbing_platform_vars;
    float height_half_span_or_tile_enabled;
    int i, j, k, tile_index;
    int cr, cg;

    struct Hero *player_hero = &hero;
    char *ship_moby = ((char *)player_hero->ship_moby);
    if (ship_moby != 0 && *(short *)(ship_moby + 0xA6) == 0x45 && (unsigned char)ship_moby[0x20] != 0xFE &&
        (unsigned char)ship_moby[0x20] != 0xFD && player_hero->state.current == 0x32)
        return;
    bobbing_platform_vars = (Bob_9e88 *)moby->pvars;
    if (bobbing_platform_vars == 0)
        return;
    switch (moby->state) {
    case BOBBING_PLATFORM_INITIALIZE: {
        float rate = frame_time;
        moby->flags |= 1;
        bobbing_platform_vars->angle = bobbing_platform_vars->angle * DEG_TO_RAD;
        bobbing_platform_vars->speed = bobbing_platform_vars->speed * DEG_TO_RAD * rate;
        if (bobbing_platform_vars->idx == 0) {
            Light_9e88 *tile_lighting;
            char *tb;
            float light_direction_angle;
            pool_init_tiles(Tiles_9e88, 22);
            pool_set_scale(1.0f);
            moby->update_dist = 0xFF;
            tile_lighting = &D_L13_001CAB80;
            tile_lighting->f00 = 16.0f;
            tile_lighting->f04 = 16.0f;
            tile_lighting->f08 = -8.0f;
            tile_lighting->f0C = -8.0f;
            tile_lighting->f10 = 1.0f;
            tile_lighting->f14 = 1.0f;
            tile_lighting->f18 = 0.9f;
            tile_lighting->f20 = 0.1f;
            light_direction_angle = fast_arc_tan(D_L13_001801C0[4], D_L13_001801C0[5]);
            tile_lighting->f30 = fast_cos(light_direction_angle) * 0.57735f;
            tile_lighting->f34 = fast_sin(light_direction_angle) * 0.57735f;
            tile_lighting->f38 = -0.57735f;
            tile_lighting->i28 = 0x2D;
            tile_lighting->i2C = 0x2E;
            tile_lighting->c3C = 0x60;
            tile_lighting->c3D = 0x40;
            tb = D_L13_001F1E80;
            gemlik_pool_tile_count = 22;
            D_L13_0016120C = 32768.0f;
            D_L13_00161210 = 255.0f;
            D_L13_00161214 = 48.0f;
            D_L13_001612D0 = Tiles_9e88;
            D_L13_001612D4 = tb;
            D_L13_00161208 = 0;
            for (i = 0; i < 22; i++) {
                pool_bind_tile(&Tiles_9e88[i], tb + i * 16);
                Tiles_9e88[bobbing_platform_vars->idx].c1E = D_L13_001F1E20[bobbing_platform_vars->idx];
            }
            for (k = 0; k < 22; k++) {
                for (j = 0; j < 2; j++) {
                    Tile_9e88 *t = &Tiles_9e88[k];
                    float x = t->x + rand_range_9e88(-6.0f, 6.0f);
                    float y = t->y + rand_range_9e88(-6.0f, 6.0f);
                    pool_ripple(x, y, 2.0f, 0.2f, Tiles_9e88, 22);
                }
            }
            for (tile_index = 0; tile_index < 22; tile_index++)
                initialize_gemlik_pool_tile_neighbors(tile_index);
        }
        moby->draw_dist = 0;
        moby->state = 1;
        break;
    }
    case BOBBING_PLATFORM_UPDATE: {
        struct Hero *hero_context;
        height_half_span_or_tile_enabled = bobbing_platform_vars->top;
        height_half_span_or_tile_enabled -= bobbing_platform_vars->bottom;
        height_half_span_or_tile_enabled *= 0.5f;
        moby->pos.z = bobbing_platform_vars->bottom + (fast_cos(bobbing_platform_vars->angle) * height_half_span_or_tile_enabled + height_half_span_or_tile_enabled);
        hero_context = &hero;
        if (hero_context->state.control_mode != 0x14 && hero_context->state.control_mode != 0x19 &&
            hero_context->state.current != 0x3D && hero_context->state.current != 0x7B &&
            hero_context->state.current != 0x7C)
            bobbing_platform_vars->angle = fast_add_rotations(bobbing_platform_vars->angle, bobbing_platform_vars->speed);
        Tiles_9e88[bobbing_platform_vars->idx].c1C = D_L13_001CAB80.c3C;
        Tiles_9e88[bobbing_platform_vars->idx].c1D = D_L13_001CAB80.c3D;
        if (random_below_9e88(200) == 0) {
            float x = Tiles_9e88[bobbing_platform_vars->idx].x + rand_range_9e88(-4.0f, 4.0f);
            float y = rand_range_9e88(-4.0f, 4.0f);
            Tile_9e88 *t = &Tiles_9e88[bobbing_platform_vars->idx];
            pool_splash(x, t->y + y, 1.0f, -0.05f, t, 1, 1);
        }
        break;
    }
    }
    if (bobbing_platform_vars->idx >= 0) {
        height_half_span_or_tile_enabled = 0.0f;
        (Tiles_9e88 + bobbing_platform_vars->idx)->z = moby->pos.z;
        if (check_scaled_vector_bsphere(48.0f, moby) != -1)
            height_half_span_or_tile_enabled = 1.0f;
        if (height_half_span_or_tile_enabled == 0.0f)
            Tiles_9e88[bobbing_platform_vars->idx].c1E = 0;
        else
            Tiles_9e88[bobbing_platform_vars->idx].c1E = D_L13_001F1E20[bobbing_platform_vars->idx];
        if (bobbing_platform_vars->idx == 0) {
            pool_flush(Tiles_9e88, 22);
            enqueue_callback_list_1(render_level13_water_grids, moby);
        }
        if (bobbing_platform_vars->idx == D_L13_00161EF0) {
            Tiles_9e88[bobbing_platform_vars->idx].c1E = D_L13_00161EF4;
            D_L13_001F1E20[bobbing_platform_vars->idx] = D_L13_00161EF4;
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

void initialize_gemlik_pool_tile_neighbors(int tile_index) {
    int i;
    D_L13_001D9BC0[tile_index].n[0] = 0xFF;
    D_L13_001D9BC0[tile_index].n[2] = 0xFF;
    D_L13_001D9BC0[tile_index].n[1] = 0xFF;
    D_L13_001D9BC0[tile_index].n[3] = 0xFF;
    D_L13_001D9BC0[tile_index].n[4] = 0xFF;
    D_L13_001D9BC0[tile_index].n[5] = 0xFF;
    D_L13_001D9BC0[tile_index].n[6] = 0xFF;
    D_L13_001D9BC0[tile_index].n[7] = 0xFF;
    for (i = 0; i < 22; i++) {
        if (tile_index != i) {
            Ent *tile_table = D_L13_001D9BC0;
            Ent *selected_tile = &tile_table[tile_index];
            float x = selected_tile->x - tile_table[i].x;
            float y = selected_tile->y - tile_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_tile->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_tile->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_tile->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_tile->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_tile->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_tile->n[6] = i;
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
void compute_lift_mount_transform(struct Moby *moby, float *mount_position, float *mount_rotation) __asm__("FUN_L13_0030ad38");

extern void compute_lift_mount_transform(struct Moby *, float *, float *) __asm__("FUN_L13_0030ad38");

void update_lift(struct Moby *moby) __asm__("FUN_L13_0030a6d8");

void update_lift(struct Moby *moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_BLEND_TO_MOUNT_TRANSFORM = 1,
        LIFT_MOVE_TO_TOP = 2,
        LIFT_MOVE_TO_BOTTOM = 3,
        LIFT_IDLE_AT_TOP = 4,
        LIFT_IDLE_AT_BOTTOM = 5,
        LIFT_WAIT_AT_TOP_FOR_HERO_TO_LEAVE = 6,
        LIFT_WAIT_AT_BOTTOM_FOR_HERO_TO_LEAVE = 7
    };
    LiftVarsA6D8 *vars = (LiftVarsA6D8 *)moby->pvars;
    float delta[4];
    float rot[4];
    float mount_position_work[4];
    float mount_transform_work[4];
    float mount_rotation[4];

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
    case LIFT_BLEND_TO_MOUNT_TRANSFORM: {
        float old = vars->blend;
        advance_accelerated_scalar(&vars->blend, &vars->blend_speed, 1.0f, D_0015ED70 * 4.0f, D_0015ED70 * 4.0f, D_0015ED6C * 4.0f);
        if (vars->blend < 1.0f) {
            float remaining_blend_ratio = (1.0f - vars->blend) / (1.0f - old);
            char *base;
            compute_lift_mount_transform(moby, mount_transform_work, mount_rotation);
            subtract_vector_xyz(mount_position_work, D_L13_001670C0, mount_transform_work);
            base = D_L13_001670C0 - 0x140;
            scale_vector_xyz(mount_position_work, mount_position_work, remaining_blend_ratio);
            add_vector_xyz(mount_transform_work, mount_transform_work, mount_position_work);
            mount_rotation[1] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x154), mount_rotation[1]) * remaining_blend_ratio, mount_rotation[1]);
            mount_rotation[2] = fast_add_rotations(fast_subtract_rotations(*(float *)(base + 0x158), mount_rotation[2]) * remaining_blend_ratio, mount_rotation[2]);
            FUN_L00_002ea9d8(mount_transform_work);
            FUN_L00_002eaa30(mount_rotation);
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
            compute_lift_mount_transform(moby, mount_position_work, mount_transform_work);
            FUN_L00_002ea9d8(mount_position_work);
            FUN_L00_002eaa30(mount_transform_work);
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
    case LIFT_WAIT_AT_TOP_FOR_HERO_TO_LEAVE:
    case LIFT_WAIT_AT_BOTTOM_FOR_HERO_TO_LEAVE:
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
    case LIFT_IDLE_AT_TOP:
    case LIFT_IDLE_AT_BOTTOM: {
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

void compute_lift_mount_transform(struct Moby *moby, float *mount_position, float *mount_rotation) {
    float mount_offset_angle, k;
    char *base;
    clear_u64_value(mount_rotation);
    k = 2.25f;
    if ((moby->command >= 2 && (moby->command & 1)) ||
        (moby->state >= 2 && (moby->state & 1))) {
        mount_offset_angle = -1.5707964f;
        mount_rotation[0] = moby->rot.y;
    } else {
        mount_offset_angle = 1.5707964f;
        mount_rotation[0] = -moby->rot.y;
    }
    mount_position[0] = fast_cos(mount_offset_angle) * k;
    mount_position[1] = fast_sin(mount_offset_angle) * k;
    mount_position[2] = 1.5f;
    transform_vector_by_basis(mount_position, mount_position, &moby->rot_row0);
    fast_vec_add(mount_position, mount_position, &moby->pos);
    base = D_0013E533 + 0xE1D;
    mount_rotation[2] = fast_arc_tan(*(float *)(base + 0xD0) - mount_position[0], *(float *)(base + 0xD4) - mount_position[1]);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_0030C320), where it is exact; names translated to the US level program. */

extern char *get_moby_suck_cannon_record(char *) __asm__("FUN_L00_002db890");
extern int D_L13_0015FFD8_m __asm__("D_L13_0015FFD8");
extern short *D_L13_001ABB40[];

int count_eligible_mobies_in_list(int moby_list_index, int excluded_moby_state) __asm__("FUN_L13_0030ae68");

int count_eligible_mobies_in_list(int moby_list_index, int excluded_moby_state) {
    short *moby_id_list = D_L13_001ABB40[moby_list_index];
    short *moby_id_cursor;
    int count = 0;
    if (moby_id_list == 0)
        return 0;
    moby_id_cursor = moby_id_list;
    do {
        int id = *moby_id_cursor & 0x7FFF;
        char *moby = (char *)(id << 8) + (int)D_L13_0015FFD8_m;
        if (moby[0x20] >= 0) {
            char *moby_callback_record = get_moby_suck_cannon_record(moby);
            if (moby_callback_record == 0 || *(short *)(moby_callback_record + 0x68) < 4) {
                if (excluded_moby_state == -1 ||
                    *(unsigned char *)((char *)(id << 8) + (int)D_L13_0015FFD8_m + 0x20) != excluded_moby_state) {
                    count++;
                }
            }
        }
    } while (*moby_id_cursor++ >= 0);
    return count;
}
/* Checks the moby's trigger lists and copies the configured state bytes into it. */
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_002EBD00.c: func_L13_0030C408), where it is exact; names translated to the US level program. */

extern int is_point_inside_clip_volume_alt(void *, int) __asm__("FUN_00214720");

void apply_configured_moby_state_and_command(struct Moby *moby) __asm__("FUN_L13_0030af50");

void apply_configured_moby_state_and_command(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int *moby_list_index_cursor = (int *)moby_vars;
    int i;
    char *base;
    for (i = 0; i < 8; i++) {
        if (*moby_list_index_cursor != -1) {
            if (count_eligible_mobies_in_list(*moby_list_index_cursor, -1) != 0)
                return;
            moby_list_index_cursor++;
        }
    }
    base = D_0013E533 + 0xE1D;
    if (*(int *)(base + 0x15F0) != 0 && *(int *)(base + 0x2084) == 0x32)
        return;
    if (*(int *)(moby_vars + 0x28) != -1 &&
        is_point_inside_clip_volume_alt(base + 0x80, *(int *)(moby_vars + 0x28)) == 0)
        return;
    if (*(int *)(moby_vars + 0x20) != -1)
        moby->state = moby_vars[0x20];
    if (*(int *)(moby_vars + 0x24) == -1)
        return;
    moby->command = moby_vars[0x24];
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
extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void FUN_L00_002607d0(int);
extern void initialize_moby_state_record(void *) __asm__("FUN_L00_0028e8a0");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");

void update_gemlik_scene_controller(struct Moby *moby) __asm__("FUN_L13_0030b628");

void update_gemlik_scene_controller(struct Moby *moby) {
    enum {
        WAIT_TO_START_INITIAL_SCENE = 0,
        START_INITIAL_SCENE = 1,
        WAIT_INITIAL_SCENE = 2,
        ENTER_CLIP_VOLUME_CHECK = 3,
        CHECK_CLIP_VOLUME_FOR_SCENE = 4,
        WAIT_CLIP_VOLUME_SCENE = 5,
        ENTER_LINKED_MOBY_REMOVAL_WAIT = 6,
        WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE = 7,
        WAIT_SCENE_BEFORE_FOLLOWUP = 8,
        START_FOLLOWUP_SCENE = 9,
        WAIT_FOLLOWUP_SCENE = 10,
        WAIT_LINKED_MOBY_REMOVAL_BEFORE_SAVE = 11,
        ENTER_LINKED_MOBY_CLEANUP_WAIT = 12,
        WAIT_TO_REMOVE_LINKED_MOBY = 13,
        FINALIZE_LINKED_MOBY_CLEANUP = 14
    };
    int *data = (int *)moby->pvars;
    switch (moby->state) {
    case WAIT_TO_START_INITIAL_SCENE:
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
    case WAIT_INITIAL_SCENE:
        if (D_L13_0015F5C4 != 2) moby->state = 3;
        break;
    case ENTER_CLIP_VOLUME_CHECK:
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
    case WAIT_CLIP_VOLUME_SCENE: {
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
    case ENTER_LINKED_MOBY_REMOVAL_WAIT:
        moby->state = 7;
        break;
    case WAIT_LINKED_MOBY_REMOVAL_BEFORE_SCENE: {
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
    case WAIT_SCENE_BEFORE_FOLLOWUP:
        if (D_L13_0015F5C4 != 2) {
            moby->state = 9;
            queue_scene_movie_with_game_language(14);
        }
        break;
    case START_FOLLOWUP_SCENE:
        if (D_L13_0015F5C4 != 2) {
            start_scene(3);
            moby->state = 10;
        }
        break;
    case WAIT_FOLLOWUP_SCENE: {
        if (D_L13_0015F5C4 != 2) {
            if (data[4] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[4] * 0x100);
                other->flags &= 0xfffd;
            }
            FUN_L00_002607d0(14);
            if (data[8] != -1) {
                struct Moby *other = (struct Moby *)(D_L13_0015FFD8_m + data[8] * 0x100);
                other->flags &= 0xffbe;
                initialize_moby_state_record(other);
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
    case WAIT_LINKED_MOBY_REMOVAL_BEFORE_SAVE: {
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
    case ENTER_LINKED_MOBY_CLEANUP_WAIT:
        moby->state = 13;
        break;
    case WAIT_TO_REMOVE_LINKED_MOBY:
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
    case FINALIZE_LINKED_MOBY_CLEANUP:
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
extern int compute_advanced_moby_path_position(void *, void *, void *, int, int, float) __asm__("FUN_L09_00294ae0");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void update_moby_path_follower(struct Moby *moby) __asm__("FUN_L13_0030bb70");

void update_moby_path_follower(struct Moby *moby) {
    int *moby_vars = (int *)moby->pvars;
    char *path;
    char *pth;
    char *target_moby;
    float nearest_path_position[4];
    int nearest_segment_index;
    float nearest_segment_progress;
    int i;
    if (moby->state == 0) {
        moby->update_dist = 0xFF;
        if (moby_vars[1] != -1) {
            path = D_L13_001B07B0[moby_vars[1]];
            for (i = 0; i < *(int *)path; i++) {
                *(float *)(path + (i << 4) + 0x1C) = FUN_001f9b48(
                    path + (i * 16 + 0x10), path + ((i + 1) % *(int *)path * 16 + 0x10));
            }
        }
        moby_vars[2] = 0;
        moby_vars[3] = 0;
        moby->state = 1;
        moby_vars[4] = 0;
    }
    if (moby_vars[0] == -1)
        return;
    target_moby = (char *)(D_L13_0015FFD8_m + (moby_vars[0] << 8));
    if (target_moby == 0 || ((unsigned char *)target_moby)[0x20] == 0xFE || ((unsigned char *)target_moby)[0x20] == 0xFD) {
        mark_moby_for_removal(moby);
        return;
    }
    if (((unsigned char *)target_moby)[0x20] >= 7) {
        moby_vars[4]++;
        if (moby_vars[1] == -1)
            return;
        pth = D_L13_001B07B0[moby_vars[1]];
        if (moby->state == 1) {
            FUN_L00_0025df68(pth, target_moby + 0x10, nearest_path_position, &nearest_segment_index, &nearest_segment_progress, 0, 100.0f, 1.0f, 0.0f);
            if (nearest_segment_index > moby_vars[2] || nearest_segment_progress > ((float *)moby_vars)[3] || moby_vars[4] < 0x1E) {
                moby_vars[2] = nearest_segment_index;
                ((float *)moby_vars)[3] = nearest_segment_progress;
                qcopy(&moby->pos, nearest_path_position);
            } else {
                moby->state = 2;
            }
        }
        if (moby->state == 2) {
            if (moby_vars[2] < *(int *)pth - 1) {
                int k = compute_advanced_moby_path_position(moby, pth, &moby->pos, moby_vars[2], 1, frame_scale * 0.25f);
                moby_vars[2] = k;
                ((float *)moby_vars)[3] = FUN_001f9b48(&moby->pos, pth + (k * 16 + 0x10));
            } else {
                moby->state = 3;
            }
        }
    } else {
        qcopy(&moby->pos, target_moby + 0x10);
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

void update_cutscene_target_effects(struct Moby *moby) __asm__("FUN_L13_0030bdf0");

void update_cutscene_target_effects(struct Moby *moby) {
    enum {
        INITIALIZE_UPDATE_DISTANCE = 0,
        UPDATE_CUTSCENE_CAMERA_TARGET = 1
    };
    switch (moby->state) {
    case INITIALIZE_UPDATE_DISTANCE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case UPDATE_CUTSCENE_CAMERA_TARGET:
        if (D_L13_0015F5C4 == 2 && D_L13_0016CB60.state == 2) {
            cutscene_set_camera_target(D_L13_0016CB60.target);
        }
        break;
    }
}
/* Records the classes of the tracked mobys, then removes this moby once too few of them are left. */
void remove_moby_when_tracked_count_below_threshold(struct Moby *moby) __asm__("FUN_L13_0030be60");

void remove_moby_when_tracked_count_below_threshold(struct Moby *moby) {
    int *moby_vars = (int *)moby->pvars;
    if (moby->state == 0) {
        int *ids = moby_vars + 4;
        int *cls = moby_vars + 12;
        char *base;
        int i;
        moby->state = 1;
        base = (char *)D_L13_0015FFD8_m;
        for (i = 0; i < 8; i++) {
            unsigned char *o = (unsigned char *)(base + (ids[i] << 8));
            if (o != 0 && o[0x20] != 0xFE && o[0x20] != 0xFD)
                cls[i] = *(short *)(o + 0xA6);
            else
                cls[i] = -1;
        }
        if (moby_vars[0] == 0)
            moby->state = 2;
    } else if (moby->state == 1) {
        int *ids = moby_vars + 4;
        int *cls = moby_vars + 12;
        char *base = (char *)D_L13_0015FFD8_m;
        int valid_tracked_moby_count = 0;
        int i;
        for (i = 0; i < 8; i++) {
            int id = ids[i];
            int saved_moby_class;
            unsigned char *o;
            if (id != -1 && (saved_moby_class = cls[i]) != -1) {
                o = (unsigned char *)(base + (id << 8));
                if (o != 0 && *(short *)(o + 0xA6) == saved_moby_class && o[0x20] != 0xFE && o[0x20] != 0xFD) {
                    valid_tracked_moby_count++;
                    continue;
                }
            }
            cls[i] = -1;
        }
        if (valid_tracked_moby_count < moby_vars[0])
            mark_moby_for_removal(moby);
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D468), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern void FUN_L00_00250df8(void *);

char *spawn_scaled_owned_moby(char *owner, void *position, float scale) __asm__("FUN_L13_0030bfb0");

char *spawn_scaled_owned_moby(char *owner, void *position, float scale) {
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
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
void mark_moby_for_removal_u(struct Obj *obj) __asm__("FUN_0020c828");

void forward_moby_hits_to_owner(struct Moby *moby) __asm__("FUN_L13_0030c060");

void forward_moby_hits_to_owner(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *owner = *(char **)(data + 0x60);
    char *hit_record;
    char forwarded_hit_descriptor[0x30];
    if (owner == 0 || *(short *)(owner + 0xA6) != *(int *)(data + 0x64) ||
        (unsigned char)owner[0x20] == 0xFE || (unsigned char)owner[0x20] == 0xFD) {
        mark_moby_for_removal_u(moby);
        return;
    }
    hit_record = find_moby_hit(moby, -1, 0);
    if (hit_record != 0) {
        qcopy(forwarded_hit_descriptor, hit_record + 0x10);
        *(int *)(forwarded_hit_descriptor + 0x10) = *(int *)(hit_record + 0x20);
        *(int *)(forwarded_hit_descriptor + 0x14) = *(int *)(hit_record + 0x24);
        forwarded_hit_descriptor[0x18] = hit_record[0x28];
        forwarded_hit_descriptor[0x19] = hit_record[0x29];
        *(short *)(forwarded_hit_descriptor + 0x1A) = *(short *)(hit_record + 0x2A);
        *(float *)(forwarded_hit_descriptor + 0x1C) = *(float *)(hit_record + 0x2C);
        *(int *)(forwarded_hit_descriptor + 0x20) = *(int *)(hit_record + 0x30);
        record_moby_hit(*(void **)(data + 0x60), forwarded_hit_descriptor);
    }
    moby->hit_slot = 0xFF;
}
/* Ported from rac1-decomp (src/overlays/l13_gemlik/vendor_0030CAE0.c: func_L13_0030D600), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L00_00250df8(void *);

char *spawn_poison_vessel_effect_moby(void *position, void *initial_velocity, int duration_frames, int effect_mode) {
    unsigned char *moby = (unsigned char *)func_0020D348_m(0x662);
    if (moby != 0) {
        char *data;
        float rotation_x_increment, dd;
        data = *(char **)(moby + 0x78);
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x23] = random_int_between(0x40, 0x80);
        set_moby_color(moby, 0x20, 0x60, 0x10);
        qcopy(moby + 0x10, position);
        qcopy(data, initial_velocity);
        rotation_x_increment = random_float_between_alt(frame_time * 6.2831855f, frame_time * 12.566371f);
        dd = frame_time;
        *(float *)(data + 0x10) = rotation_x_increment;
        *(float *)(data + 0x14) = random_float_between_alt(dd * 6.2831855f, dd * 12.566371f);
        *(short *)(data + 0x18) = duration_frames;
        *(short *)(data + 0x1A) = duration_frames;
        if (effect_mode == 0) {
            *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * random_float_between_alt(0.5f, 0.75f);
        }
        *(float *)(data + 0x1C) = *(float *)(moby + 0x2C);
        *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * random_float_between_alt(2.0f, 5.0f);
        moby[0xBC] = effect_mode;
        FUN_L00_00250df8(moby);
    }
    return (char *)moby;
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
#include "rnc/overlay/l13/gameplay/entities/rocket_data/update_moby_motion_and_emit_sparks_4.h"
extern float D_L13_00161F2C __attribute__((sda));
#include "rnc/overlay/l13/gameplay/entities/rocket_data/update_moby_motion_and_emit_sparks_3.h"
extern float D_L13_00161F40 __attribute__((sda));
#include "rnc/overlay/l13/gameplay/entities/rocket_data/update_moby_motion_and_emit_sparks_2.h"
extern float moby_spark_first_batch_spread_degrees_gp __asm__("D_L13_00161F50") __attribute__((sda));
extern float moby_spark_second_batch_spread_degrees_gp __asm__("D_L13_00161F54") __attribute__((sda));
#include "rnc/overlay/l13/gameplay/entities/rocket_data/update_moby_motion_and_emit_sparks.h"
extern int D_L13_00161F08 __attribute__((sda));
extern int D_L13_00161F0C __attribute__((sda));
extern int D_L13_00161F10 __attribute__((sda));
extern int D_L13_00161F14 __attribute__((sda));

typedef union {
    OvlQuad quad;
    float f[4];
} L13EffectVector;

#include "qzero.h"

void update_moby_motion_and_emit_sparks(struct Moby *moby) __asm__("FUN_L13_0030c3b8");

void update_moby_motion_and_emit_sparks(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    L13EffectVector old_pos, position, velocity, acceleration, second_pos;
    float angle, direction, amount;
    int spark_frame_count, second_spark_frame_count, third_spark_frame_count;
    int i;
    qcopy(&old_pos, &moby->pos);
    if (moby->command == 1) {
        velocity.f[3] = moby_spark_first_batch_discarded_velocity_w_initializer_gp;
        acceleration.f[3] = moby_spark_first_batch_acceleration_w_initializer_gp;
        i = 0;
        do {
            qcopy(&position, &moby->pos);
            position.f[0] += random_float_between_alt(-0.065f, 0.065f);
            position.f[1] += random_float_between_alt(-0.065f, 0.065f);
            position.f[2] += random_float_between_alt(-0.065f, 0.065f);
            qzero(&velocity);
            i++;
            velocity.f[2] = random_float_between_alt(moby_spark_first_batch_vertical_velocity_base_gp * frame_time * 0.9f,
                                                       moby_spark_first_batch_vertical_velocity_base_gp * frame_time * 1.1f);
            angle = random_float_between_alt(-moby_spark_first_batch_spread_degrees_gp, moby_spark_first_batch_spread_degrees_gp) * 0.017453292f;
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[0] = fast_cos(direction) * (moby_spark_first_batch_horizontal_acceleration_gp * frame_time);
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[1] = fast_sin(direction) * (moby_spark_first_batch_horizontal_acceleration_gp * frame_time);
            acceleration.f[2] = 0.0f;
            acceleration.f[2] = random_float_between_alt(moby_spark_first_batch_vertical_acceleration_base_gp * frame_time * 0.9f,
                                                          moby_spark_first_batch_vertical_acceleration_base_gp * frame_time * 1.1f);
            spark_frame_count = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(moby_spark_first_batch_frame_count_base_gp,
                                                                     moby_spark_first_batch_frame_count_base_gp + moby_spark_first_batch_frame_count_base_gp)));
            second_spark_frame_count = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(moby_spark_first_batch_second_frame_count_base_gp,
                                                                     moby_spark_first_batch_second_frame_count_base_gp * 1.2f)));
            third_spark_frame_count = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(moby_spark_first_batch_third_frame_count_base_gp,
                                                                     moby_spark_first_batch_third_frame_count_base_gp * 1.2f)));
            spawn_spark(&position, &velocity, &acceleration, D_L13_00161F08,
                           D_L13_00161F0C, spark_frame_count, second_spark_frame_count, third_spark_frame_count, -1);
        } while (i < 2);
        amount = 0.017453292f;
        velocity.f[3] = moby_spark_second_batch_discarded_velocity_w_initializer_gp;
        acceleration.f[3] = moby_spark_second_batch_acceleration_w_initializer_gp;
        i = 1;
        do {
            qcopy(&second_pos, &moby->pos);
            second_pos.f[0] += random_float_between_alt(-0.1f, 0.1f);
            second_pos.f[1] += random_float_between_alt(-0.1f, 0.1f);
            second_pos.f[2] += random_float_between_alt(-0.1f, 0.1f);
            random_float_between_alt(-moby_spark_second_batch_discarded_random_bound_gp, moby_spark_second_batch_discarded_random_bound_gp);
            qzero(&velocity);
            i--;
            velocity.f[2] = random_float_between_alt(moby_spark_second_batch_vertical_velocity_base_gp * frame_time * 0.7f,
                                                       moby_spark_second_batch_vertical_velocity_base_gp * frame_time * 1.5f);
            angle = random_float_between_alt(-moby_spark_second_batch_spread_degrees_gp, moby_spark_second_batch_spread_degrees_gp) * amount;
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)),
                                      angle);
            acceleration.f[0] = fast_cos(direction) * (moby_spark_second_batch_horizontal_acceleration_gp * frame_time);
            direction = fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)),
                                      angle);
            acceleration.f[1] = fast_sin(direction) * (moby_spark_second_batch_horizontal_acceleration_gp * frame_time);
            acceleration.f[2] = 0.0f;
            acceleration.f[2] = random_float_between_alt(moby_spark_second_batch_vertical_acceleration_base_gp * frame_time * 0.7f,
                                                          moby_spark_second_batch_vertical_acceleration_base_gp * frame_time * 1.5f);
            spark_frame_count = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(moby_spark_second_batch_frame_count_base_gp,
                                                                     moby_spark_second_batch_frame_count_base_gp + moby_spark_second_batch_frame_count_base_gp)));
            second_spark_frame_count = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(moby_spark_second_batch_second_frame_count_base_gp,
                                                                     moby_spark_second_batch_second_frame_count_base_gp * 1.2f)));
            third_spark_frame_count = FUN_001fa6d0(FUN_001f96b0_c3b8(random_float_between_alt(moby_spark_second_batch_third_frame_count_base_gp,
                                                                     moby_spark_second_batch_third_frame_count_base_gp * 1.2f)));
            spawn_spark(&second_pos, &velocity, &acceleration, D_L13_00161F10,
                           D_L13_00161F14, spark_frame_count, second_spark_frame_count, third_spark_frame_count, -1);
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
extern int count_active_moby_group_members(int, int) __asm__("FUN_L01_0026e008");
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void spawn_proximity_gated_bolts(void *) __asm__("FUN_L01_002787a0");
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void FUN_0020c828(void *);

#include "qzero.h"
extern s32 D_L13_001BA7D0[];
void blarg_ship_update(struct Moby *moby) __asm__("FUN_L13_0030cdb8");
void blarg_ship_update(struct Moby *moby) {
    enum {
        BLARG_SHIP_ENTER_DAMAGE_HANDLING = 0,
        BLARG_SHIP_HANDLE_DAMAGE = 1,
        BLARG_SHIP_EXPLODE = 2
    };
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
    case BLARG_SHIP_ENTER_DAMAGE_HANDLING:
        moby->state = 1;
        break;
    case BLARG_SHIP_HANDLE_DAMAGE:
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
    case BLARG_SHIP_EXPLODE: {
        void *pos;
        void *rot;
        void *zero_vector;
        if (moby->group != 0xff && count_active_moby_group_members(moby->group, -1) == 1 && D_0013D408[0x16] == 0) {
            D_0013D408[0x16] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        allocate_voice_for_target_entry(0, 0, moby);
        pos = &moby->pos;
        rot = &moby->rot;
        spawn_proximity_gated_bolts(moby);
        zero_vector = D_L13_0015F580;
        D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
            1 << (moby->save_id & 31);
        D_L13_001BA7D0[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
        for (j = 0; j < 2; j++) {
            for (i = 0x709; i < 0x70d; i++) {
                qzero(&vector);
                add_random_offset_to_vector_xyz(&vector, 1.5f);
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
