#ifndef LOMBYTE_RNC_UI_MENUS_REGION_ENABLED_H
#define LOMBYTE_RNC_UI_MENUS_REGION_ENABLED_H

#include "types.h"
#include "sda.h"

/* One word: passes_projected_region_callback_0 reads it as a flag. The words around it
   (D_001A03AC, D_001A03B4 ...) are each a flag of another region callback. */
extern s32 region_enabled __asm__("D_001A03B0") NOT_SDA;

#endif /* LOMBYTE_RNC_UI_MENUS_REGION_ENABLED_H */
