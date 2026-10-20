/* Shared moby classes: armed_transport, grindrail_barrier. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"


#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/data_labels.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E3740), where it is exact; names translated to the US level program. */

typedef union {
    unsigned int q __attribute__((mode(TI)));
    float f[4];
} __attribute__((aligned(16))) L16TransportVector;

char *FUN_L16_002a0dc0_5(char *owner, char *vec, void *pos, void *vel, int c) __asm__("FUN_L16_002a0dc0");
extern char *D_L16_0015FFD8;
extern char *D_L16_001B0930[];
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L16_0015F580[] __attribute__((section(".sdata")));
extern float D_L16_001D9680[4];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float distance_xyz(void *) __asm__("FUN_001f9b48");
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_L16_0015F5C4;
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int FUN_L16_002e2e88(char *, char *);
extern int fast_dec_timer(int*) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern short D_L16_00161D7C __attribute__((sda));
extern short D_L16_00161D80;
extern short D_L16_00161D88 __attribute__((sda));
extern short D_L16_00161D8C __attribute__((sda));
extern void *FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_00257470(void *, int, int);
extern void spawn_explosion(void*,void*,void*,float,float,int,int,int,float,float,float,int,float,float,int,int,int,int) __asm__("FUN_L00_0025e450");
extern void FUN_L16_002e2b60(struct Moby *);
extern void FUN_L16_002e2fa0(void *);
extern void FUN_L16_002e3050(char *);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
void FUN_L16_002d3dc0(char *moby, void *owner, void *position, char *vector);
void FUN_L16_002d3de0(char *moby, int arg);
extern int func_0022ED80_update(int, int, void *) __asm__("FUN_0022da68");
extern float func_001F9D10_update(void *, void *) __asm__("FUN_001f9b48");

void armed_transport_update(char *m) __asm__("FUN_L16_002e22d8");
void armed_transport_update(char *m) {
    enum {
        ARMED_TRANSPORT_STATE_INITIALIZE = 0,
        ARMED_TRANSPORT_STATE_WAIT_FOR_ACTIVATION = 1,
        ARMED_TRANSPORT_STATE_FOLLOW_FIRST_PATH = 2,
        ARMED_TRANSPORT_STATE_RELEASE_PASSENGERS = 3,
        ARMED_TRANSPORT_STATE_ALIGN_SECOND_PATH = 4,
        ARMED_TRANSPORT_STATE_FOLLOW_SECOND_PATH = 5,
        ARMED_TRANSPORT_STATE_TRACK_TARGET = 6,
        ARMED_TRANSPORT_STATE_SPAWN_DIRECTED_MOBY = 7,
        ARMED_TRANSPORT_STATE_DYING = 8
    };
    L16TransportVector vector, rotation;
    char *d = *(char **)(m + 0x78);

    FUN_L16_002e2b60(m);
    switch (*(unsigned char *)(m + 0x20)) {
    case ARMED_TRANSPORT_STATE_INITIALIZE: {
        char *path;
        float speed;

        *(unsigned short *)(m + 0x32) = 255;
        *(unsigned char *)(m + 0x30) = 255;
        *(float *)(d + 0x20) = 12.0f;
        *(short *)(d + 0x24) = 12;
        d[0x28] = 3;
        if (*(int *)(d + 0xD8) != -1) FUN_L16_002e2fa0(D_L16_001B0930[*(int *)(d + 0xD8)]);
        if (*(int *)(d + 0xDC) != -1) FUN_L16_002e2fa0(D_L16_001B0930[*(int *)(d + 0xDC)]);
        speed = *(float *)&D_L16_00161D80 * frame_time;
        *(int *)(d + 0x16C) = 0;
        *(int *)(d + 0x168) = 0;
        *(float *)(d + 0x170) = speed;
        *(float *)(d + 0x194) = *(float *)(m + 0x18);
        if (*(int *)(d + 0xD8) == -1) {
            m[0x20] = 6;
            break;
        }
        path = D_L16_001B0930[*(int *)(d + 0xD8)];
        *(float *)(d + 0x174) = func_001F9D10_update(path + 0x10, path + 0x20);
        qcopy(m + 0x10, path + 0x10);
        m[0x20] = 1;
        m[0x31] = 0;
        *(int *)(m + 0x94) = 0;
        *(unsigned short *)(m + 0x34) = (*(unsigned short *)(m + 0x34) | 1) & 0xEFFF;
        break;
    }
    case ARMED_TRANSPORT_STATE_WAIT_FOR_ACTIVATION: {
        int i;

        if (D_L16_0015F5C4 == 0
            && (*(int *)(d + 0xD4) == -1
                || point_in_path_xy(hero.motion.pos.f, D_L16_001B0930[*(int *)(d + 0xD4)] + 0x10,
                                     *(int *)D_L16_001B0930[*(int *)(d + 0xD4)]))) {
            m[0x20] = 2;
            *(unsigned short *)(m + 0x34) &= 0xFFFE;
            m[0x31] = 1;
            *(int *)(m + 0x94) = *(int *)(*(char **)(m + 0x24) + 0x10);
            *(unsigned short *)(m + 0x34) |= 0x1000;
            vector.q = 0;
            vector.f[2] = 3.14159f;
            for (i = 0; i < 4; i++) {
                if (((int *)(d + 0xC0))[i] >= 0) {
                    FUN_L16_002d3dc0(D_L16_0015FFD8 + (((int *)(d + 0xC0))[i] << 8), m, D_L16_001D9680,
                                      (char *)vector.f);
                }
            }
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_FOLLOW_FIRST_PATH: {
        int i, loaded = 0;
        char *path;

        for (i = 0; i < 4; i++) {
            if (((int *)(d + 0xC0))[i] >= 0) loaded = 1;
        }
        path = D_L16_001B0930[*(int *)(d + 0xD8)];
        if (loaded && (float)(*(int *)path - *(int *)(d + 0x168)) * *(float *)(d + 0x174) < 16.0f) {
            advance_accelerated_scalar((float *)(d + 0x17C), (float *)(d + 0x180), 1.5707964f, frame_time_sq * 12.566371f,
                          frame_time_sq * 12.566371f, frame_time * 6.2831855f);
        }
        if (FUN_L16_002e2e88(m, path)) {
            if (loaded) {
                m[0x20] = 3;
            } else if (*(int *)(d + 0xDC) == -1) {
                m[0x20] = 6;
            } else {
                m[0x20] = 4;
            }
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_RELEASE_PASSENGERS: {
        int i, *slot;

        if (!fast_dec_timer((int *)(d + 0x184))) break;
        slot = (int *)(d + 0xC0);
        for (i = 0; i < 4; i++, slot++) {
            if (((int *)(d + 0xC0))[i] >= 0) {
                char *passenger = D_L16_0015FFD8 + (((int *)(d + 0xC0))[i] << 8);

                clear_u64_value(rotation.f);
                rotation.f[2] = fast_add_rotations(*(float *)(m + 0x48), 3.14159f);
                transform_vector_by_basis(vector.f, D_L16_001D9680, m + 0xC0);
                add_vector_xyz(vector.f, vector.f, m + 0x10);
                FUN_L16_002d3de0(passenger, scale_game_frames(90));
                *slot = -1;
                *(int *)(d + 0x184) = scale_game_frames(30);
                break;
            }
            if (i == 3) {
                if (*(int *)(d + 0xDC) != -1) {
                    m[0x20] = 4;
                } else {
                    m[0x20] = 6;
                }
            }
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_ALIGN_SECOND_PATH: {
        float heading = *(float *)(D_L16_001B0930[*(int *)(d + 0xDC)] + 0x1C);

        advance_accelerated_scalar((float *)(m + 0x48), (float *)(d + 0x178), heading, frame_time_sq * 6.2831855f,
                      frame_time_sq * 6.2831855f, frame_time * 6.2831855f);
        if (fast_difference_between_rotations(*(float *)(m + 0x48), heading) < 0.01f) {
            m[0x20] = 5;
            *(int *)(d + 0x168) = 0;
            *(int *)(d + 0x16C) = 0;
        }
        break;
    }
    case ARMED_TRANSPORT_STATE_FOLLOW_SECOND_PATH:
        advance_accelerated_scalar((float *)(d + 0x17C), (float *)(d + 0x180), 0.0f, frame_time_sq * 12.566371f,
                      frame_time_sq * 12.566371f, frame_time * 6.2831855f);
        if (FUN_L16_002e2e88(m, D_L16_001B0930[*(int *)(d + 0xDC)])) {
            if (*(int *)(d + 0xD0) == -1) {
                mark_moby_for_removal(m);
                return;
            }
            m[0x20] = 6;
        }
        break;
    case ARMED_TRANSPORT_STATE_TRACK_TARGET: {
        float *yaw = (float *)(m + 0x48);
        float bearing;

        advance_accelerated_scalar((float *)(d + 0x17C), (float *)(d + 0x180), 0.0f, frame_time_sq * 12.566371f,
                      frame_time_sq * 12.566371f, frame_time * 6.2831855f);
        bearing = fast_arc_tan(*(float *)(d + 0x70) - *(float *)(m + 0x10),
                                    *(float *)(d + 0x74) - *(float *)(m + 0x14));
        advance_accelerated_scalar(yaw, (float *)(d + 0x178), bearing, frame_time_sq * 3.1415927f, frame_time_sq * 3.1415927f,
                      frame_time * 6.2831855f);
        advance_accelerated_scalar((float *)(m + 0x40), (float *)(d + 0x188), 0.0f, frame_time_sq * 1.5707964f,
                      frame_time_sq * 1.5707964f, frame_time * 6.2831855f);
        if (fast_dec_timer((int *)(d + 0x164)) && *(int *)(d + 0xB4) != 2) m[0x20] = 7;
        break;
    }
    case ARMED_TRANSPORT_STATE_SPAWN_DIRECTED_MOBY:
        moby_point_position(m, 1, vector.f);
        subtract_vector_xyz(rotation.f, d + 0x70, vector.f);
        normalize_vector_xyz(rotation.f, rotation.f, *(float *)&D_L16_00161D7C * frame_time);
        func_0022ED80_update(1, 0, m);
        FUN_L16_002a0dc0_5(m, (char *)rotation.f, vector.f, d + 0x70, scale_game_frames(45));
        m[0x20] = 6;
        *(int *)(d + 0x164) = scale_game_frames(180);
        break;
    case ARMED_TRANSPORT_STATE_DYING: {
        float dt = frame_time;

        *(float *)(d + 0x194) -= dt + dt;
        *(float *)(m + 0x40) += dt * 0.2617994f;
        if (fast_dec_timer((int *)(d + 0x160)) || hero.vehicle_mode == 2) {
            spawn_explosion(m, D_L16_0015F580, m + 0x10, 0.0f, 0.0f, 20, 12, 8, 4.0f, 2.5f, 9.0f, -1, 2.0f, 0.0f,
                              0, 0, -1, 0);
            FUN_L00_00263fd8(m, 0x634, (float *)(m + 0x10), m + 0x40, 0, 0, 0.0f, D_L16_0015F580, D_L16_0015F580,
                              D_L16_0015F580);
            FUN_L00_00263fd8(m, 0x635, (float *)(m + 0x10), m + 0x40, 0, 0, 0.0f, D_L16_0015F580, D_L16_0015F580,
                              D_L16_0015F580);
            FUN_L00_00263fd8(m, 0x636, (float *)(m + 0x10), m + 0x40, 0, 0, 0.0f, D_L16_0015F580, D_L16_0015F580,
                              D_L16_0015F580);
            FUN_L00_00257470(m, 0, -1);
            mark_moby_for_removal(m);
            return;
        }
        break;
    }
    }
    *(float *)(d + 0x18C) = fast_add_rotations(*(float *)(d + 0x18C),
                                          *(float *)&D_L16_00161D88 * DEG_TO_RAD * frame_time);
    *(float *)(m + 0x18) = *(float *)(d + 0x194) + *(float *)&D_L16_00161D8C * fast_sin(*(float *)(d + 0x18C));
    FUN_L16_002e3050(m);
}
#include "qcopy.h"

extern char *D_L16_0015FFD8_sda __asm__("D_L16_0015FFD8");
extern char *D_L16_001B0930[];
extern u8 D_0013E550[];

typedef struct {
    char pad[0x74];
    u8 active;
    char pad75[0x13];
    char *owner;
} L16Slot_e;

extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_0022da68_i(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int hit_reaction(void *, char *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void mark_moby_for_removal_u(char *) __asm__("FUN_0020c828");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025ff38(float, void *, void *, int, int, void *, int);

typedef struct {
    s32 status;
    float amount;
} L16Hit_e;

extern float D_L16_00161D78_gp __asm__("D_L16_00161D78") __attribute__((sda));
/* Update interaction ownership, damage responses and pursuit tracking. */
void FUN_L16_002e2b60(struct Moby *transport_moby) {
    L16Hit_e hit;
    char *transport_data = (char *)transport_moby->pvars;
    float zero;
    char *record;
    int response;

    transport_moby->scale = transport_moby->pclass->scale * D_L16_00161D78_gp;
    if ((unsigned int)(transport_moby->state - 2) < 4) {
        if (!voice_is_playing(transport_moby, *(int *)(transport_data + 0x190)))
            *(int *)(transport_data + 0x190) = FUN_0022da68_i(0, 4, transport_moby);
    } else if (voice_is_playing(transport_moby, *(int *)(transport_data + 0x190))) {
        int slot = *(int *)(transport_data + 0x190);
        if (slot != -1) {
            L16Slot_e *voice_slot = (L16Slot_e *)(D_0013E550 + slot * 0x70);
            if (voice_slot->owner == transport_moby && voice_slot->active != 0)
                release_voice_slot(slot);
        }
        *(int *)(transport_data + 0x190) = -1;
    }
    hit.amount = zero = 0.0f;
    record = find_moby_hit(transport_moby, 0x330000, 0);
    response = hit_reaction(transport_moby, record, transport_data + 0x20, 0, &hit.status, &hit.amount, 0, 4);
    if (hit.status != 1 && transport_moby->state != 8) {
        *(float *)(transport_data + 0x20) -= hit.amount;
        if (*(float *)(transport_data + 0x20) <= zero || (hero.vehicle_mode == 2 && hit.amount >= 2.0f))
            response = 1;
        if (response > 0) {
            if (response >= 3) {
                if (response < 9) {
                    FUN_0022da68_i(3, 0, transport_moby);
                    *(unsigned char *)(transport_data + 0x67) = 120;
                    *(float *)(transport_data + 0x188) =
                        randf_sym(30.0f, 50.0f) * DEG_TO_RAD * frame_time;
                }
            } else {
                FUN_0022da68_i(3, 0, transport_moby);
                if (hero.vehicle_mode != 2)
                    FUN_0022da68_i(2, 0, transport_moby);
                {
                    int *child = (int *)(transport_data + 0xC0), count = 3;
                    do {
                        char *pool = D_L16_0015FFD8_sda;
                        if (*child >= 0)
                            mark_moby_for_removal_u(pool + (*child << 8));
                        child++;
                    } while (--count >= 0);
                }
                transport_moby->state = 8;
                *(int *)(transport_data + 0x160) = scale_game_frames(90);
                *(unsigned char *)(transport_data + 0x67) = 120;
            }
        }
        trooper_anim_update(transport_moby, transport_data + 0x60);
    }
    transport_moby->hit_slot = 255;
    trooper_anim_finish(transport_moby, transport_data + 0x60);
    if (*(int *)(transport_data + 0xD0) != -1 && D_001413DC[0] != 22) {
        char *path = D_L16_001B0930[*(int *)(transport_data + 0xD0)];
        FUN_L00_0025ff38(128.0f, transport_moby, transport_data + 0x70, 0, 0, path + 0x10, *(int *)path);
    } else
        *(int *)(transport_data + 0xB4) = 2;
    if (*(int *)(transport_data + 0xB0) == 0) {
        struct Hero *player = &hero;
        *(int *)(transport_data + 0xB0) = ((int)player->moby);
        qcopy(transport_data + 0x70, &player->motion.pos);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Advance a path segment and ease the remaining travel distance. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E42F0), where it is exact; names translated to the US level program. */

extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern f32 D_0015ED70_u __asm__("D_0015ED70");
extern short D_L16_00161E44, D_L16_00161D80_q __asm__("D_L16_00161D80") __attribute__((sda));
extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("FUN_00214e58");
extern void func_00215CA8_path42_q(float, void *, int, void *, void *, int) __asm__("FUN_00214e58");
extern float func_001F9D10_path42_q(void *, void *) __asm__("FUN_001f9b48");
extern float func_00214D88_path42_q(float, float, float, float, float *,
                                    float *) __asm__("FUN_00213f38");

extern float D_L16_00161D84 __attribute__((sda));
int FUN_L16_002e2e88(char *transport_moby, char *path) {
    char *transport_data = *(char **)(transport_moby + 0x78);
    float progress = (float)*(int *)(transport_data + 0x168) + *(float *)(transport_data + 0x16C) +
                     *(float *)(transport_data + 0x170) / *(float *)(transport_data + 0x174);
    int index = truncate_float_to_s32(progress);
    float speed;
    float distance, rate;
    char *position;
    *(int *)(transport_data + 0x168) = index;
    *(float *)(transport_data + 0x16C) = progress - (float)index;
    if (index < *(int *)path - 1) {
        position = transport_moby + 0x10;
        func_00215CA8_path42_q(progress, path, 0, position, transport_moby + 0x40, 0);
        *(float *)(transport_data + 0x194) = *(float *)(transport_moby + 0x18);
        speed = 0.0f;
        distance = func_001F9D10_path42_q(position, path + (*(int *)path << 4));
        rate = D_L16_00161D84 * *(float *)&D_0015ED70_u;
        func_00214D88_path42_q(distance, rate, rate, *(float *)&D_L16_00161D80_q * frame_time,
                               &speed, (float *)(transport_data + 0x170));
        return 0;
    }
    return 1;
}
/* Sets each path point's heading toward the next point; the last copies the one before. */

typedef struct {
    float x, y, z, heading;
} PathPoint2fa0;

typedef struct {
    int count;
    int pad[3];
    PathPoint2fa0 pts[1];
} Path2fa0;

void FUN_L16_002e2fa0(void *arg) {
    Path2fa0 *path = arg;
    int i;
    int point_count;
    for (i = 0; i < path->count - 1; i++) {
        PathPoint2fa0 *points = path->pts;
        path->pts[i].heading = fast_arc_tan(path->pts[i + 1].x - path->pts[i].x, points[i + 1].y - path->pts[i].y);
    }
    point_count = path->count;
    if (point_count >= 2)
    {
        Path2fa0 *heading_source = (Path2fa0 *)((char *)path + (point_count - 2) * 16);
        Path2fa0 *b = (Path2fa0 *)((char *)path + (point_count - 1) * 16);
        b->pts[0].heading = heading_source->pts[0].heading;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E44B8), where it is exact; names translated to the US level program. */

extern void update_joint_animation(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");

void FUN_L16_002e3050(char *moby) {
    float ticks = frame_scale_sq;
    float joint_animation_smoothing_factor = ticks * 0.03f;
    float b = ticks * 0.3f;
    char *data = *(char **)(moby + 0x78);
    *(float *)(data + 0x144) = *(float *)(data + 0x17C);
    update_joint_animation(moby, data + 0xE0, 2, joint_animation_smoothing_factor, b);
}
/* Updates a moby effect and advances it when its linked object is ready. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E6B70), where it is exact; names translated to the US level program. */

extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
int FUN_L16_002e5cd0(int idx);
extern void FUN_L16_002e58d8_u(void) __asm__("FUN_L16_002e58d8");
extern void FUN_L16_002e5d30_c(int) __asm__("FUN_L16_002e5d30");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
extern int func_0022ED80_6B70(int, int, void *) __asm__("FUN_0022da68");

extern short D_L16_00161E00 __attribute__((sda));
extern short D_L16_00161E04 __attribute__((sda));
extern short D_L16_00161E08 __attribute__((sda));
void grindrail_barrier_update(struct Moby *m) __asm__("FUN_L16_002e5708");
void grindrail_barrier_update(struct Moby *barrier_moby) {
    enum {
        GRINDRAIL_BARRIER_STATE_INITIALIZE = 0,
        GRINDRAIL_BARRIER_STATE_COLLISION_ENABLED = 1,
        GRINDRAIL_BARRIER_STATE_COLLISION_DISABLED = 2
    };
    char *barrier_data = (char *)barrier_moby->pvars;
    float cycle;
    int i;
    float *cardinal_angles;
    switch (barrier_moby->state) {
    case GRINDRAIL_BARRIER_STATE_INITIALIZE:
        barrier_moby->state = 1;
        barrier_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_BARRIER_STATE_COLLISION_ENABLED:
        cycle = *(float *)(barrier_data + 0xC) + *(float *)&D_L16_00161E04 * frame_time;
        *(float *)(barrier_data + 0xC) = cycle;
        if (cycle > 1.0f)
            *(float *)(barrier_data + 0xC) = cycle - 1.0f;
        if (voice_is_playing(barrier_moby, *(int *)(barrier_data + 0x24)) == 0) {
            *(int *)(barrier_data + 0x24) = func_0022ED80_6B70(0, 4, barrier_moby);
        }
        *(float *)(barrier_data + 8) = fast_add_rotations(*(float *)(barrier_data + 8), *(float *)&D_L16_00161E00 *
                                                                      DEG_TO_RAD * frame_time);
        cardinal_angles = (float *)(barrier_data + 0x10);
        for (i = 0; i < 4; i++) {
            cardinal_angles[i] = fast_add_rotations(cardinal_angles[i],
                                      ((float *)&D_L16_00161E08)[i] * DEG_TO_RAD * frame_time);
        }
        if (barrier_moby->visible)
            enqueue_callback_list_1_alt(FUN_L16_002e58d8_u, barrier_moby);
        if (*(int *)(barrier_data + 0x20) != -1 && FUN_L16_002e5cd0(*(int *)(barrier_data + 0x20))) {
            barrier_moby->collision = 0;
            barrier_moby->state = 2;
            FUN_L16_002e5d30_c(*(int *)(barrier_data + 0x20));
        }
        break;
    case GRINDRAIL_BARRIER_STATE_COLLISION_DISABLED:
        if (!FUN_L16_002e5cd0(*(int *)(barrier_data + 0x20))) {
            barrier_moby->state = 1;
            barrier_moby->collision = barrier_moby->pclass->collision;
        }
        break;
    }
}
/* Draw the animated ring quads and four rotating accents around a moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E6D40), where it is exact; names translated to the US level program. */

typedef struct {
    float position[4][4];
    int color[4];
    float uv[4][2];
    unsigned long flags, texture, giftag, primitive;
} RingQuad;

typedef struct { char pad00[8]; float yaw, phase; float cardinal_angles[4]; } RingData;

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern float D_L16_001D9780[4][2];
extern float D_L16_001D9A70[4][2];
extern float D_L16_001D97C0[4][4];
extern float D_L16_001D9AD0[4][4];
extern float D_L16_001D9A50[4][2];
extern float D_L16_001D97A0[4][2];
extern float D_L16_001D9A90[4][4];
extern float D_L16_001D9800[4][4];
extern int D_L16_00161E18 __attribute__((sda));
extern int D_L16_00161E1C __attribute__((sda));
extern int D_L16_00161E20 __attribute__((sda));
extern int D_L16_00161E28 __attribute__((sda));
extern int D_L16_00161E2C __attribute__((sda));
extern int D_L16_00161E30 __attribute__((sda));
extern int D_L16_00161E34 __attribute__((sda));
extern int D_L16_00161E38 __attribute__((sda));
extern int D_L16_00161E3C __attribute__((sda));
extern int D_L16_00161E40 __attribute__((sda));
extern int fast_tween_color(int,int,float) __asm__("FUN_001fa6e0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");

void FUN_L16_002e58d8(char *barrier_moby) {
    RingQuad quad;
    float transform[4][4], rotation[4];
    RingData *data = *(RingData **)(barrier_moby + 0x78);
    float scale = *(float *)(barrier_moby + 0x2C) / *(float *)(*(char **)(barrier_moby + 0x24) + 0x24);
    float *u, *v;
    float *angles;
    int *color_base;
    float phase, angle_step, phase_step;
    int i;
    qcopy(rotation, barrier_moby + 0x40);
    qcopy(transform[3], barrier_moby + 0x10);
    quad.texture = get_effect_texture(*(int *)&D_L16_00161E38);
    u = &quad.uv[0][0];
    v = &quad.uv[0][1];
    quad.giftag = 0xFF9000000260UL;
    quad.primitive = *(int *)&D_L16_00161E28;
    quad.primitive |= (unsigned long)*(int *)&D_L16_00161E2C << 2;
    quad.primitive |= (unsigned long)*(int *)&D_L16_00161E30 << 4;
    quad.primitive |= (unsigned long)*(int *)&D_L16_00161E34 << 6;
    quad.primitive |= 0x8000000000UL;
    quad.flags = 0;
    color_base = quad.color;
    {
        int k;
        for (k = 0; k < 4; ++k) {
            u[k * 2] = D_L16_001D9780[k][0];
            v[k * 2] = D_L16_001D9780[k][1];
            color_base[k] = *(int *)&D_L16_00161E1C;
            qcopy(quad.position[k], D_L16_001D97C0[k]);
            scale_vector_xyz(quad.position[k], quad.position[k], scale);
        }
    }
    angles = data->cardinal_angles;
    angle_step = 6.2831855f / convert_integer_to_float(*(int *)&D_L16_00161E40);
    phase_step = 3.0f / convert_integer_to_float(*(int *)&D_L16_00161E40);
    phase = data->phase;
    while (phase < 0.0f) phase += 1.0f;
    while (phase > 1.0f) phase -= 1.0f;
    for (i = 0; i < *(int *)&D_L16_00161E40;) {
        float blend;
        int color, j;
        rotation[0] = fast_add_rotations_c(data->yaw, (float)i * angle_step);
        vu_euler_rotation_basis(transform, rotation);
        if (phase > 0.5f) blend = (-phase + 1.0f) + (-phase + 1.0f);
        else blend = phase + phase;
        ++i;
        color = fast_tween_color(*(int *)&D_L16_00161E18, *(int *)&D_L16_00161E1C, blend);
        for (j = 1; j >= 0; --j) color_base[j] = color;
        phase += phase_step;
        if (phase > 1.0f) phase -= 1.0f;
        draw_geometry_quad(&quad, transform, 0);
    }
    quad.texture = get_effect_texture(*(int *)&D_L16_00161E3C);
    {
        int k;
        int solid_color = *(int *)&D_L16_00161E20;
        for (k = 0; k < 4; ++k) {
            u[k * 2] = D_L16_001D97A0[k][0];
            v[k * 2] = D_L16_001D97A0[k][1];
            color_base[k] = solid_color;
            qcopy(quad.position[k], D_L16_001D9800[k]);
        }
    }
    {
        int count;
        for (count = 3; count >= 0; --count) {
            rotation[0] = angles[3 - count];
            vu_euler_rotation_basis(transform, rotation);
            draw_geometry_quad(&quad, transform, 0);
        }
    }
}
/* Returns 1 when every moby in the list is active, 0 if any is not or the list is empty. */
extern short *D_L16_001ABCC0_5cd0[] __asm__("D_L16_001ABCC0");
typedef struct {
    char pad[0xBC];
    unsigned char act;
    char pad2[0x43];
} M_5cd0;

int FUN_L16_002e5cd0(int idx) {
    unsigned short *moby_index_entry = (unsigned short *)D_L16_001ABCC0_5cd0[idx];
    if (moby_index_entry == 0) {
        return 0;
    }
    do {
        if (((M_5cd0 *)D_L16_0015FFD8_sda)[*moby_index_entry & 0x7FFF].act == 0) {
            return 0;
        }
    } while ((short)*moby_index_entry++ >= 0);
    return 1;
}
/* picks the nearest moby in a list (skipping class 0x5A3) and acts on it */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E7198), where it is exact; names translated to the US level program. */

/* D_L16_0015FFD8 is declared sda above (FUN_L16_002e2b60 loads it off $gp), but
   here retail loads it absolute: reach it from a neighbour, past whose size the
   assembler no longer uses $gp. */
#define D_L16_0015FFD8 (((char **)&D_L16_00161D78)[-(0x161D78 - 0x15FFD8) / 4])
extern float fast_vec_dist(void *) __asm__("FUN_001f9b48");
extern short *D_L16_001ABCC0[];
extern float func_001F9D10_2E7198(void *, void *) __asm__("FUN_001f9b48");

extern float D_L16_00161D78;
void FUN_L16_002e5d30(int idx) {
    short *p = D_L16_001ABCC0[idx];
    char *best = 0;
    float bestd = 50000.0f;
    if (p != 0) {
        do {
            char *m = D_L16_0015FFD8 + ((*(unsigned short *)p & 0x7FFF) << 8);
            if (*(short *)(m + 0xA6) != 0x5A3) {
                float d = func_001F9D10_2E7198(D_0013E533 + 0xE9D, m + 0x10);
                if (d < bestd) {
                    bestd = d;
                    best = m;
                }
            }
        } while (*p++ >= 0);
        if (best != 0) {
            allocate_voice_for_target_entry(0, 0, (int)best);
        }
    }
}
extern void FUN_L16_002e9ce0(void);
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");

void FUN_L16_002e9df0(unsigned char *moby) {
    switch (moby[0x20]) {
    case 0:
        moby[0x20] = 1;
        break;
    case 1:
        enqueue_callback_list_1((int)FUN_L16_002e9ce0, (int)moby);
        break;
    }
}
