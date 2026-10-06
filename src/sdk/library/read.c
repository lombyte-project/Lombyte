/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
extern u32 D_0012FBF0[];
extern s32 sceTtyInit(void);
extern s32 sceTtyRead();
s32 read(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0) {
        if (D_0012FBF0[0] == 0) {
            if (sceTtyInit() != 0) {
                do {
                    D_0012FBF0[0] = 1;
                } while (0);
                goto block_4;
            }
            goto block_5;
        }
    block_4:
        return sceTtyRead(arg1, arg2);
    }
block_5:
    return -1;
}
