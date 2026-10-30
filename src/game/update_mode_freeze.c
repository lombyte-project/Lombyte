#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/options.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/audio/voices/snd_continue_all_sounds_in_group.h"

struct FreezeModeState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

extern u16                    D_0013E05A[];
extern u8                     saved_camera_vectors[] __asm__("D_00141050");
extern s32                    D_0015EE38;
extern s32                    D_0015EE3C;
extern s32                    D_0015F648;
extern struct FreezeModeState freeze __asm__("D_00193300");
extern s32                    pause_current_action_list[] __asm__("D_001D5BF8");
extern char                   sound_update() __asm__("func_0022CA50");
extern s32                    scale_game_frames(s32) __asm__("func_001F96F8");
extern void                   memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void                   noop_main_callback() __asm__("func_001E93E8");
extern void                   noop_callback_k(u8 *, u8 *, s32, s32) __asm__("func_001E9440");
extern void                   fade_to_black(s32) __asm__("func_001F4A58");
extern void                   set_animation_parameter(s32, s32) __asm__("func_001FF570");
extern void                   decrement_resource_counter(void) __asm__("UpdateResourceCounter");
extern void                   load_and_initialize_level_chunk() __asm__("func_00209370");
extern void                   InitializeGlobalStateEntry(s32 value);
extern void                   music_unpause();
extern void                   snd_flush_sound_commands() __asm__("func_0012DC80");
void                          update_mode_freeze(void) __asm__("FUN_001fce28");

void update_mode_freeze(void) {
    enum {
        VIDEO_MODE_TEST_INPUT_DELAY = 0,
        VIDEO_MODE_TEST_AWAIT_START = 1,
        VIDEO_MODE_TEST_PREVIEW_ACTIVE = 2,
        VIDEO_MODE_TEST_AWAIT_TIMEOUT_ACK = 3
    };
    enum {
        FREEZE_MODE_CONFIRM_CANCEL_CHOICE = 0,
        FREEZE_MODE_TRIANGLE_SET_SHIP_FLAG_CONTINUE = 1,
        FREEZE_MODE_AWAIT_CROSS_CONTINUE = 2,
        FREEZE_MODE_STATE_DISPATCH = 3,
        FREEZE_MODE_TRIANGLE_HIDE_HERO_CONTINUE = 4,
        FREEZE_MODE_DELAYED_CONTINUE = 5,
        FREEZE_MODE_VIDEO_MODE_TEST = 6
    };
    enum {
        FREEZE_CHOICE_PREPARE_INPUT = 0,
        FREEZE_CHOICE_AWAIT_INPUT = 1,
        FREEZE_CHOICE_CONFIRM_EXIT = 2,
        FREEZE_CHOICE_CANCEL_EXIT = 3
    };
    enum {
        MODE_FREEZE_AWAIT_CROSS_RESUME = 2,
        MODE_FREEZE_AWAIT_TRIANGLE_RESUME = 3,
        MODE_FREEZE_BOOT_FLAGS_RELOAD_OR_RESUME = 4,
        MODE_FREEZE_TRIANGLE_CLEAR_FLAGS_RESUME = 5,
        MODE_FREEZE_BOOT_AWAIT_CLEAR_FLAGS_RESUME = 9,
        MODE_FREEZE_FLAG_GATED_TRIANGLE_RESUME = 12,
        MODE_FREEZE_RESUME_WITH_ACTION_LIST = 16,
        MODE_FREEZE_BOOT_GATED_CROSS_RELOAD = 19
    };
    s32 video_mode;
    s32 step;
    sound_update();
    if (freeze.unk4 != 0) {
        freeze.unk4--;
    }
    switch (freeze.unk0) {
    case FREEZE_MODE_DELAYED_CONTINUE: {
        struct FreezeModeState *state = &freeze;
        state->unk20++;
        if (scale_game_frames(0x5A) < state->unk20) {
            if (state->unk24 != 0) {
                state->unk24--;
            }
        }
        state = &freeze;
        if (scale_game_frames(0x78) < state->unk20) {
            if (controller_state.pressed & PAD_CROSS) {
                game_mode = 0;
                if (current_level_index == 1) {
                    memcard_save_data(0, -1);
                }
            }
        }
        break;
    }

    case FREEZE_MODE_TRIANGLE_HIDE_HERO_CONTINUE:
        if (controller_state.pressed & PAD_TRIANGLE) {
            struct Moby *moby = hero.moby;
            moby->visible = 0;
            moby->collision = 0;
            moby->flags = (u16)(moby->flags | 1);
            noop_main_callback();
            noop_callback_k(hero.saved_pos_rot, hero.saved_pos_rot + 0x10, 0, 1);
            game_mode = 0;
        } else if (controller_state.pressed & PAD_CROSS) {
            game_mode = 0;
        }
        break;

    case FREEZE_MODE_VIDEO_MODE_TEST:
        switch (freeze.unk1C) {
        case VIDEO_MODE_TEST_INPUT_DELAY:
            if (freeze.unk4 != 0) {
                if ((--freeze.unk4) == 0) {
                    freeze.unk1C = 1;
                }
            } else {
                freeze.unk1C = 1;
            }
            break;

        case VIDEO_MODE_TEST_AWAIT_START:
            if (controller_state.pressed & PAD_TRIANGLE) {
                game_mode = freeze.unk14;
            } else if (controller_state.pressed & PAD_CROSS) {
                fade_to_black(4);
                requested_video_mode = 0;
                freeze.unk1C = 2;
                freeze.unk20 = scale_game_frames(0x258);
            }
            break;

        case VIDEO_MODE_TEST_PREVIEW_ACTIVE:
            if ((freeze.unk20 == 0) || ((--freeze.unk20) == 0)) {
                requested_video_mode = 1;
                freeze.unk1C = 3;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                fade_to_black(4);
                requested_video_mode = 1;
                game_mode = freeze.unk14;
            } else if (controller_state.pressed & PAD_CROSS) {
                requested_video_mode = 0;
                game_mode = freeze.unk14;
            }
            break;

        case VIDEO_MODE_TEST_AWAIT_TIMEOUT_ACK:
            if (controller_state.pressed & PAD_CROSS) {
                game_mode = freeze.unk14;
            }
            break;

        default:
            game_mode = freeze.unk14;
            break;
        }

        break;

    case FREEZE_MODE_TRIANGLE_SET_SHIP_FLAG_CONTINUE:
        D_0015F648 = 2;
        if (controller_state.pressed & PAD_TRIANGLE) {
            game_mode = 0;
            hero.ship_flags = (u8)(hero.ship_flags | 1);
        } else if (controller_state.pressed & PAD_CROSS) {
            game_mode = 0;
        }
        break;

    case FREEZE_MODE_AWAIT_CROSS_CONTINUE:
        D_0015F648 = 2;
        if (controller_state.pressed & PAD_CROSS) {
            game_mode = 0;
        }
        break;

    case FREEZE_MODE_CONFIRM_CANCEL_CHOICE:
        switch (freeze.unk1C) {
        case FREEZE_CHOICE_PREPARE_INPUT:
            if (freeze.unk20 < 8) {
                freeze.unk20++;
            } else if (freeze.unk24 < 8) {
                freeze.unk24++;
            } else {
                freeze.unk1C = 1;
            }
            break;

        case FREEZE_CHOICE_AWAIT_INPUT:
            if (controller_state.pressed & PAD_CROSS) {
                freeze.unk1C = 2;
            } else if (controller_state.pressed & (PAD_CIRCLE | PAD_START)) {
                freeze.unk1C = 3;
            }
            /* Fakematch: this always-true test gives case 2/3 a second predecessor. */
            video_mode = requested_video_mode;
            if (video_mode >= 0)
                break;

        case FREEZE_CHOICE_CONFIRM_EXIT:
        case FREEZE_CHOICE_CANCEL_EXIT:
            if (freeze.unk24 != 0) {
                freeze.unk24--;
            } else if (freeze.unk20 != 0) {
                freeze.unk20--;
            } else {
                step = freeze.unk1C;
                /* Fakematch: a never-true test that combine folds away; it makes step
                   live in two blocks, so global-alloc gives it a1. */
                video_mode = requested_video_mode;
                if (video_mode < 0) {
                    freeze.unk1C = step;
                } else if (step == 2) {
                    if (hero.trick_hud_handle != (-1)) {
                        set_animation_parameter(hero.trick_hud_handle, 0);
                        hero.trick_hud_handle = -1;
                    }
                    decrement_resource_counter();
                    if (scale_game_frames(0x1068) < hero.control_mode_timer) {
                        if (current_level_index == 5) {
                            D_0015EE38++;
                        } else if (current_level_index == 0x10) {
                            D_0015EE3C++;
                        }
                    }
                    game_mode = 0;
                    noop_callback_k(saved_camera_vectors, saved_camera_vectors + 0x10, 0, 1);
                } else {
                    game_mode = 0;
                    if (hero.race_lap >= 3) {
                        hero.race_lap = (s16)(((u16)hero.race_lap) - 1);
                        hero.unk890[0xBC] = 3;
                    }
                }
            }
            break;
        }
        break;

    case FREEZE_MODE_STATE_DISPATCH:
        freeze.unk20++;
        if (scale_game_frames(0x1E) < freeze.unk20) {
            if (freeze.unk24 != 0) {
                freeze.unk24--;
            }
        }
        switch (mode_freeze_state) {
        case MODE_FREEZE_AWAIT_CROSS_RESUME:
            if (freeze.unk4 == 0) {
                if (controller_state.pressed & PAD_CROSS) {
                    mode_freeze_flags &= ~1;
                    game_mode = freeze.unk14;
                }
            }
            break;

        case 6:
            if (controller_state.pressed & PAD_CIRCLE) {
                mode_freeze_flags |= 8;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                if (boot_level_active == 0) {
                    game_mode = freeze.unk14;
                }
            }
            break;

        case 13:
            if (controller_state.pressed & PAD_CIRCLE) {
                mode_freeze_flags |= 0x10;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                if (boot_level_active == 0) {
                    game_mode = freeze.unk14;
                }
            }
            break;

        case 17:

        case 18:

        case 20:

        case 21:
            if (controller_state.pressed & PAD_CROSS) {
                mode_freeze_flags = mode_freeze_flags ^ 0x40;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.unk14;
            }
            break;

        case MODE_FREEZE_RESUME_WITH_ACTION_LIST:
            game_mode = freeze.unk14;
            pause_current_action_list[0] = freeze.unk18;
            break;

        case MODE_FREEZE_FLAG_GATED_TRIANGLE_RESUME:
            if (mode_freeze_flags & 2) {
                break;
            }

        case MODE_FREEZE_AWAIT_TRIANGLE_RESUME:

        case MODE_FREEZE_TRIANGLE_CLEAR_FLAGS_RESUME:
            if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.unk14;
            }
            break;

        case MODE_FREEZE_BOOT_GATED_CROSS_RELOAD:
            if ((boot_level_active != 0) && (mode_freeze_flags & 2)) {
                if (controller_state.pressed & PAD_CROSS) {
                    load_and_initialize_level_chunk();
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    mode_freeze_flags = mode_freeze_flags | 0x20;
                    InitializeGlobalStateEntry(0);
                    D_0013E05A[0] = 1;
                } else if (controller_state.pressed & PAD_TRIANGLE) {
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    game_mode = freeze.unk14;
                }
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.unk14;
            }
            break;

        case 23:

        case 24:
            if (controller_state.pressed & PAD_CROSS) {
                load_and_initialize_level_chunk();
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                mode_freeze_flags = mode_freeze_flags | 0x20;
                InitializeGlobalStateEntry(0);
                D_0013E05A[0] = 1;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags |= 0x20;
                game_mode = freeze.unk14;
            }
            break;

        case MODE_FREEZE_BOOT_FLAGS_RELOAD_OR_RESUME:
            if (boot_level_active != 0) {
                if (mode_freeze_flags & 2) {
                    struct PadState *pad = &controller_state;
                    if (pad->pressed & PAD_CROSS) {
                        load_and_initialize_level_chunk();
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        InitializeGlobalStateEntry(0);
                        D_0013E05A[0] = 1;
                    }
                    if (pad->pressed & PAD_TRIANGLE) {
                        mode_freeze_flags = mode_freeze_flags | 0x20;
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        game_mode = freeze.unk14;
                    }
                } else if (mode_freeze_flags & 4) {
                    if (controller_state.pressed & PAD_TRIANGLE) {
                        mode_freeze_flags = mode_freeze_flags | 0x20;
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        game_mode = freeze.unk14;
                    }
                } else {
                    game_mode = freeze.unk14;
                }
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.unk14;
            }
            break;

        case MODE_FREEZE_BOOT_AWAIT_CLEAR_FLAGS_RESUME:
            if (boot_level_active != 0) {
                if (!(mode_freeze_flags & 6)) {
                    game_mode = freeze.unk14;
                }
            }
            break;
        }

        break;
    }

    if (((u32)(game_mode - 3)) >= 2U) {
        snd_continue_all_sounds_in_group(0x1D);
        music_unpause();
        snd_flush_sound_commands();
    }
}

extern __typeof__(update_mode_freeze) func_001FCE28 __attribute__((alias("FUN_001fce28")));
