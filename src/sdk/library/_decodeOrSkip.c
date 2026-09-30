#include "types.h"

extern s32 _decodeOrSkipFrame();
extern s32 _decodeOrSkipField();

s32 _decodeOrSkip(s32 mp, s32 arg1, s32 arg2) {
    if ((*(s32 **)(mp + 0x40))[0x5D] != 3) {
        return _decodeOrSkipField(mp, arg1, arg2);
    }
    return _decodeOrSkipFrame(mp, arg1, arg2);
}
