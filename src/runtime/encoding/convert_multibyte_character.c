#include "types.h"

s32 convert_multibyte_character(void *r, s32 *pwc, const u8 *s, u32 n) __asm__("ConvertMultibyteCharacter");
s32 convert_multibyte_character(void *r, s32 *pwc, const u8 *s, u32 n) {
    s32 dummy;

    if (pwc == 0) {
        pwc = &dummy;
    }
    if (s != 0 && n == 0) {
        return -1;
    }
    if (s == 0) {
        return 0;
    }
    *pwc = *s;
    return *s != 0;
}
