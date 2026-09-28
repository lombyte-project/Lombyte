#include "types.h"
typedef struct { s32 status; u8 pad4[0x138BC]; } VoData;
typedef struct { s32 pad0; VoData *data; volatile s32 write; volatile s32 count; s32 size; } VoBuf;
extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
void vo_buf_inc_count(VoBuf *f) __asm__("FUN_0023d210");

void vo_buf_inc_count(VoBuf *f) {
    DIntr();
    f->data[f->write].status = 2;
    f->count++;
    f->write = (f->write + 1) % f->size;
    EnableInterrupts();
}

extern __typeof__(vo_buf_inc_count) func_0023D210 __attribute__((alias("FUN_0023d210")));
