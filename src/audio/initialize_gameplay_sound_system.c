/* Ported from rac1-decomp (src/game/sound.c, func_0022DBE8). */
#include "types.h"
#include "eetypes.h"
#include "qzero.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/options.h"

/* The 0x70-byte header in front of the voices, then thirty 0x70-byte voices. */
typedef struct {
    u128 clear[4];
    s32 listener_history_position;
    s32 reserved44;
    s32 group_volume[6];
    u8 pad60[0x10];
} VoicePoolHeader;

typedef struct {
    s32 handle;
    u8 state;
    u8 pad5[0x6B];
} VoicePoolSlot;

extern u8 voice_pool[] __asm__("D_0013E550");
extern s32 snd_start_sound_system(void) __asm__("FUN_0012da28");
extern void snd_set_master_volume(s32, s32) __asm__("FUN_0012e208");
extern void snd_set_playback_mode(s32) __asm__("FUN_0012e240");
extern void snd_set_mixer_mode(s32, s32) __asm__("FUN_0012e280");
extern void snd_set_group_voice_range(s32, s32, s32) __asm__("FUN_0012e2b8");
extern void snd_pre_alloc_reverb_work_area(s32, s32) __asm__("FUN_0012efa8");
extern void reset_music(void) __asm__("FUN_00215390");
void initialize_gameplay_sound_system(void) __asm__("FUN_0022c8d0");

#define POOL ((VoicePoolHeader *)voice_pool)

void initialize_gameplay_sound_system(void) {
    s32 i;
    s32 slot;
    s32 end;
    s32 volume;

    for (i = 0; i < 4; i++) {
        qzero(&POOL->clear[i]);
    }
    POOL->listener_history_position = 0;
    POOL->reserved44 = 0;
    slot = (s32)voice_pool;
    end = slot + 0xD20;
    do {
        ((VoicePoolSlot *)slot)[1].handle = 0;
        ((VoicePoolSlot *)slot)[1].state = 0;
        slot += 0x70;
    } while (slot < end);
    snd_start_sound_system();
    snd_set_playback_mode(playback_mode == 0);
    snd_set_mixer_mode(0, 1);
    snd_pre_alloc_reverb_work_area(2, 4);
    snd_set_group_voice_range(1, 0x18, 0x2F);
    snd_set_group_voice_range(2, 0x18, 0x2F);
    snd_set_group_voice_range(4, 0x18, 0x2F);
    volume = sound_volume;
    POOL->group_volume[0] = (volume * 8) / 10;
    POOL->group_volume[1] = music_volume;
    POOL->group_volume[2] = (volume * 8) / 10;
    POOL->group_volume[3] = (volume * 7) / 10;
    POOL->group_volume[4] = (volume * 7) / 10;
    POOL->group_volume[5] = volume;
    reset_music();
    snd_set_master_volume(0, POOL->group_volume[0]);
    snd_set_master_volume(1, POOL->group_volume[1]);
    snd_set_master_volume(2, POOL->group_volume[2]);
    snd_set_master_volume(3, POOL->group_volume[3]);
    snd_set_master_volume(4, POOL->group_volume[4]);
    snd_set_master_volume(5, POOL->group_volume[5]);
}

extern __typeof__(initialize_gameplay_sound_system) func_0022C8D0 __attribute__((alias("FUN_0022c8d0")));
