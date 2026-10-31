/* Level 13 Gemlik projectile moby class and its spawn helper. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "qzero.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");

extern void FUN_L00_0026dd20(void *, void *, int, int, float);

extern void spawn_projectile_expiry_particle(void *, void *) __asm__("FUN_L07_0029a2a0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float D_0015ED6C;

extern CollisionHit coll_hit_1638 __asm__("D_L13_00174140");

char *spawn_gemlik_projectile(float  speed,
                              float *spawn_position,
                              float *launch_displacement,
                              void  *source_moby) __asm__("FUN_L13_003061c8");

char *spawn_gemlik_projectile(float  speed,
                              float *spawn_position,
                              float *launch_displacement,

                              void *source_moby) {
    char *moby;
    char *moby_vars;
    float direction_length_xy;
    moby = create_moby(0x4D6);
    if (moby != 0) {
        *(int *)(moby + 0x40) = 0;
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0x7F;
        moby[0x31] = 1;
        moby_vars = ((char *)MOBY(moby)->pvars);
        direction_length_xy = vector_length_xy(launch_displacement);
        MOBY(moby)->rot.y = -fast_arc_tan(direction_length_xy, launch_displacement[2]);
        MOBY(moby)->rot.z = fast_arc_tan(launch_displacement[0], launch_displacement[1]);
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby_vars + 0x10, spawn_position);
        qcopy(moby_vars, launch_displacement);
        blend_moby_animation(moby, 1, 0, scale_game_frames(5));
        MOBY(moby)->flags = 0x208;
        *(int *)(moby_vars + 0x24) = 1;
        *(void **)(moby_vars + 0x20) = source_moby;
        *(float *)(moby_vars + 0x28) = speed;
        moby[0x23] = 0x10;
        *(int *)(moby_vars + 0x2C) =
            truncate_float_to_s32(speed / fast_vec_length(moby_vars) + 1.0f);
        update_entity(moby);
    }
    return moby;
}

typedef int OvlQuad_p __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) L13Vec_p;

struct L13Ent00178480 {
    char           pad0[0x24];
    int            i24;
    unsigned char  b28;
    unsigned char  b29;
    unsigned short s2A;
    float          f2C;
    int            pad30;
    char          *p34;
    char           pad38[8];
};

extern float fast_vec_dist(void *, void *);

extern int FUN_L00_00257b90(int, int);

extern struct L13Ent00178480 l13_moby_hit_entries[] __asm__("D_L13_00178400");

extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");

void gemlik_projectile_update(struct Moby *moby) __asm__("FUN_L13_00306300");

/* Moby class 1238 */
void gemlik_projectile_update(struct Moby *moby) {
    char *projectile_vars = (char *)moby->pvars;
    struct {
        L13Vec_p old_pos;
        char     hit[48];
        L13Vec_p velocity;
        L13Vec_p direction;
        L13Vec_p random_direction;
    } work;
    int                    i;
    int                    flags;
    int                    stationary;
    unsigned char         *hero_bytes;
    char                  *ent;
    struct L13Ent00178480 *target_hit_entry;

    qcopy(&work.old_pos, &moby->pos);
    fast_vec_add(&moby->pos, &moby->pos, projectile_vars);
    qcopy(&work.velocity, projectile_vars);
    work.velocity.z = 0.0f;
    set_vector_length_xy(&work.velocity, &work.velocity, 1.0f);
    work.velocity.z = 1.0f;
    flags = 0x10001;
    hero_bytes = (unsigned char *)((char *)((char *)&hero));
    if (hero_bytes[0x1FF6] != 0 && *(float *)(projectile_vars + 0xC) != 31337.6015625f &&
        fast_vec_dist(&moby->pos, hero_bytes + 0x80) < 2.0f) {
        fast_vec_normalize(projectile_vars, projectile_vars, 0.5f);
        fast_vec_sub(&moby->pos, &moby->pos, projectile_vars);
        qzero(projectile_vars);
        *(int *)(projectile_vars + 0x2C) = 5;
        *(float *)(projectile_vars + 0xC) = 31337.6015625f;
        for (i = 0; i < 6; i++) {
            unsigned char *hero_base = (unsigned char *)((char *)((char *)&hero));
            ent = ((char **)(hero_base + 0x2020))[i];
            if (ent != 0) {
                int hit_mask = flags | 1;
                make_hit_descriptor(work.hit, moby, hit_mask, 1.0f, &work.velocity);
                work.hit[0x18] = 1;
                *(unsigned short *)(work.hit + 0x1A) = *(unsigned short *)((char *)moby + 0xA6);
                work.hit[0x19] = 1;
                if ((unsigned char)ent[0xA4] == 0xFF ||
                    (target_hit_entry = &l13_moby_hit_entries[(unsigned char)ent[0xA4]])->p34 !=
                        ent) {
                    record_moby_hit(ent, work.hit);
                } else {
                    target_hit_entry->f2C += 1.0f;
                    l13_moby_hit_entries[(unsigned char)ent[0xA4]].b28 = 1;
                    l13_moby_hit_entries[(unsigned char)ent[0xA4]].b29 = 1;
                    l13_moby_hit_entries[(unsigned char)ent[0xA4]].s2A =
                        *(unsigned short *)((char *)moby + 0xA6);
                    {
                        struct L13Ent00178480 *matched_target_hit_entry =
                            &l13_moby_hit_entries[(unsigned char)ent[0xA4]];
                        matched_target_hit_entry->i24 = hit_mask;
                    }
                }
                break;
            }
        }
    }

    make_hit_descriptor(work.hit, moby, 0x10001, 1.0f, &work.velocity);
    *(unsigned short *)(work.hit + 0x1A) = *(unsigned short *)((char *)moby + 0xA6);
    work.hit[0x18] = 1;
    work.hit[0x19] = 1;
    stationary = *(int *)(projectile_vars + 0x24) == 0;
    if (*(float *)(projectile_vars + 0x28) <
            vector_distance_xy(&moby->pos, projectile_vars + 0x10) ||
        fast_dec_timer(projectile_vars + 0x2C) != 0) {
        if (moby->visible != 0) {
            int i;
            for (i = 0; i < 3; i++) {
                *(OvlQuad_p *)&work.random_direction = 0;
                work.random_direction.x = random_float_between(-1.0f, 1.0f);
                work.random_direction.y = random_float_between(-1.0f, 1.0f);
                work.random_direction.z = random_float_between(-1.0f, 1.0f);
                *(OvlQuad_p *)&work.direction = *(OvlQuad_p *)&work.random_direction;
                fast_vec_normalize(&work.direction,
                                   &work.direction,
                                   fast_vec_length(&work.random_direction) * 0.1f);
                fast_vec_add(&work.direction, &work.random_direction, &work.direction);
                fast_vec_normalize(
                    &work.direction,
                    &work.direction,
                    random_float_between(D_0015ED6C + D_0015ED6C, D_0015ED6C * 4.0f));
                spawn_projectile_expiry_particle(&moby->pos, &work.direction);
            }
        }
        goto remove;
    }
    if (collision_line(
            &work.old_pos, &moby->pos, stationary, *(void **)(projectile_vars + 0x20), work.hit) ==
        0)
        goto bounds;
    qcopy(&moby->pos, &coll_hit_1638.point);
    if (moby->visible != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad_p *)&work.random_direction = 0;
            work.random_direction.x = random_float_between(-1.0f, 1.0f);
            work.random_direction.y = random_float_between(-1.0f, 1.0f);
            work.random_direction.z = random_float_between(-1.0f, 1.0f);
            *(OvlQuad_p *)&work.direction = *(OvlQuad_p *)&work.random_direction;
            reflect_vector(&work.random_direction, projectile_vars, &coll_hit_1638.normal_x);
            fast_vec_normalize(
                &work.direction, &work.direction, fast_vec_length(&work.random_direction) * 0.5f);
            fast_vec_add(&work.direction, &work.random_direction, &work.direction);
            fast_vec_normalize(&work.direction,
                               &work.direction,
                               random_float_between(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
            FUN_L00_0026dd20(&moby->pos,
                             &work.direction,
                             0x7F2F4F6F,
                             FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(15)),
                             30000.0f);
        }
    }
    {
        struct Moby *target = (struct Moby *)coll_hit_1638.moby;
        if (target != 0 && *(short *)((char *)target + 0xA6) == 0)
            allocate_voice_for_target_entry(0, 0, (int)moby);
    }
remove:
    mark_moby_for_removal(moby);
    return;
bounds:
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal(moby);
}

extern char *create_moby(int);
