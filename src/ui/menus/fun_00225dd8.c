#include "rnc/ui/menus/fun_00225dd8.h"
#include "types.h"

extern u8 D_001D60B8[];
s32 FUN_00225dd8(s32 arg0) {
    s32 count = 0;
    u8 *base = D_001D60B8;
    u8 *entry = base + 4;
loop:
    count += 1;
    if (*(s32 *)(entry - 4) != arg0) {
        entry += 8;
        if (count >= 5) {
            return 1;
        }
        goto loop;
    }
    *(s32 *)entry |= 4;
    return 0;
}
