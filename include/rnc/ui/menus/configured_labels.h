#ifndef LOMBYTE_RNC_UI_MENUS_CONFIGURED_LABELS_H
#define LOMBYTE_RNC_UI_MENUS_CONFIGURED_LABELS_H

#include "types.h"

struct LabelSelectorTable {
    s32 values[8];
};

struct LabelTextTable {
    s32 values[7];
};

/* The list rebuild_configured_text_label_list builds: at most 12 entries (it scans
   twelve selectors) and a terminating entry whose text is 0. */
struct ConfiguredLabelEntry {
    s32 text;
    u8 *flag;
    s32 first_message;
    s32 second_message;
    s32 value;
};

extern struct ConfiguredLabelEntry configured_label_entries[13] __asm__("D_001D3B10");
extern struct LabelSelectorTable label_selector_table __asm__("D_001E8728");
extern struct LabelTextTable label_text_table __asm__("D_001E8748");

#endif /* LOMBYTE_RNC_UI_MENUS_CONFIGURED_LABELS_H */
