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
#include "rnc/overlay/level_labels.h"

extern int debug_print() __asm__("FUN_001e93b0");

extern void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");

extern char moby_volume_trigger_missing_volume_format[] __asm__("D_L13_001F41F8");

void update_moby_volume_trigger(char *moby) __asm__("FUN_L13_002b0e80");

void update_moby_volume_trigger(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    if (*(int *)data == -1) {
        debug_print(moby_volume_trigger_missing_volume_format, ((short)MOBY(moby)->save_id));
        mark_moby_for_removal(moby);
        return;
    }
    if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)data) != 0) {
        if (data[0x38] == 0) {
            {
                int uid = *(int *)(data + 0x4);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x24] != -1)
                                    m[0x20] = data[0x24];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x14) != -1 && data[0x2C] != -1)
                set_moby_list_state(*(int *)(data + 0x14), data[0x2C]);
            {
                int uid = *(int *)(data + 0x8);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x25] != -1)
                                    m[0x20] = data[0x25];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x18) != -1 && data[0x2D] != -1)
                set_moby_list_state(*(int *)(data + 0x18), data[0x2D]);
            {
                int uid = *(int *)(data + 0xC);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x26] != -1)
                                    m[0x20] = data[0x26];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x1C) != -1 && data[0x2E] != -1)
                set_moby_list_state(*(int *)(data + 0x1C), data[0x2E]);
            {
                int uid = *(int *)(data + 0x10);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x27] != -1)
                                    m[0x20] = data[0x27];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x20) != -1 && data[0x2F] != -1)
                set_moby_list_state(*(int *)(data + 0x20), data[0x2F]);
            if (data[0x34] != -1)
                moby[0x20] = data[0x34];
            if (data[0x36] != -1)
                moby[0xBC] = data[0x36];
            data[0x38] = 1;
        }
    } else {
        if (data[0x38] == 1) {
            {
                int uid = *(int *)(data + 0x4);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x28] != -1)
                                    m[0x20] = data[0x28];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x14) != -1 && data[0x30] != -1)
                set_moby_list_state(*(int *)(data + 0x14), data[0x30]);
            {
                int uid = *(int *)(data + 0x8);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x29] != -1)
                                    m[0x20] = data[0x29];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x18) != -1 && data[0x31] != -1)
                set_moby_list_state(*(int *)(data + 0x18), data[0x31]);
            {
                int uid = *(int *)(data + 0xC);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x2A] != -1)
                                    m[0x20] = data[0x2A];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x1C) != -1 && data[0x32] != -1)
                set_moby_list_state(*(int *)(data + 0x1C), data[0x32]);
            {
                int uid = *(int *)(data + 0x10);
                if (uid != -1) {
                    char *m = moby_base + (uid << 8);
                    if (m != 0) {
                        unsigned char st = m[0x20];
                        if (st != 0xFE) {
                            if (st != 0xFD) {
                                if (data[0x2B] != -1)
                                    m[0x20] = data[0x2B];
                            }
                        }
                    }
                }
            }
            if (*(int *)(data + 0x20) != -1 && data[0x33] != -1)
                set_moby_list_state(*(int *)(data + 0x20), data[0x33]);
            if (data[0x35] != -1)
                moby[0x20] = data[0x35];
            if (data[0x37] != -1)
                moby[0xBC] = data[0x37];
            data[0x38] = 0;
        }
    }
}
