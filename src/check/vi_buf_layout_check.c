/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/video/decoder/vi_buf.h"

SIZE_CHECK(time_stamp, struct ViBufTimeStamp, 0x18);
OFFSET_CHECK(dma_n, struct ViBuf, dma_n, 0x10);
OFFSET_CHECK(buff_size, struct ViBuf, buff_size, 0x18);
OFFSET_CHECK(d4_madr, struct ViBuf, d4_madr, 0x1C);
OFFSET_CHECK(ipu_ctrl, struct ViBuf, ipu_ctrl, 0x3C);
OFFSET_CHECK(sema, struct ViBuf, sema, 0x40);
OFFSET_CHECK(total_bytes, struct ViBuf, total_bytes, 0x48);
OFFSET_CHECK(ts, struct ViBuf, ts, 0x50);
OFFSET_CHECK(wt_ts, struct ViBuf, wt_ts, 0x5C);
SIZE_CHECK(vi_buf, struct ViBuf, 0x60);

#include "rnc/video/decoder/video_dec.h"

OFFSET_CHECK(video_dec_vi_buf, struct VideoDec, vi_buf, 0x48);
OFFSET_CHECK(video_dec_state, struct VideoDec, state, 0xA8);
SIZE_CHECK(video_dec, struct VideoDec, 0xB8);
