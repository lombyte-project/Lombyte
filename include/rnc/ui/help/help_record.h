#ifndef LOMBYTE_RNC_UI_HELP_HELP_RECORD_H
#define LOMBYTE_RNC_UI_HELP_HELP_RECORD_H

#include "types.h"

typedef struct {
    u16 count;
    u16 best;
    u32 flags;
} HelpRecord;

extern HelpRecord D_00141968[];

#endif /* LOMBYTE_RNC_UI_HELP_HELP_RECORD_H */
