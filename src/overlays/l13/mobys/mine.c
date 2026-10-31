/* Level 13 mine moby class (0x53) that Qwark's starfighter launches: it drifts to the hero's ship and explodes on contact, and its spawn helper. */
#include "types.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/l13/mobys/projectile_moby.h"

extern char D_0013F350[];

extern float D_0015ED6C;

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

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

extern char *create_moby(int);

unsigned char *spawn_level_13_mine(char *owner,
                                   char *spawn_position,
                                   char *initial_velocity) __asm__("FUN_L13_002c1f28");

unsigned char *

spawn_level_13_mine(char *firing_moby, char *spawn_position, char *initial_velocity) {
    unsigned char *moby = (unsigned char *)create_moby(0x53);
    if (moby != 0) {
        char *projectile_data;
        moby[0x30] = 0xFF;
        *(short *)((char *)moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        *(float *)((char *)moby + 0x2C) = *(float *)(*(char **)((char *)moby + 0x24) + 0x24) * 0.1f;
        projectile_data = *(char **)((char *)moby + 0x78);
        qcopy((char *)moby + 0x10, spawn_position);
        qcopy(projectile_data + 0x60, initial_velocity);
        *(char **)(projectile_data + 0x74) = firing_moby;
        projectile_data[0x28] = 0;
        *(float *)(projectile_data + 0x20) = 20.0f;
        *(short *)(projectile_data + 0x24) = 0x14;
        *(float *)(projectile_data + 0x30) = 0.5f;
        projectile_data[0x29] = 1;
        *(int *)(projectile_data + 0x70) = scale_game_frames(0x168);
        if (moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, 10);
        }
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

extern float D_0015ED60;

extern struct {
    char            pad[0x18];
    ProjectileMoby *moby;
} D_L13_00174140;

extern void make_hit_descriptor(float, void *, void *, int, void *) __asm__("FUN_L00_00259888");

extern int FUN_L00_001f0d60_2048(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

extern char *FUN_L00_0025a420(void *, int, int);

void level_13_mine_update(ProjectileMoby *moby) __asm__("FUN_L13_002c2048");

/* Moby class 83 */
void level_13_mine_update(ProjectileMoby *moby) {
    char           *moby_data = (char *)moby->data;
    ProjectileVec4  steering_and_collision_vector;
    float           hitpos[12];
    ProjectileVec4 *pos;
    ProjectileVec4 *velocity;
    ProjectileHero *hero_state;
    ProjectileMoby *target;
    ProjectileMoby *hit;
    int             flags;
    float           distance;
    float           factor;
    float           magnitude;
    short           cls_id;

    moby->scale += ((moby->cls->scale + moby->cls->scale) - moby->scale) * (D_0015ED60 * 0.1f);
    if ((moby->pad50[0x20] & 2) != 0 && (unsigned char)moby->pad50[2] != 2 &&
        (unsigned char)moby->pad50[3] != 2) {
        blend_moby_animation(moby, 2, 0, 10);
    }
    pos = &moby->pos;
    velocity = (ProjectileVec4 *)(moby_data + 0x60);
    hero_state = (ProjectileHero *)D_0013F350;
    if (hero_state->veh != 0 && (distance = fast_vec_dist(pos, &hero_state->veh->pos)) < 60.0f &&
        moby->b31 != 0) {
        factor = 1.0f - distance / 60.0f;
        if (factor > 1.0f)
            factor = 1.0f;
        magnitude = D_0015ED6C * 1.5f;
        factor *= magnitude;
        fast_vec_sub(&steering_and_collision_vector, &hero_state->veh->pos, pos);
        fast_vec_normalize(&steering_and_collision_vector, &steering_and_collision_vector, factor);
        fast_vec_add(velocity, velocity, &steering_and_collision_vector);
    }
    if (*(int *)(moby_data + 0x70) < scale_game_frames(0x168) - 9) {
        fast_vec_add(pos, pos, velocity);
        fast_vec_scale(velocity, velocity, 0.945f);
    } else {
        fast_vec_sub(
            &steering_and_collision_vector, pos, &(*(ProjectileMoby **)(moby_data + 0x74))->pos);
        fast_vec_normalize(
            &steering_and_collision_vector, &steering_and_collision_vector, D_0015ED6C * 20.0f);
        fast_vec_add(pos, pos, velocity);
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    target = *(ProjectileMoby **)(moby_data + 0x74);
    if (target != 0 && target->state != 0xFE && target->state != 0xFD &&
        fast_vec_dist(pos, &target->pos) > 8.0f) {
        make_hit_descriptor(16.0f, &hitpos, moby, 0x30000, velocity);
        qcopy(&steering_and_collision_vector, pos);
        steering_and_collision_vector.z += 0.75f;
        flags = collect_mobies_in_sphere(1.7f, &steering_and_collision_vector, 0x10, moby, &hitpos);
        flags = flags | FUN_L00_001f0d60_2048(&steering_and_collision_vector, 1.7f, 4, moby);
        hit = (ProjectileMoby *)FUN_L00_0025a420(moby, -1, 0);
        if (hit != 0) {
            float health = *(float *)(moby_data + 0x20) - *(float *)((char *)hit + 0x2C);
            *(float *)(moby_data + 0x20) = health;
            if (health <= 0.0f) {
                flags |= 0x2000;
                D_L13_00174140.moby = 0;
            }
        }
        if (D_L13_00174140.moby != 0) {
            cls_id = D_L13_00174140.moby->cls_id;
            if (cls_id == 0x504 || cls_id == 0x534 || cls_id == 0x535 || cls_id == 0x536 ||
                cls_id == 0x537 || cls_id == 0x538 || cls_id == 0x52 || cls_id == 0x53 ||
                cls_id == 0x539 || cls_id == 0x53A || cls_id == 0x191 || cls_id == 0x184 ||
                cls_id == 0x185 || cls_id == 0x186 || cls_id == 0x187 || cls_id == 0x188 ||
                cls_id == 0x189 || cls_id == 0x18A || cls_id == 0x18B || cls_id == 0x18C ||
                cls_id == 0x18F || cls_id == 0x190)
                flags = 0;
        }
        if (flags != 0) {
            if (moby->b31 != 0) {
                if ((flags & 0x2000) != 0) {
                    func_L00_0025F4A8_alt(moby,
                                          velocity,
                                          0,
                                          0.0f,
                                          0.0f,
                                          5,
                                          2,
                                          7,
                                          0.0f,
                                          0.0f,
                                          0.0f,
                                          1.0f,
                                          3,
                                          0.0f,
                                          1,
                                          1,
                                          -1,
                                          0);
                } else {
                    func_L00_0025F4A8_alt(moby,
                                          velocity,
                                          0,
                                          4.0f,
                                          16.0f,
                                          0x14,
                                          5,
                                          0x10,
                                          7.0f,
                                          4.0f,
                                          1.0f,
                                          1.0f,
                                          0,
                                          15.0f,
                                          1,
                                          1,
                                          -1,
                                          0);
                }
            }
            mark_moby_for_removal(moby);
            return;
        }
    }
    if (fast_dec_timer(moby_data + 0x70) != 0 && moby->b31 == 0) {
        mark_moby_for_removal(moby);
    }
}

extern unsigned char *
spawn_level_13_mine(char *owner, char *pos, char *vec) __asm__("FUN_L13_002c1f28");
