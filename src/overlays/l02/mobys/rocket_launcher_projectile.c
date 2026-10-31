/* Level 02 moby class rocket_launcher_projectile (0x4b8): the shot the rocket launcher fires, which drifts along its velocity shedding a smoke trail, and its spawner. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/l02/mobys/rocket_launcher_projectile_globals.h"

typedef struct {
    Vec4f vel;
    s32   timer;
} ProjectileVars;

typedef struct {
    u8  pad0[0x5C];
    u8 *tint;
} LevelFx;

extern f32 rocket_launcher_projectile_vertical_velocity_approach_scale_gp __asm__("D_L02_00161F58")
    __attribute__((sda));
s32            D_L02_00161F5C = 0x30704020;
s32            D_L02_00161F60 = 0x50e0e0e0;
extern LevelFx l02_projectile_trail_particle_tint_source __asm__("D_L02_001B2680");
extern void    fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void    fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32     approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32     scale_game_frames(s32) __asm__("FUN_001f96f8");
extern u8     *spawn_particle(void *, void *, s32, s32, s32, s32, f32, f32, f32, f32, f32) __asm__("FUN_L00_002712b8");

/* Rocket launcher projectile update: drifts along its velocity, sheds particle trails and expires when its timer runs out. */
void rocket_launcher_projectile_update(struct Moby *moby) __asm__("FUN_L02_002ebf20");

/* Moby class 1208 */
void rocket_launcher_projectile_update(struct Moby *moby) {
    enum {
        PROJECTILE_STATE_REMOVE = 0,
        PROJECTILE_STATE_ACTIVE = 1
    };
    ProjectileVars *projectile_vars = (ProjectileVars *)moby->pvars;
    Vec4f           trail_position, back, half, jit;
    u8             *particle;
    s32             i;

    switch (moby->state) {
    case PROJECTILE_STATE_REMOVE:
        mark_moby_for_removal(moby);
        return;
    case PROJECTILE_STATE_ACTIVE:
        break;
    default:
        return;
    }
    fast_vec_add(&moby->pos, &moby->pos, &projectile_vars->vel);
    approach_value(&projectile_vars->vel.z,
                   0.0f,
                   rocket_launcher_projectile_vertical_velocity_approach_scale_gp * frame_time_sq);
    moby->rot.y =
        1.5707964f - fast_arc_tan(vector_length_xy(&projectile_vars->vel), projectile_vars->vel.z);
    qcopy(&trail_position, &moby->pos);
    fast_vec_scale(&half, &projectile_vars->vel, 0.5f);
    fast_vec_scale(&back, &projectile_vars->vel, -0.1f);
    for (i = 1; i >= 0; i--) {
        s32 particle_frame_count =
            scale_game_frames(truncate_float_to_s32(random_float_between(20.0f, 60.0f)));
        particle = spawn_particle(&trail_position,
                                  &back,
                                  particle_frame_count,
                                  D_L02_00161F5C >> 24,
                                  D_L02_00161F5C & 0xFFFFFF,
                                  3,
                                  random_float_between(300000.0f, 500000.0f),
                                  10000.0f,
                                  1.0f,
                                  -0.0002f,
                                  0.0f);
        if (particle) {
            particle[2] = *l02_projectile_trail_particle_tint_source.tint;
            particle[9] = truncate_float_to_s32(4.0f) - 0x80;
        }
        fast_vec_add(&trail_position, &trail_position, &half);
        random_vector(&jit, 0.5f, 1.5f);
        fast_vec_add(&trail_position, &trail_position, &jit);
    }
    particle = spawn_particle(&moby->pos,
                              &back,
                              scale_game_frames(6),
                              D_L02_00161F60 >> 24,
                              D_L02_00161F5C & 0xFFFFFF,
                              3,
                              250000.0f,
                              5000.0f,
                              1.0f,
                              -0.0004f,
                              0.0f);
    if (particle)
        particle[9] = truncate_float_to_s32(4.0f) - 0x80;
    if (moby->visible == 0)
        fast_dec_timer(&projectile_vars->timer);
    else
        projectile_vars->timer = scale_game_frames(60);
    if (projectile_vars->timer == 0)
        mark_moby_for_removal(moby);
}

extern int scale_game_frames(int) __asm__("FUN_001f96f8");
int        D_L02_00161F4C = 0xd0;
int        D_L02_00161F50 = 0x60;
int        D_L02_00161F54 = 0x10;

/* Spawns a projectile moby from src at pos, heading along dir, with its lifetime in the moby data. */
char *
spawn_projectile_moby(struct Moby *source_moby, void *pos, float *dir) __asm__("FUN_L02_002ec228");

char *

spawn_projectile_moby(struct Moby *source_moby, void *spawn_position, float *projectile_direction) {
    char *moby;
    char *projectile_vars;
    moby = create_moby(0x4B8);
    if (moby != 0) {
        moby[0x31] = 1;
        MOBY(moby)->update_dist = MOBY(moby)->draw_dist = 0xFF;
        set_moby_color(moby, D_L02_00161F4C, D_L02_00161F50, D_L02_00161F54);
        MOBY(moby)->flags = source_moby->flags;
        qcopy(moby + 0x10, spawn_position);
        MOBY(moby)->rot.z = fast_arc_tan(projectile_direction[0], projectile_direction[1]);
        MOBY(moby)->rot.y =
            fast_arc_tan(vector_length_xy(projectile_direction), projectile_direction[2]);
        moby[0x20] = 1;
        projectile_vars = ((char *)MOBY(moby)->pvars);
        qcopy(projectile_vars, projectile_direction);
        *(int *)(projectile_vars + 0x10) = scale_game_frames(0x3C);
    }
    return moby;
}
