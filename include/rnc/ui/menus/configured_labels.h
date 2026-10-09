#ifndef LOMBYTE_RNC_UI_MENUS_CONFIGURED_LABELS_H
#define LOMBYTE_RNC_UI_MENUS_CONFIGURED_LABELS_H

#include "types.h"

struct LabelSelectorTable {
    s32 values[8];
};

struct LabelTextTable {
    s32 values[7];
};

extern struct LabelSelectorTable label_selector_table __asm__("D_001E8728");
extern struct LabelTextTable label_text_table __asm__("D_001E8748");

#endif /* LOMBYTE_RNC_UI_MENUS_CONFIGURED_LABELS_H */
