# Recovered names

[`config/us/recovered_names.json`](../config/us/recovered_names.json) holds
the naming evidence for boot functions. It is reference data: the build does
not read it.

- `symbols`: recovered original names with their address, unit and how well
  they fit it (`match`: `full`, `fragment`, `entry_size_unknown` or
  `no_unit`). A `full` match is stronger evidence than the others.
- `rename_proposals` (schema `rnc-function-name-proposals-v1`), one entry per
  address: `current_name` (`FUN_<address>`), `proposed_name` (or `null`),
  `logical_group` (the subsystem the progress report groups it under),
  `status` (`proposed`, `needs_review`, `unresolved`), `confidence`,
  `evidence`, `unit` and `source_path`.

Count entries from the file instead of copying numbers here. CI checks its
format and that each entry's unit and `source_path` match the config
(`scripts/check-cleanup.py`).

## Naming

- Functions are `lower_snake_case`, verb first when the role is clear.
- The C name is bound to the linker symbol with an assembler label, so the
  emitted symbol stays `FUN_<address>`:
  `s32 reentrant_read(struct _reent *, s32, s32, s32) __asm__("FUN_00116108");`
- `logical_group` is the report's grouping and does not have to match the
  file's directory. EE and VU are not group roots.
- `needs_review` or low confidence means more evidence is needed before the
  name is settled. A neighbour can hint at a group, not at what a function
  does.

Evidence can be recovered symbols, aliases already in the source, the code
itself, and callers or data that use it. The rules for names in the source
are in [code-cleanup.md](code-cleanup.md).
