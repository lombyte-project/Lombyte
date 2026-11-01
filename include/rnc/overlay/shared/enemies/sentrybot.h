#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_H

#include "types.h"

void sentrybot_update(unsigned char *) __asm__("FUN_L15_002979d8");
void set_sentrybot_alarm_duration(char *, int) __asm__("FUN_L15_002cbac0");
void mark_moby_killed_and_release_owned_voice(int) __asm__("FUN_L15_002a35d8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_H */
