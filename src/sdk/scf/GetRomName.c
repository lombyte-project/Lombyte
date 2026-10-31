#include "types.h"
#include "rnc/sdk/library/sdk_state.h"
#include "rnc/sdk/deci2/sce_printf.h"
#include "rnc/sdk/storage/fileio/sceClose.h"
#include "rnc/sdk/storage/fileio/sceRead.h"
#include "rnc/sdk/storage/fileio/sceopen.h"

const u8  D_00153C00[] = "rom0:ROMVER";
extern u8 rom_version_open_error_message[] __asm__("D_00153C10");
extern u8 rom_version_read_error_message[] __asm__("D_00153C28");

s8 *GetRomName(void) {
    s32 file_descriptor;

    if (RomNameStateData.loaded == 0) {
        file_descriptor = sceOpen(D_00153C00, 1);
        if (file_descriptor == -1) {
            scePrintf(rom_version_open_error_message, &RomNameStateData);
        }
        if (sceRead(file_descriptor, &RomNameStateData, 0xE) == -1) {
            scePrintf(rom_version_read_error_message);
        }
        sceClose(file_descriptor);
    }
    return (s8 *)&RomNameStateData;
}
