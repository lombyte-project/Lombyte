/* Mobys: updates every moby under the profiler. */
#include "types.h"
#include "asm.h"
#include "sda.h"

#include "rnc/mobys/moby.h"
#include "moby_update.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

typedef struct {
    s32 count;
    s32 ticks;
} MobyProfile;

#include "rnc/input/pad_globals.h"

extern s32          D_L00_0015F3F8;
extern struct Moby *D_L00_0015FFE4;
extern s16          D_L00_00197A00[];
extern char         D_L00_00160118[];
extern char         D_L00_001E90B0[];
extern char         D_L00_001E90C8[];

extern void         update_all_mobys(void) __asm__("FUN_L00_00264848");
extern void         FUN_L00_001eb078(const char *, ...);
extern void         FUN_L00_001ff040(void *, s32, s32);
extern struct Moby *FUN_L00_00250768(void);
extern void         FUN_L00_00250480(struct Moby *);
extern f32          FUN_L00_00200240(s32);
extern s32          fptodp(f32) __asm__("FUN_00120478");

void update_all_mobys_profiled(void) __asm__("FUN_L00_002648e0");

void update_all_mobys_profiled(void) {
    struct Moby *moby;
    MobyProfile *profile;
    s32          i;
    s32          next_ticks;
    s32          current_ticks;
    f32          total_ticks;
    f32          share;
    s32          scaled_ticks;
    s32          count;
    s32         *entry;
    s16          profile_label;

    if ((*(u64 *)pad_held & 0x800000007LL) != 0x800000007LL) {
        update_all_mobys();
        return;
    }

    FUN_L00_001eb078(D_L00_00160118);
    FUN_L00_001eb078(D_L00_001E90B0, D_L00_0015F3F8);
    FUN_L00_001eb078(D_L00_00160118);
    *(volatile u32 *)0x10000010 = 0x81;
    FUN_L00_001ff040((void *)0x11004000, 0, 0x700);

    moby = D_L00_0015FFE4 = FUN_L00_00250768();
    while (moby != 0) {
        s32 current_ticks;
        if ((s8)moby->state >= 0) {
            *(volatile u32 *)0x10000000 = 0;
            if (!(moby->flags & 0x40)) {
                FUN_L00_00250480(moby);
            }
            MOBY_CALL_UPDATE(moby->update, moby);
            if (!(((volatile struct Moby *)moby)->flags & 4)) {
                update_entity(moby);
            }
            current_ticks = *(volatile s32 *)0x10000000;
            entry = (s32 *)0x11004000 + (moby->class_slot * 2);
            entry[0] = entry[0] + 1;
            entry[1] = entry[1] + current_ticks;
        }
        moby = moby->next;
    }

    /* The predecrement scan includes all 224 profile entries. */
    {
        s32 i;
        total_ticks = 0.0f;
        profile = (MobyProfile *)0x11004000;
        i = 0xdf;
        do {
            i--;
            if (profile->count != 0) {
                total_ticks += (f32)profile->ticks;
            }
            profile++;
        } while (i >= 0);
    }

    current_ticks = 100000000;
    do {
        next_ticks = 0;
        profile = (MobyProfile *)0x11004000;
        for (i = 0; i < 0xe0; i++, profile++) {
            if (profile->count != 0) {
                if (profile->ticks == current_ticks) {
                    count = profile->count;
                    scaled_ticks = (current_ticks << 5) / count;
                    profile_label = D_L00_00197A00[i];
                    share = FUN_L00_00200240(current_ticks) / total_ticks;
                    share = share * 100.0f;
                    FUN_L00_001eb078(
                        D_L00_001E90C8, profile_label, profile->count, scaled_ticks, fptodp(share));
                    profile->count = 0;
                } else if (next_ticks < profile->ticks) {
                    next_ticks = profile->ticks;
                }
            }
        }
        current_ticks = next_ticks;
    } while (next_ticks != 0);
}

s16 D_L01_00197E80  NOT_SDA = 0;
char                D_L01_00160118 = 0xa;
char D_L01_0020A110 NOT_SDA = 0x2a;
char D_L01_0020A128 NOT_SDA = 0x63;
s16 D_L02_00198000  NOT_SDA = 0;
char                D_L02_00160118 = 0xa;
char D_L02_001F99B0 NOT_SDA = 0x2a;
char D_L02_001F99C8 NOT_SDA = 0x63;
s16 D_L03_00197B00  NOT_SDA = 0;
char                D_L03_00160118 = 0xa;
char D_L03_001E1D30 NOT_SDA = 0x2a;
char D_L03_001E1D48 NOT_SDA = 0x63;
s16 D_L04_00197B80  NOT_SDA = 0;
char                D_L04_00160118 = 0xa;
char D_L04_001DAE50 NOT_SDA = 0x2a;
char D_L04_001DAE68 NOT_SDA = 0x63;
s16 D_L05_00197E80  NOT_SDA = 0;
char                D_L05_00160118 = 0xa;
char D_L05_002177C0 NOT_SDA = 0x2a;
char D_L05_002177D8 NOT_SDA = 0x63;
char                D_L06_00160118 = 0xa;
char D_L06_002004B0 NOT_SDA = 0x2a;
char D_L06_002004C8 NOT_SDA = 0x63;
char                D_L07_00160118 = 0xa;
char D_L07_00210350 NOT_SDA = 0x2a;
char D_L07_00210368 NOT_SDA = 0x63;
s16 D_L08_00198200  NOT_SDA = 0;
char                D_L08_00160118 = 0xa;
char D_L08_001FAFF0 NOT_SDA = 0x2a;
char D_L08_001FB008 NOT_SDA = 0x63;
s16 D_L09_00197B80  NOT_SDA = 0;
char                D_L09_00160118 = 0xa;
char D_L09_00207BC0 NOT_SDA = 0x2a;
char D_L09_00207BD8 NOT_SDA = 0x63;
s16 D_L10_00197E80  NOT_SDA = 0;
char                D_L10_00160118 = 0xa;
char D_L10_001DBE30 NOT_SDA = 0x2a;
char D_L10_001DBE48 NOT_SDA = 0x63;
s16 D_L11_00198400  NOT_SDA = 0;
char                D_L11_00160118 = 0xa;
char D_L11_0021A200 NOT_SDA = 0x2a;
char D_L11_0021A218 NOT_SDA = 0x63;
s16 D_L12_00197E80  NOT_SDA = 0;
char                D_L12_00160118 = 0xa;
char D_L12_0020B590 NOT_SDA = 0x2a;
char D_L12_0020B5A8 NOT_SDA = 0x63;
s16 D_L13_00197D00  NOT_SDA = 0;
char                D_L13_00160118 = 0xa;
char D_L13_001F3B20 NOT_SDA = 0x2a;
char D_L13_001F3B38 NOT_SDA = 0x63;
s16 D_L14_00198100  NOT_SDA = 0;
char                D_L14_00160118 = 0xa;
char D_L14_001FB400 NOT_SDA = 0x2a;
char D_L14_001FB418 NOT_SDA = 0x63;
s16 D_L15_00198000  NOT_SDA = 0;
char                D_L15_00160118 = 0xa;
char D_L15_001E1C70 NOT_SDA = 0x2a;
char D_L15_001E1C88 NOT_SDA = 0x63;
s16 D_L16_00197E80  NOT_SDA = 0;
char                D_L16_00160118 = 0xa;
char D_L16_001E7380 NOT_SDA = 0x2a;
char D_L16_001E7398 NOT_SDA = 0x63;
s16 D_L17_00198300  NOT_SDA = 0;
char                D_L17_00160118 = 0xa;
char D_L17_001E5A70 NOT_SDA = 0x2a;
char D_L17_001E5A88 NOT_SDA = 0x63;
char                D_L18_00160118 = 0xa;
char D_L18_001F17E0 NOT_SDA = 0x2a;
char D_L18_001F17F8 NOT_SDA = 0x63;
