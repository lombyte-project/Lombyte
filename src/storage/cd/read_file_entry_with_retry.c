#include "types.h"
#include "rnc/sdk/libcdvd.h"
struct FileEntry {
    u32 lsn;
    u32 size;
};
extern u8 D_0015ED58;
extern struct FileEntry D_0013A448[];
extern u8 D_0013A4E0[];
extern s32 sceCdRead(u32, u32, void *, sceCdRMode *);
extern s32 sceCdSync(s32);
extern s32 sceCdGetError(void);
extern s32 sceGsSyncV(s32);
s32 read_file_entry_with_retry(s32 file) __asm__("FUN_0012f368");

s32 read_file_entry_with_retry(s32 file) {
    u8 buf[0x2800];
    sceCdRMode mode;
    struct FileEntry *e;
    u32 i;

    mode.trycount = 0x20;
    mode.spindlctrl = D_0015ED58;
    mode.datapattern = 0;
    mode.pad = 0;
    e = &D_0013A448[file];
    do {
        sceCdRead(e->lsn, 5, buf, &mode);
        while (sceCdSync(1) != 0) {
            sceGsSyncV(0);
        }
    } while (sceCdGetError() != 0);
    for (i = 0; i < 0x2434; i++) {
        D_0013A4E0[i] = buf[i];
    }
    return 1;
}

extern __typeof__(read_file_entry_with_retry) func_0012F368 __attribute__((alias("FUN_0012f368")));
