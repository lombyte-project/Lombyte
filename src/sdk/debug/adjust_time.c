/* adjust_time: deci clock adjust; v0-pinned hour with void addhour(). */

#include "types.h"

struct AdjustTimeArg {
    u8 pad_0[2];
    u8 hour;
};

extern void func_0012D3C0();
extern void subhour();
extern void addhour();
extern void func_0012D428();

void AdjustTime(struct AdjustTimeArg *arg0, s32 arg1)
{
    s32 hour;

    func_0012D3C0();
    hour = arg0->hour + arg1;
    if (hour >= 0) {
        if (hour >= 0x3D) {
            do {
                addhour(arg0);
                hour -= 0x3C;
            } while (hour >= 0x3D);
        }
    } else {
        do {
            hour += 0x3C;
            subhour(arg0);
        } while (hour < 0);
    }
    arg0->hour = (u8)hour;
    func_0012D428(arg0);
}
