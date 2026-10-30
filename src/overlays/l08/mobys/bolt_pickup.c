/* Level 08 moby class 0x98: bolt pickup. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/random.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002B9438), where it is exact; names translated to the US level program. */
extern int   spawn_moby_offset_effect(char *) __asm__("FUN_L08_00279f00");
extern void  copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");
extern float bolt_pickup_spin_increment_range_start_gp __asm__("D_L08_00161468")
    __attribute__((sda));
extern float bolt_pickup_spin_increment_range_end_gp __asm__("D_L08_0016146C") __attribute__((sda));
void         initialize_bolt_pickup(struct Moby *moby) __asm__("FUN_L08_002b8228");

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

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

#include "rnc/overlay/l08/mobys/bolt_pickup_data/update_bolt_pickup.h"

extern float bolt_pickup_collision_stop_speed_threshold_gp __asm__("D_L08_00161458")
    __attribute__((sda));
extern float bolt_pickup_collision_speed_reduction_gp __asm__("D_L08_0016145C")
    __attribute__((sda));
extern int   bolt_pickup_bob_period_ticks_gp __asm__("D_L08_00161460") __attribute__((sda));
extern float bolt_pickup_bob_amplitude_gp __asm__("D_L08_00161464") __attribute__((sda));
extern int   D_L08_0015F5CC;
extern void  mark_moby_for_removal(void *) __asm__("func_0020C828");
extern int   func_L00_001F39B0_2B94B0(void *, void *, int, int, int) __asm__("FUN_L08_002025d0");
extern void  func_L00_001FF610_2B94B0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int
queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern float func_001F9FA8_2B94B0(float) __asm__("FUN_001f9de0");
extern void  initialize_bolt_counter_hud(void) __asm__("FUN_L00_00239cc8");
extern void  update_bolt_counter_hud_animation_and_visibility(void) __asm__("FUN_L00_00239d00");
extern void  draw_bolt_counter_hud_panel(void) __asm__("FUN_L00_00239df8");
extern char  D_L08_001746D8_2B94B0[] __asm__("D_L08_00174658");
extern int   D_L08_00174658_lui __asm__("D_L08_00174658") NOT_SDA;
extern char  D_L08_00174700_2B94B0[] __asm__("D_L08_00174680");
extern s32   level_bolts[] __asm__("D_0013DF38");
void         update_bolt_pickup(Moby_2B94B0 *moby) __asm__("FUN_L08_002b82a0");

extern int D_L08_0015F5CC;

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
        queue_animation_update(2,
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
