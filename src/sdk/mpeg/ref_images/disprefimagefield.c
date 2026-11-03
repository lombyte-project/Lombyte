#include "types.h"
#include "rnc/sdk/mpeg/demux.h"
#include "rnc/sdk/mpeg/macroblock.h"
#include "rnc/sdk/mpeg/ref_images.h"

struct MpegPictureBuffer {
    u8  pad0[0x10];
    s32 macroblock_height;
    u8  pad14[4];
    s64 presentation_timestamp;
    s64 decoding_timestamp;
    s32 status;
    s32 coding_type;
    s32 structure;
    s32 progressive_sequence;
    s32 progressive_frame;
    s32 top_field_first;
    s32 repeat_first_field;
    s32 horizontal_offset0;
    s32 horizontal_offset1;
    s32 horizontal_offset2;
    s32 vertical_offset0;
    s32 vertical_offset1;
    s32 vertical_offset2;
    s32 display_width;
    s32 display_height;
};

union MpegTimestampSlot {
    s64 value;
    struct {
        s32 low;
        s32 high;
    } words;
};

struct MpegPictureTransfer {
    u8                      pad0[0x10];
    union MpegTimestampSlot first_timestamp;
    s64                     first_decode_timestamp;
    s64                     first_flags;
    union MpegTimestampSlot second_timestamp;
    s64                     second_decode_timestamp;
    s64                     second_flags;
};

struct MpegFieldTransferState {
    u8                          pad0[0x80];
    s32                         last_timestamp;
    u8                          pad84[4];
    s64                         timestamp_step;
    u8                          pad90[0x20];
    s32                         decoder_initialized;
    s32                         horizontal_offset0;
    s32                         horizontal_offset1;
    u8                          padbc[4];
    s32                         vertical_offset0;
    s32                         vertical_offset1;
    u8                          padc8[4];
    s32                         display_width;
    s32                         display_height;
    u8                          padd4[0xa0];
    s32                         picture_structure;
    u8                          pad178[0x6e0];
    struct MpegPictureTransfer *transfer;
};

extern void FinishMpegReferenceImage(struct MpegFieldTransferState *state) __asm__("func_00129B38");

void PrepareMpegPictureFieldTransfers(
    struct MpegFieldTransferState *state,
    struct MpegPictureBuffer      *first,
    struct MpegPictureBuffer      *second) __asm__("_dispRefImageField");

void PrepareMpegPictureFieldTransfers(struct MpegFieldTransferState *decoder,
                                      struct MpegPictureBuffer      *first_field_ref_image,

                                      struct MpegPictureBuffer *second_field_ref_image) {
    enum {
        MPEG_PICTURE_STATUS_OUTPUT_READY = 1
    };
    s64                         first_flags, second_flags;
    s32                         timestamp;
    s32                         second_timestamp;
    s32                         display_height;
    struct MpegPictureBuffer   *primary;
    struct MpegPictureBuffer   *secondary;
    struct MpegPictureTransfer *transfer;
    struct MpegPictureTransfer *second_transfer;
    struct MpegPictureTransfer *final_transfer;
    s32                         picture_structure_flag_mask;
    picture_structure_flag_mask = 0;
    if (decoder->picture_structure == 2) {
        primary = first_field_ref_image;
        secondary = second_field_ref_image;
        picture_structure_flag_mask = 0x40;
    } else {
        primary = second_field_ref_image;
        secondary = first_field_ref_image;
    }
    transfer = decoder->transfer;
    GetMpegPictureTimestampsAndFlags(decoder,
                                     primary,
                                     &transfer->first_timestamp.value,
                                     &transfer->first_decode_timestamp,
                                     &transfer->first_flags);
    second_transfer = decoder->transfer;
    timestamp = second_transfer->first_timestamp.words.low;
    decoder->timestamp_step = 1;
    decoder->last_timestamp = timestamp;
    GetMpegPictureTimestampsAndFlags(decoder,
                                     secondary,
                                     &second_transfer->second_timestamp.value,
                                     &second_transfer->second_decode_timestamp,
                                     &second_transfer->second_flags);
    final_transfer = decoder->transfer;
    second_timestamp = final_transfer->second_timestamp.words.low;
    decoder->timestamp_step = 1;
    decoder->last_timestamp = second_timestamp;
    first_flags = final_transfer->first_flags | picture_structure_flag_mask;
    decoder->display_width = primary->display_width;
    second_flags = final_transfer->second_flags;
    final_transfer->first_flags = first_flags;
    second_flags |= picture_structure_flag_mask;
    display_height = primary->display_height;
    final_transfer->second_flags = second_flags;
    decoder->display_height = display_height;
    decoder->horizontal_offset0 = primary->horizontal_offset0;
    decoder->horizontal_offset1 = secondary->horizontal_offset1;
    decoder->vertical_offset0 = primary->vertical_offset0;
    decoder->vertical_offset1 = secondary->vertical_offset1;
    if (_isOutSizeOK(decoder, first_field_ref_image) &&
        first_field_ref_image->status == MPEG_PICTURE_STATUS_OUTPUT_READY &&
        second_field_ref_image->status == MPEG_PICTURE_STATUS_OUTPUT_READY) {
        first_field_ref_image->macroblock_height *= 2;
        if (decoder->decoder_initialized)
            _csc_storeRefImage(decoder, first_field_ref_image);
        else
            _cpr8(decoder, first_field_ref_image);
        first_field_ref_image->macroblock_height >>= 1;
        FinishMpegReferenceImage(decoder);
    }
}
