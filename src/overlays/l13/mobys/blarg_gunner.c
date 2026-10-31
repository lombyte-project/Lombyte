/* Level 13 Blarg gunner moby class (0x24), its hits and the spawn helper the turret uses. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
#include "qcopy.h"

extern int FUN_L00_00257b90(int, int);

extern void FUN_L00_00257470(void *, int, int);

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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

typedef struct {
    void        *unk0;
    u8           pad4[0x8];
    void        *unkC;
    u8           pad10[0x10];
    f32          unk20;
    s16          unk24;
    u8           pad26[2];
    u8           unk28;
    u8           unk29;
    u8           unk2A;
    u8           pad2B[5];
    f32          unk30;
    u8           pad34[0x2C];
    u8           unk60[0x10];
    struct Moby *parent;
} Vars;

extern void update_entity(struct Moby *moby) __asm__("FUN_L00_00250df8");

extern void copy_hero_moby_field_38(struct Moby *moby) __asm__("FUN_L00_0025d1b8");

struct Moby *spawn_blarg_turret_child(struct Moby *parent) __asm__("FUN_L13_002b4958");

struct Moby *spawn_blarg_turret_child(struct Moby *parent) {
    struct Moby *child_moby;
    Vars        *child_vars;

    child_moby = create_moby(0x24);
    if (child_moby != 0) {
        child_vars = (Vars *)child_moby->pvars;
        child_vars->parent = parent;
        child_vars->unk0 = &child_vars->unk20;
        child_vars->unkC = child_vars->unk60;
        child_moby->update_dist = parent->update_dist;
        child_moby->draw_dist = child_vars->parent->draw_dist;
        child_moby->visible = 1;
        child_moby->flags |= 0x20;
        child_moby->state = 0;
        child_moby->command = 0;
        qcopy(&child_moby->pos, &child_vars->parent->pos);
        child_moby->scale = child_moby->pclass->scale * parent->scale / parent->pclass->scale;
        child_vars->unk20 = 1.0f;
        child_vars->unk24 = 1;
        child_vars->unk29 = 1;
        child_vars->unk2A = 3;
        child_vars->unk28 = 0;
        child_vars->unk30 = 0.9f;
        update_entity(child_moby);
        copy_hero_moby_field_38(child_moby);
    }
    return child_moby;
}

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void process_blarg_gunner_hits(struct Moby *moby, char *moby_vars) __asm__("FUN_L13_002b4a58");

void process_blarg_gunner_hits(struct Moby *moby, char *moby_vars) {
    enum {
        HIT_REACTION_NONE = 11
    };
    float *tm = (float *)(moby_vars + 0x20);
    char  *hit_record;
    int    k;
    int    hit_reaction_result;
    if (moby->state != 2) {
        hit_record = find_moby_hit(moby, 0x330000, 0);
        hit_reaction_result = hit_reaction(moby, hit_record, tm, 0, &k, 0, 0, 4);
        if (hit_record != 0) {
            if (*(int *)(hit_record + 0x24) & 0x800000) {
                char *hit_source_moby = *(char **)(hit_record + 0x20);
                if (hit_source_moby == 0) {
                    *(int *)(hit_record + 0x2C) = 0;
                    k = 1;
                } else {
                    float f = fast_difference_between_rotations(
                        fast_arc_tan(MOBY(hit_source_moby)->pos.x - moby->pos.x,
                                     MOBY(hit_source_moby)->pos.y - moby->pos.y),
                        *(float *)(*(char **)(*(char **)(moby_vars + 0x70) + 0x78) + 0x54));
                    if (f < 1.65806282f) {
                        *(int *)(hit_record + 0x2C) = 0;
                        k = 1;
                    }
                }
            }
        }
        switch (hit_reaction_result) {
        case 1:
        case 2:
            *(int *)tm = 0;
            break;
        case 0:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case HIT_REACTION_NONE:
            break;
        }
        if (k >= 2) {
            float hit_damage = *(float *)(hit_record + 0x2C);
            float current_health = *tm;
            if (current_health <= hit_damage) {
                char *x;
                *(int *)tm = 0;
                moby->flags &= 0xEFFF;
                x = *(char **)(moby_vars + 0x70);
                if (x != 0) {
                    if (*(short *)(x + 0xA6) == 0x1D) {
                        unsigned char parent_turret_state = x[0x20];
                        if (parent_turret_state != 0xFE) {
                            if (parent_turret_state != 0xFD)
                                x[0x20] = 4;
                        }
                    }
                }
                FUN_L00_00257470(moby, 0, -1);
                moby_vars[0x67] = 0x78;
                trooper_anim_update(moby, (short *)(moby_vars + 0x60));
                if (moby->prev_seq != 3)
                    blend_moby_animation(moby, 3, 1, 0);
                moby->state = 2;
            } else {
                *tm = current_health - hit_damage;
                *(unsigned char *)(moby_vars + 0x67) = 0xFA;
                *(short *)(moby_vars + 0x26) = scale_game_frames(0x3C);
                trooper_anim_update(moby, (short *)(moby_vars + 0x60));
            }
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_vars + 0x60);
}

extern float D_L13_001670C0[];

extern int FUN_L13_002b4a58_c() __asm__("FUN_L13_002b4a58");

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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

void blarg_gunner_update(struct Moby *moby) __asm__("FUN_L13_002b4c80");

/* Moby class 36 */
void blarg_gunner_update(struct Moby *moby) {
    enum {
        BLARG_GUNNER_STATE_INITIALIZE = 0,
        BLARG_GUNNER_STATE_ACTIVE = 1,
        BLARG_GUNNER_STATE_DESTROY = 2
    };
    unsigned char *moby_vars = moby->pvars;
    float          explosion_offset[4];
    float          explosion_spawn_position[4];
    switch (moby->state) {
    case BLARG_GUNNER_STATE_INITIALIZE:
        moby_vars[0x29] = 1;
        moby->state = 1;
        moby_vars[0x5A] = 6;
        moby_vars[0x58] = 8;
        break;
    case BLARG_GUNNER_STATE_ACTIVE:
        if ((*(unsigned char **)(moby_vars + 0x70))[0x20] == 3) {
            if (moby->prev_seq != 2) {
                int x = FUN_L00_00257b90(0, 3);
                int y = FUN_L00_00257b90(7, 0xC);
                blend_moby_animation(moby, 2, x, y);
            }
        } else {
            if (moby->prev_seq >= 2 || (moby->anim_flags & 2)) {
                if (!random_integer_below(7)) {
                    if (moby->prev_seq != 1) {
                        blend_moby_animation(moby, 1, 0, FUN_L00_00257b90(7, 0xD));
                    }
                } else {
                    if (moby->prev_seq != 0) {
                        blend_moby_animation(moby, 0, 0, FUN_L00_00257b90(7, 0xD));
                    }
                }
                moby->anim_speed = random_float_between(0.8f, 1.2f);
            }
        }
        FUN_L13_002b4a58_c(moby, moby_vars);
        break;
    case BLARG_GUNNER_STATE_DESTROY:
        if (moby->anim_flags & 2) {
            fast_vec_sub(explosion_offset, D_L13_001670C0, &moby->pos);
            moby_point_position(moby, 0, explosion_spawn_position);
            spawn_explosion(moby,
                            explosion_offset,
                            explosion_spawn_position,
                            0.0f,
                            0.0f,
                            0x14,
                            9,
                            0x20,
                            10.0f,
                            7.0f,
                            20.0f,
                            -1,
                            2.0f,
                            40.0f,
                            0,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
