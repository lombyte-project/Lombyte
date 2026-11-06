#ifndef LOMBYTE_RNC_UI_HELP_HELP_RECORD_H
#define LOMBYTE_RNC_UI_HELP_HELP_RECORD_H

#include "types.h"

typedef struct {
    u16 count;
    u16 latest_time_bucket;
    u32 level_flags;
} HelpRecord;

/* One record per help entry: the level code's clear_help_record_high_bits
   walks all 0x94 of them. */
#define HELP_RECORD_COUNT 0x94

extern HelpRecord D_00141968[HELP_RECORD_COUNT] __asm__("D_00141968");

#endif /* LOMBYTE_RNC_UI_HELP_HELP_RECORD_H */
