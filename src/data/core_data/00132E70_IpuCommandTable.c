#include "types.h"
#include "rnc/sdk/ipu_command_table.h"

u32 IpuCommandTable[16] __attribute__((section(".data"))) = {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 2, 0, 2, 0, 2, 3};
