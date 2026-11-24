<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/title-dark.svg">
    <img src="assets/title-light.svg" alt="RatchetAndClank-P2P" height="62">
  </picture>
</p>

<p align="center">
  <a href="docs/build/native-browser-pipeline.md"><img src="https://img.shields.io/badge/Browser-Pipeline-e0a526?style=flat-square&labelColor=0d1117" alt="Browser pipeline"></a>
  <a href="docs/build/building.md"><img src="https://img.shields.io/badge/Build-Guide-c3cbd8?style=flat-square&logo=gnubash&logoColor=c3cbd8&labelColor=0d1117" alt="Build guide"></a>
  <a href="CONTRIBUTING.md"><img src="https://img.shields.io/badge/Contributing-Guide-c3cbd8?style=flat-square&logo=github&logoColor=c3cbd8&labelColor=0d1117" alt="Contributing guide"></a>
</p>
<p align="center">
    Ratchet &amp; Clank (2002, PlayStation 2) running natively in the browser:<br>
    the game's recovered C compiled to WebAssembly, with browser services in place of the console's.
</p>

> [!WARNING]
> This repository does not include game data, executables, disc images, or proprietary toolchains.<br>
> A legitimately obtained copy of the **USA / NTSC-U** release (`SCUS_971.99`) is required.

## What this is

RatchetAndClank-P2P compiles the recovered Ratchet & Clank C to WebAssembly and
runs it in the browser with your own local USA disc image. It is not a PS2
emulator: the game's own code runs as WebAssembly, and the console's services
(graphics, movies, sound streams, memory card, pad) are browser
implementations. The goal is the full game in the browser, then two-player
peer-to-peer co-op.

**Where it is now.** The game's own boot code runs from the disc image:
- start-up, the title screen, the main menu, and the game's movies with sound;
- New Game into Veldin, with Ratchet under your control:
  - running, jumping, and the wrench;
  - quick select, the Bomb Glove, and the HUD.

Veldin's terrain, scenery and characters are drawn with WebGL at your screen's
resolution, at about 60 fps in testing.

**Not done yet:**
- gameplay sound;
- terrain fog and baked lighting;
- explosions damaging things;
- levels other than Veldin;
- saving;
- co-op.

Some code paths the game has not reached yet are still missing and stop the
game when first hit. The page then shows which function stopped it.

Details and the verified scope: [native browser pipeline](docs/build/native-browser-pipeline.md).

### Run it

Needs [Emscripten](https://emscripten.org), Node.js, Python 3, and your USA disc
image (keep it in `dumps/`, which is ignored).

```sh
git clone https://github.com/picfire/RatchetAndClank-P2P.git && cd RatchetAndClank-P2P
make browser-boot      # build the game page into build/browser
make browser-serve     # serve it at http://127.0.0.1:8777/boot.html
```

Open the page, choose your disc image, and play. The server publishes only
`build/browser`; the disc stays on your machine.

**Controls:**

| Input | Button or action |
|---|---|
| WASD / arrows | left stick |
| IJKL | right stick |
| Space / X | ✕ |
| Z | □ |
| C | ○ |
| V | △ |
| Q / E | L1 / R1 |
| Enter | START |
| F | fullscreen |

A gamepad works too, and clicking the picture lets the mouse drive the right stick.

## Supported version

| Game                   | Platform      | Region       | Boot executable |
| ---------------------- | ------------- | ------------ | --------------- |
| Ratchet & Clank (2002) | PlayStation 2 | USA / NTSC-U | `SCUS_971.99`   |

Expected SHA-256 of the boot executable:

```text
e050581032e4bb3f20341307da5b69b76f1574910519155380ea771e55c3c0c9
```

Only this release is targeted; other releases and the PlayStation 3 remaster are not supported.

## The PS2 matching build

The recovered C still rebuilds the retail executable byte for byte; the browser
build does not change that. To set up the matching toolchain:

```sh
./setup.sh --iso /path/to/your-ratchet-and-clank-usa.iso
```

Windows, macOS and other options: [docs/build/building.md](docs/build/building.md#quick-setup).
Help with names, structs, data in C and docs: [CONTRIBUTING.md](CONTRIBUTING.md).
Keep the build matching retail. Do not submit game images, extracted game data,
or proprietary compiler binaries.

## Credits

- Based on the [Lombyte](https://github.com/lombyte-project/lombyte) matching decompilation of Ratchet & Clank (GPL-3.0). This repository is a modified version of it.
- [objdiff](https://github.com/encounter/objdiff) — object-level comparison.
- [splat](https://github.com/ethteck/splat) and [spimdisasm](https://github.com/Decompollaborate/spimdisasm) — executable splitting and disassembly.
- [Himuro](https://github.com/Mikompilation/Himuro) — PS2 decompilation research and reference EE-GCC toolchain work.
- [bordplate/RC1](https://github.com/bordplate/RC1) and [OpenRAC/rac1-decomp](https://github.com/OpenRAC/rac1-decomp) — reference for function logic.
- [re-rac/rerac](https://github.com/re-rac/rerac) — reference for symbols.
- [three.js](https://threejs.org) — WebGL rendering of the world on the game page.
- The PS2 reverse-engineering and decompilation communities for the tools and research that make projects like this possible.

## License

Repository code is distributed under the [GNU General Public License v3.0](LICENSE). Code reconstructed from third-party binaries belongs to its copyright holders; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
