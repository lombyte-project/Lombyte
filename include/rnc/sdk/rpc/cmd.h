#ifndef LOMBYTE_RNC_SDK_RPC_CMD_H
#define LOMBYTE_RNC_SDK_RPC_CMD_H

#include "types.h"

void sceSifAddCmdHandler(s32, u32, u32);
s32  _sceSifCmdIntrHdlr(void);
void sceSifExitCmd(void);
int  sceSifGetSreg(int) __asm__("func_0011A458");
void sceSifInitCmd(void);
void sceSifRemoveCmdHandler(s32);
s32  _sceSifSendCmd(void *, s32, void *, void *, s32, void *, s32);
s32  sceSifSendCmd(void *, void *, void *, s32, void *, s32);

#endif /* LOMBYTE_RNC_SDK_RPC_CMD_H */
