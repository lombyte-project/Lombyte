#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00225e70/FUN_00225e70.s", FUN_00225e70);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"

typedef struct {
    s32 active;
    s32 id;
    s32 mode;
    s32 unkC;
    s32 other;
    s32 anim;
    s32 cls0;
    s32 anim0;
    s32 cls1;
    s32 anim1;
    s32 cls2;
    s32 anim2;
    s32 unk30;
    s32 unk34;
} Light;

typedef struct {
    u8 pad0[0x30];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u8 pad3C[0x64];
    s32 buf[7];
    s32 unkBC;
    u8 padC0[8];
    u8 slot[2];
    u8 cur;
    u8 pending;
    u32 base;
} StreamState;

typedef struct {
    u8 pad0[0x10];
    u128 pos;
    u8 pad20[0xC];
    f32 scale;
    u8 pad30[4];
    s16 unk34;
    u8 pad36[0xA];
    u128 rot;
    u8 pad50[0x24];
    void *update;
    u8 pad78[0x2E];
    s16 oclass;
} Moby;

typedef struct {
    s32 offset;
    s32 size;
} StreamEntry;

typedef struct {
    u8 pad0[0x18];
    StreamEntry entry[1];
} StreamTable;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} Flags;

extern s32 D_00160350;
extern s32 D_00160350_m __asm__("D_00160350") MACRO_ADDR;
extern Flags D_001516D0;
extern StreamState D_001D5BF0;
extern Light D_001D5EC0[];
extern Light D_001D6080;
typedef struct {
    u8 pad0[0x48];
    s32 ptr[1];
} ClassTbl;

extern u8 *D_001B3200[];
extern StreamTable D_00137B80;
typedef struct {
    u8 pad0[8];
    s32 type;
    u8 pad0C[0x40];
} ItemRec;

extern ItemRec D_001863D0[];
extern u8 D_00224B68[];

extern void func_0020B618();
extern s32 clear_record_flag_by_key(s32) __asm__("FUN_00225e20");
extern void relocate_asset_entry_pointers() __asm__("FUN_002032e0");
extern s32 get_stream_buffer_size(s32) __asm__("FUN_00225d88");
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");
extern void RaiseKernelTrap(void);
extern s32 FUN_00225dd8(s32);
extern s32 FUN_00226670(void);
extern void FUN_002267b8(void);
extern void FUN_00226848(s32, s32);
extern void blend_moby_animation_ex(Moby *, s32, s32, s32, s32) __asm__("FUN_002130d8");
extern void blend_moby_animation(Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern void set_moby_animation(Moby *, s32, s32) __asm__("FUN_00212ed8");
extern Moby *FUN_00225490(s32);
extern Moby *delete_moby(Moby *) __asm__("FUN_00225530");
extern void func_0020DEF8();

s32 FUN_00225e70(Moby *arg0, Moby *arg1, Moby *arg2, Moby **arg3, Moby **arg4, Moby **arg5)
{
    s32 held;
    s32 expired;
    s32 go;

    held = 0;
    expired = 0;
    go = 0;
    if (D_00160350_m == 0) {
        return 0;
    }
    if (*((u8 *)arg0 + 0x70) & 2) {
        held = 1;
        if (D_001D6080.unkC == 0 || --D_001D6080.unkC == 0) {
            expired = 1;
        }
    }
    if (D_001D5BF0.pending != 0 && D_001516D0.unk8 == 0) {
        s32 i = D_001D5BF0.pending - 1;
        s32 *p = &D_001D5BF0.buf[i];
        s32 v = *p;

        func_0020B618(v + D_001D5BF0.unkBC, v);
        D_001D5BF0.unkBC = 0;
        clear_record_flag_by_key(v);
        D_001D5BF0.slot[i] = D_001D5EC0[0].id;
        D_001D5EC0[0].active = 2;
        ((ClassTbl *)D_001B3200[0])->ptr[D_001D5EC0[0].id] = *p;
        relocate_asset_entry_pointers(D_001B3200[0], D_001D5EC0[0].id);
        D_001D5BF0.pending = 0;
    }
    if (D_001D5BF0.pending == 0 && D_001516D0.unk8 == 0 && D_00160350 > 0) {
        if (D_001D5EC0[0].active == 0) {
            u32 id = D_001D5EC0[0].id;
            StreamState *st = &D_001D5BF0;
            u8 *slot = st->slot;

            if (slot[st->cur] == id || slot[st->cur == 0] == id) {
                D_001D5EC0[0].active = 2;
            } else if (id < st->base) {
                D_001D5EC0[0].active = 3;
            } else {
                u32 k = id - st->base;
                s32 buf = st->buf[st->cur];
                s32 size = D_00137B80.entry[k].size << 11;
                s32 addr = buf + get_stream_buffer_size(buf) - size;

                st->unkBC = addr - buf;
                if (start_audio_stream_read(addr, D_00137B80.entry[k].offset, D_00137B80.entry[k].size) == 0) {
                    RaiseKernelTrap();
                }
                FUN_00225dd8(buf);
                D_001D5EC0[0].active = 1;
                st->pending = st->cur + 1;
                slot[st->cur] = 0xFF;
            }
        }
    }
    if (D_00160350 > 0 && (D_001D5EC0[0].active == 2 || D_001D5EC0[0].active == 3)) {
        s32 other = D_001D5EC0[0].other;

        if (other != 0 && D_001D5BF0.unk30 != other && D_001D5BF0.unk38 != other
            && D_001D5BF0.unk34 != other) {
            go = 0;
            FUN_00226670();
        } else if (D_001D5EC0[0].mode == 0) {
            go = 1;
        } else if (D_001D5EC0[0].mode == 1 && held) {
            go = 1;
        } else if (D_001D5EC0[0].mode == 2 && expired) {
            go = 1;
        } else if (D_001D5EC0[0].mode == 3) {
            go = 0;
            FUN_00226670();
        }
    }
    if (go) {
        D_001D6080 = D_001D5EC0[0];
        FUN_00226670();
        FUN_002267b8();
        FUN_00226848(D_001D6080.unk30, D_001D6080.unk34);
        blend_moby_animation_ex(arg0, D_001D6080.id, 0, 10, 5);
        if (D_001D6080.id == D_001D5BF0.slot[0]) {
            D_001D5BF0.cur = 1;
        } else if (D_001D6080.id == D_001D5BF0.slot[1]) {
            D_001D5BF0.cur = 0;
        }
        if (D_001863D0[D_001D6080.other].type == 2) {
            if (arg2 != 0) {
                blend_moby_animation_ex(arg2, D_001D6080.anim, 0, 10, 5);
            }
            if (arg1 != 0) {
                set_moby_animation(arg1, 1, 0);
            }
        } else if (arg1 != 0) {
            blend_moby_animation_ex(arg1, D_001D6080.anim, 0, 10, 5);
        }
        *arg3 = delete_moby(*arg3);
        *arg4 = delete_moby(*arg4);
        *arg5 = delete_moby(*arg5);
        if (D_001D6080.cls0 != -1) {
            if ((*arg3 = FUN_00225490(D_001D6080.cls0)) != 0) {
                Moby *m;
                set_moby_animation(*arg3, D_001D6080.anim0, 0);
                blend_moby_animation(*arg3, D_001D6080.anim0, 0, 10);
                (*arg3)->unk34 = 0;
                m = *arg3;
                qcopy(&m->pos, &arg0->pos);
                qcopy(&m->rot, &arg0->rot);
                if (m->oclass == 0x4A) {
                    m->scale *= 3.0f;
                }
                func_0020DEF8(*arg3);
                (*arg3)->update = D_00224B68;
            }
        }
        if (D_001D6080.cls1 != -1) {
            if ((*arg4 = FUN_00225490(D_001D6080.cls1)) != 0) {
                Moby *m;
                set_moby_animation(*arg4, D_001D6080.anim1, 0);
                blend_moby_animation(*arg4, D_001D6080.anim1, 0, 10);
                (*arg4)->unk34 = 0;
                m = *arg4;
                qcopy(&m->pos, &arg0->pos);
                qcopy(&m->rot, &arg0->rot);
                if (m->oclass == 0x4A) {
                    m->scale *= 3.0f;
                }
                func_0020DEF8(*arg4);
                (*arg4)->update = D_00224B68;
            }
        }
        if (D_001D6080.cls2 != -1) {
            if ((*arg5 = FUN_00225490(D_001D6080.cls2)) != 0) {
                Moby *m;
                set_moby_animation(*arg5, D_001D6080.anim2, 0);
                blend_moby_animation(*arg5, D_001D6080.anim2, 0, 10);
                (*arg5)->unk34 = 0;
                m = *arg5;
                qcopy(&m->pos, &arg0->pos);
                qcopy(&m->rot, &arg0->rot);
                if (m->oclass == 0x4A) {
                    m->scale *= 3.0f;
                }
                func_0020DEF8(*arg5);
                (*arg5)->update = D_00224B68;
            }
        }
    }
    return 0;
}
#endif /* NON_MATCHING */
