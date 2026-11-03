#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/sdk/ipu.h"
#include "rnc/sdk/mpeg/bitstream.h"
#include "rnc/sdk/mpeg/errors.h"
#include "rnc/sdk/mpeg/macroblock.h"

struct MpegMbState {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
};

const char invalid_macroblock_type_error_message[] __asm__("D_00153898") =
    "Invalid macroblock_type code: 0";
int       _decMB0(struct MpegDecoder *decoder,
                  int                *mb_type,
                  int                *motion_type,
                  int                *dct_type_out,
                  struct MpegMbState *motion_vector_state,
                  int                *motion_field_select,

                  int differential_motion_vector) {
    enum {
        DCT_TYPE_FRAME = 0,
        MOTION_TYPE_FIELD = 1,
        MOTION_TYPE_FRAME = 2,
        PICTURE_CODING_TYPE_PREDICTIVE = 2
    };
    int *dct_type_output;
    int *motion_field_select_output;
    u32  intra_quantiser_command_bits;
    u32  dc_reset_command_base;
    int  motion_vector_count;
    int  type_code;
    int  type;
    int  coding_type;
    int  new_var;
    int  command_base;
    int  structure;
    int  forward_r_size;
    int  backward_r_size;
    int  frame_motion;
    int  field_in_frame;
    int  dct_type;
    int  motion_flag;
    int  decode_success;
    int  coded;
    *((int *)0x10002010) =
        ((*((int *)0x10002010)) & 0xF8FFFFFF) | (decoder->picture_coding_type << 0x18);
    dct_type_output = dct_type_out;
    motion_field_select_output = motion_field_select;
    type_code = _ipuVdec(decoder, 1);
    *mb_type = type_code;
    if (type_code == 0) {
        _Error(decoder, invalid_macroblock_type_error_message);
        decoder->slice_error = 1;
        return 0;
    }
    if (type_code & 0xC) {
        if ((decoder->picture_structure == 3) && (decoder->frame_pred_frame_dct != 0)) {
            *motion_type = MOTION_TYPE_FRAME;
        } else {
            *motion_type = _nextBit(decoder, 2);
        }
    } else if ((type_code & 1) && (decoder->concealment_motion_vectors != 0)) {
        *motion_type = ((decoder->picture_structure ^ 3) == 0) ? (2) : (1);
    }
    structure = decoder->picture_structure;
    if (structure == 3) {
        motion_vector_count = (((*motion_type) ^ 1) == 0) ? (2) : (1);
        frame_motion = (*motion_type) == 2;
    } else {
        new_var = 2;
        frame_motion = 0;
        motion_vector_count = (((*motion_type) ^ new_var) == 0) ? (2) : (1);
    }
    field_in_frame = 0;
    motion_flag = (*motion_type) == 3;
    if (frame_motion == 0) {
        field_in_frame = structure == 3;
    }
    dct_type = DCT_TYPE_FRAME;
    if (((decoder->picture_structure == 3) && (decoder->frame_pred_frame_dct == 0)) &&
        (((*mb_type) & 3) != 0)) {
        dct_type = _nextBit(decoder, 1);
    } else {
        dct_type = DCT_TYPE_FRAME;
    }
    *dct_type_output = dct_type;
    if ((*mb_type) & 0x10) {
        decoder->quantiser_scale_code = _nextBit(decoder, 5);
    }
    if (((*mb_type) & 8) || (((*mb_type) & 1) && (decoder->concealment_motion_vectors != 0))) {
        if (decoder->mpeg2 != 0) {
            _motionVectors(decoder,
                           motion_vector_state,
                           differential_motion_vector,
                           motion_field_select_output,
                           0,
                           motion_vector_count,
                           frame_motion,
                           decoder->f_code_forward_h - 1,
                           decoder->f_code_forward_v - 1,
                           motion_flag,
                           field_in_frame);
        } else {
            forward_r_size = decoder->forward_f_code - 1;
            _motionVector(decoder,
                          motion_vector_state,
                          differential_motion_vector,
                          forward_r_size,
                          forward_r_size,
                          0,
                          0,
                          decoder->full_pel_forward_vector);
        }
    }
    decode_success = 0;
    if (decoder->slice_error == 0) {
        if ((*mb_type) & 4) {
            if (decoder->mpeg2 != 0) {
                _motionVectors(decoder,
                               motion_vector_state,
                               differential_motion_vector,
                               motion_field_select_output,
                               1,
                               motion_vector_count,
                               frame_motion,
                               decoder->f_code_backward_h - 1,
                               decoder->f_code_backward_v - 1,
                               0,
                               field_in_frame);
            } else {
                backward_r_size = decoder->backward_f_code - 1;
                _motionVector(decoder,
                              ((u8 *)motion_vector_state) + 8,
                              differential_motion_vector,
                              backward_r_size,
                              backward_r_size,
                              0,
                              0,
                              decoder->full_pel_backward_vector);
            }
        }
        decode_success = 0;
        if (decoder->slice_error == 0) {
            type = *mb_type;
            coded = type & 3;
            if (type & 1) {
                if (decoder->concealment_motion_vectors != 0) {
                    _flushBuf(decoder, 1);
                };
            }
            if (((*mb_type) & 3) != 0) {
                receiveDataFromIPU(decoder->mb_buf[decoder->mb_buf_index].ipu_out, 0x300);
                _waitIpuIdle(decoder);
                intra_quantiser_command_bits =
                    (((*mb_type) & 1) << 0x1B) | (decoder->quantiser_scale_code << 0x10);
                dc_reset_command_base = ((decoder->dc_reset) << 0x1A) | 0x20000000;
                SendIpuCommand(decoder,
                               (intra_quantiser_command_bits | dc_reset_command_base) |
                                   ((*dct_type_output) << 0x19));
            } else {
                decoder->mb_buf[decoder->mb_buf_index].unk13C = 1;
            }
            decoder->dc_reset = 0;
            if (decoder->slice_error != 0) {
                if (decoder->picture_structure) {
                    return 0;
                }
                return 0;
            }
            if ((((*mb_type) & 1) || ((decoder->dc_reset = 1, ((*mb_type) & 1) != 0))) &&
                (decoder->concealment_motion_vectors == 0)) {
                motion_vector_state->unk14 = 0;
                motion_vector_state->unk10 = 0;
                motion_vector_state->unk4 = 0;
                motion_vector_state->unk0 = 0;
                motion_vector_state->unk1C = 0;
                motion_vector_state->unk18 = 0;
                motion_vector_state->unkC = 0;
                motion_vector_state->unk8 = 0;
            }
            coding_type = decoder->picture_coding_type;
            decode_success = 1;
            if (coding_type == PICTURE_CODING_TYPE_PREDICTIVE) {
                decode_success = 1;
                if (!((*mb_type) & 9)) {
                    motion_vector_state->unk0 =
                        (motion_vector_state->unk4 =
                             (motion_vector_state->unk10 = (motion_vector_state->unk14 = 0)));
                    if (decoder->picture_structure == 3) {
                        *motion_type = coding_type;
                    } else {
                        *motion_type = MOTION_TYPE_FIELD;
                        *motion_field_select_output = decoder->picture_structure == 2;
                    }
                    decode_success = 1;
                }
            }
            return decode_success;
        }
        return decode_success;
    }
    return decode_success;
}
