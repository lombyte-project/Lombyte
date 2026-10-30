/* Level 18 rocket_tank. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float fast_add_rotations(float, float);
extern float fast_cos(float);
extern float fast_sin(float);
extern int   scale_game_frames(int);

extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32   allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
s32          is_value_within_interpolated_window(void *arg0, f32 fparg0) __asm__("FUN_00214cc8");
extern char *create_moby(int) __asm__("FUN_0020c4f8");
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F90A0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0xC0];
    char  buf[0x50];
    float f110;
    float pad114;
    float f118;
    char  pad11C[4];
    int   w120;
    int   w124;
    char  pad128[8];
    float f130;
    float f134;
    char *c138;
    char *c13c;
} D1454;

typedef struct {
    char          pad0[0x20];
    unsigned char state;
    char          pad21[0x32];
    unsigned char b53;
    char          pad54[0x24];
    D1454        *data;
} M1454;

extern char *
create_class_29_moby(char *owner, float *pos, float *vec, int arg3) __asm__("FUN_L18_002a75f0");
extern char *create_moby(int);
extern short D_L18_0016247C_gp __asm__("D_L18_0016247C") __attribute__((sda));
extern void  emit_radial_particle_burst(void *, void *, int) __asm__("FUN_L03_00250ae8");
f32          compute_interpolated_record_value(void *arg0) __asm__("FUN_0020c9e0");
extern short D_L18_0016247C __asm__("D_L18_0016247C");
extern void  func_L18_002F9AB8_v(void *) __asm__("FUN_L18_002f8658");

void rocket_tank_update(M1454 *m) __asm__("FUN_L18_002f7c40");
void update_rocket_tank_damage_and_aim(void *mv) __asm__("FUN_L18_002f8050");

void rocket_tank_update(M1454 *moby) {
    enum {
        ROCKET_TANK_INITIALIZE = 0,
        ROCKET_TANK_PATROLLING = 1,
        ROCKET_TANK_FIRING = 2
    };
    D1454 *tank_data = moby->data;
    update_rocket_tank_damage_and_aim(moby);
    switch (moby->state) {
    case ROCKET_TANK_INITIALIZE:
        attach_manipulator((char *)moby, 0, tank_data->buf);
        *(short *)((char *)tank_data + 0x24) = 6;
        *(unsigned char *)((char *)tank_data + 0x28) = 3;
        *(float *)((char *)tank_data + 0x20) = 6.0f;
        tank_data->w124 = scale_game_frames(240);
        tank_data->w120 = 0;
        if (tank_data->c138 == 0) {
            char *c = create_moby(0x14B);
            tank_data->c138 = c;
            c[0x30] = 0x40;
            *(short *)(tank_data->c138 + 0x32) = 0x40;
            tank_data->c138[0x31] = 1;
            {
                char *pl = ((char *)hero.moby);
                long  t = *(long *)(pl + 0x38);
                *(long *)(tank_data->c138 + 0x38) = t;
            }
            *(unsigned short *)(tank_data->c138 + 0x34) = 0x4000;
            {
                char *t = tank_data->c138;
                qcopy(t + 0x10, (char *)moby + 0x10);
                *(int *)(t + 0x94) = 0;
            }
            blend_moby_animation(tank_data->c138, 0, 0, 0);
            update_entity(tank_data->c138);
        }
        if (tank_data->c13c == 0) {
            char *c = create_moby(0x14B);
            tank_data->c13c = c;
            c[0x30] = 0x40;
            *(short *)(tank_data->c13c + 0x32) = 0x40;
            tank_data->c13c[0x31] = 1;
            {
                char *pl = ((char *)hero.moby);
                long  t = *(long *)(pl + 0x38);
                *(long *)(tank_data->c13c + 0x38) = t;
            }
            *(unsigned short *)(tank_data->c13c + 0x34) = 0x8000;
            {
                char *t = tank_data->c13c;
                qcopy(t + 0x10, (char *)moby + 0x10);
                *(int *)(t + 0x94) = 0;
            }
            blend_moby_animation(tank_data->c13c, 0, 0, 0);
            update_entity(tank_data->c13c);
        }
        if (moby->b53 != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        moby->state = 1;
        break;
    case ROCKET_TANK_PATROLLING:
        func_L18_002F9AB8_v(moby);
        if (*(int *)((char *)tank_data + 0xB4) != 2) {
            tank_data->f134 = 0;
            tank_data->f130 = 10.0f;
            if (moby->b53 != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(20));
            }
            moby->state = 2;
        }
        break;
    case ROCKET_TANK_FIRING:
        func_L18_002F9AB8_v(moby);
        approach_value(&tank_data->f130, 30.0f, frame_time * 6.0f);
        tank_data->f134 = fast_add_rotations(tank_data->f134, frame_time * 6.2831855f);
        if (is_value_within_interpolated_window((char *)moby, 4.0f) ||
            is_value_within_interpolated_window((char *)moby, 10.0f)) {
            float firing_vector[4];
            float projectile_spawn_position[4];
            float firing_vector_scale;
            if (compute_interpolated_record_value(moby) < 7.0f) {
                moby_point_position(moby, 1, projectile_spawn_position);
                firing_vector_scale = *(float *)&D_L18_0016247C_gp;
            } else {
                moby_point_position(moby, 2, projectile_spawn_position);
                firing_vector_scale = *(float *)&D_L18_0016247C;
            }
            firing_vector_scale *= frame_time;
            firing_vector[0] =
                fast_cos(tank_data->f110) * (fast_cos(tank_data->f118) * firing_vector_scale);
            firing_vector[1] =
                fast_sin(tank_data->f110) * (fast_cos(tank_data->f118) * firing_vector_scale);
            firing_vector[2] = 0;
            firing_vector[2] = -fast_sin(tank_data->f118) * firing_vector_scale;
            allocate_voice_for_target_entry(0, 0, (int)moby);
            emit_radial_particle_burst(moby, projectile_spawn_position, 0);
            if (vector_distance_xy((char *)moby + 0x10, &hero.motion.pos) > 10.0f) {
                create_class_29_moby((char *)moby, projectile_spawn_position, firing_vector, 0);
            } else {
                create_class_29_moby((char *)moby, projectile_spawn_position, firing_vector, 1);
            }
        }
        if (*(int *)((char *)tank_data + 0xB4) == 2) {
            if (moby->b53 != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(20));
            }
            moby->state = 1;
        }
        break;
    }
}

extern float D_L18_0015F580[] MACRO_ADDR;
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern void  func_L00_0025F4A8_alt(void *,
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
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F8F38.c: func_L18_002F94B0), where it is exact; names translated to the US level program. */

extern float D_L18_0015F580[];
extern float vector_length_xy(void *);
extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");
extern int   select_moby_combat_target(unsigned char *, char *, float) __asm__("FUN_L00_0025fcb8");
extern void *FUN_L00_00257470_cf(char  *src,
                                 int    cls,
                                 float *pos,
                                 void  *mat,
                                 int    a8,
                                 int    a9,
                                 float  scale,
                                 float *v10,
                                 float *v11,
                                 float *v12) __asm__("FUN_L00_00263fd8");
extern void  FUN_L00_001fff28(void *, int, float);
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  mark_moby_for_removal_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void  func_L00_0025F4A8_alt(void *,
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

void update_rocket_tank_damage_and_aim(void *mv) {
    unsigned char *moby_bytes = mv;
    char          *tank_data = *(char **)(moby_bytes + 0x78);
    int            r;
    float          damage_amount;
    float          explosion_and_aim_scratch[4];
    int            hit_reaction_status;
    hit_reaction(moby_bytes,
                 find_moby_hit(moby_bytes, 0x330000, 0),
                 tank_data + 0x20,
                 0,
                 &hit_reaction_status,
                 &damage_amount,
                 0,
                 4);
    if (hit_reaction_status != 1 && moby_bytes[0x20] != 0) {
        float remaining_health = *(float *)(tank_data + 0x20) - damage_amount;
        *(float *)(tank_data + 0x20) = remaining_health;
        if (remaining_health > 0.0f) {
            tank_data[0x67] = 0x78;
        } else {
            float *pa;
            float *dv;
            qcopy(explosion_and_aim_scratch, moby_bytes + 0x10);
            explosion_and_aim_scratch[2] = explosion_and_aim_scratch[2] + 1.0f;
            pa = (float *)(moby_bytes + 0x10);
            func_L00_0025F4A8_alt(moby_bytes,
                                  tank_data + 0x40,
                                  explosion_and_aim_scratch,
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
            dv = D_L18_0015F580;
            FUN_L00_00257470_cf(moby_bytes, 0x700, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x701, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x702, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x704, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FE, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FE, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FF, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FF, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            FUN_L00_00257470_cf(moby_bytes, 0x6FF, pa, moby_bytes + 0x40, 0, 0, 0.0f, dv, dv, dv);
            mark_moby_for_removal_cf(moby_bytes, 0, -1);
            mark_moby_for_removal(moby_bytes);
            if (*(char **)(tank_data + 0x138)) {
                mark_moby_for_removal(*(char **)(tank_data + 0x138));
            }
            if (*(char **)(tank_data + 0x13C)) {
                mark_moby_for_removal(*(char **)(tank_data + 0x13C));
            }
            return;
        }
        trooper_anim_update(moby_bytes, (short *)(tank_data + 0x60));
    }
    moby_bytes[0xA4] = 0xFF;
    if (select_moby_combat_target(moby_bytes, tank_data + 0x70, 30.0f) != 2) {
        float *pa = (float *)(moby_bytes + 0x10);
        char  *pl = (char *)&hero.motion.pos;
        if (vector_distance_xy(pa, pl) < 10.0f) {
            char *hero_base_bytes = pl - 0x80;
            *(char **)(tank_data + 0xB0) = *(char **)(hero_base_bytes + 0x2080);
            qcopy(tank_data + 0x70, *(char **)(hero_base_bytes + 0x2080) + 0x10);
        } else if (*(char **)(tank_data + 0xB0)) {
            if (30.0f < vector_distance_xy(pa, *(char **)(tank_data + 0xB0) + 0x10)) {
                *(int *)(tank_data + 0xB4) = 2;
            }
        }
    }
    if (*(int *)(tank_data + 0xB4) != 2) {
        float aim_yaw, aim_pitch;
        fast_vec_sub(explosion_and_aim_scratch, tank_data + 0x70, moby_bytes + 0x10);
        explosion_and_aim_scratch[2] = explosion_and_aim_scratch[2] - 3.0f;
        if (fast_vec_length(explosion_and_aim_scratch) > *(float *)(tank_data + 0x130)) {
            set_vector_length_xy(explosion_and_aim_scratch,
                                 explosion_and_aim_scratch,
                                 *(float *)(tank_data + 0x130));
        }
        aim_yaw = fast_sin(*(float *)(tank_data + 0x134)) * 0.08726646f;
        aim_yaw = fast_add_rotations(
            fast_arc_tan(explosion_and_aim_scratch[0], explosion_and_aim_scratch[1]), aim_yaw);
        aim_pitch = -fast_arc_tan(vector_length_xy(explosion_and_aim_scratch),
                                  explosion_and_aim_scratch[2]);
        if (aim_pitch > 0.34906584f) {
            aim_pitch = 0.34906584f;
        } else if (aim_pitch < -0.7853982f) {
            aim_pitch = -0.7853982f;
        }
        approach_angle((float *)(tank_data + 0x110),
                       (float *)(tank_data + 0x114),
                       aim_yaw,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 9.424778f);
        approach_angle((float *)(tank_data + 0x118),
                       (float *)(tank_data + 0x11C),
                       aim_pitch,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 9.424778f);
    } else {
        approach_angle((float *)(tank_data + 0x110),
                       (float *)(tank_data + 0x114),
                       *(float *)(moby_bytes + 0x48),
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 9.424778f);
        approach_angle((float *)(tank_data + 0x118),
                       (float *)(tank_data + 0x11C),
                       0.0f,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 9.424778f);
    }
    FUN_L00_001fff28(explosion_and_aim_scratch, 1, *(float *)(tank_data + 0x118));
    FUN_L00_001fff28(
        tank_data + 0xD0,
        2,
        fast_subtract_rotations(*(float *)(tank_data + 0x110), *(float *)(moby_bytes + 0x48)));
    multiply_quaternions(tank_data + 0xD0, explosion_and_aim_scratch, tank_data + 0xD0);
    trooper_anim_finish(moby_bytes, tank_data + 0x60);
}

extern s32  random_integer_below(s32) __asm__("FUN_00213260");
extern int  random_integer_below(int);
extern int *D_L18_001B0EB0[];
extern int  random_integer_below(int) __asm__("FUN_00213260");
s32         random_integer_below(s32 arg0) __asm__("FUN_00213260");

/* Steers the moby along its path toward the next node, slowing on sharp turns. */
extern short rocket_tank_path_target_speed_gp __asm__("D_L18_00162478") __attribute__((sda));
extern short rocket_tank_attached_moby_speed_reference_gp __asm__("D_L18_00162480")
    __attribute__((sda));
extern void FUN_L00_00258830(int, int, float, float, float, int);

void update_rocket_tank_path_movement(struct Moby *moby) __asm__("FUN_L18_002f8658");

void update_rocket_tank_path_movement(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float path_target_position[4];
    float movement_step[4];
    float path_target_yaw, target_movement_step, z, sc;
    char *attached_moby;
    qcopy(path_target_position,
          (char *)D_L18_001B0EB0[*(int *)(moby_vars + 0x100)] + *(int *)(moby_vars + 0x120) * 16 +
              0x10);
    path_target_yaw =
        fast_arc_tan(path_target_position[0] - moby->pos.x, path_target_position[1] - moby->pos.y);
    approach_angle(&moby->rot.z,
                   (float *)(moby_vars + 0x128),
                   path_target_yaw,
                   frame_time_sq * 3.1415927f,
                   frame_time_sq * 3.1415927f,
                   frame_time * 6.2831855f);
    target_movement_step = *(float *)&rocket_tank_path_target_speed_gp * frame_time;
    if (fast_difference_between_rotations(path_target_yaw, moby->rot.z) > 0.2617994f) {
        target_movement_step = frame_time;
    }
    approach_value((float *)(moby_vars + 0x12C), target_movement_step, frame_time_sq);
    z = 0.0f;
    fast_vec_normalize(movement_step, &moby->rot_row0, *(float *)(moby_vars + 0x12C));
    FUN_L00_00258830((int)moby, (int)movement_step, 0.5f, 0.5f, z, 0);
    if (fast_vec_dist(&moby->pos, path_target_position) < 1.0f) {
        int k = random_integer_below(*D_L18_001B0EB0[*(int *)(moby_vars + 0x100)] - 1) + 1;
        *(int *)(moby_vars + 0x120) =
            (*(int *)(moby_vars + 0x120) + k) % *D_L18_001B0EB0[*(int *)(moby_vars + 0x100)];
    }
    sc = *(float *)(moby_vars + 0x12C) /
         (*(float *)&rocket_tank_attached_moby_speed_reference_gp * frame_time);
    if (sc > 2.0f)
        sc = 2.0f;
    else if (sc < z)
        sc = z;
    attached_moby = *(char **)(moby_vars + 0x138);
    if (attached_moby) {
        char *q;
        MOBY(attached_moby)->anim_speed = sc;
        q = *(char **)(moby_vars + 0x138);
        qcopy(q + 0x40, &moby->rot);
        qcopy(q + 0x10, &moby->pos);
    }
    attached_moby = *(char **)(moby_vars + 0x13C);
    if (attached_moby) {
        char *q;
        MOBY(attached_moby)->anim_speed = sc;
        q = *(char **)(moby_vars + 0x13C);
        qcopy(q + 0x40, &moby->rot);
        qcopy(q + 0x10, &moby->pos);
    }
}
