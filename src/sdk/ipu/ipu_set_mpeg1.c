#include "types.h"

u32 SetIpuMpeg1(u32 scale) __asm__("_ipuSetMPEG1");

u32 SetIpuMpeg1(u32 scale) {
    u32 *ipu_control = (u32 *)0x10002010;
    return *ipu_control = (*ipu_control & 0xFF7FFFFFu) | (scale << 23);
}
