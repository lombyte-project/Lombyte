#ifndef LOMBYTE_RNC_SDK_IPU_H
#define LOMBYTE_RNC_SDK_IPU_H

#include "types.h"

struct sceIpuDmaEnv;
struct MpegDecoder;

u32  SetIpuMpeg1(u32) __asm__("_ipuSetMPEG1");
s16  _ipuVdec(struct MpegDecoder *, s32);
void receiveDataFromIPU(s32, s32);
void sceIpuRestartDMA(struct sceIpuDmaEnv *);
s32  SendIpuCommand() __asm__("_sendIpuCommand");
void suspend_ipu_dma_state(u32 *) __asm__("FUN_0012cc20");
s32  _waitBdecOut();
void _waitIpuIdle(s32 *);
s64  _waitIpuIdle64(s32 *);

#endif /* LOMBYTE_RNC_SDK_IPU_H */
