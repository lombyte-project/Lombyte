/* Level 08 moby class 0x1a8: shuttle. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/ui/help/try_set_help_message.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

extern float D_0015ED70;

#include "rnc/overlay/level_path.h"

extern int   D_L08_0015F5CC;
extern void  attach_manipulator(void *, s32, void *) __asm__("FUN_0020cb10");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");

/* Pvars of the hero-carrying lift. */
typedef struct {
    u8   pad0[0x20];
    u8   motion[0x40]; /* 0x20: carried-moby motion state for update_entity_matrix */
    Vec4 home_pos;     /* 0x60 */
    Vec4 home_rot;     /* 0x70 */
    s32  path;         /* 0x80: -1 when it has no path */
    u8   pad84[8];
    s32  stop; /* 0x8C: -1 when no stop is selected */
    u8   pad90[0x10];
    u8   manip;     /* 0xA0 */
    u8   manip_set; /* 0xA1 */
    u8   padA2[0xE];
    u8   spin_state[0x30]; /* 0xB0 */
    s16  unkE0;            /* 0xE0 */
    u8   padE2[0x1E];
    s32  unk100; /* 0x100 */
    u8   pad104[0xC];
    Vec4 spin;    /* 0x110: x is the wheel angle */
    s32  active;  /* 0x120 */
    s32  ridable; /* 0x124 */
} LiftVars;

typedef struct {
    s32 busy;
    u8  pad4[0x20];
    s32 owner; /* 0x24: -1 when free */
} LiftLock;

extern s32      level_game_mode __asm__("D_L08_0015F5C4");
extern s32      D_L08_0015F594 __attribute__((sda));
extern LiftLock l08_shuttle_departure_lock __asm__("D_L08_00179C10");
extern s32      D_0013CAE4[];
extern void     build_quaternion_from_axis_rotations(void *, void *) __asm__("FUN_L00_00259f50");
extern void
resolve_point_path_segment_xy_clearance(s32, float, void *, void *) __asm__("FUN_L00_00261d78");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
void        update_shuttle_stop_world_points(struct Moby *moby) __asm__("FUN_L08_002dc0c8");
void        select_shuttle_route_for_hero(struct Moby *moby) __asm__("FUN_L08_002dc180");
void        update_shuttle_wobble(struct Moby *moby) __asm__("FUN_L08_002dc4a0");
int         advance_shuttle_path_travel(struct Moby *moby) __asm__("FUN_L08_002dc648");
int         advance_shuttle_path_travel(struct Moby *m) __asm__("FUN_L08_002dc648");
void        try_set_shuttle_departure_help_message(struct Moby *moby) __asm__("FUN_L08_002dc7f0");

/* Lift update: runs the lift's state machine (0 setup, 2 carry the hero
 * when riding, 3 travel to the selected stop) and moves anything riding it
 * by how far it moved this frame. */
void shuttle_update(struct Moby *moby) __asm__("FUN_L08_002dbdb0");

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD440), where it is exact; names translated to the US level program. */
extern char shuttle_stop_local_points[] __asm__("D_L08_001DB280");

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD4F8), where it is exact; names translated to the US level program. */
typedef struct {
    int   count;
    int   pad[3];
    float e[1][4];
} Route;

#include "rnc/game/game_globals.h"

extern int D_L08_0015F5CC;

/* Per-frame update: ease the angle and position fields toward their targets. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD9C0), where it is exact; names translated to the US level program. */
extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("FUN_00214e58");

void shuttle_update(struct Moby *moby) {
    enum {
        SHUTTLE_STATE_INITIALIZE = 0,
        SHUTTLE_STATE_WAIT_FOR_DEPARTURE = 2,
        SHUTTLE_STATE_TRAVEL = 3
    };
    LiftVars *vars = (LiftVars *)moby->pvars;
    Vec4      old_pos;
    Vec4      old_rot;
    Vec4      delta;
    Vec4      hero_position_probe;

    qcopy(&old_pos, &moby->pos);
    qcopy(&old_rot, &moby->rot);
    if (level_game_mode != 2 && vars->ridable != 0) {
        moby->visible = 1;
        moby->flags &= ~1;
    } else {
        moby->visible = 0;
        moby->flags |= 1;
    }
    if (vars->manip_set == 0)
        attach_manipulator(moby, 0, &vars->manip);
    vars->spin.f[0] = fast_add_rotations(vars->spin.f[0], frame_time * 1.5707964f);
    build_quaternion_from_axis_rotations(vars->spin_state, &vars->spin);
    switch (moby->state) {
    case SHUTTLE_STATE_INITIALIZE:
        clear_u64_value(&vars->spin);
        if (vars->path == -1) {
            qcopy(&vars->home_pos, &moby->pos);
            qcopy(&vars->home_rot, &moby->rot);
            moby->state = 4;
            vars->ridable = 1;
        } else {
            vars->unk100 = -1;
            vars->unkE0 = -1;
            select_shuttle_route_for_hero((char *)moby);
            moby->state = 2;
            moby->update_dist = 0xFF;
        }
        break;
    case SHUTTLE_STATE_WAIT_FOR_DEPARTURE:
        if (hero.ground_moby == moby) {
            if (l08_shuttle_departure_lock.busy == 0 && l08_shuttle_departure_lock.owner == -1) {
                qcopy(&hero_position_probe, &hero.motion.pos);
                resolve_point_path_segment_xy_clearance(
                    vars->stop, 0.5f, &hero_position_probe, &hero_position_probe);
                if (fast_vec_dist(&hero.motion.pos, &hero_position_probe) < 0.001f) {
                    try_set_shuttle_departure_help_message((char *)moby);
                    if ((D_0013CAE4[0] & 0x10) && hero.air_frames.s == 0 && D_L08_0015F594 == 10)
                        moby->state = 3;
                }
            }
        } else {
            select_shuttle_route_for_hero((char *)moby);
        }
        break;
    case SHUTTLE_STATE_TRAVEL:
        hero.jump_lock_timer = 5;
        hero.unk1F4 = 5;
        hero.path_constrain_pending = vars->stop;
        if (advance_shuttle_path_travel((char *)moby)) {
            select_shuttle_route_for_hero((char *)moby);
            moby->state = 2;
        }
        break;
    }
    update_shuttle_wobble(moby);
    if (vars->stop != -1)
        update_shuttle_stop_world_points((char *)moby);
    fast_vec_sub(&delta, &moby->pos, &old_pos);
    update_entity_matrix(vars->motion, &delta, &old_rot, &moby->rot);
}

void update_shuttle_stop_world_points(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int  *stop_point_table = (int *)l08_paths[*(int *)(data + 0x8C)];
    if (*stop_point_table == 8) {
        char *local_point = shuttle_stop_local_points;
        char *moby_rotation_basis = &moby->rot_row0;
        char *pos = &moby->pos;
        char *world_point = (char *)stop_point_table + 0x10;
        int   i;
        for (i = 7; i >= 0; i--) {
            transform_vector_by_basis(world_point, local_point, moby_rotation_basis);
            local_point += 0x10;
            fast_vec_add(world_point, world_point, pos);
            world_point += 0x10;
        }
    }
}

void select_shuttle_route_for_hero(struct Moby *moby) {
    enum {
        SHUTTLE_SELECT_ROUTE_END = 0,
        SHUTTLE_SELECT_CONFIGURED_PATH_START = 1,
        SHUTTLE_SELECT_ROUTE_START = 2,
        SHUTTLE_SELECT_ALTERNATE_PATH_START = 3
    };
    char  *data = (char *)moby->pvars;
    char  *pl = (char *)&hero.motion.pos;
    Route *route = ((Route *)l08_paths[*(int *)(data + 0x88)]);
    Route *configured_path = ((Route *)l08_paths[*(int *)(data + 0x80)]);
    Route *alternate_path = ((Route *)l08_paths[*(int *)(data + 0x84)]);
    float  best_candidate_distance = fast_vec_dist(pl, (char *)route + (route->count << 4));
    float  configured_path_start_distance = fast_vec_dist(pl, configured_path->e[0]);
    float  alternate_path_start_distance = fast_vec_dist(pl, alternate_path->e[0]);
    int    sel = 0;
    float  y;
    if (configured_path_start_distance < best_candidate_distance) {
        best_candidate_distance = configured_path_start_distance;
        sel = 1;
    }
    if (alternate_path_start_distance < best_candidate_distance)
        sel = 3;
    if (*(int *)(data + 0x100) == sel)
        return;
    *(int *)(data + 0x100) = sel;
    *(int *)(data + 0xE4) = 0;
    *(float *)(data + 0x104) = 1.0f;
    moby->visible = 1;
    moby->flags &= 0xFFFE;
    *(int *)(data + 0x124) = 1;
    moby->collision = moby->pclass->collision;
    switch (sel) {
    case SHUTTLE_SELECT_ROUTE_END: {
        int            route_point_count = route->count;
        unsigned char *visited_table;
        int            configured_path_entry_is_ff, alternate_path_entry_is_ff;
        int            fr;
        qcopy(data + 0x60, (char *)((route_point_count << 4) + (int)route));
        y = fast_arc_tan(*(float *)((char *)route + ((route_point_count - 2) << 4) + 0x10) -
                             *(float *)(data + 0x60),
                         *(float *)((char *)route + ((route_point_count - 2) << 4) + 0x14) -
                             *(float *)(data + 0x64));
        fr = current_level_index << 4;
        visited_table = visited52;
        *(float *)(data + 0x78) = y;
        *(float *)(data + 0xE4) = 1.0f;
        *(int *)(data + 0x104) = 0;
        configured_path_entry_is_ff = visited_table[*(int *)(data + 0x94) + fr] == 0xFF;
        alternate_path_entry_is_ff = visited_table[*(int *)(data + 0x98) + fr] == 0xFF;
        if (alternate_path_entry_is_ff) {
            *(Route **)(data + 0x120) = alternate_path;
        } else if (configured_path_entry_is_ff) {
            *(Route **)(data + 0x120) = configured_path;
        } else if (32.0f < vector_distance_xy(&moby->pos, pl) || D_L08_0015F5CC < 5) {
            moby->visible = 0;
            moby->flags |= 1;
            *(int *)(data + 0x124) = 0;
            moby->collision = 0;
        }
        return;
    }
    case SHUTTLE_SELECT_CONFIGURED_PATH_START:
        qcopy(data + 0x60, configured_path->e[0]);
        y = fast_arc_tan(configured_path->e[1][0] - *(float *)(data + 0x60),
                         configured_path->e[1][1] - *(float *)(data + 0x64));
        *(Route **)(data + 0x120) = configured_path;
        break;
    case SHUTTLE_SELECT_ROUTE_START:
        qcopy(data + 0x60, route->e[0]);
        y = fast_arc_tan(route->e[1][0] - *(float *)(data + 0x60),
                         route->e[1][1] - *(float *)(data + 0x64));
        *(Route **)(data + 0x120) = route;
        break;
    case SHUTTLE_SELECT_ALTERNATE_PATH_START:
        qcopy(data + 0x60, alternate_path->e[0]);
        y = fast_arc_tan(alternate_path->e[1][0] - *(float *)(data + 0x60),
                         alternate_path->e[1][1] - *(float *)(data + 0x64));
        *(Route **)(data + 0x120) = alternate_path;
        break;
    default:
        return;
    }
    *(float *)(data + 0x78) = y;
}

/* Bobs the shuttle up and down and rocks it about its home position and rotation. */
void update_shuttle_wobble(struct Moby *moby) {
    char *pvars = (char *)moby->pvars;
    float wobble_phase, wobble_value, wobble_phase_step;
    qcopy(&moby->pos, pvars + 0x60);
    qcopy(&moby->rot, pvars + 0x70);
    wobble_phase = fast_add_rotations(*(float *)(pvars + 0xec), frame_time * 2.0943951f);
    *(float *)(pvars + 0xec) = wobble_phase;
    wobble_value = fast_sin(wobble_phase);
    wobble_phase_step = frame_time * 1.6929693f;
    moby->pos.z += wobble_value * 0.25f;
    wobble_phase = fast_add_rotations(*(float *)(pvars + 0xf0), wobble_phase_step);
    *(float *)(pvars + 0xf0) = wobble_phase;
    wobble_value = fast_add_rotations(moby->rot.y, fast_sin(wobble_phase) * 0.08726646f);
    wobble_phase_step = frame_time * 0.9250245f;
    moby->rot.y = wobble_value;
    wobble_phase = fast_add_rotations(*(float *)(pvars + 0xf4), wobble_phase_step);
    *(float *)(pvars + 0xf4) = wobble_phase;
    moby->rot.x = fast_add_rotations(moby->rot.x, fast_sin(wobble_phase) * 0.08726646f);
    if (moby->rot.x > 0.17453292f)
        moby->rot.x = 0.17453292f;
    else if (moby->rot.x < -0.17453292f)
        moby->rot.x = -0.17453292f;
    if (moby->rot.y > 0.17453292f)
        moby->rot.y = 0.17453292f;
    else if (moby->rot.y < -0.17453292f)
        moby->rot.y = -0.17453292f;
}

int advance_shuttle_path_travel(struct Moby *moby) {
    char *shuttle_vars = (char *)moby->pvars;
    int  *path;
    float path_acceleration_step = D_0015ED70 * 0.1f;
    float path_maximum_speed = D_0015ED6C_lui * 0.08f;
    path = *(int **)(shuttle_vars + 0x120);
    advance_accelerated_scalar((float *)(shuttle_vars + 0xE4),
                               (float *)(shuttle_vars + 0xE8),
                               *(float *)(shuttle_vars + 0x104),
                               path_acceleration_step,
                               path_acceleration_step,
                               path_maximum_speed);
    *(float *)(shuttle_vars + 0x110) = fast_add_rotations(*(float *)(shuttle_vars + 0x110),
                                                          *(float *)(shuttle_vars + 0xE8) * 50.0f);
    sample_camera_path(path,
                       0,
                       shuttle_vars + 0x60,
                       (float *)(shuttle_vars + 0x70),
                       0,
                       *(float *)(shuttle_vars + 0xE4) * (float)(*path - 1));
    *(int *)(shuttle_vars + 0x74) = 0;
    if (absolute_float(*(float *)(shuttle_vars + 0xE4) - *(float *)(shuttle_vars + 0x104)) < 0.2f) {
        advance_accelerated_scalar((float *)(shuttle_vars + 0x108),
                                   (float *)(shuttle_vars + 0x10C),
                                   3.14159f,
                                   D_0015ED70 * 2.0943952f,
                                   D_0015ED70 * 2.0943952f,
                                   D_0015ED6C_lui * 6.2831855f);
    } else {
        *(int *)(shuttle_vars + 0x108) = 0;
    }
    if (*(float *)(shuttle_vars + 0x104) == 0.0f) {
        *(float *)(shuttle_vars + 0x78) =
            fast_add_rotations(*(float *)(shuttle_vars + 0x78), 3.14159f);
    }
    *(float *)(shuttle_vars + 0x78) =
        fast_add_rotations(*(float *)(shuttle_vars + 0x78), *(float *)(shuttle_vars + 0x108));
    return *(float *)(shuttle_vars + 0xE4) == *(float *)(shuttle_vars + 0x104);
}

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DDB68), where it is exact; names translated to the US level program. */
void try_set_shuttle_departure_help_message(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (*(float *)(data + 0x104) > 0.9f) {
        try_set_help_message(0xA, 0x1F4D);
    } else if (*(int *)(data + 0x120) == ((int)l08_paths[*(int *)(data + 0x80)])) {
        try_set_help_message(0xA, 0x1F50);
    } else if (*(int *)(data + 0x120) == ((int)l08_paths[*(int *)(data + 0x88)])) {
        try_set_help_message(0xA, 0x1F4F);
    } else {
        try_set_help_message(0xA, 0x1F4E);
    }
}
