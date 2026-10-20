#include "types.h"

#include "rnc/rendering/material_templates.h"

void set_up_vis_gif_viewer(u64 *q, s32 n, s32 prim, s32 a3, s32 t0,
                           s32 mode) __asm__("FUN_00202fd0");

void set_up_vis_gif_viewer(u64 *packet_destination, s32 draw_control_high_word, s32 prim, s32 material_control_base, s32 material_control_shift2, s32 mode) {
    u64 w0;
    s64 w1;
    u64 w2;
    u64 *src;

    w0 = resident_material_templates[mode * 3];
    w1 = resident_material_templates[mode * 3 + 1];
    w2 = resident_material_templates[mode * 3 + 2];
    if (mode >= 0) {
        packet_destination[0] = (0x20 | (w1 & 0x1C)) | ((u64)prim << 6) | ((u64)(u32)draw_control_high_word << 32);
        packet_destination += 2;
        packet_destination[0] = (material_control_base | ((u64)material_control_shift2 << 2)) | ((u64)mode << 24);
        packet_destination += 2;
        packet_destination[0] = w0;
        packet_destination[2] = w2;
    } else if (mode < -1) {
        src = special_material_template;
        if (mode == -3) {
            src = alternate_special_material_template;
        }
        packet_destination[0] = (0x20 | ((u64)prim << 6)) | ((u64)(u32)draw_control_high_word << 32);
        packet_destination += 2;
        packet_destination[0] = 5;
        packet_destination += 2;
        packet_destination[0] = src[0];
        packet_destination[2] = src[2];
    } else {
        packet_destination[0] = (0x20 | ((u64)prim << 6)) | ((u64)(u32)draw_control_high_word << 32);
        packet_destination += 2;
        packet_destination[0] = 5;
        packet_destination += 2;
        packet_destination[0] = ((u64)0x8000 << 29 | 0x9980) << 19 | 0x7FFB;
        packet_destination[2] = 0;
    }
}

extern __typeof__(set_up_vis_gif_viewer) func_00202FD0 __attribute__((alias("FUN_00202fd0")));
