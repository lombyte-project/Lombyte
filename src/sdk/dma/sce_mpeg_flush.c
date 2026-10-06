#include "types.h"

typedef struct MpegDecoder MpegDecoder;
typedef struct MpegObject {
    u8 reserved[0x8];
    s32 state;
    u8 middle_reserved[0x34];
    MpegDecoder *decoder;
} MpegObject;

struct MpegDecoder {
    u8 reserved[0x4];
    s32 frame_ready;
    s32 frame_active;
    u8 middle_reserved[0xA0];
    s32 frame_base;
    u8 tail_reserved[0x68];
    s32 frame_count;
};

extern void FinishMpegFrame(MpegDecoder *decoder) __asm__("_lastFrame");

s32 _sceMpegFlush(MpegObject *mpeg) __asm__("_sceMpegFlush");

s32 _sceMpegFlush(MpegObject *mpeg) {
    MpegDecoder *decoder = mpeg->decoder;
    s32 result = 0;

    if (decoder->frame_ready != 0) {
        if (decoder->frame_active != 0) {
            FinishMpegFrame(decoder);
            mpeg->state = decoder->frame_count - decoder->frame_base;
            decoder->frame_ready = 0;
            result = 1;
        }
    }
    return result;
}
