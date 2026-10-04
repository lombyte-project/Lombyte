#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001fd748/FUN_001fd748.s", FUN_001fd748);
#else
#include "types.h"

typedef struct {
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
} LevelMapMarker;

typedef struct {
    s32 label_text;
    s32 pad4[2];
} LevelMapMarkerText;

typedef struct {
    u8 pad0[0x224];
    s32 selected_level;
} LevelMapSelection;

extern u8 D_0013DD40[];
extern u8 D_0013DD58[];
extern s32 D_0015ED80;
extern s32 D_0015F438;
extern s32 D_0015F690 __attribute__((sda));
extern LevelMapSelection D_001A00F0;
extern LevelMapMarkerText D_001DDD44[];
extern LevelMapMarker D_001DDE28[];
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 func_001F6250(u8 *, s32);
extern void func_001F6530(s32, s32, u64, u8 *, s32);
extern s32 func_001F96F8(s32);
extern f32 func_001F9B20(f32 *);
extern u8 *func_001FDD10(s32);
extern s32 func_001FF960(s32, s32);
extern void func_001FFC30(s32, s32, s32, s32, s32, s32);
extern void func_00200258(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00200C80(s32, s32, s32, s32, u64, s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern void func_00233980(s32, u64);

void draw_level_selection_map(s32 left, s32 right, s32 top, s32 bottom) __asm__("FUN_001fd748");

void draw_level_selection_map(s32 left, s32 right, s32 top, s32 bottom)
{
    f32 label_direction[4];
    s32 level_index;
    s32 availability;
    s32 w;
    s32 h;
    s32 marker_x;
    s32 marker_y;
    s32 label_x;
    s32 label_y;
    s32 line_start_x;
    s32 line_start_y;
    s32 direction_length;
    s32 line_fraction;
    s32 text_width;
    s32 line_end_x;
    s32 text_x;
    u8 *label_text;
    s32 texture_index;

    func_001F4280(0);
    func_00233980(0x42, 0x8000000044);
    func_00233980(0x47, 0x4B);
    func_00200E08(0, 0, 0x200, 0x1C0, 0x80000000, 0);
    texture_index = func_001FF960(0xE99A, 0xE);
    w = (right - left) * 16;
    h = (bottom - top) * 16;
    func_00200258(texture_index, 0, 0, w, h, 0, 0, 0x80);
    func_00233980(8, 0);
    func_00200258(func_001FF960(0xE99A, 0xF), 0, 0, w, h, D_0015F438 & 0xFFF, 0, 0x80);
    func_00233980(8, 5);
    for (level_index = 1; level_index < 20; level_index++) {
        if (*(volatile s32 *)&D_001DDE28[level_index].x == 0) {
            continue;
        }
        availability = 3;
        if (D_0013DD58[level_index] == 0) {
            availability = 2;
            if (D_0013DD40[level_index] == 0) {
                availability = 0;
            }
        }
        if (availability == 0) {
            continue;
        }
        marker_x = D_001DDE28[level_index].x;
        marker_y = D_001DDE28[level_index].y;
        if (D_0015ED80 != 0) {
            marker_y = marker_y * 0x1C0 / 0x1A0;
        }
        if (availability == 3 || (availability == 2 && D_0015F438 % (func_001F96F8(0x16) + func_001F96F8(8)) < func_001F96F8(0x16))) {
            func_001FFC30(func_001FF960(0xE99A, 0xC), marker_x - 5, marker_y - 5, 10, 10, 0x80);
        }
        if (level_index == D_001A00F0.selected_level) {
            label_x = marker_x + D_001DDE28[level_index].dx;
            label_y = marker_y + D_001DDE28[level_index].dy;
            label_direction[1] = D_001DDE28[level_index].dy;
            label_direction[0] = D_001DDE28[level_index].dx;
            direction_length = func_001F9B20(label_direction) * 1000.0f;
            line_fraction = direction_length - 8000;
            line_start_x = label_x + (marker_x - label_x) * line_fraction / direction_length;
            line_start_y = label_y + (marker_y - label_y) * line_fraction / direction_length;
            label_text = func_001FDD10(D_001DDD44[level_index].label_text);
            text_width = func_001F6250(label_text, -1);
            if (D_001DDE28[level_index].dx <= -1) {
                line_end_x = label_x - text_width;
            } else {
                line_end_x = label_x + text_width;
            }
            func_00200C80(line_start_x + 1, line_start_y + 1, label_x + 1, label_y + 1, 0x80000000, 0);
            func_00200C80(label_x + 1, label_y + 1, line_end_x + 1, label_y + 1, 0x80000000, 0);
            text_x = (line_end_x < label_x) ? line_end_x : label_x;
            func_001F6530(text_x + 1, label_y - D_0015F690 + 1, 0x80000000, label_text, -1);
            func_00200C80(line_start_x, line_start_y, label_x, label_y, 0x80F0F0F0, 0);
            func_00200C80(label_x, label_y, line_end_x, label_y, 0x80F0F0F0, 0);
            func_001F6530(text_x, label_y - D_0015F690, 0x80F0F0F0, label_text, -1);
            func_001FFC30(func_001FF960(0xE99A, 0xD), marker_x - 10, marker_y - 10, 20, 20, 0x80);
        }
    }
    func_001F4398();
}
#endif /* NON_MATCHING */
