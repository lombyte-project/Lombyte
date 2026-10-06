/* Ported from rac1-decomp (src/game/camera.c, func_001EDE50). */
#include "sda.h"
#include "qcopy.h"
extern char D_001871B0[];
extern char D_0018C318[];
extern char D_00187290[];
extern char D_00186F40[];
extern int D_0015F604 MACRO_ADDR;
extern int D_001E6400[];
extern char D_001870A0[];
extern unsigned char D_0015EDB4[4] MACRO_ADDR;
extern void advance_timed_camera_control(void) __asm__("FUN_001eda60");
extern void refresh_camera_control_flags(void) __asm__("func_001ED940");
extern void FUN_001ed470(void);
extern void update_camera_blend(char *) __asm__("FUN_001ed2b0");
extern void start_camera_blend(void *) __asm__("func_001EC8A0");
extern int update_all_cameras(void) __asm__("func_001EC420");
extern void apply_camera_shake(void *, int) __asm__("func_001ED360");
extern void update_camera_underwater_flag(void) __asm__("func_001ED7F0");
extern void FUN_001fa298(void *, void *);
extern void FUN_00214598(void *, void *);
extern void update_camera_environment_from_regions(void *) __asm__("FUN_001ee4b0");
extern void func_001F9AD8(void *, void *, void *);
/* Camera update, once per frame: count the frame, run the camera
   steps, then (unless the D_0018C318 freeze flag is set) take the view
   from the target object (mode 3 blends it through FUN_001ed2b0) and
   refresh its Euler angles; run func_001ED360 on the two vectors at
   D_001870A0, func_001ED7F0 and FUN_001ee4b0, and, with
   D_0015EDB4 set, the matrix's third row as a cross product. */
void update_camera(void) __asm__("FUN_001edaa8");

void update_camera(void) {
    char *c;
    char *target;
    char *v;
    float m[16];

    if (D_0015F604 == 5) {
        if (D_001E6400[0] != 0) {
            return;
        }
        *(short *)D_001871B0 = 0;
        D_001871B0[2] = 0;
    }
    c = D_00186F40;
    (*(int *)(c + 0x398))++;
    advance_timed_camera_control();
    refresh_camera_control_flags();
    FUN_001ed470();
    update_all_cameras();
    target = *(char **)(c + 0x180);
    if ((unsigned short)(*(unsigned short *)(c + 0x270) - 1) < 2) {
        start_camera_blend(*(void **)(c + 0x184));
    }
    if (*(short *)(c + 0x270) == 3) {
        update_camera_blend(target);
    } else {
        char *st = D_0018C318;
        if (*(int *)(st + 0x14) != 0) {
            goto frozen;
        }
        qcopy(c + 0x140, target + 0x30);
        qcopy(c + 0x350, target);
        qcopy(c + 0x360, target + 0x10);
        qcopy(c + 0x370, target + 0x20);
    }
    {
        char *st = D_0018C318;
        if (*(int *)(st + 0x14) == 0) {
            FUN_001fa298(m, D_00187290);
            FUN_00214598(m, D_00187290 - 0x200);
        }
    }
frozen:
    v = D_001870A0;
    apply_camera_shake(v, 0);
    apply_camera_shake(v + 0x10, 1);
    update_camera_underwater_flag();
    update_camera_environment_from_regions(v - 0x20);
    if (D_0015EDB4[0] != 0) {
        func_001F9AD8(v + 0x200, v + 0x210, v + 0x1F0);
    }
}

extern __typeof__(update_camera) func_001EDAA8 __attribute__((alias("FUN_001edaa8")));
