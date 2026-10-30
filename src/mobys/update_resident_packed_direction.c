#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/math/asm_helpers.h"

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

extern struct Vec3 resident_packed_direction __asm__("D_001CAA80");
extern void        get_moby_light_direction(s32, f32 *) __asm__("FUN_0020d510");
extern f32         fast_cos(f32) __asm__("func_001F9DC8");
extern f32         fast_sin(f32) __asm__("func_001F9DE0");
void               update_resident_packed_direction(void) __asm__("FUN_0020cfd0");

void update_resident_packed_direction(void) {
    f32 angle_source_vector[4];
    f32 angle;

    get_moby_light_direction(D_001413D0[0], angle_source_vector);
    angle = fast_arc_tan(angle_source_vector[0], angle_source_vector[1]);
    resident_packed_direction.x = fast_cos(angle) * 0.14f;
    resident_packed_direction.y = fast_sin(angle) * 0.14f;
    resident_packed_direction.z = -0.99f;
}

extern __typeof__(update_resident_packed_direction) func_0020CFD0
    __attribute__((alias("FUN_0020cfd0")));
