/* Level 05 moby class floating_platform. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/overlay/l05/mobys/floating_platform_globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/random.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float fast_sin(float);
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

typedef struct FloatingPlatformVars {
    u8  pad_0[0x20];
    f32 carry[4]; /* carried-object state for update_entity_matrix */
    u8  pad_30[0x30];
    f32 rotation_y_phase;
    u8  pad_64[0x8];
    f32 rotation_x_phase;
    u8  pad_70[0x8];
    s32 timer_78;
} FloatingPlatformVars;

/* Moby class 810 */
void floating_platform_update(unsigned char *moby) {
    enum {
        FLOATING_PLATFORM_INITIALIZE = 0,
        FLOATING_PLATFORM_UPDATE_ROCKING = 1
    };
    FloatingPlatformVars *floating_platform_vars = (FloatingPlatformVars *)MOBY(moby)->pvars;
    float                 previous_position[4];
    float                 previous_rotation[4];
    float                 position_delta[4];
    char                 *position;
    char                 *current_rotation;
    float                *previous_rotation_ptr;
    float                 k;
    float                 rotation_y_phase;
    switch (MOBY(moby)->state) {
    case FLOATING_PLATFORM_INITIALIZE:
        floating_platform_vars->timer_78 = truncate_float_to_s32(
            scale_time(random_float_between((float)floating_platform_voice_interval_min_gp,
                                            (float)floating_platform_voice_interval_max_gp)));
        floating_platform_vars->rotation_y_phase = random_angle_radians();
        floating_platform_vars->rotation_x_phase = random_angle_radians();
        MOBY(moby)->state = FLOATING_PLATFORM_UPDATE_ROCKING;
        break;
    case FLOATING_PLATFORM_UPDATE_ROCKING:
        if (fast_dec_timer(&floating_platform_vars->timer_78)) {
            allocate_voice_for_target_entry(0, 0, moby);
            floating_platform_vars->timer_78 = truncate_float_to_s32(
                scale_time(random_float_between((float)floating_platform_voice_interval_min_gp,
                                                (float)floating_platform_voice_interval_max_gp)));
        }
        position = (char *)moby + 0x10;
        qcopy(previous_position, position);
        previous_rotation_ptr = previous_rotation;
        current_rotation = (char *)moby + 0x40;
        qcopy(previous_rotation_ptr, current_rotation);
        rotation_y_phase = fast_add_rotations(floating_platform_vars->rotation_y_phase,
                                              floating_platform_rotation_y_phase_rate_degrees_gp *
                                                  DEG_TO_RAD * frame_time);
        k = floating_platform_rotation_x_phase_rate_degrees_gp * DEG_TO_RAD * frame_time;
        floating_platform_vars->rotation_y_phase = rotation_y_phase;
        floating_platform_vars->rotation_x_phase =
            fast_add_rotations(floating_platform_vars->rotation_x_phase, k);
        MOBY(moby)->rot.y = floating_platform_rotation_y_amplitude_degrees_gp * DEG_TO_RAD *
                            fast_sin(floating_platform_vars->rotation_y_phase);
        MOBY(moby)->rot.x = floating_platform_rotation_x_amplitude_degrees_gp * DEG_TO_RAD *
                            fast_sin(floating_platform_vars->rotation_x_phase);
        fast_vec_sub(position_delta, position, previous_position);
        update_entity_matrix(
            floating_platform_vars->carry, position_delta, previous_rotation_ptr, current_rotation);
        break;
    }
}
