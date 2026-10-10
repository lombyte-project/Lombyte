#include "types.h"

typedef struct ImageBufferState {
    u8 reserved[0x848];
    s32 pending_flag;
} ImageBufferState;

extern void set_image_state_flag(s32 enabled) __asm__("_ipuSetMPEG1");

void clear_image_buffer_flag(ImageBufferState *image_state) __asm__("SetImageBufferFlag");

void clear_image_buffer_flag(ImageBufferState *image_state) {
    image_state->pending_flag = 0;
    set_image_state_flag(1);
}
