/* Level 13 attached rocket moby class: create, launch, flight and explosions. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *create_attached_rocket(struct Moby *source_moby,
                             short        joint_index) __asm__("FUN_L13_003058a8");

char *create_attached_rocket(struct Moby *source_moby, short joint_index) {
    char *moby = create_moby(0x4D1);
    char *moby_vars;
    char *spawned_moby_position;
    if (moby != 0) {
        moby_vars = ((char *)MOBY(moby)->pvars);
        moby[0x31] = 1;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0x7F;
        moby[0x20] = 1;
        *(char **)(moby_vars + 0x20) = source_moby;
        *(short *)(moby_vars + 0x26) = joint_index;
        *(short *)(moby_vars + 0x24) = 1;
        spawned_moby_position = moby + 0x10;
        moby_point_position(source_moby, joint_index, spawned_moby_position);
        qcopy(moby + 0x40, &source_moby->rot);
        *(OvlQuad *)&MOBY(moby)->rot_row0 = *(OvlQuad *)&source_moby->rot_row0;
        *(OvlQuad *)&MOBY(moby)->rot_row1 = *(OvlQuad *)&source_moby->rot_row1;
        *(OvlQuad *)&MOBY(moby)->rot_row2 = *(OvlQuad *)&source_moby->rot_row2;
        qcopy(moby_vars + 0x10, spawned_moby_position);
        MOBY(moby)->collision = 0;
        update_entity(moby);
    }
    return moby;
}

void launch_attached_rocket(struct Moby *moby,
                            char        *pos,
                            int          distance_reference_moby,
                            float        max_travel_distance) __asm__("FUN_L13_00305988");

void launch_attached_rocket(struct Moby *moby,
                            char        *pos,
                            int          distance_reference_moby,

                            float max_travel_distance) {
    char *moby_vars = (char *)moby->pvars;
    char *source_moby = *(char **)(moby_vars + 0x20);
    char *mp;
    *(float *)(moby_vars + 0x28) = max_travel_distance;
    qcopy(&moby->rot, source_moby + 0x40);
    mp = &moby->pos;
    *(OvlQuad *)&moby->rot_row0 = *(OvlQuad *)&MOBY(source_moby)->rot_row0;
    *(OvlQuad *)&moby->rot_row1 = *(OvlQuad *)&MOBY(source_moby)->rot_row1;
    *(OvlQuad *)&moby->rot_row2 = *(OvlQuad *)&MOBY(source_moby)->rot_row2;
    moby_point_position(*(void **)(moby_vars + 0x20), *(short *)(moby_vars + 0x26), mp);
    qcopy(moby_vars + 0x10, mp);
    qcopy(moby_vars, pos);
    *(int *)(moby_vars + 0x2C) = distance_reference_moby;
    moby->state = 0;
    moby->flags &= 0xFFFE;
    moby->collision = moby->pclass->collision;
    update_entity(moby);
}

extern char *FUN_L00_0025a420(void *, int, int);

extern float FUN_002132a8(float, float);

extern int FUN_L00_002712b8(void *, void *, int, int, int, int, float, float, float, float, float);

extern void FUN_L00_0026dd20(void *, void *, int, int, float);

extern void spawn_projectile_expiry_particle(void *, void *) __asm__("FUN_L07_0029a2a0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float D_0015ED6C;

extern float D_L13_0015F580[] MACRO_ADDR;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) L13Vector_5a58;

extern CollisionHit coll_hit_1638 __asm__("D_L13_00174140");

void update_attached_rocket(struct Moby *moby) __asm__("FUN_L13_00305a58");

void spawn_attached_rocket_non_damaging_explosion(struct Moby *moby) __asm__("FUN_L13_00306148");

void spawn_attached_rocket_explosion(struct Moby *moby) __asm__("FUN_L13_003060b8");

void update_attached_rocket(struct Moby *moby) {
    char        *data = (char *)moby->pvars;
    struct Moby *source;
    struct Moby *target;
    struct {
        L13Vector_5a58 old_pos;
        L13Vector_5a58 velocity;
        L13Vector_5a58 direction;
        char           hit[48];
        L13Vector_5a58 random_direction;
    } work;
    int   i;
    int   stationary;
    float length;
    float distance;
    float other_distance;

    qcopy(&work.old_pos, &moby->pos);
    if (moby->state == 1) {
        source = *(struct Moby **)(data + 0x20);
        if (source == 0 || source->oclass != 0x65 || source->state == 0xfe || source->state == 0xfd)
            goto remove;
        qcopy(&moby->rot, &source->rot);
        *(OvlQuad *)&moby->rot_row0 = *(OvlQuad *)&source->rot_row0;
        *(OvlQuad *)&moby->rot_row1 = *(OvlQuad *)&source->rot_row1;
        *(OvlQuad *)&moby->rot_row2 = *(OvlQuad *)&source->rot_row2;
        moby_point_position(*(struct Moby **)(data + 0x20), *(short *)(data + 0x26), &moby->pos);
        qcopy(data + 0x10, &moby->pos);
        {
            struct Moby *source = *(struct Moby **)(data + 0x20);
            if (source != 0 && source->state != 0xfe && source->state != 0xfd &&
                (source->flags & 1))
                moby->flags |= 1;
            else
                moby->flags &= 0xfffe;
        }
        goto bounds;
    }

    {
        char *target = FUN_L00_0025a420(moby, 0x230000, 0);
        if (target != 0 && *(struct Moby **)(target + 0x20) != 0 &&
            (*(struct Moby **)(target + 0x20))->oclass == 0x1df)
            goto special;
    }
    fast_vec_add(&moby->pos, &moby->pos, data);
    for (i = 0; i < 4; i++) {
        fast_vec_scale(&work.direction, data, -convert_integer_to_float(i) * 0.25f);
        fast_vec_add(&work.velocity, &moby->pos, &work.direction);
        length = FUN_002132a8(25000.0f, 35000.0f);
        target = (struct Moby *)FUN_L00_002712b8(&work.velocity,
                                                 D_L13_0015F580,
                                                 scale_game_frames(15),
                                                 0x60,
                                                 0x4040ff,
                                                 3,
                                                 length,
                                                 500.0f,
                                                 1.0f,
                                                 -0.0004f,
                                                 0.0f);
        if (target != 0)
            *((char *)target + 9) = truncate_float_to_s32(4.0f) - 0x80;
    }
    qcopy(&work.velocity, data);
    work.velocity.z = 0.0f;
    set_vector_length_xy(&work.velocity, &work.velocity, 1.0f);
    work.velocity.z = 1.0f;
    make_hit_descriptor(work.hit, moby, 0x10001, 1.0f, &work.velocity);
    stationary = *(short *)(data + 0x24) == 0;
    distance = vector_distance_xy(&moby->pos, data + 0x10);
    other_distance = vector_distance_xy(*(char **)(data + 0x2c) + 0x10, data + 0x10);
    if (*(float *)(data + 0x28) < distance || other_distance < distance) {
        if (moby->visible != 0) {
            int i;
            for (i = 0; i < 3; i++) {
                *(OvlQuad *)&work.random_direction = 0;
                work.random_direction.x = FUN_002132a8(-1.0f, 1.0f);
                work.random_direction.y = FUN_002132a8(-1.0f, 1.0f);
                work.random_direction.z = FUN_002132a8(-1.0f, 1.0f);
                *(OvlQuad *)&work.direction = *(OvlQuad *)&work.random_direction;
                length = fast_vec_length(&work.random_direction);
                fast_vec_normalize(&work.direction, &work.direction, length * 0.1f);
                fast_vec_add(&work.direction, &work.random_direction, &work.direction);
                fast_vec_normalize(&work.direction,
                                   &work.direction,
                                   FUN_002132a8(D_0015ED6C + D_0015ED6C, D_0015ED6C * 4.0f));
                spawn_projectile_expiry_particle(&moby->pos, &work.direction);
            }
        }
        spawn_attached_rocket_explosion(moby);
        goto remove;
    }
    if (collision_line(&work.old_pos, &moby->pos, stationary, moby, work.hit) == 0)
        goto bounds;
    {
        struct Moby *target = (struct Moby *)coll_hit_1638.moby;
        if (target != 0 && target == *(struct Moby **)(data + 0x20))
            goto bounds;
    }
    qcopy(&moby->pos, &coll_hit_1638.point);
    if (moby->visible != 0) {
        int i;
        for (i = 0; i < 5; i++) {
            *(OvlQuad *)&work.direction = 0;
            work.direction.x = FUN_002132a8(-1.0f, 1.0f);
            work.direction.y = FUN_002132a8(-1.0f, 1.0f);
            work.direction.z = FUN_002132a8(-1.0f, 1.0f);
            *(OvlQuad *)&work.random_direction = *(OvlQuad *)&work.direction;
            reflect_vector(&work.direction, data, &coll_hit_1638.normal_x);
            length = fast_vec_length(&work.direction);
            fast_vec_normalize(&work.random_direction, &work.random_direction, length * 0.5f);
            fast_vec_add(&work.random_direction, &work.direction, &work.random_direction);
            fast_vec_normalize(&work.random_direction,
                               &work.random_direction,
                               FUN_002132a8(D_0015ED6C * 3.0f, D_0015ED6C * 6.0f));
            FUN_L00_0026dd20(&moby->pos,
                             &work.random_direction,
                             0x7f2f4f6f,
                             random_int_between(scale_game_frames(10), scale_game_frames(15)),
                             30000.0f);
        }
    }
    {
        struct Moby *target = (struct Moby *)coll_hit_1638.moby;
        if (target != 0 && target->oclass == 0x1df) {
        special:
            spawn_attached_rocket_non_damaging_explosion(moby);
        } else {
            spawn_attached_rocket_explosion(moby);
        }
    }
remove:
    mark_moby_for_removal(moby);
    return;
bounds:
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal(moby);
}

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

void spawn_attached_rocket_explosion(struct Moby *moby) {
    char vector[16];
    clear_u64_value(vector);
    func_L00_0025F4A8_alt(moby,
                          vector,
                          &moby->pos,
                          2.0f,
                          1.0f,
                          12,
                          4,
                          18,
                          4.0f,
                          3.0f,
                          1.0f,
                          2.0f,
                          0,
                          20.0f,
                          1,
                          1,
                          -1,
                          0);
}

extern void spawn_explosion(void *,
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

void spawn_attached_rocket_non_damaging_explosion(struct Moby *moby) {
    char vector[16];
    clear_u64_value(vector);
    spawn_explosion(moby,
                    vector,
                    &moby->pos,
                    0.0f,
                    0.0f,
                    5,
                    1,
                    5,
                    0.0f,
                    0.0f,
                    0.0f,
                    1.0f,
                    0,
                    0.0f,
                    0,
                    1,
                    -1,
                    0);
}
