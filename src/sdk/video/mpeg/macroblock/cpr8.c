#include "types.h"
#include "rnc/sdk/libmpeg.h"

struct MpegCopyParams {
    s32 unk0;
    u8  pad_4[0x8];
    s32 unkC;
    s32 unk10;
};

struct MpegDecoderFrame {
    u8  pad_0[0xE0];
    s32 unkE0;
    s32 unkE4;
    u8  pad_E8[0x8C];
    s32 unk174;
};

extern s32 DIntr();
extern s32 EnableInterrupts();

void _cpr8(struct MpegDecoder *decoder, struct MpegCopyParams *copy_params) {
    struct MpegDecoderFrame *decoder_transfer_layout;
    s32                      transfer_pass_count;
    s32                      destination_pass_base;
    s32                      interrupts_on2;
    s32                      interrupts_on;
    s32                      layout_width;
    s32                      row;
    s32                      dest_addr;
    s32                      source_addr;
    s32                      qword_count;
    s32                      source_stride;
    s32                      dest_stride;
    s32                      dest_width;
    s32                      pass;

    decoder_transfer_layout = (struct MpegDecoderFrame *)decoder;
    source_addr = copy_params->unk0 & 0x0FFFFFFF;
    destination_pass_base = decoder->unkD8 & 0x0FFFFFFF;
    if (decoder_transfer_layout->unk174 == 3) {
        goto block_3;
    }
    layout_width = decoder_transfer_layout->unkE0;
    dest_width = layout_width;
    if (layout_width != 0) {
        goto block_8;
    }
    goto block_4;
block_3:
    dest_width = decoder_transfer_layout->unkE0;
block_4:
    source_stride = copy_params->unk10 * 0x180;
    qword_count = source_stride >> 4;
    if (dest_width == 0) {
        goto block_6;
    }
    dest_stride = (dest_width >> 4) * 0x180;
    goto block_7;
block_6:
    dest_stride = source_stride;
block_7:
    transfer_pass_count = 1;
    goto block_9;
block_8:
    dest_stride = (layout_width >> 4) * 0xC0;
    source_stride = ((s32)copy_params->unk10 >> 1) * 0x180;
    transfer_pass_count = 2;
    qword_count = source_stride >> 4;
block_9:
    pass = 0;
    while (pass < transfer_pass_count) {
        dest_addr = destination_pass_base;
        for (row = 0; row < copy_params->unkC; row++) {
            interrupts_on = DIntr();
            *(volatile s32 *)0x1000D480 = 0;
            *(volatile s32 *)0x1000D410 = source_addr;
            *(volatile s32 *)0x1000D420 = qword_count;
            *(volatile s32 *)0x1000D400 = 0x101;
            if (interrupts_on == 0) {
                goto block_15;
            }
            EnableInterrupts();
        block_15:
            while (*(volatile u32 *)0x1000D400 & 0x100) {
            }
            interrupts_on2 = DIntr();
            *(volatile s32 *)0x1000D080 = 0;
            *(volatile s32 *)0x1000D010 = dest_addr;
            *(volatile s32 *)0x1000D020 = qword_count;
            *(volatile s32 *)0x1000D000 = 0x100;
            if (interrupts_on2 == 0) {
                goto block_19;
            }
            EnableInterrupts();
        block_19:
            while (*(volatile u32 *)0x1000D000 & 0x100) {
            }
            while (*(volatile u32 *)0x1000D020 != 0) {
            }
            dest_addr += dest_stride;
            source_addr += source_stride;
        }
        pass++;
        destination_pass_base += decoder_transfer_layout->unkE4 * 0xC0;
    }
    return;
}
