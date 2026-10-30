/* Follow camera setters (distance, height, offset scale, orbit clamp, lateral spring, slide) and camera volume selection. */

#include "types.h"
#include "sda.h"
#include "rnc/camera/follow_cam.h"

struct P2_388 {
    char pad[0x220];
    s32  cur;
};

struct P1_388 {
    char           pad0[0x70];
    struct P2_388 *p;
    char           pad1[0x12];
    s16            f86;
};

typedef struct {
    char  pad00[0x86];
    short f86;
} B842e83c0;

typedef struct {
    char pad00[0x70];
    u32  f70;
} B2842e83c0;

typedef struct {
    char pad00[0x220];
    u32  f220;
} C842e83c0;

typedef struct {
    char  pad00[0x84];
    short f84;
} O842e83c0;

typedef struct {
    char  pad[0x10];
    short h;
} S2e8568b;

typedef struct {
    char      pad[0x70];
    S2e8568b *p;
    char      pad2[0x12];
    short     f86;
} S2e8568;

extern u32                  D_L00_0015EF50;
extern u8                  *D_L00_00166E00_lui __asm__("D_L00_00166E00") NOT_SDA;
extern char *D_L00_00168FC0 NOT_SDA;
extern S2e8568             *D_L00_00166E00;

/* Empty apart from an unused 0x50-byte volatile stack buffer. */
void run_follow_camera_noop(void) __asm__("FUN_L00_002e8378");

void run_follow_camera_noop(void) {
    volatile char unused_stack_reservation[0x50];
}

/* -1 while D_L00_00166E00+0x86 is set; else clears its current id, 1 if it was id. */
s32 clear_follow_camera_volume_if_current(s32 id) __asm__("FUN_L00_002e8388");

s32 clear_follow_camera_volume_if_current(s32 id) {
    struct P1_388 *current_camera = (struct P1_388 *)current_follow_cam;
    if (current_camera->f86 != 0) {
        return -1;
    }
    if (current_camera->p->cur == id) {
        current_camera->p->cur = 0;
        return 1;
    }
    return 0;
}

int try_select_follow_camera_volume(int candidate_object, int unused) __asm__("FUN_L00_002e83c0");

int try_select_follow_camera_volume(int candidate_object, int unused) {
    void      *current_camera = current_follow_cam;
    C842e83c0 *camera_state;
    u8         x;
    u8         y;
    u32        tab;
    if (((B842e83c0 *)current_camera)->f86 != 0) {
        return -1;
    }
    camera_state = (C842e83c0 *)(u32)((B2842e83c0 *)current_camera)->f70;
    if (camera_state->f220 == 0) {
        camera_state->f220 = candidate_object;
        return 1;
    }
    if (camera_state->f220 == candidate_object) {
        return 1;
    }
    tab = (u32)D_L00_0015EF50;
    x = *(u8 *)(*(u32 *)(tab + ((O842e83c0 *)(u32)camera_state->f220)->f84 * 0x20 + 0x1C) + 0x1C);
    y = *(u8 *)(*(u32 *)(tab + ((O842e83c0 *)(u32)candidate_object)->f84 * 0x20 + 0x1C) + 0x1C);
    if (x < y) {
        camera_state->f220 = candidate_object;
        return 1;
    }
    return 0;
}

/* Sets a flag and floats a/b at +0x130 of D_L00_00166E00+0x70; f also stores a at +0x58. */
void follow_camera_set_distance(s32 f, f32 a, f32 b) __asm__("FUN_L00_002e8450");

void follow_camera_set_distance(s32 copy_distance_to_secondary_slot,
                                f32 distance,

                                f32 distance_override_companion_value) {
    u8 *current_camera = D_L00_00166E00_lui;
    if (*(s16 *)(current_camera + 0x86) == 0) {
        u8 *camera_distance_state = *(u8 **)(current_camera + 0x70) + 0x130;
        *(s16 *)(camera_distance_state + 0x3C) = 1;
        *(f32 *)(camera_distance_state + 0x48) = distance_override_companion_value;
        *(f32 *)(camera_distance_state + 0x40) = distance;
        if (copy_distance_to_secondary_slot)
            *(f32 *)(camera_distance_state + 0x58) = distance;
    }
}

void FUN_L00_002e8490(float camera_state_value) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x22C) = camera_state_value;
    }
}

/* Sets a flag and two floats at +0x130 of D_L00_00166E00+0x70 while its short at 0x86 is 0. */
void follow_camera_set_height(float a, float b) __asm__("FUN_L00_002e84b8");

void follow_camera_set_height(float height, float height_override_companion_value) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *camera_height_state = *(char **)(current_camera + 0x70) + 0x130;
        *(short *)(camera_height_state + 0x3e) = 1;
        *(float *)(camera_height_state + 0x4c) = height;
        *(float *)(camera_height_state + 0x54) = height_override_companion_value;
    }
}

/* Sets a flag and floats +0xF4/+0xF8 of D_L00_00166E00+0x70; add offsets a by 00168FC0+0xF0. */
void follow_camera_set_offset_scale(int add, float a, float b) __asm__("FUN_L00_002e84f0");

void follow_camera_set_offset_scale(int   add,
                                    float offset_scale,

                                    float offset_scale_companion_value) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *attach_state = *(char **)(current_camera + 0x70) + 0x40;
        *(short *)(attach_state + 0xDA) = 1;
        *(float *)(attach_state + 0xB8) = offset_scale_companion_value;
        if (add) {
            *(float *)(attach_state + 0xB4) = *(float *)(D_L00_00168FC0 + 0xF0) + offset_scale;
        } else {
            *(float *)(attach_state + 0xB4) = offset_scale;
        }
    }
}

/* Stores v through the pointer in D_L00_00166E00 while its short at 0x86 is 0. */
void set_follow_camera_orbit_clamp_mode(int camera_state_short_value) __asm__("FUN_L00_002e8568");

void set_follow_camera_orbit_clamp_mode(int camera_state_short_value) {
    S2e8568 *current_camera = D_L00_00166E00;
    if (current_camera->f86 == 0)
        current_camera->p->h = camera_state_short_value;
}

/* Stores non-zero a/b at +0x11C/+0x120 of D_L00_00166E00+0x70 while its short at 0x86 is 0. */
void follow_camera_set_lateral_spring(float a, float b) __asm__("FUN_L00_002e8590");

void follow_camera_set_lateral_spring(float lateral_spring_value,

                                      float lateral_spring_companion_value) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *attach_state = *(char **)(current_camera + 0x70) + 0x40;
        if (lateral_spring_value != 0.0f)
            *(float *)(attach_state + 0xDC) = lateral_spring_value;
        if (lateral_spring_companion_value != 0.0f)
            *(float *)(attach_state + 0xE0) = lateral_spring_companion_value;
    }
}

/* Sets the short at +0x226 of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E9AD0), where it is exact; names translated to the US level program. */
void FUN_L00_002e8620(void);

void FUN_L00_002e8620(void) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(short *)(*(char **)(current_camera + 0x70) + 0x226) = 1;
    }
}

/* Sets the byte at +0x116 of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
void follow_camera_start_slide(void) __asm__("FUN_L00_002e8648");

void follow_camera_start_slide(void) {
    u8 *current_camera = (u8 *)current_follow_cam;
    if (*(s16 *)(current_camera + 0x86) == 0) {
        *(u8 *)(*(u8 **)(current_camera + 0x70) + 0x116) = 1;
    }
}

void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");

void set_follow_camera_blend_output_flags(void) {
    char *current_camera = (char *)current_follow_cam;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *blend_output_block = *(char **)(current_camera + 0x70) + 0x1a8;
        *(int *)(blend_output_block + 0x10) |= 3;
    }
}
