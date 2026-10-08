#include "types.h"
#include "sda.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/globals.h"

struct S {
    u8 pad_0[0x140];
    float f140, f144, f148;
};
struct P {
    u8 pad_0[0x10];
    u8 f10;
};
struct T {
    u8 pad_0[0x48];
    struct P *tbl[1];
};
struct O2 {
    u8 pad_0[0x10];
    float f10, f14, f18;
    u8 pad_1C[8];
    struct T *f24;
    u8 pad_28[0xC];
    u16 f34;
    u8 pad_36[0xA];
    s32 f40, f44, f48;
    u8 pad_4C[0x28];
    s32 *f74;
};
#include "rnc/ui/menus/menu_system.h"
extern struct MenuPage D_001D45C8; /* page made current and next at init */
extern u8 D_0019C150[];
extern u8 D_0019C160[];
extern s32 D_001940C0[];
extern s32 D_0015F438;
extern s32 D_00160F0C;
extern void FUN_0023a2c0();
extern struct S D_00186F40;
extern struct O2 *D_001D5D90[];
extern u8 D_001601C0 __attribute__((sda));
extern u8 D_001601D0 __attribute__((sda));
extern s32 FUN_001f9a68(s32, s32, f32);
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern s32 sceGsSyncV(s32);
extern s32 initialize_graphics_buffer_descriptors() __asm__("FUN_00225ac0");
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern struct O2 *create_menu_preview_moby() __asm__("func_00225490");
extern s32 set_moby_animation() __asm__("func_00212ED8");

void func_00225AC0__void() __asm__("FUN_00225ac0");
void FUN_00218f98(void) {
    s32 i;
    s32 a, b, c, d;
    menu_system.state = 2;
    menu_system.current = &D_001D45C8;
    menu_system.next = &D_001D45C8;
    menu_system.close_locked = 0;
    FUN_001f9a68((s32)D_0019C150, (s32)&D_001601C0, 1.0f);
    qcopy(D_0019C150 - 0x10, &D_001601D0);
    qzero(D_0019C150 + 0x20);
    qzero(D_0019C160);
    vu1_sync_chain(1);
    sceGsSyncV(0);
    D_0015F438++;
    a = D_001940C0[1] + 0xA0000;
    b = D_001940C0[2] + 0xA0000;
    c = a + 0x3C000;
    d = b + 0xE0000;
    menu_system.unkFC = c + 0xC1000;
    menu_system.unk100 = d + 0x11800;
    D_00160F0C = 0xA0000;
    menu_system.unk104 = a;
    menu_system.unk10 = b;
    menu_system.help_text_buffer = c;
    menu_system.unk10C = d;
    func_00225AC0__void(1);
    vu1_init_chain();
    menu_system.saved_texture_start = gs_texture_allocation_start;
    if (menu_system.current != 0) {
        for (i = 0; i < 14; i++) {
            struct O2 *o = (struct O2 *)create_menu_preview_moby(0x472);
            D_001D5D90[i] = o;
            if (o != 0) {
                s32 k;
                o->f34 &= 0xFFFD;
                D_001D5D90[i]->f74 = FUN_0023a2c0;
                D_001D5D90[i]->f10 = D_00186F40.f140;
                D_001D5D90[i]->f14 = D_00186F40.f144;
                D_001D5D90[i]->f18 = D_00186F40.f148;
                D_001D5D90[i]->f40 = 0;
                D_001D5D90[i]->f44 = 0;
                D_001D5D90[i]->f48 = 0;
                k = menu_system.current->moby_anims[i];
                set_moby_animation(D_001D5D90[i], k, D_001D5D90[i]->f24->tbl[k]->f10 - 1);
            }
        }
    }
}

extern __typeof__(FUN_00218f98) func_00218F98 __attribute__((alias("FUN_00218f98")));
