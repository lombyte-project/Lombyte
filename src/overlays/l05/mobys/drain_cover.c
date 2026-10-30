/* Level 05 moby class drain_cover. */
#include "types.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"
#include "asm.h"
#include "sda.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/overlay/l05/mobys/drain_cover_globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

extern char D_0013E550[];
extern int  allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern int  frame_number __asm__("D_L05_0015F5CC");

/* Spawns a particle part (type 0x30) at a position with a velocity, life, size and colour. */
unsigned char *D_L05_001B25C0 __attribute__((section(".data"))) = 0;

unsigned char *create_particle_with_velocity_and_lifetime(float unscaled_particle_scalar,
                                                          void *initial_position_source,
                                                          void *initial_velocity_source,
                                                          int   life,
                                                          int   size,
                                                          int   color) __asm__("FUN_L05_0029bbe0");

unsigned char *create_particle_with_velocity_and_lifetime(float unscaled_particle_scalar,
                                                          void *initial_position_source,
                                                          void *initial_velocity_source,
                                                          int   life,
                                                          int   size,

                                                          int color) {
    unsigned char *allocated_object = create_part(0x30);
    if (allocated_object != 0) {
        unsigned char *velocity_block;
        qcopy(allocated_object + 0x10, initial_position_source);
        velocity_block = allocated_object + 0x20;
        *(int *)(allocated_object + 4) = color & 0xffffff;
        allocated_object[9] = truncate_float_to_s32(2.0f) + 0x10;
        allocated_object[3] = 0x44;
        allocated_object[1] = 0;
        allocated_object[2] = *D_L05_001B25C0;
        *(float *)(allocated_object + 0xc) = unscaled_particle_scalar * 210000.0f;
        allocated_object[8] = random_integer_below(0x100);
        qcopy(velocity_block, initial_velocity_source);
        *(float *)(velocity_block + 0x10) = 2.0f / convert_integer_to_float(life);
        *(short *)(allocated_object + 0xa) = life;
        *(volatile int *)(velocity_block + 0x18) = color;
        *(volatile short *)(velocity_block + 0x1c) = size;
        *(volatile int *)(velocity_block + 0x14) = color & 0xffffff;
    }
    return allocated_object;
}

/* spawn a particle at a position with a velocity and random parameters */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/partupd_0029CA28.c: func_L05_0029CCB8), where it is exact; names translated to the US level program. */

extern unsigned char *D_L05_001B25C8 NOT_SDA;
unsigned char                       *create_particle_with_velocity_and_randomized_parameters(
                          void *initial_position_source,
                          void *initial_velocity_source,
                          int   initial_word_at_04,
                          int   byte_table_index,
                          float unscaled_particle_scalar,
                          float b,
                          float initial_particle_scalar) __asm__("FUN_L05_0029be70");

unsigned char *
create_particle_with_velocity_and_randomized_parameters(void *initial_position_source,
                                                        void *initial_velocity_source,
                                                        int   initial_word_at_04,
                                                        int   byte_table_index,
                                                        float unscaled_particle_scalar,
                                                        float initial_velocity_block_scalar,

                                                        float initial_particle_scalar) {
    unsigned char *allocated_object = create_part(0x32);
    unsigned char *velocity_block;
    if (allocated_object != 0) {
        velocity_block = allocated_object + 0x20;
        qcopy(allocated_object + 0x10, initial_position_source);
        allocated_object[9] = truncate_float_to_s32(1.0f) + 0x40;
        allocated_object[3] = 0x44;
        allocated_object[1] = 0;
        allocated_object[2] = D_L05_001B25C8[byte_table_index];
        *(int *)(allocated_object + 4) = initial_word_at_04;
        *(float *)(allocated_object + 0xC) = unscaled_particle_scalar * 210000.0f;
        allocated_object[8] = (int)random_float_between(0.0f, 255.0f);
        *(short *)(allocated_object + 0xA) = scale_game_frames(0xF);
        qcopy(velocity_block, initial_velocity_source);
        *(float *)(velocity_block + 0x10) = initial_velocity_block_scalar;
        *(int *)(velocity_block + 0x14) = truncate_float_to_s32(random_float_between(-3.0f, 3.0f));
        *(float *)(velocity_block + 0x18) = initial_particle_scalar;
    }
    return allocated_object;
}

void emit_drain_cover_particles(char *, float, float) __asm__("FUN_L05_0030f218");

void         update_drain_cover_tilt(char *, float, float) __asm__("FUN_L05_0030f408");
extern int   voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void
advance_accelerated_scalar(float, float, float, float, void *, void *) __asm__("FUN_00213f38");
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern float l05_drain_cover_gravity __asm__("D_L05_00161DAC") __attribute__((sda));
extern float l05_drain_cover_hover_acceleration __asm__("D_L05_00161DB0") __attribute__((sda));
extern int   D_L05_00161DB8 __asm__("D_L05_00161DB8") __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_height_span __asm__("D_L05_00161DE8")
    __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_radial_speed_lower_bound __asm__("D_L05_00161DEC")
    __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_radial_speed_upper_bound __asm__("D_L05_00161DF0")
    __attribute__((sda));
extern int l05_drain_cover_pre_rise_particle_configuration __asm__("D_L05_00161DF4")
    __attribute__((sda));
extern int   D_L05_00161DF8 __asm__("D_L05_00161DF8") __attribute__((sda));
extern float l05_drain_cover_pre_rise_particle_vertical_speed_upper_bound __asm__("D_L05_00161DFC")
    __attribute__((sda));
extern float l05_drain_cover_nearby_effect_distance_limit __asm__("D_L05_00161E00")
    __attribute__((sda));
extern float D_L05_00161DD4 __asm__("D_L05_00161DD4") __attribute__((sda));
extern float D_0013F3D0[];
extern int   D_L05_0015F5C4;

typedef struct {
    char           pad00[0x10];
    float          pos[4];
    char           pad20[0x10];
    unsigned char  f30;
    unsigned char  f31;
    short          f32;
    unsigned short flags;
} DrainCoverLid;

typedef struct {
    char           pad00[0x20];
    char           f20[0x40];
    float          vel[4];
    float          home[4];
    float          f80;
    float          f84;
    float          f88;
    int            timer;
    char           pad90[8];
    float          f98;
    float          f9C;
    float          fA0;
    float          fA4;
    int            sound;
    DrainCoverLid *lid;
} DrainCoverData;

typedef struct {
    char            pad00[0x10];
    float           pos[4];
    unsigned char   state;
    char            pad21[0x1F];
    float           rot[4];
    char            pad50[0x28];
    DrainCoverData *d;
} DrainCoverMoby;

typedef struct {
    char  pad00[0x80];
    float pos[4];
    char  pad90[0x1FF0];
    int   f2080;
} DrainCoverHero;

void       drain_cover_update(DrainCoverMoby *moby) __asm__("FUN_L05_0030e7f8");
extern int scale_game_frames(int);

void drain_cover_update(DrainCoverMoby *moby) {
    enum {
        DRAIN_COVER_INITIALIZE = 0,
        DRAIN_COVER_WAIT_TO_RISE = 1,
        DRAIN_COVER_RISE = 2,
        DRAIN_COVER_HOVER = 3,
        DRAIN_COVER_FALL_TO_HOME = 4
    };
    DrainCoverData *data = moby->d;
    float           old_position[4];
    float           old_rotation[4];
    float           difference[4];
    float           effect_velocity[4];
    float           effect_position[4];
    float           particle_position[4];
    int             r, g, bl, particle_configuration, b;
    int             i;
    int             index;
    char           *entry;
    DrainCoverHero *hero;
    DrainCoverLid  *lid;
    float           minimum_rise_height;
    float           particle_offset_angle;

    qcopy(old_position, moby->pos);
    qcopy(old_rotation, moby->rot);
    if (moby->state >= 2) {
        if (voice_is_playing(moby, data->sound) == 0) {
            data->sound = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, data->sound) != 0) {
        index = data->sound;
        if (index != -1) {
            entry = D_0013E550 + index * 0x70;
            if (*(DrainCoverMoby **)(entry + 0x88) == moby &&
                *(unsigned char *)(entry + 0x74) != 0) {
                release_voice_slot(index);
            }
        }
        data->sound = -1;
    }
    switch (moby->state) {
    case DRAIN_COVER_INITIALIZE:
        moby->state = 1;
        data->timer = scale_game_frames(0x78);
        data->sound = 0xFF;
        qcopy(data->home, moby->pos);
        data->f98 = random_angle_radians();
        data->f9C = random_angle_radians();
        data->fA0 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
        data->fA4 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
        minimum_rise_height = data->f84 + 1.0f;
        if (data->f80 <= minimum_rise_height) {
            data->f80 = minimum_rise_height;
        }
        break;
    case DRAIN_COVER_WAIT_TO_RISE:
        update_drain_cover_tilt((char *)moby, 0.0f, 8.0f);
        if (D_L05_0015F5C4 == 0 && vector_distance_xy(moby->pos, D_0013F3D0) < 24.0f &&
            fast_dec_timer(&data->timer) != 0) {
            moby->state = 2;
            clear_u64_value(data->vel);
            data->vel[2] =
                square_root(data->f80 * 2.0f * (l05_drain_cover_gravity * frame_time_sq));
        } else if ((float)data->timer < (float)scale_game_frames(*(int *)&D_L05_00161DB8) * 0.5f) {
            emit_drain_cover_particles((char *)moby,
                                       data->home[2],
                                       l05_drain_cover_pre_rise_particle_height_span +
                                           data->home[2]);
            emit_drain_cover_particles((char *)moby,
                                       data->home[2],
                                       l05_drain_cover_pre_rise_particle_height_span +
                                           data->home[2]);
            if ((float)data->timer < (float)scale_game_frames(*(int *)&D_L05_00161DB8) * 0.2f) {
                particle_configuration = l05_drain_cover_pre_rise_particle_configuration;
                b = D_L05_00161DF8;
                r = truncate_float_to_s32(scale_time(random_float_between(10.0f, 20.0f)));
                g = truncate_float_to_s32(scale_time(random_float_between(10.0f, 20.0f)));
                bl = truncate_float_to_s32(scale_time(random_float_between(10.0f, 40.0f)));
                for (i = 0; i < 20; i++) {
                    particle_offset_angle = random_angle_radians();
                    particle_position[0] = fast_cos(particle_offset_angle) * 0.75f;
                    particle_position[1] = fast_sin(particle_offset_angle) * 0.75f;
                    particle_position[2] = 0.0f;
                    fast_vec_normalize(
                        effect_velocity,
                        particle_position,
                        random_float_between(
                            l05_drain_cover_pre_rise_particle_radial_speed_lower_bound,
                            l05_drain_cover_pre_rise_particle_radial_speed_upper_bound) *
                            frame_time);
                    effect_velocity[2] =
                        random_float_between(
                            0.0f, l05_drain_cover_pre_rise_particle_vertical_speed_upper_bound) *
                        frame_time;
                    set_vector_length_xy(effect_position, effect_velocity, 0.0f);
                    fast_vec_add(particle_position, moby->pos, particle_position);
                    particle_position[2] =
                        random_float_between(moby->pos[2] - 0.25f, moby->pos[2] + 0.5f);
                    effect_velocity[3] = effect_position[3] = random_float_between(0.5f, 1.0f);
                    spawn_spark(particle_position,
                                effect_velocity,
                                effect_position,
                                particle_configuration,
                                b,
                                r,
                                g,
                                bl,
                                0x30);
                }
            }
        }
        break;
    case DRAIN_COVER_RISE:
        if ((frame_number & 3) == 0) {
            play_entity_sound((int)moby, 1.0f, moby->pos, 0x10000, 2.0f, 1.0f, 0, 1, 0);
        }
        emit_drain_cover_particles(
            (char *)moby,
            data->home[2],
            data->vel[2] * (float)scale_game_frames(*(int *)&D_L05_00161DB8) * 0.5f + moby->pos[2]);
        update_drain_cover_tilt((char *)moby, D_L05_00161DD4, 1.0f);
        fast_vec_add(moby->pos, moby->pos, data->vel);
        data->vel[2] = data->vel[2] - l05_drain_cover_gravity * frame_time_sq;
        if (data->vel[2] <= 0.0f) {
            moby->state = 3;
            data->home[3] = data->f84 - 0.5f;
            data->timer = truncate_float_to_s32(scale_time(data->f88));
        }
        break;
    case DRAIN_COVER_HOVER:
        emit_drain_cover_particles((char *)moby, data->home[2], moby->pos[2]);
        update_drain_cover_tilt((char *)moby, D_L05_00161DD4, 1.0f);
        advance_accelerated_scalar(data->home[2] + data->home[3],
                                   l05_drain_cover_hover_acceleration * frame_time_sq,
                                   l05_drain_cover_hover_acceleration * frame_time_sq,
                                   frame_time * 20.0f,
                                   &moby->pos[2],
                                   &data->vel[2]);
        if (data->vel[2] == 0.0f) {
            if (data->home[3] > data->f84) {
                data->home[3] = data->f84 - 0.5f;
            } else {
                data->home[3] = data->f84 + 0.5f;
            }
        }
        if (fast_dec_timer(&data->timer) != 0) {
            moby->state = 4;
        }
        break;
    case DRAIN_COVER_FALL_TO_HOME:
        emit_drain_cover_particles(
            (char *)moby,
            data->home[2],
            data->vel[2] * (float)scale_game_frames(*(int *)&D_L05_00161DB8) + moby->pos[2]);
        update_drain_cover_tilt((char *)moby,
                                data->home[2] + 1.0f < moby->pos[2] ? D_L05_00161DD4 : 0.0f,
                                data->home[2] + 1.0f < moby->pos[2] ? 1.0f : 10.0f);
        fast_vec_add(moby->pos, moby->pos, data->vel);
        data->vel[2] = data->vel[2] - l05_drain_cover_gravity * frame_time_sq;
        if (moby->pos[2] < data->home[2]) {
            allocate_voice_for_target_entry(1, 0, moby);
            moby->state = 1;
            data->timer = scale_game_frames(0x78);
            qcopy(moby->pos, data->home);
            data->f98 = random_angle_radians();
            data->f9C = random_angle_radians();
            data->fA0 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
            data->fA4 = randf_sym(70.0f, 120.0f) * 0.017453292f * frame_time;
        }
        break;
    }
    hero = (DrainCoverHero *)(D_0013E550 + 0xE00);
    if (moby->pos[2] - 2.0f > hero->pos[2] &&
        vector_distance_xy(moby->pos, hero->pos) < l05_drain_cover_nearby_effect_distance_limit) {
        effect_velocity[0] =
            fast_cos(fast_arc_tan(hero->pos[0] - moby->pos[0], hero->pos[1] - moby->pos[1])) * 0.2f;
        effect_velocity[1] =
            fast_sin(fast_arc_tan(hero->pos[0] - moby->pos[0], hero->pos[1] - moby->pos[1])) * 0.2f;
        effect_velocity[2] = 0.0f;
        qcopy(effect_position, hero->pos);
        effect_position[2] += 0.75f;
        spawn_nearby_effect((void *)hero->f2080,
                            (int)moby,
                            1,
                            effect_position,
                            effect_velocity,
                            effect_velocity[2]);
    }
    fast_vec_sub(difference, moby->pos, old_position);
    update_entity_matrix(data->f20, difference, old_rotation, moby->rot);
    if (data->lid == 0) {
        data->lid = create_moby(0x388);
        if (data->lid != 0) {
            data->lid->f30 = 0;
            data->lid->flags |= 1;
            data->lid->f32 = 0;
            data->lid->f31 = 0;
            lid = data->lid;
            qcopy(lid->pos, moby->pos);
            update_entity(lid);
        }
    } else {
        lid = data->lid;
        qcopy(lid->pos, moby->pos);
        update_entity(lid);
    }
}
/* Scatters points along a range, placing each one with a heading and radius. */
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_003106E0), where it is exact; names translated to the US level program. */

extern float FUN_L00_00257c48(float lo, float hi);
extern short D_L05_00161DB8_gp __asm__("D_L05_00161DB8") __attribute__((sda));

extern short drain_cover_particle_height_span_per_particle_gp __asm__("D_L05_00161DB4")
    __attribute__((sda));
extern short drain_cover_particle_scalar_lower_bound_gp __asm__("D_L05_00161DBC")
    __attribute__((sda));
extern short drain_cover_particle_scalar_upper_bound_gp __asm__("D_L05_00161DC0")
    __attribute__((sda));
extern short drain_cover_particle_size_sampling_bound_gp __asm__("D_L05_00161DC4")
    __attribute__((sda));
extern short drain_cover_particle_vertical_speed_gp __asm__("D_L05_00161DC8") __attribute__((sda));
extern short drain_cover_particle_color_gp __asm__("D_L05_00161DCC") __attribute__((sda));
extern short drain_cover_particle_spawn_radius_upper_bound_gp __asm__("D_L05_00161DD0")
    __attribute__((sda));
void emit_drain_cover_particles(char *particle_source_moby,
                                float lower_height_bound,

                                float upper_height_bound) {
    float v[4];
    float particle_position[4];
    float ground, x, y, spawn_height_candidate, u;
    int   i, j, k, particle_size, particle_color;
    clear_u64_value(v);
    v[2] = *(float *)&drain_cover_particle_vertical_speed_gp * frame_time;
    ground = v[2] * 0.75f * convert_integer_to_float(*(int *)&D_L05_00161DB8_gp);
    j = 0;
    for (i = j; (float)i < (upper_height_bound - lower_height_bound) /
                               *(float *)&drain_cover_particle_height_span_per_particle_gp;
         i++) {
        float fi = (float)j;
        x = random_float_between(fi, *(float *)&drain_cover_particle_spawn_radius_upper_bound_gp);
        y = random_angle_radians();
        particle_position[0] = fast_cos(y) * x;
        particle_position[1] = fast_sin(y) * x;
        particle_position[2] = fi;
        fast_vec_add(particle_position, particle_source_moby + 0x10, particle_position);
        particle_position[2] =
            random_float_between(lower_height_bound, upper_height_bound) - ground;
        spawn_height_candidate =
            random_float_between((lower_height_bound + upper_height_bound) * 0.5f,
                                 upper_height_bound) -
            ground;
        if (spawn_height_candidate < particle_position[2])
            particle_position[2] = spawn_height_candidate;
        u = random_float_between(*(float *)&drain_cover_particle_scalar_lower_bound_gp,
                                 *(float *)&drain_cover_particle_scalar_upper_bound_gp);
        k = scale_game_frames(*(int *)&D_L05_00161DB8_gp);
        particle_size = truncate_float_to_s32(
            FUN_L00_00257c48(fi, (float)*(int *)&drain_cover_particle_size_sampling_bound_gp));
        particle_color = *(int *)&drain_cover_particle_color_gp;
        create_particle_with_velocity_and_lifetime(
            u, particle_position, v, k, particle_size, particle_color);
    }
}

extern float D_L05_00161DD8 __attribute__((sda));

extern float drain_cover_tilt_acceleration_gp __asm__("D_L05_00161DDC") __attribute__((sda));
extern float drain_cover_tilt_deceleration_gp __asm__("D_L05_00161DE0") __attribute__((sda));
extern float drain_cover_tilt_max_speed_gp __asm__("D_L05_00161DE4") __attribute__((sda));
extern float
approach_angle(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern float fast_add_rotations(float, float);

void update_drain_cover_tilt(char *moby, float turn_scale, float speed_scale) {
    char        *data = (char *)MOBY(moby)->pvars;
    struct Hero *game = &hero;
    float        offset[4];
    float        horizontal;
    float        vertical;
    float        angle0;
    float        angle1;
    float        target0;
    float        target1;
    float        rate0;
    float        rate1;
    float        limit;
    float        dot0;
    float        dot1;

    if ((char *)game->ground_moby == moby && game->air_frames.s == 0) {
        fast_vec_sub(offset, &game->motion.pos, &MOBY(moby)->pos);
        dot0 = fast_vec_dot(offset, &MOBY(moby)->rot_row0);
        dot1 = fast_vec_dot(offset, &MOBY(moby)->rot_row1);
        horizontal = turn_scale * dot0;
        vertical = -turn_scale * dot1;
    } else {
        horizontal = 0.0f;
        vertical = 0.0f;
    }

    *(float *)(data + 0x98) = fast_add_rotations(*(float *)(data + 0x98), *(float *)(data + 0xa0));
    *(float *)(data + 0x9c) = fast_add_rotations(*(float *)(data + 0x9c), *(float *)(data + 0xa4));
    angle0 = fast_sin(*(float *)(data + 0x98));
    target0 = fast_add_rotations(vertical, angle0 * turn_scale * D_L05_00161DD8);
    angle1 = fast_sin(*(float *)(data + 0x9c));
    target1 = fast_add_rotations(horizontal, angle1 * turn_scale * D_L05_00161DD8);

    rate0 = drain_cover_tilt_acceleration_gp * speed_scale * 0.017453292f * frame_time_sq;
    rate1 = drain_cover_tilt_deceleration_gp * speed_scale * 0.017453292f * frame_time_sq;
    limit = drain_cover_tilt_max_speed_gp * 0.017453292f * frame_time;
    approach_angle(&MOBY(moby)->rot.x, target0, (float *)(data + 0x90), rate0, rate1, limit);
    approach_angle(&MOBY(moby)->rot.y,
                   target1,
                   (float *)(data + 0x94),
                   drain_cover_tilt_acceleration_gp * speed_scale * 0.017453292f * frame_time_sq,
                   drain_cover_tilt_deceleration_gp * speed_scale * 0.017453292f * frame_time_sq,
                   drain_cover_tilt_max_speed_gp * 0.017453292f * frame_time);
}
