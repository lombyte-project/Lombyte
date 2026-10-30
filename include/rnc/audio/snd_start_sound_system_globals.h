#ifndef LOMBYTE_RNC_AUDIO_SND_START_SOUND_SYSTEM_GLOBALS_H
#define LOMBYTE_RNC_AUDIO_SND_START_SOUND_SYSTEM_GLOBALS_H

#include "types.h"

/* The two sound command batches: a command count, then the commands.
 * snd_start_sound_system leaves 0xFFC bytes free behind the count. */
extern s32 snd_command_buffer1[0x400] __asm__("D_00133280");
extern s32 snd_command_buffer2[0x400] __asm__("D_00134280");

/* A batch holds at most this many commands: snd_send_iop_command_no_wait
 * flushes when a batch's count reaches it. */
#define SND_BATCH_MAX_COMMANDS 0x100

/* The callback queued with one command of a batch (snd_send_iop_command_no_wait
 * fills it; snd_flush_sound_commands calls func with the command's reply word
 * and data once the batch completes). */
struct SndCommandCallback {
    s32 func;
    s32 pad4;
    s64 data;
};

/* One callback per command of each batch. */
extern struct SndCommandCallback snd_stream_buffer1[SND_BATCH_MAX_COMMANDS] __asm__("D_00135280");
extern struct SndCommandCallback snd_stream_buffer2[SND_BATCH_MAX_COMMANDS] __asm__("D_00136280");

/* The IOP's replies to a batch: a status word, one word per command and a
 * terminator (snd_send_current_batch receives (count << 2) + 8 bytes). */
extern s32 snd_return_buffer1[SND_BATCH_MAX_COMMANDS + 2] __asm__("D_00137280");
extern s32 snd_return_buffer2[SND_BATCH_MAX_COMMANDS + 2] __asm__("D_001376C0");

#endif /* LOMBYTE_RNC_AUDIO_SND_START_SOUND_SYSTEM_GLOBALS_H */
