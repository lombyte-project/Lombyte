#include "types.h"
#include "eetypes.h"
#include "structs.h"

typedef struct LinkObject LinkObject;
typedef struct LinkOwner LinkOwner;
struct LinkObject {
    LinkObject *next_object;
    int index;
};
struct LinkOwner {
    unsigned char padding[76];
    LinkObject **link_table;
};

void InsertLinkObject(LinkOwner *link_owner, LinkObject *link_object) __asm__("InsertLinkObject");

void InsertLinkObject(LinkOwner *link_owner, LinkObject *link_object) {
    if (link_object != 0) {
        LinkObject **link_slot = link_owner->link_table - (-link_object->index);
        link_object->next_object = *link_slot;
        *link_slot = link_object;
    }
}
