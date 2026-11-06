/* Moby groups: counts active members and sets the state of a moby list. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/overlay/shared/mobys/ground_move_control.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0026E8E0.c: func_L01_0026E8E0), where it is exact; names translated to the US level program. */

/* Sets the default ground movement tuning: radius 0.625, small steps and turns. */
void init_ground_move_control(struct GroundMoveControl *control) {
    control->collision_radius_1024ths = 0x280;
    control->step_height = 1.0f;
    control->max_drop = 0.5f;
    control->max_climb = 0.5f;
    control->turn_velocity = 0.0f;
    control->speed = 0.0f;
    control->vertical_velocity = 0.0f;
    control->max_speed = 2.7f;
    control->brake = 0.3f;
    control->acceleration = 0.3f;
    control->heading_tolerance = 1.57f;
    control->arrive_distance = 0.1f;
}

#include "sda.h"

typedef struct {
    char        pad0[0x20];
    signed char state;
    char        pad21[0xDF];
} O_26e008;

extern unsigned short *D_L01_001ABCC0_lui[] __asm__("D_L01_001ABCC0") NOT_SDA;
extern O_26e008       *D_L01_0015FFD8_26e008 __asm__("D_L01_0015FFD8");

int count_active_moby_group_members(int i, int st) __asm__("FUN_L01_0026e008");

int count_active_moby_group_members(int i, int excluded_moby_state) {
    enum {
        MIN_ACTIVE_MOBY_STATE = 0
    };
    unsigned short *moby_index_entry;
    int             qualifying_moby_count = 0;
    O_26e008       *moby;

    if (D_L01_001ABCC0_lui[i] == 0) {
        return 0;
    }
    moby_index_entry = D_L01_001ABCC0_lui[i];
    do {
        moby = (O_26e008 *)(((*moby_index_entry & 0x7FFF) << 8) + (int)D_L01_0015FFD8_26e008);
        if (moby->state >= MIN_ACTIVE_MOBY_STATE &&
            (excluded_moby_state == -1 || (unsigned char)moby->state != excluded_moby_state)) {
            qualifying_moby_count++;
        }
    } while ((short)*moby_index_entry++ >= 0);
    return qualifying_moby_count;
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_0026E8E0.c: func_L01_0026F040), where it is exact; names translated to the US level program. */

extern char           *D_L01_0015FFD8;
extern unsigned short *D_L01_001ABCC0[];

void FUN_L01_0026e090(int i, int v);

void FUN_L01_0026e090(int i, int moby_byte_value) {
    unsigned short *moby_index_list = D_L01_001ABCC0[i];
    unsigned short *moby_index_entry;
    if (moby_index_list != 0) {
        moby_index_entry = moby_index_list;
        do {
            D_L01_0015FFD8[(*moby_index_entry & 0x7FFF) * 256 + 0xBC] = moby_byte_value;
        } while (*(short *)moby_index_entry++ >= 0);
    }
}

/* 0x0026e0e0, 88 bytes.
 * Ported from rac1-decomp (src/overlays/shared/mobyutil_0026E8E0.c:
 * func_L01_0026F090), where it is exact; names translated to the US level
 * program. The datum needs the unsplit macro form here, so it is declared
 * through an alias (same symbol, .sdata) beside its neighbour's declaration. */
void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");

void set_moby_list_state(int list, int state) {
    unsigned short *moby_index_entry;
    unsigned short *moby_index_list = (unsigned short *)D_L01_001ABCC0[list];

    if (moby_index_list != 0) {
        moby_index_entry = moby_index_list;
        do {
            char *moby = (char *)(((*moby_index_entry & 0x7FFF) << 8) + (int)ents);

            if (moby[0x20] >= 0) {
                moby[0x20] = state;
            }
        } while (*(short *)moby_index_entry++ >= 0);
    }
}
