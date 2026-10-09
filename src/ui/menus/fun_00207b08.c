#include "types.h"
#include "rnc/globals.h"
#include "rnc/ui/map/map_state.h"

extern s32 D_0013D560[];
extern void FillTransferWords();
extern s32 func_001FA860();
extern void func_00208030();
extern void func_00208810();

void FUN_00207b08(s32 buffer) {
    s32 n;

    func_00208810();
    if (level_map_selection.unk28 == 0) {
        FillTransferWords(buffer, 0, 0x800);
        return;
    }
    n = func_001FA860(buffer, 0x800, level_map_selection.unk14, level_map_selection.mask);
    if (n == -1) {
        func_00208030(buffer);
    }
    if (D_0013D560[current_level_index] < n) {
        D_0013D560[current_level_index] = n;
    }
}

extern __typeof__(FUN_00207b08) func_00207B08 __attribute__((alias("FUN_00207b08")));
