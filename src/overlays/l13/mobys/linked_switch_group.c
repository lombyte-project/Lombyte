/* Level 13 linked switch group moby class (0xe7). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_alloc.h"

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

int process_linked_switch_damage(struct Moby *moby,
                                 char        *moby_data,
                                 float       *health) __asm__("FUN_L13_002e4300");

int process_linked_switch_damage(struct Moby *moby, char *moby_data, float *health) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_REMOVE = 2,
        HIT_REACTION_LOW_INTERPOLATION_TARGET = 3,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE = 4,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE = 5,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE = 6,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE = 7,
        HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE = 8,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET = 9,
        HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE = 10,
        HIT_REACTION_NO_HIT = 11
    };
    char *hit_record = find_moby_hit(moby, 0x330000, 0);
    int   hit_processing_status;
    int   result = 0;
    char *color_animation_state;
    switch (hit_reaction(moby, hit_record, health, 0, &hit_processing_status, 0, 0, 4)) {
    case HIT_REACTION_NONE:
        break;
    case HIT_REACTION_LETHAL:
        *(int *)health = 0;
        break;
    case HIT_REACTION_REMOVE:
        *(int *)health = 0;
        break;
    case HIT_REACTION_LOW_INTERPOLATION_TARGET:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_SECOND_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_THIRD_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_FOURTH_ALTERNATE:
    case HIT_REACTION_LOW_INTERPOLATION_TARGET_FIFTH_ALTERNATE:
    case HIT_REACTION_HIGH_INTERPOLATION_TARGET:
    case HIT_REACTION_HIGH_INTERPOLATION_TARGET_ALTERNATE:
    case HIT_REACTION_NO_HIT:
        break;
    }
    color_animation_state = moby_data + 0x60;
    if (hit_processing_status >= 2) {
        float          damage_amount = *(float *)(hit_record + 0x2C);
        unsigned short fl;
        if (*health <= damage_amount) {
            *(int *)health = 0;
            fl = moby->flags;
            moby->flags = fl & 0xEFFF;
            *(unsigned char *)(moby_data + 0x67) = 0x78;
            trooper_anim_update(moby, (short *)color_animation_state);
            result = 1;
        } else {
            *health = *health - damage_amount;
            *(unsigned char *)(moby_data + 0x67) = 0xFA;
            *(short *)(moby_data + 0x26) = scale_game_frames(0x3C);
            trooper_anim_update(moby, (short *)color_animation_state);
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, color_animation_state);
    return result;
}

extern void get_first_moby_in_group(char **, int, int, int) __asm__("FUN_L00_002591d0");

extern void get_next_moby_in_group(char **, char *, int, int) __asm__("FUN_L00_002592b8");

extern void set_moby_list_state(int, int) __asm__("FUN_L01_0026e0e0");

extern void debug_print(char *, short, short, int) __asm__("FUN_001e93b0");

char D_L13_001F4D90[] = "moby:%d(%d) opened at %d\n";

char D_L13_001F4DB0[] = "moby:%d(%d) failed at %d\n";

extern char linked_switch_closed_debug_format[] __asm__("D_L13_001F4DD0");

typedef struct {
    u8    pad0[0x20];
    float unk20;
    short unk24;
    u8    pad26[0x4A];
    short unk70;
    short unk72;
    short unk74;
    short unk76;
    u8    unk78;
    u8    unk79;
    u8    unk7A;
    u8    unk7B;
    int   unk7C;
    char *unk80;
    short unk84;
} SwitchVars;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void update_linked_switch_group(char *moby) __asm__("FUN_L13_002e4448");

void update_linked_switch_group(char *moby) {
    enum {
        INITIALIZE_LINKED_GROUP = 0,
        MONITOR_LINKED_GROUP_HITS = 1,
        WAIT_FOR_LINKED_GROUP_COMPLETION = 2,
        RESET_HIT_STATE = 3,
        UPDATE_OPENED_STATE = 4
    };
    SwitchVars *data = ((SwitchVars *)MOBY(moby)->pvars);
    int         hit = 0;

    if (data->unk74 == 0 && (unsigned char)(moby[0x20] - 1) < 2) {
        hit = process_linked_switch_damage((struct Moby *)moby, data, &data->unk20);
    }
    switch ((unsigned char)moby[0x20]) {
    case INITIALIZE_LINKED_GROUP:
        if (data->unk79 == 0) {
            char *iter[4];
            char *linked;

            data->unk7B = 0;
            data->unk80 = moby;
            get_first_moby_in_group(iter, (unsigned char)moby[0x21], 0, 0);
            while ((linked = *iter) != 0) {
                linked = iter[0];
                if (linked != moby && *(short *)(linked + 0xA6) == 0xE7) {
                    SwitchVars *other_data = *(SwitchVars **)(linked + 0x78);

                    other_data->unk80 = moby;
                    other_data->unk79 = 1;
                }
                data->unk7B++;
                get_next_moby_in_group(iter, iter[0], 0, 0);
            }
        }
        data->unk70 = 0;
        data->unk72 = 0;
        data->unk7A = 0;
        data->unk74 = 0;
        data->unk76 = 0;

        set_moby_color(moby, 0x80, 0x80, 0x80);
        moby[0x20] = 1;
        break;
    case MONITOR_LINKED_GROUP_HITS:
    case WAIT_FOR_LINKED_GROUP_COMPLETION:
        if (data->unk74 == 0 && hit != 0) {
            char       *linked;
            SwitchVars *linked_data;
            linked = data->unk80;
            linked_data = *(SwitchVars **)(linked + 0x78);
            if ((unsigned char)linked[0x20] == 1) {
                data->unk7A = 0;
                linked[0x20] = 2;
                if (linked_data->unk7C > 0) {
                    linked_data->unk70 = scale_game_frames(data->unk7C);
                }
            }
            linked_data->unk7A++;
            allocate_voice_for_target_entry(3, 0, moby);
            set_moby_color(moby, 0, 0xFF, 0);
            data->unk74 = 1;
        }
        if ((unsigned char)moby[0x20] == 2) {
            if (data->unk7A >= data->unk7B) {
                set_moby_list_state((unsigned char)moby[0x21], 4);
                allocate_voice_for_target_entry(1, 0, moby);
                data->unk70 = data->unk84;
                debug_print(
                    D_L13_001F4D90, ((short)MOBY(moby)->save_id), MOBY(moby)->oclass, frame_count);
                moby[0x20] = 4;
            } else {
                if (data->unk7C >= 0) {
                    if (fast_dec_timer_s16(&data->unk70) != 0) {
                        data->unk7A = 0;
                        data->unk74 = 0;

                        allocate_voice_for_target_entry(2, 0, moby);
                        set_moby_list_state((unsigned char)moby[0x21], 3);
                        debug_print(D_L13_001F4DB0,
                                    ((short)MOBY(moby)->save_id),
                                    MOBY(moby)->oclass,
                                    frame_count);
                        moby[0x20] = 3;
                    } else if (fast_dec_timer_s16(&data->unk72) != 0) {
                        allocate_voice_for_target_entry(0, 0, moby);
                        data->unk72 =
                            MIN(scale_game_frames(0x2D),
                                scale_game_frames(5) + data->unk70 / scale_game_frames(0x28));
                    }
                }
            }
        }
        break;
    case RESET_HIT_STATE:
        set_moby_color(moby, 0x80, 0x80, 0x80);

        data->unk7A = 0;
        data->unk74 = 0;
        data->unk20 = (float)data->unk24;
        moby[0x20] = 1;

        break;
    case UPDATE_OPENED_STATE:
        if (data->unk79 == 0 && data->unk70 != -1) {
            if (fast_dec_timer_s16(&data->unk70) != 0) {
                data->unk7A = 0;
                data->unk74 = 0;

                allocate_voice_for_target_entry(2, 0, moby);
                set_moby_list_state((unsigned char)moby[0x21], 3);
                debug_print(linked_switch_closed_debug_format,
                            ((short)MOBY(moby)->save_id),
                            MOBY(moby)->oclass,
                            frame_count);
                moby[0x20] = 3;
            } else if (fast_dec_timer_s16(&data->unk72) != 0) {
                allocate_voice_for_target_entry(0, 0, moby);
                data->unk72 = MIN(scale_game_frames(0x2D),
                                  scale_game_frames(5) + data->unk70 / scale_game_frames(0x28));
            }
        }
        if (fast_dec_timer_s16(&data->unk76) != 0) {
            data->unk76 = scale_game_frames(0x14);
            if (data->unk78 != 0) {
                set_moby_color(moby, 0, 0xFF, 0);
                data->unk78 = 0;
            } else {
                set_moby_color(moby, 0, 0, 0xFF);
                data->unk78 = 1;
            }
        }
        break;
    }
}
