/* Level 14 bouncing spark moby class and its spawn helper. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/camera/update_cam.h"

typedef struct {
    char  pad0[0x10];
    Vec4f vel;       /* w: gravity while bouncing, then the rolling speed */
    int   fade;      /* 0x20 */
    int   owner;     /* 0x24 */
    short trail;     /* 0x28 */
    short bounces;   /* 0x2A */
    float fade_rate; /* 0x2C */
} Spark_0e00;

typedef struct {
    char  pad0[0x18];
    int   owner; /* 0x18 */
    int   pad1C;
    Vec4f hit; /* 0x20 */
    Vec4f pad30;
    Vec4f normal; /* 0x40 */
} Coll_0e00;

typedef struct {
    float r, g, b;
} Rgb_0e00;

extern Coll_0e00 D_L14_00174540;

extern int D_L14_00174558 NOT_SDA;

extern int D_L14_0016208C_gp __asm__("D_L14_0016208C") __attribute__((sda));

#include "rnc/overlay/l14/mobys/bouncing_spark_globals.h"

extern float spark_contact_clearance_gp __asm__("D_L14_00162088") __attribute__((sda));

extern float spark_minimum_speed_gp __asm__("D_L14_00162090") __attribute__((sda));

extern float spark_rolling_speed_factor_gp __asm__("D_L14_00162094") __attribute__((sda));

extern float D_L14_001620A0 __attribute__((sda));

extern Rgb_0e00 l14_spark_rolling_fade_source_rgb __asm__("D_L14_001620A8");

extern char D_L14_0015F580[] MACRO_ADDR;

extern int coll_sphere(void *, int, void *, float) __asm__("FUN_L00_001f0d60");

extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");

extern char *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

extern int collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");

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

void update_bouncing_spark(struct Moby *spark_moby) __asm__("FUN_L14_00300e00");

void update_bouncing_spark(struct Moby *spark_moby) {
    enum {
        SPARK_COUNT_NEXT_BOUNCE = 0,
        SPARK_WAIT_FOR_DESCENT = 1,
        SPARK_ROLL_AND_FADE = 2,
        SPARK_BURST_AND_REMOVE = 3
    };
    Spark_0e00 *spark_data = (Spark_0e00 *)spark_moby->pvars;
    Vec4f       next;
    Vec4f       position_scratch;
    Vec4f       from;
    float       speed;
    float       lo;
    float       hi;
    if (((coll_sphere(&spark_moby->pos, 0, spark_moby, 0.65f) != 0) && (D_L14_00174558 != 0)) &&
        (D_L14_00174558 != spark_data->owner)) {
        spark_moby->state = 3;
    }
    if (fast_dec_timer(&spark_data->fade) != 0) {
        spark_moby->state = 3;
    }
    switch (spark_moby->state) {
    case SPARK_COUNT_NEXT_BOUNCE:

    case SPARK_WAIT_FOR_DESCENT: {
        fast_vec_add(&next, &spark_moby->pos, &spark_data->vel);
        spark_data->vel.z -= spark_data->vel.w;
        if ((spark_moby->state == 1) && (spark_data->vel.z < 0.0f)) {
            spark_moby->state = 0;
        }
        fast_dec_timer_s16(&spark_data->trail);
        if (spark_data->trail != 0) {
            int i;
            for (i = 0; i < 3; i++) {
                float one = 1.0f;
                float third = 0.3333f;
                int   spin;
                char *trail_particle;
                *((u128 *)(&from)) = *((u128 *)(&spark_moby->pos));
                interpolate_vector(&position_scratch, &from, &next, ((float)i) * third);
                spin = random_integer_below(6);
                if (random_integer_below(2) != 0) {
                    spin = -spin;
                }
                trail_particle =
                    spawn_puff(&position_scratch,
                               0.025f,
                               one,
                               one,
                               spin,
                               D_L14_0015F580,
                               ((((float)spark_data->trail) /
                                 convert_integer_to_float(scale_game_frames(D_L14_0016208C_gp))) *
                                45000.0f) +
                                   5000.0f,
                               0x80808080);
                if (trail_particle != 0) {
                    char *trail_particle_subrecord = trail_particle + 0x20;
                    *((short *)(trail_particle + 0xA)) = scale_game_frames(0x78);
                    *((int *)(trail_particle_subrecord + 4)) = 2;
                    trail_particle_subrecord[0xA] = 0x7F;
                    trail_particle_subrecord[0xB] = trail_particle[0xA];
                }
            }
        }
        if ((collision_line(&spark_moby->pos, &next, 0, spark_moby, 0) != 0) &&
            ((D_L14_00174540.owner == 0) || (D_L14_00174540.owner != spark_data->owner))) {
            qcopy(&spark_moby->pos, &D_L14_00174540.hit);
        } else {
            qcopy(&spark_moby->pos, &next);
        }
        if (((coll_sphere(&spark_moby->pos, 0, spark_moby, spark_contact_clearance_gp) != 0) &&
             ((spark_moby->state == 0) || ((spark_moby->pos.z - D_L14_00174540.hit.z) < 0.0f))) &&
            ((D_L14_00174540.owner == 0) || (D_L14_00174540.owner != spark_data->owner))) {
            fast_vec_normalize(
                &position_scratch, &D_L14_00174540.normal, spark_contact_clearance_gp);
            fast_vec_add(&spark_moby->pos, &D_L14_00174540.hit, &position_scratch);
            reflect_vector(&spark_data->vel, &spark_data->vel, &D_L14_00174540.normal);
            allocate_voice_for_target_entry(1, 0, spark_moby);
            if (spark_moby->state == 0) {
                spark_moby->state = 1;
                if ((++spark_data->bounces) == 1) {
                    set_vector_length_xy(&spark_data->vel,
                                         &spark_data->vel,
                                         spark_first_bounce_horizontal_speed_gp * frame_time);
                    spark_data->vel.z = spark_data->vel.z * D_L14_001620A0;
                } else {
                    fast_vec_scale(&from, &spark_data->vel, D_L14_001620A0);
                    if (vector_length_xy(&from) < (spark_minimum_speed_gp * frame_time)) {
                        set_vector_length_xy(&spark_data->vel,
                                             &spark_data->vel,
                                             spark_minimum_speed_gp * frame_time);
                        spark_data->vel.z *= D_L14_001620A0;
                    } else {
                        qcopy(&spark_data->vel, &from);
                    }
                }
            }
            if (spark_data->bounces >= 4) {
                float min;
                spark_moby->state = 2;
                spark_data->vel.z = 0.0f;
                spark_data->vel.w = vector_length_xy(&spark_data->vel);
                spark_data->vel.w *= D_L14_001620A0;
                min = spark_minimum_speed_gp * frame_time;
                if (spark_data->vel.w < min) {
                    spark_data->vel.w = min;
                }
                fast_vec_normalize(&spark_data->vel, &spark_data->vel, spark_data->vel.w);
                spark_data->fade_rate = 1.0f / convert_integer_to_float(spark_data->fade);
            }
        }
        break;
    }

    case SPARK_ROLL_AND_FADE: {
        float fade_fraction;
        int   r;
        int   g;
        int   b;
        float min;
        *((Rgb_0e00 *)(&next)) = l14_spark_rolling_fade_source_rgb;
        fade_fraction = ((float)spark_data->fade) * spark_data->fade_rate;
        r = truncate_float_to_s32(((((Rgb_0e00 *)(&next))->r - 255.0f) * fade_fraction) + 255.0f);
        g = truncate_float_to_s32(((((Rgb_0e00 *)(&next))->g - 0.0f) * fade_fraction) + 0.0f);
        b = truncate_float_to_s32(((((Rgb_0e00 *)(&next))->b - 0.0f) * fade_fraction) + 0.0f);
        spark_moby->glow_color = 0xFF000000;
        spark_moby->glow_color = (((b << 16) | spark_moby->glow_color) | (g << 8)) | r;
        min = spark_minimum_speed_gp * frame_time;
        spark_data->vel.w *= ((spark_rolling_speed_factor_gp - 1.0f) * frame_scale) + 1.0f;
        if (spark_data->vel.w < min) {
            spark_data->vel.w = min;
        }
        fast_vec_normalize(&spark_data->vel, &spark_data->vel, spark_data->vel.w);
        fast_vec_add(&spark_moby->pos, &spark_moby->pos, &spark_data->vel);
        break;
    }

    case SPARK_BURST_AND_REMOVE:
        spawn_explosion(spark_moby,
                        &spark_data->vel,
                        &spark_moby->pos,
                        2.0f,
                        1.0f,
                        5,
                        2,
                        4,
                        2.0f,
                        1.0f,
                        9.0f,
                        0,
                        1.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(spark_moby);
        return;
    }

    if (spark_moby->state < 2) {
        speed = vector_length_xy(&spark_data->vel);
    } else if (spark_moby->state == 2) {
        speed = spark_data->vel.w;
    } else {
        return;
    }
    lo = frame_time * 0.2f;
    hi = frame_time * 6.0f;
    if (speed < lo) {
        speed = lo;
    }
    if (hi < speed) {
        speed = hi;
    }
    speed -= lo;
    spark_moby->rot.y =
        fast_add_rotations((((speed / (hi - lo)) * 7.0f) + 1.0f) * 0.017453292f, spark_moby->rot.y);
}

extern unsigned short D_L14_0016208C __asm__("D_L14_0016208C") __attribute__((sda));

extern float spark_initial_moby_value_multiplier_gp __asm__("D_L14_00162098") __attribute__((sda));

char *create_bouncing_spark(int    owner,
                            float *pos,
                            float *initial_velocity,
                            int    fade_timer,
                            float  gravity) __asm__("FUN_L14_003014e0");

char *create_bouncing_spark(

    int owner, float *spawn_position, float *initial_velocity, int fade_timer, float gravity) {
    char          *moby = create_moby(0x4A9);
    float          initial_yaw, scaled_moby_value;
    unsigned short initial_trail_timer;
    if (moby != 0) {
        char *spark_data = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(int *)(spark_data + 0x24) = owner;
        qcopy(moby + 0x10, spawn_position);
        qcopy(spark_data + 0x10, initial_velocity);
        initial_trail_timer = D_L14_0016208C;
        *(float *)(spark_data + 0x1C) = gravity;
        *(int *)(spark_data + 0x20) = fade_timer;
        *(unsigned short *)(spark_data + 0x28) = initial_trail_timer;
        *(short *)(spark_data + 0x2A) = 0;
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        initial_yaw = fast_arc_tan(initial_velocity[0], initial_velocity[1]);
        scaled_moby_value = MOBY(moby)->scale * spark_initial_moby_value_multiplier_gp;
        MOBY(moby)->rot.z = initial_yaw;
        MOBY(moby)->scale = scaled_moby_value;
        update_entity(moby);
    }
    return moby;
}

Rgb_0e00 l14_spark_rolling_fade_source_rgb __asm__("D_L14_001620A8") = {67.0f, 2.3e+02f, 132.0f};

unsigned short D_L14_0016208C __asm__("D_L14_0016208C") = 0xc;
