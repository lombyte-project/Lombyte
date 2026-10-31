/* Shared moby class devestator_ammo (also the other weapon ammo pickups): spawning, collection and the pickup update. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "qcopy.h"
#include "rnc/overlay/item_defs.h"
#include "rnc/globals.h"

typedef struct {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) FxVec;

typedef struct {
    int   amount;
    char  pad4[2];
    short recovery_timer;
    int   weapon;
    char  padC[4];
    Vec4f velocity;
    int   timer;
    float rise_speed;
    float resting_height;
} AmmoPickup;

int          FUN_001f96f8_002c52a8(int) __asm__("FUN_001f96f8");
extern int   add_ammo(int, int) __asm__("FUN_L00_00233e40");
extern void  queue_formatted_dialogue_message(int, int, int) __asm__("FUN_L00_00263db0");
extern void  clear_u64_value(void *) __asm__("FUN_001f99f8");
extern int   f99a8_2c5be8(int) __asm__("FUN_001f99a8");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
float        approach_value(void *, float, float) __asm__("FUN_00213ed8");
extern int   FUN_001f96f8_2c53c0(int) __asm__("FUN_001f96f8");
extern float FUN_001f99c0_2c53c0(float) __asm__("FUN_001f99c0");
extern void  FUN_001f9a10_2c53c0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void  FUN_001f9a28_2c53c0(void *, void *, void *) __asm__("FUN_001f9a28");
extern float FUN_001f9af0_2c53c0(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9b48_2c53c0(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001f9b80_2c53c0(void *, void *) __asm__("FUN_001f9b80");
extern void  FUN_001f9bf8_2c53c0(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_001f9dc8_2c53c0(float) __asm__("FUN_001f9dc8");
extern int   FUN_001f9740_2c53c0(int *) __asm__("FUN_001f9740");
extern int   FUN_L00_001f0d60_2c53c0(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int   FUN_001efa68_2c53c0(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int   FUN_001fa6d0_2c53c0(float) __asm__("FUN_001fa6d0");
extern int   FUN_001f99a8_2c53c0(int) __asm__("FUN_001f99a8");
extern int   FUN_L00_0028dc90_2c53c0(int, int, int, int) __asm__("FUN_L00_0028dc90");
extern float jet_distance(void *, void *) __asm__("FUN_L00_00250430");

extern ItemDef item_defs_lui[] __asm__("D_L00_00179AC0") NOT_SDA;
extern int     D_15F5CC_2c5be8 __asm__("D_L00_0015F5CC");
extern int     weapon_ammo_pickup_totals[] __asm__("D_0013DE08");
extern int     D_L00_00161838;
extern f32     frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int     D_L00_0015F5CC_macro __asm__("D_L00_0015F5CC") MACRO_ADDR;
extern float   frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern float   frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern FxVec   D_L00_00173E60_2c53c0 __asm__("D_L00_00173E60");
extern float   frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int     D_L00_00161838_macro __asm__("D_L00_00161838") MACRO_ADDR;

int D_L01_00161868 = 0;
int D_L02_00161808 = 0;
int D_L03_00161818 = 0;
int D_L04_00161808 = 0;
int D_L05_00161858 = 0;
int D_L06_00161838 = 0;
int D_L07_00161808 = 0;
int D_L08_00161858 = 0;
int D_L09_001618E8 = 0;
int D_L10_00161818 = 0;
int D_L11_00161898 = 0;
int D_L12_00161818 = 0;
int D_L13_001618E8 = 0;
int D_L14_00161A28 = 0;
int D_L15_001619E8 = 0;
int D_L16_001618C8 = 0;
int D_L17_00161918 = 0;
int D_L18_001618A8 = 0;

unsigned char *spawn_on_path(u128 *a, int b, int c, int d) __asm__("FUN_L00_002c52a8");

unsigned char *spawn_on_path(u128 *spawn_position, int weapon_index, int ammo_amount, int d) {
    unsigned char *spawned_moby = 0;
    {
        ItemDef *spawn_config_entry = (ItemDef *)(weapon_index * 0x4C + (u8 *)item_defs_lui);
        if (spawn_config_entry->ammo_pickup_cls > 0)
            spawned_moby = create_moby(spawn_config_entry->ammo_pickup_cls);
    }
    if (spawned_moby) {
        int *moby_vars;
        spawned_moby[0x30] = 0xff;
        *(short *)(spawned_moby + 0x32) = 0xff;
        moby_vars = *(int **)(spawned_moby + 0x78);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        spawned_moby[0xbc] = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        if (ammo_amount == -1)
            moby_vars[0] = ((VendorItemInfo *)(weapon_index * 0x18 + (u8 *)D_L00_001C40B0))->hC;
        else
            moby_vars[0] = ammo_amount;
        moby_vars[2] = weapon_index;
        *(float *)(moby_vars + 3) = (float)d;
        moby_vars[8] = FUN_001f96f8_002c52a8(0x10);
        *(float *)(moby_vars + 10) = *(float *)(spawned_moby + 0x18);
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
    }
    return spawned_moby;
}

/* Update ammunition pickup physics, collection, animation and scale.
   Animation states share one stack value; index reloads follow calls. */
void devestator_ammo_update(struct Moby *m) __asm__("FUN_L00_002c53c0");

/* Moby classes 204, 213, 214, 222, 223, 225, 226, 1006, 1438, 1447, 1449 */
void devestator_ammo_update(struct Moby *pickup_moby) {
    enum {
        AMMO_PICKUP_INITIALIZE = 0,
        AMMO_PICKUP_WAIT_FOR_COLLECTION = 1,
        AMMO_PICKUP_COLLECTED = 2,
        AMMO_PICKUP_PULSE_ALPHA = 3,
        AMMO_PICKUP_RESTORE_SCALE = 4
    };
    AmmoPickup  *pickup_vars;
    unsigned int state;
    FxVec        position;
    FxVec        predicted;
    float        animated_alpha;
    float        magnitude;
    float        current;
    float        target;
    float        step;
    float        remaining;
    pickup_vars = (AmmoPickup *)pickup_moby->pvars;
    FUN_001f9740_2c53c0(&pickup_vars->timer);
    state = pickup_moby->state;
    if (state == 1 || state == 4 || state == 3) {
        magnitude = FUN_001f9af0_2c53c0(&pickup_vars->velocity);
        if (magnitude != 0.0f) {
            qcopy(&position, &pickup_moby->pos);
            position.z += 0.5f;
            if (FUN_L00_001f0d60_2c53c0(0.4f, &position, 2, 0)) {
                qcopy(&pickup_moby->pos, &coll_hit_position);
                pickup_moby->pos.z -= 0.5f;
            }
        }
        pickup_vars->velocity.z -= frame_time_sq_macro * 4.9f;
        FUN_001f9a10_2c53c0(&predicted, &pickup_moby->pos, &pickup_vars->velocity);
        pickup_vars->velocity.z -= frame_time_sq_macro * 4.9f;
        qcopy(&position, &pickup_moby->pos);
        position.z += 0.1f;
        if (FUN_001efa68_2c53c0(&position, &predicted, 2, 0, 0)) {
            clear_u64_value(&pickup_vars->velocity);
            qcopy(&pickup_moby->pos, &D_L00_00173E60_2c53c0);
        } else {
            qcopy(&pickup_moby->pos, &predicted);
        }
    }
    switch (pickup_moby->state) {
    case AMMO_PICKUP_INITIALIZE:
        pickup_moby->state = 1;
        break;
    case AMMO_PICKUP_WAIT_FOR_COLLECTION: {
        struct Hero *player;
        float       *value_ptr;
        float        height_delta;
        float        distance;
        int          duration;
        int          eligible;
        int          idx;
        int          sound_idx;
        int          account_idx;
        int          before;
        int          after;
        int          difference;
        int          sound;
        int          elapsed;
        int          random;
        int          max_ammo;
        value_ptr = &animated_alpha;
        *value_ptr = (float)pickup_moby->alpha;
        duration = FUN_001f96f8_2c53c0(30);
        approach_value(value_ptr, 128.0f, (float)(128 / duration));
        pickup_moby->alpha = FUN_001fa6d0_2c53c0(*value_ptr);
        if (pickup_vars->timer > 0)
            return;
        if (FUN_001f9af0_2c53c0(&pickup_vars->velocity) != 0.0f)
            return;
        player = &hero;
        eligible = 1;
        height_delta = FUN_001f99c0_2c53c0(pickup_moby->pos.z - player->motion.pos.f[2]);
        if (player->reach_height < height_delta) {
            eligible = 0;
        } else {
            distance = FUN_001f9b80_2c53c0(&pickup_moby->pos, &player->motion.pos);
            if (player->reach_distance < distance)
                eligible = 0;
        }
        idx = pickup_vars->weapon;
        before = weapon_ammo_counts[idx];
        max_ammo = D_L00_001C40B0[idx].hE;
        if (before >= max_ammo)
            eligible = 0;
        if (pickup_moby->pos.z < pickup_vars->resting_height - 2.0f)
            eligible = 1;
        if (hero.health.hp == 0)
            eligible = 0;
        if (!eligible)
            goto pickup_failed;

        {
            add_ammo(idx, pickup_vars->amount);
            sound_idx = pickup_vars->weapon;
            after = weapon_ammo_counts[sound_idx];
            if (before < after) {
                difference = after - before;
                if (difference == 1) {
                    sound = ((ItemDef *)((char *)item_defs + sound_idx * sizeof(ItemDef)))
                                ->pickup_message_single;
                } else {
                    sound = ((ItemDef *)((char *)item_defs + sound_idx * sizeof(ItemDef)))
                                ->pickup_message_multiple;
                }
                queue_formatted_dialogue_message(sound, difference, -1);
            }
            account_idx = pickup_vars->weapon;
            weapon_ammo_pickup_totals[account_idx] += weapon_ammo_counts[account_idx] - before;
            pickup_moby->state = 2;
            pickup_moby->scale *= frame_scale_macro * 0.20000005f + 1.0f;
            clear_u64_value(&pickup_vars->velocity);
            elapsed = D_L00_0015F5CC_macro - D_L00_00161838_macro;
            pickup_vars->rise_speed = frame_time_macro * 8.0f;
            random = FUN_001f99a8_2c53c0(elapsed);
            if (FUN_001f96f8_2c53c0(10) < random) {
                FUN_L00_0028dc90_2c53c0(0, 0, (int)pickup_moby, 0xD5);
                D_L00_00161838_macro = D_L00_0015F5CC_macro;
            }
            return;
        }
    pickup_failed:
        if (pickup_moby->alpha < 0xFF)
            ++pickup_moby->alpha;
        if (jet_distance(pickup_moby, hero.moby) <= -0.2f) {
            pickup_moby->state = 3;
            pickup_vars->recovery_timer = FUN_001f96f8_2c53c0(60);
        }
        return;
    }
    case AMMO_PICKUP_COLLECTED: {
        float limit;
        float distance;
        float factor;
        float class_scale;
        approach_value(&pickup_vars->rise_speed, 0.0f, frame_time_sq_macro * 24.0f);
        pickup_moby->pos.z += pickup_vars->rise_speed;
        limit = FUN_001f9af0_2c53c0(&pickup_vars->velocity);
        approach_value(&limit, frame_time_macro * 64.0f, frame_time_sq_macro * 24.0f);
        FUN_001f9a28_2c53c0(&pickup_vars->velocity, &hero.motion.body_center, &pickup_moby->pos);
        if (pickup_moby->scale == 0.0f || FUN_001f9af0_2c53c0(&pickup_vars->velocity) < limit) {
            mark_moby_for_removal(pickup_moby);
            return;
        }
        FUN_001f9bf8_2c53c0(&pickup_vars->velocity, &pickup_vars->velocity, limit);
        FUN_001f9a10_2c53c0(&pickup_moby->pos, &pickup_moby->pos, &pickup_vars->velocity);
        distance = FUN_001f9b48_2c53c0(&pickup_moby->pos, &hero.motion.body_center);
        factor = (float)FUN_001f96f8_2c53c0(30);
        factor = factor * limit;
        factor = factor + 0.01f;
        distance = distance / factor;
        if (1.0f < distance) {
            distance = 1.0f;
        } else if (distance < 0.3f) {
            distance = 0.3f;
        }
        class_scale = pickup_moby->pclass->scale;
        approach_value(&pickup_moby->scale, class_scale * distance, class_scale * 0.1f);
        return;
    }
    case AMMO_PICKUP_PULSE_ALPHA: {
        float *value_ptr;
        float  angle;
        float  oscillation;
        float  target;
        int    phase;
        int    random;
        int    idx;
        phase = D_L00_0015F5CC_macro % 60;
        animated_alpha = (float)pickup_moby->alpha;
        angle = (float)phase / 60.0f;
        angle = angle * 6.28318f;
        angle = angle - 3.14159f;
        oscillation = FUN_001f9dc8_2c53c0(angle);
        oscillation = oscillation * 5.0f;
        oscillation = oscillation + 5.0f;
        target = (float)(FUN_001fa6d0_2c53c0(oscillation) + 10);
        value_ptr = &animated_alpha;
        random = FUN_001f96f8_2c53c0(30);
        approach_value(value_ptr, target, (float)(128 / random));
        pickup_moby->alpha = FUN_001fa6d0_2c53c0(*value_ptr);
        if (jet_distance(pickup_moby, hero.moby) >= 0.0f) {
            pickup_moby->state = 1;
            return;
        }
        idx = pickup_vars->weapon;
        if (weapon_ammo_counts[idx] < D_L00_001C40B0[idx].hE) {
            pickup_moby->state = 1;
        }
        return;
    }
    case AMMO_PICKUP_RESTORE_SCALE:
        current = pickup_moby->scale;
        target = pickup_moby->pclass->scale;
        step = frame_scale_macro * 0.03f;
        current += (target - current) * step;
        pickup_moby->scale = current;
        target = pickup_moby->pclass->scale;
        remaining = (target - current) / target;
        if (remaining < 0.01f) {
            pickup_moby->scale = target;
            pickup_moby->state = 1;
            pickup_moby->collision = pickup_moby->pclass->collision;
        }
        break;
    }
}

int try_collect_ammo_pickup(struct Moby *pickup_moby) __asm__("FUN_L00_002c5be8");

int try_collect_ammo_pickup(struct Moby *pickup_moby) {
    int  *pickup_vars = ((int *)pickup_moby->pvars);
    int   before;
    float excess_ammo_amount;
    int   ammo_added;

    if (pickup_moby->state == 2) {
    fail:
        return 0;
    }
    before = weapon_ammo_counts[pickup_vars[2]];
    excess_ammo_amount = add_ammo(pickup_vars[2], pickup_vars[0]);
    if (before >= weapon_ammo_counts[pickup_vars[2]]) {
        goto fail;
    }
    ammo_added = weapon_ammo_counts[pickup_vars[2]] - before;
    queue_formatted_dialogue_message(
        ammo_added == 1
            ? ((ItemDef *)(pickup_vars[2] * 0x4C + (char *)item_defs_lui))->pickup_message_single
            : ((ItemDef *)(pickup_vars[2] * 0x4C + (char *)item_defs_lui))->pickup_message_multiple,
        ammo_added,
        -1);
    if (excess_ammo_amount < (float)pickup_vars[0]) {
        weapon_ammo_pickup_totals[pickup_vars[2]] += weapon_ammo_counts[pickup_vars[2]] - before;
        pickup_moby->state = 2;
        pickup_moby->scale *= frame_scale * 0.200000048f + 1.0f;
        clear_u64_value(pickup_vars + 4);
        {
            int event_counter_delta = D_15F5CC_2c5be8 - D_L00_00161838;
            int event_gate_threshold;
            *(float *)(pickup_vars + 9) = frame_time * 8.0f;
            event_gate_threshold = f99a8_2c5be8(event_counter_delta);
            if (scale_game_frames(10) < event_gate_threshold) {
                trooper_play_sound(0, 0, pickup_moby, 0xD5);
                D_L00_00161838 = D_15F5CC_2c5be8;
            }
        }
    }
    return 1;
}

int D_L00_00161838_macro __asm__("D_L00_00161838") = 0;
