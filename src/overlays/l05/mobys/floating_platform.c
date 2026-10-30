/* Level 05 moby class floating_platform. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/overlay/l05/mobys/floating_platform_globals.h"
#include "rnc/audio/voice_alloc.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_sin(float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_0030D230), where it is exact; names translated to the US level program. */

extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");

extern float floating_platform_rotation_y_phase_rate_degrees_gp __asm__("D_L05_00161CD4")
    __attribute__((sda));
extern float floating_platform_rotation_x_phase_rate_degrees_gp __asm__("D_L05_00161CD8")
    __attribute__((sda));
extern float floating_platform_rotation_y_amplitude_degrees_gp __asm__("D_L05_00161CDC")
    __attribute__((sda));
extern float floating_platform_rotation_x_amplitude_degrees_gp __asm__("D_L05_00161CE0")
    __attribute__((sda));
extern int floating_platform_voice_interval_min_gp __asm__("D_L05_00161CE4") __attribute__((sda));
extern int floating_platform_voice_interval_max_gp __asm__("D_L05_00161CE8") __attribute__((sda));
void       floating_platform_update(unsigned char *moby) __asm__("FUN_L05_0030bdd8");

void floating_platform_update(unsigned char *moby) {
    enum {
        FLOATING_PLATFORM_INITIALIZE = 0,
        FLOATING_PLATFORM_UPDATE_ROCKING = 1
    };
    char  *data = ((char *)MOBY(moby)->pvars);
    float  previous_position[4];
    float  previous_rotation[4];
    float  position_delta[4];
    char  *position;
    char  *current_rotation;
    float *previous_rotation_ptr;
    float  k;
    float  rotation_y_phase;
    switch (moby[0x20]) {
    case FLOATING_PLATFORM_INITIALIZE:
        *(int *)(data + 0x78) = truncate_float_to_s32(
            scale_time(random_float_between((float)floating_platform_voice_interval_min_gp,
                                            (float)floating_platform_voice_interval_max_gp)));
        *(float *)(data + 0x60) = random_angle_radians();
        *(float *)(data + 0x6C) = random_angle_radians();
        moby[0x20] = 1;
        break;
    case FLOATING_PLATFORM_UPDATE_ROCKING:
        if (fast_dec_timer((int *)(data + 0x78))) {
            allocate_voice_for_target_entry(0, 0, moby);
            *(int *)(data + 0x78) = truncate_float_to_s32(
                scale_time(random_float_between((float)floating_platform_voice_interval_min_gp,
                                                (float)floating_platform_voice_interval_max_gp)));
        }
        position = (char *)moby + 0x10;
        qcopy(previous_position, position);
        previous_rotation_ptr = previous_rotation;
        current_rotation = (char *)moby + 0x40;
        qcopy(previous_rotation_ptr, current_rotation);
        rotation_y_phase = fast_add_rotations(*(float *)(data + 0x60),
                                              floating_platform_rotation_y_phase_rate_degrees_gp *
                                                  DEG_TO_RAD * frame_time);
        k = floating_platform_rotation_x_phase_rate_degrees_gp * DEG_TO_RAD * frame_time;
        *(float *)(data + 0x60) = rotation_y_phase;
        *(float *)(data + 0x6C) = fast_add_rotations(*(float *)(data + 0x6C), k);
        MOBY(moby)->rot.y = floating_platform_rotation_y_amplitude_degrees_gp * DEG_TO_RAD *
                            fast_sin(*(float *)(data + 0x60));
        MOBY(moby)->rot.x = floating_platform_rotation_x_amplitude_degrees_gp * DEG_TO_RAD *
                            fast_sin(*(float *)(data + 0x6C));
        fast_vec_sub(position_delta, position, previous_position);
        update_entity_matrix(data + 0x20, position_delta, previous_rotation_ptr, current_rotation);
        break;
    }
}
