#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/audio/sound_state.h"

extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
void       snd_post_message(void) __asm__("FUN_0012e9a0");

void snd_post_message(void) {
    s32 *batch_count;
    s32  active_batch_buffer_index;
    u8  *command_buffer_pointer_table;

    active_batch_buffer_index = snd_batch_index;
    command_buffer_pointer_table = (u8 *)&snd_batch_command_buffers;
    batch_count = *(s32 **)(command_buffer_pointer_table + (active_batch_buffer_index * 4));
    *batch_count += 1;
    snd_flush_sound_commands(batch_count);
}

extern __typeof__(snd_post_message) func_0012E9A0 __attribute__((alias("FUN_0012e9a0")));

/* Recovered original symbol name. */
extern __typeof__(snd_post_message) snd_PostMessage __attribute__((alias("FUN_0012e9a0")));
