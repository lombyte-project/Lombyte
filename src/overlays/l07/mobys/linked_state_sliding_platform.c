/* Level 07 moby classes 0x3f5, 0x3f6, 0x428, 0x429: linked state sliding platform. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"

extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern char D_0013E550[];
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E370), where it is exact; names translated to the US level program. */
extern s32 D_L07_00184528[] NOT_SDA;

typedef struct {
    f32 origin_x;
    f32 origin_y;
    f32 origin_z;
    s16 sound_slot;
    s8  trigger_sound;
    s8  arrival_sound;
    s32 target_index;
    s16 flag_index;
    s16 target_state;
    s32 target_state_word;
    f32 radius;
} FUN_L07_0030cf90_Vars;

/* Slides out from its origin once its linked moby reaches one state and back when it reaches
   another; a level flag is set while it is out. */
void update_linked_state_sliding_platform(struct Moby *moby) __asm__("FUN_L07_0030cf90");
s32  allocate_voice_for_target_entry(s32   entry_index,
                                     s32   flags,
                                     void *target) __asm__("FUN_0022da68");

void update_linked_state_sliding_platform(struct Moby *moby) {
    enum {
        PLATFORM_INITIALIZE = 0,
        PLATFORM_WAIT_TO_MOVE_OUTWARD = 1,
        PLATFORM_MOVE_OUTWARD = 2,
        PLATFORM_WAIT_TO_RETURN = 3,
        PLATFORM_RETURN_TO_ORIGIN = 4
    };
    extern char           *level_mobys_gp __asm__("D_L07_0015FFD8") __attribute__((sda));
    FUN_L07_0030cf90_Vars *vars = (FUN_L07_0030cf90_Vars *)moby->pvars;
    f32                    origin[4];
    Vec4                   rotated;
    Vec4                   step;
    struct Moby           *target;
    s16                    voice;
    f32                    distance;
    f32                    step_size;

    origin[0] = vars->origin_x;
    origin[1] = vars->origin_y;
    origin[2] = vars->origin_z;
    origin[3] = 0.0f;
    if (moby->mission != 0xff && visited52[moby->mission + (current_level_index << 4)] == 0xff)
        vars->target_index = -1;

    switch (moby->state) {
    case PLATFORM_INITIALIZE: {
        Vec4f *position = &moby->pos;
        vars->origin_x = position->x;
        vars->origin_y = position->y;
        vars->origin_z = position->z;
        vars->sound_slot = -1;
        if (vars->flag_index != -1)
            D_L07_00184528[vars->flag_index] = 0;
        moby->state = 1;
        break;
    }
    case PLATFORM_WAIT_TO_MOVE_OUTWARD:
        if (vars->target_index != -1) {
            target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100);
            if (target != 0 && target->state != 0xfe && target->state != 0xfd &&
                target->state != vars->target_state)
                return;
        }
        if (vars->trigger_sound != -1)
            allocate_voice_for_target_entry(vars->trigger_sound, 0, moby);
        moby->state = 2;
        break;
    case PLATFORM_MOVE_OUTWARD:
        if (vars->target_index != -1 &&
            (target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100)) != 0 &&
            target->state != 0xfe && target->state != 0xfd &&
            target->state == vars->target_state_word) {
            allocate_voice_for_target_entry(vars->trigger_sound, 0, moby);
            moby->state = 4;
            break;
        }
        distance = vector_distance_xy(&moby->pos, origin);
        if (vars->radius < distance) {
            if (vars->arrival_sound != -1)
                allocate_voice_for_target_entry(vars->arrival_sound, 0, moby);
            if (vars->flag_index != -1)
                D_L07_00184528[vars->flag_index] = 1;
            moby->state = 3;
        } else {
            step.q = 0;
            step.f[1] = -(frame_scale * 0.01f);
            rotated.q = step.q;
            if (moby->oclass == 0x3f5 || moby->oclass == 0x429)
                rotated.f[1] = -rotated.f[1];
            transform_vector_by_basis(&step, &rotated, &moby->rot_row0);
            fast_vec_add(&moby->pos, &moby->pos, &step);
            return;
        }
        break;
    case PLATFORM_WAIT_TO_RETURN:
        if (vars->target_index != -1) {
            target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100);
            if (target != 0 && target->state != 0xfe && target->state != 0xfd) {
                if (target->state != vars->target_state_word)
                    return;
                if (vars->trigger_sound != -1)
                    allocate_voice_for_target_entry(vars->trigger_sound, 0, moby);
                moby->state = 4;
            }
        }
        break;
    case PLATFORM_RETURN_TO_ORIGIN:
        if (vars->target_index == -1 ||
            (target = (struct Moby *)(level_mobys_gp + vars->target_index * 0x100)) == 0 ||
            target->state == 0xfe || target->state == 0xfd || target->state == vars->target_state) {
            allocate_voice_for_target_entry(vars->trigger_sound, 0, moby);
            moby->state = 2;
            return;
        }
        distance = vector_distance_xy(&moby->pos, origin);
        step_size = frame_scale * 0.01f;
        if (distance <= step_size) {
            Vec4f *position = &moby->pos;
            position->x = vars->origin_x;
            position->y = vars->origin_y;
            position->z = vars->origin_z;
            position->w = 0.0f;
            voice = vars->sound_slot;
            if (voice != -1) {
                u8 *slot = (u8 *)D_0013E550 + voice * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && slot[0x74] != 0)
                    release_voice_slot(voice);
            }
            vars->sound_slot = -1;
            if (vars->arrival_sound != -1)
                allocate_voice_for_target_entry(vars->arrival_sound, 0, moby);
            if (vars->flag_index != -1)
                D_L07_00184528[vars->flag_index] = 0;
            moby->state = 1;
            break;
        }
        step.q = 0;
        step.f[1] = step_size;
        rotated.q = step.q;
        if (moby->oclass == 0x3f5 || moby->oclass == 0x429)
            rotated.f[1] = -rotated.f[1];
        transform_vector_by_basis(&step, &rotated, &moby->rot_row0);
        fast_vec_add(&moby->pos, &moby->pos, &step);
        break;
    case 5:
        break;
    }
}
