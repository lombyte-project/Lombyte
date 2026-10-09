#include "types.h"
#include "sda.h"
#include "rnc/runtime/core_state.h"
#include "rnc/runtime/resource_table.h"

s32 GlobalStateResource DATA_AT(0012F76C) = 0x12f480;

s32 CoreGlobalWord DATA_AT(0012FBF0) = {0};

s32 RpcCommandState DATA_AT(0012FC08) = {0};

ResourceEntry ResourceTable[64] DATA_AT(001DD1D8) = {0};
