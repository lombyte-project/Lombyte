#include "types.h"

extern void _waitIpuIdle(s32 *ctx);
extern s32  _peepBit(s32 *ctx, s32 count);
extern void _flushBuf(s32 *ctx, s32 count);

void _nextStartCode(s32 *decoder_state) {
    s32 raw;
    s32 amount;
    _waitIpuIdle(decoder_state);
    raw = *(volatile s32 *)0x10002020;
    amount = (-(raw & 7)) & 7;
    if (amount != 0) {
        _flushBuf(decoder_state, amount);
    }
    while (_peepBit(decoder_state, 0x18) != 1) {
        _flushBuf(decoder_state, 8);
    }
}
