#include "types.h"
typedef void (*ExtFunc)(void *);
typedef struct {
    ExtFunc f[11];
} ExtTable;
extern ExtTable D_001538B8;
extern void _nextStartCode(void *);
extern u32 _peepBit(void *, s32);
extern void _flushBuf(void *, s32);
extern u32 _nextBit(void *, s32);

void _extensionAndUserData(void *d) {
    ExtTable tbl = D_001538B8;
    u32 code;
    u32 id;

    _nextStartCode(d);
    while ((code = _peepBit(d, 32)) == 0x1B5 || code == 0x1B2) {
        if (code == 0x1B5) {
            _flushBuf(d, 32);
            id = _nextBit(d, 4);
            id = id > 10 ? 0 : id;
            tbl.f[id](d);
            _nextStartCode(d);
        } else {
            _flushBuf(d, 32);
            _nextStartCode(d);
        }
    }
}
