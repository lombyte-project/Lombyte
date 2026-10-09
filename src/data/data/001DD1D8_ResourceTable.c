#include "types.h"
#include "rnc/runtime/resource_table.h"

ResourceEntry ResourceTable[64] __attribute__((section(".data"))) = {0};
