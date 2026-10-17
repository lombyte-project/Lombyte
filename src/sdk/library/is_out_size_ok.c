#include "types.h"
#include "rnc/sdk/libmpeg.h"

const u8 D_00153980[] = "Too small buffer size for %dx%d picture\n";
extern s32 _Error(void *a, void *b);
extern s32 sprintf(char *d, char *f, ...);

s32 _isOutSizeOK(struct MpegDecoder *decoder, struct MpegRefImage *reference_image) {
    char sp_slot[0x100];
    s32 t;
    int new_var2;
    struct MpegRefImage *new_var;
    s32 out_size_ok;
    t = decoder->out_max_height;
    if (t != 0) {
        out_size_ok = 0;
        new_var = reference_image;
        if (decoder->out_max_width < reference_image->width) {
            goto tail;
        }
        new_var2 = t < reference_image->height;
        if (reference_image->unk10) {
            out_size_ok = (t < new_var->height) ^ 1;
            goto tail;
        } else {
            out_size_ok = new_var2 ^ 1;
            goto tail;
        }
    } else {
        out_size_ok = (decoder->out_max_size < (reference_image->unkC * reference_image->unk10)) ^ 1;
    }
tail:
    if (out_size_ok == 0) {
        sprintf(sp_slot, (char *)D_00153980, reference_image->width, reference_image->height);
        _Error(decoder, sp_slot);
    }
    return out_size_ok;
}
