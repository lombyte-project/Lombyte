#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _updateRefImage; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_updateRefImage/_updateRefImage.s", _updateRefImage);
#else
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
    struct MpegReferencePicture * current_frame;
    struct MpegReferencePicture * previous_frame;
    struct MpegReferencePicture * selected_frame;
    struct MpegReferencePicture *reordered_frame;
    struct MpegReferencePicture * current_top_field;
    struct MpegReferencePicture * previous_top_field;
    struct MpegReferencePicture * selected_top_field;
    struct MpegReferencePicture *reordered_top_field;
    struct MpegReferencePicture * current_bottom_field;
    struct MpegReferencePicture * previous_bottom_field;
    struct MpegReferencePicture * selected_bottom_field;
    struct MpegReferencePicture *reordered_bottom_field;
    u8 pad_1E8[0x640];
    s64 presentation_timestamp;
    s64 decoding_timestamp;
};


s32 UpdateMpegReferenceImages(struct MpegReferenceState *state, s32 force_reorder) __asm__("_updateRefImage");

s32 UpdateMpegReferenceImages(struct MpegReferenceState *state, s32 force_reorder) {
    s32 coding_type;
    s32 top_field_status;
    s32 picture_structure;
    s32 queue_threshold;
    s32 references_ready;
    s32 reference_status;
    struct MpegReferencePicture *previous_bottom_field;
    struct MpegReferencePicture *current_reference;
    struct MpegReferencePicture *previous_frame;
    struct MpegReferencePicture *previous_top_field;
    struct MpegReferencePicture *primary_reference;
    struct MpegReferencePicture *secondary_reference;
    struct MpegReferencePicture *selected_picture;

    picture_structure = state->picture_structure;
    queue_threshold = ((picture_structure ^ 3) == 0) ? 2 : 4;
    coding_type = state->coding_type;
    selected_picture = NULL;
    references_ready = 0;
    if (coding_type != 3) {
        goto swap_references;
    }
    state->selected_frame = state->reordered_frame;
    state->selected_top_field = state->reordered_top_field;
    state->selected_bottom_field = state->reordered_bottom_field;
    if ((state->queued_picture_count0 + state->queued_picture_count1) < queue_threshold) {
        goto check_reorder_gate;
    }
    state->reorder_gate = 0;
    state->broken_link = 0;
    state->closed_group = 0;
check_reorder_gate:
    if (state->reorder_gate != 0) {
        goto check_closed_group;
    }
    if (state->broken_link == 0) {
        goto clear_reorder_gate;
    }
check_closed_group:
    if (state->closed_group != 0) {
        goto clear_reorder_gate;
    }
    state->current_frame->status = 0;
    state->current_top_field->status = 0;
    state->current_bottom_field->status = 0;
clear_reorder_gate:
    state->reorder_gate = 0;
    state->broken_link = 0;
    if (state->picture_structure != 3) {
        goto check_current_fields;
    }
    reference_status = 1;
    if (state->current_frame->status != 1) {
        goto check_frame_gate;
    }
    secondary_reference = state->previous_frame;
    goto check_secondary_reference;
check_frame_gate:
    if (state->closed_group == 0) {
        goto select_picture;
    }
    secondary_reference = state->previous_frame;
    goto check_secondary_reference;
check_current_fields:
    top_field_status = state->current_top_field->status;
    if (top_field_status != 1) {
        goto check_field_gate;
    }
    if (state->current_bottom_field->status == top_field_status) {
        goto check_previous_fields;
    }
check_field_gate:
    if (state->closed_group == 0) {
        goto select_picture;
    }
check_previous_fields:
    reference_status = state->previous_top_field->status;
    if (reference_status != 1) {
        goto select_picture;
    }
    secondary_reference = state->previous_bottom_field;
check_secondary_reference:
    references_ready = reference_status;
    if (secondary_reference->status != 1) references_ready = 0;
    goto select_picture_after_reference_check;
swap_references:
    if (force_reorder != 0) {
        goto select_previous_references;
    }
    current_reference = state->current_frame;
    previous_frame = state->previous_frame;
    state->previous_frame = current_reference;
    current_reference = state->current_top_field;
    previous_top_field = state->previous_top_field;
    state->previous_top_field = current_reference;
    current_reference = state->current_bottom_field;
    previous_bottom_field = state->previous_bottom_field;
    state->current_frame = previous_frame;
    state->current_top_field = previous_top_field;
    state->current_bottom_field = previous_bottom_field;
    state->previous_bottom_field = current_reference;
select_previous_references:
    state->selected_frame = state->previous_frame;
    state->selected_top_field = state->previous_top_field;
    state->selected_bottom_field = state->previous_bottom_field;
    if (picture_structure != 3) {
        goto check_predicted_fields;
    }
    if (coding_type != 2) {
        goto references_available;
    }
    primary_reference = state->current_frame;
    reference_status = 1;
    goto check_primary_reference;
check_predicted_fields:
    /* Forced reordering can reuse a ready previous field. Otherwise test
     * the current top/bottom pair, as in the retail branch at 0x00129704. */
    primary_reference = (picture_structure == 1) ? state->previous_bottom_field : state->previous_top_field;
    if (coding_type != 2) goto references_available;
    if (force_reorder != 0 && primary_reference->status == 1) goto references_available;
    primary_reference = state->current_top_field;
    reference_status = primary_reference->status;
    if (reference_status != 1) goto select_picture;
    primary_reference = state->current_bottom_field;
check_primary_reference:
    if (primary_reference->status != reference_status) {
        goto select_picture;
    }
references_available:
    references_ready = 1;
select_picture_after_reference_check:
select_picture:
    if (state->picture_structure == 2) {
        goto select_bottom_field;
    }
    if (state->picture_structure >= 3) {
        goto check_frame_structure;
    }
    if (state->picture_structure != 1) {
        goto no_selected_field;
    }
    selected_picture = state->selected_top_field;
    goto copy_picture_metadata;
no_selected_field:
    goto copy_picture_metadata;
check_frame_structure:
    if (state->picture_structure != 3) {
        goto no_selected_frame;
    }
    selected_picture = state->selected_frame;
    goto copy_picture_metadata;
no_selected_frame:
    goto copy_picture_metadata;
select_bottom_field:
    selected_picture = state->selected_bottom_field;
copy_picture_metadata:
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
#endif /* NON_MATCHING */
