#ifndef LOMBYTE_RNC_SDK_LIBRARY_SIF_FILE_SLOT_H
#define LOMBYTE_RNC_SDK_LIBRARY_SIF_FILE_SLOT_H

#include "types.h"
#include "sda.h"
#include "sifrpc.h"

/* One open-file slot of the SIF file I/O wrappers. */
struct SifFileSlot {
    s32 fd;
    s32 flags; /* 0: free; new_iob marks a taken slot with SIF_FILE_SLOT_TAKEN */
    s32 reserved8;
    s32 reservedC;
};

#define SIF_FILE_SLOT_TAKEN 0x10000000
#define SIF_FILE_SLOT_COUNT 32

extern s32 fio_async_semas[] __asm__("D_0012FC10");
extern s32 fio_wait_sema __asm__("D_0012FC9C") NOT_SDA;
extern s32 fio_iob_sema __asm__("D_0012FCA0") NOT_SDA;         /* guards fio_iob */
extern s32 fio_async_table_sema __asm__("D_0012FCA4") NOT_SDA; /* guards fio_async_semas */
extern u8  fio_rpc_result[] __asm__("D_001574C0");             /* read through kseg1 */
extern struct SifFileSlot      fio_iob[SIF_FILE_SLOT_COUNT] __asm__("D_00157D80");
extern struct sceSifClientData fio_rpc_client __asm__("D_00157F80");

#endif /* LOMBYTE_RNC_SDK_LIBRARY_SIF_FILE_SLOT_H */
