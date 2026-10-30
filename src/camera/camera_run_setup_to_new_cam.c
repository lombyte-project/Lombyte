#include "types.h"
#include "rnc/camera/update_cam.h"

void camera_run_setup_to_new_cam(struct UpdateCam *cam) __asm__("FUN_001ebec8");

void camera_run_setup_to_new_cam(struct UpdateCam *camera_record_to_activate) {
    void (*fn)(struct UpdateCam *) = camera_types[camera_record_to_activate->type].run_setup;

    if (fn != 0) {
        fn(camera_record_to_activate);
    }
}

extern __typeof__(camera_run_setup_to_new_cam) func_001EBEC8 __attribute__((alias("FUN_001ebec8")));
