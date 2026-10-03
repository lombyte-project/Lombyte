#include "rnc/preview_animation.h"

extern s16 cd_read_active[] __asm__("D_001516D8");
extern PreviewAnimationStreamState preview_stream_state __asm__("D_001D5BF0");
extern PreviewAnimationRequest active_preview_animation __asm__("D_001D6080");
extern s32 request_audio_stream_break() __asm__("FUN_002166e8");

s32 clear_preview_animation_queue(void) __asm__("FUN_00226718");

s32 clear_preview_animation_queue(void)
{
    if (cd_read_active[0] != 0) {
        if (preview_stream_state.pending_buffer != 0) {
            request_audio_stream_break();
            preview_stream_state.pending_buffer = 0;
        }
    }
    active_preview_animation.delay_frames = 0;
    *(u32 *)0x00160350 = 0;
    return 0;
}

extern __typeof__(clear_preview_animation_queue) func_00226718 __attribute__((alias("FUN_00226718")));
