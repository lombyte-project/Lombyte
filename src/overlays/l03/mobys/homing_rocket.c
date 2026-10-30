/* Level 03 moby class homing_rocket: its spawn helper and update. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/collision.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/l03/mobys/select_moby_target_with_hero_fallback.h"
#include "rnc/overlay/l03/mobys/homing_rocket.h"
#include "rnc/overlay/l03/mobys/homing_rocket_globals.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002D5650), where it is exact; names translated to the US level program. */

extern CollisionHit coll_hit __asm__("D_L03_00173F60");
extern int          collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void         update_entity(void *) __asm__("FUN_L00_00250df8");

char *spawn_homing_rocket(void        *heading_vector,
                          void        *pos,
                          struct Moby *parent,
                          int          timer_frames,
                          float        initial_heading,
                          float        initial_speed,
                          float        unread_value) __asm__("FUN_L03_002d4288");

char *spawn_homing_rocket(void        *heading_vector,
                          void        *pos,
                          struct Moby *parent,
                          int          timer_frames,
                          float        initial_heading,
                          float        initial_speed,

                          float unread_value) {
    char *spawned_moby = (char *)create_moby(0x341);
    char *moby_vars;
    if (spawned_moby != 0) {
        float collision_start_pos[4];
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7E;
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        qcopy(spawned_moby + 0x10, pos);
        qcopy(moby_vars, heading_vector);
        MOBY(spawned_moby)->rot.z = fast_arc_tan(*(float *)moby_vars, *(float *)(moby_vars + 4));
        *(int *)(moby_vars + 0x14) = scale_game_frames(timer_frames);
        *(float *)(moby_vars + 0x18) = unread_value; /* update_homing_rocket never reads it */
        *(float *)(moby_vars + 0x1C) = initial_heading;
        *(float *)(moby_vars + 0x24) = initial_speed;
        *(char **)(moby_vars + 0x10) = parent;
        *(int *)(moby_vars + 0x28) = 0;
        *(OvlQuad *)collision_start_pos = *(OvlQuad *)&parent->pos;
        collision_start_pos[2] = ((float *)pos)[2];
        if (collision_line(collision_start_pos, pos, 2, parent, 0) != 0) {
            *(int *)(moby_vars + 0x14) = 0;
            *(OvlQuad *)&MOBY(spawned_moby)->pos = *(OvlQuad *)&coll_hit;
            *(int *)(moby_vars + 0x24) = 0;
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

extern void select_moby_target_with_hero_fallback(void *, void *, f32) __asm__("FUN_L03_0024e830");
extern f32  frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

/* Pvars of a homing rocket. */
typedef struct {
    Vec4  vel;    /* 0x00 */
    void *ignore; /* 0x10: collision owner to skip */
    s32   timer;  /* 0x14: frames left */
    u8    pad18[4];
    f32   heading;  /* 0x1C */
    f32   turn_vel; /* 0x20 */
    f32   speed;    /* 0x24 */
} RocketVars;

/* Damage record passed to FUN_L00_00259a88. */
typedef struct {
    Vec4 dir; /* 0x00: w is the force */
    u8   pad10[8];
    u8   kind;   /* 0x18 */
    u8   source; /* 0x19 */
    s16  oclass; /* 0x1A */
    u8   pad1C[0x14];
} RocketHit;

extern void  add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int   FUN_L00_00257b90(int, int);
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void  make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void  record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void  cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern float rocket_trail_offset_z_scale_gp __asm__("D_L03_00161B28") __attribute__((sda));
extern CollisionHit D_L03_00173F40;

/* Homing rocket update: flies on, trails smoke, steers up or down toward a target, explodes on impact or timeout. */
extern Vec4 D_0013F490_v __asm__("D_0013F490");
void        update_homing_rocket(struct Moby *rocket_moby) __asm__("FUN_L03_002d43c8");

void update_homing_rocket(struct Moby *rocket_moby) {
    enum {
        ROCKET_FLY = 1,
        ROCKET_BURST_AND_REMOVE = 2
    };
    Vec4                old;
    Vec4                side;
    Vec4                back;
    Vec4                trail_vector_offset;
    MobyTargetSelection target;
    Vec4                trail;
    RocketHit           hit;
    RocketVars         *rocket_vars = (RocketVars *)rocket_moby->pvars;
    Vec4f              *pos;
    u8                  state = rocket_moby->state;
    void              **level;
    float               pitch;
    float               range;

    switch (state) {
    case ROCKET_FLY:
        pos = &rocket_moby->pos;
        range = convert_integer_to_float(rocket_vars->timer * 2) * rocket_vars->speed;
        select_moby_target_with_hero_fallback(rocket_moby, &target, range);
        qcopy(&old, pos);
        fast_vec_add(pos, pos, rocket_vars);
        trail_vector_offset.q = D_0013F490_v.q;
        trail_vector_offset.f[2] *= rocket_trail_offset_z_scale_gp;
        add_vectors(&trail, pos, &trail_vector_offset);
        rocket_moby->rot.z = rocket_vars->heading;
        rocket_moby->rot.x = fast_add_rotations(rocket_moby->rot.x, frame_time * 6.2831855f);
        side.f[0] = fast_cos(rocket_vars->heading) * rocket_vars->speed * -0.5f;
        side.f[1] = fast_sin(rocket_vars->heading) * rocket_vars->speed * -0.5f;
        side.f[2] = 0.0f;
        fast_vec_add(&trail, &side, rocket_vars);
        side.q = trail.q;
        fast_vec_add(&trail, &side, &trail_vector_offset);
        side.q = trail.q;
        fast_vec_normalize(&back, rocket_vars, -0.2f);
        add_vectors(&trail, &back, pos);
        spawn_effect(&back,
                     &side,
                     0x6F00AFFF,
                     0xFF,
                     scale_game_frames(FUN_L00_00257b90(0xF, 0x16)),
                     0x28,
                     FUN_L00_00257b90(0x14, 0x23),
                     1);
        spawn_effect(&back,
                     &side,
                     0x1FFFFFFF,
                     0x4F4F4F,
                     scale_game_frames(FUN_L00_00257b90(0x1E, 0x3C)),
                     0x28,
                     FUN_L00_00257b90(0x32, 0x4B),
                     0);
        if (target.found != 0) {
            pitch = -fast_arc_tan(vector_distance_xy(pos, &target.pos),
                                  target.pos.f[2] - rocket_moby->pos.z);
            rocket_moby->rot.y = spring_turn(&rocket_vars->turn_vel,
                                             rocket_moby->rot.y,
                                             pitch,
                                             frame_time_sq_macro * 0.7853982f,
                                             frame_time_sq_macro * 3.1415927f,
                                             frame_time * 0.5235988f);
            rocket_moby->pos.z += fast_sin(-rocket_moby->rot.y) * rocket_vars->speed;
        }
        if (collision_line(pos, &old, 0, rocket_vars->ignore, 0) != 0 ||
            coll_sphere(0.2f, pos, 0, rocket_vars->ignore) != 0) {
            level = (void **)&D_L03_00173F40;
            if (level[6] != NULL) {
                make_hit_descriptor(&hit, rocket_moby, 0x10001, 1.0f, rocket_vars);
                hit.dir.f[0] = fast_cos(rocket_vars->heading);
                hit.dir.f[1] = fast_sin(rocket_vars->heading);
                hit.dir.f[2] = 1.0f;
                hit.dir.f[3] = 5627.925f;
                hit.kind = state;
                hit.source = state;
                hit.oclass = rocket_moby->oclass;
                record_moby_hit(level[6], &hit);
            }
            rocket_moby->state = 2;
        }
        if (fast_dec_timer(&rocket_vars->timer) != 0) {
            rocket_moby->state = 2;
        }
        break;
    case ROCKET_BURST_AND_REMOVE:
        cart_burst(rocket_moby, &rocket_moby->pos, 0, 0.25f, 13.0f);
        mark_moby_for_removal(rocket_moby);
        break;
    }
}
