/* Level 13 moby state relay class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/game/item_state.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern int debug_print_alt_q() __asm__("FUN_001e93b0");

extern char D_L13_001F51B0[] NOT_SDA;

void moby_state_relay_update(struct Moby *moby) __asm__("FUN_L13_002f8880");

/* Moby class 667 */
void moby_state_relay_update(struct Moby *moby) {
    enum {
        STATE_RELAY_INITIALIZE = 0,
        STATE_RELAY_WAIT_FOR_SECOND_STATE_SET = 1,
        STATE_RELAY_WAIT_FOR_FIRST_STATE_SET = 2
    };
    char        *relay_state_and_config = ((char *)moby->pvars);
    struct Moby *monitored_moby = 0;
    int          monitored_moby_index = *(int *)relay_state_and_config;
    int          found;
    int          i;
    if (monitored_moby_index != -1) {
        monitored_moby = moby_base + (monitored_moby_index << 8);
    }
    switch (relay_state_and_config[0x1C]) {
    case STATE_RELAY_INITIALIZE:
        if (monitored_moby == 0) {
            debug_print_alt_q(D_L13_001F51B0, ((short)moby->save_id), moby->oclass);
            mark_moby_for_removal(moby);
        } else if (relay_state_and_config[0x1D] != 0) {
            relay_state_and_config[0x1C] = 1;
        } else {
            relay_state_and_config[0x1C] = 2;
        }
        break;
    case STATE_RELAY_WAIT_FOR_SECOND_STATE_SET:
        found = 0;
        for (i = 0; i < 6; i++) {
            if ((unsigned char)((char)monitored_moby->state) == (relay_state_and_config + i)[0xA]) {
                found = 1;
                break;
            }
        }
        if (found) {
            int idx2 = *(int *)(relay_state_and_config + 0x10);
            if (idx2 != -1) {
                char *o = *(char **)&moby_base + (idx2 << 8);
                if (relay_state_and_config[0x15] != -1)
                    o[0x20] = relay_state_and_config[0x15];
                if (relay_state_and_config[0x17] != -1)
                    o[0xBC] = relay_state_and_config[0x17];
            }
            if (relay_state_and_config[0x19] != -1)
                moby->state = relay_state_and_config[0x19];
            if (relay_state_and_config[0x1B] != -1)
                moby->command = relay_state_and_config[0x1B];
            relay_state_and_config[0x1C] = 2;
        }
        break;
    case STATE_RELAY_WAIT_FOR_FIRST_STATE_SET:
        found = 0;
        for (i = 0; i < 6; i++) {
            if ((unsigned char)((char)monitored_moby->state) == (relay_state_and_config + i)[4]) {
                found = 1;
                break;
            }
        }
        if (found) {
            int idx2 = *(int *)(relay_state_and_config + 0x10);
            if (idx2 != -1) {
                char *o = *(char **)&moby_base + (idx2 << 8);
                if (relay_state_and_config[0x14] != -1)
                    o[0x20] = relay_state_and_config[0x14];
                if (relay_state_and_config[0x16] != -1)
                    o[0xBC] = relay_state_and_config[0x16];
            }
            if (relay_state_and_config[0x18] != -1)
                moby->state = relay_state_and_config[0x18];
            if (relay_state_and_config[0x1A] != -1)
                moby->command = relay_state_and_config[0x1A];
            relay_state_and_config[0x1C] = 1;
        }
        break;
    }
}
