#include "types.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/options.h"
#include "rnc/math/random.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "rnc/mobys/detach_manipulator.h"

typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0x32];
    u8 anim;
    u8 pad54[0x1C];
    u8 flags;
} Moby;

typedef struct {
    u8  pad0[0x34];
    s32 timer;
    s32 base;
    s32 pick;
} BossVars;

typedef struct {
    u8  pad0;
    u8  active;
    u8  pad2[0x1E];
    f32 x;
    f32 y;
    f32 z;
} Cam;

extern s32 D_001516EC NOT_SDA;
extern s16 D_0015172A NOT_SDA;
extern s32            resident_dialogue_after_completion_wait_thresholds[2] __asm__("D_00161030")
    __attribute__((sda));
extern s32 resident_dialogue_after_start_wait_thresholds[2] __asm__("D_00161038")
    __attribute__((sda));
extern s32 resident_dialogue_before_timed_idle_restore_thresholds[2] __asm__("D_00161040")
    __attribute__((sda));
extern s32 D_001610B0 MACRO_ADDR;
extern s32 D_001610B4 MACRO_ADDR;
extern BossVars       D_001E63C0;
s32 D_001E63FC        NOT_SDA = 0;
extern Cam            D_001E65E0;

extern s32 is_value_within_interpolated_window(Moby *, f32) __asm__("FUN_00214cc8");

void update_resident_dialogue_animation_state(Moby *m) __asm__("FUN_00237ed0");

void update_resident_dialogue_animation_state(Moby *resident_moby) {
    enum {
        RESIDENT_IDLE_WAIT_TO_REQUEST_DIALOGUE = 0,
        RESIDENT_TIMED_WAIT_TO_RESTORE_IDLE_ANIMATION = 2,
        RESIDENT_WAIT_TO_RESTORE_IDLE_ANIMATION = 3,
        RESIDENT_DIALOGUE_ANIMATION_AFTER_COMPLETION_WAIT = 4,
        RESIDENT_DIALOGUE_ANIMATION_AFTER_START_WAIT = 5,
        RESIDENT_DIALOGUE_ANIMATION_WAIT_TO_RESTORE_TIMED_IDLE = 6,
        RESIDENT_WAIT_FOR_ANIMATION_COMPLETION_TO_START_DIALOGUE_ANIMATION = 10,
        RESIDENT_WAIT_TO_START_DIALOGUE_ANIMATION_BEFORE_TIMED_IDLE_RESTORE = 11,
        RESIDENT_WAIT_TO_START_DIALOGUE_ANIMATION = 12
    };
    s32 dialogue_variant_index;
    s32 target_animation;

    if (big_head_cheat_enabled != 0) {
        if (D_001E65E0.active == 0) {
            attach_manipulator(resident_moby, 0, &D_001E65E0);
        }
        D_001E65E0.x = 2.37f;
        D_001E65E0.y = 2.37f;
        D_001E65E0.z = 2.37f;
    } else if (D_001E65E0.active != 0) {
        detach_manipulator(resident_moby, &D_001E65E0);
    }
    switch (resident_moby->state) {
    case RESIDENT_IDLE_WAIT_TO_REQUEST_DIALOGUE:
        if (D_001E63C0.timer > scale_game_frames(600)) {
            dialogue_variant_index = random_integer_below(2);
            D_001E63C0.pick = dialogue_variant_index;
            D_001516EC = (dialogue_variant_index * 3 + 1) * 6 + D_001E63C0.base + 10000;
            D_001610B0 = 1;
            D_001610B4 = 0;
            if (resident_moby->anim != 2) {
                blend_moby_animation(resident_moby, 2, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 12;
            D_001E63C0.timer = 0;
        }
        D_001E63C0.timer++;
        break;
    case RESIDENT_TIMED_WAIT_TO_RESTORE_IDLE_ANIMATION:
        if (D_001E63C0.timer > scale_game_frames(600)) {
            if (resident_moby->anim != 3) {
                blend_moby_animation(resident_moby, 3, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 3;
            D_001E63C0.timer = 0;
        }
        D_001E63C0.timer++;
        break;
    case RESIDENT_WAIT_TO_RESTORE_IDLE_ANIMATION:
        if (resident_moby->flags & 2) {
            if (resident_moby->anim != 0) {
                blend_moby_animation(resident_moby, 0, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 0;
        }
        break;
    case RESIDENT_DIALOGUE_ANIMATION_AFTER_COMPLETION_WAIT:
        if (is_value_within_interpolated_window(
                resident_moby,
                resident_dialogue_after_completion_wait_thresholds[D_001E63C0.pick]) != 0 &&
            D_001610B4 != 0) {
            D_001610B4 = 0;
            continue_audio_stream_if_ready();
        }
        if (resident_moby->flags & 2) {
            if (resident_moby->anim != 2) {
                blend_moby_animation(resident_moby, 2, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 2;
            D_001E63C0.timer = 0;
        }
        break;
    case RESIDENT_DIALOGUE_ANIMATION_AFTER_START_WAIT:
        if (is_value_within_interpolated_window(
                resident_moby, resident_dialogue_after_start_wait_thresholds[D_001E63C0.pick]) !=
                0 &&
            D_001610B4 != 0) {
            D_001610B4 = 0;
            continue_audio_stream_if_ready();
        }
        if (resident_moby->flags & 2) {
            if (resident_moby->anim != 2) {
                blend_moby_animation(resident_moby, 2, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 2;
            D_001E63C0.timer = 0;
        }
        break;
    case RESIDENT_DIALOGUE_ANIMATION_WAIT_TO_RESTORE_TIMED_IDLE:
        if (is_value_within_interpolated_window(
                resident_moby,
                resident_dialogue_before_timed_idle_restore_thresholds[D_001E63C0.pick]) != 0 &&
            D_001610B4 != 0) {
            D_001610B4 = 0;
            continue_audio_stream_if_ready();
        }
        if (resident_moby->flags & 2) {
            if (resident_moby->anim != 2) {
                blend_moby_animation(resident_moby, 2, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 2;
            D_001E63C0.timer = 0;
        }
        break;
    case RESIDENT_WAIT_FOR_ANIMATION_COMPLETION_TO_START_DIALOGUE_ANIMATION:
        if (D_0015172A == 3 && D_001610B0 != 0) {
            D_001610B0 = 0;
            D_001610B4 = 1;
        }
        if (resident_moby->flags & 2) {
            target_animation = D_001E63FC * 3 + 4;
            if (resident_moby->anim != target_animation) {
                blend_moby_animation(resident_moby, target_animation, 0, scale_game_frames(0xC));
            }
            resident_moby->state = 4;
        }
        break;
    case RESIDENT_WAIT_TO_START_DIALOGUE_ANIMATION:
        if (D_0015172A == 3 && D_001610B0 != 0) {
            D_001610B4 = 1;
            D_001610B0 = 0;
            target_animation = D_001E63FC * 3 + 5;
            if (resident_moby->anim != target_animation) {
                blend_moby_animation(resident_moby, target_animation, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 5;
        }
        break;
    case RESIDENT_WAIT_TO_START_DIALOGUE_ANIMATION_BEFORE_TIMED_IDLE_RESTORE:
        if (D_0015172A == 3 && D_001610B0 != 0) {
            D_001610B4 = 1;
            D_001610B0 = 0;
            target_animation = D_001E63FC * 3 + 6;
            if (resident_moby->anim != target_animation) {
                blend_moby_animation(resident_moby, target_animation, 0, scale_game_frames(0x12));
            }
            resident_moby->state = 6;
        }
        break;
    }
}

extern __typeof__(update_resident_dialogue_animation_state) func_00237ED0
    __attribute__((alias("FUN_00237ed0")));
