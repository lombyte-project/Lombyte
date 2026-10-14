#include "types.h"
#include "rnc/data_labels.h"

extern s32 sceCdSync();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 sceCdCallback(s32 arg0) {
    s32 intr;
    s32 old;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    intr = DIntr();
    old = D_00159740[0];
    D_00159740[0] = arg0;
    if (intr != 0) {
        EnableInterrupts();
    }
    return old;
}
