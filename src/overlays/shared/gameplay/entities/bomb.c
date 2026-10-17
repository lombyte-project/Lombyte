/* Shared moby classes: bomb, morphoray_chicken, ryno, rocket, drone, bolt_crate. */
#include "types.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"

#include "rnc/overlay/quad.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/collision.h"
#include "eetypes.h"
#include "qcopy.h"

/* Vars of a thrown moby, as ThrowVars_2c8440 in trespasser.c without its first 0x10 bytes. */
typedef struct {
    Vec4 vel;                      /* 0x00: velocity per step */
    Vec4 landing;                  /* 0x10: predicted landing point */
    Vec4 normal;                   /* 0x20: surface normal there */
    struct Moby *holder;           /* 0x30: moby it was thrown from */
    u8 pad_34[4];
    s16 timer;                     /* 0x38 */
    s16 unk46;
} ThrowVars_2c8440;

extern s32 D_L00_0015F5C4_2c8440 __asm__("D_L00_0015F5C4");                          /* nonzero: no throws */
extern float D_0015ED6C __attribute__((section(".sdata")));   /* frame-rate scale */
extern float D_0015ED70 __attribute__((section(".sdata")));
extern CollisionHit coll_hit __asm__("D_L00_00173E40") __attribute__((section(".data")));
/* coll_hit_2c8440's normal as a whole quadword (CollisionHit names only x/y/z) */
extern Vec4 coll_hit_normal_2c8440 __asm__("D_L00_00173E80") __attribute__((section(".data")));
extern Vec4 D_L00_00166DC0_2c8440 __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern f32 atan2_f_2c8440(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_add_rotations_2c8440(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_001f9dc8_2c8440(f32) __asm__("FUN_001f9dc8");                        /* cos */
extern f32 FUN_001f9de0_2c8440(f32) __asm__("FUN_001f9de0");                        /* sin */
extern void add_vector_xyz_2c8440(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2c8440(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz_2c8440(void *, void *, f32) __asm__("FUN_001f9a68");
extern s32 scale_game_frames_2c8440(s32) __asm__("FUN_001f96f8");
extern s32 collision_line_2c8440(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern s32 tick_countdown_2c8440(s32 *) __asm__("FUN_001f9740");
extern void FUN_00213508_2c8440(void *, s32, f32) __asm__("FUN_00213508");
extern void FUN_001f4600_2c8440(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L00_002c7df8_2c8440(void) __asm__("FUN_L00_002c7df8");
extern unsigned char *FUN_L00_002603d0_2c8440(int) __asm__("FUN_L00_002603d0");
extern s32 FUN_001f0b58_2c8440(void) __asm__("FUN_001f0b58");

/*
 * Predicts where a thrown moby lands: steps the velocity (with gravity) from
 * its start until collision_line_2c8440 hits ground (or a moby other than the hero
 * and the holder), for up to `timer` steps. Stores the landing point and
 * normal in vars and pulls the point 5% toward D_L00_00166DC0_2c8440. mode 0 needs
 * a live hit moby; mode 1 starts from the hero's hand. Returns 1 on a hit.
 */
s32 FUN_L00_002c8440(ThrowVars_2c8440 *v, struct Moby *m, s32 mode)
{
    Vec4 d, e, a, q, w;
    s32 t;
    s32 found = 0;
    s32 i;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4_2c8440 != 0)
        return 0;
    qcopy(&a, &m->pos);
    if (m->state == 1) {
        d.f[0] = FUN_001f9dc8_2c8440(fast_add_rotations_2c8440(-0.36196801066398620605f, atan2_f_2c8440(hero.unk670, hero.unk674))) * 0.85903900861740112305f;
        d.f[1] = FUN_001f9de0_2c8440(fast_add_rotations_2c8440(-0.36196801066398620605f, atan2_f_2c8440(hero.unk670, hero.unk674))) * 0.85903900861740112305f;
        d.f[2] = 0.0f;
        add_vector_xyz_2c8440(&d, &d, &hero.motion.pos);
        d.f[2] += 0.39081999659538269043f;
        qcopy(&e, &v->vel);
        if (hero.state.control_mode == 15)
            add_vector_xyz_2c8440(&e, &e, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0_2c8440((int)hero.ground_moby) && mode)
            add_vector_xyz_2c8440(&e, &e, &hero.motion.displacement);
        t = scale_game_frames_2c8440(300);
    } else {
        u8 *holder_vars = v->holder->pvars;

        qcopy(&d, &m->pos);
        qcopy(&e, &v->vel);
        {
            s32 timer = v->timer;
            s32 flag = *(s32 *)(holder_vars + 0x50);

            t = timer;
            if (flag)
                v->unk46 = 1;
        }
    }
    e.f[2] -= D_0015ED6C * 0.5f;
    q.q = ((Vec4 *)&hero.moby->pos)->q;
    q.f[2] = d.f[2];
    if (collision_line_2c8440(&q, &d, 0x10, hero.moby, 0) && (FUN_001f0b58_2c8440() || 0.0f < e.f[2])) {
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby != hero.moby && (struct Moby *)coll_hit.moby != v->holder)
                || t < scale_game_frames_2c8440(300) - scale_game_frames_2c8440(10)) {
                w.q = 0;
                w.f[2] = 0.5f;
                qcopy(&v->landing, &((struct Moby *)coll_hit.moby)->pos);
                found = 1;
                add_vector_xyz_2c8440(&w, &w, &v->landing);
                FUN_00213508_2c8440(&w, 0, 0.5f);
            }
        } else if (coll_hit.unk1C > 0) {
            qcopy(&v->landing, &coll_hit.point);
            found = 1;
        }
    }
    while (!tick_countdown_2c8440(&t) && !found) {
        qcopy(&a, &d);
        for (i = 9; i >= 0; i--) {
            add_vector_xyz_2c8440(&d, &d, &e);
            e.f[2] -= D_0015ED70 * 9.0f;
        }
        if (!collision_line_2c8440(&a, &d, 0x10, m, 0))
            continue;
        if (!FUN_001f0b58_2c8440() && 0.0f < e.f[2])
            continue;
        if (coll_hit.moby != 0 && coll_hit.unk1C <= 0) {
            if (((struct Moby *)coll_hit.moby == hero.moby || (struct Moby *)coll_hit.moby == v->holder)
                && !(t < scale_game_frames_2c8440(300) - scale_game_frames_2c8440(10)))
                continue;
            w.q = 0;
            w.f[2] = 0.5f;
            qcopy(&v->landing, &((struct Moby *)coll_hit.moby)->pos);
            add_vector_xyz_2c8440(&w, &w, &v->landing);
            FUN_00213508_2c8440(&w, 0, 0.5f);
            break;
        } else if (coll_hit.unk1C > 0) {
            qcopy(&v->landing, &coll_hit.point);
            break;
        }
    }
    if (t == 0)
        return 0;
    if (mode == 0) {
        if (coll_hit.moby == 0)
            return 0;
        if (FUN_L00_002603d0_2c8440(coll_hit.moby) == 0)
            goto ret0;
    }
    if (mode != 2 || coll_hit.moby == 0 || !FUN_L00_002603d0_2c8440(coll_hit.moby))
        FUN_001f4600_2c8440(FUN_L00_002c7df8_2c8440, m);
    v->normal.q = coll_hit_normal_2c8440.q;
    subtract_vector_xyz_2c8440(&a, &v->landing, &D_L00_00166DC0_2c8440);
    scale_vector_xyz_2c8440(&a, &a, 0.94999998807907104492f);
    add_vector_xyz_2c8440(&v->landing, &a, &D_L00_00166DC0_2c8440);
    return 1;
ret0:
    /* a second return 0 block, as in retail */
    return 0;
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002c89e8.s", FUN_L00_002c89e8);
#else
extern u8 D_0013E534 __attribute__((section(".data")));
extern int D_L00_0015FFE4;
extern f32 vector_length_xyz_2cb418(void *) __asm__("FUN_001f9af0");
extern f32 convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern s32 tick_timer_2cb418(void *) __asm__("FUN_001f9770");
extern void reflect_vector_2cb418(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_L00_001ff290_2c89e8(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern s32 FUN_001f0d60_2c89e8(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_0026ed28(struct Moby *);
extern void FUN_L00_002c8078(struct Moby *);
extern void FUN_L00_002bfe08(struct Moby *, void *, void *);
extern void FUN_L00_00260400(struct Moby *, s32, void *, void *, void *, void *);
extern f32 probe_ground_height(void *, int, f32) __asm__("FUN_00213508");

/* Six checked switch entries: state 5 shares the default state reset. */
void FUN_L00_002c89e8(struct Moby *m)
{
    Vec4 d, e, a, q;
    ThrowVars_2c8440 *v;
    struct Moby *p;
    s32 count;
    s32 timer;

    if (!m) return;
    v = (ThrowVars_2c8440 *)m->pvars;
    if (!v) return;
    if (m->pos.x < 2.0f || 1021.0f < m->pos.x ||
        m->pos.y < 2.0f || 1021.0f < m->pos.y ||
        m->pos.z < 2.0f || 1021.0f < m->pos.z) goto remove;
    if (hero.unk20A5 && m->state < 2)
        m->flags |= 0x41;
    else
        m->flags &= 0xffbe;
    if (v->holder->oclass == 0xe5 && v->unk46 == 0 && vector_length_xyz_2cb418(v) > 0.0f) {
        if (!hero.ground_moby || !FUN_L00_002603d0_2c8440((int)hero.ground_moby))
            FUN_L00_002c8440(v, m, 1);
        else if (!FUN_L00_002c8440(v, m, 0))
            FUN_L00_002c8440(v, m, 2);
    }
    switch (m->state) {
    case 0:
        m->state = 1;
        *(s32 *)v->pad_34 = scale_game_frames_2c8440(4);
        v->timer = 0;
        *(s32 *)((u8 *)v + 0x3c) = 0;
        break;
    case 1: {
        v->timer = 0;
        {
            f32 f = m->pclass->scale;
            if (m->scale < f * 0.25f)
                m->scale = m->scale + f * (frame_scale * 0.05f);
        }
        if (tick_countdown_2c8440((s32 *)v->pad_34)) {
            *(s32 *)v->pad_34 = scale_game_frames_2c8440(4);
            if (!hero.unk20A5) FUN_L00_0026ed28(m);
        }
        if (!hero.vehicle_mode && v->holder && (v->holder->state & 0xf0) != 0xf0 &&
            hero.items[0].moby && hero.items[0].moby->oclass == 0xe5) return;
        goto remove;
    }
    case 2: {
        f32 f, g, h;
        f32 half = 0.5f;
        u8 *collision_point;
        f = convert_integer_to_float(D_0013E534) + 1.0f;
        {
            f32 g = m->pclass->scale;
            if (m->scale < g * half * f)
                m->scale = m->scale + g * (frame_scale * 0.05f) * f;
        }
        if (tick_countdown_2c8440((s32 *)v->pad_34)) {
            *(s32 *)v->pad_34 = scale_game_frames_2c8440(4);
            FUN_L00_0026ed28(m);
        }
        v->vel.f[2] -= D_0015ED70 * 9.0f;
        add_vector_xyz_2c8440(&d, &m->pos, v);
        FUN_L00_001ff290_2c89e8(&q, &m->pos, &hero.motion.platform_carry);
        qcopy_nc(&e, &m->pos);
        h = frame_scale * 0.15f * m->scale;
        e.f[2] -= h / m->pclass->scale;
        qcopy_nc(&a, &d);
        h = h / m->pclass->scale;
        a.f[2] -= h;
        qcopy_nc(&q, &d);
        q.f[2] += f * 0.2f;
        if (FUN_001f0d60_2c89e8(f * 0.3f, &q, 4, hero.moby)) {
            collision_point = (u8 *)&coll_hit + 0x30;
            qcopy_nc(&m->pos, collision_point);
            m->pos.z -= f * 0.2f;
            collision_point += 0x10;
            h = 0.01f;
        } else {
            if (!collision_line_2c8440(&e, &a, 0, hero.moby, 0)) {
                goto collision_miss;
            }
            collision_point = (u8 *)&coll_hit + 0x20;
            qcopy_nc(&m->pos, collision_point);
            m->pos.z += (frame_scale * 0.15f * m->scale) / m->pclass->scale;
            collision_point += 0x20;
            h = 0.01f;
        }
        reflect_vector_2cb418(v, v, collision_point);
        scale_vector_xyz_2c8440(v, v, half);
        g = vector_length_xyz_2cb418(v);
        if (g < frame_scale * h) goto settle;
        if (hero.ground_moby) {
            if (FUN_L00_002603d0_2c8440((int)hero.ground_moby)) {
                g = vector_length_xyz_2cb418(v);
                f = vector_length_xyz_2cb418(&hero.motion.displacement);
                if (g < frame_scale * h + f) {
settle:
                    timer = (u16)v->timer + 1;
                    v->timer = timer;
                    if (!coll_hit.moby || coll_hit.unk1C > 0 ||
                        (timer = (s16)timer, scale_game_frames_2c8440(240) < timer)) {
                        m->state = 3;
                        v->timer = 0;
                    }
                }
            }
            if (hero.ground_moby && FUN_L00_002603d0_2c8440((int)hero.ground_moby))
                add_vector_xyz_2c8440(v, v, &hero.motion.displacement);
        }
        add_vector_xyz_2c8440(&m->pos, &m->pos, v);
        FUN_L00_002c8078(m);
        break;
collision_miss:
        qcopy_nc(&m->pos, &d);
        return;
    }
    case 3: {
        f32 f;
        f = probe_ground_height(&m->pos, 0, 0.5f);
        if (f != 0.0f && coll_hit.moby)
            FUN_L00_00260400(m, coll_hit.moby, &m->pos, &m->rot, &m->pos, &m->rot);
        if (tick_timer_2cb418(&v->timer)) {
            count = 0;
            for (p = (struct Moby *)D_L00_0015FFE4; p; p = p->next)
                if (p->oclass == 0xba && (p->state & 0xf0) != 0xf0) count++;
            if (count >= 8) {
                v->timer = scale_game_frames_2c8440(15);
                goto spawn_done;
            }
            {
                e.f[0] = FUN_001f9dc8_2c8440((f32)*(s32 *)((u8 *)v + 0x3c) * 6.2831855f * 0.25f);
                e.f[0] = frame_scale * 0.05f * e.f[0];
                e.f[1] = FUN_001f9de0_2c8440((f32)*(s32 *)((u8 *)v + 0x3c) * 6.2831855f * 0.25f);
                e.f[2] = frame_scale * 0.02f;
                e.f[1] = frame_scale * 0.05f * e.f[1];
                if (hero.ground_moby && FUN_L00_002603d0_2c8440((int)hero.ground_moby))
                    add_vector_xyz_2c8440(&e, &e, &hero.motion.displacement);
                add_vector_xyz_2c8440(&d, &e, &m->pos);
                FUN_L00_002bfe08(v->holder, &d, &e);
                v->timer = scale_game_frames_2c8440(8);
                count = *(s32 *)((u8 *)v + 0x3c) + 1;
                *(s32 *)((u8 *)v + 0x3c) = count;
                if (count > 3) m->state = 4;
            }
        }
spawn_done:
        if (*(s32 *)((u8 *)v + 0x3c) < 2 && tick_countdown_2c8440((s32 *)v->pad_34)) {
            *(s32 *)v->pad_34 = scale_game_frames_2c8440(4);
            FUN_L00_0026ed28(m);
        }
        break;
    }
    case 4: {
        f32 f;
        m->alpha -= 4;
        m->scale *= 0.92f;
        f = probe_ground_height(&m->pos, 0, 0.5f);
        if (f != 0.0f && coll_hit.moby)
            FUN_L00_00260400(m, coll_hit.moby, &m->pos, &m->rot, &m->pos, &m->rot);
        if (m->alpha) return;
remove:
        mark_moby_for_removal(m);
        break;
    }
    case 5:
    default:
        m->state = 0;
    }
}
#endif
/* 0x002c92d8, 112 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// resets the fields of a record (if non-null) to their defaults
/* Fills the block from 0x78 with fixed defaults and -1 ids. */
void FUN_L00_002c92d8(char *a) {
    if (a != 0) {
        *(int *)(a + 0x78) = 0;
        *(float *)(a + 0x7C) = 0.05f;
        *(float *)(a + 0x80) = 0.863f;
        *(int *)(a + 0x84) = 4;
        *(int *)(a + 0x88) = 3;
        *(float *)(a + 0x8C) = 0.625f;
        *(short *)(a + 0x9C) = -1;
        *(short *)(a + 0x9E) = -1;
        *(int *)(a + 0xA0) = -1;
        *(int *)(a + 0xA4) = -1;
        *(int *)(a + 0xA8) = -1;
        *(int *)(a + 0xAC) = -1;
    }
}

#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

typedef union {
    u128 q;
    f32 f[4];
} V4;

typedef struct {
    u8 pad00[0x70];
    void *unk70;
    u8 pad74[0xB0 - 0x74];
} Node;

typedef struct {
    u8 pad00[0x10];
    void *unk10;
    void *unk14;
    u8 pad18[0x64 - 0x18];
    u16 unk64;
    u16 unk66;
    u8 pad68[0x70 - 0x68];
    s32 unk70;
    u32 unk74;
    f32 unk78;
} Sub;

/* Class header view: pclass + 0xE and + 0x10. */
typedef struct {
    u8 pad00[0xE];
    u8 unkE;
    u8 pad0F;
    u32 unk10;
} Part;

typedef struct {
    V4 v;
    f32 t1;
    f32 t2;
    f32 t3;
} Loc;

typedef struct {
    u8 pad00[4];
    u8 f4;
    u8 f5;
    u8 f6;
} Opt;

typedef struct {
    u8 pad00[0xC];
    Opt *unk0C;
} Holder;

typedef struct {
    u8 pad00[0x10];
    V4 pos;
    u8 pad20[0x34 - 0x20];
    u16 flags34;
    u8 pad36[0x38 - 0x36];
    u64 unk38;
    u8 pad40[0x78 - 0x40];
    Holder *unk78;
} State;

typedef struct {
    u8 pad00[0x28];
    f32 f28;
    u8 pad2C[0x40 - 0x2C];
    V4 v40;
} Data;

extern u8 D_0013E520[];
extern Data D_L00_00173E40;
extern struct Moby *D_L00_001DD100[];
extern Node D_L00_001DD150[];
extern u8 D_L00_001DDF10[];
extern s32 D_L00_00161854 __attribute__((sda));
extern s32 D_L00_00161854_gp __asm__("D_L00_00161854") __attribute__((sda));
extern u32 D_L00_00161840[1] __attribute__((sda));

extern void *create_moby(int) __asm__("FUN_0020c4f8");
extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 probe_ground_height(void *, int, f32) __asm__("FUN_00213508");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern f32 square_root(f32) __asm__("FUN_001f9988");
extern s32 FUN_0022da68_c(s32, s32, void *) __asm__("FUN_0022da68");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 FUN_L00_00257b90(s32, s32);
extern f32 convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern void FUN_L00_00257470(void *, int, int);
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void set_moby_color(void *, u8, u8, u8) __asm__("FUN_L00_002502f0");
extern void FUN_L00_00269b70(void *, int, int, int, int, f32, f32);
extern void fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
extern void FUN_L00_002c92d8_c(void *) __asm__("FUN_L00_002c92d8");
extern void FUN_L00_00250df8(void *);

struct Moby *FUN_L00_002c9348(State *a) {
    struct Moby *n;
    Sub *s2;
    Loc loc;
    struct Moby *p;
    void *q;
    Opt *o;
    f32 g;
    f32 c1;
    f32 sn;

    n = (struct Moby *)create_moby(0x10E);
    if (n != NULL) {
        n->update_dist = 0x7F;
        n->draw_dist = 0x7F;
        n->visible = 1;
        s2 = (Sub *)n->pvars;
        if ((a->flags34 & 0x20) != 0) {
            o = a->unk78->unk0C;
            if (o != NULL) {
                set_moby_color(n, o->f4, o->f5, o->f6);
            } else {
                n->spawn_frame = a->unk38;
            }
        } else {
            n->spawn_frame = a->unk38;
        }
        n->anim_speed = random_float_between(0.85f, 1.15f);
        qcopy(&n->pos, &a->pos);
        clear_u64_value(&n->rot);
        n->rot.z = fast_arc_tan(n->pos.x - hero.motion.pos.f[0], n->pos.y - hero.motion.pos.f[1]);
        g = probe_ground_height(&n->pos, 0, 0.5f);
        if (n->pos.z <= g) {
            n->pos.z = g;
            n->pos.z = D_L00_00173E40.f28;
            qcopy(&loc.v, (u8 *)&D_L00_00173E40 + 0x40);
            c1 = fast_cos(n->rot.z);
            sn = fast_sin(n->rot.z);
            loc.t1 = loc.v.f[0] * c1 + loc.v.f[1] * sn;
            c1 = fast_cos(n->rot.z);
            sn = fast_sin(n->rot.z);
            loc.t2 = loc.v.f[1] * c1 - loc.v.f[0] * sn;
            loc.t3 = loc.v.f[2];
            g = fast_arc_tan(square_root(loc.t1 * loc.t1 + loc.t3 * loc.t3), loc.t2);
            n->rot.x = -g;
            n->rot.y = fast_arc_tan(loc.t3, loc.t1);
        }
        n->collision = ((Part *)n->pclass)->unk10;
        FUN_0022da68_c(1, 0, n);
        n->state = 1;
        s2->unk66 = 0;
        s2->unk64 = FUN_L00_00257b90(scale_game_frames(0x78), scale_game_frames(0x12C));
        g = convert_integer_to_float(D_0013E520[0x15]);
        s2->unk70 = -1;
        s2->unk78 = g * 3.0f + 1.0f;
        s2->unk74 = D_0013E520[0x15];
        n->command = s2->unk74;
        FUN_L00_00257470(a, 0, -1);
        q = suck_get_aim_height(a);
        if (q != NULL && *(u8 *)((u8 *)q + 0xE) == 1) {
            *(u8 *)((u8 *)q + 0xE) = 2;
        } else {
            mark_moby_for_removal(a);
        }
        c1 = 0.0f;
        FUN_L00_00269b70(&n->pos, 0x7F, 0x7F, 0x7F, scale_game_frames(0xF), 1000000.0f, c1);
        fill_transfer_words(s2, 0, 0x20);
        p = D_L00_001DD100[D_L00_00161854];
        if (p != NULL && p->oclass == 0x10E && p->state != 0xFE && p->state != 0xFD) {
            if (p->state == 5) {
                s2->unk78 = c1;
                n->flags &= ~0x20;
            } else if (!s2->unk74) {
                s2->unk14 = (void *)&D_L00_001DD150[D_L00_00161854];
                s2->unk10 = (void *)(D_L00_001DDF10 + D_L00_00161854 * 0x60);
                n->flags |= 0x1020;
                ((Node *)s2->unk14)->unk70 = (void *)D_L00_00161840;
            }
            ((Sub *)D_L00_001DD100[D_L00_00161854]->pvars)->unk70 = (void *)1;
        } else if (!s2->unk74) {
            s2->unk14 = (void *)&D_L00_001DD150[D_L00_00161854];
            s2->unk10 = (void *)(D_L00_001DDF10 + D_L00_00161854 * 0x60);
            n->flags |= 0x1020;
            ((Node *)s2->unk14)->unk70 = (void *)D_L00_00161840;
        }
        FUN_L00_002c92d8_c(s2->unk14);
        D_L00_001DD100[D_L00_00161854] = n;
        D_L00_00161854_gp = D_L00_00161854 + 1;
        D_L00_00161854 = (D_L00_00161854 + 1) % 20;
        if (s2->unk74) {
            n->flags &= ~0x20;
        }
        FUN_L00_00250df8(n);
    }
    return n;
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002c97e0.s", FUN_L00_002c97e0);
#else
extern f32 D_L00_0015F5D4_gp __asm__("D_L00_0015F5D4") __attribute__((sda));
extern f32 D_L00_0015F5D0_macro __asm__("D_L00_0015F5D0") __attribute__((section(".sdata")));
extern f32 D_L00_0015F5D4_macro __asm__("D_L00_0015F5D4") __attribute__((section(".sdata")));

extern s32 D_L00_00161850_2c97e0 __asm__("D_L00_00161850") __attribute__((sda));
extern f32 D_L00_00161858_2c97e0 __asm__("D_L00_00161858") __attribute__((sda));
extern f32 D_L00_0016185C_2c97e0 __asm__("D_L00_0016185C") __attribute__((sda));
extern f32 D_L00_00161860_2c97e0 __asm__("D_L00_00161860") __attribute__((sda));
extern f32 D_L00_00161864_2c97e0 __asm__("D_L00_00161864") __attribute__((sda));
extern f32 D_L00_00161868_2c97e0 __asm__("D_L00_00161868") __attribute__((sda));
extern f32 D_L00_0016186C_2c97e0 __asm__("D_L00_0016186C") __attribute__((sda));
extern s32 FUN_L00_00257b50_2c97e0(s32) __asm__("FUN_00213260");
extern f32 FUN_L00_00257cb0_2c97e0(void) __asm__("FUN_00213308");
extern void FUN_L00_00263ac8_2c97e0(struct Moby *, s32, void *, f32) __asm__("FUN_L00_00263ac8");
extern void *FUN_L00_0025a420_2c97e0(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 FUN_L00_00258490_2c97e0(void *, void *, void *, s32, f32, f32, f32, f32) __asm__("FUN_L00_00258490");
extern void FUN_L00_00258278_2c97e0(void *, void *, f32, f32, f32, f32) __asm__("FUN_L00_00258278");
extern s32 tick_timer_2cb418(void *) __asm__("FUN_001f9770");
extern f32 vector_length_xyz_2cb418(void *) __asm__("FUN_001f9af0");
extern f32 FUN_001fa688_2c97e0(void) __asm__("FUN_001fa688");
extern s32 FUN_001fa6d0_2c97e0(f32) __asm__("FUN_001fa6d0");
extern f32 distance_xy_2c97e0(void *, void *) __asm__("FUN_001f9b80");
extern f32 distance_xyz_2c97e0(void *, void *) __asm__("FUN_001f9b48");
extern f32 rotation_distance_2c97e0(f32, f32) __asm__("FUN_001fa688");
extern f32 rotation_delta_2c97e0(f32, f32) __asm__("FUN_001fa5c8");
extern void build_spherical_offset_2c97e0(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void FUN_L00_0026ced0_2c97e0(void *, void *, u32, u32, s32, f32) __asm__("FUN_L00_0026ced0");
extern s32 FUN_L00_001f2868_2c97e0(void *, s32, void *, s32, f32) __asm__("FUN_L00_001f2868");
extern void FUN_L00_0025a9f8_2c97e0(void *, void *, s32, s32, s32, s32, s32, s32, f32, f32, f32) __asm__("FUN_L00_0025a9f8");
extern void FUN_L00_0025e450_2c97e0(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, f32, f32, f32, f32, f32, f32, f32) __asm__("FUN_L00_0025e450");
extern void FUN_L00_002cbf68_2c97e0(void *, void *, void *) __asm__("FUN_L00_002cbf68");
extern s32 FUN_L00_002dc9f0_2c97e0(void *, void *) __asm__("FUN_L00_002dc9f0");
extern void FUN_L00_00257880_2c97e0(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern f32 FUN_L00_00259710_2c97e0(void *) __asm__("FUN_L00_00259710");
extern f32 fabs_2c97e0(f32) __asm__("FUN_001f99c0");
extern void normalize_2c97e0(void *, void *, f32) __asm__("FUN_001f9bf8");

/* Update for class 0x10e. State-specific inequalities and scale omissions
 * follow the level-00 instructions, including the close state-3 step. */
void morphoray_chicken_update(struct Moby *m) __asm__("FUN_L00_002c97e0");
void morphoray_chicken_update(struct Moby *m)
{
    V4 old_pos, next_pos, velocity, offset;
    Sub *s2 = (Sub *)m->pvars;
    s32 result;
    s32 i, count, a, b, mode;
    void *hit;
    f32 angle, scale, speed, f, g, h, rotation;
    f32 anim_speed = m->anim_speed;
    angle = atan2_f_2c8440(m->pos.x - hero.motion.pos.f[0], m->pos.y - hero.motion.pos.f[1]);
    if (m->state == 0) {
        m->state = 1;
        s2->unk66 = 0;
        s2->unk70 = -1;
        a = scale_game_frames(120);
        b = scale_game_frames(300);
        s2->unk64 = FUN_L00_00257b90(a, b);
        s2->unk78 = 1.0f;
    }
    if (m->state == 6) return;
    if (D_0013E520[0x15])
        FUN_L00_00263ac8_2c97e0(m, D_L00_00161850_2c97e0, (u8 *)s2 + 0x20, 5.0f);
    else
        FUN_L00_00263ac8_2c97e0(m, D_L00_00161850_2c97e0, (u8 *)s2 + 0x20, 7.7f);
    qcopy(&old_pos, &m->pos);
    if (s2->unk70 >= 0) {
        if (tick_countdown_2c8440(&s2->unk70) && !m->visible) goto remove;
        if (s2->unk70 >= 0) {
            f32 f;
            f = D_L00_0015F5D4_gp;
            if ((f > 0.9f || D_L00_0015F5D0_macro > 0.9f) && !m->visible && s2->unk70 != -1) {
                if (!FUN_L00_00257b50_2c97e0(199)) goto remove;
                s2->unk70 = FUN_L00_00257b90(20, 30);
                f = D_L00_0015F5D4_macro;
            }
            if (f > 1.0f || D_L00_0015F5D0_macro > 1.0f) {
                if (!m->visible) {
                    if (!FUN_L00_00257b50_2c97e0(24)) goto remove;
                } else if (!FUN_L00_00257b50_2c97e0(99)) s2->unk78 = 0.0f;
                else s2->unk70 = FUN_L00_00257b90(20, 30);
            }
        }
    }
    if ((s32)s2->unk74 > 0) {
        f = convert_integer_to_float(s2->unk74);
        m->scale += ((f * 3.0f + 1.0f) * m->pclass->scale - m->scale) * 0.1f;
    }
    scale = m->scale / m->pclass->scale;
    if (tick_countdown_2c8440((s32 *)((u8 *)s2 + 0x7c))) {
        hit = FUN_L00_0025a420_2c97e0(m, -1, 0);
        if (hit) {
            FUN_0022da68_c(1, 0, m);
            s2->unk78 -= *(f32 *)((u8 *)hit + 0x2c);
            *(s32 *)((u8 *)s2 + 0x7c) = scale_game_frames(25);
        }
    }
    m->hit_slot = 255;
    if (s2->unk78 < 1.0f) {
        if (s2->unk74 == 0) {
            i = 0;
            while (i < (s32)FUN_001fa6d0_2c97e0(scale) * 20) {
                i++;
                f = random_float_between(0.0f, 0.37f) * scale;
                g = FUN_L00_00257cb0_2c97e0();
                build_spherical_offset_2c97e0(&offset, f, g, FUN_L00_00257cb0_2c97e0());
                offset.f[2] += random_float_between(0.25f, 0.5f) * scale;
                add_vector_xyz_2c8440(&offset, &offset, &old_pos);
                f = random_float_between(D_0015ED6C * 0.1f, D_0015ED6C) * scale;
                g = FUN_L00_00257cb0_2c97e0(); h = FUN_L00_00257cb0_2c97e0();
                build_spherical_offset_2c97e0(&velocity, f, g, h);
                {
                    f32 g;
                    g = scale * 150000.0f;
                    a = scale_game_frames(30); b = scale_game_frames(57);
                    a = FUN_L00_00257b90(a, b);
                    FUN_L00_0026ced0_2c97e0(&offset, &velocity, 0x3f7f7f7f, 0x2f2f2f, a, g);
                }
            }
        } else {
            offset.q = 0;
            offset.f[0] = frame_scale * 0.5f;
            offset.f[1] = offset.f[0]; offset.f[2] = frame_scale * 0.7f;
            velocity.q = offset.q;
            FUN_L00_0025e450_2c97e0(m, &velocity, 0, 0, 0, 0, -1, 1, 1, -1, *(u8 *)&s2->unk74, 0.0f, 2.0f, 4.0f, 3.0f, 9.0f, 1.0f, 12.0f);
            a = FUN_L00_001f2868_2c97e0(&m->pos, 16, m, 0, 3.0f);
            offset.q = ((V4 *)&m->pos)->q;
            FUN_L00_0025a9f8_2c97e0(m, &offset, 0x177f00, a, 0, 0x830000, 2, 1, 1.0f, 1.0f, 1.0f);
        }
        count = FUN_L00_00257b90(5, 8);
        if (D_L00_0015F5D0_macro > 0.85f || D_L00_0015F5D4_macro > 0.85f) {
            if (D_L00_0015F5D0_macro > 1.0f || D_L00_0015F5D4_macro > 1.0f) count = 1;
            else count /= 2;
        }
        if (count > 0) {
            rotation = 0.017453292f;
            for (; count > 0; count--) {
                f = random_float_between(D_L00_00161868_2c97e0, D_L00_0016186C_2c97e0) * scale;
                g = FUN_L00_00257cb0_2c97e0();
                build_spherical_offset_2c97e0(&offset, f, g, FUN_L00_00257cb0_2c97e0());
                offset.f[2] += D_L00_00161864_2c97e0 * scale;
                add_vector_xyz_2c8440(&offset, &offset, &old_pos);
                {
                    f32 f;
                    f = random_float_between(D_L00_0016185C_2c97e0 * D_0015ED6C, D_L00_00161860_2c97e0 * D_0015ED6C);
                    g = FUN_L00_00257cb0_2c97e0();
                    build_spherical_offset_2c97e0(&velocity, f, g, D_L00_00161858_2c97e0 * rotation);
                }
                velocity.f[2] = random_float_between(3.5f, 5.5f) * D_0015ED6C;
                FUN_L00_002cbf68_2c97e0(m, &offset, &velocity);
            }
        }
        FUN_0022da68_c(1, 0, m);
        if (s2->unk70 >= 0) goto remove;
        m->collision = 0;
        m->flags = (m->flags & 0xefff) | 0x41;
        s2->unk74 = 0; m->state = 6; m->command = 0;
        return;
    }
    result = 0;
    if (tick_timer_2cb418(&s2->unk64)) {
        a = FUN_L00_00257b50_2c97e0(100);
        FUN_0022da68_c(a > 74, 0, m);
        a = scale_game_frames(120); b = scale_game_frames(300);
        s2->unk64 = FUN_L00_00257b90(a, b);
    }
    if (s2->unk70 < 0) speed = 0.333f;
    else speed = (D_0015ED6C + D_0015ED6C) * scale;
    switch (m->state) {
    case 1:
        if (m->anim_flags & 2) {
            a = FUN_L00_00257b50_2c97e0(2) == 0;
            b = scale_game_frames(5);
            FUN_L00_00257880_2c97e0(m, a, 0, b);
        }
        if (tick_timer_2cb418(&s2->unk66) && distance_xy_2c97e0(&m->pos, &hero.motion.pos) < scale * 5.0f) {
            next_pos.f[0] = fast_cos(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[1] = fast_sin(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[2] = 0.0f;
            add_vector_xyz_2c8440(&next_pos, &next_pos, &old_pos);
            result = FUN_L00_00258490_2c97e0(m, &old_pos, &next_pos, 0, scale * 0.2f, scale * 0.5f, speed, 0.2617994f);
            if (!result) {
                angle = atan2_f_2c8440(old_pos.f[0] - m->pos.x, old_pos.f[1] - m->pos.y);
                f = rotation_distance_2c97e0(m->rot.z, angle);
                if (!(f < 1.5707964f) || !(scale * 0.01f < distance_xyz_2c97e0(&m->pos, &old_pos))) goto reset_timer_only;
            }
            m->state = 3; a = scale_game_frames(10); FUN_L00_00257880_2c97e0(m, 3, 0, a); break;
        }
        if (distance_xy_2c97e0(&m->pos, &hero.motion.pos) < scale * 5.0f && rotation_distance_2c97e0(angle, m->rot.z) > 0.5235988f) {
            FUN_L00_00258490_2c97e0(m, &old_pos, &old_pos, 0, scale * 0.2f, scale * 0.5f, speed, 0.2617994f);
            m->state = 2; a = scale_game_frames(10); FUN_L00_00257880_2c97e0(m, 3, 0, a); break;
        }
idle_move:
        FUN_L00_00258490_2c97e0(m, &old_pos, &old_pos, 0, scale * 0.2f, scale * 0.5f, speed, 0.2617994f);
        break;
    case 2:
        rotation = 0.5f;
        FUN_L00_00258278_2c97e0(m, (u8 *)s2 + 0x68, angle, D_0015ED70 * 0.7853982f, D_0015ED70 * 1.5707964f, D_0015ED6C * 0.7853982f);
        FUN_L00_00258490_2c97e0(m, &old_pos, &old_pos, 0, scale * 0.2f, scale * rotation, speed, 0.2617994f);
        if (tick_timer_2cb418(&s2->unk66) && distance_xy_2c97e0(&m->pos, &hero.motion.pos) < scale * 5.0f) {
            next_pos.f[0] = fast_cos(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[1] = fast_sin(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[2] = 0.0f;
            add_vector_xyz_2c8440(&next_pos, &next_pos, &old_pos);
            f = probe_ground_height(&next_pos, 0, rotation);
            f = fabs_2c97e0(f - next_pos.f[2]);
            if (f <= (D_0015ED6C + D_0015ED6C) * scale) {
                m->state = 3; a = scale_game_frames(10); FUN_L00_00257880_2c97e0(m, 3, 0, a); break;
            }
            break;
        }
        if (rotation_distance_2c97e0(angle, m->rot.z) < 0.08726646f) goto reset_state;
        if (scale * 6.0f < distance_xy_2c97e0(&m->pos, &hero.motion.pos)) goto reset_state;
        break;
    case 3:
        if (scale * 6.0f < distance_xy_2c97e0(&m->pos, &hero.motion.pos)) {
            m->state = 1;
            a = scale_game_frames(60); b = scale_game_frames(180);
            s2->unk66 = FUN_L00_00257b90(a, b);
            a = FUN_L00_00257b50_2c97e0(2) == 0; b = scale_game_frames(5);
            FUN_L00_00257880_2c97e0(m, a, 0, b);
            goto idle_move;
        }
        if (!(distance_xy_2c97e0(&m->pos, &hero.motion.pos) < scale * 3.0f)) {
            FUN_L00_00258278_2c97e0(m, (u8 *)s2 + 0x68, angle, D_0015ED70 * 0.7853982f, D_0015ED70 * 1.5707964f, D_0015ED6C * 0.7853982f);
            rotation = 1.5707964f;
            next_pos.f[0] = fast_cos(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[1] = fast_sin(m->rot.z) * D_0015ED6C;
            goto move_scaled;
        }
        FUN_L00_00258278_2c97e0(m, (u8 *)s2 + 0x68, angle, D_0015ED70 * 0.7853982f, D_0015ED70 * 1.5707964f, D_0015ED6C * 0.7853982f);
        rotation = 1.5707964f;
        next_pos.f[0] = fast_cos(m->rot.z) * (D_0015ED6C + D_0015ED6C);
        next_pos.f[1] = fast_sin(m->rot.z) * (D_0015ED6C + D_0015ED6C);
        next_pos.f[2] = 0.0f;
        add_vector_xyz_2c8440(&next_pos, &next_pos, &old_pos);
        result = FUN_L00_00258490_2c97e0(m, &old_pos, &next_pos, 0, scale * 0.2f, scale * 0.5f, speed, 0.2617994f);
        if (!result) {
            angle = atan2_f_2c8440(old_pos.f[0] - m->pos.x, old_pos.f[1] - m->pos.y);
            f = rotation_distance_2c97e0(m->rot.z, angle);
            if (rotation < f || distance_xyz_2c97e0(&m->pos, &old_pos) < scale * 0.01f) goto reset_state;
        }
        m->state = 4; a = scale_game_frames(10); FUN_L00_00257880_2c97e0(m, 2, 0, a); break;
    case 4:
        result = tick_timer_2cb418(&s2->unk66);
        if (result && scale * 4.0f < distance_xy_2c97e0(&m->pos, &hero.motion.pos)) {
            FUN_L00_00258278_2c97e0(m, (u8 *)s2 + 0x68, angle, D_0015ED70 * 0.7853982f, D_0015ED70 * 1.5707964f, D_0015ED6C * 0.7853982f);
            rotation = 1.5707964f;
            next_pos.f[0] = fast_cos(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[1] = fast_sin(m->rot.z) * D_0015ED6C * scale;
            next_pos.f[2] = 0.0f;
            add_vector_xyz_2c8440(&next_pos, &next_pos, &old_pos);
            result = FUN_L00_00258490_2c97e0(m, &old_pos, &next_pos, 0, scale * 0.2f, scale * 0.5f, speed, 0.2617994f);
            if (!result) {
            angle = atan2_f_2c8440(old_pos.f[0] - m->pos.x, old_pos.f[1] - m->pos.y);
            f = rotation_distance_2c97e0(m->rot.z, angle);
            if (rotation < f || distance_xyz_2c97e0(&m->pos, &old_pos) < scale * 0.01f) goto reset_state;
        }
            m->state = 3; a = scale_game_frames(10); FUN_L00_00257880_2c97e0(m, 3, 0, a); break;
        }
        FUN_L00_00258278_2c97e0(m, (u8 *)s2 + 0x68, angle, D_0015ED70 * 0.7853982f, D_0015ED70 * 1.5707964f, D_0015ED6C * 0.7853982f);
        rotation = 1.5707964f;
        next_pos.f[0] = fast_cos(m->rot.z) * (D_0015ED6C + D_0015ED6C) * scale;
        next_pos.f[1] = fast_sin(m->rot.z) * (D_0015ED6C + D_0015ED6C);
move_scaled:
        next_pos.f[1] *= scale;
        next_pos.f[2] = 0.0f;
        add_vector_xyz_2c8440(&next_pos, &next_pos, &old_pos);
        result = FUN_L00_00258490_2c97e0(m, &old_pos, &next_pos, 0, scale * 0.2f, scale * 0.5f, speed, 0.2617994f);
        if (!result) {
            angle = atan2_f_2c8440(old_pos.f[0] - m->pos.x, old_pos.f[1] - m->pos.y);
            f = rotation_distance_2c97e0(m->rot.z, angle);
            if (rotation < f || distance_xyz_2c97e0(&m->pos, &old_pos) < scale * 0.01f) goto reset_state;
        }
        break;
reset_state:
        m->state = 1;
        a = scale_game_frames(60); b = scale_game_frames(180);
        s2->unk66 = FUN_L00_00257b90(a, b);
        a = FUN_L00_00257b50_2c97e0(2) == 0; mode = a; a = scale_game_frames(5);
        goto animate;
reset_timer_only:
        a = scale_game_frames(60); b = scale_game_frames(180);
        s2->unk66 = FUN_L00_00257b90(a, b);
        break;
animate:
        FUN_L00_00257880_2c97e0(m, mode, 0, a);
        break;
    case 5:
        result = FUN_L00_002dc9f0_2c97e0(m, s2->unk10);
        if (result) { m->state = 0; *(u16 *)((u8 *)s2->unk14 + 0x68) = 0; }
        if (FUN_L00_00257b50_2c97e0(4)) return;
        f = random_float_between(D_L00_00161868_2c97e0, D_L00_0016186C_2c97e0) * scale;
                g = FUN_L00_00257cb0_2c97e0();
                build_spherical_offset_2c97e0(&offset, f, g, FUN_L00_00257cb0_2c97e0());
                offset.f[2] += D_L00_00161864_2c97e0 * scale;
                add_vector_xyz_2c8440(&offset, &offset, &old_pos);
                {
                    f32 f;
                    f = random_float_between(D_L00_0016185C_2c97e0 * D_0015ED6C, D_L00_00161860_2c97e0 * D_0015ED6C);
                    g = FUN_L00_00257cb0_2c97e0();
                    build_spherical_offset_2c97e0(&velocity, f, g, D_L00_00161858_2c97e0 * 0.017453292f);
                }
                velocity.f[2] = random_float_between(3.5f, 5.5f) * D_0015ED6C;
                FUN_L00_002cbf68_2c97e0(m, &offset, &velocity);
        return;
    }
    {
        f32 f, g;
        if (!result) {
            subtract_vector_xyz_2c8440(&velocity, &old_pos, &m->pos);
            f = vector_length_xyz_2cb418(&velocity);
            if (D_0015ED6C * scale < f)
                normalize_2c97e0(&velocity, &velocity, D_0015ED6C * scale);
            add_vector_xyz_2c8440(&old_pos, &m->pos, &velocity);
            f = FUN_L00_00259710_2c97e0(&old_pos);
            g = fabs_2c97e0(f - m->pos.z);
            f = D_0015ED70;
            if (!(g <= D_0015ED6C * scale)) goto gravity;
        }
        qcopy(&m->pos, &old_pos);
        f = D_0015ED70;
    gravity:
        *(f32 *)((u8 *)s2 + 0x6c) += f * 9.8f;
        m->pos.z -= *(f32 *)((u8 *)s2 + 0x6c);
        {
            f32 f = probe_ground_height(&m->pos, 0, 0.5f);
            if (m->pos.z < f && f < m->pos.z + scale * 0.25f) {
                *(f32 *)((u8 *)s2 + 0x6c) = 0.0f; m->pos.z = f;
            }
        }
        if (!FUN_001f0b58_2c8440()) s2->unk78 = 0.0f;
        if (m->state != 5) {
            f32 f, g, angle;
            m->pos.z = D_L00_00173E40.f28;
            qcopy(&velocity, &D_L00_00173E40.v40);
            scale = 6.2831855f;
            f = fast_cos(m->rot.z); g = fast_sin(m->rot.z);
            offset.f[0] = velocity.f[0] * f + velocity.f[1] * g;
            f = fast_cos(m->rot.z); g = fast_sin(m->rot.z);
            offset.f[1] = velocity.f[1] * f - velocity.f[0] * g;
            offset.f[2] = velocity.f[2];
            g = -atan2_f_2c8440(square_root(offset.f[0] * offset.f[0] + offset.f[2] * offset.f[2]), offset.f[1]);
            angle = atan2_f_2c8440(offset.f[2], offset.f[0]);
            g = rotation_delta_2c97e0(g, m->rot.x);
            m->rot.x = fast_add_rotations_2c8440(m->rot.x, g * (D_0015ED6C * scale));
            f = rotation_delta_2c97e0(angle, m->rot.y);
            m->rot.y = fast_add_rotations_2c8440(m->rot.y, f * (D_0015ED6C * scale));
        }
    }
    if (!(m->pos.z < 0.0f)) { m->anim_speed = anim_speed; return; }
remove:
    mark_moby_for_removal(m);
}
#endif
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC210), where it is exact; names translated to the US level program. */

extern int FUN_L00_002db8f8(void *);

/* Moby state 5 while FUN_L00_002db8f8 reports a hit, back to 1 when it stops. */
int FUN_L00_002cadc0(char *a) {
    int r = FUN_L00_002db8f8(a);
    if (r == 0) {
        if (*(unsigned char *)(a + 0x20) == 5)
            a[0x20] = 1;
    } else {
        a[0x20] = 5;
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC2B0), where it is exact; names translated to the US level program. */

extern char *FUN_L00_002db890(char *);
extern int FUN_L00_002dbc30(char *, int, int, float);
extern void allocate_voice_for_target_entry(int, int, char *) __asm__("FUN_0022da68");

int FUN_L00_002cae60(char *m, int a, int b, float f) {
    char *h = FUN_L00_002db890(m);
    int r = FUN_L00_002dbc30(m, a, b, f);
    if (r != 0) {
        m[0x20] = 5;
    } else if ((unsigned char)m[0x20] == 5) {
        m[0x20] = 1;
    }
    if (*(short *)(h + 0x68) == 6) {
        allocate_voice_for_target_entry(1, 0, m);
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC360), where it is exact; names translated to the US level program. */

extern int FUN_L00_002dbe20(void *);

/* Runs FUN_L00_002dbe20 on the moby and sets its state to 5. */
int FUN_L00_002caf10(char *a) {
    int r = FUN_L00_002dbe20(a);
    a[0x20] = 5;
    return r;
}
/* Returns the word at +0x14 of the moby data, or 0 when either pointer is null. */
s32 FUN_L00_002caf40(s32 arg0) {
    s32 ptr;
    if (arg0 != 0) {
        ptr = *(s32 *)((u8 *)arg0 + 0x78);
        if (ptr != 0) {
            return *(s32 *)((u8 *)ptr + 0x14);
        }
        return 0;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC3C0), where it is exact; names translated to the US level program. */

typedef int w128 __attribute__((mode(TI)));

extern char *FUN_L00_0026daa0(char *, int, int, int, float);
extern char *func_0020D348_m(int);
extern char D_L00_00173E60_c[] __asm__("D_L00_00173E60");
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan_c2(f32, f32) __asm__("func_001F9E90");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern char D_L00_0015F580[] __attribute__((section(".sdata")));
extern float D_L00_00161890_gp __asm__("D_L00_00161890") __attribute__((sda));
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_L00_00257b90_c2(int, int) __asm__("FUN_L00_00257b90");
extern s32 scale_game_frames_c2(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern float D_L00_00161870 __attribute__((sda));
extern float D_L00_00161874 __attribute__((sda));
extern float D_L00_00161878 __attribute__((sda));
extern float D_L00_00161888 __attribute__((sda));
extern unsigned char *FUN_L00_00275158(char *a, char *b, int c, float f);
extern unsigned char D_0013E05A[];
extern unsigned char D_L00_00173E80[];
extern void *FUN_L00_0026dd20(void *, void *, unsigned, int, float);
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_002cbcc0_c(unsigned char *, int) __asm__("FUN_L00_002cbcc0");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
#include "rnc/gameplay/state/item_state.h"

char *FUN_L00_002caf70(char *owner, char *spawn_pos, char *target, float vx, float vy, float yaw, float pitch) {
    float start_position[4];
    float probe[4];
    float direction[4];
    float scaled[4];
    float spark[4];
    unsigned char *moby;
    char *vars;
    char *trail;
    unsigned char *variant;
    int result;
    int timer;
    int lifetime;
    int i;
    int sparks;
    float length;
    float blend;
    float index;
    float fraction;
    float *start;

    start = start_position;
    *(w128 *)start_position = *(w128 *)spawn_pos;
    moby = func_0020D348_m(0x131);
    if (moby != 0) {
        vars = *(char **)(moby + 0x78);
        *(char **)(vars + 0x20) = owner;
        moby[0x30] = 0x7F;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        *(long *)(moby + 0x38) = *(long *)(owner + 0x38);
        *(float *)(moby + 0x40) = D_L00_00161890_gp;
        *(float *)(moby + 0x44) = fast_add_rotations(pitch, (&D_L00_00161890_gp)[1]);
        *(float *)(moby + 0x48) = yaw;
        qcopy(moby + 0x10, start);
        if (target != 0) {
            int bounds;
            qcopy(vars, target + 0x10);
            bounds = (int)suck_get_aim_height(target);
            if (bounds != 0) {
                *(float *)(vars + 0x8) = *(float *)(vars + 0x8) + *(float *)(bounds + 0x10);
            } else {
                *(float *)(vars + 0x8) = *(float *)(vars + 0x8) + 0.5f;
            }
        } else {
            clear_u64_value_c(vars);
        }
        *(float *)(vars + 0x18) = vx;
        *(float *)(vars + 0x1C) = vy;
        *(int *)(vars + 0x10) = 0;
        *(int *)(vars + 0x14) = 0;
        timer = scale_game_frames_c2(0x23);
        variant = item_text_variant;
        timer = timer + variant[0xF] * 10;
        *(int *)(vars + 0x28) = 0;
        *(short *)(vars + 0x24) = timer;
        *(short *)(vars + 0x26) = timer;
        *(w128 *)probe = *(w128 *)(owner + 0x10);
        probe[2] = start[2];
        result = collision_line(probe, start, 0, (int)owner, 0);
        if (result != 0) {
            build_spherical_offset(direction, D_0015ED6C * 40.0f, *(float *)(moby + 0x48), -*(float *)(moby + 0x44));
            qcopy(moby + 0x10, D_L00_00173E60_c);
            start = scaled;
            sparks = 4;
            do {
                *(w128 *)spark = 0;
                spark[0] = random_float_between_c2(-1.0f, 1.0f);
                sparks--;
                spark[1] = random_float_between_c2(-1.0f, 1.0f);
                spark[2] = random_float_between_c2(-1.0f, 1.0f);
                *(w128 *)scaled = *(w128 *)spark;
                FUN_L00_001ff660(spark, direction, D_L00_00173E80);
                length = fast_vec_length(spark);
                fast_vec_normalize(start, start, length * 0.5f);
                add_vector_xyz(spark, spark, start);
                fast_vec_normalize(spark, spark, random_float_between_c2(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
                result = scale_game_frames_c2(10);
                timer = scale_game_frames_c2(15);
                lifetime = FUN_L00_00257b90_c2(result, timer);
                FUN_L00_0026dd20(moby + 0x10, (char *)spark, 0x7F2F4F6F, lifetime, 30000.0f);
            } while (sparks >= 0);
            variant = item_text_variant;
            if (variant[0xF] == 0) {
                FUN_L00_002cbcc0_c(moby, 0);
                mark_moby_for_removal_c2(moby);
                return 0;
            }
            *(int *)(vars + 0x28) = *(int *)(vars + 0x28) + 1;
            if (variant[0xF] + 5 < *(int *)(vars + 0x28)) {
                FUN_L00_002cbcc0_c(moby, 1);
                mark_moby_for_removal_c2(moby);
                return 0;
            }
            FUN_L00_001ff660(direction, direction, D_L00_00173E80);
            *(float *)(moby + 0x48) = fast_arc_tan_c2(direction[0], direction[1]);
            length = vector_length_xy(direction);
            *(float *)(moby + 0x44) = -fast_arc_tan_c2(length, direction[2]);
            return moby;
        } else {
            FUN_L00_0026daa0(moby, 0x2F4F7F7F, *(short *)(vars + 0x24), -1, D_L00_00161888);
            for (i = 0; i < 17; i++) {
                trail = (char *)FUN_L00_00275158(moby + 0x10, D_L00_0015F580, 1, 0.0f);
                if (trail != 0) {
                    *(float *)(trail + 0xC) = random_float_between_c2(D_L00_00161870, D_L00_00161874);
                    index = convert_integer_to_float_c(i);
                    fraction = index / convert_integer_to_float_c(16);
                    blend = cubic_hermite_scalar(-1.5f, 0.0f, 1.0f, 0.0f, fraction);
                    *(float *)(trail + 0xC) = *(float *)(trail + 0xC) + (D_L00_00161878 - *(float *)(trail + 0xC)) * blend;
                    result = truncate_float_to_s32_c(blend * -64.0f + 128.0f);
                    *(int *)(trail + 4) = (result << 24) | 0xFFFFFF;
                    ((int *)(vars + 0x2C))[i] = (int)trail;
                } else {
                    ((int *)(vars + 0x2C))[i] = 0;
                }
            }
        }
    }
    return moby;
}
/* Ported probe rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CC868), where it is exact; names translated to the US level program. */

/* Four floats aligned to 16 bytes, the layout moved by one lq/sq. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A;

typedef struct Moby Moby;

/* Vars of the homing shot FUN_L00_002caf70 spawns. */
typedef struct {
    f32 target_x, target_y, target_z;
    f32 unk0C;
    f32 yaw_rate;
    f32 pitch_rate;
    f32 rest_yaw;
    f32 rest_pitch;
    s32 ignore;
    s16 timer;
    s16 life;
    s32 bounces;
    Moby *follower;
    Moby *trail[16];
} ShotVars_2cb418;

typedef struct {
    Vec4A dir;
    Moby *moby;
    s32 flags;
    u8 b18;
    u8 b19;
    s16 oclass;
    f32 radius;
    s32 i20;
} ShotQuery_2cb418;

extern u8 D_0013E520_2cb418[] __asm__("D_0013E520");
extern unsigned char D_L00_00173E60[];      /* +0x20: hit normal */
extern char D_L00_00166DC0_2cb418[] __asm__("D_L00_00166DC0");
extern char D_L00_00166FF0_2cb418[] __asm__("D_L00_00166FF0");
extern char D_0013F3D0[];
extern Moby *D_001403E0_2cb418 __asm__("D_001403E0") __attribute__((section(".data")));
/* D_L00_00161894 again, through the symbol below it: retail's second load stays lui */
extern float D_0015ED70_lui __asm__("D_0015ED70");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern void FUN_L00_002cbcc0_2cb418(Moby *, s32) __asm__("FUN_L00_002cbcc0");
extern f32 vector_distance_2cb418(void *, void *) __asm__("FUN_001f9b80");
extern s32 random_int_between_2cb418(s32, s32) __asm__("FUN_L00_00257b90");
extern void spawn_spark_2cb418(void *, void *, s32, s32, f32) __asm__("FUN_L00_0026dd20");
extern void FUN_001f9c48_2cb418(void *, void *, f32) __asm__("FUN_001f9c48");
extern void normalize_vector_xyz_2cb418(void *, void *, f32) __asm__("FUN_001f9bf8");
extern f32 atan2_f_2cb418(f32, f32) __asm__("FUN_001f9e90");
extern f32 turn_toward_2cb418(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern void build_spherical_offset_2cb418(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void reflect_vector_2cb418(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern f32 random_float_between_2cb418(f32, f32) __asm__("FUN_002132a8");
extern void rotate_vector_2cb418(void *, void *, void *, f32) __asm__("FUN_00214890");
extern void scale_vector_xyz_2cb418(void *, void *, f32) __asm__("FUN_001f9a68");
extern void cross_vectors_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9ad8");
extern f32 vector_length_xy_2cb418(void *) __asm__("FUN_001f9b20");
extern void play_sound_2cb418(s32, s32, Moby *) __asm__("FUN_0022da68");
extern f32 int_to_float_2cb418(s32) __asm__("FUN_001fa6c0");
extern s32 random_integer_below_2cb418(s32) __asm__("FUN_00213260");
extern f32 random_float_2cb418(f32, f32) __asm__("FUN_L00_00257c48");
extern s32 tick_timer_2cb418(void *) __asm__("FUN_001f9770");
extern s32 collide_line_2cb418(void *, void *, s32, s32, void *) __asm__("FUN_001efa68");
extern s32 FUN_001f0b58_2cb418(void) __asm__("FUN_001f0b58");
extern s32 scale_game_frames_2cb418(s32) __asm__("FUN_001f96f8");
extern f32 fast_add_rotations_2cb418(f32, f32) __asm__("FUN_001fa580");
extern f32 vector_length_xyz_2cb418(void *) __asm__("FUN_001f9af0");
extern void add_vector_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2cb418(void *, void *, void *) __asm__("FUN_001f9a28");
extern void mark_moby_for_removal_2cb418(Moby *) __asm__("FUN_0020c828");

extern f32 D_L00_00161894[2];
extern f32 D_L00_00161890[2];
extern f32 jitter __asm__("D_L00_0016187C") __attribute__((sda));
extern f32 trail_speed __asm__("D_L00_00161880") __attribute__((sda));
extern f32 trail_decay __asm__("D_L00_00161884") __attribute__((sda));
void FUN_L00_002cb418(Moby *m) {
    Vec4A start;
    Vec4A step;
    ShotQuery_2cb418 query;
    Vec4A a;
    Vec4A b;
    Vec4A c;
    ShotVars_2cb418 *vars;
    float yaw;
    float pitch;
    float speed;
    float len;
    int i;
    int j;
    int left;

    vars = *(ShotVars_2cb418 **)&m->pvars;
    m->rot.y = fast_add_rotations_2cb418(m->rot.y, -D_L00_00161894[0]);
    if (vector_length_xyz_2cb418(vars) < 0.1f) {
        if (vars->timer < scale_game_frames_2cb418(0x23) - scale_game_frames_2cb418(1) && vars->bounces == 0) {
            yaw = vars->rest_yaw;
            pitch = vars->rest_pitch;
        } else {
            yaw = m->rot.z;
            pitch = m->rot.y;
        }
    } else {
        yaw = atan2_f_2cb418(vars->target_x - m->pos.x, vars->target_y - m->pos.y);
        pitch = -atan2_f_2cb418(vector_distance_2cb418((Vec4A *)&m->pos, vars), vars->target_z - m->pos.z);
    }
    if (vector_length_xyz_2cb418(vars) < 0.1f) {
        m->rot.z = turn_toward_2cb418(&vars->yaw_rate, m->rot.z, yaw, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 6.2831855f);
        m->rot.y = turn_toward_2cb418(&vars->pitch_rate, m->rot.y, pitch, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 6.2831855f);
    } else {
        m->rot.z = turn_toward_2cb418(&vars->yaw_rate, m->rot.z, yaw, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 4.712389f);
        m->rot.y = turn_toward_2cb418(&vars->pitch_rate, m->rot.y, pitch, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C_lui * 4.712389f);
    }
    build_spherical_offset_2cb418(&step, D_0015ED6C_lui * 40.0f, m->rot.z, -m->rot.y);
    qcopy(&start, (Vec4A *)&m->pos);
    add_vector_xyz_2cb418((Vec4A *)&m->pos, (Vec4A *)&m->pos, &step);
    if (m->pos.x < 0.0f || m->pos.y < 0.0f || m->pos.z < 0.0f
        || vector_distance_2cb418((Vec4A *)&m->pos, D_L00_00166DC0_2cb418) > 64.0f) {
        FUN_L00_002cbcc0_2cb418(m, 0);
        mark_moby_for_removal_2cb418(m);
        return;
    }
    query.flags = 0x10001;
    query.radius = 0.25f;
    query.moby = m;
    query.i20 = 1;
    subtract_vector_xyz_2cb418(&query, (Vec4A *)&m->pos, D_0013F3D0);
    FUN_001f9c48_2cb418(&query, &query, 1.0f);
    query.dir.z = 1.0f;
    query.b18 = 1;
    query.b19 = 1;
    query.dir.w = 5627.9248f;
    query.oclass = m->oclass;
    if (tick_timer_2cb418(&vars->timer) != 0) {
        FUN_L00_002cbcc0_2cb418(m, 0);
        mark_moby_for_removal_2cb418(m);
        return;
    }
    if (collide_line_2cb418(&start, (Vec4A *)&m->pos, 0, vars->ignore, &query) != 0) {
        if (FUN_001f0b58_2cb418() == 0) {
            if (vars->timer > scale_game_frames_2cb418(3)) {
                vars->timer = scale_game_frames_2cb418(3);
            }
        } else {
            qcopy((Vec4A *)&m->pos, ((char *)D_L00_00173E60));
            for (i = 0; i < 5; i++) {
                *(u128 *)&b = 0;
                b.x = random_float_between_2cb418(-1.0f, 1.0f);
                b.y = random_float_between_2cb418(-1.0f, 1.0f);
                b.z = random_float_between_2cb418(-1.0f, 1.0f);
                *(u128 *)&a = *(u128 *)&b;
                reflect_vector_2cb418(&b, &step, ((char *)D_L00_00173E60) + 0x20);
                normalize_vector_xyz_2cb418(&a, &a, vector_length_xyz_2cb418(&b) * 0.5f);
                add_vector_xyz_2cb418(&b, &b, &a);
                normalize_vector_xyz_2cb418(&b, &b, random_float_between_2cb418(D_0015ED6C_lui * 3.0f, D_0015ED6C_lui * 6.0f));
                spawn_spark_2cb418((Vec4A *)&m->pos, &b, 0x7F2F4F6F, random_int_between_2cb418(scale_game_frames_2cb418(10), scale_game_frames_2cb418(15)), 30000.0f);
            }
            if (D_0013E520_2cb418[15] == 0) {
                FUN_L00_002cbcc0_2cb418(m, 0);
                mark_moby_for_removal_2cb418(m);
                return;
            }
            vars->bounces++;
            switch (FUN_001f0b58_2cb418()) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 7:
            case 11:
            case 13:
                vars->bounces = 1000;
                break;
            }
            if (D_0013E520_2cb418[15] + 5 < vars->bounces) {
                FUN_L00_002cbcc0_2cb418(m, 1);
                mark_moby_for_removal_2cb418(m);
                return;
            }
            reflect_vector_2cb418(&a, &step, ((char *)D_L00_00173E60) + 0x20);
            m->rot.z = atan2_f_2cb418(a.x, a.y);
            m->rot.y = -atan2_f_2cb418(vector_length_xy_2cb418(&a), a.z);
            vars->yaw_rate = 0.0f;
            vars->pitch_rate = 0.0f;
            normalize_vector_xyz_2cb418(&b, ((char *)D_L00_00173E60) + 0x20, 0.05f);
            add_vector_xyz_2cb418((Vec4A *)&m->pos, ((char *)D_L00_00173E60), &b);
            if (D_001403E0_2cb418 != 0 && D_001403E0_2cb418->oclass == 0xA8
                && D_001403E0_2cb418->state != 0xFE && D_001403E0_2cb418->state != 0xFD) {
                play_sound_2cb418(4, 0, D_001403E0_2cb418);
            }
        }
    }
    m->rot.y = fast_add_rotations_2cb418(m->rot.y, D_L00_00161890[1]);
    left = vars->life - vars->timer;
    if (left < scale_game_frames_2cb418(13)) {
        speed = int_to_float_2cb418(left);
        speed = speed / int_to_float_2cb418(scale_game_frames_2cb418(13));
        speed = speed * trail_speed;
    } else {
        speed = trail_speed;
    }
    subtract_vector_xyz_2cb418(&step, &start, (Vec4A *)&m->pos);
    normalize_vector_xyz_2cb418(&step, &step, speed);
    qcopy(&a, (Vec4A *)&m->pos);
    if (vars->follower != 0) {
        qcopy((Vec4A *)&vars->follower->pos, &a);
    }
    for (j = 0; j < 16; j++) {
        Moby *t = vars->trail[j];
        if (t != 0) {
            add_vector_xyz_2cb418((Vec4A *)&t->pos, &a, &step);
            qcopy(&a, (Vec4A *)&t->pos);
            t->pos.x = t->pos.x + random_float_2cb418(0.0f, jitter);
            t->pos.y = t->pos.y + random_float_2cb418(0.0f, jitter);
            t->pos.z = t->pos.z + random_float_2cb418(0.0f, jitter);
            scale_vector_xyz_2cb418(&step, &step, trail_decay);
        }
    }
    if (random_integer_below_2cb418(5) != 0) {
        return;
    }
    cross_vectors_xyz_2cb418(&b, &step, D_L00_00166FF0_2cb418);
    rotate_vector_2cb418(&b, &b, &step, random_float_2cb418(0.0f, 3.1415927f));
    len = vector_length_xyz_2cb418(&b);
    if (len == 0.0f) {
        return;
    }
    scale_vector_xyz_2cb418(&c, &b, random_float_between_2cb418(D_0015ED6C_lui * 0.5f, D_0015ED6C_lui + D_0015ED6C_lui) / len);
    spawn_spark_2cb418((Vec4A *)&m->pos, &c, 0x7F2F4F6F, random_int_between_2cb418(scale_game_frames_2cb418(10), scale_game_frames_2cb418(15)), 30000.0f);
}
extern void FUN_00267a08_2cbcc0(void *) __asm__("FUN_L00_00267a08");
extern f32 FUN_L00_00257be8_2cbcc0(f32, f32) __asm__("FUN_L00_00257be8");
extern s32 FUN_L00_00257b90_2cbcc0(s32, s32) __asm__("FUN_L00_00257b90");
extern s32 FUN_L00_00257b50_2cbcc0(s32) __asm__("FUN_L00_00257b50");
extern s32 FUN_001fef20_2cbcc0(s32) __asm__("FUN_L00_001fef20");
extern void FUN_001ff500_2cbcc0(void *, void *, f32) __asm__("FUN_L00_001ff500");
extern void *FUN_0026dd20_2cbcc0(void *, void *, u32, s32, f32) __asm__("FUN_L00_0026dd20");
extern u8 *FUN_0026d000_2cbcc0(f32, f32, f32, f32, void *, s32, void *, u32) __asm__("FUN_L00_0026d000");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern float D_0015ED60;

void FUN_L00_002cbcc0(u8 *m, s32 enabled)
{
    s32 i;
    s32 j;
    void **handle;
    u8 *pos;
    V4 vectors[2];
    s32 lifetime;
    s32 color;
    s32 sign;
    u8 *effect;
    s32 alpha;

    handle = (void **)(*(u8 **)(m + 0x78) + 0x2c);
    for (i = 16; i >= 0; i--) {
        if (*handle != 0)
            FUN_00267a08_2cbcc0(*handle);
        *handle++ = 0;
    }
    pos = m + 0x10;

    if (enabled != 0) {
        V4 *vector = &vectors[1];
        for (j = 4; j >= 0; j--) {
            vectors[1].q = 0;
            vectors[1].f[0] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[1].f[1] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[1].f[2] = FUN_L00_00257be8_2cbcc0(-1.0f, 1.0f);
            vectors[0].q = vectors[1].q;
            FUN_001ff500_2cbcc0(vector, &vectors[0],
                FUN_L00_00257be8_2cbcc0(D_0015ED6C_lui * 3.0f, D_0015ED6C_lui * 6.0f));
            lifetime = FUN_L00_00257b90_2cbcc0(FUN_001fef20_2cbcc0(10), FUN_001fef20_2cbcc0(15));
            FUN_0026dd20_2cbcc0(pos, vector, 0x7f2f4f6f, lifetime, 35000.0f);
        }
    }

    {
        s32 j;
        for (j = 2; j >= 0; j--) {
            color = ((u32)FUN_L00_00257b90_2cbcc0(0x20, 0x80) << 24) | 0x606060;
            sign = FUN_L00_00257b50_2cbcc0(2) ? -1 : 1;
            vectors[0].q = 0;
            vectors[0].f[2] = 1.0f;
            vectors[0].f[3] = 1.0f;
            vectors[0].f[2] = FUN_L00_00257be8_2cbcc0(0.01f, 0.025f) * D_0015ED60;
            effect = FUN_0026d000_2cbcc0(0.2f, 1.0f, 1.02f, 140000.0f,
                pos, sign, &vectors[0], color);
            if (effect != 0) {
                *(s16 *)(effect + 0xa) = FUN_001fef20_2cbcc0(FUN_L00_00257b90_2cbcc0(50, 80));
                /* The direction is no longer needed; retain the effect field address. */
                i = (s32)(effect + 0x20);
                *(s32 *)(i + 4) = 2;
                ((u8 *)i)[0xa] = color >> 24;
                ((u8 *)i)[0xb] = effect[0xa];
                alpha = FUN_L00_00257b50_2cbcc0(2);
                if (alpha) {
                    /* The fixed low color byte is 0x60, so the alpha byte is 0x44. */
                    alpha = 0x44;
                    effect[3] = alpha;
                }
            }
        }
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern unsigned char D_0013E535_2cbf68 __asm__("D_0013E535") __attribute__((section(".data")));
unsigned char *FUN_0020c4f8_2cbf68(int) __asm__("FUN_0020c4f8");
float rndf_2cbf68(float, float) __asm__("FUN_002132a8");
float rnd_2cbf68(float, float) __asm__("FUN_L00_00257c48");
int FUN_001160d8_2cbf68(void) __asm__("FUN_001160d8");
float rnda_2cbf68(void) __asm__("FUN_00213308");
int rf_2cbf68(float) __asm__("FUN_001fa6d0");
int rndi_2cbf68(int, int) __asm__("FUN_L00_00257b90");
int fr_2cbf68(int) __asm__("FUN_001f96f8");
void FUN_L00_00250df8_2cbf68(void *) __asm__("FUN_L00_00250df8");
void FUN_L00_0025d1b8_2cbf68(void *) __asm__("FUN_L00_0025d1b8");

extern int D_L00_001618A4 __attribute__((sda));
extern float D_L00_001618A0 __attribute__((sda));
unsigned char *FUN_L00_002cbf68(int unused, void *pos, float *vel) {
    unsigned char *m = FUN_0020c4f8_2cbf68(0x1AC);
    if (m) {
        unsigned char *v;
        int a, b;
        m[0x30] = 0x40;
        *(unsigned short *)(m + 0x34) |= 0x1000;
        *(short *)(m + 0x32) = 0x20;
        m[0x31] = 1;
        v = *(unsigned char **)(m + 0x78);
        m[0x20] = 0;
        m[0xBC] = 0;
        qcopy(m + 0x10, pos);
        *(float *)(m + 0x18) += rndf_2cbf68(0.2f, 0.7f);
        *(float *)(m + 0x10) += vel[0] * 3.0f;
        *(float *)(m + 0x14) += vel[1] * 3.0f;
        qcopy(v, vel);
        *(float *)(m + 0x44) = rnd_2cbf68(0.17453292f, 0.5235988f);
        *(float *)(m + 0x40) = rndf_2cbf68(0.17453292f, 0.7853982f);
        {
            float t = rndf_2cbf68(200.0f, 370.0f) * DEG_TO_RAD * D_0015ED6C_lui;
            *(int *)(v + 0x24) = -1;
            *(float *)(v + 0x2C) = t;
        }
        if (FUN_001160d8_2cbf68() & 1) {
            *(int *)(v + 0x24) = 1;
            *(float *)(m + 0x44) = -*(float *)(m + 0x44);
        }
        *(float *)(m + 0x48) = rnda_2cbf68();
        *(float *)(v + 0x28) = -(rndf_2cbf68(200.0f, 550.0f) * DEG_TO_RAD * D_0015ED6C_lui);
        *(float *)(v + 0x10) = rnda_2cbf68();
        {
            float r = rnda_2cbf68();
            float n = (float)D_L00_001618A4;
            *(float *)(v + 0x14) = r;
            a = rf_2cbf68(n * 0.7f);
        }
        b = rf_2cbf68((float)D_L00_001618A4 * 1.2f);
        *(int *)(v + 0x1C) = fr_2cbf68(rndi_2cbf68(a, b));
        {
            float k = D_L00_001618A0;
            *(float *)(m + 0x2C) *= k;
        }
        if (D_0013E535_2cbf68)
            *(float *)(m + 0x2C) *= 2.0f;
        *(float *)(v + 0x20) = rndf_2cbf68(-1.2f, -0.4f) * D_0015ED6C_lui;
        FUN_L00_00250df8_2cbf68(m);
        FUN_L00_0025d1b8_2cbf68(m);
    }
    return m;
}
extern float D_0015ED70_lui __asm__("D_0015ED70");
void FUN_001f9a10_2cc210(void *, void *, void *) __asm__("FUN_001f9a10");
float FUN_001fa580_2cc210(float, float) __asm__("FUN_001fa580");
float FUN_001f99c0_2cc210(float) __asm__("FUN_001f99c0");
int FUN_001f9740_2cc210(void *) __asm__("FUN_001f9740");
void FUN_0020c828_2cc210(void *) __asm__("FUN_0020c828");
int FUN_001f96f8_2cc210(int) __asm__("FUN_001f96f8");
int FUN_L00_001fefc8_2cc210(void *) __asm__("FUN_L00_001fefc8");
extern float D_L00_001618A8 __attribute__((sda));
void FUN_L00_002cc210(unsigned char *m) {
    float *v = *(float **)(m + 0x78);
    float sc;
    FUN_001f9a10_2cc210(m + 0x10, m + 0x10, v);
    v[2] -= D_0015ED70_lui * 15.0f;
    sc = D_L00_001618A8;
    v[0] *= sc;
    v[1] *= sc;
    if (v[2] < v[8])
        v[2] = v[8];
    *(float *)(m + 0x48) = FUN_001fa580_2cc210(*(float *)(m + 0x48), v[10]);
    *(float *)(m + 0x40) = FUN_001fa580_2cc210(*(float *)(m + 0x40), v[11] * (float)((int *)v)[9]);
    if (0.7853982f < FUN_001f99c0_2cc210(*(float *)(m + 0x40))) {
        ((int *)v)[9] = -((int *)v)[9];
    }
    if (FUN_001f9740_2cc210(&v[7]) != 0 || m[0x31] == 0) {
        FUN_0020c828_2cc210(m);
    } else if (((int *)v)[7] < FUN_001f96f8_2cc210(0x1E) && FUN_L00_001fefc8_2cc210(m + 0x23)) {
        FUN_0020c828_2cc210(m);
    }
}
typedef unsigned int u128_2cc360 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2cc360 q;
    float f[4];
} V_2cc360;
typedef struct {
    char p0[0x46];
    short h46;
} C_2cc360;
typedef struct M_2cc360 {
    char p0[0x10];
    V_2cc360 pos;
    char p20[4];
    C_2cc360 *c;
    struct M_2cc360 *next;
    char p2c[5];
    unsigned char b31;
    short h32;
    unsigned short h34;
} M_2cc360;
extern M_2cc360 *D_L00_0015FFE4_2cc360 __asm__("D_L00_0015FFE4");
extern float *c338_2cc360(void *) __asm__("FUN_002141f8");
extern float dist_2cc360(void *, void *) __asm__("FUN_001f9b80");
extern float atan_2cc360(float, float) __asm__("FUN_001f9e90");
extern float diffrot_2cc360(float, float) __asm__("FUN_001fa688");
extern int coll_2cc360(void *, void *, int, void *, int) __asm__("FUN_001efa68");
M_2cc360 *FUN_L00_002cc360(float *pos, float *rot, M_2cc360 **excl, float maxYaw, float maxPitch,
                           float maxDist) {
    M_2cc360 *best = 0;
    float bestScore = 1.0e9f;
    M_2cc360 *m;
    float *info;
    V_2cc360 t;
    V_2cc360 *pp;
    float d, ys, ps, score;
    int i, found;
    for (m = D_L00_0015FFE4_2cc360; m; m = m->next) {
        if (m->h32 == 0)
            continue;
        if (excl) {
            found = 0;
            for (i = 0; i < 7; i++) {
                if (excl[i] == m) {
                    found = 1;
                    break;
                }
            }
            if (found)
                continue;
        } else if (!m->b31) {
            continue;
        }
        info = c338_2cc360(m);
        if (!info)
            continue;
        if (info[0] < 0.0f)
            continue;
        if (!(m->h34 & 0x1000))
            continue;
        if (!m)
            continue;
        if (!m->c)
            continue;
        if (m->c->h46 != 5)
            continue;
        pp = &m->pos;
        d = dist_2cc360(pos, pp);
        if (!(d < maxDist))
            continue;
        qcopy(&t, pp);
        t.f[2] += info[4] + 0.1f;
        ys = diffrot_2cc360(rot[2], atan_2cc360(t.f[0] - pos[0], t.f[1] - pos[1]));
        ys = ys * ys;
        if (ys < maxYaw * maxYaw) {
            ps = diffrot_2cc360(rot[1], atan_2cc360(d, t.f[2] - pos[2]));
            ps = ps * ps;
            if (!(ps < maxPitch * maxPitch))
                continue;
            if (5.0f < d) {
                ys = ys * ps * d;
                ys += d;
            } else {
                ys = d / 5.0f;
            }
            if (ys < bestScore) {
                if (coll_2cc360(pos, &t, 2, m, 0))
                    continue;
                bestScore = ys;
                best = m;
            }
        }
    }
    return best;
}
#include "rnc/input/pad_state.h"

#include "rnc/gameplay/state/usage_stats.h"
typedef struct { float m[3][4]; } __attribute__((aligned(16))) Mat3_2cc608;
typedef struct { float m[4][4]; } __attribute__((aligned(16))) Mat4_2cc608;
typedef struct {
    struct Moby *target;
    unsigned char shots;
    unsigned char slot;
    short shot_timer;
    int lock_timer;
    float height;
    short track_timer;
    short count;
    float life;
    struct Moby *recent[7];
} Ryno_2cc608;
typedef struct { float radius; char pad4[0xC]; float height; } Bounds_2cc608;

extern char D_L00_00166C80[];
extern Vec4 camera_rot_2cc608 __asm__("D_L00_00166DD0");
extern float camera_pos_2cc608[] __asm__("D_L00_00166DC0");
extern int play_frames_2cc608 __asm__("D_0015EEA4") __attribute__((section(".sdata")));
extern int D_0015ED84 __attribute__((section(".sdata")));
extern struct UsageStats item_usage_2cc608 __asm__("D_00141680");
extern void matcopy_2cc608(void *, void *) __asm__("FUN_001fa2b8");
extern void transpose_2cc608(void *, void *) __asm__("FUN_001fa2d8");
extern int timer32_2cc608(int *) __asm__("FUN_001f9740");
extern int timer16_2cc608(short *) __asm__("FUN_001f9770");
extern void muzzle_2cc608(struct Moby *, int, float *) __asm__("FUN_L00_0024f7c8");
extern struct Moby *FUN_L00_002cc360_c(float *, float *, struct Moby **, float, float, float) __asm__("FUN_L00_002cc360");
extern int ticks_2cc608(int) __asm__("FUN_001f96f8");
extern Bounds_2cc608 *bounds_2cc608(struct Moby *) __asm__("FUN_002141f8");
extern int has_ammo_2cc608(int) __asm__("FUN_L00_00233e98");
extern int use_ammo_2cc608(int, int) __asm__("FUN_L00_00233db8");
extern float distance_2cc608(void *, void *) __asm__("FUN_001f9b48");
extern float fabs_2cc608(float) __asm__("FUN_001f99c0");
extern void reticle_2cc608(struct Moby *, int, float, float, float *, int, int, float, int) __asm__("FUN_L00_001edf60");
extern void hud_2cc608(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_0020e4c8(void);
extern void FUN_L00_0020e698(void);
extern void apply_mat4_2cc608(float *, float *, void *) __asm__("FUN_001f9d20");
extern void apply_mat3_2cc608(float *, float *, void *) __asm__("FUN_001f9cf8");
extern void add_2cc608(float *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_2cc608(float *, float *, float) __asm__("FUN_001f9a68");
extern float angle_2cc608(float, float) __asm__("FUN_001f9e90");
extern float length_2cc608(float *) __asm__("FUN_001f9b20");
extern unsigned char *FUN_L00_002ccee0_c(struct Moby *, float *, struct Moby *, float, float, float) __asm__("FUN_L00_002ccee0");
extern int rand_2cc608(void) __asm__("FUN_001160d8");

/* Ryno (class 454) update: target lock and reticle while held, then the eight-missile salvo state machine. */
void ryno_update(struct Moby *m) __asm__("FUN_L00_002cc608");
void ryno_update(struct Moby *m) {
    Mat3_2cc608 cam;
    Mat4_2cc608 view;
    Vec4 aim;
    Vec4 dir;
    Vec4 ofs;
    Ryno_2cc608 *d = *(Ryno_2cc608 **)&m->pvars;
    struct Moby *r;
    Bounds_2cc608 *b;
    float yaw, pitch;
    int i;

    matcopy_2cc608(&cam, D_L00_00166C80);
    transpose_2cc608(&view, &cam);
    if ((unsigned int)(m->state - 2) < 2) {
        if (timer32_2cc608(&d->lock_timer) || d->target == 0 || d->target->state == 0xFE || d->target->state == 0xFD) {
            d->target = 0;
            d->height = 0;
        }
        qcopy(aim.f, camera_rot_2cc608.f);
        aim.f[1] = -aim.f[1];
        muzzle_2cc608(m, d->slot, dir.f);
        r = FUN_L00_002cc360_c(dir.f, aim.f, 0, 1.6981583f, 1.6981583f, 80.0f);
        if ((r != 0 && d->target == 0) || d->target == 0 || d->target->state == 0xFE || d->target->state == 0xFD || d->target->visible == 0) {
            d->height = 0;
            d->target = r;
            d->lock_timer = ticks_2cc608(20);
            d->life = 1.0f;
            b = bounds_2cc608(d->target);
            if (b != 0) {
                d->height = b->height;
                d->life = b->radius;
            }
        }
        if (d->target != 0 && timer16_2cc608(&d->track_timer) && has_ammo_2cc608(-1)) {
            if (d->target == 0 || d->target->state == 0xFE || d->target->state == 0xFD) {
                d->target = 0;
                d->height = 0;
            } else {
                float f = distance_2cc608(camera_pos_2cc608, ((float *)&d->target->pos)) / 150.0f;
                qcopy(aim.f, ((float *)&d->target->pos));
                f = f * 0.65f;
                aim.f[2] = aim.f[2] + d->height;
                reticle_2cc608(m, 0xFF0FFF0F, fabs_2cc608(0.9f - f), 0.0f, aim.f, 0x23, -1, 90.0f, 4);
            }
        }
    }
    switch (m->state) {
    case 0:
        d->target = 0;
        hero.items[0].unk1C = 0;
        m->state = 1;
        d->track_timer = 0;
        break;
    case 1:
        if (m->anim_flags & 2) m->state = 2;
        break;
    case 2:
        if (hero.state.current == 1) hud_2cc608(0x1E, 1);
        if ((controller_state.pressed & hero.items[0].button_mask) == 0) return;
        if (hero.unk20AC != 0) return;
        if (use_ammo_2cc608(-1, 1) == 0) return;
        {
            int per;
            struct UsageStats *s = &item_usage_2cc608;
            if (s->stat[23].count <= 0xFFFE) s->stat[23].count++;
            per = 600;
            if (ticks_2cc608(play_frames_2cc608) / per > s->stat[23].unk2) s->stat[23].unk2 = ticks_2cc608(play_frames_2cc608) / 600;
            s->stat[23].level_mask = s->stat[23].level_mask | (1 << D_0015ED84) | 0x80000000;
        }
        hero.items[0].unk1C = 2;
        m->state = 4;
        d->shot_timer = 0;
        d->slot = 0;
        d->shots = 0;
        FUN_L00_0020e4c8();
        {
            int k;
            for (k = 6; k >= 0; k--) d->recent[k] = 0;
        }
        d->count = 0;
        break;
    case 3:
        reticle_2cc608(m, 0xFF0F0FFF, 1.0f, 0.0f, 0, 0x23, -1, 90.0f, 4);
        if ((controller_state.held & 5) == 0) {
            m->state = 2;
            return;
        }
        if ((controller_state.pressed & hero.items[0].button_mask) == 0) return;
        if (hero.unk20AC != 0) return;
        if (use_ammo_2cc608(-1, 1) == 0) return;
        {
            int per;
            struct UsageStats *s = &item_usage_2cc608;
            if (s->stat[23].count <= 0xFFFE) s->stat[23].count++;
            per = 600;
            if (ticks_2cc608(play_frames_2cc608) / per > s->stat[23].unk2) s->stat[23].unk2 = ticks_2cc608(play_frames_2cc608) / 600;
            s->stat[23].level_mask = s->stat[23].level_mask | (1 << D_0015ED84) | 0x80000000;
        }
        hero.items[0].unk1C = 2;
        m->state = 4;
        d->shot_timer = 0;
        d->slot = 0;
        d->shots = 0;
        FUN_L00_0020e4c8();
        {
            int k;
            for (k = 6; k >= 0; k--) d->recent[k] = 0;
        }
        d->count = 0;
        break;
    case 4:
        if (timer16_2cc608(&d->shot_timer) == 0) return;
        d->shot_timer = ticks_2cc608(9);
        d->slot++;
        d->slot = d->slot % 9;
        d->shots++;
        dir.q = 0;
        if (hero.state.current == 30) {
            dir.f[2] = 1.0f;
            apply_mat4_2cc608(dir.f, dir.f, &view);
        } else {
            dir.f[0] = 1.0f;
            apply_mat3_2cc608(dir.f, dir.f,  hero.basis);
        }
        add_2cc608(ofs.f, &hero.motion.displacement, &hero.motion.platform_carry);
        scale_2cc608(ofs.f, ofs.f, 3.0f);
        add_2cc608(dir.f, dir.f, ofs.f);
        if (hero.items[6].unk40 != 0) {
            yaw = hero.items[6].unk4C;
        } else {
            yaw = angle_2cc608(dir.f[0], dir.f[1]);
        }
        pitch = -angle_2cc608(length_2cc608(dir.f), dir.f[2]);
        muzzle_2cc608(m, d->slot, aim.f);
        FUN_L00_0020e4c8();
        FUN_L00_002ccee0_c(m, aim.f, d->target, d->height, yaw, pitch);
        if (d->shots >= 7) {
            hero.items[0].unk1C = 0;
            FUN_L00_0020e698();
            m->state = 5;
            d->shot_timer = ticks_2cc608(60);
            d->track_timer = ticks_2cc608(60);
        }
        d->life = d->life - 1.0f;
        if (d->life <= 0.0f || d->target == 0 || d->target->state == 0xFE || d->target->state == 0xFD) {
            d->recent[d->count] = d->target;
            d->count++;
            d->count = d->count % 7;
            qcopy(aim.f, camera_rot_2cc608.f);
            aim.f[1] = -aim.f[1];
            d->target = FUN_L00_002cc360_c(((float *)&m->pos), aim.f, d->recent, 3.1415927f, 3.1415927f, 100.0f);
            if (d->target == 0 && d->count > 0 && (rand_2cc608() & (d->count + 1)) == 0) {
                d->target = d->recent[rand_2cc608() % d->count];
            }
            d->height = 0;
            d->lock_timer = ticks_2cc608(20);
            d->life = 1.0f;
            b = bounds_2cc608(d->target);
            if (b != 0) {
                d->height = b->height;
                d->life = b->radius;
            }
        }
        break;
    case 5:
        if (timer16_2cc608(&d->shot_timer) == 0) return;
        d->shots = 0;
        d->target = 0;
        m->state = 2;
        break;
    case 6:
    case 7:
    case 8:
        break;
    }
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2ccee0;
extern float D_0015ED60;
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern unsigned char *D_001413D0_2ccee0 __asm__("D_001413D0") __attribute__((section(".data")));
extern unsigned char D_L00_00173E60_2ccee0[] __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
extern unsigned char *FUN_0020c4f8_2ccee0(int) __asm__("FUN_0020c4f8");
extern unsigned char *FUN_002141f8_2ccee0(void *) __asm__("FUN_002141f8");
extern int FUN_001f96f8_2ccee0(int) __asm__("FUN_001f96f8");
extern float FUN_002132a8_2ccee0(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_00250df8_2ccee0(void *) __asm__("FUN_L00_00250df8");
extern void FUN_0022da68_2ccee0(int, int, void *) __asm__("FUN_0022da68");
extern int FUN_001efa68_2ccee0(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_00214db0_2ccee0(void *, float, float, float) __asm__("FUN_00214db0");
extern void FUN_L00_001ff660_2ccee0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_001f9bf8_2ccee0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_0025e450_2ccee0(void *, void *, void *, float, float, int, int, int, float,
                                    float, float, float, int, float, int, int, int,
                                    int) __asm__("FUN_L00_0025e450");
extern void FUN_0020c828_2ccee0(void *) __asm__("FUN_0020c828");

unsigned char *FUN_L00_002ccee0(int a, V_2ccee0 *pos, unsigned char *tgt, float f12, float f13,
                                float f14) {
    V_2ccee0 w;
    V_2ccee0 s;
    V_2ccee0 dir;
    V_2ccee0 z;
    unsigned char *m;
    unsigned char *v;
    unsigned char *r;
    unsigned char *p;
    float t;
    float f;

    m = FUN_0020c4f8_2ccee0(0x1C9);
    if (m != 0) {
        v = *(unsigned char **)(m + 0x78);
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        *(float *)(m + 0x48) = f13;
        *(float *)(m + 0x44) = f14;
        m[0x20] = 0;
        *(int *)(v + 0x20) = a;
        qcopy(m + 0x10, pos);
        qcopy(v + 0x10, pos);
        *(int *)(v + 0x34) = 0;
        *(int *)(v + 0x38) = 0;
        *(int *)(v + 0x30) = 0;
        *(unsigned char **)(v + 0x28) = tgt;
        *(float *)(m + 0x2C) = *(float *)(*(unsigned char **)(m + 0x24) + 0x24) * 0.25f;
        if (tgt != 0) {
            *(int *)(v + 0x58) = *(short *)(tgt + 0xA6);
            r = FUN_002141f8_2ccee0(tgt);
            if (r != 0) {
                *(unsigned short *)(r + 0x1E) |= 0x80;
            }
            qcopy(v, tgt + 0x10);
            *(float *)(v + 0x2C) = f12;
            *(float *)(v + 8) += f12;
        }
        *(int *)(v + 0x24) = FUN_001f96f8_2ccee0(0xFA);
        *(int *)(v + 0x44) = 0;
        *(float *)(v + 0x48) = FUN_002132a8_2ccee0(-3.1415927f, 3.1415927f);
        f = FUN_002132a8_2ccee0(-3.1415927f, 3.1415927f);
        t = D_0015ED60;
        t *= 0.00967f;
        *(float *)(v + 0x4C) = f;
        f = FUN_002132a8_2ccee0(-t, t);
        t = D_0015ED60;
        t *= 0.02406f;
        *(float *)(v + 0x50) = f;
        *(float *)(v + 0x54) = FUN_002132a8_2ccee0(-t, t);
        FUN_L00_00250df8_2ccee0(m);
        FUN_0022da68_2ccee0(1, 0, m);
        z.q = 0;
        z.f[2] = D_0015ED6C_lui * 8.0f;
        p = D_001413D0_2ccee0;
        dir.q = z.q;
        s.q = ((V_2ccee0 *)(p + 0x10))->q;
        s.f[2] = pos->f[2];
        if (FUN_001efa68_2ccee0(&s, pos, 0, p, 0)) {
            m[0xBC] = 2;
            qcopy(m + 0x10, D_L00_00173E60_2ccee0);
            FUN_00214db0_2ccee0(&w, *(float *)(v + 0x30), *(float *)(m + 0x48),
                                -*(float *)(m + 0x44));
            FUN_L00_001ff660_2ccee0(&dir, &w, D_L00_00173E60_2ccee0 + 0x20);
            FUN_001f9bf8_2ccee0(&dir, &dir, D_0015ED6C_lui + D_0015ED6C_lui);
            FUN_L00_0025e450_2ccee0(m, &dir, 0, 0.0f, 0.0f, 10, 3, 9, 4.0f, 2.0f, 9.0f, 1.0f, 0,
                                    15.0f, 1, 1, -1, 0);
            FUN_0020c828_2ccee0(m);
            return 0;
        }
    }
    return m;
}
/* Rocket moby update: steers the rocket toward its target, moves it with collision retries, then checks hits and range. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CE6A0), where it is exact; names translated to the US level program. */

typedef int u128_cd __attribute__((mode(TI)));

typedef struct { float f[4]; } __attribute__((aligned(16))) V4_cd;

typedef struct { float x, y, z, w; char *p; int c; unsigned char b18; unsigned char b19; unsigned short s; float e; int i; } RocketHit;

extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED60;
extern float D_0015ED64;
extern float D_0015ED6C;
extern float D_0015ED70;
extern float FUN_L00_001ff430(void *);
extern float FUN_L00_00258110(float *vel, float cur, float target, float k, float d, float max);
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern Data D_L00_00173E40;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_L00_00257b90_c(int, int) __asm__("FUN_L00_00257b90");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 rand(void);
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
void spawn_explosion(char *m, void *unused, char *pos, float fA, float fB, int n, int k8, int k9, float fC, float fD, float fE, float fF, int k10, float fG, int k11, int s16, int s18, unsigned char flag) __asm__("FUN_L00_0025e450");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");

void rocket_update(char *m) __asm__("FUN_L00_002cd1f0");
void rocket_update(char *m) {
    char *d;
    char *q;
    char *p;
    char *gg;
    V4_cd v20;
    V4_cd dir;
    V4_cd z;
    V4_cd v50;
    union { struct { V4_cd a; V4_cd b; V4_cd c; } v; RocketHit h; } u;
    V4_cd v90;
    V4_cd va0[4];
    int r;
    unsigned char flag;
    float t;
    float s;
    float k;
    float k60;
    float a;
    float g;
    float sp;
    float zf;

    if (m == 0) return;
    d = *(char **)(m + 0x78);
    if (d == 0) return;

    a = *(float *)(m + 0x2C);
    k = D_0015ED6C;
    *(u128_cd *)&z = 0;
    z.f[2] = k * 8.0f;
    *(u128_cd *)&dir = *(u128_cd *)&z;
    k60 = D_0015ED60;
    {
        float k64 = D_0015ED64;
        *(float *)(m + 0x2C) = a + (*(float *)(*(char **)(m + 0x24) + 0x24) - a) * (k60 * 0.05f);
        {
            float e = *(float *)(d + 0x30);
            *(float *)(d + 0x30) = e + (k * 20.0f - e) * (k60 * 0.1f) * k64;
        }
    }
    r = scale_game_frames_c(250);
    r -= scale_game_frames_c(5);
    if (*(int *)(d + 0x24) < r) {
        q = *(char **)(d + 0x28);
        if (q == 0 || *(short *)(q + 0xA6) != *(int *)(d + 0x58) || *(unsigned char *)(q + 0x20) == 0xFE || *(unsigned char *)(q + 0x20) == 0xFD) {
            *(float *)(d + 0x44) = *(float *)(d + 0x44) + D_0015ED60 * 0.1f;
            *(int *)(d + 0x28) = 0;
            r = scale_game_frames_c(250);
            r -= scale_game_frames_c(100);
            if (*(int *)(d + 0x24) < r) {
                *(float *)(m + 0x44) = fast_add_rotations(*(float *)(m + 0x44), fast_sin_c(*(float *)(d + 0x48)) * (D_0015ED60 * 0.04f));
            }
            *(float *)(m + 0x48) = fast_add_rotations(*(float *)(m + 0x48), fast_cos_c(*(float *)(d + 0x48)) * (D_0015ED60 * 0.05f));
        } else {
            qcopy(&u.v.c, q + 0x10);
            u.v.c.f[2] = u.v.c.f[2] + *(float *)(d + 0x2C);
            subtract_vector_xyz(&u.v.b, &u.v.c, d);
            subtract_vector_xyz(&u.v.a, &u.v.c, d + 0x10);
            qcopy(d, &u.v.c);
            {
                float r1 = FUN_L00_001ff430(&u.v.b);
                float fB = (u.v.b.f[0] * u.v.a.f[0] + u.v.b.f[1] * u.v.a.f[1] + u.v.b.f[2] * u.v.a.f[2]) * -2.0f;
                float fA = *(float *)(d + 0x30) * *(float *)(d + 0x30) - r1;
                float r2f = FUN_L00_001ff430(&u.v.a);
                float sq = square_root_scalar(fB * fB - (4.0f * fA) * (-r2f));
                float t1 = (-fB + sq) / (fA + fA);
                float t2 = (-fB - sq) / (fA + fA);
                if (0.0f < t1 && 0.0f < t2) {
                    t = (t2 < t1) ? t1 : t2;
                } else if (0.0f < t1) {
                    t = t1;
                } else if (0.0f < t2) {
                    t = t2;
                } else {
                    t = -1.0f;
                }
            }
            if (0.0f < t) {
                fast_vec_scale(&v90, &u.v.b, t);
                add_vector_xyz(&v90, &v90, &u.v.a);
                g = fast_arc_tan_c(v90.f[0], v90.f[1]);
                *(float *)(m + 0x48) = FUN_L00_00258110((float *)(d + 0x34), *(float *)(m + 0x48), g, D_0015ED70 * 188.49556f, 0.0f, 0.0f);
                g = fast_arc_tan_c(vector_length_xy(&v90), v90.f[2]);
                *(float *)(m + 0x44) = FUN_L00_00258110((float *)(d + 0x38), *(float *)(m + 0x44), -g, D_0015ED70 * 188.49556f, 0.0f, 0.0f);
            }
            s = fast_vec_length(&u.v.a);
            if (s < 6.0f) {
                *(float *)(d + 0x44) = *(float *)(d + 0x44) * (s / 6.0f);
            } else {
                *(float *)(d + 0x44) = *(float *)(d + 0x44) + D_0015ED60 * 0.1f;
            }
        }
    }

    build_spherical_offset(&v50, *(float *)(d + 0x30), *(float *)(m + 0x48), -*(float *)(m + 0x44));
    *(u128_cd *)&u.v.b = 0;
    u.v.b.f[0] = random_float_between_c(-1.0f, 1.0f);
    u.v.b.f[1] = random_float_between_c(-1.0f, 1.0f);
    u.v.b.f[2] = random_float_between_c(-1.0f, 1.0f);
    *(u128_cd *)&u.v.a = *(u128_cd *)&u.v.b;
    fast_vec_normalize(&u.v.a, &u.v.a, random_float_between_c(0.1f, 0.2f) * D_0015ED6C);
    fast_vec_normalize(&z, &v50, -random_float_between_c(D_0015ED6C * 0.1f, D_0015ED6C));
    add_vector_xyz(&u.v.a, &u.v.a, &z);
    fast_vec_normalize(&z, &v50, random_float_between_c(0.0f, 1.0f) * *(float *)(d + 0x30));
    add_vector_xyz(&z, &z, m + 0x10);
    {
        int c1 = scale_game_frames_c(FUN_L00_00257b90_c(0x1C, 0x28));
        FUN_L00_00269958(&z, &u.v.a, 0x6F00AFFF, 0x100000FF, c1, 0x28, FUN_L00_00257b90_c(0x28, 0x46), 1);
    }
    qcopy(&v20, m + 0x10);
    add_vector_xyz(d + 0x10, d + 0x10, &v50);
    *(float *)(d + 0x48) = fast_add_rotations(*(float *)(d + 0x48), *(float *)(d + 0x50));
    *(float *)(d + 0x4C) = fast_add_rotations(*(float *)(d + 0x4C), *(float *)(d + 0x54));
    *(float *)(d + 0x48) = fast_add_rotations(*(float *)(d + 0x48), fast_sin_c(*(float *)(d + 0x4C)) * 0.15f);
    if (0.9f < *(float *)(d + 0x44)) *(float *)(d + 0x44) = 0.9f;

    flag = 0;
    goto body;
    for (;;) {
        float a1;
        if (!(0.1f < sp)) goto done;
    body:
        a1 = fast_sin_c(*(float *)(d + 0x48));
        u.v.a.f[1] = *(float *)(d + 0x44) * a1 * fast_cos_c(*(float *)(d + 0x4C));
        a1 = fast_cos_c(*(float *)(d + 0x48));
        u.v.a.f[2] = *(float *)(d + 0x44) * a1 * fast_cos_c(*(float *)(d + 0x4C));
        zf = 0.0f;
        u.v.a.f[3] = zf;
        u.v.a.f[0] = zf;
        euler_rotation_matrix(va0, m + 0x40);
        transform_vector(&u.v.a, &u.v.a, va0);
        add_vector_xyz(m + 0x10, d + 0x10, &u.v.a);
        if (collision_line(&v20, m + 0x10, 6, (int)m, 0) == 0) goto done;
        goto hit;
    top:;
    }
hit:
    flag = 1;
    sp = *(float *)(d + 0x44) * 0.7f;
    *(float *)(d + 0x44) = sp;
    goto top;
done:

    if (flag) {
        *(float *)(m + 0x48) = fast_arc_tan_c(*(float *)(m + 0x10) - v20.f[0], *(float *)(m + 0x14) - v20.f[1]);
        *(float *)(m + 0x44) = -fast_arc_tan_c(vector_distance_xy_alt(&v20, m + 0x10), *(float *)(m + 0x18) - v20.f[2]);
        *(int *)(d + 0x34) = 0;
        *(int *)(d + 0x38) = 0;
    }

    if (*(float *)(m + 0x10) < 0.0f || *(float *)(m + 0x14) < 0.0f || *(float *)(m + 0x18) < 0.0f) goto del;
    u.h.c = 0x830000;
    u.h.e = 3.0f;
    u.h.p = m;
    u.h.i = 1;
    qcopy(&u.v.a, &v50);
    set_vector_length_xy(&u.v.a, &u.v.a, 1.0f);
    u.h.z = 1.0f;
    u.h.b18 = 3;
    u.h.w = 5627.925f;
    u.h.b19 = 3;
    u.h.s = *(unsigned short *)(m + 0xA6);
    if (collision_line(&v20, m + 0x10, 0, (int)m, (int)&u.v.a)) {
        gg = (char *)((int *)&D_L00_00173E40);
        p = *(char **)(gg + 0x18);
        if (p != 0) {
            if (p == (char *)D_001413D0_2ccee0 || p == *(char **)(d + 0x20)
                || *(short *)(p + 0xA6) == 0x1C9 || *(short *)(p + 0xA6) == 0) goto keep;
            m[0xBC] = 2;
            qcopy(m + 0x10, gg + 0x20);
            FUN_L00_001ff660(&dir, &v50, gg + 0x40);
            fast_vec_normalize(&dir, &dir, D_0015ED6C + D_0015ED6C);
            func_L00_0025F4A8_alt(m, &dir, 0, 0.0f, 0.0f, 10, 3, 9, 4.0f, 2.0f, 9.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c(m);
            return;
        } else {
            if (*(int *)(gg + 0x1C) <= 0) goto keep;
            m[0xBC] = 1;
            qcopy(m + 0x10, gg + 0x20);
            FUN_L00_001ff660(&dir, &v50, gg + 0x40);
            fast_vec_normalize(&dir, &dir, D_0015ED6C + D_0015ED6C);
            func_L00_0025F4A8_alt(m, &dir, 0, 0.0f, 1.0f, 10, 3, 9, 4.0f, 2.0f, 9.0f, 1.0f, 0, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c(m);
            return;
        }
    } else {
        if (tick_countdown_32_alt((int *)(d + 0x24)) == 0
            && !(200.0f < vector_distance_xy_alt(m + 0x10, (char *)D_001413D0_2ccee0 + 0x10))) goto keep;
    }
del:
    mark_moby_for_removal_c(m);
    return;
keep:
    if (*(int *)(d + 0x24) % scale_game_frames_c(0x20) != 0) return;
    if (rand() & 1) {
        *(float *)(d + 0x50) = *(float *)(d + 0x50) * random_float_between_c(-0.7f, -1.5f);
    }
    if (rand() & 1) {
        *(float *)(d + 0x54) = *(float *)(d + 0x54) * random_float_between_c(-0.6f, -1.6f);
    }
}
#include "qcopy.h"
typedef unsigned int u128_2cdcc0 __attribute__((mode(TI), aligned(16)));
typedef struct {
    unsigned char pad0[0x80];
    float v80[4];
    unsigned char pad90[8];
    float f98;
    unsigned char pad9c[0x1FF5 - 0x9C];
    unsigned char b1FF5;
    unsigned char b1FF6;
    unsigned char pad1ff7[0x2080 - 0x1FF7];
    unsigned char *p2080;
} P_2cdcc0;
typedef struct {
    unsigned char pad0[0xC0];
    unsigned short hC0;
    unsigned short hC2;
    unsigned int wC4;
} S_2cdcc0;
extern S_2cdcc0 D_00141680_2cdcc0 __asm__("D_00141680");
extern int D_0015EEA4_2cdcc0 __asm__("D_0015EEA4");
int FUN_L00_00233e98_2cdcc0(int) __asm__("FUN_L00_00233e98");
void FUN_L00_00233db8_2cdcc0(int, int) __asm__("FUN_L00_00233db8");
float FUN_001f9dc8_2cdcc0(float) __asm__("FUN_001f9dc8");
float FUN_001f9de0_2cdcc0(float) __asm__("FUN_001f9de0");
int FUN_001f96f8_2cdcc0(int) __asm__("FUN_001f96f8");
void FUN_001f9a10_2cdcc0(void *, void *, void *) __asm__("FUN_001f9a10");
unsigned char *FUN_L00_002ce1f0_2cdcc0(unsigned char *, void *) __asm__("FUN_L00_002ce1f0");
void FUN_L00_002cdcc0(void) {
    P_2cdcc0 *p = &(*(P_2cdcc0 *)((char *)&hero));
    S_2cdcc0 *s;
    float d[4] __attribute__((aligned(16)));
    float v[4] __attribute__((aligned(16)));
    unsigned char *m;
    int i;
    p->b1FF5 = 0;
    if (FUN_L00_00233e98_2cdcc0(0x18)) {
        d[0] = FUN_001f9dc8_2cdcc0(p->f98) * 0.25f;
        d[1] = FUN_001f9de0_2cdcc0(p->f98) * 0.25f;
        d[2] = 0.25f;
        if (p->b1FF6 < 6) {
            FUN_L00_00233db8_2cdcc0(0x18, 1);
            s = &D_00141680_2cdcc0;
            if (s->hC0 < 0xFFFF) {
                s->hC0++;
            }
            if (FUN_001f96f8_2cdcc0(D_0015EEA4_2cdcc0) / 600 > s->hC2) {
                s->hC2 = FUN_001f96f8_2cdcc0(D_0015EEA4_2cdcc0) / 600;
            }
            s->wC4 = s->wC4 | (1 << current_level_index) | 0x80000000;

            FUN_001f9a10_2cdcc0(v, p->v80, p->p2080 + 0xE0);
            FUN_001f9a10_2cdcc0(v, v, p->p2080 + 0xC0);
            for (i = p->b1FF6; i < 6; i++) {
                m = FUN_L00_002ce1f0_2cdcc0(m, v);
                if (m) {
                    qcopy(*(void **)(m + 0x78), d);
                }
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CF328), where it is exact; names translated to the US level program. */

extern int FUN_L00_002725d8(void *, int, float, float, unsigned, unsigned);

void FUN_L00_002cde78(char *m) {
    char *p = *(char **)(m + 0x78);
    if (*(int *)(p + 0x4C) == 0) {
        *(int *)(p + 0x4C) = FUN_L00_002725d8(m, 0, 0.05f, 0.01f, 0x80808080, 0x10808080);
    }
    if (*(int *)(p + 0x50) == 0) {
        *(int *)(p + 0x50) = FUN_L00_002725d8(m, 1, 0.05f, 0.01f, 0x80808080, 0x10808080);
    }
}
/* Exact match: tools/ps2eeas_nops.py keeps the hazard nop after the label the branch lands on. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C96D0.c: func_L00_002CF3D8), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern int D_L00_0015FFE4;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int FUN_002141f8_c(char *) __asm__("FUN_002141f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

char *FUN_L00_002cdf28(char *a, int *excl) {
    char *result = 0;
    char *m;
    float best = 1e9f;
    float v[4];
    for (m = (char *)D_L00_0015FFE4; m != 0; m = *(char **)(m + 0x28)) {
        char *cls = *(char **)(m + 0x24);
        int t = 0;
        float dist;
        char *h;
        char *x;
        if (cls != 0)
            t = *(unsigned char *)(cls + 0x46);
        if (m == 0 || *(unsigned char *)(m + 0x20) == 0xFE)
            continue;
        if (*(unsigned char *)(m + 0x20) == 0xFD)
            continue;
        if (t != 5 && t != 7 && t != 8)
            continue;
        if (excl != 0) {
            int found = 0;
            int i;
            for (i = 0; i < 9; i++) {
                if (excl[i] == (int)m) {
                    found = 1;
                    break;
                }
            }
            if (found)
                continue;
        }
        if (t == 5 && (*(unsigned short *)(m + 0x34) & 0x1000) == 0)
            continue;
        if (*(unsigned char *)(m + 0x31) == 0) {
            short c = *(short *)(m + 0xA6);
            if (c != 0x350 && c != 0x31)
                continue;
        }
        dist = fast_vec_dist(a, m + 0x10);
        if (*(short *)(m + 0xA6) == 0x4D6) {
            normalize_vector_xyz(v, *(char **)(m + 0x78), dist);
            add_vector_xyz(v, v, m + 0x10);
            if (dist < fast_vec_dist(a, v))
                continue;
        } else {
            float lim;
            switch (t) {
            case 5:
                lim = 4.0f;
                break;
            case 7:
                lim = 5.3f;
                break;
            case 8:
                lim = 6.0f;
                break;
            default:
                goto skip;
            }
            if (lim < dist)
                continue;
        }
    skip:
        if (!(dist < best))
            continue;
        h = FUN_L00_002db890(m);
        if (h != 0 && *(short *)(h + 0x68) > 0)
            continue;
        x = (char *)FUN_002141f8_c(m);
        qcopy(v, m + 0x10);
        if (x != 0)
            v[2] = v[2] + (*(float *)(x + 0x10) + 0.05f);
        if (collision_line(a, v, 6, (int)m, 0) == 0) {
            best = dist;
            result = m;
        }
    }
    return result;
}

extern char *spawn_2ce1f0(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_0025d1b8(void *);
extern float atan2_2ce1f0(float, float) __asm__("FUN_001f9e90");
extern float dist_2ce1f0(void *, void *) __asm__("FUN_001f9b80");

/* Spawns moby 0x1DF at pos, aims it at the player and puts it in the first
 * free of six slots. */
char *FUN_L00_002ce1f0(int a, void *pos) {
    char *m = spawn_2ce1f0(0x1DF);
    char *v;
    struct Hero *g;

    if (m != 0) {
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        *(unsigned char *)(m + 0x31) = 1;
        *(unsigned char *)(m + 0x20) = 2;
        v = *(char **)(m + 0x78);
        *(unsigned char *)(m + 0xBC) = 0;
        qcopy(m + 0x10, pos);
        *(int *)(v + 0x44) = a;
        g = &hero;
        g->ammo_used++;
        FUN_L00_00250df8(m);
        FUN_L00_0025d1b8(m);
        *(float *)(v + 0x28) = atan2_2ce1f0(*(float *)(m + 0x10) - g->motion.pos.f[0], *(float *)(m + 0x14) - g->motion.pos.f[1]);
        *(float *)(v + 0x24) = atan2_2ce1f0(dist_2ce1f0(&g->motion.pos, m + 0x10), *(float *)(m + 0x18) - g->motion.pos.f[2]);
        *(float *)(v + 0x20) = dist_2ce1f0(m + 0x10, &g->motion.pos);
        *(float *)(v + 0x3C) = g->motion.pos.f[2];
        for (*(short *)(v + 0x54) = 0; *(short *)(v + 0x54) < 6; (*(short *)(v + 0x54))++) {
            if (hero.unk2020[*(short *)(v + 0x54)] == 0) {
                hero.unk2020[*(short *)(v + 0x54)] = m;
                break;
            }
        }
        *(short *)(v + 0x56) = 100;
        *(int *)(m + 0x94) = 0;
    }
    return m;
}

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002ce358.s", FUN_L00_002ce358);
#else
extern f32 FUN_002000e8_2ce358(f32, f32) __asm__("FUN_001fa580");
extern f32 FUN_00200130_2ce358(f32, f32) __asm__("FUN_001fa5c8");
extern f32 FUN_00200240_2ce358(s32) __asm__("FUN_001fa6c0");
extern s32 FUN_00200250_2ce358(f32) __asm__("FUN_001fa6d0");
extern f32 FUN_001ff488_2ce358(void *, void *) __asm__("FUN_001f9b80");
extern f32 FUN_001ff8b0_2ce358(f32, f32) __asm__("FUN_001f9e90");
extern f32 FUN_001ff7e8_2ce358(f32) __asm__("FUN_001f9dc8");
extern f32 FUN_001ff800_2ce358(f32) __asm__("FUN_001f9de0");
extern void FUN_001ffa70_2ce358(void *, void *) __asm__("FUN_001fa050");
extern void FUN_001ff6f8_2ce358(void *, void *, void *) __asm__("FUN_001f9d20");
extern void FUN_001ff550_2ce358(void *, void *, f32) __asm__("FUN_001f9c48");
extern void FUN_001ff598_2ce358(void *, void *, f32) __asm__("FUN_001f9c90");
extern s32 FUN_001fef98_2ce358(void *) __asm__("FUN_001f9770");
extern s32 FUN_001f0d60_2ce358(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern s32 FUN_00258830_2ce358(f32, f32, f32, void *, void *, s32) __asm__("FUN_L00_00258830");
extern void FUN_00259858_2ce358(f32, void *, void *, s32) __asm__("FUN_L00_00259858");
extern void FUN_00259a88_2ce358(void *, void *) __asm__("FUN_L00_00259a88");
extern void FUN_00259fe8_2ce358(f32, void *) __asm__("FUN_L00_00259fe8");
extern void *FUN_0025a420_2ce358(void *, s32, s32) __asm__("FUN_L00_0025a420");
extern f32 FUN_00258110_2ce358(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");
extern void FUN_0025e450_2ce358(f32, f32, f32, f32, f32, f32, f32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_L00_0025e450");
extern void FUN_0022da68_2ce358(s32, s32, void *) __asm__("FUN_0022da68");
extern s32 FUN_001160d8_2ce358(void) __asm__("FUN_001160d8");
extern void FUN_0024eb68_2ce358(void *) __asm__("FUN_0020c828");
extern float D_0015ED70_lui __asm__("D_0015ED70");
extern V4 D_L00_00166DC0_2ce358 __asm__("D_L00_00166DC0");
extern V4 D_L00_00166FF0_2ce358 __asm__("D_L00_00166FF0");
extern V4 D_L00_00166FD0_2ce358 __asm__("D_L00_00166FD0");
extern V4 D_L00_00173E70_2ce358 __asm__("D_L00_00173E70");

typedef struct {
    V4 position;
    V4 offset;
    f32 strength;
    f32 elevation;
    f32 yaw;
    f32 unused2c;
    struct Moby *target;
    f32 pitch;
    f32 unused38;
    f32 height;
    f32 desired_yaw;
    f32 unused44;
    s32 target_oclass;
    f32 unused4c;
    f32 unused50;
    s16 slot;
    s16 health;
    f32 yaw_velocity;
    f32 pitch_velocity;
} Vars_2ce358;

void drone_update(struct Moby *m) __asm__("FUN_L00_002ce358");
void drone_update(struct Moby *m)
{
    Vars_2ce358 *v = (Vars_2ce358 *)m->pvars;
    V4 collision_data[1], old_pos;
    union {
        struct { V4 direction, rotated; } vectors;
        u8 effect[0x20];
    } scratch;
    V4 target_pos, matrix[4];
    struct Moby *target;
    struct Moby *closest;
    struct Moby *other;
    void *hit;
    f32 f, distance;
    s32 i, count, category, random, health;
    s32 state;
    struct Moby **slots, **exclusions;

    if (D_L00_0015F5C4_2c8440 == 2 || D_L00_0015F5C4_2c8440 == 5 ||
        D_L00_0015F5C4_2c8440 == 6 || hero.state.current == 0x32) {
        m->flags |= 0x41;
        return;
    }
    m->flags &= 0xffbe;
    if (m->state < 5) {
        (*(u8 *)((u8 *)&hero + 0x1ff4))++;
        if (hero.ammo_used < *(u8 *)((u8 *)&hero + 0x1ff4)) {
            f32 distance;
            struct Moby *other;
            closest = 0;
            count = 0;
            distance = 1000000.0f;
            *(u8 *)((u8 *)&hero + 0x1ff4) = 1;
            hero.ammo_capacity = 0;
            *(f32 *)((u8 *)&hero + 0x1ff8) = FUN_002000e8_2ce358(*(f32 *)((u8 *)&hero + 0x1ff8), frame_scale * 0.05f);
            {
                struct Moby **slots, **exclusions;
                s32 i;
                slots = (struct Moby **)hero.unk2020;
                exclusions = (struct Moby **)((u8 *)&hero + 0x1ffc);
                for (i = 0; i < 6; i++, slots++) {
                    other = *slots;
                    if (other != 0) {
                        if (other->oclass == 0x1df) {
                            if (other->state == 0xfe) {
                                *slots = 0;
                            } else if (other->state == 0xfd) {
                                *slots = 0;
                            } else {
                                count++;
                                *exclusions++ = *(struct Moby **)(other->pvars + 0x30);
                                if (other->state < 4)
                                    (hero.ammo_capacity)++;
                                if (count > 8)
                                    break;
                            }
                        } else {
                            *slots = 0;
                        }
                    }
                }
                if (count < 9) {
                    exclusions = (struct Moby **)((u8 *)&hero + 0x1ffc) + count;
                    i = count;
                    do {
                        *exclusions++ = 0;
                        i++;
                    } while (i < 9);
                }
            }
            other = (struct Moby *)FUN_L00_002cdf28((char *)&hero + 0x80, (int *)((u8 *)&hero + 0x1ffc));
            if (other != 0) {
                char *h;
                h = (char *)FUN_002141f8_c((char *)other);
                qcopy(&old_pos, &other->pos);
                if (h != 0)
                    old_pos.f[2] += *(f32 *)(h + 0x10);
                slots = (struct Moby **)hero.unk2020;
                for (i = 5; i >= 0; i--, slots++) {
                    struct Moby *candidate = *slots;
                    if (candidate != 0) {
                        target = *(struct Moby **)(candidate->pvars + 0x30);
                        if (target == 0 || target->state == 0xfe || target->state == 0xfd) {
                            f = FUN_001ff488_2ce358(&candidate->pos, &old_pos);
                            if (f < distance) {
                                closest = candidate;
                                distance = f;
                            }
                        }
                    }
                }
                if (closest != 0) {
                    Vars_2ce358 *assigned = (Vars_2ce358 *)closest->pvars;
                    assigned->target = other;
                    assigned->target_oclass = other->oclass;
                    h = (char *)FUN_002141f8_c((char *)other);
                    qcopy(&old_pos, &other->pos);
                    if (h != 0)
                        old_pos.f[2] += *(f32 *)(h + 0x10);
                    qcopy(&assigned->offset, &old_pos);
                    closest->state = 3;
                    random = FUN_001160d8_2ce358();
                    FUN_0022da68_2ce358((random % 3) + (random % 3 > 0), 0, m);
                    ((struct Moby **)((u8 *)&hero + 0x1ffc))[count] = assigned->target;

                }
            }
        }
        {
            s32 capacity = hero.ammo_used;
            s32 divisor = capacity ? capacity : 1;
            s32 next = capacity + 1;
            s32 counter = *(u8 *)((u8 *)&hero + 0x1ff4);
            if (next == 0)
                next = 1;
            v->desired_yaw = FUN_00200240_2ce358(counter) / (f32)divisor * 6.2831855f - 3.1415927f;
            v->pitch = (FUN_00200240_2ce358((counter * (next / 5)) % next) / (f32)divisor * 6.2831855f - 3.1415927f) * 0.25f;
        }
        hit = FUN_0025a420_2ce358(m, 1, 0);
        if (hit != 0) {
            s32 category;
            char *h = *(char **)((char *)hit + 0x20);
            category = h && *(char **)(h + 0x24) ? *(s16 *)(*(char **)(h + 0x24) + 0x46) : 0;
            if (category > 8) goto dispatch_state;
            if (category >= 5) {
                if (hit == 0 || h == 0 || *(s16 *)(h + 0xa6) != 0x4d6 || FUN_001fef98_2ce358(&v->health))
                    m->state = 4;
            }
        }
    }
dispatch_state:
    state = m->state;
    if (state == 3) goto state3;
    if (state < 4) {
        if (state == 2) goto state2;
    } else {
        if (state == 4) goto state4;
        if (state == 5) goto state5;
    }
    goto update_effects;
state2:
    {
        m->collision = 0;
        qcopy(&old_pos, &m->pos);
        FUN_L00_002cde78((char *)m);
        f = frame_scale * 0.05f;
        m->scale += (m->pclass->scale - m->scale) * f;
        v->strength = v->strength + (1.0f - v->strength) * f;
        f = FUN_002000e8_2ce358(v->desired_yaw, *(f32 *)((u8 *)&hero + 0x1ff8));
        distance = FUN_00200130_2ce358(f, v->yaw);
        v->yaw = FUN_002000e8_2ce358(v->yaw, distance * (frame_scale * 0.05f));
        distance = FUN_00200130_2ce358(v->pitch, v->elevation);
        v->elevation = FUN_002000e8_2ce358(v->elevation, distance * (frame_scale * 0.01f));
        scratch.vectors.direction.f[0] = FUN_001ff7e8_2ce358(v->yaw) * v->strength;
        scratch.vectors.direction.f[1] = FUN_001ff800_2ce358(v->yaw) * v->strength;
        scratch.vectors.rotated.f[1] = v->elevation;
        scratch.vectors.direction.f[2] = 0.0f;
        scratch.vectors.rotated.f[0] = 0.0f;
        scratch.vectors.rotated.f[2] = 0.0f;
        scratch.vectors.rotated.f[3] = 0.0f;
        FUN_001ffa70_2ce358(matrix, &scratch.vectors.rotated);
        FUN_001ff6f8_2ce358(&scratch.vectors.direction, &scratch.vectors.direction, matrix);
        add_vector_xyz_2c8440(&target_pos, (u8 *)&hero + 0x80, (u8 *)hero.moby + 0xe0);
        target_pos.f[0] += scratch.vectors.direction.f[0];
        target_pos.f[1] += scratch.vectors.direction.f[1];
        v->height = v->height + (target_pos.f[2] - v->height) * (frame_scale * 0.05f);
        target_pos.f[2] = v->height + scratch.vectors.direction.f[2];
        subtract_vector_xyz_2c8440(&target_pos, &target_pos, &m->pos);
        FUN_001ff598_2ce358(&target_pos, &target_pos, D_0015ED6C * 10.0f);
        if (FUN_00258830_2ce358(0.0f, 0.18f, 0.0f, m, &target_pos, 0) & 1) {
            if (!m->visible) {
                V4 *hit = &D_L00_00166DC0_2ce358;
                add_vector_xyz_2c8440(&m->pos, hit, hit + 0x23);
                subtract_vector_xyz_2c8440(&m->pos, &m->pos, hit + 0x21);
            }
        }
        qcopy(&collision_data[0], &m->pos);
        if (FUN_001f0d60_2ce358(0.2f, &collision_data[0], 0, m)) {
            category = 0;
            other = (struct Moby *)coll_hit.moby;
            if (other && other->oclass == 0 && other->state != 0xfe && other->state != 0xfd)
                goto update_rotations;
            other = (struct Moby *)((volatile CollisionHit *)&coll_hit)->moby;
            if (other) {
                category = 0;
                if (other->pclass) category = *(u8 *)((u8 *)other->pclass + 0x46);
            }
            other = (struct Moby *)((volatile CollisionHit *)&coll_hit)->moby;
            if (other && other->state != 0xfe && other->state != 0xfd &&
                (category == 5 || category == 7 || category == 8)) {
                FUN_00259858_2ce358(1.0f, scratch.effect, m, 0x10000);
                FUN_001ff550_2ce358(scratch.effect, &v->position, 1.0f);
                *(f32 *)(scratch.effect + 8) = 1.0f;
                *(f32 *)(scratch.effect + 12) = 5627.925f;
                scratch.effect[0x18] = 1;
                scratch.effect[0x19] = 2;
                *(u16 *)(scratch.effect + 0x1a) = m->oclass;
                FUN_00259a88_2ce358((struct Moby *)coll_hit.moby, scratch.effect);
                if (category == 5) {
                    if (coll_hit.moby && ((struct Moby *)coll_hit.moby)->oclass == 0x4d6 && (health = v->health) > 0) {
                        random = FUN_00200250_2ce358(*(f32 *)(scratch.effect + 0x1c));
                        if (random > 0)
                            random = health - FUN_00200250_2ce358(*(f32 *)(scratch.effect + 0x1c));
                        else
                            random = health - 1;
                        v->health = random;
                    } else {
                        FUN_0025e450_2ce358(0, 0, 0.4f, 0.2f, 4.0f, 1.0f, 8.0f, m, v, 0, 0, 1, 1, 1, 1, 1, -1, 0);
                        m->state = 4;
                    }
                }
            }
        }
update_rotations:
        subtract_vector_xyz_2c8440(&v->position, &m->pos, &old_pos);
        distance = FUN_001ff8b0_2ce358(m->pos.x - old_pos.f[0], m->pos.y - old_pos.f[1]);
        f = FUN_001ff488_2ce358(&old_pos, &m->pos);
        f = -FUN_001ff8b0_2ce358(f, m->pos.z - old_pos.f[2]);
        m->rot.z = FUN_00258110_2ce358(&v->yaw_velocity, m->rot.z, distance, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C * 6.2831855f);
        m->rot.y = FUN_00258110_2ce358(&v->pitch_velocity, m->rot.y, f, D_0015ED70_lui * 6.2831855f, D_0015ED70_lui * 3.1415927f, D_0015ED6C * 6.2831855f);
        goto update_effects;
    }
state3:
    {
        struct MobyClass *pclass = m->pclass;
        m->collision = pclass->collision;
        qcopy(&old_pos, &m->pos);
        m->scale += (pclass->scale - m->scale) * (frame_scale * 0.05f);
        m->collision = pclass->collision;
        FUN_L00_002cde78((char *)m);
        target = v->target;
        if (target == 0 || target->oclass != v->target_oclass || target->state == 0xfe || target->state == 0xfd)
            goto clear_target;
        {
            char *h = (char *)FUN_002141f8_c((char *)target);
            qcopy(&target_pos, &v->target->pos);
            if (h != 0)
                target_pos.f[2] += *(f32 *)(h + 0x10);
            subtract_vector_xyz_2c8440(&scratch.vectors.direction, &target_pos, &v->offset);
            scale_vector_xyz_2c8440(&scratch.vectors.direction, &scratch.vectors.direction, 3.1f);
            add_vector_xyz_2c8440(&scratch.vectors.rotated, &target_pos, &scratch.vectors.direction);
            subtract_vector_xyz_2c8440(&v->position, &scratch.vectors.rotated, &m->pos);
            scale_vector_xyz_2c8440(&v->position, &v->position, frame_scale * -0.8f + 1.0f);
            add_vector_xyz_2c8440(&m->pos, &m->pos, &v->position);
            qcopy(&v->offset, &target_pos);
            qcopy(&collision_data[0], &m->pos);
            if (FUN_001f0d60_2ce358(0.25f, &collision_data[0], 0, m)) {
                other = (struct Moby *)coll_hit.moby;
                if (!other || other->state == 0xfe || other->state == 0xfd || other != v->target || other->oclass == 0) {
                    qcopy(&m->pos, &D_L00_00173E70_2ce358);
                } else {
                    s32 category = other->pclass ? *(u8 *)((u8 *)other->pclass + 0x46) : 0;
                    FUN_00259858_2ce358(1.0f, scratch.effect, m, 0x10000);
                    FUN_001ff550_2ce358(scratch.effect, &v->position, 1.0f);
                    *(f32 *)(scratch.effect + 8) = 1.0f;
                    *(f32 *)(scratch.effect + 12) = 5627.925f;
                    scratch.effect[0x18] = 1;
                    scratch.effect[0x19] = 2;
                    *(u16 *)(scratch.effect + 0x1a) = m->oclass;
                    FUN_00259a88_2ce358((struct Moby *)coll_hit.moby, scratch.effect);
                    if (category == 5) {
                        if (coll_hit.moby && ((struct Moby *)coll_hit.moby)->oclass == 0x4d6 && (health = v->health) > 0) {
                            random = FUN_00200250_2ce358(*(f32 *)(scratch.effect + 0x1c));
                            if (random > 0)
                                random = health - FUN_00200250_2ce358(*(f32 *)(scratch.effect + 0x1c));
                            else
                                random = health - 1;
                            v->health = random;
                        } else {
                            FUN_0025e450_2ce358(0, 0, 0.4f, 0.2f, 4.0f, 1.0f, 8.0f, m, v, 0, 0, 1, 1, 1, 1, 1, -1, 0);
                            m->state = 4;
                        }
                    }
                }
            }
        }
        goto state3_rotations;
clear_target:
        m->state = 2;
        v->target_oclass = -1;
        v->target = 0;
state3_rotations:
        m->rot.z = FUN_001ff8b0_2ce358(m->pos.x - old_pos.f[0], m->pos.y - old_pos.f[1]);
        m->rot.y = -FUN_001ff8b0_2ce358(FUN_001ff488_2ce358(&old_pos, &m->pos), m->pos.z - old_pos.f[2]);
        goto update_effects;
    }
state4:
    {
        target = v->target;
        if (target == 0) goto fade_clear;
        if (target->oclass != v->target_oclass) goto fade_clear;
        if (target->state == 0xfe) goto fade_clear;
        if (target->state == 0xfd) goto fade_clear;
        {
            char *h = (char *)FUN_002141f8_c((char *)target);
            qcopy(&old_pos, &v->target->pos);
            if (h != 0)
                old_pos.f[2] += *(f32 *)(h + 0x10);
            subtract_vector_xyz_2c8440(&v->position, &old_pos, &m->pos);
            scale_vector_xyz_2c8440(&v->position, &v->position, frame_scale * -0.8f + 1.0f);
            FUN_001ff598_2ce358(&v->position, &v->position, frame_scale * 0.9f);
            add_vector_xyz_2c8440(&m->pos, &m->pos, &v->position);
        }
        goto fade_alpha;
fade_clear:
        v->target = 0;
fade_alpha:
        m->alpha -= 4;
        if (m->alpha < 6)
            m->state = 5;
        goto update_effects;
    }
state5:
    {
        hero.ammo_used--;
        hero.unk2020[v->slot] = 0;
        FUN_0024eb68_2ce358(m);
        return;
    }
update_effects:
    FUN_00259fe8_2ce358(0.17f, m);
}
#endif
/* Crate update (pvars at moby + 0x78). */
typedef struct {
    u8 pad00[0x1E];
    u16 flags;
} Coll;

typedef struct {
    u8 pad00[0x20];
    Coll coll;
    u64 unk40;
    u8 pad48[0xA0 - 0x48];
    struct Moby *rider;               /* crate stacked on this one */
    struct Moby *below;               /* crate this one sits on */
    s32 unkA8;
    u32 flags;
    u8 padB0[0xB4 - 0xB0];
    s32 frameB4;
    u64 unkB8;                        /* saved moby spawn_frame word */
    u8 padC0[0xC6 - 0xC0];
    s16 unkC6;
    u8 unkC8;
    u8 unkC9;
    u8 unkCA;
    u8 padCB[0xFC - 0xCB];
    s32 frameFC;
} CrateVars;

/* find_moby_hit result: hit moby in the low word of 0x20, flags in the high word. */
typedef struct {
    u8 pad00[0x20];
    union {
        u64 bits;
        struct {
            struct Moby *moby;
            u32 flags;
        } s;
    } hit;
    u8 pad28[0x2C - 0x28];
    f32 unk2C;
} Contact;

/* Fourth argument of collect_mobies_in_sphere. */
typedef struct {
    Vec4f pos;
    struct Moby *moby;
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    f32 unk1C;
    s32 unk20;
} Burst;

/* D_L00_001C40B0 rows (0x18 bytes); unkE is compared with weapon_ammo_counts. */

extern u8 D_0013D4C0[];
extern s32 weapon_ammo_counts[] __asm__("D_0013D428");
extern u8 D_L00_00197BC0[];
extern struct MobyClass *D_L00_00197300[];
extern s32 D_0015ED84 __attribute__((section(".sdata")));
extern int D_L00_0015F5CC_macro __asm__("D_L00_0015F5CC") __attribute__((section(".sdata")));
extern void *D_001413D0 __attribute__((section(".data")));
extern s32 D_001413DC __attribute__((section(".data")));

extern s32 FUN_L00_0025e3b8(void *);
extern void FUN_L00_002d1428(struct Moby *);
extern Contact *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int fun_00213928(void *, void *, void *, int, int *, int *, int *, int) __asm__("FUN_00213928");
extern void FUN_L00_002cfcb8(struct Moby *);
extern void FUN_L00_002d8138(struct Moby *);
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void update_moby_animation_state(struct Moby *) __asm__("FUN_0020c880");
extern void break_crate(struct Moby *, int, Contact *) __asm__("FUN_L00_002d09b8");
extern void crate_drop_bolts(struct Moby *) __asm__("FUN_L00_002d0538");
extern s32 collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern int FUN_L00_001fefc8(void *);
extern int FUN_L00_002591d0(struct Moby **, int, int, int);
extern int FUN_L00_002592b8(struct Moby **, struct Moby *, int, int);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");

void crate_update(struct Moby *moby) __asm__("FUN_L00_002cf218");
void crate_update(struct Moby *moby) {
    Burst burst;
    Vec4f at;
    s32 hitKind;
    s32 hitB;
    s32 hitAngle;
    struct Moby *it;
    CrateVars *pv;
    Contact *other;
    Coll *coll;
    s32 touched;
    s32 doHit;

    pv = (CrateVars *)moby->pvars;
    if (pv->rider != NULL) {
        if (FUN_L00_0025e3b8(pv->rider) == 0) {
            pv->rider = NULL;
        }
    }
    if (pv->below != NULL) {
        if (FUN_L00_0025e3b8(pv->below) == 0) {
            pv->below = NULL;
        }
    }
    pv->frameFC = D_L00_0015F5CC_macro;
    coll = &pv->coll;
    if (pv->rider == NULL) {
        coll->flags |= 1;
    } else {
        coll->flags &= ~1;
    }
    if (pv->frameB4 != D_L00_0015F5CC_macro && moby->state != 0 && (pv->flags & 0xC)) {
        FUN_L00_002d1428(moby);
    }
    touched = 0;
    other = find_moby_hit(moby, 0x1830000, 0);
    if (other != NULL && other->hit.s.flags == 0x1000000) {
        other = NULL;
        touched = 1;
    }
    if (moby->oclass >= 0x1FB && moby->oclass <= 0x1FD) {
        fun_00213928(moby, other, coll, 0, &hitKind, &hitB, &hitAngle, 4);
    }

    switch (moby->state) {
    case 0:
        clear_u64_value(&pv->unk40);
        if (D_0015ED84 == 0x12 && moby->oclass == 0x1FF && pv->unkC8 != 0) {
            moby->state = 6;
            moby->flags |= 0x41;
            moby->collision = 0;
            moby->flags &= ~0x1000;
        } else {
            moby->state = 1;
        }
        pv->unkA8 = 0;
        if ((pv->flags ^ 1) & 1) {
            FUN_L00_002cfcb8(moby);
        }
        pv->flags |= 0x14;
        if (moby->oclass == 0x1FF) {
            pv->unkC6 = 100;
        } else if (moby->oclass == 0x1F5) {
            FUN_L00_002d8138(moby);
        }
        break;

    case 1:
        if (moby->oclass == 0x1F9 &&
            (hero.unk240 == (s32)moby || hero.coll_hit_moby == (u8 *)moby ||
             (hero.ground_moby == moby && hero.air_frames.s == 0) ||
             hero.unk4F8 == (s32)moby || touched)) {
            moby->command = 0;
            moby->state = 5;
            break;
        }
        if (other == NULL || !(other->unk2C > 0.0f)) {
            break;
        }
        doHit = 1;
        if (moby->oclass == 0x1F6 && !(other->hit.s.flags & 0x20000)) {
            doHit = 0;
        }
        if (moby->oclass == 0x1F9) {
            moby->state = 5;
            if (other->hit.s.flags & 0x20000) {
                moby->command = 1;
            } else {
                moby->command = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(5.0f, 15.0f)));
            }
            pv->unkB8 = moby->spawn_frame;
            doHit = 0;
        }
        if (moby->oclass >= 0x1FB && moby->oclass <= 0x1FD) {
            if (other->hit.s.flags & 0x20000) {
                moby->oclass = 0x1FE;
            }
            if (hitKind == 3) {
                moby->oclass = 0x1FE;
            }
            if ((other->hit.bits & 0x0050000000000000LL) || hitAngle == 0xB4) {
                s32 next = moby->oclass + 1;

                if (next >= 0x1FF) {
                    next = 0x1FE;
                }
                moby->oclass = next;
            }
            if (moby->oclass != 0x1FE) {
                moby->oclass++;
                moby->class_slot = D_L00_00197BC0[moby->oclass];
                moby->pclass = D_L00_00197300[moby->class_slot];
                update_moby_animation_state(moby);
                moby->unk72 = ((Part *)moby->pclass)->unkE;
                moby->state = 2;
                moby->command = scale_game_frames(15);
                doHit = 0;
            }
        }
        if (doHit == 0) {
            break;
        }
        break_crate(moby, moby->oclass == 0x1F9, other);
        if (pv->unkC8 != 0 && other != NULL && other->hit.s.moby != NULL) {
            s16 cls = other->hit.s.moby->oclass;

            if (cls == 0x2CD || cls == 0x22C) {
                s16 saved = moby->unkB4;

                moby->unkB4 = 0;
                crate_drop_bolts(moby);
                moby->unkB4 = saved;
                break;
            }
        }
        crate_drop_bolts(moby);
        break;

    case 2:
        if (--moby->command == 0xFF) {
            moby->hit_slot = 0xFF;
            moby->state = 1;
        }
        break;

    case 3:
        if (moby->oclass == 0x1F9 && pv->unkCA <= scale_game_frames(20)) {
            f32 r;

            burst.moby = moby;
            burst.unk1C = 2.0f;
            clear_u64_value(&burst.pos);
            burst.unk20 = 0;
            normalize_vector_xyz(&at, &moby->rot_row2, 0.5f);
            add_vector_xyz(&at, &at, &moby->pos);
            r = convert_integer_to_float(pv->unkCA) / multiply_global_factor_ed64(20.0f);
            burst.unk14 = 1;
            collect_mobies_in_sphere(r + r, &at, 0x10, moby, &burst);
            burst.unk14 = 0x30000;
            collect_mobies_in_sphere(r * 3.0f, &at, 0x10, D_001413D0, &burst);
            pv->unkCA++;
            break;
        }
        if (pv->unkC8 != 0) {
            moby->flags |= 1;
            moby->collision = 0;
            moby->flags &= ~0x1000;
            moby->visible = 0;
            if (moby->unkB4 >= 2) {
                moby->unkB4 = 1;
            }
            moby->state = 6;
            if (D_0015ED84 == 0x12) {
                pv->unkC9 = FUN_L00_00257b90(300, 720);
            } else {
                pv->unkC9 = FUN_L00_00257b90(30, 60);
            }
            break;
        }
        mark_moby_for_removal(moby);
        return;

    case 6: {
        qcopy(&burst.pos, &moby->pos);
        burst.pos.z += 0.5f;
        burst.pos.w = 1.0f;
        if (fast_bsphere_check(&burst.pos, moby->draw_dist) != -1) {
            break;
        }
        if (!FUN_L00_001fefc8(&pv->unkC9)) {
            break;
        }
        if (moby->group == 0xFF) {
            moby->state = 1;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = ((Part *)moby->pclass)->unk10;
            moby->flags |= 0x1000;
            break;
        }
        {
            s32 ok = 1;

            if (D_0015ED84 == 0x12) {
                s32 low = 0;
                s32 total = 0;
                s32 i;

                for (i = 0; i < 0x25; i++) {
                    if (D_0013D4C0[i] != 0) {
                        VendorItemInfo *g = &D_L00_001C40B0[i];

                        total++;
                        if (g->hE != 0 && (f32)weapon_ammo_counts[i] < (f32)g->hE * 0.8f) {
                            low++;
                        }
                    }
                }
                if ((low < 3 && total >= 3) || (total < 3 && low != total)) {
                    ok = 0;
                }
            }
            if (!ok) {
                break;
            }
        }
        FUN_L00_002591d0(&it, moby->group, 0, 0);
        while (it != NULL) {
            if (moby != it && !(vector_distance(&moby->pos, &it->pos) > 0.1f) && it->state == 1) {
                CrateVars *ipv = (CrateVars *)it->pvars;

                if (ipv->rider == NULL) {
                    qcopy(&moby->pos, &it->pos);
                    moby->pos.z += 1.0f;
                    ipv->rider = moby;
                    pv->below = it;
                    pv->rider = NULL;
                    moby->state = 1;
                    moby->visible = 1;
                    moby->flags &= ~1;
                    moby->collision = ((Part *)moby->pclass)->unk10;
                    moby->flags |= 0x1000;
                    pv->flags |= 4;
                    break;
                }
            }
            FUN_L00_002592b8(&it, it, 0, 0);
        }
        if (moby->state != 1) {
            moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
            moby->state = 1;
            pv->rider = NULL;
            pv->below = NULL;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = ((Part *)moby->pclass)->unk10;
            moby->flags |= 0x1000;
        }
        break;
    }

    case 5:
        if (moby->command == 0) {
            moby->command = scale_game_frames(60) * 3 - 1;
            pv->unkB8 = moby->spawn_frame;
        } else if (moby->command > scale_game_frames(15) && other != NULL) {
            moby->command = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(5.0f, 15.0f)));
        }
        if (moby->command % scale_game_frames(60) == scale_game_frames(60) - 3 ||
            moby->command == scale_game_frames(13)) {
            allocate_voice_for_target_entry(1, 0x20, (char *)moby);
        }
        if (moby->command % scale_game_frames(60) > scale_game_frames(40) ||
            moby->command < scale_game_frames(15)) {
            set_moby_color(moby, 0xC8, 0x40, 0x40);
        } else {
            moby->spawn_frame = pv->unkB8;
        }
        if (moby->command == 1 || D_001413DC == 0x16) {
            moby->spawn_frame = pv->unkB8;
            break_crate(moby, 1, NULL);
            crate_drop_bolts(moby);
        }
        moby->command--;
        break;

    case 4:
        if (FUN_L00_001fefc8(&moby->command)) {
            moby->state = 1;
            moby->flags &= ~1;
        }
        break;
    }
    moby->hit_slot = 0xFF;
}
