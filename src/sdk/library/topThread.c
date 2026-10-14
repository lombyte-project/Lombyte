/* topThread t1: kernel thread dispatcher loop. */

#include "types.h"
#include "rnc/data_labels.h"

typedef struct ThreadEntry {
    u8 type;
    u8 id;
} ThreadEntry;

typedef struct TopThreadCtx {
    s32 counter;
    u32 pad;
    ThreadEntry entries[0x200];
} TopThreadCtx;

extern u8 D_001526E8[];

extern s32 WaitSema(s32 sema);
extern s32 WakeupThread(s32 id);
extern s32 RotateThreadReadyQueue(s32 priority);
extern s32 SuspendThread(s32 id);
extern s32 kprintf(const char *fmt, ...);

void topThread(TopThreadCtx *ctx) {
    s32 i;
    u8 type;

    for (;;) {
        WaitSema(D_00154500[0]);
        i = ctx->counter & 0x1FF;
        ctx->counter = i + 1;
        type = ctx->entries[i].type;
        switch (type) {
        case 0:
            WakeupThread(ctx->entries[i].id);
            break;
        case 1:
            RotateThreadReadyQueue(ctx->entries[i].id);
            break;
        case 2:
            SuspendThread(ctx->entries[i].id);
            break;
        default:
            kprintf((const char *)D_001526E8);
            break;
        }
    }
}
