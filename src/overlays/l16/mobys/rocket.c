/* Level 16 moby classes: rocket, test_dummy_door, bomb, conveyor_belt, test_dummy, wall_button, test_dummy_on_hoverboard, door. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/spawn_moby_break_fragments.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/overlay/shared/math/interpolation/count_active_moby_group_members.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/mobys/manipulator_set_scale.h"

/* Check whether this vendor moby is in state six. */
/* Ported from rac1-decomp (src/overlays/l16/vendor_002A50F0.c: func_L16_002C5A08), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x20];
    unsigned char state;
} Level16VendorMoby;

int is_moby_in_state_6(Level16VendorMoby *moby) __asm__("FUN_L16_002c4710");

int is_moby_in_state_6(Level16VendorMoby *moby) {
    return moby->state == 6;
}

#include "qcopy.h"
#include "rnc/overlay/entities.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C75D0), where it is exact; names translated to the US level program. */

extern char *create_moby(int);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern char *create_moby(int) __asm__("FUN_0020c4f8");

unsigned char *spawn_thrown_bomb(int    owner,
                                 void  *spawn_position,
                                 float *initial_velocity,
                                 int    fuse_frames,
                                 float  gravity) __asm__("FUN_L16_002c6268");

unsigned char *spawn_thrown_bomb(

    int owner, void *spawn_position, float *initial_velocity, int fuse_frames, float gravity) {
    unsigned char *moby = (unsigned char *)create_moby(0x119);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        MOBY(moby)->draw_dist = 0xFF;
        *(int *)(data + 0x20) = owner;
        *(float *)(data + 0x24) = gravity;
        *(int *)(data + 0x28) = fuse_frames;
        *(short *)(data + 0xC) = scale_game_frames(5);
        *(short *)(data + 0xE) = scale_game_frames(0xF);
        qcopy(moby + 0x10, spawn_position);
        qcopy(data + 0x10, initial_velocity);
        MOBY(moby)->rot.x = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        MOBY(moby)->rot.z = fast_arc_tan(initial_velocity[0], initial_velocity[1]) + 1.5707964f;
        update_entity(moby);
    }
    return moby;
}
/* updates the moby: initialises in state 0, then runs its motion in state 1 */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CA848), where it is exact; names translated to the US level program. */

extern float D_0015ED6C;
void         initialize_moby_sprite_effect(struct Moby *moby) __asm__("FUN_L16_002c9568");

extern void initialize_moby_sprite_effect(struct Moby *) __asm__("FUN_L16_002c9568");
extern void FUN_L16_002c9650_u(char *) __asm__("FUN_L16_002c9650");
f32         probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");

void update_spinning_moby_sprite_effect(struct Moby *moby) __asm__("FUN_L16_002c9480");

void update_spinning_moby_sprite_effect(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_SPIN_AND_DRAW_SPRITES = 1
    };
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE: {
        float ground_height;
        float vertical_distance_to_ground;
        initialize_moby_sprite_effect((char *)moby);
        moby->scale = moby->scale * 0.9f;
        ground_height = probe_ground_height(&moby->pos, 0, 0.5f);
        vertical_distance_to_ground = absolute_float(ground_height - moby->pos.z);
        if (vertical_distance_to_ground < 1.0f) {
            moby->pos.z = ground_height + 0.9f;
        }
        moby->state = 1;
        break;
    }
    case MOBY_STATE_SPIN_AND_DRAW_SPRITES:
        moby->rot.z = fast_add_rotations(moby->rot.z, D_0015ED6C * 4.1887903f);
        FUN_L16_002c9650_u((char *)moby);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CA930), where it is exact; names translated to the US level program. */

s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

void initialize_moby_sprite_effect(struct Moby *moby) {
    char  *data = (char *)moby->pvars;
    float *random_value_cursor = (float *)data;
    int   *frame_count_cursor;
    int    i;
    int    unscaled_frame_count;
    for (i = 3; i >= 0; i--, random_value_cursor++) {
        *random_value_cursor = (float)random_integer_below(0xFF);
    }
    frame_count_cursor = (int *)(data + 0x20);
    *(float *)(data + 0x10) = -1.0f;
    *(float *)(data + 0x14) = -2.25f;
    *(float *)(data + 0x18) = 1.25f;
    *(float *)(data + 0x1C) = 2.5f;
    unscaled_frame_count = 0x3F;
    for (i = 3; i >= 0; i--, frame_count_cursor++) {
        *frame_count_cursor = scale_game_frames(unscaled_frame_count);
        unscaled_frame_count += 0x40;
    }
    *(float *)(data + 0x30) = 3.5f;
    *(float *)(data + 0x34) = 4.2f;
    *(float *)(data + 0x3C) = 3.5f;
    *(float *)(data + 0x38) = 4.2f;
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CB000), where it is exact; names translated to the US level program. */

extern char  *D_L16_0015FFD8;
extern int   *D_L16_001ABCC0_c[] __asm__("D_L16_001ABCC0");
extern short *D_L16_001ABFC0_2CB000[] __asm__("D_L16_001ABCC0");

void update_conveyor_group_motion_transforms(int index, void *conveyor_motion_vector) __asm__("FUN_L16_002c9c38");

void update_conveyor_group_motion_transforms(int index, void *conveyor_motion_vector) {
    short *group_entry = D_L16_001ABFC0_2CB000[index];
    if (group_entry) {
        do {
            char *moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
            if (MOBY(moby)->oclass == 0x1D7) {
                update_entity_matrix(((char *)MOBY(moby)->pvars) + 0x20,
                                     conveyor_motion_vector,
                                     moby + 0x40,
                                     moby + 0x40);
            }
        } while (*group_entry++ >= 0);
    }
}

extern int D_L16_001ABCC0_u[] __asm__("D_L16_001ABCC0");
extern int D_L16_0015FFD8_d __asm__("D_L16_0015FFD8");
/* 0x002ceca8, 116 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// For each listed moby of class 0x21D: sets its state bytes and clears bit 0 of its flags.
void activate_test_dummy_group(int moby_group_index) __asm__("FUN_L16_002ceca8");

void activate_test_dummy_group(int moby_group_index) {
    short *group_entry = (short *)D_L16_001ABCC0_u[moby_group_index];
    if (group_entry != 0) {
        do {
            char *moby;
        again:
            moby = D_L16_0015FFD8_d + ((*group_entry & 0x7FFF) << 8);
            if (MOBY(moby)->oclass != 0x21D)
                goto again;
            {
                int            two = 2;
                unsigned short flags = MOBY(moby)->flags;
                moby[0x31] = 1;
                flags &= 0xFFFE;
                moby[0x20] = two;
                MOBY(moby)->flags = flags;
            }
        } while (*group_entry++ >= 0);
    }
}

/* Updates a moby: waits for trigger, registers a draw callback and a sound handle. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0870), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int  FUN_L16_002cf5c8_u() __asm__("FUN_L16_002cf5c8");
int         is_linked_pair_effect_enabled(void *mv) __asm__("FUN_L16_002cf738");

extern void allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern void FUN_L16_002cf7a8_u(void) __asm__("FUN_L16_002cf7a8");
extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern void func_L16_002D0990_2D0870(void *) __asm__("FUN_L16_002cf5c8");
extern int  func_0022ED80_i(int, int, void *) __asm__("FUN_0022da68");

void update_linked_pair_effect(struct Moby *moby) __asm__("FUN_L16_002cf4a8");

void update_linked_pair_effect(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE_LINKED_PAIRS = 0,
        MOBY_STATE_UPDATE_DRAW_AND_SOUND = 1
    };
    int *data = (int *)moby->pvars;
    if ((moby->flags & 2) == 0) {
        unsigned char state = moby->state;
        switch (state) {
        case MOBY_STATE_INITIALIZE_LINKED_PAIRS:
            if (data[0] == 0) {
                func_L16_002D0990_2D0870(moby);
                moby->state = 1;
            }
            break;
        case MOBY_STATE_UPDATE_DRAW_AND_SOUND:
            if (is_linked_pair_effect_enabled(moby) != 0) {
                data[2] = state;
                add_draw_callback(FUN_L16_002cf7a8_u, moby);
                if (voice_is_playing(moby, data[3]) == 0) {
                    data[3] = func_0022ED80_i(0, 4, moby);
                }
            } else if (data[2] != 0) {
                int voice_slot_index = data[3];
                data[2] = 0;
                if (voice_slot_index != -1) {
                    char *voice_entry_base = D_0013E533 + 0x1D + voice_slot_index * 0x70;
                    if (*(char **)(voice_entry_base + 0x88) == moby &&
                        *(unsigned char *)(voice_entry_base + 0x74) != 0) {
                        release_voice_slot(voice_slot_index);
                    }
                }
                data[3] = -1;
            }
            break;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0990), where it is exact; names translated to the US level program. */

extern int FUN_L16_002cf678_u(char *) __asm__("FUN_L16_002cf678");

void initialize_linked_pair_group(struct Moby *moby) __asm__("FUN_L16_002cf5c8");

void initialize_linked_pair_group(struct Moby *moby) {
    short *group_entry = D_L16_001ABCC0_c[moby->group];
    if (group_entry != 0) {
        do {
            char *other = D_L16_0015FFD8 + (((unsigned short)*group_entry & 0x7FFF) << 8);
            if (*(short *)(other + 0xA6) == 0x228) {
                int *data;
                if (moby != other) {
                    *(unsigned short *)(other + 0x34) |= 2;
                }
                data = *(int **)(other + 0x78);
                *data = FUN_L16_002cf678_u(other);
            }
        } while (*group_entry++ >= 0);
    }
}

#include "qcopy.h"

/* Copy the selected 16-byte vendor vector. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0D98), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x2C0];
    char *vectors;
} Level16VendorVectorData_u;

typedef struct {
    char                       pad0[0x78];
    Level16VendorVectorData_u *data;
} Level16VendorVectorMoby_u;

void copy_hoverboard_dummy_path_node(Level16VendorVectorMoby_u *moby,
                                     int                        index,
                                     void                      *out) __asm__("FUN_L16_002cf9d0");

void copy_hoverboard_dummy_path_node(Level16VendorVectorMoby_u *moby, int index, void *out) {
    qcopy(out, moby->data->vectors + index * 16 + 16);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D1310), where it is exact; names translated to the US level program. */

extern int offset_path_index(char *, int, int, int) __asm__("FUN_L00_0025d7a0");

void prepare_hoverboard_dummy_path_jump(struct Moby *moby) __asm__("FUN_L16_002cff48");

void prepare_hoverboard_dummy_path_jump(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (*(float *)(data + 0x2C8) <= 0.0f) {
        int   path_node_index = *(short *)(data + 0x2F4);
        char *tab = *(char **)(data + 0x2C0);
        int   path_node_byte_offset = path_node_index * 16;
        if (*(float *)(tab + path_node_byte_offset + 0x1C) == 1.0f) {
            while (1) {
                char *t;
                if (*(float *)(tab + path_node_byte_offset + 0x1C) == 2.0f)
                    break;
                path_node_index = offset_path_index(tab, path_node_index, 1, 1);
                tab = *(char **)(data + 0x2C0);
                path_node_byte_offset = path_node_index * 16;
            }
            *(short *)(data + 0x2F0) = path_node_index;
            qcopy(data + 0x270, *(char **)(data + 0x2C0) + (short)path_node_index * 16 + 0x10);
            *(int *)(data + 0x2E4) = 1;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7080), where it is exact; names translated to the US level program. */

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern int D_L16_0015F5CC;
extern s32 SubtractIntegerWithClamp(s32);

#include "rnc/overlay/l16/mobys/rocket_globals.h"

extern float test_dummy_door_height_offset_gp __asm__("D_L16_00161A84") __attribute__((sda));
extern int   last_test_dummy_door_sound_request_frame_gp __asm__("D_L16_00161A88")
    __attribute__((sda));
void test_dummy_door_update(struct Moby *moby) __asm__("FUN_L16_002d5cb8");

void test_dummy_door_update(struct Moby *moby) {
    enum {
        TEST_DUMMY_DOOR_STATE_INITIALIZE = 0,
        TEST_DUMMY_DOOR_STATE_UPDATE_HEIGHT = 1
    };
    char *data = (char *)moby->pvars;
    switch (moby->state) {
    case TEST_DUMMY_DOOR_STATE_INITIALIZE:
        moby->state = 1;
        *(float *)data = moby->pos.z;
        break;
    case TEST_DUMMY_DOOR_STATE_UPDATE_HEIGHT: {
        int   timer_result = fast_dec_timer((int *)(data + 8));
        float target_height;
        if (timer_result != 0) {
            if (timer_result == 2) {
                if (SubtractIntegerWithClamp(last_test_dummy_door_sound_request_frame_gp -
                                             D_L16_0015F5CC) >= 4) {
                    last_test_dummy_door_sound_request_frame_gp = D_L16_0015F5CC;
                    allocate_voice_for_target_entry(1, 0, moby);
                }
            }
            target_height = *(float *)data;
        } else {
            target_height = *(float *)data + test_dummy_door_height_offset_gp;
        }
        advance_accelerated_scalar(target_height,
                                   frame_time_sq * 10.0f,
                                   frame_time_sq * 10.0f,
                                   D_0015ED6C * 10.0f,
                                   &moby->pos.z,
                                   (float *)(data + 4));
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7B08), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  FUN_L00_00257470(void *, int, int);
void         mark_moby_for_removal_u(struct Obj *obj) __asm__("FUN_0020c828");

void update_moby_hit_triggered_removal(struct Moby *moby) __asm__("FUN_L16_002d6740");

void update_moby_hit_triggered_removal(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_CHECK_HIT_FOR_REMOVAL = 1,
        MOBY_STATE_REMOVE_WITH_EFFECTS = 2
    };
    int   flag = 0;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case MOBY_STATE_CHECK_HIT_FOR_REMOVAL:
        if (hit_record != 0 && 0.0f < *(float *)(hit_record + 0x2C))
            flag = 1;
        if (flag)
            moby->state = 2;
        break;
    case MOBY_STATE_REMOVE_WITH_EFFECTS:
        allocate_voice_for_target_entry(0, 0, moby);
        FUN_L00_00257470(moby, 0, -1);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x74E, 1, 0x74E, 1, 0xB, 2);
        mark_moby_for_removal_u(moby);
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002DF780), where it is exact; names translated to the US level program. */

extern float challenge_layout_param __asm__("D_L16_00161C74");

void challenge_set_layout_param(float new_value) __asm__("FUN_L16_002de3b8");

void challenge_set_layout_param(float new_value) {
    challenge_layout_param = new_value;
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E2248), where it is exact; names translated to the US level program. */

void request_target_reposition(struct Moby *moby) __asm__("FUN_L16_002e0de0");

void request_target_reposition(struct Moby *moby) {
    int state = 1;
    if (moby->state == state) {
        allocate_voice_for_target_entry(0, 0, moby);
        state = 7;
        moby->state = state;
    }
}

#include "sda.h"
#include "rnc/overlay/quad.h"

/* Advance a homing spark, emit its trail, and test its collision or impact state. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002A50F0), where it is exact; names translated to the US level program. */

typedef union {
    OvlQuad quad;
    float   f[4];
} L16SparkVector;

typedef struct {
    float          direction[4];
    void          *owner;
    int            flags;
    unsigned char  kind, enabled;
    unsigned short cls;
    float          scale;
    int            active;
} L16SparkQuery;

typedef struct {
    char  pad[0x18];
    char *moby;
    int   count;
    float position[4];
} L16SparkHit;

extern char *D_L16_001B255C NOT_SDA;
extern char *
spawn_particle(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float D_0015ED6C, frame_time;
extern float frame_time_sq;
extern float
spring_angle(void *, void *, int, float, float, float, float) __asm__("FUN_L00_0025bc98");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int   D_L16_0015F580_spark __asm__("D_L16_0015F580"); /* no foreign declaration */
extern int   collision_line() __asm__("FUN_001efa68");
extern void  spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");
extern void func_L00_0025F4A8_alt(void *,
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
void        build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern L16SparkHit D_L16_001742C0_hit __asm__("D_L16_00174240");
extern float       func_L00_0025CCF0_spark(void *, void *, float, float, float, float, int) __asm__("FUN_L00_0025bc98");

void update_homing_projectile(unsigned char *moby) __asm__("FUN_L16_002a3f38");

void update_homing_projectile(unsigned char *moby) {
    enum {
        PROJECTILE_STATE_FLYING = 0,
        PROJECTILE_STATE_IMPACT = 1
    };
    L16SparkVector desired, previous, velocity, random;
    L16SparkQuery  query;
    char          *projectile_data = ((char *)MOBY(moby)->pvars);
    void          *position;
    void          *matrix;
    char          *particle;
    struct Hero   *player;
    float          yaw, pitch;
    switch (moby[0x20]) {
    case PROJECTILE_STATE_FLYING: {
        float step, time;
        step = D_0015ED6C;
        time = frame_time_sq;
        position = moby + 0x10;
        (*(unsigned short *)(projectile_data + 6))++;
        approach_value((float *)(projectile_data + 0x10), step * 30.0f, time * 70.0f);
        if (*(void **)(projectile_data + 8) &&
            *(short *)(projectile_data + 6) > scale_game_frames(7)) {
            qcopy(desired.f, *(char **)(projectile_data + 8) + 0x10);
            desired.f[2] += 0.7f;
            yaw = fast_arc_tan(desired.f[0] - MOBY(moby)->pos.x, desired.f[1] - MOBY(moby)->pos.y);
            pitch = -fast_arc_tan(vector_distance_xy(position, desired.f),
                                  desired.f[2] - MOBY(moby)->pos.z);
            func_L00_0025CCF0_spark(moby + 0x48,
                                    projectile_data + 0x14,
                                    yaw,
                                    0.03f,
                                    0.3f,
                                    D_0015ED6C * 9.599310874938965f,
                                    0);
            func_L00_0025CCF0_spark(moby + 0x44,
                                    projectile_data + 0x18,
                                    pitch,
                                    0.03f,
                                    0.3f,
                                    D_0015ED6C * 9.599310874938965f,
                                    0);
        }
        build_spherical_offset(
            desired.f, *(float *)(projectile_data + 0x10), MOBY(moby)->rot.z, -MOBY(moby)->rot.y);
        random.quad = 0;
        random.f[0] = random_float_between(-1.0f, 1.0f);
        random.f[1] = random_float_between(-1.0f, 1.0f);
        random.f[2] = random_float_between(-1.0f, 1.0f);
        velocity.quad = random.quad;
        fast_vec_normalize(velocity.f, velocity.f, random_float_between(0.1f, 0.2f) * D_0015ED6C);
        fast_vec_normalize(
            previous.f, desired.f, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
        fast_vec_add(velocity.f, velocity.f, previous.f);
        fast_vec_normalize(previous.f,
                           desired.f,
                           random_float_between(0.0f, 1.0f) * *(float *)(projectile_data + 0x10));
        fast_vec_add(previous.f, previous.f, position);
        particle = spawn_particle(previous.f,
                                  velocity.f,
                                  scale_game_frames(60),
                                  127,
                                  0x606060,
                                  3,
                                  40000.0f,
                                  1000.0f,
                                  1.0f,
                                  -0.0002f,
                                  0.0f);
        if (particle) {
            unsigned char cls = *(unsigned char *)D_L16_001B255C;
            particle[3] = 0x44;
            particle[2] = cls;
        }
        matrix = moby + 0xC0;
        spawn_particle(position,
                       velocity.f,
                       scale_game_frames(6),
                       127,
                       0xB0B0B0,
                       3,
                       40000.0f,
                       1000.0f,
                       1.0f,
                       -0.0004f,
                       0.0f);
        random.quad = 0;
        random.f[2] = 0.02f;
        transform_vector_by_basis(random.f, random.f, matrix);
        build_look_at_matrix(random.f, random.f, matrix, random_angle_radians());
        spawn_burst(
            previous.f, random.f, 0x4F007FFF, 0x1FFFFFFF, scale_game_frames(5), 1, 20000.0f);
        qcopy(previous.f, position);
        fast_vec_add(position, position, desired.f);
        if (MOBY(moby)->pos.x < 2.0f || MOBY(moby)->pos.y < 2.0f || MOBY(moby)->pos.z < 2.0f)
            goto remove;
        {
            int impact_state;
            query.flags = 0x830000;
            impact_state = 1;
            query.owner = moby;
            query.scale = 1.0f;
            query.active = impact_state;
            qcopy(query.direction, desired.f);
            set_vector_length_xy(query.direction, query.direction, 1.0f);
            player = &hero;
            query.direction[2] = 1.0f;
            query.direction[3] = 5627.9248046875f;
            query.kind = 3;
            query.cls = ((unsigned short)MOBY(moby)->oclass);
            query.enabled = impact_state;
            if (collision_line(previous.f, position, 0, ((void *)player->moby), &query)) {
                L16SparkHit *result = &D_L16_001742C0_hit;
                char        *hit = result->moby;
                if (hit) {
                    if (hit == ((char *)player->moby) || hit == *(char **)projectile_data)
                        break;
                } else if (result->count <= 0)
                    break;
                qcopy(position, result->position);
                moby[0x20] = impact_state;
            } else if (fast_dec_timer_s16(projectile_data + 4) ||
                       vector_distance_xy(position, &player->motion.pos) > 60.0f) {
            remove:
                mark_moby_for_removal(moby);
            }
        }
        break;
    }
    case PROJECTILE_STATE_IMPACT:
        func_L00_0025F4A8_alt(moby,
                              &D_L16_0015F580_spark,
                              0,
                              0.0f,
                              0.0f,
                              3,
                              3,
                              5,
                              1.0f,
                              0.5f,
                              4.0f,
                              0.7f,
                              0,
                              7.0f,
                              0,
                              0,
                              -1,
                              0);
        mark_moby_for_removal(moby);
        break;
    }
}
/* Migrated from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C5030). */

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

extern char *D_L16_001B0930[];
extern s32   allocate_voice_for_target_entry_u(s32, s32, void *) __asm__("func_0022DA68");
extern void  sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
s32          is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");
void         mark_moby_for_removal_c(struct Obj *obj) __asm__("FUN_0020c828");

void update_path_following_rocket(struct Moby *moby) __asm__("FUN_L16_002c3d38");

void update_path_following_rocket(struct Moby *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_WAIT_FOR_TRIGGER = 1,
        ROCKET_STATE_FOLLOW_PATH = 2,
        ROCKET_STATE_WAIT_FOR_RESET = 3
    };
    char *data = (char *)moby->pvars;
    char *path = D_L16_001B0930[*(int *)(data + 4)];
    switch (moby->state) {
    case ROCKET_STATE_INITIALIZE:
        if (*(int *)data == -1 || *(int *)(data + 4) == -1) {
            mark_moby_for_removal_c(moby);
            break;
        }
        *(float *)(data + 0xC) =
            *(float *)(data + 8) * D_0015ED6C / fast_vec_dist(path + 0x10, path + 0x20);
        moby->state = 1;
        moby->update_dist = 100;
        moby->command = 0;
        break;
    case ROCKET_STATE_WAIT_FOR_TRIGGER: {
        struct Hero *player = &hero;
        if (player->state.control_mode != 15)
            break;
        if (is_point_inside_clip_volume(&player->motion.body_center, *(int *)data)) {
            moby->state = 2;
            *(short *)(data + 0x16) = 0;
        }
        break;
    }
    case ROCKET_STATE_FOLLOW_PATH:
        *(float *)(data + 0x10) += *(float *)(data + 0xC);
        if ((float)(*(int *)path - 1) <= *(float *)(data + 0x10)) {
            moby->update_dist = 0xFF;
            moby->state = 3;
        } else {
            sample_camera_path(
                (int *)path, 0, &moby->pos, &moby->rot.x, 0, *(float *)(data + 0x10));
        }
        if (moby->command)
            moby->collision = 0;
        if (!voice_is_playing(moby, *(short *)(data + 0x14))) {
            *(short *)(data + 0x14) = func_0022ED80_i(0, 4, moby);
        }
        if (*(short *)(data + 0x16) == 0 &&
            vector_distance_xy(&moby->pos, (char *)(&hero.motion.pos)) < 7.0f) {
            *(short *)(data + 0x16) = 1;
            allocate_voice_for_target_entry_u(1, 0, moby);
        }
        break;
    case ROCKET_STATE_WAIT_FOR_RESET:
        if (moby->visible == 0) {
            moby->visible = 0;
            moby->collision = 0;
            moby->flags |= 1;
        }
        if (*(int *)((char *)(&hero.state.control_mode)) != 15) {
            float pos[4];
            qcopy(pos, path + 0x10);
            pos[3] = 3.0f;
            if (fast_bsphere_check((char *)pos, 64.0f) == -1) {
                moby->command = 0;
                *(int *)(data + 0x10) = 0;
                sample_camera_path(
                    (int *)path, 0, &moby->pos, &moby->rot.x, 0, *(float *)(data + 0x10));
                moby->visible = 1;
                moby->flags &= ~1;
                moby->collision = moby->pclass->collision;
                *(short *)(data + 0x16) = 0;
                moby->state = 1;
            }
        }
        break;
    }
}

/* Spawns effects around a moby when a nearby position is valid. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C57E8), where it is exact; names translated to the US level program. */

extern float D_0015ED6C, D_0015EE70_q __asm__("D_0015EE70");
extern int   FUN_L00_001f0d60_q(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void *spawn_feather_moby(void *, void *, void *) __asm__("FUN_L00_002cbf68");
extern void  FUN_0022da68_q(int, int, void *) __asm__("FUN_0022da68");
extern void  func_00215C00_57_q(float, float, float, void *) __asm__("FUN_00214db0");
extern int   func_0022ED80_57_q(int, int, void *) __asm__("FUN_0022da68");

extern float reposition_effect_velocity_elevation_degrees_gp __asm__("D_L16_001618E8")
    __attribute__((sda));
extern float reposition_effect_velocity_scale_range_start_gp __asm__("D_L16_001618EC")
    __attribute__((sda));
extern float reposition_effect_velocity_scale_range_end_gp __asm__("D_L16_001618F0")
    __attribute__((sda));
extern float reposition_effect_spawn_z_offset_gp __asm__("D_L16_001618F4") __attribute__((sda));
extern float reposition_effect_radius_range_start_gp __asm__("D_L16_001618F8") __attribute__((sda));
extern float reposition_effect_radius_range_end_gp __asm__("D_L16_001618FC") __attribute__((sda));
void         reposition_moby_with_collision_check(struct Moby *moby,
                                                  void        *new_position) __asm__("FUN_L16_002c44f0");

void reposition_moby_with_collision_check(struct Moby *moby, void *new_position) {
    float origin[4];
    float pos[4];
    float effect_velocity[4];
    float out[4];
    float effect_position_angle, random_effect_angle, radius, speed;
    int   i;
    char *moby_data;
    *(OvlQuad *)origin = *(OvlQuad *)new_position;
    {
        float *start = origin;
        qcopy(&moby->pos, start);
        qcopy(pos, start);
    }
    origin[2] += 0.3f;
    if (FUN_L00_001f0d60_q(0.2f, pos, 0, moby)) {
        float *effectPosition = out;
        float *effectVelocity = effect_velocity;
        i = 3;
        do {
            radius = random_float_between(reposition_effect_radius_range_start_gp,
                                          reposition_effect_radius_range_end_gp);
            effect_position_angle = random_angle_radians();
            random_effect_angle = random_angle_radians();
            func_00215C00_57_q(radius, effect_position_angle, random_effect_angle, effectPosition);
            out[2] += reposition_effect_spawn_z_offset_gp;
            fast_vec_add(effectPosition, effectPosition, &moby->pos);
            speed =
                random_float_between(reposition_effect_velocity_scale_range_start_gp * D_0015ED6C,
                                     reposition_effect_velocity_scale_range_end_gp * D_0015ED6C);
            random_effect_angle = random_angle_radians();
            func_00215C00_57_q(speed,
                               random_effect_angle,
                               reposition_effect_velocity_elevation_degrees_gp * DEG_TO_RAD,
                               effectVelocity);
            speed = random_float_between(3.5f, 5.5f);
            effect_velocity[2] = speed * D_0015ED6C;
            spawn_feather_moby(moby, effectPosition, effectVelocity);
        } while (--i >= 0);
        func_0022ED80_57_q(1, 0, moby);
    } else {
        moby_data = (char *)moby->pvars;
        *(float *)(moby_data + 0x78) = 1.0f;
        if (moby->prev_seq != 2)
            blend_moby_animation(moby, 2, 0, 1);
        moby->flags = (moby->flags | 0x1000) & 0xFFBE;
        moby->collision = moby->pclass->collision;
        moby->state = 4;
        *(short *)(moby_data + 0x66) = scale_game_frames(0xB4);
    }
}
/* Update a thrown bomb (class 0x119): fly and bounce off what it hits and off the ground, rest, then burst. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C7218), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad[0x18];
    char *moby;
    int   count;
    float position[4];
    float unk30[4];
    float normal[4];
} L16SparkHitN;

typedef struct {
    char          pad00[7];
    unsigned char reaction; /* 0x07 */
    char          pad08[8];
    float         velocity[4]; /* 0x10 */
    void         *owner;       /* 0x20: the moby that threw it, left out of the sweep */
    float         gravity;     /* 0x24 */
    int           timer;       /* 0x28 */
} L16FallingData;

typedef struct {
    char            pad00[0x10];
    float           position[4]; /* 0x10 */
    unsigned char   state;       /* 0x20 */
    char            pad21[0x57];
    L16FallingData *data; /* 0x78 */
} L16FallingMoby;

extern char D_L16_0015F580_macro[] __asm__("D_L16_0015F580") MACRO_ADDR;

extern int          coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void         trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void         trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void         spawn_explosion(void *,
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
extern void         allocate_voice_for_target_entry_alt(int, int, void *) __asm__("FUN_0022da68");
extern L16SparkHitN D_L16_hitN __asm__("D_L16_00174240");
extern int          func_0022ED80_n(int, int, void *) __asm__("FUN_0022da68");

void update_thrown_bomb(L16FallingMoby *moby) __asm__("FUN_L16_002c5eb0");

void update_thrown_bomb(L16FallingMoby *moby) {
    enum {
        BOMB_STATE_MOVING = 1,
        BOMB_STATE_RESTING = 2,
        BOMB_STATE_EXPLODE = 3
    };
    float           previous[4], normal[4];
    L16FallingData *bomb_data = moby->data;

    switch (moby->state) {
    case BOMB_STATE_MOVING:
        /* No pointer locals: m->position and d->velocity are written at each use and the compiler caches
           them itself (velocity formed in the argument register and copied to a saved one, the position
           copied a second time for the ground test). */
        qcopy(previous, moby->position);
        fast_vec_add(moby->position, moby->position, bomb_data->velocity);
        if (collision_line(previous, moby->position, 5, bomb_data->owner, 0)) {
            fast_vec_normalize(normal, D_L16_hitN.normal, 1.0f);
            if (fast_vec_dot(normal, bomb_data->velocity) < 0.0f) {
                reflect_vector(bomb_data->velocity, bomb_data->velocity, normal);
                fast_vec_scale(bomb_data->velocity, bomb_data->velocity, 0.25f);
            }
        }
        if (coll_sphere(0.333f, moby->position, 2, 0)) {
            fast_vec_normalize(normal, D_L16_hitN.normal, 0.333f);
            if (fast_vec_dot(normal, bomb_data->velocity) < 0.0f) {
                reflect_vector(bomb_data->velocity, bomb_data->velocity, normal);
                fast_vec_scale(bomb_data->velocity, bomb_data->velocity, 0.25f);
            }
            qcopy(moby->position, D_L16_hitN.position);
            moby->position[2] += normal[2];
            if (normal[2] >= 0.3f && fast_vec_length(bomb_data->velocity) < D_0015ED6C * 0.25f) {
                moby->state = 2;
                bomb_data->timer = scale_game_frames(90);
            }
        }
        if (fast_dec_timer(&bomb_data->timer)) {
            moby->state = 3;
        } else if (bomb_data->timer < scale_game_frames(45) &&
                   bomb_data->timer % scale_game_frames(10) == 0) {
            bomb_data->reaction = 250;
            trooper_anim_update(moby, (short *)bomb_data);
        }
        bomb_data->velocity[2] -= bomb_data->gravity;
        break;
    case BOMB_STATE_RESTING:
        if (fast_dec_timer(&bomb_data->timer)) {
            moby->state = 3;
        } else if (bomb_data->timer < scale_game_frames(61) &&
                   bomb_data->timer % scale_game_frames(20) == 0) {
            bomb_data->reaction = 250;
            trooper_anim_update(moby, (short *)bomb_data);
        }
        break;
    case BOMB_STATE_EXPLODE:
        func_0022ED80_n(0, 0, moby);
        spawn_explosion(moby,
                        (int)D_L16_0015F580_macro,
                        0,
                        1.5f,
                        1.0f,
                        7,
                        10,
                        20,
                        3.0f,
                        1.7f,
                        4.0f,
                        -1,
                        1.0f,
                        7.0f,
                        0,
                        7,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    }
    trooper_anim_finish(moby, bomb_data);
}
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CAA18), where it is exact; names translated to the US level program. */

typedef struct {
    float a[4];
    float b[4];
    int   c[4];
    float d[4];
} L16SpriteData;

extern char           D_L16_001671C0[];
extern unsigned char *FUN_L00_00272f68(void *, int, unsigned char, int, int, int, int, float);
extern int            fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

void emit_moby_sprite_effect_particles(struct Moby *moby) __asm__("FUN_L16_002c9650");

void emit_moby_sprite_effect_particles(struct Moby *moby) {
    L16SpriteData *sprite_data = (L16SpriteData *)moby->pvars;
    float          d[4] __attribute__((aligned(16)));
    float          particle_position_step[4] __attribute__((aligned(16)));
    int            i, col;
    float          elapsed_cycle_fraction;

    moby->pos.z += 0.1f;
    fast_vec_sub(d, D_L16_001671C0, &moby->pos);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(particle_position_step, d, 0.1f);
    fast_vec_add(d, d, &moby->pos);
    moby->pos.z -= 0.1f;
    for (i = 0; i < 4; i++) {
        float unwrapped_sprite_value = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = unwrapped_sprite_value;
        if (255.0f <= unwrapped_sprite_value) {
            sprite_data->a[i] = unwrapped_sprite_value - 255.0f;
        } else if (unwrapped_sprite_value <= 0.0f) {
            sprite_data->a[i] = unwrapped_sprite_value + 255.0f;
        }
        if (fast_dec_timer(&sprite_data->c[i])) {
            sprite_data->c[i] = scale_game_frames(0xFF);
        }
        elapsed_cycle_fraction =
            convert_integer_to_float(scale_game_frames(0xFF) - sprite_data->c[i]);
        elapsed_cycle_fraction = elapsed_cycle_fraction / (float)scale_game_frames(0xFF);
        col =
            fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - elapsed_cycle_fraction));
        FUN_L00_00272f68(d,
                         col,
                         truncate_float_to_s32(sprite_data->a[i]) & 0xFF,
                         0x35,
                         1,
                         2,
                         0,
                         sprite_data->d[i]);
        fast_vec_add(d, d, particle_position_step);
    }
}
/* Updates a linked moby's movement and follows it while its state is active. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CAC40), where it is exact; names translated to the US level program. */

extern float D_0015ED70_c __asm__("D_0015ED70");
extern void  FUN_L16_002c9a50_u(unsigned char *) __asm__("FUN_L16_002c9a50");
extern void  update_conveyor_group_motion_transforms(int, void *) __asm__("FUN_L16_002c9c38");
void         draw_conveyor_belt_strips(struct Moby *m) __asm__("FUN_L16_002c9cd0");

extern float conveyor_belt_base_speed_gp __asm__("D_L16_0016198C") __attribute__((sda));
void         conveyor_belt_update(struct Moby *m) __asm__("FUN_L16_002c9878");

void conveyor_belt_update(struct Moby *moby) {
    enum {
        CONVEYOR_BELT_STATE_INITIALIZE = 0,
        CONVEYOR_BELT_STATE_APPROACH_BASE_SPEED = 1,
        CONVEYOR_BELT_STATE_APPROACH_NEGATED_SPEED = 2
    };
    char *conveyor_data = (char *)moby->pvars;
    float conveyor_motion_vector[4];
    float target;
    int   one = 1;
    switch (moby->state) {
    case CONVEYOR_BELT_STATE_INITIALIZE:
        *(int *)(conveyor_data + 0x5C) |= 4;
        moby->visible = 0;
        moby->flags |= one;
        if (*(int *)(conveyor_data + 0x60) == -1) {
            moby->flags |= 2;
        } else {
            moby->state = 2;
        }
        *(int *)(conveyor_data + 0x74) = -1;
        break;
    case CONVEYOR_BELT_STATE_APPROACH_NEGATED_SPEED:
        target = -(conveyor_belt_base_speed_gp * D_0015ED6C);
        if (*(float *)(conveyor_data + 0x6C) != target) {
            approach_value((float *)(conveyor_data + 0x6C), target, D_0015ED70_c * 12.0f);
            fast_vec_normalize(
                conveyor_motion_vector, &moby->rot_row0, *(float *)(conveyor_data + 0x6C));
            update_conveyor_group_motion_transforms(moby->group, conveyor_motion_vector);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(conveyor_data + 0x68)))
            moby->state = one;
        add_draw_callback(draw_conveyor_belt_strips, moby);
        break;
    case CONVEYOR_BELT_STATE_APPROACH_BASE_SPEED:
        target = conveyor_belt_base_speed_gp * D_0015ED6C;
        if (*(float *)(conveyor_data + 0x6C) != target) {
            approach_value((float *)(conveyor_data + 0x6C), target, D_0015ED70_c * 12.0f);
            fast_vec_normalize(
                conveyor_motion_vector, &moby->rot_row0, *(float *)(conveyor_data + 0x6C));
            update_conveyor_group_motion_transforms(moby->group, conveyor_motion_vector);
        }
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(conveyor_data + 0x64)))
            moby->state = 2;
        add_draw_callback(draw_conveyor_belt_strips, moby);
        break;
    }
    if (moby->state)
        FUN_L16_002c9a50_u(moby);
}

#ifndef NOT_SDA
#endif
#ifndef MACRO_ADDR
#endif
extern int   func_0022ED80_cae18_c9a50(int, int, void *) __asm__("FUN_0022da68");
extern int  *D_L16_001ABFC0_c9a50[] __asm__("D_L16_001ABCC0");
extern char *D_L16_00160098_c9a50 __asm__("D_L16_0015FFD8");
extern char  voice_pool[] __asm__("D_0013E550");
void         update_conveyor_group_voice(struct Moby *moby) __asm__("FUN_L16_002c9a50");

void update_conveyor_group_voice(struct Moby *moby) {
    short *group_entry = (short *)D_L16_001ABFC0_c9a50[moby->group];
    char  *moby_data = (char *)moby->pvars;
    char  *nearest = 0;
    float  best = 1024.0f;
    float  voice_position_offset[4];
    if (group_entry == 0)
        return;
    do {
        char *other = D_L16_00160098_c9a50 + (((unsigned short)*group_entry & 0x7FFF) << 8);
        if (*(short *)(other + 0xA6) == 0x1D7) {
            float candidate_distance = fast_vec_dist(D_L16_001671C0, other + 0x10);
            if (candidate_distance < best) {
                nearest = other;
                best = candidate_distance;
            }
        }
    } while (*group_entry++ >= 0);
    if (fast_vec_dist(nearest + 0x10, D_L16_001671C0) < 64.0f) {
        int   voice_slot_index = *(int *)(moby_data + 0x74);
        char *slot;
        if (voice_slot_index >= 0) {
            char *target;
            slot = voice_pool + voice_slot_index * 0x70;
            target = *(char **)(slot + 0x88);
            if (target && *(short *)(target + 0xA6) == 0x1D7) {
                fast_vec_normalize(voice_position_offset, nearest + 0xE0, 2.5f);
                fast_vec_add(voice_pool + 0x90 + *(int *)(moby_data + 0x74) * 0x70,
                             voice_position_offset,
                             nearest + 0x10);
                *(char **)(voice_pool + 0x88 + *(int *)(moby_data + 0x74) * 0x70) = nearest;
                return;
            }
        }
        voice_slot_index = func_0022ED80_cae18_c9a50(0, 13, nearest);
        *(int *)(moby_data + 0x74) = voice_slot_index;
        if (voice_slot_index >= 0) {
            fast_vec_normalize(voice_position_offset, &moby->rot_row2, 2.5f);
            fast_vec_add(voice_pool + 0x90 + *(int *)(moby_data + 0x74) * 0x70,
                         voice_position_offset,
                         &moby->pos);
        }
    }
}

#include "qcopy.h"

/* Draw paired textured strips along an indexed pose, fading the end caps. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CB098), where it is exact; names translated to the US level program. */

typedef struct {
    float point[4][4];
    int   color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RibbonPacket_u;

extern float D_L16_001D2FF0[][2], D_L16_001D32E0[][2], D_L16_001D3300[][4];
extern float D_L16_001D32C0[][2], D_L16_001D3010[][2], D_L16_001D3300[][4];
extern float D_L16_001D32C0[][2], D_L16_001D32E0[][2], D_L16_001D3030[][4];
extern int   conveyor_belt_pulse_texture_id_gp __asm__("D_L16_00161990") __attribute__((sda));
extern int   conveyor_belt_scrolling_texture_id_gp __asm__("D_L16_00161994") __attribute__((sda));

#include "rnc/overlay/l16/mobys/rocket_data/draw_conveyor_belt_strips.h"

extern int  conveyor_belt_pulse_color_start_gp __asm__("D_L16_001619A8") __attribute__((sda));
extern int  conveyor_belt_pulse_color_end_gp __asm__("D_L16_001619AC") __attribute__((sda));
extern int  conveyor_belt_scrolling_vertex_color_gp __asm__("D_L16_001619B0") __attribute__((sda));
extern int  D_L16_001600EC_m __asm__("D_L16_001600EC");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad_u(void *, void *, int) __asm__("draw_geometry_quad");

void draw_conveyor_belt_strips(struct Moby *moby) {
    L16RibbonPacket_u packets[2];
    char             *conveyor_data = ((char *)moby->pvars);
    float             scale =
        fast_vec_length(D_L16_001600EC_m + ((*(int *)(conveyor_data + 0x60)) << 7)) * 2.001f;
    int   count = truncate_float_to_s32(scale);
    float step = 2.0f / scale;
    int   period, tint, color, limit, j;
    float phase;
    long  modebits, flags;
    (*(float *)((char *)conveyor_data + 0x70)) =
        (*(float *)((char *)conveyor_data + 0x70)) + (*(float *)((char *)conveyor_data + 0x6C));
    if ((*(float *)((char *)conveyor_data + 0x70)) > 1.0f)
        (*(float *)((char *)conveyor_data + 0x70)) -= 1.0f;
    else if ((*(float *)((char *)conveyor_data + 0x70)) < 0.0f)
        (*(float *)((char *)conveyor_data + 0x70)) += 1.0f;
    modebits = 0x8000000000L;
    period = scale_game_frames(120);
    flags = 0xFF9000000260L;
    limit = count + 2;
    phase = convert_integer_to_float(D_L16_0015F5CC % period);
    phase = phase / convert_integer_to_float(period);
    phase = fast_sin(phase * 6.28318f - 3.14159f) * 0.5f + 0.5f;
    tint = fast_tween_color(
        conveyor_belt_pulse_color_start_gp, conveyor_belt_pulse_color_end_gp, phase);
    color = conveyor_belt_scrolling_vertex_color_gp;
    packets[0].texture = get_effect_texture(conveyor_belt_pulse_texture_id_gp);
    packets[0].mode = (long)conveyor_belt_alpha_blend_a_selector |
                      ((long)conveyor_belt_alpha_blend_b_selector << 2) |
                      ((long)conveyor_belt_alpha_blend_c_selector << 4) |
                      ((long)conveyor_belt_alpha_blend_d_selector << 6) | modebits;
    packets[0].flags = flags;
    packets[0].zero = 0;
    packets[1].texture = get_effect_texture(conveyor_belt_scrolling_texture_id_gp);
    packets[1].mode = (long)conveyor_belt_alpha_blend_a_selector |
                      ((long)conveyor_belt_alpha_blend_b_selector << 2) |
                      ((long)conveyor_belt_alpha_blend_c_selector << 4) |
                      ((long)conveyor_belt_alpha_blend_d_selector << 6) | modebits;
    packets[1].flags = flags;
    packets[1].zero = 0;
    {
        int vertex;
        for (vertex = 0; vertex < 4; vertex++) {
            packets[0].uv[vertex].u = D_L16_001D2FF0[vertex][0];
            packets[0].uv[vertex].v = D_L16_001D2FF0[vertex][1];
            packets[1].uv[vertex].u =
                D_L16_001D3010[vertex][0] - (*(float *)(conveyor_data + 0x70));
            packets[1].uv[vertex].v = D_L16_001D3010[vertex][1];
            packets[0].color[vertex] = tint;
            packets[1].color[vertex] = color;
            qcopy(packets[0].point[vertex], D_L16_001D3030[vertex]);
            qcopy(packets[1].point[vertex], D_L16_001D3030[vertex]);
            /* the same vertex of both packets, one packet apart */
            if (vertex < 2) {
                float *pt = packets[0].point[vertex];
                pt[0] -= step;
                pt[sizeof(L16RibbonPacket_u) / sizeof(float)] -= step;
            }
        }
    }
    {
        int strip;
        for (strip = 0; strip < limit; strip++) {
            if (strip == 0) {
                packets[0].color[1] = 0;
                packets[0].color[0] = 0;
                packets[1].color[1] = 0;
                packets[1].color[0] = 0;
            } else if (strip == count + 1) {
                packets[0].color[3] = 0;
                packets[0].color[2] = 0;
                packets[1].color[3] = 0;
                packets[1].color[2] = 0;
            } else if (strip == 1) {
                packets[0].color[1] = tint;
                packets[0].color[0] = tint;
                packets[1].color[1] = color;
                packets[1].color[0] = color;
            }
            draw_geometry_quad_u(
                &packets[0], D_L16_001600EC_m + ((*(int *)(conveyor_data + 0x60)) << 7), 0);
            draw_geometry_quad_u(
                &packets[1], D_L16_001600EC_m + ((*(int *)(conveyor_data + 0x60)) << 7), 0);
            {
                float *point = packets[0].point[0];
                for (j = 3; j >= 0; j--) {
                    float a = *point + step,
                          b = point[sizeof(L16RibbonPacket_u) / sizeof(float)] + step;
                    *point = a;
                    point[sizeof(L16RibbonPacket_u) / sizeof(float)] = b;
                    point += 4;
                }
            }
        }
    }
}

#include "qcopy.h"

/* Updates the test dummy (moby class 541): walks its path, throws at or sprays its target, bursts and respawns. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CF180), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad00[0x20];
    float         f20;
    short         h24;
    char          pad26[2];
    unsigned char b28;
    char          pad29[5];
    unsigned char b2E;
    char          pad2F[0x41];
    float         target[4];
    char          pad80[0x34];
    int           mode;
    char          padB8[8];
    int           kind;
    int           path;
    int           lives;
    int           iCC;
    int           aim[2];
    int           sound_path;
    int           node;
    float         spawn[4];
    int           timer;
    char          padF4[4];
    char         *route;
    float         turn;
    char          pad100[4];
    char         *child;
    int           step;
    float         f10C;
    unsigned char next_state;
    char          pad111[3];
    int           next_anim;
    char          pad118[4];
    float         f11C;
    char          slots[0x20];
    char          block140[1];
} L16DummyData;

extern char *D_L16_00174258 NOT_SDA;
extern float                frame_time_sq;
extern float                FUN_L00_0025abf0(void *, void *, void *, float, float);
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern float probe_ground_height(void *, int, float);
extern int   D_L16_0015F580_c __asm__("D_L16_0015F580");
extern char *D_L16_001600EC_c __asm__("D_L16_001600EC");
extern int   D_L16_001619C8_gp __asm__("D_L16_001619C8") __attribute__((sda));
extern float test_dummy_bomb_horizontal_launch_speed_gp __asm__("D_L16_001619CC")
    __attribute__((sda));

#include "rnc/overlay/l16/mobys/rocket_data/test_dummy_update.h"

extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
int        move_test_dummy_toward_position_and_heading(char *moby,
                                                       void *target_position,
                                                       float angle) __asm__("FUN_L16_002ced20");

extern int
move_test_dummy_toward_position_and_heading(char *, void *, float) __asm__("FUN_L16_002ced20");
extern int   is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern void *FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
void        *find_nearest_test_dummy_door(int index, void *position) __asm__("FUN_L16_002d5db0");

extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025f730(void *, float);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void reset_shot_tracking(char *) __asm__("FUN_L02_00264e40");
extern void effect_slots_fire(void *, void *, void *, float) __asm__("FUN_L02_00264e70");
extern void collect_mobies_around_tracked_objects(char **, int, int) __asm__("FUN_L02_002651d0");
extern void update_test_dummy_damage_and_respawn(void *) __asm__("FUN_L16_002ce9f0");
void        refresh_test_dummy_door_timer(struct Moby *moby) __asm__("FUN_L16_002d5e80");

extern void refresh_test_dummy_door_timer(struct Moby *) __asm__("FUN_L16_002d5e80");
unsigned char *
spawn_thrown_bomb(int a, void *pos, float *dir, int c, float f) __asm__("FUN_L16_002c6268");
extern unsigned char            *
func_L16_002C75D0_2cf180(int, void *, float *, float, int) __asm__("FUN_L16_002c6268");
extern int func_L00_0025A8E8_2cf180(
    void *, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");

void test_dummy_update(unsigned char *m) __asm__("FUN_L16_002cddb8");

void test_dummy_update(unsigned char *moby) {
    enum {
        TEST_DUMMY_STATE_INITIALIZE = 0,
        TEST_DUMMY_STATE_IDLE = 1,
        TEST_DUMMY_STATE_APPROACH_FIRST_PATH_NODE = 2,
        TEST_DUMMY_STATE_APPROACH_THROW_NODE = 3,
        TEST_DUMMY_STATE_THROW_AT_TARGET = 4,
        TEST_DUMMY_STATE_MOVE_ALONG_PATH = 5,
        TEST_DUMMY_STATE_TURN_TO_NEXT_PATH_NODE = 6,
        TEST_DUMMY_STATE_APPROACH_SPRAY_NODE = 7,
        TEST_DUMMY_STATE_SPRAY_FROM_CHILD = 8,
        TEST_DUMMY_STATE_TURN_AND_RESUME_SAVED_STATE = 9,
        TEST_DUMMY_STATE_BURST = 10,
        TEST_DUMMY_STATE_RESPAWN = 11
    };
    enum {
        TEST_DUMMY_KIND_THROWER = 0,
        TEST_DUMMY_KIND_PATH_MOVER = 1,
        TEST_DUMMY_KIND_SPRAYER = 2
    };
    L16SparkVector position_or_delta, position_or_direction, launch_velocity;
    L16SparkQuery  fx;
    L16DummyData  *dummy_data = ((L16DummyData *)MOBY(moby)->pvars);
    /* A count, then 16-byte points from +0x10. A point copied whole is `path + i * 16 + 16`; its x and y are read
       through `path + (i << 4)`: retail forms index + base for the first and base + index for the second, which
       is what this compiler does with a multiplication and with a shift in an address. */
    char *path;

    update_test_dummy_damage_and_respawn(moby);
    path = D_L16_001B0930[dummy_data->path];
    FUN_L00_00263ac8(2.5f, (int)moby, 2, (unsigned char *)dummy_data->block140);
    switch (moby[0x20]) {
    case TEST_DUMMY_STATE_INITIALIZE: /* set up: lives, home position, the held child for the variant */
        dummy_data->b2E = 1;
        if (dummy_data->lives > 0) {
            MOBY(moby)->unkB4 /= dummy_data->lives;
        }
        moby[0x20] = 1;
        moby[0x31] = 0;
        MOBY(moby)->flags |= 1;
        if (random_integer_below(2)) {
            MOBY(moby)->flags |= 0x8000;
        }
        dummy_data->b28 = 1;
        dummy_data->h24 = 2;
        dummy_data->f20 = 2.0f;
        dummy_data->f11C = random_angle_radians();
        dummy_data->route = find_nearest_test_dummy_door(dummy_data->iCC, moby + 0x10);
        qcopy(dummy_data->spawn, moby + 0x10);
        switch (dummy_data->kind) {
        case TEST_DUMMY_KIND_THROWER:
            dummy_data->child = create_moby(0x35F);
            break;
        case TEST_DUMMY_KIND_PATH_MOVER:
            dummy_data->child = create_moby(0x374);
            break;
        case TEST_DUMMY_KIND_SPRAYER:
            dummy_data->child = create_moby(0x36C);
            reset_shot_tracking(dummy_data->slots);
            break;
        }
        if (dummy_data->child) {
            *(unsigned short *)(dummy_data->child + 0x34) |= 0x100;
            *(short *)(dummy_data->child + 0x32) = 0x40;
            *(unsigned char *)(dummy_data->child + 0x30) = 0;
            *(unsigned char *)(dummy_data->child + 0x31) = 1;
        }
        break;
    case TEST_DUMMY_STATE_IDLE:
        break;
    case TEST_DUMMY_STATE_APPROACH_FIRST_PATH_NODE: /* turn to the path's first node, then start by variant */
        refresh_test_dummy_door_timer(dummy_data->route);
        position_or_delta.quad = *(u128 *)(path + 0x10);
        if (move_test_dummy_toward_position_and_heading(
                moby, position_or_delta.f, MOBY(moby)->rot.z)) {
            dummy_data->timer = scale_game_frames(30);
            dummy_data->node = 1;
            dummy_data->step = 1;
            if (dummy_data->kind == 0) {
                moby[0x20] = 3;
            } else if (dummy_data->kind == 1) {
                moby[0x20] = 5;
            } else {
                moby[0x20] = 7;
            }
        }
        break;
    case TEST_DUMMY_STATE_APPROACH_THROW_NODE: /* thrower: wait, face the node, wind up */
        if (fast_dec_timer(&dummy_data->timer)) {
            position_or_delta.quad = *(u128 *)(path + dummy_data->node * 16 + 16);
            if (move_test_dummy_toward_position_and_heading(
                    moby,
                    position_or_delta.f,
                    fast_arc_tan(((float *)(path + (dummy_data->node << 4)))[4] - MOBY(moby)->pos.x,
                                 ((float *)(path + (dummy_data->node << 4)))[5] -
                                     MOBY(moby)->pos.y))) {
                dummy_data->timer = scale_game_frames(30);
                moby[0x20] = 4;
                if (moby[0x53] != 21) {
                    blend_moby_animation(moby, 21, 0, scale_game_frames(30));
                }
                if (random_integer_below(2)) {
                    char *p = D_L16_001B0930[dummy_data->sound_path];
                    FUN_L00_0025ff38(
                        12.0f, (char *)moby, dummy_data->target, 0, 0, p + 16, *(int *)p);
                } else {
                    dummy_data->mode = 2;
                }
            }
        }
        break;
    case TEST_DUMMY_STATE_THROW_AT_TARGET: /* thrower: launch the held object at the target, then face it or the next node */
        if (dummy_data->mode == 2) {
            qcopy(position_or_delta.f,
                  D_L16_001600EC_c + dummy_data->aim[dummy_data->node] * 0x80 + 0x30);
        } else {
            qcopy(position_or_delta.f, dummy_data->target);
        }
        if (is_value_within_interpolated_window(moby, 15.0f) && dummy_data->child) {
            float speed = test_dummy_bomb_horizontal_launch_speed_gp * D_0015ED6C;
            float gravity = frame_time_sq * 10.0f;
            position_or_delta.f[2] = probe_ground_height(position_or_delta.f, 0, 0.5f);
            moby_point_position(dummy_data->child, 0, position_or_direction.f);
            fast_vec_sub(launch_velocity.f, position_or_delta.f, position_or_direction.f);
            launch_velocity.f[2] = 0.0f;
            fast_vec_normalize(launch_velocity.f, launch_velocity.f, speed);
            launch_velocity.f[2] =
                FUN_L00_0025abf0(position_or_direction.f, position_or_delta.f, 0, speed, -gravity);
            func_L16_002C75D0_2cf180((int)moby,
                                     position_or_direction.f,
                                     launch_velocity.f,
                                     gravity,
                                     scale_game_frames(300));
        } else if (moby[0x70] & 2) {
            dummy_data->node = (dummy_data->node + 1) & 1;
            moby[0x20] = 3;
            if (moby[0x53] != 23) {
                blend_moby_animation(moby, 23, 0, scale_game_frames(30));
            }
        }
        if (compute_interpolated_record_value(moby) < 15.0f) {
            position_or_direction.quad = *(u128 *)(moby + 16);
            move_test_dummy_toward_position_and_heading(
                moby,
                position_or_direction.f,
                fast_arc_tan(position_or_delta.f[0] - MOBY(moby)->pos.x,
                             position_or_delta.f[1] - MOBY(moby)->pos.y));
        } else {
            int node = (dummy_data->node + 1) & 1;
            position_or_direction.quad = *(u128 *)(moby + 16);
            move_test_dummy_toward_position_and_heading(
                moby,
                position_or_direction.f,
                fast_arc_tan(((float *)(path + (node << 4)))[4] - MOBY(moby)->pos.x,
                             ((float *)(path + (node << 4)))[5] - MOBY(moby)->pos.y));
        }
        break;
    case TEST_DUMMY_STATE_MOVE_ALONG_PATH: /* slider: move toward the node, emit, stop on arrival */
        if (moby[0x53] == 10) {
            if (moby[0x70] & 2) {
                blend_moby_animation(moby, 11, 0, 1);
            }
        } else {
            char        *position = (char *)moby + 16;
            struct Hero *player;
            fast_vec_sub(position_or_delta.f, path + (dummy_data->node * 16 + 16), position);
            FUN_L00_0025f730(position_or_delta.f,
                             test_dummy_path_movement_speed_limit_gp * D_0015ED6C);
            fast_vec_add(position, position, position_or_delta.f);
            moby_point_position(moby, D_L16_001619C8_gp, position_or_direction.f);
            func_L00_0025A8E8_2cf180(moby, 0.333f, position_or_direction.f, 1, 1.0f, 1.0f, 0, 1, 0);
            player = &hero;
            if (D_L16_00174258 == ((char *)player->moby) && player->invulnerable_frames == 0) {
                func_0022ED80_i(6, 0, moby);
            }
            if (fast_vec_length(position_or_delta.f) < 0.0001f) {
                if (moby[0x53] != 9) {
                    blend_moby_animation(moby, 9, 0, scale_game_frames(20));
                }
                moby[0x20] = 6;
                dummy_data->timer = scale_game_frames(60);
                if (dummy_data->node == 0) {
                    dummy_data->step = 1;
                } else if (dummy_data->node == *(int *)path - 1) {
                    dummy_data->step = -1;
                }
            }
        }
        break;
    case TEST_DUMMY_STATE_TURN_TO_NEXT_PATH_NODE: { /* slider: turn toward the next node while the timer runs */
        float *yaw = (float *)&MOBY(moby)->rot.z;
        int    next = dummy_data->node + dummy_data->step;
        float  angle = fast_arc_tan(((float *)(path + (next << 4)))[4] - MOBY(moby)->pos.x,
                                   ((float *)(path + (next << 4)))[5] - MOBY(moby)->pos.y);
        approach_angle(yaw,
                       &dummy_data->turn,
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       D_0015ED6C * 25.132742f);
        if (fast_dec_timer(&dummy_data->timer)) {
            dummy_data->node = next;
            if (moby[0x53] != 10) {
                blend_moby_animation(moby, 10, 0, scale_game_frames(10));
            }
            moby[0x20] = 5;
        }
        break;
    }
    case TEST_DUMMY_STATE_APPROACH_SPRAY_NODE: /* sprayer: wait, face the node, start */
        if (fast_dec_timer(&dummy_data->timer)) {
            position_or_delta.quad = *(u128 *)(path + dummy_data->node * 16 + 16);
            if (move_test_dummy_toward_position_and_heading(
                    moby, position_or_delta.f, MOBY(moby)->rot.z)) {
                dummy_data->timer = scale_game_frames(30);
                moby[0x20] = 8;
                if (moby[0x53] != 1) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(30));
                }
                if (random_integer_below(2)) {
                    char *p = D_L16_001B0930[dummy_data->sound_path];
                    FUN_L00_0025ff38(
                        12.0f, (char *)moby, dummy_data->target, 0, 0, p + 16, *(int *)p);
                } else {
                    dummy_data->mode = 2;
                }
            }
        }
        break;
    case TEST_DUMMY_STATE_SPRAY_FROM_CHILD: /* sprayer: face the target and emit from the child toward it */
        if (dummy_data->mode != 2) {
            position_or_delta.quad = *(u128 *)(moby + 16);
            move_test_dummy_toward_position_and_heading(
                moby,
                position_or_delta.f,
                fast_arc_tan(dummy_data->target[0] - MOBY(moby)->pos.x,
                             dummy_data->target[1] - MOBY(moby)->pos.y));
        }
        if (dummy_data->child) {
            char *slots;
            moby_point_position(dummy_data->child, 0, position_or_delta.f);
            slots = dummy_data->slots;
            position_or_direction.f[0] =
                fast_cos(fast_arc_tan(position_or_delta.f[0] - MOBY(moby)->pos.x,
                                      position_or_delta.f[1] - MOBY(moby)->pos.y));
            position_or_direction.f[1] =
                fast_sin(fast_arc_tan(position_or_delta.f[0] - MOBY(moby)->pos.x,
                                      position_or_delta.f[1] - MOBY(moby)->pos.y));
            position_or_direction.f[2] = 0.0f;
            effect_slots_fire(
                slots, position_or_delta.f, position_or_direction.f, test_dummy_spray_range_gp);
            position_or_direction.f[3] = 5627.925f;
            position_or_direction.f[2] = 1.0f;
            make_hit_descriptor(&fx, moby, 0x10001, 1.0f, position_or_direction.f);
            fx.kind = 5;
            fx.enabled = 1;
            fx.cls = ((unsigned short)MOBY(moby)->oclass);
            collect_mobies_around_tracked_objects((char **)slots, (int)moby, (int)&fx);
        }
        if (moby[0x70] & 2) {
            dummy_data->node = (dummy_data->node + 1) & 1;
            moby[0x20] = 7;
            if (moby[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(30));
            }
        }
        break;
    case TEST_DUMMY_STATE_TURN_AND_RESUME_SAVED_STATE: /* turn back to the node, then resume the saved state and animation */
        position_or_delta.quad = *(u128 *)(path + dummy_data->node * 16 + 16);
        move_test_dummy_toward_position_and_heading(moby, position_or_delta.f, dummy_data->f10C);
        if (moby[0x70] & 2) {
            moby[0x20] = dummy_data->next_state;
            if (moby[0x53] != dummy_data->next_anim) {
                blend_moby_animation(moby, dummy_data->next_anim, 0, scale_game_frames(20));
            }
        }
        break;
    case TEST_DUMMY_STATE_BURST: { /* burst into three pieces; respawn at home or delete */
        char *position = (char *)moby + 16;
        char *moby_rotation = (char *)moby + 0x40;
        float burst;
        dummy_data->f20 = 2.0f;
        dummy_data->timer = scale_game_frames(60);
        burst = frame_time_sq * 12.0f;
        FUN_L00_00263fd8(moby,
                         0x655,
                         position,
                         moby_rotation,
                         0,
                         0,
                         burst,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c);
        FUN_L00_00263fd8(moby,
                         0x656,
                         position,
                         moby_rotation,
                         0,
                         0,
                         burst,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c);
        FUN_L00_00263fd8(moby,
                         0x657,
                         position,
                         moby_rotation,
                         0,
                         0,
                         burst,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c,
                         &D_L16_0015F580_c);
        qcopy(position_or_delta.f, position);
        position_or_delta.f[2] += 1.2f;
        cart_burst(moby, position_or_delta.f, -1, 0.5f, 10.0f);
        FUN_L00_00257470(moby, 0, -1);
        func_0022ED80_i(7, 0, moby);
        dummy_data->lives--;
        if (dummy_data->lives != -1) {
            moby[0x20] = 11;
            qcopy(position, dummy_data->spawn);
            dummy_data->timer = scale_game_frames(60);
        } else {
            if (dummy_data->child) {
                mark_moby_for_removal(dummy_data->child);
            }
            mark_moby_for_removal(moby);
        }
        break;
    }
    case TEST_DUMMY_STATE_RESPAWN: /* respawn: settle to the home height as the timer runs out */
        if (fast_dec_timer(&dummy_data->timer)) {
            dummy_data->turn = 0.0f;
            moby[0x20] = 2;
            if (moby[0x53] != 9) {
                blend_moby_animation(moby, 9, 0, scale_game_frames(10));
            }
            dummy_data->f20 = 2.0f;
            MOBY(moby)->flags |= 0x1000;
        }
        MOBY(moby)->pos.z = dummy_data->spawn[2] + D_0015ED6C * 3.0f * (float)dummy_data->timer;
        break;
    }
}
/* The test dummy's respawn and hit handling: respawns or deletes it, takes damage, and carries its child on a joint. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002CFDB8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad00[0x80];
    float position[4]; /* 0x80 */
} L16HitPlayer;

typedef struct {
    char               pad00[0x20];
    float              health; /* 0x20 */
    char               pad24[10];
    unsigned char      substate; /* 0x2E: 2 asks for a respawn */
    char               pad2F[0x31];
    short              reaction[3]; /* 0x60: record func_L00_0025E4B0 and func_L00_0025E590 take */
    char               pad66;
    unsigned char      reaction_time; /* 0x67 */
    char               pad68[0x60];
    int                lives; /* 0xC8 */
    char               padCC[0x14];
    float              spawn[4]; /* 0xE0 */
    int                timer;    /* 0xF0 */
    char               padF4[0x10];
    struct L16HitMoby *child; /* 0x104 */
    char               pad108[4];
    float              heading;    /* 0x10C */
    int                next_state; /* 0x110: stored here as a word */
    int                next_anim;  /* 0x114 */
    int                frame;      /* 0x118: frame counter of the last func_L16_002D0238 */
} L16HitData;

typedef struct L16HitMoby {
    char           pad00[0x10];
    float          position[4]; /* 0x10 */
    unsigned char  state;       /* 0x20 */
    char           pad21[0x13];
    unsigned short flags; /* 0x34 */
    char           pad36[0x1D];
    unsigned char  animation; /* 0x53 */
    char           pad54[0x24];
    L16HitData    *data; /* 0x78 */
    char           pad7C[0x28];
    unsigned char  opacity; /* 0xA4 */
    char           padA5[0x1B];
    float          matrix[3][4]; /* 0xC0 */
} L16HitMoby;

extern int D_L16_001619C8_gp __asm__("D_L16_001619C8")
    __attribute__((sda)); /* no foreign declaration */
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void FUN_L16_002cee70_c(char *) __asm__("FUN_L16_002cee70");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");

/* Per-frame update of a hit-point moby: handles a hit (respawn at the spawn point or removal when out of lives) and its state timers. */
void update_test_dummy_damage_and_respawn(void *moby) {
    L16HitMoby *hit_moby = moby;
    float       matrix[16];
    int         status;
    float       amount;
    L16HitData *hit_data;
    char       *record;

    if (hit_moby->state == 1) {
        return;
    }
    hit_data = hit_moby->data;
    if (hit_data->substate == 2) {
        hit_data->substate = 1;
        hit_data->health = 2.0f;
        hit_data->timer = scale_game_frames(60);
        FUN_L00_00257470(hit_moby, 0, -1);
        hit_moby->flags &= 0xEFFF;
        hit_data->lives--;
        if (hit_data->lives != -1) {
            hit_moby->state = 11;
            qcopy(hit_moby->position, hit_data->spawn);
            hit_data->timer = scale_game_frames(60);
        } else {
            if (hit_data->child) {
                mark_moby_for_removal_c2(hit_data->child);
            }
            mark_moby_for_removal_c2(hit_moby);
        }
        return;
    }
    if (hit_data->frame != D_L16_0015F5CC) {
        FUN_L16_002cee70_c((char *)hit_moby);
    }
    amount = 0.0f;
    record = find_moby_hit(hit_moby, 0x330000, 0);
    hit_reaction(hit_moby, record, &hit_data->health, 0, &status, &amount, 0, 4);
    if (status != 1 && hit_moby->state != 10 && hit_moby->state != 11) {
        hit_data->health -= amount;
        if (hit_data->health <= 0.0f) {
            hit_moby->state = 10;
        } else {
            if (hit_moby->state != 9) {
                hit_data->next_state = hit_moby->state;
                hit_data->next_anim = hit_moby->animation;
            }
            {
                float        x = hit_moby->position[0], y = hit_moby->position[1];
                struct Hero *player = &hero;
                hit_data->heading =
                    fast_arc_tan(player->motion.pos.f[0] - x, player->motion.pos.f[1] - y);
            }
            hit_moby->state = 9;
            /* No pointer local for the reaction record anywhere: d->reaction is written at each call and the
               compiler caches it itself (formed before the hit query, and formed again here with a copy after
               the call). It only forms it again here when this place cannot be reached along one straight path
               from the hit query, that is when the animation test has two arms while its common-subexpression
               pass runs: the else arm below is what supplies the second one. The assignment is dead and leaves
               no instruction; without it the function is two instructions short (see NOTES.md). */
            if (hit_moby->animation != 8) {
                blend_moby_animation(hit_moby, 8, 2, 1);
            } else {
                record = 0;
            }
            hit_data->reaction_time = 120;
            trooper_anim_update(hit_moby, hit_data->reaction);
        }
    }
    hit_moby->opacity = 255;
    trooper_anim_finish(hit_moby, hit_data->reaction);
    if (hit_data->child) {
        moby_point_position(hit_moby, D_L16_001619C8_gp, hit_data->child->position);
        build_moby_bone_transform((char *)hit_moby, D_L16_001619C8_gp, (char *)matrix);
        copy_matrix3x4(hit_data->child->matrix[0], matrix);
        fast_vec_normalize(hit_data->child->matrix[0], hit_data->child->matrix[0], 1.0f);
        fast_vec_normalize(hit_data->child->matrix[1], hit_data->child->matrix[1], 1.0f);
        fast_vec_normalize(hit_data->child->matrix[2], hit_data->child->matrix[2], 1.0f);
        update_entity(hit_data->child);
    }
    trooper_anim_finish(hit_moby, hit_data->reaction);
}

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D00E8), where it is exact; names translated to the US level program. */

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

int move_test_dummy_toward_position_and_heading(char *moby, void *target_position, float angle) {
    float target_position_copy[4] __attribute__((aligned(16)));
    float movement_delta[4] __attribute__((aligned(16)));
    float speed;
    float distance_to_target;
    char *data = ((char *)MOBY(moby)->pvars);
    *(OvlQuad *)target_position_copy = *(OvlQuad *)target_position;
    approach_angle((float *)&MOBY(moby)->rot.z,
                   (float *)(data + 0x100),
                   angle,
                   frame_time_sq * 12.566371f,
                   frame_time_sq * 12.566371f,
                   D_0015ED6C * 25.132742f);
    speed = 0.0f;
    fast_vec_sub(movement_delta, target_position_copy, moby + 0x10);
    distance_to_target = fast_vec_length(movement_delta);
    advance_accelerated_scalar(distance_to_target,
                               frame_time_sq * 12.0f,
                               frame_time_sq * 12.0f,
                               D_0015ED6C * 6.0f,
                               &speed,
                               (float *)(data + 0xFC));
    fast_vec_normalize(movement_delta, movement_delta, *(float *)(data + 0xFC));
    fast_vec_add(moby + 0x10, moby + 0x10, movement_delta);
    if (distance_to_target < 0.05f) {
        if (*(float *)(data + 0xFC) < 0.005f)
            return 1;
        else
            return 0;
    }
    return 0;
}
/* spins the level's rotating parts by the current angle and queues the draw callback */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0238), where it is exact; names translated to the US level program. */

extern int *D_L16_001ABCC0_c2[] __asm__("D_L16_001ABCC0");
extern void draw_test_dummy_group_glows() __asm__("FUN_L16_002cef60");

void update_test_dummy_group_glow_phases(struct Moby *moby) __asm__("FUN_L16_002cee70");

void update_test_dummy_group_glow_phases(struct Moby *moby) {
    short *group_entry;
    float  angle_increment;
    char  *group_moby;
    char  *data;
    if (moby->group != 0xFF) {
        angle_increment = test_dummy_glow_phase_rate_degrees * DEG_TO_RAD * D_0015ED6C;
        group_entry = (short *)D_L16_001ABCC0_c2[moby->group];
        do {
        again:
            group_moby =
                (char *)(((*(unsigned short *)group_entry & 0x7FFF) << 8) + (int)D_L16_0015FFD8);
            if (MOBY(group_moby)->oclass != 0x21D)
                goto again;
            data = ((char *)MOBY(group_moby)->pvars);
            *(int *)(data + 0x118) = D_L16_0015F5CC;
            *(float *)(data + 0x11C) =
                fast_add_rotations(*(float *)(data + 0x11C), angle_increment);
        } while (*group_entry++ >= 0);
        add_draw_callback(draw_test_dummy_group_glows, moby);
    }
}
/* Draw callback queued by FUN_L16_002cee70: one camera-facing glow quad on each live part (class 541) of the group, its colour pulsing with the part's spin angle. */

typedef struct {
    float v[4][4];
    int   rgba[4];
    struct {
        float x, y;
    } uv[4];
    long z70;
    long tex;
    long q80;
    long alpha;
} GlowQuad;

float D_L16_001D3130[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

extern int  test_dummy_glow_pulse_color_start_gp __asm__("D_L16_001619E0") __attribute__((sda));
extern int  test_dummy_glow_pulse_color_end_gp __asm__("D_L16_001619E4") __attribute__((sda));
extern long get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad_alt(void *, void *, int) __asm__("FUN_001f7d30");

void draw_test_dummy_group_glows(char *moby) {
    GlowQuad glow_quad;
    float    glow_facing_transform[4][4] __attribute__((aligned(16)));
    short   *group_entry;
    char    *group_moby;
    int      color;
    int      i;

    glow_quad.tex = get_effect_texture_alt(0xB);
    glow_quad.alpha = 0x8000000048L;
    glow_quad.q80 = 0xFF9000000260L;
    glow_quad.z70 = 5;
    glow_quad.uv[0].x = 0.0f;
    glow_quad.uv[0].y = 0.0f;
    glow_quad.uv[1].x = 0.0f;
    glow_quad.uv[1].y = 1.0f;
    glow_quad.uv[2].x = 1.0f;
    glow_quad.uv[2].y = 0.0f;
    glow_quad.uv[3].x = 1.0f;
    glow_quad.uv[3].y = 1.0f;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(glow_quad.v[i], D_L16_001D3130[i], 0.2f);
    }
    group_entry = (short *)D_L16_001ABCC0_c2[((unsigned char *)moby)[0x21]];
    for (;; group_entry++) {
        group_moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        color = fast_tween_color(
            test_dummy_glow_pulse_color_start_gp,
            test_dummy_glow_pulse_color_end_gp,
            (fast_sin(*(float *)(((char *)MOBY(group_moby)->pvars) + 0x11C)) + 1.0f) * 0.5f);
        glow_quad.rgba[3] = color;
        glow_quad.rgba[2] = color;
        glow_quad.rgba[1] = color;
        glow_quad.rgba[0] = color;
        if (group_moby != 0 && MOBY(group_moby)->oclass == 0x21D &&
            (unsigned char)group_moby[0x20] != 0xFE && (unsigned char)group_moby[0x20] != 0xFD) {
            /* basis facing the camera, origin just above the part */
            qcopy(glow_facing_transform[3], group_moby + 0x10);
            glow_facing_transform[3][2] += 0.2f;
            glow_facing_transform[3][3] = 1.0f;
            fast_vec_sub(glow_facing_transform[0], D_L16_001671C0, glow_facing_transform[3]);
            fast_vec_normalize(glow_facing_transform[0], glow_facing_transform[0], 1.0f);
            fast_vec_cross(
                glow_facing_transform[1], glow_facing_transform[0], (u8 *)&hero.gravity_dir);
            fast_vec_normalize(glow_facing_transform[1], glow_facing_transform[1], -1.0f);
            fast_vec_cross(
                glow_facing_transform[2], glow_facing_transform[1], glow_facing_transform[0]);
            draw_geometry_quad_alt(&glow_quad, glow_facing_transform, 0);
        }
        /* a negative entry ends the group */
        if (*group_entry < 0)
            return;
    }
}

/* Pulse a linked control object and toggle its child when hit or its timer expires. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0560), where it is exact; names translated to the US level program. */

extern float         fast_add_rotations(float, float);
extern float         fast_sin(float);
extern int           scale_game_frames(int);
extern int           truncate_float_to_s32(float);
extern unsigned char visited52_lui[] __asm__("D_0014C050") NOT_SDA;
extern void          set_class_056b_collision_enabled(void *, int) __asm__("FUN_L16_002e3770");

void wall_button_update(struct Moby *m) __asm__("FUN_L16_002cf198");

void wall_button_update(struct Moby *moby) {
    enum {
        WALL_BUTTON_STATE_INITIALIZE = 0,
        WALL_BUTTON_STATE_WAIT_FOR_HIT = 1,
        WALL_BUTTON_STATE_ACTIVATED = 2
    };
    char *wall_button_data = (char *)moby->pvars;
    switch ((*(unsigned char *)((char *)(moby) + (0x20)))) {
    case WALL_BUTTON_STATE_INITIALIZE:
        (*(float *)((char *)(wall_button_data) + (0x68))) = random_angle_radians();
        (*(float *)((char *)(wall_button_data) + (0x30))) = 0.001f;
        if ((*(unsigned char *)((char *)(((char *)&(*(short *)&visited52_lui)) +
                                         (current_level_index << 4)) +
                                ((*(unsigned char *)((char *)(moby) + (0xB0)))))) == 255 &&
            (*(int *)((char *)(wall_button_data) + (0x64))) == -1) {
            (*(unsigned char *)((char *)(moby) + (0xBC))) = 1;
            (*(unsigned char *)((char *)(moby) + (0x20))) = 2;
            if ((*(int *)((char *)(wall_button_data) + (0x60))) != -1) {
                set_class_056b_collision_enabled(
                    D_L16_0015FFD8 + ((*(int *)((char *)(wall_button_data) + (0x60))) << 8), 0);
            }
        } else
            (*(unsigned char *)((char *)(moby) + (0x20))) = 1;
        break;
    case WALL_BUTTON_STATE_WAIT_FOR_HIT: {
        int color;
        (*(float *)((char *)(wall_button_data) + (0x68))) = fast_add_rotations(
            (*(float *)((char *)(wall_button_data) + (0x68))), D_0015ED6C * 6.2831855f);
        color = truncate_float_to_s32(
            (fast_sin((*(float *)((char *)(wall_button_data) + (0x68)))) * 4.0f - 3.0f) * 128.0f);
        if (color > 128)
            color = 128;
        else if (color < 32)
            color = 32;
        (*(int *)((char *)(moby) + (0x90))) = 0x80000000 | (color << 16) | (color << 8) | color;
        if (find_moby_hit(moby, 0x330000, 0)) {
            (*(unsigned char *)((char *)(moby) + (0x20))) = 2;
            (*(int *)((char *)(moby) + (0x90))) = 0x80208020;
            (*(unsigned char *)((char *)(moby) + (0xBC))) = 1;
            (*(float *)((char *)(wall_button_data) + (0x68))) = 1.5707964f;
            func_0022ED80_i(0, 0, moby);
            if ((*(int *)((char *)(wall_button_data) + (0x60))) != -1)
                set_class_056b_collision_enabled(
                    D_L16_0015FFD8 + ((*(int *)((char *)(wall_button_data) + (0x60))) << 8), 0);
            if ((*(int *)((char *)(wall_button_data) + (0x64))) > 0)
                (*(int *)((char *)(wall_button_data) + (0x6C))) =
                    scale_game_frames((*(int *)((char *)(wall_button_data) + (0x64))));
        }
        break;
    }
    case WALL_BUTTON_STATE_ACTIVATED: {
        int color = truncate_float_to_s32(
            (fast_sin((*(float *)((char *)(wall_button_data) + (0x68)))) * 0.5f + 0.5f) * 128.0f);
        if (color > 128)
            color = 128;
        else if (color < 32)
            color = 32;
        (*(int *)((char *)(moby) + (0x90))) = 0x80200020 | (color << 8);
        if ((*(int *)((char *)(wall_button_data) + (0x64))) > 0) {
            if (fast_dec_timer((int *)(wall_button_data + 0x6C))) {
                (*(unsigned char *)((char *)(moby) + (0xBC))) = 0;
                (*(unsigned char *)((char *)(moby) + (0x20))) = 1;
                (*(float *)((char *)(wall_button_data) + (0x68))) = 1.5707964f;
                if ((*(int *)((char *)(wall_button_data) + (0x60))) != -1) {
                    set_class_056b_collision_enabled(
                        D_L16_0015FFD8 + ((*(int *)((char *)(wall_button_data) + (0x60))) << 8), 1);
                }
            } else if ((*(int *)((char *)(wall_button_data) + (0x6C))) < scale_game_frames(90))
                (*(float *)((char *)(wall_button_data) + (0x68))) = fast_add_rotations(
                    (*(float *)((char *)(wall_button_data) + (0x68))), D_0015ED6C * 25.132742f);
        }
        break;
    }
    }
    (*(unsigned char *)((char *)(moby) + (0xA4))) = 255;
}
/* first other moby of type 0x228 in m's list whose 0x18 is within 0.1 of m's */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0A40), where it is exact; names translated to the US level program. */

extern short *D_L16_001ABFC0_2d0a40[] __asm__("D_L16_001ABCC0");

int find_linked_pair_partner_by_height(struct Moby *moby) __asm__("FUN_L16_002cf678");

int find_linked_pair_partner_by_height(struct Moby *moby) {
    short *group_entry = D_L16_001ABFC0_2d0a40[moby->group];
    if (group_entry == 0)
        return 0;
    while (1) {
        char *candidate_moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(candidate_moby)->oclass == 0x228 && moby != candidate_moby) {
            if (absolute_float(MOBY(candidate_moby)->pos.z - moby->pos.z) < 0.1f) {
                return (int)candidate_moby;
            }
        }
        if (*group_entry++ < 0)
            return 0;
    }
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0B00), where it is exact; names translated to the US level program. */

extern int *D_L16_001ABCC0[];

int is_linked_pair_effect_enabled(void *mv) {
    struct Moby    *moby = mv;
    unsigned short *group_entry = (unsigned short *)D_L16_001ABCC0[moby->group];
    if (group_entry == 0)
        return 1;
    for (;;) {
        unsigned char *other = D_L16_0015FFD8 + ((*group_entry & 0x7FFF) << 8);
        if (*(short *)(other + 0xA6) == 0x222) {
            if (other[0xBC] != 0)
                return 0;
        }
        if ((short)*group_entry++ < 0)
            return 1;
    }
}
/* Draw each linked pair once per generation. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0B70), where it is exact; names translated to the US level program. */

extern int *D_L16_001ABCC0_c3[] __asm__("D_L16_001ABCC0");
void        draw_linked_pair_ribbons(char *moby, char *partner) __asm__("FUN_L16_002cf850");

void draw_linked_pair_group_ribbons(struct Moby *moby) __asm__("FUN_L16_002cf7a8");

void draw_linked_pair_group_ribbons(struct Moby *moby) {
    short *group_entry = (short *)D_L16_001ABCC0_c3[moby->group];
    if (group_entry) {
        do {
            char *group_moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
            if (MOBY(group_moby)->oclass == 0x228) {
                char *data = ((char *)MOBY(group_moby)->pvars);
                int   generation = D_L16_0015F5CC;
                if (*(int *)(data + 4) != generation) {
                    char *partner = *(char **)data;
                    if (partner) {
                        char *otherData = *(char **)(partner + 0x78);
                        *(int *)(data + 4) = generation;
                        *(int *)(otherData + 4) = generation;
                        draw_linked_pair_ribbons(group_moby, partner);
                    }
                }
            }
        } while (*group_entry++ >= 0);
    }
}

#include "qcopy.h"

/* Draws the textured quad that links a moby to its partner, four times, each one a step higher than the last. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0C18), where it is exact; names translated to the US level program. */

typedef struct {
    float point[4][4];
    int   color[4];
    struct {
        float u, v;
    } uv[4];
    long zero, texture, flags, mode;
} L16RibbonPacket;

extern float linked_pair_ribbon_vertex_positions[][4] __asm__("D_L16_001D3170");
extern float linked_pair_ribbon_uvs[][2] __asm__("D_L16_001D31B0");
extern int   linked_pair_ribbon_alpha_blend_b_selector __asm__("D_L16_001619EC")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_alpha_blend_c_selector __asm__("D_L16_001619F0")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_alpha_blend_a_selector __asm__("D_L16_001619E8")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_alpha_blend_d_selector __asm__("D_L16_001619F4")
    __attribute__((sda)); /* no foreign declaration */
extern float linked_pair_ribbon_origin_z_offset __asm__("D_L16_001619F8")
    __attribute__((sda)); /* no foreign declaration */
extern float linked_pair_ribbon_pass_z_step __asm__("D_L16_001619FC")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_texture_id __asm__("D_L16_00161A00")
    __attribute__((sda)); /* no foreign declaration */
extern int linked_pair_ribbon_vertex_color __asm__("D_L16_00161A04")
    __attribute__((sda)); /* no foreign declaration */
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

/* Draws a four-pass textured ribbon quad from the moby toward its partner moby. */
void draw_linked_pair_ribbons(char *moby, char *partner) {
    L16RibbonPacket packet;
    float           frame[4][4]; /* direction, side, up, origin */
    int             color;
    int             corner;
    int             pass;

    fast_vec_sub(frame[0], partner + 0x10, moby + 0x10);
    clear_u64_value(frame[2]);
    frame[2][2] = 1.0f;
    fast_vec_cross(frame[1], frame[0], frame[2]);
    qcopy(frame[3], moby + 0x10);
    frame[3][2] += linked_pair_ribbon_origin_z_offset;
    frame[3][3] = 1.0f;
    packet.texture = get_effect_texture(linked_pair_ribbon_texture_id);
    packet.mode = (long)linked_pair_ribbon_alpha_blend_a_selector |
                  ((long)linked_pair_ribbon_alpha_blend_b_selector << 2) |
                  ((long)linked_pair_ribbon_alpha_blend_c_selector << 4) |
                  ((long)linked_pair_ribbon_alpha_blend_d_selector << 6) | 0x8000000000L;
    packet.flags = 0xFF9000000260L;
    packet.zero = 0;
    color = linked_pair_ribbon_vertex_color;
    for (corner = 0; corner < 4; corner++) {
        packet.uv[corner].u = linked_pair_ribbon_uvs[corner][0];
        packet.uv[corner].v = linked_pair_ribbon_uvs[corner][1];
        packet.color[corner] = color;
        qcopy(packet.point[corner], linked_pair_ribbon_vertex_positions[corner]);
    }
    for (pass = 0; pass < 4; pass++) {
        draw_geometry_quad(&packet, frame[0], 0);
        frame[3][2] += linked_pair_ribbon_pass_z_step;
    }
}

/* Choose the path node to steer for: step on while the current one is behind the moby or reached. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D0DC0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x2C0];
    char *vectors;
} Level16VendorVectorData_c;

typedef struct {
    char                       pad0[0x78];
    Level16VendorVectorData_c *data;
} Level16VendorVectorMoby_c;

typedef struct {
    int   count; /* 0x00: number of nodes */
    char  pad04[0xC];
    float points[1][4]; /* 0x10: position; w marks the node */
} L16PatrolPath;

typedef struct {
    char           pad0[0x260];
    float          position[4]; /* 0x260 */
    char           pad270[0x20];
    int            base_path;      /* 0x290: index into D_L16_001B0C30 */
    int            branch_path[7]; /* 0x294: indexed by a node's mark - 11 */
    char           pad2B0[0x10];
    L16PatrolPath *path; /* 0x2C0: the path being followed */
    char           pad2C4[0x30];
    short          node; /* 0x2F4: the node being steered for */
} L16PatrolData;

typedef struct {
    char           pad0[0x10];
    float          position[4]; /* 0x10 */
    char           pad20[0x28];
    float          yaw; /* 0x48 */
    char           pad4C[0x2C];
    L16PatrolData *data; /* 0x78 */
} L16PatrolMoby;

/* Patrol step: advances the moby's path node until the next node is ahead and far enough, switching between the base path and its branches. */
void advance_hoverboard_dummy_path_node(char *patrol_moby) __asm__("FUN_L16_002cf9f8");

void advance_hoverboard_dummy_path_node(char *patrol_moby) {
    L16PatrolMoby *moby = (L16PatrolMoby *)patrol_moby;
    L16PatrolData *patrol_data = moby->data;
    float          target[4];
    float          heading, distance;

    copy_hoverboard_dummy_path_node((Level16VendorVectorMoby_c *)moby, patrol_data->node, target);
    heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    for (;;) {
        L16PatrolPath *path;
        L16PatrolPath *base;

        copy_hoverboard_dummy_path_node(
            (Level16VendorVectorMoby_c *)moby, patrol_data->node, target);
        distance = vector_distance_xy(target, patrol_data->position);
        if ((fast_difference_between_rotations(heading, moby->yaw) < 1.5707964f ||
             distance > 4.0f) &&
            distance > 2.0f) {
            break;
        }
        path = patrol_data->path;
        patrol_data->node = (patrol_data->node + 1) % path->count;
        base = (L16PatrolPath *)D_L16_001B0930[patrol_data->base_path];
        if (path != base) {
            /* At the last node of a branch: back to the base path, five nodes past its nearest one. */
            if (patrol_data->node == path->count - 1) {
                patrol_data->path = base;
                patrol_data->node =
                    select_point_index_by_distance(patrol_data->position, base, 0.0f);
                patrol_data->node =
                    offset_path_index((char *)patrol_data->path, patrol_data->node, 5, 1);
            }
        } else if (path->points[patrol_data->node][3] > 10.0f && random_integer_below(100) >= 31) {
            /* At a marked node of the base path: more often than not, onto the branch the mark names. */
            int branch =
                truncate_float_to_s32(patrol_data->path->points[patrol_data->node][3]) - 11;

            patrol_data->path = (L16PatrolPath *)D_L16_001B0930[patrol_data->branch_path[branch]];
            patrol_data->node = 0;
        }
        /* The same two statements as before the loop, written out again: the compiler merges the copies. */
        copy_hoverboard_dummy_path_node(
            (Level16VendorVectorMoby_c *)moby, patrol_data->node, target);
        heading = fast_arc_tan(target[0] - moby->position[0], target[1] - moby->position[1]);
    }
}
/* Adjust animation and pitch to the remaining time in a jump. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D1420), where it is exact; names translated to the US level program. */

typedef struct {
    char  header[0x10];
    float points[1][4];
} L16LeapPath;

typedef struct {
    char  pad0[0x268];
    float height;
    char  pad26C[0xC];
    float target_height;
    char  pad27C[0x44];
    char *path;
    char  pad2C4[4];
    float vertical_speed;
    char  pad2CC[0x18];
    int   timer;
    char  pad2E8[0xC];
    short node;
    char  pad2F6[2];
    float pitch_speed;
    char  pad2FC[2];
    short sequence;
} L16LeapData_u;

typedef struct {
    char           pad0[0x44];
    float          pitch;
    char           pad48[0xA];
    unsigned char  animation, next_animation;
    char           pad54[4];
    float          animation_speed;
    char           pad5C[0x14];
    unsigned char  flags;
    char           pad71[7];
    L16LeapData_u *data;
} L16LeapMoby_u;

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
f32        compute_interpolated_record_value_u(struct InterpolatedStateEntry *arg0) __asm__("FUN_0020c9e0");
void        mine_blend_color(void) __asm__("FUN_L00_002371e0");
extern void func_L00_00237B70_anim(void *, int, int, int) __asm__("FUN_L00_002371e0");

void update_hoverboard_dummy_jump_animation_and_pitch(L16LeapMoby_u *leap_moby) __asm__("FUN_L16_002d0058");

void update_hoverboard_dummy_jump_animation_and_pitch(L16LeapMoby_u *leap_moby) {
    float          root0, root1;
    float          pitch;
    L16LeapData_u *leap_data = leap_moby->data;
    pitch = 0.0f;
    if (leap_data->timer == 0) {
        int   next;
        float distance;
        next = offset_path_index(leap_data->path, leap_data->node, 1, 1) * 16;
        distance = vector_distance_xy(leap_data->path + (leap_data->node * 16 + 0x10),
                                      leap_data->path + (next + 0x10));
        pitch = fast_arc_tan(distance,
                             *(float *)(leap_data->path + next + 0x18) -
                                 ((L16LeapPath *)leap_data->path)->points[leap_data->node][2]);
    }
    spring_angle(&leap_moby->pitch,
                 &leap_data->pitch_speed,
                 0,
                 pitch,
                 frame_scale_sq * 0.02f,
                 frame_scale_sq * 0.3f,
                 D_0015ED6C * 3.14159265f);
    if (leap_data->timer != 0) {
        float duration;
        float acceleration;
        if (leap_moby->next_animation != 5 && scale_game_frames(3) > leap_data->timer) {
            func_L00_00237B70_anim(leap_moby, 5, 4, scale_game_frames(5));
            leap_data->sequence = -1;
        }
        acceleration = frame_time_sq * 21.0f * -0.5f;
        duration = 30.0f;
        if (solve_quadratic_equation(&root0,
                                     &root1,
                                     acceleration,
                                     leap_data->vertical_speed - acceleration,
                                     leap_data->height - leap_data->target_height) > 0 &&
            root0 > 0.0f) {
            duration = truncate_float_to_s32(root0);
        }
        if (scale_game_frames(7) < leap_data->timer && scale_game_frames(11) > leap_data->timer &&
            leap_moby->next_animation == 5 && (float)scale_game_frames(27) < duration) {
            short sequence;
            int   chosen;
            chosen = random_int_between(0, 3) + 1;
            leap_data->sequence = chosen;
            sequence = chosen;
            func_L00_00237B70_anim(leap_moby, sequence, 3, scale_game_frames(7));
        }
        if (leap_data->sequence != -1) {
            if (leap_moby->animation == leap_moby->next_animation) {
                float frame = compute_interpolated_record_value_u(leap_moby);
                if (frame > 7.0f && frame < 20.0f && duration != 0.0f) {
                    float speed = 30.0f / duration;
                    if (speed > 1.7f)
                        speed = 1.7f;
                    if (speed < 0.3f)
                        speed = 0.3f;
                    leap_moby->animation_speed = speed;
                }
            }
            if (leap_data->sequence != -1) {
                if (duration < (float)scale_game_frames(5)) {
                    func_L00_00237B70_anim(leap_moby, 5, 0x33, scale_game_frames(5));
                    leap_data->sequence = -1;
                } else if (leap_moby->flags & 2) {
                    func_L00_00237B70_anim(leap_moby, 5, 0x1C, scale_game_frames(7));
                    leap_data->sequence = -1;
                }
            }
        }
        if (leap_moby->animation == leap_moby->next_animation && leap_moby->animation == 5 &&
            duration != 0.0f) {
            float remaining;
            remaining = 50.0f;
            remaining -= compute_interpolated_record_value_u(leap_moby);
            remaining /= duration;
            leap_moby->animation_speed = remaining;
        }
    } else {
        leap_moby->animation_speed = 1.0f;
        if ((leap_moby->flags & 2) && leap_moby->next_animation != 0) {
            func_L00_00237B70_anim(leap_moby, 0, 0, scale_game_frames(8));
        }
    }
}

#include "qcopy.h"

/* Update the hoverboard race competitor: wait for the race, follow the path, leap, and emit spray. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D1868), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x2C0];
    char *vectors;
} Level16VendorVectorData;

typedef struct {
    char                     pad0[0x78];
    Level16VendorVectorData *data;
} Level16VendorVectorMoby;

typedef struct {
    char  pad0[0x268];
    float height;
    char  pad26C[0xC];
    float target_height;
    char  pad27C[0x44];
    char *path;
    char  pad2C4[4];
    float vertical_speed;
    char  pad2CC[0x18];
    int   timer;
    char  pad2E8[0xC];
    short node;
    char  pad2F6[2];
    float pitch_speed;
    char  pad2FC[2];
    short sequence;
} L16LeapData;

typedef struct {
    char          pad0[0x44];
    float         pitch;
    char          pad48[0xA];
    unsigned char animation, next_animation;
    char          pad54[4];
    float         animation_speed;
    char          pad5C[0x14];
    unsigned char flags;
    char          pad71[7];
    L16LeapData  *data;
} L16LeapMoby;

typedef struct {
    char  pad0[0x140];
    float position[4];
    char  pad150[8];
    float yaw;
} L16RacePlayer;

extern L16RacePlayer D_L16_0015F580 MACRO_ADDR;
extern int                         *D_L16_00178300[];
extern float                        approach_value(float *p, float target, float maxstep);
extern int                          D_00159F4C; /* no foreign declaration */
extern int                          D_L16_00161A0C __attribute__((sda));
extern int                          D_0015EE3C_gp __asm__("D_0015EE3C") __attribute__((sda));
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int is_moby_in_class_range_500_to_540(int) __asm__("FUN_L00_0025e3b8");
extern int add_moby_frames_for_group_proximity() __asm__("FUN_L05_00307010");
extern unsigned char D_0013D4DF NOT_SDA;
extern void
spawn_nearby_effect(void *, int, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
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
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
void        advance_hoverboard_dummy_path_node(char *m) __asm__("FUN_L16_002cf9f8");
void        FUN_L16_002cf9d0_c(Level16VendorVectorMoby *moby,
                               int                      index,
                               void                    *out) __asm__("FUN_L16_002cf9d0");
void        FUN_L16_002d0058_c(L16LeapMoby *m) __asm__("FUN_L16_002d0058");

extern int hoverboard_dummy_spray_initial_color_gp __asm__("D_L16_00161A08") __attribute__((sda));
void       test_dummy_on_hoverboard_update(unsigned char *m) __asm__("FUN_L16_002d04a0");

void test_dummy_on_hoverboard_update(unsigned char *moby) {
    enum {
        HOVERBOARD_DUMMY_STATE_INITIALIZE = 0,
        HOVERBOARD_DUMMY_STATE_WAIT_FOR_RACE = 1,
        HOVERBOARD_DUMMY_STATE_RACING = 2,
        HOVERBOARD_DUMMY_STATE_WAIT_FOR_REACTIVATION = 3
    };
    float path_node_or_explosion_position[4], delta[4], ahead[4], impact[4], spray[4], lateral[4],
        dust[4];
    char *hoverboard_dummy_data = ((char *)MOBY(moby)->pvars);
    char *hero_base;
    int   i, nearby_moby_count, next;
    /* One mask for both collision calls. Held in a local it is one more instruction when GCSE sizes its
       expression table, and that size orders the stack slots of the cached addresses (sp+0x90..0xC4):
       with the literal at both calls every one of those slots is rotated by three. */
    int   mask = 0x30000;
    float distance, duration, target, z, gap, speed, angle, scale, rate, limit, offset, slow;

    /* No pointer locals: every address is written where it is used. The compiler caches them itself
       (d + 0x1E0 from here on, the others at the top of the active state). */
    manipulator_set_scale(hoverboard_dummy_data + 0x1E0, 2.5f);
    switch (moby[0x20]) {
    case HOVERBOARD_DUMMY_STATE_INITIALIZE: {
        float time;
        char *path;

        moby[0x30] = 24;
        MOBY(moby)->flags |= 0x41;
        path = D_L16_001B0930[*(int *)(hoverboard_dummy_data + 0x2B0)];
        *(char **)(hoverboard_dummy_data + 0x2C0) = path;
        FUN_L16_002cf9d0_c((Level16VendorVectorMoby *)moby, 0, path_node_or_explosion_position);
        qcopy(moby + 0x10, path_node_or_explosion_position);
        qcopy(hoverboard_dummy_data + 0x260, moby + 0x10);
        *(int *)(moby + 0x40) = 0;
        *(int *)(moby + 0x44) = 0;
        MOBY(moby)->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                         *(float *)(path + 0x24) - *(float *)(path + 0x14));
        *(unsigned char *)(hoverboard_dummy_data + 0x2B) = 1;
        time = D_0015ED6C;
        offset = (float)*(int *)(hoverboard_dummy_data + 0x2CC) * (time * 1.55f);
        *(float *)(hoverboard_dummy_data + 0x2B4) = time * 17.0f + offset;
        *(float *)(hoverboard_dummy_data + 0x2B8) = time * 24.0f + offset;
        if (D_0013D4DF == 0) {
            slow = (float)(D_0015EE3C_gp - 5);
            if (slow < 0.0f) {
                slow = 0.0f;
            }
            slow = 1.0f - slow * 0.008f;
            if (slow < 0.8f) {
                slow = 0.8f;
            }
            *(float *)(hoverboard_dummy_data + 0x2B4) *= slow;
            *(float *)(hoverboard_dummy_data + 0x2B8) *= slow;
        }
        *(float *)(hoverboard_dummy_data + 0x2E8) = random_angle_radians();
        *(unsigned char *)(hoverboard_dummy_data + 0x29) = 0;
        moby[0x20] = 1;
        break;
    }
    case HOVERBOARD_DUMMY_STATE_WAIT_FOR_RACE:
        if (hero.state.control_mode == 22) {
            moby[0x30] = 255;
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            MOBY(moby)->flags &= ~0x41;
            *(short *)(hoverboard_dummy_data + 0x2F4) = 0;
            *(short *)(hoverboard_dummy_data + 0x2F6) = scale_game_frames(180);
            *(int *)(hoverboard_dummy_data + 0x2C8) = 0;
            *(int *)(hoverboard_dummy_data + 0x2C4) = 0;
            *(int *)(hoverboard_dummy_data + 0x2E4) = 0;
            *(int *)(hoverboard_dummy_data + 0x2DC) = 0;
            *(int *)(hoverboard_dummy_data + 0x2BC) = 0;
            if (*(int *)(hoverboard_dummy_data + 0x2CC) % 2 == 1) {
                MOBY(moby)->flags |= 0x8000;
            }
            moby[0xA4] = 255;
            moby[0x20] = 2;
        }
        break;
    case HOVERBOARD_DUMMY_STATE_WAIT_FOR_REACTIVATION:
        if (hero.state.control_mode != 22) {
            moby[0x20] = 0;
        } else {
            int   count = *(unsigned short *)(hoverboard_dummy_data + 0x2F2) + 1;
            short timer = count;

            *(short *)(hoverboard_dummy_data + 0x2F2) = count;
            if (scale_game_frames(240) < timer) {
                LevelCamera *player = &l16_camera;

                if (fast_difference_between_rotations(
                        fast_arc_tan(MOBY(moby)->pos.x - player->pos.f[0],
                                     MOBY(moby)->pos.y - player->pos.f[1]),
                        player->yaw) > 1.5707964f) {
                    MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
                    MOBY(moby)->flags &= ~0x41;
                    moby[0xA4] = 255;
                    moby[0x20] = 2;
                }
            }
        }
        break;
    case HOVERBOARD_DUMMY_STATE_RACING:
        hero_base = ((char *)&hero);
        if (*(int *)(hero_base + 0x208C) != 22) {
            moby[0x20] = 0;
            break;
        }
        if (find_moby_hit(moby, mask, 0)) {
            qcopy(path_node_or_explosion_position, moby + 0x10);
            path_node_or_explosion_position[2] += 0.5f;
            spawn_explosion(moby,
                            &D_L16_0015F580,
                            path_node_or_explosion_position,
                            0.0f,
                            0.0f,
                            5,
                            15,
                            25,
                            2.5f,
                            1.5f,
                            4.0f,
                            -1,
                            1.0f,
                            7.0f,
                            0,
                            9,
                            -1,
                            0);
            spawn_moby_break_fragments(moby, 0x655, 1, 0x656, 2, 2, 0);
            spawn_moby_break_fragments(moby, 0x6FA, 1, 0x6FB, 1, 1, 0);
            *(short *)(hoverboard_dummy_data + 0x2F2) = 0;
            *(int *)(hero_base + 0x8A8) += 250;
            moby[0x20] = 3;
            MOBY(moby)->collision = 0;
            MOBY(moby)->flags |= 0x41;
            break;
        }
        moby[0xA4] = 255;
        advance_hoverboard_dummy_path_node((char *)moby);
        add_moby_frames_for_group_proximity((char *)moby);
        prepare_hoverboard_dummy_path_jump((char *)moby);
        if (*(int *)(hoverboard_dummy_data + 0x2E4) != 0) {
            (*(int *)(hoverboard_dummy_data + 0x2E4))++;
            if (*(int *)(hoverboard_dummy_data + 0x2E4) < scale_game_frames(17)) {
                i = *(short *)(hoverboard_dummy_data + 0x2F4);
                distance = 0.0f;
                while (i < *(short *)(hoverboard_dummy_data + 0x2F0)) {
                    char *path;

                    next = offset_path_index(*(char **)(hoverboard_dummy_data + 0x2C0), i, 1, 1);
                    path = *(char **)(hoverboard_dummy_data + 0x2C0);
                    distance += vector_distance_xy(path + (i * 16 + 16), path + (next * 16 + 16));
                    i = next;
                }
                {
                    short last = *(short *)(hoverboard_dummy_data + 0x2F0);
                    short first = *(short *)(hoverboard_dummy_data + 0x2F4);
                    char *path = *(char **)(hoverboard_dummy_data + 0x2C0);
                    float rise = *(float *)(path - (-(last * 16)) + 0x18) -
                                 *(float *)(path - (-(first * 16)) + 0x18);

                    duration = distance / *(float *)(hoverboard_dummy_data + 0x2BC);
                    target = rise / duration + frame_time_sq * 21.0f * duration * 0.5f;
                }
                if (*(float *)(hoverboard_dummy_data + 0x2C8) < 0.0f) {
                    *(float *)(hoverboard_dummy_data + 0x2C8) = 0.0f;
                }
                approach_value(
                    (float *)(hoverboard_dummy_data + 0x2C8), target, frame_time_sq * 110.0f);
            }
        }
        FUN_L16_002d0058_c((L16LeapMoby *)moby);
        FUN_L16_002cf9d0_c((Level16VendorVectorMoby *)moby,
                           *(short *)(hoverboard_dummy_data + 0x2F4),
                           path_node_or_explosion_position);
        angle = fast_arc_tan(path_node_or_explosion_position[0] - MOBY(moby)->pos.x,
                             path_node_or_explosion_position[1] - MOBY(moby)->pos.y);
        spring_angle(moby + 0x48,
                     hoverboard_dummy_data + 0x2C4,
                     0,
                     angle,
                     frame_scale_sq * 0.035f,
                     frame_scale_sq * 0.3f,
                     D_0015ED6C * 6.981317f);
        if (*(float *)(hoverboard_dummy_data + 0x268) < path_node_or_explosion_position[2] - 5.0f) {
            *(float *)(hoverboard_dummy_data + 0x268) = path_node_or_explosion_position[2];
        }
        delta[0] = fast_cos(MOBY(moby)->rot.z) * *(float *)(hoverboard_dummy_data + 0x2BC);
        delta[1] = fast_sin(MOBY(moby)->rot.z) * *(float *)(hoverboard_dummy_data + 0x2BC);
        delta[2] = 0.0f;
        fast_vec_add(hoverboard_dummy_data + 0x260, hoverboard_dummy_data + 0x260, delta);
        fast_vec_scale(ahead, delta, 3.0f);
        fast_vec_add(ahead, ahead, hoverboard_dummy_data + 0x260);
        ahead[2] += 0.7f;
        nearby_moby_count = collect_mobies_in_sphere(0.7f, ahead, 0, moby, 0);
        if (nearby_moby_count) {
            fast_vec_scale(impact, delta, 7.0f);
            for (i = 0; i < nearby_moby_count; i++) {
                if (is_moby_in_class_range_500_to_540((int)D_L16_00178300[i])) {
                    spawn_nearby_effect(D_L16_00178300[i],
                                        (int)moby,
                                        mask,
                                        hoverboard_dummy_data + 0x260,
                                        impact,
                                        1.0f);
                }
            }
        }
        z = probe_ground_height(hoverboard_dummy_data + 0x260, 0, 0.5f);
        gap = *(float *)(hoverboard_dummy_data + 0x268) - z;
        if (gap > 0.0f || *(float *)(hoverboard_dummy_data + 0x2C8) > 0.0f) {
            *(float *)(hoverboard_dummy_data + 0x2C8) -= frame_time_sq * 21.0f;
        } else if (absolute_float(gap) < 0.5f) {
            *(float *)(hoverboard_dummy_data + 0x2C8) = 0.0f;
            *(float *)(hoverboard_dummy_data + 0x268) = z;
            *(int *)(hoverboard_dummy_data + 0x2E4) = 0;
        }
        *(float *)(hoverboard_dummy_data + 0x268) += *(float *)(hoverboard_dummy_data + 0x2C8);
        if (fast_dec_timer_s16(hoverboard_dummy_data + 0x2F6)) {
            speed = *(float *)(hoverboard_dummy_data + 0x2B4);
            if (z < *(float *)(hoverboard_dummy_data + 0x268)) {
                speed = D_0015ED6C * 19.0f;
            }
            hero_base = ((char *)&hero);
            if (*(int *)(hero_base + 0x894) < scale_game_frames(700)) {
                speed += D_0015ED6C * 3.0f;
            }
            if (*(short *)(hero_base - (-(*(int *)(hoverboard_dummy_data + 0x2CC) * 2)) + 0x7C0) <
                *(int *)(hero_base + 0x8C0)) {
                speed -= D_0015ED6C;
            } else {
                speed += D_0015ED6C;
            }
            if (speed < *(float *)(hoverboard_dummy_data + 0x2BC)) {
                approach_value(
                    (float *)(hoverboard_dummy_data + 0x2BC), speed, frame_time_sq * 7.0f);
            } else {
                approach_value(
                    (float *)(hoverboard_dummy_data + 0x2BC), speed, frame_time_sq * 15.0f);
            }
        } else {
            approach_value((float *)(hoverboard_dummy_data + 0x2BC),
                           *(float *)(hoverboard_dummy_data + 0x2B8),
                           frame_time_sq * 20.0f);
        }
        if (gap < 0.2f && *(float *)(hoverboard_dummy_data + 0x2C8) < 0.0f) {
            *(float *)(hoverboard_dummy_data + 0x268) = z;
            *(int *)(hoverboard_dummy_data + 0x2E4) = 0;
        }
        rate = *(float *)(hoverboard_dummy_data + 0x2C4);
        limit = D_0015ED6C * 2.6179938f;
        if (limit < rate) {
            rate = limit;
        } else if (rate < -limit) {
            rate = -limit;
        }
        if (MOBY(moby)->flags & 0x8000) {
            rate = -rate;
        }
        {
            float coefficient = frame_scale_sq;

            *(float *)(hoverboard_dummy_data + 0x248) = rate * 30.0f;
            *(float *)(hoverboard_dummy_data + 0xC8) = rate * 20.0f;
            *(float *)(hoverboard_dummy_data + 0x1C8) = rate * 50.0f;
            *(float *)(hoverboard_dummy_data + 0x144) = rate * 40.0f;
            update_joint_animation(
                coefficient * 0.02f, coefficient * 0.3f, moby, hoverboard_dummy_data + 0x1E0, 3);
        }
        update_joint_animation(
            frame_scale_sq * 0.02f, frame_scale_sq * 0.3f, moby, hoverboard_dummy_data + 0x60, 0);
        update_joint_animation(
            frame_scale_sq * 0.02f, frame_scale_sq * 0.3f, moby, hoverboard_dummy_data + 0x160, 1);
        update_joint_animation(
            frame_scale_sq * 0.015f, frame_scale_sq * 0.3f, moby, hoverboard_dummy_data + 0xE0, 2);
        qcopy(moby + 0x10, hoverboard_dummy_data + 0x260);
        {
            float bob_time = D_0015ED6C;

            MOBY(moby)->pos.z += 0.25f;
            *(float *)(hoverboard_dummy_data + 0x2E8) =
                fast_add_rotations(*(float *)(hoverboard_dummy_data + 0x2E8), bob_time * 6.108652f);
        }
        MOBY(moby)->pos.z += fast_sin(*(float *)(hoverboard_dummy_data + 0x2E8)) * 0.08f;
        fast_vec_sub(ahead, moby + 0x10, hoverboard_dummy_data + 0x280);
        qcopy(hoverboard_dummy_data + 0x280, moby + 0x10);
        moby_point_position(moby, (D_L16_0015F5CC & 1) | 4, impact);
        angle = random_angle_radians();
        scale = D_0015ED6C * 0.25f;
        fast_vec_normalize(spray, moby + 0xE0, scale * fast_cos(angle));
        fast_vec_normalize(lateral, moby + 0xD0, scale * fast_sin(angle));
        fast_vec_add(spray, spray, ahead);
        fast_vec_add(spray, spray, lateral);
        qcopy(spray, ahead);
        spray[3] = 120000.0f;
        if (*(short *)(hoverboard_dummy_data + 0x2F6) == 0) {
            int ticks = random_int_between(scale_game_frames(10), scale_game_frames(13));

            spawn_effect(impact,
                         spray,
                         hoverboard_dummy_spray_initial_color_gp,
                         D_L16_00161A0C,
                         ticks,
                         15,
                         15,
                         1);
        } else {
            int ticks;

            fast_vec_scale(dust, ahead, 0.92f);
            ticks = random_int_between(scale_game_frames(15), scale_game_frames(18));
            spawn_effect(impact, dust, 0x5032F0D2, D_L16_00161A0C, ticks, 30, 20, 1);
            ticks = random_int_between(scale_game_frames(8), scale_game_frames(17));
            spawn_burst(impact,
                        spray,
                        hoverboard_dummy_spray_initial_color_gp,
                        D_L16_00161A0C,
                        ticks,
                        1,
                        15000.0f);
        }
        break;
    }
}

#include "qcopy.h"

/* Move the moby toward a target along a slerped path until the parameter reaches its goal. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D6DA8), where it is exact; names translated to the US level program. */

extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");

void door_update(unsigned char *moby) __asm__("FUN_L16_002d59e0");

void door_update(unsigned char *moby) {
    enum {
        DOOR_STATE_INITIALIZE = 0,
        DOOR_STATE_MOVING = 1,
        DOOR_STATE_IDLE = 2
    };
    char *door_data = ((char *)MOBY(moby)->pvars);
    char *door_position;
    char  position_offset[16];
    switch (moby[0x20]) {
    case DOOR_STATE_INITIALIZE:
        qcopy(door_data, moby + 0x10);
        moby[0x20] = 2;
        break;
    case DOOR_STATE_MOVING:
        door_position = (char *)moby + 0x10;
        qcopy(door_position, door_data);
        advance_accelerated_scalar(*(float *)(door_data + 0x14),
                                   frame_time_sq * 4.0f,
                                   frame_time_sq * 4.0f,
                                   D_0015ED6C * 8.0f,
                                   (float *)(door_data + 0x18),
                                   (float *)(door_data + 0x10));
        fast_vec_normalize(position_offset, moby + 0xC0, *(float *)(door_data + 0x18));
        fast_vec_add(door_position, door_position, position_offset);
        if (*(float *)(door_data + 0x18) == *(float *)(door_data + 0x14)) {
            moby[0x20] = 2;
        }
        break;
    case DOOR_STATE_IDLE:
        break;
    }
}

/* Activate the linked mobys and return the one nearest the challenge target. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D6E98), where it is exact; names translated to the US level program. */

extern float class_0287_group_activation_value_gp __asm__("D_L16_00161A80") __attribute__((sda));

void *set_class_0287_group_activation_and_find_nearest(int index,
                                                       int secondary) __asm__("FUN_L16_002d5ad0");

void *set_class_0287_group_activation_and_find_nearest(int index, int secondary) {
    short *group_entry = (short *)D_L16_001ABCC0[index];
    char  *nearest = 0;
    float  distance = 1000.0f;
    char  *moby;
    float *data;
    float  candidate_distance;
    float  activation_value;
    if (!group_entry)
        return 0;
    while (1) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass != 0x287)
            continue;
        candidate_distance = fast_vec_dist(D_L16_001671C0, moby + 0x10);
        if (candidate_distance < distance) {
            distance = candidate_distance;
            nearest = moby;
        }
        data = ((float *)MOBY(moby)->pvars);
        moby[0x20] = 1;
        activation_value = class_0287_group_activation_value_gp;
        data[5] = activation_value;
        if (secondary)
            data[6] = activation_value;
        if (*group_entry++ < 0)
            return nearest;
    }
}

/* Activate the linked mobys, clearing their timers, and return the one nearest the challenge target. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D6F90), where it is exact; names translated to the US level program. */

void *activate_class_0287_group_and_find_nearest(int index,
                                                 int secondary) __asm__("FUN_L16_002d5bc8");

void *activate_class_0287_group_and_find_nearest(int index, int secondary) {
    short *group_entry = (short *)D_L16_001ABCC0[index];
    char  *nearest = 0;
    float  distance = 1000.0f;
    char  *moby;
    int   *data;
    float  candidate_distance;
    if (!group_entry)
        return 0;
    while (1) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass != 0x287)
            continue;
        candidate_distance = fast_vec_dist(D_L16_001671C0, moby + 0x10);
        data = ((int *)MOBY(moby)->pvars);
        if (candidate_distance < distance) {
            distance = candidate_distance;
            nearest = moby;
        }
        moby[0x20] = 1;
        data[5] = 0;
        if (secondary)
            data[6] = 0;
        if (*group_entry++ < 0)
            return nearest;
    }
}

/* The linked moby of class 0x28A nearest a position, within 37 units. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7178), where it is exact; names translated to the US level program. */

void *find_nearest_test_dummy_door(int index, void *position) {
    short *group_entry = (short *)D_L16_001ABCC0[index];
    char  *nearest = 0;
    float  distance = 37.0f;
    char  *moby;
    float  candidate_distance;
    if (!group_entry)
        return 0;
    while (1) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)group_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass != 0x28A)
            continue;
        candidate_distance = fast_vec_dist(position, moby + 0x10);
        if (candidate_distance < distance) {
            distance = candidate_distance;
            nearest = moby;
        }
        if (*group_entry++ < 0)
            return nearest;
    }
}

extern s32  FUN_001f99a8_5e80(s32) __asm__("FUN_001f99a8");
extern void FUN_0022da68_5e80(int, int, void *) __asm__("FUN_0022da68");

void refresh_test_dummy_door_timer(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    if (*(int *)(moby_data + 8) == 0) {
        if (FUN_001f99a8_5e80(last_test_dummy_door_sound_request_frame_gp - D_L16_0015F5CC) >= 4) {
            last_test_dummy_door_sound_request_frame_gp = D_L16_0015F5CC;
            FUN_0022da68_5e80(0, 0, moby);
        }
    }
    *(int *)(moby_data + 8) = scale_game_frames(5);
}
/* Drive a triggered scene, player pose and objective completion. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D72C0), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad[0x72];
    unsigned char complete;
} L16ObjectiveEntry_q;

typedef struct {
    int trigger, helper, path, hidden, pose, camera, objective, pad1, pad2, timer_q;
} L16SceneData_q;

extern char  D_0013E633_q[], D_0014171B[], D_0013D388[], D_L16_00167240[] __asm__("D_0013E633");
extern char  D_0014162A_lui[] __asm__("D_0014162A") NOT_SDA;
extern char  D_L16_001671C0_lui[] __asm__("D_L16_001671C0") NOT_SDA;
extern void *set_class_0287_group_activation_and_find_nearest(int, int) __asm__("FUN_L16_002d5ad0");
extern void *activate_class_0287_group_and_find_nearest(int, int) __asm__("FUN_L16_002d5bc8");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  FUN_L00_002598b0_q(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void activate_test_dummy_group(int) __asm__("FUN_L16_002ceca8");
extern void FUN_L16_002d6260_q(char *, void *, float *) __asm__("FUN_L16_002d6260");
void        mmr_q(struct Obj *obj) __asm__("FUN_0020c828");

extern char D_0013F3D0[] NOT_SDA;
void        update_triggered_group_objective(struct Moby *moby) __asm__("FUN_L16_002d5ef8");

void update_triggered_group_objective(struct Moby *moby) {
    enum {
        SCENE_STATE_INITIALIZE = 0,
        SCENE_STATE_WAIT_FOR_TRIGGER = 1,
        SCENE_STATE_WAIT_TO_HIDE = 2,
        SCENE_STATE_WAIT_FOR_LINKED_MOBYS_AND_TIMER = 3,
        SCENE_STATE_COMPLETE = 4,
        SCENE_STATE_HIDDEN_WAIT_FOR_TRIGGER = 5,
        SCENE_STATE_HIDDEN_WAIT_FOR_LINKED_MOBYS = 6
    };
    float           position[4], rotation[4];
    L16SceneData_q *scene_data = (L16SceneData_q *)moby->pvars;
    void           *helper;
    switch (moby->state) {
    case SCENE_STATE_INITIALIZE:
        if (scene_data == 0 || scene_data->trigger == -1) {
            mmr_q(moby);
            return;
        }
        if (scene_data->hidden == -1) {
            moby->state = 5;
            moby->flags |= 1;
            moby->visible = 0;
            moby->collision = 0;
        } else {
            moby->state = 1;
            set_class_0287_group_activation_and_find_nearest(scene_data->helper, 1);
        }
        break;
    case SCENE_STATE_WAIT_FOR_TRIGGER: {
        char *player = D_0013F3D0;
        if (!is_point_inside_clip_volume(player, scene_data->trigger))
            break;
        scene_data->timer_q = scale_game_frames(60);
        moby->state = 2;
        if (scene_data->camera >= 0)
            *(unsigned short *)(player + 0x225A) = *(unsigned short *)&scene_data->camera;
        jet_mount(D_L16_001671C0_lui, D_L16_001671C0_lui + 0x10, 1, 0, 0);
        helper = activate_class_0287_group_and_find_nearest(scene_data->helper, 0);
        goto notify;
    }
    case SCENE_STATE_WAIT_TO_HIDE:
        if (scene_data->camera >= 0)
            *(unsigned short *)(D_0014162A_lui) = *(unsigned short *)&scene_data->camera;
        if (fast_dec_timer(&scene_data->timer_q)) {
            moby->state = 3;
            scene_data->timer_q = scale_game_frames(60);
            moby->visible = 0;
            moby->flags |= 1;
            activate_test_dummy_group(scene_data->path);
        }
        break;
    case SCENE_STATE_WAIT_FOR_LINKED_MOBYS_AND_TIMER:
        FUN_L16_002d6260_q(moby, position, rotation);
        set_camera_position(position);
        set_camera_rotation(rotation);
        if (scene_data->camera >= 0)
            *(unsigned short *)(D_0014162A_lui) = *(unsigned short *)&scene_data->camera;
        if (count_active_moby_group_members(scene_data->path, -1))
            break;
        if (!fast_dec_timer(&scene_data->timer_q))
            break;
        moby->visible = 1;
        moby->state = 4;
        moby->flags &= 0xFFFE;
        jet_end_mount(3);
        set_race_camera(D_L16_001600EC_m + (scene_data->pose << 7) + 0x30,
                        D_L16_001600EC_m + (scene_data->pose << 7) + 0x70,
                        0,
                        1);
        FUN_L00_002598b0_q((int)moby,
                           5.0f,
                           D_L16_001600EC_m + (scene_data->pose << 7) + 0x30,
                           0x10000,
                           50.0f,
                           1.0f,
                           0,
                           1,
                           0);
        ((L16ObjectiveEntry_q *)(D_0013D388 + scene_data->objective))->complete = 1;
        helper = set_class_0287_group_activation_and_find_nearest(scene_data->helper, 0);
    notify:
        if (helper)
            FUN_0022da68_q(1, 0, helper);
        break;
    case SCENE_STATE_COMPLETE:
        break;
    case SCENE_STATE_HIDDEN_WAIT_FOR_TRIGGER:
        if (is_point_inside_clip_volume(D_0013F3D0, scene_data->trigger)) {
            activate_class_0287_group_and_find_nearest(scene_data->helper, 0);
            activate_test_dummy_group(scene_data->path);
            moby->state = 6;
        }
        break;
    case SCENE_STATE_HIDDEN_WAIT_FOR_LINKED_MOBYS: {
        char *player = D_0013F3D0;
        if (is_point_inside_clip_volume(player, scene_data->trigger) && scene_data->camera >= 0)
            *(unsigned short *)(player + 0x225A) = *(unsigned short *)&scene_data->camera;
        if (!count_active_moby_group_members(scene_data->path, -1)) {
            moby->state = 4;
            helper = set_class_0287_group_activation_and_find_nearest(scene_data->helper, 0);
            if (helper)
                FUN_0022da68_q(1, 0, helper);
            ((L16ObjectiveEntry_q *)(D_0013D388 + scene_data->objective))->complete = 1;
        }
        break;
    }
    }
}
/* Computes a moby's direction vectors from its table entry. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002D7628), where it is exact; names translated to the US level program. */

extern int D_L16_001600EC; /* no foreign declaration */

extern float triggered_scene_rotation_y_gp __asm__("D_L16_00161A8C") __attribute__((sda));
void         compute_triggered_scene_path_pose(struct Moby *moby,
                                               void        *out1,
                                               float       *out2) __asm__("FUN_L16_002d6260");

void compute_triggered_scene_path_pose(struct Moby *moby, void *out1, float *out2) {
    char *data = (char *)moby->pvars;
    float hero_offset[4];
    char *entry = D_L16_001600EC + (*(int *)data << 7);
    char *position_path = D_L16_001B0930[*(int *)(data + 0xC)];
    fast_vec_sub(hero_offset, &hero.motion.pos, entry + 0x30);
    hero_offset[3] = 0;
    transform_vector_by_basis(hero_offset, hero_offset, entry + 0x40);
    advance_accelerated_scalar((hero_offset[1] + 1.0f) * 0.5f,
                               frame_time_sq,
                               frame_time_sq,
                               D_0015ED6C + D_0015ED6C,
                               (float *)(data + 0x1C),
                               (float *)(data + 0x20));
    lerp_vector(out1, position_path + 0x10, position_path + 0x20, *(float *)(data + 0x1C));
    clear_u64_value(out2);
    out2[1] = triggered_scene_rotation_y_gp;
    out2[2] = moby->rot.z;
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E2290), where it is exact; names translated to the US level program. */

int get_target_reposition_status(char *moby_or_private_data) __asm__("FUN_L16_002e0e28");

int get_target_reposition_status(char *moby_or_private_data) {
    unsigned char moby_state = moby_or_private_data[0x20];
    moby_or_private_data = *(char **)(moby_or_private_data + 0x78);
    if (moby_state == 7 || moby_state == 8) {
        if (moby_state == 8 && *(float *)(moby_or_private_data + 0x34) > 0.2f) {
            return 2;
        }
        return 1;
    }
    return 0;
}

int conveyor_belt_scrolling_vertex_color_gp __asm__("D_L16_001619B0") = 0x50808070;
