#ifndef RCP2P_RNC_SDK_SYSCALLS_H
#define RCP2P_RNC_SDK_SYSCALLS_H

#include "types.h"

void JumpToRfuDispatch(void);
void RfuDispatchEntry(int);
void UpdateRfuDispatchState(void);

#endif /* RCP2P_RNC_SDK_SYSCALLS_H */
