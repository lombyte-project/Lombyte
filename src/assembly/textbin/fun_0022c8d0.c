#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022c8d0/FUN_0022c8d0.s", FUN_0022c8d0);
#else
#include "types.h"
#include "qzero.h"
struct VoicePoolInitializationState {
    u8 pad_0[0x40];
    s32 listener_history_position;
    s32 reserved44;
    s32 group_0_volume;
    s32 group_1_volume;
    s32 group_2_volume;
    s32 group_3_volume;
    s32 group_4_volume;
    s32 group_5_volume;
    u8 pad_60[0x10];
    s32 handle;
};

struct VoicePoolInitializationWindow {
    u8 pad_0[0x70];
    s32 handle;
    u8 state;  /* Only the byte at offset 0x74 is cleared. */
};

extern struct VoicePoolInitializationState voice_pool __asm__("D_0013E550");
extern s32 playback_mode __asm__("D_0015EDE8");
extern s32 music_volume __asm__("D_0015EDEC");
extern s32 sound_volume __asm__("D_0015EDF0");
extern s32 snd_start_sound_system(void) __asm__("FUN_0012da28");
extern void snd_set_master_volume(s32, s32) __asm__("FUN_0012e208");
extern void snd_set_playback_mode(s32) __asm__("FUN_0012e240");
extern void snd_set_mixer_mode(s32, s32) __asm__("FUN_0012e280");
extern void snd_set_group_voice_range(s32, s32, s32) __asm__("FUN_0012e2b8");
extern void snd_pre_alloc_reverb_work_area(s32, s32) __asm__("FUN_0012efa8");
extern void reset_music(void) __asm__("FUN_00215390");
void initialize_gameplay_sound_system(void) __asm__("FUN_0022c8d0");

void initialize_gameplay_sound_system(void) {
    u8 *slot_end;
    u8 *header_block;
    struct VoicePoolInitializationWindow *slot_window;
    s32 scaled_volume_80;
    s32 scaled_volume_70;
    s32 header_block_index;

    header_block_index = 3;
    header_block = (u8 *)&voice_pool;
    do {
        qzero(header_block);
        header_block_index -= 1;
        header_block += 16;
    } while (header_block_index >= 0);
    slot_window = (struct VoicePoolInitializationWindow *)&voice_pool;
    voice_pool.listener_history_position = 0;
    voice_pool.reserved44 = 0;
    slot_end = ((u8 *)&voice_pool + 0xD20);
    voice_pool.handle = 0;
loop_3:
    slot_window->state = 0;
    slot_window = (struct VoicePoolInitializationWindow *)((u8 *)slot_window + 0x70);
    if ((s32)slot_window < (s32)slot_end) {
        slot_window->handle = 0;
        goto loop_3;
    }
    snd_start_sound_system();
    snd_set_playback_mode(playback_mode == 0);
    snd_set_mixer_mode(0, 1);
    snd_pre_alloc_reverb_work_area(2, 4);
    snd_set_group_voice_range(1, 0x18, 0x2F);
    snd_set_group_voice_range(2, 0x18, 0x2F);
    snd_set_group_voice_range(4, 0x18, 0x2F);
    scaled_volume_80 = (s32) (*(s32 *)0x15EDF0 * 8) / 10;
    voice_pool.group_1_volume = (s32) *(s32 *)0x15EDEC;
    scaled_volume_70 = (s32) (*(s32 *)0x15EDF0 * 7) / 10;
    voice_pool.group_2_volume = scaled_volume_80;
    voice_pool.group_0_volume = scaled_volume_80;
    voice_pool.group_4_volume = scaled_volume_70;
    voice_pool.group_3_volume = scaled_volume_70;
    voice_pool.group_5_volume = (s32) *(s32 *)0x15EDF0;
    reset_music();
    snd_set_master_volume(0, voice_pool.group_0_volume);
    snd_set_master_volume(1, voice_pool.group_1_volume);
    snd_set_master_volume(2, voice_pool.group_2_volume);
    snd_set_master_volume(3, voice_pool.group_3_volume);
    snd_set_master_volume(4, voice_pool.group_4_volume);
    snd_set_master_volume(5, voice_pool.group_5_volume);
}
#endif /* NON_MATCHING */
