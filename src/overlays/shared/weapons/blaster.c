/* Shared moby class blaster: the blaster held by the hero, with its auto-aim target selection. */

#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/globals.h"
#include "rnc/hero/hero.h"
#include "rnc/input/pad_state.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/collision.h"
#include "rnc/rendering/debug/debug_menu.h"

typedef union {
    OvlQuad q;
    float   f[4];
} V_2b46a8;

typedef struct Hero Hero;

typedef struct {
    u8  p0[0x2084];
    s32 i2084;
} G_B4F40_49a8;

typedef union {
    struct {
        float x, y, z, w;
    } f;
    u128 q;
} BlasterVec __attribute__((aligned(16)));

typedef struct {
    BlasterVec row[3];
} BlasterMatrix3;

typedef struct {
    BlasterVec row[4];
} BlasterMatrix4;

typedef struct BlasterVars {
    int   cooldown;
    short lock;
    char  pad6[0xA];
    int   light;
    int   glow;
} BlasterVars;

typedef struct BlasterUsage {
    char           pad0[0x78];
    unsigned short uses;
    unsigned short longest;
    int            levels;
} BlasterUsage;

typedef struct AimTarget {
    char  pad0[0x10];
    float height;
} AimTarget;

typedef struct ShotSpawn {
    BlasterVec     pos;
    struct Moby   *owner;
    int            flags;
    unsigned char  b18;
    unsigned char  b19;
    unsigned short cls;
    float          f1C;
    int            i20;
} ShotSpawn;

typedef union BlasterWork {
    BlasterMatrix3 rotation;
    BlasterVec     point;
    ShotSpawn      spawn;
    struct {
        BlasterVec v;
        BlasterVec rnd;
    } spray;
} BlasterWork;

extern int  collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int  get_ammo(int) __asm__("FUN_L00_00233e98");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int  use_ammo(int, int) __asm__("FUN_L00_00233db8");
extern int
allocate_colored_effect_slot(void *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void  release_point_light_slot(int) __asm__("FUN_L00_0023e838");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
char        *FUN_L00_0026dd20(void *pos, void *dir, int a, int b, float f);
extern int   coll_line_fix(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int   advance_moby_animation(void *) __asm__("func_0020D580");
extern void
collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_0024f7c8(void *, int, void *);
void        cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void        transform_hero_local_offset_to_world(float *out,
                                                 float  x,
                                                 float  y,
                                                 float  z) __asm__("FUN_L00_00233660");
extern int  hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_001edf60_49a8(int, int, float, void *, float, int, int, float, int) __asm__("FUN_L00_001edf60");
extern struct Moby *
FUN_L00_002b46a8_49a8(struct Moby *, void *, float *, float *) __asm__("FUN_L00_002b46a8");
extern void FUN_L00_002caf70_49a8(void *, float, float, float, float, void *, void *) __asm__("FUN_L00_002caf70");
extern int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");

extern int                  D_0015EEA4;
extern s32                  D_0015EEA4;
extern unsigned char       *D_L00_0015FFE4_2b46a8 __asm__("D_L00_0015FFE4");
extern float D_L00_0017ACF4 NOT_SDA;
extern s32                  D_L00_0015F5C4_macro __asm__("D_L00_0015F5C4");

unsigned char *select_blaster_auto_aim_target(int       ign,
                                              V_2b46a8 *from,
                                              float    *yaw,
                                              float    *pitch) __asm__("FUN_L00_002b46a8");

unsigned char *select_blaster_auto_aim_target(int ign, V_2b46a8 *from, float *yaw, float *pitch) {
    V_2b46a8       aim_origin_position;
    V_2b46a8       tp;
    V_2b46a8       aim_probe_position;
    unsigned char *candidate_moby;
    unsigned char *r;
    unsigned char *best;
    float          bestd;
    float          candidate_yaw;
    float          b;
    float          dy;
    float          candidate_distance;
    float          aim_angle_error;
    float          target_radius;
    float          target_angular_radius;
    struct Hero   *g;

    best = 0;
    aim_origin_position.q = from->q;
    from = &aim_origin_position;
    bestd = 20.0f;
    for (candidate_moby = D_L00_0015FFE4_2b46a8; candidate_moby != 0;
         candidate_moby = ((unsigned char *)MOBY(candidate_moby)->next)) {
        if ((signed char)candidate_moby[0x20] < 0) {
            continue;
        }
        qcopy(&tp, candidate_moby + 0x10);
        r = suck_get_aim_height(candidate_moby);
        if (r != 0) {
            tp.f[2] += *(float *)(r + 0x10);
        } else {
            tp.f[2] += 0.5f;
        }
        if (!(MOBY(candidate_moby)->flags & 0x1000)) {
            continue;
        }
        candidate_yaw = fast_arc_tan(tp.f[0] - from->f[0], tp.f[1] - from->f[1]);
        b = fast_arc_tan(vector_distance_xy(from, &tp), tp.f[2] - from->f[2]);
        dy = fast_difference_between_rotations(*yaw, candidate_yaw);
        candidate_distance = fast_vec_dist(from, &tp);
        if (bestd < candidate_distance) {
            continue;
        }
        g = &hero;
        build_spherical_offset(
            &aim_probe_position, candidate_distance, *yaw, *pitch + g->ground_pitch.f * 0.5f);
        fast_vec_add(&aim_probe_position, &aim_probe_position, from);
        aim_angle_error = fast_arc_sin(fast_vec_dist(&aim_probe_position, &tp) /
                                       (candidate_distance + candidate_distance));
        aim_angle_error = aim_angle_error + aim_angle_error;
        if (0.15707964f < aim_angle_error) {
            r = suck_get_aim_height(candidate_moby);
            if (r != 0) {
                target_radius = convert_integer_to_float(r[0xA]) * 0.125f;
                target_angular_radius = fast_arc_sin(target_radius / candidate_distance);
                if (target_radius < candidate_distance) {
                    aim_angle_error -= target_angular_radius;
                    if (aim_angle_error < 0.0f) {
                        aim_angle_error = 0.0f;
                    }
                }
            }
        }
        if (aim_angle_error < 0.15707964f) {
            if (collision_line(g_debug_camera.position, &tp, 6, ign, 0) ||
                collision_line(from, &tp, 6, ign, 0)) {
                return best;
            }
            *pitch = -b;
            bestd = candidate_distance;
            if (dy < 0.17453292f) {
                *yaw = candidate_yaw;
            }
            best = candidate_moby;
        }
    }
    return best;
}

/* Blaster (class 168) update: aims (reticle in first person, auto-aim otherwise), fires a shot with its sparks,
   keeps the usage statistics, returns to idle and moves the muzzle light. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B5C98), where it is exact; names translated to the US level program. */
/* NEAR: a dozen spots; this compiler hoists the 3.0f spray constant out of the loop and rebuilds the zero target before its conditional move. */
void blaster_update(struct Moby *moby) __asm__("FUN_L00_002b49a8");

void blaster_update(struct Moby *moby) {
    BlasterVec     velocity;
    BlasterVec     aim_point;
    BlasterVec     launch;
    BlasterWork    work;
    BlasterMatrix4 inverse;
    BlasterVec     reach;
    BlasterVec     end;
    struct Moby   *target;
    float          yaw;
    float          pitch;
    float          base_yaw, base_pitch;
    float          red, green, blue;
    BlasterVars   *data;
    BlasterUsage  *stats;
    int            i;
    int            never;

    velocity.q = 0;
    target = 0;
    never = 0;
    pitch = 0.0f;
    data = (BlasterVars *)moby->pvars;
    if (moby->state == 0) {
        moby->state = 1;
        data->light = -1;
        data->glow = 0;
    }
    fast_dec_timer_s16(&data->lock);
    fast_dec_timer(&data->cooldown);
    if (hero.state.current == 1)
        hero_set_state(30, 1);
    if (moby->prev_seq == 0 && moby->prev_frame < 3)
        data->cooldown = scale_game_frames(7);
    if (hero.state.current == 30 && (hero.moby->flags & 1)) {
        copy_matrix3x4(&work.rotation, l00_camera.unk0);
        transpose_rotation_matrix(&inverse, &work.rotation);
        velocity.f.z = frame_time * 40.0f;
        transform_vector(&velocity, &velocity, &inverse);
        clear_u64_value(&launch);
        launch.f.x += 0.15f;
        launch.f.z += 0.75f;
        launch.f.y += 0.15f;
        transform_vector(&launch, &launch, &inverse);
        fast_vec_add(&launch, &launch, &l00_camera.pos);
        if (hero.state.current == 30 && (hero.moby->flags & 1) && !hero.item_use_locked) {
            FUN_L00_001edf60_49a8((int)moby, 0xFF0F0FFF, 1.0f, 0, 0.0f, 0x26, -1, 90.0f, 4);
            fast_vec_normalize(&reach, &velocity, 20.0f);
            fast_vec_add(&end, &reach, &launch);
            reach.q = end.q;
            /* Fakematch: a never-taken set of target on this path keeps gcse from
               folding target's 0 into the hit test below (retail keeps it in $fp). */
            if (never)
                target = (struct Moby *)moby->pvars;
            if (coll_line_fix(&l00_camera.pos, &reach, 0, (int)moby, 0)) {
                struct Moby *hit = coll_hit_moby;
                if (hit && (hit->flags & 0x1000))
                    target = hit;
            }
        }
    } else {
        velocity.f.x = frame_time * 40.0f;
        transform_vector_by_basis(&velocity, &velocity, &hero.basis);
        FUN_L00_0024f7c8(moby, 0, &launch);
    }
    if (hero.items[6].locked_on)
        yaw = hero.items[6].lock_yaw;
    else
        yaw = fast_arc_tan(velocity.f.x, velocity.f.y);
    pitch = -fast_arc_tan(vector_length_xy(&velocity), velocity.f.z);
    base_yaw = yaw;
    base_pitch = pitch;
    if (hero.state.current != 30) {
        if (hero.unk22C8 == 1) {
            yaw = moby->rot.z;
            base_yaw = yaw;
            pitch = D_L00_0017ACF4;
            base_pitch = pitch;
        }
        target = FUN_L00_002b46a8_49a8(moby, &launch, &yaw, &pitch);
    }
    if (target && data->lock == 0) {
        if (get_ammo(-1)) {
            AimTarget *aim = (AimTarget *)suck_get_aim_height(target);
            float      distance = fast_vec_dist(g_debug_camera.position, &target->pos) / 50.0f;
            float      height;
            qcopy(&work.point, &target->pos);
            height = 0.5f;
            if (aim)
                height = aim->height;
            work.point.f.z += height;
            FUN_L00_001edf60_49a8((int)moby,
                                  0xFF0FFF0F,
                                  absolute_float(0.9f - distance * 0.65f),
                                  &work.point,
                                  0.0f,
                                  0x26,
                                  -1,
                                  90.0f,
                                  4);
        }
    }
    if (hero.item_anim_active && hero.state.current != 0x72 && D_L00_0015F5C4_macro != 2) {
        if (scale_game_frames(5 - item_text_variant[15]) < hero.item_anim_frames &&
            data->cooldown == 0) {
            if (use_ammo(-1, 1) && !hero.item_use_locked) {
                data->cooldown = scale_game_frames(6);
                if (target) {
                    float      speed = frame_time * 40.0f;
                    AimTarget *aim;
                    qcopy(&aim_point, &target->pos);
                    aim = (AimTarget *)suck_get_aim_height(target);
                    if (aim)
                        aim_point.f.z += aim->height;
                    else
                        aim_point.f.z += 0.5f;
                    pitch = -fast_arc_tan(vector_distance_xy(&launch, &aim_point),
                                          aim_point.f.z - launch.f.z);
                    base_pitch -= hero.ground_pitch.f * 0.5f;
                    if (fast_difference_between_rotations(base_pitch, pitch) < 0.17453292f) {
                        base_pitch = pitch;
                    } else if (fast_add_rotations(base_pitch, 0.17453292f) < pitch) {
                        base_pitch = fast_add_rotations(base_pitch, 0.17453292f);
                    } else if (pitch < fast_add_rotations(base_pitch, -0.17453292f)) {
                        base_pitch = fast_add_rotations(base_pitch, -0.17453292f);
                    }
                    build_spherical_offset(&work.point, speed, base_yaw, -base_pitch);
                    fast_vec_add(&work.point, &work.point, &launch);
                    yaw = fast_arc_tan(aim_point.f.x - work.point.f.x,
                                       aim_point.f.y - work.point.f.y);
                    pitch = -fast_arc_tan(vector_distance_xy(&work.point, &aim_point),
                                          aim_point.f.z - work.point.f.z);
                } else if (hero.state.current != 30) {
                    pitch -= hero.ground_pitch.f * 0.5f;
                    base_pitch -= hero.ground_pitch.f * 0.5f;
                }
                work.spawn.flags = 0x10000;
                work.spawn.owner = moby;
                work.spawn.f1C = 0.25f;
                work.spawn.i20 = 1;
                work.spawn.pos.f.x = fast_cos(fast_arc_tan(hero.facing_x, hero.facing_y));
                work.spawn.pos.f.y = fast_sin(fast_arc_tan(hero.facing_x, hero.facing_y));
                work.spawn.pos.f.z = 1.0f;
                work.spawn.pos.f.w = 5627.9248f;
                work.spawn.b18 = 1;
                work.spawn.b19 = 1;
                work.spawn.cls = (*(unsigned short *)&moby->oclass);
                transform_hero_local_offset_to_world(&inverse, 0.7f, -0.15f, 0.5f);
                collect_mobies_in_sphere(&inverse, 0.42f, 5, hero.moby, &work.spawn);
                FUN_L00_002caf70_49a8(hero.moby, yaw, pitch, base_yaw, base_pitch, &launch, target);
                data->glow = scale_game_frames(15);
                for (i = 0; i < 5; i++) {
                    work.spray.rnd.q = 0;
                    work.spray.rnd.f.x = random_float_between(-1.0f, 1.0f);
                    work.spray.rnd.f.y = random_float_between(-1.0f, 1.0f);
                    work.spray.rnd.f.z = random_float_between(-1.0f, 1.0f);
                    work.spray.v.q = work.spray.rnd.q;
                    fast_vec_normalize(
                        &work.spray.v, &work.spray.v, fast_vec_length(&velocity) * 0.65f);
                    fast_vec_add(&work.spray.v, &velocity, &work.spray.v);
                    fast_vec_normalize(&work.spray.v,
                                       &work.spray.v,
                                       random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                    FUN_L00_0026dd20(
                        &launch,
                        &work.spray.v,
                        0x5F2F4F6F,
                        random_int_between(scale_game_frames(5), scale_game_frames(10)),
                        10000.0f);
                }
                fast_vec_normalize(&velocity, &velocity, frame_time * 0.5f);
                FUN_L00_0026dd20(&launch,
                                 &velocity,
                                 0x2F4F7F7F,
                                 random_int_between(scale_game_frames(4), scale_game_frames(7)),
                                 200000.0f);
            }
            if (moby->prev_seq != 4)
                blend_moby_animation(moby, 4, 0, scale_game_frames(7));
            stats = (BlasterUsage *)item_usage;
            if (stats->uses < 65535)
                stats->uses++;
            if (scale_game_frames(D_0015EEA4) / 600 > stats->longest)
                stats->longest = scale_game_frames(D_0015EEA4) / 600;
            stats->levels = (stats->levels | (1 << current_level_index)) | 0x80000000;
        }
    }
    if (!(((controller_state.held & hero.items[0].button_mask) && get_ammo(-1)) ||
          (!(scale_game_frames(9) < hero.item_anim_frames) && hero.item_anim_active)) ||
        hero.state.current == 0x72 || D_L00_0015F5C4_macro == 2) {
        if ((moby->prev_seq != 0 || (moby->anim_flags & 2)) && hero.state.current != 0x72 &&
            D_L00_0015F5C4_macro != 2) {
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(7));
        } else if (hero.state.current == 0x72 || D_L00_0015F5C4_macro == 2) {
            set_moby_animation(moby, 1, 0);
            advance_moby_animation(moby);
        }
        cancel_hero_item_animation();
        data->cooldown = 0;
    }
    if (moby->seq != 2 && data->glow > 0) {
        red = convert_integer_to_float(data->glow < scale_game_frames(9) ? data->glow
                                                                         : scale_game_frames(9)) *
              0.7f / (float)scale_game_frames(9);
        green = convert_integer_to_float(data->glow < scale_game_frames(9) ? data->glow
                                                                           : scale_game_frames(9)) *
                0.7f / (float)scale_game_frames(9);
        blue = convert_integer_to_float(data->glow < scale_game_frames(9) ? data->glow
                                                                          : scale_game_frames(9)) *
               0.7f / (float)scale_game_frames(9);
        work.point.f.x =
            hero.motion.pos.f[0] + fast_cos(fast_arc_tan(hero.facing_x, hero.facing_y)) * 2.0f;
        work.point.f.y =
            hero.motion.pos.f[1] + fast_sin(fast_arc_tan(hero.facing_x, hero.facing_y)) * 2.0f;
        work.point.f.z = hero.motion.pos.f[2] + 1.0f;
        if (data->light == -1) {
            data->light = allocate_colored_effect_slot(&work.point, 6.0f, 0.0f, red, green, blue);
        } else {
            PointLight *light = &point_lights[data->light];
            qcopy(&light->position, &work.point);
            light->position.f[3] = 6.0f;
            light->color[0] = red;
            light->color[1] = green;
            light->color[2] = blue;
        }
        fast_dec_timer(&data->glow);
    } else if (data->light != -1) {
        release_point_light_slot(data->light);
        data->light = -1;
        data->glow = 0;
    }
}
