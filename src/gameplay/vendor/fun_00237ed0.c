#include "types.h"
#include "sda.h"

typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0x32];
    u8 anim;
    u8 pad54[0x1C];
    u8 flags;
} Moby;

typedef struct {
    u8 pad0[0x34];
    s32 timer;
    s32 base;
    s32 pick;
} BossVars;

typedef struct {
    u8 pad0;
    u8 active;
    u8 pad2[0x1E];
    f32 x;
    f32 y;
    f32 z;
} Cam;

extern s32 D_001516EC NOT_SDA;
extern s16 D_0015172A NOT_SDA;
extern u8 D_0015EDB0 MACRO_ADDR;
extern s32 D_00161030[2] __attribute__((sda));
extern s32 D_00161038[2] __attribute__((sda));
extern s32 D_00161040[2] __attribute__((sda));
extern s32 D_001610B0 MACRO_ADDR;
extern s32 D_001610B4 MACRO_ADDR;
extern BossVars D_001E63C0;
extern s32 D_001E63FC NOT_SDA;
extern Cam D_001E65E0;

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void attach_manipulator(Moby *, s32, Cam *) __asm__("FUN_0020cb10");
extern void detach_manipulator(Moby *, Cam *) __asm__("FUN_0020cb88");
extern void blend_moby_animation(Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern s32 is_value_within_interpolated_window(Moby *, f32) __asm__("FUN_00214cc8");
extern s32 continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");

void FUN_00237ed0(Moby *m) __asm__("FUN_00237ed0");

void FUN_00237ed0(Moby *m) {
    s32 r;
    s32 a;

    if (D_0015EDB0 != 0) {
        if (D_001E65E0.active == 0) {
            attach_manipulator(m, 0, &D_001E65E0);
        }
        D_001E65E0.x = 2.37f;
        D_001E65E0.y = 2.37f;
        D_001E65E0.z = 2.37f;
    } else if (D_001E65E0.active != 0) {
        detach_manipulator(m, &D_001E65E0);
    }
    switch (m->state) {
    case 0:
        if (D_001E63C0.timer > scale_game_frames(600)) {
            r = random_integer_below(2);
            D_001E63C0.pick = r;
            D_001516EC = (r * 3 + 1) * 6 + D_001E63C0.base + 10000;
            D_001610B0 = 1;
            D_001610B4 = 0;
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(0x12));
            }
            m->state = 12;
            D_001E63C0.timer = 0;
        }
        D_001E63C0.timer++;
        break;
    case 2:
        if (D_001E63C0.timer > scale_game_frames(600)) {
            if (m->anim != 3) {
                blend_moby_animation(m, 3, 0, scale_game_frames(0x12));
            }
            m->state = 3;
            D_001E63C0.timer = 0;
        }
        D_001E63C0.timer++;
        break;
    case 3:
        if (m->flags & 2) {
            if (m->anim != 0) {
                blend_moby_animation(m, 0, 0, scale_game_frames(0x12));
            }
            m->state = 0;
        }
        break;
    case 4:
        if (is_value_within_interpolated_window(m, D_00161030[D_001E63C0.pick]) != 0 &&
            D_001610B4 != 0) {
            D_001610B4 = 0;
            continue_audio_stream_if_ready();
        }
        if (m->flags & 2) {
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(0x12));
            }
            m->state = 2;
            D_001E63C0.timer = 0;
        }
        break;
    case 5:
        if (is_value_within_interpolated_window(m, D_00161038[D_001E63C0.pick]) != 0 &&
            D_001610B4 != 0) {
            D_001610B4 = 0;
            continue_audio_stream_if_ready();
        }
        if (m->flags & 2) {
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(0x12));
            }
            m->state = 2;
            D_001E63C0.timer = 0;
        }
        break;
    case 6:
        if (is_value_within_interpolated_window(m, D_00161040[D_001E63C0.pick]) != 0 &&
            D_001610B4 != 0) {
            D_001610B4 = 0;
            continue_audio_stream_if_ready();
        }
        if (m->flags & 2) {
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(0x12));
            }
            m->state = 2;
            D_001E63C0.timer = 0;
        }
        break;
    case 10:
        if (D_0015172A == 3 && D_001610B0 != 0) {
            D_001610B0 = 0;
            D_001610B4 = 1;
        }
        if (m->flags & 2) {
            a = D_001E63FC * 3 + 4;
            if (m->anim != a) {
                blend_moby_animation(m, a, 0, scale_game_frames(0xC));
            }
            m->state = 4;
        }
        break;
    case 12:
        if (D_0015172A == 3 && D_001610B0 != 0) {
            D_001610B4 = 1;
            D_001610B0 = 0;
            a = D_001E63FC * 3 + 5;
            if (m->anim != a) {
                blend_moby_animation(m, a, 0, scale_game_frames(0x12));
            }
            m->state = 5;
        }
        break;
    case 11:
        if (D_0015172A == 3 && D_001610B0 != 0) {
            D_001610B4 = 1;
            D_001610B0 = 0;
            a = D_001E63FC * 3 + 6;
            if (m->anim != a) {
                blend_moby_animation(m, a, 0, scale_game_frames(0x12));
            }
            m->state = 6;
        }
        break;
    }
}

extern __typeof__(FUN_00237ed0) func_00237ED0 __attribute__((alias("FUN_00237ed0")));
