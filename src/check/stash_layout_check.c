/* EE offsets used by FUN_00232d00, FUN_00232e40, FUN_00232f20
 * and LookupResourceEntry (0x00233038). Not linked into the game. */
#include "rnc/storage/stash.h"

#define CHECK_CAT2(a, b) a##b
#define CHECK_CAT(a, b) CHECK_CAT2(a, b)
#define CHECK(cond) typedef char CHECK_CAT(layout_check_, __LINE__)[(cond) ? 1 : -1]
#define OFFSET(type, field, off) \
    CHECK((unsigned long)&((type *)0)->field == (off))

CHECK(sizeof(struct StashState) == 0x38);
OFFSET(struct StashState, iop_base, 0x00);
OFFSET(struct StashState, capacity_bytes, 0x04);
OFFSET(struct StashState, rpc_client, 0x08);
OFFSET(struct StashState, next_iop_address, 0x30);
OFFSET(struct StashState, slot_count, 0x34);
CHECK(sizeof(struct StashEntry) == 0x10);
OFFSET(struct StashEntry, iop_address, 0x00);
OFFSET(struct StashEntry, reserved_qwords, 0x04);
OFFSET(struct StashEntry, tag, 0x08);
CHECK(sizeof(stash_entries) == 0x400);
