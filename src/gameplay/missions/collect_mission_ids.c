typedef struct {
    short id;             /* 0x00 */
    char pad02[0xE];
    unsigned short flags; /* 0x10 */
    short base;           /* 0x12 */
    short ids[8];         /* 0x14 */
    short status;         /* 0x24 */
    short sel;            /* 0x26 */
} MissionNode; /* 0x28 */

extern MissionNode *D_001A2C20[];

int collect_mission_ids(int *out, int *mask, int *nums, int arg3) __asm__("FUN_0020bc00");

int collect_mission_ids(int *out, int *mask, int *nums, int arg3) {
    MissionNode *p = D_001A2C20[0];
    int count = 0;
    int all = 1;

    *out = 0;
    if (mask != 0) {
        *mask = 0;
    }
    if (nums != 0) {
        *nums = -1;
    }
    if (p == 0) {
        return 0;
    }
    while (p->id != 0) {
        unsigned short f = p->flags;
        short t = p->status;

        if (t != 2 && !(f & 2)) {
            all = 0;
        }
        if (!(f & 2) && t != 0 && !((f & 1) && t == 2)) {
            out[count] = p->id;
            if (arg3 != 0) {
                out[count] = (p->status == 2) ? 0x523E : p->ids[p->sel];
            }
            if (mask != 0 && p->status == 2) {
                *mask |= 1 << count;
            }
            if (nums != 0) {
                *nums++ = p->base + p->sel;
            }
            count++;
        }
        p++;
    }
    if (mask != 0 && all != 0) {
        *mask |= 0x80000000;
    }
    return count;
}

extern __typeof__(collect_mission_ids) func_0020BC00 __attribute__((alias("FUN_0020bc00")));
