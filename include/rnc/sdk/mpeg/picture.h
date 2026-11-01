#ifndef LOMBYTE_RNC_SDK_MPEG_PICTURE_H
#define LOMBYTE_RNC_SDK_MPEG_PICTURE_H

#include "types.h"

struct MpegDecoder;
struct MpegDecoderState;
struct sceMpeg;

void _clearEach(u8 *);
s32  _decodeOrSkipField(struct sceMpeg *, s32, s32);
s32  _decodeOrSkipFrame(struct sceMpeg *, s32, s32);
s32  _getpic(struct sceMpeg *);
void _lastFrame(struct MpegDecoder *);
s32  _pictureData0();
s32  _decPicture(struct MpegDecoderState *);
void UpdateTempTrackData(struct MpegDecoder *, s32);

#endif /* LOMBYTE_RNC_SDK_MPEG_PICTURE_H */
