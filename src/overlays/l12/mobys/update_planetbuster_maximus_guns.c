#include "rnc/hero/hero.h"
#include "rnc/audio/voice_pool.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/entities.h"

#include "rnc/math/vector.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/overlay/level_path.h"

typedef struct Moby Moby;

extern float D_0015ED6C;
extern char *D_L12_0015FFD8_gp __asm__("D_L12_0015FFD8") MACRO_ADDR;

/* Hoven turret battery update: aims each manned gun at the hero, fires, takes hits and smokes when damaged. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00306890), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct {
    char  pad0[0x60];
    float f60;
    char  pad64[0x1C];
    float f80;
    int   w84;
    char  pad88[4];
    float f8C;
    short h90;
    char  b92;
    char  b93;
    char  pad94[4];
    short h98;
    short h9A;
    short h9C;
    short h9E;
} Gun;

extern struct Hero hero_lui __asm__("D_0013F350") NOT_SDA;
extern char        D_L12_00167240[];
extern float       frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float       D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float       FUN_L00_00257c48(float, float);
extern float       step_angle_toward_target(float *, float, int, float) __asm__("FUN_L00_0025bc00");
extern float
spring_angle(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern int                D_L12_00161F00;
extern int                D_L12_00161F04;
extern int D_L12_00161F20 MACRO_ADDR;
extern int D_L12_00161F24 MACRO_ADDR;
extern int                D_L12_00161F34 __attribute__((sda));
extern int                planetbuster_maximus_gun_part_slots[] __asm__("D_L12_001FB670");
extern int                D_L12_001FB688[];
extern int                FUN_L00_001fefc8(void *);
extern void *spawn_class_b8_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void  update_planetbuster_maximus_detached_parts(struct Moby *) __asm__("FUN_L12_003064e8");
void         initialize_planetbuster_maximus_detached_part_motion(struct Moby *moby,
                                                                  int slot) __asm__("FUN_L12_00306350");
extern int   D_L12_00161F00_gp __asm__("D_L12_00161F00") MACRO_ADDR;
extern int   D_L12_00161F04_gp __asm__("D_L12_00161F04") MACRO_ADDR;
extern int   D_L12_00161F20 __attribute__((sda));
extern int   D_L12_00161F24 __attribute__((sda));

void update_planetbuster_maximus_guns(char *moby) __asm__("FUN_L12_003054b0");

void update_planetbuster_maximus_guns(char *moby) {
    char  *owner_data = ((char *)MOBY(moby)->pvars);
    int    i;
    float  position_buffer[4];
    float  scratch_vector[4];
    float  aim_target_position[4];
    float  particle_random_vector[4];
    float *gun_position_copy_destination;
    float *hp;

    for (i = 0; i < 5; i++) {
        int   gun_entry_index;
        char *ent;
        char *gun_moby;
        Gun  *md;
        float yaw;
        float owner_to_hero_distance_xy;
        float dz;
        int   flag;
        float particle_xy_angle;
        float radial_scale;

        if (*(int *)(owner_data - -(i * 4) + 0x5D0) != -1) {
            gun_entry_index = *(int *)(owner_data - -(i * 4) + 0x5D0);
            ent = owner_data + gun_entry_index * 0x30;
            gun_moby = (char *)(D_L12_0015FFD8_gp + (*(int *)(ent + 0x20) << 8));
            md = ((Gun *)MOBY(gun_moby)->pvars);
            qcopy(position_buffer, D_L12_00167240);
            gun_position_copy_destination = scratch_vector;
            position_buffer[2] -= 0.7f;
            qcopy_nc(gun_position_copy_destination, gun_moby + 0x10);
            scratch_vector[2] += 1.5f;
            md->f60 = fast_arc_tan(vector_distance_xy(scratch_vector, position_buffer),
                                   position_buffer[2] - scratch_vector[2]);
            update_joint_animation(gun_moby,
                                   (char *)md,
                                   0,
                                   frame_scale_sq_macro * 0.008f,
                                   frame_scale_sq_macro * 0.3f);
            yaw = fast_subtract_rotations(
                fast_add_rotations(
                    fast_arc_tan(((Hero *)((char *)&hero))->motion.pos.f[0] - MOBY(gun_moby)->pos.x,
                                 ((Hero *)((char *)&hero))->motion.pos.f[1] -
                                     MOBY(gun_moby)->pos.y),
                    -1.5707964f),
                MOBY(moby)->rot.z);
            if (md->h90 == 0) {
                if (step_angle_toward_target((float *)(ent + 0x18),
                                             yaw,
                                             0,
                                             D_0015ED6C_gp * 0.5235988f +
                                                 D_0015ED6C_gp * 0.17453292f * (float)i) <
                    0.08726646f) {
                    md->h90 = 1;
                    md->f8C = 1.0f;
                    if (i & 1) {
                        md->f8C = -1.0f;
                    }
                }
            } else {
                float sweep_target_yaw = fast_add_rotations(yaw, md->f8C * 0.55850536f);
                if (absolute_float(spring_angle(ent + 0x18,
                                                sweep_target_yaw,
                                                (char *)md + 0x94,
                                                0,
                                                frame_scale_sq_macro * 0.022f,
                                                frame_scale_sq_macro * 0.3f,
                                                D_0015ED6C_gp * 0.2617994f +
                                                    (float)i * (D_0015ED6C_gp * 0.05235988f))) <
                    0.017453292f) {
                    md->f8C = -md->f8C;
                }
            }
            flag = 0;
            hp = (float *)(((char *)&hero) + 0x80);
            owner_to_hero_distance_xy = vector_distance_xy(moby + 0x10, hp);
            dz = absolute_float(hp[2] - MOBY(moby)->pos.z);
            if (scale_game_frames(0x4B0) < D_L12_00161F34) {
                if (owner_to_hero_distance_xy < 80.0f && dz < 4.0f) {
                    flag = 1;
                }
            }
            if (owner_to_hero_distance_xy > 90.0f || dz > 7.0f) {
                D_L12_00161F34 = 0;
            }
            if ((D_L12_00161F00_gp != 0 || flag) && md->f80 > 0.0f) {
                fast_dec_timer_s16(&md->h98);
                fast_dec_timer_s16(&md->h9C);
                if (md->h98 == 0) {
                    if (fast_difference_between_rotations(
                            *(float *)(owner_data + gun_entry_index * 0x30 + 0x18), yaw) <
                        0.06981317f) {
                        md->h9E = scale_game_frames(0x14);
                    }
                    if (fast_dec_timer_s16(&md->h9E) == 0) {
                        if (FUN_L00_001fefc8(&md->b92) == 0) {
                            if (FUN_L00_001fefc8(&md->b93) != 0) {
                                allocate_voice_for_target_entry(0, 0, gun_moby);
                                md->b93 = scale_game_frames(9);
                            }
                        }
                        if (fast_dec_timer_s16(&md->h9C) != 0) {
                            float rz;
                            float pitch;
                            int   n;
                            moby_point_position(gun_moby, md->h9A + 1, position_buffer);
                            md->h9A = md->h9A == 0;
                            rz = fast_add_rotations(MOBY(gun_moby)->rot.z, 1.5707964f);
                            if (D_L12_00161F00_gp != 0) {
                                qcopy(aim_target_position, D_L12_00167240);
                                aim_target_position[2] += random_float_between(-1.5f, 0.3f);
                            } else {
                                qcopy(aim_target_position,
                                      &((Hero *)((char *)&hero))->motion.body_center);
                                aim_target_position[2] += FUN_L00_00257c48(0.0f, 0.5f);
                            }
                            n = D_L12_00161F20 - 3;
                            if (scale_game_frames(5) < n) {
                                n = scale_game_frames(5);
                            }
                            if (n < 0) {
                                n = 0;
                            }
                            aim_target_position[2] +=
                                random_float_between((float)n * -0.3f, (float)n * 0.3f);
                            pitch = fast_arc_tan(
                                vector_distance_xy(position_buffer, aim_target_position),
                                aim_target_position[2] - position_buffer[2]);
                            build_spherical_offset(
                                scratch_vector, D_0015ED6C_gp * 80.0f, rz, pitch);
                            if (spawn_class_b8_projectile(gun_moby,
                                                          position_buffer,
                                                          scratch_vector,
                                                          scale_game_frames(0x96)) != 0) {
                                int j;
                                md->b92 = scale_game_frames(0x14);
                                for (j = 0; j < 9; j++) {
                                    int firing_particle_color_low_byte;
                                    int firing_particle_color;
                                    int life;
                                    particle_random_vector[0] =
                                        FUN_L00_00257c48(0.0f, D_0015ED6C_gp * 1.7f);
                                    particle_random_vector[1] =
                                        FUN_L00_00257c48(0.0f, D_0015ED6C_gp * 1.7f);
                                    particle_random_vector[2] =
                                        FUN_L00_00257c48(0.0f, D_0015ED6C_gp * 1.7f);
                                    firing_particle_color_low_byte = random_int_between(0xAA, 0xF0);
                                    firing_particle_color = random_int_between(0x6E, 0xC8);
                                    firing_particle_color = 0x7F000000 |
                                                            (firing_particle_color << 8) |
                                                            firing_particle_color_low_byte;
                                    life = scale_game_frames(random_int_between(0xF, 0x1E));
                                    spawn_effect(position_buffer,
                                                 particle_random_vector,
                                                 firing_particle_color,
                                                 0xFF,
                                                 life,
                                                 0x37,
                                                 random_int_between(0xB4, 0x10E),
                                                 1);
                                }
                            }
                            md->h9C = scale_game_frames(7);
                            n = D_L12_00161F20;
                            if (scale_game_frames(0xB) < n) {
                                n = scale_game_frames(0xB);
                            }
                            if (n < 0) {
                                n = 0;
                            }
                            md->h9C += n;
                        }
                    }
                }
            }
            if (D_L12_00161F04_gp == 0 && md->f80 > 0.0f) {
                if (fast_dec_timer(&md->w84) != 0) {
                    float dmg = 1.0f;
                    char *r = find_moby_hit(gun_moby, 0x50000, 0);
                    if (r == 0) {
                        int   k = planetbuster_maximus_gun_part_slots[i];
                        char *fallback_moby = 0;
                        if (k == 0x11) {
                            fallback_moby = moby;
                        } else {
                            int id = *(int *)(owner_data + k * 0x30 + 0x20);
                            if (id != -1) {
                                fallback_moby = (char *)(D_L12_0015FFD8_gp + (id << 8));
                            }
                        }
                        if (fallback_moby != 0) {
                            dmg = 0.2f;
                            r = find_moby_hit(fallback_moby, 0x50000, 0);
                            ((unsigned char *)fallback_moby)[0xA4] = 0xFF;
                        }
                        if (r == 0) {
                            goto done;
                        }
                    }
                    if (D_L12_00161F00_gp == 0) {
                        D_L12_00161F34 = scale_game_frames(0xFA0);
                        goto done;
                    }
                    md->w84 = scale_game_frames(0x14);
                    md->f80 -= dmg;
                    if (md->f80 <= 0.0f) {
                        int j;
                        int k;
                        *(short *)(owner_data + 0x5FC) -= 0x14;
                        if (*(short *)(owner_data + 0x5FC) < 0) {
                            *(short *)(owner_data + 0x5FC) = 0;
                        }
                        allocate_voice_for_target_entry(1, 0, gun_moby);
                        if (*(short *)(owner_data + 0x5FC) > 0) {
                            int k = planetbuster_maximus_gun_part_slots[i];
                            if (k != 0x11) {
                                D_L12_001FB688[k] = *(int *)(owner_data + k * 0x30 + 0x20);
                                *(int *)(owner_data + k * 0x30 + 0x20) = -1;
                            } else {
                                D_L12_001FB688[0x11] = 1;
                            }
                            initialize_planetbuster_maximus_detached_part_motion(
                                (struct Moby *)moby, k);
                            if (i == 4) {
                                for (j = 0; j < 0x11; j++) {
                                    int id = *(int *)(owner_data + j * 0x30 + 0x20);
                                    if (id != -1 && *(short *)((id << 8) + (int)D_L12_0015FFD8_gp +
                                                               0xA6) == 0x4FD) {
                                        D_L12_001FB688[j] = id;
                                        *(int *)(owner_data + j * 0x30 + 0x20) = -1;
                                        initialize_planetbuster_maximus_detached_part_motion(
                                            (struct Moby *)moby, j);
                                    }
                                }
                            }
                        }
                        for (k = 0; k < 0x82; k++) {
                            int c;
                            qcopy(position_buffer, gun_moby + 0x10);
                            position_buffer[2] += random_float_between(0.1f, 0.5f);
                            particle_xy_angle = random_angle_radians();
                            radial_scale = random_float_between(0.1f, 0.8f);
                            position_buffer[0] += fast_cos(particle_xy_angle) * radial_scale;
                            position_buffer[1] += fast_sin(particle_xy_angle) * radial_scale;
                            c = random_int_between(scale_game_frames(0x5A),
                                                   scale_game_frames(0xB4));
                            radial_scale =
                                random_float_between(D_0015ED6C_gp * 0.7f, D_0015ED6C_gp * 1.5f);
                            scratch_vector[0] = fast_cos(particle_xy_angle) * radial_scale;
                            scratch_vector[1] = fast_sin(particle_xy_angle) * radial_scale;
                            scratch_vector[2] = 0.0f;
                            scratch_vector[2] =
                                random_float_between(D_0015ED6C_gp, D_0015ED6C_gp * 5.5f);
                            spawn_effect_particle_with_target_scalar(
                                position_buffer, scratch_vector, 0x500A1414, 0xA0A0A, 420000.0f, c);
                        }
                        MOBY(gun_moby)->collision = 0;
                        MOBY(gun_moby)->flags |= 0x41;
                    }
                }
            done:
                ((unsigned char *)gun_moby)[0xA4] = 0xFF;
            } else {
                ((unsigned char *)gun_moby)[0xA4] = 0xFF;
            }
            if (md->f80 > 0.0f && md->f80 < 10.0f) {
                int lvl = (9 - truncate_float_to_s32(md->f80)) / 2;
                int j;
                int n = lvl + 3;
                for (j = 0; j < n; j++) {
                    int c;
                    qcopy(position_buffer, gun_moby + 0x10);
                    position_buffer[2] += 0.7f;
                    particle_xy_angle = random_angle_radians();
                    radial_scale = random_float_between(0.1f, 0.4f);
                    position_buffer[0] += fast_cos(particle_xy_angle) * radial_scale;
                    position_buffer[1] += fast_sin(particle_xy_angle) * radial_scale;
                    c = random_int_between(scale_game_frames(0x19), scale_game_frames(0x37));
                    radial_scale = random_float_between(D_0015ED6C_gp * 0.5f, D_0015ED6C_gp * 3.0f);
                    scratch_vector[0] = fast_cos(particle_xy_angle) * radial_scale;
                    scratch_vector[1] = fast_sin(particle_xy_angle) * radial_scale;
                    scratch_vector[2] = 0.0f;
                    scratch_vector[2] =
                        random_float_between(D_0015ED6C_gp * 7.0f, D_0015ED6C_gp * 17.0f) *
                        convert_integer_to_float(lvl) * 1.5f / convert_integer_to_float(5);
                    spawn_effect_particle_with_target_scalar(position_buffer,
                                                             scratch_vector,
                                                             (random_int_between(0x46, 0xCD) << 8) |
                                                                 0x460A00AA,
                                                             0xA0A46,
                                                             420000.0f,
                                                             c);
                }
                for (j = 0; j < 3; j++) {
                    int c;
                    qcopy(position_buffer, gun_moby + 0x10);
                    particle_xy_angle = random_angle_radians();
                    radial_scale = random_float_between(0.1f, 0.8f);
                    position_buffer[0] += fast_cos(particle_xy_angle) * radial_scale;
                    position_buffer[1] += fast_sin(particle_xy_angle) * radial_scale;
                    c = random_int_between(scale_game_frames(0x28), scale_game_frames(0x46));
                    radial_scale = random_float_between(D_0015ED6C_gp * 0.1f, D_0015ED6C_gp * 0.4f);
                    scratch_vector[0] = fast_cos(particle_xy_angle) * radial_scale;
                    scratch_vector[1] = fast_sin(particle_xy_angle) * radial_scale;
                    scratch_vector[2] = 0.0f;
                    scratch_vector[2] =
                        random_float_between(D_0015ED6C_gp * 2.4f, D_0015ED6C_gp * 5.5f);
                    spawn_effect_particle_with_target_scalar(
                        position_buffer, scratch_vector, 0x460A1414, 0xA0A0A, 504000.03f, c);
                }
            }
        }
    }
    update_planetbuster_maximus_detached_parts(moby);
    if (*(short *)(owner_data + 0x5FC) <= 0 && D_L12_00161F04_gp == 0) {
        D_L12_00161F24 = 1;
        moby[0x20] = 4;
        moby[0xBC] = 0;
    }
}

void initialize_planetbuster_maximus_detached_part_motion(struct Moby *moby, int slot) {
    char        *data = (char *)moby->pvars;
    struct Moby *selected;
    float       *speed;
    int          offset;
    float        initial_speed;
    float        angle_scale;
    float        first_angle;
    float        second_scale;
    char        *slot_data;

    if (slot == 0x11) {
        selected = moby;
        offset = 0x44;
    } else {
        selected = (struct Moby *)(D_L12_0015FFD8_p + D_L12_001FB688[slot] * 0x100);
        offset = slot * 4;
    }

    initial_speed = random_float_between(frame_time_sq * 3.7f, frame_time_sq * 5.0f);
    angle_scale = frame_time;
    {
        char *base = data + 0x570;
        speed = (float *)(base + offset);
    }
    *speed = initial_speed;
    first_angle = randf_sym(angle_scale * 0.05235988f, angle_scale * 0.13962634f);
    second_scale = frame_time;
    slot_data = data + slot * 0x10;
    *(float *)(slot_data + 0x450) = first_angle;
    *(float *)(slot_data + 0x454) =
        randf_sym(second_scale * 0.05235988f, second_scale * 0.17453292f);
    if (selected->oclass == 0x4fd) {
        float variation = random_float_between(0.001f, 0.0045f);
        float speed_scale = frame_time_sq;
        *(float *)(slot_data + 0x45c) = variation;
        *speed = random_float_between(speed_scale * 5.5f, speed_scale * 9.0f);
    } else if (selected->oclass == 0x500) {
        *speed = frame_time_sq * 13.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003078C8), where it is exact; names translated to the US level program. */

extern float D_L12_00161F40;

#include "rnc/overlay/l12/mobys/update_planetbuster_maximus_guns_globals.h"

extern float D_L12_00161F60;
extern float D_L12_00161F70;

void update_planetbuster_maximus_detached_parts(struct Moby *moby) {
    int   i;
    int   vertical_step_decrement_offset;
    char *data = ((char *)moby->pvars);
    int   last = 17;
    for (i = 0, vertical_step_decrement_offset = 0; i < 18;
         i++, vertical_step_decrement_offset += 4) {
        int   saved_moby_index = D_L12_001FB688[i];
        char *slot_moby;
        float position_or_vertical_step;
        if (saved_moby_index == 0) {
            continue;
        }
        if (i == last) {
            slot_moby = moby;
        } else {
            if (saved_moby_index == -1) {
                continue;
            }
            slot_moby = (char *)(D_L12_0015FFD8_gp + (saved_moby_index << 8));
        }
        if (i == last) {
            position_or_vertical_step = D_L12_00161F40;
            if (position_or_vertical_step == -1.0f) {
                qcopy((char *)&D_L12_00161F40, slot_moby + 0x10);
                qcopy((char *)&planetbuster_maximus_detached_owner_rotation, slot_moby + 0x40);
            }
            qcopy((char *)&D_L12_00161F60, slot_moby + 0x10);
            qcopy((char *)&D_L12_00161F70, slot_moby + 0x40);
            qcopy(slot_moby + 0x10, (char *)&D_L12_00161F40);
            qcopy(slot_moby + 0x40, (char *)&planetbuster_maximus_detached_owner_rotation);
            position_or_vertical_step = MOBY(slot_moby)->pos.z;
        } else {
            position_or_vertical_step = MOBY(slot_moby)->pos.z;
        }
        if (position_or_vertical_step < 20.0f) {
            if (i == last) {
                return;
            }
            mark_moby_for_removal(slot_moby);
            D_L12_001FB688[i] = -1;
        }
        position_or_vertical_step = *(float *)(data + 0x338 + i * 16) -
                                    *(float *)(data + vertical_step_decrement_offset + 0x570);
        *(float *)(data + 0x338 + i * 16) = position_or_vertical_step;
        MOBY(slot_moby)->pos.z = MOBY(slot_moby)->pos.z + position_or_vertical_step;
        if (MOBY(slot_moby)->oclass == 0x500) {
            MOBY(slot_moby)->rot.x =
                fast_add_rotations(MOBY(slot_moby)->rot.x, D_0015ED6C * 0.34906584f);
            spring_angle(
                slot_moby + 0x44, 1.5707964f, data + 0x454 + i * 16, 0, 0.00200000009f, 0.3f, 0.0f);
        } else if (MOBY(slot_moby)->oclass == 0x4FD) {
            MOBY(slot_moby)->rot.x =
                fast_add_rotations(MOBY(slot_moby)->rot.x, *(float *)(data + 0x450 + i * 16));
            spring_angle(slot_moby + 0x44,
                         -1.5707964f,
                         (float *)(data + 0x450 + i * 16) + 1,
                         0,
                         *(float *)(data + 0x45C + i * 16),
                         0.3f,
                         0.0f);
        } else if (MOBY(slot_moby)->oclass == 0x4FA) {
            float updated_rotation_angle;
            MOBY(slot_moby)->rot.x =
                fast_add_rotations(MOBY(slot_moby)->rot.x, D_0015ED6C * -0.87266463f);
            updated_rotation_angle =
                fast_add_rotations(MOBY(slot_moby)->rot.y, *(float *)(data + 0x454 + i * 16));
            MOBY(slot_moby)->pos.y = MOBY(slot_moby)->pos.y + D_0015ED6C * -4.0f;
            MOBY(slot_moby)->rot.y = updated_rotation_angle;
        } else {
            MOBY(slot_moby)->pos.y = MOBY(slot_moby)->pos.y + D_0015ED6C * -4.0f;
            MOBY(slot_moby)->rot.x =
                fast_add_rotations(MOBY(slot_moby)->rot.x, *(float *)(data + 0x450 + i * 16));
            MOBY(slot_moby)->rot.y =
                fast_add_rotations(MOBY(slot_moby)->rot.y, *(float *)(data + 0x454 + i * 16));
        }
        if (i == last) {
            qcopy((char *)&D_L12_00161F40, slot_moby + 0x10);
            qcopy((char *)&planetbuster_maximus_detached_owner_rotation, slot_moby + 0x40);
        }
        update_entity(slot_moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00307C08), where it is exact; names translated to the US level program. */

extern int            D_L12_0015F5CC;
extern unsigned char *D_L12_0015FFD8;

void update_planetbuster_maximus_glow_color(struct Moby *moby) __asm__("FUN_L12_00306828");

void update_planetbuster_maximus_glow_color(struct Moby *moby) {
    int   i;
    int   alpha = 0x80000000;
    char *data = (char *)moby->pvars;
    for (i = 0; i < 18; i++) {
        struct Moby *other;
        int          period;
        float        sine;
        float        color;
        int          red;
        int          green;
        int          blue;
        int          packed_color;
        float        phase;
        float        divisor;
        if (i == 17) {
            other = moby;
        } else {
            other = (struct Moby *)((char *)(*(struct Moby **)&D_L12_0015FFD8_p) +
                                    ((unsigned int)*(int *)(data + i * 0x30 + 0x20) << 8));
        }
        if (other->oclass == 0x4FE)
            continue;
        {
            period = scale_game_frames(0xAA);
            divisor = convert_integer_to_float(period);
            phase = (float)(D_L12_0015F5CC % period) / divisor;
            sine = fast_sin((phase + phase) * 3.1415927f + -3.1415927f);
            color = sine * 70.0f;
            red = truncate_float_to_s32(color);
            green = truncate_float_to_s32(color);
            blue = truncate_float_to_s32(sine * 10.0f);
            packed_color = ((blue + 0x46) << 16) | alpha |
                           ((green + 0xB4 > 0xFF ? 0xFF : green + 0xB4) << 8) |
                           (red + 0xB4 > 0xFF ? 0xFF : red + 0xB4);
            other->flags |= 0x10;
            other->glow_color = packed_color;
        }
    }
    /* Fakematch: keeping alpha live past the loop gives retail's schedule of the loop body. */
    alpha++;
    alpha--;
}
/* Planetbuster part update: rides its path, drops its parts and hands its voice slot back when done. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00307DB0), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
} P_7DB0;

typedef struct {
    char  pad[0x80];
    float f80;
} F80_7DB0;

typedef struct {
    char           pad[0x34];
    unsigned short flags;
} M34_7DB0;

typedef struct {
    unsigned char pad0[0x74];
    unsigned char active;
    unsigned char pad75[0x13];
    char         *owner;
} Voice_7DB0;

extern char                 D_0013E550[];
extern char *D_L12_001600EC MACRO_ADDR;
extern char                 D_0013F3D0[] NOT_SDA;
extern f32   advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern float D_L12_00161F40_gp __asm__("D_L12_00161F40") MACRO_ADDR;
extern float D_L12_00161F60_gp __asm__("D_L12_00161F60") MACRO_ADDR;
extern float D_L12_00161F70_gp __asm__("D_L12_00161F70") MACRO_ADDR;
extern float approach_value(float *, float, float);
extern int D_L12_001619A0 MACRO_ADDR;
extern int D_L12_001619B4 MACRO_ADDR;
extern int D_L12_0015F404 MACRO_ADDR;

#include "rnc/overlay/l12/mobys/update_planetbuster_maximus_guns_data/planetbuster_maximus_part_update.h"

extern float planetbuster_maximus_rotation_oscillation_amplitude_gp __asm__("D_L12_00161F30")
    __attribute__((sda));

#include "rnc/game/game_globals.h"

extern void FUN_L00_00258278(char *moby, float target, float *vel, float k, float d, float max);
extern void vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void
           capture_linked_moby_relative_transforms(char *, void *, int) __asm__("FUN_L12_0027b268");
extern int current_level_index_gp __asm__("D_0015ED84") MACRO_ADDR;
extern char D_0013F3D0[];

void planetbuster_maximus_part_update(char *moby) __asm__("FUN_L12_003069d0");

void planetbuster_maximus_part_update(char *moby) {
    enum {
        PLANETBUSTER_INITIALIZE_LINKED_PARTS = 0,
        PLANETBUSTER_WAIT_FOR_PATH_TRIGGER = 1,
        PLANETBUSTER_FOLLOW_PATH = 2,
        PLANETBUSTER_POST_PATH_GUN_UPDATE = 3,
        PLANETBUSTER_DROP_PARTS_AND_TRANSITION = 4
    };
    enum {
        PLANETBUSTER_TRANSITION_SETUP = 0,
        PLANETBUSTER_TRANSITION_INTERPOLATE = 1,
        PLANETBUSTER_TRANSITION_CLEANUP = 2
    };
    float v[4];
    float transition_rotation[4];
    char *moby_data = ((char *)MOBY(moby)->pvars);
    if (((unsigned char *)moby)[0x20] != 0 && D_L12_001FB688[17] != 0) {
        qcopy(moby + 0x10, (char *)&D_L12_00161F60_gp);
        qcopy(moby + 0x40, (char *)&D_L12_00161F70_gp);
    }
    switch ((unsigned char)moby[0x20]) {
    case PLANETBUSTER_INITIALIZE_LINKED_PARTS:
        D_L12_001619B4 = 0;
        if (visited52[(unsigned char)moby[0xB0] + (current_level_index_gp << 4)] == 0xFF) {
            int i;
            for (i = 0; i < 17; i++) {
                int s = *(int *)(moby_data + i * 0x30 + 0x20);
                if (s != -1)
                    mark_moby_for_removal((char *)D_L12_0015FFD8_gp + (s << 8));
            }
            mark_moby_for_removal(moby);
            return;
        } else {
            P_7DB0 *path;
            int     i;
            int     j;
            D_L12_00161F24 = 0;
            capture_linked_moby_relative_transforms(moby, moby_data, 0x11);
            path = ((P_7DB0 **)&(*(int *)l12_paths))[*(int *)(moby_data + 0x5C0)];
            qcopy(moby + 0x10, path + 1);
            *(float *)(moby_data + 0x618) = MOBY(moby)->pos.z;
            MOBY(moby)->rot.z =
                fast_arc_tan(path[2].v[0] - path[1].v[0], path[2].v[1] - path[1].v[1]);
            if (*(float *)(D_0013F3D0 + 0x8) < 52.0f) {
                MOBY(moby)->pos.z = MOBY(moby)->pos.z + 3.0f;
            }
            for (i = 0; i < 5; i++) {
                int   k = *(int *)(moby_data + 0x5D0 + i * 4);
                char *o = (char *)D_L12_0015FFD8_gp + (*(int *)(moby_data + k * 0x30 + 0x20) << 8);
                (*(F80_7DB0 **)(o + 0x78))->f80 = 10.0f;
            }
            D_L12_00161F40_gp = -1.0f;
            for (j = 17; j >= 0; j--)
                D_L12_001FB688[j] = 0;
            *(int *)(moby_data + 0x604) = -1;
            *(short *)(moby_data + 0x5FC) = 100;
            moby[0x20] = 1;
        }
        break;
    case PLANETBUSTER_WAIT_FOR_PATH_TRIGGER: {
        char *clip_test_position;
        int   r;
        if (is_point_inside_clip_volume(D_L12_00167240, *(int *)(moby_data + 0x624))) {
            int   i;
            char *base = (char *)D_L12_0015FFD8_gp;
            MOBY(moby)->flags |= 0x41;
            for (i = 0; i < 17; i++) {
                int s = *(int *)(moby_data + i * 0x30 + 0x20);
                if (s != -1)
                    ((M34_7DB0 *)(base + (s << 8)))->flags |= 0x41;
            }
        } else {
            int   i;
            char *base = (char *)D_L12_0015FFD8_gp;
            MOBY(moby)->flags &= ~0x41;
            for (i = 0; i < 17; i++) {
                int s = *(int *)(moby_data + i * 0x30 + 0x20);
                if (s != -1)
                    ((M34_7DB0 *)(base + (s << 8)))->flags &= ~0x41;
            }
        }
        clip_test_position = D_0013F3D0;
        r = is_point_inside_clip_volume(clip_test_position, *(int *)(moby_data + 0x628));
        if (*(float *)(clip_test_position + 8) > 52.0f && r == 0) {
            approach_value((float *)&MOBY(moby)->pos.z,
                           *(float *)(moby_data + 0x618),
                           D_0015ED6C_gp + D_0015ED6C_gp);
        } else {
            float add = 3.0f;
            if (r)
                add = 10.0f;
            if (is_point_inside_clip_volume(D_0013F3D0, *(int *)(moby_data + 0x61C))) {
                *(int *)(moby_data + 0x620) = scale_game_frames(0x5A);
            }
            if (!fast_dec_timer((int *)(moby_data + 0x620)))
                add = 19.0f;
            vendor_spring((float *)&MOBY(moby)->pos.z,
                          (float *)(moby_data + 0x62C),
                          *(float *)(moby_data + 0x618) + add,
                          frame_scale_sq_macro * 0.03f,
                          frame_scale_sq_macro * 0.3f,
                          D_0015ED6C_gp * 2.7f);
        }
        update_moby_vertical_oscillation(0.7f,
                                         D_0015ED6C_gp * 1.2217305f,
                                         moby,
                                         (float *)(moby_data + 0x5CC),
                                         (float *)(moby_data + 0x5C8));
        update_moby_rotation_oscillation(
            moby,
            moby_data + 0x5E4,
            moby_data + 0x5E8,
            planetbuster_maximus_rotation_oscillation_amplitude_gp,
            planetbuster_maximus_rotation_oscillation_common_factor_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp,
            planetbuster_maximus_rotation_oscillation_component_mix_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp);
        apply_linked_moby_relative_transforms(moby, moby_data, 0x11);
        if (is_point_inside_clip_volume(D_0013F3D0, *(int *)(moby_data + 0x5EC)) ||
            D_L12_00161F00_gp) {
            int i;
            D_L12_001619B4 = 1;
            for (i = 0; i < 5; i++) {
                int k = *(int *)(moby_data + 0x5D0 + i * 4);
                ((unsigned char *)D_L12_0015FFD8_gp +
                 (*(int *)(moby_data + k * 0x30 + 0x20) << 8))[0xA4] = 0xFF;
            }
            moby[0x20] = 2;
        }
        break;
    }
    case PLANETBUSTER_FOLLOW_PATH: {
        P_7DB0 *path;
        int     nx;
        float   target_yaw;
        update_moby_vertical_oscillation(0.7f,
                                         D_0015ED6C_gp * 1.2217305f,
                                         moby,
                                         (float *)(moby_data + 0x5CC),
                                         (float *)(moby_data + 0x5C8));
        update_moby_rotation_oscillation(
            moby,
            moby_data + 0x5E4,
            moby_data + 0x5E8,
            planetbuster_maximus_rotation_oscillation_amplitude_gp,
            planetbuster_maximus_rotation_oscillation_common_factor_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp,
            planetbuster_maximus_rotation_oscillation_component_mix_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp);
        path = ((P_7DB0 **)&(*(int *)l12_paths))[*(int *)(moby_data + 0x5C0)];
        if (*(short *)(moby_data + 0x5F8) < *(int *)path - 11) {
            approach_value(
                (float *)(moby_data + 0x5F0), D_0015ED6C_gp * 8.0f, frame_time_sq_macro * 10.0f);
        } else {
            approach_value((float *)(moby_data + 0x5F0), 0.0f, frame_time_sq_macro * 8.0f);
            if (*(float *)(moby_data + 0x5F0) == 0.0f) {
                moby[0x20] = 3;
                break;
            }
        }
        nx = (*(short *)(moby_data + 0x5F8) + 1) % *(int *)path;
        fast_vec_sub(v, &path[nx + 1], &path[*(short *)(moby_data + 0x5F8) + 1]);
        fast_vec_normalize(v, v, *(float *)(moby_data + 0x5F0));
        fast_vec_add(moby + 0x10, moby + 0x10, v);
        target_yaw = fast_arc_tan(
            *(float *)((char *)path + (nx << 4) + 0x10) -
                *(float *)((char *)path + (*(short *)(moby_data + 0x5F8) << 4) + 0x10),
            *(float *)((char *)path + (nx << 4) + 0x14) -
                *(float *)((char *)path + (*(short *)(moby_data + 0x5F8) << 4) + 0x14));
        if (*(int *)path - 20 < *(short *)(moby_data + 0x5F8))
            target_yaw = 3.0543261f;
        FUN_L00_00258278(moby,
                         target_yaw,
                         (float *)(moby_data + 0x5F4),
                         frame_scale_sq_macro * 0.0015f,
                         frame_scale_sq_macro * 0.3f,
                         D_0015ED6C_gp * 0.17453292f);
        if (vector_distance_xy(moby + 0x10, &path[*(short *)(moby_data + 0x5F8) + 1]) < 5.0f ||
            fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(
                    *(float *)((char *)path + (*(short *)(moby_data + 0x5F8) << 4) + 0x10) -
                        MOBY(moby)->pos.x,
                    *(float *)((char *)path + (*(short *)(moby_data + 0x5F8) << 4) + 0x14) -
                        MOBY(moby)->pos.y)) > 1.5707964f) {
            *(short *)(moby_data + 0x5F8) = (*(short *)(moby_data + 0x5F8) + 1) % *(int *)path;
        }
        apply_linked_moby_relative_transforms(moby, moby_data, 0x11);
        update_planetbuster_maximus_guns(moby);
        break;
    }
    case PLANETBUSTER_POST_PATH_GUN_UPDATE:
        D_L12_00161F34++;
        spring_angle(moby + 0x48,
                     3.0543261f,
                     moby_data + 0x5F4,
                     -1,
                     frame_scale_sq_macro * 0.0015f,
                     frame_scale_sq_macro * 0.3f,
                     D_0015ED6C_gp * 0.17453292f);
        update_moby_vertical_oscillation(0.7f,
                                         D_0015ED6C_gp * 1.2217305f,
                                         moby,
                                         (float *)(moby_data + 0x5CC),
                                         (float *)(moby_data + 0x5C8));
        update_moby_rotation_oscillation(
            moby,
            moby_data + 0x5E4,
            moby_data + 0x5E8,
            planetbuster_maximus_rotation_oscillation_amplitude_gp,
            planetbuster_maximus_rotation_oscillation_common_factor_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp,
            planetbuster_maximus_rotation_oscillation_component_mix_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp);
        apply_linked_moby_relative_transforms(moby, moby_data, 0x11);
        update_planetbuster_maximus_guns(moby);
        break;
    case PLANETBUSTER_DROP_PARTS_AND_TRANSITION:
        if (*(int *)(moby_data + 0x608) != -1 && *(int *)(moby_data + 0x60C) != -1) {
            switch ((unsigned char)moby[0xBC]) {
            case PLANETBUSTER_TRANSITION_SETUP:
                moby[0xBC] = 1;
                set_camera_position(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x30);
                set_camera_rotation(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x70);
                D_L12_0015F404 = 1;
                D_L12_001619A0 = 1;
                break;
            case PLANETBUSTER_TRANSITION_INTERPOLATE:
                if (*(float *)(moby_data + 0x610) < 1.0f) {
                    advance_accelerated_scalar((float *)(moby_data + 0x610),
                                               (float *)(moby_data + 0x614),
                                               1.0f,
                                               frame_time_sq_macro * 0.666f,
                                               frame_time_sq_macro * 0.666f,
                                               D_0015ED6C_gp * 0.4f);
                    lerp_vector(v,
                                D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x30,
                                D_L12_001600EC + *(int *)(moby_data + 0x60C) * 128 + 0x30,
                                *(float *)(moby_data + 0x610));
                    clear_u64_value(transition_rotation);
                    transition_rotation[0] =
                        fast_subtract_rotations(
                            *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x60C) * 128 + 0x70),
                            *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x70)) *
                        *(float *)(moby_data + 0x610);
                    transition_rotation[0] = fast_add_rotations(
                        *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x70),
                        transition_rotation[1]);
                    transition_rotation[1] =
                        fast_subtract_rotations(
                            *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x60C) * 128 + 0x74),
                            *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x74)) *
                        *(float *)(moby_data + 0x610);
                    transition_rotation[1] = fast_add_rotations(
                        *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x74),
                        transition_rotation[1]);
                    transition_rotation[2] =
                        fast_subtract_rotations(
                            *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x60C) * 128 + 0x78),
                            *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x78)) *
                        *(float *)(moby_data + 0x610);
                    transition_rotation[2] = fast_add_rotations(
                        *(float *)(D_L12_001600EC + *(int *)(moby_data + 0x608) * 128 + 0x78),
                        transition_rotation[2]);
                    set_camera_position(v);
                    set_camera_rotation(transition_rotation);
                } else {
                    moby[0xBC] = 2;
                    D_L12_0015F404 = 0;
                    jet_end_mount(3);
                }
                break;
            case PLANETBUSTER_TRANSITION_CLEANUP: {
                int i;
                for (i = 0; i < 17; i++) {
                    int s = *(int *)(moby_data + i * 0x30 + 0x20);
                    if (s != -1) {
                        char *o = (char *)D_L12_0015FFD8_gp + (s << 8);
                        if (o[0x20] >= 0)
                            mark_moby_for_removal(o);
                        *(int *)(moby_data + i * 0x30 + 0x20) = -1;
                    } else if (D_L12_001FB688[i] > 0) {
                        char *o = (char *)D_L12_0015FFD8_gp + (D_L12_001FB688[i] << 8);
                        if (o[0x20] >= 0)
                            mark_moby_for_removal(o);
                    }
                }
                D_L12_001619B4 = 0;
                start_scene(7);
                mark_moby_for_removal(moby);
                return;
            }
            }
        }
        if (*(short *)(moby_data + 0x5FA) == 0) {
            int i;
            for (i = 0; i < 17; i++) {
                if (D_L12_001FB688[i] == 0)
                    D_L12_001FB688[i] = *(int *)(moby_data + i * 0x30 + 0x20);
                *(int *)(moby_data + i * 0x30 + 0x20) = -1;
            }
            D_L12_001FB688[17] = 1;
            for (i = 0; i < 18; i++)
                initialize_planetbuster_maximus_detached_part_motion((struct Moby *)moby, i);
        }
        *(short *)(moby_data + 0x5FA) = *(short *)(moby_data + 0x5FA) + 1;
        update_planetbuster_maximus_detached_parts(moby);
        break;
    }
    update_planetbuster_maximus_glow_color(moby);
    if ((unsigned char)moby[0x20] >= 1 && (unsigned char)moby[0x20] <= 3) {
        if (*(int *)(moby_data + 0x604) == -1)
            *(int *)(moby_data + 0x604) = allocate_voice_for_target_entry(0, 4, moby);
    } else {
        int s = *(int *)(moby_data + 0x604);
        if (s != -1) {
            Voice_7DB0 *voice_pool_entry = (Voice_7DB0 *)(((unsigned char *)D_0013E550) + s * 0x70);
            if (voice_pool_entry->owner == moby && voice_pool_entry->active != 0)
                release_voice_slot(s);
        }
        *(int *)(moby_data + 0x604) = -1;
    }
}

#include "qcopy.h"

/* Spawns the next linked moby at this one's spot, sets it up and gives it a random spin. */
extern void
spawn_jittered_effect_particle_with_optional_basis(char *, int, float) __asm__("FUN_L00_0026de90");

void spawn_next_linked_moby(struct Moby *spawner) __asm__("FUN_L12_003076e0");

void spawn_next_linked_moby(struct Moby *spawner) {
    char *spawner_data = (char *)spawner->pvars;
    int  *indexed_spawner_data = (int *)(spawner_data + (*(int *)(spawner_data + 0xF0) << 2));
    struct Moby *spawned_moby =
        (struct Moby *)((unsigned char *)D_L12_0015FFD8_p + (indexed_spawner_data[0x30] << 8));
    struct MobyClass *moby_class;
    short            *aim_height_data;
    int               i;
    qcopy(&spawned_moby->pos, &spawner->pos);
    spawned_moby->pos.z -= 0.35f;
    spawned_moby->state = 0;
    spawned_moby->command = 0;
    moby_class = spawned_moby->pclass;
    spawned_moby->flags = moby_class->flags;
    spawned_moby->scale = moby_class->scale;
    spawned_moby->update_dist = 0xFF;
    spawned_moby->visible = 1;
    spawned_moby->occlusion = 0x7F80;
    spawned_moby->draw_dist = 0xFF;
    spawned_moby->unk71 = 0xFF;
    spawned_moby->unk72 = 0xFF;
    spawned_moby->hit_slot = 0xFF;
    spawned_moby->collision = moby_class->collision;
    spawned_moby->spawn_frame = spawner->spawn_frame;
    update_entity(spawned_moby);
    spawned_moby->parent = spawner;
    for (i = 0; i < 14; i++) {
        spawn_jittered_effect_particle_with_optional_basis(
            &spawner->pos, 0, random_float_between(frame_time * 3.0f, frame_time * 5.0f));
    }
    aim_height_data = suck_get_aim_height(spawned_moby);
    if (aim_height_data != 0) {
        *(float *)aim_height_data = aim_height_data[2];
    }
}

/* Door/switch moby: clears its linked mobys, waits for the hero, opens, fires its list one by one, closes. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308C20), where it is exact; names translated to the US level program. */

void spawn_next_linked_moby(struct Moby *m) __asm__("FUN_L12_003076e0");
void update_homing_mine_spawner_target(struct Moby *spawner) __asm__("FUN_L12_00307ab8");

extern void update_homing_mine_spawner_target(struct Moby *m) __asm__("FUN_L12_00307ab8");

void homing_mine_spawner_update(struct Moby *m) __asm__("FUN_L12_00307840");

void homing_mine_spawner_update(struct Moby *spawner) {
    enum {
        HOMING_MINE_SPAWNER_CLEAR_LINKED_MOBYS = 0,
        HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY = 1,
        HOMING_MINE_SPAWNER_WAIT_FOR_PRESPAWN_ANIMATION = 2,
        HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS = 3,
        HOMING_MINE_SPAWNER_WAIT_FOR_POSTSPAWN_ANIMATION = 4,
        HOMING_MINE_SPAWNER_SPAWN_SEQUENCE_COMPLETE = 5
    };
    char *spawner_data = (char *)spawner->pvars;
    int  *slots;
    int   i;
    update_homing_mine_spawner_target(spawner);
    slots = (int *)(spawner_data + 0xC0);
    switch (spawner->state) {
    case HOMING_MINE_SPAWNER_CLEAR_LINKED_MOBYS:
        for (i = 0; i < 12; i++) {
            if (slots[i] != -1) {
                char *linked_moby = (D_L12_0015FFD8_p + (slots[i] << 8));
                if (linked_moby != 0 && ((unsigned char *)linked_moby)[0x20] != 0xFE &&
                    ((unsigned char *)linked_moby)[0x20] != 0xFD) {
                    mark_moby_for_removal(linked_moby);
                }
            }
        }
        spawner->state = 1;
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_PROXIMITY:
        if (*(int *)(spawner_data + 0xB4) != 2) {
            if (vector_distance_xy(&spawner->pos, spawner_data + 0x70) <
                *(float *)(spawner_data + 0xF8)) {
                spawner->state = 2;
            }
        }
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_PRESPAWN_ANIMATION:
        if (spawner->prev_seq != 1) {
            blend_moby_animation(spawner, 1, 0, scale_game_frames(2));
        }
        if (spawner->anim_flags & 2) {
            if (spawner->prev_seq != 2) {
                blend_moby_animation(spawner, 2, 0, scale_game_frames(2));
            }
            spawner->state = 3;
        }
        break;
    case HOMING_MINE_SPAWNER_SPAWN_LINKED_MOBYS:
        if (fast_dec_timer_s16(spawner_data + 0xFC)) {
            if (slots[*(int *)(spawner_data + 0xF0)] != -1) {
                spawn_next_linked_moby(spawner);
                spawner->command = spawner->command == 0;
                *(short *)(spawner_data + 0xFC) = scale_game_frames(0x28);
            }
            *(int *)(spawner_data + 0xF0) += 1;
        }
        if (*(int *)(spawner_data + 0xF0) == 12 || slots[*(int *)(spawner_data + 0xF0)] == -1) {
            spawner->state = 4;
        }
        break;
    case HOMING_MINE_SPAWNER_WAIT_FOR_POSTSPAWN_ANIMATION:
        spawner->flags &= 0xEFFF;
        if (spawner->prev_seq != 3) {
            blend_moby_animation(spawner, 3, 0, scale_game_frames(2));
        }
        if (spawner->anim_flags & 2) {
            if (spawner->prev_seq != 0) {
                blend_moby_animation(spawner, 0, 0, scale_game_frames(2));
            }
            spawner->state = 5;
        }
        break;
    case HOMING_MINE_SPAWNER_SPAWN_SEQUENCE_COMPLETE:
        break;
    }
}

#include "qcopy.h"

/* per-frame update of a moby's timer and approach state */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00308E98), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int  select_moby_combat_target(char *, char *, float) __asm__("FUN_L00_0025fcb8");

void update_homing_mine_spawner_target(struct Moby *spawner) {
    char *spawner_data = (char *)spawner->pvars;
    char *pad;
    if (spawner->state != 0) {
        if (*(int *)(spawner_data + 0x38) != 0) {
            *(short *)(spawner_data + 0xFE) = scale_game_frames(0xB4);
        }
        *(int *)(spawner_data + 0x38) = 0;
        if (fast_dec_timer_s16(spawner_data + 0xFE)) {
            *(float *)(spawner_data + 0xF8) = *(float *)(spawner_data + 0xF4);
        } else {
            *(float *)(spawner_data + 0xF8) = *(float *)(spawner_data + 0xF4) + 5.0f;
        }
        if (select_moby_combat_target(
                spawner, spawner_data + 0x70, *(float *)(spawner_data + 0xF8)) != 2) {
            if (absolute_float(spawner->pos.z - *(float *)(spawner_data + 0x78)) > 3.0f) {
                *(int *)(spawner_data + 0xB4) = 2;
            }
        }
        if (*(int *)(spawner_data + 0xB0) == 0) {
            pad = D_0013E533 + 0xE1D;
            *(int *)(spawner_data + 0xB0) = *(int *)(pad + 0x2080);
            qcopy(spawner_data + 0x70, pad + 0x80);
        }
    }
}

#include "rnc/mobys/moby.h"

#include "rnc/game/game_globals.h"

extern int check_scaled_vector_bsphere(void *, float) __asm__("FUN_L01_00275690");
extern int FUN_0022da68(int, int, void *);
void       draw_hoven_jittered_quad_layers(struct Moby *moby) __asm__("FUN_L12_00308350");

extern void draw_hoven_jittered_quad_layers(struct Moby *) __asm__("FUN_L12_00308350");

/* Pvars: d[0] is the moby to watch, d[1] the voice slot held for this moby. */
void update_moby_draw_and_monitor_linked_state(struct Moby *moby) __asm__("FUN_L12_003081b0");

void update_moby_draw_and_monitor_linked_state(struct Moby *moby) {
    enum {
        CHECK_PERSISTENT_REMOVAL = 0,
        QUEUE_DRAW_AND_MONITOR_LINKED_MOBY = 1
    };
    int  moby_state_snapshot = moby->state;
    int  vis;
    int *moby_data = (int *)moby->pvars;

    switch (moby_state_snapshot) {
    case CHECK_PERSISTENT_REMOVAL:
        if (moby->mission != 0xFF && visited52[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            break;
        }
        moby->state = 1;
        break;
    case QUEUE_DRAW_AND_MONITOR_LINKED_MOBY:
        vis = 1;
        if (check_scaled_vector_bsphere(moby, 48.0f) == -1) {
            vis = 0;
        }
        if (vis) {
            add_draw_callback(draw_hoven_jittered_quad_layers, moby);
        }
        if (moby_data[1] == -1) {
            moby_data[1] = FUN_0022da68(0, 4, moby);
        }
        if (moby_data[0] >= 0) {
            int i = moby_data[1];
            if (i != -1) {
                char *voice_pool_entry = ((char *)&voice_pool) + i * 0x70;
                if (*(struct Moby **)(voice_pool_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_pool_entry + 0x74) != 0) {
                    release_voice_slot(i);
                }
            }
            moby_data[1] = -1;
            if (((unsigned char *)D_L12_0015FFD8_p)[(moby_data[0] << 8) + 0x20] == 7) {
                if (moby->mission != 0xFF) {
                    jet_clear_persistent(moby->mission);
                }
                FUN_0022da68(1, 0, moby);
                moby->collision = 0;
                moby->state = 2;
            }
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_00309730.c: func_L12_00309730). */

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) VendorQuadCorner;

typedef struct {
    float        corner[4][4];
    unsigned int color[4];
    float        uv[4][2];
    long         tag;
    long         texture;
    long         texture_state;
    long         gs_state;
} VendorDrawPacket;

extern int hoven_jittered_first_packet_vertex_color_gp __asm__("D_L12_00161F90")
    __attribute__((sda));
extern int hoven_jittered_second_packet_vertex_color_gp __asm__("D_L12_00161F94")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_minuend_selector_gp __asm__("D_L12_00161FBC")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_subtrahend_selector_gp __asm__("D_L12_00161FC0")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_alpha_factor_selector_gp __asm__("D_L12_00161FC4")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_addend_selector_gp __asm__("D_L12_00161FC8")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_minuend_selector_gp __asm__("D_L12_00161FCC")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_subtrahend_selector_gp __asm__("D_L12_00161FD0")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_alpha_factor_selector_gp __asm__("D_L12_00161FD4")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_addend_selector_gp __asm__("D_L12_00161FD8")
    __attribute__((sda));
extern float hoven_jittered_quad_base_uvs[4][2] __asm__("D_L12_001FB6D0");

VendorQuadCorner D_L12_001FB6F0[4] = {
    {0.0f, -5.0f, -0.02f, 1.0f},
    {0.0f, -5.0f,  0.02f, 1.0f},
    {0.0f,  5.0f, -0.02f, 1.0f},
    {0.0f,  5.0f,  0.02f, 1.0f},
};

VendorQuadCorner D_L12_001FB730[4] = {
    {0.0f, -5.0f, -0.2f, 1.0f},
    {0.0f, -5.0f,  0.2f, 1.0f},
    {0.0f,  5.0f, -0.2f, 1.0f},
    {0.0f,  5.0f,  0.2f, 1.0f},
};

extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_hoven_jittered_quad_layers(struct Moby *moby) {
    VendorDrawPacket packets[2] __attribute__((aligned(16)));
    float            matrix[16] __attribute__((aligned(16)));
    long             texture;
    int              first_color, second_color;
    int              pass, row, vertex, corner;
    float            jitter;

    copy_matrix_rotation(matrix, &moby->rot_row0);
    qcopy(&matrix[12], &moby->pos);
    matrix[15] = 1.0f;
    texture = get_effect_texture(0xE);
    packets[0].texture = packets[1].texture = texture;
    first_color = hoven_jittered_first_packet_vertex_color_gp;
    second_color = hoven_jittered_second_packet_vertex_color_gp;
    packets[0].texture_state = packets[1].texture_state = 0xFF9000000260L;
    packets[0].gs_state = (long)hoven_jittered_first_packet_blend_minuend_selector_gp |
                          ((long)hoven_jittered_first_packet_blend_subtrahend_selector_gp << 2) |
                          ((long)hoven_jittered_first_packet_blend_alpha_factor_selector_gp << 4) |
                          ((long)hoven_jittered_first_packet_blend_addend_selector_gp << 6) |
                          (0x8000L << 24);
    packets[1].gs_state = (long)hoven_jittered_second_packet_blend_minuend_selector_gp |
                          ((long)hoven_jittered_second_packet_blend_subtrahend_selector_gp << 2) |
                          ((long)hoven_jittered_second_packet_blend_alpha_factor_selector_gp << 4) |
                          ((long)hoven_jittered_second_packet_blend_addend_selector_gp << 6) |
                          (0x8000L << 24);
    packets[0].color[0] = packets[0].color[1] = packets[0].color[2] = packets[0].color[3] =
        first_color;
    packets[0].tag = packets[1].tag = 0;
    packets[1].color[0] = packets[1].color[1] = packets[1].color[2] = packets[1].color[3] =
        second_color;
    for (vertex = 0; vertex < 4; vertex++) {
        float u, v;
        qcopy(packets[1].corner[vertex], &D_L12_001FB6F0[vertex]);
        qcopy(packets[0].corner[vertex], &D_L12_001FB730[vertex]);
        u = hoven_jittered_quad_base_uvs[vertex][0];
        v = hoven_jittered_quad_base_uvs[vertex][1];
        packets[0].uv[vertex][0] = u;
        packets[0].uv[vertex][1] = v;
        packets[1].uv[vertex][0] = u;
        packets[1].uv[vertex][1] = v;
    }
    for (pass = 0; pass < 5; pass++) {
        float u_offset = random_float_between(-0.5f, 0.5f);
        for (corner = 0; corner < 4; corner++) {
            float(*first)[4] = packets[0].corner;
            float(*second)[4] = packets[1].corner;
            first[corner][2] = first[corner][2] + 1.0f;
            second[corner][2] = second[corner][2] + 1.0f;
            packets[0].uv[corner][0] = hoven_jittered_quad_base_uvs[corner][0] + u_offset;
            packets[0].uv[corner][1] = hoven_jittered_quad_base_uvs[corner][1];
        }
        jitter = 0.0f;
        row = -3;
        do {
            int   next_row = row + 1;
            int   jittered = row < 4;
            float y = (float)row + jitter;
            jitter = 0.0f;
            packets[0].corner[0][1] = y;
            packets[0].corner[1][1] = y;
            if (jittered)
                jitter = random_float_between(-0.2f, 0.2f);
            y = (float)next_row + jitter;
            packets[0].corner[2][1] = y;
            packets[0].corner[3][1] = y;
            row = next_row;
            draw_geometry_quad(&packets[0], matrix, 0);
        } while (row < 3);
    }
}

#include "sda.h"

/* Vendor-counter moby update: once the shared block is ready, latch its data and copy two vectors. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030A7A8), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

extern char scene_trigger_saved_hero_data[] __asm__("D_L12_0016CCF0");
extern int  D_L12_0015F5C4;

void trigger_scene_once_in_clip_volume(struct Moby *moby) __asm__("FUN_L12_003093c8");

void trigger_scene_once_in_clip_volume(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *base;
    char *hero_position;
    char *destination_base;
    if (D_L12_0015F5CC != 0 && D_L12_0015F5C4 == 0) {
        base = alternate_item_available;
        if (*(unsigned char *)(*(int *)(data + 4) + base + 0x5C) != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *(int *)data) != 0) {
            *(char *)(*(int *)(data + 4) + base + 0x5C) = 1;
            qcopy(scene_trigger_saved_hero_data, hero_position);
            destination_base = scene_trigger_saved_hero_data - 0x10;
            *(float *)(destination_base + 0x18) = *(float *)(hero_position + 0x258);
            qcopy(scene_trigger_saved_hero_data + 0x10, hero_position + 0x10);
            *(short *)(destination_base + 0x46) = 1;
            start_scene(*(int *)(data + 8));
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030A880). */
typedef struct {
    char  pad0[0x30];
    int   phase;
    char  pad34[0x144];
    char *mobys[4];
} L12VendorMobyList;

extern L12VendorMobyList D_L12_0016CCE0;
extern void              cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

static inline unsigned char camera_target_index(int phase) {
    if (phase == 1)
        return 2;
    return 0;
}

void emit_phase_selected_moby_effects(struct Moby *moby) __asm__("FUN_L12_003094a0");

void emit_phase_selected_moby_effects(struct Moby *moby) {
    enum {
        INITIALIZE_UPDATE_DISTANCE = 0,
        DISPATCH_PHASE_SELECTED_MOBY = 1
    };
    L12VendorMobyList *list;
    L12VendorMobyList *target_list;
    int                phase;
    int                index;
    int                never;
    int                held;

    /* Fakematch: the dead call guarded by never keeps retail's branch shape. */
    never = 0;
    switch (moby->state) {
    case INITIALIZE_UPDATE_DISTANCE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case DISPATCH_PHASE_SELECTED_MOBY:
        if (D_L12_0015F5C4 == 2) {
            list = &D_L12_0016CCE0;
            phase = list->phase;
            if (phase == 1 || phase == 7) {
                /* Fakematch: see never. */
                held = phase + 1;
                if (never)
                    cutscene_set_camera_target(held);
                index = camera_target_index(list->phase);
                target_list = &D_L12_0016CCE0;
                cutscene_set_camera_target((int)target_list->mobys[index]);
            }
        }
        break;
    }
}

/* Draws the moby's 10-piece overlay when its trigger is visible, with its own GS state. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030D248), where it is exact; names translated to the US level program. */

extern float D_L12_00162108[2] MACRO_ADDR;
extern char  hoven_scrolling_clipped_scrolled_uv_buffer[] __asm__("D_L12_00208B00");
extern int   hoven_scrolling_clipped_vertex_counts[] __asm__("D_L12_001FBD10");
extern int   hoven_scrolling_clipped_vertex_position_tables[] __asm__("D_L12_002059E0");
extern int   hoven_scrolling_clipped_base_uv_tables[] __asm__("D_L12_00205A08");
extern int   hoven_scrolling_clipped_vertex_color_tables[] __asm__("D_L12_00205A58");
extern void  FUN_L00_001fde98(int, int, int, void *, int);
extern void  FUN_L08_00258090(void *, int, int, void *);
extern void  draw_eleven_entry_scrolled_texture_batch(int) __asm__("FUN_L08_002f0c18");
extern void  vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");

void draw_hoven_scrolling_texture_layers(struct Moby *moby) __asm__("FUN_L12_0030be68");

void draw_hoven_scrolling_texture_layers(struct Moby *moby) {
    int *moby_data = (int *)moby->pvars;
    int  i;
    font_queue_vu_state();
    vu1_add_g_sregister(0x42, 0x7F00000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(0x47, 0x5360A);
    if (is_point_inside_clip_volume(D_L12_00167240, moby_data[0])) {
        vu1_add_g_sregister(6, get_effect_texture(0x2C));
        for (i = 0; i < 10; i++) {
            FUN_L08_00258090(hoven_scrolling_clipped_scrolled_uv_buffer,
                             hoven_scrolling_clipped_base_uv_tables[i],
                             hoven_scrolling_clipped_vertex_counts[i],
                             D_L12_00162108);
            FUN_L00_001fde98(hoven_scrolling_clipped_vertex_counts[i],
                             hoven_scrolling_clipped_vertex_position_tables[i],
                             hoven_scrolling_clipped_vertex_color_tables[i],
                             hoven_scrolling_clipped_scrolled_uv_buffer,
                             1);
        }
    }
    vu1_add_g_sregister(6, get_effect_texture(0x2F));
    draw_eleven_entry_scrolled_texture_batch(0);
    vu1_add_g_sregister(0x42, ((long)hoven_scrolling_second_layer_fixed_alpha << 32) | 0x68);
    vu1_add_g_sregister(6, get_effect_texture(0x30));
    draw_eleven_entry_scrolled_texture_batch(1);
}

/* Builds the colour tables once, then scrolls two pairs of wrapped phases each frame. */
extern int hoven_scrolling_color_tables_initialized_gp __asm__("D_L12_001620F0")
    __attribute__((sda));
/* The ready flag is followed, 8 bytes on, by two pairs of phases. */
extern float hoven_scrolling_layer_phase_rates[1] __asm__("D_L12_001620E0") __attribute__((sda));
extern float hoven_scrolling_clipped_uv_scroll_rates[2] __asm__("D_L12_001620D8")
    __attribute__((sda));
extern float frame_delta_time __asm__("D_0015ED7C") MACRO_ADDR;
extern int  *hoven_scrolling_color_tables[] __asm__("D_L12_00208AE0");
extern int   hoven_scrolling_color_table_entry_counts[] __asm__("D_L12_00205A80");
extern void
scale_rgba_tables(void *, int, void *, float, float, float, float, int) __asm__("FUN_L08_002f0f68");

typedef struct {
    char          p[0x20];
    unsigned char state;
    char          q[0xF];
    unsigned char b30;
    char          r;
    short         h32;
} M_bff0;

void update_hoven_scrolling_texture_layers(M_bff0 *moby) __asm__("FUN_L12_0030bff0");

void update_hoven_scrolling_texture_layers(M_bff0 *moby) {
    enum {
        INITIALIZE_SCROLL_PHASES = 0,
        UPDATE_SCROLL_PHASES_AND_QUEUE_DRAW = 1
    };
    int i;
    switch (moby->state) {
    case INITIALIZE_SCROLL_PHASES:
        if (hoven_scrolling_color_tables_initialized_gp == 0) {
            scale_rgba_tables(hoven_scrolling_color_tables,
                              5,
                              hoven_scrolling_color_table_entry_counts,
                              0.1f,
                              0.5f,
                              0.4f,
                              0.5f,
                              1);
            scale_rgba_tables(hoven_scrolling_clipped_vertex_color_tables,
                              10,
                              hoven_scrolling_clipped_vertex_counts,
                              0.1f,
                              0.6f,
                              0.4f,
                              0.5f,
                              1);
            hoven_scrolling_color_tables_initialized_gp = 1;
        }
        moby->state = 1;
        moby->b30 = 0xFF;
        moby->h32 = 0xFF;
        for (i = 0; i < 2; i++) {
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] = 0.0f;
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] = 0.0f;
        }
        break;
    case UPDATE_SCROLL_PHASES_AND_QUEUE_DRAW:
        for (i = 0; i < 2; i++) {
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] +=
                hoven_scrolling_layer_phase_rates[i * 2] * frame_delta_time;
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] > 1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] -= 1.0f;
            }
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] < -1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 2] += 1.0f;
            }
            ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] +=
                hoven_scrolling_layer_phase_rates[i * 2 + 1] * frame_delta_time;
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] > 1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] -= 1.0f;
            }
            if (((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] < -1.0f) {
                ((float *)&hoven_scrolling_color_tables_initialized_gp)[i * 2 + 3] += 1.0f;
            }
        }
        D_L12_00162108[0] += hoven_scrolling_clipped_uv_scroll_rates[0] * frame_delta_time;
        if (D_L12_00162108[0] > 1.0f) {
            D_L12_00162108[0] -= 1.0f;
        }
        if (D_L12_00162108[0] < -1.0f) {
            D_L12_00162108[0] += 1.0f;
        }
        D_L12_00162108[1] += hoven_scrolling_clipped_uv_scroll_rates[1] * frame_delta_time;
        if (D_L12_00162108[1] > 1.0f) {
            D_L12_00162108[1] -= 1.0f;
        }
        if (D_L12_00162108[1] < -1.0f) {
            D_L12_00162108[1] += 1.0f;
        }
        add_draw_callback(draw_hoven_scrolling_texture_layers, moby);
        break;
    }
}

s32 D_L12_001FB6CC[1] __attribute__((section(".data"))) = {0};

float D_L12_00161F60_gp __asm__("D_L12_00161F60") = 0;
float D_L12_00161F70_gp __asm__("D_L12_00161F70") = 0;

float D_L12_00162108[2] = {0};

float D_L12_00161F40_gp __asm__("D_L12_00161F40") = 0;

float hoven_jittered_quad_base_uvs[4][2] __asm__("D_L12_001FB6D0") = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
};
