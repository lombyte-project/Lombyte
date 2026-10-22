#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 signal_semaphore() __asm__("FUN_00118990");
extern s32 wait_semaphore() __asm__("FUN_001189b0");
void vi_buf_end_put(struct ViBuf *buf, s32 bytes) __asm__("FUN_0023bf18");

void vi_buf_end_put(struct ViBuf *video_buffer_state, s32 bytes) {
    wait_semaphore(video_buffer_state->sema);
    video_buffer_state->read_bytes = (s32)(video_buffer_state->read_bytes + bytes);
    video_buffer_state->total_bytes = (s64)(bytes + video_buffer_state->total_bytes);
    signal_semaphore(video_buffer_state->sema);
}

extern void func_0023BF18(struct ViBuf *buf, s32 bytes) __attribute__((alias("FUN_0023bf18")));
