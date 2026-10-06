#include "types.h"

struct MpegTimestampState {
    u8 pad_0[0x70];
    s32 interpolate_timestamps;
    u8 pad_74[0x4];
    s64 timestamp_scale;
    s32 previous_timestamp;
    u8 pad_84[0x4];
    s64 timestamp_step;
    s32 timestamp_carry;
    u8 pad_94[0x5C];
    s64 override_timestamp;
    s32 override_state;
};

struct MpegPictureTimestampRecord {
    u8 pad_0[0x18];
    s64 presentation_timestamp;
    s64 decoding_timestamp;
    u8 pad_28[0x4];
    s32 coding_type;
    s32 picture_structure;
    s32 progressive_sequence;
    s32 progressive_frame;
    s32 top_field_first;
    s32 repeat_first_field;
};

extern s64 __muldi3(s64 a, s64 b);

void GetMpegPictureTimestampsAndFlags(struct MpegTimestampState *state,
                                      struct MpegPictureTimestampRecord *picture,
                                      s64 *presentation_timestamp, s64 *decoding_timestamp,
                                      s64 *flags) __asm__("_getPtsDtsFlags");

void GetMpegPictureTimestampsAndFlags(struct MpegTimestampState *state,
                                      struct MpegPictureTimestampRecord *picture,
                                      s64 *presentation_timestamp, s64 *decoding_timestamp,
                                      s64 *flags) {
    s32 previous_timestamp;
    s32 timestamp_carry;
    s64 timestamp_step;
    s64 signed_step;
    s32 half_step_carry;
    s32 step_low_word;
    s32 half_step;
    s64 override_timestamp;
    s64 field_flags, sequence_flags, picture_flags;

    if (state->interpolate_timestamps != 0) {
        if ((picture->presentation_timestamp < 0) &&
            (previous_timestamp = state->previous_timestamp, (previous_timestamp >= 0))) {
            timestamp_step = state->timestamp_step;
            step_low_word = (s32)timestamp_step;
            signed_step = step_low_word;
            half_step_carry =
                (s32)__muldi3(__muldi3((s64)step_low_word & 1, state->timestamp_scale & 1),
                              (timestamp_carry = state->timestamp_carry, timestamp_carry & 1));
            /* Keep the retail truncation and additions at 32 bits. */
            half_step = (s32)(__muldi3(state->timestamp_scale, signed_step) >> 1);
            *presentation_timestamp = previous_timestamp + (s32)(half_step + half_step_carry);
            if (__muldi3((s64)step_low_word & 1, state->timestamp_scale & 1) != 0) {
                state->timestamp_carry = (timestamp_carry + 1);
            }
        } else {
            *presentation_timestamp = picture->presentation_timestamp;
        }
    } else {
        *presentation_timestamp = picture->presentation_timestamp;
    }
    if (state->override_state == 2) {
        override_timestamp = state->override_timestamp;
        if (override_timestamp >= 0) {
            *presentation_timestamp = override_timestamp;
            state->override_state = 0;
            state->override_timestamp = -1;
        }
    }
    *decoding_timestamp = picture->decoding_timestamp;
    field_flags = ((s64)picture->repeat_first_field << 5) | ((s64)picture->top_field_first << 6);
    sequence_flags = ((s64)picture->progressive_sequence << 8) | picture->coding_type;
    picture_flags = ((s64)picture->progressive_frame << 7) | ((s64)picture->picture_structure << 3);
    *flags = (sequence_flags | field_flags) | picture_flags;
}
