/* Ship rocket (class 0x40A, Wrench name rocket): the homing rocket the jet fighter fires; its spawner is in level 11 (mobys/ship_rocket.c). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"

typedef struct {
    Vec4         step;       /* moved by this each frame; its length is speed */
    Vec4         target_pos; /* 0x10 */
    void        *collision;  /* 0x20 */
    struct Moby *target;     /* 0x24 */
    s32          timer;      /* 0x28 */
    f32          speed;      /* 0x2C */
    f32          turn_speed; /* 0x30 */
} Missile_94f8;

extern void
FUN_L00_0026cbb0_94f8(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern unsigned char *FUN_L00_002712b8_94f8(void *,
                                            void *,
                                            int,
                                            int,
                                            int,
                                            int,
                                            float,
                                            float,
                                            float,
                                            float,
                                            float) __asm__("FUN_L00_002712b8");
extern void           FUN_L00_0025e450_94f8(void *,
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

extern char rocket_smoke_texture[] __asm__("D_L11_001B2A80");
extern char rocket_impact_effect_origin[] __asm__("D_L11_00174860");

/* Homing missile: turns toward its target while the target lives and stays within 3 units of
   where it was last frame, leaves a smoke trail, and bursts on a hit, when its timer runs out
   or outside the level box. */

void update_ship_rocket(struct Moby *moby) __asm__("FUN_L11_003094f8");

void update_ship_rocket(struct Moby *moby) {
    Missile_94f8 *data = (Missile_94f8 *)moby->pvars;
    Vec4f        *pos;
    Vec4          previous_pos, spray, offset, velocity, random;
    float         fraction;
    struct Moby  *target;
    char         *texture;

    qcopy(&previous_pos, &moby->pos);
    moby->scale += (moby->pclass->scale - moby->scale) * 0.1f;
    random.q = 0;
    random.f[0] = random_float_between(-1.0f, 1.0f);
    random.f[1] = random_float_between(-1.0f, 1.0f);
    random.f[2] = random_float_between(-1.0f, 1.0f);
    velocity.q = random.q;
    fast_vec_normalize(&velocity, &velocity, random_float_between(0.1f, 0.2f) * frame_time);
    fast_vec_normalize(&offset, &data->step, -random_float_between(frame_time * 0.1f, frame_time));
    fast_vec_add(&velocity, &velocity, &offset);
    FUN_L00_002712b8_94f8(&moby->pos,
                          &velocity,
                          scale_game_frames(6),
                          0x7f,
                          0xb0b0b0,
                          3,
                          40000.0f,
                          1000.0f,
                          1.0f,
                          -0.0004f,
                          0.0f);
    texture = rocket_smoke_texture;
    for (fraction = 0.0f; fraction < 1.0f; fraction += 0.33333334f) {
        unsigned char *particle;
        fast_vec_normalize(&offset,
                           &data->step,
                           random_float_between(fraction, fraction + 0.33333334f) * data->speed);
        fast_vec_add(&offset, &offset, &moby->pos);
        particle = FUN_L00_002712b8_94f8(&offset,
                                         &velocity,
                                         scale_game_frames(60),
                                         0x7f,
                                         0x606060,
                                         3,
                                         40000.0f,
                                         1000.0f,
                                         1.0f,
                                         -0.0002f,
                                         0.0f);
        if (particle != 0) {
            unsigned char *texture_pixels = *(unsigned char **)(texture + 0x5c);
            unsigned char  texture_pixel = texture_pixels[0];
            particle[3] = 0x44;
            particle[2] = texture_pixel;
        }
        random.q = 0;
        random.f[2] = 0.02f;
        transform_vector_by_basis(&random, &random, &moby->rot_row0);
        build_look_at_matrix(&random, &random, &moby->rot_row0, random_angle_radians());
        FUN_L00_0026cbb0_94f8(
            &offset, &random, 0x4f007fff, 0x1fffffff, scale_game_frames(5), 1, 20000.0f);
    }
    pos = &moby->pos;
    target = data->target;
    if (target != 0 && target->state != 0xfe && target->state != 0xfd &&
        fast_vec_dist(&data->target_pos, &target->pos) < 3.0f) {
        moby->rot.z =
            fast_arc_tan(data->target->pos.x - moby->pos.x, data->target->pos.y - moby->pos.y);
        moby->rot.y = -fast_arc_tan(vector_distance_xy(pos, &data->target->pos),
                                    data->target->pos.z - moby->pos.z);
        qcopy(&data->target_pos, &data->target->pos);
    } else {
        approach_angle(&moby->rot.z,
                       &data->turn_speed,
                       moby->rot.z,
                       frame_time_sq * 251.32741f,
                       frame_time_sq * 251.32741f,
                       frame_time * 2513.2742f);
        approach_angle(&moby->rot.y,
                       &data->turn_speed,
                       moby->rot.y,
                       frame_time_sq * 251.32741f,
                       frame_time_sq * 251.32741f,
                       frame_time * 2513.2742f);
        data->target = 0;
    }
    build_spherical_offset(&data->step, data->speed, moby->rot.z, -moby->rot.y);
    fast_vec_add(pos, pos, &data->step);
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    moby_point_position(moby, 1, &spray);
    if (collision_line(&previous_pos, &spray, 0, data->collision, 0)) {
        FUN_L00_0025e450_94f8(moby,
                              data,
                              rocket_impact_effect_origin,
                              2.0f,
                              10.0f,
                              10,
                              3,
                              0x10,
                              4.0f,
                              2.0f,
                              9.0f,
                              1.0f,
                              -1,
                              20.0f,
                              0,
                              1,
                              -1,
                              0);
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer(&data->timer))
        mark_moby_for_removal(moby);
}
