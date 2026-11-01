#ifndef LOMBYTE_RNC_SDK_INPUT_H
#define LOMBYTE_RNC_SDK_INPUT_H

#include "types.h"

s32   scePad2CheckDma(s32);
s32   scePad2GetButtonProfile(s32, u8 *);
void *scePad2GetSide(s32);
s32   scePad2GetState(s32);
s32   scePad2LinkDriver(s32);
s32   scePad2Read(s32, u8 *);
s32   scePad2SetButtonOrder(u8 *, s32 *);
s32   sceVibGetProfile(s32, u8 *);

#endif /* LOMBYTE_RNC_SDK_INPUT_H */
