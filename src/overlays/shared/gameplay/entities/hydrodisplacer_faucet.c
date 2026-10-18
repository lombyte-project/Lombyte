/* Shared moby classes: hydrodisplacer_faucet, hoverboard, lamp, floor_button, blarg_gun. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "sda.h"
#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/options.h"
extern struct Moby *create_moby(int) __asm__("FUN_0020c4f8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float D_0015EE6C_d4168 __asm__("D_0015ED6C");
char *FUN_L05_002d4168(int owner, char *pos, int arg, float f0, float f1) {
    char *moby = (char *)create_moby(0x5C3);
    if (moby != 0) {
        char *d = *(char **)(moby + 0x78);
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        *(float *)(moby + 0x48) = f0;
        *(float *)(moby + 0x44) = f1;
        moby[0x20] = 0;
        *(int *)d = owner;
        qcopy(moby + 0x10, pos);
        *(float *)(d + 0x10) = D_0015EE6C_d4168 * 30.0f * 0.7f;
        *(int *)(d + 8) = arg;
        *(int *)(d + 0x14) = 0;
        *(int *)(d + 0x18) = 0;
        *(short *)(d + 6) = 0;
        *(float *)(moby + 0x2C) *= 2.3f;
        *(short *)(d + 4) = scale_game_frames(0x12C);
    }
    return moby;
}
extern s32 moby_pool_4f60 __asm__("D_L05_0015FFD8");

void FUN_L05_002f4f60(int index, int trigger_arg, int inside) {
    int moby = moby_pool_4f60 + index * 0x100;
    char *trigger = (char *)trigger_arg;
    short kind;
    unsigned int flags;

    if (moby == 0) {
        return;
    }
    if (inside) {
        if (trigger[0x7c] > 0) {
            *(unsigned short *)(moby + 0x34) &= 0xfffd;
        } else if (trigger[0x7c] < 0) {
            *(unsigned short *)(moby + 0x34) |= 2;
        }
        if (trigger[0x7d] > 0 && (kind = *(short *)(moby + 0xa6)) != 0x386 && kind != 0x3ae && kind != 0x4ef && kind != 0x571) {
            *(char *)(moby + 0x31) = 1;
            *(unsigned short *)(moby + 0x34) &= 0xfffe;
        } else if (trigger[0x7d] < 0 && (kind = *(short *)(moby + 0xa6)) != 0x386 && kind != 0x3ae && kind != 0x4ef && kind != 0x571) {
            *(char *)(moby + 0x31) = 0;
            flags = *(unsigned short *)(moby + 0x34) | 1;
            *(unsigned short *)(moby + 0x34) = flags;
        }
        if (trigger[0x7f] > 0 && *(int *)(moby + 0x24) != 0) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        } else if (trigger[0x7f] < 0 && *(int *)(moby + 0x24) != 0) {
            *(int *)(moby + 0x94) = 0;
        }
    } else {
        if (trigger[0x7c] > 0 && (kind = *(short *)(moby + 0xa6)) != 0x386 && kind != 0x3ae && kind != 0x4ef) {
            *(unsigned short *)(moby + 0x34) |= 2;
        } else if (trigger[0x7c] < 0 && (kind = *(short *)(moby + 0xa6)) != 0x386 && kind != 0x3ae && kind != 0x4ef) {
            *(unsigned short *)(moby + 0x34) &= 0xfffd;
        }
        if (trigger[0x7d] > 0 && (kind = *(short *)(moby + 0xa6)) != 0x386 && kind != 0x3ae && kind != 0x4ef && kind != 0x571) {
            *(char *)(moby + 0x31) = 0;
            flags = *(unsigned short *)(moby + 0x34) | 1;
            *(unsigned short *)(moby + 0x34) = flags;
        } else if (trigger[0x7d] < 0 && (kind = *(short *)(moby + 0xa6)) != 0x386 && kind != 0x3ae && kind != 0x4ef && kind != 0x571) {
            *(char *)(moby + 0x31) = 1;
            *(unsigned short *)(moby + 0x34) &= 0xfffe;
        }
        if (trigger[0x7f] > 0) {
            *(int *)(moby + 0x94) = 0;
        } else if (trigger[0x7f] < 0) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Runs func_L05_002F62E8 for each entry of list i until an entry without the top bit set. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F6518), where it is exact; names translated to the US level program. */

extern int *D_L05_001ABCC0[];
extern void FUN_L05_002f4f60(int, int, int);

void FUN_L05_002f5190(int a, int b, int i, int c) {
    unsigned short *p = (unsigned short *)D_L05_001ABCC0[i];
    if (p != 0) {
        do {
            FUN_L05_002f4f60(*p & 0x7FFF, b, c);
        } while (*(short *)p++ >= 0);
    }
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

/* pvars of the trigger moby run by FUN_L05_002f5200 */
typedef struct {
    s32 path;          /* -1, or an index into D_L05_001B0930 */
    f32 radius;        /* > 0: trigger when the target is this close */
    s32 volumes[6];    /* -1, or a clip volume is_point_inside_clip_volume tests */
    s32 on_lists[12];  /* -1, or a list run by FUN_L05_002f5190 */
    s32 on_mobys[11];  /* -1, or a moby passed to FUN_L05_002f4f60 */
    u8 pad7C[2];
    s8 use_alt_target; /* nonzero: test D_L05_001671C0 instead of the hero */
} TriggerVars_2f5200;

extern Vec4 D_L05_001671C0_2f5200 __asm__("D_L05_001671C0");
extern s32 *D_L05_001B0930_2f5200[] __asm__("D_L05_001B0930");
extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern s32 absolute_float_cf(void *, void *, s32) __asm__("FUN_L00_00259740");
extern s32 is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");
extern void FUN_L05_002f5190_2f5200(struct Moby *, TriggerVars_2f5200 *, s32, s32) __asm__("FUN_L05_002f5190");
extern void FUN_L05_002f4f60_2f5200(s32, TriggerVars_2f5200 *, s32) __asm__("FUN_L05_002f4f60");

void FUN_L05_002f5200(struct Moby *m) {
    TriggerVars_2f5200 *v = (TriggerVars_2f5200 *)m->pvars;
    Vec4 target;
    s32 inside;
    s32 i;
    s32 *path;

    m->update_dist = 0xFF;
    inside = 0;
    if (v->use_alt_target != 0) {
        qcopy(&target, &D_L05_001671C0_2f5200);
    } else {
        qcopy(&target, &hero.motion.pos);
    }
    if (0.0f < v->radius && distance_xyz(&m->pos, &target) < v->radius) {
        inside = 1;
    }
    if (!inside && v->path != -1) {
        path = D_L05_001B0930_2f5200[v->path];
        inside = absolute_float_cf(&target, path + 4, path[0]) != 0;
    }
    if (!inside) {
        for (i = 0; i < 6; i++) {
            if (v->volumes[i] != -1 && is_point_inside_clip_volume(&target, v->volumes[i])) {
                inside = 1;
                break;
            }
        }
    }
    if ((inside && m->command == 0) || (!inside && m->command != 0) || m->state == 0) {
        if (inside) {
            m->command = 1;
        } else {
            m->command = 0;
        }
        m->state = m->command + 1;
        for (i = 0; i < 12; i++) {
            if (v->on_lists[i] != -1) {
                FUN_L05_002f5190_2f5200(m, v, v->on_lists[i], inside);
            }
        }
        for (i = 0; i < 11; i++) {
            if (v->on_mobys[i] != -1) {
                FUN_L05_002f4f60_2f5200(v->on_mobys[i], v, inside);
            }
        }
    }
}
/* Moby update: plays an animation when its state and a flag allow, then calls the next stage. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F9478), where it is exact; names translated to the US level program. */

extern short D_L05_0015FFD8;
extern void FUN_L05_002f81b8_u(void *) __asm__("FUN_L05_002f81b8");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void hydrodisplacer_faucet_update(char *m) __asm__("FUN_L05_002f8080");
void hydrodisplacer_faucet_update(char *m) {
    struct Hero *g = &hero;
    int f = 0;
    int *d = *(int **)(m + 0x78);
    if (((char *)g->ground_moby) == m) {
        f = g->state.control_mode == 0x13;
    }
    switch ((unsigned char)m[0x53]) {
    case 0:
        if (d[0] >= 0 && (*(unsigned char *)(*(int *)&D_L05_0015FFD8 + d[0] * 256 + 0xBC) & 1) &&
            !f)
            blend_moby_animation(m, 1, 0, 0);
        break;
    case 2:
        if (d[0] >= 0 && (*(unsigned char *)(*(int *)&D_L05_0015FFD8 + d[0] * 256 + 0xBC) & 2) &&
            !f)
            blend_moby_animation(m, 3, 0, 0);
        break;
    case 3:
        if (m[0x70] & 2)
            blend_moby_animation(m, 0, 0, 0);
        break;
    case 1:
        if (m[0x70] & 2)
            blend_moby_animation(m, 2, 0, 0);
        break;
    }
    FUN_L05_002f81b8_u(m);
}
/* eases the pulse level of the moby that holds the focus and mixes its colour from two palettes */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_002F95B0), where it is exact; names translated to the US level program. */

extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern short D_L05_001618C0 __attribute__((sda));
extern short D_L05_001618C4 __attribute__((sda));
extern short D_L05_001618C8 __attribute__((sda));
extern short D_L05_001618CC __attribute__((sda));
extern short D_L05_001618D0 __attribute__((sda));
extern short D_0015EE6C_s __asm__("D_0015ED6C");

void FUN_L05_002f81b8(void *mv) {
    char *moby = mv;
    struct Hero *base = &hero;
    char *data = *(char **)(moby + 0x78);
    int r;
    float s;
    if (((char *)base->ground_moby) == moby) {
        approach_value((float *)(data + 0x1C), 1.0f, frame_time + frame_time);
        if (((int)base->items[0].moby) != 0) {
            if (base->items[0].item_id == 0x16)
                ((unsigned char *)moby)[0xBC] = 1;
            else
                ((unsigned char *)moby)[0xBC] = 0;
        } else {
            ((unsigned char *)moby)[0xBC] = 0;
        }
    } else {
        approach_value((float *)(data + 0x1C), 0.0f, *(float *)&D_0015EE6C_s);
    }
    if (*(float *)(data + 0x1C) != 0.0f) {
        s = fast_add_rotations(*(float *)(data + 0x18), frame_time * 6.2831855f);
        *(float *)(data + 0x18) = s;
        if (((unsigned char *)moby)[0xBC] == 1) {
            r = fast_tween_color(*(int *)&D_L05_001618C4, *(int *)&D_L05_001618C8, (fast_sin(s) + 1.0f) * 0.5f);
        } else {
            r = fast_tween_color(*(int *)&D_L05_001618CC, *(int *)&D_L05_001618D0, (fast_sin(s) + 1.0f) * 0.5f);
        }
        *(int *)(moby + 0x90) = fast_tween_color(*(int *)&D_L05_001618C0, r, *(float *)(data + 0x1C));
    } else {
        *(int *)(moby + 0x90) = *(int *)&D_L05_001618C0;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_002f87a8.s", FUN_L05_002f87a8);
#else
#include "rnc/gameplay/state/item_state.h"
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern float vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void play_sound(int, int) __asm__("FUN_L00_00203908");
extern void get_moby_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void spawn_second_effect(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern void FUN_L05_0024cee8(int, int);
extern int frame_number_gp __asm__("D_L05_0015F5CC") __attribute__((sda));
extern unsigned short D_00141938 __attribute__((section(".data")));
extern unsigned short D_00141C60 __attribute__((section(".data")));
extern int D_L05_0015F6A8 __attribute__((section(".data")));
extern int D_L05_00161928 __attribute__((sda));
extern int D_L05_0016190C __attribute__((sda));
extern int D_L05_00161910 __attribute__((sda));

void hoverboard_update(unsigned char *moby) __asm__("FUN_L05_002f87a8");
void hoverboard_update(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    float delta[4], position[4], effect_position[4], offset[4], second_position[4];
    float angle, distance_scale, delta_length;
    int state = (u8)moby[0x20];
    struct Hero *g;

    switch (state) {
    case 0: {
        char *pos = D_0013F3D0;
        struct Hero *player;
        if (distance_xyz(pos, moby + 0x10) < 5.5f && frame_number_gp > 3 &&
            (player = (struct Hero *)(pos - 0x80))->state.control_mode != 0x15 &&
            player->state.control_mode != 0x16) {
            player->unk86C = moby;
            FUN_L05_0024cee8(0x6b, 1);
            if (*(int *)(data + 0x50) == 0) {
                qcopy(data + 0x10, moby + 0x10);
            }
            moby[0x20] = 1;
            *(unsigned char *)(moby + 0x30) = 0xff;
        }
        break;
    }
    case 1:
        if (*(int *)(data + 0x50) == 0 && alternate_item_available[0] != 0 &&
            D_00141938 == 0 && D_L05_0015F6A8 != 0 &&
            (g = &hero, scale_game_frames(0x4b0) < g->unk19C) &&
            D_00141C60 == 0) {
            play_sound(0x138f, 0x5f);
        }
        if (hero.state.control_mode != 0x16) {
            moby[0x30] = 0x10;
            if (*(int *)(data + 0x50) == 0) {
                qcopy(moby + 0x10, data + 0x10);
            }
            moby[0x20] = 0;
        }
        break;
    case 2:
        break;
    }
    {
        unsigned char *position_ptr = moby + 0x10;
        subtract_vector_xyz(delta, position_ptr, data);
        qcopy(data, position_ptr);
    }
    delta_length = vector_length_xyz(delta);
    if ((unsigned char)moby[0xbc] != 2 && delta_length < 2.0f) {
        get_moby_position(moby, frame_number_gp & 1, position);
        angle = random_angle_radians();
        distance_scale = frame_time * 0.25f;
        normalize_vector_xyz(effect_position, moby + 0xe0, distance_scale * fast_cos(angle));
        normalize_vector_xyz(offset, moby + 0xd0, distance_scale * fast_sin(angle));
        add_vector_xyz(effect_position, effect_position, delta);
        add_vector_xyz(effect_position, effect_position, offset);
        qcopy(effect_position, delta);
        effect_position[3] = (float)D_L05_00161928;
        if ((unsigned char)moby[0xbc] == 0) {
            int size = random_int_between(scale_game_frames(10), scale_game_frames(13));
            spawn_effect(position, effect_position, D_L05_0016190C, D_L05_00161910, size, 15, 15, 1);
            moby[0xbc] = 0;
            return;
        }
        scale_vector_xyz(second_position, delta, 0.920f);
        {
            int size = random_int_between(scale_game_frames(15), scale_game_frames(18));
            spawn_effect(position, second_position, 0x5032f0d2, D_L05_00161910, size, 30, 20, 1);
        }
        {
            int size = random_int_between(scale_game_frames(8), scale_game_frames(17));
            spawn_second_effect(position, effect_position, D_L05_0016190C, D_L05_00161910, size, 1, 15000.0f);
        }
    }
    moby[0xbc] = 0;
}
#endif

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0FC8), where it is exact; names translated to the US level program. */

extern float absolute_float(float) __asm__("func_001F99C0");
extern float vector_length_xyz(void *);
extern int FUN_L00_002591d0(int *, int, int, int);
extern int FUN_L00_002592b8(int *, int, int, int);
extern int tick_countdown_32(int *);
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void set_identity_matrix(void *) __asm__("FUN_001f9fc8");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void subtract_vector_xyz(void *, void *, void *);

void FUN_L05_00307010(char *moby) {
    char *data = *(char **)(moby + 0x78);
    struct Hero *g = &hero;
    float v[4];
    char *l1;
    char *l2;
    char *other;
    char *od;

    if (g->unk894 < scale_game_frames(0xB4)) {
        return;
    }
    if (!tick_countdown_32((int *)(data + 0x264))) {
        return;
    }
    other = ((char *)g->unk86C);
    if (!other) {
        return;
    }
    od = *(char **)(other + 0x78);
    if (*(int *)(od + 0x44) != -1) {
        FUN_L00_002591d0((int *)&l1, *(int *)(od + 0x44), 0, 0);
        while (l1) {
            if (*(short *)(l1 + 0xA6) == 0x473) {
                float a[4];
                float m[16];
                char *pos = l1;
                qcopy(a, data + 0x200);
                a[2] += 0.8f;
                subtract_vector_xyz(v, pos + 0x10, a);
                if (!(8.0f < vector_length_xyz(v))) {
                    set_identity_matrix(m);
                    transpose_rotation_matrix(m, l1 + 0xC0);
                    transform_vector(v, v, m);
                    if (absolute_float(v[0]) < 0.5f && absolute_float(v[1]) < 2.5f &&
                        absolute_float(v[2]) < 2.5f) {
                        *(unsigned short *)(data + 0x25A) += scale_game_frames(0x78);
                        *(int *)(data + 0x264) = scale_game_frames(0x3C);
                    }
                }
            }
            FUN_L00_002592b8((int *)&l1, (int)l1, 0, 0);
        }
    }
    if (*(int *)(od + 0x48) != -1) {
        FUN_L00_002591d0((int *)&l2, *(int *)(od + 0x48), 0, 0);
        while (l2) {
            if (*(short *)(l2 + 0xA6) == 0x474) {
                float m[16];
                subtract_vector_xyz(v, l2 + 0x10, data + 0x200);
                if (!(8.0f < vector_length_xyz(v))) {
                    set_identity_matrix(m);
                    transpose_rotation_matrix(m, l2 + 0xC0);
                    transform_vector(v, v, m);
                    if (absolute_float(v[0]) < 2.8f && absolute_float(v[1]) < 1.4f &&
                        absolute_float(v[2]) < 0.5f) {
                        *(unsigned short *)(data + 0x25A) += scale_game_frames(0x78);
                        *(int *)(data + 0x264) = scale_game_frames(0x3C);
                    }
                }
            }
            FUN_L00_002592b8((int *)&l2, (int)l2, 0, 0);
        }
    }
}
/* UpdateMoby_823: pick random wobble parameters, then animate them each frame */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0030D500), where it is exact; names translated to the US level program. */

extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void FUN_L00_001fff28(void *, int, float);
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L05_0030c220(struct Moby *m);
void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");

void FUN_L05_0030c0a8(unsigned char *m);
void FUN_L05_0030c0a8(unsigned char *m) {
    char *d = *(char **)(m + 0x78);
    float r, lo, hi;
    switch (m[0x20]) {
    case 0:
        *(float *)(d + 0x40) = *(float *)(m + 0x48);
        *(float *)(d + 0x48) = random_angle_radians();
        r = random_angle_radians();
        lo = frame_time * 0.47123888f;
        hi = frame_time * 0.5235988f;
        *(float *)(d + 0x44) = r;
        r = randf_sym(lo, hi);
        lo = frame_time * 0.20943951f;
        hi = frame_time * 0.2617994f;
        *(float *)(d + 0x50) = r;
        *(float *)(d + 0x4C) = randf_sym(lo, hi);
        set_moby_color(m, 0x80, 0x80, 0x80);
        attach_manipulator((char *)m, 0, d);
        m[0x20] = 1;
        break;
    case 1: {
        float k = 0.5235988f;
        float t;
        enqueue_callback_list_1((void *)FUN_L05_0030c220, m);
        t = fast_add_rotations(*(float *)(d + 0x44), *(float *)(d + 0x4C));
        *(float *)(d + 0x44) = t;
        *(float *)(m + 0x48) = fast_add_rotations(*(float *)(d + 0x40), fast_sin(t) * k);
        t = fast_add_rotations(*(float *)(d + 0x48), *(float *)(d + 0x50));
        *(float *)(d + 0x48) = t;
        FUN_L00_001fff28(d + 0x10, 1, fast_sin(t) * k + k);
        break;
    }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L05_0030c220.s", FUN_L05_0030c220);
#else
extern void get_moby_position(struct Moby *, int, float *) __asm__("FUN_L00_0024f7c8");
extern void clear_vector(float *) __asm__("FUN_001f99f8");
extern void subtract_vector_xyz_30c220(float *, const float *, const float *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz_30c220(float *, float *, float) __asm__("FUN_001f9bf8");
extern void cross_vector_xyz_30c220(float *, float *, float *) __asm__("FUN_001f9ad8");
extern void rotation_vector_30c220(float *, float *) __asm__("FUN_001fa050");
extern u64 get_effect_texture_30c220(int) __asm__("FUN_001f44b8");
extern void draw_quad_30c220(void *, float *, int) __asm__("FUN_001f7d30");
extern float D_L05_001D67B0[];
extern float D_L05_001D67D0[][4];
extern float D_L05_001D6810[][4];
extern float D_L05_001671C0_30c220[] __asm__("D_L05_001671C0");
extern int D_L05_00161CF4 __attribute__((sda));
extern int D_L05_00161CF8 __attribute__((sda));
extern int D_L05_00161CFC __attribute__((sda));
extern int D_L05_00161D00 __attribute__((sda));
extern int D_L05_00161D04 __attribute__((sda));
extern int D_L05_00161D08 __attribute__((sda));
extern int D_L05_00161D0C __attribute__((sda));

typedef struct {
    float vertex[4][4];
    int color[4];
    float texcoord[8];
    u64 control[4];
} Quad_30c220;

void FUN_L05_0030c220(struct Moby *m) {
    Quad_30c220 a, b;
    float rotation[4];
    float position[4];
    /* Draw helper reads four consecutive vectors: side, direction, up, position. */
    float matrix[16] __attribute__((aligned(16)));
    float *side;
    float *direction;
    float *up;
    u64 texture;
    u64 flags;
    float phase;
    float sign;
    float pass;
    float offset_a_x, offset_a_z, offset_b_x, offset_b_z;
    int i, j, color;
    float *vertex_a, *vertex_b;
    char *vars = (char *)m->pvars;

    get_moby_position(m, 0, position);
    clear_vector(rotation);
    rotation[2] = m->rot.z;
    rotation[1] = fast_sin(*(float *)(vars + 0x48)) * 0.5235988f + 0.5235988f;
    side = matrix;
    rotation_vector_30c220(side, rotation);
    direction = matrix + 4;
    subtract_vector_xyz_30c220(direction, D_L05_001671C0_30c220, position);
    normalize_vector_xyz_30c220(direction, direction, 1.0f);
    up = matrix + 8;
    cross_vector_xyz_30c220(side, direction, up);
    cross_vector_xyz_30c220(direction, up, side);
    qcopy(matrix + 12, position);

    texture = get_effect_texture_30c220(D_L05_00161D0C);
    flags = (u64)(s64)D_L05_00161CF4 |
            ((u64)(s64)D_L05_00161CF8 << 2) |
            ((u64)(s64)D_L05_00161CFC << 4) |
            ((u64)(s64)D_L05_00161D00 << 6) |
            0x8000000000ULL;
    b.control[1] = texture;
    a.control[1] = texture;
    b.control[2] = 0xff9000000260ULL;
    b.control[3] = flags;
    a.control[2] = 0xff9000000260ULL;
    a.control[3] = flags;
    b.control[0] = 0;
    a.control[0] = 0;
    {
        float *texcoord_b = b.texcoord;
        float *texcoord_b_v = b.texcoord + 1;
        float *texcoord_a_v = a.texcoord + 1;
        float *texcoord_a = a.texcoord;
        float *texcoord = D_L05_001D67B0;
        int count = 3;
        do {
            float u = texcoord[0];
            float v = texcoord[1];
            *texcoord_a = u;
            *texcoord_a_v = v;
            *texcoord_b = u;
            *texcoord_b_v = v;
            texcoord += 2;
            texcoord_a += 2;
            texcoord_a_v += 2;
            texcoord_b += 2;
            texcoord_b_v += 2;
        } while (--count >= 0);
    }
    for (i = 0; i < 10; i++) {
        pass = (float)i;
        vertex_a = a.vertex[0];
        j = 0;
        vertex_b = b.vertex[0];
        do {
            qcopy(vertex_a, D_L05_001D67D0[j]);
            qcopy(b.vertex[j], D_L05_001D6810[j]);
            phase = (float)(i + (j & 1)) * 0.1f;
            sign = 1.0f;
            if (j > 1) sign = -1.0f;
            color = fast_tween_color(D_L05_00161D04, D_L05_00161D08, phase);
            b.color[j] = color;
            a.color[j] = color;
            offset_a_x = phase * 8.0f * sign;
            offset_a_z = pass * 10.0f;
            offset_b_x = phase * 6.0f * sign;
            offset_b_z = pass * 12.0f;
            offset_a_z = vertex_a[2] + offset_a_z;
            offset_a_x = vertex_a[0] + offset_a_x;
            vertex_a[2] = offset_a_z;
            vertex_a[0] = offset_a_x;
            vertex_b[0] += offset_b_x;
            vertex_b[2] += offset_b_z;
            vertex_a += 4;
            vertex_b += 4;
        } while (++j < 4);
        draw_quad_30c220(&a, matrix, 0);
        draw_quad_30c220(&b, matrix, 0);
    }
}
#endif
#include "rnc/overlay/level_globals.h"
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} SaveFlags_30c5b0;
extern SaveFlags_30c5b0 D_L05_001BB6B0;
extern SaveFlags_30c5b0 D_L05_001BAA50_s __asm__("D_L05_001BAA50");
extern unsigned char D_L05_0015FC88[] __attribute__((section(".sdata")));
extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
extern char *D_L05_001B0930[];
extern u8 D_0014C050[];
extern f32 random_angle_radians_c(void) __asm__("FUN_00213308");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_sin_c(f32) __asm__("FUN_001f9de0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");

void floor_button_update(struct Moby *moby) __asm__("FUN_L05_0030c5b0");
void floor_button_update(struct Moby *moby) {
    s32 *vars = (s32 *)moby->pvars;
    s16 id;
    s32 i;
    s32 list;
    char *entry;
    f32 angle;
    s32 color;
    u32 shade;
    u32 high;

    switch (moby->state) {
    case 0:
        *(f32 *)(vars + 3) = random_angle_radians_c();
        moby->pos.z -= 0.35f;
        id = (s16)moby->save_id;
        if (D_L05_001BB6B0.collected[id] != 0 ||
            (D_0014C190_257218[current_level_index][id >> 5] >> (moby->save_id & 31) & 1) != 0 ||
            (vars[2] != 0 && D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
            moby->command = 2;
            moby->glow_color = 0x80208020;
            moby->state = 2;
            list = vars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)D_L05_001B0930[list] > 0) {
                    do {
                        entry = D_L05_001B0930[list] + i * 16;
                        if (*(f32 *)(entry + 0x1C) == ((f32 *)vars)[1])
                            *(s32 *)(entry + 0x1C) = 0;
                        list = vars[0];
                        i++;
                    } while (i < *(s32 *)D_L05_001B0930[list]);
                }
            }
        } else {
            moby->state = 1;
        }
        break;
    case 1:
        angle = fast_add_rotations_c2(*(f32 *)(vars + 3), frame_time * 6.2831855f);
        *(f32 *)(vars + 3) = angle;
        color = truncate_float_to_s32((fast_sin_c(angle) * 4.0f - 3.0f) * 128.0f);
        if (color > 0x80) color = 0x80;
        else if (color < 0x20) color = 0x20;
        shade = 0x80000000 | (color << 8);
        high = color << 16;
        moby->glow_color = high | shade | color;
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            D_L05_001BAA50_s.collected[(s16)moby->save_id] = moby->mission + 2;
            if (moby->mission == 0xFF ||
                (D_L05_0015FC88[moby->mission] != 0xFF && D_0014C050[moby->mission + current_level_index * 16] == 0xFF)) {
                D_L05_001BB6B0.collected[(s16)moby->save_id] = moby->mission + 2;
            }
            moby->state = 2;
            moby->command = 1;
            moby->glow_color = 0x80208020;
            allocate_voice_for_target_entry(0, 0, moby);
            list = vars[0];
            if (list != -1) {
                i = 0;
                if (*(s32 *)D_L05_001B0930[list] > 0) {
                    do {
                        entry = D_L05_001B0930[list] + i * 16;
                        if (*(f32 *)(entry + 0x1C) == ((f32 *)vars)[1])
                            *(s32 *)(entry + 0x1C) = 0;
                        list = vars[0];
                        i++;
                    } while (i < *(s32 *)D_L05_001B0930[list]);
                }
            }
        }
        break;
    }
}
typedef struct {
    char pad0[0x44];
    short count;
    char pad46[0x132];
    char *mobys[1];
} ML;

extern ML D_L05_0016CC60;
extern int D_L05_0015F5C4;
extern unsigned char D_L05_0017C8A0[];
extern short D_L05_00161EE8 __attribute__((sda));

void FUN_L05_00317398(char *moby) {
    int i;

    if (D_L05_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L05_0016CC60.count; i++) {
            char *e = D_L05_0016CC60.mobys[i];

            if (*(short *)(e + 0xA6) == *(short *)(moby + 0xA6)) {
                unsigned char *s = D_L05_0017C8A0;

                if (s[1] == 0) {
                    attach_manipulator(e, 0, s);
                    *(float *)(s + 0x20) = *(float *)&D_L05_00161EE8;
                    *(float *)(s + 0x24) = *(float *)&D_L05_00161EE8;
                    *(float *)(s + 0x28) = *(float *)&D_L05_00161EE8;
                }
            }
        }
    }
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_00319B58), where it is exact; names translated to the US level program. */

typedef struct {
    int id;
    char pad0[0x1C];
    float s[3];
} Dat;

extern float random_float_between(float, float) __asm__("FUN_002132a8");

void FUN_L05_00318648(char *m, int b, void *initial_motion_step, void *local_rotation_offset, int a) {
    Dat *d = *(Dat **)(m + 0x78);
    m[0x20] = 1;
    qcopy((char *)d + 0x30, initial_motion_step);
    qcopy((char *)d + 0x10, local_rotation_offset);
    d->id = scale_game_frames(a);
    d->s[0] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
    d->s[1] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
    d->s[2] = random_float_between(-(frame_time * 4.1887903f), frame_time * 4.1887903f);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_00319C08), where it is exact; names translated to the US level program. */

extern char D_L05_00174240[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");

void blarg_gun_update(char *moby) __asm__("FUN_L05_003186f8");
void blarg_gun_update(char *moby)
{
    char *data = *(char **)(moby + 0x78);
    char buf[0x70];
    char *hs;
    int n;
    float t;

    switch ((unsigned char)moby[0x20]) {
    case 0:
        if (*(unsigned short *)(moby + 0x32) & 1)
            *(unsigned short *)(moby + 0x34) |= 0x8000;
        moby[0x20] = 2;
        break;
    case 1:
        n = *(int *)(data + 4);
        if (n > 0) {
            moby_point_position(moby, n, buf + 0x40);
            *(float *)(moby + 0x40) = fast_add_rotations_c(*(float *)(moby + 0x40), *(float *)(data + 0x20));
            *(float *)(moby + 0x44) = fast_add_rotations_c(*(float *)(moby + 0x44), *(float *)(data + 0x24));
            *(float *)(moby + 0x48) = fast_add_rotations_c(*(float *)(moby + 0x48), *(float *)(data + 0x28));
            euler_rotation_matrix(buf, moby + 0x40);
            copy_matrix3x4(moby + 0xC0, buf);
            moby_point_position(moby, *(int *)(data + 4), buf + 0x50);
            subtract_vector_xyz(buf + 0x60, buf + 0x40, buf + 0x50);
        } else {
            transform_vector_by_basis(buf + 0x40, data + 0x10, moby + 0xC0);
            *(float *)(moby + 0x40) = fast_add_rotations_c(*(float *)(moby + 0x40), *(float *)(data + 0x20));
            *(float *)(moby + 0x44) = fast_add_rotations_c(*(float *)(moby + 0x44), *(float *)(data + 0x24));
            *(float *)(moby + 0x48) = fast_add_rotations_c(*(float *)(moby + 0x48), *(float *)(data + 0x28));
            euler_rotation_matrix(buf, moby + 0x40);
            if (*(unsigned short *)(moby + 0x34) & 0x8000)
                scale_vector_xyz(buf + 0x10, buf + 0x10, -1.0f);
            copy_matrix3x4(moby + 0xC0, buf);
            transform_vector_by_basis(buf + 0x50, data + 0x10, moby + 0xC0);
            subtract_vector_xyz(buf + 0x60, buf + 0x40, buf + 0x50);
            add_vector_xyz(buf + 0x50, buf + 0x50, moby + 0x10);
        }

        if (coll_sphere(convert_integer_to_float(*(int *)(data + 0xC)) * 0.0009765625f, buf + 0x50, 0, moby)) {
            hs = D_L05_00174240;
            if (*(int *)(hs + 0x18) == 0 || *(int *)(hs + 0x18) != *(int *)(data + 8)) {
                *(int *)data = 0;
                if (dot_vectors_xyz(data + 0x30, hs + 0x40) < 0.0f) {
                    FUN_L00_001ff660(data + 0x30, data + 0x30, hs + 0x40);
                    scale_vector_xyz(data + 0x30, data + 0x30, 0.5f);
                }
            }
        }
        if (fast_dec_timer((int *)data)) {
            t = 128.0f / (float)scale_game_frames(20);
            *(int *)data = 0;
            if (t <= (float)((unsigned char *)moby)[0x23]) {
                ((unsigned char *)moby)[0x23] = ((unsigned char *)moby)[0x23] - (int)t;
            } else {
                mark_moby_for_removal(moby);
            }
        }
        *(float *)(data + 0x38) = *(float *)(data + 0x38) - frame_time_sq * 10.8f;
        add_vector_xyz(moby + 0x10, moby + 0x10, data + 0x30);
        add_vector_xyz(moby + 0x10, moby + 0x10, buf + 0x60);
        if (*(float *)(moby + 0x18) < 5.0f)
            mark_moby_for_removal(moby);
        break;
    }
}
#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern char *func_L05_0031AAA8_19510(void *, int) __asm__("FUN_L05_00319598");
extern void func_L16_002E5D68_19510(void *) __asm__("FUN_L05_00319208");
extern char *D_L16_001601AC_m_19510 __asm__("D_L05_001600EC");
void FUN_L05_00319510(void *moby_v) {
    char *moby = moby_v;
    char *data = *(char **)(moby + 0x78);
    char *spawn = func_L05_0031AAA8_19510(moby, *(short *)(data + 0xB6));
    char *position;
    if (spawn != 0) {
        int index;
        char *slot = data;
        char *entry;
        position = spawn + 0x10;
        slot += *(short *)(data + 0xB4) << 2;
        index = *(int *)(slot + 0x80) << 7;
        entry = D_L16_001601AC_m_19510;
        qcopy(position, (void *)(index + (int)entry + 0x30));
        qcopy(spawn + 0x40, (void *)(index + (int)entry + 0x70));
        func_L16_002E5D68_19510(spawn);
    }
    *(short *)(data + 0xB4) = *(unsigned short *)(data + 0xB6);
}
/* Finds the pool moby in this moby's class list whose data id at 0xB4 matches. */
extern unsigned short *D_L05_001ABCC0_9598[] __asm__("D_L05_001ABCC0");
extern char *D_L05_0015FFD8_9598 __asm__("D_L05_0015FFD8");
typedef struct { char pad[0x78]; char *data; char pad2[0x84]; } M_9598;
char *FUN_L05_00319598(unsigned char *arg, int id) {
    unsigned short *p = D_L05_001ABCC0_9598[arg[0x21]];
    char *pool; int idx;
    if (p != 0) {
        pool = D_L05_0015FFD8_9598;
        do {
            idx = *p & 0x7FFF;
            if (*(short *)(((M_9598 *)pool)[idx].data + 0xB4) == id) goto found;
        } while ((short)*p++ >= 0);
    }
    return 0;
found:
    return (char *)&((M_9598 *)D_L05_0015FFD8_9598)[idx];
}
#define NOT_SDA

#define MACRO_ADDR

/* Advances a moby's path: steps the path position and returns whether it has reached the end. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031AB08), where it is exact; names translated to the US level program. */

extern float advance_accelerated_scalar(float, float, float, float, float *,
                                        float *) __asm__("FUN_00213f38");
extern void sample_camera_path(int *, int, void *, float *, int, float) __asm__("FUN_00214e58");

int FUN_L05_003195f8(char *m) {
    char *d = *(char **)(m + 0x78);
    int *path;
    advance_accelerated_scalar(1.0f, *(float *)(d + 0xD8), *(float *)(d + 0xD8),
                               *(float *)(d + 0xD4), (float *)(d + 0xC0), (float *)(d + 0xC4));
    path = *(int **)(d + 0xAC);
    sample_camera_path(path, 0, d + 0x60, (float *)(d + 0x70), 0,
                       *(float *)(d + 0xC0) * (float)(*path - 1));
    return *(float *)(d + 0xC0) >= 1.0f;
}
/* Plays the sound that goes with the moby's current variant. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031ABA0), where it is exact; names translated to the US level program. */

s32 try_set_help_message(s32 arg0, s32 arg1) __asm__("FUN_00215130");

void FUN_L05_00319690(char *moby) {
    switch (*(short *)(*(char **)(moby + 0x78) + 0xB6)) {
    case 0:
        try_set_help_message(8, 0x13A5);
        break;
    case 1:
        try_set_help_message(8, 0x1398);
        break;
    case 2:
        try_set_help_message(8, 0x139A);
        break;
    case 3:
        try_set_help_message(8, 0x139C);
        break;
    case 4:
        try_set_help_message(8, 0x1399);
        break;
    default:
        try_set_help_message(8, 0x139B);
        break;
    }
}
#include "qcopy.h"

/* Wobbles a moby's position and rotation from its sine-driven state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031AC50), where it is exact; names translated to the US level program. */

extern float fast_sin(float) __asm__("FUN_001f9de0");

void FUN_L05_00319740(char *moby) {
    char *d = *(char **)(moby + 0x78);
    float s, t, w;
    qcopy(moby + 0x10, d + 0x60);
    qcopy(moby + 0x40, d + 0x70);
    s = fast_add_rotations(*(float *)(d + 0xC8), frame_time * 2.0943951f);
    *(float *)(d + 0xC8) = s;
    t = fast_sin(s);
    w = frame_time * 1.6929693f;
    *(float *)(moby + 0x18) += t * 0.25f;
    s = fast_add_rotations(*(float *)(d + 0xCC), w);
    *(float *)(d + 0xCC) = s;
    t = fast_add_rotations(*(float *)(moby + 0x44), fast_sin(s) * 0.08726646f);
    w = frame_time * 0.9250245f;
    *(float *)(moby + 0x44) = t;
    s = fast_add_rotations(*(float *)(d + 0xD0), w);
    *(float *)(d + 0xD0) = s;
    *(float *)(moby + 0x40) =
        fast_add_rotations(*(float *)(moby + 0x40), fast_sin(s) * 0.08726646f);
    if (*(float *)(moby + 0x40) > 0.17453292f)
        *(float *)(moby + 0x40) = 0.17453292f;
    else if (*(float *)(moby + 0x40) < -0.17453292f)
        *(float *)(moby + 0x40) = -0.17453292f;
    if (*(float *)(moby + 0x44) > 0.17453292f)
        *(float *)(moby + 0x44) = 0.17453292f;
    else if (*(float *)(moby + 0x44) < -0.17453292f)
        *(float *)(moby + 0x44) = -0.17453292f;
}
/* Transforms the 7 vectors of a level table entry into the moby's space and records its index. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031ADF8), where it is exact; names translated to the US level program. */

extern char *D_L05_001B0930[];
extern char D_L05_00215C00[];
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");

void FUN_L05_003198e8(char *a) {
    char *data = *(char **)(a + 0x78);
    int idx = *(int *)(data + 0xDC);
    if (idx != -1) {
        char *p = D_L05_001B0930[idx];
        if (*(int *)p == 7) {
            int i;
            struct Hero *g;
            for (i = 0; i < 7; i++) {
                transform_vector_by_basis(p + 0x10 + i * 16, D_L05_00215C00 + i * 16, a + 0xC0);
                fast_vec_add(p + 0x10 + i * 16, p + 0x10 + i * 16, a + 0x10);
            }
            g = &hero;
            *(unsigned short *)&g->unk22DA = *(unsigned short *)(data + 0xDC);
        }
    }
}
