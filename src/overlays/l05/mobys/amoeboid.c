/* Level 05 moby class amoeboid. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/mobys/animation/blend_moby_animation.h"

extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
extern int   random_integer_below(int) __asm__("FUN_00213260");
extern float fast_cos(float);
extern float fast_sin(float);
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DBF10), where it is exact; names translated to the US level program. */

extern float FUN_L00_0025be00(float *, float *, float, float, float, float);
extern int   FUN_L00_00258b50(void *, void *, void *, void *, float);
extern void  FUN_L01_0026d930(char *p);
extern void  handle_amoeboid_hits(unsigned char *m) __asm__("FUN_L05_002daf58");
extern void  spawn_amoeboid_particles(unsigned char *moby) __asm__("FUN_L05_002db018");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");

void                amoeboid_update(struct Moby *m) __asm__("FUN_L05_002dac80");
typedef struct Moby Moby;

/* Moby class 133 */
void amoeboid_update(struct Moby *moby) {
    enum {
        AMOEBOID_INITIALIZE = 0,
        AMOEBOID_FOLLOW_PATH = 1,
        AMOEBOID_WAIT_FOR_HERO_TO_LEAVE = 2
    };
    char *moby_data = (char *)moby->pvars;
    float facing_direction[4];
    handle_amoeboid_hits(moby);
    switch (moby->state) {
    case AMOEBOID_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0x40;
        if (moby->prev_seq != 0)
            blend_moby_animation(moby, 0, 0, 0);
        if (*(int *)(moby_data + 0xB0) == -1 ||
            *(int *)l05_paths[*(int *)(moby_data + 0xB0)] == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        FUN_L01_0026d930(moby_data + 0x60);
        *(float *)(moby_data + 0x6C) = 1.0f;
        *(float *)(moby_data + 0x68) = 1.0f;
        {
            int   r = truncate_float_to_s32(409.6f);
            float movement_speed_limit = frame_time * 4.0f;
            *(int *)(moby_data + 0x60) = r;
            *(float *)(moby_data + 0x84) = movement_speed_limit;
        }
        break;
    case AMOEBOID_FOLLOW_PATH: {
        char *path_node_base =
            ((char *)l05_paths[*(int *)(moby_data + 0xB0)]) + *(int *)(moby_data + 0xB4) * 16;
        float yaw = fast_arc_tan(*(float *)(path_node_base + 0x10) - moby->pos.x,
                                 *(float *)(path_node_base + 0x14) - moby->pos.y);
        FUN_L00_0025be00(&moby->rot.z,
                         (float *)(moby_data + 0xBC),
                         yaw,
                         frame_time_sq * 1.0471976f,
                         frame_time_sq * 1.0471976f,
                         frame_time * 3.1415927f);
        facing_direction[0] = fast_cos(moby->rot.z);
        facing_direction[1] = fast_sin(moby->rot.z);
        facing_direction[2] = 0.0f;
        FUN_L00_00258b50(moby, moby_data + 0x60, facing_direction, moby_data + 0x40, 1.0f);
        if (fast_dec_timer(moby_data + 0xB8) != 0 ||
            vector_distance_xy(&moby->pos,
                               ((char *)l05_paths[*(int *)(moby_data + 0xB0)]) +
                                   (*(int *)(moby_data + 0xB4) * 16 + 0x10)) < 1.0f) {
            int r = random_integer_below(*(int *)l05_paths[*(int *)(moby_data + 0xB0)]);
            *(int *)(moby_data + 0xB4) =
                (*(int *)(moby_data + 0xB4) + r) % *(int *)l05_paths[*(int *)(moby_data + 0xB0)];
            *(int *)(moby_data + 0xB8) = scale_game_frames(600);
        }
        spawn_amoeboid_particles(moby);
        break;
    }
    case AMOEBOID_WAIT_FOR_HERO_TO_LEAVE:
        if (48.0f < vector_distance_xy(&moby->pos, &hero.motion.pos)) {
            moby->state = 1;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            moby->collision = moby->pclass->collision;
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC1E8), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char  D_0013E533[];
extern void  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  spawn_amoeboid_hit_sparks(void *, void *) __asm__("FUN_L05_002db238");

void handle_amoeboid_hits(unsigned char *moby) __asm__("FUN_L05_002daf58");

void handle_amoeboid_hits(unsigned char *moby) {
    char *hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0) {
        char *base_spark_vector = D_0013E533 + 0xF1D;
        char *hit_owner;
        moby[0x20] = 2;
        moby[0x31] = 0;
        MOBY(moby)->collision = 0;
        MOBY(moby)->flags |= 1;
        spawn_amoeboid_hit_sparks(moby, base_spark_vector);
        allocate_voice_for_target_entry(0, 0, (int)moby);
        hit_owner = *(char **)(hit_record + 0x20);
        if (hit_owner != 0 && *(short *)(hit_owner + 0xA6) == 0) {
            int   scaled_frame_increment = scale_game_frames(0x3C);
            char *y = base_spark_vector - 0x100;
            *(int *)(y + 0x8C4) += scaled_frame_increment;
        }
        moby[0xA4] = 0xFF;
    }
}

extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float random_angle_radians(void) __asm__("FUN_00213308");
/* Emits up to two particle effects around the moby, each in a random direction. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC2A8), where it is exact; names translated to the US level program. */

extern void
spawn_type_52_particle(float *, float, float, int, int, int) __asm__("FUN_L01_00287158");

void spawn_amoeboid_particles(unsigned char *moby) {
    float particle_spawn_position[3];
    if (moby[0x31] != 0) {
        if (random_integer_below(0x13) == 0) {
            float a = random_angle_radians();
            float z = 0.0f;
            float b = random_float_between(z, 0.1f);
            float c = random_float_between(z, 0.5f);
            float e = random_float_between(0.5f, 1.0f);
            int   s = truncate_float_to_s32(random_float_between(120.0f, 180.0f));
            particle_spawn_position[0] = fast_cos(a) * b;
            particle_spawn_position[1] = fast_sin(a) * b;
            particle_spawn_position[2] = z;
            fast_vec_add(particle_spawn_position, particle_spawn_position, moby + 0x10);
            spawn_type_52_particle(
                particle_spawn_position, c, e, 0x30002028, 0x2020, scale_game_frames(s));
        }
        if (random_integer_below(4) == 0) {
            float a = random_angle_radians();
            float z = 0.0f;
            float b = random_float_between(z, 0.1f);
            float c = random_float_between(z, 1.75f);
            float e = random_float_between(1.75f, 2.25f);
            int   s = truncate_float_to_s32(random_float_between(120.0f, 180.0f));
            particle_spawn_position[0] = fast_cos(a) * b;
            particle_spawn_position[1] = fast_sin(a) * b;
            particle_spawn_position[2] = z;
            fast_vec_add(particle_spawn_position, particle_spawn_position, moby + 0x10);
            spawn_type_52_particle(
                particle_spawn_position, c, e, 0x20002020, 0x2030, scale_game_frames(s));
        }
    }
}

extern void  fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
/* Explosion debris: 20 bursts thrown out along dir with random spread, each trailing 10 smoke puffs. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_002D28D0.c: func_L05_002DC4C8), where it is exact; names translated to the US level program. */

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");

void spawn_amoeboid_hit_sparks(void *moby, void *base_spark_vector) {
    float spark_position[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    char *moby_position_ptr = (char *)moby + 0x10;
    int   i, j;
    for (i = 0; i < 20; i++) {
        float spark_xy_spread_heading, sp, up;
        qcopy(spark_position, moby_position_ptr);
        add_random_offset_to_vector_xyz(spark_position, 0.25f);
        spark_position[2] += 1.0f;
        spark_xy_spread_heading = random_angle_radians();
        sp = random_float_between(1.0f, 8.0f);
        up = random_float_between(2.0f, 6.0f);
        fast_vec_scale(first_spark_vector, base_spark_vector, random_float_between(0.0f, 1.0f));
        first_spark_vector[0] += fast_cos(spark_xy_spread_heading) * (sp * frame_time);
        first_spark_vector[1] += fast_sin(spark_xy_spread_heading) * (sp * frame_time);
        first_spark_vector[2] += up * frame_time;
        qcopy(second_spark_vector, first_spark_vector);
        second_spark_vector[2] -= frame_time_sq * 20.0f * (float)scale_game_frames(0x1E);
        for (j = 0; j < 10; j++) {
            float spark_vector_w_scale = random_float_between(0.5f, 1.5f);
            int first_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count;
            first_spark_vector[3] = spark_vector_w_scale * 0.125f;
            second_spark_vector[3] = spark_vector_w_scale * 0.065f;
            add_random_offset_to_vector_xyz(spark_position, 0.2f);
            add_random_offset_to_vector_xyz(second_spark_vector, frame_time * 0.5f);
            first_spark_color =
                fast_tween_color(0x8000EEEE, 0x8000FF90, random_float_between(0.0f, 1.0f));
            second_spark_color = fast_tween_color(0xFFEE, 0xFFEE, random_float_between(0.0f, 1.0f));
            spark_frame_count = scale_game_frames(10);
            second_spark_frame_count = scale_game_frames(0x1E);
            spawn_spark(spark_position,
                        first_spark_vector,
                        second_spark_vector,
                        first_spark_color,
                        second_spark_color,
                        spark_frame_count,
                        second_spark_frame_count,
                        truncate_float_to_s32(scale_time(random_float_between(5.0f, 25.0f))),
                        -1);
        }
    }
}
