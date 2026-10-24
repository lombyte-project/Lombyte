/* Level 12 moby classes: platform, miner, blarg_trooper, blarg_helicopter, edwina, helicopter, crane_arm. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/watch.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    float position[3];
    char unk0C[0x12];
    u16 visibility;
    char unk20[0x1170];
} L12Slot;
extern char D_L12_001CBF40_c[] __asm__("D_L12_001CBF40");
extern char D_L12_001CAD00_c[] __asm__("D_L12_001CAD00");
extern char D_L12_001CBE00_c[] __asm__("D_L12_001CBE00");

extern char D_L12_0020BC90[];
extern char D_L12_00167240[];
extern float absolute_float(float) __asm__("func_001F99C0");
void initialize_level_12_slot_grid_neighbors(int selected_slot_index) __asm__("FUN_L12_002bf770");

extern void initialize_level_12_slot_grid_neighbors(int) __asm__("FUN_L12_002bf770");

#include "rnc/overlay/l12/gameplay/entities/platform_globals.h"
extern float D_0015ED70_c __asm__("D_0015ED70");
extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern void slot_limit_c(int) __asm__("FUN_001e93b0");
extern void initialize_slots_c(void *, int) __asm__("FUN_L01_002b7a48");
extern void initialize_slot_data_c(float) __asm__("FUN_L01_002b7ef0");
extern void copy_slot_c(void *, void *) __asm__("FUN_L01_002b7c68");
extern void finish_slots_c(void *, int) __asm__("FUN_L01_002b7fe0");
extern float D_L12_00180340[8];
extern int D_L12_001612D0;
extern void *D_L12_001612D4;
extern int D_L12_001612D8;
extern float D_L12_0016120C;
extern float D_L12_00161210;
extern float D_L12_00161214;
extern int D_L12_00161208;
extern float angle_c(float, float) __asm__("FUN_001f9e90");
extern float sine_c(float) __asm__("FUN_001f9dc8");
extern float cosine_c(float) __asm__("FUN_001f9de0");
extern int random_integer_below_c(int) __asm__("FUN_00213260");
extern float random_float_between_c(float, float) __asm__("FUN_002132a8");
extern void move_scalar_c(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void move_slot_c(float, float, float, float, void *, int, int) __asm__("FUN_L00_002a3ec8");
extern int FUN_L01_00275690_c(float, void *) __asm__("FUN_L01_00275690");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void register_callback_c(void *, void *) __asm__("FUN_L12_0021be50");
extern void copy_moby_z_to_group_records(struct Moby *) __asm__("FUN_L12_002bf6d0");
extern void initialize_level_12_slot_grid_neighbors(int) __asm__("FUN_L12_002bf770");
extern void render_level_12_water_grids(void) __asm__("FUN_L12_002bf748");

void update_level_12_water_tile(struct Moby *moby) __asm__("FUN_L12_002bf140");

void update_level_12_water_tile(struct Moby *moby) {
    enum {
        SLOT_INITIALIZE = 0,
        SLOT_INITIALIZE_NEIGHBORS_AND_HEIGHT = 1,
        SLOT_UPDATE_HEIGHT_AND_MOTION = 2
    };
    int state = moby->state;
    int *data = (int *)moby->pvars;
    int slot;
    float value;
    float other;
    int i;
    switch (state) {
    case SLOT_INITIALIZE:
        i = (data[3] = next_water_tile_index) + 1;
        next_water_tile_index = i;
        if (i > 0x7f) {
            slot_limit_c((int)D_L12_0020BC90);
        }
        moby->scale = moby->pclass->scale * 2.0f;
        data[0] = 3;
        moby->update_dist = 0xff;
        moby->flags |= 1;
        if (data[3] == 0) {
            {
                int i = 16;
                do {
                    ((L12Slot *)D_L12_001CBF40_c)[i].visibility = 0;
                } while (--i >= 0);
            }
            initialize_slots_c(D_L12_001CBF40_c, 17);
            initialize_slot_data_c(1.0f);
            *(float *)(D_L12_001CAD00_c + 0x00) = 16.0f;
            *(float *)(D_L12_001CAD00_c + 0x04) = 16.0f;
            *(float *)(D_L12_001CAD00_c + 0x08) = -8.0f;
            *(float *)(D_L12_001CAD00_c + 0x0c) = -8.0f;
            *(float *)(D_L12_001CAD00_c + 0x10) = 1.0f;
            *(float *)(D_L12_001CAD00_c + 0x14) = 1.0f;
            *(float *)(D_L12_001CAD00_c + 0x18) = 0.9f;
            *(float *)(D_L12_001CAD00_c + 0x20) = 0.1f;
            value = angle_c(D_L12_00180340[4], D_L12_00180340[5]);
            *(float *)(D_L12_001CAD00_c + 0x30) = sine_c(value) * 0.57735f;
            *(float *)(D_L12_001CAD00_c + 0x34) = cosine_c(value) * 0.57735f;
            *(float *)(D_L12_001CAD00_c + 0x38) = -0.57735f;
            *(int *)(D_L12_001CAD00_c + 0x28) = data[0] + 0x28;
            *(int *)(D_L12_001CAD00_c + 0x2c) = data[1] + 0x28;
            *(u8 *)(D_L12_001CAD00_c + 0x3c) = 0x1c;
            *(u8 *)(D_L12_001CAD00_c + 0x3d) = 0x40;
            D_L12_001612D0 = (int)D_L12_001CBF40_c;
            D_L12_001612D4 = D_L12_001CBE00_c;
            D_L12_001612D8 = 17;
            D_L12_0016120C = 32768.0f;
            D_L12_00161210 = 255.0f;
            D_L12_00161214 = 48.0f;
            D_L12_00161208 = 0;
        }
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190) = moby->pos.x;
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190 + 4) = moby->pos.y;
        *(float *)(D_L12_001CBF40_c + data[3] * 0x1190 + 8) = moby->pos.z;
        *(int *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x14) = data[0] + 0x28;
        *(int *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x18) = data[1] + 0x28;
        *(u8 *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x1c) = 0x1c;
        *(u8 *)(D_L12_001CBF40_c + data[3] * 0x1190 + 0x1d) = 0x44;
        ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0xffff;
        copy_slot_c(D_L12_001CBF40_c + data[3] * 0x1190, D_L12_001CBE00_c + data[3] * 16);
        moby->state = 1;
        break;
    case SLOT_INITIALIZE_NEIGHBORS_AND_HEIGHT:
        next_water_tile_index = 0;
        initialize_level_12_slot_grid_neighbors(data[3]);
        moby->state = 2;
        *(float *)(data + 4) = moby->pos.z;
        break;
    case SLOT_UPDATE_HEIGHT_AND_MOTION:
        if ((moby->command & 4) == 0) {
            moby->command = 1;
        } else {
            data[5] = 1;
            *(float *)(data + 4) += 5.0f;
            moby->command = 1;
        }
        if (data[5] != 0) {
            move_scalar_c(&moby->pos.z, (float *)(data + 2), *(float *)(data + 4), D_0015ED70_c * 4.0f, D_0015ED70_c * 4.0f, D_0015ED6C_c * 4.0f);
            copy_moby_z_to_group_records(moby);
        }
        if (absolute_float(hero.motion.pos.f[0] - moby->pos.x) <= 8.0f ||
            absolute_float(hero.motion.pos.f[1] - moby->pos.y) <= 8.0f) {
            hero.water_surface_z = moby->pos.z;
        }
        if (random_integer_below_c(200) == 0) {
            char *slot_record;
            other = random_float_between_c(-4.0f, 4.0f);
            value = *(float *)(D_L12_001CBF40_c + data[3] * 0x1190);
            value += other;
            other = random_float_between_c(-4.0f, 4.0f);
            slot_record = D_L12_001CBF40_c + data[3] * 0x1190;
            move_slot_c(value, *(float *)(slot_record + 4) + other, 1.0f, -0.05f, slot_record, 1, 1);
        }
        break;
    }
update_slot:
    slot = data[3];
    if (slot >= 0) {
        float visible = 0.0f;
        if (FUN_L01_00275690_c(64.0f, moby) != -1) visible = 1.0f;
        if (data[6] != -1 && is_point_inside_clip_volume(D_L12_00167240, data[6]) == 0) visible = 0.0f;
        if (visible == 0.0f) {
            ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0;
        } else {
            ((L12Slot *)D_L12_001CBF40_c)[data[3]].visibility = 0xffff;
        }
        if (data[3] == 0) {
            finish_slots_c(D_L12_001CBF40_c, 17);
            register_callback_c(render_level_12_water_grids, moby);
        }
    }
}
/* Level overlay code after unclassified; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

/* records, for entry arg, which of the 48 entries sit one grid step away */
/* Same routine as FUN_L05_0030d6a0, with this level's table and entry count. */

typedef struct {
    float x;
    float y;
    int pad;
    unsigned char n[0x14];
    char rest[0x1170];
} Ent;

extern Ent D_L12_001CBF40[];
float absolute_float(float input) __asm__("func_001F99C0");

void initialize_level_12_slot_grid_neighbors(int selected_slot_index) {
    int i;
    D_L12_001CBF40[selected_slot_index].n[0] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[2] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[1] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[3] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[4] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[5] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[6] = 0xFF;
    D_L12_001CBF40[selected_slot_index].n[7] = 0xFF;
    for (i = 0; i < 17; i++) {
        if (selected_slot_index != i) {
            Ent *slot_table = D_L12_001CBF40;
            Ent *selected_slot = &slot_table[selected_slot_index];
            float x = selected_slot->x - slot_table[i].x;
            float y = selected_slot->y - slot_table[i].y;
            if (absolute_float(x) < 0.1f) {
                if (absolute_float(y - 16.0f) < 0.1f)
                    selected_slot->n[0] = i;
                if (absolute_float(y + 16.0f) < 0.1f)
                    selected_slot->n[2] = i;
            } else if (absolute_float(y) < 0.1f) {
                if (absolute_float(x - 16.0f) < 0.1f)
                    selected_slot->n[1] = i;
                if (absolute_float(x + 16.0f) < 0.1f)
                    selected_slot->n[3] = i;
            } else {
                if (absolute_float(x + 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_slot->n[5] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_slot->n[7] = i;
                } else if (absolute_float(x - 16.0f) < 0.1f) {
                    if (absolute_float(y - 16.0f) < 0.1f)
                        selected_slot->n[4] = i;
                    if (absolute_float(y + 16.0f) < 0.1f)
                        selected_slot->n[6] = i;
                }
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E29C8), where it is exact; names translated to the US level program. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");
extern void FUN_L01_0026e090(int, int);

int update_moby_suck_cannon_suction_state(struct Moby *moby) __asm__("FUN_L12_002e16b8");

int update_moby_suck_cannon_suction_state(struct Moby *moby) {
    int active;
    if (moby->state == 9 || moby->state == 11 || moby->state == 13)
        return 0;
    active = process_moby_suck_cannon_suction(moby);
    if (active) {
        if (moby->group != 0xFF)
            FUN_L01_0026e090(moby->group, 1);
        moby->state = 8;
    } else if (moby->state == 8) {
        moby->state = 1;
    }
    return active;
}
#define NOT_SDA

#define MACRO_ADDR

/* Move the vendor moby to state eight unless it is in state nine or eleven. */
/* Ported from rac1-decomp (src/overlays/l12/vendor_002C0310.c: func_L12_002E2B08), where it is exact; names translated to the US level program. */


extern int cancel_suck_cannon_suction(struct Moby *) __asm__("FUN_L00_002dbe20");

int cancel_suck_cannon_suction_and_set_state_eight(struct Moby *moby) __asm__("FUN_L12_002e17f8");

int cancel_suck_cannon_suction_and_set_state_eight(struct Moby *moby) {
    int result = cancel_suck_cannon_suction(moby);
    if (moby->state == 9 || moby->state == 11) {
        return 0;
    }
    moby->state = 8;
    return result;
}
#include "sda.h"

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2B88), where it is exact; names translated to the US level program. */

typedef union {
    long long quad;
    float f[4];
} L12Vector;

extern float D_0015ED70_c __asm__("D_0015ED70");
extern float probe_ground_height_c(void *, int, float) __asm__("func_00213508");
extern void approach_value_c(f32 *, f32, f32) __asm__("func_00213ED8");
extern float func_00214358_order(float, void *, int) __asm__("FUN_00213508");
extern void func_00214D28_order(float, float, void *) __asm__("FUN_00213ed8");

void ease_hoven_anklebiter_height_to_ground(char *moby) __asm__("FUN_L12_002e1878");

void ease_hoven_anklebiter_height_to_ground(char *moby) {
    L12Vector position;
    float value;
    qcopy(&position, moby + 0x10);
    position.f[2] += 0.5f;
    value = func_00214358_order(0.5f, &position, 0);
    func_00214D28_order(value, D_0015ED70_c * 27.0f, moby + 0x18);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2BF8), where it is exact; names translated to the US level program. */

extern int random_integer_below(int);
extern int scale_game_frames(int);
extern void blend_moby_animation(void *, int, int, int);

void select_hoven_anklebiter_pursuit_animation(struct Moby *moby) __asm__("FUN_L12_002e18e8");

void select_hoven_anklebiter_pursuit_animation(struct Moby *moby) {
    enum {
        ANKLEBITER_PURSUIT_ANIMATION_VARIANT_A = 0,
        ANKLEBITER_PURSUIT_ANIMATION_VARIANT_B = 1
    };
    switch (random_integer_below(2)) {
    case ANKLEBITER_PURSUIT_ANIMATION_VARIANT_A:
        if (moby->prev_seq != 3)
            blend_moby_animation(moby, 3, 0, scale_game_frames(8));
        break;
    case ANKLEBITER_PURSUIT_ANIMATION_VARIANT_B:
        if (moby->prev_seq != 0xF)
            blend_moby_animation(moby, 0xF, 0, scale_game_frames(8));
        break;
    default:
        if (moby->prev_seq != 0x10)
            blend_moby_animation(moby, 0x10, 0, scale_game_frames(8));
        break;
    }
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Scatters 27 particles around a point with random offsets and velocities. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2CB8), where it is exact; names translated to the US level program. */


extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

int D_L12_001618C4 = 0xa0f0e8e0;
int D_L12_001618C8 = 0x20f0e8e0;
int D_L12_001618CC = 0x10017;
void spawn_spark_scatter(void *spark_scatter_origin) __asm__("FUN_L12_002e19a8");

void spawn_spark_scatter(void *spark_scatter_origin) {
    float base_position[4];
    float spark_position[4];
    float vertical_spark_vector[4];
    float radial_spark_vector[4];
    float radial_angle_radians;
    int i;
    int frame_count, second_frame_count, third_frame_count;
    *(OvlQuad *)base_position = *(OvlQuad *)spark_scatter_origin;
    i = 0x1A;
    do {
        radial_angle_radians = random_angle_radians();
        qcopy(spark_position, base_position);
        spark_position[2] = spark_position[2] + random_float_between_alt(-0.25f, -0.75f);
        spark_position[0] = spark_position[0] + random_float_between_alt(-0.25f, 0.25f);
        spark_position[1] = spark_position[1] + random_float_between_alt(-0.25f, 0.25f);
        clear_u64_value(vertical_spark_vector);
        radial_spark_vector[0] = fast_cos(radial_angle_radians) * (random_float_between_alt(spark_scatter_radial_xy_scale_range_start,
                                                        spark_scatter_radial_xy_scale_range_end) *
                               frame_time);
        radial_spark_vector[1] = fast_sin(radial_angle_radians) * (random_float_between_alt(spark_scatter_radial_xy_scale_range_start,
                                                        spark_scatter_radial_xy_scale_range_end) *
                               frame_time);
        *(int *)&radial_spark_vector[2] = 0;
        vertical_spark_vector[2] = random_float_between_alt(spark_scatter_vertical_z_range_start, spark_scatter_vertical_z_range_end) *
                frame_time;
        radial_spark_vector[2] = random_float_between_alt(spark_scatter_radial_z_range_start, spark_scatter_radial_z_range_end) *
                frame_time;
        vertical_spark_vector[3] = spark_scatter_vertical_vector_w;
        radial_spark_vector[3] = spark_scatter_radial_vector_w;
        frame_count = func_001FA898_r(random_float_between_alt((float)spark_scatter_frame_count_range_start,
                                                     (float)spark_scatter_frame_count_range_end));
        second_frame_count = func_001FA898_r(random_float_between_alt((float)spark_scatter_second_frame_count_range_start,
                                                     (float)spark_scatter_second_frame_count_range_end));
        third_frame_count = func_001FA898_r(random_float_between_alt((float)spark_scatter_third_frame_count_range_start,
                                                     (float)spark_scatter_third_frame_count_range_end));
        spawn_spark(spark_position, vertical_spark_vector, radial_spark_vector, D_L12_001618C4, D_L12_001618C8, frame_count, second_frame_count, third_frame_count,
                     D_L12_001618CC);
    } while (--i >= 0);
}
#include "rnc/math/vector.h"

/* Updates a Hoven anklebiter's (moby class 238) idle, pursuit, melee, leap, landing and death states. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E2EF0), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct AnklebiterData_2E2EF0 AnklebiterData_2E2EF0;

typedef struct AnklebiterMoby_2E2EF0 {
    char p0[0x10]; float pos[4]; unsigned char state, group; char p22[2]; char *model;
    char p28[9]; unsigned char enabled; unsigned short joints, flags; char p36[0xA];
    float rot[4]; char p50[2]; unsigned char anim_end, anim; float anim_time, anim_rate;
    char p5C[0x14]; unsigned char anim_flags; char p71[7]; AnklebiterData_2E2EF0 *data;
    char p7C[3]; unsigned char active; char p80[0x14]; int collision;
    char p98[0x24]; unsigned char proximity;
} AnklebiterMoby_2E2EF0;

struct AnklebiterData_2E2EF0 {
    char p0[0x20]; float health; char p24[5]; unsigned char mode; char p2A[0x2E];
    unsigned char blend; char p59; unsigned char count; char p5B[0x6D]; short stuck; char pCA[6]; void *tuning;
    char pD4[0x4C]; float fall[4], gravity, zero, launch_speed, launch_time; int fall_flags, fall_mode; float damping;
    char p14C[0x11]; unsigned char falling; char p15E[0x12]; float fall_a, fall_b; char p178[8];
    int effect_class; char p184[4]; float effect_size0, effect_size1; char p190[0x14]; float effect_speed;
    char p1A8[0x10]; int effect_flags; char p1BC[0x14]; float idle_pos[4], spare, idle_speed;
    char p1E8[0x18]; char collision_data[0x68]; float look_angle; char p26C[0x14];
    float home[4]; char p290[0x10]; float turn_velocity, circle_angle, circle_rate, range;
    char p2B0[8]; int init0, sleep, circle_timer, alert_timer; short group_timer, pad2CA, idle_timer, pad2CE;
    int path, blocked, return_timer, group_enabled; int *group_count; int group_check;
};

typedef struct TargetReport54_2E2EF0 { float v0[4], v10[4], v20[4], v30[4]; AnklebiterMoby_2E2EF0 *moby; int mode; char pad48[8]; } TargetReport54_2E2EF0;

typedef struct {
    char pad0[0x248];
    void *child;
} Level12VendorData_2e1be0;

typedef struct {
    char pad0[0x78];
    Level12VendorData_2e1be0 *data;
    char pad7C[0x38];
    short fieldB4;
} Level12VendorMoby;

extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
extern char *D_L12_001B0930[];
extern char D_L12_00167240[];
int D_L12_001618E8 = 0xa030303;
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz_c3(void *) __asm__("FUN_001f9af0");
extern float D_0015ED64;
extern float D_0015ED6C;
extern float D_0015ED70;
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern float probe_ground_height_c2(void *, int, float) __asm__("func_00213508");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_00259740(void *, void *, int);
extern int FUN_L00_002dc9f0();
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 rand(void);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 tick_countdown_32_alt_c2(s32 *) __asm__("FUN_001f9740");
extern void FUN_L00_00257470_c(Level12VendorMoby *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void select_moby_combat_target(char *, float *, float) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_002628d8_c(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void FUN_L12_002e3098_u(struct Moby *) __asm__("FUN_L12_002e3098");
extern void approach_value_alt(void *, float, float) __asm__("FUN_00213ed8");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a28");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float FUN_L00_0025abf0(float *a, float *b, float *out, float speed, float g);
float absolute_float(float input) __asm__("func_001F99C0");
int FUN_L00_00257b90(int lo, int hi);
int get_first_moby_in_group(char **out, int idx, int a, int b) __asm__("FUN_L00_002591d0");
int get_next_moby_in_group(char **out, char *target, int a, int b) __asm__("FUN_L00_002592b8");
int mine_fall(void *moby, void *motion) __asm__("FUN_L00_0025c698");
int FUN_L12_002e2eb8_c(struct Moby *m, float speed) __asm__("FUN_L12_002e2eb8");
s32 is_value_within_interpolated_window(void *anim, f32 value) __asm__("FUN_00214cc8");
void FUN_L00_0025a120(char *o);
void update_object_wander_movement(u8 *o, u8 *s, f32 a, f32 b) __asm__("FUN_L00_00261630");
void FUN_L00_00263b38(char *p, float x);
void FUN_L01_0026d930(char *p);
void spawn_type_52_particle_near_moby(char *moby) __asm__("FUN_L10_002e3de8");
void FUN_L12_002e18e8_c(struct Moby *moby) __asm__("FUN_L12_002e18e8");
void spawn_spark_scatter(void *pos) __asm__("FUN_L12_002e19a8");
void FUN_L12_002e3528_c(struct Moby *moby) __asm__("FUN_L12_002e3528");
extern char D_0013E633_2E2EF0[] __asm__("D_001413D0");
extern char *D_L12_001B0C30_2E2EF0[] __asm__("D_L12_001B0930");
extern char D_L12_00167240[];
extern void func_L12_002E4838_2E2EF0(void *) __asm__("FUN_L12_002e3528");
extern void func_L00_00264BB0_2E2EF0(void *,float) __asm__("FUN_L00_00263b38");
extern int FUN_L01_00274b78_c(char *,char *,float) __asm__("FUN_L01_00274b78");
extern void func_00213DE0_2E2EF0(void *,int,int,int) __asm__("FUN_00212f90");
extern void func_L01_0026E8E0_2E2EF0(char *) __asm__("FUN_L01_0026d930");
extern int func_001F9850_2E2EF0(int) __asm__("FUN_001f96f8");
extern int func_L00_00258BC8_2E2EF0(int,int) __asm__("FUN_L00_00257b90");
extern int func_001160D8_2E2EF0(void) __asm__("FUN_001160d8");
extern int func_002140B0_2E2EF0(int) __asm__("FUN_00213260");
extern float func_00214358_2E2EF0(void *,int,float) __asm__("FUN_00213508");
extern float func_001F9D10_2E2EF0(void *,void *) __asm__("FUN_001f9b48");
extern float func_L00_001FF860_2E2EF0(float,float) __asm__("FUN_001f9e90");
extern float func_001FA850_2E2EF0(float,float) __asm__("FUN_001fa688");
extern float func_001F9B88_2E2EF0(float) __asm__("FUN_001f99c0");
extern float func_002140F8_2E2EF0(float,float) __asm__("FUN_002132a8");
extern float smooth54(float *,float,float) __asm__("FUN_00213ed8");
extern void func_L00_002626A8_2E2EF0(char *,char *,float,float) __asm__("FUN_L00_00261630");
extern void fast_vec_sub(void *,void *,void *) __asm__("FUN_001f9a28");
extern float func_001F9D48_2E2EF0(void *,void *) __asm__("FUN_001f9b80");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int func_L00_0025A208_2E2EF0(int *,int,int,int) __asm__("FUN_L00_002591d0");
extern int func_L00_0025A2F0_2E2EF0(int *,int,int,int) __asm__("FUN_L00_002592b8");
extern int func_001F9938_2E2EF0(void *) __asm__("FUN_001f9770");
extern void func_L01_0026F040_2E2EF0(int,int) __asm__("FUN_L01_0026e090");
extern void spawn_spark_scatter(void *) __asm__("FUN_L12_002e19a8");
extern void func_L00_002592B0_2E2EF0(char *,float *,float,float,float,float) __asm__("FUN_L00_00258278");
extern void func_L12_002E2BF8_2E2EF0(unsigned char *) __asm__("FUN_L12_002e18e8");
extern float func_L00_0025BC48_2E2EF0(void *,void *,void *,float,float) __asm__("FUN_L00_0025abf0");
extern void setup_fall54(float,char *,float *,int,int,int) __asm__("FUN_L00_0025c558");
extern int func_L00_0025D6F0_2E2EF0(void *,void *) __asm__("FUN_L00_0025c698");
extern int func_L12_002E41C8_2E2EF0(char *,float) __asm__("FUN_L12_002e2eb8");
extern int func_001F9908_2E2EF0(int *) __asm__("FUN_001f9740");
extern int func_00215B18_2E2EF0(char *,float) __asm__("FUN_00214cc8");
extern float func_001FA790_2E2EF0(float,float) __asm__("FUN_001fa5c8");
extern float func_001FA748_2E2EF0(float,float) __asm__("FUN_001fa580");
extern float func_001F9F90_2E2EF0(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2E2EF0(float) __asm__("FUN_001f9de0");
extern void func_L00_0025AC00_2E2EF0(void *,int,int,void *,void *,float) __asm__("FUN_L00_00259bc8");
extern int func_L00_0025A778_2E2EF0(void *,void *,int) __asm__("FUN_L00_00259740");
extern int func_L00_002DDEA0_2E2EF0(void *,void *) __asm__("FUN_L00_002dc9f0");
extern float height54(void *) __asm__("FUN_0020c9e0");
extern void func_L00_002584A8_2E2EF0(void *,int,int) __asm__("FUN_L00_00257470");
extern void spawn_type_52_particle_near_moby(char *) __asm__("FUN_L10_002e3de8");
extern void func_L12_002E43A8_2E2EF0(void *) __asm__("FUN_L12_002e3098");
extern void func_0020D678_2E2EF0(void *) __asm__("FUN_0020c828");
extern void func_L00_00263950_2E2EF0(char *,char *,int,float,float) __asm__("FUN_L00_002628d8");
extern void func_L00_0025B178_2E2EF0(void *) __asm__("FUN_L00_0025a120");

void update_hoven_anklebiter(AnklebiterMoby_2E2EF0 *moby) __asm__("FUN_L12_002e1be0");

void update_hoven_anklebiter(AnklebiterMoby_2E2EF0 *moby) {
    enum {
        ANKLEBITER_INITIALIZE = 0,
        ANKLEBITER_IDLE = 1,
        ANKLEBITER_FACE_TARGET_BEFORE_PURSUIT = 3,
        ANKLEBITER_PURSUE_TARGET = 4,
        ANKLEBITER_MELEE_ATTACK = 5,
        ANKLEBITER_RETURN_HOME = 6,
        ANKLEBITER_KNOCKBACK_FALL = 7,
        ANKLEBITER_CONTROLLED_MOTION_UNTIL_RETURN_HOME = 8,
        ANKLEBITER_DORMANT = 9,
        ANKLEBITER_RECOVER_AFTER_KNOCKBACK = 10,
        ANKLEBITER_WAIT_FOR_LEAP_TRIGGER = 11,
        ANKLEBITER_LEAP_TO_HOME = 12,
        ANKLEBITER_ACTIVATE_FROM_DORMANCY = 13,
        ANKLEBITER_LAND_AFTER_LEAP = 14,
        ANKLEBITER_DEATH_EFFECTS_AND_REMOVE = 15,
        ANKLEBITER_DEATH_FALL = 99
    };
    TargetReport54_2E2EF0 work;
    AnklebiterMoby_2E2EF0 *iterator;
    float trajectory;
    AnklebiterData_2E2EF0 *data = moby->data;
    AnklebiterMoby_2E2EF0 *target;
    float angle;
    int count, first, blocked, hit;
    func_L12_002E4838_2E2EF0(moby);
    func_L00_00264BB0_2E2EF0(data->collision_data,2.2f);
    FUN_L01_00274b78_c((char *)moby,(char *)&work,data->range);
    target=work.moby;
    if (!target) target=*(AnklebiterMoby_2E2EF0 **)D_0013E633_2E2EF0;
    if (moby->pos[2]<26.0f) { func_0020D678_2E2EF0(moby); return; }
    switch(moby->state) {
    case ANKLEBITER_INITIALIZE:
        if(data->sleep) {
            qcopy(data->home,moby->pos); moby->state=9;
            if(moby->anim!=12) func_00213DE0_2E2EF0(moby,12,0,3);
            moby->collision=0; moby->flags=(moby->flags|0x41)&0xEFFF;
        } else if(data->home[3]!=0.0f) {
            moby->state=11; moby->joints=0; moby->flags&=0xEFFF; moby->collision=0;
        } else {
            qcopy(data->home,moby->pos); moby->state=1;
            if(moby->anim!=0) func_00213DE0_2E2EF0(moby,0,0,3);
        }
        {
        int initial_mode=1; /* loaded ahead of the other constants: it is the last one stored (retail keeps it in $a2 into the call's delay slot) */
        data->alert_timer=0; data->init0=0; data->damping=0.5f; data->health=2.0f; data->blend=8; data->count=5; data->mode=initial_mode; data->fall_flags=0x200;
        func_L01_0026E8E0_2E2EF0((char *)&data->effect_class);
        data->effect_class=0x38D; data->effect_flags|=0x10; data->effect_speed=D_0015ED6C*5.5f;
        data->effect_size1=0.5f; data->effect_size0=0.5f;
        if(data->group_enabled) { first=func_001F9850_2E2EF0(20); data->group_timer=func_L00_00258BC8_2E2EF0(first,func_001F9850_2E2EF0(170)); }
        qcopy(data->idle_pos,moby->pos);
        first=func_001F9850_2E2EF0(120); data->idle_timer=func_L00_00258BC8_2E2EF0(first,func_001F9850_2E2EF0(700));
        qcopy(work.v0,moby->pos); work.v0[2]+=0.8f;
        moby->pos[2]=func_00214358_2E2EF0(work.v0,0,0.5f);
        if(func_001160D8_2E2EF0()&1) moby->flags|=0x8000;
        data->tuning=&D_L12_001618E8;
        }
        break;
    case ANKLEBITER_IDLE: {
        float idle_distance, idle_angle, idle_difference;
        idle_distance=func_001F9D10_2E2EF0(target->pos,data->home);
        idle_angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        idle_difference=func_001FA850_2E2EF0(moby->rot[2],idle_angle);
        if(idle_distance<data->range && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<4.0f) {
            moby->state=3;
            if(idle_difference<0.9599311f) {
                if(moby->anim!=2) func_00213DE0_2E2EF0(moby,2,0,func_001F9850_2E2EF0(5));
                moby->anim_rate=1.55f;
            } else if(moby->anim!=17) func_00213DE0_2E2EF0(moby,17,0,func_001F9850_2E2EF0(5));
        } else if(((moby->proximity&1)&&func_002140B0_2E2EF0(19)==0)||moby->proximity==7) {
            if(func_002140B0_2E2EF0(256)&1) data->circle_rate=-data->circle_rate;
            data->circle_angle=data->circle_rate*0.017453292f;
            data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f));
            moby->state=3;
            if(moby->anim!=2) func_00213DE0_2E2EF0(moby,2,0,func_001F9850_2E2EF0(7));
            moby->proximity=0;
        } else {
            if(moby->anim!=15) func_00213DE0_2E2EF0(moby,15,0,func_001F9850_2E2EF0(14));
            count=data->idle_timer;
            if(count>func_001F9850_2E2EF0(20)) smooth54(&data->idle_speed,D_0015ED6C*2.5f,D_0015ED70*7.0f);
            else smooth54(&data->idle_speed,0.0f,D_0015ED70*7.0f);
            moby->anim_rate=0.8f;
            func_L00_002626A8_2E2EF0((char *)moby,(char *)data->idle_pos,0.35f,0.25f);
        }
        break;
    }
    case ANKLEBITER_DORMANT:
        if(data->group_enabled) {
            if(func_001F9D48_2E2EF0(moby->pos,target->pos)<2.0f) break;
            if(data->group_check<D_L12_0015F5CC) {
                data->group_check=D_L12_0015F5CC; count=0;
                func_L00_0025A208_2E2EF0((int *)&iterator,moby->group,0,0);
                while(iterator) {
                    if((signed char)iterator->state>=0 && iterator->state!=9) count++;
                    func_L00_0025A2F0_2E2EF0((int *)&iterator,(int)iterator,0,0);
                }
                *data->group_count=count;
            }
            if(*data->group_count>=6) break;
        }
        fast_vec_sub(work.v0,target->pos,moby->pos);
        if(fast_vec_length(work.v0)<data->range && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<8.0f && func_002140B0_2E2EF0(19)==0) {
            if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
        } else if(!((moby->proximity&1)&&func_001F9938_2E2EF0(&data->group_timer)) && moby->proximity!=7) break;
        moby->state=13;
        if(moby->anim!=7) func_00213DE0_2E2EF0(moby,7,0,1);
        moby->flags|=0x1000;
        *(u128 *)work.v10=*(u128 *)moby->pos;
        spawn_spark_scatter(work.v10);
        break;
    case ANKLEBITER_ACTIVATE_FROM_DORMANCY: {
        int collision=*(int *)(moby->model+0x10);
        moby->flags&=0xFFBE; moby->collision=collision;
        angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.03f,0.3f,D_0015ED6C*6.2831855f);
        if(moby->anim_flags&2) {
            if(func_002140B0_2E2EF0(256)&1) data->circle_rate=-data->circle_rate;
            data->circle_angle=data->circle_rate*0.017453292f;
            data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f));
            moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); moby->proximity=0;
        }
        break;
    }
    case ANKLEBITER_WAIT_FOR_LEAP_TRIGGER:
        angle=func_L00_001FF860_2E2EF0(data->home[0]-moby->pos[0],data->home[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.05f,0.3f,0.2f);
        fast_vec_sub(work.v0,target->pos,data->home);
        if((fast_vec_length(work.v0)<data->range&&func_001F9B88_2E2EF0(data->home[2]-target->pos[2])<8.0f)||moby->proximity==2) {
            if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,2);
            moby->joints=0x40; data->zero=0.0f;
            data->gravity=hoven_anklebiter_leap_gravity*D_0015ED70; data->launch_speed=hoven_anklebiter_leap_launch_speed*D_0015ED6C;
            data->launch_time=func_L00_0025BC48_2E2EF0(moby->pos,data->home,&trajectory,data->launch_speed,-hoven_anklebiter_leap_gravity);
            data->fall_mode=5; data->falling=0;
            setup_fall54(moby->rot[2],(char *)moby,data->fall,8,1,0);
            data->fall_a=5.0f; data->fall_b=10.0f; moby->state=12; moby->flags|=0x1000;
            moby->collision=*(int *)(moby->model+0x10);
        }
        break;
    case ANKLEBITER_LEAP_TO_HOME:
        if(func_L00_0025D6F0_2E2EF0(moby,data->fall)&1) {
            moby->state=14;
            if(moby->anim!=9) func_00213DE0_2E2EF0(moby,9,8,func_001F9850_2E2EF0(7));
            return;
        }
        if(moby->pos[2]<0.0f) { func_0020D678_2E2EF0(moby); return; }
        break;
    case ANKLEBITER_LAND_AFTER_LEAP:
        if(moby->anim_flags&2) {
            moby->state=4; data->blocked=0;
            if(func_002140B0_2E2EF0(256)&1) data->circle_rate=-data->circle_rate;
            data->circle_angle=data->circle_rate*0.017453292f;
            data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f));
            func_L12_002E2BF8_2E2EF0((unsigned char *)moby);
        }
        break;
    case ANKLEBITER_FACE_TARGET_BEFORE_PURSUIT:
        angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.03f,0.3f,D_0015ED6C*6.2831855f);
        if(moby->anim_flags&2) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); }
        break;
    case ANKLEBITER_PURSUE_TARGET: {
        float chase_angle, look_difference, clamp;
        chase_angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        look_difference=func_001FA790_2E2EF0(chase_angle,moby->rot[2]);
        clamp=0.6981317f;
        if(look_difference>clamp || (clamp=-0.6981317f,look_difference<clamp)) look_difference=clamp;
        data->look_angle=look_difference;
        if(func_L12_002E41C8_2E2EF0((char *)moby,func_001FA748_2E2EF0(chase_angle,data->circle_angle))==2) { moby->state=6; data->return_timer=0; }
        else {
            if(func_001F9908_2E2EF0(&data->circle_timer)) {
                data->circle_timer=func_001F9850_2E2EF0((int)func_002140F8_2E2EF0(30.0f,90.0f)); data->circle_angle=-data->circle_angle;
            }
            fast_vec_sub(work.v0,target->pos,data->home);
            if(func_001F9D48_2E2EF0(moby->pos,target->pos)<1.5f) {
                moby->state=5;
                if(moby->anim!=4) func_00213DE0_2E2EF0(moby,4,0,func_001F9850_2E2EF0(5));
            } else if(fast_vec_length(work.v0)<data->range+5.5f && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<4.4f) {
                if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
            } else if(moby->proximity!=1 && func_002140B0_2E2EF0(9)==0) { moby->state=6; data->return_timer=0; }
        }
        moby->proximity=0;
        break;
    }
    case ANKLEBITER_MELEE_ATTACK:
        (*(AnklebiterMoby_2E2EF0 **)D_0013E633_2E2EF0)->anim_rate=1.3f;
        angle=func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]);
        func_L00_002592B0_2E2EF0((char *)moby,&data->turn_velocity,angle,0.05f,0.3f,0.2f);
        if(moby->anim_end==moby->anim && func_00215B18_2E2EF0((char *)moby,13.0f) && func_001F9B88_2E2EF0(moby->pos[2]-target->pos[2])<0.5f && func_001F9D48_2E2EF0(moby->pos,target->pos)<2.0f && func_001FA850_2E2EF0(func_L00_001FF860_2E2EF0(target->pos[0]-moby->pos[0],target->pos[1]-moby->pos[1]),moby->rot[2])<0.2617994f) {
            work.v0[0]=func_001F9F90_2E2EF0(moby->rot[2])*0.2f; work.v0[1]=func_001F9FA8_2E2EF0(moby->rot[2])*0.2f; work.v0[2]=0.0f;
            qcopy(work.v10,target->pos); work.v10[2]+=0.75f;
            func_L00_0025AC00_2E2EF0(target,(int)moby,1,work.v10,work.v0,1.0f);
        }
        if((moby->anim_flags&2)&&func_001F9D48_2E2EF0(moby->pos,target->pos)>=2.0f) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); }
        break;
    case ANKLEBITER_RETURN_HOME: {
        float return_distance;
        func_L12_002E41C8_2E2EF0((char *)moby,func_L00_001FF860_2E2EF0(data->home[0]-moby->pos[0],data->home[1]-moby->pos[1]));
        if(func_001F9D48_2E2EF0(moby->pos,data->home)<2.0f) {
            moby->state=1;
            if(moby->anim!=0) func_00213DE0_2E2EF0(moby,0,0,func_001F9850_2E2EF0(7));
        }
        return_distance=func_001F9D10_2E2EF0(target->pos,data->home); data->return_timer++; blocked=0;
        if(data->path!=-1) {
            char *path=D_L12_001B0C30_2E2EF0[data->path];
            blocked=func_L00_0025A778_2E2EF0(D_0013F3D0_r,path+0x10,*(int *)path)==0;
        }
        if(!blocked) {
            if(return_distance<data->range) {
                if(data->return_timer>func_001F9850_2E2EF0(70)) {
                    if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
                    moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby);
                }
            } else if(moby->proximity==1) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); }
        }
        moby->proximity=0;
        break;
    }
    case ANKLEBITER_KNOCKBACK_FALL:
        if(moby->group!=255) func_L01_0026F040_2E2EF0(moby->group,1);
        if(func_L00_0025D6F0_2E2EF0(moby,data->fall)&1) {
            moby->state=10;
            if(moby->anim!=13) func_00213DE0_2E2EF0(moby,13,8,func_001F9850_2E2EF0(8));
            return;
        }
        if(moby->pos[2]<27.0f) { func_0020D678_2E2EF0(moby); return; }
        break;
    case ANKLEBITER_CONTROLLED_MOTION_UNTIL_RETURN_HOME:
        if(func_L00_002DDEA0_2E2EF0(moby,data->fall)) { moby->state=6; data->return_timer=0; data->stuck=0; }
        break;
    case ANKLEBITER_RECOVER_AFTER_KNOCKBACK:
        if(height54(moby)>29.0f) { moby->state=4; data->blocked=0; func_L12_002E2BF8_2E2EF0((unsigned char *)moby); return; }
        break;
    case ANKLEBITER_DEATH_FALL:
        hit=func_L00_0025D6F0_2E2EF0(moby,data->fall);
        if((hit&0x40)||(height54(moby)>37.0f && moby->anim_end==moby->anim)) {
            moby->anim_rate=0.0f; func_L00_002584A8_2E2EF0(moby,0,-1); moby->state=15; moby->flags|=8;
        } else if(moby->pos[2]<10.0f) { func_L00_002584A8_2E2EF0(moby,0,-1); { func_0020D678_2E2EF0(moby); return; } }
        else if(hit&1) spawn_type_52_particle_near_moby((char *)moby);
        break;
    case ANKLEBITER_DEATH_EFFECTS_AND_REMOVE:
        spawn_type_52_particle_near_moby((char *)moby); func_L12_002E43A8_2E2EF0(moby); func_L12_002E43A8_2E2EF0(moby); { func_0020D678_2E2EF0(moby); return; }
    }
    if(moby->enabled && moby->state!=9 && moby->state!=8) {
        func_L00_00263950_2E2EF0((char *)moby,data->collision_data,0,D_0015ED64*0.018f,D_0015ED64*0.3f);
        if(moby->enabled && func_001F9D10_2E2EF0(moby->pos,D_L12_00167240)<28.0f) { func_L00_0025B178_2E2EF0(moby); moby->active=22; }
    }
    return;
}
/* Moves the moby along its path at the given speed and keeps it on the path; returns the move result, or 2 when it has been stuck too long. */
#include "qcopy.h"
extern char *D_L12_001B0930_w[] __asm__("D_L12_001B0930");
extern float D_0015ED6C_w __asm__("D_0015ED6C");
extern void ease_hoven_anklebiter_height_to_ground(char *) __asm__("FUN_L12_002e1878");
extern void approach_w(char *, float *, float, float, float, float) __asm__("FUN_L00_00258278");
extern float cos_w(float) __asm__("FUN_001f9dc8");
extern float sin_w(float) __asm__("FUN_001f9de0");
extern int move_w(void *, void *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern void sub_w(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_w(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_w(void *, void *, void *) __asm__("FUN_001f9a10");
extern int path_w(int, void *, void *, void *) __asm__("FUN_L00_00261b48");
extern int inside_w(void *, void *, int) __asm__("FUN_L00_00259740");
extern float dist_w(void *, void *) __asm__("FUN_001f9b48");
extern int frames_w(int) __asm__("FUN_001f96f8");

int move_hoven_anklebiter_toward_heading(struct Moby *moby, float speed) __asm__("FUN_L12_002e2eb8");

int move_hoven_anklebiter_toward_heading(struct Moby *moby, float speed) {
    char *moby_vars = (char *)moby->pvars;
    float old[4] __attribute__((aligned(16)));
    float forward_movement_vector[4];
    float hit[4];
    float path_corrected_position[4] __attribute__((aligned(16)));
    float path_test_position[4];
    float path_test_offset[4];
    int move_result;
    *(OvlQuad *)old = *(OvlQuad *)&moby->pos;
    ease_hoven_anklebiter_height_to_ground(moby);
    approach_w(moby, (float *)(moby_vars + 0x2A0), speed, 0.05f, 0.3f, 0.2f);
    forward_movement_vector[0] = cos_w(moby->rot.z) * 2.0f;
    forward_movement_vector[1] = sin_w(moby->rot.z) * 2.0f;
    forward_movement_vector[2] = 0.0f;
    move_result = move_w(moby, moby_vars + 0x180, forward_movement_vector, hit, 1.0f);
    if (*(int *)(moby_vars + 0x2D0) != -1) {
        sub_w(path_test_offset, old, &moby->pos);
        norm_w(path_test_offset, path_test_offset, 0.05f);
        add_w(path_test_position, old, path_test_offset);
        if (path_w(*(int *)(moby_vars + 0x2D0), &moby->pos, path_test_position, path_corrected_position) != 0) {
            char *path = D_L12_001B0930_w[*(int *)(moby_vars + 0x2D0)];
            if (inside_w(&moby->pos, path + 0x10, *(int *)path) == 0)
                *(OvlQuad *)&moby->pos = *(OvlQuad *)path_corrected_position;
            if (dist_w(&moby->pos, old) < D_0015ED6C_w * 0.5f) {
                *(int *)(moby_vars + 0x2D4) += 1;
                if (frames_w(10) < *(int *)(moby_vars + 0x2D4))
                    move_result = 2;
            } else {
                *(int *)(moby_vars + 0x2D4) = 0;
            }
        }
    }
    return move_result;
}
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/state/item_state.h"

extern int D_L12_0016183C __attribute__((sda));
extern int D_L12_00161840 __attribute__((sda));
extern float D_L12_00161844 __attribute__((sda));
extern float D_L12_00161848 __attribute__((sda));
extern float D_L12_0016184C __attribute__((sda));
extern float D_L12_00161850 __attribute__((sda));
extern float D_L12_00161854 __attribute__((sda));
extern float D_L12_00161858 __attribute__((sda));
extern float D_L12_0016185C __attribute__((sda));
extern float D_L12_00161860 __attribute__((sda));
extern float D_L12_00161864 __attribute__((sda));
extern float D_L12_00161868 __attribute__((sda));
extern int D_L12_0016186C __attribute__((sda));
extern int D_L12_00161870 __attribute__((sda));
extern int D_L12_00161874 __attribute__((sda));
extern int D_L12_00161878 __attribute__((sda));
extern int D_L12_0016187C __attribute__((sda));
extern int D_L12_00161880 __attribute__((sda));
extern int D_L12_00161884 __attribute__((sda));
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern float random_float_between_e3098(float, float) __asm__("FUN_002132a8");
extern float fast_cos_e3098(float) __asm__("FUN_001f9dc8");
extern float fast_sin_e3098(float) __asm__("FUN_001f9de0");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_vector_xyz_e3098(void *, void *, void *) __asm__("FUN_001f9a10");
extern int scale_game_frames_e3098(int) __asm__("FUN_001f96f8");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern int truncate_float(float) __asm__("FUN_001fa6d0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern char *spawn_particle(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void FUN_L12_002e3098(struct Moby *moby);
void FUN_L12_002e3098(struct Moby *moby) {
    float pos[4];
    float vel[4];
    float accel[4];
    float push[4];
    char *vars;
    int count;
    float *pos_ptr;
    void *src;
    int i;
    int j;
    float angle;
    float speed;
    float rise;
    float size;
    float time_base;
    int start_color;
    int end_color;
    int life;
    int fade;

    count = D_L12_0016183C;
    vars = (char *)moby->pvars;
    if (moby->state == 15)
        count = truncate_float((float)count * 1.5f);
    for (i = 0; i < count; i++) {
        pos_ptr = pos;
        src = &moby->pos;
        qcopy_nc(pos, src);
        add_random_offset_to_vector_xyz(pos_ptr, D_L12_00161844);
        pos[2] += 0.55f;
        angle = random_angle_radians();
        speed = random_float_between_e3098(D_L12_00161850, D_L12_00161854);
        rise = random_float_between_e3098(D_L12_00161858, D_L12_0016185C);
        if (moby->state == 15) {
            speed *= 0.55f;
            rise *= 0.55f;
            pos[2] -= 0.2f;
        }
        vel[0] = fast_cos_e3098(angle) * (speed * D_0015ED6C);
        vel[1] = fast_sin_e3098(angle) * (speed * D_0015ED6C);
        vel[2] = 0.0f + rise * D_0015ED6C;
        scale_vector_xyz(push, vars + 0x120, 0.5f);
        push[2] = 0.0f;
        add_vector_xyz_e3098(vel, vel, push);
        qcopy(accel, vel);
        accel[2] -= D_L12_00161860 * D_0015ED70 * (float)scale_game_frames_e3098(D_L12_00161880);
        for (j = 0; j < D_L12_00161840; j++) {
            size = random_float_between_e3098(0.5f, 1.5f);
            vel[3] = size * D_L12_00161864;
            accel[3] = size * D_L12_00161868;
            add_random_offset_to_vector_xyz(pos, D_L12_00161848);
            add_random_offset_to_vector_xyz(accel, D_L12_0016184C * D_0015ED6C);
            start_color = fast_tween_color(D_L12_0016186C, D_L12_00161874, random_float_between_e3098(0.0f, 1.0f));
            end_color = fast_tween_color(D_L12_00161870, D_L12_00161878, random_float_between_e3098(0.0f, 1.0f));
            life = scale_game_frames_e3098(D_L12_0016187C);
            fade = scale_game_frames_e3098(D_L12_00161880);
            time_base = (float)D_L12_00161884;
            spawn_particle(pos, vel, accel, start_color, end_color, life, fade,
                truncate_float(scale_time(random_float_between_e3098(time_base * 0.5f, time_base * 2.5f))), -1);
        }
    }
}
#include "qcopy.h"
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/vector.h"

/* Variables of the Hoven anklebiter (moby->pvars); the l00 ElectroidVars damage layout moved by 0xB0. */
typedef struct {
    u8 pad0[0x20];
    f32 health;          /* 0x20 */
    u8 pad24[2];
    s16 stun_timer;      /* 0x26 */
    u8 pad28[0x10];
    s32 reset_alert;     /* 0x38 */
    u8 pad3C[0xD4];
    u8 anim[7];          /* 0x110: animation blend state */
    u8 unk117;           /* 0x117 */
    u8 pad118[8];
    u8 fall[0x10];       /* 0x120 */
    f32 unk130;          /* 0x130 */
    f32 unk134;          /* 0x134 */
    f32 unk138;          /* 0x138 */
    f32 unk13C;          /* 0x13C */
    u8 pad140[4];
    s32 unk144;          /* 0x144 */
    u8 pad148[8];
    f32 unk150;          /* 0x150 */
    f32 unk154;          /* 0x154 */
    f32 unk158;          /* 0x158 */
    u8 pad15C;
    u8 unk15D;           /* 0x15D */
    u8 pad15E[0x12];
    f32 unk170;          /* 0x170 */
    f32 unk174;          /* 0x174 */
    u8 pad178[0x134];
    f32 speed;           /* 0x2AC */
    u8 pad2B0[0x14];
    s32 alert_timer;     /* 0x2C4 */
    u8 pad2C8[0x20];
    s16 call_enabled;    /* 0x2E8 */
    s16 call_timer;      /* 0x2EA */
} AnklebiterVars_e3528;

typedef struct {
    u8 pad0[0x10];
    Vec4 pos;
} AnklebiterHit_e3528;

extern f32 D_0015ED6C_e3528 __asm__("D_0015ED6C");
extern s32 D_001413D0_e3528 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 scale_game_frames_e3528(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown_e3528(s32 *) __asm__("FUN_001f9740");
extern s32 random_integer_below_e3528(s32) __asm__("FUN_00213260");
extern f32 fast_add_rotations_e3528(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_cos_e3528(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_e3528(f32) __asm__("FUN_001f9de0");
extern s32 count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern void alert_group_e3528(s32, s32) __asm__("FUN_L01_0026e090");
extern void call_for_help_e3528(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern AnklebiterHit_e3528 *find_hit_e3528(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void take_damage_e3528(struct Moby *, AnklebiterHit_e3528 *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void knockback_e3528(void *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void start_fall_e3528(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void anim_reset_e3528(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_e3528(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L12_002e3098(struct Moby *);

extern f32 D_L12_001618D0 __attribute__((sda));
extern f32 D_L12_001618D4 __attribute__((sda));
/* Damage and alert step of the Hoven anklebiter: refreshes its speed while alerted, now and then calls for help behind
   itself, and takes hits (dying, or staggering back). */
void process_hoven_anklebiter_damage_and_alerts(struct Moby *moby) __asm__("FUN_L12_002e3528");

void process_hoven_anklebiter_damage_and_alerts(struct Moby *moby) {
    AnklebiterVars_e3528 *vars = (AnklebiterVars_e3528 *)moby->pvars;
    Vec4 help_direction_or_hit_position;
    s32 hit;
    f32 dmg;
    f32 angle;
    f32 unused;
    AnklebiterHit_e3528 *coll;

    if (vars->reset_alert != 0) {
        vars->reset_alert = 0;
        vars->alert_timer = scale_game_frames_e3528(0xF0);
    }
    if (moby->state != 9 && moby->state != 0xB && moby->state != 0) {
        if (tick_countdown_e3528(&vars->alert_timer)) {
            vars->speed = 12.0f;
        } else {
            vars->speed = 20.0f;
        }
    }
    if (vars->call_enabled != 0 && moby->state != 9) {
        s16 updated_call_timer = ++vars->call_timer;
        if (scale_game_frames_e3528(0x3C) * 15 < updated_call_timer && moby->visible == 0 && moby->group < 0xFF &&
            count_active_moby_group_members(moby->group, -1) >= 4 && random_integer_below_e3528(0x45) == 0) {
            help_direction_or_hit_position.f[0] = fast_cos_e3528(fast_add_rotations_e3528(moby->rot.z, 3.1415927f));
            help_direction_or_hit_position.f[1] = fast_sin_e3528(fast_add_rotations_e3528(moby->rot.z, 3.1415927f));
            help_direction_or_hit_position.f[2] = 0.0f;
            call_for_help_e3528(moby, D_001413D0_e3528, 0x10000, &moby->pos, help_direction_or_hit_position.f, 1.0f);
        }
    }
    coll = find_hit_e3528(moby, 0x330000, 0);
    dmg = 0.0f;
    take_damage_e3528(moby, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (coll != 0 && moby->state != 0x63 && moby->state != 8) {
        if (moby->group != 0xFF) {
            alert_group_e3528(moby->group, 7);
        }
        if (dmg != 0.0f) {
            vars->health -= dmg;
            vars->unk130 = 0.008f;
            vars->unk134 = 0.0005f;
            vars->unk138 = D_L12_001618D4 * D_0015ED6C_e3528;
            vars->unk13C = D_L12_001618D0 * D_0015ED6C_e3528;
            vars->unk15D = 0;
            vars->unk144 = 9;
            vars->unk150 = 1.0f;
            vars->unk154 = 1.0f;
            vars->unk158 = 1.0f;
            if (vars->health <= 0.0f) {
                moby->flags &= ~0x1000;
                vars->unk138 = D_0015ED6C_e3528 * 8.0f;
                vars->unk13C = D_0015ED6C_e3528 * 10.0f;
                help_direction_or_hit_position.q = coll->pos.q;
                knockback_e3528(&help_direction_or_hit_position, &angle, &vars->unk138, &vars->unk13C);
                start_fall_e3528(moby, vars->fall, 6, 1, 0, angle);
                vars->unk170 = 11.0f;
                vars->unk174 = 18.0f;
                moby->state = 0x63;
                vars->unk117 = 0x78;
                anim_reset_e3528(moby, vars->anim);
            } else {
                help_direction_or_hit_position.q = coll->pos.q;
                knockback_e3528(&help_direction_or_hit_position, &unused, &vars->unk138, &vars->unk13C);
                start_fall_e3528(moby, vars->fall, 0xE, 1, 0, unused);
                vars->unk174 = vars->unk170 = -1.0f;
                moby->state = 7;
                vars->unk117 = 0xFA;
                vars->stun_timer = scale_game_frames_e3528(0x3C);
                anim_reset_e3528(moby, vars->anim);
            }
            FUN_L12_002e3098(moby);
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_e3528(moby, vars->anim);
}
/* Hoven engine exhaust: emits a flame and a smoke puff from each of the moby's two nozzles (the second one
 * mirrored across its local Y axis), with random jitter. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E4C58), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
int FUN_L00_00257b90(int lo, int hi);
unsigned char *FUN_L00_0026d000(void *pos, int n, void *vel, int color, float r, float lo, float hi, float size);
extern void *func_L00_0026DEA0_x(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

float D_L12_00161920[4] __attribute__((section(".sdata"))) = {-1.5f, 0.8f, -0.5f, 0.0f};
void emit_platform_exhaust_effects(struct Moby *moby) __asm__("FUN_L12_002e3948");

void emit_platform_exhaust_effects(struct Moby *moby) {
    float emission_direction[4];
    float secondary_emission_direction[4];
    float emission_position[4];
    float mirrored_emission_position[4];
    float z[4];
    if (moby->visible == 0) return;
    clear_u64_value_c(z);
    emission_direction[0] = -random_float_between(0.0f, frame_time * 5.0f);
    emission_direction[1] = random_float_between(-(frame_time * 2.0f), frame_time * 2.0f);
    emission_direction[2] = random_float_between(-(frame_time * 2.0f), frame_time * 2.0f);
    transform_vector_by_basis(emission_direction, emission_direction, &moby->rot_row0);
    transform_vector_by_basis(emission_position, &D_L12_00161920, &moby->rot_row0);
    add_vector_xyz_c(emission_position, emission_position, &moby->pos);
    secondary_emission_direction[0] = -random_float_between(0.0f, frame_time);
    secondary_emission_direction[1] = random_float_between(-frame_time, frame_time);
    secondary_emission_direction[2] = random_float_between(-frame_time, frame_time);
    transform_vector_by_basis(secondary_emission_direction, secondary_emission_direction, &moby->rot_row0);
    qcopy(mirrored_emission_position, &D_L12_00161920);
    mirrored_emission_position[1] = -mirrored_emission_position[1];
    transform_vector_by_basis(mirrored_emission_position, mirrored_emission_position, &moby->rot_row0);
    add_vector_xyz_c(mirrored_emission_position, mirrored_emission_position, &moby->pos);
    FUN_L00_0026cbb0(emission_position, emission_direction, 0x4F007FFF, 0x1FFFFFFF, FUN_L00_00257b90(scale_game_frames(8), scale_game_frames(0x11)), 1, 10000.0f);
    func_L00_0026DEA0_x(emission_position, 0.05f, 1.01f, 1.03f, 6, secondary_emission_direction, 30000.0f, 0x404040);
    FUN_L00_0026cbb0(mirrored_emission_position, emission_direction, 0x4F007FFF, 0x1FFFFFFF, FUN_L00_00257b90(scale_game_frames(8), scale_game_frames(0x11)), 1, 10000.0f);
    func_L00_0026DEA0_x(mirrored_emission_position, 0.05f, 1.01f, 1.03f, 6, secondary_emission_direction, 30000.0f, 0x404040);
}
/* Steers the moby toward a target point, speeding up or braking, and turns it toward the given angles. */
extern float D_0015ED6C_k __asm__("D_0015ED6C");
extern float D_0015ED70_k __asm__("D_0015ED70");
extern float D_0015ED64_k __asm__("D_0015ED64");
extern void sub_k(void *, void *, void *) __asm__("FUN_001f9a28");
extern float len_k(void *) __asm__("FUN_001f9af0");
extern void norm_k(void *, void *, float) __asm__("FUN_001f9bf8");
extern float diff_k(float, float) __asm__("FUN_001fa688");
extern void appr_k(void *, float, float) __asm__("FUN_00213ed8");
extern float turn_k(float *, float, float *, int, float, float, float) __asm__("FUN_L00_0025bc98");
extern void add_k(void *, void *, void *) __asm__("FUN_001f9a10");

void steer_platform_toward_target(char *moby, float *target_angles, void *target, int unused, int brake) __asm__("FUN_L12_002e3c08");

void steer_platform_toward_target(char *moby, float *target_angles, void *target, int unused, int brake) {
    char *moby_vars = *(char **)(moby + 0x78);
    float target_offset[4];
    float target_distance;
    float speed;
    float acc;
    float yaw_angular_velocity_limit = 1.9198622f;
    sub_k(target_offset, target, moby + 0x10);
    target_distance = len_k(target_offset);
    speed = len_k(moby_vars + 0x40);
    if (brake) {
        if (speed == 0.0f)
            speed = D_0015ED6C_k * 0.01f;
        acc = *(float *)(moby_vars + 0xD4) * D_0015ED70_k;
        if (acc < speed * speed / (target_distance + target_distance))
            speed -= acc;
        else if (speed < *(float *)(moby_vars + 0xD0) * D_0015ED6C_k)
            speed += acc;
    } else {
        if (speed < *(float *)(moby_vars + 0xD0) * D_0015ED6C_k)
            speed += *(float *)(moby_vars + 0xD4) * D_0015ED70_k;
        else if (*(float *)(moby_vars + 0xD0) * D_0015ED6C_k < speed)
            speed -= *(float *)(moby_vars + 0xD4) * D_0015ED70_k;
    }
    norm_k(moby_vars + 0x40, target_offset, speed);
    appr_k(moby + 0x40, target_angles[0], diff_k(target_angles[0], *(float *)(moby + 0x40)) / platform_tilt_approach_divisor);
    appr_k(moby + 0x44, target_angles[1], diff_k(target_angles[1], *(float *)(moby + 0x44)) / platform_tilt_approach_divisor);
    turn_k((float *)(moby + 0x48), target_angles[2], (float *)(moby_vars + 0xF8), 0, D_0015ED64_k * 0.005f, D_0015ED64_k * 0.3f,
           D_0015ED6C_k * yaw_angular_velocity_limit);
    add_k(moby + 0x10, moby + 0x10, moby_vars + 0x40);
}
/* Projects a moby's offset from a reference point into a 2D pair, or zeroes it. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E5138), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");

void compute_platform_target_tilt(struct Moby *moby, float *out, int on) __asm__("FUN_L12_002e3e28");

void compute_platform_target_tilt(struct Moby *moby, float *out, int on) {
    float inverse_yaw_angles[4];
    float reference_offset[4];
    float inverse_yaw_matrix[4];
    float pad[12];
    float offset_scale_divisor;
    if (on) {
        fast_vec_sub(reference_offset, D_0013E533 + 0xE9D, &moby->pos);
        clear_u64_value(inverse_yaw_angles);
        inverse_yaw_angles[2] = -moby->rot.z;
        euler_rotation_matrix(inverse_yaw_matrix, inverse_yaw_angles);
        transform_vector(reference_offset, reference_offset, inverse_yaw_matrix);
        offset_scale_divisor = platform_tilt_offset_divisor;
        out[1] = reference_offset[0] / offset_scale_divisor;
        out[0] = -reference_offset[1] / offset_scale_divisor;
    } else {
        out[0] = 0;
        out[1] = 0;
    }
}
/* Update of the Hoven flying platform (class 240): follows or eases along its path, bobs, falls, and reports its motion. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E51E8), where it is exact; names translated to the US level program. */


typedef struct { float x, y, z, w; } Node_2E51E8;

typedef struct {
    int n;
    int pad4[3];
    Node_2E51E8 nodes[1];
} Path_2E51E8;

typedef struct {
    char pad0[0x10];
    int f10;
} Class_2E51E8;

typedef struct {
    int idx;
    signed char dir;
    char pad5[0xB];
    Path_2E51E8 *path;
    int pathIdx;
} Walk_2E51E8;

typedef struct {
    char pad0[0x3E];
    unsigned short f3E;
    float vel[4];
    char pad50[0x10];
    char f60[0x40];
    Walk_2E51E8 walk;
    char padB8[0x18];
    float fD0;
    float fD4;
    float fD8;
    int fDC;
    int timer;
    int kind;
    float fE8;
    float fEC;
    int fF0;
    char padF4[8];
    int fFC;
} Data_2E51E8;

extern char *D_L12_001B0930[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 vector_length_xyz_c2(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern int FUN_L08_002025d0(void *, int, int, void *, int);
extern s32 count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern s32 tick_countdown_32_alt_c(s32 *) __asm__("FUN_001f9740");
extern void FUN_L00_0025b6b8(void *, void *, void *, float);
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void add_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value_c2(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L12_002e3948_c(struct Moby *moby) __asm__("FUN_L12_002e3948");
void FUN_L12_002e3c08_c(char *m, float *ang, void *target, int unused, int brake) __asm__("FUN_L12_002e3c08");
void FUN_L12_002e3e28_c(struct Moby *moby, float *out, int on) __asm__("FUN_L12_002e3e28");
extern Path_2E51E8 *D_L12_001B0C30_2E51E8[] __asm__("D_L12_001B0930");
extern void func_L12_002E4F18_2E51E8(void *, void *, void *, int, int) __asm__("FUN_L12_002e3c08");

void platform_update(Moby *moby) __asm__("FUN_L12_002e3ed8");
void platform_update(Moby *moby) {
    enum {
        PLATFORM_INITIALIZE = 0,
        PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS = 2,
        PLATFORM_WAIT_TO_RESUME_OSCILLATION = 3,
        PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH = 4,
        PLATFORM_WAIT_TO_RESUME_FIXED_PITCH_OSCILLATION = 5,
        PLATFORM_ACCELERATE_UPWARD_UNTIL_NOT_VISIBLE = 6,
        PLATFORM_FOLLOW_WAYPOINTS = 7,
        PLATFORM_FALL_AND_TUMBLE = 8,
        PLATFORM_TRACK_REFERENCED_MOBY_HEIGHT = 9
    };
    enum {
        PLATFORM_KIND_FOLLOW_WAYPOINTS = 0,
        PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS = 1,
        PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH = 2,
        PLATFORM_KIND_DISABLE_DRAWING_AND_COLLISION = 3,
        PLATFORM_KIND_TRACK_REFERENCED_MOBY_HEIGHT = 4
    };
    float oldPos[4];
    float oldRot[4];
    float delta[4];
    float angles[4];
    float goal[4];
    float to_goal[4];
    Data_2E51E8 *data;

    data = (*(Data_2E51E8 **)&moby->pvars);
    qcopy(oldPos, (*(float (*)[4])&moby->pos));
    qcopy(oldRot, (*(float (*)[4])&moby->rot));
    if (moby->state != 0 && data->fFC != -1 && (signed char)(*(Moby * *)&D_L12_0015FFD8_p)[data->fFC].state >= 0) {
        FUN_L12_002e3948_c((char *)moby);
        return;
    }
    if (moby->state != 0 && data->fDC != -1 && count_active_moby_group_members(data->fDC, -1) != 0) {
        FUN_L12_002e3948_c((char *)moby);
        return;
    }
    switch (moby->state) {
    case PLATFORM_INITIALIZE:
        switch (data->kind) {
        case PLATFORM_KIND_FOLLOW_WAYPOINTS:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            data->walk.path = D_L12_001B0C30_2E51E8[data->walk.pathIdx];
            data->walk.idx = 0;
            data->walk.dir = 1;
            if (data->fD0 == 0.0f) data->fD0 = 6.0f;
            if (data->fD4 == 0.0f) data->fD4 = 6.0f;
            data->f3E |= 9;
            clear_u64_value_c2(data->vel);
            moby->state = 7;
            break;
        case PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            data->walk.path = D_L12_001B0C30_2E51E8[data->walk.pathIdx];
            data->walk.idx = 0;
            data->walk.dir = 1;
            if (data->fD4 == 0.0f) data->fD4 = 2.0f;
            if (data->fD0 == 0.0f) data->fD0 = 4.0f;
            data->fD8 = random_angle_radians_c();
            data->f3E |= 9;
            clear_u64_value_c2(data->vel);
            moby->state = 2;
            break;
        case PLATFORM_KIND_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH:
            (*(int *)&moby->collision) = (*(Class_2E51E8 **)&moby->pclass)->f10;
            (*(float (*)[4])&moby->rot)[1] = -1.5707964f;
            (*(float (*)[4])&moby->rot)[2] = (*(float (*)[4])&moby->rot)[2] + 3.1415927f;
            data->walk.path = D_L12_001B0C30_2E51E8[data->walk.pathIdx];
            data->walk.idx = 0;
            data->walk.dir = 1;
            if (data->fD4 == 0.0f) data->fD4 = 8.0f;
            if (data->fD0 == 0.0f) data->fD0 = 8.0f;
            clear_u64_value_c2(data->vel);
            moby->state = 4;
            break;
        case PLATFORM_KIND_DISABLE_DRAWING_AND_COLLISION:
            moby->draw_dist = 0;
            moby->state = 1;
            (*(int *)&moby->collision) = 0;
            break;
        case PLATFORM_KIND_TRACK_REFERENCED_MOBY_HEIGHT:
            moby->state = 9;
            data->fD4 = 4.0f;
            data->fD0 = 4.0f;
            data->f3E &= 0xFFFE;
            break;
        }
        break;
    case PLATFORM_FOLLOW_WAYPOINTS: {
        Walk_2E51E8 *w = &data->walk;
        Path_2E51E8 *path;
        int next;
        FUN_L12_002e3948_c((char *)moby);
        path = w->path;
        next = (w->idx + path->n + w->dir) % path->n;
        qcopy(goal, &path->nodes[next]);
        if (distance_xyz((*(float (*)[4])&moby->pos), &path->nodes[next]) < 0.5f) {
            data->walk.idx = next;
            if (data->walk.path->nodes[next].w != -1.0f) {
                data->fD0 = data->walk.path->nodes[next].w;
            }
        }
        subtract_vector_xyz_c2(to_goal, goal, (*(float (*)[4])&moby->pos));
        angles[2] = fast_arc_tan(to_goal[0], to_goal[1]);
        if (is_hero_on_moby(moby)) {
            FUN_L12_002e3e28_c((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, goal, 1, 0);
        } else {
            FUN_L12_002e3e28_c((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, goal, 0, 0);
        }
        break;
    }
    case PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS: {
        Path_2E51E8 *path = data->walk.path;
        float swing;
        data->fD4 = 0.0f;
        data->fD8 = fast_add_rotations_c2(data->fD8, D_0015ED6C * 1.2217305f);
        swing = fast_sin_c(data->fD8);
        if (data->fD8 > 1.5707964f && data->fD8 < 3.1415927f) swing = 2.0f - swing;
        if (data->fD8 > -3.1415927f && data->fD8 < -1.5707964f) swing = 2.0f - swing;
        swing = swing * 0.5f;
        swing = swing + 0.5f;
        if (swing > 1.0f) swing = swing - 2.0f;
        swing = absolute_float(swing);
        *(u128 *)angles = *(u128 *)&path->nodes[0];
        *(u128 *)goal = *(u128 *)&path->nodes[1];
        FUN_L00_0025b6b8((*(float (*)[4])&moby->pos), angles, goal, swing);
        angles[2] = (*(float (*)[4])&moby->rot)[2];
        if (is_hero_on_moby(moby)) {
            FUN_L12_002e3e28_c((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 1, 1);
        } else {
            FUN_L12_002e3e28_c((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 0, 1);
        }
        break;
    }
    case PLATFORM_OSCILLATE_BETWEEN_PATH_ENDPOINTS_FIXED_PITCH: {
        Path_2E51E8 *path = data->walk.path;
        float swing;
        data->fD8 = fast_add_rotations_c2(data->fD8, D_0015ED6C * 1.2217305f);
        swing = fast_sin_c(data->fD8);
        if (data->fD8 > 1.5707964f && data->fD8 < 3.1415927f) swing = 2.0f - swing;
        if (data->fD8 > -3.1415927f && data->fD8 < -1.5707964f) swing = 2.0f - swing;
        swing = swing * 0.5f;
        swing = swing + 0.5f;
        if (swing > 1.0f) swing = swing - 2.0f;
        swing = absolute_float(swing);
        *(u128 *)angles = *(u128 *)&path->nodes[0];
        *(u128 *)goal = *(u128 *)&path->nodes[1];
        FUN_L00_0025b6b8((*(float (*)[4])&moby->pos), angles, goal, swing);
        (*(float (*)[4])&moby->rot)[1] = -1.5707964f;
        break;
    }
    case PLATFORM_WAIT_TO_RESUME_OSCILLATION:
        angles[2] = (*(float (*)[4])&moby->rot)[2];
        if (tick_countdown_32_alt_c(&data->timer)) {
            if (data->walk.dir == 1) {
                data->walk.dir = 2;
            } else {
                data->walk.dir = 1;
            }
            moby->state = 2;
        }
        if (is_hero_on_moby(moby)) {
            FUN_L12_002e3e28_c((char *)moby, angles, 1);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 1, 1);
        } else {
            FUN_L12_002e3e28_c((char *)moby, angles, 0);
            func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 0, 1);
        }
        break;
    case PLATFORM_WAIT_TO_RESUME_FIXED_PITCH_OSCILLATION:
        if (tick_countdown_32_alt_c(&data->timer)) {
            if (data->walk.dir == 1) {
                data->walk.dir = 2;
            } else {
                data->walk.dir = 1;
            }
            moby->state = 4;
        }
        if (data->walk.dir == 2) {
            FUN_L12_002e3948_c((char *)moby);
        }
        qcopy(angles, (*(float (*)[4])&moby->rot));
        func_L12_002E4F18_2E51E8(moby, angles, (*(float (*)[4])&moby->pos), 0, 1);
        break;
    case PLATFORM_ACCELERATE_UPWARD_UNTIL_NOT_VISIBLE:
        if (vector_length_xyz_c2(data->vel) < D_0015ED6C * 6.0f) {
            data->vel[2] = data->vel[2] + D_0015ED70 * 6.0f;
        }
        add_vector_xyz_c3((*(float (*)[4])&moby->pos), (*(float (*)[4])&moby->pos), data->vel);
        if (moby->visible == 0) {
            mark_moby_for_removal_c(moby);
        }
        break;
    case PLATFORM_FALL_AND_TUMBLE:
        add_vector_xyz_c3((*(float (*)[4])&moby->pos), (*(float (*)[4])&moby->pos), data->vel);
        if (FUN_L08_002025d0((*(float (*)[4])&moby->pos), 0x600, 0, moby, 0) && data->walk.path == 0) {
            FUN_L00_0025e450_c(moby, data->vel, 0, 2.0f, 0.0f, 10, 3, 16, 4.0f, 2.0f, 9.0f, -1, 1.0f, 15.0f, 1, 1, -1, 0);
            mark_moby_for_removal_c(moby);
            return;
        }
        data->vel[2] = data->vel[2] - D_0015ED70 * 7.0f;
        (*(float (*)[4])&moby->rot)[0] = fast_add_rotations_c2(data->fE8, (*(float (*)[4])&moby->rot)[0]);
        (*(float (*)[4])&moby->rot)[1] = fast_add_rotations_c2(data->fEC, (*(float (*)[4])&moby->rot)[1]);
        if (data->walk.path != 0 && vector_distance((*(float (*)[4])&moby->pos), &data->walk.path->nodes[0]) < 0.5f) {
            moby->state = 0;
        }
        if ((*(float (*)[4])&moby->pos)[2] < 0.0f) {
            mark_moby_for_removal_c(moby);
        }
        break;
    case PLATFORM_TRACK_REFERENCED_MOBY_HEIGHT: {
        Moby *height_reference_moby = (Moby *)((char *)(*(Moby * *)&D_L12_0015FFD8_p) + (data->fF0 << 8));
        qcopy(angles, (*(float (*)[4])&moby->pos));
        angles[2] = (*(float (*)[4])&height_reference_moby->pos)[2];
        clear_u64_value_c2(goal);
        goal[2] = (*(float (*)[4])&moby->rot)[2];
        if (is_hero_on_moby(moby)) {
            angles[2] = angles[2] + 0.2f;
            FUN_L12_002e3e28_c((char *)moby, goal, 1);
            func_L12_002E4F18_2E51E8(moby, goal, angles, 1, 1);
        } else {
            angles[2] = angles[2] + 0.4f;
            FUN_L12_002e3e28_c((char *)moby, goal, 0);
            func_L12_002E4F18_2E51E8(moby, goal, angles, 0, 1);
        }
        break;
    }
    }
    subtract_vector_xyz_c2(delta, (*(float (*)[4])&moby->pos), oldPos);
    update_entity_matrix(data->f60, delta, oldRot, (*(float (*)[4])&moby->rot));
}
/* Level overlay code after gameplay/vendor; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

#include "qcopy.h"

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad00[0xC];
    float radius;
    char pad10[0x30];
} L16WatchActivation;

typedef struct {
    char pad00[0x20];
    L16WatchActivation activation;
    char body[0x60];
    float padC0;
    float pitch;
    float yaw;
    float padCC;
    float height;
    char padD4[0xC];
    char head[0x60];
    char pad140[8];
    float head_yaw;
    char pad14C[0x14];
    float target[4];
    int region;
    char pad174[4];
    int moving_timer;
    int target_timer;
} L16WatchData;

typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0xF];
    unsigned char opacity;
    char pad31[0x17];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x24];
    L16WatchData *data;
    char pad7C[0x34];
    unsigned char widget;
} L16WatchMoby;

typedef struct {
    char pad00[0x70];
    unsigned char entered;
    unsigned char mission;
} L16WatchFlags;

extern char D_0013E533[];
extern char D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9e90_cf(float, float) __asm__("FUN_001fa688");
extern float FUN_001fa688_cf(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float);
extern float truncate_float_to_s32_cf(float, float) __asm__("FUN_002132a8");
extern float vector_length_xy(void *);
extern float vector_length_xyz(void *);
extern void FUN_L01_002783a8_c(void *, float) __asm__("FUN_L01_002783a8");
extern unsigned char alternate_item_available[] __asm__("D_0013D388");
extern int D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern void FUN_0020c828_c(void *) __asm__("FUN_0020c828");
extern void initialize_moby_grounding(void *) __asm__("FUN_L02_0025c758");
extern char *func_0020D348_m_x(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8_x(void *) __asm__("FUN_L00_00250df8");
extern void update_moby_bone_attachment(void *, void *, int) __asm__("FUN_L12_0027b9c0");
extern void FUN_L00_00263d40_c(int, int) __asm__("FUN_L00_00263d40");
extern int update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern int initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int memcard_save_data(int, int) __asm__("FUN_0020b178");
extern int random_float_between_cf(float) __asm__("FUN_001fa6d0");
extern int scale_game_frames(int);
extern int tick_countdown_32(int *);
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void apply_npc_big_head_cheat(char *) __asm__("FUN_L12_002e6b60");
extern void add_vector_xyz(void *, void *, void *);
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void subtract_vector_xyz(void *, void *, void *);
extern L16WatchPlayer D_0013E633_watch __asm__("D_0013E533");

extern char *D_L12_001600EC;
void miner_update(L16WatchMoby *m) __asm__("FUN_L12_002e6c48");
void miner_update(L16WatchMoby *moby) {
    enum {
        MINER_INITIALIZE = 0,
        MINER_UPDATE_DIALOGUE = 1,
        MINER_APPLY_DIALOGUE_COMPLETION = 2
    };
    L16WatchScratch scratch;
    L16WatchData *moby_data = moby->data;
    float rate;
    float head_rate;
    int tracking;
    apply_npc_big_head_cheat((char *)moby);
    switch (moby->state) {
    case MINER_INITIALIZE:
        if (alternate_item_available[1] != 0) {
            FUN_0020c828_c(moby);
            return;
        }
        moby->state = 1;
        initialize_npc_dialogue(moby, &moby_data->activation);
        initialize_moby_grounding(moby);
        break;
    case MINER_UPDATE_DIALOGUE:
        if (alternate_item_available[1] == 0 && update_npc_dialogue(moby, &moby_data->activation)) {
            FUN_L01_002783a8_c(moby, 2.5f);
            *(int *)((char *)moby + 0x58) = 0;
            moby->state = 2;
        }
        break;
    case MINER_APPLY_DIALOGUE_COMPLETION:
        if (D_L12_0015F5C4 != 2) {
            *(float *)((char *)moby + 0x58) = 1.0f;
            alternate_item_available[1] = 1;
            FUN_L00_00263d40_c(0x2EE7, scale_game_frames(300));
            alternate_item_available[0x61] = 1;
            jet_clear_persistent(moby->widget);
            if (moby_data->region != -1) {
                char *p = D_L12_001600EC + (moby_data->region << 7);
                jet_reset_camera(p + 0x30, p + 0x70);
            }
            memcard_save_data(0, -1);
            moby->state = 1;
        }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance(moby->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                moby->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    moby->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    moby->position[1])) < 1.5707964f) {
            if (vector_length_xyz(player + 0x80) > 0.01f)
                moby_data->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32(&moby_data->moving_timer);
        } else if (moby_data->moving_timer) {
            moby_data->moving_timer = 0;
            qcopy(moby_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&moby_data->target_timer)) {
            float heading;
            moby_data->target_timer =
                random_float_between_cf(multiply_global_factor_ed64(truncate_float_to_s32_cf(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, truncate_float_to_s32_cf(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(moby_data->target, 6.0f, heading,
                                   truncate_float_to_s32_cf(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(moby_data->target, moby_data->target, moby->position);
        }
        if (moby_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        subtract_vector_xyz(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_data->pitch = pitch;
        moby_data->yaw = yaw * 0.3f;
        moby_data->head_yaw = yaw * 0.7f;
    }
    if (D_0015EDB0_b)
        moby_data->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_data->body, 1);
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, (char *)moby_data + 0xE0, 0);
    if (*(int *)((char *)moby_data + 0x180) == 0) {
        char *created_moby = func_0020D348_m_x(0x11F);
        *(char **)((char *)moby_data + 0x180) = created_moby;
        if (created_moby) {
            *(unsigned short *)(created_moby + 0x32) = *(unsigned short *)((char *)moby + 0x32);
            (*(unsigned char **)((char *)moby_data + 0x180))[0x31] = 1;
            {
                char *p = *(char **)((char *)moby_data + 0x180);
                qcopy(p + 0x10, (char *)moby + 0x10);
                qcopy(p + 0x40, (char *)moby + 0x40);
            }
            FUN_L00_00250df8_x(moby);
        }
    } else {
        update_moby_bone_attachment(moby, *(void **)((char *)moby_data + 0x180), 2);
    }
}
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8588), where it is exact; names translated to the US level program. */

extern char D_L12_00167240[];
extern void draw_level_12_effects(void) __asm__("FUN_L12_002e71b0");

void register_level_12_effects_draw_callback(struct Moby *moby) __asm__("FUN_L12_002e7208");

void register_level_12_effects_draw_callback(struct Moby *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER = 1
    };
    char *data = (char *)moby->pvars;
    switch (moby->state) {
    case DRAW_CALLBACK_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case DRAW_CALLBACK_CHECK_VOLUME_AND_REGISTER: {
        int value = *(int *)(data + 0xC);
        if (value == -1 || is_point_inside_clip_volume(D_L12_00167240, value)) {
            add_draw_callback((void *)draw_level_12_effects, moby);
        }
    } break;
    }
}
/* Forward the vendor parameters with fixed range and scale values. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E8610), where it is exact; names translated to the US level program. */

extern void spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");

void spawn_blarg_trooper_death_effects(void *moby, void *effect_position) __asm__("FUN_L12_002e7290");

void spawn_blarg_trooper_death_effects(void *moby, void *effect_position) {
    spawn_color_swapped_explosion_burst(moby, effect_position, -1, 1.0f, 10.0f);
}
extern void process_blarg_trooper_damage_and_alerts(struct Moby *) __asm__("FUN_L12_002e81e0");
void set_blarg_trooper_aim_yaw_offsets(struct Moby *moby, float *pos) __asm__("FUN_L12_002e8608");

extern void set_blarg_trooper_aim_yaw_offsets(struct Moby *, float *) __asm__("FUN_L12_002e8608");
void remove_blarg_trooper_and_attachment(struct Moby *moby) __asm__("FUN_L12_002e86d0");

extern void remove_blarg_trooper_and_attachment(struct Moby *) __asm__("FUN_L12_002e86d0");
void update_blarg_trooper_attachment(struct Moby *moby) __asm__("FUN_L12_002e8720");

extern void update_blarg_trooper_attachment(struct Moby *) __asm__("FUN_L12_002e8720");
void ease_blarg_trooper_height_to_ground(char *moby) __asm__("FUN_L12_002e87b0");

extern void ease_blarg_trooper_height_to_ground(char *) __asm__("FUN_L12_002e87b0");
void steer_blarg_trooper_toward_point(struct Moby *moby, float *target, float max_angle, float approach_speed) __asm__("FUN_L12_002e8808");

extern void steer_blarg_trooper_toward_point(struct Moby *, float *, float, float) __asm__("FUN_L12_002e8808");
extern s32 D_001413D0_e81e0 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 scale_game_frames_e81e0(s32) __asm__("FUN_001f96f8");
extern f32 fast_cos_e81e0(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_e81e0(f32) __asm__("FUN_001f9de0");
extern void call_for_help_e81e0(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern int random_integer_below_e81e0(s32) __asm__("FUN_00213260");
extern float angle_difference_8808(float, float) __asm__("FUN_001fa688");
extern float fast_add_rotations_e81e0(float, float) __asm__("FUN_001fa580");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void subtract_8808(void *, void *, void *) __asm__("FUN_001f9a28");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern struct Moby *D_L12_00178380[];
extern int coll_line_fix_72(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int query_mobys_72(void *, int, void *, int, float) __asm__("FUN_L00_001f2868");
extern int is_moby_in_class_range_500_to_540(void *) __asm__("FUN_L00_0025e3b8");
extern float FUN_L00_00257c48(float, float);
extern void blend_moby_animation_ex(void *, int, int, int, int) __asm__("FUN_002130d8");
extern int select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern int test_point_within_moby_limits(void *, void *, float, float, float) __asm__("FUN_L12_0027b8e0");
extern float vector_project_72(void *) __asm__("FUN_001f9b20");
extern void spawn_class_b8_projectile(void *, void *, void *, int) __asm__("FUN_L04_002aee30");

#define TROOPER_I(offset) (*(int *)(data + (offset)))
#define TROOPER_F(offset) (*(float *)(data + (offset)))
#define TROOPER_B(offset) (*(u8 *)(data + (offset)))
#define TROOPER_H(offset) (*(u16 *)(data + (offset)))
#define TROOPER_CHILD (*(struct Moby **)(data + 0x248))

/* Blarg trooper update: initialization, path following, aiming, attack and removal.
   The retail unsigned group sentinel is 0xff; unordered angle comparisons
   keep the original branch outcomes. Quad copies include the fourth word. */
void blarg_trooper_update(struct Moby *moby) __asm__("FUN_L12_002e72c0");
void blarg_trooper_update(struct Moby *moby) {
    enum {
        BLARG_TROOPER_INITIALIZE = 0,
        BLARG_TROOPER_FOLLOW_WAYPOINTS = 1,
        BLARG_TROOPER_WAIT_FOR_ACTIVATION = 2,
        BLARG_TROOPER_APPROACH_TARGET = 3,
        BLARG_TROOPER_KNOCKBACK_RECOVERY = 4,
        BLARG_TROOPER_PREPARE_SHOT = 5,
        BLARG_TROOPER_FIRE_PROJECTILE = 6,
        BLARG_TROOPER_WAIT_BETWEEN_SHOTS = 7,
        BLARG_TROOPER_RETURN_HOME = 8,
        BLARG_TROOPER_DEATH_FALL = 9
    };
    TargetReport54_2E2EF0 work;
    Vec4 query_position_or_projectile_origin;
    Vec4 delta;
    char *data = (char *)moby->pvars;
    struct Moby *target;

    func_L00_00264BB0_2E2EF0(data + 0x120, 3.0f);
    if (D_L12_0015F5C4 == 2) goto hide;
    if (moby->state >= 3) {
        moby->flags &= 0xffbe;
        if (TROOPER_CHILD) TROOPER_CHILD->flags &= 0xffbe;
    }
    if (TROOPER_B(0x269) && hero.state.current == 0 && hero.state.prev == 0x67) {
        if (vector_distance(&moby->pos, &hero.motion.pos) < 30.0f) goto leave;
    }
    if (TROOPER_I(0x270) != -1) {
        if (!is_point_inside_clip_volume(D_L12_00167240, TROOPER_I(0x270))) {
hide:
            moby->flags |= 0x41;
            if (TROOPER_CHILD) TROOPER_CHILD->flags |= 0x41;
            return;
        }
        moby->flags &= 0xffbe;
        if (TROOPER_CHILD) TROOPER_CHILD->flags &= 0xffbe;
    }
    if (TROOPER_B(0x2e) == 2) goto leave;
    process_blarg_trooper_damage_and_alerts(moby);
    FUN_L01_00274b78_c((char *)moby, (char *)&work, TROOPER_F(0x230));
    target = (struct Moby *)work.moby;
    if (!target) {
        target = hero.moby;
        qcopy(work.v20, hero.motion.unkD0.f);
    }
    switch (moby->state) {
    case BLARG_TROOPER_INITIALIZE: {
        float value;
        float other = TROOPER_F(0x230);
        TROOPER_F(0x20) = 2.0f;
        TROOPER_H(0x24) = 2;
        TROOPER_B(0x29) = 1;
        TROOPER_B(0x2e) = 1;
        TROOPER_B(0x58) = 8;
        TROOPER_B(0x5a) = 8;
        TROOPER_F(0x28c) = other;
        TROOPER_I(0x90) = 0x200;
        TROOPER_F(0x98) = 0.5f;
        func_L01_0026E8E0_2E2EF0(data + 0xd0);
        value = D_0015ED6C * 5.5f;
        TROOPER_I(0xd0) = 0x38d;
        *(volatile float *)(data + 0xd8) = 1.0f;
        TROOPER_I(0x108) = 0x18;
        *(volatile float *)(data + 0xf4) = value;
        *(volatile float *)(data + 0xdc) = 1.0f;
        qcopy(data + 0x220, &moby->pos);
        if (moby->group != 0xff) {
            if (**(int **)(data + 0x24c) == 1) moby->flags |= 0x8000;
            **(int **)(data + 0x24c) = (**(int **)(data + 0x24c) == 0);
        } else if (func_001160D8_2E2EF0() & 1) {
            moby->flags |= 0x8000;
        }
        if (TROOPER_I(0x274)) TROOPER_I(0x288) = scale_game_frames_e81e0(FUN_L00_00257b90(0, 300));
        moby->rot.x = 0.0f;
        moby->rot.y = 0.0f;
        if (TROOPER_F(0x244) == moby->rot.x) {
            if (moby->group != 0xff) {
                int *path = *(int **)(data + 0x250);
                int count;
                TROOPER_F(0x244) = (float)*path;
                count = *path;
                if (count > 0) *path = -count;
                else *path = count + 20;
            } else TROOPER_F(0x244) = FUN_L00_00257c48(15.0f, 35.0f);
        }
        TROOPER_F(0x244) *= 0.017453292f;
        moby->pos.z = probe_ground_height(&moby->pos, 0, 0.5f);
        if (TROOPER_B(0x268)) {
            moby->flags |= 0x41;
            if (TROOPER_CHILD) TROOPER_CHILD->flags |= 0x41;
        }
        moby->state = 2;
        break;
    }
    case BLARG_TROOPER_WAIT_FOR_ACTIVATION: {
        if (distance_xyz(&moby->pos, &hero.motion.pos) > 64.0f) return;
        if (moby->prev_seq != 0 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 0, 0, scale_game_frames_e81e0(8), 5);
        if (!test_point_within_moby_limits(moby, &target->pos, TROOPER_F(0x230), 3.0f, 0.0f)) {
            if ((TROOPER_I(0x25c) == -1 || !is_point_inside_clip_volume(&target->pos, TROOPER_I(0x25c))) && moby->command == 0) break;
        }
        if (moby->flags & 1) {
            qcopy(query_position_or_projectile_origin.f, &moby->pos);
            query_position_or_projectile_origin.f[2] += 0.5f;
            if (coll_line_fix_72(&query_position_or_projectile_origin, D_L12_00167240, 2, moby, 0)) {
                moby->flags &= 0xffbe;
                if (TROOPER_CHILD) TROOPER_CHILD->flags &= 0xffbe;
            } else {
                if (TROOPER_CHILD) mark_moby_for_removal(TROOPER_CHILD);
                mark_moby_for_removal(moby);
                return;
            }
        }
        if (moby->group != 0xff) FUN_L01_0026e090(moby->group, 1);
        if (TROOPER_I(0x254) == -1) goto state3;
        else {
            TROOPER_I(0x260) = select_point_index_by_distance(&moby->pos, D_L12_001B0930_w[TROOPER_I(0x254)], 0.0f);
            moby->state = 1;
        }
        break;
    }
    case BLARG_TROOPER_FOLLOW_WAYPOINTS: {
        int *path;
        if (!tick_countdown_32_alt_c2((int *)(data + 0x258))) break;
        if (moby->prev_seq != 8 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 8, 0, scale_game_frames_e81e0(11), 5);
        path = (int *)D_L12_001B0930_w[TROOPER_I(0x254)];
        if (vector_distance(&moby->pos, path + (TROOPER_I(0x260) * 4 + 4)) < 3.0f) ++TROOPER_I(0x260);
        if (*path - 1 < TROOPER_I(0x260)) TROOPER_I(0x260) = *path - 1;
        steer_blarg_trooper_toward_point(moby, (float *)(path + (TROOPER_I(0x260) * 4 + 4)), 0.0f, D_0015ED6C * 8.0f);
        if (TROOPER_I(0x260) == *path - 1) goto state3;
        else if (vector_distance(&moby->pos, &target->pos) < TROOPER_F(0x264)) moby->state = 3;
        break;
    }
    case BLARG_TROOPER_APPROACH_TARGET: {
        float angle;
        if (TROOPER_B(0x268) == 1) {
            int hit;
            qcopy(query_position_or_projectile_origin.f, &moby->pos);
            query_position_or_projectile_origin.f[0] += fast_cos_e81e0(moby->rot.z);
            query_position_or_projectile_origin.f[1] += fast_sin_e81e0(moby->rot.z);
            hit = query_mobys_72(&query_position_or_projectile_origin, 0, moby, 0, 1.5f);
            if (hit) {
                qcopy(delta.f, ((char *)&D_L12_0015F580));
                if (hit > 0) {
                    struct Moby **iterator = D_L12_00178380;
                    int count = hit;
                    do {
                        if (is_moby_in_class_range_500_to_540(*iterator) && func_001F9B88_2E2EF0(moby->pos.z - (*iterator)->pos.z) < 0.7f)
                            call_for_help_e81e0(*iterator, (int)hero.moby, 0x10000, &moby->pos, delta.f, 1.0f);
                        --count;
                        ++iterator;
                    } while (count != 0);
                }
            }
        }
        angle = func_001FA790_2E2EF0(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y), moby->rot.z);
        if (angle > 0.9599311f) angle = 0.9599311f;
        else if (angle < -0.9599311f) angle = -0.9599311f;
        TROOPER_F(0x188) = angle * 0.5f;
        TROOPER_F(0x208) = angle * 0.5f;
        steer_blarg_trooper_toward_point(moby, (float *)&target->pos, TROOPER_F(0x244), 0.0f);
        if (vector_distance(&moby->pos, &target->pos) < 5.5f || TROOPER_B(0x242)) {
            angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            if (angle_difference_8808(moby->rot.z, angle) < 0.08726646f) goto state5;
        }
        if (moby->prev_seq != 8 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 8, 0, scale_game_frames_e81e0(11), 5);
        break;
    }
    case BLARG_TROOPER_PREPARE_SHOT: {
        if (TROOPER_F(0x28c) < 8.0f) TROOPER_F(0x28c) = 8.0f;
        set_blarg_trooper_aim_yaw_offsets(moby, (float *)&target->pos);
        ease_blarg_trooper_height_to_ground((char *)moby);
        if (moby->prev_seq != 2 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 2, 0, scale_game_frames_e81e0(8), 5);
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 10 && moby->seq == moby->prev_seq)
                blend_moby_animation_ex(moby, 10, 0, scale_game_frames_e81e0(5), 5);
            moby->state = 6;
        }
        break;
    }
    case BLARG_TROOPER_FIRE_PROJECTILE: {
        int count; float angle; float other; float value;
        set_blarg_trooper_aim_yaw_offsets(moby, (float *)&target->pos);
        ease_blarg_trooper_height_to_ground((char *)moby);
        if (TROOPER_CHILD && is_value_within_interpolated_window(moby, 0.0f)) {
            moby_point_position(TROOPER_CHILD, 0, &query_position_or_projectile_origin);
            subtract_8808(&delta, work.v20, &query_position_or_projectile_origin);
            angle = fast_arc_tan(delta.f[0], delta.f[1]);
            other = fast_add_rotations_e81e0(moby->rot.z, TROOPER_F(0x208));
            angle = 0.2268928f < angle_difference_8808(angle, other) ? other : angle;
            value = vector_project_72(&delta);
            value = fast_arc_tan(value, delta.f[2]);
            build_spherical_offset(&delta, D_0015ED6C * 15.0f, angle, value);
            allocate_voice_for_target_entry(0, 0, (int)moby);
            count = truncate_float_to_s32(18.0f / (D_0015ED6C * 15.0f));
            spawn_class_b8_projectile(moby, &query_position_or_projectile_origin, &delta, count);
        }
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 5 && moby->seq == moby->prev_seq)
                blend_moby_animation_ex(moby, 5, 0, scale_game_frames_e81e0(8), 5);
            moby->state = 7;
        }
        break;
    }
    case BLARG_TROOPER_WAIT_BETWEEN_SHOTS: {
        float angle;
        set_blarg_trooper_aim_yaw_offsets(moby, (float *)&target->pos);
        ease_blarg_trooper_height_to_ground((char *)moby);
        if ((moby->prev_seq != 5 || (moby->anim_flags & 2)) && moby->prev_seq != 0 && moby->seq == moby->prev_seq)
            blend_moby_animation_ex(moby, 0, 0, scale_game_frames_e81e0(8), 5);
        if (!random_integer_below_e81e0(9)) goto state5;
        if (vector_distance(&moby->pos, &target->pos) > 10.5f && !TROOPER_B(0x242)) goto state3;
        angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
        if (angle_difference_8808(moby->rot.z, angle) > 0.4886922f) moby->state = 3;
        break;
    }
    case BLARG_TROOPER_RETURN_HOME: {
        float angle;
        steer_blarg_trooper_toward_point(moby, (float *)(data + 0x220), 0.0f, 0.0f);
        angle = TROOPER_F(0x23c) * 30.0f;
        if (angle > 0.7853982f) angle = 0.7853982f;
        else if (angle < -0.7853982f) angle = -0.7853982f;
        TROOPER_F(0x188) = angle * 0.7f;
        TROOPER_F(0x208) = angle * 0.4f;
        if (vector_distance(&moby->pos, data + 0x220) < 1.0f) moby->state = 2;
        break;
    }
    case BLARG_TROOPER_KNOCKBACK_RECOVERY: {
        float angle;
        moby->flags &= 0xffbe;
        if (mine_fall(moby, data + 0x70) & 0x60) {
            if (vector_distance(&moby->pos, &target->pos) < 6.0f || TROOPER_B(0x242)) {
                angle = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
                if (angle_difference_8808(moby->rot.z, angle) < 0.12217305f) {
state5:
                    moby->state = 5;
                    break;
                }
            }
state3:
            moby->state = 3;
            break;
        }
        if (!(moby->pos.z < 2.0f)) goto finish;
        goto leave;
    }
    case BLARG_TROOPER_DEATH_FALL: {
        if (mine_fall(moby, data + 0x70) & 0x60) {
            FUN_L00_00257470(moby, 0, -1);
            spawn_blarg_trooper_death_effects(moby, &moby->pos);
            goto leave;
        }
        if (moby->pos.z < 2.0f) {
            FUN_L00_00257470(moby, 0, -1);
leave:
            remove_blarg_trooper_and_attachment(moby);
            return;
        }
        break;
    }
    }
finish:
    if (moby->visible || (TROOPER_CHILD && TROOPER_CHILD->visible)) {
        update_blarg_trooper_attachment(moby);
        if (moby->flags & 0x8000) {
            TROOPER_F(0x188) = -TROOPER_F(0x188);
            TROOPER_F(0x208) = -TROOPER_F(0x208);
        }
        update_joint_animation(D_0015ED64 * 0.018f, D_0015ED64 * 0.3f, moby, data + 0x120, 5);
        update_joint_animation(D_0015ED64 * 0.024f, D_0015ED64 * 0.3f, moby, data + 0x1a0, 6);
    }
    return;
}
#undef TROOPER_I
#undef TROOPER_F
#undef TROOPER_B
#undef TROOPER_H
#undef TROOPER_CHILD
/* Variables of the Hoven blarg trooper (moby->pvars): the l00 ElectroidVars
   damage layout, with the alert and call-for-help state at 0x230.. */
typedef struct {
    u8 pad0[0x20];
    f32 health;          /* 0x20 */
    u8 pad24[2];
    s16 stun_timer;      /* 0x26 */
    u8 pad28[0x10];
    s32 reset_alert;     /* 0x38 */
    u8 pad3C[0x24];
    u8 anim[7];          /* 0x60: animation blend state */
    u8 unk67;            /* 0x67 */
    u8 pad68[8];
    u8 fall[0x10];       /* 0x70 */
    f32 unk80;           /* 0x80 */
    u8 pad84[4];
    f32 unk88;           /* 0x88 */
    f32 unk8C;           /* 0x8C */
    u8 pad90[4];
    s32 unk94;           /* 0x94 */
    u8 pad98[8];
    f32 unkA0;           /* 0xA0 */
    f32 unkA4;           /* 0xA4 */
    f32 unkA8;           /* 0xA8 */
    u8 padAC;
    u8 unkAD;            /* 0xAD */
    u8 padAE[0x12];
    f32 unkC0;           /* 0xC0 */
    f32 unkC4;           /* 0xC4 */
    u8 padC8[0x168];
    f32 speed;           /* 0x230 */
    u8 pad234[0xC];
    s16 alert_timer;     /* 0x240 */
    u8 pad242[0x32];
    s32 call_enabled;    /* 0x274 */
    s32 groups[4];       /* 0x278: group ids counted before calling for help (-1: none) */
    s32 call_timer;      /* 0x288 */
    f32 alert_speed;     /* 0x28C */
    s32 volume;          /* 0x290: clip volume that hands the trooper to FUN_L12_002e86d0 (-1: none) */
} BlargTrooperVars_e81e0;

typedef struct {
    u8 pad0[0x10];
    Vec4 pos;
    struct Moby *moby;   /* 0x20 */
} BlargTrooperHit_e81e0;

extern s32 D_001413D0_e81e0 __asm__("D_001413D0") __attribute__((section(".data")));
extern f32 D_0015ED6C_e81e0 __asm__("D_0015ED6C");
extern f32 D_0015ED70_e81e0 __asm__("D_0015ED70");
extern s32 scale_game_frames_e81e0(s32) __asm__("FUN_001f96f8");
extern s32 tick_countdown_16_e81e0(s16 *) __asm__("FUN_001f9770");
extern s32 random_integer_below_e81e0(s32) __asm__("FUN_00213260");
extern f32 fast_add_rotations_e81e0(f32, f32) __asm__("FUN_001fa580");
extern f32 fast_cos_e81e0(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_e81e0(f32) __asm__("FUN_001f9de0");
extern s32 count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern void alert_group_e81e0(s32, s32) __asm__("FUN_L01_0026e090");
extern void call_for_help_e81e0(struct Moby *, s32, s32, Vec4f *, f32 *, f32) __asm__("FUN_L00_00259bc8");
extern s32 is_point_inside_clip_volume_e81e0(void *, s32) __asm__("FUN_00214720");
extern BlargTrooperHit_e81e0 *find_hit_e81e0(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void take_damage_e81e0(struct Moby *, BlargTrooperHit_e81e0 *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void knockback_e81e0(void *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void start_fall_e81e0(struct Moby *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void anim_reset_e81e0(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void anim_update_e81e0(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern void remove_blarg_trooper_and_attachment(struct Moby *) __asm__("FUN_L12_002e86d0");

/* Damage and alert step of the Hoven blarg trooper (cf. FUN_L12_002e3528): refreshes its
   speed while alerted, calls for help behind itself once its groups have 9 or
   more members, hands off inside its clip volume, and takes hits from other
   classes (dying, or staggering back). */
void process_blarg_trooper_damage_and_alerts(struct Moby *moby) {
    BlargTrooperVars_e81e0 *vars = (BlargTrooperVars_e81e0 *)moby->pvars;
    Vec4 help_direction_or_hit_position;
    s32 hit;
    f32 dmg;
    f32 angle;
    f32 unused;
    BlargTrooperHit_e81e0 *coll;

    if (vars->reset_alert != 0) {
        vars->reset_alert = 0;
        vars->alert_timer = scale_game_frames_e81e0(0xF0);
    }
    if (moby->state != 0) {
        vars->speed = tick_countdown_16_e81e0(&vars->alert_timer) ? vars->alert_speed : 20.0f;
    }
    if (vars->call_enabled != 0 && moby->state != 2) {
        vars->call_timer++;
        if (scale_game_frames_e81e0(0x3C) * 17 < vars->call_timer) {
            s32 count = 0;
            if (vars->groups[0] != -1)
                count += count_active_moby_group_members(vars->groups[0], 2);
            if (vars->groups[1] != -1)
                count += count_active_moby_group_members(vars->groups[1], 2);
            if (vars->groups[2] != -1)
                count += count_active_moby_group_members(vars->groups[2], 2);
            if (vars->groups[3] != -1)
                count += count_active_moby_group_members(vars->groups[3], 2);
            if (count >= 9 && random_integer_below_e81e0(0x27) == 0) {
                help_direction_or_hit_position.f[0] = fast_cos_e81e0(fast_add_rotations_e81e0(moby->rot.z, 3.1415927f));
                help_direction_or_hit_position.f[1] = fast_sin_e81e0(fast_add_rotations_e81e0(moby->rot.z, 3.1415927f));
                help_direction_or_hit_position.f[2] = 0.0f;
                call_for_help_e81e0(moby, D_001413D0_e81e0, 0x10000, &moby->pos, help_direction_or_hit_position.f, 1.0f);
            }
        }
    }
    if (vars->volume != -1 && is_point_inside_clip_volume_e81e0(&hero.motion.pos, vars->volume)) {
        remove_blarg_trooper_and_attachment(moby);
        return;
    }
    coll = find_hit_e81e0(moby, 0x330000, 0);
    dmg = 0.0f;
    take_damage_e81e0(moby, coll, &vars->health, 0, &hit, &dmg, 0, 4);
    if (coll != 0 && coll->moby->oclass != moby->oclass && coll->moby->oclass != 0xB8 &&
        moby->state != 9 && dmg != 0.0f) {
        if (moby->group != 0xFF) {
            alert_group_e81e0(moby->group, 1);
        }
        vars->health -= dmg;
        vars->unk80 = D_0015ED70_e81e0 * 30.0f;
        vars->unkAD = 0;
        vars->unk94 = 9;
        vars->unkA0 = 1.0f;
        vars->unkA4 = 1.0f;
        vars->unkA8 = 1.0f;
        if (vars->health <= 0.0f) {
            moby->flags &= ~0x1000;
            vars->unk88 = D_0015ED6C_e81e0 * 12.0f;
            vars->unk8C = D_0015ED6C_e81e0 * 9.0f;
            help_direction_or_hit_position.q = coll->pos.q;
            knockback_e81e0(&help_direction_or_hit_position, &angle, &vars->unk88, &vars->unk8C);
            start_fall_e81e0(moby, vars->fall, 9, 1, 0, angle);
            vars->unkC0 = 10.0f;
            vars->unkC4 = 19.0f;
            moby->state = 9;
            vars->unk67 = 0x78;
            anim_reset_e81e0(moby, vars->anim);
        } else {
            vars->unk88 = D_0015ED6C_e81e0 * 4.7f;
            vars->unk8C = D_0015ED6C_e81e0 * 7.7f;
            help_direction_or_hit_position.q = coll->pos.q;
            knockback_e81e0(&help_direction_or_hit_position, &unused, &vars->unk88, &vars->unk8C);
            start_fall_e81e0(moby, vars->fall, 6, 1, 0, unused);
            vars->unkC0 = 4.0f;
            vars->unkC4 = 8.0f;
            moby->state = 4;
            vars->unk67 = 0xFA;
            vars->stun_timer = scale_game_frames_e81e0(0x3C);
            anim_reset_e81e0(moby, vars->anim);
        }
    }
    moby->hit_slot = 0xFF;
    anim_update_e81e0(moby, vars->anim);
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9988), where it is exact; names translated to the US level program. */

extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");

void set_blarg_trooper_aim_yaw_offsets(struct Moby *moby, float *pos) {
    char *data = (char *)moby->pvars;
    float target_yaw = fast_arc_tan(pos[0] - moby->pos.x, pos[1] - moby->pos.y);
    float yaw_offset = fast_subtract_rotations(target_yaw, moby->rot.z);
    if (yaw_offset > 1.2217305f) {
        yaw_offset = 1.2217305f;
    } else if (yaw_offset < -1.2217305f) {
        yaw_offset = -1.2217305f;
    }
    *(float *)(data + 0x188) = yaw_offset * 0.35f;
    *(float *)(data + 0x208) = yaw_offset * 0.8f;
}
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

/* Release the vendor moby and any child it owns. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9A50), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x248];
    void *child;
} Level12VendorData;


extern void FUN_L00_00257470(struct Moby *, int, int);

void remove_blarg_trooper_and_attachment(struct Moby *moby) __asm__("FUN_L12_002e86d0");
void remove_blarg_trooper_and_attachment(struct Moby *moby) {
    moby->unkB4 = 0;
    FUN_L00_00257470(moby, 0, -1);
    if (((Level12VendorData *)moby->pvars)->child != 0) {
        mark_moby_for_removal(((Level12VendorData *)moby->pvars)->child);
    }
    mark_moby_for_removal(moby);
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9AA0), where it is exact; names translated to the US level program. */

extern void *spawn_moby_attachment(void *, void *) __asm__("FUN_L04_002ce080");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void update_moby_bone_attachment(void *, void *, int) __asm__("FUN_L12_0027b9c0");

void update_blarg_trooper_attachment(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    unsigned char *target = *(unsigned char **)(data + 0x248);
    if (target != 0) {
        unsigned char *current;
        update_moby_bone_attachment(moby, target, 1);
        current = *(unsigned char **)(data + 0x248);
        if (moby->flags & 1) {
            *(unsigned short *)(current + 0x34) |= 0x41;
        } else {
            *(unsigned short *)(current + 0x34) &= 0xFFBE;
        }
    } else {
        char vector[16];
        moby_point_position(moby, 1, vector);
        *(void **)(data + 0x248) = spawn_moby_attachment(moby, vector);
    }
}
/* Initialize two vendor vectors using the global scale. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002E9B30), where it is exact; names translated to the US level program. */

extern void approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
f32 probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

/* Eases the moby's height toward the ground below it. */
void ease_blarg_trooper_height_to_ground(char *moby) {
    float value = probe_ground_height(moby + 0x10, 0, 0.5f);
    approach_value(moby + 0x18, value, 27.0f * frame_time_sq);
}
extern float angle_8808(float, float) __asm__("FUN_001f9e90");
extern float limit_angle_8808(float, float) __asm__("FUN_001fa580");
extern float angle_difference_8808(float, float) __asm__("FUN_001fa688");
extern float distance_8808(void *, void *) __asm__("FUN_001f9b80");
extern float moved_distance_8808(void *, void *) __asm__("FUN_001f9b48");
extern void turn_8808(void *, void *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void approach_8808(void *, float, float) __asm__("FUN_00213ed8");
extern void move_moby_along_heading(void *, void *) __asm__("FUN_L12_00271dd8");
extern void subtract_8808(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int path_8808(int, void *, void *, void *) __asm__("FUN_L00_00261b48");
extern int inside_8808(void *, void *, int) __asm__("FUN_L00_00259740");
extern int frames_8808(int) __asm__("FUN_001f96f8");

void steer_blarg_trooper_toward_point(struct Moby *moby, float *target, float max_angle, float approach_speed) {
    char *data = (char *)moby->pvars;
    float distance;
    float angle;
    float old[4] __attribute__((aligned(16)));
    float result[4] __attribute__((aligned(16)));
    float next[4] __attribute__((aligned(16)));
    float delta[4] __attribute__((aligned(16)));
    int count;

    if (*(unsigned char *)(data + 0x242)) {
        float *rotation = &moby->rot.z;
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn_8808(rotation, data + 0x23c, angle, frame_time_sq * 15.707963f,
                  frame_time_sq * 12.566371f, frame_time * 25.132742f);
        return;
    }
    distance = distance_8808(&moby->pos, target);
    if (distance > 6.5f && max_angle != 0.0f) {
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        angle = limit_angle_8808(angle, max_angle);
        turn_8808(&moby->rot.z, data + 0x23c, angle, frame_time_sq * 15.707963f,
                  frame_time_sq * 12.566371f, frame_time * 25.132742f);
    } else {
        float *rotation = &moby->rot.z;
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn_8808(rotation, data + 0x23c, angle, frame_time_sq * 15.707963f,
                  frame_time_sq * 12.566371f, frame_time * 25.132742f);
    }
    if (approach_speed != 0.0f) {
        approach_8808(data + 0xf4, approach_speed, frame_time_sq * 7.0f);
    } else {
        angle = angle_8808(target[0] - moby->pos.x, target[1] - moby->pos.y);
        if (angle_difference_8808(moby->rot.z, angle) < 1.5707964f && 6.0f < distance) {
            approach_8808(data + 0xf4, frame_time * 7.0f, frame_time_sq * 7.0f);
        } else {
            approach_8808(data + 0xf4, frame_time, frame_time_sq * 7.0f);
        }
    }
    *(OvlQuad *)old = *(OvlQuad *)&moby->pos;
    move_moby_along_heading(moby, data + 0xd0);
    if (*(int *)(data + 0x26c) != -1) {
        subtract_8808(delta, old, &moby->pos);
        fast_vec_normalize(delta, delta, 0.05f);
        fast_vec_add(next, old, delta);
        if (path_8808(*(int *)(data + 0x26c), &moby->pos, next, result)) {
            char *path = D_L12_001B0930_w[*(int *)(data + 0x26c)];
            if (!inside_8808(&moby->pos, path + 0x10, *(int *)path))
                *(OvlQuad *)&moby->pos = *(OvlQuad *)result;
            if (moved_distance_8808(&moby->pos, old) < frame_time * 0.5f) {
                ++*(unsigned char *)(data + 0x243);
                count = *(unsigned char *)(data + 0x243);
                if (frames_8808(15) < count)
                    *(unsigned char *)(data + 0x242) = 1;
            } else {
                *(unsigned char *)(data + 0x243) = 0;
            }
        }
    }
}
extern f32 approach_value_e9f68(void *, f32, f32) __asm__("FUN_00213ed8");
extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern float FUN_L00_00257c48(float, float);
extern void FUN_L01_00274b78(void *, void *, float);
extern void FUN_L00_00263ac8(void *, int, void *, float);
void process_blarg_helicopter_hits(struct Moby *moby) __asm__("FUN_L12_002eb220");

extern void process_blarg_helicopter_hits(struct Moby *) __asm__("FUN_L12_002eb220");
void update_blarg_helicopter_motion_tilt(struct Moby *moby) __asm__("FUN_L12_002eae00");

extern void update_blarg_helicopter_motion_tilt(struct Moby *) __asm__("FUN_L12_002eae00");
int follow_blarg_helicopter_path(struct Moby *moby, int path_index) __asm__("FUN_L12_002eaf68");

extern int follow_blarg_helicopter_path(struct Moby *, int) __asm__("FUN_L12_002eaf68");
extern void cache_consecutive_point_distances(int) __asm__("FUN_L12_0027ba70");
extern struct Moby *create_class_55b_moby_from_source(struct Moby *) __asm__("FUN_L12_00308670");
extern void start_blarg_commander_knockback(struct Moby *, struct Moby *) __asm__("FUN_L12_00308708");
extern int allocate_voice_for_target_entry_e9f68(int, int, int) __asm__("FUN_0022da68");
extern void release_voice(int) __asm__("FUN_0022d798");
extern void help_request(int, int) __asm__("FUN_L00_00203908");
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern char *spawn_rocket(void *, void *, int, int, float, float, float) __asm__("FUN_L12_002ecce8");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern unsigned char D_001413F4 __attribute__((section(".data")));
extern int D_L12_00161F00 __attribute__((section(".sdata")));
extern int D_L12_00161F20 __attribute__((section(".sdata")));
extern int D_L12_001619A0 __attribute__((sda));
typedef struct { char pad[0x25c]; struct Moby *moby; int count; } Tracking_e9f68;
extern Tracking_e9f68 D_L12_00167100;
#include "rnc/gameplay/state/game_globals.h"
#include "rnc/audio/voice_pool.h"
extern unsigned char D_0013E550[];
extern unsigned char D_0013D4CB __attribute__((section(".data")));
extern unsigned short D_00141C50 __attribute__((section(".data")));
extern int D_L12_0015F580_gp __asm__("D_L12_0015F580") __attribute__((section(".sdata")));

void blarg_helicopter_update(struct Moby *moby) __asm__("FUN_L12_002e9f68");
void blarg_helicopter_update(struct Moby *moby) {
    enum {
        BLARG_HELICOPTER_INITIALIZE = 0,
        BLARG_HELICOPTER_WAIT_FOR_ACTIVATION = 1,
        BLARG_HELICOPTER_FOLLOW_INITIAL_PATH = 2,
        BLARG_HELICOPTER_AIM_AND_FIRE = 3,
        BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS = 4,
        BLARG_HELICOPTER_KNOCKBACK = 5,
        BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET = 6
    };
    char *data = (char *)moby->pvars;
    struct Moby *child;
    float target[20] __attribute__((aligned(16)));
    OvlVec4 position, delta, aim, velocity;
    int index, count, limit, a, b, color, life, size, triggered;
    float distance, angle, value, spread;
    if (D_L12_0015F5C4 == 2) return;
    FUN_L01_00274b78(moby, target, 70.0f);
    child = *(struct Moby **)(data + 0xac);
    if (child != 0 && (signed char)child->state >= 0)
        FUN_L00_00263ac8(child, 0, data + 0xd0, 2.7f);
    process_blarg_helicopter_hits(moby);
    if (D_001413F4 == 2) {
        moby->scale = moby->pclass->scale * 0.5f;
        {
            struct Moby *child = *(struct Moby **)(data + 0xac);
            if (child != 0) child->scale = child->pclass->scale * 0.5f;
        }
    } else {
        moby->scale = moby->pclass->scale;
        {
            struct Moby *child = *(struct Moby **)(data + 0xac);
            if (child != 0) child->scale = child->pclass->scale;
        }
    }
    if ((unsigned int)(moby->state - 2) < 4) {
        if (*(int *)(data + 0xb0) == -1)
            *(int *)(data + 0xb0) = allocate_voice_for_target_entry_e9f68(3, 4, (int)moby);
    } else {
        int index = *(int *)(data + 0xb0);
        if (index != -1 && (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby && ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state)
            release_voice(index);
        *(int *)(data + 0xb0) = -1;
    }
    index = *(int *)(data + 0xc0);
    if (current_level_index == 12 && D_L12_00161F00 != 0) {
        if (index == 0) {
            if (D_L12_00167100.moby == moby) D_L12_00167100.moby = 0;
            if (!moby->visible) {
                if (*(struct Moby **)(data + 0xac)) {
                    mark_moby_for_removal(*(struct Moby **)(data + 0xac));
                    *(struct Moby **)(data + 0xac) = 0;
                }
                mark_moby_for_removal(moby);
                return;
            }
            index = *(int *)(data + 0xc0);
            goto check_mode;
        }
    } else {
check_mode:
        if (index == 0) goto update_state;
    }
    if (visited52[moby->mission + current_level_index * 16] == 255) {
        if (*(struct Moby **)(data + 0xac)) {
            mark_moby_for_removal(*(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (index == 0) goto update_state;
    if (D_L12_001619A0 != 0) {
        if (*(struct Moby **)(data + 0xac)) {
            start_blarg_commander_knockback(moby, *(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
update_state:
    switch (moby->state) {
    case BLARG_HELICOPTER_INITIALIZE: {
        int a, b, limit;
        *(short *)(data + 0x24) = 6;
        *(float *)(data + 0x20) = 6.0f;
        data[0x28] = 2;
        data[0x29] = 0;
        *(float *)(data + 0x94) = 0.0f;
        *(float *)(data + 0x98) = 0.0f;
        *(float *)(data + 0x9c) = 0.0f;
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        cache_consecutive_point_distances(*(int *)(data + 0xa0));
        cache_consecutive_point_distances(*(int *)(data + 0xa0));
        cache_consecutive_point_distances(*(int *)(data + 0xa4));
        if (moby->draw_dist < 128) moby->draw_dist = 128;
        if (*(struct Moby **)(data + 0xac) == 0) {
            *(struct Moby **)(data + 0xac) = create_class_55b_moby_from_source(moby);
            (*(struct Moby **)(data + 0xac))->update_dist = 0;
            (*(struct Moby **)(data + 0xac))->flags |= 0x41;
        }
        qcopy(&moby->pos, D_L12_001B0930[*(int *)(data + 0xa0)] + 0x10);
        if (*(int *)(data + 0xc0) != 0 && *(int *)(data + 0xc4) == 0) {
            a = scale_game_frames_alt(240);
            b = scale_game_frames_alt(400);
            a = FUN_L00_00257b90(a, b);
            {
                int limit = D_L12_00161F20 - 1;
                *(int *)(data + 0xc4) = a;
                if (limit > 10) limit = 10;
                if (limit < 0) limit = 0;
                *(int *)(data + 0xc4) += limit * scale_game_frames_alt(35);
            }
        }
        *(int *)(data + 0xb4) = 0;
        moby->state = 1;
        break;
    }
    case BLARG_HELICOPTER_WAIT_FOR_ACTIVATION: {
        int triggered, a;
        struct Moby *child;
        triggered = 0;
        if (*(int *)(data + 0xc0) && D_L12_00161F00)
            triggered = tick_countdown_32_alt_c2((int *)(data + 0xc4)) != 0;
        if ((*(int *)(data + 0x80) != -1 && is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0x80))) || triggered) {
            data[0x58] = 22; data[0x59] = 1; data[0x5a] = 7;
            moby->hit_slot = 255;
            moby->flags &= 0xffbe;
            child = *(struct Moby **)(data + 0xac);
            if (child) child->flags &= 0xffbe;
            moby->flags |= 0x1000;
            a = moby->pclass->collision;
            moby->state = 2;
            moby->collision = a;
        }
        break;
    }
    case BLARG_HELICOPTER_FOLLOW_INITIAL_PATH:
        if (*(int *)(data + 0xc0) == 0 && D_L12_00167100.moby == 0) {
            D_L12_00167100.moby = moby;
            D_L12_00167100.count = 0;
        }
        if (*(int *)(data + 0xc0)) {
            count = *(short *)(data + 0x8c);
            limit = D_L12_00161F20 - 3;
            if (limit > 5) limit = 5;
            if (limit < 0) limit = 0;
            if (count < limit * scale_game_frames_alt(15))
                *(short *)(data + 0x8c) = limit * scale_game_frames_alt(15);
        }
        if (follow_blarg_helicopter_path(moby, *(int *)(data + 0xa0))) moby->state = 3;
        break;
    case BLARG_HELICOPTER_AIM_AND_FIRE: {
        int count, limit, a, b, color, life, size;
        float distance, angle, value, spread;
        if (current_level_index == 12 && !D_0013D4CB && !D_00141C50) help_request(0x2ee2, 0x5d);
        distance = vector_distance(&moby->pos, &hero.motion.pos);
        if (*(int *)(data + 0xc0) == 0) {
            if (distance < 21.0f && moby->pos.z - hero.motion.pos.f[2] > 1.0f) {
                if (D_L12_00167100.moby == 0) {
                    D_L12_00167100.moby = moby; D_L12_00167100.count = 0;
                }
            } else if (D_L12_00167100.moby == moby) D_L12_00167100.moby = 0;
        }
        distance = vector_distance(&moby->pos, target);
        if ((distance < 40.0f && moby->pos.z - target[2] > -2.0f) || *(int *)(data + 0xc0)) {
            angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
            moby->rot.z = spring_turn((float *)(data + 0x88), moby->rot.z, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
            value = vector_distance(target, &moby->pos);
            angle = fast_arc_tan(value, moby->pos.z - target[2]);
            moby->rot.y = spring_turn((float *)(data + 0x9c), moby->rot.y, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
            clear_u64_value(data + 0x70);
            update_blarg_helicopter_motion_tilt(moby);
            if (fast_dec_timer_s16(data + 0x8c)) {
                ++*(short *)(data + 0xbc);
                moby_point_position(moby, *(int *)(data + 0xb8), &position);
                *(int *)(data + 0xb8) = *(int *)(data + 0xb8) == 0;
                qcopy(&aim, target + 8);
                if (*(int *)(data + 0xc0)) {
                    qcopy(&aim, D_L12_00167240);
                    aim.f[2] += random_float_between(-0.5f, 0.1f);
                    aim.f[0] += FUN_L00_00257c48(0.0f, 0.4f);
                }
                subtract_vector_xyz_c3(&delta, &aim, &position);
                if (*(int *)(data + 0xc0)) fast_vec_normalize(&delta, &delta, frame_time * 22.0f);
                else fast_vec_normalize(&delta, &delta, frame_time * 15.0f);
                angle = fast_arc_tan(delta.f[0], delta.f[1]);
                spread = 1.3f;
                a = scale_game_frames_alt(130);
                spawn_rocket(&delta, &position, (int)moby, a, angle, frame_time * 15.0f, 0.0f);
                allocate_voice_for_target_entry_e9f68(0, 0, (int)moby);
                fast_vec_normalize(&delta, &delta, 0.5f);
                fast_vec_add(&position, &position, &delta);
                for (count = 24; count >= 0; --count) {
                    velocity.f[0] = FUN_L00_00257c48(0.0f, frame_time * spread);
                    velocity.f[1] = FUN_L00_00257c48(0.0f, frame_time * spread);
                    velocity.f[2] = FUN_L00_00257c48(0.0f, frame_time * spread);
                    a = FUN_L00_00257b90(170, 240);
                    b = FUN_L00_00257b90(110, 180);
                    color = (b << 8) | ((long)a | 0x7f000000L);
                    a = FUN_L00_00257b90(30, 70);
                    life = scale_game_frames_alt(a);
                    size = FUN_L00_00257b90(120, 170);
                    FUN_L00_00269958(&position, &velocity, color, 255, life, 30, size, 1);
                }
                limit = 4;
                if (*(int *)(data + 0xc0)) {
                    if (D_L12_00161F20 > 10) limit = 2;
                    if (D_L12_00161F20 > 6) limit = 3;
                }
                if (*(short *)(data + 0xbc) >= limit && *(int *)(data + 0xa4) != -1) {
                    *(short *)(data + 0xbc) = 0; moby->state = 4;
                } else *(short *)(data + 0x8c) = scale_game_frames_alt(40);
            }
        }
        break;
    }
    case BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS:
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        moby->rot.z = spring_turn((float *)(data + 0x88), moby->rot.z, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
        {
            char *path = D_L12_001B0930[*(int *)(data + 0xa4)];
            if (*(short *)(data + 0xbe) != 0) subtract_vector_xyz_c3(&position, path + 16, &moby->pos);
            else subtract_vector_xyz_c3(&position, path + *(int *)path * 16, &moby->pos);
        }
        distance = vector_length_xyz_c3(&position);
        if (!(distance > 5.5f)) {
            approach_value_e9f68(data + 0x94, 0.0f, frame_time_sq * 9.0f);
            if (*(float *)(data + 0x94) == 0.0f) {
                *(short *)(data + 0x8c) = scale_game_frames_alt(50);
                *(short *)(data + 0xbe) = *(short *)(data + 0xbe) == 0;
                moby->state = 3;
                break;
            }
        } else approach_value_e9f68(data + 0x94, frame_time * 10.0f, frame_time_sq * 7.0f);
        fast_vec_normalize(data + 0x70, &position, *(float *)(data + 0x94));
        fast_vec_add(&moby->pos, &moby->pos, data + 0x70);
        update_blarg_helicopter_motion_tilt(moby);
        break;
    case BLARG_HELICOPTER_KNOCKBACK:
        if (fast_dec_timer_s16(data + 0x8e)) {
            moby->state = moby->command;
        } else {
            value = *(float *)(data + 0x94) - frame_time_sq * 4.0f;
            *(float *)(data + 0x94) = value;
            if (value < 0.0f) *(float *)(data + 0x94) = 0.0f;
            value = fast_cos_c(moby->rot.z);
            position.f[0] = value * -*(float *)(data + 0x94);
            value = fast_sin_c2(moby->rot.z);
            position.f[1] = value * -*(float *)(data + 0x94);
            position.f[2] = 0.0f;
            fast_vec_add(&moby->pos, &moby->pos, &position);
        }
        break;
    case BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET: {
        int index;
        if (D_L12_00167100.moby == moby) D_L12_00167100.moby = 0;
        if (*(struct Moby **)(data + 0xac)) {
            start_blarg_commander_knockback(moby, *(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        spawn_explosion(moby, ((char *)&D_L12_0015F580_gp), &moby->pos, 0.0f, 0.0f, 20, 3, 4, 4.0f, 2.0f, 100000.0f, 2, 3.0f, 15.0f, 1, 1, -1, 0);
        if (*(int *)(data + 0xc0) == 0) FUN_L00_00257470(moby, 0, -1);
        index = *(int *)(data + 0xb0);
        if (index != -1 && (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby && ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state) release_voice(index);
        *(int *)(data + 0xb0) = -1;
        if (*(int *)(data + 0xc0) == 0) {
remove:
            mark_moby_for_removal(moby);
            return;
        }
        a = scale_game_frames_alt(240);
        b = scale_game_frames_alt(400);
        *(int *)(data + 0xc4) = FUN_L00_00257b90(a, b);
        moby->collision = 0;
        moby->state = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        break;
    }
    }
    moby->pos.z -= fast_cos_c(*(float *)(data + 0x90)) * 0.17f;
    a = scale_game_frames_alt(120);
    angle = fast_add_rotations_c3(*(float *)(data + 0x90), 6.2831855f / (float)a);
    *(float *)(data + 0x90) = angle;
    moby->pos.z += fast_cos_c(angle) * 0.17f;
    {
        struct Moby *child = *(struct Moby **)(data + 0xac);
        if (child) {
            *(OvlQuad *)&child->pos = *(OvlQuad *)&moby->pos;
            child = *(struct Moby **)(data + 0xac);
            *(OvlQuad *)&child->rot = *(OvlQuad *)&moby->rot;
            advance_moby_animation(*(struct Moby **)(data + 0xac));
            FUN_L00_00250df8_x(*(struct Moby **)(data + 0xac));
            (*(struct Moby **)(data + 0xac))->flags |= 6;
        }
    }
    if (*(int *)(data + 0xc0) == 0 && moby->state > 1 && moby->visible && distance_xyz(&moby->pos, D_L12_00167240) < 30.0f) {
        FUN_L00_0025a120((char *)moby);
        moby->shadow_range = 24;
    }
}
/* Springs the moby's two rotation angles toward a heading taken from its data's direction vector. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC180), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

void update_blarg_helicopter_motion_tilt(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float ang = fast_subtract_rotations(fast_arc_tan(*(float *)(data + 0x70), *(float *)(data + 0x74)),
                             moby->rot.z);
    float target_rotation_y = *(float *)(data + 0x94) * 0.2268928f * fast_cos(ang) / (frame_time * 10.0f);
    float target_rotation_x = *(float *)(data + 0x94) * -0.2268928f * fast_sin(ang) / (frame_time * 10.0f);
    float updated_rotation_y, k0, rotation_y_damping, rotation_y_step_limit, k1, rotation_x_damping, rotation_x_step_limit;
    k0 = frame_time_sq * 0.5235988f;
    rotation_y_damping = frame_time_sq * 1.0471976f;
    rotation_y_step_limit = frame_time * 0.7853982f;
    updated_rotation_y = spring_turn((float *)(data + 0x9C), moby->rot.y, target_rotation_y, k0, rotation_y_damping, rotation_y_step_limit);
    k1 = frame_time_sq * 1.0471976f;
    rotation_x_damping = frame_time_sq * 2.0943952f;
    rotation_x_step_limit = frame_time * 1.2217305f;
    moby->rot.y = updated_rotation_y;
    moby->rot.x =
        spring_turn((float *)(data + 0x98), moby->rot.x, target_rotation_x, k1, rotation_x_damping, rotation_x_step_limit);
}
extern char *D_L12_001B0930_f[] __asm__("D_L12_001B0930");
extern float D_0015ED6C_f __asm__("D_0015ED6C");
extern float D_0015ED70_f __asm__("D_0015ED70");
extern void ahead_f(char *, float *, float) __asm__("FUN_L01_00274b78");
extern float atan_f(float, float) __asm__("FUN_001f9e90");
extern float turn_f(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern void appr_f(void *, float, float) __asm__("FUN_00213ed8");
extern void sub_f(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_f(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_f(void *, void *, void *) __asm__("FUN_001f9a10");
extern float dist_f(void *, void *) __asm__("FUN_001f9b48");
extern void update_blarg_helicopter_motion_tilt(struct Moby *) __asm__("FUN_L12_002eae00");
typedef struct { char pad[0x48]; float rz; } M_af68;
typedef struct { float x, y, z, w; } N_af68;

/* Follows a path: steers toward the current node, slows to a stop near the end, returns 1 once stopped. */
int follow_blarg_helicopter_path(struct Moby *moby, int path_index) {
    char *path = D_L12_001B0930_f[path_index];
    char *moby_vars = (char *)moby->pvars;
    float sum = 0.0f;
    float target_record_or_node_offset[20]; /* target record from the finder; also the step vector */
    int i;
    float target_heading;
    for (i = *(int *)(moby_vars + 0xB4); i < *(int *)path - 1; i++)
        sum += *(float *)(path + i * 16 + 0x1C);
    if (sum > 5.0f) {
        target_heading = atan_f(*(float *)((*(int *)(moby_vars + 0xB4) << 4) + path + 0x10) - moby->pos.x,
                   *(float *)((*(int *)(moby_vars + 0xB4) << 4) + path + 0x14) - moby->pos.y);
        ((M_af68 *)moby)->rz = turn_f((float *)(moby_vars + 0x88), ((M_af68 *)moby)->rz, target_heading, 0.004f, 0.3f, D_0015ED6C_f * 0.7853982f);
        appr_f(moby_vars + 0x94, D_0015ED6C_f * 10.0f, D_0015ED70_f * 7.0f);
    } else {
        ahead_f(moby, target_record_or_node_offset, 48.0f);
        target_heading = atan_f(target_record_or_node_offset[0] - moby->pos.x, target_record_or_node_offset[1] - moby->pos.y);
        ((M_af68 *)moby)->rz = turn_f((float *)(moby_vars + 0x88), ((M_af68 *)moby)->rz, target_heading, 0.004f, 0.3f, D_0015ED6C_f * 0.7853982f);
        appr_f(moby_vars + 0x94, 0.0f, D_0015ED70_f * 9.0f);
        sum = *(float *)(moby_vars + 0x94);
        if (sum == 0.0f)
            return 1;
    }
    sub_f(target_record_or_node_offset, &((N_af68 *)path)[*(int *)(moby_vars + 0xB4) + 1], &moby->pos);
    norm_f(moby_vars + 0x70, target_record_or_node_offset, *(float *)(moby_vars + 0x94));
    add_f(&moby->pos, &moby->pos, moby_vars + 0x70);
    if (dist_f(&moby->pos, &((N_af68 *)path)[*(int *)(moby_vars + 0xB4) + 1]) < 2.0f) {
        (*(int *)(moby_vars + 0xB4))++;
        if (*(int *)path - 1 < *(int *)(moby_vars + 0xB4))
            *(int *)(moby_vars + 0xB4) = *(int *)path - 1;
    }
    update_blarg_helicopter_motion_tilt(moby);
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC5A0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
float absolute_float(float input) __asm__("func_001F99C0");

void process_blarg_helicopter_hits(struct Moby *moby) {
    int hit_processing_status;
    float fv;
    int hit_reaction_code;
    char *vars;
    char *hit_record;
    float hit_to_moby_angle, hit_yaw_offset, knockback_rotation_y, knockback_rotation_x;
    vars = (char *)moby->pvars;
    fv = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_reaction_code = hit_reaction(moby, hit_record, vars + 0x20, 0, &hit_processing_status, &fv, 0, 4);
    if (hit_processing_status != 1 && moby->state != 6 &&
        *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0xB8 &&
        *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0x199) {
        float remaining_health;
        if (*(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x1CA)
            fv = 10.0f;
        remaining_health = *(float *)(vars + 0x20) - fv;
        *(float *)(vars + 0x20) = remaining_health;
        if (hit_reaction_code == 1 || remaining_health <= 0.0f) {
            allocate_voice_for_target_entry(2, 0, (int)moby);
            moby->state = 6;
        } else if (hit_reaction_code != 0xB) {
            *(unsigned char *)(vars + 0x67) = 0xB4;
            trooper_anim_update(moby, (short *)(vars + 0x60));
            if (*(float *)(hit_record + 0x1C) == 5627.925f && moby->state != 5) {
                hit_to_moby_angle = fast_arc_tan(moby->pos.x - *(float *)(hit_record + 0),
                                 moby->pos.y - *(float *)(hit_record + 4));
                hit_yaw_offset = fast_subtract_rotations(hit_to_moby_angle, moby->rot.z);
                knockback_rotation_y = fast_cos(hit_yaw_offset) * 0.08726646f;
                knockback_rotation_x = fast_sin(hit_yaw_offset) * -0.34906584f;
                if (absolute_float(moby->rot.x) < absolute_float(knockback_rotation_x)) {
                    moby->rot.x = knockback_rotation_x;
                    *(float *)(vars + 0x98) = 0.0f;
                }
                if (absolute_float(moby->rot.y) < absolute_float(knockback_rotation_y)) {
                    moby->rot.y = knockback_rotation_y;
                    *(float *)(vars + 0x9C) = 0.0f;
                }
                *(float *)(vars + 0x94) = frame_time * 4.0f;
                *(short *)(vars + 0x8E) = scale_game_frames(0x3C);
                moby->command = moby->state;
                moby->state = 5;
                allocate_voice_for_target_entry(1, 0, (int)moby);
            }
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, vars + 0x60);
}
/* Data of the second watcher (edwina_update): the L16 watcher layout with
 * the turret and base mobys it spawns at 0x164/0x168. */
typedef struct {
    char pad00[0x20];
    L16WatchActivation activation;
    char body[0x60];
    float padC0;
    float pitch;
    float yaw;
    float padCC;
    float height;
    char padD4[0xC];
    char head[0x60];
    char pad140[8];
    float head_yaw;
    char pad14C[0x14];
    int region;
    char *turret;
    char *base;
    char pad16C[4];
    float target[4];
    char pad180[4];
    int moving_timer;
    int target_timer;
} L12WatchData;

typedef struct {
    char pad[0x46C];
    int hidden;
} L12WatchGlobals;

extern L12WatchGlobals D_0013D5B0_b570 __asm__("D_0013D5B0") __attribute__((section(".data")));
extern int D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern float D_0015ED64_b570 __asm__("D_0015ED64");
extern void attach_shared_manipulator_to_matching_class_moby(void *) __asm__("FUN_L03_002db480");
extern void blend_moby_animation_b570(void *, int, int, int) __asm__("FUN_00212f90");
extern void grant_inventory_item(int, int) __asm__("FUN_L00_00260860");

void edwina_update(L16WatchMoby *m) __asm__("FUN_L12_002eb570");
void edwina_update(L16WatchMoby *moby) {
    enum {
        EDWINA_INITIALIZE = 0,
        EDWINA_UPDATE_DIALOGUE = 1,
        EDWINA_APPLY_DIALOGUE_COMPLETION = 2
    };
    L16WatchScratch scratch;
    L12WatchData *moby_data = (L12WatchData *)moby->data;
    float rate;
    float head_rate;
    int tracking;
    attach_shared_manipulator_to_matching_class_moby(moby);
    if (D_0013D5B0_b570.hidden == 0) {
        if (moby->animation != 1)
            blend_moby_animation_b570(moby, 1, 0, 2);
    } else if (moby->animation != 0) {
        blend_moby_animation_b570(moby, 0, 0, 2);
    }
    switch (moby->state) {
    case EDWINA_INITIALIZE:
        moby->state = 1;
        initialize_npc_dialogue(moby, &moby_data->activation);
        moby_data->activation.radius = 1.7f;
        initialize_moby_grounding(moby);
        if (alternate_item_available[0x13C])
            *(short *)((char *)moby_data + 0x56) = -1;
        break;
    case EDWINA_UPDATE_DIALOGUE:
        if (alternate_item_available[0x13C] == 0 && update_npc_dialogue(moby, &moby_data->activation)) {
            FUN_L01_002783a8_c(moby, 2.52f);
            *(int *)((char *)moby + 0x58) = 0;
            moby->state = 2;
        }
        break;
    case EDWINA_APPLY_DIALOGUE_COMPLETION:
        if (D_L12_0015F5C4 != 2) {
            *(float *)((char *)moby + 0x58) = 1.0f;
            jet_clear_persistent(moby->widget);
            if (moby_data->region != -1) {
                char *p = D_L12_001600EC + (moby_data->region << 7);
                jet_reset_camera(p + 0x30, p + 0x70);
            }
            moby->state = 1;
            if (*(short *)((char *)moby_data + 0x24) == 2) {
                grant_inventory_item(4, 0);
                FUN_L00_00263d40_c(0x2EE6, -1);
                if (moby_data->region != -1) {
                    char *p = D_L12_001600EC + (moby_data->region << 7);
                    jet_reset_camera(p + 0x30, p + 0x70);
                }
                memcard_save_data(0, -1);
            }
        }
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = (char *)&D_0013E633_watch + 0xE9D;
        tracking = 1;
        if (vector_distance(moby->position, player) < 8.0f &&
            FUN_001f9e90_cf(
                moby->yaw,
                FUN_001fa688_cf(((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[0] -
                                    moby->position[0],
                                ((L16WatchPlayer *)((char *)&D_0013E633_watch + 0xE1D))->aim[1] -
                                    moby->position[1])) < 1.5707964f) {
            if (vector_length_xyz(player + 0x80) > 0.01f)
                moby_data->moving_timer = scale_game_frames(120);
            else
                tick_countdown_32(&moby_data->moving_timer);
        } else if (moby_data->moving_timer) {
            moby_data->moving_timer = 0;
            qcopy(moby_data->target, (char *)&D_0013E633_watch + 0xEED);
        }
        if (tick_countdown_32(&moby_data->target_timer)) {
            float heading;
            moby_data->target_timer =
                random_float_between_cf(multiply_global_factor_ed64(truncate_float_to_s32_cf(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, truncate_float_to_s32_cf(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(moby_data->target, 6.0f, heading,
                                   truncate_float_to_s32_cf(0.0f, 30.0f) * DEG_TO_RAD);
            add_vector_xyz(moby_data->target, moby_data->target, moby->position);
        }
        if (moby_data->moving_timer) {
            qcopy(scratch.target, (char *)&D_0013E633_watch + 0xEED);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_data->target);
        }
    }
    if (D_0013D5B0_b570.hidden == 0) {
        if (moby_data->turret == 0) {
            char *o = func_0020D348_m_x(0x4C1);
            moby_data->turret = o;
            if (o) {
                *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)moby + 0x32);
                moby_data->turret[0x31] = 1;
                {
                    char *p = moby_data->turret;
                    qcopy(p + 0x10, (char *)moby + 0x10);
                    qcopy(p + 0x40, (char *)moby + 0x40);
                    FUN_L00_00250df8_x(p);
                }
                *(unsigned short *)(moby_data->turret + 0x34) = 0;
                blend_moby_animation_b570(moby_data->turret, 1, 0, 2);
            }
        } else if (D_L12_0015F5C4 == 2) {
            *(unsigned short *)(moby_data->turret + 0x34) |= 1;
            moby_data->turret[0x31] = 0;
        } else {
            *(unsigned short *)(moby_data->turret + 0x34) &= ~1;
            moby_data->turret[0x31] = 1;
        }
        if (moby_data->base == 0) {
            char *o = func_0020D348_m_x(0x196);
            moby_data->base = o;
            if (o) {
                *(unsigned short *)(o + 0x32) = *(unsigned short *)((char *)moby + 0x32);
                moby_data->base[0x31] = 1;
                {
                    char *p = moby_data->base;
                    qcopy(p + 0x10, (char *)moby + 0x10);
                    qcopy(p + 0x40, (char *)moby + 0x40);
                    FUN_L00_00250df8_x(p);
                }
            }
        } else if (D_L12_0015F5C4 == 2) {
            *(unsigned short *)(moby_data->turret + 0x34) |= 1;
            moby_data->turret[0x31] = 0;
            *(unsigned short *)(moby_data->base + 0x34) |= 1;
            moby_data->base[0x31] = 0;
        } else {
            *(unsigned short *)(moby_data->turret + 0x34) &= ~1;
            moby_data->turret[0x31] = 1;
            *(unsigned short *)(moby_data->base + 0x34) &= ~1;
            moby_data->base[0x31] = 1;
        }
    } else {
        if (moby_data->base) {
            FUN_0020c828_c(moby_data->base);
            moby_data->base = 0;
        }
        if (moby_data->turret) {
            FUN_0020c828_c(moby_data->turret);
            moby_data->turret = 0;
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        subtract_vector_xyz(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(FUN_001fa688_cf(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -FUN_001fa688_cf(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_data->pitch = pitch;
        moby_data->head_yaw = moby_data->yaw = yaw * 0.5f;
    }
    if (D_0015EDB0_b)
        moby_data->height = 2.75f;
    update_joint_animation(rate * D_0015ED64_b570, head_rate * D_0015ED64_b570, moby, moby_data->body, 1);
    update_joint_animation(rate * D_0015ED64_b570, head_rate * D_0015ED64_b570, moby, moby_data->head, 0);
}
/* Hit handler: counts hits toward a help message, bursts debris and blinks the moby until its timer runs out. */
extern char D_0013D408[];
extern int tick_countdown_32_alt(int *arg0) __asm__("FUN_001f9740");
extern s32 D_L12_001619B4 __attribute__((sda));
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void spawn_randomized_bolt_drop(void *, int, int, int, int) __asm__("FUN_L00_00260a88");
extern void FUN_L00_00263d40(int, int);
extern void clear_vector(void *) __asm__("func_001F99F8");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void process_helicopter_hits_and_reactivation(struct Moby *moby, int flag) __asm__("FUN_L12_002ebca8");

void process_helicopter_hits_and_reactivation(struct Moby *moby, int flag) {
    char *moby_vars = (char *)moby->pvars;
    float explosion_position[4];
    float explosion_vector[4];
    unsigned char *help_message_state;
    if (find_moby_hit(moby, 0xA30000, 0) != 0) {
        if (hero.state.current != 0x32) {
            helicopter_help_message_hit_count += 1;
            if (helicopter_help_message_hit_count >= 5) {
                help_message_state = (unsigned char *)(D_0013D408);
                if (help_message_state[0x15] == 0) {
                    help_message_state[0x15] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    FUN_L00_00263d40(0x53D6, -1);
                }
            }
        }
        qcopy(explosion_position, &moby->pos);
        clear_vector(explosion_vector);
        spawn_randomized_bolt_drop(moby, 0x1F, 0x4A, 2, -1);
        *(int *)(moby_vars + 0xA4) = scale_game_frames(500);
        spawn_explosion(moby, explosion_vector, explosion_position, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, -1, 3.0f, 15.0f, 1,
                         1, -1, 0);
    }
    moby->hit_slot = 0xFF;
    if (*(int *)&D_L12_001619B4 != 0 && flag != 0) {
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
    }
    if (*(int *)(moby_vars + 0xA4) == 0 && (moby->flags & 1) == 0)
        return;
    if (tick_countdown_32_alt((int *)(moby_vars + 0xA4)) != 0 && flag != 0 &&
        *(int *)&D_L12_001619B4 == 0) {
        moby->collision = moby->pclass->collision;
        moby->flags = (moby->flags & 0xFFBE) | 0x1000;
    } else {
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002ED228), where it is exact; names translated to the US level program. */

extern char *D_L12_001B0930[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern void initialize_path_descriptor(char *p) __asm__("FUN_L01_0028b410");
extern void FUN_L12_002ebca8_c(char *m, int flag) __asm__("FUN_L12_002ebca8");
extern void add_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
void check_scaled_vector_bsphere(char *vec, float scale) __asm__("FUN_L01_00275690");
extern int func_L01_00276680_i(char *vec, float scale) __asm__("FUN_L01_00275690");

extern float D_L12_001619AC __attribute__((sda));
void helicopter_update(char *m) __asm__("FUN_L12_002ebea8");
void helicopter_update(char *moby) {
    enum {
        HELICOPTER_INITIALIZE_PATH = 0,
        HELICOPTER_FOLLOW_PATH = 1
    };
    char *moby_vars = *(char **)(moby + 0x78);
    float A[4];
    float B[4];
    float C[4];
    float E[4];
    if (moby_vars != 0) {
        int moby_query_result = func_L01_00276680_i(moby, convert_integer_to_float(*(short *)(moby + 0x32)));
        FUN_L12_002ebca8_c(moby, moby_query_result == -1);
        switch ((unsigned char)moby[0x20]) {
        case HELICOPTER_INITIALIZE_PATH:
            if (*(int *)(moby_vars + 0x74) != -1) {
                char *tbl;
                int initial_node_index;
                int last_node_index;
                initialize_path_descriptor(moby_vars + 0x60);
                tbl = D_L12_001B0930[*(int *)(moby_vars + 0x74)];
                *(char **)(moby_vars + 0x70) = tbl;
                initial_node_index = *(int *)(moby_vars + 0xA0);
                last_node_index = *(int *)tbl - 1;
                if (last_node_index < initial_node_index) initial_node_index = last_node_index;
                qcopy((moby + 0x10), tbl + initial_node_index * 16 + 0x10);
                moby_vars[0x2A] = 0x10;
                moby_vars[0x28] = 2;
                moby_vars[0x2C] = 0x2C;
                *(float *)(moby_vars + 0x30) = 1.4f;
                moby_vars[0x2B] = 1;
                *(int *)(moby_vars + 0x60) = initial_node_index;
                moby[0x20] = 1;
                *(short *)(moby + 0x32) = 0xAA;
                *(unsigned short *)(moby + 0x34) |= 0x1000;
            }
            break;
        case HELICOPTER_FOLLOW_PATH: {
            char *path_state = moby_vars + 0x60;
            int current_node_index = *(int *)path_state;
            char *tbl = *(char **)(path_state + 0x10);
            int step = *(char *)(path_state + 4);
            int node_count = *(int *)tbl;
            int previous_node_index = (current_node_index + node_count - step) % node_count;
            int next = (current_node_index + node_count + step) % node_count;
            float segment_heading_delta;
            float segment_heading_delta_per_distance;
            qcopy(A, tbl + previous_node_index * 16 + 0x10);
            qcopy(B, tbl + current_node_index * 16 + 0x10);
            qcopy(C, tbl + next * 16 + 0x10);
            subtract_vector_xyz_c(A, B, A);
            subtract_vector_xyz_c(B, C, B);
            segment_heading_delta = fast_arc_tan(A[0], A[1]);
            segment_heading_delta = fast_subtract_rotations(segment_heading_delta, fast_arc_tan(B[0], B[1]));
            segment_heading_delta_per_distance = segment_heading_delta / vector_length_xyz_c(A);
            *(float *)(moby + 0x40) = fast_add_rotations_c(*(float *)(moby + 0x40),
                fast_subtract_rotations(segment_heading_delta_per_distance * D_L12_001619AC, *(float *)(moby + 0x40)) / helicopter_rotation_smoothing_divisor);
            if (distance_xyz(moby + 0x10, C) < helicopter_path_speed * D_0015ED6C) {
                char *path;
                *(int *)(moby_vars + 0x60) = next;
                path = *(char **)(path_state + 0x10);
                if (next == *(int *)path - 1) {
                    qcopy((moby + 0x10), path + 0x10);
                }
            }
            subtract_vector_xyz_c(E, C, moby + 0x10);
            normalize_vector_xyz(E, E, helicopter_path_speed * D_0015ED6C);
            add_vector_xyz_c2(moby + 0x10, moby + 0x10, E);
            *(float *)(moby + 0x48) = fast_add_rotations_c(*(float *)(moby + 0x48),
                fast_subtract_rotations(fast_arc_tan(E[0], E[1]), *(float *)(moby + 0x48)) / helicopter_rotation_smoothing_divisor);
            *(float *)(moby + 0x44) = 0.52359879f;
            break;
        }
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Idle critter: waits a random time, then plays one of its fidget animations (or reacts when the hero comes close). */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002ED550), where it is exact; names translated to the US level program. */

extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_L00_001fefc8(void *);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");

void crane_arm_update(unsigned char *m) __asm__("FUN_L12_002ec1d0");
void crane_arm_update(unsigned char *moby) {
    enum {
        CRANE_ARM_INITIALIZE = 0,
        CRANE_ARM_WAIT_DELAY = 1,
        CRANE_ARM_WAIT_ANIMATION_FLAG = 2
    };
    enum {
        CRANE_ARM_ANIM_BASE = 0,
        CRANE_ARM_ANIM_DELAY_A = 1,
        CRANE_ARM_ANIM_DELAY_B = 2
    };
    enum {
        CRANE_ARM_ANIM_BASE_TO_DELAY_A = 3,
        CRANE_ARM_ANIM_RETURN_TO_BASE_A = 4,
        CRANE_ARM_ANIM_BASE_TO_DELAY_B = 5,
        CRANE_ARM_ANIM_RETURN_TO_BASE_B = 6
    };
    float hero_distance = vector_distance(moby + 0x10, &hero.motion.pos);
    switch (moby[0x20]) {
    case CRANE_ARM_INITIALIZE:
        if (moby[0x53] != 0)
            blend_moby_animation(moby, 0, 0, 0);
        moby[0x20] = 1;
        moby[0xBC] = random_integer_below(scale_game_frames(0xB4));
        break;
    case CRANE_ARM_WAIT_DELAY:
        if (moby[0x52] == 0 && hero_distance < 9.0f && moby[0xBC] < 2)
            moby[0xBC] = 2;
        if (hero_distance < 12.0f && moby[0x52] == moby[0x53] && moby[0x52] != 0)
            moby[0xBC] = 0;
        if (FUN_L00_001fefc8(moby + 0xBC)) {
            switch (moby[0x52]) {
            case CRANE_ARM_ANIM_BASE:
                if (random_integer_below(0x100) & 1) {
                    if (moby[0x53] != 3)
                        blend_moby_animation(moby, 3, 0, 0);
                } else {
                    if (moby[0x53] != 5)
                        blend_moby_animation(moby, 5, 0, 0);
                }
                break;
            case CRANE_ARM_ANIM_DELAY_A:
                if (moby[0x53] != 4)
                    blend_moby_animation(moby, 4, 0, 0);
                break;
            case CRANE_ARM_ANIM_DELAY_B:
                if (moby[0x53] != 6)
                    blend_moby_animation(moby, 6, 0, 0);
                break;
            }
            moby[0x20] = 2;
        }
        break;
    case CRANE_ARM_WAIT_ANIMATION_FLAG:
        if (moby[0x70] & 2) {
            switch (moby[0x52]) {
            case CRANE_ARM_ANIM_BASE_TO_DELAY_A:
                if (moby[0x53] != 1)
                    blend_moby_animation(moby, 1, 0, 0);
                break;
            case CRANE_ARM_ANIM_RETURN_TO_BASE_A:
            case CRANE_ARM_ANIM_RETURN_TO_BASE_B:
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, 0);
                break;
            case CRANE_ARM_ANIM_BASE_TO_DELAY_B:
                if (moby[0x53] != 2)
                    blend_moby_animation(moby, 2, 0, 0);
                break;
            }
            moby[0x20] = 1;
            moby[0xBC] = random_integer_below(scale_game_frames(0xB4));
        }
        break;
    }
}
