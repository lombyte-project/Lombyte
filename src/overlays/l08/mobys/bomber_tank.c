/* Level 08 moby class 0xfd: bomber tank. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/game/fun_00213928.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/math/random.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/l08/mobys/bomber_tank_bomb.h"
#include "rnc/overlay/l08/mobys/bomber_tank_globals.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

/* Level 8 moby update (class 152): state machine that steers the moby toward the hero and fires its event. */
typedef struct Moby Moby;
extern float        D_0015ED70;
extern int          D_0015ED84;
extern char        *D_L08_0015FFD8;

#include "rnc/overlay/level_path.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern char            *create_moby(int);
extern void             update_entity(void *) __asm__("FUN_L00_00250df8");
extern float D_0015ED6C MACRO_ADDR;
extern void             trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void             trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void             spawn_explosion(void *,
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
extern float            D_L08_0015F580_macro[] __asm__("D_L08_0015F580") MACRO_ADDR;

/* Bomber tank (class 253) update: spawns its two parts, then a state machine for driving, firing and the group's debris. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D55E0), where it is exact; names translated to the US level program. */
extern char        D_0013D388[] NOT_SDA;
extern s32         D_001413D0[];
extern const float D_L08_001618CC __attribute__((sda));
extern float       bomber_tank_projectile_speed_gp __asm__("D_L08_001618A8") __attribute__((sda));
extern float       D_L08_001618D0 __attribute__((sda));
extern unsigned char D_0013D3B6 NOT_SDA;
extern unsigned char D_0013D4CB NOT_SDA;
extern void                     FUN_L00_00260738(void *, void *, void *, void *);
void                            spawn_bomber_tank_joint_particles(struct Moby *moby,
                                                                  int          joint,
                                                                  int          count) __asm__("FUN_L08_002d5950");
s32         is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
extern void attach_manipulator(void *, s32, void *) __asm__("FUN_0020cb10");
extern unsigned short D_00141C38 NOT_SDA;
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
void        update_bomber_tank_damage_and_aim(Moby *moby) __asm__("FUN_L08_002d4ca0");
void advance_bomber_tank_along_path(struct Moby *moby, int path_id) __asm__("FUN_L08_002d55c8");
void scatter_bomber_tank_linked_debris(struct Moby *moby) __asm__("FUN_L08_002d5d08");
void remove_bomber_tank_linked_debris(struct Moby *moby) __asm__("FUN_L08_002d6450");

typedef struct {
    u8    pad0[0x20];
    f32   health; /* 0x20 */
    u8    pad24[4];
    u8    unk28;
    u8    unk29;
    u8    unk2A;
    u8    pad2B[3];
    u8    unk2E; /* 0x2E: 2 removes the tank and its parts */
    u8    pad2F;
    f32   unk30;
    u8    pad34[0xA];
    u16   flags; /* 0x3E */
    u64   unk40;
    u8    pad48[0x10];
    u8    unk58;
    u8    pad59;
    u8    unk5A;
    u8    pad5B[5];
    u8    unk60[0x90];
    Moby *target; /* 0xF0 */
    s32   mode;   /* 0xF4 */
    u8    padF8[8];
    u8    manipulator[0x40]; /* 0x100 */
    s32 trigger_moby; /* 0x140: index of the moby whose pvars float reaching 1.0 releases the tank */
    s32   unk144;
    u8    pad148[4];
    s32   start_point; /* 0x14C: D_L08_001B0CB0 index */
    s32   path;        /* 0x150: D_L08_001B0CB0 index */
    u8    pad154[0xC];
    Moby *part_a; /* 0x160 */
    Moby *part_b; /* 0x164 */
    u8    pad168[4];
    f32   unk16C;
    f32   yaw; /* 0x170: added to rot.z for shots */
    u8    pad174[4];
    f32   pitch; /* 0x178 */
} BomberTankVars;

void update_bomber_tank(Moby *moby) __asm__("FUN_L08_002d42d8");

void update_bomber_tank(Moby *moby) {
    enum {
        BOMBER_TANK_STATE_INITIALIZE = 0,
        BOMBER_TANK_STATE_FOLLOW_PATH_AND_FIRE = 3,
        BOMBER_TANK_STATE_FOLLOW_PATH = 6,
        BOMBER_TANK_STATE_WAIT_FOR_TRIGGER = 11,
        BOMBER_TANK_STATE_SCATTER_GROUP_AND_ACTIVATE = 14,
        BOMBER_TANK_STATE_DAMAGED_FOLLOW_PATH = 99
    };
    float           old_pos[4];
    float           old_rot[4];
    float           velocity[4];
    float           muzzle[4];
    BomberTankVars *vars = (BomberTankVars *)moby->pvars;
    Moby           *target;
    /* Fakematch: hidden zero for the case 12 test below. */
    int never = 0;

    qcopy(old_pos, &moby->pos);
    qcopy(old_rot, &moby->rot);
    if (vars->unk2E == 2) {
        if (vars->part_a != 0)
            mark_moby_for_removal(vars->part_a);
        if (vars->part_b != 0)
            mark_moby_for_removal(vars->part_b);
        mark_moby_for_removal(moby);
        return;
    }
    update_bomber_tank_damage_and_aim(moby);
    target = vars->target;
    switch (moby->state) {
    case BOMBER_TANK_STATE_INITIALIZE:
        if (vars->part_a == 0) {
            Moby *part;
            vars->part_a = create_moby(0x14B);
            vars->part_a->update_dist = 0x40;
            vars->part_a->draw_dist = 0x40;
            vars->part_a->visible = 1;
            vars->part_a->spawn_frame = (*(Moby **)(D_001413D0))->spawn_frame;
            vars->part_a->flags = 0x4000;
            part = vars->part_a;
            qcopy(&part->pos, &moby->pos);
            part->collision = 0;
            blend_moby_animation(vars->part_a, 0, 0, 0);
            update_entity(vars->part_a);
        }
        if (vars->part_b == 0) {
            Moby *part;
            vars->part_b = create_moby(0x14B);
            vars->part_b->update_dist = 0x40;
            vars->part_b->draw_dist = 0x40;
            vars->part_b->visible = 1;
            vars->part_b->spawn_frame = (*(Moby **)(D_001413D0))->spawn_frame;
            vars->part_b->flags = 0xC000;
            part = vars->part_b;
            qcopy(&part->pos, &moby->pos);
            part->collision = 0;
            blend_moby_animation(vars->part_b, 0, 0, 0);
            update_entity(vars->part_b);
        }
        attach_manipulator(moby, 0, vars->manipulator);
        moby->flags |= 0x1000;
        vars->unk16C = frame_time + frame_time;
        if (vars->trigger_moby > 0) {
            moby->state = 11;
            moby->flags = (moby->flags & 0xEFFF) | 0x41;
            vars->part_a->flags |= 0x41;
            vars->part_b->flags |= 0x41;
            moby->collision = 0;
            vars->part_a->collision = 0;
            vars->part_b->collision = 0;
            if (moby->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, 0);
        } else if (vars->unk144 > 0) {
            if (D_0013D3B6 == 0) {
                moby->state = 14;
                moby->flags |= 0x41;
                vars->part_a->flags |= 0x41;
                vars->part_b->flags |= 0x41;
                moby->collision = 0;
                vars->part_a->collision = 0;
                vars->part_b->collision = 0;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, 0);
            } else {
                remove_bomber_tank_linked_debris(moby);
                qcopy(&moby->pos, (char *)l08_paths[vars->path] + 0x10);
                moby->state = 6;
                if (moby->prev_seq != 1)
                    blend_moby_animation(moby, 1, 0, 0);
            }
        } else {
            qcopy(&moby->pos, (char *)l08_paths[vars->path] + 0x10);
            moby->state = 6;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, 0);
        }
        clear_u64_value(&vars->unk40);
        vars->unk29 = 0;
        vars->unk28 = 3;
        vars->unk30 = 3.0f;
        vars->health = 9.0f;
        vars->unk2A = 12;
        vars->unk2E = 1;
        vars->flags |= 2;
        clear_u64_value(&vars->unk40);
        vars->pitch = 0.0f;
        vars->yaw = moby->rot.z;
        vars->unk58 = truncate_float_to_s32(D_L08_001618CC);
        vars->unk5A = truncate_float_to_s32(D_L08_001618D0 * 8.0f);
        break;
    case BOMBER_TANK_STATE_FOLLOW_PATH:
        advance_bomber_tank_along_path(moby, vars->path);
        if (vars->mode != 2 && vector_distance_xy(&moby->pos, &target->pos) < 20.0f) {
            if (D_00141C38 == 0 && D_0013D4CB == 0) {
                help_request(0x1F44, 0x5A);
            }
            moby->state = 3;
            if (moby->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        }
        break;
    case BOMBER_TANK_STATE_FOLLOW_PATH_AND_FIRE:
        advance_bomber_tank_along_path(moby, vars->path);
        if (vars->mode == 2 || vector_distance_xy(&moby->pos, &target->pos) > 22.0f) {
            moby->state = 6;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        if (is_value_within_interpolated_window((char *)moby, 4.0f) != 0 ||
            is_value_within_interpolated_window((char *)moby, 10.0f) != 0) {
            float shot_speed;
            char *shot;
            if (compute_interpolated_record_value(moby) == 4.0f) {
                moby_point_position(moby, 1, muzzle);
            } else {
                moby_point_position(moby, 2, muzzle);
            }
            shot_speed = bomber_tank_projectile_speed_gp * frame_time;
            velocity[0] = fast_cos(fast_add_rotations(vars->yaw, moby->rot.z)) *
                          (fast_cos(vars->pitch) * shot_speed);
            velocity[1] = fast_sin(fast_add_rotations(vars->yaw, moby->rot.z)) *
                          (fast_cos(vars->pitch) * shot_speed);
            velocity[2] = 0.0f;
            velocity[2] = -fast_sin(vars->pitch) * shot_speed;
            shot = spawn_bomber_tank_bomb((char *)moby, (char *)muzzle, (char *)velocity);
            if (shot != 0)
                shot[0x20] = 1;
        }
        break;
    case BOMBER_TANK_STATE_WAIT_FOR_TRIGGER:
        if (*(float *)((Moby *)D_L08_0015FFD8 + vars->trigger_moby)->pvars == 1.0f) {
            qcopy(&moby->pos, (char *)l08_paths[vars->start_point] + 0x10);
            qcopy(&vars->part_a->pos, &moby->pos);
            qcopy(&vars->part_b->pos, &moby->pos);
            moby->flags &= 0xFFBE;
            vars->part_a->flags &= 0xFFBE;
            vars->part_b->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            vars->part_a->collision = vars->part_a->pclass->collision;
            vars->part_b->collision = vars->part_b->pclass->collision;
            moby->flags |= 0x1000;
            if (vars->mode != 2) {
                moby->state = 6;
                if (moby->prev_seq != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case BOMBER_TANK_STATE_SCATTER_GROUP_AND_ACTIVATE:
        if (vars->mode != 2) {
            unsigned char *event_flags;
            scatter_bomber_tank_linked_debris((char *)moby);
            event_flags = (unsigned char *)D_0013D388;
            if (event_flags[0x2E] != 0)
                allocate_voice_for_target_entry(6, 0, moby);
            event_flags[0x2E] = 1;
            qcopy(&vars->part_a->pos, &moby->pos);
            qcopy(&vars->part_b->pos, &moby->pos);
            moby->flags &= 0xFFBE;
            vars->part_a->flags &= 0xFFBE;
            vars->part_b->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            vars->part_a->collision = vars->part_a->pclass->collision;
            vars->part_b->collision = vars->part_b->pclass->collision;
            moby->flags |= 0x1000;
            if (vars->mode != 2) {
                moby->state = 6;
                if (moby->prev_seq != 1)
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case 12:
        /* Fakematch: a never-true test keeps case 12 a block of its own until gcse, which then puts the default path's copies where retail has them. */
        if (never)
            vars->unk2E = 0;
        break;
    case BOMBER_TANK_STATE_DAMAGED_FOLLOW_PATH: {
        float frame;
        advance_bomber_tank_along_path(moby, vars->path);
        frame = compute_interpolated_record_value(moby);
        if ((59.0f <= frame && frame <= 60.0f) || (119.0f <= frame && frame <= 120.0f)) {
            spawn_bomber_tank_joint_particles((char *)moby, 4, 10);
        } else if ((29.0f <= frame && frame <= 30.0f) || (89.0f <= frame && frame <= 90.0f)) {
            spawn_bomber_tank_joint_particles((char *)moby, 5, 10);
        }
        break;
    }
    }
    fast_vec_sub(velocity, &moby->pos, old_pos);
    FUN_L00_00260738(vars->unk60, velocity, &moby->rot, old_rot);
}

/* Per-frame update of a batalia turret moby: damage, its death state and its two helper parts. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D5FA8), where it is exact; names translated to the US level program. */
typedef struct {
    char           pad0[0x20];
    float          health;
    char           pad24[2];
    short          timer;
    char           pad28[0x16];
    unsigned short flags;
    float          f40[4];
    char           pad50[0x50];
    short          anim0;
    short          anim2;
    char           padA4[3];
    unsigned char  tint;
    char           padA8[8];
    float          target[4];
    char           padC0[0x30];
    int            fF0;
    int            mode;
    char           padF8[0x18];
    float          f110[4];
    float          f120[4];
    char           pad130[0x24];
    int            path;
    char           pad158[8];
    Moby          *partA;
    Moby          *partB;
    char           pad168[8];
    float          yaw;
    float          yaw_speed;
    float          pitch;
    float          pitch_speed;
    char           pad180[4];
    int            part_id;
} Data;

typedef struct {
    char  pad0[0x20];
    Moby *moby;
} Hit_2D5FA8;

typedef struct {
    int  count;
    char pad4[0xC];
    char list[1];
} Path;

void *
select_candidate_by_max_float_2c(void *a, void *b, void *c, void *d) __asm__("FUN_L05_002899f8");
extern float D_0015ED6C;
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern int   D_L08_0015F5B0 __asm__("D_L08_0015F5B0");
extern int   D_L08_0015F5B4_gp __asm__("D_L08_0015F5B4") __attribute__((sda));
extern int   D_L08_0015F5BC_gp __asm__("D_L08_0015F5BC") __attribute__((sda));
extern void *
spawn_2D5FA8(Moby *, int, float *, float *, int, int, float, float *, float *, float *) __asm__("FUN_L00_00263fd8");
extern void mark_moby_for_removal_cf(Moby *, int, int) __asm__("FUN_L00_00257470");
extern int
activate_effect_l08(float, Moby *, float *, int, int, char *, int) __asm__("FUN_L00_0025ff38");
extern float
approach_2D5FA8(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern int bomber_tank_part_3_destroyed_by_class_661_gp __asm__("D_L08_0015F5B8")
    __attribute__((sda));

void update_bomber_tank_damage_and_aim(Moby *moby) {
    Data       *data = (Data *)moby->pvars;
    Hit_2D5FA8 *source;
    Hit_2D5FA8 *part_a_source;
    Hit_2D5FA8 *part_b_source;
    float       burst_position_or_aim_scratch[4];
    int         hit;
    float       damage = 0.0f;
    int         one;

    source = find_moby_hit(moby, 0x330000, 0);
    if (data->partA != 0) {
        part_a_source = find_moby_hit(data->partA, 0x210000, 0);
    } else {
        part_a_source = 0;
    }
    if (data->partB != 0) {
        part_b_source = find_moby_hit(data->partB, 0x210000, 0);
    } else {
        part_b_source = 0;
    }
    source = select_candidate_by_max_float_2c(source, part_a_source, part_b_source, 0);
    if (source != 0 && source->moby != 0) {
        short source_class = source->moby->oclass;
        if (source_class == 0x1A9 || source_class == 0x47)
            source = 0;
    }
    fun_00213928(moby, source, &data->health, 0, &hit, &damage, 0, 4);
    one = 1;
    if (hit != one && moby->state != 0) {
        data->health -= damage;
        if (damage != 0.0f)
            allocate_voice_for_target_entry(4, 0, moby);
        if (data->health > 0.0f) {
            if (damage < 4.0f) {
                data->tint = 0x78;
                if ((unsigned short)(source->moby->oclass - 0xB0) < 2) {
                    data->timer = scale_game_frames(0x3C);
                } else {
                    data->timer = scale_game_frames(0x1E);
                }
            } else if (damage < 6.0f) {
                data->tint = 0xC8;
                data->timer = scale_game_frames(0x1E);
            } else {
                data->tint = 0xE6;
                data->timer = scale_game_frames(0x1E);
            }
            if (moby->state == 0xC && moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, 0);
            }
        } else {
            int dead = 0x63;
            if (moby->state != dead) {
                qcopy(burst_position_or_aim_scratch, &moby->pos);
                burst_position_or_aim_scratch[2] += 3.0f;
                data->tint = 0xFA;
                clear_u64_value(data->f120);
                spawn_explosion(moby,
                                data->f40,
                                burst_position_or_aim_scratch,
                                0.0f,
                                0.0f,
                                20,
                                8,
                                20,
                                4.0f,
                                2.0f,
                                9.0f,
                                1.0f,
                                -1,
                                15.0f,
                                1,
                                one,
                                -1,
                                0);
                spawn_2D5FA8(moby,
                             0x6C0,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6C1,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6C2,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6C3,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6C4,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                if (moby->prev_seq != one)
                    blend_moby_animation(moby, 1, 0, 3);
                moby->state = dead;
                data->health = 6.0f;
                data->flags &= 0xFFFD;
            } else {
                if (D_0015ED84 == 8) {
                    int part = data->part_id;
                    if (part != 0 && source != 0 && source->moby != 0 &&
                        source->moby->oclass == 0x661) {
                        if (part == one)
                            D_L08_0015F5B0 = one;
                        if (part == 2)
                            D_L08_0015F5B4_gp = one;
                        if (part == 3)
                            bomber_tank_part_3_destroyed_by_class_661_gp = one;
                        if (part == 4)
                            D_L08_0015F5BC_gp = one;
                        if (D_L08_0015F5B0 != 0 && D_L08_0015F5B4_gp != 0) {
                            unsigned char *flags = item_unlocked;
                            if (flags[0xD] == 0) {
                                flags[0xD] = one;
                                allocate_voice_for_bank_entry(1, 0, 0);
                                queue_dialogue_message(0x53D6, -1);
                            }
                        }
                    }
                }
                qcopy(burst_position_or_aim_scratch, &moby->pos);
                burst_position_or_aim_scratch[2] += 1.0f;
                mark_moby_for_removal_cf(moby, 0, -1);
                allocate_voice_for_target_entry(2, 0, moby);
                spawn_explosion(moby,
                                data->f40,
                                burst_position_or_aim_scratch,
                                0.0f,
                                0.0f,
                                20,
                                8,
                                20,
                                4.0f,
                                2.0f,
                                9.0f,
                                1.0f,
                                -1,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                spawn_2D5FA8(moby,
                             0x6FE,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6FE,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6FF,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6FF,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                spawn_2D5FA8(moby,
                             0x6FF,
                             (float *)&moby->pos,
                             (float *)&moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro,
                             D_L08_0015F580_macro);
                if (data->partA != 0)
                    mark_moby_for_removal(data->partA);
                if (data->partB != 0)
                    mark_moby_for_removal(data->partB);
                mark_moby_for_removal(moby);
                return;
            }
        }
        trooper_anim_update(moby, &data->anim0);
    }

    moby->hit_slot = 0xFF;
    if (data->partA != 0)
        data->partA->hit_slot = 0xFF;
    if (data->partB != 0)
        data->partB->hit_slot = 0xFF;

    activate_effect_l08(100.0f,
                        moby,
                        data->target,
                        0,
                        0,
                        ((Path *)l08_paths[data->path])->list,
                        ((Path *)l08_paths[data->path])->count);
    if (data->fF0 == 0)
        data->fF0 = *(int *)D_001413D0;
    if (data->mode != 2) {
        float yaw, distance, pitch;
        fast_vec_sub(burst_position_or_aim_scratch, data->target, (float *)&moby->pos);
        burst_position_or_aim_scratch[2] -= 3.0f;
        yaw = fast_arc_tan(burst_position_or_aim_scratch[0], burst_position_or_aim_scratch[1]);
        distance = vector_length_xy(burst_position_or_aim_scratch);
        pitch = -fast_arc_tan(distance, burst_position_or_aim_scratch[2]);
        if (0.34906585f < pitch) {
            pitch = 0.34906585f;
        } else if (pitch < -0.7853982f) {
            pitch = -0.7853982f;
        }
        yaw = fast_subtract_rotations(yaw, moby->rot.z);
        approach_2D5FA8(&data->yaw,
                        yaw,
                        &data->yaw_speed,
                        D_0015ED70 * 12.566371f,
                        D_0015ED70 * 12.566371f,
                        D_0015ED6C_lui * 9.424778f);
        approach_2D5FA8(&data->pitch,
                        pitch,
                        &data->pitch_speed,
                        D_0015ED70 * 12.566371f,
                        D_0015ED70 * 12.566371f,
                        D_0015ED6C_lui * 9.424778f);
    } else {
        approach_2D5FA8(&data->yaw,
                        0.0f,
                        &data->yaw_speed,
                        D_0015ED70 * 12.566371f,
                        D_0015ED70 * 12.566371f,
                        D_0015ED6C_lui * 9.424778f);
        approach_2D5FA8(&data->pitch,
                        0.0f,
                        &data->pitch_speed,
                        D_0015ED70 * 12.566371f,
                        D_0015ED70 * 12.566371f,
                        D_0015ED6C_lui * 9.424778f);
    }
    rotation_2D5FA8(burst_position_or_aim_scratch, 1, data->pitch);
    rotation_2D5FA8(data->f110, 2, data->yaw);
    multiply_quaternions(data->f110, burst_position_or_aim_scratch, data->f110);
    {
        short saved_anim0 = data->anim0;
        short saved_anim2 = data->anim2;
        trooper_anim_finish(moby, &data->anim0);
        if (data->partA != 0) {
            data->anim0 = saved_anim0;
            data->anim2 = saved_anim2;
            trooper_anim_finish(data->partA, &data->anim0);
        }
        if (data->partB != 0) {
            data->anim0 = saved_anim0;
            data->anim2 = saved_anim2;
            trooper_anim_finish(data->partB, &data->anim0);
        }
    }
}

extern float bomber_tank_part_reference_speed_gp __asm__("D_L08_001618A0") __attribute__((sda));
extern void  approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void  vendor_volume(int, int) __asm__("FUN_L01_002a1968");

typedef struct {
    u8  pad0[0x80];
    int volume;
} SoundSlot;

#define SOUND_SLOT_2d55c8(id) ((SoundSlot *)(((char *)&voice_pool) + (id) * 0x70))

typedef struct {
    u8    pad0[0x40];
    float velocity[4];
    u8    pad50[0x108];
    float turn_speed;
    int   sound;
    void *linked[2];
    int   point;
    float target_speed;
    u8    pad170[0x10];
    float speed;
} PathMoverVars;

void advance_bomber_tank_along_path(struct Moby *moby, int path_id) {
    PathMoverVars *vars = (PathMoverVars *)moby->pvars;
    int           *path = (int *)l08_paths[path_id];
    float          point[4];
    float          step[3];
    float          scale;
    float          angle;
    float          speed;
    float         *heading;

    qcopy(point, path + 4 + vars->point * 4);
    heading = &moby->rot.z;
    angle = fast_arc_tan(point[0] - moby->pos.x, point[1] - moby->pos.y);
    approach_angle(heading,
                   &vars->turn_speed,
                   angle,
                   frame_time_sq * 4.712389f,
                   frame_time_sq * 4.712389f,
                   frame_time * 3.1415927f);
    approach_value(&vars->speed, vars->target_speed, frame_time_sq + frame_time_sq);
    step[0] = fast_cos(moby->rot.z) * vars->speed;
    step[1] = fast_sin(moby->rot.z) * vars->speed;
    step[2] = 0.0f;
    fast_vec_add(&moby->pos, &moby->pos, step);
    moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
    if (vector_distance_xy(&moby->pos, point) < 0.5f) {
        vars->point = (vars->point + 1) % *path;
    }
    scale = vars->speed / (bomber_tank_part_reference_speed_gp * frame_time);
    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.0f) {
        scale = 0.0f;
    }
    if (vars->linked[0] != 0) {
        char *linked;

        *(float *)((char *)vars->linked[0] + 0x58) = scale;
        linked = vars->linked[0];
        qcopy(linked + 0x40, (char *)moby + 0x40);
        qcopy(linked + 0x10, &moby->pos);
    }
    if (vars->linked[1] != 0) {
        char *linked;

        *(float *)((char *)vars->linked[1] + 0x58) = scale;
        linked = vars->linked[1];
        qcopy(linked + 0x40, (char *)moby + 0x40);
        qcopy(linked + 0x10, &moby->pos);
    }
    speed = fast_vec_length(vars->velocity);
    if (frame_time * 0.1f < speed) {
        if (voice_is_playing(moby, vars->sound) == 0) {
            vars->sound = allocate_voice_for_target_entry(1, 4, moby);
            vendor_volume(vars->sound, 1);
        } else if (SOUND_SLOT_2d55c8(vars->sound)->volume < 0x400) {
            vendor_volume(vars->sound,
                          SOUND_SLOT_2d55c8(vars->sound)->volume + 0x400 / scale_game_frames(60));
        }
    } else if (voice_is_playing(moby, vars->sound) != 0) {
        if (SOUND_SLOT_2d55c8(vars->sound)->volume < 0x100) {
            release_voice_slot(vars->sound);
            vars->sound = -1;
        } else {
            vendor_volume(vars->sound,
                          SOUND_SLOT_2d55c8(vars->sound)->volume - 0x400 / scale_game_frames(60));
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D6C58), where it is exact; names translated to the US level program. */
extern float scale_time(float) __asm__("FUN_001f96b0");

#include "rnc/overlay/l08/mobys/bomber_tank_data/spawn_bomber_tank_joint_particles_2.h"

extern float bomber_tank_joint_particle_jitter_magnitude_base_gp __asm__("D_L08_001618DC")
    __attribute__((sda));
extern float bomber_tank_joint_particle_vertical_spread_gp __asm__("D_L08_001618E0")
    __attribute__((sda));
extern float bomber_tank_joint_particle_horizontal_spread_range_end_gp __asm__("D_L08_001618E8")
    __attribute__((sda));

#include "rnc/overlay/l08/mobys/bomber_tank_data/spawn_bomber_tank_joint_particles.h"

extern void  build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void  func_L00_001FF4B0_2D6C58(void *, void *, float) __asm__("FUN_001f9bf8");
extern float func_001F9F90_2D6C58(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2D6C58(float) __asm__("FUN_001f9de0");

void spawn_bomber_tank_joint_particles(struct Moby *moby, int joint, int count) {
    char *data = ((char *)moby->pvars);
    char  joint_transform_buffer[32];
    float jitter_direction_source[8];
    float joint_pos[4];
    float velocity[4];
    float spread_vector[4];
    float jitter[4];
    float spawn_pos[4];
    int   i;
    int   frames1, frames2, frames3;
    float random_angle, offset;

    moby_point_position(moby, joint, joint_pos);
    build_moby_bone_transform(moby, joint, joint_transform_buffer);
    for (i = 0; i < count; i++) {
        float *spawn_ptr = spawn_pos;

        random_angle = random_angle_radians();
        qcopy(spawn_ptr, joint_pos);
        spawn_pos[0] += random_float_between(-0.05f, 0.05f);
        spawn_pos[1] += random_float_between(-0.05f, 0.05f);
        spawn_pos[2] += random_float_between(-0.05f, 0.05f);
        qcopy(velocity, data + 0x40);
        qcopy(spread_vector, data + 0x40);
        if (count / 3 < i) {
            func_L00_001FF4B0_2D6C58(
                jitter,
                jitter_direction_source,
                random_float_between(bomber_tank_joint_particle_jitter_magnitude_base_gp,
                                     bomber_tank_joint_particle_jitter_magnitude_base_gp * 1.2f) *
                    D_0015ED6C);
            fast_vec_add(velocity, velocity, jitter);
            fast_vec_scale(spread_vector, spread_vector, 0.5f);
            offset = func_001F9F90_2D6C58(random_angle);
            offset =
                offset * (random_float_between(
                              0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp) *
                          D_0015ED6C);
            spread_vector[0] = spread_vector[0] + offset;
            offset = func_001F9FA8_2D6C58(random_angle);
            offset =
                offset * (random_float_between(
                              0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp) *
                          D_0015ED6C);
            spread_vector[1] = spread_vector[1] + offset;
            spread_vector[2] = bomber_tank_joint_particle_vertical_spread_gp * D_0015ED6C;
        } else {
            func_L00_001FF4B0_2D6C58(
                jitter,
                jitter_direction_source,
                random_float_between(0.0f, bomber_tank_joint_particle_jitter_magnitude_base_gp) *
                    D_0015ED6C);
            fast_vec_add(velocity, velocity, jitter);
            fast_vec_scale(spread_vector, spread_vector, 0.5f);
            offset = func_001F9F90_2D6C58(random_angle);
            offset = offset *
                     (random_float_between(
                          0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp * 0.25f) *
                      D_0015ED6C);
            spread_vector[0] = spread_vector[0] + offset;
            offset = func_001F9FA8_2D6C58(random_angle);
            offset = offset *
                     (random_float_between(
                          0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp * 0.25f) *
                      D_0015ED6C);
            spread_vector[1] = spread_vector[1] + offset;
            spread_vector[2] = bomber_tank_joint_particle_vertical_spread_gp * D_0015ED6C;
        }
        velocity[3] = bomber_tank_joint_particle_velocity_w_gp;
        spread_vector[3] = bomber_tank_joint_particle_spread_vector_w_gp;
        frames1 = truncate_float_to_s32(scale_time(random_float_between(
            (float)bomber_tank_joint_particle_duration_frames_range_start_gp,
            (float)bomber_tank_joint_particle_duration_frames_range_start_gp * 1.2f)));
        frames2 = truncate_float_to_s32(scale_time(random_float_between(
            (float)bomber_tank_joint_particle_second_frame_count_range_start_gp,
            (float)bomber_tank_joint_particle_second_frame_count_range_start_gp * 1.2f)));
        frames3 = truncate_float_to_s32(scale_time(random_float_between(
            (float)bomber_tank_joint_particle_third_frame_count_range_start_gp,
            (float)bomber_tank_joint_particle_third_frame_count_range_start_gp * 1.5f)));
        spawn_spark(joint_pos,
                    velocity,
                    spread_vector,
                    bomber_tank_joint_particle_initial_color_gp,
                    bomber_tank_joint_particle_second_color_gp,
                    frames1,
                    frames2,
                    frames3,
                    -1);
    }
}

/* Spawns debris mobys while the level counter allows, then scatters or bursts the group's other mobys. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D7010), where it is exact; names translated to the US level program. */
typedef int u128_7010 __attribute__((mode(TI)));

typedef struct {
    char          pad0[0x21];
    unsigned char group;
    char          pad22[0xDE];
} Blk_7010;

extern char  *D_L08_001600EC;
extern short *D_L08_001AC040[];
extern void
initialize_moby_launch_motion(void *, int, void *, void *, int) __asm__("FUN_L05_00318648");

void scatter_bomber_tank_linked_debris(struct Moby *moby) {
    enum {
        LINKED_CLASS_SECOND_HIGHEST_Y_LAUNCH_OFFSET = 0,
        LINKED_CLASS_FOURTH_LOWEST_X_LAUNCH_OFFSET = 1,
        LINKED_CLASS_SECOND_LOWEST_X_LAUNCH_OFFSET = 2,
        LINKED_CLASS_LOWEST_X_LAUNCH_OFFSET = 3,
        LINKED_CLASS_THIRD_HIGHEST_X_LAUNCH_OFFSET = 4,
        LINKED_CLASS_LOWEST_Z_LAUNCH_OFFSET = 5,
        LINKED_CLASS_HIGHEST_X_LAUNCH_OFFSET = 6,
        LINKED_CLASS_LOWEST_Y_LAUNCH_OFFSET = 7,
        LINKED_CLASS_HIGHEST_Z_LAUNCH_OFFSET = 8,
        LINKED_CLASS_FOURTH_HIGHEST_Y_LAUNCH_OFFSET = 9,
        LINKED_CLASS_THIRD_LOWEST_X_LAUNCH_OFFSET = 10
    };
    float  position_or_velocity_scratch[4];
    float  launch_offset[4];
    float  launch_velocity_or_offset[4];
    char  *moby_vars = ((char *)moby->pvars);
    short *list = D_L08_001AC040[((Blk_7010 *)D_L08_0015FFD8 + *(int *)(moby_vars + 0x144))->group];
    char  *other;
    unsigned short index;
    int            i;

    if (list == 0)
        return;
    index = *list & 0x7FFF;
    other = (char *)((Blk_7010 *)D_L08_0015FFD8 + index);
    for (i = 0; i < 40 && available_moby_slot_count >= 50; i++) {
        char *spawned_debris_moby;
        if (random_integer_below(0xFF) & 1)
            spawned_debris_moby = create_moby(0x310);
        else
            spawned_debris_moby = create_moby(0x311);
        if (spawned_debris_moby != 0) {
            float ang;
            MOBY(spawned_debris_moby)->draw_dist = 0x40;
            MOBY(spawned_debris_moby)->update_dist = 0xFF;
            spawned_debris_moby[0x31] = 1;
            MOBY(spawned_debris_moby)->rot.x = random_angle_radians();
            MOBY(spawned_debris_moby)->rot.y = random_angle_radians();
            MOBY(spawned_debris_moby)->rot.z = random_angle_radians();
            position_or_velocity_scratch[0] = random_float_between(-1.0f, 1.0f);
            position_or_velocity_scratch[1] = random_float_between(-1.0f, 1.0f);
            position_or_velocity_scratch[2] = random_float_between(-1.0f, 1.0f);
            position_or_velocity_scratch[3] = 1.0f;
            transform_vector(position_or_velocity_scratch,
                             position_or_velocity_scratch,
                             D_L08_001600EC + (*(int *)(moby_vars + 0x148) << 7));
            qcopy(spawned_debris_moby + 0x10, position_or_velocity_scratch);
            ang = random_float_between(-0.2617994f, 0.2617994f);
            clear_u64_value(launch_offset);
            launch_velocity_or_offset[0] = fast_cos(ang) * (D_0015ED6C_lui * 15.0f);
            launch_velocity_or_offset[1] = fast_sin(ang) * (D_0015ED6C_lui * 15.0f);
            launch_velocity_or_offset[2] = 0.0f;
            launch_velocity_or_offset[2] =
                random_float_between(D_0015ED6C_lui * 8.0f, D_0015ED6C_lui * 12.0f);
            initialize_moby_launch_motion(
                spawned_debris_moby,
                (int)moby,
                launch_velocity_or_offset,
                launch_offset,
                truncate_float_to_s32(random_float_between(60.0f, 90.0f)));
        }
    }
    while (1) {
        if (*(unsigned char *)(other + 0x20) < 0x7F && other != moby) {
            unsigned short cls = *(unsigned short *)(other + 0xA6);
            if ((unsigned short)(cls - 0x24C) < 0xB) {
                float ang;
                float lo, hi;
                ang = random_float_between(-0.2617994f, 0.2617994f);
                position_or_velocity_scratch[0] = fast_cos(ang) * (D_0015ED6C_lui * 15.0f);
                position_or_velocity_scratch[1] = fast_sin(ang) * (D_0015ED6C_lui * 15.0f);
                position_or_velocity_scratch[2] = 0.0f;
                position_or_velocity_scratch[2] =
                    random_float_between(D_0015ED6C_lui * 8.0f, D_0015ED6C_lui * 12.0f);
                switch ((short)(*(unsigned short *)(other + 0xA6) - 0x24C)) {
                case LINKED_CLASS_SECOND_HIGHEST_Y_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 1.629f;
                    launch_velocity_or_offset[1] = -1.341f;
                    launch_velocity_or_offset[2] = 11.48f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_FOURTH_LOWEST_X_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 1.299f;
                    launch_velocity_or_offset[1] = -5.558f;
                    launch_velocity_or_offset[2] = 11.48f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_SECOND_LOWEST_X_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 0.629f;
                    launch_velocity_or_offset[1] = -2.527f;
                    launch_velocity_or_offset[2] = 3.409f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_LOWEST_X_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 0.019f;
                    launch_velocity_or_offset[1] = -6.19f;
                    launch_velocity_or_offset[2] = 3.409f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_THIRD_HIGHEST_X_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 3.196f;
                    launch_velocity_or_offset[1] = -2.105f;
                    launch_velocity_or_offset[2] = 7.784f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_LOWEST_Z_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 2.11f;
                    launch_velocity_or_offset[1] = -3.013f;
                    launch_velocity_or_offset[2] = 2.625f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_HIGHEST_X_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 4.33f;
                    launch_velocity_or_offset[1] = -6.449f;
                    launch_velocity_or_offset[2] = 7.397f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_LOWEST_Y_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 3.286f;
                    launch_velocity_or_offset[1] = -6.925f;
                    launch_velocity_or_offset[2] = 2.96f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_HIGHEST_Z_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 1.539f;
                    launch_velocity_or_offset[1] = -1.071f;
                    launch_velocity_or_offset[2] = 13.144f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_FOURTH_HIGHEST_Y_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 1.596f;
                    launch_velocity_or_offset[1] = -2.512f;
                    launch_velocity_or_offset[2] = 8.359f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                case LINKED_CLASS_THIRD_LOWEST_X_LAUNCH_OFFSET:
                    *(u128_7010 *)launch_velocity_or_offset = 0;
                    launch_velocity_or_offset[0] = 0.885f;
                    launch_velocity_or_offset[1] = -5.824f;
                    launch_velocity_or_offset[2] = 8.224f;
                    qcopy(launch_offset, launch_velocity_or_offset);
                    break;
                }
                if (fast_difference_between_rotations(
                        fast_arc_tan(position_or_velocity_scratch[0],
                                     position_or_velocity_scratch[1]),
                        fast_arc_tan(l08_camera.pos.f[0] - moby->pos.x,
                                     l08_camera.pos.f[1] - moby->pos.y)) < 0.2617994f) {
                    initialize_moby_launch_motion(
                        other,
                        (int)moby,
                        position_or_velocity_scratch,
                        launch_offset,
                        truncate_float_to_s32(random_float_between(10.0f, 20.0f)));
                } else {
                    initialize_moby_launch_motion(
                        other,
                        (int)moby,
                        position_or_velocity_scratch,
                        launch_offset,
                        truncate_float_to_s32(random_float_between(30.0f, 45.0f)));
                }
            } else if ((short)cls == 0x1D8) {
                clear_u64_value(position_or_velocity_scratch);
                spawn_explosion(other,
                                position_or_velocity_scratch,
                                other + 0x10,
                                0.0f,
                                0.0f,
                                0x32,
                                0x3C,
                                0x3C,
                                12.0f,
                                8.0f,
                                9.0f,
                                1.0f,
                                -1,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                mark_moby_for_removal(other);
            }
        }
        if (*list++ < -1)
            break;
        index = *list & 0x7FFF;
        other = (char *)((Blk_7010 *)D_L08_0015FFD8 + index);
    }
}

/* Remove active mobys of the selected classes from this moby's linked group. */
void remove_bomber_tank_linked_debris(struct Moby *moby) {
    short *list =
        D_L08_001AC040[(((struct Moby *)D_L08_0015FFD8) + *(s32 *)(moby->pvars + 0x144))->group];
    struct Moby *other;
    u16          index;

    if (list != 0) {
        index = *list & 0x7fff;
        other = (struct Moby *)D_L08_0015FFD8 + index;
        while (1) {
            if (other->state < 0x7f && other != moby) {
                u16 class_id = other->oclass;
                if ((u32)(class_id - 0x24c) < 0xb) {
                    mark_moby_for_removal(other);
                } else if ((s16)class_id == 0x1d8) {
                    mark_moby_for_removal(other);
                }
            }
            if (*list++ < -1)
                break;
            index = *list & 0x7fff;
            other = (struct Moby *)D_L08_0015FFD8 + index;
        }
    }
}

int D_L08_0015F5B0 __asm__("D_L08_0015F5B0") = 0;

int D_L08_0015F5B4_gp __asm__("D_L08_0015F5B4") = 0;

int D_L08_0015F5BC_gp __asm__("D_L08_0015F5BC") = 0;

int bomber_tank_part_3_destroyed_by_class_661_gp __asm__("D_L08_0015F5B8") = 0;
