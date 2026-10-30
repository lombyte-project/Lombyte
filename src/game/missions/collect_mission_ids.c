typedef struct {
    short          id; /* 0x00 */
    char           pad02[0xE];
    unsigned short flags;  /* 0x10 */
    short          base;   /* 0x12 */
    short          ids[8]; /* 0x14 */
    short          status; /* 0x24 */
    short          sel;    /* 0x26 */
} MissionNode;             /* 0x28 */

extern MissionNode *mission_id_collection_node_source[] __asm__("D_001A2C20");

int collect_mission_ids(int *out,
                        int *mask,
                        int *nums,
                        int  use_selected_ids) __asm__("FUN_0020bc00");

int collect_mission_ids(int *out, int *mask, int *nums, int use_selected_ids) {
    MissionNode *mission_node_cursor = mission_id_collection_node_source[0];
    int          count = 0;
    int          all = 1;

    *out = 0;
    if (mask != 0) {
        *mask = 0;
    }
    if (nums != 0) {
        *nums = -1;
    }
    if (mission_node_cursor == 0) {
        return 0;
    }
    while (mission_node_cursor->id != 0) {
        unsigned short f = mission_node_cursor->flags;
        short          t = mission_node_cursor->status;

        if (t != 2 && !(f & 2)) {
            all = 0;
        }
        if (!(f & 2) && t != 0 && !((f & 1) && t == 2)) {
            out[count] = mission_node_cursor->id;
            if (use_selected_ids != 0) {
                out[count] = (mission_node_cursor->status == 2)
                                 ? 0x523E
                                 : mission_node_cursor->ids[mission_node_cursor->sel];
            }
            if (mask != 0 && mission_node_cursor->status == 2) {
                *mask |= 1 << count;
            }
            if (nums != 0) {
                *nums++ = mission_node_cursor->base + mission_node_cursor->sel;
            }
            count++;
        }
        mission_node_cursor++;
    }
    if (mask != 0 && all != 0) {
        *mask |= 0x80000000;
    }
    return count;
}

extern __typeof__(collect_mission_ids) func_0020BC00 __attribute__((alias("FUN_0020bc00")));
