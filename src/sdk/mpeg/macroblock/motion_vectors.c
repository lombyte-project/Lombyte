#include "types.h"

typedef struct MpegDec MpegDec;

extern s32  _nextBit(MpegDec *d, s32 n);
extern void _motionVector(MpegDec *d,
                          s32     *PMV,
                          s32     *dmvector,
                          s32      h_r_size,
                          s32      v_r_size,
                          s32      dmv,
                          s32      mvscale,
                          s32      full_pel_vector);

void _motionVectors(MpegDec *decoder,
                    s32      PMV[2][2][2],
                    s32      dmvector[2],
                    s32      motion_vertical_field_select[2][2],
                    s32      motion_direction_index,
                    s32      motion_vector_count,
                    s32      mv_format,
                    s32      h_r_size,
                    s32      v_r_size,
                    s32      dmv,

                    s32 mvscale) {
    if (motion_vector_count == 1) {
        if (mv_format == 0 && !dmv) {
            motion_vertical_field_select[1][motion_direction_index] =
                motion_vertical_field_select[0][motion_direction_index] = _nextBit(decoder, 1);
        }
        _motionVector(
            decoder, PMV[0][motion_direction_index], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        PMV[1][motion_direction_index][0] = PMV[0][motion_direction_index][0];
        PMV[1][motion_direction_index][1] = PMV[0][motion_direction_index][1];
    } else {
        motion_vertical_field_select[0][motion_direction_index] = _nextBit(decoder, 1);
        _motionVector(
            decoder, PMV[0][motion_direction_index], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
        motion_vertical_field_select[1][motion_direction_index] = _nextBit(decoder, 1);
        _motionVector(
            decoder, PMV[1][motion_direction_index], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
    }
}
