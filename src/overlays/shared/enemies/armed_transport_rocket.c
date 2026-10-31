/* Armed transport rocket (class 0x32, Wrench name rocket): the homing rocket the armed transport fires, and the function that spawns it. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/world/collision/collision_line.h"

extern char *create_moby(int);
extern int   trooper_play_sound(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             int,
                             float,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  spawn_rocket_trail_spark(char *) __asm__("FUN_L15_0029ae18");
void         build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float fast_add_rotations(float, float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   scale_game_frames(int);

extern char  D_L16_001671C0[];
extern float class_32_symmetric_angle_bound_degrees_gp __asm__("D_L16_001613E0")
    __attribute__((sda));
extern float class_32_zero_based_angle_bound_degrees_gp __asm__("D_L16_001613E4")
    __attribute__((sda));
extern char D_L16_001671C0[], D_0013E633[];

float D_L18_001613E0 = 3e+01f;
float D_L18_001613E4 = 2e+01f;

/* Steer a projectile toward its target and emit its impact effects. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002A1C50), where it is exact; names translated to the US level program. */
void update_armed_transport_rocket(char *m) __asm__("FUN_L16_002a0a98");

/* Moby class 50 */
void update_armed_transport_rocket(char *rocket_moby) {
    float desired[4], next_position[4], delta[4], blend[4];
    char *rocket_data = ((char *)MOBY(rocket_moby)->pvars);
    void *remaining;
    float yaw, pitch;
    if (*(int *)(rocket_data + 0x34) == 0 &&
        fast_vec_dist(rocket_moby + 0x10, D_L16_001671C0) < 15.0f) {
        *(int *)(rocket_data + 0x34) = 1;
        trooper_play_sound(5, 0, (int)rocket_moby, 0x54C);
    }
    if (MOBY(rocket_moby)->state != 0)
        return;
    spawn_rocket_trail_spark(rocket_moby);
    fast_vec_sub(delta, rocket_data + 0x10, rocket_moby + 0x10);
    yaw = fast_arc_tan(delta[0], delta[1]);
    pitch = fast_arc_tan(vector_length_xy(delta), delta[2]);
    yaw = fast_add_rotations(yaw, *(float *)(rocket_data + 0x28));
    pitch = fast_add_rotations(pitch, *(float *)(rocket_data + 0x2C));
    if (pitch > 1.2217305f)
        pitch = 1.2217305f;
    else if (pitch < -1.2217305f)
        pitch = -1.2217305f;
    build_spherical_offset(desired, *(float *)(rocket_data + 0x24), yaw, pitch);
    if (fast_dec_timer((int *)(rocket_data + 0x20)) == 0) {
        float blend_ticks_remaining = convert_integer_to_float(*(int *)(rocket_data + 0x20));
        float blend_duration_ticks = convert_integer_to_float(*(int *)(rocket_data + 0x30));
        lerp_vector(blend, desired, rocket_data, blend_ticks_remaining / blend_duration_ticks);
        fast_vec_add(next_position, rocket_moby + 0x10, blend);
        MOBY(rocket_moby)->rot.z = fast_arc_tan(blend[0], blend[1]);
        MOBY(rocket_moby)->rot.y = -fast_arc_tan(vector_length_xy(blend), blend[2]);
    } else {
        fast_vec_add(next_position, rocket_moby + 0x10, desired);
        MOBY(rocket_moby)->rot.z = yaw;
        MOBY(rocket_moby)->rot.y = pitch;
    }
    remaining = delta;
    if (collision_line((rocket_moby + 0x10), next_position, 0, rocket_moby, 0) != 0 ||
        fast_vec_length(remaining) < *(float *)(rocket_data + 0x24)) {
        if (hero.vehicle_mode == 2)
            play_entity_sound((int)rocket_moby, 2.0f, (rocket_moby + 0x10), 3, 2.0f, 1.0f, 0, 1, 0);
        trooper_play_sound(4, 0, (int)rocket_moby, 0x54C);
        spawn_explosion(rocket_moby,
                        rocket_data,
                        (rocket_moby + 0x10),
                        4.0f,
                        4.0f,
                        10,
                        3,
                        16,
                        4.0f,
                        2.0f,
                        9.0f,
                        -1,
                        1.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(rocket_moby);
    } else
        qcopy((rocket_moby + 0x10), next_position);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002A1F78), where it is exact; names translated to the US level program. */
char *spawn_armed_transport_rocket(char *owner,
                                   char *initial_movement_vector,
                                   void *pos,
                                   void *vel,
                                   int   blend_duration_ticks,
                                   float unused) __asm__("FUN_L16_002a0dc0");

char *spawn_armed_transport_rocket(char *spawning_moby,
                                   char *initial_movement_vector,
                                   void *spawn_position,
                                   void *target_position,
                                   int   blend_duration_ticks,

                                   float unused) {
    char *spawned_moby = (char *)create_moby(0x32);
    char *moby_data;
    float initial_movement_length, x, random_angle_radians, y;
    if (spawned_moby) {
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->spawn_frame = *(unsigned long *)(spawning_moby + 0x38);
        MOBY(spawned_moby)->scale = MOBY(spawned_moby)->scale * 3.0f;
        qcopy(spawned_moby + 0x10, spawn_position);
        moby_data = ((char *)MOBY(spawned_moby)->pvars);
        qcopy(moby_data, initial_movement_vector);
        qcopy(moby_data + 0x10, target_position);
        *(int *)(moby_data + 0x30) = blend_duration_ticks;
        *(int *)(moby_data + 0x20) = blend_duration_ticks;
        initial_movement_length = fast_vec_length(initial_movement_vector);
        x = class_32_symmetric_angle_bound_degrees_gp;
        *(float *)(moby_data + 0x24) = initial_movement_length;
        random_angle_radians = random_float_between(-x, x) * DEG_TO_RAD;
        y = class_32_zero_based_angle_bound_degrees_gp;
        *(float *)(moby_data + 0x28) = random_angle_radians;
        *(float *)(moby_data + 0x2C) = random_float_between(0.0f, y) * DEG_TO_RAD;
        *(int *)(moby_data + 0x34) = 0;
        MOBY(spawned_moby)->rot.z = fast_arc_tan(*(float *)initial_movement_vector,
                                                 *(float *)(initial_movement_vector + 4));
        MOBY(spawned_moby)->rot.y = -fast_arc_tan(vector_length_xy(initial_movement_vector),
                                                  *(float *)(initial_movement_vector + 8));
        spawned_moby[0x31] = 0;
        MOBY(spawned_moby)->flags |= 1;
    }
    return spawned_moby;
}
