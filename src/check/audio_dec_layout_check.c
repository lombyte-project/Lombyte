/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/audio/decoder/audio_dec.h"

SIZE_CHECK(spu_stream_header, struct SpuStreamHeader, 0x20);
OFFSET_CHECK(hdr, struct AudioDec, hdr, 0x8);
OFFSET_CHECK(rate, struct AudioDec, hdr.rate, 0x14);
OFFSET_CHECK(ch, struct AudioDec, hdr.ch, 0x18);
OFFSET_CHECK(hdr_count, struct AudioDec, hdrCount, 0x30);
OFFSET_CHECK(data, struct AudioDec, data, 0x34);
OFFSET_CHECK(total_bytes, struct AudioDec, totalBytes, 0x44);
OFFSET_CHECK(iop_buff, struct AudioDec, iopBuff, 0x48);
OFFSET_CHECK(iop_zero, struct AudioDec, iopZero, 0x5C);
SIZE_CHECK(audio_dec, struct AudioDec, 0x64);
