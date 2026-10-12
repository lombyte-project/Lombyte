#include "types.h"
extern s32 space_load_id __asm__("D_0015F600");
extern s32 space_load_in_progress __asm__("D_0015F618");
extern s32 space_load_pending __asm__("D_0015F5B0");
void InitializeGlobalStateEntry(s32 value) {
    space_load_id = value;
    space_load_in_progress = space_load_pending = 1;
}
