# Overlay configuration

Metadata for the 19 level programs ([docs/overlays.md](../../docs/overlays.md)).
Names, sizes, addresses and hashes only: no game bytes.

- `us/level-table.json`: each level's byte offset and size on the USA disc.
- `us/index.json`, `us/level-NN.json`: each level's records, entry point and hashes.
- `us/functions.tsv`: the function catalogue.
- `us/levels.json`: planet name and description per level (progress map).
- `us/names/level-NN.json`: naming evidence carried over from the executable.

Overlay functions never go into `us/rnc1.us.yaml` or the boot address space.
