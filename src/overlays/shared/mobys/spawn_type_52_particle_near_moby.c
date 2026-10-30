/* Spawns a type 52 particle at a random point on a disc around a moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_00299AF0.c: func_L10_002E51A8), where it is exact; names translated to the US level program. */

extern void
spawn_type_52_particle(float *, float, float, int, int, int) __asm__("FUN_L01_00287158");

extern short type_52_particle_header_value __asm__("D_L10_00161E58") __attribute__((sda));
extern short type_52_particle_data_value __asm__("D_L10_00161E5C") __attribute__((sda));
extern short type_52_particle_scale_upper_bound __asm__("D_L10_00161E60") __attribute__((sda));
extern short type_52_particle_random_value_upper_bound __asm__("D_L10_00161E64")
    __attribute__((sda));
extern short type_52_particle_divisor_lower_bound __asm__("D_L10_00161E68") __attribute__((sda));
extern short type_52_particle_divisor_upper_bound __asm__("D_L10_00161E6C") __attribute__((sda));
extern short type_52_particle_offset_radius_upper_bound __asm__("D_L10_00161E70")
    __attribute__((sda));
void spawn_type_52_particle_near_moby(char *moby) __asm__("FUN_L10_002e3de8");

void spawn_type_52_particle_near_moby(char *moby) {
    float spawn_position[3];
    int   particle_header_value, particle_data_value;
    float offset_angle_radians = random_angle_radians();
    float offset_radius =
        random_float_between(0.0f, *(float *)&type_52_particle_offset_radius_upper_bound);
    float particle_scale =
        random_float_between(0.0f, *(float *)&type_52_particle_scale_upper_bound);
    float random_particle_value =
        random_float_between(*(float *)&type_52_particle_scale_upper_bound,
                             *(float *)&type_52_particle_random_value_upper_bound);
    int random_divisor = random_int_between(*(int *)&type_52_particle_divisor_lower_bound,
                                            *(int *)&type_52_particle_divisor_upper_bound);
    spawn_position[0] = fast_cos(offset_angle_radians) * offset_radius;
    spawn_position[1] = fast_sin(offset_angle_radians) * offset_radius;
    spawn_position[2] = 0.0f;
    fast_vec_add(spawn_position, spawn_position, (float *)&MOBY(moby)->pos);
    spawn_position[2] = spawn_position[2] + 0.025f;
    particle_header_value = *(int *)&type_52_particle_header_value;
    particle_data_value = *(int *)&type_52_particle_data_value;
    spawn_type_52_particle(spawn_position,
                           particle_scale,
                           random_particle_value,
                           particle_header_value,
                           particle_data_value,
                           random_divisor);
}

short type_52_particle_divisor_upper_bound __asm__("D_L10_00161E6C") = 0xb4;
short D_L12_00161820 = 0x2028;
short D_L12_00161824 = 0x2020;
short D_L12_00161828 = 0;
short D_L12_0016182C = 0x6666;
short D_L12_00161830 = 0x64;
short D_L12_00161834 = 0xb4;
short D_L12_00161838 = -13107;
