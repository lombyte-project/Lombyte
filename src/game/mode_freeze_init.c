#include "types.h"
#include "rnc/game/freeze_mode_state.h"
#include "rnc/globals.h"
#include "rnc/audio/voices/snd_pause_all_sounds_in_group.h"

extern s32 music_pause();
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 push_help_history() __asm__("func_001FED30");

void mode_freeze_init(u32 mode, s32 freeze_context) __asm__("FUN_001fbab8");

void mode_freeze_init(u32 mode, s32 freeze_context) {
    enum {
        FREEZE_CONTROL_MODE_16_PAUSE = 0,
        FREEZE_HERO_STATE_32_PAUSE = 1,
        FREEZE_ERROR = 3,
        FREEZE_VEHICLE_PAUSE = 4,
        FREEZE_PUSH_HELP_HISTORY = 5,
        FREEZE_VIDEO_MODE_CHANGE = 6
    };
    FreezeModeState *freeze_state;

    if (game_mode != 3) {
        snd_pause_all_sounds_in_group(0x1D);
        music_pause(0);
    }
    freeze.return_game_mode = game_mode;
    freeze.context = freeze_context;
    game_mode = 4;
    freeze.state = mode;
    switch (mode) {
    case FREEZE_CONTROL_MODE_16_PAUSE:
        freeze_state = &freeze;
        freeze_state->title = (char *)get_help_message_text(0x4F6E);
        freeze_state->line1 = (char *)get_help_message_text(0x5248);
        freeze_state->line2 = (char *)get_help_message_text(0x5249);
        freeze_state->timer = 0;
        freeze_state->choice = 0;
        freeze_state->unk20 = 0;
        freeze_state->unk24 = 0;
        freeze_state->unk28 = 0;
        break;
    case 2:
        freeze.title = (char *)get_help_message_text(0x524A);
        freeze.line1 = 0;
        freeze.timer = 0;
        break;
    case FREEZE_HERO_STATE_32_PAUSE:
    case FREEZE_VEHICLE_PAUSE:
        freeze_state = &freeze;
        freeze_state->title = (char *)get_help_message_text(0x5229);
        freeze_state->line1 = (char *)get_help_message_text(0x4EE0);
        freeze_state->line2 = (char *)get_help_message_text(0x524A);
        freeze_state->timer = 0;
        break;
    case FREEZE_PUSH_HELP_HISTORY:
        push_help_history(0x4E2B);
        freeze.timer = scale_game_frames(0x1E);
        freeze.unk20 = 0;
        freeze.unk24 = scale_game_frames(0x1E);
        break;
    case FREEZE_ERROR:
        freeze.timer = scale_game_frames(0x1E);
        freeze.unk20 = 0;
        freeze.unk24 = scale_game_frames(0x1E);
        break;
    case FREEZE_VIDEO_MODE_CHANGE:
        freeze.timer = scale_game_frames(0x1E);
        freeze.choice = 0;
        break;
    default:
        freeze.title = 0;
        freeze.line1 = 0;
        freeze.line2 = 0;
        freeze.timer = 0x78;
        break;
    }
}

extern __typeof__(mode_freeze_init) func_001FBAB8 __attribute__((alias("FUN_001fbab8")));

FreezeModeState freeze = {0};
