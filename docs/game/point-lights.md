# Point-light links

`src/rendering/lights/` groups the four boot functions that maintain point-light
links to ties, tfrags and shrubs.

| Function | USA symbol | Role |
|---|---|---|
| `update_all_point_lights` | `FUN_00201a28` | Update the directional preset and rebuild active point-light links when the existing sphere comparison exceeds 8 |
| `create_point_light` | `FUN_00201ba8` | Collect intersecting instance indices and insert the light index into their packed links |
| `refresh_point_light` | `FUN_00201f58` | Detach then recreate the links |
| `detach_point_light` | `FUN_00201f88` | Remove the index from each recorded instance and clear the three ranges |

Each light has a 0x30-byte bookkeeping slot at `D_0019C3C0`. Its first six
signed halfwords hold start/count pairs in **tie, shrub, tfrag** order; the
index buffer address follows at +0x0C. Creation fills the buffer in
**tie, tfrag, shrub** order, sharing a capacity of 0x200 signed-halfword indices.
The two orders are intentionally different. A full instance light list does
not consume an index-buffer entry.

An instance stores up to four light indices in a 16-bit word, with nibble 0xF
marking an unused entry. Detach compacts the remaining nibbles and fills the
last with 0xF. When the result is 0xFFFF, it sets the existing instance flag
byte to 1; the broader meaning of that byte remains unnamed. Tfrag records
use a 0x40-byte stride and links at +0x36; ties and shrubs use a 0x20-byte stride
and links at +0x1E.

`refresh_point_light` uses old-style declarations and calls
`detach_point_light()` without an explicit argument; these are compiler-sensitive.
