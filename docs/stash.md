# IOP stash transfers

Read `init_stash.c`, `stash_send_data.c`, `stash_get_reserved_qwords.c`, then
`stash_receive_data.c` under `src/storage/stash/`. Together they manage a
64-slot allocation table over an IOP buffer. `include/rnc/storage/stash.h`
defines their shared EE bookkeeping. HUD, map and item-preview code consume
these helpers; the SIF SDK remains responsible for transport.

## Entry points

| C name | USA symbol/address | Contract |
|---|---|---|
| `init_stash` | `FUN_00232d00` | Bind server 0x11, query buffer base/size, reset allocation cursor and slots |
| `stash_send_data` | `FUN_00232e40` | Submit DMA, reserve a range and return its slot; -1 for insufficient space, -2 for 64 slots already allocated |
| `stash_get_reserved_qwords` | `LookupResourceEntry`, 0x00233038 | Return the stored reservation length, including zero for an empty slot; -3 for an out-of-range slot |
| `stash_receive_data` | `FUN_00232f20` | Fetch a slot range in chunks through RPC command 1; -3 for invalid/empty slot, -1 for a range past its reservation |

These descriptive C names do not claim original Insomniac EE spellings.
The former `LookupResourceEntry` C name suggested a generic resource lookup;
it returns a stash length. Its linker symbol stays unchanged. Both C callers
use the new spelling. The send and length helpers now live with the initializer
and receiver; their boot link order and overlay placements stay unchanged.

## Units and ownership

`stash_state` at `D_001DD1A0` owns the IOP buffer's base, byte capacity, RPC
client, next free IOP address and allocated-slot count. `stash_entries` at
`D_001DD1D8` has a 16-byte record per slot: IOP address, reserved quadwords,
tag and an uninterpreted final word. Addresses and lengths keep their signed
32-bit access types. Tags are stored unchanged; HUD callers pass string
addresses, but this does not justify converting the stored field to a pointer.

One quadword is 16 bytes. Send's `transfer_qwords` controls DMA size, while
`reserve_qwords` controls the allocation cursor and stored length. They are
not interchangeable: `src/overlays/shared/rendering/00245c28.c` reserves
0x4800 quadwords while passing a separate transfer length. The function ignores
the DMA submission result and records the slot regardless. A returned slot
therefore does not establish successful transfer or completion.

Receive's offset and count are in quadwords. Count -1 substitutes the **whole
stored reservation**, not the remaining length after the offset; a nonzero
offset can then fail the bounds check. Each RPC fetch is capped at 0xFFFF
quadwords. The caller's RPC mode is passed through unchanged. No additional
wait or validation is introduced. Negative lengths, integer overflow, and a
transfer larger than its reservation remain retail preconditions, not repaired
behavior. Initialization retains its infinite wait/error loops.

## Layout and matching constraints

The state is 0x38 bytes; its 0x28-byte SIF client begins at +0x08, followed by
the allocation cursor at +0x30 and slot count at +0x34. The 64-entry table is
0x400 bytes. `src/check/stash_layout_check.c` checks these EE layouts.

The receiver retains the direct `D_001DD1A8` client label through
`stash_rpc_client_address`; changing it to a field expression can change
relocations. The table declaration retains its explicit `.data` annotation
from the former resource-table view. Existing aliases, SDK declarations,
access widths, expression order and compiler routes are preserved. The
item-preview caller keeps its old-style declaration to avoid an incidental
calling-convention change.

## Research limits

[OpenRAC's PAL stash module at 05da3217](https://github.com/OpenRAC/rac1-decomp/blob/05da3217a4046ac01dec3ac74f051a348f063155/src/game/stash.c)
has corresponding init, send, receive and count helpers at 0x00234018,
0x00234158, 0x00234238 and 0x00234350. Its comments attribute `StashInfo` and
`StashBlock` fields to IOPSTASH.IRX debug types. This is an upstream attribution,
not a new inspection of that IRX or independent proof of EE symbol names.
Shared research ancestry also limits the independence of the PAL comparison.
The current USA accesses and callers establish the names used here.

[OpenGOAL's project overview at 6076aab5](https://github.com/open-goal/jak-project/blob/6076aab5a95955443a9e1ed8f72a25bc0e2fcf9d/docs/project-overview.md)
separates game runtime, SDK interfaces and IOP driver responsibilities. That is
a useful organizational example: this module belongs under storage, while
its SIF calls remain SDK operations. Jak's OVERLORD is not evidence for R&C
stash layouts or function names. No external code is imported.

The [development-build notes](engine-source-layout.md) describe stash RPC
commands. This EE send helper uses direct SIF DMA, not RPC command 0.

## Qualification

The four module units and the renamed item-preview caller require focused
comparisons. Changes to shared layouts and unit paths also require final boot
and overlay gates, public checks, and an independent fresh rebuild/review.
Host layout checks or similarity scores alone cannot establish exactness.
This cleanup claims no new source recovery or native gameplay validation.
