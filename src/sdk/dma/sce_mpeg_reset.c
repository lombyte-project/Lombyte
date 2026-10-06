#include "types.h"

typedef struct MpegDecoder MpegDecoder;
typedef struct MpegObject {
    u8 reserved[0x8];
    s32 state;
    u8 middle_reserved[0x34];
    MpegDecoder *decoder;
} MpegObject;

struct MpegDecoder {
    s32 status;
    s32 input_count;
    s32 output_count;
    u8 reserved[0x74];
    s32 error_code;
    u8 middle_reserved[0x28];
    s32 stream_state;
    u8 tail_reserved[0x68];
    s32 image_state;
};

extern void ClearDecoderState(MpegDecoder *decoder) __asm__("_clearEach");
extern void SetImageBufferFlag(MpegDecoder *decoder) __asm__("SetImageBufferFlag");

void sceMpegReset(MpegObject *mpeg) __asm__("sceMpegReset");

void sceMpegReset(MpegObject *mpeg) {
    MpegDecoder *decoder = mpeg->decoder;

    decoder->status = 0;
    decoder->input_count = 0;
    decoder->output_count = 0;
    mpeg->state = 0;
    decoder->stream_state = 0;
    decoder->error_code = -1;
    ClearDecoderState(decoder);
    decoder->image_state = 0;
    SetImageBufferFlag(decoder);
}
