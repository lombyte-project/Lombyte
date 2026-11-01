#ifndef LOMBYTE_RNC_SDK_MPEG_API_H
#define LOMBYTE_RNC_SDK_MPEG_API_H

#include "types.h"

struct MpegContext;
struct MpegEvent;
struct MpegWorkArea;
struct ReadBuf;
struct VideoDec;
struct sceMpeg;

s32  _dispatchMpegCallback(struct MpegContext *, struct MpegEvent *);
void DispatchMpegCallbackNoData(void *) __asm__("_dispatchMpegCbNodata");
int  reset_work_area(struct MpegWorkArea *) __asm__("func_0012BC10");
s32  sceMpegDemuxPssRing(struct VideoDec *, u8 *, s32, struct ReadBuf *, s32);
s32  _sceMpegFlush(struct sceMpeg *);
int  sceMpegGetPicture(struct sceMpeg *, void *, int);
void sceMpegInit(void);
s32  sceMpegIsEnd(struct sceMpeg *) __asm__("func_0012BA48");
s32  sceMpegIsRefBuffEmpty(struct sceMpeg *) __asm__("func_0012BA58");
void sceMpegReset(struct sceMpeg *);

#endif /* LOMBYTE_RNC_SDK_MPEG_API_H */
