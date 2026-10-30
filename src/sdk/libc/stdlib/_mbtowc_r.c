#include "types.h"

s32 _mbtowc_r(void *r, s32 *pwc, const u8 *s, u32 n) __asm__("ConvertMultibyteCharacter");
s32 _mbtowc_r(void     *unused_parameter,
              s32      *pwc,
              const u8 *source_bytes,

              u32 available_source_bytes) {
    s32 dummy;

    if (pwc == 0) {
        pwc = &dummy;
    }
    if (source_bytes != 0 && available_source_bytes == 0) {
        return -1;
    }
    if (source_bytes == 0) {
        return 0;
    }
    *pwc = *source_bytes;
    return *source_bytes != 0;
}
