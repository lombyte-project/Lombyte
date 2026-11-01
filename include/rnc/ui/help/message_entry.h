#ifndef LOMBYTE_RNC_UI_HELP_MESSAGE_ENTRY_H
#define LOMBYTE_RNC_UI_HELP_MESSAGE_ENTRY_H

#include "types.h"

/* A relocated RC1 help-text entry; the on-disc first word is an offset.
 * Lookup compares message_id; audio uses 30000 + audio_track_offset.
 * The last word has no established meaning and must remain unnamed. */
typedef struct HelpMessageEntry {
    void *text;               /* 0x00 */
    s32   message_id;         /* 0x04 */
    s32   audio_track_offset; /* 0x08: -1 means no audio */
    s32   unknown_0C;
} HelpMessageEntry;
#endif /* LOMBYTE_RNC_UI_HELP_MESSAGE_ENTRY_H */
