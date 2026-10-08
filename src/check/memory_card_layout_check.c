/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/storage/memory_card/memory_card_state.h"

SIZE_CHECK(entry, struct MemoryCardSaveEntry, 0x1C);
OFFSET_CHECK(card_save_index, struct MemoryCard, save_index, 0x14);
OFFSET_CHECK(card_scan_save_index, struct MemoryCard, scan_save_index, 0x18);
OFFSET_CHECK(card_sync_result, struct MemoryCard, sync_result, 0x1C);
OFFSET_CHECK(card_entries, struct MemoryCard, entries, 0x20);
OFFSET_CHECK(card_errors, struct MemoryCard, errors, 0xAC);
OFFSET_CHECK(card_record_size, struct MemoryCard, record_size, 0xB4);
OFFSET_CHECK(cmd, struct MemoryCardState, cmd, 0xB8);
OFFSET_CHECK(active_card, struct MemoryCardState, active_card, 0xC0);
OFFSET_CHECK(record_index, struct MemoryCardState, record_index, 0xC8);
OFFSET_CHECK(state, struct MemoryCardState, state, 0xD4);
OFFSET_CHECK(pending_state, struct MemoryCardState, pending_state, 0xDC);
OFFSET_CHECK(err, struct MemoryCardState, err, 0xE4);
OFFSET_CHECK(buf, struct MemoryCardState, buf, 0xEC);
SIZE_CHECK(state, struct MemoryCardState, 0xF8);
