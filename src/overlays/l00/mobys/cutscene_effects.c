/* Level 00 moby class cutscene_effects (0x609): aims the cutscene camera and puffs smoke during the cutscene phases. */
#include "types.h"
#include "sda.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

typedef struct {
    u8 pad0[0x20];
    u8 state;
    u8 pad21[0xF];
    u8 unk30;
} FlockMoby;

typedef struct {
    u8    pad0[0x30];
    s32   phase;
    s32   level;
    u8    pad38[0x178 - 0x38];
    void *mobys[4];
} FlockGame;

typedef struct {
    u8  pad0[4];
    s32 unk4;
    u8  pad8[2];
    u8  unkA;
    u8  unkB;
} FlockTail;

typedef struct {
    u8        pad0[3];
    u8        unk3;
    u8        pad4[4];
    u8        unk8;
    u8        pad9;
    s16       unkA;
    u8        padC[0x20 - 0xC];
    FlockTail tail;
} FlockPart;

extern FlockGame D_L00_0016C860;
extern s32       scale_game_frames_s32(s32) __asm__("FUN_001f96f8");
extern Vec4      zero_vector __asm__("D_L00_0015F580") MACRO_ADDR;

extern FlockPart *
spawn_puff(void *, s32, void *, s32, f32, f32, f32, f32) __asm__("FUN_L00_0026d000");

void update_phase_effect_controller(FlockMoby *moby) __asm__("FUN_L00_002e3800");

void update_phase_effect_controller(FlockMoby *moby) {
    enum {
        FLOCK_EFFECT_STATE_INITIALIZE = 0,
        FLOCK_EFFECT_STATE_PROCESS_PHASE_EFFECTS = 1
    };
    f32        effect_spawn_position[4];
    void      *effect_source_moby;
    FlockPart *part;
    s32        color;
    s32        random_magnitude;
    s32        i;
    s32        phase;
    s32        phase_effect_moby_index;
    FlockTail *tail;

    switch (moby->state) {
    case FLOCK_EFFECT_STATE_INITIALIZE:
        moby->unk30 = 0xFF;
        moby->state = 1;
        break;
    case FLOCK_EFFECT_STATE_PROCESS_PHASE_EFFECTS:
        if ((D_L00_0015F5C4 == 2 && D_L00_0016C860.phase == 2 &&
             scale_game_frames_s32(0x618) <= D_L00_0016C860.level) ||
            (D_L00_0015F5C4 == 2 && D_L00_0016C860.phase == 3)) {
            phase = D_L00_0016C860.phase;
            phase_effect_moby_index = -1;
            if (phase == 2) {
                phase_effect_moby_index = 3;
            } else if (phase == 3) {
                phase_effect_moby_index = 2;
            }
            cutscene_set_camera_target(D_L00_0016C860.mobys[phase_effect_moby_index]);
        }
        if (D_L00_0015F5C4 == 2 && D_L00_0016C860.phase == 4) {
            effect_source_moby = D_L00_0016C860.mobys[3];
            if (effect_source_moby != 0 && scale_game_frames_s32(150) <= D_L00_0016C860.level &&
                D_L00_0016C860.level <= scale_game_frames_s32(220)) {
                moby_point_position(effect_source_moby, 0, effect_spawn_position);
                draw_trail(effect_spawn_position,
                           effect_spawn_position,
                           &zero_vector,
                           120000.0f,
                           50000.0f);
                for (i = 0; i < 10; i++) {
                    color = (random_int_between(0x40, 0x80) << 24) | 0x787878;
                    random_magnitude = random_int_between(0, 4);
                    part = spawn_puff(effect_spawn_position,
                                      random_integer_below(2) == 0 ? random_magnitude
                                                                   : -random_magnitude,
                                      &zero_vector,
                                      color,
                                      0.75f,
                                      1.0f,
                                      1.03f,
                                      120000.0f);
                    if (part != 0) {
                        part->unk3 = 0x44;
                        tail = &part->tail;
                        part->unk8 = random_int_between(0, 0xFF);
                        part->unkA = scale_game_frames_s32(60);
                        tail->unk4 = 2;
                        tail->unkA = color >> 24;
                        tail->unkB = part->unkA;
                    }
                }
            }
        }
        break;
    }
}
