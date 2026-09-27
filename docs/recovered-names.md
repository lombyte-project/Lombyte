# Recovered symbols and function naming

[`config/us/recovered_names.json`](../config/us/recovered_names.json) is the catalog for recovered symbol evidence and address-bound semantic names. Addresses identify functions across source moves; the catalog's function names and logical groups provide the readable names used in source organization and progress reporting.

The catalog is reference metadata, not an input to C compilation. The progress report groups functions by report category and `logical_group`; selecting a group on decomp.dev shows its member functions. Entries marked `proposed` supply the displayed function name, while group assignments can organize functions regardless of name status. Each member retains its retail address and source path.

## Catalog structure

The file uses the top-level schema `rnc-recovered-names-v2`. Its `symbols` array contains recovered original names, addresses, configured-unit context, and match extent. The `match` value is one of `full`, `fragment`, `entry_size_unknown`, or `no_unit`; a full match provides stronger boundary evidence than a fragment or unknown extent.

The `rename_proposals` object uses schema `rnc-function-name-proposals-v1`. Each entry is anchored by virtual address and records:

- `current_name`: the canonical `FUN_<address>` linker symbol.
- `proposed_name`: the semantic C identifier, or `null` when no name is supported.
- `logical_group`: a slash-separated subsystem path.
- `status`: `proposed`, `needs_review`, or `unresolved`.
- `confidence` and `evidence`: support for the function name.
- `logical_group_confidence`: support for the subsystem assignment.
- `unit` and `source_path`: the source-unit context associated with the catalog entry.

Count the entries rather than copying figures into this file: `rename_proposals.status` records how far the proposals have been applied (`base_commit` names the commit they were applied against), `match_counts` breaks down the `symbols` array by match extent, and both are regenerated with the catalog. An unresolved function may still have a tentative logical-group assignment.

## Naming and source organization

- C function identifiers use `lower_snake_case`, with verb–object order when the function's role is clear.
- The readable C identifier is declared with a GNU assembler label that emits the existing `FUN_<address>` symbol. For example, a C declaration can bind `reentrant_read` to `FUN_00116108`. This preserves link identity and matching while allowing source references to use the semantic name.
- `logical_group` describes function and subsystem responsibilities. It is the reporting taxonomy, independent of the source file's location. EE and VU are not group roots; hardware terms remain in names only when they identify the operation.
- Functions without a useful catalog group use conservative source-module fallback groups. These fallbacks avoid EE/VU architecture roots; `unclassified` remains available when the source provides no more specific grouping evidence.
- Entries marked `needs_review`, or with low name or group confidence, require further evidence before their names are treated as settled. Nearby functions can support a group hint but do not establish a function's behavior.

Evidence may cite recovered symbols, source aliases, source inspection, Git log messages or references, and neighboring functions. The `symbols` records preserve recovered spellings and extents separately from the semantic C naming catalog.
