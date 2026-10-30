/* Level 16 bomb (class 0x119): the thrown bomb and its spawner. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"

#include "qcopy.h"

extern float D_0015ED6C;

#include "sda.h"

extern float D_0015ED6C, frame_time;
extern int   collision_line() __asm__("FUN_001efa68");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

extern float D_0015ED6C, D_0015EE70_q __asm__("D_0015EE70");
/* Update a thrown bomb (class 0x119): fly and bounce off what it hits and off the ground, rest, then burst. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C7218), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad[0x18];
    char *moby;
    int   count;
    float position[4];
    float unk30[4];
    float normal[4];
} L16SparkHitN;

typedef struct {
    char          pad00[7];
    unsigned char reaction; /* 0x07 */
    char          pad08[8];
    float         velocity[4]; /* 0x10 */
    void         *owner;       /* 0x20: the moby that threw it, left out of the sweep */
    float         gravity;     /* 0x24 */
    int           timer;       /* 0x28 */
} L16FallingData;

typedef struct {
    char            pad00[0x10];
    float           position[4]; /* 0x10 */
    unsigned char   state;       /* 0x20 */
    char            pad21[0x57];
    L16FallingData *data; /* 0x78 */
} L16FallingMoby;

extern char D_L16_0015F580_macro[] __asm__("D_L16_0015F580") MACRO_ADDR;

extern int          coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void         trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void         trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void         spawn_explosion(void *,
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
extern L16SparkHitN D_L16_hitN __asm__("D_L16_00174240");

void update_thrown_bomb(L16FallingMoby *moby) __asm__("FUN_L16_002c5eb0");

void update_thrown_bomb(L16FallingMoby *moby) {
    enum {
        BOMB_STATE_MOVING = 1,
        BOMB_STATE_RESTING = 2,
        BOMB_STATE_EXPLODE = 3
    };
    float           previous[4], normal[4];
    L16FallingData *bomb_data = moby->data;

    switch (moby->state) {
    case BOMB_STATE_MOVING:
        /* No pointer locals: m->position and d->velocity are written at each use and the compiler caches
           them itself (velocity formed in the argument register and copied to a saved one, the position
           copied a second time for the ground test). */
        qcopy(previous, moby->position);
        fast_vec_add(moby->position, moby->position, bomb_data->velocity);
        if (collision_line(previous, moby->position, 5, bomb_data->owner, 0)) {
            fast_vec_normalize(normal, D_L16_hitN.normal, 1.0f);
            if (fast_vec_dot(normal, bomb_data->velocity) < 0.0f) {
                reflect_vector(bomb_data->velocity, bomb_data->velocity, normal);
                fast_vec_scale(bomb_data->velocity, bomb_data->velocity, 0.25f);
            }
        }
        if (coll_sphere(0.333f, moby->position, 2, 0)) {
            fast_vec_normalize(normal, D_L16_hitN.normal, 0.333f);
            if (fast_vec_dot(normal, bomb_data->velocity) < 0.0f) {
                reflect_vector(bomb_data->velocity, bomb_data->velocity, normal);
                fast_vec_scale(bomb_data->velocity, bomb_data->velocity, 0.25f);
            }
            qcopy(moby->position, D_L16_hitN.position);
            moby->position[2] += normal[2];
            if (normal[2] >= 0.3f && fast_vec_length(bomb_data->velocity) < D_0015ED6C * 0.25f) {
                moby->state = 2;
                bomb_data->timer = scale_game_frames(90);
            }
        }
        if (fast_dec_timer(&bomb_data->timer)) {
            moby->state = 3;
        } else if (bomb_data->timer < scale_game_frames(45) &&
                   bomb_data->timer % scale_game_frames(10) == 0) {
            bomb_data->reaction = 250;
            trooper_anim_update(moby, (short *)bomb_data);
        }
        bomb_data->velocity[2] -= bomb_data->gravity;
        break;
    case BOMB_STATE_RESTING:
        if (fast_dec_timer(&bomb_data->timer)) {
            moby->state = 3;
        } else if (bomb_data->timer < scale_game_frames(61) &&
                   bomb_data->timer % scale_game_frames(20) == 0) {
            bomb_data->reaction = 250;
            trooper_anim_update(moby, (short *)bomb_data);
        }
        break;
    case BOMB_STATE_EXPLODE:
        allocate_voice_for_target_entry(0, 0, moby);
        spawn_explosion(moby,
                        (int)D_L16_0015F580_macro,
                        0,
                        1.5f,
                        1.0f,
                        7,
                        10,
                        20,
                        3.0f,
                        1.7f,
                        4.0f,
                        -1,
                        1.0f,
                        7.0f,
                        0,
                        7,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    }
    trooper_anim_finish(moby, bomb_data);
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C75D0), where it is exact; names translated to the US level program. */

extern char *create_moby(int);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern char *create_moby(int) __asm__("FUN_0020c4f8");

unsigned char *spawn_thrown_bomb(int    owner,
                                 void  *spawn_position,
                                 float *initial_velocity,
                                 int    fuse_frames,
                                 float  gravity) __asm__("FUN_L16_002c6268");

unsigned char *spawn_thrown_bomb(

    int owner, void *spawn_position, float *initial_velocity, int fuse_frames, float gravity) {
    unsigned char *moby = (unsigned char *)create_moby(0x119);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        MOBY(moby)->draw_dist = 0xFF;
        *(int *)(data + 0x20) = owner;
        *(float *)(data + 0x24) = gravity;
        *(int *)(data + 0x28) = fuse_frames;
        *(short *)(data + 0xC) = scale_game_frames(5);
        *(short *)(data + 0xE) = scale_game_frames(0xF);
        qcopy(moby + 0x10, spawn_position);
        qcopy(data + 0x10, initial_velocity);
        MOBY(moby)->rot.x = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        MOBY(moby)->rot.z = fast_arc_tan(initial_velocity[0], initial_velocity[1]) + 1.5707964f;
        update_entity(moby);
    }
    return moby;
}
