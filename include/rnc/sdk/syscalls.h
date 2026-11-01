#ifndef LOMBYTE_RNC_SDK_SYSCALLS_H
#define LOMBYTE_RNC_SDK_SYSCALLS_H

#include "types.h"

void JumpToRfuDispatch(void);
void RfuDispatchEntry(int);
void UpdateRfuDispatchState(void);

#endif /* LOMBYTE_RNC_SDK_SYSCALLS_H */
