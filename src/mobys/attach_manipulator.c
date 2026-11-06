#include "types.h"

typedef struct Manipulator Manipulator;

typedef struct MobyAttachOwner {
    u8           pad0[0x24];
    void        *pclass;
    u8           pad28[0x3C];
    Manipulator *manipulator_head;
} MobyAttachOwner;

struct Manipulator {
    u8    joint_list; /* index into the class joint lists */
    u8    attached;
    u8    pad2[2];
    void *target; /* scratchpad matrix of the list's joint */
    void *next;
    u8    padC[0x10];
    f32   rotation_w; /* the rotation quaternion's w */
    f32   scale_x;
    f32   scale_y;
    f32   scale_z;
};

void attach_manipulator(MobyAttachOwner *owner,
                        s32              slot_index,
                        Manipulator     *manip) __asm__("FUN_0020cb10");

void attach_manipulator(MobyAttachOwner *source_moby, s32 slot_index, Manipulator *manip) {
    if (manip->attached == 0) {
        u8 *slot_record;

        manip->joint_list = slot_index;
        manip->attached = 1;
        manip->rotation_w = 1.0f;
        manip->scale_x = 1.0f;
        manip->scale_y = 1.0f;
        manip->scale_z = 1.0f;

        slot_record = *(
            u8 **)((u8 *)(*(s32 *)((u8 *)source_moby->pclass + 0x1C) + manip->joint_list * 4) + 4);
        {
            u8 record_byte_offset = *slot_record;
            manip->target = (void *)((slot_record[record_byte_offset + 4] << 6) + 0x70000000);
        }
        manip->next = source_moby->manipulator_head;
        source_moby->manipulator_head = manip;
    }
}

/* Recovered original symbol name. */
extern __typeof__(attach_manipulator) AttachManipulator __attribute__((alias("FUN_0020cb10")));
