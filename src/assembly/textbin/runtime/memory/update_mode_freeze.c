#include "types.h"
#include "asm.h"
#include "rnc/globals.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/runtime/memory/update_mode_freeze/FUN_001fce28.s",
    FUN_001fce28);
#else
/* Near miss, 2 rows: the reloaded unk1C lands in v1 instead of a1. */
#include "types.h"
#include "rnc/globals.h"

#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/hero.h"
#include "rnc/input/pad_state.h"

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

extern u16 D_0013E05A[];
extern u8 D_00141050[];
extern s32 D_0015EE38;
extern s32 D_0015EE3C;
extern s32 boot_level_active __asm__("D_0015F5E8");
extern s32 D_0015F648;
extern u8 requested_video_mode __asm__("D_0016034C");
extern struct FreezeModeState freeze __asm__("D_00193300");
extern s32 pause_current_action_list[] __asm__("D_001D5BF8");
extern char sound_update() __asm__("func_0022CA50");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void noop_main_callback() __asm__("func_001E93E8");
extern void noop_callback_k(u8 *, u8 *, s32, s32) __asm__("func_001E9440");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void set_animation_parameter(s32, s32) __asm__("func_001FF570");
extern void update_resource_counter() __asm__("func_001FF768");
extern void load_and_initialize_level_chunk() __asm__("func_00209370");
extern void initialize_global_state_entry(s32) __asm__("func_0022E188");
extern void snd_continue_all_sounds_in_group(s32) __asm__("func_0012E418");
extern void music_unpause() __asm__("func_00216088");
extern void snd_flush_sound_commands() __asm__("func_0012DC80");
void update_mode_freeze(void) __asm__("FUN_001fce28");

void update_mode_freeze(void) {
    s32 video_mode;
    sound_update();
    if (freeze.unk4 != 0) {
        freeze.unk4--;
    }
    switch (freeze.unk0) {
    case 5: {
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

    case 4:
        if (controller_state.pressed & PAD_TRIANGLE) {
            struct Moby *moby = hero.moby;
            moby->visible = 0;
            moby->collision = 0;
            moby->flags = (u16)(moby->flags | 1);
            noop_main_callback();
            noop_callback_k(hero.unk1D00, hero.unk1D00 + 0x10, 0, 1);
            game_mode = 0;
        } else if (controller_state.pressed & PAD_CROSS) {
            game_mode = 0;
        }
        break;

    case 6:
        switch (freeze.unk1C) {
        case 0:
            if (freeze.unk4 != 0) {
                if ((--freeze.unk4) == 0) {
                    freeze.unk1C = 1;
                }
            } else {
                freeze.unk1C = 1;
            }
            break;

        case 1:
            if (controller_state.pressed & PAD_TRIANGLE) {
                game_mode = freeze.unk14;
            } else if (controller_state.pressed & PAD_CROSS) {
                fade_to_black(4);
                requested_video_mode = 0;
                freeze.unk1C = 2;
                freeze.unk20 = scale_game_frames(0x258);
            }
            break;

        case 2:
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

        case 3:
            if (controller_state.pressed & PAD_CROSS) {
                game_mode = freeze.unk14;
            }
            break;

        default:
            game_mode = freeze.unk14;
            break;
        }

        break;

    case 1:
        D_0015F648 = 2;
        if (controller_state.pressed & PAD_TRIANGLE) {
            game_mode = 0;
            hero.ship_flags = (u8)(hero.ship_flags | 1);
        } else if (controller_state.pressed & PAD_CROSS) {
            game_mode = 0;
        }
        break;

    case 2:
        D_0015F648 = 2;
        if (controller_state.pressed & PAD_CROSS) {
            game_mode = 0;
        }
        break;

    case 0:
        switch (freeze.unk1C) {
        case 0:
            if (freeze.unk20 < 8) {
                freeze.unk20++;
            } else if (freeze.unk24 < 8) {
                freeze.unk24++;
            } else {
                freeze.unk1C = 1;
            }
            break;

        case 1:
            if (controller_state.pressed & PAD_CROSS) {
                freeze.unk1C = 2;
            } else if (controller_state.pressed & (PAD_CIRCLE | PAD_START)) {
                freeze.unk1C = 3;
            }
            /* Fakematch: this always-true test gives case 2/3 a second predecessor. */
            video_mode = requested_video_mode;
            if (video_mode >= 0)
                break;

        case 2:
        case 3:
            if (freeze.unk24 != 0) {
                freeze.unk24--;
            } else if (freeze.unk20 != 0) {
                freeze.unk20--;
            } else if (freeze.unk1C == 2) {
                if (hero.unk880 != (-1)) {
                    set_animation_parameter(hero.unk880, 0);
                    hero.unk880 = -1;
                }
                update_resource_counter();
                if (scale_game_frames(0x1068) < hero.unk19C) {
                    if (current_level_index == 5) {
                        D_0015EE38++;
                    } else if (current_level_index == 0x10) {
                        D_0015EE3C++;
                    }
                }
                game_mode = 0;
                noop_callback_k(D_00141050, D_00141050 + 0x10, 0, 1);
            } else {
                game_mode = 0;
                if (hero.unk89A >= 3) {
                    hero.unk89A = (s16)(((u16)hero.unk89A) - 1);
                    hero.unk890[0xBC] = 3;
                }
            }
            break;
        }
        break;

    case 3:
        freeze.unk20++;
        if (scale_game_frames(0x1E) < freeze.unk20) {
            if (freeze.unk24 != 0) {
                freeze.unk24--;
            }
        }
        switch (mode_freeze_state) {
        case 2:
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

        case 16:
            game_mode = freeze.unk14;
            pause_current_action_list[0] = freeze.unk18;
            break;

        case 12:
            if (mode_freeze_flags & 2) {
                break;
            }

        case 3:

        case 5:
            if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                game_mode = freeze.unk14;
            }
            break;

        case 19:
            if ((boot_level_active != 0) && (mode_freeze_flags & 2)) {
                if (controller_state.pressed & PAD_CROSS) {
                    load_and_initialize_level_chunk();
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    mode_freeze_flags = mode_freeze_flags | 0x20;
                    initialize_global_state_entry(0);
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
                initialize_global_state_entry(0);
                D_0013E05A[0] = 1;
            } else if (controller_state.pressed & PAD_TRIANGLE) {
                mode_freeze_flags |= 0x20;
                game_mode = freeze.unk14;
            }
            break;

        case 4:
            if (boot_level_active != 0) {
                if (mode_freeze_flags & 2) {
                    struct PadState *pad = &controller_state;
                    if (pad->pressed & PAD_CROSS) {
                        load_and_initialize_level_chunk();
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        initialize_global_state_entry(0);
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

        case 9:
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
#endif /* NON_MATCHING */
