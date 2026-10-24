#!/usr/bin/env python3
"""Build the level code compiler (SN EE-GCC 2.95.2 v2.73a) from source.

SN's ``cc1.exe`` is GCC 2.95.2 with the R5900 back end of the Cygnus EE tree.
This script rebuilds it as a native ``cc1``: the upstream gcc-2.95.2 archive,
the R5900 back end files of ``gnu-ee-binutils-gcc-1.1.tar.gz`` (the archive the
game compiler is built from), four patches from ``patches/sce-991111b/`` and the
stack in ``patches/sn-2.95.2/``. The result goes to ``tools/compilers/sn-cc1/``
and builds the ``OVERLAY_SN_UNITS`` of ``configure.py``; ``make overlays`` is the
check. SN's driver, preprocessor and ``Ps2EeAs`` are still used as they are.

Needs ``gcc -m32`` (e.g. ``gcc-multilib``), ``make`` and ``git``. Offline: pass
``--gcc-archive`` and ``--ee-archive``.
"""

from __future__ import annotations

import argparse
import importlib
import json
import os
import shutil
import subprocess
import sys
import tarfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
game = importlib.import_module("build-game-compiler")
BuildError, download, run, sha256 = game.BuildError, game.download, game.run, game.sha256

PATCHES = ROOT / "patches"

GCC_NAME = "gcc-2.95.2.tar.gz"
GCC_URLS = (
    "https://ftp.gnu.org/gnu/gcc/gcc-2.95.2.tar.gz",
    "https://mirrors.kernel.org/gnu/gcc/gcc-2.95.2.tar.gz",
    "https://ftpmirror.gnu.org/gnu/gcc/gcc-2.95.2.tar.gz",
)
GCC_SHA256 = "064e1cb06ea5d2f4a07ec46c1c64d771f74d04f404b6a6766bca2477f7d72482"
GCC_TOP = "gcc-2.95.2"

# The R5900 back end SN built on, taken whole from the EE archive.
EE_FILES = (
    "config.sub",
    "gcc/config/mips/abi64.h",
    "gcc/config/mips/elf.h",
    "gcc/config/mips/elf5900.h",
    "gcc/config/mips/elf64.h",
    "gcc/config/mips/mips.c",
    "gcc/config/mips/mips.h",
    "gcc/config/mips/mips.md",
    "gcc/config/mips/t-r5900",
)

# Application order (patches/sn-2.95.2/README.md).
PATCH_ORDER = (
    "sce-991111b/0001", "sce-991111b/0054", "sce-991111b/0064",
    "sn-2.95.2/0000", "sn-2.95.2/0001", "sn-2.95.2/0002", "sn-2.95.2/0003",
    "sn-2.95.2/0004", "sn-2.95.2/0005", "sn-2.95.2/0006",
    "sce-991111b/0062",
)

CONFIGURE = (
    "--build=i686-linux-gnu", "--host=i686-linux-gnu", "--target=mips64r5900-sf-elf",
    "--without-headers", "--disable-nls",
)
# SN's host was 32-bit Windows: a 64-bit HOST_WIDE_INT printed with msvcrt's
# "%d", and gas-style alignment directives.
HOST_CFLAGS = (
    "-O2 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -fno-stack-protector -fno-strict-aliasing"
    " -fcommon -std=gnu89 -D_GNU_SOURCE '-DHOST_WIDE_INT=long long'"
    " -DHOST_BITS_PER_WIDE_INT=64 '-DHOST_WIDE_INT_PRINT_DEC=\"%d\"'"
    " -DHAVE_GAS_MAX_SKIP_P2ALIGN -Wno-format"
)

def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("--gcc-archive", type=Path, help=f"local copy of {GCC_NAME}")
    parser.add_argument("--ee-archive", type=Path, help=f"local copy of {game.ARCHIVE_NAME}")
    parser.add_argument("--work", type=Path, default=ROOT / "build" / "sn-compiler-build",
                        help="build directory (default: build/sn-compiler-build)")
    parser.add_argument("--output", type=Path, default=ROOT / "tools" / "compilers" / "sn-cc1",
                        help="install directory (default: tools/compilers/sn-cc1)")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 1,
                        help="parallel build jobs (default: number of CPUs)")
    parser.add_argument("--check", action="store_true",
                        help="verify the archives and that the patch stack applies, without building")
    return parser.parse_args(argv)


def patch_files() -> list[Path]:
    files = []
    for entry in PATCH_ORDER:
        folder, prefix = entry.split("/")
        matches = sorted((PATCHES / folder).glob(f"{prefix}-*.patch"))
        if len(matches) != 1:
            raise BuildError(f"expected exactly one {entry}-*.patch")
        files.append(matches[0])
    return files


def archive(given: Path | None, name: str, urls: tuple[str, ...], digest: str,
            cache: Path) -> Path:
    if given is None:
        download(urls, cache / name, digest)
        return cache / name
    given = given.expanduser().resolve()
    if not given.is_file() or sha256(given) != digest:
        raise BuildError(f"{given} is missing or does not match the pinned sha256 {digest}")
    return given


def extract(gcc: Path, ee: Path, work: Path) -> Path:
    source = work / "source"
    if source.exists():
        shutil.rmtree(source)
    print(f"extracting {gcc.name}", flush=True)
    with tarfile.open(gcc) as tar:
        try:
            tar.extractall(work, filter="data")
        except TypeError:  # Python < 3.12
            tar.extractall(work)
    (work / GCC_TOP).rename(source)
    print(f"taking the R5900 back end from {ee.name}", flush=True)
    with tarfile.open(ee) as tar:
        for name in EE_FILES:
            member = tar.extractfile(f"{game.ARCHIVE_TOP}/{name}")
            if member is None:
                raise BuildError(f"{ee.name} has no {name}")
            (source / name).write_bytes(member.read())
    return source


def apply_patches(source: Path) -> None:
    # Inside this checkout git would resolve the paths against the repository
    # root and skip them all; the work folder as ceiling stops that.
    env = dict(os.environ, GIT_CEILING_DIRECTORIES=str(source.parent))
    for patch in patch_files():
        check = subprocess.run(["git", "apply", "--check", str(patch)], cwd=source, env=env,
                               capture_output=True, text=True)
        if check.returncode != 0:
            raise BuildError(f"{patch.name} does not apply: {check.stderr.strip()}")
        run(["git", "apply", str(patch)], cwd=source, env=env)
    # The parser and the gperf table ship generated; keep make from rebuilding them.
    for name in ("c-parse.c", "c-parse.h", "cexp.c", "c-gperf.h"):
        (source / "gcc" / name).touch()


def build(source: Path, work: Path, jobs: int) -> Path:
    build_dir = work / "build"
    if build_dir.exists():
        shutil.rmtree(build_dir)
    log = work / "build.log"
    env = dict(os.environ, CC="gcc -m32")
    for part, extra in (("libiberty", ()), ("gcc", ("--enable-languages=c", "--enable-haifa"))):
        (build_dir / part).mkdir(parents=True)
        run(["bash", str(source / part / "configure"), *CONFIGURE, *extra],
            cwd=build_dir / part, env=env, log=log)
    run(["make", f"-j{jobs}", "CC=gcc -m32", "CFLAGS=-O2 -fcommon -std=gnu89"],
        cwd=build_dir / "libiberty", env=env, log=log)
    run(["make", f"-j{jobs}", "LANGUAGES=c", "CC=gcc -m32", f"CFLAGS={HOST_CFLAGS}", "cc1"],
        cwd=build_dir / "gcc", env=env, log=log)
    return build_dir / "gcc" / "cc1"


def install(cc1: Path, output: Path) -> dict:
    stage = output.parent / f".{output.name}.stage"
    if stage.exists():
        shutil.rmtree(stage)
    stage.mkdir(parents=True)
    shutil.copy2(cc1, stage / "cc1")
    (stage / "cc1").chmod(0o755)
    provenance = {
        "schema": "rnc-sn-cc1-v1",
        "source": [f"{GCC_NAME} sha256 {GCC_SHA256}",
                   f"{game.ARCHIVE_NAME} sha256 {game.ARCHIVE_SHA256}: " + " ".join(EE_FILES)],
        "patches": [f"{p.parent.name}/{p.name}" for p in patch_files()],
        "patch_sha256": {f"{p.parent.name}/{p.name}": sha256(p) for p in patch_files()},
        "cc1_sha256": sha256(stage / "cc1"),
    }
    (stage / "provenance.json").write_text(json.dumps(provenance, indent=2) + "\n")
    if output.exists():
        shutil.rmtree(output)
    stage.rename(output)
    return provenance


def smoke_test(output: Path) -> None:
    probe = output / "smoke.i"
    probe.write_text("int smoke(int a) { return a * 42; }\n")
    try:
        result = subprocess.run(
            [str(output / "cc1"), "-mgas", "-msplit-addresses", str(probe), "-quiet", "-O2",
             "-o", str(output / "smoke.s")],
            capture_output=True, text=True,
        )
        if result.returncode != 0 or "smoke:" not in (output / "smoke.s").read_text():
            raise BuildError(f"the built cc1 cannot compile a test file: {result.stderr.strip()[-400:]}")
    finally:
        for name in ("smoke.i", "smoke.s"):
            (output / name).unlink(missing_ok=True)


def main(argv=None) -> int:
    args = parse_args(argv)
    if sys.platform != "linux":
        raise BuildError("the SN cc1 is a 32-bit Linux build; run this on Linux or WSL")
    for tool in ("gcc", "make", "git"):
        if shutil.which(tool) is None:
            raise BuildError(f"{tool} is not installed")
    work = args.work.expanduser().resolve()
    work.mkdir(parents=True, exist_ok=True)
    gcc = archive(args.gcc_archive, GCC_NAME, GCC_URLS, GCC_SHA256, work)
    # The game compiler build caches the EE archive; reuse it.
    ee_cache = ROOT / "build" / "game-compiler-build"
    if args.ee_archive is None and (ee_cache / game.ARCHIVE_NAME).is_file():
        args.ee_archive = ee_cache / game.ARCHIVE_NAME
    ee = archive(args.ee_archive, game.ARCHIVE_NAME, game.ARCHIVE_URLS, game.ARCHIVE_SHA256, work)
    print("archives sha256 OK")

    source = extract(gcc, ee, work)
    apply_patches(source)
    print(f"{len(PATCH_ORDER)} patches applied")
    if args.check:
        return 0

    provenance = install(build(source, work, args.jobs), args.output.expanduser().resolve())
    smoke_test(args.output.expanduser().resolve())
    print(f"\ninstalled to {args.output}\n  cc1 sha256 {provenance['cc1_sha256']}")
    print("The hash depends on the host compiler; `make overlays` is the check.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
