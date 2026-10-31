/* Level 02 moby class sand_shark_nest. */
#include "types.h"
#include "rnc/game/bolt_sources.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern float D_0015ED6C;
extern int   step_moby_motion(void *, void *) __asm__("FUN_L00_0025c698");

extern int D_L02_001ABE40[];
extern int D_L02_0015FFD8; /* no foreign declaration */

#include "sda.h"
#include "rnc/overlay/quad.h"

extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002D7550), where it is exact; names translated to the US level program. */

void launch_sand_shark(struct Moby *moby,
                       void        *position,
                       float       *direction) __asm__("FUN_L02_002d6118");

void launch_sand_shark(struct Moby *moby, void *position, float *direction) {
    char *sand_shark_vars = (char *)moby->pvars;
    moby->state = 0x1B;
    moby->flags &= 0xFFBE;
    if (moby->prev_seq != 0xB) {
        blend_moby_animation(moby, 0xB, 0, 1);
    }
    qcopy(&moby->pos, position);
    qcopy(sand_shark_vars + 0x40, direction);
    moby->rot.z = fast_arc_tan(direction[0], direction[1]);
    update_entity(moby);
}

extern char  D_L02_001673C0[];
extern float D_L02_0015F580[] MACRO_ADDR;

extern void *FUN_L00_00263fd8(char  *src,
                              int    cls,
                              float *pos,
                              void  *mat,
                              int    a8,
                              int    a9,
                              float  scale,
                              float *v10,
                              float *v11,
                              float *v12);
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_0025a120(void *);
extern void  spawn_explosion(void *,
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

/* UpdateMoby_656: steps the moby's animation state from 0/2 (when flagged) and 1/3 (when the timer runs out) */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DDE48), where it is exact; names translated to the US level program. */

extern int FUN_L00_001fefc8(void *);

/* pvars of the class sand_shark_nest_update updates (only the fields it touches). */
typedef struct {
    char          pad0[0x20];
    float         f20; /* 0x20 */
    short         s24; /* 0x24 */
    char          pad26[8];
    unsigned char b2E; /* 0x2E */
    char          pad2F[0x11];
    float         vel[4]; /* 0x40: passed to spawn_explosion */
    char          pad50[0x20];
    char          anim[0x64]; /* 0x70: passed to step_moby_motion */
    int           delay;      /* 0xD4: frames between spawns */
    int           timer;      /* 0xD8 */
    char          padDC[4];
    int           spawner; /* 0xE0: spawner moby index, -1 if none */
} Varscb38;

/* Counters kept in the spawner moby's pvars. */
typedef struct {
    char pad0[0x14C];
    int  i14C;
    int  i150;
} Spawnercb38;

extern BoltSourceStats bolt_source_stats_lui[][64] __asm__("D_0014D590") NOT_SDA;
extern int             D_0014EE90[];
extern int             D_L02_0015FFD8;
extern float           compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
void                   handle_sand_shark_nest_damage(struct Moby *moby) __asm__("FUN_L02_002dcfe0");

extern void  handle_sand_shark_nest_damage(struct Moby *m) __asm__("FUN_L02_002dcfe0");
extern char *FUN_L02_002dd2a8_c(void *) __asm__("FUN_L02_002dd2a8");
extern void  spawn_explosion(void *,
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

/* Spawner-fed moby: counts itself in on spawn, hands itself to a group target on a timer, fires a
   projectile at it (state 2) and bursts into debris when its hit animation flags it (state 4). */
void sand_shark_nest_update(unsigned char *moby) __asm__("FUN_L02_002dcb38");

/* Moby class 668 */
void sand_shark_nest_update(unsigned char *moby) {
    enum {
        SAND_SHARK_NEST_INITIALIZE = 0,
        SAND_SHARK_NEST_WAIT_FOR_SPAWN = 1,
        SAND_SHARK_NEST_LAUNCH_SAND_SHARK = 2,
        SAND_SHARK_NEST_HIT_RECOVERY = 3,
        SAND_SHARK_NEST_DEATH_KNOCKBACK = 4
    };
    Varscb38 *nest_vars = ((Varscb38 *)MOBY(moby)->pvars);
    char     *launched_shark;
    int       spawn_delay_factor;
    OvlVec4   launch_velocity_or_explosion_position;
    OvlVec4   launch_position;

    handle_sand_shark_nest_damage((struct Moby *)moby);
    if ((signed char)moby[0x20] < 0) {
        return;
    }
    if (moby[0x31] != 0 && fast_vec_dist(moby + 0x10, D_L02_001673C0) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7F] = 0x16;
    }
    switch (moby[0x20]) {
    case SAND_SHARK_NEST_INITIALIZE:
        MOBY(moby)->unkB4 -= bolt_source_stats_lui[2][moby[0xB1]].bolts;
        if (MOBY(moby)->unkB4 <= 0) {
            MOBY(moby)->unkB4 = 1;
        }
        moby[0x20] = 1;
        nest_vars->f20 = 3.0f;
        nest_vars->s24 = 3;
        nest_vars->b2E = 1;
        nest_vars->timer = scale_game_frames(nest_vars->delay);
        if (nest_vars->spawner >= 0) {
            (*(Spawnercb38 **)((nest_vars->spawner << 8) + D_L02_0015FFD8 + 0x78))->i150 += 1;
        }
        break;
    case SAND_SHARK_NEST_WAIT_FOR_SPAWN:
        if (fast_dec_timer(&nest_vars->timer)) {
            if (FUN_L02_002dd2a8_c(moby)) {
                moby[0x20] = 2;
                if (moby[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                }
            } else {
                nest_vars->timer = scale_game_frames(nest_vars->delay);
            }
        }
        break;
    case SAND_SHARK_NEST_LAUNCH_SAND_SHARK:
        if (moby[0x70] & 2) {
            moby[0x20] = 1;
            if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        } else if (compute_interpolated_record_value(moby) == 12.0f) {
            spawn_delay_factor = D_0014EE90[moby[0xB0]];
            if (spawn_delay_factor > 20) {
                spawn_delay_factor = 20;
            }
            nest_vars->timer =
                scale_game_frames(nest_vars->delay + scale_game_frames(spawn_delay_factor * 15));
            launched_shark = FUN_L02_002dd2a8_c(moby);
            if (launched_shark) {
                char *od = *(char **)(launched_shark + 0x78);
                if (nest_vars->spawner >= 0) {
                    (*(Spawnercb38 **)((nest_vars->spawner << 8) + D_L02_0015FFD8 + 0x78))->i14C +=
                        1;
                }
                qcopy(&launch_position, moby + 0x10);
                launch_position.f[2] += 1.5f;
                fast_vec_sub(&launch_velocity_or_explosion_position, od + 0x220, moby + 0x10);
                set_vector_length_xy(&launch_velocity_or_explosion_position,
                                     &launch_velocity_or_explosion_position,
                                     2.0f * D_0015ED6C);
                launch_velocity_or_explosion_position.f[2] = D_0015ED6C * 6.0f;
                launch_sand_shark((unsigned char *)launched_shark,
                                  &launch_position,
                                  launch_velocity_or_explosion_position.f);
            }
        }
        break;
    case SAND_SHARK_NEST_HIT_RECOVERY:
        step_moby_motion(moby, nest_vars->anim);
        if (moby[0x70] & 2) {
            moby[0x20] = 1;
            if (moby[0x53] != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
        break;
    case SAND_SHARK_NEST_DEATH_KNOCKBACK:
        if (step_moby_motion(moby, nest_vars->anim) & 0x40) {
            if (nest_vars->spawner >= 0) {
                extern int D_L02_0015FFD8_s __asm__("D_L02_0015FFD8") __attribute__((sda));
                (*(Spawnercb38 **)((nest_vars->spawner << 8) + D_L02_0015FFD8_s + 0x78))->i150 -= 1;
            }
            qcopy(&launch_velocity_or_explosion_position, moby + 0x10);
            spawn_explosion(moby,
                            nest_vars->vel,
                            &launch_velocity_or_explosion_position,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            -1,
                            1.0f,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00263fd8((char *)moby,
                             0x6E4,
                             (float *)&MOBY(moby)->pos,
                             moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x6E4,
                             (float *)&MOBY(moby)->pos,
                             moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x6E5,
                             (float *)&MOBY(moby)->pos,
                             moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            FUN_L00_00263fd8((char *)moby,
                             0x6E5,
                             (float *)&MOBY(moby)->pos,
                             moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L02_0015F580,
                             D_L02_0015F580,
                             D_L02_0015F580);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DE418), where it is exact; names translated to the US level program. */

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");

void handle_sand_shark_nest_damage(struct Moby *moby) {
    char *nest_vars = (char *)moby->pvars;
    float hit_knockback_vector[4];
    int   hit;
    float dmg;
    float k1;
    float k2;
    char *hit_record;
    dmg = 0.0f;
    if (((unsigned char *)nest_vars)[0x2E] == 2) {
        nest_vars[0x2E] = 1;
        if (*(int *)(nest_vars + 0xE0) >= 0) {
            char *od = *(char **)((*(int *)(nest_vars + 0xE0) << 8) + D_L02_0015FFD8 + 0x78);
            *(int *)(od + 0x150) -= 1;
        }
        mark_moby_for_removal(moby);
        return;
    }
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_reaction(moby, hit_record, nest_vars + 0x20, 0, &hit, &dmg, 0, 4);
    if (hit_record != 0 && hit != 1 && moby->state != 4) {
        if (hit == 3) {
            *(float *)(nest_vars + 0x20) = 0.0f;
        } else {
            *(float *)(nest_vars + 0x20) -= dmg;
        }
        *(int *)(nest_vars + 0x94) = 4;
        if (0.0f < *(float *)(nest_vars + 0x20)) {
            {
                float a = D_0015ED6C * 4.0f;
                float b = D_0015ED6C * 3.0f;
                *(float *)(nest_vars + 0x88) = a;
                *(float *)(nest_vars + 0x8C) = b;
            }
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit_record + 0x10);
            trooper_knockback_arc(hit_knockback_vector, &k1, nest_vars + 0x88, nest_vars + 0x8C);
            trooper_start_knockback(k1, moby, nest_vars + 0x70, 3, 1, 0);
            if (1.0f <= dmg) {
                nest_vars[0x67] = 0x78;
                moby->state = 3;
                blend_moby_animation(moby, 2, 0, 0);
            } else {
                nest_vars[0x67] = 0x78;
            }
        } else {
            moby->flags &= 0xEFFF;
            ((unsigned char *)nest_vars)[0x67] = 0xFA;
            moby->state = 4;
            {
                float b = D_0015ED6C * 11.0f;
                float a = D_0015ED6C * 3.0f;
                *(float *)(nest_vars + 0x8C) = a;
                *(float *)(nest_vars + 0x88) = b;
            }
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit_record + 0x10);
            trooper_knockback_arc(hit_knockback_vector, &k2, nest_vars + 0x88, nest_vars + 0x8C);
            trooper_start_knockback(k2, moby, nest_vars + 0x70, 3, 1, 0);
            *(float *)(nest_vars + 0xC0) = 9.0f;
            *(float *)(nest_vars + 0xC4) = 19.0f;
            if (moby->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, 0);
            FUN_L00_00257470(moby, 0, -1);
        }
        trooper_anim_update(moby, (short *)(nest_vars + 0x60));
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, nest_vars + 0x60);
    FUN_L00_001fefc8(&moby->command);
}

typedef struct {
    char          pad0[0x20];
    unsigned char state;
    unsigned char group;
    char          pad22[0x84];
    short         type;
    char          padA8[0xC];
    short         hp;
    char          padB6[2];
    void         *target;
    char          padBC[0x44];
} Level02GroupMoby;

extern Level02GroupMoby *D_L02_0015FFD8_g __asm__("D_L02_0015FFD8");

/* Finds the group's moby of type 0x244 in state 0x1A, hands it arg as target, and transfers hp to arg; returns it or 0. */
Level02GroupMoby *
select_inactive_group_sand_shark_for_nest(Level02GroupMoby *owner) __asm__("FUN_L02_002dd2a8");

Level02GroupMoby *select_inactive_group_sand_shark_for_nest(Level02GroupMoby *nest_moby) {
    short            *group_ids;
    Level02GroupMoby *mobys;
    if (nest_moby->group == 0xFF) {
        return 0;
    }
    group_ids = (short *)D_L02_001ABE40[nest_moby->group];
    mobys = D_L02_0015FFD8_g;
    do {
        int i = *(unsigned short *)group_ids & 0x7FFF;
        if (mobys[i].type == 0x244 && mobys[i].state == 0x1A) {
            mobys[i].target = nest_moby;
            if (nest_moby->hp < mobys[i].hp) {
                mobys[i].hp = nest_moby->hp;
            }
            nest_moby->hp -= D_L02_0015FFD8_g[i].hp;
            if (nest_moby->hp <= 0) {
                nest_moby->hp = 1;
            }
            return &D_L02_0015FFD8_g[i];
        }
    } while (*group_ids++ >= 0);
    return 0;
}
