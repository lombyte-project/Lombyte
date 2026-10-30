/* Mobys: records moby context bytes. */
#include "types.h"
#include "asm.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/pause_0029C2A0.c: func_L01_0029C2A0), where it is exact; names translated to the US level program. */

typedef struct {
    int            key;
    int            pad4;
    unsigned short a;
    short          flag;
    short          c;
    short          d;
} Entry;

typedef struct {
    int   n;
    Entry e[1];
} Table;

void record_moby_context_bytes(char  *source,
                               int    byte_count,
                               char  *context_moby,
                               int    mode,
                               Table *entry_table) __asm__("FUN_L01_0029b0a0");

void record_moby_context_bytes(

    char *source, int byte_count, char *context_moby, int mode, Table *entry_table) {
    short  v20 = ((short)MOBY(context_moby)->save_id);
    short  v19 = MOBY(context_moby)->oclass;
    short  key;
    int    i;
    Entry *e;
    Entry *ne;
    if (mode == 1) {
        key = source - context_moby;
    } else {
        key = (int)source - *(unsigned short *)(context_moby + 0x78);
    }
    for (i = 0; i < entry_table->n; i++) {
        e = &entry_table->e[i];
        if (e->key == key && e->a == byte_count && v20 == e->c && v19 == e->d) {
            mode = e->flag;
            if (mode) {
                copy_memory_aligned((int)&e->pad4, source, byte_count);
                return;
            }
        }
    }
    ne = &entry_table->e[entry_table->n];
    entry_table->n = entry_table->n + 1;
    ne->key = key;
    copy_memory_aligned((int)&ne->pad4, source, byte_count);
    ne->a = byte_count;
    ne->flag = mode;
    ne->c = v20;
    ne->d = v19;
}
