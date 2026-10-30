#include "types.h"
#include "rnc/data_labels.h"

extern s32 sceCdSync();
extern s32 DIntr();
extern s32 EnableInterrupts();

s32 sceCdCallback(s32 callback) {
    s32 intr;
    s32 previous_callback;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    intr = DIntr();
    previous_callback = D_00159740[0];
    D_00159740[0] = callback;
    if (intr != 0) {
        EnableInterrupts();
    }
    return previous_callback;
}
