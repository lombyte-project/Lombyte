#include "types.h"
#include "rnc/input/pad_state.h"
typedef struct {
    s32 port;
    s32 slot;
    s32 number;
    u8 reserve[4];
} scePad2SocketParam;
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
    D_0013C940.profile_state = 0;
    D_0013C940.device_state = 0;
}

extern __typeof__(init_pads) func_00217048 __attribute__((alias("FUN_00217048")));
