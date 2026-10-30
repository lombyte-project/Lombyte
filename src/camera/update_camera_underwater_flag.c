/* Ported from rac1-decomp (src/game/camera.c, func_001EDB98). */
#include "sda.h"
#include "qcopy.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"

extern float resolve_camera_surface_height(void *, int) __asm__("func_002135F0");
/* Camera-inside-water test: cast a ray through the camera focus from
   0.75 above to 0.75 below (up to six hits); on the first hit that is
   not a water surface, flag D_00186F40+0x394 when the focus is below the
   surface height + 0.04. Off for camera mode 6 or while D_0015F604 is
   set. */
void update_camera_underwater_flag(void) __asm__("FUN_001ed7f0");

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

void update_camera_underwater_flag(void) {
    char *preview_camera_base = (char *)&preview_camera;
    float probe_start[4];
    float probe_end[4];
    int   i;

    if (*(short *)(*(char **)(preview_camera_base + 0x180) + 0x86) == 6 || game_mode_macro != 0) {
        *(int *)(preview_camera_base + 0x394) = 0;
        return;
    }
    qcopy(probe_start, preview_camera_base + 0x140);
    qcopy(probe_end, preview_camera_base + 0x140);
    probe_start[2] += 0.75f;
    probe_end[2] -= 0.75f;
    i = 0;
    while (i < 6 && collision_line(probe_start, probe_end, 0x12, 0, 0) != 0) {
        if (collision_hit_material() == 0) {
            float submersion_height_threshold =
                resolve_camera_surface_height(D_00194120, 0) + 0.04f;
            char *preview_camera_state = (char *)&preview_camera;

            if (*(float *)(preview_camera_state + 0x148) < submersion_height_threshold) {
                *(int *)(preview_camera_state + 0x394) = 1;
            } else {
                *(int *)(preview_camera_state + 0x394) = 0;
            }
            return;
        }
        qcopy(probe_start, D_00194120);
        i++;
        probe_start[2] -= 0.01f;
    }
}

extern __typeof__(update_camera_underwater_flag) func_001ED7F0
    __attribute__((alias("FUN_001ed7f0")));
