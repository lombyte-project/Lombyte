#include "types.h"
#include "rnc/sdk/libmpeg.h"

int ClearMpegReferenceBuffer(struct sceMpeg *mp) __asm__("ClearMpegReferenceBuffer");

/* Zero status of reference images 0, 4, 8, 1, 5, 9. */
int ClearMpegReferenceBuffer(struct sceMpeg *mp) {
    struct MpegDecoder *decoder = mp->sys;
    struct MpegRefImage *buffer;

    buffer = decoder->ref_images[0];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = decoder->ref_images[4];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = decoder->ref_images[8];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = decoder->ref_images[1];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = decoder->ref_images[5];
    if (buffer != 0) {
        buffer->status = 0;
    }
    buffer = decoder->ref_images[9];
    if (buffer != 0) {
        buffer->status = 0;
    }
    return 1;
}
