#include "types.h"
typedef struct {
    u64 id;
    u64 type;
    s32 func;
    s32 data;
} StrCallback;

typedef struct {
    u8 pad0[0x44];
    StrCallback *callbacks;
    s32 count;
} MpegSys;

typedef struct {
    u8 pad0[0x40];
    MpegSys *sys;
} sceMpeg;

typedef struct {
    u64 unk0;
    u64 type;
} StrTypeEntry;

extern StrTypeEntry D_00132ED8[];
extern u64 _type2id(s32 strType, s32 ch);

s32 sceMpegAddStrCallback(sceMpeg *mp, s32 strType, s32 ch, s32 func, s32 data) {
    MpegSys *sys;
    StrCallback *cb;
    u64 id;
    s32 count;
    s32 i;
    s32 old;

    old = 0;
    sys = mp->sys;
    cb = sys->callbacks;
    id = _type2id(strType, ch);
    count = sys->count;
    for (i = 0; i < count; i++) {
        if (id == cb[i].id) {
            old = cb[i].func;
            break;
        }
    }
    if (i < 64) {
        sys->count = count + 1;
        cb[i].id = id;
        cb[i].data = data;
        cb[i].func = func;
        cb[i].type = D_00132ED8[strType].type;
    }
    return old;
}
