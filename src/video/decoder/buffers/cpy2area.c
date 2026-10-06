#include "types.h"
extern void func_00115248(void *, const void *, s32);
extern s32 func_0023B810(s32, s32, s32, s32, s32, s32, s32, s32)
    __attribute__((alias("FUN_0023b810")));

s32 cpy2area(s32 dst_a, s32 dst_a_len, s32 dst_b, s32 dst_b_len, s32 src_a, s32 src_a_len, s32 src_b,
                             s32 src_b_len) __asm__("FUN_0023b810");

s32 cpy2area(s32 dst_a, s32 dst_a_len, s32 dst_b, s32 dst_b_len, s32 src_a, s32 src_a_len, s32 src_b,
                             s32 src_b_len) {
    if (dst_a_len + dst_b_len < src_a_len + src_b_len) {
        return 0;
    }
    if (src_a_len >= dst_a_len) {
        func_00115248(dst_a, src_a, dst_a_len);
        func_00115248(dst_b, src_a + dst_a_len, src_a_len - dst_a_len);
        func_00115248((dst_b + src_a_len) - dst_a_len, src_b, src_b_len);
    } else if (src_b_len >= dst_a_len - src_a_len) {
        func_00115248(dst_a, src_a, src_a_len);
        func_00115248(dst_a + src_a_len, src_b, dst_a_len - src_a_len);
        func_00115248(dst_b, (src_b + dst_a_len) - src_a_len, src_b_len - (dst_a_len - src_a_len));
    } else {
        func_00115248(dst_a, src_a, src_a_len);
        func_00115248(dst_a + src_a_len, src_b, src_b_len);
    }
    return src_a_len + src_b_len;
}
