#ifndef LOMBYTE_RNC_RENDERING_INITIALIZE_MENU_PREVIEW_SCENE_GLOBALS_H
#define LOMBYTE_RNC_RENDERING_INITIALIZE_MENU_PREVIEW_SCENE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/rendering/initialize_menu_preview_scene.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern u8 menu_preview_lighting_scale_source __asm__("D_001601C0");
extern u8 menu_preview_lighting_copy_source __asm__("D_001601D0");

#endif /* LOMBYTE_RNC_RENDERING_INITIALIZE_MENU_PREVIEW_SCENE_GLOBALS_H */
