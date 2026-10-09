#ifndef LOMBYTE_RNC_SDK_IPU_COMMAND_TABLE_H
#define LOMBYTE_RNC_SDK_IPU_COMMAND_TABLE_H

#include "types.h"
#include "sda.h"

/* IPU command results, indexed by the command's top nibble. */
extern u32 IpuCommandTable[16] __asm__("D_00132E70") NOT_SDA;

#endif /* LOMBYTE_RNC_SDK_IPU_COMMAND_TABLE_H */
