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

void mode_freeze_init(u32 mode, s32 arg1) {
    struct FreezeModeState *st;

    if (game_mode != 3) {
        snd_pause_all_sounds_in_group(0x1D);
        music_pause(0);
    }
    freeze.unk14 = game_mode;
    freeze.unk18 = arg1;
    game_mode = 4;
    freeze.unk0 = mode;
    switch (mode) {
    case 0:
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
    case 1:
    case 4:
        st = &freeze;
        st->unk8 = (s32)get_help_message_text(0x5229);
        st->unkC = (s32)get_help_message_text(0x4EE0);
        st->unk10 = (s32)get_help_message_text(0x524A);
        st->unk4 = 0;
        break;
    case 5:
        push_help_history(0x4E2B);
        freeze.unk4 = scale_game_frames(0x1E);
        freeze.unk20 = 0;
        freeze.unk24 = scale_game_frames(0x1E);
        break;
    case 3:
        freeze.unk4 = scale_game_frames(0x1E);
        freeze.unk20 = 0;
        freeze.unk24 = scale_game_frames(0x1E);
        break;
    case 6:
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
