/* Shared moby classes: vendor_orb, hydrodisplacer, pilots_helmet, pterodactyl, red_plant, robot_arm. */
#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/gameplay/camera/camera_modes.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/quad.h"

#include "rnc/overlay/shared/vendor/vendor_orb_globals.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/gameplay/entities/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/interpolation.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/gameplay/animation/blend_moby_animation.h"
#include "rnc/gameplay/entities/attach_manipulator.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/gameplay/entities/random_spherical_offset.h"
#include "rnc/overlay/shared/vehicles/veldin_ship.h"
extern float vendor_orb_rotation_rate_degrees_gp __asm__("D_L00_00161BB0") __attribute__((sda));
float fast_add_rotations(float, float) __asm__("FUN_001fa580");
void FUN_L00_001fff28(char *, int, float);
void vendor_orb_update(unsigned char *p) __asm__("FUN_L00_002df730");
void vendor_orb_update(unsigned char *moby) {
    enum {
        VENDOR_ORB_INITIALIZE = 0,
        VENDOR_ORB_ROTATE = 1
    };
    char *manipulator = ((char *)MOBY(moby)->pvars);
    float rotation_step_radians;
    switch (moby[0x20]) {
    case VENDOR_ORB_INITIALIZE:
        moby[0x20] = 1;
        moby[0x73] = 0x20;
        *(float *)(manipulator + 0x40) = random_angle_radians();
        attach_manipulator(moby, 1, manipulator);
        break;
    case VENDOR_ORB_ROTATE:
        rotation_step_radians = vendor_orb_rotation_rate_degrees_gp * DEG_TO_RAD * frame_time;
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, rotation_step_radians);
        *(float *)(manipulator + 0x40) = fast_add_rotations(*(float *)(manipulator + 0x40), rotation_step_radians * -2.0f);
        FUN_L00_001fff28(manipulator + 0x10, 2, *(float *)(manipulator + 0x40));
        break;
    }
}
#include "qcopy.h"
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2df808;
typedef unsigned int U128_2df808 __attribute__((mode(TI), aligned(16)));
typedef union {
    U128_2df808 q;
    float f[4];
} R_2df808;
extern unsigned char *FUN_L00_0024e838_2df808(int) __asm__("FUN_L00_0024e838");
extern float FUN_L00_00257be8_2df808(float, float) __asm__("FUN_L00_00257be8");
unsigned char *spawn_randomly_rotated_colored_moby(int owner, V_2df808 *position, V_2df808 *vector,
                                 int flags, unsigned char red, unsigned char green,
                                 unsigned char blue, unsigned char alpha,
                                 float scale) __asm__("FUN_L00_002df808");

unsigned char *spawn_randomly_rotated_colored_moby(int owner, V_2df808 *position, V_2df808 *vector,
                                 int flags, unsigned char red, unsigned char green,
                                 unsigned char blue, unsigned char alpha,
                                 float scale) {
    unsigned char *moby = FUN_L00_0024e838_2df808(0x4a8);
    unsigned char *data;
    R_2df808 rotation_copy;
    R_2df808 rotation;
    if (moby != 0) {
        data = ((unsigned char *)MOBY(moby)->pvars);
        rotation.q = 0;
        rotation.f[0] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation.f[1] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation.f[2] = FUN_L00_00257be8_2df808(-3.1415927f, 3.1415927f);
        rotation_copy.q = rotation.q;
        moby[0x20] = 0;
        moby[0x23] = alpha;
        moby[0x30] = 0xff;
        MOBY(moby)->draw_dist = 0xff;
        moby[0x31] = 1;
        set_moby_color(moby, red, green, blue);
        *(float *)(data + 0x18) = MOBY(moby)->scale * scale;
        *(int *)(moby + 0x2c) = 0;
        *(int *)(data + 0x10) = owner;
        qcopy(moby + 0x10, position);
        qcopy(data, vector);
        qcopy(moby + 0x40, &rotation_copy);
        *(unsigned short *)(data + 0x1e) = alpha;
        *(int *)(data + 0x14) = flags;
        *(unsigned short *)(data + 0x1c) = *(unsigned short *)(data + 0x14);
        update_entity(moby);
    }
    return moby;
}
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E0E50), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct PadState PadState;

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec_2E0E50;

typedef struct {
    int i0;
    int i4;
    int i8;
    int iC;
    float f10;
    float f14;
} Data;

typedef struct {
    char p0[0x30];
    Vec_2E0E50 v30;
    char p40[0x30];
    Vec_2E0E50 v70;
} Node;

extern char D_0013F350[] NOT_SDA;
extern char D_L00_00166DC0[] NOT_SDA;
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern int FUN_001f96b0_c() __asm__("FUN_001f96b0");
extern int FUN_001f9a40_c() __asm__("FUN_001f9a40");
extern int hero_set_state() __asm__("FUN_L00_002223f8");
extern int FUN_L00_00266d60();
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void FUN_L00_002323b8(int,int);
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
s32 FUN_L00_002e02e0_c(void) __asm__("FUN_L00_002e02e0");
s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
void emit_hydrodisplacer_effect(char *m) __asm__("FUN_L00_002e01b0");
void end_hydrodisplacer_interaction(char *a) __asm__("FUN_L00_002e0340");
extern char D_0013E633_2E0E50[] __asm__("D_0013F350");
extern char D_0013A5E0_2E0E50[] __asm__("D_0013CAE0");
extern int D_L00_0015F5C4_macro __asm__("D_L00_0015F5C4") MACRO_ADDR;
extern int D_L00_0015F404_macro __asm__("D_L00_0015F404") MACRO_ADDR;
extern char *moby_table_macro __asm__("D_L00_0015FFD8") MACRO_ADDR;
extern char *D_L00_001600EC_macro __asm__("D_L00_001600EC") MACRO_ADDR;
extern float fade_level __asm__("D_L00_0015F3FC") MACRO_ADDR;
extern float D_L00_00161BB4_macro __asm__("D_L00_00161BB4") MACRO_ADDR;
extern float frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern float frame_scale_sq_macro __asm__("D_0015ED64") MACRO_ADDR;
extern float frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern char D_L00_00166EC0_2E0E50[] __asm__("D_L00_00166DC0");
extern int func_001F9850_2E0E50(int) __asm__("FUN_001f96f8");
extern int pad_pressed_within(int, int, int) __asm__("FUN_L00_00266d60");
extern int func_L00_002E1790_2E0E50(Moby *) __asm__("FUN_L00_002e02e0");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int func_001FA898_2E0E50(float) __asm__("FUN_001fa6d0");
extern float func_00214D28_2E0E50(float *, float, float) __asm__("FUN_00213ed8");
extern float advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void func_001F9C08_2E0E50(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern float func_001FA790_2E0E50(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA748_2E0E50(float, float) __asm__("FUN_001fa580");
extern int func_001F9908_2E0E50(void *) __asm__("FUN_001f9740");

void hydrodisplacer_update(Moby *moby) __asm__("FUN_L00_002df9a0");
void hydrodisplacer_update(Moby *moby) {
    enum {
        HYDRODISPLACER_INITIALIZE = 0,
        HYDRODISPLACER_WAIT_FOR_ANIMATION_FLAG = 1,
        HYDRODISPLACER_WAIT_FOR_ACTIVATION = 2,
        HYDRODISPLACER_WAIT_FOR_HERO_FLAG = 3,
        HYDRODISPLACER_CAMERA_TRANSITION = 4,
        HYDRODISPLACER_WAIT_FOR_TIMER = 5
    };
    enum {
        HYDRODISPLACER_CAMERA_PREPARE_INTERPOLATION = 0,
        HYDRODISPLACER_CAMERA_INTERPOLATE = 1,
        HYDRODISPLACER_CAMERA_RETURN_TO_ACTIVATION = 2
    };
    Vec_2E0E50 camera_pos, camera_rot;
    Data *data = (void *)moby->pvars;

    if (((Hero *)D_0013E633_2E0E50)->items[0].state == 3) {
        end_hydrodisplacer_interaction(moby);
        return;
    }
    if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full != 0) {
        if (D_L00_0015F5C4_macro == 0 && !((unsigned int)(((Hero *)D_0013E633_2E0E50)->state.current - 0x38) < 2)) {
            emit_hydrodisplacer_effect(moby);
        }
        if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full != 0) {
            float accel = frame_scale_sq_macro * 0.02f;
            float max_speed = frame_scale_sq_macro * 0.3f;

            ((Hero *)D_0013E633_2E0E50)->item_eases[0].attach_joint = 1;
            ((Hero *)D_0013E633_2E0E50)->item_eases[0].scale = 2.3f;
            ((Hero *)D_0013E633_2E0E50)->item_eases[0].accel = accel;
            ((Hero *)D_0013E633_2E0E50)->item_eases[0].max_speed = max_speed;
            if (!((unsigned int)(((Hero *)D_0013E633_2E0E50)->state.current - 0x38) < 2)) {
                float offset_y = 1024.0f / moby->scale * -0.2f;

                ((Hero *)D_0013E633_2E0E50)->item_eases[1].accel = accel;
                ((Hero *)D_0013E633_2E0E50)->item_eases[1].max_speed = max_speed;
                ((Hero *)D_0013E633_2E0E50)->item_eases[2].max_speed = max_speed;
                ((Hero *)D_0013E633_2E0E50)->item_eases[1].attach_joint = 2;
                ((Hero *)D_0013E633_2E0E50)->item_eases[2].attach_joint = 3;
                ((Hero *)D_0013E633_2E0E50)->item_eases[2].accel = accel;
                ((Hero *)D_0013E633_2E0E50)->item_eases[1].pos_target.f[1] = offset_y;
                ((Hero *)D_0013E633_2E0E50)->item_eases[2].pos_target.f[1] = offset_y;
            }
        }
    }
    switch (moby->state) {
    case HYDRODISPLACER_INITIALIZE:
        allocate_voice_for_target_entry(0, 0, moby);
        moby->state = 1;
        break;
    case HYDRODISPLACER_WAIT_FOR_ANIMATION_FLAG:
        if (moby->anim_flags & 2) {
            moby->state = 2;
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_ACTIVATION:
        if (pad_pressed_within(((Hero *)D_0013E633_2E0E50)->items[0].button_mask, func_001F9850_2E0E50(8), 0) == 0) {
            break;
        }
        if ((*(int *)D_0013A5E0_2E0E50 & ((Hero *)D_0013E633_2E0E50)->items[0].button_mask) == 0) {
            break;
        }
        if (((Hero *)D_0013E633_2E0E50)->ground_moby == 0 || ((Hero *)D_0013E633_2E0E50)->air_frames.s != 0
            || ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass != 0x155) {
            break;
        }
        if (func_L00_002E1790_2E0E50(moby) == 0 || (unsigned int)((Hero *)D_0013E633_2E0E50)->state.control_mode >= 2) {
            break;
        }
        moby->state = 3;
        hero_set_state(0x38, 0);
        if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full != 0) {
            set_hero_animation(0x83, 0, func_001F9850_2E0E50(6));
        } else {
            set_hero_animation(0x46, 0, func_001F9850_2E0E50(6));
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_HERO_FLAG: {
        int *platform;
        char *linked;

        if (((Hero *)D_0013E633_2E0E50)->ground_moby == 0 || ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass != 0x155) {
            end_hydrodisplacer_interaction(moby);
            return;
        }
        if (!(((Hero *)D_0013E633_2E0E50)->anim_flags & 2)) {
            break;
        }
        platform = (void *)((Hero *)D_0013E633_2E0E50)->ground_moby->pvars;
        linked = moby_table_macro + (platform[0] << 8);
        hero_set_state(0x39, 0);
        if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full != 0) {
            set_hero_animation(0x84, 0, func_001F9850_2E0E50(6));
        } else {
            set_hero_animation(0x47, 0, func_001F9850_2E0E50(6));
        }
        if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full == 0) {
            allocate_voice_for_target_entry(0x14, 0, ((Hero *)D_0013E633_2E0E50)->moby);
        } else {
            allocate_voice_for_target_entry(0x15, 0, ((Hero *)D_0013E633_2E0E50)->moby);
        }
        if (platform[1] != -1 && platform[2] != -1) {
            moby->command = 0;
            moby->state = 4;
            data->f10 = 0.0f;
            jet_mount(D_L00_00166EC0_2E0E50, D_L00_00166EC0_2E0E50 + 0x10, 1, 0, 0);
            if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full != 0) {
                data->i8 = platform[1];
                data->iC = platform[2];
            } else {
                data->iC = platform[1];
                data->i8 = platform[2];
            }
            break;
        }
        moby->state = 5;
        data->i0 = func_001FA898_2E0E50(scale_time(D_L00_00161BB4_macro));
        if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full == 0) {
            ((Moby *)linked)->command = 8;
            ((Hero *)D_0013E633_2E0E50)->hydro_tank_full = 1;
        } else {
            ((Moby *)linked)->command = 4;
            ((Hero *)D_0013E633_2E0E50)->hydro_tank_full = 0;
        }
        break;
    }
    case HYDRODISPLACER_CAMERA_TRANSITION:
        if (((Hero *)D_0013E633_2E0E50)->ground_moby == 0 || ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass != 0x155) {
            end_hydrodisplacer_interaction(moby);
            return;
        }
        switch (moby->command) {
        case HYDRODISPLACER_CAMERA_PREPARE_INTERPOLATION: {
            char *linked;

            func_00214D28_2E0E50(&fade_level, 1.0f, 1.0f / func_001F9850_2E0E50(15));
            if (!(1.0f <= fade_level)) {
                break;
            }
            linked = moby_table_macro + (*(int *)((Hero *)D_0013E633_2E0E50)->ground_moby->pvars << 8);
            moby->command = 1;
            set_camera_position(D_L00_001600EC_macro + (data->i8 << 7) + 0x30);
            set_camera_rotation(D_L00_001600EC_macro + (data->i8 << 7) + 0x70);
            D_L00_0015F404_macro = 1;
            if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full == 0) {
                ((Moby *)linked)->command = 8;
                ((Hero *)D_0013E633_2E0E50)->hydro_tank_full = 1;
            } else {
                ((Moby *)linked)->command = 4;
                ((Hero *)D_0013E633_2E0E50)->hydro_tank_full = 0;
            }
            break;
        }
        case HYDRODISPLACER_CAMERA_INTERPOLATE:
            if (data->f10 == 0.0f && 0.0f < fade_level) {
                func_00214D28_2E0E50(&fade_level, 0.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            if (data->f10 < 1.0f) {
                advance_accelerated_scalar(&data->f10, &data->f14, 1.0f, frame_time_sq_macro * 0.666f, frame_time_sq_macro * 0.666f, frame_time_macro * 0.5f);
                func_001F9C08_2E0E50(&camera_pos, D_L00_001600EC_macro + (data->i8 << 7) + 0x30, D_L00_001600EC_macro + (data->iC << 7) + 0x30, data->f10);
                clear_u64_value(&camera_rot);
                camera_rot.y = func_001FA790_2E0E50(((Node *)D_L00_001600EC_macro)[data->iC].v70.y, ((Node *)D_L00_001600EC_macro)[data->i8].v70.y) * data->f10;
                camera_rot.y = func_001FA748_2E0E50(((Node *)D_L00_001600EC_macro)[data->i8].v70.y, camera_rot.y);
                camera_rot.z = func_001FA790_2E0E50(((Node *)D_L00_001600EC_macro)[data->iC].v70.z, ((Node *)D_L00_001600EC_macro)[data->i8].v70.z) * data->f10;
                camera_rot.z = func_001FA748_2E0E50(((Node *)D_L00_001600EC_macro)[data->i8].v70.z, camera_rot.z);
                set_camera_position(&camera_pos);
                set_camera_rotation(&camera_rot);
                return;
            }
            if (fade_level < 1.0f) {
                func_00214D28_2E0E50(&fade_level, 1.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            moby->command = 2;
            D_L00_0015F404_macro = 0;
            jet_end_mount(3);
            return;
        case HYDRODISPLACER_CAMERA_RETURN_TO_ACTIVATION:
            if (0.0f < fade_level) {
                func_00214D28_2E0E50(&fade_level, 0.0f, 1.0f / func_001F9850_2E0E50(15));
                return;
            }
            moby->state = 2;
            hero_set_state(0x3A, 0);
            if (((Hero *)D_0013E633_2E0E50)->hydro_tank_full != 0) {
                set_hero_animation(0x48, 0, func_001F9850_2E0E50(6));
            } else {
                set_hero_animation(0x85, 0, func_001F9850_2E0E50(6));
            }
            return;
        }
        break;
    case HYDRODISPLACER_WAIT_FOR_TIMER:
        if (((Hero *)D_0013E633_2E0E50)->ground_moby != 0 && ((Hero *)D_0013E633_2E0E50)->ground_moby->oclass == 0x155
            && func_001F9908_2E0E50(data) == 0) {
            break;
        }
        end_hydrodisplacer_interaction(moby);
        moby->state = 2;
        break;
    }
}
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2e01b0;
extern float D_0015ED6C_2e01b0 __asm__("D_0015ED6C");
int FUN_001f9740_2e01b0(void *) __asm__("FUN_001f9740");
void fast_vec_normalize(V_2e01b0 *, void *, float) __asm__("FUN_001f9bf8");
void spawn_debris_chunk(V_2e01b0 *, V_2e01b0 *, int, int) __asm__("FUN_L00_0026f548");
int FUN_001f96f8_2e01b0(int) __asm__("FUN_001f96f8");

void emit_hydrodisplacer_effect(char *moby) {
    V_2e01b0 effect_position, effect_motion_vector;
    char *moby_data = ((char *)MOBY(moby)->pvars);
    struct Hero *player;
    int countdown_lower_bound;
    float normalization_scale;
    if (FUN_001f9740_2e01b0(moby_data + 4) == 0)
        return;
    moby_point_position(moby, 0, &effect_position);
    normalization_scale = -random_float_between(D_0015ED6C_2e01b0, D_0015ED6C_2e01b0 * 3.0f);
    fast_vec_normalize(&effect_motion_vector, moby + 0xD0, normalization_scale);
    player = &hero;
    effect_motion_vector.x += player->motion.displacement.f[0] * 0.8f;
    effect_motion_vector.y += player->motion.displacement.f[1] * 0.8f;
    effect_motion_vector.z += player->motion.displacement.f[2] +
           random_float_between(D_0015ED6C_2e01b0 * -0.5f, D_0015ED6C_2e01b0 + D_0015ED6C_2e01b0);
    spawn_debris_chunk(&effect_position, &effect_motion_vector, 2, random_int_between(0x5A, 0x78));
    countdown_lower_bound = FUN_001f96f8_2e01b0(5);
    *(int *)(moby_data + 4) = random_int_between(countdown_lower_bound, FUN_001f96f8_2e01b0(15));
}
extern u8 *moby_table __asm__("D_L00_0015FFD8");

/* Tests flag 2 (or 1 when &hero.hydro_tank_full is set) at 0xBC of the moby selected by +0x2FC. */
s32 test_hydrodisplacer_activation_flag(void) __asm__("FUN_L00_002e02e0");

s32 test_hydrodisplacer_activation_flag(void) {
    struct Hero *base;
    u8 *linked_moby;
    s32 ground_moby_address;
    s32 idx;
    u8 flags;
    base = &hero;
    linked_moby = moby_table;
    ground_moby_address = ((s32)base->ground_moby);
    flags = (u8)base->hydro_tank_full;
    linked_moby += *(s32 *)*(s32 *)((u8 *)ground_moby_address + 0x78) << 8;
    if (!flags) {
        if (linked_moby[0xBC] & 2)
            return 1;
    } else {
        if (linked_moby[0xBC] & 1)
            return 1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E17F0), where it is exact; names translated to the US level program. */

extern int D_L00_0015F404;
extern short D_L00_0015F3FC;

/* Moby state 4: clears two globals, calls 002eac18(3); control mode 0x13: calls 002223f8(0, 1). */

void end_hydrodisplacer_interaction(char *moby) {
    if (MOBY(moby)->state == 4) {
        D_L00_0015F404 = 0;
        *(int *)&D_L00_0015F3FC = 0;
        jet_end_mount(3);
    }
    if (hero.state.control_mode == 0x13) {
        hero_set_state(HERO_STATE_IDLE, 1);
    }
}
typedef struct {
    u8 pad[0xB0];
    u8 b0;
    u8 pad1;
    s16 h;
    u8 pad2[0x4C];
} E_2e03a0_2e03a0;
typedef struct {
    s32 fa_2e03a0;
    s32 b;
} P_2e03a0_2e03a0;
typedef struct {
    u8 pad[0x18];
    f32 z;
    u8 pad1[4];
    u8 state;
    u8 pad2[0x13];
    u16 fl;
    u8 pad3[0x12];
    f32 ang;
    u8 pad4[0x2C];
    P_2e03a0_2e03a0 *p;
    u8 pad5[0x34];
    u8 b0;
    u8 pad6;
    s16 h;
} M_2e03a0_2e03a0;
extern f32 D_0015ED6C_2e03a0 __asm__("D_0015ED6C");
extern s32 D_L00_0015F5C4_2e03a0 __asm__("D_L00_0015F5C4");
extern s32 D_L00_0015F640 __asm__("D_L00_0015F640");
extern E_2e03a0_2e03a0 *D_L00_0015FFD8_2e03a0 __asm__("D_L00_0015FFD8");
typedef struct {
    u8 pad[0x454];
    u8 arr[1];
} T_2e03a0_2e03a0;
extern u8 mission_state_macro[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern u8 D_0014C050_2e03a0[] __asm__("D_0014C050");
extern s32 D_00141410_2e03a0[] __asm__("D_00141410");
extern u8 item_available[] __asm__("D_0013D4C0");
extern f32 FUN_001fa580_2e03a0(f32, f32) __asm__("FUN_001fa580");
extern void debug_print() __asm__("FUN_001e93b0");
extern void FUN_L00_002e0788_2e03a0(void *) __asm__("FUN_L00_002e0788");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 absolute_float(f32) __asm__("FUN_001f99c0");
extern void start_scene(s32) __asm__("FUN_L00_00298840");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");

extern u8 pilots_helmet_missing_cutscene_format[] __asm__("D_L00_001EA1B0");
extern u8 pilots_helmet_already_owned_message[] __asm__("D_L00_001EA1D8");
extern u8 pilots_helmet_missing_segment_switch_format[] __asm__("D_L00_001EA200");
extern u8 pilots_helmet_acquired_message[] __asm__("D_L00_00161BC0");
extern u16 pilots_helmet_help_record[] __asm__("D_00141B08");
extern T_2e03a0_2e03a0 D_L00_001BA5D0;
extern T_2e03a0_2e03a0 D_L00_001BB230;
extern s32 current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));
void pilots_helmet_update(M_2e03a0_2e03a0 *m) __asm__("FUN_L00_002e03a0");
void pilots_helmet_update(M_2e03a0_2e03a0 *moby) {
    enum {
        PILOTS_HELMET_INITIALIZE = 0,
        PILOTS_HELMET_WAIT_FOR_HERO_PROXIMITY = 1,
        PILOTS_HELMET_GRANT_REWARD = 2,
        PILOTS_HELMET_SAVE_AND_REMOVE = 3
    };
    P_2e03a0_2e03a0 *p = moby->p;
    char *hero_data;
    E_2e03a0_2e03a0 *e;
    u32 k;
    s32 n;

    moby->ang = FUN_001fa580_2e03a0(moby->ang, D_0015ED6C_2e03a0 * 1.5707964f);
    switch (moby->state) {
    case PILOTS_HELMET_INITIALIZE:
        initialize_moby_particle_effect(moby);
        if (p->fa_2e03a0 == -1)
            debug_print(pilots_helmet_missing_cutscene_format, moby->h);
        if (item_available[7]) {
            debug_print(pilots_helmet_already_owned_message);
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->z += 1.0f;
        break;
    case PILOTS_HELMET_WAIT_FOR_HERO_PROXIMITY:
        FUN_L00_002e0788_2e03a0(moby);
        hero_data = (char *)&hero.motion.pos;
        if (vector_distance((char *)moby + 0x10, hero_data) < 3.0f) {
            hero_data -= 0x80;
            if (absolute_float(moby->z - *(f32 *)(hero_data + 0x88)) < 2.0f && *(s32 *)(hero_data + 0x22A8)) {
                moby->fl |= 0x41;
                if (p->fa_2e03a0 != -1)
                    start_scene(p->fa_2e03a0);
                else
                    debug_print(pilots_helmet_missing_cutscene_format, moby->h);
                moby->state = 2;
            }
        }
        break;
    case PILOTS_HELMET_GRANT_REWARD:
        if (D_L00_0015F5C4_2e03a0 == 2)
            return;
        debug_print(pilots_helmet_acquired_message);
        queue_dialogue_message(0x232A, -1);
        D_L00_0015F640 = 0xB4;
        grant_inventory_item(7, 1);
        if (pilots_helmet_help_record[0] == 0)
            help_request(0x2328, 0x34);
        if (p->b != -1) {
            D_L00_001BA5D0.arr[D_L00_0015FFD8_2e03a0[p->b].h] =
                D_L00_0015FFD8_2e03a0[p->b].b0 + 2;
            if (D_L00_0015FFD8_2e03a0[p->b].b0 == 0xFF ||
                (mission_state_macro[D_L00_0015FFD8_2e03a0[p->b].b0] != 0xFF &&
                 D_0014C050_2e03a0[D_L00_0015FFD8_2e03a0[p->b].b0 + current_level_index_gp * 16] ==
                     0xFF)) {
                D_L00_001BB230.arr[D_L00_0015FFD8_2e03a0[p->b].h] =
                    D_L00_0015FFD8_2e03a0[p->b].b0 + 2;
            }
        } else {
            debug_print(pilots_helmet_missing_segment_switch_format, moby->h);
        }
        if (moby->b0 != 0xFF)
            jet_clear_persistent(moby->b0);
        D_00141410_2e03a0[0] = 7;
        moby->state = 3;
        break;
    case PILOTS_HELMET_SAVE_AND_REMOVE:
        memcard_save_data(0, -1);
        mark_moby_for_removal(moby);
        break;
    }
}
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vscl_2e0788(void *, void *, float) __asm__("FUN_001f9bf8");
extern int f9740_2e0788(void *) __asm__("FUN_001f9740");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern unsigned char *spr_2e0788(OvlQuad *, int, unsigned char, int, int, int, int,
                                 float) __asm__("FUN_L00_00272f68");
typedef struct {
    char pad[0x10];
    float a[4];
    float b[4];
    int t[4];
    float c[4];
} M_2e0788;
void emit_moby_color_cycling_particles(unsigned char *moby) __asm__("FUN_L00_002e0788");

void emit_moby_color_cycling_particles(unsigned char *moby) {
    OvlQuad sprite_position_and_step[2];
    M_2e0788 *moby_data = ((M_2e0788 *)MOBY(moby)->pvars);
    float *a;
    int *t;
    int i, j, c;
    float pre_wrap_sum, color_cycle_progress;
    j = 0;
    moby += 0x10;
    fast_vec_sub(sprite_position_and_step, D_L00_00166DC0, moby);
    t = moby_data->t;
    a = moby_data->a;
    vscl_2e0788(sprite_position_and_step, sprite_position_and_step, -0.3f);
    vscl_2e0788(&sprite_position_and_step[1], sprite_position_and_step, 0.1f);
    fast_vec_add(sprite_position_and_step, sprite_position_and_step, moby);
    for (i = 3; i >= 0; i--) {
        pre_wrap_sum = *a + moby_data->b[j];
        *a = pre_wrap_sum;
        if (pre_wrap_sum >= 255.0f) {
            *a = pre_wrap_sum - 255.0f;
        } else if (pre_wrap_sum <= 0.0f) {
            *a = pre_wrap_sum + 255.0f;
        }
        if (f9740_2e0788(t)) {
            moby_data->t[j] = scale_game_frames(0xff);
        }
        t++;
        color_cycle_progress = convert_integer_to_float(scale_game_frames(0xff) - moby_data->t[j]);
        color_cycle_progress = color_cycle_progress / (float)scale_game_frames(0xff);
        c = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - color_cycle_progress));
        spr_2e0788(sprite_position_and_step, c, truncate_float_to_s32(*a++), 0x35, 1, 2, 0, moby_data->c[j]);
        j++;
        fast_vec_add(sprite_position_and_step, sprite_position_and_step, &sprite_position_and_step[1]);
    }
}
typedef int s32_2e1f28;
extern int beam_slot_state[] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern float beam_amplitude_phase[] __asm__("D_L00_00161C08") MACRO_ADDR;
extern float beam_twist_phase[] __asm__("D_L00_00161C18") MACRO_ADDR;
extern float beam_swell_phase[] __asm__("D_L00_00161C28") MACRO_ADDR;
extern int beam_endpoint_light[] __asm__("D_L00_00161C38") MACRO_ADDR;
extern int beam_tip_light[] __asm__("D_L00_00161C48") MACRO_ADDR;
extern int beam_endpoint_effect_flags[] __asm__("D_L00_00161C58") MACRO_ADDR;
extern int beam_endpoint_spark_tag_mode[] __asm__("D_L00_00161C68") MACRO_ADDR;
extern float beam_scroll_macro[] __asm__("D_L00_00161C78") MACRO_ADDR;
extern int beam_slot_id[] __asm__("D_L00_00161C88") MACRO_ADDR;
s32_2e1f28 register_beam_slot(s32_2e1f28 id, s32_2e1f28 endpoint_effect_flags, s32_2e1f28 endpoint_spark_tag_mode) __asm__("FUN_L00_002e1f28");

s32_2e1f28 register_beam_slot(s32_2e1f28 id, s32_2e1f28 a, s32_2e1f28 b) __asm__("FUN_L00_002e1f28");
s32_2e1f28 register_beam_slot(s32_2e1f28 id, s32_2e1f28 endpoint_effect_flags, s32_2e1f28 endpoint_spark_tag_mode) {
    s32_2e1f28 i, j;
    for (i = 0; i < 3; i++) {
        if (beam_slot_id[i] == id)
            return 1;
    }
    for (j = 0; j < 3; j++) {
        if (beam_slot_id[j] == 0) {
            beam_slot_id[j] = id;
            beam_amplitude_phase[j] = 0;
            beam_twist_phase[j] = 0;
            beam_swell_phase[j] = 0;
            beam_endpoint_light[j] = -1;
            beam_tip_light[j] = -1;
            beam_endpoint_effect_flags[j] = endpoint_effect_flags;
            beam_endpoint_spark_tag_mode[j] = endpoint_spark_tag_mode;
            beam_scroll_macro[j] = 0;
            beam_slot_state[j] = 1;
            return 1;
        }
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E34F0), where it is exact; names translated to the US level program. */

extern int beam_slot_state[] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern int beam_endpoint_light[] __asm__("D_L00_00161C38") MACRO_ADDR;
extern int beam_tip_light[] __asm__("D_L00_00161C48") MACRO_ADDR;
extern int beam_slot_id[] __asm__("D_L00_00161C88") MACRO_ADDR;

int release_beam_slot_by_id(int id) __asm__("FUN_L00_002e2040");
int release_beam_slot_by_id(int id) {
    unsigned int i;
    for (i = 0; (int)i < 3; i++) {
        if (beam_slot_id[i] == id) {
            beam_slot_state[i] = 0;
            beam_slot_id[i] = 0;
            if (beam_endpoint_light[i] != -1) {
                release_point_light_slot(beam_endpoint_light[i]);
                beam_endpoint_light[i] = -1;
            }
            if (beam_tip_light[i] != -1) {
                release_point_light_slot(beam_tip_light[i]);
                beam_tip_light[i] = -1;
            }
            return 1;
        }
    }
    return 0;
}
#include "sda.h"

extern int beam_slot_state[] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern int beam_endpoint_light[] __asm__("D_L00_00161C38") MACRO_ADDR;
extern int beam_tip_light[] __asm__("D_L00_00161C48") MACRO_ADDR;
extern int beam_slot_id[] __asm__("D_L00_00161C88") MACRO_ADDR;

void release_beam_slot(s32 i) __asm__("FUN_L00_002e20f8");

void release_beam_slot(s32 i) {
    s32 first_endpoint_light_index;
    first_endpoint_light_index = beam_endpoint_light[i];
    beam_slot_state[i] = 0;
    beam_slot_id[i] = 0;
    if (first_endpoint_light_index != -1) {
        release_point_light_slot(first_endpoint_light_index);
        beam_endpoint_light[i] = -1;
    }
    if (beam_tip_light[i] != -1) {
        release_point_light_slot(beam_tip_light[i]);
        beam_tip_light[i] = -1;
    }
}
#include "qcopy.h"
#include "sda.h"
typedef struct {
    f32 basis[3][4];
    f32 origin[4];
} BeamTransform;
typedef struct {
    u8 b[0x10];
} Q002e2190;
extern BeamTransform beam_transforms[] __asm__("D_L00_001E6D20");
extern Q002e2190 D_L00_001E6DE0[];
void fast_mem_copy(void *, void *, s32) __asm__("FUN_001f98d0");
s32 set_beam_origin_transform_and_endpoint(s32 id, void *beam_origin_transform, void *endpoint_position) __asm__("FUN_L00_002e2190");

s32 set_beam_origin_transform_and_endpoint(s32 id, void *src, void *v) __asm__("FUN_L00_002e2190");
s32 set_beam_origin_transform_and_endpoint(s32 id, void *beam_origin_transform, void *endpoint_position) {
    s32 i;
    for (i = 0; i < 3; i++) {
        if (id == beam_slot_id[i]) {
            if (beam_slot_state[i] == 1)
                beam_slot_state[i] = 2;
            else if (beam_slot_state[i] == 4)
                beam_slot_state[i] = 3;
            fast_mem_copy(&beam_transforms[i], beam_origin_transform, 0x40);
            qcopy(&D_L00_001E6DE0[i], endpoint_position);
            return 1;
        }
    }
    return 0;
}
#include "rnc/math/vector.h"
#include "rnc/gameplay/entities/moby.h"

/* Per-frame update of one beam slot: lays out the two 20-point strips between the slot's ends
   (a sine wave and a noisy one), spawns sparks at both ends and keeps the two looping sounds
   at the ends. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E3700), where it is exact; names translated to the US level program. */

typedef struct { unsigned char b[0x40]; } B002E3640;

typedef struct { unsigned char b[0x10]; } Q002E3640;

typedef float Vec_2E3700[4] __attribute__((aligned(16)));

typedef struct { float x, y, z, w; } Pt;

typedef struct { float v[4]; } Q_2E3700;

typedef struct {
    char pad0[0x10];
    float f10;
    short f14;
    short f16;
    int f18;
} PartSub;

typedef struct {
    char pad0[0xA];
    short fA;
    float fC;
    char pad10[0x10];
    PartSub sub;
} Part;

/* The origin row of a BeamTransform, read through its own label. */
typedef struct {
    f32 origin[4];
    u8 pad10[0x30];
} BeamOrigin;

typedef struct { Pt points[20]; } BeamStrand;

extern BeamStrand beam_strands[] __asm__("D_L00_001E6E40");
extern Vec4 zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations_c(f32, f32) __asm__("func_001FA5C8");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
f32 FUN_L00_00257c48(f32, f32);
f32 normalize_angle(f32 a) __asm__("FUN_L00_0025e310");
int allocate_colored_effect_slot(void *v, float a, float b_0023e738, float c, float d, float e) __asm__("FUN_L00_0023e738");
unsigned char *spawn_mode_controlled_rotating_alpha_fade_particle(void *a, void *b, int c, int d) __asm__("FUN_L00_00274948");
extern BeamTransform beam_transforms[] __asm__("D_L00_001E6D20");
extern BeamOrigin beam_origins[] __asm__("D_L00_001E6D50");
extern Q_2E3700 D_L00_001E7130_2E3700[] __asm__("D_L00_001E6DE0");
extern Q_2E3700 D_L00_001E7160_2E3700[] __asm__("D_L00_001E6E10");
extern BeamStrand beam_strands[] __asm__("D_L00_001E6E40");
extern char D_0013E633_2E3700[] __asm__("D_0013F5E0");
extern Vec4 zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;
extern int beam_slot_state[] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern float beam_amplitude_phase[] __asm__("D_L00_00161C08") MACRO_ADDR;
extern float beam_twist_phase[] __asm__("D_L00_00161C18") MACRO_ADDR;
extern float beam_swell_phase[] __asm__("D_L00_00161C28") MACRO_ADDR;
extern int beam_endpoint_light[] __asm__("D_L00_00161C38") MACRO_ADDR;
extern int beam_tip_light[] __asm__("D_L00_00161C48") MACRO_ADDR;
extern int beam_endpoint_effect_flags[] __asm__("D_L00_00161C58") MACRO_ADDR;
extern int beam_endpoint_spark_tag_mode[] __asm__("D_L00_00161C68") MACRO_ADDR;
extern int beam_slot_id[] __asm__("D_L00_00161C88") MACRO_ADDR;
extern float D_L00_00161BF0_gp __asm__("D_L00_00161BF0") __attribute__((sda));
extern void func_001F9BF0_2E3700(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float func_001FA748_2E3700(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_2E3700(float, float) __asm__("FUN_001fa5c8");
extern float func_001FA888_2E3700(int) __asm__("FUN_001fa6c0");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float func_001F9FA8_2E3700(float) __asm__("FUN_001f9de0");
extern float func_001F9F90_2E3700(float) __asm__("FUN_001f9dc8");
extern void func_L00_001FF4B0_2E3700(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9BD8_2E3700(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F9850_2E3700(int) __asm__("FUN_001f96f8");

void update_slot_panes_and_effects(int unused, int beam) __asm__("FUN_L00_002e2250");

void update_slot_panes_and_effects(int unused, int beam) {
    Vec_2E3700 span;
    Vec_2E3700 beam_direction;
    Vec_2E3700 side;
    Vec_2E3700 up;
    Vec_2E3700 from;
    Vec_2E3700 span2;
    Vec_2E3700 tip;
    Vec4 particle_velocity;
    Vec_2E3700 first_endpoint_effect_position;
    int strand = beam * 2;
    int next_strand = strand + 1;
    float step;
    float phase_step;
    float one;
    Pt *point;
    int i;
    int j;
    int second_strand_point_index;

    func_001F9BF0_2E3700(span, &D_L00_001E7130_2E3700[beam], &beam_origins[beam]);
    step = fast_vec_length(span) / 20.0f;
    if (beam_slot_state[beam] == 2) {
        beam_slot_state[beam] = 3;
        for (i = 0; i < 20; i++) {
            Pt *first = &beam_strands[strand].points[i];
            Pt *second = &beam_strands[next_strand].points[i];

            first->x = step * (float)i;
            first->y = first->z = 0.0f;
            first->w = 1.0f;
            second->x = step * (float)i;
            second->y = second->z = 0.0f;
            second->w = 1.0f;
        }
    }
    beam_amplitude_phase[beam] = func_001FA748_2E3700(beam_amplitude_phase[beam], 0.17453292f);
    beam_twist_phase[beam] = func_001FA790_2E3700(beam_twist_phase[beam], 0.80285144f);
    beam_swell_phase[beam] = func_001FA748_2E3700(beam_swell_phase[beam], 0.05236f);
    phase_step = 360.0f / (func_001FA888_2E3700(20) * 0.5f) * 0.017453292f;
    for (j = 0; j < 19; j++) {
        float fj = (float)j;
        float amplitude = func_001FA888_2E3700(j) / func_001FA888_2E3700(20) * 0.3f + 0.1f;
        Pt *point = &beam_strands[strand].points[j];

        amplitude = amplitude * func_001F9FA8_2E3700(normalize_angle(beam_amplitude_phase[beam] + fj * phase_step));
        point->y = 0.0f;
        point->w = 1.0f;
        point->z = amplitude;
        point->x = step * fj;
    }
    qcopy_nc(from, &beam_origins[beam]);
    one = 1.0f;
    func_001F9BF0_2E3700(span2, &D_L00_001E7130_2E3700[beam], from);
    func_L00_001FF4B0_2E3700(beam_direction, span2, one);
    fast_vec_cross(side, beam_direction, D_0013E633_2E3700);
    func_L00_001FF4B0_2E3700(side, side, -1.0f);
    fast_vec_cross(up, side, beam_direction);
    transform_vector(tip, &beam_strands[strand].points[18], beam_direction);
    qcopy(&D_L00_001E7160_2E3700[beam], tip);
    point = beam_strands[next_strand].points;
    for (second_strand_point_index = 0; second_strand_point_index < 19; second_strand_point_index++) {
        float fn = (float)second_strand_point_index;
        float twist;
        float swell;
        float height;
        float ripple;
        float random;

        point->z = 0.0f;
        twist = normalize_angle(beam_twist_phase[beam] + fn * 0.99483764f);
        swell = normalize_angle(beam_swell_phase[beam] + fn * 0.20944f);
        height = func_001F9FA8_2E3700(swell) * 0.5f;
        ripple = func_001F9FA8_2E3700(twist);
        random = randf_sym(0.0f, 0.2f);
        point->z += height * ripple + random;
        point->y = func_001F9F90_2E3700(twist) * 0.3f;
        point->w = 1.0f;
        point->x = step * fn;
        point++;
    }
    {
        float one;
        float speed = random_float_between(0.01f, 0.05f);
        float angle;

        one = 1.0f;
        angle = randf_sym(0.34906584f, 0.6981317f);
        particle_velocity.q = 0;
        particle_velocity.f[0] = one;
        particle_velocity.f[3] = one;
        particle_velocity.f[0] = speed * func_001F9F90_2E3700(angle);
        particle_velocity.f[1] = speed * func_001F9FA8_2E3700(angle);
        transform_vector_by_basis(&particle_velocity, &particle_velocity, &beam_transforms[beam]);
        spawn_mode_controlled_rotating_alpha_fade_particle(beam_transforms[beam].origin, &particle_velocity, 0x7F, beam_slot_id[beam]);
        if (random_integer_below(3) == 0) {
            Part *spark;

            fast_vec_scale(first_endpoint_effect_position, &beam_transforms[beam], 0.3f);
            func_001F9BD8_2E3700(first_endpoint_effect_position, first_endpoint_effect_position, beam_transforms[beam].origin);
            spark = spawn_mode_controlled_rotating_alpha_fade_particle(first_endpoint_effect_position, &zero_vector, 0x7F, beam_slot_id[beam]);
            if (spark != 0) {
                PartSub *sub = &spark->sub;

                spark->fC = 120000.0f;
                spark->fA = func_001F9850_2E3700(4);
                sub->f10 = one / func_001FA888_2E3700(spark->fA);
                sub->f18 = 0x7F7F7F;
                if (beam_endpoint_spark_tag_mode[beam] == 1) sub->f16 = 1;
            }
        }
        random_float_between(0.01f, 0.05f);
        random_spherical_offset(&particle_velocity, 0.01f, 0.05f);
        spawn_mode_controlled_rotating_alpha_fade_particle(tip, &particle_velocity, 0x7F, beam_slot_id[beam]);
        if (random_integer_below(3) == 0) {
            Part *spark = spawn_mode_controlled_rotating_alpha_fade_particle(tip, &zero_vector, 0x7F, beam_slot_id[beam]);

            if (spark != 0) {
                PartSub *sub = &spark->sub;

                spark->fC = 120000.0f;
                spark->fA = func_001F9850_2E3700(4);
                sub->f10 = 1.0f / func_001FA888_2E3700(spark->fA);
                sub->f18 = 0x7F7F7F;
                if (beam_endpoint_spark_tag_mode[beam] == 1) sub->f16 = 2;
            }
        }
    }
    if (beam_endpoint_effect_flags[beam] != 0) {
        if (beam_endpoint_effect_flags[beam] & 1) {
            qcopy(first_endpoint_effect_position, &beam_origins[beam]);
            first_endpoint_effect_position[2] -= 0.25f;
            if (beam_endpoint_light[beam] == -1) {
                beam_endpoint_light[beam] = allocate_colored_effect_slot(first_endpoint_effect_position, D_L00_00161BF0_gp, 0.0f, 1.0f, 1.0f, 2.0f);
            }
            if (beam_endpoint_light[beam] >= 0) {
                PointLight *light = &point_lights[beam_endpoint_light[beam]];

                qcopy(&light->position, first_endpoint_effect_position);
                light->position.f[3] = D_L00_00161BF0_gp;
            }
        }
        if (beam_endpoint_effect_flags[beam] & 2) {
            tip[2] += 0.5f;
            if (beam_tip_light[beam] == -1) {
                beam_tip_light[beam] = allocate_colored_effect_slot(tip, D_L00_00161BF0_gp, 0.0f, 1.0f, 1.0f, 2.0f);
            }
            if (beam_tip_light[beam] >= 0) {
                PointLight *light = &point_lights[beam_tip_light[beam]];

                qcopy(&light->position, tip);
                light->position.f[3] = D_L00_00161BF0_gp;
            }
        }
    }
}
/* Scrolls and draws the two panes of each active slot (3 slots). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E3FA0), where it is exact; names translated to the US level program. */

extern float beam_scroll[] __asm__("D_L00_00161C78") MACRO_ADDR;
extern int beam_slot_state[] __asm__("D_L00_00161BF8") MACRO_ADDR;
extern int beam_slot_id[] __asm__("D_L00_00161C88") MACRO_ADDR;
extern void draw_beam_strand_with_glow(int, int, void *, int, int, int) __asm__("FUN_L00_002e2c88");
extern void draw_beam_endpoint_quads_with_glow(int, int) __asm__("FUN_L00_002e3388");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

void draw_active_beam_slots(char *moby) __asm__("FUN_L00_002e2af0");

void draw_active_beam_slots(char *moby) {
    int *moby_data = ((int *)MOBY(moby)->pvars);
    int i;
    vu1_add_g_sregister(0x47, 0x51001);
    for (i = 0; i < 3; i++) {
        float scroll;
        int texture_id, glow_texture_id;
        int pane, next_pane;
        if (beam_slot_id[i] == 0 || beam_slot_state[i] < 2) continue;
        pane = i << 1;
        next_pane = pane + 1;
        scroll = beam_scroll[i] - frame_scale * 0.2f;
        beam_scroll[i] = scroll;
        if (scroll <= -8.0f) beam_scroll[i] = scroll + 8.0f;
        draw_beam_endpoint_quads_with_glow(beam_slot_id[i], i);
        texture_id = moby_data[1] + 0x28;
        glow_texture_id = moby_data[0] + 0x28;
        draw_beam_strand_with_glow(texture_id, glow_texture_id, &beam_strands[pane], 0x80, 0x40, i);
        draw_beam_strand_with_glow(texture_id, glow_texture_id, &beam_strands[next_pane], 0x30, 0x10, i);
    }
}
/* Builds the trail quad and its glow quad from the slot tables and draws them along the
   19 steps of points. */
#include "rnc/rendering/geometry_quad.h"
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E4138). */
typedef struct { OvlVec4 axis, unk10, position, unk30; } TrailSlot;
extern TrailSlot l00_beam_strand_axis_and_origin_slots[] __asm__("D_L00_001E6D30");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
void draw_beam_strand_with_glow(int texture_id, int glow_texture_id, void *points, int alpha, int glow_alpha, int slot)
{
    struct GeometryQuad quads[2];
    OvlVec4 width, glow_width;
    OvlVec4 basis[4];
    OvlVec4 transform[4];
    OvlVec4 direction, from, to;
    float zero, half_width, uv_offset;
    int color, glow_color;
    int i;

    quads[0].texture = get_effect_texture(texture_id);
    color = (alpha << 24) | 0x7F7F7F;
    quads[0].primitive = 0x8000000048L;
    quads[0].texture_state = 0xFF9000000260L;
    quads[0].reserved.value = 0;
    quads[0].colors[0] = quads[0].colors[1] = quads[0].colors[2] = quads[0].colors[3] = color;
    quads[1].texture = get_effect_texture(glow_texture_id);
    *(int *)&quads[1].texture_coordinates[0][1] = 0;
    glow_color = (glow_alpha << 24) | 0x7F4040;
    zero = quads[1].texture_coordinates[0][1];
    uv_offset = beam_scroll_macro[slot];
    quads[1].colors[0] = quads[1].colors[1] = quads[1].colors[2] = quads[1].colors[3] = glow_color;
    quads[1].primitive = quads[0].primitive;
    quads[1].texture_state = quads[0].texture_state;
    quads[1].reserved.value = quads[0].reserved.value;
    quads[1].texture_coordinates[0][0] = 1.0f;
    quads[1].texture_coordinates[1][0] = 1.0f;
    quads[1].texture_coordinates[1][1] = 1.0f;
    quads[1].texture_coordinates[2][0] = 0.0f;
    quads[1].texture_coordinates[2][1] = 0.0f;
    quads[1].texture_coordinates[3][0] = 0.0f;
    quads[1].texture_coordinates[3][1] = 1.0f;
    quads[0].texture_coordinates[1][0] = quads[0].texture_coordinates[0][0] = uv_offset + zero;
    quads[0].texture_coordinates[0][1] = zero;
    quads[0].texture_coordinates[1][1] = 1.0f;
    quads[0].texture_coordinates[3][0] = quads[0].texture_coordinates[2][0] = uv_offset + 1.0f;
    quads[0].texture_coordinates[2][1] = zero;
    quads[0].texture_coordinates[3][1] = 1.0f;
    half_width = 0.6f;
    if (*(short *)(beam_slot_id[slot] + 0xA6) == 0x1C) half_width = 0.3f;
    width.q = 0;
    glow_width.q = 0;
    glow_width.f[2] = half_width + half_width;
    width.f[3] = 1.0f;
    width.f[2] = half_width;
    glow_width.f[3] = 1.0f;

    qcopy(&basis[0], &l00_beam_strand_axis_and_origin_slots[slot].axis);
    fast_vec_sub(&basis[1], D_L00_00166DC0, &l00_beam_strand_axis_and_origin_slots[slot].position);
    fast_vec_scale(&basis[1], &basis[1], 1.0f);
    fast_vec_cross(&basis[2], &basis[1], &basis[0]);
    normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
    fast_vec_cross(&basis[0], &basis[2], &basis[1]);
    qcopy(&basis[3], &l00_beam_strand_axis_and_origin_slots[slot].position);
    qcopy(&transform[3], &l00_beam_strand_axis_and_origin_slots[slot].position);
    fast_vec_sub(&direction, &D_L00_001E6DE0[slot], &transform[3]);
    normalize_vector_xyz(&transform[0], &direction, 1.0f);
    fast_vec_cross(&transform[1], &transform[0], D_0013E633_2E3700);
    normalize_vector_xyz(&transform[1], &transform[1], -1.0f);
    fast_vec_cross(&transform[2], &transform[1], &transform[0]);

    qcopy(&quads[0].positions[0], &width);
    fast_vec_scale(&quads[0].positions[1], &quads[0].positions[0], -1.0f);
    {
        /* Fakematch: plain C copies &glow_width here and in the loop and stores glow_width.f[2];
           the pointer copies, the empty loop (it stops regmove) and the dead reset in the loop
           make 2.95.2 copy the pointer to $v1 before this qcopy, as retail does. */
        OvlVec4 *glow_src = &glow_width;
        OvlVec4 *glow;
        {
            OvlVec4 *glow_dst = &quads[1].positions[0];
            glow = glow_src;
            /* Fakematch: the empty loop stops regmove (see above). */
            do { } while (0);
            qcopy(glow_dst, glow);
        }
        fast_vec_scale(&quads[1].positions[1], &quads[1].positions[0], -1.0f);
        transform_vector(&from, &((OvlVec4 *)points)[0], transform);
        transform_vector(&to, &((OvlVec4 *)points)[1], transform);
        fast_vec_sub(&basis[0], &to, &from);
        normalize_vector_xyz(&basis[0], &basis[0], 1.0f);
        fast_vec_sub(&basis[1], D_L00_00166DC0, &from);
        fast_vec_cross(&basis[2], &basis[1], &basis[0]);
        normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
        fast_vec_cross(&basis[1], &basis[0], &basis[2]);
        qcopy(&basis[3], &from);
        transform_vector(&quads[0].positions[0], &quads[0].positions[0], basis);
        transform_vector(&quads[0].positions[1], &quads[0].positions[1], basis);
        transform_vector(&quads[1].positions[0], &quads[1].positions[0], basis);
        transform_vector(&quads[1].positions[1], &quads[1].positions[1], basis);

        i = 1;
        do {
            float taper, scroll;
            qcopy(&quads[0].positions[2], &width);
            fast_vec_scale(&quads[0].positions[3], &quads[0].positions[2], -1.0f);
            qcopy(&quads[1].positions[2], glow);
            fast_vec_scale(&quads[1].positions[3], &quads[1].positions[2], -1.0f);
            transform_vector(&from, &((OvlVec4 *)points)[i], transform);
            transform_vector(&to, &((OvlVec4 *)points)[i + 1], transform);
            fast_vec_sub(&basis[0], &to, &from);
            normalize_vector_xyz(&basis[0], &basis[0], 1.0f);
            fast_vec_sub(&basis[1], D_L00_00166DC0, &from);
            fast_vec_cross(&basis[2], &basis[1], &basis[0]);
            normalize_vector_xyz(&basis[2], &basis[2], 1.0f);
            fast_vec_cross(&basis[1], &basis[0], &basis[2]);
            qcopy(&basis[3], &from);
            transform_vector(&quads[0].positions[2], &quads[0].positions[2], basis);
            transform_vector(&quads[0].positions[3], &quads[0].positions[3], basis);
            transform_vector(&quads[1].positions[2], &quads[1].positions[2], basis);
            transform_vector(&quads[1].positions[3], &quads[1].positions[3], basis);
            if (i == 1) {
                int saved_color = quads[0].colors[0];
                int saved_glow_color = quads[1].colors[0];
                quads[0].colors[0] = quads[0].colors[1] = 0xFFFFFF;
                quads[1].colors[0] = quads[1].colors[1] = 0x7F4040;
                draw_geometry_quad(&quads[0], 0, 0);
                draw_geometry_quad(&quads[1], 0, 0);
                quads[0].colors[0] = quads[0].colors[1] = quads[0].colors[2] = quads[0].colors[3] = saved_color;
                quads[1].colors[0] = quads[1].colors[1] = quads[1].colors[2] = quads[1].colors[3] = saved_glow_color;
            } else {
                draw_geometry_quad(&quads[0], 0, 0);
                draw_geometry_quad(&quads[1], 0, 0);
            }
            qcopy(&quads[0].positions[0], &quads[0].positions[2]);
            qcopy(&quads[0].positions[1], &quads[0].positions[3]);
            qcopy(&quads[1].positions[0], &quads[1].positions[2]);
            qcopy(&quads[1].positions[1], &quads[1].positions[3]);
            taper = cubic_hermite_scalar(-1.0f, zero, 1.0f, zero, convert_integer_to_float(i) / convert_integer_to_float(20));
            scroll = beam_scroll[slot];
            quads[0].texture_coordinates[0][0] = scroll + zero;
            quads[0].texture_coordinates[0][1] = zero;
            quads[0].texture_coordinates[1][0] = scroll + zero;
            quads[0].texture_coordinates[1][1] = 1.0f;
            quads[0].texture_coordinates[2][0] = scroll + 1.0f;
            quads[0].texture_coordinates[2][1] = zero;
            quads[0].texture_coordinates[3][0] = scroll + 1.0f;
            quads[0].texture_coordinates[3][1] = 1.0f;
            width.f[2] = half_width + (0.05f - half_width) * taper;
            /* Fakematch: plain C stores glow_width.f[2]; the dead reset keeps glow_src live up to the loop. */
            { OvlVec4 *next_glow = &glow_width; next_glow->f[2] = width.f[2] + width.f[2]; }
            glow_src = &glow_width;
            i++;
        } while (i < 19);
    }
}
typedef struct {
    f32 v[4][4];
    s32 col[4];
    f32 uv[4][2];
    s64 r70;
    s64 r78;
    u64 r80;
    u64 r88;
} Q_e3388;

extern BeamOrigin beam_origins[] __asm__("D_L00_001E6D50");
extern f32 D_L00_001E6E10_e3388[][4] __asm__("D_L00_001E6E10");
extern u8 g_debug_camera[] __asm__("D_L00_00166DC0");

void FUN_001f9a28_e3388(void *, void *, void *) __asm__("FUN_001f9a28");
void FUN_001f9bf8_e3388(void *, void *, f32) __asm__("FUN_001f9bf8");
void FUN_001f9ad8_e3388(void *, void *, void *) __asm__("FUN_001f9ad8");
s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
void FUN_001f9a68_e3388(void *, void *, f32) __asm__("FUN_001f9a68");
void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
void FUN_001f9a10_e3388(void *, void *, void *) __asm__("FUN_001f9a10");
void draw_geometry_quad(void *, s32, s32) __asm__("FUN_001f7d30");

extern f32 beam_endpoint_quad_local_vertices[4][4] __asm__("D_L00_001E6CE0");
void draw_beam_endpoint_quads_with_glow(s32 unused, s32 beam_slot) __asm__("FUN_L00_002e3388");

void draw_beam_endpoint_quads_with_glow(s32 unused, s32 beam_slot) {
    Q_e3388 geometry_quad;
    f32 endpoint_to_camera_axis[4] __attribute__((aligned(16)));
    f32 endpoint_quad_side_axis[4] __attribute__((aligned(16)));
    f32 endpoint_quad_up_axis[4] __attribute__((aligned(16)));
    f32 quad_endpoint_position[4] __attribute__((aligned(16)));
    f32 core_quad_jitter_offset[4] __attribute__((aligned(16)));
    f32 quad_scale;
    s32 i;
    s32 j;

    qcopy(quad_endpoint_position, &beam_origins[beam_slot]);
    quad_endpoint_position[3] = 1.0f;
    FUN_001f9a28_e3388(endpoint_to_camera_axis, g_debug_camera, quad_endpoint_position);
    FUN_001f9bf8_e3388(endpoint_to_camera_axis, endpoint_to_camera_axis, 1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_side_axis, endpoint_to_camera_axis, &hero.gravity_dir);
    FUN_001f9bf8_e3388(endpoint_quad_side_axis, endpoint_quad_side_axis, -1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_up_axis, endpoint_quad_side_axis, endpoint_to_camera_axis);
    geometry_quad.r78 = get_effect_texture(0xB);
    geometry_quad.r88 = 0xFF00000044;
    geometry_quad.r80 = 0xFF9000000260;
    geometry_quad.r70 = 5;
    geometry_quad.col[3] = -1;
    geometry_quad.col[2] = -1;
    geometry_quad.col[1] = -1;
    geometry_quad.col[0] = -1;
    geometry_quad.uv[0][0] = 0.0f;
    geometry_quad.uv[0][1] = 0.0f;
    geometry_quad.uv[1][0] = 0.0f;
    geometry_quad.uv[1][1] = 1.0f;
    geometry_quad.uv[2][0] = 1.0f;
    geometry_quad.uv[2][1] = 0.0f;
    geometry_quad.uv[3][0] = 1.0f;
    geometry_quad.uv[3][1] = 1.0f;
    quad_scale = random_float_between(0.3f, 0.325f);
    core_quad_jitter_offset[0] = randf_sym(0.0f, 0.05f);
    core_quad_jitter_offset[1] = randf_sym(0.0f, 0.05f);
    core_quad_jitter_offset[2] = randf_sym(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], beam_endpoint_quad_local_vertices[i], quad_scale);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], endpoint_to_camera_axis);
        FUN_001f9a10_e3388(geometry_quad.v[i], geometry_quad.v[i], core_quad_jitter_offset);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    geometry_quad.r88 = 0xFF00000048;
    geometry_quad.col[3] = 0x607F4040;
    geometry_quad.col[2] = 0x607F4040;
    geometry_quad.col[1] = 0x607F4040;
    geometry_quad.col[0] = 0x607F4040;
    for (j = 0; j < 4; j++) {
        FUN_001f9a68_e3388(geometry_quad.v[j], beam_endpoint_quad_local_vertices[j], quad_scale * 3.0f);
        transform_vector(geometry_quad.v[j], geometry_quad.v[j], endpoint_to_camera_axis);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    qcopy(quad_endpoint_position, D_L00_001E6E10_e3388[beam_slot]);
    FUN_001f9a28_e3388(endpoint_to_camera_axis, g_debug_camera, quad_endpoint_position);
    FUN_001f9bf8_e3388(endpoint_to_camera_axis, endpoint_to_camera_axis, 1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_side_axis, endpoint_to_camera_axis, &hero.gravity_dir);
    FUN_001f9bf8_e3388(endpoint_quad_side_axis, endpoint_quad_side_axis, -1.0f);
    FUN_001f9ad8_e3388(endpoint_quad_up_axis, endpoint_quad_side_axis, endpoint_to_camera_axis);
    quad_scale = random_float_between(0.2f, 0.225f);
    geometry_quad.r88 = 0xFF00000044;
    geometry_quad.col[3] = -1;
    geometry_quad.col[2] = -1;
    geometry_quad.col[1] = -1;
    geometry_quad.col[0] = -1;
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], beam_endpoint_quad_local_vertices[i], quad_scale);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], endpoint_to_camera_axis);
        FUN_001f9a10_e3388(geometry_quad.v[i], geometry_quad.v[i], core_quad_jitter_offset);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
    geometry_quad.r88 = 0xFF00000048;
    geometry_quad.col[3] = 0x607F4040;
    geometry_quad.col[2] = 0x607F4040;
    geometry_quad.col[1] = 0x607F4040;
    geometry_quad.col[0] = 0x607F4040;
    for (i = 0; i < 4; i++) {
        FUN_001f9a68_e3388(geometry_quad.v[i], beam_endpoint_quad_local_vertices[i], quad_scale * 3.0f);
        transform_vector(geometry_quad.v[i], geometry_quad.v[i], endpoint_to_camera_axis);
    }
    draw_geometry_quad(&geometry_quad, 0, 0);
}
typedef struct {
    char p0[0x60];
    int path;
    float f64, f68, f6C;
    int i70;
    float f74;
} D_2e3a88;
extern int *toad_paths[] __asm__("D_L00_001B04B0") NOT_SDA;
extern float D_0015ED6C_2e3a88 __asm__("D_0015ED6C");
float fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
float absolute_float(float) __asm__("FUN_001f99c0");
void sample_camera_path(void *, float, int, void *, void *, int) __asm__("FUN_00214e58");
extern float pterodactyl_path_speed_gp __asm__("D_L00_00161C98") __attribute__((sda));
extern float pterodactyl_scale_multiplier_gp __asm__("D_L00_00161C9C") __attribute__((sda));
void pterodactyl_update(unsigned char *m) __asm__("FUN_L00_002e3a88");
void pterodactyl_update(unsigned char *moby) {
    enum {
        PTERODACTYL_INITIALIZE = 0,
        PTERODACTYL_FOLLOW_PATH = 1
    };
    D_2e3a88 *moby_data = ((D_2e3a88 *)MOBY(moby)->pvars);
    int *path = toad_paths[moby_data->path];
    float y;
    switch (moby[0x20]) {
    case PTERODACTYL_INITIALIZE:
        if (moby_data->path == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        {
            float first_segment_length = fast_vec_dist(path + 4, path + 8);
            float first_to_last_distance = fast_vec_dist(path + 4, path + *path * 4);
            moby_data->f74 = pterodactyl_path_speed_gp * D_0015ED6C_2e3a88 / first_segment_length;
            moby_data->i70 = absolute_float(first_segment_length - first_to_last_distance) < 0.1f;
        }
        moby_data->f6C = moby_data->f68 * (float)*path;
        moby[0x20] = 1;
        MOBY(moby)->scale *= pterodactyl_scale_multiplier_gp;
        moby[0x30] = 0xFF;
    case PTERODACTYL_FOLLOW_PATH:
        y = MOBY(moby)->pos.z;
        sample_camera_path(path, moby_data->f6C, moby_data->i70, moby + 0x10, moby + 0x40, 0);
        MOBY(moby)->rot.y *= 0.5f;
        MOBY(moby)->pos.z += moby_data->f64;
        moby_data->f6C += moby_data->f74;
        if ((float)*path < moby_data->f6C)
            moby_data->f6C -= (float)*path;
        if (moby[0x70] & 2) {
            if (y < MOBY(moby)->pos.z || random_integer_below(3) != 0) {
                if (moby[0x53])
                    blend_moby_animation(moby, 0, 0, 10);
            } else {
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, 10);
            }
        }
        break;
    }
}
float FUN_001f9b48(void *, void *);
float fast_vec_length(void *) __asm__("FUN_001f9af0");
int scale_game_frames(int) __asm__("FUN_001f96f8");
void red_plant_update(unsigned char *p) __asm__("FUN_L00_002e3ca8");
void red_plant_update(unsigned char *moby) {
    char *hero_position = (char *)&hero.motion.pos;
    if (FUN_001f9b48(hero_position, moby + 0x10) < 1.0f) {
        if (fast_vec_length(hero_position + 0x80) > frame_time + frame_time && moby[0x53] != 1) {
            blend_moby_animation(moby, 1, 0, scale_game_frames(6));
        }
    }
    if (moby[0x70] & 2) {
        if (moby[0x53] == 1) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(6));
        }
    }
}
/* Update of an effect-group moby: state machine over its life, then it steers a sprite along its path and bounces it off surfaces. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5238), where it is exact; names translated to the US level program. */

extern f32 fast_vec_dot(void *, void *) __asm__("func_001F9AB0");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void quaternion_to_matrix(void *, void *) __asm__("FUN_001fa4f8");
extern void spawn_explosion(void *, void *, void *, float, float, int, float, int, float, float, int, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern void build_quaternion_from_axis_angle(void *, void *, f32) __asm__("FUN_00214530");
extern void multiply_matrix3x3(void *, void *, void *) __asm__("func_001FA328");
extern void euler_rotation_basis(void *, void *) __asm__("func_001FA030");
extern int func_001F9908_r(int *) __asm__("FUN_001f9740");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern int func_L00_001F10E0_s(void *, float, int, void *) __asm__("FUN_L00_001f0d60");

#include "rnc/overlay/collision.h"
void robot_arm_update(char *m) __asm__("FUN_L00_002e3d88");
void robot_arm_update(char *moby) {
    enum {
        ROBOT_ARM_REMOVE = 0,
        ROBOT_ARM_WAIT_FOR_TIMER = 1,
        ROBOT_ARM_SPAWN_EFFECT_AND_REMOVE = 2,
        ROBOT_ARM_FADE_AND_REMOVE = 3
    };
    char *moby_data = ((char *)MOBY(moby)->pvars);
    float effect_offset_or_world_position[4];
    float rotation_step_basis[12];
    float offset_after_rotation[4];
    float rotation_offset_delta[4];
    float collision_probe_position[4];
    float bounce_rotation_matrix[16];
    float aD0[4];
    float aE0[4];
    char *rotation_step_euler;
    float collision_radius;
    float *collision_normal;

    switch (MOBY(moby)->state) {
    case ROBOT_ARM_REMOVE:
        mark_moby_for_removal(moby);
        return;
    case ROBOT_ARM_WAIT_FOR_TIMER:
        if (func_001F9908_r((int *)(moby_data + 0x30))) {
            moby[0x20] = (*(int *)(moby_data + 0x34) & 1) ? 3 : 2;
            *(int *)(moby_data + 0x30) = func_001FA898_r(scale_time(10.0f));
            return;
        }
        break;
    case ROBOT_ARM_SPAWN_EFFECT_AND_REMOVE:
        transform_vector_by_basis(effect_offset_or_world_position, moby_data + 0x20, moby + 0xC0);
        fast_vec_add(effect_offset_or_world_position, effect_offset_or_world_position, moby + 0x10);
        if (*(int *)(moby_data + 0x34) & 2) {
            spawn_color_swapped_explosion_burst(moby, effect_offset_or_world_position, -1, *(float *)(moby_data + 0x2C) * 0.5f, 0.0f);
        } else {
            float half_collision_radius = *(float *)(moby_data + 0x2C) * 0.5f;
            spawn_explosion(moby, &zero_vector, effect_offset_or_world_position, 0.0f, 0.0f, 5, *(float *)(moby_data + 0x2C), 3, half_collision_radius, 9.0f, 5, half_collision_radius, -1, 0.0f, 0, 0, -1, 0);
        }
        mark_moby_for_removal(moby);
        return;
    case ROBOT_ARM_FADE_AND_REMOVE:
        if (func_001F9908_r((int *)(moby_data + 0x30))) {
            mark_moby_for_removal(moby);
            return;
        }
        moby[0x23] = func_001FA898_r((float)*(int *)(moby_data + 0x30) / scale_time(10.0f) * 128.0f);
        break;
    }
    rotation_step_euler = moby_data + 0x10;
    euler_rotation_basis(rotation_step_basis, rotation_step_euler);
    transform_vector_by_basis(effect_offset_or_world_position, moby_data + 0x20, moby + 0xC0);
    multiply_matrix3x3(moby + 0xC0, rotation_step_basis, moby + 0xC0);
    transform_vector_by_basis(offset_after_rotation, moby_data + 0x20, moby + 0xC0);
    fast_vec_sub(rotation_offset_delta, offset_after_rotation, effect_offset_or_world_position);
    fast_vec_sub((float *)&MOBY(moby)->pos, (float *)&MOBY(moby)->pos, rotation_offset_delta);
    fast_vec_add(moby + 0x10, moby + 0x10, moby_data);
    collision_radius = *(float *)(moby_data + 0x2C);
    *(float *)(moby_data + 8) = *(float *)(moby_data + 8) - *(float *)(moby_data + 0x38);
    transform_vector_by_basis(collision_probe_position, moby_data + 0x20, moby + 0xC0);
    fast_vec_add(collision_probe_position, collision_probe_position, moby + 0x10);
    if (func_L00_001F10E0_s(collision_probe_position, collision_radius, 0, moby)) {
        float speed, approach;
        collision_normal = coll_hit_normal.f;
        normalize_vector_xyz(collision_normal, collision_normal, 1.0f);
        speed = fast_vec_length(moby_data);
        approach = fast_vec_dot(moby_data, collision_normal);
        if (approach < 0.0f) {
            reflect_vector(moby_data, moby_data, collision_normal);
            normalize_vector_xyz(aD0, collision_normal, approach * 0.5f);
            fast_vec_add(moby_data, moby_data, aD0);
            normalize_vector_xyz(moby_data, moby_data, speed);
            fast_vec_cross(aE0, collision_normal, moby_data);
            normalize_vector_xyz(aE0, aE0, 1.0f);
            build_quaternion_from_axis_angle(aE0, aE0, fast_vec_length(rotation_step_euler));
            quaternion_to_matrix(aE0, bounce_rotation_matrix);
            matrix_to_euler(bounce_rotation_matrix, rotation_step_euler);
        }
    }
}
/* Copies the vector at D_0013F3D0 into b; a is unused. */
void follow_camera_hero_target(int a, void *b) __asm__("FUN_L00_002e4168");
void follow_camera_hero_target(int unused_camera, void *out_hero_position) {
    qcopy(out_hero_position, &hero.motion.pos);
}
#include "eetypes.h"
typedef struct {
    char b[0x80];
} M_2e4180_002e4180;
extern M_2e4180_002e4180 *D_L00_001600F4_002e4180 __asm__("D_L00_001600F4");
extern char *FUN_L00_001ee1b0_002e4180(void *) __asm__("FUN_L00_001ee1b0");
extern void FUN_001f9a28_002e4180(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_002e4180(void *) __asm__("FUN_001f9af0");
extern void FUN_001f9a68_002e4180(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_001f9a10_002e4180(void *, void *, void *) __asm__("FUN_001f9a10");

int compute_follow_camera_radial_collision_positions(u128 *input_position, float radial_distance_extension) __asm__("FUN_L00_002e4180");

int compute_follow_camera_radial_collision_positions(u128 *input_position, float radial_distance_extension) {
    u128 lookup_position_copy;
    u128 position_delta;
    char *selected_record;
    char *reference_position;
    char *extended_position;
    float selected_record_vector_length, inv;
    lookup_position_copy = *input_position;
    {
        int *lookup_result = (int *)FUN_L00_001ee1b0_002e4180(&lookup_position_copy);
        if (lookup_result == 0)
            goto fail;
        selected_record = (char *)&D_L00_001600F4_002e4180[lookup_result[5]];
    }
    reference_position = selected_record + 0x30;
    FUN_001f9a28_002e4180(&position_delta, input_position, reference_position);
    selected_record_vector_length = FUN_001f9af0_002e4180(selected_record);
    inv = 1.0f / FUN_001f9af0_002e4180(&position_delta);
    extended_position = (char *)&coll_hit_position;
    FUN_001f9a68_002e4180(extended_position, &position_delta, (selected_record_vector_length + radial_distance_extension) * inv);
    FUN_001f9a68_002e4180(extended_position - 0x10, &position_delta, selected_record_vector_length * inv);
    FUN_001f9a10_002e4180(extended_position, extended_position, reference_position);
    FUN_001f9a10_002e4180(extended_position - 0x10, extended_position - 0x10, reference_position);
    return 1;
fail:
    return 0;
}
int find_camera_collision_volume_for_point(OvlQuad *, float) __asm__("FUN_L00_001ee1b0");
/* True when FUN_L00_001ee1b0 returns non-zero for a copy of v and 0. */
int test_follow_camera_point_in_collision_volume(OvlQuad *input_quad) __asm__("FUN_L00_002e4290");

int test_follow_camera_point_in_collision_volume(OvlQuad *input_quad) {
    OvlQuad input_copy = *input_quad;
    return find_camera_collision_volume_for_point(&input_copy, 0.0f) != 0;
}
typedef struct {
    u8 p0[0x2DC];
    f32 f2DC;
    u8 p1[0x30C - 0x2E0];
    s16 h30C;
    u8 p2[0x2080 - 0x30E];
    void *p2080;
} G_002e42c0;
extern struct Hero hero_data __asm__("D_0013F350") NOT_SDA;
extern u8 D_L00_00166E10_lui[] __asm__("D_L00_00166E10") NOT_SDA;
extern OvlQuad D_L00_00173E60_002e42c0 __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
void FUN_001f9a68_002e42c0(OvlQuad *, void *, f32) __asm__("FUN_001f9a68");
void FUN_001f9a10_002e42c0(OvlQuad *, OvlQuad *,
                           OvlQuad *) __asm__("FUN_001f9a10");
s32 FUN_001efa68_002e42c0(OvlQuad *, OvlQuad *, s32, void *,
                          s32) __asm__("FUN_001efa68");
s32 compute_follow_camera_ground_aware_target_position(OvlQuad *out, s32 flag) __asm__("FUN_L00_002e42c0");

s32 compute_follow_camera_ground_aware_target_position(OvlQuad *out, s32 flag) {
    OvlQuad position_and_offset_vectors[4];
    struct Hero *player = &hero_data;
    u8 *camera_view = D_L00_00166E10_lui;
    f32 ground_distance;
    if (player->frames_off_ground.s == 0) {
        follow_camera_hero_target(*(s32 *)(camera_view - 0x10), out);
        return 1;
    }
    follow_camera_hero_target(*(s32 *)(camera_view - 0x10), &position_and_offset_vectors[0]);
    ground_distance = player->ground_distance;
    if (30.0f <= ground_distance || flag == 0) {
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[3], camera_view + 0x30, 0.2f);
        FUN_001f9a10_002e42c0(&position_and_offset_vectors[1], &position_and_offset_vectors[0], &position_and_offset_vectors[3]);
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[3], camera_view + 0x30, -30.0f);
        FUN_001f9a10_002e42c0(&position_and_offset_vectors[2], &position_and_offset_vectors[0], &position_and_offset_vectors[3]);
        if (FUN_001efa68_002e42c0(&position_and_offset_vectors[1], &position_and_offset_vectors[2], 0x12, player->moby, 0)) {
            qcopy(out, &D_L00_00173E60_002e42c0);
            return 0;
        }
        qcopy(out, &position_and_offset_vectors[0]);
        return -1;
    } else {
        FUN_001f9a68_002e42c0(&position_and_offset_vectors[1], camera_view + 0x30, -ground_distance);
        FUN_001f9a10_002e42c0(out, &position_and_offset_vectors[0], &position_and_offset_vectors[1]);
        return 1;
    }
}
typedef float V4_2e4430[4] __attribute__((aligned(16)));
extern char D_L00_00166E10_002e4430[] __asm__("D_L00_00166E10") NOT_SDA;
void FUN_001f9a68_002e4430(void *, void *, float) __asm__("FUN_001f9a68");
void FUN_001f9a10_002e4430(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_001f9a28_002e4430(void *, void *, void *) __asm__("FUN_001f9a28");
float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
void clear_u64_value(void *) __asm__("FUN_001f99f8");
void follow_camera_reset(char *o) __asm__("FUN_L00_002e4430");
void follow_camera_reset(char *camera) {
    V4_2e4430 target_position, target_up_projection, b;
    char *g = D_L00_00166E10_002e4430;
    char *camera_up_axis = g + 0x30;
    char *camera_state = *(char **)(camera + 0x70);
    char *attach_state, *r;
    compute_follow_camera_ground_aware_target_position(target_position, 0);
    attach_state = camera_state + 0x40;
    r = camera_state + 0x130;
    {
        float x = fast_vec_dot(target_position, camera_up_axis);
        FUN_001f9a68_002e4430(target_up_projection, camera_up_axis, x);
    }
    FUN_001f9a28_002e4430(attach_state, target_position, target_up_projection);
    qcopy(camera_state + 0x50, target_up_projection);
    FUN_001f9a10_002e4430(attach_state, attach_state, camera_state + 0x50);
    g += 0x20;
    FUN_001f9a68_002e4430(b, g, *(float *)(attach_state + 0xB0));
    FUN_001f9a10_002e4430(camera_state + 0x80, b, attach_state);
    FUN_001f9a10_002e4430(camera_state + 0xD0, b, attach_state);
    clear_u64_value(camera_state + 0xE0);
    attach_state[0xC4] = 0;
    *(float *)(attach_state + 0xC8) = 1.0f;
    attach_state[0xC5] = 0;
    *(s16 *)(attach_state + 0xC6) = 0;
    qcopy(camera_state + 0xA0, attach_state);
    clear_u64_value(camera_state + 0xB0);
    clear_u64_value(camera_state + 0xC0);
    FUN_001f9a68_002e4430(b, g, *(float *)(r + 0x30));
    FUN_001f9a10_002e4430(camera_state + 0x90, b, attach_state);
}
/* angle between a direction and an axis, signed */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5A30), where it is exact; names translated to the US level program. */

extern char D_L00_00166E10[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_arc_sin(float) __asm__("FUN_001f9df8");
extern void FUN_001f9a10_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a68_c3(void *, void *, float) __asm__("FUN_001f9a68");

float compute_follow_camera_signed_target_angle(char *camera, float target_offset_scale) __asm__("FUN_L00_002e4580");

float compute_follow_camera_signed_target_angle(char *camera, float target_offset_scale) {
    float hero_position[4], offset_target_position[4], angle_direction[4], axis_projection[4];
    float reference_dot_or_signed_angle, axis_dot_or_direction_length;
    char *camera_view = D_L00_00166E10;
    follow_camera_hero_target(camera, hero_position);
    FUN_001f9a68_c3(offset_target_position, camera_view + 0x20, target_offset_scale);
    FUN_001f9a10_c2(offset_target_position, offset_target_position, hero_position);
    fast_vec_sub(angle_direction, offset_target_position, (float *)(camera + 0x30));
    axis_dot_or_direction_length = fast_vec_dot(angle_direction, camera + 0x10);
    FUN_001f9a68_c3(axis_projection, camera + 0x10, axis_dot_or_direction_length);
    fast_vec_sub(angle_direction, angle_direction, axis_projection);
    reference_dot_or_signed_angle = fast_vec_dot(angle_direction, camera);
    axis_dot_or_direction_length = fast_vec_length(angle_direction);
    if (axis_dot_or_direction_length == 0.0f) {
        return 0.0f;
    }
    reference_dot_or_signed_angle = 1.5707964f - fast_arc_sin(reference_dot_or_signed_angle / axis_dot_or_direction_length);
    if (fast_vec_dot(angle_direction, camera + 0x20) < 0.0f) {
        reference_dot_or_signed_angle = -reference_dot_or_signed_angle;
    }
    return reference_dot_or_signed_angle;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5B68), where it is exact; names translated to the US level program. */

typedef struct {
    int a[4];
} Vq4 __attribute__((aligned(16)));

extern char D_0013E533[];
extern int FUN_001f96f8_c(int) __asm__("FUN_001f96f8");
extern void FUN_001f9a10_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a68_c(void *, void *, float) __asm__("FUN_001f9a68");

void apply_follow_camera_translation(char *camera) __asm__("FUN_L00_002e46b8");

void apply_follow_camera_translation(char *camera) {
    Vq4 translation_delta;
    Vq4 camera_translation_adjustment, lateral_displacement;
    char *camera_view = D_L00_00166E10;
    char *camera_state = *(char **)(camera + 0x70);
    char *attach_state = camera_state + 0x40;
    char *ground_check_hero, *state_check_hero;
    qcopy(&translation_delta, D_0013E533 + 0xF5D);
    FUN_001f9a10_c(camera + 0x30, &translation_delta, camera + 0x30);
    FUN_001f9a10_c(camera_state + 0xA0, &translation_delta, camera_state + 0xA0);
    FUN_001f9a10_c(attach_state, &translation_delta, attach_state);
    FUN_001f9a10_c(camera_state + 0xD0, &translation_delta, camera_state + 0xD0);
    FUN_001f9a10_c(camera_state, &translation_delta, camera_state);
    ground_check_hero = D_0013E533 + 0xE1D;
    if (*(short *)(ground_check_hero + 0x308) == 0) {
        char *ground_moby = *(char **)(ground_check_hero + 0x2FC);
        char *camera_system_state;
        if (ground_moby == 0 || MOBY(ground_moby)->oclass != 0x22D ||
            ((camera_system_state = camera_view - 0x190), FUN_001f96f8_c(0x1E)) < *(int *)(camera_system_state + 0x398)) {
            char *translation_source_hero = D_0013E533 + 0xE1D;
            *(float *)(*(char **)(camera + 0x70) + 0x1C8) = *(float *)(translation_source_hero + 0x14C);
        }
    }
    state_check_hero = D_0013E533 + 0xE1D;
    if ((unsigned)(*(int *)(state_check_hero + 0x2084) - 0xB) < 2) {
        char *reloaded_camera_state = *(char **)(camera + 0x70);
        char *camera_up_axis = camera_view + 0x30;
        float translation_up_component;
        *(short *)(reloaded_camera_state + 0x10) = 0;
        FUN_001f9a68_c(&camera_translation_adjustment, camera_view + 0x80, *(float *)(camera_view + 0xA4));
        translation_up_component = fast_vec_dot(camera_up_axis, &translation_delta);
        FUN_001f9a68_c(&lateral_displacement, camera_up_axis, translation_up_component);
        fast_vec_sub((float *)&lateral_displacement, (float *)&translation_delta, (float *)&lateral_displacement);
        fast_vec_sub((float *)&camera_translation_adjustment, (float *)&camera_translation_adjustment, (float *)&lateral_displacement);
        FUN_001f9a10_c(camera + 0x30, &camera_translation_adjustment, camera + 0x30);
        FUN_001f9a10_c(attach_state + 0x60, &camera_translation_adjustment, attach_state + 0x60);
        FUN_001f9a10_c(attach_state, &camera_translation_adjustment, attach_state);
        FUN_001f9a10_c(attach_state + 0x90, &camera_translation_adjustment, attach_state + 0x90);
        FUN_001f9a10_c(reloaded_camera_state, &camera_translation_adjustment, reloaded_camera_state);
    }
}
typedef float V4[4] __attribute__((aligned(16)));
void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void vcopy_2e48a0(void *, void *) __asm__("FUN_001f9a68");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
void compute_follow_camera_offset_target_up_projection(void *out) __asm__("FUN_L00_002e48a0");

void compute_follow_camera_offset_target_up_projection(void *out) {
    V4 offset_target_position, hero_position;
    char *camera_view = D_L00_00166E10;
    vcopy_2e48a0(offset_target_position, camera_view + 0x20);
    follow_camera_hero_target(*(int *)(camera_view - 0x10), hero_position);
    fast_vec_add(offset_target_position, offset_target_position, hero_position);
    fast_vec_scale(out, camera_view + 0x30, fast_vec_dot(camera_view + 0x30, offset_target_position));
}
/* camera attach mode transitions: picks the next state from the player's state and arms its timers */
/* Migrated from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E5DE0). */

typedef struct {
    float v00[4];
    float v10[4];
    float v20[4];
    float v30[4];
    char pad40[0x84];
    unsigned char state;
    unsigned char prev;
    short timer;
    float invTimer;
    float invMove;
    float dist;
    short moveTimer;
    char padD6;
    unsigned char hit;
} CamAttach;

extern int camera_secondary_mode_lui __asm__("D_001413D4")NOT_SDA;
extern int D_001413DC NOT_SDA;
extern float D_L00_00166EEC NOT_SDA;
extern int FUN_001f9770_c2(void *) __asm__("FUN_001f9770");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 D_L00_00161D24_gp __asm__("D_L00_00161D24") __attribute__((sda));
extern short follow_camera_ground_aware_focus_transition_frames_gp __asm__("D_L00_00161D38") __attribute__((sda));
extern short D_L00_00161D3C_gp __asm__("D_L00_00161D3C") __attribute__((sda));
extern short D_L00_00161D40_gp __asm__("D_L00_00161D40") __attribute__((sda));
extern short follow_camera_blend_out_focus_offset_gp __asm__("D_L00_00161D48") __attribute__((sda));
extern short follow_camera_project_hero_focus_blend_base_frames_gp __asm__("D_L00_00161D4C") __attribute__((sda));
extern short follow_camera_focus_blend_transition_frames_gp __asm__("D_L00_00161D50") __attribute__((sda));
extern short D_L00_00161D54_gp __asm__("D_L00_00161D54") __attribute__((sda));
extern short follow_camera_project_hero_focus_offset_gp __asm__("D_L00_00161D58") __attribute__((sda));
extern short follow_camera_hold_focus_blend_base_frames_gp __asm__("D_L00_00161D5C") __attribute__((sda));
extern short follow_camera_hold_focus_transition_frames_gp __asm__("D_L00_00161D60") __attribute__((sda));
extern short follow_camera_hold_focus_exit_base_frames_gp __asm__("D_L00_00161D64") __attribute__((sda));
extern short follow_camera_hold_focus_blend_offset_gp __asm__("D_L00_00161D68") __attribute__((sda));
extern short D_L00_00161D6C_gp __asm__("D_L00_00161D6C") __attribute__((sda));
extern short follow_camera_continue_projected_focus_transition_frames_gp __asm__("D_L00_00161D78") __attribute__((sda));
extern short follow_camera_projected_focus_blend_offset_gp __asm__("D_L00_00161D7C") __attribute__((sda));
extern void FUN_L00_002e48a0_c(void *out, float f) __asm__("FUN_L00_002e48a0");

void FUN_L00_002e4930(char *camera) {
    char *camera_state = *(char **)(camera + 0x70);
    CamAttach *attach_state = (CamAttach *)(camera_state + 0x40);
    char *timer_camera_state;
    float inv, x, y;
    int camera_timer_base_frames;

    if (attach_state->state == 0) {
        struct Hero *player = &hero;
        if (player->state.control_mode == 2) {
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = D_L00_00161D3C_gp;
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        } else {
            struct Hero *current_player = &hero;
            if (current_player->state.current == 0xF && D_L00_00166EEC == 0.0f) {
                attach_state->prev = attach_state->state;
                attach_state->state = 3;
                attach_state->moveTimer = scale_game_frames(*(int *)&follow_camera_project_hero_focus_blend_base_frames_gp);
                attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
                attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
                inv = 1.0f / convert_integer_to_float(attach_state->timer);
                attach_state->dist = *(float *)&follow_camera_project_hero_focus_offset_gp;
                attach_state->hit = 0;
                x = *(float *)&follow_camera_project_hero_focus_offset_gp;
                attach_state->invTimer = inv;
                FUN_L00_002e48a0_c(attach_state->v30, x);
                qcopy(attach_state->v20, attach_state->v10);
            } else {
                struct Hero *state_check_player = &hero;
                if (state_check_player->state.current == 0xC && D_L00_00166EEC == 0.0f) {
                    attach_state->prev = attach_state->state;
                    attach_state->state = 8;
                    attach_state->timer = follow_camera_hold_focus_transition_frames_gp;
                    attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
                } else {
                    struct Hero *hero_state_player = &hero;
                    if (hero_state_player->state.current == 0xB && D_L00_00166EEC == 0.0f) {
                        attach_state->prev = attach_state->state;
                        attach_state->state = 6;
                        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
                        attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
                        attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
                        inv = 1.0f / convert_integer_to_float(attach_state->timer);
                        attach_state->dist = *(float *)&follow_camera_blend_out_focus_offset_gp;
                        attach_state->hit = 0;
                        x = *(float *)&follow_camera_blend_out_focus_offset_gp;
                        attach_state->invTimer = inv;
                        FUN_L00_002e48a0_c(attach_state->v30, x);
                        qcopy(attach_state->v20, attach_state->v10);
                    } else {
                        struct Hero *state_transition_player = &hero;
                        if ((state_transition_player->state.current == 0xD || state_transition_player->state.current == 0xE) &&
                            D_L00_00166EEC == 0.0f) {
                            attach_state->prev = attach_state->state;
                            attach_state->state = 5;
                            attach_state->moveTimer = scale_game_frames(*(int *)&follow_camera_hold_focus_blend_base_frames_gp);
                            attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
                            attach_state->timer = follow_camera_hold_focus_transition_frames_gp;
                            inv = 1.0f / convert_integer_to_float(attach_state->timer);
                            attach_state->dist = *(float *)&follow_camera_hold_focus_blend_offset_gp;
                            attach_state->hit = 0;
                            x = *(float *)&follow_camera_hold_focus_blend_offset_gp;
                            attach_state->invTimer = inv;
                            FUN_L00_002e48a0_c(attach_state->v30, x);
                            qcopy(attach_state->v20, attach_state->v10);
                        } else {
                            struct Hero *control_mode_player = &hero;
                            if (control_mode_player->state.control_mode == 4 && D_L00_00166EEC == 0.0f) {
                                attach_state->prev = attach_state->state;
                                attach_state->state = 2;
                                attach_state->timer = follow_camera_ground_aware_focus_transition_frames_gp;
                                attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
                            }
                        }
                    }
                }
            }
        }
        return;
    }
    if (attach_state->state != 9 && attach_state->state != 1 && camera_secondary_mode_lui == 0x14) {
        float r;
        attach_state->prev = attach_state->state;
        attach_state->state = 9;
        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D6C_gp);
        r = 1.0f / convert_integer_to_float(attach_state->moveTimer);
        y = *(float *)&follow_camera_projected_focus_blend_offset_gp;
        attach_state->hit = 0;
        attach_state->timer = 0;
        x = *(float *)&follow_camera_projected_focus_blend_offset_gp;
        attach_state->invTimer = 1.0f;
        attach_state->dist = y;
        attach_state->invMove = r;
        FUN_L00_002e48a0_c(attach_state->v30, x);
        qcopy(attach_state->v20, attach_state->v10);
        return;
    }
    if ((attach_state->state == 1 || attach_state->state == 2) && D_001413DC != 4 && D_001413DC != 2) {
        attach_state->timer = 0;
        attach_state->invTimer = 1.0f;
        attach_state->state = 0;
        return;
    }
    if (attach_state->state == 7) {
        struct Hero *p = &hero;
        if (p->past_jump_apex == 0) {
            attach_state->prev = attach_state->state;
            attach_state->state = 6;
            attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
            attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
            attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
            inv = 1.0f / convert_integer_to_float(attach_state->timer);
            attach_state->dist = *(float *)&follow_camera_blend_out_focus_offset_gp;
            attach_state->hit = 0;
            x = *(float *)&follow_camera_blend_out_focus_offset_gp;
            attach_state->invTimer = inv;
            FUN_L00_002e48a0_c(attach_state->v30, x);
            qcopy(attach_state->v20, attach_state->v10);
            return;
        }
        if (p->state.control_mode == 2) {
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
            return;
        }
    }
    if (attach_state->state == 4 && D_001413DC != 4) {
        if (D_001413DC == 2) {
            attach_state->prev = 4;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        } else {
            attach_state->prev = 4;
            attach_state->state = 0;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        }
        return;
    }
    if (attach_state->state == 3 && camera_secondary_mode_lui == 0xB) {
        attach_state->prev = attach_state->state;
        attach_state->state = 6;
        attach_state->moveTimer = scale_game_frames(*(int *)&D_L00_00161D40_gp);
        attach_state->invMove = 1.0f / convert_integer_to_float(attach_state->moveTimer);
        attach_state->timer = follow_camera_focus_blend_transition_frames_gp;
        inv = 1.0f / convert_integer_to_float(attach_state->timer);
        attach_state->dist = *(float *)&follow_camera_blend_out_focus_offset_gp;
        attach_state->hit = 0;
        x = *(float *)&follow_camera_blend_out_focus_offset_gp;
        attach_state->invTimer = inv;
        FUN_L00_002e48a0_c(attach_state->v30, x);
        qcopy(attach_state->v20, attach_state->v10);
        return;
    }
    if (attach_state->state == 5 && camera_secondary_mode_lui == 0xC) {
        attach_state->prev = attach_state->state;
        attach_state->state = 8;
        attach_state->timer = follow_camera_hold_focus_transition_frames_gp;
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 8 && camera_secondary_mode_lui != 0xC) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&follow_camera_hold_focus_exit_base_frames_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 6 && camera_secondary_mode_lui != 0xB) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if ((attach_state->state == 3 || attach_state->state == 5) && D_001413DC != 4) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 9) {
        struct Hero *p = &hero;
        if (p->state.control_mode == 2 || p->frames_off_ground.s >= scale_game_frames(0x37)) {
            goto to_one;
        }
    }
    if (attach_state->state == 0xA) {
        struct Hero *p = &hero;
        if (p->state.control_mode == 2 || p->frames_off_ground.s >= scale_game_frames(0x37)) {
            float r;
        to_one:
            attach_state->prev = attach_state->state;
            attach_state->state = 1;
            attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
            r = 1.0f / convert_integer_to_float(attach_state->timer);
            camera_timer_base_frames = *(int *)&D_L00_00161D24_gp;
            attach_state->invTimer = r;
            timer_camera_state = *(char **)(camera + 0x70);
            *(short *)(timer_camera_state + 0x20) = scale_game_frames(camera_timer_base_frames);
            qcopy(camera + 0x40, camera);
            return;
        }
    }
    if (attach_state->state == 9 && camera_secondary_mode_lui != 0x14) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0xA;
        attach_state->timer = follow_camera_continue_projected_focus_transition_frames_gp;
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 0xA && FUN_001f9770_c2(&attach_state->moveTimer)) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&D_L00_00161D54_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
        return;
    }
    if (attach_state->state == 5 && camera_secondary_mode_lui == 0x10) {
        attach_state->prev = attach_state->state;
        attach_state->state = 0;
        attach_state->timer = scale_game_frames(*(int *)&follow_camera_hold_focus_exit_base_frames_gp);
        attach_state->invTimer = 1.0f / convert_integer_to_float(attach_state->timer);
    }
}
/* per-frame state machine that eases a moby's attach pose toward the camera block's vector */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E6498), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern int FUN_001f96f8_c2(int) __asm__("FUN_001f96f8");
extern int FUN_001f9770_c(void *) __asm__("FUN_001f9770");
extern void lerp_vector(float, void *, void *, void *) __asm__("FUN_001f9a40");
extern void FUN_001f9a68_c2(void *, void *, float) __asm__("FUN_001f9a68");
float absolute_float(float input) __asm__("func_001F99C0");

extern short follow_camera_ground_focus_angle_upper_bound_gp __asm__("D_L00_00161D0C") __attribute__((sda));
extern short follow_camera_ground_focus_angle_lower_bound_gp __asm__("D_L00_00161D10") __attribute__((sda));
extern short follow_camera_projected_focus_blend_fraction_gp __asm__("D_L00_00161D80") __attribute__((sda));
void FUN_L00_002e4fe8(char *camera, float *out_focus_velocity) {
    float hero_position[4];
    float target_position_or_up_offset[4];
    char *camera_state;
    char *attach_state;
    char *camera_view;
    struct Hero *pad;
    struct Hero *padb;
    struct Hero *padc;
    char *timer_camera_state;
    float x, y;
    int flag;
    camera_view = D_L00_00166E10;
    camera_state = *(char **)(camera + 0x70);
    follow_camera_hero_target(camera, hero_position);
    pad = &hero;
    attach_state = camera_state + 0x40;
    if (pad->state.current == 0x77) {
        x = fast_vec_dot(attach_state, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        fast_vec_sub(attach_state, attach_state, out_focus_velocity);
        y = fast_vec_dot(camera_view + 0x30, camera_state + 0xA0);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, y);
        qcopy(camera_state + 0x50, out_focus_velocity);
        if (*(unsigned char *)(attach_state + 0xC4) != 0xB) {
            *(unsigned char *)(attach_state + 0xC4) = 0xB;
            qcopy(camera + 0x40, camera);
            timer_camera_state = *(char **)(camera + 0x70);
            *(short *)(timer_camera_state + 0x20) = FUN_001f96f8_c2(*(int *)&D_L00_00161D24_gp);
        }
        return;
    }
    switch (*(unsigned char *)(attach_state + 0xC4)) {
    case CAM_ATTACH_GROUND_AWARE_FOCUS_VELOCITY:
        padb = &hero;
        if (padb->state.current == 0x11 ||
            (padb->past_jump_apex != 0 && padb->ground_distance > 0.2f &&
             ((y = compute_follow_camera_signed_target_angle(camera, *(float *)(attach_state + 0xB0))) < *(float *)&follow_camera_ground_focus_angle_lower_bound_gp ||
              *(float *)&follow_camera_ground_focus_angle_upper_bound_gp < y))) {
            int v;
            *(unsigned char *)(attach_state + 0xC4) = 1;
            v = *(unsigned short *)&D_L00_00161D3C_gp;
            *(short *)(attach_state + 0xC6) = v;
            *(float *)(attach_state + 0xC8) = 1.0f / convert_integer_to_float((short)v);
        }
        padc = &hero;
        flag = 0;
        if (padc->frames_off_ground.s != 0 && *(float *)(camera_view + 0xDC) == 0.0f &&
            (padc->past_jump_apex == 0 || padc->frames_to_land >= 0x10)) {
            flag = 1;
        } else {
            if (compute_follow_camera_ground_aware_target_position(target_position_or_up_offset, 1) == 1) {
                x = fast_vec_dot(target_position_or_up_offset, camera_view + 0x30);
                if (x < fast_vec_length(attach_state + 0x10))
                    flag = 1;
            } else {
                flag = 1;
            }
        }
        if (flag != 0) {
            if (absolute_float(*(float *)(camera_view + 0x30) - *(float *)(camera_view + 0x40)) > 0.001f ||
                absolute_float(*(float *)(camera_view + 0x34) - *(float *)(camera_view + 0x44)) > 0.001f ||
                absolute_float(*(float *)(camera_view + 0x38) - *(float *)(camera_view + 0x48)) > 0.001f) {
                x = fast_vec_dot(hero_position, camera_view + 0x30);
                FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
                fast_vec_sub(attach_state, hero_position, out_focus_velocity);
            } else {
                qcopy(out_focus_velocity, attach_state + 0x10);
                x = fast_vec_dot(hero_position, camera_view + 0x30);
                FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
                fast_vec_sub(attach_state, hero_position, target_position_or_up_offset);
            }
        } else {
            compute_follow_camera_ground_aware_target_position(target_position_or_up_offset, 1);
            x = fast_vec_dot(target_position_or_up_offset, camera_view + 0x30);
            FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
            fast_vec_sub(attach_state, target_position_or_up_offset, out_focus_velocity);
        }
        break;
    case CAM_ATTACH_BLEND_TO_PROJECT_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_FOCUS_VELOCITY_WITH_HOLD_TRANSITION:
    case CAM_ATTACH_BLEND_OUT_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_BACK_FOCUS_VELOCITY:
        lerp_vector((float)*(short *)(attach_state + 0xD4) * *(float *)(attach_state + 0xCC), attach_state + 0x10, attach_state + 0x30,
                     attach_state + 0x20);
        if (*(unsigned char *)(attach_state + 0xD7) == 0) {
            if (coll_sphere(0.2f, camera + 0x30, 0x12, 0)) {
                qcopy(attach_state + 0x30, attach_state + 0x10);
                *(unsigned char *)(attach_state + 0xD7) = 1;
            }
        }
        qcopy(out_focus_velocity, attach_state + 0x10);
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, target_position_or_up_offset);
        if (FUN_001f9770_c(attach_state + 0xD4)) {
            if (*(unsigned char *)(attach_state + 0xC4) == 9) {
                *(unsigned char *)(attach_state + 0xC4) = 0xA;
                *(short *)(attach_state + 0xD4) = FUN_001f96f8_c2(*(int *)&D_L00_00161D6C_gp);
                qcopy(attach_state + 0x20, attach_state + 0x10);
                FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, *(float *)(attach_state + 0xD0));
                fast_vec_sub(attach_state + 0x30, attach_state + 0x20, target_position_or_up_offset);
            } else if (*(unsigned char *)(attach_state + 0xC4) == 6) {
                *(unsigned char *)(attach_state + 0xC4) = 7;
                *(short *)(attach_state + 0xD4) = FUN_001f96f8_c2(*(int *)&D_L00_00161D40_gp);
                qcopy(attach_state + 0x30, attach_state + 0x20);
                qcopy(attach_state + 0x20, attach_state + 0x10);
            } else {
                int v;
                *(unsigned char *)(attach_state + 0xC4) = 4;
                v = *(unsigned short *)&D_L00_00161D54_gp;
                *(short *)(attach_state + 0xC6) = v;
                *(float *)(attach_state + 0xC8) = 1.0f / convert_integer_to_float((short)v);
            }
        }
        break;
    case CAM_ATTACH_HOLD_FOCUS_VELOCITY:
        qcopy(out_focus_velocity, attach_state + 0x10);
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(target_position_or_up_offset, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, target_position_or_up_offset);
        break;
    case CAM_ATTACH_BLEND_PROJECTED_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_CONTINUE_BLEND_PROJECTED_HERO_FOCUS_VELOCITY:
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, out_focus_velocity);
        lerp_vector(*(float *)&follow_camera_projected_focus_blend_fraction_gp, out_focus_velocity, out_focus_velocity, attach_state + 0x20);
        break;
    case CAM_ATTACH_PROJECT_HERO_FOCUS_VELOCITY:
    default:
        x = fast_vec_dot(hero_position, camera_view + 0x30);
        FUN_001f9a68_c2(out_focus_velocity, camera_view + 0x30, x);
        fast_vec_sub(attach_state, hero_position, out_focus_velocity);
        break;
    }
}
f32 cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
void interpolate_follow_camera_position(void *mm, void *vv, void *tt, f32 stiffness, f32 damping, f32 limit) __asm__("FUN_L00_002e55b8");

void interpolate_follow_camera_position(void *mm, void *vv, void *tt, f32 stiffness, f32 damping, f32 limit) {
    u8 *camera_state = *(u8 **)((u8 *)mm + 0x70);
    f32 *current_values = vv;
    f32 *target_values = tt;
    current_values[0] = cam_interp_values(camera_state + 0xE0, current_values[0], target_values[0], stiffness, damping, limit);
    current_values[1] = cam_interp_values(camera_state + 0xE4, current_values[1], target_values[1], stiffness, damping, limit);
    current_values[2] = cam_interp_values(camera_state + 0xE8, current_values[2], target_values[2], stiffness, damping, limit);
}
s32 compute_follow_camera_target_position(void *mm, void *input_offset, void *out_target_position, s32 k) __asm__("FUN_L00_002e5678");

s32 compute_follow_camera_target_position(void *mm, void *input_offset, void *out_target_position, s32 k) {
    u8 *camera = mm;
    s32 attach_mode_return_flag = 1;
    u8 *stored_focus_position = *(u8 **)(camera + 0x70) + 0x40;
    f32 hero_base_position[4] __attribute__((aligned(16)));
    switch (k) {
    case CAM_ATTACH_BLEND_TO_PROJECT_HERO_FOCUS_VELOCITY:
    case CAM_ATTACH_PROJECT_HERO_FOCUS_VELOCITY:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        attach_mode_return_flag = 0;
        break;
    case CAM_ATTACH_BLEND_FOCUS_VELOCITY_WITH_HOLD_TRANSITION:
    case CAM_ATTACH_BLEND_OUT_FOCUS_VELOCITY:
    case CAM_ATTACH_BLEND_BACK_FOCUS_VELOCITY:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        attach_mode_return_flag = 0;
        break;
    case CAM_ATTACH_HERO_POSITION_BASE:
        follow_camera_hero_target((int)camera, hero_base_position);
        fast_vec_add(out_target_position, input_offset, hero_base_position);
        break;
    case CAM_ATTACH_GROUND_AWARE_FOCUS_VELOCITY:
    default:
        fast_vec_add(out_target_position, input_offset, stored_focus_position);
        break;
    }
    return attach_mode_return_flag;
}
typedef struct {
    f32 v[4];
} V002e5730 __attribute__((aligned(16)));
typedef struct {
    u8 p0[0x90];
    u8 x90[0x34];
    u8 xc4;
    u8 xc5;
    s16 xc6;
    f32 xc8;
} A002e5730;
typedef struct {
    u8 p[0x70];
    u8 *m;
} O002e5730;
extern f32 follow_camera_position_default_stiffness_gp __asm__("D_L00_00161D28") __attribute__((sda));
extern f32 follow_camera_position_default_damping_gp __asm__("D_L00_00161D2C") __attribute__((sda));
extern f32 follow_camera_position_transition_end_stiffness_gp __asm__("D_L00_00161D30") __attribute__((sda));
extern f32 follow_camera_position_transition_end_damping_gp __asm__("D_L00_00161D34") __attribute__((sda));
s32 fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
s32 compute_follow_camera_target_position(void *, void *, void *, s32) __asm__("FUN_L00_002e5678");
void interpolate_follow_camera_position(void *, void *, void *, f32, f32, f32) __asm__("FUN_L00_002e55b8");
void follow_camera_transition(O002e5730 *o, void *a) __asm__("FUN_L00_002e5730");
void follow_camera_transition(O002e5730 *camera, void *input_offset) {
    V002e5730 target_position;
    u8 *m = camera->m;
    A002e5730 *attach_state = (A002e5730 *)(m + 0x40);
    f32 k, x, y;
    u8 attach_mode;
    if (fast_dec_timer_s16(m + 0x106))
        attach_state->xc5 = attach_state->xc4;
    k = (f32)attach_state->xc6 * attach_state->xc8;
    compute_follow_camera_target_position(camera, input_offset, &target_position, attach_state->xc4);
    attach_mode = attach_state->xc4;
    if (attach_mode == 3 || attach_mode == 5 || attach_mode == 6 || attach_mode == 7 || attach_mode == 1) {
        x = follow_camera_position_transition_end_stiffness_gp + (follow_camera_position_default_stiffness_gp - follow_camera_position_transition_end_stiffness_gp) * k;
        y = follow_camera_position_transition_end_damping_gp + (follow_camera_position_default_damping_gp - follow_camera_position_transition_end_damping_gp) * k;
    } else {
        x = follow_camera_position_default_stiffness_gp;
        y = follow_camera_position_default_damping_gp;
    }
    interpolate_follow_camera_position(camera, attach_state->x90, &target_position, x, y, 0.0f);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E6CE0); names and data translated to the US level program. */

#include "rnc/gameplay/camera/follow_cam.h"

/* The camera view: +0x20 its forward axis, +0x30 its up axis. */
extern u8 camera_view[] __asm__("D_L00_00166E10");
extern f32 follow_cam_min_distance_gp __asm__("D_L00_00161CD8") __attribute__((sda));
extern s32 follow_cam_blend_frames __asm__("D_L00_00161D20");
extern s32 camera_collision_mask __asm__("D_L00_0015EF58");
/* Point where the last collision line stopped (CollisionHit.point). */
extern f32 collision_hit_point[4] __asm__("D_L00_00173E60") __attribute__((aligned(16)));

extern f32 fast_vec_length(void *v) __asm__("FUN_001f9af0");
extern void fast_vec_add(void *out, void *a, void *b) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void lerp_vector(f32 t, void *out, void *a, void *b) __asm__("FUN_001f9a40");
extern f32 convert_integer_to_float(s32 i) __asm__("FUN_001fa6c0");
extern s32 fast_dec_timer(void *timer) __asm__("FUN_001f9770");
extern f32 cam_interp_values(void *state, f32 value, f32 target, f32 stiffness, f32 damping, f32 limit) __asm__("FUN_001ebd78");
extern s32 collision_line(void *from, void *to, s32 mask, s32 ignore, s32 flags) __asm__("FUN_001efa68");
extern void follow_cam_update_mode(void *cam) __asm__("FUN_L00_002e4930");
extern void follow_cam_focus_velocity(void *cam, f32 *out) __asm__("FUN_L00_002e4fe8");
extern void follow_camera_set_height(f32 amount, f32 rate) __asm__("FUN_L00_002e84b8");

/* Updates the follow camera: eases its focus and offset vectors, then places
   the eye behind the hero, pulled in where collision lines hit. */
void follow_camera_springs(void *cam) __asm__("FUN_L00_002e5830");
void follow_camera_springs(void *camera_moby) {
    f32 velocity[4], hero_pos[4], focus[4], up[4], lateral[4], look[4], base[4], probe[4], left[4], collision_resolution_delta[4], mid[4];
    u8 *view = camera_view;
    FollowCamState *camera_state = FOLLOW_CAM_STATE((struct UpdateCam *)camera_moby);
    FollowCamTail *camera_tail = &camera_state->tail;
    FollowCamBlock *follow_block = &camera_state->follow;
    f32 up_projection_or_collision_clearance, back;

    follow_camera_hero_target(camera_moby, hero_pos);
    follow_cam_update_mode(camera_moby);
    follow_cam_focus_velocity(camera_moby, velocity);
    qcopy(&camera_state->follow.velocity, velocity);
    qcopy(focus, follow_block);
    up_projection_or_collision_clearance = fast_vec_dot(view + 0x30, camera_state->follow.offset);
    scale_vector_xyz(up, view + 0x30, up_projection_or_collision_clearance);
    fast_vec_sub(lateral, camera_state->follow.offset, up);
    up[0] = cam_interp_values(&camera_state->follow.up_ease[0], up[0], follow_block->velocity[0], follow_block->up_stiffness, follow_block->up_damping, 0.0f);
    up[1] = cam_interp_values(&camera_state->follow.up_ease[1], up[1], follow_block->velocity[1], follow_block->up_stiffness, follow_block->up_damping, 0.0f);
    up[2] = cam_interp_values(&camera_state->follow.up_ease[2], up[2], follow_block->velocity[2], follow_block->up_stiffness, follow_block->up_damping, 0.0f);
    lateral[0] = cam_interp_values(&camera_state->follow.lateral_ease[0], lateral[0], follow_block->focus[0], follow_block->lateral_stiffness, follow_block->lateral_damping, 0.0f);
    lateral[1] = cam_interp_values(&camera_state->follow.lateral_ease[1], lateral[1], follow_block->focus[1], follow_block->lateral_stiffness, follow_block->lateral_damping, 0.0f);
    lateral[2] = cam_interp_values(&camera_state->follow.lateral_ease[2], lateral[2], follow_block->focus[2], follow_block->lateral_stiffness, follow_block->lateral_damping, 0.0f);
    fast_vec_add(follow_block, follow_block, camera_state->follow.velocity);
    fast_vec_add(camera_state->follow.offset, lateral, up);
    scale_vector_xyz(look, view + 0x20, follow_block->look_height);
    if (follow_block->blending_in) {
        if (++follow_block->blend_timer > scale_game_frames(follow_cam_blend_frames)) {
            follow_block->blend_timer = scale_game_frames(follow_cam_blend_frames);
        }
        lerp_vector((f32)follow_block->blend_timer / convert_integer_to_float(scale_game_frames(follow_cam_blend_frames)), base, follow_block, camera_state->follow.offset);
        fast_vec_add(camera_state->follow.look_target, look, base);
    } else {
        fast_dec_timer(&camera_state->follow.blend_timer);
        if (follow_block->blend_timer < 0) {
            follow_block->blend_timer = 0;
        }
        lerp_vector((f32)follow_block->blend_timer / convert_integer_to_float(scale_game_frames(follow_cam_blend_frames)), base, follow_block, camera_state->follow.offset);
        fast_vec_add(camera_state->follow.look_target, look, base);
    }
    follow_camera_transition(camera_moby, look);
    if (hero.vehicle_mode == 2) {
        scale_vector_xyz(look, view + 0x20, 3.0f);
    } else if (hero.vehicle_mode == 1) {
        scale_vector_xyz(look, view + 0x20, 0.4f);
    } else {
        scale_vector_xyz(look, view + 0x20, 0.5f);
    }
    fast_vec_add(base, hero_pos, look);
    scale_vector_xyz(look, view + 0x20, camera_tail->distance);
    fast_vec_add(follow_block->eye, look, follow_block);
    up_projection_or_collision_clearance = follow_cam_min_distance_gp + FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom_range * FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom_scale * (FOLLOW_CAM_STATE(current_follow_cam)->tail.zoom - 1.0f);
    scale_vector_xyz(probe, view + 0x20, 20.0f);
    fast_vec_add(probe, base, probe);
    if (collision_line(base, probe, camera_collision_mask, (s32)hero.moby, 0)) {
        qcopy(left, collision_hit_point);
    } else {
        qcopy(left, probe);
    }
    scale_vector_xyz(probe, view + 0x20, 20.0f);
    fast_vec_sub(probe, base, probe);
    if (collision_line(base, probe, camera_collision_mask, (s32)hero.moby, 0)) {
        qcopy(probe, collision_hit_point);
    }
    fast_vec_sub(collision_resolution_delta, probe, left);
    if (fast_vec_length(collision_resolution_delta) < up_projection_or_collision_clearance + up_projection_or_collision_clearance) {
        fast_vec_add(mid, left, probe);
        scale_vector_xyz(mid, mid, 0.5f);
        qcopy(follow_block->eye, mid);
        fast_vec_sub(collision_resolution_delta, mid, follow_block);
        follow_camera_set_height(fast_vec_dot(collision_resolution_delta, view + 0x20), 0.003f);
    } else {
        f32 reach;
        fast_vec_sub(collision_resolution_delta, left, follow_block);
        back = fast_vec_length(collision_resolution_delta);
        reach = camera_tail->distance;
        if (back <= reach + up_projection_or_collision_clearance) {
            scale_vector_xyz(mid, view + 0x20, up_projection_or_collision_clearance);
            fast_vec_sub(mid, left, mid);
            qcopy(follow_block->eye, mid);
            fast_vec_sub(collision_resolution_delta, mid, follow_block);
            follow_camera_set_height(fast_vec_dot(collision_resolution_delta, view + 0x20), 0.003f);
        } else {
            up_projection_or_collision_clearance = fast_vec_dot(follow_block, view + 0x30);
            if (fast_vec_dot(probe, view + 0x30) < up_projection_or_collision_clearance) {
                qcopy(probe, follow_block);
            }
            scale_vector_xyz(look, view + 0x20, reach);
            fast_vec_add(follow_block->eye, look, probe);
        }
    }
}
/* Empty apart from an unused 0x50-byte volatile stack buffer. */
void run_follow_camera_noop(void) __asm__("FUN_L00_002e8378");

void run_follow_camera_noop(void) {
    volatile char unused_stack_reservation[0x50];
}
struct P2_388 {
    char pad[0x220];
    s32 cur;
};
struct P1_388 {
    char pad0[0x70];
    struct P2_388 *p;
    char pad1[0x12];
    s16 f86;
};
extern struct P1_388 *D_L00_00166E00_c __asm__("D_L00_00166E00");
/* -1 while D_L00_00166E00+0x86 is set; else clears its current id, 1 if it was id. */
s32 clear_follow_camera_volume_if_current(s32 id) __asm__("FUN_L00_002e8388");
s32 clear_follow_camera_volume_if_current(s32 id) {
    struct P1_388 *current_camera = D_L00_00166E00_c;
    if (current_camera->f86 != 0) {
        return -1;
    }
    if (current_camera->p->cur == id) {
        current_camera->p->cur = 0;
        return 1;
    }
    return 0;
}
extern void *D_L00_00166E00_c2 __asm__("D_L00_00166E00");
extern u32 D_L00_0015EF50;
typedef struct {
    char pad00[0x86];
    short f86;
} B842e83c0;
typedef struct {
    char pad00[0x70];
    u32 f70;
} B2842e83c0;
typedef struct {
    char pad00[0x220];
    u32 f220;
} C842e83c0;
typedef struct {
    char pad00[0x84];
    short f84;
} O842e83c0;
int try_select_follow_camera_volume(int candidate_object, int unused) __asm__("FUN_L00_002e83c0");
int try_select_follow_camera_volume(int candidate_object, int unused) {
    void *current_camera = D_L00_00166E00_c2;
    C842e83c0 *camera_state;
    u8 x;
    u8 y;
    u32 tab;
    if (((B842e83c0 *)current_camera)->f86 != 0) {
        return -1;
    }
    camera_state = (C842e83c0 *)(u32)((B2842e83c0 *)current_camera)->f70;
    if (camera_state->f220 == 0) {
        camera_state->f220 = candidate_object;
        return 1;
    }
    if (camera_state->f220 == candidate_object) {
        return 1;
    }
    tab = (u32)D_L00_0015EF50;
    x = *(u8 *)(*(u32 *)(tab + ((O842e83c0 *)(u32)camera_state->f220)->f84 * 0x20 + 0x1C) + 0x1C);
    y = *(u8 *)(*(u32 *)(tab + ((O842e83c0 *)(u32)candidate_object)->f84 * 0x20 + 0x1C) + 0x1C);
    if (x < y) {
        camera_state->f220 = candidate_object;
        return 1;
    }
    return 0;
}
extern u8 *D_L00_00166E00_lui __asm__("D_L00_00166E00") NOT_SDA;
/* Sets a flag and floats a/b at +0x130 of D_L00_00166E00+0x70; f also stores a at +0x58. */
void follow_camera_set_distance(s32 f, f32 a, f32 b) __asm__("FUN_L00_002e8450");
void follow_camera_set_distance(s32 copy_distance_to_secondary_slot, f32 distance, f32 distance_override_companion_value) {
    u8 *current_camera = D_L00_00166E00_lui;
    if (*(s16 *)(current_camera + 0x86) == 0) {
        u8 *camera_distance_state = *(u8 **)(current_camera + 0x70) + 0x130;
        *(s16 *)(camera_distance_state + 0x3C) = 1;
        *(f32 *)(camera_distance_state + 0x48) = distance_override_companion_value;
        *(f32 *)(camera_distance_state + 0x40) = distance;
        if (copy_distance_to_secondary_slot)
            *(f32 *)(camera_distance_state + 0x58) = distance;
    }
}
extern char *D_L00_00166E00_002e8490 __asm__("D_L00_00166E00");
void FUN_L00_002e8490(float camera_state_value) {
    char *current_camera = D_L00_00166E00_002e8490;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x22C) = camera_state_value;
    }
}
extern char *D_L00_00166E00_002e84b8 __asm__("D_L00_00166E00") NOT_SDA;
/* Sets a flag and two floats at +0x130 of D_L00_00166E00+0x70 while its short at 0x86 is 0. */
void follow_camera_set_height(float a, float b) __asm__("FUN_L00_002e84b8");
void follow_camera_set_height(float height, float height_override_companion_value) {
    char *current_camera = D_L00_00166E00_002e84b8;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *camera_height_state = *(char **)(current_camera + 0x70) + 0x130;
        *(short *)(camera_height_state + 0x3e) = 1;
        *(float *)(camera_height_state + 0x4c) = height;
        *(float *)(camera_height_state + 0x54) = height_override_companion_value;
    }
}
extern char *D_L00_00166E00_c3 __asm__("D_L00_00166E00") NOT_SDA;
extern char *D_L00_00168FC0 NOT_SDA;
/* Sets a flag and floats +0xF4/+0xF8 of D_L00_00166E00+0x70; add offsets a by 00168FC0+0xF0. */
void follow_camera_set_offset_scale(int add, float a, float b) __asm__("FUN_L00_002e84f0");
void follow_camera_set_offset_scale(int add, float offset_scale, float offset_scale_companion_value) {
    char *current_camera = D_L00_00166E00_c3;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *attach_state = *(char **)(current_camera + 0x70) + 0x40;
        *(short *)(attach_state + 0xDA) = 1;
        *(float *)(attach_state + 0xB8) = offset_scale_companion_value;
        if (add) {
            *(float *)(attach_state + 0xB4) = *(float *)(D_L00_00168FC0 + 0xF0) + offset_scale;
        } else {
            *(float *)(attach_state + 0xB4) = offset_scale;
        }
    }
}
typedef struct {
    char pad[0x10];
    short h;
} S2e8568b;
typedef struct {
    char pad[0x70];
    S2e8568b *p;
    char pad2[0x12];
    short f86;
} S2e8568;
extern S2e8568 *D_L00_00166E00;
/* Stores v through the pointer in D_L00_00166E00 while its short at 0x86 is 0. */
void set_follow_camera_orbit_clamp_mode(int camera_state_short_value) __asm__("FUN_L00_002e8568");
void set_follow_camera_orbit_clamp_mode(int camera_state_short_value) {
    S2e8568 *current_camera = D_L00_00166E00;
    if (current_camera->f86 == 0)
        current_camera->p->h = camera_state_short_value;
}
extern char *D_L00_00166E00_002e8590[] __asm__("D_L00_00166E00");
/* Stores non-zero a/b at +0x11C/+0x120 of D_L00_00166E00+0x70 while its short at 0x86 is 0. */
void follow_camera_set_lateral_spring(float a, float b) __asm__("FUN_L00_002e8590");
void follow_camera_set_lateral_spring(float lateral_spring_value, float lateral_spring_companion_value) {
    char *current_camera = D_L00_00166E00_002e8590[0];
    if (*(short *)(current_camera + 0x86) == 0) {
        char *attach_state = *(char **)(current_camera + 0x70) + 0x40;
        if (lateral_spring_value != 0.0f)
            *(float *)(attach_state + 0xDC) = lateral_spring_value;
        if (lateral_spring_companion_value != 0.0f)
            *(float *)(attach_state + 0xE0) = lateral_spring_companion_value;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E9AD0), where it is exact; names translated to the US level program. */

extern char *D_L00_00166E00_002e8620 __asm__("D_L00_00166E00") NOT_SDA;

/* Sets the short at +0x226 of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
void FUN_L00_002e8620(void);
void FUN_L00_002e8620(void) {
    char *current_camera = D_L00_00166E00_002e8620;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(short *)(*(char **)(current_camera + 0x70) + 0x226) = 1;
    }
}
extern u8 *vd_2e8648 __asm__("D_L00_00166E00") NOT_SDA;
/* Sets the byte at +0x116 of D_L00_00166E00+0x70 while the short at 0x86 is 0. */
void follow_camera_start_slide(void) __asm__("FUN_L00_002e8648");
void follow_camera_start_slide(void) {
    u8 *current_camera = vd_2e8648;
    if (*(s16 *)(current_camera + 0x86) == 0) {
        *(u8 *)(*(u8 **)(current_camera + 0x70) + 0x116) = 1;
    }
}
extern char *D_L00_00166E00_002e8680 __asm__("D_L00_00166E00") NOT_SDA;
void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");

void set_follow_camera_blend_output_flags(void) {
    char *current_camera = D_L00_00166E00_002e8680;
    if (*(short *)(current_camera + 0x86) == 0) {
        char *blend_output_block = *(char **)(current_camera + 0x70) + 0x1a8;
        *(int *)(blend_output_block + 0x10) |= 3;
    }
}
/* Measures the signed angle between two offsets of an object's data relative to an axis, and (within the limit) stores a scaled blend in the object's data. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002E1660.c: func_L00_002E9B60), where it is exact; names translated to the US level program. */

extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float cubic_hermite_scalar(float a, float b, float c, float d, float t) __asm__("FUN_L00_00257e20");
extern void FUN_001f9a68_c4(void *, void *, float) __asm__("FUN_001f9a68");

float update_follow_camera_angle_blend_from_direction(char *camera, float *input_direction, float blend_update_angle_limit) __asm__("FUN_L00_002e86b0");

float update_follow_camera_angle_blend_from_direction(char *camera, float *input_direction, float blend_update_angle_limit) {
    float axis_projection[4];
    float planar_input_direction[4];
    float planar_reference_direction[4];
    float input_cross_axis[4];
    char *base = D_L00_00166E10;
    char *axis = base + 0x20;
    char *camera_state = *(char **)(camera + 0x70);
    char *blend_output_block = camera_state + 0x1A8;
    char *reference_direction = camera_state + 0x130;
    float dot;
    float planar_input_length_or_length_product;
    float planar_separation_angle;
    float sign;
    float angle_delta = 0.0f;
    float z2;
    float angle_blend_fraction;

    FUN_001f9a68_c4(axis_projection, axis, fast_vec_dot(input_direction, axis));
    fast_vec_sub(planar_input_direction, input_direction, axis_projection);
    FUN_001f9a68_c4(axis_projection, axis, fast_vec_dot(reference_direction, axis));
    fast_vec_sub(planar_reference_direction, (float *)reference_direction, axis_projection);
    dot = fast_vec_dot(planar_input_direction, planar_reference_direction);
    planar_input_length_or_length_product = fast_vec_length(planar_input_direction);
    if (planar_input_length_or_length_product == angle_delta) {
        return angle_delta;
    }
    z2 = 0.0f;
    planar_input_length_or_length_product = planar_input_length_or_length_product * fast_vec_length(planar_reference_direction);
    if (planar_input_length_or_length_product == angle_delta) {
        return angle_delta;
    }
    planar_separation_angle = 1.5707964f - fast_arc_sin(dot / planar_input_length_or_length_product);
    sign = -1.0f;
    fast_vec_cross(input_cross_axis, planar_input_direction, axis);
    if (fast_vec_dot(input_cross_axis, planar_reference_direction) >= 0.0f) {
        sign = 1.0f;
    }
    angle_delta = fast_subtract_rotations(3.1415927f, planar_separation_angle);
    if (blend_update_angle_limit != z2 && blend_update_angle_limit < absolute_float(angle_delta)) {
        return angle_delta;
    }
    angle_blend_fraction = angle_delta / 1.5707964f;
    if (angle_blend_fraction > 1.0f) {
        angle_blend_fraction = 1.0f;
    }
    *(float *)(blend_output_block + 0x1C) = cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, angle_blend_fraction) * sign;
    return angle_delta;
}
void FUN_001f9a28_c2(void *, void *, void *) __asm__("FUN_001f9a28");
/* Passes b minus the vector at +0x40 of p's object, with x, to FUN_L00_002e86b0. */
float update_follow_camera_angle_blend_from_position(char *camera, void *target_position, f32 x) __asm__("FUN_L00_002e88c8");

float update_follow_camera_angle_blend_from_position(char *camera, void *target_position, f32 x) {
    float focus_to_target_direction[4];
    FUN_001f9a28_c2(focus_to_target_direction, target_position, *(char **)(camera + 0x70) + 0x40);
    return update_follow_camera_angle_blend_from_direction(camera, focus_to_target_direction, x);
}
extern char *D_L00_00166E00_002e8918 __asm__("D_L00_00166E00") NOT_SDA;
/* For non-zero x: stores x at +0x1BC of D_L00_00166E00+0x70 and calls FUN_L00_002e86b0. */
void set_follow_camera_angle_blend_from_direction(char *input_direction, float x, float y) __asm__("FUN_L00_002e8918");

void set_follow_camera_angle_blend_from_direction(char *input_direction, float x, float y) {
    char *current_camera = D_L00_00166E00_002e8918;
    if (*(short *)(current_camera + 0x86) == 0 && x != 0.0f) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x1bc) = x;
        update_follow_camera_angle_blend_from_direction(current_camera, input_direction, y);
    }
}
extern char *D_L00_00166E00_002e8970 __asm__("D_L00_00166E00") NOT_SDA;
/* Stores x at +0x1BC of D_L00_00166E00+0x70 and calls FUN_L00_002e88c8 with a and y. */
void set_follow_camera_angle_blend_from_position(char *target_position, float x, float y) __asm__("FUN_L00_002e8970");

void set_follow_camera_angle_blend_from_position(char *a, float x, float y) __asm__("FUN_L00_002e8970");
void set_follow_camera_angle_blend_from_position(char *target_position, float x, float y) {
    char *target_position_arg = target_position;
    char *current_camera = D_L00_00166E00_002e8970;
    if (*(short *)(current_camera + 0x86) == 0) {
        *(float *)(*(char **)(current_camera + 0x70) + 0x1bc) = x;
        update_follow_camera_angle_blend_from_position(current_camera, target_position_arg, y);
    }
}

f32 D_L01_0016736C[1] __attribute__((section(".data"))) = {0};
f32 D_L02_001674EC[1] __attribute__((section(".data"))) = {0};
f32 D_L03_00166FEC[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0016706C[1] __attribute__((section(".data"))) = {0};
f32 D_L05_001672EC[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0016766C[1] __attribute__((section(".data"))) = {0};
f32 D_L07_00166F6C[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0016706C[1] __attribute__((section(".data"))) = {0};
f32 D_L10_0016736C[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0016736C[1] __attribute__((section(".data"))) = {0};
f32 D_L13_001671EC[1] __attribute__((section(".data"))) = {0};
f32 D_L14_001675EC[1] __attribute__((section(".data"))) = {0};
f32 D_L15_001674EC[1] __attribute__((section(".data"))) = {0};
f32 D_L16_001672EC[1] __attribute__((section(".data"))) = {0};

float D_L01_00161FF0 = 45.0f;
float D_L01_00162040 = 1e+02f;
short D_L01_001621B0 = 0x1e;
short D_L01_001621B4 = 0xa;
short D_L01_001621B8 = 0x23;
short D_L01_001621C0 = 0;
short D_L01_001621C4 = 0x55;
short D_L01_001621C8 = 0x1e;
short D_L01_001621CC = 0x14;
short D_L01_001621D0 = 0;
short D_L01_001621D4 = 0x2d;
short D_L01_001621D8 = 0x1e;
short D_L01_001621DC = 0x14;
short D_L01_001621E0 = 0;
short D_L01_001621E4 = 0x1e;
short D_L01_001621F0 = 0x1e;
short D_L01_001621F4 = 0;
short D_L01_00162184 = 0xfdb;
short D_L01_00162188 = -10486;
short D_L01_001621F8 = 0;
f32 D_L01_001621A0 = 0.01f;
f32 D_L01_001621A4 = 0.2f;
f32 D_L01_001621A8 = 0.06f;
f32 D_L01_001621AC = 0.4f;
s32 D_L01_00162198 = 0x5a;
float D_L02_00161F48 = 45.0f;
float D_L02_00162030 = 1e+02f;
short D_L02_001622E8 = 0x1e;
short D_L02_001622EC = 0xa;
short D_L02_001622F0 = 0x23;
short D_L02_001622F8 = 0;
short D_L02_001622FC = 0x55;
short D_L02_00162300 = 0x1e;
short D_L02_00162304 = 0x14;
short D_L02_00162308 = 0;
short D_L02_0016230C = 0x2d;
short D_L02_00162310 = 0x1e;
short D_L02_00162314 = 0x14;
short D_L02_00162318 = 0;
short D_L02_0016231C = 0x1e;
short D_L02_00162328 = 0x1e;
short D_L02_0016232C = 0;
short D_L02_001622BC = 0xfdb;
short D_L02_001622C0 = -10486;
short D_L02_00162330 = 0;
f32 D_L02_001622D8 = 0.01f;
f32 D_L02_001622DC = 0.2f;
f32 D_L02_001622E0 = 0.06f;
f32 D_L02_001622E4 = 0.4f;
s32 D_L02_001622D0 = 0x5a;
float D_L03_00161C68 = 45.0f;
float D_L03_00161C6C = 1e+02f;
short D_L03_00161DB0 = 0x1e;
short D_L03_00161DB4 = 0xa;
short D_L03_00161DB8 = 0x23;
short D_L03_00161DC0 = 0;
short D_L03_00161DC4 = 0x55;
short D_L03_00161DC8 = 0x1e;
short D_L03_00161DCC = 0x14;
short D_L03_00161DD0 = 0;
short D_L03_00161DD4 = 0x2d;
short D_L03_00161DD8 = 0x1e;
short D_L03_00161DDC = 0x14;
short D_L03_00161DE0 = 0;
short D_L03_00161DE4 = 0x1e;
short D_L03_00161DF0 = 0x1e;
short D_L03_00161DF4 = 0;
short D_L03_00161D84 = 0xfdb;
short D_L03_00161D88 = -10486;
short D_L03_00161DF8 = 0;
f32 D_L03_00161DA0 = 0.01f;
f32 D_L03_00161DA4 = 0.2f;
f32 D_L03_00161DA8 = 0.06f;
f32 D_L03_00161DAC = 0.4f;
s32 D_L03_00161D98 = 0x5a;
float D_L04_00161D58 = 45.0f;
float D_L04_00161D5C = 1e+02f;
short D_L04_00161EA8 = 0x1e;
short D_L04_00161EAC = 0xa;
short D_L04_00161EB0 = 0x23;
short D_L04_00161EB8 = 0;
short D_L04_00161EBC = 0x55;
short D_L04_00161EC0 = 0x1e;
short D_L04_00161EC4 = 0x14;
short D_L04_00161EC8 = 0;
short D_L04_00161ECC = 0x2d;
short D_L04_00161ED0 = 0x1e;
short D_L04_00161ED4 = 0x14;
short D_L04_00161ED8 = 0;
short D_L04_00161EDC = 0x1e;
short D_L04_00161EE8 = 0x1e;
short D_L04_00161EEC = 0;
short D_L04_00161E7C = 0xfdb;
short D_L04_00161E80 = -10486;
short D_L04_00161EF0 = 0;
f32 D_L04_00161E98 = 0.01f;
f32 D_L04_00161E9C = 0.2f;
f32 D_L04_00161EA0 = 0.06f;
f32 D_L04_00161EA4 = 0.4f;
s32 D_L04_00161E90 = 0x5a;
float D_L05_00161FA8 = 45.0f;
float D_L05_00161FAC = 1e+02f;
short D_L05_00162100 = 0x1e;
short D_L05_00162104 = 0xa;
short D_L05_00162108 = 0x23;
short D_L05_00162110 = 0;
short D_L05_00162114 = 0x55;
short D_L05_00162118 = 0x1e;
short D_L05_0016211C = 0x14;
short D_L05_00162120 = 0;
short D_L05_00162124 = 0x2d;
short D_L05_00162128 = 0x1e;
short D_L05_0016212C = 0x14;
short D_L05_00162130 = 0;
short D_L05_00162134 = 0x1e;
short D_L05_00162140 = 0x1e;
short D_L05_00162144 = 0;
short D_L05_001620D4 = 0xfdb;
short D_L05_001620D8 = -10486;
short D_L05_00162148 = 0;
f32 D_L05_001620F0 = 0.01f;
f32 D_L05_001620F4 = 0.2f;
f32 D_L05_001620F8 = 0.06f;
f32 D_L05_001620FC = 0.4f;
s32 D_L05_001620E8 = 0x5a;
float D_L06_001622B8 = 45.0f;
float D_L06_001622BC = 1e+02f;
short D_L06_001624C8 = 0x1e;
short D_L06_001624CC = 0xa;
short D_L06_001624D0 = 0x23;
short D_L06_001624D8 = 0;
short D_L06_001624DC = 0x55;
short D_L06_001624E0 = 0x1e;
short D_L06_001624E4 = 0x14;
short D_L06_001624E8 = 0;
short D_L06_001624EC = 0x2d;
short D_L06_001624F0 = 0x1e;
short D_L06_001624F4 = 0x14;
short D_L06_001624F8 = 0;
short D_L06_001624FC = 0x1e;
short D_L06_00162508 = 0x1e;
short D_L06_0016250C = 0;
short D_L06_0016249C = 0xfdb;
short D_L06_001624A0 = -10486;
short D_L06_00162510 = 0;
f32 D_L06_001624B8 = 0.01f;
f32 D_L06_001624BC = 0.2f;
f32 D_L06_001624C0 = 0.06f;
f32 D_L06_001624C4 = 0.4f;
s32 D_L06_001624B0 = 0x5a;
float D_L07_00161CB0 = 45.0f;
float D_L07_00161CB4 = 1e+02f;
float D_L07_00161CD0 = 6.0f;
float D_L07_00161CD4 = 2.0f;
short D_L07_00161D80 = 0x1e;
short D_L07_00161D84 = 0xa;
short D_L07_00161D88 = 0x23;
short D_L07_00161D90 = 0;
short D_L07_00161D94 = 0x55;
short D_L07_00161D98 = 0x1e;
short D_L07_00161D9C = 0x14;
short D_L07_00161DA0 = 0;
short D_L07_00161DA4 = 0x2d;
short D_L07_00161DA8 = 0x1e;
short D_L07_00161DAC = 0x14;
short D_L07_00161DB0 = 0;
short D_L07_00161DB4 = 0x1e;
short D_L07_00161DC0 = 0x1e;
short D_L07_00161DC4 = 0;
short D_L07_00161D54 = 0xfdb;
short D_L07_00161D58 = -10486;
short D_L07_00161DC8 = 0;
f32 D_L07_00161D70 = 0.01f;
f32 D_L07_00161D74 = 0.2f;
f32 D_L07_00161D78 = 0.06f;
f32 D_L07_00161D7C = 0.4f;
s32 D_L07_00161D68 = 0x5a;
float D_L08_001622D0 = 45.0f;
float D_L08_001622F8 = 1e+02f;
float D_L08_001676EC NOT_SDA = 0;
short D_L08_00162540 = 0x1e;
short D_L08_00162544 = 0xa;
short D_L08_00162548 = 0x23;
short D_L08_00162550 = 0;
short D_L08_00162554 = 0x55;
short D_L08_00162558 = 0x1e;
short D_L08_0016255C = 0x14;
short D_L08_00162560 = 0;
short D_L08_00162564 = 0x2d;
short D_L08_00162568 = 0x1e;
short D_L08_0016256C = 0x14;
short D_L08_00162570 = 0;
short D_L08_00162574 = 0x1e;
short D_L08_00162580 = 0x1e;
short D_L08_00162584 = 0;
short D_L08_00162514 = 0xfdb;
short D_L08_00162518 = -10486;
short D_L08_00162588 = 0;
f32 D_L08_00162530 = 0.01f;
f32 D_L08_00162534 = 0.2f;
f32 D_L08_00162538 = 0.06f;
f32 D_L08_0016253C = 0.4f;
s32 D_L08_00162528 = 0x5a;
float D_L09_00161CA0 = 45.0f;
float D_L09_00161D9C = 1e+02f;
short D_L09_00161E98 = 0x1e;
short D_L09_00161E9C = 0xa;
short D_L09_00161EA0 = 0x23;
short D_L09_00161EA8 = 0;
short D_L09_00161EAC = 0x55;
short D_L09_00161EB0 = 0x1e;
short D_L09_00161EB4 = 0x14;
short D_L09_00161EB8 = 0;
short D_L09_00161EBC = 0x2d;
short D_L09_00161EC0 = 0x1e;
short D_L09_00161EC4 = 0x14;
short D_L09_00161EC8 = 0;
short D_L09_00161ECC = 0x1e;
short D_L09_00161ED8 = 0x1e;
short D_L09_00161EDC = 0;
short D_L09_00161E6C = 0xfdb;
short D_L09_00161E70 = -10486;
short D_L09_00161EE0 = 0;
f32 D_L09_00161E88 = 0.01f;
f32 D_L09_00161E8C = 0.2f;
f32 D_L09_00161E90 = 0.06f;
f32 D_L09_00161E94 = 0.4f;
s32 D_L09_00161E80 = 0x5a;
float D_L10_00161DE8 = 45.0f;
float D_L10_00161F08 = 1e+02f;
float D_L10_00162054 = 6.0f;
float D_L10_00162058 = 2.0f;
short D_L10_00162168 = 0x1e;
short D_L10_0016216C = 0xa;
short D_L10_00162170 = 0x23;
short D_L10_00162178 = 0;
short D_L10_0016217C = 0x55;
short D_L10_00162180 = 0x1e;
short D_L10_00162184 = 0x14;
short D_L10_00162188 = 0;
short D_L10_0016218C = 0x2d;
short D_L10_00162190 = 0x1e;
short D_L10_00162194 = 0x14;
short D_L10_00162198 = 0;
short D_L10_0016219C = 0x1e;
short D_L10_001621A8 = 0x1e;
short D_L10_001621AC = 0;
short D_L10_0016213C = 0xfdb;
short D_L10_00162140 = -10486;
short D_L10_001621B0 = 0;
f32 D_L10_00162158 = 0.01f;
f32 D_L10_0016215C = 0.2f;
f32 D_L10_00162160 = 0.06f;
f32 D_L10_00162164 = 0.4f;
s32 D_L10_00162150 = 0x5a;
float D_L11_00161E98 = 45.0f;
float D_L11_00162314 = 1e+02f;
float D_L11_001678EC NOT_SDA = 0;
short D_L11_001626D0 = 0x1e;
short D_L11_001626D4 = 0xa;
short D_L11_001626D8 = 0x23;
short D_L11_001626E0 = 0;
short D_L11_001626E4 = 0x55;
short D_L11_001626E8 = 0x1e;
short D_L11_001626EC = 0x14;
short D_L11_001626F0 = 0;
short D_L11_001626F4 = 0x2d;
short D_L11_001626F8 = 0x1e;
short D_L11_001626FC = 0x14;
short D_L11_00162700 = 0;
short D_L11_00162704 = 0x1e;
short D_L11_00162710 = 0x1e;
short D_L11_00162714 = 0;
short D_L11_001626A4 = 0xfdb;
short D_L11_001626A8 = -10486;
short D_L11_00162718 = 0;
f32 D_L11_001626C0 = 0.01f;
f32 D_L11_001626C4 = 0.2f;
f32 D_L11_001626C8 = 0.06f;
f32 D_L11_001626CC = 0.4f;
s32 D_L11_001626B8 = 0x5a;
float D_L12_00161DC0 = 45.0f;
float D_L12_00161DC4 = 1e+02f;
short D_L12_001621A8 = 0x1e;
short D_L12_001621AC = 0xa;
short D_L12_001621B0 = 0x23;
short D_L12_001621B8 = 0;
short D_L12_001621BC = 0x55;
short D_L12_001621C0 = 0x1e;
short D_L12_001621C4 = 0x14;
short D_L12_001621C8 = 0;
short D_L12_001621CC = 0x2d;
short D_L12_001621D0 = 0x1e;
short D_L12_001621D4 = 0x14;
short D_L12_001621D8 = 0;
short D_L12_001621DC = 0x1e;
short D_L12_001621E8 = 0x1e;
short D_L12_001621EC = 0;
short D_L12_0016217C = 0xfdb;
short D_L12_00162180 = -10486;
short D_L12_001621F0 = 0;
f32 D_L12_00162198 = 0.01f;
f32 D_L12_0016219C = 0.2f;
f32 D_L12_001621A0 = 0.06f;
f32 D_L12_001621A4 = 0.4f;
s32 D_L12_00162190 = 0x5a;
float D_L13_00161E18 = 45.0f;
float D_L13_00161E24 = 1e+02f;
short D_L13_00162020 = 0x1e;
short D_L13_00162024 = 0xa;
short D_L13_00162028 = 0x23;
short D_L13_00162030 = 0;
short D_L13_00162034 = 0x55;
short D_L13_00162038 = 0x1e;
short D_L13_0016203C = 0x14;
short D_L13_00162040 = 0;
short D_L13_00162044 = 0x2d;
short D_L13_00162048 = 0x1e;
short D_L13_0016204C = 0x14;
short D_L13_00162050 = 0;
short D_L13_00162054 = 0x1e;
short D_L13_00162060 = 0x1e;
short D_L13_00162064 = 0;
short D_L13_00161FF4 = 0xfdb;
short D_L13_00161FF8 = -10486;
short D_L13_00162068 = 0;
f32 D_L13_00162010 = 0.01f;
f32 D_L13_00162014 = 0.2f;
f32 D_L13_00162018 = 0.06f;
f32 D_L13_0016201C = 0.4f;
s32 D_L13_00162008 = 0x5a;
float D_L14_00162080 = 45.0f;
float D_L14_00162128 = 1e+02f;
s32 D_L14_00162188 = 0;
s32 D_L14_00162198 = 0;
BeamTransform D_L14_001EDD40 = {0};
Q002e2190 D_L14_001EDE00 = {0};
BeamStrand D_L14_001EDE60 = {0};
float D_L14_00162140 = 5.0f;

f32 D_L14_001EDD00[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};

short D_L14_001623D8 = 0x1e;
short D_L14_001623DC = 0xa;
short D_L14_001623E0 = 0x23;
short D_L14_001623E8 = 0;
short D_L14_001623EC = 0x55;
short D_L14_001623F0 = 0x1e;
short D_L14_001623F4 = 0x14;
short D_L14_001623F8 = 0;
short D_L14_001623FC = 0x2d;
short D_L14_00162400 = 0x1e;
short D_L14_00162404 = 0x14;
short D_L14_00162408 = 0;
short D_L14_0016240C = 0x1e;
short D_L14_00162418 = 0x1e;
short D_L14_0016241C = 0;
short D_L14_001623AC = 0xfdb;
short D_L14_001623B0 = -10486;
short D_L14_00162420 = 0;
f32 D_L14_001623C8 = 0.01f;
f32 D_L14_001623CC = 0.2f;
f32 D_L14_001623D0 = 0.06f;
f32 D_L14_001623D4 = 0.4f;
s32 D_L14_001623C0 = 0x5a;
float D_L15_00161F40 = 45.0f;
float D_L15_00161FB4 = 1e+02f;
s32 D_L15_00161FD8 = 0;
s32 D_L15_00162018 = 0;
s32 D_L15_00162028 = 0;
s32 D_L15_00162068 = 0;
BeamTransform D_L15_001D9570 = {0};
Q002e2190 D_L15_001D9630 = {0};
BeamStrand D_L15_001D9690 = {0};
float D_L15_00161FD0 = 5.0f;

f32 D_L15_001D9530[4][4] = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};

short D_L15_00162328 = 0x1e;
short D_L15_0016232C = 0xa;
short D_L15_00162330 = 0x23;
short D_L15_00162338 = 0;
short D_L15_0016233C = 0x55;
short D_L15_00162340 = 0x1e;
short D_L15_00162344 = 0x14;
short D_L15_00162348 = 0;
short D_L15_0016234C = 0x2d;
short D_L15_00162350 = 0x1e;
short D_L15_00162354 = 0x14;
short D_L15_00162358 = 0;
short D_L15_0016235C = 0x1e;
short D_L15_00162368 = 0x1e;
short D_L15_0016236C = 0;
short D_L15_001622FC = 0xfdb;
short D_L15_00162300 = -10486;
short D_L15_00162370 = 0;
f32 D_L15_00162318 = 0.01f;
f32 D_L15_0016231C = 0.2f;
f32 D_L15_00162320 = 0.06f;
f32 D_L15_00162324 = 0.4f;
s32 D_L15_00162310 = 0x5a;
float D_L16_00161D58 = 45.0f;
float D_L16_00161D5C = 1e+02f;
short D_L16_001620F0 = 0x1e;
short D_L16_001620F4 = 0xa;
short D_L16_001620F8 = 0x23;
short D_L16_00162100 = 0;
short D_L16_00162104 = 0x55;
short D_L16_00162108 = 0x1e;
short D_L16_0016210C = 0x14;
short D_L16_00162110 = 0;
short D_L16_00162114 = 0x2d;
short D_L16_00162118 = 0x1e;
short D_L16_0016211C = 0x14;
short D_L16_00162120 = 0;
short D_L16_00162124 = 0x1e;
short D_L16_00162130 = 0x1e;
short D_L16_00162134 = 0;
short D_L16_001620C4 = 0xfdb;
short D_L16_001620C8 = -10486;
short D_L16_00162138 = 0;
f32 D_L16_001620E0 = 0.01f;
f32 D_L16_001620E4 = 0.2f;
f32 D_L16_001620E8 = 0.06f;
f32 D_L16_001620EC = 0.4f;
s32 D_L16_001620D8 = 0x5a;
float D_L17_00161F20 = 45.0f;
float D_L17_00161F94 = 1e+02f;
float D_L17_001677EC NOT_SDA = 0;
short D_L17_001625D8 = 0x1e;
short D_L17_001625DC = 0xa;
short D_L17_001625E0 = 0x23;
short D_L17_001625E8 = 0;
short D_L17_001625EC = 0x55;
short D_L17_001625F0 = 0x1e;
short D_L17_001625F4 = 0x14;
short D_L17_001625F8 = 0;
short D_L17_001625FC = 0x2d;
short D_L17_00162600 = 0x1e;
short D_L17_00162604 = 0x14;
short D_L17_00162608 = 0;
short D_L17_0016260C = 0x1e;
short D_L17_00162618 = 0x1e;
short D_L17_0016261C = 0;
short D_L17_001625AC = 0xfdb;
short D_L17_001625B0 = -10486;
short D_L17_00162620 = 0;
f32 D_L17_001625C8 = 0.01f;
f32 D_L17_001625CC = 0.2f;
f32 D_L17_001625D0 = 0.06f;
f32 D_L17_001625D4 = 0.4f;
s32 D_L17_001625C0 = 0x5a;
float D_L18_001621F0 = 45.0f;
float D_L18_001621F4 = 1e+02f;
float D_L18_00162570 = 6.0f;
float D_L18_00162574 = 2.0f;
float D_L18_001678EC NOT_SDA = 0;
short D_L18_001626F0 = 0x1e;
short D_L18_001626F4 = 0xa;
short D_L18_001626F8 = 0x23;
short D_L18_00162700 = 0;
short D_L18_00162704 = 0x55;
short D_L18_00162708 = 0x1e;
short D_L18_0016270C = 0x14;
short D_L18_00162710 = 0;
short D_L18_00162714 = 0x2d;
short D_L18_00162718 = 0x1e;
short D_L18_0016271C = 0x14;
short D_L18_00162720 = 0;
short D_L18_00162724 = 0x1e;
short D_L18_00162730 = 0x1e;
short D_L18_00162734 = 0;
short D_L18_001626C4 = 0xfdb;
short D_L18_001626C8 = -10486;
short D_L18_00162738 = 0;
f32 D_L18_001626E0 = 0.01f;
f32 D_L18_001626E4 = 0.2f;
f32 D_L18_001626E8 = 0.06f;
f32 D_L18_001626EC = 0.4f;
s32 D_L18_001626D8 = 0x5a;

f32 beam_endpoint_quad_local_vertices[4][4] __asm__("D_L00_001E6CE0") = {
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
};

float D_L00_00166EEC NOT_SDA = 0;

short follow_camera_ground_aware_focus_transition_frames_gp __asm__("D_L00_00161D38") = 0x1e;

short D_L00_00161D40_gp __asm__("D_L00_00161D40") = 0x23;

short follow_camera_project_hero_focus_blend_base_frames_gp __asm__("D_L00_00161D4C") = 0x55;

short follow_camera_hold_focus_blend_base_frames_gp __asm__("D_L00_00161D5C") = 0x2d;

short D_L00_00161D6C_gp __asm__("D_L00_00161D6C") = 0x1e;

short follow_camera_continue_projected_focus_transition_frames_gp __asm__("D_L00_00161D78") = 0x1e;

s32 follow_cam_blend_frames __asm__("D_L00_00161D20") = 0x5a;
