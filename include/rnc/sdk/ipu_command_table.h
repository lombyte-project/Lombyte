#ifndef RCP2P_RNC_SDK_IPU_COMMAND_TABLE_H
#define RCP2P_RNC_SDK_IPU_COMMAND_TABLE_H

#include "types.h"
#include "sda.h"

/* IPU command results, indexed by the command's top nibble. */
extern u32 IpuCommandTable[16] __asm__("D_00132E70") NOT_SDA;

#endif /* RCP2P_RNC_SDK_IPU_COMMAND_TABLE_H */
