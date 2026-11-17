# Fork Direction

This fork targets native browser execution of the recovered Ratchet & Clank C
source, followed by genuine two-player peer-to-peer co-op. The owner chose this
route over the PCSX2 co-op route on 2026-11-17.

- Compile recovered game C to WebAssembly. Replace PS2-specific services with
  browser implementations, not a CPU or console emulator.
- Do not add, download, embed, or investigate Play!, PCSX2, other emulators,
  MIPS interpreters, or instruction-level PS2 runtimes as an alternative route.
  A change to this constraint requires explicit user approval. The paused PCSX2
  co-op work in `coop/` and
  [pcsx2-coop-pipeline.md](docs/build/pcsx2-coop-pipeline.md) is kept for
  reference; do not extend it without that approval.
- Do not substitute a mini-game, remote video stream, or launcher for the game.
  The parent directory's HTML prototype is outside this project's runtime.
- Follow the [native browser pipeline](docs/build/native-browser-pipeline.md).
  Work on one stage at a time; validate its gate before advancing. Record
  blockers instead of changing architecture or declaring incomplete work done.
- Reuse original C logic. Keep browser adapters and build configuration separate;
  do not mass-edit recovered sources, symbols, or original build metadata.
- Preserve the existing PS2 matching build. Its tools and byte-matching checks
  do not produce or validate a browser build. Consult
  [CONTRIBUTING.md](CONTRIBUTING.md) when changing recovered sources.
- Keep the ISO, extracted game data, and generated binaries local and ignored.
  Never serve the checkout root or upload game data to a signaling server.
- Report what actually ran, the exact validation command, and what is still
  unverified. A compiled helper is not a booted game; two networked inputs are
  not two playable characters.
