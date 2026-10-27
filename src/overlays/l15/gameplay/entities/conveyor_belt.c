/* Level 15 moby classes: conveyor_belt, poison_vessel, bolt_grabber, codebot_slot, ultramech_scientist. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

/* Updates a moby: initialises its vector, then applies an angle-based offset. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E87B0), where it is exact; names translated to the US level program. */

extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void update_entity_matrix(char *a, void *b, void *c, void *d) __asm__("FUN_L00_00260738");

void conveyor_belt_update(struct Moby *moby) __asm__("FUN_L15_002e73c0");
void conveyor_belt_update(struct Moby *moby) {
    enum {
        CONVEYOR_BELT_INITIALIZE = 0,
        CONVEYOR_BELT_UPDATE_FRAME_OFFSET = 3
    };
    char *data = (char *)moby->pvars;
    if (data != 0) {
        unsigned char state = moby->state;
        switch (state) {
        case CONVEYOR_BELT_INITIALIZE: {
            char *frame_offset = data + 0x40;
            clear_u64_value(frame_offset);
            *(int *)(data + 0x9C) |= 4;
            moby->state = 3;
            break;
        }
        case CONVEYOR_BELT_UPDATE_FRAME_OFFSET:
            moby->anim_speed = 1.3888889f;
            *(float *)(data + 0x40) =
                fast_cos(fast_add_rotations(moby->rot.z, 1.5707964f)) *
                (frame_time * 2.5f);
            *(float *)(data + 0x44) =
                fast_sin(fast_add_rotations(moby->rot.z, 1.5707964f)) *
                (frame_time * 2.5f);
            *(int *)(data + 0x48) = 0;
            break;
        }
        update_entity_matrix(data + 0x60, data + 0x40, &moby->rot, &moby->rot);
    }
}
#include "qcopy.h"
extern void *create_moby() __asm__("FUN_0020c4f8");

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E92C8), where it is exact; names translated to the US level program. */

extern void FUN_L00_00250df8(void *, void *);

char *spawn_poison_vessel(void *unused, void *vector) __asm__("FUN_L15_002e7ed8");

char *spawn_poison_vessel(void *unused, void *vector) {
    char *moby = create_moby(0x4E9);
    if (moby != 0) {
        char *data = *(char **)(moby + 0x78);
        char *field;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0x7F;
        moby[0x31] = 1;
        moby[0x20] = 0;
        moby[0xBC] = 0;
        *(int *)(data + 0x10) = 0;
        clear_u64_value(data);
        qcopy(moby + 0x10, vector);
        field = *(char **)(moby + 0x24);
        *(int *)(moby + 0x94) = *(int *)(field + 0x10);
        FUN_L00_00250df8(moby, field);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002E9358). */
#include "sda.h"

/* Poison vessel update (moby class 1257): tracks its target and fires its effect. */

typedef struct {
    char _pad00[0x10];
    unsigned char nframes; /* 0x10 */
} AnimSeq;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4f_A4D88;

struct MobyClass_A4D88 {
    u8 pad_0[0xC];
    u8 seq_count; /* number of entries in seqs[] (menu previews clamp the sequence to it) */
    u8 pad_D[3];
    u32 unk10; /* copied into Moby_A4D88.unk94 when a moby is (re)classed */
    u8 pad_14[0x8];
    void *unk1C; /* word table, indexed id * 4 + 4 (0024eec0) */
    struct GifEntry *gifs; /* patched by patch_moby_gifs */
    f32 scale; /* default draw scale: copied into Moby_A4D88.scale, divides it */
    s32 unk28;
    void **callbacks; /* function-pointer table, called with the moby */
    u8 pad_30[0x14];
    u16 flags; /* initial Moby_A4D88.flags */
    s16 unk46; /* class category; 5 is tested by targeting code */
    struct AnimSeq *seqs[1]; /* animation sequences, indexed by Moby_A4D88.seq */
};

struct Moby_A4D88 {
    Vec4f_A4D88 bsphere;
    Vec4f_A4D88 pos;
    u8 state; /* >= 0xFE: dead, waiting to respawn */
    u8 group; /* linked group: index into the level moby-list table D_Lxx_001ABCC0 (0xFF: none) */
    u8 unk22; /* class slot: pclass = D_L00_00197300[unk22] (crate_update) */
    u8 unk23; /* 0x40 for the smoke trail FUN_L09_00307ba8 spawns */
    struct MobyClass_A4D88 *pclass;
    struct Moby_A4D88 *next;
    f32 scale; /* draw scale (FUN_L01_002fa068 halves it, FUN_L00_00215ef8 divides by it) */
    u8 unk30; /* set to 0xFF (0x7F for beams) by spawners */
    u8 unk31; /* set to 1 by spawners */
    s16 unk32; /* set to 0xFF (0x7F for beams) by spawners */
    u16 flags;
    u16 unk36; /* set to 0x7F80 by spawners */
    u64 spawn_frame; /* frame count at which it may respawn */
    Vec4f_A4D88 rot; /* z: yaw (update_npc_dialogue compares it with atan2 to the hero) */
    u8 frame; /* animation frame */
    u8 prev_frame; /* frame index in prev_seq */
    u8 seq; /* animation sequence id */
    u8 prev_seq;
    f32 unk54;
    f32 unk58;
    u8 pad5C[8];
    struct Manip *manips;
    void *cur_frame_data;
    void *prev_frame_data;
    u8 unk70;
    u8 unk71; /* set to 0xFF when a moby changes class */
    u8 unk72;
    u8 unk73;
    void (*update)(struct Moby_A4D88 *moby);
    u8 *pvars;
    u8 unk7C;
    u8 pad7D;
    u8 unk7E;
    u8 unk7F; /* set to 0x17 by seeker_update near D_L09_00166F40 */
    u8 pad80[0x10];
    s32 unk90;
    u32 unk94; /* set from the class header's word 0x10 */
    s32 unk98; /* set to 1 while a carrier holds the moby (glove_of_doom_update) */
    u8 pad9C[8];
    u8 unkA4;
    u8 padA5;
    s16 oclass;
    u8 padA8[8];
    u8 unkB0; /* 0xB0: index into the level's D_0014C050 row (0xFF: not spawned) */
    u8 padB1;
    u16 save_id; /* index into the level collected[]/killed[] tables and save bits D_0014C190[level][id >> 5] */
    s16 unkB4;
    u8 padB6[2];
    void *unkB8; /* 0xB8: bolt source record; its byte 0xB1 is a per-level id (FUN_L00_002a6b70) */
    u8 unkBC;
    u8 padBD[3];
    Vec4f_A4D88 unkC0; /* 0xC0: first row of a matrix built from rot (euler_rotation_basis) */
    Vec4f_A4D88 unkD0;
    Vec4f_A4D88 unkE0;
    u8 padF0[0x10];
};

#include "rnc/overlay/quad.h"

typedef union Vec_2E9358 {
    OvlQuad q;
    float f[4];
} Vec_2E9358;

typedef struct Data_2E9358 {
    float f0;
    float f4;
    float f8;
    float fC;
    struct Moby_2E9358 *target; /* 0x10 */
} Data_2E9358;

typedef struct Moby_2E9358 {
    char pad0[0x10];
    OvlQuad pos; /* 0x10 */
    unsigned char state; /* 0x20 */
    char pad21[0x1F];
    OvlQuad v40; /* 0x40 */
    char pad50[0x28];
    Data_2E9358 *data; /* 0x78 */
    char pad7C[0x28];
    unsigned char bA4;
    char padA5;
    short hA6;
    char padA8[0x14];
    unsigned char bBC;
} Moby_2E9358;

typedef struct Res_2E9358 {
    char pad0[0x20];
    Moby_2E9358 *owner; /* 0x20 */
} Res_2E9358;

typedef struct Tb_2E9358 {
    char pad0[0x18];
    int f18;
    int f1C;
    OvlQuad v20;
    OvlQuad v30;
    OvlQuad v40;
} Tb_2E9358;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern float D_0015ED6C;
#include "rnc/overlay/l15/gameplay/entities/conveyor_belt_globals.h"
extern float poison_vessel_gravity_gp __asm__("D_L15_00161FBC") __attribute__((sda));
extern int coll_line_fix(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int FUN_L00_001f0d60();
extern int FUN_L00_001ff660();
extern int spawn_explosion() __asm__("FUN_L00_0025e450");
extern void *suck_get_aim_height(struct Moby_A4D88 *) __asm__("FUN_002141f8");
extern void collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
int FUN_L00_00260400(int u0, char *a, void *b, void *c, float *v, void *out);
int FUN_L00_0028dc90(int i, int a1, int v, int k);
void send_blast_hits(u8 *self, OvlQuad *pos, u8 **list, s32 n, u8 *skip, s32 k, s32 c0, s32 c1, f32 f, f32 spd, f32 z) __asm__("FUN_L00_0025a9f8");
extern char D_L15_001744C0_2E9358[] __asm__("D_L15_00174440");
extern int D_L15_001744D8_2E9358 __asm__("D_L15_00174458") __attribute__((section(".data")));
extern int D_001414D0_2E9358 __asm__("D_001413D0") __attribute__((section(".data")));
extern s32 D_L15_00178500[] __asm__("D_L15_00178500");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern Res_2E9358 *trooper_find_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *FUN_L00_002603d0(int);
extern void func_L00_001FF240_2E9358(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void func_L00_001FF610_2E9358(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void func_L00_001FF4B0_2E9358(void *, void *, float) __asm__("FUN_001f9bf8");
extern int func_L00_00261478_2E9358(void *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260400");
extern void func_L00_0025F4A8_2E9358(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern int probe_world_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern void mine_blast_mobys(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");

void poison_vessel_update(Moby_2E9358 *moby) __asm__("FUN_L15_002e7f68");
void poison_vessel_update(Moby_2E9358 *moby) {
    enum {
        POISON_VESSEL_FREE_FLIGHT = 0,
        POISON_VESSEL_FOLLOW_TARGET = 1
    };
    Vec_2E9358 previous_position;
    Vec_2E9358 collision_scratch_vector;
    Vec_2E9358 temporary_vector;
    Data_2E9358 *data = moby->data;
    char *follow_context = 0;
    unsigned char state;
    int blast_candidate_count;
    Res_2E9358 *hit_result;

    previous_position.q = moby->pos;
    clear_u64_value(&collision_scratch_vector);
    if (coll_sphere(0.5f, &moby->pos, 1, moby)) {
        if (D_L15_001744D8_2E9358 == 0 || FUN_L00_002603d0(D_L15_001744D8_2E9358) == 0) {
            moby->bBC = 1;
        }
    }
    hit_result = trooper_find_hit(moby, 0x830000, 0);
    if (hit_result != 0 && hit_result->owner != 0 && hit_result->owner->hA6 != moby->hA6) {
        moby->bBC = 1;
    }
    moby->bA4 = 0xFF;

    state = moby->state;
    switch (state) {
    case POISON_VESSEL_FREE_FLIGHT:
        data->f8 = data->f8 - poison_vessel_gravity_gp * frame_time_sq;
        func_L00_001FF240_2E9358(&temporary_vector, &moby->pos, data);
        if (collision_line(&previous_position, &moby->pos, 0x10, moby, 0)) {
            if (((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18 != 0 && FUN_L00_002603d0(((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18) != 0) {
                data->target = (Moby_2E9358 *)((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18;
                moby->state = 1;
                data->f8 = 0.0f;
                moby->pos = ((Tb_2E9358 *)D_L15_001744C0_2E9358)->v20;
            } else if (((Tb_2E9358 *)D_L15_001744C0_2E9358)->f18 == D_001414D0_2E9358 || data->f8 < D_0015ED6C * -9.8f) {
                moby->bBC = 1;
                qcopy(&moby->pos, &((Tb_2E9358 *)D_L15_001744C0_2E9358)->v20);
                func_L00_001FF610_2E9358(&collision_scratch_vector, data, &((Tb_2E9358 *)D_L15_001744C0_2E9358)->v40);
                func_L00_001FF4B0_2E9358(&collision_scratch_vector, &collision_scratch_vector, D_0015ED6C + D_0015ED6C);
            }
        }
        break;
    case POISON_VESSEL_FOLLOW_TARGET:
        probe_ground_height(&moby->pos, 0, 0.5f);
        if (data->target != 0) {
            Moby_2E9358 *target_moby = data->target;

            if (target_moby->state == 0xFE || data->target->state == 0xFD) {
                moby->bBC = state;
                data->target = 0;
            } else if (D_L15_001744D8_2E9358 != 0) {
                follow_context = FUN_L00_002603d0(D_L15_001744D8_2E9358);
                if (follow_context != 0) {
                    func_L00_00261478_2E9358(moby, data->target, &moby->pos, &moby->v40, &moby->pos, &moby->v40);
                    subtract_vector_xyz(&temporary_vector, &moby->pos, &previous_position);
                    *(OvlQuad *)data = temporary_vector.q;
                }
            }
        }
        if (follow_context == 0) {
            moby->state = 0;
            data->f8 = 0.0f;
            data->target = 0;
        }
        break;
    }

    if (moby->bBC != 0) {
        func_L00_0025F4A8_2E9358(moby, data, 0, 0.0f, 0.0f, 3, 3, 5, 2.0f, 1.0f, 4.0f, 1.0f, -1, 7.0f, 0, 1, -1, 0);
        trooper_play_sound(0, 0, moby, 0x79);
        blast_candidate_count = probe_world_sphere(&moby->pos, 0x10, moby, 0, 2.0f);
        temporary_vector.q = moby->pos;
        mine_blast_mobys(moby, &temporary_vector, D_L15_00178500, blast_candidate_count, 0, 0x830001, 2, 1, 1.0f, 1.0f, 1.0f);
        mark_moby_for_removal(moby);
    }
}
/* UpdateMoby_1388 (names.tsv role). Every frame: func_001FA748 on the heading at +0x48 with
 * frame time * pi/2, and the +0x2C scale from the model's +0x24 value. State 0: runs
 * func_L00_002D80A0, then deletes the moby if flag byte 0xD is set, else raises it by 1.0
 * and goes to state 1. State 1: when the hero (D_0013E633 + 0xE9D) is within 3.0, sets
 * flags 0x41, calls func_L00_00299B68(5) and goes to state 2. State 2: once the level flag
 * is no longer 2, starts the exit sequence and goes to state 3. State 3: deletes the moby. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002EBB38), where it is exact; names translated to the US level program. */

extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int D_L15_0015F5C4;
extern int D_L15_0015F640;
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char D_0013D4FD[];
extern void grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void initialize_moby_particle_effect(char *) __asm__("FUN_L00_002d6bf0");
extern void emit_moby_sprite_particles(char *) __asm__("FUN_L10_00298940");
int memcard_save_data(int slot, int flags) __asm__("FUN_0020b178");

extern short D_L15_00162078 __attribute__((sda));
void bolt_grabber_update(struct Moby *m) __asm__("FUN_L15_002ea748");
void bolt_grabber_update(struct Moby *moby) {
    enum {
        BOLT_GRABBER_INITIALIZE = 0,
        BOLT_GRABBER_WAIT_FOR_PROXIMITY = 1,
        BOLT_GRABBER_FINALIZE_AND_SAVE = 2,
        BOLT_GRABBER_REMOVE = 3
    };
    moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
    moby->scale = moby->pclass->scale * *(float *)&D_L15_00162078;
    switch (moby->state) {
    case BOLT_GRABBER_INITIALIZE:
        initialize_moby_particle_effect((char *)moby);
        if (D_0013D4FD[0xD] != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->pos.z = moby->pos.z + 1.0f;
        break;
    case BOLT_GRABBER_WAIT_FOR_PROXIMITY:
        emit_moby_sprite_particles((char *)moby);
        if (vector_distance(&moby->pos, &hero.motion.pos) < 3.0f) {
            moby->flags |= 0x41;
            start_scene(5);
            moby->state = 2;
        }
        break;
    case BOLT_GRABBER_FINALIZE_AND_SAVE:
        if (D_L15_0015F5C4 != 2) {
            queue_dialogue_message(0x3AA3, -1);
            D_L15_0015F640 = scale_game_frames(0xB4);
            grant_inventory_item(0x22, 1);
            memcard_save_data(0, -1);
            moby->state = 3;
        }
        break;
    case BOLT_GRABBER_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}
/* Per-frame update: picks a random value on spawn, then re-rolls and spawns a child when triggered. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002EBFC8), where it is exact; names translated to the US level program. */

extern char *spawn_poison_vessel(void *, void *) __asm__("FUN_L15_002e7ed8");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern short D_L15_0016207C;
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_poison_vessel_spawner(struct Moby *moby) __asm__("FUN_L15_002eabd8");

void update_poison_vessel_spawner(struct Moby *moby) {
    enum {
        INITIALIZE_SPAWN_TIMER = 0,
        WAIT_FOR_SPAWN_TIMER = 1
    };
    int *data = (int *)moby->pvars;
    switch (moby->state) {
    case INITIALIZE_SPAWN_TIMER:
        data[0] = func_001FA898_r(multiply_global_factor_ed64(*(float *)(data + 1) * 60.0f));
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case WAIT_FOR_SPAWN_TIMER:
        if (fast_dec_timer(data)) {
            data[0] = func_001FA898_r(multiply_global_factor_ed64(*(float *)&D_L15_0016207C * 60.0f));
            spawn_poison_vessel(moby, &moby->pos);
        }
        break;
    }
}
#include "sda.h"

/* Builds the two rippling vertex strips around the moby (template points moved to its position, z waved by distance from the centre), then draws them twice with scrolled texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F1FE0), where it is exact; names translated to the US level program. */

typedef struct {
    float f0;
    float f4;
} RippleData;

typedef struct {
    float v[2];
} P2;

typedef struct {
    P2 p[2];
} T4;

T4 D_L15_00162108 = {{{{1.0f, -0.62831855f}}, {{1.0f, 0.62831855f}}}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
extern int D_L15_0015F5CC;
int D_L15_001620EC = 128;
int D_L15_001620F0 = 45;
int D_L15_001620F4 = 64;
#include "rnc/overlay/l15/gameplay/entities/conveyor_belt_data/draw_moby_radial_ripple_mesh_buffers.h"
extern float ripple_mesh_buffers_z_amplitude_gp __asm__("D_L15_00162100") __attribute__((sda));
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern float ripple_mesh_buffers_uv_scroll_rate_gp __asm__("D_L15_00162104") __attribute__((sda));
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void get_moby_color(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern void vu1_add_gs_register(s32, unsigned long) __asm__("func_00233980");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

int D_L15_00162080[2] = {0x94, 0x94};
extern char D_L15_001DBCF0[];
extern char D_L15_001DCAD0[];
int D_L15_00162090[2] = {(int)D_L15_001DBCF0, (int)D_L15_001DCAD0};
extern char D_L15_001DC3E0[];
extern char D_L15_001DD1C0[];
int D_L15_001620A0[2] = {(int)D_L15_001DC3E0, (int)D_L15_001DD1C0};
extern char D_L15_001DC880[];
extern char D_L15_001DD660[];
int D_L15_001620B0[2] = {(int)D_L15_001DC880, (int)D_L15_001DD660};
extern char D_L15_001D9E40[];
extern char D_L15_001DA530[];
int D_L15_001620C0[2] = {(int)D_L15_001D9E40, (int)D_L15_001DA530};
extern char D_L15_001DAD98[];
extern char D_L15_001DB238[];
int D_L15_001620D0[2] = {(int)D_L15_001DAD98, (int)D_L15_001DB238};

void draw_moby_radial_ripple_mesh_buffers(struct Moby *moby) __asm__("FUN_L15_002eac90");

void draw_moby_radial_ripple_mesh_buffers(struct Moby *moby) {
    RippleData *ripple_data = (RippleData *)moby->pvars;
    float scale;
    int r, g, b;
    int color;
    int i, j, uv_buffer_index, uv_vertex_index, draw_buffer_index;

    scale = convert_integer_to_float(D_L15_0015F5CC) * (ripple_mesh_buffers_uv_scroll_rate_gp * frame_time);
    vu1_add_gs_register(6, get_effect_texture(D_L15_001620F0));
    vu1_add_gs_register(0x42, ((long)D_L15_001620EC << 32) | 0x44);
    vu1_add_gs_register(8, 0);
    vu1_add_gs_register(0x14, ((long)0xFF90 << 32) | 0x260);
    ripple_data->f0 = ripple_data->f4;
    ripple_data->f4 = fast_add_rotations(ripple_data->f4,
                               360.0f / multiply_global_factor_ed64(ripple_mesh_buffers_phase_advance_divisor_gp) * DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    color = (D_L15_001620F4 << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < D_L15_00162080[i]; j++) {
            int *template_vertex_buffer_entry = D_L15_001620C0 + i;
            float *p = (float *)(*template_vertex_buffer_entry + j * 12);
            float t = ripple_mesh_buffers_z_amplitude_gp *
                      fast_sin(fast_add_rotations(
                          wrap_angle(FUN_L00_00200260(square_root(p[0] * p[0] + p[1] * p[1]),
                                                      ripple_mesh_buffers_radial_wavelength_gp) *
                                     6.2831855f / ripple_mesh_buffers_radial_wavelength_gp),
                          ripple_data->f4));
            *(float *)(j * 12 + D_L15_00162090[i]) = p[0] + moby->pos.x;
            *(float *)(j * 12 + D_L15_00162090[i] + 4) = p[1] + moby->pos.y;
            *(float *)(j * 12 + D_L15_00162090[i] + 8) = p[2] + moby->pos.z + t;
            *(int *)(j * 4 + D_L15_001620B0[i]) = color;
        }
    }
    for (i = 0; i < 2; i++) {
        for (uv_buffer_index = 0; uv_buffer_index < 3; uv_buffer_index++) {
            for (uv_vertex_index = 0; uv_vertex_index < D_L15_00162080[uv_buffer_index]; uv_vertex_index++) {
                T4 uv_scroll_factors = D_L15_00162108;
                *(float *)(uv_vertex_index * 8 + D_L15_001620A0[uv_buffer_index]) =
                    *(float *)(uv_vertex_index * 8 + D_L15_001620D0[uv_buffer_index]) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[0], 1.0f);
                *(float *)(uv_vertex_index * 8 + D_L15_001620A0[uv_buffer_index] + 4) =
                    *(float *)(uv_vertex_index * 8 + D_L15_001620D0[uv_buffer_index] + 4) +
                    FUN_L00_00200260(scale * uv_scroll_factors.p[i].v[1], 1.0f);
            }
        }
        font_queue_vu_state();
        for (draw_buffer_index = 0; draw_buffer_index < 3; draw_buffer_index++) {
            FUN_L00_001fde98(D_L15_00162080[draw_buffer_index], D_L15_00162090[draw_buffer_index], D_L15_001620B0[draw_buffer_index],
                             (void *)D_L15_001620A0[draw_buffer_index], 1);
        }
    }
}
#include "sda.h"

/* Builds and draws the animated ring of lights around a moby: 68 vertices and colours, then two passes of 68 texture coordinates. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F2BD8), where it is exact; names translated to the US level program. */

typedef struct {
    float f[4];
} Quad4;

typedef struct {
    float prev;
    float angle;
} RingVars;

Quad4 D_L15_00162170 = {{1.0f, -0.62831855f, 1.0f, 0.62831855f}};
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float ripple_mesh_uv_scroll_rate_gp __asm__("D_L15_0016216C") __attribute__((sda));
float D_L15_001DDBA0[204] = {3.485078f, 4.343359f, 0.0f, 5.279609f, 3.600078f, 0.0f, 2.980781f, 2.623281f, 0.0f, 4.615703f, 1.997344f, 0.0f, 2.298828f, 0.976816f, 0.0f, 3.933828f, 0.351152f, 0.0f, 1.616797f, -0.669707f, 0.0f, 3.237344f, -1.330234f, 0.0f, 0.850156f, -2.520547f, 0.0f, 2.454375f, -3.220508f, 0.0f, -0.025f, -4.63334f, 0.0f, 1.594062f, -5.297578f, 0.0f, -0.025f, -4.63334f, 0.0f, 0.733984f, -7.373926f, -5e-06f, -0.025f, -4.63334f, 0.0f, -1.060508f, -6.630625f, 0.0f, -1.641797f, -3.963652f, 0.0f, -2.49957f, -6.034551f, 0.0f, -3.258633f, -3.293945f, 0.0f, -3.938672f, -5.438457f, 0.0f, -3.258633f, -3.293945f, 0.0f, -5.733164f, -4.695137f, 0.0f, -3.258633f, -3.293945f, 0.0f, -4.873164f, -2.618789f, 0.0f, -2.383437f, -1.181152f, 0.0f, -4.012774f, -0.541719f, 0.0f, -1.616797f, 0.669688f, 0.0f, -3.229805f, 1.348555f, 0.0f, -0.934805f, 2.316172f, 0.0f, -2.53332f, 3.029922f, 0.0f, -0.252813f, 3.962656f, 0.0f, -1.851445f, 4.676094f, 0.0f, -0.252813f, 3.962656f, 0.0f, -1.187617f, 6.278828f, 0.0f, -0.252813f, 3.962656f, 0.0f, 0.606953f, 5.535547f, 0.0f, 1.363984f, 3.292969f, 0.0f, 2.046016f, 4.939453f, 0.0f, 1.363984f, 3.292969f, 0.0f, 3.485078f, 4.343359f, 0.0f, 1.363984f, 3.292969f, 0.0f, 2.980781f, 2.623281f, 0.0f, 1.363984f, 3.292969f, 0.0f, 2.298828f, 0.976816f, 0.0f, 1.363984f, 3.292969f, 0.0f, 0.681953f, 1.646484f, 0.0f, -0.252813f, 3.962656f, 0.0f, 0.681953f, 1.646484f, 0.0f, -0.934805f, 2.316172f, 0.0f, 0.681953f, 1.646484f, 0.0f, -1.616797f, 0.669688f, 0.0f, 0.681953f, 1.646484f, 0.0f, 0.0f, 0.0f, 0.0f, 2.298828f, 0.976816f, 0.0f, 0.0f, 0.0f, 0.0f, 1.616797f, -0.669707f, 0.0f, -0.766641f, -1.85084f, 0.0f, 0.850156f, -2.520547f, 0.0f, -0.766641f, -1.85084f, 0.0f, -0.025f, -4.63334f, 0.0f, -0.766641f, -1.85084f, 0.0f, -1.641797f, -3.963652f, 0.0f, -0.766641f, -1.85084f, 0.0f, -3.258633f, -3.293945f, 0.0f, -0.766641f, -1.85084f, 0.0f, -2.383437f, -1.181152f, 0.0f, 0.0f, 0.0f, 0.0f, -1.616797f, 0.669688f, 0.0f};
float D_L15_001DDED0[68][2] = {
    {2.945454f, -1.355928f},
    {2.573813f, -2.253193f},
    {2.085415f, -1.103779f},
    {1.772446f, -1.92124f},
    {1.262183f, -0.762803f},
    {0.949351f, -1.580303f},
    {0.438921f, -0.421787f},
    {0.108657f, -1.23206f},
    {-0.486499f, -0.038467f},
    {-0.836479f, -0.840576f},
    {-1.542895f, 0.399111f},
    {-1.875015f, -0.41042f},
    {-1.542895f, 0.399111f},
    {-2.913188f, 0.019619f},
    {-1.542895f, 0.399111f},
    {-2.541538f, 0.916865f},
    {-1.208052f, 1.20751f},
    {-2.243501f, 1.636396f},
    {-0.873198f, 2.015928f},
    {-1.945454f, 2.355947f},
    {-0.873198f, 2.015928f},
    {-1.573794f, 3.253193f},
    {-0.873198f, 2.015928f},
    {-0.53562f, 2.823193f},
    {0.183198f, 1.57833f},
    {0.502915f, 2.392998f},
    {1.108618f, 1.19501f},
    {1.448052f, 2.001514f},
    {1.93186f, 0.854014f},
    {2.288735f, 1.653272f},
    {2.755102f, 0.513018f},
    {3.111821f, 1.312334f},
    {2.755102f, 0.513018f},
    {3.913188f, 0.98042f},
    {2.755102f, 0.513018f},
    {3.541548f, 0.083135f},
    {2.420259f, -0.295381f},
    {3.243501f, -0.636396f},
    {2.420259f, -0.295381f},
    {2.945454f, -1.355928f},
    {2.420259f, -0.295381f},
    {2.085415f, -1.103779f},
    {2.420259f, -0.295381f},
    {1.262183f, -0.762803f},
    {2.420259f, -0.295381f},
    {1.597017f, 0.045635f},
    {2.755102f, 0.513018f},
    {1.597017f, 0.045635f},
    {1.93186f, 0.854014f},
    {1.597017f, 0.045635f},
    {1.108618f, 1.19501f},
    {1.597017f, 0.045635f},
    {0.773774f, 0.386611f},
    {1.262183f, -0.762803f},
    {0.773774f, 0.386611f},
    {0.438921f, -0.421787f},
    {-0.151646f, 0.769932f},
    {-0.486499f, -0.038467f},
    {-0.151646f, 0.769932f},
    {-1.542895f, 0.399111f},
    {-0.151646f, 0.769932f},
    {-1.208052f, 1.20751f},
    {-0.151646f, 0.769932f},
    {-0.873198f, 2.015928f},
    {-0.151646f, 0.769932f},
    {0.183198f, 1.57833f},
    {0.773774f, 0.386611f},
    {1.108618f, 1.19501f},
};
float D_L15_001DE200[204] = {0};
float D_L15_001DE530[68][2] = {0};
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float FUN_L00_00200260(float, float);
extern float fast_add_rotations(float, float);
extern float wrap_angle(float);
extern int D_L15_0015F5CC;
int D_L15_00162120[2] = {0x44, 0};
int D_L15_00162140[2] = {(int)D_L15_001DE200, 0};
int D_L15_00162148[2] = {(int)D_L15_001DE530, 0};
int D_L15_001DE750[68] = {0};
int D_L15_00162150[1] = {(int)D_L15_001DE750};
int D_L15_00162154 = 128;
int D_L15_00162158 = 45;
int D_L15_0016215C = 64;
extern float ripple_mesh_radial_wavelength_gp __asm__("D_L15_00162160") __attribute__((sda));
#include "rnc/overlay/l15/gameplay/entities/conveyor_belt_data/draw_moby_radial_ripple_mesh.h"
extern float ripple_mesh_z_amplitude_gp __asm__("D_L15_00162168") __attribute__((sda));
extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void get_moby_color(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");

void draw_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002eb108");

void draw_moby_radial_ripple_mesh(struct Moby *moby) {
    unsigned int i = 0;
    unsigned int output_vertex_byte_offset = 0;
    RingVars *data = (RingVars *)moby->pvars;
    unsigned int scrolled_uv_vertex_index;
    int j;
    int k;
    int color;
    Quad4 uv_scroll_factors;
    int r;
    int g;
    int b;
    float scale;
    Quad4 *v_scroll_factors;
    float *output_vertex;
    scale = convert_integer_to_float(D_L15_0015F5CC) * (ripple_mesh_uv_scroll_rate_gp * frame_time);
    vu1_add_gs_register(6, get_effect_texture(D_L15_00162158));
    vu1_add_gs_register(0x42, ((long)D_L15_00162154 << 32) | 0x44);
    vu1_add_gs_register(8, 0);
    vu1_add_gs_register(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = fast_add_rotations(data->angle, 360.0f / multiply_global_factor_ed64(ripple_mesh_phase_advance_divisor_gp) *
                                                      DEG_TO_RAD * frame_time);
    get_moby_color(moby, &r, &g, &b);
    /* The second float of each pair in t: p->f[0] is t.f[1], p->f[2] is t.f[3]. */
    v_scroll_factors = (Quad4 *)&uv_scroll_factors.f[1];
    color = (D_L15_0016215C << 24) | (b << 16) | (g << 8) | r;
    output_vertex = D_L15_001DE200;
    do {
        float *template_vertex = &D_L15_001DDBA0[i * 3];
        float x = template_vertex[0];
        float y = template_vertex[1];
        float rad = square_root(x * x + y * y);
        float radius_remainder = FUN_L00_00200260(rad, ripple_mesh_radial_wavelength_gp);
        float w = wrap_angle(radius_remainder * 6.2831855f / ripple_mesh_radial_wavelength_gp);
        float z = fast_sin(fast_add_rotations(w, data->angle));
        output_vertex[0] = template_vertex[0] + moby->pos.x;
        *(float *)((char *)D_L15_001DE200 + 4 + output_vertex_byte_offset) = template_vertex[1] + moby->pos.y;
        output_vertex[2] = template_vertex[2] + moby->pos.z + ripple_mesh_z_amplitude_gp * z;
        D_L15_001DE750[i] = color;
        i++;
        output_vertex_byte_offset += 12;
        output_vertex += 3;
    } while (i < 68);
    for (j = 0; j < 2; j++) {
        for (scrolled_uv_vertex_index = 0; scrolled_uv_vertex_index < 68; scrolled_uv_vertex_index++) {
            uv_scroll_factors = D_L15_00162170;
            D_L15_001DE530[scrolled_uv_vertex_index][0] =
                D_L15_001DDED0[scrolled_uv_vertex_index][0] + FUN_L00_00200260(scale * uv_scroll_factors.f[j * 2], 1.0f);
            D_L15_001DE530[scrolled_uv_vertex_index][1] =
                D_L15_001DDED0[scrolled_uv_vertex_index][1] + FUN_L00_00200260(scale * v_scroll_factors->f[j * 2], 1.0f);
        }
        font_queue_vu_state();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98(D_L15_00162120[k], D_L15_00162140[k], D_L15_00162150[k],
                             (void *)D_L15_00162148[k], 1);
        }
    }
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"
/* Cutscene trigger moby: waits for the hero in its clip volume, then steps through the scene stages and saves at the end. */
extern int D_L15_0015F5C4;
extern int D_L15_00161AC0 __asm__("D_L15_00161AC0");
extern unsigned char *D_L15_0015FFD8_v __asm__("D_L15_0015FFD8");
extern char *D_L15_001600EC;
extern unsigned char D_0014C050_v[] __asm__("D_0014C050");
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void start_scene_p(int) __asm__("FUN_L00_00298840");
extern void FUN_L00_002502a0_v(int) __asm__("FUN_L00_002502a0");
extern void FUN_L00_00232fe8_v(void) __asm__("FUN_L00_00232fe8");
extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void FUN_L00_002330d0_v(void) __asm__("FUN_L00_002330d0");
extern void FUN_L00_00216f90_v(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L00_002607d0_v(int) __asm__("FUN_L00_002607d0");
extern void FUN_L00_00284e50_v(void *, void *) __asm__("FUN_L00_00284e50");
extern void queue_planet_unlock_message(int) __asm__("FUN_L00_00262d38");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
void draw_quartu_cutscene_messages(void) __asm__("FUN_L15_002eb6b8");

void draw_quartu_cutscene_messages(void) __asm__("FUN_L15_002eb6b8");

void update_quartu_cutscene_trigger(struct Moby *moby) __asm__("FUN_L15_002eb4c0");

void update_quartu_cutscene_trigger(struct Moby *moby) {
    enum {
        CUTSCENE_TRIGGER_WAIT_FOR_CONDITIONS = 0,
        CUTSCENE_TRIGGER_START_SECOND_SCENE = 1,
        CUTSCENE_TRIGGER_WAIT_FOR_FIRST_SCENE = 2,
        CUTSCENE_TRIGGER_FINALIZE_AND_SAVE = 3,
        CUTSCENE_TRIGGER_COMPLETED = 4
    };
    char *trigger_data = (char *)moby->pvars;
    moby->update_dist = 0x80;
    switch (moby->state) {
    case CUTSCENE_TRIGGER_WAIT_FOR_CONDITIONS:
        if (D_0014C050_v[moby->mission + current_level_index * 16] == 0xFF)
            break;
        if (D_L15_00161AC0 != 0)
            break;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)trigger_data) == 0)
            break;
        if (D_L15_0015FFD8_v[(*(int *)(trigger_data + 8) << 8) + 0x20] != 3)
            break;
        start_scene_p(3);
        moby->state = 2;
        FUN_L00_002502a0_v(moby->mission);
        FUN_L00_00232fe8_v();
        break;
    case CUTSCENE_TRIGGER_WAIT_FOR_FIRST_SCENE:
        if (D_L15_0015F5C4 != 2) {
            queue_scene_movie_with_game_language(0x11);
            moby->state = 1;
        }
        break;
    case CUTSCENE_TRIGGER_START_SECOND_SCENE:
        if (D_L15_0015F5C4 != 2) {
            start_scene_p(4);
            moby->state = 3;
            FUN_L00_002330d0_v();
            if (*(int *)(trigger_data + 4) != -1) {
                char *o = D_L15_001600EC + (*(int *)(trigger_data + 4) << 7);
                FUN_L00_00216f90_v(o + 0x30, o + 0x70, 0, 1);
            }
        }
        break;
    case CUTSCENE_TRIGGER_FINALIZE_AND_SAVE:
        if (D_L15_0015F5C4 != 2) {
            FUN_L00_002607d0_v(0x11);
            if (*(int *)(trigger_data + 4) != -1) {
                char *o = D_L15_001600EC + (*(int *)(trigger_data + 4) << 7);
                FUN_L00_00284e50_v(o + 0x30, o + 0x70);
            }
            queue_planet_unlock_message(0x11);
            memcard_save_data(0, -1);
            moby->state = 4;
        }
        break;
    case CUTSCENE_TRIGGER_COMPLETED:
        break;
    }
    if (D_L15_0015F5C4 == 2)
        enqueue_callback_list_1(draw_quartu_cutscene_messages, moby);
}
#define NOT_SDA __attribute__((section(".data")))

#define MACRO_ADDR __attribute__((section(".sdata")))

/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002ECAA8), where it is exact; names translated to the US level program. */

extern char *msg_string(s32) __asm__("func_001FDD10");
extern char D_L15_00162190[];
extern char D_L15_0016CE60[];
extern int sprintf(char *str, const char *fmt, ...) __asm__("FUN_00116248");
extern int strlen_alt(char *) __asm__("FUN_001166cc");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
void font_print_large(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_001f6530");
extern void print_text_with_texture_3(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_L00_001fb470");

int D_L15_00162180 = 150;
int D_L15_00162184 = 150;
int D_L15_00162188 = 0x3800ffff;
int D_L15_0016218C = 0x38800000;
void draw_quartu_cutscene_messages(void) {
    char formatted_message_text[0x50];
    int flag = 0;
    int val = 0;
    char *message_timing_state = D_L15_0016CE60;
    int print_character_count;
    char *second_message_timing_state;
    char *third_message_timing_state;
    if (*(int *)(message_timing_state + 0x30) == 3) {
        if (scale_game_frames(0xB5) < *(int *)(message_timing_state + 0x34)) {
            if (*(int *)(message_timing_state + 0x34) < scale_game_frames(0x104)) {
                flag = 1;
                sprintf(formatted_message_text, D_L15_00162190, msg_string(0x526A));
                val = (*(int *)(message_timing_state + 0x34) - scale_game_frames(0xB5)) >> 2;
            }
        }
        second_message_timing_state = D_L15_0016CE60;
        if (scale_game_frames(0x1C2) < *(int *)(second_message_timing_state + 0x34)) {
            if (*(int *)(second_message_timing_state + 0x34) < scale_game_frames(0x280)) {
                flag = 1;
                sprintf(formatted_message_text, D_L15_00162190, msg_string(0x526B));
                val = (*(int *)(second_message_timing_state + 0x34) - scale_game_frames(0x1C2)) >> 2;
            }
        }
        third_message_timing_state = D_L15_0016CE60;
        if (scale_game_frames(0x2EE) < *(int *)(third_message_timing_state + 0x34)) {
            if (*(int *)(third_message_timing_state + 0x34) < scale_game_frames(0x3A2)) {
                flag = 1;
                sprintf(formatted_message_text, D_L15_00162190, msg_string(0x526C));
                val = (*(int *)(third_message_timing_state + 0x34) - scale_game_frames(0x2EE)) >> 2;
            }
        }
    } else if (*(int *)(message_timing_state + 0x30) == 4) {
        if (scale_game_frames(0x8FC) < *(int *)(message_timing_state + 0x34)) {
            if (*(int *)(message_timing_state + 0x34) < scale_game_frames(0x960)) {
                flag = 1;
                sprintf(formatted_message_text, D_L15_00162190, msg_string(0x526E));
                val = (*(int *)(message_timing_state + 0x34) - scale_game_frames(0x8FC)) >> 2;
            }
        }
    }
    if (flag == 0)
        return;
    print_character_count = strlen_alt(formatted_message_text);
    if (val < print_character_count) {
        print_character_count = val;
    } else if (print_character_count < val) {
        if ((print_character_count - val) & 4)
            print_character_count = print_character_count - 1;
    }
    print_text_with_texture_3((void *)(D_L15_00162180 + 1),
                     (void *)(D_L15_00162184 + 1), (void *)D_L15_0016218C,
                     formatted_message_text, (void *)print_character_count);
    print_text_with_texture_3((void *)D_L15_00162180, (void *)D_L15_00162184,
                     (void *)D_L15_00162188, formatted_message_text, (void *)print_character_count);
}
/* Per-frame update: when the counter check is zero, builds a vector and calls the movement helper. */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002ECD18), where it is exact; names translated to the US level program. */

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void FUN_L00_0026f080(float, float, char *, float *);
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");

extern short D_L15_00162198 __attribute__((sda));
int D_L15_0016219C = 3;
void emit_randomized_vertical_effect(struct Moby *moby) __asm__("FUN_L15_002eb928");

void emit_randomized_vertical_effect(struct Moby *moby) {
    float vertical_motion_vector[4];
    float *data = (float *)moby->pvars;
    if (random_integer_below(D_L15_0016219C - 1) == 0) {
        char *pos = &moby->pos;
        clear_u64_value(vertical_motion_vector);
        vertical_motion_vector[2] = random_float_between(0.75f, 1.5f) * *(float *)&D_L15_00162198 * frame_time;
        FUN_L00_0026f080(random_float_between(0.06f, 0.12f) * 210000.0f, *data, pos, vertical_motion_vector);
    }
}
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_0029C1D0.c: func_L15_002ECDD0), where it is exact; names translated to the US level program. */


extern char *func_0020D348_m(int);
extern int copy_hero_moby_field_38() __asm__("FUN_L00_0025d1b8");
extern char *func_0020D348_c(int) __asm__("FUN_0020c4f8");
extern void func_L00_0025E210_u(void *) __asm__("FUN_L00_0025d1b8");
extern void upd_a(void *) __asm__("FUN_L00_00250df8");

char *spawn_codebot_slot_effect(char *spawn_position, char *spawn_rotation) __asm__("FUN_L15_002eb9e0");

char *spawn_codebot_slot_effect(char *spawn_position, char *spawn_rotation) {
    char saved_spawn_position_and_rotation[32] __attribute__((aligned(16)));
    char *saved_position = saved_spawn_position_and_rotation;
    char *spawned_moby;
    *(OvlQuad *)saved_spawn_position_and_rotation = *(OvlQuad *)spawn_position;
    *(OvlQuad *)(saved_spawn_position_and_rotation + 0x10) = *(OvlQuad *)spawn_rotation;
    spawned_moby = func_0020D348_c(0x594);
    if (spawned_moby != 0) {
        func_L00_0025E210_u(spawned_moby);
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0xFF;
        spawned_moby[0x31] = 1;
        qcopy(spawned_moby + 0x10, saved_position);
        *(OvlQuad *)(spawned_moby + 0x40) = *(OvlQuad *)(saved_spawn_position_and_rotation + 0x10);
        upd_a(spawned_moby);
    }
    return spawned_moby;
}
/* Shows or hides every listed moby. */

extern int D_L15_001ABE40[];
extern int D_L15_0015FFD8;

void set_moby_list_visibility(int moby_list_index, int show) __asm__("FUN_L15_002ebf28");

void set_moby_list_visibility(int moby_list_index, int show) {
    short *moby_index_entry = (short *)D_L15_001ABE40[moby_list_index];
    if (moby_index_entry == 0)
        return;
    while (1) {
        unsigned char *moby = (unsigned char *)(D_L15_0015FFD8 + ((*moby_index_entry & 0x7FFF) << 8));
        if (show) {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
            moby[0x31] = 1;
            *(unsigned short *)(moby + 0x34) &= 0xFFFC;
        } else {
            *(int *)(moby + 0x94) = 0;
            moby[0x31] = 0;
            *(unsigned short *)(moby + 0x34) |= 3;
        }
        if (*moby_index_entry++ < 0)
            return;
    }
}
/* Counts the listed mobys that are missing or dying. */

extern int D_L15_001ABE40[];
extern int D_L15_0015FFD8;

int count_missing_or_dying_mobys_in_list(int moby_list_index) __asm__("FUN_L15_002ebfb0");

int count_missing_or_dying_mobys_in_list(int moby_list_index) {
    unsigned short *moby_index_entry = (unsigned short *)D_L15_001ABE40[moby_list_index];
    int missing_or_dying_count = 0;
    int base;
    if (moby_index_entry == 0)
        return 0;
    base = D_L15_0015FFD8;
    do {
        unsigned char *moby = (unsigned char *)(base + ((moby_index_entry[0] & 0x7FFF) << 8));
        if (moby == 0 || moby[0x20] == 0xFE || moby[0x20] == 0xFD)
            missing_or_dying_count++;
    } while ((short)*moby_index_entry++ >= 0);
    return missing_or_dying_count;
}
typedef struct {
    u8 pad[0x454];
    u8 killed[1]; /* per spawn id */
} KillFlags;
extern KillFlags D_L15_001BB830;
extern unsigned int D_L15_001BAAD0[];
extern unsigned int D_L15_0014C190[];
extern int D_L15_0015ED98;
extern volatile int D_L15_00184AAC __attribute__((section(".data")));
extern int D_L15_0015FFD8;
extern float D_0013F3D0[];
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_001ff290(void *, void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void FUN_L15_002f78b8(int);
int can_spawn_codebot_slot_effect(int slot) __asm__("FUN_L15_002f78e0");

extern int can_spawn_codebot_slot_effect(int) __asm__("FUN_L15_002f78e0");
extern int count_missing_or_dying_mobys_in_list(int) __asm__("FUN_L15_002ebfb0");
extern void set_moby_list_visibility(int, int) __asm__("FUN_L15_002ebf28");
extern char *spawn_codebot_slot_effect(char *, char *) __asm__("FUN_L15_002eb9e0");
extern void FUN_L00_00216f90(void *, void *, int, int);

/* Bind this call to its checked overlay instance rather than a duplicate. */
extern float scale_time(float) __asm__("FUN_L15_001f8648");

#include "rnc/gameplay/state/item_state.h"

void codebot_slot_update(struct Moby *moby) __asm__("FUN_L15_002ec030");
void codebot_slot_update(struct Moby *moby) {
    enum {
        CODEBOT_SLOT_INITIALIZE = 0,
        CODEBOT_SLOT_WAIT_FOR_PROXIMITY = 1,
        CODEBOT_SLOT_WAIT_TO_CREATE_EFFECT = 2,
        CODEBOT_SLOT_ANIMATE_EFFECT = 3,
        CODEBOT_SLOT_COMPLETED = 4
    };
    int *data = (int *)moby->pvars;
    float position_work_vector[4] __attribute__((aligned(16)));
    float position_rotation_work_vector[4] __attribute__((aligned(16)));
    OvlQuad position;
    float angle;
    char *effect;
    if (data[3] != -1) {
        if (moby->state != 0) {
            subtract_vector_xyz(position_rotation_work_vector, (char *)D_L15_0015FFD8 + data[3] * 0x100 + 0x10, data + 4);
            *(OvlQuad *)position_work_vector = *(OvlQuad *)position_rotation_work_vector;
            FUN_L00_001ff290(position_rotation_work_vector, &moby->pos, position_work_vector);
        }
        *(OvlQuad *)(data + 4) = *(OvlQuad *)((char *)D_L15_0015FFD8 + data[3] * 0x100 + 0x10);
    }
    switch (moby->state) {
    case CODEBOT_SLOT_INITIALIZE: {
        int save_index;
        int i;
        save_index = ((int)((unsigned int)moby->save_id << 16)) >> 16;
        if (D_L15_001BB830.killed[save_index] != 0 ||
            ((*(int *)(level_save_bank_index * 0x100 + (save_index >> 5) * 4 + (char *)D_L15_0014C190) >> (moby->save_id & 31) & 1) != 0)) {
            moby->state = 4;
            break;
        }
        D_L15_00184AAC = 0;
        moby->state = 1;
        for (i = 0; i < 24; i++) {
            if (data[i + 8] != -1)
                set_moby_list_visibility(data[i + 8], 0);
        }
        break;
    }
    case CODEBOT_SLOT_WAIT_FOR_PROXIMITY:
        if (alternate_item_available[2] != 0 && vector_distance(D_0013F3D0, &moby->pos) < 3.0f && *((unsigned char *)D_0013F3D0 + 0x2024) == 0) {
            FUN_L15_002f78b8(data[0]);
            moby->state = 2;
        }
        break;
    case CODEBOT_SLOT_WAIT_TO_CREATE_EFFECT:
        if (can_spawn_codebot_slot_effect(data[0])) {
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 2.0f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 2.0f;
            position_work_vector[2] = 0.0f;
            FUN_L00_001ff290(&position, position_work_vector, &moby->pos);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            position_rotation_work_vector[1] = 1.5707964f;
            data[1] = (int)spawn_codebot_slot_effect((char *)position_work_vector, (char *)position_rotation_work_vector);
            data[2] = 0;
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 3.0f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 3.0f;
            position_work_vector[2] = 0.0f;
            FUN_L00_001ff290(&position, position_work_vector, &moby->pos);
            position_work_vector[2] = probe_ground_height(position_work_vector, 0, 0.5f);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            FUN_L00_00216f90(position_work_vector, position_rotation_work_vector, 0x72, 0);
            if (data[8] != -1)
                set_moby_list_visibility(data[8], 1);
            moby->state = 3;
        }
        break;
    case CODEBOT_SLOT_ANIMATE_EFFECT:
        if (data[1] != 0) {
            *(float *)&data[2] = fast_add_rotations(*(float *)&data[2], (360.0f / scale_time(360.0f)) * 0.017453292f);
            angle = fast_cos(*(float *)&data[2]) + 1.0f;
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * angle;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * angle;
            position_work_vector[2] = 0.0f;
            effect = (char *)data[1];
            add_vector_xyz(position_rotation_work_vector, &moby->pos, position_work_vector);
            *(OvlQuad *)(effect + 0x10) = *(OvlQuad *)position_rotation_work_vector;
            if (angle < 0.5f) {
                unsigned int *bits;
                bits = (unsigned int *)(((short)moby->save_id >> 5) * 4 + level_save_bank_index * 0x100 + (char *)D_L15_0014C190);
                *bits |= 1u << (moby->save_id & 31);
                {
                    unsigned int *bits = &D_L15_001BAAD0[(short)moby->save_id >> 5];
                    *bits |= 1u << (moby->save_id & 31);
                }
                moby->state = 4;
            }
        }
        break;
    case CODEBOT_SLOT_COMPLETED:
        D_L15_00184AAC = 1;
        if (*(volatile int *)(data + 1) == 0) {
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[2] = 0.0f;
            FUN_L00_001ff290(&position, position_work_vector, &moby->pos);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            position_rotation_work_vector[1] = 1.5707964f;
            *(OvlQuad *)&position = *(OvlQuad *)&moby->pos;
            data[1] = (int)spawn_codebot_slot_effect((char *)&position, (char *)position_rotation_work_vector);
        } else {
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[2] = 0.0f;
            FUN_L00_001ff290(&position, position_work_vector, &moby->pos);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            position_rotation_work_vector[1] = 1.5707964f;
            *(OvlQuad *)((char *)data[1] + 0x10) = *(OvlQuad *)position_work_vector;
            *(OvlQuad *)((char *)data[1] + 0x40) = *(OvlQuad *)position_rotation_work_vector;
        }
        if (alternate_item_available[0x6D] == 0 && count_missing_or_dying_mobys_in_list(data[8]) > 9) {
            alternate_item_available[0x6D] = 1;
            D_L15_0015ED98 += 15000;
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_002EDB50.c: func_L15_002EDB50). */

extern char *D_L15_0015FFE4 __attribute__((section(".sdata")));
extern char D_0013E533[];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 frame_scale_sq_gp __asm__("D_0015ED64") __attribute__((section(".sdata")));
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern s32 current_level_index_gp __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern int update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern int initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int count_active_moby_group_members(int, int) __asm__("FUN_L01_0026e008");
int memcard_save_data(int slot, int flags) __asm__("FUN_0020b178");
extern unsigned char big_head_cheat_enabled_macro[] __asm__("D_0015EDB0") __attribute__((section(".sdata")));
extern void FUN_L00_002502a0(int);
extern void unlock_planet(int) __asm__("FUN_L00_002607d0");
extern void update_joint_animation(void *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void queue_planet_unlock_message(int) __asm__("FUN_L00_00262d38");
extern void FUN_L00_00284e50(void *, void *);
extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void FUN_L01_0027a248(int, int);
extern void initialize_moby_grounding(void *) __asm__("FUN_L02_0025c758");
extern void apply_npc_big_head_cheat(void *) __asm__("FUN_L12_002e6b60");
extern void restore_saved_hero_moby_and_clear_vehicle_mode(void) __asm__("FUN_L15_00208ca8");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");
#include "rnc/gameplay/state/game_globals.h"
#include "rnc/gameplay/state/level_state.h"

void ultramech_scientist_update(struct Moby *moby) __asm__("FUN_L15_002ec760");
void ultramech_scientist_update(struct Moby *moby) {
    enum {
        ULTRAMECH_SCIENTIST_INITIALIZE = 0,
        ULTRAMECH_SCIENTIST_WAIT_FOR_DIALOGUE = 1,
        ULTRAMECH_SCIENTIST_RESUME_AFTER_DIALOGUE = 2,
        ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_CONDITIONS = 3,
        ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_DELAY = 4,
        ULTRAMECH_SCIENTIST_START_FIRST_SCENE = 5,
        ULTRAMECH_SCIENTIST_WAIT_FOR_FIRST_SCENE = 6,
        ULTRAMECH_SCIENTIST_START_SECOND_SCENE_AND_SAVE = 7,
        ULTRAMECH_SCIENTIST_SAVE_AND_DISABLE = 8
    };
    char *data;
    char *hero_pos;
    struct Moby *other;
    int control_mode;

    data = (char *)moby->pvars;
    apply_npc_big_head_cheat(moby);
    if (*(int *)(data + 0x14C) == -1) {
        return;
    }
    {
        char *player = D_0013E533 + 0xE1D;
        if (*(unsigned char *)(player + 0x20A4) != 2) {
            int mobys = D_L15_0015FFD8;
            *(int *)((*(int *)(data + 0x14C) << 8) + mobys + 0x94) = 0;
            *(unsigned short *)((*(int *)(data + 0x14C) << 8) + mobys + 0x34) |= 0x41;
        }
    }
    if (moby->state == 9) {
        moby->update_dist = 0;
        moby->collision = 0;
        moby->flags |= 0x41;
        return;
    }
    if (D_L15_0015F5C4 == 2) {
        moby->collision = 0;
        moby->flags |= 0x41;
    } else {
        moby->collision = moby->pclass->collision;
        moby->flags &= 0xFFBE;
        initialize_moby_grounding(moby);
    }

    switch (moby->state) {
    case ULTRAMECH_SCIENTIST_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        *(float *)(data + 0xC) = 20.0f;
        initialize_npc_dialogue(moby, data);
        if (visited52[*(int *)(data + 0x280) + (current_level_index_gp << 4)] != 0xFF) {
            break;
        }
        if (level_available[16] != 0) {
            moby->state = 9;
            break;
        }
        moby->state = 3;
        other = (struct Moby *)D_L15_0015FFE4;
        if (other == 0) {
            break;
        }
        {
        int dead_state = 0xFE;
        int gone_state = 0xFD;
        for (; other != 0; other = other->next) {
            if (other->mission != *(int *)(data + 0x280)) {
                continue;
            }
            if (other == moby) {
                continue;
            }
            if (other == 0) {
                continue;
            }
            if (other->state == dead_state) {
                continue;
            }
            if (other->state == gone_state) {
                continue;
            }
            mark_moby_for_removal(other);
        }
        }
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_DIALOGUE:
        hero_pos = D_0013E533 + 0xE9D;
        if (is_point_inside_clip_volume(hero_pos, *(int *)(data + 0x27C)) == 0) {
            break;
        }
        control_mode = *(int *)(hero_pos + 0x200C);
        if ((unsigned int)control_mode >= 2 && control_mode != 9) {
            break;
        }
        if (update_npc_dialogue(moby, data) == 0) {
            break;
        }
        FUN_L00_002502a0(*(int *)(data + 0x280));
        moby->anim_speed = 0.0f;
        moby->state = 2;
        other = (struct Moby *)D_L15_0015FFE4;
        if (other == 0) {
            break;
        }
        {
        int dead_state = 0xFE;
        int gone_state = 0xFD;
        for (; other != 0; other = other->next) {
            if (other->mission != *(int *)(data + 0x280)) {
                continue;
            }
            if (other == moby) {
                continue;
            }
            if (other == 0) {
                continue;
            }
            if (other->state == dead_state) {
                continue;
            }
            if (other->state == gone_state) {
                continue;
            }
            mark_moby_for_removal(other);
        }
        }
        break;
    case ULTRAMECH_SCIENTIST_RESUME_AFTER_DIALOGUE:
        if (D_L15_0015F5C4 == 2) {
            break;
        }
        moby->anim_speed = 1.0f;
        if (*(short *)(data + 0x4) == 0) {
            if (*(int *)(data + 0x150) != -1) {
                char *paths = D_L15_001600EC;
                FUN_L00_00284e50(paths + (*(int *)(data + 0x150) << 7) + 0x30, paths + (*(int *)(data + 0x150) << 7) + 0x70);
            }
            moby->state = 3;
        } else {
            moby->state = 9;
        }
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_CONDITIONS:
        {
            char *player = D_0013E533 + 0xE1D;
            if (*(unsigned char *)(player + 0x20A4) != 2) {
                break;
            }
        }
        if (*(int *)(data + 0x158) != -1) {
            char *spawned = (char *)D_L15_0015FFD8 + (*(int *)(data + 0x158) << 8);
            if (spawned != 0) {
                int state = *(unsigned char *)(spawned + 0x20);
                if (state != 0xFE) {
                    if (state != 0xFD) {
                        mark_moby_for_removal(spawned);
                        *(int *)(data + 0x158) = -1;
                    }
                }
            }
        }
        if (count_active_moby_group_members(*(int *)(data + 0x144), 0x40) != 0) {
            break;
        }
        if (count_active_moby_group_members(*(int *)(data + 0x148), 0x40) != 0) {
            break;
        }
        moby->state = 4;
        *(int *)(data + 0x15C) = scale_game_frames(120);
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_PRE_SCENE_DELAY:
        if (fast_dec_timer(data + 0x15C) == 0) {
            break;
        }
        restore_saved_hero_moby_and_clear_vehicle_mode();
        set_moby_animation((char *)D_L15_0015FFD8 + (*(int *)(data + 0x14C) << 8), 0, 0);
        *(int *)(data + 0x140) = 1;
        *(short *)(data + 0x36) = 1;
        *(unsigned char *)(data + 0x8) = 1;
        unlock_planet(0x10);
        FUN_L00_002502a0(*(int *)(data + 0x284));
        FUN_L01_0027a248(0, 5);
        if (*(int *)(data + 0x154) != -1) {
            char *paths = D_L15_001600EC;
            char *paths_again;
            FUN_L00_00216f90(paths + (*(int *)(data + 0x154) << 7) + 0x30, paths + (*(int *)(data + 0x154) << 7) + 0x70, 0, 1);
            paths_again = D_L15_001600EC;
            FUN_L00_00284e50(paths_again + (*(int *)(data + 0x154) << 7) + 0x30, paths_again + (*(int *)(data + 0x154) << 7) + 0x70);
        }
        moby->state = 5;
        break;
    case ULTRAMECH_SCIENTIST_START_FIRST_SCENE:
        start_scene(1);
        moby->state = 6;
        break;
    case ULTRAMECH_SCIENTIST_WAIT_FOR_FIRST_SCENE:
        if (D_L15_0015F5C4 == 2) {
            break;
        }
        queue_scene_movie_with_game_language(0x10);
        moby->state = 7;
        break;
    case ULTRAMECH_SCIENTIST_START_SECOND_SCENE_AND_SAVE:
        if (D_L15_0015F5C4 != 2) {
            queue_planet_unlock_message(0x10);
            start_scene(2);
            moby->state = 8;
        }
        memcard_save_data(0, -1);
        moby->state = 9;
        break;
    case ULTRAMECH_SCIENTIST_SAVE_AND_DISABLE:
        memcard_save_data(0, -1);
        moby->state = 9;
        break;
    default:
        break;
    }

    {
    char *player;
    float target[4];
    float eye[4];
    float eye_to_target_vector[4];
    float value;
    float pitch;
    float yaw;
    float deg_to_rad;
    float heading;
    float ease_rate;
    float ease_limit;
    /* Fakematch: an always-true flag around the aiming block keeps retail's register allocation. */
    int aim = 0;
    ease_rate = 0.02f;
    ease_limit = 0.3f;
    aim = 1;
    value = vector_distance(&moby->pos, D_0013E533 + 0xE9D);
    if (value < 8.0f) {
        player = D_0013E533 + 0xE1D;
        value = fast_arc_tan(*(float *)(player + 0xD0) - moby->pos.x, *(float *)(player + 0xD4) - moby->pos.y);
        value = fast_difference_between_rotations(moby->rot.z, value);
        if (value < 1.5707964f) {
            value = fast_vec_length(player + 0x100);
            if (0.01f < value) {
                *(int *)(data + 0x274) = scale_game_frames(120);
            } else {
                fast_dec_timer(data + 0x274);
            }
            goto wander;
        }
    }
    if (*(int *)(data + 0x274) != 0) {
        *(int *)(data + 0x274) = 0;
        qcopy(data + 0x260, D_0013E533 + 0xE1D + 0xD0);
    }
wander:
    if (fast_dec_timer(data + 0x278) == 0) {
        goto aim_head;
    }
    deg_to_rad = 0.017453292f;
    value = random_float_between(180.0f, 300.0f);
    value = multiply_global_factor_ed64(value);
    *(int *)(data + 0x278) = truncate_float_to_s32(value);
    value = random_float_between(-90.0f, 90.0f);
    heading = fast_add_rotations(moby->rot.z, value * deg_to_rad);
    value = random_float_between(0.0f, 30.0f);
    build_spherical_offset(data + 0x260, 6.0f, heading, value * deg_to_rad);
    add_vector_xyz(data + 0x260, data + 0x260, &moby->pos);
aim_head:
    if (*(int *)(data + 0x274) != 0) {
        qcopy(target, D_0013E533 + 0xE1D + 0xD0);
        ease_rate = 0.04f;
        ease_limit = 0.3f;
    } else {
        qcopy(target, data + 0x260);
    }
    if (aim != 0) {
    qcopy(eye, &moby->pos);
    eye[2] = eye[2] + 1.0f;
    subtract_vector_xyz(eye_to_target_vector, target, eye);
    value = fast_arc_tan(eye_to_target_vector[0], eye_to_target_vector[1]);
    yaw = fast_subtract_rotations(value, moby->rot.z);
    value = vector_length_xy(eye_to_target_vector);
    value = fast_arc_tan(value, eye_to_target_vector[2]);
    pitch = -value;
    if (yaw > 1.5707964f) {
        yaw = 1.5707964f;
    } else if (yaw < -1.5707964f) {
        yaw = -1.5707964f;
    }
    if (pitch > 0.5235988f) {
        pitch = 0.5235988f;
    } else if (pitch < -0.5235988f) {
        pitch = -0.5235988f;
    }
    *(float *)(data + 0x1C8) = yaw * 0.5f;
    *(float *)(data + 0x1C4) = pitch * 1.25f;
    *(float *)(data + 0x248) = yaw * 0.5f;
    }
    if (big_head_cheat_enabled_macro[0] != 0) {
        *(float *)(data + 0x1D0) = 2.75f;
    }
    update_joint_animation(moby, data + 0x160, 1, ease_rate * frame_scale_sq_gp, ease_limit * frame_scale_sq_gp);
    update_joint_animation(moby, data + 0x1E0, 0, ease_rate * frame_scale_sq_gp, ease_limit * frame_scale_sq_gp);
    }
}

typedef struct Talk_2ed398 {
    int zone[4];
    int npc[3];
    int vol1C;
    int vol20[3];
    int vol2C;
    int pad30;
    int wait;
    int hold38;
    int hold3C;
    int last;
    char pad44[0x54 - 0x44];
    int vol54;
} Talk_2ed398;
typedef struct Help_2ed398 {
    int busy;
    char p0[0x24 - 4];
    int cur;
} Help_2ed398;
typedef struct Stats_2ed398 {
    char p0[0x3DA];
    unsigned short t3DA;
    char p1[0x3E0 - 0x3DC];
    unsigned short n3E0;
    unsigned short t3E2;
    int lv3E4;
} Stats_2ed398;
typedef struct Items_2ed398 {
    char p0[0x6E];
    unsigned char f6E;
    unsigned char f6F;
} Items_2ed398;

extern int scale_game_frames_2ed398(int) __asm__("FUN_001f96f8");
extern int is_point_inside_clip_volume_2ed398(void *, int) __asm__("FUN_00214720");
extern int show_help_message_2ed398(int, int) __asm__("FUN_L00_00203908");
extern int FUN_L00_0020d568_2ed398(void) __asm__("FUN_L00_0020d568");
extern unsigned char D_0013D4DF_2ed398[] __asm__("D_0013D4DF");
extern int D_L15_00161AC0_macro __asm__("D_L15_00161AC0") __attribute__((section(".sdata")));
extern char *moby_list_2ed398 __asm__("D_L15_0015FFD8") __attribute__((section(".sdata")));
extern char D_00141680_2ed398[] __asm__("D_00141680");
extern char D_00141968_2ed398[] __asm__("D_00141968");
extern unsigned char D_0013D388_2ed398[] __asm__("D_0013D388");

extern int D_L15_0015F5A4 __attribute__((section(".sdata")));
extern char D_L15_00179A10[];
extern unsigned char D_0013D4C4[];
extern int D_0015EEA4_macro __asm__("D_0015EEA4") __attribute__((section(".sdata")));
extern s32 current_level_index_macro __asm__("D_0015ED84") __attribute__((section(".sdata")));
/* Quartu moby update (class 1469): hint and dialogue triggers from the hero's zones, with the play-time
   records of the two hint messages. */
void update_quartu_hint_triggers(struct Moby *moby) __asm__("FUN_L15_002ed398");

void update_quartu_hint_triggers(struct Moby *moby) {
    enum {
        HINT_DIALOGUE_INITIALIZE = 0,
        HINT_DIALOGUE_CHECK_TRIGGERS = 1
    };
    Talk_2ed398 *hint_dialogue_data = (struct Talk_2ed398 *)moby->pvars;
    int hint_dialogue_state = moby->state;
    struct Moby *npc_moby;
    int i, j, found, npc, pick;

    switch (hint_dialogue_state) {
    case HINT_DIALOGUE_INITIALIZE:
        moby->update_dist = 0xFF;
        hint_dialogue_data->wait = scale_game_frames_2ed398(0x4B0);
        moby->state = 1;
        break;
    case HINT_DIALOGUE_CHECK_TRIGGERS:
        if (hint_dialogue_data->npc[0] != -1) {
            npc_moby = (struct Moby *)(moby_list_2ed398 + (hint_dialogue_data->npc[0] << 8));
            if (npc_moby != 0 && npc_moby->oclass == 0x4E && npc_moby->state != 0xFE && npc_moby->state != 0xFD && npc_moby->state != 3
                && is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->zone[3]) != 0
                && (hero.state.control_mode < 2u || hero.state.control_mode == 9)
                && ((Help_2ed398 *)D_L15_00179A10)->busy == 0 && ((Help_2ed398 *)D_L15_00179A10)->cur == -1) {
                if (D_0013D4DF_2ed398[0] == 0) {
                    int lim, now;
                    now = scale_game_frames_2ed398(D_0015EEA4_macro);
                    lim = now - ((Stats_2ed398 *)D_00141968_2ed398)->t3DA * 600;
                    if ((int)((float)scale_game_frames_2ed398(0x12) * 60.0f) < lim || ((Stats_2ed398 *)D_00141968_2ed398)->t3DA * 600 == 0) {
                        show_help_message_2ed398(0x3A98, 0x7B);
                    } else {
                        now = scale_game_frames_2ed398(D_0015EEA4_macro);
                        if (now / 600 > ((Stats_2ed398 *)D_00141968_2ed398)->t3DA) {
                            ((Stats_2ed398 *)D_00141968_2ed398)->t3DA = scale_game_frames_2ed398(D_0015EEA4_macro) / 600;
                        }
                    }
                } else if (((Stats_2ed398 *)D_00141968_2ed398)->n3E0 != 0) {
                    int lim, now;
                    now = scale_game_frames_2ed398(D_0015EEA4_macro);
                    lim = now - ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 * 600;
                    if ((int)((float)scale_game_frames_2ed398(0x12) * 60.0f) < lim || ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 * 600 == 0) {
                        show_help_message_2ed398(0x3A99, 0x7C);
                    } else {
                        now = scale_game_frames_2ed398(D_0015EEA4_macro);
                        if (now / 600 > ((Stats_2ed398 *)D_00141968_2ed398)->t3E2) {
                            ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 = scale_game_frames_2ed398(D_0015EEA4_macro) / 600;
                        }
                    }
                } else {
                    int now;
                    ((Stats_2ed398 *)D_00141968_2ed398)->n3E0++;
                    now = scale_game_frames_2ed398(D_0015EEA4_macro);
                    if (now / 600 > ((Stats_2ed398 *)D_00141968_2ed398)->t3E2) {
                        ((Stats_2ed398 *)D_00141968_2ed398)->t3E2 = scale_game_frames_2ed398(D_0015EEA4_macro) / 600;
                    }
                    ((Stats_2ed398 *)D_00141968_2ed398)->lv3E4 = ((Stats_2ed398 *)D_00141968_2ed398)->lv3E4 | (1 << current_level_index_macro) | 0x80000000;
                }
            }
        }
        if (D_L15_0015F5A4 != 0 && ((Help_2ed398 *)D_L15_00179A10)->busy == 0
            && ((Help_2ed398 *)D_L15_00179A10)->cur == -1 && *(unsigned short *)(D_00141680_2ed398 + 0x730) == 0) {
            show_help_message_2ed398(0x3A9F, 0x89);
            D_L15_0015F5A4 = 0;
        }
        if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->vol54) != 0
            && hero.vehicle_mode == 3 && hero.glow_pulse_level == 0
            && D_L15_00161AC0_macro == 0 && *(unsigned short *)(D_00141680_2ed398 + 0x728) == 0) {
            show_help_message_2ed398(0x3A9E, 0x88);
        }
        pick = -1;
        npc = -1;
        found = 0;
        for (i = 0; i < 1; i++) {
            if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->zone[i]) != 0) {
                pick = hint_dialogue_data->zone[i];
                npc = hint_dialogue_data->npc[i];
                found = 1;
                break;
            }
        }
        if (found != 0) {
            hint_dialogue_data->hold3C = hint_dialogue_data->hold3C + 1;
        } else {
            hint_dialogue_data->hold3C = 0;
            hint_dialogue_data->last = -1;
        }
        if (npc != -1) {
            npc_moby = (struct Moby *)(moby_list_2ed398 + (npc << 8));
            if (npc_moby != 0 && npc_moby->oclass == 0x4E && npc_moby->state != 0xFE && npc_moby->state != 0xFD && npc_moby->state != 3
                && hint_dialogue_data->hold3C > hint_dialogue_data->wait && pick != -1 && pick != hint_dialogue_data->last
                && (hero.state.control_mode < 2u || hero.state.control_mode == 9)
                && ((Help_2ed398 *)D_L15_00179A10)->busy == 0 && ((Help_2ed398 *)D_L15_00179A10)->cur == -1
                && D_0013D4DF_2ed398[0] != 0) {
                hint_dialogue_data->wait = scale_game_frames_2ed398(0x708);
                if (*(int *)(D_00141680_2ed398 + 0x6D4) >= 0) {
                    show_help_message_2ed398(0x3A9A, 0x7D);
                }
                hint_dialogue_data->last = pick;
                hint_dialogue_data->hold3C = 0;
            }
        }
        if (hero.moby->seq == 0xC && hero.vehicle_mode == 2) {
            ((Items_2ed398 *)D_0013D388_2ed398)->f6F = 1;
        }
        if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->vol1C) != 0 && hero.vehicle_mode == 2
            && ((Items_2ed398 *)D_0013D388_2ed398)->f6F == 0 && ((Help_2ed398 *)D_L15_00179A10)->busy == 0
            && ((Help_2ed398 *)D_L15_00179A10)->cur == -1 && *(int *)(D_00141680_2ed398 + 0x6EC) >= 0) {
            show_help_message_2ed398(0x3A9C, 0x80);
        }
        if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->vol20[0]) != 0 && ((Help_2ed398 *)D_L15_00179A10)->busy == 0
            && ((Help_2ed398 *)D_L15_00179A10)->cur == -1 && D_0013D4C4[0] == 0
            && !(*(unsigned int *)(D_00141680_2ed398 + 0x6FC) & (1 << current_level_index_macro))) {
            show_help_message_2ed398(0x3A9D, 0x82);
        }
        pick = -1;
        found = 0;
        for (j = 0; j < 3; j++) {
            if (is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->vol20[j]) != 0) {
                found = 1;
                if (hint_dialogue_data->last != hint_dialogue_data->vol20[j]) {
                    pick = hint_dialogue_data->vol20[j];
                }
                break;
            }
        }
        if (found == 0) {
            hint_dialogue_data->hold38 = 0;
            hint_dialogue_data->last = -1;
        } else {
            hint_dialogue_data->hold38 = hint_dialogue_data->hold38 + 1;
            if (hero.state.current == 0x35) {
                hint_dialogue_data->hold38 = 0;
                hint_dialogue_data->last = -1;
            }
        }
        if (scale_game_frames_2ed398(0xE10) < hint_dialogue_data->hold38 && pick != -1 && FUN_L00_0020d568_2ed398() != 0
            && ((Help_2ed398 *)D_L15_00179A10)->busy == 0 && ((Help_2ed398 *)D_L15_00179A10)->cur == -1
            && D_0013D4C4[0] != 0) {
            show_help_message_2ed398(0x4E2E, 0x78);
            hint_dialogue_data->last = pick;
            hint_dialogue_data->hold38 = 0;
        }
        if (((Items_2ed398 *)D_0013D388_2ed398)->f6E == 0 && is_point_inside_clip_volume_2ed398(&hero.motion.pos, hint_dialogue_data->vol2C) != 0) {
            ((Items_2ed398 *)D_0013D388_2ed398)->f6E = 1;
        }
        break;
    }
}
/* Waits for state 1, then passes the slot for the level's current mode to cutscene_set_camera_target. */
struct L15MissionState {
    char pad0[0x30];
    int mode;
    char pad1[0x178 - 0x34];
    int slot[5];
};

extern int D_L15_0015F5C4;
extern struct L15MissionState mission_2edb20 __asm__("D_L15_0016CE60");
extern void cutscene_set_camera_target(int) __asm__("FUN_L00_002637f8");

void emit_cutscene_mode_moby_effects(struct Moby *moby) __asm__("FUN_L15_002edb20");

void emit_cutscene_mode_moby_effects(struct Moby *moby) {
    enum {
        INITIALIZE_UPDATE_DISTANCE = 0,
        DISPATCH_MODE_SELECTED_SLOT = 1
    };
    switch (moby->state) {
    case INITIALIZE_UPDATE_DISTANCE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case DISPATCH_MODE_SELECTED_SLOT:
        if (D_L15_0015F5C4 == 2) {
            if (mission_2edb20.mode == 1 || mission_2edb20.mode == 2 || mission_2edb20.mode == 3 ||
                mission_2edb20.mode == 4 || mission_2edb20.mode == 6) {
                int mission_mode = mission_2edb20.mode;
                int i;
                i = 0;
                if (mission_mode == 1)
                    i = 4;
                else if (mission_mode == 2 || mission_mode == 3 || mission_mode == 4)
                    i = 2;
                else if (mission_mode == 6)
                    i = 1;
                cutscene_set_camera_target(mission_2edb20.slot[i]);
            }
        }
        break;
    }
}
extern float FUN_001f9988_2edbe0(float) __asm__("FUN_001f9988");
extern float FUN_001fa6c0_2edbe0(int) __asm__("FUN_001fa6c0");
extern int FUN_001f44b8_2edbe0(int) __asm__("FUN_001f44b8");
extern void FUN_00233980_2edbe0(int, long) __asm__("FUN_00233980");
extern float FUN_001f96b0_2edbe0(float) __asm__("FUN_001f96b0");
extern float FUN_001fa580_2edbe0(float, float) __asm__("FUN_001fa580");
extern float FUN_001f9de0_2edbe0(float) __asm__("FUN_001f9de0");
extern void FUN_L00_00250320_2edbe0(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
extern float FUN_L00_00200260_2edbe0(float, float) __asm__("FUN_L00_00200260");
extern float FUN_001fa610_2edbe0(float x) __asm__("FUN_001fa610");
extern void FUN_001f76a0_2edbe0(void) __asm__("FUN_001f76a0");
extern void FUN_L00_001fde98_2edbe0(int, int, int, void *, int) __asm__("FUN_L00_001fde98");
extern float D_0015ED6C_2edbe0 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") __attribute__((section(".sdata")));
typedef struct { float f[2][2]; } Quad4_2edbe0;
typedef struct {
    float prev;
    float angle;
} RingVars_2edbe0;

float D_L15_001DECB0[46][3] = {0};
float D_L15_001DE860[46][3] = {
    {11.779387f, 28.437962f, 0.0f},
    {21.76547f, 21.76547f, 0.0f},
    {15.0f, 15.0f, 0.0f},
    {28.437956f, 11.779387f, 0.0f},
    {15.0f, 0.0f, 0.0f},
    {3e+01f, 0.0f, 0.0f},
    {15.0f, -15.0f, 0.0f},
    {28.437956f, -11.779387f, 0.0f},
    {15.0f, -15.0f, 0.0f},
    {21.76547f, -21.76547f, 0.0f},
    {15.0f, -15.0f, 0.0f},
    {11.779387f, -28.437956f, 0.0f},
    {0.0f, -15.0f, 0.0f},
    {0.0f, -3e+01f, 0.0f},
    {-15.0f, -15.0f, 0.0f},
    {-11.779387f, -28.437962f, 0.0f},
    {-15.0f, -15.0f, 0.0f},
    {-21.76547f, -21.76547f, 0.0f},
    {-15.0f, -15.0f, 0.0f},
    {-28.437956f, -11.779387f, 0.0f},
    {-15.0f, 0.0f, 0.0f},
    {-3e+01f, 0.0f, 0.0f},
    {-15.0f, 15.0f, 0.0f},
    {-28.437962f, 11.779387f, 0.0f},
    {-15.0f, 15.0f, 0.0f},
    {-21.765469f, 21.765469f, 0.0f},
    {-15.0f, 15.0f, 0.0f},
    {-11.779387f, 28.437962f, 0.0f},
    {0.0f, 15.0f, 0.0f},
    {0.0f, 3e+01f, 0.0f},
    {0.0f, 15.0f, 0.0f},
    {11.779387f, 28.437962f, 0.0f},
    {0.0f, 15.0f, 0.0f},
    {15.0f, 15.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {15.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {15.0f, -15.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, -15.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {-15.0f, -15.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {-15.0f, 0.0f, 0.0f},
    {0.0f, 15.0f, 0.0f},
    {-15.0f, 15.0f, 0.0f},
};
int D_L15_001DF048[46] = {0};
float D_L15_001DEA88[46][2] = {
    {2.949836f, -0.514755f},
    {2.375023f, -1.375023f},
    {1.7922f, -0.7922f},
    {1.514755f, -1.949835f},
    {0.5f, -0.7922f},
    {0.5f, -2.0844f},
    {-0.7922f, -0.7922f},
    {-0.514755f, -1.949835f},
    {-0.7922f, -0.7922f},
    {-1.375023f, -1.375023f},
    {-0.7922f, -0.7922f},
    {-1.949835f, -0.514755f},
    {-0.7922f, 0.5f},
    {-2.0844f, 0.5f},
    {-0.7922f, 1.7922f},
    {-1.949835f, 1.514755f},
    {-0.7922f, 1.7922f},
    {-1.375023f, 2.375022f},
    {-0.7922f, 1.7922f},
    {-0.514755f, 2.949835f},
    {0.5f, 1.7922f},
    {0.5f, 3.0844f},
    {1.7922f, 1.7922f},
    {1.514755f, 2.949835f},
    {1.7922f, 1.7922f},
    {2.375023f, 2.375022f},
    {1.7922f, 1.7922f},
    {2.949835f, 1.514755f},
    {1.7922f, 0.5f},
    {3.0844f, 0.5f},
    {1.7922f, 0.5f},
    {2.949836f, -0.514755f},
    {1.7922f, 0.5f},
    {1.7922f, -0.7922f},
    {0.5f, 0.5f},
    {0.5f, -0.7922f},
    {0.5f, 0.5f},
    {-0.7922f, -0.7922f},
    {0.5f, 0.5f},
    {-0.7922f, 0.5f},
    {0.5f, 0.5f},
    {-0.7922f, 1.7922f},
    {0.5f, 0.5f},
    {0.5f, 1.7922f},
    {1.7922f, 0.5f},
    {1.7922f, 1.7922f},
};
float D_L15_001DEED8[46][2] = {0};
extern Quad4_2edbe0 D_L15_00162208;
int D_L15_001621B0[1] = {0x2e};
int D_L15_001621D0[1] = {(int)D_L15_001DECB0};
int D_L15_001621D8[1] = {(int)D_L15_001DEED8};
int D_L15_001621E0[1] = {(int)D_L15_001DF048};
int D_L15_001621E4 = 128;
int D_L15_001621E8 = 45;
int D_L15_001621EC = 64;
extern float scaled_ripple_radial_wavelength_gp __asm__("D_L15_001621F0") __attribute__((sda));
#include "rnc/overlay/l15/gameplay/entities/conveyor_belt_data/draw_scaled_moby_radial_ripple_mesh.h"
extern float scaled_ripple_z_amplitude_gp __asm__("D_L15_001621F8") __attribute__((sda));
extern float scaled_ripple_uv_scroll_rate_gp __asm__("D_L15_001621FC") __attribute__((sda));
extern float scaled_ripple_template_vertex_scale_gp __asm__("D_L15_00162200") __attribute__((sda));
/* Builds and draws the animated ring of lights around a moby: 46 scaled vertices and colours, then two passes of 46 texture coordinates. */
void draw_scaled_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002edbe0");

void draw_scaled_moby_radial_ripple_mesh(struct Moby *moby) {
    RingVars_2edbe0 *data = (RingVars_2edbe0 *)moby->pvars;
    unsigned int i;
    unsigned int uv_vertex_index;
    int j;
    int k;
    int r;
    int g;
    int b;
    int color;
    float scale;
    scale = FUN_001fa6c0_2edbe0(D_L15_0015F5CC_macro) * (scaled_ripple_uv_scroll_rate_gp * D_0015ED6C_2edbe0);
    FUN_00233980_2edbe0(6, FUN_001f44b8_2edbe0(D_L15_001621E8));
    FUN_00233980_2edbe0(0x42, ((long)D_L15_001621E4 << 32) | 0x44);
    FUN_00233980_2edbe0(8, 0);
    FUN_00233980_2edbe0(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = FUN_001fa580_2edbe0(data->angle, 360.0f / FUN_001f96b0_2edbe0(scaled_ripple_phase_advance_divisor_gp) * 0.017453292f * D_0015ED6C_2edbe0);
    FUN_L00_00250320_2edbe0(moby, &r, &g, &b);
    color = (D_L15_001621EC << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 46; i++) {
        float *template_vertex = D_L15_001DE860[i];
        float x = template_vertex[0];
        float y = template_vertex[1];
        float rad = FUN_001f9988_2edbe0(x * x + y * y);
        float radius_remainder = FUN_L00_00200260_2edbe0(rad, scaled_ripple_radial_wavelength_gp);
        float w = FUN_001fa610_2edbe0(radius_remainder * 6.2831855f / scaled_ripple_radial_wavelength_gp);
        float z = FUN_001f9de0_2edbe0(FUN_001fa580_2edbe0(w, data->angle));
        D_L15_001DECB0[i][0] = scaled_ripple_template_vertex_scale_gp * template_vertex[0] + moby->pos.x;
        D_L15_001DECB0[i][1] = scaled_ripple_template_vertex_scale_gp * template_vertex[1] + moby->pos.y;
        D_L15_001DECB0[i][2] = scaled_ripple_template_vertex_scale_gp * template_vertex[2] + moby->pos.z + scaled_ripple_z_amplitude_gp * z;
        D_L15_001DF048[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 46; uv_vertex_index++) {
            Quad4_2edbe0 uv_scroll_factors = D_L15_00162208;
            D_L15_001DEED8[uv_vertex_index][0] = D_L15_001DEA88[uv_vertex_index][0] + FUN_L00_00200260_2edbe0(scale * uv_scroll_factors.f[j][0], 1.0f);
            D_L15_001DEED8[uv_vertex_index][1] = D_L15_001DEA88[uv_vertex_index][1] + FUN_L00_00200260_2edbe0(scale * uv_scroll_factors.f[j][1], 1.0f);
        }
        FUN_001f76a0_2edbe0();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edbe0(D_L15_001621B0[k], D_L15_001621D0[k], D_L15_001621E0[k], (void *)D_L15_001621D8[k], 1);
        }
    }
}
extern float D_0015ED6C_2edfc0 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern int D_L15_0015F5CC_macro __asm__("D_L15_0015F5CC") __attribute__((section(".sdata")));
extern int FUN_001f44b8_2edfc0(int) __asm__("FUN_001f44b8");
extern void FUN_001f76a0_2edfc0(void) __asm__("FUN_001f76a0");
extern float FUN_001f96b0_2edfc0(float) __asm__("FUN_001f96b0");
extern float FUN_001f9988_2edfc0(float) __asm__("FUN_001f9988");
extern float FUN_001f9de0_2edfc0(float) __asm__("FUN_001f9de0");
extern float FUN_001fa580_2edfc0(float, float) __asm__("FUN_001fa580");
extern float FUN_001fa610_2edfc0(float x) __asm__("FUN_001fa610");
extern float FUN_001fa6c0_2edfc0(int) __asm__("FUN_001fa6c0");
extern void FUN_00233980_2edfc0(int, long) __asm__("FUN_00233980");
extern void FUN_L00_001fde98_2edfc0(int, int, int, void *, int) __asm__("FUN_L00_001fde98");
extern float FUN_L00_00200260_2edfc0(float, float) __asm__("FUN_L00_00200260");
extern void FUN_L00_00250320_2edfc0(void *, void *, void *, void *) __asm__("FUN_L00_00250320");
typedef struct { float f[2][2]; } Quad4_2edfc0;
typedef struct {
    float prev;
    float angle;
} RingVars_2edfc0;

float D_L15_001DF940[88][3] = {0};
float D_L15_001DF100[88][3] = {
    {0.0f, -4.4e-05f, -8.4e-05f},
    {-0.545705f, -0.418775f, -7.9e-05f},
    {-1.422783f, 1.854165f, -0.000104f},
    {-1.968488f, 1.435435f, -9.9e-05f},
    {-2.845565f, 3.708374f, -0.000121f},
    {-3.391273f, 3.289643f, -0.000116f},
    {-4.268348f, 5.562582f, -0.000138f},
    {-4.814055f, 5.143852f, -0.000135f},
    {-5.691131f, 7.416792f, -0.000157f},
    {-4.814055f, 5.143852f, -0.000135f},
    {-6.23684f, 6.998059f, -0.000155f},
    {-5.359763f, 4.725121f, -0.000133f},
    {-6.782547f, 6.579327f, -0.000152f},
    {-5.90547f, 4.30639f, -0.000128f},
    {-7.328256f, 6.160594f, -0.00015f},
    {-6.451175f, 3.887654f, -0.000128f},
    {-7.873965f, 5.741859f, -0.000147f},
    {-6.954219f, 3.501659f, -0.000125f},
    {-8.377006f, 5.355859f, -0.000147f},
    {-7.457261f, 3.115659f, -0.000123f},
    {-8.88005f, 4.969857f, -0.000145f},
    {-7.960301f, 2.729661f, -0.000121f},
    {-9.383094f, 4.583856f, -0.000143f},
    {-7.960301f, 2.729661f, -0.000121f},
    {-9.886135f, 4.197855f, -0.000143f},
    {-7.960301f, 2.729661f, -0.000121f},
    {-8.463342f, 2.343659f, -0.000118f},
    {-6.537511f, 0.875461f, -9.9e-05f},
    {-7.040548f, 0.489462f, -9.6e-05f},
    {-5.114717f, -0.978736f, -7.7e-05f},
    {-5.617753f, -1.364733f, -7.4e-05f},
    {-5.114717f, -0.978736f, -7.7e-05f},
    {-4.194959f, -3.218929f, -5e-05f},
    {-5.114717f, -0.978736f, -7.7e-05f},
    {-3.691922f, -2.832938f, -5.5e-05f},
    {-4.611681f, -0.592744f, -7.9e-05f},
    {-3.188887f, -2.446948f, -5.7e-05f},
    {-4.108642f, -0.20675f, -8.2e-05f},
    {-2.685849f, -2.060957f, -6e-05f},
    {-3.605601f, 0.17924f, -8.4e-05f},
    {-2.182811f, -1.674966f, -6.4e-05f},
    {-3.059897f, 0.597974f, -9.1e-05f},
    {-1.637109f, -1.256235f, -6.9e-05f},
    {-2.514193f, 1.016704f, -9.4e-05f},
    {-1.091407f, -0.837505f, -7.4e-05f},
    {-2.514193f, 1.016704f, -9.4e-05f},
    {-0.545705f, -0.418775f, -7.9e-05f},
    {-2.514193f, 1.016704f, -9.4e-05f},
    {-1.968488f, 1.435435f, -9.9e-05f},
    {-3.936981f, 2.870914f, -0.000111f},
    {-3.391273f, 3.289643f, -0.000116f},
    {-3.936981f, 2.870914f, -0.000111f},
    {-4.814055f, 5.143852f, -0.000135f},
    {-3.936981f, 2.870914f, -0.000111f},
    {-5.359763f, 4.725121f, -0.000133f},
    {-3.936981f, 2.870914f, -0.000111f},
    {-5.90547f, 4.30639f, -0.000128f},
    {-3.936981f, 2.870914f, -0.000111f},
    {-4.482684f, 2.45218f, -0.000108f},
    {-2.514193f, 1.016704f, -9.4e-05f},
    {-4.482684f, 2.45218f, -0.000108f},
    {-3.059897f, 0.597974f, -9.1e-05f},
    {-4.482684f, 2.45218f, -0.000108f},
    {-3.605601f, 0.17924f, -8.4e-05f},
    {-4.482684f, 2.45218f, -0.000108f},
    {-5.028388f, 2.033447f, -0.000106f},
    {-5.90547f, 4.30639f, -0.000128f},
    {-5.028388f, 2.033447f, -0.000106f},
    {-6.451175f, 3.887654f, -0.000128f},
    {-5.028388f, 2.033447f, -0.000106f},
    {-6.954219f, 3.501659f, -0.000125f},
    {-5.028388f, 2.033447f, -0.000106f},
    {-5.53143f, 1.647454f, -0.000104f},
    {-3.605601f, 0.17924f, -8.4e-05f},
    {-5.53143f, 1.647454f, -0.000104f},
    {-4.108642f, -0.20675f, -8.2e-05f},
    {-6.034472f, 1.261461f, -0.000104f},
    {-4.611681f, -0.592744f, -7.9e-05f},
    {-6.034472f, 1.261461f, -0.000104f},
    {-5.114717f, -0.978736f, -7.7e-05f},
    {-6.034472f, 1.261461f, -0.000104f},
    {-6.537511f, 0.875461f, -9.9e-05f},
    {-6.034472f, 1.261461f, -0.000104f},
    {-7.960301f, 2.729661f, -0.000121f},
    {-6.034472f, 1.261461f, -0.000104f},
    {-7.457261f, 3.115659f, -0.000123f},
    {-5.53143f, 1.647454f, -0.000104f},
    {-6.954219f, 3.501659f, -0.000125f},
};
int D_L15_001E0020[88] = {0};
float D_L15_001DF520[88][2] = {
    {1.549487f, 2.971533f},
    {1.758852f, 2.698682f},
    {0.622383f, 2.260142f},
    {0.831748f, 1.987289f},
    {-0.304721f, 1.54875f},
    {-0.095357f, 1.275897f},
    {-1.231826f, 0.837359f},
    {-1.022461f, 0.564505f},
    {-2.15893f, 0.125968f},
    {-1.022461f, 0.564505f},
    {-1.949564f, -0.146887f},
    {-0.813094f, 0.291651f},
    {-1.740197f, -0.419741f},
    {-0.603729f, 0.018797f},
    {-1.530831f, -0.692595f},
    {-0.394361f, -0.254055f},
    {-1.321465f, -0.965449f},
    {-0.201364f, -0.505576f},
    {-1.128464f, -1.21697f},
    {-0.008365f, -0.757097f},
    {-0.935464f, -1.468491f},
    {0.184635f, -1.008617f},
    {-0.742463f, -1.720012f},
    {0.184635f, -1.008617f},
    {-0.549463f, -1.971533f},
    {0.184635f, -1.008617f},
    {0.377635f, -1.260136f},
    {1.111734f, -0.297221f},
    {1.304734f, -0.54874f},
    {2.038833f, 0.414175f},
    {2.231832f, 0.162657f},
    {2.038833f, 0.414175f},
    {3.15893f, 0.874054f},
    {2.038833f, 0.414175f},
    {2.965935f, 1.125572f},
    {1.845837f, 0.665694f},
    {2.772939f, 1.377091f},
    {1.65284f, 0.917213f},
    {2.579944f, 1.628609f},
    {1.459845f, 1.168733f},
    {2.386948f, 1.880127f},
    {1.250479f, 1.441585f},
    {2.177583f, 2.152979f},
    {1.041113f, 1.714437f},
    {1.968218f, 2.42583f},
    {1.041113f, 1.714437f},
    {1.758852f, 2.698682f},
    {1.041113f, 1.714437f},
    {0.831748f, 1.987289f},
    {0.114008f, 1.003043f},
    {-0.095357f, 1.275897f},
    {0.114008f, 1.003043f},
    {-1.022461f, 0.564505f},
    {0.114008f, 1.003043f},
    {-0.813094f, 0.291651f},
    {0.114008f, 1.003043f},
    {-0.603729f, 0.018797f},
    {0.114008f, 1.003043f},
    {0.323375f, 0.730191f},
    {1.041113f, 1.714437f},
    {0.323375f, 0.730191f},
    {1.250479f, 1.441585f},
    {0.323375f, 0.730191f},
    {1.459845f, 1.168733f},
    {0.323375f, 0.730191f},
    {0.532742f, 0.457339f},
    {-0.603729f, 0.018797f},
    {0.532742f, 0.457339f},
    {-0.394361f, -0.254055f},
    {0.532742f, 0.457339f},
    {-0.201364f, -0.505576f},
    {0.532742f, 0.457339f},
    {0.725738f, 0.205818f},
    {1.459845f, 1.168733f},
    {0.725738f, 0.205818f},
    {1.65284f, 0.917213f},
    {0.918734f, -0.045703f},
    {1.845837f, 0.665694f},
    {0.918734f, -0.045703f},
    {2.038833f, 0.414175f},
    {0.918734f, -0.045703f},
    {1.111734f, -0.297221f},
    {0.918734f, -0.045703f},
    {0.184635f, -1.008617f},
    {0.918734f, -0.045703f},
    {-0.008365f, -0.757097f},
    {0.725738f, 0.205818f},
    {-0.201364f, -0.505576f},
};
float D_L15_001DFD60[88][2] = {0};
extern Quad4_2edfc0 D_L15_00162278;
int D_L15_00162220[1] = {0x58};
int D_L15_00162240[1] = {(int)D_L15_001DF940};
int D_L15_00162248[1] = {(int)D_L15_001DFD60};
int D_L15_00162250[1] = {(int)D_L15_001E0020};
int D_L15_00162254 = 128;
int D_L15_00162258 = 45;
int D_L15_0016225C = 64;
extern float ripple_88_radial_wavelength_gp __asm__("D_L15_00162260") __attribute__((sda));
#include "rnc/overlay/l15/gameplay/entities/conveyor_belt_data/draw_scaled_88_vertex_moby_radial_ripple_mesh.h"
extern float ripple_88_z_amplitude_gp __asm__("D_L15_00162268") __attribute__((sda));
extern float ripple_88_uv_scroll_rate_gp __asm__("D_L15_0016226C") __attribute__((sda));
extern float ripple_88_template_vertex_scale_gp __asm__("D_L15_00162270") __attribute__((sda));
/* Builds and draws the animated ring of lights around a moby: 88 scaled vertices and colours, then two passes of 88 texture coordinates. */
void draw_scaled_88_vertex_moby_radial_ripple_mesh(struct Moby *moby) __asm__("FUN_L15_002edfc0");

void draw_scaled_88_vertex_moby_radial_ripple_mesh(struct Moby *moby) {
    RingVars_2edfc0 *data = (RingVars_2edfc0 *)moby->pvars;
    unsigned int i;
    unsigned int uv_vertex_index;
    int j;
    int k;
    int r;
    int g;
    int b;
    int color;
    float scale;
    scale = FUN_001fa6c0_2edfc0(D_L15_0015F5CC_macro) * (ripple_88_uv_scroll_rate_gp * D_0015ED6C_2edfc0);
    FUN_00233980_2edfc0(6, FUN_001f44b8_2edfc0(D_L15_00162258));
    FUN_00233980_2edfc0(0x42, ((long)D_L15_00162254 << 32) | 0x44);
    FUN_00233980_2edfc0(8, 0);
    FUN_00233980_2edfc0(0x14, 0xFF9000000260L);
    data->prev = data->angle;
    data->angle = FUN_001fa580_2edfc0(data->angle, 360.0f / FUN_001f96b0_2edfc0(ripple_88_phase_advance_divisor_gp) * 0.017453292f * D_0015ED6C_2edfc0);
    FUN_L00_00250320_2edfc0(moby, &r, &g, &b);
    color = (D_L15_0016225C << 24) | (b << 16) | (g << 8) | r;
    for (i = 0; i < 88; i++) {
        float *template_vertex = D_L15_001DF100[i];
        float x = template_vertex[0];
        float y = template_vertex[1];
        float rad = FUN_001f9988_2edfc0(x * x + y * y);
        float radius_remainder = FUN_L00_00200260_2edfc0(rad, ripple_88_radial_wavelength_gp);
        float w = FUN_001fa610_2edfc0(radius_remainder * 6.2831855f / ripple_88_radial_wavelength_gp);
        float z = FUN_001f9de0_2edfc0(FUN_001fa580_2edfc0(w, data->angle));
        D_L15_001DF940[i][0] = ripple_88_template_vertex_scale_gp * template_vertex[0] + moby->pos.x;
        D_L15_001DF940[i][1] = ripple_88_template_vertex_scale_gp * template_vertex[1] + moby->pos.y;
        D_L15_001DF940[i][2] = ripple_88_template_vertex_scale_gp * template_vertex[2] + moby->pos.z + ripple_88_z_amplitude_gp * z;
        D_L15_001E0020[i] = color;
    }
    for (j = 0; j < 2; j++) {
        for (uv_vertex_index = 0; uv_vertex_index < 88; uv_vertex_index++) {
            Quad4_2edfc0 uv_scroll_factors = D_L15_00162278;
            D_L15_001DFD60[uv_vertex_index][0] = D_L15_001DF520[uv_vertex_index][0] + FUN_L00_00200260_2edfc0(scale * uv_scroll_factors.f[j][0], 1.0f);
            D_L15_001DFD60[uv_vertex_index][1] = D_L15_001DF520[uv_vertex_index][1] + FUN_L00_00200260_2edfc0(scale * uv_scroll_factors.f[j][1], 1.0f);
        }
        FUN_001f76a0_2edfc0();
        for (k = 0; k < 1; k++) {
            FUN_L00_001fde98_2edfc0(D_L15_00162220[k], D_L15_00162240[k], D_L15_00162250[k], (void *)D_L15_00162248[k], 1);
        }
    }
}
/* Tells whether the vendor menu lets this slot's item be bought (1) or not (0). */

extern char *D_L15_00167400_lui __asm__("D_L15_00167400") __attribute__((section(".data")));
extern char *D_L15_0015EF50;

int can_spawn_codebot_slot_effect(int slot) {
    char *slot_state_data;
    if (*(short *)(D_L15_00167400_lui + 0x86) != 0x13)
        return 1;
    slot_state_data = *(char **)(*(char **)(*(char **)(D_L15_0015EF50 + slot * 32 + 0x1C) + 4) + 0x70) + 0x40;
    if (*(short *)(slot_state_data + 0x14) == 1 && codebot_slot_effect_spawn_gate != 0.0f)
        return 1;
    return *(short *)(slot_state_data + 0x14) == 3;
}
/* tests several indexed conditions of a moby's entry in the table */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_002EDB50.c: func_L15_002F99F8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int *D_L15_001B0AB0[];
extern int point_in_path_xy(float *p, float *v, int n) __asm__("FUN_L00_00259740");
extern int test_point_in_transformed_unit_cylinder(void *p, int i) __asm__("FUN_L00_0025fa38");
extern int test_point_in_transformed_unit_sphere(float *p, int idx) __asm__("FUN_L00_0025faf0");

extern char *D_L15_0015EF48[] __attribute__((section(".sdata")));
int is_hero_in_moby_follow_camera_region(char *moby) __asm__("FUN_L15_002f85a8");

int is_hero_in_moby_follow_camera_region(char *moby) {
    int *slot_data = *(int **)(D_L15_0015EF48[2] + *(short *)(moby + 0x84) * 32 + 0x1C);
    int r;
    int i;
    if (slot_data[10] >= 0 && test_point_in_transformed_unit_cylinder(D_0013E533 + 0xE9D, slot_data[10])) {
        return 0;
    }
    if (slot_data[3] >= 0) {
        if (is_point_inside_clip_volume(D_0013E533 + 0xE9D, slot_data[3])) {
            return 1;
        }
    } else if (slot_data[4] >= 0) {
        if (test_point_in_transformed_unit_cylinder(D_0013E533 + 0xE9D, slot_data[4])) {
            return 1;
        }
    } else if (slot_data[2] >= 0) {
        if (test_point_in_transformed_unit_sphere((float *)(D_0013E533 + 0xE9D), slot_data[2])) {
            return 1;
        }
    } else if (slot_data[5] >= 0) {
        i = slot_data[5];
        if (point_in_path_xy((float *)(D_0013E533 + 0xE9D), (float *)(D_L15_001B0AB0[i] + 4),
                             D_L15_001B0AB0[i][0])) {
            return 1;
        }
    }
    return 0;
}
/* tests whether the target lies within a cone of the player's axis */
/* Ported from rac1-decomp (src/overlays/l15_quartu/vendor_002EDB50.c: func_L15_002F9AE8), where it is exact; names translated to the US level program. */

extern char *D_L15_001600EC;
extern char D_L15_00167410[];
extern char D_L15_00167280[];
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern float fast_arc_sin(float) __asm__("FUN_001f9df8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f99c0(float);

int check_moby_target_angular_limits(char *moby, float projected_angle_limit_degrees, float b) __asm__("FUN_L15_002f8698");

int check_moby_target_angular_limits(char *moby, float projected_angle_limit_degrees, float b) {
    float target_displacement[4];
    float scaled_axis_vector[4];
    float target_axis_residual[4];
    float r[4];
    char *data;
    char *projection_axis_record = D_L15_00167410;
    char *g;
    float dot1;
    float dot2;
    float lenq;
    float lenr;
    float dot3;
    float den;
    char *moby_slot_table = D_L15_0015EF48[2];
    char *target_record;

    data = *(char **)(moby_slot_table + *(short *)(moby + 0x84) * 32 + 0x1C);
    target_record = D_L15_001600EC + *(int *)(data + 0x48) * 128;
    if (projected_angle_limit_degrees == 0.0f && b == 0.0f) {
        return 1;
    }
    g = D_L15_00167280;
    fast_vec_sub(target_displacement, target_record + 0x30, *(char **)(g + 0x180) + 0x30);
    dot1 = fast_vec_dot(target_displacement, (projection_axis_record + 0x30));
    fast_vec_scale(scaled_axis_vector, (projection_axis_record + 0x30), dot1);
    fast_vec_sub(target_axis_residual, target_displacement, scaled_axis_vector);
    lenq = fast_vec_length(target_axis_residual);
    dot2 = fast_vec_dot(*(char **)(g + 0x180), (projection_axis_record + 0x30));
    fast_vec_scale(scaled_axis_vector, (projection_axis_record + 0x30), dot2);
    fast_vec_sub(r, *(char **)(g + 0x180), scaled_axis_vector);
    lenr = fast_vec_length(r);
    if (b != 0.0f) {
        float target_axis_angle = fast_arc_tan(lenq, dot1);
        float reference_axis_angle = fast_arc_tan(lenr, dot2);
        float axis_angle_difference = fast_difference_between_rotations(target_axis_angle, reference_axis_angle);
        if (b * DEG_TO_RAD < axis_angle_difference) {
            return 0;
        }
    }
    if (projected_angle_limit_degrees == 0.0f) {
        return 1;
    }
    dot3 = fast_vec_dot(target_axis_residual, r);
    den = lenr * lenq;
    if (den == 0.0f) {
        return 0;
    }
    if (FUN_001f99c0(1.5707964f - fast_arc_sin(dot3 / den)) < projected_angle_limit_degrees * DEG_TO_RAD) {
        return 1;
    }
    return 0;
}
extern char *D_L15_00167400_lui __asm__("D_L15_00167400") __attribute__((section(".data")));
typedef struct { char pad[0x100]; float x; float y; } G_88e8;
extern G_88e8 D_0013C940_s __asm__("D_0013C940") __attribute__((section(".data")));
extern int try_select_follow_camera_volume(char *) __asm__("FUN_L00_002e83c0");
extern void clear_follow_camera_volume_if_current(char *) __asm__("FUN_L00_002e8388");
extern int frames_s(int) __asm__("FUN_001f96f8");
extern void set_nonzero_follow_camera_parameters(float, float, float) __asm__("FUN_L02_002f6598");
extern float itof_s(int) __asm__("FUN_001fa6c0");
extern void set_follow_camera_angle_blend_from_position(void *, float, float) __asm__("FUN_L00_002e8970");
extern void pitch_s(int, float, float) __asm__("FUN_L00_002e8450");
extern void FUN_L00_002e8620(void);
extern void yaw_s(float, float) __asm__("FUN_L00_002e84b8");
extern void zoom_s(int, float, float) __asm__("FUN_L00_002e84f0");

/* Runs the aim timer while the player holds the target in view; spins the target once aimed. */
void update_moby_follow_camera(char *moby) __asm__("FUN_L15_002f88e8");

void update_moby_follow_camera(char *moby) {
    char *slot_data = *(char **)(D_L15_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1C);
    char *target_record = D_L15_001600EC + (*(int *)(slot_data + 0x48) << 7);
    int done;
    int hold_start_frames;
    int cone_check_delay_frames;
    int cycle_reset_frames;
    int spin_ramp_frames;
    float spin_ramp_ratio;
    float spin_angle_radians;
    if (try_select_follow_camera_volume(moby) <= 0) {
        *(short *)(slot_data + 0x20) = 0;
        return;
    }
    if (is_hero_in_moby_follow_camera_region(moby) == 0) {
        *(short *)(slot_data + 0x20) = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    *(int *)(*(char **)(D_L15_00167400_lui + 0x70) + 0x230) = 2;
    done = 1;
    (*(short *)(slot_data + 0x20))++;
    hold_start_frames = frames_s(300);
    cone_check_delay_frames = frames_s(400);
    cycle_reset_frames = frames_s(560);
    spin_ramp_frames = frames_s(200);
    set_nonzero_follow_camera_parameters(1.0f, 12.0f, 0.11f);
    if (D_0013C940_s.x != 0.0f)
        *(short *)(slot_data + 0x20) = hold_start_frames;
    else if (D_0013C940_s.y != 0.0f)
        *(short *)(slot_data + 0x20) = hold_start_frames;
    if (*(short *)(slot_data + 0x20) >= hold_start_frames) {
        if (++*(int *)(slot_data + 0x3C) >= cone_check_delay_frames && check_moby_target_angular_limits(moby, 30.0f, 0.0f))
            *(short *)(slot_data + 0x20) = cycle_reset_frames;
        else
            done = 0;
    }
    if (*(short *)(slot_data + 0x20) >= cycle_reset_frames) {
        done = 1;
        *(int *)(slot_data + 0x3C) = 0;
        *(short *)(slot_data + 0x20) = 1;
    }
    if (spin_ramp_frames < *(short *)(slot_data + 0x20) && *(short *)(slot_data + 0x20) < hold_start_frames) {
        *(int *)(slot_data + 0x3C) = hold_start_frames;
        *(short *)(slot_data + 0x20) = spin_ramp_frames;
    }
    spin_ramp_ratio = itof_s(*(short *)(slot_data + 0x20)) / itof_s(spin_ramp_frames);
    spin_angle_radians = *(float *)slot_data * DEG_TO_RAD * spin_ramp_ratio;
    if (done)
        set_follow_camera_angle_blend_from_position(target_record + 0x30, spin_angle_radians, 0.0f);
    if (*(float *)(slot_data + 0x34) != 0.0f) {
        pitch_s(0, *(float *)(slot_data + 0x34), 0.003f);
        FUN_L00_002e8620();
    }
    if (*(float *)(slot_data + 0x38) != 0.0f)
        yaw_s(*(float *)(slot_data + 0x38), 0.003f);
    if (*(float *)(slot_data + 0x44) != 0.0f)
        zoom_s(0, *(float *)(slot_data + 0x44), 0.005f);
}
/* Hands a vendor moby to the shop handler when the menu is idle and its slot is live. */

extern char *D_L15_00167400_lui __asm__("D_L15_00167400") __attribute__((section(".data")));
extern unsigned char D_001413F4 __attribute__((section(".data")));
void update_moby_follow_camera(char *m) __asm__("FUN_L15_002f88e8");

int dispatch_moby_follow_camera_update(char *moby) __asm__("FUN_L15_002f8ba8");

int dispatch_moby_follow_camera_update(char *moby) {
    char *slot_data = *(char **)(D_L15_0015EF48[2] + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(short *)(D_L15_00167400_lui + 0x86) == 0 && *(short *)(slot_data + 0x20) >= 0 &&
        D_001413F4 == 2)
        update_moby_follow_camera(moby);
    return -1;
}
