/* Shared moby class codebot: the collectible codebot. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/game/item_state.h"

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void initialize_collectible_sprite_state(char *) __asm__("FUN_L02_002ea7d0");
extern void update_collectible_sprite_effect(char *) __asm__("FUN_L02_002ea8b0");

extern int   D_L15_0015F5C4;
extern float D_L15_001621A0 __attribute__((sda));

float D_L17_001623A0 = 1.0f;

/* Update for moby class 1428: a four-state trigger that spins and then deletes itself. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002ED168), where it is exact; names translated to the US level program. */
void codebot_update(struct Moby *moby) __asm__("FUN_L15_002ebd78");

void codebot_update(struct Moby *moby) {
    enum {
        CODEBOT_INITIALIZE = 0,
        CODEBOT_CHECK_HERO_PROXIMITY = 1,
        CODEBOT_RECORD_COLLECTION = 2,
        CODEBOT_REMOVE = 3
    };
    unsigned char *collection_flags;
    if (current_level_index != 0xF) {
        moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
        moby->scale = moby->pclass->scale * D_L15_001621A0;
        switch (moby->state) {
        case CODEBOT_INITIALIZE:
            initialize_collectible_sprite_state(moby);
            if (alternate_item_available[2] != 0) {
                mark_moby_for_removal(moby);
            } else {
                moby->state = 1;
                moby->pos.z += 1.0f;
            }
            break;
        case CODEBOT_CHECK_HERO_PROXIMITY:
            update_collectible_sprite_effect(moby);
            if (fast_vec_dist(&moby->pos, &hero.motion.pos) < 3.0f) {
                if (hero.state.current != 0x32) {
                    moby->flags |= 0x41;
                    start_scene(1);
                    moby->state = 2;
                }
            }
            break;
        case CODEBOT_RECORD_COLLECTION:
            if (D_L15_0015F5C4 != 2) {
                queue_dialogue_message(0x426A, -1);
                D_L15_0015F640 = scale_game_frames(0xB4);
                collection_flags = alternate_item_available;
                collection_flags[2] = 1;
                collection_flags[0x78] = 1;
                moby->state = 3;
                memcard_save_data(0, -1);
            }
            break;
        case CODEBOT_REMOVE:
            mark_moby_for_removal(moby);
            break;
        }
    }
}
