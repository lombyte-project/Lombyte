/* Ported from rac1-decomp (src/game/camera.c, func_001EDB98). */
#include "sda.h"
#include "qcopy.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/camera/update_cam.h"

extern float resolve_camera_surface_height(void *, int) __asm__("func_002135F0");
void         update_camera_underwater_flag(void) __asm__("FUN_001ed7f0");

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

void update_camera_underwater_flag(void) {
    struct GameCamera *camera = &game_camera;
    float              probe_start[4];
    float              probe_end[4];
    int                i;

    if (camera->current_camera->unk86 == 6 || game_mode_macro != 0) {
        camera->underwater = 0;
        return;
    }
    qcopy(probe_start, &camera->position);
    qcopy(probe_end, &camera->position);
    probe_start[2] += 0.75f;
    probe_end[2] -= 0.75f;
    i = 0;
    while (i < 6 && collision_line(probe_start, probe_end, 0x12, 0, 0) != 0) {
        if (collision_hit_material() == 0) {
            float submersion_height_threshold =
                resolve_camera_surface_height(D_00194120, 0) + 0.04f;
            struct GameCamera *focus_camera = &game_camera;

            if (focus_camera->position.f[2] < submersion_height_threshold) {
                focus_camera->underwater = 1;
            } else {
                focus_camera->underwater = 0;
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
