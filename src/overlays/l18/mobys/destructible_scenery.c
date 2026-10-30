/* Level 18 classes 0x375-0x3a8: scenery that bursts into sparks or flies off and explodes when its group is hit. */
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

#include "qcopy.h"
#include "rnc/overlay/shared/math/interpolation/set_moby_list_state.h"
#include "rnc/overlay/quad.h"
#include "rnc/mobys/moby_spawn.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern int scale_game_frames(int);
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int random_integer_below(int) __asm__("FUN_00213260");

extern float fast_add_rotations(float, float);
extern int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");

extern s32 random_integer_below(s32) __asm__("FUN_00213260");

extern int   FUN_L00_0025a420(void *, int, int);
void         set_moby_group_state_two(int group) __asm__("FUN_L18_002e9e50");
extern void  cart_burst(float, float, void *, void *, int) __asm__("FUN_L00_0025f090");
extern float FUN_L00_00257c48(float, float);
extern void  subtract_vector_9768(float *, float *, float *) __asm__("FUN_001f9a28");
extern void  scale_vector_9768(float *, float *, float) __asm__("FUN_001f9bf8");
extern void  add_vector_9768(float *, float *, float *) __asm__("FUN_001f9a10");
extern void  random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern int   coll_sphere(float, float *, int, void *) __asm__("FUN_L00_001f0d60");
extern void  spawn_particle(void *, void *, int, int, float, int, int, float, int, int) __asm__("FUN_L00_0026a9f0");
extern void
spawn_particle_alt_9768(float, void *, void *, int, int, int) __asm__("FUN_L00_0026a4c8");
extern int   D_L18_0015F580;
extern float D_0013F3D0[4];

#include "rnc/game/game_globals.h"

extern unsigned char D_L18_001F29A0_9768[] __asm__("D_L18_001F29A0");
extern unsigned char D_L18_001F29B8_9768[] __asm__("D_L18_001F29B8");

void update_moby_hit_response_burst_and_ballistic_flight(struct Moby *moby) __asm__("FUN_L18_002e9768");

void update_moby_hit_response_burst_and_ballistic_flight(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_WAIT_FOR_HIT = 1,
        STATE_BURST_OR_LAUNCH = 2,
        STATE_BALLISTIC_FLIGHT = 3
    };
    float *data = (float *)moby->pvars;
    long   state = moby->state;
    float  magnitude;

    switch (state) {
    case STATE_INITIALIZE: {
        if (moby->mission != 0xff) {
            int mission_table_index = moby->mission + current_level_index * 16;
            if (((unsigned char *)((int *)visited52))[mission_table_index] == 0xff)
                goto remove;
        }
        moby->state = 1;
        if (*(int *)data != 0)
            *(int *)(*(int *)data) = 3;
        return;
    }
    case STATE_WAIT_FOR_HIT:
        if (FUN_L00_0025a420(moby, 0x80000, 0)) {
            set_moby_group_state_two(moby->group);
            trooper_play_sound(0, 0, (int)moby, 0x375);
        }
        moby->hit_slot = 0xff;
        return;
    case STATE_BURST_OR_LAUNCH:
        if (moby->oclass == 0x37c) {
            float *pos;
            {
                int   i;
                int  *first, *second;
                float colors[6];
                float colors2[6];
                float position[4];
                i = 0;
                do {
                    pos = &moby->pos.x;
                    magnitude = random_float_between(8.0f, 10.0f) * frame_time;
                    __builtin_memcpy(colors, D_L18_001F29A0_9768, 24);
                    __builtin_memcpy(colors2, D_L18_001F29B8_9768, 24);
                    qcopy(position, pos);
                    ++i;
                    position[2] += random_float_between(0.0f, 6.0f);
                    first = (int *)colors + random_integer_below(6);
                    second = (int *)colors2 + random_integer_below(6);
                    {
                        int lifetime, spread;
                        lifetime = random_int_between(scale_game_frames(15), scale_game_frames(20));
                        spread = random_int_between(scale_game_frames(30), scale_game_frames(45));
                        spawn_particle(position,
                                       &D_L18_0015F580,
                                       *first,
                                       *second,
                                       1500000.0f,
                                       lifetime,
                                       spread,
                                       magnitude,
                                       0,
                                       0);
                    }
                    {
                        int lifetime, spread;
                        lifetime = random_int_between(scale_game_frames(5), scale_game_frames(10));
                        spread = random_int_between(scale_game_frames(15), scale_game_frames(20));
                        spawn_particle(position,
                                       &D_L18_0015F580,
                                       0x7fffffff,
                                       0xffffff,
                                       1500000.0f,
                                       lifetime,
                                       spread,
                                       magnitude * 0.5f,
                                       0,
                                       0);
                    }
                } while ((float)i < 10.0f);
            }
            {
                int   i;
                int  *first, *second;
                float position[4];
                float colors[6];
                float velocity[4];
                float colors2[6];
                float direction[4];
                float zero;
                i = 0;
                zero = (float)i;
                do {
                    qcopy(position, pos);
                    ++i;
                    position[2] += random_float_between(zero, 6.0f);
                    __builtin_memcpy(colors, D_L18_001F29A0_9768, 24);
                    __builtin_memcpy(colors2, D_L18_001F29B8_9768, 24);
                    *(OvlQuad *)direction = 0;
                    direction[0] = random_float_between(-1.0f, 1.0f);
                    direction[1] = random_float_between(-1.0f, 1.0f);
                    direction[2] = random_float_between(-1.0f, 1.0f);
                    *(u128 *)velocity = *(u128 *)direction;
                    scale_vector_9768(
                        velocity, velocity, random_float_between(zero, 3.0f) * frame_time);
                    first = (int *)colors + random_integer_below(6);
                    second = (int *)colors2 + random_integer_below(6);
                    spawn_particle_alt_9768(
                        500000.0f,
                        position,
                        velocity,
                        *first,
                        *second,
                        random_int_between(scale_game_frames(30), scale_game_frames(45)));
                } while ((float)i < 10.0f);
            }
        remove:
            mark_moby_for_removal(moby);
            return;
        } else {
            float vector[4];
            float speed = random_float_between(10.0f, 15.0f) * frame_time;
            moby->collision = 0;
            moby->state = 3;
            data[2] = FUN_L00_00257c48(0.0f, 90.0f) * DEG_TO_RAD * frame_time;
            data[3] = FUN_L00_00257c48(0.0f, 90.0f) * DEG_TO_RAD * frame_time;
            subtract_vector_9768(data + 4, &moby->pos.x, D_0013F3D0);
            data[6] = 0.0f;
            scale_vector_9768(data + 4, data + 4, speed);
            random_vector(vector, speed, speed);
            add_vector_9768(data + 4, data + 4, vector);
            data[6] = absolute_float(data[6]);
            data[7] = moby->pos.z;
            return;
        }
        break;
    case STATE_BALLISTIC_FLIGHT: {
        float *pos = &moby->pos.x;
        moby->rot.x = fast_add_rotations(moby->rot.x, data[2]);
        moby->rot.y = fast_add_rotations(moby->rot.y, data[2]);
        data[6] -= frame_time_sq * 20.0f;
        add_vector_9768(pos, pos, data + 4);
        if (data[7] - 16.0f > moby->pos.z || coll_sphere(1.0f, pos, 2, moby)) {
            cart_burst(1.5f, 0.0f, moby, pos, -1);
            mark_moby_for_removal(moby);
        }
        return;
    }
    default:
        return;
    }
    return;
}

void set_moby_group_state_two(int group) __asm__("FUN_L18_002e9e50");

/* Sets every moby of the group to state 2. */
void set_moby_group_state_two(int group) {
    set_moby_list_state((void *)group, 0x2);
}
