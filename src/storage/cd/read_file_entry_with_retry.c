#include "types.h"
#include "rnc/sdk/libcdvd.h"
#include "rnc/data_labels.h"
struct FileEntry {
    u32 lsn;
    u32 size;
};
extern struct FileEntry D_0013A448[];
extern s32 sceCdRead(u32, u32, void *, sceCdRMode *);
extern s32 sceCdSync(s32);
extern s32 sceCdGetError(void);
extern s32 sceGsSyncV(s32);
s32 read_file_entry_with_retry(s32 file) __asm__("FUN_0012f368");

s32 read_file_entry_with_retry(s32 file) {
    u8 cd_read_buffer[0x2800];
    sceCdRMode mode;
    struct FileEntry *file_entry;
    u32 i;

    mode.trycount = 0x20;
    mode.spindlctrl = D_0015ED58;
    mode.datapattern = 0;
    mode.pad = 0;
    file_entry = &D_0013A448[file];
    do {
        sceCdRead(file_entry->lsn, 5, cd_read_buffer, &mode);
        while (sceCdSync(1) != 0) {
            sceGsSyncV(0);
        }
    } while (sceCdGetError() != 0);
    for (i = 0; i < 0x2434; i++) {
        D_0013A4E0[i] = cd_read_buffer[i];
    }
    return 1;
}

extern __typeof__(read_file_entry_with_retry) func_0012F368 __attribute__((alias("FUN_0012f368")));
