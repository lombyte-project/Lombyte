#include "types.h"
#include "rnc/globals.h"

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

extern struct FreezeModeState freeze __asm__("D_00193300");
extern void snd_pause_all_sounds_in_group(s32) __asm__("FUN_0012e3e8");
extern s32 music_pause();
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 push_help_history() __asm__("func_001FED30");

void mode_freeze_init(u32 mode, s32 arg1) __asm__("FUN_001fbab8");

void mode_freeze_init(u32 mode, s32 freeze_context) {
    enum {
        FREEZE_CONTROL_MODE_16_PAUSE = 0,
        FREEZE_HERO_STATE_32_PAUSE = 1,
        FREEZE_ERROR = 3,
        FREEZE_VEHICLE_PAUSE = 4,
        FREEZE_PUSH_HELP_HISTORY = 5,
        FREEZE_VIDEO_MODE_CHANGE = 6
    };
    struct FreezeModeState *st;

    if (game_mode != 3) {
        snd_pause_all_sounds_in_group(0x1D);
        music_pause(0);
    }
    freeze.unk14 = game_mode;
    freeze.unk18 = freeze_context;
    game_mode = 4;
    freeze.unk0 = mode;
    switch (mode) {
    case FREEZE_CONTROL_MODE_16_PAUSE:
        st = &freeze;
        st->unk8 = (s32)get_help_message_text(0x4F6E);
        st->unkC = (s32)get_help_message_text(0x5248);
        st->unk10 = (s32)get_help_message_text(0x5249);
        st->unk4 = 0;
        st->unk1C = 0;
        st->unk20 = 0;
        st->unk24 = 0;
        st->unk28 = 0;
        break;
    case 2:
        freeze.unk8 = (s32)get_help_message_text(0x524A);
        freeze.unkC = 0;
        freeze.unk4 = 0;
        break;
    case FREEZE_HERO_STATE_32_PAUSE:
    case FREEZE_VEHICLE_PAUSE:
        st = &freeze;
        st->unk8 = (s32)get_help_message_text(0x5229);
        st->unkC = (s32)get_help_message_text(0x4EE0);
        st->unk10 = (s32)get_help_message_text(0x524A);
        st->unk4 = 0;
        break;
    case FREEZE_PUSH_HELP_HISTORY:
        push_help_history(0x4E2B);
        freeze.unk4 = scale_game_frames(0x1E);
        freeze.unk20 = 0;
        freeze.unk24 = scale_game_frames(0x1E);
        break;
    case FREEZE_ERROR:
        freeze.unk4 = scale_game_frames(0x1E);
        freeze.unk20 = 0;
        freeze.unk24 = scale_game_frames(0x1E);
        break;
    case FREEZE_VIDEO_MODE_CHANGE:
        freeze.unk4 = scale_game_frames(0x1E);
        freeze.unk1C = 0;
        break;
    default:
        freeze.unk8 = 0;
        freeze.unkC = 0;
        freeze.unk10 = 0;
        freeze.unk4 = 0x78;
        break;
    }
}

extern __typeof__(mode_freeze_init) func_001FBAB8 __attribute__((alias("FUN_001fbab8")));
