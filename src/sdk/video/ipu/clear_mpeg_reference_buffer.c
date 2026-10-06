#include "types.h"

struct MpegReferenceBuffer {
    u8 padding[0x28];
    s32 status;
};

struct MpegReferenceTable {
    u8 padding[0x1B8];
    struct MpegReferenceBuffer *buffers[10];
};

struct MpegDecoder {
    u8 padding[0x40];
    struct MpegReferenceTable *reference_table;
};

int ClearMpegReferenceBuffer(struct MpegDecoder *decoder) __asm__("ClearMpegReferenceBuffer");

int ClearMpegReferenceBuffer(struct MpegDecoder *decoder) {
    struct MpegReferenceTable *reference_table = decoder->reference_table;
    struct MpegReferenceBuffer *buffer;

    buffer = reference_table->buffers[0];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = reference_table->buffers[4];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = reference_table->buffers[8];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = reference_table->buffers[1];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = reference_table->buffers[5];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = reference_table->buffers[9];
    if (buffer != 0) {
        buffer->status = 0;
    }
    return 1;
}
