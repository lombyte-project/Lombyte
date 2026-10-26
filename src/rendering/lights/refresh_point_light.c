#include "types.h"
extern s32 create_point_light() __asm__("FUN_00201ba8");
extern s32 detach_point_light() __asm__("FUN_00201f88");
void refresh_point_light(s32 point_light_index) __asm__("FUN_00201f58");

void refresh_point_light(s32 point_light_index) {
    detach_point_light();
    create_point_light(point_light_index);
}
