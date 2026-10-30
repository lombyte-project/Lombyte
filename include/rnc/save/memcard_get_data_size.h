#ifndef LOMBYTE_RNC_SAVE_MEMCARD_GET_DATA_SIZE_H
#define LOMBYTE_RNC_SAVE_MEMCARD_GET_DATA_SIZE_H

#include "types.h"

s32 memcard_get_data_size(void *) __asm__("GetDmaPacketSpanBytes");

#endif /* LOMBYTE_RNC_SAVE_MEMCARD_GET_DATA_SIZE_H */
