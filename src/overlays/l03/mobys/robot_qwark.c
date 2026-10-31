/* Level 03 moby class robot_qwark. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/overlay/cuboid.h"

extern char  D_L03_00166EC0[];
extern void  FUN_L00_0025a120(struct Moby *);
extern void  add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int   FUN_L00_00257b90(int, int);
extern int   D_L03_0015F5C4;
extern u8    npc_dialogue_suppression_flags[] __asm__("D_0013D4CC");
extern char *D_001413D0[];
extern void  begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");

/* Pvars of the dialogue moby robot_qwark_update drives (initialize_npc_dialogue / update_npc_dialogue state). */
struct TalkerVars {
    u8  pad0[4];
    s16 mode; /* 0x04: 2 when the talk ends in a warp */
    u8  pad6[0x30];
    s16 unk36;
    u8  pad38[0x14];
    s32 warp; /* 0x4C: index into l03_cuboids, -1 for none */
};

typedef struct {
    u8           pad0[0x20];
    struct Moby *source; /* 0x20 */
    u32          pad24;
    u8           type; /* 0x28 */
} TalkerHit;

extern s32        D_L03_001BA5D0[];
extern void       set_moby_animation(struct Moby *, int, int) __asm__("FUN_00212ed8");
extern TalkerHit *talker_find_hit(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void       allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void       talker_spawn_debris(struct Moby *,
                                      void *,
                                      void *,
                                      f32,
                                      f32,
                                      s32,
                                      s32,
                                      s32,
                                      f32,
                                      f32,
                                      f32,
                                      s32,
                                      f32,
                                      f32,
                                      s32,
                                      s32,
                                      s32,
                                      s32) __asm__("FUN_L00_0025e450");
extern int        talker_init(struct Moby *, struct TalkerVars *) __asm__("FUN_L00_002668a0");
extern int        talker_update(struct Moby *, struct TalkerVars *) __asm__("FUN_L00_00266448");
extern void spawn_effect_particle_with_target_scalar(void *, void *, u32, u32, s32, f32) __asm__("FUN_L00_0026ced0");

/* A talking moby that can be shot down: talks (and may warp the hero) when
 * approached, and once hit by a type 3 hit or already recorded as collected
 * it breaks, records its save bit, and keeps sparking. */
void robot_qwark_update(struct Moby *m) __asm__("FUN_L03_002dbd90");

/* Moby class 914 */
void robot_qwark_update(struct Moby *robot_qwark_moby) {
    enum {
        ROBOT_QWARK_INITIALIZE = 0,
        ROBOT_QWARK_UPDATE_DIALOGUE = 1,
        ROBOT_QWARK_RESOLVE_DIALOGUE = 2,
        ROBOT_QWARK_BROKEN_SPARKING = 3
    };
    struct TalkerVars *dialogue_state = (struct TalkerVars *)robot_qwark_moby->pvars;
    TalkerHit         *hit;
    u8                 type;
    char              *warp_record;
    OvlVec4            particle_velocity;
    OvlVec4            pos;
    float              unused_vector_scratch[4];

    if (robot_qwark_moby->visible != 0 &&
        fast_vec_dist(&robot_qwark_moby->pos, D_L03_00166EC0) < 26.0f) {
        FUN_L00_0025a120(robot_qwark_moby);
        robot_qwark_moby->shadow_range = 0x15;
    }
    if (l03_save_state.cleared[(s16)robot_qwark_moby->save_id] != 0 ||
        (D_0014C190_257218[current_level_index][(s16)robot_qwark_moby->save_id >> 5] >>
         (robot_qwark_moby->save_id & 0x1F)) &
            1) {
        if (robot_qwark_moby->state != 3) {
            robot_qwark_moby->state = 3;
            set_moby_animation(robot_qwark_moby, 1, 0);
        }
    }
    hit = talker_find_hit(robot_qwark_moby, -1, 0);
    if (hit != 0 && hit->source != 0 && (type = hit->type) == 3 &&
        l03_save_state.cleared[(s16)robot_qwark_moby->save_id] == 0 &&
        (((D_0014C190_257218[current_level_index][(s16)robot_qwark_moby->save_id >> 5] >>
           (robot_qwark_moby->save_id & 0x1F)) ^
          1) &
         1)) {
        if (item_unlocked[5] == 0) {
            item_unlocked[5] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        pos.q = *(OvlQuad *)&robot_qwark_moby->pos;
        pos.f[2] += 1.0f;
        particle_velocity.q = 0;
        talker_spawn_debris(robot_qwark_moby,
                            &particle_velocity,
                            &pos,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            9.0f,
                            0,
                            1.0f,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
        robot_qwark_moby->state = type;
        set_moby_animation(robot_qwark_moby, 1, 0);
        D_0014C190_257218[current_level_index][(s16)robot_qwark_moby->save_id >> 5] |=
            1 << (robot_qwark_moby->save_id & 0x1F);
        D_L03_001BA5D0[(s16)robot_qwark_moby->save_id >> 5] |=
            1 << (robot_qwark_moby->save_id & 0x1F);
    }
    robot_qwark_moby->hit_slot = 0xFF;
    if (npc_dialogue_suppression_flags[0] != 0 && robot_qwark_moby->state != 3) {
        return;
    }
    switch (robot_qwark_moby->state) {
    case ROBOT_QWARK_INITIALIZE:
        robot_qwark_moby->spawn_frame = *(u64 *)(D_001413D0[0] + 0x38);
        robot_qwark_moby->state = 1;
        talker_init(robot_qwark_moby, dialogue_state);
        if (alternate_item_available[0x19] != 0) {
            dialogue_state->mode = 1;
            dialogue_state->unk36 = 2;
        }
        break;
    case ROBOT_QWARK_UPDATE_DIALOGUE:
        if (talker_update(robot_qwark_moby, dialogue_state)) {
            alternate_item_available[0x19] = 1;
            begin_dialogue_camera_transition(robot_qwark_moby, 2.7f);
            robot_qwark_moby->state = 2;
        }
        break;
    case ROBOT_QWARK_RESOLVE_DIALOGUE:
        if (D_L03_0015F5C4 == 2) {
            break;
        }
        robot_qwark_moby->state = 1;
        if (dialogue_state->mode == 2 && dialogue_state->warp != -1) {
            warp_record = (char *)l03_cuboids + dialogue_state->warp * 128;
            jet_reset_camera(warp_record + 0x30, warp_record + 0x70);
        }
        break;
    case ROBOT_QWARK_BROKEN_SPARKING:
        particle_velocity.f[0] =
            fast_cos(random_angle_radians()) * (random_float_between(0.0f, 0.25f) * frame_time);
        particle_velocity.f[1] =
            fast_sin(random_angle_radians()) * (random_float_between(0.0f, 0.25f) * frame_time);
        particle_velocity.f[2] = 0.0f;
        pos.f[0] = fast_cos(random_angle_radians()) * random_float_between(0.0f, 0.5f) *
                   random_float_between(0.0f, 1.0f);
        pos.f[1] = fast_sin(random_angle_radians()) * random_float_between(0.0f, 0.5f) *
                   random_float_between(0.0f, 1.0f);
        pos.f[2] = 0.0f;
        add_vectors(unused_vector_scratch, &pos, &robot_qwark_moby->pos);
        if (random_integer_below(3) == 0) {
            spawn_effect_particle_with_target_scalar(
                &pos,
                &particle_velocity,
                0x20202020,
                0x7F7F7F,
                scale_game_frames(FUN_L00_00257b90(0x3C, 0x78)),
                157500.0f);
        }
        break;
    }
}
