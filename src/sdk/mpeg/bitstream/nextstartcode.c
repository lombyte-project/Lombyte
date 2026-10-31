#include "types.h"
#include "rnc/sdk/ipu/wait_ipu_idle.h"
#include "rnc/sdk/mpeg/bitstream/_flushBuf.h"
#include "rnc/sdk/mpeg/bitstream/_peepBit.h"

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
