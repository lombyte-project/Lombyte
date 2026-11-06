/* Level 13 controller-linked gate moby class. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys.h"

extern int D_L13_0015FFD8;

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern char gate_missing_controller_format[] __asm__("D_L13_001F51E0");

extern int D_L13_0015FFD8;

extern int debug_print_alt() __asm__("FUN_001e93b0");

void controller_linked_gate_update(struct Moby *moby) __asm__("FUN_L13_002f8a78");

typedef struct ControllerLinkedGateVars {
    u8  pad_0[0x10];
    f32 fade;
    s32 point_offset;
    s32 start_closed;
} ControllerLinkedGateVars;

/* Moby classes 674, 677, 680 */
void controller_linked_gate_update(struct Moby *moby) {
    enum {
        GATE_STATE_INITIALIZE = 0,
        GATE_STATE_OPEN = 1,
        GATE_STATE_CLOSING = 2,
        GATE_STATE_CLOSED = 3,
        GATE_STATE_OPENING = 4
    };
    ControllerLinkedGateVars *gate_data = (ControllerLinkedGateVars *)moby->pvars;
    char                     *controller_moby = 0;
    int                       open;
    int                       close;
    int                       step;
    if (*(int *)gate_data != -1) {
        controller_moby = D_L13_0015FFD8 + (*(int *)gate_data << 8);
    }
    open = 0;
    close = 0;
    if (((unsigned char *)moby)[0x20] != 0) {
        int i;
        for (i = 0; i < 6; i++) {
            if (((unsigned char *)controller_moby)[0x20] == ((char *)gate_data + i)[4]) {
                open = 1;
                break;
            }
        }
        for (i = 0; i < 6; i++) {
            if (((unsigned char *)controller_moby)[0x20] == ((char *)gate_data + i)[0xA]) {
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
        if (gate_data->start_closed == 0) {
            gate_data->fade = 1.0f;
            moby->state = GATE_STATE_OPEN;
            moby->collision = moby->pclass->collision;
            moby->alpha = 0x14;
        } else {
            gate_data->fade = 0.0f;
            moby->state = GATE_STATE_CLOSED;
            moby->collision = 0;
            moby->alpha = 0;
        }
        break;
    case GATE_STATE_OPEN:
        if (close) {
            moby->collision = 0;
            moby->state = GATE_STATE_CLOSING;
        }
        break;
    case GATE_STATE_CLOSING:
        if (open) {
            moby->state = GATE_STATE_OPENING;
            moby->collision = moby->pclass->collision;
        }
        gate_data->fade -= frame_scale * 0.05f;
        if (gate_data->fade < 0.0f) {
            gate_data->fade = 0.0f;
            moby->state = GATE_STATE_CLOSED;
            moby->collision = 0;
            moby->visible = 0;
            moby->flags |= 1;
        }
        break;
    case GATE_STATE_CLOSED:
        if (open) {
            moby->state = GATE_STATE_OPENING;
            moby->collision = moby->pclass->collision;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
        }
        break;
    case GATE_STATE_OPENING:
        if (close) {
            moby->collision = 0;
            moby->state = GATE_STATE_CLOSING;
        }
        gate_data->fade += frame_scale * 0.05f;
        if (1.0f < gate_data->fade) {
            gate_data->fade = 1.0f;
            moby->state = GATE_STATE_OPEN;
            moby->collision = moby->pclass->collision;
        }
        break;
    }
    moby->alpha = truncate_float_to_s32(gate_data->fade * 20.0f);
    step = 0xC0;
    gate_data->point_offset += step;
    if (gate_data->point_offset > 0x1000) {
        gate_data->point_offset -= 0x1000;
        step -= 0x1000;
    } else if (gate_data->point_offset < 0) {
        gate_data->point_offset += 0x1000;
        step += 0x1000;
    }
    offset_moby_class_point_pairs(((unsigned char *)moby->pclass), step, 0);
}
