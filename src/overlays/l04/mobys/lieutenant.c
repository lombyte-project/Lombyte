/* Level 04 moby class lieutenant: the vendor that opens its shop camera when the hero comes close. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"

/* Vendor update: opens when the hero is close, sets up the shop camera in front of it once
 * its phase reaches 2, and closes when the slot is already used. */
struct VendorMobyView {
    u8   pad0[0x7F];
    u8   unk7F;
    u8   pad80[0x30];
    u8   slot; /* 0xB0 */
    u8   padB1[0xF];
    Vec4 facing; /* 0xC0 */
};

struct VendorVars {
    u8  pad0[4];
    s16 phase; /* 0x04 */
    u8  pad6[6];
    f32 alpha; /* 0x0C */
    u8  pad10[0x130];
    s32 item; /* 0x140 */
};

struct ShopCamera {
    Vec4 pos;
    Vec4 rot;
    u8   pad20[0x16];
    s16  active; /* 0x36 */
};

extern char       D_L04_00166F40[];
extern u8         D_0014C050[];
struct ShopCamera l04_lieutenant_shop_camera __asm__("D_L04_0016C9F0") = {0};

extern s32  D_L04_0015F5C4;
extern void FUN_L00_0025a120(struct Moby *);
extern int  FUN_00214720(char *, s32);
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

void lieutenant_update(struct Moby *moby) __asm__("FUN_L04_002e3078");

void lieutenant_update(struct Moby *moby) {
    enum {
        LIEUTENANT_STATE_INITIALIZE = 0,
        LIEUTENANT_STATE_UPDATE_DIALOGUE = 1,
        LIEUTENANT_STATE_UNLOCK_PLANET_AND_DISABLE = 2,
        LIEUTENANT_STATE_DISABLED = 3
    };
    Vec4               pos;
    Vec4               rot;
    Vec4              *rotation_ptr;
    struct VendorVars *vars = (struct VendorVars *)moby->pvars;

    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L04_00166F40) < 30.0f) {
        FUN_L00_0025a120(moby);
        ((struct VendorMobyView *)moby)->unk7F = 0x18;
    }
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    switch (moby->state) {
    case LIEUTENANT_STATE_INITIALIZE:
        if (D_0014C050[((struct VendorMobyView *)moby)->slot + current_level_index * 16] == 0xFF) {
            moby->state = 3;
            moby->flags |= 0x41;
            moby->collision = 0;
            break;
        }
        moby->state = 1;
        initialize_npc_dialogue(moby, vars);
        break;
    case LIEUTENANT_STATE_UPDATE_DIALOGUE:
        if (FUN_00214720(&hero.motion.pos, vars->item))
            vars->alpha = 255.0f;
        else
            vars->alpha = 1.0f;
        if (update_npc_dialogue(moby, vars))
            moby->state = 2;
        break;
    case LIEUTENANT_STATE_UNLOCK_PLANET_AND_DISABLE:
        if (vars->phase == 2) {
            unlock_planet(6);
            jet_clear_persistent(((struct VendorMobyView *)moby)->slot);
            fast_vec_normalize(&pos, &((struct VendorMobyView *)moby)->facing, 2.0f);
            fast_vec_add(&pos, &pos, &moby->pos);
            rotation_ptr = &rot;
            qcopy(rotation_ptr, &moby->rot);
            rot.f[2] = fast_add_rotations(rot.f[2], 3.14159f);
            qcopy(&l04_lieutenant_shop_camera.pos, &pos);
            qcopy(&l04_lieutenant_shop_camera.rot, rotation_ptr);
            l04_lieutenant_shop_camera.active = 1;
            memcard_save_data(0, -1);
            moby->state = 3;
            moby->flags |= 0x41;
            moby->collision = 0;
            break;
        }
        if (D_L04_0015F5C4 != 2)
            update_npc_dialogue(moby, vars);
        break;
    case LIEUTENANT_STATE_DISABLED:
        moby->flags |= 0x41;
        moby->collision = 0;
        break;
    }
}
