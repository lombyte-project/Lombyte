#include "types.h"

typedef struct MbCtx {
    u8 pad_0[0x11C];
    s32 unk11C;
    u8 pad_120[0x728];
    s32 unk848;
} MbCtx;

extern s32 _ipuVdec();
extern s32 _peepBit();
extern void _flushBuf();
extern void _Error1();
extern u8 D_001537A8[];

s32 _mbAddressIncrement(MbCtx *ctx) {
    s32 total;
    register u32 code;
    s32 bits;
    register s32 ret;
    total = 0;
    do {
        code = _ipuVdec(ctx, 0);
        switch (code) {
        case 0x22:
            ret = 1;
            break;
        case 0x23:
            ret = 1;
            total += 0x21;
            break;
        case 0:
            bits = _peepBit(ctx, 0xB);
            if (ctx->unk848 != 0 && bits == 0xF) {
                _flushBuf(ctx, 0xB);
                ret = 1;
                break;
            }
            _Error1(ctx, D_001537A8, code);
            ctx->unk11C = 1;
            return 1;
        default:
            total += code;
            ret = 0;
            break;
        }
    } while (ret != 0);
    return total;
}
