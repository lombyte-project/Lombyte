#ifndef LOMBYTE_RNC_SDK_LIBMC_H
#define LOMBYTE_RNC_SDK_LIBMC_H

#include "types.h"
#include "sda.h"
#include "sifrpc.h"

/* libmc (SDK) client side. Each call takes mc_sema, sends one RPC to the
   mcserv module and leaves its function number in mc_current_func until
   sceMcSync sees the call finish and hands back mc_rpc_result[0]. */

enum McFuncNo {
    MC_FUNC_GET_INFO = 0x1,
    MC_FUNC_OPEN = 0x2,
    MC_FUNC_CLOSE = 0x3,
    MC_FUNC_SEEK = 0x4,
    MC_FUNC_READ = 0x5,
    MC_FUNC_WRITE = 0x6,
    MC_FUNC_MKDIR = 0xB,
    MC_FUNC_CHDIR = 0xC,
    MC_FUNC_GET_DIR = 0xD,
    MC_FUNC_DELETE = 0xF,
    MC_FUNC_FORMAT = 0x10,
    MC_FUNC_UNFORMAT = 0x11,
    MC_FUNC_INIT = 0xFE
};

extern s32                     mc_current_func __asm__("D_00132DA8") NOT_SDA;
extern s32                     mc_sema __asm__("D_00132DAC") NOT_SDA;
extern struct sceSifClientData mc_rpc_client __asm__("D_00159A00");
extern s32                     mc_rpc_result[] __asm__("D_0015AFC0");

#endif /* LOMBYTE_RNC_SDK_LIBMC_H */
