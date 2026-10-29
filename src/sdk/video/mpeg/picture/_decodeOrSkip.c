#include "types.h"

extern s32 _decodeOrSkipFrame();
extern s32 _decodeOrSkipField();

s32 _decodeOrSkip(s32 mp, s32 skip_comparison_value, s32 skip_threshold) {
    if ((*(s32 **)(mp + 0x40))[0x5D] != 3) {
        return _decodeOrSkipField(mp, skip_comparison_value, skip_threshold);
    }
    return _decodeOrSkipFrame(mp, skip_comparison_value, skip_threshold);
}
