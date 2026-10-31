# Level programs

Metadata for the 19 level programs ([docs/build/overlays.md](../../../docs/build/overlays.md)).
Names, sizes, addresses and hashes only: no game bytes. Their C lives in
`src/overlays/`; their bytes, extracted from your own disc, in
`config/us/overlays/` (not in Git).

| File                 | What                                                   |
| :------------------- | :----------------------------------------------------- |
| `level-table.json`   | each level's byte offset and size on the disc          |
| `index.json`, `level-NN.json` | each level's records, entry point and hashes  |
| `functions.tsv`      | the function list                                      |
| `data.tsv`           | level data defined in C                                |
| `compiler_data.tsv`  | switch tables and float literals the compiler emits    |
| `exe_data.tsv`       | the levels' copies of executable data                  |
| `levels.json`        | planet name and description per level (progress map)   |
| `names/level-NN.json`| naming evidence carried over from the executable       |

Level functions never go into `rnc1.us.yaml` or the boot address space.
