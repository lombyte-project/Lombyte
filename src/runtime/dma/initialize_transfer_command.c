#include "types.h"
#include "rnc/ui/menus/menu_system.h"
extern u32 D_0015F604;
/* Linkage correction: D_0015F604 is a u32 array element (sibling
 * FUN_00218d78 declares `extern u32 D_0015F604[]`), i.e. non-small-data.
 * The size metadata (12 = non-small under -G8; exact extent unknown) lets
 * GAS expand the compiler's symbolic store via lui $at, matching target. */
void InitializeTransferCommand(void) {
    menu_system.close_request = 0;
    menu_system.unk10 = 0;
    D_0015F604 = 3;
    menu_system.state = 0x2D;
    menu_system.update_count = 0;
}
