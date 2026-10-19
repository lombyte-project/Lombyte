#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/update_mpeg_frame_size/func_0012C4C8.s",
            func_0012C4C8);
#else

#include "rnc/sdk/libmpeg.h"

extern s32 InitializeReferenceImage();
extern s32 _initRefImages();
extern s32 copy_object_field_backward() __asm__("func_0012BC10");
extern s32 reserve_aligned_buffer_space() __asm__("func_0012BC20");

void func_0012C4C8(struct sceMpeg *arg0) {
    u8 *sp30;
    u8 *sp34;
    u8 *sp38;
    u8 *sp3C;
    u8 *sp40;
    u8 *var_2_53;
    s32 chroma_height;
    s32 height;
    s32 width;
    s32 temp_6_16;
    s32 var_2_40;
    u32 temp_16_77;
    s32 temp_22_48;
    s32 temp_23_51;
    s32 temp_18_75;
    u8 *temp_17_63;
    u8 *temp_19_67;
    u8 *temp_20_71;
    u8 *temp_21_73;
    struct MpegDecoder *mpeg;

    mpeg = arg0->sys;
    temp_6_16 = mpeg->unk848;
    if (temp_6_16 == 0) {
        mpeg->picture_structure = 3;
        mpeg->unk13C = 1;
        mpeg->unk140 = 1;
        mpeg->progressive_frame = 1;
        mpeg->frame_pred_frame_dct = 1;
        mpeg->matrix_coefficients = 5;
    }
    mpeg->mb_width = (s32)((s32)(mpeg->horizontal_size + 0xF) >> 4);
    if (temp_6_16 != 0) {
        if (mpeg->unk13C == 0) {
            var_2_40 = ((s32)(mpeg->vertical_size + 0x1F) >> 5) * 2;
        } else {
            goto block_6;
        }
    } else {
    block_6:
        var_2_40 = (s32)(mpeg->vertical_size + 0xF) >> 4;
    }
    mpeg->mb_height = var_2_40;
    temp_22_48 = var_2_40 << 4;
    temp_23_51 = mpeg->mb_width << 4;
    if (temp_23_51 == arg0->width) {
        if (temp_22_48 == arg0->height) {
            return;
        }
        var_2_53 = ((u8 *)mpeg + 0x528);
    } else {
        var_2_53 = ((u8 *)mpeg + 0x528);
    }
    temp_17_63 = var_2_53 - 0x420;
    {
        arg0->height = temp_22_48;
        arg0->width = temp_23_51;
        sp30 = ((u8 *)mpeg + (0x320));
        temp_19_67 = ((u8 *)mpeg + (0x1E8));
        sp34 = ((u8 *)mpeg + (0x388));
        temp_20_71 = ((u8 *)mpeg + (0x250));
        temp_21_73 = ((u8 *)mpeg + (0x2B8));
        sp38 = ((u8 *)mpeg + (0x3F0));
        temp_16_77 = ((0x180U * (u32)temp_23_51) * (u32)temp_22_48) >> 8;
        sp3C = ((u8 *)mpeg + (0x458));
        sp40 = ((u8 *)mpeg + (0x4C0));
        width = temp_23_51;
        height = temp_22_48;
        copy_object_field_backward(temp_17_63);
        mpeg->frame_buffers[0] = reserve_aligned_buffer_space(mpeg, temp_17_63, temp_16_77, 0x40);
        mpeg->frame_buffers[1] = reserve_aligned_buffer_space(mpeg, temp_17_63, temp_16_77, 0x40);
        mpeg->frame_buffers[2] = reserve_aligned_buffer_space(mpeg, temp_17_63, temp_16_77, 0x40);
        _initRefImages(temp_19_67, temp_20_71, temp_21_73, sp30, sp34, sp38, sp3C, sp40, var_2_53,
                       mpeg->frame_buffers[0], mpeg->frame_buffers[1], mpeg->frame_buffers[2], width,
                       height);
        InitializeReferenceImage(temp_19_67, width, height);
        InitializeReferenceImage(temp_20_71, width, height);
        InitializeReferenceImage(temp_21_73, width, height);
        temp_18_75 = height >> 1;
        chroma_height = temp_18_75;
        InitializeReferenceImage(sp30, width, chroma_height);
        InitializeReferenceImage(sp34, width, chroma_height);
        InitializeReferenceImage(sp38, width, chroma_height);
        InitializeReferenceImage(sp3C, width, chroma_height);
        InitializeReferenceImage(sp40, width, chroma_height);
        InitializeReferenceImage(var_2_53, width, chroma_height);
    }
}
#endif /* NON_MATCHING */
