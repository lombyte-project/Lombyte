#include "types.h"

typedef struct MbCtx {
    u8  pad_0[0x11C];
    s32 slice_error;
    u8  pad_120[0x728];
    s32 mpeg2;
} MbCtx;

extern s32  _ipuVdec();
extern s32  _peepBit();
extern void _flushBuf();
extern void _Error1();
const u8    D_001537A8[] = "Invalid macroblock_address_increment code(0x%08x)";

s32 _mbAddressIncrement(MbCtx *ctx) {
    enum {
        MACROBLOCK_ADDRESS_INCREMENT_VLC_FAILURE = 0,
        MACROBLOCK_ADDRESS_INCREMENT_STUFFING = 0x22,
        MACROBLOCK_ADDRESS_INCREMENT_ESCAPE = 0x23
    };
    s32          total;
    register u32 code;
    s32          bits;
    register s32 ret;
    total = 0;
    do {
        code = _ipuVdec(ctx, 0);
        switch (code) {
        case MACROBLOCK_ADDRESS_INCREMENT_STUFFING:
            ret = 1;
            break;
        case MACROBLOCK_ADDRESS_INCREMENT_ESCAPE:
            ret = 1;
            total += 0x21;
            break;
        case MACROBLOCK_ADDRESS_INCREMENT_VLC_FAILURE:
            bits = _peepBit(ctx, 0xB);
            if (ctx->mpeg2 != 0 && bits == 0xF) {
                _flushBuf(ctx, 0xB);
                ret = 1;
                break;
            }
            _Error1(ctx, D_001537A8, code);
            ctx->slice_error = 1;
            return 1;
        default:
            total += code;
            ret = 0;
            break;
        }
    } while (ret != 0);
    return total;
}
