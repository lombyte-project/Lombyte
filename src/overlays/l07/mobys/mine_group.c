/* Level 07 moby class 0x367: mine group; places its group of mines along its path and moves them along it. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

/* Moves a moby along a path, easing between nodes and tracking the hero. */
extern char *D_L07_001B0530[];
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");

struct SpawnClassView {
    u8  pad0[0x10];
    u32 unk10; /* 0x10 */
    u8  pad14[0x10];
    f32 scale; /* 0x24 */
    u8  pad28[0x1C];
    u16 flags; /* 0x44 */
};

struct SpawnMobyView {
    u8  pad0[0x36];
    s16 unk36; /* 0x36 */
    u8  pad38[0x39];
    u8  unk71; /* 0x71 */
};

struct AnimTimer {
    f32 frames;
    s16 length;
};

extern void              update_moby_grids(struct Moby *, u32) __asm__("FUN_0020dc20");
extern void              copy_hero_moby_field_38(struct Moby *) __asm__("FUN_L00_0025d1b8");
extern struct AnimTimer *get_moby_target_info(struct Moby *) __asm__("FUN_002141f8");

/* resets a spawned moby from its class defaults and restarts its animation */
void reset_moby_from_class_defaults(struct Moby *moby) __asm__("FUN_L07_0030af40");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030C3E0), where it is exact; names translated to the US level program. */
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char  mine_group_not_found_message[] __asm__("D_L07_00211550");
extern char  D_L07_00211580[];
extern char  mine_group_extra_mines_message[] __asm__("D_L07_002115D0");
extern int   debug_print() __asm__("FUN_001e93b0");
extern int   get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int   get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern int   sample_path(void  *path,
                         void  *pos,
                         int   *segment,
                         float *distance,
                         float  step,
                         int    loop) __asm__("FUN_L00_0025d808");
extern int   select_point_index_by_distance(void *, void *, float) __asm__("FUN_L01_0028b510");
extern int   resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int);
extern void  jet_animate(void *, float) __asm__("FUN_L00_00259fe8");
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern void  set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");

/* Views of the mine group and mine pvars: a typed field store keeps retail's load order. */
typedef struct {
    char pad[0x74];
    int  group;
    int  child_count;
} MineGroupView;

typedef struct {
    char pad[0x90];
    int  segment;
    int  next_segment;
} MineSegmentView;

void        update_mine_group(struct Moby *moby) __asm__("FUN_L07_0030b000");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
void        set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern void set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
void        mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

char D_L07_00211580[] =
    "Moby 871 short on mines at %d in group %d compared to mine count in pVar %d\n";

void reset_moby_from_class_defaults(struct Moby *moby) {
    struct SpawnClassView *class_defaults;
    struct AnimTimer      *animation_timer;

    moby->state = 0;
    class_defaults = (struct SpawnClassView *)moby->pclass;
    moby->command = 0;
    moby->flags = class_defaults->flags;
    moby->scale = class_defaults->scale;
    moby->update_dist = 0xFF;
    moby->draw_dist = 0xFF;
    moby->visible = 1;
    update_moby_grids(moby, 0x80807F7F);
    ((struct SpawnMobyView *)moby)->unk36 = 0x7F80;
    moby->unk72 = 0xFF;
    moby->hit_slot = 0xFF;
    ((struct SpawnMobyView *)moby)->unk71 = 0xFF;
    moby->collision = ((struct SpawnClassView *)moby->pclass)->unk10;
    copy_hero_moby_field_38(moby);
    update_entity(moby);
    animation_timer = get_moby_target_info(moby);
    if (animation_timer != 0)
        animation_timer->frames = animation_timer->length;
}

void update_mine_group(struct Moby *moby) {
    enum {
        INITIALIZE_PATH_CHILDREN = 0x0,
        ADVANCE_PATH_CHILDREN = 0x1
    };
    enum {
        HIT_REACTION_NONE = 0x0,
        MINE_GROUP_CLEAR_HEALTH_RESPONSE = 0x1,
        MINE_GROUP_ALTERNATE_CLEAR_HEALTH_RESPONSE = 0x2,
        HIT_REACTION_START_KNOCKBACK = 0x3,
        HIT_REACTION_KNOCKBACK_RESPONSE = 0x4,
        HIT_REACTION_ALTERNATE_KNOCKBACK_RESPONSE = 0x5,
        HIT_REACTION_START_HIT_KNOCKBACK_RECOVERY = 0x6,
        HIT_REACTION_LAUNCH_HIT_KNOCKBACK_RECOVERY = 0x7,
        HIT_REACTION_ENTER_HIT_KNOCKBACK_RECOVERY = 0x8,
        HIT_REACTION_HIT_ANIMATION_RESPONSE = 0x9,
        HIT_REACTION_ALTERNATE_HIT_ANIMATION_RESPONSE = 0xA,
        HIT_REACTION_NO_ANIMATION_RESPONSE = 0xB
    };
    char  *data;
    char  *path;
    float *remaining;
    int    count;
    int    current;
    int    child;
    int    i;
    int    first;
    int    next;
    float  distance;
    float  position[4];
    char  *limit;
    int    result;

    if (!moby)
        return;
    data = (char *)moby->pvars;
    if (!data)
        return;
    remaining = (float *)(data + 0x20);
    if (*(unsigned char *)(data + 0x94)) {
        limit = find_moby_hit(moby, 0x230000, 0);
        result = resolve_moby_hit_response(moby, limit, remaining, 0, &count, 0, 0, 4);
        switch ((unsigned)result) {
        case HIT_REACTION_NONE:
        case HIT_REACTION_START_KNOCKBACK:
        case HIT_REACTION_KNOCKBACK_RESPONSE:
        case HIT_REACTION_ALTERNATE_KNOCKBACK_RESPONSE:
        case HIT_REACTION_START_HIT_KNOCKBACK_RECOVERY:
        case HIT_REACTION_LAUNCH_HIT_KNOCKBACK_RECOVERY:
        case HIT_REACTION_ENTER_HIT_KNOCKBACK_RECOVERY:
        case HIT_REACTION_HIT_ANIMATION_RESPONSE:
        case HIT_REACTION_ALTERNATE_HIT_ANIMATION_RESPONSE:
        case HIT_REACTION_NO_ANIMATION_RESPONSE:
            break;
        case MINE_GROUP_CLEAR_HEALTH_RESPONSE:
        case MINE_GROUP_ALTERNATE_CLEAR_HEALTH_RESPONSE:
            *remaining = 0.0f;
            break;
        }
        if (count > 1) {
            float step = *(float *)(limit + 0x2c);
            if (*remaining <= step) {
                *remaining = 0.0f;
                set_moby_list_state(*(int *)(data + 0x74), 3);
                clear_u64_value(position);
                spawn_explosion(moby,
                                position,
                                (void *)0,
                                0.0f,
                                0.0f,
                                10,
                                3,
                                16,
                                4.0f,
                                2.0f,
                                9.0f,
                                1.0f,
                                -1,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                mark_moby_for_removal(moby);
                return;
            }
            *remaining -= step;
            *(unsigned char *)(data + 0x67) = 0xfa;
            *(short *)(data + 0x26) = scale_game_frames(0x3c);
            trooper_anim_update(moby, (short *)(data + 0x60));
        }
        moby->hit_slot = 0xff;
    }
    trooper_anim_finish(moby, data + 0x60);
    if (*(int *)(data + 0x70) == -1)
        return;
    path = D_L07_001B0530[*(int *)(data + 0x70)];
    switch (moby->state) {
    case INITIALIZE_PATH_CHILDREN: {
        int j;
        int placed, follower;
        current = 0;
        next = 0;
        qcopy(position, path + 0x10);
        for (j = 0; j < *(int *)path; j++) {
            int offset = j * 16;
            *(float *)(path + offset + 0x1c) =
                fast_vec_dist(path + 0x10 + j * 16, path + 0x10 + ((j + 1) % *(int *)path) * 16);
        }
        if (get_first_moby_in_group(&current, *(int *)(data + 0x74), 1, 0)) {
            debug_print(mine_group_not_found_message, *(int *)(data + 0x74));
            return;
        }
        first = *(int *)(data + 0x90) = select_point_index_by_distance(&moby->pos, path, 0.0f);
        placed = 0;
        if (0 < ((MineGroupView *)data)->child_count)
            do {
                follower = *(int *)(current + 0x78);
                *(int *)(follower + 0xa0) = (int)moby;
                qcopy((void *)(current + 0x10), position);
                qcopy((void *)(follower + 0x80), position);
                qcopy((void *)(follower + 0x70), position);
                ((MineSegmentView *)follower)->segment = first;
                ((MineSegmentView *)follower)->next_segment = first + 1;
                if (get_next_moby_in_group(&current, current, 1, 0)) {
                    int child_count = *(int *)(data + 0x78);
                    if (placed < child_count - 1) {
                        debug_print(D_L07_00211580, placed + 1, *(int *)(data + 0x74), child_count);
                    } else if (placed == child_count - 1) {
                        debug_print(
                            mine_group_extra_mines_message, *(int *)(data + 0x74), child_count);
                    }
                    *(int *)(data + 0x78) = placed + 1;
                    break;
                }
                sample_path(path, position, &first, (float *)&next, *(float *)(data + 0x80), 1);
                placed++;
            } while (placed < *(int *)(data + 0x78));
        if (!*(unsigned char *)(data + 0x94)) {
            moby->collision = 0;
            moby->flags &= 0xefff;
        }
        if (!*(unsigned char *)(data + 0x95))
            moby->flags |= 1;
        moby->state = 1;
        break;
    }
    case ADVANCE_PATH_CHILDREN: {
        int cursor = 0;
        get_first_moby_in_group(&cursor, ((MineGroupView *)data)->group, 1, 0);
        for (i = 0; i < *(int *)(data + 0x78); i++) {
            child = *(int *)(cursor + 0x78);
            {
                float segment_distance =
                    fast_vec_dist((char *)child + 0x80, path + 0x10 + *(int *)(child + 0x90) * 16);
                float step = *(float *)(data + 0x7c) * frame_time;
                distance = segment_distance;
                sample_path(
                    path, (char *)child + 0x80, (int *)((char *)child + 0x90), &distance, step, 1);
            }
            *(int *)(child + 0x94) = *(int *)(child + 0x90) + 1;
            if (cursor == 0 || *(unsigned char *)(cursor + 0x20) == 0xfe ||
                *(unsigned char *)(cursor + 0x20) == 0xfd) {
                if (*(int *)(data + 0x84) != -1) {
                    fast_vec_sub((char *)child + 0x60, (char *)child + 0x80, (char *)cursor + 0x10);
                    qcopy((void *)(cursor + 0x10), (void *)(child + 0x80));
                    if (fast_dec_timer((int *)(child + 0x98)) &&
                        *(int *)(child + 0x90) == *(int *)(data + 0x90))
                        reset_moby_from_class_defaults((struct Moby *)cursor);
                }
            }
            get_next_moby_in_group(&cursor, cursor, 1, 0);
        }
        break;
    }
    }
    jet_animate(moby, 1.5f);
}
