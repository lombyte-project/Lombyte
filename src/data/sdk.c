#include "types.h"
#include "sda.h"
#include "rnc/sdk/library/sdk_state.h"
#include "rnc/sdk/sce_dma_chan.h"

s32 FsResetState DATA_AT(0012FC94) = {0};

sceDmaChan *DmaChannels[10] DATA_AT(00132D70) = {
    (sceDmaChan *)0x10008000, (sceDmaChan *)0x10009000, (sceDmaChan *)0x1000A000,
    (sceDmaChan *)0x1000B000, (sceDmaChan *)0x1000B400, (sceDmaChan *)0x1000C000,
    (sceDmaChan *)0x1000C400, (sceDmaChan *)0x1000C800, (sceDmaChan *)0x1000D000,
    (sceDmaChan *)0x1000D400,
};

struct RomNameState RomNameStateData DATA_AT(001330D8) = {0, {0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -51, -51, -51, -51, -51, -51}};
