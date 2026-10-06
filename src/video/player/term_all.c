#include "types.h"
struct AudioSystemState {
    u8 pad_0[0xD90F8];
    s32 unkD90F8;
    s32 unkD90FC;
};
extern struct AudioSystemState *D_0016120C;
extern s32 D_00161210;
extern s32 DeleteThread();
extern s32 RemoveDmacHandler();
extern s32 RemoveIntcHandler();
extern s32 TerminateThread();
extern s32 disable_intc() __asm__("func_00119028");
extern s32 disable_dmac() __asm__("func_001190F8");
extern s32 audio_dec_delete() __asm__("func_0023AC90");
extern void func_0023B958();
extern s32 func_0023BA58();
extern s32 video_dec_delete() __asm__("func_0023CC38");
extern s32 func_0023D1E0();
extern s32 sceCdSync();
void term_all(void) __asm__("FUN_0023aa68");

void term_all(void) {
    sceCdSync(0);
    func_0023B958(D_0016120C);
    func_0023D1E0((u8 *)D_0016120C + 0xD9168);

    TerminateThread(D_00161210);
    DeleteThread(D_00161210);
    disable_dmac(2);
    RemoveDmacHandler(2, *(s32 *)((u8 *)D_0016120C + 0xD90F8));
    disable_intc(2);
    RemoveIntcHandler(2, *(s32 *)((u8 *)D_0016120C + 0xD90FC));
    video_dec_delete((u8 *)D_0016120C + 0xD9048);
    audio_dec_delete((u8 *)D_0016120C + 0xD9100);
    func_0023BA58((u8 *)D_0016120C + 0xD9040);
    sceCdSync(0);
    *(s32 *)0x1000E000 &= 0xFFFFFFFD;
}

extern __typeof__(term_all) func_0023AA68 __attribute__((alias("FUN_0023aa68")));
