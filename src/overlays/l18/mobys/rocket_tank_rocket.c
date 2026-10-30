/* Level 18 rocket (class 0x29): the rocket_tank's rocket, with collision and trail, and its spawner. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");

#include "qcopy.h"

#include "sda.h"
#include "rnc/overlay/quad.h"

extern char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");
extern float          random_float_between(float, float) __asm__("FUN_002132a8");
extern int            collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void           func_L00_0025F4A8_alt(void *,
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
                                            float,
                                            int,
                                            float,
                                            int,
                                            int,
                                            int,
                                            int) __asm__("FUN_L00_0025e450");
extern unsigned char *FUN_L00_002712b8(void  *pos,
                                       float *vec,
                                       int    s,
                                       int    a,
                                       int    col,
                                       int    n,
                                       float  x,
                                       float  y,
                                       float  z,
                                       float  w,
                                       float  pw);
extern void           FUN_L00_00257d78(float *, float, float);
void                  mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern char          *D_L18_001B2ADC_lui __asm__("D_L18_001B2ADC") NOT_SDA;

void update_projectile_motion_collision_and_trail(char *moby) __asm__("FUN_L18_002a7220");

void update_projectile_motion_collision_and_trail(char *moby) {
    float  next_position[4];
    char   hit_descriptor[0x30];
    float  hit_direction_vector[4];
    float  effect_vector[4];
    float  effect_spawn_position[4];
    float  world_effect_direction[4];
    char  *data = ((char *)MOBY(moby)->pvars);
    float *p;
    float  movement_step_length, target_distance;
    char  *referenced_moby;

    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, frame_time * 6.2831855f);
    if (*(short *)(data + 0x12) != 0) {
        movement_step_length = fast_vec_length(data);
        fast_vec_sub(data, &hero.motion.near_ground_point, moby + 0x10);
        target_distance = fast_vec_length(data);
        if (movement_step_length < target_distance) {
            fast_vec_normalize(data, data, movement_step_length);
            MOBY(moby)->rot.z = fast_arc_tan(*(float *)data, *(float *)(data + 4));
            MOBY(moby)->rot.y = -fast_arc_tan(vector_length_xy(data), *(float *)(data + 8));
        } else if (target_distance < 0.1f) {
            mark_moby_for_removal(moby);
            return;
        }
    }
    fast_vec_add(next_position, moby + 0x10, data);
    referenced_moby = *(char **)(data + 0x14);
    if (referenced_moby != 0 && MOBY(referenced_moby)->oclass == 0x2E &&
        ((unsigned char *)referenced_moby)[0x20] != 0xFE &&
        ((unsigned char *)referenced_moby)[0x20] != 0xFD) {
        fast_vec_sub(hit_direction_vector, moby + 0x10, referenced_moby + 0x10);
        hit_direction_vector[2] = 0.0f;
        fast_vec_normalize(hit_direction_vector, hit_direction_vector, 2.0f);
        hit_direction_vector[3] = 5627.925f;
    } else {
        fast_vec_normalize(hit_direction_vector, data, 2.0f);
    }
    make_hit_descriptor(hit_descriptor, (int)moby, 1, hit_direction_vector, 2.0f);
    *(short *)(hit_descriptor + 0x1A) = ((unsigned short)MOBY(moby)->oclass);
    if (collision_line(moby + 0x10, next_position, 0, *(void **)(data + 0x14), hit_descriptor) !=
            0 ||
        fast_dec_timer_s16(data + 0x10) != 0) {
        func_L00_0025F4A8_alt(moby,
                              data,
                              moby + 0x10,
                              1.5f,
                              1.0f,
                              10,
                              6,
                              4,
                              1.0f,
                              0.5f,
                              9.0f,
                              0.5f,
                              0,
                              0.0f,
                              0,
                              0,
                              -1,
                              0);
        mark_moby_for_removal(moby);
    } else {
        unsigned char *particle;
        qcopy(moby + 0x10, next_position);
        FUN_L00_00257d78(effect_vector, frame_time * 0.1f, frame_time * 0.2f);
        fast_vec_scale(effect_spawn_position, data, random_float_between(0.0f, 1.0f));
        fast_vec_add(effect_spawn_position, effect_spawn_position, moby + 0x10);
        particle = FUN_L00_002712b8(effect_spawn_position,
                                    effect_vector,
                                    scale_game_frames(10),
                                    0x7F,
                                    0x606060,
                                    3,
                                    40000.0f,
                                    1000.0f,
                                    1.0f,
                                    -0.0002f,
                                    0.0f);
        if (particle != 0) {
            particle[2] = *D_L18_001B2ADC_lui;
            particle[3] = 0x44;
        }
        *(OvlQuad *)world_effect_direction = 0;
        world_effect_direction[2] = 0.02f;
        transform_vector_by_basis(world_effect_direction, world_effect_direction, moby + 0xC0);
        spawn_burst(effect_spawn_position,
                    world_effect_direction,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    scale_game_frames(10),
                    1,
                    20000.0f);
    }
}

extern void *create_moby() __asm__("FUN_0020c4f8");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002A87D0), where it is exact; names translated to the US level program. */

char *create_class_29_moby(struct Moby *owner,
                           float       *spawn_position,
                           float       *initial_movement_step,
                           int          homing_enabled) __asm__("FUN_L18_002a75f0");

char *create_class_29_moby(struct Moby *owner,
                           float       *spawn_position,
                           float       *initial_movement_step,

                           int homing_enabled) {
    char *moby = create_moby(0x29);
    if (moby != 0) {
        char *data;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        MOBY(moby)->spawn_frame = owner->spawn_frame;
        MOBY(moby)->scale = MOBY(moby)->scale * 3.0f;
        qcopy(moby + 0x10, spawn_position);
        data = ((char *)MOBY(moby)->pvars);
        qcopy(data, initial_movement_step);
        *(short *)(data + 0x10) = scale_game_frames(0xF0);
        *(short *)(data + 0x12) = homing_enabled;
        *(char **)(data + 0x14) = owner;
        MOBY(moby)->rot.z = fast_arc_tan(initial_movement_step[0], initial_movement_step[1]);
        MOBY(moby)->rot.y =
            -fast_arc_tan(vector_length_xy(initial_movement_step), initial_movement_step[2]);
    }
    return moby;
}
