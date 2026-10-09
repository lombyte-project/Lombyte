#include "types.h"
#include "rnc/sdk/ipu_command_table.h"
typedef struct {
    u8 pad0[0x11C];
    s32 unk11C;
    u8 pad120[0x6F8];
    s32 unk818;
    u8 pad81C[0x1C];
    s32 unk838;
    s32 unk83C;
    u8 pad840[0x18];
    void *cbData;
} MpegDec;

extern void _dispatchMpegCbNodata(void *cb);

#define IPU_CMD  ((volatile u64 *)0x10002000)
#define IPU_CTRL ((volatile u32 *)0x10002010)
#define IPU_BP   ((volatile u32 *)0x10002020)
#define IPU_TOP  ((u64 *)0x10002030)

s32 _ipuVdec(MpegDec *d, s32 tbl) {
    s32 i;
    s32 j;
    u32 cmd;
    s64 r;
    s64 top;
    u32 bp;

    i = 0;
    j = 0;
    while ((*IPU_CTRL & 0x80004000) == 0x80000000) {
        if (j++ > 5000) {
            _dispatchMpegCbNodata(d->cbData);
            j = 0;
        }
    }
    cmd = (tbl << 26) | 0x30000000;
    *(volatile u32 *)IPU_CMD = cmd;
    d->unk818 = IpuCommandTable[(s32)cmd >> 28];
    r = *IPU_CMD;
    while (r < 0) {
        if (i++ > 5000) {
            _dispatchMpegCbNodata(d->cbData);
            i = 0;
        }
        r = *IPU_CMD;
    }
    bp = *IPU_BP;
    top = *IPU_TOP;
    d->unk838 = top;
    if (top < 0) {
        d->unk83C = -(bp & 0x1F) & 0x1F;
    } else {
        d->unk83C = 32;
    }
    d->unk11C = (s32)r == 0;
    return (s16)r;
}
