#include "types.h"

struct MpegDecoder {
    u8 pad_0[0xDC];
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
};
struct MpegOutputSize {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
};
extern u8 D_00153980[];
extern s32 _Error(void *a, void *b);
extern s32 sprintf(char *d, char *f, ...);

s32 _isOutSizeOK(struct MpegDecoder *arg0, struct MpegOutputSize *arg1) {
    char sp_slot[0x100];
    s32 t;
    int new_var2;
    struct MpegOutputSize *new_var;
    s32 s1;
    t = arg0->unkE0;
    if (t != 0) {
        s1 = 0;
        new_var = arg1;
        if (arg0->unkDC < arg1->unk4) {
            goto tail;
        }
        new_var2 = t < arg1->unk8;
        if (arg1->unk10) {
            s1 = (t < new_var->unk8) ^ 1;
            goto tail;
        } else {
            s1 = new_var2 ^ 1;
            goto tail;
        }
    } else {
        s1 = (arg0->unkE4 < (arg1->unkC * arg1->unk10)) ^ 1;
    }
tail:
    if (s1 == 0) {
        sprintf(sp_slot, (char *)D_00153980, arg1->unk4, arg1->unk8);
        _Error(arg0, sp_slot);
    }
    return s1;
}
