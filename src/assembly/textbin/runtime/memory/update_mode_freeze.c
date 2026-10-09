#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/runtime/memory/update_mode_freeze/FUN_001fce28.s",
    FUN_001fce28);
#else

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct S_0013C940 {
    u8 pad_0[0x1A4];
    s32 unk1A4;
};
struct S_0013F350_2080 {
    u8 pad_0[0x31];
    u8 unk31;
    u8 pad_31[0x2];
    u16 unk34;
    u8 pad_36[0x5E];
    s32 unk94;
};
struct S_0013F350_890 {
    u8 pad_0[0xBC];
    u8 unkBC;
};
struct S_0013F350 {
    u8 pad_0[0x19C];
    s32 unk19C;
    u8 pad_1A0[0x6E0];
    s32 unk880;
    u8 pad_884[0xC];
    struct S_0013F350_890 *unk890;
    u8 pad_894[0x6];
    s16 unk89A;
    u8 pad_89C[0xD73];
    u8 unk160F;
    u8 pad_1610[0xA70];
    struct S_0013F350_2080 *unk2080;
};
struct S_00193300 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};
extern struct S_0013C940 D_0013C940;
#include "rnc/input/pad_state.h"
extern u16 D_0013E05A[];
extern struct S_0013F350 D_0013F350;
extern u8 D_00141050[];
extern s32 D_0015ED84;
extern s32 D_0015EE38;
extern s32 D_0015EE3C;
extern s32 mode_freeze_state __asm__("D_0015EEB0");
extern s32 mode_freeze_flags __asm__("D_0015EEB4");
extern s32 D_0015F5E8;
extern s32 D_0015F604;
extern s32 D_0015F648;
extern u8 D_0016034C;
extern struct S_00193300 D_00193300;
extern s32 D_0015EEB8[];
extern s32 D_001D5BF8[];
extern char sound_update() __asm__("func_0022CA50");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern void memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void func_001E93E8();
extern void func_001E9440(u8 *, u8 *, s32, s32);
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void set_animation_parameter(s32, s32) __asm__("func_001FF570");
extern void func_001FF768();
extern void load_and_initialize_level_chunk() __asm__("func_00209370");
extern void func_0022E188(s32);
extern void snd_continue_all_sounds_in_group(s32) __asm__("func_0012E418");
extern void func_00216088();
extern void snd_flush_sound_commands() __asm__("func_0012DC80");
void update_mode_freeze(void) __asm__("FUN_001fce28");

void update_mode_freeze(void) {
    sound_update();
    if (D_00193300.unk4 != 0) {
        D_00193300.unk4--;
    }
    switch (D_00193300.unk0) {
    case 5: {
        struct S_00193300 *st = &D_00193300;
        st->unk20++;
        if (scale_game_frames(0x5A) < st->unk20) {
            if (st->unk24 != 0) {
                st->unk24--;
            }
        }
        st = &D_00193300;
        if (scale_game_frames(0x78) < st->unk20) {
            if (controller_state.pressed & 0x40) {
                D_0015F604 = 0;
                if (D_0015ED84 == 1) {
                    memcard_save_data(0, -1);
                }
            }
        }
        break;
    }

    case 4:
        if (controller_state.pressed & 0x10) {
            struct S_0013F350_2080 *p = D_0013F350.unk2080;
            p->unk31 = 0;
            p->unk94 = 0;
            p->unk34 = (u16)(p->unk34 | 1);
            func_001E93E8();
            func_001E9440(((u8 *)(&D_0013F350)) + 0x1D00, ((u8 *)(&D_0013F350)) + 0x1D10, 0, 1);
            D_0015F604 = 0;
        } else if (controller_state.pressed & 0x40) {
            D_0015F604 = 0;
        }
        break;

    case 6:
        switch (D_00193300.unk1C) {
        case 0:
            if (D_00193300.unk4 != 0) {
                if ((--D_00193300.unk4) == 0) {
                    D_00193300.unk1C = 1;
                }
            } else {
                D_00193300.unk1C = 1;
            }
            break;

        case 1:
            if (controller_state.pressed & 0x10) {
                D_0015F604 = D_00193300.unk14;
            } else if (controller_state.pressed & 0x40) {
                fade_to_black(4);
                D_0016034C = 0;
                D_00193300.unk1C = 2;
                D_00193300.unk20 = scale_game_frames(0x258);
            }
            break;

        case 2:
            if ((D_00193300.unk20 == 0) || ((--D_00193300.unk20) == 0)) {
                D_0016034C = 1;
                D_00193300.unk1C = 3;
            } else if (controller_state.pressed & 0x10) {
                fade_to_black(4);
                D_0016034C = 1;
                D_0015F604 = D_00193300.unk14;
            } else if (controller_state.pressed & 0x40) {
                D_0016034C = 0;
                D_0015F604 = D_00193300.unk14;
            }
            break;

        case 3:
            if (controller_state.pressed & 0x40) {
                D_0015F604 = D_00193300.unk14;
            }
            break;

        default:
            D_0015F604 = D_00193300.unk14;
            break;
        }

        break;

    case 1:
        D_0015F648 = 2;
        if (controller_state.pressed & 0x10) {
            D_0015F604 = 0;
            D_0013F350.unk160F = (u8)(D_0013F350.unk160F | 1);
        } else if (controller_state.pressed & 0x40) {
            D_0015F604 = 0;
        }
        break;

    case 2:
        D_0015F648 = 2;
        if (controller_state.pressed & 0x40) {
            D_0015F604 = 0;
        }
        break;

    case 0:
        if (D_00193300.unk1C == 1)
            goto mode0_one;
        if (D_00193300.unk1C < 2) {
            if (D_00193300.unk1C == 0)
                goto mode0_zero;
            goto mode0_done;
        }
        if (D_00193300.unk1C < 4)
            goto mode0_countdown;
        goto mode0_done;
    mode0_zero:
        if (D_00193300.unk20 < 8) {
            D_00193300.unk20++;
        } else if (D_00193300.unk24 < 8) {
            D_00193300.unk24++;
        } else {
            D_00193300.unk1C = 1;
        }
        goto mode0_done;
    mode0_one:
        if (controller_state.pressed & 0x40) {
            D_00193300.unk1C = 2;
        } else if (controller_state.pressed & 0x820) {
            D_00193300.unk1C = 3;
        }
        goto mode0_done;
    mode0_countdown:
        {
            struct S_00193300 *st = &D_00193300;
            if (st->unk24 != 0) {
                st->unk24--;
            } else if (st->unk20 != 0) {
                st->unk20--;
            } else {
                if (st->unk1C == 2) {
                    if (D_0013F350.unk880 != (-1)) {
                        set_animation_parameter(D_0013F350.unk880, 0);
                        D_0013F350.unk880 = -1;
                    }
                    func_001FF768();
                    if (scale_game_frames(0x1068) < D_0013F350.unk19C) {
                        if (D_0015ED84 == 5) {
                            D_0015EE38++;
                        } else if (D_0015ED84 == 0x10) {
                            D_0015EE3C++;
                        }
                    }
                    D_0015F604 = 0;
                    func_001E9440(D_00141050, D_00141050 + 0x10, 0, 1);
                } else {
                    D_0015F604 = 0;
                    if (D_0013F350.unk89A >= 3) {
                        D_0013F350.unk89A = (s16)(((u16)D_0013F350.unk89A) - 1);
                        D_0013F350.unk890->unkBC = 3;
                    }
                }
            }
        }
    mode0_done:
        break;

    case 3:
        D_00193300.unk20++;
        if (scale_game_frames(0x1E) < D_00193300.unk20) {
            if (D_00193300.unk24 != 0) {
                D_00193300.unk24--;
            }
        }
        switch (mode_freeze_state) {
        case 2:
            if (D_00193300.unk4 == 0) {
                if (controller_state.pressed & 0x40) {
                    mode_freeze_flags &= ~1;
                    D_0015F604 = D_00193300.unk14;
                }
            }
            break;

        case 6:
            if (controller_state.pressed & 0x20) {
                mode_freeze_flags |= 8;
            } else if (controller_state.pressed & 0x10) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                if (D_0015F5E8 == 0) {
                    D_0015F604 = D_00193300.unk14;
                }
            }
            break;

        case 13:
            if (controller_state.pressed & 0x20) {
                mode_freeze_flags |= 0x10;
            } else if (controller_state.pressed & 0x10) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                if (D_0015F5E8 == 0) {
                    D_0015F604 = D_00193300.unk14;
                }
            }
            break;

        case 17:

        case 18:

        case 20:

        case 21:
            if (controller_state.pressed & 0x40) {
                mode_freeze_flags = mode_freeze_flags ^ 0x40;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                D_0015F604 = D_00193300.unk14;
            }
            break;

        case 16:
            D_0015F604 = D_00193300.unk14;
            D_001D5BF8[0] = D_00193300.unk18;
            break;

        case 12:
            if (mode_freeze_flags & 2) {
                break;
            }

        case 3:

        case 5:
            if (controller_state.pressed & 0x10) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                D_0015F604 = D_00193300.unk14;
            }
            break;

        case 19:
            if ((D_0015F5E8 != 0) && (mode_freeze_flags & 2)) {
                if (controller_state.pressed & 0x40) {
                    load_and_initialize_level_chunk();
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    mode_freeze_flags = mode_freeze_flags | 0x20;
                    func_0022E188(0);
                    D_0013E05A[0] = 1;
                } else if (controller_state.pressed & 0x10) {
                    mode_freeze_flags = mode_freeze_flags & (~2);
                    mode_freeze_flags = mode_freeze_flags & (~4);
                    D_0015F604 = D_00193300.unk14;
                }
            } else if (controller_state.pressed & 0x10) {
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                D_0015F604 = D_00193300.unk14;
            }
            break;

        case 23:

        case 24:
            if (controller_state.pressed & 0x40) {
                load_and_initialize_level_chunk();
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                mode_freeze_flags = mode_freeze_flags | 0x20;
                func_0022E188(0);
                D_0013E05A[0] = 1;
            } else if (controller_state.pressed & 0x10) {
                mode_freeze_flags |= 0x20;
                D_0015F604 = D_00193300.unk14;
            }
            break;

        case 4:
            if (D_0015F5E8 != 0) {
                if (mode_freeze_flags & 2) {
                    struct S_0013C940 *p = &D_0013C940;
                    if (p->unk1A4 & 0x40) {
                        load_and_initialize_level_chunk();
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        func_0022E188(0);
                        D_0013E05A[0] = 1;
                    }
                    if (p->unk1A4 & 0x10) {
                        mode_freeze_flags = mode_freeze_flags | 0x20;
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        D_0015F604 = D_00193300.unk14;
                    }
                } else if (mode_freeze_flags & 4) {
                    if (controller_state.pressed & 0x10) {
                        mode_freeze_flags = mode_freeze_flags | 0x20;
                        mode_freeze_flags = mode_freeze_flags & (~2);
                        mode_freeze_flags = mode_freeze_flags & (~4);
                        D_0015F604 = D_00193300.unk14;
                    }
                } else {
                    D_0015F604 = D_00193300.unk14;
                }
            } else if (controller_state.pressed & 0x10) {
                mode_freeze_flags = mode_freeze_flags | 0x20;
                mode_freeze_flags = mode_freeze_flags & (~2);
                mode_freeze_flags = mode_freeze_flags & (~4);
                D_0015F604 = D_00193300.unk14;
            }
            break;

        case 9:
            if (D_0015F5E8 != 0) {
                if (!(mode_freeze_flags & 6)) {
                    D_0015F604 = D_00193300.unk14;
                }
            }
            break;
        }

        break;
    }

    if (((u32)(D_0015F604 - 3)) >= 2U) {
        snd_continue_all_sounds_in_group(0x1D);
        func_00216088();
        snd_flush_sound_commands();
    }
}
#endif /* NON_MATCHING */
