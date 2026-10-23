/* Level 18 moby classes: rocket, mine, thruster_pack_lock_green, thruster_pack_lock_red, floating_platform. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/gameplay/state/usage_stats.h"
#define MOBY(p) ((struct Moby *)(p))

#define NOT_SDA

#define MACRO_ADDR

/* Switch to state four while active, then restore the saved state. */
/* Ported from rac1-decomp (src/overlays/l18/vendor_002A8400.c: func_L18_002D7580), where it is exact; names translated to the US level program. */


extern int store_moby_in_suck_cannon(struct Moby *) __asm__("FUN_L00_002dbb20");

int FUN_L18_002d6190(struct Moby *moby) {
    int active = store_moby_in_suck_cannon(moby);
    if (active != 0) {
        moby->state = 4;
    } else if (moby->state == 4) {
        moby->state = moby->command;
    }
    return active;
}
extern int scale_ticks(int) __asm__("FUN_001f96f8");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D7670), where it is exact; names translated to the US level program. */

void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

void hide_moby_and_disable_collision(struct Moby *moby) __asm__("FUN_L18_002d6280");

void hide_moby_and_disable_collision(struct Moby *moby) {
    unsigned short flags;
    moby->state = 5;
    if (moby->prev_seq != 0) {
        blend_moby_animation(moby, 0, 0, scale_ticks(10));
    }
    flags = moby->flags;
    moby->visible = 0;
    flags |= 1;
    moby->collision = 0;
    flags &= 0xEFFF;
    moby->flags = flags;
}
extern int D_L18_001AC240_u[] __asm__("D_L18_001AC240");
extern short D_L18_0015FFD8_d __asm__("D_L18_0015FFD8");
/* 0x002d6d50, 108 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Sets the data fields 0x30 and 0x24 of each listed moby whose class is 0x247.
void FUN_L18_002d6d50(int moby_list_index) {
    short *moby_list_entry = (short *)D_L18_001AC240_u[moby_list_index];
    if (moby_list_entry != 0) {
        char *base = *(char **)&D_L18_0015FFD8_d;
        do {
            int moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
            char *moby = (char *)moby_byte_offset + (int)base;
            if (*(short *)(moby + 0xA6) == 0x247) {
                char *data = *(char **)(moby + 0x78);
                *(int *)(data + 0x30) = 1;
                *(float *)(data + 0x24) = 1.5f;
            }
        } while (*moby_list_entry++ >= 0);
    }
}

/* Initialize the vendor action mode and scale. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D81B0), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x24];
    float scale;
    char pad28[4];
    int mode;
} Level18VendorActionData;

typedef struct {
    char pad0[0x78];
    Level18VendorActionData *data;
} Level18VendorActionMoby;

/* Switches the data to mode 5 at scale 1.5. */
void FUN_L18_002d6dc0(Level18VendorActionMoby *moby) {
    Level18VendorActionData *data = moby->data;
    data->mode = 5;
    data->scale = 1.5f;
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D9358), where it is exact; names translated to the US level program. */

extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
s32 allocate_voice_for_group_entry(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0022dba0");

void request_thruster_pack_lock_red_wait(struct Moby *moby, int value) __asm__("FUN_L18_002d7f68");

void request_thruster_pack_lock_red_wait(struct Moby *moby, int value) {
    char *data = (char *)moby->pvars;
    if (moby->state != 2) {
        moby->state = 2;
        *(int *)(data + 0x6C) = value;
    }
    if (!voice_is_playing(moby, *(int *)(data + 0x70))) {
        *(int *)(data + 0x70) = allocate_voice_for_group_entry(0x12, 0, moby);
    }
}
/* turns the moby's heading toward a target, clamped per state, and updates its hover offset */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D9B00), where it is exact; names translated to the US level program. */

extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");

extern float D_L18_00161A48 __attribute__((sda));
extern float D_L18_00161A4C __attribute__((sda));
void update_floating_platform_tilt_and_bob(struct Moby *moby) __asm__("FUN_L18_002d8710");

void update_floating_platform_tilt_and_bob(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float unclamped_rotation_y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x6C));
    moby->rot.y = unclamped_rotation_y;
    if (moby->state == 5) {
        if (unclamped_rotation_y > 0.5235988f) {
            moby->rot.y = 0.5235988f;
        } else if (unclamped_rotation_y < -0.5235988f) {
            moby->rot.y = -0.5235988f;
        }
    } else if (moby->state == 4) {
        if (unclamped_rotation_y > 0.08726646f) {
            moby->rot.y = 0.08726646f;
        } else if (unclamped_rotation_y < -0.08726646f) {
            moby->rot.y = -0.08726646f;
        }
    } else {
        if (unclamped_rotation_y > 0.034906585f) {
            moby->rot.y = 0.034906585f;
        } else if (unclamped_rotation_y < -0.034906585f) {
            moby->rot.y = -0.034906585f;
        }
    }
    *(float *)(data + 0x74) = fast_add_rotations(*(float *)(data + 0x74),
                                           D_L18_00161A4C * DEG_TO_RAD * frame_time);
    moby->pos.z = *(float *)(data + 0x64) +
                              D_L18_00161A48 * fast_sin(*(float *)(data + 0x74));
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l18/vendor_002A8400.c: func_L18_002D9C48), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x20];
    unsigned char state;
} Level18State;

int request_floating_platform_sink(Level18State *platform_moby) __asm__("FUN_L18_002d8858");

int request_floating_platform_sink(Level18State *platform_moby) {
    if (platform_moby->state == 1) {
        return 1;
    }
    if (platform_moby->state == 2) {
        platform_moby->state = 5;
    }
    return 0;
}
/* UpdateMoby_628 */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD650), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int game_frame __asm__("D_L18_0015F5CC");
extern int tick_countdown_32(int *) __asm__("FUN_001f9740");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern void play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void FUN_L00_00259a88(int, void *);
extern void draw_class_274_ring_effect() __asm__("FUN_L18_002dc4b8");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

extern float D_L18_00161CA0 __attribute__((sda));
void update_class_274_ring_effect(struct Moby *moby) __asm__("FUN_L18_002dc260");

void update_class_274_ring_effect(struct Moby *moby) {
    char hit_descriptor[0x30];
    float hit_direction_vector[4];
    float *moby_vars = (float *)moby->pvars;
    char *target_position;
    char *pos;
    int flag;

    if (tick_countdown_32((int *)(moby_vars + 2))) {
        mark_moby_for_removal(moby);
        return;
    }
    pos = &moby->pos;
    target_position = D_0013E533 + 0xE9D;
    if (vector_distance(pos, target_position) < moby_vars[0] + 0.5f) {
        fast_vec_sub(hit_direction_vector, target_position, pos);
        hit_direction_vector[2] = 0.0f;
        fast_vec_normalize(hit_direction_vector, hit_direction_vector, frame_time * 4.0f);
        hit_direction_vector[2] = frame_time * 4.0f;
        make_hit_descriptor(hit_descriptor, (int)moby, 0x10001, hit_direction_vector, 1.0f);
        FUN_L00_00259a88(*(int *)(target_position + 0x2000), hit_descriptor);
    }
    flag = game_frame & 4;
    moby_vars[1] = moby_vars[1] + D_L18_00161CA0 * frame_time;
    if (flag == 0) {
        play_entity_sound(*(int *)(moby_vars + 3), moby_vars[0], pos, 0x10000, 1.0f, 1.0f, 0, 1, 0);
    }
    enqueue_callback_list_1(draw_class_274_ring_effect, moby);
}
#include "qcopy.h"
extern void *create_moby() __asm__("FUN_0020c4f8");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD7D0), where it is exact; names translated to the US level program. */

char *create_class_274_ring_effect(void *owner, void *vector, float value) __asm__("FUN_L18_002dc3e0");

char *create_class_274_ring_effect(void *owner, void *vector, float value) {
    char *moby = create_moby(0x274);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        ((unsigned char *)moby)[0x30] = 0xFF;
        qcopy(moby + 0x10, vector);
        *(float *)data = value;
        *(int *)(data + 0x08) = 3;
        *(void **)(data + 0x0C) = owner;
    }
    return moby;
}
#define NOT_SDA

#define MACRO_ADDR

/* Read the vendor moby's current float value. */
/* Ported from rac1-decomp (src/overlays/l18/vendor_002A8400.c: func_L18_002E0F80), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x60];
    float value;
} Level18VendorFloatData;

typedef struct {
    char pad0[0x78];
    Level18VendorFloatData *data;
} Level18VendorFloatMoby;

float get_position_and_rotation_transition_progress(Level18VendorFloatMoby *moby) __asm__("FUN_L18_002dfb90");

float get_position_and_rotation_transition_progress(Level18VendorFloatMoby *moby) {
    return moby->data->value;
}
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002E36B0), where it is exact; names translated to the US level program. */

extern void vu_euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");

char *create_class_326_moby(void *vector) __asm__("FUN_L18_002e22c0");

char *create_class_326_moby(void *vector) {
    char *moby = create_moby(0x326);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        moby[0x30] = 0x40;
        moby[0x20] = 0;
        *(char **)(data + 0x0C) = moby;
        *(int *)(moby + 0x94) = 0;
        *(unsigned short *)(moby + 0x34) |= 0x41;
        qcopy(moby + 0x10, vector);
        clear_u64_value(moby + 0x40);
        vu_euler_rotation_basis(moby + 0xC0, moby + 0x40);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB4E0), where it is exact; names translated to the US level program. */

char *create_class_3d7_moby(void *owner, void *vector, int value) __asm__("FUN_L18_002ea0f0");

char *create_class_3d7_moby(void *owner, void *vector, int value) {
    char *moby = create_moby(0x3D7);
    if (moby != 0) {
        char *data;
        ((unsigned char *)moby)[0x30] = 0xFF;
        qcopy(moby + 0x10, vector);
        data = *(char **)(moby + 0x78);
        *(void **)(data + 0x20) = owner;
        *(int *)(data + 0x28) = 5;
        *(int *)(data + 0x34) = value;
        *(int *)(data + 0x24) = 0;
    }
    return moby;
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB558), where it is exact; names translated to the US level program. */

void refresh_moby_position_and_activate(struct Moby *moby, void *moby_position, void *position, int active, float speed) __asm__("FUN_L18_002ea168");

void refresh_moby_position_and_activate(struct Moby *moby, void *moby_position, void *position, int active, float speed) {
    char *data = (char *)moby->pvars;
    qcopy(&moby->pos, moby_position);
    *(int *)(data + 0x28) = scale_ticks(5);
    *(float *)(data + 0x24) = speed;
    if (active && moby->state == 0) {
        moby->state = 1;
        *(int *)(data + 0x28) = 0;
        qcopy(data, position);
    }
}

#include "sda.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002A8400), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
} __attribute__((aligned(16))) QVa840;

typedef struct {
    char pad0[0x10];
    char pos[0x10];
    unsigned char b20;
    char pad21[0xB];
    float f2C;
    unsigned char b30;
    unsigned char b31;
    short s32;
    unsigned short h34;
    char pad36[0x12];
    float f48;
    char pad4C[0x2C];
    Child *child;
    char pad7C[0x14];
    int f90;
} Obj;

extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern void func_L00_0025F4A8_alt(void *, void *, void *, float, float, int, int, int, float, float,
                                  float, float, int, float, int, int, int,
                                  int) __asm__("FUN_L00_0025e450");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern unsigned char *D_L18_001B2ADC;
extern unsigned char *FUN_L00_002712b8(void *pos, float *vec, int s, int a, int col, int n, float x,
                                       float y, float z, float w, float pw);
extern void FUN_L00_00257d78(float *, float, float);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void mark_moby_for_removal_a7220(void *obj) __asm__("FUN_0020c828");
extern char *D_L18_001B2DDC_c __asm__("D_L18_001B2ADC") __attribute__((section(".data")));

void update_projectile_motion_collision_and_trail(char *moby) __asm__("FUN_L18_002a7220");

void update_projectile_motion_collision_and_trail(char *moby) {
    float next_position[4];
    char hit_descriptor[0x30];
    float hit_direction_vector[4];
    float effect_vector[4];
    float effect_spawn_position[4];
    float world_effect_direction[4];
    char *data = *(char **)(moby + 0x78);
    float *p;
    float movement_step_length, target_distance;
    char *referenced_moby;

    *(float *)(moby + 0x40) = fast_add_rotations(*(float *)(moby + 0x40), frame_time * 6.2831855f);
    if (*(short *)(data + 0x12) != 0) {
        movement_step_length = vector_length_xyz(data);
        subtract_vector_xyz(data, &hero.motion.unkC0, moby + 0x10);
        target_distance = vector_length_xyz(data);
        if (movement_step_length < target_distance) {
            normalize_vector_xyz(data, data, movement_step_length);
            *(float *)(moby + 0x48) = fast_arc_tan(*(float *)data, *(float *)(data + 4));
            *(float *)(moby + 0x44) = -fast_arc_tan(vector_length_xy(data), *(float *)(data + 8));
        } else if (target_distance < 0.1f) {
            mark_moby_for_removal_a7220(moby);
            return;
        }
    }
    add_vector_xyz(next_position, moby + 0x10, data);
    referenced_moby = *(char **)(data + 0x14);
    if (referenced_moby != 0 && *(short *)(referenced_moby + 0xA6) == 0x2E && ((unsigned char *)referenced_moby)[0x20] != 0xFE &&
        ((unsigned char *)referenced_moby)[0x20] != 0xFD) {
        subtract_vector_xyz(hit_direction_vector, moby + 0x10, referenced_moby + 0x10);
        hit_direction_vector[2] = 0.0f;
        normalize_vector_xyz(hit_direction_vector, hit_direction_vector, 2.0f);
        hit_direction_vector[3] = 5627.925f;
    } else {
        normalize_vector_xyz(hit_direction_vector, data, 2.0f);
    }
    make_hit_descriptor(hit_descriptor, (int)moby, 1, hit_direction_vector, 2.0f);
    *(short *)(hit_descriptor + 0x1A) = *(unsigned short *)(moby + 0xA6);
    if (collision_line(moby + 0x10, next_position, 0, *(void **)(data + 0x14), hit_descriptor) != 0 ||
        fast_dec_timer_s16(data + 0x10) != 0) {
        func_L00_0025F4A8_alt(moby, data, moby + 0x10, 1.5f, 1.0f, 10, 6, 4, 1.0f, 0.5f, 9.0f, 0.5f,
                              0, 0.0f, 0, 0, -1, 0);
        mark_moby_for_removal_a7220(moby);
    } else {
        unsigned char *particle;
        qcopy(moby + 0x10, next_position);
        FUN_L00_00257d78(effect_vector, frame_time * 0.1f, frame_time * 0.2f);
        scale_vector_xyz(effect_spawn_position, data, random_float_between(0.0f, 1.0f));
        add_vector_xyz(effect_spawn_position, effect_spawn_position, moby + 0x10);
        particle = FUN_L00_002712b8(effect_spawn_position, effect_vector, scale_game_frames(10), 0x7F, 0x606060, 3, 40000.0f, 1000.0f,
                             1.0f, -0.0002f, 0.0f);
        if (particle != 0) {
            particle[2] = *D_L18_001B2DDC_c;
            particle[3] = 0x44;
        }
        *(OvlQuad *)world_effect_direction = 0;
        world_effect_direction[2] = 0.02f;
        transform_vector_by_basis(world_effect_direction, world_effect_direction, moby + 0xC0);
        FUN_L00_0026cbb0(effect_spawn_position, world_effect_direction, 0x4F007FFF, 0x1FFFFFFF, scale_game_frames(10), 1, 20000.0f);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002A87D0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");

char *create_class_29_moby(struct Moby *owner, float *spawn_position, float *initial_movement_step, int homing_enabled) __asm__("FUN_L18_002a75f0");

char *create_class_29_moby(struct Moby *owner, float *spawn_position, float *initial_movement_step, int homing_enabled) {
    char *moby = func_0020D348_m(0x29);
    if (moby != 0) {
        char *data;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(long *)(moby + 0x38) = owner->spawn_frame;
        *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * 3.0f;
        qcopy(moby + 0x10, spawn_position);
        data = *(char **)(moby + 0x78);
        qcopy(data, initial_movement_step);
        *(short *)(data + 0x10) = scale_game_frames(0xF0);
        *(short *)(data + 0x12) = homing_enabled;
        *(char **)(data + 0x14) = owner;
        *(float *)(moby + 0x48) = fast_arc_tan(initial_movement_step[0], initial_movement_step[1]);
        *(float *)(moby + 0x44) = -fast_arc_tan(vector_length_xy(initial_movement_step), initial_movement_step[2]);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D6440), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x24];
    float f24;
    float f28;
    int c;
    int lim;
    int end;
    int flags;
} L18D;

extern char *D_L18_00174858;
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern float FUN_L04_001f1978(float);
extern int D_L18_0015F5C4;
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern float D_L18_00161964;
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
void spawn_rocket_trail_effects(void *mobyp, void *vec) __asm__("FUN_L18_002d5488");
extern void spawn_rocket_explosion(void *, void *) __asm__("FUN_L18_002d5610");
void draw_rocket_target_pulse_quad(struct Moby *m) __asm__("FUN_L18_002d5768");

void draw_rocket_target_pulse_quad(struct Moby *m) __asm__("FUN_L18_002d5768");

void update_rocket_moby(struct Moby *moby) __asm__("FUN_L18_002d5050");

void update_rocket_moby(struct Moby *moby) {
    L18D *moby_vars = (L18D *)moby->pvars;
    float target_movement_step[4];
    float movement_step[4];
    float rotation_angles[4];
    float previous_position[4];
    float target_heading_basis[12];
    float configured_rotation_basis[12];
    float aA0[16];
    float movement_blend_weight, pitch_angle_or_remaining_frames, k;
    int movement_collision_result;
    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        mark_moby_for_removal(moby);
    } else if (moby->state == 0) {
        qcopy(previous_position, &moby->pos);
        movement_blend_weight = convert_integer_to_float(moby_vars->c);
        movement_blend_weight = movement_blend_weight / convert_integer_to_float(moby_vars->lim);
        if (1.0f < movement_blend_weight) {
            movement_blend_weight = 1.0f;
        }
        fast_vec_sub(target_movement_step, (char *)moby_vars + 0x10, &moby->pos);
        fast_vec_scale(target_movement_step, target_movement_step, 1.0f / convert_integer_to_float(moby_vars->end - moby_vars->c));
        clear_u64_value(rotation_angles);
        rotation_angles[2] = fast_arc_tan(target_movement_step[0], target_movement_step[1]);
        euler_rotation_basis(target_heading_basis, rotation_angles);
        transpose_rotation_matrix(aA0, target_heading_basis);
        clear_u64_value(rotation_angles);
        rotation_angles[2] = moby_vars->f24;
        rotation_angles[1] = moby_vars->f28;
        euler_rotation_basis(configured_rotation_basis, rotation_angles);
        transform_vector_by_basis(target_movement_step, target_movement_step, aA0);
        transform_vector_by_basis(target_movement_step, target_movement_step, configured_rotation_basis);
        transform_vector_by_basis(target_movement_step, target_movement_step, target_heading_basis);
        lerp_vector(movement_step, moby_vars, target_movement_step, movement_blend_weight);
        spawn_rocket_trail_effects(moby, movement_step);
        fast_vec_add(&moby->pos, &moby->pos, movement_step);
        moby->rot.z = fast_arc_tan(movement_step[0], movement_step[1]);
        pitch_angle_or_remaining_frames = fast_arc_tan(vector_length_xy(movement_step), movement_step[2]);
        k = frame_time;
        moby->rot.y = -pitch_angle_or_remaining_frames;
        moby->rot.x = fast_add_rotations(moby->rot.x, k * 18.849556f);
        moby_vars->c = moby_vars->c + 1;
        movement_collision_result = collision_line(previous_position, &moby->pos, 0, moby, 0);
        if (movement_collision_result) {
            char *collision_filter_moby = D_L18_00174858;
            if (collision_filter_moby != 0) {
                short h = *(short *)(collision_filter_moby + 0xA6);
                if (h == 0x1DF) {
                    moby_vars->flags = moby_vars->flags & 4;
                } else if (h != 0x1A3) {
                    movement_collision_result = 0;
                }
            }
        }
        if (moby_vars->c == moby_vars->end || movement_collision_result) {
            spawn_rocket_explosion(moby, movement_step);
            mark_moby_for_removal(moby);
        } else if (moby_vars->flags & 2) {
            pitch_angle_or_remaining_frames = (float)(moby_vars->end - moby_vars->c);
            if (pitch_angle_or_remaining_frames <= FUN_L04_001f1978(D_L18_00161964)) {
                enqueue_callback_list_1(draw_rocket_target_pulse_quad, moby);
            }
        }
    }
}
/* Spawns a moby that travels along a path, with its heading set from the direction vector. */
extern float D_L18_00161960 __attribute__((sda));
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

char *create_rocket_moby(float *pos, float *initial_movement_step, float *target, float movement_rotation_z, float movement_rotation_y, int frames, int kind) __asm__("FUN_L18_002d5348");

char *create_rocket_moby(float *pos, float *initial_movement_step, float *target, float movement_rotation_z, float movement_rotation_y, int frames, int kind) {
    char *moby = create_moby(0x234);
    if (moby != 0) {
        char *data;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        set_moby_color(moby, 0xC0, 0xC0, 0xC0);
        *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * D_L18_00161960;
        qcopy(moby + 0x10, pos);
        data = *(char **)(moby + 0x78);
        qcopy(data, initial_movement_step);
        qcopy(data + 0x10, target);
        *(float *)(data + 0x24) = movement_rotation_z;
        *(float *)(data + 0x28) = movement_rotation_y;
        *(int *)(data + 0x2C) = 0;
        *(int *)(data + 0x34) = frames;
        *(int *)(data + 0x30) = frames >> 1;
        *(int *)(data + 0x38) = kind;
        *(float *)(moby + 0x48) = fast_arc_tan(initial_movement_step[0], initial_movement_step[1]);
        *(float *)(moby + 0x44) = -fast_arc_tan(vector_length_xy(initial_movement_step), initial_movement_step[2]);
        allocate_voice_for_target_entry(0, 0, (int)moby);
    }
    return moby;
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D6878), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0026cbb0(void *pos, void *dir, int c, int d, int n, int k, float f);
extern int scale_game_frames(int);
typedef unsigned int Q128 __attribute__((mode(TI), aligned(16)));
typedef union {
    Q128 q;
    float v[4];
} QU;
extern unsigned char *FUN_L00_002712b8(void *pos, float *vec, int s, int a, int col, int n, float x,
                                       float y, float z, float w, float pw);
extern void FUN_L00_00257d78(float *, float, float);
extern void add_vector_xyz(void *, void *, void *);
extern void scale_vector_xyz(void *, void *, float);
extern void transform_vector_by_basis(void *, void *, void *);

void spawn_rocket_trail_effects(void *mobyp, void *movement_step) __asm__("FUN_L18_002d5488");

void spawn_rocket_trail_effects(void *mobyp, void *movement_step) {
    char *moby = mobyp;
    float effect_vector[4];
    float effect_spawn_position[4];
    QU world_effect_direction;
    unsigned char *particle;
    FUN_L00_00257d78(effect_vector, frame_time * 0.1f, frame_time * 0.2f);
    scale_vector_xyz(effect_spawn_position, movement_step, random_float_between(0.0f, 1.0f));
    add_vector_xyz(effect_spawn_position, effect_spawn_position, moby + 0x10);
    particle = FUN_L00_002712b8(effect_spawn_position, effect_vector, scale_game_frames(0x14), 0x7F, 0x606060, 3, 40000.0f, 1000.0f,
                         1.0f, -0.0002f, 0.0f);
    if (particle != 0) {
        particle[2] = *(unsigned char *)D_L18_001B2DDC_c;
        particle[3] = 0x44;
    }
    world_effect_direction.q = 0;
    world_effect_direction.v[2] = 0.02f;
    transform_vector_by_basis(&world_effect_direction, &world_effect_direction, moby + 0xC0);
    FUN_L00_0026cbb0(effect_spawn_position, &world_effect_direction, 0x4F007FFF, 0x1FFFFFFF, scale_game_frames(0x14), 1, 20000.0f);
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D6A00), where it is exact; names translated to the US level program. */

typedef struct {
    float v[4];
} __attribute__((aligned(16))) QVec;

extern float D_L18_0016196C __attribute__((sda));

void spawn_rocket_explosion(void *moby_object, void *movement_step) {
    char *moby = moby_object;
    float movement_step_copy[4];
    float *qp = movement_step_copy;
    int flags;
    float flag_selected_effect_scalar, flag_selected_effect_argument, effect_scale, hit_radius, e;
    *(OvlQuad *)qp = *(OvlQuad *)movement_step;
    flags = *(int *)(*(char **)(moby + 0x78) + 0x38);
    effect_scale = (flags & 1) ? 2.0f : 0.75f;
    flag_selected_effect_scalar = (flags & 1) ? 4.0f : 2.0f;
    flag_selected_effect_argument = (flags & 1) ? 2.0f : 1.0f;
    hit_radius = (flags & 4) ? 0.0f : 1.5f;
    func_L00_0025F4A8_alt(moby, qp, moby + 0x10, hit_radius, 1.0f, 0x14, 6, 0x20, flag_selected_effect_scalar, flag_selected_effect_argument, 9.0f, effect_scale, 1, 5.0f, 0,
                          0, -1, 0);
    if (hit_radius != 0.0f) {
        if (hero.vehicle_mode == 2) {
            play_entity_sound((int)moby, 2.0f, moby + 0x10, 2, D_L18_0016196C, 1.0f, 0, 1,
                             0);
        }
    }
}
/* Draws the moby's pulsing glow quad: four corners around its position with a fading colour. */
typedef struct {
    float m[4][4];
    unsigned int col[4];
    float uv[4][2];
    unsigned long long a, b, c, d;
} Pk_d5768;
float sparkle_quad_corners[4][4] __asm__("D_L18_001D3630") = {
    {1.0f, 1.0f, 0.1f, 0.0f},
    {1.0f, -1.0f, 0.1f, 0.0f},
    {-1.0f, 1.0f, 0.1f, 0.0f},
    {-1.0f, -1.0f, 0.1f, 0.0f},
};
float sparkle_quad_uvs[8] __asm__("D_L18_001D3670") = {1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
extern float fast_normalize_angle(float) __asm__("FUN_001fa610");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern unsigned int fast_tween_color(float, int, int) __asm__("FUN_001fa6e0");
extern unsigned long long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

extern int D_L18_00161970 __attribute__((sda));
extern int D_L18_00161974 __attribute__((sda));
extern int D_L18_00161978 __attribute__((sda));
extern int D_L18_0016197C __attribute__((sda));
extern int D_L18_00161980 __attribute__((sda));
extern int D_L18_00161984 __attribute__((sda));
void draw_rocket_target_pulse_quad(struct Moby *m) {
    Pk_d5768 pk;
    char *d = (char *)m->pvars;
    unsigned int col;
    int i;
    col = fast_tween_color((fast_sin(fast_normalize_angle((*(int *)(d + 0x2C) & 0xF) * 0.0625f * 6.28318f)) + 1.0f) * 0.5f,
                          D_L18_00161980, D_L18_00161984);
    pk.b = get_effect_texture(0xD);
    pk.d = (unsigned long long)D_L18_00161970 | ((unsigned long long)D_L18_00161974 << 2) |
           ((unsigned long long)D_L18_00161978 << 4) | ((unsigned long long)D_L18_0016197C << 6) |
           0x8000000000ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 0;
    for (i = 0; i < 4; i++) {
        float *quad_vertex = pk.m[i];
        add_vector_xyz(quad_vertex, sparkle_quad_corners[i], d + 0x10);
        quad_vertex[3] = 1.0f;
        pk.col[i] = col;
        pk.uv[i][0] = sparkle_quad_uvs[i * 2];
        pk.uv[i][1] = sparkle_quad_uvs[i * 2 + 1];
    }
    draw_geometry_quad(&pk, 0, 0);
}
/* Moby update: hovers, drifts toward the player and plays its animations by state. */
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float FUN_001f9b48(void *, void *);
extern float fast_cos(float) __asm__("func_001F9DC8");
extern int D_001413D0[];
extern float D_0015EE70 __asm__("D_0015ED70");
extern int FUN_L00_0025fcb8(char *, char *, float);
extern int FUN_L00_002dc9f0(void *, void *);
extern int scale_game_frames_i(int) __asm__("FUN_001f96f8");
extern short D_L18_00161988 __attribute__((sda));
extern float D_L18_00161994 __attribute__((sda));
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_L00_00258830(int, int, float, float, float, int);
void update_class_238_detonation(unsigned char *moby) __asm__("FUN_L18_002d5f20");

void update_class_238_detonation(unsigned char *m) __asm__("FUN_L18_002d5f20");
extern void blend_moby_animation_alt(void *, int, int, int) __asm__("FUN_00212f90");
extern short D_L18_00161A6C __asm__("D_0015ED6C");
extern float D_L18_6C_m __asm__("D_0015ED6C");

void update_class_238_moby(struct Moby *moby) __asm__("FUN_L18_002d5918");

void update_class_238_moby(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_BALLISTIC_FLIGHT = 1,
        STATE_POST_FLIGHT_SETTLING = 2,
        STATE_APPROACH_NEARBY_TARGET = 3,
        STATE_WAIT_TO_RESUME_APPROACH = 4
    };
    char *data = (char *)moby->pvars;
    float v[4];
    update_class_238_detonation(moby);
    switch (moby->state) {
    case STATE_INITIALIZE:
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xEFFF;
        *(void **)(data + 0xD0) = &D_L18_00161988;
        *(float *)(data + 0x30) = 0.05f;
        moby->state = 5;
        break;
    case STATE_BALLISTIC_FLIGHT:
        add_vector_xyz(&moby->pos, &moby->pos, data + 0x180);
        *(float *)(data + 0x188) -= frame_time_sq * 10.0f;
        if (moby->pos.z < *(float *)(data + 0x178)) {
            moby->pos.z = *(float *)(data + 0x178);
            moby->state = 2;
            *(float *)(data + 0x188) = -(D_L18_00161994 * 2.0f * D_L18_6C_m);
        }
        break;
    case STATE_POST_FLIGHT_SETTLING: {
        float settling_motion_step = approach_value((float *)(data + 0x188), 0.0f, D_L18_00161994 * 2.0f * frame_time_sq);
        moby->rot.y += settling_motion_step;
        v[0] = fast_cos(moby->rot.z) * settling_motion_step;
        v[1] = fast_sin(moby->rot.z) * settling_motion_step;
        v[2] = 0.0f;
        add_vector_xyz(&moby->pos, &moby->pos, v);
        if (*(float *)(data + 0x188) == 0.0f) {
            if (moby->prev_seq != 1) {
                blend_moby_animation_alt(moby, 1, 0, scale_game_frames_i(12));
            }
            moby->state = 3;
            moby->hit_slot = 0xFF;
        }
        break;
    }
    case STATE_APPROACH_NEARBY_TARGET:
        if (moby->prev_seq == 1 && (moby->anim_flags & 2)) {
            blend_moby_animation_alt(moby, 2, 0, scale_game_frames_i(12));
        }
        if (moby->visible) {
            char target_search_output[0x50];
            float previous_position[4];
            int target_search_result = FUN_L00_0025fcb8((char *)moby, target_search_output, 6.0f);
            char *hit;

            char *movement_step;
            if (target_search_result == 2)
                return;
            hit = *(char **)(target_search_output + 0x40);
            if (hit == 0)
                return;
            if (vector_distance(&moby->pos, hit + 0x10) < 6.0f) {
                float movement_step_limit = *(float *)&D_L18_00161A6C * 3.0f;
                float horizontal_steering_increment_limit = frame_time_sq * 6.0f;
                qcopy(previous_position, &moby->pos);
                subtract_vector_xyz(v, *(char **)(target_search_output + 0x40) + 0x10, &moby->pos);
                v[2] = -(D_0015EE70 * 10.0f);
                if (horizontal_steering_increment_limit < vector_length_xy(v)) {
                    set_vector_length_xy(v, v, horizontal_steering_increment_limit);
                }
                movement_step = data + 0x40;
                add_vector_xyz(movement_step, movement_step, v);
                if (movement_step_limit < vector_length_xy(movement_step)) {
                    set_vector_length_xy(movement_step, movement_step, movement_step_limit);
                }
                FUN_L00_00258830((int)moby, (int)movement_step, 0.0f, 0.5f, 0.0f, 0);
                subtract_vector_xyz(movement_step, &moby->pos, previous_position);
                if (0.0f < *(float *)(data + 0x48)) {
                    *(float *)(data + 0x48) = 0.0f;
                } else if (*(float *)(data + 0x48) < -movement_step_limit) {
                    *(float *)(data + 0x48) = -movement_step_limit;
                }
                if (*(char **)(target_search_output + 0x40) != (char *)D_001413D0[0]) {
                    if (FUN_001f9b48(*(char **)(target_search_output + 0x40) + 0x10, &moby->pos) < 2.0f) {
                        *(int *)(data + 0x19C) = 1;
                    }
                }
            }
        }
        break;
    case STATE_WAIT_TO_RESUME_APPROACH:
        if (FUN_L00_002dc9f0(moby, data + 0x110)) {
            moby->state = 3;
            *(short *)(data + 0xC8) = 0;
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D70E8), where it is exact; names translated to the US level program. */

typedef struct {
    char _pad00[0x10];
    unsigned char nframes; /* 0x10 */
} AnimSeq_q;

typedef struct {
    char _pad00[0x48];
    AnimSeq_q *seqs[1]; /* 0x48 */
} AnimClass_q;

typedef struct {
    char _pad00[0x24];
    AnimClass_q *pClass; /* 0x24 */
    char _pad28[0x50 - 0x28];
    unsigned char frame;     /* 0x50 */
    unsigned char nextFrame; /* 0x51 */
    unsigned char seq;       /* 0x52 */
    unsigned char prevSeq;   /* 0x53 */
    char _pad54[0x5C - 0x54];
    float unk5C; /* 0x5C */
    char _pad60[0x68 - 0x60];
    float *frameData; /* 0x68 */
    char _pad6C[4];
    unsigned char unk70; /* 0x70 */
} MobyAnim_q;

extern float D_0015EE70 __asm__("D_0015ED70");
extern float angle_atan2(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025abf0_q(float *a, float *b, float *out, float speed,
                                float g) __asm__("FUN_L00_0025abf0");
extern int *D_L18_001AC240_q[] __asm__("D_L18_001AC240") __attribute__((section(".data")));
extern int D_L18_0015FFD8_m __asm__("D_L18_0015FFD8");
extern f32 D_L18_00161994_q __asm__("D_L18_00161994") __attribute__((sda));
extern void trooper_scale(void *, void *, float) __asm__("FUN_001f9c48");
void blend_moby_animation_q(MobyAnim_q *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

char *launch_class_238_moby_from_list(float horizontal_speed, int a0, int moby_list_index, float *launch_position, float *target_position, int detonation_countdown) __asm__("FUN_L18_002d5cf8");

char *launch_class_238_moby_from_list(float horizontal_speed, int a0, int moby_list_index, float *launch_position, float *target_position, int detonation_countdown) {
    float adjusted_target_position[4];
    float r;
    float g;
    char *found = 0;
    short *moby_list_entry = (short *)D_L18_001AC240_q[moby_list_index];
    char *moby;
    char *data;
    char *d2;
    if (moby_list_entry == 0) {
        return 0;
    }
    {
        char *base = D_L18_0015FFD8_m;
        unsigned short encoded_moby_entry;
        do {
            encoded_moby_entry = *moby_list_entry;
            {
                char *candidate_moby = base + ((encoded_moby_entry & 0x7FFF) << 8);
                if (*(short *)(candidate_moby + 0xA6) == 0x238) {
                    if (((unsigned char *)candidate_moby)[0x20] == 5) {
                        found = candidate_moby;
                    }
                }
            }
            moby_list_entry++;
        } while ((short)encoded_moby_entry >= 0);
    }
    moby = found;
    if (moby != 0) {
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        *(unsigned short *)(moby + 0x34) = (*(unsigned short *)(moby + 0x34) & 0xFFBE) | 0x1000;
        moby[0xBC] = 0;
        *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        if (((unsigned char *)moby)[0x53] != 0) {
            blend_moby_animation_q(moby, 0, 0, 1);
        }
        qcopy(moby + 0x10, launch_position);
        r = *(float *)&D_L18_00161994_q;
        *(int *)(moby + 0x40) = 0;
        *(float *)(moby + 0x44) = -r;
        target_position[2] = target_position[2] + 0.75f;
        subtract_vector_xyz(adjusted_target_position, target_position, launch_position);
        trooper_scale(adjusted_target_position, adjusted_target_position, vector_length_xy(adjusted_target_position) - *(float *)&D_L18_00161994_q);
        add_vector_xyz(adjusted_target_position, adjusted_target_position, launch_position);
        data = *(char **)(moby + 0x78);
        qcopy(data + 0x170, adjusted_target_position);
        subtract_vector_xyz(data + 0x180, adjusted_target_position, launch_position);
        *(int *)(data + 0x188) = 0;
        normalize_vector_xyz(data + 0x180, data + 0x180, horizontal_speed);
        g = D_0015EE70 * 10.0f;
        *(short *)(data + 0xC8) = 0;
        *(float *)(data + 0x188) = FUN_L00_0025abf0_q(launch_position, adjusted_target_position, 0, horizontal_speed, -g);
        *(float *)(moby + 0x48) =
            angle_atan2(*(float *)(data + 0x180), *(float *)(data + 0x184));
        *(int *)(data + 0x194) = detonation_countdown;
        *(int *)(data + 0x198) = a0;
        *(int *)(data + 0x19C) = 0;
        allocate_voice_for_target_entry(0, 0, (int)moby);
    }
    return moby;
}
/* Blows the moby up when it is hit, shot by the player or close to the target, then hides it. */
extern char D_L18_0015F580_h[8] __asm__("D_L18_0015F580");
extern int trooper_find_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void explode_h(void *, void *, void *, float, float, int, int, int, float, float, float, float, int,
                      float, int, int, int, int) __asm__("FUN_L00_0025e450");

void update_class_238_detonation(unsigned char *moby) {
    char *moby_vars = *(char **)(moby + 0x78);
    struct Hero *hero_ptr;
    int hit;
    if (moby[0x20] == 5 || moby[0x20] == 0 || moby[0x20] == 4)
        return;
    hero_ptr = &hero;
    if (hero_ptr->state.current == 0x72) {
        moby[0x20] = 5;
        *(short *)(moby + 0x34) = (*(short *)(moby + 0x34) | 0x41) & ~0x1000;
        *(int *)(moby + 0x94) = 0;
        return;
    }
    hit = trooper_find_hit(moby, 0x330000, 0);
    moby[0xA4] = 0xFF;
    if (((unsigned char *)hero_ptr->coll_hit_moby) == moby || ((unsigned char *)hero_ptr->unk240) == moby || *(int *)(moby_vars + 0x19C) != 0) {
        explode_h(moby, D_L18_0015F580_h, 0, 1.5f, 1.0f, 0x14, 6, 0x20, 3.0f, 1.5f, 9.0f, 1.5f, 1, 0.0f, 1, 0, -1, 0);
        moby[0x20] = 5;
        *(short *)(moby + 0x34) = (*(short *)(moby + 0x34) | 0x41) & ~0x1000;
        *(int *)(moby + 0x94) = 0;
        return;
    }
    if (fast_dec_timer((int *)(moby_vars + 0x194)) == 0 && hit == 0) {
        if (moby[0x20] != 1)
            return;
        if (!(vector_distance(moby + 0x10, &hero_ptr->motion.pos) < 2.0f))
            return;
    }
    explode_h(moby, D_L18_0015F580_h, 0, 0.0f, 0.0f, 5, 2, 8, 1.0f, 0.5f, 9.0f, 0.5f, 1, 0.0f, 0, 0, -1, 0);
    moby[0x20] = 5;
    *(short *)(moby + 0x34) = (*(short *)(moby + 0x34) | 0x41) & ~0x1000;
    *(int *)(moby + 0x94) = 0;
}
/* Pauses the moby in state 4 while the check holds, restoring its old state after. */

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

int update_moby_state_pause(struct Moby *moby) __asm__("FUN_L18_002d6108");

int update_moby_state_pause(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int state_pause_result = process_moby_suck_cannon_suction(moby);
    if (state_pause_result != 0) {
        unsigned char current_state = moby->state;
        if ((unsigned)(current_state - 3) < 2) {
            state_pause_result = 2;
            if (current_state != 4) {
                moby->command = current_state;
                moby->state = 4;
            }
        } else {
            *(short *)(moby_vars + 0xC8) = 0;
            state_pause_result = 0;
        }
    } else {
        long t = moby->state;
        if (t == 4) {
            moby->state = moby->command;
        }
    }
    return state_pause_result;
}
/* A path and the points along it (the count, then the points from 0x10) */
typedef struct {
    s32 count;
    u8 pad4[0xC];
    Vec4f points[1];
} PathPoints;

/* One entry of the D_L18_0015F70C path list (0x20 bytes) */
typedef struct {
    u8 pad0[0xC];
    f32 weight;       /* 0xC: 0 disables the path */
    PathPoints *path; /* 0x10 */
    u8 pad14[0xC];
} PathLink;

/* pvars of FUN_L18_002d62e8: up to 4 pairs of paths that meet, the hero gets the nearer one */
typedef struct {
    struct {
        PathLink *first;
        PathLink *second;
    } pair[4];             /* 0x00: second starts where first's last point is */
    struct {
        f32 first;
        f32 second;
    } weight[4];           /* 0x20: their weights before they were cleared */
    s32 node[2];           /* 0x40: indexes into D_L18_001600EC */
} PathJunctionVars;

extern PathLink *D_L18_0015F70C;
extern s32 D_L18_0015F710;
extern char *D_L18_001600EC;
extern float FUN_001f9b48(void *, void *);
extern char *create_class_326_moby(void *vector) __asm__("FUN_L18_002e22c0");

void update_path_junction_selection(struct Moby *moby) __asm__("FUN_L18_002d62e8");

void update_path_junction_selection(struct Moby *moby) {
    enum {
        STATE_INITIALIZE_PATH_PAIRS = 0,
        STATE_SELECT_NEARER_PATH = 1
    };
    PathJunctionVars *vars = (PathJunctionVars *)moby->pvars;
    s32 pair_or_node_index;
    s32 i;
    s32 j;
    s32 k;
    f32 near_first;

    switch (moby->state) {
    case STATE_INITIALIZE_PATH_PAIRS:
        moby->state = 1;
        moby->update_dist = 0xFF;
        pair_or_node_index = 0;
        do {
            for (i = 0; i < D_L18_0015F710; i++) {
                if (D_L18_0015F70C[i].weight == 0.0f) {
                    continue;
                }
                vars->pair[pair_or_node_index].first = &D_L18_0015F70C[i];
                vars->weight[pair_or_node_index].first = D_L18_0015F70C[i].weight;
                for (j = 0; j < D_L18_0015F710; j++) {
                    if (D_L18_0015F70C[j].weight == 0.0f) {
                        continue;
                    }
                    if (FUN_001f9b48(&D_L18_0015F70C[i].path->points[0],
                                     &D_L18_0015F70C[j].path->points[D_L18_0015F70C[j].path->count - 1]) < 0.5f) {
                        vars->pair[pair_or_node_index].second = &D_L18_0015F70C[j];
                        vars->weight[pair_or_node_index].second = D_L18_0015F70C[j].weight;
                        D_L18_0015F70C[j].weight = 0.0f;
                        D_L18_0015F70C[i].weight = 0.0f;
                        pair_or_node_index++;
                        break;
                    }
                }
            }
        } while (pair_or_node_index < 4);
        for (pair_or_node_index = 0; pair_or_node_index < 2; pair_or_node_index++) {
            create_class_326_moby(D_L18_001600EC + vars->node[pair_or_node_index] * 0x80 + 0x30);
        }
        break;
    case STATE_SELECT_NEARER_PATH:
        hero.unk1CA = 5;
        for (k = 0; k < 4; k++) {
            if (hero.state.control_mode == 0xF &&
                (hero.unk560 == (s32)vars->pair[k].first->path || hero.unk560 == (s32)vars->pair[k].second->path)) {
                continue;
            }
            near_first = vector_distance(&hero.motion.pos, &vars->pair[k].first->path->points[0]);
            if (near_first < vector_distance(&hero.motion.pos, &vars->pair[k].second->path->points[0])) {
                vars->pair[k].first->weight = vars->weight[k].first;
                vars->pair[k].second->weight = 0.0f;
            } else {
                vars->pair[k].first->weight = 0.0f;
                vars->pair[k].second->weight = vars->weight[k].second;
            }
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D79F0), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    int i10;
    char pad1[0x10];
    float f24;
} L18Cls_q;

typedef struct {
    char pad0[0x10];
    float f10;
    char *p14;
    char *p18;
    char *p1C;
    float f20;
    float f24;
    float f28;
    float f2C;
    int i30;
    float f34;
    int i38;
} L18E_q;

typedef struct {
    char v[16];
    char padv[16];
    float f20;
    char pad24[4];
    float f28;
    char pad2C[4];
    int f30;
    int f34;
} Child_q;

typedef struct {
    char pad0[0x10];
    char pos[0x10];
    unsigned char b20;
    char pad21[0xB];
    float f2C;
    unsigned char b30;
    unsigned char b31;
    short s32;
    unsigned short h34;
    char pad36[0x12];
    float f48;
    char pad4C[0x2C];
    Child_q *child;
    char pad7C[0x14];
    int f90;
} Obj_q;

extern char *D_L18_00174858_q __asm__("D_L18_00174858") __attribute__((section(".data")));
extern char *func_0020D348_m_q(int) __asm__("FUN_0020c4f8");
extern short D_L18_00161A6C __asm__("D_0015ED6C");
extern float D_0015EE70 __asm__("D_0015ED70");
extern float FUN_001fa580_q(float, float) __asm__("FUN_001fa580");
extern float advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern int D_L18_0015F5C4_d __asm__("D_L18_0015F5C4");
extern int D_L18_0016D290_lui __asm__("D_L18_0016D290") __attribute__((section(".data")));
extern void FUN_001f9bf8_q(void *, void *, float) __asm__("FUN_001f9bf8");
extern void upd_a(void *) __asm__("FUN_L00_00250df8");
f32 probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
void mmr_q(void *obj) __asm__("FUN_0020c828");

extern float D_L18_00161998 __attribute__((sda));
extern float D_L18_0016199C __attribute__((sda));
extern float D_L18_001619A0 __attribute__((sda));
void thruster_pack_lock_green_update(char *moby) __asm__("FUN_L18_002d6600");
void thruster_pack_lock_green_update(char *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_UPDATE_LOCK_COMPONENTS = 1
    };
    L18E_q *moby_vars = *(L18E_q **)(moby + 0x78);
    *(float *)(moby + 0x2C) = (*(L18Cls_q **)(moby + 0x24))->f24 * D_L18_00161998;
    switch (((unsigned char *)moby)[0x20]) {
    case STATE_INITIALIZE: {
        *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + 1.0f;
        *(int *)(moby + 0x94) = 0;
        *(float *)(moby + 0x18) = probe_ground_height(moby + 0x10, 0, 0.5f);
        qcopy(moby_vars, moby + 0x10);
        if (D_L18_00174858_q != 0) {
            moby_vars->i38 = 1;
        }
        {
            char *spawned_moby = func_0020D348_m_q(0x6EB);
            spawned_moby[0x30] = 0x40;
            *(short *)(spawned_moby + 0x32) = 0x56;
            spawned_moby[0x31] = 1;
            *(short *)(spawned_moby + 0x32) = 0x56;
            *(float *)(spawned_moby + 0x2C) = (*(L18Cls_q **)(spawned_moby + 0x24))->f24 * D_L18_00161998;
            qcopy(spawned_moby + 0x10, moby_vars);
            qcopy(spawned_moby + 0x40, moby + 0x40);
            upd_a(spawned_moby);
            moby_vars->p1C = spawned_moby;
        }
        moby_vars->p14 = func_0020D348_m_q(0x6F0);
        moby_vars->p14[0x30] = 0x40;
        *(short *)(moby_vars->p14 + 0x32) = 0x56;
        moby_vars->p14[0x31] = 1;
        *(float *)(moby_vars->p14 + 0x2C) =
            (*(L18Cls_q **)(moby_vars->p14 + 0x24))->f24 * D_L18_00161998;
        {
            char *q = moby_vars->p14;
            qcopy(q + 0x40, moby + 0x40);
            upd_a(q);
        }
        moby_vars->p18 = func_0020D348_m_q(0x6F0);
        moby_vars->p18[0x30] = 0x40;
        *(short *)(moby_vars->p18 + 0x32) = 0x40;
        moby_vars->p18[0x31] = 1;
        *(float *)(moby_vars->p18 + 0x2C) =
            (*(L18Cls_q **)(moby_vars->p18 + 0x24))->f24 * D_L18_00161998;
        {
            char *q = moby_vars->p18;
            qcopy(q + 0x40, moby + 0x40);
            *(float *)(moby_vars->p18 + 0x48) = FUN_001fa580_q(*(float *)(q + 0x48), 3.1415901f);
            upd_a(moby_vars->p18);
        }
        ((unsigned char *)moby)[0x20] = 1;
        {
            float base_height = *(float *)(moby + 0x18);
            float u = D_L18_001619A0;
            moby_vars->f10 = base_height;
            moby_vars->f34 = u;
        }
        *(int *)(moby + 0x94) = (*(L18Cls_q **)(moby + 0x24))->i10;
        break;
    }
    case STATE_UPDATE_LOCK_COMPONENTS: {
        float v[4];
        if (D_L18_0015F5C4_d == 2) {
            if (moby_vars->i38 != 0) {
                if (D_L18_0016D290_lui == 3) {
                    if (moby_vars->p14 != 0)
                        mmr_q(moby_vars->p14);
                    if (moby_vars->p18 != 0)
                        mmr_q(moby_vars->p18);
                    if (moby_vars->p1C != 0)
                        mmr_q(moby_vars->p1C);
                    mmr_q(moby);
                }
            }
        }
        if (moby_vars->f24 != 0.0f) {
            if ((moby_vars->i30 == 0 && fast_dec_timer((int *)&moby_vars->f2C) == 2) ||
                (D_L18_0015F5C4_d == 2 && D_L18_0016D290_lui >= 4)) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
                moby_vars->f24 = 0;
            }
            {
                struct Hero *hero_ptr = &hero;
                if (hero_ptr->air_frames.s == 0 && hero_ptr->state.current == 0x22) {
                    allocate_voice_for_target_entry(0, 0, (int)moby);
                    moby_vars->f24 = 0;
                }
            }
        }
        if (moby_vars->f24 != 0.0f) {
            if (moby_vars->f20 != moby_vars->f24) {
                advance_accelerated_scalar(&moby_vars->f20, &moby_vars->f28, moby_vars->f24, D_0015EE70 * 4.0f, D_0015EE70 * 4.0f,
                               (*(float *)&D_L18_00161A6C) * 4.0f);
            } else {
                float k = D_0015EE70;
                advance_accelerated_scalar(&moby_vars->f34, &moby_vars->f28, D_L18_0016199C, k * 16.0f, k * 4.0f,
                               (*(float *)&D_L18_00161A6C) * 8.0f);
            }
        } else if (moby_vars->f34 != D_L18_001619A0) {
            float k = D_0015EE70;
            advance_accelerated_scalar(&moby_vars->f34, &moby_vars->f28, D_L18_001619A0, k * 16.0f, k * 4.0f,
                           (*(float *)&D_L18_00161A6C) * 8.0f);
        } else {
            advance_accelerated_scalar(&moby_vars->f20, &moby_vars->f28, moby_vars->f24, D_0015EE70 * 4.0f, D_0015EE70 * 4.0f,
                           (*(float *)&D_L18_00161A6C) * 4.0f);
        }
        if (moby_vars->p14 != 0) {
            FUN_001f9bf8_q(moby_vars->p14 + 0x10, moby_vars->p14 + 0xD0, -moby_vars->f20);
            FUN_001f9bf8_q(v, moby_vars->p14 + 0xE0, -moby_vars->f20 * 0.2f);
            add_vector_xyz(moby_vars->p14 + 0x10, moby_vars->p14 + 0x10, v);
            add_vector_xyz(moby_vars->p14 + 0x10, moby_vars->p14 + 0x10, moby_vars);
            upd_a(moby_vars->p14);
        }
        if (moby_vars->p18 != 0) {
            FUN_001f9bf8_q(moby_vars->p18 + 0x10, moby_vars->p18 + 0xD0, -moby_vars->f20);
            FUN_001f9bf8_q(v, moby_vars->p18 + 0xE0, -moby_vars->f20 * 0.2f);
            add_vector_xyz(moby_vars->p18 + 0x10, moby_vars->p18 + 0x10, v);
            add_vector_xyz(moby_vars->p18 + 0x10, moby_vars->p18 + 0x10, moby_vars);
            upd_a(moby_vars->p18);
        }
        *(float *)(moby + 0x18) = moby_vars->f10 + moby_vars->f34;
    }
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D7F48), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x58];
    char *base;
    char pad2[0x100];
} L18Glob;

typedef struct {
    char pad0[0x10];
    float x;
    float y;
} L18Pos;

extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int *D_L18_001AC240[];

void *find_class_247_moby_with_largest_xy_angle_in_list(float *reference_position, float *comparison_position, int moby_list_index) __asm__("FUN_L18_002d6b58");

void *find_class_247_moby_with_largest_xy_angle_in_list(float *reference_position, float *comparison_position, int moby_list_index) {
    short *moby_list_entry = (short *)D_L18_001AC240[moby_list_index];
    char *best = 0;
    float bestd = 0.0f;
    if (moby_list_entry == 0)
        return 0;
    while (1) {
        int moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
        if (*(short *)(moby_byte_offset + (int)*(char **)&D_L18_0015FFD8_d + 0xA6) == 0x247) {
            float comparison_to_reference_angle = fast_arc_tan(reference_position[0] - comparison_position[0], reference_position[1] - comparison_position[1]);
            L18Pos *candidate_moby = (L18Pos *)(moby_byte_offset + (int)*(char **)&D_L18_0015FFD8_d);
            float d =
                fast_difference_between_rotations(comparison_to_reference_angle, fast_arc_tan(reference_position[0] - candidate_moby->x, reference_position[1] - candidate_moby->y));
            if (bestd < d) {
                bestd = d;
                best = (char *)D_L18_0015FFD8_m + moby_byte_offset;
            }
        }
        if (*moby_list_entry++ < 0)
            return best;
    }
}
/* Of the mobys of class 0x247 in list idx, the one nearest to pos (within
   255 units), or 0. The second spelling of the table base keeps the
   address out of the loop's $gp-relative reload in the bc1f delay slot. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D8070), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);
extern int *D_L18_001AC240_c[] __asm__("D_L18_001AC240");
extern int D_L18_0015FFD8; /* no foreign declaration */
extern int D_L18_0015FFD8; /* no foreign declaration */

char *find_nearest_class_247_moby_in_list(void *reference_position, int moby_list_index) __asm__("FUN_L18_002d6c80");

char *find_nearest_class_247_moby_in_list(void *reference_position, int moby_list_index) {
    short *moby_list_entry = (short *)D_L18_001AC240_c[moby_list_index];
    char *best = 0;
    float bestd = 255.0f;
    if (moby_list_entry == 0)
        return 0;
    while (1) {
        int moby_byte_offset = (*moby_list_entry & 0x7FFF) << 8;
        char *candidate_moby = (char *)(moby_byte_offset + (int)D_L18_0015FFD8);
        if (*(short *)(candidate_moby + 0xA6) == 0x247) {
            float candidate_distance = FUN_001f9b48(reference_position, candidate_moby + 0x10);
            if (candidate_distance < bestd) {
                bestd = candidate_distance;
                best = D_L18_0015FFD8 + moby_byte_offset;
            }
        }
        if (*moby_list_entry++ < 0)
            return best;
    }
}
extern float D_L18_00161A10 __asm__("D_L18_00161A10") __attribute__((sda));
extern float D_L18_00161A14 __asm__("D_L18_00161A14") __attribute__((sda));
extern float D_L18_00161A18 __asm__("D_L18_00161A18") __attribute__((sda));
extern unsigned int D_L18_00162390 __asm__("D_L18_00162390") __attribute__((sda));
extern unsigned int D_0015EEA4 __asm__("D_0015EEA4") __attribute__((sda));
extern unsigned char menu_item_enabled_115 __asm__("D_0013D407") __attribute__((section(".data")));
/* Help message records (same layout as the usage counters): count, best play time, level mask. */
extern struct UsageStat D_00141968_7b20[] __asm__("D_00141968");
extern void draw_pulsing_timer_hud(void) __asm__("FUN_L18_002d8098");
extern void enqueue_callback_list_1_l18(void (*)(void), void *) __asm__("FUN_L18_002020c8");
extern char D_0013E550[];
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern int trigger_help_message(int, int) __asm__("FUN_L00_00203908");

extern int allocate_voice_int_7b20(int, int, int) __asm__("FUN_0022da68");

void thruster_pack_lock_red_update(struct Moby *moby) __asm__("FUN_L18_002d7b20");
void thruster_pack_lock_red_update(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_APPROACH_CONFIGURED_HEIGHT = 1,
        STATE_WAIT_FOR_GROUNDED_HERO_TRIGGER = 2,
        STATE_POST_WAIT_APPROACH_CONFIGURED_HEIGHT = 3,
        STATE_WAIT_FOR_DEPRESSION_TRIGGER = 4,
        STATE_SETTLE_DEPRESSED_HEIGHT = 5
    };
    char *data = (char *)moby->pvars;
    int index;

    switch (moby->state) {
    case STATE_INITIALIZE:
        *(float *)(data + 0x64) = moby->pos.z;
        *(unsigned short *)(data + 0x3E) |= 8;
        if (*(int *)(data + 0x60) != 0) {
            moby->state = 1;
            moby->scale = *(float *)((char *)moby->pclass + 0x24) * D_L18_00161A10;
            return;
        }
        moby->state = 4;
        return;
    case STATE_APPROACH_CONFIGURED_HEIGHT:
    case STATE_POST_WAIT_APPROACH_CONFIGURED_HEIGHT:
        advance_accelerated_scalar(&moby->pos.z, (float *)(data + 0x68),
                                   *(float *)(data + 0x64) + D_L18_00161A18,
                                   frame_time_sq * 20.0f, frame_time_sq * 40.0f,
                                   frame_time * 20.0f);
        break;
    case STATE_WAIT_FOR_GROUNDED_HERO_TRIGGER:
        advance_accelerated_scalar(&moby->pos.z, (float *)(data + 0x68),
                                   *(float *)(data + 0x64) + D_L18_00161A14,
                                   frame_time_sq * 5.0f, frame_time_sq * 10.0f,
                                   frame_time * 20.0f);
        if (hero.air_frames.s == 0 && hero.ground_moby == moby && hero.state.current == 0x22) {
            moby->state = 3;
            allocate_voice_int_7b20(0, 0, (int)moby);
            index = *(int *)(data + 0x70);
            if (index != -1) {
                unsigned char *entry = (unsigned char *)D_0013E550 + index * 0x70;
                if (*(struct Moby **)(entry + 0x88) == moby && entry[0x74] != 0)
                    release_voice_slot(index);
            }
            *(int *)(data + 0x70) = -1;
        } else if (hero.state.current != 0x72) {
            enqueue_callback_list_1_l18(draw_pulsing_timer_hud, moby);
            if (tick_countdown_32((int *)(data + 0x6C)) != 0)
                D_L18_00162390 = 1;
        }
        break;
    case STATE_WAIT_FOR_DEPRESSION_TRIGGER:
        if (hero.air_frames.s == 0 && hero.ground_moby == moby && hero.state.current == 0x22) {
            moby->state = 5;
            moby->command = 1;
            moby->pos.z -= 1.5f;
            if (current_level_index == 0x12)
                menu_item_enabled_115 = 1;
        }
        if (vector_distance(&moby->pos, &hero.motion.pos) < 2.5f) {
            struct UsageStat *help = D_00141968_7b20;
            int one = 1;
            if ((help[0x2B02].level_mask & (one << current_level_index)) == 0) {
                if (help[0x71].count != 0) {
                    if (scale_ticks(D_0015EEA4) - help[0x71].unk2 * 600 > (int)((float)scale_ticks(0x12) * 60.0f) ||
                        help[0x71].unk2 * 600 == 0) {
                        trigger_help_message(0x2B02, 0x71);
                    } else if (scale_ticks(D_0015EEA4) / 600 > help[0x71].unk2) {
                        help[0x71].unk2 = scale_ticks(D_0015EEA4) / 600;
                    }
                } else {
                    help[0x71].count++;
                    if (scale_ticks(D_0015EEA4) / 600 > help[0x71].unk2) {
                        help[0x71].unk2 = scale_ticks(D_0015EEA4) / 600;
                    }
                    help[0x71].level_mask = help[0x71].level_mask | (one << current_level_index) | 0x80000000;
                }
            }
        }
        break;
    case STATE_SETTLE_DEPRESSED_HEIGHT:
        if (approach_value((float *)&moby->pos.z, *(float *)(data + 0x64) - 1.5f,
                           0.15f) != 0.0f)
            return;
        moby->state = 6;
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D93C0), where it is exact; names translated to the US level program. */

extern char D_0013E550[];
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

void end_thruster_pack_lock_red_wait(struct Moby *moby) __asm__("FUN_L18_002d7fd0");

void end_thruster_pack_lock_red_wait(struct Moby *moby) {
    int *data = (int *)moby->pvars;
    if (moby->state == 2) {
        int voice_slot_index;
        moby->state = 3;
        voice_slot_index = data[0x70 / 4];
        if (voice_slot_index != -1) {
            unsigned char *voice_slot_entry = (unsigned char *)D_0013E550 + voice_slot_index * 0x70;
            if (*(unsigned char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                release_voice_slot(voice_slot_index);
            }
        }
        data[0x70 / 4] = -1;
    }
}
/* Checks the moby's phase: 1 (and rearms it) when done, -1 when waiting with no timer, else 0. */
int poll_moby_state_and_rearm(struct Moby *moby) __asm__("FUN_L18_002d8050");

int poll_moby_state_and_rearm(struct Moby *moby) {
    int *data = (int *)moby->pvars;
    if (moby->state == 3) {
        moby->state = 1;
        return 1;
    }
    if (moby->state == 2 && data[0x6C / 4] == 0) {
        return -1;
    }
    return 0;
}
extern float D_L18_0015F638 __attribute__((sda));
extern float D_L18_00161A40 __attribute__((sda));
extern float D_L18_00161A44 __attribute__((sda));
extern float D_L18_00161A50 __attribute__((sda));
extern float D_L18_00161A58 __attribute__((sda));
extern float D_L18_00161A5C __attribute__((sda));
extern float D_L18_00161A60 __attribute__((sda));
extern float D_L18_00161A64 __attribute__((sda));
extern int D_L18_001623A8 __attribute__((sda));
void spawn_floating_platform_sparks(struct Moby *moby) __asm__("FUN_L18_002d88b8");

extern void spawn_floating_platform_sparks(struct Moby *) __asm__("FUN_L18_002d88b8");
extern void FUN_L18_00227dd0(int, int);
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int allocate_voice_82c0(int, int, struct Moby *) __asm__("FUN_0022da68");

/* Ported from rac1-decomp nonmatching/l18_veldin2/func_L18_002D96B0.c (func_L18_002D96B0) */
/* Sinking platform update: arms its tilt, then sinks it under the hero until it is removed. */
void floating_platform_update(struct Moby *moby) __asm__("FUN_L18_002d82c0");
void floating_platform_update(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_WAIT_FOR_TILT_REQUEST = 1,
        STATE_WAIT_FOR_SINK_REQUEST = 2,
        STATE_TILT_DELAY = 3,
        STATE_WAIT_FOR_SINK_TRIGGER = 4,
        STATE_SINKING = 5
    };
    char *data;
    float delta[4] __attribute__((aligned(16)));
    float old_rot[4] __attribute__((aligned(16)));
    float tilt_rate_scale;
    float rate;
    float sink;
    float angle;

    data = (char *)moby->pvars;
    scale_vector_xyz(delta, &moby->pos, -1.0f);
    qcopy(old_rot, &moby->rot);
    switch (moby->state) {
    case STATE_INITIALIZE:
        *(float *)(data + 0x60) = moby->pos.z;
        *(float *)(data + 0x64) = moby->pos.z;
        *(int *)(data + 0x70) = 0;
        *(int *)(data + 0x68) = 0;
        *(int *)(data + 0x6C) = 0;
        angle = random_angle_radians();
        *(int *)(data + 0x5C) |= 1;
        *(float *)(data + 0x74) = angle;
        if (*(int *)(data + 0x88) != 0) {
            moby->state = 2;
        } else {
            moby->state = 1;
        }
        break;
    case STATE_TILT_DELAY:
        if (tick_countdown_32((int *)(data + 0x7C)) == 0) {
            break;
        }
        tilt_rate_scale = (convert_integer_to_float(D_L18_001623A8) - D_L18_00161A60) / (D_L18_00161A64 - D_L18_00161A60);
        if (1.0f < tilt_rate_scale) {
            tilt_rate_scale = 1.0f;
        } else if (tilt_rate_scale < 0.0f) {
            tilt_rate_scale = 0.0f;
        }
        tilt_rate_scale = 1.0f - tilt_rate_scale * 0.333f;
        if (*(int *)(data + 0x84) != 0) {
            rate = D_L18_00161A50 * tilt_rate_scale;
        } else {
            rate = D_L18_00161A58 * tilt_rate_scale;
        }
        *(float *)(data + 0x6C) = rate * DEG_TO_RAD * frame_time;
        moby->state = 4;
        break;
    case STATE_WAIT_FOR_SINK_TRIGGER:
        if (moby->visible != 0 && tick_countdown_32((int *)(data + 0x8C)) != 0) {
            spawn_floating_platform_sparks(moby);
            *(int *)(data + 0x8C) = scale_ticks(random_int_between(0x5A, 0x78));
        }
        if (hero.ground_moby == moby && hero.air_frames.s == 0) {
            *(int *)(data + 0x8C) = 0;
            moby->state = 5;
            *(short *)(data + 0x92) = allocate_voice_82c0(1, 0, moby);
        }
        if (*(int *)(data + 0x80) == 0) {
            break;
        }
        if (vector_distance(&moby->pos, &hero.motion.pos) < D_L18_00161A5C) {
            moby->state = 5;
            *(short *)(data + 0x92) = allocate_voice_82c0(1, 0, moby);
        }
        break;
    case STATE_SINKING:
        if (moby->visible != 0 && tick_countdown_32((int *)(data + 0x8C)) != 0) {
            spawn_floating_platform_sparks(moby);
            *(int *)(data + 0x8C) = scale_ticks(random_int_between(0xF, 0x1E));
        }
        sink = (convert_integer_to_float(D_L18_001623A8) - D_L18_00161A60) / (D_L18_00161A64 - D_L18_00161A60);
        if (1.0f < sink) {
            sink = 1.0f;
        } else if (sink < 0.0f) {
            sink = 0.0f;
        }
        sink = 1.0f - sink * 0.333f;
        if (*(int *)(data + 0x80) != 0 || (*(int *)(data + 0x88) != 0 && *(int *)(data + 0x84) == 0)) {
            *(float *)(data + 0x70) -= frame_time_sq * 15.0f;
        } else if (*(int *)(data + 0x84) != 0) {
            *(float *)(data + 0x70) -= D_L18_00161A40 * sink * frame_time_sq;
        } else {
            *(float *)(data + 0x70) -= D_L18_00161A44 * sink * frame_time_sq;
        }
        *(float *)(data + 0x64) += *(float *)(data + 0x70);
        if (moby->pos.z < *(float *)(data + 0x60) - 10.0f) {
            if (hero.ground_moby == moby && hero.air_frames.s == 0) {
                if (hero.motion.pos.f[2] < D_L18_0015F638 && hero.state.current != 0x77) {
                    FUN_L18_00227dd0(0x77, 1);
                }
            }
            *(float *)(data + 0x70) -= frame_time_sq * 13.7f;
            if (moby->pos.z < 15.0f) {
                mark_moby_for_removal((struct Obj *)moby);
                return;
            }
        }
        break;
    case STATE_WAIT_FOR_TILT_REQUEST:
        break;
    case STATE_WAIT_FOR_SINK_REQUEST:
        break;
    default:
        break;
    }
    update_floating_platform_tilt_and_bob(moby);
    add_vector_xyz(delta, delta, &moby->pos);
    update_entity_matrix(data + 0x20, delta, old_rot, &moby->rot);
}
/* Starts the moby's next phase with the given pair if it is waiting. */
int request_floating_platform_tilt(struct Moby *platform_moby, int tilt_delay_frames, int scaled_frame_count) __asm__("FUN_L18_002d8888");

int request_floating_platform_tilt(struct Moby *platform_moby, int tilt_delay_frames, int scaled_frame_count) {
    int *data = (int *)platform_moby->pvars;
    if (platform_moby->state == 1) {
        platform_moby->state = 3;
        data[0x7C / 4] = tilt_delay_frames;
        data[0x78 / 4] = scaled_frame_count;
        return 1;
    }
    return 0;
}
/* Spawns a random ring of particles around the moby. */
extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int FUN_001fa6e0(int, int, float);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern short D_L18_00161A68 __attribute__((sda));
extern short D_L18_00161A6C_gp __asm__("D_L18_00161A6C") __attribute__((sda));
extern short D_L18_00161A70 __attribute__((sda));
extern short D_L18_00161A74 __attribute__((sda));
extern short D_L18_00161A78 __attribute__((sda));
extern short D_L18_00161A7C __attribute__((sda));
extern short D_L18_00161A80 __attribute__((sda));
extern short D_L18_00161A84 __attribute__((sda));
extern short D_L18_00161A88 __attribute__((sda));
extern short D_L18_00161A8C __attribute__((sda));
extern short D_L18_00161A90 __attribute__((sda));
extern short D_L18_00161A94 __attribute__((sda));
extern short D_L18_00161A98 __attribute__((sda));
extern short D_L18_00161A9C __attribute__((sda));
extern short D_L18_00161AA0 __attribute__((sda));
extern short D_L18_00161AA4 __attribute__((sda));
extern short D_L18_00161AA8 __attribute__((sda));
extern short D_L18_00161AB4 __attribute__((sda));
extern void set_vector_unit_z(void *) __asm__("FUN_001f9a00");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void spawn_floating_platform_sparks(struct Moby *moby) {
    float spark_spawn_position[4];
    float world_emission_direction[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    float spark_vector_offset[4];
    int spark_count;
    float spawn_radius;
    float spawn_angle;
    int i0, i1, i2, i3, i4;
    char *moby_rotation_basis = &moby->rot_row0;
    int i;

    spark_count = random_integer_below(5) + 5;
    spawn_radius = random_float_between(1.0f, 2.5f);
    spawn_angle = random_angle_radians();
    spark_spawn_position[0] = fast_cos(spawn_angle) * spawn_radius;
    spark_spawn_position[1] = fast_sin(spawn_angle) * spawn_radius;
    spark_spawn_position[2] = 0.0f;
    transform_vector_by_basis(spark_spawn_position, spark_spawn_position, moby_rotation_basis);
    add_vector_xyz(spark_spawn_position, spark_spawn_position, &moby->pos);
    set_vector_unit_z(world_emission_direction);
    transform_vector_by_basis(world_emission_direction, world_emission_direction, moby_rotation_basis);
    for (i = 0; i < spark_count; i++) {
        FUN_L00_00257d78(spark_vector_offset, 0.0f, (*(float *)&(D_L18_00161A78)) * frame_time);
        normalize_vector_xyz(first_spark_vector, world_emission_direction,
                             (*(float *)&(D_L18_00161A68)) *
                                 (random_float_between(-(*(float *)&(D_L18_00161A74)),
                                                           (*(float *)&(D_L18_00161A74))) +
                                  1.0f) *
                                 frame_time);
        add_vector_xyz(first_spark_vector, first_spark_vector, spark_vector_offset);
        normalize_vector_xyz(second_spark_vector, first_spark_vector,
                             (*(float *)&(D_L18_00161A6C_gp)) *
                                 (random_float_between(-(*(float *)&(D_L18_00161A74)),
                                                           (*(float *)&(D_L18_00161A74))) +
                                  1.0f) *
                                 frame_time);
        second_spark_vector[2] = second_spark_vector[2] - (*(float *)&(D_L18_00161A70)) * frame_time;
        first_spark_vector[3] =
            random_float_between((*(float *)&(D_L18_00161A8C)), (*(float *)&(D_L18_00161A90)));
        second_spark_vector[3] =
            random_float_between((*(float *)&(D_L18_00161A94)), (*(float *)&(D_L18_00161A98)));
        i0 = FUN_001fa6e0((*(int *)&(D_L18_00161A9C)), (*(int *)&(D_L18_00161AA0)),
                          random_float_between(0.0f, 1.0f));
        i1 = FUN_001fa6e0((*(int *)&(D_L18_00161AA4)), (*(int *)&(D_L18_00161AA8)),
                          random_float_between(0.0f, 1.0f));
        i2 = func_001FA898_r(FUN_L04_001f1978(
            (float)(*(int *)&(D_L18_00161A7C)) * random_float_between(0.0f, 1.0f) + 1.0f));
        i3 = func_001FA898_r(FUN_L04_001f1978((float)(*(int *)&(D_L18_00161A80)) *
                                          (random_float_between(-(*(float *)&(D_L18_00161A88)),
                                                                    (*(float *)&(D_L18_00161A88))) +
                                           1.0f)));
        i4 = func_001FA898_r(FUN_L04_001f1978((float)(*(int *)&(D_L18_00161A84)) *
                                          (random_float_between(-(*(float *)&(D_L18_00161A88)),
                                                                    (*(float *)&(D_L18_00161A88))) +
                                           1.0f)));
        spawn_spark(spark_spawn_position, first_spark_vector, second_spark_vector, i0, i1, i2, i3, i4, (*(int *)&(D_L18_00161AB4)));
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DC850), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float);
extern float fast_sin(float);
extern float probe_ground_height(void *, int, float);
extern int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");
extern int scale_game_frames(int);
extern int tick_countdown_32(int *);
extern int truncate_float_to_s32_p() __asm__("FUN_001fa6d0");
extern short D_L18_00161BFC __attribute__((sda));
extern int D_L18_00161C00 __attribute__((sda));
extern float D_L18_00161C04 __attribute__((sda));
extern float D_L18_00161C08 __attribute__((sda));
extern float D_L18_00161C0C __attribute__((sda));
extern int D_L18_00161C10 __attribute__((sda));
extern int D_L18_00161C14 __attribute__((sda));
extern int D_L18_00161C18 __attribute__((sda));
extern float D_L18_00161C1C __attribute__((sda));
extern float D_L18_00161C20 __attribute__((sda));
extern float D_L18_00161C24;
extern float D_L18_00161C28 __attribute__((sda));
extern float D_L18_00161C2C __attribute__((sda));
extern int D_L18_00161C30 __attribute__((sda));
extern int D_L18_00161C34;
extern int D_L18_00161C38 __attribute__((sda));
extern int D_L18_00161C3C __attribute__((sda));
extern int D_L18_00161C40 __attribute__((sda));
extern int D_L18_00161C44 __attribute__((sda));
extern void jet_animate(unsigned char *m, float s) __asm__("FUN_L00_00259fe8");
extern void draw_expanding_projectile_ring() __asm__("FUN_L18_002dba20");
void process_expanding_ring_collisions(struct Moby *moby) __asm__("FUN_L18_002dbe80");

void process_expanding_ring_collisions(struct Moby *moby) __asm__("FUN_L18_002dbe80");
extern void add_vector_xyz(void *, void *, void *);
extern void clear_vector(void *);
extern void mark_moby_for_removal_p(void *) __asm__("FUN_0020c828");
extern void scale_vector_xyz(void *, void *, float);

void update_ballistic_ring_projectile(struct Moby *moby) __asm__("FUN_L18_002db460");

void update_ballistic_ring_projectile(struct Moby *moby) {
    enum {
        STATE_BALLISTIC_FLIGHT = 0,
        STATE_GROUND_IMPACT_DELAY = 1,
        STATE_EXPANDING_RING = 2,
        STATE_REMOVE = 3
    };
    float first_spark_vector[4];
    float second_spark_vector[4];
    float spark_vector_offset[4];
    float spark_spawn_position[4];
    float *moby_vars = (float *)moby->pvars;
    int i;
    int i0, i1, i2, i3, i4;

    if (hero.state.current == 0x72 || D_L18_0015F5C4 == 2) {
        mark_moby_for_removal_p(moby);
        return;
    }
    switch (moby->state) {
    case STATE_BALLISTIC_FLIGHT: {
        float *cc;
        i = 0;
        for (; i < D_L18_00161C40; i++) {
            cc = spark_vector_offset;
            scale_vector_xyz(first_spark_vector, moby_vars, D_L18_00161C08);
            FUN_L00_00257d78(cc, 0.0f, D_L18_00161C0C * frame_time);
            add_vector_xyz(first_spark_vector, cc, first_spark_vector);
            FUN_L00_00257d78(spark_spawn_position, 0.0f, 0.5f);
            add_vector_xyz(spark_spawn_position, spark_spawn_position, &moby->pos);
            clear_vector(second_spark_vector);
            first_spark_vector[3] = random_float_between(D_L18_00161C20, D_L18_00161C24);
            second_spark_vector[3] = random_float_between(D_L18_00161C28, D_L18_00161C2C);
            i0 = FUN_001fa6e0(D_L18_00161C30, D_L18_00161C34,
                              random_float_between(0.0f, 1.0f));
            i1 = FUN_001fa6e0(D_L18_00161C38, D_L18_00161C3C,
                              random_float_between(0.0f, 1.0f));
            i2 = func_001FA898_r(FUN_L04_001f1978(
                (float)D_L18_00161C10 * random_float_between(0.0f, 1.0f) + 1.0f));
            i3 = func_001FA898_r(FUN_L04_001f1978(
                (float)D_L18_00161C14 *
                (random_float_between(-D_L18_00161C1C, D_L18_00161C1C) +
                 1.0f)));
            i4 = func_001FA898_r(FUN_L04_001f1978(
                (float)D_L18_00161C18 *
                (random_float_between(-D_L18_00161C1C, D_L18_00161C1C) +
                 1.0f)));
            second_spark_vector[2] = second_spark_vector[2] - D_L18_00161C04 * frame_time_sq * (float)i3;
            spawn_spark(spark_spawn_position, first_spark_vector, second_spark_vector, i0, i1, i2, i3, i4, D_L18_00161C44);
        }
        moby_vars[12] = fast_add_rotations(moby_vars[12], frame_time * 18.849556f);
        {
            int col = FUN_001fa6e0(*(int *)&D_L18_00161BFC, D_L18_00161C00,
                                   (fast_sin(moby_vars[12]) + 1.0f) * 0.5f);
            float k = frame_time_sq * 10.0f;
            moby->glow_color = col;
            moby_vars[2] = moby_vars[2] - k;
        }
        add_vector_xyz(&moby->pos, &moby->pos, moby_vars);
        {
            float ground_height = probe_ground_height(&moby->pos, 0, 0.5f);
            if (moby->pos.z - 0.333f < ground_height) {
                moby->state = 1;
                moby->pos.z = ground_height + 0.333f;
                *(int *)(moby_vars + 11) = scale_game_frames(1);
            }
        }
        moby->rot.y =
            fast_add_rotations(moby->rot.y, frame_time * 8.726646f);
        if (moby->pos.z < 10.0f) {
            mark_moby_for_removal_p(moby);
        } else {
            jet_animate((unsigned char *)moby, 1.0f);
        }
        break;
    }
    case STATE_GROUND_IMPACT_DELAY:
        moby_vars[12] = fast_add_rotations(moby_vars[12], frame_time * 18.849556f);
        moby->glow_color = FUN_001fa6e0(*(int *)&D_L18_00161BFC, D_L18_00161C00,
                                             (fast_sin(moby_vars[12]) + 1.0f) * 0.5f);
        if (tick_countdown_32((int *)(moby_vars + 11))) {
            trooper_play_sound(0xF, 0, (int)moby, 0x58E);
            moby->state = 2;
            moby_vars[9] = 0.5f;
            moby->collision = 0;
            moby->visible = 0;
            moby->flags |= 1;
        }
        break;
    case STATE_EXPANDING_RING:
        moby_vars[9] = moby_vars[9] + moby_vars[8];
        process_expanding_ring_collisions(moby);
        enqueue_callback_list_1(draw_expanding_projectile_ring, moby);
        if (moby_vars[9] > moby_vars[10]) {
            moby->state = 3;
        }
        break;
    case STATE_REMOVE:
        mark_moby_for_removal_p(moby);
        break;
    }
}
/* Allocate an object, init its fields and child record. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DCD28), where it is exact; names translated to the US level program. */

extern short D_L18_00161BFC_d __asm__("D_L18_00161BFC") __attribute__((sda));

extern float D_L18_00161BF8 __attribute__((sda));
void spawn_class_270_moby(char *pos, float *initial_movement_step, int collision_filter_moby, float fa, float fb) __asm__("FUN_L18_002db938");

void spawn_class_270_moby(char *pos, float *initial_movement_step, int collision_filter_moby, float fa, float fb) {
    Obj *spawned_moby = (Obj *)func_0020D348_m(0x270);
    if (spawned_moby != 0) {
        Child *moby_vars;
        int initial_glow_color;
        spawned_moby->b30 = 0xFF;
        spawned_moby->b31 = 1;
        spawned_moby->b20 = 0;
        initial_glow_color = *(int *)&D_L18_00161BFC_d;
        spawned_moby->f2C = spawned_moby->f2C * D_L18_00161BF8;
        spawned_moby->s32 = 0xFF;
        spawned_moby->h34 |= 0x10;
        spawned_moby->f90 = initial_glow_color;
        moby_vars = spawned_moby->child;
        qcopy(spawned_moby->pos, pos);
        spawned_moby->f48 = fast_arc_tan(initial_movement_step[0], initial_movement_step[1]);
        qcopy(moby_vars->v, initial_movement_step);
        moby_vars->f34 = collision_filter_moby;
        moby_vars->f28 = fa;
        moby_vars->f20 = fb;
        moby_vars->f30 = 0;
    }
}
/* Ported from rac1-decomp nonmatching/l18_veldin2/func_L18_002DCE10.c (func_L18_002DCE10) */
typedef struct { float f[4]; } __attribute__((aligned(16))) V_dba20;
typedef struct { float u, v; } UV_dba20;
typedef struct {
    V_dba20 corner[4];
    unsigned int color[4];
    UV_dba20 uv[4];
    long unk70, tex, unk80, unk88;
} Q_dba20;
typedef struct {
    char pad0[0x20];
    float f20, f24, f28;
} D_dba20;
typedef struct {
    char pad0[0x10];
    V_dba20 pos;
    char pad20[0x58];
    D_dba20 *data;
} M_dba20;

extern void unit_matrix_dba20(void *) __asm__("FUN_001f9fc8");
extern int get_effect_texture_dba20(int) __asm__("FUN_001f44b8");
extern int truncate_dba20(float) __asm__("FUN_001fa6d0");
extern int tween_color_dba20(int, int, float) __asm__("FUN_001fa6e0");
extern float scale_time_dba20(float) __asm__("FUN_001f96b0");
extern float add_rotations_dba20(float, float) __asm__("FUN_001fa580");
extern float cos_dba20(float) __asm__("FUN_001f9dc8");
extern float sin_dba20(float) __asm__("FUN_001f9de0");
extern void clear_vector_dba20(void *) __asm__("FUN_001f99f8");
extern void euler_basis_dba20(void *, void *) __asm__("FUN_001fa030");
extern void draw_quad_dba20(void *, void *, int) __asm__("FUN_001f7d30");
extern void matrix_mul_dba20(void *, void *, void *) __asm__("FUN_001fa328");

extern float D_L18_001D4310[];

extern int D_L18_00161C48 __attribute__((sda));
extern int D_L18_00161C4C __attribute__((sda));
extern int D_L18_00161C50 __attribute__((sda));
extern int D_L18_00161C54 __attribute__((sda));
extern int D_L18_00161C5C __attribute__((sda));
extern int D_L18_00161C60 __attribute__((sda));
extern float D_L18_00161C64 __attribute__((sda));
extern float D_L18_00161C68 __attribute__((sda));
extern float D_L18_00161C6C __attribute__((sda));
extern float D_L18_00161C70 __attribute__((sda));
extern float D_L18_00161C74 __attribute__((sda));
extern float D_L18_00161C78 __attribute__((sda));
extern float D_L18_00161C7C __attribute__((sda));
void draw_expanding_projectile_ring(M_dba20 *moby) {
    Q_dba20 quad_packets[4];
    V_dba20 draw_matrix[4];
    V_dba20 segment_rotation_basis[3];
    V_dba20 rv;
    D_dba20 *moby_vars = moby->data;
    int j, k, ring_segment_index;
    int col;

    unit_matrix_dba20(draw_matrix);
    qcopy(&draw_matrix[3], &moby->pos);
    draw_matrix[3].f[2] += D_L18_00161C68;
    draw_matrix[3].f[3] = 1.0f;
    for (j = 0; j < 4; j++) {
        float fj;
        float layer_phase, color_fade_weight, rad;
        quad_packets[j].tex = get_effect_texture_dba20(0xE);
        quad_packets[j].unk70 = 0;
        quad_packets[j].unk80 = 0xFF9000000260L;
        quad_packets[j].unk88 = (long)D_L18_00161C48 | ((long)D_L18_00161C4C << 2) | ((long)D_L18_00161C50 << 4) | ((long)D_L18_00161C54 << 6) | 0x8000000000L;
        layer_phase = moby_vars->f24 * D_L18_00161C74 + (float)j * 0.25f;
        rad = moby_vars->f24 + D_L18_00161C70 * (float)j;
        layer_phase = layer_phase - (float)truncate_dba20(layer_phase);
        col = tween_color_dba20(D_L18_00161C5C, D_L18_00161C60, layer_phase);
        color_fade_weight = layer_phase * D_L18_00161C78;
        if (color_fade_weight > 1.0f) color_fade_weight = 1.0f;
        col = tween_color_dba20(col & 0xFFFFFF, col, color_fade_weight);
        color_fade_weight = (moby_vars->f28 - moby_vars->f24) / (moby_vars->f20 * scale_time_dba20(D_L18_00161C7C));
        if (color_fade_weight > 1.0f) {
            color_fade_weight = 1.0f;
        } else if (color_fade_weight < 0.0f) {
            color_fade_weight = 0.0f;
        }
        col = tween_color_dba20(col & 0xFFFFFF, col, color_fade_weight);
        fj = (float)j;
        for (k = 0; k < 4; k++) {
            float corner_angle;
            quad_packets[j].uv[k].u = D_L18_001D4310[2 * k];
            quad_packets[j].uv[k].v = D_L18_001D4310[2 * k + 1];
            if (k & 1) {
                quad_packets[j].corner[k].f[2] = layer_phase * D_L18_00161C6C;
            } else {
                quad_packets[j].corner[k].f[2] = -layer_phase * D_L18_00161C6C;
            }
            corner_angle = add_rotations_dba20((float)(k >> 1) * (D_L18_00161C64 * 0.017453292f) - 3.14159f,
                              D_L18_00161C64 * 0.25f * fj * 0.017453292f);
            quad_packets[j].corner[k].f[0] = cos_dba20(corner_angle) * rad;
            quad_packets[j].corner[k].f[1] = sin_dba20(corner_angle) * rad;
            quad_packets[j].corner[k].f[3] = 1.0f;
            quad_packets[j].color[k] = col;
        }
    }
    clear_vector_dba20(&rv);
    rv.f[2] = D_L18_00161C64 * 0.017453292f;
    euler_basis_dba20(segment_rotation_basis, &rv);
    for (ring_segment_index = 0; (float)ring_segment_index < 360.0f / D_L18_00161C64; ring_segment_index++) {
        draw_quad_dba20(&quad_packets[0], draw_matrix, 0);
        draw_quad_dba20(&quad_packets[1], draw_matrix, 0);
        draw_quad_dba20(&quad_packets[2], draw_matrix, 0);
        draw_quad_dba20(&quad_packets[3], draw_matrix, 0);
        matrix_mul_dba20(draw_matrix, draw_matrix, segment_rotation_basis);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD270), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern void FUN_L18_002dc0c8_u(char *moby, void *a1, void *a2) __asm__("FUN_L18_002dc0c8");

void process_expanding_ring_collisions(struct Moby *moby) {
    char hit[0x30];
    float hit_direction_vector[4];
    float segment_start[4];
    float segment_end[4];
    char *moby_vars = (char *)moby->pvars;
    float segment_start_angle, segment_end_angle, step;
    int i;

    fast_vec_sub(hit_direction_vector, &hero.motion.pos, &moby->pos);
    hit_direction_vector[2] = 0.0f;
    fast_vec_normalize(hit_direction_vector, hit_direction_vector, frame_time * 4.0f);
    hit_direction_vector[2] = frame_time * 4.0f;
    make_hit_descriptor(hit, (int)moby, 0x10001, hit_direction_vector, 3.0f);
    step = 1.0f / convert_integer_to_float(0x40) * 6.28318f;
    segment_start_angle = -3.14159f;
    segment_end_angle = fast_add_rotations(segment_start_angle, step);
    for (i = 0x3F; i >= 0; i--) {
        segment_start[0] = fast_cos(segment_start_angle) * *(float *)(moby_vars + 0x24);
        segment_start[1] = fast_sin(segment_start_angle) * *(float *)(moby_vars + 0x24);
        segment_start[2] = 0.0f;
        segment_end[0] = fast_cos(segment_end_angle) * *(float *)(moby_vars + 0x24);
        segment_end[1] = fast_sin(segment_end_angle) * *(float *)(moby_vars + 0x24);
        segment_end[2] = 0.0f;
        fast_vec_add(segment_start, segment_start, &moby->pos);
        segment_start[2] += 0.5f;
        fast_vec_add(segment_end, segment_end, &moby->pos);
        segment_end[2] += 0.5f;
        collision_line(segment_start, segment_end, 1, *(void **)(moby_vars + 0x34), hit);
        if (*(float *)(moby_vars + 0x24) < 3.0f) {
            FUN_L18_002dc0c8_u(moby, segment_start, segment_end);
        }
        segment_start_angle = fast_add_rotations(segment_start_angle, step);
        segment_end_angle = fast_add_rotations(segment_end_angle, step);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD4B8), where it is exact; names translated to the US level program. */

extern float FUN_001f9b48(void *, void *);
extern int FUN_001fa6e0(int, int, float);
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern s32 random_integer_below(s32) __asm__("FUN_00213260");
extern void FUN_L00_002730e0(void *, void *, int, int, int, int, float);

extern float D_L18_00161BF4 __attribute__((sda));
extern int D_L18_00161C80 __attribute__((sda));
extern int D_L18_00161C84 __attribute__((sda));
extern float D_L18_00161C88 __attribute__((sda));
extern int D_L18_00161C8C __attribute__((sda));
extern int D_L18_00161C90 __attribute__((sda));
extern float D_L18_00161C98 __attribute__((sda));
void spawn_expanding_ring_segment_particles(struct Moby *moby, void *segment_start, void *segment_end) __asm__("FUN_L18_002dc0c8");

void spawn_expanding_ring_segment_particles(struct Moby *moby, void *segment_start, void *segment_end) {
    float segment_bounding_sphere[4];
    float particle_spawn_position[4];
    float particle_velocity[4];
    char *moby_vars = (char *)moby->pvars;
    int col;

    lerp_vector(segment_bounding_sphere, segment_start, segment_end, 0.5f);
    segment_bounding_sphere[3] = FUN_001f9b48(segment_start, segment_end) * 0.5f;
    if (fast_bsphere_check((char *)segment_bounding_sphere, D_L18_00161BF4) == -1) {
        return;
    }
    if (random_float_between(0.0f, 1.0f) < D_L18_00161C98) {
        lerp_vector(particle_spawn_position, segment_start, segment_end, random_float_between(0.0f, 1.0f));
        fast_vec_sub(particle_velocity, particle_spawn_position, &moby->pos);
        fast_vec_normalize(particle_velocity, particle_velocity, *(float *)(moby_vars + 0x20));
        col = FUN_001fa6e0(D_L18_00161C80, D_L18_00161C84,
                           random_float_between(0.0f, 1.0f));
        FUN_L00_002730e0(particle_spawn_position, particle_velocity, col, scale_game_frames(D_L18_00161C8C) & 0xFF,
                         random_integer_below(0xFF) & 0xFF, 0, D_L18_00161C88);
        particle_velocity[2] = 0.0f;
        FUN_L00_002730e0(particle_spawn_position, particle_velocity, col, scale_game_frames(D_L18_00161C90) & 0xFF,
                         random_integer_below(0xFF) & 0xFF, 0, D_L18_00161C88);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD848), where it is exact; names translated to the US level program. */

extern s32 D_L18_00161C9C_d __asm__("D_L18_00161C9C") __attribute__((sda));

void refresh_ring_effect(struct Moby *moby, void *pos, float radius) __asm__("FUN_L18_002dc458");

void refresh_ring_effect(struct Moby *moby, void *pos, float radius) {
    int *moby_vars = (int *)moby->pvars;
    int countdown_limit_frames;
    qcopy(&moby->pos, pos);
    countdown_limit_frames = *(int *)&D_L18_00161C9C_d;
    moby_vars[2] += 2;
    *(float *)moby_vars = radius;
    if (scale_game_frames(countdown_limit_frames) < moby_vars[2]) {
        moby_vars[2] = scale_game_frames(*(int *)&D_L18_00161C9C_d);
    }
}
/* Ported from rac1-decomp nonmatching/l18_veldin2/func_L18_002DD8A8.c (func_L18_002DD8A8) */
typedef struct { float f[4]; } __attribute__((aligned(16))) V_dc4b8;
typedef struct { float u, v; } UV_dc4b8;
typedef struct {
    V_dc4b8 corner[4];
    unsigned int color[4];
    UV_dc4b8 uv[4];
    long unk70, tex, unk80, unk88;
} Q_dc4b8;
extern void unit_matrix_dc4b8(void *) __asm__("FUN_001f9fc8");
extern int get_effect_texture_dc4b8(int) __asm__("FUN_001f44b8");
extern void draw_quad_dc4b8(void *, void *, int) __asm__("FUN_001f7d30");
extern void matrix_mul_dc4b8(void *, void *, void *) __asm__("FUN_001fa328");
extern float itof_dc4b8(int) __asm__("FUN_001fa6c0");
extern int scale_ticks_dc4b8(int) __asm__("FUN_001f96f8");
extern float add_rotations_dc4b8(float, float) __asm__("FUN_001fa580");
extern float cos_dc4b8(float) __asm__("FUN_001f9dc8");
extern float sin_dc4b8(float) __asm__("FUN_001f9de0");
extern int tween_color_dc4b8(int, int, float) __asm__("FUN_001fa6e0");
extern int truncate_dc4b8(float) __asm__("FUN_001fa6d0");
extern void clear_vector_dc4b8(void *) __asm__("FUN_001f99f8");
extern void euler_basis_dc4b8(void *, void *) __asm__("FUN_001fa030");
extern float D_L18_001D4330[];
extern int D_L18_00161C9C_v __asm__("D_L18_00161C9C") __attribute__((sda));

extern int D_L18_00161CA4 __attribute__((sda));
extern int D_L18_00161CA8 __attribute__((sda));
extern int D_L18_00161CAC __attribute__((sda));
extern int D_L18_00161CB0 __attribute__((sda));
extern int D_L18_00161CB8 __attribute__((sda));
extern int D_L18_00161CBC __attribute__((sda));
extern float D_L18_00161CC0 __attribute__((sda));
extern float D_L18_00161CC4 __attribute__((sda));
extern float D_L18_00161CC8 __attribute__((sda));
extern float D_L18_00161CCC __attribute__((sda));
extern float D_L18_00161CD4 __attribute__((sda));
void draw_class_274_ring_effect(char *moby) {
    Q_dc4b8 quad[4];
    V_dc4b8 quad_draw_transform[4];
    V_dc4b8 ring_segment_rotation_basis[3];
    V_dc4b8 ring_segment_rotation_angles;
    float *moby_vars;
    int i, j, k;
    int layer_color;
    float layer_phase, r, countdown_ticks_float, color_fade_weight, gi;

    moby_vars = *(float **)(moby + 0x78);
    unit_matrix_dc4b8(quad_draw_transform);
    qcopy(&quad_draw_transform[3], moby + 0x10);
    quad_draw_transform[3].f[2] = quad_draw_transform[3].f[2] + D_L18_00161CC4;
    quad_draw_transform[3].f[3] = 1.0f;
    for (i = 0; i < 4; i++) {
        quad[i].tex = get_effect_texture_dc4b8(14);
        gi = D_L18_00161CCC * (float)i;
        quad[i].unk70 = 0;
        quad[i].unk80 = 0xFF9000000260L;
        quad[i].unk88 = (long)D_L18_00161CA4 | ((long)D_L18_00161CA8 << 2) |
                        ((long)D_L18_00161CAC << 4) | ((long)D_L18_00161CB0 << 6) | 0x8000000000L;
        layer_phase = moby_vars[1] + (float)i * 0.25f;
        r = moby_vars[0] + gi;
        layer_phase = layer_phase - (float)truncate_dc4b8(layer_phase);
        layer_color = tween_color_dc4b8(D_L18_00161CB8, D_L18_00161CBC, layer_phase);
        color_fade_weight = layer_phase * D_L18_00161CD4;
        if (color_fade_weight > 1.0f) color_fade_weight = 1.0f;
        layer_color = tween_color_dc4b8(layer_color & 0xFFFFFF, layer_color, color_fade_weight);
        countdown_ticks_float = itof_dc4b8(((int *)moby_vars)[2]);
        color_fade_weight = countdown_ticks_float / (float)scale_ticks_dc4b8(D_L18_00161C9C_v);
        if (color_fade_weight > 1.0f) {
            color_fade_weight = 1.0f;
        } else if (color_fade_weight < 0.0f) {
            color_fade_weight = 0.0f;
        }
        layer_color = tween_color_dc4b8(layer_color & 0xFFFFFF, layer_color, color_fade_weight);
        for (j = 0; j < 4; j++) {
            float corner_angle;
            quad[i].uv[j].u = D_L18_001D4330[j * 2];
            quad[i].uv[j].v = D_L18_001D4330[j * 2 + 1];
            quad[i].corner[j].f[2] = (j & 1) ? layer_phase * D_L18_00161CC8 : -layer_phase * D_L18_00161CC8;
            corner_angle = add_rotations_dc4b8((float)(j >> 1) * (D_L18_00161CC0 * 0.017453292f) - 3.14159f,
                              D_L18_00161CC0 * 0.25f * (float)i * 0.017453292f);
            quad[i].corner[j].f[0] = cos_dc4b8(corner_angle) * r;
            quad[i].corner[j].f[1] = sin_dc4b8(corner_angle) * r;
            quad[i].corner[j].f[3] = 1.0f;
            quad[i].color[j] = layer_color;
        }
    }
    clear_vector_dc4b8(&ring_segment_rotation_angles);
    ring_segment_rotation_angles.f[2] = D_L18_00161CC0 * 0.017453292f;
    euler_basis_dc4b8(&ring_segment_rotation_basis[0], &ring_segment_rotation_angles);
    for (k = 0; (float)k < 360.0f / D_L18_00161CC0; k++) {
        draw_quad_dc4b8(&quad[0], quad_draw_transform, 0);
        draw_quad_dc4b8(&quad[1], quad_draw_transform, 0);
        draw_quad_dc4b8(&quad[2], quad_draw_transform, 0);
        draw_quad_dc4b8(&quad[3], quad_draw_transform, 0);
        matrix_mul_dc4b8(quad_draw_transform, quad_draw_transform, &ring_segment_rotation_basis[0]);
    }
}
/* UpdateMoby state machine for the vendor moby (state at +0x20). */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002E09F8), where it is exact; names translated to the US level program. */

extern char D_001413D4_q[] __asm__("D_001413D4") __attribute__((section(".data")));
extern short D_L18_00161A6C __asm__("D_0015ED6C");
extern float D_L18_0015F3FC_d __asm__("D_L18_0015F3FC");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern int give_reward(int a, int b) __asm__("FUN_L00_00203908");
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void FUN_L00_00216f90_q(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L00_002ea9d8_q(void *) __asm__("FUN_L00_002ea9d8");
extern void FUN_L00_002ea9d8_q_002eaa30(void *) __asm__("FUN_L00_002eaa30");
extern void FUN_L00_002eaaa0_q(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eac18_q(int) __asm__("FUN_L00_002eac18");

extern int D_L18_0015F404;
extern int D_L18_00179E10[] __attribute__((section(".data")));
extern int D_L18_0017EF08 __attribute__((section(".data")));
void update_position_and_rotation_transition(struct Moby *moby) __asm__("FUN_L18_002df608");

void update_position_and_rotation_transition(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_PRE_TRANSITION_RAMP_UP = 2,
        STATE_INTERPOLATE_TRANSITION = 3,
        STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP = 4,
        STATE_POST_TRANSITION_RAMP_DOWN = 5
    };
    char *data = (char *)moby->pvars;
    float transition_scratch_vectors[12];
    switch (moby->state) {
    case STATE_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case STATE_PRE_TRANSITION_RAMP_UP:
        D_L18_0017EF08 = 1;
        approach_value((float *)(data + 0x6C), 1.0f, (*(float *)&D_L18_00161A6C) * 4.0f);
        {
            float t = *(float *)(data + 0x6C);
            if (D_L18_0015F3FC_d < t) {
                D_L18_0015F3FC_d = t;
            }
            if (1.0f <= t) {
                D_L18_0015F404 = 1;
                clear_u64_value(transition_scratch_vectors);
                transition_scratch_vectors[2] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors, 0x72, 1);
                FUN_L00_002eaaa0_q(data, data + 0x20, 1, 0, 0);
                if (*(int *)(data + 0x78) == 1) {
                    give_reward(0x4650, 0x85);
                    moby->state = 4;
                } else {
                    moby->state = 3;
                }
            }
        }
        break;
    case STATE_INTERPOLATE_TRANSITION:
        D_L18_0017EF08 = 1;
        if (fast_dec_timer((int *)(data + 0x74))) {
            advance_accelerated_scalar((float *)(data + 0x60), (float *)(data + 0x64), 1.0f,
                           *(float *)(data + 0x68), *(float *)(data + 0x68),
                           *(float *)(data + 0x70));
        }
        lerp_vector(transition_scratch_vectors, data, data + 0x10, *(float *)(data + 0x60));
        clear_u64_value(transition_scratch_vectors + 4);
        transition_scratch_vectors[5] = fast_subtract_rotations(*(float *)(data + 0x34), *(float *)(data + 0x24)) *
               *(float *)(data + 0x60);
        transition_scratch_vectors[5] = FUN_001fa580_q(*(float *)(data + 0x24), transition_scratch_vectors[5]);
        transition_scratch_vectors[6] = fast_subtract_rotations(*(float *)(data + 0x38), *(float *)(data + 0x28)) *
               *(float *)(data + 0x60);
        transition_scratch_vectors[6] = FUN_001fa580_q(*(float *)(data + 0x28), transition_scratch_vectors[6]);
        FUN_L00_002ea9d8_q(transition_scratch_vectors);
        FUN_L00_002ea9d8_q_002eaa30(transition_scratch_vectors + 4);
        approach_value((float *)(data + 0x6C), 0.0f, (*(float *)&D_L18_00161A6C) * 4.0f);
        {
            float transition_progress = *(float *)(data + 0x60);
            D_L18_0015F3FC_d = *(float *)(data + 0x6C);
            if (1.0f <= transition_progress) {
                D_L18_0015F404 = 0;
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0, 0);
                FUN_L00_002eac18_q(2);
                moby->state = 5;
            } else if (*(int *)(D_001413D4_q) != 0x72) {
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0x72, 1);
                return;
            }
        }
        break;
    case STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP:
        if (fast_dec_timer((int *)(data + 0x74))) {
            advance_accelerated_scalar((float *)(data + 0x60), (float *)(data + 0x64), 1.0f,
                           *(float *)(data + 0x68), *(float *)(data + 0x68),
                           *(float *)(data + 0x70));
        }
        lerp_vector(transition_scratch_vectors, data, data + 0x10, *(float *)(data + 0x60));
        clear_u64_value(transition_scratch_vectors + 4);
        transition_scratch_vectors[5] = fast_subtract_rotations(*(float *)(data + 0x34), *(float *)(data + 0x24)) *
               *(float *)(data + 0x60);
        transition_scratch_vectors[5] = FUN_001fa580_q(*(float *)(data + 0x24), transition_scratch_vectors[5]);
        transition_scratch_vectors[6] = fast_subtract_rotations(*(float *)(data + 0x38), *(float *)(data + 0x28)) *
               *(float *)(data + 0x60);
        transition_scratch_vectors[6] = FUN_001fa580_q(*(float *)(data + 0x28), transition_scratch_vectors[6]);
        FUN_L00_002ea9d8_q(transition_scratch_vectors);
        FUN_L00_002ea9d8_q_002eaa30(transition_scratch_vectors + 4);
        if (1.0f <= *(float *)(data + 0x60) ||
            (0.2f < *(float *)(data + 0x60) && D_L18_00179E10[0] == 0 &&
             D_L18_00179E10[9] == -1)) {
            float t;
            approach_value((float *)(data + 0x6C), 1.0f, (*(float *)&D_L18_00161A6C) * 4.0f);
            t = *(float *)(data + 0x6C);
            D_L18_0015F3FC_d = t;
            if (1.0f <= t) {
                D_L18_0015F404 = 0;
                *(float *)(data + 0x60) = 1.0f;
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0, 0);
                FUN_L00_002eac18_q(3);
                moby->state = 5;
            }
        } else if (*(int *)(D_001413D4_q) != 0x72) {
            clear_u64_value(transition_scratch_vectors + 8);
            transition_scratch_vectors[10] = *(float *)(data + 0x4C);
            FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0x72, 1);
            return;
        } else {
            approach_value((float *)(data + 0x6C), 0.0f, (*(float *)&D_L18_00161A6C) * 4.0f);
            D_L18_0015F3FC_d = *(float *)(data + 0x6C);
        }
        break;
    case STATE_POST_TRANSITION_RAMP_DOWN:
        if (0.0f < *(float *)(data + 0x6C)) {
            approach_value((float *)(data + 0x6C), 0.0f, (*(float *)&D_L18_00161A6C) * 4.0f);
            D_L18_0015F3FC_d = *(float *)(data + 0x6C);
        } else {
            moby->state = 1;
        }
        break;
    }
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002E0E90), where it is exact; names translated to the US level program. */

extern float D_0015EE70 __asm__("D_0015ED70");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");

void start_position_and_rotation_transition(struct Moby *moby, void *transition_start_position, void *transition_target_position, void *transition_start_rotation, void *transition_target_rotation, void *transition_setup_vector, int invulnerability_duration_frames, int transition_delay_frames,
                      int transition_mode) __asm__("FUN_L18_002dfaa0");

void start_position_and_rotation_transition(struct Moby *moby, void *transition_start_position, void *transition_target_position, void *transition_start_rotation, void *transition_target_rotation, void *transition_setup_vector, int invulnerability_duration_frames, int transition_delay_frames,
                      int transition_mode) {
    char *data = (char *)moby->pvars;
    *(int *)(data + 0x60) = 0;
    *(int *)(data + 0x64) = 0;
    qcopy(data, transition_start_position);
    qcopy(data + 0x10, transition_target_position);
    qcopy(data + 0x20, transition_start_rotation);
    qcopy(data + 0x30, transition_target_rotation);
    qcopy(data + 0x40, transition_setup_vector);
    moby->state = 2;
    *(float *)(data + 0x68) = D_0015EE70 * 0.333f;
    *(float *)(data + 0x70) = 1.0f / (float)scale_game_frames(invulnerability_duration_frames);
    *(int *)(data + 0x74) = scale_game_frames(transition_delay_frames);
    *(int *)(data + 0x78) = transition_mode;
    hero.invulnerable_frames = scale_game_frames(invulnerability_duration_frames);
}
/* Maps states 2 and 3 to results 1 and 2; all other states return 0. */
s32 FUN_L18_002dfba0(const u8 *moby) {
    u32 state = moby[0x20];
    s32 result = 2;

    if (state == 2) {
        return 1;
    }
    if (state != 3) {
        result = 0;
    }
    return result;
}
extern int FUN_L00_0025a420(void *, int, int);
extern void set_moby_group_state_two(int) __asm__("FUN_L18_002e9e50");
extern void cart_burst(float, float, void *, void *, int) __asm__("FUN_L00_0025f090");
extern void FUN_L00_00257d78(float *, float, float);
extern float FUN_L00_00257c48(float, float);
extern float random_float_between_9768(float, float) __asm__("FUN_002132a8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int random_integer_below_9768(int) __asm__("FUN_00213260");
extern int scale_game_frames_9768(int) __asm__("FUN_001f96f8");
extern float fast_add_rotations_9768(float, float) __asm__("FUN_001fa580");
extern float absolute_float_9768(float) __asm__("FUN_001f99c0");
extern void subtract_vector_9768(float *, float *, float *) __asm__("FUN_001f9a28");
extern void scale_vector_9768(float *, float *, float) __asm__("FUN_001f9bf8");
extern void add_vector_9768(float *, float *, float *) __asm__("FUN_001f9a10");
extern void vector_from_angles_9768(float *, float, float) __asm__("FUN_L00_00257d78");
extern int hit_test_9768(float, float *, int, void *) __asm__("FUN_L00_001f0d60");
extern void remove_moby_9768(void *) __asm__("FUN_0020c828");
extern void spawn_particle(void *, void *, int, int, float, int, int, float, int, int) __asm__("FUN_L00_0026a9f0");
extern void spawn_particle_alt_9768(float, void *, void *, int, int, int) __asm__("FUN_L00_0026a4c8");
extern float D_0015EE70 __asm__("D_0015ED70");
extern int D_L18_0015F580;
extern float D_0013F3D0[4];
#include "rnc/gameplay/state/game_globals.h"

extern unsigned char D_L18_001F29A0_9768[] __asm__("D_L18_001F29A0");
extern unsigned char D_L18_001F29B8_9768[] __asm__("D_L18_001F29B8");

void update_moby_hit_response_burst_and_ballistic_flight(struct Moby *moby) __asm__("FUN_L18_002e9768");

void update_moby_hit_response_burst_and_ballistic_flight(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_WAIT_FOR_HIT = 1,
        STATE_BURST_OR_LAUNCH = 2,
        STATE_BALLISTIC_FLIGHT = 3
    };
    float *data = (float *)moby->pvars;
    long state = moby->state;
    float magnitude;

    switch (state) {
    case STATE_INITIALIZE: {
        if (moby->mission != 0xff) {
            int mission_table_index = moby->mission + current_level_index * 16;
            if (((unsigned char *)((int *)visited52))[mission_table_index] == 0xff) goto remove;
        }
        moby->state = 1;
        if (*(int *)data != 0) *(int *)(*(int *)data) = 3;
        return;
    }
    case STATE_WAIT_FOR_HIT:
        if (FUN_L00_0025a420(moby, 0x80000, 0)) {
            set_moby_group_state_two(moby->group);
            trooper_play_sound(0, 0, (int)moby, 0x375);
        }
        moby->hit_slot = 0xff;
        return;
    case STATE_BURST_OR_LAUNCH:
        if (moby->oclass == 0x37c) {
            float *pos;
            {
                int i;
                int *first, *second;
                float colors[6];
                float colors2[6];
                float position[4];
                i = 0;
                do {
                    pos = &moby->pos.x;
                    magnitude = random_float_between_9768(8.0f, 10.0f) * frame_time;
                    __builtin_memcpy(colors, D_L18_001F29A0_9768, 24);
                    __builtin_memcpy(colors2, D_L18_001F29B8_9768, 24);
                    qcopy(position, pos);
                    ++i;
                    position[2] += random_float_between_9768(0.0f, 6.0f);
                    first = (int *)colors + random_integer_below_9768(6);
                    second = (int *)colors2 + random_integer_below_9768(6);
                    {
                        int lifetime, spread;
                        lifetime = random_int_between(scale_game_frames_9768(15), scale_game_frames_9768(20));
                        spread = random_int_between(scale_game_frames_9768(30), scale_game_frames_9768(45));
                        spawn_particle(position, &D_L18_0015F580, *first, *second, 1500000.0f, lifetime, spread, magnitude, 0, 0);
                    }
                    {
                        int lifetime, spread;
                        lifetime = random_int_between(scale_game_frames_9768(5), scale_game_frames_9768(10));
                        spread = random_int_between(scale_game_frames_9768(15), scale_game_frames_9768(20));
                        spawn_particle(position, &D_L18_0015F580, 0x7fffffff, 0xffffff, 1500000.0f, lifetime, spread, magnitude * 0.5f, 0, 0);
                    }
                } while ((float)i < 10.0f);
            }
            {
                int i;
                int *first, *second;
                float position[4];
                float colors[6];
                float velocity[4];
                float colors2[6];
                float direction[4];
                float zero;
                i = 0;
                zero = (float)i;
                do {
                    qcopy(position, pos);
                    ++i;
                    position[2] += random_float_between_9768(zero, 6.0f);
                    __builtin_memcpy(colors, D_L18_001F29A0_9768, 24);
                    __builtin_memcpy(colors2, D_L18_001F29B8_9768, 24);
                    *(OvlQuad *)direction = 0;
                    direction[0] = random_float_between_9768(-1.0f, 1.0f);
                    direction[1] = random_float_between_9768(-1.0f, 1.0f);
                    direction[2] = random_float_between_9768(-1.0f, 1.0f);
                    *(u128 *)velocity = *(u128 *)direction;
                    scale_vector_9768(velocity, velocity, random_float_between_9768(zero, 3.0f) * frame_time);
                    first = (int *)colors + random_integer_below_9768(6);
                    second = (int *)colors2 + random_integer_below_9768(6);
                    spawn_particle_alt_9768(500000.0f, position, velocity, *first,
                    *second,
                    random_int_between(scale_game_frames_9768(30), scale_game_frames_9768(45)));
                } while ((float)i < 10.0f);
            }
remove:
            remove_moby_9768(moby);
            return;
        } else {
            float vector[4];
            float speed = random_float_between_9768(10.0f, 15.0f) * frame_time;
            moby->collision = 0;
            moby->state = 3;
            data[2] = FUN_L00_00257c48(0.0f, 90.0f) * DEG_TO_RAD * frame_time;
            data[3] = FUN_L00_00257c48(0.0f, 90.0f) * DEG_TO_RAD * frame_time;
            subtract_vector_9768(data + 4, &moby->pos.x, D_0013F3D0);
            data[6] = 0.0f;
            scale_vector_9768(data + 4, data + 4, speed);
            vector_from_angles_9768(vector, speed, speed);
            add_vector_9768(data + 4, data + 4, vector);
            data[6] = absolute_float_9768(data[6]);
            data[7] = moby->pos.z;
            return;
        }
        break;
    case STATE_BALLISTIC_FLIGHT: {
        float *pos = &moby->pos.x;
        moby->rot.x = fast_add_rotations_9768(moby->rot.x, data[2]);
        moby->rot.y = fast_add_rotations_9768(moby->rot.y, data[2]);
        data[6] -= D_0015EE70 * 20.0f;
        add_vector_9768(pos, pos, data + 4);
        if (data[7] - 16.0f > moby->pos.z ||
            hit_test_9768(1.0f, pos, 2, moby)) {
            cart_burst(1.5f, 0.0f, moby, pos, -1);
            remove_moby_9768(moby);
        }
        return;
    }
    default: return;
    }
    return;
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB260), where it is exact; names translated to the US level program. */

extern float approach_value_alt(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern float probe_ground_height_c(void *, int, float) __asm__("func_00213508");
extern void draw_electric_water_bolt_effect(char *) __asm__("FUN_L15_002d8710");
extern void FUN_L18_002ea1f8_u(struct Moby *) __asm__("FUN_L18_002ea1f8");
void spawn_explosion_burst(void *arg) __asm__("FUN_L18_002ea800");
void initialize_electric_bolt_effect_if_requested(struct Moby *moby) __asm__("FUN_L18_002eacd8");

void initialize_electric_bolt_effect_if_requested(struct Moby *m) __asm__("FUN_L18_002eacd8");
extern void update_electric_bolt_geometry_and_particles(void *) __asm__("FUN_L18_002eaea0");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
void FUN_L18_002ea598_c(char *moby) __asm__("FUN_L18_002ea598");
extern void spark_s(int, void *, float, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");

extern float D_L18_00161ED0 __attribute__((sda));
extern float D_L18_00161ED4 __attribute__((sda));
extern float D_L18_00161ED8 __attribute__((sda));
extern float D_L18_00161EE0 __attribute__((sda));
void update_moby_target_travel_and_electric_effects(struct Moby *moby) __asm__("FUN_L18_002e9e70");

void update_moby_target_travel_and_electric_effects(struct Moby *moby) {
    enum {
        STATE_TIMED_REMOVAL_WAIT = 0,
        STATE_TRAVEL_TO_TARGET = 1
    };
    float *moby_vars = (float *)moby->pvars;
    float updated_rotation_angle = fast_add_rotations_c(moby_vars[0xB], D_L18_00161ED0 * DEG_TO_RAD * frame_time);
    float second_rotation_angle_step = D_L18_00161ED4 * DEG_TO_RAD * frame_time;
    moby_vars[0xB] = updated_rotation_angle;
    moby_vars[0xC] = fast_add_rotations_c(moby_vars[0xC], second_rotation_angle_step);
    switch (MOBY(moby)->state) {
    case STATE_TIMED_REMOVAL_WAIT:
        if (fast_dec_timer((int *)(moby_vars + 10))) {
            mark_moby_for_removal_c2(moby);
            return;
        }
        break;
    case STATE_TRAVEL_TO_TARGET: {
        float target_displacement[4];
        float ground_position[4];
        float *position = &moby->pos.x;
        float *arc_endpoint_position = moby_vars + 4;
        float movement_step_length = D_L18_00161EE0 * frame_time;
        float target_distance;
        subtract_vector_xyz(target_displacement, moby_vars, position);
        target_distance = vector_length_xyz(target_displacement);
        FUN_L18_002ea598_c(moby);
        FUN_L00_00257d78(arc_endpoint_position, D_L18_00161ED8 * moby_vars[9], D_L18_00161ED8 * moby_vars[9]);
        add_vector_xyz(arc_endpoint_position, arc_endpoint_position, position);
        initialize_electric_bolt_effect_if_requested(moby);
        update_electric_bolt_geometry_and_particles(moby);
        enqueue_callback_list_1((void (*)(void))draw_electric_water_bolt_effect, moby);
        if (target_distance < movement_step_length) {
            spawn_explosion_burst(moby);
            mark_moby_for_removal_c2(moby);
            return;
        }
        normalize_vector_xyz(target_displacement, target_displacement, movement_step_length);
        add_vector_xyz(position, position, target_displacement);
        qcopy(ground_position, position);
        ground_position[2] = probe_ground_height_c(ground_position, 0, 0.5f);
        spark_s(*(int *)(moby_vars + 8), position, 2.0f, 0x10001, 5.0f, 1.0f, 0, 1, 0);
        spark_s(*(int *)(moby_vars + 8), ground_position, 2.0f, 0x10001, 5.0f, 1.0f, 0, 1, 0);
        if (fast_dec_timer((int *)(moby_vars + 13))) {
            approach_value_alt(moby_vars + 9, 0.0f, 0.2f);
            if (moby_vars[9] == 0.0f) {
                mark_moby_for_removal_c2(moby);
                return;
            }
        }
        break;
    }
    }
    enqueue_callback_list_1(FUN_L18_002ea1f8_u, moby);
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB5E8), where it is exact; names translated to the US level program. */

typedef struct {
    float vtx[4][4];
    int color[4];
    float uv[4][2];
    long a;
    long b;
    long c;
    long d;
} Pkt;

extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
typedef struct {
    char pad[0x140];
    float position[4];
    char pad150[0x10];
    float shake_amplitude;
    float shake_offset;
    int shake_frames;
} L18FlareView;
extern L18FlareView D_L18_00167680 __attribute__((section(".data")));
extern float D_L18_001D9B50[][2];
extern float D_L18_001D9B70[][4][4];
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern s32 fast_tween_color_c(s32, s32, f32) __asm__("func_001FA6E0");
extern s64 get_effect_texture_c(s32) __asm__("func_001F44B8");
extern int D_L18_00161EE4 __attribute__((sda));
extern int D_L18_00161EE8 __attribute__((sda));
extern int D_L18_00161EEC __attribute__((sda));
extern int D_L18_00161EF0 __attribute__((sda));
extern int D_L18_00161EF8 __attribute__((sda));
extern int D_L18_00161F08 __attribute__((sda));
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void fast_draw_quad_real(void *, void *, int);
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");

void FUN_L18_002ea1f8(char *moby) {
    Pkt pkt[4];
    float rot[4];
    float m[4][4][4];
    char *data = *(char **)(moby + 0x78);

    clear_u64_value_c(rot);
    rot[2] = fast_arc_tan_c(D_L18_00167680.position[0] - *(float *)(moby + 0x10), D_L18_00167680.position[1] - *(float *)(moby + 0x14));
    rot[1] = -fast_arc_tan_c(vector_distance_xy_alt(moby + 0x10, &D_L18_00167680.position[0]), D_L18_00167680.position[2] - *(float *)(moby + 0x18));
    euler_rotation_matrix(m[0][0], rot);
    euler_rotation_matrix(m[1][0], rot);
    fast_vec_scale_c(m[1][1], m[1][1], *(float *)(data + 0x24));
    fast_vec_scale_c(m[1][2], m[1][2], *(float *)(data + 0x24));
    qcopy(m[0][3], moby + 0x10);
    qcopy(m[1][3], moby + 0x10);
    rot[0] = *(float *)(data + 0x2C);
    euler_rotation_matrix(m[2][0], rot);
    fast_vec_scale_c(m[2][1], m[2][1], *(float *)(data + 0x24));
    fast_vec_scale_c(m[2][2], m[2][2], *(float *)(data + 0x24));
    qcopy(m[2][3], moby + 0x10);
    rot[0] = *(float *)(data + 0x30);
    euler_rotation_matrix(m[3][0], rot);
    fast_vec_scale_c(m[3][1], m[3][1], *(float *)(data + 0x24));
    fast_vec_scale_c(m[3][2], m[3][2], *(float *)(data + 0x24));
    qcopy(m[3][3], moby + 0x10);
    {
    int i;
    int j;
    Pkt *p;
    for (i = 0; i < 4; i++) {
        {
            long r = ((long (*)(int))get_effect_texture_c)(((int *)&(*(short *)&D_L18_00161F08))[i]);
            p = &pkt[i];
            p->b = r;
        }
        p->c = 0xFF9000000260;
        p->d = (D_L18_00161EE4) | (long)(D_L18_00161EE8) << 2 | (long)(D_L18_00161EEC) << 4 |
                   (long)(D_L18_00161EF0) << 6 | 0x8000L << 24;
        p->a = 0;
        for (j = 0; j < 4; j++) {
            pkt[i].uv[j][0] = D_L18_001D9B50[j][0];
            pkt[i].uv[j][1] = D_L18_001D9B50[j][1];
            if (i == 0) {
                pkt[0].color[j] = fast_tween_color_c(D_L18_00161EF8 & 0xFFFFFF, D_L18_00161EF8, *(float *)(data + 0x24));
            } else {
                pkt[i].color[j] = ((int *)&(*(short *)&D_L18_00161EF8))[i];
            }
            qcopy(pkt[i].vtx[j], D_L18_001D9B70[i][j]);
        }
    }
    }
    fast_draw_quad_real(&pkt[0], m[0], 0);
    fast_draw_quad_real(&pkt[1], m[1], 0);
    fast_draw_quad_real(&pkt[2], m[2], 0);
    fast_draw_quad_real(&pkt[3], m[3], 0);
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EB988), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void FUN_L00_00257d78(float *, float, float);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern short D_L18_00161F1C __attribute__((sda));
extern short D_L18_00161F20 __attribute__((sda));
extern short D_L18_00161F24 __attribute__((sda));
extern short D_L18_00161F28 __attribute__((sda));
extern short D_L18_00161F2C __attribute__((sda));
extern short D_L18_00161F30 __attribute__((sda));
extern short D_L18_00161F34 __attribute__((sda));
extern short D_L18_00161F38 __attribute__((sda));
extern short D_L18_00161F3C __attribute__((sda));
extern short D_L18_00161F40 __attribute__((sda));
extern short D_L18_00161F44 __attribute__((sda));
extern short D_L18_00161F48 __attribute__((sda));
extern short D_L18_00161F4C __attribute__((sda));
extern short D_L18_00161F50 __attribute__((sda));
extern short D_L18_00161F54 __attribute__((sda));
extern short D_L18_00161F58 __attribute__((sda));
extern short D_L18_00161F5C __attribute__((sda));
extern short D_L18_00161F60 __attribute__((sda));
extern short D_L18_00161F64 __attribute__((sda));
extern short D_L18_00161F68 __attribute__((sda));
void FUN_L18_002ea598(struct Moby *moby) {
    float spark_spawn_position[4];
    float first_spark_vector[4];
    float second_spark_vector[4];
    float scaled_target_direction[4];
    int i;
    int initial_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count, third_spark_frame_count;
    float speed_min, speed_max, sink_min, sink_max;
    fast_vec_sub(scaled_target_direction, (void *)moby->pvars, &moby->pos);
    fast_vec_normalize(scaled_target_direction, scaled_target_direction, frame_time * 60.0f);
    speed_min = (*(float *)&(D_L18_00161F54)) * frame_time;
    speed_max = (*(float *)&(D_L18_00161F58)) * frame_time;
    sink_min = (*(float *)&(D_L18_00161F60)) * frame_time;
    sink_max = (*(float *)&(D_L18_00161F64)) * frame_time;
    for (i = 0; i < 3; i++) {
        FUN_L00_00257d78(first_spark_vector, speed_min, speed_max);
        fast_vec_scale(second_spark_vector, first_spark_vector, (*(float *)&(D_L18_00161F5C)));
        second_spark_vector[2] -= random_float_between(sink_min, sink_max);
        spark_frame_count = func_001FA898_r(FUN_L04_001f1978(random_float_between(
            (*(float *)&(D_L18_00161F3C)), (*(float *)&(D_L18_00161F40)))));
        second_spark_frame_count = func_001FA898_r(FUN_L04_001f1978(random_float_between(
            (*(float *)&(D_L18_00161F44)), (*(float *)&(D_L18_00161F48)))));
        third_spark_frame_count = func_001FA898_r(FUN_L04_001f1978(random_float_between(
            (*(float *)&(D_L18_00161F4C)), (*(float *)&(D_L18_00161F50)))));
        first_spark_vector[3] = random_float_between((*(float *)&(D_L18_00161F2C)),
                                          (*(float *)&(D_L18_00161F30)));
        second_spark_vector[3] = random_float_between((*(float *)&(D_L18_00161F34)),
                                          (*(float *)&(D_L18_00161F38)));
        fast_vec_scale(spark_spawn_position, scaled_target_direction, random_float_between(0.0f, 1.0f));
        fast_vec_add(spark_spawn_position, spark_spawn_position, &moby->pos);
        initial_spark_color = FUN_001fa6e0((*(int *)&(D_L18_00161F1C)), (*(int *)&(D_L18_00161F20)),
                          random_float_between(0.0f, 1.0f));
        second_spark_color = FUN_001fa6e0((*(int *)&(D_L18_00161F24)), (*(int *)&(D_L18_00161F28)),
                          random_float_between(0.0f, 1.0f));
        spawn_spark(spark_spawn_position, first_spark_vector, second_spark_vector, initial_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count, third_spark_frame_count, (*(int *)&(D_L18_00161F68)));
    }
}
extern void FUN_L06_0030bef8(void *, int, int, int);
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern char *spawn_glow_particle(void *, void *, void *, float, int, unsigned char, unsigned char, unsigned char, unsigned char) __asm__("FUN_L00_002ac910");
extern int D_L18_00161F6C __attribute__((sda));
extern int D_L18_00161F78 __attribute__((sda));
extern int D_L18_00161F7C __attribute__((sda));
extern int D_L18_00161F80 __attribute__((sda));
extern int D_L18_00161F84 __attribute__((sda));
extern int D_L18_00161F88 __attribute__((sda));
extern int D_L18_00161F98 __attribute__((sda));
extern int D_L18_00161FAC __attribute__((sda));
extern int D_L18_00161FB0 __attribute__((sda));
extern int D_L18_00161FB4 __attribute__((sda));
extern int D_L18_00161FB8 __attribute__((sda));
extern int D_L18_00161FC4 __attribute__((sda));
extern int D_L18_00161FD0 __attribute__((sda));
extern int D_L18_00161FDC __attribute__((sda));
extern int D_L18_00161FE8 __attribute__((sda));
extern float D_L18_00161F70 __attribute__((sda));
extern float D_L18_00161F74 __attribute__((sda));
extern float D_L18_00161F8C __attribute__((sda));
extern float D_L18_00161F90 __attribute__((sda));
extern float D_L18_00161F94 __attribute__((sda));
extern float D_L18_00161FA0 __attribute__((sda));
extern float D_L18_00161FA4 __attribute__((sda));
extern float D_L18_00161FA8 __attribute__((sda));
extern float D_L18_00161FBC __attribute__((sda));
extern float D_L18_00161FC0 __attribute__((sda));
extern float D_L18_00161FCC __attribute__((sda));
extern float D_L18_00161FD8 __attribute__((sda));
extern float D_L18_00161FE4 __attribute__((sda));
extern int D_L18_001D9C70[];
extern int D_L18_001D9C88[];
extern unsigned char D_L18_00161FC8 __attribute__((sda));
extern unsigned char D_L18_00161FD4 __attribute__((sda));
extern unsigned char D_L18_00161FE0 __attribute__((sda));
extern unsigned char D_L18_00161FC9[3] __attribute__((section(".sdata")));
extern unsigned char D_L18_00161FD5[3] __attribute__((section(".sdata")));
extern unsigned char D_L18_00161FE1[3] __attribute__((section(".sdata")));

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EBBF0). */
void spawn_explosion_burst(void *arg) __asm__("FUN_L18_002ea800");
void spawn_explosion_burst(void *source_moby) {
    float velocity[4] __attribute__((aligned(16)));
    float offset[4] __attribute__((aligned(16)));
    struct Moby *moby = source_moby;
    int i = 0;
    int j;
    L18FlareView *camera;
    *(Q128 *)velocity = *(Q128 *)&moby->pos;
    FUN_L06_0030bef8(velocity, scale_ticks(30), 0x80806060, 0x801010);
    trooper_play_sound(0x10, 0, (int)source_moby, 0x58E);
    if (i < D_L18_00161F6C) {
        int a, b, third_spark_frame_count;
        float angle, elevation, scale, speed;
        do {
            ++i;
            elevation = random_float_between(-0.34906584f, 0.34906584f);
            angle = random_angle_radians();
            speed = random_float_between(D_L18_00161F70, D_L18_00161F74);
            build_spherical_offset(velocity, speed * frame_time, angle, elevation);
            scale_vector_xyz(offset, velocity, D_L18_00161F94);
            scale = random_float_between(0.8f, 1.2f);
            offset[3] = velocity[3] = D_L18_00161F8C;
            offset[2] += D_L18_00161F90 * scale * frame_time;
            scale = random_float_between(0.8f, 1.2f);
            a = truncate_float_to_s32((float)scale_ticks(D_L18_00161F80) * scale);
            b = truncate_float_to_s32((float)scale_ticks(D_L18_00161F84) * scale);
            third_spark_frame_count = truncate_float_to_s32((float)scale_ticks(D_L18_00161F88) * scale);
            spawn_spark(&moby->pos, velocity, offset, D_L18_00161F78, D_L18_00161F7C, a, b, third_spark_frame_count, *(int *)&D_L18_00161F68);
        } while (i < D_L18_00161F6C);
    }
    j = 0;
    if (D_L18_00161F98 > 0) {
        int a, b, color1, color2;
        float offset_x, offset_y;
        float angle, elevation, scale, speed;
        do {
            ++j;
            speed = random_float_between(D_L18_00161FA0, D_L18_00161FA4) * frame_time;
            elevation = random_float_between(1.0471976f, 1.3962634f);
            angle = random_angle_radians();
            build_spherical_offset(velocity, speed, angle, elevation);
            angle = random_angle_radians();
            offset_x = fast_cos(angle);
            offset[0] = random_float_between(0.0f, D_L18_00161FBC);
            offset_x *= offset[0];
            offset[0] = offset_x;
            offset_y = fast_sin(angle);
            offset[1] = random_float_between(0.0f, D_L18_00161FBC);
            offset_y *= offset[1];
            offset[1] = offset_y;
            offset[2] = 0.0f;
            add_vector_xyz(offset, offset, &moby->pos);
            color1 = D_L18_001D9C70[random_integer_below(6)];
            color2 = D_L18_001D9C88[random_integer_below(6)];
            scale = D_L18_00161FA8 * 400000.0f;
            a = scale_ticks(random_int_between(D_L18_00161FAC, D_L18_00161FB0));
            b = scale_ticks(random_int_between(D_L18_00161FB4, D_L18_00161FB8));
            spawn_particle(offset, velocity, color1, color2, scale, a, b, speed, 0, 0);
        } while (j < D_L18_00161F98);
    }
    if (D_L18_00161FC0 > 0.0f) {
        void *pos = &moby->pos;
        int duration = scale_ticks(D_L18_00161FC4);
        spawn_glow_particle(source_moby, pos, &D_L18_0015F580, D_L18_00161FC0, duration, D_L18_00161FC8, D_L18_00161FC9[0], D_L18_00161FC9[1], D_L18_00161FC9[2]);
    }
    if (D_L18_00161FCC > 0.0f) {
        void *pos = &moby->pos;
        int duration = scale_ticks(D_L18_00161FD0);
        spawn_glow_particle(source_moby, pos, &D_L18_0015F580, D_L18_00161FCC, duration, D_L18_00161FD4, D_L18_00161FD5[0], D_L18_00161FD5[1], D_L18_00161FD5[2]);
    }
    if (D_L18_00161FD8 > 0.0f) {
        void *pos = &moby->pos;
        int duration = scale_ticks(D_L18_00161FDC);
        spawn_glow_particle(source_moby, pos, &D_L18_0015F580, D_L18_00161FD8, duration, D_L18_00161FE0, D_L18_00161FE1[0], D_L18_00161FE1[1], D_L18_00161FE1[2]);
    }
    if (D_L18_00161FE4 > 0.0f) {
        camera = &D_L18_00167680;
        camera->shake_amplitude = D_L18_00161FE4;
        camera->shake_frames = scale_ticks(D_L18_00161FE8);
    }
}
/* Starts the moby's trail when requested: resets the trail points and the counters. */
extern int D_L18_00162008_q __asm__("D_L18_00162008") __attribute__((sda));
extern int D_L18_00162010_q __asm__("D_L18_00162010") __attribute__((sda));
extern int D_L18_00162024_q __asm__("D_L18_00162024") __attribute__((sda));
extern int D_L18_00162054_q __asm__("D_L18_00162054");
extern short D_L18_001620C8_q __asm__("D_L18_001620C8");
extern short D_L18_001620D0_q __asm__("D_L18_001620D0");
extern short D_L18_001620B8_b[4] __asm__("D_L18_001620B8");
extern float D_L18_001D9CF0_q[][4] __asm__("D_L18_001D9CF0");
extern float D_L18_001D9D00_q[][4] __asm__("D_L18_001D9D00");
extern int D_L18_001D9F80_q[] __asm__("D_L18_001D9F80");
extern float D_L18_001D9FD0_q[][4] __asm__("D_L18_001D9FD0");
extern int D_L18_001DA110_q[] __asm__("D_L18_001DA110");
extern int scale_frames_q(int) __asm__("FUN_001f96f8");
extern void norm_q(void *, void *, float) __asm__("FUN_001f9bf8");

extern int D_L18_00161FEC __attribute__((sda));
extern float D_L18_00161FF0 __attribute__((sda));
extern int D_L18_00162080 __attribute__((sda));
extern short D_L18_001620CA;
extern short D_L18_001620CC;
extern short D_L18_001620CE;
extern short D_L18_001620D2;
extern short D_L18_001620D4;
extern short D_L18_001620D6;
extern int D_L18_001620D8;
extern int D_L18_001620DC;
void initialize_electric_bolt_effect_if_requested(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float arc_endpoint_displacement[4];
    float step[4];
    int i;
    int j;
    if (D_L18_00161FEC != 0) {
        D_L18_00161FEC = 0;
        D_L18_001620DC = 0;
        D_L18_001620D8 = scale_frames_q(D_L18_00162080);
        D_L18_00162054_q = 0x7F2020;
        subtract_vector_xyz(arc_endpoint_displacement, moby_vars + 0x10, &moby->pos);
        norm_q(step, arc_endpoint_displacement, D_L18_00161FF0 / 20.0f);
        qcopy(D_L18_001D9D00_q, &moby->pos);
        for (i = 1; i < 20; i++) {
            add_vector_xyz(D_L18_001D9D00_q[i], D_L18_001D9CF0_q[i], step);
            D_L18_001D9F80_q[i] = 0;
            clear_u64_value(D_L18_001D9FD0_q[i]);
            D_L18_001DA110_q[i] = 0;
        }
        for (j = 0; j < 4; j++)
            D_L18_001620B8_b[j] = -1;
        D_L18_001620C8_q = 8;
        D_L18_001620D0_q = 4;
        D_L18_001620CA = 4;
        D_L18_001620D2 = 2;
        D_L18_001620D4 = 4;
        D_L18_001620CC = 8;
        D_L18_001620CE = 4;
        D_L18_001620D6 = 2;
        D_L18_00162008_q = 0;
        D_L18_00162010_q = 0;
        D_L18_00162024_q = 0;
    }
}
/* Veldin 2 arc emitter: rebuilds the 20-point bolt and its mirror, regrows or jitters four side arcs, then spawns three particles. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002EC290), where it is exact; names translated to the US level program. */

typedef struct { float f[4]; } __attribute__((aligned(16))) V_ec290;

typedef struct { V_ec290 p[5]; } P_ec290;

typedef struct { V_ec290 hdr; V_ec290 p[4]; } H_ec290;

extern H_ec290 D_L18_001DA150[];
extern P_ec290 D_L18_001DA160[];
extern V_ec290 D_L18_001D9CF0[];
extern V_ec290 D_L18_001D9D00[2][20];
extern V_ec290 D_L18_001D9FD0[];
extern char D_0013F5E0[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_L18_001677C0_c[] __asm__("D_L18_001677C0");
extern float D_L18_001D9F80[];
extern float D_L18_001DA110[];
extern float FUN_L00_00257c48(float lo, float hi);
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern int D_L18_0015F580; /* no foreign declaration */
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern float D_L18_00161FFC __attribute__((sda));
extern float D_L18_00162000 __attribute__((sda));
extern float D_L18_00162004 __attribute__((sda));
extern s32 D_L18_00162008 __attribute__((sda));
extern float D_L18_0016200C __attribute__((sda));
extern s32 D_L18_00162010 __attribute__((sda));
extern float D_L18_00162014 __attribute__((sda));
extern float D_L18_00162018 __attribute__((sda));
extern float D_L18_0016201C __attribute__((sda));
extern float D_L18_00162020 __attribute__((sda));
extern s32 D_L18_00162024 __attribute__((sda));
extern float D_L18_00162028 __attribute__((sda));
extern float D_L18_0016202C __attribute__((sda));
extern float D_L18_00162040 __attribute__((sda));
extern float D_L18_00162044 __attribute__((sda));
extern s32 D_L18_00162054_c __asm__("D_L18_00162054") __attribute__((sda));
extern float D_L18_00162084 __attribute__((sda));
extern float D_L18_0016208C __attribute__((sda));
extern short D_L18_001620B8[] __attribute__((section(".sdata")));
extern short D_L18_001620C0[] __attribute__((section(".sdata")));
extern short D_L18_001620C8[] __attribute__((section(".sdata")));
extern short D_L18_001620D0[] __attribute__((section(".sdata")));
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
unsigned char *FUN_L00_00272060(void *pos, int s, int color, int mode, int b, float *v, float x, float y, float z);
extern void spawn_ec290(void *, int, float, float, int, int, int, void *, float) __asm__("FUN_L00_00272060");

extern f32 frame_scale_macro __asm__("D_0015ED60") __attribute__((section(".sdata")));
void update_electric_bolt_geometry_and_particles(void *moby) {
    V_ec290 scratch_vectors[10];
    V_ec290 side_arc_segment_offsets[5];
    unsigned char flag = *(unsigned char *)D_0013E533;
    char *data = *(char **)((char *)moby + 0x78);
    float scale;
    int i;
    int k;
    float rotation_angle_or_particle_scale;
    int r;

    *(float *)&D_L18_00162008 = fast_add_rotations(*(float *)&D_L18_00162008, *(float *)&D_L18_0016200C);
    *(float *)&D_L18_00162010 = fast_subtract_rotations(*(float *)&D_L18_00162010, *(float *)&D_L18_00162014);
    *(float *)&D_L18_00162024 = fast_add_rotations(*(float *)&D_L18_00162024, *(float *)&D_L18_00162028);
    scale = distance_xyz((char *)moby + 0x10, data + 0x10) * 0.05f;
    if (flag) {
        *(int *)&D_L18_00162054_c = 0x7F2040;
    } else {
        *(int *)&D_L18_00162054_c = 0x7F2020;
    }
    subtract_vector_xyz(&scratch_vectors[0], data + 0x10, (char *)moby + 0x10);
    normalize_vector_xyz(&scratch_vectors[0], &scratch_vectors[0], 1.0f);
    cross_vectors_xyz(&scratch_vectors[1], &scratch_vectors[0], D_0013F5E0);
    build_look_at_matrix(&scratch_vectors[0], &scratch_vectors[0], &scratch_vectors[1], 0.17453292f);
    scale_vector_xyz_c(&scratch_vectors[3], &scratch_vectors[0], scale);
    qcopy(D_L18_001D9D00[0], (char *)moby + 0x10);
    qcopy(D_L18_001D9D00[1], (char *)moby + 0x10);
    for (i = 1; i < 20; i++) {
        float primary_bolt_z_offset;
        float arc_endpoint_distance;
        float blended_step_length;
        float azimuth_angle;
        float b;
        primary_bolt_z_offset = *(float *)&D_L18_00162000
              + (*(float *)&D_L18_00162004 - *(float *)&D_L18_00162000) * (convert_integer_to_float(i) * 0.05f);
        primary_bolt_z_offset = primary_bolt_z_offset * fast_sin_c(normalize_angle(*(float *)&D_L18_00162008 + (float)i * *(float *)&D_L18_00161FFC));
        primary_bolt_z_offset = primary_bolt_z_offset + FUN_L00_00257c48(*(float *)&D_L18_00162040, *(float *)&D_L18_00162044);
        D_L18_001D9D00[0][i].f[2] = D_L18_001D9D00[0][i].f[2] - D_L18_001D9F80[i];
        D_L18_001D9F80[i] = primary_bolt_z_offset;
        azimuth_angle = random_angle_radians_c();
        b = random_angle_radians_c();
        build_spherical_offset(&scratch_vectors[1], scale, azimuth_angle, b);
        scratch_vectors[1].f[2] = scratch_vectors[1].f[2] * 0.5f;
        lerp_vector(&scratch_vectors[2], &scratch_vectors[3], &scratch_vectors[1], *(float *)&D_L18_0016208C);
        blended_step_length = vector_length_xyz(&scratch_vectors[2]);
        if (blended_step_length == 0.0f) {
            qcopy(&D_L18_001D9D00[0][i], &D_L18_001D9D00[0][i - 1]);
        } else {
            scale_vector_xyz_c(&scratch_vectors[2], &scratch_vectors[2], scale / blended_step_length);
            add_vector_xyz(&D_L18_001D9D00[0][i], &D_L18_001D9CF0[i], &scratch_vectors[2]);
            qcopy(&scratch_vectors[3], &scratch_vectors[2]);
        }
        D_L18_001D9D00[0][i].f[2] = D_L18_001D9D00[0][i].f[2] + primary_bolt_z_offset;
        subtract_vector_xyz(&scratch_vectors[1], &D_L18_001D9D00[0][i], &D_L18_001D9CF0[i]);
        subtract_vector_xyz(&scratch_vectors[5], data + 0x10, &D_L18_001D9D00[0][i]);
        arc_endpoint_distance = vector_length_xyz(&scratch_vectors[5]);
        if (arc_endpoint_distance != 0.0f) {
            float endpoint_direction_blend_weight;
            if (i == 19) {
                endpoint_direction_blend_weight = 0.5f;
                scale = arc_endpoint_distance;
            } else if (i >= 16) {
                endpoint_direction_blend_weight = 0.5f;
                scale = arc_endpoint_distance / convert_integer_to_float(19 - i);
            } else {
                endpoint_direction_blend_weight = 0.1f;
            }
            scale_vector_xyz_c(&scratch_vectors[5], &scratch_vectors[5], scale / arc_endpoint_distance);
            lerp_vector(&scratch_vectors[3], &scratch_vectors[3], &scratch_vectors[5], endpoint_direction_blend_weight);
        }
        if (i < 10) {
            float g;
            float secondary_bolt_z_offset;
            float secondary_bolt_lateral_angle;
            float secondary_bolt_lateral_offset;
            g = normalize_angle(*(float *)&D_L18_00162010 + (float)i * *(float *)&D_L18_00162018);
            secondary_bolt_z_offset = *(float *)&D_L18_0016201C
                * fast_sin_c(normalize_angle(*(float *)&D_L18_00162024 + (float)i * *(float *)&D_L18_0016202C));
            qcopy(&D_L18_001D9D00[1][i], &D_L18_001D9D00[0][i]);
            secondary_bolt_z_offset = secondary_bolt_z_offset * fast_sin_c(g) + FUN_L00_00257c48(0.0f, 0.1f);
            D_L18_001D9D00[1][i].f[2] = D_L18_001D9D00[1][i].f[2] + secondary_bolt_z_offset;
            D_L18_001DA110[i] = secondary_bolt_z_offset;
            secondary_bolt_lateral_angle = fast_add_rotations(fast_arc_tan(scratch_vectors[1].f[0], scratch_vectors[1].f[1]), 1.5707964f);
            secondary_bolt_lateral_offset = *(float *)&D_L18_00162020 * fast_cos_c(g);
            scratch_vectors[4].f[0] = fast_cos_c(secondary_bolt_lateral_angle) * secondary_bolt_lateral_offset;
            scratch_vectors[4].f[1] = fast_sin_c(secondary_bolt_lateral_angle) * secondary_bolt_lateral_offset;
            scratch_vectors[4].f[2] = 0.0f;
            qcopy(&D_L18_001D9FD0[i], &scratch_vectors[4]);
            add_vector_xyz(&D_L18_001D9D00[1][i], &D_L18_001D9D00[1][i], &scratch_vectors[4]);
        } else {
            int r = 21 - i;
            qcopy(&D_L18_001D9D00[1][i], &D_L18_001D9D00[0][i]);
            D_L18_001D9D00[1][i].f[2] = D_L18_001D9D00[1][i].f[2] + D_L18_001DA110[r];
            add_vector_xyz(&D_L18_001D9D00[1][i], &D_L18_001D9D00[1][i], &D_L18_001D9FD0[r]);
            qcopy(&scratch_vectors[4], &D_L18_001D9FD0[i]);
        }
    }
    cross_vectors_xyz(&scratch_vectors[6], &scratch_vectors[0], D_0013F5E0);
    normalize_vector_xyz(&scratch_vectors[6], &scratch_vectors[6], 1.0f);
    normalize_vector_xyz(&scratch_vectors[7], D_0013F5E0, 1.0f);
    for (k = 0; k < 4; k++) {
        float side_arc_scale;
        if (k < 2) {
            qcopy(&scratch_vectors[8], (char *)moby + 0x10);
            side_arc_scale = 0.4f;
        } else {
            qcopy(&scratch_vectors[8], data + 0x10);
            side_arc_scale = 0.5f;
        }
        if (fast_dec_timer_s16(D_L18_001620B8 + k)) {
            int j;
            D_L18_001620B8[k] = scale_game_frames_alt(D_L18_001620C8[k]);
            D_L18_001620C0[k] = 0;
            scale = FUN_L00_00257c48(0.52359879f, 1.04719758f);
            rotation_angle_or_particle_scale = random_float_between_c(-1.04719758f, 0.17453292f);
            scale_vector_xyz_c(&scratch_vectors[5], &scratch_vectors[0], side_arc_scale);
            build_look_at_matrix(&scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[6], rotation_angle_or_particle_scale);
            build_look_at_matrix(&scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[7], scale);
            qcopy(&D_L18_001DA160[k].p[0], &scratch_vectors[8]);
            add_vector_xyz(&D_L18_001DA160[k].p[1], &scratch_vectors[8], &scratch_vectors[5]);
            side_arc_scale = 1.0f;
            for (j = 2; j < 5; j++) {
                subtract_vector_xyz(&scratch_vectors[9], &D_L18_001DA150[k].p[j - 1], D_L18_001677C0_c);
                rotation_angle_or_particle_scale = random_float_between_c(0.17453292f, 0.959931076f) * side_arc_scale;
                side_arc_scale = -side_arc_scale;
                build_look_at_matrix(&scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[9], rotation_angle_or_particle_scale);
                add_vector_xyz((D_L18_001DA160[k].p + j), &D_L18_001DA150[k].p[j - 1], &scratch_vectors[5]);
            }
        } else {
            int j;
            for (j = 1; j < 5; j++) {
                subtract_vector_xyz(&side_arc_segment_offsets[j], (D_L18_001DA160[k].p + j), &D_L18_001DA150[k].p[j - 1]);
                side_arc_segment_offsets[j].f[0] += FUN_L00_00257c48(0.0f, 0.1f);
                side_arc_segment_offsets[j].f[1] += FUN_L00_00257c48(0.0f, 0.1f);
                side_arc_segment_offsets[j].f[2] += FUN_L00_00257c48(0.0f, 0.1f);
            }
            qcopy(&D_L18_001DA160[k].p[0], &scratch_vectors[8]);
            for (j = 1; j < 5; j++) {
                add_vector_xyz((D_L18_001DA160[k].p + j), &D_L18_001DA150[k].p[j - 1], &side_arc_segment_offsets[j]);
            }
            {
                int x = D_L18_001620D0[k];
                int y = D_L18_001620B8[k];
                float fr;
                if (x < y) {
                    x = y - x;
                } else {
                    x = x - y;
                }
                fr = convert_integer_to_float(x);
                fr = fr / convert_integer_to_float(scale_game_frames_alt(15));
                D_L18_001620C0[k] = func_001FA898_r((1.0f - fr) * 32.0f);
            }
        }
    }
    scale = FUN_L00_00257c48(0.52359879f, 1.04719758f);
    rotation_angle_or_particle_scale = random_float_between_c(-0.52359879f, 1.04719758f);
    scale_vector_xyz_c(&scratch_vectors[5], &scratch_vectors[0], *(float *)&D_L18_00162084 * frame_scale_macro);
    build_look_at_matrix(&scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[6], rotation_angle_or_particle_scale);
    build_look_at_matrix(&scratch_vectors[5], &scratch_vectors[5], &scratch_vectors[7], scale);
    r = random_int_between(1, 8);
    if (random_integer_below(2)) r = -r;
    rotation_angle_or_particle_scale = random_float_between_c(0.2f, 0.6f);
    spawn_ec290((char *)moby + 0x10, scale_game_frames_alt(15), rotation_angle_or_particle_scale * 0.3f, rotation_angle_or_particle_scale, *(int *)&D_L18_00162054_c | 0x7F000000, 0, r, &scratch_vectors[5], 0.0f);
    r = -r;
    spawn_ec290((char *)moby + 0x10, scale_game_frames_alt(15), rotation_angle_or_particle_scale * 0.15f, rotation_angle_or_particle_scale * 0.5f, 0x307F7F7F, 0, r, &scratch_vectors[5], 0.0f);
    r = random_int_between(1, 8);
    if (random_integer_below(2)) r = -r;
    rotation_angle_or_particle_scale = random_float_between_c(0.15f, 3.0f);
    spawn_ec290(data + 0x10, scale_game_frames_alt(12), rotation_angle_or_particle_scale * 0.1f, rotation_angle_or_particle_scale, *(int *)&D_L18_00162054_c | 0x7F000000, 0, r, &D_L18_0015F580, 0.0f);
}
typedef struct {
    float f[4];
} __attribute__((aligned(16))) Vec130;
typedef struct {
    float u, v;
} UVec130;
typedef struct {
    Vec130 corner[4];
    unsigned int color[4];
    UVec130 uv[4];
    long unk70, tex, unk80, unk88;
} Qec130;
extern unsigned int D_L18_001620A0 __asm__("D_L18_001620A0") __attribute__((sda));
extern unsigned int D_L18_00162054 __asm__("D_L18_00162054") __attribute__((sda));
extern float D_L18_001620A8 __asm__("D_L18_001620A8") __attribute__((sda));
extern float D_L18_001620AC __asm__("D_L18_001620AC") __attribute__((sda));
extern Vec130 D_L18_001677C0 __asm__("D_L18_001677C0") __attribute__((section(".data")));
extern Vec130 D_L18_001D9CA0[] __asm__("D_L18_001D9CA0") __attribute__((section(".data")));
extern UVec130 D_L18_001D9CE0[] __asm__("D_L18_001D9CE0") __attribute__((section(".data")));
extern void vscl_ec130(void *, void *, float) __asm__("FUN_001f9bf8");
extern void vmul_ec130(float, void *, void *) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern long tex_ec130(int) __asm__("FUN_001f44b8");
extern float spread_k(float, float) __asm__("FUN_L00_00257c48");
extern void draw_ec130(void *, void *, int) __asm__("FUN_001f7d30");

void draw_moby_billboard_quad_pair(void *mv) __asm__("FUN_L18_002ec130");

void draw_moby_billboard_quad_pair(void *mv) {
    Qec130 quad;
    Vec130 quad_transform[4];
    Vec130 *pos;
    unsigned int c;
    float corner_scale;
    int i;
    pos = (Vec130 *)mv;
    quad.tex = tex_ec130(0xB);
    pos = (Vec130 *)((char *)pos + 0x10);
    quad.unk88 = 0x8000000048L;
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    subtract_vector_xyz(&quad_transform[3], &D_L18_001677C0, pos);
    vscl_ec130(&quad_transform[3], &quad_transform[3], 0.3f);
    add_vector_xyz(&quad_transform[3], &quad_transform[3], pos);
    quad_transform[3].f[3] = 1.0f;
    subtract_vector_xyz(&quad_transform[0], &D_L18_001677C0, &quad_transform[3]);
    vscl_ec130(&quad_transform[0], &quad_transform[0], 1.0f);
    cross_vectors_xyz(&quad_transform[1], &quad_transform[0], (Vec130 *)&hero.gravity_dir);
    vscl_ec130(&quad_transform[1], &quad_transform[1], -1.0f);
    cross_vectors_xyz(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    c = D_L18_001620A0;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    corner_scale = D_L18_001620A8 + spread_k(0.0f, 0.025f);
    for (i = 0; i < 4; i++) {
        vmul_ec130(corner_scale, &quad.corner[i], &D_L18_001D9CA0[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &quad_transform[0]);
        quad.uv[i].u = D_L18_001D9CE0[i].u;
        quad.uv[i].v = D_L18_001D9CE0[i].v;
    }
    draw_ec130(&quad, 0, 0);
    c = D_L18_00162054;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    corner_scale = D_L18_001620AC + spread_k(0.0f, 0.05f);
    for (i = 0; i < 4; i++) {
        vmul_ec130(corner_scale, &quad.corner[i], &D_L18_001D9CA0[i]);
        transform_vector(&quad.corner[i], &quad.corner[i], &quad_transform[0]);
        quad.uv[i].u = D_L18_001D9CE0[i].u;
        quad.uv[i].v = D_L18_001D9CE0[i].v;
    }
    draw_ec130(&quad, 0, 0);
}
