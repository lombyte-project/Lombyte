#include "types.h"
#include "rnc/runtime/resource_table.h"


s32 LookupResourceEntry(s32 resource_index) {
    if ((u32)resource_index >= 0x40u) {
        return -3;
    }
    return ResourceTable[resource_index].value;
}
