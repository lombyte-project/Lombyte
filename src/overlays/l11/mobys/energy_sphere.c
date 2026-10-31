/* Level 11 moby class energy_sphere (class 0x511): spawning, moving and timing the sphere, its particles and sparks. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/world/collision/collision_line.h"

extern float frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00319FA0), where it is exact; names translated to the US level program. */

extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
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
extern void FUN_L11_00319028_c(char *moby) __asm__("FUN_L11_00319028");
extern void FUN_L11_003191c0_c(char *moby) __asm__("FUN_L11_003191c0");

extern short energy_sphere_particle_scale_target_gp __asm__("D_L11_00162388") __attribute__((sda));
void         energy_sphere_update(unsigned char *moby) __asm__("FUN_L11_00318b30");
extern float approach_value(float *p, float target, float maxstep);
extern int   scale_game_frames(int);

/* Moby class 1297 */
void energy_sphere_update(unsigned char *moby) {
    enum {
        ENERGY_SPHERE_APPROACH_SCALE_AND_REMOVE = 1,
        ENERGY_SPHERE_MOVE_AND_DETONATE = 2
    };
    char  *energy_sphere_private_vars = ((char *)MOBY(moby)->pvars);
    float  previous_position[4];
    float *vp;
    FUN_L11_00319028_c((char *)moby);
    switch (moby[0x20]) {
    case ENERGY_SPHERE_APPROACH_SCALE_AND_REMOVE:
        if (fast_dec_timer(energy_sphere_private_vars + 0x20)) {
            float *scale_approach_pointer = ((float *)MOBY(moby)->pclass);
            float  scale_approach_frames = (float)scale_game_frames(0x14);
            float  target_scale = scale_approach_pointer[9];
            float  scale_approach_step = (((float *)MOBY(moby)->pclass))[9] / scale_approach_frames;
            scale_approach_pointer = (float *)(energy_sphere_private_vars + 0x1C);
            approach_value((float *)&MOBY(moby)->scale, target_scale, scale_approach_step);
            approach_value(scale_approach_pointer,
                           *(float *)&energy_sphere_particle_scale_target_gp,
                           *(float *)&energy_sphere_particle_scale_target_gp /
                               (float)scale_game_frames(0x14));
        }
        if (fast_dec_timer(energy_sphere_private_vars + 0x28)) {
            mark_moby_for_removal(moby);
        }
        break;
    case ENERGY_SPHERE_MOVE_AND_DETONATE: {
        char *sphere_position = (char *)moby + 0x10;
        FUN_L11_003191c0_c((char *)moby);
        vp = previous_position;
        qcopy(vp, sphere_position);
        fast_vec_add(sphere_position, sphere_position, energy_sphere_private_vars);
        if (fast_dec_timer(energy_sphere_private_vars + 0x20) ||
            collision_line(vp, (char *)moby + 0x10, 0, moby, 0) ||
            collect_mobies_in_sphere(
                0.333f, (char *)moby + 0x10, 0, *(void **)(energy_sphere_private_vars + 0x24), 0)) {
            spawn_explosion(moby,
                            energy_sphere_private_vars,
                            (char *)moby + 0x10,
                            1.5f,
                            2.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            9.0f,
                            -1,
                            1.0f,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
        }
        break;
    }
    }
}

/* Spawns a spark effect moby from a source moby, initialises its particle bytes and warms it up. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A180), where it is exact; names translated to the US level program. */

extern int FUN_L11_00318ed8_u() __asm__("FUN_L11_00318ed8");

char *spawn_class_511_moby_with_sparks(char *source_moby,
                                       void *pos,
                                       int   ticks) __asm__("FUN_L11_00318d10");

char *spawn_class_511_moby_with_sparks(char *source_moby, void *spawn_position, int ticks) {
    char *mob = create_moby(0x511);
    if (mob != 0) {
        char          *moby_vars;
        unsigned char *initialization_byte_cursor;
        int            i, k, initialization_byte_value;
        *(short *)(mob + 0x32) = 0x40;
        *(unsigned char *)(mob + 0x30) = 0xFF;
        mob[0x31] = 1;
        *(int *)(mob + 0x2C) = 0;
        moby_vars = *(char **)(mob + 0x78);
        *(long *)(mob + 0x38) = ((long)MOBY(source_moby)->spawn_frame);
        mob[0x20] = 1;
        qcopy(mob + 0x10, spawn_position);
        qcopy(mob + 0x40, source_moby + 0x40);
        clear_u64_value(moby_vars);
        *(char **)(moby_vars + 0x24) = source_moby;
        initialization_byte_cursor = (unsigned char *)(moby_vars + 0x14);
        *(int *)(moby_vars + 0x20) = scale_game_frames(ticks);
        *(int *)(moby_vars + 0x28) = 5;
        euler_rotation_basis(mob + 0xC0, mob + 0x40);
        for (i = 0, initialization_byte_value = 10; i < 4;
             i++, initialization_byte_value += 10, initialization_byte_cursor++) {
            initialization_byte_cursor[-4] = random_integer_below(0xFF);
            if (i * (i & 1))
                initialization_byte_cursor[0] = 0xFF;
            else
                initialization_byte_cursor[0] = 1;
            initialization_byte_cursor[4] = initialization_byte_value;
        }
        for (k = 0x31; k >= 0; k--)
            FUN_L11_00318ed8_u(mob);
    }
    return mob;
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A2E8), where it is exact; names translated to the US level program. */

void set_moby_position_and_refresh_removal_timer(struct Moby *moby,
                                                 void *new_position) __asm__("FUN_L11_00318e78");

void set_moby_position_and_refresh_removal_timer(struct Moby *moby, void *new_position) {
    char *moby_vars = (char *)moby->pvars;
    qcopy(&moby->pos, new_position);
    *(int *)(moby_vars + 0x28) = 5;
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A308), where it is exact; names translated to the US level program. */

void set_moby_state_two_data_and_timer(struct Moby *moby, void *src) __asm__("FUN_L11_00318e98");

void set_moby_state_two_data_and_timer(struct Moby *moby, void *movement_step) {
    char *moby_vars;
    moby->state = 2;
    moby_vars = (char *)moby->pvars;
    qcopy(moby_vars, movement_step);
    *(int *)(moby_vars + 0x20) = scale_game_frames(0x3C);
}
/* Builds a few direction vectors from a moby's axes and calls the 0x219780 spawner. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A348), where it is exact; names translated to the US level program. */

extern short moby_radial_spark_initial_word_gp __asm__("D_L11_0016238C") __attribute__((sda));
extern short moby_radial_spark_second_word_gp __asm__("D_L11_00162390") __attribute__((sda));
extern short moby_radial_spark_frame_count_gp __asm__("D_L11_00162394") __attribute__((sda));
extern short moby_radial_spark_second_frame_count_gp __asm__("D_L11_00162398") __attribute__((sda));
extern short moby_radial_spark_third_frame_count_gp __asm__("D_L11_0016239C") __attribute__((sda));
extern short moby_radial_spark_first_vector_w_gp __asm__("D_L11_001623A0") __attribute__((sda));
extern short moby_radial_spark_second_vector_w_gp __asm__("D_L11_001623A4") __attribute__((sda));
extern short spark_lookup_index_and_flags_gp __asm__("D_L11_001623A8") __attribute__((sda));
void         spawn_moby_radial_spark(struct Moby *source_moby) __asm__("FUN_L11_00318ed8");

void spawn_moby_radial_spark(struct Moby *source_moby) {
    float spark_position[4];
    float radial_offset[4];
    float row1_offset[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    float radial_distance = random_float_between(2.5f, 3.0f);
    float radial_angle = random_angle_radians();
    fast_vec_normalize(
        row1_offset, &source_moby->rot_row1, radial_distance * fast_sin(radial_angle));
    fast_vec_normalize(
        radial_offset, &source_moby->rot_row2, radial_distance * fast_cos(radial_angle));
    fast_vec_add(radial_offset, radial_offset, row1_offset);
    fast_vec_sub(spark_position, &source_moby->pos, radial_offset);
    fast_vec_scale(first_spark_vector,
                   radial_offset,
                   1.0f /
                       (float)scale_game_frames(*(int *)&moby_radial_spark_frame_count_gp +
                                                *(int *)&moby_radial_spark_second_frame_count_gp +
                                                *(int *)&moby_radial_spark_third_frame_count_gp));
    qcopy(second_spark_vector, first_spark_vector);
    first_spark_vector[3] = *(float *)&moby_radial_spark_first_vector_w_gp;
    second_spark_vector[3] = *(float *)&moby_radial_spark_second_vector_w_gp;
    spawn_spark(spark_position,
                first_spark_vector,
                second_spark_vector,
                *(int *)&moby_radial_spark_initial_word_gp,
                *(int *)&moby_radial_spark_second_word_gp,
                *(int *)&moby_radial_spark_frame_count_gp,
                *(int *)&moby_radial_spark_second_frame_count_gp,
                *(int *)&moby_radial_spark_third_frame_count_gp,
                *(int *)&spark_lookup_index_and_flags_gp);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A498), where it is exact; names translated to the US level program. */

extern float          D_L11_001677C0_lui[] __asm__("D_L11_001677C0") NOT_SDA;
extern int            FUN_001fa6e0_q(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned char *FUN_L00_00272f68_q(float         f,
                                         void         *pos,
                                         int           a1,
                                         unsigned char a2,
                                         int           idx,
                                         int           flag,
                                         int           s,
                                         int           mode) __asm__("FUN_L00_00272f68");

extern short energy_sphere_particle_color_end_gp __asm__("D_L11_00162384") __attribute__((sda));
extern short energy_sphere_particle_color_start_gp __asm__("D_L11_00162380") __attribute__((sda));
void         spawn_energy_sphere_particles(struct Moby *moby) __asm__("FUN_L11_00319028");

void spawn_energy_sphere_particles(struct Moby *moby) {
    float          effect_position[4];
    float          position_step[4];
    char          *energy_sphere_vars = (char *)moby->pvars;
    unsigned char *effect_byte_cursor;
    float         *moby_position = &moby->pos.x;
    int            i;
    fast_vec_sub(effect_position, D_L11_001677C0_lui, moby_position);
    effect_byte_cursor = (unsigned char *)(energy_sphere_vars + 0x10);
    fast_vec_normalize(effect_position, effect_position, -0.3f);
    fast_vec_normalize(position_step, effect_position, 0.1f);
    fast_vec_add(effect_position, effect_position, moby_position);
    for (i = 0; i < 4; i++) {
        int           a = FUN_001fa6e0_q(*(int *)&energy_sphere_particle_color_start_gp,
                               *(int *)&energy_sphere_particle_color_end_gp,
                               (float)i * 0.333f);
        unsigned char updated_effect_byte;
        effect_byte_cursor[0] += effect_byte_cursor[4];
        updated_effect_byte = truncate_float_to_s32((float)effect_byte_cursor[0]);
        FUN_L00_00272f68_q(*(float *)(energy_sphere_vars + 0x1C) *
                               convert_integer_to_float(effect_byte_cursor[8]) * 0.025f,
                           effect_position,
                           a,
                           updated_effect_byte,
                           0x35,
                           1,
                           2,
                           0);
        effect_byte_cursor++;
        fast_vec_add(effect_position, effect_position, position_step);
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031A630), where it is exact; names translated to the US level program. */

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

extern short energy_sphere_spark_radial_range_start_gp __asm__("D_L11_001623AC")
    __attribute__((sda));
extern short energy_sphere_spark_radial_range_end_gp __asm__("D_L11_001623B0") __attribute__((sda));
extern short energy_sphere_spark_first_vector_w_gp __asm__("D_L11_001623B4") __attribute__((sda));
extern short energy_sphere_spark_second_vector_w_gp __asm__("D_L11_001623B8") __attribute__((sda));
extern short energy_sphere_spark_color_range_start_gp __asm__("D_L11_001623BC")
    __attribute__((sda));
extern short energy_sphere_spark_color_range_end_gp __asm__("D_L11_001623C0") __attribute__((sda));
extern short energy_sphere_spark_frame_count_gp __asm__("D_L11_001623C4") __attribute__((sda));
extern short energy_sphere_spark_second_frame_count_gp __asm__("D_L11_001623C8")
    __attribute__((sda));
extern short energy_sphere_spark_third_frame_count_gp __asm__("D_L11_001623CC")
    __attribute__((sda));
void spawn_energy_sphere_spark(struct Moby *moby) __asm__("FUN_L11_003191c0");

void spawn_energy_sphere_spark(struct Moby *moby) {
    float first_spark_vector[4];
    float second_spark_vector[4];
    int   first_spark_color;
    int   second_spark_color;
    random_vector(first_spark_vector,
                  *(float *)&energy_sphere_spark_radial_range_start_gp * frame_scale_inv_macro[1],
                  *(float *)&energy_sphere_spark_radial_range_end_gp * frame_scale_inv_macro[1]);
    random_vector(second_spark_vector,
                  *(float *)&energy_sphere_spark_radial_range_start_gp * frame_scale_inv_macro[1],
                  *(float *)&energy_sphere_spark_radial_range_end_gp * frame_scale_inv_macro[1]);
    first_spark_vector[3] = *(float *)&energy_sphere_spark_first_vector_w_gp;
    second_spark_vector[3] = *(float *)&energy_sphere_spark_second_vector_w_gp;
    first_spark_color = fast_tween_color(*(int *)&energy_sphere_spark_color_range_start_gp,
                                         *(int *)&energy_sphere_spark_color_range_end_gp,
                                         random_float_between(0.0f, 1.0f));
    second_spark_color = fast_tween_color(*(int *)&energy_sphere_spark_color_range_start_gp,
                                          *(int *)&energy_sphere_spark_color_range_end_gp,
                                          random_float_between(0.0f, 1.0f));
    spawn_spark(&moby->pos,
                first_spark_vector,
                second_spark_vector,
                first_spark_color,
                second_spark_color,
                *(int *)&energy_sphere_spark_frame_count_gp,
                *(int *)&energy_sphere_spark_second_frame_count_gp,
                *(int *)&energy_sphere_spark_third_frame_count_gp,
                *(int *)&spark_lookup_index_and_flags_gp);
}
