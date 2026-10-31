/* Level 13 controller-linked gate moby class. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"

extern int D_L13_0015FFD8;

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern void offset_moby_class_point_pairs(void *, int, int) __asm__("FUN_L00_0024efb0");

extern char gate_missing_controller_format[] __asm__("D_L13_001F51E0");

extern int D_L13_0015FFD8;

extern int debug_print_alt() __asm__("FUN_001e93b0");

void controller_linked_gate_update(struct Moby *moby) __asm__("FUN_L13_002f8a78");

/* Moby classes 674, 677, 680 */
void controller_linked_gate_update(struct Moby *moby) {
    enum {
        GATE_STATE_INITIALIZE = 0,
        GATE_STATE_OPEN = 1,
        GATE_STATE_CLOSING = 2,
        GATE_STATE_CLOSED = 3,
        GATE_STATE_OPENING = 4
    };
    char *gate_data = ((char *)moby->pvars);
    char *controller_moby = 0;
    int   open;
    int   close;
    int   step;
    if (*(int *)gate_data != -1) {
        controller_moby = D_L13_0015FFD8 + (*(int *)gate_data << 8);
    }
    open = 0;
    close = 0;
    if (((unsigned char *)moby)[0x20] != 0) {
        int i;
        for (i = 0; i < 6; i++) {
            if (((unsigned char *)controller_moby)[0x20] == (gate_data + i)[4]) {
                open = 1;
                break;
            }
        }
        for (i = 0; i < 6; i++) {
            if (((unsigned char *)controller_moby)[0x20] == (gate_data + i)[0xA]) {
                close = 1;
                break;
            }
        }
    }
    switch (((unsigned char *)moby)[0x20]) {
    case GATE_STATE_INITIALIZE:
        if (controller_moby == 0) {
            debug_print_alt(gate_missing_controller_format, ((short)moby->save_id), moby->oclass);
            mark_moby_for_removal(moby);
            return;
        }
        set_moby_color(moby, 0x80, 0x80, 0x80);
        moby->flags |= 0xA08;
        if (*(int *)(gate_data + 0x18) == 0) {
            *(float *)(gate_data + 0x10) = 1.0f;
            moby->state = 1;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
            moby->alpha = 0x14;
        } else {
            *(float *)(gate_data + 0x10) = 0.0f;
            moby->state = 3;
            moby->collision = 0;
            moby->alpha = 0;
        }
        break;
    case GATE_STATE_OPEN:
        if (close) {
            moby->collision = 0;
            moby->state = 2;
        }
        break;
    case GATE_STATE_CLOSING:
        if (open) {
            moby->state = 4;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
        }
        *(float *)(gate_data + 0x10) -= frame_scale * 0.05f;
        if (*(float *)(gate_data + 0x10) < 0.0f) {
            *(float *)(gate_data + 0x10) = 0.0f;
            moby->state = 3;
            moby->collision = 0;
            moby->visible = 0;
            moby->flags |= 1;
        }
        break;
    case GATE_STATE_CLOSED:
        if (open) {
            moby->state = 4;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
            moby->visible = 1;
            moby->flags &= 0xFFFE;
        }
        break;
    case GATE_STATE_OPENING:
        if (close) {
            moby->collision = 0;
            moby->state = 2;
        }
        *(float *)(gate_data + 0x10) += frame_scale * 0.05f;
        if (1.0f < *(float *)(gate_data + 0x10)) {
            *(float *)(gate_data + 0x10) = 1.0f;
            moby->state = 1;
            moby->collision = *(int *)(((char *)moby->pclass) + 0x10);
        }
        break;
    }
    moby->alpha = truncate_float_to_s32(*(float *)(gate_data + 0x10) * 20.0f);
    step = 0xC0;
    *(int *)(gate_data + 0x14) += step;
    if (*(int *)(gate_data + 0x14) > 0x1000) {
        *(int *)(gate_data + 0x14) -= 0x1000;
        step -= 0x1000;
    } else if (*(int *)(gate_data + 0x14) < 0) {
        *(int *)(gate_data + 0x14) += 0x1000;
        step += 0x1000;
    }
    offset_moby_class_point_pairs(((unsigned char *)moby->pclass), step, 0);
}
