#include "types.h"
#include "rnc/sdk/ipu/ipu_set_mpeg1.h"

typedef struct ImageBufferState {
    u8  reserved[0x848];
    s32 pending_flag;
} ImageBufferState;

void set_mpeg1_mode(ImageBufferState *image_state) __asm__("SetImageBufferFlag");

void set_mpeg1_mode(ImageBufferState *image_state) {
    image_state->pending_flag = 0;
    SetIpuMpeg1(1);
}
