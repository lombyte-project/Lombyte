/* Level 04 moby classes: snow_plow, floating_platform, transport_ship, woodcut_bot, leaf, upper_door, suck_cannon. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* UpdateMoby_584 */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D4960), where it is exact; names translated to the US level program. */

extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_L00_0025d1b8(void *);
extern void FUN_L00_00260738(char *, void *, void *, void *);

void FUN_L04_002d3580(char *m) {
    char *d = *(char **)(m + 0x78);
    float v[4];
    FUN_001f9a28(v, m + 0x10, d + 0xA0);
    FUN_L00_00260738(d + 0x60, v, m + 0x40, m + 0x40);
    qcopy(d + 0xA0, m + 0x10);
    if (*(unsigned char *)(m + 0x20) == 0) {
        *(int *)(d + 0x20) = 0;
        *(short *)(d + 0x24) = 0;
        d[0x28] = 4;
        *(short *)(d + 0x3E) = 0xD;
        FUN_L00_0025d1b8(m);
        m[0x20] = 1;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L04_002ca420.s", FUN_L04_002ca420);
#else
extern int D_L04_0015FFD8;
extern int *D_L04_001B0630[];
extern char *D_L04_001600EC;
extern char D_L04_001DBE10[];
extern char D_L04_00166F40[];
extern float D_L04_0015F580[] __attribute__((section(".sdata")));
extern int D_L04_0015EDF8 __attribute__((sda));
extern unsigned char D_0013D40E;
extern void debug_print() __asm__("FUN_001e93b0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int FUN_001f96f8(int);
extern int FUN_001f9740(int *);
extern void fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float FUN_001f9b48(void *, void *);
extern float FUN_001f9b80(void *, void *);
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void clear_vector(void *) __asm__("FUN_001f99f8");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void FUN_L01_002f5168(void *);
extern void FUN_L00_00258278(void *, float, void *, float, float, float);
extern void attach_manipulator(char *, int, void *) __asm__("FUN_0020cb10");
extern void FUN_L00_001fff28(void *, int, float);
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void FUN_L00_00250df8(void *);
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int allocate_voice_for_target_entry(int, int, struct Moby *) __asm__("FUN_0022da68");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0025f090(void *, void *, int, float, float);
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

/* Class-dependent movement, manipulator rotation, hit effects and visibility. */
void FUN_L04_002ca420(struct Moby *moby) {
    Vec4 old;
    Vec4 delta;
    struct {
        Vec4 vector;
        struct Moby *moby;
        int flags;
        int unused;
        float scale;
        int zero;
    } sphere, sphere2;
    char *data = (char *)moby->pvars;
    unsigned char state = moby->state;
    int i, *p, *target;
    float amount, length;
    int timer;
    qcopy(&old, &moby->pos);
    if (!data) return;
    if (!state) {
        if (*(int *)((int)(data + 0x60) + 0x14) != -1) {
            int *p;
            p = D_L04_001B0630[*(int *)((int)(data + 0x60) + 0x14)];
            if (FUN_001f9b48(p + 4, p + *p * 4) < 0.5f) --*p;
        } else {
            char *message = D_L04_001DBE10;
            int oclass = moby->oclass;
            timer = truncate_float_to_s32((float)(((int)moby - D_L04_0015FFD8) >> 8));
            debug_print(message, oclass, timer);
        }
        moby->draw_dist = 0x7f;
        moby->update_dist = 0xff;
        *(float *)(data + 0x138) = *(float *)(data + 0xfc);
        *(int *)(data + 0x178) = moby->draw_dist;
    }
    FUN_L01_002f5168(moby);
    if (moby->oclass != 0x1e4) {
        fast_dec_timer_s16(data + 0x14a);
        switch (*(unsigned char *)(data + 0x149)) {
        case 0: {
            float length;
            length = *(float *)(data + 0xfc);
            amount = *(float *)(data + 0x138);
            if (length < amount) {
                *(float *)(data + 0xfc) = length + frame_time_sq * 20.0f;
                if (amount < *(float *)(data + 0xfc)) *(float *)(data + 0xfc) = amount;
            }
            target = (int *)(data + 0x150);
            p = target;
            i = 0;
            do {
                if (*(short *)(data + 0x14a) == 0 && *p != -1 &&
                    is_point_inside_clip_volume(&moby->pos, *p)) {
                    qcopy(data + 0x160, &moby->pos);
                    i = *(unsigned char *)target;
                    *(unsigned char *)(data + 0x149) = 1;
                    *(unsigned char *)(data + 0x148) = i;
                    *(int *)(data + 0x7c) = 0;
                    *(float *)(data + 0x14c) = moby->rot.z;
                    moby->state = 3;
                    *(int *)(data + 0x11c) = FUN_001f96f8(36000);
                    break;
                }
                ++i; ++p; ++target;
            } while (i < 4);
            break;
        }
        case 1:
            FUN_L00_00258278(moby, *(float *)(D_L04_001600EC + *(unsigned char *)(data + 0x148) * 0x80 + 0x78),
                data + 0x7c, frame_time_sq * 1.0471976f, frame_time_sq * 3.1415927f, frame_time * 3.1415927f);
            if (FUN_001f9b80(&moby->pos, D_L04_001600EC + *(unsigned char *)(data + 0x148) * 0x80 + 0x30) < 1.0f &&
                fast_difference_between_rotations(moby->rot.z, *(float *)(D_L04_001600EC + *(unsigned char *)(data + 0x148) * 0x80 + 0x78)) < 0.17453292f) {
                *(unsigned char *)(data + 0x149) = 2;
                *(short *)(data + 0x14a) = FUN_001f96f8(300);
            }
            FUN_001f9a28(&delta, D_L04_001600EC + *(unsigned char *)(data + 0x148) * 0x80 + 0x30, &moby->pos);
            normalize_vector_xyz(&delta, &delta, frame_time * 3.0f);
            add_vector_xyz(&moby->pos, &moby->pos, &delta);
            break;
        case 2:
            if (*(short *)(data + 0x14a) == 0) {
                *(float *)(data + 0x7c) = 0.0f;
                *(unsigned char *)(data + 0x149) = 3;
            }
            break;
        case 3:
            FUN_L00_00258278(moby, *(float *)(data + 0x14c), data + 0x7c,
                frame_time_sq * 1.0471976f, frame_time_sq * 3.1415927f, frame_time * 3.1415927f);
            if (FUN_001f9b80(&moby->pos, data + 0x160) < 1.0f &&
                fast_difference_between_rotations(moby->rot.z, *(float *)(data + 0x14c)) < 0.034906585f) {
                *(unsigned char *)(data + 0x149) = 0;
                *(short *)(data + 0x14a) = FUN_001f96f8(600);
                *(float *)(data + 0x7c) = 0.0f;
                *(float *)(data + 0xfc) = 0.0f;
                *(int *)(data + 0x11c) = 0;
            }
            FUN_001f9a28(&delta, data + 0x160, &moby->pos);
            length = fast_vec_length(&delta);
            amount = frame_time * 3.0f;
            if (!(length < amount)) length = amount;
            normalize_vector_xyz(&delta, &delta, length);
            add_vector_xyz(&moby->pos, &moby->pos, &delta);
            break;
        }
    }
    if (!state) {
        switch ((short)(moby->oclass - 0x1d2)) {
        case 0: case 0xe: case 0x13: case 0x14: case 0x1c: case 0x1d: case 0x20: case 0x59:
            attach_manipulator((char *)moby, 0, data + 0x180);
            *(int *)(data + 0x140) = 0;
            break;
        case 0x16: case 0x1b:
            attach_manipulator((char *)moby, 0, data + 0x180);
            *(int *)(data + 0x140) = 0;
            attach_manipulator((char *)moby, 1, data + 0x1c0);
            *(int *)(data + 0x144) = 0;
            break;
        }
    } else {
        switch ((short)(moby->oclass - 0x1d2)) {
        case 0: case 0xe:
            *(float *)(data + 0x140) = fast_add_rotations(*(float *)(data + 0x140), frame_time * 12.566371f);
            FUN_L00_001fff28(data + 0x190, 0, *(float *)(data + 0x140));
            break;
        case 0x13: case 0x14: case 0x1c: case 0x1d: case 0x20: case 0x59:
            *(float *)(data + 0x140) = fast_add_rotations(*(float *)(data + 0x140), frame_time * 12.566371f);
            FUN_L00_001fff28(data + 0x190, 2, *(float *)(data + 0x140));
            break;
        case 0x16: case 0x1b:
            *(float *)(data + 0x140) = fast_add_rotations(*(float *)(data + 0x140), frame_time * 12.566371f);
            FUN_L00_001fff28(data + 0x190, 2, *(float *)(data + 0x140));
            *(float *)(data + 0x144) = fast_add_rotations(*(float *)(data + 0x144), frame_time * 12.566371f);
            FUN_L00_001fff28(data + 0x1d0, 2, *(float *)(data + 0x144));
            break;
        }
    }
    if (*(int *)(data + 0x17c) != -1) {
        struct Moby *other = (struct Moby *)(D_L04_0015FFD8 + (*(int *)(data + 0x17c) << 8));
        other->draw_dist = 0x7f;
        if (moby->oclass == 0x1e0) {
            delta.q = 0;
            delta.f[0] = 0.5f;
            delta.f[2] = -5.0f;
            transform_vector_by_basis(&delta, &delta, &moby->rot_row0);
            add_vector_xyz(&other->pos, &moby->pos, &delta);
            qcopy(&other->rot, &moby->rot);
            other->rot.y = fast_add_rotations(other->rot.y, 1.5707964f);
        } else if (moby->oclass == 0x1ee) {
            delta.q = 0;
            delta.f[2] = -2.0f;
            transform_vector_by_basis(&delta, &delta, &moby->rot_row0);
            add_vector_xyz(&other->pos, &moby->pos, &delta);
            qcopy(&other->rot, &moby->rot);
        } else goto hit;
        advance_moby_animation(other);
        FUN_L00_00250df8(other);
        refresh_moby_spatial_bounds_from_basis(other);
        other->flags |= 6;
    }
hit:
    if (find_moby_hit(moby, 0x210000, 0)) {
        if (++D_L04_0015EDF8 >= 10 && !D_0013D40E) {
            D_0013D40E = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        switch ((short)(moby->oclass - 0x1d2)) {
        case 0x13: case 0x14:
            sphere.flags = 0x810000;
            sphere.scale = 1.0f;
            sphere.moby = moby;
            clear_vector(&sphere.vector);
            sphere.zero = 0;
            collect_mobies_in_sphere(&moby->pos, 4.0f, 0x10, moby, &sphere);
        case 0: case 0xe: case 0x16: case 0x18: case 0x1c: case 0x1d: case 0x20: case 0x59:
            delta.q = 0;
            delta.f[2] = 0.75f;
            clear_vector(&sphere.vector);
            add_vector_xyz(&delta, &delta, &moby->pos);
            func_L00_0025F4A8_alt(moby, &sphere.vector, &delta, 0.0f, 0.0f, 10, 3, 4, 2.0f, 1.0f,
                100000.0f, 3.0f, -1, 15.0f, 1, 1, -1, 0);
            if (moby->oclass == 0x1f2 || moby->oclass == 0x22b) allocate_voice_for_target_entry(0, 0, moby);
            else if (moby->oclass != 0x1e6) allocate_voice_for_target_entry(1, 0, moby);
            *(int *)(data + 0x170) = FUN_001f96f8(600);
            *(int *)(data + 0x174) = 1;
            break;
        case 0x1b:
            sphere.vector.q = 0;
            sphere.vector.f[2] = 0.75f;
            clear_vector(&sphere.moby);
            moby_point_position(moby, 0, &delta);
            add_vector_xyz(&delta, &delta, &sphere.vector);
            FUN_L00_0025f090(moby, &delta, -1, 1.0f, 13.0f);
            moby_point_position(moby, 1, &delta);
            add_vector_xyz(&delta, &delta, &sphere.vector);
            FUN_L00_0025f090(moby, &delta, -1, 1.0f, 13.0f);
            add_vector_xyz(&delta, &sphere.vector, &moby->pos);
            func_L00_0025F4A8_alt(moby, &sphere.moby, &delta, 0.0f, 0.0f, 20, 5, 4, 2.0f, 1.0f,
                100000.0f, 3.0f, -1, 15.0f, 1, 1, -1, 0);
            allocate_voice_for_target_entry(1, 0, moby);
            sphere2.flags = 0x810000;
            sphere2.scale = 2.0f;
            sphere2.moby = moby;
            clear_vector(&sphere2.vector);
            sphere2.zero = 0;
            collect_mobies_in_sphere(&moby->pos, 6.0f, 0x10, moby, &sphere2);
            *(int *)(data + 0x170) = FUN_001f96f8(600);
            *(int *)(data + 0x174) = 1;
            break;
        }
        scale_vector_xyz(&delta, &moby->rot_row0, frame_scale * 0.075f);
        delta.f[2] += frame_scale * 0.08f;
        if (moby->oclass == 0x1d2) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x63f, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x641, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x642, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
        }
        else if (moby->oclass == 0x1e0) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x642, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x643, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x644, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
        }
        else if (moby->oclass == 0x1e6) {
            int i;
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x645, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            i = 5;
            do {
                --i;
                timer = FUN_001f96f8(90);
                FUN_L00_00263fd8(moby, 0x646, &moby->pos, &moby->rot, timer, 0,
                    frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            } while (i >= 0);
        }
        else if (moby->oclass == 0x1ea) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x647, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x648, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x649, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
        }
        else if (moby->oclass == 0x1ee) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x64d, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x64e, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
        }
        else if (moby->oclass == 0x1ef) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x64f, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x650, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x651, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x658, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
        }
        else if (moby->oclass == 0x1f2) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x652, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x653, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x654, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
        }
        else if (moby->oclass == 0x1ed) {
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x793, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            timer = FUN_001f96f8(90);
            FUN_L00_00263fd8(moby, 0x794, &moby->pos, &moby->rot, timer, 0,
                frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            i = 2;
            do {
                --i;
                timer = FUN_001f96f8(90);
                FUN_L00_00263fd8(moby, 0x795, &moby->pos, &moby->rot, timer, 0,
                    frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            } while (i >= 0);
            i = 2;
            do {
                --i;
                timer = FUN_001f96f8(90);
                FUN_L00_00263fd8(moby, 0x796, &moby->pos, &moby->rot, timer, 0,
                    frame_time_sq * 12.0f, &delta, D_L04_0015F580, D_L04_0015F580);
            } while (i >= 0);
        }
    }
    moby->hit_slot = 0xff;
    if (*(int *)(data + 0x174) == 0) return;
    if (FUN_001f9740((int *)(data + 0x170))) {
        amount = fast_arc_tan(moby->pos.x - *(float *)(D_L04_00166F40), moby->pos.y - *(float *)(D_L04_00166F40 + 4));
        if (1.5707964f < fast_difference_between_rotations(*(float *)(D_L04_00166F40 + 0x18), amount)) {
            moby->collision = moby->pclass->collision;
            moby->flags &= 0xffbe;
            *(int *)(data + 0x174) = 0;
            if (*(int *)(data + 0x17c) != -1) {
                struct Moby *other = (struct Moby *)(D_L04_0015FFD8 + (*(int *)(data + 0x17c) << 8));
                other->flags &= 0xffbe;
            }
            moby->flags |= 0x1000;
            return;
        }
    }
    moby->collision = 0;
    moby->flags |= 0x41;
    if (*(int *)(data + 0x17c) != -1) {
        struct Moby *other = (struct Moby *)(D_L04_0015FFD8 + (*(int *)(data + 0x17c) << 8));
        other->flags |= 0x41;
    }
    moby->flags &= 0xefff;
}
#endif

#include "qcopy.h"

/* Floating platform: bobs on a slow wave above its start height (or above the moby it rides), and spins
 * its three rings. */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002CF180), where it is exact; names translated to the US level program. */

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_normalize_angle(f32) __asm__("func_001FA610");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern int D_L04_0015FFD8; /* no foreign declaration */
extern short D_L04_0016195C __attribute__((sda));
extern void FUN_L00_00260738(char *, void *, void *, void *);
extern void FUN_L00_002628d8(char *, char *, int, float, float);
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");

void FUN_L04_002cdda0(char *moby) {
    float old[4];
    float delta[4];
    char *data = *(char **)(moby + 0x78);
    qcopy(old, moby + 0x10);
    if (data == 0) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case 0: {
        int i;
        *(float *)(moby + 0x58) = random_float_between(0.5f, 0.8f);
        moby[0x20] = 1;
        *(float *)(data + 0x220) = *(float *)(moby + 0x18) + 15.0f;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(data + 0x3E) = 0xD;
        for (i = 0; i < 3; i++) {
            ((float *)(data + 0x224))[i] = random_angle_radians();
        }
        break;
    }
    case 1: {
        float off = 0.0f;
        float bob = fast_cos(fast_normalize_angle(compute_interpolated_record_value(moby) * 6.2831855f / 118.0f)) * 0.75f;
        int i;
        for (i = 0; i < 3; i++) {
            float *p = (float *)(data + 0x224) + i;
            float a = fast_add_rotations(*p, *(float *)&D_L04_0016195C * DEG_TO_RAD * frame_time);
            float g = frame_scale_sq;
            *p = a;
            *(float *)(data + 0x108 + i * 0x80) = a;
            FUN_L00_002628d8(moby, data + 0xA0 + i * 0x80, i, g * 0.03f, g * 0.3f);
        }
        if (*(int *)(data + 0x230) != -1) {
            char *o = D_L04_0015FFD8 + (*(int *)(data + 0x230) << 8);
            if (*(short *)(o + 0xA6) == 0x118) {
                off = **(float **)(o + 0x78) * 15.0f;
            }
        } else {
            off = 15.0f;
        }
        *(float *)(moby + 0x18) = *(float *)(data + 0x220) + bob - off;
        break;
    }
    }
    subtract_vector_xyz(delta, moby + 0x10, old);
    FUN_L00_00260738(data + 0x60, delta, moby + 0x40, moby + 0x40);
}

extern void remove_moby_2ce060(void *) __asm__("FUN_0020c828");

/* Marks the moby for removal; the unused buffer is kept for its frame. */
void FUN_L04_002ce060(void *moby) {
    char buf[0x50];

    remove_moby_2ce060(moby);
}

extern void FUN_L04_00292370(void *, char *);
extern void FUN_L04_002923b8(void *, char *);

void FUN_L04_002d1418(void *m) {
    char *d = *(char **)(m + 0x78);
    char *p = d + 0x160;
    float speed;
    float angleHalf;
    float angleFull;
    float angleOther;
    float amount;
    float frameScale;
    float otherScale;
    *(int *)(d + 0x1d0) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1d4) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1d8) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1dc) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1e0) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1e4) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1e8) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1ec) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x54) + 0x14);
    *(int *)(d + 0x1f0) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1f4) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1f8) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x1fc) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    *(int *)(d + 0x200) = *(int *)(*(char **)(*(char **)(m + 0x24) + 0x70) + 0x14);
    FUN_L04_00292370(m, p);
    frameScale = frame_time_sq;
    otherScale = frame_time;
    angleFull = frameScale * 6.2831855f;
    angleOther = otherScale * 6.2831855f;
    speed = frameScale * 9.8f;
    angleHalf = frameScale * 3.1415927f;
    amount = frameScale * 10.0f;
    *(unsigned char *)(d + 0x214) = 7;
    *(unsigned char *)(d + 0x210) = 0;
    *(unsigned char *)(d + 0x211) = 0;
    *(float *)(d + 0x23c) = speed;
    *(float *)(d + 0x22c) = angleHalf;
    *(float *)(d + 0x230) = angleFull;
    *(float *)(d + 0x234) = angleOther;
    *(float *)(d + 0x224) = amount;
    *(unsigned char *)(d + 0x212) = 0xff;
    *(unsigned char *)(d + 0x213) = 0x10;
    *(unsigned char *)(d + 0x215) = 7;
    *(unsigned char *)(d + 0x250) = 10;
    *(unsigned char *)(d + 0x251) = 8;
    *(unsigned char *)(d + 0x253) = 12;
    *(unsigned char *)(d + 0x252) = 14;
    *(unsigned char *)(d + 0x300) = 11;
    *(unsigned char *)(d + 0x301) = 9;
    *(unsigned char *)(d + 0x303) = 13;
    *(unsigned char *)(d + 0x302) = 15;
    *(float *)(d + 0x240) = 0.5f;
    FUN_L04_002923b8(m, p);
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D29E8), where it is exact; names translated to the US level program. */

extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void clear_vector(void *) __asm__("FUN_001f99f8");

void FUN_L04_002d1608(void *m) {
    char pos[16];
    char vec[16];
    clear_vector(vec);
    moby_point_position(m, 12, pos);
    func_L00_0025F4A8_alt(m, vec, pos, 0.0f, 0.0f, 10, 3, 4, 2.0f, 1.0f, 100000.0f, 1.0f, -1, 15.0f,
                          1, 1, -1, 0);
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D2A98), where it is exact; names translated to the US level program. */


typedef struct {
    int v[6];
} WBJoints;

typedef struct {
    float v[5];
} WBAngles;

typedef struct WBData {
    /* 0x000 */ char pad000[0x20];
    /* 0x020 */ float hp;
    /* 0x024 */ short f24;
    /* 0x026 */ char pad026[2];
    /* 0x028 */ unsigned char f28;
    /* 0x029 */ unsigned char f29;
    /* 0x02A */ char pad02A[6];
    /* 0x030 */ float f30;
    /* 0x034 */ char pad034[4];
    /* 0x038 */ int f38;
    /* 0x03C */ char pad03C[0x1C];
    /* 0x058 */ unsigned char f58;
    /* 0x059 */ char pad059;
    /* 0x05A */ unsigned char f5A;
    /* 0x05B */ char pad05B[0x55];
    /* 0x0B0 */ char fB0[7];
    /* 0x0B7 */ unsigned char fB7;
    /* 0x0B8 */ char pad0B8[8];
    /* 0x0C0 */ char fC0[0x10];
    /* 0x0D0 */ float fD0;
    /* 0x0D4 */ float fD4;
    /* 0x0D8 */ float fD8;
    /* 0x0DC */ float fDC;
    /* 0x0E0 */ int fE0;
    /* 0x0E4 */ int fE4;
    /* 0x0E8 */ float fE8;
    /* 0x0EC */ char pad0EC[0x11];
    /* 0x0FD */ unsigned char fFD;
    /* 0x0FE */ char pad0FE[0x12];
    /* 0x110 */ float f110;
    /* 0x114 */ float f114;
    /* 0x118 */ char pad118[8];
    /* 0x120 */ unsigned char f120;
    /* 0x121 */ unsigned char f121;
    /* 0x122 */ char pad122[0xE];
    /* 0x130 */ char f130[0x30];
    /* 0x160 */ char f160[0xB7];
    /* 0x217 */ unsigned char f217;
    /* 0x218 */ char pad218[0x198];
    /* 0x3B0 */ float f3B0[4];
    /* 0x3C0 */ unsigned char f3C0;
    /* 0x3C1 */ unsigned char f3C1;
    /* 0x3C2 */ short t3C2;
    /* 0x3C4 */ short t3C4;
    /* 0x3C6 */ short t3C6;
    /* 0x3C8 */ float f3C8;
    /* 0x3CC */ char pad3CC[4];
    /* 0x3D0 */ float f3D0;
    /* 0x3D4 */ float f3D4;
    /* 0x3D8 */ float f3D8;
    /* 0x3DC */ int f3DC;
    /* 0x3E0 */ float f3E0;
    /* 0x3E4 */ float f3E4;
    /* 0x3E8 */ float f3E8;
} WBData;

typedef struct WBMoby {
    /* 0x00 */ char pad00[0x10];
    /* 0x10 */ float pos[4];
    /* 0x20 */ unsigned char state;
    /* 0x21 */ char pad21[0x10];
    /* 0x31 */ unsigned char f31;
    /* 0x32 */ char pad32[0xE];
    /* 0x40 */ float rot[4];
    /* 0x50 */ char pad50[2];
    /* 0x52 */ unsigned char anim;
    /* 0x53 */ unsigned char anim2;
    /* 0x54 */ char pad54[4];
    /* 0x58 */ float f58;
    /* 0x5C */ char pad5C[0x14];
    /* 0x70 */ unsigned char f70;
    /* 0x71 */ char pad71[7];
    /* 0x78 */ WBData *d;
    /* 0x7C */ char pad7C[3];
    /* 0x7F */ unsigned char f7F;
    /* 0x80 */ char pad80[0x24];
    /* 0xA4 */ unsigned char fA4;
    /* 0xA5 */ char padA5[0x17];
    /* 0xBC */ unsigned char fBC;
} WBMoby;

typedef struct WBQuery {
    /* 0x00 */ float v[4];
    /* 0x10 */ void *m;
    /* 0x14 */ int f14;
    /* 0x18 */ int f18;
    /* 0x1C */ float f1C;
    /* 0x20 */ int f20;
    /* 0x24 */ int pad24[3];
} WBQuery;

WBAngles D_L04_001DC468 = {{0.5235988f, 0.7853982f, 0.2617994f, 1.0471976f, 0.0f}};
WBJoints D_L04_001DC450 = {{0, 2, 1, 3, 4, 5}};
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char D_L04_00166F40[];
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L04_0015F580[] __attribute__((section(".sdata")));
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float FUN_001f9b48(void *, void *);
extern float FUN_001f9b80(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float spring_turn(float, float, void *, float, float, float) __asm__("FUN_L00_00258110");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern int *D_L04_001B0630[];
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern int FUN_L00_0025c698(void *, void *);
extern int FUN_L04_00295778(void *, void *, float, float, float);
extern int is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void *memset(void *, int, unsigned int);
extern void coll_line_fix(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern void multiply_quaternions(void *, void *, void *) __asm__("FUN_001fa3c0");
extern void collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void FUN_L00_001fff28(void *, int, float);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258490(void *, void *, void *, int, float, float, float, float);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025ab48(void *, float *, void *, void *);
extern void FUN_L00_0025c558(float, void *, void *, int, int, int);
extern void FUN_L00_0025d458(void *, void *);
extern void FUN_L00_0025d538(void *, void *);
extern void FUN_L00_0025fcb8(void *, void *, float);
extern void FUN_L00_00263ac8(float, void *, int, void *);
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void FUN_L00_002a1538(void *);
extern void spawn_crate_debris(void *, void *, void *, void *, int, int, float, float) __asm__("FUN_L00_002af8b0");
extern void FUN_L04_00295de8(void *, void *);
extern void FUN_L04_002d1418(void *);
extern void add_vector_xyz(void *, void *, void *);
extern void attach_manipulator(char *, int, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
extern void transform_scaled_vertex_batch(char *arg0, int arg1, void *arg2,
                                          char *arg3) __asm__("FUN_0020cd48");
void FUN_L04_002d1608(void *m);
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void FUN_L04_002d16b8(WBMoby *m) {
    int excited = 0;
    WBData *d = m->d;
    unsigned char *p120;
    short *tm;
    float *pos;
    unsigned char *p;
    float tgt[4];
    float unused[16];
    float v60[4];
    float v70[4];
    float adiff;
    float dist;
    float hd;

    if (d->f38 != 0 || d->t3C6 != 0)
        excited = 1;
    FUN_L00_0025fcb8(m, tgt, excited ? d->f3E8 * 2.0f : d->f3E8);
    p = &d->f120;
    FUN_L00_00263ac8(2.5f, m, 6, p);
    p120 = p;
    pos = m->pos;
    if (m->f31 != 0 && FUN_001f9b48(pos, D_L04_00166F40) < 27.0f) {
        FUN_L00_0025a120(m);
        m->f7F = 0x15;
    }
    adiff = fast_difference_between_rotations(m->rot[2],
                                              fast_arc_tan(tgt[0] - m->pos[0], tgt[1] - m->pos[1]));
    dist = FUN_001f9b80(pos, tgt);
    hd = compute_interpolated_record_value(m);
    qcopy(v60, pos);
    qcopy(v70, pos);
    fast_dec_timer_s16(&d->t3C4);

    if (m->state != 0xB && m->state != 0) {
        int r;
        int htype;
        float damage;
        float angle;
        float one;
        float v[4];

        damage = 0.0f;
        p = (unsigned char *)find_moby_hit(m, 0xA10000, 0);
        r = hit_reaction(m, p, &d->hp, 0, &htype, &damage, 0, 4);
        d->fE0 = 0x15E;
        d->fE8 = 0.35f;
        if (p != 0 && damage != 0.0f && m->state != 0 && d->t3C4 == 0) {
            unsigned char *src;
            float a;
            float b;
            float c;

            src = *(unsigned char **)(p + 0x20);
            one = 1.0f;
            if (src != 0) {
                struct Hero *h = &hero;
                if (src != ((unsigned char *)h->items[0].moby)) {
                    angle = fast_arc_tan(m->pos[0] - *(float *)(src + 0x10),
                                         m->pos[1] - *(float *)(src + 0x14));
                } else {
                    angle = fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                         m->pos[1] - h->motion.pos.f[1]);
                }
            } else {
                struct Hero *h = &hero;
                angle = fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                     m->pos[1] - h->motion.pos.f[1]);
            }
            a = frame_time_sq * 30.0f;
            b = frame_time_sq * 10.0f;
            c = frame_time * 5.5f;
            d->fE4 = 9;
            d->fD0 = a;
            d->fFD = 0;
            d->fD4 = b;
            d->fD8 = c;
            d->hp -= damage;
            if (d->hp <= 0.0f)
                r = 1;
            switch (r) {
            case 0:
                break;
            case 1:
            case 2:
                d->fD4 = frame_time_sq * 17.0f;
                d->fD8 = frame_time * 7.0f;
                d->fDC = frame_time * 13.0f;
                *(OvlQuad *)v = *(OvlQuad *)(p + 0x10);
                FUN_L00_0025ab48(v, &angle, &d->fD8, &d->fDC);
                FUN_L00_0025c558(angle, m, d->fC0, 8, 1, 0);
                d->f110 = 14.0f;
                d->f114 = 24.0f;
                m->state = 0xB;
                FUN_L00_00257470(m, 0, -1);
                d->fB7 = 0x78;
                FUN_L00_0025d458(m, d->fB0);
                d->f3E4 = (2.0f / multiply_global_factor_ed64(15.0f)) * 2.0f;
                break;
            case 3:
            case 6:
                d->f3C8 = -0.5235988f;
                if (m->anim != 4 || hd < 16.0f || hd > 27.0f) {
                    short cls;

                    m->fBC |= 5;
                    blend_moby_animation(m, 5, 0, scale_game_frames(3));
                    FUN_L00_002a1538(d->f160);
                    m->state = 9;
                    cls = *(short *)(*(char **)(p + 0x20) + 0xA6);
                    if (cls == 0x131 || cls == 0xB0) {
                        d->f3E4 = (2.0f / multiply_global_factor_ed64(15.0f)) * 0.5f;
                    } else {
                        d->f3E4 = 2.0f / multiply_global_factor_ed64(15.0f);
                    }
                    *(OvlQuad *)v = *(OvlQuad *)(p + 0x10);
                    FUN_L00_0025ab48(v, &angle, &d->f3E4, &one);
                    d->fB7 = 0x78;
                    FUN_L00_0025d458(m, d->fB0);
                    d->t3C4 = scale_game_frames(0xF);
                    d->t3C6 = scale_game_frames(0xF0);
                }
                break;
            case 4:
            case 5:
                blend_moby_animation(m, 6, 0, scale_game_frames(10));
                FUN_L00_002a1538(d->f160);
                m->state = 1;
                d->f3E4 = 2.0f / multiply_global_factor_ed64(15.0f);
                *(OvlQuad *)v = *(OvlQuad *)(p + 0x10);
                FUN_L00_0025ab48(v, &angle, &d->f3E4, &one);
                m->rot[2] = fast_add_rotations(3.1415927f, angle);
                d->fB7 = 0x78;
                FUN_L00_0025d458(m, d->fB0);
                d->t3C4 = scale_game_frames(0x1E);
                d->t3C2 = scale_game_frames(0xB4);
                d->t3C6 = scale_game_frames(0xF0);
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                break;
            }
        }
    }
    m->fA4 = 0xFF;
    if (d->f38 != 0)
        d->t3C6 = scale_game_frames(0xF0);
    tm = &d->t3C2;
    d->f38 = 0;
    fast_dec_timer_s16(&d->t3C6);

    if (m->state == 8) {
        if (m->anim == 4 && hd > 16.0f && hd < 27.0f) {
            WBJoints joints = D_L04_001DC450;
            float pts[6][4];
            WBQuery s;
            float mid[4];
            int i;

            s.f14 = 0x10001;
            s.f1C = 1.0f;
            s.m = m;
            subtract_vector_xyz(&s, &hero.motion.pos, pos);
            normalize_vector_xyz(&s, &s, frame_time * 5.0f);
            s.f20 = 1;
            transform_scaled_vertex_batch(m, 6, &joints, pts);
            for (i = 0; i < 5; i++)
                coll_line_fix(pts[i], pts[i + 1], 9, m, &s);
            add_vector_xyz(mid, pts[0], pts[4]);
            scale_vector_xyz(mid, mid, 0.5f);
            collect_mobies_in_sphere(mid, FUN_001f9b48(pts[0], pts[4]) * 0.5f, 0x10, m, &s);
            moby_point_position(m, 0x11, mid);
            collect_mobies_in_sphere(mid, 0.5f, 0x10, m, &s);
        }
    }
    fast_dec_timer_s16(tm);

    switch (m->state) {
    case 0:
        d->hp = 2.0f;
        d->f24 = 2;
        d->f30 = 1.0f;
        d->f28 = 1;
        d->f29 = 0;
        m->fBC = 0;
        d->f3E0 = 0.0f;
        d->t3C6 = 0;
        d->f58 = 8;
        d->f5A = 8;
        qcopy(d->f3B0, pos);
        if (d->f121 == 0)
            attach_manipulator(m, 6, p120);
        FUN_L04_002d1418(m);
        if (d->f3C0 != 0) {
            m->state = 3;
            blend_moby_animation(m, 0xB, 0, scale_game_frames(10));
        } else {
            m->state = 1;
        }
        break;
    case 1:
        if (m->f70 & 2) {
            if (excited ? (d->f3E8 * 2.0f < dist) : (d->f3E8 < dist)) {
                int anims[6];

                memset(anims, 0, 0x18);
                anims[5] = 1;
                blend_moby_animation(m, anims[random_integer_below(6)], 0, scale_game_frames(0x14));
            } else {
                blend_moby_animation(m, 0, 0, scale_game_frames(0x14));
            }
        }
        if (d->t3C2 == 0) {
            if (dist < 3.0f && adiff < 1.5707964f) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                m->state = 8;
            } else if (excited ? (dist < d->f3E8 * 2.0f) : (dist < d->f3E8)) {
                if (m->fBC & 1) {
                    if (excited || d->f3DC == -1 ||
                        point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                         *D_L04_001B0630[d->f3DC]) != 0) {
                        m->state = 5;
                        m->fBC |= 8;
                    }
                } else {
                    d->f3D0 = 0.0f;
                    blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                    FUN_L00_002a1538(d->f160);
                    m->state = 0xA;
                    m->fBC |= 1;
                }
            }
            if (!excited) {
                if (FUN_001f9b80(pos, d->f3B0) > 2.0f) {
                    if (d->f3E8 < dist ||
                        (d->f3DC != -1 && point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                                           *D_L04_001B0630[d->f3DC]) == 0)) {
                        m->state = 6;
                        m->fBC |= 8;
                    }
                }
            }
        }
        break;
    case 2:
        if (m->anim == 2 && (m->f70 & 2)) {
            if (excited || d->f3DC == -1 ||
                point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4, *D_L04_001B0630[d->f3DC]) != 0) {
                m->state = 5;
                m->fBC |= 8;
            } else {
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
                m->state = 1;
            }
        }
        break;
    case 3: {
        if (m->anim != 0 && (m->f70 & 2)) {
            blend_moby_animation(m, 0xC, 0, scale_game_frames(10));
            m->f58 = random_float_between(0.8f, 1.2f);
        }
        if (is_value_within_interpolated_window(m, 36.0f) != 0) {
            int i;

            for (i = 0; i < 20; i++) {
                float p[4];
                float vel[4];
                float off[4];
                float rot[4];
                float mtx[16];

                *(OvlQuad *)rot = 0;
                rot[0] = random_float_between(-1.0f, 1.0f);
                rot[1] = random_float_between(-1.0f, 1.0f);
                rot[2] = random_float_between(-1.0f, 1.0f);
                *(OvlQuad *)off = *(OvlQuad *)rot;
                moby_point_position(m, 0, p);
                vel[0] = fast_cos(m->rot[2]) * -(random_float_between(5.0f, 10.0f) * frame_time);
                vel[1] = fast_sin(m->rot[2]) * -(random_float_between(5.0f, 10.0f) * frame_time);
                vel[2] = 0.0f;
                normalize_vector_xyz(off, off, frame_time * 3.0f);
                off[2] += frame_time * 3.0f;
                FUN_L00_001ff290(mtx, vel, off);
                rot[0] = random_angle_radians();
                rot[1] = random_angle_radians();
                rot[2] = random_angle_radians();
                spawn_crate_debris(m, vel, p, rot, 0x5EC, 0, 0.35f, 0.7f);
            }
        }
        if (d->t3C2 == 0) {
            if (dist < 3.0f && adiff < 1.5707964f) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                m->state = 8;
            } else if (excited ? (dist < d->f3E8 * 2.0f) : (dist < d->f3E8)) {
                if (m->fBC & 1) {
                    if (excited || d->f3DC == -1 ||
                        point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                         *D_L04_001B0630[d->f3DC]) != 0) {
                        m->state = 5;
                        m->fBC |= 8;
                    }
                } else {
                    d->f3D0 = 0.0f;
                    blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                    FUN_L00_002a1538(d->f160);
                    m->state = 0xA;
                    m->fBC |= 1;
                }
            }
        }
        break;
    }
    case 4:
        if (excited) {
            if (dist < 3.0f && adiff < 1.5707964f) {
                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                FUN_L00_002a1538(d->f160);
                m->state = 8;
            } else if (d->f3DC == -1 || point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                                         *D_L04_001B0630[d->f3DC]) != 0) {
                m->state = 5;
                m->fBC |= 8;
            } else if (adiff > 1.5707964f) {
                d->f3D0 = 0.0f;
                blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                FUN_L00_002a1538(d->f160);
                m->state = 0xA;
            }
        } else {
            m->state = 1;
        }
        break;
    case 5: {
        if (m->fBC & 8) {
            m->fBC &= ~8;
            FUN_L04_00295de8(m, d->f160);
        }
        FUN_L04_00295778(m, d->f160, frame_time * 4.0f,
                         fast_arc_tan(tgt[0] - m->pos[0], tgt[1] - m->pos[1]), frame_time_sq * 20.0f);
        if (dist < 3.0f && adiff < 1.5707964f) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            FUN_L00_002a1538(d->f160);
            m->state = 8;
        } else if (d->f3DC != -1) {
            float v[4];

            v[0] = fast_cos(m->rot[2]) * 2.0f;
            v[1] = fast_sin(m->rot[2]) * 2.0f;
            v[2] = 0.0f;
            p = (unsigned char *)v;
            add_vector_xyz(p, p, pos);
            if (point_in_path_xy(p, D_L04_001B0630[d->f3DC] + 4, *D_L04_001B0630[d->f3DC]) == 0) {
                if (excited && point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                                *D_L04_001B0630[d->f3DC]) == 0) {
                    m->state = 7;
                } else if (point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                            *D_L04_001B0630[d->f3DC]) != 0) {
                    d->f3D0 = 0.0f;
                    blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                    FUN_L00_002a1538(d->f160);
                    m->state = 0xA;
                }
            }
            if (!excited) {
                if (point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4, *D_L04_001B0630[d->f3DC]) ==
                    0) {
                    m->state = 6;
                }
            }
        }
        break;
    }
    case 6:
        if (m->fBC & 8) {
            m->fBC &= ~8;
            FUN_L04_00295de8(m, d->f160);
        }
        if (dist < 3.0f && adiff < 1.5707964f) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            FUN_L00_002a1538(d->f160);
            m->state = 8;
        } else {
            if (m->fBC & 8) {
                m->fBC &= ~8;
                FUN_L04_00295de8(m, d->f160);
            }
            if (FUN_001f9b80(pos, d->f3B0) < 2.0f) {
                m->state = 7;
            } else {
                FUN_L04_00295778(m, d->f160, frame_time * 4.0f,
                                 fast_arc_tan(d->f3B0[0] - m->pos[0], d->f3B0[1] - m->pos[1]),
                                 frame_time_sq * 20.0f);
                if (d->f3DC != -1) {
                    float v[4];

                    v[0] = fast_cos(m->rot[2]) * 2.0f;
                    v[1] = fast_sin(m->rot[2]) * 2.0f;
                    v[2] = 0.0f;
                    add_vector_xyz(v, v, pos);
                    if (point_in_path_xy(v, D_L04_001B0630[d->f3DC] + 4,
                                         *D_L04_001B0630[d->f3DC]) == 0) {
                        d->f3D0 = 0.0f;
                        blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                        FUN_L00_002a1538(d->f160);
                        m->state = 0xA;
                        m->fBC |= 0x10;
                    } else if (point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4,
                                                *D_L04_001B0630[d->f3DC]) != 0) {
                        m->state = 7;
                    }
                }
            }
        }
        break;
    case 7:
        if (dist < 3.0f && adiff < 1.5707964f) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            FUN_L00_002a1538(d->f160);
            m->state = 8;
        } else {
            FUN_L04_00295778(m, d->f160, frame_time * 0.0f, m->rot[2], frame_time_sq * 20.0f);
            if (d->f217 == 0) {
                if (excited) {
                    m->state = 4;
                } else {
                    m->state = 1;
                }
            }
        }
        break;
    case 8:
        if (m->anim == 4 && (m->f70 & 2)) {
            d->t3C2 = 0;
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
            m->state = 1;
        }
        break;
    case 9:
        if (m->anim2 != 5 || (m->anim == 5 && (m->f70 & 2))) {
            if (m->fBC & 4) {
                m->fBC &= ~4;
                if (d->f3DC == -1) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    m->state = 1;
                    d->t3C2 = 0;
                } else {
                    WBAngles tab = D_L04_001DC468;
                    float v[4];
                    float sign = 1.0f;
                    int found = 0;
                    int i;

                    for (i = 0; i < 9; i++) {
                        struct Hero *h = &hero;
                        float ang;

                        if ((i & 1) == 0) {
                            sign = 1.0f;
                            if (random_integer_below(100) & 1)
                                sign = -1.0f;
                        } else {
                            sign = -sign;
                        }
                        ang = fast_add_rotations(sign * tab.v[i >> 1],
                                                 fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                                              m->pos[1] - h->motion.pos.f[1]));
                        build_spherical_offset(v, 6.0f, ang, m->rot[1]);
                        add_vector_xyz(v, v, pos);
                        if (point_in_path_xy(v, D_L04_001B0630[d->f3DC] + 4,
                                             *D_L04_001B0630[d->f3DC]) != 0) {
                            found = 1;
                            blend_moby_animation(m, 0x10, 0, scale_game_frames(10));
                            d->f3D4 = ang;
                            d->f3D8 = 0.0f;
                            break;
                        }
                    }
                    if (!found) {
                        if (m->anim == 5) {
                            if (dist < 3.0f && adiff < 1.5707964f) {
                                blend_moby_animation(m, 4, 0, scale_game_frames(10));
                                m->state = 8;
                            } else {
                                blend_moby_animation(m, 0, 0, scale_game_frames(10));
                                m->state = 1;
                            }
                        } else {
                            blend_moby_animation(m, 0, 0, scale_game_frames(10));
                            m->state = 1;
                            d->t3C2 = 0;
                        }
                    }
                }
            } else {
                float v[4];

                if (hd < 28.0f && m->anim == 0x10) {
                    float lim = frame_scale * 0.11111111f;

                    if (d->f3D8 < lim) {
                        d->f3D8 = d->f3D8 + frame_time_sq * 20.0f;
                    } else {
                        d->f3D8 = lim;
                    }
                    build_spherical_offset(v, d->f3D8, d->f3D4, m->rot[1]);
                    add_vector_xyz(pos, pos, v);
                    m->rot[2] = spring_turn(
                        m->rot[2], fast_arc_tan(tgt[0] - m->pos[0], tgt[1] - m->pos[1]), &d->f3D0,
                        frame_time_sq * 3.1415927f, frame_time_sq * 6.2831855f, frame_time * 4.1887903f);
                } else if (d->f3D8 > 0.0f) {
                    d->f3D8 -= frame_time_sq * 200.0f;
                    build_spherical_offset(v, d->f3D8, d->f3D4, m->rot[1]);
                    add_vector_xyz(pos, pos, v);
                    m->rot[2] = spring_turn(
                        m->rot[2], fast_arc_tan(tgt[0] - m->pos[0], tgt[1] - m->pos[1]), &d->f3D0,
                        frame_time_sq * 3.1415927f, frame_time_sq * 6.2831855f, frame_time * 4.1887903f);
                } else {
                    m->state = 1;
                    d->t3C2 = 0;
                }
            }
        }
        break;
    case 10:
        if (m->anim == 0x10 && (m->f70 & 2)) {
            if ((m->fBC & 0x10) ||
                point_in_path_xy(tgt, D_L04_001B0630[d->f3DC] + 4, *D_L04_001B0630[d->f3DC]) == 0) {
                m->state = 6;
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            } else {
                m->state = 5;
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            }
            m->fBC = (m->fBC | 8) & ~0x10;
        } else if (m->fBC & 0x10) {
            m->rot[2] = spring_turn(
                m->rot[2], fast_arc_tan(d->f3B0[0] - m->pos[0], d->f3B0[1] - m->pos[1]), &d->f3D0,
                frame_time_sq * 3.1415927f, frame_time_sq * 6.2831855f, frame_time * 4.1887903f);
        } else {
            m->rot[2] =
                spring_turn(m->rot[2], fast_arc_tan(tgt[0] - m->pos[0], tgt[1] - m->pos[1]),
                                 &d->f3D0, 0.03f, 0.3f, frame_time * 6.4577184f);
        }
        break;
    case 11:
        if ((FUN_L00_0025c698(m, d->fC0) & 0x40) || m->pos[2] < 1.0f) {
            FUN_L04_002d1608(m);
            FUN_L00_00263fd8(m, 0x6CB, pos, m->rot, 0, 0, 0.0f, D_L04_0015F580, D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8(m, 0x6CC, pos, m->rot, 0, 0, 0.0f, D_L04_0015F580, D_L04_0015F580,
                             D_L04_0015F580);
            FUN_L00_00263fd8(m, 0x6CD, pos, m->rot, 0, 0, 0.0f, D_L04_0015F580, D_L04_0015F580,
                             D_L04_0015F580);
            mark_moby_for_removal(m);
            return;
        }
        break;
    }

    {
        float w[16];

        FUN_L00_0025d538(m, d->fB0);
        if (d->f3E4 > 0.0f) {
            struct Hero *h = &hero;

            w[0] = fast_cos(fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                         m->pos[1] - h->motion.pos.f[1])) *
                   d->f3E4;
            w[1] = fast_sin(fast_arc_tan(m->pos[0] - h->motion.pos.f[0],
                                         m->pos[1] - h->motion.pos.f[1])) *
                   d->f3E4;
            w[2] = 0.0f;
            add_vector_xyz(w, w, pos);
            d->f3E4 += -2.0f / (multiply_global_factor_ed64(15.0f) * multiply_global_factor_ed64(15.0f));
            FUN_L00_00258490(m, pos, w, 1, 0.25f, 1.0f, 0.75f, 0.87266463f);
        }
        d->f3C8 *= 0.96f;
        FUN_L00_001fff28(w, 1, d->f3C8);
        FUN_L00_001fff28(d->f130, 2, 0.0f);
        multiply_quaternions(d->f130, d->f130, w);
        if (m->state != 9)
            d->f3C1 = m->state;
        if (m->state != 5 && m->fBC != 6) {
            float g;

            d->f3E0 += frame_time_sq * 9.8f;
            m->pos[2] -= d->f3E0 - 2.0f;
            g = probe_ground_height(pos, 0, 0.5f);
            m->pos[2] -= 2.0f;
            if (m->pos[2] < g) {
                m->pos[2] = g;
                d->f3E0 = 0.0f;
            }
            if (m->pos[2] < 2.0f) {
                FUN_L00_00257470(m, 0, -1);
                mark_moby_for_removal(m);
            }
        }
    }
}
/* update: blend two keyframes of a path and write the result to a position */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D8348), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float FUN_L00_00200260(float, float);
extern int *D_L04_001B0630[];
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern char *D_L04_0015FFD0[] __attribute__((section(".sdata")));
void FUN_L04_002d6f68(char *moby) {
    int *p = *(int **)(moby + 0x78);
    int *t = D_L04_001B0630[p[2]];
    char *base = D_L04_0015FFD0[2];
    int o0 = p[0] * 256;
    float *src = *(float **)(base + o0 + 0x78);
    char *dst = base + p[1] * 256;
    float a[4];
    float b[4];
    float f, g;
    int idx;
    int n = 1;
    float one = 1.0f;

    f = convert_integer_to_float(t[0] - 1);
    g = (one - src[0]) * f;
    f = FUN_L00_00200260(g, one);
    idx = func_001FA898_r(g);
    if (f == 0.0f)
        n = 0;
    n += idx;
    scale_vector_xyz(a, (char *)t + (idx * 16 + 0x10), one - f);
    scale_vector_xyz(b, (char *)t + (n * 16 + 0x10), f);
    add_vector_xyz(moby + 0x10, a, b);
    qcopy(dst + 0x10, moby + 0x10);
}
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern int FUN_001f9740(s32 *);
extern int FUN_001f96f8(int);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void FUN_L00_001ff660(void *, void *, void *);
extern float D_L04_001CB940[6];
extern char D_L04_00173FE0[];

/* Three-state motion update. The saved position is restored when the timer expires. */
void FUN_L04_002d7e90(struct Moby *moby) {
    Vec4 random;
    Vec4 old;
    Vec4 work;
    Vec4 initialDirection;
    Vec4 direction;
    char *data = (char *)moby->pvars;
    float speed;
    float amount;
    int timer;
    float *motion;

    old.q = 0;
    old.f[0] = random_float_between(-1.0f, 1.0f);
    old.f[1] = random_float_between(-1.0f, 1.0f);
    old.f[2] = random_float_between(-1.0f, 1.0f);
    random.q = old.q;
    speed = vector_length_xy(data);
    qcopy(&old, &moby->pos);
    switch (moby->state) {
    case 0:
        qcopy(data + 0x30, &moby->pos);
        *(int *)(data + 0x40) = *(int *)(data + 0x44) = random_int_between(120, 2400);
        *(float *)(data + 0x4c) = moby->scale * random_float_between(0.8f, 1.2f);
        moby->state = 1;
        break;
    case 1:
        if (!(FUN_001f9b80(&moby->pos, D_L04_00166F40) < 32.0f))
            return;
        if (!FUN_001f9740((int *)(data + 0x44)))
            return;
        direction.q = 0;
        direction.f[0] = random_float_between(-1.0f, 1.0f);
        direction.f[1] = random_float_between(-1.0f, 1.0f);
        direction.f[2] = random_float_between(-1.0f, 1.0f);
        moby->update_dist = 0xff;
        moby->draw_dist = 0x40;
        moby->scale = 0.0f;
        motion = (float *)moby->pvars;
        initialDirection.q = direction.q;
        normalize_vector_xyz(&initialDirection, &initialDirection, D_L04_001CB940[5] / 5.0f);
        motion[0] = D_L04_001CB940[5] * fast_cos(D_L04_001CB940[1]);
        motion[1] = D_L04_001CB940[5] * fast_sin(D_L04_001CB940[1]);
        motion[2] = frame_time * -0.5f;
        normalize_vector_xyz(&moby->rot, &initialDirection, 3.1415927f);
        normalize_vector_xyz(motion + 8, &initialDirection, 0.098174773f);
        normalize_vector_xyz(motion + 4, &initialDirection, D_L04_001CB940[5] * 0.5f);
        *(int *)(motion + 16) = *(int *)(motion + 17) = random_int_between(720, 1200);
        moby->state = 2;
        return;
    case 2:
        FUN_001f9740((int *)(data + 0x44));
        normalize_vector_xyz(&random, &random, D_L04_001CB940[5] / 10.0f);
        add_vector_xyz(data + 0x10, data + 0x10, &random);
        normalize_vector_xyz(&work, data + 0x10, 5.0f);
        normalize_vector_xyz(data + 0x10, data + 0x10, D_L04_001CB940[5] * 0.5f);
        add_vector_xyz(&work, &work, &moby->pos);
        if (collision_line(&old, &work, 6, 0, 0) && !random_integer_below(8)) {
            FUN_L00_001ff660(data + 0x10, data + 0x10, D_L04_00173FE0 + 0x20);
            *(float *)(data + 0x18) += D_L04_001CB940[5] / 10.0f;
        }
        add_vector_xyz(data, data, data + 0x10);
        work.f[0] = D_L04_001CB940[5] * fast_cos(D_L04_001CB940[1]);
        work.f[1] = D_L04_001CB940[5] * fast_sin(D_L04_001CB940[1]);
        work.f[2] = frame_time * -0.5f;
        scale_vector_xyz(&work, &work, 0.4f);
        scale_vector_xyz(data, data, 0.6f);
        add_vector_xyz(data, &work, data);
        add_vector_xyz(&moby->pos, &moby->pos, data);
        if (collision_line(&old, &moby->pos, 6, 0, 0)) {
            qcopy(&moby->pos, D_L04_00173FE0);
            FUN_L00_001ff660(data, data, D_L04_00173FE0 + 0x20);
            scale_vector_xyz(data, data, 0.25f);
            add_vector_xyz(&moby->pos, &moby->pos, data);
            *(float *)(data + 8) += D_L04_001CB940[5] / 5.0f;
        }
        if (D_L04_001CB940[5] < speed)
            normalize_vector_xyz(data, data, D_L04_001CB940[5]);
        {
            float amount = frame_time * -0.5f;
            if (amount < *(float *)(data + 8))
                *(float *)(data + 8) += amount / 10.0f;
            else
                *(float *)(data + 8) = amount;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x20));
        moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x24));
        moby->rot.z = fast_add_rotations(moby->rot.z, *(float *)(data + 0x28));
        if (*(int *)(data + 0x40) - FUN_001f96f8(10) < *(int *)(data + 0x44)) {
            amount = convert_integer_to_float(*(int *)(data + 0x44) - *(int *)(data + 0x40));
            moby->scale = (*(float *)(data + 0x4c) * amount) / multiply_global_factor_ed64(10.0f);
        } else {
            if (*(int *)(data + 0x44) < FUN_001f96f8(100))
                moby->scale *= 0.99f;
            else
                moby->scale = *(float *)(data + 0x4c);
        }
        if (*(int *)(data + 0x44) > 0)
            return;
        qcopy(&moby->pos, data + 0x30);
        timer = random_int_between(120, 2400);
        /* Preserve the retail ordering of the timer reload stores. */
        *(volatile int *)(data + 0x40) = timer;
        *(volatile int *)(data + 0x44) = timer;
        moby->state = 1;
        break;
    default:
        return;
    }
    moby->scale = 0.0f;
}
/* Rise speed of a riser by its class. */
float riser_rise_speed(struct Moby *moby) __asm__("FUN_L04_002e1768");

float riser_rise_speed(struct Moby *moby) {
    switch (moby->oclass) {
    case 0x44D:
    case 0x5FC:
        return 1.3f;
    case 0x5FB:
        return -2.55f;
    }
    return -2.05f;
}
/* Riser: once its id is collected or its trigger moby reaches the open state, rises for a
 * second, then records its id in the level bitsets. */
typedef struct {
    u8 pad[0x454];
    u8 collected[1];
} LevelCollected;

struct RiserVars {
    s32 trigger;   /* moby index whose state opens it, -1 for none */
    f32 baseZ;
    s32 timer;
};

extern s32 D_L04_001BA650[];
extern LevelCollected D_L04_001BB3B0;
extern int FUN_L00_0028dc90(int, int, struct Moby *, int);
extern int FUN_001f96f8(int);
extern int FUN_001f9740(s32 *);
extern int allocate_voice_for_target_entry(int, int, struct Moby *) __asm__("FUN_0022da68");

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
void FUN_L04_002e17d8(struct Moby *m) {
    struct RiserVars *d = (struct RiserVars *)m->pvars;
    struct Moby *t;
    s16 idx;
    u16 id;

    switch (m->state) {
    case 0:
        d->baseZ = m->pos.z;
        m->state = 1;
        break;
    case 1:
        id = m->save_id;
        idx = id;
        if (D_L04_001BB3B0.collected[idx] == 0 && !((D_0014C190_257218[current_level_index][idx >> 5] >> (id & 0x1F)) & 1)) {
            if (d->trigger == -1)
                break;
            t = (struct Moby *)((d->trigger << 8) + D_L04_0015FFD8);
            if (!((t->oclass == 0x267 && t->state == 4) || (t->oclass == 0x4A6 && t->state == 2)))
                return;
        }
        if (m->oclass == 0x44D || m->oclass == 0x5FC)
            FUN_L00_0028dc90(0, 0, m, 0x44D);
        m->state = 2;
        d->timer = FUN_001f96f8(60);
        break;
    case 2:
        m->pos.z += riser_rise_speed(m) * frame_time;
        if (FUN_001f9740(&d->timer)) {
            D_0014C190_257218[current_level_index][(s16)m->save_id >> 5] |= 1 << (m->save_id & 0x1F);
            D_L04_001BA650[(s16)m->save_id >> 5] |= 1 << (m->save_id & 0x1F);
            if (m->oclass == 0x44D)
                allocate_voice_for_target_entry(1, 0, m);
            m->state = 3;
        }
        break;
    }
}
/* Warp portal: spins, opens once the hero is near, then warps to the exit moby. */
struct PortalVars {
    s32 unk0;
    s32 exitMoby;   /* 0x04: moby index to warp to, -1 for none */
};

struct PortalMobyView {
    u8 pad0[0xB2];
    s16 uid;        /* 0xB2 */
};

extern s32 D_L04_0015F5C4;
extern s32 D_L04_0015F640;
extern u8 D_0013D4F1[];
extern u8 D_0013D4C9[];
extern char D_L04_001DC760[];
extern struct Moby *portal_mobys_2e1a10 __asm__("D_L04_0015FFD8");
extern void FUN_L00_002d6bf0(struct Moby *);
extern void remove_moby_2e1a10(struct Moby *) __asm__("FUN_0020c828");
extern void FUN_L04_002e1d00_m(struct Moby *) __asm__("FUN_L04_002e1d00");
extern void FUN_L00_00298840(int);
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void FUN_L00_00260860(int, int);
extern void FUN_L00_00284e50(void *, void *);
extern void debug_print(char *, int) __asm__("FUN_001e93b0");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

void FUN_L04_002e1a10(struct Moby *moby) {
    struct PortalVars *vars = (struct PortalVars *)moby->pvars;
    struct Moby *exit;

    moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
    if (D_L04_0015F5C4 == 2)
        moby->flags |= 0x41;
    else if (moby->flags & 1)
        moby->flags &= ~0x41;
    switch (moby->state) {
    case 0:
        FUN_L00_002d6bf0(moby);
        if (D_0013D4F1[0]) {
            remove_moby_2e1a10(moby);
            break;
        }
        moby->state = 1;
        moby->pos.z += 1.0f;
        break;
    case 1:
        FUN_L04_002e1d00_m(moby);
        if (FUN_001f9b80(&moby->pos, &hero.motion.pos) < 3.0f) {
            moby->flags |= 0x41;
            FUN_L00_00298840(2);
            moby->state = 2;
        }
        break;
    case 2:
        if (D_L04_0015F5C4 == 2)
            break;
        if (!D_0013D4C9[0])
            queue_dialogue_message(0xFA7, -1);
        else
            queue_dialogue_message(0x53DF, -1);
        D_L04_0015F640 = 0xB4;
        FUN_L00_00260860(9, 1);
        if (vars->exitMoby != -1) {
            exit = &portal_mobys_2e1a10[vars->exitMoby];
            FUN_L00_00284e50(&exit->pos, &exit->rot);
        } else {
            debug_print(D_L04_001DC760, ((struct PortalMobyView *)moby)->uid);
        }
        moby->state = 3;
        memcard_save_data(0, -1);
        break;
    case 3:
        remove_moby_2e1a10(moby);
        break;
    }
}
/* Same source as the exact FUN_L10_00298940, with a 0.25 lift and the draw skipped in mode 2. */
typedef struct {
    float pad0[4];
    float a[4];
    float b[4];
    int c[4];
    float d[4];
} L10SpriteData_298940;
extern void func_001F9BF0_298940(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_L00_001FF4B0_298940(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_298940(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9908_298940(int *) __asm__("FUN_001f9740");
extern int func_001F9850_298940(int) __asm__("FUN_001f96f8");
extern float func_001FA888_298940(int) __asm__("FUN_001fa6c0");
extern int func_001FA8A8_298940(int, int, float) __asm__("FUN_001fa6e0");
extern float func_001F9B88_298940(float) __asm__("FUN_001f99c0");
extern unsigned char *func_L00_00273E08_298940(void *, int, unsigned char, int, int, int, int,
                                               float) __asm__("FUN_L00_00272f68");
extern int func_001FA898_298940(float) __asm__("FUN_001fa6d0");
extern char D_L10_00167240_298940[] __asm__("D_L04_00166F40");
extern int D_L04_0015F5C4;
void FUN_L04_002e1d00(char *m) {
    L10SpriteData_298940 *p = *(L10SpriteData_298940 **)(m + 0x78);
    float d[4] __attribute__((aligned(16)));
    float e[4] __attribute__((aligned(16)));
    int i, col;
    float f;

    *(float *)(m + 0x18) += 0.25f;
    func_001F9BF0_298940(d, D_L10_00167240_298940, m + 0x10);
    func_L00_001FF4B0_298940(d, d, -0.3f);
    func_L00_001FF4B0_298940(e, d, 0.1f);
    func_001F9BD8_298940(d, d, m + 0x10);
    *(float *)(m + 0x18) -= 0.25f;
    for (i = 0; i < 4; i++) {
        float h = p->a[i] + p->b[i];
        p->a[i] = h;
        if (255.0f <= h) {
            p->a[i] = h - 255.0f;
        } else if (h <= 0.0f) {
            p->a[i] = h + 255.0f;
        }
        if (func_001F9908_298940(&p->c[i])) {
            p->c[i] = func_001F9850_298940(0xFF);
        }
        f = func_001FA888_298940(func_001F9850_298940(0xFF) - p->c[i]);
        f = f / (float)func_001F9850_298940(0xFF);
        col = func_001FA8A8_298940(0x4040FFFF, 0x1040FFFF, func_001F9B88_298940(0.5f - f));
        if (D_L04_0015F5C4 != 2)
            func_L00_00273E08_298940(d, col, func_001FA898_298940(p->a[i]) & 0xFF, 0x35, 1, 2, 0,
                                     p->d[i]);
        func_001F9BD8_298940(d, d, e);
    }
}
