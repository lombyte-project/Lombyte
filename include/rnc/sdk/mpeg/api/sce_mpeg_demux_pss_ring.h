#ifndef LOMBYTE_RNC_SDK_MPEG_API_SCE_MPEG_DEMUX_PSS_RING_H
#define LOMBYTE_RNC_SDK_MPEG_API_SCE_MPEG_DEMUX_PSS_RING_H

#include "types.h"

struct ReadBuf;
struct VideoDec;

s32 sceMpegDemuxPssRing(struct VideoDec *, u8 *, s32, struct ReadBuf *, s32);

#endif /* LOMBYTE_RNC_SDK_MPEG_API_SCE_MPEG_DEMUX_PSS_RING_H */
