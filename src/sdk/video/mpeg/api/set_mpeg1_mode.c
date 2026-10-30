#include "types.h"

typedef struct ImageBufferState {
    u8  reserved[0x848];
    s32 pending_flag;
} ImageBufferState;

extern void SetIpuMpeg1(s32 enabled) __asm__("_ipuSetMPEG1");

void set_mpeg1_mode(ImageBufferState *image_state) __asm__("SetImageBufferFlag");

void set_mpeg1_mode(ImageBufferState *image_state) {
    image_state->pending_flag = 0;
    SetIpuMpeg1(1);
}
