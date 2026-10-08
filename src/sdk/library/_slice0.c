#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern u8 D_00153848[];
extern s32 _Error();
extern s32 _decMB0();
extern s32 _doMC();
extern s32 _mbAddressIncrement();
extern s32 _motionComp0();
extern s32 _peepBit();
extern s32 _skipMB0();
extern s32 _sliceA0();
extern s32 _waitBdecOut();

s32 _slice0(struct MpegDecoder *mp, s32 nmb) {
    s32 blk[16];
    s32 mbx;
    s32 inc;
    s32 mbtype;
    s32 cbp;
    s32 qs;
    s32 ret;

    mbx = 0;
    inc = 0;
    ret = _sliceA0(mp, nmb, &mbx, &inc, blk);
    if (ret != 0) {
        return ret;
    }
    mp->unk11C = 0;
    for (;;) {
        if (mbx >= nmb) {
            return 0;
        }
        mp->mb_buf[mp->mb_buf_index].unk13C = 0;
        if (_waitBdecOut(mp) == 0) {
            return 2;
        }
        if (inc == 0) {
            if (_peepBit(mp, 0x17) == 0 || mp->unk11C != 0) {
                mp->unk11C = 0;
                return 3;
            }
            inc = _mbAddressIncrement(mp);
            if (mp->unk11C != 0) {
                mp->unk11C = 0;
                return 1;
            }
        }
        if (mbx >= nmb) {
            _Error(mp, D_00153848);
            return 2;
        }
        if (inc == 1) {
            if (_decMB0(mp, &mbtype, &cbp, &qs, blk, blk + 8, blk + 12) == 0) {
                mp->unk11C = 0;
                return 1;
            }
        } else {
            if (_skipMB0(mp, blk, &cbp, blk + 8, &mbtype) == 0) {
                mp->unk11C = 0;
                return 2;
            }
        }
        if (_motionComp0(mp, mbx, inc, mbtype, cbp, blk, blk + 8, blk + 12) == 0) {
            mp->unk11C = 0;
            return 2;
        }
        if (mbx != 0) {
            _doMC(mp, mp->mb_buf_index ^ 1);
        }
        mbx++;
        mp->mb_buf_index ^= 1;
        inc--;
    }
}
