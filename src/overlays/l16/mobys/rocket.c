/* Level 16 rocket (class 0x5c3): a homing projectile. */
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
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"

#include "qcopy.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern float D_0015ED6C;

#include "sda.h"
#include "rnc/overlay/quad.h"

/* Advance a homing spark, emit its trail, and test its collision or impact state. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002A50F0), where it is exact; names translated to the US level program. */

#include "rnc/overlay/l16/mobys/hit_descriptor.h"

typedef struct {
    char  pad[0x18];
    char *moby;
    int   count;
    float position[4];
} L16SparkHit;

extern char *D_L16_001B255C NOT_SDA;
extern char *
spawn_particle(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float D_0015ED6C, frame_time;
extern float frame_time_sq;
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   D_L16_0015F580_spark __asm__("D_L16_0015F580"); /* no foreign declaration */
extern int   collision_line() __asm__("FUN_001efa68");
extern void  spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void func_L00_0025F4A8_alt(void *,
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
void        build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern L16SparkHit D_L16_001742C0_hit __asm__("D_L16_00174240");
extern float       func_L00_0025CCF0_spark(void *, void *, float, float, float, float, int) __asm__("FUN_L00_0025bc98");

void homing_projectile_update(unsigned char *moby) __asm__("FUN_L16_002a3f38");

/* Moby class 1475 */
void homing_projectile_update(unsigned char *moby) {
    enum {
        PROJECTILE_STATE_FLYING = 0,
        PROJECTILE_STATE_IMPACT = 1
    };
    L16QuadVector    desired, previous, velocity, random;
    L16HitDescriptor query;
    char            *projectile_data = ((char *)MOBY(moby)->pvars);
    void            *position;
    void            *matrix;
    char            *particle;
    struct Hero     *player;
    float            yaw, pitch;
    switch (moby[0x20]) {
    case PROJECTILE_STATE_FLYING: {
        float step, time;
        step = D_0015ED6C;
        time = frame_time_sq;
        position = moby + 0x10;
        (*(unsigned short *)(projectile_data + 6))++;
        approach_value((float *)(projectile_data + 0x10), step * 30.0f, time * 70.0f);
        if (*(void **)(projectile_data + 8) &&
            *(short *)(projectile_data + 6) > scale_game_frames(7)) {
            qcopy(desired.f, *(char **)(projectile_data + 8) + 0x10);
            desired.f[2] += 0.7f;
            yaw = fast_arc_tan(desired.f[0] - MOBY(moby)->pos.x, desired.f[1] - MOBY(moby)->pos.y);
            pitch = -fast_arc_tan(vector_distance_xy(position, desired.f),
                                  desired.f[2] - MOBY(moby)->pos.z);
            func_L00_0025CCF0_spark(moby + 0x48,
                                    projectile_data + 0x14,
                                    yaw,
                                    0.03f,
                                    0.3f,
                                    D_0015ED6C * 9.599310874938965f,
                                    0);
            func_L00_0025CCF0_spark(moby + 0x44,
                                    projectile_data + 0x18,
                                    pitch,
                                    0.03f,
                                    0.3f,
                                    D_0015ED6C * 9.599310874938965f,
                                    0);
        }
        build_spherical_offset(
            desired.f, *(float *)(projectile_data + 0x10), MOBY(moby)->rot.z, -MOBY(moby)->rot.y);
        random.quad = 0;
        random.f[0] = random_float_between(-1.0f, 1.0f);
        random.f[1] = random_float_between(-1.0f, 1.0f);
        random.f[2] = random_float_between(-1.0f, 1.0f);
        velocity.quad = random.quad;
        fast_vec_normalize(velocity.f, velocity.f, random_float_between(0.1f, 0.2f) * D_0015ED6C);
        fast_vec_normalize(
            previous.f, desired.f, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
        fast_vec_add(velocity.f, velocity.f, previous.f);
        fast_vec_normalize(previous.f,
                           desired.f,
                           random_float_between(0.0f, 1.0f) * *(float *)(projectile_data + 0x10));
        fast_vec_add(previous.f, previous.f, position);
        particle = spawn_particle(previous.f,
                                  velocity.f,
                                  scale_game_frames(60),
                                  127,
                                  0x606060,
                                  3,
                                  40000.0f,
                                  1000.0f,
                                  1.0f,
                                  -0.0002f,
                                  0.0f);
        if (particle) {
            unsigned char cls = *(unsigned char *)D_L16_001B255C;
            particle[3] = 0x44;
            particle[2] = cls;
        }
        matrix = moby + 0xC0;
        spawn_particle(position,
                       velocity.f,
                       scale_game_frames(6),
                       127,
                       0xB0B0B0,
                       3,
                       40000.0f,
                       1000.0f,
                       1.0f,
                       -0.0004f,
                       0.0f);
        random.quad = 0;
        random.f[2] = 0.02f;
        transform_vector_by_basis(random.f, random.f, matrix);
        build_look_at_matrix(random.f, random.f, matrix, random_angle_radians());
        spawn_burst(
            previous.f, random.f, 0x4F007FFF, 0x1FFFFFFF, scale_game_frames(5), 1, 20000.0f);
        qcopy(previous.f, position);
        fast_vec_add(position, position, desired.f);
        if (MOBY(moby)->pos.x < 2.0f || MOBY(moby)->pos.y < 2.0f || MOBY(moby)->pos.z < 2.0f)
            goto remove;
        {
            int impact_state;
            query.flags = 0x830000;
            impact_state = 1;
            query.owner = moby;
            query.scale = 1.0f;
            query.active = impact_state;
            qcopy(query.direction, desired.f);
            set_vector_length_xy(query.direction, query.direction, 1.0f);
            player = &hero;
            query.direction[2] = 1.0f;
            query.direction[3] = 5627.9248046875f;
            query.kind = 3;
            query.cls = ((unsigned short)MOBY(moby)->oclass);
            query.enabled = impact_state;
            if (collision_line(previous.f, position, 0, ((void *)player->moby), &query)) {
                L16SparkHit *collision_hit = &D_L16_001742C0_hit;
                char        *hit = collision_hit->moby;
                if (hit) {
                    if (hit == ((char *)player->moby) || hit == *(char **)projectile_data)
                        break;
                } else if (collision_hit->count <= 0)
                    break;
                qcopy(position, collision_hit->position);
                moby[0x20] = impact_state;
            } else if (fast_dec_timer_s16(projectile_data + 4) ||
                       vector_distance_xy(position, &player->motion.pos) > 60.0f) {
            remove:
                mark_moby_for_removal(moby);
            }
        }
        break;
    }
    case PROJECTILE_STATE_IMPACT:
        func_L00_0025F4A8_alt(moby,
                              &D_L16_0015F580_spark,
                              0,
                              0.0f,
                              0.0f,
                              3,
                              3,
                              5,
                              1.0f,
                              0.5f,
                              4.0f,
                              0.7f,
                              0,
                              7.0f,
                              0,
                              0,
                              -1,
                              0);
        mark_moby_for_removal(moby);
        break;
    }
}
