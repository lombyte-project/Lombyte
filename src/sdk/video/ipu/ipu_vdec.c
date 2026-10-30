#include "types.h"
#include "rnc/sdk/ipu_command_table.h"

typedef struct {
    u8    pad0[0x11C];
    s32   unk11C;
    u8    pad120[0x6F8];
    s32   unk818;
    u8    pad81C[0x1C];
    s32   unk838;
    s32   unk83C;
    u8    pad840[0x18];
    void *cbData;
} MpegDec;

extern void _dispatchMpegCbNodata(void *cb);

#define IPU_CMD  ((volatile u64 *)0x10002000)
#define IPU_CTRL ((volatile u32 *)0x10002010)
#define IPU_BP   ((volatile u32 *)0x10002020)
#define IPU_TOP  ((u64 *)0x10002030)

s32 _ipuVdec(MpegDec *decoder, s32 vlc_table_selector) {
    s32 i;
    s32 j;
    u32 cmd;
    s64 command_result;
    s64 top;
    u32 bp;

    i = 0;
    j = 0;
    while ((*IPU_CTRL & 0x80004000) == 0x80000000) {
        if (j++ > 5000) {
            _dispatchMpegCbNodata(decoder->cbData);
            j = 0;
        }
    }
    cmd = (vlc_table_selector << 26) | 0x30000000;
    *(volatile u32 *)IPU_CMD = cmd;
    decoder->unk818 = IpuCommandTable[(s32)cmd >> 28];
    command_result = *IPU_CMD;
    while (command_result < 0) {
        if (i++ > 5000) {
            _dispatchMpegCbNodata(decoder->cbData);
            i = 0;
        }
        command_result = *IPU_CMD;
    }
    bp = *IPU_BP;
    top = *IPU_TOP;
    decoder->unk838 = top;
    if (top < 0) {
        decoder->unk83C = -(bp & 0x1F) & 0x1F;
    } else {
        decoder->unk83C = 32;
    }
    decoder->unk11C = (s32)command_result == 0;
    return (s16)command_result;
}
