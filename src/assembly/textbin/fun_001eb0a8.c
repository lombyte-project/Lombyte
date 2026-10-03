#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001eb0a8/FUN_001eb0a8.s", FUN_001eb0a8);
#else
#include "types.h"

typedef struct { f32 x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    Vec4 position;
    u8 pad20[0x30];
    u8 current_frame;
    u8 next_frame;
    u8 pad52[2];
    f32 frame_fraction;
    u8 pad58[0x19];
    u8 cached_frame;
    u8 pad72[0xD];
    u8 update_enabled;
    u8 pad80[0x26];
    s16 class_id;
} RenderSequenceActor;

typedef struct {
    u8 pad0[0x78];
    Vec4 *animation_positions;
} RenderSequenceSidecar;

typedef struct {
    u8 pad0[0x34];
    s32 time;
    s32 frame;
    s32 sequence_frame;
    s16 end_time;
    u8 pad42[2];
    s16 count;
    u8 pad46[0x132];
    RenderSequenceActor *actors[1];
} RenderSequenceState;

extern RenderSequenceState render_sequence __asm__("D_0018CB20");
extern f32 sequence_fade __asm__("D_0015F43C");
extern s32 game_stage __asm__("D_0015F604");
extern s32 intro_overlay_alpha __asm__("D_0015EF50");
extern s32 language_intro_overlay_alpha __asm__("D_0015EF54");
extern s32 intro_overlay_timer __asm__("D_0015EF58");
extern s32 D_0013CAE4[];
extern void InitializeTransferCommand(void);
extern void func_001E9410(RenderSequenceActor *);
extern void func_001E9428(void);
extern void func_001E9430(void);
extern void func_001EAF88(void);
extern s32 func_001F96F8(s32);
extern void func_001F9A10(Vec4 *, Vec4 *, Vec4 *);
extern void func_001F9A68(Vec4 *, Vec4 *, f32);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 func_001FA6C0(s32);
extern void func_001FCE28(void);
extern void func_002049F0(s32);
extern void func_0020C880(RenderSequenceActor *);
extern void func_0020DEF8(RenderSequenceActor *);
extern void func_002192A8(void);
extern void func_0022CA50(void);

void update_gameplay_frame(void) __asm__("FUN_001eb0a8");

void update_gameplay_frame(void)
{
    Vec4 first_position;
    Vec4 next_position;
    RenderSequenceActor *actor;
    Vec4 *animation_positions;
    s32 actor_index;
    s32 frame_index;
    f32 frame_fraction;

    func_001E9430();
    sequence_fade -= 0.0625f;
    render_sequence.frame++;
    render_sequence.time++;
    if (sequence_fade < 0.0f) {
        sequence_fade = 0.0f;
    }
    if (render_sequence.time >= render_sequence.end_time) {
        render_sequence.sequence_frame = 0;
        render_sequence.time = 0;
        func_002049F0(0);
    } else if (render_sequence.frame >= 0x60) {
        func_002049F0(++render_sequence.sequence_frame);
    }
    func_001EAF88();
    for (actor_index = 0; actor_index < render_sequence.count; actor_index++) {
        actor = render_sequence.actors[actor_index];
        frame_index = render_sequence.frame >> 1;
        actor->current_frame = frame_index;
        actor->next_frame = frame_index + 1;
        func_0020C880(actor);
        frame_fraction = func_001FA6C0(render_sequence.frame & 1) * 0.5f;
        actor->frame_fraction = frame_fraction;
        animation_positions = ((RenderSequenceSidecar *)actor)->animation_positions;
        func_001F9A68(&first_position, &animation_positions[actor->current_frame], 1.0f - frame_fraction);
        func_001F9A68(&next_position, &animation_positions[actor->next_frame], frame_fraction);
        func_001F9A10(&actor->position, &first_position, &next_position);
        actor->cached_frame = 0xFF;
        func_0020DEF8(actor);
        actor->update_enabled = 0;
        if (actor->class_id == 0) {
            func_001E9410(actor);
        }
    }
    func_001E9428();
    if (game_stage == 0) {
        intro_overlay_timer++;
        if (func_001F96F8(0x3C) < intro_overlay_timer) {
            if (++intro_overlay_alpha > 0x40) {
                intro_overlay_alpha = 0x40;
            }
        }
        if (func_001F96F8(0x78) < intro_overlay_timer) {
            language_intro_overlay_alpha = (s32)(fast_cos((intro_overlay_timer - func_001F96F8(0x78)) % 60 * 0.10471976f + -3.1415927f) * 32.0f) + 0x60;
        }
        if (D_0013CAE4[0] & 0x840) {
            InitializeTransferCommand();
        }
        func_0022CA50();
    } else if (game_stage == 3) {
        intro_overlay_timer = func_001F96F8(0x3C);
        if ((intro_overlay_alpha -= 0x10) < 0) {
            intro_overlay_alpha = 0;
        }
        if ((language_intro_overlay_alpha -= 0x10) < 0) {
            language_intro_overlay_alpha = 0;
        }
        func_002192A8();
        func_0022CA50();
    } else if (game_stage == 4) {
        func_001FCE28();
        func_0022CA50();
    }
}

extern __typeof__(update_gameplay_frame) func_001EB0A8 __attribute__((alias("FUN_001eb0a8")));

#endif /* NON_MATCHING */
