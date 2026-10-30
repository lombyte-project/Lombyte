#include "types.h"
#include "rnc/sdk/libgraph.h"
#include "rnc/sdk/graph/sce_gs_get_gparam.h"

typedef struct {
    s16 nSInterlace;
    u16 wMode;
    s16 nSFrame_mode;
    s16 pad06;
    s32 pad08[2];
} GsVideoModeState;

extern s32  checkModelVersion(void);
extern void InvokeKernelSyscall0080(
    s32 mode, s32 *horizontal, s32 *vertical, s32 *display_width, s32 *display_height);
extern void scePrintf(const char *format, ...);
extern char unsupported_display_mode_format[] __asm__("D_00153040");

/* The progressive arms spell this out three times: cc1 merges the divides but keeps
   each zero check, and retail's load order needs all three. */
#define MAGNIFICATION(w) (((w) + 0x9ff) / (w))

void sceGsSetDefDispEnv(struct sceGsDispEnv *output,
                        s16                  pixel_storage_format,
                        s16                  width,
                        s16                  height,
                        s16                  horizontal_offset,

                        s16 vertical_offset) {
    GsVideoModeState *state;
    s32               kernel_horizontal, kernel_vertical, kernel_width, kernel_height;
    s32               mode, scale;
    s64               scaled_horizontal;
    u64               display_width;
    u16               interlace;
    u64               display_vertical;
    u64               progressive_width;

    state = sceGsGetGParam();
    if (!((u32)(state->wMode - 2) < 2) && checkModelVersion() != 0) {
        InvokeKernelSyscall0080(
            (s16)state->wMode, &kernel_horizontal, &kernel_vertical, &kernel_width, &kernel_height);
    } else {
        kernel_height = 0;
        kernel_width = 0;
        kernel_vertical = 0;
        kernel_horizontal = 0;
    }
    mode = state->wMode;
    interlace = state->nSInterlace;

    output->pmode = 0x66;
    if (state->nSInterlace != 0) {
        if (state->nSFrame_mode == 0)
            output->smode2 = 1;
        else
            output->smode2 = 3;
    } else {
        output->smode2 = 2;
    }
    output->dispfb =
        ((u64)(pixel_storage_format & 15) << 15) | ((u64)(((width + 63) >> 6) & 63) << 9);

    if ((s16)mode == 2) {
        if ((s16)interlace == 1) {
            s32 scale;
            u64 value;
            scaled_horizontal = (s64)kernel_horizontal;
            scale = MAGNIFICATION(width);
            display_vertical = (u64)((vertical_offset + kernel_vertical + 0x32) & 0xfff) << 12;
            display_width = (u64)(s64)(scale * width - 1) << 32;
            scaled_horizontal = scaled_horizontal + 0x27c;
            scaled_horizontal = (scaled_horizontal + horizontal_offset * scale) & 0xfff;
            value = (u64)(s64)(scale - 1) << 23;
            if (state->nSFrame_mode != 0) {
                display_width = (value | display_width) |
                                ((u64)scaled_horizontal | ((u64)(s64)(height * 2 - 1) << 44));
            } else {
                display_width = (value | display_width) |
                                ((u64)scaled_horizontal | ((u64)(s64)(height - 1) << 44));
            }
            output->display = display_width | display_vertical;
        } else {
            s64 horizontal;
            u64 value;

            scale = MAGNIFICATION(width);
            progressive_width = (u64)(s64)(MAGNIFICATION(width) * width - 1) << 32;
            value = ((s64)(MAGNIFICATION(width) - 1) << 23) | progressive_width;
            horizontal = (s64)kernel_horizontal + 0x27c;
            output->display = (value | (((horizontal + horizontal_offset * scale) & 0xfff) |
                                        ((s64)(height - 1) << 44))) |
                              ((s64)((vertical_offset + kernel_vertical + 0x19) & 0xfff) << 12);
        }
    } else if ((s16)mode == 3) {
        if ((s16)interlace == 1) {
            s32 scale;
            u64 value;
            scaled_horizontal = (s64)kernel_horizontal;
            scale = MAGNIFICATION(width);
            display_vertical = (u64)((vertical_offset + kernel_vertical + 0x48) & 0xfff) << 12;
            display_width = (u64)(s64)(scale * width - 1) << 32;
            scaled_horizontal = scaled_horizontal + 0x290;
            scaled_horizontal = (scaled_horizontal + horizontal_offset * scale) & 0xfff;
            value = (u64)(s64)(scale - 1) << 23;
            if (state->nSFrame_mode != 0) {
                display_width = (value | display_width) |
                                ((u64)scaled_horizontal | ((u64)(s64)(height * 2 - 1) << 44));
            } else {
                display_width = (value | display_width) |
                                ((u64)scaled_horizontal | ((u64)(s64)(height - 1) << 44));
            }
            output->display = display_width | display_vertical;
        } else {
            s64 horizontal;
            u64 value;

            scale = MAGNIFICATION(width);
            progressive_width = (u64)(s64)(MAGNIFICATION(width) * width - 1) << 32;
            value = ((s64)(MAGNIFICATION(width) - 1) << 23) | progressive_width;
            horizontal = (s64)kernel_horizontal + 0x290;
            output->display = (value | (((horizontal + horizontal_offset * scale) & 0xfff) |
                                        ((s64)(height - 1) << 44))) |
                              ((s64)((vertical_offset + kernel_vertical + 0x24) & 0xfff) << 12);
        }
    } else if ((s16)mode == 0x50) {
        output->display =
            ((u64)(s64)(height - 1) << 44) |
            ((u64)((vertical_offset + kernel_vertical + 0x23) & 0xfff) << 12) | 0x800000ULL |
            ((u64)(((s64)((0x2d0 - width) / 2 * 2) +
                    ((s64)kernel_horizontal + ((s64)(horizontal_offset * 2) + 0xe8))) &
                   0xfff)) |
            ((u64)(s64)(width * 2 - 1) << 32);
    } else {
        scePrintf(unsupported_display_mode_format);
    }
    output->bgcolor = 0;
}
