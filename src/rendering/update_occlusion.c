#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
extern u32 occl_update[] __asm__("D_0018C334");
extern s32 build_occlusion_visibility() __asm__("FUN_001f2820");
extern s32 fill_transfer_words() __asm__("FUN_001f97e8");
void update_occlusion(void) __asm__("FUN_001f2c10");

void update_occlusion(void) {
    if (occl_update[0] == 0) {
        fill_transfer_words(occl_visibility, -1, 0x80);
    } else if (occl_update[0] == 2) {
        build_occlusion_visibility();
    }
}

/* Recovered original symbol name. */
extern __typeof__(update_occlusion) UpdateOcclusion__Fv __attribute__((alias("FUN_001f2c10")));
