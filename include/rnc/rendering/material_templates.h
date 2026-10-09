#ifndef LOMBYTE_RNC_RENDERING_MATERIAL_TEMPLATES_H
#define LOMBYTE_RNC_RENDERING_MATERIAL_TEMPLATES_H

#include "types.h"

/* GIF material templates, three doublewords each. The resident table is
   indexed by material mode (set_up_vis_gif_viewer reads mode * 3 .. + 2);
   the level loaders rewrite the special template for each level. */
extern u64 resident_material_templates[0x30] __asm__("D_0019E540");
extern u64 special_material_template[3] __asm__("D_0019E6C0");
extern u64 alternate_special_material_template[3] __asm__("D_0019E6D8");
extern u64 gold_weapon_texture_state[3] __asm__("D_0019E6F0");

#endif /* LOMBYTE_RNC_RENDERING_MATERIAL_TEMPLATES_H */
