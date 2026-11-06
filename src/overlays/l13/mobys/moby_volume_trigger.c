/* Level 13 moby volume trigger class. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"

#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
#include "rnc/overlay/shared/world/level_labels.h"

extern int debug_print() __asm__("FUN_001e93b0");

extern void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");

extern char moby_volume_trigger_missing_volume_format[] __asm__("D_L13_001F41F8");

void moby_volume_trigger_update(char *moby) __asm__("FUN_L13_002b0e80");

typedef struct MobyVolumeTriggerVars {
    u8  pad_0[0x4];
    s32 first_moby;
    s32 second_moby;
    s32 third_moby;
    s32 fourth_moby;
    s32 first_moby_list;
    s32 second_moby_list;
    s32 third_moby_list;
    s32 fourth_moby_list;
    s8  first_moby_entry_state;
    s8  second_moby_entry_state;
    s8  third_moby_entry_state;
    s8  fourth_moby_entry_state;
    s8  first_moby_exit_state;
    s8  second_moby_exit_state;
    s8  third_moby_exit_state;
    s8  fourth_moby_exit_state;
    s8  first_list_entry_state;
    s8  second_list_entry_state;
    s8  third_list_entry_state;
    s8  fourth_list_entry_state;
    s8  first_list_exit_state;
    s8  second_list_exit_state;
    s8  third_list_exit_state;
    s8  fourth_list_exit_state;
    s8  trigger_entry_state;
    s8  trigger_exit_state;
    s8  trigger_entry_command;
    s8  trigger_exit_command;
    s8  hero_inside;
} MobyVolumeTriggerVars;

/* Moby class 6 */
void moby_volume_trigger_update(char *moby) {
    MobyVolumeTriggerVars *volume_trigger_vars = (MobyVolumeTriggerVars *)MOBY(moby)->pvars;
    if (*(int *)volume_trigger_vars == -1) {
        debug_print(moby_volume_trigger_missing_volume_format, ((short)MOBY(moby)->save_id));
        mark_moby_for_removal(moby);
        return;
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)volume_trigger_vars) != 0) {
        if (volume_trigger_vars->hero_inside == 0) {
            {
                int uid = volume_trigger_vars->first_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->first_moby_entry_state != -1)
                                    m[0x20] = volume_trigger_vars->first_moby_entry_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->first_moby_list != -1 &&
                volume_trigger_vars->first_list_entry_state != -1)
                set_moby_list_state(volume_trigger_vars->first_moby_list,
                                    volume_trigger_vars->first_list_entry_state);
            {
                int uid = volume_trigger_vars->second_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->second_moby_entry_state != -1)
                                    m[0x20] = volume_trigger_vars->second_moby_entry_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->second_moby_list != -1 &&
                volume_trigger_vars->second_list_entry_state != -1)
                set_moby_list_state(volume_trigger_vars->second_moby_list,
                                    volume_trigger_vars->second_list_entry_state);
            {
                int uid = volume_trigger_vars->third_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->third_moby_entry_state != -1)
                                    m[0x20] = volume_trigger_vars->third_moby_entry_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->third_moby_list != -1 &&
                volume_trigger_vars->third_list_entry_state != -1)
                set_moby_list_state(volume_trigger_vars->third_moby_list,
                                    volume_trigger_vars->third_list_entry_state);
            {
                int uid = volume_trigger_vars->fourth_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->fourth_moby_entry_state != -1)
                                    m[0x20] = volume_trigger_vars->fourth_moby_entry_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->fourth_moby_list != -1 &&
                volume_trigger_vars->fourth_list_entry_state != -1)
                set_moby_list_state(volume_trigger_vars->fourth_moby_list,
                                    volume_trigger_vars->fourth_list_entry_state);
            if (volume_trigger_vars->trigger_entry_state != -1)
                MOBY(moby)->state = volume_trigger_vars->trigger_entry_state;
            if (volume_trigger_vars->trigger_entry_command != -1)
                MOBY(moby)->command = volume_trigger_vars->trigger_entry_command;
            volume_trigger_vars->hero_inside = 1;
        }
    } else {
        if (volume_trigger_vars->hero_inside == 1) {
            {
                int uid = volume_trigger_vars->first_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->first_moby_exit_state != -1)
                                    m[0x20] = volume_trigger_vars->first_moby_exit_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->first_moby_list != -1 &&
                volume_trigger_vars->first_list_exit_state != -1)
                set_moby_list_state(volume_trigger_vars->first_moby_list,
                                    volume_trigger_vars->first_list_exit_state);
            {
                int uid = volume_trigger_vars->second_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->second_moby_exit_state != -1)
                                    m[0x20] = volume_trigger_vars->second_moby_exit_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->second_moby_list != -1 &&
                volume_trigger_vars->second_list_exit_state != -1)
                set_moby_list_state(volume_trigger_vars->second_moby_list,
                                    volume_trigger_vars->second_list_exit_state);
            {
                int uid = volume_trigger_vars->third_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->third_moby_exit_state != -1)
                                    m[0x20] = volume_trigger_vars->third_moby_exit_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->third_moby_list != -1 &&
                volume_trigger_vars->third_list_exit_state != -1)
                set_moby_list_state(volume_trigger_vars->third_moby_list,
                                    volume_trigger_vars->third_list_exit_state);
            {
                int uid = volume_trigger_vars->fourth_moby;
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (volume_trigger_vars->fourth_moby_exit_state != -1)
                                    m[0x20] = volume_trigger_vars->fourth_moby_exit_state;
                            }
                        }
                    }
                }
            }
            if (volume_trigger_vars->fourth_moby_list != -1 &&
                volume_trigger_vars->fourth_list_exit_state != -1)
                set_moby_list_state(volume_trigger_vars->fourth_moby_list,
                                    volume_trigger_vars->fourth_list_exit_state);
            if (volume_trigger_vars->trigger_exit_state != -1)
                MOBY(moby)->state = volume_trigger_vars->trigger_exit_state;
            if (volume_trigger_vars->trigger_exit_command != -1)
                MOBY(moby)->command = volume_trigger_vars->trigger_exit_command;
            volume_trigger_vars->hero_inside = 0;
        }
    }
}
