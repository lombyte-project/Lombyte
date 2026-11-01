#ifndef LOMBYTE_RNC_SDK_MPEG_DEMUX_H
#define LOMBYTE_RNC_SDK_MPEG_DEMUX_H

#include "types.h"

struct MpegPictureTimestampRecord;
struct MpegTimestampState;
struct PES_BS;
struct PES_HDR;
struct PackHeaderInfo;

s32  _PES_packet(s32, struct PES_BS *, struct PES_HDR *);
void GetMpegPictureTimestampsAndFlags(struct MpegTimestampState *,
                                      struct MpegPictureTimestampRecord *,
                                      s64 *,
                                      s64 *,
                                      s64 *) __asm__("_getPtsDtsFlags");
s32  _pack_header(s32, struct PackHeaderInfo *);
u64  _type2id(s32, s32);

#endif /* LOMBYTE_RNC_SDK_MPEG_DEMUX_H */
