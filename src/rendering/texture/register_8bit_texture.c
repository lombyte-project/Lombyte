#include "types.h"
#include "sda.h"
#include "rnc/rendering/texture_upload.h"



extern s32 pending_texture_upload_count_macro __asm__("D_0015F458") MACRO_ADDR;
/* Registers an 8-bit texture: returns its GS TEX0 value (tbp = tex >> 8,
   tbw = 1 << max(tw - 6, 0), PSMT8, tw, th, tcc 1, cbp = clut >> 8,
   cld 4) and, while there is room (64 entries), records it in the next
   D_0018D040 slot. The tcc term is the one unsigned field: it keeps fold
   from re-pairing the constants, which gives retail's OR tree. */
long register_8bit_texture(s32 tw, s32 th, s32 clut_data, s32 image_data, s32 cbp, s32 tbp) __asm__("FUN_00204e30");

long register_8bit_texture(s32 tw, s32 th, s32 clut_data, s32 image_data, s32 cbp, s32 tbp) {
    s32 w;
    long reg;

    w = tw - 6;
    if (w < 0) {
        w = 0;
    }
    w = 1 << w;
    cbp >>= 8;
    tbp >>= 8;
    reg = (long)tbp | ((long)w << 14) | ((long)0x13 << 20) | ((long)tw << 26) |
          ((unsigned long)1 << 34) | ((long)cbp << 37) | ((long)4 << 61) | ((long)th << 30);
    if (pending_texture_upload_count_macro < TEXTURE_UPLOAD_MAX) {
        pending_texture_uploads[pending_texture_upload_count_macro].clut_data = clut_data;
        pending_texture_uploads[pending_texture_upload_count_macro].cbp = cbp;
        pending_texture_uploads[pending_texture_upload_count_macro].unk4 = 0;
        pending_texture_uploads[pending_texture_upload_count_macro].image_data = image_data;
        pending_texture_uploads[pending_texture_upload_count_macro].tw = tw;
        pending_texture_uploads[pending_texture_upload_count_macro].th = th;
        pending_texture_uploads[pending_texture_upload_count_macro].tbp = tbp;
        pending_texture_upload_count_macro++;
    }
    return reg;
}

extern __typeof__(register_8bit_texture) func_00204E30 __attribute__((alias("FUN_00204e30")));
