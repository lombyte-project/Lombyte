#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"

struct CameraSavedState camera_saved_states[3] __attribute__((section(".data"))) = {0};
