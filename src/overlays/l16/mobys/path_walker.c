/* Level 16 moby classes path_walker (0x683-0x687): mobys that walk a path in a group. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

extern char *D_L16_001B0930[];
extern float frame_time, D_0015EE70;

/* Update of a path walker: keeps its sound alive, follows the path behind its owner and jumps ahead while unobserved. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E8B30), where it is exact; names translated to the US level program. */

typedef struct {
    int            path_index;
    float          parameter;
    float          speed;
    float          period;
    unsigned char *owner;
    float          height_step;
    float          target_speed;
    short          countdown;
    short          sound;
} L16WalkerData_c;

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

#include "rnc/overlay/l16/mobys/path_walker_globals.h"

extern float path_walker_gap_range_start_gp __asm__("D_L16_00161E68") __attribute__((sda));
extern float path_walker_gap_range_end_gp __asm__("D_L16_00161E6C") __attribute__((sda));
extern float path_walker_speed_range_start_gp __asm__("D_L16_00161E70") __attribute__((sda));
extern float path_walker_speed_range_end_gp __asm__("D_L16_00161E74") __attribute__((sda));
extern float path_walker_speed_approach_acceleration_gp __asm__("D_L16_00161E78")
    __attribute__((sda));
extern float path_walker_height_range_start_gp __asm__("D_L16_00161E7C") __attribute__((sda));
extern float path_walker_height_range_end_gp __asm__("D_L16_00161E80") __attribute__((sda));
extern float path_walker_reposition_advance_rate_gp __asm__("D_L16_00161E8C") __attribute__((sda));
extern int   path_walker_draw_distance_gp __asm__("D_L16_00161E90") __attribute__((sda));
extern int   allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
void         emit_path_walker_particle(unsigned char *source_moby,
                                       void          *previous_position) __asm__("FUN_L16_002e7a30");

extern void emit_path_walker_particle(unsigned char *, void *) __asm__("FUN_L16_002e7a30");
void        initialize_path_walker_group(struct Moby *moby) __asm__("FUN_L16_002e7ba0");

extern void initialize_path_walker_group(struct Moby *) __asm__("FUN_L16_002e7ba0");
extern void sample_camera_path(void *, f32, s32, void *, void *, s32) __asm__("func_00214E58");

void path_walker_update(unsigned char *walker_moby) __asm__("FUN_L16_002e76b8");

/* Moby classes 1667-1671 */
void path_walker_update(unsigned char *walker_moby) {
    enum {
        PATH_WALKER_INITIALIZE_GROUP = 0,
        PATH_WALKER_FOLLOW_PATH = 1
    };
    float            old[4], rotation[4], trial[4];
    char            *path;
    L16WalkerData_c *walker_data;
    int              playing = 0;

    walker_data = ((L16WalkerData_c *)MOBY(walker_moby)->pvars);
    path = D_L16_001B0930[walker_data->path_index];
    if (((((unsigned short)MOBY(walker_moby)->oclass) ^ 1) & 1) != 0) {
        if (voice_is_playing(walker_moby, walker_data->sound) == 0) {
            walker_data->sound = allocate_voice_for_target_entry(0, 4, (int)walker_moby);
        } else {
            playing = 1;
        }
    } else {
        qcopy(old, walker_moby + 0x10);
    }
    switch (walker_moby[0x20]) {
    case PATH_WALKER_INITIALIZE_GROUP:
        if (walker_moby[0x21] == 255) {
            mark_moby_for_removal(walker_moby);
            return;
        }
        initialize_path_walker_group(walker_moby);
        break;
    case PATH_WALKER_FOLLOW_PATH: {
        L16WalkerData_c *other = *(L16WalkerData_c **)(walker_data->owner + 0x78);
        float            gap, step, next, height, acceleration;
        /* `playing` again, in a variable of case 1's own. The logic does not need it: tested directly,
           `playing` is one value from the top of the function to the test, the compiler ranks it below
           `path` and gives them $s5 and $s4, the reverse of retail. The narrowing copy ends that value
           here and starts another; each ranks above `path`, both get $s4 and the copy itself leaves no
           instruction. A same-width copy (int) is folded away and does not do it. */
        char blocked = playing;

        sample_camera_path(path,
                           walker_data->parameter,
                           0,
                           walker_moby + 0x10,
                           (float *)&MOBY(walker_moby)->rot,
                           0);
        acceleration = path_walker_speed_approach_acceleration_gp * frame_time_sq;
        MOBY(walker_moby)->pos.z += walker_data->height_step;
        walker_data->parameter += walker_data->speed;
        approach_value(&walker_data->speed, walker_data->target_speed, acceleration);
        gap = other->parameter - walker_data->parameter;
        if (gap < 0.0f) {
            gap += walker_data->period;
        }
        if (gap < path_walker_gap_range_start_gp &&
            walker_data->target_speed > other->target_speed) {
            float saved = walker_data->target_speed;
            float theirs = other->target_speed;

            other->target_speed = saved;
            walker_data->target_speed = theirs;
        }
        if (gap < 1.5f) {
            step = frame_time_sq * 50.0f;
            approach_value(&walker_data->speed, walker_data->target_speed, step);
            approach_value(&other->speed, other->target_speed, step);
        }
        if (walker_moby[0x31] == 0 && blocked == 0) {
            if (gap > path_walker_gap_range_end_gp + 1.0f) {
                height = random_float_between(path_walker_height_range_start_gp,
                                              path_walker_height_range_end_gp);
                if (walker_data->owner[0x31] != 0 && walker_data->countdown != 0) {
                    next = other->parameter - random_float_between(path_walker_gap_range_start_gp,
                                                                   path_walker_gap_range_end_gp);
                    walker_data->countdown--;
                } else {
                    next = walker_data->parameter +
                           path_walker_reposition_advance_rate_gp * frame_time;
                }
                if (next < 0.0f) {
                    next += walker_data->period;
                }
                sample_camera_path(path, next, 0, trial, rotation, 1);
                trial[2] += height;
                trial[3] = 2.0f;
                if (fast_bsphere_check((char *)trial, (float)path_walker_draw_distance_gp) == -1) {
                    float random = random_float_between(path_walker_speed_range_start_gp,
                                                        path_walker_speed_range_end_gp);

                    walker_data->parameter = next;
                    walker_data->height_step = height;
                    walker_data->target_speed = random * frame_time;
                }
            }
        } else {
            walker_data->countdown = 30;
        }
        if (walker_data->parameter > walker_data->period) {
            walker_data->parameter -= walker_data->period;
        }
        break;
    }
    }
    if (((unsigned short)MOBY(walker_moby)->oclass) & 1) {
        emit_path_walker_particle(walker_moby, old);
    }
}

extern int scale_game_frames(int);
extern int D_L16_0015F5CC;

extern char D_L16_001671C0[];

#include "rnc/overlay/quad.h"

#include "rnc/math/spherical_offset.h"

extern void
func_L00_0026DD70_emit(void *, void *, int, int, float, int) __asm__("FUN_L00_0026ced0");

int          D_L16_00161E98 = 0x60d0d080;
int          D_L16_00161E9C = 0x801000;
extern float path_walker_particle_movement_velocity_scale_gp __asm__("D_L16_00161EA0")
    __attribute__((sda));
extern float path_walker_particle_random_velocity_scale_gp __asm__("D_L16_00161EA4")
    __attribute__((sda));
extern float path_walker_particle_target_scalar_scale_gp __asm__("D_L16_00161EA8")
    __attribute__((sda));

void emit_path_walker_particle(unsigned char *source_moby, void *previous_position) {
    float input[4];
    float offset[4];
    float movement_velocity[4];
    float velocity[4];
    float random_offset_angle, second_random_offset_angle, size, movement_distance, final_size;
    *(OvlQuad *)input = *(OvlQuad *)previous_position;
    if ((unsigned char)source_moby[0x31] == 0)
        return;
    if ((D_L16_0015F5CC & 1) != (((int)source_moby >> 8) & 1))
        return;
    if (!(fast_vec_dist(source_moby + 0x10, D_L16_001671C0) < 75.0f))
        return;
    movement_distance = fast_vec_dist(source_moby + 0x10, input);
    fast_vec_scale(movement_velocity,
                   source_moby + 0xC0,
                   path_walker_particle_movement_velocity_scale_gp * movement_distance);
    size = path_walker_particle_random_velocity_scale_gp;
    size *= frame_time;
    fast_vec_normalize(offset, source_moby + 0xC0, -0.5f);
    fast_vec_add(offset, offset, source_moby + 0x10);
    random_offset_angle = random_angle_radians();
    second_random_offset_angle = random_angle_radians();
    build_spherical_offset(velocity, size, random_offset_angle, second_random_offset_angle);
    fast_vec_add(velocity, velocity, movement_velocity);
    final_size = path_walker_particle_target_scalar_scale_gp;
    final_size *= 210000.0f;
    func_L00_0026DD70_emit(
        offset, velocity, D_L16_00161E98, D_L16_00161E9C, final_size, scale_game_frames(0x23));
}

/* Start of a walker group: lists the group's mobys and spaces them along the path in random order. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9018), where it is exact; names translated to the US level program. */

typedef struct {
    int            path_index;
    float          parameter;
    float          speed;
    float          period;
    unsigned char *owner;
    float          height_step;
    float          target_speed;
    short          countdown;
    short          sound;
} L16WalkerData;

extern char *D_L16_0015FFD8;
extern float path_walker_initial_scale_multiplier __asm__("D_L16_00161E94")
    __attribute__((sda)); /* no foreign declaration */
extern short *D_L16_001ABCC0[];

void initialize_path_walker_group(struct Moby *moby) {
    unsigned char *objects[16];
    unsigned char *previous;
    L16WalkerData *walker_data = (L16WalkerData *)moby->pvars;
    short         *list = D_L16_001ABCC0[moby->group];
    short          count = 0, rank;
    int            i;
    float          period;

    if (list == 0 || walker_data->path_index == -1) {
        return;
    }
    period = (float)*(int *)D_L16_001B0930[walker_data->path_index] - 1.0f;
    do {
        objects[count] =
            (unsigned char *)(D_L16_0015FFD8 + ((*(unsigned short *)list & 0x7FFF) << 8));
        count++;
    } while (*list++ >= 0);
    previous = objects[count - 1];
    rank = count;
    for (i = 0; i < count; i++) {
        unsigned char *current;
        L16WalkerData *selected_walker_data;
        unsigned short flags;

        if (i == count - 1) {
            /* the last one takes the slot the loop index is on */
            current = objects[i];
        } else {
            /* the others take a random slot, or the next one still in use after it */
            int selected = random_integer_below(count - 1);

            for (;;) {
                if (objects[selected] != 0) {
                    current = objects[selected];
                    objects[selected] = 0;
                    break;
                }
                selected = (selected + 1) % (count - 1);
            }
        }
        flags = path_walker_draw_distance_gp;
        current[0x30] = 255;
        *(float *)(current + 0x2C) *= path_walker_initial_scale_multiplier;
        *(int *)(current + 0x94) = 0;
        *(unsigned short *)(current + 0x32) = flags;
        selected_walker_data = *(L16WalkerData **)(current + 0x78);
        selected_walker_data->parameter = (float)rank * (period / convert_integer_to_float(count));
        selected_walker_data->owner = previous;
        selected_walker_data->period = period;
        previous = current;
        selected_walker_data->height_step = random_float_between(path_walker_height_range_start_gp,
                                                                 path_walker_height_range_end_gp);
        selected_walker_data->speed =
            random_float_between(path_walker_speed_range_start_gp, path_walker_speed_range_end_gp) *
            frame_time;
        selected_walker_data->target_speed = selected_walker_data->speed;
        current[0x20] = 1;
        rank--;
    }
}
