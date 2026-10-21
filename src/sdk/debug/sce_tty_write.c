#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/data_labels.h"

struct TtyState {
    volatile s32 unk0; /* deci2 handle from sceDeci2Open */
    volatile s32 unk4; /* published payload length */
    volatile s32 unk8;
    volatile s32 unkC; /* busy flag, also written by sceTtyHandler */
    s32 unk10;         /* MMIO block pointer (with 0x20000000 flag bit) */
    s32 unk14;
    s32 unk18;
};

struct Mmio {
    u16 unk0; /* transmit length */
    u16 unk2;
    u16 unk4;
    u8 unk6;
    s8 unk7; /* CallDebugCharacter mode byte (read with `lb`) */
    u32 unk8;
    u8 data[0xF4]; /* payload window, addressed at block + 0xC */
};

extern struct TtyState D_00154A50;
extern s32 CallDebugCharacter(s32, s32);
extern s32 DIntr();
extern s32 EnableInterrupts();
extern void SceDeci2Poll(s32);

s32 sceTtyWrite(s8 *text, s32 text_len) {
    s32 written_count;  /* characters consumed -> return value */
    s32 remaining; /* countdown, hits -1 after len+1 tests */
    s32 window_bytes; /* bytes resident in the payload window */
    s8 *text_cursor; /* source cursor */
    struct Mmio *mmio;
    u8 *payload_cursor; /* payload cursor */

    written_count = 0;
    remaining = text_len;
    window_bytes = 0;
    text_cursor = text;
    if (D_00154A50.unkC == 0) {
        DIntr();
        D_00154A50.unkC = 1;
        mmio = (struct Mmio *)((u32)D_00154A80 | 0x20000000);
        D_00154A50.unk10 = (s32)mmio;
        payload_cursor = mmio->data;
        do {
            remaining = remaining - 1;
            if (remaining == -1) {
                break;
            }
            if (*text_cursor == 0xA) {
                *payload_cursor = 0xD;
                window_bytes = window_bytes + 1;
                payload_cursor = payload_cursor + 1;
                if (window_bytes >= 0x100) {
                    break;
                }
            }
            *payload_cursor = (u8)*text_cursor;
            window_bytes = window_bytes + 1;
            text_cursor = text_cursor + 1;
            payload_cursor = payload_cursor + 1;
            written_count = written_count + 1;
        } while (window_bytes < 0x100);
        D_00154A50.unk4 = window_bytes + 0xC;
        mmio->unk0 = D_00154A50.unk4;
        if (CallDebugCharacter(D_00154A50.unk0, mmio->unk7) < 0) {
            *(s32 *)&D_00154A50.unkC = 0;
            EnableInterrupts();
            return -1;
        }
        if (D_00154A50.unkC != 0) {
            do {
                SceDeci2Poll(D_00154A50.unk0);
            } while (D_00154A50.unkC != 0);
        }
        EnableInterrupts();
        return written_count;
    }
    return -1;
}
