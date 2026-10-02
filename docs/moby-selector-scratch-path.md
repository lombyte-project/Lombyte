# Moby selector-to-scratch transform path
This is a static data-flow reference for the USA / NTSC-U boot executable
`SCUS_971.99`. It records confirmed relationships around the intentional
low-level assembly units `FUN_00210850` and `FUN_002109B8`, plus the C wrapper
`FUN_0020CCA8` that calls the resolver.
## Scope and terminology
The terms below describe dataflow only:
- **selector**: an integer used to reach a class-owned descriptor;
- **descriptor**: the record reached by that lookup;
- **slot**: an operational index into scratch storage;
- **scratch slot block**: a 0x40-byte block at `S[i]`;
- **M4**: a 0x40-byte, four-qword matrix-shaped input or output block in this
  path.
`M4` does not imply a bind matrix, inverse bind matrix, joint matrix, skeleton
pose, or final world-space pose.
For the notation used below:
```text
M       = Moby argument
C       = *(void **)(M + 0x24)
N       = *(u8 *)(C + 0x08)
P       = *(void **)(C + 0x18)
T       = *(void **)(C + 0x1C)
S[i]    = 0x70000000 + 0x40*i
```
## `FUN_0020CCA8`: caller context
`FUN_0020CCA8` passes its Moby argument, a count of one, a local selector, and
its output buffer to `FUN_00210850`:
```text
FUN_0020CCA8(M, selector, out)
    -> FUN_00210850(M, 1, &selector, out)
```
After that call, the wrapper performs these confirmed output-side operations:
1. It scales the fourth 16-byte vector at `out + 0x30` by
   `*(f32 *)(M + 0x2C) / 1024`.
2. It combines `out` with a copied 0x40-byte block at `M + 0xC0` through the
   existing matrix helper.
3. It adds the 16-byte vector at `M + 0x10` to `out + 0x30`.
This note does not assign a row/column convention or a multiplication order to
the matrix helper.
## `FUN_00210850`: selector, descriptor, and work selection
For a selector `q`, the resolver reaches a descriptor through `T`:
```text
desc = *(void **)(T + 4 + 4*q)
n    = *(u16 *)(desc + 0x00)
b[k] = *(u8 *)(desc + 0x04 + k), for 0 <= k < n
```
The following behavior is confirmed:
- `FUN_00210850` clears a per-call work-selection region beginning at
  `D_001B2D00`.
- Each byte `b[k]` read from the descriptor list writes a selection mark to the
  byte-indexed entry at `D_001B2D00 + b[k]` before the call to
  `FUN_002109B8`.
- The selection buffer is passed to `FUN_002109B8` as its second argument.
- For each processed descriptor, the resolver stores the byte used by its
  per-request result entry. After `FUN_002109B8` returns, that byte indexes
  the `S[slot]` block copied to the caller output.
The buffer is temporary work state for the invocation; the observed paths clear
and rebuild it before the next invocation. This note does not assign semantic
names to descriptor fields or to slot values.
## `FUN_002109B8`: per-slot class input and scratch output
The evaluator reads these Moby fields as inputs:
```text
M + 0x24    class pointer
M + 0x54    interpolation/state field; semantic type not assigned here
M + 0x68    frame input pointer
M + 0x6C    alternate frame input pointer
```
For each staged record `i`, the word at `P + 0x10*i + 0x0C` is copied to
`S[i] + 0x2C`. The sparse-update paths traced for this evaluator preserve that
word while updating adjacent scratch data.
For selected slots, the final path uses that word as an optional M4 input:
```text
base = *(void **)(S[i] + 0x2C)
base == 0      -> identity fallback
base != 0      -> read four qwords from base
compose the selected slot data with that input
write the final four qwords to S[i] + 0x00/+0x10/+0x20/+0x30
```
This establishes that `C+0x18` and `C+0x1C` serve distinct data paths:
```text
C + 0x18 -> P: per-slot 0x10-byte records and optional M4 input pointers
C + 0x1C -> T: selector-to-descriptor lookup
```
## Auxiliary list at `M + 0x64`
A separate path uses `M + 0x64` as a linked-list root. In the observed nodes:
```text
node + 0x04 -> address derived from S[tail]
node + 0x08 -> next node
```
`FUN_002109B8` accesses scratch-derived transform data through that list. The
list's higher-level role is not assigned here.
## Evidence limits
### Confirmed
- The selector lookup through `C+0x1C` reaches descriptor records.
- Descriptor bytes feed the per-call work-selection buffer at `D_001B2D00`.
- `FUN_00210850` passes that selection state to `FUN_002109B8`.
- Scratch slots use the 0x40-byte stride shown above.
- `C+0x18` reaches `P`, whose records use a 0x10-byte stride.
- `P[i]+0x0C` supplies the optional four-qword input used by the final slot
  path; zero selects the identity fallback.
- The final selected result is written as four qwords in `S[i]` and can be
  exported by `FUN_00210850`.
- `M+0x64` roots an auxiliary linked path to scratch-derived addresses.
### Not established
This note does not establish anatomical bones, joints, skeleton nodes,
parent/child hierarchy, bind matrices, inverse bind matrices, a complete pose,
final world-space pose, or skinning data.
## Reproducibility boundary
This note applies only to the supported `SCUS_971.99` USA / NTSC-U boot
executable. The expected boot-executable SHA-256 is documented in the project
README. The note records dataflow conclusions only; the retail executable,
locally generated oracle, raw comparison artifacts, and any runtime captures
remain outside the repository.
