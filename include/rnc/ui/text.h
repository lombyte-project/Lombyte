#ifndef LOMBYTE_RNC_UI_TEXT_H
#define LOMBYTE_RNC_UI_TEXT_H

#include "types.h"

struct MenuScreen;

void link_localized_display_text(void) __asm__("FUN_001fdd58");
s32  rebuild_configured_text_label_list(void) __asm__("FUN_0021a1e0");
s32  render_localized_ui_entry_list(struct MenuScreen *) __asm__("FUN_0021b1c8");

#endif /* LOMBYTE_RNC_UI_TEXT_H */
