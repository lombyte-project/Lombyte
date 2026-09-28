#include "types.h"
typedef struct { s32 port; s32 slot; s32 number; u8 reserve[4]; } scePad2SocketParam;
struct PadState { u8 pad0[0x194]; s32 socket; s32 unk198; s32 unk19C; };
extern scePad2SocketParam D_001CD760;
extern struct PadState D_0013C940;
extern s32 sceDbcInit(void);
extern s32 scePad2Init(s32);
extern s32 scePad2CreateSocket(scePad2SocketParam *, void *);
void init_pads(void) __asm__("FUN_00217048");

void init_pads(void) {
    sceDbcInit();
    scePad2Init(0);
    D_001CD760.port = 2;
    D_001CD760.slot = 0;
    D_0013C940.socket = scePad2CreateSocket(&D_001CD760, &D_0013C940);
    D_001CD760.port = 2;
    D_001CD760.slot = 1;
    D_0013C940.unk198 = 0;
    D_0013C940.unk19C = 0;
}

extern __typeof__(init_pads) func_00217048 __attribute__((alias("FUN_00217048")));
