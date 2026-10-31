/* Moby class 0xA6: watches four linked mobys and moves one that waits in state 0x50 to itself; removed after three hits. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"

typedef struct {
    int            w0;
    int            w4;
    int            w8;
    int            wC;
    int            idx[4];
    unsigned char *p[4];
} V_2b3458;

int  find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

int linked_moby_selection_delay_gp __asm__("D_L00_00161478") = 0;
int owned_record_cooldown_gp __asm__("D_L00_00161480") = 0;

int D_L01_001614A0 = 0;
int D_L01_001614A8 = 0;
int D_L02_00161448 = 0;
int D_L02_00161450 = 0;
int D_L03_00161458 = 0;
int D_L03_00161460 = 0;
int D_L04_00161448 = 0;
int D_L04_00161450 = 0;
int D_L05_00161498 = 0;
int D_L05_001614A0 = 0;
int D_L06_00161470 = 0;
int D_L06_00161478 = 0;
int D_L07_00161448 = 0;
int D_L07_00161450 = 0;
int D_L08_00161498 = 0;
int D_L08_001614A0 = 0;
int D_L09_00161500 = 0;
int D_L09_00161508 = 0;
int D_L10_00161458 = 0;
int D_L10_00161460 = 0;
int D_L11_001614D8 = 0;
int D_L11_001614E0 = 0;
int D_L12_00161458 = 0;
int D_L12_00161460 = 0;
int D_L13_00161528 = 0;
int D_L13_00161530 = 0;
int D_L14_00161660 = 0;
int D_L14_00161668 = 0;
int D_L15_00161600 = 0;
int D_L15_00161608 = 0;
int D_L16_00161508 = 0;
int D_L16_00161510 = 0;
int D_L17_00161558 = 0;
int D_L17_00161560 = 0;
int D_L18_001614E8 = 0;
int D_L18_001614F0 = 0;

void linked_moby_states_update(unsigned char *source_moby) __asm__("FUN_L00_002b3458");

/* Moby class 166 */
void linked_moby_states_update(unsigned char *source_moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_ACTIVE = 1
    };
    V_2b3458 *moby_vars = ((V_2b3458 *)MOBY(source_moby)->pvars);
    int       matching_owned_record, i;
    switch (source_moby[0x20]) {
    case STATE_INITIALIZE:
        source_moby[0x20] = 1;
        moby_vars->w4 = linked_moby_selection_delay_gp;
        moby_vars->w8 = 3;
        moby_vars->p[0] = moby_table + (moby_vars->idx[0] << 8);
        moby_vars->p[1] = moby_table + (moby_vars->idx[1] << 8);
        moby_vars->p[2] = moby_table + (moby_vars->idx[2] << 8);
        moby_vars->p[3] = moby_table + (moby_vars->idx[3] << 8);
        break;
    case STATE_ACTIVE:
        matching_owned_record = find_moby_hit(source_moby, 0x10000, 0);
        fast_dec_timer(&moby_vars->wC);
        if (matching_owned_record && moby_vars->wC == 0) {
            source_moby[0xA4] = 0xFF;
            moby_vars->wC = owned_record_cooldown_gp;
            if (--moby_vars->w8 <= 0) {
                mark_moby_for_removal(source_moby);
                return;
            }
        }
        goto test;
    found: {
        unsigned char *selected_object;
        moby_vars->p[i][0x20] = 0x51;
        selected_object = moby_vars->p[i];
        qcopy(selected_object + 0x10, source_moby + 0x10);
        selected_object[0xA4] = 0xFF;
        return;
    }
    test:
        if (moby_vars->p[0][0x20] == 0x50 || moby_vars->p[1][0x20] == 0x50 ||
            moby_vars->p[2][0x20] == 0x50 || moby_vars->p[3][0x20] == 0x50) {
            if (fast_dec_timer(&moby_vars->w4)) {
                moby_vars->w4 = linked_moby_selection_delay_gp;
                for (i = 0; i < 4; i++) {
                    if (moby_vars->p[i][0x20] == 0x50)
                        goto found;
                }
            }
        }
        break;
    }
}
