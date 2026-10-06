#include "types.h"

extern s32 GetOsdConfigParam();
extern s32 SetOsdConfigParam();

s32 PatchIsNeeded(void) {
    union {
        u32 words[4];
        struct {
            u32 a;
            u32 b;
            u8 pad[8];
        } f;
    } cfg;

    GetOsdConfigParam(&cfg);
    cfg.f.b = (cfg.f.a & 0xFFFF1FFF) | 0x2000;
    SetOsdConfigParam(&cfg.f.b);
    GetOsdConfigParam(&cfg.f.b);
    SetOsdConfigParam(&cfg);
    return ((cfg.f.b >> 13) & 7) == 0;
}
