#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _getAllRefs; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_getAllRefs/_getAllRefs.s", _getAllRefs);
#else
#include "types.h"
#include "rnc/sdk/libmpeg.h"

/* libmpeg motion compensation: builds the forward and backward
   predictions of one macroblock (the MPEG-2 reference decoder's
   form_predictions, on the library's decoder state). */
extern char D_00153710[];
extern char D_00153730[];
extern char D_00153750[];
extern void _Error1(struct MpegDecoder *d, char *fmt, int code);
extern void _dualPrimeVector(struct MpegDecoder *d, int dmv[2][2], int *dmvector, int mvx, int mvy);
extern void _getRef0(struct MpegDecoder *d, struct MpegRefImage *ref, int sfield, int dfield,
                     int yofs, int h, int bx, int by, int dx, int dy, int fieldpred, int avg);

void _getAllRefs(struct MpegDecoder *d, int bx, int by, int mb_type, int motion_type,
                 int pmv[2][2][2], int mvfs[2][2], int *dmvector) {
    int dmv[2][2];
    struct MpegRefImage *refs[2][2];
    int one = 1;
    int avg;
    int currentfield;
    int same;

    avg = 0;

    d->mb_buf[d->mb_buf_index].fetch_count = 0;

    if ((mb_type & 8) || d->picture_coding_type == 2) {
        if (d->picture_structure == 3) {
            if (motion_type == 2 || !(mb_type & 8)) {
                _getRef0(d, d->ref_images[0], 0, 0, 0, 16, bx, by, pmv[0][0][0], pmv[0][0][1], 0,
                         0);
            } else if (motion_type == one) {
                _getRef0(d, d->ref_images[0], mvfs[0][0], 0, 0, 8, bx, by, pmv[0][0][0],
                         pmv[0][0][1] >> 1, one, 0);
                _getRef0(d, d->ref_images[0], mvfs[1][0], 1, 0, 8, bx, by, pmv[1][0][0],
                         pmv[1][0][1] >> 1, one, 0);
            } else if (motion_type == 3) {
                _dualPrimeVector(d, dmv, dmvector, pmv[0][0][0], pmv[0][0][1] >> 1);
                _getRef0(d, d->ref_images[0], 0, 0, 0, 8, bx, by, pmv[0][0][0], pmv[0][0][1] >> 1,
                         one, 0);
                _getRef0(d, d->ref_images[0], 1, 0, 0, 8, bx, by, dmv[0][0], dmv[0][1], one, one);
                _getRef0(d, d->ref_images[0], 1, 1, 0, 8, bx, by, pmv[0][0][0], pmv[0][0][1] >> 1,
                         one, 0);
                _getRef0(d, d->ref_images[0], 0, 1, 0, 8, bx, by, dmv[1][0], dmv[1][1], one, one);
            } else {
                _Error1(d, D_00153710, motion_type);
            }
        } else {
            currentfield = d->picture_structure == 2;
            /* Keep the retail stack order: two field choices per reference set. */
            refs[0][0] = d->ref_images[4];
            refs[0][1] = d->ref_images[8];
            refs[1][0] = d->ref_images[5];
            refs[1][1] = d->ref_images[9];
            same = 0;
            if (d->picture_coding_type == 2 && d->second_field) {
                same = currentfield != mvfs[0][0];
            }
            if (motion_type == 1 || !(mb_type & 8)) {
                _getRef0(d, refs[same][mvfs[0][0]], 0, 0, 0, 16, bx, by, pmv[0][0][0], pmv[0][0][1],
                         0, 0);
            } else if (motion_type == 2) {
                _getRef0(d, refs[same][mvfs[0][0]], 0, 0, 0, 8, bx, by, pmv[0][0][0], pmv[0][0][1],
                         0, 0);
                same = d->picture_coding_type == motion_type && d->second_field &&
                       currentfield != mvfs[1][0];
                _getRef0(d, refs[same][mvfs[1][0]], 0, 0, 8, 8, bx, by, pmv[1][0][0], pmv[1][0][1],
                         0, 0);
            } else if (motion_type == 3) {
                same = d->second_field ? one : 0;
                _dualPrimeVector(d, dmv, dmvector, pmv[0][0][0], pmv[0][0][1]);
                _getRef0(d, refs[0][currentfield], 0, 0, 0, 16, bx, by, pmv[0][0][0], pmv[0][0][1],
                         0, 0);
                _getRef0(d, refs[same][!currentfield], 0, 0, 0, 16, bx, by, dmv[0][0], dmv[0][1], 0,
                         1);
            } else {
                _Error1(d, D_00153730, motion_type);
            }
        }
        avg = 1;
    }

    if (mb_type & 4) {
        if (d->picture_structure == 3) {
            if (motion_type == 2) {
                _getRef0(d, d->ref_images[1], 0, 0, 0, 16, bx, by, pmv[0][1][0], pmv[0][1][1], 0,
                         avg);
            } else {
                _getRef0(d, d->ref_images[1], mvfs[0][1], 0, 0, 8, bx, by, pmv[0][1][0],
                         pmv[0][1][1] >> 1, 1, avg);
                _getRef0(d, d->ref_images[1], mvfs[1][1], 1, 0, 8, bx, by, pmv[1][1][0],
                         pmv[1][1][1] >> 1, 1, avg);
            }
        } else if (motion_type == 1) {
            _getRef0(d, mvfs[0][1] ? d->ref_images[9] : d->ref_images[5], 0, 0, 0, 16, bx, by,
                     pmv[0][1][0], pmv[0][1][1], 0, avg);
        } else if (motion_type == 2) {
            _getRef0(d, mvfs[0][1] ? d->ref_images[9] : d->ref_images[5], 0, 0, 0, 8, bx, by,
                     pmv[0][1][0], pmv[0][1][1], 0, avg);
            _getRef0(d, mvfs[1][1] ? d->ref_images[9] : d->ref_images[5], 0, 0, 8, 8, bx, by,
                     pmv[1][1][0], pmv[1][1][1], 0, avg);
        } else {
            _Error1(d, D_00153750, motion_type);
        }
    }
}
#endif /* NON_MATCHING */
