#include "types.h"
#include "rnc/sdk/libmpeg.h"

int ClearMpegReferenceBuffer(struct sceMpeg *mp) __asm__("ClearMpegReferenceBuffer");

/* Zero status of reference images 0, 4, 8, 1, 5, 9. */
int ClearMpegReferenceBuffer(struct sceMpeg *mp) {
    struct MpegDecoder  *decoder = mp->sys;
    struct MpegRefImage *reference_image;

    reference_image = decoder->ref_images[0];
    if (reference_image != 0) {
        reference_image->status = 0;
    }
    reference_image = decoder->ref_images[4];
    if (reference_image != 0) {
        reference_image->status = 0;
    }
    reference_image = decoder->ref_images[8];
    if (reference_image != 0) {
        reference_image->status = 0;
    }
    reference_image = decoder->ref_images[1];
    if (reference_image != 0) {
        reference_image->status = 0;
    }
    reference_image = decoder->ref_images[5];
    if (reference_image != 0) {
        reference_image->status = 0;
    }
    reference_image = decoder->ref_images[9];
    if (reference_image != 0) {
        reference_image->status = 0;
    }
    return 1;
}
