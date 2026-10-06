#include "types.h"
struct Point {
    u64 flags;
    s16 y;
    s16 x;
    s16 u;
    s16 v;
};
extern s32 D_0015F45C;
extern struct Point D_0018D440[];
extern s16 FUN_001f97a0(s32);
void unpack_point_records(s32 *src, s32 count) __asm__("FUN_00202800");

void unpack_point_records(s32 *src, s32 count) {
    s32 x;
    s32 y;
    s32 u;
    s32 v;

    for (D_0015F45C = 0; D_0015F45C < count; D_0015F45C++) {
        x = *src++;
        y = *src++;
        u = *src++;
        v = *src++;
        D_0018D440[D_0015F45C].x = x >> 4;
        D_0018D440[D_0015F45C].y = y >> 4;
        D_0018D440[D_0015F45C].u = FUN_001f97a0(u);
        D_0018D440[D_0015F45C].v = FUN_001f97a0(v);
        D_0018D440[D_0015F45C].flags = 0;
    }
}

extern __typeof__(unpack_point_records) func_00202800 __attribute__((alias("FUN_00202800")));
