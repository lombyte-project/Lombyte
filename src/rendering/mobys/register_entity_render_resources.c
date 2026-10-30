#include "types.h"
#include "rnc/rendering/render_globals.h"

extern u8  tie_light_data2[] __asm__("D_001E4400");
extern s32 light_ties() __asm__("FUN_00237370");
void       register_entity_render_resources(void) __asm__("FUN_00235898");

void register_entity_render_resources(void) {
    light_ties(tie_light_data);
    light_ties(tie_light_data2);
}
