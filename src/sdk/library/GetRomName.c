#include "types.h"
#include "rnc/sdk/library/sdk_state.h"

extern u8 D_00153C00[];
extern u8 D_00153C10[];
extern u8 D_00153C28[];
extern s32 sceClose();
extern s32 sceOpen();
extern s32 scePrintf();
extern s32 sceRead();

s8 *GetRomName(void) {
    s32 file_descriptor;

    if (RomNameStateData.loaded == 0) {
        file_descriptor = sceOpen(D_00153C00, 1);
        if (file_descriptor == -1) {
            scePrintf(D_00153C10, &RomNameStateData);
        }
        if (sceRead(file_descriptor, &RomNameStateData, 0xE) == -1) {
            scePrintf(D_00153C28);
        }
        sceClose(file_descriptor);
    }
    return (s8 *)&RomNameStateData;
}
