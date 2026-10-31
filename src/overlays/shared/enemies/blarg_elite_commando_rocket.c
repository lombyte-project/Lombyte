/* Blarg elite commando rocket (class 0x31, Wrench name rocket): the rocket the elite commando fires, its trail sparks and detonation glow, and the function that spawns it. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/world/collision/collision_line.h"
#include "sda.h"
#include "rnc/overlay/shared/enemies/blarg_elite_commando_rocket_data/spawn_rocket_trail_spark.h"

extern int   random_integer_below(int);
extern int   scale_game_frames(int);
extern float scale_time(float) __asm__("FUN_001f96b0");

extern char  D_L15_001673C0_y[] __asm__("D_L15_001673C0");
extern int   D_L15_00174458[];
extern float D_0015ED6C;
extern void  make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
void         spawn_rocket_trail_spark(char *moby) __asm__("FUN_L15_0029ae18");

void spawn_rocket_detonation_glow(char *moby) __asm__("FUN_L15_0029af88");

extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int rocket_trail_spark_initial_configuration_word_gp __asm__("D_L15_00161418")
    __attribute__((sda));
extern float rocket_trail_spark_frame_count_range_start_gp __asm__("D_L15_00161428")
    __attribute__((sda));
extern float rocket_trail_spark_third_frame_count_range_start_gp __asm__("D_L15_00161438")
    __attribute__((sda));
extern float rocket_trail_spark_first_velocity_scale_gp __asm__("D_L15_00161448")
    __attribute__((sda));
extern int rocket_trail_spark_second_configuration_word_gp __asm__("D_L15_0016141C")
    __attribute__((sda));

extern float rocket_trail_spark_frame_count_range_end_gp __asm__("D_L15_0016142C")
    __attribute__((sda));
extern float rocket_trail_spark_second_frame_count_range_start_gp __asm__("D_L15_00161430")
    __attribute__((sda));
extern float rocket_trail_spark_second_frame_count_range_end_gp __asm__("D_L15_00161434")
    __attribute__((sda));
extern float rocket_trail_spark_third_frame_count_range_end_gp __asm__("D_L15_0016143C")
    __attribute__((sda));
extern float rocket_trail_spark_first_noise_radius_factor_gp __asm__("D_L15_00161440")
    __attribute__((sda));
extern float rocket_trail_spark_second_noise_radius_factor_gp __asm__("D_L15_00161444")
    __attribute__((sda));
extern float rocket_trail_spark_second_velocity_scale_gp __asm__("D_L15_0016144C")
    __attribute__((sda));
extern int rocket_trail_spark_lookup_index_and_flags_gp __asm__("D_L15_00161454")
    __attribute__((sda));
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");

extern float D_L15_0015F580[] MACRO_ADDR;
extern void  spawn_glow_particle(char *, char *, int, int, float, int, int, int, int) __asm__("FUN_L00_002ac910");

extern float rocket_detonation_glow_scale_gp __asm__("D_L15_00161458") __attribute__((sda));
extern int rocket_detonation_glow_lifetime_frames_gp __asm__("D_L15_0016145C") __attribute__((sda));
extern unsigned char l15_rocket_detonation_glow_alpha __asm__("D_L15_00161460")
    __attribute__((sda));
extern unsigned char l15_rocket_detonation_glow_red __asm__("D_L15_00161464") __attribute__((sda));
extern unsigned char l15_rocket_detonation_glow_green __asm__("D_L15_00161468")
    __attribute__((sda));
extern unsigned char l15_rocket_detonation_glow_blue __asm__("D_L15_0016146C") __attribute__((sda));

/* Flies the moby along its velocity, spinning; it dies with a sound when it hits something or times out. */

void blarg_elite_commando_rocket_update(unsigned char *m) __asm__("FUN_L15_0029ac30");

/* Moby class 49 */
void blarg_elite_commando_rocket_update(unsigned char *moby) {
    char *rocket_data = ((char *)MOBY(moby)->pvars);
    float rocket_next_position[4];
    float back[4];
    char  col[0x30];
    float collision_setup_value;
    if (*(int *)(rocket_data + 0x10) == 0) {
        if (fast_vec_dist(moby + 0x10, D_L15_001673C0_y) < 15.0f) {
            trooper_play_sound(9, 0, moby, 0x27E);
            *(int *)(rocket_data + 0x10) = 1;
        }
    }
    if (moby[0x20] != 0)
        return;
    collision_setup_value = 1.0f;
    MOBY(moby)->rot.x = fast_add_rotations(MOBY(moby)->rot.x, D_0015ED6C * 6.2831855f);
    fast_vec_add(rocket_next_position, moby + 0x10, rocket_data);
    fast_vec_scale(back, rocket_data, -2.0f);
    fast_vec_add(back, back, moby + 0x10);
    if (hero.vehicle_mode == 2 && random_integer_below(5) != 0)
        collision_setup_value = 0.0f;
    make_hit_descriptor(col, moby, 0x10003, collision_setup_value, rocket_data);
    *(short *)(col + 0x1A) = MOBY(moby)->oclass;
    if (collision_line(back, rocket_next_position, 0x10, moby, col) != 0 ||
        fast_dec_timer(rocket_data + 0x14) != 0) {
        if (D_L15_00174458[0] != *(int *)(rocket_data + 0x18)) {
            trooper_play_sound(8, 0, moby, 0x27E);
            spawn_rocket_detonation_glow((char *)moby);
            mark_moby_for_removal(moby);
            return;
        }
    }
    qcopy(moby + 0x10, rocket_next_position);
    spawn_rocket_trail_spark((char *)moby);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029BFF8), where it is exact; names translated to the US level program. */

void spawn_rocket_trail_spark(char *moby) {
    float noise[4];
    float spark_a[4];
    float spark_b[4];
    char *rocket_velocity = ((char *)MOBY(moby)->pvars);
    float noise_radius;
    int   frame_count, second_frame_count, third_frame_count;
    noise_radius = rocket_trail_spark_first_noise_radius_factor_gp;
    noise_radius *= D_0015ED6C;
    fast_vec_scale(spark_a, rocket_velocity, rocket_trail_spark_first_velocity_scale_gp);
    random_vector(noise, noise_radius, noise_radius);
    fast_vec_add(spark_a, spark_a, noise);
    noise_radius = rocket_trail_spark_second_noise_radius_factor_gp;
    noise_radius *= D_0015ED6C;
    fast_vec_scale(spark_b, rocket_velocity, rocket_trail_spark_second_velocity_scale_gp);
    random_vector(noise, noise_radius, noise_radius);
    fast_vec_add(spark_b, spark_b, noise);
    spark_a[3] = rocket_trail_spark_first_vector_fourth_component_gp;
    spark_b[3] = rocket_trail_spark_second_vector_fourth_component_gp;
    frame_count = truncate_float_to_s32(
        scale_time(random_float_between(rocket_trail_spark_frame_count_range_start_gp,
                                        rocket_trail_spark_frame_count_range_end_gp)));
    second_frame_count = truncate_float_to_s32(
        scale_time(random_float_between(rocket_trail_spark_second_frame_count_range_start_gp,
                                        rocket_trail_spark_second_frame_count_range_end_gp)));
    third_frame_count = truncate_float_to_s32(
        scale_time(random_float_between(rocket_trail_spark_third_frame_count_range_start_gp,
                                        rocket_trail_spark_third_frame_count_range_end_gp)));
    spawn_spark(moby + 0x10,
                spark_a,
                spark_b,
                rocket_trail_spark_initial_configuration_word_gp,
                rocket_trail_spark_second_configuration_word_gp,
                frame_count,
                second_frame_count,
                third_frame_count,
                rocket_trail_spark_lookup_index_and_flags_gp);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_0029C168), where it is exact; names translated to the US level program. */

void spawn_rocket_detonation_glow(char *moby) {
    char *glow_spawn_position = moby + 0x10;
    int   scaled_frame_count = scale_game_frames(rocket_detonation_glow_lifetime_frames_gp);
    spawn_glow_particle(moby,
                        glow_spawn_position,
                        (int)&D_L15_0015F580,
                        scaled_frame_count,
                        rocket_detonation_glow_scale_gp,
                        l15_rocket_detonation_glow_red,
                        l15_rocket_detonation_glow_green,
                        l15_rocket_detonation_glow_blue,
                        l15_rocket_detonation_glow_alpha);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002A1B58), where it is exact; names translated to the US level program. */
char *spawn_blarg_elite_commando_rocket(char  *owner,
                                        float *initial_movement_vector,
                                        char  *spawn_position) __asm__("FUN_L16_002a09a0");

char *spawn_blarg_elite_commando_rocket(char  *firing_commando_moby,
                                        float *initial_movement_vector,

                                        char *spawn_position) {
    char *moby = create_moby(0x31);
    if (moby != 0) {
        char *spawned_rocket_data;
        float xy_length;
        float elevation_angle;
        ((unsigned char *)moby)[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        MOBY(moby)->spawn_frame = *(unsigned long *)(firing_commando_moby + 0x38);
        MOBY(moby)->scale = MOBY(moby)->scale * 3.0f;
        qcopy(moby + 0x10, spawn_position);
        spawned_rocket_data = ((char *)MOBY(moby)->pvars);
        qcopy(spawned_rocket_data, initial_movement_vector);
        *(int *)(spawned_rocket_data + 0x14) = scale_game_frames(0xF0);
        *(char **)(spawned_rocket_data + 0x18) = firing_commando_moby;
        *(int *)(spawned_rocket_data + 0x10) = 0;
        MOBY(moby)->rot.z = fast_arc_tan(initial_movement_vector[0], initial_movement_vector[1]);
        xy_length = vector_length_xy(initial_movement_vector);
        elevation_angle = fast_arc_tan(xy_length, initial_movement_vector[2]);
        MOBY(moby)->rot.y = -elevation_angle;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
    }
    return moby;
}

float         D_L16_001613C8 = 0.5f;
int           D_L16_001613CC = 0xf;
unsigned char D_L16_001613D0 = 0x60;
unsigned char D_L16_001613D4 = 0x90;
unsigned char D_L16_001613D8 = 0xc0;
unsigned char D_L16_001613DC = 0;
int           D_L17_001613B8 = 0x8000c070;
float         D_L17_001613C8 = 2.0f;
float         D_L17_001613D8 = 5.0f;
float         D_L17_001613E8 = 0;
int           D_L17_001613BC = 0x4000c0c0;
float         D_L17_001613CC = 2.0f;
float         D_L17_001613D0 = 5.0f;
float         D_L17_001613D4 = 1e+01f;
float         D_L17_001613DC = 1e+01f;
float         D_L17_001613E0 = 0.25f;
float         D_L17_001613E4 = 2.0f;
float         D_L17_001613EC = 0;
int           D_L17_001613F4 = 0x18;
float         D_L17_001613F8 = 0.5f;
int           D_L17_001613FC = 0xf;
unsigned char D_L17_00161400 = 0x60;
unsigned char D_L17_00161404 = 0x90;
unsigned char D_L17_00161408 = 0xc0;
unsigned char D_L17_0016140C = 0;
float         D_L18_001613C8 = 0.5f;
int           D_L18_001613CC = 0xf;
unsigned char D_L18_001613D0 = 0x60;
unsigned char D_L18_001613D4 = 0x90;
unsigned char D_L18_001613D8 = 0xc0;
unsigned char D_L18_001613DC = 0;

unsigned char l15_rocket_detonation_glow_alpha __asm__("D_L15_00161460") = 0x60;
