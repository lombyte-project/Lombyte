#ifndef RCP2P_RNC_SAVE_MEMCARD_GET_DATA_SIZE_H
#define RCP2P_RNC_SAVE_MEMCARD_GET_DATA_SIZE_H

#include "types.h"

s32 memcard_get_data_size(void *) __asm__("GetDmaPacketSpanBytes");

#endif /* RCP2P_RNC_SAVE_MEMCARD_GET_DATA_SIZE_H */
