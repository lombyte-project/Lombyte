/* Level 12 moby class mine (class 1269): the homing mine the homing_mine_spawner launches. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

extern float D_L12_0015F580[] MACRO_ADDR;
extern void  make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void  find_point_ahead(struct Moby *, void *, float) __asm__("FUN_L01_00274b78");
extern void *find_moby_hit(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern int
fun_00213928(struct Moby *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern int
collect_mobies_in_sphere(void *, int, struct Moby *, void *, float) __asm__("FUN_L00_001f2868");
extern int  coll_sphere(void *, int, struct Moby *, float) __asm__("FUN_L00_001f0d60");
extern void vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void spawn_explosion_304e00(struct Moby *,
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
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void jet_animate(float, struct Moby *) __asm__("FUN_L00_00259fe8");

typedef struct {
    u8   pad0[0x20];
    s32  damage[0x10];
    Vec4 start;
    f32  speed;
    f32  bob;
    f32  bob_angle;
    f32  climb;
    f32  turn;
    s32  timer;
} Data_304e00;

typedef struct {
    u8           pad0[0x20];
    struct Moby *moby;
} HitSource;

extern struct Moby *hit_moby __asm__("D_L12_001742D8") NOT_SDA;
/* A homing mine: rises, then steers toward the point ahead and blows up near it or on contact. */
void update_homing_mine(struct Moby *moby) __asm__("FUN_L12_00304e00");

void update_homing_mine(struct Moby *moby) {
    enum {
        HOMING_MINE_STATE_INITIALIZE = 0,
        HOMING_MINE_STATE_HOMING = 1,
        HOMING_MINE_STATE_DETONATE = 2
    };
    Data_304e00 *data = (Data_304e00 *)moby->pvars;
    s32         *damage = data->damage;
    f32          target[20];
    f32          velocity[3];
    u8           hit_info[0x30];
    Vec4         direction;
    Vec4         probe;
    int          hits;
    HitSource   *source;
    float        angle;
    float        turn;
    float        height;
    float        difference;

    find_point_ahead(moby, target, 8.0f);
    source = find_moby_hit(moby, 0x230000, 0);
    fun_00213928(moby, source, damage, 0, &hits, 0, 0, 4);
    if (vector_distance_xy(&moby->pos, target) < 0.25f ||
        (hits >= 2 && source != NULL && source->moby->oclass != 0x4F5)) {
        spawn_explosion_304e00(moby,
                               D_L12_0015F580,
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
                               2,
                               7.0f,
                               0,
                               0,
                               -1,
                               0);
        FUN_L00_00257470(moby, 0, -1);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case HOMING_MINE_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        qcopy(&data->start, &moby->pos);
        moby->state = 1;
        data->turn = 0.6981317f;
        if (moby->parent != NULL && ((struct Moby *)moby->parent)->command != 0)
            data->turn = -data->turn;
        data->timer = 0;
        break;
    case HOMING_MINE_STATE_HOMING:
        data->timer++;
        if (data->timer < scale_game_frames(30))
            moby->pos.z +=
                fast_sin((f32)data->timer * 3.1415927f / (f32)scale_game_frames(30)) * 0.07f;
        if (scale_game_frames(15) < data->timer)
            data->speed += frame_time_sq * 30.0f;
        if (scale_game_frames(30) < data->timer) {
            if (moby->prev_seq == 0)
                blend_moby_animation(moby, 1, 0, 2);
            if (moby->prev_seq == 1 && (moby->anim_flags & 2))
                blend_moby_animation(moby, 2, 0, 2);
        }
        if (frame_time * 5.75f < data->speed)
            data->speed = frame_time * 5.75f;
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn = angle;
        if (vector_distance_xy(&moby->pos, target) < 7.0f)
            turn = fast_add_rotations(angle, data->turn);
        velocity[0] = fast_cos(turn) * data->speed;
        velocity[1] = fast_sin(turn) * data->speed;
        velocity[2] = 0.0f;
        fast_vec_add(&moby->pos, &moby->pos, velocity);
        if (scale_game_frames(30) < data->timer) {
            data->bob_angle = fast_add_rotations(data->bob_angle, frame_time * 5.2359877f);
            moby->pos.z -= data->bob;
            data->bob = fast_sin(data->bob_angle) * 0.1f;
            moby->pos.z += data->bob;
        }
        if (scale_game_frames(30) < data->timer) {
            height = probe_ground_height(&moby->pos, 0, 0.5f);
            if (height < hero.ground_z.f)
                height = hero.ground_z.f;
            difference = height - moby->pos.z;
            if (difference < 0.5f && -3.0f < difference)
                vendor_spring(&moby->pos.z,
                              &data->climb,
                              height + 0.58f,
                              frame_scale_sq * 0.02f,
                              frame_scale_sq * 0.3f,
                              frame_time * 4.0f);
        }
        if (scale_game_frames(30) < data->timer) {
            direction.f[0] = fast_cos(angle) * 0.3f;
            direction.f[1] = fast_sin(angle) * 0.3f;
            direction.f[3] = 5627.925f;
            direction.f[2] = 1.2f;
            make_hit_descriptor(hit_info, moby, 0x10001, 1.0f, &direction);
            qcopy(&probe, &moby->pos);
            probe.f[2] += 0.4f;
            if (!collect_mobies_in_sphere(&probe, 0x10, moby, hit_info, 0.4f) || hit_moby == NULL ||
                hit_moby->oclass == 0x4F5 || hit_moby == moby->parent) {
                qcopy(&probe, &moby->pos);
                probe.f[2] += 0.35f;
                if (coll_sphere(&probe, 0, moby, 0.35f))
                    moby->state = 2;
            } else {
                moby->state = 2;
            }
        }
        break;
    case HOMING_MINE_STATE_DETONATE:
        spawn_explosion_304e00(moby,
                               D_L12_0015F580,
                               0,
                               0.0f,
                               0.0f,
                               5,
                               15,
                               25,
                               2.0f,
                               1.0f,
                               4.0f,
                               1.0f,
                               2,
                               7.0f,
                               0,
                               9,
                               -1,
                               0);
        mark_moby_for_removal(moby);
        return;
    }
    jet_animate(1.0f, moby);
}
