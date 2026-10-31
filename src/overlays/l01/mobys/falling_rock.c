/* Level 01 moby class falling_rock. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/l01/mobys/falling_rock_globals.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/l01/hero/hero_set_state.h"

#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/level_labels.h"

extern int  cutscene_active __asm__("D_L01_0015F404");
extern void jet_end_mount(int) __asm__("FUN_L00_002eac18");

#include "sda.h"

#include "qcopy.h"

extern s32 current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

extern void
spawn_debris_chunk(void *, void *, u32, s32, f32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");

typedef struct {
    u8  pad0[0x80];
    s32 idx;
} Sub;

typedef struct {
    u8   pad0[0x78];
    Sub *sub;
} Obj;

f32        falling_rock_camera_rotation_angle_gp __asm__("D_L01_00161C28") = 0;
f32        falling_rock_camera_distance_gp __asm__("D_L01_00161C2C") = 7.0f;
f32        falling_rock_camera_height_gp __asm__("D_L01_00161C30") = 2.5f;
f32        falling_rock_camera_offset_scale_gp __asm__("D_L01_00161C34") = 3.5f;
extern f32 falling_rock_camera_override_companion_value_gp __asm__("D_L01_00161C38")
    __attribute__((sda));

extern void
     build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
void set_follow_camera_angle_blend_from_direction(void *, f32, f32) __asm__("FUN_L00_002e8918");
void follow_camera_set_distance(s32, f32, f32) __asm__("FUN_L00_002e8450");
void follow_camera_set_height(f32, f32) __asm__("FUN_L00_002e84b8");
void follow_camera_set_offset_scale(s32, f32, f32) __asm__("FUN_L00_002e84f0");

void apply_falling_rock_camera_overrides(Obj *falling_rock_moby) __asm__("FUN_L01_002f9000");

void apply_falling_rock_camera_overrides(Obj *falling_rock_moby) {
    u8           rotated_vector[16];
    LevelCuboid *camera_record;

    camera_record = &l01_cuboids[falling_rock_moby->sub->idx];
    build_look_at_matrix(rotated_vector,
                         &camera_record->axes[0],
                         &camera_record->axes[2],
                         falling_rock_camera_rotation_angle_gp);
    set_follow_camera_angle_blend_from_direction(rotated_vector, 0.13962634f, 0.0f);
    follow_camera_set_distance(
        0, falling_rock_camera_distance_gp, falling_rock_camera_override_companion_value_gp);
    follow_camera_set_height(falling_rock_camera_height_gp,
                             falling_rock_camera_override_companion_value_gp);
    follow_camera_set_offset_scale(
        0, falling_rock_camera_offset_scale_gp, falling_rock_camera_override_companion_value_gp);
}

typedef union {
    u128 q;
    f32  f[4];
} CapVec;

typedef struct {
    u32 c[3];
} CapColors;

typedef struct {
    u8     pad0[0x20];
    s32    w20;
    s16    h24;
    u8     pad26[2];
    u8     b28;
    u8     pad29[0x15];
    s16    h3E;
    u8     pad40[0x20];
    CapVec rot;
    CapVec home;
    s32    cam;
    s16    wait;
    s16    hold;
    s16    rise_len;
    s16    rise;
    s32    music;
} CapVars;

typedef struct {
    u8       pad0[0x10];
    CapVec   pos;
    u8       state;
    u8       pad21[0x57];
    CapVars *pvars;
    u8       pad7c[0x36];
    s16      id;
} CapMoby;

typedef struct {
    f32 x, y, z;
} CapVec3;

#include "rnc/overlay/level_save_state.h"

CapColors D_L01_00161C40 = {
    {0x2b8, 0x2b9, 0x2ba}
};

extern s32 D_0014C190[][64];
extern s32 cutscene_active __asm__("D_L01_0015F404");
void       apply_falling_rock_camera_overrides(Obj *obj) __asm__("FUN_L01_002f9000");

void falling_rock_update(CapMoby *m) __asm__("FUN_L01_002f9080");

/* Moby class 701 */
void falling_rock_update(CapMoby *falling_rock) {
    enum {
        FALLING_ROCK_STATE_INITIALIZE = 0,
        FALLING_ROCK_STATE_WAIT_FOR_TRIGGER = 1,
        FALLING_ROCK_STATE_WAIT_BEFORE_FALL = 2,
        FALLING_ROCK_STATE_FALLING = 3,
        FALLING_ROCK_STATE_FALLEN = 4
    };
    CapVars     *falling_rock_vars;
    LevelCuboid *camera_record;

    falling_rock_vars = falling_rock->pvars;
    if (falling_rock->state != 1 && !fast_dec_timer_s16(&falling_rock_vars->wait) &&
        random_integer_below(100) < 5) {
        CapColors colors;
        CapVec    debris_initial_velocity;
        CapVec    pos;
        f32       debris_spawn_radius;
        f32       debris_spawn_angle;

        colors = D_L01_00161C40;
        pos.q = 0;
        pos.f[0] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        pos.f[1] = random_float_between(frame_time_macro * -0.5f, frame_time_macro * 0.5f);
        debris_initial_velocity.q = pos.q;
        debris_spawn_radius = random_float_between(2.0f, 3.0f);
        debris_spawn_angle = random_angle_radians();
        pos.f[0] = fast_cos(debris_spawn_angle) * debris_spawn_radius;
        pos.f[1] = fast_sin(debris_spawn_angle) * debris_spawn_radius;
        pos.f[2] = 0.0f;
        fast_vec_add(&pos, &pos, &falling_rock_vars->home);
        spawn_debris_chunk(&pos,
                           &debris_initial_velocity,
                           colors.c[random_integer_below(3)],
                           random_int_between(0xB4, 0x12C),
                           0.05f,
                           1.0f,
                           1.0f,
                           0.75f,
                           0);
    }
    switch (falling_rock->state) {
    case FALLING_ROCK_STATE_INITIALIZE:
        falling_rock_vars->w20 = 0;
        falling_rock_vars->h24 = 0;
        falling_rock_vars->b28 = 4;
        falling_rock_vars->h3E = 5;
        qcopy(&falling_rock_vars->home, &falling_rock->pos);
        falling_rock->state = 1;
        if (l01_save_state.cleared[falling_rock->id] != 0 ||
            (D_0014C190[current_level_index_macro][falling_rock->id >> 5] >>
             (falling_rock->id & 0x1F)) &
                1) {
            clear_u64_value(&falling_rock_vars->rot);
            falling_rock->state = 4;
            falling_rock->pos.f[2] = falling_rock_vars->home.f[2] - 20.0f;
            falling_rock_vars->music = -1;
        }
        break;
    case FALLING_ROCK_STATE_WAIT_FOR_TRIGGER:
        if (falling_rock_vars->cam != -1) {
            camera_record = (LevelCuboid *)(falling_rock_vars->cam * 128 + (s32)l01_cuboids);
            if (hero.motion.pos.f[2] >= camera_record->pos.z &&
                vector_distance_xy(&hero.motion.pos, &camera_record->pos) < 10.0f &&
                ((u32)hero.state.control_mode < 2 || hero.state.control_mode == 9)) {
                falling_rock_vars->wait =
                    random_int_between(scale_game_frames(300), scale_game_frames(600));
                falling_rock_vars->rise = scale_game_frames(falling_rock_vars->rise_len);
                falling_rock->state = 2;
                l01_camera.shake_amount = 0.4f;
                l01_camera.shake_frames = scale_game_frames(30);
                allocate_voice_for_target_entry(1, 0, falling_rock);
            }
        }
        break;
    case FALLING_ROCK_STATE_WAIT_BEFORE_FALL:
        apply_falling_rock_camera_overrides((Obj *)falling_rock);
        if (fast_dec_timer_s16(&falling_rock_vars->rise)) {
            clear_u64_value(&falling_rock_vars->rot);
            falling_rock_vars->hold = scale_game_frames(0x2D);
            falling_rock->state = 3;
        }
        break;
    case FALLING_ROCK_STATE_FALLING: {
        CapVec3 falling_rock_position_before_step;

        apply_falling_rock_camera_overrides((Obj *)falling_rock);
        falling_rock->pos.f[2] -= 0.0f;
        qcopy(&falling_rock_position_before_step, &falling_rock->pos);
        falling_rock_vars->rot.f[2] -= frame_time_sq * 30.0f;
        fast_vec_add(&falling_rock->pos, &falling_rock->pos, &falling_rock_vars->rot);
        if (fast_dec_timer_s16(&falling_rock_vars->hold) &&
            20.0f < falling_rock_vars->home.f[2] - falling_rock->pos.f[2]) {
            l01_camera.shake_amount = 0.1f;
            l01_camera.shake_frames = scale_game_frames(20);
            clear_u64_value(&falling_rock_vars->rot);
            falling_rock->state = 4;
            allocate_voice_for_target_entry(0, 0, falling_rock);
            falling_rock->pos.f[2] = falling_rock_vars->home.f[2] - 20.0f;
            D_0014C190[current_level_index_macro][falling_rock->id >> 5] |=
                1 << (falling_rock->id & 0x1F);
            moby_killed[falling_rock->id >> 5] |= 1 << (falling_rock->id & 0x1F);
        }
        falling_rock->pos.f[2] += 0.0f;
        break;
    }
    case FALLING_ROCK_STATE_FALLEN:
        if (falling_rock_vars->wait == 0 && falling_rock_vars->music != -1) {
            jet_end_mount(0);
            l01_hero_set_state(HERO_STATE_IDLE, 1);
            cutscene_active = 0;
            falling_rock_vars->music = -1;
        }
        break;
    }
}

f32 falling_rock_camera_override_companion_value_gp __asm__("D_L01_00161C38") = 0.02f;
