/* Level 13 linked switch group moby class (0xe7). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/world/level_labels.h"
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
    enum {
        HIT_PROCESSING_DAMAGE_THRESHOLD = 2
    };
    char *hit_record = find_moby_hit(moby, 0x330000, 0);
    int   hit_processing_status;
    int   health_depleted = 0;
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
    if (hit_processing_status >= HIT_PROCESSING_DAMAGE_THRESHOLD) {
        float          damage_amount = *(float *)(hit_record + 0x2C);
        unsigned short fl;
        if (*health <= damage_amount) {
            *(int *)health = 0;
            fl = moby->flags;
            moby->flags = fl & 0xEFFF;
            *(unsigned char *)(moby_data + 0x67) = 0x78;
            trooper_anim_update(moby, (short *)color_animation_state);
            health_depleted = 1;
        } else {
            *health = *health - damage_amount;
            *(unsigned char *)(moby_data + 0x67) = 0xFA;
            *(short *)(moby_data + 0x26) = scale_game_frames(0x3C);
            trooper_anim_update(moby, (short *)color_animation_state);
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, color_animation_state);
    return health_depleted;
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
    float health;
    short max_health;
    u8    pad26[0x4A];
    short group_timer;
    short tick_sound_timer;
    short hit;
    short opened_color_flash_timer;
    u8    flash_blue;
    u8    is_group_follower;
    u8    reported_hit_count;
    u8    enumerated_group_entry_count;
    int   group_time_limit;
    char *group_leader;
    short opened_state_duration;
} SwitchVars;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void linked_switch_group_update(char *moby) __asm__("FUN_L13_002e4448");

/* Moby class 231 */
void linked_switch_group_update(char *moby) {
    enum {
        INITIALIZE_LINKED_GROUP = 0,
        MONITOR_LINKED_GROUP_HITS = 1,
        WAIT_FOR_LINKED_GROUP_COMPLETION = 2,
        RESET_HIT_STATE = 3,
        UPDATE_OPENED_STATE = 4
    };
    SwitchVars *switch_vars = ((SwitchVars *)MOBY(moby)->pvars);
    int         hit = 0;

    if (switch_vars->hit == 0 && (unsigned char)(MOBY(moby)->state - 1) < 2) {
        hit = process_linked_switch_damage((struct Moby *)moby, switch_vars, &switch_vars->health);
    }
    switch ((unsigned char)MOBY(moby)->state) {
    case INITIALIZE_LINKED_GROUP:
        if (switch_vars->is_group_follower == 0) {
            char *iter[4];
            char *linked;

            switch_vars->enumerated_group_entry_count = 0;
            switch_vars->group_leader = moby;
            get_first_moby_in_group(iter, (unsigned char)MOBY(moby)->group, 0, 0);
            while ((linked = *iter) != 0) {
                linked = iter[0];
                if (linked != moby && *(short *)(linked + 0xA6) == 0xE7) {
                    SwitchVars *other_data = *(SwitchVars **)(linked + 0x78);

                    other_data->group_leader = moby;
                    other_data->is_group_follower = 1;
                }
                switch_vars->enumerated_group_entry_count++;
                get_next_moby_in_group(iter, iter[0], 0, 0);
            }
        }
        switch_vars->group_timer = 0;
        switch_vars->tick_sound_timer = 0;
        switch_vars->reported_hit_count = 0;
        switch_vars->hit = 0;
        switch_vars->opened_color_flash_timer = 0;

        set_moby_color(moby, 0x80, 0x80, 0x80);
        MOBY(moby)->state = MONITOR_LINKED_GROUP_HITS;
        break;
    case MONITOR_LINKED_GROUP_HITS:
    case WAIT_FOR_LINKED_GROUP_COMPLETION:
        if (switch_vars->hit == 0 && hit != 0) {
            char       *linked;
            SwitchVars *linked_data;
            linked = switch_vars->group_leader;
            linked_data = *(SwitchVars **)(linked + 0x78);
            if ((unsigned char)linked[0x20] == 1) {
                switch_vars->reported_hit_count = 0;
                linked[0x20] = 2;
                if (linked_data->group_time_limit > 0) {
                    linked_data->group_timer = scale_game_frames(switch_vars->group_time_limit);
                }
            }
            linked_data->reported_hit_count++;
            allocate_voice_for_target_entry(3, 0, moby);
            set_moby_color(moby, 0, 0xFF, 0);
            switch_vars->hit = 1;
        }
        if ((unsigned char)MOBY(moby)->state == WAIT_FOR_LINKED_GROUP_COMPLETION) {
            if (switch_vars->reported_hit_count >= switch_vars->enumerated_group_entry_count) {
                set_moby_list_state((unsigned char)MOBY(moby)->group, 4);
                allocate_voice_for_target_entry(1, 0, moby);
                switch_vars->group_timer = switch_vars->opened_state_duration;
                debug_print(
                    D_L13_001F4D90, ((short)MOBY(moby)->save_id), MOBY(moby)->oclass, frame_count);
                MOBY(moby)->state = UPDATE_OPENED_STATE;
            } else {
                if (switch_vars->group_time_limit >= 0) {
                    if (fast_dec_timer_s16(&switch_vars->group_timer) != 0) {
                        switch_vars->reported_hit_count = 0;
                        switch_vars->hit = 0;

                        allocate_voice_for_target_entry(2, 0, moby);
                        set_moby_list_state((unsigned char)MOBY(moby)->group, 3);
                        debug_print(D_L13_001F4DB0,
                                    ((short)MOBY(moby)->save_id),
                                    MOBY(moby)->oclass,
                                    frame_count);
                        MOBY(moby)->state = RESET_HIT_STATE;
                    } else if (fast_dec_timer_s16(&switch_vars->tick_sound_timer) != 0) {
                        allocate_voice_for_target_entry(0, 0, moby);
                        switch_vars->tick_sound_timer =
                            MIN(scale_game_frames(0x2D),
                                scale_game_frames(5) +
                                    switch_vars->group_timer / scale_game_frames(0x28));
                    }
                }
            }
        }
        break;
    case RESET_HIT_STATE:
        set_moby_color(moby, 0x80, 0x80, 0x80);

        switch_vars->reported_hit_count = 0;
        switch_vars->hit = 0;
        switch_vars->health = (float)switch_vars->max_health;
        MOBY(moby)->state = MONITOR_LINKED_GROUP_HITS;

        break;
    case UPDATE_OPENED_STATE:
        if (switch_vars->is_group_follower == 0 && switch_vars->group_timer != -1) {
            if (fast_dec_timer_s16(&switch_vars->group_timer) != 0) {
                switch_vars->reported_hit_count = 0;
                switch_vars->hit = 0;

                allocate_voice_for_target_entry(2, 0, moby);
                set_moby_list_state((unsigned char)MOBY(moby)->group, 3);
                debug_print(linked_switch_closed_debug_format,
                            ((short)MOBY(moby)->save_id),
                            MOBY(moby)->oclass,
                            frame_count);
                MOBY(moby)->state = RESET_HIT_STATE;
            } else if (fast_dec_timer_s16(&switch_vars->tick_sound_timer) != 0) {
                allocate_voice_for_target_entry(0, 0, moby);
                switch_vars->tick_sound_timer =
                    MIN(scale_game_frames(0x2D),
                        scale_game_frames(5) + switch_vars->group_timer / scale_game_frames(0x28));
            }
        }
        if (fast_dec_timer_s16(&switch_vars->opened_color_flash_timer) != 0) {
            switch_vars->opened_color_flash_timer = scale_game_frames(0x14);
            if (switch_vars->flash_blue != 0) {
                set_moby_color(moby, 0, 0xFF, 0);
                switch_vars->flash_blue = 0;
            } else {
                set_moby_color(moby, 0, 0, 0xFF);
                switch_vars->flash_blue = 1;
            }
        }
        break;
    }
}
