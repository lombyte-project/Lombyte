/* Ported from rac1-decomp (src/game/camera.c, func_001EDE50). */
#include "sda.h"
#include "qcopy.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/camera/update_camera_tracking_state.h"

extern int           text_screen_active[] __asm__("D_001E6400");
extern char          camera_shake_vectors[] __asm__("D_001870A0");
extern unsigned char D_0015EDB4[4] MACRO_ADDR;
extern void          advance_timed_camera_control(void) __asm__("FUN_001eda60");
extern void          refresh_camera_control_flags(void) __asm__("func_001ED940");
extern void          update_camera_blend(char *) __asm__("FUN_001ed2b0");
extern void          start_camera_blend(void *) __asm__("func_001EC8A0");
extern int           update_all_cameras(void) __asm__("func_001EC420");
extern void          apply_camera_shake(void *, int) __asm__("func_001ED360");
extern void          update_camera_underwater_flag(void) __asm__("func_001ED7F0");
extern void          update_camera_environment_from_regions(void *) __asm__("FUN_001ee4b0");
extern void          fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
/* Camera update, once per frame: count the frame, run the camera
   steps, then (unless the D_0018C318 freeze flag is set) take the view
   from the target object (mode 3 blends it through update_camera_blend) and
   refresh its Euler angles; run apply_camera_shake on the two vectors at
   D_001870A0, update_camera_underwater_flag and update_camera_environment_from_regions, and, with
   D_0015EDB4 set, the matrix's third row as a cross product. */
void update_camera(void) __asm__("FUN_001edaa8");

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

void update_camera(void) {
    char *preview_camera_state;
    char *target;
    char *camera_shake_base;
    float rotation_matrix[16];

    if (game_mode_macro == 5) {
        if (text_screen_active[0] != 0) {
            return;
        }
        *(short *)cam_trans_state = 0;
        cam_trans_state[2] = 0;
    }
    preview_camera_state = (char *)&preview_camera;
    (*(int *)(preview_camera_state + 0x398))++;
    advance_timed_camera_control();
    refresh_camera_control_flags();
    update_camera_tracking_state();
    update_all_cameras();
    target = *(char **)(preview_camera_state + 0x180);
    if ((unsigned short)(*(unsigned short *)(preview_camera_state + 0x270) - 1) < 2) {
        start_camera_blend(*(void **)(preview_camera_state + 0x184));
    }
    if (*(short *)(preview_camera_state + 0x270) == 3) {
        update_camera_blend(target);
    } else {
        char *st = occl_cam_state;
        if (*(int *)(st + 0x14) != 0) {
            goto frozen;
        }
        qcopy(preview_camera_state + 0x140, target + 0x30);
        qcopy(preview_camera_state + 0x350, target);
        qcopy(preview_camera_state + 0x360, target + 0x10);
        qcopy(preview_camera_state + 0x370, target + 0x20);
    }
    {
        char *st = occl_cam_state;
        if (*(int *)(st + 0x14) == 0) {
            copy_matrix_rotation(rotation_matrix, camera_rows);
            matrix_to_euler(rotation_matrix, camera_rows - 0x200);
        }
    }
frozen:
    camera_shake_base = camera_shake_vectors;
    apply_camera_shake(camera_shake_base, 0);
    apply_camera_shake(camera_shake_base + 0x10, 1);
    update_camera_underwater_flag();
    update_camera_environment_from_regions(camera_shake_base - 0x20);
    if (D_0015EDB4[0] != 0) {
        fast_vec_cross(
            camera_shake_base + 0x200, camera_shake_base + 0x210, camera_shake_base + 0x1F0);
    }
}

extern __typeof__(update_camera) func_001EDAA8 __attribute__((alias("FUN_001edaa8")));
