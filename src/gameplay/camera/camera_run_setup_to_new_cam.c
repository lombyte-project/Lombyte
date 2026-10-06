#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"

extern struct CameraType camera_types[] __asm__("D_001E8C00");
void camera_run_setup_to_new_cam(struct UpdateCam *cam) __asm__("FUN_001ebec8");

void camera_run_setup_to_new_cam(struct UpdateCam *cam) {
    void (*fn)(struct UpdateCam *) = camera_types[cam->type].run_setup;

    if (fn != 0) {
        fn(cam);
    }
}

extern __typeof__(camera_run_setup_to_new_cam) func_001EBEC8 __attribute__((alias("FUN_001ebec8")));
