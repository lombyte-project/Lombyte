/* Level 04 moby class eudora_bridge. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/overlay/level_labels.h"

extern void refresh_moby_spatial_bounds_from_basis(void *);
extern s32  voice_is_playing(long moby, s32 voice) __asm__("FUN_L00_0028d8c0");
extern void release_voice_slot(int);

/* Pvars of a moby that follows a linked switch moby. */
typedef struct {
    s32 link;  /* index of the linked moby in the moby array, -1 for none */
    s32 voice; /* playing loop sound, -1 for none */
} SwitchFollowerVars;

/* The fields of a moby class header read when a moby changes class. */
typedef struct {
    u8  pad0[0xE];
    u8  unkE;
    u8  padF;
    u32 unk10;
    u8  pad14[0x10];
    f32 scale; /* 0x24 */
} MobyClassHeader;

extern u8  level04_moby_class_slot_by_oclass[] __asm__("D_L04_00197D40"); /* class slot by oclass */
extern s32 allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");

/* Follows a linked switch moby (class 0x118): mirrors its position value, loops a sound while it moves, plays a click at either end and swaps class 0x41C back to 0x1B0 when it reaches 1.0. */
void eudora_bridge_update(struct Moby *m) __asm__("FUN_L04_002c6858");

/* Moby classes 432, 1052 */
void eudora_bridge_update(struct Moby *moby) {
    enum {
        EUDORA_BRIDGE_STATE_INITIALIZE = 0,
        EUDORA_BRIDGE_STATE_FOLLOW_LINKED_SWITCH = 1,
        EUDORA_BRIDGE_STATE_RELEASE_MOVEMENT_VOICE = 2
    };
    SwitchFollowerVars *switch_follower_vars = (SwitchFollowerVars *)moby->pvars;
    f32                 last = moby->anim_time;
    struct Moby        *linked_switch_moby;
    f32                *linked_switch_travel;
    f32                 sampled_switch_travel;

    if (switch_follower_vars == NULL) {
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case EUDORA_BRIDGE_STATE_INITIALIZE:
        blend_moby_animation(moby, 1, 0, scale_game_frames(300));
        moby->state = 1;
        moby->anim_speed = 0;
        switch_follower_vars->voice = -1;
        if (moby->oclass == 0x1B0) {
            moby->oclass = 0x41C;
            moby->class_slot = level04_moby_class_slot_by_oclass[0x41C];
            moby->pclass = (struct MobyClass *)D_L04_00197480[moby->class_slot];
            moby->unk71 = 0xFF;
            moby->scale = ((MobyClassHeader *)moby->pclass)->scale;
            update_moby_animation_state(moby);
            moby->unk72 = ((MobyClassHeader *)moby->pclass)->unkE;
            moby->collision = ((MobyClassHeader *)moby->pclass)->unk10;
            refresh_moby_spatial_bounds_from_basis(moby);
        }
        break;
    case EUDORA_BRIDGE_STATE_FOLLOW_LINKED_SWITCH:
        if (switch_follower_vars->link == -1)
            break;
        linked_switch_moby = (struct Moby *)(moby_array + (switch_follower_vars->link << 8));
        if (linked_switch_moby->oclass != 0x118)
            break;
        linked_switch_travel = (f32 *)linked_switch_moby->pvars;
        sampled_switch_travel = *linked_switch_travel;
        moby->anim_time = sampled_switch_travel;
        if (sampled_switch_travel != last) {
            if (*linked_switch_travel == 1.0f || *linked_switch_travel == 0.0f) {
                if (voice_is_playing(moby, switch_follower_vars->voice)) {
                    s32 h = switch_follower_vars->voice;
                    if (h != -1) {
                        char *e = (char *)&voice_pool + h * 0x70;
                        if (*(struct Moby **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0)
                            release_voice_slot(h);
                    }
                    switch_follower_vars->voice = -1;
                }
                allocate_voice_for_target_entry(1, 0, moby);
                if (moby->oclass == 0x41C && *linked_switch_travel == 1.0f) {
                    moby->oclass = 0x1B0;
                    moby->class_slot = level04_moby_class_slot_by_oclass[0x1B0];
                    moby->pclass = (struct MobyClass *)D_L04_00197480[moby->class_slot];
                    moby->unk71 = 0xFF;
                    moby->scale = ((MobyClassHeader *)moby->pclass)->scale;
                    update_moby_animation_state(moby);
                    moby->unk72 = ((MobyClassHeader *)moby->pclass)->unkE;
                    moby->collision = ((MobyClassHeader *)moby->pclass)->unk10;
                    refresh_moby_spatial_bounds_from_basis(moby);
                    moby->state = 2;
                }
            } else if (!voice_is_playing(moby, switch_follower_vars->voice)) {
                switch_follower_vars->voice = allocate_voice_for_target_entry(0, 4, moby);
            }
        } else {
            if (voice_is_playing(moby, switch_follower_vars->voice)) {
                s32 h = switch_follower_vars->voice;
                if (h != -1) {
                    char *e = (char *)&voice_pool + h * 0x70;
                    if (*(struct Moby **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0)
                        release_voice_slot(h);
                }
                switch_follower_vars->voice = -1;
            }
        }
        break;
    case EUDORA_BRIDGE_STATE_RELEASE_MOVEMENT_VOICE:
        if (voice_is_playing(moby, switch_follower_vars->voice)) {
            s32 h = switch_follower_vars->voice;
            if (h != -1) {
                char *e = (char *)&voice_pool + h * 0x70;
                if (*(struct Moby **)(e + 0x88) == moby && *(u8 *)(e + 0x74) != 0)
                    release_voice_slot(h);
            }
            switch_follower_vars->voice = -1;
        }
        break;
    }
}
