#ifndef LOMBYTE_RNC_UI_MENUS_MENU_RESOURCE_STREAM_H
#define LOMBYTE_RNC_UI_MENUS_MENU_RESOURCE_STREAM_H

#include "types.h"

typedef struct {
    s32 values[6];
} LanguageResourceOffsets;

extern LanguageResourceOffsets menu_language_resource_offsets __asm__("D_001E87D0");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_RESOURCE_STREAM_H */
