#ifndef LOMBYTE_RNC_UI_HELP_FIND_HELP_ENTRY_GLOBALS_H
#define LOMBYTE_RNC_UI_HELP_FIND_HELP_ENTRY_GLOBALS_H

#include "types.h"

/* Help entries find_help_entry searches; it returns an index below this. */
#define HELP_ENTRY_COUNT 150

extern s16 message_id_code_table[HELP_ENTRY_COUNT][2] __asm__("D_00199710");

#endif /* LOMBYTE_RNC_UI_HELP_FIND_HELP_ENTRY_GLOBALS_H */
