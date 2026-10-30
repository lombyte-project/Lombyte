/* Ground particle scatter (class 0x578): fans particles out over the ground around the camera target, with its random parameter refresh. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/rendering/effects/ground_particle_scatter_globals.h"
#include "rnc/overlay/shared/rendering/effects/ground_particle_scatter.h"

typedef struct Moby Moby;

typedef int u128_3091B0 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_3091B0;

typedef struct {
    char     p0[0x30];
    V_3091B0 v30;
} Obj;

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");

extern short ground_particle_scatter_count_interpolation_start_override_gp __asm__("D_L08_0016234C")
    __attribute__((sda));
extern short ground_particle_scatter_count_interpolation_end_override_gp __asm__("D_L08_00162350")
    __attribute__((sda));
void randomize_ground_particle_scatter_parameters(int use_reduced_xy_range_preset) __asm__("FUN_L08_00307b90");

void randomize_ground_particle_scatter_parameters(int use_reduced_xy_range_preset) {
    float xy_random_range_scale, countdown_range_start, count_interpolation_end,
        count_interpolation_start;
    float xy_range_reference_vector[2];
    float xy_length_ratio;

    if (use_reduced_xy_range_preset == 0) {
        xy_random_range_scale = 5.0f;
        countdown_range_start = 2.0f;
        count_interpolation_end = 8.0f;
        count_interpolation_start = 4.0f;
    } else {
        xy_random_range_scale = 2.5f;
        countdown_range_start = 20.0f;
        count_interpolation_end = 3.0f;
        count_interpolation_start = 2.0f;
    }

    if (*(float *)&ground_particle_scatter_count_interpolation_start_override_gp >= 0.0f)
        count_interpolation_start =
            *(float *)&ground_particle_scatter_count_interpolation_start_override_gp;
    if (*(float *)&ground_particle_scatter_count_interpolation_end_override_gp >= 0.0f)
        count_interpolation_end =
            *(float *)&ground_particle_scatter_count_interpolation_end_override_gp;
    ground_particle_scatter_spawn_vector_xy_bias[0] = random_float_between(
        -(xy_random_range_scale * frame_time), xy_random_range_scale * frame_time);
    ground_particle_scatter_spawn_vector_xy_bias[1] = random_float_between(
        -(xy_random_range_scale * frame_time), xy_random_range_scale * frame_time);
    countdown_range_start = scale_time(countdown_range_start);
    ground_particle_scatter_refresh_countdown =
        random_float_between(countdown_range_start, scale_time(10.0f));
    xy_range_reference_vector[0] = xy_random_range_scale * frame_time;
    xy_range_reference_vector[1] = xy_range_reference_vector[0];
    xy_length_ratio = vector_length_xy(ground_particle_scatter_spawn_vector_xy_bias) /
                      fast_vec_length(xy_range_reference_vector);
    ground_particle_scatter_base_attempt_count =
        (int)(xy_length_ratio * (count_interpolation_end - count_interpolation_start) +
              count_interpolation_start);
}

/* moby class 1400 update: steers the offset vectors from the mode, fans them out to the object over counted loops */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_003091B0), where it is exact; names translated to the US level program. */

extern char  D_L08_001675C0[];
extern float D_L08_00160260 __attribute__((sda));
extern float ground_particle_scatter_minimum_z_scale_gp __asm__("D_L08_00162340")
    __attribute__((sda));
extern float ground_particle_scatter_reference_height_offset_gp __asm__("D_L08_00162344")
    __attribute__((sda));
extern float ground_particle_scatter_xy_offset_random_range_gp __asm__("D_L08_00162348")
    __attribute__((sda));
extern float ground_particle_scatter_minimum_z_scale_override_gp __asm__("D_L08_00162354")
    __attribute__((sda));
extern int ground_particle_scatter_distant_spawn_argument_gp __asm__("D_L08_00162358")
    __attribute__((sda));
extern float ground_particle_scatter_ground_height_floor_offset_gp __asm__("D_L08_0016235C")
    __attribute__((sda));
extern int                        update_float_countdown() __asm__("FUN_L08_00211b08");
extern u128_3091B0 D_L08_00160250 MACRO_ADDR;
extern void spawn_span_particle(void *, void *, void *, float) __asm__("FUN_L08_00272cb8");
void        FUN_L00_00262cf0(void *a, void *b);
void create_type_49_particle(void *pos, int kind, void *vel, float f) __asm__("FUN_L08_0027f5f0");
void randomize_ground_particle_scatter_parameters(int arg) __asm__("FUN_L08_00307b90");
extern char  D_L08_00167640_3091B0[] __asm__("D_L08_001675C0");
extern void  randomize_ground_particle_scatter_parameters(int) __asm__("FUN_L08_00307b90");
extern void  fast_add_rotations_cf(void *, void *, float, float, float) __asm__("FUN_L00_00262cf0");
extern float func_002140F8_3091B0(float, float) __asm__("FUN_002132a8");
extern void  func_L08_00273A80_3091B0(void *, void *, float, void *) __asm__("FUN_L08_00272cb8");
extern void  func_L08_002803B8_3091B0(void *, void *, float, void *) __asm__("FUN_L08_0027f5f0");

void scatter_ground_particles(Moby *moby) __asm__("FUN_L08_00307cf0");

void scatter_ground_particles(Moby *moby) {
    enum {
        GROUND_PARTICLES_STATE_INITIALIZE = 0,
        GROUND_PARTICLES_STATE_SCATTER = 2
    };

    V_3091B0 pt;
    V_3091B0 dv;
    V_3091B0 base;
    V_3091B0 origin_delta_or_spawn_vector;
    V_3091B0 par;
    int      flag;
    int      big;
    int      pass_attempt_count;
    int      remaining_pass_attempts;
    Obj     *moby_data;
    float    lo;
    float    hi;
    float    fall;
    float    spread;
    float    ground_height;
    float    w;
    float    zero;

    if (current_level_index == 0xC || current_level_index == 0x200) {
        flag = 1;
    } else {
        flag = 0;
    }

    switch (flag) {
    case TRUE:
        fall = 0.2f;
        big = 0;
        hi = 5.0f;
        spread = frame_time;
        lo = 0.5f;
        break;
    case FALSE:
    default: {
        float k = frame_time;
        big = 1;
        spread = k + k;
    }
        fall = 0.15f;
        hi = 18.0f;
        lo = ground_particle_scatter_minimum_z_scale_gp;
        break;
    }

    {
        float k = ground_particle_scatter_minimum_z_scale_override_gp;
        zero = 0.0f;
        if (zero <= k)
            lo = k;
    }

    moby_data = (Obj *)moby->pvars;

    switch (moby->state) {
    case GROUND_PARTICLES_STATE_INITIALIZE:
        moby->state = 2;
        moby->update_dist = 0xFF;
        randomize_ground_particle_scatter_parameters(flag);
        break;
    case GROUND_PARTICLES_STATE_SCATTER:
        if (update_float_countdown(&ground_particle_scatter_refresh_countdown))
            randomize_ground_particle_scatter_parameters(flag);
        fast_vec_sub(&dv, D_L08_00167640_3091B0, &moby_data->v30);
        D_L08_00160250 = *(u128_3091B0 *)&dv;
        *(u128_3091B0 *)&base = *(u128_3091B0 *)D_L08_00167640_3091B0;
        pass_attempt_count = ground_particle_scatter_base_attempt_count;
        fast_add_rotations_cf(&base,
                              &base,
                              16.0f,
                              ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->f158,
                              ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->f154);
        base.z = ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height +
                 ground_particle_scatter_reference_height_offset_gp;
        if (pass_attempt_count > 0) {
            V_3091B0 *po = &origin_delta_or_spawn_vector;
            remaining_pass_attempts = pass_attempt_count;
            do {
                float origin_distance_xy;
                float t;
                float u;
                *(u128_3091B0 *)&pt = *(u128_3091B0 *)&base;
                w = fast_cos(func_002140F8_3091B0(0.0f, 6.2831855f));
                w = w *
                    func_002140F8_3091B0(0.0f, ground_particle_scatter_xy_offset_random_range_gp);
                pt.x += w;
                w = fast_sin(func_002140F8_3091B0(0.0f, 6.2831855f));
                w = w *
                    func_002140F8_3091B0(0.0f, ground_particle_scatter_xy_offset_random_range_gp);
                pt.y += w;
                fast_vec_sub(po, &pt, D_L08_00167640_3091B0);
                origin_distance_xy = vector_length_xy(po);
                if (origin_distance_xy <= 8.0f) {
                    w = -spread;
                    par.x = ground_particle_scatter_spawn_vector_xy_bias[0] +
                            func_002140F8_3091B0(w, spread);
                    par.y = ground_particle_scatter_spawn_vector_y_bias +
                            func_002140F8_3091B0(w, spread);
                    par.z = -(hi * frame_time);
                    par.w = 0.0f;
                } else {
                    float h = (origin_distance_xy - 8.0f) * fall;
                    h = hi - h;
                    if (h < lo)
                        h = lo;
                    w = spread;
                    w = -w;
                    par.x = ground_particle_scatter_spawn_vector_xy_bias[0] +
                            func_002140F8_3091B0(w, spread);
                    par.y = ground_particle_scatter_spawn_vector_y_bias +
                            func_002140F8_3091B0(w, spread);
                    par.z = -(h * frame_time);
                    par.w = 0.0f;
                }
                D_L08_00160260 =
                    ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height -
                    ground_particle_scatter_ground_height_floor_offset_gp;
                ground_height = ground_height_at(&pt);
                if (ground_height <
                    ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height +
                        ground_particle_scatter_reference_height_offset_gp) {
                    void *distance_selected_spawn_argument;
                    if (ground_height < D_L08_00160260) {
                        w = D_L08_00160260;
                    } else {
                        w = ground_height;
                    }
                    distance_selected_spawn_argument = 0;
                    if (8.0f < origin_distance_xy)
                        distance_selected_spawn_argument =
                            ground_particle_scatter_distant_spawn_argument_gp;
                    if (flag == 0)
                        func_L08_00273A80_3091B0(&pt, distance_selected_spawn_argument, w, &par);
                    if (flag == 1)
                        func_L08_002803B8_3091B0(&pt, distance_selected_spawn_argument, w, &par);
                }
            } while (--remaining_pass_attempts != 0);
        }
        pass_attempt_count = (int)((float)pass_attempt_count * 1.4f);
        *(u128_3091B0 *)&base = *(u128_3091B0 *)D_L08_00167640_3091B0;
        base.z = ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height +
                 ground_particle_scatter_reference_height_offset_gp;
        if (pass_attempt_count > 0) {
            float negative_spread = -spread;
            remaining_pass_attempts = pass_attempt_count;
            do {
                float u;
                float t;
                origin_delta_or_spawn_vector.x = ground_particle_scatter_spawn_vector_xy_bias[0] +
                                                 func_002140F8_3091B0(negative_spread, spread);
                u = func_002140F8_3091B0(negative_spread, spread);
                t = hi * frame_time;
                *(int *)&origin_delta_or_spawn_vector.w = 0;
                *(u128_3091B0 *)&pt = *(u128_3091B0 *)&base;
                origin_delta_or_spawn_vector.y = ground_particle_scatter_spawn_vector_y_bias + u;
                origin_delta_or_spawn_vector.z = -t;
                w = fast_cos(func_002140F8_3091B0(0.0f, 6.2831855f));
                w = w * func_002140F8_3091B0(0.0f, 8.0f);
                pt.x += w;
                w = fast_sin(func_002140F8_3091B0(0.0f, 6.2831855f));
                w = w * func_002140F8_3091B0(0.0f, 8.0f);
                pt.y += w;
                D_L08_00160260 =
                    ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height -
                    ground_particle_scatter_ground_height_floor_offset_gp;
                ground_height = ground_height_at(&pt);
                if (ground_height <
                    ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height +
                        ground_particle_scatter_reference_height_offset_gp) {
                    if (ground_height < D_L08_00160260) {
                        w = D_L08_00160260;
                    } else {
                        w = ground_height;
                    }
                    if (flag == 0)
                        func_L08_00273A80_3091B0(&pt, 0, w, &origin_delta_or_spawn_vector);
                    if (flag == 1)
                        func_L08_002803B8_3091B0(&pt, 0, w, &origin_delta_or_spawn_vector);
                }
            } while (--remaining_pass_attempts != 0);
        }
        if (big != 0) {
            int sign;
            int h;
            w = 30.0f;
            *(u128_3091B0 *)&base = *(u128_3091B0 *)D_L08_00167640_3091B0;
            fast_add_rotations_cf(&base,
                                  &base,
                                  w,
                                  ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->f158,
                                  ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->f154);
            sign = pass_attempt_count >> 31;
            base.z = ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height +
                     ground_particle_scatter_reference_height_offset_gp;
            if (pass_attempt_count > 0) {
                remaining_pass_attempts = pass_attempt_count;
                do {
                    *(u128_3091B0 *)&pt = *(u128_3091B0 *)&base;
                    w = fast_cos(func_002140F8_3091B0(0.0f, 6.2831855f));
                    w = w * func_002140F8_3091B0(0.0f, 30.0f);
                    pt.x += w;
                    w = fast_sin(func_002140F8_3091B0(0.0f, 6.2831855f));
                    w = w * func_002140F8_3091B0(0.0f, 30.0f);
                    pt.y += w;
                    spawn_ground_contact_effect_particle(&pt);
                } while (--remaining_pass_attempts != 0);
            }
            *(u128_3091B0 *)&base = *(u128_3091B0 *)D_L08_00167640_3091B0;
            fast_add_rotations_cf(&base,
                                  &base,
                                  4.0f,
                                  ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->f158,
                                  ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->f154);
            base.z = ((GroundScatterCamera *)(D_L08_00167640_3091B0 - 0x140))->reference_height +
                     ground_particle_scatter_reference_height_offset_gp;
            h = (pass_attempt_count - sign) >> 1;
            if (h > 0) {
                remaining_pass_attempts = h;
                do {
                    *(u128_3091B0 *)&pt = *(u128_3091B0 *)&base;
                    w = fast_cos(func_002140F8_3091B0(0.0f, 6.2831855f));
                    w = w * func_002140F8_3091B0(0.0f, 8.0f);
                    pt.x += w;
                    w = fast_sin(func_002140F8_3091B0(0.0f, 6.2831855f));
                    w = w * func_002140F8_3091B0(0.0f, 8.0f);
                    pt.y += w;
                    spawn_ground_contact_effect_particle(&pt);
                } while (--remaining_pass_attempts != 0);
            }
        }
        break;
    }

    qcopy(&moby_data->v30, D_L08_00167640_3091B0);
}

float D_L12_00162020[2] = {0};

short D_L12_0016200C = 0;
short D_L12_00162010 = 0;
float D_L12_00160260 = 0;
float D_L12_00162000 = 0.5f;
float D_L12_00162004 = 1e+01f;
float D_L12_00162008 = 18.0f;
float D_L12_00162014 = -1.0f;
int   D_L12_00162018 = 1;
float D_L12_0016201C = 5.0f;
short D_L14_0016221C = 0;
short D_L14_00162220 = 0;
float D_L14_00160260 = 0;
float D_L14_00162210 = 0.5f;
float D_L14_00162214 = 1e+01f;
float D_L14_00162218 = 18.0f;
float D_L14_00162224 = -1.0f;
int   D_L14_00162228 = 1;
float D_L14_0016222C = 5.0f;

float ground_particle_scatter_spawn_vector_xy_bias[2] __asm__("D_L08_00162360") = {0};

int ground_particle_scatter_distant_spawn_argument_gp __asm__("D_L08_00162358") = 1;
