#ifndef LOMBYTE_RNC_OVERLAY_INTERPOLATED_FRAME_PACKET_H
#define LOMBYTE_RNC_OVERLAY_INTERPOLATED_FRAME_PACKET_H

#include "types.h"

/* The packet build_interpolated_dma_packet (FUN_L02_002a40f0) builds at the
 * draw parameters' dst and sends as one DMA ref of 0x401 quadwords
 * (0x30000401): a tag quadword it writes first (0x0800000000000400, 0), then
 * the 0x400 quadwords FUN_L02_0024fca0 interpolates between two animation
 * frames. */
typedef struct {
    u64  tag[2];
    u128 interpolated[0x400];
} InterpolatedFramePacket;

#endif /* LOMBYTE_RNC_OVERLAY_INTERPOLATED_FRAME_PACKET_H */
