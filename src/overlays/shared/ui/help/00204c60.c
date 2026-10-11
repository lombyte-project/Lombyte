/* Hero creation FUN_L00_00204c60, split out of 00203b18.c because only SN
   cc1 2.95.2 reproduces it (OVERLAY_SN_UNITS): the 0x17A680 loop keeps one
   pointer per cleared field, which the game compiler folds into one. */
#include "types.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"

extern Vec4 D_0013E090[2] NOT_SDA; /* camera start: position, rotation */
extern s32 D_00141660 NOT_SDA;
extern struct Moby *D_L00_0015FFD8; /* first moby */
extern struct Moby *D_L00_0015FFDC; /* end of the moby table */
extern s32 D_L00_00160540;
extern s32 D_0015EDA0; /* starting hit points */
extern struct HeroEase D_L00_0017A680[] NOT_SDA;
extern void memset_words(void *, s32, s32) __asm__("FUN_001f97e8");
extern f32 probe_ground_height(Vec4f *, s32, f32) __asm__("FUN_00213508");
extern void FUN_L00_002127b8(void);
extern void FUN_L00_00208820(void);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 random_int_between(s32, s32) __asm__("FUN_L00_00257b90");

/* Clears the hero, binds it to the first moby of class 0 (snapped to the
   ground, its position and yaw copied in, the camera start set from it unless
   D_L00_00160540), resets the 31 manipulator records and the eight sound
   voices, and sets the starting hit points and timers. The item ids are read
   into save[] before and after the clear and never written back. */
void FUN_L00_00204c60(void) {
    s32 save[7];
    s32 i;
    struct Moby *m;
    f32 z;

    for (i = 0; i < 7; i++)
        save[i] = hero.items[i].item_id;
    memset_words(&hero, 0, 0x2310);
    for (i = 0; i < 7; i++)
        save[i] = hero.items[i].item_id;

    for (i = 0; &D_L00_0015FFD8[i] < D_L00_0015FFDC; i++) {
        m = &D_L00_0015FFD8[i];
        if (m->oclass == 0) {
            hero.moby = m;
            z = probe_ground_height(&m->pos, 0, 0.5f);
            if (z > 0.0f)
                hero.moby->pos.z = z;
            hero.moby->flags |= 2;
            hero.unkA88 = (u8 *)hero.moby;
            hero.motion.rot.f[2] = hero.moby->rot.z;
            qcopy(&hero.motion.pos, &hero.moby->pos);
            FUN_L00_002127b8();
            FUN_L00_00208820();
            if (D_L00_00160540 == 0) {
                qcopy(&D_0013E090[0], &hero.motion.pos);
                qcopy(&D_0013E090[1], &hero.motion.rot);
            }
            break;
        }
    }
    if (D_00141660 == 0)
        D_00141660 = 10;

    for (i = 0; i < 31; i++) {
        D_L00_0017A680[i].attached = 0;
        clear_u64_value(&D_L00_0017A680[i].rot_target);
        clear_u64_value(&D_L00_0017A680[i].rot);
        clear_u64_value(&D_L00_0017A680[i].rot_vel);
        clear_u64_value(&D_L00_0017A680[i].pos_target);
        clear_u64_value(&D_L00_0017A680[i].pos);
        clear_u64_value(&D_L00_0017A680[i].pos_vel);
    }
    /* the eight voice slots unk2218..unk2234 as one array, indexed from the hero base */
    for (i = 0; i < 8; i++)
        ((struct { u8 pad[0x2218]; s32 v[8]; } *)&hero)->v[i] = -1;
    hero.health.hp = D_0015EDA0;
    hero.reach_distance = 2.125f;
    hero.reach_height = 1.25f;
    hero.health.unk22AC = 4;
    hero.health.unk22B0 = hero.health.hp;
    hero.unk1010 = random_int_between(scale_game_frames(0xB4), scale_game_frames(0x12C));
    hero.unk1014 = 0.007f;
    hero.unk1018 = 0.3f;
    hero.unkD20 = 0.97f;
}
