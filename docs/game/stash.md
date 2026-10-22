# IOP stash transfers

`src/storage/stash/` manages a 64-slot allocation table over an IOP buffer.
The shared EE state and entries are defined in `include/rnc/storage/stash.h`.

## Functions

| C name | USA symbol/address | What it does |
|---|---|---|
| `init_stash` | `FUN_00232d00` | Bind server 0x11, query buffer base/size, reset allocation cursor and slots |
| `stash_send_data` | `FUN_00232e40` | Submit DMA, reserve a range and return its slot; -1 for insufficient space, -2 for 64 slots already allocated |
| `stash_get_reserved_qwords` | `LookupResourceEntry`, 0x00233038 | Return the stored reservation length, including zero for an empty slot; -3 for an out-of-range slot |
| `stash_receive_data` | `FUN_00232f20` | Fetch a slot range in chunks through RPC command 1; -3 for invalid/empty slot, -1 for a range past its reservation |

The names describe what the code does, not Insomniac's original names.
`stash_get_reserved_qwords` keeps the `LookupResourceEntry` linker symbol.

## Transfers

One quadword is 16 bytes. Send's `transfer_qwords` controls DMA size;
`reserve_qwords` controls reserved space and the stored length. They can differ:
`src/overlays/shared/rendering/00245c28.c` reserves 0x4800 quadwords while
passing a separate transfer length. Send uses direct SIF DMA, not RPC command 0.
It ignores the DMA result and records the slot regardless, so a returned slot
does not guarantee transfer success or completion.

Receive's offset and count are in quadwords. Count -1 substitutes the whole
stored reservation; a nonzero offset can then fail the bounds check. Each
RPC fetch is capped at 0xFFFF quadwords, with the caller's mode unchanged.
Negative lengths, integer overflow and send transfers larger than their
reservation remain unchecked. Initialization keeps its endless wait/error loops.

## Layouts

- `stash_state` at `D_001DD1A0` is 0x38 bytes: IOP base, byte capacity,
  a 0x28-byte SIF client at +0x08, allocation cursor at +0x30 and slot count
  at +0x34.
- `stash_entries` at `D_001DD1D8` contains 64 records, totaling 0x400 bytes.
  Each 16-byte record holds an IOP address, reserved quadwords, tag and an
  uninterpreted final word. Addresses, lengths and tags stay signed 32-bit; HUD callers pass string addresses as tags.
- The receiver uses `stash_rpc_client_address`, the direct `D_001DD1A8`
  label for the client's address, to preserve its relocations.
- The table keeps its `.data` annotation; the item-preview caller keeps its
  old-style declaration so the calling code stays the same.

`src/check/stash_layout_check.c` checks the EE offsets and sizes.

## OpenRAC comparison

[OpenRAC's stash module](https://github.com/OpenRAC/rac1-decomp/blob/05da3217a4046ac01dec3ac74f051a348f063155/src/game/stash.c)
has corresponding init, send, receive and count helpers at 0x00234018,
0x00234158, 0x00234238 and 0x00234350. Its comments attribute `StashInfo` and
`StashBlock` fields to IOPSTASH.IRX debug types. That is their claim, not independent
proof of EE names: both projects share earlier research.
