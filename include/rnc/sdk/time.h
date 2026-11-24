#ifndef RCP2P_RNC_SDK_TIME_H
#define RCP2P_RNC_SDK_TIME_H

#include "types.h"

void AddClockHour() __asm__("addhour");
u8   BcdToTime(u8);
void Subhour() __asm__("subhour");
u32  TimeToBcd(u32);

#endif /* RCP2P_RNC_SDK_TIME_H */
