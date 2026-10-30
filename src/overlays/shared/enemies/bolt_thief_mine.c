/* Floating mine (class 0x3A5, Wrench name mine): the mine the bolt thief drops; spins and pulses, blows up on a hit and respawns out of view; its spawner. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"

typedef struct {
    struct Moby *link;
    float        rest_z;
    float        z_speed;
    float        phase;
    float        spin[3];
} FloatMineVars;

typedef struct {
    float damage;
    s16   unk4;
    u8    pad6[0xA];
    float unk10;
    u8    pad14[0x26];
    u8    unk3A;
    u8    unk3B;
    u8    pad3C[4];
} HitInfo;

typedef struct {
    u8           pad0[0x20];
    struct Moby *moby;
} MobyHit;

extern char *floating_mine_respawn_address_limit __asm__("D_L14_0015FFDC");

#include "rnc/overlay/shared/enemies/bolt_thief_mine_data/update_bolt_thief_mine.h"

extern float probe_ground_height(float, void *, int) __asm__("FUN_00213508");
extern int   resolve_moby_hit_response(
      struct Moby *, MobyHit *, HitInfo *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void explode_ff640(struct Moby *,
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

/* Floating mine: spins and pulses (and bobs in level 16); blows up on a hit and respawns once out of view. */
void update_bolt_thief_mine(struct Moby *moby) __asm__("FUN_L14_002ff640");

void update_bolt_thief_mine(struct Moby *moby) {
    enum {
        FLOAT_MINE_INITIALIZE = 0,
        FLOAT_MINE_ACTIVE = 1,
        FLOAT_MINE_WAIT_OFFSCREEN_RESPAWN = 2
    };
    FloatMineVars *vars = (FloatMineVars *)moby->pvars;
    union {
        HitInfo info;
        float   sphere[4];
    } scratch;
    float    explosion_vector[4];
    int      count;
    MobyHit *hit;
    float    glow_pulse_sine;
    int      glow_red_intensity;
    int      glow_green_blue_intensity;

    switch (moby->state) {
    case FLOAT_MINE_INITIALIZE:
        vars->rest_z = -1.0f;
        if (vars->link == 0) {
            moby->pos.z = probe_ground_height(0.5f, &moby->pos, 0) + 0.7f;
            vars->spin[0] = random_float_between(0.010471975f, 0.020943951f);
            vars->spin[1] = random_float_between(0.006981317f, 0.013962634f);
            vars->spin[2] = random_float_between(0.0034906585f, 0.006981317f);
            moby->state = 1;
        } else if ((s8)vars->link->state < 0) {
            moby->state = 1;
        }
        break;
    case FLOAT_MINE_ACTIVE: {
        if (current_level_index == 16) {
            if (vars->rest_z < 0.0f)
                vars->rest_z = probe_ground_height(0.5f, &moby->pos, 0) + 0.7f;
            vendor_spring(&moby->pos.z,
                          &vars->z_speed,
                          vars->rest_z,
                          frame_scale_sq * 0.03f,
                          frame_scale_sq * 0.3f,
                          0.0f);
            if ((char *)moby > floating_mine_respawn_address_limit && hero.state.control_mode != 15)
                goto remove;
        }
        hit = find_moby_hit(moby, 0x230000, 0);
        fast_mem_zero16(&scratch.info, 0x40);
        scratch.info.unk4 = 1;
        scratch.info.damage = 1.0f;
        scratch.info.unk10 = 0.5f;
        scratch.info.unk3A = 7;
        scratch.info.unk3B = 0xFF;
        resolve_moby_hit_response(moby, hit, &scratch.info, 0, &count, 0, 0, 4);
        if ((count >= 2 && hit != 0 && hit->moby->oclass != 0x283) ||
            hero.coll_hit_moby == (u8 *)moby) {
            clear_u64_value(explosion_vector);
            allocate_voice_for_target_entry(0, 0, moby);
            if (hero.coll_hit_moby == (u8 *)moby)
                explode_ff640(moby,
                              explosion_vector,
                              &moby->pos,
                              3.0f,
                              1.0f,
                              10,
                              20,
                              60,
                              3.0f,
                              5.0f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              1,
                              1,
                              -1,
                              0);
            else
                explode_ff640(moby,
                              explosion_vector,
                              &moby->pos,
                              0.0f,
                              0.0f,
                              10,
                              20,
                              60,
                              3.0f,
                              5.0f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              1,
                              1,
                              -1,
                              0);
            if ((char *)moby < floating_mine_respawn_address_limit) {
                moby->state = 2;
                moby->flags |= 0x41;
                moby->hit_slot = 0xFF;
                moby->collision = 0;
                moby->update_dist = 0xFF;
                return;
            }
        remove:
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case FLOAT_MINE_WAIT_OFFSCREEN_RESPAWN: {
        qcopy(scratch.sphere, &moby->pos);
        scratch.sphere[3] = 3.0f;
        if (fast_bsphere_check(scratch.sphere, 64.0f) == -1) {
            moby->state = 1;
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
        }
        break;
    }
    }
    vars->phase = fast_add_rotations(vars->phase, floating_mine_glow_phase_step_gp);
    glow_pulse_sine = fast_sin(vars->phase);
    glow_red_intensity = truncate_float_to_s32(glow_pulse_sine * 48.0f) + 0xCF;
    glow_green_blue_intensity = truncate_float_to_s32(glow_pulse_sine * 32.0f) + 0x30;
    moby->glow_color = glow_green_blue_intensity << 16 |
                       (0xFF000000 | glow_green_blue_intensity << 8) | glow_red_intensity;
    moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin[0]);
    moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin[1]);
    moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin[2]);
}

/* Spawns moby class 0x3A5 at the given position, owned by owner, with three randomised angle values. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00300F80), where it is exact; names translated to the US level program. */

unsigned char *spawn_bolt_thief_mine(unsigned char *owner,
                                     char          *spawn_position) __asm__("FUN_L14_002ffaf8");

unsigned char *spawn_bolt_thief_mine(unsigned char *owner, char *spawn_position) {
    unsigned char *spawned_moby = create_moby(0x3A5);
    if (spawned_moby != 0) {
        char *spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(unsigned char **)spawned_vars = owner;
        *(int *)(spawned_vars + 8) = 0;
        *(int *)(spawned_vars + 0xC) = 0;
        *(float *)(spawned_vars + 4) = -1.0f;
        *(float *)(spawned_vars + 0x10) = random_float_between(0.0104719754f, 0.0209439509f);
        *(float *)(spawned_vars + 0x14) = random_float_between(0.00698131695f, 0.0139626339f);
        *(float *)(spawned_vars + 0x18) = random_float_between(0.00349065848f, 0.00698131695f);
        qcopy(spawned_moby + 0x10, spawn_position);
        MOBY(spawned_moby)->rot.z = *(float *)(owner + 0x48);
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
