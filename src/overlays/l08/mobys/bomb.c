/* Level 08 moby classes: bomb, bomber_tank, blarg_bombthrower, shuttle, rocket. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/overlay/shared/math/vectors/random_vector.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/game/fun_00213928.h"
#include "rnc/overlay/shared/props/trackdrive_left.h"
#include "rnc/overlay/shared/vehicles/veldin_ship.h"
#include "rnc/overlay/shared/math/vectors/rotation_2D5FA8.h"
#include "rnc/math/random.h"
#include "rnc/ui/help/try_set_help_message.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002B9438), where it is exact; names translated to the US level program. */

extern int  spawn_moby_offset_effect(char *) __asm__("FUN_L08_00279f00");
extern void copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");

#include "rnc/overlay/l08/mobys/bomb_globals.h"

extern float bolt_pickup_spin_increment_range_start_gp __asm__("D_L08_00161468")
    __attribute__((sda));
extern float bolt_pickup_spin_increment_range_end_gp __asm__("D_L08_0016146C") __attribute__((sda));
void         initialize_bolt_pickup(struct Moby *moby) __asm__("FUN_L08_002b8228");

void initialize_bolt_pickup(struct Moby *moby) {
    float random_range_start = bolt_pickup_spin_increment_range_start_gp;
    float random_range_end = bolt_pickup_spin_increment_range_end_gp;
    char *moby_vars;
    moby->rot.x = 0.0f;
    moby->rot.y = 0.5235988f;
    moby->rot.z = 0.0f;
    moby_vars = (char *)moby->pvars;
    *(float *)(moby_vars + 0xC) = random_float_between(random_range_start, random_range_end);
    *(int *)(moby_vars + 0x28) = 0;
    *(float *)(moby_vars + 8) = moby->pos.z;
    copy_hero_moby_field_38(moby);
    *(int *)(moby_vars + 0x24) = spawn_moby_offset_effect(moby);
}
/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */

#include "qcopy.h"

/* Level 8 moby update (class 152): state machine that steers the moby toward the hero and fires its event. */

typedef struct Moby Moby;

typedef struct Target {
    char  pad0[0xA];
    short fA;
} Target;

typedef struct MobyData {
    char    pad0[8];
    float   f8;
    float   fC;
    float   vel[4];
    int     f20;
    Target *target;
    int     ticks;
    int     f2C;
} MobyData;

typedef struct Moby_2B94B0 {
    char          pad0[0x10];
    float         pos[4];
    unsigned char state;
    char          pad21[0x27];
    float         f48;
    char          pad4C[0x2C];
    MobyData     *data;
} Moby_2B94B0;

typedef struct Hero_2B94B0 {
    char  pad0[0x80];
    float pos[4];
    char  pad90[0x480];
    float v510[4];
    float v520[4];
    float v530[4];
    char  pad540[0x1B4C];
    int   f208C;
    char  pad2090[0x1F8];
    float f2288;
    float f228C;
} Hero_2B94B0;

extern float D_0015ED70;
extern int   D_0015ED84;
extern char *D_L08_0015FFD8;

#include "rnc/overlay/l08/mobys/bomb_data/update_bolt_pickup.h"
#include "rnc/overlay/level_path.h"

extern float bolt_pickup_collision_stop_speed_threshold_gp __asm__("D_L08_00161458")
    __attribute__((sda));
extern float bolt_pickup_collision_speed_reduction_gp __asm__("D_L08_0016145C")
    __attribute__((sda));
extern int   bolt_pickup_bob_period_ticks_gp __asm__("D_L08_00161460") __attribute__((sda));
extern float bolt_pickup_bob_amplitude_gp __asm__("D_L08_00161464") __attribute__((sda));
extern int   D_L08_00174658;
extern int   D_L08_0015F5CC;
extern int   reflect_vector() __asm__("FUN_L00_001ff660");
extern int   FUN_L08_002025d0(void *, int, int, void *, int);
extern s32   queue_animation_update() __asm__("func_001FF308");
extern void  mark_moby_for_removal(void *) __asm__("func_0020C828");
float        absolute_float(float input) __asm__("func_001F99C0");
extern int   func_L00_001F39B0_2B94B0(void *, void *, int, int, int) __asm__("FUN_L08_002025d0");
extern void  func_L00_001FF610_2B94B0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int
func_001FFB38_2B94B0(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern float func_001F9FA8_2B94B0(float) __asm__("FUN_001f9de0");
extern void  initialize_bolt_counter_hud(void) __asm__("FUN_L00_00239cc8");
extern void  update_bolt_counter_hud_animation_and_visibility(void) __asm__("FUN_L00_00239d00");
extern void  draw_bolt_counter_hud_panel(void) __asm__("FUN_L00_00239df8");
extern char  D_L08_001746D8_2B94B0[] __asm__("D_L08_00174658");
extern int   D_L08_00174658_lui __asm__("D_L08_00174658") NOT_SDA;
extern char  D_L08_00174700_2B94B0[] __asm__("D_L08_00174680");
extern s32   level_bolts[] __asm__("D_0013DF38");

void update_bolt_pickup(Moby_2B94B0 *moby) __asm__("FUN_L08_002b82a0");

void update_bolt_pickup(Moby_2B94B0 *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_PHYSICS_MOVEMENT = 1,
        MOBY_STATE_CHECK_COLLECTION = 2,
        MOBY_STATE_VERTICAL_BOB_ONLY = 3,
        MOBY_STATE_REMOVE = 4
    };
    MobyData *moby_data = moby->data;
    float     A[4], F[4], B[4], C[4], P[4], E[4];
    float     f20, bob_phase_radians, adjusted_speed_or_bob_sine;
    int       hit, ok, bob_period_ticks;

    moby_data->ticks++;
    if (moby->state != 0)
        moby->pos[2] = moby_data->f8;
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        initialize_bolt_pickup(moby);
        if (moby_data->f2C != 0)
            moby->state = 1;
        else
            moby->state = 2;
        break;
    case MOBY_STATE_PHYSICS_MOVEMENT:
        qcopy(P, moby->pos);
        moby_data->vel[2] = moby_data->vel[2] - D_0015ED70 * 9.8f;
        f20 = fast_vec_length(moby_data->vel);
        if (bolt_pickup_collision_clearance_distance_gp * 1.8f * frame_scale < f20) {
            fast_vec_normalize(moby_data->vel,
                               moby_data->vel,
                               bolt_pickup_collision_clearance_distance_gp * 1.8f * frame_scale);
            f20 = bolt_pickup_collision_clearance_distance_gp * 1.8f * frame_scale;
        }
        fast_vec_add(A, P, moby_data->vel);
        fast_vec_normalize(B, moby_data->vel, bolt_pickup_collision_clearance_distance_gp);
        fast_vec_sub(B, P, B);
        fast_vec_normalize(C, moby_data->vel, f20 + bolt_pickup_collision_clearance_distance_gp);
        fast_vec_add(C, C, P);
        if (D_L08_0015F5CC % 2 == (((char *)moby - D_L08_0015FFD8) >> 8) % 2 &&
            func_L00_001F39B0_2B94B0(B, C, 2, 0, 0) != 0) {
            ok = 1;
            if (D_L08_00174658_lui == 0 || moby_data->ticks >= 10) {
                if (f20 < bolt_pickup_collision_stop_speed_threshold_gp) {
                    ok = 0;
                } else {
                    fast_vec_sub(E, P, (D_L08_001746D8_2B94B0 + 8));
                    fast_vec_normalize(E, E, bolt_pickup_collision_clearance_distance_gp);
                    fast_vec_add(P, (D_L08_001746D8_2B94B0 + 8), E);
                }
                if (ok != 0) {
                    func_L00_001FF610_2B94B0(F, moby_data->vel, D_L08_00174700_2B94B0);
                    adjusted_speed_or_bob_sine =
                        fast_vec_length(F) - bolt_pickup_collision_speed_reduction_gp;
                    fast_vec_normalize(
                        moby_data->vel,
                        F,
                        adjusted_speed_or_bob_sine < 0.0f ? 0.0f : adjusted_speed_or_bob_sine);
                } else {
                    clear_u64_value(moby_data->vel);
                    qcopy(P, (D_L08_001746D8_2B94B0 + 8));
                    P[2] = P[2] + bolt_pickup_collision_clearance_distance_gp;
                    if (*(int *)D_L08_001746D8_2B94B0 == 0) {
                        clear_u64_value(moby_data->vel);
                        moby->state = 2;
                    }
                }
            } else {
                qcopy(P, A);
            }
        } else {
            qcopy(P, A);
        }
        qcopy(moby->pos, P);
    case MOBY_STATE_CHECK_COLLECTION:
        hit = 0;
        moby->f48 = fast_add_rotations(moby->f48, moby_data->fC);
        if (((Hero_2B94B0 *)((char *)&hero))->f208C == 15) {
            if (vector_distance_xy(((Hero_2B94B0 *)((char *)&hero))->v510, moby->pos) < 0.5f)
                hit = 1;
            if (vector_distance_xy(((Hero_2B94B0 *)((char *)&hero))->v520, moby->pos) < 0.5f)
                hit = 1;
            if (vector_distance_xy(((Hero_2B94B0 *)((char *)&hero))->v530, moby->pos) < 0.5f)
                hit = 1;
        } else if (vector_distance_xy(moby->pos, ((Hero_2B94B0 *)((char *)&hero))->pos) <
                   ((Hero_2B94B0 *)((char *)&hero))->f2288) {
            if (absolute_float(moby->pos[2] - ((Hero_2B94B0 *)((char *)&hero))->pos[2]) <
                ((Hero_2B94B0 *)((char *)&hero))->f228C)
                hit = 1;
        }
        if (hit == 0)
            break;
        if (moby_data->target != 0)
            moby_data->target->fA = 0;
        current_bolt_count = current_bolt_count + moby_data->f20;
        ((int *)level_bolts)[D_0015ED84] = ((int *)level_bolts)[D_0015ED84] + moby_data->f20;
        allocate_voice_for_target_entry(0, 0, moby);
        func_001FFB38_2B94B0(2,
                             0x754E,
                             initialize_bolt_counter_hud,
                             update_bolt_counter_hud_animation_and_visibility,
                             draw_bolt_counter_hud_panel,
                             &current_bolt_count,
                             0x98967F);
        mark_moby_for_removal(moby);
        return;
    case MOBY_STATE_VERTICAL_BOB_ONLY:
        break;
    case MOBY_STATE_REMOVE:
        if (moby_data->target != 0)
            moby_data->target->fA = 0;
        mark_moby_for_removal(moby);
        return;
    }
    bob_period_ticks = bolt_pickup_bob_period_ticks_gp;
    bob_phase_radians =
        (float)(moby_data->ticks % bob_period_ticks) / (float)(bob_period_ticks - 1) * 6.2831855f -
        3.1415927f;
    adjusted_speed_or_bob_sine = func_001F9FA8_2B94B0(bob_phase_radians);
    moby_data->f8 = moby->pos[2];
    moby->pos[2] = moby->pos[2] + bolt_pickup_bob_amplitude_gp * adjusted_speed_or_bob_sine;
}

#include "qcopy.h"

/* Spawns a moby of class 0xF8 at a position and facing derived from a vector. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D35E8), where it is exact; names translated to the US level program. */

extern char *create_moby(int);
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");

char *spawn_bomb_with_velocity(int   source_moby,
                               char *spawn_position,
                               char *initial_velocity) __asm__("FUN_L08_002d22e0");

char *spawn_bomb_with_velocity(int source_moby, char *spawn_position, char *initial_velocity) {
    char *spawned_moby = create_moby(0xF8);
    if (spawned_moby != 0) {
        char *bomb_vars = ((char *)MOBY(spawned_moby)->pvars);
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        *(int *)(bomb_vars + 0x30) = source_moby;
        *(int *)(bomb_vars + 0x20) = scale_game_frames(300);
        *(float *)(bomb_vars + 0x2C) = 1.0f;
        *(int *)(bomb_vars + 0x24) = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(bomb_vars + 0x10, initial_velocity);
        MOBY(spawned_moby)->rot.x = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        MOBY(spawned_moby)->rot.z =
            fast_arc_tan(*(float *)initial_velocity, *(float *)(initial_velocity + 4)) + 1.5707964f;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
/* Bomb (class 248) update: fuse, bounce and blast state machine on the moby's state byte */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D36F8), where it is exact; names translated to the US level program. */

typedef int v128_36F8 __attribute__((mode(TI)));

typedef struct {
    char b[16];
} blk16;

struct Probe_36F8 {
    float v[4];
    void *moby;
    int   flags;
    int   pad;
    float f;
    int   st;
};

struct Ray_36F8 {
    float             start[4];
    float             end[4];
    struct Probe_36F8 x;
};

union Coll_36F8 {
    struct Ray_36F8   r;
    struct Probe_36F8 p;
};

typedef int OvlQuad_c __attribute__((mode(TI)));

typedef struct {
    u8 pad0[0x18];
    s32 moby; /* 0x18: passed to suck_get_aim_height (reads moby+0x34, moby+0x78); 0 when no moby was hit */
    s32 unk1C; /* 0x1C: > 0 gates copying point and the normal into the hero (shared/ui/help/00226f10.c) */
    union {
        OvlQuad_c q;
        f32       f[4];
    } point;        /* 0x20: f[2] is read as the ground height */
    u8 pad30[0x10]; /* 0x30: position after push-out (copied into hero.motion.pos) */
    f32 normal_x; /* 0x40: start of the surface normal; its angle is tested against 50 degrees (0.87266463) */
    f32 normal_y; /* 0x44: fast_arc_tan (atan2) of normal_x, normal_y is stored in the hero */
    f32 normal_z; /* 0x48: ledge probes compare atan2(normal_z, xy length) with 20 degrees */
} CollisionHit_2d23f0;

#include "rnc/overlay/collision.h"

extern CollisionHit     D_L08_00174640;
extern blk16            l08_bomb_moving_particle_initial_colors __asm__("D_L08_00161868");
extern float D_0015ED6C MACRO_ADDR;
extern float            D_0015ED70_gp __asm__("D_0015ED70") MACRO_ADDR;
extern float            D_L08_00174660[];
extern int              collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int              coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern float bomb_downward_acceleration_gp __asm__("D_L08_00161860") __attribute__((sda));
extern void  FUN_L00_001ff660_c(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void  jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
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
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void
FUN_L00_00269958_c(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern float D_L08_0015F580_macro[] __asm__("D_L08_0015F580") MACRO_ADDR;

void update_bomb(char *moby) __asm__("FUN_L08_002d23f0");

void update_bomb(char *moby) {
    enum {
        BOMB_STATE_MOVING = 1,
        BOMB_STATE_STATIONARY_FUSE = 2,
        BOMB_STATE_EXPLODE_AND_REMOVE = 0x62,
        BOMB_STATE_BURST_AND_REMOVE = 0x63
    };
    union Coll_36F8 c;
    blk16           table_or_bounce_scratch;
    float           normalized_direction[4];
    float           random_direction[4];
    char           *bomb_vars;
    int             st;
    int             two;
    float           collision_z_offset;

    bomb_vars = ((char *)MOBY(moby)->pvars);
    if (MOBY(moby)->state != 0x37) {
        if (coll_sphere(0.5f, moby + 0x10, 0x11, moby)) {
            int sphere_hit_moby = D_L08_00174658_lui;
            if (sphere_hit_moby != 0 && (sphere_hit_moby != *(int *)(bomb_vars + 0x30) ||
                                         *(int *)(bomb_vars + 0x24) != 0)) {
                if (!(MOBY(moby)->command & 2))
                    MOBY(moby)->state = 0x62;
                else
                    MOBY(moby)->state = 0x63;
                qcopy((moby + 0x10), ((char *)D_L08_00174660));
                FUN_L00_001ff660_c(
                    bomb_vars + 0x10, bomb_vars + 0x10, ((char *)D_L08_00174660) + 0x20);
                fast_vec_normalize(bomb_vars + 0x10, bomb_vars + 0x10, D_0015ED6C + D_0015ED6C);
            }
        }
    }

    st = MOBY(moby)->state;
    two = 2;
    switch (st) {
    case BOMB_STATE_MOVING: {
        float velocity_magnitude;
        velocity_magnitude = fast_vec_length((bomb_vars + 0x10));
        MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, velocity_magnitude * 0.5f);
        if (*(int *)(bomb_vars + 0x24) == 0) {
            float lo = -1.0f;
            float hi = 1.0f;
            int  *selected_table_entry;
            table_or_bounce_scratch = l08_bomb_moving_particle_initial_colors;
            *(v128_36F8 *)random_direction = 0;
            random_direction[0] = random_float_between(lo, hi);
            random_direction[1] = random_float_between(lo, hi);
            random_direction[2] = random_float_between(lo, hi);
            *(v128_36F8 *)normalized_direction = *(v128_36F8 *)random_direction;
            fast_vec_normalize(normalized_direction, normalized_direction, D_0015ED6C);
            selected_table_entry = (int *)table_or_bounce_scratch.b + random_integer_below(4);
            FUN_L00_00269958_c((moby + 0x10),
                               normalized_direction,
                               *selected_table_entry,
                               0x4FFF,
                               scale_game_frames(0x2D),
                               0x32,
                               0x78,
                               1);
        }
        qcopy(c.r.start, moby + 0x10);
        collision_z_offset = 0.16f;
        c.r.start[2] -= collision_z_offset;
        fast_vec_add((moby + 0x10), (moby + 0x10), (bomb_vars + 0x10));
        *(float *)(bomb_vars + 0x18) -= bomb_downward_acceleration_gp * D_0015ED70_gp;
        qcopy(c.r.end, moby + 0x10);
        c.r.end[2] -= collision_z_offset;
        c.r.x.moby = moby;
        c.r.x.flags = 0x810000;
        c.r.x.st = st;
        c.r.x.f = 1.0f;
        qcopy(c.r.x.v, bomb_vars + 0x10);
        if (collision_line(c.r.start, c.r.end, 0, moby, 0)) {
            char *collision_result = (char *)&D_L08_00174640;
            int   v = *(int *)(collision_result + 0x18);
            if (v != 0) {
                if (v != *(int *)(bomb_vars + 0x30) || *(int *)(bomb_vars + 0x24) != 0) {
                    if (!(MOBY(moby)->command & 2))
                        MOBY(moby)->state = 0x62;
                    else
                        MOBY(moby)->state = 0x63;
                    qcopy((moby + 0x10), ((char *)D_L08_00174660));
                    {
                        char *v = bomb_vars + 0x10;
                        FUN_L00_001ff660_c(v, v, ((char *)D_L08_00174660) + 0x20);
                        fast_vec_normalize(v, v, D_0015ED6C + D_0015ED6C);
                    }
                }
            } else if (*(int *)(collision_result + 0x1C) > 0) {
                if (MOBY(moby)->command & 1) {
                    float k;
                    allocate_voice_for_target_entry(1, 0, moby);
                    qcopy((moby + 0x10), collision_result + 0x20);
                    MOBY(moby)->pos.z += collision_z_offset;
                    FUN_L00_001ff660_c(
                        (bomb_vars + 0x10), (bomb_vars + 0x10), collision_result + 0x40);
                    fast_vec_cross(
                        &table_or_bounce_scratch, (bomb_vars + 0x10), collision_result + 0x40);
                    k = D_0015ED6C * 1.5f;
                    fast_vec_normalize(&table_or_bounce_scratch,
                                       &table_or_bounce_scratch,
                                       random_float_between(-k, k));
                    fast_vec_add((bomb_vars + 0x10), (bomb_vars + 0x10), &table_or_bounce_scratch);
                    fast_vec_scale((bomb_vars + 0x10), (bomb_vars + 0x10), 0.4f);
                    *(int *)(bomb_vars + 0x24) += 1;
                    *(int *)(bomb_vars + 0x20) = scale_game_frames(0xB4);
                    if (fast_vec_length((bomb_vars + 0x10)) < D_0015ED6C) {
                        int r;
                        MOBY(moby)->state = two;
                        r = scale_game_frames(0x78);
                        *(int *)(bomb_vars + 0x20) = r;
                        *(float *)(bomb_vars + 0x28) = 254.0f / (float)r;
                        clear_u64_value((bomb_vars + 0x10));
                    }
                } else {
                    MOBY(moby)->state = 0x62;
                    qcopy((moby + 0x10), collision_result + 0x20);
                    FUN_L00_001ff660_c(
                        (bomb_vars + 0x10), (bomb_vars + 0x10), collision_result + 0x40);
                    fast_vec_normalize(
                        (bomb_vars + 0x10), (bomb_vars + 0x10), D_0015ED6C + D_0015ED6C);
                }
            }
        } else if (fast_dec_timer(bomb_vars + 0x20)) {
            if (MOBY(moby)->command & 2) {
                MOBY(moby)->state = 0x63;
            } else {
                MOBY(moby)->state = 0x62;
            }
        }
        break;
    }
    case BOMB_STATE_STATIONARY_FUSE:
        if (fast_dec_timer(bomb_vars + 0x20)) {
            MOBY(moby)->state = 0x63;
        } else if (*(int *)(bomb_vars + 0x20) == scale_game_frames(0x3C) ||
                   *(int *)(bomb_vars + 0x20) == scale_game_frames(0x28) ||
                   *(int *)(bomb_vars + 0x20) == scale_game_frames(0x14)) {
            *(unsigned char *)(bomb_vars + 7) = 0xFA;
            trooper_anim_update(moby, (short *)bomb_vars);
        }
        trooper_anim_finish(moby, bomb_vars);
        break;
    case BOMB_STATE_EXPLODE_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, moby);
        c.p.moby = moby;
        c.p.flags = 0x810001;
        c.p.f = 1.0f;
        clear_u64_value(&c.p);
        c.p.st = 0;
        collect_mobies_in_sphere(moby + 0x10, 1.0f, 0x10, moby, &c.p);
        spawn_explosion(moby,
                        D_L08_0015F580_macro,
                        0,
                        0.0f,
                        0.0f,
                        7,
                        10,
                        20,
                        3.0f,
                        1.7f,
                        4.0f,
                        1.0f,
                        -1,
                        7.0f,
                        0,
                        7,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    case BOMB_STATE_BURST_AND_REMOVE: {
        char *p;
        allocate_voice_for_target_entry(0, 0, moby);
        p = moby + 0x10;
        c.p.moby = moby;
        c.p.flags = 0x810001;
        c.p.f = 1.0f;
        clear_u64_value(&c.p);
        c.p.st = 0;
        collect_mobies_in_sphere(p, 1.0f, 0x10, moby, &c.p);
        cart_burst(moby, p, -1, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    }
    jet_animate(moby, 0.2f);
}
/* Bomber tank (class 253) update: spawns its two parts, then a state machine for driving, firing and the group's debris. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D55E0), where it is exact; names translated to the US level program. */

extern char *
spawn_bomber_tank_projectile(char *src, char *pos, char *vec) __asm__("FUN_L08_002dc8a0");
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
            shot = spawn_bomber_tank_projectile((char *)moby, (char *)muzzle, (char *)velocity);
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
extern float         D_0015ED6C;
extern int           FUN_L00_001fff28();
extern void         *FUN_L00_0025a420(void *, int, int);
extern void          FUN_L00_00263d40(int, int);
int                  FUN_L00_0025ff38(float best,
                                      char *a,
                                      char *o,
                                      int  *excluded,
                                      int   excluded_count,
                                      void *region,
                                      int   region_count);
void                *FUN_L00_00263fd8(char  *src,
                                      int    cls,
                                      float *pos,
                                      void  *mat,
                                      int    a8,
                                      int    a9,
                                      float *v10,
                                      float *v11,
                                      float  scale,
                                      float *v12);
void                 FUN_L00_00257470(unsigned char *m, int flags, int arg);
extern float         D_0015ED6C_lui __asm__("D_0015ED6C");
extern int           D_L08_0015F5B0 __asm__("D_L08_0015F5B0");
extern int           D_L08_0015F5B4_gp __asm__("D_L08_0015F5B4") __attribute__((sda));
extern int           D_L08_0015F5BC_gp __asm__("D_L08_0015F5BC") __attribute__((sda));
extern unsigned char D_0013D50F_2D5FA8[] __asm__("D_0013D408");
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
                            unsigned char *flags = D_0013D50F_2D5FA8;
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
extern void  approach_value_l08(float *, float, float) __asm__("FUN_00213ed8");
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
    approach_value_l08(&vars->speed, vars->target_speed, frame_time_sq + frame_time_sq);
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

#include "rnc/overlay/l08/mobys/bomb_data/spawn_bomber_tank_joint_particles_2.h"

extern float bomber_tank_joint_particle_jitter_magnitude_base_gp __asm__("D_L08_001618DC")
    __attribute__((sda));
extern float bomber_tank_joint_particle_vertical_spread_gp __asm__("D_L08_001618E0")
    __attribute__((sda));
extern float bomber_tank_joint_particle_horizontal_spread_range_end_gp __asm__("D_L08_001618E8")
    __attribute__((sda));

#include "rnc/overlay/l08/mobys/bomb_data/spawn_bomber_tank_joint_particles.h"

extern int   FUN_L00_0024f7c8();
extern void  build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern float func_00214158_2D6C58(void) __asm__("FUN_00213308");
extern float func_002140F8_2D6C58(float, float) __asm__("FUN_002132a8");
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

        random_angle = func_00214158_2D6C58();
        qcopy(spawn_ptr, joint_pos);
        spawn_pos[0] += func_002140F8_2D6C58(-0.05f, 0.05f);
        spawn_pos[1] += func_002140F8_2D6C58(-0.05f, 0.05f);
        spawn_pos[2] += func_002140F8_2D6C58(-0.05f, 0.05f);
        qcopy(velocity, data + 0x40);
        qcopy(spread_vector, data + 0x40);
        if (count / 3 < i) {
            func_L00_001FF4B0_2D6C58(
                jitter,
                jitter_direction_source,
                func_002140F8_2D6C58(bomber_tank_joint_particle_jitter_magnitude_base_gp,
                                     bomber_tank_joint_particle_jitter_magnitude_base_gp * 1.2f) *
                    D_0015ED6C);
            fast_vec_add(velocity, velocity, jitter);
            fast_vec_scale(spread_vector, spread_vector, 0.5f);
            offset = func_001F9F90_2D6C58(random_angle);
            offset =
                offset * (func_002140F8_2D6C58(
                              0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp) *
                          D_0015ED6C);
            spread_vector[0] = spread_vector[0] + offset;
            offset = func_001F9FA8_2D6C58(random_angle);
            offset =
                offset * (func_002140F8_2D6C58(
                              0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp) *
                          D_0015ED6C);
            spread_vector[1] = spread_vector[1] + offset;
            spread_vector[2] = bomber_tank_joint_particle_vertical_spread_gp * D_0015ED6C;
        } else {
            func_L00_001FF4B0_2D6C58(
                jitter,
                jitter_direction_source,
                func_002140F8_2D6C58(0.0f, bomber_tank_joint_particle_jitter_magnitude_base_gp) *
                    D_0015ED6C);
            fast_vec_add(velocity, velocity, jitter);
            fast_vec_scale(spread_vector, spread_vector, 0.5f);
            offset = func_001F9F90_2D6C58(random_angle);
            offset = offset *
                     (func_002140F8_2D6C58(
                          0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp * 0.25f) *
                      D_0015ED6C);
            spread_vector[0] = spread_vector[0] + offset;
            offset = func_001F9FA8_2D6C58(random_angle);
            offset = offset *
                     (func_002140F8_2D6C58(
                          0.0f, bomber_tank_joint_particle_horizontal_spread_range_end_gp * 0.25f) *
                      D_0015ED6C);
            spread_vector[1] = spread_vector[1] + offset;
            spread_vector[2] = bomber_tank_joint_particle_vertical_spread_gp * D_0015ED6C;
        }
        velocity[3] = bomber_tank_joint_particle_velocity_w_gp;
        spread_vector[3] = bomber_tank_joint_particle_spread_vector_w_gp;
        frames1 = truncate_float_to_s32(scale_time(func_002140F8_2D6C58(
            (float)bomber_tank_joint_particle_duration_frames_range_start_gp,
            (float)bomber_tank_joint_particle_duration_frames_range_start_gp * 1.2f)));
        frames2 = truncate_float_to_s32(scale_time(func_002140F8_2D6C58(
            (float)bomber_tank_joint_particle_second_frame_count_range_start_gp,
            (float)bomber_tank_joint_particle_second_frame_count_range_start_gp * 1.2f)));
        frames3 = truncate_float_to_s32(scale_time(func_002140F8_2D6C58(
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
            MOBY(spawned_debris_moby)->rot.x = func_00214158_2D6C58();
            MOBY(spawned_debris_moby)->rot.y = func_00214158_2D6C58();
            MOBY(spawned_debris_moby)->rot.z = func_00214158_2D6C58();
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

/* Splashes the hero into the water: once inits the water plane, queues the
 * surface draw, and when the hero breaks the surface spawns spray and ripples. */

extern int   water_entry_plane_initialized_gp __asm__("D_L08_00161970") __attribute__((sda));
extern float water_entry_plane_data[4] __asm__("D_L08_001D7060");
extern f32   D_L08_0015F580[] MACRO_ADDR;
extern void  FUN_L08_002da0d0(void *);
extern int   FUN_L00_00257b90(int, int);
extern void  spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern unsigned char *
spawn_splash(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");

void update_hero_water_entry_effects(struct Moby *moby) __asm__("FUN_L08_002da0f0");

void update_hero_water_entry_effects(struct Moby *moby) {
    Vec4         pos;
    Vec4         spray_velocity_or_splash_position;
    struct Moby *water_entry_moby;
    int          i;
    int          spray_random_value;
    float        sc, sp;

    if (water_entry_plane_initialized_gp == 0) {
        water_entry_plane_initialized_gp = 1;
        prepare_scaled_table_with_byte_128(water_entry_plane_data, 0.6666667f);
    }
    add_draw_callback(FUN_L08_002da0d0, moby);
    if (current_level_index != 8)
        return;
    if (!(hero.motion.pos.f[2] < water_entry_plane_data[2]))
        return;
    if (!(water_entry_plane_data[2] <= hero.motion.pos.f[2] - hero.motion.displacement.f[2]))
        return;
    if (voice_is_playing(hero.moby, hero.health.unk22B2)) {
        release_voice_slot(hero.health.unk22B2);
        hero.health.unk22B2 = -1;
    }
    qcopy(&pos, &hero.motion.pos);
    pos.f[2] = water_entry_plane_data[2];
    water_entry_moby = spawn_burst_moby(&pos, 3.0f);
    if (water_entry_moby)
        water_entry_moby->alpha = 0x70;
    for (i = 0; i < 16; i++) {
        float spray_heading_radians = random_angle_radians();
        float k = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
        spray_velocity_or_splash_position.f[0] = fast_cos(spray_heading_radians) * k;
        spray_velocity_or_splash_position.f[1] = fast_sin(spray_heading_radians) * k;
        spray_velocity_or_splash_position.f[2] =
            random_float_between(frame_time * 3.0f, frame_time * 6.5f);
        spray_random_value = FUN_L00_00257b90(0x5A, 0x78);
        spawn_debris_chunk(
            &pos, &spray_velocity_or_splash_position, random_integer_below(2), spray_random_value);
    }
    for (i = 0; i < 16; i++) {
        unsigned char *splash;
        random_vector(spray_velocity_or_splash_position.f, 1.0f, 1.0f);
        fast_vec_add(&spray_velocity_or_splash_position, &spray_velocity_or_splash_position, &pos);
        spray_velocity_or_splash_position.f[2] = water_entry_plane_data[2] + 0.05f;
        sc = random_float_between(0.7f, 1.0f);
        sp = i == 0 ? 2.0f : -2.0f;
        splash = spawn_splash(
            &spray_velocity_or_splash_position, D_L08_0015F580, &water_entry_plane_data[2], sc, sp);
        if (splash)
            *(short *)(splash + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(30.0f, 60.0f)));
    }
}

extern float line_puff_initial_z_velocity_range_end_gp __asm__("D_L08_00161998")
    __attribute__((sda));
extern float line_puff_position_jitter_range_end_gp __asm__("D_L08_001619A0") __attribute__((sda));
extern u16   line_puff_duration_frames_gp __asm__("D_L08_001619A8") __attribute__((sda));
extern unsigned char *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern char *spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");

/* Strings count glowing puffs from one point to another, growing from size0
 * to size1, then bursts a spark shower at each end. */
void spawn_particle_line_effect(struct Moby *moby,
                                void        *from,
                                void        *to,
                                int          count,
                                f32          size0,
                                f32          size1) __asm__("FUN_L08_002da3f0");

void spawn_particle_line_effect(

    struct Moby *moby, void *from, void *to, int count, f32 size0, f32 size1) {
    Vec4           step;
    Vec4           puff_spawn_position;
    Vec4           effect_initial_velocity;
    f32            grow;
    f32            size;
    unsigned char *puff;
    unsigned char *tail;
    int            alpha, grey;
    int            i;

    fast_vec_sub(&step, to, from);
    fast_vec_scale(&step, &step, 1.0f / (f32)count);
    grow = (size1 - size0) / (f32)count;
    effect_initial_velocity.q = 0;
    effect_initial_velocity.f[3] = 1.0f;
    effect_initial_velocity.f[2] = 0.01f;
    for (i = 0; i < count; i++) {
        fast_vec_scale(&puff_spawn_position, &step, convert_integer_to_float(i));
        fast_vec_add(&puff_spawn_position, from, &puff_spawn_position);
        effect_initial_velocity.f[0] = randf_sym(0.0f, 0.005f);
        effect_initial_velocity.f[1] = randf_sym(0.0f, 0.005f);
        effect_initial_velocity.f[2] =
            random_float_between(line_puff_initial_z_velocity_range_end_gp * 0.1f,
                                 line_puff_initial_z_velocity_range_end_gp);
        alpha = FUN_L00_00257b90(0x30, 0x70);
        grey = FUN_L00_00257b90(0x30, 0x7F);
        grey |= (grey << 16) | (grey << 8);
        size = random_float_between(1.0f, 1.02f);
        puff = spawn_puff(&puff_spawn_position,
                          line_puff_position_jitter_range_end_gp,
                          1.0f,
                          size,
                          FUN_L00_00257b90(-2, 2),
                          &effect_initial_velocity,
                          grow + size0 * 210000.0f,
                          (alpha << 24) | grey);
        if (puff) {
            tail = puff + 0x20;
            if (random_integer_below(2)) {
                puff[3] = 0x44;
                grey = FUN_L00_00257b90(0x60, 0xE0);
                grey |= (grey << 16) | (grey << 8);
                *(u32 *)(puff + 4) = (alpha << 24) | grey;
            }
            *(u16 *)(puff + 0xA) = line_puff_duration_frames_gp;
            *(s32 *)(tail + 4) = 2;
            tail[0xA] = alpha;
            tail[0xB] = line_puff_duration_frames_gp;
        }
    }
    effect_initial_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[2] = 0.0f;
    effect_initial_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(from,
                &effect_initial_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                1,
                10000.0f);
    effect_initial_velocity.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    effect_initial_velocity.f[2] = 0.0f;
    effect_initial_velocity.f[2] = random_float_between(0.01f, 0.03f);
    spawn_burst(to,
                &effect_initial_velocity,
                0x4F007FFF,
                0x1FFFFFFF,
                FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                1,
                20000.0f);
}
/* Picks the best candidate moby from the moby's list by heading and distance.
 * Exact since tools/ps2eeas_nops.py puts the hazard nop after the shared bc1fl's label. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DBD88), where it is exact; names translated to the US level program. */

char *select_blarg_bombthrower_waypoint(struct Moby *moby) __asm__("FUN_L08_002daa10");

char *select_blarg_bombthrower_waypoint(struct Moby *moby) {
    short       *group_entry = D_L08_001AC040[moby->group];
    char        *best;
    char        *candidate_moby;
    float        bestD, bestA;
    int          candidate_moby_index;
    struct Hero *base;
    if (group_entry == 0)
        return 0;
    best = 0;
    candidate_moby_index = *(unsigned short *)group_entry & 0x7FFF;
    candidate_moby = D_L08_0015FFD8 + (candidate_moby_index << 8);
    bestA = 0.0f;
    bestD = bestA;
    for (;;) {
        if (MOBY(candidate_moby)->oclass != 0x14D && MOBY(candidate_moby)->command == 0) {
            float candidate_hero_heading_difference, candidate_distance, candidate_heading;
            candidate_heading = fast_arc_tan(MOBY(candidate_moby)->pos.x - moby->pos.x,
                                             MOBY(candidate_moby)->pos.y - moby->pos.y);
            base = &hero;
            candidate_hero_heading_difference = fast_difference_between_rotations(
                candidate_heading,
                fast_arc_tan(base->motion.pos.f[0] - moby->pos.x,
                             base->motion.pos.f[1] - moby->pos.y));
            candidate_distance = fast_vec_dist(&moby->pos, candidate_moby + 0x10);
            if (candidate_hero_heading_difference < 1.5707964f) {
                if (bestA < candidate_hero_heading_difference) {
                    best = candidate_moby;
                    bestD = candidate_distance;
                    bestA = candidate_hero_heading_difference;
                }
            } else if ((candidate_distance >= 16.0f &&
                        (candidate_distance < bestD || bestD < 16.0f)) ||
                       (candidate_distance < 16.0f && bestD < candidate_distance)) {
                best = candidate_moby;
                bestD = candidate_distance;
                bestA = candidate_hero_heading_difference;
            }
        }
        if (*group_entry < -1)
            return best;
        group_entry++;
        candidate_moby_index = *(unsigned short *)group_entry & 0x7FFF;
        candidate_moby = D_L08_0015FFD8 + (candidate_moby_index << 8);
    }
}
/* Update function of the blarg bomb thrower: hit reactions, then a state machine that walks to its bomb crate, picks up and throws bombs at the hero. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DBF68), where it is exact; names translated to the US level program. */

typedef struct {
    char         pad0[0x20];
    struct Moby *moby;
} Hit_2D5FA8_c;

typedef struct {
    char         pad0[0x40];
    struct Moby *target;
    int          result;
} Look;

typedef struct {
    char          pad0[0x20];
    float         health;
    char          pad24[5];
    unsigned char b29;
    char          pad2A[0xE];
    int           i38;
    char          pad3C[4];
    float         v40[4];
    char          pad50[8];
    unsigned char b58;
    char          pad59;
    unsigned char b5A;
    char          pad5B[5];
    short         anim[3];
    char          pad66;
    unsigned char tint;
    char          pad68[8];
    char          blk70[0x10];
    float         f80, f84, f88, f8C;
    int           pad90;
    int           i94;
    char          pad98[0x15];
    unsigned char bAD;
    char          padAE[0x12];
    float         fC0, fC4;
    char          padC8[8];
    char          blkD0[8];
    float         fD8, fDC;
    char          padE0[0x14];
    float         fF4;
    char          padF8[0x14];
    float         f10C, f110, f114;
    char          pad118[8];
    struct Moby  *waypoint;
    struct Moby  *p124;
    int           i128;
    char          blk12C[4];
    short         s130, s132;
    int           i134;
    char          pad138[0x28];
    char          blk160[0x10];
} Data_DBF68;

char        *spawn_bomb_with_velocity(int arg, char *pos, char *vec) __asm__("FUN_L08_002d22e0");
char        *FUN_L08_002daa10_c(struct Moby *moby) __asm__("FUN_L08_002daa10");
extern float D_L08_00161978_gp __asm__("D_L08_00161978") __attribute__((sda));
extern float l08_bombthrower_throw_timing_speed_gp __asm__("D_L08_0016197C") __attribute__((sda));
extern float l08_bombthrower_aim_shortening_rate_gp __asm__("D_L08_00161988") __attribute__((sda));
extern float l08_bombthrower_death_horizontal_knockback_speed_gp __asm__("D_L08_0016198C")
    __attribute__((sda));
extern float l08_bombthrower_death_vertical_knockback_speed_gp __asm__("D_L08_00161990")
    __attribute__((sda));
extern float D_L08_001675C0[];
extern float FUN_L04_001f1978(float);
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern float fast_vec_dist(void *, void *);
extern int   D_L08_001619AC_gp __asm__("D_L08_001619AC") __attribute__((sda));
extern int   D_L08_001619D0_gp __asm__("D_L08_001619D0") __attribute__((sda));
extern int   FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern int
select_nearest_xy_moby_with_hero_fallback(void *, float, void *) __asm__("FUN_L08_0026a3d0");
extern void FUN_L00_00258278(void *, void *, float, float, float, float);
extern void FUN_L00_0025a120(void *);
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void  FUN_L00_0025f730(void *, float);
extern void  FUN_L01_0026d930(char *p);
extern void  spawn_moby_table_puff_pattern(void *) __asm__("FUN_L04_002c46d0");
void         FUN_L00_00263ac8(float x, int a, int b, unsigned char *p);
extern void  FUN_00212f90(struct Moby *, int, int, int);
extern void  explode_2D5FA8_c(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void  func_L00_00264B40_x(struct Moby *, int, float, char *) __asm__("FUN_L00_00263ac8");
extern float FUN_0020c9e0(struct Moby *);
extern void
func_L00_002592B0_x(float, void *, void *, float, float, float) __asm__("FUN_L00_00258278");

void blarg_bombthrower_update(struct Moby *moby) __asm__("FUN_L08_002dabf0");

void blarg_bombthrower_update(struct Moby *moby) {
    enum {
        BOMBTHROWER_STATE_INITIALIZE = 0,
        BOMBTHROWER_STATE_APPROACH_WAYPOINT = 1,
        BOMBTHROWER_STATE_WAIT_TO_THROW = 3,
        BOMBTHROWER_STATE_MOVE_TO_THROW_POSITION = 4,
        BOMBTHROWER_STATE_THROW_BOMB = 5,
        BOMBTHROWER_STATE_MOVE_ONTO_WAYPOINT = 6,
        BOMBTHROWER_STATE_DAMAGE_KNOCKBACK = 0x5A,
        BOMBTHROWER_STATE_WAIT_FOR_FALL_RECOVERY = 0x5B,
        BOMBTHROWER_STATE_MOTION_RECOVERY = 0x5C,
        BOMBTHROWER_STATE_FALL_RECOVERY = 0x5D,
        BOMBTHROWER_STATE_DYING = 0x63
    };
    Data_DBF68  *vars = (Data_DBF68 *)moby->pvars;
    Look         search;
    float        point[4];
    float        velocity[4];
    float        aim[4];
    int          hit;
    float        damage;
    float        scratch;
    struct Moby *target;
    struct Moby *old_waypoint;

    damage = 0.0f;
    func_L00_00264B40_x(moby, D_L08_001619AC_gp, 3.0f, vars->blk160);
    if (moby->visible != 0) {
        if (fast_vec_dist(&moby->pos, D_L08_001675C0) < 29.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x17;
        }
    }
    if (moby->state == 3) {
        moby->flags &= 0xFBFF;
    } else {
        moby->flags |= 0x400;
    }
    if (moby->state == 100) {
        float duration = convert_integer_to_float(D_L08_001619D0_gp);
        int   start_alpha = vars->s132;
        moby->alpha = truncate_float_to_s32(
            (float)start_alpha +
            (float)-start_alpha * ((float)(D_L08_001619D0_gp - vars->s130) / duration));
        spawn_moby_table_puff_pattern(moby);
        return;
    }
    fun_00213928(moby, find_moby_hit(moby, 0x330000, 0), &vars->health, 0, &hit, &damage, 0, 4);
    if (hit != 1 && moby->state != 0x63) {
        moby->flags |= 0x1000;
        vars->health -= damage;
        if (0.0f < vars->health) {
            if (damage < 1.0f) {
                vars->tint = 100;
            } else {
                char *hero_bytes;
                vars->f80 = frame_time_sq * 30.0f;
                vars->f88 = frame_time * 5.0f;
                vars->f8C = frame_time * 7.0f;
                vars->i94 = 9;
                vars->f84 = 0.0005f;
                vars->bAD = 0;
                hero_bytes = (char *)&hero;
                trooper_start_knockback(moby,
                                        vars->blk70,
                                        fast_arc_tan(moby->pos.x - *(float *)(hero_bytes + 0x80),
                                                     moby->pos.y - *(float *)(hero_bytes + 0x84)),
                                        6,
                                        1,
                                        0);
                vars->fC0 = 6.0f;
                vars->fC4 = 10.0f;
                vars->tint = 0x78;
                moby->state = 0x5A;
            }
        } else {
            char *hero_bytes;
            vars->i94 = 9;
            vars->f84 = 0.0005f;
            vars->f80 = frame_time_sq * 30.0f;
            vars->f88 = l08_bombthrower_death_horizontal_knockback_speed_gp * frame_time;
            vars->f8C = l08_bombthrower_death_vertical_knockback_speed_gp * frame_time;
            vars->bAD = 0;
            hero_bytes = (char *)&hero;
            trooper_start_knockback(moby,
                                    vars->blk70,
                                    fast_arc_tan(moby->pos.x - *(float *)(hero_bytes + 0x80),
                                                 moby->pos.y - *(float *)(hero_bytes + 0x84)),
                                    9,
                                    1,
                                    0);
            vars->fC0 = 6.0f;
            vars->fC4 = 10.0f;
            explode_2D5FA8_c(moby, 0, -1);
            vars->tint = 0xFA;
            moby->state = 0x63;
            moby->flags &= 0xEFFF;
            vars->waypoint->command = 0;
        }
        trooper_anim_update(moby, vars->anim);
    }
    moby->hit_slot = 0xFF;
    search.result = select_nearest_xy_moby_with_hero_fallback(moby, 16.0f, &search);
    if (search.target == 0) {
        search.target = *(struct Moby **)&D_001413D0;
    }
    target = search.target;
    switch (moby->state) {
    case BOMBTHROWER_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        vars->b29 = 1;
        vars->health = 2.0f;
        FUN_L01_0026d930(vars->blkD0);
        vars->fDC = 2.0f;
        vars->fD8 = 2.0f;
        vars->f10C = 0.03f;
        vars->f110 = 0.3f;
        vars->f114 = 0.25f;
        vars->fF4 = D_L08_00161978_gp * frame_time;
        vars->b58 = truncate_float_to_s32(17.0f);
        vars->b5A = truncate_float_to_s32(10.4f);
        clear_u64_value(vars->v40);
        vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby);
        vars->waypoint->command = 1;
        vars->i128 = scale_game_frames(0xB4);
        if (moby->prev_seq != 4) {
            FUN_00212f90(moby, 4, 0, 0);
        }
        moby->state = 1;
        vars->i134 = scale_game_frames(0xF0);
        break;
    case BOMBTHROWER_STATE_APPROACH_WAYPOINT:
        FUN_L00_00259028(
            (char *)moby, vars->blkD0, (float *)&vars->waypoint->pos, (char *)vars->v40);
        if (fast_vec_dist(&moby->pos, &vars->waypoint->pos) < 3.0f) {
            if (fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(vars->waypoint->pos.x - moby->pos.x,
                                 vars->waypoint->pos.y - moby->pos.y)) < 0.2617994f) {
                if (6.0f < vector_distance_xy(&moby->pos, &target->pos)) {
                    if (moby->prev_seq != 5) {
                        FUN_00212f90(moby, 5, 0, 10);
                    }
                    moby->state = 6;
                    break;
                }
                vars->i134 = scale_game_frames(0xF0);
                {
                    struct Moby *old_waypoint = vars->waypoint;
                    struct Moby *next_waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby);
                    vars->waypoint = next_waypoint;
                    if (next_waypoint == 0) {
                        vars->waypoint = old_waypoint;
                    }
                    old_waypoint->command = 0;
                    vars->waypoint->command = 1;
                }
                break;
            }
        }
        if (fast_dec_timer(&vars->i134) != 0) {
            old_waypoint = vars->waypoint;
            if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                vars->waypoint = old_waypoint;
            }
            old_waypoint->command = 0;
            vars->waypoint->command = 1;
            vars->i134 = scale_game_frames(0xF0);
        }
        break;
    case BOMBTHROWER_STATE_MOVE_ONTO_WAYPOINT:
        FUN_L00_00259028(
            (char *)moby, vars->blkD0, (float *)&vars->waypoint->pos, (char *)vars->v40);
        if (moby->anim_flags & 2) {
            FUN_00212f90(vars->waypoint, 1, 0, scale_game_frames(6));
            qcopy(&moby->pos, &vars->waypoint->pos);
            moby->pos.z -= 0.1f;
            if (moby->prev_seq != 1) {
                FUN_00212f90(moby, 1, 0, 10);
            }
            moby->state = 3;
            moby->flags &= 0xEFFF;
            moby->rot.z = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            vars->i128 = scale_game_frames(0x78);
            moby->collision = 0;
        }
        break;
    case BOMBTHROWER_STATE_WAIT_TO_THROW: {
        float target_distance_xy = vector_distance_xy(&moby->pos, &target->pos);
        func_L00_002592B0_x(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                            moby,
                            vars->blk12C,
                            0.02f,
                            0.3f,
                            0.2f);
        if (target_distance_xy < 20.0f && 8.0f < target_distance_xy &&
            fast_dec_timer(&vars->i128) != 0) {
            LevelCamera *cam = &l08_camera;
            if (fast_difference_between_rotations(
                    cam->yaw,
                    fast_arc_tan(moby->pos.x - cam->pos.f[0], moby->pos.y - cam->pos.f[1])) <
                0.55850536f) {
                goto attack;
            }
        }
        if (vars->i38 == 0 || !(0.5f < target_distance_xy)) {
            break;
        }
    attack:
        FUN_00212f90(vars->waypoint, 1, 0, scale_game_frames(6));
        if (moby->prev_seq != 2) {
            FUN_00212f90(moby, 2, 0, scale_game_frames(7));
        }
        moby->state = 4;
        moby->flags |= 0x1000;
        moby->collision = moby->pclass->collision;
        break;
    }
    case BOMBTHROWER_STATE_MOVE_TO_THROW_POSITION:
        point[0] = fast_cos(moby->rot.z) * 2.0f;
        point[1] = fast_sin(moby->rot.z) * 2.0f;
        point[2] = 0.0f;
        fast_vec_add(point, point, &vars->waypoint->pos);
        if (0.2f < vector_distance_xy(&moby->pos, point)) {
            FUN_L00_00259028((char *)moby, vars->blkD0, point, (char *)vars->v40);
        }
        if (moby->anim_flags & 2) {
            LevelCamera *cam;
            if (vector_distance_xy(&moby->pos, &target->pos) < 6.0f ||
                (cam = &l08_camera,
                 0.55850536f <
                     fast_difference_between_rotations(
                         cam->yaw,
                         fast_arc_tan(moby->pos.x - cam->pos.f[0], moby->pos.y - cam->pos.f[1])))) {
                old_waypoint = vars->waypoint;
                if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                    vars->waypoint = old_waypoint;
                }
                old_waypoint->command = 0;
                vars->waypoint->command = 1;
                if (moby->prev_seq != 4) {
                    FUN_00212f90(moby, 4, 0, scale_game_frames(7));
                }
                moby->state = 1;
                vars->i134 = scale_game_frames(0xF0);
            } else {
                if (moby->prev_seq != 3) {
                    FUN_00212f90(moby, 3, 0, scale_game_frames(6));
                }
                moby->state = 5;
            }
        }
        break;
    case BOMBTHROWER_STATE_THROW_BOMB:
        func_L00_002592B0_x(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                            moby,
                            vars->blk12C,
                            0.02f,
                            0.3f,
                            0.2f);
        if (moby->seq == moby->prev_seq && FUN_0020c9e0(moby) == 19.0f) {
            float distance, time;
            char *next;
            fast_vec_sub(aim, &target->pos, &moby->pos);
            aim[2] = 0.0f;
            distance = fast_vec_length(aim);
            time = distance / FUN_L04_001f1978(l08_bombthrower_throw_timing_speed_gp);
            distance = distance - (l08_bombthrower_aim_shortening_rate_gp * time + 2.0f);
            if (10.0f < distance) {
                distance = 10.0f;
            } else if (distance < 3.0f) {
                distance = 3.0f;
            }
            FUN_L00_0025f730(aim, distance);
            fast_vec_add(aim, aim, &moby->pos);
            moby_point_position(moby, 9, point);
            fast_vec_sub(velocity, &target->pos, point);
            set_vector_length_xy(velocity, velocity, time);
            velocity[2] = FUN_L00_0025abf0(point, aim, &scratch, time, -(frame_time_sq * 15.0f));
            next = spawn_bomb_with_velocity((int)moby, (char *)point, (char *)velocity);
            vars->p124 = (struct Moby *)next;
            if (next != 0) {
                char *next_vars = *(char **)(next + 0x78);
                *(int *)(next_vars + 0x20) = scale_game_frames(0xB4);
                *(short *)(next_vars + 0xC) = scale_game_frames(10);
                *(short *)(next_vars + 0xE) = scale_game_frames(10);
                vars->p124->state = 1;
                vars->p124->command |= 3;
                vars->i128 = scale_game_frames(0xB4);
            }
            break;
        }
        if (moby->anim_flags & 2) {
            moby->state = 1;
            if (moby->prev_seq != 4) {
                FUN_00212f90(moby, 4, 0, 3);
            }
            vars->i134 = scale_game_frames(0xF0);
        }
        break;
    case BOMBTHROWER_STATE_WAIT_FOR_FALL_RECOVERY:
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 7) {
                FUN_00212f90(moby, 7, 0, 3);
            }
            moby->state = 0x5D;
        }
        break;
    case BOMBTHROWER_STATE_DAMAGE_KNOCKBACK:
    case BOMBTHROWER_STATE_MOTION_RECOVERY:
    case BOMBTHROWER_STATE_FALL_RECOVERY:
        if (mine_fall(moby, vars->blk70) & 0x40) {
            old_waypoint = vars->waypoint;
            if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                vars->waypoint = old_waypoint;
            } else {
                old_waypoint->command = 0;
                vars->waypoint->command = 1;
            }
            if (moby->prev_seq != 4) {
                FUN_00212f90(moby, 4, 0, 10);
            }
            moby->state = 1;
            vars->i134 = scale_game_frames(0xF0);
        }
        if (moby->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case BOMBTHROWER_STATE_DYING:
        if (mine_fall(moby, vars->blk70) & 0x40) {
            moby->anim_speed = 0.0f;
            moby->state = 100;
            moby->collision = 0;
            vars->s132 = 0x28;
            vars->s130 = D_L08_001619D0_gp;
            spawn_moby_table_puff_pattern(moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, vars->anim);
    vars->i38 = 0;
}

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
extern void update_moby_transform_wobble(struct Moby *) __asm__("FUN_L08_002dc4a0");
void        update_shuttle_stop_world_points(struct Moby *moby) __asm__("FUN_L08_002dc0c8");

void select_shuttle_route_for_hero(struct Moby *moby) __asm__("FUN_L08_002dc180");

int advance_shuttle_path_travel(struct Moby *moby) __asm__("FUN_L08_002dc648");

int  advance_shuttle_path_travel(struct Moby *m) __asm__("FUN_L08_002dc648");
void try_set_shuttle_departure_help_message(struct Moby *moby) __asm__("FUN_L08_002dc7f0");

/* Lift update: runs the lift's state machine (0 setup, 2 carry the hero
 * when riding, 3 travel to the selected stop) and moves anything riding it
 * by how far it moved this frame. */
void shuttle_update(struct Moby *moby) __asm__("FUN_L08_002dbdb0");

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
    update_moby_transform_wobble(moby);
    if (vars->stop != -1)
        update_shuttle_stop_world_points((char *)moby);
    fast_vec_sub(&delta, &moby->pos, &old_pos);
    update_entity_matrix(vars->motion, &delta, &old_rot, &moby->rot);
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD440), where it is exact; names translated to the US level program. */

extern char shuttle_stop_local_points[] __asm__("D_L08_001DB280");

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

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD4F8), where it is exact; names translated to the US level program. */

typedef struct {
    int   count;
    int   pad[3];
    float e[1][4];
} Route;

#include "rnc/game/game_globals.h"

extern int D_L08_0015F5CC; /* no foreign declaration */

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
/* Per-frame update: ease the angle and position fields toward their targets. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD9C0), where it is exact; names translated to the US level program. */

extern f32  advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("FUN_00214e58");

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
/* Spawns a moby of class 0x1A9 at a position, with a random spin and a heading from a vector. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DDC18), where it is exact; names translated to the US level program. */

char *spawn_bomber_tank_projectile(char *source_moby,
                                   char *spawn_position,
                                   char *initial_velocity) __asm__("FUN_L08_002dc8a0");

char *

spawn_bomber_tank_projectile(char *source_moby, char *spawn_position, char *initial_velocity) {
    char *moby = create_moby(0x1A9);
    char *data;
    if (moby) {
        data = ((char *)MOBY(moby)->pvars);
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        *(char **)(data + 0x20) = source_moby;
        *(int *)(data + 0x10) = scale_game_frames(0x12C);
        *(float *)(data + 0x1C) = 1.0f;
        *(int *)(data + 0x14) = 0;
        qcopy(moby + 0x10, spawn_position);
        qcopy(data, initial_velocity);
        MOBY(moby)->rot.x = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        MOBY(moby)->rot.z =
            fast_arc_tan(*(float *)initial_velocity, *(float *)(initial_velocity + 4)) + 1.5707964f;
        update_entity(moby);
    }
    return moby;
}

extern int
FUN_L08_002dc9a8_collision(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int   FUN_L08_002dc9a8_trace(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void  FUN_L08_002dc9a8_add(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void  FUN_L08_002dc9a8_scale(float, void *, void *) __asm__("FUN_001f9bf8");
extern float FUN_L08_002dc9a8_random(float, float) __asm__("FUN_002132a8");
extern float FUN_L08_002dc9a8_random_word(void) __asm__("FUN_00213308");
extern float FUN_L08_002dc9a8_sin(float) __asm__("FUN_001f9dc8");
extern float FUN_L08_002dc9a8_cos(float) __asm__("FUN_001f9de0");
extern int   FUN_L08_002dc9a8_random_below(int) __asm__("FUN_00213260");
extern int   FUN_L08_002dc9a8_random_color(int) __asm__("FUN_001f96f8");
extern int   FUN_L08_002dc9a8_color(float, int, int) __asm__("FUN_L08_00264930");
extern void
FUN_L08_002dc9a8_emit(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");

#include "rnc/overlay/collision.h"

extern int D_L08_00174658_2DC9A8 __asm__("D_L08_00174658") NOT_SDA;

typedef struct __attribute__((packed)) EffectVectorCopy {
    unsigned long long values[2];
} EffectVectorCopy;

typedef struct EffectRecord {
    float pos[4];
    float velocity[4];
    void *owner;
    int   flags;
    int   unknown;
    float scale;
    int   state;
} EffectRecord;

void update_ballistic_projectile(struct Moby *moby) __asm__("FUN_L08_002dc9a8");

void update_ballistic_projectile(struct Moby *moby) {
    enum {
        PROJECTILE_STATE_FLYING = 1,
        PROJECTILE_STATE_DETONATE = 99
    };
    enum {
        DETONATION_EFFECT_FIXED_COLORS = 0,
        DETONATION_EFFECT_NO_EMISSION = 1,
        DETONATION_EFFECT_TWO_COLOR_CHOICE = 2,
        DETONATION_EFFECT_SPHERICAL_VELOCITY = 3
    };
    char        *moby_bytes = (char *)moby;
    char        *data = *(char **)(moby_bytes + 0x78);
    float        original[4];
    EffectRecord record;
    float       *position;
    float        effect[4];
    float        velocity[4];
    float        frame_time;
    float        radius;
    int          i, axis;
    float        angle;

    if (FUN_L08_002dc9a8_collision(moby_bytes + 0x10, 0.5f, 0, moby_bytes, 0) != 0 &&
        D_L08_00174658_2DC9A8 != 0 &&
        (D_L08_00174658_2DC9A8 != *(int *)(data + 0x20) || *(int *)(data + 0x14) != 0)) {
        moby_bytes[0x20] = 99;
    }
    switch ((unsigned char)moby_bytes[0x20]) {
    case PROJECTILE_STATE_FLYING: {
        position = (float *)(moby_bytes + 0x10);
        qcopy(original, position);
        fast_vec_add(position, position, data);
        *(float *)(data + 8) -= D_0015ED70 * 10.8f;
        qcopy(record.pos, position);
        record.owner = moby_bytes;
        record.flags = 0x810000;
        record.state = 1;
        record.scale = 1.0f;
        qcopy(record.velocity, data);
        if (FUN_L08_002dc9a8_trace(original, &record, 0, moby_bytes, 0) != 0) {
            if (*(int *)(data + 0x20) != 0 && D_L08_00174640.moby != *(int *)(data + 0x20)) {
                moby_bytes[0x20] = 99;
                qcopy(position, D_L08_00174640.point.f);
                FUN_L08_002dc9a8_add(data, data, &D_L08_00174640.normal_x);
                FUN_L08_002dc9a8_scale(D_0015ED6C_lui + D_0015ED6C_lui, data, data);
            }
        } else {
            if (fast_dec_timer(data + 0x10) != 0)
                moby_bytes[0x20] = 99;
        }
    } break;
    case PROJECTILE_STATE_DETONATE: {
        record.pos[0] = D_0015ED6C_lui * 14.0f;
        record.pos[1] = D_0015ED6C_lui * 22.0f;
        record.pos[2] = D_0015ED6C_lui * 12.0f;
        record.pos[3] = 0.0f;
        *(EffectVectorCopy *)original = *(EffectVectorCopy *)record.pos;
        record.owner = moby_bytes;
        record.flags = 0x810001;
        record.scale = 1.0f;
        clear_u64_value(record.velocity);
        record.state = 0;
        FUN_L08_002dc9a8_collision(moby_bytes + 0x10, 1.0f, 9, moby_bytes, record.velocity);
        allocate_voice_for_target_entry(0, 0, moby_bytes);
        for (i = 0; i < 50; i++) {
            axis = i % 4;
            *(u128 *)velocity = 0;
            velocity[0] = FUN_L08_002dc9a8_random(D_0015ED6C_lui * -0.5f, D_0015ED6C_lui * 0.5f);
            velocity[1] = FUN_L08_002dc9a8_random(D_0015ED6C_lui * -0.5f, D_0015ED6C_lui * 0.5f);
            velocity[2] = FUN_L08_002dc9a8_random(D_0015ED6C_lui * 4.0f, original[axis]);
            *(u128 *)effect = *(u128 *)velocity;
            radius = FUN_L08_002dc9a8_random(0.0f, 0.25f);
            angle = FUN_L08_002dc9a8_random_word();
            record.pos[0] = FUN_L08_002dc9a8_sin(angle) * radius;
            record.pos[1] = FUN_L08_002dc9a8_cos(angle) * radius;
            record.pos[2] = 0.0f;
            fast_vec_add(record.pos, record.pos, moby_bytes + 0x10);
            frame_time = radius * D_0015ED6C_lui;
            radius *= 0.717f;
            record.pos[2] -= radius;
            effect[2] -= frame_time * 8.0f;
            position = effect;
            switch (axis) {
            case DETONATION_EFFECT_FIXED_COLORS: {
                int   color, color2, alpha;
                float size = FUN_L08_002dc9a8_random(200000.0f, 300000.0f);
                color = FUN_L08_002dc9a8_random_color(60);
                color2 = FUN_L08_002dc9a8_random_color(120);
                alpha = random_int_between(color, color2);
                FUN_L08_002dc9a8_emit(record.pos, position, 0x1f101820, 0x101010, size, alpha, 0);
            } break;
            case DETONATION_EFFECT_TWO_COLOR_CHOICE: {
                int color, color2, alpha;
                color = FUN_L08_002dc9a8_random_below(100) < 40 ? 0x5ff8f8f8 : 0x2f486078;
                color2 = FUN_L08_002dc9a8_random_color(30);
                alpha = FUN_L08_002dc9a8_random_color(45);
                FUN_L08_002dc9a8_emit(record.pos,
                                      position,
                                      color,
                                      0x0f000020,
                                      150000.0f,
                                      random_int_between(color2, alpha),
                                      2);
            } break;
            case DETONATION_EFFECT_SPHERICAL_VELOCITY: {
                int color, color2, alpha;
                color = FUN_L08_002dc9a8_color(
                    FUN_L08_002dc9a8_random(0.25f, 1.0f), 0x7f000000, 0x7f182030);
                color2 = FUN_L08_002dc9a8_color(FUN_L08_002dc9a8_random(0.5f, 1.0f), 0, 0x5f5f5f);
                radius = FUN_L08_002dc9a8_random(0.0f, 1.0f) * D_0015ED6C_lui;
                build_spherical_offset(position, radius, angle, FUN_L08_002dc9a8_random_word());
                radius = FUN_L08_002dc9a8_random(150000.0f, 200000.0f);
                alpha = FUN_L08_002dc9a8_random_color(60);
                alpha = random_int_between(alpha, FUN_L08_002dc9a8_random_color(120));
                FUN_L08_002dc9a8_emit(record.pos, position, color, color2, radius, alpha, 3);
            } break;
            case DETONATION_EFFECT_NO_EMISSION:
                break;
            }
        }
        mark_moby_for_removal(moby_bytes);
    } break;
    }
}

#include "sda.h"

/* Rocket guidance, route traversal, launch effects, and destruction. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DE738), where it is exact; names translated to the US level program. */

typedef struct {
    int   count;
    int   pad[3];
    float e[1][4];
} Route_2dd4f8_2dd3c0;

typedef unsigned int Q46 __attribute__((mode(TI)));

typedef float V46[4] __attribute__((aligned(16)));

typedef struct M46 M46;

typedef struct D46 D46;

typedef struct {
    char  pad0[0x10];
    int   animation;
    char  pad14[0x10];
    float scale;
} Model46;

struct M46 {
    char           pad0[0xC];
    float          distance;
    V46            position;
    unsigned char  state, group, pad22[2];
    Model46       *model;
    float          pad28, scale;
    unsigned char  pad30[2];
    unsigned short flags;
    char           pad34[0xC];
    V46            rotation;
    char           pad50[0x28];
    D46           *data;
    char           pad7C[0x18];
    int            animation;
    char           pad98[0xE];
    unsigned short class_id;
    char           padA8[0x18];
    V46            matrix[4];
};

struct D46 {
    char                 pad0[0x20];
    V46                  velocity;
    char                 pad30[0x10];
    float                pitch, yaw, roll;
    M46                 *target;
    int                  waypoint;
    signed char          direction;
    char                 pad55[3];
    int                  health;
    int                  pad5C;
    Route_2dd4f8_2dd3c0 *path;
    int                  path_index;
    char                 pad68[0x18];
    float                yaw_velocity;
    int                  pad84, effect_path;
    float                activation_range;
    int                  persistent, timer;
    float                speed;
};

typedef struct {
    int                  waypoint;
    signed char          direction;
    char                 pad5[0xB];
    Route_2dd4f8_2dd3c0 *path;
} RouteState46;

typedef struct {
    char          pad0[0x2C];
    unsigned char active;
} Persistent46;

extern char *D_L08_0015FFD8_macro __asm__("D_L08_0015FFD8") MACRO_ADDR;
extern char  D_0013F3D0[] NOT_SDA;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

#include "rnc/overlay/l08/mobys/bomb_data/update_rocket_3.h"

extern float rocket_guidance_velocity_correction_limit_gp __asm__("D_L08_00161A04")
    __attribute__((sda));
extern float rocket_route_speed_gp __asm__("D_L08_00161A08") __attribute__((sda));
extern float rocket_target_hit_distance_gp __asm__("D_L08_00161A0C") __attribute__((sda));
extern float rocket_effect_radial_speed_range_end_gp __asm__("D_L08_00161A14") __attribute__((sda));

#include "rnc/overlay/l08/mobys/bomb_data/update_rocket_2.h"

extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern float approach_value(float *, float, float);
extern int   D_L08_0015F5C4_macro __asm__("D_L08_0015F5C4") MACRO_ADDR;

#include "rnc/overlay/l08/mobys/bomb_data/update_rocket.h"

extern unsigned int D_L08_0015FFDC MACRO_ADDR;
extern void                        FUN_L00_00262cf0(void *, void *, float, float, float);
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void FUN_L08_002de528_u(void *, int, int, int) __asm__("FUN_L08_002de528");

void update_rocket(M46 *moby) __asm__("FUN_L08_002dd3c0");

void update_rocket(M46 *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_APPROACH_TARGET_OFFSET = 1,
        ROCKET_STATE_HOME_TOWARD_TARGET = 2,
        ROCKET_STATE_WAIT_FOR_ACTIVATION = 3,
        ROCKET_STATE_FOLLOW_ROUTE = 4,
        ROCKET_STATE_FLY_CONFIGURED_HEADING = 5,
        ROCKET_STATE_EXPLODE_AND_REMOVE = 6
    };
    enum {
        LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_POSITIVE_Z_LAUNCH_OFFSET = 0,
        LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET = 1,
        LINKED_CLASS_NEGATIVE_X_NEGATIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET = 2,
        LINKED_CLASS_VERTICAL_LAUNCH_OFFSET = 3,
        LINKED_CLASS_NEGATIVE_Y_POSITIVE_Z_LAUNCH_OFFSET = 4,
        LINKED_CLASS_ZERO_LAUNCH_OFFSET = 5
    };
    V46  target, delta, velocity_correction_or_effect_velocity, effect, aux;
    D46 *data = moby->data;
    if (FUN_L00_0025a420(moby, 0x210000, 0) && moby->state != 4) {
        allocate_voice_for_target_entry(0, 0, moby);
        cart_burst(moby, moby->position, -1, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case ROCKET_STATE_INITIALIZE: {
        char         *persistence_base = D_0013D388;
        Persistent46 *state = (Persistent46 *)(persistence_base + data->persistent);
        if (state->active) {
            short *link = D_L08_001AC040[moby->group];
            do {
                mark_moby_for_removal(D_L08_0015FFD8_macro +
                                      ((*(unsigned short *)link & 0x7FFF) << 8));
            } while (*link++ >= -1);
            return;
        }
        if (!data->persistent)
            data->timer = scale_game_frames(10);
        data->path = ((Route_2dd4f8_2dd3c0 *)l08_paths[data->path_index]);
        qcopy(moby->position, data->path->e[0]);
        data->waypoint = 1;
        moby->state = 3;
        moby->animation = 0;
        moby->flags = 0;
        return;
    }
    case ROCKET_STATE_APPROACH_TARGET_OFFSET: {
        D46  *target_data = data->target->data;
        int   life, size0, size1;
        float yaw, pitch, radius, angle;
        moby->rotation[0] = fast_add_rotations(moby->rotation[0], D_0015ED6C * 6.2831855f);
        qcopy(target, data->target->position);
        target[2] += 5.0f;
        yaw = fast_arc_tan(moby->position[0] - target[0], moby->position[1] - target[1]);
        pitch =
            fast_arc_tan(vector_distance_xy(target, moby->position), moby->position[2] - target[2]);
        FUN_L00_00262cf0(target, target, data->roll, data->yaw, data->pitch);
        fast_vec_sub(delta, target, moby->position);
        FUN_L00_0025f730(delta, target_data->speed * D_0015ED6C);
        fast_vec_sub(velocity_correction_or_effect_velocity, delta, data->velocity);
        FUN_L00_0025f730(velocity_correction_or_effect_velocity,
                         rocket_guidance_velocity_correction_limit_gp * frame_time_sq_macro);
        fast_vec_add(data->velocity, data->velocity, velocity_correction_or_effect_velocity);
        fast_vec_add(moby->position, moby->position, data->velocity);
        moby->rotation[1] = -fast_arc_tan(vector_length_xy(data->velocity), data->velocity[2]);
        moby->rotation[2] = fast_arc_tan(data->velocity[0], data->velocity[1]);
        if (fast_difference_between_rotations(yaw, data->yaw) < 0.08726646f &&
            fast_difference_between_rotations(pitch, data->pitch) < 0.08726646f)
            moby->state = 2;
        radius = random_float_between(0.0f, rocket_effect_radial_speed_range_end_gp) * D_0015ED6C;
        angle = random_angle_radians();
        fast_vec_normalize(effect, moby->matrix[2], radius * fast_cos(angle));
        fast_vec_normalize(aux, moby->matrix[1], radius * fast_sin(angle));
        fast_vec_add(effect, effect, aux);
        qcopy(aux, aux);
        effect[3] = rocket_guidance_effect_velocity_w_gp;
        aux[3] = rocket_guidance_effect_auxiliary_vector_w_gp;
        life = scale_game_frames(rocket_guidance_effect_frame_count_gp);
        size0 = truncate_float_to_s32(rocket_guidance_effect_velocity_w_gp);
        size1 = truncate_float_to_s32(rocket_guidance_effect_auxiliary_vector_w_gp);
        spawn_effect(moby->position,
                     effect,
                     rocket_guidance_effect_initial_color_gp,
                     rocket_guidance_effect_second_color_gp,
                     life,
                     size0,
                     size1,
                     1);
        return;
    }
    case ROCKET_STATE_HOME_TOWARD_TARGET: {
        D46  *target_data = data->target->data;
        int   life, size0, size1;
        float scale, radius, angle;
        moby->rotation[0] = fast_add_rotations(moby->rotation[0], D_0015ED6C * 6.2831855f);
        scale = moby->model->scale * 3.0f;
        approach_value(&moby->scale, scale, moby->model->scale / (float)scale_game_frames(180));
        qcopy(target, data->target->position);
        target[2] += 5.0f;
        fast_vec_sub(delta, target, moby->position);
        FUN_L00_0025f730(delta, target_data->speed * D_0015ED6C);
        fast_vec_sub(velocity_correction_or_effect_velocity, delta, data->velocity);
        FUN_L00_0025f730(velocity_correction_or_effect_velocity,
                         rocket_guidance_velocity_correction_limit_gp * 4.0f * frame_time_sq_macro);
        fast_vec_add(data->velocity, data->velocity, velocity_correction_or_effect_velocity);
        fast_vec_add(moby->position, moby->position, data->velocity);
        moby->rotation[1] = -fast_arc_tan(vector_length_xy(data->velocity), data->velocity[2]);
        moby->rotation[2] = fast_arc_tan(data->velocity[0], data->velocity[1]);
        if (fast_vec_dist(moby->position, data->target->position) < rocket_target_hit_distance_gp) {
            target_data->health -= 60;
            clear_u64_value(data->velocity);
            allocate_voice_for_target_entry(0, 0, moby);
            moby->state = 6;
        }
        radius = random_float_between(0.0f, rocket_effect_radial_speed_range_end_gp) * D_0015ED6C;
        angle = random_angle_radians();
        fast_vec_normalize(effect, moby->matrix[2], radius * fast_cos(angle));
        fast_vec_normalize(aux, moby->matrix[1], radius * fast_sin(angle));
        fast_vec_add(effect, effect, aux);
        qcopy(aux, aux);
        effect[3] = rocket_guidance_effect_velocity_w_gp;
        aux[3] = rocket_guidance_effect_auxiliary_vector_w_gp;
        life = scale_game_frames(rocket_guidance_effect_frame_count_gp);
        size0 = truncate_float_to_s32(rocket_guidance_effect_velocity_w_gp);
        size1 = truncate_float_to_s32(rocket_guidance_effect_auxiliary_vector_w_gp);
        spawn_effect(moby->position,
                     effect,
                     rocket_guidance_effect_initial_color_gp,
                     rocket_guidance_effect_second_color_gp,
                     life,
                     size0,
                     size1,
                     1);
        return;
    }
    case ROCKET_STATE_WAIT_FOR_ACTIVATION:
        if (D_L08_0015F5C4_macro)
            return;
        if (fast_vec_dist(D_0013F3D0, (char *)data->path + (data->path->count << 4)) <
                data->activation_range &&
            fast_dec_timer(&data->timer)) {
            moby->flags = 0x40;
            moby->state = 4;
            moby->animation = moby->model->animation;
            moby->rotation[2] = fast_arc_tan(data->path->e[1][0] - moby->position[0],
                                             data->path->e[1][1] - moby->position[1]);
            allocate_voice_for_target_entry(1, 0, moby);
        }
        return;
    case ROCKET_STATE_FOLLOW_ROUTE: {
        RouteState46 *route;
        int           next;
        float         yaw;
        if (D_L08_0015F5C4_macro)
            return;
        route = (RouteState46 *)&data->waypoint;
        next = (route->waypoint + route->path->count + route->direction) % route->path->count;
        yaw =
            fast_arc_tan(*(float *)((char *)route->path + (next << 4) + 0x10) - moby->position[0],
                         *(float *)((char *)route->path + (next << 4) + 0x14) - moby->position[1]);
        FUN_L00_00258278(moby, &data->yaw_velocity, yaw, 0.1f, 1.3f, 1.0f);
        moby->rotation[1] = fast_arc_tan(vector_distance_xy(route->path->e[next], moby->position),
                                         moby->position[2] - route->path->e[next][2]);
        qcopy_nc(target, route->path->e[next]);
        if (route->waypoint != route->path->count - 1) {
            int life0, life1, size1;
            fast_vec_sub(delta, target, moby->position);
            fast_vec_normalize(delta, delta, rocket_route_speed_gp * D_0015ED6C);
            fast_vec_add(moby->position, moby->position, delta);
            if (fast_vec_dist(moby->position, target) < 0.5f)
                route->waypoint = next;
            *(Q46 *)effect = (Q46)0;
            effect[0] = random_float_between(-1.0f, 1.0f);
            effect[1] = random_float_between(-1.0f, 1.0f);
            effect[2] = random_float_between(-1.0f, 1.0f);
            *(Q46 *)velocity_correction_or_effect_velocity = *(Q46 *)effect;
            FUN_L00_0025f730(velocity_correction_or_effect_velocity, D_0015ED6C + D_0015ED6C);
            life0 = scale_game_frames(FUN_L00_00257b90(30, 45));
            size1 = FUN_L00_00257b90(40, 70);
            spawn_effect(moby->position,
                         velocity_correction_or_effect_velocity,
                         0x6F00AFFF,
                         0xFF,
                         life0,
                         40,
                         size1,
                         1);
            *(Q46 *)aux = (Q46)0;
            aux[0] = random_float_between(-1.0f, 1.0f);
            aux[1] = random_float_between(-1.0f, 1.0f);
            aux[2] = random_float_between(-1.0f, 1.0f);
            *(Q46 *)effect = *(Q46 *)aux;
            FUN_L00_0025f730(effect, D_0015ED6C + D_0015ED6C);
            life1 = scale_game_frames(FUN_L00_00257b90(60, 120));
            size1 = FUN_L00_00257b90(100, 150);
            spawn_effect(moby->position, effect, 0x1FFFFFFF, 0x4F4F4F, life1, 40, size1, 0);
            return;
        } else {
            short *link = D_L08_001AC040[moby->group];
            long   special = 0;
            int    index;
            M46   *other;
            if (!link)
                return;
            index = *(unsigned short *)link & 0x7FFF;
            other = (M46 *)(D_L08_0015FFD8_macro + (index << 8));
            ((Persistent46 *)(D_0013D388 + data->persistent))->active = 1;
        linked_object:
            if (other->state < 0x7F && other != moby) {
                unsigned short linked_moby_class_id = other->class_id;
                if ((unsigned int)(linked_moby_class_id - 0x223) < 6) {
                    clear_u64_value(velocity_correction_or_effect_velocity);
                    switch ((short)(other->class_id - 0x223)) {
                    case LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_POSITIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[0] = -3.331f;
                        aux[1] = 3.331f;
                        aux[2] = 0.275f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_NEGATIVE_X_POSITIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[0] = -6.0f;
                        aux[1] = 6.0f;
                        aux[2] = -1.772f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_NEGATIVE_X_NEGATIVE_Y_NEGATIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[0] = -2.0f;
                        aux[1] = -1.152f;
                        aux[2] = -1.772f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_VERTICAL_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[2] = 0.975f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_NEGATIVE_Y_POSITIVE_Z_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        aux[1] = -1.665f;
                        aux[2] = 0.642f;
                        qcopy(effect, aux);
                        break;
                    case LINKED_CLASS_ZERO_LAUNCH_OFFSET:
                        *(Q46 *)aux = (Q46)0;
                        qcopy(effect, aux);
                        break;
                    }
                    if (other->distance > 1500.0f)
                        mark_moby_for_removal(other);
                    else {
                        float  launch_yaw, center_yaw, min_frames, max_frames, angle_delta;
                        float *launch_offset;
                        fast_vec_sub(velocity_correction_or_effect_velocity,
                                     other->position,
                                     moby->position);
                        velocity_correction_or_effect_velocity[2] = 0.0f;
                        fast_vec_normalize(velocity_correction_or_effect_velocity,
                                           velocity_correction_or_effect_velocity,
                                           D_0015ED6C * 12.0f);
                        velocity_correction_or_effect_velocity[2] =
                            random_float_between(D_0015ED6C * 8.0f, D_0015ED6C * 12.0f);
                        other->animation = 0;
                        launch_yaw = fast_arc_tan(velocity_correction_or_effect_velocity[0],
                                                  velocity_correction_or_effect_velocity[1]);
                        center_yaw = fast_arc_tan(l08_camera.pos.f[0] - moby->position[0],
                                                  l08_camera.pos.f[1] - moby->position[1]);
                        angle_delta = fast_difference_between_rotations(launch_yaw, center_yaw);
                        launch_offset = effect;
                        if (angle_delta < 0.7853982f) {
                            min_frames = 10.0f;
                            max_frames = 20.0f;
                        } else {
                            min_frames = 60.0f;
                            max_frames = 90.0f;
                        }
                        initialize_moby_launch_motion(
                            other,
                            (int)moby,
                            velocity_correction_or_effect_velocity,
                            launch_offset,
                            truncate_float_to_s32(random_float_between(min_frames, max_frames)));
                    }
                } else if ((short)linked_moby_class_id == 0x1A8) {
                    special = 1;
                    clear_u64_value(velocity_correction_or_effect_velocity);
                    spawn_explosion(other,
                                    velocity_correction_or_effect_velocity,
                                    other->position,
                                    0.0f,
                                    0.0f,
                                    50,
                                    60,
                                    60,
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
            if (*link++ >= -1) {
                index = *(unsigned short *)link & 0x7FFF;
                other = (M46 *)(D_L08_0015FFD8_macro + (index << 8));
                goto linked_object;
            }
            if (special)
                FUN_L08_002de528_u(moby, data->effect_path, 30, 1);
            else
                FUN_L08_002de528_u(moby, data->effect_path, 30, 0);
            allocate_voice_for_target_entry(2, 0, moby);
            moby->state = 6;
        }
        return;
    }
    case ROCKET_STATE_FLY_CONFIGURED_HEADING:
        step_angle_toward_target(&moby->rotation[2], 0, data->yaw, D_0015ED6C * 0.5235988f);
        step_angle_toward_target(&moby->rotation[1], 0, -data->pitch, D_0015ED6C * 0.5235988f);
        moby->rotation[0] = fast_add_rotations(moby->rotation[0], D_0015ED6C * 6.2831855f);
        build_spherical_offset(
            data->velocity,
            (rocket_configured_heading_speed_base_gp + rocket_configured_heading_speed_base_gp) *
                D_0015ED6C,
            moby->rotation[2],
            -moby->rotation[1]);
        fast_vec_add(moby->position, moby->position, data->velocity);
        if (FUN_L08_002025d0(moby->position, 0x400, 2, moby, 0) || moby->position[0] < 5.0f ||
            moby->position[0] > 500.0f || moby->position[1] < 5.0f || moby->position[1] > 500.0f ||
            moby->position[2] < 5.0f || moby->position[2] > 500.0f ||
            fast_vec_dist(D_0013F3D0, moby->position) < 16.0f)
            moby->state = 6;
        return;
    case ROCKET_STATE_EXPLODE_AND_REMOVE:
        if (D_L08_0015FFDC < (unsigned int)moby)
            spawn_explosion(moby,
                            data->velocity,
                            moby->position,
                            0.0f,
                            0.0f,
                            15,
                            10,
                            10,
                            3.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
        mark_moby_for_removal(moby);
        break;
    default:
        break;
    }
}

extern void  set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern int   class_1b3_projectile_color_red_gp __asm__("D_L08_00161A34") __attribute__((sda));
extern int   class_1b3_projectile_color_green_gp __asm__("D_L08_00161A38") __attribute__((sda));
extern int   class_1b3_projectile_color_blue_gp __asm__("D_L08_00161A3C") __attribute__((sda));
extern float class_1b3_projectile_scale_multiplier_gp __asm__("D_L08_00161A40")
    __attribute__((sda));

struct Moby *spawn_class_1b3_projectile(void        *pos,
                                        struct Moby *target,
                                        void        *velocity,
                                        float        pitch,
                                        float        yaw) __asm__("FUN_L08_002de3e0");

struct Moby *

spawn_class_1b3_projectile(void *pos, struct Moby *target, void *velocity, float pitch, float yaw) {
    struct Moby *moby = create_moby(0x1B3);
    char        *data;
    float       *reference;
    float        distance;
    float        scale;
    struct Moby *target_copy = target;
    void        *velocity_copy = velocity;
    if (moby != 0) {
        data = (char *)moby->pvars;
        qcopy(&moby->pos, pos);
        reference = D_L08_001675C0;
        distance = vector_distance_xy(reference, &moby->pos);
        reference = (float *)((char *)reference - 0x140);
        moby->rot.y = -fast_arc_tan(distance, reference[82] - moby->pos.z);
        *(volatile float *)&moby->rot.z =
            fast_arc_tan(reference[80] - moby->pos.x, reference[81] - moby->pos.y);
        scale = moby->pclass->scale;
        scale *= class_1b3_projectile_scale_multiplier_gp;
        moby->update_dist = 0xFF;
        moby->visible = 1;
        *(volatile u8 *)&moby->state = 1;
        moby->draw_dist = 0xFF;
        moby->scale = scale;
        set_moby_color(moby,
                       class_1b3_projectile_color_red_gp,
                       class_1b3_projectile_color_green_gp,
                       class_1b3_projectile_color_blue_gp);
        *(float *)(data + 0x40) = pitch;
        *(float *)(data + 0x48) = 70.0f;
        *(float *)(data + 0x44) = yaw;
        *(struct Moby **)(data + 0x4C) = target_copy;
        qcopy(data + 0x20, velocity_copy);
        update_entity(moby);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DF8A0), where it is exact; names translated to the US level program. */

void spawn_rocket_explosion_and_debris(char *moby,
                                       int   transform_index,
                                       int   spawn_count,
                                       int   flag) __asm__("FUN_L08_002de528");

void spawn_rocket_explosion_and_debris(char *moby, int transform_index, int spawn_count, int flag) {
    float spawn_position[4];
    float local_rotation_offset[4];
    float launch_velocity[4];
    char *data = ((char *)MOBY(moby)->pvars);
    int   i;

    if (flag) {
        spawn_explosion(moby,
                        data + 0x20,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x1E,
                        0x14,
                        0x14,
                        6.0f,
                        4.0f,
                        9.0f,
                        1.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
    } else {
        spawn_explosion(moby,
                        data + 0x20,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x2D,
                        0x1E,
                        0x1E,
                        10.0f,
                        6.0f,
                        9.0f,
                        1.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
    }
    for (i = 0; i < spawn_count; i++) {
        {
            char *spawned_moby;
            if (flag) {
                if (random_integer_below(0xFF) & 1)
                    spawned_moby = create_moby(0x30E);
                else
                    spawned_moby = create_moby(0x30F);
            } else {
                if (random_integer_below(0xFF) & 1)
                    spawned_moby = create_moby(0x310);
                else
                    spawned_moby = create_moby(0x311);
            }
            MOBY(spawned_moby)->draw_dist = 0x40;
            spawned_moby[0x31] = 1;
            spawned_moby[0x30] = 0x40;
            MOBY(spawned_moby)->spawn_frame = ((long)MOBY(moby)->spawn_frame);
            MOBY(spawned_moby)->rot.x = random_angle_radians();
            MOBY(spawned_moby)->rot.y = random_angle_radians();
            MOBY(spawned_moby)->rot.z = random_angle_radians();
            spawn_position[0] = random_float_between(-1.0f, 1.0f);
            spawn_position[1] = random_float_between(-1.0f, 1.0f);
            spawn_position[2] = random_float_between(-1.0f, 1.0f);
            spawn_position[3] = 1.0f;
            transform_vector(
                spawn_position, spawn_position, D_L08_001600EC + (transform_index << 7));
            qcopy(spawned_moby + 0x10, spawn_position);
            clear_u64_value(local_rotation_offset);
            fast_vec_sub(launch_velocity, spawn_position, moby + 0x10);
            set_vector_length_xy(
                launch_velocity,
                launch_velocity,
                random_float_between(D_0015ED6C_lui * 12.0f, D_0015ED6C_lui * 15.0f));
            launch_velocity[2] =
                random_float_between(D_0015ED6C_lui * 6.0f, D_0015ED6C_lui * 18.0f);
            initialize_moby_launch_motion(
                spawned_moby,
                (int)moby,
                launch_velocity,
                local_rotation_offset,
                truncate_float_to_s32(random_float_between(60.0f, 90.0f)));
        }
    }
}
/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002DFBC0) */

/* Update function: switch on the state byte at +0x20. State 0 reads the table D_L08_001B0FB0[*data] (deleting
 * the moby if the index is -1), 1 steps along it, 2 falls under gravity and deletes the moby below z = 10.0
 * or on a hit. */

extern int camera_secondary_mode_lui __asm__("D_001413D4") NOT_SDA;

#include "rnc/overlay/l08/mobys/bomb_data/update_path_following_moby.h"

extern unsigned char D_0013D408[];
extern void          FUN_L08_002dec90_c(char *m, char *pos) __asm__("FUN_L08_002dec90");

void update_path_following_moby(char *moby) __asm__("FUN_L08_002de848");

void update_path_following_moby(char *moby) {
    enum {
        MOBY_STATE_INITIALIZE_PATH = 0,
        MOBY_STATE_FOLLOW_PATH = 1,
        MOBY_STATE_FALLING = 2
    };
    float previous_position[4];
    float frame_displacement[4];
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    char *moby_position = moby + 0x10;
    int  *path;
    qcopy(previous_position, moby_position);
    path = (int *)l08_paths[*(int *)moby_vars];
    switch (MOBY(moby)->state) {
    case MOBY_STATE_INITIALIZE_PATH:
        if (*(int *)moby_vars == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        {
            float first_path_segment_length =
                fast_vec_dist((char *)path + 0x10, (char *)path + 0x20);
            fast_vec_dist((char *)path + 0x10, (char *)path + (*path << 4));
            *(float *)(moby_vars + 8) =
                path_following_moby_speed_gp * D_0015ED6C / first_path_segment_length;
            *(float *)(moby_vars + 4) = *(float *)(moby_vars + 4) * (float)*path;
            *(float *)(moby_vars + 0xC) = randf_sym(0.0f, 5.0f);
        }
        moby[0x20] = 1;
        MOBY(moby)->update_dist = 0xFF;
    case MOBY_STATE_FOLLOW_PATH: {
        float path_point_count;
        sample_camera_path(
            path, 1, moby + 0x10, (float *)&MOBY(moby)->rot, 0, *(float *)(moby_vars + 4));
        MOBY(moby)->rot.y = MOBY(moby)->rot.y * 0.5f;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + *(float *)(moby_vars + 0xC);
        *(float *)(moby_vars + 4) = *(float *)(moby_vars + 4) + *(float *)(moby_vars + 8);
        path_point_count = (float)*path;
        if (path_point_count < *(float *)(moby_vars + 4)) {
            *(float *)(moby_vars + 4) = *(float *)(moby_vars + 4) - path_point_count;
        }
        break;
    }
    case MOBY_STATE_FALLING: {
        float falling_gravity = 15.0f;
        float falling_removal_height = 10.0f;
        float dd;
        float falling_rotation_step;
        char *falling_velocity = moby_vars + 0x10;
        falling_rotation_step = D_0015ED6C * 7.3303828f;
        *(float *)(moby_vars + 0x18) = *(float *)(moby_vars + 0x18) - D_0015ED70 * falling_gravity;
        dd = fast_add_rotations(MOBY(moby)->rot.x, falling_rotation_step);
        MOBY(moby)->rot.x = MOBY(moby)->rot.x + dd;
        MOBY(moby)->rot.y =
            -fast_arc_tan(vector_length_xy(falling_velocity), *(float *)(moby_vars + 0x18));
        fast_vec_add(moby_position, moby_position, falling_velocity);
        if (MOBY(moby)->pos.z < falling_removal_height) {
            mark_moby_for_removal(moby);
            return;
        }
        if (coll_sphere(1.25f, moby_position, 0, moby)) {
            spawn_explosion(moby,
                            falling_velocity,
                            moby_position,
                            0.0f,
                            0.0f,
                            0x14,
                            0x28,
                            0x10,
                            falling_removal_height,
                            5.0f,
                            9.0f,
                            1.0f,
                            -1,
                            falling_gravity,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    default:
        break;
    }
    if (MOBY(moby)->pos.x < 4.0f || MOBY(moby)->pos.y < 4.0f || MOBY(moby)->pos.z < 4.0f) {
        MOBY(moby)->collision = 0;
    } else {
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
    }
    if (find_moby_hit(moby, 0x10000, 0) != 0 && MOBY(moby)->state != 2) {
        if (camera_secondary_mode_lui == 0x32 && D_0013D408[0xC] == 0) {
            D_0013D408[0xC] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            FUN_L00_00263d40(0x53D6, -1);
        }
        fast_vec_sub(frame_displacement, moby + 0x10, previous_position);
        qcopy(moby_vars + 0x10, frame_displacement);
        *(float *)(moby_vars + 0x18) = *(float *)(moby_vars + 0x18) + (D_0015ED6C + D_0015ED6C);
        spawn_explosion(moby,
                        frame_displacement,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x14,
                        0x28,
                        0x10,
                        10.0f,
                        5.0f,
                        9.0f,
                        1.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        moby[0x20] = 2;
    }
    MOBY(moby)->hit_slot = 0xFF;
    if (MOBY(moby)->visible != 0) {
        FUN_L08_002dec90_c(moby, (char *)previous_position);
    }
}
/* Spawns a burst of particles at an offset from the moby, with random sizes and colours from level tuning values. */

extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float moby_motion_spark_first_displacement_scale_gp __asm__("D_L08_00161A48")
    __attribute__((sda));
extern float moby_motion_spark_second_displacement_scale_gp __asm__("D_L08_00161A4C")
    __attribute__((sda));
extern float moby_motion_spark_jitter_magnitude_range_end_gp __asm__("D_L08_00161A50")
    __attribute__((sda));
extern int moby_motion_spark_frame_count_random_span_gp __asm__("D_L08_00161A54")
    __attribute__((sda));
extern int moby_motion_spark_second_frame_count_base_gp __asm__("D_L08_00161A58")
    __attribute__((sda));
extern int moby_motion_spark_third_frame_count_base_gp __asm__("D_L08_00161A5C")
    __attribute__((sda));
extern float moby_motion_spark_frame_count_variation_extent_gp __asm__("D_L08_00161A60")
    __attribute__((sda));
extern float moby_motion_spark_first_vector_w_range_start_gp __asm__("D_L08_00161A64")
    __attribute__((sda));
extern float moby_motion_spark_first_vector_w_range_end_gp __asm__("D_L08_00161A68")
    __attribute__((sda));
extern float moby_motion_spark_second_vector_w_range_start_gp __asm__("D_L08_00161A6C")
    __attribute__((sda));
extern float moby_motion_spark_second_vector_w_range_end_gp __asm__("D_L08_00161A70")
    __attribute__((sda));
extern int moby_motion_spark_initial_color_start_gp __asm__("D_L08_00161A74") __attribute__((sda));
extern int moby_motion_spark_initial_color_end_gp __asm__("D_L08_00161A78") __attribute__((sda));
extern int moby_motion_spark_second_color_start_gp __asm__("D_L08_00161A7C") __attribute__((sda));
extern int moby_motion_spark_second_color_end_gp __asm__("D_L08_00161A80") __attribute__((sda));

void spawn_moby_motion_spark(struct Moby *moby, char *pos) __asm__("FUN_L08_002dec90");

void spawn_moby_motion_spark(struct Moby *moby, char *pos) {
    float scaled_displacement_vector[4];
    float jittered_displacement_vector[4];
    float frame_displacement[4];
    float random_jitter[4];
    float spark_position[4];
    float local_spawn_offset[4];
    int initial_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count;
    char *spawned_spark;
    fast_vec_sub(frame_displacement, &moby->pos, pos);
    *(OvlQuad *)local_spawn_offset = 0;
    local_spawn_offset[0] = -2.3f;
    local_spawn_offset[2] = 0.3f;
    transform_vector_by_basis(spark_position, local_spawn_offset, &moby->rot_row0);
    fast_vec_add(spark_position, spark_position, &moby->pos);
    fast_vec_scale(scaled_displacement_vector,
                   frame_displacement,
                   moby_motion_spark_first_displacement_scale_gp);
    fast_vec_scale(jittered_displacement_vector,
                   frame_displacement,
                   moby_motion_spark_second_displacement_scale_gp);
    random_vector(
        random_jitter, 0.0f, moby_motion_spark_jitter_magnitude_range_end_gp * frame_time);
    fast_vec_add(jittered_displacement_vector, jittered_displacement_vector, random_jitter);
    scaled_displacement_vector[3] =
        random_float_between(moby_motion_spark_first_vector_w_range_start_gp,
                             moby_motion_spark_first_vector_w_range_end_gp);
    jittered_displacement_vector[3] =
        random_float_between(moby_motion_spark_second_vector_w_range_start_gp,
                             moby_motion_spark_second_vector_w_range_end_gp);
    initial_spark_color = fast_tween_color(moby_motion_spark_initial_color_start_gp,
                                           moby_motion_spark_initial_color_end_gp,
                                           random_float_between(0.0f, 1.0f));
    second_spark_color = fast_tween_color(moby_motion_spark_second_color_start_gp,
                                          moby_motion_spark_second_color_end_gp,
                                          random_float_between(0.0f, 1.0f));
    frame_count = truncate_float_to_s32(scale_time(
        (float)moby_motion_spark_frame_count_random_span_gp * random_float_between(0.0f, 1.0f) +
        1.0f));
    second_frame_count = truncate_float_to_s32(
        scale_time((float)moby_motion_spark_second_frame_count_base_gp *
                   (random_float_between(-moby_motion_spark_frame_count_variation_extent_gp,
                                         moby_motion_spark_frame_count_variation_extent_gp) +
                    1.0f)));
    third_frame_count = truncate_float_to_s32(
        scale_time((float)moby_motion_spark_third_frame_count_base_gp *
                   (random_float_between(-moby_motion_spark_frame_count_variation_extent_gp,
                                         moby_motion_spark_frame_count_variation_extent_gp) +
                    1.0f)));
    spawned_spark = spawn_spark(spark_position,
                                scaled_displacement_vector,
                                jittered_displacement_vector,
                                initial_spark_color,
                                second_spark_color,
                                frame_count,
                                second_frame_count,
                                third_frame_count,
                                -1);
    if (spawned_spark != 0) {
        spawned_spark[9] = truncate_float_to_s32(8.0f) - 0x70;
    }
}

blk16 l08_bomb_moving_particle_initial_colors __asm__("D_L08_00161868") = {
    {-1, -1, -1, 0x2f, -1, -1, 0, 0x2f, -1, 0x7f, 0, 0x2f, -1, 0x4f, 0, 0x2f}
};

int D_L08_0015F5B0 __asm__("D_L08_0015F5B0") = 0;
int D_L08_0015F5B4_gp __asm__("D_L08_0015F5B4") = 0;
int D_L08_0015F5BC_gp __asm__("D_L08_0015F5BC") = 0;

int bomber_tank_part_3_destroyed_by_class_661_gp __asm__("D_L08_0015F5B8") = 0;

float water_entry_plane_data[4] __asm__("D_L08_001D7060") = {-2.6e+02f, -2.2e+02f, 15.0f, 1.6e+05f};

int moby_motion_spark_initial_color_end_gp __asm__("D_L08_00161A78") = 0x6020a0c0;

int moby_motion_spark_second_color_end_gp __asm__("D_L08_00161A80") = 0x20601020;
