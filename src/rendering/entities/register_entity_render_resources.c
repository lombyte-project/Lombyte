#include "types.h"
extern u8 tie_light_data[] __asm__("D_001E3200");
extern u8 tie_light_data2[] __asm__("D_001E4400");
extern s32 FUN_00237370();
void register_entity_render_resources(void) __asm__("FUN_00235898");

void register_entity_render_resources(void) {
    FUN_00237370(tie_light_data);
    FUN_00237370(tie_light_data2);
}
