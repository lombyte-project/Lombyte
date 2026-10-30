#ifndef LOMBYTE_RNC_SDK_LIBCDVD_H
#define LOMBYTE_RNC_SDK_LIBCDVD_H

#include "types.h"
#include "sda.h"
#include "sifrpc.h"

/* Read mode passed to sceCdRead: retry count, spindle control and the
   sector data pattern. */
typedef struct sceCdRMode {
    u8 trycount;
    u8 spindlctrl;
    u8 datapattern;
    u8 pad;
} sceCdRMode;

/* Client state. N commands (sceCdRead) and S commands each take their
   semaphore and use their own RPC client; *_keep_cmd is the command that
   holds the semaphore, printed when another one finds it taken. */
extern s32          cd_debug __asm__("D_001312D0") NOT_SDA; /* > 0: print errors */
extern s32          cd_scmd_keep_cmd __asm__("D_001312D8") NOT_SDA;
extern s32          cd_ncmd_keep_cmd __asm__("D_001312DC") NOT_SDA;
extern volatile s32 cd_callback_sema __asm__("D_001312E0") NOT_SDA;
extern volatile s32 cd_ncmd_sema __asm__("D_001312E8") NOT_SDA;
extern volatile s32 cd_scmd_sema __asm__("D_001312EC") NOT_SDA;
extern volatile s32
    cd_read_pending __asm__("D_001312F0") NOT_SDA; /* sceCdRead until its callback */
extern struct sceSifClientData cd_ncmd_client __asm__("D_00132490");
extern u8                      cd_scmd_result[] __asm__("D_001324C0"); /* read through kseg1 */
extern struct sceSifClientData cd_scmd_client __asm__("D_00132D08");

#endif /* LOMBYTE_RNC_SDK_LIBCDVD_H */
