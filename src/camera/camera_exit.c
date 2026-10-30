#include "types.h"
#include "rnc/camera/update_cam.h"

void camera_exit(struct UpdateCam *cam) __asm__("FUN_001ec3d8");

void camera_exit(struct UpdateCam *active_camera_record) {
    void (*fn)(struct UpdateCam *) = camera_types[active_camera_record->type].exit;

    if (fn != 0) {
        fn(active_camera_record);
    }
}

extern __typeof__(camera_exit) func_001EC3D8 __attribute__((alias("FUN_001ec3d8")));
