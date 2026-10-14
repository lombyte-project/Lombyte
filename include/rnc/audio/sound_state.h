#ifndef LOMBYTE_RNC_AUDIO_SOUND_STATE_H
#define LOMBYTE_RNC_AUDIO_SOUND_STATE_H

#include "types.h"

extern char voice_positions[] __asm__("D_0013E5E0");
extern u8 sound_command_client[] __asm__("D_0015EBC0");
extern s32 snd_cd_stream_active __asm__("D_0015EC8C") __attribute__((sda));
extern s32 snd_cd_status_callback __asm__("D_0015EC90") __attribute__((sda));
extern s32 snd_cd_sync_pending __asm__("D_0015EC94") __attribute__((sda));
extern s32 snd_cd_stream_end_pending __asm__("D_0015EC98") __attribute__((sda));
extern s32 snd_batch_command_buffers __asm__("D_0015ECA0");
extern s32 snd_batch_index __asm__("D_0015ECC0");
extern s32 snd_cd_callback_pending __asm__("D_0015ECC8") __attribute__((sda));
extern int sound_bank_handle __asm__("D_0015ED5C");
extern s32 level_sound_def_count __asm__("D_0015F630");
extern u8 *level_sound_defs __asm__("D_0015F634");

#endif /* LOMBYTE_RNC_AUDIO_SOUND_STATE_H */
