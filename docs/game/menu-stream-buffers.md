# Menu stream buffers

`src/ui/menus/stream_buffers/` owns the five descriptors used by menu images,
previews, prompts, save screens and the map. The C names describe what the code
does; the linker symbols stay the same.

## Functions

| Function | USA address | What it does |
|---|---|---|
| `initialize_menu_stream_buffers` | 0x00225AC0 | Mode zero initializes one small buffer and clears four slots; nonzero initializes three small and two large buffers |
| `acquire_menu_stream_buffer` | 0x00225C18 | Acquire and fill the first available buffer of the requested size class; return its address or zero |
| `release_menu_stream_buffer` | 0x00225CD8 | Clear allocation/read flags on an allocated buffer; always return zero, including when not found |
| `get_menu_stream_buffer_size` | 0x00225D88 | Return the matching descriptor's capacity in bytes, or -1 if not found |
| `mark_menu_stream_buffer_read_active` | 0x00225DD8 | Set the read flag on the first address match; return zero, or one if not found |
| `clear_menu_stream_buffer_read_active` | 0x00225E20 | Clear the read flag on the first address match; return zero, or one if not found |

## Flags and layout

`MenuStreamBuffer` is eight bytes: signed 32-bit address at +0 and flags at +4.
The five-entry table at `D_001D60B8` occupies 0x28 bytes; its layout is checked
in `src/check/menu_stream_buffer_layout_check.c`.

- `LARGE` (1) selects capacity 0x4F000 bytes; otherwise capacity is 0x11800.
  Acquire selects small buffers for argument zero and large buffers for any
  nonzero argument. It fills the selected buffer with 0xDEADBEEF.
- `ALLOCATED` (2) prevents acquisition. Release clears it without freeing the
  backing memory. Initialization takes backing addresses from `menu_system`.
- `READ_ACTIVE` (4) is marked by read callers and cleared after reads. If both
  allocation and read flags are set, release clears the read flag and, when
  `menu_system.pending_buffer` is nonzero, requests a stream break and clears
  that pending flag. Release does not establish successful I/O completion.

Size and read-flag helpers match addresses without checking allocation or
rejecting zero. Callers keep their existing types and old-style prototypes, because
changing them changes the generated code.
