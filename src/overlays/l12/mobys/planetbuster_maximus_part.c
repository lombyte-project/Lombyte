/* Level 12 moby class planetbuster_maximus_part: the Planetbuster Maximus parts, their guns, glow and detached pieces. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/overlay/shared/world/level_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/overlay/shared/world/level_path.h"

enum {
    PLANETBUSTER_INITIALIZE_LINKED_PARTS = 0,
    PLANETBUSTER_WAIT_FOR_PATH_TRIGGER = 1,
    PLANETBUSTER_FOLLOW_PATH = 2,
    PLANETBUSTER_POST_PATH_GUN_UPDATE = 3,
    PLANETBUSTER_DROP_PARTS_AND_TRANSITION = 4
};

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
    int   damage_cooldown_timer;
    char  pad88[4];
    float yaw_sweep_direction;
    short yaw_sweep_active;
    char  firing_voice_window_timer;
    char  firing_voice_repeat_timer;
    char  pad94[4];
    short firing_inhibit_timer;
    short firing_point_selector;
    short shot_cooldown_timer;
    short aim_alignment_fire_holdoff_timer;
} Gun;

extern char  D_L12_00167240[];
extern float frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float FUN_L00_00257c48(float, float);
extern float step_angle_toward_target(float *, float, int, float) __asm__("FUN_L00_0025bc00");
extern float
spring_angle(void *, float, void *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern int D_L12_00161F20 MACRO_ADDR;
extern int D_L12_00161F24 MACRO_ADDR;
extern int                l12_planetbuster_maximus_proximity_fire_counter __asm__("D_L12_00161F34")
    __attribute__((sda));
extern int planetbuster_maximus_gun_part_slots[] __asm__("D_L12_001FB670");
extern int D_L12_001FB688[];
extern int FUN_L00_001fefc8(void *);
extern void             *
spawn_blarg_trooper_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void update_planetbuster_maximus_detached_parts(struct Moby *) __asm__("FUN_L12_003064e8");
void        initialize_planetbuster_maximus_detached_part_motion(struct Moby *moby,
                                                                 int slot) __asm__("FUN_L12_00306350");
extern int  D_L12_00161F00_gp __asm__("D_L12_00161F00") MACRO_ADDR;
extern int  D_L12_00161F04_gp __asm__("D_L12_00161F04") MACRO_ADDR;
extern int  D_L12_00161F20 __attribute__((sda));
extern int  D_L12_00161F24 __attribute__((sda));

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
            if (md->yaw_sweep_active == 0) {
                if (step_angle_toward_target((float *)(ent + 0x18),
                                             yaw,
                                             0,
                                             D_0015ED6C_gp * 0.5235988f +
                                                 D_0015ED6C_gp * 0.17453292f * (float)i) <
                    0.08726646f) {
                    md->yaw_sweep_active = 1;
                    md->yaw_sweep_direction = 1.0f;
                    if (i & 1) {
                        md->yaw_sweep_direction = -1.0f;
                    }
                }
            } else {
                float sweep_target_yaw =
                    fast_add_rotations(yaw, md->yaw_sweep_direction * 0.55850536f);
                if (absolute_float(spring_angle(ent + 0x18,
                                                sweep_target_yaw,
                                                (char *)md + 0x94,
                                                0,
                                                frame_scale_sq_macro * 0.022f,
                                                frame_scale_sq_macro * 0.3f,
                                                D_0015ED6C_gp * 0.2617994f +
                                                    (float)i * (D_0015ED6C_gp * 0.05235988f))) <
                    0.017453292f) {
                    md->yaw_sweep_direction = -md->yaw_sweep_direction;
                }
            }
            flag = 0;
            hp = (float *)(((char *)&hero) + 0x80);
            owner_to_hero_distance_xy = vector_distance_xy(moby + 0x10, hp);
            dz = absolute_float(hp[2] - MOBY(moby)->pos.z);
            if (scale_game_frames(0x4B0) < l12_planetbuster_maximus_proximity_fire_counter) {
                if (owner_to_hero_distance_xy < 80.0f && dz < 4.0f) {
                    flag = 1;
                }
            }
            if (owner_to_hero_distance_xy > 90.0f || dz > 7.0f) {
                l12_planetbuster_maximus_proximity_fire_counter = 0;
            }
            if ((D_L12_00161F00_gp != 0 || flag) && md->f80 > 0.0f) {
                fast_dec_timer_s16(&md->firing_inhibit_timer);
                fast_dec_timer_s16(&md->shot_cooldown_timer);
                if (md->firing_inhibit_timer == 0) {
                    if (fast_difference_between_rotations(
                            *(float *)(owner_data + gun_entry_index * 0x30 + 0x18), yaw) <
                        0.06981317f) {
                        md->aim_alignment_fire_holdoff_timer = scale_game_frames(0x14);
                    }
                    if (fast_dec_timer_s16(&md->aim_alignment_fire_holdoff_timer) == 0) {
                        if (FUN_L00_001fefc8(&md->firing_voice_window_timer) == 0) {
                            if (FUN_L00_001fefc8(&md->firing_voice_repeat_timer) != 0) {
                                allocate_voice_for_target_entry(0, 0, gun_moby);
                                md->firing_voice_repeat_timer = scale_game_frames(9);
                            }
                        }
                        if (fast_dec_timer_s16(&md->shot_cooldown_timer) != 0) {
                            float rz;
                            float pitch;
                            int   n;
                            moby_point_position(
                                gun_moby, md->firing_point_selector + 1, position_buffer);
                            md->firing_point_selector = md->firing_point_selector == 0;
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
                            if (spawn_blarg_trooper_projectile(gun_moby,
                                                               position_buffer,
                                                               scratch_vector,
                                                               scale_game_frames(0x96)) != 0) {
                                int j;
                                md->firing_voice_window_timer = scale_game_frames(0x14);
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
                            md->shot_cooldown_timer = scale_game_frames(7);
                            n = D_L12_00161F20;
                            if (scale_game_frames(0xB) < n) {
                                n = scale_game_frames(0xB);
                            }
                            if (n < 0) {
                                n = 0;
                            }
                            md->shot_cooldown_timer += n;
                        }
                    }
                }
            }
            if (D_L12_00161F04_gp == 0 && md->f80 > 0.0f) {
                if (fast_dec_timer(&md->damage_cooldown_timer) != 0) {
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
                        l12_planetbuster_maximus_proximity_fire_counter = scale_game_frames(0xFA0);
                        goto done;
                    }
                    md->damage_cooldown_timer = scale_game_frames(0x14);
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
        MOBY(moby)->state = PLANETBUSTER_DROP_PARTS_AND_TRANSITION;
        MOBY(moby)->command = 0;
    }
}

void initialize_planetbuster_maximus_detached_part_motion(struct Moby *moby, int slot) {
    char        *motion_owner_pvars = (char *)moby->pvars;
    struct Moby *selected;
    float       *speed;
    int          vertical_step_decrement_byte_offset;
    float        initial_speed;
    float        angle_scale;
    float        first_angle;
    float        second_scale;
    char        *slot_data;

    if (slot == 0x11) {
        selected = moby;
        vertical_step_decrement_byte_offset = 0x44;
    } else {
        selected = (struct Moby *)(D_L12_0015FFD8_p + D_L12_001FB688[slot] * 0x100);
        vertical_step_decrement_byte_offset = slot * 4;
    }

    initial_speed = random_float_between(frame_time_sq * 3.7f, frame_time_sq * 5.0f);
    angle_scale = frame_time;
    {
        char *vertical_step_decrements = motion_owner_pvars + 0x570;
        speed = (float *)(vertical_step_decrements + vertical_step_decrement_byte_offset);
    }
    *speed = initial_speed;
    first_angle = randf_sym(angle_scale * 0.05235988f, angle_scale * 0.13962634f);
    second_scale = frame_time;
    slot_data = motion_owner_pvars + slot * 0x10;
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

#include "rnc/overlay/l12/mobys/planetbuster_maximus_part_globals.h"

extern float D_L12_00161F60;
extern float D_L12_00161F70;

void update_planetbuster_maximus_detached_parts(struct Moby *moby) {
    int   i;
    int   vertical_step_decrement_offset;
    char *detached_parts_owner_data = ((char *)moby->pvars);
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
        position_or_vertical_step =
            *(float *)(detached_parts_owner_data + 0x338 + i * 16) -
            *(float *)(detached_parts_owner_data + vertical_step_decrement_offset + 0x570);
        *(float *)(detached_parts_owner_data + 0x338 + i * 16) = position_or_vertical_step;
        MOBY(slot_moby)->pos.z = MOBY(slot_moby)->pos.z + position_or_vertical_step;
        if (MOBY(slot_moby)->oclass == 0x500) {
            MOBY(slot_moby)->rot.x =
                fast_add_rotations(MOBY(slot_moby)->rot.x, D_0015ED6C * 0.34906584f);
            spring_angle(slot_moby + 0x44,
                         1.5707964f,
                         detached_parts_owner_data + 0x454 + i * 16,
                         0,
                         0.00200000009f,
                         0.3f,
                         0.0f);
        } else if (MOBY(slot_moby)->oclass == 0x4FD) {
            MOBY(slot_moby)->rot.x = fast_add_rotations(
                MOBY(slot_moby)->rot.x, *(float *)(detached_parts_owner_data + 0x450 + i * 16));
            spring_angle(slot_moby + 0x44,
                         -1.5707964f,
                         (float *)(detached_parts_owner_data + 0x450 + i * 16) + 1,
                         0,
                         *(float *)(detached_parts_owner_data + 0x45C + i * 16),
                         0.3f,
                         0.0f);
        } else if (MOBY(slot_moby)->oclass == 0x4FA) {
            float updated_rotation_angle;
            MOBY(slot_moby)->rot.x =
                fast_add_rotations(MOBY(slot_moby)->rot.x, D_0015ED6C * -0.87266463f);
            updated_rotation_angle = fast_add_rotations(
                MOBY(slot_moby)->rot.y, *(float *)(detached_parts_owner_data + 0x454 + i * 16));
            MOBY(slot_moby)->pos.y = MOBY(slot_moby)->pos.y + D_0015ED6C * -4.0f;
            MOBY(slot_moby)->rot.y = updated_rotation_angle;
        } else {
            MOBY(slot_moby)->pos.y = MOBY(slot_moby)->pos.y + D_0015ED6C * -4.0f;
            MOBY(slot_moby)->rot.x = fast_add_rotations(
                MOBY(slot_moby)->rot.x, *(float *)(detached_parts_owner_data + 0x450 + i * 16));
            MOBY(slot_moby)->rot.y = fast_add_rotations(
                MOBY(slot_moby)->rot.y, *(float *)(detached_parts_owner_data + 0x454 + i * 16));
        }
        if (i == last) {
            qcopy((char *)&D_L12_00161F40, slot_moby + 0x10);
            qcopy((char *)&planetbuster_maximus_detached_owner_rotation, slot_moby + 0x40);
        }
        update_entity(slot_moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00307C08), where it is exact; names translated to the US level program. */

extern int D_L12_0015F5CC;

void update_planetbuster_maximus_glow_color(struct Moby *moby) __asm__("FUN_L12_00306828");

void update_planetbuster_maximus_glow_color(struct Moby *moby) {
    int   i;
    int   alpha = 0x80000000;
    char *linked_parts_owner_pvars = (char *)moby->pvars;
    for (i = 0; i < 18; i++) {
        struct Moby *glow_target_moby;
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
            glow_target_moby = moby;
        } else {
            glow_target_moby =
                (struct Moby *)((char *)(*(struct Moby **)&D_L12_0015FFD8_p) +
                                ((unsigned int)*(int *)(linked_parts_owner_pvars + i * 0x30 + 0x20)
                                 << 8));
        }
        if (glow_target_moby->oclass == 0x4FE)
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
            glow_target_moby->flags |= 0x10;
            glow_target_moby->glow_color = packed_color;
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

#include "rnc/overlay/l12/mobys/planetbuster_maximus_part_data/planetbuster_maximus_part_update.h"

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

typedef struct PlanetbusterMaximusVars {
    u8  pad_0[0x5C0];
    s32 path;
    u8  pad_5C4[0x4];
    f32 vertical_oscillation_previous_offset;
    f32 vertical_oscillation_phase;
    u8  pad_5D0[0x14];
    u8  rotation_common_factor_phase;
    u8  pad_5E5[0x3];
    u8  rotation_component_mix_phase;
    u8  pad_5E9[0x3];
    s32 path_start_clip_volume;
    f32 path_movement_step;
    f32 yaw_turn_step;
    s16 path_node;
    s16 detached_parts_update_count;
    s16 remaining_gun_health;
    u8  pad_5FE[0x6];
    s32 voice_slot;
    s32 camera_point;
    s32 transition_target_camera_point;
    f32 camera_transition_fraction;
    f32 camera_transition_step;
    f32 base_z;
    s32 height_timer_refresh_clip_volume;
    s32 timer;
    s32 linked_parts_flags_clip_volume;
    s32 height_raise_clip_volume;
    f32 height_spring_velocity;
} PlanetbusterMaximusVars;

/* Moby class 1274 */
void planetbuster_maximus_part_update(char *moby) {
    enum {
        PLANETBUSTER_TRANSITION_SETUP = 0,
        PLANETBUSTER_TRANSITION_INTERPOLATE = 1,
        PLANETBUSTER_TRANSITION_CLEANUP = 2
    };
    float                    v[4];
    float                    transition_rotation[4];
    PlanetbusterMaximusVars *moby_data = (PlanetbusterMaximusVars *)MOBY(moby)->pvars;
    if (((unsigned char *)moby)[0x20] != 0 && D_L12_001FB688[17] != 0) {
        qcopy(moby + 0x10, (char *)&D_L12_00161F60_gp);
        qcopy(moby + 0x40, (char *)&D_L12_00161F70_gp);
    }
    switch ((unsigned char)MOBY(moby)->state) {
    case PLANETBUSTER_INITIALIZE_LINKED_PARTS:
        D_L12_001619B4 = 0;
        if (visited52[(unsigned char)MOBY(moby)->mission + (current_level_index_gp << 4)] == 0xFF) {
            int i;
            for (i = 0; i < 17; i++) {
                int s = *(int *)((u8 *)moby_data + i * 0x30 + 0x20);
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
            path = ((P_7DB0 **)&(*(int *)l12_paths))[moby_data->path];
            qcopy(moby + 0x10, path + 1);
            moby_data->base_z = MOBY(moby)->pos.z;
            MOBY(moby)->rot.z =
                fast_arc_tan(path[2].v[0] - path[1].v[0], path[2].v[1] - path[1].v[1]);
            if (*(float *)(D_0013F3D0 + 0x8) < 52.0f) {
                MOBY(moby)->pos.z = MOBY(moby)->pos.z + 3.0f;
            }
            for (i = 0; i < 5; i++) {
                int   k = *(int *)((u8 *)moby_data + 0x5D0 + i * 4);
                char *o =
                    (char *)D_L12_0015FFD8_gp + (*(int *)((u8 *)moby_data + k * 0x30 + 0x20) << 8);
                (*(F80_7DB0 **)(o + 0x78))->f80 = 10.0f;
            }
            D_L12_00161F40_gp = -1.0f;
            for (j = 17; j >= 0; j--)
                D_L12_001FB688[j] = 0;
            moby_data->voice_slot = -1;
            moby_data->remaining_gun_health = 100;
            MOBY(moby)->state = PLANETBUSTER_WAIT_FOR_PATH_TRIGGER;
        }
        break;
    case PLANETBUSTER_WAIT_FOR_PATH_TRIGGER: {
        char *clip_test_position;
        int   r;
        if (is_point_inside_clip_volume(D_L12_00167240,
                                        moby_data->linked_parts_flags_clip_volume)) {
            int   i;
            char *base = (char *)D_L12_0015FFD8_gp;
            MOBY(moby)->flags |= 0x41;
            for (i = 0; i < 17; i++) {
                int s = *(int *)((u8 *)moby_data + i * 0x30 + 0x20);
                if (s != -1)
                    ((M34_7DB0 *)(base + (s << 8)))->flags |= 0x41;
            }
        } else {
            int   i;
            char *base = (char *)D_L12_0015FFD8_gp;
            MOBY(moby)->flags &= ~0x41;
            for (i = 0; i < 17; i++) {
                int s = *(int *)((u8 *)moby_data + i * 0x30 + 0x20);
                if (s != -1)
                    ((M34_7DB0 *)(base + (s << 8)))->flags &= ~0x41;
            }
        }
        clip_test_position = D_0013F3D0;
        r = is_point_inside_clip_volume(clip_test_position, moby_data->height_raise_clip_volume);
        if (*(float *)(clip_test_position + 8) > 52.0f && r == 0) {
            approach_value(
                (float *)&MOBY(moby)->pos.z, moby_data->base_z, D_0015ED6C_gp + D_0015ED6C_gp);
        } else {
            float add = 3.0f;
            if (r)
                add = 10.0f;
            if (is_point_inside_clip_volume(D_0013F3D0,
                                            moby_data->height_timer_refresh_clip_volume)) {
                moby_data->timer = scale_game_frames(0x5A);
            }
            if (!fast_dec_timer(&moby_data->timer))
                add = 19.0f;
            vendor_spring((float *)&MOBY(moby)->pos.z,
                          &moby_data->height_spring_velocity,
                          moby_data->base_z + add,
                          frame_scale_sq_macro * 0.03f,
                          frame_scale_sq_macro * 0.3f,
                          D_0015ED6C_gp * 2.7f);
        }
        update_moby_vertical_oscillation(0.7f,
                                         D_0015ED6C_gp * 1.2217305f,
                                         moby,
                                         &moby_data->vertical_oscillation_phase,
                                         &moby_data->vertical_oscillation_previous_offset);
        update_moby_rotation_oscillation(
            moby,
            &moby_data->rotation_common_factor_phase,
            &moby_data->rotation_component_mix_phase,
            planetbuster_maximus_rotation_oscillation_amplitude_gp,
            planetbuster_maximus_rotation_oscillation_common_factor_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp,
            planetbuster_maximus_rotation_oscillation_component_mix_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp);
        apply_linked_moby_relative_transforms(moby, moby_data, 0x11);
        if (is_point_inside_clip_volume(D_0013F3D0, moby_data->path_start_clip_volume) ||
            D_L12_00161F00_gp) {
            int i;
            D_L12_001619B4 = 1;
            for (i = 0; i < 5; i++) {
                int k = *(int *)((u8 *)moby_data + 0x5D0 + i * 4);
                ((unsigned char *)D_L12_0015FFD8_gp +
                 (*(int *)((u8 *)moby_data + k * 0x30 + 0x20) << 8))[0xA4] = 0xFF;
            }
            MOBY(moby)->state = PLANETBUSTER_FOLLOW_PATH;
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
                                         &moby_data->vertical_oscillation_phase,
                                         &moby_data->vertical_oscillation_previous_offset);
        update_moby_rotation_oscillation(
            moby,
            &moby_data->rotation_common_factor_phase,
            &moby_data->rotation_component_mix_phase,
            planetbuster_maximus_rotation_oscillation_amplitude_gp,
            planetbuster_maximus_rotation_oscillation_common_factor_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp,
            planetbuster_maximus_rotation_oscillation_component_mix_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp);
        path = ((P_7DB0 **)&(*(int *)l12_paths))[moby_data->path];
        if (moby_data->path_node < *(int *)path - 11) {
            approach_value(
                &moby_data->path_movement_step, D_0015ED6C_gp * 8.0f, frame_time_sq_macro * 10.0f);
        } else {
            approach_value(&moby_data->path_movement_step, 0.0f, frame_time_sq_macro * 8.0f);
            if (moby_data->path_movement_step == 0.0f) {
                MOBY(moby)->state = PLANETBUSTER_POST_PATH_GUN_UPDATE;
                break;
            }
        }
        nx = (moby_data->path_node + 1) % *(int *)path;
        fast_vec_sub(v, &path[nx + 1], &path[moby_data->path_node + 1]);
        fast_vec_normalize(v, v, moby_data->path_movement_step);
        fast_vec_add(moby + 0x10, moby + 0x10, v);
        target_yaw =
            fast_arc_tan(*(float *)((char *)path + (nx << 4) + 0x10) -
                             *(float *)((char *)path + (moby_data->path_node << 4) + 0x10),
                         *(float *)((char *)path + (nx << 4) + 0x14) -
                             *(float *)((char *)path + (moby_data->path_node << 4) + 0x14));
        if (*(int *)path - 20 < moby_data->path_node)
            target_yaw = 3.0543261f;
        FUN_L00_00258278(moby,
                         target_yaw,
                         &moby_data->yaw_turn_step,
                         frame_scale_sq_macro * 0.0015f,
                         frame_scale_sq_macro * 0.3f,
                         D_0015ED6C_gp * 0.17453292f);
        if (vector_distance_xy(moby + 0x10, &path[moby_data->path_node + 1]) < 5.0f ||
            fast_difference_between_rotations(
                MOBY(moby)->rot.z,
                fast_arc_tan(*(float *)((char *)path + (moby_data->path_node << 4) + 0x10) -
                                 MOBY(moby)->pos.x,
                             *(float *)((char *)path + (moby_data->path_node << 4) + 0x14) -
                                 MOBY(moby)->pos.y)) > 1.5707964f) {
            moby_data->path_node = (moby_data->path_node + 1) % *(int *)path;
        }
        apply_linked_moby_relative_transforms(moby, moby_data, 0x11);
        update_planetbuster_maximus_guns(moby);
        break;
    }
    case PLANETBUSTER_POST_PATH_GUN_UPDATE:
        l12_planetbuster_maximus_proximity_fire_counter++;
        spring_angle(moby + 0x48,
                     3.0543261f,
                     &moby_data->yaw_turn_step,
                     -1,
                     frame_scale_sq_macro * 0.0015f,
                     frame_scale_sq_macro * 0.3f,
                     D_0015ED6C_gp * 0.17453292f);
        update_moby_vertical_oscillation(0.7f,
                                         D_0015ED6C_gp * 1.2217305f,
                                         moby,
                                         &moby_data->vertical_oscillation_phase,
                                         &moby_data->vertical_oscillation_previous_offset);
        update_moby_rotation_oscillation(
            moby,
            &moby_data->rotation_common_factor_phase,
            &moby_data->rotation_component_mix_phase,
            planetbuster_maximus_rotation_oscillation_amplitude_gp,
            planetbuster_maximus_rotation_oscillation_common_factor_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp,
            planetbuster_maximus_rotation_oscillation_component_mix_phase_rate_degrees *
                0.017453292f * D_0015ED6C_gp);
        apply_linked_moby_relative_transforms(moby, moby_data, 0x11);
        update_planetbuster_maximus_guns(moby);
        break;
    case PLANETBUSTER_DROP_PARTS_AND_TRANSITION:
        if (moby_data->camera_point != -1 && moby_data->transition_target_camera_point != -1) {
            switch ((unsigned char)MOBY(moby)->command) {
            case PLANETBUSTER_TRANSITION_SETUP:
                MOBY(moby)->command = 1;
                set_camera_position(D_L12_001600EC + moby_data->camera_point * 128 + 0x30);
                set_camera_rotation(D_L12_001600EC + moby_data->camera_point * 128 + 0x70);
                D_L12_0015F404 = 1;
                D_L12_001619A0 = 1;
                break;
            case PLANETBUSTER_TRANSITION_INTERPOLATE:
                if (moby_data->camera_transition_fraction < 1.0f) {
                    advance_accelerated_scalar(&moby_data->camera_transition_fraction,
                                               &moby_data->camera_transition_step,
                                               1.0f,
                                               frame_time_sq_macro * 0.666f,
                                               frame_time_sq_macro * 0.666f,
                                               D_0015ED6C_gp * 0.4f);
                    lerp_vector(v,
                                D_L12_001600EC + moby_data->camera_point * 128 + 0x30,
                                D_L12_001600EC + moby_data->transition_target_camera_point * 128 +
                                    0x30,
                                moby_data->camera_transition_fraction);
                    clear_u64_value(transition_rotation);
                    transition_rotation[0] =
                        fast_subtract_rotations(
                            *(float *)(D_L12_001600EC +
                                       moby_data->transition_target_camera_point * 128 + 0x70),
                            *(float *)(D_L12_001600EC + moby_data->camera_point * 128 + 0x70)) *
                        moby_data->camera_transition_fraction;
                    transition_rotation[0] = fast_add_rotations(
                        *(float *)(D_L12_001600EC + moby_data->camera_point * 128 + 0x70),
                        transition_rotation[1]);
                    transition_rotation[1] =
                        fast_subtract_rotations(
                            *(float *)(D_L12_001600EC +
                                       moby_data->transition_target_camera_point * 128 + 0x74),
                            *(float *)(D_L12_001600EC + moby_data->camera_point * 128 + 0x74)) *
                        moby_data->camera_transition_fraction;
                    transition_rotation[1] = fast_add_rotations(
                        *(float *)(D_L12_001600EC + moby_data->camera_point * 128 + 0x74),
                        transition_rotation[1]);
                    transition_rotation[2] =
                        fast_subtract_rotations(
                            *(float *)(D_L12_001600EC +
                                       moby_data->transition_target_camera_point * 128 + 0x78),
                            *(float *)(D_L12_001600EC + moby_data->camera_point * 128 + 0x78)) *
                        moby_data->camera_transition_fraction;
                    transition_rotation[2] = fast_add_rotations(
                        *(float *)(D_L12_001600EC + moby_data->camera_point * 128 + 0x78),
                        transition_rotation[2]);
                    set_camera_position(v);
                    set_camera_rotation(transition_rotation);
                } else {
                    MOBY(moby)->command = 2;
                    D_L12_0015F404 = 0;
                    jet_end_mount(3);
                }
                break;
            case PLANETBUSTER_TRANSITION_CLEANUP: {
                int i;
                for (i = 0; i < 17; i++) {
                    int s = *(int *)((u8 *)moby_data + i * 0x30 + 0x20);
                    if (s != -1) {
                        char *o = (char *)D_L12_0015FFD8_gp + (s << 8);
                        if (o[0x20] >= 0)
                            mark_moby_for_removal(o);
                        *(int *)((u8 *)moby_data + i * 0x30 + 0x20) = -1;
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
        if (moby_data->detached_parts_update_count == 0) {
            int i;
            for (i = 0; i < 17; i++) {
                if (D_L12_001FB688[i] == 0)
                    D_L12_001FB688[i] = *(int *)((u8 *)moby_data + i * 0x30 + 0x20);
                *(int *)((u8 *)moby_data + i * 0x30 + 0x20) = -1;
            }
            D_L12_001FB688[17] = 1;
            for (i = 0; i < 18; i++)
                initialize_planetbuster_maximus_detached_part_motion((struct Moby *)moby, i);
        }
        moby_data->detached_parts_update_count = moby_data->detached_parts_update_count + 1;
        update_planetbuster_maximus_detached_parts(moby);
        break;
    }
    update_planetbuster_maximus_glow_color(moby);
    if ((unsigned char)MOBY(moby)->state >= PLANETBUSTER_WAIT_FOR_PATH_TRIGGER &&
        (unsigned char)MOBY(moby)->state <= PLANETBUSTER_POST_PATH_GUN_UPDATE) {
        if (moby_data->voice_slot == -1)
            moby_data->voice_slot = allocate_voice_for_target_entry(0, 4, moby);
    } else {
        int s = moby_data->voice_slot;
        if (s != -1) {
            Voice_7DB0 *voice_pool_entry = (Voice_7DB0 *)(((unsigned char *)D_0013E550) + s * 0x70);
            if (voice_pool_entry->owner == moby && voice_pool_entry->active != 0)
                release_voice_slot(s);
        }
        moby_data->voice_slot = -1;
    }
}

float D_L12_00161F60_gp __asm__("D_L12_00161F60") = 0;
float D_L12_00161F70_gp __asm__("D_L12_00161F70") = 0;

float D_L12_00161F40_gp __asm__("D_L12_00161F40") = 0;
