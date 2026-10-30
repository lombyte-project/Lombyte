/* Level 13 asteroid moby class: hits and fragments. */
#include "types.h"
#include "qzero.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/level_labels.h"

enum AsteroidSpinAxis {
    SPIN_AXIS_X = 0x0,
    SPIN_AXIS_Y = 0x1,
    SPIN_AXIS_Z = 0x2
};

extern f32 random_angle_radians(void) __asm__("FUN_00213308");

extern f32 random_float_between(f32, f32) __asm__("FUN_002132a8");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern char *create_moby(int);

extern float random_float_between(float, float) __asm__("FUN_002132a8");

extern int FUN_L00_00257b90(int, int);

extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

extern s32 rand(void);

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

extern float random_angle_radians(void) __asm__("FUN_00213308");

extern s32 rand();

char *spawn_asteroid_fragment(char *owner,
                              char *spawn_position,
                              char *initial_velocity,
                              int   remaining_split_count,
                              float scale) __asm__("FUN_L13_002e1140");

char *spawn_asteroid_fragment(char *owner,
                              char *spawn_position,
                              char *initial_velocity,
                              int   remaining_split_count,

                              float scale) {
    char *moby = create_moby(0x584);
    if (moby != 0) {
        char *moby_data;
        float spin_increment;
        ((unsigned char *)moby)[0x30] = 0xFF;
        moby_data = ((char *)MOBY(moby)->pvars);
        MOBY(moby)->draw_dist = *(unsigned short *)(owner + 0x32);
        moby[0x31] = 1;
        moby[0x20] = 2;
        moby[0xBC] = 0;
        moby[0x72] = 0;
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby_data, initial_velocity);
        MOBY(moby)->scale = scale;
        *(char **)(moby_data + 0x48) = owner;
        *(int *)(moby_data + 0x40) = 1;
        *(short *)(moby_data + 0x4C) = scale_game_frames(0x1E);
        *(short *)(moby_data + 0x4E) = remaining_split_count;
        MOBY(moby)->collision = 0;
        spin_increment = random_float_between(-30.0f, 30.0f) * DEG_TO_RAD * frame_time;
        switch (rand() % 3) {
        case SPIN_AXIS_X:
            *(float *)(moby_data + 0x30) = spin_increment;
            break;
        case SPIN_AXIS_Y:
            *(float *)(moby_data + 0x34) = spin_increment;
            break;
        case SPIN_AXIS_Z:
            *(float *)(moby_data + 0x38) = spin_increment;
            break;
        }
        spin_increment = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD * frame_time;
        switch (rand() % 3) {
        case SPIN_AXIS_X:
            *(float *)(moby_data + 0x30) = spin_increment;
            break;
        case SPIN_AXIS_Y:
            *(float *)(moby_data + 0x34) = spin_increment;
            break;
        case SPIN_AXIS_Z:
            *(float *)(moby_data + 0x38) = spin_increment;
            break;
        }
        MOBY(moby)->rot.x = random_angle_radians();
        MOBY(moby)->rot.y = random_angle_radians();
        MOBY(moby)->rot.z = random_angle_radians();
        update_entity(moby);
        copy_hero_moby_field_38(moby);
    }
    return moby;
}

char *spawn_asteroid_fragment(char *owner, char *pos, char *vec, int a3, float f) __asm__("FUN_L13_002e1140");

extern char D_L13_00160680[] MACRO_ADDR;

extern void func_L00_0025F4A8_s(void *,
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

void process_asteroid_hit(struct Moby *moby,
                          char        *blob_data,
                          float        scale_ratio) __asm__("FUN_L13_002e1348");

void process_asteroid_hit(struct Moby *moby, char *blob_data, float scale_ratio) {
    float split_velocity[4];
    float split_spawn_position[4];
    float nearby_effect_scale = scale_ratio * 7.0f;
    float proximity_gated_effect_scale;
    int   i;
    proximity_gated_effect_scale = scale_ratio * 10.0f;
    if (20.0f < fast_vec_dist(&moby->pos, &hero.motion.pos)) {
        nearby_effect_scale = proximity_gated_effect_scale = 0.0f;
    }
    allocate_voice_for_target_entry(0, 0, (int)moby);
    if (*(short *)(blob_data + 0x4C) == 0) {
        func_L00_0025F4A8_s(moby,
                            D_L13_00160680,
                            0,
                            nearby_effect_scale,
                            proximity_gated_effect_scale,
                            truncate_float_to_s32(scale_ratio * 10.0f),
                            truncate_float_to_s32(scale_ratio * 3.0f),
                            truncate_float_to_s32(scale_ratio * 16.0f),
                            scale_ratio * 4.0f,
                            scale_ratio + scale_ratio,
                            9.0f,
                            1.0f,
                            -1,
                            scale_ratio * 15.0f,
                            0,
                            1,
                            -1,
                            0);
    }
    moby->unk72 = 0;
    if (*(short *)(blob_data + 0x4E) > 0) {
        for (i = 0; i < FUN_L00_00257b90(1, 2); i++) {
            float part = moby->scale * random_float_between(0.25f, 0.75f);
            random_vector(split_velocity, frame_time * 5.0f, frame_time * 10.0f);
            fast_vec_scale(blob_data, blob_data, 0.75f);
            fast_vec_add(split_velocity, split_velocity, blob_data);
            fast_vec_normalize(split_spawn_position, split_velocity, 0.5f);
            fast_vec_add(split_spawn_position, split_spawn_position, &moby->pos);
            fast_vec_add(split_spawn_position, split_spawn_position, split_velocity);
            spawn_asteroid_fragment(moby,
                                    (char *)split_spawn_position,
                                    (char *)split_velocity,
                                    *(short *)(blob_data + 0x4E) - 1,
                                    part);
            moby->scale -= part;
            if (--*(short *)(blob_data + 0x4E) == 0)
                break;
            if (moby->scale / moby->pclass->scale <= 0.2f)
                break;
        }
        moby->collision = 0;
        *(short *)(blob_data + 0x4C) = scale_game_frames(0x1E);
    } else if (*(int *)(blob_data + 0x40) == 0) {
        moby->state = 4;
        moby->flags |= 1;
        moby->collision = 0;
        moby->visible = 0;
        moby->scale = *(float *)(blob_data + 0x2C);
    } else {
        mark_moby_for_removal(moby);
    }
}

struct BlobVars {
    Vec4f        vel;         /* 0x00: moved by it each frame once launched */
    Vec4f        home;        /* 0x10: respawn point; w: 20 */
    f32          dir[3];      /* 0x20: launch direction (rot_row0 at spawn) */
    f32          start_scale; /* 0x2C */
    Vec4f        spin;        /* 0x30: x/y/z rotation per frame */
    s32          child;       /* 0x40: 1 when split off a bigger blob */
    s32          timer;       /* 0x44: frames left before it respawns */
    struct Moby *owner;       /* 0x48: blob it split off */
    s16          cooldown;    /* 0x4C: frames before it can hit again */
    s16          pieces;      /* 0x4E: blobs left to split off */
};

extern s32 random_int_between(s32, s32) __asm__("FUN_L00_00257b90");

extern s32 rand_1638(void) __asm__("FUN_001160d8");

extern s32 FUN_L00_001f0d60_1638(f32, void *, s32, struct Moby *) __asm__("FUN_L00_001f0d60");

extern char *find_moby_hit_1638(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");

extern void
FUN_L13_002e1348_1638(struct Moby *, struct BlobVars *, f32) __asm__("FUN_L13_002e1348");

extern Vec4f hero_pos __asm__("D_0013F3D0");

extern CollisionHit coll_hit_1638 __asm__("D_L13_00174140");

void asteroid_update(struct Moby *moby) __asm__("FUN_L13_002e1638");

void asteroid_update(struct Moby *moby) {
    enum {
        ASTEROID_INITIALIZE = 0,
        ASTEROID_WAIT_TO_LAUNCH = 1,
        ASTEROID_MOVE_AND_COLLIDE = 2,
        ASTEROID_RESET_AT_HOME = 3,
        ASTEROID_WAIT_TO_RESPAWN = 4
    };
    struct BlobVars *vars = (struct BlobVars *)moby->pvars;
    f32              distance = fast_vec_dist(&moby->pos, &hero_pos);
    f32              size;
    f32              spin;
    char            *hit;

    switch (moby->state) {
    case ASTEROID_INITIALIZE:
        qcopy(&vars->home, &moby->pos);
        vars->home.w = 20.0f;
        qcopy(vars->dir, &moby->rot_row0);
        vars->start_scale = moby->scale;
        vars->pieces = random_int_between(2, 3);
        if (moby->scale < moby->pclass->scale) {
            moby->unk72 = 0;
        }
        moby->state = 3;
        break;
    case ASTEROID_WAIT_TO_LAUNCH:
        if (hero.state.current == 0x32 && hero.ship_moby != 0 && hero.ship_moby->oclass == 0x45 &&
            (moby->visible != 0 || distance < 40.0f)) {
            fast_vec_normalize(vars, vars->dir, random_float_between(1.0f, 10.0f) * frame_time);
            moby->state = 2;
        }
        break;
    case ASTEROID_MOVE_AND_COLLIDE:
        fast_dec_timer_s16(&vars->cooldown);
        fast_vec_add(&moby->pos, &moby->pos, vars);
        if (moby->visible != 0 || distance < 40.0f) {
            if (vars->child == 0) {
                vars->timer = scale_game_frames(0x168);
            } else {
                vars->timer = scale_game_frames(0x78);
            }
            if (vars->cooldown == 0) {
                moby->collision = moby->pclass->collision;
                size = moby->scale / moby->pclass->scale;
                if ((frame_count & 1) == (((char *)moby - (char *)moby_base) >> 8 & 1) &&
                    FUN_L00_001f0d60_1638(size * 6.2f, &moby->pos, 0, moby) != 0) {
                    if ((struct Moby *)coll_hit_1638.moby != vars->owner) {
                        reflect_vector(vars, vars, &coll_hit_1638.normal_x);
                        qcopy(&moby->pos, coll_hit_1638.pad30);
                        if (coll_hit_1638.moby == 0 ||
                            ((struct Moby *)coll_hit_1638.moby)->oclass != 0xD4) {
                            FUN_L13_002e1348_1638(moby, vars, size);
                        }
                    }
                } else {
                    hit = find_moby_hit_1638(moby, 0x330000, 0);
                    if (hit != 0 && (*(struct Moby **)(hit + 0x20))->oclass != 0xD4) {
                        hit += 0x10;
                        size = moby->scale;
                        size /= moby->pclass->scale;
                        clamp_vector_length(hit, hit, frame_time * 20.0f);
                        fast_vec_add(vars, vars, hit);
                        FUN_L13_002e1348_1638(moby, vars, size);
                    }
                    moby->hit_slot = 0xFF;
                }
            }
        } else if (fast_dec_timer(&vars->timer) != 0) {
            if (vars->child == 1) {
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
            moby->visible = 0;
            moby->scale = vars->start_scale;
        }
        break;
    case ASTEROID_RESET_AT_HOME:
        qcopy(&moby->pos, &vars->home);
        qzero(&vars->spin);
        spin = random_float_between(-30.0f, 30.0f) * DEG_TO_RAD * frame_time;
        switch (rand_1638() % 3) {
        case SPIN_AXIS_X:
            vars->spin.x = spin;
            break;
        case SPIN_AXIS_Y:
            vars->spin.y = spin;
            break;
        case SPIN_AXIS_Z:
            vars->spin.z = spin;
            break;
        }
        spin = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD * frame_time;
        switch (rand_1638() % 3) {
        case SPIN_AXIS_X:
            vars->spin.x = spin;
            break;
        case SPIN_AXIS_Y:
            vars->spin.y = spin;
            break;
        case SPIN_AXIS_Z:
            vars->spin.z = spin;
            break;
        }
        moby->rot.x = random_angle_radians();
        moby->rot.y = random_angle_radians();
        moby->rot.z = random_angle_radians();
        qzero(&vars->vel);
        moby->scale = vars->start_scale;
        if (moby->pclass->scale < moby->scale) {
            moby->unk72 = 100;
        }
        moby->state = 1;
        break;
    case ASTEROID_WAIT_TO_RESPAWN:
        if (fast_bsphere_check(&vars->home, moby->draw_dist) < 0) {
            qcopy(&moby->pos, &vars->home);
            moby->collision = moby->pclass->collision;
            moby->visible = 1;
            moby->flags &= ~1;
            moby->state = 3;
        }
        break;
    }
    if (moby->pos.x < 20.0f) {
        moby->pos.x = 20.0f;
    }
    if (moby->pos.x > 1003.0f) {
        moby->pos.x = 1003.0f;
    }
    if (moby->pos.y < 20.0f) {
        moby->pos.y = 20.0f;
    }
    if (moby->pos.y > 1003.0f) {
        moby->pos.y = 1003.0f;
    }
    if (moby->pos.z < 20.0f) {
        moby->pos.z = 20.0f;
    }
    if (moby->pos.z > 1003.0f) {
        moby->pos.z = 1003.0f;
    }
    moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin.x);
    moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin.y);
    moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin.z);
}

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
