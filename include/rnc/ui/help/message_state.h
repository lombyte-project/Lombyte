#ifndef LOMBYTE_RNC_UI_HELP_MESSAGE_STATE_H
#define LOMBYTE_RNC_UI_HELP_MESSAGE_STATE_H

#include "types.h"
#include "rnc/ui/help/find_help_entry_globals.h"

/* The help entries shown so far, oldest first: push_help_history moves an
   entry already listed to the end, so each find_help_entry index appears
   at most once. */
extern u8  active_message_records[HELP_ENTRY_COUNT] __asm__("D_00141E08");
extern s32 active_message_record_count __asm__("D_0015EE30");

#endif /* LOMBYTE_RNC_UI_HELP_MESSAGE_STATE_H */
