/* Ported from rac1-decomp (src/core/00125630.c: func_00129690). */
#include "types.h"

struct MpegReferencePicture {
    u8 pad_0[0x18];
    s64 presentation_timestamp;
    s64 decoding_timestamp;
    s32 status;
    s32 coding_type;
    s32 picture_structure;
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

struct MpegReferenceState {
    u8 pad_0[0xA0];
    s32 queued_picture_count0;
    s32 queued_picture_count1;
    u8 pad_A8[0x40];
    s32 reorder_gate;
    u8 pad_EC[0x50];
    s32 progressive_sequence;
    u8 pad_140[0x8];
    s32 display_width;
    s32 display_height;
    s32 coding_type;
    u8 pad_154[0x20];
    s32 picture_structure;
    s32 top_field_first;
    u8 pad_17C[0x8];
    s32 repeat_first_field;
    s32 progressive_frame;
    s32 horizontal_offset0;
    s32 horizontal_offset1;
    s32 horizontal_offset2;
    s32 vertical_offset0;
    s32 vertical_offset1;
    s32 vertical_offset2;
    s32 closed_group;
    s32 broken_link;
    u8 pad_1AC[0xC];
    struct MpegReferencePicture *current_frame;
    struct MpegReferencePicture *previous_frame;
    struct MpegReferencePicture *selected_frame;
    struct MpegReferencePicture *reordered_frame;
    struct MpegReferencePicture *current_top_field;
    struct MpegReferencePicture *previous_top_field;
    struct MpegReferencePicture *selected_top_field;
    struct MpegReferencePicture *reordered_top_field;
    struct MpegReferencePicture *current_bottom_field;
    struct MpegReferencePicture *previous_bottom_field;
    struct MpegReferencePicture *selected_bottom_field;
    struct MpegReferencePicture *reordered_bottom_field;
    u8 pad_1E8[0x640];
    s64 presentation_timestamp;
    s64 decoding_timestamp;
};

s32 UpdateMpegReferenceImages(struct MpegReferenceState *state,
                              s32 force_reorder) __asm__("_updateRefImage");

s32 UpdateMpegReferenceImages(struct MpegReferenceState *state, s32 force_reorder) {
    enum {
        MPEG_PICTURE_STRUCTURE_TOP_FIELD = 1,
        MPEG_PICTURE_STRUCTURE_BOTTOM_FIELD = 2,
        MPEG_PICTURE_STRUCTURE_FRAME = 3
    };
    struct MpegReferencePicture *selected_picture;
    s32 references_ready;
    s32 queue_threshold;
    struct MpegReferencePicture *swap;
    struct MpegReferencePicture *predicted_field;

    selected_picture = NULL;
    references_ready = 0;
    queue_threshold = (state->picture_structure == 3) ? 2 : 4;
    if (state->coding_type == 3) {
        state->selected_frame = state->reordered_frame;
        state->selected_top_field = state->reordered_top_field;
        state->selected_bottom_field = state->reordered_bottom_field;
        if (state->queued_picture_count0 + state->queued_picture_count1 >= queue_threshold) {
            state->reorder_gate = 0;
            state->broken_link = 0;
            state->closed_group = 0;
        }
        if ((state->reorder_gate != 0 || state->broken_link != 0) && state->closed_group == 0) {
            state->current_frame->status = 0;
            state->current_top_field->status = 0;
            state->current_bottom_field->status = 0;
        }
        state->reorder_gate = 0;
        state->broken_link = 0;
        if (state->picture_structure == 3) {
            if (state->current_frame->status == 1 || state->closed_group != 0) {
                if (state->previous_frame->status == 1) {
                    references_ready = 1;
                }
            }
        } else {
            if ((state->current_top_field->status == 1 && state->current_bottom_field->status == 1) ||
                state->closed_group != 0) {
                if (state->previous_top_field->status == 1) {
                    if (state->previous_bottom_field->status == 1) {
                        references_ready = 1;
                    }
                }
            }
        }
    } else {
        if (force_reorder == 0) {
            swap = state->current_frame;
            state->current_frame = state->previous_frame;
            state->previous_frame = swap;
            swap = state->current_top_field;
            state->current_top_field = state->previous_top_field;
            state->previous_top_field = swap;
            swap = state->current_bottom_field;
            state->current_bottom_field = state->previous_bottom_field;
            state->previous_bottom_field = swap;
        }
        state->selected_frame = state->previous_frame;
        state->selected_top_field = state->previous_top_field;
        state->selected_bottom_field = state->previous_bottom_field;
        if (state->picture_structure == 3) {
            if (state->coding_type != 2 || state->current_frame->status == 1) {
                references_ready = 1;
            }
        } else {
            predicted_field = (state->picture_structure == 1) ? state->selected_bottom_field
                                                              : state->selected_top_field;
            if (state->coding_type != 2 || (force_reorder != 0 && predicted_field->status == 1) ||
                (state->current_top_field->status == 1 && state->current_bottom_field->status == 1)) {
                references_ready = 1;
            }
        }
    }
    switch (state->picture_structure) {
    case MPEG_PICTURE_STRUCTURE_FRAME:
        selected_picture = state->selected_frame;
        break;
    case MPEG_PICTURE_STRUCTURE_TOP_FIELD:
        selected_picture = state->selected_top_field;
        break;
    case MPEG_PICTURE_STRUCTURE_BOTTOM_FIELD:
        selected_picture = state->selected_bottom_field;
        break;
    }
    selected_picture->status = 0;
    selected_picture->presentation_timestamp = state->presentation_timestamp;
    selected_picture->coding_type = state->coding_type;
    selected_picture->decoding_timestamp = state->decoding_timestamp;
    selected_picture->picture_structure = state->picture_structure;
    selected_picture->progressive_sequence = state->progressive_sequence;
    selected_picture->progressive_frame = state->progressive_frame;
    selected_picture->top_field_first = state->top_field_first;
    selected_picture->repeat_first_field = state->repeat_first_field;
    selected_picture->horizontal_offset0 = state->horizontal_offset0;
    selected_picture->horizontal_offset1 = state->horizontal_offset1;
    selected_picture->horizontal_offset2 = state->horizontal_offset2;
    selected_picture->vertical_offset0 = state->vertical_offset0;
    selected_picture->vertical_offset1 = state->vertical_offset1;
    selected_picture->vertical_offset2 = state->vertical_offset2;
    selected_picture->display_width = state->display_width;
    selected_picture->display_height = state->display_height;
    return references_ready;
}
