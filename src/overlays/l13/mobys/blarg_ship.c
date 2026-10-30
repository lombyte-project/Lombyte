/* Level 13 Blarg ship moby class. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/count_active_moby_group_members.h"
#include "qzero.h"
#include "rnc/game/item_state.h"

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
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

extern int resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");

extern void trooper_anim_update(struct Moby *, void *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");

extern void FUN_0020c828(void *);

extern float D_L13_0015F580[] MACRO_ADDR;

extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");

extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");

extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);

extern s32 blarg_ship_destroyed_save_id_bits[] __asm__("D_L13_001BA7D0");

void blarg_ship_update(struct Moby *moby) __asm__("FUN_L13_0030cdb8");

void blarg_ship_update(struct Moby *moby) {
    enum {
        BLARG_SHIP_ENTER_DAMAGE_HANDLING = 0,
        BLARG_SHIP_HANDLE_DAMAGE = 1,
        BLARG_SHIP_EXPLODE = 2
    };
    char   *data;
    void   *hit;
    OvlVec4 vector;
    char    hit_buffer[16];
    int     i, j;
    if ((D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] >> (moby->save_id & 31) &
         1) != 0) {
        FUN_0020c828(moby);
        return;
    }
    data = (char *)moby->pvars;
    hit = find_moby_hit(moby, 0x10000, 0);
    resolve_moby_hit_response(moby, hit, data + 0x20, 0, hit_buffer, 0, 0, 4);
    switch (moby->state) {
    case BLARG_SHIP_ENTER_DAMAGE_HANDLING:
        moby->state = 1;
        break;
    case BLARG_SHIP_HANDLE_DAMAGE:
        if (hit != 0) {
            float cost = *(float *)((char *)hit + 0x2c);
            if (*(float *)(data + 0x20) < cost) {
                moby->state = 2;
            } else {
                ((unsigned char *)data)[0x67] = 0xfa;
                *(float *)(data + 0x20) -= cost;
                trooper_anim_update(moby, data + 0x60);
            }
            moby->hit_slot = 0xff;
        }
        trooper_anim_finish(moby, data + 0x60);
        break;
    case BLARG_SHIP_EXPLODE: {
        void *pos;
        void *rot;
        void *zero_vector;
        if (moby->group != 0xff && count_active_moby_group_members(moby->group, -1) == 1 &&
            item_unlocked[0x16] == 0) {
            item_unlocked[0x16] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53d6, -1);
        }
        allocate_voice_for_target_entry(0, 0, moby);
        pos = &moby->pos;
        rot = &moby->rot;
        spawn_proximity_gated_bolts(moby);
        zero_vector = D_L13_0015F580;
        D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
            1 << (moby->save_id & 31);
        blarg_ship_destroyed_save_id_bits[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
        for (j = 0; j < 2; j++) {
            for (i = 0x709; i < 0x70d; i++) {
                qzero(&vector);
                add_random_offset_to_vector_xyz(&vector, 1.5f);
                vector.f[2] += 1.7f;
                fast_vec_add(&vector, &vector, pos);
                FUN_L00_00263fd8(
                    moby, i, &vector, rot, 0, 0, 0.0f, zero_vector, zero_vector, zero_vector);
            }
        }
        spawn_explosion(
            moby, pos, 0, 0.0f, 0.0f, 10, 3, 0x10, 6.0f, 3.0f, 9.0f, 1.1f, 0, 15.0f, 1, 3, -1, 0);
        FUN_0020c828(moby);
        break;
    }
    }
}

s32 D_L13_001CABA8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001CABAC[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001CABB0[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001CABB4[1] __attribute__((section(".data"))) = {0};

f32 D_L13_001CABB8[1] __attribute__((section(".data"))) = {0};
