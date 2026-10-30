/* checkModelVersion c2: read the model-version string and validate its range. */

#include "types.h"

extern s32 sceOpen(const char *path, s32 flags);
extern s32 sceRead(s32 fd, void *buf, s32 size);
extern s32 sceClose(s32 fd);
extern s32 atoi(const char *str) __asm__("CheckStateRange");
const u8   D_00153030[] = "rom0:ROMVER";

s32 checkModelVersion(void) {
    s32 fd;
    u32 i;
    s8  rom_version_string[0x100];

    fd = sceOpen((const char *)D_00153030, 1);
    if (fd < 0)
        return -1;

    for (i = 0; i < 0x100; i++) {
        sceRead(fd, &rom_version_string[i], 1);
        if (rom_version_string[i] == 0)
            break;
    }

    sceClose(fd);
    return 0x1315670 < atoi((const char *)&rom_version_string[i - 9]);
}
