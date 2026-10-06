#include "types.h"
#include "asm.h"

#include "types.h"
#include "sda.h"

struct LabelSelectorTable {
    s32 values[8];
};

struct LabelTextTable {
    s32 values[7];
};

struct ConfiguredLabelEntry {
    s32 text;
    u8 *flag;
    s32 first_message;
    s32 second_message;
    s32 value;
};

extern struct LabelSelectorTable label_selector_table __asm__("D_001E8728");
extern struct LabelTextTable label_text_table __asm__("D_001E8748");
extern u8 selector_available[] __asm__("D_0015EDC0") MACRO_ADDR;
extern u8 selector_values[] __asm__("D_0015EDB0") MACRO_ADDR;
extern struct ConfiguredLabelEntry configured_label_entries[] __asm__("D_001D3B10");

s32 rebuild_configured_text_label_list(void) __asm__("FUN_0021a1e0");

s32 rebuild_configured_text_label_list(void) {
    struct LabelSelectorTable selectors;
    struct LabelTextTable text_ids;
    struct ConfiguredLabelEntry *entry;
    int repeat;
    s32 entry_count;
    s32 source_index;
    s32 selector;

    repeat = 0;
    selectors = label_selector_table;
    text_ids = label_text_table;
    do {
        entry_count = 0;
        do {
            /* Retail scans twelve selectors despite copying eight selector words and
       seven text words. Preserve that original overrun while this stays pending. */
            for (source_index = 0; source_index < 12; source_index++) {
                selector = selectors.values[source_index];
                if (selector != -1 && selector_available[selector] != 0) {
                    configured_label_entries[entry_count].flag = &selector_values[selector];
                    configured_label_entries[entry_count].text = selectors.values[source_index + 8];
                    configured_label_entries[entry_count].first_message = 0x4F5A;
                    configured_label_entries[entry_count].second_message = 0x4F5B;
                    configured_label_entries[entry_count].value = 0;
                    entry_count++;
                }
            }
        } while (0);
    } while (repeat);
    configured_label_entries[entry_count].text = 0;
    return 0;
}

extern __typeof__(rebuild_configured_text_label_list) func_0021A1E0
    __attribute__((alias("FUN_0021a1e0")));
