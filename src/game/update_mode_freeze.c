#include "types.h"
#include "rnc/game/freeze_mode_state.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/input/pad_state.h"
#include "rnc/game/options.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/audio/voices/snd_continue_all_sounds_in_group.h"
#include "rnc/game/transitions.h"
#include "rnc/runtime.h"

extern u16  D_0013E05A[];
extern u8   saved_camera_vectors[] __asm__("D_00141050");
extern s32  D_0015EE38;
extern s32  D_0015EE3C;
extern s32  D_0015F648;
extern s32  pause_current_action_list[] __asm__("D_001D5BF8");
extern char sound_update() __asm__("func_0022CA50");
extern s32  scale_game_frames(s32) __asm__("func_001F96F8");
extern void memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void noop_main_callback() __asm__("func_001E93E8");
extern void noop_callback_k(u8 *, u8 *, s32, s32) __asm__("func_001E9440");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void set_animation_parameter(s32, s32) __asm__("func_001FF570");
extern void load_and_initialize_level_chunk() __asm__("func_00209370");
extern void music_unpause();
extern void snd_flush_sound_commands() __asm__("func_0012DC80");
void        update_mode_freeze(void) __asm__("FUN_001fce28");

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
        GAME_MODE_CONTINUE = 0,
        VIDEO_MODE_REQUEST_PAL_DISABLED = 0,
        VIDEO_MODE_REQUEST_PAL_ENABLED = 1
    };
    s32 video_mode;
    s32 step;
    sound_update();
    if (freeze.timer != 0) {
        freeze.timer--;
    }
    switch (freeze.state) {
    case FREEZE_MODE_DELAYED_CONTINUE: {
        FreezeModeState *state = &freeze;
        state->box_timer++;
        if (scale_game_frames(0x5A) < state->box_timer) {
            if (state->fade_step != 0) {
                state->fade_step--;
            }
        }
        state = &freeze;
        if (scale_game_frames(0x78) < state->box_timer) {
            if (controller_state.pressed & PAD_CROSS) {
                game_mode = GAME_MODE_CONTINUE;
                if (current_level_index == LEVEL_NOVALIS) {
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
            game_mode = GAME_MODE_CONTINUE;
        } else if (controller_state.pressed & PAD_CROSS) {
            game_mode = GAME_MODE_CONTINUE;
        }
        break;

    case FREEZE_MODE_VIDEO_MODE_TEST:
        switch (freeze.choice) {
        case VIDEO_MODE_TEST_INPUT_DELAY:
            if (freeze.timer != 0) {
                if ((--freeze.timer) == 0) {
                    freeze.choice = 1;
                }
            } else {
                freeze.choice = 1;
            }
            break;

        case VIDEO_MODE_TEST_AWAIT_START:
            if (controller_state.pressed & PAD_TRIANGLE) {
                game_mode = freeze.return_game_mode;
            } else if (controller_state.pressed & PAD_CROSS) {
                fade_to_black(4);
                requested_video_mode = VIDEO_MODE_REQUEST_PAL_DISABLED;
                freeze.choice = 2;
                freeze.box_timer = scale_game_frames(0x258);
            }
            break;

        case VIDEO_MODE_TEST_PREVIEW_ACTIVE:
            if ((freeze.box_timer == 0) || ((--freeze.box_timer) == 0)) {
                requested_video_mode = VIDEO_MODE_REQUEST_PAL_ENABLED;
                freeze.choice = 3;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                fade_to_black(4);
                requested_video_mode = VIDEO_MODE_REQUEST_PAL_ENABLED;
                game_mode = freeze.return_game_mode;
            } else if (controller_state.pressed & PAD_CROSS) {
                requested_video_mode = VIDEO_MODE_REQUEST_PAL_DISABLED;
                game_mode = freeze.return_game_mode;
            }
            break;

        case VIDEO_MODE_TEST_AWAIT_TIMEOUT_ACK:
            if (controller_state.pressed & PAD_CROSS) {
                game_mode = freeze.return_game_mode;
            }
            break;

        default:
            game_mode = freeze.return_game_mode;
            break;
        }

        break;

    case FREEZE_MODE_TRIANGLE_SET_SHIP_FLAG_CONTINUE:
        D_0015F648 = 2;
        if (controller_state.pressed & PAD_TRIANGLE) {
            game_mode = GAME_MODE_CONTINUE;
            hero.ship_flags = (u8)(hero.ship_flags | 1);
        } else if (controller_state.pressed & PAD_CROSS) {
            game_mode = GAME_MODE_CONTINUE;
        }
        break;

    case FREEZE_MODE_AWAIT_CROSS_CONTINUE:
        D_0015F648 = 2;
        if (controller_state.pressed & PAD_CROSS) {
            game_mode = GAME_MODE_CONTINUE;
        }
        break;

    case FREEZE_MODE_CONFIRM_CANCEL_CHOICE:
        switch (freeze.choice) {
        case FREEZE_CHOICE_PREPARE_INPUT:
            if (freeze.box_timer < 8) {
                freeze.box_timer++;
            } else if (freeze.fade_step < 8) {
                freeze.fade_step++;
            } else {
                freeze.choice = 1;
            }
            break;

        case FREEZE_CHOICE_AWAIT_INPUT:
            if (controller_state.pressed & PAD_CROSS) {
                freeze.choice = 2;
            } else if (controller_state.pressed & (PAD_CIRCLE | PAD_START)) {
                freeze.choice = 3;
            }
            /* Fakematch: this always-true test gives case 2/3 a second predecessor. */
            video_mode = requested_video_mode;
            if (video_mode >= 0)
                break;

        case FREEZE_CHOICE_CONFIRM_EXIT:
        case FREEZE_CHOICE_CANCEL_EXIT:
            if (freeze.fade_step != 0) {
                freeze.fade_step--;
            } else if (freeze.box_timer != 0) {
                freeze.box_timer--;
            } else {
                step = freeze.choice;
                /* Fakematch: a never-true test that combine folds away; it makes step
                   live in two blocks, so global-alloc gives it a1. */
                video_mode = requested_video_mode;
                if (video_mode < 0) {
                    freeze.choice = step;
                } else if (step == 2) {
                    if (hero.trick_hud_handle != (-1)) {
                        set_animation_parameter(hero.trick_hud_handle, 0);
                        hero.trick_hud_handle = -1;
                    }
                    decrement_resource_counter();
                    if (scale_game_frames(0x1068) < hero.control_mode_timer) {
                        if (current_level_index == LEVEL_RILGAR) {
                            D_0015EE38++;
                        } else if (current_level_index == LEVEL_KALEBO3) {
                            D_0015EE3C++;
                        }
                    }
                    game_mode = GAME_MODE_CONTINUE;
                    noop_callback_k(saved_camera_vectors, saved_camera_vectors + 0x10, 0, 1);
                } else {
                    game_mode = GAME_MODE_CONTINUE;
                    if (hero.race_lap >= 3) {
                        hero.race_lap = (s16)(((u16)hero.race_lap) - 1);
                        hero.race_moby[0xBC] = 3;
                    }
                }
            }
            break;
        }
        break;

    case FREEZE_MODE_STATE_DISPATCH:
        freeze.box_timer++;
        if (scale_game_frames(0x1E) < freeze.box_timer) {
            if (freeze.fade_step != 0) {
                freeze.fade_step--;
            }
        }
        switch (save_card_state) {
        case CS_WARNING:
            if (freeze.timer == 0) {
                if (controller_state.pressed & PAD_CROSS) {
                    mode_freeze_flags &= ~1;
                    game_mode = freeze.return_game_mode;
                }
            }
            break;

        case CS_PROMPT_FORMAT:
            if (controller_state.pressed & PAD_CIRCLE) {
                mode_freeze_flags |= 8;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                if (boot_level_active == 0) {
                    game_mode = freeze.return_game_mode;
                }
            }
            break;

        case CS_PROMPT_CREATE_SAVE:
            if (controller_state.pressed & PAD_CIRCLE) {
                mode_freeze_flags |= 0x10;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                if (boot_level_active == 0) {
                    game_mode = freeze.return_game_mode;
                }
            }
            break;

        case CS_FORMAT_FAILED:

        case CS_CREATE_FAILED:

        case CS_LOAD_FAILED:

        case CS_SAVE_FAILED:
            if (controller_state.pressed & PAD_CROSS) {
                mode_freeze_flags = mode_freeze_flags ^ 0x40;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.return_game_mode;
            }
            break;

        case CS_NEWCARD:
            game_mode = freeze.return_game_mode;
            pause_current_action_list[0] = freeze.context;
            break;

        case CS_NOSAVE:
            if (mode_freeze_flags & 2) {
                break;
            }

        case CS_NOCARD:

        case CS_UNFORMATTED:
            if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.return_game_mode;
            }
            break;

        case CS_NO_ROOM:
            if ((boot_level_active != 0) && (mode_freeze_flags & 2)) {
                if (controller_state.pressed & PAD_CROSS) {
                    load_and_initialize_level_chunk();
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    mode_freeze_flags = mode_freeze_flags | 0x20;
                    request_space_load(0);
                    D_0013E05A[0] = 1;
                } else if (controller_state.pressed & PAD_TRIANGLE) {
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    game_mode = freeze.return_game_mode;
                }
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.return_game_mode;
            }
            break;

        case CS_PROMPT_BEGIN_UNFORMATTED:

        case CS_PROMPT_BEGIN_NOSAVE:
            if (controller_state.pressed & PAD_CROSS) {
                load_and_initialize_level_chunk();
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                mode_freeze_flags = mode_freeze_flags | 0x20;
                request_space_load(0);
                D_0013E05A[0] = 1;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags |= 0x20;
                game_mode = freeze.return_game_mode;
            }
            break;

        case CS_WAIT_FOR_CARD:
            if (boot_level_active != 0) {
                if (mode_freeze_flags & 2) {
                    struct PadState *pad = &controller_state;
                    if (pad->pressed & PAD_CROSS) {
                        load_and_initialize_level_chunk();
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        request_space_load(0);
                        D_0013E05A[0] = 1;
                    }
                    if (pad->pressed & PAD_TRIANGLE) {
                        mode_freeze_flags = mode_freeze_flags | 0x20;
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        game_mode = freeze.return_game_mode;
                    }
                } else if (mode_freeze_flags & 4) {
                    if (controller_state.pressed & PAD_TRIANGLE) {
                        mode_freeze_flags = mode_freeze_flags | 0x20;
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        game_mode = freeze.return_game_mode;
                    }
                } else {
                    game_mode = freeze.return_game_mode;
                }
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.return_game_mode;
            }
            break;

        case CS_FORMATTED:
            if (boot_level_active != 0) {
                if (!(mode_freeze_flags & 6)) {
                    game_mode = freeze.return_game_mode;
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

/* Small data: two counters only this file reads and bumps. */
s32 D_0015EE38 = 0;
s32 D_0015EE3C = 0;
