#include "types.h"
extern u8 D_001E2A00[];
extern s32 D_001E3000[];
extern u8 D_001E3E00[];
extern u8 D_001E4200[];
extern s32 FUN_001f98d0();
extern s32 patch_tie_texture_fields() __asm__("FUN_00235780");
void copy_render_buffer_pair(void) __asm__("FUN_00235840");

void copy_render_buffer_pair(void) {
    patch_tie_texture_fields();
    FUN_001f98d0(D_001E3000, D_001E4200, 0x200);
    FUN_001f98d0(D_001E2A00, D_001E3E00, 0x400);
    patch_tie_texture_fields();
}
