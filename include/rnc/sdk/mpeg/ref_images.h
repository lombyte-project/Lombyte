#ifndef LOMBYTE_RNC_SDK_MPEG_REF_IMAGES_H
#define LOMBYTE_RNC_SDK_MPEG_REF_IMAGES_H

#include "types.h"

struct MpegDecoder;
struct MpegRefImage;
struct MpegReferenceState;

void _getRef0(
    struct MpegDecoder *, struct MpegRefImage *, int, int, int, int, int, int, int, int, int, int);
s32  UpdateMpegReferenceImages(struct MpegReferenceState *, s32) __asm__("_updateRefImage");
s32  _csc_storeRefImage();
void _initRefImages(unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    unsigned int *,
                    void *,
                    void *,
                    void *,
                    int,
                    int);
s32  InitializeReferenceImage();
s32  _isOutSizeOK(struct MpegDecoder *, struct MpegRefImage *);
void _outputFrame(struct MpegDecoder *, s32, s32);

#endif /* LOMBYTE_RNC_SDK_MPEG_REF_IMAGES_H */
