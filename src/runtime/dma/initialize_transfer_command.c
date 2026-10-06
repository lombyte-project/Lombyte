#include "types.h"
struct Globals_001D5BF0 {
    s32 unk0;
    u8 pad_4[0x8];
    s32 unkC;
    s32 unk10;
    u8 pad_14[0xFC];
    s32 unk110;
};
extern u32 D_0015F604;
extern struct Globals_001D5BF0 D_001D5BF0;
/* Linkage correction: D_0015F604 is a u32 array element (sibling
 * FUN_00218d78 declares `extern u32 D_0015F604[]`), i.e. non-small-data.
 * The size metadata (12 = non-small under -G8; exact extent unknown) lets
 * GAS expand the compiler's symbolic store via lui $at, matching target. */
void InitializeTransferCommand(void) {
    D_001D5BF0.unkC = 0;
    D_001D5BF0.unk10 = 0;
    D_0015F604 = 3;
    D_001D5BF0.unk0 = 0x2D;
    D_001D5BF0.unk110 = 0;
}
