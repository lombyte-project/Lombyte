#include "types.h"

typedef struct ImageBufferState {
    u8 reserved[0x848];
    s32 pending_flag;
} ImageBufferState;

extern void SetImageStateFlag(s32 enabled) __asm__("_ipuSetMPEG1");

void ClearImageBufferFlag(ImageBufferState *image_state) __asm__("SetImageBufferFlag");

void ClearImageBufferFlag(ImageBufferState *image_state) {
    image_state->pending_flag = 0;
    SetImageStateFlag(1);
}
