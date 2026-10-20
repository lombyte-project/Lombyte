/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

typedef struct {
    u8 p[0x18E];
    s16 h;
    s32 n;
    u8 p2[0x1E0 - 0x194];
    u32 t[30];
} S_266d60;
extern S_266d60 D_0013C940_266d60 __asm__("D_0013C940");
u32 pad_pressed_within(u32 mask, s32 n, s32 *out) __asm__("FUN_L00_00266d60");
u32 pad_pressed_within(u32 mask, s32 n, s32 *out) {
    s32 lim = D_0013C940_266d60.n;
    s32 i;
    if (lim >= n)
        lim = n;
    for (i = 1; i < lim + 1; i++) {
        s32 k = (D_0013C940_266d60.h - i + 30) % 30;
        if (D_0013C940_266d60.t[k] & mask) {
            if (out)
                *out = i - 1;
            return D_0013C940_266d60.t[k] & mask;
        }
    }
    return 0;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/stream_002670E0.c: func_L00_00267C48), where it is exact; names translated to the US level program. */

extern int FUN_L00_00266d60_u(int, int, int *) __asm__("FUN_L00_00266d60");
extern s32 SubtractIntegerWithClamp(s32);

/* True when both lookups succeed and the clamped difference of their values is below d. */
int FUN_L00_00266e00(int first_button_mask, int b, int lookback_frames, int press_difference_limit) {
    int v[2];
    int r = 0;
    if (FUN_L00_00266d60_u(first_button_mask, lookback_frames, &v[0])) {
        if (FUN_L00_00266d60_u(b, lookback_frames, &v[1])) {
            r = SubtractIntegerWithClamp(v[0] - v[1]) < press_difference_limit;
        }
    }
    return r;
}
typedef struct {
    char pad[0x18E];
    s16 head;
    s32 count;
    char pad2[0x2D0 - 0x194];
    f32 v[30];
} Hist_266e80;
extern Hist_266e80 D_0013C940;
/* The value n entries back in the 30-entry history ring (n is capped at the count). */
f32 FUN_L00_00266e80(s32 history_offset) {
    s32 m;
    if (D_0013C940.count < history_offset)
        history_offset = D_0013C940.count;
    return D_0013C940.v[(D_0013C940.head - history_offset + 30) % 30];
}
