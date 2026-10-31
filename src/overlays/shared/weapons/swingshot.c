/* Shared moby class swingshot: the swingshot held by the hero, which fires the hook and swings the hero, and its cable. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/overlay/quad.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "rnc/input/pad_state.h"
#include "rnc/input/pad_globals.h"
#include "rnc/overlay/shared/weapons/swingshot_hook.h"

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2c5dc8;

typedef struct {
    float u, v;
} UV;

typedef struct {
    V_2c5dc8     corner[4];
    unsigned int color[4];
    UV           uv[4];
    long         x70, tex, x80, x88;
} Quad;

typedef struct {
    char  pad0[0x14];
    char *x14;
    float x18, x1c, x20;
} Vars_2c5dc8;

extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int  scale_game_frames(int) __asm__("FUN_001f96f8");
long        get_effect_texture(int) __asm__("FUN_001f44b8");
void        sub_2c5dc8(void *, void *, void *) __asm__("FUN_001f9a28");
float       fast_vec_length(void *) __asm__("FUN_001f9af0");
void        fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
float       fast_normalize_angle(float) __asm__("FUN_001fa610");
void        fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
void        draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern void FUN_L00_0024f7c8(void *, int, void *);

extern V_2c5dc8       g_debug_camera_lui __asm__("D_L00_00166DC0") NOT_SDA;
extern f32 frame_time MACRO_ADDR;

void draw_swingshot_cable(struct Moby *moby) __asm__("FUN_L00_002c5dc8");

void draw_swingshot_cable(struct Moby *moby) {
    Quad     segment_quad;
    V_2c5dc8 previous_segment_center, segment_center, attachment_position, segment_direction, f,
        unused_span_perpendicular, wobble_vector_scratch, segment_to_reference, k;
    Vars_2c5dc8 *v;
    unsigned int col;
    float        half = 0.05f;
    float        span_length, distance_along_span, step, x, amplitude_envelope, amp, wob;
    if (moby == 0)
        return;
    v = ((Vars_2c5dc8 *)moby->pvars);
    if (v == 0)
        return;
    segment_quad.tex = get_effect_texture(0xF);
    col = 0x80808080;
    segment_quad.color[3] = col;
    segment_quad.color[2] = col;
    segment_quad.color[1] = col;
    segment_quad.color[0] = col;
    segment_quad.x80 = 0xFF9000000260L;
    segment_quad.x70 = 5;
    segment_quad.x88 = 0xFF00000044L;
    segment_quad.uv[0].u = 0.0f;
    segment_quad.uv[0].v = 0.0f;
    segment_quad.uv[1].u = 1.0f;
    segment_quad.uv[1].v = 0.0f;
    segment_quad.uv[2].u = 0.0f;
    segment_quad.uv[2].v = 1.0f;
    segment_quad.uv[3].u = 1.0f;
    segment_quad.uv[3].v = 1.0f;
    moby_point_position(moby, 0, &attachment_position);
    qcopy(&previous_segment_center, v->x14 + 0x10);
    qcopy(&segment_center, &previous_segment_center);
    sub_2c5dc8(&segment_direction, &attachment_position, &previous_segment_center);
    distance_along_span = 0.0f;
    span_length = fast_vec_length(&segment_direction);
    qcopy(&wobble_vector_scratch, &segment_direction);
    wobble_vector_scratch.f[2] -= 0.1f;
    fast_vec_cross(&f, &segment_direction, &wobble_vector_scratch);
    fast_vec_cross(&unused_span_perpendicular, &segment_direction, &f);
    qcopy(&segment_quad.corner[0], &previous_segment_center);
    qcopy(&segment_quad.corner[2], &previous_segment_center);
    segment_quad.corner[0].f[2] -= half;
    segment_quad.corner[2].f[2] += half;
    wob = fast_cos(fast_normalize_angle(v->x18));
    while (distance_along_span < span_length) {
        step = 0.2f;
        if (span_length - distance_along_span < step)
            step = span_length - distance_along_span;
        distance_along_span += step;
        x = distance_along_span * 3.1415927f;
        amplitude_envelope = fast_sin(x / span_length);
        amp = fast_sin(fast_normalize_angle(x / v->x1c)) * amplitude_envelope * wob * v->x20;
        fast_vec_normalize(&segment_direction, &segment_direction, step);
        fast_vec_add(&segment_center, &segment_center, &segment_direction);
        qcopy(&segment_quad.corner[1], &segment_center);
        qcopy(&segment_quad.corner[3], &segment_center);
        sub_2c5dc8(&segment_to_reference, &g_debug_camera_lui, &segment_center);
        fast_vec_cross(&k, &segment_to_reference, &segment_direction);
        fast_vec_normalize(&k, &k, 0.05f);
        fast_vec_add(&segment_quad.corner[1], &segment_quad.corner[1], &k);
        sub_2c5dc8(&segment_quad.corner[3], &segment_quad.corner[3], &k);
        fast_vec_normalize(&wobble_vector_scratch, &f, amp);
        fast_vec_add(&segment_quad.corner[1], &segment_quad.corner[1], &wobble_vector_scratch);
        fast_vec_add(&segment_quad.corner[3], &segment_quad.corner[3], &wobble_vector_scratch);
        draw_geometry_quad(&segment_quad, 0, 1);
        qcopy(&previous_segment_center, &segment_center);
        qcopy(&segment_quad.corner[0], &segment_quad.corner[1]);
        qcopy(&segment_quad.corner[2], &segment_quad.corner[3]);
    }
}

#ifndef NON_MATCHING

/* Swingshot (class 208) update: keeps the hook at the glove, shows the targets, fires the hook to the swing target
   or the grind target, swings the hero, and reels the hook back. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C12B0.c: func_L00_002C74B8), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef int OvlQuad_6158 __attribute__((mode(TI)));

typedef unsigned int Q_2C74B8 __attribute__((mode(TI)));

typedef union {
    struct {
        float x, y, z, w;
    } f;
    Q_2C74B8 q;
} V_2C74B8 __attribute__((aligned(16)));

typedef struct {
    V_2C74B8 row[3];
} M3;

typedef struct HookData {
    char  pad0[4];
    float speed;
} HookData;

typedef struct Data {
    char  pad0[0x14];
    Moby *hook;
    float spin;
    float scale;
    float size;
} Data;

extern f32     fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float   frame_time;
extern int     FUN_L00_00205000();
extern int     hero_set_state() __asm__("FUN_L00_002223f8");
extern int     FUN_L00_002323b8();
unsigned char *spawn_swingshot_hook(unsigned char *src, float f) __asm__("FUN_L00_002c6f88");
void           suck_draw_reticle(int   a,
                                 int   b,
                                 void *pos,
                                 int   kind,
                                 int   mode,
                                 float x,
                                 float y,
                                 float z) __asm__("FUN_L00_001edf60");
extern Moby   *FUN_L00_002c6f88_2bffa8(void *, float) __asm__("FUN_L00_002c6f88");
extern void FUN_L00_001edf60_2bffa8(int, int, float, void *, int, float, int, float, int) __asm__("FUN_L00_001edf60");
extern int   FUN_001f96f8_2bffa8(int) __asm__("FUN_001f96f8");
extern float FUN_001f9b48_2bffa8(void *, void *) __asm__("FUN_001f9b48");
extern void  FUN_001f9a28_2bffa8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  FUN_001f9bf8_2bffa8(void *, void *, float) __asm__("FUN_001f9bf8");
extern void  FUN_001f9a10_2bffa8(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001f9af0_2bffa8(void *) __asm__("FUN_001f9af0");
extern int   FUN_001efa68_2bffa8(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int   hero_idle_sequence(int) __asm__("FUN_L00_00205000");
extern void  draw_swingshot_cable(struct Moby *) __asm__("FUN_L00_002c5dc8");

void swingshot_update(Moby *m) __asm__("FUN_L00_002c6158");

/* Moby class 208 */
void swingshot_update(Moby *m) {
    V_2C74B8  v0;
    V_2C74B8  v10;
    Data     *d;
    HookData *hd;
    float     speed;
    int       n;

    if (m == 0)
        return;
    if (hero.state.current == 1)
        hero_set_state(30, 1);
    d = (void *)m->pvars;
    if (d == 0)
        return;
    hd = 0;
    if (!d->hook)
        d->hook = FUN_L00_002c6f88_2bffa8(m, frame_time * 24.0f);
    if (d->hook)
        hd = (void *)d->hook->pvars;
    if ((hero.state.current == 1 || hero.state.current == 30) && hero.state_timer >= 2) {
        FUN_L00_001edf60_2bffa8((int)m, 0xFF0000FF, 1.0f, 0, 0x24, 0.0f, -1, 90.0f, 4);
        if (hero.grapple_target_in_sight && hero.grapple_target)
            FUN_L00_001edf60_2bffa8(
                (int)m, 0xFF00FF00, 0.5f, &hero.grapple_target->pos, 0x24, 0.0f, -1, 90.0f, 4);
        else if (hero.swing_target)
            FUN_L00_001edf60_2bffa8(
                (int)m, 0xFF00FF00, 0.5f, &hero.swing_target->pos, 0x24, 0.0f, -1, 90.0f, 4);
    }
    switch (m->state) {
    case 0:
    case 1:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        if (d->hook) {
            moby_point_position(m, 0, &d->hook->pos);
            {
                Moby *h = d->hook;
                *(u128 *)&h->rot_row0 = *(u128 *)&m->rot_row0;
                *(u128 *)&h->rot_row1 = *(u128 *)&m->rot_row1;
                *(u128 *)&h->rot_row2 = *(u128 *)&m->rot_row2;
            }
        }
        if (hero.state.current == 0x24 || (hero.state.control_mode == 0xF && hero.unk5B4)) {
            if (hd) {
                d->size = 0.280000001f;
                d->scale = 1.0f;
                d->spin = 0.0f;
                hd->speed = frame_time * 24.0f;
                if (m->prev_seq != 3)
                    blend_moby_animation(m, 3, 0, FUN_001f96f8_2bffa8(10));
                allocate_voice_for_target_entry(0, 0, m);
                m->state = 2;
            }
        }
        if (hero.state.current == 0x2C && hd) {
            speed = FUN_001f9b48_2bffa8(&hero.swing_target_moby->pos, &d->hook->pos);
            n = FUN_001f96f8_2bffa8(7);
            if (hero.items[0].ready_frames < FUN_001f96f8_2bffa8(30))
                n = FUN_001f96f8_2bffa8(5);
            hd->speed = speed / (float)n;
            d->size = 0.280000001f;
            d->scale = 1.0f;
            d->spin = 0.0f;
            if (m->prev_seq != 3)
                blend_moby_animation(m, 3, 0, FUN_001f96f8_2bffa8(10));
            allocate_voice_for_target_entry(0, 0, m);
            m->state = 9;
        }
        break;
    case 2:
        add_draw_callback(draw_swingshot_cable, m);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f9a28_2bffa8(&v0, &hero.grapple_target->pos, &d->hook->pos);
        speed = hd->speed;
        if (hero.state.control_mode == 0xF) {
            speed = speed + hero.unk574;
        } else if (hero.state.prev_control_mode == 4 || hero.state.prev_control_mode == 2) {
            if (2.0f < hero.ground_distance)
                speed = frame_time * 48.0f;
        }
        qcopy(&v10, &hero.motion.pos);
        v10.f.z += 0.5f;
        if (FUN_001efa68_2bffa8(&v10, &hero.grapple_target->pos, 2, 0, 0)) {
            if (0.5f < hero.ground_distance) {
                hero_set_state(6, 1);
            } else {
                hero_set_state(0, 0);
                set_hero_animation(hero_idle_sequence(0), 0, (float)FUN_001f96f8_2bffa8(14));
            }
            m->state = 6;
        } else if (speed < FUN_001f9b48_2bffa8(&d->hook->pos, &hero.grapple_target->pos)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
        } else {
            qcopy(&d->hook->pos, &hero.grapple_target->pos);
            m->state = 3;
        }
        break;
    case 3:
        allocate_voice_for_target_entry(1, 0, m);
        add_draw_callback(draw_swingshot_cable, m);
        qcopy(&d->hook->pos, &hero.grapple_target->pos);
        m->state = 4;
        break;
    case 4:
        moby_point_position(m, 0, &v0);
        qcopy(&v0, &hero.motion.pos);
        v0.f.z += 0.5f;
        if (FUN_001efa68_2bffa8(&v0, &hero.grapple_target->pos, 2, 0, 0)) {
            if (hero.air_frames.s)
                hero_set_state(6, 1);
            else
                hero_set_state(0, 1);
            m->state = 6;
            break;
        }
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        add_draw_callback(draw_swingshot_cable, m);
        d->spin = d->spin + 0.69813168f;
        d->size = d->size * (frame_scale * -0.100000024f + 1.0f);
        if (d->size < 0.168000013f) {
            hero.unk5B4 = 0;
            if (d->hook && 3.5f < FUN_001f9b48_2bffa8(&d->hook->pos, &hero.motion.pos)) {
                if (!(controller_state.held & hero.items[0].button_mask)) {
                    hero.unk988 = 1;
                    hero_set_state(0, 0);
                    set_hero_animation(hero_idle_sequence(0), 0, (float)FUN_001f96f8_2bffa8(14));
                    m->state = 6;
                } else {
                    allocate_voice_for_target_entry(2, 0, m);
                    if (hero.unk978)
                        hero_set_state(0x26, 1);
                    else
                        hero_set_state(0x25, 1);
                    m->state = 5;
                }
            } else {
                hero_set_state(0, 1);
                m->state = 6;
            }
        }
        break;
    case 5:
        qcopy(&d->hook->pos, &hero.grapple_target->pos);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        d->size = d->size * (frame_scale * -0.100000024f + 1.0f);
        d->spin = d->spin + 0.69813168f;
        add_draw_callback(draw_swingshot_cable, m);
        if ((unsigned int)(hero.state.current - 0x25) >= 2)
            m->state = 6;
        break;
    case 6:
        add_draw_callback(draw_swingshot_cable, m);
        hd->speed = frame_time * 80.0f;
        m->state = 7;
        break;
    case 7:
        moby_point_position(m, 0, &v10);
        FUN_001f9a28_2bffa8(&v0, &v10, &d->hook->pos);
        if (hd->speed < FUN_001f9b48_2bffa8(&d->hook->pos, &v10)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
            add_draw_callback(draw_swingshot_cable, m);
        } else {
            qcopy(&d->hook->pos, &v10);
            if (m->prev_seq != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        break;
    case 8:
        break;
    case 9:
        add_draw_callback(draw_swingshot_cable, m);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        FUN_001f9a28_2bffa8(&v0, &hero.swing_target_moby->pos, &d->hook->pos);
        if (hd->speed < FUN_001f9af0_2bffa8(&v0)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
        } else {
            qcopy(&d->hook->pos, &hero.swing_target_moby->pos);
            m->state = 11;
            allocate_voice_for_target_entry(1, 0, m);
        }
        break;
    case 10:
        add_draw_callback(draw_swingshot_cable, m);
        m->state = 11;
        break;
    case 11:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        add_draw_callback(draw_swingshot_cable, m);
        d->spin = d->spin + 0.69813168f;
        d->size = d->size * (frame_scale * -0.600000024f + 1.0f);
        if (d->size < 0.168000013f) {
            hero.unk99C = 1;
            m->state = 12;
        }
        break;
    case 12:
        if (hero.swing_target_moby)
            qcopy(&d->hook->pos, &hero.swing_target_moby->pos);
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        d->size = d->size * (frame_scale * -0.600000024f + 1.0f);
        d->spin = d->spin + 0.69813168f;
        add_draw_callback(draw_swingshot_cable, m);
        if (hero.state.current != 0x2C)
            m->state = 13;
        break;
    case 13:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        add_draw_callback(draw_swingshot_cable, m);
        hd->speed = frame_time * 80.0f;
        m->state = 14;
        break;
    case 14:
        if (m->anim_flags & 2) {
            if (m->prev_seq != 4)
                blend_moby_animation(m, 4, 0, FUN_001f96f8_2bffa8(10));
        }
        moby_point_position(m, 0, &v10);
        FUN_001f9a28_2bffa8(&v0, &v10, &d->hook->pos);
        if (hd->speed < FUN_001f9b48_2bffa8(&d->hook->pos, &v10)) {
            FUN_001f9bf8_2bffa8(&v0, &v0, hd->speed);
            FUN_001f9a10_2bffa8(&d->hook->pos, &d->hook->pos, &v0);
            add_draw_callback(draw_swingshot_cable, m);
        } else {
            qcopy(&d->hook->pos, &v10);
            if (m->prev_seq != 1)
                blend_moby_animation(m, 1, 0, FUN_001f96f8_2bffa8(10));
            m->state = 1;
        }
        if (hero.state.current == 0x2C && hd) {
            hd->speed = FUN_001f9b48_2bffa8(&hero.swing_target_moby->pos, &d->hook->pos) / 10.0f;
            d->scale = 1.0f;
            d->size = 0.0f;
            d->spin = 0.0f;
            if (m->prev_seq != 3)
                blend_moby_animation(m, 3, 0, FUN_001f96f8_2bffa8(10));
            m->state = 9;
        }
        break;
    case 15:
        break;
    }
}
#else
extern s32   D_001413E4_2c6158 __asm__("D_001413E4");
extern s32   D_0013F4E8_2c6158 __asm__("D_0013F4E8");
extern s32   D_0013FCB8_2c6158 __asm__("D_0013FCB8");
extern u8   *D_0013FCB4_2c6158 __asm__("D_0013FCB4");
extern u8   *D_0013FCE0_2c6158 __asm__("D_0013FCE0");
extern u8   *D_0013FCE4_2c6158 __asm__("D_0013FCE4");
extern s32   D_0013F904_2c6158 __asm__("D_0013F904");
extern s32   D_00140400_2c6158 __asm__("D_00140400");
extern s32   D_0013FCC8_2c6158 __asm__("D_0013FCC8");
extern s32   D_0013FCD8_2c6158 __asm__("D_0013FCD8");
extern s32   D_0013FCEC_2c6158 __asm__("D_0013FCEC");
extern float D_0013F8C4_2c6158 __asm__("D_0013F8C4");
extern float D_0013F62C_2c6158 __asm__("D_0013F62C");
extern void  hero_set_state(s32, s32) __asm__("FUN_L00_002223f8");
extern void  FUN_001edf60_2c6158(void *, float, u32, void *, s32, float, s32, float, s32) __asm__("FUN_L00_001edf60");
extern s32   FUN_001fef20_2c6158(s32) __asm__("FUN_001f96f8");
extern void  FUN_001f93e0_2c6158(void *, void *) __asm__("FUN_L00_001f93e0");
extern float FUN_001ff450_2c6158(void *, void *) __asm__("FUN_001f9b48");
extern void  FUN_001ff2a8_2c6158(void *, void *, void *) __asm__("FUN_001f9a28");
extern s32   FUN_001efc70_2c6158(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void  FUN_001ff500_2c6158(float, void *, void *) __asm__("FUN_001f9bf8");
extern void  FUN_001ff278_2c6158(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001ff3d8_2c6158(void *) __asm__("FUN_001f9af0");
extern void *hero_idle_sequence(s32) __asm__("FUN_L00_00205000");
extern void  set_hero_animation(float, void *, s32) __asm__("FUN_L00_002323b8");

void swingshot_update(u8 *m) __asm__("FUN_L00_002c6158");

/* Moby class 208 */
void swingshot_update(u8 *m) {
    u8     *vars;
    u8     *child;
    u8     *motion;
    OvlVec4 a, b;
    float   speed, distance;
    s32     frames;
    void   *effect;

    if (m == 0)
        return;
    if (hero.state.current == 1)
        hero_set_state(30, 1);
    vars = *(u8 **)(m + 0x78);
    if (vars == 0)
        return;
    motion = 0;
    if (*(u8 **)(vars + 0x14) == 0) {
        if ((*(u8 **)(vars + 0x14) = spawn_swingshot_hook(m, frame_time * 24.0f)) == 0)
            goto child_ready;
    }
    motion = *(u8 **)(*(u8 **)(vars + 0x14) + 0x78);
child_ready:;

    if (hero.state.current == 1 || hero.state.current == 30) {
        if (hero.state_timer >= 2) {
            FUN_001edf60_2c6158(m, 1.0f, 0xff0000ff, 0, 36, 0.0f, -1, 90.0f, 4);
            if (hero.grapple_target_in_sight && ((u8 *)hero.grapple_target)) {
                FUN_001edf60_2c6158(m,
                                    0.5f,
                                    0xff00ff00,
                                    ((u8 *)hero.grapple_target) + 0x10,
                                    36,
                                    0.0f,
                                    -1,
                                    90.0f,
                                    4);
            } else if (((u8 *)hero.swing_target)) {
                FUN_001edf60_2c6158(
                    m, 0.5f, 0xff00ff00, ((u8 *)hero.swing_target) + 0x10, 36, 0.0f, -1, 90.0f, 4);
            }
        }
    }
    switch (m[0x20]) {
    case 0:
    case 1:
        if (m[0x70] & 2) {
            if (m[0x53] != 1)
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            m[0x20] = 1;
        }
        if (*(u8 **)(vars + 0x14) != 0) {
            FUN_L00_0024f7c8(m, 0, *(u8 **)(vars + 0x14) + 0x10);
            child = *(u8 **)(vars + 0x14);
            *(u128 *)(child + 0xc0) = *(u128 *)(m + 0xc0);
            *(u128 *)(child + 0xd0) = *(u128 *)(m + 0xd0);
            *(u128 *)(child + 0xe0) = *(u128 *)(m + 0xe0);
        }
        if ((hero.state.current == 36 || (hero.state.control_mode == 15 && hero.unk5B4)) &&
            motion) {
            *(float *)(vars + 0x18) = 0.0f;
            *(float *)(vars + 0x1c) = 1.0f;
            *(float *)(vars + 0x20) = 0.28f;
            *(float *)(motion + 4) = frame_time * 24.0f;
            if (m[0x53] != 3)
                blend_moby_animation(m, 3, 0, scale_game_frames(10));
            allocate_voice_for_target_entry(0, 0, m);
            m[0x20] = 2;
        }
        if (hero.state.current == 44) {
            if (motion == 0)
                return;
            distance = FUN_001ff450_2c6158(((u8 *)hero.swing_target_moby) + 0x10,
                                           (*(u8 **)(vars + 0x14)) + 0x10);
            frames = scale_game_frames(7);
            if (hero.items[0].ready_frames < scale_game_frames(30))
                frames = scale_game_frames(5);
            *(float *)(motion + 4) = distance / (float)frames;
            *(float *)(vars + 0x18) = 0.0f;
            *(float *)(vars + 0x1c) = 1.0f;
            *(float *)(vars + 0x20) = 0.28f;
            if (m[0x53] != 3)
                blend_moby_animation(m, 3, 0, scale_game_frames(10));
            allocate_voice_for_target_entry(0, 0, m);
            m[0x20] = 9;
        }
        return;
    case 2:
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001ff2a8_2c6158(&a, ((u8 *)hero.grapple_target) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
        speed = *(float *)(motion + 4);
        if (hero.state.control_mode == 15)
            speed += hero.unk574;
        else if ((hero.state.prev_control_mode == 4 || hero.state.prev_control_mode == 2) &&
                 hero.ground_distance > 2.0f)
            speed = frame_time * 48.0f;
        qcopy(&b, &hero.motion.pos);
        b.f[2] += 0.5f;
        if (FUN_001efc70_2c6158(&b, ((u8 *)hero.grapple_target) + 0x10, 2, 0, 0)) {
            if (hero.ground_distance > 0.5f)
                hero_set_state(6, 1);
            else {
                hero_set_state(0, 0);
                effect = hero_idle_sequence(0);
                frames = scale_game_frames(14);
                set_hero_animation((float)frames, effect, 0);
            }
            m[0x20] = 6;
            return;
        }
        distance =
            FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        if (speed < distance) {
            FUN_001ff500_2c6158(speed, &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        m[0x20] = 3;
        return;
    case 3:
        allocate_voice_for_target_entry(1, 0, m);
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        m[0x20] = 4;
        return;
    case 4: {
        float factor;
        /* Retail tests ordered comparisons; an unordered fade value returns here. */
        FUN_L00_0024f7c8(m, 0, &a);
        qcopy(&a, &hero.motion.pos);
        a.f[2] += 0.5f;
        if (FUN_001efc70_2c6158(&a, ((u8 *)hero.grapple_target) + 0x10, 2, 0, 0) != 0) {
            if (hero.air_frames.s != 0)
                hero_set_state(6, 1);
            else
                hero_set_state(0, 1);
            m[0x20] = 6;
            return;
        }
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        factor = *(float *)(vars + 0x20) * (frame_scale * -0.100000024f + 1.0f);
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) = factor;
        if (!(factor < 0.16800001f))
            return;
        hero.unk5B4 = 0;
        if ((*(u8 **)(vars + 0x14)) != 0 &&
            3.5f < FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &hero.motion.pos)) {
            if ((pad_held[0] & hero.items[0].button_mask) == 0) {
                hero.unk988 = 1;
                hero_set_state(0, 0);
                effect = hero_idle_sequence(0);
                frames = scale_game_frames(14);
                set_hero_animation((float)frames, effect, 0);
                m[0x20] = 6;
            } else {
                allocate_voice_for_target_entry(2, 0, m);
                if (hero.unk978 != 0)
                    hero_set_state(38, 1);
                else
                    hero_set_state(37, 1);
                m[0x20] = 5;
            }
        } else {
            hero_set_state(0, 1);
            m[0x20] = 6;
            return;
        }
        return;
    }
    case 5: {
        float fade_scale;
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.grapple_target) + 0x10);
        fade_scale = frame_scale;
        if ((m[0x70] & 2) && m[0x53] != 4) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            fade_scale = frame_scale;
        }
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) *= fade_scale * -0.100000024f + 1.0f;
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if ((u32)(hero.state.current - 37) < 2)
            return;
        m[0x20] = 6;
        return;
    }
    case 6:
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        *(float *)(motion + 4) = frame_time * 80.0f;
        m[0x20] = 7;
        return;
    case 7:
        FUN_L00_0024f7c8(m, 0, &b);
        FUN_001ff2a8_2c6158(&a, &b, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (*(float *)(motion + 4) < distance) {
            FUN_001ff500_2c6158(*(float *)(motion + 4), &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            FUN_001f93e0_2c6158(draw_swingshot_cable, m);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (m[0x53] != 1)
            blend_moby_animation(m, 1, 0, scale_game_frames(10));
        m[0x20] = 1;
        return;
    case 9: {
        float step;
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001ff2a8_2c6158(
            &a, ((u8 *)hero.swing_target_moby) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff3d8_2c6158(&a);
        step = *(float *)(motion + 4);
        if (step < distance) {
            FUN_001ff500_2c6158(step, &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            return;
        }
        qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.swing_target_moby) + 0x10);
        m[0x20] = 11;
        allocate_voice_for_target_entry(1, 0, m);
        return;
    }
    case 10:
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        m[0x20] = 11;
        return;
    case 11: {
        float factor;
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        factor = *(float *)(vars + 0x20) * (frame_scale * -0.6f + 1.0f);
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) = factor;
        if (!(factor < 0.16800001f))
            return;
        hero.unk99C = 1;
        m[0x20] = 12;
        return;
    }
    case 12: {
        float fade_scale;
        if (((u8 *)hero.swing_target_moby))
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, ((u8 *)hero.swing_target_moby) + 0x10);
        fade_scale = frame_scale;
        if ((m[0x70] & 2) && m[0x53] != 4) {
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
            fade_scale = frame_scale;
        }
        *(float *)(vars + 0x18) += 0.6981317f;
        *(float *)(vars + 0x20) *= fade_scale * -0.6f + 1.0f;
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        if (hero.state.current != 44)
            m[0x20] = 13;
        return;
    }
    case 13:
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        *(float *)(motion + 4) = frame_time * 80.0f;
        m[0x20] = 14;
        return;
    case 14:
        if ((m[0x70] & 2) && m[0x53] != 4)
            blend_moby_animation(m, 4, 0, scale_game_frames(10));
        FUN_L00_0024f7c8(m, 0, &b);
        FUN_001ff2a8_2c6158(&a, &b, (*(u8 **)(vars + 0x14)) + 0x10);
        distance = FUN_001ff450_2c6158((*(u8 **)(vars + 0x14)) + 0x10, &b);
        if (*(float *)(motion + 4) < distance) {
            FUN_001ff500_2c6158(*(float *)(motion + 4), &a, &a);
            FUN_001ff278_2c6158((*(u8 **)(vars + 0x14)) + 0x10, (*(u8 **)(vars + 0x14)) + 0x10, &a);
            FUN_001f93e0_2c6158(draw_swingshot_cable, m);
        } else {
            qcopy((*(u8 **)(vars + 0x14)) + 0x10, &b);
            if (m[0x53] != 1)
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            m[0x20] = 1;
        }
        if (hero.state.current != 44 || motion == 0)
            return;
        distance = FUN_001ff450_2c6158(((u8 *)hero.swing_target_moby) + 0x10,
                                       (*(u8 **)(vars + 0x14)) + 0x10);
        *(float *)(motion + 4) = distance / 10.0f;
        *(float *)(vars + 0x1c) = 1.0f;
        *(float *)(vars + 0x18) = 0.0f;
        *(float *)(vars + 0x20) = 0.0f;
        if (m[0x53] != 3)
            blend_moby_animation(m, 3, 0, scale_game_frames(10));
        m[0x20] = 9;
        return;
    case 15:
    default:
        return;
    }
}
#endif
