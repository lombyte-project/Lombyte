#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"

extern struct CameraType camera_types[] __asm__("D_001E8C00");
void camera_exit(struct UpdateCam *cam) __asm__("FUN_001ec3d8");

void camera_exit(struct UpdateCam *cam) {
    void (*fn)(struct UpdateCam *) = camera_types[cam->type].exit;

    if (fn != 0) {
        fn(cam);
    }
}

extern __typeof__(camera_exit) func_001EC3D8 __attribute__((alias("FUN_001ec3d8")));
