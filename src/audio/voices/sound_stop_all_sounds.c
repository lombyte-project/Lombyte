#include "types.h"
#include "eetypes.h"
#include "qzero.h"

typedef struct {
    s32 x0;
    u8 x4;
    u8 pad5[0x6B];
} SndVoice;

typedef struct {
    u128 q[4];
    s32 x40;
    u8 pad44[0x2C];
    SndVoice voices[30];
} SndState;

extern SndState D_0013E550;
extern s32 func_0012DC80();
extern s32 func_0012E3B8();
extern s32 func_0012EB00();

void sound_stop_all_sounds(void) __asm__("FUN_0022dcd0");
void sound_stop_all_sounds(void) {
    s32 i;
    SndState *s;
    u128 *q;
    s32 p;
    s32 end;

    func_0012EB00();
    func_0012DC80();
    func_0012E3B8();
    while (func_0012DC80() != 0) {
    }
    for (i = 0; i < 4; i++) {
        qzero(&D_0013E550.q[i]);
    }
    s = &D_0013E550;
    s->x40 = 0;
    p = (s32)s;
    end = p + 0xD20;
    do {
        ((SndVoice *)p)[1].x0 = 0;
        ((SndVoice *)p)[1].x4 = 0;
        p += 0x70;
    } while (p < end);
}

extern __typeof__(sound_stop_all_sounds) func_0022DCD0 __attribute__((alias("FUN_0022dcd0")));
