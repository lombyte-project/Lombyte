/* Level 09 moby classes 0x50d, 0x528: floating ship piece. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/rotations/reorient_position_about_pivot.h"

extern struct Moby *moby_list __asm__("D_L09_0015FFD8");
typedef struct Moby Moby;
extern Moby        *moby_list __asm__("D_L09_0015FFD8");

#include "qzero.h"

/* Level 9 moby update: 0 sets up and either rises from below or reports a missing target, 1 waits for the target
 * moby to die, 2 rides with the hero (wobbles data+0x60/0x64 towards the hero's offset) or settles back.
 * The settling branch stores the height through a struct member (Mpos): a member store does not make the compiler
 * reload D_0015EE6C, a cast store does. The hero block is offsets of D_0013E633 cast to a struct, as retail
 * derives 0xE1D from the register holding 0xE9D. */
/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_0030A560), where it is exact; names translated to the US level program. */
typedef struct Hero Hero;
typedef int         u128_30A560 __attribute__((mode(TI)));

typedef struct {
    char  pad0[0x18];
    float z;
} Mpos_30A560;

extern float floating_ship_piece_hero_offset_tilt_gain_gp __asm__("D_L09_00161DC4")
    __attribute__((sda));
extern float floating_ship_piece_sink_speed_gp __asm__("D_L09_00161DC8") __attribute__((sda));
extern float floating_ship_piece_height_return_rate_gp __asm__("D_L09_00161DCC")
    __attribute__((sda));

#include "rnc/overlay/l09/mobys/floating_ship_piece_data/update_floating_ship_piece.h"

extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void  func_001E9730_30A560() __asm__("FUN_001e93b0");
extern int   func_001F9850_30A560(int) __asm__("FUN_001f96f8");
extern float func_L00_0025CCF0_30A560(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern char D_L09_00209518_30A560[] __asm__("D_L09_00209228");
void        update_floating_ship_piece(char *moby) __asm__("FUN_L09_003091b0");

void update_floating_ship_piece(char *moby) {
    enum {
        PLATFORM_STATE_INITIALIZE = 0,
        PLATFORM_STATE_WAIT_FOR_LINK_REMOVAL = 1,
        PLATFORM_STATE_ACTIVE = 2
    };
    char         *data = *(char **)(moby + 0x78);
    float         movement[4];
    float         original_rotation[4];
    float         hero_offset[4];
    float         old_angles[4];
    float         old_pos[4];
    float         carried_pos[4];
    float         start_z, new_z, sink_level, old_z;
    int           state;
    char         *linked;
    float         side_dot, front_dot;
    unsigned char linked_state;

    fast_vec_scale(movement, moby + 0x10, -1.0f);
    qcopy(original_rotation, moby + 0x40);
    old_z = *(float *)(moby + 0x18);
    sink_level = *(float *)(data + 0x84) - 0.300000012f;
    state = ((unsigned char *)moby)[0x20];
    switch (state) {
    case PLATFORM_STATE_INITIALIZE:
        *(int *)(data + 0x80) = 1;
        qcopy(data + 0x60, moby + 0x40);
        qzero(data + 0x70);
        start_z = *(float *)(moby + 0x18);
        *(float *)(data + 0x84) = start_z;
        if (*(int *)(data + 0x8C) != -1) {
            *(float *)(moby + 0x18) = start_z - 5.0f;
            *(float *)(moby + 0x40) = random_float_between(-1.57079637f, 1.57079637f);
            *(float *)(moby + 0x44) = random_float_between(-1.57079637f, 1.57079637f);
            moby[0x20] = 1;
        } else {
            func_001E9730_30A560(
                D_L09_00209518_30A560, *(short *)(moby + 0xB2), *(short *)(moby + 0xA6));
            moby[0x20] = 2;
        }
        break;
    case PLATFORM_STATE_WAIT_FOR_LINK_REMOVAL:
        linked = (char *)((char *)moby_list + (*(int *)(data + 0x8C) << 8));
        if (linked == 0) {
            goto set2;
        }
        linked_state = ((unsigned char *)linked)[0x20];
        if (linked_state == 0xFE) {
            goto set2;
        }
        if (linked_state != 0xFD) {
            break;
        }
    set2:
        moby[0x20] = 2;
        break;
    case PLATFORM_STATE_ACTIVE:
        if (hero.air_frames.s == 0 && ((char *)hero.ground_moby) == moby) {
            if (*(int *)(data + 0x88) == 0) {
                allocate_voice_for_target_entry(0, 0, moby);
            }
            if (*(int *)(data + 0x88) < func_001F9850_30A560(30)) {
                *(int *)(data + 0x88) = func_001F9850_30A560(30);
            }
        }
        if (*(int *)(data + 0x88) != 0) {
            float deg_to_rad;
            float tilt;
            float pitch_target;
            fast_dec_timer((int *)(data + 0x88));
            fast_vec_sub(hero_offset, ((unsigned char *)&hero + 0x80), moby + 0x10);
            deg_to_rad = 0.0174532924f;
            side_dot = fast_vec_dot(hero_offset, moby + 0xC0);
            front_dot = fast_vec_dot(hero_offset, moby + 0xD0);
            tilt = floating_ship_piece_hero_offset_tilt_gain_gp;
            pitch_target = tilt * side_dot;
            *(u128_30A560 *)old_angles = *(u128_30A560 *)(data + 0x60);
            *(u128_30A560 *)old_pos = *(u128_30A560 *)(moby + 0x10);
            func_L00_0025CCF0_30A560(
                data + 0x60,
                -tilt * front_dot,
                data + 0x70,
                0,
                floating_ship_piece_loaded_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq,
                floating_ship_piece_loaded_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                floating_ship_piece_loaded_tilt_snap_tolerance_scale_gp * deg_to_rad * frame_time);
            func_L00_0025CCF0_30A560(
                data + 0x64,
                pitch_target,
                data + 0x74,
                0,
                floating_ship_piece_loaded_tilt_time_squared_gain_gp * deg_to_rad * frame_time_sq,
                floating_ship_piece_loaded_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                floating_ship_piece_loaded_tilt_snap_tolerance_scale_gp * deg_to_rad * frame_time);
            new_z = *(float *)(moby + 0x18) - floating_ship_piece_sink_speed_gp * frame_time;
            *(float *)(moby + 0x18) = new_z;
            if (sink_level < old_z && new_z <= sink_level) {
                allocate_voice_for_target_entry(1, 0, moby);
            }
            reorient_position_about_pivot(
                ((unsigned char *)&hero + 0x80), carried_pos, moby + 0x10, old_angles, data + 0x60);
            fast_vec_sub(
                (unsigned char *)&hero + 0xF0, carried_pos, ((unsigned char *)&hero + 0x80));
            hero.motion.platform_delta.f[2] =
                hero.motion.platform_delta.f[2] + (*(float *)(moby + 0x18) - old_pos[2]);
        } else {
            float rise;
            ((Mpos_30A560 *)moby)->z =
                ((Mpos_30A560 *)moby)->z +
                (rise = floating_ship_piece_height_return_rate_gp * frame_time) *
                    (*(float *)(data + 0x84) - ((Mpos_30A560 *)moby)->z);
            if (((Mpos_30A560 *)moby)->z < *(float *)(data + 0x84) - 2.0f) {
                float deg_to_rad;
                ((Mpos_30A560 *)moby)->z = ((Mpos_30A560 *)moby)->z + rise;
                deg_to_rad = 0.0174532924f;
                func_L00_0025CCF0_30A560(
                    data + 0x60,
                    0.0f,
                    data + 0x70,
                    0,
                    floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad *
                        frame_time_sq * 3.0f,
                    floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                    frame_time * 1.57079637f);
                func_L00_0025CCF0_30A560(
                    data + 0x64,
                    0.0f,
                    data + 0x74,
                    0,
                    floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad *
                        frame_time_sq * 3.0f,
                    floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                    frame_time * 1.57079637f);
            } else {
                float deg_to_rad;
                deg_to_rad = 0.0174532924f;
                func_L00_0025CCF0_30A560(
                    data + 0x60,
                    0.0f,
                    data + 0x70,
                    0,
                    floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad *
                        frame_time_sq,
                    floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                    floating_ship_piece_return_tilt_snap_tolerance_scale_gp * deg_to_rad *
                        frame_time);
                func_L00_0025CCF0_30A560(
                    data + 0x64,
                    0.0f,
                    data + 0x74,
                    0,
                    floating_ship_piece_return_tilt_time_squared_gain_gp * deg_to_rad *
                        frame_time_sq,
                    floating_ship_piece_return_tilt_frame_scale_squared_gain_gp * frame_scale_sq,
                    floating_ship_piece_return_tilt_snap_tolerance_scale_gp * deg_to_rad *
                        frame_time);
            }
            if (((Mpos_30A560 *)moby)->z > *(float *)(data + 0x84)) {
                ((Mpos_30A560 *)moby)->z = *(float *)(data + 0x84);
            }
            if (old_z < sink_level) {
                if (sink_level <= ((Mpos_30A560 *)moby)->z) {
                    allocate_voice_for_target_entry(2, 0, moby);
                }
            }
        }
        update_moby_vertical_oscillation(0.140000001f,
                                         frame_time * 0.52359879f,
                                         moby,
                                         (float *)(data + 0x98),
                                         (float *)(data + 0x9C));
        update_moby_rotation_oscillation(moby,
                                         data + 0x90,
                                         data + 0x94,
                                         0.0872664601f,
                                         frame_time * 0.436332315f,
                                         frame_time * 0.541052043f);
        *(float *)(moby + 0x40) =
            fast_add_rotations(*(float *)(data + 0x60), *(float *)(moby + 0x40));
        *(float *)(moby + 0x44) =
            fast_add_rotations(*(float *)(data + 0x64), *(float *)(moby + 0x44));
        fast_vec_add(movement, movement, moby + 0x10);
        update_entity_matrix(data + 0x20, movement, original_rotation, moby + 0x40);
        break;
    }
}
