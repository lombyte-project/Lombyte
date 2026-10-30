/* Ported from rac1-decomp (src/core/00125630.c, func_00125F88). */
#include "types.h"

#include "types.h"
#include "rnc/sdk/libmpeg.h"

/* libmpeg motion compensation: queues the forward/backward reference
   fetch of one macroblock (luma and chroma) for the IPU/DMA stage. */
extern MpegMcFunc mpeg_luma_mc_functions[] __asm__("D_00132E30");
extern MpegMcFunc mpeg_chroma_mc_functions[] __asm__("D_00132E50");

void _getRef0(struct MpegDecoder  *decoder,
              struct MpegRefImage *ref,
              int                  sfield,
              int                  dfield,
              int                  yofs,
              int                  luma_height,
              int                  bx,
              int                  by,
              int                  dx,
              int                  dy,
              int                  fieldpred,

              int avg) {
    struct MpegMcFetch *fl;
    struct MpegMcFetch *fc;
    u8                 *mc;
    int fetch_index, x, y, mbx, mby, ly, lx, first_span_rows, fetch_scratch_base, code, mb;
    int cmvx, cmvy, ch, cmbx, cmby;
    int xh, yh, cyo, luma_first_span_offset, luma_second_span_offset;
    u8 *data;

    fetch_index = decoder->mb_buf[decoder->mb_buf_index].fetch_count;
    mc = decoder->mc_work;
    fl = &decoder->mb_buf[decoder->mb_buf_index].luma[fetch_index];
    fc = &decoder->mb_buf[decoder->mb_buf_index].chroma[fetch_index];

    x = (dx >> 1) + bx;
    if (fieldpred) {
        y = ((dy >> 1) << 1) + yofs + (by + sfield);
    } else {
        y = (dy >> 1) + yofs + (by + sfield);
    }
    mbx = x >> 4;
    mby = y >> 4;
    mb = mbx * ref->mb_height + mby;
    fl->x = x - (mbx << 4);
    fl->addr = mc + ((dfield + yofs) << 5);
    ly = y - (mby << 4);
    yh = dy & 1;
    xh = dx & 1;
    if (yh) {
        if (ly + (luma_height << fieldpred) >= 16) {
            first_span_rows = (16 >> fieldpred) - (ly >> fieldpred) - 1;
            fl->n0 = first_span_rows;
            fl->n1 = luma_height - first_span_rows;
        } else {
            fl->n0 = luma_height;
            fl->n1 = 0;
        }
    } else {
        if (ly + (luma_height << fieldpred) >= 17) {
            first_span_rows = (16 >> fieldpred) - (ly >> fieldpred);
            fl->n0 = first_span_rows;
            fl->n1 = luma_height - first_span_rows;
        } else {
            fl->n0 = luma_height;
            fl->n1 = 0;
        }
    }
    fetch_scratch_base = decoder->mb_buf[decoder->mb_buf_index].spr_base + fetch_index * 0x600;
    cmvx = dx / 2;
    luma_first_span_offset = ly << 4;
    fl->p0 = (u8 *)(fetch_scratch_base + luma_first_span_offset);
    luma_second_span_offset = luma_first_span_offset + 0x300;
    fl->p1 = (u8 *)(fetch_scratch_base + luma_second_span_offset);
    fl->step = 16 << fieldpred;
    cmvy = dy / 2;
    code = (avg << 2) | (xh << 1) | yh;

    ch = luma_height >> 1;
    x = (cmvx >> 1) + (bx >> 1);
    cyo = yofs >> 1;
    if (fieldpred) {
        y = ((cmvy >> 1) << 1) + cyo + ((by >> 1) + sfield);
    } else {
        y = (cmvy >> 1) + cyo + ((by >> 1) + sfield);
    }
    cmby = y >> 3;
    cmbx = x >> 3;
    xh = cmvx & 1;
    yh = cmvy & 1;
    fc->x = x - (cmbx << 3);
    fc->addr = mc + (((dfield + cyo) << 4) + 0x200);
    ly = y - (cmby << 3);
    if (yh) {
        if (ly + (ch << fieldpred) >= 8) {
            first_span_rows = (8 >> fieldpred) - (ly >> fieldpred) - 1;
            fc->n0 = first_span_rows;
            fc->n1 = ch - first_span_rows;
        } else {
            fc->n0 = ch;
            fc->n1 = 0;
        }
    } else {
        if (ly + (ch << fieldpred) >= 9) {
            first_span_rows = (8 >> fieldpred) - (ly >> fieldpred);
            fc->n0 = first_span_rows;
            fc->n1 = ch - first_span_rows;
        } else {
            fc->n0 = ch;
            fc->n1 = 0;
        }
    }
    fc->step = 8 << fieldpred;
    decoder->mb_buf[decoder->mb_buf_index].luma_func[fetch_index] = mpeg_luma_mc_functions[code];
    data = ref->data;
    {
        int chroma_first_span_offset = (ly << 3) + 0x100;
        int chroma_second_span_offset = (ly << 3) + 0x400;
        u8 *chroma_first_span_data = (u8 *)((((cmbx - mbx) << 1) + (cmby - mby)) * 0x180 +
                                            fetch_scratch_base + chroma_first_span_offset);
        u8 *chroma_second_span_data = (u8 *)((((cmbx - mbx) << 1) + (cmby - mby)) * 0x180 +
                                             fetch_scratch_base + chroma_second_span_offset);
        fc->p1 = chroma_second_span_data;
        fc->p0 = chroma_first_span_data;
    }
    decoder->mb_buf[decoder->mb_buf_index].luma_ref[fetch_index] = data + mb * 0x180;
    decoder->mb_buf[decoder->mb_buf_index].chroma_ref[fetch_index] =
        data + (mb + ref->mb_height) * 0x180;
    decoder->mb_buf[decoder->mb_buf_index].chroma_func[fetch_index] =
        mpeg_chroma_mc_functions[(avg << 2) | (xh << 1) | yh];
    decoder->mb_buf[decoder->mb_buf_index].fetch_count++;
}
