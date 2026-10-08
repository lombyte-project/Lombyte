#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8 D_00153980[];
extern s32 _Error(void *a, void *b);
extern s32 sprintf(char *d, char *f, ...);

s32 _isOutSizeOK(struct MpegDecoder *arg0, struct MpegRefImage *arg1) {
    char sp_slot[0x100];
    s32 t;
    int new_var2;
    struct MpegRefImage *new_var;
    s32 s1;
    t = arg0->out_max_height;
    if (t != 0) {
        s1 = 0;
        new_var = arg1;
        if (arg0->out_max_width < arg1->width) {
            goto tail;
        }
        new_var2 = t < arg1->height;
        if (arg1->unk10) {
            s1 = (t < new_var->height) ^ 1;
            goto tail;
        } else {
            s1 = new_var2 ^ 1;
            goto tail;
        }
    } else {
        s1 = (arg0->out_max_size < (arg1->unkC * arg1->unk10)) ^ 1;
    }
tail:
    if (s1 == 0) {
        sprintf(sp_slot, (char *)D_00153980, arg1->width, arg1->height);
        _Error(arg0, sp_slot);
    }
    return s1;
}
