#include "types.h"

extern s32 D_001611E8;
extern u8 *D_0016120C;
extern s32 vo_buf_dec_count() __asm__("func_0023D340");

s32 handler_end_image(void) __asm__("FUN_0023b540");

s32 handler_end_image(void) {
    if (D_001611E8 != 0) {
        vo_buf_dec_count(D_0016120C + 0xD9168);
        *(s32 *)0x1611E8 = 0;
    }
    __asm__ __volatile__("sync\nei");
    return 0;
}
