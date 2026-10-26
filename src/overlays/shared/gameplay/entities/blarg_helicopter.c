/* Shared moby classes: spawner, bomb, blarg_helicopter, statue, fred_1, qwark_rilgar, garage_door. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/collision.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_00292AC0), where it is exact; names translated to the US level program. */

extern char D_L03_001CAF00[];
extern void FUN_L02_0024f748(char *);
extern void FUN_L02_0024fa80(char *);
extern void build_interpolated_dma_packet(char *) __asm__("FUN_L02_002a40f0");
extern void restore_view_after_textured_grid(char *) __asm__("FUN_L02_002a46e0");
extern void WriteDmaChannel(u32, u32, u32);
extern void FUN_L02_002101b8(u32, u32, u32);

void draw_textured_grid(char *moby) __asm__("FUN_L03_00291918");

void draw_textured_grid(char *moby) {
    build_interpolated_dma_packet(moby);
    FUN_L02_002101b8(D_L03_001CAF00, 0x70002800, 0xF8);
    FUN_L02_0024f748(moby);
    FUN_L02_0024fa80(moby);
    restore_view_after_textured_grid(moby);
}
#include "qcopy.h"
#include "rnc/overlay/entities.h"

/* update: follow the parent, blend its color, scale by the wind factor */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002BC140), where it is exact; names translated to the US level program. */

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L00_00262cf0(void *, void *, float, float, float);
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void update_parent_following_effect(char *moby) __asm__("FUN_L03_002bae48");

void update_parent_following_effect(char *moby) {
    char *moby_pvars = *(char **)(moby + 0x78);
    float ratio;
    float accumulated_offset;
    int blended_color;
    int g;
    int b;
    if (fast_dec_timer_s16(moby_pvars + 0xE)) {
        mark_moby_for_removal(moby);
        return;
    }
    *(float *)(moby + 0x44) = *(float *)(*(char **)moby_pvars + 0x44);
    *(float *)(moby + 0x48) = *(float *)(*(char **)moby_pvars + 0x48);
    qcopy(moby + 0x10, *(char **)(*(char **)moby_pvars + 0x78) + 0xF0);
    accumulated_offset = *(float *)(moby_pvars + 0x10) + *(float *)(moby_pvars + 0x14);
    *(float *)(moby_pvars + 0x10) = accumulated_offset;
    FUN_L00_00262cf0(moby + 0x10, moby + 0x10, -accumulated_offset, *(float *)(moby + 0x48),
                     *(float *)(moby + 0x44));
    *(float *)(moby + 0x40) =
        fast_add_rotations(*(float *)(moby + 0x40), frame_time * 5.235987663269043f);
    ratio = convert_integer_to_float(*(short *)(moby_pvars + 0xE));
    ratio = ratio / convert_integer_to_float(*(short *)(moby_pvars + 0xC));
    blended_color = fast_tween_color(*(int *)(moby_pvars + 8), *(int *)(moby_pvars + 4), ratio);
    b = (blended_color >> 16) & 0xFF;
    g = (blended_color >> 8) & 0xFF;
    set_moby_color(moby, blended_color & 0xFF, g, b);
    moby[0x23] = func_001FA898_r(ratio * 128.0f);
    *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * (frame_scale * 0.019999981f + 1.0f);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002BFF68), where it is exact; names translated to the US level program. */

extern char *D_L03_001600EC;
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");

int sample_random_transformed_point(int transform_index, float *out) __asm__("FUN_L03_002beba0");

int sample_random_transformed_point(int transform_index, float *out) {
    char *selected_transform;
    if (transform_index != -1) {
        selected_transform = D_L03_001600EC + transform_index * 128;
        out[0] = random_float_between_alt(-1.0f, 1.0f);
        out[1] = random_float_between_alt(-1.0f, 1.0f);
        out[2] = random_float_between_alt(-1.0f, 1.0f);
        transform_vector_by_basis(out, out, selected_transform);
        fast_vec_add(out, out, selected_transform + 0x30);
        return 0;
    }
    return 1;
}
/* Class header fields the spawner copies into a fresh moby. */
typedef struct {
    u8 pad0[0xF];
    u8 always_draw; /* 0x0F */
    s32 unk10;      /* 0x10 */
    u8 pad14[0x10];
    f32 scale;      /* 0x24 */
    u8 pad28[0x18];
    s32 unk40;      /* 0x40 */
    u16 flags;      /* 0x44 */
} SpawnClass;

/* Spawner pvars. */
typedef struct {
    u8 pad0[0x83];
    u8 options;     /* 0x83: bit 1 = spawn at a tenth of the class scale */
    u8 pad84[2];
    u8 live;        /* 0x86: children alive */
    u8 pad87[0x61];
    s32 slots[18];  /* 0xE8: moby indices of the pooled children, -1 = empty */
} Spawner;

typedef struct {
    f32 timer;      /* 0x00 */
    s16 duration;   /* 0x04 */
    u8 pad6[0x2A];
    s32 unk30;
    s32 unk34;
} SpawnSound;

typedef struct {
    u8 pad0[0x68];
    s16 unk68;
    u8 pad6A[2];
    s32 unk6C;
    u8 pad70[0x24];
    s32 unk94;
} SpawnTarget;

extern struct Moby *D_L03_0015FFD8_mb __asm__("D_L03_0015FFD8") __attribute__((sda));
extern struct Moby *D_L03_0015FFE4;
extern s32 D_L03_0015F5CC;
extern char D_L03_001E2FA0[];

extern void set_moby_animation(struct Moby *, s32, s32) __asm__("FUN_00212ed8");
extern SpawnTarget *get_moby_suck_cannon_record(struct Moby *) __asm__("FUN_L00_002db890");
extern void copy_hero_moby_field_38(struct Moby *) __asm__("FUN_L00_0025d1b8");
extern SpawnSound *suck_get_aim_height(struct Moby *) __asm__("FUN_002141f8");
extern void debug_print() __asm__("FUN_001e93b0");

/* Spawner: revives the first free pooled child (of class oclass, -1 = any) at a copy of at, counts the live children, returns the revived one or NULL. */
struct Moby *revive_spawner_pooled_child(struct Moby *spawner_moby, Vec4 *at, s32 oclass) __asm__("FUN_L03_002bec60");

struct Moby *revive_spawner_pooled_child(struct Moby *spawner_moby, Vec4 *at, s32 oclass)
{
    Vec4 pos;
    Vec4 *spawn_position_source;
    Spawner *spawner_data;
    struct Moby *current_moby;
    struct Moby *spawned;
    SpawnClass *spawned_moby_class;
    SpawnTarget *target;
    SpawnSound *sound;
    s32 found;
    s32 i;
    s32 pooled_child_moby_index;
    u16 flags;

    spawn_position_source = &pos;
    pos.q = at->q;
    if (spawner_moby == NULL || (spawner_data = (Spawner *)spawner_moby->pvars) == NULL) {
        return NULL;
    }
    spawner_data->live = 0;
    found = 0;
    spawned = NULL;
    for (i = 0; i < 18; i++) {
        pooled_child_moby_index = spawner_data->slots[i];
        if (pooled_child_moby_index == -1) {
            continue;
        }
        current_moby = &D_L03_0015FFD8_mb[pooled_child_moby_index];
        if (current_moby != NULL && current_moby->state != 0xFE && current_moby->state != 0xFD) {
            spawner_data->live++;
            continue;
        }
        if (oclass != -1 && oclass != current_moby->oclass) {
            continue;
        }
        if (found) {
            continue;
        }
        spawner_data->live++;
        found = 1;
        qcopy(&current_moby->pos, spawn_position_source);
        current_moby->pos.w = 0.0f;
        current_moby->state = 0;
        current_moby->command = 0;
        spawned_moby_class = (SpawnClass *)current_moby->pclass;
        flags = spawned_moby_class->flags;
        current_moby->flags = flags;
        current_moby->scale = spawned_moby_class->scale;
        current_moby->update_dist = spawner_moby->update_dist;
        current_moby->draw_dist = spawner_moby->draw_dist;
        current_moby->visible = found;
        if (spawned_moby_class->unk40 != 0) {
            current_moby->flags = flags | 0x10;
        }
        if (((SpawnClass *)current_moby->pclass)->always_draw != 0) {
            current_moby->flags |= 0x400;
        }
        current_moby->unk71 = 0xFF;
        current_moby->occlusion = 0x7F80;
        current_moby->unk72 = 0xFF;
        current_moby->hit_slot = 0xFF;
        if (spawner_data->options & 2) {
            current_moby->scale = ((SpawnClass *)current_moby->pclass)->scale * 0.1f;
        }
        set_moby_animation(current_moby, 0, 0);
        target = get_moby_suck_cannon_record(current_moby);
        if (target != NULL) {
            target->unk94 = 0;
            target->unk68 = 0;
            target->unk6C = 0;
        }
        if (spawner_moby->unkB4 < current_moby->unkB4) {
            current_moby->unkB4 = spawner_moby->unkB4;
        }
        if (current_moby->unkB4 <= 0) {
            current_moby->unkB4 = 1;
        }
        spawner_moby->unkB4 -= current_moby->unkB4;
        if (spawner_moby->unkB4 < 0) {
            spawner_moby->unkB4 = 1;
        }
        current_moby->collision = ((SpawnClass *)current_moby->pclass)->unk10;
        copy_hero_moby_field_38(current_moby);
        FUN_L00_00250df8(current_moby);
        spawned = current_moby;
        sound = suck_get_aim_height(spawned);
        if (sound != NULL) {
            sound->unk34 = 0;
            sound->unk30 = 0;
            sound->timer = sound->duration;
        }
    }
    for (current_moby = D_L03_0015FFE4; current_moby != NULL; current_moby = current_moby->next) {
        if (current_moby->oclass == 0x221 && current_moby->state != 0xFE && current_moby->state != 0xFD) {
            spawner_data->live++;
        }
    }
    if (spawned == NULL) {
        debug_print(D_L03_001E2FA0, D_L03_0015F5CC, (s16)spawner_moby->save_id, spawner_moby->oclass);
    }
    return spawned;
}
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) Vec4A_2bef68;
extern char * D_L03_00160064_p __asm__("D_L03_0015FFE4") __attribute__((section(".sdata")));
extern char * D_L03_00160058_p __asm__("D_L03_0015FFD8") __attribute__((section(".sdata")));
extern int FUN_001f9770_2bef68(void *) __asm__("FUN_001f9770");
extern void FUN_001f9a10_2bef68(void *, void *, void *) __asm__("FUN_001f9a10");
extern float FUN_001fa6c0_2bef68(int) __asm__("FUN_001fa6c0");
extern void FUN_0020c828_2bef68(void *) __asm__("FUN_0020c828");
extern float FUN_002132a8_2bef68(float, float) __asm__("FUN_002132a8");
typedef struct Data_2C0330 {
    char pad0[0x20];
    float hp;
    char pad24[5];
    unsigned char b29;
    char pad2A[0x2E];
    unsigned char b58;
    char pad59;
    unsigned char b5A;
    char pad5B[5];
    short f60[3];
    char b66;
    unsigned char b67;
    char pad68[8];
    char f70[0x12];
    unsigned char b82;
    unsigned char b83;
    unsigned char b84;
    unsigned char b85;
    unsigned char count;
    unsigned char max;
    short timer;
    short h8A;
    short h8C;
    char pad8E[2];
    float f90;
    char pad94[8];
    float f9C;
    int pts[4];
    char padB0[0x20];
    int fD0;
    char padD4[8];
    float fDC;
    char padE0[4];
    float fE4;
    int ids[18];
} Data_2C0330;
typedef struct {
    char pad0[0x2C];
    float f2C;
} Hit_2C0330;
extern float FUN_001f9b48_2bef68(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_L00_0025a120_2bef68(void *) __asm__("FUN_L00_0025a120");
extern float FUN_001f9b80_2bef68(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_00214720_2bef68(void *arg0, int arg1) __asm__("FUN_00214720");
extern char *FUN_L00_0025a420_2bef68(void *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_00213928_2bef68(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void FUN_L00_0025d458_2bef68(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void FUN_L00_00257470_2bef68(void *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025e450_2bef68(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263fd8_2bef68(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void FUN_001153fc_2bef68(void *, int, int) __asm__("FUN_001153fc");
extern void FUN_L00_0024f7c8_2bef68(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float FUN_L00_0025e310_2bef68(float) __asm__("FUN_L00_0025e310");
extern float FUN_001f9dc8_2bef68(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2bef68(float) __asm__("FUN_001f9de0");
extern void FUN_001f9c90_2bef68(void *, void *, float) __asm__("FUN_001f9c90");
extern void FUN_001f9a28_2bef68(void *,void *,void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a68_2bef68(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f9af0_2bef68(void *a) __asm__("FUN_001f9af0");
extern void FUN_001f9bf8_2bef68(void *,void *,float) __asm__("FUN_001f9bf8");
extern float FUN_00213508_2bef68(void *, int, float) __asm__("FUN_00213508");
extern int FUN_001160d8_2bef68(void) __asm__("FUN_001160d8");
extern void add_random_offset_to_vector_xyz(float *v, float s) __asm__("FUN_L00_0025f8e0");
extern void FUN_L00_001ff290_2bef68(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern float FUN_L00_0025abf0_2bef68(void *, void *, void *, float, float) __asm__("FUN_L00_0025abf0");
extern char *spawn_class_221_moby(char *owner, float *pos, float *dir, short ang, int arg) __asm__("FUN_L03_002c45d8");
extern int FUN_001f96f8_2bef68(int) __asm__("FUN_001f96f8");
extern void FUN_L00_0025d538_2bef68(void *, void *) __asm__("FUN_L00_0025d538");
extern char D_L03_00166EC0[];
extern char D_0013E533_2bef68[] __asm__("D_0013E533");
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") __attribute__((section(".sdata")));
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));

extern f32 frame_scale_macro __asm__("D_0015ED60") __attribute__((section(".sdata")));
/* Update of moby class 455 (spawner): counts its living children, takes hits, runs its state machine and spawns class 0x221 children at a point picked by its mode. */
void spawner_update(struct Moby *moby) __asm__("FUN_L03_002bef68");
void spawner_update(struct Moby *moby) {
    enum {
        SPAWNER_INITIALIZE = 0,
        SPAWNER_WAIT_FOR_PROXIMITY = 1,
        SPAWNER_SPAWN_ENABLED = 2,
        SPAWNER_SPAWN_ENABLED_SHARED = 3,
        SPAWNER_ALLOW_SPAWNING = 4,
        SPAWNER_SPAWN_ENABLED_AFTER_PROXIMITY = 5,
        SPAWNER_SPAWN_ENABLED_ALIAS = 6,
        SPAWNER_SPAWN_ENABLED_SHARED_ALIAS = 7,
        SPAWNER_DESTROY = 8,
        SPAWNER_RESET_TO_PROXIMITY_WAIT = 9
    };
    enum {
        SPAWNER_TRIGGER_DISTANCE_ONLY = 0,
        SPAWNER_TRIGGER_VOLUME_ONLY = 1,
        SPAWNER_TRIGGER_VOLUME_OR_DISTANCE = 2
    };
    enum {
        SPAWNER_TARGET_COUNT_ANGULAR_SPREAD = 0,
        SPAWNER_TARGET_HALF_REFERENCE_OFFSET = 1,
        SPAWNER_TARGET_UNUSED_POINT = 2,
        SPAWNER_TARGET_HELPER_INDEXED_POINT = 3,
        SPAWNER_TARGET_COUNT_INDEXED_POINT = 4,
        SPAWNER_TARGET_SKIP_SETUP = 5
    };
    enum {
        SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED = 0,
        SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED = 1,
        SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED_ALIAS = 2,
        SPAWNER_ACTIVATION_SET_SPAWN_ENABLED = 3,
        SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY = 4,
        SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY_ALIAS = 5,
        SPAWNER_ACTIVATION_STAY_IN_PROXIMITY_WAIT = 6
    };
    int used[4];
    Vec4A_2bef68 launch_origin;
    Vec4A_2bef68 launch_velocity;
    Vec4A_2bef68 target_position;
    Vec4A_2bef68 spawn_position;
    Vec4A_2bef68 sampled_point_scratch;
    int damage_query_status;
    Data_2C0330 *spawner_data;
    Hit_2C0330 *r;
    int *ids;
    int i;
    int near1;
    int near2;
    int timerDone;
    int spawn;
    int inVol;
    int room;
    int cond;
    float reference_distance_xy;
    float target_ground_height;
    int kids = 18;

    if (moby == 0) return;
    spawner_data = (Data_2C0330 *)moby->pvars;
    if (moby->visible != 0 && FUN_001f9b48_2bef68(&moby->pos, D_L03_00166EC0) < 28.0f) {
        FUN_L00_0025a120_2bef68(moby);
        moby->shadow_range = 0x16;
    }
    if (spawner_data == 0) return;
    ids = spawner_data->ids;
    spawner_data->count = 0;
    for (i = 0; i < kids; i++) {
        struct Moby *o;
        int id = ids[i];
        if (id == -1) continue;
        o = (struct Moby *)(D_L03_00160058_p + (id << 8));
        if (o != 0 && o->state != 0xFE && o->state != 0xFD) {
            spawner_data->count++;
        }
    }
    {
        struct Moby *o;
        for (o = (struct Moby *)D_L03_00160064_p; o != 0; o = o->next) {
            if (o->oclass == 0x221 && o->state != 0xFE && o->state != 0xFD) {
                spawner_data->count++;
            }
        }
    }
    reference_distance_xy = FUN_001f9b80_2bef68(&moby->pos, D_0013E533_2bef68 + 0xE9D);
    near1 = reference_distance_xy < spawner_data->f9C;
    near2 = reference_distance_xy < spawner_data->f90;
    spawn = 0;
    timerDone = FUN_001f9770_2bef68(&spawner_data->timer) != 0;
    inVol = 0;
    room = spawner_data->count < spawner_data->max;
    if (spawner_data->fD0 != -1) {
        inVol = FUN_00214720_2bef68(D_0013E533_2bef68 + 0xE9D, spawner_data->fD0) != 0;
    }
    cond = inVol | near2;
    switch (spawner_data->b82) {
    case SPAWNER_TRIGGER_DISTANCE_ONLY:
        cond = near1;
        break;
    case SPAWNER_TRIGGER_VOLUME_ONLY:
        cond = inVol;
        break;
    case SPAWNER_TRIGGER_VOLUME_OR_DISTANCE:
        break;
    }
    if (spawner_data->h8A == 0 && spawner_data->count != 0) {
        room = 0;
    }
    r = (Hit_2C0330 *)FUN_L00_0025a420_2bef68(moby, 0xC30000, 0);
    FUN_00213928_2bef68(moby, r, &spawner_data->hp, 0, &damage_query_status, 0, 0, 4);
    if (damage_query_status >= 2) {
        float remaining_health = spawner_data->hp - r->f2C;
        spawner_data->hp = remaining_health;
        if (remaining_health > 0.0f) {
            spawner_data->b67 = 0x5A;
            FUN_L00_0025d458_2bef68(moby, spawner_data->f60);
        } else {
            FUN_L00_00257470_2bef68(moby, 0, -1);
            moby->state = 8;
            spawner_data->b67 = 0x78;
            FUN_L00_0025d458_2bef68(moby, spawner_data->f60);
        }
    }
    moby->hit_slot = 0xFF;
    switch (moby->state) {
    case SPAWNER_INITIALIZE: {
        int k;
        for (k = 0; k < kids; k++) {
            struct Moby *o;
            int id = ids[k];
            if (id == -1) continue;
            o = (struct Moby *)(D_L03_00160058_p + (id << 8));
            if (o != 0 && o->state != 0xFE && o->state != 0xFD) {
                FUN_0020c828_2bef68(o);
            }
        }
        moby->flags |= 0x1000;
        spawner_data->b5A = 3;
        spawner_data->b58 = 8;
        spawner_data->b29 = 0;
        moby->state = 1;
        break;
    }
    case SPAWNER_WAIT_FOR_PROXIMITY:
        if (near1) {
            switch (spawner_data->b84) {
            case SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED:
                moby->state = 2;
                break;
            case SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED:
                moby->state = 3;
                break;
            case SPAWNER_ACTIVATION_ENTER_SHARED_SPAWN_ENABLED_ALIAS:
                moby->state = 7;
                break;
            case SPAWNER_ACTIVATION_SET_SPAWN_ENABLED:
                moby->state = 2;
                break;
            case SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY:
                moby->state = 5;
                break;
            case SPAWNER_ACTIVATION_ENTER_SPAWN_ENABLED_AFTER_PROXIMITY_ALIAS:
                moby->state = 5;
                break;
            case SPAWNER_ACTIVATION_STAY_IN_PROXIMITY_WAIT:
            default:
                moby->state = 1;
                break;
            }
        }
        break;
    case SPAWNER_SPAWN_ENABLED:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_SHARED:
        spawn = 1;
        break;
    case SPAWNER_ALLOW_SPAWNING:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_AFTER_PROXIMITY:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_ALIAS:
        spawn = 1;
        break;
    case SPAWNER_SPAWN_ENABLED_SHARED_ALIAS:
        spawn = 1;
        break;
    case SPAWNER_DESTROY: {
        float z;
        float *p;
        Vec4A_2bef68 *spawner_rotation;
        z = 0.0f;
        p = D_L03_0015F580_macro;
        FUN_L00_0025e450_2bef68(moby, spawner_data->f70, 0, 0.0f, 0.0f, 0x1E, 8, 0x14, 8.0f, 4.0f, 18.0f, 1.0f, 1, 15.0f, 1, 1, -1, 0);
        spawner_rotation = (Vec4A_2bef68 *)&moby->rot;
        FUN_L00_00263fd8_2bef68(moby, 0x6C5, &moby->pos, spawner_rotation, 0, 0, z, p, p, p);
        FUN_L00_00263fd8_2bef68(moby, 0x6C6, &moby->pos, spawner_rotation, 0, 0, z, p, p, p);
        FUN_L00_00263fd8_2bef68(moby, 0x6C7, &moby->pos, spawner_rotation, 0, 0, z, p, p, p);
        FUN_0020c828_2bef68(moby);
        return;
    }
    case SPAWNER_RESET_TO_PROXIMITY_WAIT:
    default:
        moby->state = 1;
        break;
    }
    if (spawn && cond && timerDone && room) {
        int *used_point_flags = used;
        FUN_001153fc_2bef68(used_point_flags, 0, 0x10);
        while (spawner_data->h8A == 0 ? spawner_data->count < spawner_data->max : spawner_data->timer == 0) {
            FUN_L00_0024f7c8_2bef68(moby, 0, &launch_origin);
            switch (spawner_data->b85) {
            case SPAWNER_TARGET_COUNT_ANGULAR_SPREAD: {
                float target_spread_angle;
                float rad;
                target_spread_angle = FUN_L00_0025e310_2bef68(FUN_001fa6c0_2bef68(spawner_data->count) * 2.0f * 3.1415927f / FUN_001fa6c0_2bef68(spawner_data->max + 1) - 3.1415927f);
                rad = FUN_001f9b80_2bef68(&moby->pos, D_0013E533_2bef68 + 0xE9D) * 0.5f;
                if (spawner_data->b83 & 8) {
                    rad = FUN_002132a8_2bef68(rad, 3.0f);
                }
                if (rad < 3.0f) {
                    rad = 3.0f;
                }
                launch_velocity.x = FUN_001f9dc8_2bef68(target_spread_angle) * rad;
                launch_velocity.y = FUN_001f9de0_2bef68(target_spread_angle) * rad;
                launch_velocity.z = 0.0f;
                FUN_001f9c90_2bef68(&launch_velocity, &launch_velocity, spawner_data->f90);
                FUN_001f9a10_2bef68(&target_position, &launch_origin, &launch_velocity);
                target_ground_height = FUN_00213508_2bef68(&target_position, 0, 0.5f);
                if (target_ground_height != 0.0f) {
                    target_position.z = target_ground_height;
                }
                break;
            }
            case SPAWNER_TARGET_HALF_REFERENCE_OFFSET: {
                float len;
                FUN_001f9a28_2bef68(&launch_velocity, D_0013E533_2bef68 + 0xE9D, &launch_origin);
                launch_velocity.z = 0.0f;
                FUN_001f9a68_2bef68(&launch_velocity, &launch_velocity, 0.5f);
                len = FUN_001f9af0_2bef68(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    FUN_001f9bf8_2bef68(&launch_velocity, &launch_velocity, FUN_002132a8_2bef68(len, 3.0f));
                }
                if (FUN_001f9af0_2bef68(&launch_velocity) < 3.0f) {
                    FUN_001f9bf8_2bef68(&launch_velocity, &launch_velocity, 3.0f);
                }
                FUN_001f9c90_2bef68(&launch_velocity, &launch_velocity, spawner_data->f90);
                FUN_001f9a10_2bef68(&target_position, &launch_origin, &launch_velocity);
                target_ground_height = FUN_00213508_2bef68(&target_position, 0, 0.5f);
                if (target_ground_height != 0.0f) {
                    target_position.z = target_ground_height;
                }
                break;
            }
            case SPAWNER_TARGET_UNUSED_POINT: {
                float best = 100000.0f;
                int bi = -1;
                int j;
                float len;
                for (j = 0; j < 4; j++) {
                    float dd;
                    if (used[j] == 1) continue;
                    if (sample_random_transformed_point(spawner_data->pts[j], (float *)&sampled_point_scratch) != 0) continue;
                    dd = FUN_001f9b80_2bef68(&moby->pos, D_0013E533_2bef68 + 0xE9D);
                    if (dd < best) {
                        qcopy(&spawn_position, &sampled_point_scratch);
                        best = dd;
                        bi = j;
                    }
                }
                if (bi != -1) {
                    used[bi] = 1;
                }
                FUN_001f9a28_2bef68(&launch_velocity, &spawn_position, &launch_origin);
                len = FUN_001f9af0_2bef68(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    FUN_001f9bf8_2bef68(&launch_velocity, &launch_velocity, FUN_002132a8_2bef68(len, 3.0f));
                }
                qcopy(&target_position, &spawn_position);
                break;
            }
            case SPAWNER_TARGET_HELPER_INDEXED_POINT: {
                int *p = spawner_data->pts;
                int n = 0;
                int k;
                int c;
                int j;
                float len;
                for (c = 0; c < 4; c++) {
                    if (p[c] != -1) n++;
                }
                k = FUN_001160d8_2bef68() % n + 1;
                for (j = 0; j < 4; j++) {
                    if (p[j] != -1) k--;
                    if (k != 0) continue;
                    if (sample_random_transformed_point(p[j], (float *)&spawn_position) != 0) continue;
                    break;
                }
                FUN_001f9a28_2bef68(&launch_velocity, &spawn_position, &launch_origin);
                len = FUN_001f9af0_2bef68(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    FUN_001f9bf8_2bef68(&launch_velocity, &launch_velocity, FUN_002132a8_2bef68(len, 3.0f));
                }
                qcopy(&target_position, &spawn_position);
                break;
            }
            case SPAWNER_TARGET_COUNT_INDEXED_POINT: {
                int *p = spawner_data->pts;
                int point_selection_spawn_count = spawner_data->count;
                int n = 0;
                int k;
                int c;
                int j;
                float len;
                for (c = 0; c < 4; c++) {
                    if (p[c] != -1) n++;
                }
                k = point_selection_spawn_count % n + 1;
                for (j = 0; j < 4; j++) {
                    if (p[j] != -1) k--;
                    if (k != 0) continue;
                    if (sample_random_transformed_point(p[j], (float *)&spawn_position) != 0) continue;
                    break;
                }
                FUN_001f9a28_2bef68(&launch_velocity, &spawn_position, &launch_origin);
                len = FUN_001f9af0_2bef68(&launch_velocity);
                if (spawner_data->b83 & 8) {
                    FUN_001f9bf8_2bef68(&launch_velocity, &launch_velocity, FUN_002132a8_2bef68(len, 3.0f));
                }
                qcopy(&target_position, &spawn_position);
                break;
            }
            case SPAWNER_TARGET_SKIP_SETUP:
                break;
            }
            {
                float speed;
                char *spawned_moby;
                speed = FUN_001f9af0_2bef68(&launch_velocity) * (spawner_data->fE4 * frame_scale_macro);
                launch_velocity.z = 0.0f;
                FUN_001f9bf8_2bef68(&launch_velocity, &launch_velocity, speed);
                FUN_001f9bf8_2bef68(&spawn_position, &launch_velocity, 0.05f);
                add_random_offset_to_vector_xyz((float *)&spawn_position, 0.025f);
                FUN_L00_001ff290_2bef68(&sampled_point_scratch, &spawn_position, &launch_origin);
                launch_velocity.z = FUN_L00_0025abf0_2bef68(&spawn_position, &target_position, 0, speed, -(spawner_data->fDC * frame_time_sq_macro));
                spawned_moby = spawn_class_221_moby((char *)moby, (float *)&spawn_position, (float *)&launch_velocity, spawner_data->h8C, spawner_data->b83);
                if (spawned_moby != 0) {
                    *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
                }
                spawner_data->timer = FUN_001f96f8_2bef68(spawner_data->h8A);
                spawner_data->count++;
            }
        }
    }
    FUN_L00_0025d538_2bef68(moby, spawner_data->f60);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002C59A0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8(void *);
extern char *func_0020D348_m_2C59A0(int) __asm__("FUN_0020c4f8");

char *spawn_class_221_moby(char *owner, float *pos, float *launch_velocity, short ang, int spawn_options) {
    char *spawned_moby;
    char *moby_pvars;
    spawned_moby = func_0020D348_m_2C59A0(0x221);
    if (spawned_moby != 0) {
        moby_pvars = *(char **)(spawned_moby + 0x78);
        *(char **)(moby_pvars + 0x10) = owner;
        qcopy(spawned_moby + 0x10, pos);
        qcopy(moby_pvars, launch_velocity);
        *(float *)(spawned_moby + 0x40) = random_float_between_alt(-3.1415927f, 3.1415927f);
        *(float *)(spawned_moby + 0x44) = random_float_between_alt(-3.1415927f, 3.1415927f);
        *(int *)(moby_pvars + 0x18) = 0;
        *(short *)(moby_pvars + 0x14) = scale_game_frames(500);
        *(short *)(moby_pvars + 0x16) = ang;
        *(unsigned char *)(spawned_moby + 0x30) = (*(unsigned char **)(moby_pvars + 0x10))[0x30];
        *(unsigned short *)(spawned_moby + 0x32) = *(unsigned short *)(*(char **)(moby_pvars + 0x10) + 0x32);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 0;
        *(float *)(spawned_moby + 0x2C) = *(float *)(*(char **)(spawned_moby + 0x24) + 0x24) * 0.001f;
        *(int *)(moby_pvars + 0x1C) = spawn_options;
        *(int *)(moby_pvars + 0x20) = 0;
        FUN_L00_00250df8(spawned_moby);
    }
    return spawned_moby;
}
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_001ff660(void *, void *, void *);
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int FUN_0022da68(int, int, void *);
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float FUN_002132a8(float, float);
extern void jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern void FUN_0020c828(void *);
extern CollisionHit D_L03_00173F40 __attribute__((section(".data")));

/* Drifts and bounces off walls (state 0), lands and spawns its child moby (1), then shrinks while the child grows (2). */
void FUN_L03_002c4718(struct Moby *moby)
{
    float *vars;
    Vec4 *pos;
    struct Moby *other;
    Vec4 old_pos;
    Vec4 next_pos;
    Vec4 delta;
    int old_disable, old_owner_disable, old_collision, old_owner_collision;
    float angle;
    float distance;

    if (moby == NULL) return;
    vars = (float *)moby->pvars;
    if (vars == NULL) return;
    pos = (Vec4 *)&moby->pos;
    fast_dec_timer(vars + 8);
    qcopy(&old_pos, pos);
    switch (moby->state) {
    case 0: {
        moby->scale += (moby->pclass->scale - moby->scale) * (frame_scale * 0.05f);
        fast_vec_add(pos, pos, vars);
        fast_vec_normalize(&next_pos, vars, 0.2f);
        fast_vec_add(&next_pos, &next_pos, pos);
        old_disable = moby->collision_disable;
        old_collision = moby->collision;
        old_owner_disable = (*(struct Moby **)(vars + 4))->collision_disable;
        old_owner_collision = (*(struct Moby **)(vars + 4))->collision;
        moby->collision_disable = -1;
        (*(struct Moby **)(vars + 4))->collision_disable = -1;
        moby->collision = 0;
        (*(struct Moby **)(vars + 4))->collision = 0;
        if (collision_line(&old_pos, &next_pos, 4, moby, 0)) {
            fast_vec_sub(&next_pos, &D_L03_00173F40.point, &next_pos);
            fast_vec_normalize(&next_pos, &next_pos, 0.215f);
            fast_vec_add(pos, pos, &next_pos);
            if (D_L03_00173F40.moby == 0) {
                FUN_L00_001ff660(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f && collision_hit_material() == -1) {
                    fast_vec_scale(vars, vars, 0.5f);
                    if (fast_vec_length(vars) < 0.025f) {
                        moby->state = 1;
                        goto first_collision_timer;
                    }
                }
            } else if (((struct Moby *)D_L03_00173F40.moby)->oclass != 0x221) {
                FUN_L00_001ff660(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f && collision_hit_material() == -1)
                    fast_vec_scale(vars, vars, 1.2f);
            } else {
                vars[2] *= FUN_002132a8(0.9f, 1.1f);
            }
first_collision_timer:
            if (*(int *)(vars + 8) == 0) {
                FUN_0022da68(0, 0, moby);
                *(int *)(vars + 8) = 12;
            }
        }
        if (coll_sphere(0.2f, (Vec4 *)&moby->pos, 4, moby)) {
            fast_vec_sub(&next_pos, &D_L03_00173F40.point, &D_L03_00173F40.pad30);
            fast_vec_normalize(&next_pos, &next_pos, 0.015f);
            fast_vec_add((Vec4 *)&moby->pos, &D_L03_00173F40.pad30, &next_pos);
            if (D_L03_00173F40.moby == 0) {
                FUN_L00_001ff660(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f && collision_hit_material() == -1) {
                    fast_vec_scale(vars, vars, 0.5f);
                    if (fast_vec_length(vars) < 0.025f) moby->state = 1;
                }
            } else if (((struct Moby *)D_L03_00173F40.moby)->oclass != 0x221) {
                FUN_L00_001ff660(vars, vars, &D_L03_00173F40.normal_x);
                angle = vector_length_xy(&D_L03_00173F40.normal_x);
                if (fast_arc_tan(D_L03_00173F40.normal_z, angle) < 0.6981317f && collision_hit_material() == -1)
                    fast_vec_scale(vars, vars, 1.2f);
            } else {
                vars[2] *= FUN_002132a8(0.9f, 1.1f);
            }
            if (*(int *)(vars + 8) == 0) {
                FUN_0022da68(0, 0, moby);
                *(int *)(vars + 8) = 12;
            }
        }
        for (other = D_L03_0015FFE4; other != NULL; other = other->next) {
            if (other == moby || (int)other == 0 || other->state == 0xfe || other->state == 0xfd) continue;
            fast_vec_sub(&delta, (Vec4 *)&moby->pos, &other->pos);
            delta.f[2] = 0.0f;
            distance = fast_vec_length(&delta);
            if (distance < 1.8f) {
                fast_vec_scale(&delta, &delta, (1.8f - distance) * (frame_scale * -0.5f + 1.0f));
                fast_vec_add((Vec4 *)&moby->pos, (Vec4 *)&moby->pos, &delta);
            }
        }
        if (fast_dec_timer_s16(vars + 5)) moby->state = 2;
        moby->rot.x = normalize_angle(vars[0]);
        moby->rot.y = normalize_angle(vars[1]);
        moby->collision_disable = old_disable;
        (*(struct Moby **)(vars + 4))->collision_disable = old_owner_disable;
        moby->collision = old_collision;
        (*(struct Moby **)(vars + 4))->collision = old_owner_collision;
        vars[2] -= frame_time_sq * 9.8f;
        moby->rot.x += frame_scale * 0.01f;
        moby->rot.y += frame_scale * 0.02f;
    break;
    }
    case 1: {
        float height = probe_ground_height(pos, 0, 0.5f);
        moby->pos.w = 0.0f;
        moby->pos.z = height + 0.2f;
        next_pos.q = *(u128 *)pos;
        *(struct Moby **)(vars + 6) = revive_spawner_pooled_child(*(struct Moby **)(vars + 4), &next_pos, -1);
        moby->state = 2;
    break;
    }
    case 2: {
        struct Moby *other;
        float rate = frame_scale * 0.05f;
        moby->scale -= moby->scale * rate;
        other = *(struct Moby **)(vars + 6);
        if (other != NULL && (*(int *)(vars + 7) & 4))
            other->scale += (other->pclass->scale - other->scale) * rate;
        if (moby->scale < moby->pclass->scale * (frame_scale * 0.05f)) {
            moby->scale = 0.0001f;
            other = *(struct Moby **)(vars + 6);
            if (other == NULL || other->state == 0xfe || other->state == 0xfd ||
                !(*(int *)(vars + 7) & 4) || other->pclass->scale - 0.01f <= other->scale)
                goto remove_moby;
        }
    }
    }
    jet_animate(moby, moby->scale * 0.3f / moby->pclass->scale);
    if (moby->pos.z < 5.0f || moby->pos.z > 500.0f) {
remove_moby:
        FUN_0020c828(moby);
        return;
    }
    if (moby->pos.x < 5.0f) moby->pos.x = 5.0f;
    /* Fakematch: the do-while only weights moby's uses for the register allocator, so moby stays in $18 like retail. */
    if (moby->pos.x > 1018.0f) do { moby->pos.x = 1018.0f; } while (0);
    if (moby->pos.y < 5.0f) moby->pos.y = 5.0f;
    if (moby->pos.y > 1018.0f) moby->pos.y = 1018.0f;
    if (moby->pos.z < 5.0f) moby->pos.z = 5.0f;
    if (moby->pos.z > 1018.0f) moby->pos.z = 1018.0f;
    return;
}
/* Recognize the active vendor object state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00310738), where it is exact; names translated to the US level program. */

int is_moby_class_23e_in_state_14(char *moby) __asm__("FUN_L03_002c6c60");

int is_moby_class_23e_in_state_14(char *moby) {
    if (*(short *)(moby + 0xA6) == 0x23e && ((unsigned char *)moby)[0x20] == 14) {
        return 1;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00301090), where it is exact; names translated to the US level program. */

void set_moby_state_14(unsigned char *moby) __asm__("FUN_L03_002c6c90");

void set_moby_state_14(unsigned char *moby) {
    moby[0x20] = 14;
}
/* Updates the gunship (helicopter) moby: activation path (1), patrol route with gun aiming and shots (2),
   exit path or climb-out (3), a short reaction after a route change (4) and destruction (5). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002CDDC8), where it is exact; names translated to the US level program. */

typedef float HVec49_2CDDC8[4] __attribute__((aligned(16)));

typedef unsigned int HQuad49_2CDDC8 __attribute__((mode(TI)));

typedef struct HMob49_2CDDC8 HMob49_2CDDC8;

typedef struct HData49_2CDDC8 HData49_2CDDC8;

typedef struct { float xyz[3],length; } HNode49_2CDDC8;

typedef struct { int count; char pad4[12]; HNode49_2CDDC8 points[1]; } HPath49_2CDDC8;

typedef struct { int index; char pad4[12]; HPath49_2CDDC8 *path; int path_id; char pad18[24]; } HRoute49_2CDDC8;

struct HData49_2CDDC8 {
    char pad0[0x20]; float health; short health_word; char pad26[2];
    unsigned char kind,flag29,pad2A[4],enabled,pad2F[9]; int request;
    char pad3C[0x1C]; unsigned char gun_kind,gun_enabled,gun_mode,pad5B[0xC5];
    HRoute49_2CDDC8 route_a,route_b; float gun_yaw_matrix[16],gun_pitch_matrix[16];
    float yaw_velocity; short shot_timer,reaction_timer; int shoot_region,switch_region;
    float bob,speed,pitch_velocity,roll_velocity,gun_yaw,gun_yaw_velocity,gun_pitch,gun_pitch_velocity;
    int exit_path,other_exit_path,child_index,alternate;
    HMob49_2CDDC8 *projectile; int other_switch_region,activation_region,activation_path,sound,timer;
};

struct HMob49_2CDDC8 {
    char pad0[16]; HVec49_2CDDC8 position; unsigned char state,pad21[15];
    unsigned char alpha,nearby; short brightness; unsigned short flags; char pad36[10];
    HVec49_2CDDC8 rotation; char pad50[0x28]; HData49_2CDDC8 *data; char pad7C[3]; unsigned char update_distance;
    char pad80[0x14]; int animation; char pad98[14]; short class_id; char padA8[8];
    unsigned char persistent,padB1; unsigned short pool_index; char padB4[8]; unsigned char previous_state;
    char padBD[3]; float transform[16];
};

typedef struct { HVec49_2CDDC8 position,rotation,from,to; HMob49_2CDDC8 *moby; int result,pad48,pad4C; } HTarget49_2CDDC8;

typedef struct {
    HVec49_2CDDC8 direction; HMob49_2CDDC8 *source; int flags; unsigned char kind,enabled; unsigned short class_id;
    float radius; int allow; char pad24[12];
} HQuery49_2CDDC8;

typedef struct {
    HTarget49_2CDDC8 target; HVec49_2CDDC8 movement,projected,offset; HQuery49_2CDDC8 hit;
    HVec49_2CDDC8 direction,shot_velocity,shot_origin; int segment_a; float fraction_a; int segment_b; float fraction_b;
} HWork49_2CDDC8;

char *spawn_homing_spark_projectile(char *a, char *pos, int c, int d, float f) __asm__("FUN_L03_002d5208");
extern char *D_L03_0015FFD8_c __asm__("D_L03_0015FFD8") __attribute__((sda));
extern char D_L03_00166EC0[];
extern f32 approach_value(f32 target, f32 step, f32 *value) __asm__("FUN_00213ed8");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") __attribute__((section(".sdata")));
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int D_0014C050; /* no foreign declaration */
extern int D_0015ED68_e[] __asm__("D_0015ED68") __attribute__((section(".sdata")));
extern float blarg_helicopter_bob_period __asm__("D_L03_00161994") __attribute__((sda)); /* no foreign declaration */
extern float blarg_helicopter_bob_amplitude __asm__("D_L03_00161998") __attribute__((sda)); /* no foreign declaration */
#include "rnc/overlay/shared/gameplay/entities/blarg_helicopter_globals.h"
extern float blarg_helicopter_path_min_speed_gp __asm__("D_L03_0016199C") __attribute__((sda));
extern int transpose_rotation_matrix() __asm__("FUN_001fa2d8");
extern int FUN_L00_001fff28();
extern int FUN_L00_00258110();
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int *arg0) __asm__("FUN_001f9740");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_L00_001ff290(void *, void *, void *);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025a120(void *);
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_002628d8_c(char *, char *, int, float, float) __asm__("FUN_L00_002628d8");
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void RaiseKernelTrap();
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void advance_resident_object_animation(void *) __asm__("FUN_0020d580");
extern void attach_manipulator_c(int, int, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void detach_manipulator(int, void *) __asm__("func_0020CB88");
extern void mark_moby_for_removal_c2(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
float absolute_float(float input) __asm__("func_001F99C0");
s32 FUN_L00_0025df68(s32 *path, void *point, void *nearest, f32 *fraction, s32 *segment, s32 wrap, f32 limit, f32 step, f32 height_tolerance);
void record_moby_hit(unsigned char *m, void *src) __asm__("FUN_L00_00259a88");
void manipulator_set_scale(char *p, float x) __asm__("FUN_L00_00263b38");
void handle_blarg_helicopter_hits(char *m) __asm__("FUN_L03_002ce238");
extern int D_L03_001BA5D0[] __asm__("D_L03_001BA5D0");
extern HPath49_2CDDC8 *D_L03_001B08B0_2CDDC8[] __asm__("D_L03_001B05B0");
extern unsigned char D_0014171B_2CDDC8[] __asm__("D_0014C050");
extern int FUN_L03_0024e830_c(void*,void*,float) __asm__("FUN_L03_0024e830");
extern void func_L00_00264BB0_2CDDC8(void*,float) __asm__("FUN_L00_00263b38");
extern void func_L00_00263950_2CDDC8(char*,char*,int,float,float) __asm__("FUN_L00_002628d8");
extern void handle_blarg_helicopter_hits(char*) __asm__("FUN_L03_002ce238");
extern void func_0020D960_2CDDC8(char*,int,void*) __asm__("FUN_0020cb10");
extern void func_00213DE0_2CDDC8(void*,int,int,int) __asm__("FUN_00212f90");
extern int func_L00_0025EFC0_2CDDC8(void*,void*,void*,int*,float*,int,float,float,float) __asm__("FUN_L00_0025df68");
extern float func_L00_00259148_2CDDC8(float*,float,float,float,float,float) __asm__("FUN_L00_00258110");
extern void func_001FA4A0_2CDDC8(void*,void*) __asm__("FUN_001fa2d8");
extern void func_001F9EE8_2CDDC8(void*,void*,void*) __asm__("FUN_001f9d20");
extern void func_L00_001FFED8_2CDDC8(void*,int,float) __asm__("FUN_L00_001fff28");
extern float func_00214D28_2CDDC8(float*,float,float) __asm__("FUN_00213ed8");
extern float func_001F9CE8_2CDDC8(void*) __asm__("FUN_001f9b20");
extern float func_001F9B88_2CDDC8(float) __asm__("FUN_001f99c0");
extern float func_001F9878_2CDDC8(float) __asm__("FUN_001f96b0");
extern float func_L00_001FF860_2CDDC8(float,float) __asm__("FUN_001f9e90");
extern float func_001FA790_2CDDC8(float,float) __asm__("FUN_001fa5c8");
extern void func_L00_001FF500_2CDDC8(void*,void*,float) __asm__("FUN_001f9c48");
extern void func_L00_0028EBF0_2CDDC8(int) __asm__("FUN_0022d798");
extern void func_001F9978_2CDDC8(void) __asm__("FUN_001f97b0");
extern float func_00214158_2CDDC8(void) __asm__("FUN_00213308");
extern char *spawn_homing_spark_projectile(char*,char*,int,int,float) __asm__("FUN_L03_002d5208");
extern void func_L00_0025AAC0_2CDDC8(void*,void*) __asm__("FUN_L00_00259a88");
extern void func_0020D9D8_2CDDC8(void*,void*) __asm__("FUN_0020cb88");
extern void func_L00_002514B8_2CDDC8(void*) __asm__("FUN_0020d580");
extern void func_0020D678_2CDDC8(void *) __asm__("FUN_0020c828");
extern int func_001F9850_2CDDC8(int) __asm__("FUN_001f96f8");
extern int func_001F9908_2CDDC8(void *) __asm__("FUN_001f9740");
extern int func_001FA898_2CDDC8(float) __asm__("FUN_001fa6d0");
extern float func_001F9D10_2CDDC8(void *, void *) __asm__("FUN_001f9b48");
extern int func_00215570_2CDDC8(void *, int) __asm__("FUN_00214720");
extern char D_0013E633_2CDDC8[] __asm__("D_0013E533");
extern float func_001FA888_2CDDC8(int) __asm__("FUN_001fa6c0");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_normalize(void *,void *,float) __asm__("FUN_001f9bf8");
extern float func_001F9F90_2CDDC8(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2CDDC8(float) __asm__("FUN_001f9de0");
extern void func_L00_00250800_2CDDC8(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float func_001F9D48_2CDDC8(void *, void *) __asm__("FUN_001f9b80");
extern int func_001F9938_2CDDC8(void *) __asm__("FUN_001f9770");
extern float func_001FA748_2CDDC8(float, float) __asm__("FUN_001fa580");
extern void func_L00_001FF240_2CDDC8(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void func_L00_0025F4A8_2CDDC8(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void func_L00_002584A8_2CDDC8(void *, int, int) __asm__("FUN_L00_00257470");
extern void func_L00_00265050_2CDDC8(void *, int, void *, void *, int, int, float, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern char D_L03_00166EC0[];
extern void func_L00_0025B178_2CDDC8(void *) __asm__("FUN_L00_0025a120");
extern void func_L00_00251E30_2CDDC8(void *) __asm__("FUN_L00_00250df8");

void blarg_helicopter_update(HMob49_2CDDC8 *moby) __asm__("FUN_L03_002cca00");
void blarg_helicopter_update(HMob49_2CDDC8 *moby) {
    enum {
        BLARG_HELICOPTER_INITIALIZE = 0,
        BLARG_HELICOPTER_ACTIVATION_PATH = 1,
        BLARG_HELICOPTER_PATROL_AND_ATTACK = 2,
        BLARG_HELICOPTER_EXIT = 3,
        BLARG_HELICOPTER_TIMED_REACTION = 4,
        BLARG_HELICOPTER_DESTROY = 5
    };
    HData49_2CDDC8 *data=moby->data;
    HWork49_2CDDC8 work;
    float target_range=40.0f;
    float motion_scale,motion_gain;
    int index;
    if (data->timer) target_range=80.0f;
    FUN_L03_0024e830_c(moby,&work.target,target_range);
    index=(short)moby->pool_index;
    if ((D_L03_001BA5D0[index>>5]>>(moby->pool_index&31))&1) {
        if (data->child_index!=-1) func_0020D678_2CDDC8((HMob49_2CDDC8*)(D_L03_0015FFD8_c+(data->child_index<<8)));
        func_0020D678_2CDDC8(moby); return;
    }
    if (data->route_a.path_id==-1 || (moby->state!=0 && data->enabled!=1)) {
        if (data->child_index!=-1) func_0020D678_2CDDC8((HMob49_2CDDC8*)(D_L03_0015FFD8_c+(data->child_index<<8)));
        func_0020D678_2CDDC8(moby); return;
    }
    if (data->request) { data->request=0; data->timer=func_001F9850_2CDDC8(240); }
    func_001F9908_2CDDC8(&data->timer);
    if (data->child_index!=-1) {
        HMob49_2CDDC8 *child=(HMob49_2CDDC8*)(D_L03_0015FFD8_c+(data->child_index<<8));
        if (child->class_id==0x23E && (signed char)child->state>=0) {
            char *gun=(char*)child->data+0x150;
            func_L00_00264BB0_2CDDC8(gun,2.5f);
            func_L00_00263950_2CDDC8((char*)child,gun,2,frame_scale_sq*0.03f,frame_scale_sq*0.3f);
        }
    }
    handle_blarg_helicopter_hits((char*)moby);
    switch (moby->state) {
    case BLARG_HELICOPTER_INITIALIZE: {
        HPath49_2CDDC8 **paths;
        moby->update_distance=0x20; data->health=3.0f; data->health_word=func_001FA898_2CDDC8(3.0f);
        data->enabled=1; data->kind=2; data->gun_kind=25; data->gun_enabled=1; data->gun_mode=6; data->flag29=0;
        paths=D_L03_001B08B0_2CDDC8;
        data->route_a.path=paths[data->route_a.path_id];
        if (data->route_b.path_id!=-1) data->route_b.path=paths[data->route_b.path_id];
        qcopy(moby->position,data->route_a.path->points);
        data->route_a.index=1; data->speed=0; data->pitch_velocity=0; data->roll_velocity=0; data->alternate=0;
        func_0020D960_2CDDC8((char*)moby,0,data->gun_yaw_matrix); func_0020D960_2CDDC8((char*)moby,1,data->gun_pitch_matrix);
        data->sound=-1; data->projectile=0; data->gun_yaw=moby->rotation[2]; data->gun_yaw_velocity=0;
        data->gun_pitch=moby->rotation[1]; data->gun_pitch_velocity=0;
        if (moby->brightness<128) moby->brightness=128;
        if (data->child_index!=-1) {
            HMob49_2CDDC8 *child=(HMob49_2CDDC8*)(D_L03_0015FFD8_c+(data->child_index<<8));
            func_00213DE0_2CDDC8(child,8,0,0); child->alpha=0; child->animation=0; child->flags&=0xFFBE;
        }
        if (D_0014171B_2CDDC8[moby->persistent+(D_0015ED68_e[7]<<4)]!=255 && data->activation_path!=-1 && data->activation_region!=-1) {
            HPath49_2CDDC8 *path=paths[data->activation_path];
            int i;
            for (i=0; i<path->count-2; i++)
                path->points[i].length=func_001F9D10_2CDDC8(&path->points[i],&path->points[i+1]);
            *(HQuad49_2CDDC8*)moby->position=*(HQuad49_2CDDC8*)path->points; moby->state=1; break;
        }
        moby->state=2;
        break;
    }
    case BLARG_HELICOPTER_ACTIVATION_PATH: {
        HPath49_2CDDC8 *path=D_L03_001B08B0_2CDDC8[data->activation_path];
        float progress,distance,step,limit,angle;
        if (data->activation_region!=-1 && !func_00215570_2CDDC8(D_0013F3D0,data->activation_region)) break;
        data->activation_region=-1;
        func_L00_0025EFC0_2CDDC8(path,moby->position,work.projected,&work.segment_a,&work.fraction_a,0,999.0f,5.0f,0.0f);
        progress=func_001FA888_2CDDC8(work.segment_a)*path->points[0].length+work.fraction_a+2.0f;
        distance=func_001F9D10_2CDDC8(moby->position,(char*)path+(path->count<<4));
        step=D_0015ED70*10.0f;
        if (distance<=data->speed*data->speed/(step+step)) {
            float speed=data->speed-step; limit=blarg_helicopter_path_min_speed_gp*D_0015ED6C; data->speed=speed; if (speed<limit) data->speed=limit;
        } else { float speed=data->speed+step; limit=D_0015ED6C*20.0f; data->speed=speed; if (limit<speed) data->speed=limit; }
        work.segment_a=func_001FA898_2CDDC8(progress/path->points[0].length);
        work.fraction_a=(progress-func_001FA888_2CDDC8(work.segment_a)*path->points[0].length)/path->points[0].length;
        if (work.segment_a>=path->count-1) {
            if (data->route_a.path_id!=-1 && data->speed<=blarg_helicopter_path_min_speed_gp*D_0015ED6C) {
                data->alternate=0; qcopy(moby->position,data->route_a.path->points); data->route_a.index=1;
                moby->previous_state=2; moby->state=4; data->reaction_timer=func_001F9850_2CDDC8(15); break;
            }
            if (data->speed<=blarg_helicopter_path_min_speed_gp*D_0015ED6C) { func_001F9978_2CDDC8(); return; }
            work.segment_a=path->count-2; work.fraction_a=1.0f;
        }
        fast_vec_sub(work.projected,&path->points[work.segment_a+1],&path->points[work.segment_a]);
        fast_vec_scale(work.projected,work.projected,work.fraction_a);
        fast_vec_add(work.projected,work.projected,path->points+work.segment_a);
        if (work.segment_a+1<path->count-8) {
            float *next=(float *)((char *)path+((work.segment_a+1)<<4));
            angle=func_L00_001FF860_2CDDC8(next[4]-moby->position[0],next[5]-moby->position[1]);
            moby->rotation[2]=func_L00_00259148_2CDDC8(&data->yaw_velocity,moby->rotation[2],angle,0.01f,0.3f,0.1f);
        } else {
            angle=func_L00_001FF860_2CDDC8(work.target.position[0]-moby->position[0],work.target.position[1]-moby->position[1]);
            moby->rotation[2]=func_L00_00259148_2CDDC8(&data->yaw_velocity,moby->rotation[2],angle,0.01f,0.3f,0.1f);
        }
        fast_vec_sub(work.movement,work.projected,moby->position);
        distance=fast_vec_length(work.movement); if (data->speed<distance) distance=data->speed;
        fast_vec_normalize(work.movement,work.movement,distance);
        motion_scale=20.0f; motion_gain=1.0471976f;
    {
        float delta,roll,pitch;
        fast_vec_add(moby->position,moby->position,work.movement);
        delta=func_001FA790_2CDDC8(func_L00_001FF860_2CDDC8(work.movement[0],work.movement[1]),moby->rotation[2]);
        roll=data->speed*0.34906584f*func_001F9F90_2CDDC8(delta)/(D_0015ED6C*motion_scale);
        pitch=data->speed*-0.34906584f*func_001F9FA8_2CDDC8(delta);
        motion_scale*=D_0015ED6C; pitch/=motion_scale;
        moby->rotation[1]=func_L00_00259148_2CDDC8(&data->roll_velocity,moby->rotation[1],roll,D_0015ED70*0.5235988f,D_0015ED70*motion_gain,D_0015ED6C*0.7853982f);
        moby->rotation[0]=func_L00_00259148_2CDDC8(&data->pitch_velocity,moby->rotation[0],pitch,D_0015ED70*motion_gain,D_0015ED70*2.0943952f,D_0015ED6C*1.5707964f);
    }
        break;
    }
    case BLARG_HELICOPTER_PATROL_AND_ATTACK: {
        HRoute49_2CDDC8 *route=data->alternate ? &data->route_b : &data->route_a;
        int point=route->index;
        float angle=func_L00_001FF860_2CDDC8(work.target.position[0]-moby->position[0],work.target.position[1]-moby->position[1]);
        HMob49_2CDDC8 *projectile;
        float distance,other_distance;
        moby->rotation[2]=func_L00_00259148_2CDDC8(&data->yaw_velocity,moby->rotation[2],angle,0.01f,0.3f,0.1f);
        projectile=data->projectile;
        if (!projectile || projectile->class_id!=0x350 || projectile->state==254 || projectile->state==253) {
            func_L00_00250800_2CDDC8(moby,0,work.movement);
            fast_vec_sub(work.direction,work.target.position,work.movement);
            *(HQuad49_2CDDC8*)work.projected=*(HQuad49_2CDDC8*)work.direction; func_001FA4A0_2CDDC8(work.offset,moby->transform); func_001F9EE8_2CDDC8(work.projected,work.projected,work.offset);
            angle=func_L00_001FF860_2CDDC8(work.projected[0],work.projected[1]);
            data->gun_yaw=func_L00_00259148_2CDDC8(&data->gun_yaw_velocity,data->gun_yaw,angle,D_0015ED70*6.2831855f,D_0015ED70*12.566371f,D_0015ED6C*6.2831855f);
            angle=-func_L00_001FF860_2CDDC8(func_001F9CE8_2CDDC8(work.projected),work.projected[2]);
            data->gun_pitch=func_L00_00259148_2CDDC8(&data->gun_pitch_velocity,data->gun_pitch,angle,D_0015ED70*3.1415927f,D_0015ED70*6.2831855f,D_0015ED6C*1.5707964f);
            func_L00_001FFED8_2CDDC8(data->gun_yaw_matrix+4,2,data->gun_yaw); func_L00_001FFED8_2CDDC8(data->gun_pitch_matrix+4,2,data->gun_pitch); data->projectile=0;
        } else {
            func_L00_00250800_2CDDC8(moby,0,work.movement);
            fast_vec_sub(work.direction,data->projectile->position,work.movement);
            *(HQuad49_2CDDC8*)work.projected=*(HQuad49_2CDDC8*)work.direction; func_001FA4A0_2CDDC8(work.offset,moby->transform); func_001F9EE8_2CDDC8(work.projected,work.projected,work.offset);
            angle=func_L00_001FF860_2CDDC8(work.projected[0],work.projected[1]);
            data->gun_yaw=func_L00_00259148_2CDDC8(&data->gun_yaw_velocity,data->gun_yaw,angle,D_0015ED70*6.2831855f,D_0015ED70*12.566371f,D_0015ED6C*6.2831855f);
            angle=-func_L00_001FF860_2CDDC8(func_001F9CE8_2CDDC8(work.projected),work.projected[2]);
            data->gun_pitch=func_L00_00259148_2CDDC8(&data->gun_pitch_velocity,data->gun_pitch,angle,D_0015ED70*3.1415927f,D_0015ED70*6.2831855f,D_0015ED6C*1.5707964f);
            func_L00_001FFED8_2CDDC8(data->gun_yaw_matrix+4,2,data->gun_yaw); func_L00_001FFED8_2CDDC8(data->gun_pitch_matrix+4,2,data->gun_pitch);
        }
        if (point>0 && point<route->path->count-1) {
            float node_distance=func_001F9D48_2CDDC8(&route->path->points[point],work.target.position);
            if (node_distance<20.0f && func_001F9B88_2CDDC8(work.target.position[2]-moby->position[2])<10.0f && node_distance<func_001F9D10_2CDDC8(&route->path->points[point+1],work.target.position) && point<route->path->count-2) route->index++;
        }
        fast_vec_sub(work.direction,&route->path->points[point],moby->position);
        distance=fast_vec_length(work.direction);
        if (distance<data->speed*data->speed/((D_0015ED70*4.0f)*2.0f) || func_001F9D10_2CDDC8(&route->path->points[point],work.target.position)<func_001F9D10_2CDDC8(moby->position,work.target.position)) {
            data->speed-=D_0015ED70*4.0f; if (data->speed<0) data->speed=0;
        } else { data->speed+=D_0015ED70*4.0f; if (D_0015ED6C*10.0f<data->speed) data->speed=D_0015ED6C*10.0f; }
        distance=func_001F9D10_2CDDC8(&route->path->points[point],work.target.position); other_distance=func_001F9D10_2CDDC8(moby->position,work.target.position);
        if (distance<other_distance) fast_vec_normalize(work.direction,work.direction,-data->speed); else fast_vec_normalize(work.direction,work.direction,data->speed);
        motion_scale=10.0f; motion_gain=1.0471976f;
    {
        float delta,roll,pitch;
        fast_vec_add(moby->position,moby->position,work.direction);
        delta=func_001FA790_2CDDC8(func_L00_001FF860_2CDDC8(work.direction[0],work.direction[1]),moby->rotation[2]);
        roll=data->speed*0.34906584f*func_001F9F90_2CDDC8(delta)/(D_0015ED6C*motion_scale);
        pitch=data->speed*-0.34906584f*func_001F9FA8_2CDDC8(delta);
        motion_scale*=D_0015ED6C; pitch/=motion_scale;
        moby->rotation[1]=func_L00_00259148_2CDDC8(&data->roll_velocity,moby->rotation[1],roll,D_0015ED70*0.5235988f,D_0015ED70*motion_gain,D_0015ED6C*0.7853982f);
        moby->rotation[0]=func_L00_00259148_2CDDC8(&data->pitch_velocity,moby->rotation[0],pitch,D_0015ED70*motion_gain,D_0015ED70*2.0943952f,D_0015ED6C*1.5707964f);
    }
        if (func_00215570_2CDDC8(work.target.position,data->alternate ? data->other_switch_region : data->switch_region)) {
            if (data->alternate ? data->other_exit_path!=-1 : data->exit_path!=-1) {
                HPath49_2CDDC8 *path=D_L03_001B08B0_2CDDC8[data->alternate ? data->other_exit_path : data->exit_path]; int i;
                for (i=0; i<path->count-2; i++)
                    path->points[i].length=func_001F9D10_2CDDC8(&path->points[i],&path->points[i+1]);
            }
            moby->previous_state=3; moby->state=4; data->reaction_timer=func_001F9850_2CDDC8(15); break;
        } else if (func_00215570_2CDDC8(work.target.position,data->shoot_region) && (func_001F9938_2CDDC8(&data->shot_timer) || (!data->projectile && data->timer))) {
            data->shot_timer=func_001F9850_2CDDC8(360); qcopy(work.shot_origin,work.target.position);
            fast_vec_sub(work.shot_velocity,work.shot_origin,work.movement); func_L00_001FF500_2CDDC8(work.shot_velocity,work.shot_velocity,D_0015ED6C*6.0f); work.shot_velocity[2]=0;
            data->projectile=(HMob49_2CDDC8*)spawn_homing_spark_projectile((char*)work.shot_velocity,(char*)work.movement,(int)moby,func_001F9850_2CDDC8(240),work.target.position[2]);
        }
        break;
    }
    case BLARG_HELICOPTER_EXIT: {
        HPath49_2CDDC8 *path;
        float progress,distance,step,limit,angle;
        if (data->exit_path==-1) {
            func_00214D28_2CDDC8(&moby->position[2],200.0f,D_0015ED6C*8.0f);
            moby->rotation[1]=func_L00_00259148_2CDDC8(&data->roll_velocity,moby->rotation[1],0.0f,D_0015ED70*0.5235988f,D_0015ED70*1.0471976f,D_0015ED6C*0.7853982f);
            moby->rotation[0]=func_L00_00259148_2CDDC8(&data->pitch_velocity,moby->rotation[0],0.0f,D_0015ED70*0.5235988f,D_0015ED70*1.0471976f,D_0015ED6C*0.7853982f); break;
        }
        path=D_L03_001B08B0_2CDDC8[data->alternate ? data->other_exit_path : data->exit_path];
        func_L00_0025EFC0_2CDDC8(path,moby->position,work.projected,&work.segment_b,&work.fraction_b,0,999.0f,5.0f,0.0f);
        progress=func_001FA888_2CDDC8(work.segment_b)*path->points[0].length+work.fraction_b+2.0f;
        distance=func_001F9D10_2CDDC8(moby->position,(char*)path+(path->count<<4)); step=D_0015ED70*10.0f;
        if (distance<=data->speed*data->speed/(step+step)) {
            float speed=data->speed-step; limit=blarg_helicopter_path_min_speed_gp*D_0015ED6C; data->speed=speed; if (speed<limit) data->speed=limit;
        } else { float speed=data->speed+step; limit=D_0015ED6C*20.0f; data->speed=speed; if (limit<speed) data->speed=limit; }
        work.segment_b=func_001FA898_2CDDC8(progress/path->points[0].length);
        work.fraction_b=(progress-func_001FA888_2CDDC8(work.segment_b)*path->points[0].length)/path->points[0].length;
        if (work.segment_b>=path->count-1) {
            if (data->other_exit_path!=-1 && data->route_b.path_id!=-1 && data->speed<=blarg_helicopter_path_min_speed_gp*D_0015ED6C) {
                HRoute49_2CDDC8 *route; data->alternate^=1; route=data->alternate ? &data->route_b : &data->route_a;
                qcopy(moby->position,route->path->points); route->index=1;
                moby->previous_state=2; moby->state=4; data->reaction_timer=func_001F9850_2CDDC8(15); break;
            }
            if (data->speed<=blarg_helicopter_path_min_speed_gp*D_0015ED6C) { func_001F9978_2CDDC8(); return; }
            work.segment_b=path->count-2; work.fraction_b=1.0f;
        }
        fast_vec_sub(work.projected,&path->points[work.segment_b+1],&path->points[work.segment_b]);
        fast_vec_scale(work.projected,work.projected,work.fraction_b); fast_vec_add(work.projected,work.projected,path->points+work.segment_b);
        angle=func_L00_001FF860_2CDDC8(work.target.position[0]-moby->position[0],work.target.position[1]-moby->position[1]);
        moby->rotation[2]=func_L00_00259148_2CDDC8(&data->yaw_velocity,moby->rotation[2],angle,0.01f,0.3f,0.1f);
        fast_vec_sub(work.movement,work.projected,moby->position); distance=fast_vec_length(work.movement);
        if (data->speed<distance) distance=data->speed;
        fast_vec_normalize(work.movement,work.movement,distance); motion_scale=20.0f; motion_gain=1.0471976f;
    {
        float delta,roll,pitch;
        fast_vec_add(moby->position,moby->position,work.movement);
        delta=func_001FA790_2CDDC8(func_L00_001FF860_2CDDC8(work.movement[0],work.movement[1]),moby->rotation[2]);
        roll=data->speed*0.34906584f*func_001F9F90_2CDDC8(delta)/(D_0015ED6C*motion_scale);
        pitch=data->speed*-0.34906584f*func_001F9FA8_2CDDC8(delta);
        motion_scale*=D_0015ED6C; pitch/=motion_scale;
        moby->rotation[1]=func_L00_00259148_2CDDC8(&data->roll_velocity,moby->rotation[1],roll,D_0015ED70*0.5235988f,D_0015ED70*motion_gain,D_0015ED6C*0.7853982f);
        moby->rotation[0]=func_L00_00259148_2CDDC8(&data->pitch_velocity,moby->rotation[0],pitch,D_0015ED70*motion_gain,D_0015ED70*2.0943952f,D_0015ED6C*1.5707964f);
    }
        break;
    }
    case BLARG_HELICOPTER_TIMED_REACTION:
        if (func_001F9938_2CDDC8(&data->reaction_timer)) moby->state=moby->previous_state;
        else {
            data->speed-=D_0015ED70*4.0f; if (data->speed<0) data->speed=0;
            motion_scale=10.0f; motion_gain=1.0471976f;
            work.movement[0]=func_001F9F90_2CDDC8(moby->rotation[2])*-data->speed;
            work.movement[1]=func_001F9FA8_2CDDC8(moby->rotation[2])*-data->speed; work.movement[2]=0;
    {
        float delta,roll,pitch;
        fast_vec_add(moby->position,moby->position,work.movement);
        delta=func_001FA790_2CDDC8(func_L00_001FF860_2CDDC8(work.movement[0],work.movement[1]),moby->rotation[2]);
        roll=data->speed*0.34906584f*func_001F9F90_2CDDC8(delta)/(D_0015ED6C*motion_scale);
        pitch=data->speed*-0.34906584f*func_001F9FA8_2CDDC8(delta);
        motion_scale*=D_0015ED6C; pitch/=motion_scale;
        moby->rotation[1]=func_L00_00259148_2CDDC8(&data->roll_velocity,moby->rotation[1],roll,D_0015ED70*0.5235988f,D_0015ED70*motion_gain,D_0015ED6C*0.7853982f);
        moby->rotation[0]=func_L00_00259148_2CDDC8(&data->pitch_velocity,moby->rotation[0],pitch,D_0015ED70*motion_gain,D_0015ED70*2.0943952f,D_0015ED6C*1.5707964f);
    }
        
        }
        break;
    case BLARG_HELICOPTER_DESTROY: {
        float angle; int i; 
        *(HQuad49_2CDDC8*)work.movement=0; angle=func_00214158_2CDDC8(); func_L00_00250800_2CDDC8(moby,4,work.projected);
        for (i=0; i<4; i++) {
            work.offset[0]=func_001F9F90_2CDDC8(angle)*1.5f; work.offset[1]=func_001F9FA8_2CDDC8(angle)*1.5f;
            work.offset[2]=0; angle=func_001FA748_2CDDC8(angle,1.5707964f); func_L00_001FF240_2CDDC8(work.hit.direction,work.offset,work.projected);
        }
        func_L00_00250800_2CDDC8(moby,5,work.offset);
        if (data->child_index!=-1) {
            HMob49_2CDDC8 *child=(HMob49_2CDDC8*)(D_L03_0015FFD8_c+(data->child_index<<8));
            ((unsigned char*)child->data)[0x262]=2; child->alpha=255; child->flags&=0xFFF9;
            work.hit.radius=20.0f; work.hit.flags=0x830000; work.hit.allow=1; work.hit.source=moby;
            fast_vec_sub(work.hit.direction,child->position,moby->position); func_L00_001FF500_2CDDC8(work.hit.direction,work.hit.direction,1.0f);
            work.hit.direction[2]=1.0f; work.hit.direction[3]=5627.925f; work.hit.kind=3; work.hit.enabled=3; work.hit.class_id=moby->class_id;
            func_L00_0025AAC0_2CDDC8(child,&work.hit);
        }
        func_L00_0025F4A8_2CDDC8(moby,work.movement,moby->position,0.0f,0.0f,20,3,4,4.0f,2.0f,100000.0f,3.0f,2,15.0f,1,1,-1,0);
        func_L00_002584A8_2CDDC8(moby,0,-1);
        func_L00_00265050_2CDDC8(moby,0x637,moby->position,moby->rotation,0,0,0.0f,D_L03_0015F580_macro,D_L03_0015F580_macro,D_L03_0015F580_macro);
        func_L00_00265050_2CDDC8(moby,0x638,moby->position,moby->rotation,0,0,0.0f,D_L03_0015F580_macro,D_L03_0015F580_macro,D_L03_0015F580_macro);
        func_L00_00265050_2CDDC8(moby,0x639,moby->position,moby->rotation,0,0,0.0f,D_L03_0015F580_macro,D_L03_0015F580_macro,D_L03_0015F580_macro);
        func_0020D9D8_2CDDC8(moby,data->gun_yaw_matrix); func_0020D9D8_2CDDC8(moby,data->gun_pitch_matrix);
        if (data->sound!=-1) {
            unsigned char *audio=(unsigned char*)(D_0013E633_2CDDC8+0x1D+data->sound*0x70);
            if (*(HMob49_2CDDC8**)(audio+0x88)==moby && audio[0x74]) func_L00_0028EBF0_2CDDC8(data->sound);
        }
        data->sound=-1; func_0020D678_2CDDC8(moby); return;
    }
    default: break;
    }
    if (moby->nearby && func_001F9D10_2CDDC8(moby->position,D_L03_00166EC0)<40.0f) func_L00_0025B178_2CDDC8(moby);
    moby->position[2]-=blarg_helicopter_bob_amplitude*func_001F9F90_2CDDC8(data->bob);
    data->bob=func_001FA748_2CDDC8(data->bob,6.2831855f/func_001F9878_2CDDC8(blarg_helicopter_bob_period*60.0f));
    moby->position[2]+=blarg_helicopter_bob_amplitude*func_001F9F90_2CDDC8(data->bob);
    if (data->child_index!=-1) {
        HMob49_2CDDC8 *child=(HMob49_2CDDC8*)(D_L03_0015FFD8_c+(data->child_index<<8));
        *(HQuad49_2CDDC8*)child->position=*(HQuad49_2CDDC8*)moby->position; *(HQuad49_2CDDC8*)child->rotation=*(HQuad49_2CDDC8*)moby->rotation;
        func_L00_002514B8_2CDDC8(child); func_L00_00251E30_2CDDC8(child); child->flags|=6;
    }
}
/* Moby update: advance a timer from a collision query, switch state, aim head angles. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002CF600), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern int hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
float absolute_float(float input) __asm__("func_001F99C0");

void handle_blarg_helicopter_hits(char *helicopter_moby) {
    int hit_reaction_status;
    float fv;
    int hit_reaction_result;
    char *helicopter_data;
    char *hit_record;
    float reaction_heading_angle, reaction_relative_heading_angle, reaction_roll_angle, reaction_pitch_angle;
    helicopter_data = *(char **)(helicopter_moby + 0x78);
    fv = 0.0f;
    hit_record = find_moby_hit(helicopter_moby, 0x330000, 0);
    hit_reaction_result = hit_reaction(helicopter_moby, hit_record, helicopter_data + 0x20, 0, &hit_reaction_status, &fv, 0, 4);
    if (hit_reaction_status != 1 && *(unsigned char *)(helicopter_moby + 0x20) != 5) {
        float remaining_health = *(float *)(helicopter_data + 0x20) - fv;
        *(float *)(helicopter_data + 0x20) = remaining_health;
        if (hit_reaction_result == 1 || remaining_health <= 0.0f) {
            *(unsigned char *)(helicopter_moby + 0x20) = 5;
        } else if (hit_reaction_result != 0xB) {
            *(unsigned char *)(helicopter_data + 0x67) = 0xB4;
            trooper_anim_update(helicopter_moby, (short *)(helicopter_data + 0x60));
            if (*(float *)(hit_record + 0x1C) == 5627.925f && *(unsigned char *)(helicopter_moby + 0x20) != 4) {
                reaction_heading_angle = fast_arc_tan(*(float *)(helicopter_moby + 0x10) - *(float *)(hit_record + 0),
                                 *(float *)(helicopter_moby + 0x14) - *(float *)(hit_record + 4));
                reaction_relative_heading_angle = fast_subtract_rotations(reaction_heading_angle, *(float *)(helicopter_moby + 0x48));
                reaction_roll_angle = fast_cos(reaction_relative_heading_angle) * 0.08726646f;
                reaction_pitch_angle = fast_sin(reaction_relative_heading_angle) * -0.34906584f;
                if (absolute_float(*(float *)(helicopter_moby + 0x40)) < absolute_float(reaction_pitch_angle)) {
                    *(float *)(helicopter_moby + 0x40) = reaction_pitch_angle;
                    *(float *)(helicopter_data + 0x218) = 0.0f;
                }
                if (absolute_float(*(float *)(helicopter_moby + 0x44)) < absolute_float(reaction_roll_angle)) {
                    *(float *)(helicopter_moby + 0x44) = reaction_roll_angle;
                    *(float *)(helicopter_data + 0x21C) = 0.0f;
                }
                *(float *)(helicopter_data + 0x214) = frame_time * 4.0f;
                *(short *)(helicopter_data + 0x206) = scale_game_frames(0x3C);
                *(unsigned char *)(helicopter_moby + 0xBC) = *(unsigned char *)(helicopter_moby + 0x20);
                *(unsigned char *)(helicopter_moby + 0x20) = 4;
                allocate_voice_for_target_entry(1, 0, (int)helicopter_moby);
            }
        }
    }
    *(unsigned char *)(helicopter_moby + 0xA4) = 0xFF;
    trooper_anim_finish(helicopter_moby, helicopter_data + 0x60);
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_0031BDB8.c: func_L07_00320B50), where it is exact; names translated to the US level program. */

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

extern float D_L03_0015F580[] MACRO_ADDR;
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void spawn_proximity_gated_bolts(void *) __asm__("FUN_L01_002787a0");
void mark_moby_for_removal_c(struct Obj *obj) __asm__("FUN_0020c828");

void statue_update(unsigned char *m) __asm__("FUN_L03_002d3cd8");
void statue_update(unsigned char *statue_moby) {
    enum {
        STATUE_INITIALIZE = 0,
        STATUE_WAITING_FOR_HIT = 1,
        STATUE_BREAK_APART = 2
    };
    int flag = 0;
    float z;
    char *zero_vector, *statue_position, *statue_rotation;
    char *x = find_moby_hit(statue_moby, 0x10000, 0);
    switch (statue_moby[0x20]) {
    case STATUE_INITIALIZE:
        statue_moby[0x20] = 1;
        break;
    case STATUE_WAITING_FOR_HIT:
        if (x != 0) {
            if (*(float *)(x + 0x2C) > 0.0f)
                flag = 1;
        }
        if (flag)
            statue_moby[0x20] = 2;
        break;
    case STATUE_BREAK_APART:
        allocate_voice_for_target_entry(0, 0, (int)statue_moby);
        z = 0.0f;
        zero_vector = D_L03_0015F580;
        statue_position = statue_moby + 0x10;
        spawn_proximity_gated_bolts(statue_moby);
        statue_rotation = statue_moby + 0x40;
        FUN_L00_00263fd8(statue_moby, 0x727, statue_position, statue_rotation, 0, 0, z, zero_vector, zero_vector, zero_vector);
        FUN_L00_00263fd8(statue_moby, 0x728, statue_position, statue_rotation, 0, 0, z, zero_vector, zero_vector, zero_vector);
        FUN_L00_00263fd8(statue_moby, 0x729, statue_position, statue_rotation, 0, 0, z, zero_vector, zero_vector, zero_vector);
        mark_moby_for_removal_c(statue_moby);
        break;
    }
}
/* Update of moby class 848: a spark that homes on its target moby, sheds particles and an arc when close, and dies on a hit. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002D5B88), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef int u128_2D5B88 __attribute__((mode(TI)));

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec_2D5B88;

typedef struct {
    Vec_2D5B88 vel;
    Moby *target;
    int timer;
    int flip;
} Data_2D5B88;

typedef struct {
    Vec_2D5B88 dir;
    void *owner;
    int flags;
    unsigned char b18;
    unsigned char b19;
    unsigned short h1A;
    float f1C;
    int i20;
} Hit_2D5B88;

typedef struct {
    Vec_2D5B88 pos;
    Vec_2D5B88 v10;
    Vec_2D5B88 v20;
    Vec_2D5B88 v30;
    void *moby;
} Tgt_2D5B88;

extern Moby *D_L03_00173F58 __attribute__((section(".data")));
extern char *FUN_L00_0026dd20(void *pos, void *vel, int c, int d, float f);
extern char D_0013E533[];
extern CollisionHit coll_hit __asm__("D_L03_00173F60");
extern char D_L03_00173F80[];
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos_c2(f32) __asm__("func_001F9DC8");
extern f32 fast_sin_c2(f32) __asm__("func_001F9DE0");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C_c __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float homing_spark_turn_rate_degrees_gp __asm__("D_L03_00161B30") __attribute__((sda));
extern float step_angle_toward_target(float *, int, float, float) __asm__("FUN_L00_0025bc00");
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern int collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int collect_mobies_in_sphere(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_00259888(void *, void *, int, float, void *);
extern void select_moby_target_with_hero_fallback(void *, void *, float) __asm__("FUN_L03_0024e830");
extern void create_type_31_part(void *, void *, void *, void *, int) __asm__("FUN_L03_00260130");
extern void mark_moby_for_removal_c_c(void *) __asm__("func_0020C828");
extern char D_0013E550[];

void update_homing_spark_projectile(Moby *moby) __asm__("FUN_L03_002d47c0");

void update_homing_spark_projectile(Moby *moby) {
    enum {
        SPARK_HOME_AND_EMIT = 1,
        SPARK_CLEANUP_AND_REMOVE = 2
    };
    Tgt_2D5B88 target_info;
    Vec_2D5B88 start_pos;
    Hit_2D5B88 hit;
    Vec_2D5B88 direction;
    Vec_2D5B88 muzzle;
    Vec_2D5B88 aim;
    Vec_2D5B88 ray_end;
    Vec_2D5B88 side_offset;
    Vec_2D5B88 gun_pos;
    Vec_2D5B88 spark_vel;
    Vec_2D5B88 spark_dir;
    Vec_2D5B88 tracer_vel;
    Vec_2D5B88 tracer_offset;
    float heading;
    int *voice_slot = 0;
    Data_2D5B88 *data = (Data_2D5B88 *)((char *)moby->pvars);
    int state;
    int i;
    int j;
    float max_climb;
    float target_distance;

    select_moby_target_with_hero_fallback(moby, &target_info, 20.0f);
    if (data->target->oclass == 0x277) {
        voice_slot = (int *)(((char *)data->target->pvars) + 0x250);
    } else if (data->target->oclass == 0xE9) {
        voice_slot = (int *)(((char *)data->target->pvars) + 0x1B8);
        if (data->target->state == 0x40 || data->target->state == 1) {
            moby->state = 2;
        }
    }
    state = moby->state;
    switch (state) {
    case SPARK_HOME_AND_EMIT:
        set_vector_length_xy(&direction, data, 1.0f);
        data->vel.z = 1.0f;
        FUN_L00_00259888(&hit, moby, 0x10003, 2.0f, &direction);
        set_vector_length_xy(&hit, &hit, 1.0f);
        hit.dir.z = 1.0f;
        hit.dir.w = 5627.9248f;
        hit.b18 = state;
        hit.b19 = state;
        hit.h1A = moby->oclass;
        qcopy(&start_pos, &moby->pos);
        data->vel.z = -(moby->pos.z - data->vel.w) / 10.0f;
        max_climb = D_0015ED6C_c * 8.0f;
        if (data->vel.z > max_climb) {
            data->vel.z = max_climb;
        } else if (data->vel.z < -max_climb) {
            data->vel.z = -max_climb;
        }
        heading = fast_arc_tan(data->vel.x, data->vel.y);
        if (fast_difference_between_rotations_c(heading, fast_arc_tan(target_info.pos.x - moby->pos.x, target_info.pos.y - moby->pos.y)) < 1.5707964f) {
            step_angle_toward_target(&heading, 0, fast_arc_tan(target_info.pos.x - moby->pos.x, target_info.pos.y - moby->pos.y), homing_spark_turn_rate_degrees_gp * 0.017453292f * D_0015ED6C_c);
            data->vel.x = fast_cos_c2(heading) * vector_length_xy(data);
            data->vel.y = fast_sin_c2(heading) * vector_length_xy(data);
        }
        add_vector_xyz(&moby->pos, &moby->pos, data);
        target_distance = vector_distance(&moby->pos, &data->target->pos);
        if (voice_slot != 0) {
            int voice = *voice_slot;
            if (data->target->pos.z - target_info.pos.z < target_distance) {
                if (voice_is_playing(data->target, voice) == 0) {
                    *voice_slot = allocate_voice_for_target_entry_alt(0, 4, (int)data->target);
                }
            } else {
                if (voice != -1) {
                    char *entry = ((char *)&D_0013E550) + voice * 0x70;
                    if (*(Moby **)(entry + 0x88) == data->target && *(unsigned char *)(entry + 0x74) != 0) {
                        release_voice_slot(voice);
                    }
                }
                *voice_slot = -1;
            }
        }
        if (data->target->pos.z - target_info.pos.z < target_distance) {
            if (data->timer % scale_game_frames_alt(6) == 0) {
                data->flip ^= 1;
                side_offset.x = fast_cos_c2(fast_add_rotations_c2(heading, data->flip ? 1.5707964f : -1.5707964f)) * 0.25f;
                side_offset.y = fast_sin_c2(fast_add_rotations_c2(heading, data->flip ? 1.5707964f : -1.5707964f)) * 0.25f;
                side_offset.z = 0.0f;
                add_vector_xyz(&gun_pos, &moby->pos, &side_offset);
                FUN_L00_0024f7c8(data->target, 3 - data->flip, &muzzle);
                subtract_vector_xyz(&aim, &gun_pos, &muzzle);
                scale_vector_xyz(&ray_end, &aim, 2.0f);
                add_vector_xyz(&ray_end, &ray_end, &muzzle);
                for (i = 0; i < 5; i++) {
                    *(u128_2D5B88 *)&spark_dir = 0;
                    spark_dir.x = random_float_between_c(-1.0f, 1.0f);
                    spark_dir.y = random_float_between_c(-1.0f, 1.0f);
                    spark_dir.z = random_float_between_c(-1.0f, 1.0f);
                    *(u128_2D5B88 *)&spark_vel = *(u128_2D5B88 *)&spark_dir;
                    normalize_vector_xyz(&spark_vel, &spark_vel, vector_length_xyz(&aim) * 0.65f);
                    add_vector_xyz(&spark_vel, &aim, &spark_vel);
                    normalize_vector_xyz(&spark_vel, &spark_vel, random_float_between_c(D_0015ED6C_c * 5.0f, D_0015ED6C_c * 15.0f));
                    FUN_L00_0026dd20(&muzzle, &spark_vel, 0x5F2F4F6F, random_int_between(scale_game_frames_alt(5), scale_game_frames_alt(10)), 50000.0f);
                }
                normalize_vector_xyz(&aim, &aim, D_0015ED6C_c * 0.5f);
                FUN_L00_0026dd20(&muzzle, &aim, 0x2F4F7F7F, random_int_between(scale_game_frames_alt(4), scale_game_frames_alt(7)), 400000.0f);
                scale_vector_xyz(&spark_dir, data, multiply_global_factor_ed64(6.0f));
                add_vector_xyz(&spark_dir, &spark_dir, &moby->pos);
                subtract_vector_xyz(&spark_dir, &spark_dir, &side_offset);
                subtract_vector_xyz(&tracer_vel, &spark_dir, &muzzle);
                normalize_vector_xyz(&spark_dir, &tracer_vel, 2.0f);
                subtract_vector_xyz(&tracer_offset, &muzzle, &spark_dir);
                *(u128_2D5B88 *)&spark_vel = *(u128_2D5B88 *)&tracer_offset;
                add_vector_xyz(&spark_dir, &spark_dir, &spark_vel);
                normalize_vector_xyz(&tracer_vel, &tracer_vel, D_0015ED6C_c * 100.0f);
                create_type_31_part(data->target, &spark_vel, &spark_dir, &tracer_vel, scale_game_frames_alt(0x2D));
                if (collision_line(&muzzle, &ray_end, 0, data->target, 0) != 0) {
                    qcopy(&muzzle, &coll_hit);
                    normalize_vector_xyz(&aim, &aim, D_0015ED6C_c * 10.0f);
                    for (j = 0; j < 10; j++) {
                        *(u128_2D5B88 *)&spark_dir = 0;
                        spark_dir.x = random_float_between_c(-1.0f, 1.0f);
                        spark_dir.y = random_float_between_c(-1.0f, 1.0f);
                        spark_dir.z = random_float_between_c(-1.0f, 1.0f);
                        *(u128_2D5B88 *)&spark_vel = *(u128_2D5B88 *)&spark_dir;
                        FUN_L00_001ff660(&spark_dir, &aim, D_L03_00173F80);
                        normalize_vector_xyz(&spark_vel, &spark_vel, vector_length_xyz(&spark_dir) * 0.5f);
                        add_vector_xyz(&spark_dir, &spark_dir, &spark_vel);
                        normalize_vector_xyz(&spark_dir, &spark_dir, random_float_between_c(D_0015ED6C_c * 3.0f, D_0015ED6C_c * 6.0f));
                        FUN_L00_0026dd20(&muzzle, &spark_dir, 0x7F2F4F6F, random_int_between(scale_game_frames_alt(0xF), scale_game_frames_alt(0x14)), 30000.0f);
                    }
                    normalize_vector_xyz(&aim, &aim, -1.0f);
                    add_vector_xyz(&ray_end, &muzzle, &aim);
                    if (collision_line(&muzzle, &ray_end, 0x10, data->target, &hit) != 0 && D_L03_00173F58 != 0
                        && (D_L03_00173F58 == ((Hero *)(D_0013E533 + 0xE1D))->moby || D_L03_00173F58->oclass == 0x1DF)) {
                        moby->state = 2;
                    }
                    qcopy(&ray_end, &muzzle);
                    ray_end.z += 2.0f;
                    if (collision_line(&muzzle, &ray_end, 0x10, data->target, &hit) != 0 && D_L03_00173F58 != 0
                        && (D_L03_00173F58 == ((Hero *)(D_0013E533 + 0xE1D))->moby || D_L03_00173F58->oclass == 0x1DF)) {
                        moby->state = 2;
                    }
                    if (collect_mobies_in_sphere(0.5f, &moby->pos, 0x10, moby, &hit) != 0 && D_L03_00173F58 != 0
                        && (D_L03_00173F58 == ((Hero *)(D_0013E533 + 0xE1D))->moby || D_L03_00173F58->oclass == 0x1DF)) {
                        moby->state = 2;
                    }
                }
            }
        }
        if (tick_countdown_32_alt(&data->timer) != 0 || data->target == 0 || data->target->state == 0xFE || data->target->state == 0xFD) {
            moby->state = 2;
        }
        break;
    case SPARK_CLEANUP_AND_REMOVE:
        if (voice_slot != 0) {
            int voice = *voice_slot;
            if (voice != -1) {
                char *entry = ((char *)&D_0013E550) + voice * 0x70;
                if (*(Moby **)(entry + 0x88) == data->target && *(unsigned char *)(entry + 0x74) != 0) {
                    release_voice_slot(voice);
                }
            }
            *voice_slot = -1;
        }
        mark_moby_for_removal_c_c(moby);
        break;
    }
}
/* Spawns a class 0x350 moby and fills its data block from a position, vector and parameters. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_002D65D0), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m5208(int) __asm__("FUN_0020c4f8");

char *spawn_homing_spark_projectile(char *velocity, char *spawn_position, int target_moby, int lifetime_frames, float target_height) {
    char *spawned_moby = (char *)func_0020D348_m5208(0x350);
    char *data;
    if (spawned_moby != 0) {
        *(unsigned char *)(spawned_moby + 0x30) = 0xFF;
        *(short *)(spawned_moby + 0x32) = 0x7E;
        *(unsigned char *)(spawned_moby + 0x31) = 1;
        *(unsigned char *)(spawned_moby + 0x20) = 1;
        data = *(char **)(spawned_moby + 0x78);
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(data, velocity);
        *(float *)(data + 0xC) = target_height;
        *(int *)(data + 0x14) = lifetime_frames;
        *(int *)(data + 0x10) = target_moby;
        *(int *)(data + 0x18) = 0;
    }
    return spawned_moby;
}
typedef struct {
    char pad0[0x44];
    short count;
    char pad46[0x132];
    char *mobys[1];
} ML;

extern ML D_L03_0016C960;
extern int D_L03_0015F5C4;
extern unsigned char D_0015EDB0;
extern unsigned char D_L03_0017C540[];
extern short D_L03_00161BC4 __attribute__((sda));
extern void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");

void attach_shared_manipulator_to_matching_class_moby(char *moby) __asm__("FUN_L03_002db480");

void attach_shared_manipulator_to_matching_class_moby(char *moby) {
    int i;

    if (D_L03_0015F5C4 == 2 && D_0015EDB0 != 0) {
        for (i = 0; i < D_L03_0016C960.count; i++) {
            char *candidate_moby = D_L03_0016C960.mobys[i];

            if (*(short *)(candidate_moby + 0xA6) == *(short *)(moby + 0xA6)) {
                unsigned char *manipulator = D_L03_0017C540;

                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 1, manipulator);
                    *(float *)(manipulator + 0x20) = *(float *)&D_L03_00161BC4;
                    *(float *)(manipulator + 0x24) = *(float *)&D_L03_00161BC4;
                    *(float *)(manipulator + 0x28) = *(float *)&D_L03_00161BC4;
                }
            }
        }
    }
}

extern float FUN_001f9b48(void *, void *);
extern float atan_2dc310(float, float) __asm__("FUN_001f9e90");
extern float rotdiff_2dc310(float, float) __asm__("FUN_001fa688");
extern void FUN_0020c828(void *);

void fred_1_update(char *m) __asm__("FUN_L03_002dc310");
void fred_1_update(char *moby) {
    float unused_stack_padding[12]; /* unused; sets the frame size */
    FUN_001f9b48(moby + 0x10, hero.motion.pos.f);
    rotdiff_2dc310(atan_2dc310(hero.motion.pos.f[0] - *(float *)(moby + 0x10),
                               hero.motion.pos.f[1] - *(float *)(moby + 0x14)),
                   *(float *)(moby + 0x48));
    FUN_0020c828(moby);
}

/* Level overlay code after gameplay/vendor; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Activate the watcher and steer its body and head toward the player. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E45F8), where it is exact; names translated to the US level program. */

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L03WatchScratch;

typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0xF];
    unsigned char opacity;
    unsigned char near_flag;
    char pad32[0x16];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x24];
    char *data;
    char pad7C[3];
    unsigned char flag7F;
} L03WatchMoby;

typedef struct {
    char pad00[0x64];
    float pitch;
    float yaw;
    char pad6C[4];
    float height;
    char pad74[0x74];
    float head_yaw;
    char padEC[0x14];
    float target[4];
    char pad110[8];
    int moving_timer;
    int target_timer;
    int region_a;
    int region_b;
    int region_c;
} L03WatchData;

typedef struct {
    char pad00[0xD0];
    float aim[4];
} L03Player;
extern unsigned char *D_L03_0015FFD8 __attribute__((sda));
extern char D_L03_00166EC0[];
extern char D_L03_001E3628[], D_L03_001E3660[], D_L03_001E3690[];
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern int FUN_001fa6d0(float);
extern float FUN_001f9b48(void *, void *);
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void debug_print(void *) __asm__("FUN_001e93b0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void FUN_0020c828(void *);
extern void FUN_L00_0025a120(void *);
extern void set_moby_dialogue_record_value(void *, int) __asm__("FUN_L00_00266858");
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");
extern void attach_cutscene_fixed_scale_manipulator(void *) __asm__("FUN_L01_002fb4b8");

/* Activate the watcher and steer its body and head toward the player. */
void qwark_rilgar_update(L03WatchMoby *m) __asm__("FUN_L03_002dc478");
void qwark_rilgar_update(L03WatchMoby *moby) {
    enum {
        QWARK_RILGAR_INITIALIZE = 0,
        QWARK_RILGAR_WAIT_FOR_PLAYER_IN_REGION = 1,
        QWARK_RILGAR_POST_REGION_TRIGGER = 2
    };
    L03WatchScratch scratch;
    L03WatchData *watch_data = (L03WatchData *)moby->data;
    float rate;
    float head_rate;
    int tracking;
    attach_cutscene_fixed_scale_manipulator(moby);
    if (moby->near_flag) {
        if (FUN_001f9b48(moby->position, D_L03_00166EC0) < 30.0f) {
            FUN_L00_0025a120(moby);
            moby->flag7F = 24;
        }
    }
    switch (moby->state) {
    case QWARK_RILGAR_INITIALIZE:
        moby->opacity = 0xFF;
        if (watch_data->region_a == -1)
            debug_print(D_L03_001E3628);
        if (watch_data->region_c == -1)
            debug_print(D_L03_001E3660);
        if (watch_data->region_b == -1) {
            debug_print(D_L03_001E3690);
        } else {
            unsigned char *moby_pool = D_L03_0015FFD8;
            if (moby_pool[watch_data->region_b * 256 + 0x20] != 0) {
                if (watch_data->region_c != -1)
                    moby_pool[watch_data->region_c * 256 + 0x20] = 3;
                FUN_0020c828(moby);
                return;
            }
        }
        moby->state = 1;
        break;
    case QWARK_RILGAR_WAIT_FOR_PLAYER_IN_REGION:
        if (watch_data->region_a != -1 && is_point_inside_clip_volume(&hero.motion.pos, watch_data->region_a)) {
            set_moby_dialogue_record_value(moby, 1);
            moby->state = 2;
        }
        break;
    case QWARK_RILGAR_POST_REGION_TRIGGER:
        moby->state = 3;
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *hero_position = (char *)&hero.motion.pos;
        L03Player *player_aim_view;
        tracking = 1;
        if (vector_distance(moby->position, hero_position) < 8.0f &&
            (player_aim_view = (L03Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 moby->yaw, fast_arc_tan(player_aim_view->aim[0] - moby->position[0], player_aim_view->aim[1] - moby->position[1]))) <
                1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                watch_data->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&watch_data->moving_timer);
        } else if (watch_data->moving_timer) {
            watch_data->moving_timer = 0;
            qcopy(watch_data->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&watch_data->target_timer)) {
            float heading;
            watch_data->target_timer = FUN_001fa6d0(multiply_global_factor_ed64(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(watch_data->target, 6.0f, heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(watch_data->target, watch_data->target, moby->position);
        }
        if (watch_data->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, watch_data->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        watch_data->pitch = pitch;
        watch_data->yaw = yaw * 0.6f;
        watch_data->head_yaw = yaw * 0.4f;
    }
    if (D_0015EDB0_b)
        watch_data->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, watch_data, 0);
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, moby, (char *)watch_data + 0x80, 1);
}
extern void prepare_scaled_float_table_with_target_byte(void *, int, float) __asm__("FUN_L02_002a4058");
extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern unsigned char D_L03_001DC1A0[];
extern void register_audio_stream_callback(void) __asm__("FUN_00215420");

void update_moby_draw_callback(unsigned char *moby) __asm__("FUN_L03_002dc9b0");

void update_moby_draw_callback(unsigned char *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_REGISTER = 1
    };
    switch (moby[0x20]) {
    case DRAW_CALLBACK_INITIALIZE:
        prepare_scaled_float_table_with_target_byte(D_L03_001DC1A0, 0x20, 0.6666667f);
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        break;
    case DRAW_CALLBACK_REGISTER:
        add_draw_callback(register_audio_stream_callback, moby);
        break;
    }
}

/* Level state record; byte i at 0x454 is set once moby unkB2 == i is collected. */
typedef struct {
    u8 pad[0x454];
    u8 collected[1];
} LevelCollected;

extern LevelCollected D_L03_001BB330;

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
extern s32 current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));
/*
 * Rises once its moby (unkB2) is collected: waits in state 0 (keeping the
 * target point at its position), then climbs D_0015ED6C per frame until 4
 * above the target, and stays there.
 */
void garage_door_update(struct Moby *m) __asm__("FUN_L03_002dca30");
void garage_door_update(struct Moby *door_moby)
{
    enum {
        GARAGE_DOOR_WAITING_FOR_COLLECTION = 0,
        GARAGE_DOOR_RISING = 1,
        GARAGE_DOOR_RAISED = 2
    };
    Vec4 *target = (Vec4 *)door_moby->pvars;
    s16 save_id_index;
    u16 id;

    switch (door_moby->state) {
    case GARAGE_DOOR_WAITING_FOR_COLLECTION:
        /* one quadword copy (lq/sq); qcopy() schedules differently */
        target->q = ((Vec4 *)&door_moby->pos)->q;
        id = door_moby->save_id;
        save_id_index = id;
        if (D_L03_001BB330.collected[save_id_index] == 0) {
            if (!((D_0014C190_257218[current_level_index_gp][save_id_index >> 5] >> (id & 0x1F)) & 1))
                return;
        }
        door_moby->state = 1;
        return;
    case GARAGE_DOOR_RISING:
        door_moby->pos.z += frame_time;
        if (target->f[2] + 4.0f <= door_moby->pos.z)
            door_moby->state = 2;
        return;
    case GARAGE_DOOR_RAISED:
        door_moby->pos.z = target->f[2] + 4.0f;
        return;
    }
}

f32 D_L06_0017CBE0[1] __attribute__((section(".data"))) = {0};
f32 D_L06_0017CBE4[1] __attribute__((section(".data"))) = {0};
