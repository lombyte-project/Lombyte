# `tools/`: local toolchain (not tracked)

`../setup.sh` installs everything here. Git ignores this folder except this
README. Nothing here is configurable: the build always looks at these paths.

| Path                          | What                                               |
| :---------------------------- | :------------------------------------------------- |
| `compilers/game-compiler`     | game code compiler (required)                      |
| `compilers/sdk-compiler`      | SDK library compiler                               |
| `compilers/sn-compiler`       | SN EE-GCC 2.95.2 and Ps2EeAs (assembles game code) |
| `compilers/sn-cc1`            | SN cc1 built from source (`OVERLAY_SN_UNITS`)      |
| `binutils-mips-ps2-decompals` | R5900 binutils                                     |
| `objdiff`                     | objdiff CLI                                        |

No game data or proprietary tools are stored in the repository.
