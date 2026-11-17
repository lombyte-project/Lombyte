# PCSX2 Co-op Pipeline

> **Paused on 2026-11-17.** The owner chose the
> [native browser route](native-browser-pipeline.md). This route and `coop/` are kept for
> reference; extending them needs the owner's approval.

## Summary

The unmodified game runs in PCSX2. Co-op is added from outside and, later, from
inside the game:

- **Companion process (`coop/`).** A Node program per player reads and writes game
  memory through PCSX2's PINE socket and exchanges state with the other player.
- **In-game code (later stages).** C compiled for the PS2 against this repo's
  recovered headers, patched into the game to spawn and animate the second
  player and to hook game events.

Each player runs their own PCSX2, BIOS dump, and disc image. The recovered
source supplies addresses and struct layouts; `coop/game-memory.mjs` names the
header each one came from.

The native browser route is paused; see
[native-browser-pipeline.md](native-browser-pipeline.md) and the
`browser-native-wip` branch.

## Setup

1. Install PCSX2 and configure it with a BIOS dumped from your own console.
2. Enable PINE: Settings → Advanced → PINE Settings → Enable, slot `28011`.
   (Advanced settings must be shown first, under Tools.)
3. Boot `dumps/` disc image and load into a level.
4. `node coop/spike-mirror.mjs` prints Ratchet's live position.

A second instance for local testing needs a different PINE slot (for example
`28012`) and its own PCSX2 data directory.

## Stages And Gates

### 1. Read Live Game State

`coop/pine.mjs` speaks PINE; `coop/spike-mirror.mjs` prints the hero position.

Gate: with the game running, the printed position follows Ratchet as he moves.

### 2. Drive An Object In A Second Instance

`node coop/spike-mirror.mjs --to 28012 --moby <index>` writes instance A's hero
position into one of instance B's mobys (`--mobys <count>` lists candidates).

Gate: the chosen object in B visibly follows Ratchet in A. The object's own
update may fight the writes; that is acceptable here and is fixed in stage 3.

### 3. Spawn A Remote-Player Puppet

Patch in-game C that spawns a moby with Ratchet's model, no update of its own,
and applies position, rotation, and animation received from the companion.

Gate: two instances in the same level each show the other player animating.

### 4. Peer-To-Peer Transport

Replace the same-machine link with a network connection between companions.

Gate: two separate machines show each other's player in the same level. No disc
data, BIOS data, or memory dumps leave either machine; only player state does.

### 5. Shared World State

Sync enemies, crates, bolts, and mission progress one system at a time, with one
player authoritative.

Gate per system: both players observe the same outcome for that system.

## Current Progress (2026-11-15)

- Stage 1 tooling is written. `node --test coop/` passes against a stand-in
  socket server that implements the PINE framing as the client understands it.
- **Not yet run against PCSX2.** PCSX2 is not installed on the development
  machine and no BIOS is present, so the stage 1 gate is open. The PINE wire
  format and socket path are written from the protocol's documentation, not
  confirmed against a live emulator.
- `MOBY_TABLE_PTR` comes from a shared-overlay label and is assumed, not
  verified, to hold in every level.
