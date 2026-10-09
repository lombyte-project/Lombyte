#ifndef LOMBYTE_RNC_SDK_LIBRARY_SDK_STATE_H
#define LOMBYTE_RNC_SDK_LIBRARY_SDK_STATE_H

#include "types.h"
#include "sda.h"

/* Status word of sceFsReset. */
extern s32 FsResetState __asm__("D_0012FC94") NOT_SDA;

/* Semaphore ids created by supplement_crt0. */
extern s32 FirstSemaphore __asm__("D_00130320") NOT_SDA;
extern s32 SecondSemaphore __asm__("D_00130324") NOT_SDA;

/* Language code the T10K console model reports. */
extern u8 ScfLanguage __asm__("D_001330D4") NOT_SDA;

/* The target keeps this ROM-name record out of the EE small-data area. */
struct RomNameState {
    s8 loaded;
    s8 padding[3];
    s8 model_code;
    s8 reserved[16];
};

extern struct RomNameState RomNameStateData __asm__("D_001330D8");

#endif /* LOMBYTE_RNC_SDK_LIBRARY_SDK_STATE_H */
