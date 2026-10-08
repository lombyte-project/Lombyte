#include "asm.h"

/* Exact SDK/library unit sceGsSetDefDispEnv; symbolic expected assembly retained pending source recovery. */

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/dma/sce_gs_set_def_disp_env/sceGsSetDefDispEnv.s",
            sceGsSetDefDispEnv);
#else
#include "types.h"
#include "rnc/sdk/libgraph.h"
typedef struct {
    s16 nSInterlace;
    u16 wMode;
    s16 nSFrame_mode;
    s16 pad06;
    s32 pad08[2];
} GsVideoModeState;

extern GsVideoModeState *GetCoreDataTable(void);
extern s32 checkModelVersion(void);
extern void InvokeKernelSyscall0080(s32 mode, s32 *horizontal, s32 *vertical,
                                     s32 *display_width, s32 *display_height);
extern void scePrintf(const char *format, ...);

void sceGsSetDefDispEnv(struct sceGsDispEnv *output, s16 pixel_storage_format, s16 width,
                        s16 height, s16 horizontal_offset, s16 vertical_offset) {
    GsVideoModeState *state;
    s32 kernel_horizontal, kernel_vertical, kernel_width, kernel_height;
    s32 mode, scale;
    u16 interlace;
    u64 value;

    state = GetCoreDataTable();
    if ((u32)(state->wMode - 2) >= 2 && checkModelVersion() != 0) {
        InvokeKernelSyscall0080((s16)state->wMode, &kernel_horizontal, &kernel_vertical,
                                 &kernel_width, &kernel_height);
    } else {
        kernel_height = 0;
        kernel_width = 0;
        kernel_vertical = 0;
        kernel_horizontal = 0;
    }
    mode = state->wMode;
    interlace = state->nSInterlace;

    output->pmode = 0x66;
    value = 2;
    if (interlace != 0) {
        value = 3;
        if (state->nSFrame_mode == 0) value = 1;
    }
    output->smode2 = value;
    output->dispfb = ((u64)(pixel_storage_format & 15) << 15) |
                     ((u64)((width + 63) & 0xfc0) << 3);

    if (mode == 2) {
        if (interlace == 1) {
            scale = (width + 0x9ff) / width;
            value = ((u64)(s64)(width * scale - 1) << 32) |
                    ((u64)(s64)(scale - 1) << 23) |
                    ((u64)(((s64)(horizontal_offset * scale) + ((s64)kernel_horizontal + 0x27c)) & 0xfff)) |
                    ((u64)((vertical_offset + kernel_vertical + 0x32) & 0xfff) << 12);
            if (state->nSFrame_mode == 0) {
                value |= (u64)(s64)(height - 1) << 44;
            } else {
                value |= (u64)(s64)(height * 2 - 1) << 44;
            }
        } else {
            scale = (width + 0x9ff) / width;
            value = ((u64)(s64)(height - 1) << 44) |
                    ((u64)(s64)(width * scale - 1) << 32) |
                    ((u64)(s64)(scale - 1) << 23) |
                    ((u64)(((s64)(horizontal_offset * scale) + ((s64)kernel_horizontal + 0x27c)) & 0xfff)) |
                    ((u64)((vertical_offset + kernel_vertical + 0x19) & 0xfff) << 12);
        }
        output->display = value;
    } else if (mode == 3) {
        if (interlace == 1) {
            scale = (width + 0x9ff) / width;
            value = ((u64)(s64)(width * scale - 1) << 32) |
                    ((u64)(s64)(scale - 1) << 23) |
                    ((u64)(((s64)(horizontal_offset * scale) + ((s64)kernel_horizontal + 0x290)) & 0xfff)) |
                    ((u64)((vertical_offset + kernel_vertical + 0x48) & 0xfff) << 12);
            if (state->nSFrame_mode == 0) {
                value |= (u64)(s64)(height - 1) << 44;
            } else {
                value |= (u64)(s64)(height * 2 - 1) << 44;
            }
        } else {
            scale = (width + 0x9ff) / width;
            value = ((u64)(s64)(height - 1) << 44) |
                    ((u64)(s64)(width * scale - 1) << 32) |
                    ((u64)(s64)(scale - 1) << 23) |
                    ((u64)(((s64)(horizontal_offset * scale) + ((s64)kernel_horizontal + 0x290)) & 0xfff)) |
                    ((u64)((vertical_offset + kernel_vertical + 0x24) & 0xfff) << 12);
        }
        output->display = value;
    } else if (mode == 0x50) {
        output->display = ((u64)(s64)(height - 1) << 44) |
                          ((u64)(s64)(width * 2 - 1) << 32) |
                          ((u64)(((s64)((0x2d0 - width) / 2 * 2) + kernel_horizontal +
                                  horizontal_offset * 2 + 0xe8) & 0xfff)) |
                          ((u64)((vertical_offset + kernel_vertical + 0x23) & 0xfff) << 12) |
                          0x800000ULL;
    } else {
        scePrintf("sceGsDefDispEnv:Not support displaymode for 0x%x!!\n");
    }
    output->bgcolor = 0;
}
#endif
